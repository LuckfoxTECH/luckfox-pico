/**
  * Copyright (c) 2023 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_imx415.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2023-12-11     HongLin Lan     first implementation
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

#ifdef RT_USING_IMX415

//#define REG_LIST_CONVERSION_DEBUG
#define IMX415_DEBUG_PRT      0

#if IMX415_DEBUG_PRT
#include <stdio.h>
#define imx415_dbg(fmt, args...)               rt_kprintf("[IMX415] "fmt"", ##args)
#else
#define imx415_dbg(fmt, args...)
#endif

#define imx415_info(fmt, args...)         rt_kprintf("[IMX415] "fmt"", ##args)
#define imx415_err(fmt, args...)         rt_kprintf("[IMX415] ERR "fmt"", ##args)

#define IMX415_LF_EXPO_REG_H        0x3052
#define IMX415_LF_EXPO_REG_M        0x3051
#define IMX415_LF_EXPO_REG_L        0x3050

#define IMX415_SF1_EXPO_REG_H       0x3056
#define IMX415_SF1_EXPO_REG_M       0x3055
#define IMX415_SF1_EXPO_REG_L       0x3054

#define IMX415_LF_GAIN_REG_H        0x3091
#define IMX415_LF_GAIN_REG_L        0x3090
#define IMX415_SF1_GAIN_REG_H       0x3093
#define IMX415_SF1_GAIN_REG_L       0x3092
#define IMX415_SF2_GAIN_REG_H       0x3095
#define IMX415_SF2_GAIN_REG_L       0x3094

#define IMX415_GAIN_MIN                0x00
#define IMX415_GAIN_MAX                0xf0
#define IMX415_GAIN_STEP               1
#define IMX415_GAIN_DEFAULT            0x00

#define IMX415_FLIP_REG             0x3030
#define IMX415_VTS_REG_L            0x3024
#define IMX415_VTS_REG_M            0x3025
#define IMX415_VTS_REG_H            0x3026
#define IMX415_MIRROR_BIT_MASK      (1UL << 0)
#define IMX415_FLIP_BIT_MASK        (1UL << 1)

#define IMX415_REG_CTRL_MODE        0x3000
#define IMX415_STREAM_ON            0x00
#define IMX415_STREAM_OFF           0x01

#define REG_END                     (0xFFFF)
#define REG_DELAY                   (0xFFFE)

#define IMX415_2LANES               2

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

struct imx415_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct imx415_mode
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
    const struct imx415_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct imx415_dev
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
    const struct imx415_mode *cur_mode;
    struct imx415_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct imx415_dev *rt_imx415_dev_t;

static struct imx415_dev g_imx415;

#ifndef REG_LIST_CONVERSION_DEBUG

/*
 * Xclk 27Mhz
 * 90.059fps
 * CSI-2_2lane
 * AD:10bit Output:12bit
 * 2376Mbps
 * Master Mode
 * Time 9.999ms Gain:6dB
 * 2568x1440 2/2-line binning & Window cropping
 */
