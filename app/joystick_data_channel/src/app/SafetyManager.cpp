// app/SafetyManager.cpp
#include "SafetyManager.hpp"

// ============================================================
// GLOBAL INSTANCE
// ============================================================
SafetyManager g_safety;

// ============================================================
// INIT
// ============================================================
void SafetyManager::init()
{
    g_safety.emergency_.store(false, std::memory_order_relaxed);
    g_safety.reason_.store(EmergencyReason::NONE, std::memory_order_relaxed);
    g_safety.last_input_ms_.store(0, std::memory_order_relaxed);
}

// ============================================================
// INPUT NOTIFY
// ============================================================
void SafetyManager::notifyInput(u32 now_ms)
{
    last_input_ms_.store(now_ms, std::memory_order_release);
}

// ============================================================
// EMERGENCY TRIGGER (LATCHED)
// ============================================================
void SafetyManager::trigger(EmergencyReason r)
{
    // Latch emergency once
    bool expected = false;
    if (emergency_.compare_exchange_strong(
            expected, true,
            std::memory_order_acq_rel))
    {
        reason_.store(r, std::memory_order_release);
    }
}

// ============================================================
// STATUS
// ============================================================
bool SafetyManager::isEmergency() const
{
    return emergency_.load(std::memory_order_acquire);
}

EmergencyReason SafetyManager::reason() const
{
    return reason_.load(std::memory_order_acquire);
}

// ============================================================
// WATCHDOG CHECK (OPTIONAL PATH)
// ============================================================
void SafetyManager::watchdogCheck(u32 now_ms)
{
    if (emergency_.load(std::memory_order_acquire))
        return;

    const u32 last = last_input_ms_.load(std::memory_order_acquire);

    if ((now_ms - last) > 3000)
    {
        trigger(EmergencyReason::WATCHDOG);
    }
}
