/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-17     Bernard      the first version
 * 2006-04-28     Bernard      fix the scheduler algorthm
 * 2006-04-30     Bernard      add SCHEDULER_DEBUG
 * 2006-05-27     Bernard      fix the scheduler algorthm for same priority
 *                             thread schedule
 * 2006-06-04     Bernard      rewrite the scheduler algorithm
 * 2006-08-03     Bernard      add hook support
 * 2006-09-05     Bernard      add 32 priority level support
 * 2006-09-24     Bernard      add rt_system_scheduler_start function
 * 2009-09-16     Bernard      fix _rt_scheduler_stack_check
 * 2010-04-11     yi.qiu       add module feature
 * 2010-07-13     Bernard      fix the maximal number of rt_scheduler_lock_nest
 *                             issue found by kuronca
 * 2010-12-13     Bernard      add defunct list initialization even if not use heap.
 * 2011-05-10     Bernard      clean scheduler debug log.
 * 2013-12-21     Grissiom     add rt_critical_level
 */

#include <rtthread.h>
#include <rthw.h>
#include "task.h"

rt_list_t rt_thread_defunct;

#ifdef RT_USING_HOOK
static void (*rt_scheduler_hook)(struct rt_thread *from, struct rt_thread *to);

/**
 * @addtogroup Hook
 */

/**@{*/

/**
 * This function will set a hook function, which will be invoked when thread
 * switch happens.
 *
 * @param hook the hook function
 */
void
rt_scheduler_sethook(void (*hook)(struct rt_thread *from, struct rt_thread *to))
{
    rt_scheduler_hook = hook;
}

/**@}*/
#endif


/**
 * @ingroup SystemInit
 * This function will initialize the system scheduler
 */
void rt_system_scheduler_init(void)
{
    /* initialize thread defunct */
    rt_list_init(&rt_thread_defunct);
}

/**
 * @ingroup SystemInit
 * This function will startup scheduler. It will select one thread
 * with the highest priority level, then switch to it.
 */
void rt_system_scheduler_start(void)
{
    vTaskStartScheduler();
}

/**
 * @addtogroup Thread
 */

/**@{*/

/**
 * This function will perform one schedule. It will select one thread
 * with the highest priority level, then switch to it.
 */
void rt_schedule(void)
{
    int level;

    level = rt_hw_interrupt_disable();
    portYIELD_WITHIN_API();
    rt_hw_interrupt_enable(level);
}

/*
 * This function will insert a thread to system ready queue. The state of
 * thread will be set as READY and remove from suspend queue.
 *
 * @param thread the thread to be inserted
 * @note Please do not invoke this function in user application.
 */
void rt_schedule_insert_thread(struct rt_thread *thread)
{
}

/*
 * This function will remove a thread from system ready queue.
 *
 * @param thread the thread to be removed
 *
 * @note Please do not invoke this function in user application.
 */
void rt_schedule_remove_thread(struct rt_thread *thread)
{
}

/**
 * This function will lock the thread scheduler.
 */
void rt_enter_critical(void)
{
    taskENTER_CRITICAL();
}
RTM_EXPORT(rt_enter_critical);

/**
 * This function will unlock the thread scheduler.
 */
void rt_exit_critical(void)
{
    taskEXIT_CRITICAL();
}
RTM_EXPORT(rt_exit_critical);

/**
 * Get the scheduler lock level
 *
 * @return the level of the scheduler lock. 0 means unlocked.
 */
rt_uint16_t rt_critical_level(void)
{
    return 0;
}
RTM_EXPORT(rt_critical_level);
/**@}*/

