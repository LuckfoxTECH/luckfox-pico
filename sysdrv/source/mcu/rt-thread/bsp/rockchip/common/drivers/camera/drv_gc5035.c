/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_gc5035.c
  * @version V0.0.1
  * @brief   sensor driver for rk625
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
#endif

#ifdef CAMERA_MODULE_COMPILED

#if defined(__RT_THREAD__)
#include "board.h"
#ifdef SOC_SWALLOW
#include "board_cam.h"
#endif
#elif defined(__RK_OS__)
#include "driver/drv_clock.h"
#endif

#if defined(RT_USING_GC5035) || defined(CONFIG_DRIVER_GC5035)
#define GC5035_MODULE_COMPILED
#else
#undef GC5035_MODULE_COMPILED
#endif

#endif


#ifdef GC5035_MODULE_COMPILED

#define GC5035_DEBUG_INFO 0
#include "gc_camera_module.h"
#include "cam_iq_data_gc5035_v1.1_a2.h"


#if GC5035_DEBUG_INFO
#include <stdio.h>
#define GC5035_DEBUG(...)               rk_kprintf("[GC5035]:");rk_kprintf(__VA_ARGS__)
#else
#define GC5035_DEBUG(...)
#endif


#define GC5035_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define RT_USING_GC5035_OPS 0

#define GC5035_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME                "i2c0"
#define GC5035_7BIT_ADDRESS         (0x37)
#define GC5035_REG_ID_H_ADDRESS     (0xF0)
#define GC5035_REG_ID_L_ADDRESS     (0xF1)
#define GC5035_SENSOR_ID            (0x5035)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define GC5035_PIN_CTRL_ENABLE          1
#define GC5035_I2C_DEBUG_ENABLE         1
#define GC5035_TESTPATTERN_ENABLE       0

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

#define RK_NULL                         RT_NULL
#define MACRO_ASSERT(EX)                RT_ASSERT(EX)
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

/* AEC  -----------------------------------------------------------------------*/

#define GC5035_FETCH_GAIN(VAL)                  (VAL & 0xFF)    /* gain[7:0] */
#define GC5035_AEC_ANALOG_GAIN_REG              0xb6            /* Bits 0 -7 */
#define GC5035_AEC_DIGITAL_INT_GAIN_REG         0xb1            /* Bits 0 -3 */
#define GC5035_AEC_DIGITAL_FRAC_GAIN_REG        0xb2            /* Bits 2 -7 */

#define GC5035_AEC_EXPO_HIGH_REG                0x03            /* Exposure Bits 8-12 */
#define GC5035_AEC_EXPO_LOW_REG                 0x04            /* Exposure Bits 0-7 */

#define GC5035_FETCH_HIGH_BYTE_EXP(VAL)         ((VAL >> 8) & 0x0F)     /* 5 Bits */
#define GC5035_FETCH_LOW_BYTE_EXP(VAL)          (VAL & 0xFF)            /* 8 Bits */

#define GC5035_TIMING_VTS_HIGH_REG              0x41
#define GC5035_TIMING_VTS_LOW_REG               0x42
#define GC5035_TIMING_HTS_HIGH_REG              0x05
#define GC5035_TIMING_HTS_LOW_REG               0x06
#define GC5035_TIMING_WINV_HIGH_REG             0x0d
#define GC5035_TIMING_WINV_LOW_REG              0x0e
#define GC5035_TIMING_WINH_HIGH_REG             0x0f
#define GC5035_TIMING_WINH_LOW_REG              0x10
#define GC5035_FINE_INTG_TIME_MIN               0
#define GC5035_FINE_INTG_TIME_MAX_MARGIN        0
#define GC5035_COARSE_INTG_TIME_MIN             1
#define GC5035_COARSE_INTG_TIME_MAX_MARGIN      4
#define GC5035_HORIZONTAL_OUTPUT_SIZE_HIGH_REG  0x97
#define GC5035_HORIZONTAL_OUTPUT_SIZE_LOW_REG   0x98
#define GC5035_VERTICAL_OUTPUT_SIZE_HIGH_REG    0x95
#define GC5035_VERTICAL_OUTPUT_SIZE_LOW_REG     0x96
#define GC5035_H_WIN_OFF_HIGH_REG               0x93
#define GC5035_H_WIN_OFF_LOW_REG                0x94
#define GC5035_V_WIN_OFF_HIGH_REG               0x91
#define GC5035_V_WIN_OFF_LOW_REG                0x92

#define DD_PARAM_QTY_5035           200
#define INFO_ROM_START_5035         0x08
#define INFO_WIDTH_5035             0x08
#define WB_ROM_START_5035           0x88
#define WB_WIDTH_5035               0x05
#define GOLDEN_ROM_START_5035       0xe0
#define GOLDEN_WIDTH_5035           0x05
#define WINDOW_WIDTH                0x0a30
#define WINDOW_HEIGHT               0x079c
/* SENSOR MIRROR FLIP INFO */
#define GC5035_MIRROR_FLIP_ENABLE   1
#if GC5035_MIRROR_FLIP_ENABLE
#define GC5035_MIRROR               0x82
#define GC5035_RSTDUMMY1            0x02
#define GC5035_RSTDUMMY2            0x7c
//#define GC5035_RSTDUMMY1          0x03
//#define GC5035_RSTDUMMY2          0xfc
#else
#define GC5035_MIRROR               0x80
#define GC5035_RSTDUMMY1            0x02
#define GC5035_RSTDUMMY2            0x7c
#endif

#define GC5035_REG_CTRL_MODE        0x3e
#define GC5035_MODE_SW_STANDBY      0x01
#define GC5035_MODE_STREAMING       0x91

#define GC5035_REG_SET_PAGE         0xfe
#define GC5035_SET_PAGE_ZERO         0x00

#define FASTBOOT

#define GC5035_OTP_DATA_LENGTH             1024

/* OTP MODULE INFO PARAMETERS */
#define GC5035_OTP_MODULE_FLAG_OFFSET      0x1f10
#define GC5035_OTP_MODULE_DATA_OFFSET      0x1f18
#define GC5035_OTP_MODULE_DATA_SIZE        6

#define GC5035_OTP_GET_2BIT_FLAG(flag, bit)   ((flag >> bit) & 0x03)
#define GC5035_OTP_GET_OFFSET(size)           (size << 3)


/* OTP FLAG TYPE */
#define GC5035_OTP_FLAG_EMPTY              0x00
#define GC5035_OTP_FLAG_VALID              0x01
#define GC5035_OTP_FLAG_INVALID            0x02
#define GC5035_OTP_FLAG_INVALID2           0x03

/* OTP WB PARAMETERS */
#define GC5035_OTP_WB_FLAG_OFFSET          0x1f78
#define GC5035_OTP_WB_DATA_OFFSET          0x1f80
#define GC5035_OTP_WB_DATA_SIZE            4
#define GC5035_OTP_GOLDEN_DATA_OFFSET      0x1fc0
#define GC5035_OTP_GOLDEN_DATA_SIZE        4
#define GC5035_OTP_WB_CAL_BASE             0x0800
#define GC5035_OTP_WB_GAIN_BASE            0x0400

/* OTP DPC PARAMETERS */
#define GC5035_OTP_DPC_FLAG_OFFSET         0x0068
#define GC5035_OTP_DPC_TOTAL_NUMBER_OFFSET 0x0070
#define GC5035_OTP_DPC_ERROR_NUMBER_OFFSET 0x0078

/* OTP REGISTER UPDATE PARAMETERS */
#define GC5035_OTP_REG_FLAG_OFFSET         0x0880
#define GC5035_OTP_REG_DATA_OFFSET         0x0888
#define GC5035_OTP_REG_MAX_GROUP           5
#define GC5035_OTP_REG_BYTE_PER_GROUP      5
#define GC5035_OTP_REG_REG_PER_GROUP       2
#define GC5035_OTP_REG_BYTE_PER_REG        2
#define GC5035_OTP_REG_DATA_SIZE           (GC5035_OTP_REG_MAX_GROUP * GC5035_OTP_REG_BYTE_PER_GROUP)
#define GC5035_OTP_REG_REG_SIZE            (GC5035_OTP_REG_MAX_GROUP * GC5035_OTP_REG_REG_PER_GROUP)

#define GC5035_FLIPMIRROR_STATE_NORMAL          0
#define GC5035_FLIPMIRROR_STATE_FLIPMIRROR      1
#define GC5035_FLIPMIRROR_STATE_MIRROR          2
#define GC5035_FLIPMIRROR_STATE_FLIP            3
#define GC5035_FLIPMIRROR_REGADDR               0x17
#define GC5035_FLIPMIRROR_REGVAL_NORMAL         0
#define GC5035_FLIPMIRROR_REGVAL_FLIPMIRROR     0x03
#define GC5035_FLIPMIRROR_REGVAL_MIRROR         0x01
#define GC5035_FLIPMIRROR_REGVAL_FLIP           0x02

#define GC5035_OTP_CHECK_1BIT_FLAG(flag, bit) (((flag >> bit) & 0x01) == GC5035_OTP_FLAG_VALID)
/* timing  ------------------------------------------------------------*/

#define GC5035_EXT_CLK 24000000
#define USE_25FPS


//#define ov5695_TEST_PATTERN_REG 0x4503

static const u32 GC5035_AGC_Param[17][2] =
{
    {64, 0},
    {76, 1},
    {90, 2},
    {106, 3},
    {126, 8},
    {150, 9},
    {179, 10},
    {211, 11},
    {250, 12},
    {301, 13},
    {358, 14},
    {427, 15},
    {499, 16},
    {589, 17},
    {704, 18},
    {830, 19},
    {998, 20},
};

static struct rt_mutex spi_spi2apb_lock;
#define GC5035_LOCK() rt_mutex_take(&spi_spi2apb_lock, RT_WAITING_FOREVER)
#define GC5035_UNLOCK() rt_mutex_release(&spi_spi2apb_lock)

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

