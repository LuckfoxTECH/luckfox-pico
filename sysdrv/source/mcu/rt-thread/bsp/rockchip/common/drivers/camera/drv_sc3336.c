/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc3336.c
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
#include "hal_base.h"
#include <rtconfig.h>
#include "board.h"
#include "drv_clock.h"
#include "board.h"

#ifdef RT_USING_SC3336
#define SC3336_DEBUG_INFO      0

#if SC3336_DEBUG_INFO
#include <stdio.h>
#define SC3336_DEBUG(...)               rk_kprintf("[SC3336]:");rk_kprintf(__VA_ARGS__)
#else
#define SC3336_DEBUG(...)
#endif

#define SC3336_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define RT_USING_SC3336_OPS 0

#define SC3336_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME                "i2c4"
#define SC3336_7BIT_ADDRESS         (0x30)
#define SC3336_REG_ID_H_ADDRESS     (0x3107)
#define SC3336_REG_ID_L_ADDRESS     (0x3108)
#define SC3336_SENSOR_ID            (0xcb34)
#define SC3336_REG_EXP_LONG_H           0x3e00
#define SC3336_REG_EXP_LONG_M           0x3e01
#define SC3336_REG_EXP_LONG_L           0x3e02
#define SC3336_REG_EXP_SHORT_H          0x3e22
#define SC3336_REG_EXP_SHORT_M          0x3e04
#define SC3336_REG_EXP_SHORT_L          0x3e05
#define SC3336_REG_DIG_GAIN            0x3e06
#define SC3336_REG_DIG_FINE_GAIN       0x3e07
#define SC3336_REG_ANA_GAIN            0x3e09
#define SC3336_REG_SDIG_GAIN           0x3e10
#define SC3336_REG_SDIG_FINE_GAIN      0x3e11
#define SC3336_REG_SANA_GAIN           0x3e12
#define SC3336_REG_SANA_FINE_GAIN      0x3e13
#define SC3336_GAIN_MIN                0x0080
#define SC3336_GAIN_MAX                (99614)  //48.64*16*128
#define SC3336_GAIN_STEP               1
#define SC3336_GAIN_DEFAULT            0x0080
#define SC3336_LGAIN                   0
#define SC3336_SGAIN                   1
#define SC3336_FLIP_MIRROR_REG         0x3221
#define SC3336_REG_VTS_H               0x320e
#define SC3336_REG_VTS_L               0x320f
#define SC3336_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC3336_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define SC3336_PIN_CTRL_ENABLE          1
#define SC3336_I2C_DEBUG_ENABLE         0
#define SC3336_TESTPATTERN_ENABLE       0

/* Compiler Related Definitions */
#define rk_inline                               rt_inline

/* redefine macro function */
#define RK_ALIGN(size, align)                   RT_ALIGN(size, align)
#define rk_container_of(ptr, type, member)      rt_container_of(ptr, type, member)

/* redefine system variables */
typedef rt_mutex_t                              rk_mutex_t;
typedef rt_sem_t                                rk_semaphore_t;
typedef struct rt_device                        rk_device;
typedef struct rt_i2c_bus_device                rk_i2c_bus_device;
typedef struct clk_gate                         rk_clk_gate;

/* define private system variables */
typedef rt_list_t                               rk_list_node;
typedef rt_list_t                               rk_queue_list;

#define rk_list_first_entry(ptr, type, member)              rt_list_first_entry(ptr, type, member)
#define rk_list_for_each_entry(pos, head, member)           rt_list_for_each_entry(pos, head, member)
#define rk_kprintf                                          rt_kprintf

/* redefine the basic data type */
typedef rt_err_t                        ret_err_t;
typedef rt_size_t                       ret_size_t;
typedef rt_base_t                       dt_base_t;
typedef rt_ubase_t                      dt_ubase_t;
typedef rt_tick_t                       rk_tick_t;
typedef int                             dt_cmd_t;

#define RK_NULL                          RT_NULL
#define MACRO_ASSERT(EX)                 RT_ASSERT(EX)

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

#if 0
struct mclk
{
    rk_clk_gate *gate;
    eCLOCK_Name clk_name;
};
#endif

#define I2C_WRITE_CONTINUE

struct sc3336_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
};

struct sc3336_mode
{
    uint32_t bus_fmt;
    uint32_t width;
    uint32_t height;
    struct v4l2_fract max_fps;
    uint32_t hts_def;
    uint32_t vts_def;
    uint32_t exp_def;
    /* const struct sensor_reg *reg_list; */
#ifdef I2C_WRITE_CONTINUE
    const uint8_t *reg_list;
#else
    const struct sc3336_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint32_t hdr_mode;
};

#ifdef I2C_WRITE_CONTINUE

