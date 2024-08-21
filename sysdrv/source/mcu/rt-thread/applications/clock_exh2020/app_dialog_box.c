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
#define DIALOG_REGION_X      0
#define DIALOG_REGION_Y      0

#define DIALOG_WIN_XRES      DISP_XRES
#define DIALOG_WIN_YRES      DISP_YRES
#define DIALOG_WIN_FB_W      DIALOG_WIN_XRES
#define DIALOG_WIN_FB_H      DIALOG_WIN_YRES
#define DIALOG_WIN_COLOR_DEPTH       16

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
static struct rt_work init_work;

struct g_dialog_box_data_t *g_dialog_box_data = RT_NULL;
struct refrsh_param_t app_dialog_box_refrsh_param;

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
static refrsh_request_param_t touch_up_refrsh_request_param;
static rt_err_t app_dialog_box_touch_up(void *param)
{
    if (g_dialog_box_data->mode == 0)       //applist
    {
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

        touch_up_refrsh_request_param.wflag = (0x01 << app_func_list_refrsh_param.win_id) | (0x01 << alpha_win_refrsh_param.win_id);
        touch_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&touch_up_refrsh_request_param);
    }
    else if (g_dialog_box_data->mode == 1)  //set menu
    {
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

        touch_up_refrsh_request_param.wflag = (0x01 << app_setting_menu_refrsh_param.win_id) | (0x01 << alpha_win_refrsh_param.win_id);
        touch_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&touch_up_refrsh_request_param);
    }

    return RT_EOK;
}
struct app_main_touch_cb_t app_dialog_box_touch_cb =
{
    .tp_touch_down  = 0,
    .tp_move_lr     = 0,
    .tp_move_updn   = 0,
    .tp_move_up     = app_dialog_box_touch_up,
    .tp_touch_up    = app_dialog_box_touch_up,
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
rt_err_t app_dialog_box_refrsh(struct rt_display_config *wincfg, void *param)
{
    struct rt_device_graphic_info *info = &app_main_data->disp->info;
    struct g_dialog_box_data_t *pdata = g_dialog_box_data;
    struct refrsh_param_t *par = (struct refrsh_param_t *)param;

    wincfg->winId   = par->win_id;
    wincfg->zpos    = par->win_layer;
    wincfg->format  = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut     = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->fb    = pdata->fb;
    wincfg->fblen = pdata->fblen;
    wincfg->xVir  = DIALOG_WIN_FB_W;
    wincfg->w     = DIALOG_WIN_XRES;
    wincfg->h     = DIALOG_WIN_YRES;
    wincfg->x     = DIALOG_REGION_X + ((info->width  - DIALOG_WIN_XRES) / 2);
    wincfg->y     = DIALOG_REGION_Y + ((info->height - DIALOG_WIN_YRES) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * DIALOG_WIN_COLOR_DEPTH) % 32) == 0);

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

/**
 * draw needle.
 */
static void app_dialog_box_init_work(struct rt_work *work, void *work_data)
{
    struct g_dialog_box_data_t *pdata;

    g_dialog_box_data = pdata = (struct g_dialog_box_data_t *)rt_malloc(sizeof(struct g_dialog_box_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct g_dialog_box_data_t));

    /* framebuffer malloc */
    pdata->fblen = DIALOG_WIN_FB_W * DIALOG_WIN_FB_H * DIALOG_WIN_COLOR_DEPTH / 8;
    pdata->fb   = (rt_uint8_t *)rt_malloc_psram(pdata->fblen);
    RT_ASSERT(pdata->fb != RT_NULL);

    int fd = open("img_dialog.dta", O_RDONLY, 0);
    read(fd, pdata->fb, pdata->fblen);
    close(fd);
}

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */

/**
 * App clock fast init.
 */
void app_dialog_box_init(void)
{
    rt_work_init(&init_work, app_dialog_box_init_work, RT_NULL);
    rt_workqueue_dowork(app_main_data->workqueue, &init_work);
}
