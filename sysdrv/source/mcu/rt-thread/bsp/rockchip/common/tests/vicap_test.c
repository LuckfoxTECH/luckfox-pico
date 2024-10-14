/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    vicap_test.c
  * @version V0.0.1
  * @brief   camera interface processor test for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_VICAP

#ifdef RT_USING_COMMON_TEST_VICAP

#include "drv_vicap.h"
#include "drv_heap.h"
#include "stdlib.h"
#include "drv_display.h"
#include "display_pattern.h"

#define VICAP_TEST_DEBUG                    1
#define VICAP_ENABLE_SAMPLE_RAW8            0

#if defined(VICAP_TEST_DEBUG)
#define VICAP_TEST_INFO(...)                \
        do { \
            rk_kprintf("[VICAP-TEST]:");    \
            rk_kprintf(__VA_ARGS__);    \
            }while(0)
#else
#define VICAP_TEST_INFO(...)
#endif

#define DISPLAY_DEVICE_NAME                     "lcd"
#define DEVICE_NAME_SIZE                        (12)
#define FRAME_Y_SIZE(width, height)             ((width) * (height))
#define FRAME_UV_SIZE(width, height)            ((width) * (height) >> 1)

typedef enum
{
    STREAM_MODE_PHOTO = 0x01,
    STREAM_MODE_PREVIEW,
    STREAM_MODE_POWER_LITTLE,
    STREAM_MODE_POWER_LOW
} eVICAP_StreamMode;

struct buffer
{
    void *start;
    uint32_t framesize;
    uint32_t ysize_frame;
    uint32_t ysize_block;
    uint32_t uvsize_block;
    uint32_t totalsize;
    uint32_t width;
    uint32_t height;
};

struct parameters
{
    char dev_name[DEVICE_NAME_SIZE];
    uint32_t buf_num;
    uint32_t frame_num;
    uint32_t block_num;
    uint32_t skip_num;
    int32_t sample_buf;
    int32_t skip_buf;
    eVICAP_workMode workmode;
    eVICAP_StreamMode streammode;
    struct rk_camera_pix_format pixformat;
    struct rk_camera_crop_rect crop;
    struct vicap_video_requestbuffers reqbuf;
};

static struct buffer image;
static struct rk_vicap_dev *vicapdev;
static struct parameters options =
{
    .dev_name   = "vicap_0",
    .frame_num  = 1,
    .buf_num    = 6,
    .block_num  = 8,
    .skip_num   = 2,
    .workmode   = VICAP_WORKMODE_BLOCK_PINGPONG,
    .streammode = STREAM_MODE_PHOTO,
    .pixformat  = {
        .pixelformat    = V4L2_PIX_FMT_NV12,
        .width          = 640,
        .height         = 480,
    },
};

#ifdef RT_USING_VICAP_TEST_DISPLAY
static rk_device *dev_lcd;
static struct rt_device_graphic_info *graphic_info;
static struct CRTC_WIN_STATE *win_config;
static struct VOP_POST_SCALE_INFO *post_scale;

static void sample_single_frame(struct vicap_video_buf *buf,
                                uint8_t *framestart)
{
    uint8_t *ydst;
    uint8_t *uvdst;
    uint8_t *ysrc;
    uint8_t *uvsrc;
    uint8_t *ystart;
    uint8_t *uvstart;
    static bool startcap = false;
    static uint32_t startframeid;

    if (options.workmode == VICAP_WORKMODE_BLOCK_PINGPONG)
    {
        /* first block of every frame */
        if (buf->id.block_id == 0 && !startcap)
        {
            startcap = true;
            startframeid = buf->id.frame_id;
        }

        if (startcap)
        {
            ystart = framestart;
            ydst = ystart + image.ysize_block * buf->id.block_id;
            ysrc = buf->addr;
            rk_memcpy(ydst, ysrc, image.ysize_block);

            if (VICAP_ENABLE_SAMPLE_RAW8 != true)
            {
                uvstart = ystart + image.ysize_frame;
                uvdst = uvstart + image.uvsize_block * buf->id.block_id;
                uvsrc = buf->addr + image.ysize_block;
                rk_memcpy(uvdst, uvsrc, image.uvsize_block);
            }
        }

        /* accomplish sampling one frame */
        if (startframeid == buf->id.frame_id &&
                buf->id.block_id == (options.block_num - 1) &&
                startcap)
        {
            startcap = false;
            startframeid = 0x0;
        }
    }
    else
    {
        /* sample for oneframe and framepingpong */
        ystart = framestart;
        ydst = ystart;
        ysrc = buf->addr;
        rk_memcpy(ydst, ysrc, image.ysize_frame);

        if (VICAP_ENABLE_SAMPLE_RAW8 != true)
        {
            uvstart = ystart + image.ysize_frame;
            uvdst = uvstart;
            uvsrc = buf->addr + image.ysize_frame;
            rk_memcpy(uvdst, uvsrc, image.framesize - image.ysize_frame);
        }
    }
}
#endif

