/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_saradc.c
  * @author  Simon Xue
  * @version V0.1
  * @date    19-Jun-2019
  * @brief   saradc driver
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>

#if defined(RT_USING_SARADC)
#include "hal_base.h"
#include <rtdef.h>
#include <rtthread.h>
#include <rtdbg.h>

#define ADC_NAME "rk_adc0"
#define TIMEOUT 10

struct rk_saradc
{
    struct SARADC_REG *reg;
    rt_uint32_t chn;
    rt_uint32_t mode;
    struct rt_event event;
    struct rt_adc_device rk_saradc_dev;
};

static struct rk_saradc rk_saradc;

static rt_err_t rk_wait_saradc_completed(struct rk_saradc *rk_saradc)
{
    rt_uint32_t status;

    if (rt_event_recv(&rk_saradc->event, 0xffffffff, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      rt_tick_from_millisecond(1), &status) != RT_EOK)
    {
        LOG_E("wait completed timeout");
        return -RT_ETIMEOUT;
    }

    if (status != 0x1)
        return -RT_ERROR;

    return RT_EOK;
}
static rt_err_t rk_get_saradc_value(struct rt_adc_device *device, rt_uint32_t channel, rt_uint32_t *value)
{
    int ret;

    struct rk_saradc *rk_saradc = device->parent.user_data;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(value != RT_NULL);

    HAL_SARADC_Start(rk_saradc->reg, rk_saradc->mode, channel);

    ret = rk_wait_saradc_completed(rk_saradc);

    if (ret != RT_EOK)
    {
        LOG_D("%s :  failed to read adc data\n", ADC_NAME);
        return -RT_ERROR;
    }

    *value = (rt_uint32_t)HAL_SARADC_GetRaw(rk_saradc->reg, channel);

    HAL_SARADC_Stop(rk_saradc->reg);

    return RT_EOK;
}

static void rk_saradc_irq_handler(int vector, void *param)
{
    rt_interrupt_enter();

    HAL_SARADC_IrqHandler(rk_saradc.reg);

    rt_event_send(&rk_saradc.event, 0x1);

    rt_interrupt_leave();
}

static const struct rt_adc_ops rk_saradc_ops =
{
    .convert = rk_get_saradc_value,
};

static int rk_saradc_init(void)
{
    int result = RT_EOK;

    rk_saradc.mode = SARADC_INT_MOD;
    rk_saradc.reg = SARADC;

    HAL_CRU_ClkEnable(PCLK_SARADC_CONTROL_GATE);
    HAL_CRU_ClkEnable(CLK_SARADC_GATE);

#ifdef SRST_P_SARADC
    HAL_CRU_ClkResetAssert(SRST_P_SARADC);
    HAL_DelayUs(10);
    HAL_CRU_ClkResetDeassert(SRST_P_SARADC);
#endif

    rt_event_init(&rk_saradc.event, "saradc", RT_IPC_FLAG_FIFO);

    rt_hw_interrupt_install(SARADC_IRQn, rk_saradc_irq_handler, RT_NULL, RT_NULL);
    rt_hw_interrupt_umask(SARADC_IRQn);

    if (rt_hw_adc_register(&rk_saradc.rk_saradc_dev, ADC_NAME, &rk_saradc_ops, &rk_saradc) == RT_EOK)
    {
        LOG_D("%s init success", ADC_NAME);
    }
    else
    {
        LOG_E("%s register failed", ADC_NAME);
        result = -RT_ERROR;
    }

    return result;
}

INIT_BOARD_EXPORT(rk_saradc_init);

#endif
