/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    psram_test.c
  * @version V1.0
  * @brief   QPI Psram test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-04-30     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_PSRAM

#include "drv_psram.h"
#include "hal_base.h"
#include "hal_bsp.h"

struct rk_psram_device *psram_device = RT_NULL;

#define PSRAM_BUFFER_LIMIT  0x100000

static void psram_test_show_usage()
{
    rt_kprintf("1. psram_test dev write offset size loop\n");
    rt_kprintf("2. psram_test dev read offset size loop\n");
    rt_kprintf("3. psram_test dev stress offset size loop\n");
    rt_kprintf("4. psram_test dev suspend(code should be place in sram)\n");
    rt_kprintf("5. psram_test dev resume(code should be place in sram)\n");
    rt_kprintf("like:\n");
    rt_kprintf("\tpsram_test psram write 2097152 4096 2000\n");
    rt_kprintf("\tpsram_test psram read 2097152 4096 2000\n");
    rt_kprintf("\tpsram_test psram stress 2097152 2097152 5000\n");
    rt_kprintf("\tpsram_test psram suspend\n");
    rt_kprintf("\tpsram_test psram resume\n");
}

HAL_Status psram_dbg_hex(char *s, void *buf, uint32_t width, uint32_t len)
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

static uint32_t psram_stress_test(uint32_t from, uint32_t size, uint32_t loop)
{
    int32_t ret, i, j;
    uint32_t test_lba;
    rt_uint8_t *txbuf = NULL, *rxbuf = NULL;
    rt_uint32_t *pwrite32 = NULL, *pread32 = NULL;
    rt_uint32_t test_begin_lba, test_end_lba;
    rt_uint32_t block_size = 0x1000;

    test_begin_lba = from / block_size;
    test_end_lba = (from + size) / block_size;
    txbuf = (rt_uint8_t *)rt_malloc_align(block_size, 64);
    rxbuf = (rt_uint8_t *)rt_malloc_align(block_size, 64);
    RT_ASSERT(txbuf);
    RT_ASSERT(rxbuf);

    pwrite32 = (rt_uint32_t *)txbuf;
    pread32 = (rt_uint32_t *)rxbuf;
    for (int32_t i = 0; i < (block_size / 4); i++)
        pwrite32[i] = i;

    rt_kprintf("---------%s from LBA 0x%lx to 0x%lx---------\n", __func__, test_begin_lba, test_end_lba);
    for (i = 0; i < loop; i++)
    {
        rt_kprintf("---------%s loop %d---------\n", __func__, i);
        for (test_lba = test_begin_lba; test_lba < test_end_lba; test_lba++)
        {
            pwrite32[0] = test_lba;
            ret = rk_psram_write(psram_device, test_lba * block_size, (const rt_uint8_t *)pwrite32, block_size);
            if (ret != block_size)
                goto out;
            pread32[0] = -1;
            ret = rk_psram_read(psram_device, test_lba * block_size, (rt_uint8_t *)pread32, block_size);
            if (ret != block_size)
                goto out;
            for (j = 0; j < (int32_t)block_size / 4; j++)
            {
                if (pwrite32[j] != pread32[j])
                {
                    psram_dbg_hex("w:", pwrite32, 4, 256);
                    psram_dbg_hex("r:", pread32, 4, 256);
                    ret = rk_psram_read(psram_device, test_lba * block_size, (rt_uint8_t *)pread32, block_size);
                    psram_dbg_hex("r1:", pread32, 4, 256);
                    rt_kprintf("check not match:row=0x%lx, num=0x%lx, write=0x%lx, read=0x%lx 0x%lx 0x%lx 0x%lx\n",
                               test_lba, j, pwrite32[j], pread32[j], pread32[j + 1], pread32[j + 2], pread32[j - 1]);

                    ret = -1;
                    goto out;
                }
            }
            rt_kprintf("test_lba= 0x%lx\n", test_lba);
        }

        rt_kprintf("---------%s recheck %d---------\n", __func__);
        for (test_lba = test_begin_lba; test_lba < test_end_lba; test_lba++)
        {
            pwrite32[0] = test_lba;
            pread32[0] = -1;
            ret = rk_psram_read(psram_device, test_lba * block_size, (rt_uint8_t *)pread32, block_size);
            if (ret != block_size)
                goto out;
            for (j = 0; j < (int32_t)block_size / 4; j++)
            {
                if (pwrite32[j] != pread32[j])
                {
                    psram_dbg_hex("w:", pwrite32, 4, 16);
                    psram_dbg_hex("r:", pread32, 4, 16);
                    rt_kprintf("recheck not match:row=0x%lx, num=0x%lx, write=0x%lx, read=0x%lx 0x%lx 0x%lx 0x%lx\n",
                               test_lba, j, pwrite32[j], pread32[j], pread32[j + 1], pread32[j + 2], pread32[j - 1]);

                    goto out;
                }
            }
            rt_kprintf("recheck_lba= 0x%lx\n", test_lba);
        }
    }
    ret = 0;

    rt_kprintf("---------%s SUCCESS---------\n", __func__);

out:
    rt_free_align(txbuf);
    rt_free_align(rxbuf);

    return ret;
}

