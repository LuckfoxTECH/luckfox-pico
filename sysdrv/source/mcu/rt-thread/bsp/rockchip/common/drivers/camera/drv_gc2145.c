/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_gc2145.c
  * @version V0.0.1
  * @brief   video input processor driver for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

#if defined(__RT_THREAD__)
#include "camera.h"
#elif defined(__RK_OS__)
#include "driver/camera.h"
#endif

#ifdef CAMERA_MODULE_COMPILED

#if defined(__RT_THREAD__)
#include "drv_clock.h"
#include "board.h"
#elif defined(__RK_OS__)
#include "driver/drv_clock.h"
#endif

#if defined(RT_USING_GC2145) || defined(CONFIG_DRIVER_GC2145)
#define GC2145_MODULE_COMPILED
#else
#undef GC2145_MODULE_COMPILED
#endif

#endif


#ifdef GC2145_MODULE_COMPILED

#define GC2145_DEBUG_INFO 0

#if GC2145_DEBUG_INFO
#include <stdio.h>
#define GC2145_DEBUG(...)               rk_kprintf("[GC2145]:");rk_kprintf(__VA_ARGS__)
#else
#define GC2145_DEBUG(...)
#endif

#define GC2145_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define RT_USING_GC2145_OPS 0

#define GC2145_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME                "i2c0"
#define GC2145_7BIT_ADDRESS         (0x78 >> 1)
#define GC2145_REG_ID_H_ADDRESS     (0xf0)
#define GC2145_REG_ID_L_ADDRESS     (0xf1)
#define GC2145_SENSOR_ID            (0x2145)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define GC2145_PIN_CTRL_ENABLE          1
#define GC2145_I2C_DEBUG_ENABLE         1
#define GC2145_TESTPATTERN_ENABLE       0

#if  RT_USING_GC2145_OPS
struct gc2145_ops
{
    ret_err_t (*init)(struct gc2145_dev *dev);
    ret_err_t (*open)(struct gc2145_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct gc2145_dev *dev);
    ret_err_t (*control)(struct gc2145_dev *dev, int cmd, void *arg);
    ret_err_t (*rx_indicate)(struct gc2145_dev *dev, ret_size_t size);
};
#endif

struct mclk
{
    rk_clk_gate *gate;
    eCLOCK_Name clk_name;
};

struct gc2145_dev
{
    struct rk_camera_device parent;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];

#if RT_USING_GC2145_OPS
    struct gc2145_ops *ops;
#endif

    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    struct mclk mclk;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
    struct rt_mutex mutex_lock;
};
typedef struct gc2145_dev *rt_gc2145_dev_t;

struct sensor_reg
{
    uint8_t reg_addr;
    uint8_t val;
};

