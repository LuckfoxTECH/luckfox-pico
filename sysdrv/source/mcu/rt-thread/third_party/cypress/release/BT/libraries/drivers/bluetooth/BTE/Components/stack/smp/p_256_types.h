/*
********************************************************************
* THIS INFORMATION IS PROPRIETARY TO
* BROADCOM CORP.
*-------------------------------------------------------------------
*
*           Copyright (c) 2003 Broadcom Corp.
*                      ALL RIGHTS RESERVED
*
********************************************************************

********************************************************************
*    File Name: Types.h
*
*    Abstract: Basic type definitions for the BCM204X Baseband Controller
*              Firmware
*
*    $History:$
*
********************************************************************
*/

#ifndef P_256_TYPES_H
#define P_256_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

//
// Basic Types
//

#if defined(WIN32)
typedef char                CHAR;
typedef short               SHORT;
typedef long                LONG;
#if 0
typedef unsigned char       UINT8;
typedef signed   char       INT8;
typedef unsigned short      UINT16;
typedef signed   short      INT16;
typedef unsigned int        UINT32;
typedef signed   int        INT32;
typedef unsigned __int64    UINT64;
#endif
typedef signed   __int64    INT64;

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef unsigned char       uint8;
typedef signed   char       int8;
typedef unsigned short      uint16;
typedef signed   short      int16;
typedef unsigned long       uint32;
typedef signed   long       int32;

#else

#if 0
typedef unsigned char       UINT8;
typedef signed   char       INT8;
typedef unsigned short      UINT16;
typedef signed   short      INT16;
typedef unsigned long       UINT32;
typedef signed   long       INT32;
typedef unsigned long long  UINT64;
#endif
typedef signed long long    INT64;

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef unsigned char       uint8;
typedef signed   char       int8;
typedef unsigned short      uint16;
typedef signed   short      int16;
typedef unsigned long       uint32;
typedef signed   long       int32;

#endif // win32

// Machine dependent data types, for processor efficiency
typedef unsigned int MBOOL;
typedef unsigned int MU8;
typedef unsigned int MU16;
typedef unsigned int MU32;
typedef int MI8;
typedef int MI16;
typedef int MI32;


// This definition is for backward compatibility
typedef UINT32          BOOL32;
typedef UINT8            BOOL8;

#ifndef NULL
#define NULL 0
#endif


//
// Rounding macros
//
#define ROUND2(n)       (((n) + 1) & ~1)
#define ROUND4(n)       (((n) + 3) & ~3)
#define ROUND8(n)       (((n) + 7) & ~7)

//
// inline macro
//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
//
// #define FOO FREESODA
// printf("%s    %s\n", STRINGIFY(FOO), TOSTRING(FOO));
//
// The code above will generate the following output:
//        FOO    FREESODA

//
// Assume we are compiled under Little Endian system.
#define BE_SWAP_16(var)  (( UINT16)( ((var) << 8) | (((var) >> 8) & 0xff) ) )
#define LE_SWAP_16(var)  (var)
#define BE_SWAP_32(var)  (     ((var & 0x000000FF)<<24)  \
                           |   ((var & 0x0000FF00)<<8)   \
                           |   ((var & 0x00FF0000)>>8)   \
                           |  ((var & 0xFF000000)>>24)   \
                         )
#define LE_SWAP_32(var)    (var)

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifdef __cplusplus
}
#endif

/*
*******************************************************************************
*
* $HISTORY:$
*
*******************************************************************************
*/
#endif /* P_256_TYPES_H */
