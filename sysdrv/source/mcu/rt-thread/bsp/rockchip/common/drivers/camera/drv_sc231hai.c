/**
  * Copyright (c) 2024 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc231hai.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2024-02-07    Longchang.Ma          first implementation
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

#ifdef RT_USING_SC231HAI

//#define REG_LIST_CONVERSION_DEBUG
#define SC231HAI_DEBUG_PRT      1

#if SC231HAI_DEBUG_PRT
#include <stdio.h>
#define sc231hai_dbg(fmt, args...)       rt_kprintf("[SC231HAI] "fmt"", ##args)
#else
#define sc231hai_dbg(fmt, args...)
#endif

#define sc231hai_info(fmt, args...)      rt_kprintf("[SC231HAI] "fmt"", ##args)
#define sc231hai_err(fmt, args...)       rt_kprintf("[SC231HAI] ERR "fmt"", ##args)

#define SC231HAI_REG_EXP_LONG_H          0x3e00
#define SC231HAI_REG_EXP_LONG_M          0x3e01
#define SC231HAI_REG_EXP_LONG_L          0x3e02

#define SC231HAI_REG_EXP_SHORT_H         0x3e22
#define SC231HAI_REG_EXP_SHORT_M         0x3e04
#define SC231HAI_REG_EXP_SHORT_L         0x3e05

#define SC231HAI_REG_DIG_GAIN            0x3e06
#define SC231HAI_REG_DIG_FINE_GAIN       0x3e07
#define SC231HAI_REG_ANA_GAIN            0x3e08
#define SC231HAI_REG_ANA_FINE_GAIN       0x3e09

#define SC231HAI_REG_SDIG_GAIN           0x3e10
#define SC231HAI_REG_SDIG_FINE_GAIN      0x3e11
#define SC231HAI_REG_SANA_GAIN           0x3e12
#define SC231HAI_REG_SANA_FINE_GAIN      0x3e13

#define SC231HAI_REG_MAX_SEXPOSURE_H     0x3e23
#define SC231HAI_REG_MAX_SEXPOSURE_L     0x3e24

#define SC231HAI_GAIN_MIN                0x20
#define SC231HAI_GAIN_MAX                (117 * 16 * 32)       // 116.55*15.875*32
#define SC231HAI_GAIN_STEP               1
#define SC231HAI_GAIN_DEFAULT            0x40
#define SC231HAI_LGAIN                   0
#define SC231HAI_SGAIN                   1
#define SC231HAI_FLIP_MIRROR_REG         0x3221
#define SC231HAI_REG_VTS_H               0x320e
#define SC231HAI_REG_VTS_L               0x320f
#define SC231HAI_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC231HAI_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC231HAI_REG_STREAM_CTRL         0x0100
#define SC231HAI_STREAM_ON               0x01
#define SC231HAI_STREAM_OFF              0x00

#define SC231HAI_FETCH_EXP_H(VAL)        (((VAL) >> 12) & 0xF)
#define SC231HAI_FETCH_EXP_M(VAL)        (((VAL) >> 4) & 0xFF)
#define SC231HAI_FETCH_EXP_L(VAL)        (((VAL) & 0xF) << 4)

#define SC231HAI_REG_TEST_PATTERN        0x4501
#define SC231HAI_TEST_PATTERN_BIT_MASK   BIT(3)

#define REG_END                         (0x0)
#define REG_DELAY                       (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
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

struct sc231hai_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc231hai_mode
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
    const struct sc231hai_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc231hai_dev
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
    const struct sc231hai_mode *cur_mode;
    struct sc231hai_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc231hai_dev *rt_sc231hai_dev_t;

#ifndef REG_LIST_CONVERSION_DEBUG
//27MHz
//7425Mbps 2lane 640x480 120fps
static const uint8_t sc231hai_linear_640x480_120fps_regs[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x1c,
    0x5, 0x30, 0x58, 0x21, 0x53, 0x40,
    0x16, 0x32, 0x00, 0x01, 0x40, 0x00, 0x3c, 0x06, 0x47, 0x04, 0x03, 0x02, 0x80, 0x01, 0xe0, 0x08, 0xca, 0x02, 0x26, 0x00, 0x02, 0x00, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x50, 0x00,
    0x4, 0x33, 0x01, 0x0a, 0x20,
    0x5, 0x33, 0x04, 0x90, 0x00, 0x78,
    0x3, 0x33, 0x09, 0xd0,
    0x3, 0x33, 0x0b, 0xe8,
    0x3, 0x33, 0x0d, 0x08,
    0x3, 0x33, 0x1c, 0x04,
    0x4, 0x33, 0x1e, 0x81, 0xc1,
    0x3, 0x33, 0x23, 0x06,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x3, 0x33, 0x64, 0x5e,
    0x3, 0x33, 0x6c, 0x8c,
    0x3, 0x33, 0x7f, 0x13,
    0x10, 0x33, 0x8f, 0x80, 0x08, 0x18, 0xb8, 0x0e, 0x14, 0x10, 0x88, 0x98, 0xf8, 0x0a, 0x0e, 0x10, 0x14,
    0x4, 0x33, 0xae, 0x80, 0xc0,
    0x4, 0x33, 0xb2, 0x50, 0x08,
    0x8, 0x33, 0xf8, 0x00, 0x78, 0x00, 0x78, 0x48, 0x78,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x40, 0x58, 0x08, 0x0c,
    0x4, 0x34, 0xf8, 0x78, 0x18,
    0x4, 0x36, 0x19, 0x20, 0x90,
    0x3, 0x36, 0x33, 0x44,
    0x3, 0x36, 0x37, 0x5c,
    0x4, 0x36, 0x3c, 0xc0, 0x02,
    0xf, 0x36, 0x60, 0x80, 0x81, 0x8f, 0x81, 0x81, 0x82, 0x8f, 0x08, 0x80, 0x88, 0x98, 0xb8, 0xf8,
    0x5, 0x36, 0x70, 0xc2, 0xc2, 0x98,
    0x5, 0x36, 0x80, 0x43, 0x54, 0x54,
    0x4, 0x36, 0xc0, 0x80, 0x88,
    0x4, 0x36, 0xc8, 0x88, 0xb8,
    0x3, 0x36, 0xec, 0x6c,
    0x3, 0x37, 0x18, 0x04,
    0x3, 0x37, 0x22, 0x8b,
    0x3, 0x37, 0x24, 0xd1,
    0x3, 0x37, 0x41, 0x08,
    0x5, 0x37, 0x70, 0x17, 0x9b, 0x9b,
    0x4, 0x37, 0xc0, 0x88, 0xb8,
    0x4, 0x39, 0x02, 0xc0, 0x40,
    0x3, 0x39, 0x09, 0x00,
    0x3, 0x39, 0x1f, 0x41,
    0x3, 0x39, 0x26, 0xe0,
    0x4, 0x39, 0x33, 0x80, 0x02,
    0x3, 0x39, 0x37, 0x6f,
    0x5, 0x3e, 0x00, 0x00, 0x44, 0x10,
    0x3, 0x3e, 0x08, 0x00,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x45, 0x0d, 0x07,
    0x3, 0x48, 0x37, 0x36,
    0x3, 0x50, 0x00, 0x46,
    0x3, 0x57, 0x80, 0x76,
    0x3, 0x57, 0x84, 0x10,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x08, 0x0a, 0x0a, 0x08, 0x40,
    0x3, 0x57, 0x92, 0x04,
    0x3, 0x57, 0x95, 0x04,
    0x4, 0x57, 0xac, 0x00, 0x00,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x3, 0x36, 0xe9, 0x24,
    0x3, 0x37, 0xf9, 0x24,
    0x41,
};

//27MHz
//7425Mbps 2lane 1920x1080 60fps
static const uint8_t sc231hai_linear_1920x1080_60fps_regs[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x02,
    0x5, 0x30, 0x58, 0x21, 0x53, 0x40,
    0x3, 0x32, 0x50, 0x00,
    0x4, 0x33, 0x01, 0x0a, 0x20,
    0x5, 0x33, 0x04, 0x90, 0x00, 0x78,
    0x3, 0x33, 0x09, 0xd0,
    0x3, 0x33, 0x0b, 0xe8,
    0x3, 0x33, 0x0d, 0x08,
    0x3, 0x33, 0x1c, 0x04,
    0x4, 0x33, 0x1e, 0x81, 0xc1,
    0x3, 0x33, 0x23, 0x06,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x3, 0x33, 0x64, 0x5e,
    0x3, 0x33, 0x6c, 0x8c,
    0x3, 0x33, 0x7f, 0x13,
    0x10, 0x33, 0x8f, 0x80, 0x08, 0x18, 0xb8, 0x0e, 0x14, 0x10, 0x88, 0x98, 0xf8, 0x0a, 0x0e, 0x10, 0x14,
    0x4, 0x33, 0xae, 0x80, 0xc0,
    0x4, 0x33, 0xb2, 0x50, 0x08,
    0x8, 0x33, 0xf8, 0x00, 0x78, 0x00, 0x78, 0x48, 0x78,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x40, 0x58, 0x08, 0x0c,
    0x4, 0x34, 0xf8, 0x78, 0x18,
    0x4, 0x36, 0x19, 0x20, 0x90,
    0x3, 0x36, 0x33, 0x44,
    0x3, 0x36, 0x37, 0x5c,
    0x4, 0x36, 0x3c, 0xc0, 0x02,
    0xf, 0x36, 0x60, 0x80, 0x81, 0x8f, 0x81, 0x81, 0x82, 0x8f, 0x08, 0x80, 0x88, 0x98, 0xb8, 0xf8,
    0x5, 0x36, 0x70, 0xc2, 0xc2, 0x98,
    0x5, 0x36, 0x80, 0x43, 0x54, 0x54,
    0x4, 0x36, 0xc0, 0x80, 0x88,
    0x4, 0x36, 0xc8, 0x88, 0xb8,
    0x3, 0x37, 0x18, 0x04,
    0x3, 0x37, 0x22, 0x8b,
    0x3, 0x37, 0x24, 0xd1,
    0x3, 0x37, 0x41, 0x08,
    0x5, 0x37, 0x70, 0x17, 0x9b, 0x9b,
    0x4, 0x37, 0xc0, 0x88, 0xb8,
    0x4, 0x39, 0x02, 0xc0, 0x40,
    0x3, 0x39, 0x09, 0x00,
    0x3, 0x39, 0x1f, 0x41,
    0x3, 0x39, 0x26, 0xe0,
    0x4, 0x39, 0x33, 0x80, 0x02,
    0x3, 0x39, 0x37, 0x6f,
    0x5, 0x3e, 0x00, 0x00, 0x8b, 0xf0,
    0x3, 0x3e, 0x08, 0x00,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x45, 0x0d, 0x07,
    0x3, 0x57, 0x80, 0x76,
    0x3, 0x57, 0x84, 0x10,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x08, 0x0a, 0x0a, 0x08, 0x40,
    0x3, 0x57, 0x92, 0x04,
    0x3, 0x57, 0x95, 0x04,
    0x4, 0x57, 0xac, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x24,
    0x3, 0x37, 0xf9, 0x24,
    0x3a,
};

#else
/*
 * Xclk 27Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 7425Mbps, 2lane
 * binning to 640x480 then centered crop
 */
