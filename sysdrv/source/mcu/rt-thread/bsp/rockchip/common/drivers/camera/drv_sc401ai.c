/**
  * Copyright (c) 2023 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc401ai.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2023-9-13     Chad.Ma      first implementation
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

#ifdef RT_USING_SC401AI

//#define REG_LIST_CONVERSION_DEBUG
#define SC401AI_DEBUG_PRT      0

#if SC401AI_DEBUG_PRT
#include <stdio.h>
#define sc401ai_dbg(fmt, args...)               rt_kprintf("[SC401AI] "fmt"", ##args)
#else
#define sc401ai_dbg(fmt, args...)
#endif

#define sc401ai_info(fmt, args...)         rt_kprintf("[SC401AI] "fmt"", ##args)
#define sc401ai_err(fmt, args...)         rt_kprintf("[SC401AI] ERR "fmt"", ##args)

#define SC401AI_REG_EXP_LONG_H           0x3e00
#define SC401AI_REG_EXP_LONG_M           0x3e01
#define SC401AI_REG_EXP_LONG_L           0x3e02
#define SC401AI_REG_EXP_SHORT_H          0x3e22
#define SC401AI_REG_EXP_SHORT_M          0x3e04
#define SC401AI_REG_EXP_SHORT_L          0x3e05
#define SC401AI_REG_DIG_GAIN            0x3e06
#define SC401AI_REG_DIG_FINE_GAIN       0x3e07
#define SC401AI_REG_ANA_GAIN            0x3e08
#define SC401AI_REG_ANA_FINE_GAIN       0x3e09
#define SC401AI_REG_SDIG_GAIN           0x3e10
#define SC401AI_REG_SDIG_FINE_GAIN      0x3e11
#define SC401AI_REG_SANA_GAIN           0x3e12
#define SC401AI_REG_SANA_FINE_GAIN      0x3e13
#define SC401AI_EXP_MIN                 2
#define SC401AI_GAIN_MIN                0x0020
#define SC401AI_GAIN_MAX                (16384)  //32 * 16 * 32
#define SC401AI_GAIN_STEP               1
#define SC401AI_GAIN_DEFAULT            0x0020
#define SC401AI_LGAIN                   0
#define SC401AI_SGAIN                   1
#define SC401AI_FLIP_MIRROR_REG         0x3221
#define SC401AI_REG_VTS_H               0x320e
#define SC401AI_REG_VTS_L               0x320f
#define SC401AI_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC401AI_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC401AI_REG_STREAM_CTRL         0x0100
#define SC401AI_STREAM_ON               0x01
#define SC401AI_STREAM_OFF              0x00

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

struct sc401ai_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc401ai_mode
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
    const struct sc401ai_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc401ai_dev
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
    const struct sc401ai_mode *cur_mode;
    struct sc401ai_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc401ai_dev *rt_sc401ai_dev_t;

static struct sc401ai_dev g_sc401ai;

#ifndef REG_LIST_CONVERSION_DEBUG
static const uint8_t g_sc401ai_1280x720_60fps_reg_table[] =
{
    0x3,  0x01,  0x03,  0x01,
    0x3,  0x01,  0x00,  0x00,
    0x3,  0x36,  0xe9,  0x80,
    0x3,  0x36,  0xf9,  0x80,
    0x4,  0x30,  0x18,  0x3a,  0x0c,
    0x3,  0x30,  0x1c,  0x78,
    0x3,  0x30,  0x1f,  0x74,
    0x6,  0x32,  0x08,  0x05,  0x00,  0x02,  0xd0,
    0x4,  0x32,  0x0e,  0x02,  0xee,
    0x3,  0x32,  0x11,  0x02,
    0x5,  0x32,  0x13,  0x02,  0x11,  0x31,
    0x3,  0x32,  0x20,  0x17,
    0x3,  0x32,  0x23,  0x80,
    0x3,  0x32,  0x50,  0x00,
    0x3,  0x32,  0x53,  0x08,
    0x3,  0x32,  0x74,  0x01,
    0x6,  0x33,  0x01,  0x20,  0x18,  0x10,  0x50,
    0x3,  0x33,  0x06,  0x38,
    0x4,  0x33,  0x08,  0x18,  0x60,
    0x3,  0x33,  0x0b,  0xc0,
    0x6,  0x33,  0x0d,  0x10,  0x18,  0x04,  0x02,
    0x3,  0x33,  0x1c,  0x04,
    0x5,  0x33,  0x1e,  0x41,  0x51,  0x09,
    0x3,  0x33,  0x33,  0x10,
    0x3,  0x33,  0x4c,  0x08,
    0x3,  0x33,  0x56,  0x09,
    0x3,  0x33,  0x64,  0x17,
    0x3,  0x33,  0x8e,  0xfd,
    0xf,  0x33,  0x90,  0x08,  0x18,  0x38,  0x20,  0x20,  0x20,  0x08,  0x18,  0x38,  0x20,  0x20,  0x20,  0x20,
    0x3,  0x33,  0xac,  0x10,
    0x4,  0x33,  0xae,  0x18,  0x19,
    0x3,  0x36,  0x0f,  0x01,
    0x3,  0x36,  0x20,  0x08,
    0x3,  0x36,  0x37,  0x25,
    0x3,  0x36,  0x3a,  0x12,
    0x9,  0x36,  0x70,  0x0a,  0x07,  0x57,  0x5e,  0x84,  0x88,  0x8a,
    0x6,  0x36,  0x7a,  0x58,  0x78,  0x58,  0x78,
    0x5,  0x36,  0x90,  0x33,  0x43,  0x34,
    0x4,  0x36,  0x9c,  0x40,  0x78,
    0x6,  0x36,  0xea,  0x32,  0x0d,  0x2c,  0x24,
    0x6,  0x36,  0xfa,  0x32,  0x33,  0x10,  0x14,
    0x3,  0x39,  0x08,  0x41,
    0x3,  0x39,  0x6c,  0x0e,
    0x6,  0x3e,  0x00,  0x00,  0x5d,  0x40,  0x0b,
    0x4,  0x3e,  0x08,  0x03,  0x40,
    0x3,  0x3e,  0x1b,  0x2a,
    0x3,  0x45,  0x09,  0x30,
    0x3,  0x48,  0x19,  0x04,
    0x3,  0x48,  0x1b,  0x03,
    0x3,  0x48,  0x1d,  0x09,
    0x3,  0x48,  0x1f,  0x02,
    0x3,  0x48,  0x21,  0x08,
    0x3,  0x48,  0x23,  0x02,
    0x3,  0x48,  0x25,  0x02,
    0x3,  0x48,  0x27,  0x02,
    0x3,  0x48,  0x29,  0x04,
    0x4,  0x50,  0x00,  0x46,  0x44,
    0x3,  0x57,  0xa8,  0xd0,
    0x4,  0x59,  0x00,  0xf1,  0x04,
    0x3,  0x36,  0xe9,  0x44,
    0x3,  0x36,  0xf9,  0x44,
    // 0x3,  0x01,  0x00,  0x01,
    0x3d,
};
static const uint8_t g_sc401ai_2560x1440_30fps_reg_table[] =
{
    0x3,  0x01,  0x03,  0x01,
    0x3,  0x01,  0x00,  0x00,
    0x3,  0x36,  0xe9,  0x80,
    0x3,  0x36,  0xf9,  0x80,
    0x4,  0x30,  0x18,  0x3a,  0x0c,
    0x3,  0x30,  0x1c,  0x78,
    0x3,  0x30,  0x1f,  0x05,
    0x6,  0x32,  0x08,  0x0a,  0x00,  0x05,  0xa0,
    0x4,  0x32,  0x0e,  0x05,  0xdc,
    0x4,  0x32,  0x14,  0x11,  0x11,
    0x3,  0x32,  0x23,  0x80,
    0x3,  0x32,  0x50,  0x00,
    0x3,  0x32,  0x53,  0x08,
    0x3,  0x32,  0x74,  0x01,
    0x6,  0x33,  0x01,  0x20,  0x18,  0x10,  0x50,
    0x3,  0x33,  0x06,  0x38,
    0x4,  0x33,  0x08,  0x18,  0x60,
    0x3,  0x33,  0x0b,  0xc0,
    0x6,  0x33,  0x0d,  0x10,  0x18,  0x04,  0x02,
    0x3,  0x33,  0x1c,  0x04,
    0x5,  0x33,  0x1e,  0x41,  0x51,  0x09,
    0x3,  0x33,  0x33,  0x10,
    0x3,  0x33,  0x4c,  0x08,
    0x3,  0x33,  0x56,  0x09,
    0x3,  0x33,  0x64,  0x17,
    0x3,  0x33,  0x8e,  0xfd,
    0xf,  0x33,  0x90,  0x08,  0x18,  0x38,  0x20,  0x20,  0x20,  0x08,  0x18,  0x38,  0x20,  0x20,  0x20,  0x20,
    0x3,  0x33,  0xac,  0x10,
    0x4,  0x33,  0xae,  0x18,  0x19,
    0x3,  0x36,  0x0f,  0x01,
    0x3,  0x36,  0x20,  0x08,
    0x3,  0x36,  0x37,  0x25,
    0x3,  0x36,  0x3a,  0x12,
    0x9,  0x36,  0x70,  0x0a,  0x07,  0x57,  0x5e,  0x84,  0x88,  0x8a,
    0x6,  0x36,  0x7a,  0x58,  0x78,  0x58,  0x78,
    0x5,  0x36,  0x90,  0x33,  0x43,  0x34,
    0x4,  0x36,  0x9c,  0x40,  0x78,
    0x6,  0x36,  0xea,  0x39,  0x0d,  0x1c,  0x24,
    0x6,  0x36,  0xfa,  0x39,  0x33,  0x10,  0x14,
    0x3,  0x39,  0x08,  0x41,
    0x3,  0x39,  0x6c,  0x0e,
    0x6,  0x3e,  0x00,  0x00,  0xb6,  0x00,  0x0b,
    0x4,  0x3e,  0x08,  0x03,  0x40,
    0x3,  0x3e,  0x1b,  0x2a,
    0x3,  0x45,  0x09,  0x30,
    0x3,  0x48,  0x19,  0x08,
    0x3,  0x48,  0x1b,  0x05,
    0x3,  0x48,  0x1d,  0x11,
    0x3,  0x48,  0x1f,  0x04,
    0x3,  0x48,  0x21,  0x09,
    0x3,  0x48,  0x23,  0x04,
    0x3,  0x48,  0x25,  0x04,
    0x3,  0x48,  0x27,  0x04,
    0x3,  0x48,  0x29,  0x07,
    0x3,  0x50,  0x01,  0x44,
    0x3,  0x57,  0xa8,  0xd0,
    0x3,  0x36,  0xe9,  0x14,
    0x3,  0x36,  0xf9,  0x14,
    // 0x3,  0x01,  0x00,  0x01,
    0x3a,
};

#else
/*
 * Xclk 24Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 315Mbps, 2lane
 * bining to 1280x720
 */
