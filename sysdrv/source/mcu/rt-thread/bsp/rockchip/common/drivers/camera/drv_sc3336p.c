/**
  * Copyright (c) 2024 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_sc3336p.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2024-02-22     HongLin Lan      first implementation
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

#ifdef RT_USING_SC3336P
#define sc3336p_debug_INFO      0

#if sc3336p_debug_INFO
#include <stdio.h>
#define sc3336p_debug(...)               rk_kprintf("[SC3336P]:");rk_kprintf(__VA_ARGS__)
#else
#define sc3336p_debug(...)
#endif

#define sc3336p_info(fmt, args...)         rt_kprintf("[SC3336P] "fmt"", ##args)
#define sc3336p_err(fmt, args...)         rt_kprintf("[SC3336P] ERR "fmt"", ##args)

#define I2C_WRITE_CONTINUE

#define SC3336P_XVCLK_FREQ_24M      24000000

#define SC3336P_REG_ID_H_ADDRESS     (0x3107)
#define SC3336P_REG_ID_L_ADDRESS     (0x3108)
#define SC3336P_SENSOR_ID            (0x9c41)
#define SC3336P_REG_EXP_LONG_H           0x3e00
#define SC3336P_REG_EXP_LONG_M           0x3e01
#define SC3336P_REG_EXP_LONG_L           0x3e02

#define SC3336P_REG_DIG_GAIN            0x3e06
#define SC3336P_REG_DIG_FINE_GAIN       0x3e07
#define SC3336P_REG_ANA_GAIN            0x3e09
#define SC3336P_EXP_MIN                 2
#define SC3336P_GAIN_MIN                0x0080
#define SC3336P_GAIN_MAX                (99614)  //48.64*16*128
#define SC3336P_GAIN_STEP               1
#define SC3336P_GAIN_DEFAULT            0x0080
#define SC3336P_LGAIN                   0
#define SC3336P_SGAIN                   1
#define SC3336P_FLIP_MIRROR_REG         0x3221
#define SC3336P_REG_VTS_H               0x320e
#define SC3336P_REG_VTS_L               0x320f
#define SC3336P_FETCH_MIRROR(VAL, ENABLE)       (ENABLE ? VAL | 0x06 : VAL & 0xf9)
#define SC3336P_FETCH_FLIP(VAL, ENABLE)         (ENABLE ? VAL | 0x60 : VAL & 0x9f)

#define SC3336P_REG_STREAM_CTRL         0x0100
#define SC3336P_STREAM_ON               0x01
#define SC3336P_STREAM_OFF              0x00

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

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

struct sc3336p_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct sc3336p_mode
{
    struct v4l2_fract max_fps;
    uint32_t bus_fmt;
    uint32_t width;
    uint32_t height;
    uint32_t hts_def;
    uint32_t vts_def;
    uint32_t exp_def;
    uint32_t xvclk;
    uint32_t link_freq;
#ifdef I2C_WRITE_CONTINUE
    const uint8_t *reg_list;
#else
    const struct sc3336p_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint32_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct sc3336p_dev
{
    struct rk_camera_device parent;
    struct rt_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    uint8_t time_valid_delay;
    uint8_t gain_valid_delay;
    struct rt_i2c_bus_device *i2c_bus;
    uint8_t i2c_addr;
    uint8_t flip;
    struct rt_mutex mutex_lock;
    struct rk_camera_exp_val init_exp;
    const struct sc3336p_mode *cur_mode;
    struct sc3336p_mode *dst_mode;
    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct sc3336p_dev *rt_sc3336p_dev_t;

static struct sc3336p_dev g_sc3336p;

#ifdef I2C_WRITE_CONTINUE
//cleaned_0x04_SC3336p_MIPI_24Minput_2Lane_10bit_255Mbps_1152x648_60.006fps_hbin_vbin.ini
static const uint8_t g_sc3336p_1152x648_60fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x30, 0x1f, 0x04,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0xb8, 0x33,
    0xa, 0x32, 0x08, 0x04, 0x80, 0x02, 0x88, 0x05, 0x00, 0x02, 0x98,
    0x3, 0x32, 0x11, 0x02,
    0x3, 0x32, 0x13, 0x02,
    0x3, 0x32, 0x15, 0x31,
    0x3, 0x32, 0x20, 0x01,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x20,
    0x3, 0x33, 0x01, 0x04,
    0x3, 0x33, 0x06, 0x50,
    0x5, 0x33, 0x09, 0xf8, 0x00, 0xd8,
    0x3, 0x33, 0x14, 0x13,
    0x3, 0x33, 0x1f, 0xe9,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x04, 0x04, 0x04, 0x08, 0x0b, 0x1f, 0x04, 0x0a, 0x3a, 0x60,
    0x3, 0x33, 0xa2, 0x04,
    0x6, 0x33, 0xac, 0x08, 0x1c, 0x10, 0x30,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x48,
    0x3, 0x33, 0xf9, 0x60,
    0x5, 0x33, 0xfb, 0x74, 0x4b, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x4b, 0x5f, 0x20, 0x18, 0x00, 0xe8, 0x01, 0x00,
    0x4, 0x34, 0xf8, 0x5f, 0x18,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x64, 0x32,
    0x4, 0x36, 0x3b, 0x03, 0x08,
    0x3, 0x36, 0x41, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xf0, 0xc0, 0xc0, 0x86, 0x86, 0x86,
    0x6, 0x36, 0x7c, 0x48, 0x49, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x22, 0x22, 0x33,
    0x4, 0x36, 0x9c, 0x4b, 0x4f,
    0x9, 0x36, 0xb0, 0x87, 0x90, 0xa1, 0xc8, 0x49, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x11, 0x0d, 0x2c, 0x26,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x5, 0x37, 0x71, 0x09, 0x09, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x5f,
    0x6, 0x37, 0xfa, 0x11, 0x33, 0x11, 0x08,
    0x4, 0x39, 0x04, 0x04, 0x8c,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0x1f, 0x49,
    0x3, 0x39, 0x21, 0x20,
    0x3, 0x39, 0x26, 0x21,
    0xc, 0x39, 0x33, 0x80, 0x08, 0x00, 0x90, 0x79, 0x77, 0x00, 0x00, 0x00, 0x28,
    0x3, 0x39, 0xdc, 0x02,
    0x4, 0x3e, 0x01, 0x29, 0x00,
    0x3, 0x3e, 0x09, 0x00,
    0x4, 0x44, 0x0d, 0x10, 0x01,
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
    0x3, 0x57, 0x80, 0x76,
    0x4, 0x57, 0x84, 0x10, 0x04,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x04, 0x0a, 0x0a, 0x04, 0x40,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04,
    0x4, 0x57, 0x99, 0x46, 0x77,
    0x3, 0x57, 0xa1, 0x04,
    0x3, 0x57, 0xa8, 0xd2,
    0x6, 0x57, 0xaa, 0x2a, 0x7f, 0x00, 0x00,
    0x4, 0x59, 0x00, 0xf1, 0x04,
    0x3, 0x59, 0x88, 0x70,
    0xe, 0x59, 0xe2, 0x08, 0x03, 0x00, 0x10, 0x06, 0x00, 0x08, 0x02, 0x00, 0x10, 0x04, 0x00,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x54,
    0x3, 0x37, 0xf9, 0x47,
    0x52,
};

//cleaned_0x02_SC3336p_MIPI_24Minput_2Lane_10bit_495Mbps_2304x1296_30fps.ini
static const uint8_t g_sc3336p_2304x1296_30fps_reg_table[] =
{
    0x3, 0x01, 0x03, 0x01,
    0x3, 0x36, 0xe9, 0x80,
    0x3, 0x37, 0xf9, 0x80,
    0x3, 0x30, 0x1f, 0x02,
    0x3, 0x30, 0xb8, 0x33,
    0x4, 0x32, 0x0e, 0x05, 0x28,
    0x3, 0x32, 0x53, 0x10,
    0x3, 0x32, 0x5f, 0x20,
    0x3, 0x33, 0x01, 0x04,
    0x3, 0x33, 0x06, 0x50,
    0x5, 0x33, 0x09, 0xf8, 0x00, 0xd8,
    0x3, 0x33, 0x14, 0x13,
    0x3, 0x33, 0x1f, 0xe9,
    0x4, 0x33, 0x33, 0x10, 0x40,
    0x4, 0x33, 0x5e, 0x06, 0x0a,
    0x3, 0x33, 0x64, 0x5e,
    0x4, 0x33, 0x7c, 0x02, 0x0e,
    0xf, 0x33, 0x90, 0x01, 0x03, 0x07, 0x04, 0x04, 0x04, 0x08, 0x0b, 0x1f, 0x04, 0x0a, 0x3a, 0x60,
    0x3, 0x33, 0xa2, 0x04,
    0x6, 0x33, 0xac, 0x08, 0x1c, 0x10, 0x30,
    0x3, 0x33, 0xb1, 0x80,
    0x3, 0x33, 0xb3, 0x48,
    0x3, 0x33, 0xf9, 0x60,
    0x5, 0x33, 0xfb, 0x74, 0x4b, 0x5f,
    0x3, 0x34, 0x9f, 0x03,
    0xa, 0x34, 0xa6, 0x4b, 0x5f, 0x20, 0x18, 0x00, 0xe8, 0x01, 0x00,
    0x4, 0x34, 0xf8, 0x5f, 0x18,
    0x6, 0x36, 0x30, 0xc0, 0x84, 0x64, 0x32,
    0x4, 0x36, 0x3b, 0x03, 0x08,
    0x3, 0x36, 0x41, 0x38,
    0x3, 0x36, 0x70, 0x4e,
    0x8, 0x36, 0x74, 0xf0, 0xc0, 0xc0, 0x86, 0x86, 0x86,
    0x6, 0x36, 0x7c, 0x48, 0x49, 0x4b, 0x5f,
    0x5, 0x36, 0x90, 0x22, 0x22, 0x33,
    0x4, 0x36, 0x9c, 0x4b, 0x4f,
    0x9, 0x36, 0xb0, 0x87, 0x90, 0xa1, 0xc8, 0x49, 0x4b, 0x4f,
    0x6, 0x36, 0xea, 0x0b, 0x0d, 0x1c, 0x26,
    0x3, 0x37, 0x0f, 0x01,
    0x3, 0x37, 0x22, 0x09,
    0x4, 0x37, 0x24, 0x41, 0xc1,
    0x5, 0x37, 0x71, 0x09, 0x09, 0x05,
    0x4, 0x37, 0x7a, 0x48, 0x5f,
    0x6, 0x37, 0xfa, 0x0b, 0x33, 0x11, 0x08,
    0x4, 0x39, 0x04, 0x04, 0x8c,
    0x3, 0x39, 0x1d, 0x04,
    0x3, 0x39, 0x1f, 0x49,
    0x3, 0x39, 0x21, 0x20,
    0x3, 0x39, 0x26, 0x21,
    0xc, 0x39, 0x33, 0x80, 0x08, 0x00, 0x90, 0x79, 0x77, 0x00, 0x00, 0x00, 0x28,
    0x3, 0x39, 0xdc, 0x02,
    0x4, 0x3e, 0x01, 0x52, 0x00,
    0x3, 0x3e, 0x09, 0x00,
    0x4, 0x44, 0x0d, 0x10, 0x01,
    0x3, 0x45, 0x09, 0x20,
    0x3, 0x57, 0x80, 0x76,
    0x4, 0x57, 0x84, 0x10, 0x04,
    0x9, 0x57, 0x87, 0x0a, 0x0a, 0x04, 0x0a, 0x0a, 0x04, 0x40,
    0x8, 0x57, 0x90, 0x08, 0x04, 0x04, 0x08, 0x04, 0x04,
    0x4, 0x57, 0x99, 0x46, 0x77,
    0x3, 0x57, 0xa1, 0x04,
    0x3, 0x57, 0xa8, 0xd2,
    0x6, 0x57, 0xaa, 0x2a, 0x7f, 0x00, 0x00,
    0xe, 0x59, 0xe2, 0x08, 0x03, 0x00, 0x10, 0x06, 0x00, 0x08, 0x02, 0x00, 0x10, 0x04, 0x00,
    0x10, 0x5a, 0xe0, 0xfe, 0x40, 0x38, 0x30, 0x28, 0x38, 0x30, 0x28, 0x3f, 0x34, 0x2c, 0x3f, 0x34, 0x2c,
    0x3, 0x36, 0xe9, 0x53,
    0x3, 0x37, 0xf9, 0x27,
    0x42,
};

#else
//cleaned_0x04_SC3336p_MIPI_24Minput_2Lane_10bit_255Mbps_1152x648_60.006fps_hbin_vbin.ini
static const struct sc3336p_sensor_reg g_sc3336p_1152x648_60fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x301f, 0x04},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x30b8, 0x33},
    {0x3208, 0x04},
    {0x3209, 0x80},
    {0x320a, 0x02},
    {0x320b, 0x88},
    {0x320c, 0x05},
    {0x320d, 0x00},
    {0x320e, 0x02},
    {0x320f, 0x98},
    {0x3211, 0x02},
    {0x3213, 0x02},
    {0x3215, 0x31},
    {0x3220, 0x01},
    {0x3253, 0x10},
    {0x325f, 0x20},
    {0x3301, 0x04},
    {0x3306, 0x50},
    {0x3309, 0xf8},
    {0x330a, 0x00},
    {0x330b, 0xd8},
    {0x3314, 0x13},
    {0x331f, 0xe9},
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
    {0x339c, 0x60},
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
    {0x34aa, 0x00},
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
    {0x3674, 0xf0},
    {0x3675, 0xc0},
    {0x3676, 0xc0},
    {0x3677, 0x86},
    {0x3678, 0x86},
    {0x3679, 0x86},
    {0x367c, 0x48},
    {0x367d, 0x49},
    {0x367e, 0x4b},
    {0x367f, 0x5f},
    {0x3690, 0x22},
    {0x3691, 0x22},
    {0x3692, 0x33},
    {0x369c, 0x4b},
    {0x369d, 0x4f},
    {0x36b0, 0x87},
    {0x36b1, 0x90},
    {0x36b2, 0xa1},
    {0x36b3, 0xc8},
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
    {0x391f, 0x49},
    {0x3921, 0x20},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x08},
    {0x3935, 0x00},
    {0x3936, 0x90},
    {0x3937, 0x79},
    {0x3938, 0x77},
    {0x3939, 0x00},
    {0x393a, 0x00},
    {0x393b, 0x00},
    {0x393c, 0x28},
    {0x39dc, 0x02},
    {0x3e01, 0x29},
    {0x3e02, 0x00},
    {0x3e09, 0x00},
    {0x440d, 0x10},
    {0x440e, 0x01},
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
    {0x5780, 0x76},
    {0x5784, 0x10},
    {0x5785, 0x04},
    {0x5787, 0x0a},
    {0x5788, 0x0a},
    {0x5789, 0x04},
    {0x578a, 0x0a},
    {0x578b, 0x0a},
    {0x578c, 0x04},
    {0x578d, 0x40},
    {0x5790, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x04},
    {0x5793, 0x08},
    {0x5794, 0x04},
    {0x5795, 0x04},
    {0x5799, 0x46},
    {0x579a, 0x77},
    {0x57a1, 0x04},
    {0x57a8, 0xd2},
    {0x57aa, 0x2a},
    {0x57ab, 0x7f},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x5900, 0xf1},
    {0x5901, 0x04},
    {0x5988, 0x70},
    {0x59e2, 0x08},
    {0x59e3, 0x03},
    {0x59e4, 0x00},
    {0x59e5, 0x10},
    {0x59e6, 0x06},
    {0x59e7, 0x00},
    {0x59e8, 0x08},
    {0x59e9, 0x02},
    {0x59ea, 0x00},
    {0x59eb, 0x10},
    {0x59ec, 0x04},
    {0x59ed, 0x00},
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

//cleaned_0x02_SC3336p_MIPI_24Minput_2Lane_10bit_495Mbps_2304x1296_30fps.ini
static const struct sc3336p_sensor_reg g_sc3336p_2304x1296_30fps_reg_table[] =
{
    {0x0103, 0x01},
    {0x36e9, 0x80},
    {0x37f9, 0x80},
    {0x301f, 0x02},
    {0x30b8, 0x33},
    {0x320e, 0x05},
    {0x320f, 0x28},
    {0x3253, 0x10},
    {0x325f, 0x20},
    {0x3301, 0x04},
    {0x3306, 0x50},
    {0x3309, 0xf8},
    {0x330a, 0x00},
    {0x330b, 0xd8},
    {0x3314, 0x13},
    {0x331f, 0xe9},
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
    {0x339c, 0x60},
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
    {0x34aa, 0x00},
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
    {0x3674, 0xf0},
    {0x3675, 0xc0},
    {0x3676, 0xc0},
    {0x3677, 0x86},
    {0x3678, 0x86},
    {0x3679, 0x86},
    {0x367c, 0x48},
    {0x367d, 0x49},
    {0x367e, 0x4b},
    {0x367f, 0x5f},
    {0x3690, 0x22},
    {0x3691, 0x22},
    {0x3692, 0x33},
    {0x369c, 0x4b},
    {0x369d, 0x4f},
    {0x36b0, 0x87},
    {0x36b1, 0x90},
    {0x36b2, 0xa1},
    {0x36b3, 0xc8},
    {0x36b4, 0x49},
    {0x36b5, 0x4b},
    {0x36b6, 0x4f},
    {0x36ea, 0x0b},
    {0x36eb, 0x0d},
    {0x36ec, 0x1c},
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
    {0x37fa, 0x0b},
    {0x37fb, 0x33},
    {0x37fc, 0x11},
    {0x37fd, 0x08},
    {0x3904, 0x04},
    {0x3905, 0x8c},
    {0x391d, 0x04},
    {0x391f, 0x49},
    {0x3921, 0x20},
    {0x3926, 0x21},
    {0x3933, 0x80},
    {0x3934, 0x08},
    {0x3935, 0x00},
    {0x3936, 0x90},
    {0x3937, 0x79},
    {0x3938, 0x77},
    {0x3939, 0x00},
    {0x393a, 0x00},
    {0x393b, 0x00},
    {0x393c, 0x28},
    {0x39dc, 0x02},
    {0x3e01, 0x52},
    {0x3e02, 0x00},
    {0x3e09, 0x00},
    {0x440d, 0x10},
    {0x440e, 0x01},
    {0x4509, 0x20},
    {0x5780, 0x76},
    {0x5784, 0x10},
    {0x5785, 0x04},
    {0x5787, 0x0a},
    {0x5788, 0x0a},
    {0x5789, 0x04},
    {0x578a, 0x0a},
    {0x578b, 0x0a},
    {0x578c, 0x04},
    {0x578d, 0x40},
    {0x5790, 0x08},
    {0x5791, 0x04},
    {0x5792, 0x04},
    {0x5793, 0x08},
    {0x5794, 0x04},
    {0x5795, 0x04},
    {0x5799, 0x46},
    {0x579a, 0x77},
    {0x57a1, 0x04},
    {0x57a8, 0xd2},
    {0x57aa, 0x2a},
    {0x57ab, 0x7f},
    {0x57ac, 0x00},
    {0x57ad, 0x00},
    {0x59e2, 0x08},
    {0x59e3, 0x03},
    {0x59e4, 0x00},
    {0x59e5, 0x10},
    {0x59e6, 0x06},
    {0x59e7, 0x00},
    {0x59e8, 0x08},
    {0x59e9, 0x02},
    {0x59ea, 0x00},
    {0x59eb, 0x10},
    {0x59ec, 0x04},
    {0x59ed, 0x00},
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
    {0x36e9, 0x53},
    {0x37f9, 0x27},
    {REG_END, 0x00},
};
#endif

static const struct sc3336p_mode supported_modes[] =
{
    [SC3336P_1152X648] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1152,
        .height = 648,
        .max_fps = {
            .numerator = 10000,
            .denominator = 600000,
        },
        .exp_def = 0x80,
        .hts_def = 0x0500,
        .vts_def = 0x0298,
        .xvclk = SC3336P_XVCLK_FREQ_24M,
        .link_freq = 127500000,
        .reg_list = g_sc3336p_1152x648_60fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc3336p_1152x648_60fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [SC3336P_2304X1296] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 2304,
        .height = 1296,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x80,
        .hts_def = 0x05dc,
        .vts_def = 0x0528,
        .xvclk = SC3336P_XVCLK_FREQ_24M,
        .link_freq = 247500000,
        .reg_list = g_sc3336p_2304x1296_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_sc3336p_2304x1296_30fps_reg_table),
        .hdr_mode = NO_HDR,
    }
};

struct sc3336p_ops
{
    ret_err_t (*init)(struct sc3336p_dev *dev);
    ret_err_t (*open)(struct sc3336p_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct sc3336p_dev *dev);
    ret_err_t (*control)(struct sc3336p_dev *dev, int cmd, void *arg);
};

static ret_err_t sc3336p_read_reg(rk_i2c_bus_device *bus,
                                  uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    int retry = 10;
    ret_err_t ret;

    RT_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    msg[0].addr = g_sc3336p.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_sc3336p.i2c_addr;
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
            sc3336p_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t sc3336p_write_reg(rk_i2c_bus_device *bus,
                                   uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    send_buf[2] = data;

    msg.addr = g_sc3336p.i2c_addr;
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
            sc3336p_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifdef I2C_WRITE_CONTINUE
static int sc3336p_write_multiple_reg_continue(struct sc3336p_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    rk_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msg;
    int ret = 0;
    int offset = 0;
    int retry = 10;

    RT_ASSERT(dev != RT_NULL && i2c_data != RT_NULL);

    i2c_bus = dev->i2c_bus;
    RT_ASSERT(i2c_bus != RK_NULL);

    for (i = 0; i < i2c_data[len - 1];)
    {
        msg.addr = g_sc3336p.i2c_addr;
        msg.flags = RT_I2C_WR;
        msg.buf = (uint8_t *)&i2c_data[offset + 1];
        msg.len = i2c_data[offset];
        ret = rt_i2c_transfer(i2c_bus, &msg, 1);
        if (ret != 1)
        {
            sc3336p_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
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
static ret_err_t sc3336p_write_reg_continue(rk_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RK_NULL);

    msgs.addr = g_sc3336p.i2c_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = data;
    msgs.len = len;

    while (1)
    {
        ret = rt_i2c_transfer(bus, &msgs, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            sc3336p_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void sc3336p_write_multiple_reg(struct sc3336p_dev *dev,
                                       const struct sc3336p_sensor_reg *reg, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    int k = 0;
    char *data = rt_malloc(len);
    uint16_t reg_addr;
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
                rt_kprintf("0x%x, ", k + 2);
                for (j = 0; j < k + 2; j++)
                    rt_kprintf("0x%02x, ", data[j]);
                rt_kprintf("\n");

                sc3336p_write_reg_discontinued(i2c_bus, data, k + 2);
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

                sc3336p_write_reg_discontinued(i2c_bus, data, k + 2);
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
                    cnt++;
                    rt_kprintf("0x%x, ", k + 2);
                    for (j = 0; j < k + 2; j++)
                        rt_kprintf("0x%02x, ", data[j]);
                    rt_kprintf("\n");

                    sc3336p_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct sc3336p_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        sc3336p_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        sc3336p_write_multiple_reg((struct sc3336p_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

static int sc3336p_get_gain_reg(uint8_t *again, uint8_t *dgain,
                                uint8_t *dgain_fine, uint32_t total_gain)
{
    int ret = 0;
    uint32_t gain_factor = 0;

    if (total_gain < SC3336P_GAIN_MIN)
        total_gain = SC3336P_GAIN_MIN;
    else if (total_gain > SC3336P_GAIN_MAX)
        total_gain = SC3336P_GAIN_MAX;

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

static rt_err_t rk_sc3336p_set_expval(struct sc3336p_dev *dev, struct rk_camera_exp_val *exp)
{
    rt_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint8_t a_gain, d_gain, d_gain_fine;

    RT_ASSERT(dev != RK_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        sc3336p_debug("sc3336p don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    if (l_exp_time < SC3336P_EXP_MIN)
        l_exp_time = SC3336P_EXP_MIN;

    sc3336p_debug("total_time reg: 0x%x, total_gain reg 0x%x\n",
                  l_exp_time, l_a_gain);

    ret  = sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_EXP_LONG_H,
                             (uint8_t)((l_exp_time >> 12) & 0xf));
    ret |= sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_EXP_LONG_M,
                             (uint8_t)((l_exp_time >> 4) & 0xff));
    ret |= sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_EXP_LONG_L,
                             (uint8_t)((l_exp_time & 0xf) << 4));
    sc3336p_get_gain_reg(&a_gain, &d_gain, &d_gain_fine, l_a_gain);

    sc3336p_debug("%s: a_gain: 0x%x d_gain: 0x%x, d_gain_fine: 0x%x\n", __func__, a_gain, d_gain, d_gain_fine);

    ret = sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_ANA_GAIN, a_gain);
    ret |= sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_DIG_GAIN, d_gain);
    ret |= sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_DIG_FINE_GAIN, d_gain_fine);

    rt_mutex_release(&dev->mutex_lock);

    return ret;
}

static rt_err_t rk_sc3336p_set_vts(struct sc3336p_dev *dev, uint32_t dst_vts)
{
    rt_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RK_NULL);

    sc3336p_debug("(%s) set vts: 0x%x \n", __FUNCTION__, dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_VTS_L,
                            (uint8_t)(dst_vts & 0xff));
    ret |= sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_VTS_H,
                             (uint8_t)(dst_vts >> 8));

    return ret;
}

static rt_err_t rk_sc3336p_set_flip_mirror(struct sc3336p_dev *dev, uint32_t flip)
{
    rt_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RK_NULL);

    ret = sc3336p_read_reg(dev->i2c_bus, SC3336P_FLIP_MIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = SC3336P_FETCH_MIRROR(val, flip & 0x01);
        val = SC3336P_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = SC3336P_FETCH_MIRROR(val, false);
        val = SC3336P_FETCH_FLIP(val, false);
        break;
    };
    sc3336p_debug("(%s) flip 0x%x, reg val 0x%x\n", __FUNCTION__, flip, val);
    ret |= sc3336p_write_reg(dev->i2c_bus, SC3336P_FLIP_MIRROR_REG, val);
    return ret;
}

static void sc3336p_stream_on(struct sc3336p_dev *dev)
{
    sc3336p_debug("(%s) enter tick:%u\n", __FUNCTION__, rt_tick_get());

    RT_ASSERT(dev != RK_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifdef I2C_WRITE_CONTINUE
    sc3336p_write_multiple_reg_continue((struct sc3336p_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#else
    sc3336p_write_multiple_reg((struct sc3336p_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc3336p_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc3336p_set_flip_mirror(dev, dev->flip);

    sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_STREAM_CTRL, SC3336P_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    sc3336p_debug("(%s) exit tick:%u\n", __FUNCTION__, rt_tick_get());
}

static void sc3336p_stream_on_late(struct sc3336p_dev *dev)
{

    sc3336p_debug("%s enter tick:%u\n", __FUNCTION__, rt_tick_get());

    RT_ASSERT(dev != RK_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_sc3336p_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_sc3336p_set_flip_mirror(dev, dev->flip);

    sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_STREAM_CTRL, SC3336P_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    sc3336p_debug("%s exit tick:%u\n", __FUNCTION__, rt_tick_get());
}

static void sc3336p_stream_off(struct sc3336p_dev *dev)
{
    rk_i2c_bus_device *i2c_bus;

    sc3336p_debug("(%s) enter \n", __FUNCTION__);

    RT_ASSERT(dev != RK_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        sc3336p_write_reg(dev->i2c_bus, SC3336P_REG_STREAM_CTRL, SC3336P_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        sc3336p_info("Err: not find out i2c bus!\n");
    }
    dev->streaming = false;

    sc3336p_debug("(%s) exit \n", __FUNCTION__);
}

ret_err_t rk_sc3336p_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc3336p_dev *sc3336p;

    sc3336p = (struct sc3336p_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&sc3336p->parent;

    if (sc3336p)
    {
        camera->info.mbus_fmt.width = sc3336p->cur_mode->width;
        camera->info.mbus_fmt.height = sc3336p->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = sc3336p->cur_mode->bus_fmt;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = sc3336p->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode = sc3336p->cur_mode->hdr_mode;

        sc3336p->i2c_bus = (rk_i2c_bus_device *)rt_device_find(sc3336p->i2c_name);

        if (!sc3336p->i2c_bus)
        {
            sc3336p_err("not find i2c source 2:%s !!!\n",
                        sc3336p->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            sc3336p_debug("find i2c_bus:%s\n", sc3336p->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_sc3336p_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_sc3336p_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static rt_err_t rk_sc3336p_get_expinf(struct sc3336p_dev *dev, struct rk_camera_exp_info *exp)
{
    rt_err_t ret = RET_SYS_EOK;
    const struct sc3336p_mode *mode;

    RT_ASSERT(dev != RK_NULL);

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

static rt_err_t rk_sc3336p_get_intput_fmt(struct sc3336p_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    rt_err_t ret = RET_SYS_EOK;
    const struct sc3336p_mode *mode;

    RT_ASSERT(dev != RK_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = mode->bus_fmt;

    return ret;
}

static rt_err_t rk_sc3336p_set_intput_fmt(struct sc3336p_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    rt_err_t ret = RET_SYS_EOK;
    const struct sc3336p_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RK_NULL);

    sc3336p_info("dst resulotion, width %d, height %d\n",
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

#ifdef I2C_WRITE_CONTINUE
            sc3336p_write_multiple_reg_continue((struct sc3336p_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#else
            sc3336p_write_multiple_reg((struct sc3336p_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_sc3336p_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static rt_err_t rk_sc3336p_match_dst_config(struct sc3336p_dev *dev, struct rk_camera_dst_config *dst_config)
{
    rt_err_t ret = RET_SYS_ENOSYS;
    const struct sc3336p_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RK_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;
    sc3336p_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct sc3336p_mode *)mode;
            ret = RET_SYS_EOK;
            sc3336p_info("find match resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            sc3336p_debug("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        sc3336p_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_sc3336p_control(struct rk_camera_device *dev,
                             int cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc3336p_dev *sc3336p;

    RT_ASSERT(dev != RK_NULL);
    sc3336p = (struct sc3336p_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_sc3336p_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        sc3336p_stream_on(sc3336p);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        sc3336p_stream_off(sc3336p);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_sc3336p_get_expinf(sc3336p, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_sc3336p_set_expval(sc3336p, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_sc3336p_set_vts(sc3336p, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_sc3336p_get_intput_fmt(sc3336p, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_sc3336p_set_intput_fmt(sc3336p, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_sc3336p_match_dst_config(sc3336p, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_sc3336p_set_flip_mirror(sc3336p, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        sc3336p_stream_on_late(sc3336p);
    }
    break;
    default:
        sc3336p_debug("(%s) exit CMD %d\n", __FUNCTION__, cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_sc3336p_ops =
{
    rk_sc3336p_init,
    rk_sc3336p_open,
    NULL,
    rk_sc3336p_control
};

int rk_camera_3336p_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct sc3336p_dev *instance = &g_sc3336p;
    struct rk_camera_device *camera = &instance->parent;
    struct camera_board_desc *sc3336p = (struct camera_board_desc *)&camera_sc3336p;

    camera->ops = &rk_sc3336p_ops;

    rt_strncpy(instance->name, sc3336p->isp_subdev_name, rt_strlen(sc3336p->isp_subdev_name));
    rt_strncpy(instance->i2c_name, sc3336p->i2c_bus, rt_strlen(sc3336p->i2c_bus));

    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (sc3336p->mode_id <= SC3336P_MODE_ID_MAX)
    {
        sc3336p_debug(instance, "mode_id: %d\n", sc3336p->mode_id);
        instance->cur_mode = &supported_modes[sc3336p->mode_id];
    }
    else
    {
        sc3336p_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }

    instance->dst_mode = (struct sc3336p_mode *)&supported_modes[SC3336P_2304X1296];
    instance->time_valid_delay = sc3336p->time_valid_delay;
    instance->gain_valid_delay = sc3336p->gain_valid_delay;
    instance->i2c_addr = sc3336p->i2c_addr;
    instance->flip = 0;

    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, 1);

    if (sc3336p->pwdn_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)sc3336p->pwdn_gpio.gpio_group, sc3336p->pwdn_gpio.gpio_pin, GPIO_OUT);
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)sc3336p->pwdn_gpio.gpio_group, sc3336p->pwdn_gpio.gpio_pin, !sc3336p->pwdn_active);
    }

    if (sc3336p->mclk_out_gate_id && sc3336p->mclk_id)
    {
        clk_set_rate(sc3336p->mclk_id, instance->cur_mode->xvclk);
        HAL_CRU_ClkEnable(sc3336p->mclk_out_gate_id);
    }

    rt_mutex_init(&instance->mutex_lock, "SC3336P_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, sc3336p->isp_subdev_name, rt_strlen(sc3336p->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    sc3336p_debug("(%s) exit \n", __FUNCTION__);
    return ret;
}

void sc3336p_detect(void)
{
    struct sc3336p_dev *instance = &g_sc3336p;

    sc3336p_debug("start to detect SC3336P for testing \n");
    sc3336p_debug("dev name:%s\n", instance->name);
    sc3336p_debug("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        sc3336p_debug("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        sc3336p_debug("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_3336p_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(sc3336p_detect, check SC3336P is available or not);
#endif
#endif
#endif

