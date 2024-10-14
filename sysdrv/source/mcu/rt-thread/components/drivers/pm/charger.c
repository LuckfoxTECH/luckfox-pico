/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-12-26     Joseph Chen     first implementation
 */

#include <drivers/charger.h>

/* RT-Thread Device Interface */

/*
 * This function initializes charger
 */
static rt_err_t rt_charger_init(struct rt_device *dev)
{
    rt_charger_t *charger;

    RT_ASSERT(dev != RT_NULL);
    charger = (rt_charger_t *)dev;
    if (charger->ops->init)
    {
        return (charger->ops->init(charger));
    }

    return (-RT_ENOSYS);
}

static rt_err_t rt_charger_control(struct rt_device *dev, int cmd, void *args)
{
    rt_charger_t *charger;

    RT_ASSERT(dev != RT_NULL);
    charger = (rt_charger_t *)dev;

    return (charger->ops->control(charger, cmd, args));
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops charger_ops =
{
    rt_charger_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rt_charger_control,
};
#endif

/**
 * This function register a charger device
 */
rt_err_t rt_charger_register(rt_charger_t *charger, const char *name,
                             rt_uint32_t flag)
{
    struct rt_device *device;
    RT_ASSERT(charger != RT_NULL);

    device = &(charger->parent);

    device->type        = RT_Device_Class_PM;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &charger_ops;
#else
    device->init        = rt_charger_init;
    device->open        = RT_NULL;
    device->close       = RT_NULL;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rt_charger_control;
#endif
    device->user_data   = RT_NULL;

    return rt_device_register(device, name, flag);
}