struct gc5035_dev
{
    struct rk_camera_device parent;
    struct gc_camera_module cam_mod;
    rk_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];

    struct gc5035_ops *ops;

    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
    //struct rt_mutex mutex_lock;
};
typedef struct gc5035_dev *rt_gc5035_dev_t;

struct gc5035_ops
{
    ret_err_t (*init)(struct rk_camera_device *dev);
    ret_err_t (*open)(struct rk_camera_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_camera_device *dev);
    ret_err_t (*control)(struct rk_camera_device *dev, int cmd, void *arg);
};

struct gc5035_sensor_reg
{
    u8 reg_addr;
    u8 val;
};

struct gc5035_reg_t
{
    u8 page;
    u8 addr;
    u8 value;
};
/* REGISTER UPDATE STRUCTURE */
struct gc5035_reg_update_t
{
    u8 cnt;
    struct gc5035_reg_t reg[GC5035_OTP_REG_REG_SIZE];
};


struct otp_struct
{
    u32 flag; //bit[7]: info bit[6]:wb bit[3]:dd
    u32 module_id;
    u32 lens_id;
    u32 year;
    u32 month;
    u32 day;
    u32 rg_ratio;
    u32 bg_ratio;
    u32 golden_rg;
    u32 golden_bg;
    u32 total_num;
    struct gc5035_reg_update_t regs;
};

bool ov_camera_module_get_testpattern_en(
    struct gc5035_dev *dev)
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


static struct gc5035_dev g_gc5035;
static struct otp_struct *g_otp_info = NULL;
struct sensor_config_info_s              *gc5035_sensorInfo = NULL;
struct isp_supplemental_sensor_mode_data *gc5035_sensorModeData = NULL;

static const struct gc5035_sensor_reg gc5035_init_tab_2592_1944_2lane[] =
{
    {0xfc, 0x01},
    {0xf4, 0x40},
    {0xf5, 0xe9},
    {0xf6, 0x14},
    {0xf8, 0x49},
    {0xf9, 0x82},
    {0xfa, 0x00},
    {0xfc, 0x81},
    {0xfe, 0x00},
    {0x36, 0x01},
    {0xd3, 0x87},
    {0x36, 0x00},
    {0x33, 0x00},
    {0xfe, 0x03},
    {0x01, 0xe7},
    {0xf7, 0x01},
    {0xfc, 0x8f},
    {0xfc, 0x8f},
    {0xfc, 0x8e},
    {0xfe, 0x00},
    {0xee, 0x30},
    {0x87, 0x18},
    {0xfe, 0x01},
    {0x8c, 0x90},
    {0xfe, 0x00},

    {0xfe, 0x00},
    {0x05, 0x03},
    {0x06, 0x3c},
    {0x9d, 0x0c},
    {0x09, 0x00},
    {0x0a, 0x04},
    {0x0b, 0x00},
    {0x0c, 0x03},
    {0x0d, 0x07},
    {0x0e, 0xa8},
    {0x0f, 0x0a},
    {0x10, 0x30},
    {0x11, 0x02},
    {0x17, GC5035_MIRROR},
    {0x19, 0x05},
    {0xfe, 0x02},
    {0x30, 0x03},
    {0x31, 0x03},
    {0xfe, 0x00},
    {0xd9, 0xc0},
    {0x1b, 0x20},
    {0x21, 0x48},
    {0x28, 0x22},
    {0x29, 0x58},
    {0x44, 0x20},
    {0x4b, 0x10},
    {0x4e, 0x1a},
    {0x50, 0x11},
    {0x52, 0x33},
    {0x53, 0x44},
    {0x55, 0x10},
    {0x5b, 0x11},
    {0xc5, 0x02},
    {0x8c, 0x1a},
    {0xfe, 0x02},
    {0x33, 0x05},
    {0x32, 0x38},
    {0xfe, 0x00},
    {0x91, 0x80},
    {0x92, 0x28},
    {0x93, 0x20},
    {0x95, 0xa0},
    {0x96, 0xe0},
    {0xd5, 0xfc},
    {0x97, 0x28},
    {0x16, 0x0c},
    {0x1a, 0x1a},
    {0x1f, 0x11},
    {0x20, 0x10},
    {0x46, 0xe3},
    {0x4a, 0x04},
    {0x54, GC5035_RSTDUMMY1},
    {0x62, 0x00},
    {0x72, 0xcf},
    {0x73, 0xc9},
    {0x7a, 0x05},
    {0x7d, 0xcc},
    {0x90, 0x00},
    {0xce, 0x98},
    {0xd0, 0xb2},
    {0xd2, 0x40},
    {0xe6, 0xe0},
    {0xfe, 0x02},
    {0x12, 0x01},
    {0x13, 0x01},
    {0x14, 0x01},
    {0x15, 0x02},
    {0x22, GC5035_RSTDUMMY2},
    {0x91, 0x00},
    {0x92, 0x00},
    {0x93, 0x00},
    {0x94, 0x00},
    {0xfe, 0x00},
    {0xfc, 0x88},
    {0xfe, 0x10},
    {0xfe, 0x00},
    {0xfc, 0x8e},
    {0xfe, 0x00},
    {0xfe, 0x00},
    {0xfe, 0x00},
    {0xfc, 0x88},
    {0xfe, 0x10},
    {0xfe, 0x00},
    {0xfc, 0x8e},

    {0xfe, 0x01},
    {0x53, 0x00},
    {0x89, 0x03},
    {0x60, 0x40},
    {0x87, 0x50},

    {0xfe, 0x01},
    {0x42, 0x21},
    {0x49, 0x03},
    {0x4a, 0xff},
    {0x4b, 0xc0},
    {0x55, 0x00},

    {0xfe, 0x00},
    {0xb0, 0x6e},
    {0xb1, 0x01},
    {0xb2, 0x00},
    {0xb3, 0x00},
    {0xb4, 0x00},
    {0xb6, 0x00},

    {0xfe, 0x01},
    {0x41, 0x28},
    {0x4c, 0x00},
    {0x4d, 0x00},
    {0x4e, 0x3c},
    {0x44, 0x08},
    {0x48, 0x01},

    {0xfe, 0x01},
    {0x91, 0x00},
    {0x92, 0x08},
    {0x93, 0x00},
    {0x94, 0x07},
    {0x95, 0x07},
    {0x96, 0x98},
    {0x97, 0x0a},
    {0x98, 0x20},
    {0x99, 0x00},

    {0xfe, 0x03},
    {0x02, 0x57},
    {0x03, 0xb7},
    {0x15, 0x14},
    {0x18, 0x0f},
    {0x21, 0x22},
    {0x22, 0x06}, /* lwc 1920*5/4 */
    {0x23, 0x48},
    {0x24, 0x12},
    {0x25, 0x28},
    {0x26, 0x08},
    {0x29, 0x06},
    {0x2a, 0x58},
    {0x2b, 0x08},
    {0xfe, 0x01},
    {0x8c, 0x10},
    {0xfe, 0x00},
    {0x3e, 0x01},
#ifdef USE_25FPS
//frame length
    {0xfe, 0x00},
    {0x41, 0x08},
    {0x42, 0x46},
#endif
//test window size
//  {0xfe, 0x00},
//  {0x0d, 0x07},
//  {0x0e, 0x98},
//  {0x0f, 0x0a},
//  {0x10, 0x20},
    {REG_END, 0x00},
};

#ifdef USE_30FPS
static struct gc_camera_module_config gc5035_configs[] =
{
    {
        .name = "2592x1944_30fps",
        .frm_fmt = {
            .width = 2592,
            .height = 1944,
#if GC5035_MIRROR_FLIP_ENABLE
            .code = V4L2_MBUS_FMT_SGBRG10_1X10
#else
            .code = V4L2_MBUS_FMT_SRGGB10_1X10
#endif
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
        .reg_table = (void *)gc5035_init_tab_2592_1944_2lane,
        .reg_table_num_entries =
        sizeof(gc5035_init_tab_2592_1944_2lane) /
        sizeof(gc5035_init_tab_2592_1944_2lane[0]),
        .v_blanking_time_us = 5000,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 2, 876, GC5035_EXT_CLK, 0, 0x30)
    }
};
#endif

#ifdef USE_25FPS
static struct gc_camera_module_config gc5035_configs[] =
{
    {
        .name = "2592x1944_25fps",
        .frm_fmt = {
            .width = 2592,
            .height = 1944,
#if GC5035_MIRROR_FLIP_ENABLE
            .code = V4L2_MBUS_FMT_SGBRG10_1X10
#else
            .code = V4L2_MBUS_FMT_SRGGB10_1X10
#endif
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
        .reg_table = (void *)gc5035_init_tab_2592_1944_2lane,
        .reg_table_num_entries =
        sizeof(gc5035_init_tab_2592_1944_2lane) /
        sizeof(gc5035_init_tab_2592_1944_2lane[0]),
        .v_blanking_time_us = 3300,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 2, 876, GC5035_EXT_CLK, 0, 0x30)
    }
};
#endif

//#define PLTFRM_CAM_ITF_MIPI_CFG_EX(v, nb, br, mk, embedded_vc, embedded_type)\
//    .itf_cfg = {\
//        .type = PLTFRM_CAM_ITF_MIPI,\
//            .cfg = {\
//                .mipi = {\
//                    .dphy_index = 0,\
//                    .vc = v,\
//                    .nb_lanes = nb,\
//                    .bit_rate = br,\
//                    .embedded_data_vc = embedded_vc,\
//                    .embedded_data_type = embedded_type,\
//                } \
//            },\
//        .mclk_hz = mk\
//    }

static ret_err_t gc5035_g_vts(struct gc5035_dev *dev, u32 *vts);

u32 gc5035_gcd(u32 a, u32 b)
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

