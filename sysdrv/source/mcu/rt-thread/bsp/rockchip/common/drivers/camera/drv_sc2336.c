/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc2336.c
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

#ifdef RT_USING_SC2336

//#define REG_LIST_CONVERSION_DEBUG
#define SC2336_DEBUG_PRT      0

#if SC2336_DEBUG_PRT
#include <stdio.h>
#define sc2336_dbg(fmt, args...)               rt_kprintf("[SC2336] "fmt"", ##args)
#else
#define sc2336_dbg(fmt, args...)
#endif

#define sc2336_info(fmt, args...)         rt_kprintf("[SC2336] "fmt"", ##args)
#define sc2336_err(fmt, args...)         rt_kprintf("[SC2336] ERR "fmt"", ##args)

#define SC2336_REG_EXP_LONG_H           0x3e00
#define SC2336_REG_EXP_LONG_M           0x3e01
#define SC2336_REG_EXP_LONG_L           0x3e02
#define SC2336_REG_EXP_SHORT_H          0x3e22
#define SC2336_REG_EXP_SHORT_M          0x3e04
#define SC2336_REG_EXP_SHORT_L          0x3e05
#define SC2336_REG_DIG_GAIN            0x3e06
#define SC2336_REG_DIG_FINE_GAIN       0x3e07
#define SC2336_REG_ANA_GAIN            0x3e09
#define SC2336_REG_SDIG_GAIN           0x3e10
#define SC2336_REG_SDIG_FINE_GAIN      0x3e11
#define SC2336_REG_SANA_GAIN           0x3e12
#define SC2336_REG_SANA_FINE_GAIN      0x3e13
#define SC2336_EXP_MIN                 1
#define SC2336_GAIN_MIN                0x0020
#define SC2336_GAIN_MAX                (4096)  //32*4*32
#define SC2336_GAIN_STEP               1
#define SC2336_GAIN_DEFAULT            0x0020
#define SC2336_LGAIN                   0
#define SC2336_SGAIN                   1
#define SC2336_FLIP_MIRROR_REG         0x3221
#define SC2336_REG_VTS_H               0x320e
#define SC2336_REG_VTS_L               0x320f
#define SC2336_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC2336_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC2336_REG_STREAM_CTRL         0x0100
#define SC2336_STREAM_ON               0x01
#define SC2336_STREAM_OFF              0x00

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

struct sc2336_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc2336_mode
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
    const struct sc2336_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc2336_dev
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
    const struct sc2336_mode *cur_mode;
    struct sc2336_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc2336_dev *rt_sc2336_dev_t;

static struct sc2336_dev g_sc2336;

