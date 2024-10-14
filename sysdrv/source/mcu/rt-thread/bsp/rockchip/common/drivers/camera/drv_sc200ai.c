/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc200ai.c
  * @version V0.0.2
  *
  * Change Logs:
  * 2022-07-07     ISP Team      first implementation
  * 2023-12-11     PD3           add support 60fps config
  ******************************************************************************
  */

#include "camera.h"
#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"
#include <rtconfig.h>
#include "board.h"
#include "drv_clock.h"
#include "board.h"

#ifdef RT_USING_SC200AI

RT_WEAK const struct camera_board_desc camera_sc200ai[] = {0};

#define SC200AI_DEBUG_PRT      0

#define sc200ai_info(dev, fmt, ...)          rt_kprintf("sc200ai-%s "fmt"", dev->name, ##__VA_ARGS__)
#define sc200ai_err(dev, fmt, ...)           rt_kprintf("sc200ai-%s error "fmt"", dev->name, ##__VA_ARGS__)
#if SC200AI_DEBUG_PRT
#include <stdio.h>
#define sc200ai_dbg(dev, fmt, ...)           rt_kprintf("sc200ai-%s "fmt"", dev->name, ##__VA_ARGS__)
#else
#define sc200ai_dbg(dev, fmt, ...)
#endif

#define SC200AI_REG_RESET           0x0103
#define SC200AI_REG_EXPOSURE_H      0x3e00
#define SC200AI_REG_EXPOSURE_M      0x3e01
#define SC200AI_REG_EXPOSURE_L      0x3e02
#define SC200AI_REG_SEXPOSURE_H     0x3e22
#define SC200AI_REG_SEXPOSURE_M     0x3e04
#define SC200AI_REG_SEXPOSURE_L     0x3e05
#define SC200AI_REG_DIG_GAIN        0x3e06
#define SC200AI_REG_DIG_FINE_GAIN   0x3e07
#define SC200AI_REG_ANA_GAIN        0x3e08
#define SC200AI_REG_ANA_FINE_GAIN   0x3e09
#define SC200AI_REG_SDIG_GAIN       0x3e10
#define SC200AI_REG_SDIG_FINE_GAIN  0x3e11
#define SC200AI_REG_SANA_GAIN       0x3e12
#define SC200AI_REG_SANA_FINE_GAIN  0x3e13
#define SC200AI_REG_MAX_SEXPOSURE_H 0x3e23
#define SC200AI_REG_MAX_SEXPOSURE_L 0x3e24
#define SC200AI_GAIN_MIN        0x0040
#define SC200AI_GAIN_MAX        (54 * 32 * 64)       //53.975*31.75*64
#define SC200AI_GAIN_STEP       1
#define SC200AI_GAIN_DEFAULT        0x0800
#define SC200AI_LGAIN           0
#define SC200AI_SGAIN           1
#define SC200AI_FLIP_MIRROR_REG         0x3221
#define SC200AI_REG_VTS_H               0x320e
#define SC200AI_REG_VTS_L               0x320f

#define SC200AI_REG_STREAM_CTRL                  0x0100
#define SC200AI_STREAM_ON                        0x01
#define SC200AI_STREAM_OFF                       0x00

#define SC200AI_FETCH_EXP_H(VAL)        (((VAL) >> 12) & 0xF)
#define SC200AI_FETCH_EXP_M(VAL)        (((VAL) >> 4) & 0xFF)
#define SC200AI_FETCH_EXP_L(VAL)        (((VAL) & 0xF) << 4)

#define SC200AI_FETCH_AGAIN_H(VAL)      (((VAL) >> 8) & 0x03)
#define SC200AI_FETCH_AGAIN_L(VAL)      ((VAL) & 0xFF)

#define SC200AI_FETCH_MIRROR(VAL, ENABLE)   (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC200AI_FETCH_FLIP(VAL, ENABLE)     (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC200AI_REG_TEST_PATTERN    0x4501
#define SC200AI_TEST_PATTERN_BIT_MASK   BIT(3)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define SC200AI_PIN_CTRL_ENABLE          1
#define SC200AI_I2C_DEBUG_ENABLE         0
#define SC200AI_TESTPATTERN_ENABLE       0

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

struct sc200ai_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));


struct sc200ai_mode
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
struct sc200ai
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
    const struct sc200ai_mode *cur_mode;
    struct sc200ai_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

struct sc200ai_ops
{
    ret_err_t (*init)(struct sc200ai *dev);
    ret_err_t (*open)(struct sc200ai *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc200ai *dev);
    ret_err_t (*control)(struct sc200ai *dev, dt_cmd_t cmd, void *arg);
};

