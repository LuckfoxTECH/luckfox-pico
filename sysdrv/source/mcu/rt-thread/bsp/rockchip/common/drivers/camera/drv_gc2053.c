/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_gc2053.c
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
#elif defined(__RK_OS__)
#include "driver/drv_clock.h"
#endif

#if defined(RT_USING_GC2053) || defined(CONFIG_DRIVER_GC2053)
#define GC2053_MODULE_COMPILED
#else
#undef GC2053_MODULE_COMPILED
#endif

#endif


#ifdef GC2053_MODULE_COMPILED

#define GC2053_DEBUG_INFO 0
#include "gc_camera_module.h"
#include "cam_iq_data_gc2053_v1.1.h"

#if GC2053_DEBUG_INFO
#include <stdio.h>
#define GC2053_DEBUG(...)               rk_kprintf("[GC2053]:");rk_kprintf(__VA_ARGS__)
#else
#define GC2053_DEBUG(...)
#endif


#define GC2053_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define RT_USING_GC2053_OPS 0

#define GC2053_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME                "i2c0"
#define GC2053_7BIT_ADDRESS         (0x37)
#define GC2053_REG_ID_H_ADDRESS     (0xF0)
#define GC2053_REG_ID_L_ADDRESS     (0xF1)
#define GC2053_SENSOR_ID            (0x2053)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define GC2053_PIN_CTRL_ENABLE          1
#define GC2053_I2C_DEBUG_ENABLE         1
#define GC2053_TESTPATTERN_ENABLE       0

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

#define GC2053_FETCH_GAIN(VAL)                  (VAL & 0xFF)    /* gain[7:0] */
#define GC2053_AEC_ANALOG_GAIN_REG              0xb6            /* Bits 0 -7 */
#define GC2053_AEC_DIGITAL_INT_GAIN_REG         0xb1            /* Bits 0 -3 */
#define GC2053_AEC_DIGITAL_FRAC_GAIN_REG        0xb2            /* Bits 2 -7 */

#define GC2053_AEC_EXPO_HIGH_REG                0x03            /* Exposure Bits 8-12 */
#define GC2053_AEC_EXPO_LOW_REG                 0x04            /* Exposure Bits 0-7 */

#define GC2053_FETCH_HIGH_BYTE_EXP(VAL)         ((VAL >> 8) & 0x0F)     /* 5 Bits */
#define GC2053_FETCH_LOW_BYTE_EXP(VAL)          (VAL & 0xFF)            /* 8 Bits */

#define GC2053_TIMING_VTS_HIGH_REG              0x41
#define GC2053_TIMING_VTS_LOW_REG               0x42
#define GC2053_TIMING_HTS_HIGH_REG              0x05
#define GC2053_TIMING_HTS_LOW_REG               0x06

#define GC2053_TIMING_VB_HIGH_REG               0x07
#define GC2053_TIMING_VB_LOW_REG                0x08
#define GC2053_TIMING_HB_HIGH_REG               0x05
#define GC2053_TIMING_HB_LOW_REG                0x06
#define GC2053_TIMING_WINV_HIGH_REG             0x0d
#define GC2053_TIMING_WINV_LOW_REG              0x0e
#define GC2053_TIMING_WINH_HIGH_REG             0x0f
#define GC2053_TIMING_WINH_LOW_REG              0x10
#define GC2053_FINE_INTG_TIME_MIN               0
#define GC2053_FINE_INTG_TIME_MAX_MARGIN        0
#define GC2053_COARSE_INTG_TIME_MIN             1
#define GC2053_COARSE_INTG_TIME_MAX_MARGIN      4
#define GC2053_HORIZONTAL_OUTPUT_SIZE_HIGH_REG  0x97   // P1
#define GC2053_HORIZONTAL_OUTPUT_SIZE_LOW_REG   0x98   // P1
#define GC2053_VERTICAL_OUTPUT_SIZE_HIGH_REG    0x95   // P1
#define GC2053_VERTICAL_OUTPUT_SIZE_LOW_REG     0x96   // P1
#define GC2053_H_WIN_OFF_HIGH_REG               0x93   // P1
#define GC2053_H_WIN_OFF_LOW_REG                0x94   // P1
#define GC2053_V_WIN_OFF_HIGH_REG               0x91   // P1
#define GC2053_V_WIN_OFF_LOW_REG                0x92   // P1

#define GC2053_REG_CTRL_MODE                    0x10   // P3,0x03
#define GC2053_MODE_SW_STANDBY                  0x00
#define GC2053_MODE_STREAMING                   0x90

#define GC2053_REG_SET_PAGE                     0xfe
#define GC2053_SET_PAGE_ZERO                    0x00

#define GC2053_FLIPMIRROR_STATE_NORMAL          0
#define GC2053_FLIPMIRROR_STATE_FLIPMIRROR      1
#define GC2053_FLIPMIRROR_STATE_MIRROR          2
#define GC2053_FLIPMIRROR_STATE_FLIP            3
#define GC2053_FLIPMIRROR_REGADDR               0x17
#define GC2053_FLIPMIRROR_REGVAL_NORMAL         0
#define GC2053_FLIPMIRROR_REGVAL_FLIPMIRROR     0x03
#define GC2053_FLIPMIRROR_REGVAL_MIRROR         0x01
#define GC2053_FLIPMIRROR_REGVAL_FLIP           0x02

/* timing  ------------------------------------------------------------*/

#define GC2053_EXT_CLK 24000000
//#define USE_25FPS 1
#define USE_30FPS
#define CAMERA_BOOT_RETRY_NUM 10

static const u8 regValTable[29][4] =
{
    /*0xb4 0xb3  0xb8  0xb9*/
    {0x00, 0x00, 0x01, 0x00},
    {0x00, 0x10, 0x01, 0x0c},
    {0x00, 0x20, 0x01, 0x1b},
    {0x00, 0x30, 0x01, 0x2c},
    {0x00, 0x40, 0x01, 0x3f},
    {0x00, 0x50, 0x02, 0x16},
    {0x00, 0x60, 0x02, 0x35},
    {0x00, 0x70, 0x03, 0x16},
    {0x00, 0x80, 0x04, 0x02},
    {0x00, 0x90, 0x04, 0x31},
    {0x00, 0xa0, 0x05, 0x32},
    {0x00, 0xb0, 0x06, 0x35},
    {0x00, 0xc0, 0x08, 0x04},
    {0x00, 0x5a, 0x09, 0x19},
    {0x00, 0x83, 0x0b, 0x0f},
    {0x00, 0x93, 0x0d, 0x12},
    {0x00, 0x84, 0x10, 0x00},
    {0x00, 0x94, 0x12, 0x3a},
    {0x01, 0x2c, 0x1a, 0x02},
    {0x01, 0x3c, 0x1b, 0x20},
    {0x00, 0x8c, 0x20, 0x0f},
    {0x00, 0x9c, 0x26, 0x07},
    {0x02, 0x64, 0x36, 0x21},
    {0x02, 0x74, 0x37, 0x3a},
    {0x00, 0xc6, 0x3d, 0x02},
    {0x00, 0xdc, 0x3f, 0x3f},
    {0x02, 0x85, 0x3f, 0x3f},
    {0x02, 0x95, 0x3f, 0x3f},
    {0x00, 0xce, 0x3f, 0x3f},
};

