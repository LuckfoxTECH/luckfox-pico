// com/FrameCodec.hpp
#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "Frame.hpp"
#include "PduTypes.hpp"

// ============================================================
// BASIC TYPES
// ============================================================
using u8 = uint8_t;

// Forward declaration (no app dependency)
struct ControlSnapshot;

// ============================================================
// LOW-LEVEL FRAME API
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
    // -----------------------------
    // TX
    // -----------------------------
    static std::vector<u8>
    build_fullstate_frame(const ControlSnapshot& snap);

    // -----------------------------
    // RX
    // -----------------------------
    static bool decode_frame(
        const u8* buf,
        size_t    len,
        Frame&    out
    );

    template<typename T>
    static bool decode_pdu(const Frame& frame, T& out)
    {
        if (frame.length != sizeof(T))
            return false;

        std::memcpy(&out, frame.payload, sizeof(T));
        return true;
    }
};
