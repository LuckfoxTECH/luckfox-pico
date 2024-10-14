/**
  * Copyright (c) 2023 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc4336pp.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2023-09-02     Yuefu Su      first implementation
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

#ifdef RT_USING_SC4336P

RT_WEAK const struct camera_board_desc camera_sc4336p[] = {0};

#define SC4336P_DEBUG_PRT      0

#define sc4336p_info(dev, fmt, ...)          rt_kprintf("sc4336p-%s "fmt"", dev->name, ##__VA_ARGS__)
#define sc4336p_err(dev, fmt, ...)           rt_kprintf("sc4336p-%s error "fmt"", dev->name, ##__VA_ARGS__)
#if SC4336P_DEBUG_PRT
#include <stdio.h>
#define sc4336p_dbg(dev, fmt, ...)           rt_kprintf("sc4336p-%s "fmt"", dev->name, ##__VA_ARGS__)
#else
#define sc4336p_dbg(dev, fmt, ...)
#endif

#define SC4336P_CHIP_ID                  0x9c42
#define SC4336P_REG_CHIP_ID_H            0x3107
#define SC4336P_REG_CHIP_ID_L            0x3108
#define SC4336P_REG_EXP_LONG_H           0x3e00
#define SC4336P_REG_EXP_LONG_M           0x3e01
#define SC4336P_REG_EXP_LONG_L           0x3e02
#define SC4336P_REG_EXP_SHORT_H          0x3e22
#define SC4336P_REG_EXP_SHORT_M          0x3e04
#define SC4336P_REG_EXP_SHORT_L          0x3e05
#define SC4336P_REG_DIG_GAIN            0x3e06
#define SC4336P_REG_DIG_FINE_GAIN       0x3e07
#define SC4336P_REG_ANA_GAIN            0x3e09
#define SC4336P_REG_SDIG_GAIN           0x3e10
#define SC4336P_REG_SDIG_FINE_GAIN      0x3e11
#define SC4336P_REG_SANA_GAIN           0x3e12
#define SC4336P_REG_SANA_FINE_GAIN      0x3e13
#define SC4336P_EXP_MIN                 2
#define SC4336P_GAIN_MIN                0x0020
#define SC4336P_GAIN_MAX                (16128)  //32 * 15.75 * 32
#define SC4336P_GAIN_STEP               1
#define SC4336P_GAIN_DEFAULT            0x0020
#define SC4336P_LGAIN                   0
#define SC4336P_SGAIN                   1
#define SC4336P_FLIP_MIRROR_REG         0x3221
#define SC4336P_REG_VTS_H               0x320e
#define SC4336P_REG_VTS_L               0x320f
#define SC4336P_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC4336P_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC4336P_REG_STREAM_CTRL         0x0100
#define SC4336P_STREAM_ON               0x01
#define SC4336P_STREAM_OFF              0x00

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

struct sc4336p_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc4336p_mode
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
struct sc4336p
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
    const struct sc4336p_mode *cur_mode;
    struct sc4336p_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

/*
 * Xclk 24Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 315Mbps, 2lane
 * bining to 1280x720
 */
