/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pdm.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   pdm driver for rksoc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#if defined(RT_USING_PDM) && (defined(RT_USING_PDM0) || defined(RT_USING_PDM1))

#include "rk_audio.h"
#include "drv_clock.h"

#include "hal_base.h"
#include "hal_bsp.h"

struct rk_pdm_dev
{
    struct audio_dai dai;
    struct HAL_PDM_DEV *hPdm;
#if defined(RT_USING_PMU)
    struct pd *pd;
#endif
};

static inline struct rk_pdm_dev *to_pdm(struct audio_dai *dai)
{
    return HAL_CONTAINER_OF(dai, struct rk_pdm_dev, dai);
}

static rt_err_t rk_pdm_init(struct audio_dai *dai, struct AUDIO_INIT_CONFIG *config)
{
    struct rk_pdm_dev *pdm = to_pdm(dai);

#if defined(RT_USING_PMU)
    if (pdm->pd)
        pd_on(pdm->pd);
#endif
    clk_enable_by_id(pdm->hPdm->mclkGate);

    HAL_PDM_Init(pdm->hPdm, config);

    return RT_EOK;
}

static rt_err_t rk_pdm_deinit(struct audio_dai *dai)
{
    struct rk_pdm_dev *pdm = to_pdm(dai);

    HAL_PDM_DeInit(pdm->hPdm);

    clk_disable_by_id(pdm->hPdm->mclkGate);

#if defined(RT_USING_PMU)
    if (pdm->pd)
        pd_off(pdm->pd);
#endif

    return RT_EOK;
}

static rt_err_t rk_pdm_config(struct audio_dai *dai,
                              eAUDIO_streamType stream,
                              struct AUDIO_PARAMS *params)
{
    struct rk_pdm_dev *pdm = to_pdm(dai);

    RT_ASSERT(stream == AUDIO_STREAM_CAPTURE);
    HAL_PDM_Config(pdm->hPdm, params);

    return RT_EOK;
}

static rt_err_t rk_pdm_start(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_pdm_dev *pdm = to_pdm(dai);

    RT_ASSERT(stream == AUDIO_STREAM_CAPTURE);
    HAL_PDM_Enable(pdm->hPdm);

    return RT_EOK;
}

static rt_err_t rk_pdm_stop(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_pdm_dev *pdm = to_pdm(dai);

    RT_ASSERT(stream == AUDIO_STREAM_CAPTURE);
    HAL_PDM_Disable(pdm->hPdm);

    return RT_EOK;
}

static const struct audio_dai_ops rk_pdm_ops =
{
    .init = rk_pdm_init,
    .deinit = rk_pdm_deinit,
    .config = rk_pdm_config,
    .start = rk_pdm_start,
    .stop = rk_pdm_stop,
};

static struct audio_dai *rk_pdm_init_dai(struct HAL_PDM_DEV *hPdm)
{
    struct rk_pdm_dev *pdm;

    pdm = rt_calloc(1, sizeof(*pdm));
    RT_ASSERT(pdm);

    pdm->dai.ops = &rk_pdm_ops;
    pdm->dai.id = (uint32_t)hPdm->pReg;
    pdm->dai.dmaData[AUDIO_STREAM_CAPTURE] = &hPdm->rxDmaData;
#if defined(RT_USING_PMU)
    pdm->pd = get_pd_from_id(hPdm->pd);
#endif
    pdm->hPdm = hPdm;

    return &pdm->dai;
}

int rt_hw_pdm_init(void)
{
#if defined(RT_USING_PDM0)
    rk_audio_register_dai(rk_pdm_init_dai(&g_pdm0Dev));
#endif
#if defined(RT_USING_PDM1)
    rk_audio_register_dai(rk_pdm_init_dai(&g_pdm1Dev));
#endif

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_pdm_init);

#endif
