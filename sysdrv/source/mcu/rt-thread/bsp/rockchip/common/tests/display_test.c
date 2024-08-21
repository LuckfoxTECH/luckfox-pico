/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    display_test.c
  * @version V0.1
  * @brief   display driver test for pisces
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Huang Jiachai   first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_DISPLAY
#include "drv_display.h"
#include "drv_heap.h"
#include "display_pattern.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "drv_display.h"
#include "hal_base.h"

#define USR_M4_TO_CREATE_PATTERN
//#define EBOOK_DEMO
//#define CLOCK_DEMO

#ifdef USR_M4_TO_CREATE_PATTERN
#ifdef RT_USING_PANEL_S6E3HC2_X1
#define SRC_DATA_W  260
#define SRC_DATA_H  195
#define X_ALIGNED   720
#define Y_ALIGNED   65
#else
#define SRC_DATA_W  240
#define SRC_DATA_H  320
#define X_ALIGNED   1
#define Y_ALIGNED   2
#endif
#define SRC_DATA_ADDR   0x20080000
#define SRC_CBCR_DATA_ADDR   0x200c0000
#define SRC_DATA_FMT    RTGRAPHIC_PIXEL_FORMAT_RGB888
#elif defined(EBOOK_DEMO)
#include "cn_white_ebook_demo.h"
#define SRC_DATA_ADDR   cn_white_ebook_demo
#define SRC_CBCR_DATA_ADDR   0x200c0000
#define SRC_DATA_W  1056
#define SRC_DATA_H  2209
#define SRC_DATA_FMT    RTGRAPHIC_PIXEL_FORMAT_GRAY1
#define SRC_DATA_1BPP
#define X_ALIGNED   1
#define Y_ALIGNED   2
#elif defined(CLOCK_DEMO)
#include "clock_demo.h"
#define SRC_DATA_ADDR   clock_demo
#define SRC_CBCR_DATA_ADDR   0x200c0000
#define SRC_DATA_W  440
#define SRC_DATA_H  440
#define SRC_DATA_FMT    RTGRAPHIC_PIXEL_FORMAT_GRAY256
#define SRC_DATA_8BPP
#define X_ALIGNED   1
#define Y_ALIGNED   2
#else
#ifdef RT_USING_PANEL_S6E3HC2_X1
/* enable dsc and area display, display area must 720x325 align */
#define SRC_DATA_W  720
#define SRC_DATA_H  325
#elif defined(RT_USING_PANEL_S6E3HC2_X4)
#define SRC_DATA_W      360
#define SRC_DATA_H      240
#elif defined(RT_USING_PANEL_SS)
#define SRC_DATA_W  1080
#define SRC_DATA_H  100
#else
#define SRC_DATA_W      320
#define SRC_DATA_H      240
#endif
#define SRC_DATA_FMT    RTGRAPHIC_PIXEL_FORMAT_RGB888
#define X_ALIGNED   1
#define Y_ALIGNED   2
//#define SRC_DATA_8BPP
#endif
static uint32_t rtt_framebuffer_test;
static uint32_t rtt_framebuffer_uv;
static uint32_t dsp_dtcm_addr = 0x30200000;

#define GPIO_BANKx      (GPIO_BANK0)
#define PINNUM_INT  (29)
#define GPIOx  (GPIO0)
#define GPIO_PIN_INT  (GPIO_PIN_D5)

struct rt_work switch_work;
struct rt_workqueue *switch_workqueue;
int switch_times = 0;

#if defined SRC_DATA_1BPP
/**
  * 1bpp format lut
  */
static uint32_t bpp_lut[2] =
{
    0x00000000, 0x00ffffff
};
#elif defined SRC_DATA_2BPP
/**
  * 2bpp format lut
  */
static uint32_t bpp_lut[4] =
{
    0x00000000, 0x00505050, 0x00a0a0a0, 0x00f0f0f0
};
#elif defined SRC_DATA_4BPP
/**
  * 4bpp format lut
  */
static uint32_t bpp_lut[16] =
{
    0x00000000, 0x00800000, 0x00008000, 0x00808000,
    0x00000080, 0x00800080, 0x00008080, 0x00808080,
    0x00c0c0c0, 0x00ff0000, 0x0000ff00, 0x00ffff00,
    0x000000ff, 0x00ff00ff, 0x0000ffff, 0x00ffffff
};
#elif defined SRC_DATA_8BPP
/**
  * 8bpp format lut
  */
static uint32_t bpp_lut[256] =
{
    0x00000000, 0x00800000, 0x00008000, 0x00808000,
    0x00000080, 0x00800080, 0x00008080, 0x00c0c0c0,
    0x00c0dcc0, 0x00a6caf0, 0x00402000, 0x00602000,
    0x00802000, 0x00a02000, 0x00c02000, 0x00e02000,
    0x00004000, 0x00204000, 0x00404000, 0x00604000,
    0x00804000, 0x00a04000, 0x00c04000, 0x00e04000,
    0x00006000, 0x00206000, 0x00406000, 0x00606000,
    0x00806000, 0x00a06000, 0x00c06000, 0x00e06000,
    0x00008000, 0x00208000, 0x00408000, 0x00608000,
    0x00808000, 0x00a08000, 0x00c08000, 0x00e08000,
    0x0000a000, 0x0020a000, 0x0040a000, 0x0060a000,
    0x0080a000, 0x00a0a000, 0x00c0a000, 0x00e0a000,
    0x0000c000, 0x0020c000, 0x0040c000, 0x0060c000,
    0x0080c000, 0x00a0c000, 0x00c0c000, 0x00e0c000,
    0x0000e000, 0x0020e000, 0x0040e000, 0x0060e000,
    0x0080e000, 0x00a0e000, 0x00c0e000, 0x00e0e000,
    0x00000040, 0x00200040, 0x00400040, 0x00600040,
    0x00800040, 0x00a00040, 0x00c00040, 0x00e00040,
    0x00002040, 0x00202040, 0x00402040, 0x00602040,
    0x00802040, 0x00a02040, 0x00c02040, 0x00e02040,
    0x00004040, 0x00204040, 0x00404040, 0x00604040,
    0x00804040, 0x00a04040, 0x00c04040, 0x00e04040,
    0x00006040, 0x00206040, 0x00406040, 0x00606040,
    0x00806040, 0x00a06040, 0x00c06040, 0x00e06040,
    0x00008040, 0x00208040, 0x00408040, 0x00608040,
    0x00808040, 0x00a08040, 0x00c08040, 0x00e08040,
    0x0000a040, 0x0020a040, 0x0040a040, 0x0060a040,
    0x0080a040, 0x00a0a040, 0x00c0a040, 0x00e0a040,
    0x0000c040, 0x0020c040, 0x0040c040, 0x0060c040,
    0x0080c040, 0x00a0c040, 0x00c0c040, 0x00e0c040,
    0x0000e040, 0x0020e040, 0x0040e040, 0x0060e040,
    0x0080e040, 0x00a0e040, 0x00c0e040, 0x00e0e040,
    0x00000080, 0x00200080, 0x00400080, 0x00600080,
    0x00800080, 0x00a00080, 0x00c00080, 0x00e00080,
    0x00002080, 0x00202080, 0x00402080, 0x00602080,
    0x00802080, 0x00a02080, 0x00c02080, 0x00e02080,
    0x00004080, 0x00204080, 0x00404080, 0x00604080,
    0x00804080, 0x00a04080, 0x00c04080, 0x00e04080,
    0x00006080, 0x00206080, 0x00406080, 0x00606080,
    0x00806080, 0x00a06080, 0x00c06080, 0x00e06080,
    0x00008080, 0x00208080, 0x00408080, 0x00608080,
    0x00808080, 0x00a08080, 0x00c08080, 0x00e08080,
    0x0000a080, 0x0020a080, 0x0040a080, 0x0060a080,
    0x0080a080, 0x00a0a080, 0x00c0a080, 0x00e0a080,
    0x0000c080, 0x0020c080, 0x0040c080, 0x0060c080,
    0x0080c080, 0x00a0c080, 0x00c0c080, 0x00e0c080,
    0x0000e080, 0x0020e080, 0x0040e080, 0x0060e080,
    0x0080e080, 0x00a0e080, 0x00c0e080, 0x00e0e080,
    0x000000c0, 0x002000c0, 0x004000c0, 0x006000c0,
    0x008000c0, 0x00a000c0, 0x00c000c0, 0x00e000c0,
    0x000020c0, 0x002020c0, 0x004020c0, 0x006020c0,
    0x008020c0, 0x00a020c0, 0x00c020c0, 0x00e020c0,
    0x000040c0, 0x002040c0, 0x004040c0, 0x006040c0,
    0x008040c0, 0x00a040c0, 0x00c040c0, 0x00e040c0,
    0x000060c0, 0x002060c0, 0x004060c0, 0x006060c0,
    0x008060c0, 0x00a060c0, 0x00c060c0, 0x00e060c0,
    0x000080c0, 0x002080c0, 0x004080c0, 0x006080c0,
    0x008080c0, 0x00a080c0, 0x00c080c0, 0x00e080c0,
    0x0000a0c0, 0x0020a0c0, 0x0040a0c0, 0x0060a0c0,
    0x0080a0c0, 0x00a0a0c0, 0x00c0a0c0, 0x00e0a0c0,
    0x0000c0c0, 0x0020c0c0, 0x0040c0c0, 0x0060c0c0,
    0x0080c0c0, 0x00a0c0c0, 0x00fffbf0, 0x00a0a0a4,
    0x00808080, 0x00ff0000, 0x0000ff00, 0x00ffff00,
    0x000000ff, 0x00ff00ff, 0x0000ffff, 0x00ffffff
};
#else
static uint32_t *bpp_lut = NULL;
#endif

