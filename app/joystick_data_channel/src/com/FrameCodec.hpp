#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>

#include "Frame.hpp"
#include "PduTypes.hpp"
#include "app/ControlState.hpp"

// ============================================================
// BASIC TYPES
// ============================================================
using u8 = uint8_t;

// ============================================================
// LOW-LEVEL FRAME API (implemented in FrameCodec.cpp)
// ============================================================
std::vector<u8> build_frame_from_pdu(
    const void* pdu,
    u8          pdu_size,
    u8          pdu_type,
    u8          msg_id
);

bool parse_frame(
    const u8* buf,
    size_t    len,
    Frame&    out
);

// ============================================================
// HIGH-LEVEL FRAME CODEC (AUTOSAR COM)
// ============================================================
class FrameCodec
{
public:
    // --------------------------------------------------------
    // FullState → Frame
    // --------------------------------------------------------
    static std::vector<u8>
    build_fullstate_frame(const ControlSnapshot& snap)
    {
        FullStatePdu pdu{};
        pdu.type      = PduType::FULL_STATE;
        pdu.steering  = snap.steering;
        pdu.throttle  = snap.throttle;
        pdu.brake     = snap.brake;
        pdu.direction = static_cast<u8>(snap.direction);
        pdu.seq       = snap.seq;

        return build_frame_from_pdu(
            &pdu,
            sizeof(pdu),
            static_cast<u8>(PduType::FULL_STATE),
            static_cast<u8>(snap.seq & 0xFF)
        );
    }

    // --------------------------------------------------------
    // Emergency → Frame
    // --------------------------------------------------------
    static std::vector<u8>
    build_emergency_frame(EmergencyReason reason, u32 timestamp)
    {
        EmergencyPdu pdu{};
        pdu.type      = PduType::EMERGENCY;
        pdu.reason    = reason;
        pdu.timestamp = timestamp;

        return build_frame_from_pdu(
            &pdu,
            sizeof(pdu),
            static_cast<u8>(PduType::EMERGENCY),
            0xFF
        );
    }
};
