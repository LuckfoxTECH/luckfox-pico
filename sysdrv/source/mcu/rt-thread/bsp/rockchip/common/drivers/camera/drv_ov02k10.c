
/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_ov02k10.c
  * @version V0.0.1
  * @brief   video input processor driver for RK625
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

#if defined(__RT_THREAD__)
#include "camera.h"
#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"
#include <rtconfig.h>

#elif defined(__RK_OS__)
#include "driver/camera.h"
#include "drv_clock.h"
#endif

#ifdef CAMERA_MODULE_COMPILED

#if defined(__RT_THREAD__)
//#include "drv_clock.h"
#include "board.h"
#ifdef SOC_SWALLOW
#include "board_cam.h"
#endif
#elif defined(__RK_OS__)
#include "driver/drv_clock.h"
#endif

#if defined(RT_USING_OV02k10) || defined(CONFIG_DRIVER_OV02k10)
#define OV02k10_MODULE_COMPILED
#else
#undef OV02k10_MODULE_COMPILED
#endif

#endif

#ifdef OV02k10_MODULE_COMPILED
#include "ov_camera_module.h"
#include "cam_iq_data_ov02k10_v1.1_a2.h"

#define OV02K10_DEBUG_INFO 0
#define USED_2LANE

#if OV02K10_DEBUG_INFO
#include <stdio.h>
#define OV02K10_DEBUG(...)               rk_kprintf("[OV02K10]:");rk_kprintf(__VA_ARGS__)
#else
#define OV02K10_DEBUG(...)
#endif
#define OV02K10_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)
#define RT_USING_OV02K10_OPS 0

#define OV02K10_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME                 "i2c0"

#define OV02K10_7BIT_ADDRESS         (0x36)
#define OV02K10_REG_ID_H_ADDRESS     (0x300A)
#define OV02K10_REG_ID_M_ADDRESS     (0x300B)
#define OV02K10_REG_ID_L_ADDRESS     (0x300C)
#define OV02K10_SENSOR_ID            (0x530243)

#define SENSOR_ID(_hsb,_msb, _lsb)       ((_hsb) << 16 |(_msb) << 8 | (_lsb))

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))
#define OV02K10_PIN_CTRL_ENABLE          1
#define OV02K10_I2C_DEBUG_ENABLE         1
#define OV02K10_TESTPATTERN_ENABLE       0

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
#define IS_ERR_VALUE(x)                 ((x != 0) ? 1 : 0 )

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
/* AEC  ------------------------------------------------------------*/
#define OV02K10_AEC_PK_LONG_GAIN_REG_H              0x3508
#define OV02K10_AEC_PK_LONG_GAIN_REG_L              0x3509
#define OV02K10_AEC_PK_SHORT_GAIN_REG_H             0x3548
#define OV02K10_AEC_PK_SHORT_GAIN_REG_L             0x3549

#define OV02K10_AEC_PK_LONG_EXPO_2ND_REG            0x3501      /* Exposure Bits 8-15 */
#define OV02K10_AEC_PK_LONG_EXPO_1ST_REG            0x3502      /* Exposure Bits 0-7 */
#define OV02K10_AEC_PK_SHORT_EXPO_2ND_REG           0x3541      /* Exposure Bits 8-15 */
#define OV02K10_AEC_PK_SHORT_EXPO_1ST_REG           0x3542      /* Exposure Bits 0-7 */

#define OV02K10_FETCH_2ND_BYTE_EXP(VAL)                         (((VAL) >> 8) & 0xFF)
#define OV02K10_FETCH_1ST_BYTE_EXP(VAL)                         ((VAL) & 0xFF)

#define OV02K10_AEC_GROUP_UPDATE_ADDRESS                        0x3208
#define OV02K10_AEC_GROUP_UPDATE_START_DATA                     0x00
#define OV02K10_AEC_GROUP_UPDATE_END_DATA                       0x10
#define OV02K10_AEC_GROUP_UPDATE_END_LAUNCH                     0xA0
#define OV02K10_HORIZONTAL_START_HIGH_REG                       0x3800      //[15:8]
#define OV02K10_HORIZONTAL_START_LOW_REG                        0x3801      //[7:0]
#define OV02K10_VERTICAL_START_HIGH_REG                         0x3802
#define OV02K10_VERTICAL_START_LOW_REG                          0x3803
#define OV02K10_HORIZONTAL_END_HIGH_REG                         0x3804
#define OV02K10_HORIZONTAL_END_LOW_REG                          0x3805
#define OV02K10_VERTICAL_END_HIGH_REG                           0x3806
#define OV02K10_VERTICAL_END_LOW_REG                            0x3807
#define OV02K10_HORIZONTAL_OUTPUT_SIZE_HIGH_REG                 0x3808      //[10:8]
#define OV02K10_HORIZONTAL_OUTPUT_SIZE_LOW_REG                  0x3809      //[7:0]
#define OV02K10_VERTICAL_OUTPUT_SIZE_HIGH_REG                   0x380a      //[15:8]
#define OV02K10_VERTICAL_OUTPUT_SIZE_LOW_REG                    0x380b      //[7:0]
#define OV02K10_TIMING_HTS_HIGH_REG                             0x380c      //[15:8]
#define OV02K10_TIMING_HTS_LOW_REG                              0x380d      //[7:0]
#define OV02K10_TIMING_VTS_HIGH_REG                             0x380e
#define OV02K10_TIMING_VTS_LOW_REG                              0x380f
#define OV02K10_WINDOW_HORIZONTAL_TRUNCATION_SIZE_HIGH_REG      0x3810
#define OV02K10_WINDOW_HORIZONTAL_TRUNCATION_SIZE_LOW_REG       0x3811
#define OV02K10_WINDOW_VERTICAL_TRUNCATION_SIZE_HIGH_REG        0x3812
#define OV02K10_WINDOW_VERTICAL_TRUNCATION_SIZE_LOW_REG         0x3813
#define OV02K10_TIMING_X_INC                                    0x3814
#define OV02K10_TIMING_Y_INC                                    0x3816

//#define OV02K10_MIRRORFLIP_REG                                  0x3820      //bit[1] = 1,mirror
//#define OV02K10_FLIP_REG                                        0x3821      //bit[3:2]
#define OV02K10_FLIPMIRROR_REGADDR                              0x3820
#define OV02K10_FLIPMIRROR_STATE_NORMAL                         0
#define OV02K10_FLIPMIRROR_STATE_FLIPMIRROR                     1
#define OV02K10_FLIPMIRROR_STATE_MIRROR                         2
#define OV02K10_FLIPMIRROR_STATE_FLIP                           3
#define OV02K10_FLIPMIRROR_REGBIT_MIRROR                        0x02
#define OV02K10_FLIPMIRROR_REGBIT_FLIP                          0x0c

#define OV02K10_FINE_INTG_TIME_MIN                              0
#define OV02K10_FINE_INTG_TIME_MAX_MARGIN                       0
#define OV02K10_COARSE_INTG_TIME_MIN                            16
#define OV02K10_COARSE_INTG_TIME_MAX_MARGIN                     8

#define OV02K10_AGAIN_REAL_MIN              0x10
#define OV02K10_AGAIN_REAL_MAX              0xf8
#define OV02K10_AGAIN_FORMAT_REG            0x3509      //bit[4]
#define OV02K10_REAL_GAIN                   1
#define OV02K10_SENSOR_GAIN                 0

/* ov camera moudule config  ------------------------------------------------------------*/
#define OV_CAMERA_MODULE_CTRL_UPDT_GAIN             0x01
#define OV_CAMERA_MODULE_CTRL_UPDT_EXP_TIME         0x02
#define OV_CAMERA_MODULE_CTRL_UPDT_WB_TEMPERATURE   0x04
#define OV_CAMERA_MODULE_CTRL_UPDT_AUTO_WB          0x08
#define OV_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN        0x10
#define OV_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP         0x20
#define OV_CAMERA_MODULE_CTRL_UPDT_FOCUS_ABSOLUTE   0x40
#define OV_CAMERA_MODULE_CTRL_UPDT_PRESET_WB        0x80
#define OV_CAMERA_MODULE_CTRL_UPDT_VTS_VALUE        0x100
#define OV_CAMERA_MODULE_CTRL_UPDT_TEMPFPS_VALUE    0x200

/* timing  ------------------------------------------------------------*/
struct sensor_config_info_s              *ov02k10_sensorInfo = NULL;
struct isp_supplemental_sensor_mode_data *ov02k10_sensorModeData = NULL;

#define OV02K10_EXT_CLK 24000000
#define OV02K10_TEST_PATTERN_REG 0x5081

static struct rt_mutex spi_spi2apb_lock;
#define ov02k10_LOCK() rt_mutex_take(&spi_spi2apb_lock, RT_WAITING_FOREVER)
#define ov02k10_UNLOCK() rt_mutex_release(&spi_spi2apb_lock)

#define USE_25FPS

struct ov02k10_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
};


struct ov02k10_dev
{
    struct rk_camera_device parent;
    struct ov_camera_module cam_mod;
    rk_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];
#if 1//RT_USING_OV02K10_OPS
    struct ov02k10_ops *ops;
#endif
    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
    //struct rt_mutex mutex_lock;
};

struct otp_struct
{
    int flag; // bit[7]: info & wb
    int module_integrator_id;
    int lens_id;
    int rg_ratio;
    int bg_ratio;
};

/* ov camera config  ------------------------------------------------------------*/

#define PLTFRM_CAM_ITF_MIPI_CFG_EX(v, nb, br, mk, embedded_vc, embedded_type)\
    .itf_cfg = {\
        .type = PLTFRM_CAM_ITF_MIPI,\
            .cfg = {\
                .mipi = {\
                    .dphy_index = 0,\
                    .vc = v,\
                    .nb_lanes = nb,\
                    .bit_rate = br,\
                    .embedded_data_vc = embedded_vc,\
                    .embedded_data_type = embedded_type,\
                } \
            },\
        .mclk_hz = mk\
    }