#ifndef REG_LIST_CONVERSION_DEBUG
static const uint8_t g_sc2336_960x540_60fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x5d,
    0x3, 0x31, 0x06, 0x05,
    0xa, 0x32, 0x08, 0x03, 0xc0, 0x02, 0x1c, 0x08, 0xca, 0x02, 0x58,
    0x3, 0x32, 0x11, 0x02,
    0x3, 0x32, 0x13, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x17,
    0x4, 0x32, 0x48, 0x04, 0x0b,
    0x3, 0x32, 0x53, 0x08,
    0x5, 0x33, 0x01, 0x09, 0xff, 0x10,
    0x4, 0x33, 0x06, 0x60, 0x02,
    0x6, 0x33, 0x0a, 0x01, 0x10, 0x16, 0xff,
    0x3, 0x33, 0x18, 0x02,
    0x3, 0x33, 0x21, 0x0a,
    0x3, 0x33, 0x27, 0x0e,
    0x3, 0x33, 0x2b, 0x12,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x1f,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0x8, 0x33, 0x90, 0x09, 0x0f, 0x1f, 0x20, 0x20, 0xff,
    0x3, 0x33, 0xa2, 0x04,
    0x5, 0x33, 0xb1, 0x80, 0x68, 0x42,
    0x3, 0x33, 0xf9, 0x70,
    0x5, 0x33, 0xfb, 0xd0, 0x0f, 0x1f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x0f, 0x1f, 0x42, 0x06, 0x01, 0x23, 0x01, 0x84,
    0x3, 0x36, 0x30, 0xf4,
    0x3, 0x36, 0x33, 0x22,
    0x3, 0x36, 0x39, 0xf4,
    0x3, 0x36, 0x3c, 0x47,
    0x3, 0x36, 0x70, 0x09,
    0x5, 0x36, 0x74, 0xf4, 0xfb, 0xed,
    0x4, 0x36, 0x7c, 0x09, 0x0f,
    0x5, 0x36, 0x90, 0x33, 0x33, 0x43,
    0x8, 0x36, 0x98, 0x89, 0x96, 0xd0, 0xd0, 0x09, 0x0f,
    0x5, 0x36, 0xa2, 0x09, 0x0f, 0x1f,
    0x3, 0x36, 0xd0, 0x01,
    0x6, 0x36, 0xea, 0x09, 0x0c, 0x2c, 0x28,
    0x3, 0x37, 0x22, 0xe1,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x3, 0x37, 0x28, 0x20,
    0x6, 0x37, 0xfa, 0x09, 0x32, 0x11, 0x37,
    0x3, 0x39, 0x00, 0x0d,
    0x3, 0x39, 0x05, 0x98,
    0x4, 0x39, 0x1b, 0x81, 0x10,
    0x4, 0x39, 0x33, 0x81, 0xc5,
    0x6, 0x39, 0x40, 0x68, 0x00, 0x01, 0xc6,
    0x4, 0x39, 0x52, 0x02, 0x0f,
    0x4, 0x3e, 0x01, 0x25, 0x20,
    0x3, 0x3e, 0x08, 0x1f,
    0x3, 0x3e, 0x1b, 0x14,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x38,
    0x3, 0x48, 0x19, 0x03,
    0x3, 0x48, 0x1b, 0x02,
    0x3, 0x48, 0x1d, 0x06,
    0x3, 0x48, 0x1f, 0x02,
    0x3, 0x48, 0x21, 0x07,
    0x3, 0x48, 0x23, 0x02,
    0x3, 0x48, 0x25, 0x02,
    0x3, 0x48, 0x27, 0x02,
    0x3, 0x48, 0x29, 0x03,
    0x3, 0x50, 0x00, 0x46,
    0x3, 0x50, 0x11, 0x40,
    0x3, 0x57, 0x99, 0x06,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x3, 0x59, 0x88, 0x70,
    0x12, 0x5a, 0xe0, 0xfe, 0x40, 0x30, 0x28, 0x20, 0x30, 0x28, 0x20, 0x3c, 0x30, 0x28, 0x3c, 0x30, 0x28, 0xfe, 0x40,
    0xe, 0x5a, 0xf4, 0x30, 0x28, 0x20, 0x30, 0x28, 0x20, 0x3c, 0x30, 0x28, 0x3c, 0x30, 0x28,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x4c,
};
static const uint8_t g_sc2336_1920x1080_30fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x02,
    0x3, 0x31, 0x06, 0x05,
    0x6, 0x32, 0x0c, 0x08, 0xca, 0x04, 0xb0,
    0x4, 0x32, 0x48, 0x04, 0x0b,
    0x3, 0x32, 0x53, 0x08,
    0x5, 0x33, 0x01, 0x09, 0xff, 0x10,
    0x4, 0x33, 0x06, 0x60, 0x02,
    0x6, 0x33, 0x0a, 0x01, 0x10, 0x16, 0xff,
    0x3, 0x33, 0x18, 0x02,
    0x3, 0x33, 0x21, 0x0a,
    0x3, 0x33, 0x27, 0x0e,
    0x3, 0x33, 0x2b, 0x12,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x1f,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0x8, 0x33, 0x90, 0x09, 0x0f, 0x1f, 0x20, 0x20, 0xff,
    0x3, 0x33, 0xa2, 0x04,
    0x5, 0x33, 0xb1, 0x80, 0x68, 0x42,
    0x3, 0x33, 0xf9, 0x70,
    0x5, 0x33, 0xfb, 0xd0, 0x0f, 0x1f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x0f, 0x1f, 0x42, 0x06, 0x01, 0x23, 0x01, 0x84,
    0x3, 0x36, 0x30, 0xf4,
    0x3, 0x36, 0x33, 0x22,
    0x3, 0x36, 0x39, 0xf4,
    0x3, 0x36, 0x3c, 0x47,
    0x3, 0x36, 0x70, 0x09,
    0x5, 0x36, 0x74, 0xf4, 0xfb, 0xed,
    0x4, 0x36, 0x7c, 0x09, 0x0f,
    0x5, 0x36, 0x90, 0x33, 0x33, 0x43,
    0x8, 0x36, 0x98, 0x89, 0x96, 0xd0, 0xd0, 0x09, 0x0f,
    0x5, 0x36, 0xa2, 0x09, 0x0f, 0x1f,
    0x3, 0x36, 0xd0, 0x01,
    0x6, 0x36, 0xea, 0x09, 0x0c, 0x1c, 0x28,
    0x3, 0x37, 0x22, 0xe1,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x3, 0x37, 0x28, 0x20,
    0x6, 0x37, 0xfa, 0x09, 0x32, 0x11, 0x37,
    0x3, 0x39, 0x00, 0x0d,
    0x3, 0x39, 0x05, 0x98,
    0x4, 0x39, 0x1b, 0x81, 0x10,
    0x4, 0x39, 0x33, 0x81, 0xc5,
    0x6, 0x39, 0x40, 0x68, 0x00, 0x01, 0xc6,
    0x4, 0x39, 0x52, 0x02, 0x0f,
    0x4, 0x3e, 0x01, 0x4a, 0xa0,
    0x3, 0x3e, 0x08, 0x1f,
    0x3, 0x3e, 0x1b, 0x14,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x38,
    0x3, 0x48, 0x19, 0x06,
    0x3, 0x48, 0x1b, 0x03,
    0x3, 0x48, 0x1d, 0x0b,
    0x3, 0x48, 0x1f, 0x03,
    0x3, 0x48, 0x21, 0x08,
    0x3, 0x48, 0x23, 0x03,
    0x3, 0x48, 0x25, 0x03,
    0x3, 0x48, 0x27, 0x03,
    0x3, 0x48, 0x29, 0x05,
    0x3, 0x57, 0x99, 0x06,
    0x12, 0x5a, 0xe0, 0xfe, 0x40, 0x30, 0x28, 0x20, 0x30, 0x28, 0x20, 0x3c, 0x30, 0x28, 0x3c, 0x30, 0x28, 0xfe, 0x40,
    0xe, 0x5a, 0xf4, 0x30, 0x28, 0x20, 0x30, 0x28, 0x20, 0x3c, 0x30, 0x28, 0x3c, 0x30, 0x28,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x44,
};
#else
/*
 * Xclk 24Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 205Mbps, 2lane
 * binning to 960x540
 */
