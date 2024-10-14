/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "wiced_rtos.h"
#include "wiced_defaults.h"
#include "RTOS/wwd_rtos_interface.h"
#include "wiced_time.h"
#include "wiced_platform.h"
#include "rthw.h"
#include <rtthread.h>

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#ifndef APPLICATION_STACK_SIZE
#define APPLICATION_STACK_SIZE WICED_DEFAULT_APPLICATION_STACK_SIZE
#endif

#ifndef TIMER_THREAD_STACK_SIZE
#define TIMER_THREAD_STACK_SIZE      1024
#endif
#define TIMER_QUEUE_LENGTH  5

#define SYSTEM_MONITOR_THREAD_STACK_SIZE   512


/* FreeRTOS Event Group definition is based on
 *  configUSE_16_BIT_TICKS
 *
 * See <FreeRTOS>/Source/event_roups.c for detail.
 */
#if configUSE_16_BIT_TICKS == 1
#define eventEVENT_BITS_MASK    0xffU           /* 8-bit */
#else
#define eventEVENT_BITS_MASK    0xffffffUL      /* 24-bit */
#endif


/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

//typedef TimerCallbackFunction_t native_timer_handler_t;
//typedef TaskFunction_t          native_thread_t;

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct
{
    event_handler_t function;
    void           *arg;
} timer_queue_message_t;

typedef struct
{
    event_handler_t function;
    void           *arg;
} wiced_event_message_t;

#define ALTERNATE_MAIN
/******************************************************
 *               Static Function Declarations
 ******************************************************/
#ifndef ALTERNATE_MAIN
static void application_thread_main(void *arg);
#endif

/******************************************************
 *               Variable Definitions
 ******************************************************/

extern const uint32_t ms_to_tick_ratio;
wiced_worker_thread_t wiced_hardware_io_worker_thread;
wiced_worker_thread_t wiced_networking_worker_thread;

//static TaskHandle_t  app_thread_handle;
#ifndef WICED_DISABLE_WATCHDOG
//static TaskHandle_t  system_monitor_thread_handle;
#endif /* WICED_DISABLE_WATCHDOG */
//static wiced_time_t wiced_time_offset = 0;

/******************************************************
 *               Function Definitions
 ******************************************************/

/**
 *  Main function - starts ThreadX
 *  Called from the crt0 _start function
 *
 */
#ifndef ALTERNATE_MAIN
int main(void)
{

#if defined ( __IAR_SYSTEMS_ICC__ )
    /* IAR allows init functions in __low_level_init(), but it is run before global
     * variables have been initialised, so the following init still needs to be done
     * When using GCC, this is done in crt0_GCC.c
     */
    platform_init_mcu_infrastructure();
    platform_init_external_devices();
#endif /* #elif defined ( __IAR_SYSTEMS_ICC__ ) */

#ifndef WICED_DISABLE_WATCHDOG
    /* Start the watchdog kicking thread */
    xTaskCreate((TaskFunction_t) system_monitor_thread_main, "system monitor", SYSTEM_MONITOR_THREAD_STACK_SIZE / sizeof(portSTACK_TYPE), NULL, RTOS_HIGHEST_PRIORITY, &system_monitor_thread_handle);
#endif /* WICED_DISABLE_WATCHDOG */
    /* Create an initial thread */
    xTaskCreate(application_thread_main, "app_thread", APPLICATION_STACK_SIZE / sizeof(portSTACK_TYPE), NULL, WICED_PRIORITY_TO_NATIVE_PRIORITY(WICED_APPLICATION_PRIORITY), &app_thread_handle);

    /* Start the FreeRTOS scheduler - this call should never return */
    vTaskStartScheduler();

    /* Should never get here, unless there is an error in vTaskStartScheduler */
    return 0;
}

static void application_thread_main(void *arg)
{
    UNUSED_PARAMETER(arg);
    application_start();

    malloc_leak_check(NULL, LEAK_CHECK_THREAD);
    vTaskDelete(NULL);
}
#endif /* ifndef ALTERNATE_MAIN */

typedef void (*rt_task_cb_function)(void *parameter);