static struct rt_device *g_display_dev;
static struct rt_device *g_backlight_dev;
bool mipi_switch_to_ap = 0;
static struct rt_mutex display_lock;
struct CRTC_WIN_STATE *gwin_config;
struct VOP_POST_SCALE_INFO *gpost_scale;
struct rt_device_graphic_info *ggraphic_info;

static int32_t AlignDown(int32_t value, int32_t align)
{
    return value / align * align;
}

static int32_t framebuffer_alloc(rt_size_t size)
{
#if defined(RT_USING_LARGE_HEAP)
    return (uint32_t)rt_dma_malloc_large(size);
#else
    return 0;
#endif
}

static void framebuffer_free(void *base)
{
#if defined(RT_USING_LARGE_HEAP)
    rt_dma_free_large(base);
#endif
}

static void flush_cache(void)
{
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)rtt_framebuffer_test, SRC_DATA_W * SRC_DATA_H * 4);
}

static inline void display_delay(uint8_t time)
{
    rt_thread_delay(50 * time);
}

static uint32_t get_stride(uint32_t format, uint32_t width)
{
    switch (format)
    {
    case RTGRAPHIC_PIXEL_FORMAT_ARGB888:
    case RTGRAPHIC_PIXEL_FORMAT_ABGR888:
        return width * 4;
    case RTGRAPHIC_PIXEL_FORMAT_RGB888:
        return width * 3;
    case RTGRAPHIC_PIXEL_FORMAT_BGR565:
    case RTGRAPHIC_PIXEL_FORMAT_RGB565:
        return width * 2;
    case RTGRAPHIC_PIXEL_FORMAT_YUV420:
        return width;
    default:
        rt_kprintf("get_stride unsupport format:%d\n", format);
        return width * 4;
    }
}

static int switch_to_full_screen(struct rt_device *dev)
{
    rt_err_t ret;

    gwin_config->winId = 0;
    gwin_config->winEn = 0;
    gwin_config->format = RTGRAPHIC_PIXEL_FORMAT_ARGB888;
    gwin_config->yrgbAddr = rtt_framebuffer_test;
    gwin_config->xVir = SRC_DATA_W;

    gwin_config->srcX = 0;
    gwin_config->srcY = 0;
    gwin_config->srcW = ggraphic_info->width;
    gwin_config->srcH = ggraphic_info->height;

    gpost_scale->srcW = ggraphic_info->width;
    gpost_scale->srcH = ggraphic_info->height;
    gpost_scale->dstX = 0;
    gpost_scale->dstY = 0;
    gpost_scale->dstW = ggraphic_info->width;
    gpost_scale->dstH = ggraphic_info->height;

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_ENABLE, NULL);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, gpost_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, gwin_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    rt_thread_mdelay(200);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_DISABLE, NULL);
    RT_ASSERT(ret == RT_EOK);

    return 0;
}

static void display_win_init(struct CRTC_WIN_STATE *win_config)
{
    win_config->winEn = true;
    win_config->winId = 0;
    win_config->zpos = 0;
    win_config->format = SRC_DATA_FMT;
#ifdef USR_M4_TO_CREATE_PATTERN
    win_config->yrgbAddr = (uint32_t)rtt_framebuffer_test;
    win_config->cbcrAddr = (uint32_t)rtt_framebuffer_uv;
#else
    win_config->yrgbAddr = SRC_DATA_ADDR;
    win_config->cbcrAddr = SRC_CBCR_DATA_ADDR;
#endif
    win_config->yrgbLength = 0;
    win_config->cbcrLength = 0;
    win_config->xVir = SRC_DATA_W;

    win_config->srcX = 0;
    win_config->srcY = 0;
    win_config->srcW = SRC_DATA_W;
    win_config->srcH = SRC_DATA_H;

    win_config->crtcX = 0;
    win_config->crtcY = 0;
    win_config->crtcW = SRC_DATA_W;
    win_config->crtcH = SRC_DATA_H;

    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;

    win_config->alphaEn = 0;
    win_config->alphaMode = VOP_ALPHA_MODE_PER_PIXEL;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;
}

static void disable_all_win(struct rt_device *dev,
                            struct CRTC_WIN_STATE *win_config)
{
    rt_err_t ret;

    rt_kprintf("Display test: disable all win\n");
    win_config->winEn = 0;
    win_config->winId = 0;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    win_config->winEn = 0;
    win_config->winId = 1;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    win_config->winEn = 0;
    win_config->winId = 2;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    display_delay(1);
}

static void test_win_alpha(struct rt_device *dev,
                           struct CRTC_WIN_STATE *win_config,
                           struct VOP_POST_SCALE_INFO *post_scale)
{
    rt_err_t ret;
    uint16_t i;

    rt_kprintf("Display test: test win alpha!\n");

