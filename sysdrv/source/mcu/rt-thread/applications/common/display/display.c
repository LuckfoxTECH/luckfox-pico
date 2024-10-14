/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtthread.h>
#include <stdio.h>
#include <math.h>

#include "drv_display.h"
#include "drv_heap.h"
#include "drv_pm.h"
#include "display.h"

static  struct rt_display_data *g_disp_data = RT_NULL;

#if defined(RT_USING_SPI_SCREEN)

/**
 * Group for RT-RT_USING_SPI_SCREEN
 */

/**
 * Screen clear.
 */
rt_err_t rt_display_screen_clear(void)
{
    return RT_EOK;
}

/**
 * Display application initial.
 */
static rt_display_data_t rt_display_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_device_t device;
    rt_display_data_t disp_data;
    struct rt_device_graphic_info info;

    if (g_disp_data != RT_NULL)
    {
        rt_kprintf("waring: rt_display already initialed!\n");
        return g_disp_data;
    }

    rt_enter_critical();

    disp_data = (struct rt_display_data *)rt_malloc(sizeof(struct rt_display_data));
    RT_ASSERT(disp_data != RT_NULL);
    rt_memset((void *)disp_data, 0, sizeof(struct rt_display_data));

    device = rt_device_find("spi_screen");
    RT_ASSERT(device != RT_NULL);

    ret = rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(device, RTGRAPHIC_CTRL_POWERON, NULL);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(device, RTGRAPHIC_CTRL_GET_INFO, &info);
    RT_ASSERT(ret == RT_EOK);
    memcpy(&disp_data->info, &info, sizeof(struct rt_device_graphic_info));

    disp_data->disp_mq = rt_mq_create("disp_mq", sizeof(struct rt_display_mq_t), 4, RT_IPC_FLAG_FIFO);
    RT_ASSERT(disp_data->disp_mq != RT_NULL);

    disp_data->device = device;
    g_disp_data = disp_data;

    rt_exit_critical();

    return disp_data;
}

/**
 * Display process.
 */
static rt_err_t rt_display_process(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret;
    struct rt_device_graphic_info info;

    if (disp_mq->cfgsta)
    {
        ret = rt_device_control(g_disp_data->device, RTGRAPHIC_CTRL_GET_INFO, &info);
        RT_ASSERT(ret == RT_EOK);

        pm_runtime_request(PM_RUNTIME_ID_VOP);
        ret = rt_device_control(g_disp_data->device, RTGRAPHIC_CTRL_RECT_UPDATE, &info);
        RT_ASSERT(ret == RT_EOK);
        pm_runtime_release(PM_RUNTIME_ID_VOP);
    }

    return RT_EOK;
}

/**
 * End of group for RT-RT_USING_SPI_SCREEN
 */

#else //#elif defined(RT_USING_VOP)

/**
 * Group for RT-RT_USING_VOP
 */

/**
 * color palette for RGB332
 */
uint32_t bpp_lut[256] = {0};

/**
 * Display driver sync hook, wait for drv_display finish.
 */
static rt_err_t rt_display_sync_hook(rt_device_t device)
{
    struct display_state *disp_state = (struct display_state *)device->user_data;
    int ret;
    struct display_sync display_sync_data;

    if (disp_state->panel_state.display_mode == DISPLAY_CMD_MODE)
    {
        display_sync_data.cmd = DISPLAY_SYNC;
        ret = rt_device_control(device, RK_DISPLAY_CTRL_DISPLAY_SYNC, &display_sync_data);
        if (ret != RT_EOK)
            rt_kprintf("rt_display_sync_hook time out\n");
    }

    return RT_EOK;
}

/**
 * color palette for RGB332 and BGR233,default format is RGB332.
 */
