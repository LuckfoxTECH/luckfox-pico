/**
 * Copyright (c) 2023 Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 * @file    drv_gc4023.c
 * @version V0.0.1
 *
 * Change Logs:
 * 2023-12-20     Wang Xiaobin      first implementation
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

#ifdef RT_USING_GC4023

// #define REG_LIST_CONVERSION_DEBUG
#define GC4023_DEBUG_PRT 0

#if GC4023_DEBUG_PRT
#include <stdio.h>
#define gc4023_dbg(fmt, args...)    rt_kprintf("[GC4023] " fmt "", ##args)
#else
#define gc4023_dbg(fmt, args...)
#endif

#define gc4023_info(fmt, args...)         rt_kprintf("[GC4023] "fmt"", ##args)
#define gc4023_err(fmt, args...)         rt_kprintf("[GC4023] ERR "fmt"", ##args)

#define GC4023_REG_CTRL_MODE    0x0100
#define GC4023_MODE_SW_STANDBY  0x00
#define GC4023_MODE_STREAMING   0x09

#define GC4023_REG_EXPOSURE_H   0x0202
#define GC4023_REG_EXPOSURE_L   0x0203
#define GC4023_EXPOSURE_MIN     4
#define GC4023_EXPOSURE_STEP    1
#define GC4023_EXP_MIN          2
#define GC4023_VTS_MAX          0x7fff

#define GC4023_GAIN_MIN         0x0040
#define GC4023_GAIN_MAX         (0xffff)
#define GC4023_GAIN_STEP        1
#define GC4023_GAIN_DEFAULT     256

#define GC4023_REG_VTS_H        0x0340
#define GC4023_REG_VTS_L        0x0341

#define GC4023_FLIP_MIRROR_REG   0x022c
#define GC4023_FLIP_MIRROR_REG_1 0x0a73

#define GC4023_FETCH_MIRROR(VAL, ENABLE) (ENABLE ? VAL | 0x01 : VAL & 0xfe)
#define GC4023_FETCH_FLIP(VAL, ENABLE) (ENABLE ? VAL | 0x02 : VAL & 0xfd)

#define REG_END (0x0)
#define REG_DELAY (0xff)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* redefine system err code */
#define RET_SYS_EOK (RT_EOK)
#define RET_SYS_ERROR (-RT_ERROR)
#define RET_SYS_ETIMEOUT (-RT_ETIMEOUT) /**< Timed out */
#define RET_SYS_EFULL (-RT_EFULL)       /**< The resource is full */
#define RET_SYS_EEMPTY (-RT_EEMPTY)     /**< The resource is empty */
#define RET_SYS_ENOMEM (-RT_ENOMEM)     /**< No memory */
#define RET_SYS_ENOSYS (-RT_ENOSYS)     /**< No system */
#define RET_SYS_EBUSY (-RT_EBUSY)       /**< Busy */
#define RET_SYS_EIO (-RT_EIO)           /**< IO error */
#define RET_SYS_EINTR (-RT_EINTR)       /**< Interrupted system call */
#define RET_SYS_EINVAL (-RT_EINVAL)     /**< Invalid argument */

struct rt_i2c_bus_device;

struct gc4023_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct gc4023_mode
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
    const uint8_t *stream_on_reg_list;
#else
    const struct gc4023_sensor_reg *reg_list;
    const struct gc4023_sensor_reg *stream_on_reg_list;
#endif
    const int reg_list_size;
    const int stream_on_reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct gc4023_dev
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
    const struct gc4023_mode *cur_mode;
    struct gc4023_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct gc4023_dev *rt_gc4023_dev_t;

static struct gc4023_dev g_gc4023;

static const uint32_t reg_val_table_liner[26][7] =
{
 //   614   615   218   1467  1468  b8    b9
    {0x00, 0x00, 0x00, 0x0D, 0x15, 0x01, 0x00},
    {0x80, 0x02, 0x00, 0x0D, 0x15, 0x01, 0x0B},
    {0x01, 0x00, 0x00, 0x0D, 0x15, 0x01, 0x19},
    {0x81, 0x02, 0x00, 0x0E, 0x16, 0x01, 0x2A},
    {0x02, 0x00, 0x00, 0x0E, 0x16, 0x02, 0x00},
    {0x82, 0x02, 0x00, 0x0F, 0x17, 0x02, 0x17},
    {0x03, 0x00, 0x00, 0x10, 0x18, 0x02, 0x33},
    {0x83, 0x02, 0x00, 0x11, 0x19, 0x03, 0x14},
    {0x04, 0x00, 0x00, 0x12, 0x1a, 0x04, 0x00},
    {0x80, 0x02, 0x20, 0x13, 0x1b, 0x04, 0x2F},
    {0x01, 0x00, 0x20, 0x14, 0x1c, 0x05, 0x26},
    {0x81, 0x02, 0x20, 0x15, 0x1d, 0x06, 0x28},
    {0x02, 0x00, 0x20, 0x16, 0x1e, 0x08, 0x00},
    {0x82, 0x02, 0x20, 0x16, 0x1e, 0x09, 0x1E},
    {0x03, 0x00, 0x20, 0x18, 0x20, 0x0B, 0x0C},
    {0x83, 0x02, 0x20, 0x18, 0x20, 0x0D, 0x11},
    {0x04, 0x00, 0x20, 0x18, 0x20, 0x10, 0x00},
    {0x84, 0x02, 0x20, 0x19, 0x21, 0x12, 0x3D},
    {0x05, 0x00, 0x20, 0x19, 0x21, 0x16, 0x19},
    {0x85, 0x02, 0x20, 0x1A, 0x22, 0x1A, 0x22},
    {0xb5, 0x04, 0x20, 0x1B, 0x23, 0x20, 0x00},
    {0x85, 0x05, 0x20, 0x1B, 0x23, 0x25, 0x3A},
    {0x05, 0x08, 0x20, 0x1C, 0x24, 0x2C, 0x33},
    {0x45, 0x09, 0x20, 0x1D, 0x25, 0x35, 0x05},
    {0x55, 0x0a, 0x20, 0x1F, 0x27, 0x40, 0x00},
};