static const uint8_t g_imx415_1284X720_binning_crop_90fps_reg_table[] =
{
    0x3, 0x30, 0x08, 0x5d,
    0x3, 0x30, 0x0a, 0x42,
    0x3, 0x30, 0x1c, 0x04,
    0x5, 0x30, 0x20, 0x01, 0x01, 0x01,
    0x4, 0x30, 0x24, 0xab, 0x07,
    0x4, 0x30, 0x28, 0xa4, 0x01,
    0x3, 0x30, 0x31, 0x00,
    0x3, 0x30, 0x33, 0x00,
    0xa, 0x30, 0x40, 0x88, 0x02, 0x08, 0x0a, 0xf0, 0x02, 0x40, 0x0b,
    0x3, 0x30, 0x50, 0xc4,
    0x3, 0x30, 0x90, 0x14,
    0x3, 0x30, 0xc1, 0x00,
    0x4, 0x30, 0xd9, 0x02, 0x01,
    0x3, 0x31, 0x16, 0x23,
    0x5, 0x31, 0x18, 0x08, 0x01, 0xe7,
    0x3, 0x31, 0x1e, 0x23,
    0x3, 0x32, 0xd4, 0x21,
    0x3, 0x32, 0xec, 0xa1,
    0xa, 0x34, 0x4c, 0x2b, 0x01, 0xed, 0x01, 0xf6, 0x02, 0x7f, 0x03,
    0x3, 0x35, 0x8a, 0x04,
    0x3, 0x35, 0xa1, 0x02,
    0x3, 0x35, 0xec, 0x27,
    0x3, 0x35, 0xee, 0x8d,
    0x3, 0x35, 0xf0, 0x8d,
    0x3, 0x35, 0xf2, 0x29,
    0x3, 0x36, 0xbc, 0x0c,
    0x5, 0x36, 0xcc, 0x53, 0x00, 0x3c,
    0x5, 0x36, 0xd0, 0x8c, 0x00, 0x71,
    0x3, 0x36, 0xd4, 0x3c,
    0x5, 0x36, 0xd6, 0x53, 0x00, 0x71,
    0x4, 0x36, 0xda, 0x8c, 0x00,
    0x3, 0x37, 0x01, 0x00,
    0x3, 0x37, 0x20, 0x00,
    0x3, 0x37, 0x24, 0x02,
    0x3, 0x37, 0x26, 0x02,
    0x3, 0x37, 0x32, 0x02,
    0x3, 0x37, 0x34, 0x03,
    0x3, 0x37, 0x36, 0x03,
    0x3, 0x37, 0x42, 0x03,
    0x3, 0x38, 0x62, 0xe0,
    0x4, 0x38, 0xcc, 0x30, 0x2f,
    0x3, 0x39, 0x5c, 0x0c,
    0x3, 0x39, 0xa4, 0x07,
    0x3, 0x39, 0xa8, 0x32,
    0x3, 0x39, 0xaa, 0x32,
    0x3, 0x39, 0xac, 0x32,
    0x3, 0x39, 0xae, 0x32,
    0x3, 0x39, 0xb0, 0x32,
    0x3, 0x39, 0xb2, 0x2f,
    0x3, 0x39, 0xb4, 0x2d,
    0x3, 0x39, 0xb6, 0x28,
    0x3, 0x39, 0xb8, 0x30,
    0x3, 0x39, 0xba, 0x30,
    0x3, 0x39, 0xbc, 0x30,
    0x3, 0x39, 0xbe, 0x30,
    0x3, 0x39, 0xc0, 0x30,
    0x3, 0x39, 0xc2, 0x2e,
    0x3, 0x39, 0xc4, 0x2b,
    0x3, 0x39, 0xc6, 0x25,
    0x3, 0x3a, 0x42, 0xd1,
    0x3, 0x3a, 0x4c, 0x77,
    0x3, 0x3a, 0xe0, 0x02,
    0x3, 0x3a, 0xec, 0x0c,
    0x3, 0x3b, 0x00, 0x2e,
    0x3, 0x3b, 0x06, 0x29,
    0x4, 0x3b, 0x98, 0x25, 0x21,
    0x6, 0x3b, 0x9b, 0x13, 0x13, 0x13, 0x13,
    0xb, 0x3b, 0xa1, 0x00, 0x06, 0x0b, 0x10, 0x14, 0x18, 0x1a, 0x1a, 0x1a,
    0xf, 0x3b, 0xac, 0xed, 0x01, 0xf6, 0x02, 0xa2, 0x03, 0xe0, 0x03, 0xe0, 0x03, 0xe0, 0x03, 0xe0,
    0x3, 0x3b, 0xba, 0xe0,
    0x3, 0x3b, 0xbc, 0xda,
    0x3, 0x3b, 0xbe, 0x88,
    0x3, 0x3b, 0xc0, 0x44,
    0x3, 0x3b, 0xc2, 0x7b,
    0x3, 0x3b, 0xc4, 0xa2,
    0x3, 0x3b, 0xc8, 0xbd,
    0x3, 0x3b, 0xca, 0xbd,
    0x3, 0x40, 0x01, 0x01,
    0x4, 0x40, 0x04, 0xc0, 0x06,
    0x3, 0x40, 0x18, 0xe7,
    0x3, 0x40, 0x1a, 0x8f,
    0x3, 0x40, 0x1c, 0x8f,
    0x5, 0x40, 0x1e, 0x7f, 0x02, 0x97,
    0x5, 0x40, 0x22, 0x0f, 0x01, 0x97,
    0x3, 0x40, 0x26, 0xf7,
    0x3, 0x40, 0x28, 0x7f,
    0x3, 0x30, 0x02, 0x00,
    0x3, 0xff, 0xfe, 0x0a,
    0x58,
};

/*
 * Xclk 27Mhz
 * 15fps
 * CSI-2_2lane
 * AD:12bit Output:12bit
 * 891Mbps
 * Master Mode
 * Time 9.988ms Gain:6dB
 * All-pixel
 */