static const u32 gainLevelTable[] =
{
    64,
    74,
    89,
    102,
    127,
    147,
    177,
    203,
    260,
    300,
    361,
    415,
    504,
    581,
    722,
    832,
    1027,
    1182,
    1408,
    1621,
    1990,
    2291,
    2850,
    3282,
    4048,
    5180,
    5500,
    6744,
    7073,
};


static struct rt_mutex spi_spi2apb_lock;
#define GC2053_LOCK() rt_mutex_take(&spi_spi2apb_lock, RT_WAITING_FOREVER)
#define GC2053_UNLOCK() rt_mutex_release(&spi_spi2apb_lock)

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

struct gc2053_dev
{
    struct rk_camera_device parent;
    struct gc_camera_module cam_mod;
    rk_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];

    struct gc2053_ops *ops;

    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
    //struct rt_mutex mutex_lock;
};
typedef struct gc2053_dev *rt_gc2053_dev_t;

struct gc2053_ops
{
    ret_err_t (*init)(struct rk_camera_device *dev);
    ret_err_t (*open)(struct rk_camera_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_camera_device *dev);
    ret_err_t (*control)(struct rk_camera_device *dev, int cmd, void *arg);
};

struct gc2053_sensor_reg
{
    u8 reg_addr;
    u8 val;
};

struct gc2053_reg_t
{
    u8 page;
    u8 addr;
    u8 value;
};


bool ov_camera_module_get_testpattern_en(
    struct gc2053_dev *dev)
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


static struct gc2053_dev g_gc2053;
struct sensor_config_info_s              *gc2053_sensorInfo = NULL;
struct isp_supplemental_sensor_mode_data *gc2053_sensorModeData = NULL;


/* Actual_window_size=1920*1080 1lane*/
/* mclk=24Mhz,mipi_clock=816Mbps,frame_rate=30fps */
/* pixel_line_total=2416,line_frame_total=1133,row_time=26.629us */

static const struct gc2053_sensor_reg gc2053_init_tab_1920_1080_1lane[] =
{
    {   0xfe, 0x80},
    {   0xfe, 0x80},
    {   0xfe, 0x80},
    {   0xfe, 0x00},
    {   0xf2, 0x00}, //[1]I2C_open_ena [0]pwd_dn
    {   0xf3, 0x00}, //0f//00[3]Sdata_pad_io [2:0]Ssync_pad_io
    {   0xf4, 0x36}, //[6:4]pll_ldo_set
    {   0xf5, 0xc0}, //[7]soc_mclk_enable [6]pll_ldo_en [5:4]cp_clk_sel [3:0]cp_clk_div
    {   0xf6, 0x81}, //84//44//[7:3]wpllclk_div [2:0]refmp_div
    {   0xf7, 0x01}, //[7]refdiv2d5_en [6]refdiv1d5_en [5:4]scaler_mode [3]refmp_enb [1]div2en [0]pllmp_en
    {   0xf8, 0x22}, //22//2c//3f//2c//[7:0]pllmp_div
    {   0xf9, 0x80}, //82//42//[7:3]rpllclk_div [2:1]pllmp_prediv [0]analog_pwc
    {   0xfc, 0x8e},
    /****CISCTL & ANALOG****/
    {   0xfe, 0x00},
    {   0x87, 0x18}, //[6]aec_delay_mode
    {   0xee, 0x30}, //[5:4]dwen_sramen
    {   0xd0, 0xb7}, //ramp_en
    {   0x03, 0x04},
    {   0x04, 0x10},
    {   0x05, 0x04}, //05//05
    {   0x06, 0xB8}, //00//60//[11:0]hb
    {   0x07, 0x00},
    {   0x08, 0x19},
    {   0x09, 0x00},
    {   0x0a, 0x02}, //cisctl row start
    {   0x0b, 0x00},
    {   0x0c, 0x02}, //cisctl col start
    {   0x0d, 0x04},
    {   0x0e, 0x40},
    {   0x12, 0xe2}, //vsync_ahead_mode
    {   0x13, 0x16},
    {   0x19, 0x0a}, //ad_pipe_num
    {   0x21, 0x1c}, //eqc1fc_eqc2fc_sw
    {   0x28, 0x0a}, //16//eqc2_c2clpen_sw
    {   0x29, 0x24}, //eq_post_width
    {   0x2b, 0x04}, //c2clpen --eqc2
    {   0x32, 0xf8}, //[5]txh_en ->vref_r
    {   0x37, 0x03}, //[3:2]eqc2sel=0
    {   0x39, 0x15}, //17 //[3:0]rsgl
    {   0x43, 0x07}, //vclamp
    {   0x44, 0x40}, //0e//post_tx_width
    {   0x46, 0x0b},
    {   0x4b, 0x20}, //rst_tx_width
    {   0x4e, 0x08}, //12//ramp_t1_width
    {   0x55, 0x20}, //read_tx_width_pp
    {   0x66, 0x05}, //18//stspd_width_r1
    {   0x67, 0x05}, //40//5//stspd_width_r
    {   0x77, 0x01}, //dacin offset x31
    {   0x78, 0x00}, //dacin offset
    {   0x7c, 0x93}, //[1:0] co1comp
    {   0x8c, 0x12}, //12 ramp_t1_ref
    {   0x8d, 0x92}, //90
    {   0x90, 0x00}, //0x0: Fix FPS by P0_0x41[MSB] P0_0x42[LSB] frame_length; 0x01: min_fps set by VB+WINV,if exp is too min,the fps maye 30fps
#ifdef USE_30FPS
    {   0x41, 0x04},
    {   0x42, 0x6d},
#endif
#ifdef USE_25FPS
    {   0x41, 0x05},
    {   0x42, 0x45},
#endif

    {   0x9d, 0x10},
    {   0xce, 0x7c}, //70//78//[4:2]c1isel
    {   0xd2, 0x41}, //[5:3]c2clamp
    {   0xd3, 0xdc}, //ec//0x39[7]=0,0xd3[3]=1 rsgh=vref
    {   0xda, 0x05},
    {   0xdb, 0x00}, //ramps_width
    {   0xe6, 0x50}, //ramps offset
    /*gain*/
    {   0xb6, 0xc0},
    {   0xb0, 0x70},
    {   0xb1, 0x01},
    {   0xb2, 0x00},
    {   0xb3, 0x00},
    {   0xb4, 0x00},
    {   0xb8, 0x01},
    {   0xb9, 0x00},
    /*blk*/
    {   0x26, 0x30}, //23 //[4]д0��ȫn mode
    {   0xfe, 0x01},
    {   0x40, 0x23},
    {   0x55, 0x07},
    {   0x60, 0x00}, //[7:0]WB_offset   //0x40
    {   0xfe, 0x04},
    {   0x14, 0x78}, //g1 ratio
    {   0x15, 0x78}, //r ratio
    {   0x16, 0x78}, //b ratio
    {   0x17, 0x78}, //g2 ratio
    /*window*/
    {   0xfe, 0x01},
    {   0x92, 0x00}, //win y1
    {   0x94, 0x03}, //win x1
    {   0x95, 0x04},
    {   0x96, 0x38}, //[10:0]out_height
    {   0x97, 0x07},
    {   0x98, 0x80}, //[11:0]out_width
    /*ISP*/
    {   0xfe, 0x01},
    {   0x01, 0x05},//03//[3]dpc blending mode [2]noise_mode [1:0]center_choose 2b'11:median 2b'10:avg 2'b00:near
    {   0x02, 0x89}, //[7:0]BFF_sram_mode
    {   0x04, 0x01}, //[0]DD_en
    {   0x07, 0xa6},
    {   0x08, 0xa9},
    {   0x09, 0xa8},
    {   0x0a, 0xa7},
    {   0x0b, 0xff},
    {   0x0c, 0xff},
    {   0x0f, 0x00},
    {   0x50, 0x1c},
    {   0x89, 0x03},
    {   0xfe, 0x04},
    {   0x28, 0x86}, //84
    {   0x29, 0x86}, //84
    {   0x2a, 0x86}, //84
    {   0x2b, 0x68}, //84
    {   0x2c, 0x68}, //84
    {   0x2d, 0x68}, //84
    {   0x2e, 0x68}, //83
    {   0x2f, 0x68}, //82
    {   0x30, 0x4f}, //82
    {   0x31, 0x68}, //82
    {   0x32, 0x67}, //82
    {   0x33, 0x66}, //82
    {   0x34, 0x66}, //82
    {   0x35, 0x66}, //82
    {   0x36, 0x66}, //64
    {   0x37, 0x66}, //68
    {   0x38, 0x62},
    {   0x39, 0x62},
    {   0x3a, 0x62},
    {   0x3b, 0x62},
    {   0x3c, 0x62},
    {   0x3d, 0x62},
    {   0x3e, 0x62},
    {   0x3f, 0x62},
    /****DVP & MIPI****/
    {   0xfe, 0x01},
    {   0x9a, 0x06},
    {   0xfe, 0x00},
    {   0x7b, 0x2a}, //[7:6]updn [5:4]drv_high_data [3:2]drv_low_data [1:0]drv_pclk
    {   0x23, 0x2d}, //[3]rst_rc [2:1]drv_sync [0]pwd_rc
    {   0xfe, 0x03},
    {   0x01, 0x27}, //20//27[6:5]clkctr [2]phy-lane1_en [1]phy-lane0_en [0]phy_clk_en
    {   0x02, 0x56}, //[7:6]data1ctr [5:4]data0ctr [3:0]mipi_diff
    {   0x03, 0x8e},
    {   0x12, 0x80},
    {   0x13, 0x07}, //LWC
    {   0x15, 0x10}, //[1:0]clk_lane_mode
    {   0xfe, 0x00},
    {   0x3e, 0x90},
    {REG_END, 0x00},
};