static ret_err_t gc5035_write_reg(rk_i2c_bus_device *bus,
                                  uint8_t reg, const uint8_t *data)
{
    uint8_t send_buf[2];
    struct rt_i2c_msg msgs;
    int ret = 0;

    MACRO_ASSERT(bus != RK_NULL);
    for (int tries = 0; tries < 5; tries++)
    {
        send_buf[0] = (reg & 0xff);
        send_buf[1] = *data;

        msgs.addr = GC5035_7BIT_ADDRESS;
        msgs.flags = RT_I2C_WR;
        msgs.buf = send_buf;
        msgs.len = 2;
        ret = rt_i2c_transfer(bus, &msgs, 1);

        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
    }
    GC5035_DEBUG("(%s):s0.0 i2c_bus error\n");
    return RET_SYS_ERROR;
}

static ret_err_t gc5035_read_reg(rk_i2c_bus_device *bus,
                                 uint8_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = (reg & 0xff);
    msg[0].addr = GC5035_7BIT_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 1;
    msg[0].buf = send_buf;

    msg[1].addr = GC5035_7BIT_ADDRESS;
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

static ret_err_t gc5035_write_multiple_reg(struct gc5035_dev *dev,
        const struct gc5035_sensor_reg *reg)
{
    uint16_t i;
    rk_i2c_bus_device *i2c_bus;
    ret_err_t ret;
    int num = 0;

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
            ret = gc5035_write_reg(i2c_bus, reg[i].reg_addr, &reg[i].val);
            if (ret != RET_SYS_EOK)
            {
                num = num + 1;
                rk_kprintf("[GC5035]: write init_table failed!!! num = %d\n", num);
            }
        }
    }
    return ret;

}

static ret_err_t rt_gc5035_detect_sensor(struct gc5035_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    ret_err_t ret;
    uint8_t pid, ver;
    uint16_t chipid;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    uint32_t cnt = 0;

    GC5035_DEBUG("(%s) enter \n", __FUNCTION__);

RETRY:
    cnt++;
    ret = gc5035_read_reg(bus, GC5035_REG_ID_H_ADDRESS, &pid);
    if (ret == RET_SYS_EOK)
    {
        ret = gc5035_read_reg(bus, GC5035_REG_ID_L_ADDRESS, &ver);
        chipid = SENSOR_ID(pid, ver);
        GC5035_INFO(dev, "Info: GC5035 pid:0x%x,ver:0x%x!\n", pid, ver);
        if (chipid == GC5035_SENSOR_ID)
        {
            GC5035_INFO(dev, "Info: GC5035 detected successfully !!!\n");
            ret = RET_SYS_EOK;
        }
        else
        {
            GC5035_INFO(dev, "Info: GC5035 detected failed,not match:%d != %d !!!\n",
                        GC5035_SENSOR_ID, chipid);
            ret = -RT_EINVAL;
            if (cnt > CAMERA_BOOT_RETRY_NUM)
                goto EXIT;
            goto RETRY;
        }
    }
    else
    {
        GC5035_INFO(dev, "Warning: GC5035 i2c read failed !!!\n");
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }
EXIT:
    GC5035_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static void gc5035_stream_on(struct rk_camera_device *dev)
{
    uint8_t val;
    GC5035_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);

    struct gc5035_dev *gc5035;
    gc5035 = (struct gc5035_dev *)dev;
    ret_err_t ret = RET_SYS_EOK;
    u8 data;
    data = GC5035_SET_PAGE_ZERO;
    ret = gc5035_write_reg(gc5035->i2c_bus, GC5035_REG_SET_PAGE, &data);
    data = GC5035_MODE_STREAMING;
    ret |= gc5035_write_reg(gc5035->i2c_bus, GC5035_REG_CTRL_MODE, &data);
    if (ret != RET_SYS_EOK)
    {
        GC5035_DEBUG("stream on failed!!! \n", __FUNCTION__);
    }

    GC5035_DEBUG("(%s) exit \n", __FUNCTION__);
}

static void gc5035_stream_off(struct rk_camera_device *dev)
{
    uint8_t val;
    rk_i2c_bus_device *i2c_bus;
    struct gc5035_dev *gc5035 = (struct gc5035_dev *)dev;
    int ret;

    GC5035_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);
    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        //rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);
        val = GC5035_SET_PAGE_ZERO;
        ret = gc5035_write_reg(i2c_bus,
                               GC5035_REG_SET_PAGE, &val);
        val = GC5035_MODE_SW_STANDBY;
        ret |= gc5035_write_reg(i2c_bus,
                                GC5035_REG_CTRL_MODE, &val);
        if (ret != RET_SYS_EOK)
        {
            GC5035_INFO(dev, "stream off failed with %d!\n", ret);
        }
        //rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        GC5035_INFO(dev, "Err: not find out i2c bus!\n");
    }

    GC5035_DEBUG("(%s) exit \n", __FUNCTION__);
}

ret_err_t rk_gc5035_init(struct rk_camera_device *dev)
{
    GC5035_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct gc5035_dev *gc5035;

    gc5035 = (struct gc5035_dev *)&dev;
#if RT_USING_GC5035_OPS
    if (gc5035->ops->init)
    {
        return (gc5035->ops->init(gc5035));
    }
#else

    if (gc5035)
    {
        gc5035->i2c_bus = (rk_i2c_bus_device *)rt_device_find(gc5035->i2c_name);

        if (!gc5035->i2c_bus)
        {
            GC5035_DEBUG("Warning:not find i2c source:%s !!!\n",
                         gc5035->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            GC5035_DEBUG("(%s):s0 find i2c_bus:%s\n", gc5035->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    GC5035_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

ret_err_t gc5035_g_sensor_iq_info(struct gc5035_dev *dev, void *args)
{
    ret_err_t ret = 0;
    struct rk_camera_iq_info *info = (struct rk_camera_iq_info *)args;
    info->addr = (void *)cam_iq_data;
    return ret;
}

static ret_err_t gc5035_otp_enable(struct gc5035_dev *dev)
{
    ret_err_t ret = 0;
    u8 data;

    data = 0x01;
    ret = gc5035_write_reg(dev->i2c_bus, 0xfc, &data);
    data = 0x40;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf4, &data);
    data = 0xe9;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf5, &data);
    data = 0x14;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf6, &data);
    data = 0x49;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf8, &data);
    data = 0x82;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf9, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfa, &data);
    data = 0x81;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfc, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    data = 0x01;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x36, &data);
    data = 0x87;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xd3, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x36, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x33, &data);
    data = 0x01;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf7, &data);
    data = 0x8e;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfc, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    data = 0x30;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xee, &data);
    data = 0x10;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfa, &data);
    data = 0xe9;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf5, &data);
    data = 0x02;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    data = 0xc0;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x67, &data);
    data = 0x3f;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x59, &data);
    data = 0x80;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x55, &data);
    data = 0x80;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x65, &data);
    data = 0x03;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x66, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    if (ret != RET_SYS_EOK)
    {
        GC5035_INFO(dev, "Warning:(%s) failed!!!\n", __FUNCTION__);
    }
    return ret;
}

static ret_err_t gc5035_otp_disable(struct gc5035_dev *dev)
{
    ret_err_t ret = 0;
    u8 data;

    data = 0x02;
    ret = gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x67, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    data = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xfa, &data);
    if (ret != RET_SYS_EOK)
    {
        GC5035_INFO(dev, "Warning:(%s) failed!!!\n", __FUNCTION__);
    }
    return ret;
}

static u8 gc5035_otp_read_byte(struct gc5035_dev *dev, u16 addr)
{
    u8 val;
    ret_err_t ret = 0;
    u8 data;

    data = 0x02;
    ret = gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    data = (addr >> 8) & 0x1f;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x69, &data);
    data = addr & 0xff;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x6a, &data);
    data = 0x20;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf3, &data);

    ret |= gc5035_read_reg(dev->i2c_bus, 0x6c, &val);

    if (ret != RET_SYS_EOK)
    {
        GC5035_INFO(dev, "Warning:(%s) failed!!!\n", __FUNCTION__);
    }

    return val;
}

static ret_err_t gc5035_otp_read_group(struct gc5035_dev *dev,
                                       u16 addr,
                                       u8 *data,
                                       u16 length)
{
    u16 i = 0;
    ret_err_t ret = 0;
    u8 temp;

    if ((((addr & 0x1fff) >> 3) + length) > GC5035_OTP_DATA_LENGTH)
    {
        return -1;
    }

    temp = 0x02;
    ret = gc5035_write_reg(dev->i2c_bus, 0xfe, &temp);
    temp = (addr >> 8) & 0x1f;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x69, &temp);
    temp = addr & 0xff;
    ret |= gc5035_write_reg(dev->i2c_bus, 0x6a, &temp);
    temp = 0x20;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf3, &temp);
    temp = 0x12;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf3, &temp);

    for (i = 0; i < length; i++)
        ret |= gc5035_read_reg(dev->i2c_bus, 0x6c, &data[i]);

    temp = 0x00;
    ret |= gc5035_write_reg(dev->i2c_bus, 0xf3, &temp);

    if (ret != RET_SYS_EOK)
    {
        GC5035_INFO(dev, "Warning:(%s) failed!!!\n", __FUNCTION__);
    }
    return ret;
}

