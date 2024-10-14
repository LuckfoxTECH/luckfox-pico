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
#define FUNC_REGION_X      0
#define FUNC_REGION_Y      0

#define FUNC_WIN_XRES      DISP_XRES
#define FUNC_WIN_YRES      DISP_YRES
#define FUNC_WIN_FB_W      FUNC_WIN_XRES
#define FUNC_WIN_FB_H      (FUNC_WIN_YRES * 4 + FUNC_WIN_YRES * 2 / 3)

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
struct g_func_list_data_t *g_func_list_data = RT_NULL;
struct refrsh_param_t app_func_list_refrsh_param;

//static refrsh_request_param_t  app_func_list_refrsh_req_param;

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

static rt_int16_t move_updn_len  = 0;
static rt_int16_t move_updn_step = 10;
static rt_err_t app_funclist_move_updn_design(void *param);
static design_cb_t funclist_move_updn_design = { .cb = app_funclist_move_updn_design, };
static refrsh_request_param_t funclist_move_updn_refr_param;

static rt_err_t app_funclist_move_updn_design(void *param)
{
    struct g_func_list_data_t *pdata = g_func_list_data;
    rt_int16_t offset = 0;

    if (move_updn_len > 0)
    {
        if (move_updn_len > move_updn_step)
        {
            offset = move_updn_step;
            move_updn_len -= offset;
        }
        else
        {
            offset = move_updn_len;
            move_updn_len = 0;
        }

        pdata->mov_yoffset += offset * pdata->mov_ydir;
        funclist_move_updn_refr_param.wflag = (0x01 << app_func_list_refrsh_param.win_id) |
                                              (0x01 << alpha_win_refrsh_param.win_id);
        funclist_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&funclist_move_updn_refr_param);

        app_clock_design_request(0, &funclist_move_updn_design, RT_NULL);
    }

    return RT_EOK;
}

static rt_err_t app_funclist_move_lr_design(void *param);
static design_cb_t funclist_move_lr_design = { .cb = app_funclist_move_lr_design, };
static refrsh_request_param_t funclist_move_lr_design_refr_param;

static rt_err_t app_funclist_move_lr_design(void *param)
{
    struct g_func_list_data_t *appdata   = g_func_list_data;
    struct g_alpha_win_data_t *alphadata = g_alpha_win_data;

    appdata->mov_xoffset += TOUCH_MOVE_STEP * appdata->mov_xdir;
    alphadata->mov_xoffset = appdata->mov_xoffset;
    if (appdata->mov_xoffset >= FUNC_WIN_XRES)
    {
        app_clock_touch_unregister();
        app_clock_touch_register(&app_setting_main_touch_cb);

        // refresh win layers register
        app_clock_refrsh_param.win_id    = APP_CLOCK_WIN_0;
        app_clock_refrsh_param.win_layer = WIN_BOTTOM_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_0, app_clock_win_refresh, &app_clock_refrsh_param);

        app_setting_main_refrsh_param.win_id    = APP_CLOCK_WIN_1;
        app_setting_main_refrsh_param.win_layer = WIN_TOP_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_1, app_setting_main_refresh, &app_setting_main_refrsh_param);

        funclist_move_lr_design_refr_param.wflag = (0x01 << app_setting_main_refrsh_param.win_id);
        funclist_move_lr_design_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&funclist_move_lr_design_refr_param);
    }
    else if (appdata->mov_xoffset <= 0)
    {
        appdata->mov_xdir      = 0;
        appdata->mov_xoffset   = 0;
        alphadata->mov_xoffset = 0;
        funclist_move_lr_design_refr_param.wflag = (0x01 << app_setting_main_refrsh_param.win_id) |
                (0x01 << app_func_list_refrsh_param.win_id) |
                (0x01 << alpha_win_refrsh_param.win_id);
        funclist_move_lr_design_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&funclist_move_lr_design_refr_param);
    }
    else
    {
        funclist_move_lr_design_refr_param.wflag = (0x01 << app_setting_main_refrsh_param.win_id) |
                (0x01 << app_func_list_refrsh_param.win_id) |
                (0x01 << alpha_win_refrsh_param.win_id);
        funclist_move_lr_design_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&funclist_move_lr_design_refr_param);

        app_clock_design_request(0, &funclist_move_lr_design, RT_NULL);
    }

    return RT_EOK;
}

static rt_err_t app_funclist_touch_down(void *param)
{
    struct g_func_list_data_t *appdata    = g_func_list_data;
    struct g_alpha_win_data_t  *alphadata = g_alpha_win_data;

    appdata->mov_xdir      = 0;
    appdata->mov_ydir      = 0;
    appdata->mov_xoffset   = 0;
    alphadata->mov_xoffset = 0;

    return RT_EOK;
}

