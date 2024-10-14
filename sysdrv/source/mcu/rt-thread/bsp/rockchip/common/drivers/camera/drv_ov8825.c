/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_gc2145.c
  * @version V0.0.1
  * @brief   video input processor driver for rk2108
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
#include "board.h"
#elif defined(__RK_OS__)
#include "driver/camera.h"
#endif

#ifdef CAMERA_MODULE_COMPILED

#if defined(__RT_THREAD__)
//#include "drv_clock.h"
//#include "board.h"
#elif defined(__RK_OS__)
#include "driver/drv_clock.h"
#endif

#if defined(RT_USING_OV8825) || defined(CONFIG_DRIVER_OV8825)
#define OV8825_MODULE_COMPILED
#else
#undef OV8825_MODULE_COMPILED
#endif

#endif


#ifdef OV8825_MODULE_COMPILED

#define OV8825_DEBUG_INFO 1

#if OV8825_DEBUG_INFO
#include <stdio.h>
#define OV8825_DEBUG(...)               rk_kprintf("[OV8825]:");rk_kprintf(__VA_ARGS__)
#else
#define OV8825_DEBUG(...)
#endif

#define OV8825_INFO(dev,arg...)         rk_kprintf("[%s]:",dev->name);rk_kprintf(arg)

#define RT_USING_OV8825_OPS 0

#define OV8825_DEVICE_NAME          "sensor_0"
#define I2C_BUS_NAME                "i2c0"
#define OV8825_7BIT_ADDRESS         (0x36)
#define OV8825_REG_ID_H_ADDRESS     (0x300A)
#define OV8825_REG_ID_L_ADDRESS     (0x300B)
#define OV8825_SENSOR_ID            (0x8825)

#define REG_END                     (0x0)
#define REG_DELAY                   (0xff)

#define SENSOR_ID(_msb, _lsb)       ((_msb) << 8 | (_lsb))
#define ARRAY_SIZE(x)               (sizeof(x) / sizeof((x)[0]))

#define OV8825_PIN_CTRL_ENABLE          1
#define OV8825_I2C_DEBUG_ENABLE         1
#define OV8825_TESTPATTERN_ENABLE       0

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

struct ov8825_dev
{
    struct rk_camera_device parent;
    rk_device dev;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];

#if 1//RT_USING_OV8825_OPS
    struct ov8825_ops *ops;
#endif

    int32_t pin_rst;
    int32_t pin_pwdn;
    int32_t pin_clkout;
    //struct mclk mclk;
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    rk_i2c_bus_device *i2c_bus;
    //struct rt_mutex mutex_lock;
};
typedef struct ov8825_dev *rt_ov8825_dev_t;

struct ov8825_ops
{
    ret_err_t (*init)(struct rk_camera_device *dev);
    ret_err_t (*open)(struct rk_camera_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_camera_device *dev);
    ret_err_t (*control)(struct rk_camera_device *dev, int cmd, void *arg);
};

struct ov8825_sensor_reg
{
    uint16_t reg_addr;
    uint8_t val;
};

