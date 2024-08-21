/**
  * Copyright (c) 2024 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc450ai.c
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

#ifdef RT_USING_SC450AI

//#define REG_LIST_CONVERSION_DEBUG
#define SC450AI_DEBUG_PRT      0

#if SC450AI_DEBUG_PRT
#include <stdio.h>
#define sc450ai_dbg(fmt, args...)               rt_kprintf("[SC450AI] "fmt"", ##args)
#else
#define sc450ai_dbg(fmt, args...)
#endif

#define sc450ai_info(fmt, args...)         rt_kprintf("[SC450AI] "fmt"", ##args)
#define sc450ai_err(fmt, args...)         rt_kprintf("[SC450AI] ERR "fmt"", ##args)

#define SC450AI_REG_EXP_LONG_H           0x3e00
#define SC450AI_REG_EXP_LONG_M           0x3e01
#define SC450AI_REG_EXP_LONG_L           0x3e02
#define SC450AI_REG_DIG_GAIN            0x3e06
#define SC450AI_REG_DIG_FINE_GAIN       0x3e07
#define SC450AI_REG_ANA_GAIN            0x3e08
#define SC450AI_REG_ANA_FINE_GAIN       0x3e09
#define SC450AI_EXP_MIN                 2
#define SC450AI_GAIN_MIN                0x40
#define SC450AI_GAIN_MAX                (61975)  //60.523*16*64 (99614) //48.64*16*128
#define SC450AI_GAIN_STEP               1
#define SC450AI_GAIN_DEFAULT            0x40
#define SC450AI_FLIP_MIRROR_REG         0x3221
#define SC450AI_REG_VTS_H               0x320e
#define SC450AI_REG_VTS_L               0x320f
#define SC450AI_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC450AI_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC450AI_REG_STREAM_CTRL         0x0100
#define SC450AI_STREAM_ON               0x01
#define SC450AI_STREAM_OFF              0x00

#define REG_END                     (0x0)
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

struct sc450ai_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc450ai_mode
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
    const struct sc450ai_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc450ai_dev
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
    const struct sc450ai_mode *cur_mode;
    struct sc450ai_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc450ai_dev *rt_sc450ai_dev_t;

static struct sc450ai_dev g_sc450ai;

#ifndef REG_LIST_CONVERSION_DEBUG
/*
 * Xclk 27Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 720Mbps, 2lane
 * binning to 1344x760
 */
