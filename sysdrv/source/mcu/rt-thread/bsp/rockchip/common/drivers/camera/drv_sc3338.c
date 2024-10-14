/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc3338.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2022-07-07     ISP Team      first implementation
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
#include "isp3/drv_isp3.h"

#ifdef RT_USING_SC3338

RT_WEAK const struct camera_board_desc camera_sc3338[] = {0};

#define SC3338_DEBUG_PRT      0

#define sc3338_info(dev, fmt, ...)          rt_kprintf("sc3338-%s "fmt"", dev->name, ##__VA_ARGS__)
#define sc3338_err(dev, fmt, ...)           rt_kprintf("sc3338-%s error "fmt"", dev->name, ##__VA_ARGS__)
#if SC3338_DEBUG_PRT
#include <stdio.h>
#define sc3338_dbg(dev, fmt, ...)           rt_kprintf("sc3338-%s "fmt"", dev->name, ##__VA_ARGS__)
#else
#define sc3338_dbg(dev, fmt, ...)
#endif

#define SC3338_DEFAULT_I2CADDR                  0x30

#define SC3338_REG_RESET                        0x0103
#define SC3338_REG_I2C_ADDR                     0x3004
#define SC3338_REG_EXP_LONG_H                   0x3e00
#define SC3338_REG_EXP_LONG_M                   0x3e01
#define SC3338_REG_EXP_LONG_L                   0x3e02
#define SC3338_REG_EXP_SHORT_H                  0x3e22
#define SC3338_REG_EXP_SHORT_M                  0x3e04
#define SC3338_REG_EXP_SHORT_L                  0x3e05
#define SC3338_REG_DIG_GAIN                     0x3e06
#define SC3338_REG_DIG_FINE_GAIN                0x3e07
#define SC3338_REG_ANA_GAIN                     0x3e09
#define SC3338_REG_SDIG_GAIN                    0x3e10
#define SC3338_REG_SDIG_FINE_GAIN               0x3e11
#define SC3338_REG_SANA_GAIN                    0x3e12
#define SC3338_REG_SANA_FINE_GAIN               0x3e13
#define SC3338_EXP_MIN                          2
#define SC3338_GAIN_MIN                         0x0080
#define SC3338_GAIN_MAX                         (99614)  //48.64*16*128
#define SC3338_GAIN_STEP                        1
#define SC3338_GAIN_DEFAULT                     0x0080
#define SC3338_LGAIN                            0
#define SC3338_SGAIN                            1
#define SC3338_FLIP_MIRROR_REG                  0x3221
#define SC3338_REG_VTS_H                        0x320e
#define SC3338_REG_VTS_L                        0x320f
#define SC3338_FETCH_MIRROR(VAL, ENABLE)        (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC3338_FETCH_FLIP(VAL, ENABLE)          (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC3338_REG_STREAM_CTRL                  0x0100
#define SC3338_STREAM_ON                        0x01
#define SC3338_STREAM_OFF                       0x00

#define REG_END                                 (0x0)
#define REG_DELAY                               (0xff)

#define ARRAY_SIZE(x)                           (sizeof(x) / sizeof((x)[0]))

struct sc3338_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc3338_mode
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
    const uint8_t *reg_list;
    const int reg_list_size;
    uint8_t hdr_mode;
    uint8_t use_diff_reg;
} __attribute__((__packed__));

#pragma pack(1)
struct sc3338
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
    const struct sc3338_mode *cur_mode;
    struct sc3338_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

struct sc3338_ops
{
    ret_err_t (*init)(struct sc3338 *dev);
    ret_err_t (*open)(struct sc3338 *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc3338 *dev);
    ret_err_t (*control)(struct sc3338 *dev, dt_cmd_t cmd, void *arg);
};

/*
 * Xclk 27Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 225Mbps, 2lane
 * binning to 1152x648 then centered crop
 */