//static refrsh_request_param_t move_lr_refrsh_request_param;
static rt_err_t app_funclist_touch_move_lr(void *param)
{
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;
    struct g_func_list_data_t *appdata   = g_func_list_data;
    struct g_alpha_win_data_t *alphadata = g_alpha_win_data;

    appdata->mov_xdir    = -pdata->xdir;
    appdata->mov_xoffset = -pdata->xoffset;

    appdata->mov_xoffset = appdata->mov_xoffset & 0xfffc;   //align 32
    if (appdata->mov_xoffset <= 0)
    {
        return RT_EOK;
    }
    alphadata->mov_xoffset = appdata->mov_xoffset;

    refrsh_request_param_t refr_param;
    refr_param.wflag = (0x01 << app_setting_main_refrsh_param.win_id) |
                       (0x01 << app_func_list_refrsh_param.win_id) |
                       (0x01 << alpha_win_refrsh_param.win_id);
    refr_param.wait = RT_WAITING_NO;

    return (app_clock_refresh_now(&refr_param));
}

//static refrsh_request_param_t move_updn_refrsh_request_param;
static rt_err_t app_funclist_touch_move_updn(void *param)
{
    struct g_func_list_data_t *appdata = g_func_list_data;
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;
    struct rt_touch_data   *p   = &pdata->cur_point[0];
    struct rt_touch_data   *pre = &pdata->pre_point[0];
    rt_int16_t yoffset = (rt_int16_t)(p->y_coordinate - pre->y_coordinate);

    appdata->mov_ydir    = -pdata->ydir;
    appdata->mov_yoffset -= yoffset;

    if (appdata->mov_yoffset <= 0)
    {
        appdata->mov_yoffset = 0;
    }
    if (appdata->mov_yoffset >= FUNC_WIN_FB_H - FUNC_WIN_YRES)
    {
        appdata->mov_yoffset = FUNC_WIN_FB_H - FUNC_WIN_YRES;
    }

    refrsh_request_param_t refr_param;
    refr_param.wflag = (0x01 << app_func_list_refrsh_param.win_id) |
                       (0x01 << alpha_win_refrsh_param.win_id);
    refr_param.wait = RT_WAITING_NO;

    return (app_clock_refresh_now(&refr_param));
}

static refrsh_request_param_t move_up_refrsh_request_param;
static rt_err_t app_funclist_touch_moveup(void *param)
{
    struct g_func_list_data_t *appdata = g_func_list_data;
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;

    if (pdata->dir_mode == TOUCH_DIR_MODE_LR)
    {
        app_clock_design_request(0, &funclist_move_lr_design, RT_NULL);
    }
    else if (pdata->dir_mode == TOUCH_DIR_MODE_UPDN)
    {
        if (appdata->mov_yoffset <= 0)
        {
            appdata->mov_yoffset = 0;
            move_up_refrsh_request_param.wflag = (0x01 << app_func_list_refrsh_param.win_id) |
                                                 (0x01 << alpha_win_refrsh_param.win_id);
            move_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
            app_clock_refresh_request(&move_up_refrsh_request_param);

            goto exit;
        }
        if (appdata->mov_yoffset >= FUNC_WIN_FB_H - FUNC_WIN_YRES)
        {
            appdata->mov_yoffset = FUNC_WIN_FB_H - FUNC_WIN_YRES;
            move_up_refrsh_request_param.wflag = (0x01 << app_func_list_refrsh_param.win_id) |
                                                 (0x01 << alpha_win_refrsh_param.win_id);
            move_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
            app_clock_refresh_request(&move_up_refrsh_request_param);
            goto exit;
        }

        if (pdata->ydir == 1)
        {
            move_updn_len = appdata->mov_yoffset % (FUNC_WIN_YRES / 3);
        }
        else //if (pdata->ydir == -1)
        {
            move_updn_len = (FUNC_WIN_YRES / 3) - appdata->mov_yoffset % (FUNC_WIN_YRES / 3);
        }

        move_updn_step = move_updn_len / 4;
        if (move_updn_step == 0) move_updn_step = 1;

        app_clock_design_request(0, &funclist_move_updn_design, RT_NULL);
    }

exit:
    app_clock_touch_value_reset();

    return RT_EOK;
}