static const uint8_t g_sc450ai_1344x760_120fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x4, 0x30, 0x18, 0x3a, 0x0c,
    0x3, 0x30, 0x1c, 0x78,
    0x3, 0x30, 0x1f, 0x75,
    0x3, 0x30, 0x2e, 0x00,
    0xa, 0x32, 0x08, 0x05, 0x40, 0x02, 0xf8, 0x03, 0xa8, 0x03, 0x0c,
    0x3, 0x32, 0x11, 0x04,
    0x5, 0x32, 0x13, 0x04, 0x11, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x23, 0xc0,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x44,
    0x3, 0x32, 0x74, 0x09,
    0x3, 0x32, 0x80, 0x01,
    0x3, 0x33, 0x01, 0x08,
    0x3, 0x33, 0x06, 0x24,
    0x3, 0x33, 0x09, 0x60,
    0x3, 0x33, 0x0b, 0x64,
    0x3, 0x33, 0x0d, 0x30,
    0x3, 0x33, 0x15, 0x00,
    0x3, 0x33, 0x1f, 0x59,
    0x3, 0x33, 0x5d, 0x60,
    0x3, 0x33, 0x64, 0x56,
    0x10, 0x33, 0x8f, 0x80, 0x08, 0x18, 0x38, 0x0a, 0x10, 0x18, 0x08, 0x18, 0x38, 0x0f, 0x12, 0x14, 0x18,
    0x3, 0x33, 0xaf, 0x18,
    0x3, 0x36, 0x0f, 0x13,
    0x3, 0x36, 0x21, 0xec,
    0x3, 0x36, 0x27, 0xa0,
    0x3, 0x36, 0x30, 0x90,
    0x3, 0x36, 0x33, 0x56,
    0x4, 0x36, 0x37, 0x1d, 0x0a,
    0x5, 0x36, 0x3c, 0x0f, 0x0f, 0x08,
    0x9, 0x36, 0x70, 0x4a, 0xe0, 0xe0, 0xe0, 0xb0, 0x88, 0x8c,
    0x6, 0x36, 0x7a, 0x48, 0x58, 0x48, 0x58,
    0x5, 0x36, 0x90, 0x34, 0x43, 0x44,
    0x7, 0x36, 0x99, 0x03, 0x0f, 0x1f, 0x40, 0x48,
    0x4, 0x36, 0xa2, 0x48, 0x78,
    0x7, 0x36, 0xb0, 0x54, 0x75, 0x35, 0x48, 0x78,
    0x5, 0x36, 0xb7, 0xa0, 0xa0, 0x20,
    0x4, 0x36, 0xbd, 0x40, 0x48,
    0x3, 0x36, 0xd0, 0x20,
    0x7, 0x36, 0xe0, 0x08, 0x08, 0x12, 0x48, 0x78,
    0x6, 0x36, 0xfa, 0x0d, 0xa4, 0x00, 0x24,
    0x4, 0x39, 0x07, 0x00, 0x41,
    0x4, 0x39, 0x1e, 0x01, 0x11,
    0xc, 0x39, 0x33, 0x82, 0x0b, 0x02, 0x5e, 0x76, 0x78, 0x00, 0x28, 0x00, 0x1d,
    0x6, 0x3e, 0x00, 0x00, 0x61, 0x00, 0x0b,
    0x3, 0x3e, 0x08, 0x03,
    0x3, 0x3e, 0x1b, 0x2a,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x48, 0x37, 0x16,
    0x4, 0x50, 0x00, 0x4e, 0x44,
    0x3, 0x57, 0x80, 0x76,
    0x4, 0x57, 0x84, 0x08, 0x04,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x40,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04,
    0x4, 0x57, 0x99, 0x46, 0x77,
    0x3, 0x57, 0xa1, 0x04,
    0x3, 0x57, 0xa8, 0xd0,
    0x6, 0x57, 0xaa, 0x2a, 0x7f, 0x00, 0x00,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x12, 0x59, 0xe0, 0xfe, 0x40, 0x3f, 0x38, 0x30, 0x3f, 0x38, 0x30, 0x3f, 0x3c, 0x38, 0x3f, 0x3c, 0x38, 0xfe, 0x40,
    0xe, 0x59, 0xf4, 0x3f, 0x38, 0x30, 0x3f, 0x38, 0x30, 0x3f, 0x3c, 0x38, 0x3f, 0x3c, 0x38,
    0x3, 0x36, 0xe9, 0x44,
    0x3, 0x36, 0xf9, 0x20,
    0x45,
};

/*
 * Xclk 27Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 720Mbps, 2lane
 * full resolution
 */
