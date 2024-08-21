/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @brief   image signal processing (ISP) abstract device
  *
  ******************************************************************************
  */
#include "isp.h"

#if defined(RT_USING_ISP)

#undef ENABLE_ISP_DEBUG
#ifdef ENABLE_ISP_DEBUG
#define rk_isp2_function_enter()           rt_kprintf("[isp2]:(%s) enter \n",__FUNCTION__)
#define rk_isp2_function_exit()            rt_kprintf("[isp2]:(%s) exit \n",__FUNCTION__)
#else
#define rk_isp2_function_enter()
#define rk_isp2_function_exit()
#endif

#if ISP_WORK_QUEUE_ENABLE
void rk_isp_workqueue_fun(struct rt_work *work, void *work_data)
{
    rk_isp2_function_enter();

    struct rk_isp_work *isp_work = (struct rk_isp_work *)work_data;
    isp_work->fun(isp_work->parameter);

    rk_isp2_function_exit();
}
#endif

static rt_err_t rk_isp_control(struct rt_device *dev,
                               int cmd,
                               void *args)
{
    rt_err_t ret;
    struct rk_isp_dev *isp;

    rk_isp2_function_enter();

    RT_ASSERT(dev != RT_NULL);
    isp = (struct rk_isp_dev *)dev;
    ret = (isp->ops->control(isp, cmd, args));

    rk_isp2_function_exit();

    return ret;
}

static rt_err_t rk_isp_rx_indicate(struct rt_device *dev, rt_size_t size)
{
    struct rk_isp_dev *isp;

    rk_isp2_function_enter();

    RT_ASSERT(dev != RT_NULL);

    isp = (struct rk_isp_dev *)dev;
    if (isp->ops->rx_indicate)
    {
        return isp->ops->rx_indicate(isp, RT_NULL);
    }

    rk_isp2_function_exit();

    return RT_ENOSYS;
}

static rt_err_t rk_isp_init(struct rt_device *dev)
{
    struct rk_isp_dev *isp;

    rk_isp2_function_enter();
    RT_ASSERT(dev != RT_NULL);

    isp = (struct rk_isp_dev *)dev;
    if (isp->ops->init)
    {
        return (isp->ops->init(isp));
    }
    else
    {
        rt_kprintf("in rk_isp_init can not ld init func\n");
    }

    rk_isp2_function_exit();
    return RT_ENOSYS;
}

static rt_err_t rk_isp_open(struct rt_device *dev, rt_uint16_t oflag)
{
    struct rk_isp_dev *isp;

    rk_isp2_function_enter();
    RT_ASSERT(dev != RT_NULL)

    isp = (struct rk_isp_dev *)dev;
    if (isp->ops->open)
    {
        return (isp->ops->open(isp, RT_DEVICE_OFLAG_RDWR));
    }

    rk_isp2_function_exit();
    return RT_ENOSYS;
}

static rt_err_t rk_isp_close(struct rt_device *dev)
{
    struct rk_isp_dev *isp;

    rk_isp2_function_enter();

    RT_ASSERT(dev != RT_NULL);

    isp = (struct rk_isp_dev *)dev;
    if (isp->ops->close)
    {
        return isp->ops->close(isp);
    }

    rk_isp2_function_exit();

    return RT_ENOSYS;
}

const static struct rt_device_ops rk_isp_dev_ops =
{
    rk_isp_init,
    rk_isp_open,
    rk_isp_close,
    RT_NULL,
    RT_NULL,
    rk_isp_control
};

rt_err_t rk_isp_controller_register(struct rk_isp_dev *isp,
                                    const char *name,
                                    void *data)
{
    unsigned int ret;
    struct rt_device *device;

    rk_isp2_function_enter();

    RT_ASSERT(isp != RT_NULL);

    device = &(isp->parent);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &rk_isp_dev_ops;
#else
    device->init        = rk_isp_init;
    device->open        = rk_isp_open;
    device->close       = rk_isp_close;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rk_isp_control;
#endif
    device->user_data   = data;

    ret = rt_device_register(device, name, RT_DEVICE_FLAG_RDWR);

    rk_isp2_function_exit();

    return ret;
}

#endif