static const uint8_t g_sc3336_320x240_120fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
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
    0x48,
};
static const uint8_t g_sc3336_640x480_75fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
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
    0x48,
};

static const uint8_t g_sc3336_2304x1296_25fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
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
    0x38
};

#else
static const struct sc3336_sensor_reg g_sc3336_320x240_120fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x6a},
    {0x30b8, 0x44},
    {0x3200, 0x00},
    {0x3201, 0x00},
    {0x3202, 0x01},
    {0x3203, 0x98},
    {0x3204, 0x09},
    {0x3205, 0x07},
    {0x3206, 0x03},
    {0x3207, 0x7f},
    {0x3208, 0x01},
    {0x3209, 0x40},
    {0x320a, 0x00},
    {0x320b, 0xf0},
    {0x320e, 0x01},
    {0x320f, 0x2c},
    {0x3210, 0x01},
    {0x3211, 0xa2},
    {0x3212, 0x00},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3253, 0x10},
    {0x325f, 0x20},
    {0x3301, 0x04},
    {0x3306, 0x50},
    {0x3309, 0xa8},
    {0x330a, 0x00},
    {0x330b, 0xd8},
    {0x3314, 0x13},
    {0x331f, 0x99},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335e, 0x06},
    {0x335f, 0x0a},
    {0x3364, 0x5e},
    {0x337c, 0x02},
    {0x337d, 0x0e},
    {0x3390, 0x01},
    {0x3391, 0x03},
    {0x3392, 0x07},
    {0x3393, 0x04},
    {0x3394, 0x04},
    {0x3395, 0x04},
    {0x3396, 0x08},
    {0x3397, 0x0b},
    {0x3398, 0x1f},
    {0x3399, 0x04},
    {0x339a, 0x0a},
    {0x339b, 0x3a},
    {0x339c, 0xb4},
    {0x33a2, 0x04},
    {0x33ac, 0x08},
    {0x33ad, 0x1c},
    {0x33ae, 0x10},
    {0x33af, 0x30},
    {0x33b1, 0x80},
    {0x33b3, 0x48},
    {0x33f9, 0x60},
    {0x33fb, 0x74},
    {0x33fc, 0x4b},
    {0x33fd, 0x5f},
    {0x349f, 0x03},
    {0x34a6, 0x4b},
    {0x34a7, 0x5f},
    {0x34a8, 0x20},
    {0x34a9, 0x18},
    {0x34ab, 0xe8},
    {0x34ac, 0x01},
    {0x34ad, 0x00},
    {0x34f8, 0x5f},
    {0x34f9, 0x18},
    {0x3630, 0xc0},
    {0x3631, 0x84},
    {0x3632, 0x64},
    {0x3633, 0x32},
    {0x363b, 0x03},
    {0x363c, 0x08},
    {0x3641, 0x38},
    {0x3670, 0x4e},
    {0x3674, 0xc0},
    {0x3675, 0xc0},
    {0x3676, 0xc0},
    {0x3677, 0x84},
    {0x3678, 0x84},
    {0x3679, 0x84},
    {0x367c, 0x48},
    {0x367d, 0x49},
    {0x367e, 0x4b},
    {0x367f, 0x5f},
    {0x3690, 0x32},
    {0x3691, 0x32},
    {0x3692, 0x42},
    {0x369c, 0x4b},
    {0x369d, 0x5f},
    {0x36b0, 0x87},
    {0x36b1, 0x90},
    {0x36b2, 0xa1},
    {0x36b3, 0xd8},
    {0x36b4, 0x49},
    {0x36b5, 0x4b},
    {0x36b6, 0x4f},
    {0x36ea, 0x0a},
    {0x36eb, 0x0d},
    {0x36ec, 0x2c},
    {0x36ed, 0x26},
    {0x370f, 0x01},
    {0x3722, 0x09},
    {0x3724, 0x41},
    {0x3725, 0xc1},
    {0x3771, 0x09},
    {0x3772, 0x09},
    {0x3773, 0x05},
    {0x377a, 0x48},
    {0x377b, 0x5f},
    {0x37fa, 0x0a},
    {0x37fb, 0x33},
    {0x37fc, 0x11},
    {0x37fd, 0x18},
    {0x3904, 0x04},
    {0x3905, 0x8c},
    {0x391d, 0x04},
    {0x3921, 0x20},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x0a},
    {0x3935, 0x00},
    {0x3936, 0x2a},
    {0x3937, 0x6a},
    {0x3938, 0x6a},
    {0x39dc, 0x02},
    {0x3e01, 0x12},
    {0x3e02, 0x40},
    {0x3e09, 0x00},
    {0x440e, 0x02},
    {0x4509, 0x20},
    {0x4819, 0x04},
    {0x481b, 0x02},
    {0x481d, 0x06},
    {0x481f, 0x02},
    {0x4821, 0x07},
    {0x4823, 0x02},
    {0x4825, 0x02},
    {0x4827, 0x02},
    {0x4829, 0x03},
    {0x5000, 0x46},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x5988, 0x70},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x38},
    {0x5ae3, 0x30},
    {0x5ae4, 0x28},
    {0x5ae5, 0x38},
    {0x5ae6, 0x30},
    {0x5ae7, 0x28},
    {0x5ae8, 0x3f},
    {0x5ae9, 0x34},
    {0x5aea, 0x2c},
    {0x5aeb, 0x3f},
    {0x5aec, 0x34},
    {0x5aed, 0x2c},
    {0x36e9, 0x44},
    {0x37f9, 0x44},
    {REG_END, 0x00},
};