static const uint32_t gain_level_table[26] =
{
    64,
    76,
    90,
    106,
    128,
    152,
    179,
    212,
    256,
    303,
    358,
    425,
    512,
    607,
    717,
    849,
    1024,
    1213,
    1434,
    1699,
    2048,
    2427,
    2867,
    3398,
    4096,
    0xffff,
};

#ifndef REG_LIST_CONVERSION_DEBUG
static const uint8_t g_gc4023_1280x720_60fps_reg_table[] =
{
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x0a, 0x34, 0x00,
    0x3, 0x06, 0x1c, 0x10,
    0x3, 0x03, 0x1c, 0x01,
    0x3, 0x0a, 0x38, 0x00,
    0x3, 0x01, 0x15, 0x12,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0x00,
    0x3, 0x03, 0xfe, 0x10,
    0x3, 0x03, 0xfe, 0x00,
    0x3, 0x0a, 0x38, 0x00,
    0x3, 0x0a, 0x38, 0x01,
    0x3, 0x0a, 0x20, 0x17,
    0x6, 0x06, 0x1c, 0x50, 0x21, 0x60, 0x06,
    0x3, 0x0a, 0x21, 0x10,
    0x6, 0x0a, 0x34, 0x40, 0x01, 0x4e, 0x06,
    0x3, 0x0a, 0x28, 0x01,
    0x4, 0x03, 0x14, 0x50, 0x00,
    0x3, 0x03, 0x1c, 0xce,
    0x3, 0x02, 0x19, 0x47,
    0x4, 0x03, 0x42, 0x04, 0x65,
    0x4, 0x02, 0x59, 0x02, 0xfa,
    0x4, 0x03, 0x40, 0x03, 0x20,
    0x7, 0x03, 0x47, 0x02, 0x0a, 0x08, 0x05, 0xa8,
    0x6, 0x00, 0x94, 0x0a, 0x00, 0x05, 0xa0,
    0x3, 0x00, 0x99, 0x03,
    0x3, 0x00, 0x9b, 0x04,
    0x3, 0x02, 0x18, 0x10,
    0x3, 0x00, 0x77, 0x08,
    0x3, 0x06, 0x0c, 0x01,
    0x4, 0x06, 0x0e, 0x08, 0x05,
    0x3, 0x07, 0x0c, 0x01,
    0x4, 0x07, 0x0e, 0x08, 0x05,
    0x3, 0x09, 0x09, 0x03,
    0x3, 0x09, 0x02, 0x04,
    0x3, 0x09, 0x04, 0x0b,
    0x4, 0x09, 0x07, 0x54, 0x06,
    0x3, 0x09, 0x03, 0x9d,
    0x3, 0x07, 0x2a, 0x18,
    0x3, 0x07, 0x24, 0x0a,
    0x3, 0x07, 0x27, 0x0a,
    0x4, 0x07, 0x2a, 0x1c, 0x0a,
    0x3, 0x14, 0x66, 0x10,
    0x3, 0x14, 0x68, 0x12,
    0x3, 0x14, 0x67, 0x1a,
    0x4, 0x14, 0x69, 0x80, 0xe8,
    0x3, 0x07, 0x07, 0x07,
    0x3, 0x07, 0x37, 0x0f,
    0x3, 0x07, 0x04, 0x01,
    0x3, 0x07, 0x06, 0x02,
    0x3, 0x07, 0x16, 0x02,
    0x3, 0x07, 0x08, 0xc8,
    0x3, 0x07, 0x18, 0xc8,
    0x3, 0x06, 0x1a, 0x00,
    0x3, 0x14, 0x30, 0x80,
    0x5, 0x14, 0x07, 0x10, 0x16, 0x03,
    0xb, 0x14, 0x6d, 0x0e, 0x42, 0x43, 0x3c, 0x3d, 0x3a, 0x3a, 0x40, 0x46,
    0x3, 0x14, 0x20, 0x14,
    0x3, 0x14, 0x64, 0x15,
    0x4, 0x14, 0x6c, 0x40, 0x40,
    0x3, 0x14, 0x23, 0x08,
    0x3, 0x14, 0x28, 0x10,
    0x3, 0x14, 0x62, 0x18,
    0x3, 0x02, 0xce, 0x04,
    0x3, 0x14, 0x3a, 0x0f,
    0x3, 0x14, 0x2b, 0x88,
    0x3, 0x02, 0x45, 0xc9,
    0x3, 0x02, 0x3a, 0x08,
    0x3, 0x02, 0xcd, 0x99,
    0x4, 0x06, 0x12, 0x02, 0xc7,
    0x3, 0x02, 0x43, 0x03,
    0x3, 0x02, 0x1b, 0x09,
    0x3, 0x00, 0x89, 0x03,
    0x3, 0x00, 0x40, 0xa3,
    0x3, 0x00, 0x75, 0x64,
    0x3, 0x00, 0x04, 0x0f,
    0x3, 0x00, 0x02, 0xab,
    0x3, 0x00, 0x53, 0x0a,
    0x3, 0x02, 0x05, 0x0c,
    0x4, 0x02, 0x02, 0x06, 0x27,
    0x4, 0x06, 0x14, 0x00, 0x00,
    0x4, 0x01, 0x81, 0x0c, 0x05,
    0x3, 0x01, 0x85, 0x01,
    0x3, 0x01, 0x80, 0x46,
    0x3, 0x01, 0x00, 0x08,
    0x3, 0x01, 0x06, 0x38,
    0x4, 0x01, 0x0d, 0x40, 0x06,
    0x5, 0x01, 0x13, 0x02, 0x01, 0x12,
    0x3, 0x00, 0x87, 0x50,
    0x3, 0x00, 0x52, 0x02,
    0x3, 0x00, 0x76, 0x01,
    0x3, 0x02, 0x1a, 0x10,
    0x6, 0x04, 0x34, 0x75, 0x75, 0x75, 0x75,
    0x6, 0x04, 0x30, 0x0a, 0x0a, 0x0a, 0x0a,
    0x6, 0x04, 0x58, 0x00, 0x00, 0x00, 0x00,
    0x3, 0x0a, 0x67, 0x80,
    0x3, 0x0a, 0x54, 0x0e,
    0x3, 0x0a, 0x65, 0x10,
    0x3, 0x0a, 0x98, 0x10,
    0x3, 0x05, 0xbe, 0x00,
    0x3, 0x05, 0xa9, 0x01,
    0x3, 0x00, 0x29, 0x08,
    0x3, 0x00, 0x2b, 0xa8,
    0x3, 0x0a, 0x83, 0xe0,
    0x4, 0x0a, 0x72, 0x02, 0x60,
    0x3, 0x0a, 0x75, 0x41,
    0x3, 0x0a, 0x70, 0x03,
    0x3, 0x0a, 0x5a, 0x80,
    0x3, 0x05, 0xbe, 0x01,
    0x3, 0x0a, 0x70, 0x00,
    0x3, 0x00, 0x80, 0x02,
    0x3,  0x0a,  0x67,  0x00,
    0x6f,
};

