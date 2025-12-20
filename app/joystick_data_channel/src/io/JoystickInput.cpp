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

// ============================================================
// IO TASK (AUTOSAR IO Runnable)
// ============================================================
void JoystickInput::run()
{
    int fd = open("/dev/input/js2", O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("Joystick open failed");
        return;
    }

    js_event e{};

    while (running_.load(std::memory_order_acquire))
    {
        if (read(fd, &e, sizeof(e)) != sizeof(e))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            continue;
        }

        e.type &= ~JS_EVENT_INIT;
        const u32 ts = now_ms();

        // ================= AXIS =================
        if (e.type == JS_EVENT_AXIS)
        {
            if (e.number == 0) // steering
            {
                g_eventRing.push({
                    EventType::STEERING,
                    static_cast<s16>(e.value),
                    0,
                    ts
                });
            }
            else if (e.number == 2) // brake
            {
                u8 br = (e.value + 32767) * 100 / 65534;
                g_eventRing.push({
                    EventType::BRAKE,
                    0,
                    br,
                    ts
                });
            }
            else if (e.number == 5) // throttle
            {
                u8 th = (e.value + 32767) * 100 / 65534;
                g_eventRing.push({
                    EventType::THROTTLE,
                    0,
                    th,
                    ts
                });
            }
        }
        // ================= BUTTON =================
        else if (e.type == JS_EVENT_BUTTON)
        {
            if (e.number == 0 && e.value == 1)
            {
                g_eventRing.push({
                    EventType::EMERGENCY,
                    0,
                    1,
                    ts
                });
            }
            else if (e.number == 5 && e.value == 1)
            {
                g_eventRing.push({
                    EventType::DIRECTION,
                    0,
                    static_cast<u8>(Direction::FWD),
                    ts
                });
            }
            else if (e.number == 4 && e.value == 1)
            {
                g_eventRing.push({
                    EventType::DIRECTION,
                    0,
                    static_cast<u8>(Direction::REV),
                    ts
                });
            }
        }
    }

    close(fd);
}