static struct ov8825_dev g_ov8825;
static const struct ov8825_sensor_reg g_ov8825_init_reg_table[] =
{
    {0x0103, 0x01}, //,0xsoftware,0xreset);
    {0x3000, 0x16},
    {0x3001, 0x00},
    {0x3002, 0x6c},
    {0x3003, 0xce},
    {0x3004, 0xd4},
    {0x3005, 0x10}, // reduce sensor frequency
    {0x3006, 0x10},
    {0x3007, 0x3b},
    {0x300d, 0x00},
    {0x301f, 0x09},
    {0x3020, 0x01},
    {0x3010, 0x00},

    {0x3011, 0x00}, //one lane;
    //{0x3011,0x01}, //two lane;

    {0x3012, 0x80},
    {0x3013, 0x39},
    {0x3018, 0x00},
    {0x3104, 0x20},
    {0x3106, 0x15},
    {0x3300, 0x00},
    {0x3500, 0x00},
    {0x3501, 0x4e},
    {0x3502, 0xa0},
    {0x3503, 0x07},
    {0x3509, 0x10},
    {0x350b, 0x1f},
    {0x3600, 0x06},
    {0x3601, 0x34},
    {0x3602, 0x42},
    {0x3603, 0x5c},
    {0x3604, 0x98},
    {0x3605, 0xf5},
    {0x3609, 0xb4},
    {0x360a, 0x7c},
    {0x360b, 0xc9},
    {0x360c, 0x0b},
    {0x3612, 0x00},
    {0x3613, 0x02},
    {0x3614, 0x0f},
    {0x3615, 0x00},
    {0x3616, 0x03},
    {0x3617, 0xa1},
    {0x3618, 0x00},
    {0x3619, 0x00},
    {0x361a, 0x00},
    {0x361b, 0x00},
    {0x3700, 0x20},
    {0x3701, 0x44},
    {0x3702, 0x50},
    {0x3703, 0xcc},
    {0x3704, 0x19},
    {0x3705, 0x32},
    {0x3706, 0x4b},
    {0x3707, 0x63},
    {0x3708, 0x84},
    {0x3709, 0x40},
    {0x370a, 0x33},
    {0x370b, 0x01},
    {0x370c, 0x50},
    {0x370d, 0x00},
    {0x370e, 0x00},
    {0x3711, 0x0f},
    {0x3712, 0x9c},
    {0x3724, 0x01},
    {0x3725, 0x92},
    {0x3726, 0x01},
    {0x3727, 0xc7},
    {0x3800, 0x00},
    {0x3801, 0x00},
    {0x3802, 0x00},
    {0x3803, 0x00},
    {0x3804, 0x0c},
    {0x3805, 0xdf},
    {0x3806, 0x09},
    {0x3807, 0x9b},
    {0x3808, 0x06},
    {0x3809, 0x60},
    {0x380a, 0x04},
    {0x380b, 0xc8},
    {0x380c, 0x0d},
    {0x380d, 0xbc},
    {0x380e, 0x04},
    {0x380f, 0xf0},
    {0x3810, 0x00},
    {0x3811, 0x08},
    {0x3812, 0x00},
    {0x3813, 0x04},
    {0x3814, 0x31},
    {0x3815, 0x31},
    {0x3816, 0x02},
    {0x3817, 0x40},
    {0x3818, 0x00},
    {0x3819, 0x40},
    {0x3820, 0x81},
    {0x3821, 0x17},
    {0x3b1f, 0x00},
    {0x3d00, 0x00},
    {0x3d01, 0x00},
    {0x3d02, 0x00},
    {0x3d03, 0x00},
    {0x3d04, 0x00},
    {0x3d05, 0x00},
    {0x3d06, 0x00},
    {0x3d07, 0x00},
    {0x3d08, 0x00},
    {0x3d09, 0x00},
    {0x3d0a, 0x00},
    {0x3d0b, 0x00},
    {0x3d0c, 0x00},
    {0x3d0d, 0x00},
    {0x3d0e, 0x00},
    {0x3d0f, 0x00},
    {0x3d10, 0x00},
    {0x3d11, 0x00},
    {0x3d12, 0x00},
    {0x3d13, 0x00},
    {0x3d14, 0x00},
    {0x3d15, 0x00},
    {0x3d16, 0x00},
    {0x3d17, 0x00},
    {0x3d18, 0x00},
    {0x3d19, 0x00},
    {0x3d1a, 0x00},
    {0x3d1b, 0x00},
    {0x3d1c, 0x00},
    {0x3d1d, 0x00},
    {0x3d1e, 0x00},
    {0x3d1f, 0x00},
    {0x3d80, 0x00},
    {0x3d81, 0x00},
    {0x3d84, 0x00},
    {0x3f00, 0x00},
    {0x3f01, 0xfc},
    {0x3f05, 0x10},
    {0x3f06, 0x00},
    {0x3f07, 0x00},
    {0x4000, 0x29},
    {0x4001, 0x02},
    {0x4002, 0x45},
    {0x4003, 0x08},
    {0x4004, 0x04},
    {0x4005, 0x18},
    {0x404e, 0x37},
    {0x404f, 0x8f},
    {0x4300, 0xff},
    {0x4303, 0x00},
    {0x4304, 0x08},
    {0x4307, 0x00},
    {0x4600, 0x04},
    {0x4601, 0x00},
    {0x4602, 0x30},
    {0x4800, 0x04},
    {0x4801, 0x0f},
    {0x4814, 0x2b}, //data type
    {0x4837, 0x30}, // reduce sensor frequency
    {0x4843, 0x02},
    {0x5000, 0x06},
    {0x5001, 0x00},
    {0x5002, 0x00},
    {0x5068, 0x00},
    {0x506a, 0x00},
    {0x501f, 0x00},
    {0x5780, 0xfc},
    {0x5c00, 0x80},
    {0x5c01, 0x00},
    {0x5c02, 0x00},
    {0x5c03, 0x00},
    {0x5c04, 0x00},
    {0x5c05, 0x00},
    {0x5c06, 0x00},
    {0x5c07, 0x80},
    {0x5c08, 0x10},
    {0x6700, 0x05},
    {0x6701, 0x19},
    {0x6702, 0xfd},
    {0x6703, 0xd7},
    {0x6704, 0xff},
    {0x6705, 0xff},
    {0x6800, 0x10},
    {0x6801, 0x02},
    {0x6802, 0x90},
    {0x6803, 0x10},
    {0x6804, 0x59},
    {0x6900, 0x60},
    {0x6901, 0x04},
    {0x5800, 0x0f},
    {0x5801, 0x0d},
    {0x5802, 0x09},
    {0x5803, 0x0a},
    {0x5804, 0x0d},
    {0x5805, 0x14},
    {0x5806, 0x0a},
    {0x5807, 0x04},
    {0x5808, 0x03},
    {0x5809, 0x03},
    {0x580a, 0x05},
    {0x580b, 0x0a},
    {0x580c, 0x05},
    {0x580d, 0x02},
    {0x580e, 0x00},
    {0x580f, 0x00},
    {0x5810, 0x03},
    {0x5811, 0x05},
    {0x5812, 0x09},
    {0x5813, 0x03},
    {0x5814, 0x01},
    {0x5815, 0x01},
    {0x5816, 0x04},
    {0x5817, 0x09},
    {0x5818, 0x09},
    {0x5819, 0x08},
    {0x581a, 0x06},
    {0x581b, 0x06},
    {0x581c, 0x08},
    {0x581d, 0x06},
    {0x581e, 0x33},
    {0x581f, 0x11},
    {0x5820, 0x0e},
    {0x5821, 0x0f},
    {0x5822, 0x11},
    {0x5823, 0x3f},
    {0x5824, 0x08},
    {0x5825, 0x46},
    {0x5826, 0x46},
    {0x5827, 0x46},
    {0x5828, 0x46},
    {0x5829, 0x46},
    {0x582a, 0x42},
    {0x582b, 0x42},
    {0x582c, 0x44},
    {0x582d, 0x46},
    {0x582e, 0x46},
    {0x582f, 0x60},
    {0x5830, 0x62},
    {0x5831, 0x42},
    {0x5832, 0x46},
    {0x5833, 0x46},
    {0x5834, 0x44},
    {0x5835, 0x44},
    {0x5836, 0x44},
    {0x5837, 0x48},
    {0x5838, 0x28},
    {0x5839, 0x46},
    {0x583a, 0x48},
    {0x583b, 0x68},
    {0x583c, 0x28},
    {0x583d, 0xae},
    {0x5842, 0x00},
    {0x5843, 0xef},
    {0x5844, 0x01},
    {0x5845, 0x3f},
    {0x5846, 0x01},
    {0x5847, 0x3f},
    {0x5848, 0x00},
    {0x5849, 0xd5},
    {0x3400, 0x04}, //,0xred,0xh);
    {0x3401, 0x00}, //,0xred,0xl);
    {0x3402, 0x04}, //,0xgreen,0xhRKI2CWrite(I2C_Sensor_Addr,0x3403,0x00);//,0xgreen,0xl);
    {0x3404, 0x04}, //,0xblue,0xh);
    {0x3405, 0x00}, //,0xblue,0xl);
    {0x3406, 0x01}, //,0xMWB,0xmanual);
    {0x5001, 0x01}, //,0xMWB,0xon);
    {0x5000, 0x86}, //,0xLENC,0xon,,0xBPC,0xon,,0xWPC,0xon);
    {0x301a, 0x71}, //,0xMIPI,0xstream,0xoff);
    {0x301c, 0xf4}, //,0xclock,0xin,0xLP11,0xmode);
    {0x0100, 0x01}, //,0xwake,0xup,0xfrom,0xstandby);
    {0x0100, 0x00},
    {0x3004, 0xd8},
    {0x3006, 0x20}, //00);
    {0x3020, 0x01},
    {0x3501, 0x74},
    {0x3502, 0x60},
    {0x3700, 0x20},
    {0x3702, 0x50},
    {0x3703, 0xcc},
    {0x3704, 0x19},
    {0x3705, 0x32},
    {0x3706, 0x4b},
    {0x3708, 0x84},
    {0x3709, 0x40},
    {0x370a, 0x31},
    {0x3711, 0x0f},
    {0x3712, 0x9c},
    {0x3724, 0x01},
    {0x3725, 0x92},
    {0x3726, 0x01},
    {0x3727, 0xc7},
    {0x3802, 0x01},
    {0x3803, 0x30},
    {0x3806, 0x08},
    {0x3807, 0x67},
    {0x3808, 0x07},
    {0x3809, 0x80},
    {0x380a, 0x04},
    {0x380b, 0x38},
    {0x380c, 0x0d},
    {0x380d, 0xf0},
    {0x380e, 0x07},
    {0x380f, 0x4c},
    {0x3811, 0x10},
    {0x3813, 0x06},
    {0x3814, 0x11},
    {0x3815, 0x11},
    {0x3820, 0x80},
    {0x3821, 0x16},
    {0x3f00, 0x02},
    {0x4005, 0x18},
    {0x4601, 0x01},
    {0x4602, 0x00},
    {0x4837, 0x30}, // 0x1e
    {0x5068, 0x53},
    {0x506a, 0x53},
    {0x0100, 0x01},
    {0x301c, 0xf0}, //,0xrelease,0xclock);
    {0x301a, 0x70}, //,0xMIPI,0xstream,0xon);
    {REG_END, 0x00},
};