static void stream_pre_operation(void)
{
    uint32_t width, height;

    if (options.workmode == VICAP_WORKMODE_BLOCK_PINGPONG)
    {
        options.sample_buf = options.frame_num * options.block_num;
        if (options.skip_num != 0)
        {
            options.skip_buf = options.skip_num * options.block_num;
        }
        else
        {
            options.skip_buf = 0;
        }
    }
    else
    {
        options.block_num = 1;
        options.sample_buf = options.frame_num;
        if (options.skip_num != 0)
        {
            options.skip_buf = options.skip_num;
        }
        else
        {
            options.skip_buf = 0;
        }
    }

    if (options.crop.width != 0 && options.crop.height != 0)
    {
        width = options.crop.width;
        height = options.crop.height;
    }
    else
    {
        width = options.pixformat.width;
        height = options.pixformat.height;
    }

    image.width = width;
    image.height = height;
    image.ysize_frame = FRAME_Y_SIZE(width, height);
    image.ysize_block = image.ysize_frame / options.block_num;
    if (VICAP_ENABLE_SAMPLE_RAW8 != true)
    {
        image.uvsize_block = FRAME_UV_SIZE(width, height) / options.block_num;
        image.framesize = FRAME_Y_SIZE(width, height) + FRAME_UV_SIZE(width, height);
        image.totalsize = image.framesize * options.frame_num;
#ifdef RT_USING_VICAP_TEST_DISPLAY
        image.start = rk_dma_malloc_large(image.totalsize);
#endif
    }
    else
    {
        image.uvsize_block = 0;
        image.framesize = FRAME_Y_SIZE(width, height);
        image.totalsize = image.framesize * options.frame_num;
#ifdef RT_USING_VICAP_TEST_DISPLAY
        image.start = rk_dma_malloc_large(image.totalsize);
#endif
    }
    VICAP_TEST_INFO("image start:0x%x,fsize:0x%x,tsize:0x%x,yszie_frame:0x%x,ysize_block:0x%x,uvsize_block:0x%x\n",
                    (uint32_t)image.start,
                    image.framesize,
                    image.totalsize,
                    image.ysize_frame,
                    image.ysize_block,
                    image.uvsize_block);

}

#ifdef RT_USING_VICAP_TEST_DISPLAY
static void display_config_win_layer(struct CRTC_WIN_STATE *win_config,
                                     struct rt_device_graphic_info *graphic_info,
                                     uint8_t *yaddr,
                                     uint8_t *uvaddr,
                                     uint32_t width,
                                     uint32_t height)
{
    win_config->winEn = true;
    win_config->winUpdate = true;
    win_config->winId = 0;
    win_config->zpos = 0;
    win_config->format = RTGRAPHIC_PIXEL_FORMAT_YUV420;

    win_config->yrgbAddr = (uint32_t)yaddr;
    win_config->cbcrAddr = (uint32_t)uvaddr;

    win_config->yrgbLength = 0;
    win_config->cbcrLength = 0;
    win_config->xVir = (uint16_t)width;

    win_config->srcW = (uint16_t)width;
    win_config->srcH = (uint16_t)height;
    win_config->srcX = (graphic_info->width - win_config->srcW) / 2;
    win_config->srcY = (graphic_info->height - win_config->srcH) / 2;;

    win_config->crtcX = 0;
    win_config->crtcY = 0;
    win_config->crtcW = (uint16_t)width;
    win_config->crtcH = (uint16_t)height;

    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;

    win_config->alphaEn = 0;
    win_config->alphaMode = VOP_ALPHA_MODE_PER_PIXEL;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;

}

static void display_config_post_scale(struct VOP_POST_SCALE_INFO *post_scale,
                                      struct CRTC_WIN_STATE *win_config,
                                      struct rt_device_graphic_info *graphic_info)
{
    post_scale->srcW = graphic_info->width;
    post_scale->srcH = win_config->srcH;
    post_scale->dstX = 0;
    post_scale->dstY = win_config->srcY;
    post_scale->dstW = graphic_info->width;
    post_scale->dstH = win_config->srcH;
}

static void display_post_operation(rk_device *dev_lcd)
{
    ret_err_t ret;

    ret = rk_device_control(dev_lcd, RK_DISPLAY_CTRL_DISABLE, NULL);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("disable lcd failed!\n");
        return;
    }

    rk_free(graphic_info);
    rk_free(win_config);
    rk_free(post_scale);
}

static void display_pre_operation(rk_device *dev_lcd)
{
    ret_err_t ret;

    ret = rk_device_open(dev_lcd, RT_DEVICE_FLAG_RDWR);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("open lcd failed!\n");
        return;
    }

    ret = rt_device_control(dev_lcd, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)0);
    ret = rt_device_control(dev_lcd, RTGRAPHIC_CTRL_POWERON, NULL);
    RT_ASSERT(ret == RT_EOK);

