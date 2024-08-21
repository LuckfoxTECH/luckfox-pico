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
#define MENU_REGION_X      0
#define MENU_REGION_Y      0

#define MENU_WIN_XRES      DISP_XRES
#define MENU_WIN_YRES      DISP_YRES
#define MENU_WIN_FB_W      MENU_WIN_XRES
#define MENU_WIN_FB_H      (MENU_WIN_YRES * 4 + MENU_WIN_YRES * 2 / 3)

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
struct g_setting_menu_data_t *g_setting_menu_data = RT_NULL;
struct refrsh_param_t app_setting_menu_refrsh_param;

//static refrsh_request_param_t  app_setting_menu_refrsh_req_param;

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
static rt_int16_t move_updn_len  = 0;
static rt_int16_t move_updn_step = 10;
static rt_err_t app_setting_move_updn_design(void *param);
static design_cb_t setting_move_updn_design = { .cb = app_setting_move_updn_design, };
static refrsh_request_param_t setting_move_updn_refr_param;

static rt_err_t app_setting_move_updn_design(void *param)
{
    struct g_setting_menu_data_t *pdata = g_setting_menu_data;
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
        setting_move_updn_refr_param.wflag = (0x01 << app_setting_menu_refrsh_param.win_id) |
                                             (0x01 << alpha_win_refrsh_param.win_id);
        setting_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&setting_move_updn_refr_param);

        app_clock_design_request(0, &setting_move_updn_design, RT_NULL);
    }

    return RT_EOK;
}

static rt_err_t app_setting_move_lr_design(void *param);
static design_cb_t setting_move_lr_design = { .cb = app_setting_move_lr_design, };
static refrsh_request_param_t setting_move_lr_design_refr_param;

static rt_err_t app_setting_move_lr_design(void *param)
{
    struct g_setting_menu_data_t *appdata = g_setting_menu_data;
    struct g_alpha_win_data_t    *alphadata = g_alpha_win_data;

    appdata->mov_xoffset += TOUCH_MOVE_STEP * appdata->mov_xdir;
    alphadata->mov_xoffset = appdata->mov_xoffset;
    if (appdata->mov_xoffset >= MENU_WIN_XRES)
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

        setting_move_lr_design_refr_param.wflag = (0x01 << app_setting_main_refrsh_param.win_id);
        setting_move_lr_design_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&setting_move_lr_design_refr_param);
    }
    else if (appdata->mov_xoffset <= 0)
    {
        appdata->mov_xdir      = 0;
        appdata->mov_xoffset   = 0;
        alphadata->mov_xoffset = 0;
        setting_move_lr_design_refr_param.wflag = (0x01 << app_setting_main_refrsh_param.win_id) |
                (0x01 << app_setting_menu_refrsh_param.win_id) |
                (0x01 << alpha_win_refrsh_param.win_id);
        setting_move_lr_design_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&setting_move_lr_design_refr_param);
    }
    else
    {
        setting_move_lr_design_refr_param.wflag = (0x01 << app_setting_main_refrsh_param.win_id) |
                (0x01 << app_setting_menu_refrsh_param.win_id) |
                (0x01 << alpha_win_refrsh_param.win_id);
        setting_move_lr_design_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&setting_move_lr_design_refr_param);

        app_clock_design_request(0, &setting_move_lr_design, RT_NULL);
    }

    return RT_EOK;
}

static rt_err_t app_menu_touch_down(void *param)
{
    struct g_setting_menu_data_t *appdata = g_setting_menu_data;
    struct g_alpha_win_data_t  *alphadata = g_alpha_win_data;

    appdata->mov_xdir      = 0;
    appdata->mov_ydir      = 0;
    appdata->mov_xoffset   = 0;
    alphadata->mov_xoffset = 0;

    return RT_EOK;
}

//static refrsh_request_param_t move_lr_refrsh_request_param;
static rt_err_t app_menu_touch_move_lr(void *param)
{
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;
    struct g_setting_menu_data_t *appdata = g_setting_menu_data;
    struct g_alpha_win_data_t  *alphadata = g_alpha_win_data;

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
                       (0x01 << app_setting_menu_refrsh_param.win_id) |
                       (0x01 << alpha_win_refrsh_param.win_id);
    refr_param.wait = RT_WAITING_NO;

    return (app_clock_refresh_now(&refr_param));
}

