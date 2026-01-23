// com/PduTypes.hpp
#pragma once

#include <cstdint>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s16 = int16_t;

/* ================================
 * AUTOSAR-LIKE PDU TYPE ID
 * ================================ */
enum class PduType : u8
{
    // -------- TX : Client -> Server --------
    FULL_STATE     = 0x01,   // joystick / control command

    // -------- RX : Server -> Client --------
    VEHICLE_STATE  = 0x10,   // speed, rpm, faults
    ACK            = 0x11,   // ack
    HEARTBEAT      = 0x12,   // heartbeat 


    // -------- BOTH DIR --------
    EMERGENCY      = 0xF0    // emergency notify
};

#pragma pack(push,1)

/* ================================
 * FULL STATE PDU (TX)
 * ================================ */
struct FullStatePdu
{
    s16 steering;     // -1000 .. +1000
    u8  throttle;     // 0 .. 100
    u8  brake;        // 0 .. 100
    u8  direction;    // enum Direction

    u16 seq;          // FULL 16-bit sequence
    u32 ts_ms;        // sender timestamp (ms)
};

#pragma pack(pop)