static struct gc2145_dev gc2145;
static const struct sensor_reg g_gc2145_init_reg_table[] =
{
    {0xfe, 0xf0},
    {0xfe, 0xf0},
    {0xfe, 0xf0},
    {0xfc, 0x06},
    {0xf6, 0x00},
    {0xf7, 0x1d},
    {0xf8, 0x84},
    {0xfa, 0x00},
    {0xf9, 0xfe},
    {0xf2, 0x00},
    /*ISP reg*/
    {0xfe, 0x00},
    {0x03, 0x04},
    {0x04, 0xe2},
    {0x09, 0x00},
    {0x0a, 0x00},
    {0x0b, 0x00},
    {0x0c, 0x00},
    {0x0d, 0x04},
    {0x0e, 0xc0},
    {0x0f, 0x06},
    {0x10, 0x52},
    {0x12, 0x2e},
    {0x17, 0x14},
    {0x18, 0x22},
    {0x19, 0x0e},
    {0x1a, 0x01},
    {0x1b, 0x4b},
    {0x1c, 0x07},
    {0x1d, 0x10},
    {0x1e, 0x88},
    {0x1f, 0x78},
    {0x20, 0x03},
    {0x21, 0x40},
    {0x22, 0xa0},
    {0x24, 0x3f},
    {0x25, 0x01},
    {0x26, 0x10},
    {0x2d, 0x60},
    {0x30, 0x01},
    {0x31, 0x90},
    {0x33, 0x06},
    {0x34, 0x01},
    {0xfe, 0x00},
    {0x80, 0x7f},
    {0x81, 0x26},
    {0x82, 0xfa},
    {0x83, 0x00},
    {0x84, 0x00}, //yuv:0x00 raw:0x18
    {0x86, 0x02},
    {0x88, 0x03},
    {0x89, 0x03},
    {0x85, 0x08},
    {0x8a, 0x00},
    {0x8b, 0x00},
    {0xb0, 0x55},
    {0xc3, 0x00},
    {0xc4, 0x80},
    {0xc5, 0x90},
    {0xc6, 0x3b},
    {0xc7, 0x46},
    {0xec, 0x06},
    {0xed, 0x04},
    {0xee, 0x60},
    {0xef, 0x90},
    {0xb6, 0x01},
    {0x90, 0x01},
    {0x91, 0x00},
    {0x92, 0x00},
    {0x93, 0x00},
    {0x94, 0x00},
    {0x95, 0x04},
    {0x96, 0xb0},
    {0x97, 0x06},
    {0x98, 0x40},
    /*BLK*/
    {0xfe, 0x00},
    {0x40, 0x42},
    {0x41, 0x00},
    {0x43, 0x5b},
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},
    {0x63, 0x00},
    {0x64, 0x00},
    {0x65, 0x00},
    {0x66, 0x20},
    {0x67, 0x20},
    {0x68, 0x20},
    {0x69, 0x20},
    {0x76, 0x00},
    {0x6a, 0x08},
    {0x6b, 0x08},
    {0x6c, 0x08},
    {0x6d, 0x08},
    {0x6e, 0x08},
    {0x6f, 0x08},
    {0x70, 0x08},
    {0x71, 0x08},
    {0x76, 0x00},
    {0x72, 0xf0},
    {0x7e, 0x3c},
    {0x7f, 0x00},
    {0xfe, 0x02},
    {0x48, 0x15},
    {0x49, 0x00},
    {0x4b, 0x0b},
    {0xfe, 0x00},
    /*AEC*/
    {0xfe, 0x01},
    {0x01, 0x04},
    {0x02, 0xc0},
    {0x03, 0x04},
    {0x04, 0x90},
    {0x05, 0x30},
    {0x06, 0x90},
    {0x07, 0x30},
    {0x08, 0x80},
    {0x09, 0x00},
    {0x0a, 0x82},
    {0x0b, 0x11},
    {0x0c, 0x10},
    {0x11, 0x10},
    {0x13, 0x7b},
    {0x17, 0x00},
    {0x1c, 0x11},
    {0x1e, 0x61},
    {0x1f, 0x35},
    {0x20, 0x40},
    {0x22, 0x40},
    {0x23, 0x20},
    {0xfe, 0x02},
    {0x0f, 0x04},
    {0xfe, 0x01},
    {0x12, 0x35},
    {0x15, 0xb0},
    {0x10, 0x31},
    {0x3e, 0x28},
    {0x3f, 0xb0},
    {0x40, 0x90},
    {0x41, 0x0f},

    /*INTPEE*/
    {0xfe, 0x02},
    {0x90, 0x6c},
    {0x91, 0x03},
    {0x92, 0xcb},
    {0x94, 0x33},
    {0x95, 0x84},
    {0x97, 0x45},
    {0xa2, 0x11},
    {0xfe, 0x00},
    /*DNDD*/
    {0xfe, 0x02},
    {0x80, 0xc1},
    {0x81, 0x08},
    {0x82, 0x1f},
    {0x83, 0x10},
    {0x84, 0x0a},
    {0x86, 0xf0},
    {0x87, 0x50},
    {0x88, 0x15},
    {0x89, 0xb0},
    {0x8a, 0x30},
    {0x8b, 0x10},
    /*ASDE*/
    {0xfe, 0x01},
    {0x21, 0x04},
    {0xfe, 0x02},
    {0xa3, 0x50},
    {0xa4, 0x20},
    {0xa5, 0x40},
    {0xa6, 0x80},
    {0xab, 0x40},
    {0xae, 0x0c},
    {0xb3, 0x46},
    {0xb4, 0x64},
    {0xb6, 0x38},
    {0xb7, 0x01},
    {0xb9, 0x2b},
    {0x3c, 0x04},
    {0x3d, 0x15},
    {0x4b, 0x06},
    {0x4c, 0x20},
    {0xfe, 0x00},
    /*GAMMA*/
    /*gamma1*/
    {0xfe, 0x02},
    {0x10, 0x09},
    {0x11, 0x0d},
    {0x12, 0x13},
    {0x13, 0x19},
    {0x14, 0x27},
    {0x15, 0x37},
    {0x16, 0x45},
    {0x17, 0x53},
    {0x18, 0x69},
    {0x19, 0x7d},
    {0x1a, 0x8f},
    {0x1b, 0x9d},
    {0x1c, 0xa9},
    {0x1d, 0xbd},
    {0x1e, 0xcd},
    {0x1f, 0xd9},
    {0x20, 0xe3},
    {0x21, 0xea},
    {0x22, 0xef},
    {0x23, 0xf5},
    {0x24, 0xf9},
    {0x25, 0xff},

    {0xfe, 0x00},
    {0xc6, 0x20},
    {0xc7, 0x2b},

    /*gamma2*/
    {0xfe, 0x02},
    {0x26, 0x0f},
    {0x27, 0x14},
    {0x28, 0x19},
    {0x29, 0x1e},
    {0x2a, 0x27},
    {0x2b, 0x33},
    {0x2c, 0x3b},
    {0x2d, 0x45},
    {0x2e, 0x59},
    {0x2f, 0x69},
    {0x30, 0x7c},
    {0x31, 0x89},
    {0x32, 0x98},
    {0x33, 0xae},
    {0x34, 0xc0},
    {0x35, 0xcf},
    {0x36, 0xda},
    {0x37, 0xe2},
    {0x38, 0xe9},
    {0x39, 0xf3},
    {0x3a, 0xf9},
    {0x3b, 0xff},

    /*YCP*/
    {0xfe, 0x02},
    {0xd1, 0x40},
    {0xd2, 0x40},
    {0xd3, 0x48},
    {0xd6, 0xf0},
    {0xd7, 0x10},
    {0xd8, 0xda},
    {0xdd, 0x14},
    {0xde, 0x86},
    {0xed, 0x80},
    {0xee, 0x00},
    {0xef, 0x3f},
    {0xd8, 0xd8},
    /*abs*/
    {0xfe, 0x01},
    {0x9f, 0x40},
    /*LSC*/
    {0xfe, 0x01},
    {0xc2, 0x14},
    {0xc3, 0x0d},
    {0xc4, 0x0c},
    {0xc8, 0x15},
    {0xc9, 0x0d},
    {0xca, 0x0a},
    {0xbc, 0x24},
    {0xbd, 0x10},
    {0xbe, 0x0b},
    {0xb6, 0x25},
    {0xb7, 0x16},
    {0xb8, 0x15},
    {0xc5, 0x00},
    {0xc6, 0x00},
    {0xc7, 0x00},
    {0xcb, 0x00},
    {0xcc, 0x00},
    {0xcd, 0x00},
    {0xbf, 0x07},
    {0xc0, 0x00},
    {0xc1, 0x00},
    {0xb9, 0x00},
    {0xba, 0x00},
    {0xbb, 0x00},
    {0xaa, 0x01},
    {0xab, 0x01},
    {0xac, 0x00},
    {0xad, 0x05},
    {0xae, 0x06},
    {0xaf, 0x0e},
    {0xb0, 0x0b},
    {0xb1, 0x07},
    {0xb2, 0x06},
    {0xb3, 0x17},
    {0xb4, 0x0e},
    {0xb5, 0x0e},
    {0xd0, 0x09},
    {0xd1, 0x00},
    {0xd2, 0x00},
    {0xd6, 0x08},
    {0xd7, 0x00},
    {0xd8, 0x00},
    {0xd9, 0x00},
    {0xda, 0x00},
    {0xdb, 0x00},
    {0xd3, 0x0a},
    {0xd4, 0x00},
    {0xd5, 0x00},
    {0xa4, 0x00},
    {0xa5, 0x00},
    {0xa6, 0x77},
    {0xa7, 0x77},
    {0xa8, 0x77},
    {0xa9, 0x77},
    {0xa1, 0x80},
    {0xa2, 0x80},

    {0xfe, 0x01},
    {0xdf, 0x0d},
    {0xdc, 0x25},
    {0xdd, 0x30},
    {0xe0, 0x77},
    {0xe1, 0x80},
    {0xe2, 0x77},
    {0xe3, 0x90},
    {0xe6, 0x90},
    {0xe7, 0xa0},
    {0xe8, 0x90},
    {0xe9, 0xa0},
    {0xfe, 0x00},
    /*AWB*/
    {0xfe, 0x01},
    {0x4f, 0x00},
    {0x4f, 0x00},
    {0x4b, 0x01},
    {0x4f, 0x00},

    {0x4c, 0x01},
    {0x4d, 0x71},
    {0x4e, 0x01},
    {0x4c, 0x01},
    {0x4d, 0x91},
    {0x4e, 0x01},
    {0x4c, 0x01},
    {0x4d, 0x70},
    {0x4e, 0x01},
    {0x4c, 0x01},
    {0x4d, 0x90},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xb0},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0x8f},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0x6f},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xaf},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xd0},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xf0},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xcf},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0xef},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0x6e},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8e},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xae},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xce},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x4d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xad},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xcd},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x4c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xac},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xcc},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xcb},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x4b},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6b},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8b},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xab},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8a},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xaa},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xca},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xca},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xc9},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x8a},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0x89},
    {0x4e, 0x04},
    {0x4c, 0x01},
    {0x4d, 0xa9},
    {0x4e, 0x04},
    {0x4c, 0x02},
    {0x4d, 0x0b},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x0a},
    {0x4e, 0x05},
    {0x4c, 0x01},
    {0x4d, 0xeb},
    {0x4e, 0x05},
    {0x4c, 0x01},
    {0x4d, 0xea},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x09},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x29},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x2a},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x4a},
    {0x4e, 0x05},
    {0x4c, 0x02},
    {0x4d, 0x8a},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x49},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x69},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x89},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0xa9},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x48},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x68},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0x69},
    {0x4e, 0x06},
    {0x4c, 0x02},
    {0x4d, 0xca},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xc9},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe9},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x09},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xc8},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe8},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xa7},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xc7},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe7},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x07},
    {0x4e, 0x07},

    {0x4f, 0x01},
    {0x50, 0x80},
    {0x51, 0xa8},
    {0x52, 0x47},
    {0x53, 0x38},
    {0x54, 0xc7},
    {0x56, 0x0e},
    {0x58, 0x08},
    {0x5b, 0x00},
    {0x5c, 0x74},
    {0x5d, 0x8b},
    {0x61, 0xdb},
    {0x62, 0xb8},
    {0x63, 0x86},
    {0x64, 0xc0},
    {0x65, 0x04},
    {0x67, 0xa8},
    {0x68, 0xb0},
    {0x69, 0x00},
    {0x6a, 0xa8},
    {0x6b, 0xb0},
    {0x6c, 0xaf},
    {0x6d, 0x8b},
    {0x6e, 0x50},
    {0x6f, 0x18},
    {0x73, 0xf0},
    {0x70, 0x0d},
    {0x71, 0x60},
    {0x72, 0x80},
    {0x74, 0x01},
    {0x75, 0x01},
    {0x7f, 0x0c},
    {0x76, 0x70},
    {0x77, 0x58},
    {0x78, 0xa0},
    {0x79, 0x5e},
    {0x7a, 0x54},
    {0x7b, 0x58},
    {0xfe, 0x00},
    /*CC*/
    {0xfe, 0x02},
    {0xc0, 0x01},
    {0xc1, 0x44},
    {0xc2, 0xfd},
    {0xc3, 0x04},
    {0xc4, 0xF0},
    {0xc5, 0x48},
    {0xc6, 0xfd},
    {0xc7, 0x46},
    {0xc8, 0xfd},
    {0xc9, 0x02},
    {0xca, 0xe0},
    {0xcb, 0x45},
    {0xcc, 0xec},
    {0xcd, 0x48},
    {0xce, 0xf0},
    {0xcf, 0xf0},
    {0xe3, 0x0c},
    {0xe4, 0x4b},
    {0xe5, 0xe0},
    /*ABS*/
    {0xfe, 0x01},
    {0x9f, 0x40},
    {0xfe, 0x00},
    /*OUTPUT*/
    {0xfe, 0x00},
    {0xf2, 0x0f},
    /*dark sun*/
    {0xfe, 0x02},
    {0x40, 0xbf},
    {0x46, 0xcf},
    {0xfe, 0x00},

    /*frame rate 50Hz*/
    {0xfe, 0x00},
    {0x05, 0x01},
    {0x06, 0x56},
    {0x07, 0x00},
    {0x08, 0x32},
    {0xfe, 0x01},
    {0x25, 0x00},
    {0x26, 0xfa},

    {0x27, 0x04},
    {0x28, 0xe2},
    {0x29, 0x04},
    {0x2a, 0xe2},
    {0x2b, 0x04},
    {0x2c, 0xe2},
    {0x2d, 0x04},
    {0x2e, 0xe2},
    {0xfe, 0x00},

    {0xfe, 0x00},
    {0xfd, 0x01},
    {0xfa, 0x00},
    /*crop window*/
    {0xfe, 0x00},
    {0x90, 0x01},
    {0x91, 0x00},
    {0x92, 0x00},
    {0x93, 0x00},
    {0x94, 0x00},
    {0x95, 0x02},
    {0x96, 0x58},
    {0x97, 0x03},
    {0x98, 0x20},
    {0x99, 0x11},
    {0x9a, 0x06},
    /*AWB*/
    {0xfe, 0x00},
    {0xec, 0x02},
    {0xed, 0x02},
    {0xee, 0x30},
    {0xef, 0x48},
    {0xfe, 0x02},
    {0x9d, 0x08},
    {0xfe, 0x01},
    {0x74, 0x00},
    /*AEC*/
    {0xfe, 0x01},
    {0x01, 0x04},
    {0x02, 0x60},
    {0x03, 0x02},
    {0x04, 0x48},
    {0x05, 0x18},
    {0x06, 0x50},
    {0x07, 0x10},
    {0x08, 0x38},
    {0x0a, 0x80},
    {0x21, 0x04},
    {0xfe, 0x00},
    {0x20, 0x03},
    {0xfe, 0x00},
    {REG_END, 0x0}
};

