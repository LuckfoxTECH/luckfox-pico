/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_gt9760s.c
  * @version V0.0.1
  * @brief   vcm driver for rk625
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-01-11     ISP Team      first implementation
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
#include "vcm.h"
#elif defined(__RK_OS__)
#include "driver/camera.h"
#endif

#if defined(RT_USING_VCM)
#define VCM_DEBUG 0
#define VCM_ERROR 1
#if VCM_DEBUG
#include <stdio.h>
#define VCM_DBG(...)                \
do {\
rt_kprintf("[CAMERA VCM]:");    \
rt_kprintf(__VA_ARGS__);    \
} while (0)
#define rk_vcm_function_enter()             rt_kprintf("[CAMERA VCM]:(%s) enter \n",__FUNCTION__)
#define rk_vcm_function_exit()              rt_kprintf("[CAMERA VCM]:(%s) exit \n",__FUNCTION__)
#else
#define VCM_DBG(...)
#define rk_vcm_function_enter()
#define rk_vcm_function_enter()
#endif

#if VCM_ERROR
#include <stdio.h>
#define VCM_ERR(...)                \
do {\
rt_kprintf("[CAMERA VCM ERR]:");    \
rt_kprintf(__VA_ARGS__);    \
} while (0)
#else
#define VCM_ERR(...)
#endif

#define GT9760S_SEL_ON_BYTE1            0xEC
#define GT9760S_SEL_ON_BYTE2            0xA3
#define GT9760S_DVO_DLC_BYTE1           0xA1
#define GT9760S_DVO_DLC_BYTE2           0xD
#define GT9760S_T_SRC_BYTE1             0xF2
#define GT9760S_T_SRC_BYTE2             0xF8
#define GT9760S_SEL_OFF_BYTE1           0xDC
#define GT9760S_SEL_OFF_BYTE2           0x51
#define GT9760S_I2C_SALVE_ADDR          0x0C
/*
* Time to move the motor, This is fixed in the DLC specific setting
*/
#define GT9760S_DLC_MOVE_MS             33

/*
 * Focus position values:
 * 65 logical positions ( 0 - 64 )
 * where 64 is the setting for infinity and 0 for macro
 * corresponding to
 * 1024 register settings (0 - 1023)
 * where 0 is the setting for infinity and 1023 for macro
 */
#define MAX_LOG                        64U
#define MAX_REG                        1023U
#define MAX_VCMDRV_CURRENT             120U
#define MAX_VCMDRV_REG                 1023U

#define VCMDRV_DEFAULT_START_CURRENT   0x14
#define VCMDRV_DEFAULT_RATED_CURRENT   0x26
#define VCMDRV_DEFAULT_STEP_MODE       0x0D

/* ======================================================================== */
struct rk_gt9760s_dev
{
    struct rk_vcm_dev parent;
    char name[RK_CAMERA_DEVICE_NAME_SIZE];
    char i2c_name[RK_CAMERA_I2C_NAME_SIZE];
    struct rt_i2c_bus_device *i2c_bus;
    unsigned short current_related_pos;
    unsigned short current_lens_pos;
    unsigned int start_current;
    unsigned int rated_current;
    unsigned int step;
    unsigned int step_mode;
    unsigned int vcm_movefull_t;

    uint32_t start_move_tv;
    uint32_t end_move_tv;
    unsigned long move_ms;
};

static struct rk_gt9760s_dev g_vcm_dev ;
/* ======================================================================== */
static int gt9760s_read_msg(
    struct rt_i2c_bus_device *bus,
    unsigned char *msb, unsigned char *lsb)
{
    int ret = 0;
    struct rt_i2c_msg msgs;
    unsigned char data[2];
    int retries;

    MACRO_ASSERT(bus != RT_NULL);

    for (retries = 0; retries < 5; retries++)
    {
        msgs.addr = GT9760S_I2C_SALVE_ADDR;
        msgs.flags = 1;
        msgs.len = 2;
        msgs.buf = data;

        ret = rt_i2c_transfer(bus, &msgs, 1);
        if (ret == 1)
        {
            VCM_DBG(
                "%s: vcm i2c ok, addr 0x%x, data 0x%x, 0x%x\n",
                __FUNCTION__, msgs.addr, data[0], data[1]);

            *msb = data[0];
            *lsb = data[1];
            return 0;
        }

        VCM_DBG("retrying I2C... %d\n", retries);
        retries++;
        //set_current_state(TASK_UNINTERRUPTIBLE);
        //schedule_timeout(msecs_to_jiffies(20));
    }
    VCM_ERR(
        "%s: i2c write to failed with error %d\n", __FUNCTION__, ret);
    return RT_ERROR;
}