static const struct sc3336_sensor_reg g_sc3336_640x480_75fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x67},
    {0x30b8, 0x44},
    {0x3200, 0x00},
    {0x3201, 0x00},
    {0x3202, 0x00},
    {0x3203, 0xa8},
    {0x3204, 0x09},
    {0x3205, 0x07},
    {0x3206, 0x04},
    {0x3207, 0x6f},
    {0x3208, 0x02},
    {0x3209, 0x80},
    {0x320a, 0x01},
    {0x320b, 0xe0},
    {0x320e, 0x02},
    {0x320f, 0x20},
    {0x3210, 0x01},
    {0x3211, 0x02},
    {0x3212, 0x00},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3253, 0x10},
    {0x325f, 0x20},
    {0x3301, 0x04},
    {0x3306, 0x50},
    {0x3309, 0xa8},
    {0x330a, 0x00},
    {0x330b, 0xd8},
    {0x3314, 0x13},
    {0x331f, 0x99},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335e, 0x06},
    {0x335f, 0x0a},
    {0x3364, 0x5e},
    {0x337c, 0x02},
    {0x337d, 0x0e},
    {0x3390, 0x01},
    {0x3391, 0x03},
    {0x3392, 0x07},
    {0x3393, 0x04},
    {0x3394, 0x04},
    {0x3395, 0x04},
    {0x3396, 0x08},
    {0x3397, 0x0b},
    {0x3398, 0x1f},
    {0x3399, 0x04},
    {0x339a, 0x0a},
    {0x339b, 0x3a},
    {0x339c, 0xb4},
    {0x33a2, 0x04},
    {0x33ac, 0x08},
    {0x33ad, 0x1c},
    {0x33ae, 0x10},
    {0x33af, 0x30},
    {0x33b1, 0x80},
    {0x33b3, 0x48},
    {0x33f9, 0x60},
    {0x33fb, 0x74},
    {0x33fc, 0x4b},
    {0x33fd, 0x5f},
    {0x349f, 0x03},
    {0x34a6, 0x4b},
    {0x34a7, 0x5f},
    {0x34a8, 0x20},
    {0x34a9, 0x18},
    {0x34ab, 0xe8},
    {0x34ac, 0x01},
    {0x34ad, 0x00},
    {0x34f8, 0x5f},
    {0x34f9, 0x18},
    {0x3630, 0xc0},
    {0x3631, 0x84},
    {0x3632, 0x64},
    {0x3633, 0x32},
    {0x363b, 0x03},
    {0x363c, 0x08},
    {0x3641, 0x38},
    {0x3670, 0x4e},
    {0x3674, 0xc0},
    {0x3675, 0xc0},
    {0x3676, 0xc0},
    {0x3677, 0x84},
    {0x3678, 0x84},
    {0x3679, 0x84},
    {0x367c, 0x48},
    {0x367d, 0x49},
    {0x367e, 0x4b},
    {0x367f, 0x5f},
    {0x3690, 0x32},
    {0x3691, 0x32},
    {0x3692, 0x42},
    {0x369c, 0x4b},
    {0x369d, 0x5f},
    {0x36b0, 0x87},
    {0x36b1, 0x90},
    {0x36b2, 0xa1},
    {0x36b3, 0xd8},
    {0x36b4, 0x49},
    {0x36b5, 0x4b},
    {0x36b6, 0x4f},
    {0x36ea, 0x11},
    {0x36eb, 0x0d},
    {0x36ec, 0x2c},
    {0x36ed, 0x26},
    {0x370f, 0x01},
    {0x3722, 0x09},
    {0x3724, 0x41},
    {0x3725, 0xc1},
    {0x3771, 0x09},
    {0x3772, 0x09},
    {0x3773, 0x05},
    {0x377a, 0x48},
    {0x377b, 0x5f},
    {0x37fa, 0x11},
    {0x37fb, 0x33},
    {0x37fc, 0x11},
    {0x37fd, 0x08},
    {0x3904, 0x04},
    {0x3905, 0x8c},
    {0x391d, 0x04},
    {0x3921, 0x20},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x0a},
    {0x3935, 0x00},
    {0x3936, 0x2a},
    {0x3937, 0x6a},
    {0x3938, 0x6a},
    {0x39dc, 0x02},
    {0x3e01, 0x21},
    {0x3e02, 0x80},
    {0x3e09, 0x00},
    {0x440e, 0x02},
    {0x4509, 0x20},
    {0x4819, 0x04},
    {0x481b, 0x02},
    {0x481d, 0x07},
    {0x481f, 0x02},
    {0x4821, 0x07},
    {0x4823, 0x02},
    {0x4825, 0x02},
    {0x4827, 0x02},
    {0x4829, 0x03},
    {0x5000, 0x46},
    {0x5900, 0x01},
    {0x5901, 0x04},
    {0x5988, 0x70},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x38},
    {0x5ae3, 0x30},
    {0x5ae4, 0x28},
    {0x5ae5, 0x38},
    {0x5ae6, 0x30},
    {0x5ae7, 0x28},
    {0x5ae8, 0x3f},
    {0x5ae9, 0x34},
    {0x5aea, 0x2c},
    {0x5aeb, 0x3f},
    {0x5aec, 0x34},
    {0x5aed, 0x2c},
    {0x36e9, 0x54},
    {0x37f9, 0x47},
    {REG_END, 0x00},
};

