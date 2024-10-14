/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pcm.c
  * @author  sugar zhang
  * @version V0.1
  * @date    1-Mar-2019
  * @brief   pcm driver for rk soc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_PCM

#include "dma.h"
#include "rk_audio.h"
#include "hal_base.h"

#define PCM_BOUNDARY_MAX    (0x7FFFFFFFU)

struct rk_pcm
{
    struct audio_pcm pcm;
    struct rt_device *dma;
    struct rt_dma_transfer dma_xfer;
};

static inline struct rk_pcm *to_rkpcm(struct audio_pcm *pcm)
{
    return HAL_CONTAINER_OF(pcm, struct rk_pcm, pcm);
}

static void rk_pcm_complete(void *arg)
{
    struct audio_pcm *pcm = (struct audio_pcm *)arg;

    rk_audio_stream_update(pcm->as);
}

static rt_err_t rk_pcm_init(struct audio_pcm *pcm,
                            eAUDIO_streamType stream,
                            struct audio_buf *buf)
{
    struct rk_pcm *rkpcm = to_rkpcm(pcm);
    const struct AUDIO_DMA_DATA *dmaData = pcm->card->dai->dmaData[stream];
    struct rt_dma_transfer *dx = &rkpcm->dma_xfer;
    rt_err_t ret;

    rkpcm->pcm.abuf = *buf;
    pcm->boundary = pcm->abuf.buf_size;

    while (pcm->boundary << 1 <= PCM_BOUNDARY_MAX - pcm->abuf.buf_size)
        pcm->boundary <<= 1;

    rkpcm->dma = rt_dma_get((uint32_t)dmaData->dmac);
    RT_ASSERT(rkpcm->dma);
    dx->dma_req_num = dmaData->dmaReqCh;

    ret = rt_device_control(rkpcm->dma, RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL, dx);
    RT_ASSERT(ret == RT_EOK);

    return ret;
}

static rt_err_t rk_pcm_deinit(struct audio_pcm *pcm)
{
    struct rk_pcm *rkpcm = to_rkpcm(pcm);
    struct rt_dma_transfer *dx = &rkpcm->dma_xfer;
    rt_err_t ret;

    ret = rt_device_control(rkpcm->dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL, dx);

    return ret;
}

static rt_err_t rk_pcm_config(struct audio_pcm *pcm, eAUDIO_streamType stream,
                              struct AUDIO_PARAMS *params)
{
    struct rk_pcm *rkpcm = to_rkpcm(pcm);
    const struct AUDIO_DMA_DATA *dmaData = pcm->card->dai->dmaData[stream];
    struct rt_dma_transfer *dx = &rkpcm->dma_xfer;
    struct audio_buf *buf = &rkpcm->pcm.abuf;
    rt_err_t ret;

    RT_ASSERT(buf->buf_size);
    RT_ASSERT(buf->period_size);

    rkpcm->pcm.params = *params;
    dx->len = frames_to_bytes(pcm, buf->buf_size);
    dx->period_len = frames_to_bytes(pcm, buf->period_size);
    if (stream == AUDIO_STREAM_PLAYBACK)
    {
        dx->direction = RT_DMA_MEM_TO_DEV;
        dx->src_addr = (rt_uint32_t)buf->buf;
        dx->dst_addr = dmaData->addr;
        dx->dst_addr_width = dmaData->addrWidth;
        dx->dst_maxburst = dmaData->maxBurst;
    }
    else
    {
        dx->direction = RT_DMA_DEV_TO_MEM;
        dx->src_addr = dmaData->addr;
        dx->src_addr_width = dmaData->addrWidth;
        dx->src_maxburst = dmaData->maxBurst;
        dx->dst_addr = (rt_uint32_t)buf->buf;
    }

    dx->callback = rk_pcm_complete;
    dx->cparam = pcm;

    ret = rt_device_control(rkpcm->dma, RT_DEVICE_CTRL_DMA_CYCLIC_PREPARE, dx);
    RT_ASSERT(ret == RT_EOK);

    return ret;
}

static rt_err_t rk_pcm_start(struct audio_pcm *pcm)
{
    struct rk_pcm *rkpcm = to_rkpcm(pcm);
    struct rt_dma_transfer *dx = &rkpcm->dma_xfer;

    return rt_device_control(rkpcm->dma, RT_DEVICE_CTRL_DMA_START, dx);
}

static rt_err_t rk_pcm_stop(struct audio_pcm *pcm)
{
    struct rk_pcm *rkpcm = to_rkpcm(pcm);
    struct rt_dma_transfer *dx = &rkpcm->dma_xfer;

    return rt_device_control(rkpcm->dma, RT_DEVICE_CTRL_DMA_STOP, dx);
}

static const struct audio_pcm_ops rk_pcm_ops =
{
    .init = rk_pcm_init,
    .deinit = rk_pcm_deinit,
    .config = rk_pcm_config,
    .start = rk_pcm_start,
    .stop = rk_pcm_stop,
};

struct audio_pcm *rk_audio_new_pcm(struct audio_card *card)
{
    struct rk_pcm *rkpcm;

    rkpcm = rt_calloc(1, sizeof(*rkpcm));
    if (!rkpcm)
        return RT_NULL;

    rkpcm->pcm.ops = &rk_pcm_ops;
    rkpcm->pcm.card = card;

    rkpcm->pcm.wait = rt_sem_create(card->desc.name, 0, RT_IPC_FLAG_FIFO);
    if (!rkpcm->pcm.wait)
    {
        rt_free(rkpcm);
        return RT_NULL;
    }

    return &rkpcm->pcm;
}

void rk_audio_free_pcm(struct audio_pcm *pcm)
{
    struct rk_pcm *rkpcm = to_rkpcm(pcm);

    rt_sem_release(rkpcm->pcm.wait);
    rt_free(rkpcm);
}

#endif
