/*******************************************************************************
* THIS INFORMATION IS PROPRIETARY TO BROADCOM CORP
*
* ------------------------------------------------------------------------------
*
* Copyright (c) 2003 Broadcom Corp.
*
*          ALL RIGHTS RESERVED
*
********************************************************************************
*
* File Name: timer.h
*
* Abstract: This file contains the defines and routines to control the
*       arm timers.
*
* Functions:
*
*******************************************************************************/
#ifndef P_256_TIMER_H
#define P_256_TIMER_H

#include "p_256_types.h"
#ifndef BOOL
#define BOOL BOOLEAN
#endif

// Countdown_Interval_secs = (cpufreq_freq / prescale)_secs * count
// 0.012500         = (1 / (24000000 / 16)) * count
//                  = (1 / 1500000) * count
// count = 1500000/1 * 12500/1000000
//       = 15/1 * 12500/10
//       = 15/1 * 1250/1
//       = 18750

#define TIMER_ENABLE_BIT            0x80
#define TIMER_MODE_PERIODIC_BIT     0x40
#define TIMER_INT_ENABLE_BIT        0x20
#define TIMER_PRE_SCALE_BITS_256    0x08
#define TIMER_PRE_SCALE_BITS_16     0x04
#define TIMER_PRE_SCALE_BITS_1      0x00
#define TIMER_COUNTERSIZE_MODE      0x02        // 1 = 32 bits, 0 = 16 bits
#define TIMER_1_SHOT_BIT            0x01

///////////////////////////////////////////////////////////////////////////////
// Timer 1 //
/////////////
#define TIMER_1_INTERVAL_IN_MICROS  12500UL     // 12.500 ms

#define TIMER_1_TICKS_SEC           (1000000 / TIMER_1_INTERVAL_IN_MICROS)
#define TIMER_1_28_TICKS_SEC           (1280000 / TIMER_1_INTERVAL_IN_MICROS)
// Convert micro seconds to ticks
#define US_TO_TICKS(us)     (((UINT32)us) / TIMER_1_INTERVAL_IN_MICROS)
// Convert 1.28 seconds to ticks
#define TIM_1_28S_TO_TICKS(sec)    (((UINT32)sec) * TIMER_1_28_TICKS_SEC)

#define TIMER_1_PRE_SCALE_BITS  TIMER_PRE_SCALE_BITS_1


extern const UINT8 clkTbls[];
extern UINT8 cpuClockDivisor;

///////////////////////////////////////////////////////////////////////////////
// Timer 2 //
/////////////
// Timer2 is a dedicated CPU clock ticks counter which wraps from zero back to
// 0xFFFFFFFF

///////////////////////////////////////////////////////////////////////////////
// Routine Declarations //
//////////////////////////
void timer_startTimer1(void);

#if !defined(HW_TIMER_CLK_1MHZ)
void timer_pauseProgramming(BOOL restore);
UINT32 timer_QFormatCalc(UINT8 pauseCpuClk, UINT8 cpuClk);
#endif

void timer_clearTimer1Int(void);

void timer_delayUs(UINT32 microseconds);

void timer_clearTimer2Int(void);

void timer_startTimer2(void);
// Timer2 is a dedicated CPU clock ticks counter which wraps from zero back to
// 0xFFFFFFFF
#define timer_getTimer2time(time)       *(time) = REG32(timer2value_adr);

/*
******************************************************************************************
*
* $HISTORY:$
*
******************************************************************************************
*/
#endif /* P_256_TIMER_H */
