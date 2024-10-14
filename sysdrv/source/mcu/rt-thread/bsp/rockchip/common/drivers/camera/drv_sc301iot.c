/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc301iot.c
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

#ifdef RT_USING_SC301IOT

//#define REG_LIST_CONVERSION_DEBUG
#define SC301IOT_DEBUG_PRT      0

#if SC301IOT_DEBUG_PRT
#include <stdio.h>
#define sc301iot_dbg(fmt, args...)               rt_kprintf("[SC301IOT] "fmt"", ##args)
#else
#define sc301iot_dbg(fmt, args...)
#endif

#define sc301iot_info(fmt, args...)         rt_kprintf("[SC301IOT] "fmt"", ##args)
#define sc301iot_err(fmt, args...)         rt_kprintf("[SC301IOT] ERR "fmt"", ##args)

#define SC301IOT_REG_EXP_LONG_H           0x3e00
#define SC301IOT_REG_EXP_LONG_M           0x3e01
#define SC301IOT_REG_EXP_LONG_L           0x3e02
#define SC301IOT_REG_EXP_SHORT_H          0x3e22
#define SC301IOT_REG_EXP_SHORT_M          0x3e04
#define SC301IOT_REG_EXP_SHORT_L          0x3e05
#define SC301IOT_REG_DIG_GAIN            0x3e06
#define SC301IOT_REG_DIG_FINE_GAIN       0x3e07
#define SC301IOT_REG_ANA_GAIN            0x3e09
#define SC301IOT_REG_SDIG_GAIN           0x3e10
#define SC301IOT_REG_SDIG_FINE_GAIN      0x3e11
#define SC301IOT_REG_SANA_GAIN           0x3e12
#define SC301IOT_REG_SANA_FINE_GAIN      0x3e13
#define SC301IOT_EXP_MIN                 0
#define SC301IOT_GAIN_MIN                0x0040
#define SC301IOT_GAIN_MAX                50610       //50.208*31.5*64
#define SC301IOT_GAIN_STEP               1
#define SC301IOT_GAIN_DEFAULT            0x0040
#define SC301IOT_LGAIN                   0
#define SC301IOT_SGAIN                   1
#define SC301IOT_FLIP_MIRROR_REG         0x3221
#define SC301IOT_REG_VTS_H               0x320e
#define SC301IOT_REG_VTS_L               0x320f
#define SC301IOT_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC301IOT_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC301IOT_REG_STREAM_CTRL         0x0100
#define SC301IOT_STREAM_ON               0x01
#define SC301IOT_STREAM_OFF              0x00

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

struct sc301iot_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc301iot_mode
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
    const struct sc301iot_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc301iot_dev
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
    const struct sc301iot_mode *cur_mode;
    struct sc301iot_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc301iot_dev *rt_sc301iot_dev_t;

static struct sc301iot_dev g_sc301iot;