static int gc5035_otp_read_module_info(struct gc5035_dev *dev, struct otp_struct *otp_p)
{
    u8 otp_flag = 0;
    u8 index = 0;
    u16 module_start_offset = GC5035_OTP_MODULE_DATA_OFFSET;
    u8 info[GC5035_OTP_MODULE_DATA_SIZE];
    u16 check = 0;
    int i = 0;
    int ret = 0;

    /* OTP module info*/
    otp_flag = gc5035_otp_read_byte(dev, GC5035_OTP_MODULE_FLAG_OFFSET);
    for (index = 0; index < 2; index++)
    {
        switch (GC5035_OTP_GET_2BIT_FLAG(otp_flag, 2 * (1 - index)))
        {
        case GC5035_OTP_FLAG_EMPTY:
            GC5035_INFO(dev, "%s GC5035_OTP_INFO group %d is Empty!\n",
                        __func__, index + 1);
            break;
        case GC5035_OTP_FLAG_VALID:
            module_start_offset = GC5035_OTP_MODULE_DATA_OFFSET
                                  + GC5035_OTP_GET_OFFSET(index * GC5035_OTP_MODULE_DATA_SIZE);
            ret = gc5035_otp_read_group(dev, module_start_offset, &info[0], GC5035_OTP_MODULE_DATA_SIZE);
            for (i = 0; i < GC5035_OTP_MODULE_DATA_SIZE - 1; i++)
                check += info[i];

            if ((check % 255 + 1) == info[GC5035_OTP_MODULE_DATA_SIZE - 1])
            {
                otp_p->flag |= 0x80;
                otp_p->module_id = info[0];
                otp_p->lens_id = info[1];
                otp_p->year = info[2];
                otp_p->month = info[3];
                otp_p->day = info[4];

                GC5035_INFO(dev, "fac info: module(0x%x) lens(0x%x) time(%d_%d_%d)\n",
                            otp_p->module_id,
                            otp_p->lens_id,
                            otp_p->year,
                            otp_p->month,
                            otp_p->day);
            }
            else
                GC5035_INFO(dev, "check sum %d error! check sum = 0x%x, calculate result = 0x%x\n",
                            index + 1, info[GC5035_OTP_MODULE_DATA_SIZE - 1], (check % 255 + 1));
            break;
        case GC5035_OTP_FLAG_INVALID:
        case GC5035_OTP_FLAG_INVALID2:
            GC5035_INFO(dev, "%s GC5035_OTP_INFO group %d is Invalid !!\n",
                        __func__, index + 1);
            break;
        default:
            break;
        }
    }
    return ret;
}

static int gc5035_otp_read_wb_info(struct gc5035_dev *dev, struct otp_struct *otp_p)
{
    u8 flag = 0;
    u8 index = 0;
    u16 wb_start_offset = GC5035_OTP_WB_DATA_OFFSET;
    u8 wb[GC5035_OTP_WB_DATA_SIZE];
    u16 check = 0;
    int i = 0;
    int ret = 0;

    /* OTP module info*/
    flag = gc5035_otp_read_byte(dev, GC5035_OTP_WB_FLAG_OFFSET);
    for (index = 0; index < 2; index++)
    {
        switch (GC5035_OTP_GET_2BIT_FLAG(flag, 2 * (1 - index)))
        {
        case GC5035_OTP_FLAG_EMPTY:
            GC5035_INFO(dev, "%s GC5035_OTP_WB group %d is Empty!\n",
                        __func__, index + 1);
            break;
        case GC5035_OTP_FLAG_VALID:
            wb_start_offset = GC5035_OTP_WB_DATA_OFFSET
                              + GC5035_OTP_GET_OFFSET(index * GC5035_OTP_WB_DATA_SIZE);
            ret = gc5035_otp_read_group(dev, wb_start_offset, &wb[0], GC5035_OTP_WB_DATA_SIZE);
            for (i = 0; i < GC5035_OTP_WB_DATA_SIZE - 1; i++)
                check += wb[i];

            if ((check % 255 + 1) == wb[GC5035_OTP_WB_DATA_SIZE - 1])
            {
                otp_p->flag |= 0x40;
                otp_p->rg_ratio = (wb[0] | ((wb[1] & 0xf0) << 4));
                otp_p->bg_ratio = (((wb[1] & 0x0f) << 8) | wb[2]);

                GC5035_INFO(dev, "wb info: rg(0x%x) bg(0x%x)\n",
                            otp_p->rg_ratio,
                            otp_p->bg_ratio);
            }
            else
                GC5035_INFO(dev, "check sum %d error! check sum = 0x%x, calculate result = 0x%x\n",
                            index + 1, wb[GC5035_OTP_WB_DATA_SIZE - 1], (check % 255 + 1));
            break;
        case GC5035_OTP_FLAG_INVALID:
        case GC5035_OTP_FLAG_INVALID2:
            GC5035_INFO(dev, "%s GC5035_OTP_WB group %d is Invalid !!\n",
                        __func__, index + 1);
            break;
        default:
            break;
        }
        switch (GC5035_OTP_GET_2BIT_FLAG(flag, 2 * (3 - index)))
        {
        case GC5035_OTP_FLAG_EMPTY:
            GC5035_INFO(dev, "%s GC5035_OTP_WB_GOLDEN group %d is Empty!\n",
                        __func__, index + 1);
            break;
        case GC5035_OTP_FLAG_VALID:
            wb_start_offset = GC5035_OTP_GOLDEN_DATA_OFFSET
                              + GC5035_OTP_GET_OFFSET(index * GC5035_OTP_GOLDEN_DATA_SIZE);
            ret |= gc5035_otp_read_group(dev, wb_start_offset, &wb[0], GC5035_OTP_GOLDEN_DATA_SIZE);
            for (i = 0; i < GC5035_OTP_GOLDEN_DATA_SIZE - 1; i++)
                check += wb[i];

            if ((check % 255 + 1) == wb[GC5035_OTP_GOLDEN_DATA_SIZE - 1])
            {
                otp_p->flag |= 0x20;
                otp_p->golden_rg = (wb[0] | ((wb[1] & 0xf0) << 4));
                otp_p->golden_bg = (((wb[1] & 0x0f) << 8) | wb[2]);

                GC5035_INFO(dev, "golden wb info: rg(0x%x) bg(0x%x)\n",
                            otp_p->golden_rg,
                            otp_p->golden_bg);
            }
            else
                GC5035_INFO(dev, "check sum %d error! check sum = 0x%x, calculate result = 0x%x\n",
                            index + 1, wb[GC5035_OTP_GOLDEN_DATA_SIZE - 1], (check % 255 + 1));
            break;
        case GC5035_OTP_FLAG_INVALID:
        case GC5035_OTP_FLAG_INVALID2:
            GC5035_INFO(dev, "%s GC5035_OTP_WB_GOLDEN group %d is Invalid !!\n",
                        __func__, index + 1);
            break;
        default:
            break;
        }
    }
    return ret;
}

static void gc5035_gcore_read_dpc(struct gc5035_dev *dev, struct otp_struct *otp_p)
{
    u8 dpcFlag = 0;

    dpcFlag = gc5035_otp_read_byte(dev, GC5035_OTP_DPC_FLAG_OFFSET);
    switch (GC5035_OTP_GET_2BIT_FLAG(dpcFlag, 0))
    {
    case GC5035_OTP_FLAG_EMPTY:
    {
        GC5035_INFO(dev, "dpc info is empty!!\n");
        break;
    }
    case GC5035_OTP_FLAG_VALID:
    {
        otp_p->total_num = gc5035_otp_read_byte(dev, GC5035_OTP_DPC_TOTAL_NUMBER_OFFSET)
                           + gc5035_otp_read_byte(dev, GC5035_OTP_DPC_ERROR_NUMBER_OFFSET);
        otp_p->flag = 0x10;
        GC5035_INFO(dev, "otp dpc total_num = %d\n", otp_p->total_num);
        break;
    }
    default:
        break;
    }
}

static void gc5035_gcore_read_reg(struct gc5035_dev *dev, struct otp_struct *otp_p)
{
    u8 i = 0;
    u8 j = 0;
    u16 base_group = 0;
    u8 reg[GC5035_OTP_REG_DATA_SIZE];
    struct gc5035_reg_update_t *pRegs = &otp_p->regs;
    u8 flag = 0;

    memset(&reg, 0, GC5035_OTP_REG_DATA_SIZE);
    flag = gc5035_otp_read_byte(dev, GC5035_OTP_REG_FLAG_OFFSET);
    if (flag == GC5035_OTP_FLAG_VALID)
    {
        gc5035_otp_read_group(dev, GC5035_OTP_REG_DATA_OFFSET, &reg[0], GC5035_OTP_REG_DATA_SIZE);

        for (i = 0; i < GC5035_OTP_REG_MAX_GROUP; i++)
        {
            base_group = i * GC5035_OTP_REG_BYTE_PER_GROUP;
            for (j = 0; j < GC5035_OTP_REG_REG_PER_GROUP; j++)
                if (GC5035_OTP_CHECK_1BIT_FLAG(reg[base_group], (4 * j + 3)))
                {
                    pRegs->reg[pRegs->cnt].page =
                        (reg[base_group] >> (4 * j)) & 0x07;
                    pRegs->reg[pRegs->cnt].addr =
                        reg[base_group + j * GC5035_OTP_REG_BYTE_PER_REG + 1];
                    pRegs->reg[pRegs->cnt].value =
                        reg[base_group + j * GC5035_OTP_REG_BYTE_PER_REG + 2];
                    GC5035_INFO(dev, "register[%d] P%d:0x%x->0x%x\n",
                                pRegs->cnt, pRegs->reg[pRegs->cnt].page,
                                pRegs->reg[pRegs->cnt].addr, pRegs->reg[pRegs->cnt].value);
                    pRegs->cnt++;
                }
        }
        otp_p->flag |= 0x08;

    }
}

static int gc5035_otp_read(struct gc5035_dev *dev)
{
    struct otp_struct *otp_p = NULL;
    int ret = 0;
    if (otp_p == NULL)
    {
        otp_p = (struct otp_struct *)rt_malloc(sizeof(struct otp_struct));
        rt_memset(otp_p, 0x00, sizeof(struct otp_struct));
    }

    if (!otp_p)
        return -ENOMEM;
    ret = gc5035_otp_enable(dev);
    ret |= gc5035_otp_read_module_info(dev, otp_p);
    ret |= gc5035_otp_read_wb_info(dev, otp_p);
    gc5035_gcore_read_dpc(dev, otp_p);
    gc5035_gcore_read_reg(dev, otp_p);
    ret |= gc5035_otp_disable(dev);
    if (otp_p->flag)
    {
        g_otp_info = otp_p;
    }
    else
    {
        g_otp_info = NULL;
//      devm_kfree(&cam_mod->client->dev, otp_p);
        rt_free(otp_p);
    }
    return ret;
}

