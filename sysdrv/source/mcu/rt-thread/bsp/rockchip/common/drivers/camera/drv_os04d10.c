/**
  * Copyright (c) 2023 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_os04d10.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2023-12-19     Honglin Lan      first implementation
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

#ifdef RT_USING_OS04D10

//#define REG_LIST_CONVERSION_DEBUG
#define OS04D10_DEBUG_PRT      0

#if OS04D10_DEBUG_PRT
#include <stdio.h>
#define os04d10_dbg(fmt, args...)               rt_kprintf("[OS04D10] "fmt"", ##args)
#else
#define os04d10_dbg(fmt, args...)
#endif

#define os04d10_info(fmt, args...)              rt_kprintf("[OS04D10] "fmt"", ##args)
#define os04d10_err(fmt, args...)               rt_kprintf("[OS04D10] ERR "fmt"", ##args)

#define OS04D10_FETCH_2ND_BYTE_EXP(VAL)                         (((VAL) >> 8) & 0xFF)
#define OS04D10_FETCH_1ST_BYTE_EXP(VAL)                         ((VAL) & 0xFF)

#define OS04D10_XVCLK_FREQ      360000000

#define OS04D10_REG_PAGE_SEL        0xfd
#define OS04D10_REG_EXP_UPDATE      0x01

#define OS04D10_REG_PAGE_SYS_CTRL   0
#define OS04D10_REG_PAGE_CIS_TIMING 1
#define OS04D10_REG_PAGE_ISP_MIPI   2
#define OS04D10_REG_PAGE_DAC_CODE   3
#define OS04D10_REG_PAGE_DPC_SRAM   4
#define OS04D10_REG_PAGE_CIS_CTRL   5
#define OS04D10_REG_PAGE_OTP_CTRL   6
#define OS04D10_REG_PAGE_CIS_SRAM   7

#define CHIP_ID             0x53044410
#define OS04D10_REG_CHIP_ID     0x02

#define OS04D10_REG_CTRL_MODE       0x20
#define OS04D10_MODE_SW_STANDBY     0x01
#define OS04D10_MODE_STREAMING      0x03

#define OS04D10_REG_EXPOSURE_H      0x03
#define OS04D10_REG_EXPOSURE_L      0x04

#define OS04D10_REG_DIG_GAIN_H      0x37
#define OS04D10_REG_DIG_GAIN_L      0x39
#define OS04D10_REG_ANA_GAIN        0x24

#define OS04D10_REG_VBLANK_H        0x05
#define OS04D10_REG_VBLANK_L        0x06

#define OS04D10_EXPOSURE_MIN        1
#define OS04D10_EXPOSURE_STEP       1
#define OS04D10_VTS_MAX         0x7fff

#define OS04D10_GAIN_MIN        0x40
#define OS04D10_GAIN_MAX        (31744) //15.5 * 32 * 64
#define OS04D10_GAIN_STEP       1
#define OS04D10_GAIN_DEFAULT        0x40

#define OS04D10_FETCH_EXP_H(VAL)    (((VAL) >> 8) & 0xFF)   /* 8 Bits */
#define OS04D10_FETCH_EXP_L(VAL)    ((VAL) & 0xFF)  /* 8 Bits */

#define OS04D10_FLIP_REG        0x32
#define OS04D10_FETCH_MIRROR(VAL, ENABLE)        (ENABLE ? VAL | 0x01 : VAL & 0xfe)
#define OS04D10_FETCH_FLIP(VAL, ENABLE)          (ENABLE ? VAL | 0x02 : VAL & 0xfd)

#define REG_END                     0xFF
#define REG_DELAY                   0xFE

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

struct os04d10_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct os04d10_mode
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
    const struct os04d10_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct os04d10_dev
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
    const struct os04d10_mode *cur_mode;
    struct os04d10_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct os04d10_dev *rt_os04d10_dev_t;

static struct os04d10_dev g_os04d10;

//#define REG_LIST_CONVERSION_DEBUG
#ifndef REG_LIST_CONVERSION_DEBUG

/*
 * Xclk 24Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 720Mbps, 2lane
 * raw 10
 * 640X360_4x4_binning
 */
