/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-04-12     misonyo      the first version
 * 2019-04-04     misonyo      fix some bugs
 */

#include <cmsis_os2.h>
#include "cmsis_rtthread.h"
#include "board.h"
#include "os_tick.h"
#include <rthw.h>

/// Kernel Information
#define API_VERSION         20010002   ///< API version (2.1.2)
///< RT-Thread Kernel version
#define RT_KERNEL_VERSION            (((rt_uint32_t)RT_VERSION * 10000000UL) | \
                                   ((rt_uint32_t)RT_SUBVERSION *    10000UL) | \
                                   ((rt_uint32_t)RT_REVISION *        1UL))
#define KERNEL_Id     "RT-Thread"  ///< Kernel identification string

#define DEFAULT_STACK_SIZE 512
#define DEFAULT_TICK 5
#define WAITING_THREAD_FLAGS 0x08
#define MALLOC_CB 0x10
#define MALLOC_STACK 0x04
#define MALLOC_MEM 0x02

extern void rt_thread_exit(void);

static osKernelState_t kernel_state = osKernelInactive;

static void thread_cleanup(rt_thread_t thread)
{
    thread_cb_t *thread_cb;
    thread_cb = (thread_cb_t *)(thread->user_data);

    /* clear cleanup function */
    thread->cleanup = RT_NULL;
    if (thread_cb->flags & osThreadJoinable)
    {
        rt_sem_release(thread_cb->joinable_sem);
    }
    else
    {
        if (thread_cb->flags & MALLOC_STACK)
            rt_free(thread_cb->thread.stack_addr);

        if (thread_cb->flags & MALLOC_CB)
            rt_free(thread_cb);
    }
}

//  ==== Kernel Management Functions ====

//  ==== Helper functions ====
/// Block Kernel (disable: thread switching, time tick, post ISR processing).
#ifdef SysTick

static volatile uint8_t          blocked;  ///< Blocked
static uint8_t                    pendSV;  ///< Pending SV

/// Get Pending SV (Service Call) Flag
/// \return     Pending SV Flag
__STATIC_INLINE uint8_t GetPendSV (void) 
{
    return ((uint8_t)((SCB->ICSR & (SCB_ICSR_PENDSVSET_Msk)) >> 24));
}

/// Clear Pending SV (Service Call) Flag
__STATIC_INLINE void ClrPendSV (void) 
{
    SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;
}

/// Set Pending SV (Service Call) Flag
__STATIC_INLINE void SetPendSV (void) 
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/// Block Kernel (disable: thread switching, time tick, post ISR processing).
static void KernelBlock (void) 
{

    OS_Tick_Disable();

    blocked = 1U;
    __DSB();

    if (GetPendSV() != 0U) 
    {
        ClrPendSV();
        pendSV = 1U;
    }
}

/// Unblock Kernel
static void KernelUnblock (void)
{

    blocked = 0U;
    __DSB();

    if (pendSV != 0U) 
    {
        pendSV = 0U;
        SetPendSV();
    }

    OS_Tick_Enable();
}

#endif

/// Initialize the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelInitialize(void)
{
    kernel_state = osKernelReady;

    return osOK;
}

///  Get RTOS Kernel Information.
/// \param[out]    version       pointer to buffer for retrieving version information.
/// \param[out]    id_buf        pointer to buffer for retrieving kernel identification string.
/// \param[in]     id_size       size of buffer for kernel identification string.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelGetInfo(osVersion_t *version, char *id_buf, uint32_t id_size)
{
    if ((RT_NULL == version) || (RT_NULL == id_buf) || id_size < sizeof(KERNEL_Id))
        return osErrorParameter;

    version->api = API_VERSION;
    version->kernel = RT_KERNEL_VERSION;

    id_size = sizeof(KERNEL_Id);
    rt_strncpy(id_buf, KERNEL_Id, id_size);

    return osOK;
}

/// Get the current RTOS Kernel state.
/// \return current RTOS Kernel state.
osKernelState_t osKernelGetState(void)
{
    return kernel_state;
}

/// Start the RTOS Kernel scheduler.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelStart(void)
{
    osStatus_t state;

    if (osKernelReady == kernel_state)
    {
        kernel_state = osKernelRunning;

        state = osOK;
    }
    else
    {
        state = osError;
    }

    return state;
}

