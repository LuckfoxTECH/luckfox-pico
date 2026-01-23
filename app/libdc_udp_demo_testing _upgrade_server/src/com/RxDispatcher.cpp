// com/RxDispatcher.cpp
#include "RxDispatcher.hpp"

#include "FrameCodec.hpp"
#include "PduTypes.hpp"

#include "app/EventRing.hpp"
#include "app/ControlState.hpp"

// ============================================================
// EXTERN GLOBALS (RTE-style)
// ============================================================
extern EventRing<Event, 128> g_eventRing;
extern ControlState g_state;
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
    case PduType::FULL_STATE:
    {
        FullStatePdu pdu{};
        if (!FrameCodec::decode_pdu(frame, pdu)) {
            printf("[DISPATCH] decode_pdu FAILED (len=%d, expect=%zu)\n",
                frame.length,
                sizeof(FullStatePdu));
            return;
        }

        // --- Debug print RX data ---
        printf("[RX][FULL_STATE] "
            "steering=%d throttle=%u brake=%u direction=%u\n",
            static_cast<int>(pdu.steering),
            static_cast<unsigned>(pdu.throttle),
            static_cast<unsigned>(pdu.brake),
            static_cast<unsigned>(pdu.direction));

        // --- Update RX state ---
        g_state.setSteering(pdu.steering);
        g_state.setThrottle(pdu.throttle);
        g_state.setBrake(pdu.brake);
        g_state.setDirection(static_cast<Direction>(pdu.direction));

        // --- Notify application ---
        Event ev{};
        ev.type     = EventType::COM_RX;
        ev.u8_value = static_cast<u8>(PduType::FULL_STATE);

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
