/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-10     Chris Zhong      First version
*/

#include "lv_app_main.h"
#include "drv_heap.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
#include "lv_demo_benchmark.h"
#include "lv_demo_music.h"

#define MS_TO_TICK(ms) (((ms) * RT_TICK_PER_SECOND) / 1000)
#define LVGL_TASK_TICKS MS_TO_TICK(1)

typedef struct {
	rt_event_t	event;
	rt_timer_t	timer;
} lvgl_cntx_t;
static lvgl_cntx_t lvgl_ctx = {0};

static void lvgl_task_timer(void *parameter)
{
	rt_event_send(lvgl_ctx.event, LVGL_EVENT_TASK_HANDLER);
}

void lvgl_task_commit(void)
{
	rt_event_send(lvgl_ctx.event, LVGL_EVENT_COMMIT);
}

static void lvgl_task(void *arg)
{
	uint32_t event;
	rt_err_t ret = RT_EOK;

	lv_port_disp_init();
	lv_port_indev_init();

#if LV_USE_FILESYSTEM
	lv_port_fs_init();
#endif

	printf("\n Welcome to the LittleVGL v8.3.3\n");

	lvgl_ctx.event = rt_event_create("lvEvent", RT_IPC_FLAG_FIFO);
	RT_ASSERT(lvgl_ctx.event != RT_NULL);
	rt_event_send(lvgl_ctx.event, LVGL_EVENT_REFR_DONE);

	lvgl_ctx.timer = rt_timer_create("lvTimer", lvgl_task_timer, NULL, LVGL_TASK_TICKS, RT_TIMER_FLAG_PERIODIC);
	RT_ASSERT(lvgl_ctx.timer != RT_NULL);
	rt_timer_start(lvgl_ctx.timer);

#if LV_USE_DEMO_MUSIC
	lv_demo_music();
#elif LV_USE_DEMO_BENCHMARK
	lv_demo_benchmark();
#endif

	while (1) {
		ret = rt_event_recv(lvgl_ctx.event, LVGL_EVENT_TASK_HANDLER | LVGL_EVENT_COMMIT,
				    RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
				    RT_WAITING_FOREVER, &event);
		if (ret != RT_EOK) {
			/* Reserved... */
		}

		if (event & LVGL_EVENT_TASK_HANDLER)
			lv_task_handler();

		if (event & LVGL_EVENT_COMMIT)
			lv_disp_refer();
	}

	rt_timer_stop(lvgl_ctx.timer);
	ret = rt_timer_delete(lvgl_ctx.timer);
	lvgl_ctx.timer = RT_NULL;
	RT_ASSERT(ret == RT_EOK);

	rt_event_delete(lvgl_ctx.event);
	lv_port_disp_deinit();
	lv_port_indev_deinit();
}

int lvgl_app_init(void)
{
	rt_thread_t lvgl;

	lvgl = rt_thread_create("lvgl", lvgl_task, RT_NULL, 4096, 4, 20);
	rt_thread_startup(lvgl);

	return RT_EOK;
}

INIT_APP_EXPORT(lvgl_app_init);
