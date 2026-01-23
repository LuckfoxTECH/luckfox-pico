#include <atomic>
#include <cstdint>
#include <thread>
#include <iostream>
#include <chrono>
#include <cstring>
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string>
#include "udp_signaling.hpp"
#include "rtc/rtc.hpp"

/* ============================================================
 * BASIC TYPES
 * ============================================================ */
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s16 = int16_t;

/**
 * @brief Enumeration of emergency reasons for vehicle safety system.
 * 
 * This enum defines all possible causes of emergency state in the system.
 * Each value corresponds to a specific failure or trigger condition.
 */
enum class EmergencyReason : uint8_t
{
    NONE           = 0U,  		/**< No emergency condition */
    BUTTON         = 1U,  		/**< Emergency Stop button pressed by driver */
    WATCHDOG       = 2U,  		/**< Input signal timeout detected by watchdog */
    SENSOR_FAIL    = 3U,  		/**< Critical sensor failure (e.g., speed, steering angle) */
    COMM_ERROR     = 4U, 		/**< Communication error (CAN, Ethernet, FlexRay) */
    BRAKE_FAIL     = 5U,  		/**< Brake system failure */
    STEERING_FAIL  = 6U,  		/**< Steering system failure */
    POWER_FAIL     = 7U,  		/**< Power supply or ECU failure */
    SOFTWARE_FAULT = 8U   		/**< Software fault or CPU overload */
};

/* ============================================================
 * AUTOSAR PDUs
 * ============================================================ */
#pragma pack(push,1)
struct Frame 
{
    u8 sof;
    u8 msg_id;
    u8 cmd;
    u8 length;
    u8 payload[32];
    u8 checksum;
};

struct FullStatePDU
{
    u8  pdu_type;     // 0x01
    s16 steering;
    u8  throttle;
    u8  brake;
    u8  direction;
    u32 seq;
};

struct AckPDU
{
    u8  pdu_type;     // 0x02
    u32 seq;
    u8  result;       // 0=OK
};

struct EmergencyPDU
{
    u8  pdu_type;     // 0x03
    EmergencyReason  reason;       // 1=Button, 2=Watchdog
    u32 timestamp;
};

#pragma pack(pop)

static inline uint8_t axis_to_angle(int16_t axis)
{
    // clamp
    if (axis >  32767) axis =  32767;
    if (axis < -32767) axis = -32767;

    // map [-32767, 32767] -> [0, 120]
    int angle = (axis + 32767) * 120 / 65534;

    if (angle < 0)   angle = 0;
    if (angle > 120) angle = 120;

    return static_cast<uint8_t>(angle);
}
uint8_t calc_checksum(const Frame& f);
// ---------- Build Frame ----------
std::vector<uint8_t> build_frame(uint8_t msg_id, uint8_t cmd, const uint8_t* payload, uint8_t len) {
    Frame f{};
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
bool parse_frame(const uint8_t* buf, size_t size, Frame& out) {
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


/* ============================================================
 * TIME
 * ============================================================ */
static inline u32 now_ms()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count();
}

/**
 * @brief Vehicle movement direction.
 */

enum class Direction : uint8_t
{
    STOP  = 0U,  /**< Vehicle is stationary */
    FWD   = 1U,  /**<    FWD   = 1U,  /**< Forward direction */
    REV   = 2U   /**< Reverse direction */
};

/* ============================================================
 * CONTROL STATE (AUTOSAR FULL STATE)
 * ============================================================ */
struct ControlState
{
    std::atomic<s16> steering{0};    // [-32767..32767]
    std::atomic<u8>  throttle{0};    // [0..100]
    std::atomic<u8>  brake{0};
    std::atomic<Direction> direction{Direction::STOP};
    std::atomic<u32> seq{0};
};

static ControlState g_state;

/* ============================================================
 * SAFETY STATE
 * ============================================================ */
struct SafetyState
{
    std::atomic<bool> emergency{false};
    std::atomic<u32>  last_input_ms{0};
};

static SafetyState g_safety;

/* ============================================================
 * EVENT DEFINITIONS
 * ============================================================ */
enum class EventType : u8
{
    STEERING,
    THROTTLE,
    BRAKE,
    DIRECTION,
    EMERGENCY
};

struct EventPDU
{
    EventType type;
    s16       s16_value;
    u8        u8_value;
    u32       timestamp;
};

/* ============================================================
 * LOCK-FREE RING BUFFER (SPSC)
 * ============================================================ */
template<typename T, size_t N>
class RingBuffer
{
public:
    bool push(const T& v)
    {
        size_t h = head.load(std::memory_order_relaxed);
        size_t n = (h + 1) % N;
        if (n == tail.load(std::memory_order_acquire))
            return false;

        buf[h] = v;
        head.store(n, std::memory_order_release);
        return true;
    }

    bool pop(T& out)
    {
        size_t t = tail.load(std::memory_order_relaxed);
        if (t == head.load(std::memory_order_acquire))
            return false;

        out = buf[t];
        tail.store((t + 1) % N, std::memory_order_release);
        return true;
    }

private:
    T buf[N];
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};
};

