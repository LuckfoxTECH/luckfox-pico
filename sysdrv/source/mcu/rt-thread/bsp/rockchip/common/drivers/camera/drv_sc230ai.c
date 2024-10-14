/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc230ai.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2022-07-07     ISP Team          first implementation
  * 2022-12-08     chujin.zhou     sc230ai support get config from boardcfg
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

#ifdef RT_USING_SC230AI

//#define REG_LIST_CONVERSION_DEBUG
#define SC230AI_DEBUG_PRT      0

#if SC230AI_DEBUG_PRT
#include <stdio.h>
#define sc230ai_dbg(fmt, args...)               rt_kprintf("[SC230AI] "fmt"", ##args)
#else
#define sc230ai_dbg(fmt, args...)
#endif

#define sc230ai_info(fmt, args...)         rt_kprintf("[SC230AI] "fmt"", ##args)
#define sc230ai_err(fmt, args...)         rt_kprintf("[SC230AI] ERR "fmt"", ##args)

#define SC230AI_REG_EXP_LONG_H           0x3e00
#define SC230AI_REG_EXP_LONG_M           0x3e01
#define SC230AI_REG_EXP_LONG_L           0x3e02
#define SC230AI_REG_EXP_SHORT_H          0x3e22
#define SC230AI_REG_EXP_SHORT_M          0x3e04
#define SC230AI_REG_EXP_SHORT_L          0x3e05
#define SC230AI_REG_DIG_GAIN            0x3e06
#define SC230AI_REG_DIG_FINE_GAIN       0x3e07
#define SC230AI_REG_ANA_GAIN            0x3e09
#define SC230AI_REG_SDIG_GAIN           0x3e10
#define SC230AI_REG_SDIG_FINE_GAIN      0x3e11
#define SC230AI_REG_SANA_GAIN           0x3e12
#define SC230AI_REG_SANA_FINE_GAIN      0x3e13
#define SC230AI_GAIN_MIN                1000
#define SC230AI_GAIN_MAX                1574800       // 99.200*15.875*1000
#define SC230AI_GAIN_STEP               1
#define SC230AI_GAIN_DEFAULT            1000
#define SC230AI_LGAIN                   0
#define SC230AI_SGAIN                   1
#define SC230AI_FLIP_MIRROR_REG         0x3221
#define SC230AI_REG_VTS_H               0x320e
#define SC230AI_REG_VTS_L               0x320f
#define SC230AI_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC230AI_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC230AI_REG_STREAM_CTRL         0x0100
#define SC230AI_STREAM_ON               0x01
#define SC230AI_STREAM_OFF              0x00

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

struct sc230ai_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc230ai_mode
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
    const struct sc230ai_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc230ai_dev
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
    const struct sc230ai_mode *cur_mode;
    struct sc230ai_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc230ai_dev *rt_sc230ai_dev_t;

#ifndef REG_LIST_CONVERSION_DEBUG
static const uint8_t g_sc230ai_init_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x2d,
    0x3, 0x32, 0x03, 0x3c,
    0x7, 0x32, 0x07, 0x03, 0x02, 0x80, 0x01, 0xe0,
    0x8, 0x32, 0x0e, 0x02, 0x32, 0x00, 0xa2, 0x00, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x33, 0x01, 0x09,
    0x3, 0x33, 0x06, 0x48,
    0x6, 0x33, 0x08, 0x18, 0x68, 0x00, 0xc0,
    0x3, 0x33, 0x1f, 0x59,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x5, 0x33, 0x5d, 0x60, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x01, 0x0b, 0x0f, 0x0c, 0x0d, 0x60, 0x48, 0x49, 0x4f, 0x0a, 0x0f, 0x14, 0x60,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xaf, 0x40,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb9, 0x0a,
    0x3, 0x33, 0xf9, 0x70,
    0x5, 0x33, 0xfb, 0x90, 0x4b, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0x4, 0x34, 0xa6, 0x4b, 0x4f,
    0x5, 0x34, 0xab, 0xe0, 0x01, 0x00,
    0x4, 0x34, 0xf8, 0x5f, 0x10,
    0x3, 0x36, 0x30, 0xc0,
    0x3, 0x36, 0x37, 0x29,
    0x3, 0x36, 0x3b, 0x20,
    0x3, 0x36, 0x70, 0x09,
    0x5, 0x36, 0x74, 0xb0, 0x80, 0x88,
    0x4, 0x36, 0x7c, 0x40, 0x49,
    0x5, 0x36, 0x90, 0x44, 0x44, 0x54,
    0x4, 0x36, 0x9c, 0x49, 0x4f,
    0x7, 0x36, 0xae, 0x4b, 0x4f, 0x87, 0x9b, 0xb7,
    0x3, 0x36, 0xd0, 0x01,
    0x3, 0x36, 0xeb, 0x04,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x17,
    0x3, 0x37, 0x28, 0x90,
    0x7, 0x37, 0xb0, 0x17, 0x17, 0x97, 0x4b, 0x4f,
    0x5, 0x37, 0xfb, 0x24, 0x10, 0x22,
    0x3, 0x39, 0x01, 0x02,
    0x3, 0x39, 0x04, 0x04,
    0x3, 0x39, 0x1f, 0x04,
    0x4, 0x39, 0x33, 0x84, 0x02,
    0x6, 0x39, 0x40, 0x62, 0x00, 0x04, 0x03,
    0x4, 0x3e, 0x01, 0x45, 0xb0,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x0d, 0x11,
    0x3, 0x48, 0x19, 0x05,
    0x3, 0x48, 0x1b, 0x03,
    0x3, 0x48, 0x1d, 0x0a,
    0x3, 0x48, 0x1f, 0x02,
    0x3, 0x48, 0x21, 0x08,
    0x3, 0x48, 0x23, 0x03,
    0x3, 0x48, 0x25, 0x02,
    0x3, 0x48, 0x29, 0x04,
    0x3, 0x50, 0x00, 0x46,
    0x3, 0x50, 0x10, 0x01,
    0x4, 0x57, 0x8a, 0x10, 0x08,
    0x7, 0x57, 0x91, 0x04, 0x00, 0x10, 0x08, 0x00,
    0x3, 0x57, 0x99, 0x06,
    0x3, 0x57, 0xad, 0x00,
    0x4, 0x59, 0x00, 0xf1, 0x04,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0xe, 0x5a, 0xf4, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x37, 0xf9, 0x24,
    0x47,

};