//27MHz
//371.25Mbps 2lane 960x540 120fps
static const uint8_t sc200ai_linear_960x540_120fps_regs[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x4e,
    0xa, 0x32, 0x08, 0x03, 0xc0, 0x02, 0x1c, 0x04, 0x4c, 0x02, 0x32,
    0x3, 0x32, 0x11, 0x02,
    0x3, 0x32, 0x13, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x17,
    0x3, 0x32, 0x43, 0x01,
    0x4, 0x32, 0x48, 0x02, 0x09,
    0x3, 0x32, 0x53, 0x08,
    0x3, 0x32, 0x71, 0x0a,
    0x6, 0x33, 0x01, 0x06, 0x0c, 0x08, 0x60,
    0x3, 0x33, 0x06, 0x30,
    0x4, 0x33, 0x08, 0x10, 0x70,
    0x3, 0x33, 0x0b, 0x80,
    0x6, 0x33, 0x0d, 0x16, 0x1c, 0x02, 0x02,
    0x3, 0x33, 0x1c, 0x04,
    0x5, 0x33, 0x1e, 0x51, 0x61, 0x07,
    0x3, 0x33, 0x33, 0x10,
    0x3, 0x33, 0x4c, 0x08,
    0x3, 0x33, 0x56, 0x09,
    0x3, 0x33, 0x64, 0x17,
    0xf, 0x33, 0x90, 0x08, 0x18, 0x38, 0x06, 0x06, 0x06, 0x08, 0x18, 0x38, 0x06, 0x0a, 0x10, 0x20,
    0x3, 0x33, 0xac, 0x08,
    0x4, 0x33, 0xae, 0x10, 0x19,
    0x4, 0x36, 0x21, 0xe8, 0x16,
    0x3, 0x36, 0x30, 0xa0,
    0x3, 0x36, 0x37, 0x36,
    0x5, 0x36, 0x3a, 0x1f, 0xc6, 0x0e,
    0x3, 0x36, 0x70, 0x0a,
    0x5, 0x36, 0x74, 0x82, 0x76, 0x78,
    0x4, 0x36, 0x7c, 0x48, 0x58,
    0x5, 0x36, 0x90, 0x34, 0x33, 0x44,
    0x4, 0x36, 0x9c, 0x40, 0x48,
    0x4, 0x36, 0xeb, 0x0c, 0x1c,
    0x3, 0x36, 0xfd, 0x14,
    0x3, 0x39, 0x01, 0x02,
    0x3, 0x39, 0x04, 0x04,
    0x3, 0x39, 0x08, 0x41,
    0x3, 0x39, 0x1f, 0x10,
    0x4, 0x3e, 0x01, 0x45, 0xc0,
    0x4, 0x3e, 0x16, 0x00, 0x80,
    0x3, 0x3f, 0x09, 0x48,
    0x3, 0x48, 0x19, 0x05,
    0x3, 0x48, 0x1b, 0x03,
    0x3, 0x48, 0x1d, 0x0a,
    0x3, 0x48, 0x1f, 0x02,
    0x3, 0x48, 0x21, 0x08,
    0x3, 0x48, 0x23, 0x03,
    0x3, 0x48, 0x25, 0x02,
    0x3, 0x48, 0x27, 0x03,
    0x3, 0x48, 0x29, 0x04,
    0x3, 0x50, 0x00, 0x46,
    0x4, 0x57, 0x87, 0x10, 0x06,
    0x4, 0x57, 0x8a, 0x10, 0x06,
    0x8, 0x57, 0x90, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00,
    0x3, 0x57, 0x99, 0x00,
    0x4, 0x57, 0xc7, 0x10, 0x06,
    0x4, 0x57, 0xca, 0x10, 0x06,
    0x3, 0x57, 0xd1, 0x10,
    0x3, 0x57, 0xd4, 0x10,
    0x3, 0x57, 0xd9, 0x00,
    0x4, 0x59, 0x00, 0xf1, 0x04,
    0x12, 0x59, 0xe0, 0x60, 0x08, 0x3f, 0x18, 0x18, 0x3f, 0x06, 0x02, 0x38, 0x10, 0x0c, 0x10, 0x04, 0x02, 0xa0, 0x08,
    0xe, 0x59, 0xf4, 0x18, 0x10, 0x0c, 0x10, 0x06, 0x02, 0x18, 0x10, 0x0c, 0x10, 0x04, 0x02,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x36, 0xf9, 0x24,
    0x46,
};