#if 0
    ret = rk_device_control(dev_lcd, RK_DISPLAY_CTRL_ENABLE, NULL);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("enable lcd failed!\n");
        return;
    }
#endif

    graphic_info = (struct rt_device_graphic_info *)rk_malloc(sizeof(struct rt_device_graphic_info));
    if (!graphic_info)
    {
        VICAP_TEST_INFO("malloc graphic_info failed!\n");
        return;
    }

    ret = rk_device_control(dev_lcd,
                            RTGRAPHIC_CTRL_GET_INFO,
                            graphic_info);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("get graphic info failed\n");
        return;
    }

    VICAP_TEST_INFO("graphic info:width:%d,height:%d,bitspp:%d\n",
                    graphic_info->width, graphic_info->height, graphic_info->bits_per_pixel);

    win_config = (struct CRTC_WIN_STATE *)rk_malloc(sizeof(struct CRTC_WIN_STATE));
    if (!win_config)
    {
        VICAP_TEST_INFO("malloc win_config failed!\n");
        return;
    }

    post_scale = (struct VOP_POST_SCALE_INFO *)rk_malloc(sizeof(struct VOP_POST_SCALE_INFO));
    if (!post_scale)
    {
        VICAP_TEST_INFO("malloc post_scale failed!\n");
        return;
    }

}
#endif

static void stream_mode_photograph(rk_device *dev)
{
    ret_err_t ret;
    bool startcap;
    int frmcnt, blkcnt;
#ifdef RT_USING_VICAP_TEST_DISPLAY
    uint8_t *framestart;
#endif
    struct vicap_video_buf buf;

    for (frmcnt = 0, startcap = false; frmcnt < options.frame_num;
            startcap ? frmcnt++ : (frmcnt = frmcnt))
    {
        for (blkcnt = 0, startcap = false; blkcnt < options.block_num;
                startcap ? blkcnt++ : (blkcnt = blkcnt))
        {
            ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_DQBUF, &buf);
            if (ret != RET_SYS_EOK)
            {
                VICAP_TEST_INFO("dqueue buf fail,stop streaming,count:%d!\n", blkcnt);
                return;
            }
            else
            {
#ifdef RT_USING_VICAP_TEST_DISPLAY
                framestart = (uint8_t *)image.start + frmcnt * image.framesize;
                if (options.workmode == VICAP_WORKMODE_BLOCK_PINGPONG)
                {
                    /* first block of every frame */
                    if (buf.id.block_id == 0x0 && !startcap)
                    {
                        startcap = true;
                    }
                }
                else
                {
                    if (!startcap)
                        startcap = true;
                }

                if (startcap)
                {
                    sample_single_frame(&buf, framestart);
                }
#else
                startcap = true;
#endif
            }

            ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_QBUF, &buf);
            if (ret != RET_SYS_EOK)
            {
                VICAP_TEST_INFO("queue buf[%d] failed on streaming!\n", buf.index);
            }
        }
    }

    ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_STREAM_OFF, RK_NULL);
    ret = rk_device_close(dev);

#ifdef RT_USING_VICAP_TEST_DISPLAY
    for (frmcnt = 0; frmcnt < options.frame_num; frmcnt++)
    {
        uint8_t *yaddr = RK_NULL, *uvaddr = RK_NULL;

        yaddr = (uint8_t *)image.start + frmcnt * image.framesize;
        uvaddr = yaddr + image.ysize_frame;

        HAL_DCACHE_CleanByRange((uint32_t)yaddr, image.framesize);
        display_config_win_layer(win_config, graphic_info, yaddr, uvaddr,
                                 image.width, image.height);
        display_config_post_scale(post_scale, win_config, graphic_info);

        ret = rk_device_control(dev_lcd,
                                RK_DISPLAY_CTRL_SET_SCALE, post_scale);
        MACRO_ASSERT(ret == RET_SYS_EOK);

        ret = rk_device_control(dev_lcd,
                                RK_DISPLAY_CTRL_SET_PLANE, win_config);
        MACRO_ASSERT(ret == RET_SYS_EOK);

        ret = rk_device_control(dev_lcd, RK_DISPLAY_CTRL_COMMIT, NULL);
        MACRO_ASSERT(ret == RET_SYS_EOK);

        HAL_DelayMs(1000);
    }
#endif

}