#ifdef SC230AI_MASTER_SLAVER
static const uint8_t g_sc230ai_1080p_reg_table_master[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x0a, 0x24,
    0x3, 0x30, 0x1f, 0x56,
    0x3, 0x30, 0x32, 0xa0,
    0x3, 0x32, 0x30, 0x02, //set low level to 20ms
    0x4, 0x32, 0x0e, 0x05, 0x46,
    0x3, 0x33, 0x01, 0x07,
    0x3, 0x33, 0x04, 0x50,
    0x3, 0x33, 0x06, 0x70,
    0x6, 0x33, 0x08, 0x18, 0x68, 0x01, 0x20,
    0x3, 0x33, 0x14, 0x15,
    0x4, 0x33, 0x1e, 0x41, 0x59,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x5, 0x33, 0x5d, 0x60, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x01, 0x0b, 0x0f, 0x09, 0x0d, 0x60, 0x48, 0x49, 0x4b, 0x06, 0x0a, 0x0d, 0x60,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xad, 0x2c,
    0x3, 0x33, 0xaf, 0x40,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x40,
    0x3, 0x33, 0xb9, 0x0a,
    0x3, 0x33, 0xf9, 0x78,
    0x5, 0x33, 0xfb, 0xa0, 0x4f, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x4b, 0x5f, 0x30, 0x20, 0x01, 0x28, 0x01, 0x58,
    0x4, 0x34, 0xf8, 0x7f, 0x10,
    0x3, 0x36, 0x30, 0xc0,
    0x4, 0x36, 0x32, 0x54, 0x44,
    0x4, 0x36, 0x3b, 0x20, 0x08,
    0x3, 0x36, 0x70, 0x09,
    0x5, 0x36, 0x74, 0xb0, 0x80, 0x88,
    0x4, 0x36, 0x7c, 0x40, 0x49,
    0x5, 0x36, 0x90, 0x33, 0x33, 0x43,
    0x4, 0x36, 0x9c, 0x49, 0x4f,
    0x7, 0x36, 0xae, 0x4b, 0x4f, 0x87, 0x9b, 0xb7,
    0x3, 0x36, 0xd0, 0x01,
    0x3, 0x37, 0x22, 0x97,
    0x3, 0x37, 0x24, 0x22,
    0x3, 0x37, 0x28, 0x90,
    0x4, 0x39, 0x01, 0x02, 0xc5,
    0x3, 0x39, 0x04, 0x04,
    0x6, 0x39, 0x07, 0x00, 0x41, 0x00, 0x00,
    0x4, 0x39, 0x33, 0x84, 0x0a,
    0x6, 0x39, 0x40, 0x64, 0x00, 0x04, 0x0b,
    0x5, 0x3e, 0x00, 0x00, 0xa8, 0x30,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x0d, 0x11,
    0x3, 0x50, 0x10, 0x01,
    0x8, 0x57, 0x87, 0x08, 0x03, 0x00, 0x10, 0x08, 0x00,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x00, 0x10, 0x08, 0x00,
    0x3, 0x57, 0x99, 0x06,
    0x3, 0x57, 0xad, 0x00,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0xe, 0x5a, 0xf4, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x37, 0xf9, 0x27,
    0x3d,
};