static const uint8_t g_imx415_3864X2192_15fps_reg_table[] =
{
    0x3, 0x30, 0x08, 0x5d,
    0x3, 0x30, 0x0a, 0x42,
    0x4, 0x30, 0x28, 0x98, 0x08,
    0x3, 0x30, 0x33, 0x05,
    0x4, 0x30, 0x50, 0x79, 0x07,
    0x3, 0x30, 0x90, 0x14,
    0x3, 0x30, 0xc1, 0x00,
    0x3, 0x31, 0x16, 0x23,
    0x3, 0x31, 0x18, 0xc6,
    0x3, 0x31, 0x1a, 0xe7,
    0x3, 0x31, 0x1e, 0x23,
    0x3, 0x32, 0xd4, 0x21,
    0x3, 0x32, 0xec, 0xa1,
    0xa, 0x34, 0x4c, 0x2b, 0x01, 0xed, 0x01, 0xf6, 0x02, 0x7f, 0x03,
    0x3, 0x35, 0x8a, 0x04,
    0x3, 0x35, 0xa1, 0x02,
    0x3, 0x35, 0xec, 0x27,
    0x3, 0x35, 0xee, 0x8d,
    0x3, 0x35, 0xf0, 0x8d,
    0x3, 0x35, 0xf2, 0x29,
    0x3, 0x36, 0xbc, 0x0c,
    0x5, 0x36, 0xcc, 0x53, 0x00, 0x3c,
    0x5, 0x36, 0xd0, 0x8c, 0x00, 0x71,
    0x3, 0x36, 0xd4, 0x3c,
    0x5, 0x36, 0xd6, 0x53, 0x00, 0x71,
    0x4, 0x36, 0xda, 0x8c, 0x00,
    0x3, 0x37, 0x20, 0x00,
    0x3, 0x37, 0x24, 0x02,
    0x3, 0x37, 0x26, 0x02,
    0x3, 0x37, 0x32, 0x02,
    0x3, 0x37, 0x34, 0x03,
    0x3, 0x37, 0x36, 0x03,
    0x3, 0x37, 0x42, 0x03,
    0x3, 0x38, 0x62, 0xe0,
    0x4, 0x38, 0xcc, 0x30, 0x2f,
    0x3, 0x39, 0x5c, 0x0c,
    0x3, 0x39, 0xa4, 0x07,
    0x3, 0x39, 0xa8, 0x32,
    0x3, 0x39, 0xaa, 0x32,
    0x3, 0x39, 0xac, 0x32,
    0x3, 0x39, 0xae, 0x32,
    0x3, 0x39, 0xb0, 0x32,
    0x3, 0x39, 0xb2, 0x2f,
    0x3, 0x39, 0xb4, 0x2d,
    0x3, 0x39, 0xb6, 0x28,
    0x3, 0x39, 0xb8, 0x30,
    0x3, 0x39, 0xba, 0x30,
    0x3, 0x39, 0xbc, 0x30,
    0x3, 0x39, 0xbe, 0x30,
    0x3, 0x39, 0xc0, 0x30,
    0x3, 0x39, 0xc2, 0x2e,
    0x3, 0x39, 0xc4, 0x2b,
    0x3, 0x39, 0xc6, 0x25,
    0x3, 0x3a, 0x42, 0xd1,
    0x3, 0x3a, 0x4c, 0x77,
    0x3, 0x3a, 0xe0, 0x02,
    0x3, 0x3a, 0xec, 0x0c,
    0x3, 0x3b, 0x00, 0x2e,
    0x3, 0x3b, 0x06, 0x29,
    0x4, 0x3b, 0x98, 0x25, 0x21,
    0x6, 0x3b, 0x9b, 0x13, 0x13, 0x13, 0x13,
    0xb, 0x3b, 0xa1, 0x00, 0x06, 0x0b, 0x10, 0x14, 0x18, 0x1a, 0x1a, 0x1a,
    0xf, 0x3b, 0xac, 0xed, 0x01, 0xf6, 0x02, 0xa2, 0x03, 0xe0, 0x03, 0xe0, 0x03, 0xe0, 0x03, 0xe0,
    0x3, 0x3b, 0xba, 0xe0,
    0x3, 0x3b, 0xbc, 0xda,
    0x3, 0x3b, 0xbe, 0x88,
    0x3, 0x3b, 0xc0, 0x44,
    0x3, 0x3b, 0xc2, 0x7b,
    0x3, 0x3b, 0xc4, 0xa2,
    0x3, 0x3b, 0xc8, 0xbd,
    0x3, 0x3b, 0xca, 0xbd,
    0x3, 0x40, 0x01, 0x01,
    0x4, 0x40, 0x04, 0xc0, 0x06,
    0x3, 0x40, 0x0c, 0x00,
    0x3, 0x40, 0x18, 0x7f,
    0x3, 0x40, 0x1a, 0x37,
    0x3, 0x40, 0x1c, 0x37,
    0x5, 0x40, 0x1e, 0xf7, 0x00, 0x3f,
    0x3, 0x40, 0x22, 0x6f,
    0x3, 0x40, 0x24, 0x3f,
    0x3, 0x40, 0x26, 0x5f,
    0x3, 0x40, 0x28, 0x2f,
    0x3, 0x40, 0x74, 0x01,
    0x3, 0x30, 0x02, 0x00,
    0x3, 0xff, 0xfe, 0x0a,
    0x55,
};
#else

/*
 * Xclk 27Mhz
 * 90.059fps
 * CSI-2_2lane
 * AD:10bit Output:12bit
 * 2376Mbps
 * Master Mode
 * Time 9.999ms Gain:6dB
 * 2568x1440 2/2-line binning & Window cropping
 */
