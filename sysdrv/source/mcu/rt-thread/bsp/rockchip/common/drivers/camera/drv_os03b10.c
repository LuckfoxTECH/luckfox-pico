/**
  * Copyright (c) 2024 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_os03b10.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2024-02-20     Honglin Lan      first implementation
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

#ifdef RT_USING_OS03B10

//#define REG_LIST_CONVERSION_DEBUG
#define OS03B10_DEBUG_PRT      0

#if OS03B10_DEBUG_PRT
#include <stdio.h>
#define os03b10_dbg(fmt, args...)               rt_kprintf("[OS03B10] "fmt"", ##args)
#else
#define os03b10_dbg(fmt, args...)
#endif

#define os03b10_info(fmt, args...)              rt_kprintf("[OS03B10] "fmt"", ##args)
#define os03b10_err(fmt, args...)               rt_kprintf("[OS03B10] ERR "fmt"", ##args)

#define OS03B10_FETCH_2ND_BYTE_EXP(VAL)                         (((VAL) >> 8) & 0xFF)
#define OS03B10_FETCH_1ST_BYTE_EXP(VAL)                         ((VAL) & 0xFF)

#define OS03B10_XVCLK_FREQ_24M      24000000
#define MIPI_FREQ_270M          270000000

#define OS03B10_REG_PAGE_SEL        0xfd
#define OS03B10_REG_EXP_UPDATE      0x01
#define OS03B10_REG_BAYER_ORDER     0x5e

#define OS03B10_REG_PAGE_SYS_CTRL   0
#define OS03B10_REG_PAGE_CIS_TIMING 1
#define OS03B10_REG_PAGE_ISP_MIPI   2
#define OS03B10_REG_PAGE_DAC_CODE   3
#define OS03B10_REG_PAGE_DPC_SRAM   4
#define OS03B10_REG_PAGE_CIS_CTRL   5
#define OS03B10_REG_PAGE_OTP_CTRL   6
#define OS03B10_REG_PAGE_CIS_SRAM   7

#define CHIP_ID             0x5303
#define OS03B10_REG_CHIP_ID_H     0x02
#define OS03B10_REG_CHIP_ID_L       0x03

#define OS03B10_REG_CTRL_MODE       0xb1
#define OS03B10_MODE_SW_STANDBY     0x00
#define OS03B10_MODE_STREAMING      0x03

#define OS03B10_REG_EXPOSURE_H      0x03
#define OS03B10_REG_EXPOSURE_L      0x04

#define OS03B10_REG_DIG_GAIN_H      0x37
#define OS03B10_REG_DIG_GAIN_L      0x39
#define OS03B10_REG_ANA_GAIN        0x24

#define OS03B10_REG_VBLANK_H        0x05
#define OS03B10_REG_VBLANK_L        0x06

#define OS03B10_EXPOSURE_MIN        4
#define OS03B10_EXPOSURE_STEP       1
#define OS03B10_VTS_MAX         0x7fff

#define OS03B10_GAIN_MIN        0x10
#define OS03B10_GAIN_MAX        0x2000
#define OS03B10_GAIN_STEP       1
#define OS03B10_GAIN_DEFAULT        0x10

#define OS03B10_FETCH_EXP_H(VAL)    (((VAL) >> 8) & 0xFF)   /* 8 Bits */
#define OS03B10_FETCH_EXP_L(VAL)    ((VAL) & 0xFF)  /* 8 Bits */

#define OS03B10_FETCH_AGAIN(VAL)    ((VAL) & 0xFF)
#define OS03B10_FETCH_DGAIN_H(VAL)  (((VAL) >> 8) & 0xFF)
#define OS03B10_FETCH_DGAIN_L(VAL)  ((VAL) & 0xFF)

#define OS03B10_FLIP_REG        0x3f
#define OS03B10_FETCH_MIRROR(VAL, ENABLE)        (ENABLE ? VAL | 0x01 : VAL & 0xfe)
#define OS03B10_FETCH_FLIP(VAL, ENABLE)          (ENABLE ? VAL | 0x02 : VAL & 0xfd)

#define REG_END                     0xEF
#define REG_DELAY                   0xEE

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

struct os03b10_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct os03b10_mode
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
    const struct os03b10_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct os03b10_dev
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
    const struct os03b10_mode *cur_mode;
    struct os03b10_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct os03b10_dev *rt_os03b10_dev_t;

static struct os03b10_dev g_os03b10;

//#define REG_LIST_CONVERSION_DEBUG
#ifndef REG_LIST_CONVERSION_DEBUG

/*
 * Xclk 24Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 270Mbps, 2lane
 * raw 10
 * 1152X648_2x2_binning
 */
