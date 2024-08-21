/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc223a.c
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

#ifdef RT_USING_SC223A

RT_WEAK const struct camera_board_desc camera_sc223a[] = {0};

#define SC223A_DEBUG_PRT      1

#define sc223a_info(dev, fmt, ...)          rt_kprintf("sc223a-%s "fmt"", dev->name, ##__VA_ARGS__)
#define sc223a_err(dev, fmt, ...)           rt_kprintf("sc223a-%s error "fmt"", dev->name, ##__VA_ARGS__)
#if SC223A_DEBUG_PRT
#include <stdio.h>
#define sc223a_dbg(dev, fmt, ...)           rt_kprintf("sc223a-%s "fmt"", dev->name, ##__VA_ARGS__)
#else
#define sc223a_dbg(dev, fmt, ...)
#endif

#define SC223A_DEFAULT_I2CADDR                  0x30

#define SC223A_REG_RESET                        0x0103
#define SC223A_REG_I2C_ADDR                     0x3004
#define SC223A_REG_EXP_LONG_H                   0x3e00
#define SC223A_REG_EXP_LONG_M                   0x3e01
#define SC223A_REG_EXP_LONG_L                   0x3e02
#define SC223A_REG_EXP_SHORT_H                  0x3e22
#define SC223A_REG_EXP_SHORT_M                  0x3e04
#define SC223A_REG_EXP_SHORT_L                  0x3e05
#define SC223A_REG_DIG_GAIN                     0x3e06
#define SC223A_REG_DIG_FINE_GAIN                0x3e07
#define SC223A_REG_ANA_GAIN                     0x3e09
#define SC223A_REG_SDIG_GAIN                    0x3e10
#define SC223A_REG_SDIG_FINE_GAIN               0x3e11
#define SC223A_REG_SANA_GAIN                    0x3e12
#define SC223A_REG_SANA_FINE_GAIN               0x3e13
#define SC223A_EXP_MIN                          3
#define SC223A_GAIN_MIN                         0x0080
#define SC223A_GAIN_MAX                         (29656)  //57.92*4*128
#define SC223A_GAIN_STEP                        1
#define SC223A_GAIN_DEFAULT                     0x0080
#define SC223A_LGAIN                            0
#define SC223A_SGAIN                            1
#define SC223A_FLIP_MIRROR_REG                  0x3221
#define SC223A_REG_VTS_H                        0x320e
#define SC223A_REG_VTS_L                        0x320f
#define SC223A_FETCH_MIRROR(VAL, ENABLE)        (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC223A_FETCH_FLIP(VAL, ENABLE)          (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC223A_REG_STREAM_CTRL                  0x0100
#define SC223A_STREAM_ON                        0x01
#define SC223A_STREAM_OFF                       0x00

#define REG_END                                 (0x0)
#define REG_DELAY                               (0xff)

#define ARRAY_SIZE(x)                           (sizeof(x) / sizeof((x)[0]))

struct sc223a_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc223a_mode
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
} __attribute__((__packed__));

#pragma pack(1)
struct sc223a
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
    const struct sc223a_mode *cur_mode;
    struct sc223a_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

struct sc223a_ops
{
    ret_err_t (*init)(struct sc223a *dev);
    ret_err_t (*open)(struct sc223a *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc223a *dev);
    ret_err_t (*control)(struct sc223a *dev, dt_cmd_t cmd, void *arg);
};

/*
 * Xclk 24Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 202.5Mbps, 2lane
 * only binning to 960x540
 */
