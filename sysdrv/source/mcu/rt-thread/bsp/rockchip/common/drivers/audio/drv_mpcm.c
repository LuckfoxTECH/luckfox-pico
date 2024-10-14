/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_mpcm.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   mpcm driver for rk soc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_MULTI_PCM

#include "dma.h"
#include "rk_audio.h"
#include "drv_mdais.h"
#include "hal_base.h"

#define PCM_BOUNDARY_MAX    (0x7FFFFFFFU)

struct rk_mpcm
{
    struct audio_pcm pcm;
    struct rk_mdais_dev *mdais;
    struct rt_device *dma[MAX_DAIS];
    struct rt_dma_transfer dma_xfers[MAX_DAIS];
    uint8_t master_xfer;
};

static inline struct rk_mpcm *to_rkmpcm(struct audio_pcm *pcm)
{
    return HAL_CONTAINER_OF(pcm, struct rk_mpcm, pcm);
}

static void rk_mpcm_complete(void *arg)
{
    struct audio_pcm *pcm = (struct audio_pcm *)arg;

    rk_audio_stream_update(pcm->as);
}

static rt_err_t rk_mpcm_init(struct audio_pcm *pcm,
                             eAUDIO_streamType stream,
                             struct audio_buf *buf)
{
    struct rk_mpcm *rkmpcm = to_rkmpcm(pcm);
    struct rk_mdais_dev *mdais = rkmpcm->mdais;
    struct rt_dma_transfer *dx = rkmpcm->dma_xfers;
    struct audio_dai *child;
    const struct AUDIO_DMA_DATA *data;
    const uint8_t *maps;
    rt_err_t ret = RT_EOK;
    int i;

    if (stream == AUDIO_STREAM_CAPTURE)
        maps = mdais->desc->capture_mapping;
    else
        maps = mdais->desc->playback_mapping;

    rkmpcm->pcm.abuf = *buf;
    pcm->boundary = pcm->abuf.buf_size;

    while (pcm->boundary << 1 <= PCM_BOUNDARY_MAX - pcm->abuf.buf_size)
        pcm->boundary <<= 1;

    for (i = 0; i < mdais->num_dais; i++, dx++)
    {
        if (!maps[i])
            continue;
        child = mdais->dais[i].dai;
        data = child->dmaData[stream];
        dx->dma_req_num = data->dmaReqCh;
        rkmpcm->dma[i] = rt_dma_get((uint32_t)data->dmac);
        ret = rt_device_control(rkmpcm->dma[i], RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL, dx);
        RT_ASSERT(ret == RT_EOK);
    }

    return ret;
}

static rt_err_t rk_mpcm_deinit(struct audio_pcm *pcm)
{
    struct rk_mpcm *rkmpcm = to_rkmpcm(pcm);
    struct rk_mdais_dev *mdais = rkmpcm->mdais;
    struct rt_dma_transfer *dx = rkmpcm->dma_xfers;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < mdais->num_dais; i++, dx++)
    {
        if (dx->chan)
            ret = rt_device_control(rkmpcm->dma[i], RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL, dx);
    }

    return ret;
}

static rt_err_t rk_mpcm_config(struct audio_pcm *pcm, eAUDIO_streamType stream,
                               struct AUDIO_PARAMS *params)
{
    struct rk_mpcm *rkmpcm = to_rkmpcm(pcm);
    struct rk_mdais_dev *mdais = rkmpcm->mdais;
    const struct AUDIO_DMA_DATA *data;
    struct rt_dma_transfer *dx = rkmpcm->dma_xfers;
    struct audio_buf *buf = &rkmpcm->pcm.abuf;
    struct audio_dai *child;
    rt_err_t ret;
    const uint8_t *maps;
    bool callback = false;
    int i, sz, frame_bytes, offset = 0;


    RT_ASSERT(buf->buf_size);
    RT_ASSERT(buf->period_size);

    rkmpcm->pcm.params = *params;

    if (stream == AUDIO_STREAM_CAPTURE)
        maps = mdais->desc->capture_mapping;
    else
        maps = mdais->desc->playback_mapping;

    frame_bytes = frames_to_bytes(pcm, 1);