//25fps
static const struct sc3336_sensor_reg g_sc3336_2304x1296_25fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x01},
    {0x30b8, 0x33},
    {0x320e, 0x06},
    {0x320f, 0x54},
    {0x3253, 0x10},
    {0x325f, 0x20},
    {0x3301, 0x04},
    {0x3306, 0x50},
    {0x3309, 0xa8},
    {0x330a, 0x00},
    {0x330b, 0xd8},
    {0x3314, 0x13},
    {0x331f, 0x99},
    {0x3333, 0x10},
    {0x3334, 0x40},
    {0x335e, 0x06},
    {0x335f, 0x0a},
    {0x3364, 0x5e},
    {0x337c, 0x02},
    {0x337d, 0x0e},
    {0x3390, 0x01},
    {0x3391, 0x03},
    {0x3392, 0x07},
    {0x3393, 0x04},
    {0x3394, 0x04},
    {0x3395, 0x04},
    {0x3396, 0x08},
    {0x3397, 0x0b},
    {0x3398, 0x1f},
    {0x3399, 0x04},
    {0x339a, 0x0a},
    {0x339b, 0x3a},
    {0x339c, 0xb4},
    {0x33a2, 0x04},
    {0x33ac, 0x08},
    {0x33ad, 0x1c},
    {0x33ae, 0x10},
    {0x33af, 0x30},
    {0x33b1, 0x80},
    {0x33b3, 0x48},
    {0x33f9, 0x60},
    {0x33fb, 0x74},
    {0x33fc, 0x4b},
    {0x33fd, 0x5f},
    {0x349f, 0x03},
    {0x34a6, 0x4b},
    {0x34a7, 0x5f},
    {0x34a8, 0x20},
    {0x34a9, 0x18},
    {0x34ab, 0xe8},
    {0x34ac, 0x01},
    {0x34ad, 0x00},
    {0x34f8, 0x5f},
    {0x34f9, 0x18},
    {0x3630, 0xc0},
    {0x3631, 0x84},
    {0x3632, 0x64},
    {0x3633, 0x32},
    {0x363b, 0x03},
    {0x363c, 0x08},
    {0x3641, 0x38},
    {0x3670, 0x4e},
    {0x3674, 0xc0},
    {0x3675, 0xc0},
    {0x3676, 0xc0},
    {0x3677, 0x84},
    {0x3678, 0x84},
    {0x3679, 0x84},
    {0x367c, 0x48},
    {0x367d, 0x49},
    {0x367e, 0x4b},
    {0x367f, 0x5f},
    {0x3690, 0x32},
    {0x3691, 0x32},
    {0x3692, 0x42},
    {0x369c, 0x4b},
    {0x369d, 0x5f},
    {0x36b0, 0x87},
    {0x36b1, 0x90},
    {0x36b2, 0xa1},
    {0x36b3, 0xd8},
    {0x36b4, 0x49},
    {0x36b5, 0x4b},
    {0x36b6, 0x4f},
    {0x370f, 0x01},
    {0x3722, 0x09},
    {0x3724, 0x41},
    {0x3725, 0xc1},
    {0x3771, 0x09},
    {0x3772, 0x09},
    {0x3773, 0x05},
    {0x377a, 0x48},
    {0x377b, 0x5f},
    {0x3904, 0x04},
    {0x3905, 0x8c},
    {0x391d, 0x04},
    {0x3921, 0x20},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x0a},
    {0x3935, 0x00},
    {0x3936, 0x2a},
    {0x3937, 0x6a},
    {0x3938, 0x6a},
    {0x39dc, 0x02},
    {0x3e01, 0x53},
    {0x3e02, 0xe0},
    {0x3e09, 0x00},
    {0x440e, 0x02},
    {0x4509, 0x20},
    {0x5ae0, 0xfe},
    {0x5ae1, 0x40},
    {0x5ae2, 0x38},
    {0x5ae3, 0x30},
    {0x5ae4, 0x28},
    {0x5ae5, 0x38},
    {0x5ae6, 0x30},
    {0x5ae7, 0x28},
    {0x5ae8, 0x3f},
    {0x5ae9, 0x34},
    {0x5aea, 0x2c},
    {0x5aeb, 0x3f},
    {0x5aec, 0x34},
    {0x5aed, 0x2c},
    {0x36e9, 0x54},
    {0x37f9, 0x27},
    {REG_END, 0x00},
};
#endif