static void gc5035_otp_update_dd(struct gc5035_dev *dev)
{
    u8 state = 0;
    u8 n = 0;
    int ret = 0;
    u8 data;

    if (0x10 == g_otp_info->flag)
    {
        GC5035_INFO(dev, "DD auto load start!\n");
        data = 0x02;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xbe, &data);
        data = 0x01;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xa9, &data);
        data = 0x33;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x09, &data);
        data = (g_otp_info->total_num >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x01, &data);
        data = g_otp_info->total_num & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x02, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x03, &data);
        data = 0x80;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x04, &data);
        data = 0x0a;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x95, &data);
        data = 0x30;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x96, &data);
        data = 0x0a;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x97, &data);
        data = 0x32;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x98, &data);
        data = 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x99, &data);
        data = 0xa9;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x9a, &data);
        data = 0x80;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xf3, &data);

        while (n < 3)
        {
            ret |= gc5035_read_reg(dev->i2c_bus, 0x6c, &state);
            if ((state | 0xfe) == 0xff)
                rt_thread_mdelay(10);
            else
                n = 3;
            n++;
        }

        data = 0x01;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xbe, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x09, &data);
        data = 0x01;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
        data = 0x02;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x80, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    }
}

static void gc5035_otp_update_wb(struct gc5035_dev *dev)
{
    u16 r_gain = GC5035_OTP_WB_GAIN_BASE;
    u16 g_gain = GC5035_OTP_WB_GAIN_BASE;
    u16 b_gain = GC5035_OTP_WB_GAIN_BASE;
    u16 base_gain = GC5035_OTP_WB_CAL_BASE;
    u16 r_gain_curr = GC5035_OTP_WB_CAL_BASE;
    u16 g_gain_curr = GC5035_OTP_WB_CAL_BASE;
    u16 b_gain_curr = GC5035_OTP_WB_CAL_BASE;
    u32 golden_bg_ratio = 0;
    u32 golden_rg_ratio = 0;
    u32 golden_g_value = 0;
    u8 data;
    ret_err_t ret;

    struct module_awb_cfg *awb_cfg = &dev->cam_mod.awb_cfg;

    if (awb_cfg->enable)
    {
        golden_g_value = (awb_cfg->golden_gb_value +
                          awb_cfg->golden_gr_value) / 2;
        golden_bg_ratio =
            awb_cfg->golden_b_value * 0x400 / golden_g_value;
        golden_rg_ratio =
            awb_cfg->golden_r_value * 0x400 / golden_g_value;
    }
    /* apply OTP WB Calibration */
    if ((g_otp_info->flag & 0x40) && golden_bg_ratio && golden_rg_ratio)
    {
        r_gain_curr = GC5035_OTP_WB_CAL_BASE * golden_rg_ratio / g_otp_info->rg_ratio;
        b_gain_curr = GC5035_OTP_WB_CAL_BASE * golden_bg_ratio / g_otp_info->bg_ratio;
        g_gain_curr = GC5035_OTP_WB_CAL_BASE;

        base_gain = (r_gain_curr < b_gain_curr) ? r_gain_curr : b_gain_curr;
        base_gain = (base_gain < g_gain_curr) ? base_gain : g_gain_curr;

        r_gain = GC5035_OTP_WB_GAIN_BASE * r_gain_curr / base_gain;
        g_gain = GC5035_OTP_WB_GAIN_BASE * g_gain_curr / base_gain;
        b_gain = GC5035_OTP_WB_GAIN_BASE * b_gain_curr / base_gain;
        GC5035_INFO(dev, "channel gain r = 0x%x, g = 0x%x, b = 0x%x\n", r_gain, g_gain, b_gain);


        data = 0x04;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
        data = g_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x40, &data);
        data = r_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x41, &data);
        data = b_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x42, &data);
        data = g_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x43, &data);
        data = g_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x44, &data);
        data = r_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x45, &data);
        data = b_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x46, &data);
        data = g_gain & 0xff;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x47, &data);
        data = (g_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x48, &data);
        data = (r_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x49, &data);
        data = (b_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x4a, &data);
        data = (g_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x4b, &data);
        data = (g_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x4c, &data);
        data = (r_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x4d, &data);
        data = (b_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x4e, &data);
        data = (g_gain >> 8) & 0x07;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x4f, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
    }
    if (ret != RET_SYS_EOK)
    {
        GC5035_INFO(dev, "Warning:(%s) failed!!!\n", __FUNCTION__);
    }
}

static void gc5035_otp_update_reg(struct gc5035_dev *dev)
{
    u8 i = 0;
    u8 data;
    ret_err_t ret;

    if (g_otp_info->flag & 0x08)
    {
        GC5035_INFO(dev, "reg count = %d\n", g_otp_info->regs.cnt);
        for (i = 0; i < g_otp_info->regs.cnt; i++)
        {
            data = g_otp_info->regs.reg[i].page;
            ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
            data = g_otp_info->regs.reg[i].value;
            ret |= gc5035_write_reg(dev->i2c_bus,
                                    g_otp_info->regs.reg[i].addr, &data);
            GC5035_INFO(dev, "reg[%d] P%d:0x%x -> 0x%x\n", i, g_otp_info->regs.reg[i].page,
                        g_otp_info->regs.reg[i].addr, g_otp_info->regs.reg[i].value);
        }
    }
    if (ret != RET_SYS_EOK)
    {
        GC5035_INFO(dev, "Warning:(%s) failed!!!\n", __FUNCTION__);
    }
}

/*--------------------------------------------------------------------------*/
static void gc5035_apply_otp(struct gc5035_dev *dev)
{
    ret_err_t ret = 0;
    u8 data;

    if (g_otp_info)
    {
        data = 0x10;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfa, &data);
        data = 0xe9;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xf5, &data);
        data = 0x02;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
        data = 0xc0;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x67, &data);
        data = 0x3f;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x59, &data);
        data = 0x80;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x55, &data);
        data = 0x80;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x65, &data);
        data = 0x03;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x66, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);

        gc5035_otp_update_dd(dev);
        gc5035_otp_update_wb(dev);
        gc5035_otp_update_reg(dev);
        data = 0x02;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0x67, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfe, &data);
        data = 0x00;
        ret |= gc5035_write_reg(dev->i2c_bus, 0xfa, &data);
    }
}

/*--------------------------------------------------------------------------*/

static ret_err_t gc5035_g_vts(struct gc5035_dev *dev, u32 *vts)
{
    u8 msb, lsb;
    ret_err_t ret = 0;
    u8 data;


    data = GC5035_SET_PAGE_ZERO;
    ret |= gc5035_write_reg(dev->i2c_bus, GC5035_REG_SET_PAGE, &data);
    ret |= gc5035_read_reg(dev->i2c_bus, GC5035_TIMING_VTS_HIGH_REG, &msb);
    if (IS_ERR_VALUE(ret))
        goto err;

    ret |= gc5035_read_reg(dev->i2c_bus, GC5035_TIMING_VTS_LOW_REG, &lsb);
    if (ret != RET_SYS_EOK)
        goto err;

    *vts = msb & 0x3f;
    *vts = *vts << 8;
    *vts = *vts | lsb;

//  *vts = (msb << 8) | lsb;

    return RET_SYS_EOK;
err:
    GC5035_INFO(dev, "failed with error (%d)\n", ret);
    return ret;
}

/* ======================================================================== */

static ret_err_t gc5035_s_vts(struct gc5035_dev *dev,
                              u32 vts)
{
    ret_err_t ret = 0;
    u8 data;
    struct gc_camera_module *cam_mod = &dev->cam_mod;


    if (vts > 0x3fff)
        vts = 0x3fff;
    if (vts < cam_mod->vts_min)
        vts = cam_mod->vts_min;

    data = GC5035_SET_PAGE_ZERO;
    ret |= gc5035_write_reg(dev->i2c_bus, GC5035_REG_SET_PAGE, &data);
    data = vts & 0xFF;
    ret |= gc5035_write_reg(dev->i2c_bus, GC5035_TIMING_VTS_LOW_REG, &data);
    data = (vts >> 8) & 0x3F;
    ret |= gc5035_write_reg(dev->i2c_bus, GC5035_TIMING_VTS_HIGH_REG, &data);

    if (IS_ERR_VALUE(ret))
    {
        GC5035_INFO(dev, "s_vts:failed with error (%d)\n", ret);
    }
    else
    {
        GC5035_DEBUG(dev, "updated vts = 0x%x,vts_min=0x%x\n", vts, cam_mod->vts_min);
        cam_mod->vts_cur = vts;
    }
    return ret;
}

/* ======================================================================== */

ret_err_t gc5035_auto_adjust_fps_state(struct gc5035_dev *dev, void *args)
{

    ret_err_t ret;
    bool *auto_adjust_fps = (bool *)args;
    dev->cam_mod.auto_adjust_fps = *auto_adjust_fps;
    GC5035_INFO(dev, "state of gc5035_auto_adjust_fps is : %d.", dev->cam_mod.auto_adjust_fps);
    return RET_SYS_EOK;
}

/* ======================================================================== */

static ret_err_t gc5035_auto_adjust_fps(struct gc5035_dev *dev,
                                        u32 exp_time)
{
    ret_err_t ret = 0;
    u32 vts = 0;
    u8 data;

    struct gc_camera_module *cam_mod = &dev->cam_mod;

    if ((exp_time + GC5035_COARSE_INTG_TIME_MAX_MARGIN)
            > cam_mod->vts_min)
        vts = exp_time + GC5035_COARSE_INTG_TIME_MAX_MARGIN;
    else
        vts = cam_mod->vts_min;

    if (vts > 0xfff)
        vts = 0xfff;
    else
        vts = vts;

    data = GC5035_SET_PAGE_ZERO;
    ret |= gc5035_write_reg(dev->i2c_bus, GC5035_REG_SET_PAGE, &data);
    data = vts & 0xFF;
    ret |= gc5035_write_reg(dev->i2c_bus, GC5035_TIMING_VTS_LOW_REG, &data);
    data = (vts >> 8) & 0x0F;
    ret |= gc5035_write_reg(dev->i2c_bus, GC5035_TIMING_VTS_HIGH_REG, &data);

    if (IS_ERR_VALUE(ret))
    {
        GC5035_INFO(dev, "failed with error (%d)\n", ret);
    }
    else
    {
        GC5035_DEBUG(dev, "updated vts = 0x%x,vts_min=0x%x\n", vts, cam_mod->vts_min);
        cam_mod->vts_cur = vts;
    }
    return ret;
}

