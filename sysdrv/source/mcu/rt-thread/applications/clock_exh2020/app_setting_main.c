#include <rtthread.h>
#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "image_info.h"
#include "display.h"

#include <littlevgl2rtt.h>
#include <lvgl/lvgl.h>

#if defined(RT_USING_TOUCH_DRIVERS)
#include "touch.h"
#include "touchpanel.h"
#endif

#include "app_main.h"

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */

/* display region define */
#define LV_REGION_X      0
#define LV_REGION_Y      0

#define LV_WIN_XRES      DISP_XRES
#define LV_WIN_YRES      DISP_YRES
#define LV_WIN_FB_W      LV_WIN_XRES
#define LV_WIN_FB_H      LV_WIN_YRES

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct app_setting_main_data_t *g_setting_main_data = RT_NULL;
struct refrsh_param_t app_setting_main_refrsh_param;

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

static rt_err_t app_setting_move_updn_design(void *param);
static design_cb_t setting_move_updn_design = { .cb = app_setting_move_updn_design, };
static refrsh_request_param_t setting_move_updn_refr_param;

static rt_err_t app_setting_move_updn_design(void *param)
{
    struct app_setting_main_data_t *pdata = g_setting_main_data;

    pdata->mov_offset += TOUCH_MOVE_STEP * pdata->mov_dir;
    if (pdata->mov_offset >= LV_WIN_YRES)   // return to setting full window
    {
        pdata->mov_dir     = 0;
        pdata->mov_offset  = LV_WIN_YRES;

        setting_move_updn_refr_param.wflag = 0x01 << app_setting_main_refrsh_param.win_id;
        setting_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&setting_move_updn_refr_param);
    }
    else if (pdata->mov_offset <= 0)        // switch to clock window
    {
        pdata->mov_dir     = 0;
        pdata->mov_offset  = 0;

        app_clock_touch_unregister();

        app_clock_touch_register(&app_clock_touch_cb);
        app_clock_timer_cb_register(app_clock_update);

        setting_move_updn_refr_param.wflag = 0x01 << app_clock_refrsh_param.win_id;
        setting_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&setting_move_updn_refr_param);
    }
    else // continue move
    {
        app_clock_design_request(0, &setting_move_updn_design, RT_NULL);

        setting_move_updn_refr_param.wflag = (0x01 << app_clock_refrsh_param.win_id) | (0x01 << app_setting_main_refrsh_param.win_id);
        setting_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&setting_move_updn_refr_param);
    }

    return RT_EOK;
}

static rt_err_t app_setting_touch_move_updown(void *param)
{
    struct app_main_data_t        *pdata = (struct app_main_data_t *)param;
    struct app_setting_main_data_t *appdata = g_setting_main_data;

    appdata->mov_dir = pdata->ydir;
    appdata->mov_offset = pdata->yoffset + LV_WIN_YRES;

    if (appdata->mov_offset <= 0)
    {
        appdata->mov_offset = 0;
        return RT_EOK;
    }
    if (appdata->mov_offset >= LV_WIN_YRES)
    {
        appdata->mov_offset = LV_WIN_YRES;
        return RT_EOK;
    }

    refrsh_request_param_t refr_param;
    refr_param.wflag = (0x01 << app_clock_refrsh_param.win_id) | (0x01 << app_setting_main_refrsh_param.win_id);
    refr_param.wait = RT_WAITING_NO;

    return (app_clock_refresh_now(&refr_param));
}

static rt_err_t app_setting_touch_move_up(void *param)
{
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;

    if (pdata->dir_mode == TOUCH_DIR_MODE_UPDN)
    {
        app_clock_design_request(0, &setting_move_updn_design, RT_NULL);
    }

    app_clock_touch_value_reset();

    return RT_EOK;
}