#ifdef USE_30FPS
static struct gc_camera_module_config gc2053_configs[] =
{
    {
        .name = "1920x1080_30fps",
        .frm_fmt = {
            .width = 1920,
            .height = 1080,
            .code = V4L2_MBUS_FMT_SRGGB10_1X10
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
        .reg_table = (void *)gc2053_init_tab_1920_1080_1lane,
        .reg_table_num_entries =
        ARRAY_SIZE(gc2053_init_tab_1920_1080_1lane),
        .v_blanking_time_us = 5000,
        .ignore_measurement_check = 1,
        PLTFRM_CAM_ITF_MIPI_CFG(0, 1, 816, GC2053_EXT_CLK)
    }
};
#endif

#ifdef USE_25FPS
static struct gc_camera_module_config gc2053_configs[] =
{
    {
        .name = "1920x1080_30fps",
        .frm_fmt = {
            .width = 1920,
            .height = 1080,
            .code = V4L2_MBUS_FMT_SRGGB10_1X10
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
        .reg_table = (void *)gc2053_init_tab_1920_1080_1lane,
        .reg_table_num_entries =
        ARRAY_SIZE(gc2053_init_tab_1920_1080_1lane),
        .v_blanking_time_us = 5000,
        .ignore_measurement_check = 1,
        PLTFRM_CAM_ITF_MIPI_CFG(0, 1, 816, GC2053_EXT_CLK)
    }
};

#endif

static ret_err_t gc2053_g_vts(struct gc2053_dev *dev, u32 *vts);

u32 gc2053_gcd(u32 a, u32 b)
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

static ret_err_t gc2053_write_reg(rk_i2c_bus_device *bus,
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

        msgs.addr = GC2053_7BIT_ADDRESS;
        msgs.flags = RT_I2C_WR;
        msgs.buf = send_buf;
        msgs.len = 2;
        ret = rt_i2c_transfer(bus, &msgs, 1);

        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
    }
    GC2053_DEBUG("(%s):s0.0 i2c_bus error\n");
    return RET_SYS_ERROR;
}

static ret_err_t gc2053_read_reg(rk_i2c_bus_device *bus,
                                 uint8_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = (reg & 0xff);
    msg[0].addr = GC2053_7BIT_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 1;
    msg[0].buf = send_buf;

    msg[1].addr = GC2053_7BIT_ADDRESS;
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

static ret_err_t gc2053_write_multiple_reg(struct gc2053_dev *dev,
        const struct gc2053_sensor_reg *reg)
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
            ret = gc2053_write_reg(i2c_bus, reg[i].reg_addr, &reg[i].val);
            if (ret != RET_SYS_EOK)
            {
                num = num + 1;
                rk_kprintf("[GC2053]: write init_table failed!!! num = %d\n", num);
            }
        }
    }
    return ret;

}

