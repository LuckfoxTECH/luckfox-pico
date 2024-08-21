/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_vicap.c
  * @version V0.0.1
  * @brief   video capture unit driver for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup VICAP
 *  @{
 */

/** @defgroup VICAP_How_To_Use How To Use
 *  @{

 The VICAP driver is used to sample image data transferred by bt601/bt656.In order
 to let the driver takes effect,there are there tasks need to be completed as belowed:

- (A) Implement the image sensor driver based on camera framework which has been
      implemented in camera.c.
- (B) Assign the image sensor's device name to macro definition "RK_VICAP_SUBDEV_NAME"
      which is in drv_vicap.h.The vicap driver distinguish sensor with this unique value.
- (C) Stream on the vicap to capture image, the sequence belowed must be observed:
    - 1) Find out the vicap device by device name:
         vicapdev = rk_device_find(name);
    - 2) Open the found vicap device in step1 to init device:
         ret = rk_device_open(vicapdev, RT_DEVICE_OFLAG_RDWR);
    - 3) Set the work mode of vicap device after opening device:
         ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_SET_WORKMODE, &workmode);
    - 4) Set the format for outputing:
         ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_SET_FMT, &format);
    - 5) Set the crop information if it is required:
         ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_CROP_IMAGE, &crop);
    - 6) Set the block num if the vicap's block mode is required:
         ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_SET_BLOCK_NUM, &num);
    - 7) Set the buf num required by application to capture image:
         ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_REQBUF, &reqbuf);
    - 8) Query the buffers have been allocated in step 7):
         ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_QUERYBUF, &buf);
    - 9) Queue all the buffers have been checked in step 8 into vicap drivers:
         ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_QBUF, &buf);
    - 10) Stream on the vicap device to capture image:
          ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_STREAM_ON, RK_NULL);
    - 11) Dqueue the buf filled with image data from driver for processing in application:
          ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_DQBUF, &buf);
    - 12) Queue the buf has been processed in application into driver:
          ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_QBUF, &buf);
    - 13) Loop step11 and step 12 untill application stops streaming;
    - 14) Stop streaming:
          ret = rk_device_control(vicapdev, RK_DEVICE_CTRL_VICAP_STREAM_OFF, RK_NULL);
    - 15) Close device
          ret = rk_device_close(vicapdev);

 @} */

#if defined(__RT_THREAD__)
#include "drv_vicap.h"
#elif defined(__RK_OS__)
#include "driver/drv_vicap.h"
#endif

#ifdef VICAP_MODULE_COMPILED

/********************* Private MACRO Definition ******************************/
/** @defgroup VICAP_Private_Macro Private Macro
 *  @{
 */

#define VICAP_DEBUG             0
#define VICAP_DEBUG_REGISTER    0
#if VICAP_DEBUG
#include <stdio.h>
#define VICAP_DBG(...)                \
            do { \
                rk_kprintf("[VICAP]:");    \
                rk_kprintf(__VA_ARGS__);    \
                }while(0)
#define rk_vicap_function_enter()           rk_kprintf("[VICAP]:(%s) enter \n",__FUNCTION__)
#define rk_vicap_function_exit()            rk_kprintf("[VICAP]:(%s) exit \n",__FUNCTION__)
#else
#define VICAP_DBG(...)
#define rk_vicap_function_enter()
#define rk_vicap_function_exit()
#endif

#if VICAP_DEBUG_REGISTER
#define VICAP_SHOW_REG                      rk_kprintf
#else
#define VICAP_SHOW_REG
#endif

#define VICAP_INFO(dev,arg...)              rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define ARRAY_SIZE(x)                       (sizeof(x) / sizeof((x)[0]))
#define ENABLE  true
#define DISABLE false
/*************************************************************************/

#define VICAP_WORKQUEUE_STACK_SIZE                      (512)
#define VICAP_WORKQUEUE_PRIORITY                        (12)

#define VICAP_INT_ERR_MASK                              (VICAP_DVP_INTSTAT_BLOCK_ERR_MASK |\
                                                         VICAP_DVP_INTSTAT_BUS_ERR_MASK |\
                                                         VICAP_DVP_INTSTAT_PIX_ERR_MASK |\
                                                         VICAP_DVP_INTSTAT_LINE_ERR_MASK)

#define VICAP_INT_OVERFLOW_MASK                         (VICAP_DVP_INTSTAT_DFIFO_OF_MASK |\
                                                         VICAP_DVP_INTSTAT_IFIFO_OF_MASK)

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


/** @} */  // VICAP_Private_Macro

static struct rk_vicap_dev vicap_instance;

/**
 *@brief  cache the output parameters of vicap to match input image format.
 */
static const struct vicap_output_fmt g_output_fmts[] =
{
    {
        .fourcc = V4L2_PIX_FMT_NV16,
        .cplanes = 2,
        .mplanes = 1,
        .format = VICAP_OUTPUT_YUV_422 | VICAP_UV_STORED_ORDER_UVUV,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_NV61,
        .format = VICAP_OUTPUT_YUV_422 | VICAP_UV_STORED_ORDER_VUVU,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_NV12,
        .format = VICAP_OUTPUT_YUV_420 | VICAP_UV_STORED_ORDER_UVUV,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_NV21,
        .format = VICAP_OUTPUT_YUV_420 | VICAP_UV_STORED_ORDER_VUVU,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_YUYV,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_YVYU,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_UYVY,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_VYUY,
        .cplanes = 2,
        .mplanes = 1,
        .bpp = { 8, 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_RGB24,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 24 },
    }, {
        .fourcc = V4L2_PIX_FMT_RGB565,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_BGR666,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 18 },
    }, {
        .fourcc = V4L2_PIX_FMT_SRGGB8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
    }, {
        .fourcc = V4L2_PIX_FMT_SGRBG8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
    }, {
        .fourcc = V4L2_PIX_FMT_SGBRG8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR8,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
    }, {
        .fourcc = V4L2_PIX_FMT_SRGGB10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SGRBG10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SGBRG10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR10,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SRGGB12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SGRBG12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SGBRG12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR12,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_SBGGR16,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_Y16,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 16 },
    }, {
        .fourcc = V4L2_PIX_FMT_GREY,
        .cplanes = 1,
        .mplanes = 1,
        .bpp = { 8 },
    }
};

/**
 *@brief  cache the input parameters of vicap to match input image format.
 */
static const struct vicap_input_fmt g_input_fmts[] =
{
    {
        .mbus_code = MEDIA_BUS_FMT_YUYV8_2X8,
        .data.yuv_input_order = VICAP_YUV_IN_ORDER_YUYV,
        .fmt_type = VICAP_DEVICE_FORMAT_YUV,
    },
    {
        .mbus_code = MEDIA_BUS_FMT_YVYU8_2X8,
        .data.yuv_input_order = VICAP_YUV_IN_ORDER_YVYU,
        .fmt_type = VICAP_DEVICE_FORMAT_YUV,
    },
    {
        .mbus_code = MEDIA_BUS_FMT_UYVY8_2X8,
        .data.yuv_input_order = VICAP_YUV_IN_ORDER_UYVY,
        .fmt_type   = VICAP_DEVICE_FORMAT_YUV,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_VYUY8_2X8,
        .data.yuv_input_order = VICAP_YUV_IN_ORDER_VYUY,
        .fmt_type   = VICAP_DEVICE_FORMAT_YUV,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SBGGR8_1X8,
        .data.raw_input_width = VICAP_RAW_WIDTH_8BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGBRG8_1X8,
        .data.raw_input_width = VICAP_RAW_WIDTH_8BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGRBG8_1X8,
        .data.raw_input_width = VICAP_RAW_WIDTH_8BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SRGGB8_1X8,
        .data.raw_input_width = VICAP_RAW_WIDTH_8BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SBGGR10_1X10,
        .data.raw_input_width = VICAP_RAW_WIDTH_10BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGBRG10_1X10,
        .data.raw_input_width = VICAP_RAW_WIDTH_10BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGRBG10_1X10,
        .data.raw_input_width = VICAP_RAW_WIDTH_10BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SRGGB10_1X10,
        .data.raw_input_width = VICAP_RAW_WIDTH_10BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SBGGR12_1X12,
        .data.raw_input_width = VICAP_RAW_WIDTH_12BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGBRG12_1X12,
        .data.raw_input_width = VICAP_RAW_WIDTH_12BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SGRBG12_1X12,
        .data.raw_input_width = VICAP_RAW_WIDTH_12BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_SRGGB12_1X12,
        .data.raw_input_width = VICAP_RAW_WIDTH_12BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_RGB888_1X24,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_Y8_1X8,
        .data.raw_input_width = VICAP_RAW_WIDTH_8BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_Y10_1X10,
        .data.raw_input_width = VICAP_RAW_WIDTH_10BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    },
    {
        .mbus_code  = MEDIA_BUS_FMT_Y12_1X12,
        .data.raw_input_width = VICAP_RAW_WIDTH_12BIT,
        .fmt_type   = VICAP_DEVICE_FORMAT_RAW,
    }
};

/********************* Private Function Definition ***************************/
/** @defgroup VICAP_Public_Functions Public Functions
 *  @{
 */

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
        return RET_SYS_EINVAL;
    }

    return 0;
}

/**
 * @brief  Judge whether set Y address only or not based on output format.
 * @param fmt : The output format set by application.
 * @return : When the format type is raw or grey,it will return true.
 */
static bool rk_vicap_judge_set_YAddress_only(const struct vicap_output_fmt *fmt)
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
static uint32_t rk_vicap_align_bits_per_pixel(const struct vicap_output_fmt *fmt,
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
            bpp = max(fmt->bpp[plane_index], VICAP_RAW_STORED_BIT_WIDTH);
            bpp = roundup(bpp, VICAP_RAW_STORED_BIT_WIDTH);
            break;
        default:
            rk_kprintf("rk_vicap:fourcc: %d is not supported!\n", fmt->fourcc);
            break;
        }
    }

    return bpp;
}

/**
 * @brief  In raw or jpeg mode, data is stored by 16-bits,
           so need to align virtual line width.
 */
static uint32_t rk_vicap_cal_raw_vir_line_ratio(const struct vicap_output_fmt *fmt)
{
    uint32_t ratio = 0, bpp = 0;

    if (fmt)
    {
        bpp = rk_vicap_align_bits_per_pixel(fmt, 0);
        ratio = bpp / VICAP_YUV_STORED_BIT_WIDTH;
    }
    rk_kprintf("ratio:%d,bpp:%d\n", ratio, bpp);
    return ratio;
}

#if VICAP_DEBUG_REGISTER

