/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_isp3.c
  * @version V0.0.1
  * @date    6-July-2022
  * @brief   image signal processing (ISP) driver
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup Isp
 *  @{
 */

/** @defgroup Isp_How_To_Use How To Use
 *  @{

 The isp driver is used to sample image data transferred by bt601/bt656.In order
 to let the driver takes effect,there are there tasks need to be completed as belowed:

- (A) Implement the image sensor driver based on camera framework which has been
      implemented in camera.c.
- (B) Assign the image sensor's device name to macro definition "RK_ISP_SUBDEV_NAME"
      which is in drv_isp.h.The isp driver distinguish sensor with this unique value.
- (C) Stream on the isp to capture image, the sequence belowed must be observed:
    - 1) Find out the isp device by device name:
         ispdev = rk_device_find(name);
    - 2) Open the found isp device in step1 to init device:
         ret = rk_device_open(ispdev, RT_DEVICE_OFLAG_RDWR);
    - 3) Set the work mode of isp device after opening device:
         ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_SET_WORKMODE, &workmode);
    - 4) Set the format for outputing:
         ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_SET_FMT, &format);
    - 5) Set the crop information if it is required:
         ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_CROP_IMAGE, &crop);
    - 6) Set the block num if the isp's block mode is required:
         ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_SET_BLOCK_NUM, &num);
    - 7) Set the buf num required by application to capture image:
         ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_REQBUF, &reqbuf);
    - 8) Query the buffers have been allocated in step 7):
         ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_QUERYBUF, &buf);
    - 9) Queue all the buffers have been checked in step 8 into isp drivers:
         ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_QBUF, &buf);
    - 10) Stream on the isp device to capture image:
          ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_STREAM_ON, RT_NULL);
    - 11) Dqueue the buf filled with image data from driver for processing in application:
          ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_DQBUF, &buf);
    - 12) Queue the buf has been processed in application into driver:
          ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_QBUF, &buf);
    - 13) Loop step11 and step 12 untill application stops streaming;
    - 14) Stop streaming:
          ret = rk_device_control(ispdev, RK_DEVICE_CTRL_ISP_STREAM_OFF, RT_NULL);
    - 15) Close device
          ret = rk_device_close(ispdev);

 @} */
#include "drv_isp3.h"
#include "drv_isp3_subdev.h"
#include "../drv_clock.h"

#if defined(RT_USING_ISP3)

#include "drv_isp_module.h"
#include "regs_v3x.h"

struct config_param g_param;
/********************* Private MACRO Definition ******************************/
/** @defgroup ISP_Private_Macro Private Macro
 *  @{
 */

#define ISP_DEBUG             0
#define DEBUG_RKISP_COMPILE_ENABLE 0
#if ISP_DEBUG
#include <stdio.h>
#define ISP_DBG(...)                \
            do { \
                rt_kprintf("[ISP]:");    \
                rt_kprintf(__VA_ARGS__);    \
                }while(0)
#define rk_isp_function_enter()           rt_kprintf("[ISP]:(%s) enter \n",__FUNCTION__)
#define rk_isp_function_exit()            rt_kprintf("[ISP]:(%s) exit \n",__FUNCTION__)
#else
#define ISP_DBG(...)
#define rk_isp_function_enter()
#define rk_isp_function_exit()
#endif

#if ISP_DEBUG_REGISTER
#define ISP_SHOW_REG                      rt_kprintf
#else
#define ISP_SHOW_REG
#endif

#define ISP_INFO(dev,arg...)              rt_kprintf("[%s]:",dev->name);rt_kprintf(arg)

#define ARRAY_SIZE(x)                       (sizeof(x) / sizeof((x)[0]))
#define ENABLE  true
#define DISABLE false

#define ISP_WORKQUEUE_STACK_SIZE                        (512)
#define ISP_WORKQUEUE_PRIORITY                          (12)
#define RT_LARGE_MALLOC_THRRESH                         512

#define min(x, y)                                       (x < y ? x : y)
#define max(x, y)                                       (x < y ? y : x)

#define roundup(x, y)                                   ({\
                                                            (((x) + (y - 1)) / y) * y;\
                                                         })

#define clamp(val,min,max)                              ({ \
                                                            typeof(val) __val = (val); \
                                                            typeof(min) __min = (min); \
                                                            typeof(max) __max = (max); \
                                                            (void) (&__val == &__min); \
                                                            (void) (&__val == &__max); \
                                                            __val = __val < __min ? __min: __val; \
                                                            __val > __max ? __max: __val; })

/* 0x3000 * CAM_NUM */
#define SHARED_MEM_RESERVED_HEAD_SIZE                   0x6000

RT_WEAK struct dphy_board_desc csi2_dphy_board = {0};
RT_WEAK struct csi2_board_desc csi2_host1_board = {0};
RT_WEAK struct csi2_board_desc csi2_host0_board = {0};
RT_WEAK struct vicap_board_desc vicap_board = {0};

/* ISP_Private_Macro */

struct rk_isp_dev isp_instance;

/**
 * cache the output parameters of isp to match input image format.
 */
static const struct isp_output_fmt g_output_fmts[] =
{
    {
        .fourcc = V4L2_PIX_FMT_NV16,
        .cplanes = 2,
        .mplanes = 1,
        .format = ISP_OUTPUT_YUV_422 | ISP_UV_STORED_ORDER_UVUV,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
        .uv_swap = 0,
        .write_fmt_mp = MI_CTRL_MP_WRITE_YUV_SPLA,
    }, {
        .fourcc = V4L2_PIX_FMT_NV61,
        .format = ISP_OUTPUT_YUV_422 | ISP_UV_STORED_ORDER_VUVU,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
        .uv_swap = 1,
        .write_fmt_mp = MI_CTRL_MP_WRITE_YUV_SPLA,
    }, {
        .fourcc = V4L2_PIX_FMT_NV12,
        .format = ISP_OUTPUT_YUV_420 | ISP_UV_STORED_ORDER_UVUV,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
    }, {
        .fourcc = V4L2_PIX_FMT_NV21,
        .format = ISP_OUTPUT_YUV_420 | ISP_UV_STORED_ORDER_VUVU,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
    }, {
        .fourcc = V4L2_PIX_FMT_YUYV,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
    }, {
        .fourcc = V4L2_PIX_FMT_YVYU,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
    }, {
        .fourcc = V4L2_PIX_FMT_UYVY,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
    }, {
        .fourcc = V4L2_PIX_FMT_VYUY,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
    }, {
        .fourcc = V4L2_PIX_FMT_RGB24,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 24 },
        .fmt_type = ISP_DEVICE_FORMAT_RGB,
    }, {
        .fourcc = V4L2_PIX_FMT_RGB565,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RGB,
    }, {
        .fourcc = V4L2_PIX_FMT_BGR666,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 18 },
        .fmt_type = ISP_DEVICE_FORMAT_RGB,
    }, {
        .fourcc = V4L2_PIX_FMT_SRGGB8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_YUV_PLA_OR_RAW8,
        .format_data_width = 8,
    }, {
        .fourcc = V4L2_PIX_FMT_SGRBG8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .format_data_width = 8,
    }, {
        .fourcc = V4L2_PIX_FMT_SGBRG8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .format_data_width = 8,
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .format_data_width = 8,
    }, {
        .fourcc = V4L2_PIX_FMT_SRGGB10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 10,
    }, {
        .fourcc = V4L2_PIX_FMT_SGRBG10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 10,
    }, {
        .fourcc = V4L2_PIX_FMT_SGBRG10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 10,
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 10,
    }, {
        .fourcc = V4L2_PIX_FMT_SRGGB12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 12,
    }, {
        .fourcc = V4L2_PIX_FMT_SGRBG12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 12,
    }, {
        .fourcc = V4L2_PIX_FMT_SGBRG12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 12,
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .write_fmt_mp = MI_CTRL_MP_WRITE_RAW12,
        .format_data_width = 12,
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR16,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_RAW,
        .format_data_width = 16,
    }, {
        .fourcc = V4L2_PIX_FMT_Y16,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
        .format_data_width = 16,
    }, {
        .fourcc = V4L2_PIX_FMT_GREY,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
        .format_data_width = 8,
    }
};

/**
 * cache the input parameters of isp to match input image format.
 */
static const struct isp_input_fmt g_input_fmts[] =
{
    {
        .mbus_code = MEDIA_BUS_FMT_YUYV8_2X8,
        .data.yuv_input_order = ISP_YUV_IN_ORDER_YUYV,
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
        .mipi_dt = MIPI_CSI2_DT_YUV422_8BITS,
        .data_width = 8,
    },
    {
        .mbus_code = MEDIA_BUS_FMT_YVYU8_2X8,
        .data.yuv_input_order = ISP_YUV_IN_ORDER_YVYU,
        .fmt_type = ISP_DEVICE_FORMAT_YUV,
        .mipi_dt = MIPI_CSI2_DT_YUV422_8BITS,
        .data_width = 8,
    },
    {
        .mbus_code = MEDIA_BUS_FMT_UYVY8_2X8,
        .data.yuv_input_order = ISP_YUV_IN_ORDER_UYVY,
        .fmt_type   = ISP_DEVICE_FORMAT_YUV,
        .mipi_dt = MIPI_CSI2_DT_YUV422_8BITS,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_VYUY8_2X8,
        .data.yuv_input_order = ISP_YUV_IN_ORDER_VYUY,
        .fmt_type   = ISP_DEVICE_FORMAT_YUV,
        .mipi_dt = MIPI_CSI2_DT_YUV422_8BITS,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SBGGR8_1X8,
        .data.raw_input_width = ISP_RAW_WIDTH_8BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW8,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGBRG8_1X8,
        .data.raw_input_width = ISP_RAW_WIDTH_8BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW8,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGRBG8_1X8,
        .data.raw_input_width = ISP_RAW_WIDTH_8BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW8,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SRGGB8_1X8,
        .data.raw_input_width = ISP_RAW_WIDTH_8BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW8,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SBGGR10_1X10,
        .data.raw_input_width = ISP_RAW_WIDTH_10BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW10,
        .data_width = 10,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGBRG10_1X10,
        .data.raw_input_width = ISP_RAW_WIDTH_10BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW10,
        .data_width = 10,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGRBG10_1X10,
        .data.raw_input_width = ISP_RAW_WIDTH_10BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW10,
        .data_width = 10,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SRGGB10_1X10,
        .data.raw_input_width = ISP_RAW_WIDTH_10BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW10,
        .data_width = 10,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SBGGR12_1X12,
        .data.raw_input_width = ISP_RAW_WIDTH_12BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW12,
        .data_width = 12,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGBRG12_1X12,
        .data.raw_input_width = ISP_RAW_WIDTH_12BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW12,
        .data_width = 12,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGRBG12_1X12,
        .data.raw_input_width = ISP_RAW_WIDTH_12BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW12,
        .data_width = 12,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SRGGB12_1X12,
        .data.raw_input_width = ISP_RAW_WIDTH_12BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW12,
        .data_width = 12,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_RGB888_1X24,
        .mipi_dt = MIPI_CSI2_DT_RGB888,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_Y8_1X8,
        .data.raw_input_width = ISP_RAW_WIDTH_8BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW8,
        .data_width = 8,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_Y10_1X10,
        .data.raw_input_width = ISP_RAW_WIDTH_10BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW10,
        .data_width = 10,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_Y12_1X12,
        .data.raw_input_width = ISP_RAW_WIDTH_12BIT,
        .fmt_type   = ISP_DEVICE_FORMAT_RAW,
        .mipi_dt = MIPI_CSI2_DT_RAW12,
        .data_width = 12,
    }
};

static uint32_t bytesperline;

/********************* Private Function Definition ***************************/
/*
 * ISP_Public_Functions Public Functions
 */
static void rk_isp_init_heap(struct rk_isp_dev *dev,
                             uint32_t start, uint32_t size)
{
    struct heap_pool *heap = &dev->heap;

    heap->start_addr = start;
    heap->end_addr = start + size - 1;
    heap->total_size = size;
    heap->available_size = heap->total_size;
    heap->cur_addr = heap->start_addr;
}