static const uint8_t g_sc230ai_1080p_reg_table_slaver[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x57,
    0x4, 0x32, 0x0e, 0x05, 0x46,
    0x3, 0x32, 0x22, 0x01,
    0x3, 0x32, 0x24, 0x92, //fsync
    0x3, 0x33, 0x01, 0x07,
    0x3, 0x33, 0x04, 0x50,
    0x3, 0x33, 0x06, 0x70,
    0x6, 0x33, 0x08, 0x18, 0x68, 0x01, 0x20,
    0x3, 0x33, 0x14, 0x15,
    0x4, 0x33, 0x1e, 0x41, 0x59,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x5, 0x33, 0x5d, 0x60, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x01, 0x0b, 0x0f, 0x09, 0x0d, 0x60, 0x48, 0x49, 0x4b, 0x06, 0x0a, 0x0d, 0x60,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xad, 0x2c,
    0x3, 0x33, 0xaf, 0x40,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x40,
    0x3, 0x33, 0xb9, 0x0a,
    0x3, 0x33, 0xf9, 0x78,
    0x5, 0x33, 0xfb, 0xa0, 0x4f, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x4b, 0x5f, 0x30, 0x20, 0x01, 0x28, 0x01, 0x58,
    0x4, 0x34, 0xf8, 0x7f, 0x10,
    0x3, 0x36, 0x14, 0x01,
    0x3, 0x36, 0x30, 0xc0,
    0x4, 0x36, 0x32, 0x54, 0x44,
    0x4, 0x36, 0x3b, 0x20, 0x08,
    0x3, 0x36, 0x70, 0x09,
    0x5, 0x36, 0x74, 0xb0, 0x80, 0x88,
    0x4, 0x36, 0x7c, 0x40, 0x49,
    0x5, 0x36, 0x90, 0x33, 0x33, 0x43,
    0x4, 0x36, 0x9c, 0x49, 0x4f,
    0x7, 0x36, 0xae, 0x4b, 0x4f, 0x87, 0x9b, 0xb7,
    0x3, 0x36, 0xd0, 0x01,
    0x3, 0x37, 0x22, 0x97,
    0x3, 0x37, 0x24, 0x22,
    0x3, 0x37, 0x28, 0x90,
    0x4, 0x39, 0x01, 0x02, 0xc5,
    0x3, 0x39, 0x04, 0x04,
    0x6, 0x39, 0x07, 0x00, 0x41, 0x00, 0x00,
    0x4, 0x39, 0x33, 0x84, 0x0a,
    0x6, 0x39, 0x40, 0x64, 0x00, 0x04, 0x0b,
    0x5, 0x3e, 0x00, 0x00, 0xa8, 0x30,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x0d, 0x11,
    0x3, 0x50, 0x10, 0x01,
    0x8, 0x57, 0x87, 0x08, 0x03, 0x00, 0x10, 0x08, 0x00,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x00, 0x10, 0x08, 0x00,
    0x3, 0x57, 0x99, 0x06,
    0x3, 0x57, 0xad, 0x00,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0xe, 0x5a, 0xf4, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x37, 0xf9, 0x27,
    0x3, 0x01, 0x00, 0x01,//stream on
    0x3e,
};

#else
static const uint8_t g_sc230ai_1080p_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x01,
    0x4, 0x32, 0x0e, 0x05, 0x46,
    0x3, 0x32, 0x21, 0x66,
    0x3, 0x33, 0x01, 0x07,
    0x3, 0x33, 0x04, 0x50,
    0x3, 0x33, 0x06, 0x70,
    0x6, 0x33, 0x08, 0x18, 0x68, 0x01, 0x20,
    0x3, 0x33, 0x14, 0x15,
    0x4, 0x33, 0x1e, 0x41, 0x59,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x5, 0x33, 0x5d, 0x60, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x01, 0x0b, 0x0f, 0x09, 0x0d, 0x60, 0x48, 0x49, 0x4b, 0x06, 0x0a, 0x0d, 0x60,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xad, 0x2c,
    0x3, 0x33, 0xaf, 0x40,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x40,
    0x3, 0x33, 0xb9, 0x0a,
    0x3, 0x33, 0xf9, 0x78,
    0x5, 0x33, 0xfb, 0xa0, 0x4f, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x4b, 0x5f, 0x30, 0x20, 0x01, 0x28, 0x01, 0x58,
    0x4, 0x34, 0xf8, 0x7f, 0x10,
    0x3, 0x36, 0x30, 0xc0,
    0x4, 0x36, 0x32, 0x54, 0x44,
    0x4, 0x36, 0x3b, 0x20, 0x08,
    0x3, 0x36, 0x70, 0x09,
    0x5, 0x36, 0x74, 0xb0, 0x80, 0x88,
    0x4, 0x36, 0x7c, 0x40, 0x49,
    0x5, 0x36, 0x90, 0x33, 0x33, 0x43,
    0x4, 0x36, 0x9c, 0x49, 0x4f,
    0x7, 0x36, 0xae, 0x4b, 0x4f, 0x87, 0x9b, 0xb7,
    0x3, 0x36, 0xd0, 0x01,
    0x3, 0x37, 0x22, 0x97,
    0x3, 0x37, 0x24, 0x22,
    0x3, 0x37, 0x28, 0x90,
    0x4, 0x39, 0x01, 0x02, 0xc5,
    0x3, 0x39, 0x04, 0x04,
    0x6, 0x39, 0x07, 0x00, 0x41, 0x00, 0x00,
    0x4, 0x39, 0x33, 0x84, 0x0a,
    0x6, 0x39, 0x40, 0x64, 0x00, 0x04, 0x0b,
    0x5, 0x3e, 0x00, 0x00, 0x8c, 0x10,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x0d, 0x11,
    0x3, 0x50, 0x10, 0x01,
    0x8, 0x57, 0x87, 0x08, 0x03, 0x00, 0x10, 0x08, 0x00,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x00, 0x10, 0x08, 0x00,
    0x3, 0x57, 0x99, 0x06,
    0x3, 0x57, 0xad, 0x00,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0xe, 0x5a, 0xf4, 0x3f, 0x38, 0x28, 0x3f, 0x38, 0x28, 0x3f, 0x3c, 0x2c, 0x3f, 0x3c, 0x2c,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x37, 0xf9, 0x27,
    0x3b,
};
#endif

