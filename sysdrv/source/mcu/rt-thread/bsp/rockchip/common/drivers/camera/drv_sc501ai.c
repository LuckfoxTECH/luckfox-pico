/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc501ai.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2022-12-04     Yuefu Su      first implementation
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
#include "board.h"

#ifdef RT_USING_SC501AI

//#define REG_LIST_CONVERSION_DEBUG
#define SC501AI_DEBUG_PRT      0

#if SC501AI_DEBUG_PRT
#include <stdio.h>
#define sc501ai_dbg(fmt, args...)               rt_kprintf("[SC501AI] "fmt"", ##args)
#else
#define sc501ai_dbg(fmt, args...)
#endif

#define sc501ai_info(fmt, args...)              rt_kprintf("[SC501AI] "fmt"", ##args)
#define sc501ai_err(fmt, args...)               rt_kprintf("[SC501AI] ERR "fmt"", ##args)

#define SC501AI_REG_EXP_LONG_H                  0x3e00
#define SC501AI_REG_EXP_LONG_M                  0x3e01
#define SC501AI_REG_EXP_LONG_L                  0x3e02
#define SC501AI_REG_EXP_SHORT_H                 0x3e22
#define SC501AI_REG_EXP_SHORT_M                 0x3e04
#define SC501AI_REG_EXP_SHORT_L                 0x3e05
#define SC501AI_REG_DIG_GAIN                    0x3e06
#define SC501AI_REG_DIG_FINE_GAIN               0x3e07
#define SC501AI_REG_ANA_GAIN                    0x3e08
#define SC501AI_REG_ANA_FINE_GAIN               0x3e09
#define SC501AI_EXP_MIN                         3
#define SC501AI_GAIN_MIN                        0x0080
#define SC501AI_GAIN_MAX                        (97809)  //24.067 * 31.75 * 128
#define SC501AI_GAIN_STEP                       1
#define SC501AI_GAIN_DEFAULT                    0x0080
#define SC501AI_FLIP_MIRROR_REG                 0x3221
#define SC501AI_REG_VTS_H                       0x320e
#define SC501AI_REG_VTS_L                       0x320f
#define SC501AI_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC501AI_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC501AI_REG_STREAM_CTRL                 0x0100
#define SC501AI_STREAM_ON                       0x01
#define SC501AI_STREAM_OFF                      0x00

#define REG_END                                 (0x0)
#define REG_DELAY                               (0xff)

#define ARRAY_SIZE(x)                           (sizeof(x) / sizeof((x)[0]))

/* redefine system err code */
#define RET_SYS_EOK                             (RT_EOK)
#define RET_SYS_ERROR                           (-RT_ERROR)
#define RET_SYS_ETIMEOUT                        (-RT_ETIMEOUT)  /**< Timed out */
#define RET_SYS_EFULL                           (-RT_EFULL)     /**< The resource is full */
#define RET_SYS_EEMPTY                          (-RT_EEMPTY)    /**< The resource is empty */
#define RET_SYS_ENOMEM                          (-RT_ENOMEM)    /**< No memory */
#define RET_SYS_ENOSYS                          (-RT_ENOSYS)    /**< No system */
#define RET_SYS_EBUSY                           (-RT_EBUSY)     /**< Busy */
#define RET_SYS_EIO                             (-RT_EIO)       /**< IO error */
#define RET_SYS_EINTR                           (-RT_EINTR)     /**< Interrupted system call */
#define RET_SYS_EINVAL                          (-RT_EINVAL)    /**< Invalid argument */

struct rt_i2c_bus_device;

struct sc501ai_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc501ai_mode
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
    const struct sc501ai_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc501ai_dev
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
    const struct sc501ai_mode *cur_mode;
    struct sc501ai_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc501ai_dev *rt_sc501ai_dev_t;

static struct sc501ai_dev g_sc501ai;

