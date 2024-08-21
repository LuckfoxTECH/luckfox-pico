/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    snor_test.c
  * @version V1.0
  * @brief   spi nor test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-08-27     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_SNOR

#include <drivers/mtd_nor.h>
#include "hal_base.h"
#include "drv_snor.h"

struct rt_mtd_nor_device *snor_device = RT_NULL;

#define SNOR_BUFFER_LIMIT  0x1000
#define TUNING_PATTERN_SIZE 0x40

static const uint8_t tuning_blk_pattern_4bit[TUNING_PATTERN_SIZE] =
{
    0xff, 0x0f, 0xff, 0x00, 0xff, 0xcc, 0xc3, 0xcc,
    0xc3, 0x3c, 0xcc, 0xff, 0xfe, 0xff, 0xfe, 0xef,
    0xff, 0xdf, 0xff, 0xdd, 0xff, 0xfb, 0xff, 0xfb,
    0xbf, 0xff, 0x7f, 0xff, 0x77, 0xf7, 0xbd, 0xef,
    0xff, 0xf0, 0xff, 0xf0, 0x0f, 0xfc, 0xcc, 0x3c,
    0xcc, 0x33, 0xcc, 0xcf, 0xff, 0xef, 0xff, 0xee,
    0xff, 0xfd, 0xff, 0xfd, 0xdf, 0xff, 0xbf, 0xff,
    0xbb, 0xff, 0xf7, 0xff, 0xf7, 0x7f, 0x7b, 0xde,
};

static void snor_test_show_usage()
{
    rt_kprintf("1. snor_test write offset size\n");
    rt_kprintf("2. snor_test read offset size loop\n");
    rt_kprintf("3. snor_test erase offset\n");
    rt_kprintf("4. snor_test stress offset size loop\n");
    rt_kprintf("5. snor_test io_stress offset size loop\n");
    rt_kprintf("6. snor_test pm_test\n");
    rt_kprintf("like:\n");
    rt_kprintf("\tsnor_test write 2097152 4096\n");
    rt_kprintf("\tsnor_test read 2097152 4096 2000\n");
    rt_kprintf("\tsnor_test erase 2097152\n");
    rt_kprintf("\tsnor_test stress 2097152 2097152 5000\n");
    rt_kprintf("\tsnor_test io_stress 2097152 2097152 5000\n");
    rt_kprintf("\tsnor_test pm_test\n");
}

HAL_Status snor_dbg_hex(char *s, void *buf, uint32_t width, uint32_t len)
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

