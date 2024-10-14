/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    vicap.c
  * @version V0.0.1
  * @brief   video capture device abstract for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

#if defined(__RT_THREAD__)
#include "vicap.h"
#elif defined(__RK_OS__)
#include "driver/vicap.h"
#endif

#ifdef VICAP_MODULE_COMPILED

#define VICAP_DEBUG 0
#if VICAP_DEBUG
#include <stdio.h>
#define VICAP_DBG(...) rk_kprintf("[VICAP]:");rk_kprintf(__VA_ARGS__)
#else
#define VICAP_DBG(...)
#endif

#if defined(__RT_THREAD__)

#if VICAP_WORK_QUEUE_ENABLE
void rk_vicap_workqueue_fun(struct rt_work *work, void *work_data)
{
    VICAP_DBG("(%s) enter \n", __FUNCTION__);
    struct rk_vicap_work *vicap_work = (struct rk_vicap_work *)work_data;
    vicap_work->fun(vicap_work->parameter);
    /*if work is allocate by malloc need free */
    /*rt_free(cif_work);*/
    VICAP_DBG("(%s) exit \n", __FUNCTION__);
}
#endif

static ret_err_t rk_rtthread_vicap_control(rk_device *dev,
        dt_cmd_t cmd,
        void *args)
{
    struct rk_vicap_device *vicap;
    ret_err_t ret;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);
    vicap = (struct rk_vicap_device *)dev;
    ret = (vicap->ops->control(vicap, cmd, args));

    VICAP_DBG("(%s) exit \n", __FUNCTION__);
    return ret;
}

static ret_err_t rk_rtthread_vicap_rx_indicate(rk_device *dev, rt_size_t size)
{
    struct rk_vicap_device *vicap;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);

    vicap = (struct rk_vicap_device *)dev;
    if (vicap->ops->rx_indicate)
    {
        return vicap->ops->rx_indicate(vicap, RK_NULL);
    }

    VICAP_DBG("(%s) exit \n", __FUNCTION__);
    return RET_SYS_ENOSYS;

}

static ret_err_t rk_rtthread_vicap_init(rk_device *dev)
{

    struct rk_vicap_device *vicap;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);

    vicap = (struct rk_vicap_device *)dev;
    if (vicap->ops->init)
    {
        return (vicap->ops->init(vicap));
    }

    VICAP_DBG("(%s) exit \n", __FUNCTION__);
    return RET_SYS_ENOSYS;
}

static ret_err_t rk_rtthread_vicap_open(rk_device *dev, rt_uint16_t oflag)
{
    struct rk_vicap_device *vicap;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL)

    vicap = (struct rk_vicap_device *)dev;
    if (vicap->ops->open)
    {
        return (vicap->ops->open(vicap, RT_DEVICE_OFLAG_RDWR));
    }

    VICAP_DBG("(%s) exit \n", __FUNCTION__);
    return RET_SYS_ENOSYS;
}

static ret_err_t rk_rtthread_vicap_close(rk_device *dev)
{
    struct rk_vicap_device *vicap;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    vicap = (struct rk_vicap_device *)dev;
    if (vicap->ops->close)
    {
        return vicap->ops->close(vicap);
    }

    VICAP_DBG("(%s) exit \n", __FUNCTION__);
    return RET_SYS_ENOSYS;
}

const static struct rt_device_ops rk_vicap_dev_ops =
{
    rk_rtthread_vicap_init,
    rk_rtthread_vicap_open,
    rk_rtthread_vicap_close,
    RK_NULL,
    RK_NULL,
    rk_rtthread_vicap_control
};

ret_err_t rk_rtthread_vicap_register(struct rk_vicap_device *vicap,
                                     const char *name,
                                     void *data)
{
    uint32_t ret;
    rk_device *device;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(vicap != RK_NULL);

    device = &(vicap->parent);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RK_NULL;
    device->tx_complete = RK_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &rk_vicap_dev_ops;
#else
    device->init        = rk_rtthread_vicap_init;
    device->open        = rk_rtthread_vicap_open;
    device->close       = rk_rtthread_vicap_close;
    device->read        = RK_NULL;
    device->write       = RK_NULL;
    device->control     = rk_rtthread_vicap_control;
#endif
    device->user_data   = data;

    ret = rt_device_register(device, name, RT_DEVICE_FLAG_RDWR);
    VICAP_DBG("(%s) exit \n", __FUNCTION__);
    return ret;

}