void rk_vicap_dump_reg(struct rk_vicap_dev *dev)
{
    struct VICAP_REG *reg;

    reg = dev->vicap_reg;
    VICAP_SHOW_REG("*********DUMP VICAP REG START**********\n");

    VICAP_SHOW_REG("VICAP_DVP_CTRL[0x%x]:0x%lx\n",
                   &reg->DVP_CTRL, READ_REG(reg->DVP_CTRL));
    VICAP_SHOW_REG("VICAP_DVP_INTEN[0x%x]:0x%lx\n",
                   &reg->DVP_INTEN, READ_REG(reg->DVP_INTEN));
    VICAP_SHOW_REG("VICAP_DVP_FOR[0x%x]:0x%lx\n",
                   &reg->DVP_FOR, READ_REG(reg->DVP_FOR));

    VICAP_SHOW_REG("VICAP_DVP_FRAME0_ADDR_Y[0x%x]:0x%lx\n",
                   &reg->DVP_FRM0_ADDR_Y, READ_REG(reg->DVP_FRM0_ADDR_Y));
    VICAP_SHOW_REG("VICAP_DVP_FRAME0_ADDR_UV[0x%x]:0x%lx\n",
                   &reg->DVP_FRM0_ADDR_UV, READ_REG(reg->DVP_FRM0_ADDR_UV));
    VICAP_SHOW_REG("VICAP_DVP_FRAME1_ADDR_Y[0x%x]:0x%lx\n",
                   &reg->DVP_FRM1_ADDR_Y, READ_REG(reg->DVP_FRM1_ADDR_Y));
    VICAP_SHOW_REG("VICAP_DVP_FRAME1_ADDR_UV[0x%x]:0x%lx\n",
                   &reg->DVP_FRM1_ADDR_UV, READ_REG(reg->DVP_FRM1_ADDR_UV));
    VICAP_SHOW_REG("VICAP_DVP_FRAME_STATUS[0x%x]:0x%lx\n",
                   &reg->DVP_FRAME_STATUS, READ_REG(reg->DVP_FRAME_STATUS));

    VICAP_SHOW_REG("VICAP_DVP_BLOCK0_ADDR_Y[0x%x]:0x%lx\n",
                   &reg->DVP_BLOCK0_ADDR_Y, READ_REG(reg->DVP_BLOCK0_ADDR_Y));
    VICAP_SHOW_REG("VICAP_DVP_BLOCK0_ADDR_UV[0x%x]:0x%lx\n",
                   &reg->DVP_BLOCK0_ADDR_UV, READ_REG(reg->DVP_BLOCK0_ADDR_UV));
    VICAP_SHOW_REG("VICAP_DVP_BLOCK1_ADDR_Y[0x%x]:0x%lx\n",
                   &reg->DVP_BLOCK1_ADDR_Y, READ_REG(reg->DVP_BLOCK1_ADDR_Y));
    VICAP_SHOW_REG("VICAP_DVP_BLOCK1_ADDR_UV[0x%x]:0x%lx\n",
                   &reg->DVP_BLOCK1_ADDR_UV, READ_REG(reg->DVP_BLOCK1_ADDR_UV));
    VICAP_SHOW_REG("VICAP_DVP_BLOCK_STATUS[0x%x]:0x%lx\n",
                   &reg->DVP_BLOCK_STATUS, READ_REG(reg->DVP_BLOCK_STATUS));
    VICAP_SHOW_REG("VICAP_DVP_CROP[0x%x]:0x%lx\n",
                   &reg->DVP_CROP, READ_REG(reg->DVP_CROP));
    VICAP_SHOW_REG("VICAP_DVP_PATH_SEL[0x%x]:0x%lx\n",
                   &reg->DVP_PATH_SEL, READ_REG(reg->DVP_PATH_SEL));

    VICAP_SHOW_REG("VICAP_DVP_VIR_LINE_WIDTH[0x%x]:0x%lx\n",
                   &reg->DVP_VIR_LINE_WIDTH, READ_REG(reg->DVP_VIR_LINE_WIDTH));
    VICAP_SHOW_REG("VICAP_DVP_SET_SIZE[0x%x]:0x%lx\n",
                   &reg->DVP_SET_SIZE, READ_REG(reg->DVP_SET_SIZE));
    VICAP_SHOW_REG("VICAP_DVP_BLOCK_LINE_NUM[0x%x]:0x%lx\n",
                   &reg->DVP_BLOCK_LINE_NUM, READ_REG(reg->DVP_BLOCK_LINE_NUM));
    VICAP_SHOW_REG("VICAP_DVP_LINE_INT_NUM[0x%x]:0x%lx\n",
                   &reg->DVP_LINE_INT_NUM, READ_REG(reg->DVP_LINE_INT_NUM));
    VICAP_SHOW_REG("VICAP_DVP_WATER_LINE[0x%x]:0x%lx\n",
                   &reg->DVP_WATER_LINE, READ_REG(reg->DVP_WATER_LINE));
    VICAP_SHOW_REG("VICAP_DVP_CUR_DST[0x%x]:0x%lx\n",
                   &reg->DVP_CUR_DST, READ_REG(reg->DVP_CUR_DST));

    VICAP_SHOW_REG("*********DUMP VICAP REG END**********\n");
}

#endif

/**
 * @brief  Enqueue a vicap_video_buf in driver for processing
 * @param buf : The buf is to enqueued into vicap driver.
 */
static void __enqueue_in_driver(struct vicap_video_buf *buf)
{
    struct vicap_videobuf_queue *queue = buf->buf_q;
    dt_base_t level;

    level = rk_hw_interrupt_disable();
    buf->state = VICAP_BUF_STATE_ACTIVE;
    queue->owned_by_drv_count++;
    rk_list_insert_before(&queue->actived_list, &buf->actived_entry);
    rk_hw_interrupt_enable(level);
}

/**
 * @brief  get vicap device instance.
 * @param dev : The vicap abstract device object in vicap layer.
 */
static inline struct rk_vicap_dev *rk_get_vicap_dev(struct rk_vicap_device *dev)
{
#if defined(__RT_THREAD__)
    struct rk_vicap_dev *vicapdev = dev->parent.user_data;
#elif defined(__RK_OS__)
    struct rk_vicap_dev *vicapdev = dev->parent.priv_data;
#endif

    return vicapdev;
}

/**
 * @brief  change the buf state for processing.
 * @param buf : The buf in videobuf queue to be changed state.
 * @param state : The state to be changed as.
 */