/// Lock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelLock(void)
{
    rt_uint16_t level;

    level = rt_critical_level();

    rt_enter_critical();

    kernel_state = osKernelLocked;

    if (level)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/// Unlock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelUnlock(void)
{
    rt_uint16_t level;

    level = rt_critical_level();

    if (0U == level)
    {
        return 0;
    }
    else
    {
        rt_exit_critical();
        if (1u == level)
            kernel_state = osKernelRunning;

        return 1;
    }
}

/// Restore the RTOS Kernel scheduler lock state.
/// \param[in]     lock          lock state obtained by \ref osKernelLock or \ref osKernelUnlock.
/// \return new lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelRestoreLock(int32_t lock)
{
    if (1u == lock)
    {
        osKernelLock();

        return 1;
    }
    else
    {
        osKernelUnlock();

        return 0;
    }
}

/// Get the RTOS kernel tick count.
/// \return RTOS kernel current tick count.
uint32_t osKernelGetTickCount(void)
{
    return (uint32_t)rt_tick_get();
}

/// Get the RTOS kernel tick frequency.
/// \return frequency of the kernel tick.
uint32_t osKernelGetTickFreq(void)
{

    return RT_TICK_PER_SECOND;
}

/// The function returns the current RTOS kernel system timer as a 32-bit value.
/// \return RTOS kernel current system timer count as 32-bit value
#ifdef SysTick

uint32_t osKernelGetSysTimerCount(void)	
{
    uint32_t irqmask ;
    rt_tick_t ticks;
    uint32_t val;

    irqmask = rt_hw_interrupt_disable();

    ticks = rt_tick_get();
    val   = OS_Tick_GetCount();

    if (OS_Tick_GetOverflow() != 0U) 
    {
        val = OS_Tick_GetCount();
        ticks++;
    }
    
    val += ticks * OS_Tick_GetInterval();
    rt_hw_interrupt_enable(irqmask);

    return (val);
}

#endif    /*SysTick*/



/// Get the RTOS kernel system timer frequency.
/// \return frequency of the system timer.
uint32_t osKernelGetSysTimerFreq(void)
{
    return RT_TICK_PER_SECOND;
}

/// Suspend the RTOS Kernel scheduler.
/// \return time in ticks, for how long the system can sleep or power-down.
#ifdef SysTick

uint32_t osKernelSuspend (void)
{
    rt_thread_t thread;
    rt_timer_t timer;
    struct rt_object_information *info_thread;
    struct rt_object_information *info_timer;
    struct rt_list_node *node;
    rt_uint8_t timer_index = 0;
    rt_tick_t min_tick = 0;
    rt_tick_t cur_tick = 0;
    rt_tick_t temp_tick = 0;

	if (kernel_state != osKernelRunning)
	{
		return 0U;
	}
    
    info_thread = rt_object_get_information(RT_Object_Class_Thread);
    info_timer = rt_object_get_information(RT_Object_Class_Timer);
    
    KernelBlock();
    
    min_tick = osWaitForever;
    
    cur_tick = rt_tick_get();
    
    /*check thread delay list*/
    if (info_thread != NULL)
    {
        for (node = info_thread->object_list.next; node != &(info_thread->object_list); node = node->next)
        {
            thread = rt_list_entry(node, struct rt_thread, list);

            if (thread->thread_timer.parent.flag & RT_TIMER_FLAG_ACTIVATED)
            {
                temp_tick = thread->thread_timer.timeout_tick - cur_tick;

                if (temp_tick < min_tick)
                {
                    min_tick = temp_tick;
                }
            }
        }
    }
    
    /*check active timer list*/
    if (info_timer != NULL)
    {
        for (node = info_timer->object_list.next; node != &(info_timer->object_list); node = node->next)
        {
            timer = rt_list_entry(node, struct rt_timer, row[timer_index++]);
            
            if (timer->parent.flag & RT_TIMER_FLAG_ACTIVATED)
            {
                temp_tick = timer->timeout_tick - cur_tick;
                
                if (temp_tick < min_tick)
                {
                    min_tick = temp_tick;
                }
            }
        }
    }

    if (osWaitForever == min_tick)
        min_tick = 0U;

    kernel_state = osKernelSuspended;

    return (min_tick);
}
#endif /* SysTick */

/// Resume the RTOS Kernel scheduler.
/// \param[in]     sleep_ticks   time in ticks for how long the system was in sleep or power-down mode.
#ifdef SysTick

void osKernelResume (uint32_t sleep_ticks)
{
    rt_tick_t delay_tick = 0;

    if (kernel_state != osKernelSuspended)
    {
        return;
    }

    delay_tick = (rt_tick_t)sleep_ticks;
    
    rt_enter_critical();
    
    while(delay_tick > 0)
    {
        rt_tick_increase();   /*Process Thread Delay list and  Process Active Timer list*/
        delay_tick --;
    }
    
    rt_exit_critical();
    
    kernel_state = osKernelRunning;
    
    KernelUnblock();
    
    return;
}

#endif

//  ==== Thread Management Functions ====

/// Create a thread and add it to Active Threads.
/// \param[in]     func          thread function.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \param[in]     attr          thread attributes; NULL: default values.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadNew(osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
    void *stack;
    rt_uint8_t rtt_prio;
    rt_uint32_t stack_size;
    thread_cb_t *thread_cb;
    char name[RT_NAME_MAX];
    static rt_uint16_t thread_number = 1U;

    /* Check parameters */
    if (RT_NULL == func)
    {
        return RT_NULL;
    }

    if ((RT_NULL == attr) || (RT_NULL == attr->cb_mem))
    {
        thread_cb = rt_malloc(sizeof(thread_cb_t));
        if (RT_NULL == thread_cb)
            return RT_NULL;
        rt_memset(thread_cb, 0, sizeof(thread_cb_t));
        thread_cb->flags |= MALLOC_CB;
    }
    else
    {
        if (attr->cb_size >= sizeof(thread_cb_t))
        {
            thread_cb = attr->cb_mem;
            thread_cb->flags = 0;
        }
        else
            return RT_NULL;
    }

    if ((RT_NULL == attr) || (RT_NULL == attr->name))
        rt_snprintf(name, sizeof(name), "th%02d", thread_number ++);
    else
        rt_snprintf(name, sizeof(name), "%s", attr->name);

    if ((RT_NULL == attr) || (osPriorityNone == attr->priority))
    {
        thread_cb->prio = osPriorityNormal;
    }
    else
    {
        if ((attr->priority < osPriorityIdle) || (attr->priority > osPriorityISR))
            return RT_NULL;

        thread_cb->prio = attr->priority;
    }
    if ((RT_NULL == attr) || (0U == attr->stack_size))
        stack_size = DEFAULT_STACK_SIZE;
    else
        stack_size = attr->stack_size;

    if ((RT_NULL == attr) || (RT_NULL == attr->stack_mem))
    {
        stack = rt_malloc(stack_size);
        if (RT_NULL == stack)
        {
            if (thread_cb->flags & MALLOC_CB)
                rt_free(thread_cb);
            return RT_NULL;
        }
        thread_cb->flags |= MALLOC_STACK;
    }
    else
    {
        stack = (void *)(attr->stack_mem);
    }

    if ((RT_NULL != attr) && (0 != attr->attr_bits))
        thread_cb->flags |= attr->attr_bits;

    rtt_prio = (osPriorityISR - thread_cb->prio) * RT_THREAD_PRIORITY_MAX / osPriorityISR;
    rt_thread_init(&(thread_cb->thread), name, func, argument, stack, stack_size, rtt_prio, DEFAULT_TICK);

    if (thread_cb->flags & osThreadJoinable)
    {
        thread_cb->joinable_sem = rt_sem_create(name, 0, RT_IPC_FLAG_FIFO);
        if (RT_NULL == thread_cb->joinable_sem)
        {
            if (thread_cb->flags & MALLOC_CB)
                rt_free(thread_cb);
            if (thread_cb->flags & MALLOC_STACK)
                rt_free(stack);

            return RT_NULL;
        }
    }
    else
        thread_cb->joinable_sem = RT_NULL;

    thread_cb->thread.cleanup = thread_cleanup;
    thread_cb->thread.user_data = (rt_uint32_t)thread_cb;

    rt_thread_startup(&(thread_cb->thread));

    return thread_cb;
}

/// Get name of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return name as NULL terminated string.
const char *osThreadGetName(osThreadId_t thread_id)
{
    thread_cb_t *thread_cb  = (thread_cb_t *)thread_id;


    /* Check parameters */
    if ((RT_NULL == thread_cb) || (rt_object_get_type((rt_object_t)(&thread_cb->thread)) != RT_Object_Class_Thread))
    {
        return RT_NULL;
    }

    return thread_cb->thread.name;
}

/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadGetId(void)
{
    rt_thread_t thread;

    thread = rt_thread_self();

    return (osThreadId_t)(thread->user_data);
}

/// Get current thread state of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current thread state of the specified thread.
osThreadState_t osThreadGetState(osThreadId_t thread_id)
{
    osThreadState_t state;
    thread_cb_t *thread_cb  = (thread_cb_t *)thread_id;

    /* Check parameters */
    if ((RT_NULL == thread_cb) || (rt_object_get_type((rt_object_t)(&thread_cb->thread)) != RT_Object_Class_Thread))
    {
        return osThreadError;
    }

    switch (thread_cb->thread.stat)
    {
    case RT_THREAD_INIT:
        state = osThreadInactive;
        break;
    case RT_THREAD_READY:
        state = osThreadReady;
        break;
    case RT_THREAD_SUSPEND:
        state = osThreadBlocked;
        break;
    case RT_THREAD_RUNNING:
        state = osThreadRunning;
        break;
    case RT_THREAD_CLOSE:
        state = osThreadTerminated;
        break;
    default:
        state = osThreadError;
        break;
    }

    return state;
}

/// Get stack size of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return stack size in bytes.
uint32_t osThreadGetStackSize(osThreadId_t thread_id)
{
    thread_cb_t *thread_cb  = (thread_cb_t *)thread_id;

    /* Check parameters */
    if ((RT_NULL == thread_cb) || (rt_object_get_type((rt_object_t)(&thread_cb->thread)) != RT_Object_Class_Thread))
    {
        return 0U;
    }

    return ((uint32_t)thread_cb->thread.stack_size);
}

