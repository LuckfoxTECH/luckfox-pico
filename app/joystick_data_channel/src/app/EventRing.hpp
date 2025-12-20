// app/EventRing.hpp
#pragma once

#include <atomic>
#include <cstdint>
#include <cstddef>

// ============================================================
// BASIC TYPES
// ============================================================
using u8  = uint8_t;
using u32 = uint32_t;
using s16 = int16_t;

// ============================================================
// EVENT TYPES
// ============================================================
enum class EventType : u8
{
    STEERING,
    THROTTLE,
    BRAKE,
    DIRECTION,
    EMERGENCY
};

// ============================================================
// EVENT DATA
// ============================================================
struct Event
{
    EventType type;
    s16       s16_value{0};
    u8        u8_value{0};
    u32       timestamp{0};
};

// ============================================================
// EVENT RING (SPSC, Lock-Free)
// ============================================================
template<typename T, size_t N>
class EventRing
{
public:
    // --------------------------------------------------------
    // Init (must be called before use)
    // --------------------------------------------------------
    static void init(EventRing& ring)
    {
        ring.head.store(0, std::memory_order_relaxed);
        ring.tail.store(0, std::memory_order_relaxed);
    }

    // --------------------------------------------------------
    // Push (Producer)
    // --------------------------------------------------------
    bool push(const T& v)
    {
        const size_t h = head.load(std::memory_order_relaxed);
        const size_t n = next(h);

        // full
        if (n == tail.load(std::memory_order_acquire))
            return false;

        buffer[h] = v;
        head.store(n, std::memory_order_release);
        return true;
    }

    // --------------------------------------------------------
    // Pop (Consumer)
    // --------------------------------------------------------
    bool pop(T& out)
    {
        const size_t t = tail.load(std::memory_order_relaxed);

        // empty
        if (t == head.load(std::memory_order_acquire))
            return false;

        out = buffer[t];
        tail.store(next(t), std::memory_order_release);
        return true;
    }

    // --------------------------------------------------------
    // Status helpers (optional)
    // --------------------------------------------------------
    bool empty() const
    {
        return head.load(std::memory_order_acquire) ==
               tail.load(std::memory_order_acquire);
    }

    bool full() const
    {
        return next(head.load(std::memory_order_acquire)) ==
               tail.load(std::memory_order_acquire);
    }

private:
    static constexpr size_t next(size_t v)
    {
        return (v + 1) % N;
    }

private:
    T buffer[N];
    std::atomic<size_t> head{0}; // producer
    std::atomic<size_t> tail{0}; // consumer
};

// ============================================================
// GLOBAL EVENT RING
// ============================================================
extern EventRing<Event, 128> g_eventRing;
