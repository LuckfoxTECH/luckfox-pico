#include <rtthread.h>
#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "hal_base.h"
#include "image_info.h"
#include "display.h"

#if defined(RT_USING_TOUCH_DRIVERS)
#include "touch.h"
#include "touchpanel.h"
#endif

#include "app_main.h"
#include "lib_imageprocess.h"

#ifdef RT_USING_FWANALYSIS
#include "rkpart.h"
#endif

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */
#define CLOCK_MAX_BUF_ID    3
#define CLOCK_STYLE_MAX_NUM 4
#define CLOCK_STYLE_FOC_ID  (CLOCK_MAX_BUF_ID / 2)

/* display region define */
#define CLOCK_REGION_X      0
#define CLOCK_REGION_Y      0

#define CLOCK_WIN_COLOR_DEPTH       16
#define CLOCK_WIN_XRES      DISP_XRES
#define CLOCK_WIN_YRES      DISP_YRES
#define CLOCK_WIN_FB_W      (CLOCK_WIN_XRES * CLOCK_MAX_BUF_ID)
#define CLOCK_WIN_FB_H      CLOCK_WIN_YRES

/* 0: Disable, 1 : Include backgroud loading, 2 : Just rotate and blit */
#define CLOCK_SHOW_FPS      0
#ifdef NEW_ROTATE_ALGORITHM
#define CLOCK_FAST_RGB565   1
#else
#define CLOCK_FAST_RGB565   0
#endif

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
extern image_info_t img_clk0_hour_info;
extern image_info_t img_clk0_min_info;
extern image_info_t img_clk0_sec_info;
extern image_info_t img_clk0_center_info;

extern image_info_t img_clk1_hour_info;
extern image_info_t img_clk1_min_info;
extern image_info_t img_clk1_sec_info;

extern image_info_t img_clk2_hour_info;
extern image_info_t img_clk2_min_info;
extern image_info_t img_clk2_sec_info;

extern image_info_t img_heart_info;

static rt_err_t app_clock_design_next(void *param);
static rt_err_t app_clock_design_all(void *param);
static rt_err_t app_clock_move_lr_design(void *param);
static rt_err_t app_clock_move_updn_design(void *param);
static rt_err_t app_clock_smooth_design(void *param);

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct app_clock_data_t  *app_clock_data = RT_NULL;
struct refrsh_param_t    app_clock_refrsh_param;

struct clock_needle_img_t
{
    image_info_t *info;
    float cx;
    float cy;
};

struct clock_needle_grp_t
{
    struct clock_needle_img_t hour;
    struct clock_needle_img_t min;
    struct clock_needle_img_t sec;
    struct clock_needle_img_t cen;
};

#ifndef NEW_ROTATE_ALGORITHM
static const struct clock_needle_grp_t needle_img[CLOCK_STYLE_MAX_NUM] =
{
    {{&img_clk0_hour_info, 5.0, 5.0}, {&img_clk0_min_info, 2.0, 2.0}, {&img_clk0_sec_info, 2.0, 2.0}, {&img_clk0_center_info, 6.0, 6.0}},
    {{&img_clk0_hour_info, 5.0, 5.0}, {&img_clk0_min_info, 2.0, 2.0}, {&img_clk0_sec_info, 2.0, 2.0}, {&img_clk0_center_info, 6.0, 6.0}},
    {{&img_clk0_hour_info, 5.0, 5.0}, {&img_clk0_min_info, 2.0, 2.0}, {&img_clk0_sec_info, 2.0, 2.0}, {&img_clk0_center_info, 6.0, 6.0}},
    {{NULL, 0.0, 0.0}, {NULL, 0.0, 0.0}, {NULL, 0.0, 0.0}, {NULL, 0.0, 0.0}},
};
#else
static const struct clock_needle_grp_t needle_img[CLOCK_STYLE_MAX_NUM] =
{
    {{&img_clk0_hour_info, 6.0, 6.0}, {&img_clk0_min_info, 6.0, 6.0}, {&img_clk0_sec_info, 19.0, 6.0}, {&img_clk0_center_info, 6.0, 6.0}},
    {{&img_clk1_hour_info, 1, 4.0}, {&img_clk1_min_info, 1, 4.0}, {&img_clk1_sec_info, 29.0, 6.0}, {&img_clk0_center_info, 6.0, 6.0}},
    {{&img_clk2_hour_info, -10.0, 6.0}, {&img_clk2_min_info, -10.0, 4.0}, {&img_clk2_sec_info, 6.0, 6.0}, {&img_clk0_center_info, 6.0, 6.0}},
    {{NULL, 0.0, 0.0}, {NULL, 0.0, 0.0}, {NULL, 0.0, 0.0}, {NULL, 0.0, 0.0}},
};
#endif