static const uint8_t g_gc4023_2560x1440_15fps_reg_table[] =
{
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x0a, 0x34, 0x00,
    0x3, 0x06, 0x1c, 0x10,
    0x3, 0x03, 0x1c, 0x01,
    0x3, 0x0a, 0x38, 0x00,
    0x3, 0x01, 0x15, 0x12,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0x00,
    0x3, 0x03, 0xfe, 0x10,
    0x3, 0x03, 0xfe, 0x00,
    0x3, 0x0a, 0x38, 0x00,
    0x3, 0x0a, 0x38, 0x01,
    0x3, 0x0a, 0x20, 0x07,
    0x6, 0x06, 0x1c, 0x50, 0x22, 0x78, 0x06,
    0x3, 0x0a, 0x21, 0x10,
    0x6, 0x0a, 0x34, 0x40, 0x01, 0x4e, 0x06,
    0x4, 0x03, 0x14, 0x50, 0x00,
    0x3, 0x03, 0x1c, 0xce,
    0x3, 0x02, 0x19, 0x47,
    0x4, 0x03, 0x42, 0x04, 0xb0,
    0x4, 0x02, 0x59, 0x05, 0xa0,
    0x4, 0x03, 0x40, 0x0b, 0xb8,
    0x7, 0x03, 0x47, 0x02, 0x0a, 0x08, 0x05, 0xa8,
    0x6, 0x00, 0x94, 0x0a, 0x00, 0x05, 0xa0,
    0x3, 0x00, 0x99, 0x04,
    0x3, 0x00, 0x9b, 0x04,
    0x3, 0x02, 0x18, 0x00,
    0x3, 0x06, 0x0c, 0x01,
    0x4, 0x06, 0x0e, 0x08, 0x05,
    0x3, 0x07, 0x0c, 0x01,
    0x4, 0x07, 0x0e, 0x08, 0x05,
    0x3, 0x09, 0x09, 0x03,
    0x3, 0x09, 0x02, 0x04,
    0x3, 0x09, 0x04, 0x0b,
    0x4, 0x09, 0x07, 0x54, 0x06,
    0x3, 0x09, 0x03, 0x9d,
    0x3, 0x07, 0x2a, 0x18,
    0x3, 0x07, 0x24, 0x0a,
    0x3, 0x07, 0x27, 0x0a,
    0x4, 0x07, 0x2a, 0x1c, 0x0a,
    0x3, 0x14, 0x66, 0x10,
    0x3, 0x14, 0x68, 0x0b,
    0x3, 0x14, 0x67, 0x13,
    0x4, 0x14, 0x69, 0x80, 0xe8,
    0x3, 0x07, 0x07, 0x07,
    0x3, 0x07, 0x37, 0x0f,
    0x3, 0x07, 0x04, 0x01,
    0x3, 0x07, 0x06, 0x03,
    0x3, 0x07, 0x16, 0x03,
    0x3, 0x07, 0x08, 0xc8,
    0x3, 0x07, 0x18, 0xc8,
    0x3, 0x06, 0x1a, 0x00,
    0x3, 0x14, 0x30, 0x80,
    0x5, 0x14, 0x07, 0x10, 0x16, 0x03,
    0xb, 0x14, 0x6d, 0x0e, 0x42, 0x43, 0x3c, 0x3d, 0x3a, 0x3a, 0x40, 0x46,
    0x3, 0x14, 0x20, 0x14,
    0x3, 0x14, 0x64, 0x15,
    0x4, 0x14, 0x6c, 0x40, 0x40,
    0x3, 0x14, 0x23, 0x08,
    0x3, 0x14, 0x28, 0x10,
    0x3, 0x14, 0x62, 0x18,
    0x3, 0x02, 0xce, 0x04,
    0x3, 0x14, 0x3a, 0x0f,
    0x3, 0x14, 0x2b, 0x88,
    0x3, 0x02, 0x45, 0xc9,
    0x3, 0x02, 0x3a, 0x08,
    0x3, 0x02, 0xcd, 0x99,
    0x4, 0x06, 0x12, 0x02, 0xc7,
    0x3, 0x02, 0x43, 0x03,
    0x3, 0x02, 0x1b, 0x09,
    0x3, 0x00, 0x89, 0x03,
    0x3, 0x00, 0x40, 0xa3,
    0x3, 0x00, 0x75, 0x64,
    0x3, 0x00, 0x04, 0x0f,
    0x3, 0x00, 0x02, 0xab,
    0x3, 0x00, 0x53, 0x0a,
    0x3, 0x02, 0x05, 0x0c,
    0x4, 0x02, 0x02, 0x06, 0x27,
    0x4, 0x06, 0x14, 0x00, 0x00,
    0x4, 0x01, 0x81, 0x0c, 0x05,
    0x3, 0x01, 0x85, 0x01,
    0x3, 0x01, 0x80, 0x46,
    0x3, 0x01, 0x00, 0x08,
    0x3, 0x01, 0x06, 0x38,
    0x4, 0x01, 0x0d, 0x80, 0x0c,
    0x5, 0x01, 0x13, 0x02, 0x01, 0x12,
    0x3, 0x02, 0x2c, 0x00,
    0x3, 0x0a, 0x67, 0x80,
    0x3, 0x0a, 0x54, 0x0e,
    0x3, 0x0a, 0x65, 0x10,
    0x3, 0x0a, 0x98, 0x10,
    0x3, 0x05, 0xbe, 0x00,
    0x3, 0x05, 0xa9, 0x01,
    0x3, 0x00, 0x29, 0x08,
    0x3, 0x00, 0x2b, 0xa8,
    0x3, 0x0a, 0x83, 0xe0,
    0x4, 0x0a, 0x72, 0x02, 0x60,
    0x3, 0x0a, 0x75, 0x41,
    0x3, 0x0a, 0x70, 0x03,
    0x3, 0x0a, 0x5a, 0x80,
    0x3, 0x05, 0xbe, 0x01,
    0x3, 0x0a, 0x70, 0x00,
    0x3, 0x00, 0x80, 0x02,
    0x3,  0x0a,  0x67,  0x00,
    0x67,
};