static uint32_t rk_isp_heap_malloc(struct rk_isp_dev *dev, uint32_t size,
                                   uint32_t align)
{
    uint32_t mem;

    struct heap_pool *heap = &dev->heap;
    uint32_t mod, useless;

    mod = heap->cur_addr % align;
    if (mod != 0)
    {
        useless = align - mod;
        heap->cur_addr += useless;
        heap->wasteful_size += useless;
    }

    mem = heap->cur_addr;
    heap->cur_addr += (size - 1);
    heap->available_size = heap->end_addr - heap->cur_addr + 1;
    ISP_INFO(dev, "buf head[%x] heap->end_addr %x\n", mem, heap->end_addr);

    return mem;
}

/**
 * @brief  Get xsubs and ysubs based on fourcc formats to calc image size.
 * @param fcc: fourcc code.
 * @param xsubs: horizontal color samples in a 4*4 matrix, for yuv.
 * @param ysubs: vertical color samples in a 4*4 matrix, for yuv.
 */
int fcc_xysubs(uint32_t fcc, uint32_t *xsubs, uint32_t *ysubs)
{
    switch (fcc)
    {
    case V4L2_PIX_FMT_GREY:
    case V4L2_PIX_FMT_YUV444M:
        *xsubs = 1;
        *ysubs = 1;
        break;
    case V4L2_PIX_FMT_YUYV:
    case V4L2_PIX_FMT_YVYU:
    case V4L2_PIX_FMT_VYUY:
    case V4L2_PIX_FMT_YUV422P:
    case V4L2_PIX_FMT_NV16:
    case V4L2_PIX_FMT_NV61:
    case V4L2_PIX_FMT_YVU422M:
        *xsubs = 2;
        *ysubs = 1;
        break;
    case V4L2_PIX_FMT_NV21:
    case V4L2_PIX_FMT_NV12:
    case V4L2_PIX_FMT_NV21M:
    case V4L2_PIX_FMT_NV12M:
    case V4L2_PIX_FMT_YUV420:
    case V4L2_PIX_FMT_YVU420:
        *xsubs = 2;
        *ysubs = 2;
        break;
    default:
        return -RT_EINVAL;
    }

    return 0;
}

/**
 * @brief  Judge whether set Y address only or not based on output format.
 * @param fmt : The output format set by application.
 * @return : When the format type is raw or grey,it will return true.
 */
static bool rk_isp_judge_set_YAddress_only(const struct isp_output_fmt *fmt)
{
    bool yes;

    switch (fmt->fourcc)
    {
    case V4L2_PIX_FMT_SRGGB8:
    case V4L2_PIX_FMT_SGRBG8:
    case V4L2_PIX_FMT_SGBRG8:
    case V4L2_PIX_FMT_SBGGR8:
    case V4L2_PIX_FMT_Y16:
    case V4L2_PIX_FMT_GREY:
    case V4L2_PIX_FMT_RGB24:
    case V4L2_PIX_FMT_RGB565:
    case V4L2_PIX_FMT_BGR666:
    case V4L2_PIX_FMT_SRGGB10:
    case V4L2_PIX_FMT_SGRBG10:
    case V4L2_PIX_FMT_SGBRG10:
    case V4L2_PIX_FMT_SBGGR10:
    case V4L2_PIX_FMT_SRGGB12:
    case V4L2_PIX_FMT_SGRBG12:
    case V4L2_PIX_FMT_SGBRG12:
    case V4L2_PIX_FMT_SBGGR12:
    case V4L2_PIX_FMT_SBGGR16:
        yes = true;
        break;
    default:
        yes = false;
        break;
    }

    return yes;
}

/**
 * @brief  In raw or jpeg mode, data is stored by 16-bits,so need to align it.
 * @param fmt : The output format parameters.
 * @param plane_index : The plane index in one frame format.
 */
static uint32_t rk_isp_align_bits_per_pixel(const struct isp_output_fmt *fmt,
        int plane_index)
{
    uint32_t bpp = 0;

    if (fmt)
    {
        switch (fmt->fourcc)
        {
        case V4L2_PIX_FMT_NV16:
        case V4L2_PIX_FMT_NV61:
        case V4L2_PIX_FMT_NV12:
        case V4L2_PIX_FMT_NV21:
        case V4L2_PIX_FMT_YUYV:
        case V4L2_PIX_FMT_YVYU:
        case V4L2_PIX_FMT_UYVY:
        case V4L2_PIX_FMT_VYUY:
        case V4L2_PIX_FMT_Y16:
        case V4L2_PIX_FMT_GREY:
        case V4L2_PIX_FMT_SRGGB8:
        case V4L2_PIX_FMT_SGRBG8:
        case V4L2_PIX_FMT_SGBRG8:
        case V4L2_PIX_FMT_SBGGR8:
            bpp = fmt->bpp[plane_index];
            break;
        case V4L2_PIX_FMT_RGB24:
        case V4L2_PIX_FMT_RGB565:
        case V4L2_PIX_FMT_BGR666:
        case V4L2_PIX_FMT_SRGGB10:
        case V4L2_PIX_FMT_SGRBG10:
        case V4L2_PIX_FMT_SGBRG10:
        case V4L2_PIX_FMT_SBGGR10:
        case V4L2_PIX_FMT_SRGGB12:
        case V4L2_PIX_FMT_SGRBG12:
        case V4L2_PIX_FMT_SGBRG12:
        case V4L2_PIX_FMT_SBGGR12:
        case V4L2_PIX_FMT_SBGGR16:
            bpp = max(fmt->bpp[plane_index], ISP_RAW_STORED_BIT_WIDTH);
            bpp = roundup(bpp, ISP_RAW_STORED_BIT_WIDTH);
            break;
        default:
            rt_kprintf("rk_isp:fourcc: %d is not supported!\n", fmt->fourcc);
            break;
        }
    }

    return bpp;
}

/**
 * @brief  Enqueue a isp_video_buf in driver for processing
 * @param buf : The buf is to enqueued into isp driver.
 */
static void __enqueue_in_driver(struct isp_video_buf *buf)
{
    struct isp_videobuf_queue *queue = buf->buf_q;
    rt_base_t level;

    level = rt_hw_interrupt_disable();
    buf->state = ISP_BUF_STATE_ACTIVE;
    queue->owned_by_drv_count++;
    rt_list_insert_before(&queue->actived_list, &buf->actived_entry);
    rt_hw_interrupt_enable(level);
}

/**
 * @brief  get isp device instance.
 * @param dev : The isp abstract device object in isp layer.
 */
static inline struct rk_isp_dev *rk_get_isp_dev(struct rk_isp_device *dev)
{
    struct rk_isp_dev *ispdev = dev->parent.user_data;

    return ispdev;
}

/**
 * @brief  change the buf state for processing.
 * @param buf : The buf in videobuf queue to be changed state.
 * @param state : The state to be changed as.
 */
static void rk_isp_videobuf_done(struct isp_video_buf *buf,
                                 eISP_Bufstat state,
                                 eISP_Listtype list_type)
{
    rt_base_t level;

    struct isp_videobuf_queue *queue = buf->buf_q;
    struct rk_isp_dev *ispdev = rt_container_of(queue, struct rk_isp_dev, buf_queue);

    RT_ASSERT(buf != RT_NULL);

    if (buf->state != ISP_BUF_STATE_ACTIVE)
    {
        ISP_INFO(ispdev, "passed buf[%d] state is:0x%x not in ISP_BUF_STATE_ACTIVE\n",
                 buf->index, buf->state);
        return;
    }

    if (state != ISP_BUF_STATE_DONE &&
            state != ISP_BUF_STATE_ERROR &&
            state != ISP_BUF_STATE_QUEUED &&
            state != ISP_BUF_STATE_REQUEUEING
       )
        state = ISP_BUF_STATE_ERROR;

    if (state == ISP_BUF_STATE_QUEUED ||
            state == ISP_BUF_STATE_REQUEUEING)
    {
        buf->state = ISP_BUF_STATE_QUEUED;
    }
    else
    {
        level = rt_hw_interrupt_disable();

        if (list_type == ISP_BUF_LIST_TYPE_MP)
        {
            rt_list_insert_before(&queue->done_list, &buf->done_entry);
            buf->state = state;
            buf->frame_id = ispdev->frame_id;
            buf->listtype = ISP_BUF_LIST_TYPE_MP;
            queue->owned_by_drv_count--;
        }
        else if (list_type == ISP_BUF_LIST_TYPE_LF)
        {
            rt_list_insert_before(&queue->lf_done_list, &buf->done_entry);
            buf->state = state;
            buf->listtype = ISP_BUF_LIST_TYPE_LF;
            queue->owned_by_drv_count--;
        }
        else if (list_type == ISP_BUF_LIST_TYPE_MF)
        {
            rt_list_insert_before(&queue->mf_done_list, &buf->done_entry);
            buf->state = state;
            buf->listtype = ISP_BUF_LIST_TYPE_MF;
            queue->owned_by_drv_count--;
        }
        else if (list_type == ISP_BUF_LIST_TYPE_SF)
        {
            rt_list_insert_before(&queue->sf_done_list, &buf->done_entry);
            buf->state = state;
            buf->listtype = ISP_BUF_LIST_TYPE_SF;
            queue->owned_by_drv_count--;
        }

        rt_hw_interrupt_enable(level);
        rt_sem_release(queue->qsem);
    }

    switch (state)
    {
    case ISP_BUF_STATE_REQUEUEING:
        if (queue->streaming)
        {
            __enqueue_in_driver(buf);
        }
        break;

    default:
        /* TODO:Inform any processes that may be waiting for buffers */
        break;
    }
}

/**
 * @brief  find out the usable buf in actived list of buf queue.
 * @param dev : The isp device.
 */
static struct isp_video_buf *rk_isp_find_valid_buf(struct rk_isp_dev *dev)
{
    rt_base_t level;
    struct isp_video_buf *buf;
    struct isp_videobuf_queue *queue;

    RT_ASSERT(dev != RT_NULL);

    queue = &dev->buf_queue;
    if (queue->queued_count < 1)
    {
        ISP_INFO(dev, "Warning: there is no buf queued in video buf queue!\n");
        return RT_NULL;
    }

    if (queue->owned_by_drv_count < 1)
    {
        ISP_INFO(dev, "Warning: there is no buf enqueued in driver,drv_count:%d!\n",
                 queue->owned_by_drv_count);
        return RT_NULL;
    }

    level = rt_hw_interrupt_disable();
    if (!rt_list_isempty(&queue->actived_list))
    {
        buf = rt_list_first_entry(&queue->actived_list,
                                  struct isp_video_buf,
                                  actived_entry);
        rt_list_remove(&buf->actived_entry);
    }
    else
    {
        rt_hw_interrupt_enable(level);
        return RT_NULL;
    }
    rt_hw_interrupt_enable(level);

    return buf;
}

/**
 * @brief  Initialize the subdev of isp, commontly it is image sensor.
 * @param dev : The isp device.
 * @attention : Actually it is completed in init callback of sensor's driver.
 */
static rt_err_t rk_isp_init_subdev(struct rk_isp_dev *dev)
{
    rt_err_t ret;
    struct rt_device *device;
    struct rk_camera_device *camera;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->subdev != RT_NULL);

    camera = dev->subdev;
    device = &camera->parent;
    ret = rt_device_init(device);
    if (ret != RT_EOK)
    {
        ISP_INFO(dev, "init subdev failed!\n");
    }

    rk_isp_function_exit();

    return ret;
}

/**
 * @brief  Get the instance of subdev by its device name.
 * @param dev : The isp device.
 */
static struct rk_camera_device *rk_isp_ctrl_get_subdev(struct rk_isp_dev *dev)
{
    struct rt_device *subdev;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);

    subdev = rt_device_find(RK_ISP_MAIN_SUBDEV_NAME);
    if (subdev == RT_NULL)
    {
        ISP_INFO(dev, "Err: Can't find isp subdev:%s\n", RK_ISP_MAIN_SUBDEV_NAME);
        return RT_NULL;
    }

    rk_isp_function_exit();

    return (struct rk_camera_device *)subdev;
}

