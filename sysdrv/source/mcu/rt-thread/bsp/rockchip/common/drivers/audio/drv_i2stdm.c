/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_i2stdm.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   i2stdm driver for rk soc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <rthw.h>

#if defined(RT_USING_I2STDM) && (defined(RT_USING_I2STDM0) || defined(RT_USING_I2STDM1) || defined(RT_USING_I2STDM2))

#include "rk_audio.h"
#include "drv_clock.h"
#ifdef RT_USING_PM
#include <drivers/pm.h>
#endif

#include "hal_base.h"
#include "hal_bsp.h"

struct rk_i2stdm_dev
{
    struct rt_device parent;
    struct audio_dai dai;
    struct HAL_I2STDM_DEV *hI2sTdm;
    uint32_t trctrl_cnt;
    struct rt_mutex trcm_lock;
    struct clk_gate *mclk_tx_gate;
    struct clk_gate *mclk_rx_gate;
    struct clk_gate *hclk_gate;
#if defined(RT_USING_PMU)
    struct pd *pd;
#endif
};

static inline struct rk_i2stdm_dev *to_i2stdm(struct audio_dai *dai)
{
    return HAL_CONTAINER_OF(dai, struct rk_i2stdm_dev, dai);
}

static rt_err_t rk_i2stdm_init(struct audio_dai *dai,
                               struct AUDIO_INIT_CONFIG *config)
{
    struct rk_i2stdm_dev *i2stdm = to_i2stdm(dai);

#if defined(RT_USING_PMU)
    if (i2stdm->pd)
        pd_on(i2stdm->pd);
#endif
    clk_enable(i2stdm->hclk_gate);
    HAL_I2STDM_Init(i2stdm->hI2sTdm, config);

    return RT_EOK;
}

static rt_err_t rk_i2stdm_deinit(struct audio_dai *dai)
{
    struct rk_i2stdm_dev *i2stdm = to_i2stdm(dai);

    HAL_I2STDM_DeInit(i2stdm->hI2sTdm);
    clk_disable(i2stdm->hclk_gate);

#if defined(RT_USING_PMU)
    if (i2stdm->pd)
        pd_off(i2stdm->pd);
#endif

    return RT_EOK;
}

static rt_err_t rk_i2stdm_set_clk(struct audio_dai *dai,
                                  eAUDIO_streamType stream,
                                  uint32_t freq)
{
    struct rk_i2stdm_dev *i2stdm = to_i2stdm(dai);

    if (i2stdm->hI2sTdm->trcmMode != TRCM_NONE)
    {
        clk_set_rate(i2stdm->hI2sTdm->mclkTx, freq);
        clk_set_rate(i2stdm->hI2sTdm->mclkRx, freq);

        return RT_EOK;
    }

    if (stream == AUDIO_STREAM_PLAYBACK)
        clk_set_rate(i2stdm->hI2sTdm->mclkTx, freq);
    else
        clk_set_rate(i2stdm->hI2sTdm->mclkRx, freq);

    return RT_EOK;
}

static rt_err_t rk_i2stdm_config(struct audio_dai *dai,
                                 eAUDIO_streamType stream,
                                 struct AUDIO_PARAMS *params)
{
    struct rk_i2stdm_dev *i2stdm = to_i2stdm(dai);

    HAL_I2STDM_Config(i2stdm->hI2sTdm, stream, params);

    return RT_EOK;
}

static rt_err_t rk_i2stdm_start(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_i2stdm_dev *i2stdm = to_i2stdm(dai);
#ifdef HAL_I2STDM_TDM_MULTI_LANES
    rt_base_t level;
#endif

    if (i2stdm->hI2sTdm->trcmMode != TRCM_NONE)
    {
        clk_enable(i2stdm->mclk_tx_gate);
        clk_enable(i2stdm->mclk_rx_gate);

        rt_mutex_take(&i2stdm->trcm_lock, RT_WAITING_FOREVER);
        HAL_I2STDM_TxRxEnable(i2stdm->hI2sTdm, stream, !i2stdm->trctrl_cnt);
        i2stdm->trctrl_cnt++;
        rt_mutex_release(&i2stdm->trcm_lock);
    }
    else
    {
        if (stream == AUDIO_STREAM_PLAYBACK)
            clk_enable(i2stdm->mclk_tx_gate);
        else
            clk_enable(i2stdm->mclk_rx_gate);


#ifdef HAL_I2STDM_TDM_MULTI_LANES
        level = rt_hw_interrupt_disable();
#endif
        HAL_I2STDM_Enable(i2stdm->hI2sTdm, stream);
#ifdef HAL_I2STDM_TDM_MULTI_LANES
        rt_hw_interrupt_enable(level);
#endif
    }

    return RT_EOK;
}