static void rt_display_bpp_lut_init(int format)
{
    int i = 0;
    int r2, r1, r0, g2, g1, g0, b1, b0;
    int R, G, B;
    float f;

    for (i = 0; i < 256; i++)
    {
        if (format == FORMAT_RGB_332)
        {
            r2 = (i & 0x80) >> 7;
            r1 = (i & 0x40) >> 6;
            r0 = (i & 0x20) >> 5;
            g2 = (i & 0x10) >> 4;
            g1 = (i & 0x8) >> 3;
            g0 = (i & 0x4) >> 2;
            b1 = (i & 0x2) >> 1;
            b0 = (i & 0x1) >> 0;
            R = (r2 << 7) | (r1 << 6) | (r0 << 5) | (r2 << 4) | (r1 << 3) | (r0 << 2) | (r2 << 1) | r1;
            G = (g2 << 7) | (g1 << 6) | (g0 << 5) | (g2 << 4) | (g1 << 3) | (g0 << 2) | (g2 << 1) | g1;
            B = (b1 << 7) | (b0 << 6) | (b1 << 5) | (b0 << 4) | (b1 << 3) | (b0 << 2) | (b1 << 1) | b0;
        }
        else if (format == FORMAT_BGR_233)
        {
            b1 = (i & 0x80) >> 7;
            b0 = (i & 0x40) >> 6;
            g2 = (i & 0x20) >> 5;
            g1 = (i & 0x10) >> 4;
            g0 = (i & 0x8) >> 3;
            r2 = (i & 0x4) >> 2;
            r1 = (i & 0x2) >> 1;
            r0 = (i & 0x1) >> 0;
            R = (r2 << 7) | (r1 << 6) | (r0 << 5) | (r2 << 4) | (r1 << 3) | (r0 << 2) | (r2 << 1) | r1;
            G = (g2 << 7) | (g1 << 6) | (g0 << 5) | (g2 << 4) | (g1 << 3) | (g0 << 2) | (g2 << 1) | g1;
            B = (b1 << 7) | (b0 << 6) | (b1 << 5) | (b0 << 4) | (b1 << 3) | (b0 << 2) | (b1 << 1) | b0;
        }
        else
        {
            f = (i + 0.5F) / 256;
            R = (unsigned char)((float)pow(f, 1 / GAMMA_RED) * 255 - 0.5F);
            G = (unsigned char)((float)pow(f, 1 / GAMMA_GREEN) * 255 - 0.5F);
            B = (unsigned char)((float)pow(f, 1 / GAMMA_BLUE) * 255 - 0.5F);
        }
        bpp_lut[i] = (R << 16) | (G << 8) | B;

        /* if (i % 4 == 0)
            printf("\n");
        printf("0x%08x, ", bpp_lut[i]); */
    }
}

/**
 * list win layers.
 */