static const uint8_t g_sc450ai_2688x1520_30fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x4, 0x30, 0x18, 0x3a, 0x0c,
    0x3, 0x30, 0x1c, 0x78,
    0x3, 0x30, 0x1f, 0x3c,
    0x3, 0x30, 0x2e, 0x00,
    0xa, 0x32, 0x08, 0x0a, 0x80, 0x05, 0xf0, 0x02, 0xee, 0x06, 0x18,
    0x4, 0x32, 0x14, 0x11, 0x11,
    0x3, 0x32, 0x20, 0x00,
    0x3, 0x32, 0x23, 0xc0,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x44,
    0x3, 0x32, 0x74, 0x09,
    0x3, 0x32, 0x80, 0x01,
    0x3, 0x33, 0x01, 0x07,
    0x3, 0x33, 0x06, 0x20,
    0x3, 0x33, 0x08, 0x08,
    0x3, 0x33, 0x0b, 0x58,
    0x3, 0x33, 0x0e, 0x18,
    0x3, 0x33, 0x15, 0x00,
    0x3, 0x33, 0x5d, 0x60,
    0x3, 0x33, 0x64, 0x56,
    0x10, 0x33, 0x8f, 0x80, 0x08, 0x18, 0x38, 0x07, 0x10, 0x18, 0x08, 0x18, 0x38, 0x10, 0x13, 0x15, 0x18,
    0x3, 0x33, 0xaf, 0x18,
    0x3, 0x36, 0x0f, 0x13,
    0x4, 0x36, 0x21, 0xec, 0x00,
    0x3, 0x36, 0x25, 0x0b,
    0x3, 0x36, 0x27, 0x20,
    0x3, 0x36, 0x30, 0x90,
    0x3, 0x36, 0x33, 0x56,
    0x4, 0x36, 0x37, 0x1d, 0x12,
    0x5, 0x36, 0x3c, 0x0f, 0x0f, 0x08,
    0x9, 0x36, 0x70, 0x4a, 0xe0, 0xe0, 0xe0, 0xc0, 0x87, 0x8c,
    0x6, 0x36, 0x7a, 0x48, 0x58, 0x48, 0x58,
    0x5, 0x36, 0x90, 0x22, 0x33, 0x44,
    0x7, 0x36, 0x99, 0x03, 0x0f, 0x1f, 0x40, 0x78,
    0x4, 0x36, 0xa2, 0x48, 0x78,
    0x7, 0x36, 0xb0, 0x53, 0x74, 0x34, 0x40, 0x78,
    0x5, 0x36, 0xb7, 0xa0, 0xa0, 0x20,
    0x4, 0x36, 0xbd, 0x40, 0x48,
    0x3, 0x36, 0xd0, 0x20,
    0x7, 0x36, 0xe0, 0x08, 0x08, 0x12, 0x48, 0x78,
    0x3, 0x36, 0xec, 0x43,
    0x3, 0x36, 0xfc, 0x00,
    0x4, 0x39, 0x07, 0x00, 0x41,
    0x4, 0x39, 0x1e, 0xf1, 0x11,
    0xc, 0x39, 0x33, 0x82, 0x30, 0x02, 0xc7, 0x76, 0x76, 0x00, 0x28, 0x00, 0x23,
    0x5, 0x3e, 0x01, 0xc2, 0x60, 0x0b,
    0x3, 0x3e, 0x08, 0x03,
    0x3, 0x3e, 0x1b, 0x2a,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x48, 0x37, 0x16,
    0x4, 0x50, 0x00, 0x0e, 0x44,
    0x4, 0x57, 0x84, 0x08, 0x04,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x40,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04,
    0x3, 0x57, 0x99, 0x06,
    0x6, 0x57, 0xaa, 0x28, 0x00, 0x00, 0x00,
    0x12, 0x59, 0xe0, 0xfe, 0x40, 0x3f, 0x38, 0x30, 0x3f, 0x38, 0x30, 0x3f, 0x3c, 0x38, 0x3f, 0x3c, 0x38, 0xfe, 0x40,
    0xe, 0x59, 0xf4, 0x3f, 0x38, 0x30, 0x3f, 0x38, 0x30, 0x3f, 0x3c, 0x38, 0x3f, 0x3c, 0x38,
    0x3, 0x36, 0xe9, 0x44,
    0x3, 0x36, 0xf9, 0x20,
    0x41,
};

#else
/*
 * Xclk 27Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 720Mbps, 2lane
 * binning to 1344x760
 */