static const struct sc401ai_sensor_reg g_sc401ai_1280x720_60fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x16},
    {0x30b8, 0x44},
    {0x3208, 0x05},
    {0x3209, 0x00},
    {0x320a, 0x02},
    {0x320b, 0xd0},
    {0x320e, 0x02},
    {0x320f, 0xee},
    {0x3211, 0x02},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3253, 0x10},
    {0x3301, 0x0a},
    {0x3302, 0xff},
    {0x3305, 0x00},
    {0x3306, 0x90},
    {0x3308, 0x08},
    {0x330a, 0x01},
    {0x330b, 0xb0},
    {0x330d, 0xf0},
    {0x3333, 0x10},
    {0x335e, 0x06},
    {0x335f, 0x0a},
    {0x3364, 0x5e},
    {0x337d, 0x0e},
    {0x338f, 0x20},
    {0x3390, 0x08},
    {0x3391, 0x09},
    {0x3392, 0x0f},
    {0x3393, 0x18},
    {0x3394, 0x60},
    {0x3395, 0xff},
    {0x3396, 0x08},
    {0x3397, 0x09},
    {0x3398, 0x0f},
    {0x3399, 0x0a},
    {0x339a, 0x18},
    {0x339b, 0x60},
    {0x339c, 0xff},
    {0x33a2, 0x04},
    {0x33ad, 0x0c},
    {0x33b2, 0x40},
    {0x33b3, 0x30},
    {0x33f8, 0x00},
    {0x33f9, 0xa0},
    {0x33fa, 0x00},
    {0x33fb, 0xe0},
    {0x33fc, 0x09},
    {0x33fd, 0x1f},
    {0x349f, 0x03},
    {0x34a6, 0x09},
    {0x34a7, 0x1f},
    {0x34a8, 0x28},
    {0x34a9, 0x28},
    {0x34aa, 0x01},
    {0x34ab, 0xd0},
    {0x34ac, 0x02},
    {0x34ad, 0x10},
    {0x34f8, 0x1f},
    {0x34f9, 0x20},
    {0x3630, 0xc0},
    {0x3631, 0x84},
    {0x3633, 0x44},
    {0x3637, 0x4c},
    {0x3641, 0x38},
    {0x3670, 0x56},
    {0x3674, 0xc0},
    {0x3675, 0xa0},
    {0x3676, 0xa0},
    {0x3677, 0x84},
    {0x3678, 0x88},
    {0x3679, 0x8d},
    {0x367c, 0x09},
    {0x367d, 0x0b},
    {0x367e, 0x08},
    {0x367f, 0x0f},
    {0x3696, 0x45},
    {0x3697, 0x55},
    {0x3698, 0x56},
    {0x36a0, 0x0f},
    {0x36a1, 0x1f},
    {0x36b0, 0x81},
    {0x36b1, 0x83},
    {0x36b2, 0x85},
    {0x36b3, 0x8b},
    {0x36b4, 0x09},
    {0x36b5, 0x0b},
    {0x36b6, 0x0f},
    {0x36ea, 0x07},
    {0x36eb, 0x04},
    {0x36ec, 0x1c},
    {0x36ed, 0x2a},
    {0x370f, 0x01},
    {0x3722, 0x09},
    {0x3724, 0x21},
    {0x3771, 0x09},
    {0x3772, 0x05},
    {0x3773, 0x05},
    {0x377a, 0x0f},
    {0x377b, 0x1f},
    {0x37fa, 0x0e},
    {0x37fb, 0x31},
    {0x37fc, 0x11},
    {0x37fd, 0x16},
    {0x3905, 0x8c},
    {0x391d, 0x04},
    {0x391e, 0x01},
    {0x391f, 0x49},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x0a},
    {0x3937, 0x78},
    {0x39dc, 0x02},
    {0x3e00, 0x00},
    {0x3e01, 0x2e},
    {0x3e02, 0x60},
    {0x440e, 0x02},
    {0x4509, 0x28},
    {0x450d, 0x32},
    {0x4819, 0x04},
    {0x481b, 0x03},
    {0x481d, 0x09},
    {0x481f, 0x02},
    {0x4821, 0x08},
    {0x4823, 0x02},
    {0x4825, 0x02},
    {0x4827, 0x02},
    {0x4829, 0x04},
    {0x5000, 0x46},
    {0x578d, 0x40},
    {0x5799, 0x46},
    {0x579a, 0x77},
    {0x57d9, 0x46},
    {0x57da, 0x77},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x5988, 0x70},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x38},
    {0x5ae3, 0x30},
    {0x5ae4, 0x28},
    {0x5ae5, 0x38},
    {0x5ae6, 0x30},
    {0x5ae7, 0x28},
    {0x5ae8, 0x3f},
    {0x5ae9, 0x34},
    {0x5aea, 0x2c},
    {0x5aeb, 0x3f},
    {0x5aec, 0x34},
    {0x5aed, 0x2c},
    {0x36e9, 0x53},
    {0x37f9, 0x53},
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 630Mbps, 2lane
 * full resolution
 */
