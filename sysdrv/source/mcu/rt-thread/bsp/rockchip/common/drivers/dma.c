/**
  * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dma.c
  * @author  Sugar Zhang
  * @version V0.1
  * @date    8-Feb-2019
  * @brief   general dma framework driver for rksoc
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>

#ifdef RT_USING_DMA

#include "dma.h"

#ifdef DMA_DEBUG
#define RT_DMA_DEBUG(fmt, args...)  rt_kprintf(fmt, ##args);
#else
#define RT_DMA_DEBUG(fmt, args...)
#endif

static uint8_t s_dma_index = 0;

static rt_err_t rk_dma_request_channel(struct rt_dma_device  *dma,
                                       struct rt_dma_transfer *xfer)
{
    rt_err_t ret = RT_EOK;
    struct rt_dma_chan *chan;

    chan = dma->request_channel(dma, xfer->dma_req_num);
    if (chan)
        xfer->chan = chan;
    else
        ret = RT_EFULL;

    return ret;
}

static rt_err_t rk_dma_release_channel(struct rt_dma_device *dma,
                                       struct rt_dma_transfer *xfer)
{
    struct rt_dma_chan *chan = xfer->chan;

    RT_ASSERT(chan);

    return dma->release_channel(chan);
}

static rt_err_t rk_dma_config_channel(struct rt_dma_device *dma,
                                      struct rt_dma_transfer *xfer)
{
    struct rt_dma_chan *chan = xfer->chan;
    struct dma_slave_config slave_config;

    RT_ASSERT(chan);

    slave_config.direction = xfer->direction;
    slave_config.src_addr = xfer->src_addr;
    slave_config.src_addr_width = xfer->src_addr_width;
    slave_config.src_maxburst = xfer->src_maxburst;
    slave_config.dst_addr = xfer->dst_addr;
    slave_config.dst_addr_width = xfer->dst_addr_width;
    slave_config.dst_maxburst = xfer->dst_maxburst;
    slave_config.src_interlace_size = xfer->src_interlace_size;
    slave_config.dst_interlace_size = xfer->dst_interlace_size;

    return dma->config(chan, &slave_config);
}

static rt_err_t rk_dma_single_prep(struct rt_dma_device *dma,
                                   struct rt_dma_transfer *xfer)
{
    struct rt_dma_chan *chan = xfer->chan;
    rt_err_t ret = RT_EOK;
    rt_uint32_t dma_addr;

    RT_ASSERT(chan);

    if (xfer->direction == RT_DMA_MEM_TO_DEV)
        dma_addr = xfer->src_addr;
    else
        dma_addr = xfer->dst_addr;

    ret = rk_dma_config_channel(dma, xfer);
    if (ret)
        return ret;

    switch (xfer->direction)
    {
    case RT_DMA_MEM_TO_MEM:
        dma->prep_dma_memcpy(chan, xfer->dst_addr, xfer->src_addr,
                             xfer->len, xfer->callback, xfer->cparam);
        break;
    case RT_DMA_MEM_TO_DEV:
    case RT_DMA_DEV_TO_MEM:
        dma->prep_dma_single(chan, dma_addr, xfer->len, xfer->direction,
                             xfer->callback, xfer->cparam);
        break;
    default:
        ret = RT_EINVAL;
        break;
    }

    return ret;
}

static rt_err_t rk_dma_get_position(struct rt_dma_device *dma,
                                    struct rt_dma_transfer *xfer)
{
    struct rt_dma_chan *chan = xfer->chan;

    RT_ASSERT(chan);

    xfer->position = dma->get_position(chan);

    return RT_EOK;
}

static rt_err_t rk_dma_stop(struct rt_dma_device *dma,
                            struct rt_dma_transfer *xfer)
{
    struct rt_dma_chan *chan = xfer->chan;

    RT_ASSERT(chan);

    return dma->stop(chan);
}

static rt_err_t rk_dma_start(struct rt_dma_device *dma,
                             struct rt_dma_transfer *xfer)
{
    struct rt_dma_chan *chan = xfer->chan;

    RT_ASSERT(chan);

    return dma->start(chan);
}

static rt_err_t rk_dma_cyclic_prep(struct rt_dma_device *dma,
                                   struct rt_dma_transfer *xfer)
{
    struct rt_dma_chan *chan = xfer->chan;
    rt_uint32_t dma_addr;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(chan);

    ret = rk_dma_config_channel(dma, xfer);
    if (ret)
        return ret;

    if (xfer->direction == RT_DMA_MEM_TO_DEV)
        dma_addr = xfer->src_addr;
    else
        dma_addr = xfer->dst_addr;

    return dma->prep_dma_cyclic(chan, dma_addr,
                                xfer->len, xfer->period_len,
                                xfer->direction,
                                xfer->callback, xfer->cparam);
}

static rt_err_t rt_dma_control(struct rt_device *dev, int cmd, void *arg)
{
    struct rt_dma_device *dma = (struct rt_dma_device *)dev->user_data;
    struct rt_dma_transfer *xfer = (struct rt_dma_transfer *)arg;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(dma);
    RT_ASSERT(xfer);

    RT_DMA_DEBUG("%s: cmd: %d\n", __func__, cmd);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL:
        ret = rk_dma_request_channel(dma, xfer);
        break;
    case RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL:
        ret = rk_dma_release_channel(dma, xfer);
        break;
    case RT_DEVICE_CTRL_DMA_SINGLE_PREPARE:
        ret = rk_dma_single_prep(dma, xfer);
        break;
    case RT_DEVICE_CTRL_DMA_CYCLIC_PREPARE:
        ret = rk_dma_cyclic_prep(dma, xfer);
        break;
    case RT_DEVICE_CTRL_DMA_START:
        ret = rk_dma_start(dma, xfer);
        break;
    case RT_DEVICE_CTRL_DMA_STOP:
        ret = rk_dma_stop(dma, xfer);
        break;
    case RT_DEVICE_CTRL_DMA_GET_POSITION:
        ret = rk_dma_get_position(dma, xfer);
        break;
    default:
        ret = RT_EINVAL;
        break;
    }

    return ret;
}

/**
 * This function return dma device by dma base addr
 */