    win_config->winEn = 1;
    win_config->winId = 0;
    win_config->zpos = win_config->winId;

    win_config->alphaEn = 1;
    win_config->alphaMode = VOP_ALPHA_MODE_USER_DEFINED;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;
    win_config->globalAlphaValue = 0;
    rt_kprintf("Display test: win0 global alpha test\n");
    for (i = 0; i < 0xff;)
    {
        win_config->globalAlphaValue = i;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
        i += 5;
    }
    disable_all_win(g_display_dev, win_config);
    display_win_init(win_config);

    win_config->winEn = 1;
    win_config->winId = 1;
    win_config->zpos = win_config->winId;

    win_config->alphaEn = 1;
    win_config->alphaMode = VOP_ALPHA_MODE_USER_DEFINED;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;

    win_config->srcY += SRC_DATA_H;
    post_scale->dstX = win_config->srcX;
    post_scale->dstY = win_config->srcY;
    win_config->yrgbAddr = rtt_framebuffer_test;
    rt_kprintf("Display test: win1 global alpha test\n");
    for (i = 0; i < 0xff;)
    {
        win_config->globalAlphaValue = i;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
        i += 5;
    }
    disable_all_win(dev, win_config);
    display_delay(2);
    display_win_init(win_config);
    win_config->winEn = 1;
    win_config->winId = 2;
    win_config->zpos = win_config->winId;

    win_config->alphaEn = 1;
    win_config->alphaMode = VOP_ALPHA_MODE_USER_DEFINED;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;

    win_config->srcY += SRC_DATA_H;
    post_scale->dstX = win_config->srcX;
    post_scale->dstY = win_config->srcY;
    rt_kprintf("Display test: win2 global alpha test!\n");
    win_config->yrgbAddr = rtt_framebuffer_test;
    for (i = 0; i < 0xff;)
    {
        win_config->globalAlphaValue = i;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
        i += 5;
    }
    disable_all_win(dev, win_config);
    display_delay(2);

    rt_kprintf("Display test: win1 per-pixel alpha test\n");
    display_win_init(win_config);
    win_config->zpos = win_config->winId;

    post_scale->srcW = win_config->srcW;
    post_scale->srcH = win_config->srcH * 2;
    post_scale->dstX = 0;
    post_scale->dstY = 0;
    post_scale->dstW = win_config->srcW;
    post_scale->dstH = post_scale->srcH;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    /* win 0 */
    win_config->winEn = 1;
    win_config->winId = 0;
    win_config->zpos = win_config->winId;
    win_config->srcX = 0;
    win_config->srcY = 0;
    win_config->alphaEn = 0;
    win_config->alphaMode = VOP_ALPHA_MODE_USER_DEFINED;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    /* win 1 */
    win_config->winEn = 1;
    win_config->winId = 1;
    win_config->zpos = win_config->winId;
    win_config->format = RTGRAPHIC_PIXEL_FORMAT_ARGB888;
    win_config->srcX = 0;
    win_config->srcY = win_config->srcH / 2;
    win_config->alphaEn = 1;
    win_config->alphaMode = VOP_ALPHA_MODE_PER_PIXEL;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    rt_kprintf("Display test: test win alpha exit!\n");
}

static void test_dsc_display(uint8_t win_id,
                             struct rt_device *dev,
                             struct CRTC_WIN_STATE *win_config,
                             struct VOP_POST_SCALE_INFO *post_scale,
                             struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret;
    uint16_t i = 0, j = 0, buffer_index = 0;
    uint16_t step  = SRC_DATA_H;
    uint16_t y_times = (graphic_info->height - SRC_DATA_H) / step;

    win_config->winId = win_id;
    win_config->winEn = 1;

    for (j = 0; j < 10; j++)
    {
        if (j % 2 == 0)
        {
            if (buffer_index++ % 2 == 0)
                win_config->yrgbAddr = SRC_DATA_ADDR;
            else
                win_config->yrgbAddr = SRC_CBCR_DATA_ADDR;
            win_config->srcX = 0;
        }
        else
        {
            win_config->srcX = SRC_DATA_W;
        }

        for (i = 0; i <= y_times; i++)
        {
            win_config->srcY = step * i;
            if (win_config->srcY + win_config->srcH > graphic_info->height)
                win_config->srcY = graphic_info->height - win_config->srcH;
            post_scale->dstX = win_config->srcX;
            post_scale->dstY = win_config->srcY;
            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_SCALE, post_scale);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_PLANE, win_config);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
            RT_ASSERT(ret == RT_EOK);
            display_delay(10);
            rt_kprintf("dsc test, pos:%dx%d\n\n", win_config->srcX, win_config->srcY);
        }
    }
}

static void test_win_loop(uint8_t win_id,
                          struct rt_device *dev,
                          struct CRTC_WIN_STATE *win_config,
                          struct VOP_POST_SCALE_INFO *post_scale,
                          struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret;
    uint16_t i = 0, j = 0;
    uint16_t step  = 8;
    uint16_t x_times = SRC_DATA_W / 2 / step;
    uint16_t y_times = SRC_DATA_H / 2 / step;

    rt_kprintf("Display test: test win%d loop!\n", win_id);
    win_config->winId = win_id;
    win_config->winEn = 1;
    win_config->zpos = win_config->winId;
    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;

    for (i = 0; i < x_times; i++)
    {
        win_config->xLoopOffset = i * 8;
        rt_kprintf("Display test: test win%d loop, xloopofset:%d!\n", win_id, win_config->xLoopOffset);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
    }

    display_win_init(win_config);
    win_config->winId = win_id;
    win_config->zpos = win_config->winId;

    for (i = 0; i < y_times; i++)
    {
        win_config->yLoopOffset = i * 8;
        rt_kprintf("Display test: test win%d loop, yloopofset:%d!\n", win_id, win_config->yLoopOffset);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
    }
    display_win_init(win_config);
    win_config->winId = win_id;
    win_config->zpos = win_config->winId;

    for (i = 0; i < 6; i++)
    {
        win_config->xLoopOffset = i * 8;
        win_config->yLoopOffset = i * 8;
        rt_kprintf("Display test: test win%d loop, loop:%d %d!\n", win_id, win_config->xLoopOffset, win_config->yLoopOffset);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
    }

    for (j = 0; j < 6; j++)
    {
        display_win_init(win_config);
        win_config->winId = win_id;
        win_config->zpos = win_config->winId;
        win_config->srcX = graphic_info->width / 4;
        win_config->srcY = graphic_info->height / 4;
        win_config->srcW = SRC_DATA_W / 2;

        post_scale->srcW = graphic_info->width;
        post_scale->srcH = win_config->srcH;
        post_scale->dstX = 0;
        post_scale->dstY = win_config->srcY;
        post_scale->dstW = graphic_info->width;
        post_scale->dstH = win_config->srcH;

        for (i = 0; i < 5; i++)
        {
            win_config->xLoopOffset = i * 8;
            rt_kprintf("Display test: test win%d @%dx%d loop, xloopofset:%d\n", win_id, win_config->srcX, win_config->srcY, win_id, win_config->xLoopOffset);
            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_SCALE, post_scale);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_PLANE, win_config);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
            RT_ASSERT(ret == RT_EOK);
            display_delay(2);
        }
    }

    display_win_init(win_config);
}