/**
 * @brief  Set the work mode of isp.
 * @param dev : The isp device.
 * @param mode : The work mode to be set as.
 * @attention : The application must call this function before setting format.
 */
static rt_err_t rk_isp_ctrl_set_workmode(struct rk_isp_dev *dev, eISP_workMode mode)
{
    rk_isp_function_enter();

    if (mode != ISP_WORKMODE_FRAME_ONEFRAME &&
            mode != ISP_WORKMODE_FRAME_PINGPONG &&
            mode != ISP_WORKMODE_BLOCK_PINGPONG)
    {
        ISP_INFO(dev, "Err: the mode parameter:0x%x is invalid !!!\n", mode);
        return -RT_EINVAL;
    }
    dev->work_mode = mode;

    rk_isp_function_exit();

    return RT_EOK;
}

/**
 * @brief  Set the output format required by the application.
 * @param dev : The isp device.
 * @param format : The format parameter set by the application.
 */
static rt_err_t rk_isp_ctrl_set_format(struct rk_isp_dev *dev,
                                       struct rk_camera_pix_format *format)
{
    uint8_t i;
    struct isp_output_info *output_info;
    struct rk_camera_pix_format *output_pix_format;

    rk_isp_function_enter();
    output_info = &dev->output;
    output_pix_format = &output_info->pix_format;
    rt_memcpy(output_pix_format, format, sizeof(struct rk_camera_pix_format));

    for (i = 0; i < ARRAY_SIZE(g_output_fmts); i++)
    {
        if (output_pix_format->pixelformat == g_output_fmts[i].fourcc)
        {
            output_info->output_fmt = &g_output_fmts[i];
            break;
        }
    }

    if (i == ARRAY_SIZE(g_output_fmts))
    {
        ISP_INFO(dev, "Err: the set format:0x%x is not supported!\n",
                 output_pix_format->pixelformat);
        return -RT_EEMPTY;
    }

    rk_isp_function_exit();

    return RT_EOK;
}

/**
 * @brief  Set crop window requied by the application.
 * @param dev : The isp device.
 * @param rect : The crop information set by application.
 */
static rt_err_t rk_isp_ctrl_set_crop_window(struct rk_isp_dev *dev,
        struct rk_camera_crop_rect *rect)
{
    uint8_t ret = RT_EOK;
    int32_t d_val;
    struct isp_output_info *output_info;
    struct rk_camera_crop_rect *output_crop;
    struct rk_camera_mbus_framefmt *input_format;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(rect != RT_NULL);

    output_info = &dev->output;
    output_crop = &output_info->crop;
    input_format = &dev->input.cam_info.mbus_fmt;

    d_val = (int32_t)input_format->width - (int32_t)rect->x_axis;
    if (d_val < (int32_t)rect->width)
    {
        ret = -RT_EINVAL;
        ISP_INFO(dev, "Err: width plus x_axis must not be greater than input width!\n");
        goto ERR;
    }

    d_val = (int32_t)input_format->height - (int32_t)rect->y_axis;
    if (d_val < (int32_t)rect->height)
    {
        ret = -RT_EINVAL;
        ISP_INFO(dev, "Err: height plus y_axis must not be greater than input height!\n");
        goto ERR;
    }

    output_crop->x_axis = rect->x_axis;
    output_crop->y_axis = rect->y_axis;
    output_crop->width = rect->width;
    output_crop->height = rect->height;
    output_info->is_crop = true;

ERR:
    rk_isp_function_exit();

    return ret;
}

/**
 * @brief enable/disable isp relevant clk
 * @param dev : The isp device.
 * @param enable : The enable status of isp clk
 */
static void rk_isp_hw_set_clk(struct rk_isp_dev *dev, bool enable)
{
    struct isp_clock_info *isp_clk;

    RT_ASSERT(dev != RT_NULL);

    isp_clk = &dev->isp_clk;
    if (enable)
    {
        if (isp_clk->isp_aclk)
        {
            rk_clk_enable1(isp_clk->isp_aclk);
        }

        if (isp_clk->isp_hclk)
        {
            rk_clk_enable1(isp_clk->isp_hclk);
        }

        if (isp_clk->isp_pclk)
        {
            rk_clk_enable1(isp_clk->isp_pclk);
        }
        if (isp_clk->isp0_vicap_clk)
        {
            rk_clk_enable1(isp_clk->isp0_vicap_clk);
        }
    }
    else
    {
        if (isp_clk->isp_aclk)
        {
            rk_clk_disable1(isp_clk->isp_aclk);
        }

        if (isp_clk->isp_hclk)
        {
            rk_clk_disable1(isp_clk->isp_hclk);
        }

        if (isp_clk->isp_pclk)
        {
            rk_clk_disable1(isp_clk->isp_pclk);
        }
        if (isp_clk->isp0_vicap_clk)
        {
            rk_clk_disable1(isp_clk->isp0_vicap_clk);
        }
    }
}

static void rk_isp_hw_set_vicap_clk(struct rk_isp_dev *dev, bool enable)
{
    struct vicap_clock_info *vicap_clk;

    RT_ASSERT(dev != RT_NULL);

//clk_dump();
    ISP_INFO(dev, "rk_isp_hw_set_vicap_clk enable is %d\n", enable);
    vicap_clk = &dev->vicap_clk;
    if (enable)
    {
        if (vicap_clk->vicap_pclk)
        {
            rk_clk_enable1(vicap_clk->vicap_pclk);
        }

        if (vicap_clk->vicap_hclk)
        {
            rk_clk_enable1(vicap_clk->vicap_hclk);
        }

        if (vicap_clk->vicap_aclk)
        {
            rk_clk_enable1(vicap_clk->vicap_aclk);
        }

        if (vicap_clk->vicap_dclk)
        {
            rk_clk_enable1(vicap_clk->vicap_dclk);
        }

        if (vicap_clk->vicap_i0clk)
        {
            rk_clk_enable1(vicap_clk->vicap_i0clk);
        }

        if (vicap_clk->vicap_rx0pclk)
        {
            rk_clk_enable1(vicap_clk->vicap_rx0pclk);
        }
        if (vicap_clk->vicap_pclk_vepu)
        {
            rk_clk_enable1(vicap_clk->vicap_pclk_vepu);
        }
    }
    else
    {
        if (vicap_clk->vicap_pclk)
        {
            rk_clk_disable1(vicap_clk->vicap_pclk);
        }

        if (vicap_clk->vicap_hclk)
        {
            rk_clk_disable1(vicap_clk->vicap_hclk);
        }

        if (vicap_clk->vicap_aclk)
        {
            rk_clk_disable1(vicap_clk->vicap_aclk);
        }

        if (vicap_clk->vicap_dclk)
        {
            rk_clk_disable1(vicap_clk->vicap_dclk);
        }

        if (vicap_clk->vicap_i0clk)
        {
            rk_clk_disable1(vicap_clk->vicap_i0clk);
        }

        if (vicap_clk->vicap_rx0pclk)
        {
            rk_clk_disable1(vicap_clk->vicap_rx0pclk);
        }
        if (vicap_clk->vicap_pclk_vepu)
        {
            rk_clk_disable1(vicap_clk->vicap_pclk_vepu);
        }
    }
}

static void rk_isp_hw_set_csi2_clk(struct rk_isp_dev *dev, bool enable)
{
    struct csi2_clock_info *csi2_clk;

    RT_ASSERT(dev != RT_NULL);

    csi2_clk = &dev->csi2_clk;
    if (enable)
    {
        if (csi2_clk->csi2_pclk)
        {
            rk_clk_enable1(csi2_clk->csi2_pclk);
        }
        if (csi2_clk->csi2_rxbyteclkhs0)
        {
            rk_clk_enable1(csi2_clk->csi2_rxbyteclkhs0);
        }
    }
    else
    {
        if (csi2_clk->csi2_pclk)
        {
            rk_clk_disable1(csi2_clk->csi2_pclk);
        }
        if (csi2_clk->csi2_rxbyteclkhs0)
        {
            rk_clk_disable1(csi2_clk->csi2_rxbyteclkhs0);
        }
    }
}

static void rk_isp_hw_set_dphy_clk(struct rk_isp_dev *dev, bool enable)
{
    struct dphy_clock_info *dphy_clk;

    RT_ASSERT(dev != RT_NULL);

    dphy_clk = &dev->dphy_clk;
    if (enable)
    {
        if (dphy_clk->dphy_pclk)
        {
            rk_clk_enable1(dphy_clk->dphy_pclk);
        }
    }
    else
    {
        if (dphy_clk->dphy_pclk)
        {
            rk_clk_disable1(dphy_clk->dphy_pclk);
        }
    }
}

/**
 * @brief  Init the yuv address of mp path.
 * @param dev : The isp device.
 */
static rt_err_t rk_isp_hw_init_mp_yuv_addr(struct rk_isp_dev *dev)
{
    struct isp_video_buf *buf;
    struct isp_output_info *output_info;
    struct isp_plane_info *plane_info;
    const struct isp_output_fmt *fmt;
    rt_err_t ret = RT_EOK;
    uint32_t y_addr, cb_addr, cr_addr, width, height;

    rk_isp_function_enter();
    output_info = &dev->output;
    plane_info = &dev->plane_info;
    fmt = output_info->output_fmt;

    if (output_info->is_crop)
    {
        width = output_info->crop.width;
        height = output_info->crop.height;
    }
    else
    {
        width = output_info->pix_format.width;
        height = output_info->pix_format.height;
    }

    /* in frame oneframe mode, the address switch is also pingpong,
     * so the frame1 address also needs to be set.
     */
    buf = RT_NULL;
    if (!dev->buf_addr0)
    {
        buf = rk_isp_find_valid_buf(dev);
        if (buf)
        {
            y_addr = (uint32_t)(buf->addr);
            if (!rk_isp_judge_set_YAddress_only(fmt))
            {
                cb_addr = y_addr + plane_info->plane_size[ISP_COLOR_Y];
                cr_addr = cb_addr + plane_info->plane_size[ISP_COLOR_CBCR] / 2;
            }
            else
            {
                cb_addr = y_addr;
                cr_addr = y_addr;
            }

            /* set default isp mp output format is NV12 */
            RKISP_WRITE(ISP3X_MI_MP_WR_Y_BASE, y_addr);
            RKISP_WRITE(ISP3X_MI_MP_WR_Y_SIZE, width * height);
            RKISP_WRITE(ISP3X_MI_MP_WR_Y_OFFS_CNT, 0x0);
            RKISP_WRITE(ISP3X_MI_MP_WR_Y_OFFS_CNT_START, 0x0);
            RKISP_WRITE(ISP3X_MI_MP_WR_Y_IRQ_OFFS, 0x0);

            RKISP_WRITE(ISP3X_MI_MP_WR_CB_BASE, cb_addr);
            RKISP_WRITE(ISP3X_MI_MP_WR_CB_SIZE, width * height / 2);
            RKISP_WRITE(ISP3X_MI_MP_WR_CB_OFFS_CNT, 0x0);
            RKISP_WRITE(ISP3X_MI_MP_WR_CB_OFFS_CNT_START, 0x0);

            RKISP_WRITE(ISP3X_MI_MP_WR_CR_BASE, cr_addr);
            RKISP_WRITE(ISP3X_MI_MP_WR_CR_SIZE, width * height / 2);
            RKISP_WRITE(ISP3X_MI_MP_WR_CR_OFFS_CNT, 0x0);
            RKISP_WRITE(ISP3X_MI_MP_WR_CR_OFFS_CNT_START, 0x0);

            dev->buf_addr0 = buf;
            buf = RT_NULL;
        }
        else
        {
            ISP_INFO(dev, "Err: thers is no buf to init frame addr!\n");
            ret = RT_EEMPTY;
        }
    }

    rk_isp_function_exit();

    return ret;
}

/**
 * @brief  Update mp path yuv address.
 * @param dev : The isp device.
 */
