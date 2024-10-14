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

#ifdef RT_USING_CPU_USAGE

static uint64_t g_last_idle_tick = 0;                     /* Last total tick for OS idle state. */
static uint64_t g_last_elapsed_tick = 0;                  /* Last total tick since OS boot. */

static uint64_t g_last_enter_irq_time = 0;                /* Last time enter level 0 interrupt. */
static uint64_t g_total_irq_consume_time = 0;             /* Toal time concume in IRQ. */

static rt_thread_t g_last_running_task;                   /* The task is preempted by the IRQ. */

static int object_name_maxlen(const char *type_name, struct rt_list_node *list)
{
    struct rt_list_node *node;
    struct rt_object *object = NULL;
    int max_length = rt_strlen(type_name), length;

    rt_enter_critical();
    for (node = list->next; node != list; node = node->next)
    {
        object = rt_list_entry(node, struct rt_object, list);

        length = rt_strlen(object->name);
        if (length > max_length) max_length = length;
    }
    rt_exit_critical();

    if (max_length > RT_NAME_MAX || max_length == 0) max_length = RT_NAME_MAX;

    return max_length;
}

rt_inline void object_split(int len)
{
    while (len--) rt_kprintf("-");
}

static uint32_t timer_count_to_tick(uint64_t count)
{
    return (count * RT_TICK_PER_SECOND) / PLL_INPUT_OSC_RATE;
}

void rt_scheduler_hook(struct rt_thread *from, struct rt_thread *to)
{
    rt_tick_t current_tick, running_tick;

    /* Count from thread total_running_tick. */
    current_tick = rt_tick_get();
    /* Avoid less than 0, when the task only runs for a short time and is preempted by IRQ. it's need? */
    running_tick = current_tick >= from->last_schedule_tick ? current_tick - from->last_schedule_tick : 0;
    from->total_running_tick += running_tick;

    /* update to thread last_schedule_tick. */
    to->last_schedule_tick = current_tick;
}

/* TODO: Support more detailed interrupt statistics. */
void rt_irq_enter_hook(void)
{
    /* We only begin count in level 0 interrupt. */
    if (rt_interrupt_get_nest() == 1)
    {
        g_last_running_task = rt_thread_self();
        g_last_enter_irq_time = HAL_GetSysTimerCount();
    }
}

void rt_irq_leave_hook(void)
{
    uint64_t count;

    /* We only end count in level 0 interrupt. */
    if (rt_interrupt_get_nest() == 0)
    {
        count = HAL_GetSysTimerCount() - g_last_enter_irq_time;
        g_last_running_task->last_schedule_tick += timer_count_to_tick(count);
        g_total_irq_consume_time += count;
    }
}

/**
 * This function will print cpu usage, like TOP command in posix.
 *
 */
void rt_cpu_usage_top(int argc, char **argv)
{
    int maxlen;
    uint32_t interval_ms = 1000;
    uint32_t i = 0, loop = 1;
    struct rt_thread *thread;
    struct rt_list_node *node;
    struct rt_object_information *info;
    struct rt_list_node *list;
    const char *item_title = "thread";
    rt_tick_t start, end, running_tick, total_tick, total_usr_tick, irq_tick;
    uint64_t irq_time_count, snap_irq_consume_time;
    char szBuf[32];

    if (argc != 1 && argc != 3)
    {
        rt_kprintf("usage: top [interval in ms, default is 1000ms] [loop, default is 1]\n");
        return;
    }

    if (argc == 3)
    {
        interval_ms = atoi(argv[1]);
        loop = atoi(argv[2]);
    }

    info = rt_object_get_information(RT_Object_Class_Thread);
    list = &info->object_list;

    maxlen = object_name_maxlen(item_title, list);

    while (i++ < loop)
    {
        start = rt_tick_get();
        for (node = list->next; node != list; node = node->next)
        {
            thread = rt_list_entry(node, struct rt_thread, list);

            thread->snap_running_tick = thread->total_running_tick;
        }
        snap_irq_consume_time = g_total_irq_consume_time;
        rt_thread_delay((interval_ms * RT_TICK_PER_SECOND) / 1000);
        end = rt_tick_get();
        total_tick = end - start;

        rt_kprintf("%-*.s pri  status  cpu used\n", maxlen, item_title);
        object_split(maxlen);
        rt_kprintf(" ---  ------- ----------\n");
        total_usr_tick = 0;
        for (node = list->next; node != list; node = node->next)
        {
            rt_uint8_t stat;
            thread = rt_list_entry(node, struct rt_thread, list);
            rt_kprintf("%-*.*s %3d ", maxlen, RT_NAME_MAX, thread->name, thread->current_priority);

            stat = (thread->stat & RT_THREAD_STAT_MASK);
            if (stat == RT_THREAD_READY)        rt_kprintf(" ready  ");
            else if (stat == RT_THREAD_SUSPEND) rt_kprintf(" suspend");
            else if (stat == RT_THREAD_INIT)    rt_kprintf(" init   ");
            else if (stat == RT_THREAD_CLOSE)   rt_kprintf(" close  ");

            running_tick = thread->total_running_tick - thread->snap_running_tick;
            total_usr_tick += running_tick;
            snprintf(szBuf, sizeof(szBuf), " %.2f", (running_tick * 100.0) / total_tick);
            rt_kprintf("%s\n", szBuf);
        }
        if (total_usr_tick > total_tick)
        {
            rt_kprintf("total_usr_tick: %d, total_tick: %d, ratio: %d\n", total_usr_tick, total_tick, (total_usr_tick * 100) / total_tick);
            total_usr_tick = total_tick;
        }
        irq_time_count = g_total_irq_consume_time - snap_irq_consume_time;
        irq_tick = timer_count_to_tick(irq_time_count);
        snprintf(szBuf, sizeof(szBuf), " %.2f(IRQ: %.2f)", ((total_tick - total_usr_tick) * 100.0) / total_tick, (irq_tick * 100.0) / total_tick);
        rt_kprintf("\nOther(Include IRQ and OS scheduler):%s\n", szBuf);
    }
}

/**
 * Get current cpu usage[0-100]
 *
 * @return the cpu usage[0-100]
 */
uint32_t rt_cpu_usage_get(void)
{
    rt_tick_t total_tick, idle_tick;
    uint32_t usage;

    if (!g_last_elapsed_tick || !g_last_idle_tick)
    {
        /* For the first time, we return to the cpu usage so far. */
        idle_tick = rt_thread_idle_gethandler()->total_running_tick;
        total_tick = rt_tick_get();
    }
    else
    {
        /* At other times, we return the cpu usage since the last statistics. */
        idle_tick = rt_thread_idle_gethandler()->total_running_tick - g_last_idle_tick;
        total_tick = rt_tick_get() - g_last_elapsed_tick;
    }

    g_last_idle_tick = idle_tick;
    g_last_elapsed_tick = total_tick;
    usage = ((total_tick - idle_tick) * 100) / total_tick;
    return usage;
}

/**
 * The cpu usage module init.
 *
 */
void rt_cpu_usage_init(void)
{
    rt_base_t level;

    level = rt_hw_interrupt_disable();
    rt_scheduler_sethook(rt_scheduler_hook);
    rt_interrupt_enter_sethook(rt_irq_enter_hook);
    rt_interrupt_leave_sethook(rt_irq_leave_hook);
    rt_hw_interrupt_enable(level);
}

INIT_BOARD_EXPORT(rt_cpu_usage_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT_ALIAS(rt_cpu_usage_top, __cmd_top, Execute command with top.);
#endif

#endif /* End of RT_USING_CPU_USAGE */