#ifndef REG_LIST_CONVERSION_DEBUG
static const uint8_t g_sc501ai_1440x810_60fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x4, 0x30, 0x18, 0x32, 0x0c,
    0x3, 0x30, 0x1f, 0xca,
    0xf, 0x32, 0x07, 0x5b, 0x05, 0xa0, 0x03, 0x2a, 0x06, 0x2c, 0x03, 0x43, 0x00, 0x02, 0x00, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x17,
    0x3, 0x32, 0x53, 0x0a,
    0x6, 0x33, 0x01, 0x0a, 0x18, 0x10, 0x60,
    0x3, 0x33, 0x06, 0x60,
    0x6, 0x33, 0x08, 0x10, 0x70, 0x00, 0xf0,
    0x6, 0x33, 0x0d, 0x18, 0x20, 0x02, 0x02,
    0x3, 0x33, 0x1c, 0x04,
    0x5, 0x33, 0x1e, 0x51, 0x61, 0x09,
    0x3, 0x33, 0x33, 0x10,
    0x3, 0x33, 0x4c, 0x08,
    0x3, 0x33, 0x56, 0x09,
    0x3, 0x33, 0x64, 0x17,
    0x3, 0x33, 0x6d, 0x03,
    0xf, 0x33, 0x90, 0x08, 0x18, 0x38, 0x0a, 0x20, 0x20, 0x08, 0x18, 0x38, 0x0a, 0x20, 0x20, 0x20,
    0x3, 0x33, 0xac, 0x10,
    0x4, 0x33, 0xae, 0x10, 0x19,
    0x3, 0x36, 0x0f, 0x01,
    0x3, 0x36, 0x22, 0x03,
    0x3, 0x36, 0x3a, 0x1f,
    0x3, 0x36, 0x3c, 0x40,
    0x3, 0x36, 0x51, 0x7d,
    0x9, 0x36, 0x70, 0x0a, 0x07, 0x17, 0x1e, 0x82, 0x64, 0x66,
    0x6, 0x36, 0x7a, 0x48, 0x78, 0x58, 0x78,
    0x5, 0x36, 0x90, 0x34, 0x34, 0x54,
    0x4, 0x36, 0x9c, 0x48, 0x78,
    0x3, 0x36, 0xec, 0x1a,
    0x3, 0x39, 0x04, 0x04,
    0x3, 0x39, 0x08, 0x41,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0xc2, 0x30,
    0x4, 0x3e, 0x01, 0x67, 0xc0,
    0x4, 0x3e, 0x16, 0x00, 0x80,
    0x3, 0x45, 0x00, 0x88,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x50, 0x00, 0x46,
    0x3, 0x57, 0x99, 0x00,
    0x4, 0x59, 0x00, 0xf1, 0x04,
    0x8, 0x59, 0xe0, 0x60, 0x08, 0x3f, 0x18, 0x18, 0x3f,
    0x6, 0x59, 0xe7, 0x02, 0x38, 0x20, 0x0c,
    0x6, 0x59, 0xec, 0x08, 0x02, 0xa0, 0x08,
    0x5, 0x59, 0xf4, 0x18, 0x10, 0x0c,
    0x6, 0x59, 0xf9, 0x02, 0x18, 0x10, 0x0c,
    0x3, 0x59, 0xff, 0x02,
    0x3, 0x36, 0xe9, 0x1c,
    0x3, 0x36, 0xf9, 0x24,
    0x35,
};
static const uint8_t g_sc501ai_2880x1616_30fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x4, 0x30, 0x18, 0x32, 0x0c,
    0x3, 0x30, 0x1f, 0x0b,
    0x3, 0x32, 0x03, 0x02,
    0x3, 0x32, 0x07, 0x59,
    0x3, 0x32, 0x0b, 0x50,
    0x3, 0x32, 0x53, 0x0a,
    0x6, 0x33, 0x01, 0x0a, 0x18, 0x10, 0x60,
    0x3, 0x33, 0x06, 0x60,
    0x6, 0x33, 0x08, 0x10, 0x70, 0x00, 0xf0,
    0x6, 0x33, 0x0d, 0x18, 0x20, 0x02, 0x02,
    0x3, 0x33, 0x1c, 0x04,
    0x5, 0x33, 0x1e, 0x51, 0x61, 0x09,
    0x3, 0x33, 0x33, 0x10,
    0x3, 0x33, 0x4c, 0x08,
    0x3, 0x33, 0x56, 0x09,
    0x3, 0x33, 0x64, 0x17,
    0x3, 0x33, 0x6d, 0x03,
    0xf, 0x33, 0x90, 0x08, 0x18, 0x38, 0x0a, 0x20, 0x20, 0x08, 0x18, 0x38, 0x0a, 0x20, 0x20, 0x20,
    0x3, 0x33, 0xac, 0x10,
    0x4, 0x33, 0xae, 0x10, 0x19,
    0x3, 0x36, 0x0f, 0x01,
    0x3, 0x36, 0x22, 0x03,
    0x3, 0x36, 0x3a, 0x1f,
    0x3, 0x36, 0x3c, 0x40,
    0x3, 0x36, 0x51, 0x7d,
    0x9, 0x36, 0x70, 0x0a, 0x07, 0x17, 0x1e, 0x82, 0x64, 0x66,
    0x6, 0x36, 0x7a, 0x48, 0x78, 0x58, 0x78,
    0x5, 0x36, 0x90, 0x34, 0x34, 0x54,
    0x4, 0x36, 0x9c, 0x48, 0x78,
    0x3, 0x36, 0xec, 0x0a,
    0x3, 0x39, 0x04, 0x04,
    0x3, 0x39, 0x08, 0x41,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0xc2, 0x30,
    0x4, 0x3e, 0x01, 0xcd, 0xc0,
    0x4, 0x3e, 0x16, 0x00, 0x80,
    0x3, 0x45, 0x00, 0x88,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x48, 0x37, 0x14,
    0x3, 0x57, 0x99, 0x00,
    0x8, 0x59, 0xe0, 0x60, 0x08, 0x3f, 0x18, 0x18, 0x3f,
    0x6, 0x59, 0xe7, 0x02, 0x38, 0x20, 0x0c,
    0x6, 0x59, 0xec, 0x08, 0x02, 0xa0, 0x08,
    0x5, 0x59, 0xf4, 0x18, 0x10, 0x0c,
    0x6, 0x59, 0xf9, 0x02, 0x18, 0x10, 0x0c,
    0x3, 0x59, 0xff, 0x02,
    0x3, 0x36, 0xe9, 0x1c,
    0x3, 0x36, 0xf9, 0x24,
    0x33,

};
#else
/*
 * Xclk 27Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 396Mbps, 2lane
 * binning to 1440x810
 */
