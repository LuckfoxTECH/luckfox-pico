#ifndef _LESMPCONST_H_
#define _LESMPCONST_H_
/*
******************************************************************************
* THIS INFORMATION IS PROPRIETARY TO
* BROADCOM CORP.
*-----------------------------------------------------------------------------
*
*           Copyright (c) 1999, 2011 Broadcom Corp.
*                      ALL RIGHTS RESERVED
*
******************************************************************************

******************************************************************************
*    File Name: lesmp_const.h
*
*    Abstract: This is the header file for LE SMP. It has the constants
*              defined in the specification.
*
*    $History:$
*
******************************************************************************
*/
#include "types.h"

// This is for WIN32 platform.
#ifdef __cplusplus
extern "C" {
#endif


#ifdef _WIN32
#include <pshpack1.h>
#endif
// GHS syntax.
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////
//    The following data structure and constants are coming from
//                        BT Spec. 4.0

// random number is 16 bytes.
#define LESMP_RAND_SIZE             16

// Max key is 16 bytes.
#define LESMP_MAX_KEY_SIZE          16
#define LESMP_MIN_KEY_SIZE          7

// Vol 3, Part H, 2.4.5
#define LESMP_CMAC_SIZE             8

typedef PACKED struct
{
    UINT8   Code;
} LESMP_SMP_HDR;


#define LESMP_CODE_RESERVED         0x00
#define LESMP_CODE_PAIRING_REQ      0x01
#define LESMP_CODE_PAIRING_RSP      0x02
#define LESMP_CODE_PAIRING_CONF     0x03
#define LESMP_CODE_PAIRING_RAND     0x04
#define LESMP_CODE_PAIRING_FAILED   0x05
#define LESMP_CODE_ENC_INFO         0x06
#define LESMP_CODE_MASTER_ID        0x07
#define LESMP_CODE_ID_INFO          0x08
#define LESMP_CODE_ID_ADDR_INFO     0x09
#define LESMP_CODE_SIGNING_INFO     0x0A
#define LESMP_CODE_SECURITY_REQ     0x0B




// IO Capability
#define LESMP_IO_CAP_DISP_ONLY                  0x00
#define LESMP_IO_CAP_DISP_YES_NO                0x01
#define LESMP_IO_CAP_DISP_KEYBOARD_ONLY         0x02
#define LESMP_IO_CAP_DISP_NO_IO                 0x03
#define LESMP_IO_CAP_DISP_KEYBOARD_DISP         0x04
#define LESMP_IO_CAP_MAX          LESMP_IO_CAP_DISP_KEYBOARD_DISP // last one              


// OOB data flag
#define LESMP_OOB_AUTH_DATA_NOT_PRESENT             0x00
#define LESMP_OOB_AUTH_DATA_FROM_REMOTE_PRESENT     0x01

// AuthReq
#define LESMP_AUTH_FLAG_BONDING_MASK            0x03
#define LESMP_AUTH_FLAG_NO_BONDING              0x00
#define LESMP_AUTH_FLAG_BONDING                 0x01

// MITM, Man In The Middle flag, bit[2] of AuthReq octet.
#define LESMP_AUTH_REQ_FLAG_MITM                0x04


// KEY DISTRIBUTION.
#define LESMP_KEY_DISTRIBUTION_ENC_KEY          0x1
#define LESMP_KEY_DISTRIBUTION_ID_KEY           0x2
#define LESMP_KEY_DISTRIBUTION_SIGN_KEY         0x4

typedef PACKED struct
{
    UINT8   IOCapability;
    UINT8   OOBDataFlag;
    UINT8   AuthReq;
    UINT8   MaxEncKeySize;
    UINT8   InitiatorKeyDistrib;
    UINT8   ResponderKeyDistrib;
} LESMP_PAIRING_REQ_CMD;


typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x01
    LESMP_PAIRING_REQ_CMD cmd;
} LESMP_PAIRING_REQ;



typedef PACKED struct
{
    UINT8   IOCapability;
    UINT8   OOBDataFlag;
    UINT8   AuthReq;
    UINT8   MaxEncKeySize;
    UINT8   InitiatorKeyDistrib;
    UINT8   ResponderKeyDistrib;
} LESMP_PAIRING_RSP_CMD;



typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x02
    LESMP_PAIRING_RSP_CMD cmd;
} LESMP_PAIRING_RSP;


// pairing confirmation and paring rand has similar
// structure.
typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x03 or 0x4.
    UINT8   Value[LESMP_RAND_SIZE]; // 16 bytes
} LESMP_PAIRING_CONF_AND_RAND;


#define LESMP_PAIRING_FAILED_REASON_RESERVED                0x0
#define LESMP_PAIRING_FAILED_REASON_PASS_KEY_FAILED     0x1
#define LESMP_PAIRING_FAILED_REASON_OOB_NOT_AVAIL           0x2
#define LESMP_PAIRING_FAILED_REASON_AUTH_REQUIREMENT        0x3
#define LESMP_PAIRING_FAILED_REASON_CONF_VAL_FAILED       0x4
#define LESMP_PAIRING_FAILED_REASON_PAIRING_NOT_SUPPORTED 0x5
#define LESMP_PAIRING_FAILED_REASON_ENCRYPTION_KEY_SIZE   0x6
#define LESMP_PAIRING_FAILED_REASON_CMD_NOT_SUPPORTED     0x7
#define LESMP_PAIRING_FAILED_REASON_UNSPECIFIED         0x8
#define LESMP_PAIRING_FAILED_REASON_REPEAT_ATTEMPT        0x9




typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x05
    UINT8   Reason;
} LESMP_PAIRING_FAILED;


typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x06
    UINT8   ltk[LESMP_MAX_KEY_SIZE];
} LESMP_PAIRING_ENC_INFO;


typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x07
    UINT16  ediv;      // 2 bytes.
    UINT8   rand[LESMP_MAX_KEY_SIZE / 2]; // this is 8 bytes.
} LESMP_PAIRING_MASTER_ID;


typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x08
    UINT8   irk[LESMP_MAX_KEY_SIZE]; //
} LESMP_PAIRING_IRK;


typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x09
    UINT8   addrType;  // address type
    UINT8   bd_addr[6]; // 6 bytes
} LESMP_PAIRING_ID_ADDR_INFO;

typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x0a
    UINT8   csrk[LESMP_MAX_KEY_SIZE]; // 16 bytes
} LESMP_PAIRING_SIGNING_INFO;


typedef PACKED struct
{
    UINT8   Code;      // This code should be 0x0b
    UINT8   authReq;
} LESMP_PAIRING_SECURITY_REQUEST;


#ifdef _WIN32
#include <poppack.h>
#endif
// restore the pack setting. GHS syntax.
#pragma pack()

//////////////////////////////////////////////////////////////////////////////
//


// This is for WIN32 platform.
#ifdef __cplusplus
}
#endif
#endif // end of #ifndef _LESMPCONST_H_