static void rk_vicap_videobuf_done(struct vicap_video_buf *buf, eVICAP_Bufstat state)
{
    dt_base_t level;

    struct vicap_videobuf_queue *queue = buf->buf_q;
    struct rk_vicap_dev *vicapdev = rk_container_of(queue, struct rk_vicap_dev, buf_queue);

    MACRO_ASSERT(buf != RK_NULL);

    if (buf->state != VICAP_BUF_STATE_ACTIVE)
    {
        VICAP_INFO(vicapdev, "passed buf[%d] state is:0x%x not in VICAP_BUF_STATE_ACTIVE\n",
                   buf->index, buf->state);
        return;
    }

    if (state != VICAP_BUF_STATE_DONE &&
            state != VICAP_BUF_STATE_ERROR &&
            state != VICAP_BUF_STATE_QUEUED &&
            state != VICAP_BUF_STATE_REQUEUEING
       )
        state = VICAP_BUF_STATE_ERROR;

    if (state == VICAP_BUF_STATE_QUEUED ||
            state == VICAP_BUF_STATE_REQUEUEING)
    {
        buf->state = VICAP_BUF_STATE_QUEUED;
    }
    else
    {
        level = rk_hw_interrupt_disable();

        rk_list_insert_before(&queue->done_list, &buf->done_entry);
        buf->state = state;
        queue->owned_by_drv_count--;

        rk_hw_interrupt_enable(level);

        rk_semaphore_release(queue->qsem);
    }

    switch (state)
    {
    case VICAP_BUF_STATE_REQUEUEING:
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
 * @param dev : The vicap device.
 */
static struct vicap_video_buf *rk_vicap_find_valid_buf(struct rk_vicap_dev *dev)
{
    dt_base_t level;
    struct vicap_video_buf *buf;
    struct vicap_videobuf_queue *queue;

    MACRO_ASSERT(dev != RK_NULL);

    queue = &dev->buf_queue;
    if (queue->queued_count < 1)
    {
        VICAP_INFO(dev, "Warning: there is no buf queued in video buf queue!\n");
        return RK_NULL;
    }

    if (queue->owned_by_drv_count < 1)
    {
        VICAP_INFO(dev, "Warning: there is no buf enqueued in driver,drv_count:%d!\n",
                   queue->owned_by_drv_count);
        return RK_NULL;
    }

    level = rk_hw_interrupt_disable();
    if (!rk_queue_list_isemty(&queue->actived_list))
    {
        buf = rk_list_first_entry(&queue->actived_list,
                                  struct vicap_video_buf,
                                  actived_entry);
        rk_list_remove(&queue->actived_list, &buf->actived_entry);
    }
    else
    {
        rk_hw_interrupt_enable(level);
        return RK_NULL;
    }
    rk_hw_interrupt_enable(level);

    return buf;
}

/**
 * @brief  Initialize the subdev of vicap, commontly it is image sensor.
 * @param dev : The vicap device.
 * @attention : Actually it is completed in init callback of sensor's driver.
 */
static ret_err_t rk_vicap_init_subdev(struct rk_vicap_dev *dev)
{
    ret_err_t ret;
    rk_device *device;
    struct rk_camera_device *camera;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(dev->subdev != RK_NULL);

    camera = dev->subdev;
    device = &camera->parent;
    ret = rk_device_init(device);
    if (ret == RET_SYS_EOK)
    {
        VICAP_INFO(dev, "init subdev succe!\n");
    }
    else
    {
        VICAP_INFO(dev, "init subdev failed!\n");
    }

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  Get the instance of subdev by its device name.
 * @param dev : The vicap device.
 */
static struct rk_camera_device *rk_vicap_ctrl_get_subdev(struct rk_vicap_dev *dev)
{
    rk_device *subdev;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    subdev = rk_find_device(RK_VICAP_SUBDEV_NAME);
    if (subdev == RK_NULL)
    {
        VICAP_INFO(dev, "Err: Can't find vicap subdev:%s\n", RK_VICAP_SUBDEV_NAME);
        return RK_NULL;
    }

    rk_vicap_function_exit();

    return (struct rk_camera_device *)subdev;
}

/**
 * @brief  Set the block num for block work mode.
 * @param dev : The vicap device.
 * @param num : The block num of one frame to be split
 * @attention : the num is best to be divided by the height of one frame.
 */
static ret_err_t rk_vicap_ctrl_set_block_num(struct rk_vicap_dev *dev, uint32_t num)
{
    uint32_t blk_num;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    blk_num = min(num, (uint32_t)VICAP_BLOCK_NUM_MAX);
    blk_num = max(blk_num, (uint32_t)VICAP_BLOCK_NUM_MIN);
    if (blk_num != num)
    {
        VICAP_INFO(dev, "the set block num:%d is not in [%d:%d],set as:%d\n",
                   num, VICAP_BLOCK_NUM_MIN, VICAP_BLOCK_NUM_MAX, blk_num);
    }
    dev->block_info.block_num = blk_num;

    rk_vicap_function_exit();

    return RET_SYS_EOK;

}

/**
 * @brief  Set the work mode of vicap.
 * @param dev : The vicap device.
 * @param mode : The work mode to be set as.
 * @attention : The application must call this function before setting format.
 */
static ret_err_t rk_vicap_ctrl_set_workmode(struct rk_vicap_dev *dev, eVICAP_workMode mode)
{
    rk_vicap_function_enter();

    if (mode != VICAP_WORKMODE_FRAME_ONEFRAME &&
            mode != VICAP_WORKMODE_FRAME_PINGPONG &&
            mode != VICAP_WORKMODE_BLOCK_PINGPONG)
    {
        VICAP_INFO(dev, "Err: the mode parameter is invalid !!!\n");
        return RET_SYS_EINVAL;
    }
    dev->work_mode = mode;

    rk_vicap_function_exit();

    return RET_SYS_EOK;
}

/**
 * @brief  Set the output format required by the application.
 * @param dev : The vicap device.
 * @param format : The format parameter set by the application.
 */
static ret_err_t rk_vicap_ctrl_set_format(struct rk_vicap_dev *dev,
        struct rk_camera_pix_format *format)
{
    uint8_t i, ret = RET_SYS_EOK;
    uint32_t width = 0, height = 0;
    struct vicap_output_info *output_info;
    struct rk_camera_pix_format *output_pix_format;
    struct rk_camera_mbus_framefmt *input_format;

    rk_vicap_function_enter();

    output_info = &dev->output;
    output_pix_format = &output_info->pix_format;
    input_format = &dev->input.cam_info.mbus_fmt;
    rk_memcpy(output_pix_format, format, sizeof(struct rk_camera_pix_format));

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
        VICAP_INFO(dev, "Err: the set format:0x%x is not supported!\n",
                   output_pix_format->pixelformat);
        return RET_SYS_EEMPTY;
    }

    width = clamp(format->width, (uint32_t)VICAP_WIDOW_WIDTH_MIN,
                  min(input_format->width, VICAP_WIDOW_WIDTH_MAX));
    height = clamp(format->height, (uint32_t)VICAP_WIDOW_HEIGHT_MIN,
                   min(input_format->height, VICAP_WIDOW_HEIGHT_MAX));
    if (width != output_pix_format->width ||
            height != output_pix_format->height)
    {
        VICAP_INFO(dev, "Warning: the set resolution(%d, %d) is not in range (%d, %d) - (%d, %d), clamped!",
                   format->width, format->height,
                   VICAP_WIDOW_WIDTH_MIN, VICAP_WIDOW_HEIGHT_MIN,
                   min(input_format->width, (uint32_t)VICAP_WIDOW_WIDTH_MAX),
                   min(input_format->height, (uint32_t)VICAP_WIDOW_HEIGHT_MAX));

        format->width = width;
        output_pix_format->width = width;
        format->height = height;
        output_pix_format->height = height;
    }

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  Set crop window requied by the application.
 * @param dev : The vicap device.
 * @param rect : The crop information set by application.
 */
static ret_err_t rk_vicap_ctrl_set_crop_window(struct rk_vicap_dev *dev,
        struct rk_camera_crop_rect *rect)
{
    uint8_t ret = RET_SYS_EOK;
    int32_t d_val;
    struct vicap_output_info *output_info;
    struct rk_camera_crop_rect *output_crop;
    struct rk_camera_mbus_framefmt *input_format;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(rect != RK_NULL);

    output_info = &dev->output;
    output_crop = &output_info->crop;
    input_format = &dev->input.cam_info.mbus_fmt;

    d_val = (int32_t)input_format->width - (int32_t)rect->x_axis;
    if (d_val < (int32_t)rect->width)
    {
        ret = RET_SYS_EINVAL;
        VICAP_INFO(dev, "Err: width plus x_axis must not be greater than input width!\n");
        goto ERR;
    }

    d_val = (int32_t)input_format->height - (int32_t)rect->y_axis;
    if (d_val < (int32_t)rect->height)
    {
        ret = RET_SYS_EINVAL;
        VICAP_INFO(dev, "Err: height plus y_axis must not be greater than input height!\n");
        goto ERR;
    }

    output_crop->x_axis = rect->x_axis;
    output_crop->y_axis = rect->y_axis;
    output_crop->width = rect->width;
    output_crop->height = rect->height;
    output_info->is_crop = true;

ERR:

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief enable/disable vicap relevant clk
 * @param dev : The vicap device.
 * @param enable : The enable status of vicap clk
 */
static void rk_vicap_hw_set_clk(struct rk_vicap_dev *dev, bool enable)
{
    struct vicap_clock_info *vicap_clk;

    MACRO_ASSERT(dev != RK_NULL);

    vicap_clk = &dev->vicap_clk;
    if (enable)
    {
        if (vicap_clk->vicap_aclk)
        {
            rk_clk_enable(vicap_clk->vicap_aclk);
        }

        if (vicap_clk->vicap_hclk)
        {
            rk_clk_enable(vicap_clk->vicap_hclk);
        }

        if (vicap_clk->vicap_pclk)
        {
            rk_clk_enable(vicap_clk->vicap_pclk);
        }
    }
    else
    {
        if (vicap_clk->vicap_aclk)
        {
            rk_clk_disable(vicap_clk->vicap_aclk);
        }

        if (vicap_clk->vicap_hclk)
        {
            rk_clk_disable(vicap_clk->vicap_hclk);
        }

        if (vicap_clk->vicap_pclk)
        {
            rk_clk_disable(vicap_clk->vicap_pclk);
        }
    }
}

/**
 * @brief  Clear the register of frame status.
 * @param dev : The vicap device.
 * @param mask : The mask bits of frame status.
 */
static inline void rk_vicap_hw_clear_frame_status(struct rk_vicap_dev *dev,
        uint32_t mask)
{
    MACRO_ASSERT(dev != RK_NULL);

    HAL_VICAP_ClearFrameStatus(dev->vicap_reg, mask);
}

/**
 * @brief  Init the yuv address of frame work mode.
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_hw_init_frame_yuv_addr(struct rk_vicap_dev *dev)
{
    uint32_t i, y_addr, uv_addr, width, height, bpp, y_len, num;
    struct vicap_video_buf *buf;
    struct vicap_output_info *output_info;
    const struct vicap_output_fmt *fmt;
    ret_err_t ret = RET_SYS_EOK;

    rk_vicap_function_enter();

    output_info = &dev->output;
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

    bpp = rk_vicap_align_bits_per_pixel(output_info->output_fmt, 0);
    y_len = width * height * bpp / VICAP_YUV_STORED_BIT_WIDTH;

    num = 0;
    if (!dev->buf_addr0)
        num += 1;
    if (!dev->buf_addr1)
        num += 1;

    /* in frame oneframe mode, the address switch is also pingpong,
     * so the frame1 address also needs to be set.
     */
    buf = RK_NULL;
    for (i = 0; i < num; i++)
    {
        buf = rk_vicap_find_valid_buf(dev);
        if (buf)
        {
            y_addr = (uint32_t)(buf->addr);
            if (!rk_vicap_judge_set_YAddress_only(fmt))
            {
                uv_addr = y_addr + y_len;
            }
            else
            {
                uv_addr = y_addr;
            }

            if (!dev->buf_addr0)
            {
                HAL_VICAP_SetFrm0YAddr(dev->vicap_reg, y_addr);
                HAL_VICAP_SetFrm0UvAddr(dev->vicap_reg, uv_addr);
                dev->buf_addr0 = buf;
            }
            else if (!dev->buf_addr1)
            {
                HAL_VICAP_SetFrm1YAddr(dev->vicap_reg, y_addr);
                HAL_VICAP_SetFrm1UvAddr(dev->vicap_reg, uv_addr);
                dev->buf_addr1 = buf;
            }

            buf = RK_NULL;
        }
        else
        {
            VICAP_INFO(dev, "Err: thers is no buf to init frame addr!\n");
            ret = RET_SYS_EEMPTY;
        }
    }

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  Update the  yuv address of frame mode.
 * @param dev : The vicap device.
 * @param frmstat : The status of frame mode.
 */
static struct vicap_video_buf *rk_vicap_hw_update_frm_sub_yuv_addr(struct rk_vicap_dev *dev,
        uint32_t frmstat)
{
    uint32_t y_addr, uv_addr, width, height, bpp, y_len;
    struct vicap_video_buf *cur_buf = RK_NULL;
    struct vicap_video_buf *next_buf = RK_NULL;
    struct vicap_output_info *output_info;
    const struct vicap_output_fmt *fmt;

    output_info = &dev->output;
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

    bpp = rk_vicap_align_bits_per_pixel(output_info->output_fmt, 0);
    y_len = width * height * bpp / VICAP_YUV_STORED_BIT_WIDTH;

    if (frmstat & VICAP_DVP_FRAME_STATUS_F0_STS_MASK)
    {
        cur_buf = dev->buf_addr0;
    }
    else if (frmstat & VICAP_DVP_FRAME_STATUS_F1_STS_MASK)
    {
        cur_buf = dev->buf_addr1;
    }

    if (cur_buf)
    {
        if (cur_buf->state == VICAP_BUF_STATE_ACTIVE)
        {
            rk_hw_cpu_dcache_ops(RK_HW_CACHE_INVALIDATE,
                                 cur_buf->addr,
                                 cur_buf->bufsize);
            cur_buf->timestamp = rk_tick_get();
            next_buf = rk_vicap_find_valid_buf(dev);
            if (next_buf)
            {
                y_addr = (uint32_t)(next_buf->addr);
                if (!rk_vicap_judge_set_YAddress_only(fmt))
                {
                    uv_addr = y_addr + y_len;
                }
                else
                {
                    uv_addr = y_addr;
                }

                if (frmstat & VICAP_DVP_FRAME_STATUS_F0_STS_MASK)
                {
                    dev->buf_addr0 = next_buf;
                    HAL_VICAP_SetFrm0YAddr(dev->vicap_reg, y_addr);
                    HAL_VICAP_SetFrm0UvAddr(dev->vicap_reg, uv_addr);
                }
                else if (frmstat & VICAP_DVP_FRAME_STATUS_F1_STS_MASK)
                {
                    dev->buf_addr1 = next_buf;
                    HAL_VICAP_SetFrm1YAddr(dev->vicap_reg, y_addr);
                    HAL_VICAP_SetFrm1UvAddr(dev->vicap_reg, uv_addr);
                }
            }
            else
            {
                cur_buf = RK_NULL;
            }
        }
    }
    else
    {
        VICAP_INFO(dev, "Warning: the frame yuv addr is null !!!\n");
    }

    return cur_buf;
}

/**
 * @brief  Identify the vicap's work mode to updata frame yuv address.
 * @param dev : The vicap device.
 */
static struct vicap_video_buf *rk_vicap_hw_update_frame_yuv_addr(struct rk_vicap_dev *dev)
{
    uint32_t frmstat;
    uint32_t frmnum;
    struct vicap_video_buf *buf = RK_NULL;

    frmstat = HAL_VICAP_GetFrameStatus(dev->vicap_reg);
    frmnum = ((frmstat & VICAP_DVP_FRAME_STATUS_FRAME_NUM_MASK) >>
              VICAP_DVP_FRAME_STATUS_FRAME_NUM_SHIFT);

    /*frame pingpong:do not need to clear status,
     *frame oneframe:need to clear status
     */
    if (dev->work_mode == VICAP_WORKMODE_FRAME_PINGPONG)
    {
        buf = rk_vicap_hw_update_frm_sub_yuv_addr(dev, frmstat);
    }

    if (dev->work_mode == VICAP_WORKMODE_FRAME_ONEFRAME)
    {
        if (frmstat & VICAP_DVP_FRAME_STATUS_F0_STS_MASK)
        {
            if ((frmnum % 2) != 0)
            {
                buf = rk_vicap_hw_update_frm_sub_yuv_addr(dev,
                        VICAP_DVP_FRAME_STATUS_F0_STS_MASK);
            }
            else
            {
                buf = rk_vicap_hw_update_frm_sub_yuv_addr(dev,
                        VICAP_DVP_FRAME_STATUS_F1_STS_MASK);
            }
            HAL_VICAP_ClearFrameStatus(dev->vicap_reg, frmstat &
                                       (VICAP_DVP_FRAME_STATUS_F0_STS_MASK |
                                        VICAP_DVP_FRAME_STATUS_F1_STS_MASK));
        }
    }

    return buf;
}

/**
 * @brief  Clear the register of block status.
 * @param dev : The vicap device.
 * @param mask : The mask bits of block status.
 */
static inline void rk_vicap_hw_clear_block_status(struct rk_vicap_dev *dev,
        uint32_t mask)
{
    MACRO_ASSERT(dev != RK_NULL);

    HAL_VICAP_ClearBlockStatus(dev->vicap_reg, mask);
}

/**
 * @brief  Init the line num of each block.
 * @param dev : The vicap device.
 * @param num : The line num of per block.
 * @attention : the num is the result of height divided by block num.
 */
static inline void rk_vicap_hw_init_block_line_num(struct rk_vicap_dev *dev,
        uint32_t num)
{
    MACRO_ASSERT(dev != RK_NULL);

    HAL_VICAP_SetBlockLineNum(dev->vicap_reg, num);
}

/**
 * @brief  Init the yuv address of block.
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_hw_init_block_yuv_addr(struct rk_vicap_dev *dev)
{
    uint32_t i, num, y_addr, uv_addr, y_len;
    struct vicap_video_buf *buf;
    const struct vicap_output_fmt *fmt;
    ret_err_t  ret = RET_SYS_EOK;

    rk_vicap_function_enter();

    fmt = dev->output.output_fmt;
    y_len = dev->block_info.Y_len_per_block;

    num = 0;
    if (!dev->buf_addr0)
        num += 1;
    if (!dev->buf_addr1)
        num += 1;

    buf = RK_NULL;
    for (i = 0; i < num; i++)
    {
        buf = rk_vicap_find_valid_buf(dev);
        if (buf)
        {
            y_addr = (uint32_t)(buf->addr);

            if (!rk_vicap_judge_set_YAddress_only(fmt))
            {
                uv_addr = y_addr + y_len;
            }
            else
            {
                uv_addr = y_addr;
            }

            if (!dev->buf_addr0)
            {
                HAL_VICAP_SetBlock0YAddr(dev->vicap_reg, y_addr);
                HAL_VICAP_SetBlock0UvAddr(dev->vicap_reg, uv_addr);
                dev->buf_addr0 = buf;
                VICAP_DBG("init block0 buf[%d],y:0x%x,uv:0x%x\n",
                          buf->index, y_addr, uv_addr);
            }
            else if (!dev->buf_addr1)
            {
                HAL_VICAP_SetBlock1YAddr(dev->vicap_reg, y_addr);
                HAL_VICAP_SetBlock1UvAddr(dev->vicap_reg, uv_addr);
                dev->buf_addr1 = buf;
                VICAP_DBG("init block1 buf[%d],y:0x%x,uv:0x%x\n",
                          buf->index, y_addr, uv_addr);
            }

            buf = RK_NULL;
        }
        else
        {
            VICAP_INFO(dev, "Err: thers is no buf to init block addr!\n");
            ret = RET_SYS_EEMPTY;
        }
    }

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  Update yuv address of block.
 * @param dev : The vicap device.
 */
static struct vicap_video_buf *rk_vicap_hw_update_block_yuv_addr(struct rk_vicap_dev *dev)
{
    uint32_t blkstat, blkid, frmstat, frmid, intstat;
    uint32_t y_addr, uv_addr;
    struct vicap_video_buf *cur_buf = RK_NULL;
    struct vicap_video_buf *next_buf = RK_NULL;
    const struct vicap_output_fmt *fmt = dev->output.output_fmt;
    bool is_update_blk0;

    intstat = HAL_VICAP_GetIrqStatus(dev->vicap_reg);

    frmstat = HAL_VICAP_GetFrameStatus(dev->vicap_reg);
    frmid = frmstat & VICAP_DVP_FRAME_STATUS_FRAME_NUM_MASK;
    frmid >>= VICAP_DVP_FRAME_STATUS_FRAME_NUM_SHIFT;

    blkstat = HAL_VICAP_GetBlockStatus(dev->vicap_reg);
    blkid = blkstat & VICAP_DVP_BLOCK_STATUS_BLK_ID_MASK;
    blkid >>= VICAP_DVP_BLOCK_STATUS_BLK_ID_SHIFT;

    if (blkstat & VICAP_DVP_BLOCK_STATUS_BLK0_STATUS_MASK)
    {
        is_update_blk0 = true;
    }
    else
    {
        is_update_blk0 = false;
    }

    if (is_update_blk0)
    {
        cur_buf = dev->buf_addr0;
    }
    else
    {
        cur_buf = dev->buf_addr1;
    }

    if (cur_buf)
    {
        if (cur_buf->state == VICAP_BUF_STATE_ACTIVE)
        {
            rk_hw_cpu_dcache_ops(RK_HW_CACHE_INVALIDATE,
                                 cur_buf->addr,
                                 cur_buf->bufsize);
            cur_buf->id.block_id = blkid;
            cur_buf->timestamp = rk_tick_get();

            /* at the last block, frame num will be add 1,
             * so it needs to decrease for consistency
             */
            if (intstat & VICAP_DVP_INTSTAT_DMA_FRAME_END_MASK)
            {
                cur_buf->id.frame_id = frmid - 1;
            }
            else
            {
                cur_buf->id.frame_id = frmid;
            }

            next_buf = rk_vicap_find_valid_buf(dev);
            if (next_buf)
            {
                y_addr = (uint32_t)(next_buf->addr);
                if (!rk_vicap_judge_set_YAddress_only(fmt))
                {
                    uv_addr = y_addr + dev->block_info.Y_len_per_block;
                }
                else
                {
                    uv_addr = y_addr;
                }

                if (is_update_blk0)
                {
                    dev->buf_addr0 = next_buf;
                    HAL_VICAP_SetBlock0YAddr(dev->vicap_reg, y_addr);
                    HAL_VICAP_SetBlock0UvAddr(dev->vicap_reg, uv_addr);
                }
                else
                {
                    dev->buf_addr1 = next_buf;
                    HAL_VICAP_SetBlock1YAddr(dev->vicap_reg, y_addr);
                    HAL_VICAP_SetBlock1UvAddr(dev->vicap_reg, uv_addr);
                }
            }
            else
            {
                cur_buf = RK_NULL;
            }
        }
    }
    else
    {
        VICAP_INFO(dev, "Warning: the block%d yuv addr is null !!!\n", is_update_blk0 ? 0 : 1);
    }

    HAL_VICAP_ClearBlockStatus(dev->vicap_reg, blkstat);

    return cur_buf;
}

/**
 * @brief  Init the yuv address according vicap work mode.
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_hw_init_yuv_addr(struct rk_vicap_dev *dev)
{
    ret_err_t ret;

    rk_vicap_function_enter();

    if (dev->work_mode == VICAP_WORKMODE_BLOCK_PINGPONG)
    {
        VICAP_DBG("(%s) init block pingpong yuv addr\n", __FUNCTION__);

        if (dev->block_info.block_num < VICAP_BLOCK_NUM_MIN)
        {
            ret = RET_SYS_EINVAL;
            VICAP_INFO(dev, "Err:the block num is invalid!\n");
            goto ERR;
        }
        ret = rk_vicap_hw_init_block_yuv_addr(dev);
        rk_vicap_hw_init_block_line_num(dev, dev->block_info.lines_per_block);
        rk_vicap_hw_clear_block_status(dev,
                                       VICAP_DVP_BLOCK_STATUS_BLK0_STATUS_MASK |
                                       VICAP_DVP_BLOCK_STATUS_BLK1_STATUS_MASK);
    }
    else
    {
        VICAP_DBG("(%s) init frame yuv addr\n", __FUNCTION__);

        ret = rk_vicap_hw_init_frame_yuv_addr(dev);
        rk_vicap_hw_clear_frame_status(dev,
                                       VICAP_DVP_FRAME_STATUS_F0_STS_MASK |
                                       VICAP_DVP_FRAME_STATUS_F1_STS_MASK);
    }
ERR:
    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  Set the crop parameters into related registers.
 * @param dev : The vicap device.
 */
static inline void rk_vicap_hw_set_cap_window(struct rk_vicap_dev *dev)
{
    struct vicap_output_info *vicap_output;
    struct vicap_input_fmt *vicap_input_fmt;
    const struct vicap_output_fmt *vicap_output_fmt;
    uint32_t ratio, width, height, x_axis, y_axis, v_width;

    rk_vicap_function_enter();

    vicap_output = &dev->output;
    vicap_output_fmt = vicap_output->output_fmt;
    vicap_input_fmt = &dev->input.input_fmt;

    if (vicap_output->is_crop)
    {
        y_axis = vicap_output->crop.y_axis;
        x_axis = vicap_output->crop.x_axis;
        width = vicap_output->crop.width;
        height = vicap_output->crop.height;
    }
    else
    {
        y_axis = 0;
        x_axis = 0;
        width = vicap_output->pix_format.width;
        height = vicap_output->pix_format.height;
    }

    if (vicap_input_fmt->fmt_type == VICAP_DEVICE_FORMAT_RAW)
    {
        ratio = rk_vicap_cal_raw_vir_line_ratio(vicap_output_fmt);
        v_width = width * ratio;
        VICAP_DBG("v_width1:0x%x,width:0x%x,ratio:%d\n", v_width, width, ratio);
    }
    else
    {
        v_width = width;
        ratio = 1;
        VICAP_DBG("v_width2:0x%x,width:0x%x,ratio:%d\n", v_width, width, ratio);
    }

    HAL_VICAP_SetVirtualLineWidth(dev->vicap_reg, v_width);
    HAL_VICAP_SetCropOrdinate(dev->vicap_reg, y_axis, x_axis);
    HAL_VICAP_SetReceivedSize(dev->vicap_reg, height, width);

    rk_vicap_function_exit();
}

/**
 * @brief  Set the parameter of axiburst type .
 * @param dev : The vicap device.
 */
static inline void rk_vicap_hw_set_axibursttype(struct rk_vicap_dev *dev,
        eVICAP_axiBurstType type)
{
    rk_vicap_function_enter();

    HAL_VICAP_SetAxiBurstType(dev->vicap_reg, type);

    rk_vicap_function_exit();

}

/**
 * @brief  Set the enable bit of vicap ctrl register.
 * @param dev : The vicap device.
 * @param enable : The status of enable bit.
 */
static inline void rk_vicap_hw_set_cap_enable(struct rk_vicap_dev *dev, bool enable)
{
    uint32_t stat;

    VICAP_DBG("(%s): %s \n", __FUNCTION__, enable ? "true" : "false");

    if (enable)
    {
        HAL_VICAP_SetCaptureEnable(dev->vicap_reg, enable);
    }
    else
    {
        /* wirte 1 to stop dma */
        HAL_VICAP_SetDMAStop(dev->vicap_reg, 0x01);
        do
        {
            stat = HAL_VICAP_GetDMAIdleStatus(dev->vicap_reg);
            VICAP_INFO(dev, "dma status: %d\n", stat);
        }
        while (stat & VICAP_DVP_DMA_IDLE_REQ_DMA_IDLE_REQ_MASK);

        /* when disable vicap, cru must be reset after dma stop,
         * otherwise block id will be err in next enable.
         * normally, it is necessary to delay 10-20 us for resetting vicap cru
         */
#if defined(RKMCU_RK2108)
        HAL_DelayUs(20);
        HAL_CRU_ClkResetAssert(SRST_A_CIF);
        HAL_CRU_ClkResetAssert(SRST_H_CIF);
        HAL_CRU_ClkResetAssert(SRST_P_CIF);
        HAL_DelayUs(20);
        HAL_CRU_ClkResetDeassert(SRST_A_CIF);
        HAL_CRU_ClkResetDeassert(SRST_H_CIF);
        HAL_CRU_ClkResetDeassert(SRST_P_CIF);
#elif defined(CONFIG_MCU_RK2206)
        HAL_DelayUs(20);
        HAL_CRU_ClkResetAssert(SRST_A_VIP);
        HAL_CRU_ClkResetAssert(SRST_H_VIP);
        HAL_CRU_ClkResetAssert(SRST_VIP);
        HAL_DelayUs(20);
        HAL_CRU_ClkResetDeassert(SRST_A_VIP);
        HAL_CRU_ClkResetDeassert(SRST_H_VIP);
        HAL_CRU_ClkResetDeassert(SRST_VIP);
#endif
    }

    VICAP_DBG("(%s):%s \n", __FUNCTION__, enable ? "true" : "false");
}

/**
 * @brief  Set line num of line0 to trigger interrupt.
 * @param dev : The vicap device.
 * @param num : Line num of line0 to trigger interrupt.
 */
static inline void rk_vicap_hw_set_line0_int_num(struct rk_vicap_dev *dev,
        uint32_t num)
{
    rk_vicap_function_enter();

    HAL_VICAP_SetLine0IntNum(dev->vicap_reg, num);

    rk_vicap_function_enter();
}

/**
 * @brief  Set line num of line1 to trigger interrupt.
 * @param dev : The vicap device.
 * @param num : Line num of line1 to trigger interrupt.
 */
static inline void rk_vicap_hw_set_line1_int_num(struct rk_vicap_dev *dev,
        uint32_t num)
{
    rk_vicap_function_enter();

    HAL_VICAP_SetLine1IntNum(dev->vicap_reg, num);

    rk_vicap_function_exit();
}

/**
 * @brief  Set the interrupt bits to trigger interrupt.
 * @param dev : The vicap device.
 * @param enable : The state of vicap interrupt.
 */
static void rk_vicap_hw_set_irq(struct rk_vicap_dev *dev, bool enable)
{
    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    if (enable)
    {
        HAL_VICAP_SetIrqEnable(dev->vicap_reg);
    }
    else
    {
        HAL_VICAP_SetIrqDisable(dev->vicap_reg);
        HAL_VICAP_ClearIrqStatus(dev->vicap_reg, 0x0);
    }

    rk_vicap_function_exit();
}

/**
 * @brief  Set the work mode bit of vicap ctrl register.
 * param dev : The vicap device.
 */
rk_inline void rk_vicap_hw_set_workmode(struct rk_vicap_dev *dev)
{
    rk_vicap_function_enter();

    HAL_VICAP_SetWorkmode(dev->vicap_reg, dev->work_mode);

    rk_vicap_function_exit();
}

/**
 * @brief  Set the input path bit of vicap path register.
 * param dev : The vicap device.
 */
static void _rk_vicap_hw_set_path_sel_(struct rk_vicap_dev *dev)
{
    struct vicap_input_fmt *fmt = RK_NULL;

    rk_vicap_function_enter();

    fmt = &dev->input.input_fmt;
    if (fmt)
    {
        if (fmt->fmt_type == VICAP_DEVICE_FORMAT_YUV)
        {
            HAL_VICAP_SetPath(dev->vicap_reg, VICAP_PATH_SEL_YUV);
        }
        else
        {
            HAL_VICAP_SetPath(dev->vicap_reg, VICAP_PATH_SEL_RAW);
        }
    }

    rk_vicap_function_exit();

}

/**
 * @brief  Set the yuv order of input data.
 * param dev : The vicap device.
 */
rk_inline void _rk_vicap_hw_set_yuv_inorder_(struct rk_vicap_dev *dev)
{
    struct vicap_input_fmt *fmt = RK_NULL;

    fmt = &dev->input.input_fmt;
    HAL_VICAP_SetYuvInOrder(dev->vicap_reg, fmt->data.yuv_input_order);

}

/**
 * @brief  Set the raw width of input data.
 * param dev : The vicap device.
 */
rk_inline void _rk_vicap_hw_set_raw_width_(struct rk_vicap_dev *dev)
{
    struct vicap_input_fmt *fmt;

    fmt = &dev->input.input_fmt;
    HAL_VICAP_SetRawWidth(dev->vicap_reg, fmt->data.raw_input_width);
}

/**
 * @brief  Set input mode and vsync/href pol:
 * @param dev : The vicap device
 * @attention: rk2108:
 *          bt601:YUV,RAW
 *          bt656:PAL,NTSC
 *          bt1120:not surpported
 */
static void _rk_vicap_hw_set_input_mode_(struct rk_vicap_dev *dev)
{
    eVICAP_hrefPol hpol;
    eVICAP_vsyncPol vpol;
    struct vicap_input_fmt *fmt;
    struct rk_camera_mbus_config *mbus_config;

    MACRO_ASSERT(dev != RK_NULL);

    fmt = &dev->input.input_fmt;
    mbus_config = &dev->input.cam_info.mbus_config;

    if (mbus_config->mbus_type == CAMERA_MBUS_BT601)
    {
        if (fmt->fmt_type == VICAP_DEVICE_FORMAT_YUV)
            HAL_VICAP_SetInputMode(dev->vicap_reg, VICAP_INPUTMODE_YUV);
        else if (fmt->fmt_type == VICAP_DEVICE_FORMAT_RAW)
            HAL_VICAP_SetInputMode(dev->vicap_reg, VICAP_INPUTMODE_RAW);

        if (mbus_config->flags & MEDIA_BUS_FLAGS_HSYNC_MASK)
            hpol = VICAP_HREF_POL_HIGH_ACTIVE;
        else
            hpol = VICAP_HREF_POL_LOW_ACTIVE;

        if (mbus_config->flags & MEDIA_BUS_FLAGS_VSYNC_MASK)
            vpol = VICAP_VSYNC_POL_HIGH_ACTIVE;
        else
            vpol = VICAP_VSYNC_POL_LOW_ACTIVE;

        HAL_VICAP_SetHrefVsynPol(dev->vicap_reg, hpol, vpol);

    }
    else if (mbus_config->mbus_type == CAMERA_MBUS_BT656)
    {
        /* PAL and NTSC is not decided by format type */
        if ((fmt->fmt_type == VICAP_DEVICE_FORMAT_YUV) ||
                (fmt->fmt_type == VICAP_DEVICE_FORMAT_RAW))
            VICAP_INFO(dev, "Warning: the vicap is not surport:bt656:yuv and raw\n");

        if (mbus_config->std_id & MEDIA_STD_ID_PAL_MASK)
            HAL_VICAP_SetInputMode(dev->vicap_reg, VICAP_INPUTMODE_PAL);
        else
            HAL_VICAP_SetInputMode(dev->vicap_reg, VICAP_INPUTMODE_NTSC);

    }
    else if (mbus_config->mbus_type == CAMERA_MBUS_BT1120)
    {
        /*TODO*/
    }
}

/**
 * @brief  Set the format of input data.
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_hw_set_input_format(struct rk_vicap_dev *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct vicap_input_fmt *fmt;

    /*set input format*/
    fmt = &dev->input.input_fmt;

    _rk_vicap_hw_set_path_sel_(dev);
    if (fmt->fmt_type == VICAP_DEVICE_FORMAT_YUV)
    {
        _rk_vicap_hw_set_yuv_inorder_(dev);
    }
    else
    {
        _rk_vicap_hw_set_raw_width_(dev);
        /* TODO: set raw end type */
    }

    _rk_vicap_hw_set_input_mode_(dev);

    return ret;
}

/**
 * @brief  Set the format of output data.
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_hw_set_output_format(struct rk_vicap_dev *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct vicap_output_fmt *fmt;

    MACRO_ASSERT(dev != RK_NULL);

    fmt = dev->output.output_fmt;
    if (fmt)
    {
        /* set receive only y */
        if (fmt->fourcc == V4L2_PIX_FMT_Y16 || fmt->fourcc == V4L2_PIX_FMT_GREY)
            HAL_VICAP_SetYmodeOnly(dev->vicap_reg, true);
        else
            HAL_VICAP_SetYmodeOnly(dev->vicap_reg, false);

        if (fmt->format & VICAP_OUTPUT_422_420_MASK)
            HAL_VICAP_SetOutFormat(dev->vicap_reg, VICAP_OUTPUT_IS_420);
        else
            HAL_VICAP_SetOutFormat(dev->vicap_reg, VICAP_OUTPUT_IS_422);

        if (fmt->format & VICAP_UV_STORED_ORDER_MASK)
            HAL_VICAP_SetUvStoreOrder(dev->vicap_reg, VICAP_UV_STORE_ORDER_VUVU);
        else
            HAL_VICAP_SetUvStoreOrder(dev->vicap_reg, VICAP_UV_STORE_ORDER_UVUV);
    }
    else
    {
        ret = RET_SYS_EEMPTY;
        VICAP_INFO(dev, "Err: the output format is not found !\n");
    }

    return ret;
}

/**
 * @brief  Set the format of input data and output data.
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_hw_set_format(struct rk_vicap_dev *dev)
{
    ret_err_t ret = 0;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    ret = rk_vicap_hw_set_input_format(dev);
    if (ret < 0)
    {
        VICAP_INFO(dev, "Err: set vicap input format failed!\n");
        goto ERR;
    }

    ret = rk_vicap_hw_set_output_format(dev);
    if (ret < 0)
    {
        VICAP_INFO(dev, "Err: set vicap output format failed!\n");
        goto ERR;
    }

ERR:

    rk_vicap_function_enter();
    return ret;
}

/**
 * @brief  Enable or disable subdev of vicap.
 * @param dev : The vicap device.
 * @param enable : The state of subdev.
 */
static ret_err_t rk_vicap_hw_set_subdev_enable(struct rk_vicap_dev *dev, bool enable)
{
    ret_err_t ret;
    struct rk_camera_device *subdev;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);
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
        VICAP_INFO(dev, "Warning: the subdev ops is null, check it!\n");
        ret = RET_SYS_ENOSYS;
    }

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  The stream on process of vicap registers
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_hw_set_stream_on(struct rk_vicap_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    rk_vicap_function_enter();

    /* start to config vicap */
    rk_vicap_hw_set_clk(dev, ENABLE);
    rk_vicap_hw_set_cap_enable(dev, DISABLE);
    rk_vicap_hw_set_axibursttype(dev, VICAP_AXIBURST_TYPE16);
    rk_vicap_hw_set_workmode(dev);
    rk_vicap_hw_init_yuv_addr(dev);
    rk_vicap_hw_set_format(dev);
    rk_vicap_hw_set_cap_window(dev);
    rk_vicap_hw_set_irq(dev, ENABLE);
#if VICAP_DEBUG_REGISTER
    rk_vicap_dump_reg(dev);
#endif

    /* Set is_streamoff to false right before streaming to avoid race,
     * so we can make sure irq handler read the right value
     */
    dev->is_streamoff = false;

    rk_vicap_hw_set_cap_enable(dev, ENABLE);
    rk_vicap_hw_set_subdev_enable(dev, ENABLE);

    rk_vicap_function_exit();

    return RET_SYS_EOK;

}

