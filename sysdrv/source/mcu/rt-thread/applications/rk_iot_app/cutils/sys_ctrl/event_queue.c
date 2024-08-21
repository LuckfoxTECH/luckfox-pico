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
#include <rtdef.h>
#include <rtservice.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "container.h"
#include "event_queue.h"

#define DBG_SECTION_NAME        "EVT_QUE"
#include "rkdebug.h"


typedef struct prio_event_queue
{
    event_queue base;
    container_base *container;
    uint32_t msg_size;
} prio_event_queue;

static int complare_event_msg(uint32_t newArg, uint32_t oldArg)
{
    event_msg *newMsg = (event_msg *)newArg;
    event_msg *oldMsg = (event_msg *)oldArg;

    return newMsg->event < oldMsg->event;
}

static int prio_event_queue_deinit(struct event_queue *base)
{
    prio_event_queue *impl = __containerof(base, prio_event_queue, base);

    int ret = impl->container->deinit(impl->container);

    if (ret == 0)
        rt_free(impl);

    return ret;
}

static int prio_event_send(struct event_queue *base, struct event_msg *msg, uint32_t wait_ms)
{
    prio_event_queue *impl = __containerof(base, prio_event_queue, base);

//  EVTMSG_DEBUG("send event: 0x%x", msg->event);

    /* TODO: cache some to faster create msg and less fragmented memory */
    struct event_msg *newMsg = rt_malloc(impl->msg_size);

    if (newMsg == NULL)
        return -1;

    rt_memcpy(newMsg, msg, impl->msg_size);

    int ret = impl->container->push(impl->container, (uint32_t)newMsg, wait_ms);

    if (ret != 0)
    {
        rt_free(newMsg);
//      EVTMSG_ALERT("send event timeout");
        return -2;
    }

    return 0;
}

static int prio_event_recv(struct event_queue *base, struct event_msg *msg, uint32_t wait_ms)
{
    prio_event_queue *impl = __containerof(base, prio_event_queue, base);
    event_msg *newMsg;

    int ret = impl->container->pop(impl->container, (uint32_t *)&newMsg, wait_ms);

    if (ret != 0)
        return -2;

    rt_memcpy(msg, newMsg, impl->msg_size);
    rt_free(newMsg);

    LOGD("%s() : recv event: 0x%x", __func__, msg->event);

    return 0;
}

struct event_queue *prio_event_queue_create(uint32_t queue_len, uint32_t msg_size)
{
    prio_event_queue *impl = rt_malloc(sizeof(*impl));

    if (impl == NULL)
        return NULL;

    rt_memset(impl, 0, sizeof(*impl));

    impl->container = sorted_list_create(queue_len, complare_event_msg);

    if (impl->container == NULL)
        goto out;

    impl->base.send = prio_event_send;
    impl->base.recv = prio_event_recv;
    impl->base.deinit = prio_event_queue_deinit;
    impl->msg_size = msg_size;

    return &impl->base;

out:
    LOGE("sorted_list_create failed");
    rt_free(impl);
    return NULL;
}

typedef struct normal_event_queue
{
    event_queue base;
    rt_mq_t queue;
} normal_event_queue;

static int normal_event_queue_deinit(struct event_queue *base)
{
    normal_event_queue *impl = __containerof(base, normal_event_queue, base);
    rt_err_t ret;

#if CTRL_MSG_VALIDITY_CHECK

    if (impl->queue == RT_NULL)
    {
        LOGE("%s(), invalid queue %p", __func__, g_ctrl_msg_queue.handle)
        return 0;
    }

#endif

    ret = rt_mq_delete(impl->queue);

    if (ret != RT_EOK)
    {
        LOGE("%s() failed, err 0x%x", __func__, ret);
        return -1;
    }

    rt_free(impl);

    LOGE("%s()", __func__);
    return 0;
}

static int normal_event_send(struct event_queue *base, struct event_msg *msg, uint32_t wait_ms)
{
    normal_event_queue *impl = __containerof(base, normal_event_queue, base);
    rt_err_t ret;

    LOGD("%s():%d", __func__, __LINE__);
//    rt_kprintf("normal_event_send: %p, event = %d \n", msg,msg->event);

    LOGD("send event: 0x%x", msg->event);

#if CTRL_MSG_VALIDITY_CHECK

    /*if (!OS_QueueIsValid(&impl->queue)) {*/
    if (impl->queue == RT_NULL)
    {
//      EVTMSG_ALERT("%s(), invalid queue %p", __func__, &impl->queue);
        LOGE("%s(), invalid queue %p", __func__, &impl->queue);
        return -1;
    }

#endif

    //rt_kprintf("[dargon]%s():%d\n",__func__,__LINE__);
    ret = rt_mq_send(impl->queue, msg, sizeof(struct event_msg));

    if (ret != RT_EOK)
    {
        LOGE("%s() failed, err 0x%x", __func__, ret);
        return -1;
    }

    //rt_kprintf("[dargon]%s():%d\n",__func__,__LINE__);
    return 0;
}

static int normal_event_recv(struct event_queue *base, struct event_msg *msg, uint32_t wait_ms)
{
    normal_event_queue *impl = __containerof(base, normal_event_queue, base);
    rt_err_t ret;

#if CTRL_MSG_VALIDITY_CHECK

    /*if (!OS_QueueIsValid(&impl->queue)) {*/
    if (!impl->queue)
    {
//        EVTMSG_ALERT("%s(), invalid queue %p", __func__, &impl->queue);
        LOGD("%s(), invalid queue %p", __func__, &impl->queue);
        return -1;
    }

#endif
    //LOGD("%s(): Enter! ",__func__);
    ret = rt_mq_recv(impl->queue, msg, sizeof(struct event_msg), wait_ms);

    if (ret != RT_EOK)
    {
        LOGE("%s() failed, err 0x%x", __func__, ret);
        return -1;
    }

    LOGD("recv event: 0x%x", msg->event);

    return 0;
}

struct event_queue *normal_event_queue_create(uint32_t queue_len, uint32_t msg_size)
{
    normal_event_queue *impl = rt_malloc(sizeof(*impl));

    if (impl == NULL)
        return NULL;

    rt_memset(impl, 0, sizeof(*impl));

    impl->base.send = normal_event_send;
    impl->base.recv = normal_event_recv;
    impl->base.deinit = normal_event_queue_deinit;


#if CTRL_MSG_VALIDITY_CHECK

    if (impl->queue != RT_NULL)
    {
        LOGW("control message queue already inited");
        return -1;
    }

#endif

    impl->queue = rt_mq_create("event_queue", msg_size, queue_len, RT_IPC_FLAG_FIFO);

    if (impl->queue == RT_NULL)
    {
        LOGE("%s() failed!", __func__);
        goto out;
    }

    LOGD("%s()", __func__);
    return &impl->base;

out:

    if (impl != NULL)
        rt_free(impl);

    return NULL;
}
