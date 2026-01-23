// app/SafetyManager.hpp
#pragma once

#include <atomic>
#include <cstdint>

// ============================================================
// BASIC TYPES
// ============================================================
using u32 = uint32_t;

// ============================================================
// EMERGENCY REASON
// ============================================================
enum class EmergencyReason : uint8_t
{
    NONE = 0,
    BUTTON,
    WATCHDOG,
    COMM_ERROR
};

// ============================================================
// SAFETY MANAGER (SWC)
// ============================================================
class SafetyManager
{
public:
    // --------------------------------------------------------
    // Init (must be called at startup)
    // --------------------------------------------------------
    static void init();

    // --------------------------------------------------------
    // Runtime API
    // --------------------------------------------------------
    void notifyInput(u32 now_ms);
    void trigger(EmergencyReason r);

    bool isEmergency() const;
    EmergencyReason reason() const;

    // --------------------------------------------------------
    // Optional watchdog check (if not using Watchdog thread)
    // --------------------------------------------------------
    void watchdogCheck(u32 now_ms);

private:
    std::atomic<bool>            emergency_{false};
    std::atomic<EmergencyReason> reason_{EmergencyReason::NONE};
    std::atomic<u32>             last_input_ms_{0};
};

// ============================================================
// GLOBAL INSTANCE
// ============================================================
extern SafetyManager g_safety;