/// Get available stack space of a thread based on stack watermark recording during execution.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return remaining stack space in bytes.
uint32_t osThreadGetStackSpace(osThreadId_t thread_id)
{
    rt_uint8_t *ptr;
    thread_cb_t *thread_cb  = (thread_cb_t *)thread_id;

    /* Check parameters */
    if ((RT_NULL == thread_cb) || (rt_object_get_type((rt_object_t)(&thread_cb->thread)) != RT_Object_Class_Thread))
    {
        return 0U;
    }

    ptr = (rt_uint8_t *)thread_cb->thread.stack_addr;

    while (*ptr == '#')ptr ++;

    return (ptr - (rt_uint8_t *)thread_cb->thread.stack_addr);
}

/// Change priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSetPriority(osThreadId_t thread_id, osPriority_t priority)
{
    rt_uint8_t rt_priority;
    thread_cb_t *thread_cb = (thread_cb_t *)thread_id;

    // Check parameters
    if ((RT_NULL == thread_cb) || (priority < osPriorityIdle) || (priority > osPriorityISR))
    {
        return osErrorParameter;
    }

    thread_cb->prio = priority;
    rt_priority = (osPriorityISR - thread_cb->prio) * RT_THREAD_PRIORITY_MAX / osPriorityISR;

    rt_thread_control(&(thread_cb->thread), RT_THREAD_CTRL_CHANGE_PRIORITY, &rt_priority);

    return osOK;
}

/// Get current priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current priority value of the specified thread.
osPriority_t osThreadGetPriority(osThreadId_t thread_id)
{
    thread_cb_t *thread_cb = (thread_cb_t *)thread_id;

    /* Check parameters */
    if ((RT_NULL == thread_cb) || (rt_object_get_type((rt_object_t)(&thread_cb->thread)) != RT_Object_Class_Thread))
    {
        return osPriorityError;
    }

    return (osPriority_t)thread_cb->prio;
}

/// Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadYield(void)
{
    rt_thread_yield();

    return osOK;
}

/// Suspend execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSuspend(osThreadId_t thread_id)
{
    rt_err_t result;
    thread_cb_t *thread_cb = (thread_cb_t *)thread_id;

    /* Check parameters */
    if (RT_NULL == thread_cb)
    {
        return osErrorParameter;
    }

    result = rt_thread_suspend(&(thread_cb->thread));

    if (RT_EOK == result)
        return osOK;
    else
        return osError;
}

/// Resume execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadResume(osThreadId_t thread_id)
{
    rt_err_t result;
    thread_cb_t *thread_cb = (thread_cb_t *)thread_id;

    /* Check parameters */
    if (RT_NULL == thread_cb)
    {
        return osErrorParameter;
    }

    result = rt_thread_resume(&(thread_cb->thread));

    if (RT_EOK == result)
        return osOK;
    else
        return osError;
}

/// Detach a thread (thread storage can be reclaimed when thread terminates).
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadDetach(osThreadId_t thread_id)
{
    thread_cb_t *thread_cb  = (thread_cb_t *)thread_id;

    /* Check parameters */
    if ((RT_NULL == thread_cb) || (rt_object_get_type((rt_object_t)(&thread_cb->thread)) != RT_Object_Class_Thread))
    {
        return osErrorParameter;
    }

    /* Check object attributes */
    if ((thread_cb->flags & osThreadJoinable) == 0)
    {
        return osErrorResource;
    }

    if ((thread_cb->thread.stat & RT_THREAD_STAT_MASK) == RT_THREAD_CLOSE)
    {
        rt_thread_detach(&(thread_cb->thread));

        if (thread_cb->flags & osThreadJoinable)
            rt_sem_delete(thread_cb->joinable_sem);

        if (thread_cb->flags & MALLOC_STACK)
            rt_free(thread_cb->thread.stack_addr);

        if (thread_cb->flags & MALLOC_CB)
            rt_free(thread_cb);
    }
    else
    {
        rt_enter_critical();
        /* change to detach state */
        thread_cb->flags &= ~osThreadJoinable;
        /* delete joinable semaphore */
        rt_sem_delete(thread_cb->joinable_sem);
        thread_cb->joinable_sem = RT_NULL;
        rt_exit_critical();
    }

    return osOK;
}

/// Wait for specified thread to terminate.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadJoin(osThreadId_t thread_id)
{
    rt_err_t result;
    thread_cb_t *thread_cb = (thread_cb_t *)thread_id;

    /* Check parameters */
    if (RT_NULL == thread_cb)
    {
        return osErrorParameter;
    }

    if (((&thread_cb->thread) == rt_thread_self()) ||
            (0 == (thread_cb->flags & osThreadJoinable)))
    {
        /* join self or join a detached thread*/
        return osErrorResource;
    }

    result = rt_sem_take(thread_cb->joinable_sem, RT_WAITING_FOREVER);
    if (RT_EOK == result)
    {
        /* release resource */
        if (thread_cb->flags & osThreadJoinable)
            rt_sem_delete(thread_cb->joinable_sem);

        if (thread_cb->flags & MALLOC_STACK)
            rt_free(thread_cb->thread.stack_addr);

        if (thread_cb->flags & MALLOC_CB)
            rt_free(thread_cb);
    }
    else
        return osError;

    return osOK;
}

/// Terminate execution of current running thread.
__NO_RETURN void osThreadExit(void)  /* doing */
{
    rt_thread_exit();

    RT_ASSERT(0);
    while (1);
}

/// Terminate execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadTerminate(osThreadId_t thread_id)
{
    thread_cb_t *thread_cb;

    thread_cb = (thread_cb_t *)(rt_thread_self()->user_data);

    /* Check parameters */
    if (RT_NULL == thread_cb)
    {
        return osErrorParameter;
    }

    rt_thread_detach(&(thread_cb->thread));
    rt_schedule();

    return osOK;
}

/// Get number of active threads.
/// \return number of active threads.
uint32_t osThreadGetCount(void)
{
    rt_uint32_t thread_count = 0U;
    struct rt_object_information *info;

    info = rt_object_get_information(RT_Object_Class_Thread);

    rt_enter_critical();
    thread_count = rt_list_len(&(info->object_list));
    rt_exit_critical();

    return thread_count;
}

/// Enumerate active threads.
/// \param[out]    thread_array  pointer to array for retrieving thread IDs.
/// \param[in]     array_items   maximum number of items in array for retrieving thread IDs.
/// \return number of enumerated threads.
uint32_t osThreadEnumerate(osThreadId_t *thread_array, uint32_t array_items)
{
    rt_uint32_t thread_count = 0U;
    rt_thread_t thread;
    struct rt_object_information *info;
    struct rt_list_node *node;

    /* Check parameters */
    if ((RT_NULL == thread_array) || (0U == array_items))
    {
        return 0U;
    }

    info = rt_object_get_information(RT_Object_Class_Thread);

    rt_enter_critical();
    for (node = info->object_list.next; node != &(info->object_list); node = node->next)
    {
        thread = rt_list_entry(node, struct rt_thread, list);
        thread_array[thread_count] = (osThreadId_t)thread;
        thread_count++;

        if (thread_count >= array_items)
            break;
    }
    rt_exit_critical();

    return thread_count;
}

