/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-10     Chris Zhong      First version
*/
#ifndef __LV_PORT_DISP_H__
#define __LV_PORT_DISP_H__

#include <rtthread.h>
#include "drv_display.h"
#include "dma.h"
#include "drv_heap.h"
#include "lvgl.h"

typedef struct {
	rt_device_t lcd;
	rt_device_t bl;
	
	struct rt_device_graphic_info* dgi;
	lv_disp_draw_buf_t disp_buf;
	lv_color_t* buf_1;
	lv_color_t* buf_2;

	struct CRTC_WIN_STATE* cfg;
	struct VOP_POST_SCALE_INFO *scale;

	rt_device_t dma;
	struct rt_dma_transfer xfer;
	rt_sem_t sem;
	rt_uint8_t dma_run;

}lv_disp_cntx_t;

rt_err_t lv_port_disp_init(void);
rt_err_t lv_port_disp_deinit(void);

struct rt_device_graphic_info* lv_port_dgi_get(void);

void lv_disp_refer(void);
#endif