static const img_load_info_t img_clk_bkg[CLOCK_STYLE_MAX_NUM] =
{
    { 368, 448, "img_clk0_bkg.dta"},
    { 368, 448, "img_clk1_bkg.dta"},
    { 368, 448, "img_clk2_bkg.dta"},
    { 368, 448, "img_heartrate_bkg.dta"},
};

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
#if CLOCK_FAST_RGB565
struct pre_info
{
    short info[DISP_YRES][2];
    uint8_t min;
};
struct pre_info preInfo[3] = {{{{0}, {0}}, -1}, {{{0}, {0}}, -1}, {{{0}, {0}}, -1}};
static struct rotateimage_st pbg;
#endif
#define HEART_ICON_X    66
#define HEART_ICON_Y    116
static int scale_val_grp[] = {32, 24, 20, 16, 12, 8, 4, 0, 4, 8, 12, 16, 20, 24};
static int scale_index = 0;
static rt_err_t app_clock_design(void *param)
{
    clock_design_param_t *par = (clock_design_param_t *)param;
    clock_time_t *time = &app_main_data->tmr_data;
    rt_uint8_t   *fb   = app_clock_data->top_fb;

    par->buf_id = par->buf_id % CLOCK_MAX_BUF_ID;
    rt_int8_t     style_id = app_clock_data->cur_style + (par->buf_id - 1);
    if (style_id < 0) style_id += CLOCK_STYLE_MAX_NUM;
    if (style_id >= CLOCK_STYLE_MAX_NUM) style_id -= CLOCK_STYLE_MAX_NUM;

    image_info_t *img_hour = needle_img[style_id].hour.info;
    image_info_t *img_min  = needle_img[style_id].min.info;
    image_info_t *img_sec  = needle_img[style_id].sec.info;
#ifndef NEW_ROTATE_ALGORITHM
    image_info_t *img_cen  = needle_img[style_id].cen.info;
#endif

    rt_uint16_t   xstart   = (rt_uint16_t)(par->buf_id) * CLOCK_WIN_XRES;
    static uint8_t pre_style = -1;
    int32_t hour;
    int32_t angle;

#if CLOCK_SHOW_FPS > 0
    uint32_t st, et;
#endif

#if CLOCK_SHOW_FPS == 1
    st = HAL_GetTick();
#endif

    if (style_id == 3)
    {
        struct image_st ips, ipd;
        rt_uint8_t *win_fb;
        rt_uint8_t *bak_fb = app_clock_data->bak_fb;

        ips.width = img_heart_info.w;
        ips.height = img_heart_info.h;
        ips.stride = ips.width * 4;
        ips.pdata = img_heart_info.data;

        ipd.width = img_heart_info.w - scale_val_grp[scale_index];
        ipd.height = img_heart_info.h - scale_val_grp[scale_index];
        ipd.stride = CLOCK_WIN_FB_W * 2;
        ipd.pdata = fb + xstart * 2 + (HEART_ICON_Y + scale_val_grp[scale_index] / 2) * ipd.stride + (HEART_ICON_X + scale_val_grp[scale_index] / 2) * 2;

        win_fb = fb + xstart * 2 + HEART_ICON_Y * ipd.stride + HEART_ICON_X * 2;

        if ((par->buf_id != 1) || (par->buf_id == 1 && pre_style != app_clock_data->cur_style))
        {
            app_clock_load_img((img_load_info_t *)&img_clk_bkg[style_id], (rt_uint8_t *)fb, CLOCK_WIN_FB_W, CLOCK_WIN_FB_H, xstart, 2);
            for (int i = 0; i < img_heart_info.h; i++)
                memcpy(bak_fb + i * ips.width * 2, win_fb + i * ipd.stride, ips.width * 2);
        }
        else
        {
            for (int i = 0; i < img_heart_info.h; i++)
                memcpy(win_fb + i * ipd.stride, bak_fb + i * ips.width * 2, ips.width * 2);
        }

        rk_scale_process(&ips, &ipd, TYPE_ARGB8888_565);
        scale_index++;
        if (scale_index >= (sizeof(scale_val_grp) / sizeof(int)))
            scale_index = 0;

        goto DESIGN_EXIT;
    }

    //get bottom area
#if !CLOCK_FAST_RGB565
    app_clock_load_img((img_load_info_t *)&img_clk_bkg[style_id], (rt_uint8_t *)fb, CLOCK_WIN_FB_W, CLOCK_WIN_FB_H, xstart, 2);
#endif

#if CLOCK_SHOW_FPS == 2
    st = HAL_GetTick();
#endif

#ifndef NEW_ROTATE_ALGORITHM
    //draw clock needles
    rt_uint16_t xoffset = (CLOCK_WIN_XRES / 2) + xstart;
    rt_uint16_t yoffset = (CLOCK_WIN_YRES / 2);

    //draw hour line
    hour = time->hour;
    if (hour >= 12) hour -= 12;
    angle = hour * (360 / 12) + (time->minute * 30) / 60 - 90;
    if (angle < 0) angle += 360;
    rt_display_rotate_16bit((float)angle, img_hour->w, img_hour->h, (unsigned short *)img_hour->data,
                            (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_WIN_FB_W + xoffset)),
                            CLOCK_WIN_FB_W, 0, img_hour->h / 2);

    //draw min line
    angle = time->minute * (360 / 60) - 90;
    if (angle < 0) angle += 360;
    rt_display_rotate_16bit((float)angle, img_min->w, img_min->h, (unsigned short *)img_min->data,
                            (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_WIN_FB_W + xoffset)),
                            CLOCK_WIN_FB_W, 0, img_min->h / 2);

    //draw second line
    angle  = time->second * (360 / 60) - 90;
    if (angle < 0) angle += 360;
    rt_display_rotate_16bit((float)angle, img_sec->w, img_sec->h, (unsigned short *)img_sec->data,
                            (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_WIN_FB_W + xoffset)),
                            CLOCK_WIN_FB_W, 0, img_sec->h / 2);

    //draw centre
    yoffset  -= img_cen->h / 2;
    xoffset  -= img_cen->w / 2;
    rt_display_img_fill(img_cen, fb, CLOCK_WIN_FB_W, xoffset, yoffset);
