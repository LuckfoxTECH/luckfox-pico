/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-28     Bernard      first version
 * 2006-04-29     Bernard      implement thread timer
 * 2006-04-30     Bernard      added THREAD_DEBUG
 * 2006-05-27     Bernard      fixed the rt_thread_yield bug
 * 2006-06-03     Bernard      fixed the thread timer init bug
 * 2006-08-10     Bernard      fixed the timer bug in thread_sleep
 * 2006-09-03     Bernard      changed rt_timer_delete to rt_timer_detach
 * 2006-09-03     Bernard      implement rt_thread_detach
 * 2008-02-16     Bernard      fixed the rt_thread_timeout bug
 * 2010-03-21     Bernard      change the errno of rt_thread_delay/sleep to
 *                             RT_EOK.
 * 2010-11-10     Bernard      add cleanup callback function in thread exit.
 * 2011-09-01     Bernard      fixed rt_thread_exit issue when the current
 *                             thread preempted, which reported by Jiaxing Lee.
 * 2011-09-08     Bernard      fixed the scheduling issue in rt_thread_startup.
 * 2012-12-29     Bernard      fixed compiling warning.
 * 2016-08-09     ArdaFu       add thread suspend and resume hook.
 * 2017-04-10     armink       fixed the rt_thread_delete and rt_thread_detach
                               bug when thread has not startup.
 */

#include <rtthread.h>
#include <rthw.h>
#include "task.h"

#ifdef RT_USING_HOOK

static void (*rt_thread_suspend_hook)(rt_thread_t thread);
static void (*rt_thread_resume_hook)(rt_thread_t thread);
static void (*rt_thread_inited_hook)(rt_thread_t thread);

/**
 * @ingroup Hook
 * This function sets a hook function when the system suspend a thread.
 *
 * @param hook the specified hook function
 *
 * @note the hook function must be simple and never be blocked or suspend.
 */
void rt_thread_suspend_sethook(void (*hook)(rt_thread_t thread))
{
    rt_thread_suspend_hook = hook;
}

/**
 * @ingroup Hook
 * This function sets a hook function when the system resume a thread.
 *
 * @param hook the specified hook function
 *
 * @note the hook function must be simple and never be blocked or suspend.
 */
void rt_thread_resume_sethook(void (*hook)(rt_thread_t thread))
{
    rt_thread_resume_hook = hook;
}

/**
 * @ingroup Hook
 * This function sets a hook function when a thread is initialized.
 *
 * @param hook the specified hook function
 */
void rt_thread_inited_sethook(void (*hook)(rt_thread_t thread))
{
    rt_thread_inited_hook = hook;
}

#endif

void rt_thread_exit(void)
{
    rt_thread_t thread;

    thread = (rt_thread_t)xTaskGetCurrentTaskHandle();

    /* change stat */
    thread->stat = RT_THREAD_CLOSE;

    /* remove it from timer list */
    rt_timer_detach(&thread->thread_timer);

    if (thread->cleanup != NULL)
    {
        thread->cleanup(thread);
    }

    vTaskDelete((TaskHandle_t)thread);
}

/**
 * @addtogroup Thread
 */

/**@{*/

/**
 * This function will initialize a thread, normally it's used to initialize a
 * static thread object.
 *
 * @param thread the static thread object
 * @param name the name of thread, which shall be unique
 * @param entry the entry function of thread
 * @param parameter the parameter of thread enter function
 * @param stack_start the start address of thread stack
 * @param stack_size the size of thread stack
 * @param priority the priority of thread
 * @param tick the time slice if there are same priority thread
 *
 * @return the operation status, RT_EOK on OK, -RT_ERROR on error
 */
rt_err_t rt_thread_init(struct rt_thread *thread,
                        const char       *name,
                        void (*entry)(void *parameter),
                        void             *parameter,
                        void             *stack_start,
                        rt_uint32_t       stack_size,
                        rt_uint8_t        priority,
                        rt_uint32_t       tick)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
    TaskHandle_t xReturn;
    rt_err_t ret = RT_ERROR;

    /* thread check */
    RT_ASSERT(thread != RT_NULL);
    RT_ASSERT(stack_start != RT_NULL);

    rt_enter_critical();

    thread->type |= RT_Object_Class_Static;

    /* priority init */
    thread->init_priority    = priority;
    thread->current_priority = priority;

    /* init thread list */
    rt_list_init(&(thread->tlist));

    rt_strncpy(thread->name, name, RT_NAME_MAX);

    /* error and flags */
    thread->error = RT_EOK;
    thread->stat  = RT_THREAD_INIT;

    /* initialize cleanup function and user data */
    thread->cleanup   = 0;

    /* init thread timer */
    rt_timer_init(&(thread->thread_timer),
                  thread->name,
                  rt_thread_timeout,
                  thread,
                  0,
                  RT_TIMER_FLAG_ONE_SHOT);

    xReturn = xTaskCreateStaticV2(entry, name, stack_size / sizeof(StackType_t), parameter, RT_THREAD_PRIORITY_MAX - priority, stack_start, thread, 0);
    if (xReturn)
    {
        ret = RT_EOK;
    }

    rt_exit_critical();

    return ret;
