/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __TOUCHPANEL_H__
#define __TOUCHPANEL_H__
#include <rtthread.h>

#include "touch.h"

#define TOUCHPANEL_BLOCK_MOVE_ENABLE  (0x01UL << 0)   /* Is block movement allowed */

struct rt_touchpanel_block
{
    rt_slist_t  slist;

    rt_uint16_t x;
    rt_uint16_t y;
    rt_uint16_t w;
    rt_uint16_t h;

    rt_tick_t   timestamp;
    rt_uint8_t  event;
    rt_uint32_t attribute;

    const char  *name;
    rt_err_t (*callback)(struct rt_touch_data *p, rt_uint8_t num);
};

struct rt_touchpanel_data
{
    rt_device_t dev;
    rt_sem_t    sem;
    rt_mutex_t  mutex;

    rt_uint8_t  id[8];
    struct rt_touch_info info;
    struct rt_touch_data *point;
    struct rt_touch_data *cur_point;

    rt_slist_t  b_list;   //rt_touchpanel_block list head
    void (*callback)(struct rt_touch_data *p, rt_uint8_t num);
};

rt_err_t rt_touchpanel_block_register(struct rt_touchpanel_block *block);
rt_err_t rt_touchpanel_block_unregister(struct rt_touchpanel_block *block);
rt_err_t rt_touchpoint_is_in_block(struct rt_touch_data *p, struct rt_touchpanel_block *block);
rt_err_t rt_touchpoint_is_valid(struct rt_touch_data *p, struct rt_touchpanel_block *block);

#endif