#else
/*
 * Xclk 27Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 371.25Mbps, 2lane
 * binning to  960x540 then centered crop
 */
static const struct sc230ai_sensor_reg g_sc230ai_init_reg_table[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x2d},
    {0x3203, 0x3c},
    {0x3207, 0x03},
    {0x3208, 0x02},
    {0x3209, 0x80},
    {0x320a, 0x01},
    {0x320b, 0xe0},
    {0x320e, 0x02},
    {0x320f, 0x32},
    {0x3210, 0x00},
    {0x3211, 0xa2},
    {0x3212, 0x00},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3301, 0x09},
    {0x3306, 0x48},
    {0x3308, 0x18},
    {0x3309, 0x68},
    {0x330a, 0x00},
    {0x330b, 0xc0},
    {0x331f, 0x59},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335d, 0x60},
    {0x335e, 0x06},
    {0x335f, 0x08},
    {0x3364, 0x5e},
    {0x337c, 0x02},
    {0x337d, 0x0a},
    {0x3390, 0x01},
    {0x3391, 0x0b},
    {0x3392, 0x0f},
    {0x3393, 0x0c},
    {0x3394, 0x0d},
    {0x3395, 0x60},
    {0x3396, 0x48},
    {0x3397, 0x49},
    {0x3398, 0x4f},
    {0x3399, 0x0a},
    {0x339a, 0x0f},
    {0x339b, 0x14},
    {0x339c, 0x60},
    {0x33a2, 0x04},
    {0x33af, 0x40},
    {0x33b1, 0x80},
    {0x33b9, 0x0a},
    {0x33f9, 0x70},
    {0x33fb, 0x90},
    {0x33fc, 0x4b},
    {0x33fd, 0x5f},
    {0x349f, 0x03},
    {0x34a6, 0x4b},
    {0x34a7, 0x4f},
    {0x34ab, 0xe0},
    {0x34ac, 0x01},
    {0x34ad, 0x00},
    {0x34f8, 0x5f},
    {0x34f9, 0x10},
    {0x3630, 0xc0},
    {0x3637, 0x29},
    {0x363b, 0x20},
    {0x3670, 0x09},
    {0x3674, 0xb0},
    {0x3675, 0x80},
    {0x3676, 0x88},
    {0x367c, 0x40},
    {0x367d, 0x49},
    {0x3690, 0x44},
    {0x3691, 0x44},
    {0x3692, 0x54},
    {0x369c, 0x49},
    {0x369d, 0x4f},
    {0x36ae, 0x4b},
    {0x36af, 0x4f},
    {0x36b0, 0x87},
    {0x36b1, 0x9b},
    {0x36b2, 0xb7},
    {0x36d0, 0x01},
    {0x36eb, 0x04},
    {0x370f, 0x01},
    {0x3722, 0x17},
    {0x3728, 0x90},
    {0x37b0, 0x17},
    {0x37b1, 0x17},
    {0x37b2, 0x97},
    {0x37b3, 0x4b},
    {0x37b4, 0x4f},
    {0x37fb, 0x24},
    {0x37fc, 0x10},
    {0x37fd, 0x22},
    {0x3901, 0x02},
    {0x3904, 0x04},
    {0x391f, 0x04},
    {0x3933, 0x84},
    {0x3934, 0x02},
    {0x3940, 0x62},
    {0x3941, 0x00},
    {0x3942, 0x04},
    {0x3943, 0x03},
    {0x3e01, 0x45},
    {0x3e02, 0xb0},
    {0x440e, 0x02},
    {0x450d, 0x11},
    {0x4819, 0x05},
    {0x481b, 0x03},
    {0x481d, 0x0a},
    {0x481f, 0x02},
    {0x4821, 0x08},
    {0x4823, 0x03},
    {0x4825, 0x02},
    {0x4829, 0x04},
    {0x5000, 0x46},
    {0x5010, 0x01},
    {0x578a, 0x10},
    {0x578b, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x00},
    {0x5793, 0x10},
    {0x5794, 0x08},
    {0x5795, 0x00},
    {0x5799, 0x06},
    {0x57ad, 0x00},
    {0x5900, 0xf1},
    {0x5901, 0x04},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x3f},
    {0x5ae3, 0x38},
    {0x5ae4, 0x28},
    {0x5ae5, 0x3f},
    {0x5ae6, 0x38},
    {0x5ae7, 0x28},
    {0x5ae8, 0x3f},
    {0x5ae9, 0x3c},
    {0x5aea, 0x2c},
    {0x5aeb, 0x3f},
    {0x5aec, 0x3c},
    {0x5aed, 0x2c},
    {0x5af4, 0x3f},
    {0x5af5, 0x38},
    {0x5af6, 0x28},
    {0x5af7, 0x3f},
    {0x5af8, 0x38},
    {0x5af9, 0x28},
    {0x5afa, 0x3f},
    {0x5afb, 0x3c},
    {0x5afc, 0x2c},
    {0x5afd, 0x3f},
    {0x5afe, 0x3c},
    {0x5aff, 0x2c},
    {0x36e9, 0x20},
    {0x37f9, 0x24},
    {REG_END, 0x00},

};

