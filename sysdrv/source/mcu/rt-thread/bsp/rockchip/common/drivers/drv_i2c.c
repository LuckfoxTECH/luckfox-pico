/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_i2c.c
  * @author  David Wu
  * @version V0.1
  * @date    26-Mar-2019
  * @brief   I2C Driver
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup I2C
 *  @{
 */

/** @defgroup I2C_How_To_Use How To Use
 *  @{

  See more information, click [here](https://www.rt-thread.org/document/site/programming-manual/device/i2c/i2c/)

 @} */

#include <rthw.h>
#include "drv_pm.h"
#include "drv_i2c.h"

#ifdef RT_USING_PM
#include <drivers/pm.h>
#endif

#ifdef RT_USING_I2C

#include "hal_bsp.h"
#include "drv_clock.h"
#include <drivers/i2c.h>

#define RK_I2C_DEBUG 0

#if RK_I2C_DEBUG
#define rk_i2c_dbg(dev, fmt, ...) \
    rt_kprintf("%s: " fmt, ((struct rt_device *)dev)->parent.name, ##__VA_ARGS__)
#else
#define rk_i2c_dbg(dev, fmt, ...) \
do { \
} while(0)
#endif

#define ROCKCHIP_I2C_TIMEOUT (1 * RT_TICK_PER_SECOND)

/* I2C controller driver's private data. */
struct rockchip_i2c
{
    /* I2C bus */
    struct rt_i2c_bus_device bus;
    const char *name;
    uint32_t pmState;

    /* irq handler */
    rt_isr_handler_t irq_handler;

    /* error status */
    int32_t error;

    struct rt_completion done;

    /* HAL */
    struct I2C_HANDLE instance;
    const struct HAL_I2C_DEV *dev;
};

static void rockchip_i2c_irq(struct rockchip_i2c *i2c)
{
    struct I2C_HANDLE *pI2C = &i2c->instance;
    int status;

    /* enter interrupt */
    rt_interrupt_enter();

    status = HAL_I2C_IRQHandler(pI2C);
    if (status != HAL_BUSY)
    {
        i2c->error = status;
        if (i2c->error)
            rk_i2c_dbg(&i2c->bus.parent, "irq handle error: %d.\n",
                       i2c->error);
        rt_completion_done(&i2c->done);
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

static rt_err_t rockchip_i2c_configure(struct rockchip_i2c *i2c, struct rt_i2c_msg *msgs, int num)
{
    struct I2C_HANDLE *pI2C = &i2c->instance;
    uint32_t addr = (msgs[0].addr & 0x7f) << 1;
    struct rt_i2c_msg *message;
    rt_err_t ret = 0;

    if (num >= 2 && msgs[0].len < 4 &&
            !(msgs[0].flags & RT_I2C_RD) && (msgs[1].flags & RT_I2C_RD))
    {
        uint32_t reg_addr = 0;
        int i;

        for (i = 0; i < msgs[0].len; ++i)
        {
            reg_addr |= msgs[0].buf[i] << (i * 8);
            reg_addr |= HAL_I2C_REG_MRXADDR_VALID(i);
        }

        addr |= HAL_I2C_REG_MRXADDR_VALID(0);
        HAL_I2C_ConfigureMode(pI2C, REG_CON_MOD_REGISTER_TX, addr, reg_addr);

        message = &msgs[1];
        ret = 2;
    }
    else
    {
        if (msgs[0].flags & RT_I2C_RD)
        {
            addr |= 1; /* set read bit */
            addr |= HAL_I2C_REG_MRXADDR_VALID(0);

            HAL_I2C_ConfigureMode(pI2C, REG_CON_MOD_REGISTER_TX, addr, 0);
        }
        else
        {
            HAL_I2C_ConfigureMode(pI2C, REG_CON_MOD_TX, 0, 0);
        }

        message = &msgs[0];
        ret = 1;
    }

    HAL_I2C_SetupMsg(pI2C, msgs[0].addr, message->buf, message->len, message->flags);

    return ret;
}

/*
 * Prepare controller for a transaction and call i2c_rockchip_xfer
 */
static rt_size_t rockchip_i2c_xfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[],
                                   rt_uint32_t  num)
{
    struct rockchip_i2c *i2c = bus->priv;
    struct I2C_HANDLE *pI2C = &i2c->instance;
    int i, ret = 0;
    bool use_interrupt = true;
    const struct HAL_I2C_DEV *i2c_dev = i2c->dev;

    pm_runtime_request(i2c_dev->runtimeID);
    i2c->pmState = PM_BUSY;

    clk_enable_by_id(i2c_dev->pclkGateID);
    clk_enable_by_id(i2c_dev->clkGateID);

    if (use_interrupt)
        rt_hw_interrupt_umask(i2c_dev->irqNum);

    for (i = 0; i < num; i += ret)
    {
        uint32_t tmo = bus->timeout;
        bool last = false;

        ret = rockchip_i2c_configure(i2c, msgs + i, num - i);
        if (ret <= 0)
        {
            rt_kprintf("i2c_setup() failed\n");
            break;
        }

        /* Is it the last message? */
        if (i + ret >= num)
            last = true;

        if (use_interrupt)
        {
            int timeout = 0;

            HAL_I2C_Transfer(pI2C, I2C_IT, last);
            timeout = rt_completion_wait(&i2c->done, rt_tick_from_millisecond(tmo));
            if (timeout)
                tmo = 0;
        }
        else
        {
            HAL_I2C_Transfer(pI2C, I2C_POLL, last);
            do
            {
                rt_thread_delay(1);
                i2c->error = HAL_I2C_IRQHandler(pI2C);
                if (i2c->error != HAL_BUSY)
                    break;
            }
            while (tmo--);
        }

        /* timeout to force stop */
        if (tmo <= 0)
        {
            HAL_I2C_ForceStop(pI2C);
            ret = -RT_ETIMEOUT;
            break;
        }

        if (i2c->error)
        {
            /* translate the error from hal to rt */
            ret = i2c->error;
            break;
        }
    }

    HAL_I2C_Close(pI2C);
    if (use_interrupt)
        rt_hw_interrupt_mask(i2c_dev->irqNum);

    clk_disable_by_id(i2c_dev->pclkGateID);
    clk_disable_by_id(i2c_dev->clkGateID);
    i2c->pmState = PM_IDLE;

    pm_runtime_release(i2c_dev->runtimeID);

    return ret < 0 ? ret : num;
}

static struct rt_i2c_bus_device_ops bus_ops =
{
    .master_xfer = rockchip_i2c_xfer,
};

#if defined(RT_USING_PM)
static int rt_i2c_pm_suspend(const struct rt_device *device)
{
    struct rockchip_i2c *i2c = device->user_data;

    if (i2c->pmState == PM_IDLE)
    {
        i2c->pmState = PM_SUSPEND;
        return RT_EOK;
    }
    else
        return RT_ERROR;
}

static void rt_i2c_pm_resume(const struct rt_device *device)
{
    struct rockchip_i2c *i2c = device->user_data;
    const struct HAL_I2C_DEV *i2c_dev = i2c->dev;
    rt_uint32_t freq;

    freq = HAL_CRU_ClkGetFreq(i2c_dev->clkID);
    clk_enable_by_id(i2c_dev->pclkGateID);
    HAL_I2C_AdaptDIV(&i2c->instance, freq);
    clk_disable_by_id(i2c_dev->pclkGateID);
    i2c->pmState = PM_IDLE;
}

static struct rt_device_pm_ops rk_i2c_pm_ops =
{
    .suspend = rt_i2c_pm_suspend,
    .resume = rt_i2c_pm_resume,
};
#endif

/**
 * rt_rockchip_i2c_probe - probe function for I2C Controller
 * @i2c: I2C private structure which contains I2C specific data
 * @speed: I2C bus speed
 */
static rt_err_t rt_rockchip_i2c_probe(struct rockchip_i2c *i2c, eI2C_BusSpeed speed)
{
    rt_uint32_t freq;
    const struct HAL_I2C_DEV *i2c_dev = i2c->dev;

    /* register irq */
    rt_hw_interrupt_install(i2c_dev->irqNum, i2c->irq_handler, i2c, i2c->name);
    rt_hw_interrupt_mask(i2c_dev->irqNum);

    /* Get clock rate here. */
    freq = clk_get_rate(i2c_dev->clkID);
    clk_enable_by_id(i2c_dev->pclkGateID);
    HAL_I2C_Init(&i2c->instance, i2c_dev->pReg, freq, speed);
    clk_disable_by_id(i2c_dev->pclkGateID);

    i2c->pmState = PM_IDLE;
    i2c->bus.parent.user_data = i2c;
#if defined(RT_USING_PM)
    rt_pm_register_device(&i2c->bus.parent, &rk_i2c_pm_ops);
#endif
    rt_completion_init(&i2c->done);

    return rt_i2c_bus_device_register(&i2c->bus, i2c->name);
}

RT_WEAK const struct rockchip_i2c_config rockchip_i2c_config_table[] = {0};

#define DEFINE_ROCKCHIP_I2C(ID)                             \
static void rockchip_i2c##ID##_irq(int irq, void *param);   \
                                                            \
static struct rockchip_i2c i2c##ID =                        \
{                                                           \
    .bus.priv = &i2c##ID,                                   \
    .bus.ops = &bus_ops,                                    \
    .bus.timeout = ROCKCHIP_I2C_TIMEOUT,                    \
    .name = "i2c"#ID,                                       \
    .dev = &g_i2c##ID##Dev,                                 \
    .irq_handler = rockchip_i2c##ID##_irq,                  \
};                                                          \
                                                            \
static void rockchip_i2c##ID##_irq(int irq, void *param)    \
{                                                           \
    rockchip_i2c_irq(&i2c##ID);                             \
}

#ifdef RT_USING_I2C0
DEFINE_ROCKCHIP_I2C(0)
#endif

#ifdef RT_USING_I2C1
DEFINE_ROCKCHIP_I2C(1)
#endif

#ifdef RT_USING_I2C2
DEFINE_ROCKCHIP_I2C(2)
#endif

#ifdef RT_USING_I2C3
DEFINE_ROCKCHIP_I2C(3)
#endif

#ifdef RT_USING_I2C4
DEFINE_ROCKCHIP_I2C(4)
#endif

#ifdef RT_USING_I2C5
DEFINE_ROCKCHIP_I2C(5)
#endif

static struct rockchip_i2c *const rockchip_i2c_table[] =
{
#ifdef RT_USING_I2C0
    &i2c0,
#endif
#ifdef RT_USING_I2C1
    &i2c1,
#endif
#ifdef RT_USING_I2C2
    &i2c2,
#endif
#ifdef RT_USING_I2C3
    &i2c3,
#endif
#ifdef RT_USING_I2C4
    &i2c4,
#endif
#ifdef RT_USING_I2C5
    &i2c5,
#endif
    RT_NULL
};

int rockchip_rt_hw_i2c_init(void)
{
    struct rockchip_i2c *const *i2c;
    const struct rockchip_i2c_config *cfg;
    eI2C_BusSpeed speed;

    for (i2c = rockchip_i2c_table; *i2c != RT_NULL; i2c++)
    {
        speed = I2C_100K;
        for (cfg = rockchip_i2c_config_table; cfg->id != 0; cfg++)
        {
            if ((*i2c)->dev->pReg == cfg->id)
            {
                speed = cfg->speed;
            }
        }
        rt_rockchip_i2c_probe(*i2c, speed);
    }

    return 0;
}

INIT_PREV_EXPORT(rockchip_rt_hw_i2c_init);
#endif

/** @} */  // I2C

/** @} */  // RKBSP_Driver_Reference
