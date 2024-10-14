/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_spi2apb.c
  * @author  David Wu
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   spi2apb driver
  *
  ******************************************************************************
  */

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"
#include "spi2apb.h"

#if defined(RT_USING_SPI2APB)

#define SPI2APB_DEBUG 0

#if SPI2APB_DEBUG
#define spi2apb_dbg(dev, fmt, ...) \
do { \
    rt_kprintf("%s:", ((struct rt_device *)dev)->parent.name); \
    rt_kprintf(fmt, ##__VA_ARGS__); \
} while(0)
#else
#define spi2apb_dbg(dev, fmt, ...) \
do { \
} while(0)
#endif

struct rockchip_spi2apb
{
    struct rt_spi2apb_device device;
    struct SPI2APB_REG *base;
    rt_uint8_t irq;
    spi2apb_callback callback;
    uint32_t regs_backup[3];
};

static struct rockchip_spi2apb rk_spi2apb =
{
    .base = SPI2APB,
    .irq = SPISLV0_IRQn,
    .callback = NULL,
};

static void rockchip_spi2apb_irq(int irq, void *param)
{
    struct rockchip_spi2apb *spi2apb = &rk_spi2apb;

    /* enter interrupt */
    rt_interrupt_enter();

    HAL_SPI2APB_CleanIrq(spi2apb->base);
    if (spi2apb->callback)
        spi2apb->callback(HAL_SPI2APB_ReadReg1(spi2apb->base));

    /* leave interrupt */
    rt_interrupt_leave();
}

static rt_err_t rockchip_spi2apb_init(rt_device_t dev)
{
    struct rt_spi2apb_device *device = (struct rt_spi2apb_device *)dev;
    struct rockchip_spi2apb *spi2apb = device->parent.user_data;

    /* register irq */
    rt_hw_interrupt_install(spi2apb->irq, rockchip_spi2apb_irq, spi2apb,
                            device->parent.parent.name);
    rt_hw_interrupt_umask(spi2apb->irq);

    return RT_EOK;
}

static rt_err_t rockchip_spi2apb_control(rt_device_t dev, int cmd, void *arg)
{
    struct rt_spi2apb_device *device = (struct rt_spi2apb_device *)dev;
    struct rockchip_spi2apb *spi2apb = device->parent.user_data;
    struct rt_spi2apb_configuration *config;
    struct SPI2APB_REG *pReg;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(spi2apb != RT_NULL);
    RT_ASSERT(arg != RT_NULL);

    pReg = spi2apb->base;
    spi2apb_dbg(&device->parent, "rt_spi2apb_control cmd: %d\n", cmd);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_SPI2APB_CONFIGURATION:
        config = (struct rt_spi2apb_configuration *)arg;
        HAL_SPI2APB_Configurate(pReg, config->mode | config->clock_polarity);
        break;
    case RT_DEVICE_CTRL_SPI2APB_QUERY_STATUS:
        *(rt_uint32_t *)arg = HAL_SPI2APB_ReadStatus(pReg);
        break;
    case RT_DEVICE_CTRL_SPI2APB_REGISTER_CB:
        /* Enable irq. */
        spi2apb->callback = (spi2apb_callback)arg;
        rt_hw_interrupt_umask(spi2apb->irq);
        HAL_SPI2APB_UnmaskIrq(pReg, true);
        break;
    case RT_DEVICE_CTRL_SPI2APB_READ_REG0:
        *(rt_uint32_t *)arg = HAL_SPI2APB_ReadReg0(pReg);
        break;
    case RT_DEVICE_CTRL_SPI2APB_READ_REG1:
        *(rt_uint32_t *)arg = HAL_SPI2APB_ReadReg1(pReg);
        break;
    case RT_DEVICE_CTRL_SPI2APB_WRITE_REG2:
        HAL_SPI2APB_WriteReg2(pReg, *((rt_uint32_t *)arg));
        break;
    default:
        ret = RT_EINVAL;
        break;
    }

    return ret;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops rockchip_spi2apb_ops =
{
    rockchip_spi2apb_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rockchip_spi2apb_control,
};
#endif

/**
 * rockchip_spi_suspend - suspend function for SPI Controller
 * @device: spi device structure
 */
static int rockchip_spi2apb_suspend(const struct rt_device *device)
{
    struct rt_spi2apb_device *dev = (struct rt_spi2apb_device *)device;
    struct rockchip_spi2apb *spi2apb = dev->parent.user_data;

    HAL_SPI2APB_Suspend(spi2apb->base, spi2apb->regs_backup);
    return RT_EOK;
}

/**
 * rockchip_spi_resume - resume function for SPI Controller
 * @device: spi device structure
 */
static void rockchip_spi2apb_resume(const struct rt_device *device)
{
    struct rt_spi2apb_device *dev = (struct rt_spi2apb_device *)device;
    struct rockchip_spi2apb *spi2apb = dev->parent.user_data;

    HAL_SPI2APB_Resume(spi2apb->base, spi2apb->regs_backup);
}

static struct rt_device_pm_ops rockchip_spi2apb_pm_ops =
{
    .suspend = rockchip_spi2apb_suspend,
    .resume = rockchip_spi2apb_resume,
};

/**
 * This function register a spi2apb device
 */
rt_err_t rt_hw_spi2apb_register(struct rt_spi2apb_device *spi2apb,
                                const char *name,
                                rt_uint32_t flag,
                                void *data)
{
    struct rt_device *device;

    RT_ASSERT(spi2apb != RT_NULL);

    device = &(spi2apb->parent);

    device->type        = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &rockchip_spi2apb_ops;
#else
    device->init        = rockchip_spi2apb_init;
    device->open        = RT_NULL;
    device->close       = RT_NULL;
    device->read        = RT_NULL;
    device->write       = RT_NULL;
    device->control     = rockchip_spi2apb_control;
#endif
    device->user_data   = data;

    rt_pm_register_device((struct rt_device *)spi2apb, &rockchip_spi2apb_pm_ops);

    return rt_device_register(device, name, flag);
}

int rt_hw_spi2apb_init(void)
{
    return rt_hw_spi2apb_register(&rk_spi2apb.device, "spi2apb",
                                  RT_DEVICE_FLAG_RDWR, &rk_spi2apb);
}

INIT_DEVICE_EXPORT(rt_hw_spi2apb_init);

#endif