/**
 * @brief  iomux vicap io.
 */
static void rk_vicap_hw_iomux(void)
{
#if defined(__RK_OS__)
    iomux_config_vicap();
#endif
}

/**
 * @brief  malloc buf for vicap driver
 * @param buf : The buf stores malloced image address
 * @param size : The size of buf to be malloced.
 */
static void rk_vicap_malloc_image_buf(struct vicap_video_buf *buf,
                                      uint32_t size)
{
    if (size > RT_LARGE_MALLOC_THRRESH)
    {
        buf->addr = (uint8_t *)rk_dma_malloc_large(size);
        buf->malloced_by_dma_large = true;
    }
    else
    {
        buf->addr = (uint8_t *)rk_mem_malloc(size);
        buf->malloced_by_dma_large = false;
    }

    if (!buf->addr)
    {
        rk_kprintf("Warning: malloc buf[%d] failed\n", buf->index);
    }

}

/**
 * @brief free buf malloced by vicap driver
 * @param buf : The vicap driver's buf to be freed.
 */
static void rk_vicap_free_image_buf(struct vicap_video_buf *buf)
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
 * @param dev : The vicap device.
 */
static ret_err_t rk_vicap_release_buf(struct rk_vicap_dev *dev)
{
    uint8_t index;
    struct vicap_video_buf *buf;
    struct vicap_videobuf_queue *queue;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    queue = &dev->buf_queue;
    if (queue->owned_by_drv_count)
    {
        for (index = 0; index < queue->num_buffers; index++)
        {
            buf = queue->bufs[index];
            if (buf->state == VICAP_BUF_STATE_ACTIVE)
            {
                VICAP_DBG("release buf\n");
                rk_vicap_videobuf_done(buf, VICAP_BUF_STATE_ERROR);
            }
        }

        if (queue->owned_by_drv_count != 0)
        {
            VICAP_INFO(dev, "Warning: there is still buf enqueued in driver!\n");
        }
    }

    for (index = 0; index < queue->num_buffers; index++)
    {
        buf = queue->bufs[index];
        rk_vicap_free_image_buf(buf);
        rk_mem_free(buf);
        queue->bufs[index] = RK_NULL;
    }

    queue->streaming = 0;
    queue->queued_count = 0;
    queue->done_count = 0;
    queue->owned_by_drv_count = 0;
    queue->num_buffers = 0;
    queue->min_buffers_needed = VICAP_VIDEO_BUF_NUM_MIN;

    rk_queue_list_init(&queue->queued_list);
    rk_queue_list_init(&queue->done_list);
    rk_queue_list_init(&queue->actived_list);

    rk_vicap_function_exit();

    return RET_SYS_EOK;
}

