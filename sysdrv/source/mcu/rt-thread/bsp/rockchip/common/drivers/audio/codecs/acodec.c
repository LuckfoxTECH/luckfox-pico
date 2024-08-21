/**
  * Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_acodec.c
  * @author  sugar zhang
  * @version V0.1
  * @date    22-Jun-2021
  * @brief   driver for rk2106 internal acodec
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_ACODEC

#include "rk_audio.h"
#include "drv_clock.h"

#include "board.h"
#include "hal_base.h"
#include "hal_bsp.h"

struct rk_acodec_dev
{
    struct audio_codec codec;
    struct HAL_ACODEC_DEV *hAcodec;
};

static inline struct rk_acodec_dev *to_acodec(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct rk_acodec_dev, codec);
}

static rt_err_t rk_acodec_init(struct audio_codec *codec,
                               struct AUDIO_INIT_CONFIG *config)
{
    struct rk_acodec_dev *acodec = to_acodec(codec);

    HAL_ACODEC_Init(acodec->hAcodec, config);

    return RT_EOK;
}

static rt_err_t rk_acodec_deinit(struct audio_codec *codec)
{
    struct rk_acodec_dev *acodec = to_acodec(codec);

    HAL_ACODEC_DeInit(acodec->hAcodec);

    return RT_EOK;
}

static rt_err_t rk_acodec_set_clk(struct audio_codec *codec,
                                  eAUDIO_streamType stream,
                                  uint32_t freq)
{
    return RT_EOK;
}

static rt_err_t rk_acodec_config(struct audio_codec *codec,
                                 eAUDIO_streamType stream,
                                 struct AUDIO_PARAMS *params)
{
    struct rk_acodec_dev *acodec = to_acodec(codec);

    HAL_ACODEC_Config(acodec->hAcodec, stream, params);

    return RT_EOK;
}

static rt_err_t rk_acodec_start(struct audio_codec *codec,
                                eAUDIO_streamType stream)
{
    struct rk_acodec_dev *acodec = to_acodec(codec);

    HAL_ACODEC_Enable(acodec->hAcodec, stream);

#ifdef PA_MUTE_PIN
    HAL_GPIO_SetPinLevel(PA_MUTE_GPIO, PA_MUTE_PIN, PA_MUTE_SWITCH_ON);
#endif

    return RT_EOK;
}

static rt_err_t rk_acodec_stop(struct audio_codec *codec,
                               eAUDIO_streamType stream)
{
    struct rk_acodec_dev *acodec = to_acodec(codec);

    HAL_ACODEC_Disable(acodec->hAcodec, stream);

#ifdef PA_MUTE_PIN
    HAL_GPIO_SetPinLevel(PA_MUTE_GPIO, PA_MUTE_PIN, PA_MUTE_SWITCH_OFF);
#endif

    return RT_EOK;
}

static const struct audio_codec_ops rk_acodec_ops =
{
    .init = rk_acodec_init,
    .deinit = rk_acodec_deinit,
    .set_clk = rk_acodec_set_clk,
    .config = rk_acodec_config,
    .start = rk_acodec_start,
    .stop = rk_acodec_stop,
};

static struct audio_codec *rk_acodec_init_codec(struct HAL_ACODEC_DEV *hAcodec)
{
    struct rk_acodec_dev *acodec;

    acodec = rt_calloc(1, sizeof(*acodec));
    RT_ASSERT(acodec);

    acodec->codec.ops = &rk_acodec_ops;
    acodec->codec.id = (uint32_t)hAcodec->pReg;
    acodec->hAcodec = hAcodec;

    return &acodec->codec;
}

int rt_hw_acodec_init(void)
{
    rk_audio_register_codec(rk_acodec_init_codec(&g_acodecDev));

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_acodec_init);

#endif