#define PLTFRM_CAM_ITF_MIPI_CFG(v, nb, br, mk)\
    PLTFRM_CAM_ITF_MIPI_CFG_EX(v, nb, br, mk, 0xFF, 0)


typedef struct ov02k10_dev *rt_ov02k10_dev_t;

struct ov02k10_ops
{
    ret_err_t (*init)(struct rk_camera_device *dev);
    ret_err_t (*open)(struct rk_camera_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_camera_device *dev);
    ret_err_t (*control)(struct rk_camera_device *dev, int cmd, void *arg);
};

bool ov_camera_module_get_testpattern_en(
    struct ov02k10_dev *dev)
{
    return dev->cam_mod.testpattern_en;
}

struct HAL_ISP_Set_Exp_s
{
    uint32_t vts;
    uint32_t exposure;
    uint32_t gain;
    uint32_t gain_percent;
    bool cls_exp_before;
#if 0
    bool hdr_enable;
    uint32_t l_regtime;
    uint32_t l_reggain;
    uint32_t m_regtime;
    uint32_t m_reggain;
    uint32_t s_regtime;
    uint32_t s_reggain;
    float l_time;
    float l_gain;
    float m_time;
    float m_gain;
    float s_time;
    float s_gain;
#endif
};

static struct ov02k10_dev g_ov02k10;
static struct otp_struct g_otp_info = {0};

static ret_err_t ov02k10_g_vts(struct ov02k10_dev *dev, u32 *vts);

/* ======================================================================== */
/* Base sensor configs */
/* ======================================================================== */