static const struct sc401ai_sensor_reg g_sc401ai_2560x1440_30fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x03},
    {0x30b8, 0x44},
    {0x3253, 0x10},
    {0x3301, 0x0a},
    {0x3302, 0xff},
    {0x3305, 0x00},
    {0x3306, 0x90},
    {0x3308, 0x08},
    {0x330a, 0x01},
    {0x330b, 0xb0},
    {0x330d, 0xf0},
    {0x3333, 0x10},
    {0x335e, 0x06},
    {0x335f, 0x0a},
    {0x3364, 0x5e},
    {0x337d, 0x0e},
    {0x338f, 0x20},
    {0x3390, 0x08},
    {0x3391, 0x09},
    {0x3392, 0x0f},
    {0x3393, 0x18},
    {0x3394, 0x60},
    {0x3395, 0xff},
    {0x3396, 0x08},
    {0x3397, 0x09},
    {0x3398, 0x0f},
    {0x3399, 0x0a},
    {0x339a, 0x18},
    {0x339b, 0x60},
    {0x339c, 0xff},
    {0x33a2, 0x04},
    {0x33ad, 0x0c},
    {0x33b2, 0x40},
    {0x33b3, 0x30},
    {0x33f8, 0x00},
    {0x33f9, 0xa0},
    {0x33fa, 0x00},
    {0x33fb, 0xe0},
    {0x33fc, 0x09},
    {0x33fd, 0x1f},
    {0x349f, 0x03},
    {0x34a6, 0x09},
    {0x34a7, 0x1f},
    {0x34a8, 0x28},
    {0x34a9, 0x28},
    {0x34aa, 0x01},
    {0x34ab, 0xd0},
    {0x34ac, 0x02},
    {0x34ad, 0x10},
    {0x34f8, 0x1f},
    {0x34f9, 0x20},
    {0x3630, 0xc0},
    {0x3631, 0x84},
    {0x3633, 0x44},
    {0x3637, 0x4c},
    {0x3641, 0x38},
    {0x3670, 0x56},
    {0x3674, 0xc0},
    {0x3675, 0xa0},
    {0x3676, 0xa0},
    {0x3677, 0x84},
    {0x3678, 0x88},
    {0x3679, 0x8d},
    {0x367c, 0x09},
    {0x367d, 0x0b},
    {0x367e, 0x08},
    {0x367f, 0x0f},
    {0x3696, 0x44},
    {0x3697, 0x54},
    {0x3698, 0x54},
    {0x36a0, 0x0f},
    {0x36a1, 0x1f},
    {0x36b0, 0x81},
    {0x36b1, 0x83},
    {0x36b2, 0x85},
    {0x36b3, 0x8b},
    {0x36b4, 0x09},
    {0x36b5, 0x0b},
    {0x36b6, 0x0f},
    {0x36ea, 0x07},
    {0x36eb, 0x04},
    {0x36ec, 0x0c},
    {0x36ed, 0xaa},
    {0x370f, 0x01},
    {0x3722, 0x09},
    {0x3724, 0x21},
    {0x3771, 0x09},
    {0x3772, 0x05},
    {0x3773, 0x05},
    {0x377a, 0x0f},
    {0x377b, 0x1f},
    {0x37fa, 0x07},
    {0x37fb, 0x31},
    {0x37fc, 0x11},
    {0x37fd, 0x16},
    {0x3905, 0x8c},
    {0x391d, 0x04},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x03},
    {0x3935, 0x00},
    {0x3936, 0x08},
    {0x3937, 0x74},
    {0x3938, 0x6f},
    {0x3939, 0x00},
    {0x393a, 0x00},
    {0x39dc, 0x02},
    {0x3e00, 0x00},
    {0x3e01, 0x5d},
    {0x3e02, 0x40},
    {0x440e, 0x02},
    {0x4509, 0x28},
    {0x450d, 0x32},
    {0x5000, 0x06},
    {0x5799, 0x46},
    {0x579a, 0x77},
    {0x57d9, 0x46},
    {0x57da, 0x77},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x38},
    {0x5ae3, 0x30},
    {0x5ae4, 0x28},
    {0x5ae5, 0x38},
    {0x5ae6, 0x30},
    {0x5ae7, 0x28},
    {0x5ae8, 0x3f},
    {0x5ae9, 0x34},
    {0x5aea, 0x2c},
    {0x5aeb, 0x3f},
    {0x5aec, 0x34},
    {0x5aed, 0x2c},
    {0x36e9, 0x53},
    {0x37f9, 0x23},
    {0x320e, 0x07},
    {0x320f, 0x08},
    {REG_END, 0x00},
};
#endif