wiced_result_t wiced_rtos_create_thread(wiced_thread_t *thread, uint8_t priority, const char *name, wiced_thread_function_t function, uint32_t stack_size, void *arg)
{
    rt_kprintf("\nwiced_rtos_create_thread: %s pri: %d stack_size: 0x%x\n",
               name, priority, stack_size);

    /* Limit priority to default lib priority
    if ( priority > RTOS_HIGHEST_PRIORITY )
    {
        priority = RTOS_HIGHEST_PRIORITY;
    } */

    //priority = RTOS_HIGHEST_PRIORITY - priority - 15;
    //priority = 1;

    //return host_rtos_create_thread_with_arg( WICED_GET_THREAD_HANDLE( thread ), function, name, NULL, stack_size, WICED_PRIORITY_TO_NATIVE_PRIORITY( priority ), (uint32_t) arg );
    *thread = rt_thread_create(name,
                               (rt_task_cb_function)function,
                               arg,
                               stack_size,
                               WICED_PRIORITY_TO_NATIVE_PRIORITY(priority),
                               5);

    RT_ASSERT(*thread != RT_NULL);
    rt_thread_startup(*thread);
    return WICED_WWD_SUCCESS;
}

#if 0
wiced_result_t wiced_rtos_create_thread_with_stack(wiced_thread_t *thread, uint8_t priority, const char *name, wiced_thread_function_t function, void *stack, uint32_t stack_size, void *arg)
{
    /* Limit priority to default lib priority */
    if (priority > RTOS_HIGHEST_PRIORITY)
    {
        priority = RTOS_HIGHEST_PRIORITY;
    }

    //return  host_rtos_create_thread_with_arg( WICED_GET_THREAD_HANDLE( thread ), function, name, stack, stack_size, WICED_PRIORITY_TO_NATIVE_PRIORITY( priority ), (uint32_t)arg );
    RT_ASSERT(RT_EOK == rt_thread_init(struct rt_thread * thread,
                                       name,
                                       function, arg,
                                       stack, stack_size,
                                       WICED_PRIORITY_TO_NATIVE_PRIORITY(priority), 5));
    return WICED_WWD_SUCCESS;
}
#endif

wiced_result_t wiced_rtos_delete_thread(wiced_thread_t *thread)
{
    RT_ASSERT(rt_thread_delete(*thread) == RT_EOK);
    *thread = RT_NULL;
    return WICED_WWD_SUCCESS;
}


wiced_result_t wiced_rtos_is_current_thread(wiced_thread_t *thread)
{
    if (rt_thread_self() == *thread)
    {
        return WICED_SUCCESS;
    }
    else
    {
        return WICED_ERROR;
    }
}

wiced_result_t wiced_rtos_check_stack(void)
{
    /* TODO: Add stack checking here. */

    return WICED_SUCCESS;
}
#if 0
wiced_result_t wiced_rtos_thread_force_awake(wiced_thread_t *thread)
{
    xTaskAbortDelay(*thread);
    return WICED_SUCCESS;
}


wiced_result_t wiced_time_get_time(wiced_time_t *time_ptr)
{
    *time_ptr = (wiced_time_t)(xTaskGetTickCount() * ms_to_tick_ratio) + wiced_time_offset;
    return WICED_SUCCESS;
}

wiced_result_t wiced_time_set_time(const wiced_time_t *time_ptr)
{
    wiced_time_offset = *time_ptr - (wiced_time_t)(xTaskGetTickCount() * ms_to_tick_ratio);
    return WICED_SUCCESS;
}
#endif

