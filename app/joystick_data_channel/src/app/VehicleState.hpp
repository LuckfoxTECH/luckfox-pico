// app/VehicleState.hpp
#pragma once

#include <atomic>
#include <cstdint>

// ============================================================
// BASIC TYPES
// ============================================================
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

// ============================================================
// VEHICLE STATE SNAPSHOT (RX view)
// ============================================================
struct VehicleSnapshot
{
    u32 seq;          // RX sequence / rolling counter
    u16 speed;        // vehicle speed
    u16 rpm;          // engine / motor rpm
    u32 fault_flags;  // bitmask
};

// ============================================================
// VEHICLE STATE (AUTOSAR Receiver SWC)
// ============================================================
class VehicleState
{
public:
    // --------------------------------------------------------
    // Init (called once at system startup)
    // --------------------------------------------------------
    static void init();

    // --------------------------------------------------------
    // Setters (called by RxDispatcher)
    // --------------------------------------------------------
    void setSpeed(u16 v);
    void setRpm(u16 v);
    void setFaultFlags(u32 flags);

    // --------------------------------------------------------
    // Snapshot (called by Application / HMI / Control logic)
    // --------------------------------------------------------
    static VehicleSnapshot snapshot();

private:
    // --------------------------------------------------------
    // Internal atomic state
    // --------------------------------------------------------
    std::atomic<u16> speed_{0};
    std::atomic<u16> rpm_{0};
    std::atomic<u32> fault_flags_{0};
    std::atomic<u32> seq_{0};
};

// ============================================================
// GLOBAL INSTANCE (RTE-style)
// ============================================================
extern VehicleState g_vehicleState;
