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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "kernel/os/os.h"
#include "event_queue.h"
#include "looper.h"

#define DBG_SECTION_NAME        "LOOPER"
#include "rkdebug.h"

/*
#define LOOPER_DEBUG(fmt, arg...)   //rt_kprintf("[Looper debug] <%s : %d> " fmt "\n", __func__, __LINE__, ##arg)
#define LOOPER_ALERT(fmt, arg...)   //rt_kprintf("[Looper alert] <%s : %d> " fmt "\n", __func__, __LINE__, ##arg)
#define LOGE(fmt, arg...)   //rt_kprintf("[Looper error] <%s : %d> " fmt "\n", __func__, __LINE__, ##arg)
#define LOOPER_NOWAY()              //rt_kprintf("[Looper should not be here] <%s : %d> \n", __func__, __LINE__)
#define LOOPER_NOTSUPPORT()         LOOPER_ALERT("not support command")
*/

struct looper_base
{
    struct event_queue *queue;
    looper_extend extend;
    rt_thread_t thd;
    void *arg;
    void *msg;
};

static void main_looper(void *arg)
{
    looper_base *base = (looper_base *)arg;
    looper_extend extend;
    struct event_msg *msg = base->msg;
    int ret;
    uint32_t t0, t1;

    LOGD("%s() Enter ...", __func__);
    while (1)
    {
        ret = base->queue->recv(base->queue, msg, -1);
        LOGD("main_looper:msg.event = %d ", msg->event);
        if (ret != 0)
        {
            LOGE("queue error");
            continue;
        }

        if (msg->execute)
        {
            t0 = rt_tick_get();
            msg->execute(msg);
            t1 = rt_tick_get();
#define HANDLER_OVERTIME 1000
            if ((t1 - t0 > HANDLER_OVERTIME) && (t0 - t1 < 0))
                LOGW("handler: %p run %d ms", msg->execute, t1 - t0);
        }

        extend = base->extend;
        while (extend != NULL)
        {
            t0 = rt_tick_get();
            extend = (looper_extend)base->extend(msg, base->arg);
            t1 = rt_tick_get();

#define LOOPER_EXTEND_OVERTIME 5000
            if ((t1 - t0 > LOOPER_EXTEND_OVERTIME) && (t0 - t1 < 0))
                LOGW("looper extend function run %d ms", t1 - t0);
        }

        if (msg->destruct != NULL)
            msg->destruct(msg);

        rt_thread_delay(10);
    }
}

looper_base *looper_create(struct event_queue *queue)
{
    looper_base *base = rt_malloc(sizeof(*base));
    if (base == NULL)
        return NULL;
    rt_memset(base, 0, sizeof(*base));

    base->queue = queue;
    base->extend = NULL;
    base->arg = NULL;
    base->msg = rt_malloc(sizeof(struct event_msg));

    base->thd = rt_thread_create("Looper0",
                                 main_looper, base,
                                 2 * 1024, RT_THREAD_PRIORITY_MAX / 2, 10);
    if (base->thd != RT_NULL)
    {
        rt_thread_startup(base->thd);
    }
    else
    {
        goto failed;
    }

    return base;

failed:
    if (base != NULL)
        rt_free(base);
    return NULL;
}

struct looper_factory *set_thread_param(struct looper_factory *ctor, rt_uint32_t prio, uint32_t stack)
{
    ctor->prio = prio;
    ctor->stack = stack;
    return ctor;
}

struct looper_factory *set_msg_size(struct looper_factory *ctor, uint32_t size)
{
    if (ctor->looper->msg)
        rt_free(ctor->looper->msg);
    ctor->looper->msg = rt_malloc(size);
    return ctor;
}

struct looper_factory *set_extend_function(struct looper_factory *ctor, looper_extend extend, void *arg)
{
    ctor->looper->extend = extend;
    ctor->looper->arg = arg;
    return ctor;
}

looper_base *create_looper(struct looper_factory *ctor)
{
    looper_base *looper = ctor->looper;
    uint32_t prio = ctor->prio;
    uint32_t stack = ctor->stack;

    rt_free(ctor);

    if (looper->msg == NULL)
        looper->msg = rt_malloc(sizeof(struct event_msg));

    looper->thd = rt_thread_create("Looper1",
                                   main_looper,
                                   looper,
                                   stack,
                                   prio, 10);
    if (looper->thd == RT_NULL)
    {
        LOGE("thread create error, maybe no RAM to create");
        goto failed;
    }
    else
        rt_thread_startup(looper->thd);

    return looper;

failed:
    if (looper->msg)
        rt_free(looper->msg);
    if (looper != NULL)
        rt_free(looper);
    return NULL;
}

struct looper_factory *looper_factory_create(struct event_queue *queue)
{
    looper_base *base = rt_malloc(sizeof(*base));
    if (base == NULL)
        return NULL;
    rt_memset(base, 0, sizeof(*base));

    looper_factory *ctor = rt_malloc(sizeof(*ctor));
    if (ctor == NULL)
        goto failed;
    rt_memset(ctor, 0, sizeof(*ctor));

    base->queue = queue;
    base->extend = NULL;
    base->arg = NULL;
    base->msg = NULL;

    ctor->looper = base;
    ctor->prio = RT_THREAD_PRIORITY_MAX / 2;
    ctor->stack = 2 * 1024;
    ctor->set_thread_param = set_thread_param;
    ctor->set_msg_size = set_msg_size;
    ctor->set_extend_function = set_extend_function;
    ctor->create_looper = create_looper;

    return ctor;

failed:
    if (ctor != NULL)
        rt_free(base);
    if (base->msg != NULL)
        rt_free(base->msg);
    if (base != NULL)
        rt_free(base);
    return NULL;
}