/* MCLK:24MHz  1920x1080  30fps   mipi 4lane   960Mbps/lane */
static const struct ov02k10_sensor_reg ov02k10_init_tab_1920_1080[] =
{
//global setting
    {   0x302a, 0x00},
#ifdef USED_2LANE
    {   0x0103, 0x01},
    {   0x0109, 0x01},
    {   0x0104, 0x02},
    {   0x0102, 0x00},
    {   0x0305, 0x5c},
    {   0x0306, 0x00},
    {   0x0307, 0x00},
    {   0x030a, 0x01},
    {   0x0317, 0x09},
    {   0x0323, 0x07},
    {   0x0324, 0x01},
    {   0x0325, 0xb0},
    {   0x0327, 0x07},
    {   0x032c, 0x02},
    {   0x032d, 0x02},
    {   0x032e, 0x05},
    {   0x300f, 0x11},
    {   0x3012, 0x21},
    {   0x3026, 0x10},
    {   0x3027, 0x08},
    {   0x302d, 0x24},
    {   0x3103, 0x29},
    {   0x3106, 0x10},
    {   0x3400, 0x00},
    {   0x3406, 0x08},
    {   0x3408, 0x05},
    {   0x340c, 0x05},
    {   0x3425, 0x51},
    {   0x3426, 0x10},
    {   0x3427, 0x14},
    {   0x3428, 0x10},
    {   0x3429, 0x10},
    {   0x342a, 0x10},
    {   0x342b, 0x04},
    {   0x3504, 0x08},
    {   0x3508, 0x01},
    {   0x3509, 0x00},
    {   0x3544, 0x08},
    {   0x3548, 0x01},
    {   0x3549, 0x00},
    {   0x3584, 0x08},
    {   0x3588, 0x01},
    {   0x3589, 0x00},
    {   0x3601, 0x70},
    {   0x3604, 0xe3},
    {   0x3605, 0x7f},
    {   0x3606, 0x00},
    {   0x3608, 0xa8},
    {   0x360a, 0xd0},
    {   0x360b, 0x08},
    {   0x360e, 0xc8},
    {   0x360f, 0x66},
    {   0x3610, 0x81},
    {   0x3611, 0x89},
    {   0x3612, 0x4e},
    {   0x3613, 0xbd},
    {   0x362a, 0x0e},
    {   0x362b, 0x0e},
    {   0x362c, 0x0e},
    {   0x362d, 0x0e},
    {   0x362e, 0x0c},
    {   0x362f, 0x1a},
    {   0x3630, 0x32},
    {   0x3631, 0x64},
    {   0x3638, 0x00},
    {   0x3643, 0x00},
    {   0x3644, 0x00},
    {   0x3645, 0x00},
    {   0x3646, 0x00},
    {   0x3647, 0x00},
    {   0x3648, 0x00},
    {   0x3649, 0x00},
    {   0x364a, 0x04},
    {   0x364c, 0x0e},
    {   0x364d, 0x0e},
    {   0x364e, 0x0e},
    {   0x364f, 0x0e},
    {   0x3650, 0xff},
    {   0x3651, 0xff},
    {   0x3661, 0x07},
    {   0x3662, 0x02},
    {   0x3663, 0x20},
    {   0x3665, 0x12},
    {   0x3667, 0xd4},
    {   0x3668, 0x80},
    {   0x366f, 0x00},
    {   0x3670, 0xc7},
    {   0x3671, 0x08},
    {   0x3673, 0x2a},
    {   0x3681, 0x80},
    {   0x3700, 0x26},
    {   0x3701, 0x1e},
    {   0x3702, 0x25},
    {   0x3703, 0x28},
    {   0x3706, 0x3e},
    {   0x3707, 0x0a},
    {   0x3708, 0x36},
    {   0x3709, 0x55},
    {   0x370a, 0x00},
    {   0x370b, 0xa3},
    {   0x3714, 0x01},
    {   0x371b, 0x16},
    {   0x371c, 0x00},
    {   0x371d, 0x08},
    {   0x3756, 0x9b},
    {   0x3757, 0x9b},
    {   0x3762, 0x1d},
    {   0x376c, 0x00},
    {   0x3776, 0x05},
    {   0x3777, 0x22},
    {   0x3779, 0x60},
    {   0x377c, 0x48},
    {   0x3783, 0x02},
    {   0x3784, 0x06},
    {   0x3785, 0x0a},
    {   0x3790, 0x10},
    {   0x3793, 0x04},
    {   0x3794, 0x07},
    {   0x3796, 0x00},
    {   0x3797, 0x02},
    {   0x379c, 0x4d},
    {   0x37a1, 0x80},
    {   0x37bb, 0x88},
    {   0x37be, 0x01},
    {   0x37bf, 0x00},
    {   0x37c0, 0x01},
    {   0x37c7, 0x56},
    {   0x37ca, 0x21},
    {   0x37cc, 0x13},
    {   0x37cd, 0x90},
    {   0x37cf, 0x02},
    {   0x37d1, 0x3e},
    {   0x37d2, 0x00},
    {   0x37d3, 0xa3},
    {   0x37d5, 0x3e},
    {   0x37d6, 0x00},
    {   0x37d7, 0xa3},
    {   0x37d8, 0x01},
    {   0x37da, 0x00},
    {   0x37db, 0x00},
    {   0x37dc, 0x00},
    {   0x37dd, 0x00},
    {   0x3800, 0x00},
    {   0x3801, 0x00},
    {   0x3802, 0x00},
    {   0x3803, 0x04},
    {   0x3804, 0x07},
    {   0x3805, 0x8f},
    {   0x3806, 0x04},
    {   0x3807, 0x43},
    {   0x3808, 0x07},
    {   0x3809, 0x80},
    {   0x380a, 0x04},
    {   0x380b, 0x38},
    {   0x380c, 0x0b},
    {   0x380d, 0x40},
    {   0x380e, 0x09},
    {   0x380f, 0xc4},
    {   0x3811, 0x08},
    {   0x3813, 0x04},
    {   0x3814, 0x01},
    {   0x3815, 0x01},
    {   0x3816, 0x01},
    {   0x3817, 0x01},
    {   0x381c, 0x00},
    {   0x3820, 0x02},
    {   0x3821, 0x00},
    {   0x3822, 0x14},
    {   0x384c, 0x0b},
    {   0x384d, 0x40},
    {   0x3858, 0x0d},
    {   0x3865, 0x00},
    {   0x3866, 0xc0},
    {   0x3867, 0x00},
    {   0x3868, 0xc0},
    {   0x3900, 0x13},
    {   0x3940, 0x13},
    {   0x3980, 0x13},
    {   0x3c01, 0x11},
    {   0x3c05, 0x00},
    {   0x3c0f, 0x1c},
    {   0x3c12, 0x0d},
    {   0x3c14, 0x21},
    {   0x3c19, 0x01},
    {   0x3c21, 0x40},
    {   0x3c3b, 0x18},
    {   0x3c3d, 0xc9},
    {   0x3c55, 0xcb},
    {   0x3c5d, 0xcf},
    {   0x3c5e, 0xcf},
    {   0x3ce0, 0x00},
    {   0x3ce1, 0x00},
    {   0x3ce2, 0x00},
    {   0x3ce3, 0x00},
    {   0x3d8c, 0x70},
    {   0x3d8d, 0x10},
    {   0x4001, 0x2f},
    {   0x4033, 0x80},
    {   0x4008, 0x02},
    {   0x4009, 0x11},
    {   0x4004, 0x00},
    {   0x4005, 0x40},
    {   0x400a, 0x01},
    {   0x400b, 0x3c},
    {   0x400e, 0x40},
    {   0x4011, 0xbb},
    {   0x410f, 0x01},
    {   0x402e, 0x00},
    {   0x402f, 0x40},
    {   0x4030, 0x00},
    {   0x4031, 0x40},
    {   0x4032, 0x0f},
    {   0x4050, 0x00},
    {   0x4051, 0x07},
    {   0x4288, 0xcf},
    {   0x4289, 0x03},
    {   0x428a, 0x46},
    {   0x430b, 0x0f},
    {   0x430c, 0xfc},
    {   0x430d, 0x00},
    {   0x430e, 0x00},
    {   0x4314, 0x04},
    {   0x4500, 0x18},
    {   0x4501, 0x18},
    {   0x4504, 0x00},
    {   0x4507, 0x02},
    {   0x4508, 0x1a},
    {   0x4603, 0x00},
    {   0x4640, 0x62},
    {   0x4646, 0xaa},
    {   0x4647, 0x55},
    {   0x4648, 0x99},
    {   0x4649, 0x66},
    {   0x464d, 0x00},
    {   0x4654, 0x11},
    {   0x4655, 0x22},
    {   0x4800, 0x04},
    {   0x4810, 0xff},
    {   0x4811, 0xff},
    {   0x480e, 0x00},
    {   0x4813, 0x00},
    {   0x4825, 0x64},  // lpx_p
    {   0x4826, 0x49},  // hs_prepare min
    {   0x4827, 0x78},  // hs_prepare max
    {   0x4837, 0x0e},
    {   0x484b, 0x27},
    {   0x4d00, 0x4e},
    {   0x4d01, 0x0c},
    {   0x4d09, 0x4f},
    {   0x5000, 0x1f},
    {   0x5080, 0x00},
    {   0x50c0, 0x00},
    {   0x5100, 0x00},
    {   0x5200, 0x00},
    {   0x5201, 0x70},
    {   0x5202, 0x03},
    {   0x5203, 0x7f},
    {   0x5780, 0x53},
    {   0x5786, 0x01},
    {   0x0305, 0x50},
    {   0x4837, 0x10},
#ifdef USE_30FPS
    {   0x380e, 0x04},
    {   0x380f, 0xe2},
#endif

#ifdef USE_25FPS
    {   0x380e, 0x05},
    {   0x380f, 0xdc},
#endif
    {   0x3501, 0x04},
    {   0x3502, 0xda},
#else
    //4 lane
    {   0x0103, 0x01},
    {   0x0109, 0x01},
    {   0x0104, 0x02},
    {   0x0102, 0x00},
    {   0x0305, 0x5c},
    {   0x0306, 0x00},
    {   0x0307, 0x00},
    {   0x030a, 0x01},
    {   0x0317, 0x09},
    {   0x0323, 0x07},
    {   0x0324, 0x01},
    {   0x0325, 0xb0},
    {   0x0327, 0x07},
    {   0x032c, 0x02},
    {   0x032d, 0x02},
    {   0x032e, 0x05},
    {   0x300f, 0x11},
    {   0x3012, 0x41},
    {   0x3026, 0x10},
    {   0x3027, 0x08},
    {   0x302d, 0x24},
    {   0x3103, 0x29},
    {   0x3106, 0x10},
    {   0x3400, 0x00},
    {   0x3406, 0x08},
    {   0x3408, 0x05},
    {   0x340c, 0x05},
    {   0x3425, 0x51},
    {   0x3426, 0x10},
    {   0x3427, 0x14},
    {   0x3428, 0x10},
    {   0x3429, 0x10},
    {   0x342a, 0x10},
    {   0x342b, 0x04},
    {   0x3504, 0x08},
    {   0x3508, 0x01},
    {   0x3509, 0x00},
    {   0x3544, 0x08},
    {   0x3548, 0x01},
    {   0x3549, 0x00},
    {   0x3584, 0x08},
    {   0x3588, 0x01},
    {   0x3589, 0x00},
    {   0x3601, 0x70},
    {   0x3604, 0xe3},
    {   0x3605, 0x7f},
    {   0x3606, 0x00},
    {   0x3608, 0xa8},
    {   0x360a, 0xd0},
    {   0x360b, 0x08},
    {   0x360e, 0xc8},
    {   0x360f, 0x66},
    {   0x3610, 0x81},
    {   0x3611, 0x89},
    {   0x3612, 0x4e},
    {   0x3613, 0xbd},
    {   0x362a, 0x0e},
    {   0x362b, 0x0e},
    {   0x362c, 0x0e},
    {   0x362d, 0x0e},
    {   0x362e, 0x0c},
    {   0x362f, 0x1a},
    {   0x3630, 0x32},
    {   0x3631, 0x64},
    {   0x3638, 0x00},
    {   0x3643, 0x00},
    {   0x3644, 0x00},
    {   0x3645, 0x00},
    {   0x3646, 0x00},
    {   0x3647, 0x00},
    {   0x3648, 0x00},
    {   0x3649, 0x00},
    {   0x364a, 0x04},
    {   0x364c, 0x0e},
    {   0x364d, 0x0e},
    {   0x364e, 0x0e},
    {   0x364f, 0x0e},
    {   0x3650, 0xff},
    {   0x3651, 0xff},
    {   0x3661, 0x07},
    {   0x3662, 0x02},
    {   0x3663, 0x20},
    {   0x3665, 0x12},
    {   0x3667, 0xd4},
    {   0x3668, 0x80},
    {   0x366f, 0x00},
    {   0x3670, 0xc7},
    {   0x3671, 0x08},
    {   0x3673, 0x2a},
    {   0x3681, 0x80},
    {   0x3700, 0x26},
    {   0x3701, 0x1e},
    {   0x3702, 0x25},
    {   0x3703, 0x28},
    {   0x3706, 0x3e},
    {   0x3707, 0x0a},
    {   0x3708, 0x36},
    {   0x3709, 0x55},
    {   0x370a, 0x00},
    {   0x370b, 0xa3},
    {   0x3714, 0x01},
    {   0x371b, 0x16},
    {   0x371c, 0x00},
    {   0x371d, 0x08},
    {   0x3756, 0x9b},
    {   0x3757, 0x9b},
    {   0x3762, 0x1d},
    {   0x376c, 0x00},
    {   0x3776, 0x05},
    {   0x3777, 0x22},
    {   0x3779, 0x60},
    {   0x377c, 0x48},
    {   0x3783, 0x02},
    {   0x3784, 0x06},
    {   0x3785, 0x0a},
    {   0x3790, 0x10},
    {   0x3793, 0x04},
    {   0x3794, 0x07},
    {   0x3796, 0x00},
    {   0x3797, 0x02},
    {   0x379c, 0x4d},
    {   0x37a1, 0x80},
    {   0x37bb, 0x88},
    {   0x37be, 0x01},
    {   0x37bf, 0x00},
    {   0x37c0, 0x01},
    {   0x37c7, 0x56},
    {   0x37ca, 0x21},
    {   0x37cc, 0x13},
    {   0x37cd, 0x90},
    {   0x37cf, 0x02},
    {   0x37d1, 0x3e},
    {   0x37d2, 0x00},
    {   0x37d3, 0xa3},
    {   0x37d5, 0x3e},
    {   0x37d6, 0x00},
    {   0x37d7, 0xa3},
    {   0x37d8, 0x01},
    {   0x37da, 0x00},
    {   0x37db, 0x00},
    {   0x37dc, 0x00},
    {   0x37dd, 0x00},
    {   0x3800, 0x00},
    {   0x3801, 0x00},
    {   0x3802, 0x00},
    {   0x3803, 0x04},
    {   0x3804, 0x07},
    {   0x3805, 0x8f},
    {   0x3806, 0x04},
    {   0x3807, 0x43},
    {   0x3808, 0x07},
    {   0x3809, 0x80},
    {   0x380a, 0x04},
    {   0x380b, 0x38},
    {   0x380c, 0x05},
    {   0x380d, 0xa0},
    {   0x380e, 0x09},
    {   0x380f, 0xc4},
    {   0x3811, 0x08},
    {   0x3813, 0x04},
    {   0x3814, 0x01},
    {   0x3815, 0x01},
    {   0x3816, 0x01},
    {   0x3817, 0x01},
    {   0x381c, 0x00},
    {   0x3820, 0x02},
    {   0x3821, 0x00},
    {   0x3822, 0x14},
    {   0x384c, 0x02},
    {   0x384d, 0xd0},
    {   0x3858, 0x0d},
    {   0x3865, 0x00},
    {   0x3866, 0xc0},
    {   0x3867, 0x00},
    {   0x3868, 0xc0},
    {   0x3900, 0x13},
    {   0x3940, 0x13},
    {   0x3980, 0x13},
    {   0x3c01, 0x11},
    {   0x3c05, 0x00},
    {   0x3c0f, 0x1c},
    {   0x3c12, 0x0d},
    {   0x3c14, 0x21},
    {   0x3c19, 0x01},
    {   0x3c21, 0x40},
    {   0x3c3b, 0x18},
    {   0x3c3d, 0xc9},
    {   0x3c55, 0xcb},
    {   0x3c5d, 0xcf},
    {   0x3c5e, 0xcf},
    {   0x3ce0, 0x00},
    {   0x3ce1, 0x00},
    {   0x3ce2, 0x00},
    {   0x3ce3, 0x00},
    {   0x3d8c, 0x70},
    {   0x3d8d, 0x10},
    {   0x4001, 0x2f},
    {   0x4033, 0x80},
    {   0x4008, 0x02},
    {   0x4009, 0x11},
    {   0x4004, 0x00},
    {   0x4005, 0x40},
    {   0x400a, 0x01},
    {   0x400b, 0x3c},
    {   0x400e, 0x40},
    {   0x4011, 0xbb},
    {   0x410f, 0x01},
    {   0x402e, 0x00},
    {   0x402f, 0x40},
    {   0x4030, 0x00},
    {   0x4031, 0x40},
    {   0x4032, 0x0f},
    {   0x4050, 0x00},
    {   0x4051, 0x07},
    {   0x4288, 0xcf},
    {   0x4289, 0x03},
    {   0x428a, 0x46},
    {   0x430b, 0x0f},
    {   0x430c, 0xfc},
    {   0x430d, 0x00},
    {   0x430e, 0x00},
    {   0x4314, 0x04},
    {   0x4500, 0x18},
    {   0x4501, 0x18},
    {   0x4504, 0x00},
    {   0x4507, 0x02},
    {   0x4508, 0x1a},
    {   0x4603, 0x00},
    {   0x4640, 0x62},
    {   0x4646, 0xaa},
    {   0x4647, 0x55},
    {   0x4648, 0x99},
    {   0x4649, 0x66},
    {   0x464d, 0x00},
    {   0x4654, 0x11},
    {   0x4655, 0x22},
    {   0x4800, 0x04},
    {   0x4810, 0xff},
    {   0x4811, 0xff},
    {   0x480e, 0x00},
    {   0x4813, 0x00},
    {   0x4837, 0x0e},
    {   0x484b, 0x27},
    {   0x4d00, 0x4e},
    {   0x4d01, 0x0c},
    {   0x4d09, 0x4f},
    {   0x5000, 0x1f},
    {   0x5080, 0x00},
    {   0x50c0, 0x00},
    {   0x5100, 0x00},
    {   0x5200, 0x00},
    {   0x5201, 0x70},
    {   0x5202, 0x03},
    {   0x5203, 0x7f},
    {   0x5780, 0x53},
    {   0x5786, 0x01},
    {   0x0305, 0x50},
    {   0x4837, 0x10},
    {   0x3501, 0x04},
    {   0x3502, 0xda},
#endif
    {   REG_END, 0x00}

};