static const struct sc231hai_sensor_reg sc231hai_linear_640x480_120fps_regs[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x1c},
    {0x3058, 0x21},
    {0x3059, 0x53},
    {0x305a, 0x40},
    {0x3200, 0x01},
    {0x3201, 0x40},
    {0x3202, 0x00},
    {0x3203, 0x3c},
    {0x3204, 0x06},
    {0x3205, 0x47},
    {0x3206, 0x04},
    {0x3207, 0x03},
    {0x3208, 0x02},
    {0x3209, 0x80},
    {0x320a, 0x01},
    {0x320b, 0xe0},
    {0x320c, 0x08},
    {0x320d, 0xca},
    {0x320e, 0x02},
    {0x320f, 0x26},
    {0x3210, 0x00},
    {0x3211, 0x02},
    {0x3212, 0x00},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3250, 0x00},
    {0x3301, 0x0a},
    {0x3302, 0x20},
    {0x3304, 0x90},
    {0x3305, 0x00},
    {0x3306, 0x78},
    {0x3309, 0xd0},
    {0x330b, 0xe8},
    {0x330d, 0x08},
    {0x331c, 0x04},
    {0x331e, 0x81},
    {0x331f, 0xc1},
    {0x3323, 0x06},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x3364, 0x5e},
    {0x336c, 0x8c},
    {0x337f, 0x13},
    {0x338f, 0x80},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0xb8},
    {0x3393, 0x0e},
    {0x3394, 0x14},
    {0x3395, 0x10},
    {0x3396, 0x88},
    {0x3397, 0x98},
    {0x3398, 0xf8},
    {0x3399, 0x0a},
    {0x339a, 0x0e},
    {0x339b, 0x10},
    {0x339c, 0x14},
    {0x33ae, 0x80},
    {0x33af, 0xc0},
    {0x33b2, 0x50},
    {0x33b3, 0x08},
    {0x33f8, 0x00},
    {0x33f9, 0x78},
    {0x33fa, 0x00},
    {0x33fb, 0x78},
    {0x33fc, 0x48},
    {0x33fd, 0x78},
    {0x349f, 0x03},
    {0x34a6, 0x40},
    {0x34a7, 0x58},
    {0x34a8, 0x08},
    {0x34a9, 0x0c},
    {0x34f8, 0x78},
    {0x34f9, 0x18},
    {0x3619, 0x20},
    {0x361a, 0x90},
    {0x3633, 0x44},
    {0x3637, 0x5c},
    {0x363c, 0xc0},
    {0x363d, 0x02},
    {0x3660, 0x80},
    {0x3661, 0x81},
    {0x3662, 0x8f},
    {0x3663, 0x81},
    {0x3664, 0x81},
    {0x3665, 0x82},
    {0x3666, 0x8f},
    {0x3667, 0x08},
    {0x3668, 0x80},
    {0x3669, 0x88},
    {0x366a, 0x98},
    {0x366b, 0xb8},
    {0x366c, 0xf8},
    {0x3670, 0xc2},
    {0x3671, 0xc2},
    {0x3672, 0x98},
    {0x3680, 0x43},
    {0x3681, 0x54},
    {0x3682, 0x54},
    {0x36c0, 0x80},
    {0x36c1, 0x88},
    {0x36c8, 0x88},
    {0x36c9, 0xb8},
    {0x36ec, 0x6c},
    {0x3718, 0x04},
    {0x3722, 0x8b},
    {0x3724, 0xd1},
    {0x3741, 0x08},
    {0x3770, 0x17},
    {0x3771, 0x9b},
    {0x3772, 0x9b},
    {0x37c0, 0x88},
    {0x37c1, 0xb8},
    {0x3902, 0xc0},
    {0x3903, 0x40},
    {0x3909, 0x00},
    {0x391f, 0x41},
    {0x3926, 0xe0},
    {0x3933, 0x80},
    {0x3934, 0x02},
    {0x3937, 0x6f},
    {0x3e00, 0x00},
    {0x3e01, 0x44},
    {0x3e02, 0x10},
    {0x3e08, 0x00},
    {0x4509, 0x20},
    {0x450d, 0x07},
    {0x4837, 0x36},
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
    {0x5792, 0x04},
    {0x5795, 0x04},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x36e9, 0x24},
    {0x37f9, 0x24},
    {REG_END, 0x00},
};

