// app/EventHandler.cpp
#include "EventHandler.hpp"

#include "EventRing.hpp"
#include "ControlState.hpp"
#include "SafetyManager.hpp"

#include <thread>
#include <chrono>

// ===== Extern global objects =====
extern EventRing<Event, 128> g_eventRing;
extern ControlState         g_state;
extern SafetyManager        g_safety;

void EventHandler::run(std::atomic<bool>& running)
{
    using namespace std::chrono;

    while (running.load(std::memory_order_acquire))
    {
        Event ev;

        // === Event Dispatch ===
        while (g_eventRing.pop(ev))
        {
            g_safety.notifyInput(ev.timestamp);

            switch (ev.type)
            {
            case EventType::STEERING:
                g_state.setSteering(ev.s16_value);
                break;

            case EventType::THROTTLE:
                g_state.setThrottle(ev.u8_value);
                break;

            case EventType::BRAKE:
                g_state.setBrake(ev.u8_value);
                break;

            case EventType::DIRECTION:
                g_state.setDirection(
                    static_cast<Direction>(ev.u8_value));
                break;

            case EventType::EMERGENCY:
                g_safety.trigger(EmergencyReason::BUTTON);
                break;

            default:
                break;
            }
        }

        // === COM Trigger (AUTOSAR-like) ===
        if (g_safety.isEmergency())
        {
            // build EmergencyPDU → send
        }
        else
        {
            // build FullStatePDU → send
        }

        // === Task Period (50 Hz) ===
        std::this_thread::sleep_for(milliseconds(20));
    }

    // === Optional cleanup here ===
}
