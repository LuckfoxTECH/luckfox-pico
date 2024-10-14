/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    spi_screen_test.c
  * @version V0.1
  * @brief   spi screen test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-02-17     Chris Zhong     first implementation
  *
  ******************************************************************************
  */
#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_SPI_SCREEN_TEST
#include "drv_heap.h"
#include "display_pattern.h"

#define SRC_DATA_FMT    RTGRAPHIC_PIXEL_FORMAT_RGB565

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

static void spi_screen_test(int argc, char **argv)
{
    struct rt_device_graphic_info graphic_info;
    static struct rt_device *g_display_dev;
    uint32_t rtt_framebuffer_test;
    uint32_t fb_length;
    void *planes[3];
    int stride;
    rt_err_t ret;

    rt_kprintf("%s !\n", __func__);

    g_display_dev = rt_device_find("spi_screen");
    RT_ASSERT(g_display_dev != RT_NULL);

    ret = rt_device_open(g_display_dev, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_GET_INFO, &graphic_info);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWERON, NULL);
    RT_ASSERT(ret == RT_EOK);

    stride = get_stride(graphic_info.pixel_format, graphic_info.width);

    fb_length = graphic_info.width * graphic_info.height * graphic_info.bits_per_pixel / 8;

    if (graphic_info.framebuffer)
    {
        rtt_framebuffer_test = (uint32_t)graphic_info.framebuffer;
    }
    else
    {
        rtt_framebuffer_test = (uint32_t)rt_malloc(fb_length);
        if (rtt_framebuffer_test == RT_NULL)
        {
            rt_kprintf("malloc memory: 0x%x failed\n", fb_length);
            return;
        }

        graphic_info.framebuffer = (void *)rtt_framebuffer_test;
    }

    planes[0] = (void *)rtt_framebuffer_test;
    rt_memset((void *)rtt_framebuffer_test, 0, fb_length);

    util_fill_pattern(graphic_info.pixel_format, planes, graphic_info.width, graphic_info.height, stride);

    ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_RECT_UPDATE, &graphic_info);

    if (graphic_info.framebuffer == NULL)
        rt_free((void *)rtt_framebuffer_test);

    rt_kprintf("Exit display test!\n");
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(spi_screen_test, spi screen drive test. e.g: spi_screen_test);
#endif
#endif