static ret_err_t rt_gc2053_detect_sensor(struct gc2053_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    ret_err_t ret;
    uint8_t pid, ver;
    uint16_t chipid;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    uint32_t cnt = 0;

    GC2053_DEBUG("(%s) enter \n", __FUNCTION__);

RETRY:
    cnt++;
    rt_thread_mdelay(1);
    ret = gc2053_read_reg(bus, GC2053_REG_ID_H_ADDRESS, &pid);
    if (ret == RET_SYS_EOK)
    {
        ret = gc2053_read_reg(bus, GC2053_REG_ID_L_ADDRESS, &ver);
        chipid = SENSOR_ID(pid, ver);
        GC2053_INFO(dev, "Info: GC2053 pid:0x%x,ver:0x%x!\n", pid, ver);
        if (chipid == GC2053_SENSOR_ID)
        {
            GC2053_INFO(dev, "Info: GC2053 detected successfully !!!\n");
            ret = RET_SYS_EOK;
        }
        else
        {
            GC2053_INFO(dev, "Info: GC2053 detected failed,not match:%d != %d !!!\n",
                        GC2053_SENSOR_ID, chipid);
            ret = -RT_EINVAL;
            if (cnt > CAMERA_BOOT_RETRY_NUM)
                goto EXIT;
            goto RETRY;
        }
    }
    else
    {
        GC2053_INFO(dev, "Warning: GC2053 i2c read failed !!!\n");
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }
EXIT:
    GC2053_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static void gc2053_stream_on(struct rk_camera_device *dev)
{
    GC2053_DEBUG("(%s) enter \n", gc2053_stream_on);
    MACRO_ASSERT(dev != RK_NULL);

    struct gc2053_dev *gc2053;
    gc2053 = (struct gc2053_dev *)dev;
    ret_err_t ret = RET_SYS_EOK;
    u8 data;
    ret = gc2053_g_vts(gc2053, &gc2053->cam_mod.vts_min);

    data = 0x03;
    ret = gc2053_write_reg(dev->i2c_bus, 0xfe, &data);
    data = 0x90;
    ret |= gc2053_write_reg(dev->i2c_bus, 0x10, &data);
    data = 0x00;
    ret |= gc2053_write_reg(dev->i2c_bus, 0xfe, &data);

    if (ret != RET_SYS_EOK)
    {
        GC2053_DEBUG("stream on failed!!! \n", __FUNCTION__);
    }

    GC2053_DEBUG("(%s) exit \n", __FUNCTION__);
}

static void gc2053_stream_off(struct rk_camera_device *dev)
{
    uint8_t val;
    rk_i2c_bus_device *i2c_bus;
    struct gc2053_dev *gc2053 = (struct gc2053_dev *)dev;
    int ret;

    GC2053_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);
    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        val = 0x03;
        ret = gc2053_write_reg(i2c_bus, 0xfe, &val);
        val = 0x00;
        ret |= gc2053_write_reg(i2c_bus, 0x10, &val);
        val = 0x00;
        ret |= gc2053_write_reg(i2c_bus, 0xfe, &val);

        if (ret != RET_SYS_EOK)
        {
            GC2053_INFO(dev, "stream off failed with %d!\n", ret);
        }
    }
    else
    {
        GC2053_INFO(dev, "Err: not find out i2c bus!\n");
    }

    GC2053_DEBUG("(%s) exit \n", __FUNCTION__);
}

ret_err_t rk_gc2053_init(struct rk_camera_device *dev)
{
    GC2053_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct gc2053_dev *gc2053;

    gc2053 = (struct gc2053_dev *)&dev;
#if RT_USING_GC2053_OPS
    if (gc2053->ops->init)
    {
        return (gc2053->ops->init(gc2053));
    }
#else

    if (gc2053)
    {
        gc2053->i2c_bus = (rk_i2c_bus_device *)rt_device_find(gc2053->i2c_name);

        if (!gc2053->i2c_bus)
        {
            GC2053_DEBUG("Warning:not find i2c source:%s !!!\n",
                         gc2053->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            GC2053_DEBUG("(%s):s0 find i2c_bus:%s\n", gc2053->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    GC2053_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

ret_err_t gc2053_g_sensor_iq_info(struct gc2053_dev *dev, void *args)
{
    ret_err_t ret = 0;
    struct rk_camera_iq_info *info = (struct rk_camera_iq_info *)args;
    info->addr = (void *)cam_iq_data;
    return ret;
}

/*--------------------------------------------------------------------------*/

static ret_err_t gc2053_g_vts(struct gc2053_dev *dev, u32 *vts)
{
    ret_err_t ret = 0;
    u8 data;

    u8 msb, lsb;

    data = GC2053_SET_PAGE_ZERO;
    ret |= gc2053_write_reg(dev->i2c_bus, GC2053_REG_SET_PAGE, &data);

    ret |= gc2053_read_reg(dev->i2c_bus, GC2053_TIMING_VTS_HIGH_REG, &msb);
    if (ret != RET_SYS_EOK)
        goto err;

    ret |= gc2053_read_reg(dev->i2c_bus, GC2053_TIMING_VTS_LOW_REG, &lsb);
    if (ret != RET_SYS_EOK)
        goto err;

    *vts = msb & 0x3f;
    *vts = (*vts << 8) | lsb;

    return RET_SYS_EOK;

err:
    GC2053_INFO(dev, "failed with error (%d)\n", ret);
    return ret;

}

/* ======================================================================== */

static ret_err_t gc2053_s_vts(struct gc2053_dev *dev,
                              u32 vts)
{
    ret_err_t ret = 0;
    u8 data;
    struct gc_camera_module *cam_mod = &dev->cam_mod;

    if (vts > 0x3fff)
    {
        vts = 0x3fff;
//        GC2053_INFO(dev, "s_vts:WARNING:actual frame length = 0x%x\n", vts);
    }

    if (vts < cam_mod->vts_min)
    {
        vts = cam_mod->vts_min;
//        GC2053_INFO(dev, "s_vts:WARNING:actual frame length = 0x%x\n", vts);

    }

    data = GC2053_SET_PAGE_ZERO;
    ret |= gc2053_write_reg(dev->i2c_bus, GC2053_REG_SET_PAGE, &data);

    data = vts & 0xFF;
    ret |= gc2053_write_reg(dev->i2c_bus, GC2053_TIMING_VTS_LOW_REG, &data);
    data = (vts >> 8) & 0x3F;
    ret |= gc2053_write_reg(dev->i2c_bus, GC2053_TIMING_VTS_HIGH_REG, &data);

    if (IS_ERR_VALUE(ret))
    {
        GC2053_INFO(dev, "s_vts:failed with error (%d)\n", ret);
    }
    else
    {
//        GC2053_DEBUG(dev, "updated vts = 0x%x,vts_min=0x%x\n", vts, cam_mod->vts_min);
        cam_mod->vts_cur = vts;
    }
    return ret;

}

/* ======================================================================== */

ret_err_t gc2053_auto_adjust_fps_state(struct gc2053_dev *dev, void *args)
{

    ret_err_t ret;
    bool *auto_adjust_fps = (bool *)args;
    dev->cam_mod.auto_adjust_fps = *auto_adjust_fps;
//    GC2053_INFO(dev, "state of gc2053_auto_adjust_fps is : %d.\n", dev->cam_mod.auto_adjust_fps);
    return RET_SYS_EOK;
}

/* ======================================================================== */

static ret_err_t gc2053_auto_adjust_fps(struct gc2053_dev *dev,
                                        u32 exp_time)
{
    ret_err_t ret = 0;
    u32 vts = 0;
    u8 data;

    struct gc_camera_module *cam_mod = &dev->cam_mod;

    if ((exp_time + GC2053_COARSE_INTG_TIME_MAX_MARGIN)
            > cam_mod->vts_min)
        vts = exp_time + GC2053_COARSE_INTG_TIME_MAX_MARGIN;
    else
        vts = cam_mod->vts_min;

    if (vts > 0x3fff) vts = 0x3fff;

    data = GC2053_SET_PAGE_ZERO;
    ret |= gc2053_write_reg(dev->i2c_bus, GC2053_REG_SET_PAGE, &data);

    data = vts & 0xFF;
    ret |= gc2053_write_reg(dev->i2c_bus, GC2053_TIMING_VTS_LOW_REG, &data);
    data = (vts >> 8) & 0x3F;
    ret |= gc2053_write_reg(dev->i2c_bus, GC2053_TIMING_VTS_HIGH_REG, &data);

    if (IS_ERR_VALUE(ret))
    {
        GC2053_INFO(dev, "failed with error (%d)\n", ret);
    }
    else
    {
        GC2053_DEBUG(dev, "updated vts = 0x%x,vts_min=0x%x\n", vts, cam_mod->vts_min);
        cam_mod->vts_cur = vts;
    }
    return ret;

}

/* ======================================================================== */

static ret_err_t gc2053_s_aec(struct gc2053_dev *dev, void *args)
{
    int i;
    u8 data;
    ret_err_t ret = 0;
    struct gc_camera_module *cam_mod = &dev->cam_mod;
    struct HAL_ISP_Set_Exp_s *aec = (struct HAL_ISP_Set_Exp_s *)args;

//    GC2053_INFO(dev,
//                "exp_time = %d lines, gain = %d\n", aec->exposure, aec->gain);

    if ((cam_mod->state == GC_CAMERA_MODULE_SW_STANDBY) ||
            (cam_mod->state == GC_CAMERA_MODULE_STREAMING))
    {
        u32 d_gain_code;
        u32 gain = aec->gain;
        u32 exp_time = aec->exposure;

        gain = gain * aec->gain_percent / 100;

        if (cam_mod->auto_adjust_fps)
        {
            ret |= gc2053_auto_adjust_fps(dev, exp_time);
        }

        for (i = 0; i < ARRAY_SIZE(gainLevelTable); i++)
        {
            if ((gainLevelTable[i] <= gain) && (gain < gainLevelTable[i + 1]))
            {
                break;
            }
        }

        d_gain_code = gain * 64 / gainLevelTable[i];

        data = GC2053_SET_PAGE_ZERO;
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_REG_SET_PAGE, &data);

        data = regValTable[i][0];
        ret |= gc2053_write_reg(dev->i2c_bus, 0xb4, &data);

        data = regValTable[i][1];
        ret |= gc2053_write_reg(dev->i2c_bus, 0xb3, &data);

        data = regValTable[i][2];
        ret |= gc2053_write_reg(dev->i2c_bus, 0xb8, &data);

        data = regValTable[i][3];
        ret |= gc2053_write_reg(dev->i2c_bus, 0xb9, &data);

        data = d_gain_code >> 6;
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_AEC_DIGITAL_INT_GAIN_REG, &data);

        data = (d_gain_code << 2) & 0xFC;
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_AEC_DIGITAL_FRAC_GAIN_REG, &data);

//        GC2053_INFO(dev,
//                    "gain = 0x%x, d_gain_code = 0x%x\n", gain, d_gain_code);

        if (exp_time < 1)
        {
            exp_time = 1;
        }
        if (exp_time > 8191)
        {
            exp_time = 8191;
        }

        data = GC2053_FETCH_HIGH_BYTE_EXP(exp_time);
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_AEC_EXPO_HIGH_REG, &data);

        data = GC2053_FETCH_LOW_BYTE_EXP(exp_time);
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_AEC_EXPO_LOW_REG, &data);

        if (ret == RET_SYS_EOK)
        {
//            GC2053_INFO(dev,
//                        "gain = 0x%x, a_gain_code = 0x%x, d_gain_code = 0x%x\n",
//                        gain, gain, d_gain_code);
            cam_mod->exp_config.gain = gain;
            cam_mod->exp_config.exp_time = exp_time;
        }

        if (!cam_mod->auto_adjust_fps)
        {
            ret |= gc2053_s_vts(dev, cam_mod->exp_config.vts_value);
        }
    }

    if (ret != RET_SYS_EOK)
    {
        GC2053_INFO(dev, "s_aec:failed with error (%d)\n", ret);
    }

    return ret;
}