static refrsh_request_param_t touch_up_refrsh_request_param;
static rt_err_t app_funclist_touch_up(void *param)
{
    struct app_main_data_t        *pdata = (struct app_main_data_t *)param;
    struct rt_touch_data   *p   = &pdata->cur_point[0];

    if ((FUNC_WIN_YRES / 3 < p->y_coordinate) && (p->y_coordinate < (FUNC_WIN_YRES * 2) / 3))
    {
        app_clock_touch_unregister();
        app_clock_touch_register(&app_dialog_box_touch_cb);

        // refresh win layers register
        app_func_list_refrsh_param.win_id    = APP_CLOCK_WIN_0;
        app_func_list_refrsh_param.win_layer = WIN_BOTTOM_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_0, app_func_list_refrsh, &app_func_list_refrsh_param);

        alpha_win_refrsh_param.win_id    = APP_CLOCK_WIN_2;
        alpha_win_refrsh_param.win_layer = WIN_MIDDLE_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_2, alpha_win_refresh, &alpha_win_refrsh_param);

        app_dialog_box_refrsh_param.win_id    = APP_CLOCK_WIN_1;
        app_dialog_box_refrsh_param.win_layer = WIN_TOP_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_1, app_dialog_box_refrsh, &app_dialog_box_refrsh_param);

        touch_up_refrsh_request_param.wflag = (0x01 << app_dialog_box_refrsh_param.win_id);
        touch_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&touch_up_refrsh_request_param);

        g_dialog_box_data->mode = 0;

        app_clock_touch_value_reset();
    }

    return RT_EOK;
}
struct app_main_touch_cb_t app_func_list_touch_cb =
{
    .tp_touch_down  = app_funclist_touch_down,
    .tp_move_lr     = app_funclist_touch_move_lr,
    .tp_move_updn   = app_funclist_touch_move_updn,
    .tp_move_up     = app_funclist_touch_moveup,
    .tp_touch_up    = app_funclist_touch_up,
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
rt_err_t app_func_list_refrsh(struct rt_display_config *wincfg, void *param)
{
    struct rt_device_graphic_info *info = &app_main_data->disp->info;
    struct g_func_list_data_t *pdata = g_func_list_data;
    struct refrsh_param_t *par = (struct refrsh_param_t *)param;

    wincfg->winId   = par->win_id;
    wincfg->zpos    = par->win_layer;
    //wincfg->colorkey = COLOR_KEY_EN | 0x5555;
    wincfg->format  = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut     = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->fb    = pdata->fb    + (pdata->mov_yoffset * FUNC_WIN_FB_W + pdata->mov_xoffset) * sizeof(rt_uint16_t);
    wincfg->fblen = pdata->fblen - (pdata->mov_yoffset * FUNC_WIN_FB_W + pdata->mov_xoffset) * sizeof(rt_uint16_t);
    wincfg->xVir  = FUNC_WIN_FB_W;
    wincfg->w     = FUNC_WIN_XRES - pdata->mov_xoffset;
    wincfg->h     = FUNC_WIN_YRES;
    wincfg->x     = FUNC_REGION_X + ((info->width  - FUNC_WIN_XRES) / 2);
    wincfg->y     = FUNC_REGION_Y + ((info->height - FUNC_WIN_YRES) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * LV_COLOR_DEPTH) % 32) == 0);

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

#define APP_LIST_NUM  12

struct lv_app_list_data_t
{
    lv_style_t main_style;
    lv_style_t list_btn_style;

    lv_obj_t   *obj_main;
    lv_obj_t   *list_btn[APP_LIST_NUM];
    lv_img_dsc_t dsc[APP_LIST_NUM];
};
static struct lv_app_list_data_t lv_app_list_data;
static struct rt_work init_work;

struct lv_app_list_src_t
{
    const char *src;
    const char *title;
};

static const struct lv_app_list_src_t app_list[APP_LIST_NUM] =
{
    {"L:/message.bin",              " Message"},
    {"L:/weather.bin",              " Weather"},
    {"L:/bus_card.bin",             " Bus Card"},
    {"L:/alipay.bin",               " Alipay"},
    {"L:/stopwatch.bin",            " Stopwatch"},
    {"L:/clock_timer.bin",          " Clock"},
    {"L:/heart_rate.bin",           " HeartRate"},
    {"L:/physical_exercise.bin",    " Physical"},
    {"L:/breathing_training.bin",   " Breathing"},
    {"L:/altitude_barometer.bin",   " Altitude"},
    {"L:/compass.bin",              " Compass"},
    {"L:/pressure.bin",             " Pressure"},
};