//  ==== Thread Flags Functions ====

/// Set the specified Thread Flags of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     flags         specifies the flags of the thread that shall be set.
/// \return thread flags after setting or error code if highest bit set.
uint32_t osThreadFlagsSet(osThreadId_t thread_id, uint32_t flags)
{
    register rt_base_t status;
    register rt_ubase_t level;
    rt_bool_t need_schedule = RT_FALSE;
    thread_cb_t *thread_cb;
    rt_uint32_t return_value;

    thread_cb = (thread_cb_t *)(thread_id);

    /* Check parameters */
    if ((RT_NULL == thread_cb) || (rt_object_get_type((rt_object_t)(&thread_cb->thread)) != RT_Object_Class_Thread))
    {
        return osFlagsErrorParameter;
    }

    level = rt_hw_interrupt_disable();

    thread_cb->flag_set |= flags;
    return_value = thread_cb->flag_set;

    /* Check if Thread is waiting for Thread Flags */
    if (thread_cb->thread.event_info & WAITING_THREAD_FLAGS)
    {
        status = -RT_ERROR;
        if (thread_cb->thread.event_info & osFlagsWaitAll)
        {
            if ((thread_cb->thread.event_set & thread_cb->flag_set) == thread_cb->thread.event_set)
            {
                /* received an AND event */
                status = RT_EOK;
            }
        }
        else
        {
            if (thread_cb->thread.event_set & thread_cb->flag_set)
            {
                /* save recieved event set */
                thread_cb->thread.event_set &= thread_cb->flag_set;
                /* received an OR event */
                status = RT_EOK;
            }
        }

        /* condition is satisfied, resume thread */
        if (RT_EOK == status)
        {
            thread_cb->thread.event_info &= ~WAITING_THREAD_FLAGS;
            /* clear event */
            if (!(thread_cb->thread.event_info & osFlagsNoClear))
                thread_cb->flag_set &= ~thread_cb->thread.event_set;

            /* resume thread, and thread list breaks out */
            rt_thread_resume(rt_thread_self());
            need_schedule = RT_TRUE;
        }
    }

    rt_hw_interrupt_enable(level);

    if (need_schedule == RT_TRUE)
        rt_schedule();

    return return_value;
}

/// Clear the specified Thread Flags of current running thread.
/// \param[in]     flags         specifies the flags of the thread that shall be cleared.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsClear(uint32_t flags)
{
    rt_thread_t thread = rt_thread_self();
    thread_cb_t *thread_cb;
    rt_uint32_t flag;

    /* Check parameters */
    if (RT_NULL == thread)
    {
        return osFlagsErrorParameter;
    }

    thread_cb = (thread_cb_t *)(thread->user_data);

    rt_enter_critical();

    flag = thread_cb->flag_set;
    thread_cb->flag_set &= ~flags;

    rt_exit_critical();

    return flag;
}

/// Get the current Thread Flags of current running thread.
/// \return current thread flags.
uint32_t osThreadFlagsGet(void)
{
    rt_thread_t thread = rt_thread_self();
    thread_cb_t *thread_cb;

    /* Check parameters */
    if (RT_NULL == thread)
    {
        return osFlagsErrorParameter;
    }

    thread_cb = (thread_cb_t *)(thread->user_data);

    return thread_cb->flag_set;
}

/// Wait for one or more Thread Flags of the current running thread to become signaled.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsWait(uint32_t flags, uint32_t options, uint32_t timeout)
{
    rt_uint32_t return_value;
    register rt_ubase_t level;
    register rt_base_t status = -RT_ERROR;
    rt_thread_t thread = rt_thread_self();
    thread_cb_t *thread_cb;

    /* Check parameters */
    if (RT_NULL == thread)
    {
        return osFlagsErrorParameter;
    }

    thread->error = RT_EOK;
    thread_cb = (thread_cb_t *)(thread->user_data);

    level = rt_hw_interrupt_disable();

    if (options & osFlagsWaitAll)
    {
        if ((thread_cb->flag_set & flags) == flags)
            status = RT_EOK;
    }
    else
    {
        if (thread_cb->flag_set & flags)
            status = RT_EOK;
    }

    if (RT_EOK == status)
    {
        return_value = thread_cb->flag_set & flags;
        if (!(options & osFlagsNoClear))
            thread_cb->flag_set &= ~flags;
    }
    else if (0U == timeout)
    {
        rt_hw_interrupt_enable(level);
        return osFlagsErrorResource;
    }
    else
    {
        thread->event_set = flags;
        thread->event_info = options | WAITING_THREAD_FLAGS;
        rt_thread_suspend(thread);
        /* if there is a waiting timeout, active thread timer */
        if ((timeout > 0U) && (timeout != osWaitForever))
        {
            /* reset the timeout of thread timer and start it */
            rt_timer_control(&(thread->thread_timer),
                             RT_TIMER_CTRL_SET_TIME,
                             &timeout);
            rt_timer_start(&(thread->thread_timer));
        }

        rt_hw_interrupt_enable(level);
        rt_schedule();

        if (thread->error != RT_EOK)
        {
            return thread->error;
        }

        level = rt_hw_interrupt_disable();
        return_value = thread->event_set;
    }

    rt_hw_interrupt_enable(level);

    return return_value;
}

//  ==== Generic Wait Functions ====

/// Wait for Timeout (Time Delay).
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value
/// \return status code that indicates the execution status of the function.
osStatus_t osDelay(uint32_t ticks)
{
    rt_thread_delay(ticks);

    return osOK;
}

/// Wait until specified time.
/// \param[in]     ticks         absolute time in ticks
/// \return status code that indicates the execution status of the function.
osStatus_t osDelayUntil(uint32_t ticks)
{
    uint64_t cur_ticks;

    cur_ticks = rt_tick_get();

    if (ticks == cur_ticks)
    {

    }
    else if (ticks > cur_ticks)
    {
        rt_thread_delay(ticks - cur_ticks);
    }
    else
    {
        rt_thread_delay(((rt_uint32_t) - 1) - cur_ticks + ticks);
    }

    return osOK;
}

//  ==== Timer Management Functions ====

/// Create and Initialize a timer.
/// \param[in]     func          start address of a timer call back function.
/// \param[in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
/// \param[in]     argument      argument to the timer call back function.
/// \param[in]     attr          timer attributes; NULL: default values.
/// \return timer ID for reference by other functions or NULL in case of error.

osTimerId_t osTimerNew(osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
    timer_cb_t *timer_cb;
    char name[RT_NAME_MAX];
    static rt_uint16_t timer_number = 0U;
    rt_uint8_t flag = RT_TIMER_FLAG_SOFT_TIMER;

    /* Check parameters */
    if ((RT_NULL == func) || ((type != osTimerOnce) && (type != osTimerPeriodic)))
    {
        return RT_NULL;
    }

    /* RT-Thread object's name can't be NULL */
    if ((RT_NULL == attr) || (RT_NULL == attr->name))
        rt_snprintf(name, sizeof(name), "timer%02d", timer_number++);
    else
        rt_snprintf(name, sizeof(name), "%s", attr->name);

    if ((RT_NULL == attr) || (RT_NULL == attr->cb_mem))
    {
        timer_cb = rt_malloc(sizeof(timer_cb_t));
        if (RT_NULL == timer_cb)
            return RT_NULL;
        rt_memset(timer_cb, 0, sizeof(timer_cb_t));
        timer_cb->flags |= MALLOC_CB;
    }
    else
    {
        if (attr->cb_size >= sizeof(timer_cb_t))
        {
            timer_cb = attr->cb_mem;
            timer_cb->flags = 0;
        }
        else
            return RT_NULL;
    }

    if (osTimerPeriodic == type)
    {
        flag |= RT_TIMER_FLAG_PERIODIC;
    }

    rt_timer_init(&(timer_cb->timer), name, func, argument, 0, flag);

    return timer_cb;
}

