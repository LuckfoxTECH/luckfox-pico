/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dsp.c
  * @author  Huaping Liao
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   general dsp framework driver for pisces
  *
  ******************************************************************************
  */

#include "dsp.h"

#ifdef RT_USING_DSP

static rt_err_t rt_dsp_open(struct rt_device *dev, rt_uint16_t oflag)
{
    struct rt_dsp_device *dsp;

    RT_ASSERT(dev != RT_NULL);
    dsp = (struct rt_dsp_device *)dev;

    if (dsp->ops->open(dsp, oflag) != RT_EOK)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t rt_dsp_close(struct rt_device *dev)
{
    struct rt_dsp_device *dsp;

    RT_ASSERT(dev != RT_NULL);
    dsp = (struct rt_dsp_device *)dev;

    if (dsp->ops->close(dsp) != RT_EOK)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t rt_dsp_control(struct rt_device *dev,
                               int cmd,
                               void *args)
{
    struct rt_dsp_device *dsp;
    rt_err_t result = RT_EOK;

    RT_ASSERT(dev != RT_NULL);
    dsp = (struct rt_dsp_device *)dev;

    switch (cmd)
    {
    case RT_DSP_CTL_ON:
        if (dsp->ops->on)
            result = dsp->ops->on(dsp);
        break;
    case RT_DSP_CTL_OFF:
        if (dsp->ops->off)
            result = dsp->ops->off(dsp);
        break;
    case RT_DSP_CTL_SUSPEND:
        if (dsp->ops->suspend)
            result = dsp->ops->suspend(dsp);
        break;
    case RT_DSP_CTL_RESUME:
        if (dsp->ops->resume)
            result = dsp->ops->resume(dsp);
        break;
    case RT_DSP_CTL_SHUTDOWN:
        if (dsp->ops->shutdown)
            result = dsp->ops->shutdown(dsp);
        break;
    case RT_DSP_CTL_WAKEUP:
        if (dsp->ops->wakeup)
            result = dsp->ops->wakeup(dsp);
        break;
    default:
        if (dsp->ops->control)
            result = dsp->ops->control(dsp, cmd, args);
        break;
    }

    return result;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops rt_dsp_ops =
{
    RT_NULL,
    rt_dsp_open,
    rt_dsp_close,
    RT_NULL,
    RT_NULL,
    rt_dsp_control,
};
#endif

/**
 * This function register a dsp device
 */
rt_err_t rt_hw_dsp_register(struct rt_dsp_device *dsp,
                            const char *name,
                            rt_uint32_t flag,
                            void *data)
{
    rt_uint32_t ret;
    struct rt_device *device;

    RT_ASSERT(dsp != RT_NULL);

    device = &(dsp->parent);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &rt_dsp_ops;
#else
    device->init        = RT_NULL;
    device->open        = rt_dsp_open;
    device->close       = rt_dsp_close;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rt_dsp_control;
#endif
    device->user_data   = data;

    ret = rt_device_register(device, name, flag);

    return ret;
}

#endif
