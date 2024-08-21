/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_os02k10.c
  * @version V0.0.1
  *
  * Change Logs:
  * 2023-06-15     Honglin Lan      first implementation
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

#ifdef RT_USING_OS02K10

//#define REG_LIST_CONVERSION_DEBUG
#define OS02K10_DEBUG_PRT      0

#if OS02K10_DEBUG_PRT
#include <stdio.h>
#define os02k10_dbg(fmt, args...)               rt_kprintf("[OS02K10] "fmt"", ##args)
#else
#define os02k10_dbg(fmt, args...)
#endif

#define os02k10_info(fmt, args...)              rt_kprintf("[OS02K10] "fmt"", ##args)
#define os02k10_err(fmt, args...)               rt_kprintf("[OS02K10] ERR "fmt"", ##args)

#define OS02K10_FETCH_2ND_BYTE_EXP(VAL)                         (((VAL) >> 8) & 0xFF)
#define OS02K10_FETCH_1ST_BYTE_EXP(VAL)                         ((VAL) & 0xFF)

#define OS02K10_AEC_LONG_EXP_REG_H                      0x3501 //bit[7:0] to long exposure[15:8]
#define OS02K10_AEC_LONG_EXP_REG_L                      0x3502 //bit[7:0] to long exposure[7:0]
#define OS02K10_AEC_LONG_REL_GAIN_REG_H                 0x3508 //bit[3:0] to long real gain[7:4]
#define OS02K10_AEC_LONG_REL_GAIN_REG_L                 0x3509 //bit[7:4] to long real gain[3:0]
#define OS02K10_AEC_LONG_DIG_GAIN_REG_H                 0x350a //bit[3:0] to long digital gain[13:10]
#define OS02K10_AEC_LONG_DIG_GAIN_REG_M                 0x350b //bit[7:0] to long digital gain[9:2]
#define OS02K10_AEC_LONG_DIG_GAIN_REG_L                 0x350c //bit[7:6] to long digital gain[1:0]

//#define OS02K10_AEC_SHORT_EXP_REG_H                   0x3541 //bit[7:0] to short exposure[15:8]
//#define OS02K10_AEC_SHORT_EXP_REG_L                   0x3542 //bit[7:0] to short exposure[7:0]
#define OS02K10_AEC_SHORT_REL_GAIN_REG_H                0x3548 //bit[3:0] to short real gain[7:4]
#define OS02K10_AEC_SHORT_REL_GAIN_REG_L                0x3549 //bit[7:4] to short real gain[3:0]
#define OS02K10_AEC_SHORT_DIG_GAIN_REG_H                0x354a //bit[3:0] to short digital gain[13:10]
#define OS02K10_AEC_SHORT_DIG_GAIN_REG_M                0x354b //bit[7:0] to short digital gain[9:2]
#define OS02K10_AEC_SHORT_DIG_GAIN_REG_L                0x354c //bit[7:6] to short digital gain[1:0]

#define OS02K10_AEC_VERY_SHORT_EXP_REG_H                0x3581 //bit[7:0] to very short exposure[15:8]
#define OS02K10_AEC_VERY_SHORT_EXP_REG_L                0x3582 //bit[7:0] to very short exposure[7:0]
#define OS02K10_AEC_VERY_SHORT_REL_GAIN_REG_H           0x3588 //bit[3:0] to very short real gain[7:4]
#define OS02K10_AEC_VERY_SHORT_REL_GAIN_REG_L           0x3589 //bit[7:4] to very short real gain[3:0]
#define OS02K10_AEC_VERY_SHORT_DIG_GAIN_REG_H           0x358a //bit[3:0] to very short digital gain[13:10]
#define OS02K10_AEC_VERY_SHORT_DIG_GAIN_REG_M           0x358b //bit[7:0] to very short digital gain[9:2]
#define OS02K10_AEC_VERY_SHORT_DIG_GAIN_REG_L           0x358c //bit[7:6] to very short digital gain[1:0]

#define OS02K10_AEC_TIMING_HTS_REG_H                    0x380c //bit[7:0] to horizontal timing[15:8] for log and short exposure
#define OS02K10_AEC_TIMING_HTS_REG_L                    0x380d //bit[7:0] to horizontal timing[7:0] for log and short exposure
#define OS02K10_AEC_TIMING_VTS_REG_H                    0x380e //bit[7:0] to vertical timing[15:8] for log and short exposure
#define OS02K10_AEC_TIMING_VTS_REG_L                    0x380f //bit[7:0] to vertical timing[7:0] for log and short exposure
#define OS02K10_AEC_TIMING_VERY_HTS_REG_H               0x384c //bit[7:0] to horizontal timing[15:8] for very short exposure
#define OS02K10_AEC_TIMING_VERY_HTS_REG_L               0x384d //bit[7:0] to horizontal timing[7:0] for very short exposure

#define OS02K10_HORIZONTAL_START_REG_H                  0x3800 //bit[7:0] to array horizontal start[15:8]
#define OS02K10_HORIZONTAL_START_REG_L                  0x3801 //bit[7:0] to array horizontal start[7:0]
#define OS02K10_VERTICAL_START_REG_H                    0x3802 //bit[7:0] to array vertical start[15:8]
#define OS02K10_VERTICAL_START_REG_L                    0x3803 //bit[7:0] to array vertical start[15:8]
#define OS02K10_HORIZONTAL_END_REG_H                    0x3804 //bit[7:0] to array horizontal end[15:8]
#define OS02K10_HORIZONTAL_END_REG_L                    0x3805 //bit[7:0] to array horizontal end[15:8]
#define OS02K10_VERTICAL_END_REG_H                      0x3806 //bit[7:0] to array vertical end[15:8]
#define OS02K10_VERTICAL_END_REG_L                      0x3807 //bit[7:0] to array vertical end[15:8]
#define OS02K10_HORIZONTAL_OUTPUT_SIZE_REG_H            0x3808 //bit[7:0] to array horizontal output size for final image[15:8]
#define OS02K10_HORIZONTAL_OUTPUT_SIZE_REG_L            0x3809 //bit[7:0] to array horizontal output size for final image[7:0]
#define OS02K10_VERTICAL_OUTPUT_SIZE_REG_H              0x380a //bit[7:0] to array vertical output size for final image[15:8]
#define OS02K10_VERTICAL_OUTPUT_SIZE_REG_L              0x380b //bit[7:0] to array vertical output size for final image[7:0]

#define OS02K10_WINDOW_HORIZONTAL_TRUNC_SIZE_REG_H      0x3810 //bit[7:0] to isp window horizontal truncation size[15:8]
#define OS02K10_WINDOW_HORIZONTAL_TRUNC_SIZE_REG_L      0x3811 //bit[7:0] to isp window horizontal truncation size[7:0]
#define OS02K10_WINDOW_VERTICAL_TRUNC_SIZE_REG_H        0x3812 //bit[7:0] to isp window vertical truncation size[15:8]
#define OS02K10_WINDOW_VERTICAL_TRUNC_SIZE_REG_L        0x3813 //bit[7:0] to isp window vertical truncation size[15:8]

#define OS02K10_FLIPMIRROR_REG                          0x3820
#define OS02K10_FLIPMIRROR_REGBIT_MIRROR                0x02   //bit[1] = 2'b1, mirror
#define OS02K10_FLIPMIRROR_REGBIT_FLIP                  0x0c   //bit[3:2] = 2'b11, flip
#define OS02K10_FLIPMIRROR_REGBIT_MIRROR_FLIP           0x0e   //bit[3:1] = 2'b111, mirror and flip
#define OS02K10_FLIPMIRROR_STATE_NORMAL                 0
#define OS02K10_FLIPMIRROR_STATE_FLIPMIRROR             1
#define OS02K10_FLIPMIRROR_STATE_MIRROR                 2
#define OS02K10_FLIPMIRROR_STATE_FLIP                   3

#define OS02K10_FINE_INTG_TIME_MIN                      0
#define OS02K10_FINE_INTG_TIME_MAX_MARGIN               0
#define OS02K10_COARSE_INTG_TIME_MIN                    16
#define OS02K10_COARSE_INTG_TIME_MAX_MARGIN             8

#define OS02K10_AGAIN_REAL_MIN                          0x10
#define OS02K10_AGAIN_REAL_MAX                          0xf8
#define OS02K10_REAL_GAIN                               1
#define OS02K10_SENSOR_GAIN                             0

#define OS02K10_GAIN_MIN                1000
#define OS02K10_GAIN_MAX                1722628       //108.512*15.875*1000
#define OS02K10_GAIN_STEP               1
#define OS02K10_GAIN_DEFAULT            1000
#define OS02K10_LGAIN                   0
#define OS02K10_SGAIN                   1

#define OS02K10_FETCH_MIRROR(VAL, ENABLE)        (ENABLE ? VAL | 0x02 : VAL & 0xfd)
#define OS02K10_FETCH_FLIP(VAL, ENABLE)          (ENABLE ? VAL | 0x0c : VAL & 0xf3)
#define OS02K10_REG_STREAM_CTRL         0x0100
#define OS02K10_STREAM_ON               0x01
#define OS02K10_STREAM_OFF              0x00

#define REG_END                     (0x0)
#define REG_DELAY                   (0xfffe)

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

struct os02k10_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
} __attribute__((__packed__));

struct os02k10_mode
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
    const struct os02k10_sensor_reg *reg_list;
#endif
    const int reg_list_size;
    uint8_t hdr_mode;
} __attribute__((__packed__));

