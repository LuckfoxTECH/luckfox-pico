// io/JoystickInput.cpp
#include "io/JoystickInput.hpp"

#include "app/EventRing.hpp"
#include "app/ControlState.hpp"
#include "app/Time.hpp"

#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <cstdio>
#include <poll.h>
#include <errno.h>
#include <algorithm>

// ============================================================
// INIT
// ============================================================
void JoystickInput::init()
{
    // Future:
    // - load calibration
    // - deadzone
    // - axis mapping
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

bool JoystickInput::debounce_button(uint8_t button, uint32_t now)
{
    uint32_t &last = last_button_ts_[button];

    if ((now - last) < DEBOUNCE_MS)
        return false;

    last = now;
    return true;
}

// ============================================================
// IO TASK (AUTOSAR IO Runnable)
// ============================================================
void JoystickInput::run()
{
    constexpr const char* JS_DEV = "/dev/input/js2";
    constexpr int POLL_TIMEOUT_MS = 10;

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

        const u32 ts = now_ms();

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
                    break;

                case 2: // Brake
                    evt.type     = EventType::BRAKE;
                    evt.u8_value =
                        static_cast<u8>((value + 32768) * 100 / 65535);
                    break;

                case 5: // Throttle
                    evt.type     = EventType::THROTTLE;
                    evt.u8_value =
                        static_cast<u8>((value + 32768) * 100 / 65535);
                    break;

                default:
                    continue;
            }

            g_eventRing.push(evt);
        }

        // ================= BUTTON =================
        else if (js.type == JS_EVENT_BUTTON)
        {
            if (js.value != 1)
                continue;

            if (!debounce_button(js.number, ts))
                continue;

            switch (js.number)
            {
                case 0: // Emergency
                    evt.type     = EventType::EMERGENCY;
                    evt.u8_value = 1;
                    break;

                case 5: // Forward
                    evt.type     = EventType::DIRECTION;
                    evt.u8_value = static_cast<u8>(Direction::FWD);
                    break;

                case 4: // Reverse
                    evt.type     = EventType::DIRECTION;
                    evt.u8_value = static_cast<u8>(Direction::REV);
                    break;

                default:
                    continue;
            }

            g_eventRing.push(evt);
        }
    }
}