static const struct sc2336_sensor_reg g_sc2336_960x540_60fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x5d},
    {0x3106, 0x05},
    {0x3208, 0x03},
    {0x3209, 0xc0},
    {0x320a, 0x02},
    {0x320b, 0x1c},
    {0x320c, 0x08},
    {0x320d, 0xca},
    {0x320e, 0x02},
    {0x320f, 0x58},
    {0x3211, 0x02},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x17},
    {0x3248, 0x04},
    {0x3249, 0x0b},
    {0x3253, 0x08},
    {0x3301, 0x09},
    {0x3302, 0xff},
    {0x3303, 0x10},
    {0x3306, 0x60},
    {0x3307, 0x02},
    {0x330a, 0x01},
    {0x330b, 0x10},
    {0x330c, 0x16},
    {0x330d, 0xff},
    {0x3318, 0x02},
    {0x3321, 0x0a},
    {0x3327, 0x0e},
    {0x332b, 0x12},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335e, 0x06},
    {0x335f, 0x0a},
    {0x3364, 0x1f},
    {0x337c, 0x02},
    {0x337d, 0x0e},
    {0x3390, 0x09},
    {0x3391, 0x0f},
    {0x3392, 0x1f},
    {0x3393, 0x20},
    {0x3394, 0x20},
    {0x3395, 0xff},
    {0x33a2, 0x04},
    {0x33b1, 0x80},
    {0x33b2, 0x68},
    {0x33b3, 0x42},
    {0x33f9, 0x70},
    {0x33fb, 0xd0},
    {0x33fc, 0x0f},
    {0x33fd, 0x1f},
    {0x349f, 0x03},
    {0x34a6, 0x0f},
    {0x34a7, 0x1f},
    {0x34a8, 0x42},
    {0x34a9, 0x06},
    {0x34aa, 0x01},
    {0x34ab, 0x23},
    {0x34ac, 0x01},
    {0x34ad, 0x84},
    {0x3630, 0xf4},
    {0x3633, 0x22},
    {0x3639, 0xf4},
    {0x363c, 0x47},
    {0x3670, 0x09},
    {0x3674, 0xf4},
    {0x3675, 0xfb},
    {0x3676, 0xed},
    {0x367c, 0x09},
    {0x367d, 0x0f},
    {0x3690, 0x33},
    {0x3691, 0x33},
    {0x3692, 0x43},
    {0x3698, 0x89},
    {0x3699, 0x96},
    {0x369a, 0xd0},
    {0x369b, 0xd0},
    {0x369c, 0x09},
    {0x369d, 0x0f},
    {0x36a2, 0x09},
    {0x36a3, 0x0f},
    {0x36a4, 0x1f},
    {0x36d0, 0x01},
    {0x36ea, 0x09},
    {0x36eb, 0x0c},
    {0x36ec, 0x2c},
    {0x36ed, 0x28},
    {0x3722, 0xe1},
    {0x3724, 0x41},
    {0x3725, 0xc1},
    {0x3728, 0x20},
    {0x37fa, 0x09},
    {0x37fb, 0x32},
    {0x37fc, 0x11},
    {0x37fd, 0x37},
    {0x3900, 0x0d},
    {0x3905, 0x98},
    {0x391b, 0x81},
    {0x391c, 0x10},
    {0x3933, 0x81},
    {0x3934, 0xc5},
    {0x3940, 0x68},
    {0x3941, 0x00},
    {0x3942, 0x01},
    {0x3943, 0xc6},
    {0x3952, 0x02},
    {0x3953, 0x0f},
    {0x3e01, 0x25},
    {0x3e02, 0x20},
    {0x3e08, 0x1f},
    {0x3e1b, 0x14},
    {0x440e, 0x02},
    {0x4509, 0x38},
    {0x4819, 0x03},
    {0x481b, 0x02},
    {0x481d, 0x06},
    {0x481f, 0x02},
    {0x4821, 0x07},
    {0x4823, 0x02},
    {0x4825, 0x02},
    {0x4827, 0x02},
    {0x4829, 0x03},
    {0x5000, 0x46},
    {0x5011, 0x40},
    {0x5799, 0x06},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x5988, 0x70},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x30},
    {0x5ae3, 0x28},
    {0x5ae4, 0x20},
    {0x5ae5, 0x30},
    {0x5ae6, 0x28},
    {0x5ae7, 0x20},
    {0x5ae8, 0x3c},
    {0x5ae9, 0x30},
    {0x5aea, 0x28},
    {0x5aeb, 0x3c},
    {0x5aec, 0x30},
    {0x5aed, 0x28},
    {0x5aee, 0xfe},
    {0x5aef, 0x40},
    {0x5af4, 0x30},
    {0x5af5, 0x28},
    {0x5af6, 0x20},
    {0x5af7, 0x30},
    {0x5af8, 0x28},
    {0x5af9, 0x20},
    {0x5afa, 0x3c},
    {0x5afb, 0x30},
    {0x5afc, 0x28},
    {0x5afd, 0x3c},
    {0x5afe, 0x30},
    {0x5aff, 0x28},
    {0x36e9, 0x53},
    {0x37f9, 0x53},
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 405Mbps, 2lane
 * full resolution
 */