/* ======================================================================== */

/*
static ret_err_t gc2053_g_aec(struct gc2053_dev *dev, void *args)
{
    ret_err_t ret;
    struct HAL_ISP_Set_Exp_s *aec_value = (struct HAL_ISP_Set_Exp_s *)args;
    struct gc_camera_module *cam_mod = &dev->cam_mod;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    u32 shutter;
    u8 data;
    u8 analog_gain = 0;

    data = GC2053_SET_PAGE_ZERO;
    ret |= gc2053_write_reg(dev->i2c_bus,
                            GC2053_REG_SET_PAGE, &data);

    ret |= gc2053_read_reg(dev->i2c_bus,
                           GC2053_AEC_ANALOG_GAIN_REG, &analog_gain);

    if (ret == RET_SYS_EOK)
    {
        aec_value->gain = analog_gain;
        cam_mod->exp_config.gain = aec_value->gain;
    }

    ret |= gc2053_read_reg(dev->i2c_bus,
                           GC2053_AEC_EXPO_HIGH_REG, &data);
    shutter = (data & 0x0f) << 8;
    ret |= gc2053_read_reg(dev->i2c_bus,
                           GC2053_AEC_EXPO_LOW_REG, &data);
    shutter = shutter | (data & 0xff);

    if (ret == RET_SYS_EOK)
    {
        aec_value->exposure = shutter;
        cam_mod->exp_config.exp_time = aec_value->exposure;
    }
    return ret;
}
*/
/* ======================================================================== */

ret_err_t gc2053_g_fmt(struct gc2053_dev *dev, void *args)
{
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    if (dev->cam_mod.active_config)
    {
        fmt->code = dev->cam_mod.active_config->frm_fmt.code;
        fmt->width = dev->cam_mod.active_config->frm_fmt.width;
        fmt->height = dev->cam_mod.active_config->frm_fmt.height;
        return RET_SYS_EOK;
    }

    GC2053_INFO(dev, "Warning: no active config\n");
    return RET_SYS_ERROR;
}
/* ======================================================================== */

ret_err_t gc2053_g_sensor_frm_interval(struct gc2053_dev *dev, void *args)
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

    GC2053_INFO(dev, "no active config\n");
    return RET_SYS_ERROR;
}

/* ======================================================================== */

static void gc_camera_module_set_active_config(
    struct gc2053_dev *dev,
    struct gc_camera_module_config *new_config)
{

    if (RK_NULL == new_config)
    {
        dev->cam_mod.active_config = new_config;
        GC2053_INFO(dev, "Warning: no active config\n");
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
            GC2053_INFO(dev,
                        "activating config '%s'\n",
                        dev->cam_mod.active_config->name);
        }
    }
}

/* ======================================================================== */

static struct gc_camera_module_config *gc_camera_module_find_config(
    struct gc2053_dev *dev,
    struct v4l2_mbus_framefmt *fmt,
    struct v4l2_subdev_frame_interval *frm_intrvl)
{
    u32 i;
    u32 gcdiv;
    struct v4l2_subdev_frame_interval norm_interval;

