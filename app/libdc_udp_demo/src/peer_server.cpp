// peer_client_rt.cpp
// Based on your peer_client_fixed.cpp but with RT-friendly thread design
#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <queue>
#include <cassert>
#include <pthread.h>
#include <sched.h>

#include "udp_signaling.hpp"
#include "rtc/rtc.hpp"

// ---------- ENUM COMMAND ----------
typedef enum {
    CMD_NONE            = 0x00,
    CMD_PING            = 0x01,
    CMD_GET_STATUS      = 0x02,
    CMD_GET_SENSOR      = 0x03,

    CMD_SET_SPEED       = 0x10,
    CMD_SET_DIRECTION   = 0x11,
    CMD_SET_STEERING    = 0x12,
    CMD_SET_BRAKE       = 0x13,

    CMD_START_MOTOR     = 0x20,
    CMD_STOP_MOTOR      = 0x21,

    CMD_MAX
} Command_t;

// ---------- ACK ENUM ----------
typedef enum {
    ACK_OK              = 0x00,
    ACK_INVALID_CMD     = 0x01,
    ACK_INVALID_PAYLOAD = 0x02,
    ACK_CRC_ERROR       = 0x03,
    ACK_BUSY            = 0x04,
    ACK_FAIL            = 0x05
} AckCode_t;

// ---------- Frame Struct ----------
typedef struct {
    uint8_t sof;
    uint8_t msg_id;
    uint8_t cmd;
    uint8_t length;
    uint8_t payload[256];
    uint8_t checksum;
} Frame_t;

// ---------- Checksum ----------
uint8_t calc_checksum(const Frame_t& f) {
    uint8_t sum = f.sof ^ f.msg_id ^ f.cmd ^ f.length;
    for (int i = 0; i < f.length; i++) sum ^= f.payload[i];
    return sum;
}

// ---------- Build Frame ----------
std::vector<uint8_t> build_frame(uint8_t msg_id, uint8_t cmd, const uint8_t* payload, uint8_t len) {
    Frame_t f{};
    f.sof = 0xAA;
    f.msg_id = msg_id;
    f.cmd = cmd;
    f.length = len;

    if (len > 0 && payload != nullptr) {
        memcpy(f.payload, payload, len);
    }

    f.checksum = calc_checksum(f);

    std::vector<uint8_t> out;
    out.reserve(4 + len + 1);
    out.push_back(f.sof);
    out.push_back(f.msg_id);
    out.push_back(f.cmd);
    out.push_back(f.length);
    for (int i = 0; i < len; i++) out.push_back(f.payload[i]);
    out.push_back(f.checksum);
    return out;
}

// ---------- Parse Frame ----------
bool parse_frame(const uint8_t* buf, size_t size, Frame_t& out) {
    if (size < 5) return false;
    if (buf[0] != 0xAA) return false;

    out.sof = buf[0];
    out.msg_id = buf[1];
    out.cmd = buf[2];
    out.length = buf[3];

    if (4 + out.length + 1 != size) return false;

    memcpy(out.payload, &buf[4], out.length);
    out.checksum = buf[4 + out.length];

    return out.checksum == calc_checksum(out);
}