#pragma pack(1)
struct os02k10_dev
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
    const struct os02k10_mode *cur_mode;
    struct os02k10_mode *dst_mode;

    bool has_init_exp;
    bool streaming;
};
#pragma pack()

typedef struct os02k10_dev *rt_os02k10_dev_t;

//#define REG_LIST_CONVERSION_DEBUG
#ifndef REG_LIST_CONVERSION_DEBUG
static const uint8_t g_os02k10_640x480_120fps_reg_table[] =
{
    0x3,  0x01,  0x03,  0x01,
    0x3,  0xFF,  0xFE,  0x0a,
    0x3,  0x01,  0x03,  0x00,
    0x3,  0x01,  0x09,  0x01,
    0x3,  0x01,  0x04,  0x02,
    0x3,  0x01,  0x02,  0x00,
    0x5,  0x03,  0x05,  0x5c,  0x00,  0x00,
    0x3,  0x03,  0x0a,  0x01,
    0x3,  0x03,  0x17,  0x09,
    0x5,  0x03,  0x23,  0x07,  0x01,  0xb0,
    0x3,  0x03,  0x27,  0x07,
    0x5,  0x03,  0x2c,  0x02,  0x02,  0x05,
    0x3,  0x30,  0x0f,  0x11,
    0x3,  0x30,  0x12,  0x21,
    0x4,  0x30,  0x26,  0x10,  0x08,
    0x3,  0x30,  0x2d,  0x24,
    0x3,  0x31,  0x03,  0x29,
    0x3,  0x31,  0x06,  0x10,
    0x3,  0x34,  0x00,  0x00,
    0x3,  0x34,  0x06,  0x08,
    0x3,  0x34,  0x08,  0x05,
    0x3,  0x34,  0x0c,  0x05,
    0x9,  0x34,  0x25,  0x51,  0x10,  0x14,  0x10,  0x10,  0x10,  0x04,
    0x3,  0x35,  0x04,  0x08,
    0x4,  0x35,  0x08,  0x01,  0x00,
    0x3,  0x35,  0x44,  0x08,
    0x4,  0x35,  0x48,  0x01,  0x00,
    0x3,  0x35,  0x84,  0x08,
    0x4,  0x35,  0x88,  0x01,  0x00,
    0x3,  0x36,  0x01,  0x70,
    0x5,  0x36,  0x04,  0xe3,  0x7f,  0x00,
    0x3,  0x36,  0x08,  0xa8,
    0x4,  0x36,  0x0a,  0xd0,  0x08,
    0x8,  0x36,  0x0e,  0xc8,  0x66,  0x81,  0x89,  0x4e,  0xbd,
    0xa,  0x36,  0x2a,  0x0e,  0x0e,  0x0e,  0x0e,  0x0c,  0x1a,  0x32,  0x64,
    0x3,  0x36,  0x38,  0x00,
    0xa,  0x36,  0x43,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,
    0x8,  0x36,  0x4c,  0x0e,  0x0e,  0x0e,  0x0e,  0xff,  0xff,
    0x5,  0x36,  0x61,  0x07,  0x02,  0x20,
    0x3,  0x36,  0x65,  0x12,
    0x4,  0x36,  0x67,  0xd4,  0x80,
    0x5,  0x36,  0x6f,  0x00,  0xc7,  0x08,
    0x3,  0x36,  0x73,  0x2a,
    0x3,  0x36,  0x81,  0x80,
    0x6,  0x37,  0x00,  0x26,  0x1e,  0x25,  0x28,
    0x8,  0x37,  0x06,  0x3e,  0x0a,  0x36,  0x55,  0x00,  0xa3,
    0x3,  0x37,  0x14,  0x03,
    0x5,  0x37,  0x1b,  0x16,  0x00,  0x08,
    0x4,  0x37,  0x56,  0x9b,  0x9b,
    0x3,  0x37,  0x62,  0x1d,
    0x3,  0x37,  0x6c,  0x10,
    0x4,  0x37,  0x76,  0x05,  0x22,
    0x3,  0x37,  0x79,  0x60,
    0x3,  0x37,  0x7c,  0x48,
    0x5,  0x37,  0x83,  0x02,  0x06,  0x0a,
    0x3,  0x37,  0x90,  0x10,
    0x4,  0x37,  0x93,  0x04,  0x07,
    0x4,  0x37,  0x96,  0x00,  0x02,
    0x3,  0x37,  0x9c,  0x4d,
    0x3,  0x37,  0xa1,  0x80,
    0x3,  0x37,  0xbb,  0x88,
    0x6,  0x37,  0xbd,  0x01,  0x01,  0x00,  0x01,
    0x3,  0x37,  0xc7,  0x56,
    0x3,  0x37,  0xca,  0x21,
    0x4,  0x37,  0xcc,  0x13,  0x90,
    0x3,  0x37,  0xcf,  0x04,
    0x5,  0x37,  0xd1,  0x3e,  0x00,  0xa3,
    0x6,  0x37,  0xd5,  0x3e,  0x00,  0xa3,  0x01,
    0x6,  0x37,  0xda,  0x00,  0x00,  0x00,  0x00,
    0x12,  0x38,  0x00,  0x00,  0x00,  0x00,  0x04,  0x07,  0x8f,  0x04,  0x43,  0x07,  0x80,  0x04,  0x38,  0x02,  0xd0,  0x04,  0xe2,
    0x3,  0x38,  0x11,  0x08,
    0x7,  0x38,  0x13,  0x04,  0x03,  0x01,  0x03,  0x01,
    0x3,  0x38,  0x1c,  0x00,
    0x5,  0x38,  0x20,  0x02,  0x09,  0x14,
    0x3,  0x38,  0x33,  0x41,
    0x4,  0x38,  0x4c,  0x02,  0xd0,
    0x3,  0x38,  0x58,  0x0d,
    0x6,  0x38,  0x65,  0x00,  0xc0,  0x00,  0xc0,
    0x3,  0x39,  0x00,  0x13,
    0x3,  0x39,  0x40,  0x13,
    0x3,  0x39,  0x80,  0x13,
    0x8,  0x39,  0x0c,  0x03,  0x02,  0x01,  0x03,  0x02,  0x01,
    0x8,  0x39,  0x4c,  0x02,  0x02,  0x01,  0x02,  0x02,  0x01,
    0x8,  0x39,  0x8c,  0x02,  0x01,  0x01,  0x02,  0x01,  0x01,
    0x3,  0x53,  0x95,  0x38,
    0x3,  0x53,  0x92,  0x14,
    0x8,  0x53,  0x96,  0x02,  0x01,  0x01,  0x02,  0x01,  0x01,
    0x3,  0x54,  0x15,  0x38,
    0x3,  0x54,  0x12,  0x14,
    0x8,  0x54,  0x16,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,
    0x3,  0x54,  0x95,  0x38,
    0x3,  0x54,  0x92,  0x14,
    0x8,  0x54,  0x96,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,
    0x3,  0x3c,  0x01,  0x11,
    0x3,  0x3c,  0x05,  0x00,
    0x3,  0x3c,  0x0f,  0x1c,
    0x3,  0x3c,  0x12,  0x0d,
    0x3,  0x3c,  0x14,  0x21,
    0x3,  0x3c,  0x19,  0x01,
    0x3,  0x3c,  0x21,  0x40,
    0x3,  0x3c,  0x3b,  0x18,
    0x3,  0x3c,  0x3d,  0xc9,
    0x3,  0x3c,  0x55,  0x08,
    0x4,  0x3c,  0x5d,  0xcf,  0xcf,
    0x6,  0x3c,  0xe0,  0x00,  0x00,  0x00,  0x00,
    0x4,  0x3d,  0x8c,  0x70,  0x10,
    0x3,  0x40,  0x01,  0x2f,
    0x3,  0x40,  0x33,  0x80,
    0x4,  0x40,  0x08,  0x02,  0x07,
    0x4,  0x40,  0x04,  0x00,  0x40,
    0x4,  0x40,  0x0a,  0x01,  0x3c,
    0x3,  0x40,  0x0e,  0x40,
    0x3,  0x40,  0x11,  0xbb,
    0x3,  0x41,  0x0f,  0x01,
    0x6,  0x40,  0x28,  0x6f,  0x0f,  0x3f,  0x01,
    0x7,  0x40,  0x2e,  0x00,  0x40,  0x00,  0x40,  0x2f,
    0x4,  0x40,  0x50,  0x00,  0x03,
    0x5,  0x42,  0x88,  0xcf,  0x03,  0x46,
    0x6,  0x43,  0x0b,  0x0f,  0xfc,  0x00,  0x00,
    0x3,  0x43,  0x14,  0x04,
    0x4,  0x45,  0x00,  0x1a,  0x18,
    0x3,  0x45,  0x04,  0x00,
    0x4,  0x45,  0x07,  0x02,  0x1a,
    0x3,  0x46,  0x03,  0x00,
    0x3,  0x46,  0x40,  0x62,
    0x6,  0x46,  0x46,  0xaa,  0x55,  0x99,  0x66,
    0x3,  0x46,  0x4d,  0x00,
    0x4,  0x46,  0x54,  0x11,  0x22,
    0x3,  0x48,  0x00,  0x04,
    0x4,  0x48,  0x10,  0xff,  0xff,
    0x3,  0x48,  0x0e,  0x00,
    0x3,  0x48,  0x13,  0x00,
    0x3,  0x48,  0x37,  0x0e,
    0x3,  0x48,  0x4b,  0x27,
    0x8,  0x4d,  0x00,  0x4e,  0x0c,  0xb8,  0xea,  0x74,  0xb7,
    0x3,  0x4d,  0x09,  0x4f,
    0x3,  0x50,  0x00,  0x1f,
    0x3,  0x50,  0x80,  0x00,
    0x3,  0x50,  0xc0,  0x00,
    0x3,  0x51,  0x00,  0x00,
    0x6,  0x52,  0x00,  0x00,  0x70,  0x03,  0x7f,
    0x3,  0x57,  0x80,  0x53,
    0x3,  0x57,  0x86,  0x01,
    0x3,  0x35,  0x01,  0x02,
    0xe,  0x38,  0x00,  0x00,  0x00,  0x00,  0x00,  0x07,  0x8f,  0x04,  0x47,  0x02,  0x80,  0x01,  0xe0,
    0x3,  0x38,  0x11,  0xa0,
    0x3,  0x38,  0x13,  0x1e,
    0x3,  0x03,  0x05,  0x50,
    0x3,  0x48,  0x37,  0x10,
    0x6,  0x38,  0x0c,  0x05,  0xa0,  0x02,  0x71,
    0x4,  0x35,  0x01,  0x02,  0x69,
    0x97,
};

