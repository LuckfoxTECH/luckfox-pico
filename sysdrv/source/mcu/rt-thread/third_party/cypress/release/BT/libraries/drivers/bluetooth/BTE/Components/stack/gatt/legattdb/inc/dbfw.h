/*
********************************************************************
* THIS INFORMATION IS PROPRIETARY TO
* BROADCOM CORP.
*-------------------------------------------------------------------
*
*           Copyright (c) 1999, 2009 Broadcom Corp.
*                      ALL RIGHTS RESERVED
*
********************************************************************

********************************************************************
*    File Name: dbfw.h
*
*    Abstract:
*       This is based upon BCM2045 Debug Framework Software Design Document ver 0.6
*
*    $History:$
*
********************************************************************
*/

#ifndef __DBFW_H__
#define __DBFW_H__

#ifdef DEBUG_FRAMEWORK

#include "fid.h"        // File ID
#include "dbfw_fp.h"    // Footprint ID
#include "types.h"
#if defined(COMPILER_GHS)
#include "arm_ghs.h"
#endif

#define __AT__ __FUNCTION__ ":" TOSTRING(__LINE__)
#define __LOC__ __FILE__##" "##__AT__

#define DBFW_PAD_OUTPUT_ADDR (((UINT32*)gpiodata_adr) + (1 << dbfw_outputPadIndex))

#if defined(COMPILER_GHS)
#define CALLER_ADDR     __builtin_return_address(0)
#else
#define CALLER_ADDR     0
#endif

#ifdef BT_DEBUG_UART
#define dbfw_dbguart_write_byte(x) while(dbguart_write(x) == DEBUG_UART_TX_FULL){}
#else
#define dbfw_dbguart_write_byte(x)
#endif

#define DBFW_TRACE_FORMAT_VERSION       1       // Nokia IF
#define DBFW_CORE_DUMP_FORMAT_VERSION   1

#define DBFW_TRACE_QUEUE_SIZE     32  // # of traces in queue
#define DBFW_HCI_DUMP_BUF_SIZE    240

#define DBFW_TRACE_MODULE_EXT_T 0x1F    // if trace_module_id == TRACE_EXT, then use TRACE_EXT_t (TBD)

#define DBFW_MODULE_ID_MAX      (30)    //  0 - 30
#define DBFW_FILE_ID_MAX        (2047)  //  0 - 2047
#define DBFW_LINE_NUMBER_MAX    (16383) //  1 - 16383, 0: trace_ts is used

// Global Warning Flag
#define DBFW_WARNING_OS_QUEUE_ALLOC_FAIL        0x00000001
#define DBFW_WARNING_OS_QUEUE_FREE_FAIL         0x00000002
#define DBFW_WARNING_OS_QUEUE_SEND_FAIL         0x00000004
#define DBFW_WARNING_OS_QUEUE_SEND_FRONT_FAIL   0x00000008
#define DBFW_WARNING_OS_BLOCK_POOL_FREE_FAIL    0x00000010
#define DBFW_WARNING_OS_EXCEPTION               0x00000100

// Module ID
enum
{
    MODULE_ID_DBFW   = 0,
    MODULE_ID_BTC    = 1,
    MODULE_ID_LC     = 2,
    MODULE_ID_LM     = 3,
    MODULE_ID_HCI    = 4,
    MODULE_ID_TRAN   = 5,
    MODULE_ID_FM     = 6,
    MODULE_ID_BCS    = 7,
    MODULE_ID_ULP    = 8,
    MODULE_ID_UHE    = 9,
    MODULE_ID_BLEAPP = 0x0A,
    MODULE_ID_OTHERS =  DBFW_MODULE_ID_MAX,
};

// Levels of Trace

enum
{
    TRACE_DEBUG     = 0,
    TRACE_INFO      = 1,
    TRACE_WARNING   = 2,
    TRACE_CRITICAL  = 3,
};

// Trace Mode
enum
{
    TRACE_MODE_STOP = 0,
    TRACE_MODE_POLL = 1,
    TRACE_MODE_PUSH = 2,
};

// Trace Flow
enum
{
    TRACE_FLOW_STOP_WHEN_FULL      = 0,
    TRACE_FLOW_OVERWRITE_WHEN_FULL = 1,
};