#else
    struct rotateimage_st ps, pd;
    float h_cx = needle_img[style_id].hour.cx;
    float h_cy = needle_img[style_id].hour.cy;
    float m_cx = needle_img[style_id].min.cx;
    float m_cy = needle_img[style_id].min.cy;
    float s_cx = needle_img[style_id].sec.cx;
    float s_cy = needle_img[style_id].sec.cy;

    pd.width = CLOCK_WIN_XRES;
    pd.height = CLOCK_WIN_YRES;
    pd.stride = CLOCK_WIN_FB_W * 2;
    pd.cx = pd.width / 2;
    pd.cy = pd.height / 2;
    pd.pdata = fb + xstart * 2;

#if !CLOCK_FAST_RGB565
    ps.width = img_hour->w;
    ps.height = img_hour->h;
    ps.stride = ps.width * 4;
    ps.cx = h_cx;
    ps.cy = h_cy;
    ps.pdata = img_hour->data;

    hour = time->hour;
    if (hour >= 12)
    {
        hour -= 12;
    }
    angle = hour * (360 / 12) + (time->minute * 30) / 60 - 90;
    if (angle < 0)
    {
        angle += 360;
    }
    rk_rotate_process_16bit(&ps, &pd, (360 - angle % 360));

    ps.width = img_min->w;
    ps.height = img_min->h;
    ps.stride = ps.width * 4;
    ps.cx = m_cx;
    ps.cy = m_cy;
    ps.pdata = img_min->data;

    angle = time->minute * (360 / 60) - 90;
    if (angle < 0)
    {
        angle += 360;
    }
    rk_rotate_process_16bit(&ps, &pd, (360 - angle % 360));