/* when enable preview mode, options.frame_num must be set as 2 for displaying */
static void stream_mode_preview(rk_device *dev, bool is_sample)
{
    ret_err_t ret;
    bool startcap;
    int frmcnt, blkcnt;
    static int frmphase;
    struct vicap_video_buf buf;
#ifdef RT_USING_VICAP_TEST_DISPLAY
    uint8_t *framestart;
    uint8_t *yaddr = RK_NULL, *uvaddr = RK_NULL;
#endif

    /* sample one frame to display */
    (frmphase == 1) ? (frmcnt = 1) : (frmcnt = 0);
    for (; frmcnt <= 1; frmcnt = 2)
    {
        for (blkcnt = 0, startcap = false; blkcnt < options.block_num;
                startcap ? blkcnt++ : (blkcnt = blkcnt))
        {
            ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_DQBUF, &buf);
            if (ret != RET_SYS_EOK)
            {
                VICAP_TEST_INFO("dqueue buf fail,stop streaming,count:%d!\n", blkcnt);
                return;
            }
            else
            {
#ifdef RT_USING_VICAP_TEST_DISPLAY
                framestart = (uint8_t *)image.start + frmcnt * image.framesize;
                if (options.workmode == VICAP_WORKMODE_BLOCK_PINGPONG)
                {
                    /* first block of every frame */
                    if (buf.id.block_id == 0x0 && !startcap)
                    {
                        startcap = true;
                    }
                }
                else
                {
                    if (!startcap)
                        startcap = true;
                }

                if (startcap)
                {
                    sample_single_frame(&buf, framestart);
                }
#else
                startcap = true;
#endif
            }

            ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_QBUF, &buf);
            if (ret != RET_SYS_EOK)
            {
                VICAP_TEST_INFO("queue buf[%d] failed on streaming!\n", buf.index);
            }
        }

#ifdef RT_USING_VICAP_TEST_DISPLAY
        if (startcap)
        {
            yaddr = (uint8_t *)framestart;
            uvaddr = yaddr + image.ysize_frame;

            HAL_DCACHE_CleanByRange((uint32_t)yaddr, image.framesize);
            display_config_win_layer(win_config, graphic_info, yaddr, uvaddr,
                                     image.width, image.height);
            display_config_post_scale(post_scale, win_config, graphic_info);

            ret = rk_device_control(dev_lcd,
                                    RK_DISPLAY_CTRL_SET_SCALE, post_scale);
            MACRO_ASSERT(ret == RET_SYS_EOK);

            ret = rk_device_control(dev_lcd,
                                    RK_DISPLAY_CTRL_SET_PLANE, win_config);
            MACRO_ASSERT(ret == RET_SYS_EOK);

            ret = rk_device_control(dev_lcd, RK_DISPLAY_CTRL_COMMIT, NULL);
            MACRO_ASSERT(ret == RET_SYS_EOK);
        }
#endif
    }

    (frmphase == 1) ? (frmphase = 0) : (frmphase = 1);

    if (!is_sample)
    {
        ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_STREAM_OFF, RK_NULL);
        ret = rk_device_close(dev);
    }
}

static int option_parse_workmode(char *option)
{
    char *cmp;
    char val[10];
    int workmode = -1, len = 0;

    cmp = option;
    if (cmp)
    {
        cmp += rk_strlen("--set-workmode=");
        len = rk_strlen(cmp);

        rk_memset(val, '\0', 10);
        rk_strncpy(val, cmp, len);

        if (!rk_strcmp(val, "oneframe"))
        {
            workmode = VICAP_WORKMODE_FRAME_ONEFRAME;
        }
        else if (!rk_strcmp(val, "pingpong"))
        {
            workmode = VICAP_WORKMODE_FRAME_PINGPONG;
        }
        else if (!rk_strcmp(val, "block"))
        {
            workmode = VICAP_WORKMODE_BLOCK_PINGPONG;
        }
        else
        {
            VICAP_TEST_INFO("warning:not find out workmode parameter!\n");
        }

        if (workmode < 0)
        {
            VICAP_TEST_INFO("workmode set as: %s failed\n", val);
        }
        else
        {
            VICAP_TEST_INFO("workmode set as: %s\n", val);
        }
    }

    return workmode;
}

static void option_parse_pixelformat(char *option,
                                     struct rk_camera_pix_format *pfmt)
{
    int len;
    char *cur, *cmp, *temp;
    char val[10];

