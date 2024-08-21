/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-12-26     Joseph Chen     first implementation
 */

#include <drivers/fuel_gauge.h>

/* RT-Thread Device Interface */

/*
 * This function initializes fuel gauge
 */
static rt_err_t rt_fg_init(struct rt_device *dev)
{
    rt_fg_t *fg;

    RT_ASSERT(dev != RT_NULL);
    fg = (rt_fg_t *)dev;
    if (fg->ops->init)
    {
        return (fg->ops->init(fg));
    }

    return (-RT_ENOSYS);
}

static rt_err_t rt_fg_control(struct rt_device *dev, int cmd, void *args)
{
    rt_fg_t *fg;

    RT_ASSERT(dev != RT_NULL);
    fg = (rt_fg_t *)dev;

    return (fg->ops->control(fg, cmd, args));
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops fg_ops =
{
    rt_fg_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rt_fg_control,
};
#endif

/**
 * This function register a fuel gauge device
 */
rt_err_t rt_fg_register(rt_fg_t *fg, const char *name, rt_uint32_t flag)
{
    struct rt_device *device;
    RT_ASSERT(fg != RT_NULL);

    device = &(fg->parent);

    device->type        = RT_Device_Class_PM;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &fg_ops;
#else
    device->init        = rt_fg_init;
    device->open        = RT_NULL;
    device->close       = RT_NULL;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rt_fg_control;
#endif
    device->user_data   = RT_NULL;


    return rt_device_register(device, name, flag);
}

static int fg_init_auto(void)
{
    rt_device_t device;

    device = rt_device_find("fuel_gauge");
    if (device == RT_NULL)
    {
        return -RT_ERROR;
    }

    return rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
}

INIT_DEVICE_EXPORT(fg_init_auto);