/*
 * Xclk 27Mhz
 * max_framerate 25fps
 * mipi_datarate per lane 371.25Mbps, 2lane
 * full resolution
 */
static const struct sc230ai_sensor_reg g_sc230ai_1080p_reg_table[] =
{
    {0x0103, 0x01},
    {0x0100, 0x00},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x01},
    {0x320e, 0x05},
    {0x320f, 0x46},
    {0x3221, 0x66},
    {0x3301, 0x07},
    {0x3304, 0x50},
    {0x3306, 0x70},
    {0x3308, 0x18},
    {0x3309, 0x68},
    {0x330a, 0x01},
    {0x330b, 0x20},
    {0x3314, 0x15},
    {0x331e, 0x41},
    {0x331f, 0x59},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335d, 0x60},
    {0x335e, 0x06},
    {0x335f, 0x08},
    {0x3364, 0x5e},
    {0x337c, 0x02},
    {0x337d, 0x0a},
    {0x3390, 0x01},
    {0x3391, 0x0b},
    {0x3392, 0x0f},
    {0x3393, 0x09},
    {0x3394, 0x0d},
    {0x3395, 0x60},
    {0x3396, 0x48},
    {0x3397, 0x49},
    {0x3398, 0x4b},
    {0x3399, 0x06},
    {0x339a, 0x0a},
    {0x339b, 0x0d},
    {0x339c, 0x60},
    {0x33a2, 0x04},
    {0x33ad, 0x2c},
    {0x33af, 0x40},
    {0x33b1, 0x80},
    {0x33b3, 0x40},
    {0x33b9, 0x0a},
    {0x33f9, 0x78},
    {0x33fb, 0xa0},
    {0x33fc, 0x4f},
    {0x33fd, 0x5f},
    {0x349f, 0x03},
    {0x34a6, 0x4b},
    {0x34a7, 0x5f},
    {0x34a8, 0x30},
    {0x34a9, 0x20},
    {0x34aa, 0x01},
    {0x34ab, 0x28},
    {0x34ac, 0x01},
    {0x34ad, 0x58},
    {0x34f8, 0x7f},
    {0x34f9, 0x10},
    {0x3630, 0xc0},
    {0x3632, 0x54},
    {0x3633, 0x44},
    {0x363b, 0x20},
    {0x363c, 0x08},
    {0x3670, 0x09},
    {0x3674, 0xb0},
    {0x3675, 0x80},
    {0x3676, 0x88},
    {0x367c, 0x40},
    {0x367d, 0x49},
    {0x3690, 0x33},
    {0x3691, 0x33},
    {0x3692, 0x43},
    {0x369c, 0x49},
    {0x369d, 0x4f},
    {0x36ae, 0x4b},
    {0x36af, 0x4f},
    {0x36b0, 0x87},
    {0x36b1, 0x9b},
    {0x36b2, 0xb7},
    {0x36d0, 0x01},
    {0x3722, 0x97},
    {0x3724, 0x22},
    {0x3728, 0x90},
    {0x3901, 0x02},
    {0x3902, 0xc5},
    {0x3904, 0x04},
    {0x3907, 0x00},
    {0x3908, 0x41},
    {0x3909, 0x00},
    {0x390a, 0x00},
    {0x3933, 0x84},
    {0x3934, 0x0a},
    {0x3940, 0x64},
    {0x3941, 0x00},
    {0x3942, 0x04},
    {0x3943, 0x0b},
    {0x3e00, 0x00},
    {0x3e01, 0x8c},
    {0x3e02, 0x10},
    {0x440e, 0x02},
    {0x450d, 0x11},
    {0x5010, 0x01},
    {0x5787, 0x08},
    {0x5788, 0x03},
    {0x5789, 0x00},
    {0x578a, 0x10},
    {0x578b, 0x08},
    {0x578c, 0x00},
    {0x5790, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x00},
    {0x5793, 0x10},
    {0x5794, 0x08},
    {0x5795, 0x00},
    {0x5799, 0x06},
    {0x57ad, 0x00},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x3f},
    {0x5ae3, 0x38},
    {0x5ae4, 0x28},
    {0x5ae5, 0x3f},
    {0x5ae6, 0x38},
    {0x5ae7, 0x28},
    {0x5ae8, 0x3f},
    {0x5ae9, 0x3c},
    {0x5aea, 0x2c},
    {0x5aeb, 0x3f},
    {0x5aec, 0x3c},
    {0x5aed, 0x2c},
    {0x5af4, 0x3f},
    {0x5af5, 0x38},
    {0x5af6, 0x28},
    {0x5af7, 0x3f},
    {0x5af8, 0x38},
    {0x5af9, 0x28},
    {0x5afa, 0x3f},
    {0x5afb, 0x3c},
    {0x5afc, 0x2c},
    {0x5afd, 0x3f},
    {0x5afe, 0x3c},
    {0x5aff, 0x2c},
    {0x36e9, 0x20},
    {0x37f9, 0x27},
    {0x0100, 0x01},
    {REG_END, 0x00},
};
#endif

