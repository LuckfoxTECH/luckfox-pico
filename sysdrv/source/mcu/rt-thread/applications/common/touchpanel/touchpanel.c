/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#include "touch.h"
#include "touchpanel.h"

//#define RT_TOUCHPANEL_DEBUG
#ifdef RT_TOUCHPANEL_DEBUG
#define touchpanel_dbg(...) \
    do { \
        rt_kprintf("%s: ", __FUNCTION__);         \
        rt_kprintf(__VA_ARGS__);         \
    } while(0)
#else
#define touchpanel_dbg(...)
#endif

static  struct rt_touchpanel_data *g_tp_data = RT_NULL;

rt_err_t rt_touchpanel_block_register(struct rt_touchpanel_block *block)
{
    struct rt_touchpanel_data  *touch_data = g_tp_data;
    rt_slist_t  *pos, *head = &touch_data->b_list;

    rt_mutex_take(touch_data->mutex, RT_WAITING_FOREVER);

    /* check if node already registered */
    rt_slist_for_each(pos, head)
    {
        if (pos == &block->slist)
        {
            rt_kprintf("rt_touchpanel_block_register fail, block(0x%x) already registered!\n", block);

            rt_mutex_release(touch_data->mutex);

            return RT_ERROR;
        }
    }

    /* register node */

    rt_slist_append(head, &block->slist);

    rt_mutex_release(touch_data->mutex);

    return RT_EOK;
}

rt_err_t rt_touchpanel_block_unregister(struct rt_touchpanel_block *block)
{
    struct rt_touchpanel_data *touch_data = g_tp_data;
    rt_slist_t *pos, *head = &touch_data->b_list;

    rt_mutex_take(touch_data->mutex, RT_WAITING_FOREVER);

    /* check if node already registered */
    rt_slist_for_each(pos, head)
    {
        if (pos == &block->slist)
        {

            rt_slist_remove(head, pos);

            rt_mutex_release(touch_data->mutex);

            return RT_EOK;
        }
    }

    rt_kprintf("rt_touchpanel_block_unregister fail, block(0x%x) can nod find!\n", block);

    rt_mutex_release(touch_data->mutex);

    return RT_ERROR;
}

rt_err_t rt_touchpoint_is_in_block(struct rt_touch_data *p, struct rt_touchpanel_block *block)
{
    if (((block->x <= p->x_coordinate) && (p->x_coordinate <= block->x + block->w)) &&
            ((block->y <= p->y_coordinate) && (p->y_coordinate <= block->y + block->h)))
    {
        return RT_EOK;
    }

    return RT_ERROR;
}

rt_err_t rt_touchpoint_is_valid(struct rt_touch_data *p, struct rt_touchpanel_block *block)
{
    rt_err_t ret = RT_ERROR;

    if (p->event == RT_TOUCH_EVENT_DOWN)
    {
        if (RT_EOK == rt_touchpoint_is_in_block(p, block))
        {
            touchpanel_dbg("touch [%s] down.\n", block->name);
            block->event = RT_TOUCH_EVENT_DOWN;
            block->timestamp = p->timestamp;
            ret = RT_EOK;
        }
    }
    else if (p->event == RT_TOUCH_EVENT_MOVE)
    {
        if (RT_EOK == rt_touchpoint_is_in_block(p, block))
        {
            if ((block->event == RT_TOUCH_EVENT_DOWN) ||
                    (block->event == RT_TOUCH_EVENT_MOVE))
            {
                touchpanel_dbg("touch [%s] move.\n", block->name);
                block->event = RT_TOUCH_EVENT_MOVE;
                ret = RT_EOK;
            }
        }
        else
        {
            if ((block->event == RT_TOUCH_EVENT_DOWN) ||
                    (block->event == RT_TOUCH_EVENT_MOVE))
            {
                touchpanel_dbg("touch [%s] outside.\n", block->name);
                if (block->attribute & TOUCHPANEL_BLOCK_MOVE_ENABLE)
                {
                    block->event = RT_TOUCH_EVENT_MOVE;
                }
                else
                {
                    block->event = RT_TOUCH_EVENT_UP;
                }
                ret = RT_EOK;
            }
        }
    }
    else if (p->event == RT_TOUCH_EVENT_UP)
    {
        if ((block->event == RT_TOUCH_EVENT_DOWN) ||
                (block->event == RT_TOUCH_EVENT_MOVE))
        {
            touchpanel_dbg("touch [%s] up.\n", block->name);
            ret = RT_EOK;
        }
        block->event = RT_TOUCH_EVENT_UP;
    }

    return ret;
}

