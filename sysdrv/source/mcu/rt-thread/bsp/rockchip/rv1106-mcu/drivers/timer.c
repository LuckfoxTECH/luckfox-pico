/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    timer.c
  * @author  Jason Zhu
  * @version V0.1
  * @date    11-Jan-2022
  * @brief
  *
  ******************************************************************************
  */

#include <rtthread.h>
#include "timer.h"
#include "riscv_csr_encoding.h"

static struct timer_cmp_t timer_cmp;

/* REG ADDR DEF end */
#define word32  *(volatile unsigned int *)
#define word16  *(volatile unsigned short *)
#define word8   *(volatile unsigned char *)

static void _reset_mtime()
{
    word32(CTIMER_BASE + CTIMER_MTIME) = 0x00;
    word32(CTIMER_BASE + CTIMER_MTIMEH) = 0x00;
}

static void _reset_mtimecmp()
{
    word32(CTIMER_BASE + CTIMER_MTIMECMP) = 0x00;
    word32(CTIMER_BASE + CTIMER_MTIMECMPH) = 0x00;
}

static void _write_mtime_ctrl(unsigned int t)
{
    word32(CTIMER_BASE + CTIMER_MTIME_CTRL) = t;
}

static void _write_mtime_div(unsigned int t)
{
    word32(CTIMER_BASE + CTIMER_MTIME_DIV) = t;
}

static void _write_mtimecmp(unsigned int t)
{
    word32(CTIMER_BASE + CTIMER_MTIMECMP) = t;
}

static void _write_mtimecmph(unsigned int t)
{
    word32(CTIMER_BASE + CTIMER_MTIMECMPH) = t;
}

static void _write_mtime(unsigned int t)
{
    word32(CTIMER_BASE + CTIMER_MTIME) = t;
}

static void _write_mtimeh(unsigned int t)
{
    word32(CTIMER_BASE + CTIMER_MTIMEH) = t;
}

static unsigned int _read_mtimecmp()
{
    return word32(CTIMER_BASE + CTIMER_MTIMECMP);
}

static unsigned int _read_mtimecmph()
{
    return word32(CTIMER_BASE + CTIMER_MTIMECMPH);
}

static unsigned int _read_mtime()
{
    return word32(CTIMER_BASE + CTIMER_MTIME);
}

static unsigned int _read_mtimeh()
{
    return word32(CTIMER_BASE + CTIMER_MTIMEH);
}

static unsigned int _read_mtime_ctrl()
{
    return word32(CTIMER_BASE + CTIMER_MTIME_CTRL);
}

static unsigned int _read_mtime_div()
{
    return word32(CTIMER_BASE + CTIMER_MTIME_DIV);
}

static void _run_timer()
{
    unsigned int temp;
    temp = word32(CTIMER_BASE + CTIMER_MTIME_CTRL);
    temp |= (1 << SCR1_MTIME_CTRL_EN);
    word32(CTIMER_BASE + CTIMER_MTIME_CTRL) = temp;
}

static void _stop_timer()
{
    unsigned int temp;
    temp = word32(CTIMER_BASE + CTIMER_MTIME_CTRL);
    temp &= (~(1 << SCR1_MTIME_CTRL_EN));
    word32(CTIMER_BASE + CTIMER_MTIME_CTRL) = temp;
}

void sysTick_stop(void)
{
    _stop_timer();
}

void _update_timer(void)
{
    clear_csr(mie, MIP_MTIP);
    _write_mtimecmp(-1);
    _write_mtimecmph(0);
    timer_cmp.time_cmp += timer_cmp.step;
    _write_mtimecmp(timer_cmp.time_cmp);
    if (_read_mtimeh() > timer_cmp.time_cmph)
    {
        timer_cmp.time_cmph + 1;
        _write_mtimecmph(timer_cmp.time_cmph);
    }

    set_csr(mie, MIP_MTIP);
}

void sysTick_config(unsigned int ticks)
{
    _stop_timer();
    _reset_mtime();
    _reset_mtimecmp();
    _write_mtime_div(SCR1_SYS_TIMER_DIV);
    _write_mtimecmp(ticks / SCR1_SYS_TIMER_DIV);
    timer_cmp.time_cmp = ticks / SCR1_SYS_TIMER_DIV;
    timer_cmp.time_cmph = 0;
    timer_cmp.step = ticks / SCR1_SYS_TIMER_DIV;
    set_csr(mie, MIP_MTIP);
    _run_timer();
}