static refrsh_request_param_t touch_up_refrsh_request_param;
static rt_err_t app_setting_touch_up(void *param)
{
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;
    struct rt_touch_data   *p   = &pdata->cur_point[0];


    if ((((21 - 0) < p->x_coordinate) && (p->x_coordinate < (21  + 128 + 0))) &&
            (((73 - 0) < p->y_coordinate) && (p->y_coordinate < (73 + 128 + 0))))
    {
        //rt_kprintf("msg touch down\n");

        app_clock_touch_unregister();
        app_clock_touch_register(&app_func_list_touch_cb);

        // refresh win layers register
        app_setting_main_refrsh_param.win_id    = APP_CLOCK_WIN_0;
        app_setting_main_refrsh_param.win_layer = WIN_BOTTOM_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_0, app_setting_main_refresh, &app_setting_main_refrsh_param);

        app_func_list_refrsh_param.win_id    = APP_CLOCK_WIN_1;
        app_func_list_refrsh_param.win_layer = WIN_MIDDLE_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_1, app_func_list_refrsh, &app_func_list_refrsh_param);

        alpha_win_refrsh_param.win_id    = APP_CLOCK_WIN_2;
        alpha_win_refrsh_param.win_layer = WIN_TOP_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_2, alpha_win_refresh, &alpha_win_refrsh_param);

        g_func_list_data->mov_xdir    = 0;
        g_func_list_data->mov_ydir    = 0;
        g_func_list_data->mov_xoffset = 0;
        g_func_list_data->mov_yoffset = 0;
        g_alpha_win_data->mov_xoffset = 0;

        touch_up_refrsh_request_param.wflag = (0x01 << app_func_list_refrsh_param.win_id) | (0x01 << alpha_win_refrsh_param.win_id);
        touch_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&touch_up_refrsh_request_param);
    }
    else if ((((219 - 0) < p->x_coordinate) && (p->x_coordinate < (219 + 128 + 0))) &&
             (((73  - 0) < p->y_coordinate) && (p->y_coordinate < (73  + 128 + 0))))
    {
        //rt_kprintf("setting touch down\n");

        app_clock_touch_unregister();
        app_clock_touch_register(&app_setting_menu_touch_cb);

        // refresh win layers register
        app_setting_main_refrsh_param.win_id    = APP_CLOCK_WIN_0;
        app_setting_main_refrsh_param.win_layer = WIN_BOTTOM_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_0, app_setting_main_refresh, &app_setting_main_refrsh_param);

        app_setting_menu_refrsh_param.win_id    = APP_CLOCK_WIN_1;
        app_setting_menu_refrsh_param.win_layer = WIN_MIDDLE_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_1, app_setting_menu_refrsh, &app_setting_menu_refrsh_param);

        alpha_win_refrsh_param.win_id    = APP_CLOCK_WIN_2;
        alpha_win_refrsh_param.win_layer = WIN_TOP_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_2, alpha_win_refresh, &alpha_win_refrsh_param);

        g_setting_menu_data->mov_xdir    = 0;
        g_setting_menu_data->mov_ydir    = 0;
        g_setting_menu_data->mov_xoffset = 0;
        g_setting_menu_data->mov_yoffset = 0;
        g_alpha_win_data->mov_xoffset    = 0;

        touch_up_refrsh_request_param.wflag = (0x01 << app_setting_menu_refrsh_param.win_id) | (0x01 << alpha_win_refrsh_param.win_id);
        touch_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&touch_up_refrsh_request_param);
    }


    return RT_EOK;
}

struct app_main_touch_cb_t app_setting_main_touch_cb =
{
    .tp_move_updn   = app_setting_touch_move_updown,
    .tp_move_up     = app_setting_touch_move_up,
    .tp_touch_up    = app_setting_touch_up,
};

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
/**
 * top layer refresh.
 */
rt_err_t app_setting_main_refresh(struct rt_display_config *wincfg, void *param)
{
    struct rt_device_graphic_info *info = &app_main_data->disp->info;
    struct app_setting_main_data_t *pdata = g_setting_main_data;
    struct refrsh_param_t *par = (struct refrsh_param_t *)param;

    //RT_ASSERT(pdata->mov_offset > 0);
    if (pdata->mov_offset <= 0)
    {
        rt_kprintf("waring: app_setting_main_refresh mov_offset %d <= 0\n", pdata->mov_offset);
        pdata->mov_offset = 1;
    }
    if (pdata->mov_offset > LV_WIN_FB_H)
    {
        rt_kprintf("waring: app_setting_main_refresh mov_offset %d > LV_WIN_FB_H\n", pdata->mov_offset);
        pdata->mov_offset = LV_WIN_FB_H;
    }

    wincfg->winId   = par->win_id;
    wincfg->zpos    = par->win_layer;
    wincfg->colorkey = COLOR_KEY_EN | 0x5555;
    wincfg->format  = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut     = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->fb    = pdata->fb + pdata->fblen - pdata->mov_offset * sizeof(rt_uint16_t) * LV_WIN_FB_W;
    wincfg->fblen = pdata->mov_offset * sizeof(rt_uint16_t) * LV_WIN_FB_W;
    wincfg->xVir  = LV_WIN_FB_W;
    wincfg->w     = LV_WIN_XRES;
    wincfg->h     = pdata->mov_offset; /* LV_WIN_YRES */
    wincfg->x     = LV_REGION_X + ((info->width  - LV_WIN_XRES) / 2);
    wincfg->y     = LV_REGION_Y + ((info->height - LV_WIN_YRES) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * LV_COLOR_DEPTH) % 32) == 0);
    //RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */

static struct rt_work init_work;

static lv_img_dsc_t applist_dsc;
static lv_img_dsc_t setting_dsc;
static lv_style_t main_style;
static lv_style_t date_label_style;

const char *week_days[] =
{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
};