static int gt9760s_write_msg(
    struct rt_i2c_bus_device *bus,
    unsigned char msb, unsigned char lsb)
{
    int ret = 0;
    struct rt_i2c_msg msg;
    unsigned char data[2];
    int retries;

    MACRO_ASSERT(bus != RT_NULL);

    for (retries = 0; retries < 5; retries++)
    {
        msg.addr = GT9760S_I2C_SALVE_ADDR;
        msg.flags = 0;
        msg.len = 2;
        msg.buf = data;
        data[0] = msb;
        data[1] = lsb;

        ret = rt_i2c_transfer(bus, &msg, 1);
        if (ret == 1)
        {
            VCM_DBG(
                "%s: vcm i2c ok, addr 0x%x, data 0x%x, 0x%x\n",
                __FUNCTION__, msg.addr, data[0], data[1]);
            return 0;
        }

        VCM_DBG(
            "retrying I2C... %d\n", retries);
        retries++;
        //set_current_state(TASK_UNINTERRUPTIBLE);
        //schedule_timeout(msecs_to_jiffies(20.0));
    }
    VCM_ERR(
        "%s: i2c write to failed with error %d\n", __FUNCTION__, ret);
    return RT_ERROR;
}

static inline struct rk_gt9760s_dev *rk_get_vcm_dev(struct rk_vcm_dev *dev)
{
    struct rk_gt9760s_dev *vcmdev = dev->parent.user_data;

    return vcmdev;
}

static rt_err_t rk_gt9760s_init(struct rk_vcm_dev *dev)
{
    int ret = 0;
    struct rk_gt9760s_dev *vcm_dev;
    struct rt_i2c_bus_device *i2c_bus;
    unsigned int start_current;
    unsigned int rated_current;
    unsigned int step_mode;
    int current_distance;

    RT_ASSERT(dev != RT_NULL);

    vcm_dev = rk_get_vcm_dev(dev);
    RT_ASSERT(vcm_dev != RT_NULL);


    start_current = VCMDRV_DEFAULT_START_CURRENT;
    rated_current = VCMDRV_DEFAULT_RATED_CURRENT;
    step_mode = VCMDRV_DEFAULT_STEP_MODE;
    current_distance = rated_current - start_current;
    current_distance = current_distance * MAX_VCMDRV_REG / MAX_VCMDRV_CURRENT;
    vcm_dev->step = (current_distance + (MAX_LOG - 1)) / MAX_LOG;
    vcm_dev->start_current = start_current * MAX_VCMDRV_REG / MAX_VCMDRV_CURRENT;
    vcm_dev->rated_current = vcm_dev->start_current + MAX_LOG * vcm_dev->step;
    vcm_dev->step_mode    = step_mode;
    vcm_dev->move_ms      = 0;
    vcm_dev->current_related_pos = MAX_LOG;
    vcm_dev->vcm_movefull_t = GT9760S_DLC_MOVE_MS;
    //gettimeofday(&dev->start_move_tv);
    //gettimeofday(&dev->end_move_tv);

    i2c_bus = vcm_dev->i2c_bus;

    ret = gt9760s_write_msg(i2c_bus, GT9760S_SEL_ON_BYTE1,
                            GT9760S_SEL_ON_BYTE2);
    if (ret)
        goto __err;

    ret = gt9760s_write_msg(i2c_bus, GT9760S_DVO_DLC_BYTE1,
                            GT9760S_DVO_DLC_BYTE2);
    if (ret)
        goto __err;

    ret = gt9760s_write_msg(i2c_bus, GT9760S_T_SRC_BYTE1,
                            GT9760S_T_SRC_BYTE2);
    if (ret)
        goto __err;

    ret = gt9760s_write_msg(i2c_bus, GT9760S_SEL_OFF_BYTE1,
                            GT9760S_SEL_OFF_BYTE2);
    if (ret)
        goto __err;

    return 0;
__err:
    VCM_ERR("failed with error %d\n", ret);
    return -1;
}

static int rt_do_get_gt9760s_pos(
    struct rk_gt9760s_dev *vcm_dev,
    unsigned int *cur_pos)
{
    RT_ASSERT(vcm_dev);
    struct rt_i2c_bus_device *i2c_bus = vcm_dev->i2c_bus;
    int ret;
    unsigned char lsb;
    unsigned char msb;
    unsigned int abs_step;

    ret = gt9760s_read_msg(i2c_bus, &msb, &lsb);
    if (ret)
        goto __err;

    abs_step = (((unsigned int)(msb & 0x3FU)) << 4U) | (((unsigned int)lsb) >> 4U);
    if (abs_step <= vcm_dev->start_current)
        abs_step = MAX_LOG;
    else if ((abs_step > vcm_dev->start_current) && (abs_step <= vcm_dev->rated_current))
        abs_step = (vcm_dev->rated_current - abs_step) / vcm_dev->step;
    else
        abs_step = 0;

    *cur_pos = abs_step;
    VCM_DBG("%s: get position %d\n", __FUNCTION__, *cur_pos);
    return 0;

__err:
    VCM_ERR("%s: failed with error %d\n", __FUNCTION__, ret);
    return ret;
}