#else
    pbg.pdata = app_clock_data->bottom_fb + xstart * 2;
    if (!app_clock_data->switch_done ||
            preInfo[style_id].min != time->minute)
    {
        preInfo[style_id].min = time->minute;
        memset(preInfo[style_id].info[0], 0, DISP_YRES * 2 * sizeof(short));
        pbg.width = CLOCK_WIN_XRES;
        pbg.height = CLOCK_WIN_YRES;
        pbg.stride = CLOCK_WIN_FB_W * 2;
        pbg.cx = pbg.width / 2;
        pbg.cy = pbg.height / 2;

        ps.width = img_hour->w;
        ps.height = img_hour->h;
        ps.stride = ps.width * 4;
        ps.cx = h_cx;
        ps.cy = h_cy;
        ps.pdata = img_hour->data;

        app_clock_load_img((img_load_info_t *)&img_clk_bkg[style_id], (rt_uint8_t *)pbg.pdata, CLOCK_WIN_FB_W, CLOCK_WIN_FB_H, 0, 2);
        hour = time->hour;
        if (hour >= 12)
        {
            hour -= 12;
        }
        angle = hour * (360 / 12) + (time->minute * 30) / 60 - 90;
        if (angle < 0)
        {
            angle += 360;
        }
        rk_rotate_process_16bit(&ps, &pbg, (360 - angle % 360));

        ps.width = img_min->w;
        ps.height = img_min->h;
        ps.stride = ps.width * 4;
        ps.cx = m_cx;
        ps.cy = m_cy;
        ps.pdata = img_min->data;

        angle = time->minute * (360 / 60) - 90;
        if (angle < 0)
        {
            angle += 360;
        }
        rk_rotate_process_16bit(&ps, &pbg, (360 - angle % 360));
        for (int i = 0; i < pd.height; i++)
            memcpy(pd.pdata + i * pd.stride, pbg.pdata + i * pbg.stride, pbg.width * 2);
    }
#endif
    ps.width = img_sec->w;
    ps.height = img_sec->h;
    ps.stride = ps.width * 4;
    ps.cx = s_cx;
    ps.cy = s_cy;
    ps.pdata = img_sec->data;

    angle = time->second * (360 / 60) - 90;
    if (angle < 0)
    {
        angle += 360;
    }
#if !CLOCK_FAST_RGB565
    rk_rotate_process_16bit(&ps, &pd, (360 - angle % 360));
#else
    rk_rotate_process_16bitfast(&ps, &pbg, &pd, preInfo[style_id].info[0], (360 - angle % 360));
#endif
#endif

DESIGN_EXIT:

    if (par->buf_id == (CLOCK_MAX_BUF_ID - 1))
        app_clock_data->switch_done = 1;
    if (par->buf_id == 1)
        pre_style = app_clock_data->cur_style;

#if CLOCK_SHOW_FPS > 0
    et = HAL_GetTick();
    rt_kprintf("FPS:%d(%dms)\n", 1000 / (et - st), et - st);
#endif

    return RT_EOK;
}

/**
 * top layer refresh.
 */