    cur = option;
    if (cur)
    {
        cur += rk_strlen("--set-format=");

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, "fourcc=");
        if (cmp)
        {
            cmp += rk_strlen("fourcc=");
            temp = rk_strstr(cmp, ",");
            if (temp)
            {
                len = temp - cmp;
            }
            else
            {
                len = rk_strlen(cmp);
            }
            rk_strncpy(val, cmp, len);

            if (!rk_strcmp(val, "NV12"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_NV12;
            }
            else if (!rk_strcmp(val, "NV21"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_NV21;
            }
            else if (!rk_strcmp(val, "YUYV"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_YUYV;
            }
            else if (!rk_strcmp(val, "YVYU"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_YVYU;
            }
            else if (!rk_strcmp(val, "UYVY"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_UYVY;
            }
            else if (!rk_strcmp(val, "VYUY"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_VYUY;
            }
            else if (!rk_strcmp(val, "RGGB"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SRGGB8;
            }
            else if (!rk_strcmp(val, "BA81"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SBGGR8;
            }
            else if (!rk_strcmp(val, "GBRG"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SGBRG8;
            }
            else if (!rk_strcmp(val, "GRBG"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SGRBG8;
            }
            else if (!rk_strcmp(val, "BG12"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SBGGR12;
            }
            else if (!rk_strcmp(val, "GB12"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SGBRG12;
            }
            else if (!rk_strcmp(val, "BA12"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SGRBG12;
            }
            else if (!rk_strcmp(val, "RG12"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_SRGGB12;
            }
            else if (!rk_strcmp(val, "GREY"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_GREY;
            }
            else if (!rk_strcmp(val, "Y16"))
            {
                pfmt->pixelformat = V4L2_PIX_FMT_Y16;
            }
            else
            {
                VICAP_TEST_INFO("The set fourcc is:%s not surpported!\n", val);
            }

            if (pfmt->pixelformat)
            {
                VICAP_TEST_INFO("The set fourcc is:%s\n", val);
            }
        }

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, "width=");
        if (cmp)
        {
            cmp += rk_strlen("width=");
            temp = rk_strstr(cmp, ",");
            if (temp)
            {
                len = temp - cmp;
            }
            else
            {
                len = rk_strlen(cmp);
            }

            rk_strncpy(val, cmp, len);
            pfmt->width = (uint32_t)atoi(val);

            VICAP_TEST_INFO("The set width is:%s,val:%d\n", val, pfmt->width);
        }

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, "height=");
        if (cmp)
        {
            cmp += rk_strlen("height=");
            temp = rk_strstr(cmp, ",");
            if (temp)
            {
                len = temp - cmp;
            }
            else
            {
                len = rk_strlen(cmp);
            }

            rk_strncpy(val, cmp, len);
            pfmt->height = (uint32_t)atoi(val);

            VICAP_TEST_INFO("The set height is:%s,val:%d\n", val, pfmt->height);
        }
    }
}

static void option_parse_crop(char *option,
                              struct rk_camera_crop_rect *crop)
{
    int len;
    char *cur, *cmp, *temp;
    char val[10];

    cur = option;
    if (cur)
    {
        cur += rk_strlen("--set-crop=");

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, "x=");
        if (cmp)
        {
            cmp += rk_strlen("x=");
            temp = rk_strstr(cmp, ",");
            if (temp)
            {
                len = temp - cmp;
            }
            else
            {
                len = rk_strlen(cmp);
            }

            rk_strncpy(val, cmp, len);
            crop->x_axis = (uint32_t)atoi(val);

            VICAP_TEST_INFO("The set crop x is:%s,val:%d\n", val, crop->x_axis);
        }

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, "y=");
        if (cmp)
        {
            cmp += rk_strlen("y=");
            temp = rk_strstr(cmp, ",");
            if (temp)
            {
                len = temp - cmp;
            }
            else
            {
                len = rk_strlen(cmp);
            }

            rk_strncpy(val, cmp, len);
            crop->y_axis = (uint32_t)atoi(val);

            VICAP_TEST_INFO("The set crop y is:%s,val:%d\n", val, crop->y_axis);
        }

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, "width=");
        if (cmp)
        {
            cmp += rk_strlen("width=");
            temp = rk_strstr(cmp, ",");
            if (temp)
            {
                len = temp - cmp;
            }
            else
            {
                len = rk_strlen(cmp);
            }

            rk_strncpy(val, cmp, len);
            crop->width = (uint32_t)atoi(val);

            VICAP_TEST_INFO("The set crop width is:%s,val:%d\n", val, crop->width);
        }

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, "height=");
        if (cmp)
        {
            cmp += rk_strlen("height=");
            temp = rk_strstr(cmp, ",");
            if (temp)
            {
                len = temp - cmp;
            }
            else
            {
                len = rk_strlen(cmp);
            }

            rk_strncpy(val, cmp, len);
            crop->height = (uint32_t)atoi(val);

            VICAP_TEST_INFO("The set crop height is:%s,val:%d\n", val, crop->height);
        }

    }
}

static uint8_t option_parse_buf_num(char *option)
{
    int len;
    char *cur, *cmp;
    char val[10];
    uint32_t num = 0;

    cur = option;
    if (cur)
    {
        cur += rk_strlen("--stream-buf=");

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, ",");
        if (cmp)
        {
            len = cmp - cur;
        }
        else
        {
            len = rk_strlen(cur);
        }
        rk_strncpy(val, cur, len);
        num = (uint32_t)atoi(val);
    }

    VICAP_TEST_INFO("The set buf num is:%s,val:%d\n", cur, num);
    return num;
}

static uint8_t option_parse_block_num(char *option)
{
    int len;
    char *cur, *cmp;
    char val[10];
    uint32_t num = 0;

    cur = option;
    if (cur)
    {
        cur += rk_strlen("--set-blocks=");

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, ",");
        if (cmp)
        {
            len = cmp - cur;
        }
        else
        {
            len = rk_strlen(cur);
        }
        rk_strncpy(val, cur, len);
        num = (uint32_t)atoi(val);
    }

    VICAP_TEST_INFO("The set block num is:%s,val:%d\n", cur, num);
    return num;

}

static uint8_t option_parse_frame_num(char *option)
{
    int len;
    char *cur, *cmp;
    char val[10];
    uint32_t num = 0;

    cur = option;
    if (cur)
    {
        cur += rk_strlen("--stream-count=");

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, ",");
        if (cmp)
        {
            len = cmp - cur;
        }
        else
        {
            len = rk_strlen(cur);
        }
        rk_strncpy(val, cur, len);
        num = (uint32_t)atoi(val);
    }

    VICAP_TEST_INFO("The set frame num is:%s,val:%d\n", cur, num);
    return num;
}

static uint8_t option_parse_skip_frame_num(char *option)
{
    int len;
    char *cur, *cmp;
    char val[10];
    uint32_t num = 0;

    cur = option;
    if (cur)
    {
        cur += rk_strlen("--skip-count=");

        rk_memset(val, '\0', 10);
        cmp = rk_strstr(cur, ",");
        if (cmp)
        {
            len = cmp - cur;
        }
        else
        {
            len = rk_strlen(cur);
        }
        rk_strncpy(val, cur, len);
        num = (uint32_t)atoi(val);
    }

    VICAP_TEST_INFO("The skip frame num is:%s,val:%d\n", cur, num);
    return num;

}

static int option_parse_stream_mode(char *option)
{
    char *cmp;
    char val[15];
    int streammode = -1, len = 0;

    cmp = option;
    if (cmp)
    {
        cmp += rk_strlen("--stream-mode=");
        len = rk_strlen(cmp);

        rk_memset(val, '\0', 15);
        rk_strncpy(val, cmp, len);

        if (!rk_strcmp(val, "photo"))
        {
            streammode = STREAM_MODE_PHOTO;
        }
        else if (!rk_strcmp(val, "preview"))
        {
            streammode = STREAM_MODE_PREVIEW;
        }
        else if (!rk_strcmp(val, "power_normal"))
        {
            streammode = STREAM_MODE_PREVIEW;
        }
        else if (!rk_strcmp(val, "power_little"))
        {
            streammode = STREAM_MODE_POWER_LITTLE;
        }
        else if (!rk_strcmp(val, "power_low"))
        {
            streammode = STREAM_MODE_POWER_LOW;
        }
        else
        {
            VICAP_TEST_INFO("warning:not find out stream mode parameter:%s,%s!\n", cmp, val);
        }

        if (streammode > 0)
        {
            VICAP_TEST_INFO("stream mode set as:%s\n", val);
        }
    }

    return streammode;

}

static void option_parse_device_name(char *option, char *dev_name)
{
    char *cmp;
    int len = 0;

    cmp = option;
    if (cmp)
    {
        cmp += rk_strlen("--set-dev=");
        len = rk_strlen(cmp);

        rk_memset(dev_name, '\0', DEVICE_NAME_SIZE);
        rk_strncpy(dev_name, cmp, len);

        VICAP_TEST_INFO("device name set as:%s\n", dev_name);
    }
    else
    {
        VICAP_TEST_INFO("parse device name failed!\n");
    }
}

static void process_parse_option_parameters(char **argv)
{
    int index;
    char *option;

    for (index = 0; argv[index] != RK_NULL; index++)
    {
        option = rk_strstr(argv[index], "--set-workmode=");
        if (option)
        {
            options.workmode = option_parse_workmode(option);
        }

        option = rk_strstr(argv[index], "--set-format=");
        if (option)
        {
            option_parse_pixelformat(option, &options.pixformat);
        }

        option = rk_strstr(argv[index], "--set-crop=");
        if (option)
        {
            option_parse_crop(option, &options.crop);
        }

        option = rk_strstr(argv[index], "--stream-buf=");
        if (option)
        {
            options.buf_num = option_parse_buf_num(option);
        }

        option = rk_strstr(argv[index], "--set-blocks=");
        if (option)
        {
            options.block_num = option_parse_block_num(option);
        }

        option = rk_strstr(argv[index], "--stream-count=");
        if (option)
        {
            options.frame_num = option_parse_frame_num(option);
        }

        option = rk_strstr(argv[index], "--skip-count=");
        if (option)
        {
            options.skip_num = option_parse_skip_frame_num(option);
        }

        option = rk_strstr(argv[index], "--stream-mode=");
        if (option)
        {
            options.streammode = option_parse_stream_mode(option);
        }

        option = rk_strstr(argv[index], "--set-dev=");
        if (option)
        {
            option_parse_device_name(option, options.dev_name);
        }
    }

}

static void process_config_vicap(rk_device *dev)
{
    int count;
    ret_err_t ret;
    uint8_t index;
    struct vicap_video_requestbuffers reqbuf;

    ret = rk_device_open(dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("Err: open vicap device failded!\n");
        return;
    }

    ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_SET_WORKMODE, &options.workmode);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("Err: set vicap work mode failded!\n");
        return;
    }

    ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_SET_FMT, &options.pixformat);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("Err: set vicap format failded!\n");
        return;
    }

    if (options.crop.width != 0 && options.crop.height != 0)
    {
        ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_CROP_IMAGE, &options.crop);
        if (ret != RET_SYS_EOK)
        {
            VICAP_TEST_INFO("Err: set vicap crop failded!\n");
            return;
        }
    }

    ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_SET_BLOCK_NUM, &options.block_num);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("Err: set vicap block num failded!\n");
        return;
    }

    reqbuf.count = options.buf_num;
    reqbuf.work_mode = options.workmode;
    ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_REQBUF, &reqbuf);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("Err: request vicap buf num failded!\n");
        return;
    }

    if (reqbuf.count < options.buf_num)
    {
        VICAP_TEST_INFO("Warning:allocated buf num is:%d,lower than requested num:%d\n",
                        reqbuf.count, options.buf_num);
    }

    for (index = 0; index < reqbuf.count; index++)
    {
        struct vicap_video_buf buf;

        buf.index = index;
        ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_QUERYBUF, &buf);
        if (ret != RET_SYS_EOK)
        {
            VICAP_TEST_INFO("Err: query buf[%d] failded!\n", index);
            return;
        }
    }

    for (index = 0; index < reqbuf.count; index++)
    {
        struct vicap_video_buf buf;

        buf.index = index;
        ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_QBUF, &buf);
        if (ret != RET_SYS_EOK)
        {
            VICAP_TEST_INFO("Err: qbuf buf[%d] failded!\n", index);
            return;
        }
    }

    ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_STREAM_ON, RK_NULL);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("Err: stream on vicap failed!\n");
        return;
    }

    if (options.skip_buf != 0)
    {
        for (count = 1; count <= options.skip_buf; count++)
        {
            struct vicap_video_buf buf;

RETRY1:
            ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_DQBUF, &buf);
            if (ret != RET_SYS_EOK)
            {
                VICAP_TEST_INFO("dqueue buf failed,retry it,%d!\n", count);
                goto RETRY1;
            }
            else
            {
RETRY2:
                ret = rk_device_control(dev, RK_DEVICE_CTRL_VICAP_QBUF, &buf);
                if (ret != RET_SYS_EOK)
                {
                    VICAP_TEST_INFO("queue buf[%d] failed on streaming!\n", buf.index);
                    goto RETRY2;
                }
            }

        }
        VICAP_TEST_INFO("has skipped %d frames\n", options.skip_buf / options.block_num);
    }

}

