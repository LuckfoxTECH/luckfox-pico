/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dma_test.c
  * @author  sugar zhang
  * @version V0.1
  * @date    10-Feb-2019
  * @brief   dma test for pisces
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>

#if defined(RT_USING_DMA)

#include <stdint.h>
#include "hal_base.h"
#include "dma.h"

#define DMA_BYTES 4096
#define COUNTS    1000

static struct rt_device *dma;
static rt_sem_t mem_sem;
static rt_sem_t tx_sem;
static rt_sem_t rx_sem;

#ifdef DMA_MEMTODEV_TEST
static void tx_complete(void *param)
{
    rt_sem_release(tx_sem);
}

static void rx_complete(void *param)
{
    rt_sem_release(rx_sem);
}
#endif

static void m2m_complete(void *param)
{
    rt_sem_release(mem_sem);
}

void dma_test(void)
{
    struct rt_dma_transfer *m2m_xfer;
    struct rt_dma_transfer *tx_xfer;
    struct rt_dma_transfer *rx_xfer;
    int *src_mem, *dst_mem;
    rt_err_t ret;
    int i, len;
    uint32_t tick_s, tick_e; /* ms */

    m2m_xfer = (struct rt_dma_transfer *)rt_malloc(sizeof(*m2m_xfer));
    tx_xfer = (struct rt_dma_transfer *)rt_malloc(sizeof(*tx_xfer));
    rx_xfer = (struct rt_dma_transfer *)rt_malloc(sizeof(*rx_xfer));
    src_mem = rt_dma_malloc(DMA_BYTES);
    dst_mem = rt_dma_malloc(DMA_BYTES);
    mem_sem = rt_sem_create("memSem", 0, RT_IPC_FLAG_FIFO);
    tx_sem = rt_sem_create("txSem", 0, RT_IPC_FLAG_FIFO);
    rx_sem = rt_sem_create("rxSem", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(m2m_xfer != RT_NULL);
    RT_ASSERT(tx_xfer != RT_NULL);
    RT_ASSERT(rx_xfer != RT_NULL);
    RT_ASSERT(src_mem != RT_NULL);
    RT_ASSERT(dst_mem != RT_NULL);
    RT_ASSERT(mem_sem != RT_NULL);
    RT_ASSERT(tx_sem != RT_NULL);
    RT_ASSERT(rx_sem != RT_NULL);

    len = DMA_BYTES / sizeof(int);
    for (i = 0; i < len; i++)
        src_mem[i] = len - i;
    rt_memset(dst_mem, 0x0, DMA_BYTES);
    HAL_DCACHE_CleanInvalidateByRange((uint32_t)src_mem, DMA_BYTES);
    HAL_DCACHE_CleanInvalidateByRange((uint32_t)dst_mem, DMA_BYTES);

    dma = rt_device_find("dmac0");

    /* memcpy test */
    rt_memset(m2m_xfer, 0x0, sizeof(*m2m_xfer));
    m2m_xfer->direction = RT_DMA_MEM_TO_MEM;
    m2m_xfer->dst_addr = (rt_uint32_t)dst_mem;
    m2m_xfer->src_addr = (rt_uint32_t)src_mem;
    m2m_xfer->len = DMA_BYTES;
    m2m_xfer->callback = m2m_complete;
    m2m_xfer->cparam = m2m_xfer;
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL, m2m_xfer);
    RT_ASSERT(ret == RT_EOK);

    tick_s = HAL_GetTick();
    for (i = 0; i < COUNTS; i++)
    {
        ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_SINGLE_PREPARE, m2m_xfer);
        RT_ASSERT(ret == RT_EOK);
        ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_START, m2m_xfer);
        RT_ASSERT(ret == RT_EOK);
        /* wait for complete */
        while (rt_sem_trytake(mem_sem) != RT_EOK)
            HAL_DelayUs(1);
        ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_STOP, m2m_xfer);
        RT_ASSERT(ret == RT_EOK);
    }
    tick_e = HAL_GetTick();

    ret = rt_memcmp(src_mem, dst_mem, DMA_BYTES);

    rt_kprintf("dma memcpy [%s]: avg: %7u KB/S with src: 0x%x dst: 0x%x len: %u counts: %u\n",
               ret ? "FAIL" : "PASS", (DMA_BYTES / 1024) * COUNTS * 1000 / (tick_e - tick_s),
               src_mem, dst_mem, DMA_BYTES, COUNTS);

    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL, m2m_xfer);
    RT_ASSERT(ret == RT_EOK);

    tick_s = HAL_GetTick();
    for (i = 0; i < COUNTS; i++)
        rt_memcpy(dst_mem, src_mem, DMA_BYTES);
    tick_e = HAL_GetTick();

    ret = rt_memcmp(dst_mem, src_mem, DMA_BYTES);

    rt_kprintf("cpu memcpy [%s]: avg: %7u KB/S with src: 0x%x dst: 0x%x len: %u counts: %u\n",
               ret ? "FAIL" : "PASS", (DMA_BYTES / 1024) * COUNTS * 1000 / (tick_e - tick_s),
               src_mem, dst_mem, DMA_BYTES, COUNTS);