static const struct sc450ai_sensor_reg g_sc450ai_1344x760_120fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x36f9, 0x80},
    {0x3018, 0x3a},
    {0x3019, 0x0c},
    {0x301c, 0x78},
    {0x301f, 0x75},
    {0x302e, 0x00},
    {0x3208, 0x05},
    {0x3209, 0x40},
    {0x320a, 0x02},
    {0x320b, 0xf8},
    {0x320c, 0x03},
    {0x320d, 0xa8},
    {0x320e, 0x03},
    {0x320f, 0x0c},
    {0x3211, 0x04},
    {0x3213, 0x04},
    {0x3214, 0x11},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3223, 0xc0},
    {0x3253, 0x10},
    {0x325f, 0x44},
    {0x3274, 0x09},
    {0x3280, 0x01},
    {0x3301, 0x08},
    {0x3306, 0x24},
    {0x3309, 0x60},
    {0x330b, 0x64},
    {0x330d, 0x30},
    {0x3315, 0x00},
    {0x331f, 0x59},
    {0x335d, 0x60},
    {0x3364, 0x56},
    {0x338f, 0x80},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0x38},
    {0x3393, 0x0a},
    {0x3394, 0x10},
    {0x3395, 0x18},
    {0x3396, 0x08},
    {0x3397, 0x18},
    {0x3398, 0x38},
    {0x3399, 0x0f},
    {0x339a, 0x12},
    {0x339b, 0x14},
    {0x339c, 0x18},
    {0x33af, 0x18},
    {0x360f, 0x13},
    {0x3621, 0xec},
    {0x3627, 0xa0},
    {0x3630, 0x90},
    {0x3633, 0x56},
    {0x3637, 0x1d},
    {0x3638, 0x0a},
    {0x363c, 0x0f},
    {0x363d, 0x0f},
    {0x363e, 0x08},
    {0x3670, 0x4a},
    {0x3671, 0xe0},
    {0x3672, 0xe0},
    {0x3673, 0xe0},
    {0x3674, 0xb0},
    {0x3675, 0x88},
    {0x3676, 0x8c},
    {0x367a, 0x48},
    {0x367b, 0x58},
    {0x367c, 0x48},
    {0x367d, 0x58},
    {0x3690, 0x34},
    {0x3691, 0x43},
    {0x3692, 0x44},
    {0x3699, 0x03},
    {0x369a, 0x0f},
    {0x369b, 0x1f},
    {0x369c, 0x40},
    {0x369d, 0x48},
    {0x36a2, 0x48},
    {0x36a3, 0x78},
    {0x36b0, 0x54},
    {0x36b1, 0x75},
    {0x36b2, 0x35},
    {0x36b3, 0x48},
    {0x36b4, 0x78},
    {0x36b7, 0xa0},
    {0x36b8, 0xa0},
    {0x36b9, 0x20},
    {0x36bd, 0x40},
    {0x36be, 0x48},
    {0x36d0, 0x20},
    {0x36e0, 0x08},
    {0x36e1, 0x08},
    {0x36e2, 0x12},
    {0x36e3, 0x48},
    {0x36e4, 0x78},
    {0x36fa, 0x0d},
    {0x36fb, 0xa4},
    {0x36fc, 0x00},
    {0x36fd, 0x24},
    {0x3907, 0x00},
    {0x3908, 0x41},
    {0x391e, 0x01},
    {0x391f, 0x11},
    {0x3933, 0x82},
    {0x3934, 0x0b},
    {0x3935, 0x02},
    {0x3936, 0x5e},
    {0x3937, 0x76},
    {0x3938, 0x78},
    {0x3939, 0x00},
    {0x393a, 0x28},
    {0x393b, 0x00},
    {0x393c, 0x1d},
    {0x3e00, 0x00},
    {0x3e01, 0x61},
    {0x3e02, 0x00},
    {0x3e03, 0x0b},
    {0x3e08, 0x03},
    {0x3e1b, 0x2a},
    {0x440e, 0x02},
    {0x4509, 0x20},
    {0x4837, 0x16},
    {0x5000, 0x4e},
    {0x5001, 0x44},
    {0x5780, 0x76},
    {0x5784, 0x08},
    {0x5785, 0x04},
    {0x5787, 0x0a},
    {0x5788, 0x0a},
    {0x5789, 0x0a},
    {0x578a, 0x0a},
    {0x578b, 0x0a},
    {0x578c, 0x0a},
    {0x578d, 0x40},
    {0x5790, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x04},
    {0x5793, 0x08},
    {0x5794, 0x04},
    {0x5795, 0x04},
    {0x5799, 0x46},
    {0x579a, 0x77},
    {0x57a1, 0x04},
    {0x57a8, 0xd0},
    {0x57aa, 0x2a},
    {0x57ab, 0x7f},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x59e0, 0xfe},
    {0x59e1, 0x40},
    {0x59e2, 0x3f},
    {0x59e3, 0x38},
    {0x59e4, 0x30},
    {0x59e5, 0x3f},
    {0x59e6, 0x38},
    {0x59e7, 0x30},
    {0x59e8, 0x3f},
    {0x59e9, 0x3c},
    {0x59ea, 0x38},
    {0x59eb, 0x3f},
    {0x59ec, 0x3c},
    {0x59ed, 0x38},
    {0x59ee, 0xfe},
    {0x59ef, 0x40},
    {0x59f4, 0x3f},
    {0x59f5, 0x38},
    {0x59f6, 0x30},
    {0x59f7, 0x3f},
    {0x59f8, 0x38},
    {0x59f9, 0x30},
    {0x59fa, 0x3f},
    {0x59fb, 0x3c},
    {0x59fc, 0x38},
    {0x59fd, 0x3f},
    {0x59fe, 0x3c},
    {0x59ff, 0x38},
    {0x36e9, 0x44},
    {0x36f9, 0x20},
    {REG_END, 0x00},
};