//27MHz
//371.25Mbps 2lane 1920x1080 30fps
static const uint8_t sc200ai_linear_1920x1080_30fps_regs[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x03,
    0x6, 0x32, 0x0c, 0x04, 0x4c, 0x04, 0x65,
    0x3, 0x32, 0x43, 0x01,
    0x4, 0x32, 0x48, 0x02, 0x09,
    0x3, 0x32, 0x53, 0x08,
    0x3, 0x32, 0x71, 0x0a,
    0x3, 0x33, 0x01, 0x20,
    0x3, 0x33, 0x04, 0x40,
    0x3, 0x33, 0x06, 0x32,
    0x3, 0x33, 0x0b, 0x88,
    0x3, 0x33, 0x0f, 0x02,
    0x3, 0x33, 0x1e, 0x39,
    0x3, 0x33, 0x33, 0x10,
    0x4, 0x36, 0x21, 0xe8, 0x16,
    0x3, 0x36, 0x37, 0x1b,
    0x5, 0x36, 0x3a, 0x1f, 0xc6, 0x0e,
    0x3, 0x36, 0x70, 0x0a,
    0x5, 0x36, 0x74, 0x82, 0x76, 0x78,
    0x4, 0x36, 0x7c, 0x48, 0x58,
    0x5, 0x36, 0x90, 0x34, 0x33, 0x44,
    0x4, 0x36, 0x9c, 0x40, 0x48,
    0x3, 0x39, 0x01, 0x02,
    0x3, 0x39, 0x04, 0x04,
    0x3, 0x39, 0x08, 0x41,
    0x3, 0x39, 0x1d, 0x14,
    0x3, 0x39, 0x1f, 0x18,
    0x4, 0x3e, 0x01, 0x8c, 0x20,
    0x4, 0x3e, 0x16, 0x00, 0x80,
    0x3, 0x3f, 0x09, 0x48,
    0x4, 0x57, 0x87, 0x10, 0x06,
    0x4, 0x57, 0x8a, 0x10, 0x06,
    0x8, 0x57, 0x90, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00,
    0x3, 0x57, 0x99, 0x00,
    0x4, 0x57, 0xc7, 0x10, 0x06,
    0x4, 0x57, 0xca, 0x10, 0x06,
    0x3, 0x57, 0xd1, 0x10,
    0x3, 0x57, 0xd4, 0x10,
    0x3, 0x57, 0xd9, 0x00,
    0x12, 0x59, 0xe0, 0x60, 0x08, 0x3f, 0x18, 0x18, 0x3f, 0x06, 0x02, 0x38, 0x10, 0x0c, 0x10, 0x04, 0x02, 0xa0, 0x08,
    0xe, 0x59, 0xf4, 0x18, 0x10, 0x0c, 0x10, 0x06, 0x02, 0x18, 0x10, 0x0c, 0x10, 0x04, 0x02,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x36, 0xf9, 0x27,
    0x2e,
};

//27MHz
//742.5Mbps 2lane 1920x1080 60fps
static const uint8_t sc200ai_linear_1920x1080_60fps_regs[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x01,
    0x3, 0x32, 0x43, 0x01,
    0x4, 0x32, 0x48, 0x02, 0x09,
    0x3, 0x32, 0x53, 0x08,
    0x3, 0x32, 0x71, 0x0a,
    0x6, 0x33, 0x01, 0x06, 0x0c, 0x08, 0x60,
    0x3, 0x33, 0x06, 0x30,
    0x4, 0x33, 0x08, 0x10, 0x70,
    0x3, 0x33, 0x0b, 0x80,
    0x6, 0x33, 0x0d, 0x16, 0x1c, 0x02, 0x02,
    0x3, 0x33, 0x1c, 0x04,
    0x5, 0x33, 0x1e, 0x51, 0x61, 0x07,
    0x3, 0x33, 0x33, 0x10,
    0x3, 0x33, 0x4c, 0x08,
    0x3, 0x33, 0x56, 0x09,
    0x3, 0x33, 0x64, 0x17,
    0xf, 0x33, 0x90, 0x08, 0x18, 0x38, 0x06, 0x06, 0x06, 0x08, 0x18, 0x38, 0x06, 0x0a, 0x10, 0x20,
    0x3, 0x33, 0xac, 0x08,
    0x4, 0x33, 0xae, 0x10, 0x19,
    0x4, 0x36, 0x21, 0xe8, 0x16,
    0x3, 0x36, 0x30, 0xa0,
    0x3, 0x36, 0x37, 0x36,
    0x5, 0x36, 0x3a, 0x1f, 0xc6, 0x0e,
    0x3, 0x36, 0x70, 0x0a,
    0x5, 0x36, 0x74, 0x82, 0x76, 0x78,
    0x4, 0x36, 0x7c, 0x48, 0x58,
    0x5, 0x36, 0x90, 0x34, 0x33, 0x44,
    0x4, 0x36, 0x9c, 0x40, 0x48,
    0x4, 0x36, 0xeb, 0x0c, 0x0c,
    0x3, 0x36, 0xfd, 0x14,
    0x3, 0x39, 0x01, 0x02,
    0x3, 0x39, 0x04, 0x04,
    0x3, 0x39, 0x08, 0x41,
    0x3, 0x39, 0x1f, 0x10,
    0x4, 0x3e, 0x01, 0x8c, 0x20,
    0x4, 0x3e, 0x16, 0x00, 0x80,
    0x3, 0x3f, 0x09, 0x48,
    0x3, 0x48, 0x19, 0x09,
    0x3, 0x48, 0x1b, 0x05,
    0x3, 0x48, 0x1d, 0x14,
    0x3, 0x48, 0x1f, 0x04,
    0x3, 0x48, 0x21, 0x0a,
    0x3, 0x48, 0x23, 0x05,
    0x3, 0x48, 0x25, 0x04,
    0x3, 0x48, 0x27, 0x05,
    0x3, 0x48, 0x29, 0x08,
    0x4, 0x57, 0x87, 0x10, 0x06,
    0x4, 0x57, 0x8a, 0x10, 0x06,
    0x8, 0x57, 0x90, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00,
    0x3, 0x57, 0x99, 0x00,
    0x4, 0x57, 0xc7, 0x10, 0x06,
    0x4, 0x57, 0xca, 0x10, 0x06,
    0x3, 0x57, 0xd1, 0x10,
    0x3, 0x57, 0xd4, 0x10,
    0x3, 0x57, 0xd9, 0x00,
    0x12, 0x59, 0xe0, 0x60, 0x08, 0x3f, 0x18, 0x18, 0x3f, 0x06, 0x02, 0x38, 0x10, 0x0c, 0x10, 0x04, 0x02, 0xa0, 0x08,
    0xe, 0x59, 0xf4, 0x18, 0x10, 0x0c, 0x10, 0x06, 0x02, 0x18, 0x10, 0x0c, 0x10, 0x04, 0x02,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x36, 0xf9, 0x24,
    0x3f,
};