//static refrsh_request_param_t move_updn_refrsh_request_param;
static rt_err_t app_menu_touch_move_updn(void *param)
{
    struct g_setting_menu_data_t *appdata = g_setting_menu_data;
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
    if (appdata->mov_yoffset >= MENU_WIN_FB_H - MENU_WIN_YRES)
    {
        appdata->mov_yoffset = MENU_WIN_FB_H - MENU_WIN_YRES;
    }

    refrsh_request_param_t refr_param;
    refr_param.wflag = (0x01 << app_setting_menu_refrsh_param.win_id) |
                       (0x01 << alpha_win_refrsh_param.win_id);
    refr_param.wait = RT_WAITING_NO;

    return (app_clock_refresh_now(&refr_param));
}

static refrsh_request_param_t move_up_refrsh_request_param;
static rt_err_t app_menu_touch_move_up(void *param)
{
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;
    struct g_setting_menu_data_t *appdata = g_setting_menu_data;

    if (pdata->dir_mode == TOUCH_DIR_MODE_LR)
    {
        app_clock_design_request(0, &setting_move_lr_design, RT_NULL);
    }
    else if (pdata->dir_mode == TOUCH_DIR_MODE_UPDN)
    {
        if (appdata->mov_yoffset <= 0)
        {
            appdata->mov_yoffset = 0;
            move_up_refrsh_request_param.wflag = (0x01 << app_setting_menu_refrsh_param.win_id) |
                                                 (0x01 << alpha_win_refrsh_param.win_id);
            move_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
            app_clock_refresh_request(&move_up_refrsh_request_param);

            goto exit;
        }
        if (appdata->mov_yoffset >= MENU_WIN_FB_H - MENU_WIN_YRES)
        {
            appdata->mov_yoffset = MENU_WIN_FB_H - MENU_WIN_YRES;
            move_up_refrsh_request_param.wflag = (0x01 << app_setting_menu_refrsh_param.win_id) |
                                                 (0x01 << alpha_win_refrsh_param.win_id);
            move_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
            app_clock_refresh_request(&move_up_refrsh_request_param);
            goto exit;
        }

        if (pdata->ydir == 1)
        {
            move_updn_len = appdata->mov_yoffset % (MENU_WIN_YRES / 3);
        }
        else //if (pdata->ydir == -1)
        {
            move_updn_len = (MENU_WIN_YRES / 3) - appdata->mov_yoffset % (MENU_WIN_YRES / 3);
        }

        move_updn_step = move_updn_len / 4;
        if (move_updn_step == 0) move_updn_step = 1;

        app_clock_design_request(0, &setting_move_updn_design, RT_NULL);
    }

exit:
    app_clock_touch_value_reset();

    return RT_EOK;
}