#ifndef REG_LIST_CONVERSION_DEBUG
static const uint8_t g_SC301IOT_1000x750_120fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1c, 0x78,
    0x3, 0x30, 0x1f, 0x49,
    0x3, 0x30, 0xb8, 0x44,
    0x18, 0x32, 0x00, 0x00, 0x00, 0x00, 0x12, 0x08, 0x07, 0x05, 0xf5, 0x03, 0xe8, 0x02, 0xee, 0x04, 0x98, 0x02, 0xfd, 0x00, 0x02, 0x00, 0x02, 0x11, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x23, 0xd0,
    0x3, 0x32, 0x31, 0x01,
    0x3, 0x32, 0x53, 0x0c,
    0x3, 0x32, 0x74, 0x09,
    0x3, 0x33, 0x01, 0x08,
    0x3, 0x33, 0x04, 0x80,
    0x3, 0x33, 0x06, 0x58,
    0x6, 0x33, 0x08, 0x08, 0xa0, 0x00, 0xe0,
    0x3, 0x33, 0x0e, 0x10,
    0x3, 0x33, 0x14, 0x14,
    0x4, 0x33, 0x1e, 0x71, 0x91,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x08, 0x08, 0x08, 0x08, 0x09, 0x1f, 0x08, 0x10, 0x3c, 0x78,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xad, 0x0c,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x38,
    0x3, 0x33, 0xf9, 0x58,
    0x5, 0x33, 0xfb, 0x80, 0x48, 0x4f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x48, 0x4f, 0x38, 0x28, 0x00, 0xe0, 0x01, 0x08,
    0x4, 0x34, 0xf8, 0x5f, 0x18,
    0x6, 0x36, 0x30, 0xf0, 0x85, 0x74, 0x22,
    0x4, 0x36, 0x37, 0x4d, 0xcb,
    0x3, 0x36, 0x3a, 0x8b,
    0x3, 0x36, 0x3c, 0x08,
    0x3, 0x36, 0x41, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xc0, 0xa0, 0x90, 0x83, 0x86, 0x89,
    0x6, 0x36, 0x7c, 0x48, 0x4f, 0x48, 0x4b,
    0x5, 0x36, 0x90, 0x33, 0x44, 0x55,
    0x7, 0x36, 0x99, 0x8a, 0xa1, 0xc2, 0x48, 0x4f,
    0x4, 0x36, 0xa2, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x0f, 0x0c, 0x1c, 0x15,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x14, 0x80,
    0x3, 0x37, 0x22, 0x01,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x3, 0x37, 0x28, 0x00,
    0x5, 0x37, 0x71, 0x09, 0x09, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x4f,
    0x6, 0x37, 0xfa, 0x09, 0x31, 0x10, 0x18,
    0x3, 0x39, 0x05, 0x8d,
    0x3, 0x39, 0x1d, 0x08,
    0x3, 0x39, 0x22, 0x1a,
    0x3, 0x39, 0x26, 0x21,
    0x4, 0x39, 0x33, 0x80, 0x0d,
    0x3, 0x39, 0x37, 0x6a,
    0x4, 0x39, 0x39, 0x00, 0x0e,
    0x3, 0x39, 0xdc, 0x02,
    0x6, 0x3e, 0x00, 0x00, 0x2f, 0x50, 0x0b,
    0x3, 0x3e, 0x1b, 0x2a,
    0x3, 0x44, 0x07, 0x34,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x10,
    0x3, 0x48, 0x19, 0x07,
    0x3, 0x48, 0x1b, 0x04,
    0x3, 0x48, 0x1d, 0x0f,
    0x3, 0x48, 0x1f, 0x03,
    0x3, 0x48, 0x21, 0x09,
    0x3, 0x48, 0x23, 0x04,
    0x3, 0x48, 0x25, 0x03,
    0x3, 0x48, 0x27, 0x04,
    0x3, 0x48, 0x29, 0x06,
    0x4, 0x50, 0x00, 0x46, 0x40,
    0x3, 0x50, 0x07, 0x80,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x24,
    0x3, 0x30, 0x28, 0x05,
    0x52,
};

