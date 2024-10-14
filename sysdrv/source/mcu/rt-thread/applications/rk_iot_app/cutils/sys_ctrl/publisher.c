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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "observer.h"
#include "event_queue.h"
#include "publisher.h"

#define DBG_SECTION_NAME        "PUBLISHER"
#include "rkdebug.h"

#define PUBLISHER_THREAD_STACKSIZE (2 * 1024)

static void atomic_set(int *var, uint32_t cnt)
{
    // arch_irq_disable();
    rt_enter_critical();
    *var = cnt;
    // arch_irq_enable();
    rt_exit_critical();
}

static int __attach(struct publisher_base *base, observer_base *obs, int once)
{
    int ret = 0;
    observer_state attach_state = OBSERVER_ATTACHED;

    if (once)
        attach_state = OBSERVER_ATTACHED_ONCE;

    LOGD("%s: new observe event 0x%x", __func__, obs->event);

    /* TODO: entry critical section to sync list */
    // OS_RecursiveMutexLock(&base->lock, -1);
    rt_mutex_take(base->lock, RT_WAITING_FOREVER);

    if (obs->state == OBSERVER_DETACHED)
        obs->state = attach_state;
    else if (rt_list_isempty(&obs->node))
    {
        // arch_irq_disable();
        // list_add_tail(&obs->node, &base->head);
        rt_list_insert_after(&base->head, &obs->node);
        obs->state = attach_state;
        //rt_kprintf("%s(): rt_list_insert_after \n",__func__);
        // arch_irq_enable();
    }
    else
    {
        LOGD("new observe event: %u failed", obs->event);
        //rt_kprintf("%s(): new observe event:%u failed \n",__func__, obs->event);
        ret = -1;
    }

    // OS_RecursiveMutexUnlock(&base->lock);
    rt_mutex_release(base->lock);
    /* TODO: exit critical section */

    return ret;
}

static int attach_once(struct publisher_base *base, observer_base *obs)
{
    return __attach(base, obs, 1);
}

static int attach(struct publisher_base *base, observer_base *obs)
{
    return __attach(base, obs, 0);
}

static int detach(struct publisher_base *base, observer_base *obs)
{
    /* TODO: entry critical section to sync list or return failed */
    // OS_RecursiveMutexLock(&base->lock, -1); /* it can't call in interrupt, should be fixed */
    rt_mutex_take(base->lock, RT_WAITING_FOREVER);

    if (base->state == PUBLISHER_IDLE)
    {
        // list_del(&obs->node);
        rt_list_remove(&obs->node);
        obs->state = OBSERVER_ILDE;
    }
    else
        atomic_set(&obs->state, OBSERVER_DETACHED);

    // OS_RecursiveMutexUnlock(&base->lock);
    rt_mutex_release(base->lock);
    /* TODO: exit critical section */

    LOGD("remove observe event: %d", obs->event);

    return 0;
}

static int notify(struct publisher_base *base, uint32_t event, uint32_t arg)
{
    observer_base *obs = NULL;
    observer_base *itor = NULL;
    observer_base *safe = NULL;
    int cnt = 0;

    LOGD("%s() Enter ...", __func__);
    /* TODO: define some event to debug, for example, event -1 can be detect how many observer now. */

    // OS_RecursiveMutexLock(&base->lock, -1);
    rt_mutex_take(base->lock, RT_WAITING_FOREVER);
    atomic_set(&base->state, PUBLISHER_WORKING);

    /* trigger observers */
    rt_list_for_each_entry(itor, &base->head, node)
    {
        /* TODO: detect the observer if is locked by detach or sth else */
        //rt_kprintf("%s(): newEvent = %d, obsEvent = %d\n",__func__,event, itor->event);
        LOGD("state = %d ", itor->state);
        if (base->compare(event, itor->event) == 0
                && (itor->state == OBSERVER_ATTACHED || itor->state == OBSERVER_ATTACHED_ONCE))
        {
            obs = itor;
            uint32_t t0 = rt_tick_get();
            obs->trigger(obs, event, arg);
            uint32_t t1 = rt_tick_get();

            if (itor->state == OBSERVER_ATTACHED_ONCE)
                obs->state = OBSERVER_DETACHED;

#define OBSERVER_TRIGGER_OVERTIME 1000
            if ((t1 - t0 > OBSERVER_TRIGGER_OVERTIME) && /*(OS_TimeAfter(t1, t0))*/ (t0 - t1 < 0))
                LOGD("obs: %p callback run %d ms", obs, t1 - t0);

            cnt++;
        }
    }

    /* remove observers detached in trigger function */
    rt_list_for_each_entry_safe(itor, safe, &base->head, node)
    {
        if (itor->state == OBSERVER_DETACHED)
        {
            rt_list_remove(&itor->node);
            atomic_set(&itor->state, OBSERVER_ILDE);
        }
    }

    atomic_set(&base->state, PUBLISHER_IDLE);
    // OS_RecursiveMutexUnlock(&base->lock);
    rt_mutex_release(base->lock);

    if (obs == NULL)
    {
        LOGE("%s: %d: no observer eyes on this event\n", __func__, __LINE__);
        return 0;
    }

    LOGD("%s: Out,have %d Observers", __func__, cnt);
    return cnt;
}