static refrsh_request_param_t touch_up_refrsh_request_param;
static rt_err_t app_menu_touch_up(void *param)
{
    struct app_main_data_t        *pdata = (struct app_main_data_t *)param;
    struct rt_touch_data   *p   = &pdata->cur_point[0];

    if ((MENU_WIN_YRES / 3 < p->y_coordinate) && (p->y_coordinate < (MENU_WIN_YRES * 2) / 3))
    {
        app_clock_touch_unregister();
        app_clock_touch_register(&app_dialog_box_touch_cb);

        // refresh win layers register
        app_setting_menu_refrsh_param.win_id    = APP_CLOCK_WIN_0;
        app_setting_menu_refrsh_param.win_layer = WIN_BOTTOM_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_0, app_setting_menu_refrsh, &app_setting_menu_refrsh_param);

        alpha_win_refrsh_param.win_id    = APP_CLOCK_WIN_2;
        alpha_win_refrsh_param.win_layer = WIN_MIDDLE_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_2, alpha_win_refresh, &alpha_win_refrsh_param);

        app_dialog_box_refrsh_param.win_id    = APP_CLOCK_WIN_1;
        app_dialog_box_refrsh_param.win_layer = WIN_TOP_LAYER;
        app_clock_refresh_register(APP_CLOCK_WIN_1, app_dialog_box_refrsh, &app_dialog_box_refrsh_param);

        touch_up_refrsh_request_param.wflag = (0x01 << app_dialog_box_refrsh_param.win_id);
        touch_up_refrsh_request_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&touch_up_refrsh_request_param);

        g_dialog_box_data->mode = 1;

        app_clock_touch_value_reset();
    }

    return RT_EOK;
}
struct app_main_touch_cb_t app_setting_menu_touch_cb =
{
    .tp_touch_down      = app_menu_touch_down,
    .tp_move_lr         = app_menu_touch_move_lr,
    .tp_move_updn       = app_menu_touch_move_updn,
    .tp_move_up         = app_menu_touch_move_up,
    .tp_touch_up        = app_menu_touch_up,
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
rt_err_t app_setting_menu_refrsh(struct rt_display_config *wincfg, void *param)
{
    struct rt_device_graphic_info *info = &app_main_data->disp->info;
    struct g_setting_menu_data_t *pdata = g_setting_menu_data;
    struct refrsh_param_t *par = (struct refrsh_param_t *)param;

    wincfg->winId   = par->win_id;
    wincfg->zpos    = par->win_layer;
    //wincfg->colorkey = COLOR_KEY_EN | 0x5555;
    wincfg->format  = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut     = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->fb    = pdata->fb    + (pdata->mov_yoffset * MENU_WIN_FB_W + pdata->mov_xoffset) * sizeof(rt_uint16_t);
    wincfg->fblen = pdata->fblen - (pdata->mov_yoffset * MENU_WIN_FB_W + pdata->mov_xoffset) * sizeof(rt_uint16_t);
    wincfg->xVir  = MENU_WIN_FB_W;
    wincfg->w     = MENU_WIN_XRES - pdata->mov_xoffset;
    wincfg->h     = MENU_WIN_YRES;
    wincfg->x     = MENU_REGION_X + ((info->width  - MENU_WIN_XRES) / 2);
    wincfg->y     = MENU_REGION_Y + ((info->height - MENU_WIN_YRES) / 2);
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
#define SETTING_MENU_NUM  12
struct lv_setting_menu_data_t
{
    lv_style_t main_style;
    lv_style_t list_btn_style;

    lv_obj_t   *obj_main;
    lv_obj_t   *list_btn[SETTING_MENU_NUM];
    lv_img_dsc_t dsc[SETTING_MENU_NUM];
};
static struct lv_setting_menu_data_t lv_setting_menu_data;
static struct rt_work init_work;

struct lv_setting_menu_src_t
{
    const char *src;
    const char *title;
};

static const struct lv_setting_menu_src_t menu_list[SETTING_MENU_NUM] =
{
    {"L:/alarm_clock.bin",          " Alarm"},
    {"L:/sleep.bin",                " Sleep"},
    {"L:/no_disturb.bin",           " Undisturb"},
    {"L:/screen_display.bin",       " Screen"},
    {"L:/flashlight.bin",           " FlashLight"},
    {"L:/find_phone.bin",           " Find phone"},
    {"L:/breathing_training.bin",   " Breathing"},
    {"L:/activity_record.bin",      " Activity"},
    {"L:/exercise_record.bin",      " Exercise"},
    {"L:/training_status.bin",      " Training"},
    {"L:/user_guide.bin",           " User Guide"},
    {"L:/about_watche.bin",         " Aboult"},
};

static void app_lv_setting_menu_init_work(struct rt_work *work, void *work_data)
{
    struct g_setting_menu_data_t *pdata;
    rt_uint16_t i;

    g_setting_menu_data = pdata = (struct g_setting_menu_data_t *)rt_malloc(sizeof(struct g_setting_menu_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct g_setting_menu_data_t));

    /* framebuffer malloc */
    pdata->fblen = MENU_WIN_FB_W * MENU_WIN_FB_H * LV_COLOR_DEPTH / 8;
    pdata->fb   = (rt_uint8_t *)rt_malloc_psram(pdata->fblen);
    RT_ASSERT(pdata->fb != RT_NULL);
    rt_memset(pdata->fb, 0, pdata->fblen);

    rt_thread_mdelay(1);

    RT_ASSERT(LV_HOR_RES == MENU_WIN_FB_W);
    RT_ASSERT(LV_VER_RES <= MENU_WIN_FB_H);
    RT_ASSERT(16 == LV_COLOR_DEPTH);

    /* background styles init */
    lv_style_init(&lv_setting_menu_data.main_style);
    lv_style_set_radius(&lv_setting_menu_data.main_style, LV_STATE_DEFAULT, 0/*LV_RADIUS_CIRCLE*/);
    lv_style_set_bg_color(&lv_setting_menu_data.main_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&lv_setting_menu_data.main_style, LV_STATE_DEFAULT, 0);

    /* date label styles init */
    lv_style_init(&lv_setting_menu_data.list_btn_style);
    lv_style_set_bg_opa(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, &lv_font_montserrat_44);
    lv_style_set_pad_top(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, 10);
    lv_style_set_pad_bottom(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, 11);
    lv_style_set_pad_left(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&lv_setting_menu_data.list_btn_style, LV_STATE_DEFAULT, 0);

    rt_uint16_t btn_hsize;
    rt_uint16_t btn_id = 0;
    rt_uint8_t  *fb = pdata->fb + MENU_WIN_FB_W * (LV_VER_RES / 3) * LV_COLOR_DEPTH / 8;

    for (i = 0; i < SETTING_MENU_NUM; i++)
    {
        lv_clock_img_file_load(&lv_setting_menu_data.dsc[i], menu_list[i].src);
    }

    while (1)
    {
        rt_uint16_t list_cnt = 0;

        lv_setting_menu_data.obj_main = lv_list_create(lv_scr_act(), NULL);
        lv_obj_add_style(lv_setting_menu_data.obj_main, LV_STATE_DEFAULT, &lv_setting_menu_data.main_style);
        lv_obj_set_size(lv_setting_menu_data.obj_main, LV_HOR_RES, LV_VER_RES);
        lv_obj_set_pos(lv_setting_menu_data.obj_main, 0, 0);
        lv_list_set_scrollbar_mode(lv_setting_menu_data.obj_main, LV_SCROLLBAR_MODE_OFF);

        btn_hsize = 0;
        while (1)
        {
            lv_setting_menu_data.list_btn[list_cnt] = lv_list_add_btn(lv_setting_menu_data.obj_main, &lv_setting_menu_data.dsc[btn_id], menu_list[btn_id].title);
            lv_obj_add_style(lv_setting_menu_data.list_btn[list_cnt], LV_STATE_DEFAULT, &lv_setting_menu_data.list_btn_style);
            //rt_kprintf("id = %d, y = %d, h = %d\n", btn_id, lv_obj_get_y(lv_setting_menu_data.list_btn[list_cnt]), lv_obj_get_height(lv_setting_menu_data.list_btn[list_cnt]));

            btn_hsize = lv_obj_get_y(lv_setting_menu_data.list_btn[list_cnt]) + lv_obj_get_height(lv_setting_menu_data.list_btn[list_cnt]);
            if (btn_hsize > LV_VER_RES)
            {
                btn_hsize -= lv_obj_get_height(lv_setting_menu_data.list_btn[list_cnt]);
                break;
            }

            list_cnt++;
            if (++btn_id >= SETTING_MENU_NUM)
            {
                break;
            }
        }

        lv_refr_now(lv_disp_get_default());

        rt_thread_mdelay(1);

        lv_obj_del(lv_setting_menu_data.obj_main);

        rt_memcpy(fb, g_lvdata->fb, MENU_WIN_FB_W * btn_hsize * LV_COLOR_DEPTH / 8);
        fb += MENU_WIN_FB_W * btn_hsize * LV_COLOR_DEPTH / 8;

        if (btn_id >= SETTING_MENU_NUM)
        {
            break;
        }
    }

    for (i = 0; i < SETTING_MENU_NUM; i++)
    {
        lv_clock_img_dsc_free(&lv_setting_menu_data.dsc[i]);
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
void app_setting_menu_init(void)
{
    rt_work_init(&init_work, app_lv_setting_menu_init_work, RT_NULL);
    rt_workqueue_dowork(app_main_data->workqueue, &init_work);
}

