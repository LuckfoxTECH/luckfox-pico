// com/RxDispatcher.cpp
#include "RxDispatcher.hpp"

#include "FrameCodec.hpp"
#include "PduTypes.hpp"

#include "app/EventRing.hpp"
#include "app/VehicleState.hpp"

// ============================================================
// EXTERN GLOBALS (RTE-style)
// ============================================================
extern EventRing<Event, 128> g_eventRing;
extern VehicleState         g_vehicleState;

// ============================================================
// DISPATCH (AUTOSAR Com_RxIndication)
// ============================================================
void RxDispatcher::dispatch(const Frame& frame)
{
    const PduType type = static_cast<PduType>(frame.cmd);

    // printf("[RX DISPATCH] cmd=0x%02X type=%d len=%d msg_id=%d\n",
    //        frame.cmd,
    //        static_cast<int>(type),
    //        frame.length,
    //        frame.msg_id);

    switch (type)
    {
    // ========================================================
    // VEHICLE STATE (main RX data)
    // ========================================================
    case PduType::VEHICLE_STATE:
    {
        VehicleStatePdu pdu{};
        if (!FrameCodec::decode_pdu(frame, pdu)) {
            printf("[DISPATCH] decode_pdu FAILED (len=%d, expect=%zu)\n",
                frame.length,
                sizeof(VehicleStatePdu));
            return;
        }

        std::cout << "[COM][VEHICLE_STATE] "
            << "speed=" << pdu.speed
            << " rpm=" << pdu.rpm
            << " fault=0x"
            << std::hex << int(pdu.fault_flags)
            << std::dec << "\n";

        // --- Update RX state ---
        g_vehicleState.setSpeed(pdu.speed);
        g_vehicleState.setRpm(pdu.rpm);
        g_vehicleState.setFaultFlags(pdu.fault_flags);

        // --- Notify application ---
        Event ev{};
        ev.type     = EventType::COM_RX;
        ev.u8_value = static_cast<u8>(PduType::VEHICLE_STATE);

        g_eventRing.push(ev);
        break;
    }

    // ========================================================
    // ACK / HEARTBEAT
    // ========================================================
    case PduType::ACK:
    {
        AckPdu pdu{};
        if (!FrameCodec::decode_pdu(frame, pdu))
            return;

        Event ev{};
        ev.type     = EventType::COM_RX;
        ev.u8_value = static_cast<u8>(PduType::ACK);

        g_eventRing.push(ev);
        break;
    }

    // ========================================================
    // EMERGENCY FROM VEHICLE
    // ========================================================
    case PduType::EMERGENCY:
    {
        EmergencyPdu pdu{};
        if (!FrameCodec::decode_pdu(frame, pdu))
            return;

        Event ev{};
        ev.type      = EventType::EMERGENCY;
        ev.u8_value  = static_cast<u8>(pdu.reason);
        ev.timestamp = pdu.timestamp;

        g_eventRing.push(ev);
        break;
    }

    // ========================================================
    // UNKNOWN
    // ========================================================
    default:
        break;
    }
}