// Output Channel Selection
enum
{
    DBFW_OUTPUT_CHANNEL_HCI             = 0,
    DBFW_OUTPUT_CHANNEL_DBGUART_BIN     = 1,
    DBFW_OUTPUT_CHANNEL_DBGUART_ASCII   = 2,
    DBFW_OUTPUT_CHANNEL_GPS_NPE_DEBUG   = 3,
};

#define TRACE_FIELD_LINE_MASK   0x03FFF
#define TRACE_LINE_MASK         0x03FFF

#define TRACE_TIMESTAMP_HALF_SLOT_BT_CLK    0x00
#define TRACE_TIMESTAMP_1_US                0x01

// Trace Internal Format
typedef PACKED struct
{
    //UINT32  trace_level:2;      // TRACE_CRITICAL, TRACE_WARNING, TRACE_INFO or TRACE_DEBUG
    //UINT32  trace_module_id:5;  // which module this trace belongs to
    UINT32  unused: 7;
    UINT32  file_id: 11;        // file id
    UINT32  line_number: 14;    // if line number is 0, trace_var is a timestamp
    //    line number is 1, trace_var is us
    //    line 2 - 20 are reserved for different usages of trace_var
    UINT32  trace_var;          // trace variable / timestamp in half BT slot unit
} TRACE_t;

//  Default Configuration
#define DBFW_CONFIG_DEFAULT_MODULEMASK     0x7FFFFFFF
#define DBFW_CONFIG_DEFAULT_TRACE_LEVEL    TRACE_CRITICAL
#define DBFW_CONFIG_DEFAULT_TRACEDUMP_OUTPUT_CHANNEL    DBFW_OUTPUT_CHANNEL_HCI
#define DBFW_CONFIG_DEFAULT_STACKDUMP_OUTPUT_CHANNEL    DBFW_OUTPUT_CHANNEL_HCI
#define DBFW_CONFIG_DEFAULT_COREDUMP_OUTPUT_CHANNEL     DBFW_OUTPUT_CHANNEL_HCI
#define DBFW_CONFIG_DEFAULT_TRACE_MODE     TRACE_MODE_STOP
#define DBFW_CONFIG_DEFAULT_TRACE_FLOW     TRACE_FLOW_OVERWRITE_WHEN_FULL
#define DBFW_CONFIG_DEFAULT_TRACE_PUSH_TIMEOUT_IN_MS   100
#define DBFW_CONFIG_DEFAULT_TRACE_PUSH_WATERMARK       8

// Configuration
typedef struct
{
    UINT32  moduleMask;
    UINT16  push_timeout_in_ms; // ms
    UINT16  trace_watermark;
    UINT8   traceFormatVersion;
    UINT8   coreDumpFormatVersion;
    UINT8   trace_level;
    UINT8   trace_mode;
    UINT8   trace_flow;
    UINT8   tracedump_output_channel_selection;
    UINT8   stackdump_output_channel_selection;
    UINT8   coredump_output_channel_selection;
} DBFW_CONFIG_t;
extern DBFW_CONFIG_t dbfw_config;

#define DEBUG_FRAMEWORK_DUMP_TYPE_TRACE     'T'
#define DEBUG_FRAMEWORK_DUMP_TYPE_STACK     'S'
#define DEBUG_FRAMEWORK_DUMP_TYPE_CORE      'C'
#define DEBUG_FRAMEWORK_DUMP_TYPE_DBINT32   'D'

// Trace Dump

#define TRACE_INFO_STATUS_FULL      (1 << 0)
#define TRACE_INFO_STATUS_OVERFLOW  (1 << 1)

typedef struct
{
    INT32   cnt;
    INT32   poll_nof_traces;
    UINT32  missed;
    UINT16  rptr;
    UINT16  wptr;
    UINT16  cnt2;
    UINT8   status;
    BOOL8   goDump;
} TRACE_INFO_t;

// Stack Dump

#define DBFW_STACK_DUMP_NOF_SCAN_DEPTH_IN_BYTE  1024
#define DBFW_STACK_DUMP_NV_LOG_FORMAT_ID        'S'
#define DBFW_STACK_DUMP_NV_LOG_FILE_NAME_SIZE   18
#define DBFW_STACK_DUMP_MAX_NOF_CALLS           20

extern UINT8 dbfw_stack_dump_assertindex;

