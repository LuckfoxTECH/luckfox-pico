/**
  * Copyright (c) 2024 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc431hai.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2024-01-16     HongLin Lan  first implementation
  *
  ******************************************************************************
  */

#include "camera.h"
#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <rtconfig.h>
#include "hal_base.h"
#include "board.h"
#include "drv_clock.h"

#ifdef RT_USING_SC431HAI

//#define REG_LIST_CONVERSION_DEBUG
#define SC431HAI_DEBUG_PRT      0

#if SC431HAI_DEBUG_PRT
#include <stdio.h>
#define sc431hai_dbg(fmt, args...)               rt_kprintf("[SC431HAI] "fmt"", ##args)
#else
#define sc431hai_dbg(fmt, args...)
#endif

#define sc431hai_info(fmt, args...)         rt_kprintf("[SC431HAI] "fmt"", ##args)
#define sc431hai_err(fmt, args...)         rt_kprintf("[SC431HAI] ERR "fmt"", ##args)

#define SC431HAI_REG_EXP_LONG_H           0x3e00
#define SC431HAI_REG_EXP_LONG_M           0x3e01
#define SC431HAI_REG_EXP_LONG_L           0x3e02
#define SC431HAI_REG_DIG_GAIN            0x3e06
#define SC431HAI_REG_DIG_FINE_GAIN       0x3e07
#define SC431HAI_REG_ANA_GAIN            0x3e08
#define SC431HAI_REG_ANA_FINE_GAIN       0x3e09
#define SC431HAI_EXP_MIN                 2
#define SC431HAI_GAIN_MIN                0x40
#define SC431HAI_GAIN_MAX                (49674)  //48.510*16*64
#define SC431HAI_GAIN_STEP               1
#define SC431HAI_GAIN_DEFAULT            0x40
#define SC431HAI_FLIP_MIRROR_REG         0x3221
#define SC431HAI_REG_VTS_H               0x320e
#define SC431HAI_REG_VTS_L               0x320f
#define SC431HAI_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC431HAI_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC431HAI_REG_STREAM_CTRL         0x0100
#define SC431HAI_STREAM_ON               0x01
#define SC431HAI_STREAM_OFF              0x00

#define REG_END                     (0x00)
#define REG_DELAY                   (0xff)

#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

/* redefine system err code */
#define RET_SYS_EOK                     (RT_EOK)
#define RET_SYS_ERROR                   (-RT_ERROR)
#define RET_SYS_ETIMEOUT                (-RT_ETIMEOUT)  /**< Timed out */
#define RET_SYS_EFULL                   (-RT_EFULL)     /**< The resource is full */
#define RET_SYS_EEMPTY                  (-RT_EEMPTY)    /**< The resource is empty */
#define RET_SYS_ENOMEM                  (-RT_ENOMEM)    /**< No memory */
#define RET_SYS_ENOSYS                  (-RT_ENOSYS)    /**< No system */
#define RET_SYS_EBUSY                   (-RT_EBUSY)     /**< Busy */
#define RET_SYS_EIO                     (-RT_EIO)       /**< IO error */
#define RET_SYS_EINTR                   (-RT_EINTR)     /**< Interrupted system call */
#define RET_SYS_EINVAL                  (-RT_EINVAL)    /**< Invalid argument */

struct rt_i2c_bus_device;