static const struct sc2336_sensor_reg g_sc2336_1920x1080_30fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x02},
    {0x3106, 0x05},
    {0x320c, 0x08},
    {0x320d, 0xca},
    {0x320e, 0x04},
    {0x320f, 0xb0},
    {0x3248, 0x04},
    {0x3249, 0x0b},
    {0x3253, 0x08},
    {0x3301, 0x09},
    {0x3302, 0xff},
    {0x3303, 0x10},
    {0x3306, 0x60},
    {0x3307, 0x02},
    {0x330a, 0x01},
    {0x330b, 0x10},
    {0x330c, 0x16},
    {0x330d, 0xff},
    {0x3318, 0x02},
    {0x3321, 0x0a},
    {0x3327, 0x0e},
    {0x332b, 0x12},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335e, 0x06},
    {0x335f, 0x0a},
    {0x3364, 0x1f},
    {0x337c, 0x02},
    {0x337d, 0x0e},
    {0x3390, 0x09},
    {0x3391, 0x0f},
    {0x3392, 0x1f},
    {0x3393, 0x20},
    {0x3394, 0x20},
    {0x3395, 0xff},
    {0x33a2, 0x04},
    {0x33b1, 0x80},
    {0x33b2, 0x68},
    {0x33b3, 0x42},
    {0x33f9, 0x70},
    {0x33fb, 0xd0},
    {0x33fc, 0x0f},
    {0x33fd, 0x1f},
    {0x349f, 0x03},
    {0x34a6, 0x0f},
    {0x34a7, 0x1f},
    {0x34a8, 0x42},
    {0x34a9, 0x06},
    {0x34aa, 0x01},
    {0x34ab, 0x23},
    {0x34ac, 0x01},
    {0x34ad, 0x84},
    {0x3630, 0xf4},
    {0x3633, 0x22},
    {0x3639, 0xf4},
    {0x363c, 0x47},
    {0x3670, 0x09},
    {0x3674, 0xf4},
    {0x3675, 0xfb},
    {0x3676, 0xed},
    {0x367c, 0x09},
    {0x367d, 0x0f},
    {0x3690, 0x33},
    {0x3691, 0x33},
    {0x3692, 0x43},
    {0x3698, 0x89},
    {0x3699, 0x96},
    {0x369a, 0xd0},
    {0x369b, 0xd0},
    {0x369c, 0x09},
    {0x369d, 0x0f},
    {0x36a2, 0x09},
    {0x36a3, 0x0f},
    {0x36a4, 0x1f},
    {0x36d0, 0x01},
    {0x36ea, 0x09},
    {0x36eb, 0x0c},
    {0x36ec, 0x1c},
    {0x36ed, 0x28},
    {0x3722, 0xe1},
    {0x3724, 0x41},
    {0x3725, 0xc1},
    {0x3728, 0x20},
    {0x37fa, 0x09},
    {0x37fb, 0x32},
    {0x37fc, 0x11},
    {0x37fd, 0x37},
    {0x3900, 0x0d},
    {0x3905, 0x98},
    {0x391b, 0x81},
    {0x391c, 0x10},
    {0x3933, 0x81},
    {0x3934, 0xc5},
    {0x3940, 0x68},
    {0x3941, 0x00},
    {0x3942, 0x01},
    {0x3943, 0xc6},
    {0x3952, 0x02},
    {0x3953, 0x0f},
    {0x3e01, 0x4a},
    {0x3e02, 0xa0},
    {0x3e08, 0x1f},
    {0x3e1b, 0x14},
    {0x440e, 0x02},
    {0x4509, 0x38},
    {0x4819, 0x06},
    {0x481b, 0x03},
    {0x481d, 0x0b},
    {0x481f, 0x03},
    {0x4821, 0x08},
    {0x4823, 0x03},
    {0x4825, 0x03},
    {0x4827, 0x03},
    {0x4829, 0x05},
    {0x5799, 0x06},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x30},
    {0x5ae3, 0x28},
    {0x5ae4, 0x20},
    {0x5ae5, 0x30},
    {0x5ae6, 0x28},
    {0x5ae7, 0x20},
    {0x5ae8, 0x3c},
    {0x5ae9, 0x30},
    {0x5aea, 0x28},
    {0x5aeb, 0x3c},
    {0x5aec, 0x30},
    {0x5aed, 0x28},
    {0x5aee, 0xfe},
    {0x5aef, 0x40},
    {0x5af4, 0x30},
    {0x5af5, 0x28},
    {0x5af6, 0x20},
    {0x5af7, 0x30},
    {0x5af8, 0x28},
    {0x5af9, 0x20},
    {0x5afa, 0x3c},
    {0x5afb, 0x30},
    {0x5afc, 0x28},
    {0x5afd, 0x3c},
    {0x5afe, 0x30},
    {0x5aff, 0x28},
    {0x36e9, 0x53},
    {0x37f9, 0x53},
    {REG_END, 0x00},
};
#endif