rt_err_t app_clock_win_refresh(struct rt_display_config *wincfg, void *param)
{
    struct rt_device_graphic_info *info = &app_main_data->disp->info;
    struct app_clock_data_t *pdata = app_clock_data;
    struct refrsh_param_t *par = (struct refrsh_param_t *)param;

    wincfg->winId   = par->win_id;
    wincfg->zpos    = par->win_layer;
    //wincfg->colorkey = COLOR_KEY_EN | 0;
    wincfg->format  = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut     = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->fb    = pdata->top_fb    + (CLOCK_WIN_XRES + pdata->mov_offset) * sizeof(rt_uint16_t);
    wincfg->fblen = pdata->top_fblen - (CLOCK_WIN_XRES + pdata->mov_offset) * sizeof(rt_uint16_t);
    wincfg->xVir  = CLOCK_WIN_FB_W;
    wincfg->w     = CLOCK_WIN_XRES;
    wincfg->h     = CLOCK_WIN_YRES;
    wincfg->x     = CLOCK_REGION_X + ((info->width  - CLOCK_WIN_XRES) / 2);
    wincfg->y     = CLOCK_REGION_Y + ((info->height - CLOCK_WIN_YRES) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * CLOCK_WIN_COLOR_DEPTH) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

static design_cb_t          clk_design_next_cb    = {.cb = app_clock_design_next,};
static clock_design_param_t clk_design_next_param = {.buf_id = 0};
static rt_err_t app_clock_design_next(void *param)
{
    app_clock_design(&clk_design_next_param);

    clk_design_next_param.buf_id++;
    if (clk_design_next_param.buf_id == 1)
    {
        clk_design_next_param.buf_id++;
    }

    if (clk_design_next_param.buf_id < CLOCK_MAX_BUF_ID)
    {
        app_clock_design_request(0, &clk_design_next_cb, &clk_design_next_param);
    }

    return RT_EOK;
}

static design_cb_t          clk_design_all_cb    = {.cb = app_clock_design_all,};
static clock_design_param_t clk_design_all_param = {.buf_id = 0};
static rt_err_t app_clock_design_all(void *param)
{
    rt_int8_t style_id;

    clk_design_all_param.buf_id = 1;
    style_id = (app_clock_data->cur_style + clk_design_all_param.buf_id - 1);

    if (style_id < 0)
    {
        style_id += CLOCK_STYLE_MAX_NUM;
    }

    if (style_id >= CLOCK_STYLE_MAX_NUM)
    {
        style_id -= CLOCK_STYLE_MAX_NUM;
    }

    if (style_id != 3)
    {
        app_clock_design(&clk_design_all_param);
    }

    if (app_main_data->xoffset > 0)
    {
        clk_design_all_param.buf_id = 0;

        style_id--;
        if (style_id < 0)
        {
            style_id = CLOCK_STYLE_MAX_NUM - 1;
        }

        if (style_id != 3)
        {
            app_clock_design(&clk_design_all_param);
        }
    }

    if (app_main_data->xoffset < 0)
    {
        clk_design_all_param.buf_id = 2;

        style_id++;

        if (style_id >= CLOCK_STYLE_MAX_NUM)
        {
            style_id = 0;
        }

        if (style_id != 3)
        {
            app_clock_design(&clk_design_all_param);
        }
    }

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
static rt_uint8_t  clock_second_bk = 60;
static rt_uint8_t  clock_minute_bk = 60;

static design_cb_t            clkupd_design = {.cb = app_clock_design,};
static clock_design_param_t   clkupd_design_param;
static refrsh_request_param_t clkupd_refrsh_request_param;
void app_clock_update(void)
{
    if ((clock_second_bk == app_main_data->tmr_data.second) &&
            (app_clock_data->cur_style != 3))
    {
        return;
    }
    clock_second_bk = app_main_data->tmr_data.second;

    if (app_main_data->dir_mode != TOUCH_DIR_MODE_NULL)
    {
        if (app_main_data->dir_mode == TOUCH_DIR_MODE_LR)
        {
            app_clock_design_request(0, &clk_design_all_cb, &clk_design_all_param);

            clkupd_refrsh_request_param.wflag = 0x01 << app_clock_refrsh_param.win_id;
            clkupd_refrsh_request_param.wait = RT_WAITING_FOREVER;
            app_clock_refresh_request(&clkupd_refrsh_request_param);
        }
    }
    else
    {
        clkupd_design_param.buf_id = 1;
        app_clock_design_request(0, &clkupd_design, &clkupd_design_param);

        clkupd_refrsh_request_param.wflag = 0x01 << app_clock_refrsh_param.win_id;
        clkupd_refrsh_request_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&clkupd_refrsh_request_param);
    }
    if (clock_minute_bk != app_main_data->tmr_data.minute)
    {
        clock_minute_bk = app_main_data->tmr_data.minute;

        clk_design_next_param.buf_id = 0;
        app_clock_design_request(0, &clk_design_next_cb, &clk_design_next_param);
    }
}

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

static design_cb_t            clock_move_lr_design = { .cb = app_clock_move_lr_design, };
static refrsh_request_param_t clock_move_lr_refr_param;
static rt_err_t app_clock_move_lr_design(void *param)
{
    struct app_clock_data_t *pdata = app_clock_data;

    pdata->mov_offset += TOUCH_MOVE_STEP * pdata->mov_dir;
    if (ABS(pdata->mov_offset) >= (int)param)
    {
        pdata->cur_style += (int)param == 0 ? 0 : pdata->mov_dir;
        if (pdata->cur_style < 0)
        {
            pdata->cur_style += CLOCK_STYLE_MAX_NUM;
        }
        if (pdata->cur_style >= CLOCK_STYLE_MAX_NUM)
        {
            pdata->cur_style -= CLOCK_STYLE_MAX_NUM;
        }
        write_device_sn(DEV_VENDOR_KEY, (char *)&pdata->cur_style, 1);

        pdata->mov_dir      = 0;
        pdata->mov_offset   = 0;

        app_clock_data->switch_done = 0;

        if ((int)param != 0)
        {
            clock_design_param_t tmp_param;
            tmp_param.buf_id = 1;
            app_clock_design(&tmp_param);
        }

        clk_design_next_param.buf_id = 0;
        app_clock_design_request(0, &clk_design_next_cb, &clk_design_next_param);

        app_clock_timer_cb_register(app_clock_update);

    }
    else
    {
        app_clock_design_request(0, &clock_move_lr_design, (void *)CLOCK_WIN_XRES);
    }

    clock_move_lr_refr_param.wflag = 0x01 << app_clock_refrsh_param.win_id;
    clock_move_lr_refr_param.wait = RT_WAITING_FOREVER;
    app_clock_refresh_request(&clock_move_lr_refr_param);

    return RT_EOK;
}

static design_cb_t            clock_move_updn_design = { .cb = app_clock_move_updn_design, };
static refrsh_request_param_t clock_move_updn_refr_param;
static rt_err_t app_clock_move_updn_design(void *param)
{
    struct app_setting_main_data_t *pdata = g_setting_main_data;

    pdata->mov_offset += TOUCH_MOVE_STEP * pdata->mov_dir;
    if (pdata->mov_offset >= CLOCK_WIN_YRES)
    {

        pdata->mov_dir     = 0;
        pdata->mov_offset  = CLOCK_WIN_YRES;

        app_clock_touch_unregister();
        app_clock_touch_register(&app_setting_main_touch_cb);

        clock_move_updn_refr_param.wflag = 0x01 << app_setting_main_refrsh_param.win_id;
        clock_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&clock_move_updn_refr_param);
    }
    else if (pdata->mov_offset <= 0)
    {
        pdata->mov_dir     = 0;
        pdata->mov_offset  = 0;

        app_clock_timer_cb_register(app_clock_update);

        clock_move_updn_refr_param.wflag = 0x01 << app_clock_refrsh_param.win_id;
        clock_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&clock_move_updn_refr_param);
    }
    else
    {
        app_clock_design_request(0, &clock_move_updn_design, RT_NULL);

        clock_move_updn_refr_param.wflag = (0x01 << app_clock_refrsh_param.win_id) | (0x01 << app_setting_main_refrsh_param.win_id);
        clock_move_updn_refr_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&clock_move_updn_refr_param);
    }

    return RT_EOK;
}