static const uint8_t sc223a_960x540_60fps_regs[] =
{
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x44,
    0x3, 0x30, 0xb8, 0x44,
    0xa, 0x32, 0x08, 0x03, 0xc0, 0x02, 0x1c, 0x08, 0xc8, 0x02, 0x58,
    0x3, 0x32, 0x11, 0x02,
    0x3, 0x32, 0x13, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x53, 0x0c,
    0x3, 0x32, 0x81, 0x80,
    0x4, 0x33, 0x01, 0x06, 0x12,
    0x3, 0x33, 0x06, 0x84,
    0x5, 0x33, 0x09, 0x60, 0x00, 0xe0,
    0x3, 0x33, 0x0d, 0x20,
    0x3, 0x33, 0x14, 0x15,
    0x5, 0x33, 0x1e, 0x41, 0x51, 0x0a,
    0x3, 0x33, 0x26, 0x0e,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x5, 0x33, 0x5d, 0x60, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x56,
    0x6, 0x33, 0x7a, 0x06, 0x0e, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x03, 0x0f, 0x1f, 0x06, 0x06, 0x06, 0x48, 0x4b, 0x5f, 0x06, 0x06, 0x9c, 0x9c,
    0x4, 0x33, 0xa2, 0x04, 0x0a,
    0x3, 0x33, 0xad, 0x1c,
    0x3, 0x33, 0xaf, 0x40,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x20,
    0x3, 0x34, 0x9f, 0x02,
    0x6, 0x34, 0xa6, 0x48, 0x4b, 0x20, 0x20,
    0x4, 0x34, 0xf8, 0x5f, 0x10,
    0x3, 0x36, 0x16, 0xac,
    0x6, 0x36, 0x30, 0xc0, 0x86, 0x26, 0x32,
    0x3, 0x36, 0x37, 0x29,
    0x5, 0x36, 0x3a, 0x84, 0x04, 0x08,
    0x3, 0x36, 0x41, 0x3a,
    0x3, 0x36, 0x4f, 0x39,
    0x3, 0x36, 0x70, 0xce,
    0x8, 0x36, 0x74, 0xc0, 0xc0, 0xc0, 0x86, 0x8b, 0x8c,
    0x6, 0x36, 0x7c, 0x4b, 0x5f, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x62, 0x63, 0x63,
    0x7, 0x36, 0x99, 0x86, 0x92, 0xa4, 0x48, 0x4b,
    0x4, 0x36, 0xa2, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x09, 0x0c, 0x2c, 0x28,
    0x3, 0x37, 0x0f, 0x01,
    0x4, 0x37, 0x21, 0x6c, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc4,
    0x7, 0x37, 0xb0, 0x09, 0x09, 0x09, 0x48, 0x5f,
    0x6, 0x37, 0xfa, 0x09, 0x32, 0x10, 0x37,
    0x4, 0x39, 0x00, 0x19, 0x02,
    0x3, 0x39, 0x05, 0xb8,
    0x4, 0x39, 0x1b, 0x82, 0x00,
    0x3, 0x39, 0x1f, 0x04,
    0x4, 0x39, 0x33, 0x81, 0x4c,
    0x4, 0x39, 0x3f, 0xff, 0x73,
    0x4, 0x39, 0x42, 0x01, 0x4d,
    0x3, 0x39, 0x46, 0x20,
    0x3, 0x39, 0x57, 0x86,
    0x4, 0x3e, 0x01, 0x4a, 0x20,
    0x3, 0x3e, 0x28, 0xc4,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x01, 0xc0,
    0x3, 0x45, 0x09, 0x14,
    0x3, 0x45, 0x0d, 0x11,
    0x3, 0x45, 0x18, 0x00,
    0x3, 0x45, 0x1b, 0x0a,
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
    0x6, 0x50, 0x1c, 0x00, 0x60, 0x00, 0x40,
    0x3, 0x57, 0x99, 0x06,
    0x4, 0x59, 0x00, 0x01, 0x04,
    0x12, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c, 0xfe, 0x40,
    0xe, 0x5a, 0xf4, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x54,
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 405Mbps, 2lane
 * full resolution
 */
static const uint8_t sc223a_1920x1080_30fps_regs[] =
{
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x08,
    0x3, 0x30, 0xb8, 0x44,
    0x6, 0x32, 0x0c, 0x08, 0xca, 0x04, 0xb0,
    0x3, 0x32, 0x53, 0x0c,
    0x3, 0x32, 0x81, 0x80,
    0x4, 0x33, 0x01, 0x06, 0x12,
    0x3, 0x33, 0x06, 0x84,
    0x5, 0x33, 0x09, 0x60, 0x00, 0xe0,
    0x3, 0x33, 0x0d, 0x20,
    0x3, 0x33, 0x14, 0x15,
    0x5, 0x33, 0x1e, 0x41, 0x51, 0x0a,
    0x3, 0x33, 0x26, 0x0e,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x5, 0x33, 0x5d, 0x60, 0x06, 0x08,
    0x3, 0x33, 0x64, 0x56,
    0x6, 0x33, 0x7a, 0x06, 0x0e, 0x02, 0x0a,
    0xf, 0x33, 0x90, 0x03, 0x0f, 0x1f, 0x06, 0x06, 0x06, 0x48, 0x4b, 0x5f, 0x06, 0x06, 0x9c, 0x9c,
    0x4, 0x33, 0xa2, 0x04, 0x0a,
    0x3, 0x33, 0xad, 0x1c,
    0x3, 0x33, 0xaf, 0x40,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x20,
    0x3, 0x34, 0x9f, 0x02,
    0x6, 0x34, 0xa6, 0x48, 0x4b, 0x20, 0x20,
    0x4, 0x34, 0xf8, 0x5f, 0x10,
    0x3, 0x36, 0x16, 0xac,
    0x6, 0x36, 0x30, 0xc0, 0x86, 0x26, 0x32,
    0x3, 0x36, 0x37, 0x29,
    0x5, 0x36, 0x3a, 0x84, 0x04, 0x08,
    0x3, 0x36, 0x41, 0x3a,
    0x3, 0x36, 0x4f, 0x39,
    0x3, 0x36, 0x70, 0xce,
    0x8, 0x36, 0x74, 0xc0, 0xc0, 0xc0, 0x86, 0x8b, 0x8c,
    0x6, 0x36, 0x7c, 0x4b, 0x5f, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x62, 0x63, 0x63,
    0x7, 0x36, 0x99, 0x86, 0x92, 0xa4, 0x48, 0x4b,
    0x4, 0x36, 0xa2, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x09, 0x0c, 0x1c, 0x28,
    0x3, 0x37, 0x0f, 0x01,
    0x4, 0x37, 0x21, 0x6c, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc4,
    0x7, 0x37, 0xb0, 0x09, 0x09, 0x09, 0x48, 0x5f,
    0x6, 0x37, 0xfa, 0x09, 0x32, 0x10, 0x37,
    0x4, 0x39, 0x00, 0x19, 0x02,
    0x3, 0x39, 0x05, 0xb8,
    0x4, 0x39, 0x1b, 0x82, 0x00,
    0x3, 0x39, 0x1f, 0x04,
    0x4, 0x39, 0x33, 0x81, 0x4c,
    0x4, 0x39, 0x3f, 0xff, 0x73,
    0x4, 0x39, 0x42, 0x01, 0x4d,
    0x3, 0x39, 0x46, 0x20,
    0x3, 0x39, 0x57, 0x86,
    0x4, 0x3e, 0x01, 0x95, 0x60,
    0x3, 0x3e, 0x28, 0xc4,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x01, 0xc0,
    0x3, 0x45, 0x09, 0x14,
    0x3, 0x45, 0x0d, 0x11,
    0x3, 0x45, 0x18, 0x00,
    0x3, 0x45, 0x1b, 0x0a,
    0x3, 0x48, 0x19, 0x07,
    0x3, 0x48, 0x1b, 0x04,
    0x3, 0x48, 0x1d, 0x0e,
    0x3, 0x48, 0x1f, 0x03,
    0x3, 0x48, 0x21, 0x09,
    0x3, 0x48, 0x23, 0x04,
    0x3, 0x48, 0x25, 0x03,
    0x3, 0x48, 0x27, 0x03,
    0x3, 0x48, 0x29, 0x06,
    0x6, 0x50, 0x1c, 0x00, 0x60, 0x00, 0x40,
    0x3, 0x57, 0x99, 0x06,
    0x12, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c, 0xfe, 0x40,
    0xe, 0x5a, 0xf4, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x4e,
};

static const struct sc223a_mode supported_modes[] =
{
    [SC223A_960X540] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 960,
        .height = 540,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x46,
        .hts_def = 0x8c8,
        .vts_def = 0x258,
        .xvclk = 24000000,
        .link_freq = 101250000,
        .reg_list = sc223a_960x540_60fps_regs,
        .reg_list_size = ARRAY_SIZE(sc223a_960x540_60fps_regs),
        .hdr_mode = NO_HDR,
    },
    [SC223A_1920X1080] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x46,
        .hts_def = 0x8ca,
        .vts_def = 0x04b0,
        .xvclk = 24000000,
        .link_freq = 202500000,
        .reg_list = sc223a_1920x1080_30fps_regs,
        .reg_list_size = ARRAY_SIZE(sc223a_1920x1080_30fps_regs),
        .hdr_mode = NO_HDR,
    }
};

