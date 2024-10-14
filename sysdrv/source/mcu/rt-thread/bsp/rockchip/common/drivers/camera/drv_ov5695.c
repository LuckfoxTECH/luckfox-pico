/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_5695.c
  * @version V0.0.1
  * @brief   video input processor driver for rk625
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
//#include "drv_clock.h"
#include "board.h"
#ifdef SOC_SWALLOW
#include "board_cam.h"
#endif
#elif defined(__RK_OS__)
#include "driver/drv_clock.h"
#endif

#if defined(RT_USING_OV5695) || defined(CONFIG_DRIVER_OV5695)
#define OV5695_MODULE_COMPILED
#else
#undef OV5695_MODULE_COMPILED
#endif

#endif

//#define USE_BINNING_HDR_MODE
#define USE_FULL_LINEAR_MODE


#ifdef OV5695_MODULE_COMPILED
#include "ov_camera_module.h"
#include "cam_iq_data_ov5695_v1.1_a2.h"

#define OV5695_DEBUG_INFO 0

#if OV5695_DEBUG_INFO
#include <stdio.h>
#define OV5695_DEBUG(...)               rk_kprintf("[OV5695]:");rk_kprintf(__VA_ARGS__)
#else
#define OV5695_DEBUG(...)
#endif

#define OV5695_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define RT_USING_OV5695_OPS 0

#define OV5695_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME                "i2c0"
#define OV5695_7BIT_ADDRESS_READ         (0x6d>>1)
#define OV5695_7BIT_ADDRESS_WRITE        (0x6c>>1)
//#define OV5695_7BIT_ADDRESS_READ         (0x21>>1)
//#define OV5695_7BIT_ADDRESS_WRITE        (0x20>>1)

#define OV5695_REG_ID_H_ADDRESS     (0x300B)
#define OV5695_REG_ID_L_ADDRESS     (0x300C)
#define OV5695_SENSOR_ID            (0x5695)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define OV5695_PIN_CTRL_ENABLE          1
#define OV5695_I2C_DEBUG_ENABLE         1
#define OV5695_TESTPATTERN_ENABLE       0

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

/* AEC  -----------------------------------------------------------------------*/
#define OV5695_again_real_min       0x10
#define OV5695_again_real_max       0xf8
#define OV5695_real_gain            1
#define OV5695_sensor_gain          0

#define OV5695_AEC_PK_LONG_EXPO_3RD_REG             0x3500 /* Exposure Bits 16-19 */
#define OV5695_AEC_PK_LONG_EXPO_2ND_REG             0x3501 /* Exposure Bits 8-15 */
#define OV5695_AEC_PK_LONG_EXPO_1ST_REG             0x3502 /* Exposure Bits 0-7 */
#define OV5695_FETCH_3RD_BYTE_EXP(VAL)              (((VAL) >> 12) & 0xF)
#define OV5695_FETCH_2ND_BYTE_EXP(VAL)              (((VAL) >> 4) & 0xFF)
#define OV5695_FETCH_1ST_BYTE_EXP(VAL)              (((VAL) & 0x0F) << 4)
#define OV5695_GAIN16_REG                           0x3509

#define OV5695_AEC_GROUP_UPDATE_ADDRESS             0x3208
#define OV5695_AEC_GROUP_UPDATE_START_DATA          0x00
#define OV5695_AEC_GROUP_UPDATE_END_DATA            0x10
#define OV5695_AEC_GROUP_UPDATE_END_LAUNCH          0xA0

#define OV5695_TIMING_HTS_HIGH_REG                  0x380c
#define OV5695_TIMING_HTS_LOW_REG                   0x380d
#define OV5695_TIMING_VTS_HIGH_REG                  0x380E
#define OV5695_TIMING_VTS_LOW_REG                   0x380F
#define OV5695_INTEGRATION_TIME_MARGIN              8
#define OV5695_FINE_INTG_TIME_MIN                   0
#define OV5695_FINE_INTG_TIME_MAX_MARGIN            0
#define OV5695_COARSE_INTG_TIME_MIN                 16
#define OV5695_COARSE_INTG_TIME_MAX_MARGIN          4
#define OV5695_TIMING_X_INC                         0x3814
#define OV5695_TIMING_Y_INC                         0x3816
#define OV5695_HORIZONTAL_START_HIGH_REG            0x3800
#define OV5695_HORIZONTAL_START_LOW_REG             0x3801
#define OV5695_VERTICAL_START_HIGH_REG              0x3802
#define OV5695_VERTICAL_START_LOW_REG               0x3803
#define OV5695_HORIZONTAL_END_HIGH_REG              0x3804
#define OV5695_HORIZONTAL_END_LOW_REG               0x3805
#define OV5695_VERTICAL_END_HIGH_REG                0x3806
#define OV5695_VERTICAL_END_LOW_REG                 0x3807
#define OV5695_HORIZONTAL_OUTPUT_SIZE_HIGH_REG      0x3808
#define OV5695_HORIZONTAL_OUTPUT_SIZE_LOW_REG       0x3809
#define OV5695_VERTICAL_OUTPUT_SIZE_HIGH_REG        0x380a
#define OV5695_VERTICAL_OUTPUT_SIZE_LOW_REG         0x380b
//#define OV5695_FLIP_MIRROR_REG                      0x3820

#define OV5695_FLIPMIRROR_STATE_NORMAL              0
#define OV5695_FLIPMIRROR_STATE_FLIPMIRROR          1
#define OV5695_FLIPMIRROR_STATE_MIRROR              2
#define OV5695_FLIPMIRROR_STATE_FLIP                3
#define OV5695_FLIPMIRROR_REGADDR                   0x3820
#define OV5695_FLIPMIRROR_STATE_NORMAL              0
#define OV5695_FLIPMIRROR_STATE_FLIPMIRROR          1
#define OV5695_FLIPMIRROR_STATE_MIRROR              2
#define OV5695_FLIPMIRROR_STATE_FLIP                3
#define OV5695_FLIPMIRROR_REGBIT_MIRROR             0x08
#define OV5695_FLIPMIRROR_REGBIT_FLIP               0x10

/* OV camera moudule config  ------------------------------------------------------------*/

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

struct sensor_config_info_s              *ov5695_sensorInfo = NULL;
struct isp_supplemental_sensor_mode_data *ov5695_sensorModeData = NULL;


#define OV5695_EXT_CLK 24000000

#define ov5695_TEST_PATTERN_REG 0x4503

#define USE_30FPS

static struct rt_mutex spi_spi2apb_lock;
#define ov5695_LOCK() rt_mutex_take(&spi_spi2apb_lock, RT_WAITING_FOREVER)
#define ov5695_UNLOCK() rt_mutex_release(&spi_spi2apb_lock)
struct ov5695_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
};

struct ov5695_dev
{
    struct rk_camera_device parent;
    struct ov_camera_module cam_mod;
    rk_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];
#if 1//RT_USING_OV5695_OPS
    struct ov5695_ops *ops;
#endif
    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
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
typedef struct ov5695_dev *rt_ov5695_dev_t;

struct ov5695_ops
{
    ret_err_t (*init)(struct rk_camera_device *dev);
    ret_err_t (*open)(struct rk_camera_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_camera_device *dev);
    ret_err_t (*control)(struct rk_camera_device *dev, int cmd, void *arg);
};

bool ov_camera_module_get_testpattern_en(
    struct ov5695_dev *dev)
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

static struct ov5695_dev g_ov5695;
static struct otp_struct g_otp_info = {0};

static ret_err_t ov5695_g_VTS(struct ov5695_dev *dev, u32 *vts);
static int apply_otp(struct ov5695_dev *dev, struct otp_struct *otp_ptr);