void psram_test(int argc, char **argv)
{
    char *name, *cmd;
    rt_uint8_t *txbuf = NULL, *rxbuf = NULL;
    uint32_t loop, start_time, end_time, cost_time;
    rt_off_t offset = 0;
    rt_uint32_t size = 0;
    int i;

    if (argc < 3)
        goto out;

    name = argv[1];
    psram_device = (struct rk_psram_device *)rt_device_find(name);
    if (psram_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: psram....\n");
        return;
    }

    cmd = argv[2];
    if (!rt_strcmp(cmd, "write"))
    {
        if (argc != 6)
            goto out;
        offset = atoi(argv[3]);
        size = atoi(argv[4]);
        loop = atoi(argv[5]);

        if (size > PSRAM_BUFFER_LIMIT)
        {
            rt_kprintf("write size 0x%lx, limit \n", size, PSRAM_BUFFER_LIMIT);
            return;
        }

        if ((offset + size) > psram_device->size)
        {
            rt_kprintf("write over: 0x%lx 0x%lx\n", (offset + size), psram_device->size);
            return;
        }

        txbuf = (rt_uint8_t *)rt_malloc_align(size, 64);
        if (!txbuf)
        {
            rt_kprintf("spi write alloc buf size %d fail\n", size);
            return;
        }

        for (i = 0; i < size; i++)
            txbuf[i] = i % 256;

        start_time = HAL_GetTick();
        for (i = 0; i < loop; i++)
            rk_psram_write(psram_device, offset, (const rt_uint8_t *)txbuf, size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        psram_dbg_hex("write: ", txbuf, 4, 16);
        rt_kprintf("psram write speed: %ld KB/s\n", size * loop / cost_time);

        rt_free_align(txbuf);
    }
    else if (!rt_strcmp(cmd, "read"))
    {

        if (argc != 6)
            goto out;
        offset = atoi(argv[3]);
        size = atoi(argv[4]);
        loop = atoi(argv[5]);

        if (size > PSRAM_BUFFER_LIMIT)
        {
            rt_kprintf("read size 0x%lx, limit \n", size, PSRAM_BUFFER_LIMIT);
            return;
        }

        if ((offset + size) > psram_device->size)
        {
            rt_kprintf("read over: 0x%lx 0x%lx\n", (offset + size), psram_device->size);
            return;
        }

        rxbuf = (rt_uint8_t *)rt_malloc_align(size, 64);
        if (!rxbuf)
        {
            rt_kprintf("spi write alloc buf size %d fail\n", size);
            return;
        }

        rt_memset(rxbuf, 0, size);;

        start_time = HAL_GetTick();
        for (i = 0; i < loop; i++)
            rk_psram_read(psram_device, offset, rxbuf, size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        psram_dbg_hex("read: ", rxbuf, 4, 16);
        rt_kprintf("psram read speed: %ld KB/s\n", size * loop / cost_time);

        rt_free_align(rxbuf);
    }
    else if (!rt_strcmp(cmd, "xip_read"))
    {

        if (argc != 6)
            goto out;
        offset = atoi(argv[3]);
        size = atoi(argv[4]);
        loop = atoi(argv[5]);

        if (size > PSRAM_BUFFER_LIMIT)
        {
            rt_kprintf("read size 0x%lx, limit \n", size, PSRAM_BUFFER_LIMIT);
            return;
        }

        if ((offset + size) > psram_device->size)
        {
            rt_kprintf("read over: 0x%lx 0x%lx\n", (offset + size), psram_device->size);
            return;
        }

        rxbuf = (rt_uint8_t *)rt_malloc_align(size, 64);
        if (!rxbuf)
        {
            rt_kprintf("spi write alloc buf size %d fail\n", size);
            return;
        }

        rt_memset(rxbuf, 0, size);;

        start_time = HAL_GetTick();
        for (i = 0; i < loop; i++)
        {
#ifdef RT_USING_CACHE
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (uint32_t *)(g_fspi1Dev.xipMemData + offset), size);
#endif
            rt_memcpy(rxbuf, (uint32_t *)(g_fspi1Dev.xipMemData + offset), size);
        }
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        psram_dbg_hex("read: ", rxbuf, 4, 16);
        rt_kprintf("psram read speed: %ld KB/s, tick count= %d， from %p to %p \n", size * loop / cost_time, cost_time, (uint32_t *)(g_fspi1Dev.xipMemData + offset), rxbuf);

        rt_free_align(rxbuf);
    }
    else if (!rt_strcmp(cmd, "stress"))
    {
        if (argc != 6)
            goto out;
        offset = atoi(argv[3]);
        size = atoi(argv[4]);
        loop = atoi(argv[5]);

        if ((offset + size) > psram_device->size)
        {
            rt_kprintf("test over: 0x%lx 0x%lx\n", (offset + size), psram_device->size);
            return;
        }

        psram_stress_test(offset, size, loop);
    }
    else if (!rt_strcmp(cmd, "suspend"))
    {
        rk_psram_suspend();
    }
    else if (!rt_strcmp(cmd, "resume"))
    {
        rk_psram_resume();
    }
    else
    {

        goto out;
    }

    return;
out:
    psram_test_show_usage();
    return;
}

MSH_CMD_EXPORT(psram_test, spt test cmd);

#endif
