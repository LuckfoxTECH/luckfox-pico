/**
 * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 * @file    drv_gc3003.c
 * @version V0.0.1
 *
 * Change Logs:
 * 2022-07-07     ISP Team      first implementation
 * 2022-12-08     chujin.zhou bsp: rockchip: camera: gc3003 support get config from boardcfg
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

#ifdef RT_USING_GC3003

#define GC3003_DEBUG_PRT 0

#if GC3003_DEBUG_PRT
#include <stdio.h>
#define gc3003_dbg(fmt, args...)    rt_kprintf("[GC3003] " fmt "", ##args)
#else
#define gc3003_dbg(fmt, args...)
#endif

#define gc3003_info(fmt, args...)         rt_kprintf("[GC3003] "fmt"", ##args)
#define gc3003_err(fmt, args...)         rt_kprintf("[GC3003] ERR "fmt"", ##args)

#define GC3003_REG_CTRL_MODE    0x023e
#define GC3003_MODE_SW_STANDBY  0x00
#define GC3003_MODE_STREAMING   0x99

#define GC3003_REG_EXPOSURE_H 0x0d03
#define GC3003_REG_EXPOSURE_L 0x0d04
#define GC3003_EXPOSURE_MIN 4
#define GC3003_EXPOSURE_STEP 1
#define GC3003_EXP_MIN 2

#define GC3003_GAIN_MIN 0x0040
#define GC3003_GAIN_MAX (0xffff)
#define GC3003_GAIN_STEP 1
#define GC3003_GAIN_DEFAULT 0x0040

#define GC3003_FLIP_MIRROR_REG 0x0015
#define GC3003_FLIP_MIRROR_REG_1 0x0d15 // todo

#define GC3003_REG_VTS_H 0x0d41
#define GC3003_REG_VTS_L 0x0d42

#define GC3003_FETCH_MIRROR(VAL, ENABLE) (ENABLE ? VAL | 0x01 : VAL & 0xfe)
#define GC3003_FETCH_FLIP(VAL, ENABLE) (ENABLE ? VAL | 0x02 : VAL & 0xfd)

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

struct gc3003_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct gc3003_mode
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
    const struct gc3003_sensor_reg *reg_list;
    const struct gc3003_sensor_reg *stream_on_reg_list;
#endif
    const int reg_list_size;
    const int stream_on_reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct gc3003_dev
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
    const struct gc3003_mode *cur_mode;
    struct gc3003_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct gc3003_dev *rt_gc3003_dev_t;

static struct gc3003_dev g_gc3003;

static const uint32_t reg_val_table_liner[25][6] =
{
    {0x00, 0x00, 0x05, 0x01, 0x01, 0x00},
    {0x0a, 0x00, 0x06, 0x01, 0x01, 0x0c},
    {0x00, 0x01, 0x06, 0x01, 0x01, 0x1a},
    {0x0a, 0x01, 0x08, 0x01, 0x01, 0x2a},
    {0x20, 0x00, 0x0a, 0x02, 0x02, 0x00},
    {0x25, 0x00, 0x0a, 0x03, 0x02, 0x18},
    {0x20, 0x01, 0x0a, 0x04, 0x02, 0x33},
    {0x25, 0x01, 0x0b, 0x05, 0x03, 0x14},
    {0x30, 0x00, 0x0b, 0x06, 0x04, 0x00},
    {0x32, 0x80, 0x0c, 0x09, 0x04, 0x2f},
    {0x30, 0x01, 0x0c, 0x0c, 0x05, 0x26},
    {0x32, 0x81, 0x0d, 0x0e, 0x06, 0x29},
    {0x38, 0x00, 0x0e, 0x10, 0x08, 0x00},
    {0x39, 0x40, 0x10, 0x12, 0x09, 0x1f},
    {0x38, 0x01, 0x12, 0x12, 0x0b, 0x0d},
    {0x39, 0x41, 0x14, 0x14, 0x0d, 0x12},
    {0x30, 0x08, 0x15, 0x16, 0x10, 0x00},
    {0x32, 0x88, 0x18, 0x1a, 0x12, 0x3e},
    {0x30, 0x09, 0x1a, 0x1d, 0x16, 0x1a},
    {0x32, 0x89, 0x1c, 0x22, 0x1a, 0x23},
    {0x38, 0x08, 0x1e, 0x26, 0x20, 0x00},
    {0x39, 0x48, 0x20, 0x2d, 0x25, 0x3b},
    {0x38, 0x09, 0x22, 0x32, 0x2c, 0x33},
    {0x39, 0x49, 0x24, 0x3a, 0x35, 0x06},
    {0x38, 0x0a, 0x26, 0x42, 0x3f, 0x3f},
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
    716,
    848,
    1024,
    1214,
    1434,
    1699,
    2048,
    2427,
    2865,
    3393,
    4096,
    0xffffffff,
};

#ifndef REG_LIST_CONVERSION_DEBUG

static const uint8_t g_gc3003_320x240_120fps_reg_table[] =
{
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0x00,
    0x3, 0x03, 0xf3, 0x00,
    0x8, 0x03, 0xf5, 0xc0, 0x06, 0x01, 0x46, 0x13, 0x00,
    0x5, 0x03, 0xe0, 0x16, 0x0d, 0x30,
    0x3, 0x03, 0xe4, 0x08,
    0x3, 0x03, 0xfc, 0xce,
    0x4, 0x0d, 0x05, 0x05, 0xdc,
    0x3, 0x0d, 0x76, 0x00,
    0x4, 0x0d, 0x41, 0x01, 0x2c,
    0x3, 0x0d, 0x0a, 0x02,
    0x3, 0x00, 0x0c, 0x02,
    0x4, 0x0d, 0x0d, 0x00, 0xf8,
    0x4, 0x00, 0x0f, 0x01, 0x48,
    0x3, 0x00, 0x17, 0x0c,
    0x3, 0x0d, 0x53, 0x12,
    0x3, 0x00, 0x51, 0x03,
    0x3, 0x00, 0x82, 0x01,
    0x4, 0x00, 0x8c, 0x05, 0xd0,
    0x3, 0x0d, 0xb7, 0x01,
    0x9, 0x0d, 0xb0, 0xad, 0x00, 0x8c, 0xf4, 0x00, 0x97, 0x08,
    0x3, 0x0d, 0x25, 0xcb,
    0x3, 0x0d, 0x4a, 0x04,
    0x3, 0x00, 0xd2, 0x70,
    0x3, 0x00, 0xd7, 0x19,
    0x4, 0x00, 0xd9, 0x1c, 0xc1,
    0x3, 0x0d, 0x55, 0x1b,
    0x3, 0x0d, 0x92, 0x17,
    0x3, 0x0d, 0xc2, 0x30,
    0x3, 0x0d, 0x2a, 0x30,
    0x3, 0x0d, 0x19, 0x51,
    0x3, 0x0d, 0x29, 0x30,
    0x3, 0x0d, 0x20, 0x30,
    0x3, 0x0d, 0x72, 0x12,
    0x3, 0x0d, 0x4e, 0x12,
    0x3, 0x0d, 0x43, 0x20,
    0x3, 0x00, 0x50, 0x0c,
    0x3, 0x00, 0x6e, 0x03,
    0x3, 0x01, 0x53, 0x50,
    0x3, 0x01, 0x92, 0x04,
    0x7, 0x01, 0x94, 0x04, 0x00, 0xf0, 0x01, 0x40,
    0x5, 0x00, 0x77, 0x01, 0x04, 0x65,
    0x3, 0x00, 0x67, 0xc0,
    0x6, 0x00, 0x54, 0xff, 0x02, 0x00, 0x04,
    0x4, 0x00, 0x5a, 0xff, 0x07,
    0x3, 0x00, 0xd5, 0x03,
    0x3, 0x01, 0x02, 0x10,
    0x3, 0x0d, 0x4a, 0x04,
    0x3, 0x04, 0xe0, 0xff,
    0x3, 0x03, 0x1e, 0x3e,
    0x3, 0x01, 0x59, 0x01,
    0x4, 0x01, 0x4f, 0x28, 0x40,
    0x3, 0x00, 0x26, 0x00,
    0x3, 0x0d, 0x26, 0xa0,
    0x6, 0x04, 0x14, 0x76, 0x75, 0x75, 0x76,
    0x3, 0x01, 0x55, 0x01,
    0x6, 0x01, 0x70, 0x3f, 0x3f, 0x3f, 0x3f,
    0x1a, 0x04, 0x28, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x05, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x18, 0x18, 0x18, 0x18, 0x1d, 0x20, 0x22, 0x24,
    0xa, 0x04, 0x68, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x4, 0x01, 0x08, 0xf0, 0x80,
    0x4, 0x0d, 0x03, 0x01, 0x00,
    0x3, 0x00, 0x7a, 0x60,
    0x3, 0x00, 0xd0, 0x00,
    0x3, 0x00, 0x80, 0x05,
    0x4, 0x02, 0x91, 0x0f, 0xff,
    0x4, 0x02, 0x02, 0x53, 0x4e,
    0x3, 0x02, 0x06, 0x03,
    0x4, 0x02, 0x12, 0x0b, 0x40,
    0x3, 0x02, 0x15, 0x10,
    0x3, 0x02, 0x3e, 0x99,
    0x3, 0x03, 0xfe, 0x10,
    0x3, 0x01, 0x83, 0x09,
    0x3, 0x01, 0x87, 0x51,
    0x3, 0x0d, 0x22, 0x01,
    0x3, 0x0d, 0x21, 0x2c,
    0x4, 0x0d, 0x03, 0x01, 0x00,
    0x3, 0x0d, 0x23, 0x0e,
    0x3, 0x03, 0xfe, 0x00,
    0x50,
};

static const uint8_t g_gc3003_2304x1296_30fps_reg_table[] =
{
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0x00,
    0x3, 0x03, 0xf3, 0x00,
    0x8, 0x03, 0xf5, 0xc0, 0x06, 0x01, 0x46, 0x13, 0x00,
    0x5, 0x03, 0xe0, 0x16, 0x0d, 0x30,
    0x3, 0x03, 0xe4, 0x08,
    0x3, 0x03, 0xfc, 0xce,
    0x4, 0x0d, 0x05, 0x05, 0x40,
    0x3, 0x0d, 0x76, 0x00,
    0x4, 0x0d, 0x41, 0x05, 0x3c,
    0x3, 0x0d, 0x0a, 0x02,
    0x3, 0x00, 0x0c, 0x02,
    0x4, 0x0d, 0x0d, 0x05, 0x18,
    0x4, 0x00, 0x0f, 0x09, 0x08,
    0x3, 0x00, 0x17, 0x0c,
    0x3, 0x0d, 0x53, 0x12,
    0x3, 0x00, 0x51, 0x03,
    0x3, 0x00, 0x82, 0x01,
    0x4, 0x00, 0x8c, 0x05, 0xd0,
    0x3, 0x0d, 0xb7, 0x01,
    0x9, 0x0d, 0xb0, 0xad, 0x00, 0x8c, 0xf4, 0x00, 0x97, 0x08,
    0x3, 0x0d, 0x25, 0xcb,
    0x3, 0x0d, 0x4a, 0x04,
    0x3, 0x00, 0xd2, 0x70,
    0x3, 0x00, 0xd7, 0x19,
    0x4, 0x00, 0xd9, 0x1c, 0xc1,
    0x3, 0x0d, 0x55, 0x1b,
    0x3, 0x0d, 0x92, 0x17,
    0x3, 0x0d, 0xc2, 0x30,
    0x3, 0x0d, 0x2a, 0x30,
    0x3, 0x0d, 0x19, 0x51,
    0x3, 0x0d, 0x29, 0x30,
    0x3, 0x0d, 0x20, 0x30,
    0x3, 0x0d, 0x72, 0x12,
    0x3, 0x0d, 0x4e, 0x12,
    0x3, 0x0d, 0x43, 0x20,
    0x3, 0x00, 0x50, 0x0c,
    0x3, 0x00, 0x6e, 0x03,
    0x3, 0x01, 0x53, 0x50,
    0x3, 0x01, 0x92, 0x04,
    0x7, 0x01, 0x94, 0x04, 0x05, 0x10, 0x09, 0x00,
    0x5, 0x00, 0x77, 0x01, 0x04, 0x65,
    0x3, 0x00, 0x67, 0xc0,
    0x6, 0x00, 0x54, 0xff, 0x02, 0x00, 0x04,
    0x4, 0x00, 0x5a, 0xff, 0x07,
    0x3, 0x00, 0xd5, 0x03,
    0x3, 0x01, 0x02, 0x10,
    0x3, 0x0d, 0x4a, 0x04,
    0x3, 0x04, 0xe0, 0xff,
    0x3, 0x03, 0x1e, 0x3e,
    0x3, 0x01, 0x59, 0x01,
    0x4, 0x01, 0x4f, 0x28, 0x40,
    0x3, 0x00, 0x26, 0x00,
    0x3, 0x0d, 0x26, 0xa0,
    0x6, 0x04, 0x14, 0x76, 0x75, 0x75, 0x76,
    0x3, 0x01, 0x55, 0x01,
    0x6, 0x01, 0x70, 0x3f, 0x3f, 0x3f, 0x3f,
    0x1a, 0x04, 0x28, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x05, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x18, 0x18, 0x18, 0x18, 0x1d, 0x20, 0x22, 0x24,
    0xa, 0x04, 0x68, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x4, 0x01, 0x08, 0xf0, 0x80,
    0x4, 0x0d, 0x03, 0x05, 0x00,
    0x3, 0x00, 0x7a, 0x60,
    0x3, 0x00, 0xd0, 0x00,
    0x3, 0x00, 0x80, 0x05,
    0x4, 0x02, 0x91, 0x0f, 0xff,
    0x5, 0x02, 0x01, 0x27, 0x53, 0x4e,
    0x3, 0x02, 0x06, 0x03,
    0x4, 0x02, 0x12, 0x0b, 0x40,
    0x3, 0x02, 0x15, 0x12,
    0x3, 0x03, 0xfe, 0x10,
    0x3, 0x01, 0x83, 0x09,
    0x3, 0x01, 0x87, 0x51,
    0x3, 0x0d, 0x22, 0x04,
    0x3, 0x0d, 0x21, 0x3c,
    0x4, 0x0d, 0x03, 0x01, 0x28,
    0x3, 0x0d, 0x23, 0x0e,
    0x3, 0x03, 0xfe, 0x00,
    0x4f,
};

static const uint8_t g_gc3003_1920x528_70fps_reg_table[] =
{
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0xf0,
    0x3, 0x03, 0xfe, 0x00,
    0x3, 0x03, 0xf3, 0x00,
    0x8, 0x03, 0xf5, 0xc0, 0x06, 0x01, 0x46, 0x13, 0x00,
    0x5, 0x03, 0xe0, 0x16, 0x0d, 0x30,
    0x3, 0x03, 0xe4, 0x08,
    0x3, 0x03, 0xfc, 0xce,
    0x4, 0x0d, 0x05, 0x05, 0x40,
    0x3, 0x0d, 0x76, 0x00,
    0x4, 0x0d, 0x41, 0x02, 0x3c,
    0x4, 0x0d, 0x09, 0x01, 0x7a,
    0x3, 0x00, 0x0c, 0x02,
    0x4, 0x0d, 0x0d, 0x02, 0x10,
    0x4, 0x00, 0x0f, 0x09, 0x08,
    0x3, 0x00, 0x17, 0x0c,
    0x3, 0x0d, 0x53, 0x12,
    0x3, 0x00, 0x51, 0x03,
    0x3, 0x00, 0x82, 0x01,
    0x3, 0x00, 0x86, 0x20,
    0x6, 0x00, 0x8a, 0x01, 0x1d, 0x05, 0xd0,
    0x3, 0x0d, 0xb7, 0x01,
    0x9, 0x0d, 0xb0, 0x05, 0x00, 0x04, 0x54, 0x00, 0x17, 0x08,
    0x3, 0x0d, 0x25, 0xcb,
    0x3, 0x0d, 0x4a, 0x04,
    0x3, 0x00, 0xd2, 0x70,
    0x3, 0x00, 0xd7, 0x19,
    0x4, 0x00, 0xd9, 0x10, 0xc1,
    0x3, 0x0d, 0x55, 0x1b,
    0x3, 0x0d, 0x92, 0x17,
    0x3, 0x0d, 0xc2, 0x30,
    0x3, 0x0d, 0x2a, 0x30,
    0x3, 0x0d, 0x19, 0x51,
    0x3, 0x0d, 0x29, 0x30,
    0x3, 0x0d, 0x20, 0x30,
    0x3, 0x0d, 0x72, 0x12,
    0x3, 0x0d, 0x4e, 0x12,
    0x3, 0x0d, 0x43, 0x20,
    0x3, 0x00, 0x50, 0x0c,
    0x3, 0x00, 0x6e, 0x03,
    0x3, 0x01, 0x53, 0x50,
    0x9, 0x01, 0x92, 0x00, 0x00, 0xc0, 0x02, 0x1c, 0x07, 0x80,
    0x5, 0x00, 0x77, 0x01, 0x65, 0x04,
    0x3, 0x00, 0x67, 0xc0,
    0x6, 0x00, 0x54, 0xff, 0x02, 0x00, 0x04,
    0x4, 0x00, 0x5a, 0xff, 0x07,
    0x3, 0x00, 0xd5, 0x03,
    0x3, 0x01, 0x02, 0x10,
    0x3, 0x0d, 0x4a, 0x04,
    0x3, 0x04, 0xe0, 0xff,
    0x3, 0x03, 0x1e, 0x3e,
    0x3, 0x01, 0x59, 0x01,
    0x4, 0x01, 0x4f, 0x28, 0x40,
    0x3, 0x00, 0x26, 0x00,
    0x3, 0x0d, 0x26, 0xa0,
    0x6, 0x04, 0x14, 0x77, 0x77, 0x77, 0x77,
    0x3, 0x01, 0x55, 0x00,
    0x6, 0x01, 0x70, 0x3e, 0x3e, 0x3e, 0x3e,
    0x1a, 0x04, 0x28, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x05, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x18, 0x18, 0x18, 0x18, 0x1d, 0x20, 0x22, 0x24,
    0xa, 0x04, 0x68, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x4, 0x01, 0x08, 0xf0, 0x80,
    0x4, 0x0d, 0x03, 0x05, 0x00,
    0x3, 0x00, 0x7a, 0x60,
    0x3, 0x00, 0xd0, 0x00,
    0x3, 0x00, 0x80, 0x09,
    0x4, 0x02, 0x91, 0x0f, 0xff,
    0x5, 0x02, 0x01, 0x27, 0x53, 0x4e,
    0x3, 0x02, 0x06, 0x03,
    0x4, 0x02, 0x12, 0x0b, 0x40,
    0x3, 0x02, 0x15, 0x12,
    0x3, 0x03, 0xfe, 0x10,
    0x3, 0x01, 0x83, 0x09,
    0x3, 0x01, 0x87, 0x51,
    0x3, 0x0d, 0x22, 0x01,
    0x3, 0x0d, 0x21, 0x2c,
    0x4, 0x0d, 0x03, 0x01, 0x00,
    0x3, 0x0d, 0x23, 0x0e,
    0x3, 0x03, 0xfe, 0x00,
    0x4f,
};

static const uint8_t g_gc3003_stream_on_reg_table[] =
{
    0x5, 0x02, 0x01, 0x27, 0x53, 0x4e,
    0x3, 0x02, 0x06, 0x03,
    0x4, 0x02, 0x12, 0x0b, 0x40,
    0x3, 0x02, 0x15, 0x10,
    0x3, 0x02, 0x3e, 0x99,
    0x3, 0x03, 0xfe, 0x00,
    0x6,
};

#else
/*
 * Xclk 27Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 630Mbps, 2lane
 * binning to 320x240 then centered crop
 */