//27MHz
//371.25Mbps 2lane 1920x1080 30fps
static const uint8_t sc200ai_hdr_1920x1080_30fps_regs[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x01, 0x00, 0x00,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x36, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x02,
    0x6, 0x32, 0x0c, 0x04, 0x4c, 0x08, 0xcc,
    0x3, 0x32, 0x20, 0x53,
    0x3, 0x32, 0x43, 0x01,
    0x4, 0x32, 0x48, 0x02, 0x09,
    0x3, 0x32, 0x50, 0x3f,
    0x3, 0x32, 0x53, 0x08,
    0x3, 0x32, 0x71, 0x0a,
    0x6, 0x33, 0x01, 0x06, 0x0c, 0x08, 0x60,
    0x3, 0x33, 0x06, 0x30,
    0x4, 0x33, 0x08, 0x10, 0x70,
    0x3, 0x33, 0x0b, 0x80,
    0x6, 0x33, 0x0d, 0x16, 0x1c, 0x02, 0x02,
    0x3, 0x33, 0x1c, 0x04,
    0x5, 0x33, 0x1e, 0x51, 0x61, 0x07,
    0x3, 0x33, 0x33, 0x10,
    0x3, 0x33, 0x47, 0x77,
    0x3, 0x33, 0x4c, 0x08,
    0x3, 0x33, 0x56, 0x09,
    0x3, 0x33, 0x64, 0x17,
    0x3, 0x33, 0x6c, 0xcc,
    0xf, 0x33, 0x90, 0x08, 0x18, 0x38, 0x06, 0x06, 0x06, 0x08, 0x18, 0x38, 0x06, 0x0a, 0x10, 0x20,
    0x3, 0x33, 0xac, 0x08,
    0x4, 0x33, 0xae, 0x10, 0x19,
    0x4, 0x36, 0x21, 0xe8, 0x16,
    0x3, 0x36, 0x30, 0xa0,
    0x3, 0x36, 0x37, 0x36,
    0x5, 0x36, 0x3a, 0x1f, 0xc6, 0x0e,
    0x3, 0x36, 0x70, 0x0a,
    0x5, 0x36, 0x74, 0x82, 0x76, 0x78,
    0x4, 0x36, 0x7c, 0x48, 0x58,
    0x5, 0x36, 0x90, 0x34, 0x33, 0x44,
    0x4, 0x36, 0x9c, 0x40, 0x48,
    0x4, 0x36, 0xeb, 0x0c, 0x0c,
    0x3, 0x36, 0xfd, 0x14,
    0x3, 0x39, 0x01, 0x02,
    0x3, 0x39, 0x04, 0x04,
    0x3, 0x39, 0x08, 0x41,
    0x3, 0x39, 0x1f, 0x10,
    0x5, 0x3e, 0x00, 0x01, 0x06, 0x00,
    0x8, 0x3e, 0x04, 0x10, 0x60, 0x00, 0x80, 0x03, 0x40,
    0x6, 0x3e, 0x10, 0x00, 0x80, 0x03, 0x40,
    0x4, 0x3e, 0x16, 0x00, 0x80,
    0x4, 0x3e, 0x23, 0x01, 0x9e,
    0x3, 0x3f, 0x09, 0x48,
    0x3, 0x48, 0x16, 0xb1,
    0x3, 0x48, 0x19, 0x09,
    0x3, 0x48, 0x1b, 0x05,
    0x3, 0x48, 0x1d, 0x14,
    0x3, 0x48, 0x1f, 0x04,
    0x3, 0x48, 0x21, 0x0a,
    0x3, 0x48, 0x23, 0x05,
    0x3, 0x48, 0x25, 0x04,
    0x3, 0x48, 0x27, 0x05,
    0x3, 0x48, 0x29, 0x08,
    0x4, 0x57, 0x87, 0x10, 0x06,
    0x4, 0x57, 0x8a, 0x10, 0x06,
    0x8, 0x57, 0x90, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00,
    0x3, 0x57, 0x99, 0x00,
    0x4, 0x57, 0xc7, 0x10, 0x06,
    0x4, 0x57, 0xca, 0x10, 0x06,
    0x3, 0x57, 0xd1, 0x10,
    0x3, 0x57, 0xd4, 0x10,
    0x3, 0x57, 0xd9, 0x00,
    0x12, 0x59, 0xe0, 0x60, 0x08, 0x3f, 0x18, 0x18, 0x3f, 0x06, 0x02, 0x38, 0x10, 0x0c, 0x10, 0x04, 0x02, 0xa0, 0x08,
    0xe, 0x59, 0xf4, 0x18, 0x10, 0x0c, 0x10, 0x06, 0x02, 0x18, 0x10, 0x0c, 0x10, 0x04, 0x02,
    0x3, 0x36, 0xe9, 0x20,
    0x3, 0x36, 0xf9, 0x24,
    0x48,
};

