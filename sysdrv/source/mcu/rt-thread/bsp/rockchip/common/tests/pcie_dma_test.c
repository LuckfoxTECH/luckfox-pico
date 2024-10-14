/**
  * Copyright (c) 2023 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    pcie_dma_test.c
  * @version V1.0
  * @brief   QPI Psram test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2023-01-04     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_PCIE_DMA

#include "drv_pcie_dma.h"
#include "hal_base.h"
#include "hal_bsp.h"

static struct rk_pcie_dma_device *dma = RT_NULL;

#define PCIE_DMA_BUFFER_LIMIT  0x100000

static void pcie_dma_test_show_usage()
{
    rt_kprintf("1. pcie_dma_test from_bus chn local remote size loop\n");
    rt_kprintf("2. pcie_dma_test to_bus chn local remote size loop\n");
    rt_kprintf("like:\n");
    rt_kprintf("\tpcie_dma_test from_bus 0 0x3C000000 0x3C000000 4096 1\n");
    rt_kprintf("\tpcie_dma_test to_bus 0 0x3C001000 0x3C001000 4096 1\n");
    rt_kprintf("\tpcie_dma_test from_bus 1 0x3C100000 0x3C100000 4096 1\n");
    rt_kprintf("\tpcie_dma_test to_bus 1 0x3C101000 0x3C101000 4096 1\n");
}

HAL_Status pcie_dma_dbg_hex(char *s, void *buf, uint32_t width, uint32_t len)
{
    uint32_t i, j;
    unsigned char *p8 = (unsigned char *)buf;
    unsigned short *p16 = (unsigned short *)buf;
    uint32_t *p32 = (uint32_t *)buf;

    j = 0;
    for (i = 0; i < len; i++)
    {
        if (j == 0)
        {
            rt_kprintf("[SNOR TEST] %s %p + 0x%lx:", s, buf, i * width);
        }

        if (width == 4)
            rt_kprintf("0x%08lx,", p32[i]);
        else if (width == 2)
            rt_kprintf("0x%04x,", p16[i]);
        else
            rt_kprintf("0x%02x,", p8[i]);

        if (++j >= 16)
        {
            j = 0;
            rt_kprintf("\n");
        }
    }
    rt_kprintf("\n");

    return HAL_OK;
}

int pcie_dma_simple_test(u32 chn, u32 local, u32 remote, u32 dir, u32 size, u32 loop)
{
    struct dma_table *table;
    struct rk_pcie_dma_transfer *transfer;
    int i, ret;

    rt_kprintf("%s %d local=%x remote=%x dir=%d size=%d\n", __func__, chn, local, remote, dir, size);

    dma = (struct rk_pcie_dma_device *)rt_device_find("pcie_dma");
    if (dma == RT_NULL)
    {
        rt_kprintf("Did not find device: pcie_dma....\n");
        return -RT_EINVAL;
    }

    table = rt_malloc(sizeof(struct dma_table));
    if (!table)
    {
        rt_kprintf("%s table malloc failed\n", __func__);
        return -RT_EINVAL;
    }

    transfer = rt_malloc(sizeof(struct rk_pcie_dma_transfer));
    if (!transfer)
    {
        rt_kprintf("%s transfer malloc failed\n", __func__);
        rt_free(table);
        return -RT_EINVAL;
    }

    transfer->dma_req_num = chn;
    ret = rk_pcie_dma_requst_channel(dma, transfer);
    if (ret)
    {
        rt_kprintf("%s request channel failed %d\n", __func__, ret);
        goto out;
    }

    for (i = 0; i < loop; i++)
    {
#ifdef RT_USING_CACHE
        if (dir == DMA_TO_BUS)
        {
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)local, size);
        }
#endif

        memset(table, 0, sizeof(struct dma_table));
        table->buf_size = size;
        table->bus = remote;
        table->local = local;
        table->chn = chn;
        table->dir = dir;
        rk_pcie_dma_prepare(dma, transfer, table);
        rk_pcie_dma_start(dma, transfer);
        rk_pcie_dma_wait_for_complete(dma, transfer, RT_UINT32_MAX);

#ifdef RT_USING_CACHE
        if (dir == DMA_FROM_BUS)
        {
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)local, size);
        }
#endif
    }

    rk_pcie_dma_release_channel(dma, transfer);

out:
    rt_free(table);
    rt_free(transfer);

    return ret;
}

void pcie_dma_test(int argc, char **argv)
{
    char *cmd, *ptr;
    uint32_t chn, loop, start_time, end_time, cost_time;
    rt_off_t local, remote;
    rt_uint32_t size = 0;

    if (argc < 3)
        goto out;

    cmd = argv[1];
    if (!rt_strcmp(cmd, "to_bus"))
    {
        if (argc != 7)
            goto out;
        chn = atoi(argv[2]);
        local = strtoul(argv[3], &ptr, 0);
        remote = strtoul(argv[4], &ptr, 0);
        size = strtoul(argv[5], &ptr, 0);
        loop = atoi(argv[6]);

        if (chn > DMA_REQ_DMA_CHAN_MAX)
        {
            rt_kprintf("chn %d is out of DMA_REQ_DMA_CHAN_MAX\n", chn);
            return;
        }

        if (size > PCIE_DMA_BUFFER_LIMIT)
        {
            rt_kprintf("write size 0x%lx, limit \n", size, PCIE_DMA_BUFFER_LIMIT);
            return;
        }

        start_time = HAL_GetTick();
        pcie_dma_simple_test(chn, local, remote, DMA_TO_BUS, size, loop);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */
        rt_kprintf("pcie_dma write speed: %ld KB/s\n", size * loop / cost_time);
    }
    else if (!rt_strcmp(cmd, "from_bus"))
    {
        if (argc != 7)
            goto out;
        chn = atoi(argv[2]);
        local = strtoul(argv[3], &ptr, 0);
        remote = strtoul(argv[4], &ptr, 0);
        size = strtoul(argv[5], &ptr, 0);
        loop = atoi(argv[6]);

        if (chn > DMA_REQ_DMA_CHAN_MAX)
        {
            rt_kprintf("chn %d is out of DMA_REQ_DMA_CHAN_MAX\n", chn);
            return;
        }

        if (size > PCIE_DMA_BUFFER_LIMIT)
        {
            rt_kprintf("write size 0x%lx, limit \n", size, PCIE_DMA_BUFFER_LIMIT);
            return;
        }

        start_time = HAL_GetTick();
        pcie_dma_simple_test(chn, local, remote, DMA_FROM_BUS, size, loop);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */
        rt_kprintf("pcie_dma write speed: %ld KB/s\n", size * loop / cost_time);
    }
    else
    {

        goto out;
    }

    return;
out:
    pcie_dma_test_show_usage();
    return;
}

MSH_CMD_EXPORT(pcie_dma_test, spt test cmd);

#endif
