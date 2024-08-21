/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_f37.c
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

#if defined(RT_USING_F37) || defined(CONFIG_DRIVER_F37)
#define F37_MODULE_COMPILED
#else
#undef F37_MODULE_COMPILED
#endif

#endif


#ifdef F37_MODULE_COMPILED

#define F37_DEBUG_INFO 0
#include "jx_camera_module.h"
#include "cam_iq_data_f37_v1.1_a2.h"


#if F37_DEBUG_INFO
#include <stdio.h>
#define F37_DEBUG(...)               rk_kprintf("[F37]:");rk_kprintf(__VA_ARGS__)
#else
#define F37_DEBUG(...)
#endif


#define F37_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define RT_USING_F37_OPS 0

#define F37_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME               "i2c0"
#define F37_7BIT_ADDRESS         (0x40)
#define F37_REG_ID_H_ADDRESS     (0x0A)
#define F37_REG_ID_L_ADDRESS     (0x0B)
#define F37_SENSOR_ID            (0x0F37)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define F37_PIN_CTRL_ENABLE          1
#define F37_I2C_DEBUG_ENABLE         1
#define F37_TESTPATTERN_ENABLE       0

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

#define F37_FETCH_GAIN(VAL)                  (VAL & 0xFF)    /* gain[7:0] */
#define F37_AEC_ANALOG_GAIN_REG              0x00            /* Bits 0 -7 */

#define F37_AEC_EXPO_HIGH_REG                0x02            /* Exposure Bits 8-15 */
#define F37_AEC_EXPO_LOW_REG                 0x01            /* Exposure Bits 0-7 */


#define F37_FETCH_HIGH_BYTE_EXP(VAL)        (((VAL) >> 8) & 0xFF)   /* 8-15 Bits */
#define F37_FETCH_LOW_BYTE_EXP(VAL)         ((VAL) & 0xFF)          /* 0-7 Bits */
#define F37_TIMING_VTS_HIGH_REG             0x23
#define F37_TIMING_VTS_LOW_REG              0x22
#define F37_TIMING_HTS_HIGH_REG             0x21
#define F37_TIMING_HTS_LOW_REG              0x20


#define F37_FINE_INTG_TIME_MIN                          0
#define F37_FINE_INTG_TIME_MAX_MARGIN                   0
#define F37_COARSE_INTG_TIME_MIN                        1
#define F37_COARSE_INTG_TIME_MAX_MARGIN                 4
#define F37_HORIZONTAL_VERTICAL_OUTPUT_SIZE_HIGH_REG    0x26 /*{ Vwin[11:8],Hwin[11:8]}*/
#define F37_HORIZONTAL_OUTPUT_SIZE_LOW_REG              0x24
#define F37_VERTICAL_OUTPUT_SIZE_LOW_REG                0x25
/*output window start position*/
#define F37_H_V_WIN_OFF_HIGH_REG                        0x29 /*{VwinSt[11:8],HwinSt[11:8]}*/
#define F37_H_WIN_OFF_LOW_REG                           0x27
#define F37_V_WIN_OFF_LOW_REG                           0x28
#define F37_FLIP_MIRROR_REG                             0x12 /*Bit[4]:flip;Bit[5]:mirror*/

#define F37_FLIPMIRROR_STATE_NORMAL          0
#define F37_FLIPMIRROR_STATE_FLIPMIRROR      1
#define F37_FLIPMIRROR_STATE_MIRROR          2
#define F37_FLIPMIRROR_STATE_FLIP            3

/* timing  ------------------------------------------------------------*/

#define F37_EXT_CLK     24000000

#define USE_30FPS
#define F37_LANES       2
#define F37_MIPICLK     324

static struct rt_mutex spi_spi2apb_lock;
#define F37_LOCK() rt_mutex_take(&spi_spi2apb_lock, RT_WAITING_FOREVER)
#define F37_UNLOCK() rt_mutex_release(&spi_spi2apb_lock)

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

struct f37_dev
{
    struct rk_camera_device parent;
    struct jx_camera_module cam_mod;
    rk_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];

    struct f37_ops *ops;

    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
    //struct rt_mutex mutex_lock;
};
typedef struct f37_dev *rt_f37_dev_t;

struct f37_ops
{
    ret_err_t (*init)(struct rk_camera_device *dev);
    ret_err_t (*open)(struct rk_camera_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_camera_device *dev);
    ret_err_t (*control)(struct rk_camera_device *dev, int cmd, void *arg);
};

struct f37_sensor_reg
{
    u8 reg_addr;
    u8 val;
};

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


static struct f37_dev g_f37;
struct sensor_config_info_s              *f37_sensorInfo = NULL;
struct isp_supplemental_sensor_mode_data *f37_sensorModeData = NULL;