struct sc431hai_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc431hai_mode
{
    struct v4l2_fract max_fps;
    uint16_t bus_fmt;
    uint16_t width;
    uint16_t height;
    uint16_t hts_def;
    uint16_t vts_def;
    uint16_t exp_def;
    uint32_t xvclk;
    uint32_t link_freq;
#ifndef REG_LIST_CONVERSION_DEBUG
    const uint8_t *reg_list;
#else
    const struct sc431hai_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc431hai_dev
{
    struct rk_camera_device parent;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];
    uint8_t time_valid_delay;
    uint8_t gain_valid_delay;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    struct rt_i2c_bus_device *i2c_bus;
    uint8_t i2c_addr;
    uint8_t flip;
    struct rt_mutex mutex_lock;
    struct rk_camera_exp_val init_exp;
    const struct sc431hai_mode *cur_mode;
    struct sc431hai_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc431hai_dev *rt_sc431hai_dev_t;

static struct sc431hai_dev g_sc431hai;

#ifndef REG_LIST_CONVERSION_DEBUG
/*
 * Xclk 24Mhz
 * max_framerate 112fps
 * mipi_datarate per lane 630Mbps, 2lane, 10bit
 * binning to 1280x720
 * Cleaned_0x08_SC431AI_MIPI_24Minput_2Lane_10bit_630Mbps_1280x720_112fps_hsub_vbin_外供.ini
 */
static const uint8_t g_sc431hai_1280x720_112fps_reg_table[] =
{
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x4, 0x30, 0x18, 0x3a, 0x0c,
    0x3, 0x30, 0x1f, 0x08,
    0x5, 0x30, 0x58, 0x21, 0x53, 0x40,
    0xa, 0x32, 0x08, 0x05, 0x00, 0x02, 0xd0, 0x05, 0xdc, 0x02, 0xee,
    0x3, 0x32, 0x11, 0x02,
    0x3, 0x32, 0x13, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x50, 0x00,
    0x3, 0x33, 0x01, 0x12,
    0x8, 0x33, 0x04, 0x50, 0x00, 0x68, 0x04, 0x0a, 0x88,
    0x3, 0x33, 0x0b, 0xd8,
    0x4, 0x33, 0x0d, 0x08, 0x28,
    0x4, 0x33, 0x1e, 0x41, 0x79,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x3, 0x33, 0x64, 0x5e,
    0x11, 0x33, 0x8e, 0xe2, 0x80, 0x08, 0x18, 0xb8, 0x12, 0x14, 0x10, 0x88, 0x98, 0xb8, 0x0e, 0x18, 0x20, 0x24,
    0x6, 0x33, 0xac, 0x0a, 0x10, 0x4c, 0x84,
    0x4, 0x33, 0xb2, 0x50, 0x10,
    0x8, 0x33, 0xf8, 0x00, 0x68, 0x00, 0x68, 0x48, 0x78,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x40, 0x58, 0x10, 0x10,
    0x4, 0x34, 0xf8, 0x78, 0x10,
    0x3, 0x36, 0x33, 0x44,
    0x4, 0x36, 0x3b, 0x8f, 0x02,
    0x3, 0x36, 0x41, 0x08,
    0x3, 0x36, 0x54, 0x20,
    0x5, 0x36, 0x74, 0xc2, 0xb4, 0x88,
    0x4, 0x36, 0x7c, 0x88, 0xb8,
    0x7, 0x36, 0x90, 0x34, 0x45, 0x55, 0x88, 0xb8,
    0x8, 0x36, 0x96, 0x87, 0x85, 0x87, 0x8d, 0xa5, 0xe6,
    0x7, 0x36, 0xa2, 0x80, 0x88, 0xf8, 0xb8, 0x98,
    0x3, 0x36, 0xd0, 0x15,
    0x6, 0x36, 0xea, 0x23, 0x0c, 0x55, 0x18,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x03,
    0x3, 0x37, 0x27, 0x14,
    0x7, 0x37, 0xb0, 0x17, 0x9b, 0x9b, 0x88, 0xb8,
    0x6, 0x37, 0xfa, 0x23, 0x44, 0x20, 0x1c,
    0x3, 0x39, 0x1f, 0x41,
    0x3, 0x39, 0x26, 0xe0,
    0x8, 0x39, 0x33, 0x80, 0xf8, 0x00, 0x2d, 0x64, 0x62,
    0x5, 0x3e, 0x00, 0x00, 0x5d, 0x10,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x45, 0x0d, 0x0b,
    0x3, 0x50, 0x00, 0x46,
    0x3, 0x57, 0x80, 0x76,
    0x3, 0x57, 0x84, 0x10,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x08, 0x0a, 0x0a, 0x08, 0x40,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04,
    0x4, 0x57, 0xac, 0x00, 0x00,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x39,
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 630Mbps, 2lane, 10bit
 * full resolution
 * Cleaned_0x07_SC431AI_MIPI_24Minput_2Lane_10bit_630Mbps_2560x1440_30fps_外供.ini
 */
static const uint8_t g_sc431hai_2560x1440_30fps_reg_table[] =
{
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x4, 0x30, 0x18, 0x3a, 0x0c,
    0x3, 0x30, 0x1f, 0x07,
    0x5, 0x30, 0x58, 0x21, 0x53, 0x40,
    0x3, 0x32, 0x50, 0x00,
    0x3, 0x33, 0x01, 0x0c,
    0x8, 0x33, 0x04, 0x50, 0x00, 0x50, 0x04, 0x0a, 0x60,
    0x3, 0x33, 0x0b, 0xc8,
    0x4, 0x33, 0x0d, 0x08, 0x38,
    0x4, 0x33, 0x1e, 0x41, 0x51,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x3, 0x33, 0x64, 0x5e,
    0x11, 0x33, 0x8e, 0xe2, 0x80, 0x08, 0x18, 0xb8, 0x12, 0x14, 0x10, 0x88, 0x98, 0xb8, 0x10, 0x16, 0x1c, 0x40,
    0x6, 0x33, 0xac, 0x0a, 0x10, 0x4f, 0x5e,
    0x4, 0x33, 0xb2, 0x50, 0x10,
    0x8, 0x33, 0xf8, 0x00, 0x50, 0x00, 0x50, 0x48, 0x78,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x40, 0x58, 0x10, 0x10,
    0x4, 0x34, 0xf8, 0x78, 0x10,
    0x3, 0x36, 0x33, 0x44,
    0x4, 0x36, 0x3b, 0x8f, 0x02,
    0x3, 0x36, 0x41, 0x08,
    0x3, 0x36, 0x54, 0x20,
    0x5, 0x36, 0x74, 0xc2, 0xb4, 0x88,
    0x4, 0x36, 0x7c, 0x88, 0xb8,
    0x7, 0x36, 0x90, 0x34, 0x44, 0x54, 0x88, 0x98,
    0x8, 0x36, 0x96, 0x87, 0x85, 0x87, 0x8d, 0xa5, 0xe6,
    0x7, 0x36, 0xa2, 0x80, 0x88, 0xf8, 0xb8, 0x98,
    0x3, 0x36, 0xd0, 0x15,
    0x6, 0x36, 0xea, 0x23, 0x0d, 0x55, 0x18,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x03,
    0x3, 0x37, 0x27, 0x14,
    0x7, 0x37, 0xb0, 0x17, 0x9b, 0x9b, 0x88, 0xb8,
    0x6, 0x37, 0xfa, 0x23, 0x54, 0x21, 0x1c,
    0x3, 0x39, 0x1f, 0x41,
    0x3, 0x39, 0x26, 0xe0,
    0x8, 0x39, 0x33, 0x80, 0xf9, 0x00, 0x55, 0x63, 0x61,
    0x5, 0x3e, 0x00, 0x00, 0xba, 0xd0,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x45, 0x0d, 0x0d,
    0x3, 0x48, 0x19, 0x08,
    0x3, 0x48, 0x1b, 0x05,
    0x3, 0x48, 0x1d, 0x11,
    0x3, 0x48, 0x1f, 0x04,
    0x3, 0x48, 0x21, 0x09,
    0x3, 0x48, 0x23, 0x04,
    0x3, 0x48, 0x25, 0x04,
    0x3, 0x48, 0x27, 0x04,
    0x3, 0x48, 0x29, 0x07,
    0x3, 0x57, 0x80, 0x76,
    0x3, 0x57, 0x84, 0x10,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x08, 0x0a, 0x0a, 0x08, 0x40,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04,
    0x4, 0x57, 0xac, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x3b,
};

#else
/*
 * Xclk 24Mhz
 * max_framerate 112fps
 * mipi_datarate per lane 630Mbps, 2lane, 10bit
 * binning to 1280x720
 */
static const struct sc431hai_sensor_reg g_sc431hai_1280x720_112fps_reg_table[] =
{
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x3018, 0x3a},
    {0x3019, 0x0c},
    {0x301f, 0x08},
    {0x3058, 0x21},
    {0x3059, 0x53},
    {0x305a, 0x40},
    {0x3208, 0x05},
    {0x3209, 0x00},
    {0x320a, 0x02},
    {0x320b, 0xd0},
    {0x320c, 0x05},
    {0x320d, 0xdc},
    {0x320e, 0x02},
    {0x320f, 0xee},
    {0x3211, 0x02},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3250, 0x00},
    {0x3301, 0x12},
    {0x3304, 0x50},
    {0x3305, 0x00},
    {0x3306, 0x68},
    {0x3307, 0x04},
    {0x3308, 0x0a},
    {0x3309, 0x88},
    {0x330b, 0xd8},
    {0x330d, 0x08},
    {0x330e, 0x28},
    {0x331e, 0x41},
    {0x331f, 0x79},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x3364, 0x5e},
    {0x338e, 0xe2},
    {0x338f, 0x80},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0xb8},
    {0x3393, 0x12},
    {0x3394, 0x14},
    {0x3395, 0x10},
    {0x3396, 0x88},
    {0x3397, 0x98},
    {0x3398, 0xb8},
    {0x3399, 0x0e},
    {0x339a, 0x18},
    {0x339b, 0x20},
    {0x339c, 0x24},
    {0x33ac, 0x0a},
    {0x33ad, 0x10},
    {0x33ae, 0x4c},
    {0x33af, 0x84},
    {0x33b2, 0x50},
    {0x33b3, 0x10},
    {0x33f8, 0x00},
    {0x33f9, 0x68},
    {0x33fa, 0x00},
    {0x33fb, 0x68},
    {0x33fc, 0x48},
    {0x33fd, 0x78},
    {0x349f, 0x03},
    {0x34a6, 0x40},
    {0x34a7, 0x58},
    {0x34a8, 0x10},
    {0x34a9, 0x10},
    {0x34f8, 0x78},
    {0x34f9, 0x10},
    {0x3633, 0x44},
    {0x363b, 0x8f},
    {0x363c, 0x02},
    {0x3641, 0x08},
    {0x3654, 0x20},
    {0x3674, 0xc2},
    {0x3675, 0xb4},
    {0x3676, 0x88},
    {0x367c, 0x88},
    {0x367d, 0xb8},
    {0x3690, 0x34},
    {0x3691, 0x45},
    {0x3692, 0x55},
    {0x3693, 0x88},
    {0x3694, 0xb8},
    {0x3696, 0x87},
    {0x3697, 0x85},
    {0x3698, 0x87},
    {0x3699, 0x8d},
    {0x369a, 0xa5},
    {0x369b, 0xe6},
    {0x36a2, 0x80},
    {0x36a3, 0x88},
    {0x36a4, 0xf8},
    {0x36a5, 0xb8},
    {0x36a6, 0x98},
    {0x36d0, 0x15},
    {0x36ea, 0x23},
    {0x36eb, 0x0c},
    {0x36ec, 0x55},
    {0x36ed, 0x18},
    {0x370f, 0x01},
    {0x3722, 0x03},
    {0x3727, 0x14},
    {0x37b0, 0x17},
    {0x37b1, 0x9b},
    {0x37b2, 0x9b},
    {0x37b3, 0x88},
    {0x37b4, 0xb8},
    {0x37fa, 0x23},
    {0x37fb, 0x44},
    {0x37fc, 0x20},
    {0x37fd, 0x1c},
    {0x391f, 0x41},
    {0x3926, 0xe0},
    {0x3933, 0x80},
    {0x3934, 0xf8},
    {0x3935, 0x00},
    {0x3936, 0x2d},
    {0x3937, 0x64},
    {0x3938, 0x62},
    {0x3e00, 0x00},
    {0x3e01, 0x5d},
    {0x3e02, 0x10},
    {0x4509, 0x20},
    {0x450d, 0x0b},
    {0x5000, 0x46},
    {0x5780, 0x76},
    {0x5784, 0x10},
    {0x5787, 0x0a},
    {0x5788, 0x0a},
    {0x5789, 0x08},
    {0x578a, 0x0a},
    {0x578b, 0x0a},
    {0x578c, 0x08},
    {0x578d, 0x40},
    {0x5790, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x04},
    {0x5793, 0x08},
    {0x5794, 0x04},
    {0x5795, 0x04},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x36e9, 0x53},
    {0x37f9, 0x53},
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 630Mbps, 2lane, 10bit
 * full resolution
 */