/// Get name of a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return name as NULL terminated string.
const char *osTimerGetName(osTimerId_t timer_id)
{
    timer_cb_t *timer_cb;

    timer_cb = (timer_cb_t *)timer_id;

    /* Check parameters */
    if ((RT_NULL == timer_cb) || (rt_object_get_type(&timer_cb->timer.parent) != RT_Object_Class_Timer))
    {
        return RT_NULL;
    }

    return timer_cb->timer.parent.name;
}

/// Start or restart a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value of the timer.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStart(osTimerId_t timer_id, uint32_t ticks)
{
    rt_err_t result;
    timer_cb_t *timer_cb;

    timer_cb = (timer_cb_t *)timer_id;
    /* Check parameters */
    if ((RT_NULL == timer_cb) || (ticks == 0))
    {
        return osErrorParameter;
    }

    rt_timer_control(&(timer_cb->timer), RT_TIMER_CTRL_SET_TIME, &ticks);

    result = rt_timer_start(&(timer_cb->timer));
    if (RT_EOK == result)
        return osOK;
    else
        return osError;
}

/// Stop a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStop(osTimerId_t timer_id)
{
    rt_err_t result;
    timer_cb_t *timer_cb;

    timer_cb = (timer_cb_t *)timer_id;

    /* Check parameters */
    if (RT_NULL == timer_cb)
    {
        return osErrorParameter;
    }

    result = rt_timer_stop(&(timer_cb->timer));

    if (RT_EOK == result)
        return osOK;
    else
        return osError;
}

/// Check if a timer is running.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return 0 not running, 1 running.
uint32_t osTimerIsRunning(osTimerId_t timer_id)
{
    timer_cb_t *timer_cb;

    timer_cb = (timer_cb_t *)timer_id;

    /* Check parameters */
    if ((RT_NULL == timer_cb) || (rt_object_get_type(&timer_cb->timer.parent) != RT_Object_Class_Timer))
    {
        return 0U;
    }

    if ((timer_cb->timer.parent.flag & RT_TIMER_FLAG_ACTIVATED) == 1u)
    {
        return 1;
    }
    else
        return 0U;
}

/// Delete a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerDelete(osTimerId_t timer_id)
{
    timer_cb_t *timer_cb;

    timer_cb = (timer_cb_t *)timer_id;

    /* Check parameters */
    if (RT_NULL == timer_cb)
    {
        return osErrorParameter;
    }

    rt_timer_detach(&(timer_cb->timer));

    if (timer_cb->flags & MALLOC_CB)
        rt_free(timer_cb);

    return osOK;
}

#ifdef RT_USING_EVENT
//  ==== Event Flags Management Functions ====

/// Create and Initialize an Event Flags object.
/// \param[in]     attr          event flags attributes; NULL: default values.
/// \return event flags ID for reference by other functions or NULL in case of error.
osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr)
{
    char name[RT_NAME_MAX];
    event_cb_t *event_cb;
    static rt_uint16_t event_number = 0U;

    if ((RT_NULL == attr) || (RT_NULL == attr->name))
    {
        rt_snprintf(name, sizeof(name), "event%02d", event_number++);
    }
    else
        rt_snprintf(name, sizeof(name), "%s", attr->name);

    if ((RT_NULL == attr) || (RT_NULL == attr->cb_mem))
    {
        event_cb = rt_malloc(sizeof(event_cb_t));
        if (RT_NULL == event_cb)
            return RT_NULL;
        rt_memset(event_cb, 0, sizeof(event_cb_t));
        event_cb->flags |= MALLOC_CB;
    }
    else
    {
        if (attr->cb_size >= sizeof(event_cb_t))
        {
            event_cb = attr->cb_mem;
            event_cb->flags = 0;
        }
        else
            return RT_NULL;
    }

    rt_event_init(&(event_cb->event), name, RT_IPC_FLAG_FIFO);

    return event_cb;
}

/// Get name of an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return name as NULL terminated string.
const char *osEventFlagsGetName(osEventFlagsId_t ef_id)
{
    event_cb_t *event_cb = (event_cb_t *)ef_id;

    /* Check parameters */
    if ((RT_NULL == event_cb) || (rt_object_get_type(&event_cb->event.parent.parent) != RT_Object_Class_Event))
    {
        return RT_NULL;
    }

    return event_cb->event.parent.parent.name;
}

/// Set the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be set.
/// \return event flags after setting or error code if highest bit set.
uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags)
{
    rt_err_t result;
    rt_uint32_t set_flags;
    event_cb_t *event_cb = (event_cb_t *)ef_id;

    /* Check parameters */
    if ((RT_NULL == event_cb) || (rt_object_get_type(&event_cb->event.parent.parent) != RT_Object_Class_Event))
    {
        return ((uint32_t)osFlagsErrorParameter);
    }

    set_flags = event_cb->event.set |= flags;

    result = rt_event_send(&(event_cb->event), flags);

    if (RT_EOK == result)
        return set_flags;
    else
        return osFlagsError;
}

/// Clear the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be cleared.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsClear(osEventFlagsId_t ef_id, uint32_t flags)
{
    rt_uint32_t set_flags;
    register rt_ubase_t level;
    event_cb_t *event_cb = (event_cb_t *)ef_id;

    /* Check parameters */
    if ((RT_NULL == event_cb) || (rt_object_get_type(&event_cb->event.parent.parent) != RT_Object_Class_Event))
    {
        return ((uint32_t)osFlagsErrorParameter);
    }

    set_flags = event_cb->event.set;

    level = rt_hw_interrupt_disable();

    event_cb->event.set &= ~flags;

    rt_hw_interrupt_enable(level);

    return set_flags;
}

/// Get the current Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return current event flags.
uint32_t osEventFlagsGet(osEventFlagsId_t ef_id)
{
    event_cb_t *event_cb = (event_cb_t *)ef_id;

    /* Check parameters */
    if ((RT_NULL == event_cb) || (rt_object_get_type(&event_cb->event.parent.parent) != RT_Object_Class_Event))
    {
        return 0U;
    }

    return event_cb->event.set;
}

/// Wait for one or more Event Flags to become signaled.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{
    rt_err_t result;
    rt_uint32_t rt_recv;
    rt_uint8_t rt_options = 0U;
    event_cb_t *event_cb = (event_cb_t *)ef_id;

    /* Check parameters */
    if (RT_NULL == event_cb)
    {
        return ((uint32_t)osFlagsErrorParameter);
    }

    if (options & osFlagsWaitAll)
    {
        rt_options |= RT_EVENT_FLAG_AND;
    }
    else
    {
        rt_options |= RT_EVENT_FLAG_OR;
    }

    if (!(options & osFlagsNoClear))
    {
        rt_options |= RT_EVENT_FLAG_CLEAR;
    }

    result = rt_event_recv(&(event_cb->event), flags, (rt_uint8_t)rt_options, timeout, &rt_recv);

    if (RT_EOK == result)
        return rt_recv;
    else if (-RT_ETIMEOUT == result)
    {
        if (0U == timeout)
            return osFlagsErrorResource;
        else
            return osFlagsErrorTimeout;
    }
    else
        return osFlagsErrorUnknown;
}

