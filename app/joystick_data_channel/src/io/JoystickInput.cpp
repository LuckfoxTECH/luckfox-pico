// io/JoystickInput.cpp
#include "io/JoystickInput.hpp"

#include "app/EventRing.hpp"
#include "app/ControlState.hpp"
#include "app/Time.hpp"

#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <algorithm>
#include <cstdio>

// ============================================================
// CONFIG
// ============================================================
constexpr const char* JS_DEV = "/dev/input/js2";
constexpr int POLL_TIMEOUT_MS = 5;

// ============================================================
// LOCAL STATE
// ============================================================
static Direction g_direction = Direction::STOP;
static uint8_t   g_throttle  = 0;

// ============================================================
// DIRECTION HELPERS
// ============================================================
static Direction next_fwd(Direction cur)
{
    switch (cur) {
        case Direction::FWD:  return Direction::FWD;
        case Direction::STOP: return Direction::FWD;
        case Direction::REV:  return Direction::STOP;
    }
    return Direction::STOP;
}

static Direction next_rev(Direction cur)
{
    switch (cur) {
        case Direction::REV:  return Direction::REV;
        case Direction::STOP: return Direction::REV;
        case Direction::FWD:  return Direction::STOP;
    }
    return Direction::STOP;
}

// ============================================================
// INIT
// ============================================================
void JoystickInput::init()
{
    // Future:
    // - deadzone
    // - axis remap
    // - calibration
}

// ============================================================
// START / STOP
// ============================================================
void JoystickInput::start()
{
    running_.store(true, std::memory_order_release);
    thread_ = std::thread(&JoystickInput::run, this);
}

void JoystickInput::stop()
{
    running_.store(false, std::memory_order_release);

    if (thread_.joinable())
        thread_.join();
}

// ============================================================
// IO TASK (AUTOSAR IO Runnable)
// ============================================================
void JoystickInput::run()
{
    int fd = open(JS_DEV, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("Joystick open failed");
        return;
    }

    struct FdGuard {
        int fd;
        ~FdGuard() { if (fd >= 0) close(fd); }
    } guard{fd};

    pollfd pfd{};
    pfd.fd     = fd;
    pfd.events = POLLIN;

    js_event js{};

    while (running_.load(std::memory_order_acquire))
    {
        int pret = poll(&pfd, 1, POLL_TIMEOUT_MS);
        if (pret <= 0)
            continue;

        if (!(pfd.revents & POLLIN))
            continue;

        ssize_t n = read(fd, &js, sizeof(js));
        if (n != sizeof(js))
            continue;

        js.type &= ~JS_EVENT_INIT;

        const uint32_t ts = now_ms();

        Event evt{};
        evt.timestamp = ts;
        evt.s16_value = 0;
        evt.u8_value  = 0;

        // ================= AXIS =================
        if (js.type == JS_EVENT_AXIS)
        {
            const int value =
                std::clamp<int>(js.value, -32768, 32767);

            switch (js.number)
            {
                case 0: // Steering
                    evt.type      = EventType::STEERING;
                    evt.s16_value = static_cast<s16>(value);
                    g_eventRing.push(evt);
                    break;

                case 2: // Brake
                    evt.type     = EventType::BRAKE;
                    evt.u8_value =
                        static_cast<u8>((value + 32768) * 100 / 65535);
                    g_eventRing.push(evt);
                    break;

                case 5: // Throttle
                {
                    g_throttle =
                        static_cast<u8>((value + 32768) * 100 / 65535);

                    evt.type     = EventType::THROTTLE;
                    evt.u8_value = g_throttle;
                    g_eventRing.push(evt);
                    break;
                }

                default:
                    break;
            }
        }

        // ================= BUTTON =================
        else if (js.type == JS_EVENT_BUTTON)
        {
            if (js.value != 1)
                continue;

            bool dir_changed = false;

            switch (js.number)
            {
                case 0: // Emergency
                    evt.type     = EventType::EMERGENCY;
                    evt.u8_value = 1;
                    g_eventRing.push(evt);
                    break;

                case 5: // Forward button
                    if (g_throttle == 0) {
                        g_direction = next_fwd(g_direction);
                        dir_changed = true;
                    }
                    break;

                case 4: // Reverse button
                    if (g_throttle == 0) {
                        g_direction = next_rev(g_direction);
                        dir_changed = true;
                    }
                    break;

                default:
                    break;
            }

            if (dir_changed)
            {
                evt.type     = EventType::DIRECTION;
                evt.u8_value = static_cast<uint8_t>(g_direction);
                g_eventRing.push(evt);
            }
        }
    }
}