#ifdef USE_30FPS
static struct ov_camera_module_config ov02k10_configs[] =
{

    {
        .name = "1920x1080_30fps",
        .frm_fmt = {
            .width = 1920,
            .height = 1080,
            .code = V4L2_MBUS_FMT_SBGGR10_1X10
        },
        .frm_intrvl = {
            .interval = {
                .numerator = 1,
                .denominator = 30
            }
        },
        .auto_exp_enabled = false,
        .auto_gain_enabled = false,
        .auto_wb_enabled = false,
        .reg_table = (void *)ov02k10_init_tab_1920_1080,
        .reg_table_num_entries = ARRAY_SIZE(ov02k10_init_tab_1920_1080),
        .v_blanking_time_us = 4500,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        /* PLTFRM_CAM_ITF_MIPI_CFG(0, 2, 840, OV02K10_EXT_CLK) */
#ifdef USED_2LANE
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 2, 1000, OV02K10_EXT_CLK, 0, 0x30)
#else
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 4, 1350, OV02K10_EXT_CLK, 0, 0x30)
#endif

    }

};
#endif

#ifdef USE_25FPS
static struct ov_camera_module_config ov02k10_configs[] =
{

    {
        .name = "1920x1080_25fps",
        .frm_fmt = {
            .width = 1920,
            .height = 1080,
            .code = V4L2_MBUS_FMT_SBGGR10_1X10
        },
        .frm_intrvl = {
            .interval = {
                .numerator = 1,
                .denominator = 25
            }
        },
        .auto_exp_enabled = false,
        .auto_gain_enabled = false,
        .auto_wb_enabled = false,
        .reg_table = (void *)ov02k10_init_tab_1920_1080,
        .reg_table_num_entries = ARRAY_SIZE(ov02k10_init_tab_1920_1080),
        .v_blanking_time_us = 11000,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        /* PLTFRM_CAM_ITF_MIPI_CFG(0, 2, 840, OV02K10_EXT_CLK) */
#ifdef USED_2LANE
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 2, 1000, OV02K10_EXT_CLK, 0, 0x30)
#else
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 4, 1350, OV02K10_EXT_CLK, 0, 0x30)
#endif

    }

};
#endif

u32 ov02k10_gcd(u32 a, u32 b)
{
    u32 r;
    while (b != 0)
    {
        r = b;
        b = a % b;
        a = r;
    }
    return a;
}

static ret_err_t ov02k10_write_reg(rk_i2c_bus_device *bus,
                                   uint16_t reg, const uint8_t *data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msgs;
    int ret = 0;

    MACRO_ASSERT(bus != RK_NULL);

    for (int tries = 0; tries < 5; tries++)
    {
        send_buf[0] = ((reg >> 8) & 0xff);
        send_buf[1] = ((reg >> 0) & 0xff);
        send_buf[2] = *data;

        msgs.addr = OV02K10_7BIT_ADDRESS;
        msgs.flags = RT_I2C_WR;
        msgs.buf = send_buf;
        msgs.len = 3;
        ret = rt_i2c_transfer(bus, &msgs, 1);

        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
    }
    OV02K10_DEBUG("(%s):s0.0 i2c_bus error\n");
    return RET_SYS_ERROR;
}

static ret_err_t ov02k10_read_reg(rk_i2c_bus_device *bus,
                                  uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = (reg  & 0xff);
    msg[0].addr = OV02K10_7BIT_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = OV02K10_7BIT_ADDRESS;
    msg[1].flags = RT_I2C_RD;
    msg[1].len = 1;
    msg[1].buf = recv_buf;

    ret = rt_i2c_transfer(bus, msg, 2);
    *data = recv_buf[0];
    if (ret == 2)
    {
        ret = RET_SYS_EOK;
    }
    else
    {
        ret = RET_SYS_ERROR;
    }

    return ret;
}

static void ov02k10_write_multiple_reg(struct ov02k10_dev *dev,
                                       const struct ov02k10_sensor_reg *reg)
{
    uint16_t i;
    rk_i2c_bus_device *i2c_bus;

    MACRO_ASSERT(dev != RK_NULL);
    MACRO_ASSERT(reg != RK_NULL);

    i2c_bus = dev->i2c_bus;
    MACRO_ASSERT(i2c_bus != RK_NULL);

    for (i = 0;; i++)
    {
        if (reg[i].reg_addr == REG_END)
        {
            break;
        }

        if (reg[i].reg_addr == REG_DELAY)
        {
            HAL_DelayUs(reg[i].val);
        }
        else
        {
            ov02k10_write_reg(i2c_bus, reg[i].reg_addr, &reg[i].val);
        }
    }

}

static ret_err_t rt_ov02k10_detect_sensor(struct ov02k10_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    ret_err_t ret;
    uint32_t cnt = 0;
    u8 hsb = 0;
    u8 msb = 0;
    u8 lsb = 0;

    u32 chipid;
    rk_i2c_bus_device *bus = dev->i2c_bus;

    OV02K10_DEBUG("(%s) enter \n", __FUNCTION__);

RETRY:
    cnt++;
    rt_thread_mdelay(1);
    ret = ov02k10_read_reg(bus, OV02K10_REG_ID_H_ADDRESS, &hsb);
    if (ret != RET_SYS_EOK)
    {
        OV02K10_INFO(dev, "Warning: OV02K10 hsb i2c read failed !!!\n");
        ret = -RT_EINVAL;
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }

    ret |= ov02k10_read_reg(bus, OV02K10_REG_ID_M_ADDRESS, &msb);
    if (ret != RET_SYS_EOK)
    {
        OV02K10_INFO(dev, "Warning: OV02K10 msb i2c read failed !!!\n");
        ret = -RT_EINVAL;
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }

    ret |= ov02k10_read_reg(bus, OV02K10_REG_ID_L_ADDRESS, &lsb);
    if (ret != RET_SYS_EOK)
    {
        OV02K10_INFO(dev, "Warning: OV02K10 lsb i2c read failed !!!\n");
        ret = -RT_EINVAL;
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }

    if (ret == RET_SYS_EOK)
    {
        chipid = SENSOR_ID(hsb, msb, lsb);
        OV02K10_INFO(dev, "Info: OV02K10  hsb:0x%x, msb:0x%x, lsb:0x%x!\n", hsb, msb, lsb);
        if (chipid == OV02K10_SENSOR_ID)
        {
            OV02K10_INFO(dev, "Info: OV02K10 detected successfully !!!\n");
            ret = RET_SYS_EOK;
        }
        else
        {
            OV02K10_INFO(dev, "Info: OV02K10 detected failed,not match:0x%x != 0x%x !!!\n",
                         OV02K10_SENSOR_ID, chipid);
            ret = -RT_EINVAL;
            if (cnt > CAMERA_BOOT_RETRY_NUM)
                goto EXIT;
            goto RETRY;
        }
    }
    else
    {
        OV02K10_INFO(dev, "Warning: OV02K10 register read failed !!!\n");
        ret = -RT_EINVAL;
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }
EXIT:
    OV02K10_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}


static ret_err_t ov02k10_stream_on(struct ov02k10_dev *dev)
{
    u8 val;
    u8 data;
    ret_err_t ret;
    bool testpattern_en;
    OV02K10_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);

    data = 1;
    ret = ov02k10_write_reg(dev->i2c_bus, 0x0100, &data);
//  mutex_unlock(&cam_mod->lock);
    if (ret == RET_SYS_ERROR)
        goto err;

//  msleep(25);
    rt_thread_mdelay(25);
    return ret;
err:
    OV02K10_INFO(dev, "stream on failed with error (%d)\n", ret);
    return ret;
}

static ret_err_t ov02k10_stream_off(struct ov02k10_dev *dev)
{
    u8 val;
    rk_i2c_bus_device *i2c_bus;
    ret_err_t ret;

    OV02K10_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);
    i2c_bus = dev->i2c_bus;
    val = 0x0;
    //  mutex_lock(&cam_mod->lock);
    ret = ov02k10_write_reg(i2c_bus, 0x0100, &val);//to stop  camera
    //  mutex_unlock(&cam_mod->lock);


    if (ret == RET_SYS_ERROR)
        goto err;

    OV02K10_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
err:
    OV02K10_INFO(dev, "stream off failed with error (%d)\n", ret);
    return ret;
}