// ---------- Base64 ----------
static const std::string b64_table =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const std::string &in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val<<8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(b64_table[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) out.push_back(b64_table[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

std::string base64_decode(const std::string &in) {
    std::vector<int> T(256,-1);
    for (int i=0;i<64;i++) T[(unsigned char)b64_table[i]] = i;
    std::string out;
    int val=0, valb=-8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

// ---------- Helper: convert vector<uint8_t> -> rtc::binary (vector<std::byte>) ----------
static rtc::binary vec_u8_to_rtcbin(const std::vector<uint8_t>& v) {
    rtc::binary out(v.size());
    for (size_t i = 0; i < v.size(); ++i) out[i] = std::byte(v[i]);
    return out;
}

// ------------------ RT Utilities: simple SPSC ring and pools ------------------
// Simple single-producer single-consumer ring for pointers to slots.
// Not generalized lockfree queue with ABA protections — simple and works for typical SPSC usage.
template<typename T, size_t N>
class SPSC_Ring {
public:
    SPSC_Ring() {
        static_assert((N & (N-1)) == 0, "N must be power of two");
        head.store(0);
        tail.store(0);
        for (size_t i=0;i<N;i++) buf[i] = nullptr;
    }
    bool push(T* item) {
        size_t h = head.load(std::memory_order_relaxed);
        size_t next = (h + 1) & (N-1);
        if (next == tail.load(std::memory_order_acquire)) return false; // full
        buf[h] = item;
        head.store(next, std::memory_order_release);
        return true;
    }
    bool pop(T*& item) {
        size_t t = tail.load(std::memory_order_relaxed);
        if (t == head.load(std::memory_order_acquire)) return false; // empty
        item = buf[t];
        tail.store((t + 1) & (N-1), std::memory_order_release);
        return true;
    }
    bool empty() const {
        return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
    }
private:
    T* buf[N];
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
};

// Preallocated slot for Rx/Tx to avoid heap on fast path
struct Slot {
    size_t len;
    uint8_t data[512];
};

// Pools and rings
static const size_t RING_SIZE = 256; // must be power of two
using SlotRing = SPSC_Ring<Slot, RING_SIZE>;
static Slot rxPoolStorage[RING_SIZE];
static Slot txPoolStorage[RING_SIZE];
static std::atomic<size_t> rxPoolFreeIndex{0};
static std::atomic<size_t> txPoolFreeIndex{0};

static SlotRing rxRing; // WebRTC callback -> rx_handler
static SlotRing txRing; // control loop -> tx_dispatcher

// Try to allocate a slot from simple pool (wrap-around). Not thread-safe multi-producer; we use it from single producer contexts.
Slot* alloc_rx_slot() {
    // simple lock-free-ish circular allocation: may overwrite if more producers — but here single producer expected.
    size_t idx = rxPoolFreeIndex.fetch_add(1);
    return &rxPoolStorage[idx % RING_SIZE];
}
Slot* alloc_tx_slot() {
    size_t idx = txPoolFreeIndex.fetch_add(1);
    return &txPoolStorage[idx % RING_SIZE];
}

// ------------------ Thread priority helper (Linux) ------------------
void set_thread_realtime(std::thread &thr, int priority, int cpu = -1) {
#ifdef __linux__
    pthread_t handle = thr.native_handle();
    sched_param sch;
    sch.sched_priority = priority;
    if (pthread_setschedparam(handle, SCHED_FIFO, &sch) != 0) {
        std::cerr << "[WARN] failed to set SCHED_FIFO (need CAP_SYS_NICE?).\n";
    }
    if (cpu >= 0) {
        cpu_set_t cpus;
        CPU_ZERO(&cpus);
        CPU_SET(cpu, &cpus);
        if (pthread_setaffinity_np(handle, sizeof(cpu_set_t), &cpus) != 0) {
            std::cerr << "[WARN] failed to set thread affinity\n";
        }
    }
#else
    (void)thr; (void)priority; (void)cpu;
#endif
}

// ------------------ Simple logger thread (non-RT) ------------------
std::mutex logMutex;
std::condition_variable logCv;
std::queue<std::string> logQueue;
std::atomic<bool> stopping{false};

void log_enqueue(const std::string &s) {
    {
        std::lock_guard<std::mutex> g(logMutex);
        logQueue.push(s);
    }
    logCv.notify_one();
}

void logger_thread_fn() {
    while (!stopping.load()) {
        std::unique_lock<std::mutex> lk(logMutex);
        logCv.wait_for(lk, std::chrono::milliseconds(200), []{return !logQueue.empty() || stopping.load();});
        while (!logQueue.empty()) {
            std::string s = logQueue.front();
            logQueue.pop();
            lk.unlock();
            std::cout << s << std::endl;
            lk.lock();
        }
    }
    // flush leftover
    std::lock_guard<std::mutex> g(logMutex);
    while (!logQueue.empty()) {
        std::cout << logQueue.front() << std::endl;
        logQueue.pop();
    }
}

// ------------------ Global references (for threads) ------------------
std::shared_ptr<rtc::DataChannel> g_dc;
std::shared_ptr<rtc::PeerConnection> g_pc;

// ------------------ Threads ------------------

// RX handler: parse frames popped from rxRing and push to processing (here we will print ACKs)
void rx_handler_thread_fn() {
    // This thread should be RT priority in deployment
    while (!stopping.load()) {
        Slot* s = nullptr;
        if (rxRing.pop(s)) {
            // parse
            Frame_t rx{};
            bool ok = parse_frame(s->data, s->len, rx);
            if (!ok) {
                log_enqueue("[RX] Invalid frame");
            } else {
                // Example: If this is an ACK, log minimal info (use log thread)
                char buf[128];
                snprintf(buf, sizeof(buf), "[RX] Got cmd=0x%02X msg_id=%u ack_code=%u", rx.cmd, rx.msg_id, rx.payload[0]);
                log_enqueue(buf);
                // If needed, push to other RT queues (not implemented here)
            }
        } else {
            // nothing — small sleep to avoid busyspin; in real RT, prefer blocking wait
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

// Control loop: periodic, build status frames and/or commands and push to txRing
void control_loop_thread_fn(int period_ms = 10) {
    // RT priority recommended
    auto next = std::chrono::steady_clock::now();
    uint8_t seq = 1;
    while (!stopping.load()) {
        next += std::chrono::milliseconds(period_ms);

        // Example: build a periodic telemetry/status frame (here we simulate)
        uint8_t payload[2];
        payload[0] = 0x55; // example data
        payload[1] = seq++;

        auto frame_vec = build_frame(seq, CMD_GET_STATUS, payload, 2);

        Slot* s = alloc_tx_slot();
        s->len = frame_vec.size();
        memcpy(s->data, frame_vec.data(), s->len);

        // push to txRing; if full, drop (or implement backpressure)
        if (!txRing.push(s)) {
            log_enqueue("[TX] txRing full, dropping frame");
        }

        // wait until next period
        std::this_thread::sleep_until(next);
    }
}

// TX dispatcher: pop from txRing and call dc->send (run as RT)
void tx_dispatcher_thread_fn() {
    while (!stopping.load()) {
        Slot* s = nullptr;
        if (txRing.pop(s)) {
            if (g_dc) {
                try {
                    rtc::binary bin = vec_u8_to_rtcbin(std::vector<uint8_t>(s->data, s->data + s->len));
                    g_dc->send(bin);
                    // minimal logging
                    // avoid expensive formatting on RT path
                } catch (const std::exception &e) {
                    std::string msg = std::string("[TX] send exception: ") + e.what();
                    log_enqueue(msg);
                } catch (...) {
                    log_enqueue("[TX] send unknown exception");
                }
            } else {
                log_enqueue("[TX] no DataChannel available");
            }
        } else {
            // nothing to send
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

// ------------------ MAIN ------------------
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: peer_client <server_ip> <server_port>\n";
        return 1;
    }

    std::string server_ip = argv[1];
    int server_port = std::stoi(argv[2]);

    const int CLIENT_PORT = 6001;
    std::cout << "[Client] Server=" << server_ip << ":" << server_port
              << " local_port=" << CLIENT_PORT << std::endl;

    UdpSignaling signaling("", CLIENT_PORT, server_ip, server_port);
    signaling.start();

    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    auto pc = std::make_shared<rtc::PeerConnection>(config);
    g_pc = pc;

    // start logger thread
    std::thread logger_t(logger_thread_fn);

    // --- Signaling callbacks ---
    pc->onLocalDescription([&signaling](rtc::Description desc) {
        std::string msg = "SDP|" + base64_encode(std::string(desc));
        signaling.send(msg);
        log_enqueue("[Signaling] Sent SDP");
    });

    pc->onLocalCandidate([&signaling](rtc::Candidate cand) {
        std::string msg = "ICE|" + base64_encode(cand.candidate())
                        + "|" + base64_encode(cand.mid());
        signaling.send(msg);
        log_enqueue("[Signaling] Sent ICE");
    });

    signaling.onMessage([&](const std::string &msg, const std::string&, int) {
        if (msg.rfind("SDP|", 0) == 0) {
            pc->setRemoteDescription(rtc::Description(base64_decode(msg.substr(4))));
            log_enqueue("[Signaling] Got remote SDP");
        } else if (msg.rfind("ICE|", 0) == 0) {
            size_t p = msg.find('|', 4);
            if (p != std::string::npos) {
                std::string c = base64_decode(msg.substr(4, p-4));
                std::string mid = base64_decode(msg.substr(p+1));
                pc->addRemoteCandidate(rtc::Candidate(c, mid));
                log_enqueue("[Signaling] Got ICE");
            }
        }
    });

    // --- Create DataChannel ---
    rtc::DataChannelInit opts;
    // Example: set unordered/unreliable for telemetry to reduce latency
    // opts.ordered = false; opts.maxRetransmits = 0;
    auto dc = pc->createDataChannel("ctrl", opts);
    g_dc = dc;

    // WebRTC callback: fast copy into rxRing
    dc->onMessage([&](rtc::message_variant msg){
        if (std::holds_alternative<rtc::binary>(msg)) {
            auto bin = std::get<rtc::binary>(msg);
            // allocate slot and copy quickly
            Slot* s = alloc_rx_slot();
            size_t len = bin.size();
            if (len > sizeof(s->data)) {
                // too big, drop
                log_enqueue("[RX] incoming too large, drop");
                return;
            }
            s->len = len;
            // rtc::binary::data() returns pointer-like; reinterpret
            const uint8_t* p = reinterpret_cast<const uint8_t*>(bin.data());
            memcpy(s->data, p, len);
            if (!rxRing.push(s)) {
                log_enqueue("[RX] rxRing full, drop");
            }
        } else if (std::holds_alternative<std::string>(msg)) {
            // small text messages -> log via logger thread
            log_enqueue(std::string("[DC TEXT] ") + std::get<std::string>(msg));
        }
    });

    dc->onOpen([&](){
        log_enqueue("[Client] DC OPEN");
        // prepare a test frame but push into txRing so send happens in tx dispatcher
        uint8_t speed_payload[1] = { 80 }; // set speed = 80%
        auto frame = build_frame(1, CMD_SET_DIRECTION, speed_payload, 1);
        Slot* s = alloc_tx_slot();
        s->len = frame.size();
        memcpy(s->data, frame.data(), s->len);
        if (!txRing.push(s)) {
            log_enqueue("[TX] failed to push test frame");
        } else {
            log_enqueue("[Client] queued test frame to txRing");
        }
    });

    // Start RT threads
    std::thread rx_handler_t(rx_handler_thread_fn);
    std::thread control_loop_t([](){ control_loop_thread_fn(10); }); // 10 ms period
    std::thread tx_dispatcher_t(tx_dispatcher_thread_fn);

    // Optionally set RT priorities (requires privileges)
    set_thread_realtime(control_loop_t, 80, 1);   // high priority
    set_thread_realtime(tx_dispatcher_t, 70, 1);
    set_thread_realtime(rx_handler_t, 60, 1);
    // logger stays normal

    pc->setLocalDescription();

    // Main thread waits for user interrupt (Ctrl+C) to stop
    std::cout << "[Client] Running. Press Ctrl+C to exit.\n";
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // graceful stop (unreachable in current loop; add signal handler if needed)
    stopping.store(true);
    logCv.notify_all();
    logger_t.join();
    rx_handler_t.join();
    control_loop_t.join();
    tx_dispatcher_t.join();

    return 0;
}