#else
static const struct gc4023_sensor_reg g_gc4023_1280x720_60fps_reg_table[] =
{
    {0x0100, 0x00},
    {0x0a34, 0x00},
    {0x061c, 0x10},
    {0x031c, 0x01},
    {0x0a38, 0x00},
    {0x0115, 0x12}
    {0x03fe, 0xf0},
    {0x03fe, 0x00},
    {0x03fe, 0x10},
    {0x03fe, 0x00},
    {0x0a38, 0x00},
    {0x0a38, 0x01},
    {0x0a20, 0x17},
    {0x061c, 0x50},
    {0x061d, 0x21},
    {0x061e, 0x60},
    {0x061f, 0x06},
    {0x0a21, 0x10},
    {0x0a34, 0x40},
    {0x0a35, 0x01},
    {0x0a36, 0x4e},
    {0x0a37, 0x06},
    {0x0a28, 0x01},
    {0x0314, 0x50},
    {0x0315, 0x00},
    {0x031c, 0xce},
    {0x0219, 0x47},
    {0x0342, 0x04},
    {0x0343, 0x65},
    {0x0259, 0x02},
    {0x025a, 0xfa},
    {0x0340, 0x03},
    {0x0341, 0x20},
    {0x0347, 0x02},
    {0x0348, 0x0a},
    {0x0349, 0x08},
    {0x034a, 0x05},
    {0x034b, 0xa8},
    {0x0094, 0x0a},
    {0x0095, 0x00},
    {0x0096, 0x05},
    {0x0097, 0xa0},
    {0x0099, 0x03},
    {0x009b, 0x04},
    {0x0218, 0x10},
    {0x0077, 0x08},
    {0x060c, 0x01},
    {0x060e, 0x08},
    {0x060f, 0x05},
    {0x070c, 0x01},
    {0x070e, 0x08},
    {0x070f, 0x05},
    {0x0909, 0x03},
    {0x0902, 0x04},
    {0x0904, 0x0b},
    {0x0907, 0x54},
    {0x0908, 0x06},
    {0x0903, 0x9d},
    {0x072a, 0x18},
    {0x0724, 0x0a},
    {0x0727, 0x0a},
    {0x072a, 0x1c},
    {0x072b, 0x0a},
    {0x1466, 0x10},
    {0x1468, 0x12},
    {0x1467, 0x1a},
    {0x1469, 0x80},
    {0x146a, 0xe8},
    {0x0707, 0x07},
    {0x0737, 0x0f},
    {0x0704, 0x01},
    {0x0706, 0x02},
    {0x0716, 0x02},
    {0x0708, 0xc8},
    {0x0718, 0xc8},
    {0x061a, 0x00},
    {0x1430, 0x80},
    {0x1407, 0x10},
    {0x1408, 0x16},
    {0x1409, 0x03},
    {0x146d, 0x0e},
    {0x146e, 0x42},
    {0x146f, 0x43},
    {0x1470, 0x3c},
    {0x1471, 0x3d},
    {0x1472, 0x3a},
    {0x1473, 0x3a},
    {0x1474, 0x40},
    {0x1475, 0x46},
    {0x1420, 0x14},
    {0x1464, 0x15},
    {0x146c, 0x40},
    {0x146d, 0x40},
    {0x1423, 0x08},
    {0x1428, 0x10},
    {0x1462, 0x18},
    {0x02ce, 0x04},
    {0x143a, 0x0f},
    {0x142b, 0x88},
    {0x0245, 0xc9},
    {0x023a, 0x08},
    {0x02cd, 0x99},
    {0x0612, 0x02},
    {0x0613, 0xc7},
    {0x0243, 0x03},
    {0x021b, 0x09},
    {0x0089, 0x03},
    {0x0040, 0xa3},
    {0x0075, 0x64},
    {0x0004, 0x0f},
    {0x0002, 0xab},
    {0x0053, 0x0a},
    {0x0205, 0x0c},
    {0x0202, 0x06},
    {0x0203, 0x27},
    {0x0614, 0x00},
    {0x0615, 0x00},
    {0x0181, 0x0c},
    {0x0182, 0x05},
    {0x0185, 0x01},
    {0x0180, 0x46},
    {0x0100, 0x08},
    {0x0106, 0x38},
    {0x010d, 0x40},
    {0x010e, 0x06},
    {0x0113, 0x02},
    {0x0114, 0x01},
    {0x0115, 0x12},
    {0x0087, 0x50},
    {0x0052, 0x02},
    {0x0076, 0x01},
    {0x021a, 0x10},
    {0x0434, 0x75},
    {0x0435, 0x75},
    {0x0436, 0x75},
    {0x0437, 0x75},
    {0x0430, 0x0a},
    {0x0431, 0x0a},
    {0x0432, 0x0a},
    {0x0433, 0x0a},
    {0x0458, 0x00},
    {0x0459, 0x00},
    {0x045a, 0x00},
    {0x045b, 0x00},
    {0x0a67, 0x80},
    {0x0a54, 0x0e},
    {0x0a65, 0x10},
    {0x0a98, 0x10},
    {0x05be, 0x00},
    {0x05a9, 0x01},
    {0x0029, 0x08},
    {0x002b, 0xa8},
    {0x0a83, 0xe0},
    {0x0a72, 0x02},
    {0x0a73, 0x60},
    {0x0a75, 0x41},
    {0x0a70, 0x03},
    {0x0a5a, 0x80},
    {0x00ff, 0x14},
    {0x05be, 0x01},
    {0x0a70, 0x00},
    {0x0080, 0x02},
    {0x0a67, 0x00},
    {0x0000, 0x00},
};
/*
 * Xclk 27Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 630Mbps, 2lane
 * full resolution
 */