typedef PACKED struct
{
    UINT8   ID; //'S'
    UINT8   nv_log_chksum;
    UINT8   nof_calls;
    UINT8   assert_idx;
    char    file_name[DBFW_STACK_DUMP_NV_LOG_FILE_NAME_SIZE];
    UINT16  line_number;
} STACK_DUMP_NV_LOG_INFO_t;

typedef PACKED struct
{
    STACK_DUMP_NV_LOG_INFO_t info;
    UINT32  calls[DBFW_STACK_DUMP_MAX_NOF_CALLS];
} STACK_DUMP_NV_LOG_t;

// Core Dump

#define DBFW_CORE_DUMP_PKT_TYPE_DUMP_INFO   1
#define DBFW_CORE_DUMP_PKT_TYPE_CPU_REGS    2
#define DBFW_CORE_DUMP_PKT_TYPE_RAM_IMAGE   3
#define DBFW_CORE_DUMP_PKT_TYPE_HW_REGS     4

#define DBFW_COREDUMP_TAG_PREFETCH_HANDLER      0x10
#define DBFW_COREDUMP_TAG_UNDEFINED_HANDLER     0x11
#define DBFW_COREDUMP_TAG_ABORT_HANDLER         0x12
#define DBFW_COREDUMP_TAG_ISR_FIQ               0x13
#define DBFW_COREDUMP_TAG_WDOG                  0x14

#if 0
/* This struct must match what isr.arm does in the exception handlers,
 * and what bld/mpserv_bt.mbs does in its scripted breakpoints.
 * IF YOU CHANGE IT, don't forget to change the others. */
typedef struct arm_regs_st
{
    UINT32 sp;
    UINT32 lr;
    UINT32 cpsr;
    UINT32 spsr;
    UINT32 saved_sp; /* for the current mode -- i.e. LR_abt if ABT */
    UINT32 saved_lr; /* likewise */
    UINT32 r[8];     /* R0-R7 */

    /* The other registers -- e.g. R8-R12, or LR_und when in ABT mode
       -- aren't saved, not much use for them. */
} arm_regs;
#endif

extern char *dbfw_memInit(char *mm_top);
extern void dbfw_init(void);
extern void trace(UINT32 level_module_id, UINT32 fid, UINT32 line, UINT32 var);
extern void trace_ts(UINT32 level_module_id, UINT32 fid, UINT32 line, UINT32 var);
extern int trace_dump(TRACE_t *buf, int size_in_byte, UINT8 *staus);
extern void dbfw_proc_in_idle(void);
extern void dbfw_startPushTimer(void);
extern void dbfw_ForceDump(void);

extern void dbfw_gpio_dump_all0(UINT8 times);
extern void dbfw_gpio_dumpByte(UINT8 val);
extern void dbfw_gpio_dumpAnInteger(UINT32 val);
extern void dbfw_gpio_coredump_cpu_regs(UINT32 *regs, int nof_regs);

extern UINT16 dbfw_trace_queue_size;
extern DBFW_CONFIG_t dbfw_config;
extern TRACE_INFO_t dbfw_trace_info;
extern BOOL8 dbfw_hci_dump_config_n_status;
extern UINT32 dbfw_warning_flag;

#define DBFW_COREDUMP_SIGNATURE 0x38620980
extern UINT32 dbfw_coredump_signature;
extern BOOL8 dbfw_coredump_flag;

// Trace Variable Format
// UINT32 x 1
#define TVF_D(x)            (UINT32)(x)
// UINT16 x 2
#define TVF_WW(x,y)         (UINT32)((((UINT32)(x) << 16) & 0xFFFF0000) | ((UINT32)(y) & 0x0000FFFF))
// UINT8 x 2
#define TVF_BB(x,y)         (UINT32)((((UINT32)(x) <<  8) & 0x0000FF00) | ((UINT32)(y) & 0x000000FF))
// UINT8 x 2 + UINT16
#define TVF_BBW(x,y,z)      TVF_WW(TVF_BB(x,y), z)
// UINT8 x 4
#define TVF_BBBB(x,y,z,w)   TVF_WW(TVF_BB(x, y), TVF_BB(z, w))

#ifndef TRACE_COMPILE_LEVEL
#define TRACE_COMPILE_LEVEL 3       // Default Trace Compile Level
#endif