static const uint8_t g_os02k10_1920x1080_30fps_reg_table[] =
{
    0x3,  0x01,  0x03,  0x01,
    0x3,  0xFF,  0xFE,  0x0a,
    0x3,  0x01,  0x03,  0x00,
    0x3,  0x01,  0x09,  0x01,
    0x3,  0x01,  0x04,  0x02,
    0x3,  0x01,  0x02,  0x00,
    0x5,  0x03,  0x05,  0x5c,  0x00,  0x00,
    0x3,  0x03,  0x0a,  0x01,
    0x3,  0x03,  0x17,  0x09,
    0x5,  0x03,  0x23,  0x07,  0x01,  0xb0,
    0x3,  0x03,  0x27,  0x07,
    0x5,  0x03,  0x2c,  0x02,  0x02,  0x05,
    0x3,  0x30,  0x0f,  0x11,
    0x3,  0x30,  0x12,  0x21,
    0x4,  0x30,  0x26,  0x10,  0x08,
    0x3,  0x30,  0x2d,  0x24,
    0x3,  0x31,  0x03,  0x29,
    0x3,  0x31,  0x06,  0x10,
    0x3,  0x34,  0x00,  0x00,
    0x3,  0x34,  0x06,  0x08,
    0x3,  0x34,  0x08,  0x05,
    0x3,  0x34,  0x0c,  0x05,
    0x9,  0x34,  0x25,  0x51,  0x10,  0x14,  0x10,  0x10,  0x10,  0x04,
    0x3,  0x35,  0x04,  0x08,
    0x4,  0x35,  0x08,  0x01,  0x00,
    0x3,  0x35,  0x44,  0x08,
    0x4,  0x35,  0x48,  0x01,  0x00,
    0x3,  0x35,  0x84,  0x08,
    0x4,  0x35,  0x88,  0x01,  0x00,
    0x3,  0x36,  0x01,  0x70,
    0x5,  0x36,  0x04,  0xe3,  0x7f,  0x00,
    0x3,  0x36,  0x08,  0xa8,
    0x4,  0x36,  0x0a,  0xd0,  0x08,
    0x8,  0x36,  0x0e,  0xc8,  0x66,  0x81,  0x89,  0x4e,  0xbd,
    0xa,  0x36,  0x2a,  0x0e,  0x0e,  0x0e,  0x0e,  0x0c,  0x1a,  0x32,  0x64,
    0x3,  0x36,  0x38,  0x00,
    0xa,  0x36,  0x43,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,
    0x8,  0x36,  0x4c,  0x0e,  0x0e,  0x0e,  0x0e,  0xff,  0xff,
    0x5,  0x36,  0x61,  0x07,  0x02,  0x20,
    0x3,  0x36,  0x65,  0x12,
    0x4,  0x36,  0x67,  0xd4,  0x80,
    0x5,  0x36,  0x6f,  0x00,  0xc7,  0x08,
    0x3,  0x36,  0x73,  0x2a,
    0x3,  0x36,  0x81,  0x80,
    0x6,  0x37,  0x00,  0x26,  0x1e,  0x25,  0x28,
    0x8,  0x37,  0x06,  0x3e,  0x0a,  0x36,  0x55,  0x00,  0xa3,
    0x3,  0x37,  0x14,  0x01,
    0x5,  0x37,  0x1b,  0x16,  0x00,  0x08,
    0x4,  0x37,  0x56,  0x9b,  0x9b,
    0x3,  0x37,  0x62,  0x1d,
    0x3,  0x37,  0x6c,  0x00,
    0x4,  0x37,  0x76,  0x05,  0x22,
    0x3,  0x37,  0x79,  0x60,
    0x3,  0x37,  0x7c,  0x48,
    0x5,  0x37,  0x83,  0x02,  0x06,  0x0a,
    0x3,  0x37,  0x90,  0x10,
    0x4,  0x37,  0x93,  0x04,  0x07,
    0x4,  0x37,  0x96,  0x00,  0x02,
    0x3,  0x37,  0x9c,  0x4d,
    0x3,  0x37,  0xa1,  0x80,
    0x3,  0x37,  0xbb,  0x88,
    0x6,  0x37,  0xbd,  0x01,  0x01,  0x00,  0x01,
    0x3,  0x37,  0xc7,  0x56,
    0x3,  0x37,  0xca,  0x21,
    0x4,  0x37,  0xcc,  0x13,  0x90,
    0x3,  0x37,  0xcf,  0x02,
    0x5,  0x37,  0xd1,  0x3e,  0x00,  0xa3,
    0x6,  0x37,  0xd5,  0x3e,  0x00,  0xa3,  0x01,
    0x6,  0x37,  0xda,  0x00,  0x00,  0x00,  0x00,
    0x12,  0x38,  0x00,  0x00,  0x00,  0x00,  0x04,  0x07,  0x8f,  0x04,  0x43,  0x07,  0x80,  0x04,  0x38,  0x05,  0xa0,  0x09,  0xc0,
    0x3,  0x38,  0x11,  0x08,
    0x7,  0x38,  0x13,  0x04,  0x01,  0x01,  0x01,  0x01,
    0x3,  0x38,  0x1c,  0x00,
    0x5,  0x38,  0x20,  0x02,  0x00,  0x14,
    0x3,  0x38,  0x33,  0x41,
    0x4,  0x38,  0x4c,  0x02,  0xd0,
    0x3,  0x38,  0x58,  0x0d,
    0x6,  0x38,  0x65,  0x00,  0xc0,  0x00,  0xc0,
    0x3,  0x39,  0x00,  0x13,
    0x3,  0x39,  0x40,  0x13,
    0x3,  0x39,  0x80,  0x13,
    0x8,  0x39,  0x0c,  0x03,  0x02,  0x01,  0x03,  0x02,  0x01,
    0x8,  0x39,  0x4c,  0x02,  0x02,  0x01,  0x02,  0x02,  0x01,
    0x8,  0x39,  0x8c,  0x02,  0x01,  0x01,  0x02,  0x01,  0x01,
    0x3,  0x53,  0x95,  0x38,
    0x3,  0x53,  0x92,  0x14,
    0x8,  0x53,  0x96,  0x02,  0x01,  0x01,  0x02,  0x01,  0x01,
    0x3,  0x54,  0x15,  0x38,
    0x3,  0x54,  0x12,  0x14,
    0x8,  0x54,  0x16,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,
    0x3,  0x54,  0x95,  0x38,
    0x3,  0x54,  0x92,  0x14,
    0x8,  0x54,  0x96,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,
    0x3,  0x3c,  0x01,  0x11,
    0x3,  0x3c,  0x05,  0x00,
    0x3,  0x3c,  0x0f,  0x1c,
    0x3,  0x3c,  0x12,  0x0d,
    0x3,  0x3c,  0x14,  0x21,
    0x3,  0x3c,  0x19,  0x01,
    0x3,  0x3c,  0x21,  0x40,
    0x3,  0x3c,  0x3b,  0x18,
    0x3,  0x3c,  0x3d,  0xc9,
    0x3,  0x3c,  0x55,  0xcb,
    0x4,  0x3c,  0x5d,  0xcf,  0xcf,
    0x6,  0x3c,  0xe0,  0x00,  0x00,  0x00,  0x00,
    0x4,  0x3d,  0x8c,  0x70,  0x10,
    0x3,  0x40,  0x01,  0x2f,
    0x3,  0x40,  0x33,  0x80,
    0x4,  0x40,  0x08,  0x02,  0x11,
    0x4,  0x40,  0x04,  0x00,  0x40,
    0x4,  0x40,  0x0a,  0x01,  0x3c,
    0x3,  0x40,  0x0e,  0x40,
    0x3,  0x40,  0x11,  0xbb,
    0x3,  0x41,  0x0f,  0x01,
    0x6,  0x40,  0x28,  0x6f,  0x0f,  0x3f,  0x01,
    0x7,  0x40,  0x2e,  0x00,  0x40,  0x00,  0x40,  0x2f,
    0x4,  0x40,  0x50,  0x00,  0x07,
    0x5,  0x42,  0x88,  0xcf,  0x03,  0x46,
    0x6,  0x43,  0x0b,  0x0f,  0xfc,  0x00,  0x00,
    0x3,  0x43,  0x14,  0x04,
    0x4,  0x45,  0x00,  0x18,  0x18,
    0x3,  0x45,  0x04,  0x00,
    0x4,  0x45,  0x07,  0x02,  0x1a,
    0x3,  0x46,  0x03,  0x00,
    0x3,  0x46,  0x40,  0x62,
    0x6,  0x46,  0x46,  0xaa,  0x55,  0x99,  0x66,
    0x3,  0x46,  0x4d,  0x00,
    0x4,  0x46,  0x54,  0x11,  0x22,
    0x3,  0x48,  0x00,  0x04,
    0x4,  0x48,  0x10,  0xff,  0xff,
    0x3,  0x48,  0x0e,  0x00,
    0x3,  0x48,  0x13,  0x00,
    0x3,  0x48,  0x37,  0x0e,
    0x3,  0x48,  0x4b,  0x27,
    0x8,  0x4d,  0x00,  0x4e,  0x0c,  0xb8,  0xea,  0x74,  0xb7,
    0x3,  0x4d,  0x09,  0x4f,
    0x3,  0x50,  0x00,  0x1f,
    0x3,  0x50,  0x80,  0x00,
    0x3,  0x50,  0xc0,  0x00,
    0x3,  0x51,  0x00,  0x00,
    0x6,  0x52,  0x00,  0x00,  0x70,  0x03,  0x7f,
    0x3,  0x57,  0x80,  0x53,
    0x3,  0x57,  0x86,  0x01,
    0x3,  0x35,  0x01,  0x02,
    0x3,  0x03,  0x05,  0x50,
    0x3,  0x48,  0x37,  0x10,
    0x6,  0x38,  0x0c,  0x05,  0xa0,  0x09,  0xc0,
    0x4,  0x35,  0x01,  0x04,  0xda,
    0x94,
};
#else
/*
 * Xclk 24Mhz
 * max_framerate 60fps
 * mipi_datarate per lane 205Mbps, 2lane
 * binning to 960x540
 */
