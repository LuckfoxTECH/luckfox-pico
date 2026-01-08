// com/PduTypes.hpp
#pragma once

#include <cstdint>
#include "app/SafetyManager.hpp"
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
    s16 steering;    // steering angle
    u8  throttle;    // throttle %
    u8  brake;       // brake %
    u8  direction;   // enum Direction
};

/* ================================
 * VEHICLE STATE PDU (RX)
 * ================================ */
struct VehicleStatePdu
{
    u16 speed;        // km/h * 10
    u16 rpm;          // engine rpm
    u32 fault_flags;  // bitmask
};

/* ================================
 * ACK / HEARTBEAT PDU (RX)
 * ================================ */
struct AckPdu
{
    u8 result;        // 0 = OK, !=0 = error
};

/* ================================
 * EMERGENCY PDU (TX & RX)
 * ================================ */
struct EmergencyPdu
{
    EmergencyReason reason;   // enum
    u32             timestamp;
};

#pragma pack(pop)
