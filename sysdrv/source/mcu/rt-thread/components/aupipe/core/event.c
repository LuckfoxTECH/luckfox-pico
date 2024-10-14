/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "PEVENT"

ApEvent *aupipe_event_new(void)
{
    ApEvent *event = rt_malloc(sizeof(ApEvent));
    if (!event)
        goto null_event;
    memset(event, 0, sizeof(ApEvent));

    event->lock = rt_mutex_create("ApEvent", RT_IPC_FLAG_FIFO);
    if (!event->lock)
        goto null_lock;
    event->sem = rt_sem_create("ApEvent", 0, RT_IPC_FLAG_FIFO);
    if (!event->sem)
        goto null_sem;

    aupipe_list_init(&event->msg);

    return event;

null_sem:
    rt_mutex_delete(event->lock);
null_lock:
    rt_free(event);
null_event:
    return NULL;
}

int aupipe_event_push(ApEvent *event, ApEventMsg *msg)
{
    ApList *list = rt_malloc(sizeof(ApList));
    if (!list)
        return -RT_ERROR;
    list->data = (void *)msg;

    rt_mutex_take(event->lock, RT_WAITING_FOREVER);
    aupipe_list_insert_head(&event->msg, list);
    if (event->wait_cnt)
    {
        rt_sem_release(event->sem);
        event->wait_cnt--;
    }
    rt_mutex_release(event->lock);

    return RT_EOK;
}

ApEventMsg *aupipe_event_pull(ApEvent *event)
{
    ApEventMsg *msg;
    ApList *list;

    rt_mutex_take(event->lock, RT_WAITING_FOREVER);
    if (aupipe_list_is_empty(&event->msg))
    {
        event->wait_cnt++;
        rt_mutex_release(event->lock);
        rt_sem_take(event->sem, RT_WAITING_FOREVER);
        rt_mutex_take(event->lock, RT_WAITING_FOREVER);
    }
    list = aupipe_list_pop_tail(&event->msg);
    msg = (ApEventMsg *)list->data;
    rt_free(list);
    rt_mutex_release(event->lock);

    return msg;
}

void aupipe_event_delete(ApEvent *event)
{
    ApEventMsg *msg;
    ApList *list;

    while (!aupipe_list_is_empty(&event->msg))
    {
        list = aupipe_list_pop_tail(&event->msg);
        msg = (ApEventMsg *)list->data;
        if (msg->need_free)
            rt_free(msg);
        rt_free(list);
    }

    rt_sem_delete(event->sem);
    rt_mutex_delete(event->lock);
    rt_free(event);
}