/*
static void main_publisher(void *arg)
{
    publisher_base *base = (publisher_base *)arg;
    struct event_msg msg;
    int ret;

    while (1)
    {
        ret = base->queue->recv(base->queue, &msg, -1);
        if (ret != 0) {
            PUBLISHER_ERROR("queue error");
            continue;
        }

        ret = base->notify(base, msg.event, msg.data);
        PUBLISHER_DEBUG("event 0x%x notified %d observers", msg.event, ret);

        if (msg.execute)
            msg.execute(&msg);

        if (msg.destruct != NULL)
            msg.destruct(msg.data);
    }

}
*/

static void *publisher_main(struct event_msg *msg, void *arg)
{
    int ret;
    publisher_base *base = (publisher_base *)arg;
    (void)ret;

    ret = base->notify(base, msg->event, msg->data);
    LOGD("%s: %d: event 0x%x notified %d observers", __func__, __LINE__, msg->event, ret);
    return NULL;
}

/* TODO: add stack size */
publisher_base *publisher_create(struct event_queue *queue, int (*compare)(uint32_t newEvent, uint32_t obsEvent),
                                 rt_uint32_t prio, uint32_t stack)
{
    publisher_base *base = rt_malloc(sizeof(*base));

    if (base == NULL)
        return NULL;

    rt_memset(base, 0, sizeof(*base));

    base->lock = rt_mutex_create("Publisher", RT_IPC_FLAG_FIFO);

    //if (ret != OS_OK)
    if (base->lock == RT_NULL)
        goto failed;

    // INIT_LIST_HEAD(&base->head);
    rt_list_init(&base->head);
//  base->queue = queue;
    base->touch = attach_once;
    base->attach = attach;
    base->detach = detach;
    base->notify = notify;
    base->compare = compare;
    /*
        if (OS_ThreadCreate(&base->thd, "Publish", main_publisher,
                            base, prio, stack) != OS_OK)
        {
            PUBLISHER_ERROR("thread create error, maybe no RAM to create");
            goto failed;
        }
    */
    looper_factory *looper_ctor = looper_factory_create(queue);
    base->looper = looper_ctor->set_thread_param(looper_ctor, prio, stack)
                   ->set_extend_function(looper_ctor, publisher_main, base)
                   ->create_looper(looper_ctor);

    if (base->looper == NULL)
        goto failed;

    return base;

failed:

    if (base->lock)
        rt_mutex_delete(base->lock);

    if (base != NULL)
        rt_free(base);

    return NULL;
}

static int notice_all(uint32_t newEvent, uint32_t obsEvent)
{
    LOGD("%s() Enter ...", __func__);
    return 0;
}

static struct publisher_factory *set_compare(struct publisher_factory *ctor, int (*compare)(uint32_t newEvent, uint32_t obsEvent))
{
    ctor->publisher->compare = compare;
    return ctor;
}

static struct publisher_factory *set_thread_param(struct publisher_factory *ctor, rt_uint32_t prio, uint32_t stack)
{
    ctor->prio = prio;
    ctor->stack = stack;
    return ctor;
}

static struct publisher_factory *set_msg_size(struct publisher_factory *ctor, uint32_t size)
{
    ctor->size = size;
    return ctor;
}

static struct publisher_base *create_publisher(struct publisher_factory *ctor)
{
    publisher_base *publisher = ctor->publisher;
    rt_uint32_t prio = ctor->prio;
    uint32_t stack = ctor->stack;
    uint32_t size = ctor->size;
    struct event_queue *queue = ctor->queue;

    rt_free(ctor);

    looper_factory *looper_ctor = looper_factory_create(queue);
    publisher->looper = looper_ctor->set_thread_param(looper_ctor, prio, stack)
                        ->set_msg_size(looper_ctor, size)
                        ->set_extend_function(looper_ctor, publisher_main, publisher)
                        ->create_looper(looper_ctor);

    if (publisher->looper == NULL)
        goto failed;

    return publisher;

failed:
    rt_mutex_delete(publisher->lock);

    if (publisher != NULL)
        rt_free(publisher);

    return NULL;
}

struct publisher_factory *publisher_factory_create(struct event_queue *queue)
{
    publisher_base *base = rt_malloc(sizeof(*base));

    if (base == NULL)
        return NULL;

    rt_memset(base, 0, sizeof(*base));

    publisher_factory *ctor = rt_malloc(sizeof(*ctor));

    if (ctor == NULL)
        goto failed;

    rt_memset(ctor, 0, sizeof(*ctor));

    base->lock = rt_mutex_create("Publisher_facto", RT_IPC_FLAG_FIFO);

    if (base->lock == RT_NULL)
        goto failed;

    // INIT_LIST_HEAD(&base->head);
    rt_list_init(&base->head);

    base->touch = attach_once;
    base->attach = attach;
    base->detach = detach;
    base->notify = notify;
    base->compare = notice_all;

    ctor->publisher = base;
    ctor->prio = RT_THREAD_PRIORITY_MAX / 2;
    ctor->stack = 2 * 1024;
    ctor->size = sizeof(struct event_msg);
    ctor->set_compare = set_compare;
    ctor->set_thread_param = set_thread_param;
    ctor->set_msg_size = set_msg_size;
    ctor->create_publisher = create_publisher;
    ctor->queue = queue;

    return ctor;

failed:
    if (base->lock)
        rt_mutex_delete(base->lock);

    if (ctor != NULL)
        rt_free(ctor);

    if (base != NULL)
        rt_free(base);

    return NULL;
}