static const struct sc431hai_sensor_reg g_sc431hai_2560x1440_30fps_reg_table[] =
{
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x3018, 0x3a},
    {0x3019, 0x0c},
    {0x301f, 0x07},
    {0x3058, 0x21},
    {0x3059, 0x53},
    {0x305a, 0x40},
    {0x3250, 0x00},
    {0x3301, 0x0c},
    {0x3304, 0x50},
    {0x3305, 0x00},
    {0x3306, 0x50},
    {0x3307, 0x04},
    {0x3308, 0x0a},
    {0x3309, 0x60},
    {0x330b, 0xc8},
    {0x330d, 0x08},
    {0x330e, 0x38},
    {0x331e, 0x41},
    {0x331f, 0x51},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x3364, 0x5e},
    {0x338e, 0xe2},
    {0x338f, 0x80},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0xb8},
    {0x3393, 0x12},
    {0x3394, 0x14},
    {0x3395, 0x10},
    {0x3396, 0x88},
    {0x3397, 0x98},
    {0x3398, 0xb8},
    {0x3399, 0x10},
    {0x339a, 0x16},
    {0x339b, 0x1c},
    {0x339c, 0x40},
    {0x33ac, 0x0a},
    {0x33ad, 0x10},
    {0x33ae, 0x4f},
    {0x33af, 0x5e},
    {0x33b2, 0x50},
    {0x33b3, 0x10},
    {0x33f8, 0x00},
    {0x33f9, 0x50},
    {0x33fa, 0x00},
    {0x33fb, 0x50},
    {0x33fc, 0x48},
    {0x33fd, 0x78},
    {0x349f, 0x03},
    {0x34a6, 0x40},
    {0x34a7, 0x58},
    {0x34a8, 0x10},
    {0x34a9, 0x10},
    {0x34f8, 0x78},
    {0x34f9, 0x10},
    {0x3633, 0x44},
    {0x363b, 0x8f},
    {0x363c, 0x02},
    {0x3641, 0x08},
    {0x3654, 0x20},
    {0x3674, 0xc2},
    {0x3675, 0xb4},
    {0x3676, 0x88},
    {0x367c, 0x88},
    {0x367d, 0xb8},
    {0x3690, 0x34},
    {0x3691, 0x44},
    {0x3692, 0x54},
    {0x3693, 0x88},
    {0x3694, 0x98},
    {0x3696, 0x87},
    {0x3697, 0x85},
    {0x3698, 0x87},
    {0x3699, 0x8d},
    {0x369a, 0xa5},
    {0x369b, 0xe6},
    {0x36a2, 0x80},
    {0x36a3, 0x88},
    {0x36a4, 0xf8},
    {0x36a5, 0xb8},
    {0x36a6, 0x98},
    {0x36d0, 0x15},
    {0x36ea, 0x23},
    {0x36eb, 0x0d},
    {0x36ec, 0x55},
    {0x36ed, 0x18},
    {0x370f, 0x01},
    {0x3722, 0x03},
    {0x3727, 0x14},
    {0x37b0, 0x17},
    {0x37b1, 0x9b},
    {0x37b2, 0x9b},
    {0x37b3, 0x88},
    {0x37b4, 0xb8},
    {0x37fa, 0x23},
    {0x37fb, 0x54},
    {0x37fc, 0x21},
    {0x37fd, 0x1c},
    {0x391f, 0x41},
    {0x3926, 0xe0},
    {0x3933, 0x80},
    {0x3934, 0xf9},
    {0x3935, 0x00},
    {0x3936, 0x55},
    {0x3937, 0x63},
    {0x3938, 0x61},
    {0x3e00, 0x00},
    {0x3e01, 0xba},
    {0x3e02, 0xd0},
    {0x4509, 0x20},
    {0x450d, 0x0d},
    {0x4819, 0x08},
    {0x481b, 0x05},
    {0x481d, 0x11},
    {0x481f, 0x04},
    {0x4821, 0x09},
    {0x4823, 0x04},
    {0x4825, 0x04},
    {0x4827, 0x04},
    {0x4829, 0x07},
    {0x5780, 0x76},
    {0x5784, 0x10},
    {0x5787, 0x0a},
    {0x5788, 0x0a},
    {0x5789, 0x08},
    {0x578a, 0x0a},
    {0x578b, 0x0a},
    {0x578c, 0x08},
    {0x578d, 0x40},
    {0x5790, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x04},
    {0x5793, 0x08},
    {0x5794, 0x04},
    {0x5795, 0x04},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x36e9, 0x53},
    {0x37f9, 0x53},
    {REG_END, 0x00},
};
#endif