static struct isp_video_buf *rk_isp_hw_update_mp_yuv_addr(struct rk_isp_dev *dev, uint32_t frmstat)
{
    struct isp_plane_info *plane_info;
    uint32_t y_addr, cb_addr, cr_addr ;
    struct isp_video_buf *cur_buf = RT_NULL, *next_buf = RK_NULL;

    plane_info = &dev->plane_info;

    if (frmstat & ISP3X_MI_MP_FRAME)
    {
        dev->frm_id++;
    }

    if (dev->buf_addr0)
    {
        cur_buf = dev->buf_addr0;
        if (cur_buf->state == ISP_BUF_STATE_ACTIVE)
        {
            next_buf = rk_isp_find_valid_buf(dev);
            if (next_buf)
            {
                y_addr = (uint32_t)(next_buf->addr);
                cb_addr = y_addr + plane_info->plane_size[ISP_COLOR_Y];
                cr_addr = cb_addr + plane_info->plane_size[ISP_COLOR_CBCR] / 2;

                RKISP_WRITE(ISP3X_MI_MP_WR_Y_BASE, y_addr);
                RKISP_WRITE(ISP3X_MI_MP_WR_CB_BASE, cb_addr);
                RKISP_WRITE(ISP3X_MI_MP_WR_CR_BASE, cr_addr);
                dev->buf_addr0 = next_buf;
            }
            else
            {
                cur_buf = RK_NULL;
                rt_kprintf("no buf for updating!\n");
            }
        }
    }
    else
    {
        rt_kprintf("update buf failed!\n");
    }

    return cur_buf;

}

/**
 * @brief  Set the parameter of axiburst type
 * @param dev : The isp device.
 */
static void rk_isp_hw_set_isp_top(struct rk_isp_dev *dev, bool enable)
{
    struct isp_input_info *input_info;
    struct rk_camera_info *cam_info;
    struct isp_input_fmt *input_fmt;
    const struct isp_output_fmt *output_fmt;
    struct rk_camera_mbus_framefmt *mbus_fmt;
    struct rk_camera_mbus_config *mbus_config;
    uint32_t isp_ctrl = 0x0, acq_mult = 1, acq_prop = 0x0, irq_mask, flags;

    rk_isp_function_enter();
    input_info = &dev->input;
    input_fmt = &input_info->input_fmt;
    output_fmt = dev->output.output_fmt;
    cam_info = &input_info->cam_info;
    mbus_fmt = &cam_info->mbus_fmt;
    mbus_config = &cam_info->mbus_config;

    if (enable)
    {
        if (input_fmt->fmt_type == ISP_DEVICE_FORMAT_YUV)
        {
            acq_mult = 2;
            if (mbus_config->mbus_type == CAMERA_MBUS_CSI2_DPHY ||
                    mbus_config->mbus_type == CAMERA_MBUS_PARALLEL)
            {
                isp_ctrl |= ISP_TOP_CTRL_ISP_MODE_BT601_YUV_H_V_SYNC;
            }
            else if (mbus_config->mbus_type == CAMERA_MBUS_BT656)
            {
                isp_ctrl |= ISP_TOP_CTRL_ISP_MODE_BT656_YUV_EMBEDDED_SYNC;
            }
        }
        else if (input_fmt->fmt_type == ISP_DEVICE_FORMAT_RAW)
        {
            acq_mult = 1;
            if (output_fmt->fmt_type == ISP_DEVICE_FORMAT_RAW)
            {
                if (mbus_config->mbus_type == CAMERA_MBUS_BT656)
                {
                    isp_ctrl |= ISP_TOP_CTRL_ISP_MODE_RAW_BT656;
                }
                else
                {
                    isp_ctrl |= ISP_TOP_CTRL_ISP_MODE_RAW_BT601;
                }
            }
            else
            {
                if (mbus_config->mbus_type == CAMERA_MBUS_BT656)
                {
                    isp_ctrl |= ISP_TOP_CTRL_ISP_MODE_BAYER_RGB_BT656;
                }
                else
                {
                    isp_ctrl |= ISP_TOP_CTRL_ISP_MODE_BAYER_RGB_BT601;
                }
                /* disable debayer module */
                //WRITE_REG(debayer->CONTROL, 0x00000110);
            }
        }

        /* set bayer type for acq */
        switch (input_fmt->mbus_code)
        {
        case MEDIA_BUS_FMT_SBGGR8_1X8:
        case MEDIA_BUS_FMT_SBGGR10_1X10:
        case MEDIA_BUS_FMT_SBGGR12_1X12:
            acq_prop |= ISP_TOP_ACQ_PROP_BAYER_PATTERN_BGGR;
            break;
        case MEDIA_BUS_FMT_SGBRG8_1X8:
        case MEDIA_BUS_FMT_SGBRG10_1X10:
        case MEDIA_BUS_FMT_SGBRG12_1X12:
            acq_prop |= ISP_TOP_ACQ_PROP_BAYER_PATTERN_GBRG;
            break;
        case MEDIA_BUS_FMT_SGRBG8_1X8:
        case MEDIA_BUS_FMT_SGRBG10_1X10:
        case MEDIA_BUS_FMT_SGRBG12_1X12:
            acq_prop |= ISP_TOP_ACQ_PROP_BAYER_PATTERN_GRBG;
            break;
        case MEDIA_BUS_FMT_SRGGB8_1X8:
        case MEDIA_BUS_FMT_SRGGB10_1X10:
        case MEDIA_BUS_FMT_SRGGB12_1X12:
            acq_prop |= ISP_TOP_ACQ_PROP_BAYER_PATTERN_RGGB;
            break;
        default:
            break;
        }

        if (mbus_config->mbus_type == CAMERA_MBUS_BT656 ||
                mbus_config->mbus_type == CAMERA_MBUS_PARALLEL)
        {
            flags = mbus_config->flags & MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_MASK;
            if (flags == MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_RISING)
            {
                acq_prop |= ISP_TOP_ACQ_PROP_SAMPLE_EDGE_POSITIVE;
            }
            else
            {
                acq_prop |= ISP_TOP_ACQ_PROP_SAMPLE_EDGE_NEGATIVE;
            }

            flags = mbus_config->flags & MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_MASK;
            if (flags == MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_HIGHT)
            {
                acq_prop |= ISP_TOP_ACQ_PROP_VSYNC_POL_HIGH;
            }
            else
            {
                acq_prop |= ISP_TOP_ACQ_PROP_VSYNC_POL_LOW;
            }

            flags = mbus_config->flags & MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_MASK;
            if (flags == MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_HIGHT)
            {
                acq_prop |= ISP_TOP_ACQ_PROP_HSYNC_POL_HIGH;
            }
            else
            {
                acq_prop |= ISP_TOP_ACQ_PROP_HSYNC_POL_LOW;
            }
        }

        if (input_fmt->fmt_type == ISP_DEVICE_FORMAT_YUV)
        {
            switch (input_fmt->data.yuv_input_order)
            {
            case ISP_YUV_IN_ORDER_YUYV:
                acq_prop |= ISP_TOP_ACQ_PROP_CCIR_SEQ_YUYV;
                break;
            case ISP_YUV_IN_ORDER_YVYU:
                acq_prop |= ISP_TOP_ACQ_PROP_CCIR_SEQ_YVYU;
                break;
            case ISP_YUV_IN_ORDER_VYUY:
                acq_prop |= ISP_TOP_ACQ_PROP_CCIR_SEQ_VYUY;
                break;

            case ISP_YUV_IN_ORDER_UYVY:
                acq_prop |= ISP_TOP_ACQ_PROP_CCIR_SEQ_UYVY;
                break;
            }
        }

        switch (input_fmt->data_width)
        {
        case 8:
            acq_prop |= ISP_TOP_ACQ_PROP_INPUT_SELECTION_8BITS_4ZEROES_LSBS;
            break;
        case 10:
            acq_prop |= ISP_TOP_ACQ_PROP_INPUT_SELECTION_10BITS_2ZEROES_LSBS;
            break;
        case 12:
            acq_prop |= ISP_TOP_ACQ_PROP_INPUT_SELECTION_12BITS;
            break;

        default:
            acq_prop |= ISP_TOP_ACQ_PROP_INPUT_SELECTION_8BITS_4ZEROES_LSBS;
            break;
        }

        isp_ctrl |= ISP_CTRL0_SW_INFORM_ENABLE_MASK |
                    ISP_CTRL0_SYS_CFG_GEN_UPD_FIX_MASK |
                    ISP_CTRL0_SW_ISP_ENABLE_MASK |
                    ISP_CTRL0_SYS_CFG_FORCE_UPD_MASK |
                    ISP_CTRL0_ISP_AWB_ENABLE_MASK;

        irq_mask = ISP_ISP_IMSC_SW_IMSC_V_START_MASK |
                   ISP_ISP_IMSC_SW_IMSC_FRAME_MASK |
                   ISP_ISP_IMSC_SW_IMSC_PIC_SIZE_ERR_MASK;

#ifdef RT_USING_RK_AOV
        acq_prop |= RKISP_READ(ISP3X_ISP_CTRL1);
#endif
        RKISP_WRITE(ISP3X_ISP_CTRL1, acq_prop);

        /* Acquisition Size */
        RKISP_WRITE(ISP3X_ISP_ACQ_H_OFFS, 0x0);
        RKISP_WRITE(ISP3X_ISP_ACQ_V_OFFS, 0x0);
        RKISP_WRITE(ISP3X_ISP_ACQ_H_SIZE, mbus_fmt->width * acq_mult);
        RKISP_WRITE(ISP3X_ISP_ACQ_V_SIZE, mbus_fmt->height);
        RKISP_WRITE(ISP3X_ISP_ACQ_NR_FRAMES, 0x0);

        /* ISP Out Area */
        RKISP_WRITE(ISP3X_ISP_OUT_H_OFFS, 0x0);
        RKISP_WRITE(ISP3X_ISP_OUT_V_OFFS, 0x0);
        RKISP_WRITE(ISP3X_ISP_OUT_H_SIZE, mbus_fmt->width);
        RKISP_WRITE(ISP3X_ISP_OUT_V_SIZE, mbus_fmt->height);
        RKISP_WRITE(ISP3X_ISP_IMSC, irq_mask);
        RKISP_WRITE(ISP3X_ISP_CTRL0, isp_ctrl);
    }
    else
    {
        isp_ctrl = 0x0;
        RKISP_WRITE(ISP3X_ISP_CTRL0, isp_ctrl);
    }

    rk_isp_function_exit();
}

/**
 * @brief  Enable or disable subdev of isp.
 * @param dev : The isp device.
 * @param enable : The state of subdev.
 */
static rt_err_t rk_isp_hw_set_subdev_enable(struct rk_isp_dev *dev, bool enable)
{
    rt_err_t ret;
    struct rk_camera_device *subdev;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    subdev = dev->subdev;
    if (subdev->ops)
    {
        if (enable)
            ret = subdev->ops->control(subdev,
                                       RK_DEVICE_CTRL_CAMERA_STREAM_ON,
                                       NULL);
        else
            ret = subdev->ops->control(subdev,
                                       RK_DEVICE_CTRL_CAMERA_STREAM_OFF,
                                       NULL);
    }
    else
    {
        ISP_INFO(dev, "Warning: the subdev ops is null, check it!\n");
        ret = RT_ENOSYS;
    }

    rk_isp_function_exit();

    return ret;
}

/**
 * @brief  The stream on process of isp registers
 * @param dev : The isp device.
 */