static const struct gc4023_sensor_reg g_gc4023_2560x1440_30fps_reg_table[] =
{
    {0x0100, 0x00},
    {0x0a34, 0x00},
    {0x061c, 0x10},
    {0x031c, 0x01},
    {0x0a38, 0x00},
    {0x0115, 0x12}
    {0x03fe, 0xf0},
    {0x03fe, 0x00},
    {0x03fe, 0x10},
    {0x03fe, 0x00},
    {0x0a38, 0x00},
    {0x0a38, 0x01},
    {0x0a20, 0x07},
    {0x061c, 0x50},
    {0x061d, 0x22},
    {0x061e, 0x78},
    {0x061f, 0x06},
    {0x0a21, 0x10},
    {0x0a34, 0x40},
    {0x0a35, 0x01},
    {0x0a36, 0x4e},
    {0x0a37, 0x06},
    {0x0314, 0x50},
    {0x0315, 0x00},
    {0x031c, 0xce},
    {0x0219, 0x47},
    {0x0342, 0x04},
    {0x0343, 0xb0},
    {0x0259, 0x05},
    {0x025a, 0xa0},
    {0x0340, 0x05},
    {0x0341, 0xdc},
    {0x0347, 0x02},
    {0x0348, 0x0a},
    {0x0349, 0x08},
    {0x034a, 0x05},
    {0x034b, 0xa8},
    {0x0094, 0x0a},
    {0x0095, 0x00},
    {0x0096, 0x05},
    {0x0097, 0xa0},
    {0x0099, 0x04},
    {0x009b, 0x04},
    {0x0218, 0x00},
    {0x060c, 0x01},
    {0x060e, 0x08},
    {0x060f, 0x05},
    {0x070c, 0x01},
    {0x070e, 0x08},
    {0x070f, 0x05},
    {0x0909, 0x03},
    {0x0902, 0x04},
    {0x0904, 0x0b},
    {0x0907, 0x54},
    {0x0908, 0x06},
    {0x0903, 0x9d},
    {0x072a, 0x18},
    {0x0724, 0x0a},
    {0x0727, 0x0a},
    {0x072a, 0x1c},
    {0x072b, 0x0a},
    {0x1466, 0x10},
    {0x1468, 0x0b},
    {0x1467, 0x13},
    {0x1469, 0x80},
    {0x146a, 0xe8},
    {0x0707, 0x07},
    {0x0737, 0x0f},
    {0x0704, 0x01},
    {0x0706, 0x03},
    {0x0716, 0x03},
    {0x0708, 0xc8},
    {0x0718, 0xc8},
    {0x061a, 0x00},
    {0x1430, 0x80},
    {0x1407, 0x10},
    {0x1408, 0x16},
    {0x1409, 0x03},
    {0x146d, 0x0e},
    {0x146e, 0x42},
    {0x146f, 0x43},
    {0x1470, 0x3c},
    {0x1471, 0x3d},
    {0x1472, 0x3a},
    {0x1473, 0x3a},
    {0x1474, 0x40},
    {0x1475, 0x46},
    {0x1420, 0x14},
    {0x1464, 0x15},
    {0x146c, 0x40},
    {0x146d, 0x40},
    {0x1423, 0x08},
    {0x1428, 0x10},
    {0x1462, 0x18},
    {0x02ce, 0x04},
    {0x143a, 0x0f},
    {0x142b, 0x88},
    {0x0245, 0xc9},
    {0x023a, 0x08},
    {0x02cd, 0x99},
    {0x0612, 0x02},
    {0x0613, 0xc7},
    {0x0243, 0x03},
    {0x021b, 0x09},
    {0x0089, 0x03},
    {0x0040, 0xa3},
    {0x0075, 0x64},
    {0x0004, 0x0f},
    {0x0002, 0xab},
    {0x0053, 0x0a},
    {0x0205, 0x0c},
    {0x0202, 0x06},
    {0x0203, 0x27},
    {0x0614, 0x00},
    {0x0615, 0x00},
    {0x0181, 0x0c},
    {0x0182, 0x05},
    {0x0185, 0x01},
    {0x0180, 0x46},
    {0x0100, 0x08},
    {0x0106, 0x38},
    {0x010d, 0x80},
    {0x010e, 0x0c},
    {0x0113, 0x02},
    {0x0114, 0x01},
    {0x0115, 0x12},
    {0x022c, 0x00},
    {0x0a67, 0x80},
    {0x0a54, 0x0e},
    {0x0a65, 0x10},
    {0x0a98, 0x10},
    {0x05be, 0x00},
    {0x05a9, 0x01},
    {0x0029, 0x08},
    {0x002b, 0xa8},
    {0x0a83, 0xe0},
    {0x0a72, 0x02},
    {0x0a73, 0x60},
    {0x0a75, 0x41},
    {0x0a70, 0x03},
    {0x0a5a, 0x80},
    {0x00ff, 0x14},
    {0x05be, 0x01},
    {0x0a70, 0x00},
    {0x0080, 0x02},
    {0x0a67, 0x00},
    {0x0000, 0x00},
};