static void process_sample_image(rk_device *dev, bool is_sample)
{
    if (options.streammode == STREAM_MODE_PHOTO)
    {
        stream_mode_photograph(dev);
    }
    else
    {
        stream_mode_preview(dev, is_sample);
    }
}

static void process_show_help(void)
{
    VICAP_TEST_INFO("************* vicap test help*************\n");
    VICAP_TEST_INFO("dev_find: dev_find <name>, name is the vicap device name in kernel,default is vicap_0 \n\n");
    VICAP_TEST_INFO("dev_set: dev_set <option>, surpported option is as belowed:\n");
    VICAP_TEST_INFO("         --set-dev=<device>:\n");
    VICAP_TEST_INFO("           <device>: the name of device to be opened, the command is the same as dev_find.\n");
    VICAP_TEST_INFO("         --set-workmode=<mode>:\n");
    VICAP_TEST_INFO("           <mode>:can be oneframe,pingpong or block.\n");
    VICAP_TEST_INFO("         --set-format=fourcc=<cc>,width=<width>,height=<height>:\n");
    VICAP_TEST_INFO("           <cc>: supported FOURCC is listed in camera_mediabus.h\n");
    VICAP_TEST_INFO("           <width>, <height>: in range [64:8192].\n");
    VICAP_TEST_INFO("         --set-blocks=<block num>:\n");
    VICAP_TEST_INFO("           <block num>: the num of one frame image is divided in vicap block mode.\n");
    VICAP_TEST_INFO("         --stream-buf=<buf num>:\n");
    VICAP_TEST_INFO("           <buf num>: the num of buf will be requested in driver.\n");
    VICAP_TEST_INFO("         --stream-count=<count num>:\n");
    VICAP_TEST_INFO("           <count num>: the num of image to be captured for application\n");
    VICAP_TEST_INFO("         --stream-mode=<stream mode>:\n");
    VICAP_TEST_INFO("           <stream mode>: can be photo or preview\n");
    VICAP_TEST_INFO("         --skip-count=<skip count>:\n");
    VICAP_TEST_INFO("           <skip count>: the frame num will be skipped by application\n\n");
    VICAP_TEST_INFO("dev_streamon: stream on the device set by dev_find or dev_set --set-dev=<device>\n\n");
}