static rt_err_t rk_isp_hw_set_stream_on(struct rk_isp_dev *dev)
{
    uint32_t mask;
    rt_err_t ret = RT_EOK;
    struct isp_output_info *output_info;
    uint32_t width, height;

    RT_ASSERT(dev != RT_NULL);

    rk_isp_function_enter();

    output_info = &dev->output;

    if (output_info->is_crop)
    {
        width = output_info->crop.width;
        height = output_info->crop.height;
    }
    else
    {
        width = output_info->pix_format.width;
        height = output_info->pix_format.height;
    }

    /* start to config isp */
    rk_isp_hw_set_clk(dev, ENABLE);
    rk_isp_hw_set_vicap_clk(dev, ENABLE);
    rk_isp_hw_set_csi2_clk(dev, ENABLE);
    rk_isp_hw_set_dphy_clk(dev, ENABLE);

    /* Set is_streamoff to false right before streaming to avoid race,
     * so we can make sure irq handler read the right value
     */
    dev->is_streamoff = false;

    rk_isp_hw_init_mp_yuv_addr(dev);
#if 0
    rk_isp_hw_set_mi(dev);
    mask = READ_REG(mi->MI_WR_INIT);
    mask |= CIF_MI_INIT_SOFT_UPD;
    WRITE_REG(mi->MI_WR_INIT, mask);

    rk_isp_config(dev);
    rk_isp_hw_set_isp_top(dev, ENABLE);
    rk_isp_hw_set_marvin_ctrl(dev, ENABLE);
#else
    RKISP_WRITE(ISP3X_SWS_CFG, 0x8);
    RKISP_WRITE(ISP3X_DUAL_CROP_CTRL, 0x20);
    RKISP_WRITE(ISP3X_MAIN_RESIZE_CTRL, 0);
    RKISP_WRITE(ISP3X_MAIN_RESIZE_CTRL, 0x100);
    RKISP_WRITE(ISP3X_VI_DPCL, 0x5);
    RKISP_WRITE(ISP3X_MI_MP_WR_Y_LLENGTH, width);
    RKISP_WRITE(ISP3X_MI_MP_WR_Y_PIC_SIZE, width * height);
    RKISP_WRITE(ISP3X_MI_WR_XTD_FORMAT_CTRL, 0);
    RKISP_WRITE(ISP3X_MPFBC_CTRL, 0x502);
    RKISP_WRITE(ISP32_MI_MP_WR_CTRL, 0x100);
    RKISP_WRITE(ISP3X_MI_WR_CTRL, 0x217a2000);
    RKISP_WRITE(ISP3X_VI_DPCL, 0x205);

    rk_isp_module_init(dev);
    RKISP_WRITE(ISP3X_ISP_CTRL0, 0x6397); //
#ifdef RT_USING_RK_AOV
    RKISP_WRITE(ISP3X_MI_WR_CTRL, 0x217a2000); //bit0 enable mi
#else
    RKISP_WRITE(ISP3X_MI_WR_CTRL, 0x217a2001); //bit0 enable mi
#endif
    RKISP_WRITE(ISP3X_MI_WR_INIT, 0x10);

    rk_isp_hw_set_isp_top(dev, ENABLE);
    RKISP_WRITE(ISP3X_DEBAYER_CONTROL, 0x111);
    mask = RKISP_READ(ISP3X_MI_IMSC);
    mask |= ISP3X_MI_MP_FRAME;
    RKISP_WRITE(ISP3X_MI_IMSC, mask);

#endif

    rk_isp_hw_vicap_init(dev);
    rk_isp_hw_csi_host_init(dev);
    rk_isp_hw_mipi_lvds_dphy_init(dev);

#ifndef RT_USING_CAM_STREAM_ON_LATE
    ret = rk_isp_hw_set_subdev_enable(dev, ENABLE);
#endif

    rk_isp_function_exit();

    return ret;
}

/**
 * @brief  malloc buf for isp driver
 * @param buf : The buf stores malloced image address
 * @param size : The size of buf to be malloced.
 */
static void rk_isp_malloc_image_buf(struct isp_video_buf *buf,
                                    uint32_t size)
{
    struct rk_isp_dev *dev = &isp_instance;

    /* address aligned with 4k for isp iommu*/
    buf->addr = (uint8_t *)rk_isp_heap_malloc(dev, size, 4096);
}

/**
 * @brief free buf malloced by isp driver
 * @param buf : The isp driver's buf to be freed.
 */
static void rk_isp_free_image_buf(struct isp_video_buf *buf)
{
    if (buf->malloced_by_dma_large)
    {
        rk_dma_free_large(buf->addr);
    }
    else
    {
        rk_mem_free(buf->addr);
    }
}

/**
 * @brief  Release all buffers in videobuf queue
 * @param dev : The isp device.
 */
static rt_err_t rk_isp_release_buf(struct rk_isp_dev *dev)
{
    uint8_t index;
    struct isp_video_buf *buf;
    struct isp_videobuf_queue *queue;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);

    queue = &dev->buf_queue;
    if (queue->owned_by_drv_count)
    {
        for (index = 0; index < queue->num_buffers; index++)
        {
            buf = &queue->bufs[index];

            if (buf->state == ISP_BUF_STATE_ACTIVE)
            {
                ISP_DBG("release buf\n");
                buf->listtype = ISP_BUF_LIST_TYPE_MP;
                rk_isp_videobuf_done(buf, ISP_BUF_STATE_ERROR, buf->listtype);
            }
        }

        if (queue->owned_by_drv_count != 0)
        {
            ISP_INFO(dev, "Warning: there is still buf enqueued in driver!\n");
        }
    }

    /* isp needs to read data in linux kernel, so no release */

    for (index = 0; index < queue->num_buffers; index++)
    {
        buf = &queue->bufs[index];
        rk_isp_free_image_buf(buf);
    }

    queue->streaming = 0;
    queue->queued_count = 0;
    queue->done_count = 0;
    queue->owned_by_drv_count = 0;
    queue->num_buffers = 0;
    queue->min_buffers_needed = ISP_VIDEO_BUF_NUM_MIN;

    rt_list_init(&queue->queued_list);
    rt_list_init(&queue->done_list);
    rt_list_init(&queue->actived_list);
    rt_list_init(&queue->lf_done_list);
    rt_list_init(&queue->mf_done_list);
    rt_list_init(&queue->sf_done_list);

    rk_isp_function_exit();

    return RT_EOK;
}

/**
 * @brief  The application request buffers from kernel
 * @param dev : The isp device.
 * @param req : Contains the num of buffers required by application.
 */
static rt_err_t rk_isp_ctrl_reqbuf(struct rk_isp_dev *dev,
                                   struct isp_video_requestbuffers *req)
{
    uint8_t index, num_allocated_buffers, num_buffers,
            planes;
    uint32_t width, height, size_per_buf = 0, x_ratio = 0x1, y_ratio = 0x1, mod;
    struct isp_videobuf_queue *queue = &dev->buf_queue;
    struct isp_output_info *output_info = &dev->output;
    struct isp_plane_info *plane_info = &dev->plane_info;
    const struct isp_output_fmt *isp_output_format = output_info->output_fmt;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(req != RT_NULL);

    if (queue->streaming)
    {
        ISP_INFO(dev, "Err: the queue is streaming!\n");
        return -RT_EBUSY;
    }

    num_buffers = req->count;
    if (num_buffers > ISP_VIDEO_BUF_NUM_MAX)
    {
        ISP_INFO(dev, "Err: the requested buf num is:%d beyond 32!\n", num_buffers);
        return -RT_EBUSY;
    }

    if (output_info->is_crop)
    {
        width = output_info->crop.width;
        height = output_info->crop.height;
    }
    else
    {
        width = output_info->pix_format.width;
        height = output_info->pix_format.height;
    }

    if (req->type == ISP_BUF_MEM_TYPE_COMPACT)
    {
        dev->buf_mem_type = ISP_BUF_MEM_TYPE_COMPACT;
        if (isp_output_format->fmt_type == ISP_DEVICE_FORMAT_RAW)
        {
            /* size cal by actual data width */
            bytesperline = width * isp_output_format->format_data_width;
            bytesperline >>= 3;
            bytesperline = RT_ALIGN(bytesperline, RK_ISP_ROW_BYTE_ALIGN);
            size_per_buf = (height + 24) * bytesperline; //There is needed more than 24 lines to fix isp controler bug
        }
        plane_info->plane_size[0] = size_per_buf;

        /* size aligned with 4096 */
        mod = size_per_buf % 4096;
        if (mod)
        {
            size_per_buf += (4096 - mod);
        }
    }
    else
    {
        dev->buf_mem_type = ISP_BUF_MEM_TYPE_NORMAL;
        fcc_xysubs(isp_output_format->fourcc, &x_ratio, &y_ratio);
        /* height to align with 16 when allocating memory
         * so that Rockchip encoder can use DMA buffer directly
         */
        height = RK_ALIGN(height, 16);
        size_per_buf = 0;
        if (isp_output_format->cplanes)
            planes = isp_output_format->cplanes;
        else
            planes = isp_output_format->mplanes;

        for (index = 0; index < planes; index++)
        {
            uint32_t bpp, bpl, size;

            if (index > 0)
            {
                /* round up */
                width = (width + x_ratio - 1) / x_ratio;
                height = (height + y_ratio - 1) / y_ratio;
            }
            bpp = rk_isp_align_bits_per_pixel(isp_output_format, index);
            bpl = width * bpp / ISP_YUV_STORED_BIT_WIDTH;
            size = bpl * height;

            plane_info->plane_size[index] = size;

            size_per_buf += size;
        }
    }

    num_allocated_buffers = 0;
    for (index = 0; index < num_buffers; index++)
    {
        struct isp_video_buf *buf;

        buf = &queue->bufs[index];
        if (buf)
        {
            /* the memory address needed to be aligned with 8bytes */
            rk_isp_malloc_image_buf(buf, size_per_buf);
            if (buf->addr)
            {
                buf->index = index;
                buf->bufsize = size_per_buf;
                buf->buf_q = queue;
                buf->state = ISP_BUF_STATE_DEQUEUED;
                buf->listtype = ISP_BUF_LIST_TYPE_IDLE;
                rt_list_init(&buf->done_entry);
                rt_list_init(&buf->queued_entry);
                rt_list_init(&buf->actived_entry);
                num_allocated_buffers += 1;
            }
            else
            {
                ISP_INFO(dev, "Err: malloc buf[%d] memory:%d failed\n", index, size_per_buf);
                break;
            }
        }
    }

    queue->num_buffers = num_allocated_buffers;
    rk_isp_function_exit();

    return RT_EOK;
}


/**
 * @brief  The application queries buffers have been required.
 * @param dev : The isp device.
 * @param buf : The video buf to be queried.
 */
static rt_err_t rk_isp_ctrl_querybuf(struct rk_isp_dev *dev, struct isp_video_buf *buf)
{
    struct isp_videobuf_queue *queue;
    struct isp_video_buf *buf_in_q;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(buf != RT_NULL);

    queue = &dev->buf_queue;

    if (buf->index >= queue->num_buffers)
    {
        ISP_INFO(dev, "Err: buf index out of range!\n");
        return -RT_EINVAL;
    }

    buf_in_q = &queue->bufs[buf->index];
    if (buf_in_q)
    {
        buf->index = buf_in_q->index;
        buf->bufsize = buf_in_q->bufsize;
    }
    else
    {
        ISP_INFO(dev, "Err: the buf[%d] is null!\n");
        return -RT_ENOMEM;
    }

    switch (buf_in_q->state)
    {
    case ISP_BUF_STATE_QUEUED:
    case ISP_BUF_STATE_ACTIVE:
        buf->state |= ISP_BUF_STATE_QUEUED;
        ISP_DBG("buf_in_q[%d]:active or queued\n", buf_in_q->index);
        break;
    case ISP_BUF_STATE_ERROR:
        buf->state |= ISP_BUF_STATE_ERROR;
        ISP_DBG("buf_in_q[%d]: error\n", buf_in_q->index);
    /* fall through */
    case ISP_BUF_STATE_DONE:
        buf->state |= ISP_BUF_STATE_DONE;
        ISP_DBG("buf_in_q[%d]:done\n", buf_in_q->index);
        break;
    case ISP_BUF_STATE_PREPARED:
        buf->state |= ISP_BUF_STATE_PREPARED;
        ISP_DBG("buf_in_q[%d]:prepared\n", buf_in_q->index);
        break;
    case ISP_BUF_STATE_PREPARING:
    case ISP_BUF_STATE_DEQUEUED:
    case ISP_BUF_STATE_REQUEUEING:
        ISP_DBG("buf_in_q[%d]:preparing,dequeued,requeueing\n", buf_in_q->index);
        /* nothing */
        break;
    }

    rk_isp_function_exit();

    return RT_EOK;
}