static int rt_do_set_gt9760s_pos(
    struct rk_gt9760s_dev *vcm_dev,
    unsigned int dest_pos)
{
    int ret;
    unsigned char lsb;
    unsigned char msb;
    unsigned int position;

    RT_ASSERT(vcm_dev);
    struct rt_i2c_bus_device *i2c_bus = vcm_dev->i2c_bus;

    if (dest_pos > MAX_LOG)
    {
        VCM_ERR(
            "value out of range, must be in [0..%d]\n", MAX_LOG);
        ret = -ERANGE;
        goto __err;
    }
    VCM_DBG("%s: set position %d\n", __FUNCTION__, dest_pos);

    if (dest_pos >= MAX_LOG)
        position = vcm_dev->start_current;
    else
        position = vcm_dev->start_current + (vcm_dev->step * (MAX_LOG - dest_pos));

    if (position > MAX_VCMDRV_REG)
        position = MAX_VCMDRV_REG;

    vcm_dev->current_lens_pos = position;
    vcm_dev->current_related_pos = dest_pos;
    msb = (0x00U | ((vcm_dev->current_lens_pos & 0x3F0U) >> 4U));
    lsb = (((vcm_dev->current_lens_pos & 0x0FU) << 4U) | vcm_dev->step_mode);
    ret = gt9760s_write_msg(i2c_bus, msb, lsb);
    if (ret)
        goto __err;

    return 0;
__err:
    VCM_ERR(
        "%s: failed with error %d\n", __FUNCTION__, ret);
    return ret;
}

ret_err_t rk_gt9760s_control(struct rk_vcm_dev *dev,
                             int cmd,
                             void *args)
{
    VCM_DBG("(%s) enter \n", __FUNCTION__);

    ret_err_t ret = RET_SYS_EOK;
    struct rk_gt9760s_dev *vcm_dev;
    int *data = (int *)args;
    struct vcm_move_res move;
    MACRO_ASSERT(dev != RK_NULL);
    vcm_dev = rk_get_vcm_dev(dev);

    switch (cmd)
    {

    case RK_VCM_DEVICE_CID_INIT:
    {
        ret = rk_gt9760s_init(dev);
    }
    break;

    case RK_GET_VCM_FOCUS_ABSOLUTE:
    {
        ret = rt_do_get_gt9760s_pos(vcm_dev, (unsigned int *)args);
    }
    break;

    case RK_GET_VCM_MOVE_RES:
        move.start_move_tv = vcm_dev->start_move_tv;
        move.end_move_tv = vcm_dev->end_move_tv;
        VCM_DBG("%s: [%x %x]\n", __FUNCTION__, move.start_move_tv, move.end_move_tv);
        if (args != RT_NULL)
        {
            rt_memcpy(args, &move, sizeof(struct vcm_move_res));
        }
        else
            ret = RT_ENOMEM;
        break;
    case RK_SET_VCM_FOCUS_ABSOLUTE:
    {
        rt_do_set_gt9760s_pos(vcm_dev, (unsigned int)data[0]);
        vcm_dev->move_ms = GT9760S_DLC_MOVE_MS;
        vcm_dev->start_move_tv = HAL_GetTick();
        vcm_dev->end_move_tv = vcm_dev->start_move_tv + vcm_dev->move_ms;
    }
    break;
    default:
        break;
    }

    VCM_DBG("(%s) exit \n", __FUNCTION__);
    return ret;
}

const static struct rk_vcm_ops rk_gt9760s_ops =
{
    rk_gt9760s_init,
    RT_NULL,
    RT_NULL,
    rk_gt9760s_control
};

int rk_vcm_gt9760s_init(void)
{
    ret_err_t ret;
    struct rk_vcm_dev *vcm;
    struct rk_gt9760s_dev *instance = &g_vcm_dev;

    VCM_DBG("(%s) in \n", __FUNCTION__);

    vcm = &instance->parent;
    vcm->ops = &rk_gt9760s_ops;

    rt_strncpy(instance->name, GT9760S_DRIVER_NAME, rt_strlen(GT9760S_DRIVER_NAME));
    rt_strncpy(instance->i2c_name, I2C_BUS_NAME, rt_strlen(I2C_BUS_NAME));

    instance->i2c_bus = (rk_i2c_bus_device *)rt_device_find(instance->i2c_name);
    if (!instance->i2c_bus)
    {
        VCM_DBG("Warning:not find i2c source:%s !!!\n", instance->i2c_name);
        ret = RT_ERROR;
        goto ERR;
    }
    else
    {
        VCM_DBG("(%s):s0.0 find i2c_bus\n");
    }

    rk_vcm_controller_register(vcm, instance->name, instance);

    VCM_DBG("(%s) exit \n", __FUNCTION__);
    return 0;
ERR:
    VCM_DBG("(%s) probe failed !!!\n", __FUNCTION__);
    return ret;

}

INIT_DEVICE_EXPORT(rk_vcm_gt9760s_init);
#endif