    for (i = 0; i < mdais->num_dais; i++, dx++)
    {
        if (!maps[i])
            continue;
        child = mdais->dais[i].dai;
        data = child->dmaData[stream];
        dx->len = frames_to_bytes(pcm, buf->buf_size);
        dx->period_len = frames_to_bytes(pcm, buf->period_size);

        /* refine params for interlace access */
        sz = samples_to_bytes(pcm, maps[i]);
        rt_kprintf("fb: %d, sz: %d\n", frame_bytes, sz);
        if (stream == AUDIO_STREAM_PLAYBACK)
        {
            dx->direction = RT_DMA_MEM_TO_DEV;
            dx->src_addr = (rt_uint32_t)buf->buf + offset;
            dx->dst_addr = data->addr;
            dx->dst_addr_width = data->addrWidth;
            dx->dst_maxburst = data->maxBurst;
            if (sz)
            {
                dx->src_interlace_size = frame_bytes - sz;
                if (dx->src_interlace_size)
                    dx->dst_maxburst = sz / dx->dst_addr_width;
            }
        }
        else
        {
            dx->direction = RT_DMA_DEV_TO_MEM;
            dx->src_addr = data->addr;
            dx->src_addr_width = data->addrWidth;
            dx->src_maxburst = data->maxBurst;
            dx->dst_addr = (rt_uint32_t)buf->buf + offset;
            if (sz)
            {
                dx->dst_interlace_size = frame_bytes - sz;
                if (dx->dst_interlace_size)
                    dx->src_maxburst = sz / dx->src_addr_width;
            }
        }

        if (!callback)
        {
            dx->callback = rk_mpcm_complete;
            dx->cparam = pcm;
            callback = true;
            rkmpcm->master_xfer = i;
        }

        ret = rt_device_control(rkmpcm->dma[i], RT_DEVICE_CTRL_DMA_CYCLIC_PREPARE, dx);
        if (ret)
            return ret;

        offset += sz;
    }

    return ret;
}

static rt_err_t rk_mpcm_start(struct audio_pcm *pcm)
{
    struct rk_mpcm *rkmpcm = to_rkmpcm(pcm);
    struct rt_dma_transfer *dx = rkmpcm->dma_xfers;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < rkmpcm->mdais->num_dais; i++, dx++)
    {
        if (dx->chan)
        {
            ret = rt_device_control(rkmpcm->dma[i], RT_DEVICE_CTRL_DMA_START, dx);
            if (ret)
                return ret;
        }
    }

    return ret;
}

static rt_err_t rk_mpcm_stop(struct audio_pcm *pcm)
{
    struct rk_mpcm *rkmpcm = to_rkmpcm(pcm);
    struct rt_dma_transfer *dx = rkmpcm->dma_xfers;
    rt_err_t ret = RT_EOK;
    int i;

    for (i = 0; i < rkmpcm->mdais->num_dais; i++, dx++)
    {
        if (dx->chan)
        {
            ret = rt_device_control(rkmpcm->dma[i], RT_DEVICE_CTRL_DMA_STOP, dx);
            if (ret)
                return ret;
        }
    }

    return ret;
}

static const struct audio_pcm_ops rk_mpcm_ops =
{
    .init = rk_mpcm_init,
    .deinit = rk_mpcm_deinit,
    .config = rk_mpcm_config,
    .start = rk_mpcm_start,
    .stop = rk_mpcm_stop,
};

struct audio_pcm *rk_audio_new_mpcm(struct audio_card *card)
{
    struct rk_mpcm *rkmpcm;

    rkmpcm = rt_calloc(1, sizeof(*rkmpcm));
    if (!rkmpcm)
        return RT_NULL;

    rkmpcm->mdais = to_mdais(card->dai);
    rkmpcm->pcm.ops = &rk_mpcm_ops;
    rkmpcm->pcm.card = card;

    rkmpcm->pcm.wait = rt_sem_create(card->desc.name, 0, RT_IPC_FLAG_FIFO);
    if (!rkmpcm->pcm.wait)
    {
        rt_free(rkmpcm);
        return RT_NULL;
    }

    return &rkmpcm->pcm;
}

void rk_audio_free_mpcm(struct audio_pcm *pcm)
{
    struct rk_mpcm *rkmpcm = to_rkmpcm(pcm);

    rt_sem_release(rkmpcm->pcm.wait);
    rt_free(rkmpcm);
}

#endif
