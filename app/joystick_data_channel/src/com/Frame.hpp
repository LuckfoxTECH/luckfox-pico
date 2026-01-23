// com/Frame.hpp
#pragma once
#include <cstdint>

using u8 = uint8_t;

/*
 * | SOF | MSG_ID | CMD | LEN | PAYLOAD | CHECKSUM |
 */
#pragma pack(push,1)
struct Frame
{
    u8 sof;              // 0xAA
    u8 msg_id;           // rolling / seq low byte
    u8 cmd;              // maps to PduType
    u8 length;           // payload length
    u8 payload[32];      // fixed max
    u8 checksum;         // XOR checksum
};
#pragma pack(pop)

/* ================================
 * Frame constants
 * ================================ */
static constexpr u8 FRAME_SOF = 0xAA;
static constexpr u8 FRAME_MAX_PAYLOAD = 32;