#ifdef USE_BINNING_HDR_MODE
/* MCLK:24MHz  1296x972  60fps   mipi 2lane   840Mbps/lane */
/*global setting*/
static const struct ov5695_sensor_reg ov5695_init_tab_1296_972_30fps[] =
{
    {   0x0103, 0x01},
    {   0x0100, 0x00},
    {   0x0300, 0x04},
    {   0x0301, 0x00},
    {   0x0302, 0x69},
    {   0x0303, 0x00},
    {   0x0304, 0x00},
    {   0x0305, 0x01},
    {   0x0307, 0x00},
    {   0x030b, 0x00},
    {   0x030c, 0x00},
    {   0x030d, 0x1e},
    {   0x030e, 0x04},
    {   0x030f, 0x03},
    {   0x0312, 0x01},
    {   0x3000, 0x00},
    {   0x3002, 0x21},
    {   0x3016, 0x32},
    {   0x3022, 0x51},
    {   0x3106, 0x15},
    {   0x3107, 0x01},
    {   0x3108, 0x05},
    {   0x3500, 0x00},
    {   0x3501, 0x3e},
    {   0x3502, 0x00},
    {   0x3503, 0x08},
    {   0x3504, 0x03},
    {   0x3505, 0x8c},
    {   0x3507, 0x03},
    {   0x3508, 0x00},
    {   0x3509, 0x10},
    {   0x350c, 0x00},
    {   0x350d, 0x80},
    {   0x3510, 0x00},
    {   0x3511, 0x02},
    {   0x3512, 0x00},
    {   0x3601, 0x55},
    {   0x3602, 0x58},
    {   0x3611, 0x58},
    {   0x3614, 0x30},
    {   0x3615, 0x77},
    {   0x3621, 0x08},
    {   0x3624, 0x40},
    {   0x3633, 0x0c},
    {   0x3634, 0x0c},
    {   0x3635, 0x0c},
    {   0x3636, 0x0c},
    {   0x3638, 0x00},
    {   0x3639, 0x00},
    {   0x363a, 0x00},
    {   0x363b, 0x00},
    {   0x363c, 0xff},
    {   0x363d, 0xfa},
    {   0x3650, 0x44},
    {   0x3651, 0x44},
    {   0x3652, 0x44},
    {   0x3653, 0x44},
    {   0x3654, 0x44},
    {   0x3655, 0x44},
    {   0x3656, 0x44},
    {   0x3657, 0x44},
    {   0x3660, 0x00},
    {   0x3661, 0x00},
    {   0x3662, 0x00},
    {   0x366a, 0x00},
    {   0x366e, 0x0c},
    {   0x3673, 0x04},
    {   0x3700, 0x14},
    {   0x3703, 0x0c},
    {   0x3706, 0x24},
    {   0x3714, 0x27},
    {   0x3715, 0x01},
    {   0x3716, 0x00},
    {   0x3717, 0x02},
    {   0x3733, 0x10},
    {   0x3734, 0x40},
    {   0x373f, 0xa0},
    {   0x3765, 0x20},
    {   0x37a1, 0x1d},
    {   0x37a8, 0x26},
    {   0x37ab, 0x14},
    {   0x37c2, 0x04},
    {   0x37c3, 0xf0},
    {   0x37cb, 0x09},
    {   0x37cc, 0x13},
    {   0x37cd, 0x1f},
    {   0x37ce, 0x1f},
    {   0x3800, 0x00},
    {   0x3801, 0x00},
    {   0x3802, 0x00},
    {   0x3803, 0x00},
    {   0x3804, 0x0a},
    {   0x3805, 0x3f},
    {   0x3806, 0x07},
    {   0x3807, 0xaf},
    {   0x3808, 0x05},
    {   0x3809, 0x10},
    {   0x380a, 0x03},
    {   0x380b, 0xcc},
    {   0x380c, 0x02},
    {   0x380d, 0xe4},
    {   0x380e, 0x07},
    {   0x380f, 0xe8},
    {   0x3810, 0x00},
    {   0x3811, 0x00},
    {   0x3812, 0x00},
    {   0x3813, 0x06},
    {   0x3814, 0x03},
    {   0x3815, 0x01},
    {   0x3816, 0x03},
    {   0x3817, 0x01},
    {   0x3818, 0x00},
    {   0x3819, 0x00},
    {   0x381a, 0x00},
    {   0x381b, 0x01},
    {   0x3820, 0x83},
    {   0x3821, 0x01},
    {   0x3c80, 0x08},
    {   0x3c82, 0x00},
    {   0x3c83, 0x00},
    {   0x3c88, 0x00},
    {   0x3d85, 0x14},
    {   0x3f02, 0x08},
    {   0x3f03, 0x10},
    {   0x4008, 0x02},
    {   0x4009, 0x09},
    {   0x404e, 0x20},
    {   0x4501, 0x00},
    {   0x4502, 0x10},
    {   0x4800, 0x00},
    {   0x481f, 0x2a},
    {   0x4837, 0x13},
    {   0x5000, 0x13},
    {   0x5780, 0x3e},
    {   0x5781, 0x0f},
    {   0x5782, 0x44},
    {   0x5783, 0x02},
    {   0x5784, 0x01},
    {   0x5785, 0x01},
    {   0x5786, 0x00},
    {   0x5787, 0x04},
    {   0x5788, 0x02},
    {   0x5789, 0x0f},
    {   0x578a, 0xfd},
    {   0x578b, 0xf5},
    {   0x578c, 0xf5},
    {   0x578d, 0x03},
    {   0x578e, 0x08},
    {   0x578f, 0x0c},
    {   0x5790, 0x08},
    {   0x5791, 0x06},
    {   0x5792, 0x00},
    {   0x5793, 0x52},
    {   0x5794, 0xa3},
    {   0x5b00, 0x00},
    {   0x5b01, 0x1c},
    {   0x5b02, 0x00},
    {   0x5b03, 0x7f},
    {   0x5b05, 0x6c},
    {   0x5e10, 0xfc},
    {   0x4010, 0xf1},
    {   0x3503, 0x08},
    {   0x3505, 0x8c},
    {   0x3507, 0x03},
    {   0x3508, 0x00},
    {   0x3509, 0xf8},
    {   REG_END, 0x00}
};
#endif

/* MCLK:24MHz  2592x1944  30fps   mipi 2lane   840Mbps/lane */

#ifdef USE_FULL_LINEAR_MODE
static const struct ov5695_sensor_reg ov5695_init_tab_2592_1944_30fps[] =
{
//global setting
    {   0x0103, 0x01},
    {   0x0100, 0x00},
    {   0x0300, 0x04},
    {   0x0301, 0x00},
    {   0x0302, 0x69},
    {   0x0303, 0x00},
    {   0x0304, 0x00},
    {   0x0305, 0x01},
    {   0x0307, 0x00},
    {   0x030b, 0x00},
    {   0x030c, 0x00},
    {   0x030d, 0x1e},
    {   0x030e, 0x04},
    {   0x030f, 0x03},
    {   0x0312, 0x01},
    {   0x3000, 0x00},
    {   0x3002, 0x21},
    {   0x3022, 0x51},
    {   0x3106, 0x15},
    {   0x3107, 0x01},
    {   0x3108, 0x05},
    {   0x3500, 0x00},
    {   0x3501, 0x7e},
    {   0x3502, 0x00},
    {   0x3503, 0x08},
    {   0x3504, 0x03},
    {   0x3505, 0x8c},
    {   0x3507, 0x03},
    {   0x3508, 0x00},
    {   0x3509, 0x10},
    {   0x350c, 0x00},
    {   0x350d, 0x80},
    {   0x3510, 0x00},
    {   0x3511, 0x02},
    {   0x3512, 0x00},
    {   0x3601, 0x55},
    {   0x3602, 0x58},
    {   0x3614, 0x30},
    {   0x3615, 0x77},
    {   0x3621, 0x08},
    {   0x3624, 0x40},
    {   0x3633, 0x0c},
    {   0x3634, 0x0c},
    {   0x3635, 0x0c},
    {   0x3636, 0x0c},
    {   0x3638, 0x00},
    {   0x3639, 0x00},
    {   0x363a, 0x00},
    {   0x363b, 0x00},
    {   0x363c, 0xff},
    {   0x363d, 0xfa},
    {   0x3650, 0x44},
    {   0x3651, 0x44},
    {   0x3652, 0x44},
    {   0x3653, 0x44},
    {   0x3654, 0x44},
    {   0x3655, 0x44},
    {   0x3656, 0x44},
    {   0x3657, 0x44},
    {   0x3660, 0x00},
    {   0x3661, 0x00},
    {   0x3662, 0x00},
    {   0x366a, 0x00},
    {   0x366e, 0x18},
    {   0x3673, 0x04},
    {   0x3700, 0x14},
    {   0x3703, 0x0c},
    {   0x3715, 0x01},
    {   0x3733, 0x10},
    {   0x3734, 0x40},
    {   0x373f, 0xa0},
    {   0x3765, 0x20},
    {   0x37a1, 0x1d},
    {   0x37a8, 0x26},
    {   0x37ab, 0x14},
    {   0x37c2, 0x04},
    {   0x37cb, 0x09},
    {   0x37cc, 0x13},
    {   0x37cd, 0x1f},
    {   0x37ce, 0x1f},
    {   0x3800, 0x00},
    {   0x3801, 0x00},
    {   0x3802, 0x00},
    {   0x3803, 0x04},
    {   0x3804, 0x0a},
    {   0x3805, 0x3f},
    {   0x3806, 0x07},
    {   0x3807, 0xab},
    {   0x3808, 0x0a},
    {   0x3809, 0x20},
    {   0x380a, 0x07},
    {   0x380b, 0x98},
    {   0x380c, 0x02},
    {   0x380d, 0xe4},
    {   0x380e, 0x07},
    {   0x380f, 0xe8},
    {   0x3810, 0x00},
    {   0x3811, 0x06},
    {   0x3812, 0x00},
    {   0x3813, 0x08},
    {   0x3814, 0x01},
    {   0x3815, 0x01},
    {   0x3816, 0x01},
    {   0x3817, 0x01},
    {   0x3818, 0x00},
    {   0x3819, 0x00},
    {   0x381a, 0x00},
    {   0x381b, 0x01},
    {   0x3820, 0x80},
    {   0x3821, 0x00},
    {   0x3c80, 0x08},
    {   0x3c82, 0x00},
    {   0x3c83, 0x00},
    {   0x3c88, 0x00},
    {   0x3d85, 0x14},
    {   0x3f02, 0x08},
    {   0x3f03, 0x10},
    {   0x4008, 0x04},
    {   0x4009, 0x13},
    {   0x404e, 0x20},
    {   0x4501, 0x00},
    {   0x4502, 0x10},
    {   0x4800, 0x00},
    {   0x4819, 0xc0}, /* HS_Zero */
    //{   0x481B, 0x4b}, /* HS_TRAIL */
    {   0x481f, 0x2a},
    {   0x4825, 0x64}, /* LPX_P */
    {   0x4826, 0x49}, /* HS_PREPARE MIN */
    {   0x4827, 0x78}, /* HS_PREPARE MAX */
    {   0x4837, 0x13},
    {   0x5000, 0x17},
    {   0x5780, 0x3e},
    {   0x5781, 0x0f},
    {   0x5782, 0x44},
    {   0x5783, 0x02},
    {   0x5784, 0x01},
    {   0x5785, 0x01},
    {   0x5786, 0x00},
    {   0x5787, 0x04},
    {   0x5788, 0x02},
    {   0x5789, 0x0f},
    {   0x578a, 0xfd},
    {   0x578b, 0xf5},
    {   0x578c, 0xf5},
    {   0x578d, 0x03},
    {   0x578e, 0x08},
    {   0x578f, 0x0c},
    {   0x5790, 0x08},
    {   0x5791, 0x06},
    {   0x5792, 0x00},
    {   0x5793, 0x52},
    {   0x5794, 0xa3},
    {   0x5b00, 0x00},
    {   0x5b01, 0x1c},
    {   0x5b02, 0x00},
    {   0x5b03, 0x7f},
    {   0x5b05, 0x6c},
    {   0x5e10, 0xfc},
    {   0x4010, 0xf1},
    {   0x3503, 0x08},
    {   0x3505, 0x8c},
    {   0x3507, 0x03},
    {   0x3508, 0x00},
    {   0x3509, 0x80},
    {   REG_END, 0x00}
};

