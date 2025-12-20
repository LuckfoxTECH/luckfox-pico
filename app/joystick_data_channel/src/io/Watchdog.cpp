// io/Watchdog.cpp
#include "Watchdog.hpp"

#include <thread>
#include <chrono>
#include <atomic>

#include "app/Time.hpp"
#include "app/EventRing.hpp"
#include "app/SafetyManager.hpp"

// ============================================================
// CONFIG
// ============================================================
static constexpr u32 WATCHDOG_PERIOD_MS = 100;

// ============================================================
// EXTERN (RTE)
// ============================================================
extern EventRing<Event, 128> g_eventRing;
extern SafetyManager g_safety;

// ============================================================
// WATCHDOG RUNNABLE
// ============================================================
void watchdog::run(std::atomic<bool>& running)
{
    using namespace std::chrono;

    while (running.load(std::memory_order_acquire))
    {
        const u32 now = now_ms();

        g_safety.watchdogCheck(now);

        if (g_safety.isEmergency())
        {
            Event ev{};
            ev.type      = EventType::EMERGENCY;
            ev.timestamp = now;
            ev.u8_value =
                static_cast<u8>(g_safety.reason());

            g_eventRing.push(ev);
            break;
        }

        std::this_thread::sleep_for(
            milliseconds(WATCHDOG_PERIOD_MS)
        );
    }
}