static ret_err_t ov8825_write_reg(rk_i2c_bus_device *bus,
                                  uint16_t reg, const uint8_t *data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msgs;
    int ret = 0;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    send_buf[2] = data[0];

    msgs.addr = OV8825_7BIT_ADDRESS;
    msgs.flags = RT_I2C_WR;
    msgs.buf = send_buf;
    msgs.len = 3;
    ret = rt_i2c_transfer(bus, &msgs, 1);

    if (ret == 1)
    {
        OV8825_DEBUG("(%s):s0.0 i2c_bus ok\n");
        return RET_SYS_EOK;
    }
    else
    {
        OV8825_DEBUG("(%s):s0.0 i2c_bus error\n");
        return RET_SYS_ERROR;
    }
}

static ret_err_t ov8825_read_reg(rk_i2c_bus_device *bus,
                                 uint16_t reg, uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    uint8_t send_buf[2];
    uint8_t recv_buf[1];
    ret_err_t ret;

    MACRO_ASSERT(bus != RK_NULL);

    send_buf[0] = ((reg >> 8) & 0xff);
    send_buf[1] = ((reg >> 0) & 0xff);
    msg[0].addr = OV8825_7BIT_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = 2;
    msg[0].buf = send_buf;

    msg[1].addr = OV8825_7BIT_ADDRESS;
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

static void ov8825_write_multiple_reg(struct ov8825_dev *dev,
                                      const struct ov8825_sensor_reg *reg)
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
            ov8825_write_reg(i2c_bus, reg[i].reg_addr, &reg[i].val);
        }
    }

}