static const uint8_t g_SC301IOT_2048x1536_30fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1c, 0x78,
    0x3, 0x30, 0x1f, 0x11,
    0x3, 0x30, 0xb8, 0x44,
    0xa, 0x32, 0x08, 0x08, 0x00, 0x06, 0x00, 0x04, 0x65, 0x06, 0x40,
    0x4, 0x32, 0x14, 0x11, 0x11,
    0x3, 0x32, 0x53, 0x0c,
    0x3, 0x32, 0x74, 0x09,
    0x3, 0x33, 0x01, 0x08,
    0x3, 0x33, 0x06, 0x58,
    0x3, 0x33, 0x08, 0x08,
    0x4, 0x33, 0x0a, 0x00, 0xe0,
    0x3, 0x33, 0x0e, 0x10,
    0x3, 0x33, 0x14, 0x14,
    0x4, 0x33, 0x1e, 0x55, 0x7d,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x08, 0x08, 0x08, 0x08, 0x09, 0x1f, 0x08, 0x0a, 0x40, 0x88,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xad, 0x0c,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x30,
    0x3, 0x33, 0xf9, 0x68,
    0x5, 0x33, 0xfb, 0x80, 0x48, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x48, 0x5f, 0x30, 0x30, 0x00, 0xf0, 0x01, 0x08,
    0x4, 0x34, 0xf8, 0x5f, 0x10,
    0x6, 0x36, 0x30, 0xf0, 0x85, 0x74, 0x22,
    0x4, 0x36, 0x37, 0x4d, 0xcb,
    0x3, 0x36, 0x3a, 0x8b,
    0x3, 0x36, 0x3c, 0x08,
    0x4, 0x36, 0x40, 0x00, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xc0, 0xb0, 0xa0, 0x83, 0x87, 0x8a,
    0x6, 0x36, 0x7c, 0x49, 0x4f, 0x48, 0x4b,
    0x5, 0x36, 0x90, 0x33, 0x33, 0x44,
    0x7, 0x36, 0x99, 0x8a, 0xa1, 0xc2, 0x48, 0x4f,
    0x4, 0x36, 0xa2, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x09, 0x0d, 0x1c, 0x25,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x14, 0x00,
    0x3, 0x37, 0x22, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x3, 0x37, 0x28, 0x00,
    0x5, 0x37, 0x71, 0x09, 0x05, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x49,
    0x6, 0x37, 0xfa, 0x09, 0x33, 0x11, 0x18,
    0x3, 0x39, 0x05, 0x8d,
    0x3, 0x39, 0x1d, 0x08,
    0x3, 0x39, 0x22, 0x1a,
    0x3, 0x39, 0x26, 0x21,
    0x4, 0x39, 0x33, 0x80, 0x0d,
    0x3, 0x39, 0x37, 0x6a,
    0x4, 0x39, 0x39, 0x00, 0x0e,
    0x3, 0x39, 0xdc, 0x02,
    0x6, 0x3e, 0x00, 0x00, 0x63, 0x80, 0x0b,
    0x3, 0x3e, 0x1b, 0x2a,
    0x3, 0x44, 0x07, 0x34,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x50, 0x01, 0x40,
    0x3, 0x50, 0x07, 0x80,
    0x3, 0x36, 0xe9, 0x24,
    0x3, 0x37, 0xf9, 0x24,
    0x3, 0x32, 0x51, 0x90,
    0x4, 0x36, 0x50, 0x33, 0x7f,
    0x3, 0x30, 0x28, 0x05,
    0x47,
};
#else

/*
 * Xclk 24Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 540Mbps, 2lane
 * crop to 2000x1500 then binning to 1000x750
 */