static const struct sensor_reg g_gc2145_qvga_reg_table[] =
{
    {0xfe, 0x00},
    {0x05, 0x02}, //0x02
    {0x06, 0x20}, //0x20
    {0x07, 0x03},
    {0x08, 0x80},
    {0xb6, 0x01},
    {0xfd, 0x03},
    {0xfa, 0x00},
    {0x18, 0x42},
    /*crop window*/
    {0xfe, 0x00},
    {0x90, 0x01},
    {0x91, 0x00},
    {0x92, 0x00},
    {0x93, 0x00},
    {0x94, 0x00},
    {0x95, 0x02}, //0x02 00 0x01 height
    {0x96, 0x58}, //0x58 f0 0x2c
    {0x97, 0x03}, //0x03 01 0x01 width
    {0x98, 0x20}, //0x20 40 0x90
    {0x99, 0x11},
    {0x9a, 0x06},
    /*AWB*/
    {0xfe, 0x00},
    {0xec, 0x02},
    {0xed, 0x02},
    {0xee, 0x30},
    {0xef, 0x48},
    {0xfe, 0x02},
    {0x9d, 0x08},
    {0xfe, 0x01},
    {0x74, 0x00},
    /*AEC*/
    {0xfe, 0x01},
    {0x01, 0x04},
    {0x02, 0x60},
    {0x03, 0x02},
    {0x04, 0x48},
    {0x05, 0x18},
    {0x06, 0x50},
    {0x07, 0x10},
    {0x08, 0x38},
    {0x0a, 0x80},
    {0x21, 0x04},
    {0xfe, 0x00},
    {0x20, 0x03},
    {0xfe, 0x00},
    {REG_END, 0x00},
};

