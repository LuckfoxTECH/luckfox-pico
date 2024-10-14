/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_mdais.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   mdais driver for rk soc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#if defined(RT_USING_MULTI_DAIS)

#include "rk_audio.h"
#include "drv_clock.h"
#include "drv_mdais.h"
#include "hal_base.h"
#include "hal_bsp.h"

static inline const uint8_t *get_maps(eAUDIO_streamType stream,
                                      struct rk_mdais_dev *mdais)
{
    if (stream == AUDIO_STREAM_CAPTURE)
        return mdais->desc->capture_mapping;
    else
        return mdais->desc->playback_mapping;
}

static rt_err_t rk_mdais_init(struct audio_dai *dai,
                              struct AUDIO_INIT_CONFIG *config)
{
    struct rk_mdais_dev *mdais = to_mdais(dai);
    struct audio_dai *child;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < mdais->num_dais; i++)
    {
        child = mdais->dais[i].dai;
        if (child->ops && child->ops->init)
        {
            ret = child->ops->init(child, config);
            if (ret < 0)
                return ret;
        }
    }

    return ret;
}

static rt_err_t rk_mdais_deinit(struct audio_dai *dai)
{
    struct rk_mdais_dev *mdais = to_mdais(dai);
    struct audio_dai *child;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < mdais->num_dais; i++)
    {
        child = mdais->dais[i].dai;
        if (child->ops && child->ops->deinit)
        {
            ret = child->ops->deinit(child);
            if (ret < 0)
                return ret;
        }
    }

    return ret;
}

static rt_err_t rk_mdais_set_clk(struct audio_dai *dai,
                                 eAUDIO_streamType stream,
                                 uint32_t freq)
{
    struct rk_mdais_dev *mdais = to_mdais(dai);
    const uint8_t *maps = get_maps(stream, mdais);
    struct audio_dai *child;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < mdais->num_dais; i++)
    {
        if (!maps[i])
            continue;
        child = mdais->dais[i].dai;
        if (child->ops && child->ops->set_clk)
        {
            ret = child->ops->set_clk(child, stream, freq);
            if (ret < 0)
                return ret;
        }
    }

    return ret;
}

static rt_err_t rk_mdais_config(struct audio_dai *dai,
                                eAUDIO_streamType stream,
                                struct AUDIO_PARAMS *params)
{
    struct rk_mdais_dev *mdais = to_mdais(dai);
    const uint8_t *maps = get_maps(stream, mdais);
    struct audio_dai *child;
    struct AUDIO_PARAMS cparams;
    rt_err_t ret = RT_EOK;
    int i;

    rt_memcpy(&cparams, params, sizeof(*params));

    for (i = 0; i < mdais->num_dais; i++)
    {
        if (!maps[i])
            continue;
        child = mdais->dais[i].dai;
        cparams.channels = maps[i];
        if (child->ops && child->ops->config)
        {
            ret = child->ops->config(child, stream, &cparams);
            if (ret < 0)
                return ret;
        }
    }

    return ret;
}

static rt_err_t rk_mdais_start(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_mdais_dev *mdais = to_mdais(dai);
    const uint8_t *maps = get_maps(stream, mdais);
    struct audio_dai *child;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < mdais->num_dais; i++)
    {
        if (!maps[i])
            continue;
        child = mdais->dais[i].dai;
        if (child->ops && child->ops->start)
        {
            ret = child->ops->start(child, stream);
            if (ret < 0)
                return ret;
        }
    }

    return ret;
}

static rt_err_t rk_mdais_stop(struct audio_dai *dai, eAUDIO_streamType stream)
{
    struct rk_mdais_dev *mdais = to_mdais(dai);
    const uint8_t *maps = get_maps(stream, mdais);
    struct audio_dai *child;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < mdais->num_dais; i++)
    {
        if (!maps[i])
            continue;
        child = mdais->dais[i].dai;
        if (child->ops && child->ops->stop)
        {
            ret = child->ops->stop(child, stream);
            if (ret < 0)
                return ret;
        }
    }

    return ret;
}

static const struct audio_dai_ops rk_mdais_ops =
{
    .init = rk_mdais_init,
    .deinit = rk_mdais_deinit,
    .set_clk = rk_mdais_set_clk,
    .config = rk_mdais_config,
    .start = rk_mdais_start,
    .stop = rk_mdais_stop,
};

static struct audio_dai *rk_mdais_init_dai(const struct audio_mdais_desc *amd)
{
    struct rk_mdais_dev *mdais;
    struct rk_dai *dais;
    uint8_t count = 0, i = 0;

    mdais = rt_calloc(1, sizeof(*mdais));
    RT_ASSERT(mdais);

    /* child dai counts */
    while (amd->dais[count] != RT_NULL)
        count++;

    dais = rt_calloc(count, sizeof(*dais));
    if (!dais)
        goto err;

    for (i = 0; i < count; i++)
    {
        dais[i].dai = rk_audio_find_dai((uint32_t)amd->dais[i]);
        if (!dais[i].dai)
        {
            rt_kprintf("%s: dai: %p missing\n", __func__, amd->dais[i]);
            goto err;
        }
    }

    mdais->num_dais = count;
    mdais->dais = dais;
    mdais->desc = amd;
    mdais->dai.ops = &rk_mdais_ops;
    mdais->dai.id = (uint32_t)amd;

    return &mdais->dai;
err:
    if (dais)
        rt_free(dais);
    if (mdais)
        rt_free(mdais);

    return RT_NULL;
}

extern const struct audio_mdais_desc rk_mdais;

int rt_hw_mdais_init(void)
{
    rt_err_t ret;

    ret = rk_audio_register_dai(rk_mdais_init_dai(&rk_mdais));
    if (ret)
        rt_kprintf("%s: mdais register failed!\n", __func__);

    return ret;
}

INIT_COMPONENT_EXPORT(rt_hw_mdais_init);
#endif