static const uint8_t g_os03b10_1152x648_60fps_reg_table[] =
{
    0x2, 0xfd, 0x00,
    0x2, 0x20, 0x00,
    0x2, 0xee, 0x05,
    0x2, 0xfd, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0x36, 0x01,
    0x2, 0xfd, 0x00,
    0x5, 0x2e, 0x2d, 0x01, 0x02, 0x90,
    0x2, 0x41, 0x12,
    0x2, 0x36, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0x44, 0x40,
    0x2, 0x38, 0x21,
    0x2, 0x45, 0x04,
    0x2, 0xfd, 0x01,
    0x3, 0x03, 0x00, 0x04,
    0x2, 0x06, 0x00,
    0x2, 0x24, 0xff,
    0x2, 0x01, 0x01,
    0x2, 0x18, 0x2f,
    0x2, 0x1a, 0x06,
    0x2, 0x19, 0x58,
    0x2, 0x1b, 0x3c,
    0x4, 0x2e, 0x03, 0x02, 0x5f,
    0x2, 0x3c, 0xca,
    0x2, 0x31, 0x0c,
    0x2, 0xfd, 0x03,
    0x2, 0x01, 0x0e,
    0x2, 0xfd, 0x01,
    0x3, 0x51, 0x0e, 0x04,
    0x2, 0x57, 0x0b,
    0x2, 0x5a, 0xdd,
    0x2, 0x66, 0xd0,
    0x2, 0x6e, 0x26,
    0x2, 0x71, 0x80,
    0x2, 0x73, 0x2b,
    0x4, 0xb8, 0x1c, 0x22, 0x22,
    0x2, 0xd0, 0x21,
    0x5, 0xd2, 0x8f, 0x1a, 0x20, 0x20,
    0x2, 0xfd, 0x01,
    0x2, 0xbd, 0x00,
    0x3, 0xd7, 0xad, 0xef,
    0x5, 0xe8, 0x0a, 0x05, 0x08, 0x07,
    0x2, 0xfd, 0x03,
    0x2, 0x00, 0x53,
    0x2, 0x03, 0xcd,
    0x7, 0x06, 0x07, 0x78, 0x36, 0x28, 0x0d, 0x06,
    0x2, 0x0f, 0x13,
    0x2, 0xfd, 0x01,
    0x5, 0x1d, 0x08, 0x18, 0x30, 0x5c,
    0x2, 0xbc, 0x00,
    0x2, 0xfd, 0x03,
    0x2, 0x02, 0x00,
    0x2, 0x05, 0x18,
    0x2, 0xfd, 0x02,
    0x2, 0x5e, 0x22,
    0x2, 0x34, 0x80,
    0x9, 0xa0, 0x00, 0x02, 0x02, 0x88, 0x00, 0x02, 0x04, 0x80,
    0x2, 0xfd, 0x01,
    0x5, 0xf0, 0x40, 0x40, 0x40, 0x40,
    0x3, 0xfa, 0x5c, 0x6b,
    0x3, 0xf6, 0x00, 0xc0,
    0x2, 0xfc, 0x00,
    0x2, 0xfe, 0xc0,
    0x5, 0xc4, 0x70, 0x70, 0x70, 0x70,
    0x2, 0xfd, 0x01,
    0x2, 0xb1, 0x01,
    0x2, 0xce, 0x5c,
    0x6, 0x8e, 0x04, 0x80, 0x02, 0x88, 0x19,
    0x3, 0x94, 0x44, 0x44,
    0x2, 0x98, 0x55,
    0x3, 0x9d, 0x03, 0x5f,
    0x2, 0xa1, 0x01,
    0x4, 0xa4, 0x13, 0xff, 0xff,
    0x2, 0xb1, 0x03,
    0x2, 0x01, 0x02,
    0x2, 0x14, 0x03,
    0x4e,
};

/*
 * Xclk 24Mhz
 * max_framerate 15fps
 * mipi_datarate per lane 270Mbps, 2lane
 * raw 10
 * 2304 x 1296
 */