static struct sc200ai_mode supported_modes[] =
{
    [SC200AI_960X540] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 960,
        .height = 540,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0232,
        .xvclk = 27000000,
        .link_freq = 185625000,
        .reg_list = sc200ai_linear_960x540_120fps_regs,
        .reg_list_size = ARRAY_SIZE(sc200ai_linear_960x540_120fps_regs),
        .hdr_mode = NO_HDR,
    },
    [SC200AI_1920X1080_60FPS] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0465,
        .xvclk = 27000000,  // 27MHZ
        .link_freq = 371250000,  // 371.25MHZ
        .reg_list = sc200ai_linear_1920x1080_60fps_regs,
        .reg_list_size = ARRAY_SIZE(sc200ai_linear_1920x1080_60fps_regs),
        .hdr_mode = NO_HDR,
    },
    [SC200AI_1920X1080_30FPS] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x0465,
        .xvclk = 27000000,  // 27MHZ
        .link_freq = 185625000,  // 185.625MHZ
        .reg_list = sc200ai_linear_1920x1080_30fps_regs,
        .reg_list_size = ARRAY_SIZE(sc200ai_linear_1920x1080_30fps_regs),
        .hdr_mode = NO_HDR,
    },
    [SC200AI_1920X1080_HDR] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x46,
        .hts_def = 0x44C * 2,
        .vts_def = 0x8CC,
        .xvclk = 27000000,
        .link_freq = 371250000,
        .reg_list = sc200ai_hdr_1920x1080_30fps_regs,
        .reg_list_size = ARRAY_SIZE(sc200ai_hdr_1920x1080_30fps_regs),
        .hdr_mode = HDR_X2,
    }
};

static ret_err_t sc200ai_read_reg(struct sc200ai *dev, uint16_t reg, uint8_t *data)
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
            sc200ai_err(dev, "read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc200ai_write_reg(struct sc200ai *dev, uint16_t reg, uint8_t data)
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
            sc200ai_err(dev, "write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static int sc200ai_write_multiple_reg_continue(struct sc200ai *dev, const uint8_t *i2c_data, int len)
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
            sc200ai_err(dev, "write multi-regs, retry=%d, addr=0x%02x%02x\n",
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

/* mode: 0 = lgain  1 = sgain */
static int sc200ai_set_gain_reg(struct sc200ai *dev, uint32_t gain, int mode)
{
    uint8_t Coarse_gain = 1, DIG_gain = 1;
    uint32_t Dcg_gainx100 = 1, ANA_Fine_gainx64 = 1;
    uint8_t Coarse_gain_reg = 0, DIG_gain_reg = 0;
    uint8_t ANA_Fine_gain_reg = 0x20, DIG_Fine_gain_reg = 0x80;
    int ret = 0;

    gain = gain * 16;
    if (gain <= 1024)
        gain = 1024;
    else if (gain > SC200AI_GAIN_MAX * 16)
        gain = SC200AI_GAIN_MAX * 16;

    if (gain < 2 * 1024)                 // start again
    {
        Dcg_gainx100 = 100;
        Coarse_gain = 1;
        DIG_gain = 1;
        Coarse_gain_reg = 0x03;
        DIG_gain_reg = 0x0;
        DIG_Fine_gain_reg = 0x80;
    }
    else if (gain <= 3456)
    {
        Dcg_gainx100 = 100;
        Coarse_gain = 2;
        DIG_gain = 1;
        Coarse_gain_reg = 0x07;
        DIG_gain_reg = 0x0;
        DIG_Fine_gain_reg = 0x80;
    }
    else if (gain <= 6908)
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 1;
        DIG_gain = 1;
        Coarse_gain_reg = 0x23;
        DIG_gain_reg = 0x0;
        DIG_Fine_gain_reg = 0x80;
    }
    else if (gain <= 13817)
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 2;
        DIG_gain = 1;
        Coarse_gain_reg = 0x27;
        DIG_gain_reg = 0x0;
        DIG_Fine_gain_reg = 0x80;
    }
    else if (gain <= 27635)
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 4;
        DIG_gain = 1;
        Coarse_gain_reg = 0x2f;
        DIG_gain_reg = 0x0;
        DIG_Fine_gain_reg = 0x80;
    }
    else if (gain <= 55270)               // end again
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 8;
        DIG_gain = 1;
        Coarse_gain_reg = 0x3f;
        DIG_gain_reg = 0x0;
        DIG_Fine_gain_reg = 0x80;
    }
    else if (gain < 55270 * 2)             // start dgain
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 8;
        DIG_gain = 1;
        ANA_Fine_gainx64 = 127;
        Coarse_gain_reg = 0x3f;
        DIG_gain_reg = 0x0;
        ANA_Fine_gain_reg = 0x7f;
    }
    else if (gain < 55270 * 4)
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 8;
        DIG_gain = 2;
        ANA_Fine_gainx64 = 127;
        Coarse_gain_reg = 0x3f;
        DIG_gain_reg = 0x1;
        ANA_Fine_gain_reg = 0x7f;
    }
    else if (gain < 55270 * 8)
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 8;
        DIG_gain = 4;
        ANA_Fine_gainx64 = 127;
        Coarse_gain_reg = 0x3f;
        DIG_gain_reg = 0x3;
        ANA_Fine_gain_reg = 0x7f;
    }
    else if (gain < 55270 * 16)
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 8;
        DIG_gain = 8;
        ANA_Fine_gainx64 = 127;
        Coarse_gain_reg = 0x3f;
        DIG_gain_reg = 0x7;
        ANA_Fine_gain_reg = 0x7f;
    }
    else if (gain <= 1754822)
    {
        Dcg_gainx100 = 340;
        Coarse_gain = 8;
        DIG_gain = 16;
        ANA_Fine_gainx64 = 127;
        Coarse_gain_reg = 0x3f;
        DIG_gain_reg = 0xF;
        ANA_Fine_gain_reg = 0x7f;
    }

    if (gain < 3456)
        ANA_Fine_gain_reg = abs(100 * gain / (Dcg_gainx100 * Coarse_gain) / 16);
    else if (gain == 3456)
        ANA_Fine_gain_reg = 0x6C;
    else if (gain < 55270)
        ANA_Fine_gain_reg = abs(100 * gain / (Dcg_gainx100 * Coarse_gain) / 16);
    else
        DIG_Fine_gain_reg = abs(800 * gain / (Dcg_gainx100 * Coarse_gain *
                                              DIG_gain) / ANA_Fine_gainx64);

    if (mode == SC200AI_LGAIN)
    {
        ret = sc200ai_write_reg(dev, SC200AI_REG_DIG_GAIN,
                                DIG_gain_reg & 0xF);
        ret |= sc200ai_write_reg(dev, SC200AI_REG_DIG_FINE_GAIN,
                                 DIG_Fine_gain_reg);
        ret |= sc200ai_write_reg(dev, SC200AI_REG_ANA_GAIN,
                                 Coarse_gain_reg);
        ret |= sc200ai_write_reg(dev, SC200AI_REG_ANA_FINE_GAIN,
                                 ANA_Fine_gain_reg);
    }
    else
    {
        ret = sc200ai_write_reg(dev->i2c_bus, SC200AI_REG_SDIG_GAIN,
                                DIG_gain_reg & 0xF);
        ret |= sc200ai_write_reg(dev->i2c_bus, SC200AI_REG_SDIG_FINE_GAIN,
                                 DIG_Fine_gain_reg);
        ret |= sc200ai_write_reg(dev->i2c_bus, SC200AI_REG_SANA_GAIN,
                                 Coarse_gain_reg);
        ret |= sc200ai_write_reg(dev->i2c_bus, SC200AI_REG_SANA_FINE_GAIN,
                                 ANA_Fine_gain_reg);
    }

    if (gain <= 20 * 1024)
        ret |= sc200ai_write_reg(dev, 0x5799,
                                 0x0);
    else if (gain >= 30 * 1024)
        ret |= sc200ai_write_reg(dev, 0x5799,
                                 0x07);

    return ret;
}

