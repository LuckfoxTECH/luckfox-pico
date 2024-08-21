/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <rtthread.h>
#include <rtservice.h>
#include <rtdef.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "observer.h"
#include "publisher.h"
#include "event_queue.h"
#include "sys_ctrl.h"
#include "rkdebug.h"

typedef struct sys_ctrl_msg
{
    event_msg msg;
    rt_sem_t  sem;
} sys_ctrl_msg;

static event_queue *g_sys_queue = NULL;
static publisher_base *g_sys_publisher = NULL;


static inline event_queue *get_sys_handler_queue()
{
    return g_sys_queue;
}

static int compare(uint32_t newEvent, uint32_t obsEvent)
{
    LOGD("sys_ctrl: %s() : Enter", __func__);

    if (!CMP_EVENT_TYPE(newEvent, obsEvent) && (EVENT_SUBTYPE(obsEvent) == ALL_SUBTYPE || newEvent == obsEvent))
        return 0;

    LOGD("cmp -1, newEvent: 0x%x, obsEvent: 0x%x", newEvent, obsEvent);

    return -1;
}

int sys_ctrl_create(void)
{
    uint32_t queue_len = 6;

    if (g_sys_queue == NULL)
#if SYS_CTRL_PRIO_QUEUE
        g_sys_queue = prio_event_queue_create(queue_len, sizeof(struct sys_ctrl_msg));
#else
        g_sys_queue = normal_event_queue_create(queue_len, sizeof(struct sys_ctrl_msg));
#endif
    if (g_sys_queue == NULL)
    {
        LOGE("create queue failed");
        return -1;
    }

    if (g_sys_publisher == NULL)
    {
        publisher_factory *ctor = publisher_factory_create(g_sys_queue);
        g_sys_publisher = ctor->set_thread_param(ctor, RT_THREAD_PRIORITY_MAX / 2 + 2, 2 * 1024)
                          ->set_compare(ctor, compare)
                          ->set_msg_size(ctor, sizeof(struct sys_ctrl_msg))
                          ->create_publisher(ctor);
    }
    if (g_sys_publisher == NULL)
    {
        LOGE("create publisher failed");
        return -1;
    }

    return 0;
}

int sys_ctrl_touch(observer_base *obs)
{
    if (g_sys_publisher == NULL)
        return -1;

    return g_sys_publisher->touch(g_sys_publisher, obs);
}

int sys_ctrl_attach(observer_base *obs)
{
    if (g_sys_publisher == NULL)
        return -1;

    return g_sys_publisher->attach(g_sys_publisher, obs);
}

int sys_ctrl_detach(observer_base *obs)
{
    if (g_sys_publisher == NULL)
        return -1;

    return g_sys_publisher->detach(g_sys_publisher, obs);
}

static int event_send(event_queue *queue, uint16_t type, uint16_t subtype, uint32_t data, void (*destruct)(event_msg *), uint32_t wait_ms)
{
    LOGD("%s():%d", __func__, __LINE__);
    struct sys_ctrl_msg msg = {{RT_NULL, destruct, MK_EVENT(type, subtype), data}, RT_NULL};
    LOGD("event_send:event = %d ", msg.msg.event);

    return queue->send(queue, &msg.msg, wait_ms);
}


static void freeMsgData(event_msg *msg)
{
    free((void *)msg->data);
}

int sys_event_send(uint16_t type, uint16_t subtype, uint32_t data, uint32_t wait_ms)
{
    if (g_sys_queue == RT_NULL)
        return -1;

    LOGD("sys_event_send: type = %d, subtype = %d, data = %d", type, subtype, data);
    return event_send(g_sys_queue, type, subtype, data, RT_NULL, wait_ms);
}

int sys_event_send_with_free(uint16_t type, uint16_t subtype, void *data, uint32_t wait_ms)
{
    if (g_sys_queue == NULL)
        return -1;

    return event_send(g_sys_queue, type, subtype, (uint32_t)data, freeMsgData, wait_ms);
}

int sys_event_send_with_destruct(uint16_t type, uint16_t subtype, void *data, void (*destruct)(event_msg *), uint32_t wait_ms)
{
    if (g_sys_queue == NULL)
        return -1;

    return event_send(g_sys_queue, type, subtype, (uint32_t)data, destruct, wait_ms);
}

int sys_event_handler_send(uint16_t type, uint16_t subtype, uint32_t data, void (*exec)(event_msg *), uint32_t wait_ms)
{
    event_queue *queue = get_sys_handler_queue();
    if (queue == NULL)
        return -1;

    struct sys_ctrl_msg msg = {{exec, NULL, MK_EVENT(type, subtype), data}, NULL};
    return queue->send(queue, &msg.msg, wait_ms);
}

void sys_handler_finish(event_msg *msg)
{
    rt_sem_release((rt_sem_t)msg->extra[0]);
}

int sys_event_handler_send_wait_finish(uint16_t type, uint16_t subtype, uint32_t data, void (*exec)(event_msg *), uint32_t wait_ms)
{
    event_queue *queue = get_sys_handler_queue();
    if (queue == NULL)
        return -1;

    int ret;
    rt_sem_t sem;
    rt_memset(&sem, 0, sizeof(sem));
    sem = rt_sem_create("sys_hdl", 0, RT_IPC_FLAG_FIFO);

    struct sys_ctrl_msg msg = {{exec, sys_handler_finish, MK_EVENT(type, subtype), data}, sem};
    ret = queue->send(queue, &msg.msg, wait_ms);
    if (ret != 0)
        goto out;

    rt_sem_take(sem, RT_WAITING_FOREVER);
out:
    rt_sem_delete(sem);
    return ret;
}

int sys_handler_send(void (*exec)(event_msg *), uint32_t data, uint32_t wait_ms)
{
    return sys_event_handler_send(CTRL_MSG_TYPE_HANDLER, ALL_SUBTYPE, data, exec, wait_ms);
}

int sys_handler_send_wait_finish(void (*exec)(event_msg *), uint32_t data, uint32_t wait_ms)
{
    return sys_event_handler_send_wait_finish(CTRL_MSG_TYPE_HANDLER, ALL_SUBTYPE, data, exec, wait_ms);
}