/**
 * @brief  The application queues buffer from application to kernel.
 * @param dev : The isp device.
 * @param buf : The video buf to be queued.
 */
rt_err_t rk_isp_ctrl_qbuf(struct rk_isp_dev *dev, struct isp_video_buf *buf)
{
    rt_base_t level;
    struct isp_videobuf_queue *queue;
    struct isp_video_buf *buf_in_q;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(buf != RT_NULL);

    queue = &dev->buf_queue;
    if (buf->index >= queue->num_buffers)
    {
        ISP_INFO(dev, "Err: buf index out of range !\n");
        return -RT_EINVAL;
    }

    if (queue->bufs[buf->index].addr == RT_NULL)
    {
        ISP_INFO(dev, "Err: buf[%d] is NULL\n", buf->index);
        return -RT_EINVAL;
    }
    buf_in_q = &queue->bufs[buf->index];

    switch (buf_in_q->state)
    {
    case ISP_BUF_STATE_DEQUEUED:
    case ISP_BUF_STATE_PREPARED:
        break;

    case ISP_BUF_STATE_PREPARING:
        ISP_INFO(dev, "buf[%d] still being prepared!\n", buf_in_q->index);
        return -RT_EINVAL;
        break;

    default:
        ISP_INFO(dev, "invalid buffer state %d\n", buf_in_q->state);
        return -RT_EINVAL;
    }

    level = rt_hw_interrupt_disable();
    rt_list_insert_before(&queue->queued_list,
                          &buf_in_q->queued_entry);
    queue->queued_count++;
    buf_in_q->state = ISP_BUF_STATE_QUEUED;
    buf_in_q->listtype = ISP_BUF_LIST_TYPE_IDLE;
    rt_hw_interrupt_enable(level);

    if (queue->streaming)
    {
        __enqueue_in_driver(buf_in_q);
    }

    rk_isp_function_exit();

    return RT_EOK;
}

#define SHOW_BUF_LIST_TYPE_LOG(type)           \
({\
    char *name;\
    switch(type)\
    {\
        case ISP_BUF_LIST_TYPE_LF:\
        name = "long frame";\
        break;\
        case ISP_BUF_LIST_TYPE_MF:\
        name = "middle frame";\
        break;\
        case ISP_BUF_LIST_TYPE_SF:\
        name = "short frame";\
        break;\
        case ISP_BUF_LIST_TYPE_MP:\
        name = "main path";\
        break;\
        case ISP_BUF_LIST_TYPE_SP:\
        name = "self path";\
        break;\
        default:\
        name = "unknown";\
        break;\
    }\
    name;\
})

/**
 * @brief  The application dqueues buffer from kernel to application.
 * @param dev : The isp device.
 * @param buf : The video buf to be dqueued.
 */
rt_err_t rk_isp_ctrl_dqbuf(struct rk_isp_dev *dev, struct isp_video_buf *buf)
{
    rt_err_t ret;
    rt_base_t level;
    struct isp_videobuf_queue *queue = RT_NULL;
    struct isp_video_buf *dqbuf = RT_NULL;
    rt_list_t *done_list_head = NULL;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);

    queue = &dev->buf_queue;
    if (!queue->streaming)
    {
        ISP_INFO(dev, "Err: isp is streamed off, no buffer to be dqueued!\n");

        return -RT_ERROR;
    }

    ret = rt_sem_take(queue->qsem, rt_tick_from_millisecond(100));
    if (ret != RT_EOK)
    {
        ISP_INFO(dev, "Err: take dqbuf sem failed!\n");

        return -RT_ERROR;
    }

    level = rt_hw_interrupt_disable();
    if (buf->listtype == ISP_BUF_LIST_TYPE_MP)
    {
        done_list_head = &queue->done_list;
    }
    else if (buf->listtype == ISP_BUF_LIST_TYPE_LF)
    {
        done_list_head = &queue->lf_done_list;
    }
    else if (buf->listtype == ISP_BUF_LIST_TYPE_MF)
    {
        done_list_head = &queue->mf_done_list;
    }
    else if (buf->listtype == ISP_BUF_LIST_TYPE_SF)
    {
        done_list_head = &queue->sf_done_list;
    }
    if (!rt_list_isempty(done_list_head))
    {
        dqbuf = rt_list_first_entry(done_list_head,
                                    struct isp_video_buf,
                                    done_entry);
        rt_list_remove(&dqbuf->done_entry);
        rt_list_remove(&dqbuf->queued_entry);
        dqbuf->state = ISP_BUF_STATE_DEQUEUED;
        queue->queued_count--;
        rt_memcpy(buf, dqbuf, sizeof(struct isp_video_buf));
        rt_hw_interrupt_enable(level);
    }
    else
    {
        rt_hw_interrupt_enable(level);
        ret = -RT_ENOMEM;
        ISP_INFO(dev, "Warning:no buf for %s dqueuing,%d\n",
                 SHOW_BUF_LIST_TYPE_LOG(buf->listtype));
    }

    rk_isp_function_exit();

    return ret;
}

/**
 * @brief  The application streams on isp.
 * @param dev : The isp device.
 */
rt_err_t rk_isp_ctrl_stream_on(struct rk_isp_dev *dev)
{
    rt_err_t ret = RT_EOK;
    struct isp_video_buf *buf;
    struct isp_videobuf_queue *queue;
    uint32_t index;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);

    queue = &dev->buf_queue;
    if (queue->streaming)
    {
        ISP_INFO(dev, "isp has been stream on!\n");
        return RT_EOK;
    }

    if (!queue->num_buffers ||
            queue->num_buffers < queue->min_buffers_needed)
    {
        ISP_INFO(dev, "Err: the amount of buf is %d, is not enough to stream!\n", queue->num_buffers);
        return -RT_EINVAL;
    }

    for (index = 0; index < RK_ISP_IRQ_MAX; index++)
    {
        if (dev->irq_set[index].irq_handler != RK_NULL)
            rt_hw_interrupt_umask(dev->irq_set[index].irq_num);
    }

    if (queue->num_buffers >= queue->min_buffers_needed)
    {
        rt_list_for_each_entry(buf, &queue->queued_list, queued_entry)
        {
            __enqueue_in_driver(buf);
            ISP_DBG("enqueue buf[%d]:0x%x:0x%x, state:0x%x,drv_count:%d\n", buf->index,
                    (uint32_t)buf->addr, *(uint32_t *)buf->addr,
                    (uint32_t)buf->state, queue->owned_by_drv_count);
        }
        ret = rk_isp_hw_set_stream_on(dev);
    }

    queue->streaming = 1;

    rk_isp_function_exit();

    return ret;
}

/**
 * @brief  The application streams off isp.
 * @param dev : The isp device.
 */
rt_err_t rk_isp_ctrl_stream_off(struct rk_isp_dev *dev, bool is_force)
{
    rt_err_t ret;
    uint32_t index;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);

    dev->is_streamoff = true;
    if (is_force)
    {
        ret = RT_EOK;
    }
    else
    {
        ret = rt_sem_take(dev->streamoff_sem, RT_WAITING_FOREVER);
    }
    if (ret == RT_EOK)
    {

        rk_isp_hw_set_subdev_enable(dev, DISABLE);
        rk_isp_hw_mipi_lvds_dphy_reinit(dev);
        rk_isp_hw_csi_host_reinit(dev);
        rk_isp_hw_vicap_reinit(dev);
        rk_isp_release_buf(dev);

        for (index = 0; index < RK_ISP_IRQ_MAX; index++)
        {
            if (dev->irq_set[index].irq_handler != RK_NULL)
                rt_hw_interrupt_mask(dev->irq_set[index].irq_num);
        }

        dev->buf_addr0 = RT_NULL;
        dev->buf_addr1 = RT_NULL;
        dev->buf_lf_addr0 = RT_NULL;
        dev->buf_lf_addr1 = RT_NULL;
        dev->buf_mf_addr0 = RT_NULL;
        dev->buf_mf_addr1 = RT_NULL;
        dev->buf_sf_addr0 = RT_NULL;
        dev->buf_sf_addr1 = RT_NULL;
    }
    else
    {
        ISP_INFO(dev, "Err: stream off failed!\n");
    }

    rk_isp_function_exit();

    return ret;
}

#if ISP_WORK_QUEUE_ENABLE
/**
 * @brief  The work handler of frame's oneframe mode and pingpong mode
 * @param parameter : The isp device pointer.
 */
static void rk_isp_workqueue_frame_handler(void *parameter)
{
    /*TODO:*/
    struct rk_isp_dev *dev = (struct rk_isp_dev *)parameter;

    RT_ASSERT(dev != RT_NULL);
    rk_isp_hw_update_frame_yuv_addr(dev);
}

/**
 * @brief  The work handler of block pingpong mode
 * @param parameter : The isp device pointer.
 * maybe not used.
 */
static void rk_isp_workqueue_block_handler(void *parameter)
{
    struct isp_video_buf *buf = (struct isp_video_buf *)parameter;

    rk_isp_videobuf_done(buf, ISP_BUF_STATE_DONE);
}

/**
 * @brief  Init isp workqueue
 * maybe not used.
 */
rt_err_t rk_isp_workqueue_init(void)
{
    /*TODO:may be used*/
    return RT_EOK;
}

/**
 * @brief  The work queue of isp to handle task of different work mode.
 * @param fun : The handler instance of different work mode.
 * @param parameter : The isp device pointer.
 * maybe not be used.
 */
static rt_err_t rk_isp_workqueue_dowork(struct rk_isp_dev *dev,
                                        void (*fun)(void *parameter),
                                        void *parameter)
{
    rt_err_t ret;
    struct rk_isp_work *isp_work;
    struct rt_workqueue *workqueue;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);

    workqueue = dev->isp_workqueue;
    if (workqueue == RT_NULL)
    {
        ISP_INFO(dev, "%s:(line:%d):isp workqueue is null\n",
                 __FUNCTION__, __LINE__);
        return -RT_EINVAL;
    }

    isp_work = &dev->isp_work;
    if (isp_work == RT_NULL)
    {
        ISP_INFO(dev, "%s:(line:%d):isp workqueue handle fun is null\n",
                 __FUNCTION__, __LINE__);
        return -RT_EINVAL;
    }

    if (fun == RT_NULL)
    {
        ISP_INFO(dev, "%s:(line:%d):isp workqueue handle fun is null\n",
                 __FUNCTION__, __LINE__);
        return -RT_EINVAL;
    }

    isp_work->fun = fun;
    isp_work->parameter = parameter;
    rt_work_init(&isp_work->work, rk_isp_workqueue_fun, isp_work);
    ret = rt_workqueue_dowork(workqueue, &isp_work->work);
    if (ret != RT_EOK)
    {
        ISP_INFO(dev, "%s:(line:%d):isp workqueue dowork failed!\n",
                 __FUNCTION__, __LINE__);
    }

    rk_isp_function_exit();

    return ret;
}
#endif

static void rk_isp_sub_isp_top_irq(struct rk_isp_dev *dev, uint32_t stat)
{
#if 0

    if (stat & ISP_ISP_MIS_MIS_V_START_MASK)
    {
        rt_kprintf("isp vstart id %d\n", dev->frame_id);
    }

    if (stat & ISP_ISP_MIS_MIS_DATA_LOSS_MASK)
    {
        rt_kprintf("isp data loss\n");
    }

    if (stat & ISP_ISP_MIS_MIS_PIC_SIZE_ERR_MASK)
    {
        rt_kprintf("isp pic size err\n");
    }

    if (stat & ISP_ISP_MIS_MIS_FRAME_IN_MASK)
    {
        rt_kprintf("isp frame in\n");
    }

    if (stat & ISP_ISP_MIS_MIS_FRAME_MASK)
    {
        rt_kprintf("isp frame out\n");
    }
#endif

    RKISP_WRITE(ISP3X_ISP_ICR, stat);

}