/*
 * Xclk 27Mhz
 * max_framerate 25fps
 * mipi_datarate per lane 742.5Mbps, 2lane
 * full resolution
 */
static const struct sc231hai_sensor_reg sc231hai_linear_1920x1080_60fps_regs[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x02},
    {0x3058, 0x21},
    {0x3059, 0x53},
    {0x305a, 0x40},
    {0x3250, 0x00},
    {0x3301, 0x0a},
    {0x3302, 0x20},
    {0x3304, 0x90},
    {0x3305, 0x00},
    {0x3306, 0x78},
    {0x3309, 0xd0},
    {0x330b, 0xe8},
    {0x330d, 0x08},
    {0x331c, 0x04},
    {0x331e, 0x81},
    {0x331f, 0xc1},
    {0x3323, 0x06},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x3364, 0x5e},
    {0x336c, 0x8c},
    {0x337f, 0x13},
    {0x338f, 0x80},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0xb8},
    {0x3393, 0x0e},
    {0x3394, 0x14},
    {0x3395, 0x10},
    {0x3396, 0x88},
    {0x3397, 0x98},
    {0x3398, 0xf8},
    {0x3399, 0x0a},
    {0x339a, 0x0e},
    {0x339b, 0x10},
    {0x339c, 0x14},
    {0x33ae, 0x80},
    {0x33af, 0xc0},
    {0x33b2, 0x50},
    {0x33b3, 0x08},
    {0x33f8, 0x00},
    {0x33f9, 0x78},
    {0x33fa, 0x00},
    {0x33fb, 0x78},
    {0x33fc, 0x48},
    {0x33fd, 0x78},
    {0x349f, 0x03},
    {0x34a6, 0x40},
    {0x34a7, 0x58},
    {0x34a8, 0x08},
    {0x34a9, 0x0c},
    {0x34f8, 0x78},
    {0x34f9, 0x18},
    {0x3619, 0x20},
    {0x361a, 0x90},
    {0x3633, 0x44},
    {0x3637, 0x5c},
    {0x363c, 0xc0},
    {0x363d, 0x02},
    {0x3660, 0x80},
    {0x3661, 0x81},
    {0x3662, 0x8f},
    {0x3663, 0x81},
    {0x3664, 0x81},
    {0x3665, 0x82},
    {0x3666, 0x8f},
    {0x3667, 0x08},
    {0x3668, 0x80},
    {0x3669, 0x88},
    {0x366a, 0x98},
    {0x366b, 0xb8},
    {0x366c, 0xf8},
    {0x3670, 0xc2},
    {0x3671, 0xc2},
    {0x3672, 0x98},
    {0x3680, 0x43},
    {0x3681, 0x54},
    {0x3682, 0x54},
    {0x36c0, 0x80},
    {0x36c1, 0x88},
    {0x36c8, 0x88},
    {0x36c9, 0xb8},
    {0x3718, 0x04},
    {0x3722, 0x8b},
    {0x3724, 0xd1},
    {0x3741, 0x08},
    {0x3770, 0x17},
    {0x3771, 0x9b},
    {0x3772, 0x9b},
    {0x37c0, 0x88},
    {0x37c1, 0xb8},
    {0x3902, 0xc0},
    {0x3903, 0x40},
    {0x3909, 0x00},
    {0x391f, 0x41},
    {0x3926, 0xe0},
    {0x3933, 0x80},
    {0x3934, 0x02},
    {0x3937, 0x6f},
    {0x3e00, 0x00},
    {0x3e01, 0x8b},
    {0x3e02, 0xf0},
    {0x3e08, 0x00},
    {0x4509, 0x20},
    {0x450d, 0x07},
    {0x5780, 0x76},
    {0x5784, 0x10},
    {0x5787, 0x0a},
    {0x5788, 0x0a},
    {0x5789, 0x08},
    {0x578a, 0x0a},
    {0x578b, 0x0a},
    {0x578c, 0x08},
    {0x578d, 0x40},
    {0x5792, 0x04},
    {0x5795, 0x04},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x36e9, 0x24},
    {0x37f9, 0x24},
    {REG_END, 0x00},
};
#endif