static const struct sc431hai_mode supported_modes[] =
{
    [SC431HAI_1280X720] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1280,
        .height = 720,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1120000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x05dc,
        .vts_def = 0x02ee,
        .xvclk = 24000000,
        .link_freq = 315000000,
        .reg_list = g_sc431hai_1280x720_112fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc431hai_1280x720_112fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [SC431HAI_2560X1440] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2560,
        .height = 1440,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x2ee * 4,
        .vts_def = 0x05dc,
        .xvclk = 24000000,
        .link_freq = 315000000,
        .reg_list = g_sc431hai_2560x1440_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc431hai_2560x1440_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct sc431hai_ops
{
    ret_err_t (*init)(struct sc431hai_dev *dev);
    ret_err_t (*open)(struct sc431hai_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc431hai_dev *dev);
    ret_err_t (*control)(struct sc431hai_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc431hai_read_reg(struct rt_i2c_bus_device *bus,
                                   uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    int retry = 10;
    ret_err_t ret;

    RT_ASSERT(bus != RT_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    msg[0].addr = g_sc431hai.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_sc431hai.i2c_addr;
    msg[1].flags = RT_I2C_RD;
    msg[1].len = 1;
    msg[1].buf = recv_buf;

    while (1)
    {
        ret = rt_i2c_transfer(bus, msg, 2);
        if (ret == 2)
        {
            *data = recv_buf[0];
            return RET_SYS_EOK;
        }
        else
        {
            sc431hai_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc431hai_write_reg(struct rt_i2c_bus_device *bus,
                                    uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    send_buf[2] = data;

    msg.addr = g_sc431hai.i2c_addr;
    msg.flags = RT_I2C_WR;
    msg.buf = send_buf;
    msg.len = 3;

    while (1)
    {
        ret = rt_i2c_transfer(bus, &msg, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            sc431hai_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int sc431hai_write_multiple_reg_continue(struct sc431hai_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msg;
    int ret = 0;
    int offset = 0;
    int retry = 10;

    RT_ASSERT(dev != RT_NULL && i2c_data != RT_NULL);

    i2c_bus = dev->i2c_bus;
    RT_ASSERT(i2c_bus != RT_NULL);

    for (i = 0; i < i2c_data[len - 1];)
    {
        msg.addr = g_sc431hai.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            sc431hai_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
        offset += (i2c_data[offset] + 1);
        retry = 10;
        i++;
    }

    return RET_SYS_EOK;
}

#else
static ret_err_t sc431hai_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msgs.addr = g_sc431hai.i2c_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = data;
    msgs.len = len;

    while (1)
    {
        ret = rt_i2c_transfer(bus, &msgs, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            sc431hai_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc431hai_write_multiple_reg(struct sc431hai_dev *dev,
                                        const struct sc431hai_sensor_reg *reg, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    int k = 0;
    char *data = rt_malloc(len);
    uint16_t reg_addr;
    int j = 0;
    int cnt = 0;


    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(reg != RT_NULL);

    i2c_bus = dev->i2c_bus;
    RT_ASSERT(i2c_bus != RT_NULL);

    for (i = 0;; i++)
    {
        if (reg[i].reg_addr == REG_END)
        {
            if (k > 0)
            {
                cnt++;
                rt_kprintf("0x%x, ", k + 2);
                for (j = 0; j < k + 2; j++)
                    rt_kprintf("0x%02x, ", data[j]);
                rt_kprintf("\n");

                sc431hai_write_reg_discontinued(i2c_bus, data, k + 2);
                k = 0;
            }
            break;
        }

        if (reg[i].reg_addr == REG_DELAY)
        {
            if (k > 0)
            {
                cnt++;
                rt_kprintf("0x%x, ", k + 2);
                for (j = 0; j < k + 2; j++)
                    rt_kprintf("0x%02x, ", data[j]);
                rt_kprintf("\n");

                sc431hai_write_reg_discontinued(i2c_bus, data, k + 2);
                k = 0;
            }
            HAL_DelayUs(reg[i].val);
        }
        else
        {
            if (k == 0)
            {
                reg_addr = reg[i].reg_addr;
                data[0] = ((reg_addr >> 8) & 0xff);
                data[1] = ((reg_addr >> 0) & 0xff);
                data[2] = reg[i].val;
                k++;
            }
            else
            {
                if ((reg[i - 1].reg_addr + 1) == reg[i].reg_addr)
                {
                    data[k + 2] = reg[i].val;
                    k++;
                    //rt_kprintf(">>>k %d, addr %04x\n", k, reg[i].reg_addr);
                }
                else
                {
                    cnt++;
                    rt_kprintf("0x%x, ", k + 2);
                    for (j = 0; j < k + 2; j++)
                        rt_kprintf("0x%02x, ", data[j]);
                    rt_kprintf("\n");

                    sc431hai_write_reg_discontinued(i2c_bus, data, k + 2);
                    reg_addr = reg[i].reg_addr;
                    data[0] = ((reg_addr >> 8) & 0xff);
                    data[1] = ((reg_addr >> 0) & 0xff);
                    data[2] = reg[i].val;
                    k = 1;
                }
            }
        }
    }

    rt_kprintf("0x%x,\n", cnt);
}

static void rk_continuous_reg_dump(struct sc431hai_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc431hai_info("id: %d, width: %d, height: %d\n",
                      id, supported_modes[id].width, supported_modes[id].height);
        sc431hai_write_multiple_reg((struct sc431hai_dev *)dev,
                                    supported_modes[id].reg_list,
                                    supported_modes[id].reg_list_size);
    }
}
#endif

static int sc431hai_get_gain_reg(uint8_t *again, uint8_t *dgain, uint8_t *again_fine,
                                 uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    if (total_gain < 64)
        total_gain = 64;
    else if (total_gain > SC431HAI_GAIN_MAX)
        total_gain = SC431HAI_GAIN_MAX;

    gain_factor = total_gain * 1000 / 64;
    if (gain_factor < 1540)
    {
        *again = 0x00;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 1000;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 3080)
    {
        *again = 0x80;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 1540;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 6160)
    {
        *again = 0x81;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 3080;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 12320)
    {
        *again = 0x83;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 6160;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 24640)
    {
        *again = 0x87;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 12320;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 48510)
    {
        *again = 0x8f;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 24640;
        *dgain_fine = 0x80;
    }
    //open dgain begin  max digital gain 16X
    else if (gain_factor < 48510 * 2)
    {
        *again = 0x8f;
        *dgain = 0x00;
        *again_fine = 0x3f;
        *dgain_fine = gain_factor * 128 / 48510;
    }
    else if (gain_factor < 48510 * 4)
    {
        *again = 0x8f;
        *dgain = 0x01;
        *again_fine = 0x3f;
        *dgain_fine = gain_factor * 128 / 48510 / 2;
    }
    else if (gain_factor  < 48510 * 8)
    {
        *again = 0x8f;
        *dgain = 0x03;
        *again_fine = 0x3f;
        *dgain_fine = gain_factor * 128 / 48510 / 4;
    }
    else if (gain_factor < 48510 * 16)
    {
        *again = 0x8f;
        *dgain = 0x07;
        *again_fine = 0x3f;
        *dgain_fine = gain_factor * 128 / 48510 / 8;
    }

    return ret;
}

static ret_err_t rk_sc431hai_set_expval(struct sc431hai_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, a_gain_fine, d_gain_fine;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc431hai_dbg("sc431hai don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0] << 1;
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC431HAI_EXP_MIN)
        l_exp_time = SC431HAI_EXP_MIN;

    sc431hai_dbg("total_time reg: 0x%x, total_gain reg 0x%x\n",
                 l_exp_time, l_a_gain);

    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_EXP_LONG_H,
                             (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_EXP_LONG_M,
                             (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_EXP_LONG_L,
                             (uint8_t)((l_exp_time & 0xf) << 4));
    sc431hai_get_gain_reg(&a_gain, &d_gain, &a_gain_fine, &d_gain_fine, l_a_gain);

    sc431hai_dbg("a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", a_gain, d_gain,
                 a_gain_fine, d_gain_fine);

    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_ANA_GAIN, a_gain);
    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_ANA_FINE_GAIN, a_gain_fine);
    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_DIG_GAIN, d_gain);
    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc431hai_set_vts(struct sc431hai_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc431hai_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_VTS_L,
                             (uint8_t)(dst_vts & 0xff));
    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_VTS_H,
                             (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc431hai_set_flip_mirror(struct sc431hai_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc431hai_read_reg(dev->i2c_bus, SC431HAI_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC431HAI_FETCH_MIRROR(val, flip & 0x01);
        val = SC431HAI_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC431HAI_FETCH_MIRROR(val, false);
        val = SC431HAI_FETCH_FLIP(val, false);
        break;
    };
    sc431hai_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc431hai_write_reg(dev->i2c_bus, SC431HAI_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc431hai_stream_on(struct sc431hai_dev *dev)
{
    sc431hai_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(5);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    sc431hai_write_multiple_reg_continue((struct sc431hai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc431hai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc431hai_set_flip_mirror(dev, dev->flip);

    sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_STREAM_CTRL, SC431HAI_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc431hai_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc431hai_stream_on_late(struct sc431hai_dev *dev)
{

    sc431hai_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc431hai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc431hai_set_flip_mirror(dev, dev->flip);

    sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_STREAM_CTRL, SC431HAI_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc431hai_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc431hai_stream_off(struct sc431hai_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc431hai_write_reg(dev->i2c_bus, SC431HAI_REG_STREAM_CTRL, SC431HAI_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc431hai_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_sc431hai_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc431hai_dev *sc431hai;

    sc431hai = (struct sc431hai_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc431hai->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc431hai);
#endif

    if (sc431hai)
    {
        camera->info.mbus_fmt.width = sc431hai->cur_mode->width;
        camera->info.mbus_fmt.height = sc431hai->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc431hai->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc431hai->cur_mode->hdr_mode;

        sc431hai->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc431hai->i2c_name);

        if (!sc431hai->i2c_bus)
        {
            sc431hai_err("not find i2c :%s !!!\n",
                         sc431hai->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc431hai_dbg("find i2c_bus:%s\n", sc431hai->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc431hai_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc431hai_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_sc431hai_get_expinf(struct sc431hai_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc431hai_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    exp->width = mode->width;
    exp->height = mode->height;
    exp->hts = mode->hts_def;
    exp->vts = mode->vts_def;
    exp->pix_clk = (uint64_t)exp->hts * (uint64_t)exp->vts * (uint64_t)mode->max_fps.denominator /
                   (uint64_t)mode->max_fps.numerator;
    exp->time_valid_delay = dev->time_valid_delay;
    exp->gain_valid_delay = dev->gain_valid_delay;

    exp->dst_width = dev->dst_mode->width;
    exp->dst_height = dev->dst_mode->height;
    exp->dst_hts = dev->dst_mode->hts_def;
    exp->dst_vts = dev->dst_mode->vts_def;
    exp->dst_pix_clk = (uint64_t)exp->dst_hts * (uint64_t)exp->dst_vts *
                       (uint64_t)dev->dst_mode->max_fps.denominator /
                       (uint64_t)dev->dst_mode->max_fps.numerator;

    return ret;
}

static ret_err_t rk_sc431hai_get_intput_fmt(struct sc431hai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc431hai_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc431hai_set_intput_fmt(struct sc431hai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc431hai_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc431hai_info("dst resulotion, width %d, height %d\n",
                  dev->dst_mode->width, dev->dst_mode->height);
    if (mbus_fmt->width == dev->dst_mode->width &&
            mbus_fmt->height == dev->dst_mode->height)
    {
        mode = dev->dst_mode;
        is_find_fmt = true;
    }
    else
    {
        for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
        {
            mode = &supported_modes[i];
            if (mbus_fmt->width == mode->width &&
                    mbus_fmt->height == mode->height)
            {
                is_find_fmt = true;
                break;
            }
        }
    }
    if (is_find_fmt)
    {
        if (mode->width != dev->cur_mode->width)
        {
            dev->cur_mode = mode;
            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
#ifndef REG_LIST_CONVERSION_DEBUG
            struct camera_board_desc *sc431hai = (struct camera_board_desc *)&camera_sc431hai;
            if (sc431hai->rst_gpio.gpio_group)
            {
                HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc431hai->rst_gpio.gpio_group, sc431hai->rst_gpio.gpio_pin, GPIO_OUT);
                /* Set XCLR low At least 500ns after power raising */
                HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc431hai->rst_gpio.gpio_group, sc431hai->rst_gpio.gpio_pin, sc431hai->rst_active);
                HAL_DelayMs(5);
                sc431hai_info("set to reset, tick:%u\n", rt_tick_get());
                /* At least 1us between XCLR and clk */
                HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc431hai->rst_gpio.gpio_group, sc431hai->rst_gpio.gpio_pin, !sc431hai->rst_active);
                HAL_DelayMs(5);
            }

            sc431hai_write_multiple_reg_continue((struct sc431hai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc431hai_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc431hai_match_dst_config(struct sc431hai_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc431hai_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc431hai_info("cur resulotion, width %d, height %d, fps %d!\n",
                  dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc431hai_mode *)mode;
            ret = RET_SYS_EOK;
            sc431hai_info("find dst resulotion, width %d, height %d, fps %d!\n",
                          dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc431hai_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc431hai_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc431hai_control(struct rk_camera_device *dev,
                              dt_cmd_t cmd,
                              void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc431hai_dev *sc431hai;

    RT_ASSERT(dev != RT_NULL);
    sc431hai = (struct sc431hai_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc431hai_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc431hai_stream_on(sc431hai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc431hai_stream_off(sc431hai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc431hai_get_expinf(sc431hai, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc431hai_set_expval(sc431hai, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc431hai_set_vts(sc431hai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc431hai_get_intput_fmt(sc431hai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc431hai_set_intput_fmt(sc431hai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc431hai_match_dst_config(sc431hai, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc431hai_set_flip_mirror(sc431hai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc431hai_stream_on_late(sc431hai);
    }
    break;
    default:
        sc431hai_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc431hai_ops =
{
    rk_sc431hai_init,
    rk_sc431hai_open,
    NULL,
    rk_sc431hai_control
};

int rk_camera_sc431hai_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc431hai_dev *instance = &g_sc431hai;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *sc431hai = (struct camera_board_desc *)&camera_sc431hai;

    camera->ops = &rk_sc431hai_ops;

    rt_strncpy(instance->name, sc431hai->isp_subdev_name, rt_strlen(sc431hai->isp_subdev_name));
    rt_strncpy(instance->i2c_name, sc431hai->i2c_bus, rt_strlen(sc431hai->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (sc431hai->mode_id <= SC431HAI_MODE_ID_MAX)
    {
        sc431hai_dbg("mode_id: %d\n", sc431hai->mode_id);
        instance->cur_mode = &supported_modes[sc431hai->mode_id];
    }
    else
    {
        sc431hai_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct sc431hai_mode *)&supported_modes[SC431HAI_2560X1440];
    instance->time_valid_delay = sc431hai->time_valid_delay;
    instance->gain_valid_delay = sc431hai->gain_valid_delay;
    instance->i2c_addr = sc431hai->i2c_addr;
    instance->flip = 0;

    HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc431hai->rst_gpio.gpio_group, sc431hai->rst_gpio.gpio_pin, GPIO_OUT);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc431hai->rst_gpio.gpio_group, sc431hai->rst_gpio.gpio_pin, !sc431hai->rst_active);

    if (sc431hai->pwdn_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc431hai->pwdn_gpio.gpio_group, sc431hai->pwdn_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc431hai->pwdn_gpio.gpio_group, sc431hai->pwdn_gpio.gpio_pin, !sc431hai->pwdn_active);
    }

    if (sc431hai->mclk_out_gate_id && sc431hai->mclk_id)
    {
        clk_set_rate(sc431hai->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(sc431hai->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "SC431HAI_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, sc431hai->isp_subdev_name, rt_strlen(sc431hai->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void sc431hai_detect(void)
{
    struct sc431hai_dev *instance = &g_sc431hai;

    sc431hai_dbg("start to detect SC431HAI for testing \n");
    sc431hai_dbg("dev name:%s\n", instance->name);
    sc431hai_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        sc431hai_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        sc431hai_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc431hai_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc431hai_detect, check SC431HAI is available or not);
#endif
#endif
#endif