static rt_err_t rt_display_win_layers_list(struct rt_display_config **head, struct rt_display_config *tail)
{
    RT_ASSERT(tail != RT_NULL);

    if (*head == RT_NULL)
    {
        *head = tail;
    }
    else
    {
        struct rt_display_config *tmp = *head;
        while (tmp->next != RT_NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = tail;
        tail->next = RT_NULL;
    }

    return RT_EOK;
}

/**
 * Configuration win layers.
 */
static rt_err_t rt_display_win_layers_set(struct rt_display_config *wincfg)
{
    rt_err_t ret = RT_EOK;
    struct CRTC_WIN_STATE win_config;
    struct VOP_POST_SCALE_INFO post_scale;
    struct rt_display_data *disp_data = g_disp_data;
    struct rt_device_graphic_info *info = &disp_data->info;
    rt_device_t device = disp_data->device;
    struct rt_display_config *cfg;
    struct display_state *disp_state = (struct display_state *)device->user_data;

    RT_ASSERT(wincfg != RT_NULL);

    /* post scale set */
    cfg = wincfg;
    {
        rt_memset(&post_scale, 0, sizeof(struct VOP_POST_SCALE_INFO));
        post_scale.dstX = 0;
        post_scale.dstY = 0xffff;
        post_scale.srcW = info->width;
        post_scale.srcH = 0;

        rt_uint16_t dstY2 = 0;

        while (cfg != RT_NULL)
        {
            post_scale.dstY = MIN(post_scale.dstY, cfg->y);
            post_scale.dstY = MIN(post_scale.dstY, cfg->ylast);
            dstY2 = MAX(dstY2, cfg->y + cfg->h);
            dstY2 = MAX(dstY2, cfg->ylast + cfg->h);

            cfg = cfg->next;
        }

        post_scale.srcH = dstY2 - post_scale.dstY;
        post_scale.dstW = post_scale.srcW * WSCALE;
        post_scale.dstH = post_scale.srcH * HSCALE;
#if 0
        rt_kprintf("dstX = %d, dstY = %d, dstW = %d, dstH =%d, srcW = %d, srcH =%d\n",
                   post_scale.dstX, post_scale.dstY, post_scale.dstW, post_scale.dstH,
                   post_scale.srcW,  post_scale.srcH);
#endif
        ret = rt_device_control(device,
                                RK_DISPLAY_CTRL_SET_SCALE, &post_scale);
        RT_ASSERT(ret == RT_EOK);
    }

    /* win0 set */
    cfg = wincfg;
    while (cfg != RT_NULL)
    {
        rt_memset(&win_config, 0, sizeof(struct CRTC_WIN_STATE));
        win_config.winId = cfg->winId;
        win_config.winEn = 1;
        win_config.winUpdate = 1;

        win_config.zpos = cfg->zpos;

        win_config.yrgbAddr = (uint32_t)cfg->fb;
        win_config.cbcrAddr = (uint32_t)cfg->fb;
        win_config.yrgbLength = cfg->fblen;
        win_config.cbcrLength = cfg->fblen;
        win_config.colorKey   = cfg->colorkey;

        if (cfg->xVir)
        {
            win_config.xVir  = cfg->xVir;
        }
        else
        {
            win_config.xVir  = cfg->w;
        }
        win_config.srcX  = cfg->x;
        win_config.srcY  = cfg->y;
        win_config.srcW  = cfg->w;
        win_config.srcH  = cfg->h;

        win_config.xLoopOffset = cfg->xLoopOffset;
        win_config.yLoopOffset = cfg->yLoopOffset;

        win_config.alphaEn = cfg->alphaEn;
        win_config.alphaMode = cfg->alphaMode;
        win_config.alphaPreMul = cfg->alphaPreMul;
        win_config.globalAlphaValue = cfg->globalAlphaValue;

        win_config.lut    = disp_data->lut[cfg->winId].lut;
        win_config.format = disp_data->lut[cfg->winId].format;
#if 0
        rt_kprintf("winId = %d, srcX = %d, srcY = %d, srcW = %d, srcH =%d, lut = %d, format =%d\n",
                   win_config.winId, win_config.srcX, win_config.srcY, win_config.srcW, win_config.srcH,
                   win_config.lut,  win_config.format);
        rt_kprintf("alphaEn = %d, alphaMode = %d, alphaPreMul = %d, globalAlphaValue = %d\n",
                   win_config.alphaEn, win_config.alphaMode, win_config.alphaPreMul, win_config.globalAlphaValue);
#endif
        ret = rt_device_control(device, RK_DISPLAY_CTRL_SET_PLANE, &win_config);
        RT_ASSERT(ret == RT_EOK);

        cfg = cfg->next;
    }

    // load lut
    cfg = wincfg;
    while (cfg != RT_NULL)
    {
        if (cfg->winId < 2 && cfg->format == RTGRAPHIC_PIXEL_FORMAT_RGB332)
        {
            if (cfg->hide_win)
            {
                struct crtc_lut_state lut_state;

                lut_state.win_id = cfg->winId;
                ret = rt_device_control(device, RK_DISPLAY_CTRL_DISABLE_LUT, &lut_state);
                RT_ASSERT(ret == RT_EOK);
            }
            if (cfg->new_lut)
            {
                struct crtc_lut_state lut_state;
                memset(&lut_state, 0, sizeof(struct crtc_lut_state));
                lut_state.win_id = cfg->winId;
                lut_state.lut = cfg->lut;
                lut_state.lut_size = cfg->lutsize;
                ret = rt_device_control(device, RK_DISPLAY_CTRL_LOAD_LUT, &lut_state);
                RT_ASSERT(ret == RT_EOK);

                disp_data->lut[cfg->winId].lut    = cfg->lut;
                disp_data->lut[cfg->winId].format = cfg->format;
            }
        }

        cfg = cfg->next;
    }

    ret = rt_device_control(device, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_display_sync_hook(device);
    RT_ASSERT(ret == RT_EOK);

    // close win
    if (disp_state->panel_state.display_mode == DISPLAY_CMD_MODE)
    {
        cfg = wincfg;
        while (cfg != RT_NULL)
        {
            ret = rt_device_control(device, RK_DISPLAY_CTRL_CLOSE_WIN, cfg->winId);
            RT_ASSERT(ret == RT_EOK);

            cfg = cfg->next;
        }
    }

    return ret;
}

/**
 * Display lut set.
 */
static rt_uint8_t fmt2bps[RTGRAPHIC_PIXEL_FORMAT_VYUY422_4 + 1] =
{
    1,  //RTGRAPHIC_PIXEL_FORMAT_MONO = 0,
    1,  //RTGRAPHIC_PIXEL_FORMAT_GRAY1,
    2,  //RTGRAPHIC_PIXEL_FORMAT_GRAY4,
    4,  //RTGRAPHIC_PIXEL_FORMAT_GRAY16,
    8,  //RTGRAPHIC_PIXEL_FORMAT_GRAY256,
    8,  //RTGRAPHIC_PIXEL_FORMAT_RGB332,
    16, //RTGRAPHIC_PIXEL_FORMAT_RGB444,
    16, //RTGRAPHIC_PIXEL_FORMAT_RGB565,
    16, //RTGRAPHIC_PIXEL_FORMAT_BGR565 = RTGRAPHIC_PIXEL_FORMAT_RGB565P,
    24, //RTGRAPHIC_PIXEL_FORMAT_RGB666,
    24, //RTGRAPHIC_PIXEL_FORMAT_RGB888,
    32, //RTGRAPHIC_PIXEL_FORMAT_ARGB888,
    32, //RTGRAPHIC_PIXEL_FORMAT_ABGR888,
    24, //RTGRAPHIC_PIXEL_FORMAT_ARGB565,
    32,  //RTGRAPHIC_PIXEL_FORMAT_ALPHA,
    32, //RTGRAPHIC_PIXEL_FORMAT_YUV420,
    32, //RTGRAPHIC_PIXEL_FORMAT_YUV422,
    32, //RTGRAPHIC_PIXEL_FORMAT_YUV444,
    32, //RTGRAPHIC_PIXEL_FORMAT_YVYU422,
    32, //RTGRAPHIC_PIXEL_FORMAT_VYUY422,
    32, //RTGRAPHIC_PIXEL_FORMAT_YUV420_4,
    32, //RTGRAPHIC_PIXEL_FORMAT_YUV422_4,
    32, //RTGRAPHIC_PIXEL_FORMAT_YUV444_4,
    32, //RTGRAPHIC_PIXEL_FORMAT_YVYU422_4,
    32, //RTGRAPHIC_PIXEL_FORMAT_VYUY422_4,
};
rt_err_t rt_display_win_clear(rt_uint8_t winid, rt_uint8_t fmt,
                              rt_uint16_t y, rt_uint16_t h, rt_uint8_t data)
{
    rt_err_t ret;
    struct rt_display_config wincfg;

    rt_memset(&wincfg, 0, sizeof(struct rt_display_config));
    wincfg.winId = winid;
    wincfg.format = fmt;
    wincfg.x     = 0;
    wincfg.y     = y;
    wincfg.w     = MAX(32 / fmt2bps[fmt], 1);
    wincfg.h     = h;
    wincfg.fblen = wincfg.w * wincfg.h * fmt2bps[fmt] / 8;
#ifdef RT_USING_LARGE_HEAP
    wincfg.fb    = (rt_uint8_t *)rt_malloc_large(wincfg.fblen);
#else
    wincfg.fb    = (rt_uint8_t *)rt_malloc(wincfg.fblen);
#endif
    rt_memset((void *)wincfg.fb, data, wincfg.fblen);

    ret = rt_display_win_layers_set(&wincfg);
    RT_ASSERT(ret == RT_EOK);
#ifdef RT_USING_LARGE_HEAP
    rt_free_large(wincfg.fb);
#else
    rt_free(wincfg.fb);
#endif
    return RT_EOK;
}

/**
 * Display lut set.
 */
static rt_err_t rt_display_lut_update(struct rt_display_lut *lutA)
{
    rt_err_t ret = RT_EOK;
    rt_display_data_t disp_data = g_disp_data;
    rt_device_t device = disp_data->device;

    RT_ASSERT(lutA != RT_NULL);

    if (lutA->size != 0)
    {
        // close bpp mode and refresh frame
        disp_data->lut[lutA->winId].lut    = RT_NULL;
        disp_data->lut[lutA->winId].format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
        rt_display_win_clear(lutA->winId, RTGRAPHIC_PIXEL_FORMAT_RGB565, 0, 2, 0);

        // update lut
        struct crtc_lut_state lut_state;
        memset(&lut_state, 0, sizeof(struct crtc_lut_state));
        lut_state.win_id = lutA->winId;
        lut_state.lut = lutA->lut;
        lut_state.lut_size = lutA->size;
        ret = rt_device_control(device, RK_DISPLAY_CTRL_LOAD_LUT, &lut_state);
        RT_ASSERT(ret == RT_EOK);

        disp_data->lut[lutA->winId].lut    = lutA->lut;
        disp_data->lut[lutA->winId].format = lutA->format;

        // enable new lut and refresh frame
        rt_display_win_clear(lutA->winId, lutA->format, 0, 2, 0);
    }
    else
    {
        disp_data->lut[lutA->winId].lut    = lutA->lut;
        disp_data->lut[lutA->winId].format = lutA->format;
    }

    return RT_EOK;
}

static void rt_display_power_control(bool on)
{
    rt_display_data_t disp_data = g_disp_data;
    rt_device_t device = disp_data->device;
    struct display_state *disp_state = (struct display_state *)device->user_data;
    rt_err_t ret = RT_EOK;

    if (disp_state->panel_state.display_mode != DISPLAY_CMD_MODE)
        return;

    if (on)
    {
        if (disp_data->enable_cnt == 0)
            ret = rt_device_control(g_disp_data->device, RK_DISPLAY_CTRL_ENABLE, NULL);
        disp_data->enable_cnt++;
    }
    else
    {
        if (disp_data->enable_cnt > 0)
        {
            disp_data->enable_cnt--;
            if (disp_data->enable_cnt == 0)
                ret = rt_device_control(g_disp_data->device, RK_DISPLAY_CTRL_DISABLE, NULL);
        }
    }
    RT_ASSERT(ret == RT_EOK);
}

/**
 * Get MAX value of backlight.
 */
static rt_uint16_t rt_display_get_bl_max(rt_device_t device)
{
    struct display_state *disp_state = (struct display_state *)device->user_data;

    return disp_state->panel_state.max_brightness;
}

/**
 * Get MAX value of backlight.
 */
rt_uint16_t rt_display_backlight_get(void)
{
    struct rt_display_data *disp_data = g_disp_data;
    rt_device_t device = disp_data->device;
    struct display_state *disp_state = (struct display_state *)device->user_data;

    return disp_state->panel_state.brightness * 100 / disp_state->panel_state.max_brightness;
}

/**
 * backlight set (0 ~ 100).
 */
rt_err_t rt_display_backlight_set(rt_uint16_t val)
{
    rt_err_t ret = RT_EOK;
    struct rt_display_data *disp_data = g_disp_data;
    rt_device_t device = disp_data->device;
    struct display_state *disp_state = (struct display_state *)device->user_data;
    rt_uint16_t blval  = val * disp_state->panel_state.max_brightness / 100;

    rt_display_power_control(true);
    if (blval == 0)
    {
        ret = rt_device_control(device, RK_DISPLAY_CTRL_UPDATE_BL, &blval);
        ret = rt_device_control(device, RTGRAPHIC_CTRL_POWEROFF, NULL);
        RT_ASSERT(ret == RT_EOK);
    }
    else
    {
        if (disp_state->panel_state.brightness == 0)
            ret = rt_device_control(device, RTGRAPHIC_CTRL_POWERON, NULL);

        ret = rt_device_control(device, RK_DISPLAY_CTRL_UPDATE_BL, &blval);
        RT_ASSERT(ret == RT_EOK);
    }

    rt_display_power_control(false);

    return ret;
}

/**
 * Win layers close
 */
void rt_display_win_layer_close(rt_uint8_t winid)
{
    rt_err_t ret;
    struct CRTC_WIN_STATE win_config;

    rt_device_t device = g_disp_data->device;
    struct display_state *disp_state = (struct display_state *)device->user_data;
    if (disp_state->panel_state.display_mode == DISPLAY_VIDEO_MODE)
    {
        win_config.winId = winid;
        win_config.winEn = 0;
        win_config.winUpdate = 1;
        ret = rt_device_control(device, RK_DISPLAY_CTRL_SET_PLANE, &win_config);
        RT_ASSERT(ret == RT_EOK);
    }
}

/**
 * Screen clear.
 */
rt_err_t rt_display_screen_clear(void)
{
    struct rt_device_graphic_info *info = &g_disp_data->info;
    struct rt_display_lut  lut;

    rt_memset(&lut, 0, sizeof(struct rt_display_lut));
    lut.winId  = 2;
    lut.format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    rt_display_lut_update(&lut);
    rt_display_win_clear(2, RTGRAPHIC_PIXEL_FORMAT_RGB565, 0, info->height, 0);

    rt_display_win_layer_close(2);

    return RT_EOK;
}

/**
 * Clear screen for display initial.
 */
static rt_err_t rt_display_screen_init(rt_device_t device)
{
    rt_err_t ret = RT_EOK;
    struct CRTC_WIN_STATE win_config;
    struct rt_device_graphic_info info;

    ret = rt_device_control(device, RTGRAPHIC_CTRL_GET_INFO, &info);
    RT_ASSERT(ret == RT_EOK);

    rt_memset(&win_config, 0, sizeof(struct CRTC_WIN_STATE));
    win_config.winEn = 0;
    win_config.winId = 0;
    win_config.winUpdate = 1;

    win_config.srcW = info.width;
    win_config.srcH = info.height;
    win_config.crtcX = 0;
    win_config.crtcY = 0;
    win_config.crtcW = info.width;
    win_config.crtcH = info.height;

    ret = rt_device_control(device, RK_DISPLAY_CTRL_SET_PLANE, &win_config);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * Display application initial, initial screen and win layers.
 */
static rt_display_data_t rt_display_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_device_t device;
    rt_display_data_t disp_data;
    struct rt_device_graphic_info info;
    struct crtc_lut_state lut_state;

    if (g_disp_data != RT_NULL)
    {
        rt_kprintf("waring: rt_display already initialed!\n");
        return g_disp_data;
    }

    rt_enter_critical();

    disp_data = (struct rt_display_data *)rt_malloc(sizeof(struct rt_display_data));
    RT_ASSERT(disp_data != RT_NULL);
    rt_memset((void *)disp_data, 0, sizeof(struct rt_display_data));

    device = rt_device_find("lcd");
    RT_ASSERT(device != RT_NULL);

    ret = rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    disp_data->disp_mq = rt_mq_create("disp_mq", sizeof(struct rt_display_mq_t), 4, RT_IPC_FLAG_FIFO);
    RT_ASSERT(disp_data->disp_mq != RT_NULL);

#ifdef RKMCU_PISCES
    int path = SWITCH_TO_INTERNAL_DPHY;
    ret = rt_device_control(device, RK_DISPLAY_CTRL_MIPI_SWITCH, &path);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(device, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)1);
    RT_ASSERT(ret == RT_EOK);
#else //RKMCU_RK2108 or Others
    ret = rt_device_control(device, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)0);
    RT_ASSERT(ret == RT_EOK);
#endif

    ret = rt_device_control(device, RTGRAPHIC_CTRL_POWERON, NULL);
    RT_ASSERT(ret == RT_EOK);

    disp_data->enable_cnt = 1;

    ret = rt_device_control(device, RTGRAPHIC_CTRL_GET_INFO, &info);
    RT_ASSERT(ret == RT_EOK);
    memcpy(&disp_data->info, &info, sizeof(struct rt_device_graphic_info));

    /* init lut */
    rt_display_bpp_lut_init(FORMAT_RGB_332);
    memset(&lut_state, 0, sizeof(struct crtc_lut_state));
    for (rt_uint32_t i = 0; i < 2; i++)
    {
        lut_state.win_id = i;
        lut_state.lut = bpp_lut;
        lut_state.lut_size = sizeof(bpp_lut) / sizeof(bpp_lut[0]);
        ret = rt_device_control(device, RK_DISPLAY_CTRL_LOAD_LUT, &lut_state);
        RT_ASSERT(ret == RT_EOK);
        disp_data->lut[i].lut    = bpp_lut;
        disp_data->lut[i].format = RTGRAPHIC_PIXEL_FORMAT_RGB332;
    }
    disp_data->lut[2].lut    = RT_NULL;
    disp_data->lut[2].format = RTGRAPHIC_PIXEL_FORMAT_RGB565;

    disp_data->blval = rt_display_get_bl_max(device) / 2;
    ret = rt_device_control(device, RK_DISPLAY_CTRL_UPDATE_BL, &disp_data->blval);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(device, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    /* clear screen */
    ret = rt_display_screen_init(device);
    RT_ASSERT(ret == RT_EOK);

    disp_data->device = device;

    g_disp_data = disp_data;

    rt_exit_critical();

    return disp_data;
}

/**
 * Display process.
 */
static rt_err_t rt_display_process(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t i;
    struct rt_display_config *winhead = RT_NULL;
    struct rt_display_data *disp = g_disp_data;

    for (i = 0; i < WIN_MAX_LAYER; i++)
    {
        if (disp_mq->cfgsta & (0x01 << i))
        {
            rt_uint8_t winid;
            struct rt_display_lut *old_lut;
            struct rt_display_config *wincfg;

            wincfg  = &disp_mq->win[i];
            winid   = wincfg->winId;
            old_lut = &disp->lut[winid];
            if ((wincfg->format != old_lut->format) || (wincfg->lut != old_lut->lut))
            {
                struct rt_display_lut  lut;

                rt_memset(&lut, 0, sizeof(struct rt_display_lut));
                lut.winId  = winid;
                lut.format = wincfg->format;
                lut.lut    = wincfg->lut;
                lut.size   = wincfg->lutsize;

                ret = rt_display_lut_update(&lut);
                RT_ASSERT(ret == RT_EOK);
            }

            rt_display_win_layers_list(&winhead, wincfg);
        }
    }

    ret = rt_display_win_layers_set(winhead);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * End of group for RT_USING_VOP
 */

#endif

/**
 * Display application deinitial, free resources.
 */
static void rt_display_deinit(rt_display_data_t disp_data)
{
    rt_err_t ret;
    do
    {
        ret = rt_mq_delete(disp_data->disp_mq);
        rt_thread_delay(10);
    }
    while (ret != RT_EOK);

    while (1)
    {
        if (RT_EOK == rt_device_close(disp_data->device))
        {
            break;
        }
        rt_thread_delay(10);
    }

    rt_free(g_disp_data);
    g_disp_data = RT_NULL;
}

/**
 * Get global display data struct.
 */
rt_display_data_t rt_display_get_disp(void)
{
    rt_uint32_t timeout = 200;

    while (--timeout != 0)
    {
        if (g_disp_data != RT_NULL)
        {
            return g_disp_data;
        }
        rt_thread_delay(1);
    };

    rt_kprintf("error: rt_display_get_disp fail!\n");
    return RT_NULL;
}

/*
 **************************************************************************************************
 *
 * rt display thread.
 *
 **************************************************************************************************
 */
/**
 * rt display thread.
 */
static void rt_display_thread(void *p)
{
    rt_err_t ret;
    struct rt_display_mq_t disp_mq;
    struct rt_display_data *disp;

    disp = rt_display_init();
    RT_ASSERT(disp != RT_NULL);

    while (1)
    {
#if defined(RT_USING_VOP)
        /* If no refresh request message is received within 50ms, turn off vop to save power */
        ret = rt_mq_recv(disp->disp_mq, &disp_mq, sizeof(struct rt_display_mq_t), RK_DISPLAY_TIME_OUT);
        if (ret == -RT_ETIMEOUT)
        {
            rt_display_power_control(false);
            ret = rt_mq_recv(disp->disp_mq, &disp_mq, sizeof(struct rt_display_mq_t), RT_WAITING_FOREVER);
            RT_ASSERT(ret == RT_EOK);
            rt_display_power_control(true);
        }
#else
        ret = rt_mq_recv(disp->disp_mq, &disp_mq, sizeof(struct rt_display_mq_t), RT_WAITING_FOREVER);
        RT_ASSERT(ret == RT_EOK);
#endif

        ret = rt_display_process(&disp_mq);
        RT_ASSERT(ret == RT_EOK);

        if (disp_mq.disp_finish)
        {
            ret = disp_mq.disp_finish();
            RT_ASSERT(ret == RT_EOK);
        }
    }

    rt_display_deinit(disp);
    disp = RT_NULL;
}

/**
 * olpc clock demo application init.
 */
int rt_display_thread_init(void)
{
    rt_thread_t rtt_disp;

    rtt_disp = rt_thread_create("disp", rt_display_thread, RT_NULL, 2048, 3, 10);
    RT_ASSERT(rtt_disp != RT_NULL);
    rt_thread_startup(rtt_disp);

    return RT_EOK;
}
INIT_APP_EXPORT(rt_display_thread_init);