static const struct os02k10_sensor_reg g_os02k10_640x480_120fps_reg_table[] =
{
    {0x0103, 0x01},
    {REG_DELAY, 0x0a},
    {0x0109, 0x01},
    {0x0104, 0x02},
    {0x0102, 0x00},
    {0x0305, 0x5c},
    {0x0306, 0x00},
    {0x0307, 0x00},
    {0x030a, 0x01},
    {0x0317, 0x09},
    {0x0323, 0x07},
    {0x0324, 0x01},
    {0x0325, 0xb0},
    {0x0327, 0x07},
    {0x032c, 0x02},
    {0x032d, 0x02},
    {0x032e, 0x05},
    {0x300f, 0x11},
    {0x3012, 0x21},
    {0x3026, 0x10},
    {0x3027, 0x08},
    {0x302d, 0x24},
    {0x3103, 0x29},
    {0x3106, 0x10},
    {0x3400, 0x00},
    {0x3406, 0x08},
    {0x3408, 0x05},
    {0x340c, 0x05},
    {0x3425, 0x51},
    {0x3426, 0x10},
    {0x3427, 0x14},
    {0x3428, 0x10},
    {0x3429, 0x10},
    {0x342a, 0x10},
    {0x342b, 0x04},
    {0x3504, 0x08},
    {0x3508, 0x01},
    {0x3509, 0x00},
    {0x3544, 0x08},
    {0x3548, 0x01},
    {0x3549, 0x00},
    {0x3584, 0x08},
    {0x3588, 0x01},
    {0x3589, 0x00},
    {0x3601, 0x70},
    {0x3604, 0xe3},
    {0x3605, 0x7f},
    {0x3606, 0x00},
    {0x3608, 0xa8},
    {0x360a, 0xd0},
    {0x360b, 0x08},
    {0x360e, 0xc8},
    {0x360f, 0x66},
    {0x3610, 0x81},
    {0x3611, 0x89},
    {0x3612, 0x4e},
    {0x3613, 0xbd},
    {0x362a, 0x0e},
    {0x362b, 0x0e},
    {0x362c, 0x0e},
    {0x362d, 0x0e},
    {0x362e, 0x0c},
    {0x362f, 0x1a},
    {0x3630, 0x32},
    {0x3631, 0x64},
    {0x3638, 0x00},
    {0x3643, 0x00},
    {0x3644, 0x00},
    {0x3645, 0x00},
    {0x3646, 0x00},
    {0x3647, 0x00},
    {0x3648, 0x00},
    {0x3649, 0x00},
    {0x364a, 0x04},
    {0x364c, 0x0e},
    {0x364d, 0x0e},
    {0x364e, 0x0e},
    {0x364f, 0x0e},
    {0x3650, 0xff},
    {0x3651, 0xff},
    {0x3661, 0x07},
    {0x3662, 0x02},
    {0x3663, 0x20},
    {0x3665, 0x12},
    {0x3667, 0xd4},
    {0x3668, 0x80},
    {0x366f, 0x00},
    {0x3670, 0xc7},
    {0x3671, 0x08},
    {0x3673, 0x2a},
    {0x3681, 0x80},
    {0x3700, 0x26},
    {0x3701, 0x1e},
    {0x3702, 0x25},
    {0x3703, 0x28},
    {0x3706, 0x3e},
    {0x3707, 0x0a},
    {0x3708, 0x36},
    {0x3709, 0x55},
    {0x370a, 0x00},
    {0x370b, 0xa3},
    {0x3714, 0x03},
    {0x371b, 0x16},
    {0x371c, 0x00},
    {0x371d, 0x08},
    {0x3756, 0x9b},
    {0x3757, 0x9b},
    {0x3762, 0x1d},
    {0x376c, 0x10},
    {0x3776, 0x05},
    {0x3777, 0x22},
    {0x3779, 0x60},
    {0x377c, 0x48},
    {0x3783, 0x02},
    {0x3784, 0x06},
    {0x3785, 0x0a},
    {0x3790, 0x10},
    {0x3793, 0x04},
    {0x3794, 0x07},
    {0x3796, 0x00},
    {0x3797, 0x02},
    {0x379c, 0x4d},
    {0x37a1, 0x80},
    {0x37bb, 0x88},
    {0x37bd, 0x01},
    {0x37be, 0x01},
    {0x37bf, 0x00},
    {0x37c0, 0x01},
    {0x37c7, 0x56},
    {0x37ca, 0x21},
    {0x37cc, 0x13},
    {0x37cd, 0x90},
    {0x37cf, 0x04},
    {0x37d1, 0x3e},
    {0x37d2, 0x00},
    {0x37d3, 0xa3},
    {0x37d5, 0x3e},
    {0x37d6, 0x00},
    {0x37d7, 0xa3},
    {0x37d8, 0x01},
    {0x37da, 0x00},
    {0x37db, 0x00},
    {0x37dc, 0x00},
    {0x37dd, 0x00},
    {0x3800, 0x00},
    {0x3801, 0x00},
    {0x3802, 0x00},
    {0x3803, 0x04},
    {0x3804, 0x07},
    {0x3805, 0x8f},
    {0x3806, 0x04},
    {0x3807, 0x43},
    {0x3808, 0x07},
    {0x3809, 0x80},
    {0x380a, 0x04},
    {0x380b, 0x38},
    {0x380c, 0x02},
    {0x380d, 0xd0},
    {0x380e, 0x04},
    {0x380f, 0xe2},
    {0x3811, 0x08},
    {0x3813, 0x04},
    {0x3814, 0x03},
    {0x3815, 0x01},
    {0x3816, 0x03},
    {0x3817, 0x01},
    {0x381c, 0x00},
    {0x3820, 0x02},
    {0x3821, 0x09},
    {0x3822, 0x14},
    {0x3833, 0x41},
    {0x384c, 0x02},
    {0x384d, 0xd0},
    {0x3858, 0x0d},
    {0x3865, 0x00},
    {0x3866, 0xc0},
    {0x3867, 0x00},
    {0x3868, 0xc0},
    {0x3900, 0x13},
    {0x3940, 0x13},
    {0x3980, 0x13},
    {0x390c, 0x03},
    {0x390d, 0x02},
    {0x390e, 0x01},
    {0x390f, 0x03},
    {0x3910, 0x02},
    {0x3911, 0x01},
    {0x394c, 0x02},
    {0x394d, 0x02},
    {0x394e, 0x01},
    {0x394f, 0x02},
    {0x3950, 0x02},
    {0x3951, 0x01},
    {0x398c, 0x02},
    {0x398d, 0x01},
    {0x398e, 0x01},
    {0x398f, 0x02},
    {0x3990, 0x01},
    {0x3991, 0x01},
    {0x5395, 0x38},
    {0x5392, 0x14},
    {0x5396, 0x02},
    {0x5397, 0x01},
    {0x5398, 0x01},
    {0x5399, 0x02},
    {0x539a, 0x01},
    {0x539b, 0x01},
    {0x5415, 0x38},
    {0x5412, 0x14},
    {0x5416, 0x01},
    {0x5417, 0x01},
    {0x5418, 0x01},
    {0x5419, 0x01},
    {0x541a, 0x01},
    {0x541b, 0x01},
    {0x5495, 0x38},
    {0x5492, 0x14},
    {0x5496, 0x01},
    {0x5497, 0x01},
    {0x5498, 0x01},
    {0x5499, 0x01},
    {0x549a, 0x01},
    {0x549b, 0x01},
    {0x3c01, 0x11},
    {0x3c05, 0x00},
    {0x3c0f, 0x1c},
    {0x3c12, 0x0d},
    {0x3c14, 0x21},
    {0x3c19, 0x01},
    {0x3c21, 0x40},
    {0x3c3b, 0x18},
    {0x3c3d, 0xc9},
    {0x3c55, 0x08},
    {0x3c5d, 0xcf},
    {0x3c5e, 0xcf},
    {0x3ce0, 0x00},
    {0x3ce1, 0x00},
    {0x3ce2, 0x00},
    {0x3ce3, 0x00},
    {0x3d8c, 0x70},
    {0x3d8d, 0x10},
    {0x4001, 0x2f},
    {0x4033, 0x80},
    {0x4008, 0x02},
    {0x4009, 0x07},
    {0x4004, 0x00},
    {0x4005, 0x40},
    {0x400a, 0x01},
    {0x400b, 0x3c},
    {0x400e, 0x40},
    {0x4011, 0xbb},
    {0x410f, 0x01},
    {0x4028, 0x6f},
    {0x4029, 0x0f},
    {0x402a, 0x3f},
    {0x402b, 0x01},
    {0x402e, 0x00},
    {0x402f, 0x40},
    {0x4030, 0x00},
    {0x4031, 0x40},
    {0x4032, 0x2f},
    {0x4050, 0x00},
    {0x4051, 0x03},
    {0x4288, 0xcf},
    {0x4289, 0x03},
    {0x428a, 0x46},
    {0x430b, 0x0f},
    {0x430c, 0xfc},
    {0x430d, 0x00},
    {0x430e, 0x00},
    {0x4314, 0x04},
    {0x4500, 0x1a},
    {0x4501, 0x18},
    {0x4504, 0x00},
    {0x4507, 0x02},
    {0x4508, 0x1a},
    {0x4603, 0x00},
    {0x4640, 0x62},
    {0x4646, 0xaa},
    {0x4647, 0x55},
    {0x4648, 0x99},
    {0x4649, 0x66},
    {0x464d, 0x00},
    {0x4654, 0x11},
    {0x4655, 0x22},
    {0x4800, 0x04},
    {0x4810, 0xff},
    {0x4811, 0xff},
    {0x480e, 0x00},
    {0x4813, 0x00},
    {0x4837, 0x0e},
    {0x484b, 0x27},
    {0x4d00, 0x4e},
    {0x4d01, 0x0c},
    {0x4d02, 0xb8},
    {0x4d03, 0xea},
    {0x4d04, 0x74},
    {0x4d05, 0xb7},
    {0x4d09, 0x4f},
    {0x5000, 0x1f},
    {0x5080, 0x00},
    {0x50c0, 0x00},
    {0x5100, 0x00},
    {0x5200, 0x00},
    {0x5201, 0x70},
    {0x5202, 0x03},
    {0x5203, 0x7f},
    {0x5780, 0x53},
    {0x5786, 0x01},
    {0x3501, 0x02},
    {0x3800, 0x00},
    {0x3801, 0x00},
    {0x3802, 0x00},
    {0x3803, 0x00},
    {0x3804, 0x07},
    {0x3805, 0x8f},
    {0x3806, 0x04},
    {0x3807, 0x47},
    {0x3808, 0x02},
    {0x3809, 0x80},
    {0x380a, 0x01},
    {0x380b, 0xe0},
    {0x3811, 0xa0},
    {0x3813, 0x1e},
    {0x0305, 0x50},
    {0x4837, 0x10},
    {0x380c, 0x05},
    {0x380d, 0xa0},
    {0x380e, 0x02},
    {0x380f, 0x71},
    {0x3501, 0x02},
    {0x3502, 0x69},
    {REG_END, 0x00},
};