static const uint8_t sc3338_320x240_120fps_regs[] =
{
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x6a,
    0x3, 0x30, 0xb8, 0x44,
    0xe, 0x32, 0x00, 0x00, 0x00, 0x01, 0x98, 0x09, 0x07, 0x03, 0x7f, 0x01, 0x40, 0x00, 0xf0,
    0x8, 0x32, 0x0e, 0x01, 0x2c, 0x01, 0xa2, 0x00, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x20,
    0x3, 0x33, 0x01, 0x04,
    0x3, 0x33, 0x06, 0x50,
    0x5, 0x33, 0x09, 0xa8, 0x00, 0xd8,
    0x3, 0x33, 0x14, 0x13,
    0x3, 0x33, 0x1f, 0x99,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x04, 0x04, 0x04, 0x08, 0x0b, 0x1f, 0x04, 0x0a, 0x3a, 0xb4,
    0x3, 0x33, 0xa2, 0x04,
    0x6, 0x33, 0xac, 0x08, 0x1c, 0x10, 0x30,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x48,
    0x3, 0x33, 0xf9, 0x60,
    0x5, 0x33, 0xfb, 0x74, 0x4b, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x4b, 0x5f, 0x20, 0x18,
    0x5, 0x34, 0xab, 0xe8, 0x01, 0x00,
    0x4, 0x34, 0xf8, 0x5f, 0x18,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x64, 0x32,
    0x4, 0x36, 0x3b, 0x03, 0x08,
    0x3, 0x36, 0x41, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xc0, 0xc0, 0xc0, 0x84, 0x84, 0x84,
    0x6, 0x36, 0x7c, 0x48, 0x49, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x32, 0x32, 0x42,
    0x4, 0x36, 0x9c, 0x4b, 0x5f,
    0x9, 0x36, 0xb0, 0x87, 0x90, 0xa1, 0xd8, 0x49, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x0a, 0x0d, 0x2c, 0x26,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x5, 0x37, 0x71, 0x09, 0x09, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x5f,
    0x6, 0x37, 0xfa, 0x0a, 0x33, 0x11, 0x18,
    0x4, 0x39, 0x04, 0x04, 0x8c,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0x21, 0x20,
    0x3, 0x39, 0x26, 0x21,
    0x8, 0x39, 0x33, 0x80, 0x0a, 0x00, 0x2a, 0x6a, 0x6a,
    0x3, 0x39, 0xdc, 0x02,
    0x4, 0x3e, 0x01, 0x12, 0x40,
    0x3, 0x3e, 0x09, 0x00,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x48, 0x19, 0x04,
    0x3, 0x48, 0x1b, 0x02,
    0x3, 0x48, 0x1d, 0x06,
    0x3, 0x48, 0x1f, 0x02,
    0x3, 0x48, 0x21, 0x07,
    0x3, 0x48, 0x23, 0x02,
    0x3, 0x48, 0x25, 0x02,
    0x3, 0x48, 0x27, 0x02,
    0x3, 0x48, 0x29, 0x03,
    0x3, 0x50, 0x00, 0x46,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x3, 0x59, 0x88, 0x70,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x44,
    0x3, 0x37, 0xf9, 0x44,
    0x3, 0x30, 0x28, 0x27,
    0x49,
};

/*
 * Xclk 24Mhz
 * max_framerate 75fps
 * mipi_datarate per lane 255Mbps, 2lane
 * binning to 1152x648 then centered crop
 */