/**
 * @brief  The application request buffers from kernel
 * @param dev : The vicap device.
 * @param req : Contains the num of buffers required by application.
 */
static ret_err_t rk_vicap_ctrl_reqbuf(struct rk_vicap_dev *dev,
                                      struct vicap_video_requestbuffers *req)
{
    uint8_t index, count, num_allocated_buffers, num_buffers,
            num_blocks, planes;
    uint32_t width, height, lines_buf, lines_per_block, size_per_buf,
             x_ratio, y_ratio;
    struct vicap_videobuf_queue *queue = &dev->buf_queue;
    struct vicap_output_info *output_info = &dev->output;
    struct vicap_block_info *block_info = &dev->block_info;
    const struct vicap_output_fmt *vicap_output_format = output_info->output_fmt;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(req != RK_NULL);

    if (dev->work_mode != req->work_mode)
    {
        VICAP_INFO(dev, "Err: work mode is not matched with vicap driver's!\n");
        return RET_SYS_EINVAL;
    }

    if (queue->streaming)
    {
        VICAP_INFO(dev, "Err: the queue is streaming!\n");
        return RET_SYS_EBUSY;
    }

    rk_mutex_take(queue->qmutex_lock, RK_WAITING_FOREVER);
    count = req->count;
    if (queue->num_buffers != 0 || count == 0)
    {
        for (index = 0; index < queue->num_buffers; index++)
        {
            struct vicap_video_buf *buf;

            buf = queue->bufs[index];
            if (!buf)
            {
                continue;
            }
            else
            {
                rk_vicap_free_image_buf(buf);
                rk_mem_free(buf);
                queue->bufs[index] = RK_NULL;
            }
        }

        if (count == 0)
            return RET_SYS_EOK;
    }
    rk_mutex_release(queue->qmutex_lock);