static struct rk_vicap_dev *process_find_device(char *name)
{
    char *tmp;
    uint8_t object_id;
    rk_device *dev = RK_NULL;
    struct rk_vicap_dev *vicapdev = RK_NULL;

    tmp = rk_strstr(name, "_");
    tmp += 1;
    object_id = atoi(tmp);
    VICAP_TEST_INFO("the device to be found:%s, object_id:%d\n", name, object_id);
    dev = rk_find_device(name);
    if (dev)
    {
        vicapdev = (struct rk_vicap_dev *)dev;
        if (!rk_strcmp(name, vicapdev->name))
        {
            VICAP_TEST_INFO("find the device:%s\n", name);
        }
        else
        {
            VICAP_TEST_INFO("Warning: the device is not matched!\n");
        }
    }
    else
    {
        VICAP_TEST_INFO("the device:%s is not found.\n", name);
    }

#ifdef RT_USING_VICAP_TEST_DISPLAY
    dev_lcd = rk_find_device(DISPLAY_DEVICE_NAME);
    if (!dev_lcd)
    {
        VICAP_TEST_INFO("Warning:display device:%s is not found.\n",
                        DISPLAY_DEVICE_NAME);
    }
    else
    {
        VICAP_TEST_INFO("find the display device:%s\n", dev_lcd->parent.name);
    }
#endif
    return vicapdev;

}

