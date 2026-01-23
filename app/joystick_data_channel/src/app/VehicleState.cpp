// app/VehicleState.cpp
#include "VehicleState.hpp"

// ============================================================
// GLOBAL INSTANCE
// ============================================================
VehicleState g_vehicleState;

// ============================================================
// INIT
// ============================================================
void VehicleState::init()
{
    g_vehicleState.speed_.store(0, std::memory_order_relaxed);
    g_vehicleState.rpm_.store(0, std::memory_order_relaxed);
    g_vehicleState.fault_flags_.store(0, std::memory_order_relaxed);
    g_vehicleState.seq_.store(0, std::memory_order_relaxed);
}

// ============================================================
// SETTERS (RxDispatcher context)
// ============================================================
void VehicleState::setSpeed(u16 v)
{
    speed_.store(v, std::memory_order_release);
    seq_.fetch_add(1, std::memory_order_relaxed);
}

void VehicleState::setRpm(u16 v)
{
    rpm_.store(v, std::memory_order_release);
}

void VehicleState::setFaultFlags(u32 flags)
{
    fault_flags_.store(flags, std::memory_order_release);
}

// ============================================================
// SNAPSHOT (Application context)
// ============================================================
VehicleSnapshot VehicleState::snapshot()
{
    VehicleSnapshot s;

    s.seq = g_vehicleState.seq_.load(std::memory_order_relaxed);

    s.speed       = g_vehicleState.speed_.load(std::memory_order_acquire);
    s.rpm         = g_vehicleState.rpm_.load(std::memory_order_acquire);
    s.fault_flags = g_vehicleState.fault_flags_.load(std::memory_order_acquire);

    return s;
}