#endif

static struct ov_camera_module_config ov5695_configs[] =
{
    /*
     *{
     *        .name = "2560x1440_30fps",
     *        .frm_fmt = {
     *                .width = 2560,
     *                .height = 1440,
     *                .code = V4L2_MBUS_FMT_SBGGR10_1X10
     *        },
     *        .frm_intrvl = {
     *                .interval = {
     *                        .numerator = 1,
     *                        .denominator = 30
     *                }
     *        },
     *        .auto_exp_enabled = false,
     *        .auto_gain_enabled = false,
     *        .auto_wb_enabled = false,
     *        .reg_table = (void *)ov5695_init_tab_2560_1440_30fps,
     *        .reg_table_num_entries = ARRAY_SIZE(ov5695_init_tab_2560_1440_30fps),
     *        .v_blanking_time_us = 9600,
     *        .max_exp_gain_h = 16,
     *        .max_exp_gain_l = 0,
     *        PLTFRM_CAM_ITF_MIPI_CFG(0, 2, 840, OV5695_EXT_CLK)
     *},
     */
#ifdef USE_FULL_LINEAR_MODE
    {
        .name = "2592x1944_30fps",
        .frm_fmt = {
            .width = 2592,
            .height = 1944,
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
        .reg_table = (void *)ov5695_init_tab_2592_1944_30fps,
        .reg_table_num_entries = ARRAY_SIZE(ov5695_init_tab_2592_1944_30fps),
        .v_blanking_time_us = 1300,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        /* PLTFRM_CAM_ITF_MIPI_CFG(0, 2, 840, OV5695_EXT_CLK) */
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 2, 840, OV5695_EXT_CLK, 0, 0x30)
    }
#endif
#ifdef USE_BINNING_HDR_MODE
    {
        .name = "1296x972_60fps",
        .frm_fmt = {
            .width = 1296,
            .height = 972,
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
        .reg_table = (void *)ov5695_init_tab_1296_972_30fps,
        .reg_table_num_entries = ARRAY_SIZE(ov5695_init_tab_1296_972_30fps),
        .v_blanking_time_us = 1300,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        /* PLTFRM_CAM_ITF_MIPI_CFG(0, 2, 840, OV5695_EXT_CLK) */
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, 2, 840, OV5695_EXT_CLK, 0, 0x30)
    }
#endif
};

static ret_err_t ov5695_g_VTS(struct ov5695_dev *dev, u32 *vts);

u32 ov5695_gcd(u32 a, u32 b)
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

static ret_err_t ov5695_write_reg(rk_i2c_bus_device *bus,
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

        msgs.addr = OV5695_7BIT_ADDRESS_WRITE;
        msgs.flags = RT_I2C_WR;
        msgs.buf = send_buf;
        msgs.len = 3;
        ret = rt_i2c_transfer(bus, &msgs, 1);

        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
    }
    OV5695_DEBUG("(%s):s0.0 i2c_bus error\n");
    return RET_SYS_ERROR;

}

static ret_err_t ov5695_read_reg(rk_i2c_bus_device *bus,
                                 uint16_t reg, uint8_t *data)
{

    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = (reg  & 0xff);
    msg[0].addr = OV5695_7BIT_ADDRESS_WRITE;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = OV5695_7BIT_ADDRESS_WRITE;
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

static void ov5695_write_multiple_reg(struct ov5695_dev *dev,
                                      const struct ov5695_sensor_reg *reg)
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
            ov5695_write_reg(i2c_bus, reg[i].reg_addr, &reg[i].val);
        }
    }

}

static ret_err_t rt_ov5695_detect_sensor(struct ov5695_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    ret_err_t ret;
    uint8_t  ver;
    uint16_t chipid;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    uint32_t cnt = 0;

    OV5695_DEBUG("(%s) enter \n", __FUNCTION__);
    //rk_clk_set_rate(dev->mclk.clk_name, 24000000);
    //rk_clk_enable(dev->mclk.gate);

    /* powerdown: active high */
    if (cam_preisp_power_on_check())
    {
        HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_HIGH);
    }

    /* reset: active low */
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_LOW);
    HAL_DelayMs(2);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_HIGH);


//  rt_pin_write(GPIO0_B0, GPIO_LOW);
//  rt_pin_write(GPIO0_B2, GPIO_HIGH);
//
//    rt_pin_write(GPIO0_B2, GPIO_LOW);
//  HAL_DelayMs(2);
//    rt_pin_write(GPIO0_B2, GPIO_HIGH);


    uint8_t pid = 0;
RETRY:
    cnt++;
    ret = ov5695_read_reg(bus, OV5695_REG_ID_H_ADDRESS, &pid);
    if (ret == RET_SYS_EOK)
    {
        ret = ov5695_read_reg(bus, OV5695_REG_ID_L_ADDRESS, &ver);
        chipid = SENSOR_ID(pid, ver);
        OV5695_INFO(dev, "Info: OV5695 pid:0x%x,ver:0x%x!\n", pid, ver);
        if (chipid == OV5695_SENSOR_ID)
        {
            OV5695_INFO(dev, "Info: OV5695 detected successfully !!!\n");
            ret = RET_SYS_EOK;
        }
        else
        {
            OV5695_INFO(dev, "Info: OV5695 detected failed,not match:%d != %d !!!\n",
                        OV5695_SENSOR_ID, chipid);
            ret = -RT_EINVAL;
            if (cnt > CAMERA_BOOT_RETRY_NUM)
                goto EXIT;
            goto RETRY;
        }
    }
    else
    {
        OV5695_INFO(dev, "Warning: OV5695 i2c read failed !!!\n");
        ret = -RT_EINVAL;
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }
EXIT:
    OV5695_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static ret_err_t ov5695_stream_on(struct ov5695_dev *dev)
{
    u8 val;
    u8 data;
    ret_err_t ret;
    OV5695_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);

    data = 1;
    ret = ov5695_write_reg(dev->i2c_bus, 0x0100, &data);
//  mutex_unlock(&cam_mod->lock);
    if (ret == RET_SYS_ERROR)
        goto err;

//  msleep(25);
    rt_thread_mdelay(25);
    return ret;
err:
    OV5695_INFO(dev, "stream on failed with error (%d)\n", ret);
    return ret;
}

static ret_err_t ov5695_stream_off(struct ov5695_dev *dev)
{
    u8 val;
    rk_i2c_bus_device *i2c_bus;
    ret_err_t ret;

    OV5695_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);
    i2c_bus = dev->i2c_bus;
    val = 0x0;
    ret = ov5695_write_reg(i2c_bus, 0x0100, &val);//to stop  camera

    if (ret == RET_SYS_ERROR)
        goto err;

    OV5695_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