static const struct sc230ai_mode supported_modes[] =
{
    [SC230AI_640X480] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 640,
        .height = 480,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x46,
        .hts_def = 0x898,
        .vts_def = 0x0232,
        .xvclk = 27000000,
        .link_freq = 185625000,
        .reg_list = g_sc230ai_init_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc230ai_init_reg_table),
        .hdr_mode = NO_HDR,
    },
#ifdef SC230AI_MASTER_SLAVER
    [1] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 250000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0546,
        .xvclk = 27000000,
        .link_freq = 185625000,
        .reg_list = g_sc230ai_1080p_reg_table_master,
        .reg_list_size = ARRAY_SIZE(g_sc230ai_1080p_reg_table_master),
        .hdr_mode = NO_HDR,
    },
    [2] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 250000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0546,
        .xvclk = 27000000,
        .link_freq = 185625000,
        .reg_list = g_sc230ai_1080p_reg_table_slaver,
        .reg_list_size = ARRAY_SIZE(g_sc230ai_1080p_reg_table_slaver),
        .hdr_mode = NO_HDR,
    },
#else
    [SC230AI_1920X1080] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 250000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0546,
        .xvclk = 27000000,
        .link_freq = 185625000,
        .reg_list = g_sc230ai_1080p_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc230ai_1080p_reg_table),
        .hdr_mode = NO_HDR,
    }
#endif
};

struct sc230ai_ops
{
    ret_err_t (*init)(struct sc230ai_dev *dev);
    ret_err_t (*open)(struct sc230ai_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc230ai_dev *dev);
    ret_err_t (*control)(struct sc230ai_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc230ai_read_reg(struct sc230ai_dev *dev,
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
            sc230ai_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }

    return ret;
}

static ret_err_t sc230ai_write_reg(struct sc230ai_dev *dev,
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
            sc230ai_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }

}

#ifndef REG_LIST_CONVERSION_DEBUG

