/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_acdcdig.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   audio adc driver for rk soc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_ACDCDIG

#include "rk_audio.h"
#include "drv_clock.h"
#include "drv_regulator.h"

#include "hal_base.h"
#include "hal_bsp.h"

struct rk_acdcdig_dev
{
    struct audio_codec codec;
    struct HAL_ACDCDIG_DEV *hAcdcDig;
#if defined(RT_USING_PMU)
    struct pd *pd;
#endif
};

static inline struct rk_acdcdig_dev *to_acdcdig(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct rk_acdcdig_dev, codec);
}

static rt_err_t rk_acdcdig_init(struct audio_codec *codec,
                                struct AUDIO_INIT_CONFIG *config)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);
#ifdef HAL_PWR_MODULE_ENABLED
    struct regulator_desc *ldo_audio;

    ldo_audio = regulator_get_desc_by_pwrid(PWR_ID_VCC_AUDIO);
    if (!ldo_audio)
        return -RT_ERROR;

    regulator_enable(ldo_audio);
#endif
#if defined(RT_USING_PMU)
    if (acdcdig->pd)
        pd_on(acdcdig->pd);
#endif

    HAL_ACDCDIG_Init(acdcdig->hAcdcDig, config);

    return RT_EOK;
}

static rt_err_t rk_acdcdig_deinit(struct audio_codec *codec)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);
#ifdef HAL_PWR_MODULE_ENABLED
    struct regulator_desc *ldo_audio;
#endif

    HAL_ACDCDIG_DeInit(acdcdig->hAcdcDig);

#if defined(RT_USING_PMU)
    if (acdcdig->pd)
        pd_off(acdcdig->pd);
#endif
#ifdef HAL_PWR_MODULE_ENABLED
    ldo_audio = regulator_get_desc_by_pwrid(PWR_ID_VCC_AUDIO);
    if (!ldo_audio)
        return -RT_ERROR;

    regulator_disable(ldo_audio);
#endif

    return RT_EOK;
}

static rt_err_t rk_acdcdig_set_clk(struct audio_codec *codec,
                                   eAUDIO_streamType stream,
                                   uint32_t freq)
{
    return RT_EOK;
}

static rt_err_t rk_acdcdig_config(struct audio_codec *codec,
                                  eAUDIO_streamType stream,
                                  struct AUDIO_PARAMS *params)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);

    HAL_ACDCDIG_Config(acdcdig->hAcdcDig, stream, params);

    return RT_EOK;
}

static rt_err_t rk_acdcdig_start(struct audio_codec *codec,
                                 eAUDIO_streamType stream)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);

    HAL_ACDCDIG_Enable(acdcdig->hAcdcDig, stream);

    return RT_EOK;
}

static rt_err_t rk_acdcdig_stop(struct audio_codec *codec,
                                eAUDIO_streamType stream)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);

    HAL_ACDCDIG_Disable(acdcdig->hAcdcDig, stream);

    return RT_EOK;
}

static rt_err_t rk_acdcdig_set_gain(struct audio_codec *codec,
                                    eAUDIO_streamType stream,
                                    struct AUDIO_DB_CONFIG *db_config)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);

    HAL_ACDCDIG_SetGain(acdcdig->hAcdcDig, stream, db_config);

    return RT_EOK;
}

static rt_err_t rk_acdcdig_get_gain(struct audio_codec *codec,
                                    eAUDIO_streamType stream,
                                    struct AUDIO_DB_CONFIG *db_config)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);

    HAL_ACDCDIG_GetGain(acdcdig->hAcdcDig, stream, db_config);

    return RT_EOK;
}

static rt_err_t rk_acdcdig_get_gaininfo(struct audio_codec *codec,
                                        struct AUDIO_GAIN_INFO *info)
{
    struct rk_acdcdig_dev *acdcdig = to_acdcdig(codec);

    HAL_ACDCDIG_GetGainInfo(acdcdig->hAcdcDig, info);

    return RT_EOK;
}

static const struct audio_codec_ops rk_acdcdig_ops =
{
    .init = rk_acdcdig_init,
    .deinit = rk_acdcdig_deinit,
    .set_clk = rk_acdcdig_set_clk,
    .config = rk_acdcdig_config,
    .start = rk_acdcdig_start,
    .stop = rk_acdcdig_stop,
    .set_gain = rk_acdcdig_set_gain,
    .get_gain = rk_acdcdig_get_gain,
    .get_gaininfo = rk_acdcdig_get_gaininfo,
};

static struct audio_codec *rk_acdcdig_init_codec(struct HAL_ACDCDIG_DEV *hAcdcDig)
{
    struct rk_acdcdig_dev *acdcdig;

    acdcdig = rt_calloc(1, sizeof(*acdcdig));
    RT_ASSERT(acdcdig);

    acdcdig->codec.ops = &rk_acdcdig_ops;
    acdcdig->codec.id = (uint32_t)hAcdcDig->pReg;
#if defined(RT_USING_PMU)
    acdcdig->pd = get_pd_from_id(hAcdcDig->pd);
#endif
    acdcdig->hAcdcDig = hAcdcDig;

    return &acdcdig->codec;
}

int rt_hw_acdcdig_init(void)
{
#if defined(ACDCDIG)
    rk_audio_register_codec(rk_acdcdig_init_codec(&g_acdcDigDev));
#endif

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_acdcdig_init);

#endif