ret_err_t rk_ov02k10_init(struct rk_camera_device *dev)
{
    OV02K10_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct ov02k10_dev *ov02k10;
    ov02k10 = (struct ov02k10_dev *)&dev;
#if RT_USING_OV02K10_OPS
    if (ov02k10->ops->init)
    {
        return (ov02k10->ops->init(ov02k10));
    }
#else

    if (ov02k10 /* && camera*/)
    {
        ov02k10->i2c_bus = (rk_i2c_bus_device *)rt_device_find(ov02k10->i2c_name);
        if (!ov02k10->i2c_bus)
        {
            OV02K10_DEBUG("Warning:not find i2c source:%s !!!\n",
                          ov02k10->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            OV02K10_DEBUG("(%s):s0 find i2c_bus:%s\n", ov02k10->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    OV02K10_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

static ret_err_t rk_ov02k10_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    OV02K10_DEBUG("(%s) enter \n", __FUNCTION__);

    struct ov02k10_dev *ov02k10;
    struct rk_camera_device *camera;
    ret_err_t ret;
    u8 data;
    bool testpattern_en;

    MACRO_ASSERT(dev != RK_NULL);

    ov02k10 = (struct ov02k10_dev *)dev;
    camera = (struct rk_camera_device *)(&ov02k10->parent);

    cam_power_on(0);
    ov02k10_write_multiple_reg(ov02k10, ov02k10_init_tab_1920_1080);
    OV02K10_DEBUG(ov02k10, "active config = %s\n", ov02k10->cam_mod.active_config->name);

    ret = ov02k10_g_vts(ov02k10, &ov02k10->cam_mod.vts_min);

    if (ret == RET_SYS_ERROR)
        goto open_err;
    testpattern_en = ov_camera_module_get_testpattern_en(ov02k10);

//  mutex_lock(&cam_mod->lock);
    if (testpattern_en)
    {
        data = 0x01;
        ov02k10_write_reg(dev->i2c_bus, OV02K10_TEST_PATTERN_REG, &data);
    }

    OV02K10_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;

open_err:
    OV02K10_DEBUG(ov02k10, "stream on failed with error (%d)\n", ret);
    return ret;
}

/*--------------------------------------------------------------------------*/

ret_err_t rk_ov02k10_close(struct rk_camera_device *dev)
{
    struct ov02k10_dev *ov02k10 = (struct ov02k10_dev *)dev;;
    struct rk_camera_device *camera;
    ret_err_t ret;

    OV02K10_DEBUG(dev, "(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    cam_power_off(0);

    OV02K10_DEBUG(ov02k10, "(%s) exit \n", __FUNCTION__);
    return ret;
}

/*--------------------------------------------------------------------------*/

static ret_err_t ov02k10_g_vts(struct ov02k10_dev *dev, u32 *vts)
{
    u8 msb, lsb;
    ret_err_t ret;

    rk_i2c_bus_device *bus = dev->i2c_bus;
    ret = ov02k10_read_reg(bus, OV02K10_TIMING_VTS_HIGH_REG, &msb);

    if (ret == RET_SYS_EOK)
    {
        ret = ov02k10_read_reg(bus, OV02K10_TIMING_VTS_LOW_REG, &lsb);
        if (ret == RET_SYS_EOK)
        {
            *vts = msb & 0x00ff;
            *vts = (*vts << 8) | lsb;
            return RET_SYS_EOK;
        }
        else
            goto err;
    }
    else
        goto err;
err:
    OV02K10_INFO(dev, "Warning: OV02K10 get VTS failed !!!\n");
    return ret;
}

/*--------------------------------------------------------------------------*/

static ret_err_t ov02k10_s_vts(struct ov02k10_dev *dev,
                               u32 vts)
{
    ret_err_t ret = 0;

    if (vts < dev->cam_mod.vts_min)
        return RET_SYS_ERROR;
    u8 temp;
    temp = vts & 0xff;
    ret = ov02k10_write_reg(dev->i2c_bus, OV02K10_TIMING_VTS_LOW_REG, &temp);
    temp = (vts >> 8) & 0xff;
    ret |= ov02k10_write_reg(dev->i2c_bus, OV02K10_TIMING_VTS_HIGH_REG, &temp);
    if (ret == RET_SYS_EOK)
    {
        dev->cam_mod.vts_cur = vts;
    }
    else
    {
        OV02K10_INFO(dev, "Warning: set VTS failed with error (%d)!!!\n", ret);
    }

    return ret;
}

/*--------------------------------------------------------------------------*/

ret_err_t ov02k10_auto_adjust_fps_state(struct ov02k10_dev *dev, void *args)
{

    ret_err_t ret;
    bool *auto_adjust_fps = (bool *)args;
    dev->cam_mod.auto_adjust_fps = *auto_adjust_fps;
    OV02K10_INFO(dev, "state of ov02k10_auto_adjust_fps is : %d.", dev->cam_mod.auto_adjust_fps);
    return RET_SYS_EOK;
}

/*--------------------------------------------------------------------------*/
static int ov02k10_auto_adjust_fps(struct ov02k10_dev *dev,
                                   u32 exp_time)
{
    int ret = 0;
    u32 vts;
    struct ov_camera_module *cam_mod = &dev->cam_mod;

    if ((exp_time + OV02K10_COARSE_INTG_TIME_MAX_MARGIN)
            > cam_mod->vts_min)
        vts = exp_time + OV02K10_COARSE_INTG_TIME_MAX_MARGIN;
    else
        vts = cam_mod->vts_min;

    ret =  ov02k10_s_vts(dev, vts);

    if (ret != RET_SYS_EOK)
    {
        OV02K10_INFO(dev, "failed with error (%d)\n", ret);
    }
    else
    {
        OV02K10_INFO(dev, "updated vts = %d,vts_min=%d\n",
                     vts, cam_mod->vts_min);
        cam_mod->vts_cur = vts;
    }

    return ret;
}

/*--------------------------------------------------------------------------*/

static ret_err_t ov02k10_g_shutter(struct ov02k10_dev *dev,
                                   u32 *shutter)
{
    u8 shutter_2ND, shutter_1ST;
    ret_err_t ret;

    ret = ov02k10_read_reg(dev->i2c_bus,
                           OV02K10_AEC_PK_LONG_EXPO_2ND_REG, &shutter_2ND);

    ret |= ov02k10_read_reg(dev->i2c_bus,
                            OV02K10_AEC_PK_LONG_EXPO_1ST_REG, &shutter_1ST);

    if (ret == RET_SYS_EOK)
    {
        *shutter = ((shutter_2ND << 8) & 0xff) | (shutter_1ST & 0xff);
        return RET_SYS_EOK;
    }
    else
    {
        OV02K10_INFO(dev, "Warning: get shtter failed with error (%d)!!!\n", ret);
        return ret;
    }

}

/*--------------------------------------------------------------------------*/

static ret_err_t ov02k10_s_shutter(struct ov02k10_dev *dev, u32 *shutter)
{

    u8 temp;
    ret_err_t ret;

    temp = OV02K10_FETCH_2ND_BYTE_EXP(*shutter);
    ret = ov02k10_write_reg(dev->i2c_bus,
                            OV02K10_AEC_PK_LONG_EXPO_2ND_REG, &temp);

    temp = OV02K10_FETCH_1ST_BYTE_EXP(*shutter);
    ret |= ov02k10_write_reg(dev->i2c_bus,
                             OV02K10_AEC_PK_LONG_EXPO_1ST_REG, &temp);

    if (ret == RET_SYS_EOK)
    {
        return RET_SYS_EOK;
    }
    else
    {
        OV02K10_INFO(dev, "Warning: set shutter failed with error (%d)!!!\n", ret);
        return ret;
    }

}

/*--------------------------------------------------------------------------*/

static ret_err_t ov02k10_g_gain16(struct ov02k10_dev *dev,
                                  u32 *gain)
{
// read real gain, 16 = 1x
    u8 gain16_H;
    u8 gain16_L;
    ret_err_t ret;
    ret = ov02k10_read_reg(dev->i2c_bus, OV02K10_AEC_PK_LONG_GAIN_REG_H, &gain16_H);
    gain16_H = gain16_H & 0x0f;
    ret |= ov02k10_read_reg(dev->i2c_bus, OV02K10_AEC_PK_LONG_GAIN_REG_L, &gain16_L);
    gain16_L = gain16_L & 0xf0;
    *gain = (gain16_H << 4) | (gain16_L >> 4);
    if (ret == RET_SYS_EOK)
    {
        return RET_SYS_EOK;
    }
    else
    {
        OV02K10_INFO(dev, "Warning: get gain16 failed with error (%d)!!!\n", ret);
        return ret;
    }

}

/*--------------------------------------------------------------------------*/

static ret_err_t ov02k10_s_gain16(struct ov02k10_dev *dev, u32 gain16)
{

    u8 data;
    ret_err_t ret = 0;
    if (gain16 < OV02K10_AGAIN_REAL_MIN)
        gain16 = OV02K10_AGAIN_REAL_MIN;
    else if (gain16 > OV02K10_AGAIN_REAL_MAX)
        gain16 = OV02K10_AGAIN_REAL_MAX;

    data = (gain16 >> 4) & 0x0F;
    ret = ov02k10_write_reg(dev->i2c_bus,
                            OV02K10_AEC_PK_LONG_GAIN_REG_H, &data);

    data = (gain16 << 4) & 0xF0;
    ret |= ov02k10_write_reg(dev->i2c_bus,
                             OV02K10_AEC_PK_LONG_GAIN_REG_L, &data);

    if (ret == RET_SYS_EOK)
    {
        return RET_SYS_EOK;
    }
    else
    {
        OV02K10_INFO(dev, "Warning: set gain16 failed with error (%d)!!!\n", ret);
        return ret;
    }

}

static ret_err_t ov02k10_s_aec(struct ov02k10_dev *dev, void *args)
{

    ret_err_t ret;
    u32 vts;
    struct HAL_ISP_Set_Exp_s *aec = (struct HAL_ISP_Set_Exp_s *)args;
    struct ov_camera_module *cam_mod = &dev->cam_mod;
    rk_i2c_bus_device *bus = dev->i2c_bus;

    if (aec->gain < OV02K10_AGAIN_REAL_MIN)
        aec->gain = OV02K10_AGAIN_REAL_MIN;
    else if (aec->gain > OV02K10_AGAIN_REAL_MAX)
        aec->gain = OV02K10_AGAIN_REAL_MAX;

    ret = ov02k10_g_vts(dev, &vts);
    if (ret != RET_SYS_EOK)
    {
        OV02K10_INFO(dev, "get vts error.\n");
    }

    cam_mod->vts_cur = vts ;
    vts = vts == 0 ? cam_mod->vts_min : cam_mod->vts_cur;
    if (!cam_mod->auto_adjust_fps &&
            aec->exposure > vts - OV02K10_COARSE_INTG_TIME_MAX_MARGIN)
    {
        aec->exposure = vts - OV02K10_COARSE_INTG_TIME_MAX_MARGIN;
    }

    /* hold reg en */
    u8 reg_data;
    reg_data = OV02K10_AEC_GROUP_UPDATE_START_DATA;
    ret = ov02k10_write_reg(bus, OV02K10_AEC_GROUP_UPDATE_ADDRESS, &reg_data);

    if (ret == RET_SYS_EOK && cam_mod->auto_adjust_fps)
        ret |= ov02k10_auto_adjust_fps(dev, cam_mod->exp_config.exp_time);

    aec->gain = aec->gain & 0xfff;
    ret = ov02k10_s_gain16(dev, aec->gain);
    if (ret == RET_SYS_EOK)
    {
        cam_mod->exp_config.gain = aec->gain;
    }

    ret = ov02k10_s_shutter(dev, &(aec->exposure));
    if (ret == RET_SYS_EOK)
    {
        cam_mod->exp_config.exp_time = aec->exposure;
    }

    if (!cam_mod->auto_adjust_fps)
    {
        ret = ov02k10_s_vts(dev, cam_mod->exp_config.vts_value);
    }

    /* hold reg end */
    reg_data = OV02K10_AEC_GROUP_UPDATE_END_DATA;
    ret = ov02k10_write_reg(bus, OV02K10_AEC_GROUP_UPDATE_ADDRESS, &reg_data);
    reg_data = OV02K10_AEC_GROUP_UPDATE_END_LAUNCH;
    ret = ov02k10_write_reg(bus, OV02K10_AEC_GROUP_UPDATE_ADDRESS, &reg_data);
//  rt_thread_mdelay(34);
    return ret;
}

static ret_err_t ov02k10_g_aec(struct ov02k10_dev *dev, void *args)
{
    ret_err_t ret;
    struct HAL_ISP_Set_Exp_s *aec_value = (struct HAL_ISP_Set_Exp_s *)args;
    struct ov_camera_module *cam_mod = &dev->cam_mod;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    u32 gain;
    u32 shutter;
    u8 reg_data;

    ret = ov02k10_g_gain16(dev, &gain);
    if (ret == RET_SYS_EOK)
    {
        aec_value->gain = gain;
        cam_mod->exp_config.gain = aec_value->gain;
    }

    ret = ov02k10_g_shutter(dev, &shutter);
    if (ret == RET_SYS_EOK)
    {
        aec_value->exposure = shutter;
        cam_mod->exp_config.exp_time = aec_value->exposure;
    }

    return ret;
}

ret_err_t ov02k10_g_fmt(struct ov02k10_dev *dev, void *args)
{

    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    if (dev->cam_mod.active_config)
    {
        fmt->code = dev->cam_mod.active_config->frm_fmt.code;
        fmt->width = dev->cam_mod.active_config->frm_fmt.width;
        fmt->height = dev->cam_mod.active_config->frm_fmt.height;
        return RET_SYS_EOK;
    }

    OV02K10_INFO(dev, "no active config\n");
    return -1;
}

/* ======================================================================== */

ret_err_t ov02k10_g_sensor_frm_interval(struct ov02k10_dev *dev, void *args)
{
    struct v4l2_subdev_frame_interval *frm_inter =
        (struct v4l2_subdev_frame_interval *)args;
    if (dev->cam_mod.active_config)
    {
        frm_inter->interval.denominator =
            dev->cam_mod.active_config->frm_intrvl.interval.denominator;
        frm_inter->interval.numerator =
            dev->cam_mod.active_config->frm_intrvl.interval.numerator;

        return RET_SYS_EOK;
    }

    OV02K10_INFO(dev, "no active config\n");
    return RET_SYS_ERROR;
}

/* ======================================================================== */

static void ov_camera_module_set_active_config(
    struct ov02k10_dev *dev,
    struct ov_camera_module_config *new_config)
{

    if (RK_NULL == new_config)
    {
        dev->cam_mod.active_config = new_config;
        OV02K10_INFO(dev,
                     "no active config\n");
    }
    else
    {
        dev->cam_mod.ctrl_updt &= OV_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP |
                                  OV_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN |
                                  OV_CAMERA_MODULE_CTRL_UPDT_AUTO_WB;
        if (new_config->auto_exp_enabled !=
                dev->cam_mod.exp_config.auto_exp)
        {
            dev->cam_mod.ctrl_updt |=
                OV_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP;
            dev->cam_mod.exp_config.auto_exp =
                new_config->auto_exp_enabled;
        }
        if (new_config->auto_gain_enabled !=
                dev->cam_mod.exp_config.auto_gain)
        {
            dev->cam_mod.ctrl_updt |=
                OV_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN;
            dev->cam_mod.exp_config.auto_gain =
                new_config->auto_gain_enabled;
        }
        if (new_config->auto_wb_enabled !=
                dev->cam_mod.wb_config.auto_wb)
        {
            dev->cam_mod.ctrl_updt |=
                OV_CAMERA_MODULE_CTRL_UPDT_AUTO_WB;
            dev->cam_mod.wb_config.auto_wb =
                new_config->auto_wb_enabled;
        }
        if (new_config != dev->cam_mod.active_config)
        {
            dev->cam_mod.update_config = true;
            dev->cam_mod.active_config = new_config;
            OV02K10_INFO(dev,
                         "activating config '%s'\n",
                         dev->cam_mod.active_config->name);
        }
    }
}

/* ======================================================================== */

static struct ov_camera_module_config *ov_camera_module_find_config(
    struct ov02k10_dev *dev,
    struct v4l2_mbus_framefmt *fmt,
    struct v4l2_subdev_frame_interval *frm_intrvl)
{
    u32 i;
    u32 gcdiv;
    struct v4l2_subdev_frame_interval norm_interval;

    for (i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (NULL != frm_intrvl)
        {

            gcdiv = ov02k10_gcd(dev->cam_mod.custom.configs[i].
                                frm_intrvl.interval.numerator,
                                dev->cam_mod.custom.configs[i].
                                frm_intrvl.interval.denominator);

            norm_interval.interval.numerator =
                dev->cam_mod.custom.configs[i].
                frm_intrvl.interval.numerator / gcdiv;
            norm_interval.interval.denominator =
                dev->cam_mod.custom.configs[i].
                frm_intrvl.interval.denominator / gcdiv;

            if ((frm_intrvl->interval.numerator !=
                    norm_interval.interval.numerator) ||
                    (frm_intrvl->interval.denominator !=
                     norm_interval.interval.denominator))
                continue;
        }
        if (!(RK_NULL == fmt))
        {

            if ((dev->cam_mod.custom.configs[i].frm_fmt.width !=
                    fmt->width) ||
                    (dev->cam_mod.custom.configs[i].frm_fmt.height !=
                     fmt->height) ||
                    (dev->cam_mod.custom.configs[i].frm_fmt.code !=
                     fmt->code))
            {
                continue;
            }
        }
        OV02K10_INFO(dev,
                     "found matching config %s\n",
                     dev->cam_mod.custom.configs[i].name);
        return &dev->cam_mod.custom.configs[i];
    }
    OV02K10_INFO(dev, "no matching config found\n");

    return RK_NULL;
}
/* ======================================================================== */
ret_err_t ov02k10_s_fmt(struct ov02k10_dev *dev, void *args)
{
    struct ov_camera_module_config *config;
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    ret_err_t ret = RET_SYS_EOK;

    config = ov_camera_module_find_config(dev, fmt, NULL);

    if (config == NULL)
    {
        OV02K10_INFO(dev,
                     "format %dx%d, code 0x%04x, not supported\n",
                     fmt->width, fmt->height, fmt->code);
        ret = RET_SYS_ERROR;
        goto err;
    }
    dev->cam_mod.frm_fmt_valid = true;
    dev->cam_mod.frm_fmt = *fmt;
    OV02K10_INFO(dev, "fmt is found,%dx%d, fmt code 0x%04x\n", fmt->width, fmt->height, fmt->code);

    struct ov_camera_module_config *config_found = ov_camera_module_find_config(
                dev, fmt, &dev->cam_mod.frm_intrvl);

    if (dev->cam_mod.frm_intrvl_valid &&
            (RK_NULL != config_found))
    {
        ov_camera_module_set_active_config(dev, config_found);
    }
    else
    {
        ov_camera_module_set_active_config(dev, config);
    }

    return RET_SYS_EOK;
err:
    ret = RET_SYS_ERROR;
    OV02K10_INFO(dev,
                 "s_fmt failed with error %d\n", ret);
    return ret;
}

/* ======================================================================== */

ret_err_t ov02k10_g_v_blank_us(struct ov02k10_dev *dev, void *args)
{
    u32 *v_blanking_time_us = (u32 *)args;
    *v_blanking_time_us = dev->cam_mod.active_config->v_blanking_time_us;
    return RET_SYS_EOK;
}

/* ======================================================================== */

ret_err_t ov02k10_g_itf_cfg(struct ov02k10_dev *dev, void *args)
{

    struct pltfrm_cam_itf *itf_cfg = (struct pltfrm_cam_itf *)args;
    struct ov_camera_module_config *config;

    if (dev->cam_mod.custom.num_configs <= 0)
    {
        OV02K10_INFO(dev,
                     "cam_mod->custom.num_configs is NULL,"
                     "Get interface config failed!\n");
        return -RET_SYS_ERROR;
    }

    if (NULL == dev->cam_mod.active_config)
        config = &dev->cam_mod.custom.configs[0];
    else
        config = dev->cam_mod.active_config;

    *itf_cfg = config->itf_cfg;

    return RET_SYS_EOK;
}

/* ======================================================================== */

static ret_err_t ov02k10_g_timings(struct ov02k10_dev *dev,
                                   struct ov_camera_module_timings *timings)
{
    unsigned int vts;

    if (NULL == dev->cam_mod.active_config)
        goto err;

    *timings = dev->cam_mod.active_config->timings;

    vts = (!dev->cam_mod.vts_cur) ?
          timings->frame_length_lines :
          dev->cam_mod.vts_cur;

    if (dev->cam_mod.frm_intrvl_valid)
        timings->vt_pix_clk_freq_hz =
            dev->cam_mod.frm_intrvl.interval.denominator
            * vts
            * timings->line_length_pck;
    else
        timings->vt_pix_clk_freq_hz =
            dev->cam_mod.active_config->frm_intrvl.interval.denominator
            * vts
            * timings->line_length_pck;

    timings->frame_length_lines = vts;

    return RET_SYS_EOK;
err:
    OV02K10_INFO(dev, "cam_mod:failed with error (%d)\n", RET_SYS_EEMPTY);
    return RET_SYS_EEMPTY;
}

/* ======================================================================== */

ret_err_t ov02k10_g_sensor_mode(struct ov02k10_dev *dev, void *args)
{
    ret_err_t ret;
    static int frm_idx = 0; // flag of 1-st frame.

    struct ov_camera_module_timings ov_timings;

    int *args_data = (int *)args;

    ret = ov02k10_g_timings(dev, &ov_timings);

    if (RET_SYS_EOK != ret)
    {
        OV02K10_INFO(dev,
                     "failed with error %d\n", ret);
        return ret;
    }

    ov02k10_sensorModeData->isp_input_horizontal_start = 0;
    ov02k10_sensorModeData->isp_input_vertical_start   = 0;
    ov02k10_sensorModeData->isp_input_width   = 1920;
    ov02k10_sensorModeData->isp_input_height  = 1080;
    ov02k10_sensorModeData->isp_output_width  = 1920;
    ov02k10_sensorModeData->isp_output_height = 1080;

    ov02k10_sensorModeData->sensor_output_width   = ov_timings.sensor_output_width;
    ov02k10_sensorModeData->sensor_output_height  = ov_timings.sensor_output_height;
    ov02k10_sensorModeData->crop_horizontal_start = ov_timings.crop_horizontal_start;
    ov02k10_sensorModeData->crop_vertical_start = ov_timings.crop_vertical_start;
    ov02k10_sensorModeData->crop_horizontal_end = ov_timings.crop_horizontal_end;
    ov02k10_sensorModeData->crop_vertical_end   = ov_timings.crop_vertical_end;
    ov02k10_sensorModeData->line_length_pck     = ov_timings.line_length_pck;
    ov02k10_sensorModeData->frame_length_lines  = ov_timings.frame_length_lines;
    ov02k10_sensorModeData->vt_pix_clk_freq_hz  = ov_timings.vt_pix_clk_freq_hz;
    ov02k10_sensorModeData->binning_factor_x    = ov_timings.binning_factor_x;
    ov02k10_sensorModeData->binning_factor_y    = ov_timings.binning_factor_y;

    ov02k10_sensorModeData->coarse_integration_time_max_margin = ov_timings.coarse_integration_time_max_margin;
    ov02k10_sensorModeData->coarse_integration_time_min        = ov_timings.coarse_integration_time_min;
    ov02k10_sensorModeData->fine_integration_time_max_margin   = ov_timings.fine_integration_time_max_margin;
    ov02k10_sensorModeData->fine_integration_time_min          = ov_timings.fine_integration_time_min;

    ov02k10_sensorModeData->exposure_valid_frame[0] = dev->cam_mod.custom.exposure_valid_frame[0];
    ov02k10_sensorModeData->exposure_valid_frame[1] = dev->cam_mod.custom.exposure_valid_frame[1];
    ov02k10_sensorModeData->lane_num = 2;

    if (frm_idx == 0)
    {
        ov02k10_sensorModeData->exp_time = 182;
        ov02k10_sensorModeData->gain = 16;

        dev->cam_mod.exp_config.exp_time = 182;
        dev->cam_mod.exp_config.gain = 16;
    }
    else
    {
        if (dev->cam_mod.exp_config.exp_time)
        {
            ov02k10_sensorModeData->exp_time = dev->cam_mod.exp_config.exp_time;
            ov02k10_sensorModeData->exp_time_l = dev->cam_mod.exp_config.exp_time_l;
            ov02k10_sensorModeData->exp_time_s = dev->cam_mod.exp_config.exp_time_s;
        }
        else
        {
            ov02k10_sensorModeData->exp_time = ov_timings.exp_time;
            ov02k10_sensorModeData->exp_time_l = ov_timings.exp_time;
            ov02k10_sensorModeData->exp_time_s = ov_timings.exp_time;
        }
        if (dev->cam_mod.exp_config.gain)
        {
            ov02k10_sensorModeData->gain = dev->cam_mod.exp_config.gain;
            ov02k10_sensorModeData->gain_l = dev->cam_mod.exp_config.gain_l;
            ov02k10_sensorModeData->gain_s = dev->cam_mod.exp_config.gain_s;
        }
        else
        {
            ov02k10_sensorModeData->gain = ov_timings.gain;
            ov02k10_sensorModeData->gain_l = ov_timings.gain;
            ov02k10_sensorModeData->gain_s = ov_timings.gain;
        }
    }

    frm_idx++;

    if (frm_idx > 1000)
        frm_idx = 1;

    if (dev->cam_mod.active_config)
    {
        ov02k10_sensorModeData->max_exp_gain_h = dev->cam_mod.active_config->max_exp_gain_h;
        ov02k10_sensorModeData->max_exp_gain_l = dev->cam_mod.active_config->max_exp_gain_l;
    }
    else
    {
        ov02k10_sensorModeData->max_exp_gain_h = dev->cam_mod.custom.configs[0].max_exp_gain_h;
        ov02k10_sensorModeData->max_exp_gain_l = dev->cam_mod.custom.configs[0].max_exp_gain_l;
    }

    *args_data = (int)ov02k10_sensorModeData;

    return ret;
}

/* ======================================================================== */
//ret_err_t ov02k10_g_light_vol(struct ov02k10_dev *dev,void *args){
//  int vol = 0;
//  struct pltfrm_cam_ls *ls = (struct pltfrm_cam_ls *)args;
//
//  ls->val = PLTFRM_LS_INVAL;
//  if (!IS_ERR_OR_NULL(pdata->ls.ls_ctrl)) {
//      ret = lightsensor_vol_r(&pdata->ls.ls_ctrl->dev, &vol);
//      if (ret == 0) {
//          ls->light_vol = vol;
//          ls->val = PLTFRM_LS_HOLD;
//          if ((vol >= pdata->ls.range[0]) &&
//              (vol <= pdata->ls.range[1])){
//              ls->val = PLTFRM_LS_DAY;
//          }
//
//          if ((vol >= pdata->ls.range[2]) &&
//              (vol <= pdata->ls.range[3])){
//              ls->val = PLTFRM_LS_NIGHT;
//          }
//      }
//  }
//}
/* ======================================================================== */

static int ov02k10_filltimings(struct ov_camera_module_custom_config *custom)
{
    int i, j;
    struct ov_camera_module_config *config;
    struct ov_camera_module_timings *timings;
    struct ov_camera_module_reg *reg_table;
    int reg_table_num_entries;

    for (i = 0; i < custom->num_configs; i++)
    {
        config = &custom->configs[i];
        reg_table = config->reg_table;
        reg_table_num_entries = config->reg_table_num_entries;
        timings = &config->timings;

        for (j = 0; j < reg_table_num_entries; j++)
        {
            switch (reg_table[j].reg)
            {
            case OV02K10_TIMING_VTS_HIGH_REG:
                timings->frame_length_lines = reg_table[j].val << 8;
                break;
            case OV02K10_TIMING_VTS_LOW_REG:
                timings->frame_length_lines |= reg_table[j].val;
                break;
            case OV02K10_TIMING_HTS_HIGH_REG:
                timings->line_length_pck =
                    (reg_table[j].val << 8);
                break;
            case OV02K10_TIMING_HTS_LOW_REG:
                timings->line_length_pck |= reg_table[j].val;
                break;
            case OV02K10_TIMING_X_INC:
                timings->binning_factor_x = ((reg_table[j].val >> 4) + 1) / 2;
                if (timings->binning_factor_x == 0)
                    timings->binning_factor_x = 1;
                break;
            case OV02K10_TIMING_Y_INC:
                timings->binning_factor_y = ((reg_table[j].val >> 4) + 1) / 2;
                if (timings->binning_factor_y == 0)
                    timings->binning_factor_y = 1;
                break;
            case OV02K10_HORIZONTAL_START_HIGH_REG:
                timings->crop_horizontal_start = reg_table[j].val << 8;
                break;
            case OV02K10_HORIZONTAL_START_LOW_REG:
                timings->crop_horizontal_start |= reg_table[j].val;
                break;
            case OV02K10_VERTICAL_START_HIGH_REG:
                timings->crop_vertical_start = reg_table[j].val << 8;
                break;
            case OV02K10_VERTICAL_START_LOW_REG:
                timings->crop_vertical_start |= reg_table[j].val;
                break;
            case OV02K10_HORIZONTAL_END_HIGH_REG:
                timings->crop_horizontal_end = reg_table[j].val << 8;
                break;
            case OV02K10_HORIZONTAL_END_LOW_REG:
                timings->crop_horizontal_end |= reg_table[j].val;
                break;
            case OV02K10_VERTICAL_END_HIGH_REG:
                timings->crop_vertical_end = reg_table[j].val << 8;
                break;
            case OV02K10_VERTICAL_END_LOW_REG:
                timings->crop_vertical_end |= reg_table[j].val;
                break;
            case OV02K10_HORIZONTAL_OUTPUT_SIZE_HIGH_REG:
                timings->sensor_output_width = reg_table[j].val << 8;
                break;
            case OV02K10_HORIZONTAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_width |= reg_table[j].val;
                break;
            case OV02K10_VERTICAL_OUTPUT_SIZE_HIGH_REG:
                timings->sensor_output_height = reg_table[j].val << 8;
                break;
            case OV02K10_VERTICAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_height |= reg_table[j].val;
                break;
            }
        }

        timings->vt_pix_clk_freq_hz = config->frm_intrvl.interval.denominator
                                      * timings->frame_length_lines
                                      * timings->line_length_pck;

        timings->coarse_integration_time_min = OV02K10_COARSE_INTG_TIME_MIN;
        timings->coarse_integration_time_max_margin =
            OV02K10_COARSE_INTG_TIME_MAX_MARGIN;

        /* OV Sensor do not use fine integration time. */
        timings->fine_integration_time_min = OV02K10_FINE_INTG_TIME_MIN;
        timings->fine_integration_time_max_margin =
            OV02K10_FINE_INTG_TIME_MAX_MARGIN;
    }

    return 0;
}

ret_err_t ov02k10_g_sensor_configInfo(struct ov02k10_dev *dev, void *args)
{

    ret_err_t ret;
    int *args_data = (int *)args;

    ov02k10_sensorInfo->config_num = dev->cam_mod.custom.num_configs;
    for (u8 i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (i >= SENSOR_CONFIG_NUM)
            break;
        ov02k10_sensorInfo->sensor_fmt[i] =
            pltfrm_camera_module_pix_fmt2csi2_dt(dev->cam_mod.custom.configs[i].frm_fmt.code);
        ov02k10_sensorInfo->reso[i].width  = dev->cam_mod.custom.configs[i].frm_fmt.width;
        ov02k10_sensorInfo->reso[i].height = dev->cam_mod.custom.configs[i].frm_fmt.height;
    }

    *args_data = (int)ov02k10_sensorInfo;

    return 0;
}

ret_err_t ov02k10_g_sensor_iq_info(struct ov02k10_dev *dev, void *args)
{
    ret_err_t ret = 0;
    struct rk_camera_iq_info *info = (struct rk_camera_iq_info *)args;
    info->addr = (void *)CamIqData;
    return ret;
}

static ret_err_t ov02k10_s_flipMirror(struct ov02k10_dev *dev, void *args)
{
    ret_err_t ret = 0;
    u8 data;
    u8 *state = (u8 *)args;

    switch (*state)
    {
    case OV02K10_FLIPMIRROR_STATE_NORMAL:
    {
        data &= ~OV02K10_FLIPMIRROR_REGBIT_FLIP;
        data &= ~OV02K10_FLIPMIRROR_REGBIT_MIRROR;
        ret |= ov02k10_write_reg(dev->i2c_bus, OV02K10_FLIPMIRROR_REGADDR, &data);
    }
    break;
    case OV02K10_FLIPMIRROR_STATE_FLIPMIRROR:
    {
        data |= OV02K10_FLIPMIRROR_REGBIT_MIRROR;
        data |= OV02K10_FLIPMIRROR_REGBIT_FLIP;
        ret |= ov02k10_write_reg(dev->i2c_bus, OV02K10_FLIPMIRROR_REGADDR, &data);
    }
    break;
    case OV02K10_FLIPMIRROR_STATE_MIRROR:
    {
        data |= OV02K10_FLIPMIRROR_REGBIT_MIRROR;
        data &= ~OV02K10_FLIPMIRROR_REGBIT_FLIP;
        ret |= ov02k10_write_reg(dev->i2c_bus, OV02K10_FLIPMIRROR_REGADDR, &data);
    }
    break;
    case OV02K10_FLIPMIRROR_STATE_FLIP:
    {
        data |= OV02K10_FLIPMIRROR_REGBIT_FLIP;
        data &= ~OV02K10_FLIPMIRROR_REGBIT_MIRROR;
        ret |= ov02k10_write_reg(dev->i2c_bus, OV02K10_FLIPMIRROR_REGADDR, &data);
    }
    break;
    default:
    {
        OV02K10_INFO(dev, "%d is wrong flip_mirror state!!!\n", *state);
        ret = RET_SYS_ERROR;
    }
    break;
    }

    if (IS_ERR_VALUE(ret))
    {
        OV02K10_INFO(dev, "s_flipMirror:failed with error (%d)\n", ret);
    }

    return ret;
}

ret_err_t rk_ov02k10_control(struct rk_camera_device *dev,
                             int cmd,
                             void *args)
{
    OV02K10_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct ov02k10_dev *ov02k10;

    MACRO_ASSERT(dev != RK_NULL);
    ov02k10 = (struct ov02k10_dev *)dev;
#if RT_USING_OV02K10_OPS
    return (ov02k10->ops->control(ov02k10, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_ov02k10_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        ret = ov02k10_stream_on(ov02k10);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        ret = ov02k10_stream_off(ov02k10);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_AEC:
    {
        ret = ov02k10_g_aec(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_AEC:
    {
        ret = ov02k10_s_aec(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = ov02k10_g_fmt(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = ov02k10_s_fmt(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_BLANK:
    {
        ret = ov02k10_g_v_blank_us(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_ITF_CFG:
    {
        ret = ov02k10_g_itf_cfg(ov02k10, args);
    }
    break;

    case RK_VIDIOC_SENSOR_MODE_DATA:
    {
        ret = ov02k10_g_sensor_mode(ov02k10, args);
    }
    break;

    case RK_V4L2_CID_AUTO_FPS:
    {
        ret = ov02k10_auto_adjust_fps_state(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_IQ_INFO:
    {
        ret = ov02k10_g_sensor_iq_info(ov02k10, args);
    }
    break;

    case RK_VIDIOC_SENSOR_CONFIGINFO:
    {
        ret = ov02k10_g_sensor_configInfo(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_FRMINTVAL:
    {
        ret = ov02k10_g_sensor_frm_interval(ov02k10, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = ov02k10_s_flipMirror(ov02k10, args);
    }
    break;

    default:
        break;
    }
#endif
    OV02K10_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

struct rk_camera_ops rk_ov02k10_ops =
{
    rk_ov02k10_init,
    rk_ov02k10_open,
    rk_ov02k10_close,
    rk_ov02k10_control
};

/*--------------------------------------------------------------------------*/

static struct ov_camera_module_custom_config ov02k10_custom_config =
{

    .configs = ov02k10_configs,
    .num_configs = ARRAY_SIZE(ov02k10_configs),
    .power_up_delays_ms = {1, 1, 0},
    /*
     * 0: Exposure time valid fileds;
     * 1: Exposure gain valid fileds;
     * (2 fileds == 1 frames)
     */
    .exposure_valid_frame = {4, 4},

};

/*--------------------------------------------------------------------------*/

int rk_camera_ov02k10_init(void)
{
    ret_err_t ret;
    rk_device *device = &g_ov02k10.dev;
    struct ov02k10_dev *instance = &g_ov02k10;
    struct rk_camera_device *camera = &instance->parent;
    camera->ops = &rk_ov02k10_ops;

    rt_strncpy(instance->name, OV02K10_DEVICE_NAME, rt_strlen(OV02K10_DEVICE_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

#if OV02K10_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        OV02K10_INFO(instance, "Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        OV02K10_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

    if (ov02k10_sensorInfo == NULL)
    {
        ov02k10_sensorInfo = (struct sensor_config_info_s *)rt_malloc(sizeof(struct sensor_config_info_s));
        if (ov02k10_sensorInfo != NULL)
        {
            rt_memset(ov02k10_sensorInfo, 0x00, sizeof(struct sensor_config_info_s));
        }
        else
        {
            OV02K10_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    if (ov02k10_sensorModeData == NULL)
    {
        ov02k10_sensorModeData = (struct isp_supplemental_sensor_mode_data *)rt_malloc(sizeof(struct isp_supplemental_sensor_mode_data));
        if (ov02k10_sensorModeData != NULL)
        {
            rt_memset(ov02k10_sensorModeData, 0x00, sizeof(struct isp_supplemental_sensor_mode_data));
        }
        else
        {
            OV02K10_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    cam_power_init(0);
    cam_power_on(0);
    ret = rt_ov02k10_detect_sensor(instance);
    cam_power_off(0);

    if (ret != RET_SYS_EOK)
        goto ERR;

    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, instance->name, rt_strlen(OV02K10_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);

    instance->cam_mod.custom = ov02k10_custom_config;
    instance->cam_mod.active_config = ov02k10_configs;
    ret = ov02k10_filltimings(&instance->cam_mod.custom);
    instance->cam_mod.active_config->line_length_pck = 2880;
    instance->cam_mod.active_config->frame_length_lines = 2500;
    instance->cam_mod.custom.configs->line_length_pck = 2880;
    instance->cam_mod.custom.configs->frame_length_lines = 2500;
    instance->cam_mod.exp_config.gain_percent = 100;

    return 0;
ERR:
    OV02K10_DEBUG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

void ov02k10_detect(void)
{
    struct ov02k10_dev *instance = &g_ov02k10;

    OV02K10_DEBUG("start to detect OV02K10 for testing \n");
    OV02K10_DEBUG("dev name:%s\n", instance->name);
    OV02K10_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        OV02K10_DEBUG("Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        OV02K10_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_ov02k10_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(ov02k10_detect, check g_ov02k10 is available or not);
#endif
#endif

#endif