static const struct f37_sensor_reg jx_f37_1080p_linear_1lane[] =
{
    {0x12, 0x60},
    {0x48, 0x8A},
    {0x48, 0x0A},
    {0x0E, 0x11},
    {0x0F, 0x14},
    {0x10, 0x36},
    {0x11, 0x80},
    {0x0D, 0xF0},
    {0x5F, 0x41},
    {0x60, 0x20},
    {0x58, 0x12},
    {0x57, 0x60},
    {0x9D, 0x00},
    {0x20, 0x80},
    {0x21, 0x07},
    {0x22, 0x65},
    {0x23, 0x04},
    {0x24, 0xC0},
    {0x25, 0x38},
    {0x26, 0x43},
    {0x27, 0x1A},
    {0x28, 0x15},
    {0x29, 0x07},
    {0x2A, 0x0A},
    {0x2B, 0x17},
    {0x2C, 0x00},
    {0x2D, 0x00},
    {0x2E, 0x14},
    {0x2F, 0x44},
    {0x41, 0xC8},
    {0x42, 0x3B},
    {0x47, 0x42},
    {0x76, 0x60},
    {0x77, 0x09},
    {0x1D, 0x00},
    {0x1E, 0x04},
    {0x6C, 0x40},
    {0x6E, 0x2C},
    {0x70, 0xf8},
    {0x71, 0xD0},
    {0x72, 0xD4},
    {0x73, 0x58},
    {0x74, 0x02},
    {0x78, 0xAE},
    {0x89, 0x01},
    {0x6B, 0x20},
    {0x86, 0x40},
    {0x31, 0x0C},
    {0x32, 0x38},
    {0x33, 0x6C},
    {0x34, 0x88},
    {0x35, 0x88},
    {0x3A, 0xAF},
    {0x3B, 0x00},
    {0x3C, 0x57},
    {0x3D, 0x78},
    {0x3E, 0xFF},
    {0x3F, 0xF8},
    {0x40, 0xFF},
    {0x56, 0xB2},
    {0x59, 0xE8},
    {0x5A, 0x04},
    {0x85, 0x70},
    {0x8A, 0x04},
    {0x91, 0x13},
    {0x9B, 0x03},
    {0x9C, 0xE1},
    {0xA9, 0x78},
    {0x5B, 0xB0},
    {0x5C, 0x71},
    {0x5D, 0xF6},
    {0x5E, 0x14},
    {0x62, 0x01},
    {0x63, 0x0F},
    {0x64, 0xC0},
    {0x65, 0x02},
    {0x67, 0x65},
    {0x66, 0x04},
    {0x68, 0x00},
    {0x69, 0x7C},
    {0x6A, 0x12},
    {0x7A, 0x80},
    {0x82, 0x21},
    {0x8F, 0x91},
    {0xAE, 0x30},
    {0x13, 0x81},
    {0x96, 0x04},
    {0x4A, 0x05},
    {0x7E, 0xCD},
    {0x50, 0x02},
    {0x49, 0x10},
    {0xAF, 0x12},
    {0x80, 0x41},
    {0x7B, 0x4A},
    {0x7C, 0x08},
    {0x7F, 0x57},
    {0x90, 0x00},
    {0x8C, 0xFF},
    {0x8D, 0xC7},
    {0x8E, 0x00},
    {0x8B, 0x01},
    {0x0C, 0x00},
    {0x81, 0x74},
    {0x19, 0x20},
    {0x46, 0x00},
    {0x12, 0x20},
    {0x48, 0x8A},
    {0x48, 0x0A},
    {REG_END, 0x00},
};

#ifdef USE_30FPS
static struct jx_camera_module_config f37_configs[] =
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
        .reg_table = (void *)jx_f37_1080p_linear_1lane,
        .reg_table_num_entries =
        sizeof(jx_f37_1080p_linear_1lane) /
        sizeof(jx_f37_1080p_linear_1lane[0]),
        .v_blanking_time_us = 1333,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, F37_LANES, F37_MIPICLK, F37_EXT_CLK, 0, 0x30)
    }
};
#endif

#ifdef USE_25FPS
static struct jx_camera_module_config f37_configs[] =
{
    {
        .name = "1920x1080_25fps",
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
        .reg_table = (void *)jx_f37_1080p_linear_1lane,
        .reg_table_num_entries =
        sizeof(jx_f37_1080p_linear_1lane) /
        sizeof(jx_f37_1080p_linear_1lane[0]),
        .v_blanking_time_us = 3300,
        .max_exp_gain_h = 16,
        .max_exp_gain_l = 0,
        PLTFRM_CAM_ITF_MIPI_CFG_EX(0, F37_LANES, F37_MIPICLK, F37_EXT_CLK, 0, 0x30)
    }
};
#endif

static ret_err_t f37_g_vts(struct f37_dev *dev, u32 *vts);

u32 f37_gcd(u32 a, u32 b)
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

static ret_err_t f37_write_reg(rk_i2c_bus_device *bus,
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

        msgs.addr = F37_7BIT_ADDRESS;
        msgs.flags = RT_I2C_WR;
        msgs.buf = send_buf;
        msgs.len = 2;
        ret = rt_i2c_transfer(bus, &msgs, 1);

        if (ret == 1)
        {
            return RET_SYS_EOK;
        }
    }
    F37_DEBUG("(%s):s0.0 i2c_bus error\n");
    return RET_SYS_ERROR;
}

static ret_err_t f37_read_reg(rk_i2c_bus_device *bus,
                              uint8_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = (reg & 0xff);
    msg[0].addr = F37_7BIT_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 1;
    msg[0].buf = send_buf;

    msg[1].addr = F37_7BIT_ADDRESS;
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

static ret_err_t f37_write_multiple_reg(struct f37_dev *dev,
                                        const struct f37_sensor_reg *reg)
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
            ret = f37_write_reg(i2c_bus, reg[i].reg_addr, &reg[i].val);
            if (ret != RET_SYS_EOK)
            {
                num = num + 1;
                rk_kprintf("[F37]: write init_table failed!!! num = %d\n", num);
            }
        }
    }
    return ret;

}