/// Delete an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osEventFlagsDelete(osEventFlagsId_t ef_id)
{
    event_cb_t *event_cb = (event_cb_t *)ef_id;

    /* Check parameters */
    if (RT_NULL == event_cb)
    {
        return osErrorParameter;
    }

    rt_event_detach(&(event_cb->event));

    if (event_cb->flags & MALLOC_CB)
        rt_free(event_cb);

    return osOK;
}

#endif

#ifdef RT_USING_MUTEX
//  ==== Mutex Management Functions ====

/// Create and Initialize a Mutex object.
/// \param[in]     attr          mutex attributes; NULL: default values.
/// \return mutex ID for reference by other functions or NULL in case of error.
osMutexId_t osMutexNew(const osMutexAttr_t *attr)
{
    char name[RT_NAME_MAX];
    mutex_cb_t *mutex_cb;
    static rt_uint16_t mutex_number = 0U;

    if ((RT_NULL == attr) || (RT_NULL == attr->name))
    {
        rt_snprintf(name, sizeof(name), "mutex%02d", mutex_number++);
    }
    else
        rt_snprintf(name, sizeof(name), "%s", attr->name);

    if ((RT_NULL == attr) || (RT_NULL == attr->cb_mem))
    {
        mutex_cb = rt_malloc(sizeof(mutex_cb_t));
        if (RT_NULL == mutex_cb)
            return RT_NULL;
        rt_memset(mutex_cb, 0, sizeof(mutex_cb_t));
        mutex_cb->flags |= MALLOC_CB;
    }
    else
    {
        if (attr->cb_size >= sizeof(mutex_cb_t))
        {
            mutex_cb = attr->cb_mem;
            mutex_cb->flags = 0;
        }
        else
            return RT_NULL;
    }

    if ((RT_NULL == attr) || (0 == attr->attr_bits))
    {
        mutex_cb->flags |= osMutexRecursive;
    }
    else
        mutex_cb->flags |= attr->attr_bits;

    rt_mutex_init(&(mutex_cb->mutex), name, RT_IPC_FLAG_FIFO);

    return mutex_cb;
}

/// Get name of a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return name as NULL terminated string.
const char *osMutexGetName(osMutexId_t mutex_id)
{
    mutex_cb_t *mutex_cb = (mutex_cb_t *)mutex_id;

    /* Check parameters */
    if ((RT_NULL == mutex_cb) || (rt_object_get_type(&mutex_cb->mutex.parent.parent) != RT_Object_Class_Mutex))
    {
        return RT_NULL;
    }

    return mutex_cb->mutex.parent.parent.name;
}

/// Acquire a Mutex or timeout if it is locked.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout)
{
    rt_err_t result;
    mutex_cb_t *mutex_cb = (mutex_cb_t *)mutex_id;

    /* Check parameters */
    if (RT_NULL == mutex_cb)
    {
        return osErrorParameter;
    }

    rt_enter_critical();
    if ((mutex_cb->mutex.owner == rt_thread_self()) && !(mutex_cb->flags & osMutexRecursive))
    {
        rt_exit_critical();
        return osError;
    }
    rt_exit_critical();

    result = rt_mutex_take(&(mutex_cb->mutex), timeout);

    if (RT_EOK == result)
        return osOK;
    else if (-RT_ETIMEOUT == result)
    {
        if (0U == timeout)
            return osErrorResource;
        else
            return osErrorTimeout;
    }
    else
        return osError;
}

/// Release a Mutex that was acquired by \ref osMutexAcquire.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexRelease(osMutexId_t mutex_id)
{
    rt_err_t result;
    mutex_cb_t *mutex_cb = (mutex_cb_t *)mutex_id;

    /* Check parameters */
    if (RT_NULL == mutex_cb)
    {
        return osErrorParameter;
    }

    result = rt_mutex_release(&(mutex_cb->mutex));

    if (RT_EOK == result)
        return osOK;
    else
        return osErrorResource;
}

/// Get Thread which owns a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return thread ID of owner thread or NULL when mutex was not acquired.
osThreadId_t osMutexGetOwner(osMutexId_t mutex_id)
{
    mutex_cb_t *mutex_cb = (mutex_cb_t *)mutex_id;

    /* Check parameters */
    if ((RT_NULL == mutex_cb) || (rt_object_get_type(&mutex_cb->mutex.parent.parent) != RT_Object_Class_Mutex))
    {
        return RT_NULL;
    }

    return mutex_cb->mutex.owner;
}

/// Delete a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexDelete(osMutexId_t mutex_id)
{
    mutex_cb_t *mutex_cb = (mutex_cb_t *)mutex_id;

    /* Check parameters */
    if (RT_NULL == mutex_cb)
    {
        return osErrorParameter;
    }

    rt_mutex_detach(&(mutex_cb->mutex));

    if (mutex_cb->flags & MALLOC_CB)
        rt_free(mutex_cb);

    return osOK;
}

#endif

#ifdef RT_USING_SEMAPHORE
osSemaphoreId_t osSemaphoreNew(uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{
    char name[RT_NAME_MAX];
    sem_cb_t *sem_cb;
    static rt_uint16_t semaphore_number = 0U;

    /* Check parameters */
    if ((0U == max_count) || (initial_count > max_count))
    {
        return RT_NULL;
    }

    if ((RT_NULL == attr) || (RT_NULL == attr->name))
    {
        rt_snprintf(name, sizeof(name), "sem%02d", semaphore_number++);
    }
    else
        rt_snprintf(name, sizeof(name), "%s", attr->name);

    if ((RT_NULL == attr) || (RT_NULL == attr->cb_mem))
    {
        sem_cb = rt_malloc(sizeof(sem_cb_t));
        if (RT_NULL == sem_cb)
            return RT_NULL;
        rt_memset(sem_cb, 0, sizeof(sem_cb_t));
        sem_cb->flags |= MALLOC_CB;
    }
    else
    {
        if (attr->cb_size >= sizeof(sem_cb_t))
        {
            sem_cb = attr->cb_mem;
            sem_cb->flags = 0;
        }
        else
            return RT_NULL;
    }

    rt_sem_init(&(sem_cb->sem), name, initial_count, RT_IPC_FLAG_FIFO);

    return sem_cb;
}

/// Get name of a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return name as NULL terminated string.
const char *osSemaphoreGetName(osSemaphoreId_t semaphore_id)
{
    sem_cb_t *sem_cb = (sem_cb_t *)semaphore_id;

    /* Check parameters */
    if ((RT_NULL == sem_cb) || (rt_object_get_type(&sem_cb->sem.parent.parent) != RT_Object_Class_Semaphore))
    {
        return RT_NULL;
    }

    return sem_cb->sem.parent.parent.name;
}

/// Acquire a Semaphore token or timeout if no tokens are available.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreAcquire(osSemaphoreId_t semaphore_id, uint32_t timeout)
{
    rt_err_t result;
    sem_cb_t *sem_cb = (sem_cb_t *)semaphore_id;

    /* Check parameters */
    if (RT_NULL == sem_cb)
    {
        return osErrorParameter;
    }

    result = rt_sem_take(&(sem_cb->sem), timeout);

    if (RT_EOK == result)
        return osOK;
    else if (-RT_ETIMEOUT == result)
    {
        if (0U == timeout)
            return osErrorResource;
        else
            return osErrorTimeout;
    }
    else
        return osError;
}

/// Release a Semaphore token that was acquired by \ref osSemaphoreAcquire.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id)
{
    rt_err_t result;
    sem_cb_t *sem_cb = (sem_cb_t *)semaphore_id;

    /* Check parameters */
    if (RT_NULL == sem_cb)
    {
        return osErrorParameter;
    }

    result = rt_sem_release(&(sem_cb->sem));

    if (RT_EOK == result)
        return osOK;
    else
        return osError;
}

/// Get current Semaphore token count.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return number of tokens available.
uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphore_id)
{
    sem_cb_t *sem_cb = (sem_cb_t *)semaphore_id;

    /* Check parameters */
    if ((RT_NULL == sem_cb) || (rt_object_get_type(&sem_cb->sem.parent.parent) != RT_Object_Class_Semaphore))
    {
        return 0U;
    }

    return sem_cb->sem.value;
}