static RingBuffer<EventPDU, 128> g_event_ring;

/* ============================================================
 * COM – SERIALIZE + FRAME
 * ============================================================ */
 
 // ---------- Checksum ----------
uint8_t calc_checksum(const Frame& f) {
    uint8_t sum = f.sof ^ f.msg_id ^ f.cmd ^ f.length;
    for (int i = 0; i < f.length; i++) sum ^= f.payload[i];
    return sum;
}

static std::vector<uint8_t> serialize_fullstate(const FullStatePDU& fs)
{
    std::vector<uint8_t> out(sizeof(FullStatePDU));
    std::memcpy(out.data(), &fs, sizeof(FullStatePDU));
    return out;
}

namespace Com
{
    static std::vector<u8> build_fullstate_frame(const FullStatePDU& fs)
    {
        auto payload = serialize_fullstate(fs);

        Frame f{};
        f.sof    = 0xAA;
        f.msg_id = fs.seq & 0xFF;
        f.cmd    = 0x01;
        f.length = payload.size();
        std::memcpy(f.payload, payload.data(), f.length);
        f.checksum = calc_checksum(f);

        std::vector<u8> out;
        out.reserve(5 + f.length);
        out.push_back(f.sof);
        out.push_back(f.msg_id);
        out.push_back(f.cmd);
        out.push_back(f.length);
        for (u8 i = 0; i < f.length; i++) out.push_back(f.payload[i]);
        out.push_back(f.checksum);
        return out;
    }
}

/* ============================================================
 * TRANSPORT ADAPTER
 * ============================================================ */
class ITransport
{
public:
    virtual ~ITransport() = default;
    virtual void send_binary(const std::vector<u8>& data) = 0;
};

class DummyTransport : public ITransport
{
public:
    void send_binary(const std::vector<u8>& data) override
    {
        std::cout << "[TX] size=" << data.size()
                  << " seq=" << int(data[1]) << "\n";
    }
};
 
/* ============================================================
 * SAFETY HELPERS
 * ============================================================ */

static std::atomic<EmergencyReason> g_emergency_reason{EmergencyReason::NONE};

// Optional: request flag + ack flag for controlled clear (depends on your protocol)
static std::atomic<bool> g_emergency_clear_requested{false};
static std::atomic<bool> g_emergency_clear_acked{false};



/**
 * @brief Trigger emergency state with a specific reason (type-safe).
 *        Sets brake = true and throttle = 0 to ensure fail-safe behavior.
 */
static inline void trigger_emergency(EmergencyReason reason)
{
    bool expected = false;
    if (g_safety.emergency.compare_exchange_strong(expected, true,
        std::memory_order_release, std::memory_order_relaxed))
    {
        g_emergency_reason.store(reason, std::memory_order_release);
        g_emergency_clear_requested.store(false, std::memory_order_relaxed);
        g_emergency_clear_acked.store(false, std::memory_order_relaxed);

        g_state.brake.store(0, std::memory_order_release);
        g_state.throttle.store(0,   std::memory_order_release);

        std::cerr << "[EMERGENCY] reason=" << static_cast<int>(reason) << "\n";
       }
}

// ============================================================
// CONFIGURATION CONSTANTS
// ============================================================
static constexpr uint32_t kMinStableMsAfterInput = 200U;

/**
 * @brief Attempt to clear emergency state if safety conditions are satisfied.
 * @return true if emergency was cleared; false otherwise.
 */