static int app_clock_smooth_design_enable = 0;

static design_cb_t            clock_smooth_design = { .cb = app_clock_smooth_design, };
static refrsh_request_param_t clock_smooth_refrsh_requset_param;
static rt_err_t app_clock_smooth_design(void *param)
{
    struct app_clock_data_t *appdata = app_clock_data;
    struct app_setting_main_data_t *setdata = g_setting_main_data;
    struct app_main_data_t *pdata = app_main_data;
    uint32_t tick;

    if (pdata->dir_mode && pdata->mov_speed)
    {
        tick = HAL_GetTick() - pdata->down_timestamp;
        pdata->down_timestamp = HAL_GetTick();
        pdata->mov_fix = ((int32_t)tick * pdata->mov_speed) / 1000;
        pdata->mov_speed /= 2;

        if ((rt_uint32_t)param == (0x01 << app_clock_refrsh_param.win_id))
        {
            appdata->mov_offset = -(pdata->xoffset + pdata->mov_fix);
            if (appdata->mov_offset <= -CLOCK_WIN_XRES)
            {
                appdata->mov_offset = -CLOCK_WIN_XRES;
            }
            if (appdata->mov_offset >= CLOCK_WIN_XRES)
            {
                appdata->mov_offset = CLOCK_WIN_XRES;
            }
        }
        if ((rt_uint32_t)param == (0x01 << app_clock_refrsh_param.win_id | 0x01 << app_setting_main_refrsh_param.win_id))
        {
            setdata->mov_offset = (pdata->yoffset + pdata->mov_fix);
            if (setdata->mov_offset <= 1)
            {
                setdata->mov_offset = 1;
            }
            if (setdata->mov_offset >= CLOCK_WIN_YRES)
            {
                setdata->mov_offset = CLOCK_WIN_YRES;
            }
        }
        clock_smooth_refrsh_requset_param.wflag = (rt_uint32_t)param;
        clock_smooth_refrsh_requset_param.wait = RT_WAITING_FOREVER;
        app_clock_refresh_request(&clock_smooth_refrsh_requset_param);

        app_clock_design_request(0, &clock_smooth_design, param);
    }
    else
    {
        app_clock_smooth_design_enable = 0;
    }

    return RT_EOK;
}

