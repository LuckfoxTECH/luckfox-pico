// com/FrameCodec.cpp
#include "FrameCodec.hpp"
#include "app/ControlState.hpp"

#include <cstring>

// ============================================================
// INTERNAL UTILITIES (private to this TU)
// ============================================================

static u8 calc_checksum(const Frame& f)
{
    u8 sum = f.sof ^ f.msg_id ^ f.cmd ^ f.length;

    for (u8 i = 0; i < f.length; ++i)
        sum ^= f.payload[i];

    return sum;
}

// ============================================================
// LOW-LEVEL FRAME API
// ============================================================

std::vector<u8> build_frame_from_pdu(
    const void* pdu,
    u8          pdu_size,
    u8          pdu_type,
    u8          msg_id)
{
    Frame f{};
    f.sof    = FRAME_SOF;
    f.msg_id = msg_id;
    f.cmd    = pdu_type;
    f.length = pdu_size;

    // Defensive: payload size check
    if (pdu_size > FRAME_MAX_PAYLOAD)
        return {};

    std::memcpy(f.payload, pdu, pdu_size);
    f.checksum = calc_checksum(f);

    // Serialize frame
    std::vector<u8> out;
    out.reserve(5 + f.length);

    out.push_back(f.sof);
    out.push_back(f.msg_id);
    out.push_back(f.cmd);
    out.push_back(f.length);

    for (u8 i = 0; i < f.length; ++i)
        out.push_back(f.payload[i]);

    out.push_back(f.checksum);

    return out;
}

bool parse_frame(
    const u8* buf,
    size_t    len,
    Frame&    out)
{
    // Minimum frame: SOF + MSG_ID + CMD + LEN + CHECKSUM
    if (len < 5)
        return false;

    if (buf[0] != FRAME_SOF)
        return false;

    out.sof    = buf[0];
    out.msg_id = buf[1];
    out.cmd    = buf[2];
    out.length = buf[3];

    if (out.length > FRAME_MAX_PAYLOAD)
        return false;

    if (len != static_cast<size_t>(5 + out.length))
        return false;

    std::memcpy(out.payload, &buf[4], out.length);
    out.checksum = buf[4 + out.length];

    return (out.checksum == calc_checksum(out));
}

// ============================================================
// HIGH-LEVEL FRAME CODEC (AUTOSAR COM)
// ============================================================

std::vector<u8>
FrameCodec::build_fullstate_frame(const ControlSnapshot& snap)
{
    static u8 rolling_msg_id = 0;

    FullStatePdu pdu{};
    pdu.steering  = static_cast<s16>(snap.steering);
    pdu.throttle  = static_cast<u8>(snap.throttle);
    pdu.brake     = static_cast<u8>(snap.brake);
    pdu.direction = static_cast<u8>(snap.direction);

    pdu.seq   = static_cast<u16>(snap.seq);
    pdu.ts_ms = snap.ts_ms;

    return build_frame_from_pdu(
        &pdu,
        sizeof(pdu),
        static_cast<u8>(PduType::FULL_STATE),
        rolling_msg_id++
    );
}


bool FrameCodec::decode_frame(
    const u8* buf,
    size_t    len,
    Frame&    out)
{
    return parse_frame(buf, len, out);
}