static const struct sc501ai_sensor_reg g_sc501ai_1440x810_60fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x36f9, 0x80},
    {0x3018, 0x32},
    {0x3019, 0x0c},
    {0x301f, 0xca},
    {0x3207, 0x5b},
    {0x3208, 0x05},
    {0x3209, 0xa0},
    {0x320a, 0x03},
    {0x320b, 0x2a},
    {0x320c, 0x06},
    {0x320d, 0x2c},
    {0x320e, 0x03},
    {0x320f, 0x43},
    {0x3210, 0x00},
    {0x3211, 0x02},
    {0x3212, 0x00},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x17},
    {0x3253, 0x0a},
    {0x3301, 0x0a},
    {0x3302, 0x18},
    {0x3303, 0x10},
    {0x3304, 0x60},
    {0x3306, 0x60},
    {0x3308, 0x10},
    {0x3309, 0x70},
    {0x330a, 0x00},
    {0x330b, 0xf0},
    {0x330d, 0x18},
    {0x330e, 0x20},
    {0x330f, 0x02},
    {0x3310, 0x02},
    {0x331c, 0x04},
    {0x331e, 0x51},
    {0x331f, 0x61},
    {0x3320, 0x09},
    {0x3333, 0x10},
    {0x334c, 0x08},
    {0x3356, 0x09},
    {0x3364, 0x17},
    {0x336d, 0x03},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0x38},
    {0x3393, 0x0a},
    {0x3394, 0x20},
    {0x3395, 0x20},
    {0x3396, 0x08},
    {0x3397, 0x18},
    {0x3398, 0x38},
    {0x3399, 0x0a},
    {0x339a, 0x20},
    {0x339b, 0x20},
    {0x339c, 0x20},
    {0x33ac, 0x10},
    {0x33ae, 0x10},
    {0x33af, 0x19},
    {0x360f, 0x01},
    {0x3622, 0x03},
    {0x363a, 0x1f},
    {0x363c, 0x40},
    {0x3651, 0x7d},
    {0x3670, 0x0a},
    {0x3671, 0x07},
    {0x3672, 0x17},
    {0x3673, 0x1e},
    {0x3674, 0x82},
    {0x3675, 0x64},
    {0x3676, 0x66},
    {0x367a, 0x48},
    {0x367b, 0x78},
    {0x367c, 0x58},
    {0x367d, 0x78},
    {0x3690, 0x34},
    {0x3691, 0x34},
    {0x3692, 0x54},
    {0x369c, 0x48},
    {0x369d, 0x78},
    {0x36ec, 0x1a},
    {0x3904, 0x04},
    {0x3908, 0x41},
    {0x391d, 0x04},
    {0x39c2, 0x30},
    {0x3e01, 0x67},
    {0x3e02, 0xc0},
    {0x3e16, 0x00},
    {0x3e17, 0x80},
    {0x4500, 0x88},
    {0x4509, 0x20},
    {0x5000, 0x46},
    {0x5799, 0x00},
    {0x5900, 0xf1},
    {0x5901, 0x04},
    {0x59e0, 0x60},
    {0x59e1, 0x08},
    {0x59e2, 0x3f},
    {0x59e3, 0x18},
    {0x59e4, 0x18},
    {0x59e5, 0x3f},
    {0x59e7, 0x02},
    {0x59e8, 0x38},
    {0x59e9, 0x20},
    {0x59ea, 0x0c},
    {0x59ec, 0x08},
    {0x59ed, 0x02},
    {0x59ee, 0xa0},
    {0x59ef, 0x08},
    {0x59f4, 0x18},
    {0x59f5, 0x10},
    {0x59f6, 0x0c},
    {0x59f9, 0x02},
    {0x59fa, 0x18},
    {0x59fb, 0x10},
    {0x59fc, 0x0c},
    {0x59ff, 0x02},
    {0x36e9, 0x1c},
    {0x36f9, 0x24},
    {REG_END, 0x00},
};