#endif

    return RT_ERROR;
}
RTM_EXPORT(rt_thread_init);

/**
 * This function will return self thread object
 *
 * @return the self thread object
 */
rt_thread_t rt_thread_self(void)
{
    return (rt_thread_t)xTaskGetCurrentTaskHandle();
}
RTM_EXPORT(rt_thread_self);

/**
 * This function will start a thread and put it to system ready queue
 *
 * @param thread the thread to be started
 *
 * @return the operation status, RT_EOK on OK, -RT_ERROR on error
 */
rt_err_t rt_thread_startup(rt_thread_t thread)
{
    BaseType_t ret;

    /* thread check */
    RT_ASSERT(thread != RT_NULL);

    if (thread->current_priority != thread->init_priority)
    {
        /* set current priority to init priority */
        thread->current_priority = thread->init_priority;

        vTaskPrioritySet((TaskHandle_t)thread, RT_THREAD_PRIORITY_MAX - thread->current_priority);
    }

    ret = xTaskStartup((TaskHandle_t)thread);
    if (ret == pdPASS)
        return RT_EOK;
    else
        return RT_ERROR;
}
RTM_EXPORT(rt_thread_startup);

/**
 * This function will detach a thread. The thread object will be removed from
 * thread queue and detached/deleted from system object management.
 *
 * @param thread the thread to be deleted
 *
 * @return the operation status, RT_EOK on OK, -RT_ERROR on error
 */
rt_err_t rt_thread_detach(rt_thread_t thread)
{
    /* thread check */
    RT_ASSERT(thread != RT_NULL);

    /* change stat */
    thread->stat = RT_THREAD_CLOSE;

    /* release thread timer */
    rt_timer_detach(&(thread->thread_timer));

    if (thread->cleanup != NULL)
    {
        thread->cleanup(thread);
    }

    vTaskDelete((TaskHandle_t)thread);

    return RT_EOK;
}
RTM_EXPORT(rt_thread_detach);

#ifdef RT_USING_HEAP
/**
 * This function will create a thread object and allocate thread object memory
 * and stack.
 *
 * @param name the name of thread, which shall be unique
 * @param entry the entry function of thread
 * @param parameter the parameter of thread enter function
 * @param stack_size the size of thread stack
 * @param priority the priority of thread
 * @param tick the time slice if there are same priority thread
 *
 * @return the created thread object
 */
rt_thread_t rt_thread_create(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick)
{
    rt_thread_t thread = RT_NULL;

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    BaseType_t ret;

    rt_enter_critical();

    ret = xTaskCreateV2(entry,
                        name,
                        stack_size / sizeof(StackType_t),
                        parameter,
                        RT_THREAD_PRIORITY_MAX - priority,
                        (TaskHandle_t *)&thread, 0);

    if (ret != pdPASS)
    {
        thread = RT_NULL;
    }
    else
    {
        /* priority init */
        thread->init_priority    = priority;
        thread->current_priority = priority;

        /* init thread list */
        rt_list_init(&(thread->tlist));

        rt_strncpy(thread->name, name, RT_NAME_MAX);

        /* error and flags */
        thread->error = RT_EOK;
        thread->stat  = RT_THREAD_INIT;

        /* initialize cleanup function and user data */
        thread->cleanup   = 0;

        /* init thread timer */
        rt_timer_init(&(thread->thread_timer),
                      thread->name,
                      rt_thread_timeout,
                      thread,
                      0,
                      RT_TIMER_FLAG_ONE_SHOT);
    }

    rt_exit_critical();
#endif

    return thread;
}
RTM_EXPORT(rt_thread_create);

/**
 * This function will delete a thread. The thread object will be removed from
 * thread queue and deleted from system object management in the idle thread.
 *
 * @param thread the thread to be deleted
 *
 * @return the operation status, RT_EOK on OK, -RT_ERROR on error
 */
rt_err_t rt_thread_delete(rt_thread_t thread)
{
    if (thread == NULL)
    {
        thread = rt_thread_self();
    }

    /* change stat */
    thread->stat = RT_THREAD_CLOSE;

    /* remove from suspend list */
    rt_list_remove(&(thread->tlist));

    /* release thread timer */
    rt_timer_detach(&(thread->thread_timer));

    if (thread->cleanup != NULL)
    {
        thread->cleanup(thread);
    }

    vTaskDelete((TaskHandle_t)thread);

    return RT_EOK;
}
RTM_EXPORT(rt_thread_delete);
#endif

/**
 * This function will let current thread yield processor, and scheduler will
 * choose a highest thread to run. After yield processor, the current thread
 * is still in READY state.
 *
 * @return RT_EOK
 */
rt_err_t rt_thread_yield(void)
{
    taskYIELD();

    return RT_EOK;
}
RTM_EXPORT(rt_thread_yield);

/**
 * This function will let current thread sleep for some ticks.
 *
 * @param tick the sleep ticks
 *
 * @return RT_EOK
 */