static const uint8_t g_sc4336p_1280x720_60fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x16,
    0x3, 0x30, 0xb8, 0x44,
    0xa, 0x32, 0x08, 0x05, 0x00, 0x02, 0xd0, 0x0b, 0x15, 0x02, 0xe4,
    0x3, 0x32, 0x11, 0x02,
    0x3, 0x32, 0x13, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x53, 0x10,
    0x4, 0x33, 0x01, 0x0a, 0xff,
    0x4, 0x33, 0x05, 0x00, 0x90,
    0x3, 0x33, 0x08, 0x08,
    0x4, 0x33, 0x0a, 0x01, 0xb0,
    0x3, 0x33, 0x0d, 0xf0,
    0x3, 0x33, 0x14, 0x14,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x3, 0x33, 0x7d, 0x0e,
    0x10, 0x33, 0x8f, 0x20, 0x08, 0x09, 0x0f, 0x18, 0x60, 0xff, 0x08, 0x09, 0x0f, 0x0a, 0x18, 0x60, 0xff,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xad, 0x0c,
    0x4, 0x33, 0xb2, 0x40, 0x30,
    0x8, 0x33, 0xf8, 0x00, 0xb0, 0x00, 0xf8, 0x09, 0x1f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x09, 0x1f, 0x28, 0x28, 0x01, 0xe0, 0x02, 0x28,
    0x4, 0x34, 0xf8, 0x1f, 0x20,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x54, 0x44,
    0x3, 0x36, 0x37, 0x49,
    0x3, 0x36, 0x3f, 0xc0,
    0x3, 0x36, 0x41, 0x28,
    0x3, 0x36, 0x70, 0x56,
    0x8, 0x36, 0x74, 0xb0, 0xa0, 0xa0, 0x84, 0x88, 0x8d,
    0x6, 0x36, 0x7c, 0x09, 0x0b, 0x08, 0x0f,
    0x5, 0x36, 0x96, 0x24, 0x34, 0x34,
    0x4, 0x36, 0xa0, 0x0f, 0x1f,
    0x9, 0x36, 0xb0, 0x81, 0x83, 0x85, 0x8b, 0x09, 0x0b, 0x0f,
    0x4, 0x36, 0xec, 0x1c, 0x2a,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x3, 0x37, 0x24, 0x21,
    0x5, 0x37, 0x71, 0x09, 0x05, 0x05,
    0x4, 0x37, 0x7a, 0x0f, 0x1f,
    0x3, 0x39, 0x05, 0x8c,
    0x3, 0x39, 0x1d, 0x02,
    0x3, 0x39, 0x1f, 0x49,
    0x3, 0x39, 0x26, 0x21,
    0x4, 0x39, 0x33, 0x80, 0x03,
    0x3, 0x39, 0x37, 0x7b,
    0x4, 0x39, 0x39, 0x00, 0x00,
    0x3, 0x39, 0xdc, 0x02,
    0x5, 0x3e, 0x00, 0x00, 0x2d, 0xc0,
    0x3, 0x44, 0x0e, 0x02,
    0x3, 0x45, 0x09, 0x28,
    0x3, 0x45, 0x0d, 0x32,
    0x3, 0x48, 0x19, 0x05,
    0x3, 0x48, 0x1b, 0x03,
    0x3, 0x48, 0x1d, 0x09,
    0x3, 0x48, 0x1f, 0x02,
    0x3, 0x48, 0x21, 0x08,
    0x3, 0x48, 0x23, 0x02,
    0x3, 0x48, 0x25, 0x02,
    0x3, 0x48, 0x27, 0x02,
    0x3, 0x48, 0x29, 0x04,
    0x3, 0x50, 0x00, 0x46,
    0x3, 0x57, 0x8d, 0x40,
    0x4, 0x57, 0x99, 0x46, 0x77,
    0x4, 0x57, 0xd9, 0x46, 0x77,
    0x4, 0x59, 0x00, 0xf1, 0x04,
    0x3, 0x59, 0x88, 0x70,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x4b,
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 630Mbps, 2lane
 * full resolution
 */
