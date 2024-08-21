/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-22     Cliff Chen   first implementation
 */

#include "rtthread.h"
#include "rthw.h"
#include "hal_base.h"
#include "drv_usage.h"

#ifdef RT_USING_COMMON_TEST_USAGE

/* thread 1: Fixed 50% CPU usage. */
static void thread1_entry(void *parameter)
{
    while (1)
    {
        HAL_CPUDelayUs(500000);
        rt_thread_delay(RT_TICK_PER_SECOND / 2);
    }
}

/* thread 2: CPU usage is random from 0-10% */
static void thread2_entry(void *parameter)
{
    uint32_t active_us, sleep_tick;
    uint32_t max_us;

    max_us = 100000;         /* max cpu usage is 10%=100ms. */
    while (1)
    {
        active_us = random() % max_us;
        HAL_CPUDelayUs(active_us);
        sleep_tick = ((1000000 - active_us) * RT_TICK_PER_SECOND) / 1000000;
        rt_thread_delay(sleep_tick);
    }
}

/* IRQ: CPU usage is random from 0-5%, triggered once per second. */
static void timer_isr(int vector, void *param)
{
    uint32_t active_us, max_us;

    rt_interrupt_enter();
    max_us = 50000;         /* max cpu usage is 5%=50ms. */
    active_us = random() % max_us;
    HAL_CPUDelayUs(active_us);
    HAL_TIMER_ClrInt(TIMER0);
    rt_interrupt_leave();
}

void test_cpu_usage_top(void)
{
    rt_thread_t thread1, thread2;

    /* create test thread. */
    thread1 = rt_thread_create("t1_top", thread1_entry, NULL, 1024, 20, 10);
    thread2 = rt_thread_create("t2_top", thread2_entry, NULL, 1024, 10, 10);
    RT_ASSERT(thread1 != NULL && thread2 != NULL);

    /* config TIMER0 for test. */
    rt_hw_interrupt_install(TIMER0_IRQn, timer_isr, NULL, "timer0");
    rt_hw_interrupt_umask(TIMER0_IRQn);
    HAL_TIMER_Init(TIMER0, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(TIMER0, PLL_INPUT_OSC_RATE);
    HAL_TIMER_Start_IT(TIMER0);

    /* startup the test threads. */
    rt_thread_startup(thread1);
    rt_thread_startup(thread2);

    rt_kprintf("top test init finish, please type the top command in shell.\n");
}

void test_cpu_usage_get(void)
{
    rt_kprintf("current cpu usage: %d\n", rt_cpu_usage_get());
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT_ALIAS(test_cpu_usage_top, __cmd_top_test, Execute command with top test.);
FINSH_FUNCTION_EXPORT_ALIAS(test_cpu_usage_get, __cmd_cpu_usage, Execute command with cpu usage get.);
#endif /* End of RT_USING_FINSH*/

#endif  /* End of RT_USING_COMMON_TEST_USAGE */