wiced_result_t wiced_rtos_init_mutex(wiced_mutex_t *mutex)
{
    RT_ASSERT(mutex != NULL);

    /* Mutex uses priority inheritance */
    *mutex = rt_mutex_create("", RT_IPC_FLAG_FIFO);
    RT_ASSERT(*mutex != NULL);

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_lock_mutex(wiced_mutex_t *mutex)
{
    RT_ASSERT((mutex != RT_NULL) && (*mutex != RT_NULL));

    RT_ASSERT(rt_mutex_take(*mutex, WICED_WAIT_FOREVER) == RT_EOK);

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_unlock_mutex(wiced_mutex_t *mutex)
{
    RT_ASSERT((mutex != RT_NULL) && (*mutex != RT_NULL));

    RT_ASSERT(rt_mutex_release(*mutex) == RT_EOK);

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_deinit_mutex(wiced_mutex_t *mutex)
{
    RT_ASSERT((mutex != RT_NULL) && (*mutex != RT_NULL));

    RT_ASSERT(rt_mutex_delete(*mutex) == RT_EOK);
    *mutex = RT_NULL;
    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_init_queue(wiced_queue_t *queue, const char *name, uint32_t message_size, uint32_t number_of_messages)
{
    *queue = rt_mq_create(name, message_size, number_of_messages, RT_IPC_FLAG_FIFO);
    RT_ASSERT(*queue != RT_NULL);
    return WICED_SUCCESS;
}


#if 0 /* Not yet implemented by other RTOSs */
wiced_result_t wiced_rtos_push_to_queue_front(wiced_queue_t *queue, void *message, uint32_t timeout_ms)
{
    if (xQueueSendToFront(*queue, message, (portTickType)(timeout_ms / ms_to_tick_ratio)) != pdPASS)
    {
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}
#endif

wiced_result_t wiced_rtos_get_queue_occupancy(wiced_queue_t *queue, uint32_t *count)
{
    UNUSED_PARAMETER(queue);
    UNUSED_PARAMETER(count);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_rtos_deinit_queue(wiced_queue_t *queue)
{
    RT_ASSERT(RT_EOK == rt_mq_delete(*queue));
    *queue = RT_NULL;
    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_is_queue_empty(wiced_queue_t *queue)
{
    int cnt;

    rt_enter_critical();
    cnt = (*queue)->entry & 0x0ffff;
    rt_exit_critical();

    return (cnt == 0) ? WICED_SUCCESS : WICED_ERROR;
}

wiced_result_t wiced_rtos_is_queue_full(wiced_queue_t *queue)
{
    bool b;

    rt_enter_critical();
    b = ((*queue)->entry == (*queue)->max_msgs);
    rt_exit_critical();

    return (b) ? WICED_SUCCESS : WICED_ERROR;
}

wiced_result_t wiced_rtos_push_to_queue(wiced_queue_t *queue, void *message, uint32_t timeout_ms)
{
    rt_err_t ret = rt_mq_send((*queue), message, (*queue)->msg_size);
    if (ret != RT_EOK) rt_kprintf(" ERROR: RET: %d [0x%08x]\n", ret, *queue);
    RT_ASSERT(ret == RT_EOK);
    return (ret == RT_EOK) ? WICED_SUCCESS : WICED_ERROR;
}

wiced_result_t wiced_rtos_pop_from_queue(wiced_queue_t *queue, void *message, uint32_t timeout_ms)
{
    rt_err_t ret = rt_mq_recv(*queue, message, (*queue)->msg_size, timeout_ms);
    //RT_ASSERT( ret== RT_EOK);
    return (ret == RT_EOK) ? WICED_SUCCESS : WICED_ERROR;
}

static void timer_callback(void *handle)
{
    wiced_timer_t *timer = (wiced_timer_t *)(handle);

    if (timer->function)
    {
        timer->function(timer->arg);
    }
}

wiced_result_t wiced_rtos_init_timer(wiced_timer_t *timer, uint32_t time_ms, timer_handler_t function, void *arg)
{
    RT_ASSERT(timer != NULL);

    timer->function = function;
    timer->arg      = arg;

    //timer->handle = xTimerCreate(  (const char *)"", (TickType_t)( time_ms / ms_to_tick_ratio ), pdTRUE, timer, (native_timer_handler_t) timer_callback );
    timer->handle = rt_timer_create("",
                                    timer_callback,
                                    timer,
                                    time_ms,
                                    RT_TIMER_FLAG_PERIODIC);

    RT_ASSERT(timer->handle != RT_NULL);

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_start_timer(wiced_timer_t *timer)
{
    RT_ASSERT(timer->handle != NULL);

    RT_ASSERT(RT_EOK == rt_timer_start(timer->handle));

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_stop_timer(wiced_timer_t *timer)
{
    RT_ASSERT(timer->handle != NULL);

    if (timer->handle->parent.flag & RT_TIMER_FLAG_ACTIVATED)
    {
        RT_ASSERT(RT_EOK == rt_timer_stop(timer->handle));
    }
    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_deinit_timer(wiced_timer_t *timer)
{
    RT_ASSERT(timer->handle != NULL);

    RT_ASSERT(RT_EOK == rt_timer_delete(timer->handle));
    timer->handle = RT_NULL;

    return WICED_SUCCESS;
}

#if 0
wiced_result_t wiced_rtos_is_timer_running(wiced_timer_t *timer)
{
    if (timer->handle == NULL)
    {
        return WICED_ERROR;
    }

    return (xTimerIsTimerActive(timer->handle) != 0) ? WICED_SUCCESS : WICED_ERROR;
}
#endif

/* -------------------- Event Groups -------------------- */

wiced_result_t wiced_rtos_init_event_flags(wiced_event_flags_t *event_flags)
{
    *event_flags = rt_event_create("", RT_IPC_FLAG_FIFO);
    RT_ASSERT(*event_flags != RT_NULL);
    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_wait_for_event_flags(wiced_event_flags_t *event_flags, uint32_t flags_to_wait_for, uint32_t *flags_set, wiced_bool_t clear_set_flags, wiced_event_flags_wait_option_t wait_option, uint32_t timeout_ms)
{
    rt_uint8_t option = RT_EVENT_FLAG_OR;
    rt_err_t err;
    if (WAIT_FOR_ALL_EVENTS == wait_option)
    {
        option = RT_EVENT_FLAG_AND;;
    }

    if (clear_set_flags)
    {
        option |= RT_EVENT_FLAG_CLEAR;
    }

    err = rt_event_recv(*event_flags,
                        flags_to_wait_for,
                        option,
                        timeout_ms,
                        (rt_uint32_t *)flags_set);
    RT_ASSERT((err == RT_EOK) || (err == -RT_ETIMEOUT));

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_set_event_flags(wiced_event_flags_t *event_flags, uint32_t flags_to_set)
{
    RT_ASSERT(*event_flags != NULL);
    RT_ASSERT(RT_EOK == rt_event_send(*event_flags, flags_to_set));

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_deinit_event_flags(wiced_event_flags_t *event_flags)
{
    RT_ASSERT(*event_flags != NULL);

    RT_ASSERT(RT_EOK == rt_event_delete(*event_flags));    /* No return value */

    *event_flags = NULL;

    return WICED_SUCCESS;
}

/*
 * semphore operations
*/
wiced_result_t wiced_rtos_init_semaphore(wiced_semaphore_t *semaphore)
{
    *semaphore = rt_sem_create("", 1, RT_IPC_FLAG_FIFO);
    RT_ASSERT(RT_NULL != *semaphore);
    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_set_semaphore(wiced_semaphore_t *semaphore)
{
    RT_ASSERT(RT_EOK == rt_sem_release(*semaphore));
    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_get_semaphore(wiced_semaphore_t *semaphore, uint32_t timeout_ms)
{
    RT_ASSERT(RT_EOK == rt_sem_take(*semaphore, timeout_ms));
    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_deinit_semaphore(wiced_semaphore_t *semaphore)
{
    RT_ASSERT(RT_EOK == rt_sem_delete(*semaphore));
    return WICED_SUCCESS;
}
/*
 * timer operations
 */
wiced_result_t wiced_rtos_delay_microseconds(uint32_t microseconds)
{
    rt_thread_mdelay(microseconds);
    return WICED_SUCCESS;
}

// this function need to be implemented / rewritten
wiced_result_t wiced_rtos_delay_milliseconds(uint32_t us)
{
    uint32_t COUNT = us * 1000;
    uint32_t i = 0;

    while (i++ < COUNT);
    return WICED_SUCCESS;
}

wiced_result_t command_console_init(wiced_uart_t uart, uint32_t line_len, char *buffer, uint32_t history_len, char *history_buffer_ptr, const char *delimiter_string)
{
    return WICED_SUCCESS;
}

wiced_result_t wiced_core_init(void)
{
    return WICED_SUCCESS;
}

#if 0
#define STATIC_LOG_BUFFER
void LogMsg(uint32_t trace_set_mask, const char *fmt_str, ...)
{
#ifdef STATIC_LOG_BUFFER
    static char buffer[2048];
#else
    static char *buffer;  // Save stack space - make global
    buffer = rt_malloc(256);
    if (buffer == NULL)
    {
        RT_ASSERT(0);
        return ;
    }
#endif
    va_list ap;

    va_start(ap, fmt_str);
    vsprintf(buffer, fmt_str, ap);
    va_end(ap);
    rt_kprintf("%s\n", buffer);
#ifndef STATIC_LOG_BUFFER
    rt_free(buffer);
#endif
}
#endif

wiced_result_t wiced_rtos_is_timer_running(wiced_timer_t *timer)
{
    if (timer->handle == NULL)
    {
        return WICED_ERROR;
    }


    return WICED_SUCCESS;
}

void wiced_rtos_suspend_thread(wiced_thread_t tsk)
{
    if (tsk == 0)
    {
        tsk = rt_thread_self();
    }
    rt_thread_suspend(tsk);
}

void wiced_rtos_resume_thread(wiced_thread_t tsk)
{
    rt_thread_resume(tsk);
}

void wiced_assert(const char *msg, bool b)
{
    if (!b)
    {
        rt_kprintf(msg);
        RT_ASSERT(0);
    }
}

/*
 * enable / disable interrupt.
 */
static rt_base_t int_level;
void  WICED_DISABLE_INTERRUPTS()
{
    int_level = rt_hw_interrupt_disable();
}

void WICED_ENABLE_INTERRUPTS()
{
    rt_hw_interrupt_enable(int_level);
}
