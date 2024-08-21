/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_i2s.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   i2s driver for rk soc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_I2S

#include "rk_audio.h"
#include "drv_clock.h"
#include "drv_pm.h"

#ifdef RT_USING_PM
#include <drivers/pm.h>
#endif

#include "hal_base.h"
#include "hal_bsp.h"

struct rk_i2s_dev
{
    struct rt_device parent;
    struct audio_dai dai;
    struct HAL_I2S_DEV *hI2s;
    struct clk_gate *mclk_gate;
#if defined(RT_USING_PMU)
    struct pd *pd;
#endif
};

static inline struct rk_i2s_dev *to_i2s(struct audio_dai *dai)
{
    return HAL_CONTAINER_OF(dai, struct rk_i2s_dev, dai);
}

static rt_err_t rk_i2s_init(struct audio_dai *dai, struct AUDIO_INIT_CONFIG *config)
{
    struct rk_i2s_dev *i2s = to_i2s(dai);

#if defined(RT_USING_PMU)
    if (i2s->pd)
        pd_on(i2s->pd);
#endif
    HAL_I2S_Init(i2s->hI2s, config);

    return RT_EOK;
}

static rt_err_t rk_i2s_deinit(struct audio_dai *dai)
{
    struct rk_i2s_dev *i2s = to_i2s(dai);

    HAL_I2S_DeInit(i2s->hI2s);

#if defined(RT_USING_PMU)
    if (i2s->pd)
        pd_off(i2s->pd);
#endif

    return RT_EOK;
}

static rt_err_t rk_i2s_set_clk(struct audio_dai *dai,
                               eAUDIO_streamType stream,
                               uint32_t freq)
{
    struct rk_i2s_dev *i2s = to_i2s(dai);

    clk_set_rate(i2s->hI2s->mclk, freq);

    return RT_EOK;
}

static rt_err_t rk_i2s_config(struct audio_dai *dai,
                              eAUDIO_streamType stream,
                              struct AUDIO_PARAMS *params)
{
    struct rk_i2s_dev *i2s = to_i2s(dai);

    HAL_I2S_Config(i2s->hI2s, stream, params);

    return RT_EOK;
}

static rt_err_t rk_i2s_start(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_i2s_dev *i2s = to_i2s(dai);

    pm_runtime_request(PM_RUNTIME_ID_I2S);

    clk_enable(i2s->mclk_gate);
    HAL_I2S_Enable(i2s->hI2s, stream);

    return RT_EOK;
}

static rt_err_t rk_i2s_stop(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_i2s_dev *i2s = to_i2s(dai);

    HAL_I2S_Disable(i2s->hI2s, stream);
    clk_disable(i2s->mclk_gate);

    pm_runtime_release(PM_RUNTIME_ID_I2S);

    return RT_EOK;
}

#if defined(RT_USING_PM)
static int rk_i2s_pm_suspend(const struct rt_device *device)
{
    return RT_EOK;
}

static void rk_i2s_pm_resume(const struct rt_device *device)
{
}

static struct rt_device_pm_ops rk_i2s_pm_ops =
{
    .suspend = rk_i2s_pm_suspend,
    .resume = rk_i2s_pm_resume,
};
#endif

static const struct audio_dai_ops rk_i2s_ops =
{
    .init = rk_i2s_init,
    .deinit = rk_i2s_deinit,
    .set_clk = rk_i2s_set_clk,
    .config = rk_i2s_config,
    .start = rk_i2s_start,
    .stop = rk_i2s_stop,
};

static struct audio_dai *rk_i2s_init_dai(struct HAL_I2S_DEV *hI2s)
{
    struct rk_i2s_dev *i2s;

    i2s = rt_calloc(1, sizeof(*i2s));
    RT_ASSERT(i2s);

    i2s->dai.ops = &rk_i2s_ops;
    i2s->dai.id = (uint32_t)hI2s->pReg;
    i2s->dai.dmaData[AUDIO_STREAM_PLAYBACK] = &hI2s->txDmaData;
    i2s->dai.dmaData[AUDIO_STREAM_CAPTURE] = &hI2s->rxDmaData;
    i2s->mclk_gate = get_clk_gate_from_id(hI2s->mclkGate);
#if defined(RT_USING_PMU)
    i2s->pd = get_pd_from_id(hI2s->pd);
#endif
#if defined(RT_USING_PM)
    i2s->parent.user_data = i2s;
    rt_pm_register_device(&i2s->parent, &rk_i2s_pm_ops);
#endif
    i2s->hI2s = hI2s;

    return &i2s->dai;
}

int rt_hw_i2s_init(void)
{
#if defined(RT_USING_I2S0)
    rk_audio_register_dai(rk_i2s_init_dai(&g_i2s0Dev));
#endif
#if defined(RT_USING_I2S1)
    rk_audio_register_dai(rk_i2s_init_dai(&g_i2s1Dev));
#endif
#if defined(RT_USING_I2S2)
    rk_audio_register_dai(rk_i2s_init_dai(&g_i2s2Dev));
#endif

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_i2s_init);
#endif