/* ======================================================================== */

static ret_err_t gc5035_s_aec(struct gc5035_dev *dev, void *args)
{
    int i;
    ret_err_t ret = 0;
    u32 temp_gain = 0;
    u32 caltime = 0;
    u32 Dgain_ratio = 0;
    struct gc_camera_module *cam_mod = &dev->cam_mod;
    struct HAL_ISP_Set_Exp_s *aec = (struct HAL_ISP_Set_Exp_s *)args;
    u8 data;

    if ((cam_mod->state == GC_CAMERA_MODULE_SW_STANDBY) ||
            (cam_mod->state == GC_CAMERA_MODULE_STREAMING))
    {

        u32 gain = aec->gain;
        u32 exp_time = aec->exposure;

        gain = gain * cam_mod->exp_config.gain_percent / 100;
        if (gain < 0x40)
            gain = 0x40;
        else if (gain > 0x400)
            gain = 0x400;
        if (!IS_ERR_VALUE(ret) && cam_mod->auto_adjust_fps)
            ret |= gc5035_auto_adjust_fps(dev,
                                          cam_mod->exp_config.exp_time);

        caltime = exp_time / 2;
        caltime = caltime * 2;
        Dgain_ratio = 256 * exp_time / caltime;

        data = GC5035_SET_PAGE_ZERO;
        ret |= gc5035_write_reg(dev->i2c_bus,
                                GC5035_REG_SET_PAGE, &data);

        data = GC5035_FETCH_HIGH_BYTE_EXP(exp_time);
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_AEC_EXPO_HIGH_REG, &data);
        data = GC5035_FETCH_LOW_BYTE_EXP(exp_time);
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_AEC_EXPO_LOW_REG, &data);

        u32 shutter;
        ret |= gc5035_read_reg(dev->i2c_bus, GC5035_AEC_EXPO_HIGH_REG, &data);
        shutter = (data & 0x0f) << 8;
        ret |= gc5035_read_reg(dev->i2c_bus, GC5035_AEC_EXPO_LOW_REG, &data);
        shutter = shutter | (data & 0xff);

        for (i = 16; i >= 0; i--)
        {

            if (gain >= GC5035_AGC_Param[i][0])
                break;
        }

        data = GC5035_AGC_Param[i][1];

        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_AEC_ANALOG_GAIN_REG, &data);
        ret |= gc5035_read_reg(dev->i2c_bus, GC5035_AEC_ANALOG_GAIN_REG, &data);

        temp_gain = gain;
        temp_gain = temp_gain * Dgain_ratio / GC5035_AGC_Param[i][0];
        data = temp_gain >> 8 & 0x0f;
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_AEC_DIGITAL_INT_GAIN_REG, &data);
        data = temp_gain & 0xfc;
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_AEC_DIGITAL_FRAC_GAIN_REG, &data);

        if (ret == RET_SYS_EOK)
        {
            GC5035_DEBUG(dev,
                         "gain = 0x%x, a_gain_code = 0x%x, d_gain_code = 0x%x\n",
                         gain, gain, temp_gain);
            cam_mod->exp_config.gain = gain;
            cam_mod->exp_config.exp_time = exp_time;
        }

        if (!cam_mod->auto_adjust_fps)
            ret |= gc5035_s_vts(dev, cam_mod->exp_config.vts_value);
    }

    if (ret != RET_SYS_EOK)
    {
        GC5035_INFO(dev, "s_aec:failed with error (%d)\n", ret);
    }

    return ret;
}

/* ======================================================================== */

static ret_err_t gc5035_g_aec(struct gc5035_dev *dev, void *args)
{
    ret_err_t ret;
    struct HAL_ISP_Set_Exp_s *aec_value = (struct HAL_ISP_Set_Exp_s *)args;
    struct gc_camera_module *cam_mod = &dev->cam_mod;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    u32 shutter;
    u8 data;
    u8 analog_gain = 0;

    data = GC5035_SET_PAGE_ZERO;
    ret |= gc5035_write_reg(dev->i2c_bus,
                            GC5035_REG_SET_PAGE, &data);

    ret |= gc5035_read_reg(dev->i2c_bus,
                           GC5035_AEC_ANALOG_GAIN_REG, &analog_gain);

    if (ret == RET_SYS_EOK)
    {
        aec_value->gain = analog_gain;
        cam_mod->exp_config.gain = aec_value->gain;
    }

    ret |= gc5035_read_reg(dev->i2c_bus,
                           GC5035_AEC_EXPO_HIGH_REG, &data);
    shutter = (data & 0x0f) << 8;
    ret |= gc5035_read_reg(dev->i2c_bus,
                           GC5035_AEC_EXPO_LOW_REG, &data);
    shutter = shutter | (data & 0xff);

    if (ret == RET_SYS_EOK)
    {
        aec_value->exposure = shutter;
        cam_mod->exp_config.exp_time = aec_value->exposure;
    }
    return ret;
}

/* ======================================================================== */

ret_err_t gc5035_g_fmt(struct gc5035_dev *dev, void *args)
{
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    if (dev->cam_mod.active_config)
    {
        fmt->code = dev->cam_mod.active_config->frm_fmt.code;
        fmt->width = dev->cam_mod.active_config->frm_fmt.width;
        fmt->height = dev->cam_mod.active_config->frm_fmt.height;
        return RET_SYS_EOK;
    }

    GC5035_INFO(dev, "Warning: no active config\n");
    return RET_SYS_ERROR;
}
/* ======================================================================== */

ret_err_t gc5035_g_sensor_frm_interval(struct gc5035_dev *dev, void *args)
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

    GC5035_INFO(dev, "no active config\n");
    return RET_SYS_ERROR;
}

/* ======================================================================== */

static void gc_camera_module_set_active_config(
    struct gc5035_dev *dev,
    struct gc_camera_module_config *new_config)
{

    if (RK_NULL == new_config)
    {
        dev->cam_mod.active_config = new_config;
        GC5035_INFO(dev, "Warning: no active config\n");
    }
    else
    {
        dev->cam_mod.ctrl_updt &= GC_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP |
                                  GC_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN |
                                  GC_CAMERA_MODULE_CTRL_UPDT_AUTO_WB;
        if (new_config->auto_exp_enabled !=
                dev->cam_mod.exp_config.auto_exp)
        {
            dev->cam_mod.ctrl_updt |=
                GC_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP;
            dev->cam_mod.exp_config.auto_exp =
                new_config->auto_exp_enabled;
        }
        if (new_config->auto_gain_enabled !=
                dev->cam_mod.exp_config.auto_gain)
        {
            dev->cam_mod.ctrl_updt |=
                GC_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN;
            dev->cam_mod.exp_config.auto_gain =
                new_config->auto_gain_enabled;
        }
        if (new_config->auto_wb_enabled !=
                dev->cam_mod.wb_config.auto_wb)
        {
            dev->cam_mod.ctrl_updt |=
                GC_CAMERA_MODULE_CTRL_UPDT_AUTO_WB;
            dev->cam_mod.wb_config.auto_wb =
                new_config->auto_wb_enabled;
        }
        if (new_config != dev->cam_mod.active_config)
        {
            dev->cam_mod.update_config = true;
            dev->cam_mod.active_config = new_config;
            GC5035_INFO(dev,
                        "activating config '%s'\n",
                        dev->cam_mod.active_config->name);
        }
    }
}

/* ======================================================================== */

static struct gc_camera_module_config *gc_camera_module_find_config(
    struct gc5035_dev *dev,
    struct v4l2_mbus_framefmt *fmt,
    struct v4l2_subdev_frame_interval *frm_intrvl)
{
    u32 i;
    u32 gcdiv;
    struct v4l2_subdev_frame_interval norm_interval;

    GC5035_INFO(dev, "fmt = 0x%x,frm_intrvl = 0x%x\n", fmt, frm_intrvl);

    if (NULL != fmt)
        GC5035_INFO(dev,
                    "gc_camera_module_find_config:%dx%d, fmt code 0x%04x\n",
                    fmt->width, fmt->height, fmt->code);

    if (NULL != frm_intrvl)
    {
        GC5035_INFO(dev,
                    "gc_camera_module_find_config:frame interval %d/%d....\n",
                    frm_intrvl->interval.numerator,
                    frm_intrvl->interval.denominator);
    }