#if GC2145_TESTPATTERN_ENABLE
static const struct sensor_reg g_gc2145_testpattern_reg_table[] =
{
    {0xfe, 0x00},
    {0xb6, 0x00},
    {0x80, 0x08},
    {0x81, 0x00},
    {0x82, 0x00},
    {0x03, 0x06},
    {0x04, 0x40},
    {0x40, 0x00},
    {0x5e, 0x00},
    {0x5f, 0x00},
    {0x60, 0x00},
    {0x61, 0x00},
    {0x62, 0x00},
    {0x63, 0x00},
    {0x64, 0x00},
    {0x65, 0x00},
    {0x66, 0x00},
    {0x67, 0x00},
    {0x68, 0x00},
    {0x69, 0x00},
    {0xb0, 0x60},
    {0xb1, 0x20},
    {0xb2, 0x40},
    {0xb3, 0x40},
    {0xb4, 0x40},
    {0xb5, 0x40},
    {0xfe, 0x02},
    {0x3c, 0x00},
    {0x4b, 0x00},
    {0x49, 0x00},
    {0xdd, 0x00},
    {0xfe, 0x00},
    {0x8c, 0x01},
    {REG_END, 0x00},
};
#endif

static ret_err_t gc2145_write_reg(rk_i2c_bus_device *bus,
                                  uint8_t reg, const uint8_t *data)
{
    uint8_t send_buf[2];
    struct rt_i2c_msg msgs;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = (reg & 0xff);
    send_buf[1] = data[0];

    msgs.addr = GC2145_7BIT_ADDRESS;
    msgs.flags = RT_I2C_WR;
    msgs.buf = send_buf;
    msgs.len = 2;

    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RET_SYS_EOK;
    }
    else
    {
        return RET_SYS_ERROR;
    }
}