#endif

static const struct gc4023_mode supported_modes[] =
{
    [GC4023_1280X720] = {
        .bus_fmt = MEDIA_BUS_FMT_SRGGB10_1X10,
        .width = 1280,
        .height = 720,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x0100,
        .hts_def = 0x0AA0,
        .vts_def = 0x02EE,
        .xvclk = 27000000,
        .link_freq = 306000000,
        .reg_list = g_gc4023_1280x720_60fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_gc4023_1280x720_60fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [GC4023_2560X1440] = {
        .bus_fmt = MEDIA_BUS_FMT_SRGGB10_1X10,
        .width = 2560,
        .height = 1440,
        .max_fps = {
            .numerator = 10000,
            .denominator = 150000,
        },
        .exp_def = 0x0100,
        .hts_def = 0x0AA0,
        .vts_def = 0x0BB8,
        .xvclk = 27000000,
        .link_freq = 351000000,
        .reg_list = g_gc4023_2560x1440_15fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_gc4023_2560x1440_15fps_reg_table),
        .hdr_mode = NO_HDR,
    }
};

struct gc4023_ops
{
    ret_err_t (*init)(struct gc4023_dev *dev);
    ret_err_t (*open)(struct gc4023_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct gc4023_dev *dev);
    ret_err_t (*control)(struct gc4023_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t gc4023_read_reg(struct rt_i2c_bus_device *bus,
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
    msg[0].addr = g_gc4023.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_gc4023.i2c_addr;
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
            gc4023_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t gc4023_write_reg(struct rt_i2c_bus_device *bus,
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

    msg.addr = g_gc4023.i2c_addr;
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
            gc4023_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int gc4023_write_multiple_reg_continue(struct gc4023_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msg;
    int ret = 0;
    int offset = 0;
    int retry = 10;

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(i2c_data != RT_NULL);

    i2c_bus = dev->i2c_bus;
    RT_ASSERT(i2c_bus != RT_NULL);

    for (i = 0; i < i2c_data[len - 1];)
    {
        msg.addr = g_gc4023.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            gc4023_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t gc4023_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msg.addr = g_gc4023.i2c_addr;
    msg.flags = RT_I2C_WR;
    msg.buf = (unsigned char *)data;
    msg.len = len;
    while (1)
    {
        ret = rt_i2c_transfer(bus, &msg, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            gc4023_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void gc4023_write_multiple_reg(struct gc4023_dev *dev,
                                      const struct gc4023_sensor_reg *reg, int len)
{
    uint16_t i;
    struct  rt_i2c_bus_device *i2c_bus;
    int k = 0;
    char *data = rt_malloc(len);
    uint16_t reg_addr;
    int j = 0;
    int cnt = 0;

    RT_ASSERT(dev != RT_NULL && i2c_data != RT_NULL);

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
                gc4023_write_reg_discontinued(i2c_bus, data, k + 2);
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
                gc4023_write_reg_discontinued(i2c_bus, data, k + 2);
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
                    // rt_kprintf(">>>k %d, addr %04x\n", k, reg[i].reg_addr);
                }
                else
                {
                    cnt++;
                    rt_kprintf("0x%x, ", k + 2);
                    for (j = 0; j < k + 2; j++)
                        rt_kprintf("0x%02x, ", data[j]);
                    rt_kprintf("\n");
                    gc4023_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct gc4023_dev *dev)
{

    int id = 0;
    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        gc4023_info("id: %d, width: %d, height: %d\n",
                    id, supported_modes[id].width, supported_modes[id].height);
        gc4023_write_multiple_reg((struct gc4023_dev *)dev,
                                  supported_modes[id].reg_list,
                                  supported_modes[id].reg_list_size);
    }
}
#endif

static void gc4023_get_gain_reg(int *line, uint32_t *tol_dig_gain, uint32_t gain)
{
    int total;

    if (gain < 64)
        gain = 64;
    total = sizeof(gain_level_table) / sizeof(uint32_t) - 1;
    for (*line = 0; *line < total; (*line)++)
    {
        if (gain_level_table[*line] <= gain &&
                gain < gain_level_table[*line + 1])
            break;
    }

    *tol_dig_gain = (gain * 64 / gain_level_table[*line]);
    if (*line >= total)
        *line = total - 1;
}

static ret_err_t rk_gc4023_set_expval(struct gc4023_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time, gain, tol_dig_gain = 0;
    int line;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        gc4023_dbg("gc4023 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    gain = exp->reg_gain[0];

    if (l_exp_time < GC4023_EXP_MIN)
        l_exp_time = GC4023_EXP_MIN;

    gc4023_dbg("gc4023: exp req: L_exp: 0x%x, 0x%x\n",
               l_exp_time, gain);

    ret = gc4023_write_reg(dev->i2c_bus, GC4023_REG_EXPOSURE_H,
                           (uint8_t)(l_exp_time >> 8));
    ret |= gc4023_write_reg(dev->i2c_bus, GC4023_REG_EXPOSURE_L,
                            (uint8_t)(l_exp_time & 0xff));
    gc4023_get_gain_reg(&line, &tol_dig_gain, gain);

    gc4023_dbg("gain: %d, line: %d tol_dig_gain: %d\n",  gain, line, tol_dig_gain);

    ret |= gc4023_write_reg(dev->i2c_bus, 0x614,
                            reg_val_table_liner[line][0]);
    ret |= gc4023_write_reg(dev->i2c_bus, 0x615,
                            reg_val_table_liner[line][1]);
    ret |= gc4023_write_reg(dev->i2c_bus, 0x218,
                            reg_val_table_liner[line][2]);
    ret |= gc4023_write_reg(dev->i2c_bus, 0x1467,
                            reg_val_table_liner[line][3]);
    ret |= gc4023_write_reg(dev->i2c_bus, 0x1468,
                            reg_val_table_liner[line][4]);
    ret |= gc4023_write_reg(dev->i2c_bus, 0xb8,
                            reg_val_table_liner[line][5]);
    ret |= gc4023_write_reg(dev->i2c_bus, 0xb9,
                            reg_val_table_liner[line][6]);

    ret |= gc4023_write_reg(dev->i2c_bus, 0x64,
                            (tol_dig_gain >> 6));
    ret |= gc4023_write_reg(dev->i2c_bus, 0x65,
                            ((tol_dig_gain & 0x3f) << 2));

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_gc4023_set_vts(struct gc4023_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    gc4023_dbg("set vts: 0x%x \n", dst_vts);

    ret = gc4023_write_reg(dev->i2c_bus, GC4023_REG_VTS_L,
                           (uint8_t)(dst_vts & 0xff));
    ret |= gc4023_write_reg(dev->i2c_bus, GC4023_REG_VTS_H,
                            (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_gc4023_set_flip_mirror(struct gc4023_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0, val_1 = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = gc4023_read_reg(dev->i2c_bus, GC4023_FLIP_MIRROR_REG, &val);
    ret = gc4023_read_reg(dev->i2c_bus, GC4023_FLIP_MIRROR_REG_1, &val_1);
    switch (flip)
    {
    case 0 ... 3:
        val = GC4023_FETCH_MIRROR(val, flip & 0x01);
        val = GC4023_FETCH_FLIP(val, (flip >> 1) & 0x01);
        val_1 = GC4023_FETCH_MIRROR(val, flip & 0x01);
        val_1 = GC4023_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = GC4023_FETCH_MIRROR(val, false);
        val = GC4023_FETCH_FLIP(val, false);
        val_1 = GC4023_FETCH_MIRROR(val, false);
        val_1 = GC4023_FETCH_FLIP(val, false);
        break;
    };
    gc4023_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = gc4023_write_reg(dev->i2c_bus, GC4023_FLIP_MIRROR_REG, val);
    ret = gc4023_write_reg(dev->i2c_bus, GC4023_FLIP_MIRROR_REG_1, val_1);

    return ret;
}

static void gc4023_stream_on(struct gc4023_dev *dev)
{
    gc4023_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    gc4023_write_multiple_reg_continue((struct gc4023_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_gc4023_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_gc4023_set_flip_mirror(dev, dev->flip);
    gc4023_write_reg(dev->i2c_bus, GC4023_REG_CTRL_MODE, GC4023_MODE_STREAMING);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    gc4023_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void gc4023_stream_on_late(struct gc4023_dev *dev)
{
    gc4023_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_gc4023_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_gc4023_set_flip_mirror(dev, dev->flip);

    gc4023_write_reg(dev->i2c_bus, GC4023_REG_CTRL_MODE, GC4023_MODE_STREAMING);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    gc4023_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void gc4023_stream_off(struct gc4023_dev *dev)
{
    struct  rt_i2c_bus_device *i2c_bus;


    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        gc4023_write_reg(dev->i2c_bus, GC4023_REG_CTRL_MODE, GC4023_MODE_SW_STANDBY);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        gc4023_err("not find out i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_gc4023_init(struct rk_camera_device *dev)
{

    ret_err_t ret = RET_SYS_EOK;
    struct gc4023_dev *gc4023;

    gc4023 = (struct gc4023_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&gc4023->parent;
#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(gc4023);
#endif


    if (gc4023)
    {
        camera->info.mbus_fmt.width = gc4023->cur_mode->width;
        camera->info.mbus_fmt.height = gc4023->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10; // 0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = gc4023->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode = gc4023->cur_mode->hdr_mode;

        gc4023->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(gc4023->i2c_name);

        if (!gc4023->i2c_bus)
        {
            gc4023_dbg("not find i2c source 2:%s !!!\n", gc4023->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            gc4023_dbg("s0 find i2c_bus:%s\n", gc4023->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_gc4023_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_GC4023_close(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_gc4023_get_expinf(struct gc4023_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct gc4023_mode *mode;

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

static ret_err_t rk_gc4023_get_intput_fmt(struct gc4023_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct gc4023_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_gc4023_set_intput_fmt(struct gc4023_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct gc4023_mode *mode;
    int i = 0;
    bool is_find_fmt = false;
    struct camera_board_desc *gc4023 = (struct camera_board_desc *)&camera_gc4023;

    RT_ASSERT(dev != RT_NULL);

    gc4023_info("dst resulotion, width %d, height %d\n",
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
            gc4023_info("switch resulotion, width %d, height %d, fps %d\n",
                mode->width, mode->height, mode->max_fps.denominator / mode->max_fps.numerator);
            dev->cur_mode = mode;

            gc4023_dbg("reset by rst_gpio\n");
            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc4023->rst_gpio.gpio_group, gc4023->rst_gpio.gpio_pin, gc4023->rst_active);
            HAL_DelayMs(1);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc4023->rst_gpio.gpio_group, gc4023->rst_gpio.gpio_pin, !gc4023->rst_active);

#ifndef REG_LIST_CONVERSION_DEBUG
            gc4023_write_multiple_reg_continue((struct gc4023_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_gc4023_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static rt_err_t rk_gc4023_match_dst_config(struct gc4023_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct gc4023_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    gc4023_info("cur resulotion, width %d, height %d, fps %d!\n",
                dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);

    gc4023_info("dst resulotion, width %d, height %d, fps %d!\n",
                dst_config->width, dst_config->height, dst_fps);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct gc4023_mode *)mode;
            ret = RET_SYS_EOK;
            gc4023_info("find dst resulotion, width %d, height %d, fps %d!\n",
                        dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            gc4023_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        gc4023_err("dst fps %d is larger than cur fps %d\n", dst_fps, cur_fps);
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_gc4023_control(struct rk_camera_device *dev,
                            int cmd,
                            void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct gc4023_dev *GC4023;

    RT_ASSERT(dev != RT_NULL);
    GC4023 = (struct gc4023_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_gc4023_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        gc4023_stream_on(GC4023);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        gc4023_stream_off(GC4023);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_gc4023_get_expinf(GC4023, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_gc4023_set_expval(GC4023, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_gc4023_set_vts(GC4023, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_gc4023_get_intput_fmt(GC4023, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_gc4023_set_intput_fmt(GC4023, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_gc4023_match_dst_config(GC4023, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_gc4023_set_flip_mirror(GC4023, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        gc4023_stream_on_late(GC4023);
    }
    break;
    default:
        gc4023_dbg(" exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_gc4023_ops =
{
    rk_gc4023_init,
    rk_gc4023_open,
    NULL,
    rk_gc4023_control
};

int rk_camera_gc4023_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct gc4023_dev *instance = &g_gc4023;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *gc4023 = (struct camera_board_desc *)&camera_gc4023;

    camera->ops = &rk_gc4023_ops;

    rt_strncpy(instance->name, gc4023->isp_subdev_name, rt_strlen(gc4023->isp_subdev_name));
    rt_strncpy(instance->i2c_name, gc4023->i2c_bus, rt_strlen(gc4023->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (gc4023->mode_id <= GC4023_MODE_ID_MAX)
    {
        gc4023_dbg("mode_id: %d\n", gc4023->mode_id);
        instance->cur_mode = &supported_modes[gc4023->mode_id];
    }
    else
    {
        gc4023_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct gc4023_mode *)&supported_modes[GC4023_2560X1440];
    instance->time_valid_delay = gc4023->time_valid_delay;
    instance->gain_valid_delay = gc4023->gain_valid_delay;
    instance->i2c_addr = gc4023->i2c_addr;
    instance->flip = 0;

    RT_ASSERT(gc4023->rst_gpio.gpio_group != RT_NULL);
    RT_ASSERT(gc4023->pwdn_gpio.gpio_group != RT_NULL);

    HAL_GPIO_SetPinDirection((struct GPIO_REG *)gc4023->pwdn_gpio.gpio_group, gc4023->pwdn_gpio.gpio_pin, GPIO_OUT);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc4023->rst_gpio.gpio_group, gc4023->rst_gpio.gpio_pin, gc4023->rst_active);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc4023->pwdn_gpio.gpio_group, gc4023->pwdn_gpio.gpio_pin, gc4023->pwdn_active);
    HAL_DelayMs(1);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc4023->rst_gpio.gpio_group, gc4023->rst_gpio.gpio_pin, !gc4023->rst_active);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc4023->pwdn_gpio.gpio_group, gc4023->pwdn_gpio.gpio_pin, !gc4023->pwdn_active);

    if (gc4023->mclk_out_gate_id && gc4023->mclk_id)
    {
        clk_set_rate(gc4023->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(gc4023->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "GC4023_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, gc4023->isp_subdev_name, rt_strlen(gc4023->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void gc4023_detect(void)
{
    struct gc4023_dev *instance = &g_gc4023;

    gc4023_dbg("start to detect GC4023 for testing \n");
    gc4023_dbg("dev name:%s\n", instance->name);
    gc4023_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        gc4023_dbg("not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        gc4023_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_gc4023_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(gc4023_detect, check GC4023 is available or not);
#endif
#endif
#endif