rt_err_t rt_thread_sleep(rt_tick_t tick)
{
    vTaskDelay(tick);

    return RT_EOK;
}

/**
 * This function will let current thread delay for some ticks.
 *
 * @param tick the delay ticks
 *
 * @return RT_EOK
 */
rt_err_t rt_thread_delay(rt_tick_t tick)
{
    return rt_thread_sleep(tick);
}
RTM_EXPORT(rt_thread_delay);

/**
 * This function will let current thread delay for some milliseconds.
 *
 * @param tick the delay time
 *
 * @return RT_EOK
 */
rt_err_t rt_thread_mdelay(rt_int32_t ms)
{
    rt_tick_t tick;

    tick = rt_tick_from_millisecond(ms);

    return rt_thread_sleep(tick);
}
RTM_EXPORT(rt_thread_mdelay);

/**
 * This function will control thread behaviors according to control command.
 *
 * @param thread the specified thread to be controlled
 * @param cmd the control command, which includes
 *  RT_THREAD_CTRL_CHANGE_PRIORITY for changing priority level of thread;
 *  RT_THREAD_CTRL_STARTUP for starting a thread;
 *  RT_THREAD_CTRL_CLOSE for delete a thread.
 * @param arg the argument of control command
 *
 * @return RT_EOK
 */
rt_err_t rt_thread_control(rt_thread_t thread, int cmd, void *arg)
{
    /* thread check */
    RT_ASSERT(thread != RT_NULL);

    switch (cmd)
    {
    case RT_THREAD_CTRL_CHANGE_PRIORITY:
        /* change thread priority */
        thread->current_priority = *(rt_uint8_t *)arg;
        vTaskPrioritySet((TaskHandle_t)thread, RT_THREAD_PRIORITY_MAX - thread->current_priority);
        break;

    case RT_THREAD_CTRL_STARTUP:
        return rt_thread_startup(thread);

#ifdef RT_USING_HEAP
    case RT_THREAD_CTRL_CLOSE:
        return rt_thread_delete(thread);
#endif

    default:
        break;
    }

    return RT_EOK;
}
RTM_EXPORT(rt_thread_control);

/**
 * This function will suspend the specified thread.
 *
 * @param thread the thread to be suspended
 *
 * @return the operation status, RT_EOK on OK, -RT_ERROR on error
 *
 * @note if suspend self thread, after this function call, the
 * rt_schedule() must be invoked.
 */
rt_err_t rt_thread_suspend(rt_thread_t thread)
{
    register rt_base_t temp;

    /* thread check */
    RT_ASSERT(thread != RT_NULL);

    /* disable interrupt */
    temp = rt_hw_interrupt_disable();

    /* stop thread timer anyway */
    rt_timer_stop(&(thread->thread_timer));

    /* enable interrupt */
    rt_hw_interrupt_enable(temp);

    vTaskSuspendV2((TaskHandle_t)thread);

    return RT_EOK;
}
RTM_EXPORT(rt_thread_suspend);

/**
 * This function will resume a thread and put it to system ready queue.
 *
 * @param thread the thread to be resumed
 *
 * @return the operation status, RT_EOK on OK, -RT_ERROR on error
 */
rt_err_t rt_thread_resume(rt_thread_t thread)
{
    RT_ASSERT(thread != RT_NULL);

    /* remove from suspend list */
    rt_list_remove(&(thread->tlist));

    rt_timer_stop(&thread->thread_timer);

    if (xPortIsInsideInterrupt() == pdFALSE)
        vTaskResumeV2((TaskHandle_t)thread);
    else
        xTaskResumeFromISR((TaskHandle_t)thread);

    return RT_EOK;
}
RTM_EXPORT(rt_thread_resume);

/**
 * This function is the timeout function for thread, normally which is invoked
 * when thread is timeout to wait some resource.
 *
 * @param parameter the parameter of thread timeout function
 */
void rt_thread_timeout(void *parameter)
{
    struct rt_thread *thread;

    thread = (struct rt_thread *)parameter;

    /* thread check */
    RT_ASSERT(thread != RT_NULL);

    /* set error number */
    thread->error = -RT_ETIMEOUT;

    /* remove from suspend list */
    rt_list_remove(&(thread->tlist));

    rt_thread_resume(thread);

    rt_schedule();
}
RTM_EXPORT(rt_thread_timeout);

/**
 * This function will find the specified thread.
 *
 * @param name the name of thread finding
 *
 * @return the found thread
 *
 * @note please don't invoke this function in interrupt status.
 */
rt_thread_t rt_thread_find(char *name)
{
    return (rt_thread_t)xTaskGetHandle(name);
}
RTM_EXPORT(rt_thread_find);

void rt_thread_get_stack_info(rt_thread_t thread, uint32_t *pxStack, uint32_t *pxSize)
{
    vTaskGetStackInfo((TaskHandle_t)thread, pxStack, pxSize);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for (;;);
}

#if (configSUPPORT_STATIC_ALLOCATION == 1)
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
#endif

/**@}*/