static rt_err_t app_clock_touch_down(void *param)
{
    //struct app_main_data_t *pdata = (struct app_main_data_t *)param;

    // app_clock_timer_cb_unregister();
    app_clock_design_request(1, &clk_design_all_cb, &clk_design_all_param);

    return RT_EOK;
}

static rt_err_t app_clock_touch_up(void *param)
{
    //struct app_main_data_t *pdata = (struct app_main_data_t *)param;

    // app_clock_timer_cb_register(app_clock_update);

    return RT_EOK;
}

static rt_err_t app_clock_touch_move_lr(void *param)
{
    struct app_main_data_t  *pdata = (struct app_main_data_t *)param;
    struct app_clock_data_t *appdata  = app_clock_data;

    appdata->mov_dir    = -pdata->xdir;
    appdata->mov_offset = -(pdata->xoffset + pdata->mov_fix);

    if (appdata->mov_offset <= -CLOCK_WIN_XRES)
    {
        appdata->mov_offset = -CLOCK_WIN_XRES;
    }
    if (appdata->mov_offset >= CLOCK_WIN_XRES)
    {
        appdata->mov_offset = CLOCK_WIN_XRES;
    }

    if (app_clock_smooth_design_enable == 0)
    {
        app_clock_smooth_design_enable = 1;
        app_clock_design_request(0, &clock_smooth_design, (void *)(0x01 << app_clock_refrsh_param.win_id));
    }

    return RT_EOK;
}

static rt_err_t app_clock_touch_move_updown(void *param)
{
    struct app_main_data_t        *pdata = (struct app_main_data_t *)param;
    struct app_setting_main_data_t *appdata = g_setting_main_data;

    appdata->mov_dir = pdata->ydir;
    appdata->mov_offset = pdata->yoffset - pdata->mov_fix;

    if (appdata->mov_offset <= 1)
    {
        appdata->mov_offset = 1;
        return RT_EOK;
    }
    if (appdata->mov_offset >= CLOCK_WIN_YRES)
    {
        appdata->mov_offset = CLOCK_WIN_YRES;
        return RT_EOK;
    }

    if (app_clock_smooth_design_enable == 0)
    {
        app_clock_smooth_design_enable = 1;
        app_clock_design_request(0, &clock_smooth_design, (void *)(0x01 << app_clock_refrsh_param.win_id | 0x01 << app_setting_main_refrsh_param.win_id));
    }

    return RT_EOK;
}

static rt_err_t app_clock_touch_moveup(void *param)
{
    struct app_main_data_t *pdata = (struct app_main_data_t *)param;
    struct rt_touch_data *cur_p   = &pdata->cur_point[0];
    struct rt_touch_data *down_p   = &pdata->down_point[0];

    app_clock_smooth_design_enable = 0;

    if (pdata->dir_mode == TOUCH_DIR_MODE_LR)
    {
        if (ABS((int32_t)down_p->x_coordinate - (int32_t)cur_p->x_coordinate) >= CLOCK_WIN_XRES / 3)
        {
            app_clock_design_request(0, &clock_move_lr_design, (void *)CLOCK_WIN_XRES);
        }
        else
        {
            app_clock_design_request(0, &clock_move_lr_design, (void *)0);
        }
    }
    else if (pdata->dir_mode == TOUCH_DIR_MODE_UPDN)
    {
        app_clock_timer_cb_unregister();

        if (ABS((int32_t)down_p->y_coordinate - (int32_t)cur_p->y_coordinate) >= CLOCK_WIN_YRES / 3)
        {
            app_clock_design_request(0, &clock_move_updn_design, (void *)CLOCK_WIN_YRES);
        }
        else
        {
            app_clock_design_request(0, &clock_move_updn_design, (void *)0);
        }
    }

    app_clock_touch_value_reset();

    return RT_EOK;
}