static const uint8_t g_sc4336p_2560x1440_30fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x05,
    0x3, 0x30, 0xb8, 0x44,
    0x4, 0x32, 0x0e, 0x05, 0xdc,
    0x3, 0x32, 0x53, 0x10,
    0x4, 0x33, 0x01, 0x0a, 0xff,
    0x4, 0x33, 0x05, 0x00, 0x90,
    0x3, 0x33, 0x08, 0x08,
    0x4, 0x33, 0x0a, 0x01, 0xb0,
    0x3, 0x33, 0x0d, 0xf0,
    0x3, 0x33, 0x14, 0x14,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x3, 0x33, 0x7d, 0x0e,
    0x10, 0x33, 0x8f, 0x20, 0x08, 0x09, 0x0f, 0x18, 0x60, 0xff, 0x08, 0x09, 0x0f, 0x0a, 0x18, 0x60, 0xff,
    0x3, 0x33, 0xa2, 0x04,
    0x3, 0x33, 0xad, 0x0c,
    0x4, 0x33, 0xb2, 0x40, 0x30,
    0x8, 0x33, 0xf8, 0x00, 0xb0, 0x00, 0xf8, 0x09, 0x1f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x09, 0x1f, 0x28, 0x28, 0x01, 0xe0, 0x02, 0x28,
    0x4, 0x34, 0xf8, 0x1f, 0x20,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x54, 0x44,
    0x3, 0x36, 0x37, 0x49,
    0x3, 0x36, 0x3f, 0xc0,
    0x3, 0x36, 0x41, 0x28,
    0x3, 0x36, 0x70, 0x56,
    0x8, 0x36, 0x74, 0xb0, 0xa0, 0xa0, 0x84, 0x88, 0x8d,
    0x6, 0x36, 0x7c, 0x09, 0x0b, 0x08, 0x0f,
    0x5, 0x36, 0x96, 0x24, 0x34, 0x34,
    0x4, 0x36, 0xa0, 0x0f, 0x1f,
    0x9, 0x36, 0xb0, 0x81, 0x83, 0x85, 0x8b, 0x09, 0x0b, 0x0f,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x3, 0x37, 0x24, 0x21,
    0x5, 0x37, 0x71, 0x09, 0x05, 0x05,
    0x4, 0x37, 0x7a, 0x0f, 0x1f,
    0x3, 0x39, 0x05, 0x8c,
    0x3, 0x39, 0x1d, 0x02,
    0x3, 0x39, 0x1f, 0x49,
    0x3, 0x39, 0x26, 0x21,
    0x4, 0x39, 0x33, 0x80, 0x03,
    0x3, 0x39, 0x37, 0x7b,
    0x4, 0x39, 0x39, 0x00, 0x00,
    0x3, 0x39, 0xdc, 0x02,
    0x5, 0x3e, 0x00, 0x00, 0x5d, 0x40,
    0x4, 0x44, 0x0d, 0x10, 0x01,
    0x3, 0x45, 0x09, 0x28,
    0x3, 0x45, 0x0d, 0x32,
    0x3, 0x50, 0x00, 0x06,
    0x3, 0x57, 0x80, 0x76,
    0x4, 0x57, 0x84, 0x10, 0x04,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x08, 0x0a, 0x0a, 0x08, 0x40,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04,
    0x4, 0x57, 0x99, 0x46, 0x77,
    0x3, 0x57, 0xa1, 0x04,
    0x3, 0x57, 0xa8, 0xd2,
    0x6, 0x57, 0xaa, 0x2a, 0x7f, 0x00, 0x00,
    0x4, 0x57, 0xd9, 0x46, 0x77,
    0xe, 0x59, 0xe2, 0x08, 0x03, 0x00, 0x10, 0x06, 0x00, 0x08, 0x02, 0x00, 0x10, 0x04, 0x00,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x53,
    0x42,
};