static void app_func_list_init_work(struct rt_work *work, void *work_data)
{
    struct g_func_list_data_t *pdata;
    rt_uint16_t i;

    g_func_list_data = pdata = (struct g_func_list_data_t *)rt_malloc(sizeof(struct g_func_list_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct g_func_list_data_t));

    /* framebuffer malloc */
    pdata->fblen = FUNC_WIN_FB_W * FUNC_WIN_FB_H * LV_COLOR_DEPTH / 8;
    pdata->fb   = (rt_uint8_t *)rt_malloc_psram(pdata->fblen);
    RT_ASSERT(pdata->fb != RT_NULL);
    rt_memset(pdata->fb, 0, pdata->fblen);

    rt_thread_mdelay(1);

    RT_ASSERT(LV_HOR_RES == FUNC_WIN_FB_W);
    RT_ASSERT(LV_VER_RES <= FUNC_WIN_FB_H);
    RT_ASSERT(16 == LV_COLOR_DEPTH);

    /* background styles init */
    lv_style_init(&lv_app_list_data.main_style);
    lv_style_set_radius(&lv_app_list_data.main_style, LV_STATE_DEFAULT, 0/*LV_RADIUS_CIRCLE*/);
    lv_style_set_bg_color(&lv_app_list_data.main_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&lv_app_list_data.main_style, LV_STATE_DEFAULT, 0);

    /* date label styles init */
    lv_style_init(&lv_app_list_data.list_btn_style);
    lv_style_set_bg_opa(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, &lv_font_montserrat_44);
    lv_style_set_pad_top(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, 10);
    lv_style_set_pad_bottom(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, 11);
    lv_style_set_pad_left(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&lv_app_list_data.list_btn_style, LV_STATE_DEFAULT, 0);

    rt_uint16_t btn_hsize;
    rt_uint16_t btn_id = 0;
    rt_uint8_t  *fb = pdata->fb + FUNC_WIN_FB_W * (LV_VER_RES / 3) * LV_COLOR_DEPTH / 8;

    for (i = 0; i < APP_LIST_NUM; i++)
    {
        lv_clock_img_file_load(&lv_app_list_data.dsc[i], app_list[i].src);
    }

    while (1)
    {
        rt_uint16_t list_cnt = 0;

        lv_app_list_data.obj_main = lv_list_create(lv_scr_act(), NULL);
        lv_obj_add_style(lv_app_list_data.obj_main, LV_STATE_DEFAULT, &lv_app_list_data.main_style);
        lv_obj_set_size(lv_app_list_data.obj_main, LV_HOR_RES, LV_VER_RES);
        lv_obj_set_pos(lv_app_list_data.obj_main, 0, 0);
        lv_list_set_scrollbar_mode(lv_app_list_data.obj_main, LV_SCROLLBAR_MODE_OFF);

        btn_hsize = 0;
        while (1)
        {
            lv_app_list_data.list_btn[list_cnt] = lv_list_add_btn(lv_app_list_data.obj_main, &lv_app_list_data.dsc[btn_id], app_list[btn_id].title);
            lv_obj_add_style(lv_app_list_data.list_btn[list_cnt], LV_STATE_DEFAULT, &lv_app_list_data.list_btn_style);
            //rt_kprintf("id = %d, y = %d, h = %d\n", btn_id, lv_obj_get_y(lv_app_list_data.list_btn[list_cnt]), lv_obj_get_height(lv_app_list_data.list_btn[list_cnt]));

            btn_hsize = lv_obj_get_y(lv_app_list_data.list_btn[list_cnt]) + lv_obj_get_height(lv_app_list_data.list_btn[list_cnt]);
            if (btn_hsize > LV_VER_RES)
            {
                btn_hsize -= lv_obj_get_height(lv_app_list_data.list_btn[list_cnt]);
                break;
            }

            list_cnt++;
            if (++btn_id >= APP_LIST_NUM)
            {
                break;
            }
        }

        lv_refr_now(lv_disp_get_default());

        rt_thread_mdelay(1);

        lv_obj_del(lv_app_list_data.obj_main);

        rt_memcpy(fb, g_lvdata->fb, FUNC_WIN_FB_W * btn_hsize * LV_COLOR_DEPTH / 8);
        fb += FUNC_WIN_FB_W * btn_hsize * LV_COLOR_DEPTH / 8;

        if (btn_id >= APP_LIST_NUM)
        {
            break;
        }
    }

    for (i = 0; i < APP_LIST_NUM; i++)
    {
        lv_clock_img_dsc_free(&lv_app_list_data.dsc[i]);
    }
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
void app_func_list_init(void)
{
    rt_work_init(&init_work, app_func_list_init_work, RT_NULL);
    rt_workqueue_dowork(app_main_data->workqueue, &init_work);
}