/*
 * Xclk 27Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 720Mbps, 2lane
 * full resolution
 */
static const struct sc450ai_sensor_reg g_sc450ai_2688x1520_30fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x36f9, 0x80},
    {0x3018, 0x3a},
    {0x3019, 0x0c},
    {0x301c, 0x78},
    {0x301f, 0x3c},
    {0x302e, 0x00},
    {0x3208, 0x0a},
    {0x3209, 0x80},
    {0x320a, 0x05},
    {0x320b, 0xf0},
    {0x320c, 0x02},
    {0x320d, 0xee},
    {0x320e, 0x06},
    {0x320f, 0x18},
    {0x3214, 0x11},
    {0x3215, 0x11},
    {0x3220, 0x00},
    {0x3223, 0xc0},
    {0x3253, 0x10},
    {0x325f, 0x44},
    {0x3274, 0x09},
    {0x3280, 0x01},
    {0x3301, 0x07},
    {0x3306, 0x20},
    {0x3308, 0x08},
    {0x330b, 0x58},
    {0x330e, 0x18},
    {0x3315, 0x00},
    {0x335d, 0x60},
    {0x3364, 0x56},
    {0x338f, 0x80},
    {0x3390, 0x08},
    {0x3391, 0x18},
    {0x3392, 0x38},
    {0x3393, 0x07},
    {0x3394, 0x10},
    {0x3395, 0x18},
    {0x3396, 0x08},
    {0x3397, 0x18},
    {0x3398, 0x38},
    {0x3399, 0x10},
    {0x339a, 0x13},
    {0x339b, 0x15},
    {0x339c, 0x18},
    {0x33af, 0x18},
    {0x360f, 0x13},
    {0x3621, 0xec},
    {0x3622, 0x00},
    {0x3625, 0x0b},
    {0x3627, 0x20},
    {0x3630, 0x90},
    {0x3633, 0x56},
    {0x3637, 0x1d},
    {0x3638, 0x12},
    {0x363c, 0x0f},
    {0x363d, 0x0f},
    {0x363e, 0x08},
    {0x3670, 0x4a},
    {0x3671, 0xe0},
    {0x3672, 0xe0},
    {0x3673, 0xe0},
    {0x3674, 0xc0},
    {0x3675, 0x87},
    {0x3676, 0x8c},
    {0x367a, 0x48},
    {0x367b, 0x58},
    {0x367c, 0x48},
    {0x367d, 0x58},
    {0x3690, 0x22},
    {0x3691, 0x33},
    {0x3692, 0x44},
    {0x3699, 0x03},
    {0x369a, 0x0f},
    {0x369b, 0x1f},
    {0x369c, 0x40},
    {0x369d, 0x78},
    {0x36a2, 0x48},
    {0x36a3, 0x78},
    {0x36b0, 0x53},
    {0x36b1, 0x74},
    {0x36b2, 0x34},
    {0x36b3, 0x40},
    {0x36b4, 0x78},
    {0x36b7, 0xa0},
    {0x36b8, 0xa0},
    {0x36b9, 0x20},
    {0x36bd, 0x40},
    {0x36be, 0x48},
    {0x36d0, 0x20},
    {0x36e0, 0x08},
    {0x36e1, 0x08},
    {0x36e2, 0x12},
    {0x36e3, 0x48},
    {0x36e4, 0x78},
    {0x36ec, 0x43},
    {0x36fc, 0x00},
    {0x3907, 0x00},
    {0x3908, 0x41},
    {0x391e, 0xf1},
    {0x391f, 0x11},
    {0x3933, 0x82},
    {0x3934, 0x30},
    {0x3935, 0x02},
    {0x3936, 0xc7},
    {0x3937, 0x76},
    {0x3938, 0x76},
    {0x3939, 0x00},
    {0x393a, 0x28},
    {0x393b, 0x00},
    {0x393c, 0x23},
    {0x3e01, 0xc2},
    {0x3e02, 0x60},
    {0x3e03, 0x0b},
    {0x3e08, 0x03},
    {0x3e1b, 0x2a},
    {0x440e, 0x02},
    {0x4509, 0x20},
    {0x4837, 0x16},
    {0x5000, 0x0e},
    {0x5001, 0x44},
    {0x5784, 0x08},
    {0x5785, 0x04},
    {0x5787, 0x0a},
    {0x5788, 0x0a},
    {0x5789, 0x0a},
    {0x578a, 0x0a},
    {0x578b, 0x0a},
    {0x578c, 0x0a},
    {0x578d, 0x40},
    {0x5790, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x04},
    {0x5793, 0x08},
    {0x5794, 0x04},
    {0x5795, 0x04},
    {0x5799, 0x06},
    {0x57aa, 0x28},
    {0x57ab, 0x00},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x59e0, 0xfe},
    {0x59e1, 0x40},
    {0x59e2, 0x3f},
    {0x59e3, 0x38},
    {0x59e4, 0x30},
    {0x59e5, 0x3f},
    {0x59e6, 0x38},
    {0x59e7, 0x30},
    {0x59e8, 0x3f},
    {0x59e9, 0x3c},
    {0x59ea, 0x38},
    {0x59eb, 0x3f},
    {0x59ec, 0x3c},
    {0x59ed, 0x38},
    {0x59ee, 0xfe},
    {0x59ef, 0x40},
    {0x59f4, 0x3f},
    {0x59f5, 0x38},
    {0x59f6, 0x30},
    {0x59f7, 0x3f},
    {0x59f8, 0x38},
    {0x59f9, 0x30},
    {0x59fa, 0x3f},
    {0x59fb, 0x3c},
    {0x59fc, 0x38},
    {0x59fd, 0x3f},
    {0x59fe, 0x3c},
    {0x59ff, 0x38},
    {0x36e9, 0x44},
    {0x36f9, 0x20},
    {REG_END, 0x00},
};
#endif