static void test_win_move(uint8_t win_id,
                          struct rt_device *dev,
                          struct CRTC_WIN_STATE *win_config,
                          struct VOP_POST_SCALE_INFO *post_scale,
                          struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret;
    uint16_t i = 0;
    uint16_t step  = 8;
    uint16_t x_times = (graphic_info->width - SRC_DATA_W) / step;
    uint16_t y_times = (graphic_info->height - SRC_DATA_H) / step;

    win_config->winId = win_id;
    win_config->winEn = 1;
    win_config->winUpdate = 1;
    win_config->srcX = 0;
    win_config->srcY = 0;

    post_scale->srcW = win_config->srcW;
    post_scale->srcH = win_config->srcH;
    post_scale->dstX = win_config->srcX;
    post_scale->dstY = win_config->srcY;
    post_scale->dstW = win_config->srcW;
    post_scale->dstH = win_config->srcH;

    for (i = 0; i <= x_times; i++)
    {
        win_config->srcX = step * i;
        if (win_config->srcX + win_config->srcW > graphic_info->width)
            win_config->srcX = graphic_info->width - win_config->srcW;
        post_scale->dstX = win_config->srcX;
        post_scale->dstY = win_config->srcY;

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
    }

    for (i = 0; i <= y_times; i++)
    {
        win_config->srcY = step * i;
        if (win_config->srcY + win_config->srcH > graphic_info->height)
            win_config->srcY = graphic_info->height - win_config->srcH;
        post_scale->dstX = win_config->srcX;
        post_scale->dstY = win_config->srcY;

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
    }

    for (i = 0; i < 1000; i++)
    {
        if (win_config->srcX >= step)
            win_config->srcX -= step;
        else
            win_config->srcX = 0;
        if (win_config->srcY >= step)
            win_config->srcY -= step;
        else
            win_config->srcY = 0;
        if (!win_config->srcX && !win_config->srcY)
            break;
        post_scale->dstX = win_config->srcX;
        post_scale->dstY = win_config->srcY;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(2);
    }
}

static void test_post_scale(uint8_t win_id, struct rt_device *dev,
                            struct CRTC_WIN_STATE *win_config,
                            struct VOP_POST_SCALE_INFO *post_scale)
{
    rt_err_t ret;
    uint16_t src_w, src_h;

    rt_kprintf("Display test: test post scale!\n");

    src_w = post_scale->srcW;
    src_h = post_scale->srcH;

    win_config->winEn = 1;
    win_config->winId = 0;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    display_delay(20);

    post_scale->srcW = src_w / 2;
    win_config->srcW = post_scale->srcW;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    display_delay(20);
    post_scale->srcW = src_w / 4;
    win_config->srcW = post_scale->srcW;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    display_delay(20);

    post_scale->srcH = src_h / 2;
    win_config->srcH = post_scale->srcH;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    display_delay(20);

    post_scale->srcH = src_h / 4;
    win_config->srcH = post_scale->srcH;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    display_delay(20);
}

static void test_post_bcsh(struct rt_device *dev,
                           struct CRTC_WIN_STATE *win_config,
                           struct VOP_POST_SCALE_INFO *post_scale)
{
    rt_err_t ret;
    uint16_t  i;
    struct VOP_BCSH_INFO bcsh_info;

    bcsh_info.brightness = 50;
    bcsh_info.contrast = 50;
    bcsh_info.satCon = 50;
    bcsh_info.hue = 50;

    rt_kprintf("Display test: test post bcsh brightness!\n");

    for (i = 0; i < 100; i++)
    {
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        bcsh_info.brightness = i;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_BCSH, &bcsh_info);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(1);
    }

    rt_kprintf("Display test: test post bcsh contrast!\n");
    bcsh_info.brightness  = 50;
    for (i = 0; i < 100; i++)
    {
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        bcsh_info.contrast = i;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_BCSH, &bcsh_info);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);

        display_delay(1);
    }

    rt_kprintf("Display test: test post bcsh satCon!\n");
    bcsh_info.contrast  = 50;
    for (i = 0; i < 100; i++)
    {
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        bcsh_info.satCon = i;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_BCSH, &bcsh_info);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);

        display_delay(1);
    }

    rt_kprintf("Display test: test post bcsh hue!\n");
    bcsh_info.satCon  = 50;
    for (i = 0; i < 100; i++)
    {
        bcsh_info.satCon = i;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_BCSH, &bcsh_info);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);

        display_delay(1);
    }

    bcsh_info.brightness = 50;
    bcsh_info.contrast = 50;
    bcsh_info.satCon = 50;
    bcsh_info.hue = 50;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_BCSH, &bcsh_info);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    rt_kprintf("Display test: test post bcsh!\n");
}

/**
 *  -          -      -       -     -                 -     -       -
 * |  csc_out0  |    | csc_in0 |   | coe00 coe01 coe02 |   | offset0 |
 * |  csc_out1  | =  | csc_in1 | X | coe10 coe11 coe12 | + | offset1 |
 * |  csc_out2  |    | csc_in2 |   | coe20 coe21 coe22 |   | offset2 |
 *  -          -      -       -     -                 -     -       -
 *
 * bt709tobt2020
 * {0.6274, 0.3293, 0.0433},
 * {0.0691, 0.9195, 0.0114},
 * {0.0164, 0.0880, 0.8956}
 *
 * coe00 = 0.6274 * 0x80  = 0x50
 * coe01 = 0.3293 * 0x80  = 0x2a
 * coe02 = 0.0433 * 0x80 = 0x05
 * offset0 = 0
 * ......
 * coe_3x3 = {
 *     {0x50, 0x2a, 0x05},
 *     {0x05, 0x75, 0x02},
 *     {0x02, 0x08, 0x72}
 * };
 * coe_offset[3] = {0x0, 0x0, 0x0};
 *
 */

static void test_post_matrix_coe(struct rt_device *dev,
                                 struct CRTC_WIN_STATE *win_config,
                                 struct VOP_POST_SCALE_INFO *post_scale)
{
    int i = 0;
    rt_err_t ret;
    struct VOP_COLOR_MATRIX_INFO matrix_info;
    uint8_t coe_3x3[3][3] = {{0x50, 0x2a, 0x05},
        {0x05, 0x75, 0x02},
        {0x02, 0x08, 0x72}
    };
    uint8_t coe_offset[3] = {0x0, 0x0, 0x0};

    matrix_info.colorMatrixCoe = (uint8_t *)coe_3x3;
    matrix_info.colorMatrixOffset = (uint8_t *)coe_offset;

    for (i = 0; i < 20; i++)
    {
        matrix_info.colorMatrixEn = 0;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_COLOR_MATRIX, &matrix_info);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);

        display_delay(4);

        matrix_info.colorMatrixEn = 1;

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_COLOR_MATRIX, &matrix_info);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(4);
    }

    rt_kprintf("Display test: test post matrix coe test\n");
}

static void test_post_gamma_coe(struct rt_device *dev,
                                struct CRTC_WIN_STATE *win_config,
                                struct VOP_POST_SCALE_INFO *post_scale)
{
    rt_err_t ret;
    struct VOP_GAMMA_COE_INFO gamma_info;