static void app_lv_setting_main_init_work(struct rt_work *work, void *work_data)
{
    struct app_setting_main_data_t *pdata;

    g_setting_main_data = pdata = (struct app_setting_main_data_t *)rt_malloc(sizeof(struct app_setting_main_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct app_setting_main_data_t));

    /* framebuffer malloc */
    pdata->fblen = LV_WIN_FB_W * LV_WIN_FB_H * LV_COLOR_DEPTH / 8;
    pdata->fb   = (rt_uint8_t *)rt_malloc_psram(pdata->fblen);
    RT_ASSERT(pdata->fb != RT_NULL);

    RT_ASSERT(LV_HOR_RES == LV_WIN_FB_W);
    RT_ASSERT(LV_VER_RES == LV_WIN_FB_H);
    RT_ASSERT(16 == LV_COLOR_DEPTH);

    /* Image soruce load */
    lv_clock_img_file_load(&applist_dsc, "L:/application_list.bin");
    lv_clock_img_file_load(&setting_dsc, "L:/system_menu.bin");

    /* style init */
    lv_style_init(&main_style);
    lv_style_set_bg_color(&main_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_radius(&main_style, LV_STATE_DEFAULT, 0/*LV_RADIUS_CIRCLE*/);
    lv_style_set_border_width(&main_style, LV_STATE_DEFAULT, 0);

    lv_style_init(&date_label_style);
    lv_style_set_bg_opa(&date_label_style, LV_STATE_DEFAULT, LV_OPA_80);
    lv_style_set_bg_color(&date_label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_color(&date_label_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_color(&date_label_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_border_color(&date_label_style, LV_STATE_FOCUSED, LV_COLOR_GRAY);
    lv_style_set_border_width(&date_label_style, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_top(&date_label_style, LV_STATE_DEFAULT, 4);
    lv_style_set_pad_bottom(&date_label_style, LV_STATE_DEFAULT, 4);
    lv_style_set_pad_left(&date_label_style, LV_STATE_DEFAULT, 4);
    lv_style_set_pad_right(&date_label_style, LV_STATE_DEFAULT, 4);

    /* Background */
    lv_obj_t *obj_main = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_add_style(obj_main, LV_STATE_DEFAULT, &main_style);
    lv_obj_set_size(obj_main, LV_WIN_FB_W, LV_WIN_FB_H);
    lv_obj_set_pos(obj_main, 0, 0);

    //msg btn
    lv_obj_t *obj_msg = lv_img_create(obj_main, NULL);
    lv_img_set_src(obj_msg, &applist_dsc);
    lv_obj_set_pos(obj_msg, lv_obj_get_width(obj_main) / 2 - applist_dsc.header.w - 35, lv_obj_get_height(obj_main) / 4 - applist_dsc.header.h / 2 + 25);
    //rt_kprintf("msg: x = %d, y = %d, w = %d, h = %d\n", lv_obj_get_x(obj_msg), lv_obj_get_y(obj_msg), lv_obj_get_width(obj_msg), lv_obj_get_height(obj_msg));

    lv_obj_t *msg_label = lv_label_create(obj_main, NULL);
    lv_obj_set_pos(msg_label, lv_obj_get_width(obj_main) / 2 - applist_dsc.header.w - 40, lv_obj_get_height(obj_main) / 4 + applist_dsc.header.h / 2 + 35);
    lv_label_set_text(msg_label, "Application");

    //setting btn
    lv_obj_t *obj_setting = lv_img_create(obj_main, NULL);
    lv_img_set_src(obj_setting, &setting_dsc);
    lv_obj_set_pos(obj_setting, lv_obj_get_width(obj_main) / 2 + 35, lv_obj_get_height(obj_main) / 4 - setting_dsc.header.h / 2 + 15 + 10);
    //rt_kprintf("setting: x = %d, y = %d, w = %d, h = %d\n", lv_obj_get_x(obj_setting), lv_obj_get_y(obj_setting), lv_obj_get_width(obj_setting), lv_obj_get_height(obj_msg));

    lv_obj_t *setting_label = lv_label_create(obj_main, NULL);
    lv_obj_set_pos(setting_label, lv_obj_get_width(obj_main) / 2 + 55, lv_obj_get_height(obj_main) / 4 + setting_dsc.header.h / 2 + 35);
    lv_label_set_text(setting_label, "Setting");

    /* Date info */
    clock_time_t *date = &app_main_data->tmr_data;
    char str[16];
    lv_obj_t *obj_date = lv_label_create(obj_main, NULL);
    lv_obj_add_style(obj_date, LV_STATE_DEFAULT, &date_label_style);
    snprintf(str, 16, "%04d-%02d-%02d", date->year, date->month, date->day);
    lv_label_set_text(obj_date, str);
    lv_obj_align(obj_date, obj_main, LV_ALIGN_CENTER, 0, lv_obj_get_height(obj_main) / 4 + 40);

    lv_obj_t *obj_week = lv_label_create(obj_main, NULL);
    lv_label_set_text(obj_week, week_days[date->weekdays]);
    lv_obj_align(obj_week, obj_main, LV_ALIGN_CENTER, 0, lv_obj_get_height(obj_main) / 4 + 80);

    //draw all......
    lv_refr_now(lv_disp_get_default());

    // copy data to app_setting_main
    rt_memcpy(pdata->fb, g_lvdata->fb, g_lvdata->fblen);

    lv_clock_img_dsc_free(&applist_dsc);
    lv_clock_img_dsc_free(&setting_dsc);

    lv_obj_del(obj_main);
}

/**
 * App clock fast init.
 */
void app_setting_main_init(void)
{
    rt_work_init(&init_work, app_lv_setting_main_init_work, RT_NULL);
    rt_workqueue_dowork(app_main_data->workqueue, &init_work);
}