/*
 * Xclk 27Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 792Mbps, 2lane
 * full resolution
 */
static const struct sc501ai_sensor_reg g_sc501ai_2880x1616_30fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x36f9, 0x80},
    {0x3018, 0x32},
    {0x3019, 0x0c},
    {0x301f, 0x0b},
    {0x3203, 0x02},
    {0x3207, 0x59},
    {0x320b, 0x50},
    {0x3253, 0x0a},
    {0x3301, 0x0a},
    {0x3302, 0x18},
    {0x3303, 0x10},
    {0x3304, 0x60},
    {0x3306, 0x60},
    {0x3308, 0x10},
    {0x3309, 0x70},
    {0x330a, 0x00},
    {0x330b, 0xf0},
    {0x330d, 0x18},
    {0x330e, 0x20},
    {0x330f, 0x02},
    {0x3310, 0x02},
    {0x331c, 0x04},
    {0x331e, 0x51},
    {0x331f, 0x61},
    {0x3320, 0x09},
    {0x3333, 0x10},
    {0x334c, 0x08},
    {0x3356, 0x09},
    {0x3364, 0x17},
    {0x336d, 0x03},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0x38},
    {0x3393, 0x0a},
    {0x3394, 0x20},
    {0x3395, 0x20},
    {0x3396, 0x08},
    {0x3397, 0x18},
    {0x3398, 0x38},
    {0x3399, 0x0a},
    {0x339a, 0x20},
    {0x339b, 0x20},
    {0x339c, 0x20},
    {0x33ac, 0x10},
    {0x33ae, 0x10},
    {0x33af, 0x19},
    {0x360f, 0x01},
    {0x3622, 0x03},
    {0x363a, 0x1f},
    {0x363c, 0x40},
    {0x3651, 0x7d},
    {0x3670, 0x0a},
    {0x3671, 0x07},
    {0x3672, 0x17},
    {0x3673, 0x1e},
    {0x3674, 0x82},
    {0x3675, 0x64},
    {0x3676, 0x66},
    {0x367a, 0x48},
    {0x367b, 0x78},
    {0x367c, 0x58},
    {0x367d, 0x78},
    {0x3690, 0x34},
    {0x3691, 0x34},
    {0x3692, 0x54},
    {0x369c, 0x48},
    {0x369d, 0x78},
    {0x36ec, 0x0a},
    {0x3904, 0x04},
    {0x3908, 0x41},
    {0x391d, 0x04},
    {0x39c2, 0x30},
    {0x3e01, 0xcd},
    {0x3e02, 0xc0},
    {0x3e16, 0x00},
    {0x3e17, 0x80},
    {0x4500, 0x88},
    {0x4509, 0x20},
    {0x4837, 0x14},
    {0x5799, 0x00},
    {0x59e0, 0x60},
    {0x59e1, 0x08},
    {0x59e2, 0x3f},
    {0x59e3, 0x18},
    {0x59e4, 0x18},
    {0x59e5, 0x3f},
    {0x59e7, 0x02},
    {0x59e8, 0x38},
    {0x59e9, 0x20},
    {0x59ea, 0x0c},
    {0x59ec, 0x08},
    {0x59ed, 0x02},
    {0x59ee, 0xa0},
    {0x59ef, 0x08},
    {0x59f4, 0x18},
    {0x59f5, 0x10},
    {0x59f6, 0x0c},
    {0x59f9, 0x02},
    {0x59fa, 0x18},
    {0x59fb, 0x10},
    {0x59fc, 0x0c},
    {0x59ff, 0x02},
    {0x36e9, 0x1c},
    {0x36f9, 0x24},
    {REG_END, 0x00},
};
#endif