    for (i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (NULL != frm_intrvl)
        {
            gcdiv = gc5035_gcd(dev->cam_mod.custom.configs[i].
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
        GC5035_INFO(dev,
                    "found matching config %s\n",
                    dev->cam_mod.custom.configs[i].name);
        return &dev->cam_mod.custom.configs[i];
    }
    GC5035_INFO(dev, "no matching config found\n");

    return RK_NULL;
}

/* ======================================================================== */

ret_err_t gc5035_s_fmt(struct gc5035_dev *dev, void *args)
{
    struct gc_camera_module_config *config;
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    ret_err_t ret = RET_SYS_EOK;

    GC5035_INFO(dev, "gc5035_s_fmt:%dx%d, fmt code 0x%04x\n",
                fmt->width, fmt->height, fmt->code);

    config = gc_camera_module_find_config(dev, fmt, NULL);

    if (config == NULL)
    {
        GC5035_INFO(dev,
                    "format %dx%d, code 0x%04x, not supported\n",
                    fmt->width, fmt->height, fmt->code);
        ret = RET_SYS_ERROR;
        goto err;
    }
    dev->cam_mod.frm_fmt_valid = true;
    dev->cam_mod.frm_fmt = *fmt;
    GC5035_INFO(dev, "fmt is supported,%dx%d, fmt code 0x%04x\n", fmt->width, fmt->height, fmt->code);

    struct gc_camera_module_config *config_found = gc_camera_module_find_config(
                dev, fmt, &dev->cam_mod.frm_intrvl);

    if (dev->cam_mod.frm_intrvl_valid &&
            (RK_NULL != config_found))
    {
        gc_camera_module_set_active_config(dev, config_found);
    }
    else
    {
        gc_camera_module_set_active_config(dev, config);
    }

    return RET_SYS_EOK;
err:
    ret = RET_SYS_ERROR;
    GC5035_INFO(dev,
                "failed with error %d\n", ret);
    return ret;
}

/* ======================================================================== */

ret_err_t gc5035_g_v_blank_us(struct gc5035_dev *dev, void *args)
{
    u32 *v_blanking_time_us = (u32 *)args;
    *v_blanking_time_us = dev->cam_mod.active_config->v_blanking_time_us;
    return RET_SYS_EOK;
}

/* ======================================================================== */

ret_err_t gc5035_g_itf_cfg(struct gc5035_dev *dev, void *args)
{
    struct pltfrm_cam_itf *itf_cfg = (struct pltfrm_cam_itf *)args;
    struct gc_camera_module_config *config;

    if (dev->cam_mod.custom.num_configs <= 0)
    {
        GC5035_INFO(dev,
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

static ret_err_t gc5035_g_timings(struct gc5035_dev *dev,
                                  struct gc_camera_module_timings *timings)
{
    unsigned int vts;

    if (NULL == dev->cam_mod.active_config)
        goto err;

    *timings = dev->cam_mod.active_config->timings;

    vts = (!dev->cam_mod.vts_cur) ?
          timings->frame_length_lines :
          dev->cam_mod.vts_cur;

    timings->frame_length_lines = vts;

    return RET_SYS_EOK;
err:
    GC5035_INFO(dev, "cam_mod:failed with error (%d)\n", RET_SYS_EEMPTY);
    return RET_SYS_EEMPTY;
}

/* ======================================================================== */

ret_err_t gc5035_g_sensor_mode(struct gc5035_dev *dev, void *args)
{
    ret_err_t ret;
    struct gc_camera_module_timings gc_timings;

    int *args_data = (int *)args;

    ret = gc5035_g_timings(dev, &gc_timings);

    if (RET_SYS_EOK != ret)
    {
        GC5035_INFO(dev,
                    "failed with error %d\n", ret);
        return ret;
    }

    gc5035_sensorModeData->isp_input_horizontal_start = 0;
    gc5035_sensorModeData->isp_input_vertical_start   = 0;
    gc5035_sensorModeData->isp_input_width   = 2592;
    gc5035_sensorModeData->isp_input_height  = 1944;
    gc5035_sensorModeData->isp_output_width  = 2592;
    gc5035_sensorModeData->isp_output_height = 1944;

    gc5035_sensorModeData->sensor_output_width = gc_timings.sensor_output_width;
    gc5035_sensorModeData->sensor_output_height = gc_timings.sensor_output_height;
    gc5035_sensorModeData->crop_horizontal_start = gc_timings.crop_horizontal_start;
    gc5035_sensorModeData->crop_vertical_start = gc_timings.crop_vertical_start;
    gc5035_sensorModeData->crop_horizontal_end = gc_timings.crop_horizontal_end;
    gc5035_sensorModeData->crop_vertical_end = gc_timings.crop_vertical_end;
    gc5035_sensorModeData->line_length_pck = gc_timings.line_length_pck;
    gc5035_sensorModeData->frame_length_lines = gc_timings.frame_length_lines;
    gc5035_sensorModeData->vt_pix_clk_freq_hz = gc_timings.vt_pix_clk_freq_hz;
    gc5035_sensorModeData->binning_factor_x = gc_timings.binning_factor_x;
    gc5035_sensorModeData->binning_factor_y = gc_timings.binning_factor_y;
    gc5035_sensorModeData->coarse_integration_time_max_margin =
        gc_timings.coarse_integration_time_max_margin;
    gc5035_sensorModeData->coarse_integration_time_min =
        gc_timings.coarse_integration_time_min;
    gc5035_sensorModeData->fine_integration_time_max_margin =
        gc_timings.fine_integration_time_max_margin;
    gc5035_sensorModeData->fine_integration_time_min =
        gc_timings.fine_integration_time_min;

    gc5035_sensorModeData->exposure_valid_frame[0] =
        dev->cam_mod.custom.exposure_valid_frame[0];
    gc5035_sensorModeData->exposure_valid_frame[1] =
        dev->cam_mod.custom.exposure_valid_frame[1];
    gc5035_sensorModeData->lane_num = 2;
    if (dev->cam_mod.exp_config.exp_time)
    {
        gc5035_sensorModeData->exp_time = dev->cam_mod.exp_config.exp_time;
        gc5035_sensorModeData->exp_time_l = dev->cam_mod.exp_config.exp_time_l;
        gc5035_sensorModeData->exp_time_s = dev->cam_mod.exp_config.exp_time_s;
    }
    else
    {
        gc5035_sensorModeData->exp_time = gc_timings.exp_time;
        gc5035_sensorModeData->exp_time_l = gc_timings.exp_time;
        gc5035_sensorModeData->exp_time_s = gc_timings.exp_time;
    }
    if (dev->cam_mod.exp_config.gain)
    {
        gc5035_sensorModeData->gain = dev->cam_mod.exp_config.gain;
        gc5035_sensorModeData->gain_l = dev->cam_mod.exp_config.gain_l;
        gc5035_sensorModeData->gain_s = dev->cam_mod.exp_config.gain_s;
    }
    else
    {
        gc5035_sensorModeData->gain = gc_timings.gain;
        gc5035_sensorModeData->gain_l = gc_timings.gain;
        gc5035_sensorModeData->gain_s = gc_timings.gain;
    }
    if (dev->cam_mod.active_config)
    {
        gc5035_sensorModeData->max_exp_gain_h = dev->cam_mod.active_config->max_exp_gain_h;
        gc5035_sensorModeData->max_exp_gain_l = dev->cam_mod.active_config->max_exp_gain_l;
    }
    else
    {
        gc5035_sensorModeData->max_exp_gain_h = dev->cam_mod.custom.configs[0].max_exp_gain_h;
        gc5035_sensorModeData->max_exp_gain_l = dev->cam_mod.custom.configs[0].max_exp_gain_l;
    }

    *args_data = (int)gc5035_sensorModeData;

    return ret;
}

/*--------------------------------------------------------------------------*/

static int gc5035_filltimings(struct gc_camera_module_custom_config *custom)
{
    int i, j;
    u32 win_h_off = 0, win_v_off = 0;
    u32 vts = 0, hts = 0, winv = 0, winh = 0;
    struct gc_camera_module_config *config;
    struct gc_camera_module_timings *timings;
    struct gc_camera_module_reg *reg_table;
    int reg_table_num_entries;

    for (i = 0; i < custom->num_configs; i++)
    {
        config = &custom->configs[i];
        reg_table = config->reg_table;
        reg_table_num_entries = config->reg_table_num_entries;
        timings = &config->timings;

//        if (timings == NULL)
//        {
//            timings = (struct gc_camera_module_timings *)rt_malloc(sizeof(struct gc_camera_module_timings));
//            rt_memset(timings, 0x00, sizeof(struct gc_camera_module_timings));
//        }
//      memset(timings, 0x00, sizeof(*timings));
        for (j = 0; j < reg_table_num_entries; j++)
        {
            switch (reg_table[j].reg)
            {
            case GC5035_TIMING_VTS_HIGH_REG:
                vts = ((reg_table[j].val << 8) |
                       (vts & 0xff));
                break;
            case GC5035_TIMING_VTS_LOW_REG:
                vts = (reg_table[j].val |
                       (vts & 0x3f00));
                break;
            case GC5035_TIMING_HTS_HIGH_REG:
                hts = ((reg_table[j].val << 8) |
                       hts);
                break;
            case GC5035_TIMING_HTS_LOW_REG:
                hts = (reg_table[j].val |
                       (hts & 0x0f00));
                break;
            case GC5035_TIMING_WINV_HIGH_REG:
                winv = ((reg_table[j].val << 8) |
                        (winv & 0xff));
                break;
            case GC5035_TIMING_WINV_LOW_REG:
                winv = (reg_table[j].val |
                        (winv & 0xff00));
                break;
            case GC5035_TIMING_WINH_HIGH_REG:
                winh = ((reg_table[j].val << 8) |
                        winh);
                break;
            case GC5035_TIMING_WINH_LOW_REG:
                winh = (reg_table[j].val |
                        (winh & 0xff00));
                break;
            case GC5035_H_WIN_OFF_HIGH_REG:
                win_h_off = (reg_table[j].val & 0xf) << 8;
                break;
            case GC5035_H_WIN_OFF_LOW_REG:
                win_h_off |= (reg_table[j].val & 0xff);
                break;
            case GC5035_V_WIN_OFF_HIGH_REG:
                win_v_off = (reg_table[j].val & 0xf) << 8;
                break;
            case GC5035_V_WIN_OFF_LOW_REG:
                win_v_off |= (reg_table[j].val & 0xff);
                break;
            case GC5035_HORIZONTAL_OUTPUT_SIZE_HIGH_REG:
                timings->sensor_output_width =
                    ((reg_table[j].val << 8) |
                     (timings->sensor_output_width & 0xff));
                break;
            case GC5035_HORIZONTAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_width =
                    (reg_table[j].val |
                     (timings->sensor_output_width & 0xff00));
                break;
            case GC5035_VERTICAL_OUTPUT_SIZE_HIGH_REG:
                timings->sensor_output_height =
                    ((reg_table[j].val << 8) |
                     (timings->sensor_output_height & 0xff));
                break;
            case GC5035_VERTICAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_height =
                    (reg_table[j].val |
                     (timings->sensor_output_height & 0xff00));
                break;
            }
        }

        /* timings->frame_length_lines = winv + 16 + vb;
        timings->line_length_pck = winh + 202 + hb; */
        timings->frame_length_lines = 0x07D0;
        timings->line_length_pck = 0x0B68;

        timings->crop_horizontal_start = win_h_off;
        timings->crop_horizontal_end = win_h_off + timings->sensor_output_width;
        timings->crop_vertical_start = win_v_off;
        timings->crop_vertical_end = win_v_off + timings->sensor_output_height;

        //timings->exp_time >>= 4;
        timings->vt_pix_clk_freq_hz =
            config->frm_intrvl.interval.denominator
            * timings->frame_length_lines
            * timings->line_length_pck;

        timings->coarse_integration_time_min =
            GC5035_COARSE_INTG_TIME_MIN;
        timings->coarse_integration_time_max_margin =
            GC5035_COARSE_INTG_TIME_MAX_MARGIN;

        /* OV Sensor do not use fine integration time. */
        timings->fine_integration_time_min =
            GC5035_FINE_INTG_TIME_MIN;
        timings->fine_integration_time_max_margin =
            GC5035_FINE_INTG_TIME_MAX_MARGIN;

        timings->binning_factor_x = 1;
        timings->binning_factor_y = 1;
    }

    return 0;
}

ret_err_t gc5035_g_sensor_configInfo(struct gc5035_dev *dev, void *args)
{

    ret_err_t ret;
    int *args_data = (int *)args;

    gc5035_sensorInfo->config_num = dev->cam_mod.custom.num_configs;
    for (u8 i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (i >= SENSOR_CONFIG_NUM)
            break;
        gc5035_sensorInfo->sensor_fmt[i] =
            pltfrm_camera_module_pix_fmt2csi2_dt(dev->cam_mod.custom.configs[i].frm_fmt.code);
        gc5035_sensorInfo->reso[i].width  = dev->cam_mod.custom.configs[i].frm_fmt.width;
        gc5035_sensorInfo->reso[i].height = dev->cam_mod.custom.configs[i].frm_fmt.height;
    }

    *args_data = (int)gc5035_sensorInfo;
    return 0;
}

static ret_err_t gc5035_s_flipMirror(struct gc5035_dev *dev, void *args)
{
    ret_err_t ret = 0;
    u8 data;
    u8 *state = (u8 *)args;
    data = GC5035_SET_PAGE_ZERO;
    ret = gc5035_write_reg(dev->i2c_bus, GC5035_REG_SET_PAGE, &data);

    switch (*state)
    {
    case GC5035_FLIPMIRROR_STATE_NORMAL:
    {
        data = GC5035_FLIPMIRROR_REGVAL_NORMAL;
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_FLIPMIRROR_REGADDR, &data);
        gc5035_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SRGGB10_1X10;
    }
    break;
    case GC5035_FLIPMIRROR_STATE_FLIPMIRROR:
    {
        data = GC5035_FLIPMIRROR_REGVAL_FLIPMIRROR;
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_FLIPMIRROR_REGADDR, &data);
        gc5035_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SBGGR10_1X10;
    }
    break;
    case GC5035_FLIPMIRROR_STATE_MIRROR:
    {
        data = GC5035_FLIPMIRROR_REGVAL_MIRROR;
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_FLIPMIRROR_REGADDR, &data);
        gc5035_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SGRBG10_1X10;
    }
    break;
    case GC5035_FLIPMIRROR_STATE_FLIP:
    {
        data = GC5035_FLIPMIRROR_REGVAL_FLIP;
        ret |= gc5035_write_reg(dev->i2c_bus, GC5035_FLIPMIRROR_REGADDR, &data);
        gc5035_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SGBRG10_1X10;
    }
    break;
    default:
    {
        GC5035_INFO(dev, "%d is wrong flip_mirror state!!!\n", *state);
        ret = RET_SYS_ERROR;
    }
    break;
    }

    if (IS_ERR_VALUE(ret))
    {
        GC5035_INFO(dev, "s_flipMirror:failed with error (%d)\n", ret);
    }

    return ret;
}

ret_err_t rk_gc5035_control(struct rk_camera_device *dev, int cmd, void *args)
{
    GC5035_DEBUG("(%s) enter cmd = %d \n", __FUNCTION__, cmd);

    ret_err_t ret = RET_SYS_EOK;
    struct gc5035_dev *gc5035;

    MACRO_ASSERT(dev != RK_NULL);
    gc5035 = (struct gc5035_dev *)dev;
#if RT_USING_GC8825_OPS
    return (gc5035->ops->control(gc5035, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_gc5035_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        gc5035_stream_on(dev);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        gc5035_stream_off(dev);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_IQ_INFO:
    {
        ret = gc5035_g_sensor_iq_info(gc5035, args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_AEC:
    {
        ret = gc5035_g_aec(gc5035, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_AEC:
    {
        ret = gc5035_s_aec(gc5035, args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = gc5035_g_fmt(gc5035, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = gc5035_s_fmt(gc5035, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_BLANK:
    {
        ret = gc5035_g_v_blank_us(gc5035, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_ITF_CFG:
    {
        ret = gc5035_g_itf_cfg(gc5035, args);
    }
    break;

    case RK_VIDIOC_SENSOR_MODE_DATA:
    {
        ret = gc5035_g_sensor_mode(gc5035, args);
    }
    break;

    case RK_VIDIOC_SENSOR_CONFIGINFO:
    {
        ret = gc5035_g_sensor_configInfo(gc5035, args);
    }
    break;

    case RK_V4L2_CID_AUTO_FPS:
    {
        ret = gc5035_auto_adjust_fps_state(gc5035, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_FRMINTVAL:
    {
        ret = gc5035_g_sensor_frm_interval(gc5035, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = gc5035_s_flipMirror(gc5035, args);
    }
    break;

    default:
        break;
    }
#endif
    GC5035_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static ret_err_t rk_gc5035_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    GC5035_DEBUG("(%s) enter \n", __FUNCTION__);

    struct gc5035_dev *gc5035;
    struct rk_camera_device *camera;
    ret_err_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    gc5035 = (struct gc5035_dev *)&g_gc5035;
    camera = (struct rk_camera_device *)(&gc5035->parent);

    cam_power_on(0);

    gc5035_write_multiple_reg(gc5035, gc5035_init_tab_2592_1944_2lane);
    ret = gc5035_g_vts(gc5035, &gc5035->cam_mod.vts_min);

    GC5035_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

static ret_err_t rk_gc5035_close(struct rk_camera_device *dev)
{
    struct gc5035_dev *gc5035;
    uint8_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    gc5035 = (struct gc5035_dev *)dev;

    cam_power_off(0);

    //GC5035_INFO(dev, "(%s) exit \n", __FUNCTION__);
    return ret;

}

struct rk_camera_ops rk_gc5035_ops =
{
    rk_gc5035_init,
    rk_gc5035_open,
    rk_gc5035_close,
    rk_gc5035_control
};

static struct gc_camera_module_custom_config gc5035_custom_config =
{
    .configs = gc5035_configs,
    .num_configs = sizeof(gc5035_configs) / sizeof(gc5035_configs[0]),
    .power_up_delays_ms = {5, 30, 30},
    .exposure_valid_frame = {4, 4},
};

int rk_camera_gc5035_init(void)
{
    ret_err_t ret;
    rk_device *device = &g_gc5035.dev;
    struct gc5035_dev *instance = &g_gc5035;
    struct rk_camera_device *camera = &instance->parent;
    camera->ops = &rk_gc5035_ops;

    rt_strncpy(instance->name, GC5035_DEVICE_NAME, rt_strlen(GC5035_DEVICE_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

#if GC5035_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        GC5035_INFO(instance, "Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        GC5035_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

    if (gc5035_sensorInfo == NULL)
    {
        gc5035_sensorInfo = (struct sensor_config_info_s *)rt_malloc(sizeof(struct sensor_config_info_s));
        if (gc5035_sensorInfo != NULL)
        {
            rt_memset(gc5035_sensorInfo, 0x00, sizeof(struct sensor_config_info_s));
        }
        else
        {
            GC5035_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    if (gc5035_sensorModeData == NULL)
    {
        gc5035_sensorModeData = (struct isp_supplemental_sensor_mode_data *)rt_malloc(sizeof(struct isp_supplemental_sensor_mode_data));
        if (gc5035_sensorModeData != NULL)
        {
            rt_memset(gc5035_sensorModeData, 0x00, sizeof(struct isp_supplemental_sensor_mode_data));
        }
        else
        {
            GC5035_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    cam_power_init(0);
    cam_power_on(0);
    ret = rt_gc5035_detect_sensor(instance);
    cam_power_off(0);

    if (ret != RET_SYS_EOK)
        goto ERR;

    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, instance->name, rt_strlen(GC5035_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);

    instance->cam_mod.custom = gc5035_custom_config;
    instance->cam_mod.active_config = gc5035_configs;
    ret = gc5035_filltimings(&instance->cam_mod.custom);
    instance->cam_mod.exp_config.gain_percent = 100;
    instance->cam_mod.state = GC_CAMERA_MODULE_STREAMING;

    GC5035_DEBUG("(%s) exit \n", __FUNCTION__);
    return 0;
ERR:
    GC5035_DEBUG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

void gc5035_detect(void)
{
    struct gc5035_dev *instance = &g_gc5035;

    GC5035_DEBUG("start to detect GC5035 for testing \n");
    GC5035_DEBUG("dev name:%s\n", instance->name);
    GC5035_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        GC5035_DEBUG("Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        GC5035_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }
    //rt_ov8825_detect_sensor(instance);
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_gc5035_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(gc5035_detect, check g_gc5035 is available or not);
#endif
#endif

#endif