static void rk_isp_sub_csi2rx_irq(struct rk_isp_dev *dev)
{
#if 0
    uint32_t mask;
    struct CSI2RX_REG *csi2rx;

    csi2rx = dev->csi2rx;

    mask = READ_REG(csi2rx->ERR_PHY);
    if (mask)
    {
        rt_kprintf("mipi err phy:0x%x\n", mask);
    }

    mask = READ_REG(csi2rx->ERR_PACKET);
    if (mask)
    {
        rt_kprintf("mipi err packet:0x%x\n", mask);
    }

    mask = READ_REG(csi2rx->ERR_OVERFLOW);
    if (mask)
    {
        rt_kprintf("mipi err overflow:0x%x\n", mask);
    }

    mask = READ_REG(csi2rx->ERR_PACKET);
    if (mask)
    {
        rt_kprintf("mipi err packet:0x%x\n", mask);
    }

    mask = READ_REG(csi2rx->ERR_STAT);
    if (mask & 0xeff00)
        rt_kprintf("MIPI error: size: 0x%08x\n", mask);
    if (mask & MIPI_DROP_FRM)
        rt_kprintf("MIPI drop frame\n");
    rk_isp_luma_isr(dev, mask);
#endif
}

/**
 * @brief  Handle the interrupt of isp.
 */
static void rk_irq_handler_isp(int vector, void *param)
{
    struct rk_isp_dev *ispdev = (struct rk_isp_dev *)param;
    uint32_t mis_val = RK_NULL;

    rk_isp_function_enter();

    rt_interrupt_enter();

    mis_val = RKISP_READ(ISP3X_ISP_MIS);
    if (mis_val)
    {
        rk_isp_isr(ispdev, mis_val);
        rk_isp_sub_isp_top_irq(ispdev, mis_val);
#ifdef RT_USING_RK_AOV
        if (ispdev->is_streamoff && !(RKISP_READ(ISP3X_MI_WR_CTRL) & 1))
            rt_sem_release(ispdev->streamoff_sem);
#endif
    }
    rt_interrupt_leave();

    rk_isp_function_exit();
}

/**
 * @brief  Handle the interrupt of isp mi.
 */
static void rk_irq_handler_mi(int vector, void *param)
{
    struct rk_isp_dev *ispdev = (struct rk_isp_dev *)param;
    struct isp_video_buf *buf = RK_NULL;
    uint32_t mis_val = RK_NULL;

    rk_isp_function_enter();

    rt_interrupt_enter();

    mis_val = RKISP_READ(ISP3X_MI_MIS);
    if (mis_val)
    {
        RKISP_WRITE(ISP3X_MI_ICR, mis_val);
        if (mis_val & ISP3X_MI_MP_FRAME)
        {
            buf = rk_isp_hw_update_mp_yuv_addr(ispdev, mis_val);
            if (buf)
                rk_isp_videobuf_done(buf, ISP_BUF_STATE_DONE, ISP_BUF_LIST_TYPE_MP);
        }

        if (mis_val & ISP3X_MI_DMA_READY)
        {
            /* TODO: dma rx */
        }

        if (mis_val & (ISP3X_MI_MP_FRAME | ISP3X_MI_SP_FRAME))
        {
            if (ispdev->is_streamoff)
            {
                rt_sem_release(ispdev->streamoff_sem);
            }
        }
    }

    ISP_DBG("%s mis_val:0x%x\n", __FUNCTION__, mis_val);
    rt_interrupt_leave();
    rk_isp_function_enter();
}

/**
 * @brief  Handle the interrupt of isp mipi.
 */
static void rk_irq_handler_mipi(int vector, void *param)
{
    struct rk_isp_dev *ispdev = (struct rk_isp_dev *)param;

    rk_isp_function_enter();

    rt_interrupt_enter();

    rk_isp_sub_csi2rx_irq(ispdev);

    rt_interrupt_leave();

    rk_isp_function_exit();

}

/**
 * @brief  Init the isp device for application.
 * @param dev : The isp device.
 */
rt_err_t rk_isp_init(struct rk_isp_device *dev)
{
    uint8_t i;
    struct rk_isp_dev *ispdev = rk_get_isp_dev(dev);
    struct rk_camera_device *subdev = ispdev->subdev;
    struct rk_camera_mbus_framefmt *mbus_fmt;
    const struct isp_input_fmt *fmt;
    struct config_param *param = &g_param;//(struct config_param *)CONFIG_ADDRESS;
    struct isp_init_info *isp_mem_config = &param->isp;

    RT_ASSERT(dev != RT_NULL);

    rk_isp_function_enter();

    if (!subdev)
    {
        subdev = rk_isp_ctrl_get_subdev(ispdev);
        if (!subdev)
        {
            ISP_INFO(ispdev, "Can't find isp subdev, please check it!\n");
            return RT_NULL;
        }
        else
        {
            ispdev->subdev = subdev;
            rk_isp_init_subdev(ispdev);
        }
    }

    mbus_fmt = &subdev->info.mbus_fmt;
    fmt = RT_NULL;
    for (i = 0; i < ARRAY_SIZE(g_input_fmts); i++)
    {
        if (mbus_fmt->pixelcode == g_input_fmts[i].mbus_code)
        {
            fmt = &g_input_fmts[i];
            break;
        }
    }
    if (!fmt)
    {
        ISP_INFO(ispdev, "Warning: the input format is not found\n");
    }

    rt_memcpy(&ispdev->input.cam_info,
              &subdev->info,
              sizeof(struct rk_camera_info));
    rt_memcpy(&ispdev->input.input_fmt,
              fmt,
              sizeof(struct isp_input_fmt));

    ispdev->output.pix_format.width = mbus_fmt->width;
    ispdev->output.pix_format.height = mbus_fmt->height;
    ispdev->output.is_crop = false;
    ispdev->output.output_fmt = &g_output_fmts[0];
    ispdev->hdr_mode = subdev->info.hdr_mode;

    rk_isp_init_heap(ispdev,
                     isp_mem_config->share_mem_addr + SHARED_MEM_RESERVED_HEAD_SIZE,
                     isp_mem_config->share_mem_size - SHARED_MEM_RESERVED_HEAD_SIZE);

    rk_isp_function_exit();

    return RT_EOK;
}

/**
 * @brief  Open the isp device for application.
 * @param dev : The isp device.
 * @param oflag : The flag of open.
 */
rt_err_t rk_isp_open(struct rk_isp_device *dev, uint16_t oflag)
{
    struct rk_isp_dev *ispdev;
    struct isp_videobuf_queue *queue;

    ispdev = rk_get_isp_dev(dev);

    if (!ispdev->streamoff_sem)
    {
        ispdev->streamoff_sem = rt_sem_create("offsem", 0, RT_IPC_FLAG_FIFO);
        if (!ispdev->streamoff_sem)
        {
            ISP_INFO(ispdev, "create stream off semaphore failed!\n");
            return -RT_ENOMEM;
        }
    }

    if (!ispdev->stats_sem)
    {
        ispdev->stats_sem = rt_sem_create("luma_sem", 0, RT_IPC_FLAG_FIFO);
        if (!ispdev->stats_sem)
        {
            ISP_INFO(ispdev, "create luma semaphore failed!\n");
            return -RT_ENOMEM;
        }
    }

    queue = &ispdev->buf_queue;
    queue->done_count = 0;
    queue->queued_count = 0;
    queue->streaming = 0;
    queue->num_buffers = 0;
    queue->min_buffers_needed = ISP_VIDEO_BUF_NUM_MIN;
    rt_list_init(&queue->queued_list);
    rt_list_init(&queue->done_list);
    rt_list_init(&queue->actived_list);
    rt_list_init(&queue->lf_done_list);
    rt_list_init(&queue->mf_done_list);
    rt_list_init(&queue->sf_done_list);
    if (!queue->done_lock)
    {
        queue->done_lock = rt_mutex_create("isp_donelist_lock",
                                           RT_IPC_FLAG_FIFO);
        if (!queue->done_lock)
        {
            ISP_INFO(ispdev, "create queue done mutex failed!\n");
            return -RT_ENOMEM;
        }
    }

    if (!queue->qmutex_lock)
    {
        queue->qmutex_lock = rt_mutex_create("isp_qmutex_lock",
                                             RT_IPC_FLAG_FIFO);
        if (!queue->qmutex_lock)
        {
            ISP_INFO(ispdev, "create queue queued mutex failed!\n");
            return -RT_ENOMEM;
        }
    }

    if (!queue->qsem)
    {
        queue->qsem = rt_sem_create("isp_qsem", 0, RT_IPC_FLAG_FIFO);
        if (!queue->qsem)
        {
            ISP_INFO(ispdev, "create queue semaphore failed!\n");
            return -RT_ENOMEM;
        }
    }

    return RT_EOK;
}

/**
 * @brief  Close the isp device for application.
 * @param dev : The isp device.
 */
rt_err_t rk_isp_close(struct rk_isp_device *dev)
{
    struct isp_videobuf_queue *queue;
    struct rk_isp_dev *ispdev;
    struct rt_device *dev_object;

    rk_isp_function_enter();

    ispdev = rk_get_isp_dev(dev);
    dev_object = &ispdev->parent.parent;
    if (dev_object->open_flag & RT_DEVICE_OFLAG_OPEN)
    {
        if (ispdev->streamoff_sem)
        {
            rt_sem_delete(ispdev->streamoff_sem);
            ispdev->streamoff_sem = RT_NULL;
        }

        if (ispdev->stats_sem)
        {
            rt_sem_delete(ispdev->stats_sem);
            ispdev->stats_sem = RT_NULL;
        }

        queue = &ispdev->buf_queue;
        if (queue)
        {
            if (queue->qsem)
            {
                rt_sem_delete(queue->qsem);
                queue->qsem = RT_NULL;
            }
            if (queue->done_lock)
            {
                rt_mutex_delete(queue->done_lock);
                queue->done_lock = RT_NULL;
            }
            if (queue->qmutex_lock)
            {
                rt_mutex_delete(queue->qmutex_lock);
                queue->qmutex_lock = RT_NULL;
            }
        }
    }
    else
    {
        ISP_INFO(ispdev, "Warning: isp maybe has been closed!\n");
        return RT_ENOSYS;
    }

    return RT_EOK;

    rk_isp_function_exit();
}

/**
 * @brief  Control interface for application to control isp device.
 * @param cmd : The control command to set isp device.
 * @param args : The parameter of command argument for setting.
 */