static ret_err_t rt_f37_detect_sensor(struct f37_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    ret_err_t ret;
    uint8_t pid, ver;
    uint16_t chipid;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    uint32_t cnt = 0;

    F37_DEBUG("(%s) enter \n", __FUNCTION__);

RETRY:
    cnt++;
    ret = f37_read_reg(bus, F37_REG_ID_H_ADDRESS, &pid);
    if (ret == RET_SYS_EOK)
    {
        ret = f37_read_reg(bus, F37_REG_ID_L_ADDRESS, &ver);
        chipid = SENSOR_ID(pid, ver);
        F37_INFO(dev, "Info: F37 pid:0x%x,ver:0x%x!\n", pid, ver);
        if (chipid == F37_SENSOR_ID)
        {
            F37_INFO(dev, "Info: F37 detected successfully !!!\n");
            ret = RET_SYS_EOK;
        }
        else
        {
            F37_INFO(dev, "Info: F37 detected failed,not match:0x%x != 0x%x !!!\n",
                     chipid, F37_SENSOR_ID);
            ret = -RT_EINVAL;
            if (cnt > CAMERA_BOOT_RETRY_NUM)
                goto EXIT;
            goto RETRY;
        }
    }
    else
    {
        F37_INFO(dev, "Warning: F37 i2c read failed !!!\n");
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }
EXIT:
    F37_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static void f37_stream_on(struct rk_camera_device *dev)
{
    uint8_t val;
    F37_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);

    struct f37_dev *f37;
    f37 = (struct f37_dev *)dev;
    ret_err_t ret = RET_SYS_EOK;
    u8 data;

    f37_write_multiple_reg(f37, jx_f37_1080p_linear_1lane);

    ret = f37_read_reg(f37->i2c_bus, 0x12, &data);
    data &= (~0x40);
    ret |= f37_write_reg(f37->i2c_bus, 0x12, &data);
    if (ret != RET_SYS_EOK)
    {
        F37_DEBUG("stream on failed!!! \n", __FUNCTION__);
    }

    F37_DEBUG("(%s) exit \n", __FUNCTION__);
}

static void f37_stream_off(struct rk_camera_device *dev)
{
    uint8_t data;
    struct f37_dev *f37 = (struct f37_dev *)dev;
    int ret;

    F37_DEBUG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);
    if (f37->i2c_bus)
    {
        ret = f37_read_reg(f37->i2c_bus, 0x12, &data);
        data |= 0x40;
        ret |= f37_write_reg(f37->i2c_bus, 0x12, &data);

        if (ret != RET_SYS_EOK)
        {
            F37_INFO(dev, "stream off failed with %d!\n", ret);
        }
    }
    else
    {
        F37_INFO(dev, "Err: not find out i2c bus!\n");
    }

    F37_DEBUG("(%s) exit \n", __FUNCTION__);
}

