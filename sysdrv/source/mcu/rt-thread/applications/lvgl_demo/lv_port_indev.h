/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-10     Chris Zhong      First version
*/

#ifndef LV_PORT_INDEV_H
#define LV_PORT_INDEV_H

#include <rtthread.h>
#include "lvgl.h"

typedef enum {
	TOUCH_EVENT_NONE = 0x00,
	TOUCH_EVENT_UP   = 0x01, 
	TOUCH_EVENT_DOWN = 0x02, 
	TOUCH_EVENT_MOVE = 0x03
}touch_event_id_t;

typedef struct {
	rt_bool_t down;
	lv_coord_t x;
	lv_coord_t y;
	touch_event_id_t event;
}touch_event_t;

void lv_port_indev_init(void);
void lv_port_indev_deinit(void);

#endif
