// com/FrameCodec.cpp
#include "Frame.hpp"
#include "PduTypes.hpp"
#include <cstring>
#include <vector>

static u8 calc_checksum(const Frame& f)
{
    u8 sum = f.sof ^ f.msg_id ^ f.cmd ^ f.length;
    for (u8 i = 0; i < f.length; ++i)
        sum ^= f.payload[i];
    return sum;
}

template<typename T>
static std::vector<u8> serialize_pdu(const T& pdu)
{
    std::vector<u8> out(sizeof(T));
    std::memcpy(out.data(), &pdu, sizeof(T));
    return out;
}

std::vector<u8> build_frame_from_pdu(
    const void* pdu,
    u8 pdu_size,
    u8 pdu_type,
    u8 msg_id)
{
    Frame f{};
    f.sof    = FRAME_SOF;
    f.msg_id = msg_id;
    f.cmd    = pdu_type;
    f.length = pdu_size;

    std::memcpy(f.payload, pdu, pdu_size);
    f.checksum = calc_checksum(f);

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
    size_t len,
    Frame& out)
{
    if (len < 5) return false;
    if (buf[0] != FRAME_SOF) return false;

    out.sof    = buf[0];
    out.msg_id = buf[1];
    out.cmd    = buf[2];
    out.length = buf[3];

    if (out.length > FRAME_MAX_PAYLOAD) return false;
    if (len != (5 + out.length)) return false;

    std::memcpy(out.payload, &buf[4], out.length);
    out.checksum = buf[4 + out.length];

    return out.checksum == calc_checksum(out);
}

template<typename T>
bool deserialize_pdu(const Frame& f, T& out)
{
    if (f.length != sizeof(T))
        return false;

    std::memcpy(&out, f.payload, sizeof(T));
    return true;
}
