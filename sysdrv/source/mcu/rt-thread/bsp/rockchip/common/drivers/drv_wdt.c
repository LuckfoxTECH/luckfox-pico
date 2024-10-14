/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_wdt.c
  * @author  Simon Xue
  * @version V0.1
  * @date    11-Mar-2019
  * @brief   watchdog driver
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup WDT
 *  @{
 */

/** @defgroup WDT_How_To_Use How To Use
 *  @{

 See more information, click [here](https://www.rt-thread.org/document/site/programming-manual/device/watchdog/watchdog/)

 @} */


#include "hal_base.h"
#include <rtdevice.h>

#if defined(RT_USING_WDT)

#ifdef RT_USING_WDT_DUMP
#include "cm_backtrace.h"
#endif

struct rt_wdt_dev
{
    rt_watchdog_t dw_wdt;
    uint32_t type;
    rt_uint32_t timeoutperiod; /* second */
    rt_uint32_t timeleft;
    uint32_t wdt_used;
    uint32_t freq;
};

static struct rt_wdt_dev *wdt_dev = RT_NULL;

static rt_err_t dw_wdt_stop(void)
{
    HAL_Status ret;

    ret = HAL_CRU_ClkDisable(PCLK_WDT0_GATE);
    if (ret)
    {
        rt_kprintf("failed to disable wdt clk, ret=%d", ret);

        return RT_EINVAL;
    }

    return RT_EOK;
}

static rt_err_t dw_wdt_start(uint32_t type)
{
    HAL_Status ret;

    ret = HAL_CRU_ClkEnable(PCLK_WDT0_GATE);
    if (ret)
    {
        rt_kprintf("failed to enable wdt clk, ret=%d", ret);

        return RT_EINVAL;
    }

    HAL_WDT_Start(type);

    return RT_EOK;
}

static rt_err_t dw_wdt_init(rt_watchdog_t *wdt)
{
    HAL_Status ret;
    uint32_t freq;

#ifdef CONFIG_RT_USING_SND_GLB_RST
    HAL_CRU_WdtGlbRstEnable(GLB_RST_SND_WDT0);
#else
    HAL_CRU_WdtGlbRstEnable(GLB_RST_FST_WDT0);
#endif

    freq = HAL_CRU_ClkGetFreq(PCLK_WDT);
    wdt_dev->freq = freq;

    ret = HAL_WDT_Init(freq, WDT);
    if (ret)
    {
        rt_kprintf("initialize wdt failed, ret=%d", ret);

        return RT_EINVAL;
    }

    return RT_EOK;
}

rt_err_t dw_wdt_set_top(int top_msec)
{
    HAL_WDT_SetTopMsec(top_msec, wdt_dev->freq);

    return RT_EOK;
}

static rt_err_t dw_wdt_control(rt_watchdog_t *wdt, int cmd, void *arg)
{
    rt_uint32_t top = 0;
    rt_uint32_t type;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        top = *(rt_uint32_t *)arg;
        HAL_WDT_SetTimeout(top);
        wdt_dev->timeoutperiod = top;
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMELEFT:
        *(rt_uint32_t *)arg = HAL_WDT_GetTimeLeft();
        break;
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        HAL_WDT_SetTimeout(wdt_dev->timeoutperiod);
        HAL_WDT_KeepAlive();
        break;
    case RT_DEVICE_CTRL_WDT_START:
        type = *(rt_uint32_t *)arg;
        dw_wdt_start(type);
        wdt_dev->wdt_used = 1;
        wdt_dev->type = type;
        break;
    case RT_DEVICE_CTRL_WDT_STOP:
        dw_wdt_stop();
        break;
    default:

        return RT_EIO;
    }

    return RT_EOK;
}

static const struct rt_watchdog_ops dw_wdt_pos =
{
    .init = dw_wdt_init,
    .control = dw_wdt_control,
};

static void rt_wdt_irqhandler(void)
{
    rt_uint32_t status;

    rt_interrupt_enter();

    status = HAL_WDT_GetIntStatus();

    rt_kprintf("%s, %d, status = 0x%x\n", __func__, __LINE__, status);

#ifdef RT_USING_WDT_DUMP
    cm_dump_thread();
    HAL_WDT_ClearInterrupt();
#endif

    rt_interrupt_leave();
}
#if defined(RT_USING_PM)
static int rt_wdt_pm_suspend(const struct rt_device *device)
{
    wdt_dev->timeleft = HAL_WDT_GetTimeLeft() / wdt_dev->freq;
    dw_wdt_stop();
    return RT_EOK;
}

static void rt_wdt_pm_resume(const struct rt_device *device)
{
    rt_err_t err;

    if (!wdt_dev->wdt_used)
        return;

    err = dw_wdt_init(&wdt_dev->dw_wdt);
    if (err)
    {
        rt_kprintf("failed to resume wdt\n");
        return;
    }

    HAL_WDT_SetTimeout(wdt_dev->timeleft);
    HAL_WDT_KeepAlive();
    dw_wdt_start(wdt_dev->type);
}

static struct rt_device_pm_ops rk_wdt_pm_ops =
{
    .suspend = rt_wdt_pm_suspend,
    .resume = rt_wdt_pm_resume,
};
#endif

int wdt_dev_init(void)
{
    wdt_dev = (struct rt_wdt_dev *)rt_calloc(1, sizeof(struct rt_wdt_dev));
    if (wdt_dev == RT_NULL)
        return -RT_ENOMEM;

    wdt_dev->dw_wdt.ops = &dw_wdt_pos;

    HAL_NVIC_SetIRQHandler(WDT0_IRQn, rt_wdt_irqhandler);
    HAL_NVIC_EnableIRQ(WDT0_IRQn);

#if defined(RT_USING_PM)
    rt_pm_register_device(&wdt_dev->dw_wdt.parent, &rk_wdt_pm_ops);
#endif

    rt_hw_watchdog_register(&wdt_dev->dw_wdt, "dw_wdt",  0, RT_NULL);

    return RT_EOK;
}

INIT_DEVICE_EXPORT(wdt_dev_init);

#endif

/** @} */  // WDT

/** @} */  // RKBSP_Driver_Reference