    num_buffers = min(count, (uint8_t)VICAP_VIDEO_BUF_NUM_MAX);
    num_buffers = max(num_buffers, queue->min_buffers_needed);

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

    if (dev->work_mode == VICAP_WORKMODE_BLOCK_PINGPONG)
    {
        num_blocks = block_info->block_num;

        if (num_blocks < VICAP_BLOCK_NUM_MIN)
        {
            VICAP_INFO(dev, "Err: the set block num:%d must not be lower than %d\n ",
                       num_blocks, VICAP_BLOCK_NUM_MIN);
            return RET_SYS_EINVAL;
        }
        lines_per_block = height / num_blocks;
        block_info->lines_per_block = lines_per_block;
        lines_buf = lines_per_block;
    }
    else
    {
        lines_buf = height;
    }

    fcc_xysubs(vicap_output_format->fourcc, &x_ratio, &y_ratio);

    size_per_buf = 0;
    if (vicap_output_format->cplanes)
        planes = vicap_output_format->cplanes;
    else
        planes = vicap_output_format->mplanes;
    for (index = 0; index < planes; index++)
    {
        uint32_t bpp, bpl, size;

        if (index > 0)
        {
            /* round up */
            width = (width + x_ratio - 1) / x_ratio;
            lines_buf = (lines_buf + y_ratio - 1) / y_ratio;
        }
        bpp = rk_vicap_align_bits_per_pixel(vicap_output_format, index);
        bpl = width * bpp / VICAP_YUV_STORED_BIT_WIDTH;
        size = bpl * lines_buf;
        if (index == 0)
        {
            if (dev->work_mode == VICAP_WORKMODE_BLOCK_PINGPONG)
                block_info->Y_len_per_block = size;
            VICAP_INFO(dev, "y len per block:0x%x\n", block_info->Y_len_per_block);
        }

        size_per_buf += size;
        VICAP_INFO(dev, "the plane[%d] size:0x%x, the total size of buf:0x%x\n",
                   index, size, size_per_buf);
    }

    num_allocated_buffers = 0;
    for (index = 0; index < num_buffers; index++)
    {
        struct vicap_video_buf *buf;

        buf = (struct vicap_video_buf *)rk_mem_malloc(sizeof(struct vicap_video_buf));
        if (buf)
        {
            /* the memory address needed to be aligned with 8bytes */
            rk_vicap_malloc_image_buf(buf, size_per_buf);
            if (buf->addr)
            {
                buf->index = index;
                buf->bufsize = size_per_buf;
                buf->buf_q = queue;
                buf->state = VICAP_BUF_STATE_DEQUEUED;
                rk_list_node_init(&buf->done_entry);
                rk_list_node_init(&buf->queued_entry);
                rk_list_node_init(&buf->actived_entry);
                queue->bufs[index] = buf;

                num_allocated_buffers += 1;

                rk_hw_cpu_dcache_ops(RK_HW_CACHE_FLUSH, (void *)buf->addr, size_per_buf);

            }
            else
            {
                rk_mem_free(buf);
                VICAP_INFO(dev, "Err: malloc buf[%d] memory:%d failed\n", index, size_per_buf);
                break;
            }
        }
        else
        {
            VICAP_INFO(dev, "Warning: the memory is not enough to allocate buf[%d]\n",
                       index);
            break;
        }
    }

    if (num_allocated_buffers < VICAP_VIDEO_BUF_NUM_MIN)
    {
        for (index = 0; index < num_allocated_buffers; index++)
        {
            struct vicap_video_buf *buf = RK_NULL;

            buf = queue->bufs[index];
            if (buf)
            {
                rk_vicap_free_image_buf(buf);
                rk_mem_free(buf);
                queue->bufs[index] = RK_NULL;
            }
        }
        VICAP_INFO(dev, "Err: allocated buffer failed,the amount is %d,must not be lower than %d\n",
                   num_allocated_buffers, VICAP_VIDEO_BUF_NUM_MIN);
        return RET_SYS_ENOMEM;
    }

    rk_mutex_take(queue->qmutex_lock, RK_WAITING_FOREVER);
    queue->num_buffers = num_allocated_buffers;
    rk_mutex_release(queue->qmutex_lock);

    req->count = num_allocated_buffers;

    rk_vicap_function_exit();

    return RET_SYS_EOK;
}

/**
 * @brief  The application queries buffers have been required.
 * @param dev : The vicap device.
 * @param buf : The video buf to be queried.
 */
static ret_err_t rk_vicap_ctrl_querybuf(struct rk_vicap_dev *dev, struct vicap_video_buf *buf)
{
    struct vicap_videobuf_queue *queue;
    struct vicap_video_buf *buf_in_q;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(buf != RK_NULL);

    queue = &dev->buf_queue;

    if (buf->index >= queue->num_buffers)
    {
        VICAP_INFO(dev, "Err: buf index out of range!\n");
        return RET_SYS_EINVAL;
    }

    buf_in_q = queue->bufs[buf->index];
    if (buf_in_q)
    {
        buf->index = buf_in_q->index;
        buf->bufsize = buf_in_q->bufsize;
    }
    else
    {
        VICAP_INFO(dev, "Err: the buf[%d] is null!\n");
        return RET_SYS_ENOMEM;
    }

    switch (buf_in_q->state)
    {
    case VICAP_BUF_STATE_QUEUED:
    case VICAP_BUF_STATE_ACTIVE:
        buf->state |= VICAP_BUF_STATE_QUEUED;
        VICAP_DBG("buf_in_q[%d]:active or queued\n", buf_in_q->index);
        break;
    case VICAP_BUF_STATE_ERROR:
        buf->state |= VICAP_BUF_STATE_ERROR;
        VICAP_DBG("buf_in_q[%d]: error\n", buf_in_q->index);
    /* fall through */
    case VICAP_BUF_STATE_DONE:
        buf->state |= VICAP_BUF_STATE_DONE;
        VICAP_DBG("buf_in_q[%d]:done\n", buf_in_q->index);
        break;
    case VICAP_BUF_STATE_PREPARED:
        buf->state |= VICAP_BUF_STATE_PREPARED;
        VICAP_DBG("buf_in_q[%d]:prepared\n", buf_in_q->index);
        break;
    case VICAP_BUF_STATE_PREPARING:
    case VICAP_BUF_STATE_DEQUEUED:
    case VICAP_BUF_STATE_REQUEUEING:
        VICAP_DBG("buf_in_q[%d]:preparing,dequeued,requeueing\n", buf_in_q->index);
        /* nothing */
        break;
    }

    rk_vicap_function_exit();

    return RET_SYS_EOK;
}

/**
 * @brief  The application queues buffer from application to kernel.
 * @param dev : The vicap device.
 * @param buf : The video buf to be queued.
 */
ret_err_t rk_vicap_ctrl_qbuf(struct rk_vicap_dev *dev, struct vicap_video_buf *buf)
{
    dt_base_t level;
    struct vicap_videobuf_queue *queue;
    struct vicap_video_buf *buf_in_q;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(buf != RK_NULL);

    queue = &dev->buf_queue;
    if (buf->index >= queue->num_buffers)
    {
        VICAP_INFO(dev, "Err: buf index out of range !\n");
        return RET_SYS_EINVAL;
    }

    if (queue->bufs[buf->index] == RK_NULL)
    {
        VICAP_INFO(dev, "Err: buf[%d] is NULL\n", buf->index);
        return RET_SYS_EINVAL;
    }

    buf_in_q = queue->bufs[buf->index];

    switch (buf_in_q->state)
    {
    case VICAP_BUF_STATE_DEQUEUED:
    case VICAP_BUF_STATE_PREPARED:
        break;

    case VICAP_BUF_STATE_PREPARING:
        VICAP_INFO(dev, "buf[%d] still being prepared!\n", buf_in_q->index);
        return RET_SYS_EINVAL;
        break;

    default:
        VICAP_INFO(dev, "invalid buffer state %d\n", buf_in_q->state);
        return RET_SYS_EINVAL;
    }

    level = rk_hw_interrupt_disable();
    rk_list_insert_before(&queue->queued_list,
                          &buf_in_q->queued_entry);
    queue->queued_count++;
    buf_in_q->state = VICAP_BUF_STATE_QUEUED;
    rk_hw_interrupt_enable(level);

    if (queue->streaming)
    {
        __enqueue_in_driver(buf_in_q);
    }

    rk_vicap_function_exit();

    return RET_SYS_EOK;
}

/**
 * @brief  The application dqueues buffer from kernel to application.
 * @param dev : The vicap device.
 * @param buf : The video buf to be dqueued.
 */