/// Delete a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreDelete(osSemaphoreId_t semaphore_id)
{
    sem_cb_t *sem_cb = (sem_cb_t *)semaphore_id;

    /* Check parameters */
    if (RT_NULL == sem_cb)
    {
        return osErrorParameter;
    }

    rt_sem_detach(&(sem_cb->sem));

    if (sem_cb->flags & MALLOC_CB)
        rt_free(sem_cb);

    return osOK;
}

#endif

#ifdef RT_USING_MEMPOOL
//  ==== Memory Pool Management Functions ====

/// Create and Initialize a Memory Pool object.
/// \param[in]     block_count   maximum number of memory blocks in memory pool.
/// \param[in]     block_size    memory block size in bytes.
/// \param[in]     attr          memory pool attributes; NULL: default values.
/// \return memory pool ID for reference by other functions or NULL in case of error.
osMemoryPoolId_t osMemoryPoolNew(uint32_t block_count, uint32_t block_size, const osMemoryPoolAttr_t *attr)
{
    char name[RT_NAME_MAX];
    void *mp_addr;
    rt_uint32_t mp_size;
    mempool_cb_t *mempool_cb;
    static rt_uint16_t memory_pool_number = 0U;

    /* Check parameters */
    if ((0U == block_count) || (0U == block_size))
    {
        return RT_NULL;
    }

    if ((RT_NULL == attr) || (RT_NULL == attr->name))
    {
        rt_snprintf(name, sizeof(name), "mp%02d", memory_pool_number++);
    }
    else
        rt_snprintf(name, sizeof(name), "%s", attr->name);

    if ((RT_NULL == attr) || (RT_NULL == attr->cb_mem))
    {
        mempool_cb = rt_malloc(sizeof(mempool_cb_t));
        if (RT_NULL == mempool_cb)
            return RT_NULL;
        rt_memset(mempool_cb, 0, sizeof(mempool_cb_t));
        mempool_cb->flags |= MALLOC_CB;
    }
    else
    {
        if (attr->cb_size >= sizeof(mempool_cb_t))
        {
            mempool_cb = attr->cb_mem;
            mempool_cb->flags = 0;
        }
        else
            return RT_NULL;
    }

    if ((RT_NULL == attr) || (RT_NULL == attr->mp_mem))
    {
        block_size = RT_ALIGN(block_size, RT_ALIGN_SIZE);
        mp_size = (block_size + sizeof(rt_uint8_t *)) * block_count;
        mp_addr = rt_malloc((block_size + sizeof(rt_uint8_t *)) * block_count);

        if (RT_NULL == mp_addr)
        {
            if (mempool_cb->flags & MALLOC_CB)
                rt_free(mempool_cb);
            return RT_NULL;
        }
        mempool_cb->flags |= MALLOC_MEM;
    }
    else
    {
        mp_addr = (void *)(attr->mp_mem);
        mp_size = attr->mp_size;
    }

    rt_mp_init(&(mempool_cb->mp), name, mp_addr, mp_size, block_size);

    return mempool_cb;
}

/// Get name of a Memory Pool object.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return name as NULL terminated string.
const char *osMemoryPoolGetName(osMemoryPoolId_t mp_id)
{
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if ((RT_NULL == mempool_cb) || (rt_object_get_type(&mempool_cb->mp.parent) != RT_Object_Class_MemPool))
    {
        return RT_NULL;
    }

    return mempool_cb->mp.parent.name;
}

/// Allocate a memory block from a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return address of the allocated memory block or NULL in case of no memory is available.
void *osMemoryPoolAlloc(osMemoryPoolId_t mp_id, uint32_t timeout)
{
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if (RT_NULL == mempool_cb)
    {
        return RT_NULL;
    }

    return rt_mp_alloc(&(mempool_cb->mp), timeout);
}

/// Return an allocated memory block back to a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \param[in]     block         address of the allocated memory block to be returned to the memory pool.
/// \return status code that indicates the execution status of the function.
osStatus_t osMemoryPoolFree(osMemoryPoolId_t mp_id, void *block)
{
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if (RT_NULL == mempool_cb)
    {
        return osErrorParameter;
    }

    rt_mp_free(block);

    return osOK;
}

/// Get maximum number of memory blocks in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return maximum number of memory blocks.
uint32_t osMemoryPoolGetCapacity(osMemoryPoolId_t mp_id)
{
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if ((RT_NULL == mempool_cb) || (rt_object_get_type(&mempool_cb->mp.parent) != RT_Object_Class_MemPool))
    {
        return 0U;
    }

    return mempool_cb->mp.block_total_count;
}

/// Get memory block size in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return memory block size in bytes.
uint32_t osMemoryPoolGetBlockSize(osMemoryPoolId_t mp_id)
{
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if ((RT_NULL == mempool_cb) || (rt_object_get_type(&mempool_cb->mp.parent) != RT_Object_Class_MemPool))
    {
        return 0U;
    }

    return mempool_cb->mp.block_size;
}

/// Get number of memory blocks used in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return number of memory blocks used.
uint32_t osMemoryPoolGetCount(osMemoryPoolId_t mp_id)
{
    rt_size_t used_blocks;
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if ((RT_NULL == mempool_cb) || (rt_object_get_type(&mempool_cb->mp.parent) != RT_Object_Class_MemPool))
    {
        return 0U;
    }

    used_blocks = mempool_cb->mp.block_total_count - mempool_cb->mp.block_free_count;

    return (uint32_t)used_blocks;
}

/// Get number of memory blocks available in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return number of memory blocks available.
uint32_t osMemoryPoolGetSpace(osMemoryPoolId_t mp_id)
{
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if ((RT_NULL == mempool_cb) || (rt_object_get_type(&mempool_cb->mp.parent) != RT_Object_Class_MemPool))
    {
        return 0U;
    }

    return mempool_cb->mp.block_free_count;
}