    uint8_t gamma_coe0[16] = {0x00, 0x11, 0x22, 0x33, 0x40, 0x51, 0x62, 0x73,
                              0x00, 0x11, 0x22, 0x33, 0x40, 0x51, 0x62, 0x73
                             };

    uint8_t gamma_coe1[16] = {0xb8, 0xc0, 0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf3,
                              0xb8, 0xc0, 0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf3
                             };

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    display_delay(8);

    gamma_info.gammaCoeEnable = 1;
    gamma_info.gammaCoeWord = gamma_coe0;

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_GAMMA_COE, &gamma_info);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    display_delay(8);

    gamma_info.gammaCoeWord = gamma_coe1;

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_GAMMA_COE, &gamma_info);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    display_delay(8);
    gamma_info.gammaCoeEnable = 0;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_GAMMA_COE, &gamma_info);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    rt_kprintf("Display test: test post matrix coe test\n");
    rt_thread_delay(8);
}

static void test_post_clip(struct rt_device *dev,
                           struct CRTC_WIN_STATE *win_config,
                           struct VOP_POST_SCALE_INFO *post_scale)
{
    int i = 0;
    rt_err_t ret;
    struct VOP_POST_CLIP_INFO clip_info;

    clip_info.postClipEn = 1;

    for (i = 0; i < 20; i++)
    {
        clip_info.postYThres = 20;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_POST_CLIP, &clip_info);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);

        display_delay(10);

        clip_info.postYThres = 0;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_POST_CLIP, &clip_info);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(10);
    }

    rt_kprintf("Display test: test post clip test\n");
}

#if 0
static void mipi_switch_thread(void *p)
{
    rt_err_t ret;
    uint8_t path;

    rt_thread_delay(1000);
    while (switch_times++ < 1000)
    {
        /* switch to pisces */
        rt_mutex_take(&display_lock, RT_WAITING_FOREVER);
        rt_kprintf("switch to pisces...\n");
        path = SWITCH_TO_INTERNAL_DPHY;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_MIPI_SWITCH, &path);
        RT_ASSERT(ret == RT_EOK);
        switch_to_full_screen(g_display_dev);
        mipi_switch_to_ap = 0;
        rt_mutex_release(&display_lock);
        rt_thread_delay(3000);

        /* switch to AP */
        rt_mutex_take(&display_lock, RT_WAITING_FOREVER);
        mipi_switch_to_ap = 1;
        switch_to_full_screen(g_display_dev);
        rt_kprintf("switch to ap...\n");
        path = SWITCH_TO_AP_BYPASS;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_MIPI_SWITCH, &path);
        RT_ASSERT(ret == RT_EOK);
        rt_mutex_release(&display_lock);
        rt_thread_delay(3000);
    }
}
#endif

static int test_mipi_switch(struct rt_device *dev,
                            struct CRTC_WIN_STATE *win_config,
                            struct VOP_POST_SCALE_INFO *post_scale,
                            struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret;
    uint16_t i = 0, j = 0, time_out = 0;
    uint16_t step  = SRC_DATA_H * 2;
    uint16_t y_times = graphic_info->height / step - 1;
    rt_thread_t thread = RT_NULL;

    gwin_config = (struct CRTC_WIN_STATE *)rt_malloc(sizeof(struct CRTC_WIN_STATE));
    RT_ASSERT(gwin_config != RT_NULL);
    gpost_scale = (struct VOP_POST_SCALE_INFO *)rt_malloc(sizeof(struct VOP_POST_SCALE_INFO));
    RT_ASSERT(gpost_scale != RT_NULL);
    ggraphic_info = (struct rt_device_graphic_info *)rt_malloc(sizeof(struct rt_device_graphic_info));
    RT_ASSERT(ggraphic_info != RT_NULL);

    memcpy(gwin_config, win_config, sizeof(struct CRTC_WIN_STATE));
    memcpy(gpost_scale, post_scale, sizeof(struct VOP_POST_SCALE_INFO));
    memcpy(ggraphic_info, graphic_info, sizeof(struct rt_device_graphic_info));

    /**
     * use gpio interrupt from ap to switch mipi
     *
     * thread = rt_thread_create("mipi_switch_test", mipi_switch_thread, RT_NULL, 2048, 5, 10);
     * if (thread == RT_NULL)
     *{
     *   return -1;
     * }
     *
     * rt_thread_startup(thread);
     */

    win_config->winId = 0;
    win_config->winEn = 1;

    for (j = 0; j < 1000000; j++)
    {
        if (j % 2 == 0)
        {
            win_config->yrgbAddr = rtt_framebuffer_test;
            win_config->srcX = 0;
        }
        else
        {
            win_config->yrgbAddr = SRC_CBCR_DATA_ADDR;
            win_config->srcX = graphic_info->width / 2;
        }
        win_config->yrgbAddr = rtt_framebuffer_test;
        post_scale->srcW = graphic_info->width;
        post_scale->dstX = 0;
        post_scale->dstY = 0;
        post_scale->dstW = graphic_info->width;

        for (i = 0; i <= y_times; i++)
        {
            while (mipi_switch_to_ap)
            {
                rt_thread_mdelay(1);
                if (time_out++ > 1000)
                {
                    rt_kprintf("mipi switch in ap state\n");
                    time_out = 0;
                }
            }

            rt_mutex_take(&display_lock, RT_WAITING_FOREVER);

            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)0);
            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_ENABLE, NULL);
            RT_ASSERT(ret == RT_EOK);

            win_config->xVir = SRC_DATA_W;
            win_config->yrgbAddr = rtt_framebuffer_test;
            win_config->srcY = step * i;
            if (win_config->srcY + win_config->srcH > graphic_info->height)
                win_config->srcY = graphic_info->height - win_config->srcH;

            post_scale->dstY = win_config->srcY;
            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_SCALE, post_scale);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_PLANE, win_config);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
            RT_ASSERT(ret == RT_EOK);

            display_delay(3);

            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_DISABLE, NULL);
            rt_mutex_release(&display_lock);
        }
    }
    rt_thread_detach(thread);
    rt_free(ggraphic_info);
    rt_free(gpost_scale);
    rt_free(gwin_config);
    return 0;
}

static void switch_work_func(struct rt_work *work, void *work_data)
{
    rt_err_t ret;
    uint8_t path;

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)1);

    if (HAL_GPIO_GetPinLevel(GPIOx, GPIO_PIN_INT) == GPIO_HIGH)
    {
        /* switch to AP */
        rt_mutex_take(&display_lock, RT_WAITING_FOREVER);
        mipi_switch_to_ap = 1;

        switch_to_full_screen(g_display_dev);

        ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWERON, NULL);
        rt_kprintf("switch to ap...\n");
        path = SWITCH_TO_AP_BYPASS;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_MIPI_SWITCH, &path);
        RT_ASSERT(ret == RT_EOK);
        rt_thread_delay(1);
        ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWEROFF, NULL);
        RT_ASSERT(ret == RT_EOK);

        rt_mutex_release(&display_lock);
    }
    else
    {
        if (switch_times++ == 0)
        {
            rt_kprintf("switch_work_func first irq\n");
            return;
        }
        /* switch to M1 */
        rt_mutex_take(&display_lock, RT_WAITING_FOREVER);
        rt_kprintf("switch to pisces...switch_times:%d\n", switch_times);
        ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWERON, NULL);
        RT_ASSERT(ret == RT_EOK);
        rt_thread_delay(1);
        path = SWITCH_TO_INTERNAL_DPHY;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_MIPI_SWITCH, &path);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWEROFF, NULL);

        switch_to_full_screen(g_display_dev);
        mipi_switch_to_ap = 0;

        rt_mutex_release(&display_lock);
    }
}

