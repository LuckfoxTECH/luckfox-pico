#include <atomic>
#include <cstdint>
#include <thread>
#include <iostream>
#include <chrono>
#include <cstring>
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>

/* ============================================================
 * BASIC TYPES
 * ============================================================ */
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s16 = int16_t;

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
 * AUTOSAR PDUs
 * ============================================================ */
#pragma pack(push,1)

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
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        u32 now  = now_ms();
        u32 last = g_safety.last_input_ms.load();

        if (!g_safety.emergency.load() && (now - last) > 30000)
        {
            std::cerr << "[WD] Input timeout\n";
            trigger_emergency(EmergencyReason::WATCHDOG);
        }
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
void event_handler_thread()
{
    EventPDU ev{};

    while (true)
    {
        if (!g_event_ring.pop(ev)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        /* ---------- EMERGENCY ---------- */
        if (g_safety.emergency.load(std::memory_order_acquire))
        {
            EmergencyPDU em{};
            em.pdu_type  = 0x03U;

            EmergencyReason reason = g_emergency_reason.load(std::memory_order_acquire);
            em.reason    = reason;           
            em.timestamp = now_ms();

            std::cout << "[SEND] EMERGENCY PDU reason="
                    << static_cast<int>(reason) << " ts=" << em.timestamp << "\n";

            // Try clear automatically if safe (optional)
            bool cleared = clear_emergency_if_safe();

            continue;
        }


        /* ---------- FULL STATE SNAPSHOT ---------- */
        FullStatePDU fs{};
        fs.pdu_type  = 0x01;
        fs.steering  = g_state.steering.load();
        fs.throttle  = g_state.throttle.load();
        fs.brake     = g_state.brake.load();
        Direction dir = g_state.direction.load(std::memory_order_acquire);
        fs.direction  = static_cast<u8>(dir);
        fs.seq       = g_state.seq.fetch_add(1);

        std::cout << "[SEND] FULL "
                  << "seq=" << fs.seq
                  << " steer=" << fs.steering
                  << " thr=" << (int)fs.throttle
                  << " brake=" << (int)fs.brake
                  << " direction=" << (int)fs.direction
                  << "\n";

        /*
         * TODO:
         *  - send via WebRTC DataChannel
         *  - wait ACK
         *  - retry / timeout
         */
    }
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main()
{
    std::cout << "=== AUTOSAR PEER CLIENT CORE ===\n";

    g_safety.last_input_ms.store(now_ms());

    std::thread t_js(joystick_thread);
    std::thread t_ev(event_handler_thread);
    std::thread t_wd(watchdog_thread);

    t_js.join();
    t_ev.join();
    t_wd.join();
    return 0;
}
