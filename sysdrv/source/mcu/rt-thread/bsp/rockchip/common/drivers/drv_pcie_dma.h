/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pcie_dma.h
  * @version V1.0
  * @brief   pcie_dma interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-12-30     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#ifdef RT_USING_PCIE_DMA

#ifndef __DRV_PCIE_DMA_H__
#define __DRV_PCIE_DMA_H__

/**
 * data type
 */
typedef unsigned long  u32;
typedef unsigned short  u16;
typedef unsigned char  u8;

struct rk_pcie_dma_device;

struct rk_pcie_dma_ops
{
    void (*start_dma_func)(struct rk_pcie_dma_device *obj, struct DMA_TABLE *table);
    void (*config_dma_func)(struct rk_pcie_dma_device *obj, struct DMA_TABLE *table);
    int (*wait_for_finished)(struct rk_pcie_dma_device *obj, struct DMA_TABLE *table, u32 timeout_us);
};

struct rk_pcie_dma_chan
{
    struct rk_pcie_dma_device *device;
    struct DMA_TABLE *cur;
    u8 chan_id;
    rt_bool_t used;
};

struct rk_pcie_dma_device
{
    struct rt_device parent;

    /* Controller resource */
    struct rk_pcie_dma_chan chan[DMA_REQ_DMA_CHAN_MAX];

    /* DMA frameware */
    struct rk_pcie_dma_ops *ops;
    struct rt_mutex lock;

    void *priv;
};

struct rk_pcie_dma_transfer
{
    /* user's params */
    enum HAL_PCIE_DMA_CHAN dma_req_num; /* peri dma request num */

    struct rk_pcie_dma_chan *chan; /* don't touch this. */
};

rt_err_t rk_pcie_dma_requst_channel(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer);
rt_err_t rk_pcie_dma_release_channel(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer);
rt_err_t rk_pcie_dma_prepare(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer, struct DMA_TABLE *table);
rt_err_t rk_pcie_dma_start(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer);
rt_err_t rk_pcie_dma_wait_for_complete(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer, uint32_t timeout);
struct rk_pcie_dma_device *rk_pcie_dma_register(void *priv_data, struct rk_pcie_dma_ops *ops);

#endif
#endif