static const uint8_t g_os04d10_640x360_120fps_reg_table[] =
{
    0x2, 0xfd, 0x00,
    0x2, 0x20, 0x00,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x41, 0xa8,
    0x2, 0x45, 0x24,
    0x3, 0x30, 0x02, 0x24,
    0x2, 0x35, 0xc9,
    0x2, 0x38, 0x15,
    0x2, 0xfd, 0x01,
    0x3, 0x03, 0x00, 0x04,
    0x2, 0x06, 0x01,
    0x2, 0x24, 0xff,
    0x2, 0x31, 0x26,
    0x2, 0x02, 0x01,
    0x2, 0x42, 0x5a,
    0x2, 0x47, 0x0c,
    0x2, 0x45, 0x02,
    0x2, 0x48, 0x0c,
    0x2, 0x4b, 0x88,
    0x3, 0xd4, 0x05, 0xd2,
    0x3, 0xd7, 0x05, 0xd2,
    0x7, 0x50, 0x01, 0x11, 0x18, 0x01, 0x01, 0x01,
    0x2, 0x57, 0x08,
    0x2, 0x5c, 0x40,
    0x5, 0x7c, 0x06, 0x05, 0x05, 0x05,
    0x7, 0x90, 0x60, 0x0f, 0x35, 0x36, 0x0f, 0x7e,
    0x2, 0x98, 0x5d,
    0x2, 0xa8, 0x50,
    0x7, 0xaa, 0x14, 0x05, 0x14, 0x05, 0x4a, 0x0e,
    0x3, 0xb2, 0x07, 0x0c,
    0xc, 0xc9, 0x28, 0x5e, 0x5e, 0x5e, 0x5e, 0x5c, 0x5c, 0x5c, 0x5c, 0x7c, 0x7c,
    0x2, 0xdb, 0x0f,
    0x2, 0xfd, 0x01,
    0x2, 0x46, 0x77,
    0x3, 0xdd, 0x00, 0x3f,
    0x2, 0xfd, 0x03,
    0x2, 0x2b, 0x0a,
    0x3,  0x01,  0x22,  0x03,
    0x2, 0x00, 0x06,
    0x2, 0x2a, 0x22,
    0x2, 0x29, 0x0b,
    0x3, 0x1e, 0x10, 0x02,
    0x5, 0x1a, 0x24, 0x62, 0xce, 0xd3,
    0x2, 0x04, 0x0f,
    0xa, 0x36, 0x00, 0x05, 0x09, 0x19, 0x38, 0x22, 0x22, 0x22, 0x03,
    0x2, 0xfd, 0x02,
    0x2, 0xc1, 0x05,
    0x3, 0x8c, 0x03, 0x01,
    0x2, 0x95, 0x02,
    0x2, 0x98, 0x02,
    0x2, 0x5e, 0x22,
    0x4, 0xa1, 0x00, 0x01, 0x68,
    0x4, 0xa5, 0x02, 0x02, 0x80,
    0x5, 0x8e, 0x02, 0x80, 0x01, 0x68,
    0x2, 0xce, 0x65,
    0x2, 0xfd, 0x03,
    0x2, 0x03, 0x30,
    0x2, 0x05, 0x00,
    0x3, 0x12, 0x70, 0x70,
    0x2, 0x16, 0x13,
    0x2, 0x21, 0xca,
    0x2, 0x27, 0x95,
    0x4, 0x2c, 0x55, 0x08, 0xca,
    0x2, 0x3f, 0xe7,
    0x2, 0xfd, 0x00,
    0x2, 0x8b, 0x01,
    0x2, 0x8d, 0x00,
    0x2, 0xfd, 0x01,
    0x2, 0x01, 0x02,
    0x2, 0xfd, 0x05,
    0x5, 0xc4, 0x62, 0x62, 0x62, 0x62,
    0x2, 0xce, 0x3e,
    0x6, 0xf0, 0x40, 0x40, 0x40, 0x40, 0x00,
    0x4, 0xf9, 0x03, 0x5d, 0x6b,
    0x2, 0xb1, 0x01,
    0x4e,
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 720Mbps, 2lane
 * raw 10
 * 2560 x 1440
 */
static const uint8_t g_os04d10_2560x1440_30fps_reg_table[] =
{
    0x2, 0xfd, 0x00,
    0x2, 0x20, 0x00,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x41, 0xa8,
    0x2, 0x45, 0x24,
    0x2, 0x31, 0x20,
    0x2, 0x38, 0x15,
    0x2, 0xfd, 0x01,
    0x3, 0x03, 0x00, 0x04,
    0x2, 0x06, 0x01,
    0x2, 0x24, 0xff,
    0x2, 0x02, 0x01,
    0x2, 0x42, 0x5a,
    0x2, 0x47, 0x0c,
    0x2, 0x45, 0x02,
    0x2, 0x48, 0x0c,
    0x2, 0x4b, 0x88,
    0x3, 0xd4, 0x05, 0xd2,
    0x3, 0xd7, 0x05, 0xd2,
    0x7, 0x50, 0x01, 0x11, 0x18, 0x01, 0x01, 0x01,
    0x2, 0x57, 0x08,
    0x2, 0x5c, 0x40,
    0x5, 0x7c, 0x06, 0x05, 0x05, 0x05,
    0x7, 0x90, 0x60, 0x0f, 0x35, 0x36, 0x0f, 0x7e,
    0x2, 0x98, 0x5d,
    0x2, 0xa8, 0x50,
    0x7, 0xaa, 0x14, 0x05, 0x14, 0x05, 0x4a, 0x0e,
    0x3, 0xb2, 0x07, 0x0c,
    0xc, 0xc9, 0x28, 0x5e, 0x5e, 0x5e, 0x5e, 0x5c, 0x5c, 0x5c, 0x5c, 0x7c, 0x7c,
    0x2, 0xdb, 0x0f,
    0x2, 0xfd, 0x01,
    0x2, 0x46, 0x77,
    0x3, 0xdd, 0x00, 0x3f,
    0x2, 0xfd, 0x03,
    0x2, 0x2b, 0x0a,
    0x3,  0x01,  0x22,  0x03,
    0x2, 0x00, 0x06,
    0x2, 0x2a, 0x22,
    0x2, 0x29, 0x0b,
    0x3, 0x1e, 0x10, 0x02,
    0x5, 0x1a, 0x24, 0x62, 0xce, 0xd3,
    0x2, 0x04, 0x0f,
    0xa, 0x36, 0x00, 0x05, 0x09, 0x19, 0x38, 0x22, 0x22, 0x22, 0x03,
    0x2, 0xfd, 0x02,
    0x2, 0x5e, 0x22,
    0x4, 0xa1, 0x04, 0x05, 0xa0,
    0x4, 0xa5, 0x04, 0x0a, 0x00,
    0x5, 0x8e, 0x0a, 0x00, 0x05, 0xa0,
    0x2, 0xce, 0x65,
    0x2, 0xfd, 0x03,
    0x2, 0x03, 0x30,
    0x2, 0x05, 0x00,
    0x3, 0x12, 0x70, 0x70,
    0x2, 0x16, 0x13,
    0x2, 0x21, 0xca,
    0x2, 0x27, 0x95,
    0x4, 0x2c, 0x55, 0x08, 0xca,
    0x2, 0x3f, 0xe7,
    0x2, 0xfd, 0x00,
    0x2, 0x8b, 0x01,
    0x2, 0x8d, 0x00,
    0x2, 0xfd, 0x01,
    0x2, 0x01, 0x02,
    0x2, 0xfd, 0x05,
    0x5, 0xc4, 0x62, 0x62, 0x62, 0x62,
    0x6, 0xf0, 0x40, 0x40, 0x40, 0x40, 0x00,
    0x4, 0xf9, 0x03, 0x5d, 0x6b,
    0x2, 0xb1, 0x01,
    0x2, 0xfd, 0x02,
    0x2, 0x5e, 0x32,
    0x49,
};

/*
 * Xclk 24Mhz
 * max_framerate 15fps
 * mipi_datarate per lane 720Mbps, 2lane
 * raw 10
 * 2568 x 1448
 */
static const uint8_t g_os04d10_2568x1448_15fps_reg_table[] =
{
    0x2, 0xfd, 0x00,
    0x2, 0x20, 0x00,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x20, 0x01,
    0x2, 0x41, 0xa8,
    0x2, 0x45, 0x24,
    0x2, 0x31, 0x20,
    0x2, 0x38, 0x15,
    0x2, 0xfd, 0x01,
    0x5, 0x03, 0x00, 0x04, 0x05, 0xc3,
    0x2, 0x24, 0xff,
    0x2, 0x02, 0x01,
    0x2, 0x42, 0x5a,
    0x2, 0x47, 0x0c,
    0x2, 0x45, 0x02,
    0x2, 0x48, 0x0c,
    0x2, 0x4b, 0x88,
    0x3, 0xd4, 0x05, 0xd2,
    0x3, 0xd7, 0x05, 0xd2,
    0x7, 0x50, 0x01, 0x11, 0x18, 0x01, 0x01, 0x01,
    0x2, 0x57, 0x08,
    0x2, 0x5c, 0x40,
    0x5, 0x7c, 0x06, 0x05, 0x05, 0x05,
    0x7, 0x90, 0x60, 0x0f, 0x35, 0x36, 0x0f, 0x7e,
    0x2, 0x98, 0x5d,
    0x2, 0xa8, 0x50,
    0x7, 0xaa, 0x14, 0x05, 0x14, 0x05, 0x4a, 0x0e,
    0x3, 0xb2, 0x07, 0x0c,
    0xc, 0xc9, 0x28, 0x5e, 0x5e, 0x5e, 0x5e, 0x5c, 0x5c, 0x5c, 0x5c, 0x7c, 0x7c,
    0x2, 0xdb, 0x0f,
    0x2, 0xfd, 0x01,
    0x2, 0x46, 0x77,
    0x3, 0xdd, 0x00, 0x3f,
    0x2, 0xfd, 0x03,
    0x2, 0x2b, 0x0a,
    0x3,  0x01,  0x22,  0x03,
    0x2, 0x00, 0x06,
    0x2, 0x2a, 0x22,
    0x2, 0x29, 0x0b,
    0x3, 0x1e, 0x10, 0x02,
    0x5, 0x1a, 0x24, 0x62, 0xce, 0xd3,
    0x2, 0x04, 0x0f,
    0xa, 0x36, 0x00, 0x05, 0x09, 0x19, 0x38, 0x22, 0x22, 0x22, 0x03,
    0x2, 0xfd, 0x02,
    0x2, 0xce, 0x65,
    0x2, 0xfd, 0x03,
    0x2, 0x03, 0x30,
    0x2, 0x05, 0x00,
    0x3, 0x12, 0x70, 0x70,
    0x2, 0x16, 0x13,
    0x2, 0x21, 0xca,
    0x2, 0x27, 0x95,
    0x4, 0x2c, 0x55, 0x08, 0xca,
    0x2, 0x3f, 0xe7,
    0x2, 0xfd, 0x00,
    0x2, 0x8b, 0x01,
    0x2, 0x8d, 0x00,
    0x2, 0xfd, 0x01,
    0x2, 0x01, 0x02,
    0x2, 0xfd, 0x05,
    0x5, 0xc4, 0x62, 0x62, 0x62, 0x62,
    0x6, 0xf0, 0x40, 0x40, 0x40, 0x40, 0x00,
    0x4, 0xf9, 0x03, 0x5d, 0x6b,
    0x2, 0xb1, 0x01,
    0x2, 0xfd, 0x02,
    0x2, 0x5e, 0x32,
    0x44,
};
#else
/*
 * Xclk 24Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 205Mbps, 2lane
 * 4X4 binning to 640X360
 */
static const struct os04d10_sensor_reg g_os04d10_640x360_120fps_reg_table[] =
{
    {0xfd, 0x00},
    {0x20, 0x00},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x41, 0xa8},
    {0x45, 0x24},
    {0x30, 0x02},
    {0x31, 0x24},
    {0x35, 0xc9},
    {0x38, 0x15},
    {0xfd, 0x01},
    {0x03, 0x00},
    {0x04, 0x04},
    {0x06, 0x01},
    {0x24, 0xff},
    {0x31, 0x26},
    {0x02, 0x01},
    {0x42, 0x5a},
    {0x47, 0x0c},
    {0x45, 0x02},
    {0x48, 0x0c},
    {0x4b, 0x88},
    {0xd4, 0x05},
    {0xd5, 0xd2},
    {0xd7, 0x05},
    {0xd8, 0xd2},
    {0x50, 0x01},
    {0x51, 0x11},
    {0x52, 0x18},
    {0x53, 0x01},
    {0x54, 0x01},
    {0x55, 0x01},
    {0x57, 0x08},
    {0x5c, 0x40},
    {0x7c, 0x06},
    {0x7d, 0x05},
    {0x7e, 0x05},
    {0x7f, 0x05},
    {0x90, 0x60},
    {0x91, 0x0f},
    {0x92, 0x35},
    {0x93, 0x36},
    {0x94, 0x0f},
    {0x95, 0x7e},
    {0x98, 0x5d},
    {0xa8, 0x50},
    {0xaa, 0x14},
    {0xab, 0x05},
    {0xac, 0x14},
    {0xad, 0x05},
    {0xae, 0x4a},
    {0xaf, 0x0e},
    {0xb2, 0x07},
    {0xb3, 0x0c},
    {0xc9, 0x28},
    {0xca, 0x5e},
    {0xcb, 0x5e},
    {0xcc, 0x5e},
    {0xcd, 0x5e},
    {0xce, 0x5c},
    {0xcf, 0x5c},
    {0xd0, 0x5c},
    {0xd1, 0x5c},
    {0xd2, 0x7c},
    {0xd3, 0x7c},
    {0xdb, 0x0f},
    {0xfd, 0x01},
    {0x46, 0x77},
    {0xdd, 0x00},
    {0xde, 0x3f},
    {0xfd, 0x03},
    {0x2b, 0x0a},
    {0x01, 0x22},
    {0x02, 0x03},
    {0x00, 0x06},
    {0x2a, 0x22},
    {0x29, 0x0b},
    {0x1e, 0x10},
    {0x1f, 0x02},
    {0x1a, 0x24},
    {0x1b, 0x62},
    {0x1c, 0xce},
    {0x1d, 0xd3},
    {0x04, 0x0f},
    {0x36, 0x00},
    {0x37, 0x05},
    {0x38, 0x09},
    {0x39, 0x19},
    {0x3a, 0x38},
    {0x3b, 0x22},
    {0x3c, 0x22},
    {0x3d, 0x22},
    {0x3e, 0x03},
    {0xfd, 0x02},
    {0xc1, 0x05},
    {0x8c, 0x03},
    {0x8d, 0x01},
    {0x95, 0x02},
    {0x98, 0x02},
    {0x5e, 0x22},
    {0xa1, 0x00},
    {0xa2, 0x01},
    {0xa3, 0x68},
    {0xa5, 0x02},
    {0xa6, 0x02},
    {0xa7, 0x80},
    {0x8e, 0x02},
    {0x8f, 0x80},
    {0x90, 0x01},
    {0x91, 0x68},
    {0xce, 0x65},
    {0xfd, 0x03},
    {0x03, 0x30},
    {0x05, 0x00},
    {0x12, 0x70},
    {0x13, 0x70},
    {0x16, 0x13},
    {0x21, 0xca},
    {0x27, 0x95},
    {0x2c, 0x55},
    {0x2d, 0x08},
    {0x2e, 0xca},
    {0x3f, 0xe7},
    {0xfd, 0x00},
    {0x8b, 0x01},
    {0x8d, 0x00},
    {0xfd, 0x01},
    {0x01, 0x02},
    {0xfd, 0x05},
    {0xc4, 0x62},
    {0xc5, 0x62},
    {0xc6, 0x62},
    {0xc7, 0x62},
    {0xce, 0x3e},
    {0xf0, 0x40},
    {0xf1, 0x40},
    {0xf2, 0x40},
    {0xf3, 0x40},
    {0xf4, 0x00},
    {0xf9, 0x03},
    {0xfa, 0x5d},
    {0xfb, 0x6b},
    {0xb1, 0x01},
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 720Mbps, 2lane
 * raw 10
 * 2560 x 1440
 */
static const struct os04d10_sensor_reg g_os04d10_2560x1440_30fps_reg_table[] =
{
    {0xfd, 0x00},
    {0x20, 0x00},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x41, 0xa8},
    {0x45, 0x24},
    {0x31, 0x20},
    {0x38, 0x15},
    {0xfd, 0x01},
    {0x03, 0x00},
    {0x04, 0x04},
    {0x06, 0x01},
    {0x24, 0xff},
    {0x02, 0x01},
    {0x42, 0x5a},
    {0x47, 0x0c},
    {0x45, 0x02},
    {0x48, 0x0c},
    {0x4b, 0x88},
    {0xd4, 0x05},
    {0xd5, 0xd2},
    {0xd7, 0x05},
    {0xd8, 0xd2},
    {0x50, 0x01},
    {0x51, 0x11},
    {0x52, 0x18},
    {0x53, 0x01},
    {0x54, 0x01},
    {0x55, 0x01},
    {0x57, 0x08},
    {0x5c, 0x40},
    {0x7c, 0x06},
    {0x7d, 0x05},
    {0x7e, 0x05},
    {0x7f, 0x05},
    {0x90, 0x60},
    {0x91, 0x0f},
    {0x92, 0x35},
    {0x93, 0x36},
    {0x94, 0x0f},
    {0x95, 0x7e},
    {0x98, 0x5d},
    {0xa8, 0x50},
    {0xaa, 0x14},
    {0xab, 0x05},
    {0xac, 0x14},
    {0xad, 0x05},
    {0xae, 0x4a},
    {0xaf, 0x0e},
    {0xb2, 0x07},
    {0xb3, 0x0c},
    {0xc9, 0x28},
    {0xca, 0x5e},
    {0xcb, 0x5e},
    {0xcc, 0x5e},
    {0xcd, 0x5e},
    {0xce, 0x5c},
    {0xcf, 0x5c},
    {0xd0, 0x5c},
    {0xd1, 0x5c},
    {0xd2, 0x7c},
    {0xd3, 0x7c},
    {0xdb, 0x0f},
    {0xfd, 0x01},
    {0x46, 0x77},
    {0xdd, 0x00},
    {0xde, 0x3f},
    {0xfd, 0x03},
    {0x2b, 0x0a},
    {0x01, 0x22},
    {0x02, 0x03},
    {0x00, 0x06},
    {0x2a, 0x22},
    {0x29, 0x0b},
    {0x1e, 0x10},
    {0x1f, 0x02},
    {0x1a, 0x24},
    {0x1b, 0x62},
    {0x1c, 0xce},
    {0x1d, 0xd3},
    {0x04, 0x0f},
    {0x36, 0x00},
    {0x37, 0x05},
    {0x38, 0x09},
    {0x39, 0x19},
    {0x3a, 0x38},
    {0x3b, 0x22},
    {0x3c, 0x22},
    {0x3d, 0x22},
    {0x3e, 0x03},
    {0xfd, 0x02},
    {0x5e, 0x22},
    {0xa1, 0x04},
    {0xa2, 0x05},
    {0xa3, 0xa0},
    {0xa5, 0x04},
    {0xa6, 0x0a},
    {0xa7, 0x00},
    {0x8e, 0x0a},
    {0x8f, 0x00},
    {0x90, 0x05},
    {0x91, 0xa0},
    {0xce, 0x65},
    {0xfd, 0x03},
    {0x03, 0x30},
    {0x05, 0x00},
    {0x12, 0x70},
    {0x13, 0x70},
    {0x16, 0x13},
    {0x21, 0xca},
    {0x27, 0x95},
    {0x2c, 0x55},
    {0x2d, 0x08},
    {0x2e, 0xca},
    {0x3f, 0xe7},
    {0xfd, 0x00},
    {0x8b, 0x01},
    {0x8d, 0x00},
    {0xfd, 0x01},
    {0x01, 0x02},
    {0xfd, 0x05},
    {0xc4, 0x62},
    {0xc5, 0x62},
    {0xc6, 0x62},
    {0xc7, 0x62},
    {0xf0, 0x40},
    {0xf1, 0x40},
    {0xf2, 0x40},
    {0xf3, 0x40},
    {0xf4, 0x00},
    {0xf9, 0x03},
    {0xfa, 0x5d},
    {0xfb, 0x6b},
    {0xb1, 0x01},
    {0xfd, 0x02},
    {0x5e, 0x32}
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 15fps
 * mipi_datarate per lane 720Mbps, 2lane
 * full resolution, 2568x1448
 */
static const struct os04d10_sensor_reg g_os04d10_2568x1448_15fps_reg_table[] =
{
    {0xfd, 0x00},
    {0x20, 0x00},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x20, 0x01},
    {0x41, 0xa8},
    {0x45, 0x24},
    {0x31, 0x20},
    {0x38, 0x15},
    {0xfd, 0x01},
    {0x03, 0x00},
    {0x04, 0x04},
    {0x05, 0x05},
    {0x06, 0xc3},
    {0x24, 0xff},
    {0x02, 0x01},
    {0x42, 0x5a},
    {0x47, 0x0c},
    {0x45, 0x02},
    {0x48, 0x0c},
    {0x4b, 0x88},
    {0xd4, 0x05},
    {0xd5, 0xd2},
    {0xd7, 0x05},
    {0xd8, 0xd2},
    {0x50, 0x01},
    {0x51, 0x11},
    {0x52, 0x18},
    {0x53, 0x01},
    {0x54, 0x01},
    {0x55, 0x01},
    {0x57, 0x08},
    {0x5c, 0x40},
    {0x7c, 0x06},
    {0x7d, 0x05},
    {0x7e, 0x05},
    {0x7f, 0x05},
    {0x90, 0x60},
    {0x91, 0x0f},
    {0x92, 0x35},
    {0x93, 0x36},
    {0x94, 0x0f},
    {0x95, 0x7e},
    {0x98, 0x5d},
    {0xa8, 0x50},
    {0xaa, 0x14},
    {0xab, 0x05},
    {0xac, 0x14},
    {0xad, 0x05},
    {0xae, 0x4a},
    {0xaf, 0x0e},
    {0xb2, 0x07},
    {0xb3, 0x0c},
    {0xc9, 0x28},
    {0xca, 0x5e},
    {0xcb, 0x5e},
    {0xcc, 0x5e},
    {0xcd, 0x5e},
    {0xce, 0x5c},
    {0xcf, 0x5c},
    {0xd0, 0x5c},
    {0xd1, 0x5c},
    {0xd2, 0x7c},
    {0xd3, 0x7c},
    {0xdb, 0x0f},
    {0xfd, 0x01},
    {0x46, 0x77},
    {0xdd, 0x00},
    {0xde, 0x3f},
    {0xfd, 0x03},
    {0x2b, 0x0a},
    {0x01, 0x22},
    {0x02, 0x03},
    {0x00, 0x06},
    {0x2a, 0x22},
    {0x29, 0x0b},
    {0x1e, 0x10},
    {0x1f, 0x02},
    {0x1a, 0x24},
    {0x1b, 0x62},
    {0x1c, 0xce},
    {0x1d, 0xd3},
    {0x04, 0x0f},
    {0x36, 0x00},
    {0x37, 0x05},
    {0x38, 0x09},
    {0x39, 0x19},
    {0x3a, 0x38},
    {0x3b, 0x22},
    {0x3c, 0x22},
    {0x3d, 0x22},
    {0x3e, 0x03},
    {0xfd, 0x02},
    {0xce, 0x65},
    {0xfd, 0x03},
    {0x03, 0x30},
    {0x05, 0x00},
    {0x12, 0x70},
    {0x13, 0x70},
    {0x16, 0x13},
    {0x21, 0xca},
    {0x27, 0x95},
    {0x2c, 0x55},
    {0x2d, 0x08},
    {0x2e, 0xca},
    {0x3f, 0xe7},
    {0xfd, 0x00},
    {0x8b, 0x01},
    {0x8d, 0x00},
    {0xfd, 0x01},
    {0x01, 0x02},
    {0xfd, 0x05},
    {0xc4, 0x62},
    {0xc5, 0x62},
    {0xc6, 0x62},
    {0xc7, 0x62},
    {0xf0, 0x40},
    {0xf1, 0x40},
    {0xf2, 0x40},
    {0xf3, 0x40},
    {0xf4, 0x00},
    {0xf9, 0x03},
    {0xfa, 0x5d},
    {0xfb, 0x6b},
    {0xb1, 0x01},
    {0xfd, 0x02},
    {0x5e, 0x32}
    {REG_END, 0x00},
};
#endif