static const uint8_t g_os03b10_2304x1296_30fps_reg_table[] =
{
    0x2, 0xfd, 0x00,
    0x2, 0x20, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0x36, 0x01,
    0x2, 0xfd, 0x00,
    0x3, 0x2e, 0x2d, 0x01,
    0x2, 0x41, 0x12,
    0x2, 0x36, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0xee, 0x05,
    0x2, 0xfd, 0x00,
    0x2, 0xfd, 0x00,
    0x2, 0x44, 0x40,
    0x2, 0x38, 0x21,
    0x2, 0x45, 0x04,
    0x2, 0xfd, 0x01,
    0x3, 0x03, 0x00, 0x04,
    0x2, 0x06, 0x00,
    0x2, 0x24, 0xff,
    0x2, 0x01, 0x01,
    0x2, 0x18, 0x2f,
    0x2, 0x1a, 0x06,
    0x2, 0x19, 0x58,
    0x2, 0x1b, 0x3c,
    0x4, 0x2e, 0x03, 0x02, 0x52,
    0x2, 0x3c, 0xca,
    0x2, 0xfd, 0x03,
    0x2, 0x01, 0x0e,
    0x2, 0xfd, 0x01,
    0x3, 0x51, 0x0e, 0x0b,
    0x2, 0x57, 0x0b,
    0x2, 0x5a, 0xe0,
    0x2, 0x66, 0xd0,
    0x2, 0x6e, 0x26,
    0x2, 0x71, 0x80,
    0x2, 0x73, 0x2b,
    0x2, 0xb8, 0x1c,
    0x2, 0xd0, 0x20,
    0x3, 0xd2, 0x8e, 0x1a,
    0x2, 0xfd, 0x01,
    0x2, 0xbd, 0x00,
    0x3, 0xd7, 0xbe, 0xef,
    0x5, 0xe8, 0x09, 0x05, 0x08, 0x06,
    0x2, 0xfd, 0x03,
    0x2, 0x00, 0x5c,
    0x2, 0x03, 0xcd,
    0x7, 0x06, 0x07, 0x78, 0x36, 0x28, 0x0c, 0x06,
    0x2, 0x0f, 0x13,
    0x2, 0xfd, 0x01,
    0x5, 0x1d, 0x08, 0x18, 0x30, 0x5c,
    0x2, 0xbc, 0x00,
    0x2, 0xfd, 0x03,
    0x2, 0x02, 0x00,
    0x2, 0x05, 0x18,
    0x2, 0xfd, 0x02,
    0x2, 0x5e, 0x22,
    0x2, 0x34, 0x80,
    0x2, 0xfd, 0x01,
    0x5, 0xf0, 0x40, 0x40, 0x40, 0x40,
    0x3, 0xfa, 0x5c, 0x6b,
    0x3, 0xf6, 0x00, 0xc0,
    0x2, 0xfc, 0x00,
    0x2, 0xfe, 0xc0,
    0x5, 0xc4, 0x70, 0x70, 0x70, 0x70,
    0x2, 0xfd, 0x01,
    0x2, 0xce, 0x7c,
    0x2, 0x8f, 0x00,
    0x3, 0x91, 0x10, 0x19,
    0x3, 0x94, 0x44, 0x44,
    0x2, 0x98, 0x55,
    0x3, 0x9d, 0x03, 0x5f,
    0x4, 0xa4, 0x13, 0xff, 0xff,
    0x2, 0x01, 0x02,
    0x2, 0x14, 0x03,
    0x4d,
};
#else
/*
 * Xclk 24Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 270Mbps, 2lane
 * raw 10
 * 1152X648_2x2_binning
 */