static void mipi_switch_isr(void *args)
{
    rt_workqueue_dowork(switch_workqueue, &switch_work);
}

static void init_mipi_switch_interrupt(void)
{
    rt_err_t ret;

    rt_kprintf("Enter display test! set isr \n");

    HAL_GPIO_SetPinDirection(GPIOx, GPIO_PIN_INT, GPIO_IN);
    ret = rt_pin_attach_irq(BANK_PIN(GPIO_BANKx, PINNUM_INT),
                            PIN_IRQ_MODE_RISING_FALLING,
                            mipi_switch_isr, "mipi_switch isr");
    RT_ASSERT(ret == RT_EOK);
    ret = rt_pin_irq_enable(BANK_PIN(GPIO_BANKx, PINNUM_INT), PIN_IRQ_ENABLE);
    RT_ASSERT(ret == RT_EOK);
}

static void test_set_plane(uint8_t win_id,
                           struct rt_device *dev,
                           struct CRTC_WIN_STATE *win_config,
                           struct VOP_POST_SCALE_INFO *post_scale,
                           struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret = RT_EOK;
    uint16_t i = 0, j = 0;
    uint16_t step  = 8;
    uint16_t x_times = SRC_DATA_W / 2 / step / 2;
    struct CRTC_WIN_STATE win1_config;
    struct display_sync display_sync_data;

    rt_kprintf("Display test: set plane!\n");
    win_config->winId = win_id;
    win_config->winEn = 1;
    win_config->winUpdate = 1;
    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;

    memcpy(&win1_config, win_config, sizeof(win1_config));

    win_config->srcX = 200;
    win_config->srcY = 65;

    win_config->srcW = 240;
    win_config->srcW = 240;

    win_config->srcH = 2 * SRC_DATA_H;

    win_config->yrgbLength = 0;
    win_config->cbcrLength = 0;
    win1_config.yrgbLength = 0;
    win1_config.cbcrLength = 0;

    display_sync_data.cmd = DISPLAY_SET_ASYNC_MODE;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_DISPLAY_SYNC, &display_sync_data);

    for (i = 0; i < x_times; i++)
    {
        rt_kprintf("Display test: test win:%d loop i: %d , xloopofset:%d!\n", i, win_id, win_config->xLoopOffset);

        win_config->winEn = 1;
        win_config->winUpdate = 1;
        win_config->xLoopOffset = i * 8;
        display_sync_data.winid = 0;
        display_sync_data.addr = 0;
        display_sync_data.cmd = DISPLAY_SYNC;

        for (j = 0; j < 100; j++)
        {
            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_DISPLAY_SYNC, &display_sync_data);
            if (ret == RT_EOK)
            {
                break;
            }
            else
            {
                rt_thread_mdelay(1);
            }
        }

        if (i == 100)
            rt_kprintf("wait RK_DISPLAY_CTRL_DISPLAY_SYNC time out\n");

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        if (1 /*i % 10 == 0*/)
        {
            win1_config.winEn = 1;
            win1_config.winId = 1;
            win1_config.winUpdate = 1;

            win1_config.yLoopOffset = i * 8;

            win1_config.srcW = 320;

            win1_config.srcX = 160;
            win1_config.srcY = 2 * SRC_DATA_H + win_config->srcY; /* + 1; */
            win1_config.srcH = win_config->srcH;
            win1_config.srcW = 720;

            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_PLANE, &win1_config);
            RT_ASSERT(ret == RT_EOK);
        }

        rt_thread_mdelay(100);
    }
}

static void test_display_clear_screen(struct rt_device *dev,
                                      struct CRTC_WIN_STATE *win_config,
                                      struct VOP_POST_SCALE_INFO *post_scale,
                                      struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret;

    post_scale->srcW = graphic_info->width;
    post_scale->srcH = graphic_info->height;
    post_scale->dstX = 0;
    post_scale->dstY = 0;
    post_scale->dstW = graphic_info->width;
    post_scale->dstH = graphic_info->height;

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    win_config->winEn = 0;
    win_config->winUpdate = 1;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    rt_kprintf("Display test: test_display_init!\n");
    display_delay(5);
}

static void usage(void)
{
    rt_kprintf("Display test utility \n\n");
    rt_kprintf("display_test -c <case> [-b <rgb buffer addr>[,<yuv buffer addr>]] [-s <wxh>]\n\n");
    rt_kprintf("-c (winmove|aod|bcsh|bl|clip|coe|color_bar|gamma|mipi_switch|scale|winalpha|winloop) test case\n");
    rt_kprintf("-b      rgb/yuv framebuffer address\n");
    rt_kprintf("-s wxh  set display width and height \n");
    rt_kprintf("Examples:\n");
    rt_kprintf("    display_test -c winmove    Test winmove\n");
    rt_kprintf("    display_test -c winmove -b 0x2080000     Test winmove, fb at 0x20080000\n");
    rt_kprintf("    display_test -c winmove -b 0x20080000 -s 320x480    Test winmove, fb at 0x20080000, image 320x480\n\n");
}

static void test_color_bar(uint8_t win_id,
                           struct rt_device *dev,
                           struct CRTC_WIN_STATE *win_config,
                           struct VOP_POST_SCALE_INFO *post_scale,
                           struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret;
    uint16_t i = 0;
    uint16_t step  = 8;
    uint16_t x_times = SRC_DATA_W / 2 / step;
#ifdef RT_USING_PANEL_S6E3HC2_X1
    uint16_t j = 0;
    uint16_t y_times = graphic_info->height / SRC_DATA_H - 1;
#endif

    rt_kprintf("Display test: color bar test!\n");
    win_config->winId = win_id;
    win_config->winEn = 1;
    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;
    win_config->srcX = (graphic_info->width - SRC_DATA_W) / 2;
    win_config->srcY = (graphic_info->height - SRC_DATA_H) / 2;
    win_config->srcW = SRC_DATA_W;
    win_config->srcH = SRC_DATA_H;
    win_config->srcY = AlignDown(win_config->srcY, Y_ALIGNED);

    post_scale->srcW = graphic_info->width;
    post_scale->srcH = win_config->srcH;
    post_scale->dstX = 0;
    post_scale->dstY = win_config->srcY;
    post_scale->dstW = graphic_info->width;
    post_scale->dstH = win_config->srcH;

    for (i = 0; i < x_times; i++)
    {
        win_config->xLoopOffset = i * 8;
        rt_kprintf("Display test: color bar test win%d xloopofset:%d!\n", win_id, win_config->xLoopOffset);
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        RT_ASSERT(ret == RT_EOK);

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
        RT_ASSERT(ret == RT_EOK);
        display_delay(1);
    }

#ifdef RT_USING_PANEL_S6E3HC2_X1
    win_config->xLoopOffset = 0;

    for (j = 0; j < y_times; j++)
    {
        win_config->srcY = SRC_DATA_H * j;

        for (i = 0; i < x_times * 8; i++)
        {
            win_config->srcX = i * 16;

            if (win_config->srcX + SRC_DATA_W > graphic_info->width)
                break;

            post_scale->dstY = win_config->srcY;

            rt_kprintf("Display test: color bar test win%d xloopofset:%d!\n", win_id, win_config->xLoopOffset);
            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_SCALE, post_scale);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_SET_PLANE, win_config);
            RT_ASSERT(ret == RT_EOK);

            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
            RT_ASSERT(ret == RT_EOK);
            display_delay(1);
        }
    }