err:
    OV5695_INFO(dev, "stream off failed with error (%d)\n", ret);
    return ret;
}

ret_err_t rk_ov5695_init(struct rk_camera_device *dev)
{
    OV5695_DEBUG("(%s) enter \n", __FUNCTION__);
    ret_err_t ret = RET_SYS_EOK;
    struct ov5695_dev *ov5695;
    ov5695 = (struct ov5695_dev *)&dev;
#if RT_USING_OV5695_OPS
    if (ov5695->ops->init)
    {
        return (ov5695->ops->init(ov5695));
    }
#else

    if (ov5695 /* && camera*/)
    {
        ov5695->i2c_bus = (rk_i2c_bus_device *)rt_device_find(ov5695->i2c_name);

        if (!ov5695->i2c_bus)
        {
            OV5695_DEBUG("Warning:not find i2c source:%s !!!\n",
                         ov5695->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            OV5695_DEBUG("(%s):s0 find i2c_bus:%s\n", ov5695->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    OV5695_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

static ret_err_t rk_ov5695_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    OV5695_DEBUG("(%s) enter \n", __FUNCTION__);

    struct ov5695_dev *ov5695;
    struct rk_camera_device *camera;
    ret_err_t ret;
    u8 data;
    bool testpattern_en;

    MACRO_ASSERT(dev != RK_NULL);

    ov5695 = (struct ov5695_dev *)dev;
    camera = (struct rk_camera_device *)(&ov5695->parent);

    cam_power_on(0);
    ov5695_write_multiple_reg(ov5695, ov5695_init_tab_2592_1944_30fps);
    OV5695_INFO(ov5695, "active config = %s\n", ov5695->cam_mod.active_config->name);
    ret = ov5695_g_VTS(ov5695, &ov5695->cam_mod.vts_min);
    if (ret == RET_SYS_ERROR)
        goto open_err;
    apply_otp(ov5695, &g_otp_info);
    testpattern_en = ov_camera_module_get_testpattern_en(ov5695);

//  mutex_lock(&cam_mod->lock);
    if (testpattern_en)
    {
        data = 0x80;
        ov5695_write_reg(ov5695->i2c_bus, ov5695_TEST_PATTERN_REG, &data);
    }

    OV5695_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

open_err:
    OV5695_DEBUG(ov5695, "stream on failed with error (%d)\n", ret);
    return ret;


}
/*--------------------------------------------------------------------------*/

ret_err_t rk_ov5695_close(struct rk_camera_device *dev)
{
    struct ov5695_dev *ov5695 = (struct ov5695_dev *)dev;;
    struct rk_camera_device *camera;
    ret_err_t ret;

    OV5695_DEBUG(dev, "(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    cam_power_off(0);

    ov5695 = (struct ov5695_dev *)dev;
    return RET_SYS_ERROR;
}

/*--------------------------------------------------------------------------*/
static int read_otp(struct ov5695_dev *dev)
{
    struct otp_struct *otp_ptr = &g_otp_info ;
    int ret = 0;
    u32  addr;
    u8  otp_flag;
    int i;
    u8 temp_rg = 0;
    u8 temp_bg = 0;
    u8 data = 0;
    u8 temp ;

    rk_i2c_bus_device *bus = dev->i2c_bus;
    ret = ov5695_read_reg(bus, 0x700C, &otp_flag);

    OV5695_INFO(dev, "otp_flag 0x%x!\n", otp_flag);
    addr = 0;
    if ((otp_flag & 0xc0) == 0x40)
    {
        addr = 0x700D; // base address of info group 1
    }
    else if ((otp_flag & 0x30) ==  0x10)
    {
        addr = 0x7012; // base address of info group 2
    }
    else if ((otp_flag & 0x0C) == 0x04)
    {
        addr = 0x7017; // base address of info group 2
    }
    if (addr != 0)
    {
        (*otp_ptr).flag = 0x80; // valid info and AWB in OTP
        ret |= ov5695_read_reg(bus, addr, &temp);
        (*otp_ptr).module_integrator_id = (u32)temp;
        ret |= ov5695_read_reg(bus, addr + 1, &temp);
        (*otp_ptr).lens_id = (u32)temp;
        ret |= ov5695_read_reg(bus, addr + 4, &temp);
        ret |= ov5695_read_reg(bus, addr + 2, &temp_rg);
        ret |= ov5695_read_reg(bus, addr + 3, &temp_bg);
        (*otp_ptr).rg_ratio = (temp_rg << 2) + ((temp >> 6) & 0x03);
        (*otp_ptr).bg_ratio = (temp_bg << 2) + ((temp >> 4) & 0x03);
        OV5695_INFO(dev,
                    "awb otp info in module_integrator_id(0x%x) lens_id(0x%x) awb info(0x%x,0x%x)\n",
                    (*otp_ptr).module_integrator_id, (*otp_ptr).lens_id,
                    (*otp_ptr).rg_ratio, (*otp_ptr).bg_ratio);
    }
    else
    {
        (*otp_ptr).flag = 0x00; // not info and AWB in OTP
        (*otp_ptr).module_integrator_id = 0;
        (*otp_ptr).lens_id = 0;
        (*otp_ptr).rg_ratio = 0;
        (*otp_ptr).bg_ratio = 0;
        OV5695_INFO(dev, "no awb info in otp!\n");
    }

    data = 0;
    for (i = 0x700C; i <= 0x701B; i++)
    {
        ret |= ov5695_write_reg(bus, i, &data);
        // clear OTP buffer, recommended use continuous write to accelarate
    }
    //set 0x5002[3] to "1"
    ret |= ov5695_read_reg(bus, 0x5001, &temp);
    data = (0x08 & 0x08) | (temp & (~0x08));
    ret |= ov5695_write_reg(bus, 0x5001, &data);

    //stream off
    data = 0x00;
    ret |= ov5695_write_reg(bus, 0x0100, &data);

    if ((*otp_ptr).flag != 0 && ret == 0)
    {
        OV5695_INFO(dev, "otp read finished!\n");
        return 0;
    }
    else
    {
        OV5695_INFO(dev, "otp read err or no exist otp data!\n");
        return -1;
    }
}

/*--------------------------------------------------------------------------*/
static int check_read_otp(struct ov5695_dev *dev)
{
    u8 temp;
    int ret = 0;
    int i = 0;
    u8 data = 0;
    rk_i2c_bus_device *bus = dev->i2c_bus;

    //stream on
    data = 0x01;
    ret = ov5695_write_reg(bus, 0x0100, &data);
    ret |= ov5695_read_reg(bus,  0x5001, &temp);
    data = temp & (~0x08);
    ret |= ov5695_write_reg(bus, 0x5001, &data);
    // read OTP into buffer
    data = 0xC0;
    ret |= ov5695_write_reg(bus, 0x3d84, &data);
    data = 0x70;
    ret |= ov5695_write_reg(bus, 0x3d88, &data);
    data = 0x0C;
    ret |= ov5695_write_reg(bus, 0x3d89, &data);
    data = 0x70;
    ret |= ov5695_write_reg(bus, 0x3d8A, &data);
    data = 0x1B;
    ret |= ov5695_write_reg(bus, 0x3d8B, &data);
    for (i = 0x700C; i <= 0x701B; i++)
    {
        data = 0;
        ret |= ov5695_write_reg(bus, i, &data);
        // clear OTP buffer, recommended use continuous write to accelarate
    }
    data = 0x01;
    ret |= ov5695_write_reg(bus, 0x3d81, &data);
    /**
     * usleep_range - Drop in replacement for udelay where wakeup is flexible
     * @min: Minimum time in usecs to sleep
     * @max: Maximum time in usecs to sleep
     * \sl_sim_isp\kernel\kernel\timer.c
     */
//  usleep_range(10000, 11000);
    return  read_otp(dev);
}

static int apply_otp(struct ov5695_dev *dev, struct otp_struct *otp_ptr)
{
    u32 rg, bg, R_gain, G_gain, B_gain, Base_gain;
    u32 golden_bg_ratio = 0;
    u32 golden_rg_ratio = 0;
    u32 golden_g_value = 0;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    struct ov_camera_module *cam_mod = &dev->cam_mod;

    OV5695_INFO(dev,
                "awb otp golden_gb(0x%x) golden_gr(0x%x) golden_b(0x%x) golden_r(0x%x)!\n",
                cam_mod->awb_cfg.golden_gb_value, cam_mod->awb_cfg.golden_gr_value,
                cam_mod->awb_cfg.golden_b_value, cam_mod->awb_cfg.golden_r_value);
    if (cam_mod->awb_cfg.enable)
    {
        golden_g_value = (cam_mod->awb_cfg.golden_gb_value +
                          cam_mod->awb_cfg.golden_gr_value) / 2;
        golden_bg_ratio = cam_mod->awb_cfg.golden_b_value * 0x100 / golden_g_value;
        golden_rg_ratio = cam_mod->awb_cfg.golden_r_value * 0x100 / golden_g_value;
    }

    // apply OTP WB Calibration
    if (((*otp_ptr).flag & 0x80) && cam_mod->awb_cfg.enable)
    {
        rg = (*otp_ptr).rg_ratio;
        bg = (*otp_ptr).bg_ratio;
        //calculate G gain
        R_gain = (golden_rg_ratio * 1000) / rg;
        B_gain = (golden_bg_ratio * 1000) / bg;
        G_gain = 1000;
        if (R_gain < 1000 || B_gain < 1000)
        {
            if (R_gain < B_gain)
                Base_gain = R_gain;
            else
                Base_gain = B_gain;
        }
        else
        {
            Base_gain = G_gain;
        }
        R_gain = 0x400 * R_gain / (Base_gain);
        B_gain = 0x400 * B_gain / (Base_gain);
        G_gain = 0x400 * G_gain / (Base_gain);
        // update sensor WB gain
        u8 data = 0;
        if (R_gain > 0x400)
        {
            data = (R_gain >> 8) & 0xff;
            ov5695_write_reg(bus, 0x5019, &data);  //change to (R_gain>>8) & 0xff from (R_gain>>8)
            data = R_gain & 0xff;
            ov5695_write_reg(bus, 0x501A, &data);
        }
        if (G_gain > 0x400)
        {
            data = (G_gain >> 8) & 0xff;
            ov5695_write_reg(bus, 0x501B, &data);  //change to (G_gain>>8) & 0xff from (G_gain>>8)
            data = G_gain & 0xff;
            ov5695_write_reg(bus, 0x501C, &data);
        }
        if (B_gain > 0x400)
        {
            data = (B_gain >> 8) & 0xff;
            ov5695_write_reg(bus, 0x501D, &data);  //change to (B_gain>>8) & 0xff from (B_gain>>8)
            data = B_gain & 0xff;
            ov5695_write_reg(bus, 0x501E, &data);
        }
        OV5695_INFO(dev, "awb otp cur_rg(0x%x) cur_bg(0x%x) gol_rg(0x%x) gol_bg(0x%x)!\n",
                    rg, bg, golden_rg_ratio, golden_bg_ratio);
        OV5695_INFO(dev, "apply awb otp r_gain(0x%x) b_gain(0x%x) g_gain(0x%x)!\n",
                    R_gain, B_gain, G_gain);

    }
    else
    {
        OV5695_INFO(dev, "no apply otp!\n");
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

static ret_err_t ov5695_g_VTS(struct ov5695_dev *dev, u32 *vts)
{
    u8 msb = 0;
    u8 lsb = 0;

    ret_err_t ret;

    rk_i2c_bus_device *bus = dev->i2c_bus;
    ret = ov5695_read_reg(bus, OV5695_TIMING_VTS_HIGH_REG, &msb);
    if (ret == RET_SYS_EOK)
    {
        ret = ov5695_read_reg(bus, OV5695_TIMING_VTS_LOW_REG, &lsb);
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
    OV5695_INFO(dev, "Warning: OV5695 get VTS failed !!!\n");
    return ret;
}

/*--------------------------------------------------------------------------*/
static ret_err_t ov5695_s_VTS(struct ov5695_dev *dev,
                              u32 vts)
{
    ret_err_t ret = 0;
    if (vts < dev->cam_mod.vts_min)
        return RET_SYS_ERROR;
    u8 temp;
    temp = vts & 0xff;
    ret = ov5695_write_reg(dev->i2c_bus, OV5695_TIMING_VTS_LOW_REG, &temp);
    temp = (vts >> 8) & 0xff;
    ret |= ov5695_write_reg(dev->i2c_bus, OV5695_TIMING_VTS_HIGH_REG, &temp);
    if (ret == RET_SYS_EOK)
    {
        OV5695_INFO(dev, "updated vts = %d,vts_min=%d\n", vts, dev->cam_mod.vts_min);
        dev->cam_mod.vts_cur = vts;
    }
    else
    {
        OV5695_INFO(dev, "Warning: set VTS failed with error (%d)!!!\n", ret);
    }

    return ret;
}

/*--------------------------------------------------------------------------*/

ret_err_t ov5695_auto_adjust_fps_state(struct ov5695_dev *dev, void *args)
{

    ret_err_t ret;
    bool *auto_adjust_fps = (bool *)args;
    dev->cam_mod.auto_adjust_fps = *auto_adjust_fps;
    OV5695_INFO(dev, "state of ov5695_auto_adjust_fps is : %d.", dev->cam_mod.auto_adjust_fps);
    return RET_SYS_EOK;
}

/*--------------------------------------------------------------------------*/

static ret_err_t    ov5695_auto_adjust_fps(struct ov5695_dev *dev,
        u32 exp_time)
{
    ret_err_t ret;
    u32 vts;

    if ((exp_time + OV5695_COARSE_INTG_TIME_MAX_MARGIN)
            > dev->cam_mod.vts_min)
        vts = exp_time + OV5695_COARSE_INTG_TIME_MAX_MARGIN;
    else
        vts = dev->cam_mod.vts_min;

    ret = ov5695_s_VTS(dev, vts);

    if (ret == RET_SYS_EOK)
    {
        OV5695_INFO(dev, "updated vts = %d,vts_min=%d\n", vts, dev->cam_mod.vts_min);
        dev->cam_mod.vts_cur = vts;
    }
    else
    {
        OV5695_INFO(dev, "Warning: auto_adjust VTS failed with error (%d)!!!\n", ret);
    }

    return ret;
}

/*--------------------------------------------------------------------------*/
static ret_err_t ov5695_g_shutter(struct ov5695_dev *dev,
                                  u32 *shutter)
{
// read shutter, in number of line period
    u8 shutterH, shutterM, shutterL;

    ret_err_t ret;
    ret = ov5695_read_reg(dev->i2c_bus, OV5695_AEC_PK_LONG_EXPO_3RD_REG, &shutterH);
    ret |= ov5695_read_reg(dev->i2c_bus, OV5695_AEC_PK_LONG_EXPO_2ND_REG, &shutterM);
    ret |= ov5695_read_reg(dev->i2c_bus, OV5695_AEC_PK_LONG_EXPO_1ST_REG, &shutterL);
    if (ret == RET_SYS_EOK)
    {

        shutter[0] = shutterH & 0x0f;
        shutter[0] = (shutter[0] << 8) + shutterM;
        shutter[0] = (shutter[0] << 4) + ((shutterL >> 4) & 0x0f);
        return RET_SYS_EOK;
    }

    else
    {
        OV5695_INFO(dev, "Warning: get shtter failed with error (%d)!!!\n", ret);
        return ret;
    }

}

/*--------------------------------------------------------------------------*/
static ret_err_t ov5695_s_shutter(struct ov5695_dev *dev, u32 *shutter)
{
// write shutter, in number of line period
    u8 temp;
    ret_err_t ret;

    shutter[0] = shutter[0] & 0xffff;
    if (*shutter < 4U)
    {
        *shutter = 4U;
    }

    temp = (shutter[0] << 4) & 0xf0;
    ret = ov5695_write_reg(dev->i2c_bus, OV5695_AEC_PK_LONG_EXPO_1ST_REG, &temp);
    temp = (shutter[0] >> 4) & 0xff;
    ret |= ov5695_write_reg(dev->i2c_bus, OV5695_AEC_PK_LONG_EXPO_2ND_REG, &temp);
    temp = (shutter[0] >> 12) & 0x0f;
    ret |= ov5695_write_reg(dev->i2c_bus, OV5695_AEC_PK_LONG_EXPO_3RD_REG, &temp);

    if (ret == RET_SYS_EOK)
    {
        return RET_SYS_EOK;
    }
    else
    {
        OV5695_INFO(dev, "Warning: set shutter failed with error (%d)!!!\n", ret);
        return ret;
    }

}

/*--------------------------------------------------------------------------*/
static ret_err_t ov5695_g_gain16(struct ov5695_dev *dev,
                                 u32 *gain)
{
// read real gain, 16 = 1x
    ret_err_t ret;
    u8 temp;
    ret = ov5695_read_reg(dev->i2c_bus, OV5695_GAIN16_REG, &temp);
    *gain = (u32)temp;
    if (ret == RET_SYS_EOK)
    {
        return RET_SYS_EOK;
    }
    else
    {
        OV5695_INFO(dev, "Warning: get gain16 failed with error (%d)!!!\n", ret);
        return ret;
    }

}

/*--------------------------------------------------------------------------*/

static ret_err_t ov5695_s_gain16(struct ov5695_dev *dev, u32 gain16)
{
// write real gain, 16 = 1x
    u8 temp;
    ret_err_t ret = 0;
    if (gain16 > OV5695_again_real_max)
        gain16 = OV5695_again_real_max;
    else if (gain16 < OV5695_again_real_min)
        gain16 = OV5695_again_real_min;

    temp = gain16 & 0xff;
    ret = ov5695_write_reg(dev->i2c_bus, OV5695_GAIN16_REG, &temp);

    if (ret == RET_SYS_EOK)
    {
        return RET_SYS_EOK;
    }
    else
    {
        OV5695_INFO(dev, "Warning: set gain16 failed with error (%d)!!!\n", ret);
        return ret;
    }

}

static ret_err_t ov5695_s_aec(struct ov5695_dev *dev, void *args)
{
    ret_err_t ret;
    u32 vts;
    struct HAL_ISP_Set_Exp_s *aec = (struct HAL_ISP_Set_Exp_s *)args;
    struct ov_camera_module *cam_mod = &dev->cam_mod;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    u8 reg_data;

    if (aec->gain_percent > 0 && aec->gain_percent <= 100)
    {
        cam_mod->exp_config.gain_percent = aec->gain_percent;
    }
    else
    {
        cam_mod->exp_config.gain_percent = 100;
    }

    if (aec->vts > cam_mod->vts_min)
    {
        cam_mod->exp_config.vts_value = aec->vts;
    }

    if (aec->gain < OV5695_again_real_min)
    {
        aec->gain = OV5695_again_real_min;
    }
    else if (aec->gain > OV5695_again_real_max)
    {
        aec->gain = OV5695_again_real_max;
    }

//  if (rt_mutex_init(&spi_spi2apb_lock, "spi2apbLock", RT_IPC_FLAG_FIFO)
//            != RT_EOK)
//        RT_ASSERT(0);
//  ov5695_LOCK();
//TODO mutex_lock(&cam_mod.lock);
    vts = cam_mod->vts_cur == 0 ? cam_mod->vts_min : cam_mod->vts_cur;

    if (!cam_mod->auto_adjust_fps &&
            aec->exposure > vts - OV5695_COARSE_INTG_TIME_MAX_MARGIN)
    {
        aec->exposure = vts - OV5695_COARSE_INTG_TIME_MAX_MARGIN;
    }

    /* hold reg en */
    reg_data = OV5695_AEC_GROUP_UPDATE_START_DATA;
    ret = ov5695_write_reg(bus, OV5695_AEC_GROUP_UPDATE_ADDRESS, &reg_data);

    if (ret == RET_SYS_EOK && cam_mod->auto_adjust_fps)
        ret |= ov5695_auto_adjust_fps(dev, cam_mod->exp_config.exp_time);

    aec->gain = aec->gain & 0xff;
    ret |= ov5695_s_gain16(dev, aec->gain);

    if (ret == RET_SYS_EOK)
    {
        cam_mod->exp_config.gain = aec->gain;
    }

    ret |= ov5695_s_shutter(dev, &(aec->exposure));

    if (ret == RET_SYS_EOK)
    {
        cam_mod->exp_config.exp_time = aec->exposure;
    }

    if (!cam_mod->auto_adjust_fps && cam_mod->vts_cur != cam_mod->exp_config.vts_value)
        ret |= ov5695_s_VTS(dev, cam_mod->exp_config.vts_value);

    /* hold reg end */
    reg_data = OV5695_AEC_GROUP_UPDATE_END_DATA;
    ret |= ov5695_write_reg(bus, OV5695_AEC_GROUP_UPDATE_ADDRESS, &reg_data);

    reg_data = OV5695_AEC_GROUP_UPDATE_END_LAUNCH;
    ret |= ov5695_write_reg(bus, OV5695_AEC_GROUP_UPDATE_ADDRESS, &reg_data);

//  ov5695_UNLOCK();

    return ret;
}

static ret_err_t ov5695_g_aec(struct ov5695_dev *dev, void *args)
{
    ret_err_t ret;
    struct HAL_ISP_Set_Exp_s *aec_value = (struct HAL_ISP_Set_Exp_s *)args;
    struct ov_camera_module *cam_mod = &dev->cam_mod;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    u32 gain;
    u32 shutter;
    u8 reg_data;

    ret |= ov5695_g_gain16(dev, &gain);
    if (ret == RET_SYS_EOK)
    {
        aec_value->gain = gain;
        cam_mod->exp_config.gain = aec_value->gain;
    }

    ret |= ov5695_g_shutter(dev, &shutter);
    if (ret == RET_SYS_EOK)
    {
        aec_value->exposure = shutter;
        cam_mod->exp_config.exp_time = aec_value->exposure;
    }

    OV5695_INFO(dev, "g_aec ret = %d\n", ret);
    OV5695_INFO(dev, "g_aec again = 0x%x\n", aec_value->gain);

    return ret;
}

ret_err_t ov5695_g_fmt(struct ov5695_dev *dev, void *args)
{
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    if (dev->cam_mod.active_config)
    {
        fmt->code = dev->cam_mod.active_config->frm_fmt.code;
        fmt->width = dev->cam_mod.active_config->frm_fmt.width;
        fmt->height = dev->cam_mod.active_config->frm_fmt.height;
        return RET_SYS_EOK;
    }

    OV5695_INFO(dev, "no active config\n");
    return RET_SYS_ERROR;
}

ret_err_t ov5695_g_sensor_frm_interval(struct ov5695_dev *dev, void *args)
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

    OV5695_INFO(dev, "no active config\n");
    return RET_SYS_ERROR;
}

/* ======================================================================== */
static void ov_camera_module_set_active_config(
    struct ov5695_dev *dev,
    struct ov_camera_module_config *new_config)
{

    if (RK_NULL == new_config)
    {
        dev->cam_mod.active_config = new_config;
        OV5695_INFO(dev,
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
            OV5695_INFO(dev,
                        "activating config '%s'\n",
                        dev->cam_mod.active_config->name);
        }
    }
}

/* ======================================================================== */
static struct ov_camera_module_config *ov_camera_module_find_config(
    struct ov5695_dev *dev,
    struct v4l2_mbus_framefmt *fmt,
    struct v4l2_subdev_frame_interval *frm_intrvl)
{
    u32 i;
    u32 gcdiv;
    struct v4l2_subdev_frame_interval norm_interval;

    OV5695_INFO(dev, "into ov_camera_module_find_config.\n");
    OV5695_INFO(dev, "fmt = 0x%x,frm_intrvl = 0x%x\n", fmt, frm_intrvl);

    if (NULL != fmt)
        OV5695_INFO(dev,
                    "ov_camera_module_find_config:%dx%d, fmt code 0x%04x\n",
                    fmt->width, fmt->height, fmt->code);

    if (NULL != frm_intrvl)
    {
        OV5695_INFO(dev,
                    "ov_camera_module_find_config:frame interval %d/%d....\n",
                    frm_intrvl->interval.numerator,
                    frm_intrvl->interval.denominator);
    }


    for (i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (NULL != frm_intrvl)
        {

            gcdiv = ov5695_gcd(dev->cam_mod.custom.configs[i].
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
        OV5695_INFO(dev,
                    "found matching config %s\n",
                    dev->cam_mod.custom.configs[i].name);
        return &dev->cam_mod.custom.configs[i];
    }
    OV5695_INFO(dev, "no matching config found\n");

    return RK_NULL;
}
/* ======================================================================== */
ret_err_t ov5695_s_fmt(struct ov5695_dev *dev, void *args)
{
    struct ov_camera_module_config *config;
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    ret_err_t ret = RET_SYS_EOK;

    OV5695_INFO(dev, "OV5695_s_fmt:%dx%d, fmt code 0x%04x\n",
                fmt->width, fmt->height, fmt->code);

    config = ov_camera_module_find_config(dev, fmt, NULL);

    if (config == NULL)
    {
        OV5695_INFO(dev,
                    "format %dx%d, code 0x%04x, not supported\n",
                    fmt->width, fmt->height, fmt->code);
        ret = RET_SYS_ERROR;
        goto err;
    }
    dev->cam_mod.frm_fmt_valid = true;
    dev->cam_mod.frm_fmt = fmt[0];
    OV5695_INFO(dev, "fmt is found,%dx%d, fmt code 0x%04x\n", fmt->width, fmt->height, fmt->code);

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
    OV5695_INFO(dev,
                "failed with error %d\n", ret);
    return ret;
}

/* ======================================================================== */

ret_err_t ov5695_g_v_blank_us(struct ov5695_dev *dev, void *args)
{
    u32 *v_blanking_time_us = (u32 *)args;
    v_blanking_time_us[0] = dev->cam_mod.active_config->v_blanking_time_us;
    return RET_SYS_EOK;
}

/* ======================================================================== */

ret_err_t ov5695_g_itf_cfg(struct ov5695_dev *dev, void *args)
{

    struct pltfrm_cam_itf *itf_cfg = (struct pltfrm_cam_itf *)args;
    struct ov_camera_module_config *config;

    if (dev->cam_mod.custom.num_configs <= 0)
    {
        OV5695_INFO(dev,
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

static ret_err_t ov5695_g_timings(struct ov5695_dev *dev,                    //copy from ov5695_g_timings
                                  struct ov_camera_module_timings *timings)
{
    unsigned int vts;
    ret_err_t ret = RET_SYS_EOK;

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
    OV5695_INFO(dev, "cam_mod:failed with error (%d)\n", RET_SYS_EEMPTY);
    return RET_SYS_EEMPTY;
}

/* ======================================================================== */

ret_err_t ov5695_g_sensor_mode(struct ov5695_dev *dev, void *args)
{
    ret_err_t ret;
    static int frm_idx = 0; // flag of 1-st frame.

    struct ov_camera_module_timings ov_timings;

    int *args_data = (int *)args;

    ret = ov5695_g_timings(dev, &ov_timings);

    if (RET_SYS_EOK != ret)
    {
        OV5695_INFO(dev,
                    "failed with error %d\n", ret);
        return ret;
    }

    ov5695_sensorModeData->isp_input_horizontal_start = 0;
    ov5695_sensorModeData->isp_input_vertical_start   = 0;
    ov5695_sensorModeData->isp_input_width   = 2592;
    ov5695_sensorModeData->isp_input_height  = 1944;
    ov5695_sensorModeData->isp_output_width  = 2592;
    ov5695_sensorModeData->isp_output_height = 1944;

    ov5695_sensorModeData->sensor_output_width   = ov_timings.sensor_output_width;
    ov5695_sensorModeData->sensor_output_height  = ov_timings.sensor_output_height;
    ov5695_sensorModeData->crop_horizontal_start = ov_timings.crop_horizontal_start;
    ov5695_sensorModeData->crop_vertical_start = ov_timings.crop_vertical_start;
    ov5695_sensorModeData->crop_horizontal_end = ov_timings.crop_horizontal_end;
    ov5695_sensorModeData->crop_vertical_end   = ov_timings.crop_vertical_end;
    ov5695_sensorModeData->line_length_pck     = ov_timings.line_length_pck;
    ov5695_sensorModeData->frame_length_lines  = ov_timings.frame_length_lines;
    ov5695_sensorModeData->vt_pix_clk_freq_hz  = ov_timings.vt_pix_clk_freq_hz;
    ov5695_sensorModeData->binning_factor_x    = ov_timings.binning_factor_x;
    ov5695_sensorModeData->binning_factor_y    = ov_timings.binning_factor_y;

    ov5695_sensorModeData->coarse_integration_time_max_margin = ov_timings.coarse_integration_time_max_margin;
    ov5695_sensorModeData->coarse_integration_time_min        = ov_timings.coarse_integration_time_min;
    ov5695_sensorModeData->fine_integration_time_max_margin   = ov_timings.fine_integration_time_max_margin;
    ov5695_sensorModeData->fine_integration_time_min          = ov_timings.fine_integration_time_min;

    ov5695_sensorModeData->exposure_valid_frame[0] = dev->cam_mod.custom.exposure_valid_frame[0];
    ov5695_sensorModeData->exposure_valid_frame[1] = dev->cam_mod.custom.exposure_valid_frame[1];
    ov5695_sensorModeData->lane_num = 2;

    if (frm_idx == 0)
    {
        ov5695_sensorModeData->exp_time = 182;
        ov5695_sensorModeData->gain = 16;

        dev->cam_mod.exp_config.exp_time = 182;
        dev->cam_mod.exp_config.gain = 16;
    }
    else
    {
        if (dev->cam_mod.exp_config.exp_time)
        {
            ov5695_sensorModeData->exp_time = dev->cam_mod.exp_config.exp_time;
            ov5695_sensorModeData->exp_time_l = dev->cam_mod.exp_config.exp_time_l;
            ov5695_sensorModeData->exp_time_s = dev->cam_mod.exp_config.exp_time_s;
        }
        else
        {
            ov5695_sensorModeData->exp_time = ov_timings.exp_time;
            ov5695_sensorModeData->exp_time_l = ov_timings.exp_time;
            ov5695_sensorModeData->exp_time_s = ov_timings.exp_time;
        }
        if (dev->cam_mod.exp_config.gain)
        {
            ov5695_sensorModeData->gain = dev->cam_mod.exp_config.gain;
            ov5695_sensorModeData->gain_l = dev->cam_mod.exp_config.gain_l;
            ov5695_sensorModeData->gain_s = dev->cam_mod.exp_config.gain_s;
        }
        else
        {
            ov5695_sensorModeData->gain = ov_timings.gain;
            ov5695_sensorModeData->gain_l = ov_timings.gain;
            ov5695_sensorModeData->gain_s = ov_timings.gain;
        }
    }

    frm_idx++;

    if (frm_idx > 1000)
        frm_idx = 1;

    if (dev->cam_mod.active_config)
    {
        ov5695_sensorModeData->max_exp_gain_h = dev->cam_mod.active_config->max_exp_gain_h;
        ov5695_sensorModeData->max_exp_gain_l = dev->cam_mod.active_config->max_exp_gain_l;
    }
    else
    {
        ov5695_sensorModeData->max_exp_gain_h = dev->cam_mod.custom.configs[0].max_exp_gain_h;
        ov5695_sensorModeData->max_exp_gain_l = dev->cam_mod.custom.configs[0].max_exp_gain_l;
    }

    *args_data = (int)ov5695_sensorModeData;

    //struct isp_supplemental_sensor_mode_data *test_p = (struct isp_supplemental_sensor_mode_data *)(*args_data);

    //rt_kprintf("<<<<>>>>> get sensor mode: re[%d], [%d, %d, %d, %d, %d, %d].\n", ret,
    //    test_p->binning_factor_x,
    //    test_p->binning_factor_y,
    //    test_p->exposure_valid_frame[0],
    //    test_p->exposure_valid_frame[1],
    //    test_p->exp_time,
    //    test_p->gain
    //    );
    //rt_kprintf(">>>>>>>>>> *args_data var: addr: [0x%x].\n",  *args_data);

    return ret;
}

/*--------------------------------------------------------------------------*/

static int ov5695_filltimings(struct ov_camera_module_custom_config *custom)
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
            case OV5695_TIMING_VTS_HIGH_REG:
                timings->frame_length_lines = reg_table[j].val << 8;
                break;
            case OV5695_TIMING_VTS_LOW_REG:
                timings->frame_length_lines |= reg_table[j].val;
                break;
            case OV5695_TIMING_HTS_HIGH_REG:
                timings->line_length_pck =
                    (reg_table[j].val << 8);
                break;
            case OV5695_TIMING_HTS_LOW_REG:
                timings->line_length_pck |= reg_table[j].val;
                break;
            case OV5695_TIMING_X_INC:
                timings->binning_factor_x = ((reg_table[j].val >> 4) + 1) / 2;
                if (timings->binning_factor_x == 0)
                    timings->binning_factor_x = 1;
                break;
            case OV5695_TIMING_Y_INC:
                timings->binning_factor_y = ((reg_table[j].val >> 4) + 1) / 2;
                if (timings->binning_factor_y == 0)
                    timings->binning_factor_y = 1;
                break;
            case OV5695_HORIZONTAL_START_HIGH_REG:
                timings->crop_horizontal_start = reg_table[j].val << 8;
                break;
            case OV5695_HORIZONTAL_START_LOW_REG:
                timings->crop_horizontal_start |= reg_table[j].val;
                break;
            case OV5695_VERTICAL_START_HIGH_REG:
                timings->crop_vertical_start = reg_table[j].val << 8;
                break;
            case OV5695_VERTICAL_START_LOW_REG:
                timings->crop_vertical_start |= reg_table[j].val;
                break;
            case OV5695_HORIZONTAL_END_HIGH_REG:
                timings->crop_horizontal_end = reg_table[j].val << 8;
                break;
            case OV5695_HORIZONTAL_END_LOW_REG:
                timings->crop_horizontal_end |= reg_table[j].val;
                break;
            case OV5695_VERTICAL_END_HIGH_REG:
                timings->crop_vertical_end = reg_table[j].val << 8;
                break;
            case OV5695_VERTICAL_END_LOW_REG:
                timings->crop_vertical_end |= reg_table[j].val;
                break;
            case OV5695_HORIZONTAL_OUTPUT_SIZE_HIGH_REG:
                timings->sensor_output_width = reg_table[j].val << 8;
                break;
            case OV5695_HORIZONTAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_width |= reg_table[j].val;
                break;
            case OV5695_VERTICAL_OUTPUT_SIZE_HIGH_REG:
                timings->sensor_output_height = reg_table[j].val << 8;
                break;
            case OV5695_VERTICAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_height |= reg_table[j].val;
                break;
            }
        }

        timings->vt_pix_clk_freq_hz = config->frm_intrvl.interval.denominator
                                      * timings->frame_length_lines
                                      * timings->line_length_pck;

        timings->coarse_integration_time_min = OV5695_COARSE_INTG_TIME_MIN;
        timings->coarse_integration_time_max_margin =
            OV5695_COARSE_INTG_TIME_MAX_MARGIN;

        /* OV Sensor do not use fine integration time. */
        timings->fine_integration_time_min = OV5695_FINE_INTG_TIME_MIN;
        timings->fine_integration_time_max_margin =
            OV5695_FINE_INTG_TIME_MAX_MARGIN;
    }

    return 0;
}

ret_err_t ov5695_g_sensor_configInfo(struct ov5695_dev *dev, void *args)
{

    ret_err_t ret;
    int *args_data = (int *)args;

    ov5695_sensorInfo->config_num = dev->cam_mod.custom.num_configs;
    //rt_kprintf(">>>>>>>>>> config_num:   [%d], addr: [0x%x].\n", ov5695_sensorInfo->config_num, ov5695_sensorInfo);
    for (u8 i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (i >= SENSOR_CONFIG_NUM)
            break;
        ov5695_sensorInfo->sensor_fmt[i] =
            pltfrm_camera_module_pix_fmt2csi2_dt(dev->cam_mod.custom.configs[i].frm_fmt.code);
        ov5695_sensorInfo->reso[i].width  = dev->cam_mod.custom.configs[i].frm_fmt.width;
        ov5695_sensorInfo->reso[i].height = dev->cam_mod.custom.configs[i].frm_fmt.height;
    }

    *args_data = (int)ov5695_sensorInfo;
    //rt_kprintf(">>>>>>>>>> sensor info *args_data var: addr: [0x%x].\n",  *args_data);

    return 0;
}

ret_err_t ov5695_g_sensor_iq_info(struct ov5695_dev *dev, void *args)
{
    ret_err_t ret = 0;
    struct rk_camera_iq_info *info = (struct rk_camera_iq_info *)args;
    info->addr = (void *)cam_iq_data;
    return ret;
}

/* ======================================================================== */
//ret_err_t ov5695_g_light_vol(struct ov5695_dev *dev,void *args){
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


static ret_err_t ov5695_s_flipMirror(struct ov5695_dev *dev, void *args)
{
    ret_err_t ret = 0;
    u8 data;
    u8 *state = (u8 *)args;

    switch (*state)
    {
    case OV5695_FLIPMIRROR_STATE_NORMAL:
    {
        data &= ~OV5695_FLIPMIRROR_REGBIT_FLIP;
        data &= ~OV5695_FLIPMIRROR_REGBIT_MIRROR;
        ret |= ov5695_write_reg(dev->i2c_bus, OV5695_FLIPMIRROR_REGADDR, &data);
    }
    break;
    case OV5695_FLIPMIRROR_STATE_FLIPMIRROR:
    {
        data |= OV5695_FLIPMIRROR_REGBIT_MIRROR;
        data |= OV5695_FLIPMIRROR_REGBIT_FLIP;
        ret |= ov5695_write_reg(dev->i2c_bus, OV5695_FLIPMIRROR_REGADDR, &data);
    }
    break;
    case OV5695_FLIPMIRROR_STATE_MIRROR:
    {
        data |= OV5695_FLIPMIRROR_REGBIT_MIRROR;
        data &= ~OV5695_FLIPMIRROR_REGBIT_FLIP;
        ret |= ov5695_write_reg(dev->i2c_bus, OV5695_FLIPMIRROR_REGADDR, &data);
    }
    break;
    case OV5695_FLIPMIRROR_STATE_FLIP:
    {
        data |= OV5695_FLIPMIRROR_REGBIT_FLIP;
        data &= ~OV5695_FLIPMIRROR_REGBIT_MIRROR;
        ret |= ov5695_write_reg(dev->i2c_bus, OV5695_FLIPMIRROR_REGADDR, &data);
    }
    break;
    default:
    {
        OV5695_INFO(dev, "%d is wrong flip_mirror state!!!\n", *state);
        ret = RET_SYS_ERROR;
    }
    break;
    }

    if (IS_ERR_VALUE(ret))
    {
        OV5695_INFO(dev, "s_flipMirror:failed with error (%d)\n", ret);
    }

    return ret;
}


/* ======================================================================== */
ret_err_t rk_ov5695_control(struct rk_camera_device *dev,
                            int cmd,
                            void *args)
{
    OV5695_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct ov5695_dev *ov5695;

    MACRO_ASSERT(dev != RK_NULL);
    ov5695 = (struct ov5695_dev *)dev;
#if RT_USING_OV5695_OPS
    return (ov5695->ops->control(ov5695, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_ov5695_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        ret = ov5695_stream_on(ov5695);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        ret = ov5695_stream_off(ov5695);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_AEC:
    {
        ret = ov5695_g_aec(ov5695, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_AEC:
    {
        ret = ov5695_s_aec(ov5695, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = ov5695_g_fmt(ov5695, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = ov5695_s_fmt(ov5695, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_BLANK:
    {
        ret = ov5695_g_v_blank_us(ov5695, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_ITF_CFG:
    {
        ret = ov5695_g_itf_cfg(ov5695, args);
    }
    break;

    case RK_VIDIOC_SENSOR_MODE_DATA:
    {
        ret = ov5695_g_sensor_mode(ov5695, args);
    }
    break;
    case RK_VIDIOC_SENSOR_CONFIGINFO:
    {
        ret = ov5695_g_sensor_configInfo(ov5695, args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_IQ_INFO:
    {
        ret = ov5695_g_sensor_iq_info(ov5695, args);
    }
    break;

    case RK_V4L2_CID_AUTO_FPS:
    {
        ret = ov5695_auto_adjust_fps_state(ov5695, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_FRMINTVAL:
    {
        ret = ov5695_g_sensor_frm_interval(ov5695, args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = ov5695_s_flipMirror(ov5695, args);
    }
    break;
    default:
        break;
    }
#endif
    OV5695_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

struct rk_camera_ops rk_ov5695_ops =
{
    rk_ov5695_init,
    rk_ov5695_open,
    rk_ov5695_close,
    rk_ov5695_control
};
/*--------------------------------------------------------------------------*/
static struct ov_camera_module_custom_config ov5695_custom_config =
{
    .configs = ov5695_configs,
    .num_configs = ARRAY_SIZE(ov5695_configs),
    .power_up_delays_ms = {5, 20, 0},
    /*
     * 0: Exposure time valid fileds;
     * 1: Exposure gain valid fileds;
     * (2 fileds == 1 frames)
     */
    .exposure_valid_frame = {4, 4}
};

int rk_camera_ov5695_init(void)
{
    ret_err_t ret;
    rk_device *device = &g_ov5695.dev;
    struct ov5695_dev *instance = &g_ov5695;
    struct rk_camera_device *camera = &instance->parent;

    camera->ops = &rk_ov5695_ops;
    rt_strncpy(instance->name, OV5695_DEVICE_NAME, rt_strlen(OV5695_DEVICE_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

#if OV5695_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        OV5695_INFO(instance, "Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        OV5695_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

    if (ov5695_sensorInfo == NULL)
    {
        ov5695_sensorInfo = (struct sensor_config_info_s *)rt_malloc(sizeof(struct sensor_config_info_s));
        if (ov5695_sensorInfo != NULL)
        {
            rt_memset(ov5695_sensorInfo, 0x00, sizeof(struct sensor_config_info_s));
        }
        else
        {
            OV5695_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    if (ov5695_sensorModeData == NULL)
    {
        ov5695_sensorModeData = (struct isp_supplemental_sensor_mode_data *)rt_malloc(sizeof(struct isp_supplemental_sensor_mode_data));
        if (ov5695_sensorModeData != NULL)
        {
            rt_memset(ov5695_sensorModeData, 0x00, sizeof(struct isp_supplemental_sensor_mode_data));
        }
        else
        {
            OV5695_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    cam_power_init(0);
    cam_power_on(0);
    ret = rt_ov5695_detect_sensor(instance);
    cam_power_off(0);

    if (ret != RET_SYS_EOK)
        goto ERR;

    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, instance->name, rt_strlen(OV5695_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);

    instance->cam_mod.custom = ov5695_custom_config;
    instance->cam_mod.active_config = ov5695_configs;
    ret = ov5695_filltimings(&instance->cam_mod.custom);
    instance->cam_mod.active_config->line_length_pck = 740;
    instance->cam_mod.active_config->frame_length_lines = 2024;
    instance->cam_mod.custom.configs->line_length_pck = 740;
    instance->cam_mod.custom.configs->frame_length_lines = 2024;
    instance->cam_mod.exp_config.gain_percent = 100;

    return 0;
ERR:
    OV5695_DEBUG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

void ov5695_detect(void)
{
    struct ov5695_dev *instance = &g_ov5695;

    OV5695_DEBUG("start to detect OV5695 for testing \n");
    OV5695_DEBUG("dev name:%s\n", instance->name);
    OV5695_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        OV5695_DEBUG("Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        OV5695_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }

}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_ov5695_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(ov5695_detect, check g_ov5695 is available or not);
#endif
#endif

#endif