static const uint8_t sc3338_640x480_75fps_regs[] =
{
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x67,
    0x3, 0x30, 0xb8, 0x44,
    0xe, 0x32, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x09, 0x07, 0x04, 0x6f, 0x02, 0x80, 0x01, 0xe0,
    0x8, 0x32, 0x0e, 0x02, 0x20, 0x01, 0x02, 0x00, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x20,
    0x3, 0x33, 0x01, 0x04,
    0x3, 0x33, 0x06, 0x50,
    0x5, 0x33, 0x09, 0xa8, 0x00, 0xd8,
    0x3, 0x33, 0x14, 0x13,
    0x3, 0x33, 0x1f, 0x99,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x04, 0x04, 0x04, 0x08, 0x0b, 0x1f, 0x04, 0x0a, 0x3a, 0xb4,
    0x3, 0x33, 0xa2, 0x04,
    0x6, 0x33, 0xac, 0x08, 0x1c, 0x10, 0x30,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x48,
    0x3, 0x33, 0xf9, 0x60,
    0x5, 0x33, 0xfb, 0x74, 0x4b, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x4b, 0x5f, 0x20, 0x18,
    0x5, 0x34, 0xab, 0xe8, 0x01, 0x00,
    0x4, 0x34, 0xf8, 0x5f, 0x18,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x64, 0x32,
    0x4, 0x36, 0x3b, 0x03, 0x08,
    0x3, 0x36, 0x41, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xc0, 0xc0, 0xc0, 0x84, 0x84, 0x84,
    0x6, 0x36, 0x7c, 0x48, 0x49, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x32, 0x32, 0x42,
    0x4, 0x36, 0x9c, 0x4b, 0x5f,
    0x9, 0x36, 0xb0, 0x87, 0x90, 0xa1, 0xd8, 0x49, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x11, 0x0d, 0x2c, 0x26,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x5, 0x37, 0x71, 0x09, 0x09, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x5f,
    0x6, 0x37, 0xfa, 0x11, 0x33, 0x11, 0x08,
    0x4, 0x39, 0x04, 0x04, 0x8c,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0x21, 0x20,
    0x3, 0x39, 0x26, 0x21,
    0x8, 0x39, 0x33, 0x80, 0x0a, 0x00, 0x2a, 0x6a, 0x6a,
    0x3, 0x39, 0xdc, 0x02,
    0x4, 0x3e, 0x01, 0x21, 0x80,
    0x3, 0x3e, 0x09, 0x00,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x48, 0x19, 0x04,
    0x3, 0x48, 0x1b, 0x02,
    0x3, 0x48, 0x1d, 0x07,
    0x3, 0x48, 0x1f, 0x02,
    0x3, 0x48, 0x21, 0x07,
    0x3, 0x48, 0x23, 0x02,
    0x3, 0x48, 0x25, 0x02,
    0x3, 0x48, 0x27, 0x02,
    0x3, 0x48, 0x29, 0x03,
    0x3, 0x50, 0x00, 0x46,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x3, 0x59, 0x88, 0x70,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x54,
    0x3, 0x37, 0xf9, 0x47,
    0x3, 0x30, 0x28, 0x27,
    0x49,
};

/*
 * Xclk 27Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 270Mbps, 2lane
 * only binning to 1152x648
 */
static const uint8_t sc3338_1152x648_60fps_regs[] =
{
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x22,
    0x3, 0x30, 0xb8, 0x33,
    0xa, 0x32, 0x08, 0x04, 0x80, 0x02, 0x88, 0x04, 0xf0, 0x02, 0xc8,
    0x3, 0x32, 0x11, 0x02,
    0x3, 0x32, 0x13, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x20,
    0x3, 0x33, 0x01, 0x04,
    0x3, 0x33, 0x06, 0x50,
    0x5, 0x33, 0x09, 0xa8, 0x00, 0xd8,
    0x3, 0x33, 0x14, 0x13,
    0x3, 0x33, 0x1f, 0x99,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x04, 0x04, 0x04, 0x08, 0x0b, 0x1f, 0x04, 0x0a, 0x3a, 0xa0,
    0x3, 0x33, 0xa2, 0x04,
    0x6, 0x33, 0xac, 0x08, 0x1c, 0x10, 0x30,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x48,
    0x3, 0x33, 0xf9, 0x60,
    0x5, 0x33, 0xfb, 0x74, 0x4b, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x4b, 0x5f, 0x20, 0x18,
    0x5, 0x34, 0xab, 0xe8, 0x01, 0x00,
    0x4, 0x34, 0xf8, 0x5f, 0x18,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x64, 0x32,
    0x4, 0x36, 0x3b, 0x03, 0x08,
    0x3, 0x36, 0x41, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xc0, 0xc0, 0xc0, 0x86, 0x86, 0x86,
    0x6, 0x36, 0x7c, 0x48, 0x49, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x32, 0x32, 0x42,
    0x4, 0x36, 0x9c, 0x4b, 0x5f,
    0x9, 0x36, 0xb0, 0x87, 0x90, 0xa1, 0xd8, 0x49, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x08, 0x0d, 0x2c, 0x26,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x5, 0x37, 0x71, 0x09, 0x09, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x5f,
    0x6, 0x37, 0xfa, 0x08, 0x33, 0x11, 0x18,
    0x4, 0x39, 0x04, 0x04, 0x8c,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0x21, 0x20,
    0x3, 0x39, 0x26, 0x21,
    0x8, 0x39, 0x33, 0x80, 0x0a, 0x00, 0x2a, 0x6a, 0x6a,
    0x3, 0x39, 0xdc, 0x02,
    0x4, 0x3e, 0x01, 0x29, 0x80,
    0x3, 0x3e, 0x09, 0x00,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x48, 0x19, 0x04,
    0x3, 0x48, 0x1b, 0x03,
    0x3, 0x48, 0x1d, 0x08,
    0x3, 0x48, 0x1f, 0x02,
    0x3, 0x48, 0x21, 0x07,
    0x3, 0x48, 0x23, 0x02,
    0x3, 0x48, 0x25, 0x02,
    0x3, 0x48, 0x27, 0x02,
    0x3, 0x48, 0x29, 0x03,
    0x3, 0x50, 0x00, 0x46,
    0x4, 0x59, 0x00, 0xf1, 0x04,
    0x3, 0x59, 0x88, 0x70,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x24,
    0x3, 0x37, 0xf9, 0x24,
    0x3, 0x30, 0x28, 0x27,
    0x4a,
};