#elif defined(__RK_OS__)

static ret_err_t rk_rkos_device_open(HDC dev, uint32 oflag)
{
    struct rk_vicap_device *vicap;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    vicap = (struct rk_vicap_device *)dev;
    if (vicap->ops->open)
    {
        return (vicap->ops->open(vicap, oflag));
    }

    VICAP_DBG("(%s) exit \n", __FUNCTION__);

    return RET_SYS_ENOSYS;

}

static ret_err_t rk_rkos_device_close(HDC dev)
{
    struct rk_vicap_device *vicap;
    ret_err_t ret;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);

    MACRO_ASSERT(dev != RK_NULL);

    vicap = (struct rk_vicap_device *)dev;
    ret = (vicap->ops->close(vicap));

    VICAP_DBG("(%s) exit \n", __FUNCTION__);

    return ret;

}

static ret_err_t rk_rkos_vicap_control(HDC dev,
                                       dt_cmd_t cmd,
                                       void *args)
{
    struct rk_vicap_device *vicap;
    ret_err_t ret;

    VICAP_DBG("(%s) enter \n", __FUNCTION__);
    MACRO_ASSERT(dev != RK_NULL);
    vicap = (struct rk_vicap_device *)dev;
    ret = (vicap->ops->control(vicap, cmd, args));

    VICAP_DBG("(%s) exit \n", __FUNCTION__);
    return ret;
}

HDC rk_rkos_vicap_create(uint8_t dev_id, void *arg)
{
    struct rk_vicap_device *vicapdev = (struct rk_vicap_device *)arg;
    rk_device *dev = &vicapdev->parent;

    dev->dev_class_id = DEV_CLASS_VICAP;
    dev->dev_object_id = dev_id;
    dev->suspend = RK_NULL;
    dev->resume = RK_NULL;
    dev->open = rk_rkos_device_open;
    dev->close = rk_rkos_device_close;
    dev->control = rk_rkos_vicap_control;
    dev->priv_data = arg;

    return dev;

}

ret_err_t rk_rkos_vicap_delete(uint8_t dev_id, void *arg)
{
    struct rk_vicap_device *vicapdev = (struct rk_vicap_device *)arg;
    rk_device *dev = &vicapdev->parent;

    dev->control = RK_NULL;
    dev->priv_data = arg;

#ifdef CONFIG_FWANALYSIS_SEGMENT
    FW_RemoveSegment(SEGMENT_ID_CIF_DEV);
#endif

    return RET_SYS_EOK;

}

ret_err_t rk_rkos_vicap_register(struct rk_vicap_device *vicapdev,
                                 uint8_t dev_object_id,
                                 void *data)
{
    ret_err_t ret;
    rk_device *dev = RK_NULL;

    ret = rkdev_create(DEV_CLASS_VICAP, dev_object_id, data);
    if (ret != RET_SYS_EOK)
    {
        rk_kprintf("vicap device create failure\n");
        return RET_SYS_ERROR;
    }

    dev = rkdev_find(DEV_CLASS_VICAP, dev_object_id);
    if (dev)
    {
        if ((uint32_t)dev != (uint32_t)vicapdev)
        {
            rk_kprintf("the create vicap device is not matched!\n");
            return RET_SYS_ERROR;
        }
    }

    rk_kprintf("vicap device create success\n");

    return RET_SYS_EOK;

}

#endif

ret_err_t rk_vicap_controller_register(struct rk_vicap_device *vicap,
                                       const char *name,
                                       void *data)
{
#if defined(__RT_THREAD__)
    return rk_rtthread_vicap_register(vicap, name, data);
#elif defined(__RK_OS__)
    uint8_t dev_id = 0;
    char *tmp;

    tmp = strstr(name, "_");
    tmp += 1;
    dev_id = (uint8_t)atoi(tmp);

    return rk_rkos_vicap_register(vicap, dev_id, data);
#endif

}

#endif