static const struct gc3003_sensor_reg g_gc3003_320x240_120fps_reg_table[] =
{
    {0x03fe, 0xf0},
    {0x03fe, 0xf0},
    {0x03fe, 0xf0},
    {0x03fe, 0x00},
    {0x03f3, 0x00},
    {0x03f5, 0xc0},
    {0x03f6, 0x06},
    {0x03f7, 0x01},
    {0x03f8, 0x46},
    {0x03f9, 0x13},
    {0x03fa, 0x00},
    {0x03e0, 0x16},
    {0x03e1, 0x0d},
    {0x03e2, 0x30},
    {0x03e4, 0x08},
    {0x03fc, 0xce},
    {0x0d05, 0x05},
    {0x0d06, 0xdc},
    {0x0d76, 0x00},
    {0x0d41, 0x01},
    {0x0d42, 0x2c},
    {0x0d0a, 0x02},
    {0x000c, 0x02},
    {0x0d0d, 0x00},
    {0x0d0e, 0xf8},
    {0x000f, 0x01},
    {0x0010, 0x48},
    {0x0017, 0x0c},
    {0x0d53, 0x12},
    {0x0051, 0x03},
    {0x0082, 0x01},
    {0x008c, 0x05},
    {0x008d, 0xd0},
    {0x0db7, 0x01},
    {0x0db0, 0xad},
    {0x0db1, 0x00},
    {0x0db2, 0x8c},
    {0x0db3, 0xf4},
    {0x0db4, 0x00},
    {0x0db5, 0x97},
    {0x0db6, 0x08},
    {0x0d25, 0xcb},
    {0x0d4a, 0x04},
    {0x00d2, 0x70},
    {0x00d7, 0x19},
    {0x00d9, 0x1c},
    {0x00da, 0xc1},
    {0x0d55, 0x1b},
    {0x0d92, 0x17},
    {0x0dc2, 0x30},
    {0x0d2a, 0x30},
    {0x0d19, 0x51},
    {0x0d29, 0x30},
    {0x0d20, 0x30},
    {0x0d72, 0x12},
    {0x0d4e, 0x12},
    {0x0d43, 0x20},
    {0x0050, 0x0c},
    {0x006e, 0x03},
    {0x0153, 0x50},
    {0x0192, 0x04},
    {0x0194, 0x04},
    {0x0195, 0x00},
    {0x0196, 0xf0},
    {0x0197, 0x01},
    {0x0198, 0x40},
    {0x0077, 0x01},
    {0x0078, 0x04},
    {0x0079, 0x65},
    {0x0067, 0xc0},
    {0x0054, 0xff},
    {0x0055, 0x02},
    {0x0056, 0x00},
    {0x0057, 0x04},
    {0x005a, 0xff},
    {0x005b, 0x07},
    {0x00d5, 0x03},
    {0x0102, 0x10},
    {0x0d4a, 0x04},
    {0x04e0, 0xff},
    {0x031e, 0x3e},
    {0x0159, 0x01},
    {0x014f, 0x28},
    {0x0150, 0x40},
    {0x0026, 0x00},
    {0x0d26, 0xa0},
    {0x0414, 0x76},
    {0x0415, 0x75},
    {0x0416, 0x75},
    {0x0417, 0x76},
    {0x0155, 0x01},
    {0x0170, 0x3f},
    {0x0171, 0x3f},
    {0x0172, 0x3f},
    {0x0173, 0x3f},
    {0x0428, 0x0b},
    {0x0429, 0x0b},
    {0x042a, 0x0b},
    {0x042b, 0x0b},
    {0x042c, 0x0b},
    {0x042d, 0x0b},
    {0x042e, 0x0b},
    {0x042f, 0x0b},
    {0x0430, 0x05},
    {0x0431, 0x05},
    {0x0432, 0x05},
    {0x0433, 0x05},
    {0x0434, 0x04},
    {0x0435, 0x04},
    {0x0436, 0x04},
    {0x0437, 0x04},
    {0x0438, 0x18},
    {0x0439, 0x18},
    {0x043a, 0x18},
    {0x043b, 0x18},
    {0x043c, 0x1d},
    {0x043d, 0x20},
    {0x043e, 0x22},
    {0x043f, 0x24},
    {0x0468, 0x04},
    {0x0469, 0x04},
    {0x046a, 0x04},
    {0x046b, 0x04},
    {0x046c, 0x04},
    {0x046d, 0x04},
    {0x046e, 0x04},
    {0x046f, 0x04},
    {0x0108, 0xf0},
    {0x0109, 0x80},
    {0x0d03, 0x01},
    {0x0d04, 0x00},
    {0x007a, 0x60},
    {0x00d0, 0x00},
    {0x0080, 0x05},
    {0x0291, 0x0f},
    {0x0292, 0xff},
    {0x0202, 0x53},
    {0x0203, 0x4e},
    {0x0206, 0x03},
    {0x0212, 0x0b},
    {0x0213, 0x40},
    {0x0215, 0x12},
    {0x023e, 0x99},
    {0x03fe, 0x10},
    {0x0183, 0x09},
    {0x0187, 0x51},
    {0x0d22, 0x01},
    {0x0d21, 0x2c},
    {0x0d03, 0x01},
    {0x0d04, 0x00},
    {0x0d23, 0x0e},
    {0x03fe, 0x00},
    {REG_END, 0x00},
};

