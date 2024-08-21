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

#ifndef __TYPES_H__
#define    __TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

//
// Basic Types
//
typedef unsigned char            BOOL8;
#ifndef VOID
#define VOID void
#endif

#if defined(WIN32)
typedef char                CHAR;
typedef short               SHORT;
typedef long                LONG;
typedef unsigned char       UINT8;
typedef signed   char       INT8;
typedef unsigned short      UINT16;
typedef signed   short      INT16;
typedef unsigned int        UINT32;
typedef signed   int        INT32;
typedef signed   __int64    INT64;
typedef unsigned __int64    UINT64;

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef unsigned char       uint8;
typedef signed   char       int8;
typedef unsigned short      uint16;
typedef signed   short      int16;
typedef unsigned long       uint32;
typedef signed   long       int32;

#elif defined(__linux__)
#include <stdint.h>
typedef uint8_t       UINT8;
typedef int8_t        INT8;
typedef uint16_t      UINT16;
typedef int16_t       INT16;
typedef uint32_t      UINT32;
typedef int32_t       INT32;
typedef int64         INT64;
typedef uint64_t      UINT64;
#else

#if defined( __IAR_SYSTEMS_ICC__ )
#if 0
typedef __UINT8_T_TYPE__    UINT8;
typedef __INT8_T_TYPE__     INT8;
typedef __UINT16_T_TYPE__   UINT16;
typedef __INT16_T_TYPE__    INT16;
typedef __UINT32_T_TYPE__   UINT32;
typedef __INT32_T_TYPE__    INT32;
typedef __INT64_T_TYPE__    INT64;
typedef __UINT64_T_TYPE__   UINT64;
#endif
#else
typedef unsigned char       UINT8;
typedef signed   char       INT8;
typedef unsigned short      UINT16;
typedef signed   short      INT16;
typedef unsigned long       UINT32;
typedef signed   long       INT32;
typedef signed long long    INT64;
typedef unsigned long long  UINT64;
#endif /* defined( __IAR_SYSTEMS_ICC__ ) */

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
#ifndef _WIN32
typedef UINT8            BOOL;
#endif

#ifndef BOOLEAN
typedef UINT8            BOOLEAN;
#endif

#ifndef BOOL32
typedef UINT32          BOOL32;
#endif

#ifndef BOOL8
typedef UINT8            BOOL8;
#endif

#ifndef NULL
#define NULL 0
#endif


//
// Basic Definitions
//

#ifndef TRUE
#define    TRUE    1
#endif

#ifndef FALSE
#define    FALSE    0
#endif


//
// pack macro
//
#if defined(COMPILER_ARM)
#define PACKED    __packed
#define ALIGN4  __align(4)
#define ALIGN8  __align(8)
#define ATTRIBUTE(x) __attribute__(x)
#define PLACE_IN_DROM1 (section("const_drom_var"))
//Satya -- need this for seamless debugging..no run time penalty
#define PLACE_IN_DROM ATTRIBUTE(PLACE_IN_DROM1)
#else // some makefiles may not define COMPILER_GHS
#define PACKED
#define ALIGN4      // use #pragma alignvar(4) ... #pragma alignvar(1)
#define ALIGN8      // use #pragma alignvar(8) ... #pragma alignvar(1)
#define ATTRIBUTE(x)
#define PLACE_IN_DROM
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

#ifndef INLINE
#ifdef __cplusplus
#if defined(COMPILER_ARM)
#define INLINE __inline
#else
#define INLINE inline
#endif
#else
#define INLINE __inline
#endif
#endif

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

void nv_log_assert(char *file, int line, UINT32 status);
BOOL32 nv_log_is_asserts_present(void);

#ifdef DUMP_STACK_AT_CRASH
void nv_log_assert_stack(UINT32 *sp, UINT32 offset);
void nv_log_assert_ext(char *file, int line, UINT32 status, int status_ext);
#endif


#ifndef ASSERT
#ifdef ENABLE_ASSERT

#ifdef FPGA_BD_2045
// To compliant with MISRA coding standards
void assert_fail(char   *file,   INT32  line, UINT32 status, char *fatal);

#define   ASSERT( X , status)               do{ if((X) == FALSE)    assert_fail(__FILE__, __LINE__, (UINT32)status, NULL);} while(0)
//void assert_fail( char*   file,   int  line , int status, char* fatal);
#define ASSERT_FATAL(X)                  assert_fail( __FILE__, __LINE__, 0, X)
#else
// To compliant with MISRA coding standards
void assert_fail(char   *file,   INT32  line, UINT32 status);

#define   ASSERT( X , status)               do {if((X) == FALSE)    assert_fail(__FILE__, __LINE__, (UINT32)status);} while(0)
#define ASSERT_FATAL(X)                  assert_fail( __FILE__, __LINE__, 0);

#endif

#else // compile assertion out
#define   ASSERT( X , status)
#define   ASSERT_FATAL(X)
#endif
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
#endif // __TYPES_H__