/*
 * Xclk 24Mhz
 * max_framerate 30fps
 * mipi_datarate per lane 405Mbps, 2lane
 * full resolution
 */
static const struct os02k10_sensor_reg g_os02k10_1920x1080_30fps_reg_table[] =
{
    {0x0103, 0x01},
    {REG_DELAY, 0x0a},
    {0x0109, 0x01},
    {0x0104, 0x02},
    {0x0102, 0x00},
    {0x0305, 0x5c},
    {0x0306, 0x00},
    {0x0307, 0x00},
    {0x030a, 0x01},
    {0x0317, 0x09},
    {0x0323, 0x07},
    {0x0324, 0x01},
    {0x0325, 0xb0},
    {0x0327, 0x07},
    {0x032c, 0x02},
    {0x032d, 0x02},
    {0x032e, 0x05},
    {0x300f, 0x11},
    {0x3012, 0x21},
    {0x3026, 0x10},
    {0x3027, 0x08},
    {0x302d, 0x24},
    {0x3103, 0x29},
    {0x3106, 0x10},
    {0x3400, 0x00},
    {0x3406, 0x08},
    {0x3408, 0x05},
    {0x340c, 0x05},
    {0x3425, 0x51},
    {0x3426, 0x10},
    {0x3427, 0x14},
    {0x3428, 0x10},
    {0x3429, 0x10},
    {0x342a, 0x10},
    {0x342b, 0x04},
    {0x3504, 0x08},
    {0x3508, 0x01},
    {0x3509, 0x00},
    {0x3544, 0x08},
    {0x3548, 0x01},
    {0x3549, 0x00},
    {0x3584, 0x08},
    {0x3588, 0x01},
    {0x3589, 0x00},
    {0x3601, 0x70},
    {0x3604, 0xe3},
    {0x3605, 0x7f},
    {0x3606, 0x00},
    {0x3608, 0xa8},
    {0x360a, 0xd0},
    {0x360b, 0x08},
    {0x360e, 0xc8},
    {0x360f, 0x66},
    {0x3610, 0x81},
    {0x3611, 0x89},
    {0x3612, 0x4e},
    {0x3613, 0xbd},
    {0x362a, 0x0e},
    {0x362b, 0x0e},
    {0x362c, 0x0e},
    {0x362d, 0x0e},
    {0x362e, 0x0c},
    {0x362f, 0x1a},
    {0x3630, 0x32},
    {0x3631, 0x64},
    {0x3638, 0x00},
    {0x3643, 0x00},
    {0x3644, 0x00},
    {0x3645, 0x00},
    {0x3646, 0x00},
    {0x3647, 0x00},
    {0x3648, 0x00},
    {0x3649, 0x00},
    {0x364a, 0x04},
    {0x364c, 0x0e},
    {0x364d, 0x0e},
    {0x364e, 0x0e},
    {0x364f, 0x0e},
    {0x3650, 0xff},
    {0x3651, 0xff},
    {0x3661, 0x07},
    {0x3662, 0x02},
    {0x3663, 0x20},
    {0x3665, 0x12},
    {0x3667, 0xd4},
    {0x3668, 0x80},
    {0x366f, 0x00},
    {0x3670, 0xc7},
    {0x3671, 0x08},
    {0x3673, 0x2a},
    {0x3681, 0x80},
    {0x3700, 0x26},
    {0x3701, 0x1e},
    {0x3702, 0x25},
    {0x3703, 0x28},
    {0x3706, 0x3e},
    {0x3707, 0x0a},
    {0x3708, 0x36},
    {0x3709, 0x55},
    {0x370a, 0x00},
    {0x370b, 0xa3},
    {0x3714, 0x01},
    {0x371b, 0x16},
    {0x371c, 0x00},
    {0x371d, 0x08},
    {0x3756, 0x9b},
    {0x3757, 0x9b},
    {0x3762, 0x1d},
    {0x376c, 0x00},
    {0x3776, 0x05},
    {0x3777, 0x22},
    {0x3779, 0x60},
    {0x377c, 0x48},
    {0x3783, 0x02},
    {0x3784, 0x06},
    {0x3785, 0x0a},
    {0x3790, 0x10},
    {0x3793, 0x04},
    {0x3794, 0x07},
    {0x3796, 0x00},
    {0x3797, 0x02},
    {0x379c, 0x4d},
    {0x37a1, 0x80},
    {0x37bb, 0x88},
    {0x37bd, 0x01},
    {0x37be, 0x01},
    {0x37bf, 0x00},
    {0x37c0, 0x01},
    {0x37c7, 0x56},
    {0x37ca, 0x21},
    {0x37cc, 0x13},
    {0x37cd, 0x90},
    {0x37cf, 0x02},
    {0x37d1, 0x3e},
    {0x37d2, 0x00},
    {0x37d3, 0xa3},
    {0x37d5, 0x3e},
    {0x37d6, 0x00},
    {0x37d7, 0xa3},
    {0x37d8, 0x01},
    {0x37da, 0x00},
    {0x37db, 0x00},
    {0x37dc, 0x00},
    {0x37dd, 0x00},
    {0x3800, 0x00},
    {0x3801, 0x00},
    {0x3802, 0x00},
    {0x3803, 0x04},
    {0x3804, 0x07},
    {0x3805, 0x8f},
    {0x3806, 0x04},
    {0x3807, 0x43},
    {0x3808, 0x07},
    {0x3809, 0x80},
    {0x380a, 0x04},
    {0x380b, 0x38},
    {0x380c, 0x05},
    {0x380d, 0xa0},
    {0x380e, 0x09},
    {0x380f, 0xc0},
    {0x3811, 0x08},
    {0x3813, 0x04},
    {0x3814, 0x01},
    {0x3815, 0x01},
    {0x3816, 0x01},
    {0x3817, 0x01},
    {0x381c, 0x00},
    {0x3820, 0x02},
    {0x3821, 0x00},
    {0x3822, 0x14},
    {0x3833, 0x41},
    {0x384c, 0x02},
    {0x384d, 0xd0},
    {0x3858, 0x0d},
    {0x3865, 0x00},
    {0x3866, 0xc0},
    {0x3867, 0x00},
    {0x3868, 0xc0},
    {0x3900, 0x13},
    {0x3940, 0x13},
    {0x3980, 0x13},
    {0x390c, 0x03},
    {0x390d, 0x02},
    {0x390e, 0x01},
    {0x390f, 0x03},
    {0x3910, 0x02},
    {0x3911, 0x01},
    {0x394c, 0x02},
    {0x394d, 0x02},
    {0x394e, 0x01},
    {0x394f, 0x02},
    {0x3950, 0x02},
    {0x3951, 0x01},
    {0x398c, 0x02},
    {0x398d, 0x01},
    {0x398e, 0x01},
    {0x398f, 0x02},
    {0x3990, 0x01},
    {0x3991, 0x01},
    {0x5395, 0x38},
    {0x5392, 0x14},
    {0x5396, 0x02},
    {0x5397, 0x01},
    {0x5398, 0x01},
    {0x5399, 0x02},
    {0x539a, 0x01},
    {0x539b, 0x01},
    {0x5415, 0x38},
    {0x5412, 0x14},
    {0x5416, 0x01},
    {0x5417, 0x01},
    {0x5418, 0x01},
    {0x5419, 0x01},
    {0x541a, 0x01},
    {0x541b, 0x01},
    {0x5495, 0x38},
    {0x5492, 0x14},
    {0x5496, 0x01},
    {0x5497, 0x01},
    {0x5498, 0x01},
    {0x5499, 0x01},
    {0x549a, 0x01},
    {0x549b, 0x01},
    {0x3c01, 0x11},
    {0x3c05, 0x00},
    {0x3c0f, 0x1c},
    {0x3c12, 0x0d},
    {0x3c14, 0x21},
    {0x3c19, 0x01},
    {0x3c21, 0x40},
    {0x3c3b, 0x18},
    {0x3c3d, 0xc9},
    {0x3c55, 0xcb},
    {0x3c5d, 0xcf},
    {0x3c5e, 0xcf},
    {0x3ce0, 0x00},
    {0x3ce1, 0x00},
    {0x3ce2, 0x00},
    {0x3ce3, 0x00},
    {0x3d8c, 0x70},
    {0x3d8d, 0x10},
    {0x4001, 0x2f},
    {0x4033, 0x80},
    {0x4008, 0x02},
    {0x4009, 0x11},
    {0x4004, 0x00},
    {0x4005, 0x40},
    {0x400a, 0x01},
    {0x400b, 0x3c},
    {0x400e, 0x40},
    {0x4011, 0xbb},
    {0x410f, 0x01},
    {0x4028, 0x6f},
    {0x4029, 0x0f},
    {0x402a, 0x3f},
    {0x402b, 0x01},
    {0x402e, 0x00},
    {0x402f, 0x40},
    {0x4030, 0x00},
    {0x4031, 0x40},
    {0x4032, 0x2f},
    {0x4050, 0x00},
    {0x4051, 0x07},
    {0x4288, 0xcf},
    {0x4289, 0x03},
    {0x428a, 0x46},
    {0x430b, 0x0f},
    {0x430c, 0xfc},
    {0x430d, 0x00},
    {0x430e, 0x00},
    {0x4314, 0x04},
    {0x4500, 0x18},
    {0x4501, 0x18},
    {0x4504, 0x00},
    {0x4507, 0x02},
    {0x4508, 0x1a},
    {0x4603, 0x00},
    {0x4640, 0x62},
    {0x4646, 0xaa},
    {0x4647, 0x55},
    {0x4648, 0x99},
    {0x4649, 0x66},
    {0x464d, 0x00},
    {0x4654, 0x11},
    {0x4655, 0x22},
    {0x4800, 0x04},
    {0x4810, 0xff},
    {0x4811, 0xff},
    {0x480e, 0x00},
    {0x4813, 0x00},
    {0x4837, 0x0e},
    {0x484b, 0x27},
    {0x4d00, 0x4e},
    {0x4d01, 0x0c},
    {0x4d02, 0xb8},
    {0x4d03, 0xea},
    {0x4d04, 0x74},
    {0x4d05, 0xb7},
    {0x4d09, 0x4f},
    {0x5000, 0x1f},
    {0x5080, 0x00},
    {0x50c0, 0x00},
    {0x5100, 0x00},
    {0x5200, 0x00},
    {0x5201, 0x70},
    {0x5202, 0x03},
    {0x5203, 0x7f},
    {0x5780, 0x53},
    {0x5786, 0x01},
    {0x3501, 0x02},
    {0x0305, 0x50},
    {0x4837, 0x10},
    {0x380c, 0x05},
    {0x380d, 0xa0},
    {0x380e, 0x09},
    {0x380f, 0xc0},
    {0x3501, 0x04},
    {0x3502, 0xda},
    {REG_END, 0x00},
};
#endif

