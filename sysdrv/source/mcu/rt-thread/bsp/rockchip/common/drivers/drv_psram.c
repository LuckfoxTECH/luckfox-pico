/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_psram.c
  * @version V1.0
  * @brief   psram interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-18     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup PSRAM
 *  @{
 */

/** @defgroup PSRAM_How_To_Use How To Use
 *  @{

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "drv_psram.h"

#ifdef RT_USING_QPIPSRAM

/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

struct psram_pm_ops
{
    rt_err_t (*suspend)(void);
    rt_err_t (*resume)(void);
};

/********************* Private Variable Definition ***************************/

struct rk_psram_device s_psram_dev[RK_PSRAM_MAX_DEV];

/********************* Private Function Definition ****************************/

/** @defgroup PSRAM_Public_Functions Public Functions
 *  @{
 */

rt_err_t rk_psram_suspend(void)
{
    struct rk_psram_device *dev = &s_psram_dev[0];

    if (dev->state != RK_PSRAM_STATE_NODEV)
    {
        return dev->ops->suspend(dev);
    }
    else
    {
        return RT_EOK;
    }
}

rt_err_t rk_psram_resume(void)
{
    struct rk_psram_device *dev = &s_psram_dev[0];

    if (dev->state != RK_PSRAM_STATE_NODEV)
    {
        return dev->ops->resume(dev);
    }
    else
    {
        return RT_EOK;
    }
}

rt_size_t rk_psram_read(struct rk_psram_device *dev, rt_off_t pos, rt_uint8_t *data, rt_size_t size)
{
    if (dev)
        return dev->ops->read(dev, pos, data, size);
    else
        return 0;
}

rt_size_t rk_psram_write(struct rk_psram_device *dev, rt_off_t pos, const rt_uint8_t *data, rt_size_t size)
{
    if (dev)
        return dev->ops->write(dev, pos, data, size);
    else
        return 0;
}

rt_err_t rk_psram_register(struct rk_psram_driver_ops *ops, rt_uint32_t size, char *name, void *priv)
{
    rt_err_t ret = -RT_ERROR;
    struct rk_psram_device *dev = RT_NULL;

    for (int32_t i = 0; i < RK_PSRAM_MAX_DEV; i++)
    {
        if (s_psram_dev[i].state == RK_PSRAM_STATE_NODEV)
        {
            dev = &s_psram_dev[i];
            break;
        }
    }

    if (!dev)
        return -RT_EFULL;

    dev->parent.type = RT_Device_Class_Char;
    dev->size = size;
    dev->ops = ops;
    ret = rt_device_register(&dev->parent, name, RT_DEVICE_FLAG_STANDALONE);
    if (ret != RT_EOK)
    {
        return ret;
    }
    dev->state = RK_PSRAM_STATE_BUSY;
    dev->priv = priv;

    return ret;
}

/** @} */  // PSRAM_Public_Function

#endif

/** @} */  // PSRAM

/** @} */  // RKBSP_Common_Driver