rt_err_t rk_isp_control(struct rk_isp_device *dev, int cmd, void *args)
{
    struct rk_isp_dev *isp = rk_get_isp_dev(dev);
    struct isp_videobuf_queue *queue;

    rt_err_t ret = RT_EOK;

    rk_isp_function_enter();

    RT_ASSERT(isp != RT_NULL);

    queue = &isp->buf_queue;

    if (cmd == RK_DEVICE_CTRL_DEVICE_INIT ||
            cmd == RK_DEVICE_CTRL_ISP_SET_WORKMODE ||
            cmd == RK_DEVICE_CTRL_ISP_SET_FMT ||
            cmd == RK_DEVICE_CTRL_ISP_CROP_IMAGE ||
            cmd == RK_DEVICE_CTRL_ISP_STREAM_ON)
    {
        if (queue->streaming)
        {
            rt_kprintf("Err: set cmd:%d failed, isp is streaminig\n", cmd);
            return RT_EBUSY;
        }
    }

    switch (cmd)
    {
    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_isp_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_ISP_SET_WORKMODE:
    {
        RT_ASSERT(args != RT_NULL);

        ret = rk_isp_ctrl_set_workmode(isp, *(eISP_workMode *)args);
    }
    break;

    case RK_DEVICE_CTRL_ISP_SET_FMT:
    {
        struct rk_camera_pix_format *format;

        RT_ASSERT(args != RT_NULL);

        format = (struct rk_camera_pix_format *)args;
        ret = rk_isp_ctrl_set_format(isp, format);
    }
    break;

    case RK_DEVICE_CTRL_ISP_CROP_IMAGE:
    {
        struct rk_camera_crop_rect *rect;

        RT_ASSERT(args != RT_NULL);

        rect = (struct rk_camera_crop_rect *)args;
        ret = rk_isp_ctrl_set_crop_window(isp, rect);
    }
    break;

    case RK_DEVICE_CTRL_ISP_REQBUF:
    {
        struct isp_video_requestbuffers *req;

        req = (struct isp_video_requestbuffers *)args;
        ret = rk_isp_ctrl_reqbuf(isp, req);
    }
    break;

    case RK_DEVICE_CTRL_ISP_QUERYBUF:
    {
        struct isp_video_buf *buf;

        buf = (struct isp_video_buf *)args;
        ret = rk_isp_ctrl_querybuf(isp, buf);
    }
    break;

    case RK_DEVICE_CTRL_ISP_QBUF:
    {
        RT_ASSERT(args != RT_NULL);

        struct isp_video_buf *buf = (struct isp_video_buf *)args;
        ret = rk_isp_ctrl_qbuf(isp, buf);
    }
    break;

    case RK_DEVICE_CTRL_ISP_DQBUF:
    {
        RT_ASSERT(args != RT_NULL);

        struct isp_video_buf *buf = (struct isp_video_buf *)args;
        ret = rk_isp_ctrl_dqbuf(isp, buf);
    }
    break;

    case RK_DEVICE_CTRL_ISP_STREAM_ON:
    {
        ret = rk_isp_ctrl_stream_on(isp);
    }
    break;

    case RK_DEVICE_CTRL_ISP_STREAM_OFF:
    {
        //RT_ASSERT(args != RT_NULL);
        ret = rk_isp_ctrl_stream_off(isp, *(bool *)args);
    }
    break;

    case RK_DEVICE_CTRL_ISP_GET_SUBDEV:
    {
        struct rk_camera_device *subdev;

        subdev = rk_isp_ctrl_get_subdev(isp);
        if (subdev)
            *(struct rk_camera_device **)args = subdev;
        else
            ret = -RT_ENOMEM;
    }
    break;

    case RK_DEVICE_CTRL_ISP_SET_PARAMS:
    {
        RT_ASSERT(args != RT_NULL);

        struct rkisp_params_buffer *buf = (struct rkisp_params_buffer *)args;
        ret = rk_isp_set_params(isp, buf);
    }
    break;

    case RK_DEVICE_CTRL_ISP_GET_STATS:
    {
        RT_ASSERT(args != RT_NULL);

        struct rkisp_stats_buffer *buf = (struct rkisp_stats_buffer *)args;
        ret = rk_isp_get_stat(isp, buf);
    }
    break;

    default:
    {
        ret = RT_ENOSYS;
    }
    break;

    }

    rk_isp_function_exit();

    return ret;
}

/**
 * @brief  The indicate interfaces of isp device.
 */
rt_err_t rk_isp_rx_indicate(struct rk_isp_device *dev, rt_size_t size)
{
    /*TODO:maybe not used*/
    return RT_EOK;
}

/**
 * @brief  The operation interfaces of isp device for setting.
 */
const static struct rk_isp_ops rk_isp_ops =
{
    rk_isp_init,
    rk_isp_open,
    rk_isp_close,
    rk_isp_control,
    rk_isp_rx_indicate
};

static rt_err_t rk_isp_register(struct rk_isp_dev *dev, char *name)
{
    uint32_t index;
    struct rk_isp_device *isp;
    struct rk_isp_irq *irq_set;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(name != RT_NULL);

    isp = &dev->parent;
    isp->ops = &rk_isp_ops;
    irq_set = dev->irq_set;

#if ISP_WORK_QUEUE_ENABLE
    dev->isp_workqueue = rt_workqueue_create("isp_wq",
                         ISP_WORKQUEUE_STACK_SIZE,
                         ISP_WORKQUEUE_PRIORITY);

    if (!dev->isp_workqueue)
    {
        ISP_INFO(dev, "workqueue created failed!\n");
        return -RT_ERROR;
    }
#endif

    for (index = 0; index < RK_ISP_IRQ_MAX; index++)
    {
        if (irq_set[index].irq_handler != RK_NULL)
        {
            rt_hw_interrupt_install(irq_set[index].irq_num,
                                    irq_set[index].irq_handler,
                                    (void *)dev,
                                    irq_set[index].name);
        }
    }
    rk_isp_function_exit();

    return rk_isp_controller_register(isp, name, dev);
}

static struct rk_isp_irq rkisp_irq_set[RK_ISP_IRQ_MAX] =
{
    [RK_ISP_IRQ_ISP] = {
        .name = "isp_irq_isp",
        .irq_num = ISP_IRQn,
        .irq_handler = &rk_irq_handler_isp
    },
    [RK_ISP_IRQ_MI] = {
        .name = "isp_irq_mi",
        .irq_num = MI_ISP_IRQn,
        .irq_handler = &rk_irq_handler_mi
    },
    [RK_ISP_IRQ_MIPI] = {
        .name = "isp_irq_mipi",
        .irq_num = MIPI_ISP_IRQn,
        .irq_handler = &rk_irq_handler_mipi
    }
};

/**
 * @brief  The probe function for registering isp device.
 */
int rk_device_isp_init(void)
{
    rt_err_t ret;
    struct rk_isp_dev *rk_ispdev = &isp_instance;
    struct config_param *param = &g_param; //(struct config_param *)CONFIG_ADDRESS;
    struct isp_init_info *isp_mem_config = &param->isp;

    RT_ASSERT(csi2_dphy_board.reg != RT_NULL);
    RT_ASSERT(csi2_host0_board.reg != RT_NULL);
    RT_ASSERT(csi2_host1_board.reg != RT_NULL);
    RT_ASSERT(vicap_board.reg != RT_NULL);

    rk_ispdev->csi2_dphy_board = &csi2_dphy_board;
    rk_ispdev->csi2_host0_board = &csi2_host0_board;
    rk_ispdev->csi2_host1_board = &csi2_host1_board;
    rk_ispdev->vicap_board = &vicap_board;

    rk_isp_function_enter();
    isp_mem_config->share_mem_addr = RT_USING_ISP_DDR_ADRESS;
    isp_mem_config->share_mem_size = RT_USING_ISP_DDR_SIZE;
    rt_strncpy(rk_ispdev->name, RK_ISP_DEVICE_NAME,
               RK_ISP_DEVICE_NAME_SIZE);

    rk_ispdev->irq_set = rkisp_irq_set;

    rk_ispdev->isp_clk.isp_aclk = get_clk_gate_from_id(ACLK_ISP3P2_GATE);
    rk_ispdev->isp_clk.isp_hclk = get_clk_gate_from_id(HCLK_ISP3P2_GATE);
    rk_ispdev->isp_clk.isp_pclk = get_clk_gate_from_id(CLK_CORE_ISP3P2_GATE);
    rk_ispdev->isp_clk.isp0_vicap_clk = get_clk_gate_from_id(ISP0CLK_VICAP_GATE);

    rk_ispdev->vicap_clk.vicap_pclk = get_clk_gate_from_id(PCLK_VICAP_GATE);
    rk_ispdev->vicap_clk.vicap_aclk = get_clk_gate_from_id(ACLK_VICAP_GATE);
    rk_ispdev->vicap_clk.vicap_hclk = get_clk_gate_from_id(HCLK_VICAP_GATE);
    rk_ispdev->vicap_clk.vicap_dclk = get_clk_gate_from_id(DCLK_VICAP_GATE);
    rk_ispdev->vicap_clk.vicap_pclk_vepu = get_clk_gate_from_id(PCLK_VICAP_VEPU_GATE);

    if (rk_ispdev->csi2_dphy_board->csi2_dphy1.enable && rk_ispdev->csi2_dphy_board->csi2_dphy2.enable)
    {
        if (!strcmp(rk_ispdev->csi2_dphy_board->csi2_dphy1.isp_subdev_name, RK_ISP_MAIN_SUBDEV_NAME) &&
                rk_ispdev->csi2_dphy_board->csi2_dphy1.csi_host_idx)
        {
            rk_ispdev->vicap_clk.vicap_i0clk = get_clk_gate_from_id(I1CLK_VICAP_GATE);
            rk_ispdev->vicap_clk.vicap_rx0pclk = get_clk_gate_from_id(RX1PCLK_VICAP_GATE);
            rk_ispdev->csi2_clk.csi2_pclk = get_clk_gate_from_id(PCLK_CSIHOST1_GATE);
            rk_ispdev->csi2_clk.csi2_rxbyteclkhs0 = get_clk_gate_from_id(CLK_RXBYTECLKHS_1_GATE);
        }
        else
        {
            rk_ispdev->vicap_clk.vicap_i0clk = get_clk_gate_from_id(I0CLK_VICAP_GATE);
            rk_ispdev->vicap_clk.vicap_rx0pclk = get_clk_gate_from_id(RX0PCLK_VICAP_GATE);
            rk_ispdev->csi2_clk.csi2_pclk = get_clk_gate_from_id(PCLK_CSIHOST0_GATE);
            rk_ispdev->csi2_clk.csi2_rxbyteclkhs0 = get_clk_gate_from_id(CLK_RXBYTECLKHS_0_GATE);
        }
    }
    else if (rk_ispdev->csi2_dphy_board->csi2_dphy1.enable)
    {
        if (rk_ispdev->csi2_dphy_board->csi2_dphy1.csi_host_idx)
        {
            rk_ispdev->vicap_clk.vicap_i0clk = get_clk_gate_from_id(I1CLK_VICAP_GATE);
            rk_ispdev->vicap_clk.vicap_rx0pclk = get_clk_gate_from_id(RX1PCLK_VICAP_GATE);
            rk_ispdev->csi2_clk.csi2_pclk = get_clk_gate_from_id(PCLK_CSIHOST1_GATE);
            rk_ispdev->csi2_clk.csi2_rxbyteclkhs0 = get_clk_gate_from_id(CLK_RXBYTECLKHS_1_GATE);
        }
        else
        {
            rk_ispdev->vicap_clk.vicap_i0clk = get_clk_gate_from_id(I0CLK_VICAP_GATE);
            rk_ispdev->vicap_clk.vicap_rx0pclk = get_clk_gate_from_id(RX0PCLK_VICAP_GATE);
            rk_ispdev->csi2_clk.csi2_pclk = get_clk_gate_from_id(PCLK_CSIHOST0_GATE);
            rk_ispdev->csi2_clk.csi2_rxbyteclkhs0 = get_clk_gate_from_id(CLK_RXBYTECLKHS_0_GATE);
        }
    }
    else if (rk_ispdev->csi2_dphy_board->csi2_dphy2.enable)
    {
        if (rk_ispdev->csi2_dphy_board->csi2_dphy2.csi_host_idx)
        {
            rk_ispdev->vicap_clk.vicap_i0clk = get_clk_gate_from_id(I1CLK_VICAP_GATE);
            rk_ispdev->vicap_clk.vicap_rx0pclk = get_clk_gate_from_id(RX1PCLK_VICAP_GATE);
            rk_ispdev->csi2_clk.csi2_pclk = get_clk_gate_from_id(PCLK_CSIHOST1_GATE);
            rk_ispdev->csi2_clk.csi2_rxbyteclkhs0 = get_clk_gate_from_id(CLK_RXBYTECLKHS_1_GATE);
        }
        else
        {
            rk_ispdev->vicap_clk.vicap_i0clk = get_clk_gate_from_id(I0CLK_VICAP_GATE);
            rk_ispdev->vicap_clk.vicap_rx0pclk = get_clk_gate_from_id(RX0PCLK_VICAP_GATE);
            rk_ispdev->csi2_clk.csi2_pclk = get_clk_gate_from_id(PCLK_CSIHOST0_GATE);
            rk_ispdev->csi2_clk.csi2_rxbyteclkhs0 = get_clk_gate_from_id(CLK_RXBYTECLKHS_0_GATE);
        }
    }

    rk_ispdev->dphy_clk.dphy_pclk = get_clk_gate_from_id(PCLK_MIPICSIPHY_GATE);

    ret = rk_isp_register(rk_ispdev, rk_ispdev->name);

    rk_isp_function_exit();
    return ret;
}

INIT_DEVICE_EXPORT(rk_device_isp_init);

#endif