static ret_err_t gc2145_read_reg(rk_i2c_bus_device *bus,
                                 uint8_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = (reg & 0xff);
    msg[0].addr = GC2145_7BIT_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 1;
    msg[0].buf = send_buf;

    msg[1].addr = GC2145_7BIT_ADDRESS;
    msg[1].flags = RT_I2C_RD;
    msg[1].len = 1;
    msg[1].buf = recv_buf;

    ret = rt_i2c_transfer(bus, msg, 2);
    *data = recv_buf[0];
    if (ret == 2)
        ret = RET_SYS_EOK;
    else
        ret = RET_SYS_ERROR;

    return ret;
}

static void gc2145_write_multiple_reg(struct gc2145_dev *dev,
                                      const struct sensor_reg *reg)
{
    uint16_t i;
    rk_i2c_bus_device *i2c_bus;

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(reg != RK_NULL);

    i2c_bus = dev->i2c_bus;
    MACRO_ASSERT(i2c_bus != RK_NULL);

    for (i = 0;; i++)
    {
        if (reg[i].reg_addr == REG_END)
        {
            break;
        }

        if (reg[i].reg_addr == REG_DELAY)
        {
            HAL_DelayUs(reg[i].val);
        }
        else
        {
            gc2145_write_reg(i2c_bus, reg[i].reg_addr, &reg[i].val);
        }
    }

}