static const struct sc501ai_mode supported_modes[] =
{
    [SC501AI_1440X810] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1440,
        .height = 810,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x062c,
        .vts_def = 0x0343,
        .xvclk = 27000000,
        .link_freq = 198000000,
        .reg_list = g_sc501ai_1440x810_60fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc501ai_1440x810_60fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [SC501AI_2880X1616] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2880,
        .height = 1616,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x0c80,
        .vts_def = 0x0672,
        .xvclk = 27000000,
        .link_freq = 396000000,
        .reg_list = g_sc501ai_2880x1616_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc501ai_2880x1616_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct sc501ai_ops
{
    ret_err_t (*init)(struct sc501ai_dev *dev);
    ret_err_t (*open)(struct sc501ai_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc501ai_dev *dev);
    ret_err_t (*control)(struct sc501ai_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc501ai_read_reg(struct rt_i2c_bus_device *bus,
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
    msg[0].addr = g_sc501ai.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_sc501ai.i2c_addr;
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
            sc501ai_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc501ai_write_reg(struct rt_i2c_bus_device *bus,
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

    msg.addr = g_sc501ai.i2c_addr;
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
            sc501ai_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int sc501ai_write_multiple_reg_continue(struct sc501ai_dev *dev,
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
        msg.addr = g_sc501ai.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];

        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            sc501ai_err("write multi-regs, retry=%d, addr=0x%02x%02x, ret: %d\n", retry, i2c_data[offset + 1], i2c_data[offset + 2], ret);
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
static ret_err_t sc501ai_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msgs.addr = g_sc501ai.i2c_addr;
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
            //sc501ai_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc501ai_write_multiple_reg(struct sc501ai_dev *dev,
                                       const struct sc501ai_sensor_reg *reg, int len)
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

                sc501ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

                sc501ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

                    sc501ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc501ai_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc501ai_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        sc501ai_write_multiple_reg((struct sc501ai_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

static int sc501ai_get_gain_reg(uint8_t *again, uint8_t *again_fine, uint8_t *dgain,
                                uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t step = 0;

    if (total_gain <= 0x60)   /* 1 - 1.5x gain */
    {
        step = total_gain - 0x40;

        *again = 0x03;
        *again_fine = step + 0x40;
        *dgain = 0x00;
        *dgain_fine = 0x80;
    }
    else if (total_gain <= 0xc0)     /* 1.5x - 3x gain */
    {
        step = (total_gain - 0x60) * 64 / 0x60 - 1;

        *again = 0x23;
        *again_fine = step + 0x40;
        *dgain = 0x00;
        *dgain_fine = 0x80;
    }
    else if (total_gain <= 0x180)     /* 3x - 6x gain */
    {
        step = (total_gain - 0xc0) * 64 / 0xc0 - 1;

        *again = 0x27;
        *again_fine = step + 0x40;
        *dgain = 0x00;
        *dgain_fine = 0x80;
    }
    else if (total_gain <= 0x300)     /* 6x - 12x gain */
    {
        step = (total_gain - 0x180) * 64 / 0x180 - 1;

        *again = 0x2f;
        *again_fine = step + 0x40;
        *dgain = 0x00;
        *dgain_fine = 0x80;
    }
    else if (total_gain <= 0x600)     /* 12x - 24x gain */
    {
        step = (total_gain - 0x300) * 64 / 0x300 - 1;

        *again = 0x3f;
        *again_fine = step + 0x40;
        *dgain = 0x00;
        *dgain_fine = 0x80;
    }
    else if (total_gain <= 0xc00)     /* 24x - 48x gain */
    {
        step = (total_gain - 0x600) * 128 / 0x600 - 1;

        *again = 0x3f;
        *again_fine = 0x7f;
        *dgain = 0x00;
        *dgain_fine = 0x80 + step;
    }
    else if (total_gain <= 0x1800)     /* 48x - 96x gain */
    {
        step = (total_gain - 0xc00) * 128 / 0xc00 - 1;

        *again = 0x3f;
        *again_fine = 0x7f;
        *dgain = 0x01;
        *dgain_fine = 0x80 + step;
    }
    else if (total_gain <= 0x3000)     /* 96x - 192x gain */
    {
        step  = (total_gain - 0x1800) * 128 / 0x1800 - 1;

        *again = 0x3f;
        *again_fine = 0x7f;
        *dgain = 0x03;
        *dgain_fine = 0x80 + step;
    }
    else if (total_gain <= 0x6000)     /* 192x - 384x gain */
    {
        step  = (total_gain - 0x3000) * 128 / 0x3000 - 1;

        *again = 0x3f;
        *again_fine = 0x7f;
        *dgain = 0x07;
        *dgain_fine = 0x80 + step;
    }
    else if (total_gain <= 0xc000)     /* 384x - 768x gain */
    {
        step  = (total_gain - 0x6000) * 128 / 0x6000 - 1;

        *again = 0x3f;
        *again_fine = 0x7f;
        *dgain = 0x0f;
        *dgain_fine = 0x80 + step;
    }
    return ret;
}
static ret_err_t rk_sc501ai_set_expval(struct sc501ai_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain = 0, a_gain_fine = 0, d_gain = 0, d_gain_fine = 0;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc501ai_dbg("sc501ai don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0] * 2;
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC501AI_EXP_MIN)
        l_exp_time = SC501AI_EXP_MIN;

    sc501ai_dbg("total_time reg: 0x%x, total_gain reg 0x%x\n",
                l_exp_time, l_a_gain);

    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_EXP_LONG_H,
                            (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_EXP_LONG_M,
                            (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_EXP_LONG_L,
                            (uint8_t)((l_exp_time & 0xf) << 4));
    sc501ai_get_gain_reg(&a_gain, &a_gain_fine, &d_gain, &d_gain_fine, l_a_gain);

    sc501ai_dbg("a_gain: 0x%x a_gain_fine: 0x%x d_gain: 0x%x d_gain_fine: 0x%x\n", a_gain, a_gain_fine, d_gain, d_gain_fine);

    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_ANA_GAIN, a_gain);
    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_ANA_FINE_GAIN, a_gain_fine);
    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_DIG_GAIN, d_gain);
    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc501ai_set_vts(struct sc501ai_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc501ai_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_VTS_L,
                            (uint8_t)(dst_vts & 0xff));
    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_VTS_H,
                            (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc501ai_set_flip_mirror(struct sc501ai_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc501ai_read_reg(dev->i2c_bus, SC501AI_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC501AI_FETCH_MIRROR(val, flip & 0x01);
        val = SC501AI_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC501AI_FETCH_MIRROR(val, false);
        val = SC501AI_FETCH_FLIP(val, false);
        break;
    };
    sc501ai_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc501ai_write_reg(dev->i2c_bus, SC501AI_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc501ai_stream_on(struct sc501ai_dev *dev)
{
    sc501ai_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    HAL_DelayMs(5);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
#ifndef REG_LIST_CONVERSION_DEBUG
    sc501ai_write_multiple_reg_continue((struct sc501ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc501ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc501ai_set_flip_mirror(dev, dev->flip);

    sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_STREAM_CTRL, SC501AI_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc501ai_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc501ai_stream_on_late(struct sc501ai_dev *dev)
{

    sc501ai_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc501ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc501ai_set_flip_mirror(dev, dev->flip);

    sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_STREAM_CTRL, SC501AI_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc501ai_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc501ai_stream_off(struct sc501ai_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc501ai_write_reg(dev->i2c_bus, SC501AI_REG_STREAM_CTRL, SC501AI_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc501ai_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_sc501ai_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc501ai_dev *sc501ai;

    sc501ai = (struct sc501ai_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc501ai->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc501ai);
#endif

    if (sc501ai)
    {
        camera->info.mbus_fmt.width = sc501ai->cur_mode->width;
        camera->info.mbus_fmt.height = sc501ai->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc501ai->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc501ai->cur_mode->hdr_mode;

        sc501ai->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc501ai->i2c_name);

        if (!sc501ai->i2c_bus)
        {
            sc501ai_err("not find i2c :%s !!!\n",
                        sc501ai->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc501ai_dbg("find i2c_bus:%s\n", sc501ai->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc501ai_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc501ai_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_sc501ai_get_expinf(struct sc501ai_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc501ai_mode *mode;

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

static ret_err_t rk_sc501ai_get_intput_fmt(struct sc501ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc501ai_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc501ai_set_intput_fmt(struct sc501ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc501ai_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc501ai_info("dst resulotion, width %d, height %d\n",
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
            sc501ai_write_multiple_reg_continue((struct sc501ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc501ai_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc501ai_match_dst_config(struct sc501ai_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc501ai_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc501ai_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc501ai_mode *)mode;
            ret = RET_SYS_EOK;
            sc501ai_info("find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
    }

    if (i == ARRAY_SIZE(supported_modes))
    {
        sc501ai_err("not find match resulotion\n");
        return -RT_EINVAL;
    }

    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc501ai_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc501ai_control(struct rk_camera_device *dev,
                             dt_cmd_t cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc501ai_dev *sc501ai;

    RT_ASSERT(dev != RT_NULL);
    sc501ai = (struct sc501ai_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc501ai_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc501ai_stream_on(sc501ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc501ai_stream_off(sc501ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc501ai_get_expinf(sc501ai, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc501ai_set_expval(sc501ai, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc501ai_set_vts(sc501ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc501ai_get_intput_fmt(sc501ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc501ai_set_intput_fmt(sc501ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc501ai_match_dst_config(sc501ai, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc501ai_set_flip_mirror(sc501ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc501ai_stream_on_late(sc501ai);
    }
    break;
    default:
        sc501ai_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc501ai_ops =
{
    rk_sc501ai_init,
    rk_sc501ai_open,
    NULL,
    rk_sc501ai_control
};

int rk_camera_sc501ai_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc501ai_dev *instance = &g_sc501ai;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *sc501ai = (struct camera_board_desc *)&camera_sc501ai;

    camera->ops = &rk_sc501ai_ops;

    rt_strncpy(instance->name, sc501ai->isp_subdev_name, rt_strlen(sc501ai->isp_subdev_name));
    rt_strncpy(instance->i2c_name, sc501ai->i2c_bus, rt_strlen(sc501ai->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (sc501ai->mode_id <= SC501AI_MODE_ID_MAX)
    {
        sc501ai_dbg("mode_id: %d\n", sc501ai->mode_id);
        instance->cur_mode = &supported_modes[sc501ai->mode_id];
    }
    else
    {
        sc501ai_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->i2c_bus->timeout = 10;

    instance->dst_mode = (struct sc501ai_mode *)&supported_modes[SC501AI_2880X1616];
    instance->time_valid_delay = sc501ai->time_valid_delay;
    instance->gain_valid_delay = sc501ai->gain_valid_delay;
    instance->i2c_addr = sc501ai->i2c_addr;
    instance->flip = 0;

    if (sc501ai->rst_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc501ai->rst_gpio.gpio_group, sc501ai->rst_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc501ai->rst_gpio.gpio_group, sc501ai->rst_gpio.gpio_pin, !sc501ai->rst_active);
    }
    if (sc501ai->pwdn_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc501ai->pwdn_gpio.gpio_group, sc501ai->pwdn_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc501ai->pwdn_gpio.gpio_group, sc501ai->pwdn_gpio.gpio_pin, !sc501ai->pwdn_active);
    }

    if (sc501ai->mclk_out_gate_id && sc501ai->mclk_id)
    {
        clk_set_rate(sc501ai->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(sc501ai->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "SC501AI_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, sc501ai->isp_subdev_name, rt_strlen(sc501ai->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);

    return ret;
}

void sc501ai_detect(void)
{
    struct sc501ai_dev *instance = &g_sc501ai;

    sc501ai_dbg("start to detect SC501AI for testing \n");
    sc501ai_dbg("dev name:%s\n", instance->name);
    sc501ai_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        sc501ai_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        sc501ai_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc501ai_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc501ai_detect, check SC501AI is available or not);
#endif
#endif
#endif