/*
 * Xclk 27Mhz
 * max_framerate 25fps
 * mipi_datarate per lane 225Mbps, 2lane
 * binning to 1152x648 then centered crop
 */
static const uint8_t sc3338_2304x1296_25fps_regs[] =
{
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x01,
    0x3, 0x30, 0xb8, 0x33,
    0x4, 0x32, 0x0e, 0x06, 0x54,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x20,
    0x3, 0x33, 0x01, 0x04,
    0x3, 0x33, 0x06, 0x50,
    0x5, 0x33, 0x09, 0xa8, 0x00, 0xd8,
    0x3, 0x33, 0x14, 0x13,
    0x3, 0x33, 0x1f, 0x99,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x04, 0x04, 0x04, 0x08, 0x0b, 0x1f, 0x04, 0x0a, 0x3a, 0xb4,
    0x3, 0x33, 0xa2, 0x04,
    0x6, 0x33, 0xac, 0x08, 0x1c, 0x10, 0x30,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x48,
    0x3, 0x33, 0xf9, 0x60,
    0x5, 0x33, 0xfb, 0x74, 0x4b, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0x6, 0x34, 0xa6, 0x4b, 0x5f, 0x20, 0x18,
    0x5, 0x34, 0xab, 0xe8, 0x01, 0x00,
    0x4, 0x34, 0xf8, 0x5f, 0x18,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x64, 0x32,
    0x4, 0x36, 0x3b, 0x03, 0x08,
    0x3, 0x36, 0x41, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xc0, 0xc0, 0xc0, 0x84, 0x84, 0x84,
    0x6, 0x36, 0x7c, 0x48, 0x49, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x32, 0x32, 0x42,
    0x4, 0x36, 0x9c, 0x4b, 0x5f,
    0x9, 0x36, 0xb0, 0x87, 0x90, 0xa1, 0xd8, 0x49, 0x4b, 0x4f,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x5, 0x37, 0x71, 0x09, 0x09, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x5f,
    0x4, 0x39, 0x04, 0x04, 0x8c,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0x21, 0x20,
    0x3, 0x39, 0x26, 0x21,
    0x8, 0x39, 0x33, 0x80, 0x0a, 0x00, 0x2a, 0x6a, 0x6a,
    0x3, 0x39, 0xdc, 0x02,
    0x4, 0x3e, 0x01, 0x53, 0xe0,
    0x3, 0x3e, 0x09, 0x00,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x20,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x54,
    0x3, 0x37, 0xf9, 0x27,
    0x3, 0x30, 0x28, 0x27,
    0x38,
};

/*
 * 648p to 1296p diff regs list
 */
static const uint8_t sc3338_1296p_diff_regs[] =
{
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x01,
    0x3, 0x30, 0xb8, 0x33,
    0x4, 0x32, 0x0e, 0x06, 0x54,

    0x5, 0x33, 0x09, 0xa8, 0x00, 0xd8,
    0x3, 0x33, 0x14, 0x13,
    0x3, 0x33, 0x1f, 0x99,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0e,

    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x04, 0x04, 0x04, 0x08, 0x0b, 0x1f, 0x04, 0x0a, 0x3a, 0xb4,
    0x3, 0x33, 0xb3, 0x48,
    0x3, 0x33, 0xf9, 0x60,
    0x5, 0x33, 0xfb, 0x74, 0x4b, 0x5f,

    0x8, 0x36, 0x74, 0xc0, 0xc0, 0xc0, 0x84, 0x84, 0x84,
    0x6, 0x36, 0x7c, 0x48, 0x49, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x32, 0x32, 0x42,
    0x4, 0x3e, 0x01, 0x53, 0xe0,
    0x3, 0x36, 0xe9, 0x54,
    0x3, 0x37, 0xf9, 0x27,
    0x18,
};