static const struct os02k10_mode supported_modes[] =
{
    [OS02K10_640X480] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 640,
        .height = 480,
        .max_fps = {
            .numerator = 10000,
            .denominator = 1200000,
        },
        .exp_def = 0x269,
        .hts_def = 0x05a0,
        .vts_def = 0x0271,
        .xvclk = 24000000,
        .link_freq = 480000000,
        .reg_list = g_os02k10_640x480_120fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_os02k10_640x480_120fps_reg_table),
        .hdr_mode = NO_HDR,
    },
    [OS02K10_1920X1080] = {
        .bus_fmt = MEDIA_BUS_FMT_SBGGR10_1X10,
        .width = 1920,
        .height = 1080,
        .max_fps = {
            .numerator = 10000,
            .denominator = 300000,
        },
        .exp_def = 0x04da,
        .hts_def = 0x05a0,
        .vts_def = 0x09c0,
        .xvclk = 24000000,
        .link_freq = 480000000,
        .reg_list = g_os02k10_1920x1080_30fps_reg_table,
        .reg_list_size = ARRAY_SIZE(g_os02k10_1920x1080_30fps_reg_table),
        .hdr_mode = NO_HDR,
    },
};

struct os02k10_ops
{
    ret_err_t (*init)(struct os02k10_dev *dev);
    ret_err_t (*open)(struct os02k10_dev *dev, uint16_t oflag);
    ret_err_t (*close)(struct os02k10_dev *dev);
    ret_err_t (*control)(struct os02k10_dev *dev, dt_cmd_t cmd, void *arg);
};

static ret_err_t os02k10_read_reg(struct os02k10_dev *dev,
                                  uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    int retry = 10;
    ret_err_t ret;

    RT_ASSERT(dev->i2c_bus != RT_NULL);

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
            return RET_SYS_EOK;
        }
        else
        {
            os02k10_err("read reg, retry=%d, reg [0x%x]\n", retry, reg);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static ret_err_t os02k10_write_reg(struct os02k10_dev *dev,
                                   uint16_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msg;
    int ret = 0;
    int retry = 10;

    RT_ASSERT(dev->i2c_bus != RT_NULL);

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
            return RET_SYS_EOK;
        }
        else
        {
            os02k10_err("write reg, retry:%d, reg [0x%x]-->0x%x\n", retry, reg, data);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

#ifndef REG_LIST_CONVERSION_DEBUG
static int os02k10_write_multiple_reg_continue(struct os02k10_dev *dev,
        const uint8_t *i2c_data, int len)
{
    uint16_t i;
    struct rt_i2c_bus_device *i2c_bus;
    struct rt_i2c_msg msg;
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
        //os02k10_info("write addr: %x, tick %d\n", addr, rt_tick_get());
        if (addr != REG_DELAY)
        {
            msg.addr = dev->i2c_addr;
            msg.flags = RT_I2C_WR;
            msg.buf = (uint8_t *)&i2c_data[offset + 1];
            msg.len = i2c_data[offset];
            ret = rt_i2c_transfer(i2c_bus, &msg, 1);

            if (ret != 1)
            {
                os02k10_err("write multi-regs, retry=%d, addr=0x%02x%02x\n", retry, i2c_data[offset + 1], i2c_data[offset + 2]);
                if (--retry)
                    continue;
                else
                    return RET_SYS_ERROR;
            }
        }
        else
        {
            os02k10_info("delay %d ms\n", i2c_data[offset + 3]);
            HAL_DelayUs(i2c_data[offset + 3] * 1000);
        }

        offset += (i2c_data[offset] + 1);
        retry = 10;
        i++;
    }

    return RET_SYS_EOK;
}

#else
static ret_err_t os02k10_write_reg_discontinued(struct os02k10_dev *dev,
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
            os02k10_err("write discontinued reg, retry:%d\n", retry);
            if (--retry)
                continue;
            else
                return RET_SYS_ERROR;
        }
    }
}

static void os02k10_write_multiple_reg(struct os02k10_dev *dev,
                                       const struct os02k10_sensor_reg *reg, int len)
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

                os02k10_write_reg_discontinued(i2c_bus, data, k + 2);
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

                os02k10_write_reg_discontinued(i2c_bus, data, k + 2);
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

                    os02k10_write_reg_discontinued(i2c_bus, data, k + 2);
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