static ret_err_t sc223a_read_reg(struct sc223a *dev, uint16_t reg, uint8_t *data)
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
            sc223a_err(dev, "read reg, retry=%d, reg [0x%x], ret: %d\n", retry, reg, ret);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc223a_write_reg(struct sc223a *dev, uint16_t reg, uint8_t data)
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
            sc223a_err(dev, "write reg, retry:%d, reg [0x%x]-->0x%x, ret:%d\n", retry, reg, data, ret);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static int sc223a_write_multiple_reg_continue(struct sc223a *dev, const uint8_t *i2c_data, int len)
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
            sc223a_err(dev, "write multi-regs, retry=%d, addr=0x%02x%02x, ret=%d\n",
                       retry, i2c_data[offset + 1], i2c_data[offset + 2], ret);
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

static int sc223a_get_gain_reg(uint8_t *again, uint8_t *dgain,
                               uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    if (total_gain < SC223A_GAIN_MIN)
        total_gain = SC223A_GAIN_MIN;
    else if (total_gain > SC223A_GAIN_MAX)
        total_gain = SC223A_GAIN_MAX;

    gain_factor = total_gain * 1000 / 128;
    if (gain_factor < 1818)          /* 1 ~ 1.81 gain*/
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000;
    }
    else if (gain_factor < 1810 * 2)     /* 1.81 ~ 3.62 gain*/
    {
        *again = 0x40;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1810;
    }
    else if (gain_factor < 1810 * 4)         /* 3.62 ~ 7.24 gain*/
    {
        *again = 0x48;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1810 / 2;
    }
    else if (gain_factor < 1810 * 8)         /* 7.24 ~ 14.48 gain*/
    {
        *again = 0x49;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1810 / 4;
    }
    else if (gain_factor < 1810 * 16)         /* 14.48 ~ 28.96 gain*/
    {
        *again = 0x4b;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1810 / 8;
    }
    else if (gain_factor < 1810 * 32)        /* 28.96 ~ 57.92 gain*/
    {
        *again = 0x4f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1810 / 16;
    }
    else if (gain_factor < 1810 * 64)        /* 57.92 ~ 115.84 gain*/
    {
        //open dgain begin  max digital gain 4X
        *again = 0x5f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1810 / 32;
    }
    else if (gain_factor  < 1810 * 128)        /* 115.84 ~ 231.68 gain*/
    {
        *again = 0x5f;
        *dgain = 0x01;
        *dgain_fine = gain_factor * 128 / 1810 / 64;
    }
    else
    {
        *again = 0x5f;
        *dgain = 0x03;
        *dgain_fine = 0x80;
    }

    return ret;
}
static ret_err_t rk_sc223a_set_expval(struct sc223a *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RT_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, d_gain_fine;
    struct camera_board_desc *sc223a = (struct camera_board_desc *)camera_sc223a;

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming && strcmp(sc223a->isp_subdev_name, RK_ISP_ASSIST_SUBDEV_NAME))
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc223a_dbg(dev, "don't stream, record exp\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0] * 2;
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC223A_EXP_MIN)
        l_exp_time = SC223A_EXP_MIN;

    sc223a_dbg(dev, "total_time reg: 0x%x, total_gain reg 0x%x\n", l_exp_time, l_a_gain);

    ret = sc223a_write_reg(dev, SC223A_REG_EXP_LONG_H,
                           (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc223a_write_reg(dev, SC223A_REG_EXP_LONG_M,
                           (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc223a_write_reg(dev, SC223A_REG_EXP_LONG_L,
                           (uint8_t)((l_exp_time & 0xf) << 4));
    sc223a_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    sc223a_dbg(dev, "a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", a_gain, d_gain, d_gain_fine);

    ret = sc223a_write_reg(dev, SC223A_REG_ANA_GAIN, a_gain);
    ret = sc223a_write_reg(dev, SC223A_REG_DIG_GAIN, d_gain);
    ret = sc223a_write_reg(dev, SC223A_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc223a_set_vts(struct sc223a *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    sc223a_dbg(dev, "set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc223a_write_reg(dev, SC223A_REG_VTS_L,
                           (uint8_t)(dst_vts & 0xff));
    ret = sc223a_write_reg(dev, SC223A_REG_VTS_H,
                           (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc223a_set_flip_mirror(struct sc223a *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    ret = sc223a_read_reg(dev, SC223A_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC223A_FETCH_MIRROR(val, flip & 0x01);
        val = SC223A_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC223A_FETCH_MIRROR(val, false);
        val = SC223A_FETCH_FLIP(val, false);
        break;
    };
    sc223a_dbg(dev, "flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc223a_write_reg(dev, SC223A_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc223a_stream_on(struct sc223a *dev)
{
    sc223a_dbg(dev, "stream_on enter tick:%u\n", rt_tick_get());

    if (dev->i2c_addr == SC223A_DEFAULT_I2CADDR)
        HAL_DelayMs(4);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->i2c_addr == SC223A_DEFAULT_I2CADDR)
        sc223a_write_reg(dev, SC223A_REG_RESET, 0x01);
    sc223a_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc223a_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc223a_set_flip_mirror(dev, dev->flip);

    sc223a_write_reg(dev, SC223A_REG_STREAM_CTRL, SC223A_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc223a_dbg(dev, "stream_on exit tick:%u\n", rt_tick_get());
}

static void sc223a_stream_on_late(struct sc223a *dev)
{
    sc223a_dbg(dev, "stream_on_late enter tick:%u\n", rt_tick_get());

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc223a_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc223a_set_flip_mirror(dev, dev->flip);

    sc223a_write_reg(dev, SC223A_REG_STREAM_CTRL, SC223A_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc223a_dbg(dev, "stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc223a_stream_off(struct sc223a *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc223a_write_reg(dev, SC223A_REG_STREAM_CTRL, SC223A_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    dev->streaming = false;
}

ret_err_t rk_sc223a_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RT_EOK;
    struct sc223a *sc223a;

    RT_ASSERT(dev != RT_NULL);
    sc223a = (struct sc223a *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc223a->parent;

    if (sc223a)
    {
        camera->info.mbus_fmt.width = sc223a->cur_mode->width;
        camera->info.mbus_fmt.height = sc223a->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc223a->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc223a->cur_mode->hdr_mode;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_sc223a_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc223a_close(struct rk_camera_device *dev)
{
    uint8_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_sc223a_get_expinf(struct sc223a *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RT_EOK;
    const struct sc223a_mode *mode;

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

static ret_err_t rk_sc223a_get_intput_fmt(struct sc223a *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc223a_mode *mode;

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc223a_set_intput_fmt(struct sc223a *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc223a_mode *mode;
    int i;
    bool is_find_fmt = false;

    sc223a_info(dev, "dst resulotion, width %d, height %d\n",
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
            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
            if (dev->i2c_addr == SC223A_DEFAULT_I2CADDR)
                sc223a_write_reg(dev, SC223A_REG_RESET, 0x01);
            sc223a_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
            ret = rk_sc223a_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc223a_match_dst_config(struct sc223a *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc223a_mode *mode;
    int i, cur_fps, dst_fps, cur_vts, dst_vts;

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc223a_info(dev, "cur resulotion, width %d, height %d, fps %d!\n",
                dev->cur_mode->width, dev->cur_mode->height,
                dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];
        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc223a_mode *)mode;
            ret = RT_EOK;
            sc223a_info(dev, "find dst resulotion, width %d, height %d, fps %d!\n",
                        dst_config->width, dst_config->height, dst_fps);
            break;
        }
    }
    if (i == ARRAY_SIZE(supported_modes))
    {
        sc223a_err(dev, "not find match resulotion\n");
        return -RT_EINVAL;
    }

    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return RT_EOK;

    if (dst_fps > cur_fps)
    {
        sc223a_err(dev, "dst fps is larger than cur fps\n");
        return -RT_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc223a_control(struct rk_camera_device *dev,
                            dt_cmd_t cmd,
                            void *args)
{
    ret_err_t ret = RT_EOK;
    struct sc223a *sc223a;

    RT_ASSERT(dev != RT_NULL);
    sc223a = (struct sc223a *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc223a_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc223a_stream_on(sc223a);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc223a_stream_off(sc223a);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc223a_get_expinf(sc223a, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc223a_set_expval(sc223a, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc223a_set_vts(sc223a, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc223a_get_intput_fmt(sc223a, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc223a_set_intput_fmt(sc223a, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc223a_match_dst_config(sc223a, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc223a_set_flip_mirror(sc223a, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc223a_stream_on_late(sc223a);
    }
    break;
    default:
        sc223a_dbg(sc223a, "exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc223a_ops =
{
    .init       = rk_sc223a_init,
    .open       = rk_sc223a_open,
    .control    = rk_sc223a_control,
};

int rk_camera_sc223a_init(void)
{
    ret_err_t ret = RT_EOK;
    const struct camera_board_desc *sc223a_desc;
    // int val_h = 0, val_l = 0;

    for (sc223a_desc = camera_sc223a; sc223a_desc->i2c_addr != 0; sc223a_desc++)
    {
        struct sc223a *sc223a;
        struct rk_camera_device *camera;
        int val_h, val_l;

        sc223a = rt_malloc(sizeof(struct sc223a));
        if (!sc223a)
        {
            rt_kprintf("malloc sc223a failed\n");
            return -RT_ENOMEM;
        }

        rt_memset(sc223a, 0x0, sizeof(struct sc223a));

        camera = &sc223a->parent;
        camera->ops = &rk_sc223a_ops;

        rt_memcpy(sc223a->name, sc223a_desc->isp_subdev_name, RK_CAMERA_DEVICE_NAME_SIZE);
        rt_memcpy(sc223a->i2c_name, sc223a_desc->i2c_bus, RK_CAMERA_I2C_NAME_SIZE);

        sc223a->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc223a->i2c_name);
        if (sc223a->i2c_bus == RT_NULL)
        {
            sc223a_err(sc223a, "can't find i2c bus:%s\n", sc223a->i2c_name);
            rt_free(sc223a);
            continue;
        }

        if (sc223a_desc->mode_id <= SC223A_MODE_ID_MAX)
        {
            sc223a_dbg(sc223a, "mode_id: %d\n", sc223a_desc->mode_id);
            sc223a->cur_mode = &supported_modes[sc223a_desc->mode_id];
        }
        else
        {
            sc223a_info(sc223a, "mode id is over range, default use mode_id: 0\n");
            sc223a->cur_mode = &supported_modes[0];
        }

        sc223a->time_valid_delay = sc223a_desc->time_valid_delay;
        sc223a->gain_valid_delay = sc223a_desc->gain_valid_delay;
        sc223a->i2c_addr = sc223a_desc->i2c_addr;

        if (sc223a_desc->rst_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc223a_desc->rst_gpio.gpio_group,
                                     sc223a_desc->rst_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc223a_desc->rst_gpio.gpio_group,
                                 sc223a_desc->rst_gpio.gpio_pin,
                                 !sc223a_desc->rst_active);
        }
        if (sc223a_desc->pwdn_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc223a_desc->pwdn_gpio.gpio_group,
                                     sc223a_desc->pwdn_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc223a_desc->pwdn_gpio.gpio_group,
                                 sc223a_desc->pwdn_gpio.gpio_pin,
                                 !sc223a_desc->pwdn_active);
        }

        if (sc223a_desc->mclk_out_gate_id && sc223a_desc->mclk_id)
        {
            clk_set_rate(sc223a_desc->mclk_id, sc223a->cur_mode->xvclk);
            HAL_CRU_ClkEnable(sc223a_desc->mclk_out_gate_id);
        }

        if (sc223a->i2c_addr != SC223A_DEFAULT_I2CADDR)
        {
            /* change the i2c address via register */
            int i2c_addr = sc223a->i2c_addr;

            HAL_DelayMs(4);
            sc223a_info(sc223a, "change i2c_addr from 0x%02x to 0x%02x\n", SC223A_DEFAULT_I2CADDR, i2c_addr);
            sc223a->i2c_addr = SC223A_DEFAULT_I2CADDR;
            sc223a_write_reg(sc223a, SC223A_REG_I2C_ADDR, i2c_addr << 1);
            sc223a->i2c_addr = i2c_addr;
        }

        rt_mutex_init(&sc223a->mutex_lock, "sc223a_mutex", RT_IPC_FLAG_FIFO);
        RT_ASSERT(rt_object_get_type(&sc223a->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
        camera->i2c_bus = sc223a->i2c_bus;
        rt_memcpy(camera->name, sc223a->name, RK_CAMERA_DEVICE_NAME_SIZE);
        ret = rk_camera_register(camera, camera->name, sc223a);
        if (ret)
        {
            sc223a_err(sc223a, "regster failed\n");
            rt_free(sc223a);
        }
    }

    return ret;
}

#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc223a_init);
#endif
#endif