    GC2053_INFO(dev, "fmt = 0x%x,frm_intrvl = 0x%x\n", fmt, frm_intrvl);

    if (NULL != fmt)
        GC2053_INFO(dev,
                    "gc_camera_module_find_config:%dx%d, fmt code 0x%04x\n",
                    fmt->width, fmt->height, fmt->code);

    if (NULL != frm_intrvl)
    {
        GC2053_INFO(dev,
                    "gc_camera_module_find_config:frame interval %d/%d....\n",
                    frm_intrvl->interval.numerator,
                    frm_intrvl->interval.denominator);
    }


    for (i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (NULL != frm_intrvl)
        {
            gcdiv = gc2053_gcd(dev->cam_mod.custom.configs[i].
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
        GC2053_INFO(dev,
                    "found matching config %s\n",
                    dev->cam_mod.custom.configs[i].name);
        return &dev->cam_mod.custom.configs[i];
    }
    GC2053_INFO(dev, "no matching config found\n");

    return RK_NULL;
}

/* ======================================================================== */

ret_err_t gc2053_s_fmt(struct gc2053_dev *dev, void *args)
{
    struct gc_camera_module_config *config;
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    ret_err_t ret = RET_SYS_EOK;

    GC2053_INFO(dev, "gc2053_s_fmt:%dx%d, fmt code 0x%04x\n",
                fmt->width, fmt->height, fmt->code);

    config = gc_camera_module_find_config(dev, fmt, NULL);

    if (config == NULL)
    {
        GC2053_INFO(dev,
                    "format %dx%d, code 0x%04x, not supported\n",
                    fmt->width, fmt->height, fmt->code);
        ret = RET_SYS_ERROR;
        goto err;
    }
    dev->cam_mod.frm_fmt_valid = true;
    dev->cam_mod.frm_fmt = *fmt;
    GC2053_INFO(dev, "fmt is supported,%dx%d, fmt code 0x%04x\n", fmt->width, fmt->height, fmt->code);

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
    GC2053_INFO(dev,
                "failed with error %d\n", ret);
    return ret;
}

/* ======================================================================== */

ret_err_t gc2053_g_v_blank_us(struct gc2053_dev *dev, void *args)
{
    u32 *v_blanking_time_us = (u32 *)args;
    *v_blanking_time_us = dev->cam_mod.active_config->v_blanking_time_us;
    return RET_SYS_EOK;
}

/* ======================================================================== */

ret_err_t gc2053_g_itf_cfg(struct gc2053_dev *dev, void *args)
{
    struct pltfrm_cam_itf *itf_cfg = (struct pltfrm_cam_itf *)args;
    struct gc_camera_module_config *config;

    if (dev->cam_mod.custom.num_configs <= 0)
    {
        GC2053_INFO(dev,
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

static ret_err_t gc2053_g_timings(struct gc2053_dev *dev,
                                  struct gc_camera_module_timings *timings)
{

    ret_err_t ret = 0;
    unsigned int vts;
    struct gc_camera_module *cam_mod = &dev->cam_mod;

    if (NULL == dev->cam_mod.active_config)
        goto err;

    *timings = cam_mod->active_config->timings;

    vts = (!cam_mod->vts_cur) ?
          timings->frame_length_lines :
          cam_mod->vts_cur;

    if (cam_mod->frm_intrvl_valid)
        timings->vt_pix_clk_freq_hz =
            cam_mod->frm_intrvl.interval.denominator
            * vts
            * timings->line_length_pck;
    else
        timings->vt_pix_clk_freq_hz =
            cam_mod->active_config->frm_intrvl.interval.denominator
            * vts
            * timings->line_length_pck;

    timings->frame_length_lines = vts;

    return ret;
err:
    GC2053_INFO(dev, "cam_mod:failed with error (%d)\n", RET_SYS_EEMPTY);
    return ret;

}

/* ======================================================================== */

ret_err_t gc2053_g_sensor_mode(struct gc2053_dev *dev, void *args)
{
    ret_err_t ret;
    struct gc_camera_module_timings gc_timings;

    int *args_data = (int *)args;

    ret = gc2053_g_timings(dev, &gc_timings);

    if (RET_SYS_EOK != ret)
    {
        GC2053_INFO(dev,
                    "failed with error %d\n", ret);
        return ret;
    }

    gc2053_sensorModeData->isp_input_horizontal_start = 0;
    gc2053_sensorModeData->isp_input_vertical_start   = 0;
    gc2053_sensorModeData->isp_input_width   = 1920;
    gc2053_sensorModeData->isp_input_height  = 1080;
    gc2053_sensorModeData->isp_output_width  = 1920;
    gc2053_sensorModeData->isp_output_height = 1080;

    gc2053_sensorModeData->sensor_output_width = gc_timings.sensor_output_width;
    gc2053_sensorModeData->sensor_output_height = gc_timings.sensor_output_height;
    gc2053_sensorModeData->crop_horizontal_start = gc_timings.crop_horizontal_start;
    gc2053_sensorModeData->crop_vertical_start = gc_timings.crop_vertical_start;
    gc2053_sensorModeData->crop_horizontal_end = gc_timings.crop_horizontal_end;
    gc2053_sensorModeData->crop_vertical_end = gc_timings.crop_vertical_end;
    gc2053_sensorModeData->line_length_pck = gc_timings.line_length_pck;
    gc2053_sensorModeData->frame_length_lines = gc_timings.frame_length_lines;
    gc2053_sensorModeData->vt_pix_clk_freq_hz = gc_timings.vt_pix_clk_freq_hz;
    gc2053_sensorModeData->binning_factor_x = gc_timings.binning_factor_x;
    gc2053_sensorModeData->binning_factor_y = gc_timings.binning_factor_y;
    gc2053_sensorModeData->coarse_integration_time_max_margin =
        gc_timings.coarse_integration_time_max_margin;
    gc2053_sensorModeData->coarse_integration_time_min =
        gc_timings.coarse_integration_time_min;
    gc2053_sensorModeData->fine_integration_time_max_margin =
        gc_timings.fine_integration_time_max_margin;
    gc2053_sensorModeData->fine_integration_time_min =
        gc_timings.fine_integration_time_min;

    gc2053_sensorModeData->exposure_valid_frame[0] =
        dev->cam_mod.custom.exposure_valid_frame[0];
    gc2053_sensorModeData->exposure_valid_frame[1] =
        dev->cam_mod.custom.exposure_valid_frame[1];
    gc2053_sensorModeData->lane_num = 1;
    if (dev->cam_mod.exp_config.exp_time)
    {
        gc2053_sensorModeData->exp_time = dev->cam_mod.exp_config.exp_time;
        gc2053_sensorModeData->exp_time_l = dev->cam_mod.exp_config.exp_time_l;
        gc2053_sensorModeData->exp_time_s = dev->cam_mod.exp_config.exp_time_s;
    }
    else
    {
        gc2053_sensorModeData->exp_time = gc_timings.exp_time;
        gc2053_sensorModeData->exp_time_l = gc_timings.exp_time;
        gc2053_sensorModeData->exp_time_s = gc_timings.exp_time;
    }
    if (dev->cam_mod.exp_config.gain)
    {
        gc2053_sensorModeData->gain = dev->cam_mod.exp_config.gain;
        gc2053_sensorModeData->gain_l = dev->cam_mod.exp_config.gain_l;
        gc2053_sensorModeData->gain_s = dev->cam_mod.exp_config.gain_s;
    }
    else
    {
        gc2053_sensorModeData->gain = gc_timings.gain;
        gc2053_sensorModeData->gain_l = gc_timings.gain;
        gc2053_sensorModeData->gain_s = gc_timings.gain;
    }
    if (dev->cam_mod.active_config)
    {
        gc2053_sensorModeData->max_exp_gain_h = dev->cam_mod.active_config->max_exp_gain_h;
        gc2053_sensorModeData->max_exp_gain_l = dev->cam_mod.active_config->max_exp_gain_l;
    }
    else
    {
        gc2053_sensorModeData->max_exp_gain_h = dev->cam_mod.custom.configs[0].max_exp_gain_h;
        gc2053_sensorModeData->max_exp_gain_l = dev->cam_mod.custom.configs[0].max_exp_gain_l;
    }

    *args_data = (int)gc2053_sensorModeData;

    // test part.
    // rt_kprintf(">>>[sensor]<<<    [%d, %d, %d, %d, %d].\n",
    //         gc2053_sensorModeData->coarse_integration_time_min,
    //         gc2053_sensorModeData->coarse_integration_time_max_margin,
    //         gc2053_sensorModeData->fine_integration_time_min,
    //         gc2053_sensorModeData->fine_integration_time_max_margin,
    //         gc2053_sensorModeData->frame_length_lines);
    // rt_kprintf(">>>[sensor]<<<    [%d, %d, %d, %d, %d].\n",
    //         gc2053_sensorModeData->line_length_pck,
    //         gc2053_sensorModeData->vt_pix_clk_freq_hz,
    //         gc2053_sensorModeData->crop_horizontal_start,
    //         gc2053_sensorModeData->crop_vertical_start,
    //         gc2053_sensorModeData->crop_horizontal_end);
    // rt_kprintf(">>>[sensor]<<<    [%d, %d, %d, %d, %d].\n",
    //         gc2053_sensorModeData->crop_vertical_end,
    //         gc2053_sensorModeData->sensor_output_width,
    //         gc2053_sensorModeData->sensor_output_height,
    //         gc2053_sensorModeData->isp_input_horizontal_start,
    //         gc2053_sensorModeData->isp_input_vertical_start);
    // rt_kprintf(">>>[sensor]<<<    [%d, %d, %d, %d, %d].\n",
    //         gc2053_sensorModeData->isp_input_width,
    //         gc2053_sensorModeData->isp_input_height,
    //         gc2053_sensorModeData->isp_output_width,
    //         gc2053_sensorModeData->isp_output_height,
    //         gc2053_sensorModeData->binning_factor_x);
    // rt_kprintf(">>>[sensor]<<<    [%d, %d, %d, %d, %d].\n",
    //         gc2053_sensorModeData->binning_factor_y,
    //         gc2053_sensorModeData->exposure_valid_frame[0],
    //         gc2053_sensorModeData->exposure_valid_frame[1],
    //         gc2053_sensorModeData->exp_time,
    //         gc2053_sensorModeData->gain);

    return ret;
}

/*--------------------------------------------------------------------------*/

static int gc2053_filltimings(struct gc_camera_module_custom_config *custom)
{
    int i, j;

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

        for (i = 0; i < custom->num_configs; i++)
        {
            config = &custom->configs[i];
            reg_table = config->reg_table;
            reg_table_num_entries = config->reg_table_num_entries;
            timings = &config->timings;

//          memset(timings, 0x00, sizeof(*timings));

            timings->sensor_output_width = 1920;
            timings->sensor_output_height = 1080;

            timings->frame_length_lines = 1133;  //frame_length_lines = winv+vb+20;
            timings->line_length_pck = 2416;     //line_length_pck = hb*2;

            timings->crop_horizontal_start = 3;
            timings->crop_horizontal_end = 3 + timings->sensor_output_width;
            timings->crop_vertical_start = 0;
            timings->crop_vertical_end = timings->sensor_output_height;

            //timings->exp_time >>= 4;
            timings->vt_pix_clk_freq_hz =
                config->frm_intrvl.interval.denominator
                * timings->frame_length_lines
                * timings->line_length_pck;

            timings->coarse_integration_time_min =
                GC2053_COARSE_INTG_TIME_MIN;
            timings->coarse_integration_time_max_margin =
                GC2053_COARSE_INTG_TIME_MAX_MARGIN;

            /* OV Sensor do not use fine integration time. */
            timings->fine_integration_time_min =
                GC2053_FINE_INTG_TIME_MIN;
            timings->fine_integration_time_max_margin =
                GC2053_FINE_INTG_TIME_MAX_MARGIN;

            timings->binning_factor_x = 1;
            timings->binning_factor_y = 1;
        }
    }

    return 0;

}

ret_err_t gc2053_g_sensor_configInfo(struct gc2053_dev *dev, void *args)
{

    ret_err_t ret;
    int *args_data = (int *)args;

    gc2053_sensorInfo->config_num = dev->cam_mod.custom.num_configs;
    for (u8 i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (i >= SENSOR_CONFIG_NUM)
            break;
        gc2053_sensorInfo->sensor_fmt[i] =
            pltfrm_camera_module_pix_fmt2csi2_dt(dev->cam_mod.custom.configs[i].frm_fmt.code);
        gc2053_sensorInfo->reso[i].width  = dev->cam_mod.custom.configs[i].frm_fmt.width;
        gc2053_sensorInfo->reso[i].height = dev->cam_mod.custom.configs[i].frm_fmt.height;
    }

    *args_data = (int)gc2053_sensorInfo;
    return 0;
}

static ret_err_t gc2053_s_flipMirror(struct gc2053_dev *dev, void *args)
{
    ret_err_t ret = 0;
    u8 data;
    u8 *state = (u8 *)args;
    data = GC2053_SET_PAGE_ZERO;
    ret = gc2053_write_reg(dev->i2c_bus, GC2053_REG_SET_PAGE, &data);

    switch (*state)
    {
    case GC2053_FLIPMIRROR_STATE_NORMAL:
    {
        data = GC2053_FLIPMIRROR_REGVAL_NORMAL;
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_FLIPMIRROR_REGADDR, &data);
        gc2053_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SRGGB10_1X10;
    }
    break;
    case GC2053_FLIPMIRROR_STATE_FLIPMIRROR:
    {
        data = GC2053_FLIPMIRROR_REGVAL_FLIPMIRROR;
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_FLIPMIRROR_REGADDR, &data);
        gc2053_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SBGGR10_1X10;
    }
    break;
    case GC2053_FLIPMIRROR_STATE_MIRROR:
    {
        data = GC2053_FLIPMIRROR_REGVAL_MIRROR;
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_FLIPMIRROR_REGADDR, &data);
        gc2053_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SGRBG10_1X10;
    }
    break;
    case GC2053_FLIPMIRROR_STATE_FLIP:
    {
        data = GC2053_FLIPMIRROR_REGVAL_FLIP;
        ret |= gc2053_write_reg(dev->i2c_bus, GC2053_FLIPMIRROR_REGADDR, &data);
        gc2053_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SGBRG10_1X10;
    }
    break;
    default:
    {
        GC2053_INFO(dev, "%d is wrong flip_mirror state!!!\n", *state);
        ret = RET_SYS_ERROR;
    }
    break;
    }

    if (IS_ERR_VALUE(ret))
    {
        GC2053_INFO(dev, "s_flipMirror:failed with error (%d)\n", ret);
    }

    return ret;
}

ret_err_t rk_gc2053_control(struct rk_camera_device *dev, int cmd, void *args)
{
    GC2053_DEBUG("(%s) enter cmd = %d \n", __FUNCTION__, cmd);

    ret_err_t ret = RET_SYS_EOK;
    struct gc2053_dev *gc2053;

    MACRO_ASSERT(dev != RK_NULL);
    gc2053 = (struct gc2053_dev *)dev;
#if RT_USING_GC8825_OPS
    return (gc2053->ops->control(gc2053, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_gc2053_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        gc2053_stream_on(dev);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        gc2053_stream_off(dev);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_IQ_INFO:
    {
        ret = gc2053_g_sensor_iq_info(gc2053, args);
    }
    break;
//    case RK_DEVICE_CTRL_CAMERA_GET_AEC:
//    {
//        ret = gc2053_g_aec(gc2053, args);
//    }
//    break;

    case RK_DEVICE_CTRL_CAMERA_SET_AEC:
    {
        ret = gc2053_s_aec(gc2053, args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = gc2053_g_fmt(gc2053, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = gc2053_s_fmt(gc2053, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_BLANK:
    {
        ret = gc2053_g_v_blank_us(gc2053, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_ITF_CFG:
    {
        ret = gc2053_g_itf_cfg(gc2053, args);
    }
    break;

    case RK_VIDIOC_SENSOR_MODE_DATA:
    {
        ret = gc2053_g_sensor_mode(gc2053, args);
    }
    break;

    case RK_VIDIOC_SENSOR_CONFIGINFO:
    {
        ret = gc2053_g_sensor_configInfo(gc2053, args);
    }
    break;

    case RK_V4L2_CID_AUTO_FPS:
    {
        ret = gc2053_auto_adjust_fps_state(gc2053, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_FRMINTVAL:
    {
        ret = gc2053_g_sensor_frm_interval(gc2053, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = gc2053_s_flipMirror(gc2053, args);
    }
    break;

    default:
        break;
    }
#endif
    GC2053_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static ret_err_t rk_gc2053_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    GC2053_DEBUG("(%s) enter \n", __FUNCTION__);

    struct gc2053_dev *gc2053;
    struct rk_camera_device *camera;
    ret_err_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    gc2053 = (struct gc2053_dev *)&g_gc2053;
    camera = (struct rk_camera_device *)(&gc2053->parent);

    cam_power_on(0);

    gc2053_write_multiple_reg(gc2053, gc2053_init_tab_1920_1080_1lane);
    ret = gc2053_g_vts(gc2053, &gc2053->cam_mod.vts_min);

    GC2053_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

static ret_err_t rk_gc2053_close(struct rk_camera_device *dev)
{
    struct gc2053_dev *gc2053;
    uint8_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    gc2053 = (struct gc2053_dev *)dev;

    cam_power_off(0);

    //GC2053_INFO(dev, "(%s) exit \n", __FUNCTION__);
    return ret;

}

struct rk_camera_ops rk_gc2053_ops =
{
    rk_gc2053_init,
    rk_gc2053_open,
    rk_gc2053_close,
    rk_gc2053_control
};

static struct gc_camera_module_custom_config gc2053_custom_config =
{
    .configs = gc2053_configs,
    .num_configs = sizeof(gc2053_configs) / sizeof(gc2053_configs[0]),
    .power_up_delays_ms = {5, 30, 30},
    .exposure_valid_frame = {4, 4},
};

int rk_camera_gc2053_init(void)
{
    ret_err_t ret;
    rk_device *device = &g_gc2053.dev;
    struct gc2053_dev *instance = &g_gc2053;
    struct rk_camera_device *camera = &instance->parent;
    camera->ops = &rk_gc2053_ops;

    rt_strncpy(instance->name, GC2053_DEVICE_NAME, rt_strlen(GC2053_DEVICE_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

#if GC2053_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        GC2053_INFO(instance, "Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        GC2053_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

    if (gc2053_sensorInfo == NULL)
    {
        gc2053_sensorInfo = (struct sensor_config_info_s *)rt_malloc(sizeof(struct sensor_config_info_s));
        if (gc2053_sensorInfo != NULL)
        {
            rt_memset(gc2053_sensorInfo, 0x00, sizeof(struct sensor_config_info_s));
        }
        else
        {
            GC2053_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    if (gc2053_sensorModeData == NULL)
    {
        gc2053_sensorModeData = (struct isp_supplemental_sensor_mode_data *)rt_malloc(sizeof(struct isp_supplemental_sensor_mode_data));
        if (gc2053_sensorModeData != NULL)
        {
            rt_memset(gc2053_sensorModeData, 0x00, sizeof(struct isp_supplemental_sensor_mode_data));
        }
        else
        {
            GC2053_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    cam_power_init(0);
    cam_power_on(0);
    ret = rt_gc2053_detect_sensor(instance);
    cam_power_off(0);

    if (ret != RET_SYS_EOK)
        goto ERR;

    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, instance->name, rt_strlen(GC2053_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);

    instance->cam_mod.custom = gc2053_custom_config;
    instance->cam_mod.active_config = gc2053_configs;
    ret = gc2053_filltimings(&instance->cam_mod.custom);
    instance->cam_mod.exp_config.gain_percent = 100;
    instance->cam_mod.state = GC_CAMERA_MODULE_STREAMING;

    GC2053_DEBUG("(%s) exit \n", __FUNCTION__);
    return 0;
ERR:
    if (gc2053_sensorModeData != NULL)
        rt_free(gc2053_sensorModeData);
    if (gc2053_sensorInfo != NULL)
        rt_free(gc2053_sensorInfo);
    GC2053_DEBUG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

void gc2053_detect(void)
{
    struct gc2053_dev *instance = &g_gc2053;

    GC2053_DEBUG("start to detect GC2053 for testing \n");
    GC2053_DEBUG("dev name:%s\n", instance->name);
    GC2053_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        GC2053_DEBUG("Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        GC2053_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }
    //rt_ov8825_detect_sensor(instance);
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_gc2053_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(gc2053_detect, check g_gc2053 is available or not);
#endif
#endif

#endif