static void rt_touchpanel_blocks_handle(struct rt_touch_data *point, rt_uint8_t num)
{
    struct rt_touchpanel_block *block;
    struct rt_touchpanel_data  *touch_data = g_tp_data;
    rt_slist_t *pos, *head = &touch_data->b_list;

    rt_mutex_take(touch_data->mutex, RT_WAITING_FOREVER);

    rt_slist_for_each(pos, head)
    {
        block = rt_slist_entry(pos, struct rt_touchpanel_block, slist);
        if (block->callback)
        {
            if (RT_EOK == block->callback(point, num))
            {
                rt_mutex_release(touch_data->mutex);
                return;
            }
        }
    }

    rt_mutex_release(touch_data->mutex);
}

static rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(g_tp_data->sem);
    return 0;
}

static void rt_touchpanel_thread(void *parameter)
{
    rt_err_t ret;
    struct rt_touchpanel_data *touchpanel;

    rt_device_t dev = rt_device_find(TOUCH_DEV_NAME);
    if (dev == RT_NULL)
    {
        rt_kprintf("%s touch device can not find.\n", TOUCH_DEV_NAME);
        return;
    }

    ret = rt_device_open(dev, RT_DEVICE_FLAG_INT_RX);
    if (ret != RT_EOK)
    {
        rt_kprintf("%s: touch device open fail.\n", TOUCH_DEV_NAME);
        return;
    }

    g_tp_data = touchpanel = (struct rt_touchpanel_data *)rt_malloc(sizeof(struct rt_touchpanel_data));
    RT_ASSERT(touchpanel != RT_NULL);
    rt_memset((void *)touchpanel, 0, sizeof(struct rt_touchpanel_data));

    touchpanel->sem = rt_sem_create("tpsem", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(touchpanel->sem != RT_NULL);

    touchpanel->mutex = rt_mutex_create("tpmutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(touchpanel->mutex != RT_NULL);

    rt_slist_init(&touchpanel->b_list);

    touchpanel->dev = dev;
    rt_device_set_rx_indicate(touchpanel->dev, rx_callback);

    rt_device_control(touchpanel->dev, RT_TOUCH_CTRL_GET_ID, touchpanel->id);
    touchpanel_dbg("id = %s\n", touchpanel->id);

    rt_device_control(touchpanel->dev, RT_TOUCH_CTRL_GET_INFO, &touchpanel->info);
    touchpanel_dbg("x_range = %d, y_range = %d, max_point = %d\n",
                   touchpanel->info.range_x,
                   touchpanel->info.range_y,
                   touchpanel->info.point_num);

    touchpanel->point = (struct rt_touch_data *)rt_malloc(sizeof(struct rt_touch_data) * touchpanel->info.point_num);
    RT_ASSERT(touchpanel->point != RT_NULL);
    rt_memset(touchpanel->point, 0, sizeof(struct rt_touch_data) * touchpanel->info.point_num);

    touchpanel->cur_point = (struct rt_touch_data *)rt_malloc(sizeof(struct rt_touch_data) * touchpanel->info.point_num);
    RT_ASSERT(touchpanel->cur_point != RT_NULL);
    rt_memset(touchpanel->cur_point, 0, sizeof(struct rt_touch_data) * touchpanel->info.point_num);

    rt_uint8_t point_num = 0;
    rt_uint8_t point_max = touchpanel->info.point_num;
    struct rt_touch_data *read_data = touchpanel->point;

    while (1)
    {
        rt_sem_take(touchpanel->sem, RT_WAITING_FOREVER);

        rt_memset(read_data, 0, sizeof(struct rt_touch_data) * point_max);
        point_num = rt_device_read(touchpanel->dev, 0, read_data, point_max);
        if (point_num)
        {
#ifdef RT_TOUCHPANEL_DEBUG
            for (rt_uint8_t i = 0; i < point_num; i++)
            {
                rt_kprintf("%d %d %d %d %d %d\n",
                           read_data[i].track_id,
                           read_data[i].event,
                           read_data[i].x_coordinate,
                           read_data[i].y_coordinate,
                           read_data[i].timestamp,
                           read_data[i].width);
            }
#endif
            rt_touchpanel_blocks_handle(read_data, point_num);
        }
    }

    ret = rt_sem_delete(touchpanel->sem);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_close(touchpanel->dev);
    RT_ASSERT(ret == RT_EOK);

    rt_free(touchpanel->cur_point);
    rt_free(touchpanel->point);
    rt_free(touchpanel);
}

/**
 * touchpanel init.
 */
int rt_touchpanel_thread_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("touchpanel", rt_touchpanel_thread, RT_NULL, 2048, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}
INIT_ENV_EXPORT(rt_touchpanel_thread_init);