/*
 * Xclk 27Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 630Mbps, 2lane
 * full resolution
 */
static const struct gc3003_sensor_reg g_gc3003_2304x1296_30fps_reg_table[] =
{
    {0x03fe, 0xf0},
    {0x03fe, 0xf0},
    {0x03fe, 0xf0},
    {0x03fe, 0x00},
    {0x03f3, 0x00},
    {0x03f5, 0xc0},
    {0x03f6, 0x06},
    {0x03f7, 0x01},
    {0x03f8, 0x46},
    {0x03f9, 0x13},
    {0x03fa, 0x00},
    {0x03e0, 0x16},
    {0x03e1, 0x0d},
    {0x03e2, 0x30},
    {0x03e4, 0x08},
    {0x03fc, 0xce},
    {0x0d05, 0x05},
    {0x0d06, 0x40},
    {0x0d76, 0x00},
    {0x0d41, 0x05},
    {0x0d42, 0x3c},
    {0x0d0a, 0x02},
    {0x000c, 0x02},
    {0x0d0d, 0x05},
    {0x0d0e, 0x18},
    {0x000f, 0x09},
    {0x0010, 0x08},
    {0x0017, 0x0c},
    {0x0d53, 0x12},
    {0x0051, 0x03},
    {0x0082, 0x01},
    {0x008c, 0x05},
    {0x008d, 0xd0},
    {0x0db7, 0x01},
    {0x0db0, 0xad},
    {0x0db1, 0x00},
    {0x0db2, 0x8c},
    {0x0db3, 0xf4},
    {0x0db4, 0x00},
    {0x0db5, 0x97},
    {0x0db6, 0x08},
    {0x0d25, 0xcb},
    {0x0d4a, 0x04},
    {0x00d2, 0x70},
    {0x00d7, 0x19},
    {0x00d9, 0x1c},
    {0x00da, 0xc1},
    {0x0d55, 0x1b},
    {0x0d92, 0x17},
    {0x0dc2, 0x30},
    {0x0d2a, 0x30},
    {0x0d19, 0x51},
    {0x0d29, 0x30},
    {0x0d20, 0x30},
    {0x0d72, 0x12},
    {0x0d4e, 0x12},
    {0x0d43, 0x20},
    {0x0050, 0x0c},
    {0x006e, 0x03},
    {0x0153, 0x50},
    {0x0192, 0x04},
    {0x0194, 0x04},
    {0x0195, 0x05},
    {0x0196, 0x10},
    {0x0197, 0x09},
    {0x0198, 0x00},
    {0x0077, 0x01},
    {0x0078, 0x04},
    {0x0079, 0x65},
    {0x0067, 0xc0},
    {0x0054, 0xff},
    {0x0055, 0x02},
    {0x0056, 0x00},
    {0x0057, 0x04},
    {0x005a, 0xff},
    {0x005b, 0x07},
    {0x00d5, 0x03},
    {0x0102, 0x10},
    {0x0d4a, 0x04},
    {0x04e0, 0xff},
    {0x031e, 0x3e},
    {0x0159, 0x01},
    {0x014f, 0x28},
    {0x0150, 0x40},
    {0x0026, 0x00},
    {0x0d26, 0xa0},
    {0x0414, 0x76},
    {0x0415, 0x75},
    {0x0416, 0x75},
    {0x0417, 0x76},
    {0x0155, 0x01},
    {0x0170, 0x3f},
    {0x0171, 0x3f},
    {0x0172, 0x3f},
    {0x0173, 0x3f},
    {0x0428, 0x0b},
    {0x0429, 0x0b},
    {0x042a, 0x0b},
    {0x042b, 0x0b},
    {0x042c, 0x0b},
    {0x042d, 0x0b},
    {0x042e, 0x0b},
    {0x042f, 0x0b},
    {0x0430, 0x05},
    {0x0431, 0x05},
    {0x0432, 0x05},
    {0x0433, 0x05},
    {0x0434, 0x04},
    {0x0435, 0x04},
    {0x0436, 0x04},
    {0x0437, 0x04},
    {0x0438, 0x18},
    {0x0439, 0x18},
    {0x043a, 0x18},
    {0x043b, 0x18},
    {0x043c, 0x1d},
    {0x043d, 0x20},
    {0x043e, 0x22},
    {0x043f, 0x24},
    {0x0468, 0x04},
    {0x0469, 0x04},
    {0x046a, 0x04},
    {0x046b, 0x04},
    {0x046c, 0x04},
    {0x046d, 0x04},
    {0x046e, 0x04},
    {0x046f, 0x04},
    {0x0108, 0xf0},
    {0x0109, 0x80},
    {0x0d03, 0x05},
    {0x0d04, 0x00},
    {0x007a, 0x60},
    {0x00d0, 0x00},
    {0x0080, 0x05},
    {0x0291, 0x0f},
    {0x0292, 0xff},
    {0x0201, 0x27},
    {0x0202, 0x53},
    {0x0203, 0x4e},
    {0x0206, 0x03},
    {0x0212, 0x0b},
    {0x0213, 0x40},
    {0x0215, 0x12},
    {0x03fe, 0x10},
    {0x0183, 0x09},
    {0x0187, 0x51},
    {0x0d22, 0x04},
    {0x0d21, 0x3C},
    {0x0d03, 0x01},
    {0x0d04, 0x28},
    {0x0d23, 0x0e},
    {0x03fe, 0x00},
    {REG_END, 0x00},
};