static int sc230ai_write_multiple_reg_continue(struct sc230ai_dev *dev,
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
            sc230ai_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t sc230ai_write_reg_discontinued(struct sc230ai_dev *dev,
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
            sc230ai_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}


static void sc230ai_write_multiple_reg(struct sc230ai_dev *dev,
                                       const struct sc230ai_sensor_reg *reg, int len)
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
                sc230ai_write_reg_discontinued(dev, data, k + 2);
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
                sc230ai_write_reg_discontinued(dev, data, k + 2);
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
                    sc230ai_write_reg_discontinued(dev, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc230ai_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc230ai_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        sc230ai_write_multiple_reg((struct sc230ai_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

static int sc230ai_get_gain_reg(uint8_t *again, uint8_t *dgain,
                                uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;

    if (total_gain < SC230AI_GAIN_MIN)
        total_gain = SC230AI_GAIN_MIN;
    else if (total_gain > SC230AI_GAIN_MAX)
        total_gain = SC230AI_GAIN_MAX;

    if (total_gain < 2000)          /* 1 ~ 2 gain*/
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 1000;
    }
    else if (total_gain < 3100)     /* 2 ~ 3.100 gain*/
    {
        *again = 0x01;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 1000 / 2;
    }
    else if (total_gain < 3100 * 2)         /* 3.100 ~ 6.200 gain*/
    {
        *again = 0x40;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 3100;
    }
    else if (total_gain < 3100 * 4)         /* 6.200 ~ 12.400 gain*/
    {
        *again = 0x48;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 3100 / 2;
    }
    else if (total_gain < 3100 * 8)         /* 12.400 ~ 24.800 gain*/
    {
        *again = 0x49;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 3100 / 4;
    }
    else if (total_gain < 3100 * 16)        /* 24.800 ~ 49.600 gain*/
    {
        *again = 0x4b;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 3100 / 8;
    }
    else if (total_gain < 3100 * 32)        /* 49.600 ~ 99.200 gain*/
    {
        *again = 0x4f;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 3100 / 16;
    }
    else if (total_gain < 3100 * 64)        /* 99.200 ~ 198.400 gain*/
    {
        *again = 0x5f;
        *dgain = 0x00;
        *dgain_fine = total_gain * 128 / 3100 / 32;
    }
    else if (total_gain < 3100 * 128)       /* 198.400 ~ 396.800 gain*/
    {
        *again = 0x5f;
        *dgain = 0x01;
        *dgain_fine = total_gain * 128 / 3100 / 64;
    }
    else if (total_gain < 3100 * 256)       /* 396.800 ~ 793.600 gain*/
    {
        *again = 0x5f;
        *dgain = 0x03;
        *dgain_fine = total_gain * 128 / 3100 / 128;
    }
    else                                    /* 793.600 ~ 1587.200 gain*/
    {
        *again = 0x5f;
        *dgain = 0x07;
        *dgain_fine = total_gain * 128 / 3100 / 256;
    }

    return ret;
}

static ret_err_t rk_sc230ai_set_expval(struct sc230ai_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, d_gain_fine;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming && strcmp(dev->name, RK_ISP_ASSIST_SUBDEV_NAME))
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc230ai_dbg("sc230ai don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0] * 2;
    l_a_gain = exp->reg_gain[0];

    sc230ai_dbg("sc230ai: exp req: L_exp: 0x%x, 0x%x\n",
                l_exp_time, l_a_gain);

    ret  = sc230ai_write_reg(dev, SC230AI_REG_EXP_LONG_H,
                             (uint8_t)((l_exp_time >> 12) & 0xf));
    ret  = sc230ai_write_reg(dev, SC230AI_REG_EXP_LONG_M,
                             (uint8_t)((l_exp_time >> 4) & 0xff));
    ret  = sc230ai_write_reg(dev, SC230AI_REG_EXP_LONG_L,
                             (uint8_t)((l_exp_time & 0xf) << 4));
    sc230ai_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    sc230ai_dbg("%s: a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", __func__, a_gain, d_gain, d_gain_fine);

    ret = sc230ai_write_reg(dev, SC230AI_REG_ANA_GAIN, a_gain);
    ret = sc230ai_write_reg(dev, SC230AI_REG_DIG_GAIN, d_gain);
    ret = sc230ai_write_reg(dev, SC230AI_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc230ai_set_vts(struct sc230ai_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    sc230ai_dbg("(%s) set vts: 0x%x \n", __FUNCTION__, dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc230ai_write_reg(dev, SC230AI_REG_VTS_L,
                            (uint8_t)(dst_vts & 0xff));
    ret |= sc230ai_write_reg(dev, SC230AI_REG_VTS_H,
                             (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc230ai_set_flip_mirror(struct sc230ai_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = sc230ai_read_reg(dev, SC230AI_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC230AI_FETCH_MIRROR(val, flip & 0x01);
        val = SC230AI_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC230AI_FETCH_MIRROR(val, false);
        val = SC230AI_FETCH_FLIP(val, false);
        break;
    };
    sc230ai_dbg("(%s) flip 0x%x, reg val 0x%x\n", __FUNCTION__, flip, val);
    ret |= sc230ai_write_reg(dev, SC230AI_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc230ai_stream_on(struct sc230ai_dev *dev)
{

    sc230ai_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    sc230ai_write_multiple_reg_continue((struct sc230ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc230ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc230ai_set_flip_mirror(dev, dev->flip);

    sc230ai_write_reg(dev, SC230AI_REG_STREAM_CTRL, SC230AI_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc230ai_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void sc230ai_stream_on_late(struct sc230ai_dev *dev)
{

    sc230ai_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    HAL_DelayMs(5);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc230ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc230ai_set_flip_mirror(dev, dev->flip);

    sc230ai_write_reg(dev, SC230AI_REG_STREAM_CTRL, SC230AI_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc230ai_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc230ai_stream_off(struct sc230ai_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc230ai_write_reg(dev, SC230AI_REG_STREAM_CTRL, SC230AI_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc230ai_err("not find out i2c bus!\n");
    }
    dev->streaming = false;

}

ret_err_t rk_sc230ai_init(struct rk_camera_device *dev)
{

    ret_err_t ret = RET_SYS_EOK;
    struct sc230ai_dev *sc230ai;

    sc230ai = (struct sc230ai_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc230ai->parent;
#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(sc230ai);
#endif

    if (sc230ai)
    {
        camera->info.mbus_fmt.width = sc230ai->cur_mode->width;
        camera->info.mbus_fmt.height = sc230ai->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc230ai->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode = sc230ai->cur_mode->hdr_mode;

        sc230ai->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc230ai->i2c_name);

        if (!sc230ai->i2c_bus)
        {
            sc230ai_err("not find i2c source 2:%s !!!\n",
                        sc230ai->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc230ai_dbg("s0 find i2c_bus:%s\n", sc230ai->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc230ai_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc230ai_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_sc230ai_get_expinf(struct sc230ai_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc230ai_mode *mode;

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

static ret_err_t rk_sc230ai_get_intput_fmt(struct sc230ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc230ai_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;

}

static ret_err_t rk_sc230ai_set_intput_fmt(struct sc230ai_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct sc230ai_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    sc230ai_info("dst resulotion, width %d, height %d\n",
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
            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
            sc230ai_write_multiple_reg_continue((struct sc230ai_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc230ai_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc230ai_match_dst_config(struct sc230ai_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc230ai_mode *mode;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;
    sc230ai_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
#ifdef SC230AI_MASTER_SLAVER
    if (dev->i2c_addr == 0x30){
        sc230ai_info("i2c-%x: main sensor as master\n", dev->i2c_addr);
        dev->dst_mode = (struct sc230ai_mode *)&supported_modes[1];
    }else if(dev->i2c_addr == 0x32){
        sc230ai_info("i2c-%x: sub sensor as slaver\n", dev->i2c_addr);
        dev->dst_mode = (struct sc230ai_mode *)&supported_modes[2];
    }
#else
    for (int i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc230ai_mode *)mode;
            ret = RET_SYS_EOK;
            sc230ai_info("find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc230ai_dbg("not find match resulotion\n");
    }
#endif
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc230ai_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }
    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc230ai_control(struct rk_camera_device *dev,
                             int cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc230ai_dev *sc230ai;

    RT_ASSERT(dev != RT_NULL);
    sc230ai = (struct sc230ai_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc230ai_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc230ai_stream_on(sc230ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc230ai_stream_off(sc230ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc230ai_get_expinf(sc230ai, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc230ai_set_expval(sc230ai, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc230ai_set_vts(sc230ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc230ai_get_intput_fmt(sc230ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc230ai_set_intput_fmt(sc230ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc230ai_match_dst_config(sc230ai, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc230ai_set_flip_mirror(sc230ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc230ai_stream_on_late(sc230ai);
    }
    break;
    default:
        sc230ai_dbg("(%s) exit CMD %d\n", __FUNCTION__, cmd);
        break;
    }
    return ret;
}

struct rk_camera_ops rk_sc230ai_ops =
{
    rk_sc230ai_init,
    rk_sc230ai_open,
    NULL,
    rk_sc230ai_control
};


int rk_camera_sc230ai_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct camera_board_desc *sc230ai;
    for(sc230ai = camera_sc230ai; sc230ai->i2c_addr != 0; sc230ai++){
        struct sc230ai_dev *instance;
        struct rk_camera_device *camera;

        instance = rt_malloc(sizeof(struct sc230ai_dev));
        if(!instance){
            rt_kprintf("malloc sc230ai failed\n");
            return -RT_ENOMEM;
        }
        rt_memset(instance, 0x0, sizeof(struct sc230ai_dev));
        camera = &instance->parent;
        camera->ops = &rk_sc230ai_ops;

        //struct camera_board_desc *sc230ai = (struct camera_board_desc *)&camera_sc230ai;
        camera->ops = &rk_sc230ai_ops;

        rt_strncpy(instance->name, sc230ai->isp_subdev_name, rt_strlen(sc230ai->isp_subdev_name));
        rt_strncpy(instance->i2c_name, sc230ai->i2c_bus, rt_strlen(sc230ai->i2c_bus));

        instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
        if (sc230ai->mode_id <= SC230AI_MODE_ID_MAX)
        {
            sc230ai_dbg("mode_id: %d\n", sc230ai->mode_id);
            instance->cur_mode = &supported_modes[sc230ai->mode_id];
        }
        else
        {
            sc230ai_info("mode id is over range, default use mode_id: 0\n");
            instance->cur_mode = &supported_modes[0];
        }
        instance->dst_mode = (struct sc230ai_mode *)&supported_modes[SC230AI_1920X1080];
        instance->time_valid_delay = sc230ai->time_valid_delay;
        instance->gain_valid_delay = sc230ai->gain_valid_delay;
        instance->i2c_addr = sc230ai->i2c_addr;
        instance->flip = 0;

        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc230ai->rst_gpio.gpio_group, sc230ai->rst_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc230ai->rst_gpio.gpio_group, sc230ai->rst_gpio.gpio_pin, !sc230ai->rst_active);

        if(sc230ai->pwdn_gpio.gpio_group){
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc230ai->pwdn_gpio.gpio_group, sc230ai->pwdn_gpio.gpio_pin, GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc230ai->pwdn_gpio.gpio_group, sc230ai->pwdn_gpio.gpio_pin, !sc230ai->pwdn_active);
        }
        if (sc230ai->mclk_out_gate_id && sc230ai->mclk_id)
        {
            clk_set_rate(sc230ai->mclk_id, instance->cur_mode->xvclk);
            HAL_CRU_ClkEnable(sc230ai->mclk_out_gate_id);
        }

        rt_mutex_init(&instance->mutex_lock, "SC230AI_mutex", RT_IPC_FLAG_FIFO);
        RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
        camera->i2c_bus = instance->i2c_bus;
        rt_strncpy(camera->name, sc230ai->isp_subdev_name, rt_strlen(sc230ai->isp_subdev_name));
        rk_camera_register(camera, camera->name, instance);
    }
    return ret;
}

void sc230ai_detect(void)
{
    const struct camera_board_desc *sc230ai_desc;
    struct rt_i2c_bus_device *i2c_bus;
    for(sc230ai_desc = camera_sc230ai; sc230ai_desc->i2c_addr != 0; sc230ai_desc++){
        sc230ai_dbg("start to detect SC230AI for testing \n");
        sc230ai_dbg("dev name:%s\n", sc230ai_desc->isp_subdev_name);
        sc230ai_dbg("dev i2c_bus:%s\n", sc230ai_desc->i2c_bus);
        i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc230ai_desc->i2c_bus);
        if (i2c_bus)
        {
            sc230ai_err("Warning:not find i2c source 1:%s !!!\n", sc230ai_desc->i2c_bus);
            return;
        }
        else
        {
            sc230ai_dbg("find i2c_bus:%s\n", sc230ai_desc->i2c_bus);
        }
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc230ai_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc230ai_detect, check SC230AI is available or not);
#endif
#endif
#endif