static ret_err_t process_open_device(char *name)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    rk_device *dev = &vicapdev->parent.parent;

    ret = rk_device_open(dev, RT_DEVICE_FLAG_RDWR);
    if (ret != RET_SYS_EOK)
    {
        VICAP_TEST_INFO("Err: open device failed!\n");
    }
    else
    {
        if (!rk_strcmp(name, vicapdev->name))
        {
            VICAP_TEST_INFO("open vicap:%s ,object:%s successfully!\n",
                            vicapdev->name,
                            vicapdev->parent.parent.parent.name);
        }
        else
        {
            VICAP_TEST_INFO("the open device:%s is not matched the exit device:%s\n",
                            name, vicapdev->name);
        }
    }

    return ret;
}

static void vicap_test(int argc, char **argv)
{
    ret_err_t ret;
    static rk_device *dev;
    static bool has_opened = false;
    static bool is_streamon = false;
    uint32_t starttime, endtime, difftime, fps;

    if (argc < 2)
    {
        VICAP_TEST_INFO("Please input test options, you can input vicap_test help for helping!\n");
        return;
    }

    if (!rk_strcmp(argv[1], "help"))
    {
        process_show_help();
    }

    if (!rk_strcmp(argv[1], "dev_find"))
    {
        int len;

        len = rk_strlen(argv[2]);
        rk_memset(options.dev_name, '\0', 12);
        rk_strncpy(options.dev_name, argv[2], len);
    }

    if (!rk_strcmp(argv[1], "dev_set"))
    {
        process_parse_option_parameters(&argv[2]);
    }

    if (!rk_strcmp(argv[1], "dev_streamon"))
    {
        is_streamon = true;
    }

    if (!rk_strcmp(argv[1], "dev_streamoff"))
    {
        is_streamon = false;
    }

    if (is_streamon)
    {
        if (!has_opened)
        {
            vicapdev = process_find_device(options.dev_name);
            if (!vicapdev)
            {
                VICAP_TEST_INFO("find device:%s failed!\n", options.dev_name);
                return;
            }

            stream_pre_operation();
#ifdef RT_USING_VICAP_TEST_DISPLAY
            display_pre_operation(dev_lcd);
#endif
            has_opened = true;
        }

        dev = &vicapdev->parent.parent;

        if (options.streammode == STREAM_MODE_PHOTO)
        {
            process_config_vicap(dev);
            process_sample_image(dev, is_streamon);
            is_streamon = false;
        }
        else if (options.streammode == STREAM_MODE_PREVIEW)
        {
            process_config_vicap(dev);

            do
            {
                /* TODO: is_streamon will controlled by external command */
                starttime = rk_tick_get();
                process_sample_image(dev, is_streamon);
                endtime = rk_tick_get();
                difftime = endtime - starttime;
                fps = (1000 + difftime - 1) / difftime;
                rk_kprintf("fps:%d\n", fps);
            }
            while (is_streamon);
        }
    }

}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(vicap_test, vicap drive test. e.g: vicap_test);
#endif

#endif

#endif