static const imx415_sensor_reg g_imx415_1284X720_binning_crop_90fps_reg_table[] =
{
    {0x3008, 0x5D}
    {0x300A, 0x42}
    {0x301C, 0x04}
    {0x3020, 0x01}
    {0x3021, 0x01}
    {0x3022, 0x01}
    {0x3024, 0xAB}
    {0x3025, 0x07}
    {0x3028, 0xA4}
    {0x3029, 0x01}
    {0x3031, 0x00}
    {0x3033, 0x00}
    {0x3040, 0x88}
    {0x3041, 0x02}
    {0x3042, 0x08}
    {0x3043, 0x0A}
    {0x3044, 0xF0}
    {0x3045, 0x02}
    {0x3046, 0x40}
    {0x3047, 0x0B}
    {0x3050, 0xC4}
    {0x3090, 0x14}
    {0x30C1, 0x00}
    {0x30D9, 0x02}
    {0x30DA, 0x01}
    {0x3116, 0x23}
    {0x3118, 0x08}
    {0x3119, 0x01}
    {0x311A, 0xE7}
    {0x311E, 0x23}
    {0x32D4, 0x21}
    {0x32EC, 0xA1}
    {0x344C, 0x2B}
    {0x344D, 0x01}
    {0x344E, 0xED}
    {0x344F, 0x01}
    {0x3450, 0xF6}
    {0x3451, 0x02}
    {0x3452, 0x7F}
    {0x3453, 0x03}
    {0x358A, 0x04}
    {0x35A1, 0x02}
    {0x35EC, 0x27}
    {0x35EE, 0x8D}
    {0x35F0, 0x8D}
    {0x35F2, 0x29}
    {0x36BC, 0x0C}
    {0x36CC, 0x53}
    {0x36CD, 0x00}
    {0x36CE, 0x3C}
    {0x36D0, 0x8C}
    {0x36D1, 0x00}
    {0x36D2, 0x71}
    {0x36D4, 0x3C}
    {0x36D6, 0x53}
    {0x36D7, 0x00}
    {0x36D8, 0x71}
    {0x36DA, 0x8C}
    {0x36DB, 0x00}
    {0x3701, 0x00}
    {0x3720, 0x00}
    {0x3724, 0x02}
    {0x3726, 0x02}
    {0x3732, 0x02}
    {0x3734, 0x03}
    {0x3736, 0x03}
    {0x3742, 0x03}
    {0x3862, 0xE0}
    {0x38CC, 0x30}
    {0x38CD, 0x2F}
    {0x395C, 0x0C}
    {0x39A4, 0x07}
    {0x39A8, 0x32}
    {0x39AA, 0x32}
    {0x39AC, 0x32}
    {0x39AE, 0x32}
    {0x39B0, 0x32}
    {0x39B2, 0x2F}
    {0x39B4, 0x2D}
    {0x39B6, 0x28}
    {0x39B8, 0x30}
    {0x39BA, 0x30}
    {0x39BC, 0x30}
    {0x39BE, 0x30}
    {0x39C0, 0x30}
    {0x39C2, 0x2E}
    {0x39C4, 0x2B}
    {0x39C6, 0x25}
    {0x3A42, 0xD1}
    {0x3A4C, 0x77}
    {0x3AE0, 0x02}
    {0x3AEC, 0x0C}
    {0x3B00, 0x2E}
    {0x3B06, 0x29}
    {0x3B98, 0x25}
    {0x3B99, 0x21}
    {0x3B9B, 0x13}
    {0x3B9C, 0x13}
    {0x3B9D, 0x13}
    {0x3B9E, 0x13}
    {0x3BA1, 0x00}
    {0x3BA2, 0x06}
    {0x3BA3, 0x0B}
    {0x3BA4, 0x10}
    {0x3BA5, 0x14}
    {0x3BA6, 0x18}
    {0x3BA7, 0x1A}
    {0x3BA8, 0x1A}
    {0x3BA9, 0x1A}
    {0x3BAC, 0xED}
    {0x3BAD, 0x01}
    {0x3BAE, 0xF6}
    {0x3BAF, 0x02}
    {0x3BB0, 0xA2}
    {0x3BB1, 0x03}
    {0x3BB2, 0xE0}
    {0x3BB3, 0x03}
    {0x3BB4, 0xE0}
    {0x3BB5, 0x03}
    {0x3BB6, 0xE0}
    {0x3BB7, 0x03}
    {0x3BB8, 0xE0}
    {0x3BBA, 0xE0}
    {0x3BBC, 0xDA}
    {0x3BBE, 0x88}
    {0x3BC0, 0x44}
    {0x3BC2, 0x7B}
    {0x3BC4, 0xA2}
    {0x3BC8, 0xBD}
    {0x3BCA, 0xBD}
    {0x4001, 0x01}
    {0x4004, 0xC0}
    {0x4005, 0x06}
    {0x4018, 0xE7}
    {0x401A, 0x8F}
    {0x401C, 0x8F}
    {0x401E, 0x7F}
    {0x401F, 0x02}
    {0x4020, 0x97}
    {0x4022, 0x0F}
    {0x4023, 0x01}
    {0x4024, 0x97}
    {0x4026, 0xF7}
    {0x4028, 0x7F}
    {0x3002, 0x00}
    {REG_DELAY, 0x1E},//wait_ms(30)
    {REG_END, 0x00},
}

