/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-10     Chris Zhong      First version
*/

#include "lv_port_indev.h"
#include "rtdef.h"
#include "touchpanel.h"
#include "lv_port_disp.h"
#include "lv_app_main.h"

#ifdef RT_USING_TOUCH_DRIVERS
static touch_event_t glte = {0};
static struct rt_touchpanel_block lvgl_touch_block;

static void lv_port_indev_touch_event(rt_int16_t x, rt_int16_t y, rt_uint8_t state)
{
    switch (state) {
    case TOUCH_EVENT_UP:
        glte.down = RT_FALSE;
        break;
    case TOUCH_EVENT_DOWN:
        glte.x = x;
        glte.y = y;
        glte.down = RT_TRUE;
        break;
    case TOUCH_EVENT_MOVE:
        glte.x = x;
        glte.y = y;
        break;
    }

	if(glte.x < 0) glte.x = 0;
	if(glte.x >= LV_HOR_RES) glte.x = LV_HOR_RES-1;
	if(glte.y < 0) glte.y = 0;
	if(glte.y >= LV_VER_RES) glte.y = LV_VER_RES-1;
}

static void touchpad_init(void)
{
	rt_memset(&glte, 0x0, sizeof(glte));
}

static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    data->point.x = glte.x;
    data->point.y = glte.y;
    data->state = glte.down ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

	/*Return `false` because we are not buffering and no more data to read*/
//    return false; 
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    return glte.down?true:false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    (*x) = glte.x;
    (*y) = glte.y;
}

static rt_err_t lv_port_touch_cb(struct rt_touch_data *point, rt_uint8_t num)
{
    struct rt_touch_data *p = &point[0];
    struct rt_touchpanel_block *b = &lvgl_touch_block;

    if (RT_EOK != rt_touchpoint_is_valid(p, b)) {
        return RT_ERROR;
    }

	if (b->event == RT_TOUCH_EVENT_DOWN) {
		glte.x = p->x_coordinate - b->x;
		glte.y = p->y_coordinate - b->y;
		glte.down = RT_TRUE;
		glte.event = TOUCH_EVENT_DOWN;
	} else if (b->event == RT_TOUCH_EVENT_MOVE) {
		glte.x = p->x_coordinate - b->x;
		glte.y = p->y_coordinate - b->y;
		glte.event = TOUCH_EVENT_MOVE;
	} else if (b->event == RT_TOUCH_EVENT_UP) {
		glte.down = RT_FALSE;
		glte.event = TOUCH_EVENT_UP;
	}

	//TODO: dispatch the touch event.
	if(glte.x < 0) glte.x = 0;
	if(glte.x >= LV_HOR_RES) glte.x = LV_HOR_RES-1;
	if(glte.y < 0) glte.y = 0;
	if(glte.y >= LV_VER_RES) glte.y = LV_VER_RES-1;
	
    return RT_EOK;
}

static void lv_port_touch_block_init(struct rt_touchpanel_block *block)
{
	struct rt_device_graphic_info* dgi = lv_port_dgi_get();

    rt_memset(block, 0, sizeof(struct rt_touchpanel_block));

    block->x = LVGL_REGION_X + ((dgi->width  - LVGL_FB_WIDTH)>>1);
    block->y = LVGL_REGION_Y + ((dgi->height - LVGL_FB_HEIGHT)>>1);
	
    block->w = LVGL_FB_WIDTH;
    block->h = LVGL_FB_HEIGHT;
	
    block->name = "lvTouch";
    block->callback = lv_port_touch_cb;
}
#endif

void lv_port_indev_init(void)
{
	static lv_indev_drv_t indev_drv;

#ifdef RT_USING_TOUCH_DRIVERS
	touchpad_init();

	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = touchpad_read;
	lv_indev_drv_register(&indev_drv);

	lv_port_touch_block_init(&lvgl_touch_block);
	rt_touchpanel_block_register(&lvgl_touch_block);
#endif
}

void lv_port_indev_deinit(void)
{
#ifdef RT_USING_TOUCH_DRIVERS
	rt_touchpanel_block_unregister(&lvgl_touch_block);
#endif
}