#if (TRACE_COMPILE_LEVEL == 0)
#define TRACE_TRACE_DEBUG(module, msg, var) trace(TVF_WW(TRACE_DEBUG, module), FID, __LINE__,var)
#define TRACE_TS_TRACE_DEBUG(module, msg, var) trace_ts(TVF_WW(TRACE_DEBUG, module), FID, __LINE__,var)
#endif

#if (TRACE_COMPILE_LEVEL <= 1)
#define TRACE_TRACE_INFO(module, msg, var) trace(TVF_WW(TRACE_INFO, module), FID, __LINE__,var)
#define TRACE_TS_TRACE_INFO(module, msg, var) trace_ts(TVF_WW(TRACE_INFO, module), FID, __LINE__,var)
#endif

#if (TRACE_COMPILE_LEVEL <= 2)
#define TRACE_TRACE_WARNING(module, msg, var) trace(TVF_WW(TRACE_WARNING, module), FID, __LINE__,var)
#define TRACE_TS_TRACE_WARNING(module, msg, var) trace_ts(TVF_WW(TRACE_WARNING, module), FID, __LINE__,var)
#endif

#if (TRACE_COMPILE_LEVEL <= 3)
#define TRACE_TRACE_CRITICAL(module, msg, var) trace(TVF_WW(TRACE_CRITICAL, module), FID, __LINE__,var)
#define TRACE_TS_TRACE_CRITICAL(module, msg, var) trace_ts(TVF_WW(TRACE_CRITICAL, module), FID, __LINE__,var)
#endif

#if !defined(TRACE_TRACE_WARNING)
#define TRACE_TRACE_WARNING(module, msg, var)
#define TRACE_TS_TRACE_WARNING(module, msg, var)
#endif

#if !defined(TRACE_TRACE_INFO)
#define TRACE_TRACE_INFO(module, msg, var)
#define TRACE_TS_TRACE_INFO(module, msg, var)
#endif

#if !defined(TRACE_TRACE_DEBUG)
#define TRACE_TRACE_DEBUG(module, msg, var)
#define TRACE_TS_TRACE_DEBUG(module, msg, var)
#endif

#define TRACE(level, module, msg, var) TRACE_##level(module, msg, var)
#define TRACE_TS(level, module, msg, var) TRACE_TS_##level(module, msg, var)

#define TRACE_FORCE_DUMP()     dbfw_ForceDump()

// For DBFW implementation use only
#define TRACE_MISSED1(msg) (__LINE__)
#define TRACE_MISSED2(level, module, msg, var) TRACE(level, module, msg, var)

#define DBFW_SET_WARNING_FLAG(flag)    dbfw_warning_flag |= flag

#ifdef DEBUG_FRAMEWORK_DEBUG
extern UINT32 dbuint32[32];
extern BOOL8 hci_dump_dbuint32;
extern UINT8 *u8p_dbuint32;
extern UINT16 *u16p_dbuint32;
extern void dbfw_caller(UINT32 sp, int depth);

#endif // DEBUG_FRAMEWORK_DEBUG


#define DBFW_GPIO_PIN_0 5
#define DBFW_GPIO_PIN_1 6

#define DBFW_GPIO_PIN_0_OUTPUT_ADDR (((UINT32*)gpiodata_adr) + (1 << DBFW_GPIO_PIN_0))
#define DBFW_GPIO_PIN_1_OUTPUT_ADDR (((UINT32*)gpiodata_adr) + (1 << DBFW_GPIO_PIN_1))

#define DBFW_GPIO_PIN_0_SET(x) REG32(DBFW_GPIO_PIN_0_OUTPUT_ADDR) = (x << DBFW_GPIO_PIN_0)
#define DBFW_GPIO_PIN_1_SET(x) REG32(DBFW_GPIO_PIN_1_OUTPUT_ADDR) = (x << DBFW_GPIO_PIN_1)

#else // DEBUG_FRAMEWORK

#define TRACE(level, module, msg, var)
#define TRACE_TS(level, module, msg, var)
#define DBFW_SET_WARNING_FLAG(flag)
#define dbfw_memInit(x) x
#define dbfw_init()
#define dbfw_proc_in_idle()
#define dbfw_startPushTimer()
#define dbfw_gpio_coredump_cpu_regs(regs, nof_regs)
#define dbfw_ForceDump()
#endif  // end of DEBUG_FRAMEWORK

#endif  // end of __DBFW_H__