static void rk_continuous_reg_dump(struct os02k10_dev *dev)
{
    int id = 0;

    for (id = 0; id < sizeof(supported_modes) / sizeof(supported_modes[0]); id++)
    {
        os02k10_info("id: %d, width: %d, height: %d\n",
                     id, supported_modes[id].width, supported_modes[id].height);
        os02k10_write_multiple_reg((struct os02k10_dev *)dev,
                                   supported_modes[id].reg_list,
                                   supported_modes[id].reg_list_size);
    }
}
#endif

static int os02k10_get_gain(struct os02k10_dev *dev, uint32_t *gain)
{
    int ret = 0;
    uint8_t gain16_H, gain16_L;
    ret = os02k10_read_reg(dev, OS02K10_AEC_LONG_REL_GAIN_REG_H, &gain16_H);
    gain16_H = gain16_H & 0x0f;
    ret |= os02k10_read_reg(dev, OS02K10_AEC_LONG_REL_GAIN_REG_L, &gain16_L);
    gain16_L = gain16_L & 0xf0;
    *gain = (gain16_H << 4) | (gain16_L >> 4);
    if (ret == RET_SYS_EOK)
    {
        return RET_SYS_EOK;
    }
    else
    {
        os02k10_info("Warning: get gain16 failed with error (%d)!!!\n", ret);
        return ret;
    }
}

static ret_err_t rk_os02k10_set_expval(struct os02k10_dev *dev, struct rk_camera_exp_val *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    uint32_t l_exp_time;
    uint32_t l_a_gain;
    uint32_t a_gain, d_gain;
    uint8_t reg_data;

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (!dev->has_init_exp && !dev->streaming)
    {
        dev->init_exp = *exp;
        dev->has_init_exp = true;
        os02k10_dbg("os02k10 don't stream, record exp for hdr!\n");

        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }

    l_exp_time = exp->reg_time[0];
    l_a_gain = exp->reg_gain[0];

    os02k10_dbg("os02k10: exp req: L_exp: 0x%x, 0x%x\n",
                l_exp_time, l_a_gain);

    ret  = os02k10_write_reg(dev, OS02K10_AEC_LONG_EXP_REG_H,
                             (uint8_t)((l_exp_time >> 8) & 0xff));
    ret  |= os02k10_write_reg(dev, OS02K10_AEC_LONG_EXP_REG_L,
                              (uint8_t)((l_exp_time) & 0xff));
    /*
    * dgain reg format is 4.10bits, again reg format is 4.4bits
    * 1x is 64, 6 decimal places
    */
    if (l_a_gain > 992)
    {
        d_gain = l_a_gain * 1024 / 992; //15.5X * 64 = 992
        a_gain = 992;
    }
    else
    {
        d_gain = 1024; //move 10 bits left
        a_gain = l_a_gain;
    }
    ret |= os02k10_read_reg(dev, OS02K10_AEC_LONG_REL_GAIN_REG_H, &reg_data);
    reg_data = reg_data & 0xf0;
    ret |= os02k10_write_reg(dev,
                             OS02K10_AEC_LONG_REL_GAIN_REG_H,
                             ((a_gain >> 6) & 0x0f) | reg_data);

    ret |= os02k10_read_reg(dev, OS02K10_AEC_LONG_REL_GAIN_REG_L, &reg_data);
    reg_data = reg_data & 0x0f;
    ret |= os02k10_write_reg(dev,
                             OS02K10_AEC_LONG_REL_GAIN_REG_L,
                             (((a_gain >> 2) & 0x0f) << 4) | reg_data);

    ret |= os02k10_read_reg(dev, OS02K10_AEC_LONG_DIG_GAIN_REG_H, &reg_data);
    reg_data = reg_data & 0xf0;
    ret |= os02k10_write_reg(dev,
                             OS02K10_AEC_LONG_DIG_GAIN_REG_H,
                             ((d_gain >> 10) & 0x0f) | reg_data);
    ret |= os02k10_write_reg(dev,
                             OS02K10_AEC_LONG_DIG_GAIN_REG_M,
                             (d_gain >> 2) & 0xff);
    ret |= os02k10_read_reg(dev, OS02K10_AEC_LONG_DIG_GAIN_REG_L, &reg_data);
    reg_data = reg_data & 0x3f;
    ret |= os02k10_write_reg(dev,
                             OS02K10_AEC_LONG_DIG_GAIN_REG_L,
                             ((d_gain & 0x03) << 6) | reg_data);

    if (ret == RET_SYS_EOK)
    {
        rt_mutex_release(&dev->mutex_lock);
        return RET_SYS_EOK;
    }
    else
    {
        os02k10_info("Warning: set gain failed with error (%d)!!!\n", ret);
        rt_mutex_release(&dev->mutex_lock);
        return ret;
    }
}

static ret_err_t rk_os02k10_set_vts(struct os02k10_dev *dev, uint32_t dst_vts)
{
    ret_err_t ret = RET_SYS_ENOSYS;

    RT_ASSERT(dev != RT_NULL);

    os02k10_dbg("set vts: 0x%x \n", dst_vts);
    dev->dst_mode->vts_def = dst_vts;

    ret = os02k10_write_reg(dev, OS02K10_AEC_TIMING_VTS_REG_L,
                            (uint8_t)(dst_vts & 0xff));
    ret = os02k10_write_reg(dev, OS02K10_AEC_TIMING_VTS_REG_H,
                            (uint8_t)(dst_vts >> 8));

    return ret;
}

static ret_err_t rk_os02k10_set_flip_mirror(struct os02k10_dev *dev, uint32_t flip)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    uint8_t val = 0;

    RT_ASSERT(dev != RT_NULL);

    ret = os02k10_read_reg(dev, OS02K10_FLIPMIRROR_REG, &val);
    switch (flip)
    {
    case 0 ... 3:
        val = OS02K10_FETCH_MIRROR(val, flip & 0x01);
        val = OS02K10_FETCH_FLIP(val, (flip >> 1) & 0x01);
        break;
    default:
        val = OS02K10_FETCH_MIRROR(val, false);
        val = OS02K10_FETCH_FLIP(val, false);
        break;
    };
    os02k10_dbg("flip 0x%x, reg val 0x%x\n", flip, val);
    ret = os02k10_write_reg(dev, OS02K10_FLIPMIRROR_REG, val);

    return ret;
}

