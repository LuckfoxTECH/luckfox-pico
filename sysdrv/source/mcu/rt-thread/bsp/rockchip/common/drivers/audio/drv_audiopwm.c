/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_audiopwm.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   audioPwm driver for rksoc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_AUDIOPWM

#include "drv_clock.h"
#ifdef RT_USING_PM
#include <drivers/pm.h>
#endif
#include "rk_audio.h"
#include "hal_base.h"
#include "hal_bsp.h"
#include "board.h"
#include "drv_regulator.h"

struct rk_audiopwm_dev
{
    struct rt_device parent;
    struct audio_dai dai;
    struct HAL_AUDIOPWM_DEV *hAudioPwm;
};

static inline struct rk_audiopwm_dev *to_audiopwm(struct audio_dai *dai)
{
    return HAL_CONTAINER_OF(dai, struct rk_audiopwm_dev, dai);
}

static void rk_audiopwm_mute(bool mute)
{
#ifdef HAL_PWR_MODULE_ENABLED
    struct regulator_desc *ldo_audio = regulator_get_desc_by_pwrid(PWR_ID_VCC33_CODEC);
#endif

    if (mute)
    {
#ifdef AUDIOPWM_PA_GPIO
        HAL_GPIO_SetPinLevel(AUDIOPWM_PA_GPIO, AUDIOPWM_PA_PIN, GPIO_LOW);
#endif
#ifdef HAL_PWR_MODULE_ENABLED
        if (ldo_audio)
            regulator_disable(ldo_audio);
#endif
    }
    else
    {
#ifdef HAL_PWR_MODULE_ENABLED
        if (ldo_audio)
            regulator_enable(ldo_audio);
#endif
#ifdef AUDIOPWM_PA_GPIO
        HAL_GPIO_SetPinLevel(AUDIOPWM_PA_GPIO, AUDIOPWM_PA_PIN, GPIO_HIGH);
#endif
    }
}

static rt_err_t rk_audiopwm_init(struct audio_dai *dai, struct AUDIO_INIT_CONFIG *config)
{
    struct rk_audiopwm_dev *audiopwm = to_audiopwm(dai);

#ifdef AUDIOPWM_PA_GPIO
    HAL_PINCTRL_SetIOMUX(AUDIOPWM_PA_GPIO_BANK, AUDIOPWM_PA_PIN, AUDIOPWM_PA_PIN_FUNC_GPIO);
    HAL_GPIO_SetPinDirection(AUDIOPWM_PA_GPIO, AUDIOPWM_PA_PIN, GPIO_OUT);
    HAL_GPIO_SetPinLevel(AUDIOPWM_PA_GPIO, AUDIOPWM_PA_PIN, GPIO_LOW);
#endif
    clk_enable_by_id(audiopwm->hAudioPwm->mclkGate);
    HAL_AUDIOPWM_Init(audiopwm->hAudioPwm, config);

    return RT_EOK;
}

static rt_err_t rk_audiopwm_deinit(struct audio_dai *dai)
{
    struct rk_audiopwm_dev *audiopwm = to_audiopwm(dai);

    HAL_AUDIOPWM_DeInit(audiopwm->hAudioPwm);
    clk_disable_by_id(audiopwm->hAudioPwm->mclkGate);

    return RT_EOK;
}

static rt_err_t rk_audiopwm_set_clk(struct audio_dai *dai,
                                    eAUDIO_streamType stream,
                                    uint32_t freq)
{
    struct rk_audiopwm_dev *audiopwm = to_audiopwm(dai);

    RT_ASSERT(stream == AUDIO_STREAM_PLAYBACK);
    clk_set_rate(audiopwm->hAudioPwm->mclk, freq);

    return RT_EOK;
}

static rt_err_t rk_audiopwm_config(struct audio_dai *dai,
                                   eAUDIO_streamType stream,
                                   struct AUDIO_PARAMS *params)
{
    struct rk_audiopwm_dev *audiopwm = to_audiopwm(dai);

    RT_ASSERT(stream == AUDIO_STREAM_PLAYBACK);
    HAL_AUDIOPWM_Config(audiopwm->hAudioPwm, params);

    return RT_EOK;
}

static rt_err_t rk_audiopwm_start(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_audiopwm_dev *audiopwm = to_audiopwm(dai);

    RT_ASSERT(stream == AUDIO_STREAM_PLAYBACK);
    HAL_AUDIOPWM_Enable(audiopwm->hAudioPwm);
    rk_audiopwm_mute(0);

    return RT_EOK;
}

static rt_err_t rk_audiopwm_stop(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_audiopwm_dev *audiopwm = to_audiopwm(dai);

    RT_ASSERT(stream == AUDIO_STREAM_PLAYBACK);
    rk_audiopwm_mute(1);
    HAL_AUDIOPWM_Disable(audiopwm->hAudioPwm);

    return RT_EOK;
}

#if defined(RT_USING_PM)
static int rk_audiopwm_pm_suspend(const struct rt_device *device)
{
    return RT_EOK;
}

static void rk_audiopwm_pm_resume(const struct rt_device *device)
{
}

static struct rt_device_pm_ops rk_audiopwm_pm_ops =
{
    .suspend = rk_audiopwm_pm_suspend,
    .resume = rk_audiopwm_pm_resume,
};
#endif

static const struct audio_dai_ops rk_audiopwm_ops =
{
    .init = rk_audiopwm_init,
    .deinit = rk_audiopwm_deinit,
    .set_clk = rk_audiopwm_set_clk,
    .config = rk_audiopwm_config,
    .start = rk_audiopwm_start,
    .stop = rk_audiopwm_stop,
};

static struct audio_dai *rk_audiopwm_init_dai(struct HAL_AUDIOPWM_DEV *hAudioPwm)
{
    struct rk_audiopwm_dev *audiopwm;

    audiopwm = rt_calloc(1, sizeof(*audiopwm));
    RT_ASSERT(audiopwm);

    audiopwm->dai.ops = &rk_audiopwm_ops;
    audiopwm->dai.id = (uint32_t)hAudioPwm->pReg;
    audiopwm->dai.dmaData[AUDIO_STREAM_PLAYBACK] = &hAudioPwm->txDmaData;
    audiopwm->hAudioPwm = hAudioPwm;

#if defined(RT_USING_PM)
    audiopwm->parent.user_data = audiopwm;
    rt_pm_register_device(&audiopwm->parent, &rk_audiopwm_pm_ops);
#endif

    return &audiopwm->dai;
}

int rt_hw_audiopwm_init(void)
{
#if defined(AUDIOPWM)
    rk_audio_register_dai(rk_audiopwm_init_dai(&g_audioPwmDev));
#endif

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_audiopwm_init);
#endif