static const struct sc450ai_mode supported_modes[] =
{
    [SC450AI_1344X760] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1344,
        .height = 760,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x80,
        .hts_def = 0x03a8,
        .vts_def = 0x030c,
        .xvclk = 27000000,
        .link_freq = 360000000,
        .reg_list = g_sc450ai_1344x760_120fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc450ai_1344x760_120fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [SC450AI_2688X1520] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2688,
        .height = 1520,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x2ee * 4,
        .vts_def = 0x0618,
        .xvclk = 27000000,
        .link_freq = 360000000,
        .reg_list = g_sc450ai_2688x1520_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc450ai_2688x1520_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct sc450ai_ops
{
    ret_err_t (*init)(struct sc450ai_dev *dev);
    ret_err_t (*open)(struct sc450ai_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc450ai_dev *dev);
    ret_err_t (*control)(struct sc450ai_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc450ai_read_reg(struct rt_i2c_bus_device *bus,
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
    msg[0].addr = g_sc450ai.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_sc450ai.i2c_addr;
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
            sc450ai_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc450ai_write_reg(struct rt_i2c_bus_device *bus,
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

    msg.addr = g_sc450ai.i2c_addr;
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
            sc450ai_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int sc450ai_write_multiple_reg_continue(struct sc450ai_dev *dev,
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
        msg.addr = g_sc450ai.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            sc450ai_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t sc450ai_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msgs.addr = g_sc450ai.i2c_addr;
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
            sc450ai_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc450ai_write_multiple_reg(struct sc450ai_dev *dev,
                                       const struct sc450ai_sensor_reg *reg, int len)
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

                sc450ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

                sc450ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

                    sc450ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc450ai_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc450ai_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        sc450ai_write_multiple_reg((struct sc450ai_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

static int sc450ai_get_gain_reg(uint8_t *again, uint8_t *dgain, uint8_t *again_fine,
                                uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    if (total_gain < 64)
        total_gain = 64;
    else if (total_gain > SC450AI_GAIN_MAX)
        total_gain = SC450AI_GAIN_MAX;

    gain_factor = total_gain * 1000 / 64;
    if (gain_factor < 2000)
    {
        *again = 0x03;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 1000;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 3813)
    {
        *again = 0x07;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 2000;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 7625)
    {
        *again = 0x23;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 3813;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 15250)
    {
        *again = 0x27;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 7625;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 30500)
    {
        *again = 0x2f;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 15250;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 60523)
    {
        *again = 0x3f;
        *dgain = 0x00;
        *again_fine = gain_factor * 64 / 30500;
        *dgain_fine = 0x80;
    }
    //open dgain begin  max digital gain 16X
    else if (gain_factor < 60523 * 2)
    {
        *again = 0x3f;
        *dgain = 0x00;
        *again_fine = 0x7f;
        *dgain_fine = gain_factor * 128 / 60523;
    }
    else if (gain_factor < 60523 * 4)
    {
        *again = 0x3f;
        *dgain = 0x01;
        *again_fine = 0x7f;
        *dgain_fine = gain_factor * 128 / 60523 / 2;
    }
    else if (gain_factor  < 60523 * 8)
    {
        *again = 0x3f;
        *dgain = 0x03;
        *again_fine = 0x7f;
        *dgain_fine = gain_factor * 128 / 60523 / 4;
    }
    else if (gain_factor < 60523 * 16)
    {
        *again = 0x3f;
        *dgain = 0x07;
        *again_fine = 0x7f;
        *dgain_fine = gain_factor * 128 / 60523 / 8;
    }

    return ret;
}

static ret_err_t rk_sc450ai_set_expval(struct sc450ai_dev *dev, struct rk_camera_exp_val *exp)
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
        sc450ai_dbg("sc450ai don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0] << 1;
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC450AI_EXP_MIN)
        l_exp_time = SC450AI_EXP_MIN;

    sc450ai_dbg("total_time reg: 0x%x, total_gain reg 0x%x\n",
                l_exp_time, l_a_gain);

    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_EXP_LONG_H,
                            (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_EXP_LONG_M,
                            (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_EXP_LONG_L,
                            (uint8_t)((l_exp_time & 0xf) << 4));
    sc450ai_get_gain_reg(&a_gain, &d_gain, &a_gain_fine, &d_gain_fine, l_a_gain);

    sc450ai_dbg("a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", a_gain, d_gain,
                a_gain_fine, d_gain_fine);

    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_ANA_GAIN, a_gain);
    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_ANA_FINE_GAIN, a_gain_fine);
    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_DIG_GAIN, d_gain);
    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc450ai_set_vts(struct sc450ai_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc450ai_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_VTS_L,
                            (uint8_t)(dst_vts & 0xff));
    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_VTS_H,
                            (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc450ai_set_flip_mirror(struct sc450ai_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc450ai_read_reg(dev->i2c_bus, SC450AI_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC450AI_FETCH_MIRROR(val, flip & 0x01);
        val = SC450AI_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC450AI_FETCH_MIRROR(val, false);
        val = SC450AI_FETCH_FLIP(val, false);
        break;
    };
    sc450ai_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc450ai_write_reg(dev->i2c_bus, SC450AI_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc450ai_stream_on(struct sc450ai_dev *dev)
{
    sc450ai_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(5);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    sc450ai_write_multiple_reg_continue((struct sc450ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc450ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc450ai_set_flip_mirror(dev, dev->flip);

    sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_STREAM_CTRL, SC450AI_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc450ai_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc450ai_stream_on_late(struct sc450ai_dev *dev)
{

    sc450ai_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc450ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc450ai_set_flip_mirror(dev, dev->flip);

    sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_STREAM_CTRL, SC450AI_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc450ai_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc450ai_stream_off(struct sc450ai_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc450ai_write_reg(dev->i2c_bus, SC450AI_REG_STREAM_CTRL, SC450AI_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc450ai_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_sc450ai_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc450ai_dev *sc450ai;

    sc450ai = (struct sc450ai_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc450ai->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc450ai);
#endif

    if (sc450ai)
    {
        camera->info.mbus_fmt.width = sc450ai->cur_mode->width;
        camera->info.mbus_fmt.height = sc450ai->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc450ai->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc450ai->cur_mode->hdr_mode;

        sc450ai->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc450ai->i2c_name);

        if (!sc450ai->i2c_bus)
        {
            sc450ai_err("not find i2c :%s !!!\n",
                        sc450ai->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc450ai_dbg("find i2c_bus:%s\n", sc450ai->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc450ai_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc450ai_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_sc450ai_get_expinf(struct sc450ai_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc450ai_mode *mode;

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

static ret_err_t rk_sc450ai_get_intput_fmt(struct sc450ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc450ai_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc450ai_set_intput_fmt(struct sc450ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc450ai_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc450ai_info("dst resulotion, width %d, height %d\n",
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
            sc450ai_write_multiple_reg_continue((struct sc450ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc450ai_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc450ai_match_dst_config(struct sc450ai_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc450ai_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc450ai_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc450ai_mode *)mode;
            ret = RET_SYS_EOK;
            sc450ai_info("find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc450ai_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc450ai_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc450ai_control(struct rk_camera_device *dev,
                             dt_cmd_t cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc450ai_dev *sc450ai;

    RT_ASSERT(dev != RT_NULL);
    sc450ai = (struct sc450ai_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc450ai_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc450ai_stream_on(sc450ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc450ai_stream_off(sc450ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc450ai_get_expinf(sc450ai, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc450ai_set_expval(sc450ai, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc450ai_set_vts(sc450ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc450ai_get_intput_fmt(sc450ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc450ai_set_intput_fmt(sc450ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc450ai_match_dst_config(sc450ai, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc450ai_set_flip_mirror(sc450ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc450ai_stream_on_late(sc450ai);
    }
    break;
    default:
        sc450ai_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc450ai_ops =
{
    rk_sc450ai_init,
    rk_sc450ai_open,
    NULL,
    rk_sc450ai_control
};

int rk_camera_sc450ai_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc450ai_dev *instance = &g_sc450ai;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *sc450ai = (struct camera_board_desc *)&camera_sc450ai;

    camera->ops = &rk_sc450ai_ops;

    rt_strncpy(instance->name, sc450ai->isp_subdev_name, rt_strlen(sc450ai->isp_subdev_name));
    rt_strncpy(instance->i2c_name, sc450ai->i2c_bus, rt_strlen(sc450ai->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (sc450ai->mode_id <= SC450AI_MODE_ID_MAX)
    {
        sc450ai_dbg("mode_id: %d\n", sc450ai->mode_id);
        instance->cur_mode = &supported_modes[sc450ai->mode_id];
    }
    else
    {
        sc450ai_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct sc450ai_mode *)&supported_modes[SC450AI_2688X1520];
    instance->time_valid_delay = sc450ai->time_valid_delay;
    instance->gain_valid_delay = sc450ai->gain_valid_delay;
    instance->i2c_addr = sc450ai->i2c_addr;
    instance->flip = 0;

    HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc450ai->rst_gpio.gpio_group, sc450ai->rst_gpio.gpio_pin, GPIO_OUT);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc450ai->rst_gpio.gpio_group, sc450ai->rst_gpio.gpio_pin, !sc450ai->rst_active);

    if (sc450ai->pwdn_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc450ai->pwdn_gpio.gpio_group, sc450ai->pwdn_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc450ai->pwdn_gpio.gpio_group, sc450ai->pwdn_gpio.gpio_pin, !sc450ai->pwdn_active);
    }

    if (sc450ai->mclk_out_gate_id && sc450ai->mclk_id)
    {
        clk_set_rate(sc450ai->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(sc450ai->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "SC450AI_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, sc450ai->isp_subdev_name, rt_strlen(sc450ai->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void sc450ai_detect(void)
{
    struct sc450ai_dev *instance = &g_sc450ai;

    sc450ai_dbg("start to detect SC450AI for testing \n");
    sc450ai_dbg("dev name:%s\n", instance->name);
    sc450ai_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        sc450ai_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        sc450ai_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc450ai_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc450ai_detect, check SC450AI is available or not);
#endif
#endif
#endif
