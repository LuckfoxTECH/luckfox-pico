/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    main.c
  * @author  Jason Zhu
  * @version V0.1
  * @date    11-Jan-2022
  * @brief
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <string.h>
#include <rtthread.h>
#include "board_base.h"

void rtthread_startup(void)
{
#ifdef RT_USING_RK_AOV
    rt_kprintf("RT_USING_RK_AOV=y\n");
    uint32_t resume_reason = readl(0xff3001c0);
    rt_kprintf("resume_reason is 0x%x\n", resume_reason);
    if (resume_reason == 0) {
        return;
    }
#endif
    /* disable interrupt first */
    rt_hw_interrupt_disable();

    /* init board */
    rt_hw_board_init();

    /* init tick */
    rt_system_tick_init();

    /* init kernel object */
    rt_system_object_init();

    /* init timer system */
    rt_system_timer_init();

    /* init scheduler system */
    rt_system_scheduler_init();

    /* init application */
    rt_application_init();

    /* init timer thread */
    rt_system_timer_thread_init();

    /* init idle thread */
    rt_thread_idle_init();

    /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return;
}

int main(void)
{
    /* startup RT-Thread RTOS */
    rtthread_startup();

    return 0;
}