static rt_err_t rk_sc200ai_set_expval(struct sc200ai *dev, struct rk_camera_exp_val *exp)
{
    rt_err_t ret = RET_SYS_EOK;
    uint8_t s_max_exp_time_l = 0, s_max_exp_time_h = 0;
    uint16_t s_max_exp_time;
    uint32_t l_exp_time = 0, s_exp_time = 0;
    uint32_t l_a_gain = 0, s_a_gain = 0;
    struct camera_board_desc *sc200ai = (struct camera_board_desc *)camera_sc200ai;

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming && strcmp(sc200ai->isp_subdev_name, RK_ISP_ASSIST_SUBDEV_NAME))
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc200ai_dbg(dev, "sc200ai don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    if (dev->cur_mode->hdr_mode == NO_HDR)
    {
        l_exp_time = exp->reg_time[0] * 2;
        l_a_gain = exp->reg_gain[0];
        s_exp_time = 0;
        s_a_gain = 0;
    }
    else if (dev->cur_mode->hdr_mode == HDR_X2)
    {
        l_exp_time = exp->reg_time[1] * 2;
        s_exp_time = exp->reg_time[0] * 2;
        l_a_gain = exp->reg_gain[1];
        s_a_gain = exp->reg_gain[0];
    }

    sc200ai_dbg(dev, "rev exp req: L_exp: 0x%x, 0x%x, S_exp: 0x%x, 0x%x\n",
                l_exp_time, l_a_gain, s_exp_time, s_a_gain);

    ret = sc200ai_read_reg(dev, SC200AI_REG_MAX_SEXPOSURE_H, &s_max_exp_time_h);
    ret = sc200ai_read_reg(dev, SC200AI_REG_MAX_SEXPOSURE_L, &s_max_exp_time_l);
    s_max_exp_time = (s_max_exp_time_h << 8) | s_max_exp_time_l;
    //set exposure
    if (dev->cur_mode->hdr_mode == NO_HDR)
    {
        if (l_exp_time > 2 * dev->cur_mode->vts_def - 8)
            l_exp_time = 2 * dev->cur_mode->vts_def - 8;
    }
    else if (dev->cur_mode->hdr_mode == HDR_X2)
    {
        if (l_exp_time > 2 * dev->cur_mode->vts_def - 2 * s_max_exp_time - 10)
            l_exp_time = 2 * dev->cur_mode->vts_def - 2 * s_max_exp_time - 10;
        if (s_exp_time > 2 * s_max_exp_time - 10)
            s_exp_time = 2 * s_max_exp_time - 10;
    }

    ret = sc200ai_write_reg(dev, SC200AI_REG_EXPOSURE_H,
                            SC200AI_FETCH_EXP_H(l_exp_time));
    ret = sc200ai_write_reg(dev, SC200AI_REG_EXPOSURE_M,
                            SC200AI_FETCH_EXP_M(l_exp_time));
    ret = sc200ai_write_reg(dev, SC200AI_REG_EXPOSURE_L,
                            SC200AI_FETCH_EXP_L(l_exp_time));
    if (dev->cur_mode->hdr_mode == HDR_X2)
    {
        ret = sc200ai_write_reg(dev, SC200AI_REG_SEXPOSURE_M,
                                SC200AI_FETCH_EXP_M(s_exp_time));
        ret = sc200ai_write_reg(dev, SC200AI_REG_SEXPOSURE_L,
                                SC200AI_FETCH_EXP_L(s_exp_time));
    }

    ret = sc200ai_set_gain_reg(dev, l_a_gain, SC200AI_LGAIN);
    if (dev->cur_mode->hdr_mode == HDR_X2)
        ret |= sc200ai_set_gain_reg(dev, s_a_gain, SC200AI_SGAIN);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static ret_err_t rk_sc200ai_set_vts(struct sc200ai *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    sc200ai_dbg(dev, "set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc200ai_write_reg(dev, SC200AI_REG_VTS_L,
                            (uint8_t)(dst_vts & 0xff));
    ret = sc200ai_write_reg(dev, SC200AI_REG_VTS_H,
                            (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_sc200ai_set_flip_mirror(struct sc200ai *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    ret = sc200ai_read_reg(dev, SC200AI_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC200AI_FETCH_MIRROR(val, flip & 0x01);
        val = SC200AI_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC200AI_FETCH_MIRROR(val, false);
        val = SC200AI_FETCH_FLIP(val, false);
        break;
    };
    sc200ai_dbg(dev, "flip 0x%x, reg val 0x%x\n", flip, val);
    ret = sc200ai_write_reg(dev, SC200AI_FLIP_MIRROR_REG, val);

    return ret;
}

static void sc200ai_stream_on(struct sc200ai *dev)
{
    sc200ai_dbg(dev, "stream_on enter tick:%u\n", rt_tick_get());

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    sc200ai_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc200ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc200ai_set_flip_mirror(dev, dev->flip);

    sc200ai_write_reg(dev, SC200AI_REG_STREAM_CTRL, SC200AI_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc200ai_dbg(dev, "stream_on exit tick:%u\n", rt_tick_get());
}

static void sc200ai_stream_on_late(struct sc200ai *dev)
{
    sc200ai_dbg(dev, "stream_on_late enter tick:%u\n", rt_tick_get());

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc200ai_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc200ai_set_flip_mirror(dev, dev->flip);

    sc200ai_write_reg(dev, SC200AI_REG_STREAM_CTRL, SC200AI_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc200ai_dbg(dev, "stream_on_late exit tick:%u\n", rt_tick_get());
}

static void sc200ai_stream_off(struct sc200ai *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc200ai_write_reg(dev, SC200AI_REG_STREAM_CTRL, SC200AI_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    dev->streaming = false;
}
ret_err_t rk_sc200ai_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RT_EOK;
    struct sc200ai *sc200ai;

    RT_ASSERT(dev != RT_NULL);
    sc200ai = (struct sc200ai *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc200ai->parent;

    if (sc200ai)
    {
        camera->info.mbus_fmt.width = sc200ai->cur_mode->width;
        camera->info.mbus_fmt.height = sc200ai->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc200ai->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  sc200ai->cur_mode->hdr_mode;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_sc200ai_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc200ai_close(struct rk_camera_device *dev)
{
    uint8_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_sc200ai_get_expinf(struct sc200ai *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RT_EOK;
    const struct sc200ai_mode *mode;

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

static ret_err_t rk_sc200ai_get_intput_fmt(struct sc200ai *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc200ai_mode *mode;

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_sc200ai_match_dst_config(struct sc200ai *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct sc200ai_mode *mode;
    int i, cur_fps, dst_fps, cur_vts, dst_vts;

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    sc200ai_info(dev, "cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height,
                 dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];
        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc200ai_mode *)mode;
            ret = RT_EOK;
            sc200ai_info(dev, "find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
    }
    if (i == ARRAY_SIZE(supported_modes))
    {
        sc200ai_err(dev, "not find match resulotion\n");
        return -RT_EINVAL;
    }

    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return RT_EOK;

    if (dst_fps > cur_fps)
    {
        sc200ai_err(dev, "dst fps is larger than cur fps\n");
        return -RT_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

static ret_err_t rk_sc200ai_set_intput_fmt(struct sc200ai *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RT_EOK;
    const struct sc200ai_mode *mode;
    int i;
    bool is_find_fmt = false;

    sc200ai_info(dev, "dst resulotion, width %d, height %d\n",
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
            sc200ai_write_multiple_reg_continue(dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
            ret = rk_sc200ai_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

ret_err_t rk_sc200ai_control(struct rk_camera_device *dev,
                             dt_cmd_t cmd,
                             void *args)
{
    ret_err_t ret = RT_EOK;
    struct sc200ai *sc200ai;

    RT_ASSERT(dev != RT_NULL);
    sc200ai = (struct sc200ai *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc200ai_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc200ai_stream_on(sc200ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc200ai_stream_off(sc200ai);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc200ai_get_expinf(sc200ai, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc200ai_set_expval(sc200ai, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc200ai_set_vts(sc200ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc200ai_get_intput_fmt(sc200ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc200ai_set_intput_fmt(sc200ai, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc200ai_match_dst_config(sc200ai, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc200ai_set_flip_mirror(sc200ai, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc200ai_stream_on_late(sc200ai);
    }
    break;
    default:
        sc200ai_dbg(sc200ai, "exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc200ai_ops =
{
    .init       = rk_sc200ai_init,
    .open       = rk_sc200ai_open,
    .control    = rk_sc200ai_control,
};



int rk_camera_sc200ai_init(void)
{
    ret_err_t ret = RT_EOK;
    const struct camera_board_desc *sc200ai_desc;

    for (sc200ai_desc = camera_sc200ai; sc200ai_desc->i2c_addr != 0; sc200ai_desc++)
    {
        struct sc200ai *sc200ai;
        struct clk_gate *clkgate;
        struct rk_camera_device *camera;

        sc200ai = rt_malloc(sizeof(struct sc200ai));
        if (!sc200ai)
        {
            rt_kprintf("malloc sc200ai failed\n");
            return -RT_ENOMEM;
        }

        rt_memset(sc200ai, 0x0, sizeof(struct sc200ai));

        camera = &sc200ai->parent;
        camera->ops = &rk_sc200ai_ops;

        rt_memcpy(sc200ai->name, sc200ai_desc->isp_subdev_name, RK_CAMERA_DEVICE_NAME_SIZE);
        rt_memcpy(sc200ai->i2c_name, sc200ai_desc->i2c_bus, RK_CAMERA_I2C_NAME_SIZE);

        sc200ai->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(sc200ai->i2c_name);
        if (sc200ai->i2c_bus == RT_NULL)
        {
            sc200ai_err(sc200ai, "can't find i2c bus:%s\n", sc200ai->i2c_name);
            rt_free(sc200ai);
            continue;
        }

        if (sc200ai_desc->mode_id <= SC200AI_MODE_ID_MAX)
        {
            sc200ai_dbg(sc200ai, "mode_id: %d\n", sc200ai_desc->mode_id);
            sc200ai->cur_mode = &supported_modes[sc200ai_desc->mode_id];
        }
        else
        {
            sc200ai_info(sc200ai, "mode id is over range, default use mode_id: 0\n");
            sc200ai->cur_mode = &supported_modes[0];
        }

        sc200ai->time_valid_delay = sc200ai_desc->time_valid_delay;
        sc200ai->gain_valid_delay = sc200ai_desc->gain_valid_delay;
        sc200ai->i2c_addr = sc200ai_desc->i2c_addr;

        if (sc200ai_desc->rst_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc200ai_desc->rst_gpio.gpio_group,
                                     sc200ai_desc->rst_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc200ai_desc->rst_gpio.gpio_group,
                                 sc200ai_desc->rst_gpio.gpio_pin,
                                 !sc200ai_desc->rst_active);
        }
        if (sc200ai_desc->pwdn_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc200ai_desc->pwdn_gpio.gpio_group,
                                     sc200ai_desc->pwdn_gpio.gpio_pin,
                                     GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc200ai_desc->pwdn_gpio.gpio_group,
                                 sc200ai_desc->pwdn_gpio.gpio_pin,
                                 !sc200ai_desc->pwdn_active);
        }

        HAL_DelayMs(4);

        if (sc200ai_desc->mclk_out_gate_id && sc200ai_desc->mclk_id)
        {
            clk_set_rate(sc200ai_desc->mclk_id, sc200ai->cur_mode->xvclk);
            HAL_CRU_ClkEnable(sc200ai_desc->mclk_out_gate_id);
        }

        rt_mutex_init(&sc200ai->mutex_lock, "sc200ai_mutex", RT_IPC_FLAG_FIFO);
        RT_ASSERT(rt_object_get_type(&sc200ai->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
        camera->i2c_bus = sc200ai->i2c_bus;
        rt_memcpy(camera->name, sc200ai->name, RK_CAMERA_DEVICE_NAME_SIZE);
        ret = rk_camera_register(camera, camera->name, sc200ai);
        if (ret)
        {
            sc200ai_err(sc200ai, "regster failed\n");
            rt_free(sc200ai);
        }
    }

    return ret;
}

#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_sc200ai_init);
#endif
#endif

