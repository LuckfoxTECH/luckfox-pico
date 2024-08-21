/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-04-12     misonyo      the first version
 */

#ifndef __RTTHREAD_OS2_H__
#define __RTTHREAD_OS2_H__

#include <rtthread.h>
#include <cmsis_os2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    rt_uint8_t flags;
    struct rt_thread thread;
    rt_sem_t joinable_sem;    ///< semaphore for joinable thread
    rt_uint8_t flag_set;      ///< thread flag set
    rt_uint8_t prio;
}thread_cb_t;

typedef struct
{
    rt_uint8_t flags;
    struct rt_timer timer;
}timer_cb_t;

#ifdef RT_USING_EVENT
typedef struct
{
    rt_uint8_t flags;
    struct rt_event event;
}event_cb_t;
#endif

#ifdef RT_USING_MUTEX
typedef struct
{
    rt_uint8_t flags;
    struct rt_mutex mutex;
}mutex_cb_t;
#endif

#ifdef RT_USING_SEMAPHORE
typedef struct
{
    rt_uint8_t flags;
    struct rt_semaphore sem;
}sem_cb_t;
#endif

#ifdef RT_USING_MEMPOOL
typedef struct
{
    rt_uint8_t flags;
    struct rt_mempool mp;
}mempool_cb_t;
#endif

#ifdef RT_USING_MESSAGEQUEUE
typedef struct
{
    rt_uint32_t init_msg_size;
    rt_uint8_t flags;
    struct rt_messagequeue mq;
}mq_cb_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