#endif
    display_win_init(win_config);
}

static void test_demo(uint8_t win_id,
                      struct rt_device *dev,
                      struct CRTC_WIN_STATE *win_config,
                      struct VOP_POST_SCALE_INFO *post_scale,
                      struct rt_device_graphic_info *graphic_info,
                      char *cmd)
{
    rt_err_t ret;

    rt_kprintf("Display test: %s test_demo!\n", cmd);
    win_config->winId = win_id;
    win_config->winEn = 1;
    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;
    win_config->srcW = SRC_DATA_W;
    win_config->srcH = SRC_DATA_H;

    if (!rt_strcmp(cmd, "clock"))
    {
        win_config->srcX = (graphic_info->width - SRC_DATA_W) / 2;
        win_config->srcY = (graphic_info->height - SRC_DATA_H) / 2;
    }
    else if (!rt_strcmp(cmd, "ebook"))
    {
        win_config->srcX = 0;
        win_config->srcY = 0;
    }

    post_scale->srcW = graphic_info->width;
    post_scale->srcH = win_config->srcH;
    post_scale->dstX = 0;
    post_scale->dstY = win_config->srcY;
    post_scale->dstW = graphic_info->width;
    post_scale->dstH = win_config->srcH;

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    display_delay(1);
}

static void parse_framebuffer(char *arg)
{
    char *str = strtok(arg, ",");

    if (!str)
        return;
    rtt_framebuffer_test = strtoul(str, NULL, 16);
    str = strtok(NULL, ",");
    if (!str)
        return;
    rtt_framebuffer_uv = strtoul(str, NULL, 16);
}

static uint32_t parse_image_size(char *arg)
{
    char *end;
    uint16_t w, h;

    w = strtoul(arg, &end, 10);
    if (*end != 'x')
        return 0;

    arg = end + 1;
    h = strtoul(arg, &end, 10);

    return (h << 16) + w;
}

static int display_test(int argc, char **argv)
{
    rt_err_t ret = RT_EOK;
    struct CRTC_WIN_STATE *win_config;
    struct VOP_POST_SCALE_INFO *post_scale;
    struct rt_device_graphic_info *graphic_info;
    struct crtc_lut_state lut_state;
    char *cmd = NULL;
    uint16_t  i;
    uint32_t fb_length, val;
    void *planes[3];
    unsigned int pitches[4];
#ifdef RKMCU_PISCES
    int path = 0;
#endif
    int opt;
    uint16_t w, h;

    if (argc == 1)
    {
        usage();
        return 0;
    }

    optind = 0;
    while ((opt = getopt(argc, argv, "c:b:s:")) > 0)
    {
        switch (opt)
        {
        case 'c':
            cmd = optarg;
            break;
        case 'b':
            parse_framebuffer(optarg);
            break;
        case 's':
            val = parse_image_size(optarg);
            if (!val)
            {
                rt_kprintf("invalid arguments for image size\n");
                return -RT_EINVAL;
            }
            w = val & 0xffff;
            h = val >> 16;
            rt_kprintf("image size: %dx%d\n", w, h);
            break;
        default:
            rt_kprintf("Unknown option: %c\n", opt);
            usage();
        }
    }

    if (cmd)
    {
        rt_kprintf("Enter display %s test! \n", cmd);

        //if (!rt_strcmp(cmd, "color_bar"))
        if (1)
        {
            pitches[0] = get_stride(SRC_DATA_FMT, SRC_DATA_W);
            fb_length = pitches[0] * SRC_DATA_H;
            if (!rtt_framebuffer_test)
                rtt_framebuffer_test = framebuffer_alloc(fb_length);
            if (rtt_framebuffer_test == RT_NULL)
            {
                rt_kprintf("malloc memory: 0x%x failed\n", fb_length);
                return -RT_ENOMEM;
            }

            rt_memset((void *)rtt_framebuffer_test, 0, fb_length);
            planes[0] = (void *)rtt_framebuffer_test;

            if (SRC_DATA_FMT == RTGRAPHIC_PIXEL_FORMAT_YUV420)
            {
                if (!rtt_framebuffer_uv)
                    rtt_framebuffer_uv = framebuffer_alloc(fb_length);
                if (rtt_framebuffer_uv == RT_NULL)
                {
                    rt_kprintf("malloc memory: 0x%x failed\n", fb_length);
                    return -RT_ENOMEM;
                }
                planes[1] = (void *)rtt_framebuffer_uv;
            }

            util_fill_pattern(SRC_DATA_FMT, planes, SRC_DATA_W, SRC_DATA_H, pitches[0]);

            HAL_DCACHE_CleanByRange((uint32_t)rtt_framebuffer_test, fb_length);
            if (SRC_DATA_FMT == RTGRAPHIC_PIXEL_FORMAT_YUV420)
                HAL_DCACHE_CleanByRange((uint32_t)rtt_framebuffer_uv, fb_length);
        }
        else
        {
            flush_cache();
        }

        g_display_dev = rt_device_find("lcd");
        RT_ASSERT(g_display_dev != RT_NULL);

        ret = rt_device_open(g_display_dev, RT_DEVICE_FLAG_RDWR);
        RT_ASSERT(ret == RT_EOK);

        g_backlight_dev = rt_device_find("backlight");
        if (g_backlight_dev)
        {
            int brightness = 0; /* todo: backlight driver only support this map for brightness control now */
            rt_kprintf("Backlight power on\n");
            rt_device_control(g_backlight_dev, RTGRAPHIC_CTRL_POWERON, NULL);
            rt_device_control(g_backlight_dev, RTGRAPHIC_CTRL_RECT_UPDATE,
                              &brightness);
        }
        else
        {
            rt_kprintf("can't find backlight dev\n");
        }

#ifdef RKMCU_PISCES
        if (!rt_strcmp(cmd, "mipi_switch"))
        {
            ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)1);
            RT_ASSERT(ret == RT_EOK);
        }

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_ENABLE, NULL);
        path = SWITCH_TO_INTERNAL_DPHY;
        ret = rt_device_control(g_display_dev,
                                RK_DISPLAY_CTRL_MIPI_SWITCH, &path);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_DISABLE, NULL);