static const struct sc2336_mode supported_modes[] =
{
    [SC2336_960X540] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 960,
        .height = 540,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x252,
        .hts_def = 0x08ca,
        .vts_def = 0x0258,
        .xvclk = 24000000,
        .link_freq = 101250000,
        .reg_list = g_sc2336_960x540_60fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc2336_960x540_60fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [SC2336_1920X1080] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x04aa,
        .hts_def = 0x08ca,
        .vts_def = 0x04b0,
        .xvclk = 24000000,
        .link_freq = 202500000,
        .reg_list = g_sc2336_1920x1080_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc2336_1920x1080_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct sc2336_ops
{
    ret_err_t (*init)(struct sc2336_dev *dev);
    ret_err_t (*open)(struct sc2336_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc2336_dev *dev);
    ret_err_t (*control)(struct sc2336_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc2336_read_reg(struct rt_i2c_bus_device *bus,
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
    msg[0].addr = g_sc2336.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_sc2336.i2c_addr;
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
            sc2336_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc2336_write_reg(struct rt_i2c_bus_device *bus,
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

    msg.addr = g_sc2336.i2c_addr;
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
            sc2336_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int sc2336_write_multiple_reg_continue(struct sc2336_dev *dev,
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
        msg.addr = g_sc2336.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            sc2336_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t sc2336_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msgs.addr = g_sc2336.i2c_addr;
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
            sc2336_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc2336_write_multiple_reg(struct sc2336_dev *dev,
                                      const struct sc2336_sensor_reg *reg, int len)
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

                sc2336_write_reg_discontinued(i2c_bus, data, k + 2);
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

                sc2336_write_reg_discontinued(i2c_bus, data, k + 2);
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

                    sc2336_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc2336_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc2336_info("id: %d, width: %d, height: %d\n",
                    id, supported_modes[id].width, supported_modes[id].height);
        sc2336_write_multiple_reg((struct sc2336_dev *)dev,
                                  supported_modes[id].reg_list,
                                  supported_modes[id].reg_list_size);
    }
}
#endif

static int sc2336_get_gain_reg(uint8_t *again, uint8_t *dgain,
                               uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    gain_factor = total_gain * 1000 / 32;
    if (gain_factor < 1000) /* 1 gain*/
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = 0x80;
    }
    if (gain_factor < 1000 * 2)          /* 1 ~ 2 gain*/
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000;
    }
    else if (gain_factor < 1000 * 4)     /* 2 ~ 4 gain*/
    {
        *again = 0x08;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000 / 2;
    }
    else if (gain_factor < 1000 * 8)         /* 4 ~ 8 gain*/
    {
        *again = 0x09;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000 / 4;
    }
    else if (gain_factor < 1000 * 16)         /* 8 ~ 16 gain*/
    {
        *again = 0x0b;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000 / 8;
    }
    else if (gain_factor < 1000 * 32)         /* 16 ~ 32 gain*/
    {
        *again = 0x0f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000 / 16;
    }
    else if (gain_factor < 1000 * 64)        /* 32 ~ 64 gain*/
    {
        *again = 0x1f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000 / 32;
    }
    else if (gain_factor < 1000 * 128)        /* 64 ~ 128 gain*/
    {
        //open dgain begin  max digital gain 4X
        *again = 0x1f;
        *dgain = 0x01;
        *dgain_fine = gain_factor * 128 / 1000 / 64;
    }
    else           /*max 128 gain*/
    {
        *again = 0x1f;
        *dgain = 0x03;
        *dgain_fine = 0x80;
    }

    *dgain_fine = *dgain_fine / 4 * 4;

    return ret;
}
static ret_err_t rk_sc2336_set_expval(struct sc2336_dev *dev, struct rk_camera_exp_val *exp)
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
        sc2336_dbg("sc2336 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC2336_EXP_MIN)
        l_exp_time = SC2336_EXP_MIN;

    sc2336_dbg("total_time reg: 0x%x, total_gain reg 0x%x\n",
               l_exp_time, l_a_gain);

    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_EXP_LONG_H,
                           (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_EXP_LONG_M,
                           (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_EXP_LONG_L,
                           (uint8_t)((l_exp_time & 0xf) << 4));
    sc2336_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    sc2336_dbg("a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", a_gain, d_gain, d_gain_fine);

    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_ANA_GAIN, a_gain);
    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_DIG_GAIN, d_gain);
    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc2336_set_vts(struct sc2336_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc2336_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_VTS_L,
                           (uint8_t)(dst_vts & 0xff));
    ret = sc2336_write_reg(dev->i2c_bus, SC2336_REG_VTS_H,
                           (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc2336_set_flip_mirror(struct sc2336_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc2336_read_reg(dev->i2c_bus, SC2336_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC2336_FETCH_MIRROR(val, flip & 0x01);
        val = SC2336_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC2336_FETCH_MIRROR(val, false);
        val = SC2336_FETCH_FLIP(val, false);
        break;
    };
    sc2336_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc2336_write_reg(dev->i2c_bus, SC2336_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc2336_stream_on(struct sc2336_dev *dev)
{
    sc2336_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(5);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    sc2336_write_multiple_reg_continue((struct sc2336_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc2336_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc2336_set_flip_mirror(dev, dev->flip);

    sc2336_write_reg(dev->i2c_bus, SC2336_REG_STREAM_CTRL, SC2336_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc2336_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc2336_stream_on_late(struct sc2336_dev *dev)
{

    sc2336_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc2336_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc2336_set_flip_mirror(dev, dev->flip);

    sc2336_write_reg(dev->i2c_bus, SC2336_REG_STREAM_CTRL, SC2336_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc2336_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc2336_stream_off(struct sc2336_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc2336_write_reg(dev->i2c_bus, SC2336_REG_STREAM_CTRL, SC2336_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc2336_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_sc2336_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc2336_dev *sc2336;

    sc2336 = (struct sc2336_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc2336->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc2336);
#endif

    if (sc2336)
    {
        camera->info.mbus_fmt.width = sc2336->cur_mode->width;
        camera->info.mbus_fmt.height = sc2336->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc2336->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc2336->cur_mode->hdr_mode;

        sc2336->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc2336->i2c_name);

        if (!sc2336->i2c_bus)
        {
            sc2336_err("not find i2c :%s !!!\n",
                       sc2336->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc2336_dbg("find i2c_bus:%s\n", sc2336->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc2336_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc2336_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_sc2336_get_expinf(struct sc2336_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc2336_mode *mode;

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

static ret_err_t rk_sc2336_get_intput_fmt(struct sc2336_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc2336_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc2336_set_intput_fmt(struct sc2336_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc2336_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc2336_info("dst resulotion, width %d, height %d\n",
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
            sc2336_write_multiple_reg_continue((struct sc2336_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc2336_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc2336_match_dst_config(struct sc2336_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc2336_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc2336_info("cur resulotion, width %d, height %d, fps %d!\n",
                dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc2336_mode *)mode;
            ret = RET_SYS_EOK;
            sc2336_info("find dst resulotion, width %d, height %d, fps %d!\n",
                        dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc2336_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc2336_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc2336_control(struct rk_camera_device *dev,
                            dt_cmd_t cmd,
                            void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc2336_dev *sc2336;

    RT_ASSERT(dev != RT_NULL);
    sc2336 = (struct sc2336_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc2336_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc2336_stream_on(sc2336);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc2336_stream_off(sc2336);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc2336_get_expinf(sc2336, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc2336_set_expval(sc2336, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc2336_set_vts(sc2336, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc2336_get_intput_fmt(sc2336, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc2336_set_intput_fmt(sc2336, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc2336_match_dst_config(sc2336, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc2336_set_flip_mirror(sc2336, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc2336_stream_on_late(sc2336);
    }
    break;
    default:
        sc2336_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc2336_ops =
{
    rk_sc2336_init,
    rk_sc2336_open,
    NULL,
    rk_sc2336_control
};

int rk_camera_sc2336_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc2336_dev *instance = &g_sc2336;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *sc2336 = (struct camera_board_desc *)&camera_sc2336;

    camera->ops = &rk_sc2336_ops;

    rt_strncpy(instance->name, sc2336->isp_subdev_name, rt_strlen(sc2336->isp_subdev_name));
    rt_strncpy(instance->i2c_name, sc2336->i2c_bus, rt_strlen(sc2336->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (sc2336->mode_id <= SC2336_MODE_ID_MAX)
    {
        sc2336_dbg("mode_id: %d\n", sc2336->mode_id);
        instance->cur_mode = &supported_modes[sc2336->mode_id];
    }
    else
    {
        sc2336_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct sc2336_mode *)&supported_modes[SC2336_1920X1080];
    instance->time_valid_delay = sc2336->time_valid_delay;
    instance->gain_valid_delay = sc2336->gain_valid_delay;
    instance->i2c_addr = sc2336->i2c_addr;
    instance->flip = 0;

    HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc2336->rst_gpio.gpio_group, sc2336->rst_gpio.gpio_pin, GPIO_OUT);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc2336->rst_gpio.gpio_group, sc2336->rst_gpio.gpio_pin, !sc2336->rst_active);

    if (sc2336->mclk_out_gate_id && sc2336->mclk_id)
    {
        clk_set_rate(sc2336->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(sc2336->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "SC2336_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, sc2336->isp_subdev_name, rt_strlen(sc2336->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void sc2336_detect(void)
{
    struct sc2336_dev *instance = &g_sc2336;

    sc2336_dbg("start to detect SC2336 for testing \n");
    sc2336_dbg("dev name:%s\n", instance->name);
    sc2336_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        sc2336_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        sc2336_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc2336_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc2336_detect, check SC2336 is available or not);
#endif
#endif
#endif
