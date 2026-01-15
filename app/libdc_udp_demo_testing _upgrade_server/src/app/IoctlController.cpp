// io/IoctlController.cpp
#include "IoctlController.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>

// ===== COM / APP =====
#include "com/PduTypes.hpp"
#include "app/EventRing.hpp"
#include "app/ControlState.hpp"

// ================= IOCTL =================
#define SERVO_SET_ANGLE   _IOW('p', 1, int)
#define GPIO_SET_LEVEL    _IOW('p', 2, int)

// ================= GLOBALS =================
extern EventRing<Event, 128> g_eventRing;
extern ControlState          g_state;

// ================= STEERING CONFIG =================
constexpr int STEERING_MAX = 32638;
constexpr int SERVO_MAX    = 20;

constexpr int DEADZONE     = 800;   // steering units
constexpr int EXPO         = 30;    // 0..100
constexpr int SERVO_RATE   = 2;     // deg per cycle

// =================================================
// HELPERS
// =================================================
static int applyDeadZone(int steering)
{
    if (std::abs(steering) < DEADZONE)
        return 0;
    return steering;
}

static int applyExpo(int steering)
{
    int sign = (steering >= 0) ? 1 : -1;
    int x    = std::abs(steering);

    // normalize 0..1000
    int norm = (x * 1000) / STEERING_MAX;

    int linear = norm;
    int cubic  = (norm * norm * norm) / (1000 * 1000);

    int mixed = ((100 - EXPO) * linear + EXPO * cubic) / 100;

    return sign * ((mixed * STEERING_MAX) / 1000);
}

static int applyRateLimit(int target)
{
    static int last = 0;

    int delta = target - last;
    if (delta > SERVO_RATE)
        delta = SERVO_RATE;
    else if (delta < -SERVO_RATE)
        delta = -SERVO_RATE;

    last += delta;
    return last;
}

// =================================================
// CTOR / DTOR
// =================================================
IoctlController::IoctlController()
{
    m_fd = open("/dev/luckfox_ctrl", O_RDWR);
    if (m_fd < 0) {
        perror("[IoctlController] open");
    }
}

IoctlController::~IoctlController()
{
    stop();

    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

// =================================================
// PUBLIC API
// =================================================
void IoctlController::start()
{
    if (running_.load())
        return;

    running_.store(true);
    thread_ = std::thread(&IoctlController::run, this);
}

void IoctlController::stop()
{
    if (!running_.load())
        return;

    running_.store(false);

    if (thread_.joinable())
        thread_.join();
}

// =================================================
// MAIN LOOP
// =================================================
void IoctlController::run()
{
    using namespace std::chrono_literals;

    while (running_.load())
    {
        Event ev{};
        if (!g_eventRing.pop(ev)) {
            std::this_thread::sleep_for(1ms);
            continue;
        }

        if (ev.type != EventType::COM_RX)
            continue;

        PduType pdu = static_cast<PduType>(ev.u8_value);

        if (pdu == PduType::FULL_STATE) {
            applyControl();
        }
    }
}

// =================================================
// APPLY STATE → IOCTL
// =================================================
void IoctlController::applyControl()
{
    if (m_fd < 0)
        return;

    // ===== SNAPSHOT =====
    ControlSnapshot s = g_state.snapshot();

    int steering  = s.steering;
    int throttle  = s.throttle;
    int brake     = s.brake;
    int direction = static_cast<int>(s.direction);

    // ===== STEERING PIPELINE =====
    int st = applyDeadZone(steering);
    st     = applyExpo(st);

    int servo_target = (st * SERVO_MAX) / STEERING_MAX;
    servo_target = std::clamp(servo_target, -SERVO_MAX, SERVO_MAX);

    int servo_angle = applyRateLimit(servo_target);

    // ===== SERVO =====
    if (ioctl(m_fd, SERVO_SET_ANGLE, &servo_angle) < 0) {
        perror("[IOCTL] SERVO_SET_ANGLE");
    }

    // ===== MOTOR ENABLE =====
    int motor_en = (throttle > 0 && brake == 0) ? 1 : 0;
    if (ioctl(m_fd, GPIO_SET_LEVEL, &motor_en) < 0) {
        perror("[IOCTL] GPIO_SET_LEVEL");
    }

    // ===== DEBUG =====
    printf(
        "[IOCTL] seq=%u steering=%d servo=%d thr=%d brk=%d dir=%d ts=%u\n",
        s.seq,
        steering,
        servo_angle,
        throttle,
        brake,
        direction,
        s.ts_ms
    );
}