static void os02k10_stream_on(struct os02k10_dev *dev)
{
    os02k10_dbg("stream_on enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);
    HAL_DelayMs(10);
    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
#ifndef REG_LIST_CONVERSION_DEBUG
    os02k10_write_multiple_reg_continue((struct os02k10_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif

#ifndef RT_USING_CAM_STREAM_ON_LATE
    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_os02k10_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_os02k10_set_flip_mirror(dev, dev->flip);

    os02k10_write_reg(dev, OS02K10_REG_STREAM_CTRL, OS02K10_STREAM_ON);
    dev->streaming = true;
#endif

    rt_mutex_release(&dev->mutex_lock);
    os02k10_dbg("stream_on exit tick:%u\n", rt_tick_get());
}

static void os02k10_stream_on_late(struct os02k10_dev *dev)
{
    os02k10_dbg("stream_on_late enter tick:%u\n", rt_tick_get());

    RT_ASSERT(dev != RT_NULL);

    rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    if (dev->has_init_exp)
    {
        rt_mutex_release(&dev->mutex_lock);
        rk_os02k10_set_expval(dev, &dev->init_exp);
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
    }
    rk_os02k10_set_flip_mirror(dev, dev->flip);

    os02k10_write_reg(dev, OS02K10_REG_STREAM_CTRL, OS02K10_STREAM_ON);
    rt_mutex_release(&dev->mutex_lock);
    dev->streaming = true;
    os02k10_dbg("stream_on_late exit tick:%u\n", rt_tick_get());
}

static void os02k10_stream_off(struct os02k10_dev *dev)
{
    struct rt_i2c_bus_device *i2c_bus;

    RT_ASSERT(dev != RT_NULL);

    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        os02k10_write_reg(dev, OS02K10_REG_STREAM_CTRL, OS02K10_STREAM_OFF);

        rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        os02k10_err("not find i2c bus!\n");
    }
    dev->streaming = false;
}

ret_err_t rk_os02k10_init(struct rk_camera_device *dev)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os02k10_dev *os02k10;

    os02k10 = (struct os02k10_dev *)dev;
    struct rk_camera_device *camera = (struct rk_camera_device *)&os02k10->parent;

#ifdef REG_LIST_CONVERSION_DEBUG
    rk_continuous_reg_dump(os02k10);
#endif

    if (os02k10)
    {
        camera->info.mbus_fmt.width = os02k10->cur_mode->width;
        camera->info.mbus_fmt.height = os02k10->cur_mode->height;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.linked_freq = os02k10->cur_mode->link_freq;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_CSI2_DPHY;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2 |
                                         MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN;
        camera->info.hdr_mode =  os02k10->cur_mode->hdr_mode;

        os02k10->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(os02k10->i2c_name);

        if (!os02k10->i2c_bus)
        {
            os02k10_err("not find i2c :%s !!!\n",
                        os02k10->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            os02k10_dbg("find i2c_bus:%s\n", os02k10->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }

    return ret;

}

static ret_err_t rk_os02k10_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    ret_err_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;
}

ret_err_t rk_os02k10_close(struct rk_camera_device *dev)
{
    uint8_t ret = RET_SYS_EOK;

    RT_ASSERT(dev != RT_NULL);

    return ret;

}

static ret_err_t rk_os02k10_get_expinf(struct os02k10_dev *dev, struct rk_camera_exp_info *exp)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os02k10_mode *mode;

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

static ret_err_t rk_os02k10_get_intput_fmt(struct os02k10_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os02k10_mode *mode;

    RT_ASSERT(dev != RT_NULL);

    mode = dev->cur_mode;
    mbus_fmt->width = mode->width;
    mbus_fmt->height = mode->height;
    mbus_fmt->field = 0;
    mbus_fmt->pixelcode = MEDIA_BUS_FMT_SBGGR10_1X10;

    return ret;
}

static ret_err_t rk_os02k10_set_intput_fmt(struct os02k10_dev *dev, struct rk_camera_mbus_framefmt *mbus_fmt)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct os02k10_mode *mode;
    int i = 0;
    bool is_find_fmt = false;

    RT_ASSERT(dev != RT_NULL);

    os02k10_info("dst resulotion, width %d, height %d\n",
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
#ifndef REG_LIST_CONVERSION_DEBUG
            os02k10_write_multiple_reg_continue((struct os02k10_dev *)dev, dev->cur_mode->reg_list, dev->cur_mode->reg_list_size);
#endif
            ret = rk_os02k10_set_flip_mirror(dev, dev->flip);
            rt_mutex_release(&dev->mutex_lock);
        }
    }

    return ret;
}

static ret_err_t rk_os02k10_match_dst_config(struct os02k10_dev *dev, struct rk_camera_dst_config *dst_config)
{
    ret_err_t ret = RET_SYS_ENOSYS;
    const struct os02k10_mode *mode;
    int i = 0;
    int cur_fps, dst_fps, cur_vts, dst_vts;

    RT_ASSERT(dev != RT_NULL);

    dst_fps = dst_config->cam_fps_denominator / dst_config->cam_fps_numerator;
    dev->flip = dst_config->cam_mirror_flip;

    os02k10_info("cur resulotion, width %d, height %d, fps %d!\n",
                 dev->cur_mode->width, dev->cur_mode->height, dev->cur_mode->max_fps.denominator / dev->cur_mode->max_fps.numerator);
    //match current resolution config
    for (i = 0; i < ARRAY_SIZE(supported_modes); i++)
    {
        mode = &supported_modes[i];

        if (dst_config->width == mode->width &&
                dst_config->height == mode->height)
        {
            dev->dst_mode = (struct os02k10_mode *)mode;
            ret = RET_SYS_EOK;
            os02k10_info("find dst resulotion, width %d, height %d, fps %d!\n",
                         dst_config->width, dst_config->height, dst_fps);
            break;
        }
        else
            os02k10_dbg("not find match resulotion\n");
    }
    cur_fps = dev->dst_mode->max_fps.denominator / dev->dst_mode->max_fps.numerator;
    cur_vts = dev->dst_mode->vts_def;

    //match fps config
    if (cur_fps == dst_fps)
        return 0;

    if (dst_fps > cur_fps)
    {
        os02k10_err("dst fps is larger than cur fps\n");
        return RET_SYS_EINVAL;
    }

    dst_vts = cur_fps * cur_vts / dst_fps;

    dev->dst_mode->max_fps.denominator = dst_config->cam_fps_denominator;
    dev->dst_mode->max_fps.numerator = dst_config->cam_fps_numerator;
    dev->dst_mode->vts_def = dst_vts;

    return ret;
}

ret_err_t rk_os02k10_control(struct rk_camera_device *dev,
                             dt_cmd_t cmd,
                             void *args)
{
    ret_err_t ret = RET_SYS_EOK;
    struct os02k10_dev *os02k10;

    RT_ASSERT(dev != RT_NULL);
    os02k10 = (struct os02k10_dev *)dev;

    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_os02k10_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        os02k10_stream_on(os02k10);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        os02k10_stream_off(os02k10);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_EXP_INF:
    {
        ret = rk_os02k10_get_expinf(os02k10, (struct rk_camera_exp_info *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL:
    {
        ret = rk_os02k10_set_expval(os02k10, (struct rk_camera_exp_val *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL:
    {
        ret = rk_os02k10_set_vts(os02k10, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = rk_os02k10_get_intput_fmt(os02k10, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = rk_os02k10_set_intput_fmt(os02k10, (struct rk_camera_mbus_framefmt *)args);
    }
    break;
    case RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG:
    {
        ret = rk_os02k10_match_dst_config(os02k10, (struct rk_camera_dst_config *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = rk_os02k10_set_flip_mirror(os02k10, *(uint32_t *)args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE:
    {
        os02k10_stream_on_late(os02k10);
    }
    break;
    default:
        os02k10_dbg("exit CMD %d\n", cmd);
        break;
    }

    return ret;
}

struct rk_camera_ops rk_os02k10_ops =
{
    rk_os02k10_init,
    rk_os02k10_open,
    NULL,
    rk_os02k10_control
};

int rk_camera_os02k10_init(void)
{
    ret_err_t ret = RET_SYS_EOK;
    const struct camera_board_desc *os02k10_desc;

    for (os02k10_desc = camera_os02k10; os02k10_desc->i2c_addr != 0; os02k10_desc++)
    {
        struct os02k10_dev *instance;
        struct rk_camera_device *camera;

        instance = rt_malloc(sizeof(struct os02k10_dev));
        if (!instance)
        {
            rt_kprintf("malloc os02k10 failed\n");
            return -RT_ENOMEM;
        }
        rt_memset(instance, 0x0, sizeof(struct os02k10_dev));
        camera = &instance->parent;
        camera->ops = &rk_os02k10_ops;

        rt_strncpy(instance->name, os02k10_desc->isp_subdev_name, rt_strlen(os02k10_desc->isp_subdev_name));
        rt_strncpy(instance->i2c_name, os02k10_desc->i2c_bus, rt_strlen(os02k10_desc->i2c_bus));

        instance->i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(instance->i2c_name);
        if (instance->i2c_bus == RT_NULL)
        {
            os02k10_err("can't find i2c bus:%s\n", instance->i2c_name);
            rt_free(instance);
            continue;
        }

        if (os02k10_desc->mode_id <= OS02K10_MODE_ID_MAX)
        {
            os02k10_dbg("mode_id: %d\n", os02k10_desc->mode_id);
            instance->cur_mode = &supported_modes[os02k10_desc->mode_id];
        }
        else
        {
            os02k10_info("mode id is over range, default use mode_id: 0\n");
            instance->cur_mode = &supported_modes[0];
        }

        instance->dst_mode = (struct os02k10_mode *)&supported_modes[OS02K10_1920X1080];
        instance->time_valid_delay = os02k10_desc->time_valid_delay;
        instance->gain_valid_delay = os02k10_desc->gain_valid_delay;
        instance->i2c_addr = os02k10_desc->i2c_addr;
        instance->flip = 0;

        HAL_DelayUs(5 * 1000);

        if (os02k10_desc->rst_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)os02k10_desc->rst_gpio.gpio_group, os02k10_desc->rst_gpio.gpio_pin, GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)os02k10_desc->rst_gpio.gpio_group, os02k10_desc->rst_gpio.gpio_pin, !os02k10_desc->rst_active);
        }
        if (os02k10_desc->pwdn_gpio.gpio_group)
        {
            HAL_GPIO_SetPinDirection((struct GPIO_REG *)os02k10_desc->pwdn_gpio.gpio_group, os02k10_desc->pwdn_gpio.gpio_pin, GPIO_OUT);
            HAL_GPIO_SetPinLevel((struct GPIO_REG *)os02k10_desc->pwdn_gpio.gpio_group, os02k10_desc->pwdn_gpio.gpio_pin, !os02k10_desc->pwdn_active);
        }

        HAL_DelayUs(2 * 1000);

        if (os02k10_desc->mclk_out_gate_id && os02k10_desc->mclk_id)
        {
            clk_set_rate(os02k10_desc->mclk_id, os02k10->cur_mode->xvclk);
            HAL_CRU_ClkEnable(os02k10_desc->mclk_out_gate_id);
        }

        rt_mutex_init(&instance->mutex_lock, "OS02K10_mutex", RT_IPC_FLAG_FIFO);
        RT_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
        camera->i2c_bus = instance->i2c_bus;
        rt_strncpy(camera->name, os02k10_desc->isp_subdev_name, rt_strlen(os02k10_desc->isp_subdev_name));
        rk_camera_register(camera, camera->name, instance);
    }
    return ret;
}

void os02k10_detect(void)
{
    const struct camera_board_desc *os02k10_desc;
    struct rt_i2c_bus_device *i2c_bus;
    for (os02k10_desc = camera_os02k10; os02k10_desc->i2c_addr != 0; os02k10_desc++)
    {
        os02k10_dbg("start to detect OS02K10 for testing \n");
        os02k10_dbg("dev name:%s\n", os02k10_desc->isp_subdev_name);
        os02k10_dbg("dev i2c_bus:%s\n", os02k10_desc->i2c_bus);
        i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(os02k10_desc->i2c_bus);
        if (i2c_bus)
        {
            os02k10_err("Warning:not find i2c source 1:%s !!!\n", os02k10_desc->i2c_bus);
            return;
        }
        else
        {
            os02k10_dbg("find i2c_bus:%s\n", os02k10_desc->i2c_bus);
        }
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_os02k10_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(os02k10_detect, check OS02K10 is available or not);
#endif
#endif
#endif
