/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @brief   image signal processing (ISP) abstract device
  *
  ******************************************************************************
  */
#include "vcm.h"

#if defined(RT_USING_VCM)

static rt_err_t rk_vcm_control(struct rt_device *dev,
                               int cmd,
                               void *args)
{
    rt_err_t ret;
    struct rk_vcm_dev *vcm;

    RT_ASSERT(dev != RT_NULL);
    vcm = (struct rk_vcm_dev *)dev;
    ret = (vcm->ops->control(vcm, cmd, args));

    return ret;
}

static rt_err_t rk_vcm_rx_indicate(struct rt_device *dev, rt_size_t size)
{
    struct rk_vcm_dev *vcm;

    RT_ASSERT(dev != RT_NULL);

    vcm = (struct rk_vcm_dev *)dev;
    if (vcm->ops->rx_indicate)
    {
        return vcm->ops->rx_indicate(vcm, RT_NULL);
    }

    return RT_ENOSYS;
}

static rt_err_t rk_vcm_init(struct rt_device *dev)
{
    struct rk_vcm_dev *vcm;

    RT_ASSERT(dev != RT_NULL);

    vcm = (struct rk_vcm_dev *)dev;
    if (vcm->ops->init)
    {
        return (vcm->ops->init(vcm));
    }
    else
    {
        rt_kprintf("in rk_vcm_init can not ld init func\n");
    }

    return RT_ENOSYS;
}

const static struct rt_device_ops rk_vcm_dev_ops =
{
    rk_vcm_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rk_vcm_control
};

rt_err_t rk_vcm_controller_register(struct rk_vcm_dev *vcm,
                                    const char *name,
                                    void *data)
{
    unsigned int ret;
    struct rt_device *device;

    RT_ASSERT(vcm != RT_NULL);

    device = &(vcm->parent);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &rk_vcm_dev_ops;
#else
    device->init        = rk_vcm_init;
    device->open        = RT_NULL;
    device->close       = RT_NULL;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rk_vcm_control;
#endif
    device->user_data   = data;

    ret = rt_device_register(device, name, RT_DEVICE_FLAG_RDWR);

    return ret;
}

#endif