/*
 * Xclk 27Mhz
 * max_framerate 70fps
 * mipi_datarate per lane 630Mbps, 2lane
 * binning to 1920x528 then centered crop
 */
static const struct gc3003_sensor_reg g_gc3003_1920x528_70fps_reg_table[] =
{
    {0x03fe, 0xf0},
    {0x03fe, 0xf0},
    {0x03fe, 0xf0},
    {0x03fe, 0x00},
    {0x03f3, 0x00},
    {0x03f5, 0xc0},
    {0x03f6, 0x06},
    {0x03f7, 0x01},
    {0x03f8, 0x46},
    {0x03f9, 0x13},
    {0x03fa, 0x00},
    {0x03e0, 0x16},
    {0x03e1, 0x0d},
    {0x03e2, 0x30},
    {0x03e4, 0x08},
    {0x03fc, 0xce},
    {0x0d05, 0x05},
    {0x0d06, 0x40},
    {0x0d76, 0x00},
    {0x0d41, 0x02},
    {0x0d42, 0x3c},
    {0x0d09, 0x01},
    {0x0d0a, 0x7a},
    {0x000c, 0x02},
    {0x0d0d, 0x02},
    {0x0d0e, 0x20}, // 528
    {0x000f, 0x09},
    {0x0010, 0x08},
    {0x0017, 0x0c},
    {0x0d53, 0x12},
    {0x0051, 0x03},
    {0x0082, 0x01},
    {0x0086, 0x20},
    {0x008a, 0x01},
    {0x008b, 0x1d},
    {0x008c, 0x05},
    {0x008d, 0xd0},
    {0x0db7, 0x01},
    {0x0db0, 0x05},
    {0x0db1, 0x00},
    {0x0db2, 0x04},
    {0x0db3, 0x54},
    {0x0db4, 0x00},
    {0x0db5, 0x17},
    {0x0db6, 0x08},
    {0x0d25, 0xcb},
    {0x0d4a, 0x04},
    {0x00d2, 0x70},
    {0x00d7, 0x19},
    {0x00d9, 0x10},
    {0x00da, 0xc1},
    {0x0d55, 0x1b},
    {0x0d92, 0x17},
    {0x0dc2, 0x30},
    {0x0d2a, 0x30},
    {0x0d19, 0x51},
    {0x0d29, 0x30},
    {0x0d20, 0x30},
    {0x0d72, 0x12},
    {0x0d4e, 0x12},
    {0x0d43, 0x20},
    {0x0050, 0x0c},
    {0x006e, 0x03},
    {0x0153, 0x50},
    {0x0192, 0x00},
    {0x0193, 0x00},
    {0x0194, 0xc0},
    {0x0195, 0x02},
    {0x0196, 0x1c},
    {0x0197, 0x07},
    {0x0198, 0x80},
    {0x0077, 0x01},
    {0x0078, 0x65},
    {0x0079, 0x04},
    {0x0067, 0xc0},
    {0x0054, 0xff},
    {0x0055, 0x02},
    {0x0056, 0x00},
    {0x0057, 0x04},
    {0x005a, 0xff},
    {0x005b, 0x07},
    {0x00d5, 0x03},
    {0x0102, 0x10},
    {0x0d4a, 0x04},
    {0x04e0, 0xff},
    {0x031e, 0x3e},
    {0x0159, 0x01},
    {0x014f, 0x28},
    {0x0150, 0x40},
    {0x0026, 0x00},
    {0x0d26, 0xa0},
    {0x0414, 0x77},
    {0x0415, 0x77},
    {0x0416, 0x77},
    {0x0417, 0x77},
    {0x0155, 0x00},
    {0x0170, 0x3e},
    {0x0171, 0x3e},
    {0x0172, 0x3e},
    {0x0173, 0x3e},
    {0x0428, 0x0b},
    {0x0429, 0x0b},
    {0x042a, 0x0b},
    {0x042b, 0x0b},
    {0x042c, 0x0b},
    {0x042d, 0x0b},
    {0x042e, 0x0b},
    {0x042f, 0x0b},
    {0x0430, 0x05},
    {0x0431, 0x05},
    {0x0432, 0x05},
    {0x0433, 0x05},
    {0x0434, 0x04},
    {0x0435, 0x04},
    {0x0436, 0x04},
    {0x0437, 0x04},
    {0x0438, 0x18},
    {0x0439, 0x18},
    {0x043a, 0x18},
    {0x043b, 0x18},
    {0x043c, 0x1d},
    {0x043d, 0x20},
    {0x043e, 0x22},
    {0x043f, 0x24},
    {0x0468, 0x04},
    {0x0469, 0x04},
    {0x046a, 0x04},
    {0x046b, 0x04},
    {0x046c, 0x04},
    {0x046d, 0x04},
    {0x046e, 0x04},
    {0x046f, 0x04},
    {0x0108, 0xf0},
    {0x0109, 0x80},
    {0x0d03, 0x05},
    {0x0d04, 0x00},
    {0x007a, 0x60},
    {0x00d0, 0x00},
    {0x0080, 0x09},
    {0x0291, 0x0f},
    {0x0292, 0xff},
    {0x0201, 0x27},
    {0x0202, 0x53},
    {0x0203, 0x4e},
    {0x0206, 0x03},
    {0x0212, 0x0b},
    {0x0213, 0x40},
    {0x0215, 0x12},
    {0x023e, 0x99},
    {0x03fe, 0x10},
    {0x0183, 0x09},
    {0x0187, 0x51},
    {0x0d22, 0x01},
    {0x0d21, 0x2c},
    {0x0d03, 0x01},
    {0x0d04, 0x00},
    {0x0d23, 0x0e},
    {0x03fe, 0x00},
    {REG_END, 0x00},
};