static inline bool clear_emergency_if_safe()
{
    // Fast path: check if we are in emergency
    if (!g_safety.emergency.load(std::memory_order_acquire)) {
        return true; // Already clear
    }

    const uint32_t now  = now_ms();
    const uint32_t last = g_safety.last_input_ms.load(std::memory_order_acquire);

    // Safety preconditions: throttle == 0, brake == true, inputs stable
    const bool throttle_zero = (g_state.throttle.load(std::memory_order_acquire) == 0U);
    const bool brake_on      =  (g_state.brake.load(std::memory_order_acquire) == 0U);
    const bool input_stable  = (now > last) && ((now - last) >= kMinStableMsAfterInput);

    if (!(throttle_zero && brake_on )) {
        // Log why we cannot clear yet (optional)
        std::cerr << "[CLEAR_EMERGENCY] Preconditions not met: "
                  << "thr0=" << throttle_zero
                  << " brake=" << brake_on;
                  //<< " stable=" << input_stable << "\n";
        return false;
    }

    // All conditions met: clear emergency and reset reason
    g_emergency_reason.store(EmergencyReason::NONE, std::memory_order_release);
    g_safety.emergency.store(false, std::memory_order_release);

    std::cerr << "[CLEAR_EMERGENCY] Cleared successfully\n";
       return true;
}

/* ============================================================
 * WATCHDOG THREAD
 * ============================================================ */
void watchdog_thread()
{
    while (true)
    {
        u32 now = now_ms();
        u32 last = g_safety.last_input_ms.load();
        if (!g_safety.emergency.load() && (now - last) > 3000)
        {
            g_safety.emergency.store(true);
            g_emergency_reason.store(EmergencyReason::WATCHDOG);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


/* ============================================================
 * JOYSTICK INPUT THREAD
 * ============================================================ */
void joystick_thread()
{
    int fd = open("/dev/input/js2", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open /dev/input/js2");
        return;
    }

    js_event e{};

    while (true)
    {
        if (read(fd, &e, sizeof(e)) != sizeof(e)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            continue;
        }

        e.type &= ~JS_EVENT_INIT;
        g_safety.last_input_ms.store(now_ms());

        if (e.type == JS_EVENT_AXIS)
        {
            if (e.number == 0) // steering
            {
                g_state.steering.store(e.value);
                g_event_ring.push({EventType::STEERING, e.value, 0, now_ms()});
            }

            else if (e.number == 2) // brake
            {
                u8 br = (e.value + 32767) * 100 / 65534;
                g_state.brake.store(br);
                g_event_ring.push({EventType::BRAKE, 0, br, now_ms()});
            }

            else if (e.number == 5) // throttle
            {
                u8 th = (e.value + 32767) * 100 / 65534;
                g_state.throttle.store(th);
                g_event_ring.push({EventType::THROTTLE, 0, th, now_ms()});
            }
        }
        else if (e.type == JS_EVENT_BUTTON)
        {
            if (e.number == 0 && e.value == 1)
            {
                trigger_emergency(EmergencyReason::BUTTON);
                g_event_ring.push({EventType::EMERGENCY, 0, 1, now_ms()});
            }

            if (e.number == 5) /**< Forward direction */
            {

                g_state.direction.store(Direction::FWD, std::memory_order_release);
                g_event_ring.push({EventType::DIRECTION, 0, static_cast<u8>(Direction::FWD), now_ms()});
            }

            else if (e.number == 4) /**< Reverse direction */
            {
                g_state.direction.store(Direction::REV, std::memory_order_release);
                g_event_ring.push({EventType::DIRECTION, 0, static_cast<u8>(Direction::REV), now_ms()});
            }
        }
    }
}

/* ============================================================
 * EVENT HANDLER + FRAME BUILDER
 * ============================================================ */
void event_handler_thread(ITransport& transport)
{
    while (true)
    {
        if (g_safety.emergency.load())
        {
            EmergencyPDU em{};
            em.pdu_type  = 0x03U;

            EmergencyReason reason = g_emergency_reason.load(std::memory_order_acquire);
            em.reason    = reason;           
            em.timestamp = now_ms();

            std::cout << "[SEND] EMERGENCY PDU reason="
                    << static_cast<int>(reason) << " ts=" << em.timestamp << "\n";
            continue;
        }

        FullStatePDU fs{};
        fs.pdu_type  = 0x01;
        fs.steering  = g_state.steering.load();
        fs.throttle  = g_state.throttle.load();
        fs.brake     = g_state.brake.load();
        fs.direction = static_cast<u8>(g_state.direction.load());
        fs.seq       = g_state.seq.fetch_add(1);

        auto frame = Com::build_fullstate_frame(fs);
        transport.send_binary(frame);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}


/* ============================================================
 * MAIN
 * ============================================================ */
int main()
{
    std::cout << "=== AUTOSAR PEER CLIENT CORE ===\n";

    g_safety.last_input_ms.store(now_ms());
    
    DummyTransport transport;

    std::thread t_js(joystick_thread);
	std::thread t_ev(event_handler_thread, std::ref(transport));
    std::thread t_wd(watchdog_thread);

    t_js.join();
    t_ev.join();
    t_wd.join();
    return 0;
}