static uint32_t snor_stress_test(uint32_t from, uint32_t size, uint32_t loop)
{
    int32_t ret, i, j;
    uint32_t test_lba;
    rt_uint8_t *txbuf = NULL, *rxbuf = NULL;
    rt_uint32_t *pwrite32 = NULL, *pread32 = NULL;
    rt_uint32_t test_begin_lba, test_end_lba;

    test_begin_lba = from / snor_device->block_size;
    test_end_lba = (from + size) / snor_device->block_size;
    txbuf = (rt_uint8_t *)rt_malloc_align(snor_device->block_size, 64);
    rxbuf = (rt_uint8_t *)rt_malloc_align(snor_device->block_size, 64);

    pwrite32 = (rt_uint32_t *)txbuf;
    pread32 = (rt_uint32_t *)rxbuf;
    for (int32_t i = 0; i < (snor_device->block_size / 4); i++)
        pwrite32[i] = i;

    rt_kprintf("---------%s from LBA 0x%lx to 0x%lx---------\n", __func__, test_begin_lba, test_end_lba);
    for (i = 0; i < loop; i++)
    {
        rt_kprintf("---------%s loop %d---------\n", __func__, i);
        for (test_lba = test_begin_lba; test_lba < test_end_lba; test_lba++)
        {
            pwrite32[0] = test_lba;
            rt_mtd_nor_erase_block(snor_device, test_lba * snor_device->block_size, snor_device->block_size);
            ret = rt_mtd_nor_write(snor_device, test_lba * snor_device->block_size, (const rt_uint8_t *)pwrite32, snor_device->block_size);
            if (ret != snor_device->block_size)
            {
                rt_kprintf("%s 0x%lx write fail\n", __func__, test_lba);
                while (1)
                    ;
            }
            pread32[0] = -1;
            ret = rt_mtd_nor_read(snor_device, test_lba * snor_device->block_size, (rt_uint8_t *)pread32, snor_device->block_size);
            if (ret != snor_device->block_size)
            {
                rt_kprintf("%s 0x%lx read fail\n", __func__, test_lba);
                while (1)
                    ;
            }
            for (j = 0; j < (int32_t)snor_device->block_size / 4; j++)
            {
                if (pwrite32[j] != pread32[j])
                {
                    snor_dbg_hex("w:", pwrite32, 4, 16);
                    snor_dbg_hex("r:", pread32, 4, 16);
                    rt_kprintf("check not match:row=0x%lx, num=0x%lx, write=0x%lx, read=0x%lx 0x%lx 0x%lx 0x%lx\n",
                               test_lba, j, pwrite32[j], pread32[j], pread32[j + 1], pread32[j + 2], pread32[j - 1]);
                    while (1)
                        ;
                }
            }
            rt_kprintf("test_lba= 0x%lx\n", test_lba);
        }
    }
    rt_kprintf("---------%s SUCCESS---------\n", __func__);

    rt_free_align(txbuf);
    rt_free_align(rxbuf);

    return HAL_OK;
}

static uint32_t snor_io_stress_test(uint32_t from, uint32_t size, uint32_t loop)
{
    int32_t ret, i, j;
    uint32_t test_lba;
    rt_uint8_t *txbuf = NULL, *rxbuf = NULL;
    rt_uint32_t *pwrite32 = NULL, *pread32 = NULL;
    rt_uint32_t test_begin_lba, test_end_lba;

    test_begin_lba = from / snor_device->block_size;
    test_end_lba = (from + size) / snor_device->block_size;
    txbuf = (rt_uint8_t *)rt_malloc_align(snor_device->block_size, 64);
    rxbuf = (rt_uint8_t *)rt_malloc_align(snor_device->block_size, 64);

    pwrite32 = (rt_uint32_t *)txbuf;
    pread32 = (rt_uint32_t *)rxbuf;
    for (int32_t i = 0; i < (snor_device->block_size / 4); i++)
        pwrite32[i] = tuning_blk_pattern_4bit[i & (TUNING_PATTERN_SIZE - 1)];

    rt_kprintf("---------%s from LBA 0x%lx to 0x%lx---------\n", __func__, test_begin_lba, test_end_lba);
    for (i = 0; i < loop; i++)
    {
        rt_kprintf("---------%s loop %d---------\n", __func__, i);
        for (test_lba = test_begin_lba; test_lba < test_end_lba; test_lba++)
        {
            pwrite32[0] = test_lba;
            rt_mtd_nor_erase_block(snor_device, test_lba * snor_device->block_size, snor_device->block_size);
            ret = rt_mtd_nor_write(snor_device, test_lba * snor_device->block_size, (const rt_uint8_t *)pwrite32, snor_device->block_size);
            if (ret != snor_device->block_size)
            {
                rt_kprintf("%s 0x%lx write fail\n", __func__, test_lba);
                while (1)
                    ;
            }
            pread32[0] = -1;
            ret = rt_mtd_nor_read(snor_device, test_lba * snor_device->block_size, (rt_uint8_t *)pread32, snor_device->block_size);
            if (ret != snor_device->block_size)
            {
                rt_kprintf("%s 0x%lx read fail\n", __func__, test_lba);
                while (1)
                    ;
            }
            for (j = 0; j < (int32_t)snor_device->block_size / 4; j++)
            {
                if (pwrite32[j] != pread32[j])
                {
                    snor_dbg_hex("w:", pwrite32, 4, 16);
                    snor_dbg_hex("r:", pread32, 4, 16);
                    rt_kprintf("check not match:row=0x%lx, num=0x%lx, write=0x%lx, read=0x%lx 0x%lx 0x%lx 0x%lx\n",
                               test_lba, j, pwrite32[j], pread32[j], pread32[j + 1], pread32[j + 2], pread32[j - 1]);
                    while (1)
                        ;
                }
            }
            rt_kprintf("test_lba= 0x%lx\n", test_lba);
        }
    }
    rt_kprintf("---------%s SUCCESS---------\n", __func__);

    rt_free_align(txbuf);
    rt_free_align(rxbuf);

    return HAL_OK;
}