static const struct sc3336_mode supported_modes[] =
{
    {
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
        .reg_list = g_sc3336_320x240_120fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc3336_320x240_120fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    {
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
        .reg_list = g_sc3336_640x480_75fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc3336_640x480_75fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    {
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
        .reg_list = g_sc3336_2304x1296_25fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc3336_2304x1296_25fps_reg_table),
        .hdr_mode = NO_HDR,
    }
};

struct sc3336_dev
{
    struct rk_camera_device parent;
    rk_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];

#if RT_USING_SC3336_OPS
    struct sc3336_ops *ops;
#endif

    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
    struct rt_mutex mutex_lock;
    struct rk_camera_exp_val init_exp;
    const struct sc3336_mode *cur_mode;
    struct sc3336_mode *dst_mode;
    int flip;
    bool has_init_exp;
    bool streaming;
};
typedef struct sc3336_dev *rt_sc3336_dev_t;

struct sc3336_ops
{
    ret_err_t (*init)(struct sc3336_dev *dev);
    ret_err_t (*open)(struct sc3336_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc3336_dev *dev);
    ret_err_t (*control)(struct sc3336_dev *dev, int cmd, void *arg);
};

static struct sc3336_dev g_sc3336;

static ret_err_t SC3336_read_reg(rk_i2c_bus_device *bus,
                                 uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    msg[0].addr = SC3336_7BIT_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = SC3336_7BIT_ADDRESS;
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

static ret_err_t SC3336_write_reg(rk_i2c_bus_device *bus,
                                  uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msgs;
    int ret = 0;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    send_buf[2] = data;

    msgs.addr = SC3336_7BIT_ADDRESS;
    msgs.flags = RT_I2C_WR;
    msgs.buf = send_buf;
    msgs.len = 3;
    ret = rt_i2c_transfer(bus, &msgs, 1);

    if (ret == 1)
    {
        SC3336_DEBUG("(%s):s0.0 i2c_bus ok\n");
        return RET_SYS_EOK;
    }
    else
    {
        SC3336_DEBUG("(%s):s0.0 i2c_bus error\n");
        return RET_SYS_ERROR;
    }
}

#ifdef I2C_WRITE_CONTINUE

#define MAX_I2C_MSG 80
static int SC3336_write_multiple_reg_continue(struct sc3336_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    rk_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msgs[MAX_I2C_MSG];
    int ret = 0;
    int offset = 0;

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(i2c_data != RK_NULL);

    i2c_bus = dev->i2c_bus;
    MACRO_ASSERT(i2c_bus != RK_NULL);

    for (i = 0; i < i2c_data[len - 1]; i++)
    {
        msgs[i].addr = SC3336_7BIT_ADDRESS;
        msgs[i].flags = RT_I2C_WR;
        msgs[i].buf = (uint8_t *)&i2c_data[offset + 1];
        msgs[i].len = i2c_data[offset];
        offset += (i2c_data[offset] + 1);
    }
#if 0
    ret = rt_i2c_transfer(i2c_bus, msgs, i2c_data[len - 1]);
#else
    for (i = 0; i < i2c_data[len - 1]; i++)
        ret |= rt_i2c_transfer(i2c_bus, &msgs[i], 1);
#endif
    if (ret == 1)
    {
        SC3336_DEBUG("(%s):s0.0 i2c_bus ok\n");
        return RET_SYS_EOK;
    }
    else
    {
        SC3336_DEBUG("(%s):s0.0 i2c_bus error\n");
        return RET_SYS_ERROR;
    }

}

#else
static ret_err_t SC3336_write_reg_continue(rk_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;

    MACRO_ASSERT(bus != RK_NULL);

    msgs.addr = SC3336_7BIT_ADDRESS;
    msgs.flags = RT_I2C_WR;
    msgs.buf = data;
    msgs.len = len;
    ret = rt_i2c_transfer(bus, &msgs, 1);

    if (ret == 1)
    {
        SC3336_DEBUG("(%s):s0.0 i2c_bus ok\n");
        return RET_SYS_EOK;
    }
    else
    {
        SC3336_DEBUG("(%s):s0.0 i2c_bus error\n");
        return RET_SYS_ERROR;
    }
}

#define I2C_WRITE_DEBUG
static void SC3336_write_multiple_reg(struct sc3336_dev *dev,
                                      const struct sc3336_sensor_reg *reg, int len)
{
    uint16_t i;
    rk_i2c_bus_device *i2c_bus;
    int k = 0;
    char *data = rt_malloc(len);
    uint16_t reg_addr;
#ifdef I2C_WRITE_DEBUG
    int j = 0;
    int cnt = 0;
#endif

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(reg != RK_NULL);

    i2c_bus = dev->i2c_bus;
    MACRO_ASSERT(i2c_bus != RK_NULL);

    for (i = 0;; i++)
    {
        if (reg[i].reg_addr == REG_END)
        {
            if (k > 0)
            {
#ifdef I2C_WRITE_DEBUG
                cnt++;
                rt_kprintf("0x%x, ", k + 2);
                for (j = 0; j < k + 2; j++)
                    rt_kprintf("0x%02x, ", data[j]);
                rt_kprintf("\n");
#endif
                SC3336_write_reg_continue(i2c_bus, data, k + 2);
                k = 0;
            }
            break;
        }

        if (reg[i].reg_addr == REG_DELAY)
        {
            if (k > 0)
            {
#ifdef I2C_WRITE_DEBUG
                cnt++;
                rt_kprintf("0x%x, ", k + 2);
                for (j = 0; j < k + 2; j++)
                    rt_kprintf("0x%02x, ", data[j]);
                rt_kprintf("\n");
#endif
                SC3336_write_reg_continue(i2c_bus, data, k + 2);
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
#ifdef I2C_WRITE_DEBUG
                    cnt++;
                    rt_kprintf("0x%x, ", k + 2);
                    for (j = 0; j < k + 2; j++)
                        rt_kprintf("0x%02x, ", data[j]);
                    rt_kprintf("\n");
#endif
                    SC3336_write_reg_continue(i2c_bus, data, k + 2);
                    reg_addr = reg[i].reg_addr;
                    data[0] = ((reg_addr >> 8) & 0xff);
                    data[1] = ((reg_addr >> 0) & 0xff);
                    data[2] = reg[i].val;
                    k = 1;
                }

            }
        }
    }
#ifdef I2C_WRITE_DEBUG
    rt_kprintf("0x%x,\n", cnt);
#endif

}
#endif