static ret_err_t rt_ov8825_detect_sensor(struct ov8825_dev *dev)
{
    MACRO_ASSERT(dev != RK_NULL);

    ret_err_t ret;
    uint8_t pid, ver;
    uint16_t chipid;
    rk_i2c_bus_device *bus = dev->i2c_bus;
    uint32_t cnt = 0;

    OV8825_DEBUG("(%s) enter \n", __FUNCTION__);
    //rk_clk_set_rate(dev->mclk.clk_name, 24000000);
    //rk_clk_enable(dev->mclk.gate);

    /* powerdown: active high */
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_LOW);
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_HIGH);

    /* reset: active low */
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_LOW);
    /* empirical time */
    HAL_DelayMs(2);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_HIGH);

RETRY:
    cnt++;
    ret = ov8825_read_reg(bus, OV8825_REG_ID_H_ADDRESS, &pid);
    if (ret == RET_SYS_EOK)
    {
        ret = ov8825_read_reg(bus, OV8825_REG_ID_L_ADDRESS, &ver);
        chipid = SENSOR_ID(pid, ver);
        OV8825_INFO(dev, "Info: OV8825 pid:0x%x,ver:0x%x!\n", pid, ver);
        if (chipid == OV8825_SENSOR_ID)
        {
            OV8825_INFO(dev, "Info: OV8825 detected successfully !!!\n");
            ret = RET_SYS_EOK;
        }
        else
        {
            OV8825_INFO(dev, "Info: OV8825 detected failed,not match:%d != %d !!!\n",
                        OV8825_SENSOR_ID, chipid);
            ret = -RT_EINVAL;
            if (cnt > CAMERA_BOOT_RETRY_NUM)
                goto EXIT;
            goto RETRY;
        }
    }
    else
    {
        OV8825_INFO(dev, "Warning: OV8825 i2c read failed !!!\n");
        if (cnt > CAMERA_BOOT_RETRY_NUM)
            goto EXIT;
        goto RETRY;
    }