void snor_test(int argc, char **argv)
{
    char *cmd;
    rt_uint8_t *txbuf = NULL, *rxbuf = NULL;
    uint32_t loop, start_time, end_time, cost_time;
    rt_off_t offset = 0;
    rt_uint32_t size = 0;
    int i;

    if (argc < 2)
        goto out;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return;
    }

    cmd = argv[1];
    if (!rt_strcmp(cmd, "write"))
    {
        if (argc != 4)
            goto out;
        offset = atoi(argv[2]);
        size = atoi(argv[3]);

        if (size > SNOR_BUFFER_LIMIT)
        {
            rt_kprintf("write size 0x%lx, limit \n", size, SNOR_BUFFER_LIMIT);
            return;
        }

        if ((offset + size) > snor_device->block_size * snor_device->block_end)
        {
            rt_kprintf("write over: 0x%lx 0x%lx\n", (offset + size), snor_device->block_size * snor_device->block_end);
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
        rt_mtd_nor_write(snor_device, offset, (const rt_uint8_t *)txbuf, size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        snor_dbg_hex("write: ", txbuf, 4, 16);
        rt_kprintf("snor write speed: %ld KB/s\n", size / cost_time);

        rt_free_align(txbuf);
    }
    else if (!rt_strcmp(cmd, "read"))
    {

        if (argc != 5)
            goto out;
        offset = atoi(argv[2]);
        size = atoi(argv[3]);
        loop = atoi(argv[4]);

        if (size > SNOR_BUFFER_LIMIT)
        {
            rt_kprintf("read size 0x%lx, limit \n", size, SNOR_BUFFER_LIMIT);
            return;
        }

        if ((offset + size) > snor_device->block_size * snor_device->block_end)
        {
            rt_kprintf("read over: 0x%lx 0x%lx\n", (offset + size), snor_device->block_size * snor_device->block_end);
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
            rt_mtd_nor_read(snor_device, offset, rxbuf, size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        snor_dbg_hex("read: ", rxbuf, 4, 16);
        rt_kprintf("snor read speed: %ld KB/s\n", size * loop / cost_time);

        rt_free_align(rxbuf);
    }
    else if (!rt_strcmp(cmd, "erase"))
    {
        if (argc != 3)
            goto out;
        offset = atoi(argv[2]);

        if ((offset + snor_device->block_size) > snor_device->block_size * snor_device->block_end)
        {
            rt_kprintf("erase over: 0x%lx 0x%lx\n", (offset + snor_device->block_size), snor_device->block_size * snor_device->block_end);
            return;
        }

        start_time = HAL_GetTick();
        rt_mtd_nor_erase_block(snor_device, offset, snor_device->block_size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        rt_kprintf("snor erase speed: %ld KB/s\n", snor_device->block_size / cost_time);
    }
    else if (!rt_strcmp(cmd, "stress"))
    {
        if (argc != 5)
            goto out;
        offset = atoi(argv[2]);
        size = atoi(argv[3]);
        loop = atoi(argv[4]);

        if ((size / snor_device->block_size) == 0)
        {
            rt_kprintf("test sector aligned size: 0x%lx\n", (size / snor_device->block_size));
            return;
        }

        if ((offset + size) > snor_device->block_size * snor_device->block_end)
        {
            rt_kprintf("test over: 0x%lx 0x%lx\n", (offset + size), snor_device->block_size * snor_device->block_end);
            return;
        }

        snor_stress_test(offset, size, loop);
    }
    else if (!rt_strcmp(cmd, "io_stress"))
    {
        if (argc != 5)
            goto out;
        offset = atoi(argv[2]);
        size = atoi(argv[3]);
        loop = atoi(argv[4]);

        if ((size / snor_device->block_size) == 0)
        {
            rt_kprintf("test sector aligned size: 0x%lx\n", (size / snor_device->block_size));
            return;
        }

        if ((offset + size) > snor_device->block_size * snor_device->block_end)
        {
            rt_kprintf("test over: 0x%lx 0x%lx\n", (offset + size), snor_device->block_size * snor_device->block_end);
            return;
        }

        snor_io_stress_test(offset, size, loop);
    }
    else if (!rt_strcmp(cmd, "pm_test"))
    {
#ifdef FSPI0
        uint32_t *p_reg = (uint32_t *)FSPI0;

        rk_snor_suspend();
        p_reg[0] = 0;
        p_reg[0x14 / 4] = 0;
        p_reg[0x3c / 4] = 0;
        p_reg[0x50 / 4] = 0;
        p_reg[0x54 / 4] = 0;
        p_reg[0x58 / 4] = 0;
        p_reg[0x60 / 4] = 0;
        p_reg[0x64 / 4] = 0;
        rk_snor_resume();
#endif
    }
    else
    {

        goto out;
    }

    return;
out:
    snor_test_show_usage();
    return;
}

int _at_snor_test(void)
{
    rt_uint32_t test_lba = 0;
    rt_int32_t ret, i;
    rt_uint8_t *txbuf = NULL, *rxbuf = NULL;
    rt_uint32_t block_size;
    rt_uint32_t *pwrite32 = NULL, *pread32 = NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return RT_ERROR;
    }

    block_size = snor_device->block_size;
    txbuf = (rt_uint8_t *)rt_malloc_align(block_size, 64);
    rxbuf = (rt_uint8_t *)rt_malloc_align(block_size, 64);

    pwrite32 = (rt_uint32_t *)txbuf;
    pread32 = (rt_uint32_t *)rxbuf;

    test_lba = snor_device->block_end - 1;
    rt_kprintf("test block 0x%x\n", test_lba);
    ret = rt_mtd_nor_read(snor_device, test_lba * block_size, (rt_uint8_t *)pwrite32, block_size);
    if (ret != block_size)
    {
        rt_kprintf("%s read failed, ret %x\n", ret);
        snor_dbg_hex("r:", pwrite32, 4, 16);
        ret = RT_ERROR;
        goto out;
    }
    rt_mtd_nor_erase_block(snor_device, test_lba * block_size, block_size);
    rt_mtd_nor_write(snor_device, test_lba * block_size, (const rt_uint8_t *)pwrite32, block_size);
    rt_mtd_nor_read(snor_device, test_lba * block_size, (rt_uint8_t *)pread32, block_size);
    ret = RT_EOK;
    for (i = 0; i < block_size / 4; i++)
    {
        if (pwrite32[i] != pread32[i])
        {
            ret = RT_ERROR;
            snor_dbg_hex("w:", pwrite32, 4, 16);
            snor_dbg_hex("r:", pread32, 4, 16);
            rt_kprintf("check not match:row=0x%lx, num=0x%lx, write=0x%lx, read=0x%lx 0x%lx 0x%lx\n",
                       test_lba, i, pwrite32[i], pread32[i], pread32[i + 1], pread32[i + 2]);
            break;
        }
    }

out:
    rt_free_align(txbuf);
    rt_free_align(rxbuf);

    rt_kprintf("%s finished, ret %x\n", __func__, ret);

    return ret;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(snor_test, snor test cmd);
FINSH_FUNCTION_EXPORT(_at_snor_test, spi nor test for auto test);
#endif

#endif