ret_err_t rk_f37_init(struct rk_camera_device *dev)
{
    F37_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct f37_dev *f37;

    f37 = (struct f37_dev *)&dev;
#if RT_USING_F37_OPS
    if (f37->ops->init)
    {
        return (f37->ops->init(f37));
    }
#else

    if (f37)
    {
        f37->i2c_bus = (rk_i2c_bus_device *)rt_device_find(f37->i2c_name);

        if (!f37->i2c_bus)
        {
            F37_DEBUG("Warning:not find i2c source:%s !!!\n",
                      f37->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            F37_DEBUG("(%s):s0 find i2c_bus:%s\n", f37->i2c_name);
        }
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    F37_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

ret_err_t f37_g_sensor_iq_info(struct f37_dev *dev, void *args)
{
    ret_err_t ret = 0;
    struct rk_camera_iq_info *info = (struct rk_camera_iq_info *)args;
    info->addr = (void *)cam_iq_data;
    return ret;
}

/*--------------------------------------------------------------------------*/

static ret_err_t f37_g_vts(struct f37_dev *dev, u32 *vts)
{
    u8 msb, lsb;
    ret_err_t ret = 0;
    u8 data;

    ret |= f37_read_reg(dev->i2c_bus, F37_TIMING_VTS_HIGH_REG, &msb);
    if (IS_ERR_VALUE(ret))
        goto err;

    ret |= f37_read_reg(dev->i2c_bus, F37_TIMING_VTS_LOW_REG, &lsb);
    if (ret != RET_SYS_EOK)
        goto err;

    *vts = msb & 0xff;
    *vts = *vts << 8;
    *vts = *vts | lsb;

    return RET_SYS_EOK;
err:
    F37_INFO(dev, "failed with error (%d)\n", ret);
    return ret;
}

/* ======================================================================== */

static ret_err_t f37_s_vts(struct f37_dev *dev,
                           u32 vts)
{
    ret_err_t ret = 0;
    u8 data;
    struct jx_camera_module *cam_mod = &dev->cam_mod;


    if (vts > 0xffff)
        vts = 0xffff;
    if (vts < cam_mod->vts_min)
        vts = cam_mod->vts_min;

    data = vts & 0xFF;
    ret |= f37_write_reg(dev->i2c_bus, F37_TIMING_VTS_LOW_REG, &data);
    data = (vts >> 8) & 0xFF;
    ret |= f37_write_reg(dev->i2c_bus, F37_TIMING_VTS_HIGH_REG, &data);

    if (IS_ERR_VALUE(ret))
    {
        F37_INFO(dev, "s_vts:failed with error (%d)\n", ret);
    }
    else
    {
        F37_DEBUG(dev, "updated vts = 0x%x,vts_min=0x%x\n", vts, cam_mod->vts_min);
        cam_mod->vts_cur = vts;
    }
    return ret;
}

/* ======================================================================== */

ret_err_t f37_auto_adjust_fps_state(struct f37_dev *dev, void *args)
{

    ret_err_t ret;
    bool *auto_adjust_fps = (bool *)args;
    dev->cam_mod.auto_adjust_fps = *auto_adjust_fps;
//    F37_INFO(dev, "state of f37_auto_adjust_fps is : %d.\n", dev->cam_mod.auto_adjust_fps);
    return RET_SYS_EOK;
}

/* ======================================================================== */

static ret_err_t f37_auto_adjust_fps(struct f37_dev *dev,
                                     u32 exp_time)
{
    ret_err_t ret = 0;
    u32 vts = 0;
    u8 data;

    struct jx_camera_module *cam_mod = &dev->cam_mod;

    if ((exp_time + F37_COARSE_INTG_TIME_MAX_MARGIN)
            > cam_mod->vts_min)
        vts = exp_time + F37_COARSE_INTG_TIME_MAX_MARGIN;
    else
        vts = cam_mod->vts_min;

    if (vts > 0xffff)
        vts = 0xffff;
    else
        vts = vts;

    data = vts & 0xFF;
    ret |= f37_write_reg(dev->i2c_bus, F37_TIMING_VTS_LOW_REG, &data);
    data = (vts >> 8) & 0xFF;
    ret |= f37_write_reg(dev->i2c_bus, F37_TIMING_VTS_HIGH_REG, &data);

    if (IS_ERR_VALUE(ret))
    {
        F37_INFO(dev, "failed with error (%d)\n", ret);
    }
    else
    {
        F37_DEBUG(dev, "updated vts = 0x%x,vts_min=0x%x\n", vts, cam_mod->vts_min);
        cam_mod->vts_cur = vts;
    }
    return ret;
}
static int f37_s_aec(struct f37_dev *dev, void *args)
{
    int i;
    u8 data;
    ret_err_t ret = 0;
    struct jx_camera_module *cam_mod = &dev->cam_mod;
    struct HAL_ISP_Set_Exp_s *aec = (struct HAL_ISP_Set_Exp_s *)args;

    /*
     * if the sensor is already streaming, write to shadow registers,
     * if the sensor is in SW standby, write to active registers,
     * if the sensor is off/registers are not writable, do nothing
     */
    if (cam_mod->state == JX_CAMERA_MODULE_SW_STANDBY ||
            cam_mod->state == JX_CAMERA_MODULE_STREAMING)
    {
        u32 a_gain = aec->gain;
        u32 coarse_again, fine_again;
        u32 exp_time = aec->exposure;

        a_gain = a_gain * aec->gain_percent / 100;
        if (a_gain > 248)
            a_gain = 248;

        if (a_gain <= 0x20)   /*1x ~ 2x*/
        {
            fine_again = a_gain - 16;
            coarse_again = (0x00 << 4);
        }
        else if (a_gain <= 0x40)     /*2x ~ 4x*/
        {
            fine_again = (a_gain >> 1) - 16;
            coarse_again = 0x01 << 4;
        }
        else if (a_gain <= 0x80)     /*4x ~ 8x*/
        {
            fine_again = (a_gain >> 2) - 16;
            coarse_again = 0x02 << 4;
        }
        else     /*8x ~ 15.5x*/
        {
            fine_again = (a_gain >> 3) - 16;
            coarse_again = 0x03 << 4;
        }

        if (!IS_ERR_VALUE(ret) && cam_mod->auto_adjust_fps)
            ret |= f37_auto_adjust_fps(dev, exp_time);

        data = (fine_again | coarse_again);
        ret |= f37_write_reg(dev->i2c_bus, F37_AEC_ANALOG_GAIN_REG, &data);

        data = F37_FETCH_HIGH_BYTE_EXP(exp_time);
        ret |= f37_write_reg(dev->i2c_bus, F37_AEC_EXPO_HIGH_REG, &data);

        data = F37_FETCH_LOW_BYTE_EXP(exp_time);
        ret |= f37_write_reg(dev->i2c_bus, F37_AEC_EXPO_LOW_REG, &data);

        if (!cam_mod->auto_adjust_fps)
            ret |= f37_s_vts(dev,
                             cam_mod->exp_config.vts_value);
    }

    if (ret != RET_SYS_EOK)
    {
        F37_INFO(dev, "s_aec:failed with error (%d)\n", ret);
    }

    return ret;
}

/* ======================================================================== */

ret_err_t f37_g_fmt(struct f37_dev *dev, void *args)
{
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    if (dev->cam_mod.active_config)
    {
        fmt->code = dev->cam_mod.active_config->frm_fmt.code;
        fmt->width = dev->cam_mod.active_config->frm_fmt.width;
        fmt->height = dev->cam_mod.active_config->frm_fmt.height;
        return RET_SYS_EOK;
    }

    F37_INFO(dev, "Warning: no active config\n");
    return RET_SYS_ERROR;
}
/* ======================================================================== */

ret_err_t f37_g_sensor_frm_interval(struct f37_dev *dev, void *args)
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

    F37_INFO(dev, "no active config\n");
    return RET_SYS_ERROR;
}

/* ======================================================================== */

static void jx_camera_module_set_active_config(
    struct f37_dev *dev,
    struct jx_camera_module_config *new_config)
{

    if (RK_NULL == new_config)
    {
        dev->cam_mod.active_config = new_config;
        F37_INFO(dev, "Warning: no active config\n");
    }
    else
    {
        dev->cam_mod.ctrl_updt &= JX_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP |
                                  JX_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN |
                                  JX_CAMERA_MODULE_CTRL_UPDT_AUTO_WB;
        if (new_config->auto_exp_enabled !=
                dev->cam_mod.exp_config.auto_exp)
        {
            dev->cam_mod.ctrl_updt |=
                JX_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP;
            dev->cam_mod.exp_config.auto_exp =
                new_config->auto_exp_enabled;
        }
        if (new_config->auto_gain_enabled !=
                dev->cam_mod.exp_config.auto_gain)
        {
            dev->cam_mod.ctrl_updt |=
                JX_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN;
            dev->cam_mod.exp_config.auto_gain =
                new_config->auto_gain_enabled;
        }
        if (new_config->auto_wb_enabled !=
                dev->cam_mod.wb_config.auto_wb)
        {
            dev->cam_mod.ctrl_updt |=
                JX_CAMERA_MODULE_CTRL_UPDT_AUTO_WB;
            dev->cam_mod.wb_config.auto_wb =
                new_config->auto_wb_enabled;
        }
        if (new_config != dev->cam_mod.active_config)
        {
            dev->cam_mod.update_config = true;
            dev->cam_mod.active_config = new_config;
            F37_INFO(dev,
                     "activating config '%s'\n",
                     dev->cam_mod.active_config->name);
        }
    }
}

/* ======================================================================== */

static struct jx_camera_module_config *jx_camera_module_find_config(
    struct f37_dev *dev,
    struct v4l2_mbus_framefmt *fmt,
    struct v4l2_subdev_frame_interval *frm_intrvl)
{
    u32 i;
    u32 gcdiv;
    struct v4l2_subdev_frame_interval norm_interval;

    F37_INFO(dev, "fmt = 0x%x,frm_intrvl = 0x%x\n", fmt, frm_intrvl);

    if (NULL != fmt)
        F37_INFO(dev,
                 "jx_camera_module_find_config:%dx%d, fmt code 0x%04x\n",
                 fmt->width, fmt->height, fmt->code);

    if (NULL != frm_intrvl)
    {
        F37_INFO(dev,
                 "jx_camera_module_find_config:frame interval %d/%d....\n",
                 frm_intrvl->interval.numerator,
                 frm_intrvl->interval.denominator);
    }


    for (i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (NULL != frm_intrvl)
        {
            gcdiv = f37_gcd(dev->cam_mod.custom.configs[i].
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
        F37_INFO(dev,
                 "found matching config %s\n",
                 dev->cam_mod.custom.configs[i].name);
        return &dev->cam_mod.custom.configs[i];
    }
    F37_INFO(dev, "no matching config found\n");

    return RK_NULL;
}

/* ======================================================================== */

ret_err_t f37_s_fmt(struct f37_dev *dev, void *args)
{
    struct jx_camera_module_config *config;
    struct v4l2_mbus_framefmt *fmt = (struct v4l2_mbus_framefmt *)args;
    ret_err_t ret = RET_SYS_EOK;

    F37_INFO(dev, "f37_s_fmt:%dx%d, fmt code 0x%04x\n",
             fmt->width, fmt->height, fmt->code);

    config = jx_camera_module_find_config(dev, fmt, NULL);

    if (config == NULL)
    {
        F37_INFO(dev,
                 "format %dx%d, code 0x%04x, not supported\n",
                 fmt->width, fmt->height, fmt->code);
        ret = RET_SYS_ERROR;
        goto err;
    }
    dev->cam_mod.frm_fmt_valid = true;
    dev->cam_mod.frm_fmt = *fmt;
    F37_INFO(dev, "fmt is supported,%dx%d, fmt code 0x%04x\n", fmt->width, fmt->height, fmt->code);
    struct jx_camera_module_config *config_found = jx_camera_module_find_config(
                dev, fmt, &dev->cam_mod.frm_intrvl);

    if (dev->cam_mod.frm_intrvl_valid &&
            (RK_NULL != config_found))
    {
        jx_camera_module_set_active_config(dev, config_found);
    }
    else
    {
        jx_camera_module_set_active_config(dev, config);
    }

    return RET_SYS_EOK;
err:
    ret = RET_SYS_ERROR;
    F37_INFO(dev,
             "failed with error %d\n", ret);
    return ret;
}

/* ======================================================================== */

ret_err_t f37_g_v_blank_us(struct f37_dev *dev, void *args)
{
    u32 *v_blanking_time_us = (u32 *)args;
    *v_blanking_time_us = dev->cam_mod.active_config->v_blanking_time_us;
    return RET_SYS_EOK;
}

/* ======================================================================== */

ret_err_t f37_g_itf_cfg(struct f37_dev *dev, void *args)
{
    struct pltfrm_cam_itf *itf_cfg = (struct pltfrm_cam_itf *)args;
    struct jx_camera_module_config *config;

    if (dev->cam_mod.custom.num_configs <= 0)
    {
        F37_INFO(dev,
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

static ret_err_t f37_g_timings(struct f37_dev *dev,
                               struct jx_camera_module_timings *timings)
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
    F37_INFO(dev, "cam_mod:failed with error (%d)\n", RET_SYS_EEMPTY);
    return RET_SYS_EEMPTY;
}

/* ======================================================================== */

ret_err_t f37_g_sensor_mode(struct f37_dev *dev, void *args)
{
    ret_err_t ret;
    struct jx_camera_module_timings jx_timings;

    int *args_data = (int *)args;

    ret = f37_g_timings(dev, &jx_timings);

    if (RET_SYS_EOK != ret)
    {
        F37_INFO(dev,
                 "failed with error %d\n", ret);
        return ret;
    }

    f37_sensorModeData->isp_input_horizontal_start = 0;
    f37_sensorModeData->isp_input_vertical_start   = 0;
    f37_sensorModeData->isp_input_width   = 1920;
    f37_sensorModeData->isp_input_height  = 1080;
    f37_sensorModeData->isp_output_width  = 1920;
    f37_sensorModeData->isp_output_height = 1080;

    f37_sensorModeData->sensor_output_width = jx_timings.sensor_output_width;
    f37_sensorModeData->sensor_output_height = jx_timings.sensor_output_height;
    f37_sensorModeData->crop_horizontal_start = jx_timings.crop_horizontal_start;
    f37_sensorModeData->crop_vertical_start = jx_timings.crop_vertical_start;
    f37_sensorModeData->crop_horizontal_end = jx_timings.crop_horizontal_end;
    f37_sensorModeData->crop_vertical_end = jx_timings.crop_vertical_end;
    f37_sensorModeData->line_length_pck = jx_timings.line_length_pck;
    f37_sensorModeData->frame_length_lines = jx_timings.frame_length_lines;
    f37_sensorModeData->vt_pix_clk_freq_hz = jx_timings.vt_pix_clk_freq_hz;
    f37_sensorModeData->binning_factor_x = jx_timings.binning_factor_x;
    f37_sensorModeData->binning_factor_y = jx_timings.binning_factor_y;
    f37_sensorModeData->coarse_integration_time_max_margin =
        jx_timings.coarse_integration_time_max_margin;
    f37_sensorModeData->coarse_integration_time_min =
        jx_timings.coarse_integration_time_min;
    f37_sensorModeData->fine_integration_time_max_margin =
        jx_timings.fine_integration_time_max_margin;
    f37_sensorModeData->fine_integration_time_min =
        jx_timings.fine_integration_time_min;

    f37_sensorModeData->exposure_valid_frame[0] =
        dev->cam_mod.custom.exposure_valid_frame[0];
    f37_sensorModeData->exposure_valid_frame[1] =
        dev->cam_mod.custom.exposure_valid_frame[1];
    f37_sensorModeData->lane_num = F37_LANES;
    if (dev->cam_mod.exp_config.exp_time)
    {
        f37_sensorModeData->exp_time = dev->cam_mod.exp_config.exp_time;
        f37_sensorModeData->exp_time_l = dev->cam_mod.exp_config.exp_time_l;
        f37_sensorModeData->exp_time_s = dev->cam_mod.exp_config.exp_time_s;
    }
    else
    {
        f37_sensorModeData->exp_time = jx_timings.exp_time;
        f37_sensorModeData->exp_time_l = jx_timings.exp_time;
        f37_sensorModeData->exp_time_s = jx_timings.exp_time;
    }
    if (dev->cam_mod.exp_config.gain)
    {
        f37_sensorModeData->gain = dev->cam_mod.exp_config.gain;
        f37_sensorModeData->gain_l = dev->cam_mod.exp_config.gain_l;
        f37_sensorModeData->gain_s = dev->cam_mod.exp_config.gain_s;
    }
    else
    {
        f37_sensorModeData->gain = jx_timings.gain;
        f37_sensorModeData->gain_l = jx_timings.gain;
        f37_sensorModeData->gain_s = jx_timings.gain;
    }
    if (dev->cam_mod.active_config)
    {
        f37_sensorModeData->max_exp_gain_h = dev->cam_mod.active_config->max_exp_gain_h;
        f37_sensorModeData->max_exp_gain_l = dev->cam_mod.active_config->max_exp_gain_l;
    }
    else
    {
        f37_sensorModeData->max_exp_gain_h = dev->cam_mod.custom.configs[0].max_exp_gain_h;
        f37_sensorModeData->max_exp_gain_l = dev->cam_mod.custom.configs[0].max_exp_gain_l;
    }

    *args_data = (int)f37_sensorModeData;

    return ret;
}

/*--------------------------------------------------------------------------*/

static int f37_filltimings(struct jx_camera_module_custom_config *custom)
{
    int i, j;
    u32 win_h_off = 0, win_v_off = 0;
    u32 vts = 0, hts = 0, winv = 0, winh = 0;
    struct jx_camera_module_config *config;
    struct jx_camera_module_timings *timings;
    struct jx_camera_module_reg *reg_table;
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
            case F37_TIMING_VTS_HIGH_REG:
                vts = ((reg_table[j].val << 8) |
                       (vts & 0xff));
                break;
            case F37_TIMING_VTS_LOW_REG:
                vts = (reg_table[j].val |
                       (vts & 0xff00));
                break;
            case F37_TIMING_HTS_HIGH_REG:
                hts = ((reg_table[j].val << 8) |
                       hts);
                break;
            case F37_TIMING_HTS_LOW_REG:
                hts = (reg_table[j].val |
                       (hts & 0x0f00));
                break;
            case F37_HORIZONTAL_VERTICAL_OUTPUT_SIZE_HIGH_REG:
            {
                timings->sensor_output_height =
                    (reg_table[j].val & 0xf0) << 4;
                timings->sensor_output_width =
                    ((reg_table[j].val & 0x0f) << 8);
                break;
            }
            case F37_VERTICAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_height =
                    (reg_table[j].val |
                     (timings->sensor_output_height & 0xff00));
                break;
            case F37_HORIZONTAL_OUTPUT_SIZE_LOW_REG:
                timings->sensor_output_width =
                    (reg_table[j].val |
                     (timings->sensor_output_width & 0xff00));
                break;
            case F37_H_V_WIN_OFF_HIGH_REG:
            {
                win_h_off = (reg_table[j].val & 0x0f) << 8;
                win_v_off = (reg_table[j].val & 0xf0) << 4;
                break;
            }
            case F37_H_WIN_OFF_LOW_REG:
                win_h_off = (reg_table[j].val & 0xff) | (win_h_off & 0xff00);
                break;
            case F37_V_WIN_OFF_LOW_REG:
                win_v_off = (reg_table[j].val & 0xff) | (win_v_off & 0xff00);
                break;
            }
        }

        timings->frame_length_lines = vts;
        timings->line_length_pck = hts;

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
            F37_COARSE_INTG_TIME_MIN;
        timings->coarse_integration_time_max_margin =
            F37_COARSE_INTG_TIME_MAX_MARGIN;

        /* OV Sensor do not use fine integration time. */
        timings->fine_integration_time_min =
            F37_FINE_INTG_TIME_MIN;
        timings->fine_integration_time_max_margin =
            F37_FINE_INTG_TIME_MAX_MARGIN;

        timings->binning_factor_x = 1;
        timings->binning_factor_y = 1;
    }

    return 0;
}

ret_err_t f37_g_sensor_configInfo(struct f37_dev *dev, void *args)
{

    ret_err_t ret;
    int *args_data = (int *)args;

    f37_sensorInfo->config_num = dev->cam_mod.custom.num_configs;
    for (u8 i = 0; i < dev->cam_mod.custom.num_configs; i++)
    {
        if (i >= SENSOR_CONFIG_NUM)
            break;
        f37_sensorInfo->sensor_fmt[i] =
            pltfrm_camera_module_pix_fmt2csi2_dt(dev->cam_mod.custom.configs[i].frm_fmt.code);
        f37_sensorInfo->reso[i].width  = dev->cam_mod.custom.configs[i].frm_fmt.width;
        f37_sensorInfo->reso[i].height = dev->cam_mod.custom.configs[i].frm_fmt.height;
    }

    *args_data = (int)f37_sensorInfo;
    return 0;
}

static ret_err_t f37_s_flipMirror(struct f37_dev *dev, void *args)
{
    ret_err_t ret = 0;
    u8 data;
    u8 *state = (u8 *)args;
    ret = f37_read_reg(dev->i2c_bus, 0x12, &data);

    switch (*state)
    {
    case F37_FLIPMIRROR_STATE_NORMAL:
    {
        data &= ~0x30;
        ret |= f37_write_reg(dev->i2c_bus, 0x12, &data);
        f37_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SRGGB10_1X10;
    }
    break;
    case F37_FLIPMIRROR_STATE_FLIPMIRROR:
    {
        data |= 0x30;
        ret |= f37_write_reg(dev->i2c_bus, 0x12, &data);
        f37_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SBGGR10_1X10;
    }
    break;
    case F37_FLIPMIRROR_STATE_MIRROR:
    {
        data &= ~0x10;
        data |= 0x20;
        ret |= f37_write_reg(dev->i2c_bus, 0x12, &data);
        f37_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SGRBG10_1X10;
    }
    break;
    case F37_FLIPMIRROR_STATE_FLIP:
    {
        data &= ~0x20;
        data |= 0x10;
        ret |= f37_write_reg(dev->i2c_bus, 0x12, &data);
        f37_configs[0].frm_fmt.code = V4L2_MBUS_FMT_SGBRG10_1X10;
    }
    break;
    default:
    {
        F37_INFO(dev, "%d is wrong flip_mirror state!!!\n", *state);
        ret = RET_SYS_ERROR;
    }
    break;
    }

    if (IS_ERR_VALUE(ret))
    {
        F37_INFO(dev, "s_flipMirror:failed with error (%d)\n", ret);
    }

    return ret;
}

ret_err_t rk_f37_control(struct rk_camera_device *dev, int cmd, void *args)
{
    F37_DEBUG("(%s) enter cmd = %d \n", __FUNCTION__, cmd);

    ret_err_t ret = RET_SYS_EOK;
    struct f37_dev *f37;

    MACRO_ASSERT(dev != RK_NULL);
    f37 = (struct f37_dev *)dev;
#if RT_USING_F37_OPS
    return (f37->ops->control(f37, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_f37_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        f37_stream_on(dev);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        f37_stream_off(dev);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_IQ_INFO:
    {
        ret = f37_g_sensor_iq_info(f37, args);
    }
    break;
//    case RK_DEVICE_CTRL_CAMERA_GET_AEC:
//    {
//        ret = f37_g_aec(f37, args);
//    }
//    break;

    case RK_DEVICE_CTRL_CAMERA_SET_AEC:
    {
        ret = f37_s_aec(f37, args);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
    {
        ret = f37_g_fmt(f37, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FORMAT:
    {
        ret = f37_s_fmt(f37, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_BLANK:
    {
        ret = f37_g_v_blank_us(f37, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_ITF_CFG:
    {
        ret = f37_g_itf_cfg(f37, args);
    }
    break;

    case RK_VIDIOC_SENSOR_MODE_DATA:
    {
        ret = f37_g_sensor_mode(f37, args);
    }
    break;

    case RK_VIDIOC_SENSOR_CONFIGINFO:
    {
        ret = f37_g_sensor_configInfo(f37, args);
    }
    break;

    case RK_V4L2_CID_AUTO_FPS:
    {
        ret = f37_auto_adjust_fps_state(f37, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_GET_FRMINTVAL:
    {
        ret = f37_g_sensor_frm_interval(f37, args);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_SET_FLIPMIRROR:
    {
        ret = f37_s_flipMirror(f37, args);
    }
    break;

    default:
        break;
    }
#endif
    F37_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static ret_err_t rk_f37_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    F37_DEBUG("(%s) enter \n", __FUNCTION__);

    struct f37_dev *f37;
    struct rk_camera_device *camera;
    ret_err_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    f37 = (struct f37_dev *)&g_f37;
    camera = (struct rk_camera_device *)(&f37->parent);

    cam_power_on(0);

    //f37_write_multiple_reg(f37, jx_f37_1080p_linear_1lane);
    ret = f37_g_vts(f37, &f37->cam_mod.vts_min);

    F37_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

static ret_err_t rk_f37_close(struct rk_camera_device *dev)
{
    struct f37_dev *f37;
    uint8_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    f37 = (struct f37_dev *)dev;

    cam_power_off(0);

    //f37_INFO(dev, "(%s) exit \n", __FUNCTION__);
    return ret;

}

struct rk_camera_ops rk_f37_ops =
{
    rk_f37_init,
    rk_f37_open,
    rk_f37_close,
    rk_f37_control
};

static struct jx_camera_module_custom_config f37_custom_config =
{
    .configs = f37_configs,
    .num_configs = sizeof(f37_configs) / sizeof(f37_configs[0]),
    .power_up_delays_ms = {5, 30, 30},
    .exposure_valid_frame = {4, 4},
};

int rk_camera_f37_init(void)
{
    ret_err_t ret;
    rk_device *device = &g_f37.dev;
    struct f37_dev *instance = &g_f37;
    struct rk_camera_device *camera = &instance->parent;
    camera->ops = &rk_f37_ops;

    rt_strncpy(instance->name, F37_DEVICE_NAME, rt_strlen(F37_DEVICE_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

#if F37_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        F37_INFO(instance, "Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        F37_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

    if (f37_sensorInfo == NULL)
    {
        f37_sensorInfo = (struct sensor_config_info_s *)rt_malloc(sizeof(struct sensor_config_info_s));
        if (f37_sensorInfo != NULL)
        {
            rt_memset(f37_sensorInfo, 0x00, sizeof(struct sensor_config_info_s));
        }
        else
        {
            F37_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    if (f37_sensorModeData == NULL)
    {
        f37_sensorModeData = (struct isp_supplemental_sensor_mode_data *)rt_malloc(sizeof(struct isp_supplemental_sensor_mode_data));
        if (f37_sensorModeData != NULL)
        {
            rt_memset(f37_sensorModeData, 0x00, sizeof(struct isp_supplemental_sensor_mode_data));
        }
        else
        {
            F37_INFO(instance, "malloc failed!!!\n");
            ret = RET_SYS_ENOMEM;
            goto ERR;
        }
    }

    cam_power_init(0);
    cam_power_on(0);
    ret = rt_f37_detect_sensor(instance);
    cam_power_off(0);

    if (ret != RET_SYS_EOK)
        goto ERR;

    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, instance->name, rt_strlen(F37_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);

    instance->cam_mod.custom = f37_custom_config;
    instance->cam_mod.active_config = f37_configs;
    ret = f37_filltimings(&instance->cam_mod.custom);
    instance->cam_mod.frm_intrvl = instance->cam_mod.active_config->frm_intrvl;
    instance->cam_mod.exp_config.gain_percent = 100;
    instance->cam_mod.state = JX_CAMERA_MODULE_STREAMING;
    instance->cam_mod.auto_adjust_fps = 1;

    //rk_f37_open(camera, 0);

    F37_DEBUG("(%s) exit \n", __FUNCTION__);
    return 0;
ERR:
    if (f37_sensorModeData != NULL)
    {
        rt_free(f37_sensorModeData);
        f37_sensorModeData = NULL;
    }
    if (f37_sensorInfo != NULL)
    {
        rt_free(f37_sensorInfo);
        f37_sensorInfo = NULL;
    }
    F37_DEBUG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

void f37_detect(void)
{
    struct f37_dev *instance = &g_f37;

    F37_DEBUG("start to detect F37 for testing \n");
    F37_DEBUG("dev name:%s\n", instance->name);
    F37_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        F37_DEBUG("Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        F37_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }
    //rt_ov8825_detect_sensor(instance);
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_f37_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(f37_detect, check g_f37 is available or not);
#endif
#endif

#endif