EXIT:
    /* powerdown: active high */
    //HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    //HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_LOW);

    /* reset: active low */
    //HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    //HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_HIGH);
    //rk_clk_disable(dev->mclk.gate);

    OV8825_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static void ov8825_stream_on(struct rk_camera_device *dev)
{
    uint8_t val;
    OV8825_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    //rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

    //rk_clk_set_rate(dev->mclk.clk_name, 24000000);
    //rk_clk_enable(dev->mclk.gate);
    /* powerdown: active high */
    //HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    //HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_LOW);

    /* reset: active low */
    //HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    //HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_HIGH);

    ov8825_write_multiple_reg((struct ov8825_dev *)dev, g_ov8825_init_reg_table);
    //val = 0x0f;
    //ov8825_write_reg(dev->i2c_bus, 0xf2, &val);
    //rt_mutex_release(&dev->mutex_lock);
    //OV8825_DEBUG("(%s) s2,reg[0xf2]:0x%x\n", __FUNCTION__, val);

    OV8825_DEBUG("(%s) exit \n", __FUNCTION__);
}

static void ov8825_stream_off(struct rk_camera_device *dev)
{
    uint8_t val;
    rk_i2c_bus_device *i2c_bus;
    struct ov8825_dev *ov8825 = (struct ov8825_dev *)dev;

    OV8825_DEBUG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);


    i2c_bus = dev->i2c_bus;
    if (i2c_bus)
    {
        //rt_mutex_take(&dev->mutex_lock, RT_WAITING_FOREVER);

        val = 0x0;
        ov8825_write_reg(i2c_bus, 0xf2, &val);//to stop  camera

        //rt_mutex_release(&dev->mutex_lock);
    }
    else
    {
        OV8825_INFO(dev, "Err: not find out i2c bus!\n");
    }

    /* powerdown: active high */
    //HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    //HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_HIGH);

    /* reset: active low */
    //HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    //HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_LOW);
    //rk_clk_disable(dev->mclk.gate);

    OV8825_DEBUG("(%s) exit \n", __FUNCTION__);
}