static const struct gc3003_sensor_reg g_gc3003_stream_on_reg_table[] =
{
    {0x0201, 0x27},
    {0x0202, 0x53},
    {0x0203, 0x4e},
    {0x0206, 0x03},
    {0x0212, 0x0b},
    {0x0213, 0x40},
    {0x0215, 0x10},
    {0x023e, 0x99},
    {0x03fe, 0x00},
    {REG_END, 0x00},
};
#endif

static const struct gc3003_mode supported_modes[] =
{
    [GC3003_1920X528] = {
        .bus_fmt = MEDIA_BUS_FMT_SRGGB10_1X10,
        .width = 1920,
        .height = 528,
        .max_fps = {
            .numerator = 10000,
            .denominator = 700000,
        },
        .exp_def = 0x100,
        .hts_def = 0x5dc * 2,
        .vts_def = 0x023c,
        .xvclk = 27000000,
        .link_freq = 310000000,
        .reg_list = g_gc3003_1920x528_70fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_gc3003_1920x528_70fps_reg_table),
        .stream_on_reg_list = g_gc3003_stream_on_reg_table,
        .stream_on_reg_list_size = ARRAY_SIZE(g_gc3003_stream_on_reg_table),
        .hdr_mode = NO_HDR,
    },
    [GC3003_2304X1296] = {
        .bus_fmt = MEDIA_BUS_FMT_SRGGB10_1X10,
        .width = 2304,
        .height = 1296,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x500,
        .hts_def = 0x540 * 2,
        .vts_def = 0x053c,
        .xvclk = 27000000,
        .link_freq = 310000000,
        .reg_list = g_gc3003_2304x1296_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_gc3003_2304x1296_30fps_reg_table),
        .stream_on_reg_list = g_gc3003_stream_on_reg_table,
        .stream_on_reg_list_size = ARRAY_SIZE(g_gc3003_stream_on_reg_table),
        .hdr_mode = NO_HDR,
    },
    [GC3003_320X240] = {
        .bus_fmt = MEDIA_BUS_FMT_SRGGB10_1X10,
        .width = 320,
        .height = 240,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x100,
        .hts_def = 0x5dc * 2,
        .vts_def = 0x012c,
        .xvclk = 27000000,
        .link_freq = 310000000,
        .reg_list = g_gc3003_320x240_120fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_gc3003_320x240_120fps_reg_table),
        .stream_on_reg_list = g_gc3003_stream_on_reg_table,
        .stream_on_reg_list_size = ARRAY_SIZE(g_gc3003_stream_on_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct gc3003_ops
{
    ret_err_t (*init)(struct gc3003_dev *dev);
    ret_err_t (*open)(struct gc3003_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct gc3003_dev *dev);
    ret_err_t (*control)(struct gc3003_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t gc3003_read_reg(struct rt_i2c_bus_device *bus,
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
    msg[0].addr = g_gc3003.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_gc3003.i2c_addr;
    msg[1].flags = RT_I2C_RD;
    msg[1].len = 1;
    msg[1].buf = recv_buf;

    while (1)
    {
        ret = rt_i2c_transfer(bus, &msg, 2);
        if (ret == 2)
        {
            *data = recv_buf[0];
            return RET_SYS_EOK;
        }
        else
        {
            gc3003_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t gc3003_write_reg(struct rt_i2c_bus_device *bus,
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

    msg.addr = g_gc3003.i2c_addr;
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
            gc3003_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int gc3003_write_multiple_reg_continue(struct gc3003_dev *dev,
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
        msg.addr = g_gc3003.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);

        if (ret != 1)
        {
            gc3003_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t gc3003_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msg.addr = g_gc3003.i2c_addr;
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
            gc3003_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}


static void gc3003_write_multiple_reg(struct gc3003_dev *dev,
                                      const struct gc3003_sensor_reg *reg, int len)
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
                gc3003_write_reg_discontinued(i2c_bus, data, k + 2);
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
                gc3003_write_reg_discontinued(i2c_bus, data, k + 2);
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
                    gc3003_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct gc3003_dev *dev)
{

    int id = 0;
    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        gc3003_info("id: %d, width: %d, height: %d\n",
                    id, supported_modes[id].width, supported_modes[id].height);
        gc3003_write_multiple_reg((struct gc3003_dev *)dev,
                                  supported_modes[id].reg_list,
                                  supported_modes[id].reg_list_size);
    }
}


#endif

static void gc3003_get_gain_reg(int *line, uint32_t *tol_dig_gain, uint32_t gain)
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

static ret_err_t rk_gc3003_set_expval(struct gc3003_dev *dev, struct rk_camera_exp_val *exp)
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
        gc3003_dbg("gc3003 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    gain = exp->reg_gain[0];

    if (l_exp_time < GC3003_EXP_MIN)
        l_exp_time = GC3003_EXP_MIN;

    gc3003_dbg("gc3003: exp req: L_exp: 0x%x, 0x%x\n",
               l_exp_time, gain);

    ret = gc3003_write_reg(dev->i2c_bus, GC3003_REG_EXPOSURE_H,
                           (uint8_t)(l_exp_time >> 8));
    ret |= gc3003_write_reg(dev->i2c_bus, GC3003_REG_EXPOSURE_L,
                            (uint8_t)(l_exp_time & 0xff));
    gc3003_get_gain_reg(&line, &tol_dig_gain, gain);

    gc3003_dbg("gain: %d, line: %d tol_dig_gain: %d\n",  gain, line, tol_dig_gain);

    ret |= gc3003_write_reg(dev->i2c_bus, 0x0d1,
                            reg_val_table_liner[line][0]);
    ret |= gc3003_write_reg(dev->i2c_bus, 0x0d0,
                            reg_val_table_liner[line][1]);
    ret |= gc3003_write_reg(dev->i2c_bus, 0x080,
                            reg_val_table_liner[line][2]);
    ret |= gc3003_write_reg(dev->i2c_bus, 0x155,
                            reg_val_table_liner[line][3]);
    ret |= gc3003_write_reg(dev->i2c_bus, 0x0b8,
                            reg_val_table_liner[line][4]);
    ret |= gc3003_write_reg(dev->i2c_bus, 0x0b9,
                            reg_val_table_liner[line][5]);
    ret |= gc3003_write_reg(dev->i2c_bus, 0x0b1,
                            (tol_dig_gain >> 6));
    ret |= gc3003_write_reg(dev->i2c_bus, 0x0b2,
                            ((tol_dig_gain & 0x3f) << 2));

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_gc3003_set_vts(struct gc3003_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    gc3003_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = gc3003_write_reg(dev->i2c_bus, GC3003_REG_VTS_L,
                           (uint8_t)(dst_vts & 0xff));
    ret |= gc3003_write_reg(dev->i2c_bus, GC3003_REG_VTS_H,
                            (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_gc3003_set_flip_mirror(struct gc3003_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0, val_1 = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = gc3003_read_reg(dev->i2c_bus, GC3003_FLIP_MIRROR_REG, &val);
    ret = gc3003_read_reg(dev->i2c_bus, GC3003_FLIP_MIRROR_REG_1, &val_1);
    switch (flip)
    {
    case 0 ... 3:
        val = GC3003_FETCH_MIRROR(val, flip & 0x01);
        val = GC3003_FETCH_FLIP(val, (flip >> 1) & 0x01);
        val_1 = GC3003_FETCH_MIRROR(val, flip & 0x01);
        val_1 = GC3003_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = GC3003_FETCH_MIRROR(val, false);
        val = GC3003_FETCH_FLIP(val, false);
        val_1 = GC3003_FETCH_MIRROR(val, false);
        val_1 = GC3003_FETCH_FLIP(val, false);
        break;
    };
    gc3003_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = gc3003_write_reg(dev->i2c_bus, GC3003_FLIP_MIRROR_REG, val);
    ret = gc3003_write_reg(dev->i2c_bus, GC3003_FLIP_MIRROR_REG_1, val_1);

    return ret;
}

static void gc3003_stream_on(struct gc3003_dev *dev)
{
    gc3003_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    gc3003_write_multiple_reg_continue((struct gc3003_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_gc3003_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_gc3003_set_flip_mirror(dev, dev->flip);
    gc3003_write_reg(dev->i2c_bus, GC3003_REG_CTRL_MODE, GC3003_MODE_STREAMING);
    /* gc3003_write_multiple_reg_continue((struct gc3003_dev *)dev, dev->cur_mode->stream_on_reg_list, dev->cur_mode->stream_on_reg_list_size); */
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    gc3003_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void gc3003_stream_on_late(struct gc3003_dev *dev)
{
    gc3003_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_gc3003_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_gc3003_set_flip_mirror(dev, dev->flip);

    gc3003_write_reg(dev->i2c_bus, GC3003_REG_CTRL_MODE, GC3003_MODE_STREAMING);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    gc3003_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void gc3003_stream_off(struct gc3003_dev *dev)
{
    struct  rt_i2c_bus_device *i2c_bus;


    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        gc3003_write_reg(dev->i2c_bus, GC3003_REG_CTRL_MODE, GC3003_MODE_SW_STANDBY);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        gc3003_err("not find out i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_gc3003_init(struct rk_camera_device *dev)
{

    ret_err_t ret = RET_SYS_EOK;
    struct gc3003_dev *gc3003;

    gc3003 = (struct gc3003_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&gc3003->parent;
#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(gc3003);
#endif


    if (gc3003)
    {
        camera->info.mbus_fmt.width = gc3003->cur_mode->width;
        camera->info.mbus_fmt.height = gc3003->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10; // 0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = gc3003->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode = gc3003->cur_mode->hdr_mode;

        gc3003->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(gc3003->i2c_name);

        if (!gc3003->i2c_bus)
        {
            gc3003_dbg("not find i2c source 2:%s !!!\n",
                       GC3003->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            gc3003_dbg("s0 find i2c_bus:%s\n", GC3003->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_gc3003_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_GC3003_close(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_gc3003_get_expinf(struct gc3003_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct gc3003_mode *mode;

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

static ret_err_t rk_gc3003_get_intput_fmt(struct gc3003_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct gc3003_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_gc3003_set_intput_fmt(struct gc3003_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct gc3003_mode *mode;
    struct camera_board_desc *gc3003 = (struct camera_board_desc *)&camera_gc3003;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    gc3003_info("dst resulotion, width %d, height %d\n",
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
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc3003->rst_gpio.gpio_group, gc3003->rst_gpio.gpio_pin, gc3003->rst_active);
            HAL_DelayUs(5000);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc3003->rst_gpio.gpio_group, gc3003->rst_gpio.gpio_pin, !gc3003->rst_active);
#ifndef REG_LIST_CONVERSION_DEBUG
            gc3003_write_multiple_reg_continue((struct gc3003_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_gc3003_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static rt_err_t rk_gc3003_match_dst_config(struct gc3003_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct gc3003_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    gc3003_info("cur resulotion, width %d, height %d, fps %d!\n",
                dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct gc3003_mode *)mode;
            ret = RET_SYS_EOK;
            gc3003_info("find dst resulotion, width %d, height %d, fps %d!\n",
                        dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            gc3003_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        gc3003_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_gc3003_control(struct rk_camera_device *dev,
                            int cmd,
                            void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct gc3003_dev *GC3003;

    RT_ASSERT(dev != RT_NULL);
    GC3003 = (struct gc3003_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_gc3003_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        gc3003_stream_on(GC3003);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        gc3003_stream_off(GC3003);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_gc3003_get_expinf(GC3003, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_gc3003_set_expval(GC3003, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_gc3003_set_vts(GC3003, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_gc3003_get_intput_fmt(GC3003, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_gc3003_set_intput_fmt(GC3003, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_gc3003_match_dst_config(GC3003, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_gc3003_set_flip_mirror(GC3003, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        gc3003_stream_on_late(GC3003);
    }
    break;
    default:
        gc3003_dbg(" exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_gc3003_ops =
{
    rk_gc3003_init,
    rk_gc3003_open,
    NULL,
    rk_gc3003_control
};

int rk_camera_gc3003_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct gc3003_dev *instance = &g_gc3003;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *gc3003 = (struct camera_board_desc *)&camera_gc3003;

    camera->ops = &rk_gc3003_ops;

    rt_strncpy(instance->name, gc3003->isp_subdev_name, rt_strlen(gc3003->isp_subdev_name));
    rt_strncpy(instance->i2c_name, gc3003->i2c_bus, rt_strlen(gc3003->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (gc3003->mode_id <= GC3003_MODE_ID_MAX)
    {
        gc3003_dbg("mode_id: %d\n", gc3003->mode_id);
        instance->cur_mode = &supported_modes[gc3003->mode_id];
    }
    else
    {
        gc3003_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct gc3003_mode *)&supported_modes[GC3003_2304X1296];
    instance->time_valid_delay = gc3003->time_valid_delay;
    instance->gain_valid_delay = gc3003->gain_valid_delay;
    instance->i2c_addr = gc3003->i2c_addr;
    instance->flip = 0;

    HAL_GPIO_SetPinDirection((struct GPIO_REG *)gc3003->rst_gpio.gpio_group, gc3003->rst_gpio.gpio_pin, GPIO_OUT);
    HAL_GPIO_SetPinLevel((struct GPIO_REG *)gc3003->rst_gpio.gpio_group, gc3003->rst_gpio.gpio_pin, !gc3003->rst_active);

    if (gc3003->mclk_out_gate_id && gc3003->mclk_id)
    {
        clk_set_rate(gc3003->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(gc3003->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "GC3003_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, gc3003->isp_subdev_name, rt_strlen(gc3003->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void gc3003_detect(void)
{
    struct gc3003_dev *instance = &g_gc3003;

    gc3003_dbg("start to detect GC3003 for testing \n");
    gc3003_dbg("dev name:%s\n", instance->name);
    gc3003_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        gc3003_dbg("not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        gc3003_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_gc3003_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(gc3003_detect, check GC3003 is available or not);
#endif
#endif
#endif