static const struct sc401ai_mode supported_modes[] =
{
    [SC401AI_1280X720] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1280,
        .height = 720,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x80,
        .hts_def = 0x0578 * 2,
        .vts_def = 0x02ee,
        .xvclk = 24000000,
        .link_freq = 157500000,
        .reg_list = g_sc401ai_1280x720_60fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc401ai_1280x720_60fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [SC401AI_2560X1440] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2560,
        .height = 1440,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x0578 * 2,
        .vts_def = 0x0708,
        .xvclk = 24000000,
        .link_freq = 315000000,
        .reg_list = g_sc401ai_2560x1440_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc401ai_2560x1440_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct sc401ai_ops
{
    ret_err_t (*init)(struct sc401ai_dev *dev);
    ret_err_t (*open)(struct sc401ai_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc401ai_dev *dev);
    ret_err_t (*control)(struct sc401ai_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc401ai_read_reg(struct rt_i2c_bus_device *bus,
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
    msg[0].addr = g_sc401ai.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_sc401ai.i2c_addr;
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
            sc401ai_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc401ai_write_reg(struct rt_i2c_bus_device *bus,
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

    msg.addr = g_sc401ai.i2c_addr;
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
            sc401ai_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int sc401ai_write_multiple_reg_continue(struct sc401ai_dev *dev,
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
        msg.addr = g_sc401ai.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            sc401ai_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t sc401ai_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msgs.addr = g_sc401ai.i2c_addr;
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
            sc401ai_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc401ai_write_multiple_reg(struct sc401ai_dev *dev,
                                      const struct sc401ai_sensor_reg *reg, int len)
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

                sc401ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

                sc401ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

                    sc401ai_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc401ai_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc401ai_info("id: %d, width: %d, height: %d\n",
                    id, supported_modes[id].width, supported_modes[id].height);
        sc401ai_write_multiple_reg((struct sc401ai_dev *)dev,
                                  supported_modes[id].reg_list,
                                  supported_modes[id].reg_list_size);
    }
}
#endif

static int sc401ai_get_gain_reg(uint8_t *again, uint8_t *dgain,
                               uint8_t *again_fine, uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint8_t coarse_gain = 1, dig_gain = 1;
    uint32_t dcg_gainx100 = 1, ana_fine_gainx64 = 1;
    uint8_t coarse_gain_reg = 0,  dig_gain_reg = 0;
    uint8_t ana_fine_gain_reg = 0x20, dig_fine_gain_reg = 0x80;

    total_gain = total_gain * 16;
    if (total_gain <= 1024)
        total_gain = 1024;
    else if (total_gain > 24 * 32 * 64 * 16)
        total_gain = 24 * 32 * 64;

    if (total_gain < 1504) {    // start again
        dcg_gainx100 = 100;
        coarse_gain = 1;
        dig_gain = 1;
        *again = coarse_gain_reg = 0x03;
        *dgain = dig_gain_reg = 0x0;
        *dgain_fine = dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 3008) {
        dcg_gainx100 = 147;
        coarse_gain = 1;
        dig_gain = 1;
        *again = coarse_gain_reg = 0x23;
        *dgain = dig_gain_reg = 0x0;
        *dgain_fine = dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 6017) {
        dcg_gainx100 = 147;
        coarse_gain = 2;
        dig_gain = 1;
        *again = coarse_gain_reg = 0x27;
        *dgain = dig_gain_reg = 0x0;
        *dgain_fine = dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 12034) {
        dcg_gainx100 = 147;
        coarse_gain = 4;
        dig_gain = 1;
        *again = coarse_gain_reg = 0x2f;
        *dgain = dig_gain_reg = 0x0;
        *dgain_fine = dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 23879) {           // end again
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 1;
        *again = coarse_gain_reg = 0x3f;
        *dgain = dig_gain_reg = 0x0;
        *dgain_fine = dig_fine_gain_reg = 0x80;
    } else if (total_gain < 23879 * 2) {         // start dgain
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 1;
        ana_fine_gainx64 = 127;
        *again = coarse_gain_reg = 0x3f;
        *dgain = dig_gain_reg = 0x0;
        *again_fine = ana_fine_gain_reg = 0x7f;
    } else if (total_gain < 23879 * 4) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 2;
        ana_fine_gainx64 = 127;
        *again = coarse_gain_reg = 0x3f;
        *dgain = dig_gain_reg = 0x1;
        *again_fine = ana_fine_gain_reg = 0x7f;
    } else if (total_gain < 23879 * 8) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 4;
        ana_fine_gainx64 = 127;
        *again = coarse_gain_reg = 0x3f;
        *dgain = dig_gain_reg = 0x3;
        *again_fine = ana_fine_gain_reg = 0x7f;
    } else if (total_gain < 23879 * 16) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 8;
        ana_fine_gainx64 = 127;
        *again = coarse_gain_reg = 0x3f;
        *dgain = dig_gain_reg = 0x7;
        *again_fine = ana_fine_gain_reg = 0x7f;
    } else if (total_gain <= 1754822) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 16;
        ana_fine_gainx64 = 127;
        *again = coarse_gain_reg = 0x3f;
        *dgain = dig_gain_reg = 0xF;
        *again_fine = ana_fine_gain_reg = 0x7f;
    }
    return ret;
}