static const struct SC301IOT_sensor_reg g_SC301IOT_1000x750_120fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301c, 0x78},
    {0x301f, 0x49},
    {0x30b8, 0x44},
    {0x3200, 0x00},
    {0x3201, 0x00},
    {0x3202, 0x00},
    {0x3203, 0x12},
    {0x3204, 0x08},
    {0x3205, 0x07},
    {0x3206, 0x05},
    {0x3207, 0xf5},
    {0x3208, 0x03},
    {0x3209, 0xe8},
    {0x320a, 0x02},
    {0x320b, 0xee},
    {0x320c, 0x04},
    {0x320d, 0x98},
    {0x320e, 0x02},
    {0x320f, 0xfd},
    {0x3210, 0x00},
    {0x3211, 0x02},
    {0x3212, 0x00},
    {0x3213, 0x02},
    {0x3214, 0x11},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3223, 0xd0},
    {0x3231, 0x01},
    {0x3253, 0x0c},
    {0x3274, 0x09},
    {0x3301, 0x08},
    {0x3304, 0x80},
    {0x3306, 0x58},
    {0x3308, 0x08},
    {0x3309, 0xa0},
    {0x330a, 0x00},
    {0x330b, 0xe0},
    {0x330e, 0x10},
    {0x3314, 0x14},
    {0x331e, 0x71},
    {0x331f, 0x91},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335e, 0x06},
    {0x335f, 0x08},
    {0x3364, 0x5e},
    {0x337c, 0x02},
    {0x337d, 0x0a},
    {0x3390, 0x01},
    {0x3391, 0x03},
    {0x3392, 0x07},
    {0x3393, 0x08},
    {0x3394, 0x08},
    {0x3395, 0x08},
    {0x3396, 0x08},
    {0x3397, 0x09},
    {0x3398, 0x1f},
    {0x3399, 0x08},
    {0x339a, 0x10},
    {0x339b, 0x3c},
    {0x339c, 0x78},
    {0x33a2, 0x04},
    {0x33ad, 0x0c},
    {0x33b1, 0x80},
    {0x33b3, 0x38},
    {0x33f9, 0x58},
    {0x33fb, 0x80},
    {0x33fc, 0x48},
    {0x33fd, 0x4f},
    {0x349f, 0x03},
    {0x34a6, 0x48},
    {0x34a7, 0x4f},
    {0x34a8, 0x38},
    {0x34a9, 0x28},
    {0x34aa, 0x00},
    {0x34ab, 0xe0},
    {0x34ac, 0x01},
    {0x34ad, 0x08},
    {0x34f8, 0x5f},
    {0x34f9, 0x18},
    {0x3630, 0xf0},
    {0x3631, 0x85},
    {0x3632, 0x74},
    {0x3633, 0x22},
    {0x3637, 0x4d},
    {0x3638, 0xcb},
    {0x363a, 0x8b},
    {0x363c, 0x08},
    {0x3641, 0x38},
    {0x3670, 0x4e},
    {0x3674, 0xc0},
    {0x3675, 0xa0},
    {0x3676, 0x90},
    {0x3677, 0x83},
    {0x3678, 0x86},
    {0x3679, 0x89},
    {0x367c, 0x48},
    {0x367d, 0x4f},
    {0x367e, 0x48},
    {0x367f, 0x4b},
    {0x3690, 0x33},
    {0x3691, 0x44},
    {0x3692, 0x55},
    {0x3699, 0x8a},
    {0x369a, 0xa1},
    {0x369b, 0xc2},
    {0x369c, 0x48},
    {0x369d, 0x4f},
    {0x36a2, 0x4b},
    {0x36a3, 0x4f},
    {0x36ea, 0x0f},
    {0x36eb, 0x0c},
    {0x36ec, 0x1c},
    {0x36ed, 0x15},
    {0x370f, 0x01},
    {0x3714, 0x80},
    {0x3722, 0x01},
    {0x3724, 0x41},
    {0x3725, 0xc1},
    {0x3728, 0x00},
    {0x3771, 0x09},
    {0x3772, 0x09},
    {0x3773, 0x05},
    {0x377a, 0x48},
    {0x377b, 0x4f},
    {0x37fa, 0x09},
    {0x37fb, 0x31},
    {0x37fc, 0x10},
    {0x37fd, 0x18},
    {0x3905, 0x8d},
    {0x391d, 0x08},
    {0x3922, 0x1a},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x0d},
    {0x3937, 0x6a},
    {0x3939, 0x00},
    {0x393a, 0x0e},
    {0x39dc, 0x02},
    {0x3e00, 0x00},
    {0x3e01, 0x2f},
    {0x3e02, 0x50},
    {0x3e03, 0x0b},
    {0x3e1b, 0x2a},
    {0x4407, 0x34},
    {0x440e, 0x02},
    {0x4509, 0x10},
    {0x4819, 0x07},
    {0x481b, 0x04},
    {0x481d, 0x0f},
    {0x481f, 0x03},
    {0x4821, 0x09},
    {0x4823, 0x04},
    {0x4825, 0x03},
    {0x4827, 0x04},
    {0x4829, 0x06},
    {0x5000, 0x46},
    {0x5001, 0x40},
    {0x5007, 0x80},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x36e9, 0x53},
    {0x37f9, 0x24},
    {0x3028, 0x05},
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 540Mbps, 2lane
 * full resolution
 */