#endif

        ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)0);
        ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWERON, NULL);
        RT_ASSERT(ret == RT_EOK);

        graphic_info = (struct rt_device_graphic_info *)rt_calloc(1, sizeof(struct rt_device_graphic_info));
        RT_ASSERT(graphic_info != RT_NULL);

        ret = rt_device_control(g_display_dev,
                                RTGRAPHIC_CTRL_GET_INFO, (void *)graphic_info);
        RT_ASSERT(ret == RT_EOK);

        win_config = (struct CRTC_WIN_STATE *)rt_calloc(1, sizeof(struct CRTC_WIN_STATE));
        RT_ASSERT(win_config != RT_NULL);

        display_win_init(win_config);
        if (rt_mutex_init(&(display_lock), "displayLock", RT_IPC_FLAG_FIFO) != RT_EOK)
        {
            RT_ASSERT(0);
        }
        if ((win_config->format >= RTGRAPHIC_PIXEL_FORMAT_GRAY1) &&
                (win_config->format <= RTGRAPHIC_PIXEL_FORMAT_GRAY256))
        {
            lut_state.win_id = 0;
            lut_state.lut = bpp_lut;
            lut_state.lut_size = sizeof(bpp_lut) / sizeof(bpp_lut[0]);

            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_LOAD_LUT, &lut_state);
            RT_ASSERT(ret == RT_EOK);

            lut_state.win_id = 1;
            ret = rt_device_control(g_display_dev,
                                    RK_DISPLAY_CTRL_LOAD_LUT, &lut_state);
            RT_ASSERT(ret == RT_EOK);
        }

        post_scale = (struct VOP_POST_SCALE_INFO *)rt_calloc(1, sizeof(struct VOP_POST_SCALE_INFO));
        RT_ASSERT(post_scale != RT_NULL);

        rt_thread_mdelay(20);
        test_display_clear_screen(g_display_dev, win_config, post_scale, graphic_info);

        post_scale->srcW = win_config->srcW;
        post_scale->srcH = win_config->srcH;
        post_scale->dstX = win_config->srcX;
        post_scale->dstY = win_config->srcY;
        post_scale->dstW = win_config->srcW;
        post_scale->dstH = win_config->srcH;

        if (!rt_strcmp(cmd, "dsc"))
        {
            test_dsc_display(0, g_display_dev, win_config, post_scale, graphic_info);
            disable_all_win(g_display_dev, win_config);
        }
        else if (!rt_strcmp(cmd, "winloop"))
        {
            for (i = 0; i < 3; i++)
            {
                test_win_loop(i, g_display_dev, win_config, post_scale, graphic_info);
                disable_all_win(g_display_dev, win_config);
                display_win_init(win_config);
            }
        }
        else if (!rt_strcmp(cmd, "winmove"))
        {
            display_win_init(win_config);
            test_win_move(0, g_display_dev, win_config, post_scale, graphic_info);
            disable_all_win(g_display_dev, win_config);
        }
        else if (!rt_strcmp(cmd, "winalpha"))
        {
            display_win_init(win_config);
            test_win_alpha(g_display_dev, win_config, post_scale);
            disable_all_win(g_display_dev, win_config);
        }
        else if (!rt_strcmp(cmd, "scale"))
        {
            display_win_init(win_config);
            test_post_scale(0, g_display_dev, win_config, post_scale);
            disable_all_win(g_display_dev, win_config);
        }
        else if (!rt_strcmp(cmd, "bcsh"))
        {
            display_win_init(win_config);
            test_post_bcsh(g_display_dev, win_config, post_scale);
        }
        else if (!rt_strcmp(cmd, "coe"))
        {
            test_post_matrix_coe(g_display_dev, win_config, post_scale);
        }
        else if (!rt_strcmp(cmd, "gamma"))
        {
            test_post_gamma_coe(g_display_dev, win_config, post_scale);
        }
        else if (!rt_strcmp(cmd, "clip"))
        {
            test_post_clip(g_display_dev, win_config, post_scale);
        }
        else if (!rt_strcmp(cmd, "mipi_switch"))
        {
            switch_workqueue = rt_workqueue_create("switchWQ", 1024, 5);
            RT_ASSERT(switch_workqueue != NULL);
            init_mipi_switch_interrupt();
            rt_work_init(&switch_work, switch_work_func, NULL);
            test_mipi_switch(g_display_dev, win_config, post_scale, graphic_info);
        }
        else if (!rt_strcmp(cmd, "set_plane")) /* async mode test */
        {
            test_set_plane(0, g_display_dev, win_config, post_scale, graphic_info);
        }
        else if (!rt_strcmp(cmd, "color_bar"))
        {
            test_color_bar(0, g_display_dev, win_config, post_scale, graphic_info);
        }
        else if (!rt_strcmp(cmd, "dsp_dtcm"))
        {
            struct rt_device *dsp_dev;

            dsp_dev = rt_device_find("dsp0");
            RT_ASSERT(dsp_dev != RT_NULL);
            rt_device_open(dsp_dev, RT_DEVICE_OFLAG_RDWR);

            memcpy((void *)dsp_dtcm_addr, (void *)rtt_framebuffer_test, fb_length);
            win_config->yrgbAddr = dsp_dtcm_addr;
            win_config->yrgbLength = fb_length;

            test_color_bar(0, g_display_dev, win_config, post_scale, graphic_info);
        }
        else if (!rt_strcmp(cmd, "bl"))
        {
            uint16_t brightness = 200;

            if (argv[3])
            {
                brightness = atol(argv[3]);
                ret = rt_device_control(g_display_dev,
                                        RK_DISPLAY_CTRL_UPDATE_BL, &brightness);

                test_demo(0, g_display_dev, win_config, post_scale, graphic_info, cmd);
            }
            else
                rt_kprintf("Usage: display_test bl brightness_value!\n");
        }
        else if (!rt_strcmp(cmd, "aod"))
        {
            if (!rt_strcmp(argv[3], "on"))
            {
                ret = rt_device_control(g_display_dev,
                                        RK_DISPLAY_CTRL_SET_AOD_ON, NULL);
            }
            else if (!rt_strcmp(argv[3], "off"))
            {
                ret = rt_device_control(g_display_dev,
                                        RK_DISPLAY_CTRL_SET_AOD_OFF, NULL);
            }
            else
                rt_kprintf("Usage: display_test aod on or off!\n");
            test_demo(0, g_display_dev, win_config, post_scale, graphic_info, cmd);
        }
        else
        {
            test_demo(0, g_display_dev, win_config, post_scale, graphic_info, cmd);
        }

        if (g_backlight_dev)
        {
            int brightness = 100; /* todo: backlight driver only support this map for brightness control now */
            rt_kprintf("Backlight power off\n");
            rt_device_control(g_backlight_dev, RTGRAPHIC_CTRL_POWEROFF, NULL);
            rt_device_control(g_backlight_dev, RTGRAPHIC_CTRL_RECT_UPDATE,
                              &brightness);
        }

        ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWEROFF, NULL);
        RT_ASSERT(ret == RT_EOK);

        rt_free(graphic_info);
        rt_free(post_scale);
        rt_free(win_config);

        framebuffer_free((void *)rtt_framebuffer_test);
        if (SRC_DATA_FMT == RTGRAPHIC_PIXEL_FORMAT_YUV420)
            framebuffer_free((void *)rtt_framebuffer_uv);
        rtt_framebuffer_test = NULL;
        rtt_framebuffer_uv = NULL;
        rt_kprintf("Exit display test!\n");
    }
    else
        usage();

    return ret;
}

static int _at_display_test(void)
{
    char *argv[2] = {"display_test", "color_bar"};

    return display_test(2, argv);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(display_test, display drive test. e.g: display_test);
MSH_CMD_EXPORT(flush_cache, flush cache. e.g: flush_cache);
FINSH_FUNCTION_EXPORT(_at_display_test, display test for auto test);
#endif
#endif