ret_err_t rk_vicap_ctrl_dqbuf(struct rk_vicap_dev *dev, struct vicap_video_buf *buf)
{
    ret_err_t ret;
    dt_base_t level;
    struct vicap_videobuf_queue *queue = RK_NULL;
    struct vicap_video_buf *dqbuf = RK_NULL;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    queue = &dev->buf_queue;
    if (!queue->streaming)
    {
        VICAP_INFO(dev, "Err: vicap is streamed off, no buffer to be dqueued!\n");

        return RET_SYS_ERROR;
    }

    ret = rk_semaphore_take(queue->qsem, RK_WAITING_FOREVER);
    if (ret != RET_SYS_EOK)
    {
        VICAP_INFO(dev, "Err: take dqbuf sem failed!\n");

        return RET_SYS_ERROR;
    }

    level = rk_hw_interrupt_disable();
    if (!rk_queue_list_isemty(&queue->done_list))
    {
        dqbuf = rk_list_first_entry(&queue->done_list,
                                    struct vicap_video_buf,
                                    done_entry);
        rk_list_remove(&queue->done_list, &dqbuf->done_entry);
        rk_list_remove(&queue->queued_list, &dqbuf->queued_entry);
        dqbuf->state = VICAP_BUF_STATE_DEQUEUED;
        queue->queued_count--;

        rk_memcpy(buf, dqbuf, sizeof(struct vicap_video_buf));
        rk_hw_interrupt_enable(level);
    }
    else
    {
        rk_hw_interrupt_enable(level);
        ret = RET_SYS_ENOMEM;
        VICAP_INFO(dev, "Warning:no buf for dqueuing,%d\n", buf->index);
    }

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  The application streams on vicap.
 * @param dev : The vicap device.
 */
ret_err_t rk_vicap_ctrl_stream_on(struct rk_vicap_dev *dev)
{
    ret_err_t ret;
    struct vicap_video_buf *buf;
    struct vicap_videobuf_queue *queue;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    queue = &dev->buf_queue;
    if (queue->streaming)
    {
        VICAP_INFO(dev, "vicap has been stream on!\n");
        return RET_SYS_EOK;
    }

    if (!queue->num_buffers ||
            queue->num_buffers < queue->min_buffers_needed)
    {
        VICAP_INFO(dev, "Err: the amount of buf is %d, is not enough to stream!\n");
        return RET_SYS_EINVAL;
    }

    if (queue->num_buffers >= queue->min_buffers_needed)
    {
        rk_list_for_each_entry(buf, &queue->queued_list, queued_entry)
        {
            __enqueue_in_driver(buf);
            VICAP_DBG(dev, "enqueue buf[%d]:0x%x:0x%x,state:0x%x,drv_count:%d\n", buf->index,
                      (uint32_t)buf->addr, *(uint32_t *)buf->addr, buf->state, queue->owned_by_drv_count);
        }
        ret = rk_vicap_hw_set_stream_on(dev);
    }

    queue->streaming = 1;

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  The application streams off vicap.
 * @param dev : The vicap device.
 */
ret_err_t rk_vicap_ctrl_stream_off(struct rk_vicap_dev *dev)
{
    ret_err_t ret;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    dev->is_streamoff = true;
    ret = rk_semaphore_take(dev->streamoff_sem, RK_WAITING_FOREVER);
    if (ret == RET_SYS_EOK)
    {
        rk_vicap_hw_set_subdev_enable(dev, DISABLE);
        rk_vicap_hw_set_clk(dev, DISABLE);
        rk_vicap_release_buf(dev);

        dev->buf_addr0 = RK_NULL;
        dev->buf_addr1 = RK_NULL;
    }
    else
    {
        VICAP_INFO(dev, "Err: stream off failed!\n");
    }

    rk_vicap_function_exit();

    return ret;
}

#if VICAP_WORK_QUEUE_ENABLE
/**
 * @brief  The work handler of frame's oneframe mode and pingpong mode
 * @param parameter : The vicap device pointer.
 */
static void rk_vicap_workqueue_frame_handler(void *parameter)
{
    /*TODO:*/
    struct rk_vicap_dev *dev = (struct rk_vicap_dev *)parameter;

    MACRO_ASSERT(dev != RK_NULL);
    rk_vicap_hw_update_frame_yuv_addr(dev);
}

/**
 * @brief  The work handler of block pingpong mode
 * @param parameter : The vicap device pointer.
 * maybe not used.
 */
static void rk_vicap_workqueue_block_handler(void *parameter)
{
    struct vicap_video_buf *buf = (struct vicap_video_buf *)parameter;

    rk_vicap_videobuf_done(buf, VICAP_BUF_STATE_DONE);
}

/**
 * @brief  Init vicap workqueue
 * maybe not used.
 */
ret_err_t rk_vicap_workqueue_init(void)
{
    /*TODO:may be used*/
    return RET_SYS_EOK;
}

/**
 * @brief  The work queue of vicap to handle task of different work mode.
 * @param fun : The handler instance of different work mode.
 * @param parameter : The vicap device pointer.
 * maybe not be used.
 */
static ret_err_t rk_vicap_workqueue_dowork(struct rk_vicap_dev *dev,
        void (*fun)(void *parameter),
        void *parameter)
{
    ret_err_t ret;
    struct rk_vicap_work *vicap_work;
    struct rt_workqueue *workqueue;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);

    workqueue = dev->vicap_workqueue;
    if (workqueue == RK_NULL)
    {
        VICAP_INFO(dev, "%s:(line:%d):vicap workqueue is null\n",
                   __FUNCTION__, __LINE__);
        return RET_SYS_EINVAL;
    }

    vicap_work = &dev->vicap_work;
    if (vicap_work == RK_NULL)
    {
        VICAP_INFO(dev, "%s:(line:%d):vicap workqueue handle fun is null\n",
                   __FUNCTION__, __LINE__);
        return RET_SYS_EINVAL;
    }

    if (fun == RK_NULL)
    {
        VICAP_INFO(dev, "%s:(line:%d):vicap workqueue handle fun is null\n",
                   __FUNCTION__, __LINE__);
        return RET_SYS_EINVAL;
    }

    vicap_work->fun = fun;
    vicap_work->parameter = parameter;
    rt_work_init(&vicap_work->work, rk_vicap_workqueue_fun, vicap_work);
    ret = rt_workqueue_dowork(workqueue, &vicap_work->work);
    if (ret != RET_SYS_EOK)
    {
        VICAP_INFO(dev, "%s:(line:%d):vicap workqueue dowork failed!\n",
                   __FUNCTION__, __LINE__);
    }

    rk_vicap_function_exit();

    return ret;
}
#endif

/**
 * @brief  Handle the interrupt  of frame work mode.
 * @param dev : The vicap device.
 */
static void rk_vicap_sample_frame_irq(struct rk_vicap_dev *dev)
{
    uint32_t intstat;
    struct vicap_video_buf *buf = RK_NULL;

    intstat = HAL_VICAP_GetIrqStatus(dev->vicap_reg);

    if (intstat & VICAP_INT_ERR_MASK)
    {
        VICAP_INFO(dev, "the vicap was triggered err,intstat:0x%x\n", intstat);
    }

    if (intstat & VICAP_INT_OVERFLOW_MASK)
    {
        VICAP_INFO(dev, "the vicap occurs overflow,intstat:0x%x\n", intstat);
    }

    if (intstat & VICAP_DVP_INTSTAT_DMA_FRAME_END_MASK)
    {
        buf = rk_vicap_hw_update_frame_yuv_addr(dev);
        if (buf)
            rk_vicap_videobuf_done(buf, VICAP_BUF_STATE_DONE);
    }

    HAL_VICAP_ClearIrqStatus(dev->vicap_reg, intstat);

    if (intstat & VICAP_DVP_INTSTAT_DMA_FRAME_END_MASK)
    {
        if (dev->is_streamoff)
        {
            rk_vicap_hw_set_irq(dev, DISABLE);
            rk_vicap_hw_set_cap_enable(dev, DISABLE);
            rk_semaphore_release(dev->streamoff_sem);
        }
    }

}

/**
 * @brief  Handle the interrupt of block pingpong work mode.
 * @param dev : The vicap device.
 */
static void rk_vicap_sample_block_irq(struct rk_vicap_dev *dev)
{
    uint32_t intstat;
    struct vicap_video_buf *buf = RK_NULL;

    intstat = HAL_VICAP_GetIrqStatus(dev->vicap_reg);

    if (intstat & VICAP_INT_ERR_MASK)
    {
        VICAP_INFO(dev, "vicap was triggered err,intstat:0x%x\n", intstat);
    }

    if (intstat & VICAP_INT_OVERFLOW_MASK)
    {
        VICAP_INFO(dev, "vicap occurs overflow,intstat:0x%x\n", intstat);
    }

    if (intstat & (VICAP_DVP_INTSTAT_BLOCK0_END_MASK |
                   VICAP_DVP_INTSTAT_BLOCK1_END_MASK))
    {
        if ((intstat & VICAP_DVP_INTSTAT_BLOCK0_END_MASK) &&
                (intstat & VICAP_DVP_INTSTAT_BLOCK1_END_MASK))
        {
            VICAP_INFO(dev, "warning:block0 & block1 dma end"\
                       "occur at the same time !!!\n");
        }

        buf = rk_vicap_hw_update_block_yuv_addr(dev);
        if (buf)
        {
            rk_vicap_videobuf_done(buf, VICAP_BUF_STATE_DONE);
        }
    }

    HAL_VICAP_ClearIrqStatus(dev->vicap_reg, intstat);

    if (intstat & VICAP_DVP_INTSTAT_DMA_FRAME_END_MASK)
    {
        if (dev->is_streamoff)
        {
            rk_vicap_hw_set_irq(dev, DISABLE);
            rk_vicap_hw_set_cap_enable(dev, DISABLE);
            rk_semaphore_release(dev->streamoff_sem);
        }
    }
}

/**
 * @brief  Handle the interrupt of vicap.
 */
static void rk_vicap_irq(void)
{
    struct rk_vicap_dev *vicapdev = &vicap_instance;

    rk_interrupt_enter();

    if (vicapdev->work_mode == VICAP_WORKMODE_BLOCK_PINGPONG)
    {
        rk_vicap_sample_block_irq(vicapdev);
    }
    else
    {
        rk_vicap_sample_frame_irq(vicapdev);
    }

    rk_interrupt_leave();
}

/**
 * @brief  Init the vicap device for application.
 * @param dev : The vicap device.
 */
ret_err_t rk_vicap_init(struct rk_vicap_device *dev)
{
    uint8_t i;
    uint32_t width, height, bpp, bpl, lines_per_block;
    struct rk_vicap_dev *vicapdev = rk_get_vicap_dev(dev);
    struct rk_camera_device *subdev = vicapdev->subdev;
    struct rk_camera_mbus_framefmt *mbus_fmt;
    const struct vicap_input_fmt *fmt;

    MACRO_ASSERT(dev != RK_NULL);

    rk_vicap_function_enter();

    if (!subdev)
    {
        subdev = rk_vicap_ctrl_get_subdev(vicapdev);
        if (!subdev)
        {
            VICAP_INFO(vicapdev, "Can't find vicap subdev, please check it!\n");
            return RK_NULL;
        }
        else
        {
            vicapdev->subdev = subdev;
            VICAP_INFO(vicapdev, "The %s subdev instance is:%s\n",
                       RK_VICAP_SUBDEV_NAME,
                       vicapdev->subdev->name);
            rk_vicap_init_subdev(vicapdev);
        }
    }

    mbus_fmt = &subdev->info.mbus_fmt;
    fmt = RK_NULL;
    VICAP_INFO(vicapdev, "len of input fmts:%d\n", ARRAY_SIZE(g_input_fmts));
    for (i = 0; i < ARRAY_SIZE(g_input_fmts); i++)
    {
        VICAP_INFO(vicapdev, "input pixelcode:0x%x,mbus_code:0x%x\n",
                   mbus_fmt->pixelcode, g_input_fmts[i].mbus_code);

        if (mbus_fmt->pixelcode == g_input_fmts[i].mbus_code)
        {
            fmt = &g_input_fmts[i];
            break;
        }
    }
    if (!fmt)
    {
        VICAP_INFO(vicapdev, "Warning: the input format is not found\n");
    }
    else
    {
        VICAP_INFO(vicapdev, "the input format is:0x%x\n", fmt->mbus_code);
    }

    rk_memcpy(&vicapdev->input.cam_info,
              &subdev->info,
              sizeof(struct rk_camera_info));
    rk_memcpy(&vicapdev->input.input_fmt,
              fmt,
              sizeof(struct vicap_input_fmt));

    width = max((uint32_t)VICAP_WIDOW_WIDTH_MIN, mbus_fmt->width);
    width = min(width, (uint32_t)VICAP_WIDOW_WIDTH_MAX);
    vicapdev->output.pix_format.width = width;

    height = max((uint32_t)VICAP_WIDOW_HEIGHT_MIN, mbus_fmt->height);
    height = min(height, (uint32_t)VICAP_WIDOW_WIDTH_MAX);
    vicapdev->output.pix_format.height = height;
    vicapdev->output.is_crop = false;
    vicapdev->output.output_fmt = &g_output_fmts[0];

    bpp = rk_vicap_align_bits_per_pixel(vicapdev->output.output_fmt, 0);
    bpl = width * bpp / VICAP_YUV_STORED_BIT_WIDTH;
    vicapdev->block_info.block_num = VICAP_BLOCK_NUM_MIN;
    lines_per_block = height / vicapdev->block_info.block_num;
    vicapdev->block_info.lines_per_block = lines_per_block;
    vicapdev->block_info.Y_len_per_block = lines_per_block * bpl;

    if (width != mbus_fmt->width ||
            height != mbus_fmt->height)
    {
        VICAP_INFO(vicapdev, "width:%d,height:%d\n", width, height);
        VICAP_INFO(vicapdev, "Warning: the input resolution:%d x %d is not in range:64 x 64 -- 8192 x 8192 !\n",
                   mbus_fmt->width, mbus_fmt->height);
    }

    rk_vicap_function_exit();

    return RET_SYS_EOK;
}

/**
 * @brief  Open the vicap device for application.
 * @param dev : The vicap device.
 * @param oflag : The flag of open.
 */
ret_err_t rk_vicap_open(struct rk_vicap_device *dev, uint16_t oflag)
{
    struct rk_vicap_dev *vicapdev;
    struct vicap_videobuf_queue *queue;

    vicapdev = rk_get_vicap_dev(dev);
    VICAP_INFO(vicapdev, "vicap dev@:0x%x\n", (uint32_t)vicapdev);

    if (!vicapdev->streamoff_sem)
    {
        vicapdev->streamoff_sem = rk_semaphore_create("offsem", 0, 1);
        if (!vicapdev->streamoff_sem)
        {
            VICAP_INFO(vicapdev, "create stream off semaphore failed!\n");
            return RET_SYS_ENOMEM;
        }
    }

    queue = &vicapdev->buf_queue;
    queue->done_count = 0;
    queue->queued_count = 0;
    queue->streaming = 0;
    queue->num_buffers = 0;
    queue->min_buffers_needed = VICAP_VIDEO_BUF_NUM_MIN;
    rk_queue_list_init(&queue->queued_list);
    rk_queue_list_init(&queue->done_list);
    rk_queue_list_init(&queue->actived_list);
    if (!queue->done_lock)
    {
        queue->done_lock = rk_mutex_create("vicap_donelist_lock",
                                           RK_IPC_FLAG_FIFO);
        if (!queue->done_lock)
        {
            VICAP_INFO(vicapdev, "create queue done mutex failed!\n");
            return RET_SYS_ENOMEM;
        }
    }

    if (!queue->qmutex_lock)
    {
        queue->qmutex_lock = rk_mutex_create("vicap_qmutex_lock",
                                             RK_IPC_FLAG_FIFO);
        if (!queue->qmutex_lock)
        {
            VICAP_INFO(vicapdev, "create queue queued mutex failed!\n");
            return RET_SYS_ENOMEM;
        }
    }

    if (!queue->qsem)
    {
        queue->qsem = rk_semaphore_create("vicap_qsem", 0, 256);
        if (!queue->qsem)
        {
            VICAP_INFO(vicapdev, "create queue semaphore failed!\n");
            return RET_SYS_ENOMEM;
        }
    }

    return RET_SYS_EOK;
}

/**
 * @brief  Close the vicap device for application.
 * @param dev : The vicap device.
 */
ret_err_t rk_vicap_close(struct rk_vicap_device *dev)
{
    struct vicap_videobuf_queue *queue;
    struct rk_vicap_dev *vicapdev;
    rk_device *dev_object;

    rk_vicap_function_enter();

    vicapdev = rk_get_vicap_dev(dev);
    dev_object = &vicapdev->parent.parent;
    if (dev_object->open_flag & RT_DEVICE_OFLAG_OPEN)
    {
        if (vicapdev->streamoff_sem)
        {
            rk_semaphore_delete(vicapdev->streamoff_sem);
            vicapdev->streamoff_sem = RK_NULL;
        }

        queue = &vicapdev->buf_queue;
        if (queue)
        {
            if (queue->qsem)
            {
                rk_semaphore_delete(queue->qsem);
                queue->qsem = RK_NULL;
            }
            if (queue->done_lock)
            {
                rk_mutex_delete(queue->done_lock);
                queue->done_lock = RK_NULL;
            }
            if (queue->qmutex_lock)
            {
                rk_mutex_delete(queue->qmutex_lock);
                queue->qmutex_lock = RK_NULL;
            }
        }
    }
    else
    {
        VICAP_INFO(vicapdev, "Warning: vicap maybe has been closed!\n");
        return RET_SYS_ENOSYS;
    }

    return RET_SYS_EOK;

    rk_vicap_function_exit();
}

/**
 * @brief  Control interface for application to control vicap device.
 * @param dev : The vicap device.
 * @param cmd : The control command to set vicap device.
 * @param args : The parameter of command argument for setting.
 */
ret_err_t rk_vicap_control(struct rk_vicap_device *dev, dt_cmd_t cmd, void *args)
{
    struct rk_vicap_dev *vicap = rk_get_vicap_dev(dev);
    struct vicap_videobuf_queue *queue;

    ret_err_t ret = RET_SYS_EOK;

    rk_vicap_function_enter();

    MACRO_ASSERT(vicap != RK_NULL);

    queue = &vicap->buf_queue;

    if (cmd == RK_DEVICE_CTRL_DEVICE_INIT ||
            cmd == RK_DEVICE_CTRL_VICAP_SET_WORKMODE ||
            cmd == RK_DEVICE_CTRL_VICAP_SET_FMT ||
            cmd == RK_DEVICE_CTRL_VICAP_CROP_IMAGE ||
            cmd == RK_DEVICE_CTRL_VICAP_SET_BLOCK_NUM ||
            cmd == RK_DEVICE_CTRL_VICAP_STREAM_ON)
    {
        if (queue->streaming)
        {
            rk_kprintf("Err: set cmd:%d failed, vicap is streaminig\n", cmd);
            return RET_SYS_EBUSY;
        }
    }

    switch (cmd)
    {
    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_vicap_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_SET_WORKMODE:
    {
        MACRO_ASSERT(args != RK_NULL);

        ret = rk_vicap_ctrl_set_workmode(vicap, *(eVICAP_workMode *)args);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_SET_FMT:
    {
        struct rk_camera_pix_format *format;

        MACRO_ASSERT(args != RK_NULL);

        format = (struct rk_camera_pix_format *)args;
        ret = rk_vicap_ctrl_set_format(vicap, format);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_CROP_IMAGE:
    {
        struct rk_camera_crop_rect *rect;

        MACRO_ASSERT(args != RK_NULL);

        rect = (struct rk_camera_crop_rect *)args;
        ret = rk_vicap_ctrl_set_crop_window(vicap, rect);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_REQBUF:
    {
        struct vicap_video_requestbuffers *req;

        req = (struct vicap_video_requestbuffers *)args;
        ret = rk_vicap_ctrl_reqbuf(vicap, req);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_QUERYBUF:
    {
        struct vicap_video_buf *buf;

        buf = (struct vicap_video_buf *)args;
        ret = rk_vicap_ctrl_querybuf(vicap, buf);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_QBUF:
    {
        MACRO_ASSERT(args != RK_NULL);

        struct vicap_video_buf *buf = (struct vicap_video_buf *)args;
        ret = rk_vicap_ctrl_qbuf(vicap, buf);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_DQBUF:
    {
        MACRO_ASSERT(args != RK_NULL);

        struct vicap_video_buf *buf = (struct vicap_video_buf *)args;
        ret = rk_vicap_ctrl_dqbuf(vicap, buf);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_STREAM_ON:
    {
        ret = rk_vicap_ctrl_stream_on(vicap);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_STREAM_OFF:
    {
        ret = rk_vicap_ctrl_stream_off(vicap);
    }
    break;

    case RK_DEVICE_CTRL_VICAP_GET_SUBDEV:
    {
        struct rk_camera_device *subdev;

        subdev = rk_vicap_ctrl_get_subdev(vicap);
        if (subdev)
            args = subdev;
        else
            ret = RET_SYS_ENOMEM;
    }
    break;

    case RK_DEVICE_CTRL_VICAP_SET_BLOCK_NUM:
    {
        MACRO_ASSERT(args != RK_NULL);

        ret = rk_vicap_ctrl_set_block_num(vicap, *(unsigned int *)args);
    }
    break;

    default:
    {
        ret = RET_SYS_ENOSYS;
    }
    break;

    }

    rk_vicap_function_exit();

    return ret;
}

/**
 * @brief  The indicate interfaces of vicap device.
 */
ret_err_t rk_vicap_rx_indicate(struct rk_vicap_device *dev, ret_size_t size)
{
    /*TODO:maybe not used*/
    return RET_SYS_EOK;
}

/**
 * @brief  The operation interfaces of vicap device for setting.
 */
const static struct rk_vicap_ops rk_vicap_ops =
{
    rk_vicap_init,
    rk_vicap_open,
    rk_vicap_close,
    rk_vicap_control,
    rk_vicap_rx_indicate
};

static ret_err_t rk_vicap_register(struct rk_vicap_dev *dev, char *name)
{
    struct rk_vicap_device *vicap;

    rk_vicap_function_enter();

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(name != RK_NULL);

    vicap = &dev->parent;
    vicap->ops = &rk_vicap_ops;

#if VICAP_WORK_QUEUE_ENABLE
    dev->vicap_workqueue = rt_workqueue_create("vicap_wq",
                           VICAP_WORKQUEUE_STACK_SIZE,
                           VICAP_WORKQUEUE_PRIORITY);

    if (!dev->vicap_workqueue)
    {
        VICAP_INFO(dev, "workqueue created failed!\n");
        return RET_SYS_ERROR;
    }
#endif

    HAL_NVIC_SetIRQHandler(dev->irq, dev->irqhandler);
    HAL_NVIC_EnableIRQ(dev->irq);
    rk_vicap_hw_iomux();

    rk_vicap_function_exit();

    return rk_vicap_controller_register(vicap, name, dev);
}

/**
 * @brief  The probe function for registering vicap device.
 */
int rk_device_vicap_init(void)
{
    ret_err_t ret;
    struct rk_vicap_dev *rk_vicapdev = &vicap_instance;
    struct vicap_clock_info *vicap_clk = &rk_vicapdev->vicap_clk;

    rk_vicap_function_enter();

    rk_strncpy(rk_vicapdev->name, RK_VICAP_DEVICE_NAME,
               RK_VICAP_DEVICE_NAME_SIZE);
    rk_vicapdev->vicap_reg = VICAP;
    rk_vicapdev->cru_reg = CRU;
    rk_vicapdev->irq = VICAP_IRQn;
    rk_vicapdev->irqhandler = (NVIC_IRQHandler)&rk_vicap_irq;

#if defined(__RT_THREAD__)

    vicap_clk->vicap_aclk = rk_get_clk_gate_from_id(ACLK_CIF_GATE);
    vicap_clk->vicap_hclk = rk_get_clk_gate_from_id(HCLK_CIF_GATE);
#if defined(RKMCU_RK2108)
    vicap_clk->vicap_pclk = rk_get_clk_gate_from_id(PCLK_CIF_GATE);
#endif

#elif defined(__RK_OS__)

    vicap_clk->vicap_aclk = rk_get_clk_gate_from_id(ACLK_VIP_GATE);
    vicap_clk->vicap_hclk = rk_get_clk_gate_from_id(HCLK_VIP_GATE);
#if defined(RKMCU_RK2206)
    WRITE_REG_MASK_WE(GRF->SOC_CON1, GRF_SOC_CON1_GRF_CON_VIP_NEG_POS_SEL_MASK,
                      0x0800);
#endif

#endif

#if defined(VICAP_MODULE_WORKMODE_FRAME_ONEFRAME)
    rk_vicap_ctrl_set_workmode(rk_vicapdev, VICAP_WORKMODE_FRAME_ONEFRAME);
#elif defined(VICAP_MODULE_WORKMODE_FRAME_PINGONG)
    rk_vicap_ctrl_set_workmode(rk_vicapdev, VICAP_WORKMODE_FRAME_PINGPONG);
#elif defined(VICAP_MODULE_WORKMODE_BLOCK_PINGPONG)
    rk_vicap_ctrl_set_workmode(rk_vicapdev, VICAP_WORKMODE_BLOCK_PINGPONG);
#endif

    ret = rk_vicap_register(rk_vicapdev, rk_vicapdev->name);

    rk_vicap_function_exit();
    return ret;
}

/** @} */  // VICAP_Public_Functions

#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_device_vicap_init);
#endif

#endif

/** @} */  // VICAP

/** @} */  // RKBSP_Driver_Reference
