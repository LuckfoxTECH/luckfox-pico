// app/ControlState.hpp
#pragma once

#include <atomic>
#include <cstdint>

// ============================================================
// BASIC TYPES
// ============================================================
using u8  = uint8_t;
using u32 = uint32_t;
using s16 = int16_t;

// ============================================================
// DIRECTION
// ============================================================
enum class Direction : u8
{
    STOP = 0,
    FWD  = 1,
    REV  = 2
};

// ============================================================
// FULL STATE SNAPSHOT (COM PDU payload)
// ============================================================
struct ControlSnapshot
{
    u32       seq;
    s16       steering;
    u8        throttle;
    u8        brake;
    Direction direction;
};

// ============================================================
// CONTROL STATE (AUTOSAR Sender–Receiver SWC)
// ============================================================
class ControlState
{
public:
    // --------------------------------------------------------
    // Init (called once at system startup)
    // --------------------------------------------------------
    static void init();

    // --------------------------------------------------------
    // Setters (called by EventHandler)
    // --------------------------------------------------------
    void setSteering(s16 v);
    void setThrottle(u8 v);
    void setBrake(u8 v);
    void setDirection(Direction d);

    // --------------------------------------------------------
    // Snapshot (called by COM)
    // --------------------------------------------------------
    static ControlSnapshot snapshot();

private:
    // --------------------------------------------------------
    // Internal atomic state
    // --------------------------------------------------------
    std::atomic<s16>       steering_{0};
    std::atomic<u8>        throttle_{0};
    std::atomic<u8>        brake_{0};
    std::atomic<Direction> direction_{Direction::STOP};
    std::atomic<u32>       seq_{0};
};

// ============================================================
// GLOBAL INSTANCE (RTE-style)
// ============================================================
extern ControlState g_state;