static ret_err_t rt_gc2145_detect_sensor(struct gc2145_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    ret_err_t ret;
    uint8_t pid, ver;
    rt_uint16_t chipid;
    rk_i2c_bus_device *bus = dev->i2c_bus;

    GC2145_DEBUG("(%s) enter \n", __FUNCTION__);
    rk_clk_set_rate(dev->mclk.clk_name, 24000000);
    rk_clk_enable(dev->mclk.gate);

    /* powerdown: active high */
    HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_LOW);

    /* reset: active low */
    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_HIGH);

RETRY:
    ret = gc2145_read_reg(bus, GC2145_REG_ID_H_ADDRESS, &pid);
    if (ret == RET_SYS_EOK)
    {
        GC2145_INFO(dev, "Info: GC2145 pid:0x%x!\n", pid);
        ret = gc2145_read_reg(bus, GC2145_REG_ID_L_ADDRESS, &ver);
        chipid = SENSOR_ID(pid, ver);
        GC2145_INFO(dev, "Info: GC2145 pid:0x%x,ver:0x%x!\n", pid, ver);
        if (chipid == GC2145_SENSOR_ID)
        {
            GC2145_INFO(dev, "Info: GC2145 detected successfully !!!\n");
            ret = RET_SYS_EOK;
        }
        else
        {
            GC2145_INFO(dev, "Info: GC2145 detected failed,not match:%d != %d !!!\n",
                        GC2145_SENSOR_ID, chipid);
            ret = -RT_EINVAL;
            goto RETRY;
        }
    }
    else
    {

        GC2145_INFO(dev, "Warning: GC2145 i2c read failed !!!\n");
        goto RETRY;
    }

    /* powerdown: active high */
    HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_LOW);

    /* reset: active low */
    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_HIGH);
    rk_clk_disable(dev->mclk.gate);

    GC2145_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static void gc2145_stream_on(struct gc2145_dev *dev)
{
    uint8_t val;
    GC2145_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    rk_clk_set_rate(dev->mclk.clk_name, 24000000);
    rk_clk_enable(dev->mclk.gate);
    /* powerdown: active high */
    HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_LOW);

    /* reset: active low */
    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_HIGH);

    gc2145_write_multiple_reg(dev, g_gc2145_init_reg_table);
    gc2145_write_multiple_reg(dev, g_gc2145_qvga_reg_table);