static const struct sc4336p_mode supported_modes[] =
{
    [SC4336P_1280X720] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1280,
        .height = 720,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x80,
        .hts_def = 0x0b15,
        .vts_def = 0x02e4,
        .xvclk = 24000000,
        .link_freq = 157500000,
        .reg_list = g_sc4336p_1280x720_60fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc4336p_1280x720_60fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [SC4336P_2560X1440] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2560,
        .height = 1440,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x0080,
        .hts_def = 0x0578 * 2,
        .vts_def = 0x05dc,
        .xvclk = 24000000,
        .link_freq = 315000000,
        .reg_list = g_sc4336p_2560x1440_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc4336p_2560x1440_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct sc4336p_ops
{
    ret_err_t (*init)(struct sc4336p *dev);
    ret_err_t (*open)(struct sc4336p *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc4336p *dev);
    ret_err_t (*control)(struct sc4336p *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t sc4336p_read_reg(struct sc4336p *dev, uint16_t reg, uint8_t *data)
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
            sc4336p_err(dev, "read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc4336p_write_reg(struct sc4336p *dev, uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

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
            sc4336p_err(dev, "write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static int sc4336p_write_multiple_reg_continue(struct sc4336p *dev, const uint8_t *i2c_data, int len)
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
            sc4336p_err(dev, "write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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

static int sc4336p_get_gain_reg(uint8_t *again, uint8_t *dgain,
                                uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    gain_factor = total_gain * 1000 / 32;
    if (gain_factor < 1000)
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = 0x80;
    }
    else if (gain_factor < 2000)          /* 1 ~ 2 gain*/
    {
        *again = 0x00;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 1000;
    }
    else if (gain_factor < 4000)     /* 2 ~ 4 gain*/
    {
        *again = 0x08;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 2000;
    }
    else if (gain_factor < 8000)         /* 4 ~ 8 gain*/
    {
        *again = 0x09;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 4000;
    }
    else if (gain_factor < 16000)         /* 8 ~ 16 gain*/
    {
        *again = 0x0b;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 8000;
    }
    else if (gain_factor < 32000)         /* 16 ~ 32 gain*/
    {
        *again = 0x0f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 16000;
    }
    //open dgain begin  max digital gain 16X
    else if (gain_factor < 32000 * 2)        /* 32 ~ 64 gain*/
    {
        *again = 0x1f;
        *dgain = 0x00;
        *dgain_fine = gain_factor * 128 / 32000;
    }
    else if (gain_factor < 32000 * 4)        /* 64 ~ 128 gain*/
    {
        *again = 0x1f;
        *dgain = 0x01;
        *dgain_fine = gain_factor * 128 / 32000 / 2;
    }
    else if (gain_factor  < 32000 * 8)        /* 128 ~ 256 gain*/
    {
        *again = 0x1f;
        *dgain = 0x03;
        *dgain_fine = gain_factor * 128 / 32000 / 4;
    }
    else if (gain_factor < 32000 * 16)       /* 256 ~ 512 gain*/
    {
        *again = 0x1f;
        *dgain = 0x07;
        *dgain_fine = gain_factor * 128 / 32000 / 8;
    }
    else                                     /* MAX 48.64*16*128*/
    {
        *again = 0x1f;
        *dgain = 0x07;
        *dgain_fine = 0xf0;
    }
    *dgain_fine = *dgain_fine / 4 * 4;

    return ret;
}

static ret_err_t rk_sc4336p_set_expval(struct sc4336p *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RT_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, d_gain_fine;

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming && strcmp(dev->name, RK_ISP_ASSIST_SUBDEV_NAME))
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc4336p_dbg(dev, "sc4336p don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC4336P_EXP_MIN)
        l_exp_time = SC4336P_EXP_MIN;

    sc4336p_dbg(dev, "total_time reg: 0x%x, total_gain reg 0x%x\n", l_exp_time, l_a_gain);

    ret = sc4336p_write_reg(dev, SC4336P_REG_EXP_LONG_H,
                            (uint8_t)((l_exp_time >> 12) & 0xf));
    ret = sc4336p_write_reg(dev, SC4336P_REG_EXP_LONG_M,
                            (uint8_t)((l_exp_time >> 4) & 0xff));
    ret = sc4336p_write_reg(dev, SC4336P_REG_EXP_LONG_L,
                            (uint8_t)((l_exp_time & 0xf) << 4));
    sc4336p_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    sc4336p_dbg(dev, "a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", a_gain, d_gain, d_gain_fine);

    ret = sc4336p_write_reg(dev, SC4336P_REG_ANA_GAIN, a_gain);
    ret = sc4336p_write_reg(dev, SC4336P_REG_DIG_GAIN, d_gain);
    ret = sc4336p_write_reg(dev, SC4336P_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc4336p_set_vts(struct sc4336p *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    sc4336p_dbg(dev, "set vts: 0x%x \n", dst_vts);

    ret = sc4336p_write_reg(dev, SC4336P_REG_VTS_L,
                            (uint8_t)(dst_vts & 0xff));
    ret = sc4336p_write_reg(dev, SC4336P_REG_VTS_H,
                            (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc4336p_set_flip_mirror(struct sc4336p *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    ret = sc4336p_read_reg(dev, SC4336P_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC4336P_FETCH_MIRROR(val, flip & 0x01);
        val = SC4336P_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC4336P_FETCH_MIRROR(val, false);
        val = SC4336P_FETCH_FLIP(val, false);
        break;
    };
    sc4336p_dbg(dev, "flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc4336p_write_reg(dev, SC4336P_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc4336p_stream_on(struct sc4336p *dev)
{
    sc4336p_dbg(dev, "stream_on enter tick:%u\n", rt_tick_get());

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    HAL_DelayMs(5);

    sc4336p_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc4336p_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc4336p_set_flip_mirror(dev, dev->flip);

    sc4336p_write_reg(dev, SC4336P_REG_STREAM_CTRL, SC4336P_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc4336p_dbg(dev, "stream_on exit tick:%u\n", rt_tick_get());
}

static void sc4336p_stream_on_late(struct sc4336p *dev)
{

    sc4336p_dbg(dev, "stream_on_late enter tick:%u\n", rt_tick_get());

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc4336p_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc4336p_set_flip_mirror(dev, dev->flip);

    sc4336p_write_reg(dev, SC4336P_REG_STREAM_CTRL, SC4336P_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc4336p_dbg(dev, "stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc4336p_stream_off(struct sc4336p *dev)
{
    if (dev->i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc4336p_write_reg(dev, SC4336P_REG_STREAM_CTRL, SC4336P_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    dev->streaming = false;
}

ret_err_t rk_sc4336p_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RT_EOK;
    struct sc4336p *sc4336p;

    RT_ASSERT(dev != RT_NULL);
    sc4336p = (struct sc4336p *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc4336p->parent;

    if (sc4336p)
    {
        camera->info.mbus_fmt.width = sc4336p->cur_mode->width;
        camera->info.mbus_fmt.height = sc4336p->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc4336p->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc4336p->cur_mode->hdr_mode;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc4336p_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc4336p_close(struct rk_camera_device *dev)
{
    uint8_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_sc4336p_get_expinf(struct sc4336p *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RT_EOK;
    const struct sc4336p_mode *mode;

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

static ret_err_t rk_sc4336p_get_intput_fmt(struct sc4336p *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc4336p_mode *mode;

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc4336p_set_intput_fmt(struct sc4336p *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc4336p_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    sc4336p_info(dev, "dst resulotion, width %d, height %d\n",
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
            sc4336p_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
            ret = rk_sc4336p_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_sc4336p_match_dst_config(struct sc4336p *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc4336p_mode *mode;
    int i, cur_fps, dst_fps, cur_vts, dst_vts;

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc4336p_info(dev, "cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height,
                 dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc4336p_mode *)mode;
            ret = RT_EOK;
            sc4336p_info(dev, "find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
    }
    if (i == ARRAY_SIZE(supported_modes))
    {
        sc4336p_err(dev, "not find match resulotion\n");
        return -RT_EINVAL;
    }

    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return RT_EOK;

    if (dst_fps > cur_fps)
    {
        sc4336p_err(dev, "dst fps is larger than cur fps\n");
        return -RT_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc4336p_control(struct rk_camera_device *dev,
                             dt_cmd_t cmd,
                             void *args)
{
    ret_err_t ret = RT_EOK;
    struct sc4336p *sc4336p;

    RT_ASSERT(dev != RT_NULL);
    sc4336p = (struct sc4336p *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc4336p_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc4336p_stream_on(sc4336p);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc4336p_stream_off(sc4336p);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc4336p_get_expinf(sc4336p, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc4336p_set_expval(sc4336p, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc4336p_set_vts(sc4336p, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc4336p_get_intput_fmt(sc4336p, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc4336p_set_intput_fmt(sc4336p, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc4336p_match_dst_config(sc4336p, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc4336p_set_flip_mirror(sc4336p, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc4336p_stream_on_late(sc4336p);
    }
    break;
    default:
        sc4336p_dbg(sc4336p, "exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc4336p_ops =
{
    .init = rk_sc4336p_init,
    .open = rk_sc4336p_open,
    .control = rk_sc4336p_control,
};

int rk_sc4336p_check_sensor_id(struct sc4336p *dev)
{
    ret_err_t ret = RT_EOK;
    uint8_t val_h, val_l;

    HAL_DelayMs(4);
    ret = sc4336p_write_reg(dev, 0x440d, 0x10);
    ret |= sc4336p_write_reg(dev, 0x4400, 0x11);
    HAL_DelayMs(2);

    ret |= sc4336p_read_reg(dev, SC4336P_REG_CHIP_ID_H, &val_h);
    ret |= sc4336p_read_reg(dev, SC4336P_REG_CHIP_ID_L, &val_l);
    if ((val_h << 8 | val_l) != SC4336P_CHIP_ID)
    {
        sc4336p_err(dev, "Unexpected sensor id(%06x), ret(%d)\n", val_h << 8 | val_l, ret);
        return RT_EINVAL;
    }
    sc4336p_info(dev, "Detected SC%04x sensor\n", SC4336P_CHIP_ID);
    return ret;
}

int rk_camera_sc4336p_init(void)
{
    ret_err_t ret = RT_EOK;
    const struct camera_board_desc *sc4336p_desc;

    for (sc4336p_desc = camera_sc4336p; sc4336p_desc->i2c_addr != 0; sc4336p_desc++)
    {
        struct sc4336p *sc4336p;
        struct clk_gate *clkgate;
        struct clk_rate *clkrate;
        struct rk_camera_device *camera;

        sc4336p = rt_malloc(sizeof(struct sc4336p));
        if (!sc4336p)
        {
            rt_kprintf("malloc sc4336p failed\n");
            return -RT_ENOMEM;
        }

        rt_memset(sc4336p, 0x0, sizeof(struct sc4336p));

        camera = &sc4336p->parent;
        camera->ops = &rk_sc4336p_ops;

        rt_strncpy(sc4336p->name, sc4336p_desc->isp_subdev_name, RK_CAMERA_DEVICE_NAME_SIZE);
        rt_strncpy(sc4336p->i2c_name, sc4336p_desc->i2c_bus, RK_CAMERA_I2C_NAME_SIZE);

        sc4336p->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc4336p->i2c_name);
        if (sc4336p->i2c_bus == RT_NULL)
        {
            sc4336p_err(sc4336p, "can't find i2c bus:%s\n", sc4336p->i2c_name);
            rt_free(sc4336p);
            continue;
        }

        if (sc4336p_desc->mode_id <= SC4336P_MODE_ID_MAX)
        {
            sc4336p_dbg(sc4336p, "mode_id: %d\n", sc4336p_desc->mode_id);
            sc4336p->cur_mode = &supported_modes[sc4336p_desc->mode_id];
        }
        else
        {
            sc4336p_info(sc4336p, "mode id is over range, default use mode_id: 0\n");
            sc4336p->cur_mode = &supported_modes[0];
        }

        sc4336p->dst_mode = (struct sc4336p_mode *)&supported_modes[SC4336P_2560X1440];
        sc4336p->time_valid_delay = sc4336p_desc->time_valid_delay;
        sc4336p->gain_valid_delay = sc4336p_desc->gain_valid_delay;
        sc4336p->i2c_addr = sc4336p_desc->i2c_addr;

        if (sc4336p_desc->rst_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc4336p_desc->rst_gpio.gpio_group,
                                     sc4336p_desc->rst_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc4336p_desc->rst_gpio.gpio_group,
                                 sc4336p_desc->rst_gpio.gpio_pin,
                                 !sc4336p_desc->rst_active);
        }
        if (sc4336p_desc->pwdn_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc4336p_desc->pwdn_gpio.gpio_group,
                                     sc4336p_desc->pwdn_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc4336p_desc->pwdn_gpio.gpio_group,
                                 sc4336p_desc->pwdn_gpio.gpio_pin,
                                 !sc4336p_desc->pwdn_active);
        }

        if (sc4336p_desc->mclk_out_gate_id && sc4336p_desc->mclk_id)
        {
            clk_set_rate(sc4336p_desc->mclk_id, sc4336p_desc->cur_mode->xvclk);
            HAL_CRU_ClkEnable(sc4336p_desc->mclk_out_gate_id);
        }

        rt_mutex_init(&sc4336p->mutex_lock, "sc4336p_mutex", RT_IPC_FLAG_FIFO);
        RT_ASSERT(rt_object_get_type(&sc4336p->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
        camera->i2c_bus = sc4336p->i2c_bus;
        rt_strncpy(camera->name, sc4336p->name, RK_CAMERA_DEVICE_NAME_SIZE);
        rk_camera_register(camera, camera->name, sc4336p);
        if (ret)
        {
            sc4336p_err(sc4336p, "regster failed\n");
            rt_free(sc4336p);
        }
        rk_sc4336p_check_sensor_id(sc4336p);
    }
    return ret;
}

#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc4336p_init);
#endif
#endif