ret_err_t rk_ov8825_init(struct rk_camera_device *dev)
{
    OV8825_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct ov8825_dev *ov8825;
    //struct rk_camera_device *camera;

    ov8825 = (struct ov8825_dev *)&dev;
    //camera = (struct rk_camera_device *)(&ov8825->parent);
#if RT_USING_OV8825_OPS
    if (ov8825->ops->init)
    {
        return (ov8825->ops->init(ov8825));
    }
#else

    if (ov8825 /* && camera*/)
    {
#if 0
        camera->info.mbus_fmt.width = 800;
        camera->info.mbus_fmt.height = 600;
        camera->info.mbus_fmt.pixelcode = MEDIA_BUS_FMT_UYVY8_2X8;//0x0c uyvy;0x08 vyuy;0x04 yvyu;0x00 yuyv
        camera->info.mbus_fmt.field = 0;
        camera->info.mbus_fmt.colorspace = 0;
        camera->info.mbus_config.mbus_type = CAMERA_MBUS_BT601;
        camera->info.mbus_config.flags = MEDIA_BUS_FLAGS_VSYNC_POLA_LOW |
                                         MEDIA_BUS_FLAGS_HSYNC_POLA_HIGHT;
#endif
        ov8825->i2c_bus = (rk_i2c_bus_device *)rt_device_find(ov8825->i2c_name);

        if (!ov8825->i2c_bus)
        {
            OV8825_DEBUG("Warning:not find i2c source:%s !!!\n",
                         ov8825->i2c_name);
            return RET_SYS_ENOSYS;
        }
        else
        {
            OV8825_DEBUG("(%s):s0 find i2c_bus:%s\n", ov8825->i2c_name);
        }
        //rt_ov8825_detect_sensor(ov8825);
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

    OV8825_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;

}

static ret_err_t rk_ov8825_open(struct rk_camera_device *dev, rt_uint16_t oflag)
{
    OV8825_DEBUG("(%s) enter \n", __FUNCTION__);

    struct ov8825_dev *ov8825;
    struct rk_camera_device *camera;
    ret_err_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    ov8825 = (struct ov8825_dev *)dev;
    camera = (struct rk_camera_device *)(&ov8825->parent);
#if RT_USING_OV8825_OPS
    if (ov8825->ops->open)
    {
        return (ov8825->ops->open(ov8825));
    }
#else

#if 0
    if (ov8825 && camera)
    {
        ret = RET_SYS_EOK;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif

#endif

    OV8825_DEBUG("(%s) exit \n", __FUNCTION__);

    return ret;
}

ret_err_t rk_ov8825_close(struct rk_camera_device *dev)
{
    //OV8825_INFO(dev, "(%s) enter \n", __FUNCTION__);

    struct ov8825_dev *ov8825;
    //struct rk_camera_device *camera;
    uint8_t ret;

    MACRO_ASSERT(dev != RK_NULL);

    ov8825 = (struct ov8825_dev *)dev;
    //camera = (struct rk_camera_device *)(&ov8825->parent);
#if RT_USING_OV8825_OPS
    if (ov8825->ops->close)
    {
        return (ov8825->ops->close(ov8825))
    }
#else
#if 0
    if (ov8825 && camera)
    {
        ret = RET_SYS_EOK;
    }
    else
    {
        ret = RET_SYS_ENOSYS;
    }
#endif
#endif

    //OV8825_INFO(dev, "(%s) exit \n", __FUNCTION__);
    return ret;

}

ret_err_t rk_ov8825_control(struct rk_camera_device *dev,
                            int cmd,
                            void *args)
{
    OV8825_DEBUG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct ov8825_dev *ov8825;

    MACRO_ASSERT(dev != RK_NULL);
    ov8825 = (struct ov8825_dev *)dev;
#if RT_USING_OV8825_OPS
    return (ov8825->ops->control(ov8825, cmd, args));
#else
    switch (cmd)
    {

    case RK_DEVICE_CTRL_DEVICE_INIT:
    {
        ret = rk_ov8825_init(dev);
    }
    break;

    case RK_DEVICE_CTRL_CAMERA_STREAM_ON:
    {
        ov8825_stream_on(dev);
    }
    break;
    case RK_DEVICE_CTRL_CAMERA_STREAM_OFF:
    {
        ov8825_stream_off(dev);
    }
    break;
    /*
        case RK_DEVICE_CTRL_CAMERA_GET_FORMAT:
        {

        }
        break;
    */
    default:
        break;
    }
#endif
    OV8825_DEBUG("(%s) exit \n", __FUNCTION__);
    return ret;
}

#if 0
const static struct rk_camera_ops rk_ov8825_ops =
{
    rk_ov8825_init,
    rk_ov8825_open,
    rk_ov8825_close,
    rk_ov8825_control,
};
#endif

struct rk_camera_ops rk_ov8825_ops =
{
    rk_ov8825_init,
    rk_ov8825_open,
    NULL,
    rk_ov8825_control
};


int rk_camera_ov8825_init(void)
{
    ret_err_t ret;

    rk_device *device = &g_ov8825.dev;
    struct ov8825_dev *instance = &g_ov8825;
    struct rk_camera_device *camera = &instance->parent;

    camera->ops = &rk_ov8825_ops;

    rt_strncpy(instance->name, OV8825_DEVICE_NAME, rt_strlen(OV8825_DEVICE_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

#if OV8825_I2C_DEBUG_ENABLE
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        OV8825_INFO(instance, "Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        OV8825_DEBUG("(%s):s0.0 find i2c_bus\n");
    }
#endif

#if 0
    instance->mclk.clk_name = CLK_CIFOUT;
    instance->mclk.gate = get_clk_gate_from_id(CLK_CIFOUT_GATE);
    clk_set_rate(CLK_CIFOUT, 24000000);
    clk_enable(instance->mclk.gate);
    OV8825_INFO(instance, "enable cif clk\n");
    clk_disable(instance->mclk.gate);

    if (!instance->mclk.gate)
    {
        OV8825_INFO(instance, "Warning:not find mclk source !!!\n");
        ret = RET_SYS_ENOSYS;
        goto ERR;
    }
    else
    {
        OV8825_INFO(instance, "Info:get the clk gate id:0x%x\n",
                    instance->mclk.gate->gate_id);
    }
#endif

#if 0//OV8825_PIN_CTRL_ENABLE
    HAL_PINCTRL_SetIOMUX(CAMERA_PWDN_GPIO_BANK,
                         CAMERA_PWDN_GPIO_PIN,
                         PIN_CONFIG_MUX_FUNC0);

    HAL_PINCTRL_SetIOMUX(CAMERA_RST_GPIO_BANK,
                         CAMERA_RST_GPIO_PIN,
                         PIN_CONFIG_MUX_FUNC0);
    /* powerdown: active high */
    HAL_GPIO_SetPinDirection(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_PWDN_GPIO_GROUP, CAMERA_PWDN_GPIO_PIN, GPIO_HIGH);

    /* reset: active low */
    HAL_GPIO_SetPinDirection(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(CAMERA_RST_GPIO_GROUP, CAMERA_RST_GPIO_PIN, GPIO_LOW);

#endif

    ret = rt_ov8825_detect_sensor(instance);
    if (ret != RET_SYS_EOK)
        goto ERR;

    //rt_mutex_init(&instance->mutex_lock, "OV8825_mutex", RT_IPC_FLAG_FIFO);
    //MACRO_ASSERT(rt_object_get_type(&instance->mutex_lock.parent.parent) == RT_Object_Class_Mutex);
    camera->i2c_bus = instance->i2c_bus;
    rt_strncpy(camera->name, instance->name, rt_strlen(OV8825_DEVICE_NAME));
    rk_camera_register(camera, camera->name, instance);

    OV8825_DEBUG("(%s) exit \n", __FUNCTION__);
    return 0;
ERR:
    OV8825_DEBUG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

void ov8825_detect(void)
{
    struct ov8825_dev *instance = &g_ov8825;

    OV8825_DEBUG("start to detect OV8825 for testing \n");
    OV8825_DEBUG("dev name:%s\n", instance->name);
    OV8825_DEBUG("dev i2c_bus:%s\n", instance->i2c_name);
    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        OV8825_DEBUG("Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        return;
    }
    else
    {
        OV8825_DEBUG("(%s):s0 find i2c_bus:%s\n", instance->i2c_name);
    }

#if 0
    if (!instance->mclk.gate)
    {
        OV8825_DEBUG("(%s):s1 not find clk gate id:%s\n", instance->mclk.gate->gate_id);
        return;
    }
    else
    {
        OV8825_DEBUG("clk:gate_id:%d, rate:%d !!!\n",
                     instance->mclk.gate->gate_id, clk_get_rate(instance->mclk.clk_name));
    }
#endif
    //rt_ov8825_detect_sensor(instance);
}
#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_camera_ov8825_init);
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(ov8825_detect, check g_ov8825 is available or not);
#endif
#endif

#endif

