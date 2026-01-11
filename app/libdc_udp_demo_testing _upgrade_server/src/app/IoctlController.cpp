// io/IoctlController.cpp
#include "IoctlController.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <algorithm>   // std::clamp

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

    if (thread_.joinable()) {
        thread_.join();
    }
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

        switch (ev.type)
        {
        case EventType::COM_RX:
            printf("[IOCTL][EV] COM_RX\n");
            break;

        case EventType::COM_TX:
            printf("[IOCTL][EV] COM_TX\n");
            break;

        default:
            printf("[IOCTL][EV] UNKNOWN\n");
            break;
        }

        if (ev.type != EventType::COM_RX)
            continue;

        // ================= PDU =================
        PduType pdu = static_cast<PduType>(ev.u8_value);

        printf("[IOCTL][PDU] raw=%u\n", ev.u8_value);

        switch (pdu)
        {
        case PduType::FULL_STATE:
            applyControl();
            break;

        default:
            printf("[IOCTL][PDU] UNKNOWN (%u)\n", ev.u8_value);
            break;
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

    // ===== SNAPSHOT (atomic-consistent) =====
    ControlSnapshot s = g_state.snapshot();

    int steering  = s.steering;
    int throttle  = s.throttle;
    int brake     = s.brake;
    int direction = static_cast<int>(s.direction);

    // ===== STEERING → SERVO =====
    int servo_angle = std::clamp(steering + 60, 0, 120);
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
