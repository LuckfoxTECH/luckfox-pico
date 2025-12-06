// peer_client_fixed.cpp
#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
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

// ---------- MAIN ----------
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

    // --- Signaling callbacks ---
    pc->onLocalDescription([&signaling](rtc::Description desc) {
        std::string msg = "SDP|" + base64_encode(std::string(desc));
        signaling.send(msg);
        std::cout << "[Client] Sent SDP\n";
    });

    pc->onLocalCandidate([&signaling](rtc::Candidate cand) {
        std::string msg = "ICE|" + base64_encode(cand.candidate())
                        + "|" + base64_encode(cand.mid());
        signaling.send(msg);
        std::cout << "[Client] Sent ICE\n";
    });

    signaling.onMessage([&](const std::string &msg, const std::string&, int) {
        if (msg.rfind("SDP|", 0) == 0) {
            pc->setRemoteDescription(rtc::Description(base64_decode(msg.substr(4))));
            std::cout << "[Client] Got remote SDP\n";
        } else if (msg.rfind("ICE|", 0) == 0) {
            size_t p = msg.find('|', 4);
            if (p != std::string::npos) {
                std::string c = base64_decode(msg.substr(4, p-4));
                std::string mid = base64_decode(msg.substr(p+1));
                pc->addRemoteCandidate(rtc::Candidate(c, mid));
                std::cout << "[Client] Got ICE\n";
            }
        }
    });

    // --- Create DataChannel ---
    rtc::DataChannelInit opts;
    auto dc = pc->createDataChannel("ctrl", opts);

    dc->onOpen([dc]() {
        std::cout << "[Client] DC OPEN\n";

        // --- TEST SEND COMMAND: CMD_SET_SPEED = 0x10 ---
        uint8_t speed_payload[1] = { 80 }; // set speed = 80%
        auto frame = build_frame(1, CMD_SET_DIRECTION, speed_payload, 1);

        // convert to rtc::binary and send
        rtc::binary bin = vec_u8_to_rtcbin(frame);
        try {
            dc->send(bin);
            std::cout << "[Client] Sent frame, len=" << bin.size() << std::endl;
        } catch (const std::exception &e) {
            std::cout << "[Client] send exception: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[Client] send unknown exception\n";
        }
    });

    dc->onMessage([&](rtc::message_variant msg){
        if (std::holds_alternative<rtc::binary>(msg)) {
            auto bin = std::get<rtc::binary>(msg);

            Frame_t rx{};
            bool ok = parse_frame(
                reinterpret_cast<const uint8_t*>(bin.data()),
                bin.size(),
                rx
            );

            if (!ok) {
                std::cout << "[Client] Invalid frame\n";
                return;
            }

            std::cout << "[Client] Got ACK: cmd=0x" << std::hex << (int)rx.cmd << std::dec
                      << " msg_id=" << (int)rx.msg_id
                      << " ack_code=" << (int)rx.payload[0] << std::endl;
        } else if (std::holds_alternative<std::string>(msg)) {
            std::cout << "[Client] Received text: " << std::get<std::string>(msg) << std::endl;
        }
    });

    pc->setLocalDescription();

    while (true) std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