/*
 * Xclk 27Mhz
 * max_framerate 120fps
 * mipi_datarate per lane 371.25Mbps, 2lane
 * binning to  960x540 then centered crop
 */
static const struct imx415_sensor_reg g_imx415_3864X2192_15fps_reg_table[] =
{
    {0x3008, 0x5D},
    {0x300A, 0x42},
    {0x3028, 0x98},
    {0x3029, 0x08},
    {0x3033, 0x05},
    {0x3050, 0x79},
    {0x3051, 0x07},
    {0x3090, 0x14},
    {0x30C1, 0x00},
    {0x3116, 0x23},
    {0x3118, 0xC6},
    {0x311A, 0xE7},
    {0x311E, 0x23},
    {0x32D4, 0x21},
    {0x32EC, 0xA1},
    {0x344C, 0x2B},
    {0x344D, 0x01},
    {0x344E, 0xED},
    {0x344F, 0x01},
    {0x3450, 0xF6},
    {0x3451, 0x02},
    {0x3452, 0x7F},
    {0x3453, 0x03},
    {0x358A, 0x04},
    {0x35A1, 0x02},
    {0x35EC, 0x27},
    {0x35EE, 0x8D},
    {0x35F0, 0x8D},
    {0x35F2, 0x29},
    {0x36BC, 0x0C},
    {0x36CC, 0x53},
    {0x36CD, 0x00},
    {0x36CE, 0x3C},
    {0x36D0, 0x8C},
    {0x36D1, 0x00},
    {0x36D2, 0x71},
    {0x36D4, 0x3C},
    {0x36D6, 0x53},
    {0x36D7, 0x00},
    {0x36D8, 0x71},
    {0x36DA, 0x8C},
    {0x36DB, 0x00},
    {0x3720, 0x00},
    {0x3724, 0x02},
    {0x3726, 0x02},
    {0x3732, 0x02},
    {0x3734, 0x03},
    {0x3736, 0x03},
    {0x3742, 0x03},
    {0x3862, 0xE0},
    {0x38CC, 0x30},
    {0x38CD, 0x2F},
    {0x395C, 0x0C},
    {0x39A4, 0x07},
    {0x39A8, 0x32},
    {0x39AA, 0x32},
    {0x39AC, 0x32},
    {0x39AE, 0x32},
    {0x39B0, 0x32},
    {0x39B2, 0x2F},
    {0x39B4, 0x2D},
    {0x39B6, 0x28},
    {0x39B8, 0x30},
    {0x39BA, 0x30},
    {0x39BC, 0x30},
    {0x39BE, 0x30},
    {0x39C0, 0x30},
    {0x39C2, 0x2E},
    {0x39C4, 0x2B},
    {0x39C6, 0x25},
    {0x3A42, 0xD1},
    {0x3A4C, 0x77},
    {0x3AE0, 0x02},
    {0x3AEC, 0x0C},
    {0x3B00, 0x2E},
    {0x3B06, 0x29},
    {0x3B98, 0x25},
    {0x3B99, 0x21},
    {0x3B9B, 0x13},
    {0x3B9C, 0x13},
    {0x3B9D, 0x13},
    {0x3B9E, 0x13},
    {0x3BA1, 0x00},
    {0x3BA2, 0x06},
    {0x3BA3, 0x0B},
    {0x3BA4, 0x10},
    {0x3BA5, 0x14},
    {0x3BA6, 0x18},
    {0x3BA7, 0x1A},
    {0x3BA8, 0x1A},
    {0x3BA9, 0x1A},
    {0x3BAC, 0xED},
    {0x3BAD, 0x01},
    {0x3BAE, 0xF6},
    {0x3BAF, 0x02},
    {0x3BB0, 0xA2},
    {0x3BB1, 0x03},
    {0x3BB2, 0xE0},
    {0x3BB3, 0x03},
    {0x3BB4, 0xE0},
    {0x3BB5, 0x03},
    {0x3BB6, 0xE0},
    {0x3BB7, 0x03},
    {0x3BB8, 0xE0},
    {0x3BBA, 0xE0},
    {0x3BBC, 0xDA},
    {0x3BBE, 0x88},
    {0x3BC0, 0x44},
    {0x3BC2, 0x7B},
    {0x3BC4, 0xA2},
    {0x3BC8, 0xBD},
    {0x3BCA, 0xBD},
    {0x4001, 0x01},
    {0x4004, 0xC0},
    {0x4005, 0x06},
    {0x400C, 0x00},
    {0x4018, 0x7F},
    {0x401A, 0x37},
    {0x401C, 0x37},
    {0x401E, 0xF7},
    {0x401F, 0x00},
    {0x4020, 0x3F},
    {0x4022, 0x6F},
    {0x4024, 0x3F},
    {0x4026, 0x5F},
    {0x4028, 0x2F},
    {0x4074, 0x01},
    {0x3002, 0x00},
    //{0x3000, 0x00},
    {REG_DELAY, 0x1E},//wait_ms(30)
    {REG_END, 0x00},
};
#endif

