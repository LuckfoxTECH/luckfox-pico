/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include "aupipe.h"

#ifdef RT_USING_AUPIPE_QUEUE

#include "drv_heap.h"

#undef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "QUEUE"

#define QUEUE(obj)                      ((ApQueue *)(obj))
#define QUEUE_QUIT                      (void *)0xFF00FF00

typedef struct _pad_info
{
    int buffer_num;
    ApList buf_pool;
}pad_info;

typedef struct queue_object
{
    ApObject base;
    ApPad *src_pad;
    ApPad *sink_pad;

    int channels;
    int buffer_num;

    int quit_flag;
    int suspend_flag;

    int default_map;
    int loop_running;

    rt_sem_t sem;
    rt_sem_t sync;
    rt_mutex_t mutex;
    rt_thread_t loop;

    pad_info *sink_info;
} ApQueue;

static int queue_process(ApPad *pad, ApBuffer *buffer);

static int queue_init(ApObject *obj, void *arg)
{
    ApQueue *queue = QUEUE(obj);

    int status = RT_EOK;
    char *parameters = (char *)arg;

    status |= aupipe_find_property(parameters, "channels",
                                   VALUE_TYPE_INT, &queue->channels);
    status |= aupipe_find_property(parameters, "buffer-num",
                                   VALUE_TYPE_INT, &queue->buffer_num);
    if (status != RT_EOK)
    {
        LOG_E("%s plug-in parameter initialization failed", queue->base.name);
        return -RT_ERROR;
    }

    queue->quit_flag = RT_FALSE;
    queue->suspend_flag = RT_FALSE;
    queue->default_map = (1 << queue->channels) - 1;

    queue->sink_info = rt_malloc(sizeof(pad_info) * queue->channels);

    queue->src_pad = aupipe_pad_new(obj, queue->channels);
    RT_ASSERT(queue->src_pad != NULL);

    queue->sink_pad = aupipe_pad_new(obj, queue->channels);
    RT_ASSERT(queue->sink_pad != NULL);
    for (int i = 0; i < queue->channels; i++)
    {
        queue->sink_info[i].buffer_num = 0;
        aupipe_list_init(&queue->sink_info[i].buf_pool);

        queue->sink_pad[i].process = queue_process;
    }

    queue->sem = rt_sem_create("queue", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(queue->sem != NULL);

    queue->sync = rt_sem_create("thsync", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(queue->sync != NULL);

    /* protect queue object resources */
    queue->mutex = rt_mutex_create("pkcodes", RT_IPC_FLAG_FIFO);
    RT_ASSERT(queue->mutex != NULL);

    return RT_EOK;
}

static int queue_deinit(ApObject *obj)
{
    ApQueue *queue = QUEUE(obj);

    rt_free(queue->src_pad);
    rt_free(queue->sink_pad);
    rt_free(queue->sink_info);

    rt_sem_delete(queue->sem);
    rt_sem_delete(queue->sync);
    rt_mutex_delete(queue->mutex);

    return RT_EOK;
}

static int queue_process(ApPad *pad, ApBuffer *buffer)
{
    ApQueue *queue = QUEUE(pad->parent);
    ApList *list;

    if (queue->quit_flag == RT_TRUE)
        return RT_EOK;

    rt_mutex_take(queue->mutex, RT_WAITING_FOREVER);
    if (queue->sink_info[pad->id].buffer_num >= queue->buffer_num)
    {
        rt_mutex_release(queue->mutex);

        queue->suspend_flag = RT_TRUE;

        rt_sem_take(queue->sync, RT_WAITING_FOREVER);
    }
    else
    {
        rt_mutex_release(queue->mutex);
    }

    list = rt_malloc(sizeof(ApList));
    if (!list)
    {
        aupipe_buffer_unref(buffer);
        return -RT_ERROR;
    }
    list->data = buffer;

    rt_mutex_take(queue->mutex, RT_WAITING_FOREVER);
    queue->sink_info[pad->id].buffer_num++;
    aupipe_list_insert_head(&queue->sink_info[pad->id].buf_pool, list);

    rt_mutex_release(queue->mutex);

    return RT_EOK;
}

static void do_push(void *arg)
{
    ApQueue *obj = QUEUE(arg);
    ApBuffer *buffer;
    ApList *list;
    int sink_map;

    sink_map = obj->default_map;

    while (obj->loop_running)
    {
        rt_mutex_take(obj->mutex, RT_WAITING_FOREVER);
        for (int i = 0; i < obj->channels; i++)
        {
            if (!aupipe_list_is_empty(&obj->sink_info[i].buf_pool))
            {
                sink_map = sink_map & (~(1 << i));
            }
        }
        rt_mutex_release(obj->mutex);

        if (sink_map != 0)
        {
            sink_map = obj->default_map;
            rt_thread_mdelay(1);
            continue;
        }
        sink_map = obj->default_map;

        for (int j = 0; j < obj->channels; j++)
        {
            rt_mutex_take(obj->mutex, RT_WAITING_FOREVER);
            list = aupipe_list_pop_tail(&obj->sink_info[j].buf_pool);
            obj->sink_info[j].buffer_num--;
            if (obj->sink_info[obj->channels -1].buffer_num < obj->buffer_num)
            {
                if (obj->suspend_flag == RT_TRUE)
                {
                    obj->suspend_flag = RT_FALSE;
                    rt_sem_release(obj->sync);
                }
            }
            rt_mutex_release(obj->mutex);

            buffer = list->data;
            rt_free(list);
            if (buffer == QUEUE_QUIT)
            {
                break;
            }

            if (obj->src_pad[j].peer == RT_NULL)
            {
                aupipe_buffer_unref(buffer);
                continue;
            }
            aupipe_pad_push(&obj->src_pad[j], buffer);
        }
    }
    rt_sem_release(obj->sem);
}

static void queue_clear_list(ApQueue *obj)
{
    ApList *list;
    ApBuffer *buffer;

    for (int i = 0; i < obj->channels; i++)
    {
        do
        {
            if (aupipe_list_is_empty(&obj->sink_info[i].buf_pool))
                break;
            list = aupipe_list_pop_tail(&obj->sink_info[i].buf_pool);
            buffer = (ApBuffer *)list->data;
            rt_free(list);
            if (buffer && buffer != QUEUE_QUIT)
                aupipe_buffer_unref(buffer);
        }
        while(1);
    }
}

static int queue_set_state(ApObject *obj, int state)
{
    ApQueue *queue = QUEUE(obj);
    switch (state)
    {
    case STATE_NULL_TO_READY:
        LOG_I("STATE_NULL_TO_READY");
        queue->loop = rt_thread_create("queue", do_push, queue,
                                       4096, RT_THREAD_PRIORITY_MAX / 2, 10);
        if (!queue->loop)
        {
            return -RT_ERROR;
        }
        queue->suspend_flag = RT_FALSE;
        break;
    case STATE_PAUSED_TO_PLAYING:
        LOG_I("STATE_PAUSED_TO_PLAYING");
        queue->loop_running = 1;
        rt_thread_startup(queue->loop);
        break;
    case STATE_PLAYING_TO_PAUSED:
        LOG_I("STATE_PLAYING_TO_PAUSED");
        if (queue->loop_running == 1)
        {
            /* loop still running */
            ApList *list;
            for (int i = 0; i < queue->channels; i++)
            {
                list = rt_malloc(sizeof(ApList));
                if (list)
                {
                    /* make sure do_push would not block
                    * in aupipe_list_is_empty
                    * */
                    list->data = QUEUE_QUIT;
                    aupipe_list_insert_head(&queue->sink_info[i].buf_pool, list);
                }
            }
            queue->loop_running = 0;
            queue->quit_flag = RT_TRUE;
            if (queue->suspend_flag == RT_TRUE)
            {
                rt_sem_release(queue->sync);
            }
            if (rt_sem_take(queue->sem, 3000))
            {
                /* Timeout, force delete */
                LOG_W("Timeout");
                rt_thread_delete(queue->loop);
            }
        }
        break;
    case STATE_READY_TO_NULL:
        LOG_I("STATE_READY_TO_NULL");
        queue_clear_list(queue);
        break;
    default:
        break;
    }

    return RT_EOK;
}

static int queue_set_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static int queue_get_property(ApObject *obj, char *name, void *arg)
{
    return RT_EOK;
}

static ApPad *queue_get_pad(ApObject *obj, int type, int id)
{
    ApQueue *queue = QUEUE(obj);

    if (type == PAD_TYPE_SINK)
        return &queue->sink_pad[id];

    return &queue->src_pad[id];
}

OBJECT_BASE_DEFINE(queue, ApQueue);

#endif