static const struct os03b10_sensor_reg g_os03b10_1152x648_60fps_reg_table[] =
{
    {0xfd, 0x00},
    {0x20, 0x00},
    {REG_DELAY, 0x05},
    {0xfd, 0x00},
    {0xfd, 0x00},
    {0x36, 0x01},
    {0xfd, 0x00},
    {0x2e, 0x2d},
    {0x2f, 0x01},
    {0x30, 0x02},
    {0x31, 0x90},
    {0x41, 0x12},
    {0x36, 0x00},
    {0xfd, 0x00},
    {0xfd, 0x00},
    {0x44, 0x40},
    {0x38, 0x21},
    {0x45, 0x04},
    {0xfd, 0x01},
    {0x03, 0x00},
    {0x04, 0x04},
    {0x06, 0x00},
    {0x24, 0xff},
    {0x01, 0x01},
    {0x18, 0x2f},
    {0x1a, 0x06},
    {0x19, 0x58},
    {0x1b, 0x3c},
    {0x2e, 0x03},
    {0x2f, 0x02},
    {0x30, 0x5f},
    {0x3c, 0xca},
    {0x31, 0x0c},
    {0xfd, 0x03},
    {0x01, 0x0e},
    {0xfd, 0x01},
    {0x51, 0x0e},
    {0x52, 0x04},
    {0x57, 0x0b},
    {0x5a, 0xdd},
    {0x66, 0xd0},
    {0x6e, 0x26},
    {0x71, 0x80},
    {0x73, 0x2b},
    {0xb8, 0x1c},
    {0xb9, 0x22},
    {0xba, 0x22},
    {0xd0, 0x21},
    {0xd2, 0x8f},
    {0xd3, 0x1a},
    {0xd4, 0x20},
    {0xd5, 0x20},
    {0xfd, 0x01},
    {0xbd, 0x00},
    {0xd7, 0xad},
    {0xd8, 0xef},
    {0xe8, 0x0a},
    {0xe9, 0x05},
    {0xea, 0x08},
    {0xeb, 0x07},
    {0xfd, 0x03},
    {0x00, 0x53},
    {0x03, 0xcd},
    {0x06, 0x07},
    {0x07, 0x78},
    {0x08, 0x36},
    {0x09, 0x28},
    {0x0a, 0x0d},
    {0x0b, 0x06},
    {0x0f, 0x13},
    {0xfd, 0x01},
    {0x1d, 0x08},
    {0x1e, 0x18},
    {0x1f, 0x30},
    {0x20, 0x5c},
    {0xbc, 0x00},
    {0xfd, 0x03},
    {0x02, 0x00},
    {0x05, 0x18},
    {0xfd, 0x02},
    {0x5e, 0x22},
    {0x34, 0x80},
    {0xa0, 0x00},
    {0xa1, 0x02},
    {0xa2, 0x02},
    {0xa3, 0x88},
    {0xa4, 0x00},
    {0xa5, 0x02},
    {0xa6, 0x04},
    {0xa7, 0x80},
    {0xfd, 0x01},
    {0xf0, 0x40},
    {0xf1, 0x40},
    {0xf2, 0x40},
    {0xf3, 0x40},
    {0xfa, 0x5c},
    {0xfb, 0x6b},
    {0xf6, 0x00},
    {0xf7, 0xc0},
    {0xfc, 0x00},
    {0xfe, 0xc0},
    {0xff, 0xcc},
    {0xc4, 0x70},
    {0xc5, 0x70},
    {0xc6, 0x70},
    {0xc7, 0x70},
    {0xfd, 0x01},
    {0xb1, 0x01},
    {0xce, 0x5c},
    {0x8e, 0x04},
    {0x8f, 0x80},
    {0x90, 0x02},
    {0x91, 0x88},
    {0x92, 0x19},
    {0x94, 0x44},
    {0x95, 0x44},
    {0x98, 0x55},
    {0x9d, 0x03},
    {0x9e, 0x5f},
    {0xa1, 0x01},
    {0xa4, 0x13},
    {0xa5, 0xff},
    {0xa6, 0xff},
    {0xb1, 0x03},
    {0x01, 0x02},
    {0x14, 0x03},
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 15fps
 * mipi_datarate per lane 270Mbps, 2lane
 * raw 10
 * 2304 x 1296
 */
static const struct os03b10_sensor_reg g_os03b10_2304x1296_30fps_reg_table[] =
{
    {0xfd, 0x00},
    {0x20, 0x00},
    {0xfd, 0x00},
    {0x36, 0x01},
    {0xfd, 0x00},
    {0x2e, 0x2d},
    {0x2f, 0x01},
    {0x41, 0x12},
    {0x36, 0x00},
    {0xfd, 0x00},
    {0xfd, 0x00},
    {0xfd, 0x00},
    {0xfd, 0x00},
    {REG_DELAY, 0x05},
    {0xfd, 0x00},
    {0xfd, 0x00},
    {0x44, 0x40},
    {0x38, 0x21},
    {0x45, 0x04},
    {0xfd, 0x01},
    {0x03, 0x00},
    {0x04, 0x04},
    {0x06, 0x00},
    {0x24, 0xff},
    {0x01, 0x01},
    {0x18, 0x2f},
    {0x1a, 0x06},
    {0x19, 0x58},
    {0x1b, 0x3c},
    {0x2e, 0x03},
    {0x2f, 0x02},
    {0x30, 0x52},
    {0x3c, 0xca},
    {0xfd, 0x03},
    {0x01, 0x0e},
    {0xfd, 0x01},
    {0x51, 0x0e},
    {0x52, 0x0b},
    {0x57, 0x0b},
    {0x5a, 0xe0},
    {0x66, 0xd0},
    {0x6e, 0x26},
    {0x71, 0x80},
    {0x73, 0x2b},
    {0xb8, 0x1c},
    {0xd0, 0x20},
    {0xd2, 0x8e},
    {0xd3, 0x1a},
    {0xfd, 0x01},
    {0xbd, 0x00},
    {0xd7, 0xbe},
    {0xd8, 0xef},
    {0xe8, 0x09},
    {0xe9, 0x05},
    {0xea, 0x08},
    {0xeb, 0x06},
    {0xfd, 0x03},
    {0x00, 0x5c},
    {0x03, 0xcd},
    {0x06, 0x07},
    {0x07, 0x78},
    {0x08, 0x36},
    {0x09, 0x28},
    {0x0a, 0x0c},
    {0x0b, 0x06},
    {0x0f, 0x13},
    {0xfd, 0x01},
    {0x1d, 0x08},
    {0x1e, 0x18},
    {0x1f, 0x30},
    {0x20, 0x5c},
    {0xbc, 0x00},
    {0xfd, 0x03},
    {0x02, 0x00},
    {0x05, 0x18},
    {0xfd, 0x02},
    {0x5e, 0x22},
    {0x34, 0x80},
    {0xfd, 0x01},
    {0xf0, 0x40},
    {0xf1, 0x40},
    {0xf2, 0x40},
    {0xf3, 0x40},
    {0xfa, 0x5c},
    {0xfb, 0x6b},
    {0xf6, 0x00},
    {0xf7, 0xc0},
    {0xfc, 0x00},
    {0xfe, 0xc0},
    {0xff, 0x88},
    {0xc4, 0x70},
    {0xc5, 0x70},
    {0xc6, 0x70},
    {0xc7, 0x70},
    {0xfd, 0x01},
    {0xce, 0x7c},
    {0x8f, 0x00},
    {0x91, 0x10},
    {0x92, 0x19},
    {0x94, 0x44},
    {0x95, 0x44},
    {0x98, 0x55},
    {0x9d, 0x03},
    {0x9e, 0x5f},
    {0xa4, 0x13},
    {0xa5, 0xff},
    {0xa6, 0xff},
    {0x01, 0x02},
    {0x14, 0x03},
    {REG_END, 0x00},
};
#endif

static const struct os03b10_mode supported_modes[] =
{
    [OS03B10_1152X648] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1152,
        .height = 648,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x044c,
        .hts_def = 0x054c,
        .vts_def = 0x0297,
        .xvclk = OS03B10_XVCLK_FREQ_24M,
        .link_freq = MIPI_FREQ_270M,
        .reg_list = g_os03b10_1152x648_60fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_os03b10_1152x648_60fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [OS03B10_2304X1296] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2304,
        .height = 1296,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x044c,
        .hts_def = 0x054e * 2,
        .vts_def = 0x052d,
        .xvclk = OS03B10_XVCLK_FREQ_24M,
        .link_freq = MIPI_FREQ_270M,
        .reg_list = g_os03b10_2304x1296_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_os03b10_2304x1296_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct os03b10_ops
{
    ret_err_t (*init)(struct os03b10_dev *dev);
    ret_err_t (*open)(struct os03b10_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct os03b10_dev *dev);
    ret_err_t (*control)(struct os03b10_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t os03b10_read_reg(struct os03b10_dev *dev,
                                  uint8_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[1];
    uint8_t recv_buf[1];
    int retry = 10;
    ret_err_t ret;

    RT_ASSERT(dev->i2c_bus != RT_NULL);

    send_buf[0] = (reg & 0xff);
    msg[0].addr = dev->i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 1;
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
            os03b10_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t os03b10_write_reg(struct os03b10_dev *dev,
                                   uint8_t reg, uint8_t data)
{
    uint8_t send_buf[2];
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(dev->i2c_bus != RT_NULL);

    send_buf[0] = (reg & 0xff);
    send_buf[1] = data;

    msg.addr = dev->i2c_addr;
    msg.flags = RT_I2C_WR;
    msg.buf = send_buf;
    msg.len = 2;

    while (1)
    {
        ret = rt_i2c_transfer(dev->i2c_bus, &msg, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            os03b10_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int os03b10_write_multiple_reg_continue(struct os03b10_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msg;
    int ret = 0;
    int offset = 0;
    int retry = 10;
    uint8_t addr;

    RT_ASSERT(dev != RT_NULL && i2c_data != RT_NULL);

    i2c_bus = dev->i2c_bus;
    RT_ASSERT(i2c_bus != RT_NULL);

    for (i = 0; i < i2c_data[len - 1];)
    {
        addr = i2c_data[offset + 1];
        if (addr != REG_DELAY)
        {
            msg.addr = dev->i2c_addr;
            msg.flags = RT_I2C_WR;
            msg.buf = (uint8_t *)&i2c_data[offset + 1];
            msg.len = i2c_data[offset];
            ret = rt_i2c_transfer(i2c_bus, &msg, 1);

            if (ret != 1)
            {
                os03b10_err("write multi-regs, retry=%d, addr=0x%02x\n", retry, i2c_data[offset + 1]);
                if (--retry)
                    continue;
                else
                    return RET_SYS_ERROR;
            }
        }
        else
        {
            os03b10_info("delay %d ms\n", i2c_data[offset + 2]);
            HAL_DelayUs(i2c_data[offset + 2] * 1000);
        }

        offset += (i2c_data[offset] + 1);
        retry = 10;
        i++;
    }

    return RET_SYS_EOK;
}

#else
static ret_err_t os03b10_write_reg_discontinued(struct os03b10_dev *dev,
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
            os03b10_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void os03b10_write_multiple_reg(struct os03b10_dev *dev,
                                       const struct os03b10_sensor_reg *reg, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    int k = 0;
    char *data = rt_malloc(len);
    uint8_t reg_addr;
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
                rt_kprintf("0x%x, ", k + 1);
                for (j = 0; j < k + 1; j++)
                    rt_kprintf("0x%02x, ", data[j]);
                rt_kprintf("\n");

                os03b10_write_reg_discontinued(i2c_bus, data, k + 1);
                k = 0;
            }
            break;
        }

        if (reg[i].reg_addr == REG_DELAY)
        {
            if (k > 0)
            {
                cnt++;
                rt_kprintf("0x%x, ", k + 1);
                for (j = 0; j < k + 1; j++)
                    rt_kprintf("0x%02x, ", data[j]);
                rt_kprintf("\n");

                os03b10_write_reg_discontinued(i2c_bus, data, k + 1);
                k = 0;
            }
            HAL_DelayUs(reg[i].val);
        }
        else
        {
            if (k == 0)
            {
                reg_addr = reg[i].reg_addr;
                data[0] = (reg_addr & 0xff);
                data[1] = reg[i].val;
                k++;
            }
            else
            {
                if ((reg[i - 1].reg_addr + 1) == reg[i].reg_addr)
                {
                    data[k + 2] = reg[i].val;
                    k++;
                }
                else
                {
                    cnt++;
                    rt_kprintf("0x%x, ", k + 1);
                    for (j = 0; j < k + 1; j++)
                        rt_kprintf("0x%02x, ", data[j]);
                    rt_kprintf("\n");

                    os03b10_write_reg_discontinued(i2c_bus, data, k + 1);
                    reg_addr = reg[i].reg_addr;
                    data[0] = (reg_addr & 0xff);
                    data[1] = reg[i].val;
                    k = 1;
                }
            }
        }
    }

    rt_kprintf("0x%x,\n", cnt);
}

static void rk_continuous_reg_dump(struct os03b10_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        os03b10_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        os03b10_write_multiple_reg((struct os03b10_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

static void os03b10_get_gain_reg(uint32_t total_gain, uint32_t *again, uint32_t *dgain)
{
    uint32_t step = 0;

    if (total_gain < 256)  /* 1x gain ~ 16x gain*/
    {
        *again = total_gain;
        *dgain = 0x40;
    }
    else if (total_gain < 512)    /* 16x gain ~ 32x gain */
    {
        step = (total_gain - 256) * 0x40 / 256;
        *again = 0xff;
        *dgain = 0x40 + step;
    }
    else if (total_gain < 1024)    /* 32x gain ~ 64x gain */
    {
        step = (total_gain - 512) * 0x80 / 512;
        *again = 0xff;
        *dgain = 0x80 + step;
    }
    else if (total_gain < 2048)    /* 64x gain ~ 128x gain */
    {
        step = (total_gain - 1024) * 0x100 / 1024;
        *again = 0xff;
        *dgain = 0x100 + step;
    }
    else if (total_gain < 4096)    /* 128x gain ~ 256x gain */
    {
        step = (total_gain - 2048) *  0x200 / 2048;
        *again = 0xff;
        *dgain = 0x200 + step;
    }
    else if (total_gain <= 8192)    /* 256x gain ~ 512x gain */
    {
        step = (total_gain - 4096) * 0x400 / 4096;
        *again = 0xff;
        *dgain = (0x400 + step) > 0x7ff ? 0x7ff : (0x400 + step);
    }
}

static ret_err_t rk_os03b10_set_expval(struct os03b10_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint32_t a_gain = 0, d_gain = 0;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        os03b10_dbg("os03b10 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    os03b10_dbg("os03b10: exp req: L_exp: 0x%x, 0x%x\n",
                l_exp_time, l_a_gain);

    ret  = os03b10_write_reg(dev,
                             OS03B10_REG_PAGE_SEL,
                             OS03B10_REG_PAGE_CIS_TIMING);
    /* 4 least significant bits of expsoure are fractional part */
    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_EXPOSURE_H,
                             (uint8_t)OS03B10_FETCH_EXP_H(l_exp_time));
    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_EXPOSURE_L,
                             (uint8_t)OS03B10_FETCH_EXP_L(l_exp_time));
    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_EXP_UPDATE,
                             0x01);

    os03b10_get_gain_reg(l_a_gain, &a_gain, &d_gain);

    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_PAGE_SEL,
                             OS03B10_REG_PAGE_CIS_TIMING);
    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_ANA_GAIN,
                             (uint8_t)OS03B10_FETCH_AGAIN(a_gain));
    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_DIG_GAIN_H,
                             (uint8_t)OS03B10_FETCH_DGAIN_H(d_gain));
    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_DIG_GAIN_L,
                             (uint8_t)OS03B10_FETCH_DGAIN_L(d_gain));
    ret |= os03b10_write_reg(dev,
                             OS03B10_REG_EXP_UPDATE,
                             0x01);

    if (ret == RET_SYS_EOK)
    {
        rt_mutex_release(&dev->mutex_lock);
        return RET_SYS_EOK;
    }
    else
    {
        os03b10_info("Warning: set gain failed with error (%d)!!!\n", ret);
        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }
}

static ret_err_t rk_os03b10_set_vts(struct os03b10_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint32_t vblank;

    RT_ASSERT(dev != RT_NULL);

    os03b10_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;
    vblank = dst_vts - dev->dst_mode->height;
    ret |= os03b10_write_reg(dev, OS03B10_REG_PAGE_SEL, OS03B10_REG_PAGE_CIS_TIMING);
    ret |= os03b10_write_reg(dev, OS03B10_REG_VBLANK_L, (uint8_t)(vblank & 0xff));
    ret |= os03b10_write_reg(dev, OS03B10_REG_VBLANK_H, (uint8_t)(vblank >> 8));
    ret |= os03b10_write_reg(dev, OS03B10_REG_EXP_UPDATE, 0x01);
    return ret;
}

static ret_err_t rk_os03b10_set_flip_mirror(struct os03b10_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = os03b10_write_reg(dev, OS03B10_REG_PAGE_SEL, OS03B10_REG_PAGE_CIS_TIMING);
    ret |= os03b10_read_reg(dev, OS03B10_FLIP_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = OS03B10_FETCH_MIRROR(val, flip & 0x01);
        val = OS03B10_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = OS03B10_FETCH_MIRROR(val, false);
        val = OS03B10_FETCH_FLIP(val, false);
        break;
    };
    os03b10_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret |= os03b10_write_reg(dev, OS03B10_FLIP_REG, val);
    ret |= os03b10_write_reg(dev, OS03B10_REG_PAGE_SEL, OS03B10_REG_PAGE_ISP_MIPI);
    ret |= os03b10_write_reg(dev, OS03B10_REG_BAYER_ORDER, 0x32);
    ret |= os03b10_write_reg(dev, OS03B10_REG_PAGE_SEL, OS03B10_REG_PAGE_CIS_TIMING);
    ret |= os03b10_write_reg(dev, OS03B10_REG_EXP_UPDATE, 0x01);

    return ret;
}

static void os03b10_stream_on(struct os03b10_dev *dev)
{
    os03b10_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(10);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
#ifndef REG_LIST_CONVERSION_DEBUG
    os03b10_write_multiple_reg_continue((struct os03b10_dev *)dev,
                                        dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_os03b10_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_os03b10_set_flip_mirror(dev, dev->flip);
    os03b10_write_reg(dev, OS03B10_REG_PAGE_SEL, OS03B10_REG_PAGE_SYS_CTRL);
    os03b10_write_reg(dev, OS03B10_REG_CTRL_MODE, OS03B10_MODE_STREAMING);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    os03b10_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void os03b10_stream_on_late(struct os03b10_dev *dev)
{
    os03b10_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_os03b10_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_os03b10_set_flip_mirror(dev, dev->flip);

    os03b10_write_reg(dev, OS03B10_REG_PAGE_SEL, OS03B10_REG_PAGE_SYS_CTRL);
    os03b10_write_reg(dev, OS03B10_REG_CTRL_MODE, OS03B10_MODE_STREAMING);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    os03b10_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void os03b10_stream_off(struct os03b10_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        //os03b10_write_reg(dev, OS03B10_REG_PAGE_SEL, OS03B10_REG_PAGE_SYS_CTRL);
        os03b10_write_reg(dev, OS03B10_REG_CTRL_MODE, OS03B10_MODE_SW_STANDBY);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        os03b10_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_os03b10_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os03b10_dev *os03b10;

    os03b10 = (struct os03b10_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&os03b10->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(os03b10);
#endif

    if (os03b10)
    {
        camera->info.mbus_fmt.width = os03b10->cur_mode->width;
        camera->info.mbus_fmt.height = os03b10->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = os03b10->cur_mode->bus_fmt;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = os03b10->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  os03b10->cur_mode->hdr_mode;

        os03b10->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(os03b10->i2c_name);

        if (!os03b10->i2c_bus)
        {
            os03b10_err("not find i2c :%s !!!\n",
                        os03b10->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            os03b10_dbg("find i2c_bus:%s\n", os03b10->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_os03b10_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_os03b10_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_os03b10_get_expinf(struct os03b10_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os03b10_mode *mode;

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

static ret_err_t rk_os03b10_get_intput_fmt(struct os03b10_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os03b10_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = mode->bus_fmt;

    return ret;
}

static ret_err_t rk_os03b10_set_intput_fmt(struct os03b10_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os03b10_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    os03b10_info("dst resulotion, width %d, height %d\n",
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
            os03b10_write_multiple_reg_continue((struct os03b10_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_os03b10_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_os03b10_match_dst_config(struct os03b10_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct os03b10_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    os03b10_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct os03b10_mode *)mode;
            ret = RET_SYS_EOK;
            os03b10_info("find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            os03b10_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        os03b10_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_os03b10_control(struct rk_camera_device *dev,
                             dt_cmd_t cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os03b10_dev *os03b10;

    RT_ASSERT(dev != RT_NULL);
    os03b10 = (struct os03b10_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_os03b10_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        os03b10_stream_on(os03b10);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        os03b10_stream_off(os03b10);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_os03b10_get_expinf(os03b10, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_os03b10_set_expval(os03b10, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_os03b10_set_vts(os03b10, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_os03b10_get_intput_fmt(os03b10, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_os03b10_set_intput_fmt(os03b10, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_os03b10_match_dst_config(os03b10, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_os03b10_set_flip_mirror(os03b10, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        os03b10_stream_on_late(os03b10);
    }
    break;
    default:
        os03b10_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_os03b10_ops =
{
    rk_os03b10_init,
    rk_os03b10_open,
    NULL,
    rk_os03b10_control
};

int rk_camera_os03b10_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os03b10_dev *instance = &g_os03b10;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *os03b10 = (struct camera_board_desc *)&camera_os03b10;

    camera->ops = &rk_os03b10_ops;

    rt_strncpy(instance->name, os03b10->isp_subdev_name, rt_strlen(os03b10->isp_subdev_name));
    rt_strncpy(instance->i2c_name, os03b10->i2c_bus, rt_strlen(os03b10->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (os03b10->mode_id <= OS03B10_MODE_ID_MAX)
    {
        os03b10_dbg("mode_id: %d\n", os03b10->mode_id);
        instance->cur_mode = &supported_modes[os03b10->mode_id];
    }
    else
    {
        os03b10_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct os03b10_mode *)&supported_modes[OS03B10_2304X1296];
    instance->time_valid_delay = os03b10->time_valid_delay;
    instance->gain_valid_delay = os03b10->gain_valid_delay;
    instance->i2c_addr = os03b10->i2c_addr;
    instance->flip = 0;

    HAL_DelayUs(5 * 1000);
    if (os03b10->rst_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)os03b10->rst_gpio.gpio_group, os03b10->rst_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)os03b10->rst_gpio.gpio_group, os03b10->rst_gpio.gpio_pin, !os03b10->rst_active);
    }
    if (os03b10->pwdn_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)os03b10->pwdn_gpio.gpio_group, os03b10->pwdn_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)os03b10->pwdn_gpio.gpio_group, os03b10->pwdn_gpio.gpio_pin, !os03b10->pwdn_active);
    }

    if (os03b10->mclk_out_gate_id && os03b10->mclk_id)
    {
        clk_set_rate(os03b10->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(os03b10->mclk_out_gate_id);
    }

    HAL_DelayUs(5 * 1000);
    rt_mutex_init(&instance->mutex_lock, "OS03B10_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, os03b10->isp_subdev_name, rt_strlen(os03b10->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void os03b10_detect(void)
{
    struct os03b10_dev *instance = &g_os03b10;

    os03b10_dbg("start to detect OS03B10 for testing \n");
    os03b10_dbg("dev name:%s\n", instance->name);
    os03b10_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        os03b10_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        os03b10_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_os03b10_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(os03b10_detect, check OS03B10 is available or not);
#endif
#endif
#endif