#if GC2145_TESTPATTERN_ENABLE
    gc2145_write_multiple_reg(dev, g_gc2145_testpattern_reg_table);
#endif
    val = 0x0f;
    gc2145_write_reg(dev->i2c_bus, 0xf2, &val);
    rt_mutex_release(&dev->mutex_lock);

    GC2145_DEBUG("(%s) s2,reg[0xf2]:0x%x\n", __FUNCTION__, val);
    GC2145_DEBUG("(%s) exit \n", __FUNCTION__);
}

static void gc2145_stream_off(struct gc2145_dev *dev)
{
    uint8_t val;
    rk_i2c_bus_device *i2c_bus;

    GC2145_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        val = 0x0;
        gc2145_write_reg(i2c_bus, 0xf2, &val);//to stop  camera

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        GC2145_INFO(dev, "Err: not find out i2c bus!\n");
    }

    /* powerdown: active high */
    HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_HIGH);

    /* reset: active low */
    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_LOW);
    rk_clk_disable(dev->mclk.gate);

    GC2145_DEBUG("(%s) exit \n", __FUNCTION__);
}

static ret_err_t rk_gc2145_init(struct rk_camera_device *dev)
{
    GC2145_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct gc2145_dev *gc2145;
    struct rk_camera_device *camera;

    MACRO_ASSERT(dev != RK_NULL);

    gc2145 = (struct gc2145_dev *)dev;
    camera = (struct rk_camera_device *)(&gc2145->parent);
#if RT_USING_GC2145_OPS
    if (gc2145->ops->init)
    {
        return (gc2145->ops->init(gc2145));
    }
#else

    if (gc2145 && camera)
    {
        camera->info.mbus_fmt.width = 800;
        camera->info.mbus_fmt.height = 600;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_UYVY8_2X8;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_BT601;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_VSYNC_POLA_LOW |
                                         MEDIA_BUS_FLAGS_HSYNC_POLA_HIGHT;
        gc2145->i2c_bus = (rk_i2c_bus_device *)rt_device_find(gc2145->i2c_name);

        if (!gc2145->i2c_bus)
        {
            GC2145_DEBUG("Warning:not find i2c source:%s !!!\n",
                         gc2145->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            GC2145_DEBUG("(%s):s0 find i2c_bus:%s\n", gc2145->i2c_name);
        }
        rt_gc2145_detect_sensor(gc2145);
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    GC2145_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

static ret_err_t rk_gc2145_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    GC2145_DEBUG("(%s) enter \n", __FUNCTION__);

    struct gc2145_dev *gc2145;
    struct rk_camera_device *camera;
    ret_err_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    gc2145 = (struct gc2145_dev *)dev;
    camera = (struct rk_camera_device *)(&gc2145->parent);
#if RT_USING_GC2145_OPS
    if (gc2145->ops->open)
    {
        return (gc2145->ops->open(gc2145));
    }
#else

    if (gc2145 && camera)
    {
        ret = RET_SYS_EOK;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

#endif

    GC2145_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

static ret_err_t rk_gc2145_close(struct rk_camera_device *dev)
{
    GC2145_INFO(dev, "(%s) enter \n", __FUNCTION__);

    struct gc2145_dev *gc2145;
    struct rk_camera_device *camera;
    uint8_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    gc2145 = (struct gc2145_dev *)dev;
    camera = (struct rk_camera_device *)(&gc2145->parent);
#if RT_USING_GC2145_OPS
    if (gc2145->ops->close)
    {
        return (gc2145->ops->close(gc2145))
    }
#else
    if (gc2145 && camera)
    {
        ret = RET_SYS_EOK;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    GC2145_INFO(dev, "(%s) exit \n", __FUNCTION__);
    return ret;

}

static ret_err_t rk_gc2145_control(struct rk_camera_device *dev,
                                   int cmd,
                                   void *args)
{
    GC2145_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct gc2145_dev *gc2145;

    MACRO_ASSERT(dev != RK_NULL);
    gc2145 = (struct gc2145_dev *)dev;
#if RT_USING_GC2145_OPS
    return (gc2145->ops->control(gc2145, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_gc2145_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        gc2145_stream_on(gc2145);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        gc2145_stream_off(gc2145);
    }
    break;
    /*
        case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
        {

        }
        break;
    */
    default:
        break;
    }
#endif
    GC2145_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

const static struct rk_camera_ops rk_gc2145_ops =
{
    rk_gc2145_init,
    rk_gc2145_open,
    rk_gc2145_close,
    rk_gc2145_control,
};

int rk_camera_gc2145_init(void)
{
    ret_err_t ret;

    struct gc2145_dev *instance = &gc2145;
    struct rk_camera_device *camera = &instance->parent;

    rt_strncpy(instance->name,
               GC2145_DEVICE_NAME,
               rt_strlen(GC2145_DEVICE_NAME));

    rt_strncpy(instance->i2c_name,
               I2C_BUS_NAME,
               rt_strlen(I2C_BUS_NAME));

#if GC2145_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        GC2145_INFO(instance, "Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        GC2145_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

    instance->mclk.clk_name = CLK_CIFOUT;
    instance->mclk.gate = get_clk_gate_from_id(CLK_CIFOUT_GATE);
    clk_set_rate(CLK_CIFOUT, 24000000);
    clk_enable(instance->mclk.gate);
    GC2145_INFO(instance, "enable cif clk\n");
    clk_disable(instance->mclk.gate);

    if (!instance->mclk.gate)
    {
        GC2145_INFO(instance, "Warning:not find mclk source !!!\n");
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        GC2145_INFO(instance, "Info:get the clk gate id:0x%x\n",
                    instance->mclk.gate->gate_id);
    }

#if GC2145_PIN_CTRL_ENABLE

    HAL_PINCTRL_SetIOMUX(CAMERA_PWDN_GPIO_BANK,
                         CAMERA_PWDN_GPIO_PIN,
                         PIN_CONFIG_MUX_FUNC0);

    HAL_PINCTRL_SetIOMUX(CAMERA_RST_GPIO_BANK,
                         CAMERA_RST_GPIO_PIN,
                         PIN_CONFIG_MUX_FUNC0);
    /* powerdown: active high */
    HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_HIGH);

    /* reset: active low */
    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_LOW);

#endif

    ret = rt_gc2145_detect_sensor(instance);
    if (ret != RET_SYS_EOK)
        goto ERR;

    rt_mutex_init(&instance->mutex_lock, "GC2145_mutex", RT_IPC_FLAG_FIFO);
    MACRO_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->ops = &rk_gc2145_ops;
    rt_strncpy(camera->name, instance->name,
               rt_strlen(GC2145_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);

    GC2145_DEBUG("(%s) exit \n", __FUNCTION__);
    return 0;
ERR:
    GC2145_DEBUG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

void GC2145_detect(void)
{
    struct gc2145_dev *instance = &gc2145;

    GC2145_DEBUG("start to detect GC2145 for testing \n");
    GC2145_DEBUG("dev name:%s\n", instance->name);
    GC2145_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        GC2145_DEBUG("Warning:not find i2c source:%s !!!\n",
                     instance->i2c_name);
        return;
    }
    else
    {
        GC2145_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }

    if (!instance->mclk.gate)
    {
        GC2145_DEBUG("(%s):s1 not find clk gate id:%s\n", instance->mclk.gate->gate_id);
        return;
    }
    else
    {
        GC2145_DEBUG("clk:gate_id:%d, rate:%d !!!\n",
                     instance->mclk.gate->gate_id, clk_get_rate(instance->mclk.clk_name));
    }
    rt_gc2145_detect_sensor(instance);
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_gc2145_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(GC2145_detect, check gc2145 is available or not);
#endif
#endif

#endif