static const struct os04d10_mode supported_modes[] =
{
    [OS04D10_640X360] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 640,
        .height = 360,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x032e,
        .vts_def = 0x0171,
        .xvclk = 24000000,
        .link_freq = 360000000,
        .reg_list = g_os04d10_640x360_120fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_os04d10_640x360_120fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [OS04D10_2560X1440] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2560,
        .height = 1440,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x032e,
        .vts_def = 0x05c1,
        .xvclk = 24000000,
        .link_freq = 360000000,
        .reg_list = g_os04d10_2560x1440_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_os04d10_2560x1440_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [OS04D10_2568X1448] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2568,
        .height = 1448,
        .max_fps = {
            .numerator = 10000,
            .denominator = 150000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x05dc * 2,
        .vts_def = 0xB83,
        .xvclk = 24000000,
        .link_freq = 360000000,
        .reg_list = g_os04d10_2568x1448_15fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_os04d10_2568x1448_15fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct os04d10_ops
{
    ret_err_t (*init)(struct os04d10_dev *dev);
    ret_err_t (*open)(struct os04d10_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct os04d10_dev *dev);
    ret_err_t (*control)(struct os04d10_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t os04d10_read_reg(struct os04d10_dev *dev,
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
            os04d10_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t os04d10_write_reg(struct os04d10_dev *dev,
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
            os04d10_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int os04d10_write_multiple_reg_continue(struct os04d10_dev *dev,
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
                os04d10_err("write multi-regs, retry=%d, addr=0x%02x\n", retry, i2c_data[offset + 1]);
                if (--retry)
                    continue;
                else
                    return RET_SYS_ERROR;
            }
        }
        else
        {
            os04d10_info("delay %d ms\n", i2c_data[offset + 2]);
            HAL_DelayUs(i2c_data[offset + 2] * 1000);
        }

        offset += (i2c_data[offset] + 1);
        retry = 10;
        i++;
    }

    return RET_SYS_EOK;
}

#else
static ret_err_t os04d10_write_reg_discontinued(struct os04d10_dev *dev,
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
            os04d10_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void os04d10_write_multiple_reg(struct os04d10_dev *dev,
                                       const struct os04d10_sensor_reg *reg, int len)
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

                os04d10_write_reg_discontinued(i2c_bus, data, k + 1);
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

                os04d10_write_reg_discontinued(i2c_bus, data, k + 1);
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

                    os04d10_write_reg_discontinued(i2c_bus, data, k + 1);
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

static void rk_continuous_reg_dump(struct os04d10_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        os04d10_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        os04d10_write_multiple_reg((struct os04d10_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

static ret_err_t rk_os04d10_set_expval(struct os04d10_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint32_t a_gain, d_gain;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        os04d10_dbg("os04d10 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    os04d10_dbg("os04d10: exp req: L_exp: 0x%x, 0x%x\n",
                l_exp_time, l_a_gain);

    ret  = os04d10_write_reg(dev,
                             OS04D10_REG_PAGE_SEL,
                             OS04D10_REG_PAGE_CIS_TIMING);
    /* 4 least significant bits of expsoure are fractional part */
    ret |= os04d10_write_reg(dev,
                             OS04D10_REG_EXPOSURE_H,
                             (uint8_t)OS04D10_FETCH_EXP_H(l_exp_time));
    ret |= os04d10_write_reg(dev,
                             OS04D10_REG_EXPOSURE_L,
                             (uint8_t)OS04D10_FETCH_EXP_L(l_exp_time));
    ret |= os04d10_write_reg(dev,
                             OS04D10_REG_EXP_UPDATE,
                             0x01);

    /* again max is 15.5 gain convert thread is 15.5 * 1 * 64 = 992*/
    if (l_a_gain < OS04D10_GAIN_MIN)
    {
        a_gain = 0x10;
        d_gain = 0x0040;
    }
    else if (l_a_gain < 992)
    {
        a_gain = l_a_gain >> 2;
        d_gain = 0x0040;
    }
    else if (l_a_gain < OS04D10_GAIN_MAX)
    {
        a_gain = 0xF8;
        d_gain = l_a_gain * 64 / 992;
    }
    else
    {
        a_gain = 0xF8;
        d_gain = 0x07FF;
    }
    ret  = os04d10_write_reg(dev,
                             OS04D10_REG_PAGE_SEL,
                             OS04D10_REG_PAGE_CIS_TIMING);
    ret |= os04d10_write_reg(dev,
                             OS04D10_REG_ANA_GAIN,
                             a_gain);
    ret  = os04d10_write_reg(dev,
                             OS04D10_REG_EXP_UPDATE,
                             0x01);

    ret  = os04d10_write_reg(dev,
                             OS04D10_REG_PAGE_SEL,
                             OS04D10_REG_PAGE_CIS_CTRL);
    ret  = os04d10_write_reg(dev,
                             OS04D10_REG_DIG_GAIN_H,
                             (d_gain >> 8) & 0xff);
    ret  = os04d10_write_reg(dev,
                             OS04D10_REG_DIG_GAIN_L,
                             d_gain & 0xff);
    ret  = os04d10_write_reg(dev,
                             OS04D10_REG_EXP_UPDATE,
                             0x01);

    if (ret == RET_SYS_EOK)
    {
        rt_mutex_release(&dev->mutex_lock);
        return RET_SYS_EOK;
    }
    else
    {
        os04d10_info("Warning: set gain failed with error (%d)!!!\n", ret);
        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }
}

static ret_err_t rk_os04d10_set_vts(struct os04d10_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint32_t vblank;

    RT_ASSERT(dev != RT_NULL);

    os04d10_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;
    vblank = dst_vts - dev->dst_mode->height;
    ret |= os04d10_write_reg(dev, OS04D10_REG_PAGE_SEL, OS04D10_REG_PAGE_CIS_TIMING);
    ret |= os04d10_write_reg(dev, OS04D10_REG_VBLANK_L, (uint8_t)(vblank & 0xff));
    ret |= os04d10_write_reg(dev, OS04D10_REG_VBLANK_H, (uint8_t)(vblank >> 8));
    ret |= os04d10_write_reg(dev, OS04D10_REG_EXP_UPDATE, 0x01);
    return ret;
}

static ret_err_t rk_os04d10_set_flip_mirror(struct os04d10_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = os04d10_write_reg(dev, OS04D10_REG_PAGE_SEL, OS04D10_REG_PAGE_CIS_TIMING);
    ret |= os04d10_read_reg(dev, OS04D10_FLIP_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = OS04D10_FETCH_MIRROR(val, flip & 0x01);
        val = OS04D10_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = OS04D10_FETCH_MIRROR(val, false);
        val = OS04D10_FETCH_FLIP(val, false);
        break;
    };
    os04d10_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret |= os04d10_write_reg(dev, OS04D10_FLIP_REG, val);
    ret |= os04d10_write_reg(dev, OS04D10_REG_EXP_UPDATE, 0x01);

    return ret;
}

static void os04d10_stream_on(struct os04d10_dev *dev)
{
    os04d10_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(10);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
#ifndef REG_LIST_CONVERSION_DEBUG
    os04d10_write_multiple_reg_continue((struct os04d10_dev *)dev,
                                        dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_os04d10_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_os04d10_set_flip_mirror(dev, dev->flip);
    os04d10_write_reg(dev, OS04D10_REG_PAGE_SEL, OS04D10_REG_PAGE_SYS_CTRL);
    os04d10_write_reg(dev, OS04D10_REG_CTRL_MODE, OS04D10_MODE_STREAMING);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    os04d10_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void os04d10_stream_on_late(struct os04d10_dev *dev)
{
    os04d10_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_os04d10_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }

    os04d10_write_reg(dev, OS04D10_REG_PAGE_SEL, OS04D10_REG_PAGE_SYS_CTRL);
    os04d10_write_reg(dev, OS04D10_REG_CTRL_MODE, OS04D10_MODE_STREAMING);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    os04d10_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void os04d10_stream_off(struct os04d10_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        os04d10_write_reg(dev, OS04D10_REG_PAGE_SEL, OS04D10_REG_PAGE_SYS_CTRL);
        os04d10_write_reg(dev, OS04D10_REG_CTRL_MODE, OS04D10_MODE_SW_STANDBY);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        os04d10_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_os04d10_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os04d10_dev *os04d10;

    os04d10 = (struct os04d10_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&os04d10->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(os04d10);
#endif

    if (os04d10)
    {
        camera->info.mbus_fmt.width = os04d10->cur_mode->width;
        camera->info.mbus_fmt.height = os04d10->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = os04d10->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  os04d10->cur_mode->hdr_mode;

        os04d10->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(os04d10->i2c_name);

        if (!os04d10->i2c_bus)
        {
            os04d10_err("not find i2c :%s !!!\n",
                        os04d10->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            os04d10_dbg("find i2c_bus:%s\n", os04d10->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_os04d10_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_os04d10_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_os04d10_get_expinf(struct os04d10_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os04d10_mode *mode;

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

static ret_err_t rk_os04d10_get_intput_fmt(struct os04d10_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os04d10_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_os04d10_set_intput_fmt(struct os04d10_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os04d10_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    os04d10_info("dst resulotion, width %d, height %d\n",
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
            os04d10_write_multiple_reg_continue((struct os04d10_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_os04d10_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_os04d10_match_dst_config(struct os04d10_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct os04d10_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    os04d10_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct os04d10_mode *)mode;
            ret = RET_SYS_EOK;
            os04d10_info("find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            os04d10_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        os04d10_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_os04d10_control(struct rk_camera_device *dev,
                             dt_cmd_t cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os04d10_dev *os04d10;

    RT_ASSERT(dev != RT_NULL);
    os04d10 = (struct os04d10_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_os04d10_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        os04d10_stream_on(os04d10);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        os04d10_stream_off(os04d10);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_os04d10_get_expinf(os04d10, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_os04d10_set_expval(os04d10, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_os04d10_set_vts(os04d10, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_os04d10_get_intput_fmt(os04d10, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_os04d10_set_intput_fmt(os04d10, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_os04d10_match_dst_config(os04d10, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_os04d10_set_flip_mirror(os04d10, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        os04d10_stream_on_late(os04d10);
    }
    break;
    default:
        os04d10_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_os04d10_ops =
{
    rk_os04d10_init,
    rk_os04d10_open,
    NULL,
    rk_os04d10_control
};

int rk_camera_os04d10_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os04d10_dev *instance = &g_os04d10;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *os04d10 = (struct camera_board_desc *)&camera_os04d10;

    camera->ops = &rk_os04d10_ops;

    rt_strncpy(instance->name, os04d10->isp_subdev_name, rt_strlen(os04d10->isp_subdev_name));
    rt_strncpy(instance->i2c_name, os04d10->i2c_bus, rt_strlen(os04d10->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (os04d10->mode_id <= OS04D10_MODE_ID_MAX)
    {
        os04d10_dbg("mode_id: %d\n", os04d10->mode_id);
        instance->cur_mode = &supported_modes[os04d10->mode_id];
    }
    else
    {
        os04d10_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct os04d10_mode *)&supported_modes[OS04D10_2568X1448];
    instance->time_valid_delay = os04d10->time_valid_delay;
    instance->gain_valid_delay = os04d10->gain_valid_delay;
    instance->i2c_addr = os04d10->i2c_addr;
    instance->flip = 0;

    HAL_DelayUs(5 * 1000);
    if (os04d10->rst_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)os04d10->rst_gpio.gpio_group, os04d10->rst_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)os04d10->rst_gpio.gpio_group, os04d10->rst_gpio.gpio_pin, !os04d10->rst_active);
    }
    if (os04d10->pwdn_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)os04d10->pwdn_gpio.gpio_group, os04d10->pwdn_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)os04d10->pwdn_gpio.gpio_group, os04d10->pwdn_gpio.gpio_pin, !os04d10->pwdn_active);
    }

    if (os04d10->mclk_out_gate_id && os04d10->mclk_id)
    {
        clk_set_rate(os04d10->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(os04d10->mclk_out_gate_id);
    }
    HAL_DelayUs(8 * 1000);
    rt_mutex_init(&instance->mutex_lock, "OS04D10_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, os04d10->isp_subdev_name, rt_strlen(os04d10->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void os04d10_detect(void)
{
    struct os04d10_dev *instance = &g_os04d10;

    os04d10_dbg("start to detect OS04D10 for testing \n");
    os04d10_dbg("dev name:%s\n", instance->name);
    os04d10_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        os04d10_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        os04d10_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_os04d10_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(os04d10_detect, check OS04D10 is available or not);
#endif
#endif
#endif
