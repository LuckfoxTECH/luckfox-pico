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

/* ============================================================
 * CONTROL STATE (AUTOSAR FULL STATE)
 * ============================================================ */
struct ControlState
{
    std::atomic<s16> steering{0};    // [-32767..32767]
    std::atomic<u8>  throttle{0};    // [0..100]
    std::atomic<bool> brake{true};
    std::atomic<u8>  direction{0};   // 0=FWD, 1=REV
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
static inline void trigger_emergency(EmergencyReason  reason)
{
    bool expected = false;
    if (g_safety.emergency.compare_exchange_strong(expected, true))
    {
        g_state.brake.store(true);
        std::cerr << "[EMERGENCY] reason=" << static_cast<int>(reason) << "\n";
    }
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

        if (!g_safety.emergency.load() && (now - last) > 300)
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
            else if (e.number == 1) // throttle
            {
                u8 th = (e.value + 32767) * 100 / 65534;
                g_state.throttle.store(th);
                g_state.brake.store(false);
                g_event_ring.push({EventType::THROTTLE, 0, th, now_ms()});
            }
        }
        else if (e.type == JS_EVENT_BUTTON)
        {
            if (e.number == 0 && e.value == 1)
            {
                trigger_emergency(EmergencyReason::WATCHDOG);
                g_event_ring.push({EventType::EMERGENCY, 0, 1, now_ms()});
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
        if (g_safety.emergency.load())
        {
            EmergencyPDU em{};
            em.pdu_type  = 0x03;
            em.reason    = EmergencyReason::BUTTON;
            em.timestamp= now_ms();

            std::cout << "[SEND] EMERGENCY PDU\n";
            continue;
        }

        /* ---------- FULL STATE SNAPSHOT ---------- */
        FullStatePDU fs{};
        fs.pdu_type  = 0x01;
        fs.steering  = g_state.steering.load();
        fs.throttle  = g_state.throttle.load();
        fs.brake     = g_state.brake.load();
        fs.direction = g_state.direction.load();
        fs.seq       = g_state.seq.fetch_add(1);

        std::cout << "[SEND] FULL "
                  << "seq=" << fs.seq
                  << " steer=" << fs.steering
                  << " thr=" << (int)fs.throttle
                  << " brake=" << (int)fs.brake
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
