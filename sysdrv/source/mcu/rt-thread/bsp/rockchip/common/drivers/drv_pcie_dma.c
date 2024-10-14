/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pcie_dma.c
  * @version V1.0
  * @brief   pcie_dma interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-08-30     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup PCIE_DMA
 *  @{
 */

/** @defgroup PCIE_DMA_How_To_Use How To Use
 *  @{

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "hal_bsp.h"
#include "drv_pcie_dma.h"

#ifdef RT_USING_PCIE_DMA

/********************* Private MACRO Definition ******************************/

// #define PCI_DMA_DEBUG
#ifdef PCI_DMA_DEBUG
#define PCI_DMA_DBG(...)     rt_kprintf(__VA_ARGS__)
#else
#define PCI_DMA_DBG(...)
#endif

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ****************************/

/** @defgroup PCIE_DMA_Public_Functions Public Functions
 *  @{
 */

rt_err_t rk_pcie_dma_requst_channel(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer)
{
    struct rk_pcie_dma_chan *chan;

    PCI_DMA_DBG("%s %d\n", __func__, __LINE__);

    if (transfer->dma_req_num >= DMA_REQ_DMA_CHAN_MAX)
    {
        rt_kprintf("%s wrong req num %d\n", __func__, transfer->dma_req_num);
        return -RT_EINVAL;
    }

    chan = &(dma->chan[transfer->dma_req_num]);
    if (chan->used)
    {
        rt_kprintf("%s chan %d is in use\n", __func__, transfer->dma_req_num);
        return -RT_EINVAL;
    }

    transfer->chan = chan;
    chan->used = RT_TRUE;

    return 0;
}

rt_err_t rk_pcie_dma_release_channel(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer)
{
    struct rk_pcie_dma_chan *chan;

    PCI_DMA_DBG("%s %d\n", __func__, __LINE__);

    chan = &(dma->chan[transfer->dma_req_num]);
    chan->used = RT_FALSE;

    return 0;
}

rt_err_t rk_pcie_dma_prepare(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer, struct DMA_TABLE *table)
{
    PCI_DMA_DBG("%s %d\n", __func__, __LINE__);

    transfer->chan->cur = table;
    dma->ops->config_dma_func(dma, table);

    return 0;
}

rt_err_t rk_pcie_dma_start(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer)
{
    PCI_DMA_DBG("%s %d %p\n", __func__, __LINE__, transfer->chan->cur);

    dma->ops->start_dma_func(dma, transfer->chan->cur);

    return 0;
}

rt_err_t rk_pcie_dma_wait_for_complete(struct rk_pcie_dma_device *dma, struct rk_pcie_dma_transfer *transfer, uint32_t timeout_us)
{
    PCI_DMA_DBG("%s %d\n", __func__, __LINE__);

    return dma->ops->wait_for_finished(dma, transfer->chan->cur, timeout_us);
}

struct rk_pcie_dma_device *rk_pcie_dma_register(void *priv_data, struct rk_pcie_dma_ops *ops)
{
    rt_err_t ret = -RT_ERROR;
    struct rk_pcie_dma_device *dev = RT_NULL;
    int i;

    if (!priv_data)
        return NULL;

    dev = rt_malloc(sizeof(struct rk_pcie_dma_device));
    if (!dev)
        return NULL;

    dev->parent.type = RT_Device_Class_Char;
    dev->parent.user_data = priv_data;
    dev->ops = ops;
    for (i = 0; i < DMA_REQ_DMA_CHAN_MAX; i ++)
    {
        dev->chan[i].device = dev;
        dev->chan[i].chan_id = i;
        dev->chan[i].used = RT_FALSE;
    }

    if (rt_mutex_init(&dev->lock, "pcie_dma_lock", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("Init pcie dma mutex error\n");
        RT_ASSERT(0);
    }

    ret = rt_device_register(&dev->parent, "pcie_dma", RT_DEVICE_FLAG_STANDALONE);
    if (ret != RT_EOK)
    {
        rt_free(dev);
        return NULL;
    }

    return dev;
}

/** @} */  // PCIE_DMA_Public_Function

#endif

/** @} */  // PCIE_DMA

/** @} */  // RKBSP_Common_Driver