static const struct sc3338_mode supported_modes[] =
{
    [SC3338_320X240] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 320,
        .height = 240,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x46,
        .hts_def = 0x0578 * 2,
        .vts_def = 0x012c,
        .xvclk = 27000000,
        .link_freq = 112500000,
        .reg_list = sc3338_320x240_120fps_regs,
        .reg_list_size = ARRAY_SIZE(sc3338_320x240_120fps_regs),
        .hdr_mode = NO_HDR,
        .use_diff_reg = 0,
    },
    [SC3338_640X480] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 640,
        .height = 480,
        .max_fps = {
            .numerator = 10000,
            .denominator = 750000,
        },
        .exp_def = 0x46,
        .hts_def = 0x0578 * 2,
        .vts_def = 0x0220,
        .xvclk = 24000000,
        .link_freq = 127500000,
        .reg_list = sc3338_640x480_75fps_regs,
        .reg_list_size = ARRAY_SIZE(sc3338_640x480_75fps_regs),
        .hdr_mode = NO_HDR,
        .use_diff_reg = 0,
    },
    [SC3338_1152X648] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1152,
        .height = 648,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x46,
        .hts_def = 0x04f0 * 2,
        .vts_def = 0x02c8,
        .xvclk = 27000000,
        .link_freq = 135000000,
        .reg_list = sc3338_1152x648_60fps_regs,
        .reg_list_size = ARRAY_SIZE(sc3338_1152x648_60fps_regs),
        .hdr_mode = NO_HDR,
        .use_diff_reg = 0,
    },
    [SC3338_2304X1296] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2304,
        .height = 1296,
        .max_fps = {
            .numerator = 10000,
            .denominator = 250000,
        },
        .exp_def = 0x46,
        .hts_def = 0x5dc,
        .vts_def = 0x0654,
        .xvclk = 27000000,
        .link_freq = 112500000,
        .reg_list = sc3338_2304x1296_25fps_regs,
        .reg_list_size = ARRAY_SIZE(sc3338_2304x1296_25fps_regs),
        .hdr_mode = NO_HDR,
        .use_diff_reg = 1,
    }
};