struct app_main_touch_cb_t app_clock_touch_cb =
{
    .tp_touch_down  = app_clock_touch_down,
    .tp_move_lr     = app_clock_touch_move_lr,
    .tp_move_updn   = app_clock_touch_move_updown,
    .tp_move_up     = app_clock_touch_moveup,
    .tp_touch_up    = app_clock_touch_up,
};

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

static void app_clock_mem_init(void)
{
    struct app_clock_data_t *pdata;

    app_clock_data = pdata = (struct app_clock_data_t *)rt_malloc(sizeof(struct app_clock_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct app_clock_data_t));

    /* top framebuffer malloc */
    pdata->top_fblen = CLOCK_WIN_FB_W * CLOCK_WIN_FB_H * CLOCK_WIN_COLOR_DEPTH / 8;
    pdata->top_fb   = (rt_uint8_t *)rt_malloc_psram(pdata->top_fblen);
    RT_ASSERT(pdata->top_fb != RT_NULL);

    pdata->bottom_fblen = CLOCK_WIN_FB_W * CLOCK_WIN_FB_H * CLOCK_WIN_COLOR_DEPTH / 8;
    pdata->bottom_fb   = (rt_uint8_t *)rt_malloc_psram(pdata->bottom_fblen);
    RT_ASSERT(pdata->bottom_fb != RT_NULL);

    pdata->bak_fblen = img_heart_info.w * img_heart_info.h * CLOCK_WIN_COLOR_DEPTH / 8;
    pdata->bak_fb   = (rt_uint8_t *)rt_malloc_psram(pdata->bak_fblen);
    RT_ASSERT(pdata->bak_fb != RT_NULL);
}

static void app_clock_refresh_init(void)
{
    // refresh win layers register
    app_clock_refrsh_param.win_id    = APP_CLOCK_WIN_0;
    app_clock_refrsh_param.win_layer = WIN_BOTTOM_LAYER;
    app_clock_refresh_register(APP_CLOCK_WIN_0, app_clock_win_refresh, &app_clock_refrsh_param);

    app_setting_main_refrsh_param.win_id    = APP_CLOCK_WIN_1;
    app_setting_main_refrsh_param.win_layer = WIN_TOP_LAYER;
    app_clock_refresh_register(APP_CLOCK_WIN_1, app_setting_main_refresh, &app_setting_main_refrsh_param);

    //draw first needle
    clock_design_param_t clock_param = { .buf_id = 1};
    app_clock_design(&clock_param);

    //refrest first clock
    refrsh_request_param_t request_param;
    request_param.wflag = 0x01 << app_clock_refrsh_param.win_id;
    request_param.wait = RT_WAITING_FOREVER;
    app_clock_refresh_now(&request_param);
}

static void app_clock_device_sn_init(void)
{
    char key[1];
    rt_err_t ret = get_device_sn(DEV_VENDOR_KEY, key, 1);

    if (ret <= 0)
        app_clock_data->cur_style = CLOCK_STYLE_FOC_ID;
    else
        app_clock_data->cur_style = key[0];

    if (app_clock_data->cur_style < 0)
        app_clock_data->cur_style = 0;
    if (app_clock_data->cur_style >= CLOCK_STYLE_MAX_NUM)
        app_clock_data->cur_style = CLOCK_STYLE_MAX_NUM - 1;

    write_device_sn(DEV_VENDOR_KEY, (char *)&app_clock_data->cur_style, 1);
}

/**
 * App clock fast init.
 */
void app_clock_init(void)
{
    rk_imagelib_init();

    app_clock_mem_init();

    app_clock_device_sn_init();

    app_clock_refresh_init();

    //register callback
    app_clock_timer_cb_register(app_clock_update);
    app_clock_touch_register(&app_clock_touch_cb);

    //prepare left & right clock design
    app_clock_data->switch_done = 0;
    clk_design_next_param.buf_id = 0;
    app_clock_design_request(0, &clk_design_next_cb, &clk_design_next_param);
}
