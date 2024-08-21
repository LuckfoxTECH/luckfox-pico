/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_vad.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   vad driver for rksoc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <rthw.h>

#ifdef RT_USING_VAD

#include "drv_clock.h"
#include "rk_audio.h"
#include "hal_base.h"
#include "hal_bsp.h"

struct rk_vad_dev
{
    struct rt_device dev;

    struct audio_dai dai;
    struct HAL_VAD_DEV *hVad;
#if defined(RT_USING_PMU)
    struct pd *pd;
#endif
};

static inline struct rk_vad_dev *to_vad(struct audio_dai *dai)
{
    return HAL_CONTAINER_OF(dai, struct rk_vad_dev, dai);
}

static rt_err_t rk_vad_init(struct audio_dai *dai, struct AUDIO_INIT_CONFIG *config)
{
    struct rk_vad_dev *vad = to_vad(dai);
    uint32_t source = (uint32_t)dai->card->desc.dai;

    HAL_ASSERT(source);

#if defined(RT_USING_PMU)
    if (vad->pd)
        pd_on(vad->pd);
#endif
    HAL_VAD_Init(vad->hVad);
    HAL_VAD_SetAudioSource(vad->hVad, source);

    return RT_EOK;
}

static rt_err_t rk_vad_deinit(struct audio_dai *dai)
{
    struct rk_vad_dev *vad = to_vad(dai);

    HAL_VAD_DeInit(vad->hVad);

#if defined(RT_USING_PMU)
    if (vad->pd)
        pd_off(vad->pd);
#endif

    return RT_EOK;
}

static rt_err_t rk_vad_config(struct audio_dai *dai,
                              eAUDIO_streamType stream,
                              struct AUDIO_PARAMS *params)
{
    struct rk_vad_dev *vad = to_vad(dai);

    RT_ASSERT(stream == AUDIO_STREAM_CAPTURE);

    HAL_VAD_Config(vad->hVad, params);

    return RT_EOK;
}

static rt_err_t rk_vad_start(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_vad_dev *vad = to_vad(dai);

    RT_ASSERT(stream == AUDIO_STREAM_CAPTURE);

    HAL_VAD_Start(vad->hVad);

    return RT_EOK;
}

static rt_err_t rk_vad_stop(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_vad_dev *vad = to_vad(dai);

    RT_ASSERT(stream == AUDIO_STREAM_CAPTURE);

    HAL_VAD_Stop(vad->hVad);

    return RT_EOK;
}

static const struct audio_dai_ops rk_vad_dai_ops =
{
    .init = rk_vad_init,
    .deinit = rk_vad_deinit,
    .config = rk_vad_config,
    .start = rk_vad_start,
    .stop = rk_vad_stop,
};

static rt_err_t rk_vad_ctl(rt_device_t dev, int cmd, void *args)
{
    struct rk_vad_dev *vad = dev->user_data;
    HAL_Status hRet = HAL_OK;

    switch (cmd)
    {
    case RK_AUDIO_CTL_VAD_SET_BUFFER:
    {
        struct audio_buf *abuf = (struct audio_buf *)args;
        uint32_t start = (uint32_t)abuf->buf; /* phy addr */
        uint32_t end = (uint32_t)(abuf->buf + abuf->buf_size);

        hRet = HAL_VAD_ConfigRingBuffer(vad->hVad, start, end);
    }
    break;
    case RK_AUDIO_CTL_VAD_SET_PERIOD:
    {
        uint32_t words = *(uint32_t *)args;

        hRet = HAL_VAD_SetPeriodSize(vad->hVad, words);
    }
    break;
    case RK_AUDIO_CTL_VAD_EN_BUSMODE:
    {
        hRet = HAL_VAD_EnableBusMode(vad->hVad);
    }
    break;
    case RK_AUDIO_CTL_VAD_DIS_BUSMODE:
    {

        hRet = HAL_VAD_DisableBusMode(vad->hVad);
    }
    break;
    default:
        hRet = HAL_INVAL;
        break;
    }

    return (rt_err_t)hRet;
}

#ifdef RT_USING_DEVICE_OPS
static const struct rt_device_ops rk_vad_ops =
{
    .control = rk_vad_ctl,
};
#endif

#if defined(VAD) && defined(RT_VAD_IRQ_HANDLED_BY_CPU)
static void rk_vad_isr(int irq, void *param)
{
    rt_interrupt_enter();

    HAL_VAD_IrqHandler(&g_vadDev);

    rt_interrupt_leave();
}
#endif

int rt_hw_vad_init(void)
{
#if defined(VAD)
    struct HAL_VAD_DEV *hVad = &g_vadDev;
    struct rk_vad_dev *vad;

    vad = rt_calloc(1, sizeof(*vad));
    RT_ASSERT(vad);

    vad->dai.ops = &rk_vad_dai_ops;
    vad->dai.id = (uint32_t)hVad->pReg;
#if defined(RT_USING_PMU)
    vad->pd = get_pd_from_id(hVad->pd);
#endif
    vad->hVad = hVad;

    rk_audio_register_dai(&vad->dai);

#ifdef RT_VAD_IRQ_HANDLED_BY_CPU
    /*
     * if dsp ASR(audio speech recognition) enabled, the irq is handled by dsp;
     * else the irq is handled by cm4.
     */
    rt_hw_interrupt_install(hVad->irq, rk_vad_isr, RT_NULL, RT_NULL);
    rt_hw_interrupt_umask(hVad->irq);
#endif

    vad->dev.type = RT_Device_Class_Miscellaneous;
#ifdef RT_USING_DEVICE_OPS
    vad->dev.ops = &rk_vad_ops;
#else
    vad->dev.init = RT_NULL;
    vad->dev.open = RT_NULL;
    vad->dev.close = RT_NULL;
    vad->dev.read = RT_NULL;
    vad->dev.write = RT_NULL;
    vad->dev.control = rk_vad_ctl;
#endif
    vad->dev.user_data = vad;

    rt_device_register(&vad->dev, "vad", RT_DEVICE_FLAG_RDONLY);
#endif
    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_vad_init);
#endif
