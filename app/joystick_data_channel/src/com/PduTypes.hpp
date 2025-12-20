// com/PduTypes.hpp
#pragma once

#include <cstdint>
#include "app/SafetyManager.hpp"  

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s16 = int16_t;

/* ================================
 * AUTOSAR PDU TYPE ID
 * ================================ */
enum class PduType : u8
{
    FULL_STATE = 0x01,
    ACK        = 0x02,
    EMERGENCY  = 0x03
};

#pragma pack(push,1)

/* ================================
 * FULL STATE PDU
 * ================================ */
struct FullStatePdu
{
    //PduType type;
    s16     steering;
    u8      throttle;
    u8      brake;
    u8      direction;
    //u32     seq;
};

/* ================================
 * ACK PDU
 * ================================ */
struct AckPdu
{
    //PduType type;
    //u32     seq;
    u8      result;
};

/* ================================
 * EMERGENCY PDU
 * ================================ */
struct EmergencyPdu
{
    //PduType         type;
    EmergencyReason reason;   
    u32             timestamp;
};

#pragma pack(pop)