static const struct SC301IOT_sensor_reg g_SC301IOT_2048x1536_30fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301c, 0x78},
    {0x301f, 0x11},
    {0x30b8, 0x44},
    {0x3208, 0x08},
    {0x3209, 0x00},
    {0x320a, 0x06},
    {0x320b, 0x00},
    {0x320c, 0x04},
    {0x320d, 0x65},
    {0x320e, 0x06},
    {0x320f, 0x40},
    {0x3214, 0x11},
    {0x3215, 0x11},
    // {0x3223, 0xc0},
    {0x3253, 0x0c},
    {0x3274, 0x09},
    {0x3301, 0x08},
    {0x3306, 0x58},
    {0x3308, 0x08},
    {0x330a, 0x00},
    {0x330b, 0xe0},
    {0x330e, 0x10},
    {0x3314, 0x14},
    {0x331e, 0x55},
    {0x331f, 0x7d},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335e, 0x06},
    {0x335f, 0x08},
    {0x3364, 0x5e},
    {0x337c, 0x02},
    {0x337d, 0x0a},
    {0x3390, 0x01},
    {0x3391, 0x03},
    {0x3392, 0x07},
    {0x3393, 0x08},
    {0x3394, 0x08},
    {0x3395, 0x08},
    {0x3396, 0x08},
    {0x3397, 0x09},
    {0x3398, 0x1f},
    {0x3399, 0x08},
    {0x339a, 0x0a},
    {0x339b, 0x40},
    {0x339c, 0x88},
    {0x33a2, 0x04},
    {0x33ad, 0x0c},
    {0x33b1, 0x80},
    {0x33b3, 0x30},
    {0x33f9, 0x68},
    {0x33fb, 0x80},
    {0x33fc, 0x48},
    {0x33fd, 0x5f},
    {0x349f, 0x03},
    {0x34a6, 0x48},
    {0x34a7, 0x5f},
    {0x34a8, 0x30},
    {0x34a9, 0x30},
    {0x34aa, 0x00},
    {0x34ab, 0xf0},
    {0x34ac, 0x01},
    {0x34ad, 0x08},
    {0x34f8, 0x5f},
    {0x34f9, 0x10},
    {0x3630, 0xf0},
    {0x3631, 0x85},
    {0x3632, 0x74},
    {0x3633, 0x22},
    {0x3637, 0x4d},
    {0x3638, 0xcb},
    {0x363a, 0x8b},
    {0x363c, 0x08},
    {0x3640, 0x00},
    {0x3641, 0x38},
    {0x3670, 0x4e},
    {0x3674, 0xc0},
    {0x3675, 0xb0},
    {0x3676, 0xa0},
    {0x3677, 0x83},
    {0x3678, 0x87},
    {0x3679, 0x8a},
    {0x367c, 0x49},
    {0x367d, 0x4f},
    {0x367e, 0x48},
    {0x367f, 0x4b},
    {0x3690, 0x33},
    {0x3691, 0x33},
    {0x3692, 0x44},
    {0x3699, 0x8a},
    {0x369a, 0xa1},
    {0x369b, 0xc2},
    {0x369c, 0x48},
    {0x369d, 0x4f},
    {0x36a2, 0x4b},
    {0x36a3, 0x4f},
    {0x36ea, 0x09},
    {0x36eb, 0x0d},
    {0x36ec, 0x1c},
    {0x36ed, 0x25},
    {0x370f, 0x01},
    {0x3714, 0x00},
    {0x3722, 0x09},
    {0x3724, 0x41},
    {0x3725, 0xc1},
    {0x3728, 0x00},
    {0x3771, 0x09},
    {0x3772, 0x05},
    {0x3773, 0x05},
    {0x377a, 0x48},
    {0x377b, 0x49},
    {0x37fa, 0x09},
    {0x37fb, 0x33},
    {0x37fc, 0x11},
    {0x37fd, 0x18},
    {0x3905, 0x8d},
    {0x391d, 0x08},
    {0x3922, 0x1a},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x0d},
    {0x3937, 0x6a},
    {0x3939, 0x00},
    {0x393a, 0x0e},
    {0x39dc, 0x02},
    {0x3e00, 0x00},
    {0x3e01, 0x63},
    {0x3e02, 0x80},
    {0x3e03, 0x0b},
    {0x3e1b, 0x2a},
    {0x4407, 0x34},
    {0x440e, 0x02},
    {0x5001, 0x40},
    {0x5007, 0x80},
    {0x36e9, 0x24},
    {0x37f9, 0x24},
    {0x3251, 0x90},
    /* strong signal */
    {0x3650, 0x33},
    {0x3651, 0x7f},
    {0x3028, 0x05},
    {REG_END, 0x00},
};
#endif

