/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    timer.h
  * @author  Jason Zhu
  * @version V0.1
  * @date    11-Jan-2022
  * @brief
  *
  ******************************************************************************
  */

#ifndef __C_TIMER__H
#define __C_TIMER__H

/* CORE TIMER */
#define CTIMER_BASE        0xff1e0000
#define CTIMER_MTIME_CTRL  0x0000
#define CTIMER_MTIME_DIV   0x0004
#define CTIMER_MTIME       0x0008
#define CTIMER_MTIMEH      0x000C
#define CTIMER_MTIMECMP    0x0010
#define CTIMER_MTIMECMPH   0x0014

#define MCAUSE_TMR_IRQ              (1 << 31 | 7)
#define SCR1_MTIME_CTRL_EN          0
#define SCR1_MTIME_CTRL_CLKSRC      1
#define SCR1_MTIME_CTRL_WR_MASK     0x3
#define SCR1_MTIME_DIV_WR_MASK      0x3FF

/* Set the system tick to 1ms */
#define SCR1_CORE_FREQUECY 297000000
#define SCR1_SYS_TIMER_DIV 1000

struct timer_cmp_t
{
    unsigned int time_cmp;
    unsigned int time_cmph;
    unsigned int step;
};

void sysTick_stop(void);
void _update_timer(void);
void sysTick_config(unsigned int ticks);

#endif