static int sc3336_get_gain_reg(uint8_t *again, uint8_t *dgain,
                               uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    if (total_gain < SC3336_GAIN_MIN)
        total_gain = SC3336_GAIN_MIN;
    else if (total_gain > SC3336_GAIN_MAX)
        total_gain = SC3336_GAIN_MAX;

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
static rt_err_t rk_sc3336_set_expval(struct sc3336_dev *dev, struct rk_camera_exp_val *exp)
{
    rt_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, d_gain_fine;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        SC3336_DEBUG("sc3336 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    rk_kprintf("sc3336: exp req: L_exp: 0x%x, 0x%x\n",
               l_exp_time, l_a_gain);

    ret  = SC3336_write_reg(dev->i2c_bus, SC3336_REG_EXP_LONG_H,
                            (uint8_t)((l_exp_time >> 12) & 0xf));
    ret |= SC3336_write_reg(dev->i2c_bus, SC3336_REG_EXP_LONG_M,
                            (uint8_t)((l_exp_time >> 4) & 0xff));
    ret |= SC3336_write_reg(dev->i2c_bus, SC3336_REG_EXP_LONG_L,
                            (uint8_t)((l_exp_time & 0xf) << 4));
    sc3336_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    rk_kprintf("%s: a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", __func__, a_gain, d_gain, d_gain_fine);

    ret = SC3336_write_reg(dev->i2c_bus, SC3336_REG_ANA_GAIN, a_gain);
    ret |= SC3336_write_reg(dev->i2c_bus, SC3336_REG_DIG_GAIN, d_gain);
    ret |= SC3336_write_reg(dev->i2c_bus, SC3336_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

static rt_err_t rk_sc3336_set_vts(struct sc3336_dev *dev, uint32_t dst_vts)
{
    rt_err_t ret = RET_SYS_ENOSYS;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    SC3336_DEBUG("(%s) set vts: 0x%x \n", __FUNCTION__, dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = SC3336_write_reg(dev->i2c_bus, SC3336_REG_VTS_L,
                           (uint8_t)(dst_vts & 0xff));
    ret |= SC3336_write_reg(dev->i2c_bus, SC3336_REG_VTS_H,
                            (uint8_t)(dst_vts >> 8));

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

static rt_err_t rk_sc3336_set_flip_mirror(struct sc3336_dev *dev, uint32_t flip)
{
    rt_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    ret = SC3336_read_reg(dev->i2c_bus, SC3336_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0:
        val = SC3336_FETCH_MIRROR(val, false);
        val = SC3336_FETCH_FLIP(val, false);
        break;
    case 1:
        val = SC3336_FETCH_MIRROR(val, true);
        val = SC3336_FETCH_FLIP(val, false);
        break;
    case 2:
        val = SC3336_FETCH_MIRROR(val, false);
        val = SC3336_FETCH_FLIP(val, true);
        break;
    case 3:
        val = SC3336_FETCH_MIRROR(val, true);
        val = SC3336_FETCH_FLIP(val, true);
        break;
    default:
        val = SC3336_FETCH_MIRROR(val, false);
        val = SC3336_FETCH_FLIP(val, false);
        break;
    };
    SC3336_DEBUG("(%s) flip 0x%x, reg val 0x%x\n", __FUNCTION__, flip, val);
    ret |= SC3336_write_reg(dev->i2c_bus, SC3336_FLIP_MIRROR_REG, val);
    SC3336_DEBUG("(%s) exit\n", __FUNCTION__);

    return ret;
}

static void SC3336_stream_on(struct sc3336_dev *dev)
{
    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    rk_kprintf("sc3336 on enter tick:%u\n", rt_tick_get());

    MACRO_ASSERT(dev != RK_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifdef I2C_WRITE_CONTINUE
    SC3336_write_multiple_reg_continue((struct sc3336_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#else
    SC3336_write_multiple_reg((struct sc3336_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc3336_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc3336_set_flip_mirror(dev, dev->flip);

    SC3336_write_reg(dev->i2c_bus, 0x0100, 0x01);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);
    rk_kprintf("sc3336 on exit tick:%u\n", rt_tick_get());
}

static void SC3336_stream_on_late(struct sc3336_dev *dev)
{

    rk_kprintf("%s enter tick:%u\n", __FUNCTION__, rt_tick_get());

    MACRO_ASSERT(dev != RK_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc3336_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc3336_set_flip_mirror(dev, dev->flip);

    SC3336_write_reg(dev->i2c_bus, 0x0100, 0x01);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    rk_kprintf("%s exit tick:%u\n", __FUNCTION__, rt_tick_get());
}

static void SC3336_stream_off(struct sc3336_dev *dev)
{
    rk_i2c_bus_device *i2c_bus;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        SC3336_write_reg(dev->i2c_bus, 0x0100, 0x00);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        SC3336_INFO(dev, "Err: not find out i2c bus!\n");
    }
    dev->streaming = false;

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);
}

ret_err_t rk_SC3336_init(struct rk_camera_device *dev)
{
    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct sc3336_dev *SC3336;

    SC3336 = (struct sc3336_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&SC3336->parent;
#if RT_USING_SC3336_OPS
    if (SC3336->ops->init)
    {
        return (SC3336->ops->init(SC3336));
    }
#else

    if (SC3336)
    {
        camera->info.mbus_fmt.width = 320;
        camera->info.mbus_fmt.height = 240;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = 112500000;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode = 0;

        SC3336->i2c_bus = (rk_i2c_bus_device *)rt_device_find(SC3336->i2c_name);

        if (!SC3336->i2c_bus)
        {
            SC3336_DEBUG("Warning:not find i2c source 2:%s !!!\n",
                         SC3336->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            SC3336_DEBUG("(%s):s0 find i2c_bus:%s\n", SC3336->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

static ret_err_t rk_SC3336_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
#if RT_USING_SC3336_OPS
    struct sc3336_dev *SC3336;
#endif
    ret_err_t ret = RET_SYS_EOK;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

#if RT_USING_SC3336_OPS
    SC3336 = (struct sc3336_dev *)dev;
    if (SC3336->ops->open)
    {
        return (SC3336->ops->open(SC3336, oflag));
    }
#endif

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

ret_err_t rk_SC3336_close(struct rk_camera_device *dev)
{
#if RT_USING_SC3336_OPS
    struct sc3336_dev *SC3336;
#endif
    uint8_t ret = RET_SYS_EOK;

    SC3336_INFO(dev, "(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

#if RT_USING_SC3336_OPS
    SC3336 = (struct sc3336_dev *)dev;
    if (SC3336->ops->close)
    {
        return (SC3336->ops->close(SC3336));
    }
#endif

    SC3336_INFO(dev, "(%s) exit \n", __FUNCTION__);
    return ret;

}

static rt_err_t rk_sc3336_get_expinf(struct sc3336_dev *dev, struct rk_camera_exp_info *exp)
{
    rt_err_t ret = RET_SYS_EOK;
    const struct sc3336_mode *mode;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    mode = dev->cur_mode;
    exp->width = mode->width;
    exp->height = mode->height;
    exp->hts = mode->hts_def;
    exp->vts = mode->vts_def;
    exp->pix_clk = (uint64_t)exp->hts * (uint64_t)exp->vts * (uint64_t)mode->max_fps.denominator /
                   (uint64_t)mode->max_fps.numerator;
    exp->time_valid_delay = 2;
    exp->gain_valid_delay = 2;

    exp->dst_width = dev->dst_mode->width;
    exp->dst_height = dev->dst_mode->height;
    exp->dst_hts = dev->dst_mode->hts_def;
    exp->dst_vts = dev->dst_mode->vts_def;
    exp->dst_pix_clk = (uint64_t)exp->dst_hts * (uint64_t)exp->dst_vts *
                       (uint64_t)dev->dst_mode->max_fps.denominator /
                       (uint64_t)dev->dst_mode->max_fps.numerator;

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static rt_err_t rk_sc3336_get_intput_fmt(struct sc3336_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    rt_err_t ret = RET_SYS_EOK;
    const struct sc3336_mode *mode;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

static rt_err_t rk_sc3336_set_intput_fmt(struct sc3336_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    rt_err_t ret = RET_SYS_EOK;
    const struct sc3336_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

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

#ifdef I2C_WRITE_CONTINUE
            SC3336_write_multiple_reg_continue((struct sc3336_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#else
            SC3336_write_multiple_reg((struct sc3336_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc3336_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

static rt_err_t rk_sc3336_match_dst_config(struct sc3336_dev *dev, struct rk_camera_dst_config *dst_config)
{
    rt_err_t ret = RET_SYS_ENOSYS;
    const struct sc3336_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = mode;
            ret = RET_SYS_EOK;
            SC3336_DEBUG("find match resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            SC3336_INFO(dev, "not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        rk_kprintf("Err dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

ret_err_t rk_SC3336_control(struct rk_camera_device *dev,
                            int cmd,
                            void *args)
{
    SC3336_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct sc3336_dev *SC3336;

    MACRO_ASSERT(dev != RK_NULL);
    SC3336 = (struct sc3336_dev *)dev;
#if RT_USING_SC3336_OPS
    return (SC3336->ops->control(SC3336, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_SC3336_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        SC3336_stream_on(SC3336);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        SC3336_stream_off(SC3336);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc3336_get_expinf(SC3336, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc3336_set_expval(SC3336, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc3336_set_vts(SC3336, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc3336_get_intput_fmt(SC3336, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc3336_set_intput_fmt(SC3336, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc3336_match_dst_config(SC3336, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc3336_set_flip_mirror(SC3336, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        SC3336_stream_on_late(SC3336);
    }
    break;
    default:
        SC3336_DEBUG("(%s) exit CMD %d\n", __FUNCTION__, cmd);
        break;
    }
#endif
    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

struct rk_camera_ops rk_sc3336_ops =
{
    rk_SC3336_init,
    rk_SC3336_open,
    NULL,
    rk_SC3336_control
};


int rk_camera_SC3336_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc3336_dev *instance = &g_sc3336;
    struct rk_camera_device *camera = &instance->parent;
    struct clk_gate *clkgate;

    camera->ops = &rk_sc3336_ops;

    rt_strncpy(instance->name, SC3336_DEVICE_NAME, rt_strlen(SC3336_DEVICE_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

#if SC3336_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        SC3336_INFO(instance, "Warning:not find i2c source 3:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        SC3336_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

    instance->cur_mode = &supported_modes[0];
    instance->dst_mode = &supported_modes[1];
    instance->flip = 0;

    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, 1);
    clk_set_rate(CAMERA_CLK_REF, 27000000);
    clkgate = get_clk_gate_from_id(CAMERA_CLK_REF);
    clk_enable(clkgate);

    rt_mutex_init(&instance->mutex_lock, "SC3336_mutex", RT_IPC_FLAG_FIFO);
    MACRO_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, instance->name, rt_strlen(SC3336_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);
    SC3336_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

void SC3336_detect(void)
{
    struct sc3336_dev *instance = &g_sc3336;

    SC3336_DEBUG("start to detect SC3336 for testing \n");
    SC3336_DEBUG("dev name:%s\n", instance->name);
    SC3336_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        SC3336_DEBUG("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        SC3336_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_SC3336_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(SC3336_detect, check SC3336 is available or not);
#endif
#endif
#endif

