/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __APP_MAIN__
#define __APP_MAIN__
#include <rtthread.h>

#if defined(RT_USING_TOUCH_DRIVERS)
#include "touch.h"
#include "touchpanel.h"
#endif

#include <littlevgl2rtt.h>
#include <lvgl/lvgl.h>

#include "image_info.h"

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */

#define CLOCK_EXH2020_DEBUG

#define APP_CLOCK_WIN_0         0
#define APP_CLOCK_WIN_1         1
#define APP_CLOCK_WIN_2         2

#define APP_CLOCK_WIN           0   // bottom
#define SETTING_MAIN_WIN        1   // top

//#define SETTING_MAIN_WIN      0   // bottom
#define SETTING_MENU_WIN        1   // mid
#define FUNC_LIST_WIN           1   // mid
#define APP_ALPHA_WIN           2   // top

//#define SETTING_MENU_WIN      0   // bottom
//#define FUNC_LIST_WIN         0   // bottom
//#define APP_ALPHA_WIN         2   // mid
#define APP_DIALOG_WIN          1   // top

#define DISP_XRES       368
#define DISP_YRES       448

#define TOUCH_REGION_X  0
#define TOUCH_REGION_Y  0
#define TOUCH_REGION_W  DISP_XRES
#define TOUCH_REGION_H  DISP_YRES

// mq cmd define
#define MQ_DESIGN_UPDATE            (0x01UL)
#define MQ_REFR_UPDATE              (0x02UL)

//event define
#define EVENT_REFR_DONE             (0x01UL << 0)

// touch define
#define TOUCH_START_THRESHOLD   20
#define TOUCH_MOVE_THRESHOLD    2
#define TOUCH_MOVE_STEP         40

#define TOUCH_DIR_MODE_NULL     0
#define TOUCH_DIR_MODE_LR       1   //move left & right
#define TOUCH_DIR_MODE_UPDN     2   //move up & down

/*
 **************************************************************************************************
 *
 * Struct & data define
 *
 **************************************************************************************************
 */
typedef struct
{
    rt_uint16_t w;
    rt_uint16_t h;
    const char *name;
} img_load_info_t;

typedef struct
{
    rt_uint16_t year;
    rt_uint8_t  month;
    rt_uint8_t  day;
    rt_uint8_t  weekdays;

    rt_uint8_t  hour;
    rt_uint8_t  minute;
    rt_uint8_t  second;
    rt_uint8_t  tick;
} clock_time_t;

#define CLOCK_EXH_MQ_NUM    32
typedef struct
{
    rt_uint32_t cmd;
    void       *param;
} clock_exh_mq_t;

typedef struct
{
    rt_slist_t list;
    rt_err_t (*cb)(void *param);
} design_cb_t;

typedef struct
{
    rt_err_t (*cb)(struct rt_display_config *wincfg, void *param);
    void *param;
} refresh_cb_t;

struct refrsh_param_t
{
    rt_uint8_t win_id;
    rt_uint8_t win_layer;
};

typedef struct
{
    rt_int32_t wait;
    rt_uint8_t wflag;
} refrsh_request_param_t;

struct app_main_touch_cb_t
{
    rt_err_t (*tp_touch_down)(void *param);

    rt_err_t (*tp_move_lr_start)(void *param);
    rt_err_t (*tp_move_updn_start)(void *param);

    rt_err_t (*tp_move_lr)(void *param);
    rt_err_t (*tp_move_updn)(void *param);

    rt_err_t (*tp_move_up)(void *param);
    rt_err_t (*tp_touch_up)(void *param);
};

struct app_lvgl_data_t
{
    rt_uint8_t *fb;
    rt_uint32_t fblen;
};

struct app_main_data_t
{
    rt_display_data_t disp;
    rt_event_t        event;
    rt_mq_t           mq;

    rt_timer_t        clk_timer;
    clock_time_t      tmr_data;
    void (*timer_cb)(void);

    struct rt_workqueue *workqueue;

    rt_slist_t        design_list;
    refresh_cb_t      refr[3];

    rt_uint8_t        tb_flag;
    rt_uint8_t        dir_mode;
    rt_int16_t        xdir;
    rt_int16_t        ydir;
    rt_int16_t        xoffset;
    rt_int16_t        yoffset;
    rt_int16_t        mov_fix;
    rt_int16_t        mov_speed;
    rt_uint32_t       down_timestamp;

    struct rt_touchpanel_block touch_block;
    struct rt_touch_data down_point[1];
    struct rt_touch_data pre_point[1];
    struct rt_touch_data cur_point[1];

    rt_err_t (*tp_touch_down)(void *param);

    rt_err_t (*tp_move_lr_start)(void *param);
    rt_err_t (*tp_move_updn_start)(void *param);

    rt_err_t (*tp_move_lr)(void *param);
    rt_err_t (*tp_move_updn)(void *param);

    rt_err_t (*tp_move_up)(void *param);
    rt_err_t (*tp_touch_up)(void *param);
    rt_device_t wdt_dev;
};
extern struct app_main_data_t *app_main_data;
extern struct app_lvgl_data_t *g_lvdata;

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

void lv_clock_img_file_load(lv_img_dsc_t *img_dsc, const char *file);
void lv_clock_img_dsc_free(lv_img_dsc_t *img_dsc);

rt_err_t app_clock_load_img(img_load_info_t *info, rt_uint8_t *pbuf, rt_uint16_t w, rt_uint16_t h, rt_uint16_t offset, rt_uint8_t bitsize);

void app_clock_timer_cb_register(void (*cb)(void));

void app_clock_timer_cb_unregister(void);

/**
 * Design request.
 */
void app_clock_design_request(rt_uint8_t urgent, design_cb_t *design, void *param);

/**
 * Refresh callback register.
 */
void app_clock_refresh_register(rt_uint8_t winid, void *cb, void *param);

/**
 * Refresh callback unregister.
 */
void app_clock_refresh_unregister(rt_uint8_t winid);

/**
 * Refresh callback request.
 */
void app_clock_refresh_request(void *param);

/**
 * Direct refrest to LCD.
 */
rt_err_t app_clock_refresh_now(void *param);

void app_clock_touch_value_reset(void);

void app_clock_touch_register(struct app_main_touch_cb_t *tcb);

void app_clock_touch_unregister(void);

/**********************
 * SUB INCLUDE
 **********************/
#include "app_clock.h"
#include "app_setting_main.h"
#include "app_setting_menu.h"
#include "app_func_list.h"
#include "app_alpha_win.h"
#include "app_dialog_box.h"

#endif