static const struct sc301iot_mode supported_modes[] =
{
    {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1000,
        .height = 750,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x46,
        .hts_def = 0x498,
        .vts_def = 0x02fd,
        .xvclk = 24000000,
        .link_freq = 270000000,
        .reg_list = g_SC301IOT_1000x750_120fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_SC301IOT_1000x750_120fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2048,
        .height = 1536,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x638,
        .hts_def = 0x8ca,
        .vts_def = 0x640,
        .xvclk = 24000000,
        .link_freq = 270000000,
        .reg_list = g_SC301IOT_2048x1536_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_SC301IOT_2048x1536_30fps_reg_table),
        .hdr_mode = NO_HDR,
    }
};

struct sc301iot_ops
{
    ret_err_t (*init)(struct sc301iot_dev *dev);
    ret_err_t (*open)(struct sc301iot_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc301iot_dev *dev);
    ret_err_t (*control)(struct sc301iot_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc301iot_read_reg(struct rt_i2c_bus_device *bus,
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
    msg[0].addr = g_sc301iot.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_sc301iot.i2c_addr;
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
            sc301iot_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc301iot_write_reg(struct rt_i2c_bus_device *bus,
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

    msg.addr = g_sc301iot.i2c_addr;
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
            sc301iot_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int sc301iot_write_multiple_reg_continue(struct sc301iot_dev *dev,
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
        msg.addr = g_sc301iot.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            sc301iot_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t sc301iot_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msgs.addr = g_sc301iot.i2c_addr;
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
            sc301iot_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc301iot_write_multiple_reg(struct sc301iot_dev *dev,
                                        const struct sc301iot_sensor_reg *reg, int len)
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

                sc301iot_write_reg_discontinued(i2c_bus, data, k + 2);
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

                sc301iot_write_reg_discontinued(i2c_bus, data, k + 2);
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

                    sc301iot_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc301iot_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc301iot_info("id: %d, width: %d, height: %d\n",
                      id, supported_modes[id].width, supported_modes[id].height);
        sc301iot_write_multiple_reg((struct sc301iot_dev *)dev,
                                    supported_modes[id].reg_list,
                                    supported_modes[id].reg_list_size);
    }
}
#endif

static int sc301iot_get_gain_reg(uint8_t *again, uint8_t *dgain,
                                 uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0, gain_factor;

    if (total_gain < SC301IOT_GAIN_MIN)
        total_gain = SC301IOT_GAIN_MIN;
    else if (total_gain > SC301IOT_GAIN_MAX)
        total_gain = SC301IOT_GAIN_MAX;

    gain_factor = total_gain * 1000 / SC301IOT_GAIN_MIN;
    if (gain_factor < 1569)          /* 1 ~ 1.569 gain*/
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000;
    }
    else if (gain_factor < 1569 * 2)     /* 1.569 ~ 3.138 gain*/
    {
        *again = 0x40;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1569;
    }
    else if (gain_factor < 1569 * 4)         /* 3.138 ~ 6.276 gain*/
    {
        *again = 0x48;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1569 / 2;
    }
    else if (gain_factor < 1569 * 8)         /* 6.276 ~ 12.552 gain*/
    {
        *again = 0x49;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1569 / 4;
    }
    else if (gain_factor < 1569 * 16)         /* 12.552 ~ 25.104 gain*/
    {
        *again = 0x4b;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1569 / 8;
    }
    else if (gain_factor < 1569 * 32)        /* 25.104 ~ 50.208 gain*/
    {
        *again = 0x4f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1569 / 16;
    }
    else if (gain_factor < 50208 * 2)        /* 50.208 ~ 100.416 gain*/
    {
        *again = 0x5f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 50208;
    }
    else if (gain_factor < 50208 * 4)        /* 25.104 ~ 50.02 gain*/
    {
        *again = 0x5f;
        *dgain = 0x01;
        *dgain_fine = gain_factor * 128 / 50208 / 2;
    }
    else if (gain_factor < 50208 * 8)        /* 25.104 ~ 50.02 gain*/
    {
        *again = 0x5f;
        *dgain = 0x03;
        *dgain_fine = gain_factor * 128 / 50208 / 4;
    }
    else if (gain_factor < 50208 * 16)        /* 25.104 ~ 50.02 gain*/
    {
        *again = 0x5f;
        *dgain = 0x07;
        *dgain_fine = gain_factor * 128 / 50208 / 8;
    }
    else if (gain_factor < 50208 * 32)        /* 25.104 ~ 50.02 gain*/
    {
        *again = 0x5f;
        *dgain = 0x0f;
        *dgain_fine = gain_factor * 128 / 50208 / 16;
    }
    /* Align to 1 / 32 accuracy */
    *dgain_fine = *dgain_fine / 4 * 4;

    return ret;
}

static ret_err_t rk_sc301iot_set_expval(struct sc301iot_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, d_gain_fine;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc301iot_dbg("sc301iot don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC301IOT_EXP_MIN)
        l_exp_time = SC301IOT_EXP_MIN;

    sc301iot_dbg("total_time reg: 0x%x, total_gain reg 0x%x\n",
                 l_exp_time, l_a_gain);

    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_EXP_LONG_H,
                             (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_EXP_LONG_M,
                             (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_EXP_LONG_L,
                             (uint8_t)((l_exp_time & 0xf) << 4));
    sc301iot_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    sc301iot_dbg("a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", a_gain, d_gain, d_gain_fine);

    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_ANA_GAIN, a_gain);
    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_DIG_GAIN, d_gain);
    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc301iot_set_vts(struct sc301iot_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc301iot_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_VTS_L,
                             (uint8_t)(dst_vts & 0xff));
    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_VTS_H,
                             (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc301iot_set_flip_mirror(struct sc301iot_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc301iot_read_reg(dev->i2c_bus, SC301IOT_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC301IOT_FETCH_MIRROR(val, flip & 0x01);
        val = SC301IOT_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC301IOT_FETCH_MIRROR(val, false);
        val = SC301IOT_FETCH_FLIP(val, false);
        break;
    };
    sc301iot_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc301iot_write_reg(dev->i2c_bus, SC301IOT_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc301iot_stream_on(struct sc301iot_dev *dev)
{
    sc301iot_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(5);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    sc301iot_write_multiple_reg_continue((struct sc301iot_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc301iot_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc301iot_set_flip_mirror(dev, dev->flip);

    sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_STREAM_CTRL, SC301IOT_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc301iot_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc301iot_stream_on_late(struct sc301iot_dev *dev)
{

    sc301iot_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc301iot_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc301iot_set_flip_mirror(dev, dev->flip);

    sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_STREAM_CTRL, SC301IOT_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc301iot_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc301iot_stream_off(struct sc301iot_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc301iot_write_reg(dev->i2c_bus, SC301IOT_REG_STREAM_CTRL, SC301IOT_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc301iot_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_sc301iot_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc301iot_dev *sc301iot;

    sc301iot = (struct sc301iot_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc301iot->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc301iot);
#endif

    if (sc301iot)
    {
        camera->info.mbus_fmt.width = sc301iot->cur_mode->width;
        camera->info.mbus_fmt.height = sc301iot->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc301iot->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc301iot->cur_mode->hdr_mode;

        sc301iot->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc301iot->i2c_name);

        if (!sc301iot->i2c_bus)
        {
            sc301iot_err("not find i2c :%s !!!\n",
                         sc301iot->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc301iot_dbg("find i2c_bus:%s\n", sc301iot->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc301iot_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc301iot_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_sc301iot_get_expinf(struct sc301iot_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc301iot_mode *mode;

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

static ret_err_t rk_sc301iot_get_intput_fmt(struct sc301iot_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc301iot_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc301iot_set_intput_fmt(struct sc301iot_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc301iot_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc301iot_info("dst resulotion, width %d, height %d\n",
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
            sc301iot_write_multiple_reg_continue((struct sc301iot_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc301iot_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc301iot_match_dst_config(struct sc301iot_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc301iot_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc301iot_info("cur resulotion, width %d, height %d, fps %d!\n",
                  dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc301iot_mode *)mode;
            ret = RET_SYS_EOK;
            sc301iot_info("find dst resulotion, width %d, height %d, fps %d!\n",
                          dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc301iot_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc301iot_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc301iot_control(struct rk_camera_device *dev,
                              dt_cmd_t cmd,
                              void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc301iot_dev *sc301iot;

    RT_ASSERT(dev != RT_NULL);
    sc301iot = (struct sc301iot_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc301iot_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc301iot_stream_on(sc301iot);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc301iot_stream_off(sc301iot);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc301iot_get_expinf(sc301iot, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc301iot_set_expval(sc301iot, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc301iot_set_vts(sc301iot, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc301iot_get_intput_fmt(sc301iot, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc301iot_set_intput_fmt(sc301iot, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc301iot_match_dst_config(sc301iot, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc301iot_set_flip_mirror(sc301iot, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc301iot_stream_on_late(sc301iot);
    }
    break;
    default:
        sc301iot_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc301iot_ops =
{
    rk_sc301iot_init,
    rk_sc301iot_open,
    NULL,
    rk_sc301iot_control
};

int rk_camera_sc301iot_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc301iot_dev *instance = &g_sc301iot;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *sc301iot = (struct camera_board_desc *)&camera_sc301iot;

    camera->ops = &rk_sc301iot_ops;

    rt_strncpy(instance->name, sc301iot->isp_subdev_name, rt_strlen(sc301iot->isp_subdev_name));
    rt_strncpy(instance->i2c_name, sc301iot->i2c_bus, rt_strlen(sc301iot->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (sc301iot->mode_id <= SC301IOT_MODE_ID_MAX)
    {
        sc301iot_dbg("mode_id: %d\n", sc301iot->mode_id);
        instance->cur_mode = &supported_modes[sc301iot->mode_id];
    }
    else
    {
        sc301iot_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct sc301iot_mode *)&supported_modes[SC301IOT_2048X1536];
    instance->time_valid_delay = sc301iot->time_valid_delay;
    instance->gain_valid_delay = sc301iot->gain_valid_delay;
    instance->i2c_addr = sc301iot->i2c_addr;
    instance->flip = 0;

    if (sc301iot->rst_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc301iot->rst_gpio.gpio_group, sc301iot->rst_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc301iot->rst_gpio.gpio_group, sc301iot->rst_gpio.gpio_pin, !sc301iot->rst_active);
    }
    if (sc301iot->pwdn_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc301iot->pwdn_gpio.gpio_group, sc301iot->pwdn_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc301iot->pwdn_gpio.gpio_group, sc301iot->pwdn_gpio.gpio_pin, !sc301iot->pwdn_active);
    }

    if (sc301iot->mclk_out_gate_id && sc301iot->mclk_id)
    {
        clk_set_rate(sc301iot->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(sc301iot->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "SC301IOT_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, sc301iot->isp_subdev_name, rt_strlen(sc301iot->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void sc301iot_detect(void)
{
    struct sc301iot_dev *instance = &g_sc301iot;

    sc301iot_dbg("start to detect SC301IOT for testing \n");
    sc301iot_dbg("dev name:%s\n", instance->name);
    sc301iot_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        sc301iot_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        sc301iot_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc301iot_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc301iot_detect, check SC301IOT is available or not);
#endif
#endif
#endif