static rt_err_t rk_i2stdm_stop(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_i2stdm_dev *i2stdm = to_i2stdm(dai);

    if (i2stdm->hI2sTdm->trcmMode != TRCM_NONE)
    {
        rt_mutex_take(&i2stdm->trcm_lock, RT_WAITING_FOREVER);
        if (i2stdm->trctrl_cnt)
            i2stdm->trctrl_cnt--;
        HAL_I2STDM_TxRxDisable(i2stdm->hI2sTdm, stream, !i2stdm->trctrl_cnt);
        rt_mutex_release(&i2stdm->trcm_lock);

        clk_disable(i2stdm->mclk_tx_gate);
        clk_disable(i2stdm->mclk_rx_gate);
    }
    else
    {
        HAL_I2STDM_Disable(i2stdm->hI2sTdm, stream);

        if (stream == AUDIO_STREAM_PLAYBACK)
            clk_disable(i2stdm->mclk_tx_gate);
        else
            clk_disable(i2stdm->mclk_rx_gate);
    }

    return RT_EOK;
}

#if defined(RT_USING_PM)
static int rk_i2stdm_pm_suspend(const struct rt_device *device)
{
    return RT_EOK;
}

static void rk_i2stdm_pm_resume(const struct rt_device *device)
{
}

static struct rt_device_pm_ops rk_i2stdm_pm_ops =
{
    .suspend = rk_i2stdm_pm_suspend,
    .resume = rk_i2stdm_pm_resume,
};
#endif

static const struct audio_dai_ops rk_i2stdm_ops =
{
    .init = rk_i2stdm_init,
    .deinit = rk_i2stdm_deinit,
    .set_clk = rk_i2stdm_set_clk,
    .config = rk_i2stdm_config,
    .start = rk_i2stdm_start,
    .stop = rk_i2stdm_stop,
};

static struct audio_dai *rk_i2stdm_init_dai(struct HAL_I2STDM_DEV *hI2sTdm)
{
    struct rk_i2stdm_dev *i2stdm;

    i2stdm = rt_calloc(1, sizeof(*i2stdm));
    RT_ASSERT(i2stdm);

    i2stdm->dai.ops = &rk_i2stdm_ops;
    i2stdm->dai.id = (uint32_t)hI2sTdm->pReg;
    i2stdm->dai.dmaData[AUDIO_STREAM_PLAYBACK] = &hI2sTdm->txDmaData;
    i2stdm->dai.dmaData[AUDIO_STREAM_CAPTURE] = &hI2sTdm->rxDmaData;
    i2stdm->hI2sTdm = hI2sTdm;
    i2stdm->trctrl_cnt = 0;
    i2stdm->mclk_tx_gate = get_clk_gate_from_id(hI2sTdm->mclkTxGate);
    i2stdm->mclk_rx_gate = get_clk_gate_from_id(hI2sTdm->mclkRxGate);
    i2stdm->hclk_gate = get_clk_gate_from_id(hI2sTdm->hclk);
#if defined(RT_USING_PMU)
    i2stdm->pd = get_pd_from_id(hI2sTdm->pd);
#endif
#if defined(RT_USING_PM)
    i2stdm->parent.user_data = i2stdm;
    rt_pm_register_device(&i2stdm->parent, &rk_i2stdm_pm_ops);
#endif
    rt_mutex_init(&i2stdm->trcm_lock, "trcm_lock", RT_IPC_FLAG_FIFO);

    return &i2stdm->dai;
}

int rt_hw_i2stdm_init(void)
{
#if defined(RT_USING_I2STDM0)
    rk_audio_register_dai(rk_i2stdm_init_dai(&g_i2sTdm0Dev));
#endif
#if defined(RT_USING_I2STDM1)
    rk_audio_register_dai(rk_i2stdm_init_dai(&g_i2sTdm1Dev));
#endif
#if defined(RT_USING_I2STDM2)
    rk_audio_register_dai(rk_i2stdm_init_dai(&g_i2sTdm2Dev));
#endif

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_i2stdm_init);
#endif