static ret_err_t rk_sc401ai_set_expval(struct sc401ai_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint32_t total_gain;
    uint8_t coarse_gain = 1, dig_gain = 1;
    uint32_t dcg_gainx100 = 1, ana_fine_gainx64 = 1;
    uint8_t coarse_gain_reg = 0,  dig_gain_reg = 0;
    uint8_t ana_fine_gain_reg = 0x20, dig_fine_gain_reg = 0x80;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc401ai_dbg("sc401ai don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0] << 1;
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC401AI_EXP_MIN)
        l_exp_time = SC401AI_EXP_MIN;

    sc401ai_dbg("total_time reg: 0x%x, total_gain reg 0x%x\n",
               l_exp_time, l_a_gain);

    ret = sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_EXP_LONG_H,
                           (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_EXP_LONG_M,
                           (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_EXP_LONG_L,
                           (uint8_t)((l_exp_time & 0xf) << 4));

    total_gain = l_a_gain;
    total_gain = total_gain * 16;
    if (total_gain <= 1024)
        total_gain = 1024;
    else if (total_gain > 24 * 32 * 64 * 16)
        total_gain = 24 * 32 * 64;

    if (total_gain < 1504) {               // start again
        dcg_gainx100 = 100;
        coarse_gain = 1;
        dig_gain = 1;
        coarse_gain_reg = 0x03;
        dig_gain_reg = 0x0;
        dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 3008) {
        dcg_gainx100 = 147;
        coarse_gain = 1;
        dig_gain = 1;
        coarse_gain_reg = 0x23;
        dig_gain_reg = 0x0;
        dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 6017) {
        dcg_gainx100 = 147;
        coarse_gain = 2;
        dig_gain = 1;
        coarse_gain_reg = 0x27;
        dig_gain_reg = 0x0;
        dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 12034) {
        dcg_gainx100 = 147;
        coarse_gain = 4;
        dig_gain = 1;
        coarse_gain_reg = 0x2f;
        dig_gain_reg = 0x0;
        dig_fine_gain_reg = 0x80;
    } else if (total_gain <= 23879) {           // end again
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 1;
        coarse_gain_reg = 0x3f;
        dig_gain_reg = 0x0;
        dig_fine_gain_reg = 0x80;
    } else if (total_gain < 23879 * 2) {         // start dgain
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 1;
        ana_fine_gainx64 = 127;
        coarse_gain_reg = 0x3f;
        dig_gain_reg = 0x0;
        ana_fine_gain_reg = 0x7f;
    } else if (total_gain < 23879 * 4) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 2;
        ana_fine_gainx64 = 127;
        coarse_gain_reg = 0x3f;
        dig_gain_reg = 0x1;
        ana_fine_gain_reg = 0x7f;
    } else if (total_gain < 23879 * 8) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 4;
        ana_fine_gainx64 = 127;
        coarse_gain_reg = 0x3f;
        dig_gain_reg = 0x3;
        ana_fine_gain_reg = 0x7f;
    } else if (total_gain < 23879 * 16) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 8;
        ana_fine_gainx64 = 127;
        coarse_gain_reg = 0x3f;
        dig_gain_reg = 0x7;
        ana_fine_gain_reg = 0x7f;
    } else if (total_gain <= 1754822) {
        dcg_gainx100 = 147;
        coarse_gain = 8;
        dig_gain = 16;
        ana_fine_gainx64 = 127;
        coarse_gain_reg = 0x3f;
        dig_gain_reg = 0xF;
        ana_fine_gain_reg = 0x7f;
    }
    if (total_gain < 1504)
        ana_fine_gain_reg = abs(100 * total_gain / (dcg_gainx100 * coarse_gain) / 16);
    else if (total_gain == 1504)
        ana_fine_gain_reg = 0x40;
    else if (total_gain < 23879)
        ana_fine_gain_reg = abs(100 * total_gain / (dcg_gainx100 * coarse_gain) / 16);
    else
        dig_fine_gain_reg = abs(800 * total_gain / (dcg_gainx100 * coarse_gain *
                                dig_gain) / ana_fine_gainx64);


    sc401ai_dbg("a_gain: 0x%x d_gain: 0x%x, a_gain_fine: 0x%x, d_gain_fine: 0x%x\n",
                 coarse_gain_reg, dig_gain_reg, ana_fine_gain_reg, dig_fine_gain_reg);

    ret = sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_ANA_GAIN, coarse_gain_reg);
    ret |= sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_ANA_FINE_GAIN, ana_fine_gain_reg);
    ret |= sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_DIG_GAIN, dig_gain_reg);
    ret |= sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_DIG_FINE_GAIN, dig_fine_gain_reg);
    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc401ai_set_vts(struct sc401ai_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc401ai_dbg("set vts: 0x%x \n", dst_vts);

    ret = sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_VTS_L,
                           (uint8_t)(dst_vts & 0xff));
    ret = sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_VTS_H,
                           (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc401ai_set_flip_mirror(struct sc401ai_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc401ai_read_reg(dev->i2c_bus, SC401AI_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC401AI_FETCH_MIRROR(val, flip & 0x01);
        val = SC401AI_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC401AI_FETCH_MIRROR(val, false);
        val = SC401AI_FETCH_FLIP(val, false);
        break;
    };
    sc401ai_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc401ai_write_reg(dev->i2c_bus, SC401AI_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc401ai_stream_on(struct sc401ai_dev *dev)
{
    sc401ai_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(5);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    sc401ai_write_multiple_reg_continue((struct sc401ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc401ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc401ai_set_flip_mirror(dev, dev->flip);

    sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_STREAM_CTRL, SC401AI_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc401ai_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc401ai_stream_on_late(struct sc401ai_dev *dev)
{

    sc401ai_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc401ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc401ai_set_flip_mirror(dev, dev->flip);

    sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_STREAM_CTRL, SC401AI_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc401ai_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc401ai_stream_off(struct sc401ai_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc401ai_write_reg(dev->i2c_bus, SC401AI_REG_STREAM_CTRL, SC401AI_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc401ai_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_sc401ai_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc401ai_dev *sc401ai;

    sc401ai = (struct sc401ai_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc401ai->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc401ai);
#endif

    if (sc401ai)
    {
        camera->info.mbus_fmt.width = sc401ai->cur_mode->width;
        camera->info.mbus_fmt.height = sc401ai->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc401ai->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc401ai->cur_mode->hdr_mode;

        sc401ai->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc401ai->i2c_name);

        if (!sc401ai->i2c_bus)
        {
            sc401ai_err("not find i2c :%s !!!\n",
                       sc401ai->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc401ai_dbg("find i2c_bus:%s\n", sc401ai->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc401ai_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc401ai_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_sc401ai_get_expinf(struct sc401ai_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc401ai_mode *mode;

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

static ret_err_t rk_sc401ai_get_intput_fmt(struct sc401ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc401ai_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc401ai_set_intput_fmt(struct sc401ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc401ai_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc401ai_info("dst resulotion, width %d, height %d\n",
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
            sc401ai_info(">>> switch to dst fmt, dst_width %d, dst_height %d, dst_fps %d, hdr: %d, dst_vts: 0x%x\n",
                        dev->cur_mode->width, dev->cur_mode->height,
                        dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator,
                        dev->cur_mode->hdr_mode, dev->cur_mode->vts_def);

            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
#ifndef REG_LIST_CONVERSION_DEBUG
            sc401ai_write_multiple_reg_continue((struct sc401ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc401ai_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc401ai_match_dst_config(struct sc401ai_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc401ai_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc401ai_info("cur resulotion, width %d, height %d, fps %d!\n",
                dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc401ai_mode *)mode;
            ret = RET_SYS_EOK;
            sc401ai_info("find dst resulotion, width %d, height %d, fps %d!\n",
                        dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc401ai_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc401ai_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc401ai_control(struct rk_camera_device *dev,
                            dt_cmd_t cmd,
                            void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc401ai_dev *sc401ai;

    RT_ASSERT(dev != RT_NULL);
    sc401ai = (struct sc401ai_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc401ai_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc401ai_stream_on(sc401ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc401ai_stream_off(sc401ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc401ai_get_expinf(sc401ai, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc401ai_set_expval(sc401ai, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc401ai_set_vts(sc401ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc401ai_get_intput_fmt(sc401ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc401ai_set_intput_fmt(sc401ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc401ai_match_dst_config(sc401ai, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc401ai_set_flip_mirror(sc401ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc401ai_stream_on_late(sc401ai);
    }
    break;
    default:
        sc401ai_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc401ai_ops =
{
    rk_sc401ai_init,
    rk_sc401ai_open,
    NULL,
    rk_sc401ai_control
};

int rk_camera_sc401ai_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc401ai_dev *instance = &g_sc401ai;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *sc401ai = (struct camera_board_desc *)&camera_sc401ai;

    camera->ops = &rk_sc401ai_ops;

    rt_strncpy(instance->name, sc401ai->isp_subdev_name, rt_strlen(sc401ai->isp_subdev_name));
    rt_strncpy(instance->i2c_name, sc401ai->i2c_bus, rt_strlen(sc401ai->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (sc401ai->mode_id <= SC401AI_MODE_ID_MAX)
    {
        sc401ai_dbg("mode_id: %d\n", sc401ai->mode_id);
        instance->cur_mode = &supported_modes[sc401ai->mode_id];
    }
    else
    {
        sc401ai_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct sc401ai_mode *)&supported_modes[SC401AI_2560X1440];
    instance->time_valid_delay = sc401ai->time_valid_delay;
    instance->gain_valid_delay = sc401ai->gain_valid_delay;
    instance->i2c_addr = sc401ai->i2c_addr;
    instance->flip = 0;

    HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc401ai->rst_gpio.gpio_group, sc401ai->rst_gpio.gpio_pin, GPIO_OUT);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc401ai->rst_gpio.gpio_group, sc401ai->rst_gpio.gpio_pin, !sc401ai->rst_active);

    if (sc401ai->mclk_out_gate_id && sc401ai->mclk_id)
    {
        clk_set_rate(sc401ai->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(sc401ai->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "SC401AI_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, sc401ai->isp_subdev_name, rt_strlen(sc401ai->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void sc401ai_detect(void)
{
    struct sc401ai_dev *instance = &g_sc401ai;

    sc401ai_dbg("start to detect SC401AI for testing \n");
    sc401ai_dbg("dev name:%s\n", instance->name);
    sc401ai_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        sc401ai_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        sc401ai_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc401ai_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc401ai_detect, check SC401AI is available or not);
#endif
#endif
#endif