#ifdef DMA_MEMTODEV_TEST
    /* mem to dev test i2s0 loop tx--->rx */
    tx_xfer->direction = RT_DMA_MEM_TO_DEV;
    tx_xfer->dma_req_num = DMA_REQ_I2S0_TX;
    tx_xfer->dst_addr = I2S0_BASE + 0x24;
    tx_xfer->dst_addr_width = RT_DMA_SLAVE_BUSWIDTH_4_BYTES;
    tx_xfer->dst_maxburst = 4;
    tx_xfer->src_addr = (rt_uint32_t)src_mem;
    tx_xfer->src_addr_width = RT_DMA_SLAVE_BUSWIDTH_4_BYTES;
    tx_xfer->src_maxburst = 4;
    tx_xfer->len = DMA_BYTES;
    tx_xfer->callback = tx_complete;
    tx_xfer->cparam = tx_xfer;

    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL, tx_xfer);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_SINGLE_PREPARE, tx_xfer);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_START, tx_xfer);
    RT_ASSERT(ret == RT_EOK);

    rt_memset(dst_mem, 0x0, DMA_BYTES);
    rx_xfer->direction = RT_DMA_DEV_TO_MEM;
    rx_xfer->dma_req_num = DMA_REQ_I2S0_RX;
    rx_xfer->src_addr = I2S0_BASE + 0x28;
    rx_xfer->src_addr_width = RT_DMA_SLAVE_BUSWIDTH_4_BYTES;
    rx_xfer->src_maxburst = 4;
    rx_xfer->dst_addr = (rt_uint32_t)dst_mem;
    rx_xfer->dst_addr_width = RT_DMA_SLAVE_BUSWIDTH_4_BYTES;
    rx_xfer->dst_maxburst = 4;
    rx_xfer->len = DMA_BYTES;
    rx_xfer->callback = rx_complete;
    rx_xfer->cparam = rx_xfer;

    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_REQUEST_CHANNEL, rx_xfer);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_SINGLE_PREPARE, rx_xfer);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_START, rx_xfer);
    RT_ASSERT(ret == RT_EOK);
    /* TBD */
    I2S0->DMACR = 0x010f010f;
    I2S0->XFER = 0x3;
    rt_sem_take(tx_sem, RT_WAITING_FOREVER);
    rt_sem_take(rx_sem, RT_WAITING_FOREVER);
    ret = rt_memcmp(src_mem, dst_mem, DMA_BYTES);
    RT_ASSERT(ret == 0);

    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_STOP, tx_xfer);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL, tx_xfer);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_STOP, rx_xfer);
    RT_ASSERT(ret == RT_EOK);
    ret = rt_device_control(dma, RT_DEVICE_CTRL_DMA_RELEASE_CHANNEL, rx_xfer);
    RT_ASSERT(ret == RT_EOK);
#endif
    rt_sem_delete(mem_sem);
    rt_sem_delete(tx_sem);
    rt_sem_delete(rx_sem);
    rt_free(m2m_xfer);
    rt_free(tx_xfer);
    rt_free(rx_xfer);
    rt_dma_free(src_mem);
    rt_dma_free(dst_mem);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(dma_test, dma drive test. e.g: dma_test);
#endif

#endif
