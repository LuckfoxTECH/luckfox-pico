/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    cpuusage_test.c
  * @version V0.1
  * @brief   cpuusage test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-11-01     Tony.Xie        first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <stdbool.h>
#include "hal_base.h"
#include "pm_test.h"

#ifdef RT_USING_COMMON_TEST_CPU_USAGE

#define MS_TO_SYS_TICK(ms) ((RT_TICK_PER_SECOND *(ms))/ 1000)

struct cpu_usage_ctrl
{
    uint32_t test_begin;
    uint64_t last_sys_time;
    uint64_t last_idle_total_time;
    uint64_t idle_total_time;
    rt_timer_t timer;
};

static struct cpu_usage_ctrl cpu_usage_data;

void rkpm_cpu_idle_time_update(uint64_t idle_time)
{
    cpu_usage_data.idle_total_time += idle_time;
}

static void cpu_usage_timer_handler(void *parameter)
{
    uint64_t delta, delta_idle;
    uint64_t cur_count, cur_idle_count;
    uint32_t cpu_usage;

    if (!cpu_usage_data.test_begin)
    {
        rt_kprintf("%s: stop!\n", __func__);
        rt_timer_stop(cpu_usage_data.timer);
    }

    cur_count = HAL_GetSysTimerCount();
    cur_idle_count = cpu_usage_data.idle_total_time;

    delta = cur_count - cpu_usage_data.last_sys_time;
    delta_idle =  cur_idle_count - cpu_usage_data.last_idle_total_time;
    cpu_usage = (uint32_t)HAL_DivU64(delta_idle * 100, delta);

    rt_kprintf("idle time-%d%, 0x%x %x, 0x%x %x\n", cpu_usage,
               (uint32_t)(delta >> 32), (uint32_t)delta,
               (uint32_t)(delta_idle >> 32), (uint32_t)delta_idle);

    PM_TEST_INFO("    time 0x%x %x, 0x%x %x, 0x%x %x\n",
                 (uint32_t)(cpu_usage_data.last_sys_time >> 32),
                 (uint32_t)(cpu_usage_data.last_sys_time),
                 (uint32_t)(cur_count >> 32), (uint32_t)cur_count,
                 (uint32_t)(delta >> 32), (uint32_t)delta);

    PM_TEST_INFO("    ilde 0x%x %x, 0x%x %x, 0x%x %x\n",
                 (uint32_t)(cpu_usage_data.last_idle_total_time >> 32),
                 (uint32_t)cpu_usage_data.last_idle_total_time,
                 (uint32_t)(cur_idle_count >> 32), (uint32_t)cur_idle_count,
                 (uint32_t)(delta_idle >> 32), (uint32_t)delta_idle);

    cpu_usage_data.last_sys_time = cur_count;
    cpu_usage_data.last_idle_total_time = cur_idle_count;
}

static void cpu_usage_cmd(int argc, char **argv)
{
    rt_tick_t test_time = 0;
    if (argc <= 1 || (strcmp(argv[1], "--help") == 0))
    {
        rt_kprintf("Usage: \n");
        rt_kprintf("    cpu_usage -s <sample ms>   start cpu_usage case\n");
        rt_kprintf("    cpu_usage -p                stop cpu_usage case\n");
        return;
    }

    if (strcmp(argv[1], "-p") == 0)
    {
        cpu_usage_data.test_begin = 0;
        return;
    }

    if (strcmp(argv[1], "-s") != 0)
    {
        return;
    }

    if (cpu_usage_data.test_begin)
        return;

    cpu_usage_data.test_begin = 1;

    if (argc >= 2)
    {
        test_time = atoi(argv[2]);
    }

    if (!test_time)
        test_time = 500;

    rt_kprintf("sampling ms %d: \n", test_time);

    test_time = MS_TO_SYS_TICK(test_time);

    rt_kprintf("sampling tick %d: \n", test_time);

    if (!cpu_usage_data.timer)
        cpu_usage_data.timer = rt_timer_create("cpu_usage",
                                               cpu_usage_timer_handler,
                                               NULL,
                                               test_time,
                                               RT_TIMER_FLAG_PERIODIC |
                                               RT_TIMER_FLAG_SOFT_TIMER);
    rt_timer_start(cpu_usage_data.timer);
    cpu_usage_data.last_sys_time = HAL_GetSysTimerCount();
    cpu_usage_data.last_idle_total_time = cpu_usage_data.idle_total_time;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(cpu_usage_cmd, cpu_usage, cpu_usage test cmd);
#endif

#endif