/// Delete a Memory Pool object.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMemoryPoolDelete(osMemoryPoolId_t mp_id)
{
    mempool_cb_t *mempool_cb = (mempool_cb_t *)mp_id;

    /* Check parameters */
    if (RT_NULL == mempool_cb)
    {
        return osErrorParameter;
    }

    rt_mp_detach(&(mempool_cb->mp));

    if (mempool_cb->flags & MALLOC_CB)
        rt_free(mempool_cb);

    if (mempool_cb->flags & MALLOC_MEM)
        rt_free(mempool_cb->mp.start_address);

    return osOK;
}

#endif

#ifdef RT_USING_MESSAGEQUEUE
//  ==== Message Queue Management Functions ====
/// Create and Initialize a Message Queue object.
/// \param[in]     msg_count     maximum number of messages in queue.
/// \param[in]     msg_size      maximum message size in bytes.
/// \param[in]     attr          message queue attributes; NULL: default values.
/// \return message queue ID for reference by other functions or NULL in case of error.
osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr)
{
    char name[RT_NAME_MAX];
    mq_cb_t *mq_cb;
    void *mq_addr;
    rt_uint32_t mq_size;
    static rt_uint16_t mq_number = 0U;

    /* Check parameters */
    if ((0U == msg_count) || (0U == msg_size))
    {
        return RT_NULL;
    }

    if ((RT_NULL == attr) || (RT_NULL == attr->name))
    {
        rt_snprintf(name, sizeof(name), "mq%02d", mq_number++);
    }
    else
        rt_snprintf(name, sizeof(name), "%s", attr->name);

    if ((RT_NULL == attr) || (RT_NULL == attr->cb_mem))
    {
        mq_cb = rt_malloc(sizeof(mq_cb_t));
        if (RT_NULL == mq_cb)
            return RT_NULL;
        rt_memset(mq_cb, 0, sizeof(mq_cb_t));
        mq_cb->flags |= MALLOC_CB;
    }
    else
    {
        if (attr->cb_size >= sizeof(mq_cb_t))
        {
            mq_cb = attr->cb_mem;
            mq_cb->flags = 0;
        }
        else
            return RT_NULL;
    }

    mq_cb->init_msg_size = msg_size;

    if ((RT_NULL == attr) || (RT_NULL == attr->mq_mem))
    {
        mq_size = (msg_size + sizeof(struct rt_messagequeue)) * msg_count;
        mq_addr = rt_malloc(mq_size);

        if (RT_NULL == mq_addr)
        {
            if (mq_cb->flags & MALLOC_CB)
                rt_free(mq_cb);
            return RT_NULL;
        }
        rt_memset(mq_addr, 0, sizeof(mq_size));
        mq_cb->flags |= MALLOC_MEM;
    }
    else
    {
        mq_addr = (void *)(attr->mq_mem);
        mq_size = attr->mq_size;
    }

    rt_mq_init(&(mq_cb->mq), name, mq_addr, msg_size, mq_size, RT_IPC_FLAG_FIFO);

    return mq_cb;
}

/// Get name of a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return name as NULL terminated string.
const char *osMessageQueueGetName(osMessageQueueId_t mq_id)
{
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */
    if ((RT_NULL == mq_cb) || (rt_object_get_type(&mq_cb->mq.parent.parent) != RT_Object_Class_MessageQueue))
    {
        return RT_NULL;
    }

    return mq_cb->mq.parent.parent.name;
}

/// Put a Message into a Queue or timeout if Queue is full.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[in]     msg_ptr       pointer to buffer with message to put into a queue.
/// \param[in]     msg_prio      message priority.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
    rt_err_t result;
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */
    if (RT_NULL == mq_cb || (RT_NULL == msg_ptr))
    {
        return osErrorParameter;
    }

    result = rt_mq_send(&(mq_cb->mq), (void *)msg_ptr, mq_cb->init_msg_size);

    if (RT_EOK == result)
        return osOK;
    else if (-RT_EFULL == result)
        return osErrorResource;
    else
        return osError;
}

/// Get a Message from a Queue or timeout if Queue is empty.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
/// \param[out]    msg_prio      pointer to buffer for message priority or NULL.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
    rt_err_t result;
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */
    if (RT_NULL == mq_cb || (RT_NULL == msg_ptr))
    {
        return osErrorParameter;
    }
    result = rt_mq_recv(&(mq_cb->mq), msg_ptr, mq_cb->init_msg_size, timeout);

    if (RT_EOK == result)
        return osOK;
    else if (-RT_ETIMEOUT == result)
    {
        if (0U == timeout)
            return osErrorResource;
        return osErrorTimeout;
    }
    else
        return osError;
}

/// Get maximum number of messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum number of messages.
uint32_t osMessageQueueGetCapacity(osMessageQueueId_t mq_id)
{
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */
    if ((RT_NULL == mq_cb) || (rt_object_get_type(&mq_cb->mq.parent.parent) != RT_Object_Class_MessageQueue))
    {
        return 0U;
    }

    return mq_cb->mq.max_msgs;
}

/// Get maximum message size in a Memory Pool.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum message size in bytes.
uint32_t osMessageQueueGetMsgSize(osMessageQueueId_t mq_id)
{
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */;
    if ((RT_NULL == mq_cb) || (rt_object_get_type(&mq_cb->mq.parent.parent) != RT_Object_Class_MessageQueue))
    {
        return 0U;
    }

    return mq_cb->mq.msg_size;
}

/// Get number of queued messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of queued messages.
uint32_t osMessageQueueGetCount(osMessageQueueId_t mq_id)
{
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */;
    if ((RT_NULL == mq_cb) || (rt_object_get_type(&mq_cb->mq.parent.parent) != RT_Object_Class_MessageQueue))
    {
        return 0U;
    }

    return mq_cb->mq.entry;
}

/// Get number of available slots for messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of available slots for messages.
uint32_t osMessageQueueGetSpace(osMessageQueueId_t mq_id)
{
    mq_cb_t *mq_cb  = (mq_cb_t *)mq_id;

    /* Check parameters */;
    if ((RT_NULL == mq_cb) || (rt_object_get_type(&mq_cb->mq.parent.parent) != RT_Object_Class_MessageQueue))
    {
        return 0U;
    }

    return (mq_cb->mq.max_msgs - mq_cb->mq.entry);
}

/// Reset a Message Queue to initial empty state.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueReset(osMessageQueueId_t mq_id)
{
    rt_err_t result;
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */;
    if (RT_NULL == mq_cb)
    {
        return osErrorParameter;
    }

    result = rt_mq_control(&(mq_cb->mq), RT_IPC_CMD_RESET, RT_NULL);

    if (RT_EOK == result)
        return osOK;
    else
        return osError;
}

/// Delete a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueDelete(osMessageQueueId_t mq_id)
{
    mq_cb_t *mq_cb = (mq_cb_t *)mq_id;

    /* Check parameters */;
    if (RT_NULL == mq_cb)
    {
        return osErrorParameter;
    }

    rt_mq_detach(&(mq_cb->mq));

    if (mq_cb->flags & MALLOC_CB)
        rt_free(mq_cb);

    if (mq_cb->flags & MALLOC_MEM)
        rt_free(mq_cb->mq.msg_pool);

    return osOK;
}

#endif