static ret_err_t sc3338_read_reg(struct sc3338 *dev, uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    int retry = 10;
    ret_err_t ret;

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
            return RT_EOK;
        }
        else
        {
            sc3338_err(dev, "read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc3338_write_reg(struct sc3338 *dev, uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    send_buf[2] = data;

    msg.addr = dev->i2c_addr;
    msg.flags = RT_I2C_WR;
    msg.buf = send_buf;
    msg.len = 3;

    while (1)
    {
        ret = rt_i2c_transfer(dev->i2c_bus, &msg, 1);
        if (ret == 1)
        {
            return RT_EOK;
        }
        else
        {
            sc3338_err(dev, "write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static int sc3338_write_multiple_reg_continue(struct sc3338 *dev, const uint8_t *i2c_data, int len)
{
    uint16_t i;
    struct rt_i2c_msg msg;
    int ret = 0;
    int offset = 0;
    int retry = 10;

    RT_ASSERT(i2c_data != RT_NULL);

    for (i = 0; i < i2c_data[len - 1];)
    {
        msg.addr = dev->i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(dev->i2c_bus, &msg, 1);

        if (ret != 1)
        {
            sc3338_err(dev, "write multi-regs, retry=%d, addr=0x%02x%02x\n",
                       retry, i2c_data[offset + 1], i2c_data[offset + 2]);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
        offset += (i2c_data[offset] + 1);
        retry = 10;
        i++;
    }

    return RT_EOK;
}

static int sc3338_get_gain_reg(uint8_t *again, uint8_t *dgain,
                               uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    if (total_gain < SC3338_GAIN_MIN)
        total_gain = SC3338_GAIN_MIN;
    else if (total_gain > SC3338_GAIN_MAX)
        total_gain = SC3338_GAIN_MAX;

    gain_factor = total_gain * 1000 / 128;
    if (gain_factor < 1520)          /* 1 ~ 1.52 gain*/
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000;
    }
    else if (gain_factor < 1520 * 2)     /* 1.52 ~ 3.04 gain*/
    {
        *again = 0x40;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1520;
    }
    else if (gain_factor < 1520 * 4)         /* 3.04 ~ 6.08 gain*/
    {
        *again = 0x48;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1520 / 2;
    }
    else if (gain_factor < 1520 * 8)         /* 6.08 ~ 12.16 gain*/
    {
        *again = 0x49;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1520 / 4;
    }
    else if (gain_factor < 1520 * 16)         /* 12.16 ~ 24.32 gain*/
    {
        *again = 0x4b;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1520 / 8;
    }
    else if (gain_factor < 1520 * 32)        /* 24.32 ~ 48.64 gain*/
    {
        *again = 0x4f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1520 / 16;
    }
    else if (gain_factor < 1520 * 64)        /* 48.64 ~ 97.28 gain*/
    {
        //open dgain begin  max digital gain 4X
        *again = 0x5f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1520 / 32;
    }
    else if (gain_factor  < 1520 * 128)        /* 97.28 ~ 194.56 gain*/
    {
        *again = 0x5f;
        *dgain = 0x01;
        *dgain_fine = gain_factor * 128 / 1520 / 64;
    }
    else if (gain_factor < 1520 * 256)       /* 217.024 ~ 434.048 gain*/
    {
        *again = 0x5f;
        *dgain = 0x03;
        *dgain_fine = gain_factor * 128 / 1520 / 128;
    }
    else if (gain_factor < 1520 * 512)       /* 434.048 ~ 868.096 gain*/
    {
        *again = 0x5f;
        *dgain = 0x07;
        *dgain_fine = gain_factor * 128 / 1520 / 256;
    }

    return ret;
}
static ret_err_t rk_sc3338_set_expval(struct sc3338 *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RT_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, d_gain_fine;
    struct camera_board_desc *sc3338 = (struct camera_board_desc *)camera_sc3338;

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming && strcmp(sc3338->isp_subdev_name, RK_ISP_ASSIST_SUBDEV_NAME))
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc3338_dbg(dev, "don't stream, record exp\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC3338_EXP_MIN)
        l_exp_time = SC3338_EXP_MIN;

    sc3338_dbg(dev, "total_time reg: 0x%x, total_gain reg 0x%x\n", l_exp_time, l_a_gain);

    ret = sc3338_write_reg(dev, SC3338_REG_EXP_LONG_H,
                           (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc3338_write_reg(dev, SC3338_REG_EXP_LONG_M,
                           (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc3338_write_reg(dev, SC3338_REG_EXP_LONG_L,
                           (uint8_t)((l_exp_time & 0xf) << 4));
    sc3338_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    sc3338_dbg(dev, "a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", a_gain, d_gain, d_gain_fine);

    ret = sc3338_write_reg(dev, SC3338_REG_ANA_GAIN, a_gain);
    ret = sc3338_write_reg(dev, SC3338_REG_DIG_GAIN, d_gain);
    ret = sc3338_write_reg(dev, SC3338_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc3338_set_vts(struct sc3338 *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    sc3338_dbg(dev, "set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc3338_write_reg(dev, SC3338_REG_VTS_L,
                           (uint8_t)(dst_vts & 0xff));
    ret = sc3338_write_reg(dev, SC3338_REG_VTS_H,
                           (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc3338_set_flip_mirror(struct sc3338 *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    ret = sc3338_read_reg(dev, SC3338_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC3338_FETCH_MIRROR(val, flip & 0x01);
        val = SC3338_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC3338_FETCH_MIRROR(val, false);
        val = SC3338_FETCH_FLIP(val, false);
        break;
    };
    sc3338_dbg(dev, "flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc3338_write_reg(dev, SC3338_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc3338_stream_on(struct sc3338 *dev)
{
    sc3338_dbg(dev, "stream_on enter tick:%u\n", rt_tick_get());

    if (dev->i2c_addr == SC3338_DEFAULT_I2CADDR)
        HAL_DelayMs(4);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->i2c_addr == SC3338_DEFAULT_I2CADDR)
        sc3338_write_reg(dev, SC3338_REG_RESET, 0x01);
    sc3338_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc3338_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc3338_set_flip_mirror(dev, dev->flip);

    sc3338_write_reg(dev, SC3338_REG_STREAM_CTRL, SC3338_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc3338_dbg(dev, "stream_on exit tick:%u\n", rt_tick_get());
}

static void sc3338_stream_on_late(struct sc3338 *dev)
{
    sc3338_dbg(dev, "stream_on_late enter tick:%u\n", rt_tick_get());

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc3338_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc3338_set_flip_mirror(dev, dev->flip);

    sc3338_write_reg(dev, SC3338_REG_STREAM_CTRL, SC3338_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc3338_dbg(dev, "stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc3338_stream_off(struct sc3338 *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc3338_write_reg(dev, SC3338_REG_STREAM_CTRL, SC3338_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    dev->streaming = false;
}

ret_err_t rk_sc3338_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RT_EOK;
    struct sc3338 *sc3338;

    RT_ASSERT(dev != RT_NULL);
    sc3338 = (struct sc3338 *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc3338->parent;

    if (sc3338)
    {
        camera->info.mbus_fmt.width = sc3338->cur_mode->width;
        camera->info.mbus_fmt.height = sc3338->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc3338->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc3338->cur_mode->hdr_mode;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_sc3338_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc3338_close(struct rk_camera_device *dev)
{
    uint8_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_sc3338_get_expinf(struct sc3338 *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RT_EOK;
    const struct sc3338_mode *mode;

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

static ret_err_t rk_sc3338_get_intput_fmt(struct sc3338 *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc3338_mode *mode;

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc3338_set_intput_fmt(struct sc3338 *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc3338_mode *mode;
    int i;
    bool is_find_fmt = false;

    sc3338_info(dev, "dst resulotion, width %d, height %d\n",
                mbus_fmt->width, mbus_fmt->height);
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
            sc3338_info(dev, "switch to dst fmt, dst_width %d, dst_height %d dst_fps %d, hdr: %d, dst_vts: 0x%x\n",
                        dev->cur_mode->width, dev->cur_mode->height,
                        dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator,
                        dev->cur_mode->hdr_mode, dev->cur_mode->vts_def
                       );
            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
            if (dev->i2c_addr == SC3338_DEFAULT_I2CADDR)
                sc3338_write_reg(dev, SC3338_REG_RESET, 0x01);

            if (mode->use_diff_reg)
            {
                /* means set 1296p reg list */
                sc3338_write_multiple_reg_continue(dev, sc3338_1296p_diff_regs, ARRAY_SIZE(sc3338_1296p_diff_regs));
            }
            else
            {
                sc3338_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
            }
            ret = rk_sc3338_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc3338_match_dst_config(struct sc3338 *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc3338_mode *mode;
    int i, cur_fps, dst_fps, cur_vts, dst_vts;

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc3338_info(dev, "cur resulotion, width %d, height %d, fps %d!\n",
                dev->cur_mode->width, dev->cur_mode->height,
                dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];
        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc3338_mode *)mode;
            ret = RT_EOK;
            sc3338_info(dev, "find dst resulotion, width %d, height %d, fps %d!\n",
                        dst_config->width, dst_config->height, dst_fps);
            break;
        }
    }
    if (i == ARRAY_SIZE(supported_modes))
    {
        sc3338_err(dev, "not find match resulotion\n");
        return -RT_EINVAL;
    }

    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return RT_EOK;

    if (dst_fps > cur_fps)
    {
        sc3338_err(dev, "dst fps is larger than cur fps\n");
        return -RT_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc3338_control(struct rk_camera_device *dev,
                            dt_cmd_t cmd,
                            void *args)
{
    ret_err_t ret = RT_EOK;
    struct sc3338 *sc3338;

    RT_ASSERT(dev != RT_NULL);
    sc3338 = (struct sc3338 *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc3338_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc3338_stream_on(sc3338);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc3338_stream_off(sc3338);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc3338_get_expinf(sc3338, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc3338_set_expval(sc3338, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc3338_set_vts(sc3338, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc3338_get_intput_fmt(sc3338, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc3338_set_intput_fmt(sc3338, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc3338_match_dst_config(sc3338, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc3338_set_flip_mirror(sc3338, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc3338_stream_on_late(sc3338);
    }
    break;
    default:
        sc3338_dbg(sc3338, "exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc3338_ops =
{
    .init       = rk_sc3338_init,
    .open       = rk_sc3338_open,
    .control    = rk_sc3338_control,
};

int rk_camera_sc3338_init(void)
{
    ret_err_t ret = RT_EOK;
    const struct camera_board_desc *sc3338_desc;

    for (sc3338_desc = camera_sc3338; sc3338_desc->i2c_addr != 0; sc3338_desc++)
    {
        struct sc3338 *sc3338;
        struct rk_camera_device *camera;

        sc3338 = rt_malloc(sizeof(struct sc3338));
        if (!sc3338)
        {
            rt_kprintf("malloc sc3338 failed\n");
            return -RT_ENOMEM;
        }

        rt_memset(sc3338, 0x0, sizeof(struct sc3338));

        camera = &sc3338->parent;
        camera->ops = &rk_sc3338_ops;

        rt_memcpy(sc3338->name, sc3338_desc->isp_subdev_name, RK_CAMERA_DEVICE_NAME_SIZE);
        rt_memcpy(sc3338->i2c_name, sc3338_desc->i2c_bus, RK_CAMERA_I2C_NAME_SIZE);

        sc3338->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc3338->i2c_name);
        if (sc3338->i2c_bus == RT_NULL)
        {
            sc3338_err(sc3338, "can't find i2c bus:%s\n", sc3338->i2c_name);
            rt_free(sc3338);
            continue;
        }

        if (sc3338_desc->mode_id <= SC3338_MODE_ID_MAX)
        {
            sc3338_dbg(sc3338, "mode_id: %d\n", sc3338_desc->mode_id);
            sc3338->cur_mode = &supported_modes[sc3338_desc->mode_id];
        }
        else
        {
            sc3338_info(sc3338, "mode id is over range, default use mode_id: 0\n");
            sc3338->cur_mode = &supported_modes[0];
        }

        sc3338->time_valid_delay = sc3338_desc->time_valid_delay;
        sc3338->gain_valid_delay = sc3338_desc->gain_valid_delay;
        sc3338->i2c_addr = sc3338_desc->i2c_addr;

        if (sc3338_desc->rst_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc3338_desc->rst_gpio.gpio_group,
                                     sc3338_desc->rst_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc3338_desc->rst_gpio.gpio_group,
                                 sc3338_desc->rst_gpio.gpio_pin,
                                 !sc3338_desc->rst_active);
        }
        if (sc3338_desc->pwdn_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc3338_desc->pwdn_gpio.gpio_group,
                                     sc3338_desc->pwdn_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc3338_desc->pwdn_gpio.gpio_group,
                                 sc3338_desc->pwdn_gpio.gpio_pin,
                                 !sc3338_desc->pwdn_active);
        }

        if (sc3338_desc->mclk_out_gate_id && sc3338_desc->mclk_id)
        {
            clk_set_rate(sc3338_desc->mclk_id, sc3338->cur_mode->xvclk);
            HAL_CRU_ClkEnable(sc3338_desc->mclk_out_gate_id);
        }

        if (sc3338->i2c_addr != SC3338_DEFAULT_I2CADDR)
        {
            /* change the i2c address via register */
            int i2c_addr = sc3338->i2c_addr;

            HAL_DelayMs(4);
            sc3338_info(sc3338, "change i2c_addr from 0x%02x to 0x%02x\n", SC3338_DEFAULT_I2CADDR, i2c_addr);
            sc3338->i2c_addr = SC3338_DEFAULT_I2CADDR;
            sc3338_write_reg(sc3338, SC3338_REG_I2C_ADDR, i2c_addr << 1);
            sc3338->i2c_addr = i2c_addr;
        }

        rt_mutex_init(&sc3338->mutex_lock, "sc3338_mutex", RT_IPC_FLAG_FIFO);
        RT_ASSERT(rt_object_get_type(&sc3338->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
        camera->i2c_bus = sc3338->i2c_bus;
        rt_memcpy(camera->name, sc3338->name, RK_CAMERA_DEVICE_NAME_SIZE);
        ret = rk_camera_register(camera, camera->name, sc3338);
        if (ret)
        {
            sc3338_err(sc3338, "regster failed\n");
            rt_free(sc3338);
        }
    }

    return ret;
}

#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc3338_init);
#endif
#endif