rt_device_t rt_dma_get(uint32_t base)
{
    struct rt_dma_device *dma;
    char name[RT_NAME_MAX];
    int i;

    for (i = 0; i < s_dma_index; i++)
    {
        rt_snprintf(name, RT_NAME_MAX, "dmac%d", i);
        dma = (struct rt_dma_device *)rt_device_find(name);
        if (dma && (dma->base == base))
            return &dma->dev;
    }

    return NULL;
}

#ifdef RT_USING_DEVICE_OPS
static const struct rt_device_ops rt_dma_ops =
{
    .control = rt_dma_control,
};
#endif

/**
 * This function register a dma device
 */
rt_err_t rt_hw_dma_register(struct rt_dma_device *dma)
{
    struct rt_device *dev;
    char name[RT_NAME_MAX];

    RT_ASSERT(dma);

    rt_snprintf(name, RT_NAME_MAX, "dmac%d", s_dma_index++);

    dev = &(dma->dev);

    dev->type        = RT_Device_Class_Miscellaneous;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;
#ifdef RT_USING_DEVICE_OPS
    dev->ops         = &rt_dma_ops;
#else
    dev->init        = RT_NULL;
    dev->open        = RT_NULL;
    dev->close       = RT_NULL;
    dev->read        = RT_NULL;
    dev->write       = RT_NULL;
    dev->control     = rt_dma_control;
#endif
    dev->user_data   = dma;

    return rt_device_register(dev, name, RT_DEVICE_FLAG_RDWR);
}

void *rt_dma_malloc(uint32_t size)
{
    uint32_t align = 4;

#ifdef CACHE_LINE_SIZE
    align = CACHE_LINE_SIZE;
#endif

#ifdef DMA_ALIGN_SIZE
    align = align > DMA_ALIGN_SIZE ? align : DMA_ALIGN_SIZE;
#endif

    return rt_malloc_align(RT_ALIGN(size, align), align);
}

#endif
