// app/ControlState.cpp
#include "ControlState.hpp"
#include <chrono>

// ============================================================
// GLOBAL INSTANCE
// ============================================================
ControlState g_state;

// ============================================================
// INIT
// ============================================================
void ControlState::init()
{
    // Safe / neutral startup state
    g_state.steering_.store(0, std::memory_order_relaxed);
    g_state.throttle_.store(0, std::memory_order_relaxed);
    g_state.brake_.store(0, std::memory_order_relaxed);
    g_state.direction_.store(Direction::STOP, std::memory_order_relaxed);
    g_state.seq_.store(0, std::memory_order_relaxed);
}

// ============================================================
// SETTERS (EventHandler context)
// ============================================================
void ControlState::setSteering(s16 v)
{
    steering_.store(v, std::memory_order_release);
}

void ControlState::setThrottle(u8 v)
{
    throttle_.store(v, std::memory_order_release);
}

void ControlState::setBrake(u8 v)
{
    brake_.store(v, std::memory_order_release);
}

void ControlState::setDirection(Direction d)
{
    direction_.store(d, std::memory_order_release);
}

// ============================================================
// SNAPSHOT (COM context)
// ============================================================
ControlSnapshot ControlState::snapshot()
{
    ControlSnapshot s;

    // Sequence increments per transmitted FullState
    s.seq = g_state.seq_.fetch_add(1, std::memory_order_relaxed);

    // Consistent atomic read
    s.steering  = g_state.steering_.load(std::memory_order_acquire);
    s.throttle  = g_state.throttle_.load(std::memory_order_acquire);
    s.brake     = g_state.brake_.load(std::memory_order_acquire);
    s.direction = g_state.direction_.load(std::memory_order_acquire);

    // Timestamp (ms) — used for latency / stale-frame detection
    s.ts_ms = static_cast<u32>(
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count()
    );

    return s;
}