static const struct imx415_mode supported_modes[] =
{
    [IMX415_1280X720] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR12_1X12,
        .width = 1284,
        .height = 720,
        .max_fps = {
            .numerator = 10000,
            .denominator = 900000,
        },
        .exp_def = 0x07AB - 8,
        .hts_def = 0x01A4 * IMX415_2LANES * 2,
        .vts_def = 0x07AB,
        .xvclk = 27000000,
        .link_freq = 1188000000,
        .reg_list = g_imx415_1284X720_binning_crop_90fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_imx415_1284X720_binning_crop_90fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [IMX415_3840X2160] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR12_1X12,
        .width = 3864,
        .height = 2192,
        .max_fps = {
            .numerator = 10000,
            .denominator = 150000,
        },
        .exp_def = 0x08ca - 0x08,
        .hts_def = 0x0898 * IMX415_2LANES * 2,
        .vts_def = 0x08ca,
        .xvclk = 27000000,
        .link_freq = 446000000,
        .reg_list = g_imx415_3864X2192_15fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_imx415_3864X2192_15fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct imx415_ops
{
    ret_err_t (*init)(struct imx415_dev *dev);
    ret_err_t (*open)(struct imx415_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct imx415_dev *dev);
    ret_err_t (*control)(struct imx415_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t imx415_read_reg(struct rt_i2c_bus_device *bus,
                                 uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    int retry = 10;
    ret_err_t ret = 0;

    RT_ASSERT(bus != RT_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    msg[0].addr = g_imx415.i2c_addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = g_imx415.i2c_addr;
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
            imx415_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
    return ret;
}

static ret_err_t imx415_write_reg(struct rt_i2c_bus_device *bus,
                                  uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;
    RT_ASSERT(bus != RT_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    send_buf[2] = data;

    msgs.addr = g_imx415.i2c_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = send_buf;
    msgs.len = 3;

    while (1)
    {
        ret = rt_i2c_transfer(bus, &msgs, 1);
        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
        else
        {
            imx415_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG

static int imx415_write_multiple_reg_continue(struct imx415_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msgs;
    int ret = 0;
    int offset = 0;
    int retry = 10;
    uint16_t addr;

    RT_ASSERT(dev != RT_NULL && i2c_data != RT_NULL);

    i2c_bus = dev->i2c_bus;
    RT_ASSERT(i2c_bus != RT_NULL);

    for (i = 0; i < i2c_data[len - 1];)
    {
        addr = (i2c_data[offset + 1] << 8) | i2c_data[offset + 2];
        if (addr != REG_DELAY)
        {
            msgs.addr = g_imx415.i2c_addr;
            msgs.flags = RT_I2C_WR;
            msgs.buf = (uint8_t *)&i2c_data[offset + 1];
            msgs.len = i2c_data[offset];
            ret = rt_i2c_transfer(i2c_bus, &msgs, 1);

            if (ret != 1)
            {
                imx415_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
                if (--retry)
                    continue;
                else
                    return RET_SYS_ERROR;
            }
        }
        else
        {
            imx415_info("delay %d ms\n", i2c_data[offset + 3]);
            HAL_DelayUs(i2c_data[offset + 3] * 1000);
        }

        offset += (i2c_data[offset] + 1);
        retry = 10;
        i++;
    }

    return RET_SYS_EOK;
}

#else
static ret_err_t imx415_write_reg_discontinued(struct rt_i2c_bus_device *bus,
        char *data, uint32_t len)
{
    struct rt_i2c_msg msgs;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(bus != RT_NULL);

    msgs.addr = g_imx415.i2c_addr;
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
            imx415_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}


static void imx415_write_multiple_reg(struct imx415_dev *dev,
                                      const struct imx415_sensor_reg *reg, int len)
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
                imx415_write_reg_discontinued(i2c_bus, data, k + 2);
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
                imx415_write_reg_discontinued(i2c_bus, data, k + 2);
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
                }
                else
                {
                    cnt++;
                    rt_kprintf("0x%x, ", k + 2);
                    for (j = 0; j < k + 2; j++)
                        rt_kprintf("0x%02x, ", data[j]);
                    rt_kprintf("\n");
                    imx415_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct imx415_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        imx415_info("id: %d, width: %d, height: %d\n",
                    id, supported_modes[id].width, supported_modes[id].height);
        imx415_write_multiple_reg((struct imx415_dev *)dev,
                                  supported_modes[id].reg_list,
                                  supported_modes[id].reg_list_size);
    }
}
#endif

static ret_err_t rk_imx415_set_expval(struct imx415_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    int cur_vts = dev->cur_mode->vts_def;
    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming && strcmp(dev->name, RK_ISP_ASSIST_SUBDEV_NAME))
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        imx415_dbg("imx415 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }
    l_exp_time = cur_vts - exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];
    imx415_dbg("imx415: exp req: L_exp: 0x%x, 0x%x\n",
               l_exp_time, l_a_gain);

    ret = imx415_write_reg(dev->i2c_bus, IMX415_LF_EXPO_REG_H,
                           (uint8_t)((l_exp_time >> 16) & 0x0f));
    ret |= imx415_write_reg(dev->i2c_bus, IMX415_LF_EXPO_REG_M,
                            (uint8_t)((l_exp_time >> 8) & 0xff));
    ret |= imx415_write_reg(dev->i2c_bus, IMX415_LF_EXPO_REG_L,
                            (uint8_t)(l_exp_time & 0xff));
    ret = imx415_write_reg(dev->i2c_bus, IMX415_LF_GAIN_REG_H, (uint8_t)((l_a_gain >> 8) & 0x7));
    ret |= imx415_write_reg(dev->i2c_bus, IMX415_LF_GAIN_REG_L, (uint8_t)(l_a_gain & 0xff));
    rt_mutex_release(&dev->mutex_lock);
    return ret;
}

static ret_err_t rk_imx415_set_vts(struct imx415_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    imx415_dbg("(%s) set vts: 0x%x \n", __FUNCTION__, dst_vts);

    ret = imx415_write_reg(dev->i2c_bus, IMX415_VTS_REG_L,
                           (uint8_t)(dst_vts & 0xff));
    ret |= imx415_write_reg(dev->i2c_bus, IMX415_VTS_REG_M,
                            (uint8_t)((dst_vts >> 8) & 0xff));
    ret |= imx415_write_reg(dev->i2c_bus, IMX415_VTS_REG_H,
                            (uint8_t)((dst_vts >> 18)) & 0x0f);

    return ret;
}

static ret_err_t rk_imx415_set_flip_mirror(struct imx415_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = imx415_read_reg(dev->i2c_bus, IMX415_FLIP_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val |= IMX415_FLIP_BIT_MASK;
        val |= IMX415_MIRROR_BIT_MASK;
        break;
    default:
        val &= ~IMX415_FLIP_BIT_MASK;
        val &= ~IMX415_MIRROR_BIT_MASK;
        break;
    };
    imx415_dbg("(%s) flip 0x%x, reg val 0x%x\n", __FUNCTION__, flip, val);
    ret |= imx415_write_reg(dev->i2c_bus, IMX415_FLIP_REG, val);

    return ret;
}

static void imx415_stream_on(struct imx415_dev *dev)
{

    imx415_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
    imx415_write_multiple_reg_continue((struct imx415_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_imx415_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_imx415_set_flip_mirror(dev, dev->flip);

    imx415_write_reg(dev->i2c_bus, IMX415_REG_CTRL_MODE, IMX415_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    imx415_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void imx415_stream_on_late(struct imx415_dev *dev)
{

    imx415_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    HAL_DelayMs(5);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_imx415_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_imx415_set_flip_mirror(dev, dev->flip);

    imx415_write_reg(dev->i2c_bus, IMX415_REG_CTRL_MODE, IMX415_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    imx415_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void imx415_stream_off(struct imx415_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        imx415_write_reg(dev->i2c_bus, IMX415_REG_CTRL_MODE, IMX415_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        imx415_err("not find out i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_imx415_init(struct rk_camera_device *dev)
{

    ret_err_t ret = RET_SYS_EOK;
    struct imx415_dev *imx415;

    imx415 = (struct imx415_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&imx415->parent;
#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(imx415);
#endif

    if (imx415)
    {
        camera->info.mbus_fmt.width = imx415->cur_mode->width;
        camera->info.mbus_fmt.height = imx415->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = imx415->cur_mode->bus_fmt;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = imx415->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode = imx415->cur_mode->hdr_mode;

        imx415->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(imx415->i2c_name);

        if (!imx415->i2c_bus)
        {
            imx415_err("not find i2c source 2:%s !!!\n",
                       imx415->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            imx415_dbg("s0 find i2c_bus:%s\n", imx415->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;
}

static ret_err_t rk_imx415_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_imx415_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

static ret_err_t rk_imx415_get_expinf(struct imx415_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct imx415_mode *mode;

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

static ret_err_t rk_imx415_get_intput_fmt(struct imx415_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct imx415_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = mode->bus_fmt;

    return ret;
}

static ret_err_t rk_imx415_set_intput_fmt(struct imx415_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct imx415_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    imx415_info("dst resulotion, width %d, height %d\n",
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

    if (is_find_fmt == false && mbus_fmt->width == 1280 && mbus_fmt->height == 720)
    {
        mode = &supported_modes[IMX415_1280X720];
        is_find_fmt = true;
        imx415_info("dst mode IMX415_1280X720\n");
    }
    else if (is_find_fmt == false && mbus_fmt->width == 3840 && mbus_fmt->height == 2160)
    {
        mode = &supported_modes[IMX415_3840X2160];
        is_find_fmt = true;
        imx415_info("dst mode IMX415_3840X2160\n");
    }
    if (is_find_fmt)
    {
        if (mode->width != dev->cur_mode->width)
        {
            dev->cur_mode = mode;
            rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

#ifndef REG_LIST_CONVERSION_DEBUG
            struct camera_board_desc *imx415 = (struct camera_board_desc *)&camera_imx415;

            if (imx415->rst_gpio.gpio_group)
            {
                HAL_GPIO_SetPinDirection((struct GPIO_REG *)imx415->rst_gpio.gpio_group, imx415->rst_gpio.gpio_pin, GPIO_OUT);
                /* Set XCLR low At least 500ns after power raising */
                HAL_GPIO_SetPinLevel((struct GPIO_REG *)imx415->rst_gpio.gpio_group, imx415->rst_gpio.gpio_pin, !imx415->rst_active);
                HAL_DelayMs(5);
                imx415_info("set to reset, tick:%u\n", rt_tick_get());
                /* At least 1us between XCLR and clk */
                HAL_GPIO_SetPinLevel((struct GPIO_REG *)imx415->rst_gpio.gpio_group, imx415->rst_gpio.gpio_pin, imx415->rst_active);
                HAL_DelayMs(5);
            }

            imx415_write_multiple_reg_continue((struct imx415_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_imx415_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_imx415_match_dst_config(struct imx415_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct imx415_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;
    imx415_info("cur resulotion, width %d, height %d, fps %d!\n",
                dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct imx415_mode *)mode;
            ret = RET_SYS_EOK;
            imx415_info("find dst resulotion, width %d, height %d, fps %d!\n",
                        dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            imx415_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        imx415_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }
    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_imx415_control(struct rk_camera_device *dev,
                            int cmd,
                            void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct imx415_dev *imx415;

    RT_ASSERT(dev != RT_NULL);
    imx415 = (struct imx415_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_imx415_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        imx415_stream_on(imx415);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        imx415_stream_off(imx415);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_imx415_get_expinf(imx415, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_imx415_set_expval(imx415, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_imx415_set_vts(imx415, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_imx415_get_intput_fmt(imx415, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_imx415_set_intput_fmt(imx415, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_imx415_match_dst_config(imx415, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_imx415_set_flip_mirror(imx415, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        imx415_stream_on_late(imx415);
    }
    break;
    default:
        imx415_dbg("(%s) exit CMD %d\n", __FUNCTION__, cmd);
        break;
    }
    return ret;
}

struct rk_camera_ops rk_imx415_ops =
{
    rk_imx415_init,
    rk_imx415_open,
    NULL,
    rk_imx415_control
};


int rk_camera_imx415_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    struct imx415_dev *instance = &g_imx415;
    struct rk_camera_device *camera = &instance->parent;
    struct clk_gate *clkgate;
    struct camera_board_desc *imx415 = (struct camera_board_desc *)&camera_imx415;
    camera->ops = &rk_imx415_ops;

    rt_strncpy(instance->name, imx415->isp_subdev_name, rt_strlen(imx415->isp_subdev_name));
    rt_strncpy(instance->i2c_name, imx415->i2c_bus, rt_strlen(imx415->i2c_bus));

    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (imx415->mode_id <= IMX415_MODE_ID_MAX)
    {
        imx415_dbg("mode_id: %d\n", imx415->mode_id);
        instance->cur_mode = &supported_modes[imx415->mode_id];
    }
    else
    {
        imx415_info("mode id is over range, default use mode_id: 0\n");
        instance->cur_mode = &supported_modes[0];
    }
    instance->dst_mode = (struct imx415_mode *)&supported_modes[IMX415_3840X2160];
    instance->time_valid_delay = imx415->time_valid_delay;
    instance->gain_valid_delay = imx415->gain_valid_delay;
    instance->i2c_addr = imx415->i2c_addr;
    instance->flip = 0;

    if (imx415->rst_gpio.gpio_group)
    {
        HAL_GPIO_SetPinDirection((struct GPIO_REG *)imx415->rst_gpio.gpio_group, imx415->rst_gpio.gpio_pin, GPIO_OUT);
        /* Set XCLR low At least 500ns after power raising */
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)imx415->rst_gpio.gpio_group, imx415->rst_gpio.gpio_pin, !imx415->rst_active);
        HAL_DelayMs(5);
        /* At least 1us between XCLR and clk */
        HAL_GPIO_SetPinLevel((struct GPIO_REG *)imx415->rst_gpio.gpio_group, imx415->rst_gpio.gpio_pin, imx415->rst_active);
        HAL_DelayMs(5);
    }

    clk_set_rate(imx415->mclk_id, instance->cur_mode->xvclk);
    clkgate = get_clk_gate_from_id(imx415->mclk_id);
    clk_enable(clkgate);

    rt_mutex_init(&instance->mutex_lock, "IMX415_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, imx415->isp_subdev_name, rt_strlen(imx415->isp_subdev_name));
    rk_camera_register(camera, camera->name, instance);
    return ret;
}

void imx415_detect(void)
{
    struct imx415_dev *instance = &g_imx415;

    imx415_dbg("start to detect IMX415 for testing \n");
    imx415_dbg("dev name:%s\n", instance->name);
    imx415_dbg("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        imx415_err("Warning:not find i2c source 1:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        imx415_dbg("find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_imx415_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(imx415_detect, check IMX415 is available or not);
#endif
#endif
#endif
