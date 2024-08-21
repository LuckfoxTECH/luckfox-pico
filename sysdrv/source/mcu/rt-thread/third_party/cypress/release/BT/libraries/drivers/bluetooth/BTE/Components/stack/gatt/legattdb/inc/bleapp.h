#ifndef _BLEAPP_H_
#define _BLEAPP_H_
/*******************************************************************************
* THIS INFORMATION IS PROPRIETARY TO BROADCOM CORP
*
* ------------------------------------------------------------------------------
*
* Copyright (c) 2011 Broadcom Corp.
*
*          ALL RIGHTS RESERVED
*
********************************************************************************
*
* File Name: bleapp.h
*
* Abstract: This is the header file that has all the interfaces for BLEAPP.
*
* Functions:
*
*******************************************************************************/


#include "bleappcfa.h"
#ifndef BCM20732
#include "hiddcfa.h"
#endif

//#include "bleprofile.h" //this is needed to access bleprofile_puart_cfg

#ifdef __cplusplus
extern "C" {
#endif


//////////////////////////////////////////////////////////////////////////////
//                      public data type definition.
//////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <pshpack1.h>
#endif
// GHS syntax.
#pragma pack(1)

// these are the ID for the Applicaation timer.
#define  BLEAPP_APP_TIMER_ADV                         0x1
#define  BLEAPP_APP_TIMER_CONN_IDLE              0x2
#define  BLEAPP_APP_TIMER_DIRECT_ADV            0x3
#define  BLEAPP_APP_TIMER_SCAN                        0x4
#define  BLEAPP_APP_TIMER_CONN                        0x5

#define  BLEAPP_APP_TIMER_FILTER_OFF             0x11

#define  BLEAPP_APP_TIMER_GENERIC                  0xF1

enum ble_hidoff_mode
{
    HIDOFF_DISABLE = 0x00,
    BAT_HIDOFF_ENABLE = 0x01,
    IDLE_HIDOFF_ENABLE = 0x02,
    BUTTON_HIDOFF_ENABLE = 0x04,
    SLEEP_UART_DISABLE = 0x08, //this is not related to hidoff mode, but used here to save variable
    SLEEP_BAT_DISABLE = 0x10, //this is not related to hidoff mode, but used here to save variable
};

enum ble_cpu_clock_enable
{
    CPU_CLOCK_DISABLE = 0x00,
    DEFAULT_CPU_CLOCK_ENABLE = 0x01,
    POWERSAVE_CPU_CLOCK_ENABLE = 0x02,
};

enum ble_select_lpo_enable
{
    SELECT_LPO_DISABLE = 0x00,
    DEFAULT_SELECT_LPO_ENABLE = 0x01,
    POWERSAVE_SELECT_LPO_ENABLE = 0x02,
    INTERVALHIGH_SELECT_LPO_ENABLE = 0x04
};

#ifdef BCM20732
typedef enum
{
    CPU_CLK_SPEED_24MHZ,
    CPU_CLK_SPEED_12MHZ,
    CPU_CLK_SPEED_8MHZ,
    CPU_CLK_SPEED_6MHZ,
    CPU_CLK_SPEED_4MHZ,
    CPU_CLK_SPEED_3MHZ,
    CPU_CLK_SPEED_2MHZ,
    CPU_CLK_SPEED_1MHZ
} CLOCK_SPEED;

enum
{
    LPO_CLK_INTERNAL,
    LPO_CLK_EXTERNAL,
    LPO_CLK_CRYSTAL,
    LPO_NO_SELECTED,
    LPO_32KHZ_OSC,
    LPO_MIA_LPO
};
#endif


//Application set up related structure
typedef PACKED struct
{
    UINT8 *p_db;
    UINT16 db_size;
    void *cfg;
    void *puart_cfg;
    void *gpio_cfg;
    void *create_func;
} BLEAPP_INIT_CFG;

typedef PACKED struct
{
    UINT8 *p_db;
    UINT16 db_size;
} BLEAPP_DB_CFG;

typedef PACKED struct
{
    UINT8   selLPOSrc;
    // selSlaveLPOSrc
    // If TRUE: Will continue to use the selected LPO SRC in Sniff.
    // If FALSE: Will use the LPX in Sniff.
    BOOLEAN selSlaveLPOSrc;
    UINT16  driftRate;
} BLEAPP_SELECT_LPO;

typedef PACKED struct
{
    UINT8   selectlpoenable;
    UINT16               IntervalLimitLPO;
    BLEAPP_SELECT_LPO    selLPO_Default;
    BLEAPP_SELECT_LPO    selLPO_Powersave;
    BLEAPP_SELECT_LPO    selLPO_IntervalHigh;
} BLEAPP_SELECT_LPO_CFG;

typedef PACKED struct
{
    UINT8 cpuclockenable;
    CLOCK_SPEED default_cpu_clock;
    CLOCK_SPEED powersave_cpu_clock;
} BLEAPP_CPU_CLOCK_CFG;

#ifdef _WIN32
#include <poppack.h>
#endif
// GHS syntax.
#pragma pack()

//////////////////////////////////////////////////////////////////////////////
//                      public interface declaration
//////////////////////////////////////////////////////////////////////////////

void bleapp_init(void);

// this will start Basic stuffs.
//void bleapp_start(void);


void bleapp_gpio_debug_init(void);
void bleapp_gpio_debug(UINT16 setting, UINT16 pinmap);
void bleapp_gpio_init(void);
void bleapp_puart_init(void);


void bleapp_appTimerCb(UINT16 arg);

#ifdef BLEAPP_UART_DETECT
UINT8 bleapp_uart_connected(void);
#endif

//tracefile
#ifdef BLE_TRACE_DISABLE
#define ble_trace0(p_str)
#define ble_trace1(fmt_str, p1)
#define ble_trace2(fmt_str, p1, p2)
#define ble_trace3(fmt_str, p1, p2, p3)
#define ble_trace4(fmt_str, p1, p2, p3, p4)
#define ble_trace5(fmt_str, p1, p2, p3, p4, p5)
#define ble_trace6(fmt_str, p1, p2, p3, p4, p5, p6)
#define ble_tracen(p_str, len)
// INLINE void ble_traceprintf(const char* fmt_str, ...){}
#else
extern void ble_trace0(const char *p_str);
extern void ble_trace1(const char *fmt_str, UINT32 p1);
extern void ble_trace2(const char *fmt_str, UINT32 p1, UINT32 p2);
extern void ble_trace3(const char *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3);
extern void ble_trace4(const char *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4);
extern void ble_trace5(const char *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5);
extern void ble_trace6(const char *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);
extern void ble_tracen(char *p_str, UINT16 len);
// extern void ble_traceprintf(const char* fmt_str, ...);
#endif
void ble_traceEnable(UINT32 channel);
void ble_traceDisable(void);

extern UINT8 bleapp_trace_enable;

#ifdef __cplusplus
}
#endif



#endif // end of #ifndef _BLEAPP_H_