static const struct sc231hai_mode supported_modes[] =
{
    [SC231HAI_640X480] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 640,
        .height = 480,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0232,
        .xvclk = 27000000,
        .link_freq = 371250000,
        .reg_list = sc231hai_linear_640x480_120fps_regs,
        .reg_list_size = ARRAY_SIZE(sc231hai_linear_640x480_120fps_regs),
        .hdr_mode = NO_HDR,
    },
    [SC231HAI_1920X1080_60FPS] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0465,
        .xvclk = 27000000,
        .link_freq = 371250000,
        .reg_list = sc231hai_linear_1920x1080_60fps_regs,
        .reg_list_size = ARRAY_SIZE(sc231hai_linear_1920x1080_60fps_regs),
        .hdr_mode = NO_HDR,
    }
};

struct sc231hai_ops
{
    ret_err_t (*init)(struct sc231hai_dev *dev);
    ret_err_t (*open)(struct sc231hai_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc231hai_dev *dev);
    ret_err_t (*control)(struct sc231hai_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc231hai_read_reg(struct sc231hai_dev *dev,
                                  uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    int retry = 10;
    ret_err_t ret;

    RT_ASSERT(dev->i2c_bus != RT_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    msg[0].addr = dev->i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = dev->i2c_addr;
    msg[1].flags = RT_I2C_RD;
    msg[1].len = 1;
    msg[1].buf = recv_buf;

    while (1)
    {
        ret = rt_i2c_transfer(dev->i2c_bus, msg, 2);
        if (ret == 2)
        {
            *data = recv_buf[0];
            return RET_SYS_EOK;
        }
        else
        {
            sc231hai_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }

    return ret;
}

static ret_err_t sc231hai_write_reg(struct sc231hai_dev *dev,
                                   uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;
    RT_ASSERT(dev->i2c_bus != RT_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    send_buf[2] = data;

    msgs.addr = dev->i2c_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = send_buf;
    msgs.len = 3;

    while (1)
    {
        ret = rt_i2c_transfer(dev->i2c_bus, &msgs, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            sc231hai_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG

static int sc231hai_write_multiple_reg_continue(struct sc231hai_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msgs;
    int ret = 0;
    int offset = 0;
    int retry = 10;

    RT_ASSERT(dev != RT_NULL && i2c_data != RT_NULL);

    i2c_bus = dev->i2c_bus;
    RT_ASSERT(i2c_bus != RT_NULL);

    for (i = 0; i < i2c_data[len - 1];)
    {
        msgs.addr = dev->i2c_addr;
        msgs.flags = RT_I2C_WR;
        msgs.buf = (uint8_t *)&i2c_data[offset + 1];
        msgs.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msgs, 1);

        if (ret != 1)
        {
            sc231hai_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t sc231hai_write_reg_discontinued(struct sc231hai_dev *dev,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(dev->i2c_bus != RT_NULL);

    msgs.addr = dev->i2c_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = data;
    msgs.len = len;

    while (1)
    {
        ret = rt_i2c_transfer(dev->i2c_bus, &msgs, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            sc231hai_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc231hai_write_multiple_reg(struct sc231hai_dev *dev,
                                       const struct sc231hai_sensor_reg *reg, int len)
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
                sc231hai_write_reg_discontinued(dev, data, k + 2);
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
                sc231hai_write_reg_discontinued(dev, data, k + 2);
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
                    sc231hai_write_reg_discontinued(dev, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc231hai_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc231hai_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        sc231hai_write_multiple_reg((struct sc231hai_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

/* mode: 0 = lgain  1 = sgain */
static int sc231hai_set_gain_reg(struct sc231hai_dev *dev, uint32_t gain, int mode)
{
    uint8_t Coarse_gain = 1, DIG_gain = 1;
    uint32_t Dcg_gainx100 = 1, ANA_Fine_gainx64 = 1;
    uint8_t Coarse_gain_reg = 0, DIG_gain_reg = 0;
    uint8_t ANA_Fine_gain_reg = 0x20, DIG_Fine_gain_reg = 0x80;
    int ret = 0;

    gain = gain * 32;
    if (gain < SC231HAI_GAIN_MIN * 32)
        gain = SC231HAI_GAIN_MIN;
    else if (gain > SC231HAI_GAIN_MAX * 32)
        gain = SC231HAI_GAIN_MAX * 32;

    if (gain < 2 * 1024) {        /* Start again  1.0x ~ 2.0x */
        Dcg_gainx100 = 100;
        Coarse_gain = 1; DIG_gain = 1;
        Coarse_gain_reg = 0x00;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 3776) {    /* 2.0x ~ 3.7x  1024 * 3.688 = 3776*/
        Dcg_gainx100 = 100;
        Coarse_gain = 2; DIG_gain = 1;
        Coarse_gain_reg = 0x01;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 7458) {    /* 3.7x ~ 7.4x  1024 * 7.284 = 7458 */
        Dcg_gainx100 = 100;
        Coarse_gain = 1; DIG_gain = 1;
        Coarse_gain_reg = 0x80;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 14946) {   /* 7.4x ~ 14.8x  1024 * 14.596 = 14946*/
        Dcg_gainx100 = 740;
        Coarse_gain = 2; DIG_gain = 1;
        Coarse_gain_reg = 0x81;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 29837) {   /* 14.8x ~ 29.6x  1024 * 29.138 = 29837*/
        Dcg_gainx100 = 740;
        Coarse_gain = 4; DIG_gain = 1;
        Coarse_gain_reg = 0x83;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 59673) {   /* 29.6x ~ 59.2x  1024 * 58.275 = 59673*/
        Dcg_gainx100 = 740;
        Coarse_gain = 8; DIG_gain = 1;
        Coarse_gain_reg = 0x87;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 119347) {
        /* End again 59.2x ~ 116.55x  1024 * 116.55 = 119347*/
        Dcg_gainx100 = 740;
        Coarse_gain = 16; DIG_gain = 1;
        Coarse_gain_reg = 0x8f;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 119347 * 2) {  /* Start dgain 1.0x ~ 2.0x */
        Dcg_gainx100 = 740;
        Coarse_gain = 16; DIG_gain = 1;
        Coarse_gain_reg = 0x8f; ANA_Fine_gain_reg = 0x3f;
        DIG_gain_reg = 0x0; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 119347 * 4) {  /* 2.0x ~ 4.0x */
        Dcg_gainx100 = 740;
        Coarse_gain = 16; DIG_gain = 2;
        Coarse_gain_reg = 0x8f; ANA_Fine_gain_reg = 0x3f;
        DIG_gain_reg = 0x1; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 119347 * 8) {  /* 4.0x ~ 8.0x */
        Dcg_gainx100 = 740;
        Coarse_gain = 16; DIG_gain = 4;
        Coarse_gain_reg = 0x8f; ANA_Fine_gain_reg = 0x3f;
        DIG_gain_reg = 0x3; DIG_Fine_gain_reg = 0x80;
    } else if (gain <= 1894633) { /* End dgain 8.0x ~ 15.875x 119347*15.875=1894633*/
        Dcg_gainx100 = 740;
        Coarse_gain = 16; DIG_gain = 8;
        Coarse_gain_reg = 0x8f; ANA_Fine_gain_reg = 0x3f;
        DIG_gain_reg = 0x7; DIG_Fine_gain_reg = 0x80;
    }

    if (gain < 3776)
        ANA_Fine_gain_reg = abs(100 * gain / (Dcg_gainx100 * Coarse_gain) / 32);
    else if (gain == 3776)    /* 3.688x */
        ANA_Fine_gain_reg = 0x3B;
    else if (gain < 119347)   /* again */
        ANA_Fine_gain_reg = abs(100 * gain / (Dcg_gainx100 * Coarse_gain) / 32);
    else                /* dgain */
        DIG_Fine_gain_reg = abs(800 * gain / (Dcg_gainx100 * Coarse_gain *
                    DIG_gain) / ANA_Fine_gainx64);

    if (mode == SC231HAI_LGAIN) {	// normal
        ret = sc231hai_write_reg(dev, SC231HAI_REG_DIG_GAIN,
                                 DIG_gain_reg & 0xF);
        ret |= sc231hai_write_reg(dev, SC231HAI_REG_DIG_FINE_GAIN,
                                  DIG_Fine_gain_reg);
        ret |= sc231hai_write_reg(dev, SC231HAI_REG_ANA_GAIN,
                                  Coarse_gain_reg);
        ret |= sc231hai_write_reg(dev, SC231HAI_REG_ANA_FINE_GAIN,
                                  ANA_Fine_gain_reg);
    } else { // hdrx2
        ret = sc231hai_write_reg(dev, SC231HAI_REG_SDIG_GAIN,
                                  DIG_gain_reg & 0xF);
        ret |= sc231hai_write_reg(dev, SC231HAI_REG_SDIG_FINE_GAIN,
                                  DIG_Fine_gain_reg);
        ret |= sc231hai_write_reg(dev, SC231HAI_REG_SANA_GAIN,
                                  Coarse_gain_reg);
        ret |= sc231hai_write_reg(dev, SC231HAI_REG_SANA_FINE_GAIN,
                                  ANA_Fine_gain_reg);
    }

    return ret;
}

static ret_err_t rk_sc231hai_set_expval(struct sc231hai_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
	uint8_t s_max_exp_time_l = 0, s_max_exp_time_h = 0;
    uint16_t s_max_exp_time;
    uint32_t l_exp_time = 0, s_exp_time = 0;
    uint32_t l_a_gain = 0, s_a_gain = 0;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming && strcmp(dev->name, RK_ISP_ASSIST_SUBDEV_NAME))
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc231hai_dbg("sc231hai don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    if (dev->cur_mode->hdr_mode == NO_HDR)
    {
        l_exp_time = exp->reg_time[0] * 2;
        l_a_gain = exp->reg_gain[0];
        s_exp_time = 0;
        s_a_gain = 0;
    }
    else if (dev->cur_mode->hdr_mode == HDR_X2)
    {
        l_exp_time = exp->reg_time[1] * 2;
        s_exp_time = exp->reg_time[0] * 2;
        l_a_gain = exp->reg_gain[1];
        s_a_gain = exp->reg_gain[0];
    }

    sc231hai_dbg("sc231hai: exp req: L_exp: 0x%x, 0x%x, S_exp: 0x%x, 0x%x\n",
                l_exp_time, l_a_gain, s_exp_time, s_a_gain);

    ret = sc231hai_read_reg(dev, SC231HAI_REG_MAX_SEXPOSURE_H, &s_max_exp_time_h);
    ret = sc231hai_read_reg(dev, SC231HAI_REG_MAX_SEXPOSURE_L, &s_max_exp_time_l);
    s_max_exp_time = (s_max_exp_time_h << 8) | s_max_exp_time_l;

    //set exposure
    if (dev->cur_mode->hdr_mode == NO_HDR)
    {
        if (l_exp_time > 2 * dev->cur_mode->vts_def - 8)
            l_exp_time = 2 * dev->cur_mode->vts_def - 8;
    }
    else if (dev->cur_mode->hdr_mode == HDR_X2)
    {
        if (l_exp_time > 2 * dev->cur_mode->vts_def - 2 * s_max_exp_time - 10)
            l_exp_time = 2 * dev->cur_mode->vts_def - 2 * s_max_exp_time - 10;
        if (s_exp_time > 2 * s_max_exp_time - 10)
            s_exp_time = 2 * s_max_exp_time - 10;
    }

    ret  = sc231hai_write_reg(dev, SC231HAI_REG_EXP_LONG_H,
                             SC231HAI_FETCH_EXP_H(l_exp_time));
    ret  = sc231hai_write_reg(dev, SC231HAI_REG_EXP_LONG_M,
                             SC231HAI_FETCH_EXP_M(l_exp_time));
    ret  = sc231hai_write_reg(dev, SC231HAI_REG_EXP_LONG_L,
                             SC231HAI_FETCH_EXP_L(l_exp_time));

    if (dev->cur_mode->hdr_mode == HDR_X2)
    {
        ret = sc231hai_write_reg(dev, SC231HAI_REG_EXP_LONG_M,
                                SC231HAI_FETCH_EXP_M(s_exp_time));
        ret = sc231hai_write_reg(dev, SC231HAI_REG_EXP_LONG_L,
                                SC231HAI_FETCH_EXP_L(s_exp_time));
    }
    ret = sc231hai_set_gain_reg(dev, l_a_gain, SC231HAI_LGAIN);
    if (dev->cur_mode->hdr_mode == HDR_X2)
        ret |= sc231hai_set_gain_reg(dev, s_a_gain, SC231HAI_SGAIN);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc231hai_set_vts(struct sc231hai_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc231hai_dbg("(%s) set vts: 0x%x \n", __FUNCTION__, dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc231hai_write_reg(dev, SC231HAI_REG_VTS_L,
                            (uint8_t)(dst_vts & 0xff));
    ret |= sc231hai_write_reg(dev, SC231HAI_REG_VTS_H,
                             (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc231hai_set_flip_mirror(struct sc231hai_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc231hai_read_reg(dev, SC231HAI_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC231HAI_FETCH_MIRROR(val, flip & 0x01);
        val = SC231HAI_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC231HAI_FETCH_MIRROR(val, false);
        val = SC231HAI_FETCH_FLIP(val, false);
        break;
    };
    sc231hai_dbg("(%s) flip 0x%x, reg val 0x%x\n", __FUNCTION__, flip, val);
    ret |= sc231hai_write_reg(dev, SC231HAI_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc231hai_stream_on(struct sc231hai_dev *dev)
{

    sc231hai_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    sc231hai_write_multiple_reg_continue((struct sc231hai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc231hai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc231hai_set_flip_mirror(dev, dev->flip);

    sc231hai_write_reg(dev, SC231HAI_REG_STREAM_CTRL, SC231HAI_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc231hai_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc231hai_stream_on_late(struct sc231hai_dev *dev)
{
    sc231hai_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(5);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc231hai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc231hai_set_flip_mirror(dev, dev->flip);

    sc231hai_write_reg(dev, SC231HAI_REG_STREAM_CTRL, SC231HAI_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc231hai_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc231hai_stream_off(struct sc231hai_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
        sc231hai_write_reg(dev, SC231HAI_REG_STREAM_CTRL, SC231HAI_STREAM_OFF);
        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc231hai_err("not find out i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_sc231hai_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc231hai_dev *sc231hai;

	RT_ASSERT(dev != RT_NULL);
    sc231hai = (struct sc231hai_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc231hai->parent;
#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc231hai);
#endif

    if (sc231hai)
    {
        camera->info.mbus_fmt.width = sc231hai->cur_mode->width;
        camera->info.mbus_fmt.height = sc231hai->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc231hai->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode = sc231hai->cur_mode->hdr_mode;

        sc231hai->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc231hai->i2c_name);

        if (!sc231hai->i2c_bus)
        {
            sc231hai_err("not find i2c source 2:%s !!!\n",
                        sc231hai->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc231hai_dbg("s0 find i2c_bus:%s\n", sc231hai->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_sc231hai_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc231hai_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_sc231hai_get_expinf(struct sc231hai_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc231hai_mode *mode;

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

static ret_err_t rk_sc231hai_get_intput_fmt(struct sc231hai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc231hai_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc231hai_set_intput_fmt(struct sc231hai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc231hai_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc231hai_info("dst resulotion, width %d, height %d\n",
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
        if (mode->width != dev->cur_mode->width || !strcmp(dev->name, RK_ISP_ASSIST_SUBDEV_NAME))
        {
            dev->cur_mode = mode;
            sc231hai_info("switch to dst fmt, dst_width %d, dst_height %d dst_fps %d, hdr: %d, dst_vts: 0x%x\n",
                         dev->cur_mode->width, dev->cur_mode->height,
                         dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator,
                         dev->cur_mode->hdr_mode, dev->cur_mode->vts_def
                         );
            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
            sc231hai_write_multiple_reg_continue((struct sc231hai_dev *)dev,
                                                dev->cur_mode->reg_list,
                                                dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc231hai_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc231hai_match_dst_config(struct sc231hai_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc231hai_mode *mode;
    int i, cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc231hai_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height,
                 dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc231hai_mode *)mode;
            ret = RET_SYS_EOK;
            sc231hai_info("find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc231hai_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return RT_EOK;

    if (dst_fps > cur_fps)
    {
        sc231hai_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }
    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc231hai_control(struct rk_camera_device *dev,
                             int cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc231hai_dev *sc231hai;

    RT_ASSERT(dev != RT_NULL);
    sc231hai = (struct sc231hai_dev *)dev;

    switch (cmd)
    {
    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc231hai_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc231hai_stream_on(sc231hai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc231hai_stream_off(sc231hai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc231hai_get_expinf(sc231hai, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc231hai_set_expval(sc231hai, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc231hai_set_vts(sc231hai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc231hai_get_intput_fmt(sc231hai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc231hai_set_intput_fmt(sc231hai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc231hai_match_dst_config(sc231hai, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc231hai_set_flip_mirror(sc231hai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc231hai_stream_on_late(sc231hai);
    }
    break;
    default:
        sc231hai_dbg("(%s) exit CMD %d\n", __FUNCTION__, cmd);
        break;
    }
    return ret;
}

struct rk_camera_ops rk_sc231hai_ops =
{
    rk_sc231hai_init,
    rk_sc231hai_open,
    NULL,
    rk_sc231hai_control
};

int rk_camera_sc231hai_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct camera_board_desc *sc231hai_desc;

    for(sc231hai_desc = camera_sc231hai; sc231hai_desc->i2c_addr != 0; sc231hai_desc++){
        struct sc231hai_dev *instance;
        struct rk_camera_device *camera;

        instance = rt_malloc(sizeof(struct sc231hai_dev));
        if(!instance){
            rt_kprintf("malloc sc231hai failed\n");
            return -RT_ENOMEM;
        }
        rt_memset(instance, 0x0, sizeof(struct sc231hai_dev));
        camera = &instance->parent;
        camera->ops = &rk_sc231hai_ops;

        //struct camera_board_desc *sc231hai = (struct camera_board_desc *)&camera_sc231hai;
        camera->ops = &rk_sc231hai_ops;

        rt_strncpy(instance->name, sc231hai_desc->isp_subdev_name, RK_CAMERA_DEVICE_NAME_SIZE);
        rt_strncpy(instance->i2c_name, sc231hai_desc->i2c_bus, RK_CAMERA_I2C_NAME_SIZE);

        instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
		if (sc231hai_desc->i2c_bus == RT_NULL)
        {
            sc231hai_err("can't find i2c bus:%s\n", instance->i2c_name);
            rt_free(instance);
            continue;
        }
        if (sc231hai_desc->mode_id <= SC231HAI_MODE_ID_MAX)
        {
            sc231hai_dbg("mode_id: %d\n", sc231hai_desc->mode_id);
            instance->cur_mode = &supported_modes[sc231hai_desc->mode_id];
        }
        else
        {
            sc231hai_info("mode id is over range, default use mode_id: 0\n");
            instance->cur_mode = &supported_modes[0];
        }

        instance->time_valid_delay = sc231hai_desc->time_valid_delay;
        instance->gain_valid_delay = sc231hai_desc->gain_valid_delay;
        instance->i2c_addr = sc231hai_desc->i2c_addr;
        instance->flip = 0;

        if (sc231hai_desc->rst_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc231hai_desc->rst_gpio.gpio_group,
                                    sc231hai_desc->rst_gpio.gpio_pin,
                                    GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc231hai_desc->rst_gpio.gpio_group,
                                sc231hai_desc->rst_gpio.gpio_pin,
                                !sc231hai_desc->rst_active);
        }

        if(sc231hai_desc->pwdn_gpio.gpio_group){
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc231hai_desc->pwdn_gpio.gpio_group,
                                    sc231hai_desc->pwdn_gpio.gpio_pin,
                                    GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc231hai_desc->pwdn_gpio.gpio_group,
                                sc231hai_desc->pwdn_gpio.gpio_pin,
                                !sc231hai_desc->pwdn_active);
        }
        if (sc231hai_desc->mclk_out_gate_id && sc231hai_desc->mclk_id)
        {
            clk_set_rate(sc231hai_desc->mclk_id, instance->cur_mode->xvclk);
            HAL_CRU_ClkEnable(sc231hai_desc->mclk_out_gate_id);
        }

        rt_mutex_init(&instance->mutex_lock, "SC231HAI_mutex", RT_IPC_FLAG_FIFO);
        RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
        camera->i2c_bus = instance->i2c_bus;
        rt_strncpy(camera->name, sc231hai_desc->isp_subdev_name, rt_strlen(sc231hai_desc->isp_subdev_name));
        ret = rk_camera_register(camera, camera->name, instance);
        if (ret)
        {
            sc231hai_err("regster failed\n");
            rt_free(instance);
        }
    }
    return ret;
}

void sc231hai_detect(void)
{
    const struct camera_board_desc *sc231hai_desc;
    struct rt_i2c_bus_device *i2c_bus;
    for(sc231hai_desc = camera_sc231hai; sc231hai_desc->i2c_addr != 0; sc231hai_desc++){
        sc231hai_dbg("start to detect SC231HAI for testing \n");
        sc231hai_dbg("dev name:%s\n", sc231hai_desc->isp_subdev_name);
        sc231hai_dbg("dev i2c_bus:%s\n", sc231hai_desc->i2c_bus);
        i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc231hai_desc->i2c_bus);
        if (i2c_bus)
        {
            sc231hai_err("Warning:not find i2c source 1:%s !!!\n", sc231hai_desc->i2c_bus);
            return;
        }
        else
        {
            sc231hai_dbg("find i2c_bus:%s\n", sc231hai_desc->i2c_bus);
        }
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc231hai_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc231hai_detect, check SC231HAI is available or not);
#endif
#endif
#endif
