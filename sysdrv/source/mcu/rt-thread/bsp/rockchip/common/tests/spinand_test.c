/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    spinand_test.c
  * @version V1.0
  * @brief   spi nand test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-06-17     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_SPINAND

#include <drivers/mtd.h>
#include "hal_base.h"

#define NAND_PAGE_SIZE  4096
#define NAND_SPARE_SIZE 64

#ifndef EUCLEAN
#define EUCLEAN                         117
#endif

struct nand_ops_t
{
    uint32_t (*erase_blk)(uint8_t cs, uint32_t page_addr);
    uint32_t (*prog_page)(uint8_t cs, uint32_t page_addr, uint32_t *data, uint32_t *spare);
    uint32_t (*read_page)(uint8_t cs, uint32_t page_addr, uint32_t *data, uint32_t *spare);
};

static uint32_t *pwrite;
static uint32_t *pread;

static uint32_t *pspare_write;
static uint32_t *pspare_read;

static uint32_t bad_blk_num;
static uint32_t bad_page_num;
static struct nand_ops_t g_nand_ops;
struct mtd_info *mtd_dev = RT_NULL;

static void spinand_test_show_usage()
{
    rt_kprintf("1. spinand_test write offset size\n");
    rt_kprintf("2. spinand_test read offset size loop\n");
    rt_kprintf("3. spinand_test erase offset\n");
    rt_kprintf("3. spinand_test erase_all\n");
    rt_kprintf("4. spinand_test stress loop\n");
    rt_kprintf("like:\n");
    rt_kprintf("\tspinand_test write 2097152 4096\n");
    rt_kprintf("\tspinand_test read 2097152 4096 2000\n");
    rt_kprintf("\tspinand_test erase_all\n");
    rt_kprintf("\tspinand_test stress 5000\n");
}

HAL_Status spinand_dbg_hex(char *s, void *buf, uint32_t width, uint32_t len)
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
            rt_kprintf("[SPI Nand TEST] %s %p + 0x%lx:", s, buf, i * width);
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

static uint32_t nand_flash_test(uint32_t blk_begin, uint32_t blk_end, uint32_t is_recheck)
{
    uint32_t i, block, page;
    int ret;
    uint32_t pages_num = 64;
    uint32_t block_addr = 64;
    uint32_t is_bad_blk = 0;
    char *print_tag = is_recheck ? "read" : "prog";
    uint32_t sec_per_page = 4; /* 4 - 2KB; 8 - 4KB */

    bad_blk_num = 0;
    bad_page_num = 0;
    for (block = blk_begin; block < blk_end; block++)
    {
        is_bad_blk = 0;
        rt_kprintf("Flash %s block: %x\n", print_tag, block);
        if (!is_recheck)
            g_nand_ops.erase_blk(0, block * block_addr);
        for (page = 0; page < pages_num; page++)
        {
            for (i = 0; i < sec_per_page * 128; i++)
                pwrite[i] = ((block * block_addr + page) << 16) + i;
            for (i = 0; i < sec_per_page / 2; i++)
                pspare_write[i] = ((block * block_addr + page) << 16) + 0x5AF0 + i;
            pwrite[0] |= 0xff; /* except bad block marker in data area */
            if (!is_recheck)
                g_nand_ops.prog_page(0, block * block_addr + page, pwrite, pspare_write);
            memset(pread, 0, sec_per_page * 512);
            memset(pspare_read, 0, sec_per_page * 2);
            ret = g_nand_ops.read_page(0, block * block_addr + page, pread, pspare_read);
            if (ret)
                is_bad_blk = 1;
            for (i = 0; i < sec_per_page * 128; i++)
            {
                if (pwrite[i] != pread[i])
                {
                    rt_kprintf("Data not macth,W:%x,R:%x,@%x,pageAddr=%x\n",
                               pwrite[i], pread[i], i, block * block_addr + page);
                    is_bad_blk = 1;
                    break;
                }
            }
            if (is_bad_blk)
            {
                bad_page_num++;
                spinand_dbg_hex("data w:", pwrite, 4, sec_per_page * 128);
                spinand_dbg_hex("data r:", pread, 4, sec_per_page * 128);
                while (1)
                    ;
            }
        }
        if (is_bad_blk)
            bad_blk_num++;
    }
    return 0;
}

static void flash_erase_all_block(uint32_t firstblk)
{
    uint32_t blk;
    uint32_t end_blk = mtd_dev->size >> mtd_dev->erasesize_shift;

    for (blk = firstblk; blk < end_blk; blk++)
    {
        rt_kprintf("erase blk %d\n", blk);
        g_nand_ops.erase_blk(0, blk << (mtd_dev->erasesize_shift -
                                        mtd_dev->writesize_shift));
    }

}

static uint32_t nand_flash_stress_test(void)
{
    uint32_t ret = 0;

    rt_kprintf("%s\n", __func__);
    flash_erase_all_block(0);
    rt_kprintf("Flash prog/read test\n");
    ret = nand_flash_test(0, mtd_dev->size >> mtd_dev->erasesize_shift, 0);
    if (ret)
        return -1;
    rt_kprintf("Flash read recheck test\n");
    ret = nand_flash_test(0, mtd_dev->size >> mtd_dev->erasesize_shift, 1);
    if (ret)
        return -1;
    rt_kprintf("%s success\n", __func__);
    return ret;
}

static uint32_t erase_blk(uint8_t cs, uint32_t page_addr)
{
    if (rt_mtd_erase(mtd_dev, page_addr << mtd_dev->writesize_shift, mtd_dev->block_size))
        return -1;
    else
        return 0;
}

static uint32_t prog_page(uint8_t cs, uint32_t page_addr, uint32_t *data, uint32_t *spare)
{
    int ret;
    struct mtd_io_desc ops;

    memset(&ops, 0, sizeof(struct mtd_io_desc));
    ops.mode = MTD_OPM_AUTO_OOB;
    ops.datlen = mtd_dev->sector_size;
    ops.datbuf = (uint8_t *)data;
    ops.ooblen = 0;
    ops.ooboffs = 0;
    ops.oobbuf = RT_NULL;

    ret = rt_mtd_write_oob(mtd_dev, (loff_t)(page_addr << mtd_dev->writesize_shift), &ops);
    if (ops.datretlen != ops.datlen)
    {
        rt_kprintf("%s %x %d %d %d\n", __func__, page_addr, ops.datretlen,
                   ops.oobretlen, ret);
        return -RT_EIO;
    }
    else
    {
        return 0;
    }
}

static uint32_t read_page(uint8_t cs, uint32_t page_addr, uint32_t *data, uint32_t *spare)
{
    int ret;
    struct mtd_io_desc ops;

    memset(&ops, 0, sizeof(struct mtd_io_desc));
    ops.mode = MTD_OPM_AUTO_OOB;
    ops.datlen = mtd_dev->sector_size;
    ops.datbuf = (uint8_t *)data;
    ops.ooblen = 0;
    ops.ooboffs = 0;
    ops.oobbuf = RT_NULL;

    ret = rt_mtd_read_oob(mtd_dev, (loff_t)(page_addr << mtd_dev->writesize_shift), &ops);
    if (ops.datretlen != ops.datlen)
    {
        rt_kprintf("%s %x %d %d %d\n", __func__, page_addr, ops.datretlen, ops.oobretlen, ret);
        return -RT_EIO;
    }
    else
    {
        return 0;
    }
}

static RT_UNUSED int nand_flash_misc_test(void)
{
    int ret = 0;
    uint32_t blk;

    rt_kprintf("%s\n", __func__);
    rt_kprintf("Bad block operation test\n");

    for (blk = 10; blk < 60; blk++)
    {
        rt_kprintf("%s mark bad block %d\n", __func__, blk);
        ret = rt_mtd_block_markbad(mtd_dev, blk);
        if (ret)
        {
            rt_kprintf("%s mark bad fail %d\n", __func__, ret);
            while (1)
                ;
        }
        ret = rt_mtd_block_isbad(mtd_dev, blk);
        if (!ret)
        {
            rt_kprintf("%s mark bad check fail %d\n", __func__, ret);
            while (1)
                ;
        }

        g_nand_ops.erase_blk(0, blk);
    }
    rt_kprintf("%s success\n", __func__);

    return ret;
}

void nand_utils_test(uint32_t loop)
{
    rt_kprintf("size %d MB\n", (uint32_t)(mtd_dev->size / 1024 / 1024));
    rt_kprintf("block_size%d KB\n", (uint32_t)(mtd_dev->block_size / 1024));
    rt_kprintf("block_size%d KB\n", (uint32_t)(mtd_dev->sector_size / 1024));

    pwrite = malloc(NAND_PAGE_SIZE);
    pspare_write = malloc(NAND_SPARE_SIZE);
    pread = malloc(NAND_PAGE_SIZE);
    pspare_read = malloc(NAND_SPARE_SIZE);

    g_nand_ops.erase_blk = erase_blk;
    g_nand_ops.read_page = read_page;
    g_nand_ops.prog_page = prog_page;

    while (loop--)
    {
        nand_flash_stress_test();
    }

    free(pwrite);
    free(pspare_write);
    free(pread);
    free(pspare_read);
}

void spinand_test(int argc, char **argv)
{
    char *cmd;
    rt_uint8_t *txbuf = NULL, *rxbuf = NULL;
    uint32_t offset, size, loop, start_time, end_time, cost_time;
    int i;

    if (argc < 3)
        goto out;

    mtd_dev = (struct mtd_info *)rt_device_find("spinand0");
    if (mtd_dev ==  RT_NULL)
    {
        rt_kprintf("Did not find device: spinand0....\n");
        return;
    }

    cmd = argv[1];
    if (!rt_strcmp(cmd, "write"))
    {
        if (argc != 4)
            goto out;
        offset = atoi(argv[2]);
        size = atoi(argv[3]);

        if (size > NAND_PAGE_SIZE)
        {
            rt_kprintf("write size 0x%lx, limit \n", size, NAND_PAGE_SIZE);
            return;
        }

        if ((offset + size) > mtd_dev->size)
        {
            rt_kprintf("write over: 0x%lx 0x%lx\n", (offset + size), mtd_dev->size);
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
        rt_mtd_write(mtd_dev, offset, (const rt_uint8_t *)txbuf, size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        spinand_dbg_hex("write: ", txbuf, 4, 16);
        rt_kprintf("spinand write speed: %ld KB/s\n", size / cost_time);

        rt_free_align(txbuf);
    }
    else if (!rt_strcmp(cmd, "read"))
    {

        if (argc != 5)
            goto out;
        offset = atoi(argv[2]);
        size = atoi(argv[3]);
        loop = atoi(argv[4]);

        if (size > NAND_PAGE_SIZE)
        {
            rt_kprintf("read size 0x%lx, limit \n", size, NAND_PAGE_SIZE);
            return;
        }

        if ((offset + size) > mtd_dev->size)
        {
            rt_kprintf("write over: 0x%lx 0x%lx\n", (offset + size), mtd_dev->size);
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
            rt_mtd_read(mtd_dev, offset, rxbuf, size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        spinand_dbg_hex("read: ", rxbuf, 4, 16);
        rt_kprintf("spinand read speed: %ld KB/s\n", size * loop / cost_time);

        rt_free_align(rxbuf);
    }
    else if (!rt_strcmp(cmd, "erase"))
    {
        if (argc != 3)
            goto out;
        offset = atoi(argv[2]);

        if ((offset + mtd_dev->block_size) > mtd_dev->size)
        {
            rt_kprintf("erase over: 0x%lx 0x%lx\n", offset, mtd_dev->size);
            return;
        }

        start_time = HAL_GetTick();
        rt_mtd_erase(mtd_dev, offset, mtd_dev->block_size);
        end_time = HAL_GetTick();
        cost_time = (end_time - start_time); /* ms */

        rt_kprintf("spinand erase speed: %ld KB/s\n", mtd_dev->block_size / cost_time);
    }
    else if (!rt_strcmp(cmd, "erase_all"))
    {
        if (argc != 3)
            goto out;

        g_nand_ops.erase_blk = erase_blk;
        g_nand_ops.read_page = read_page;
        g_nand_ops.prog_page = prog_page;

        flash_erase_all_block(0);
        rt_kprintf("erase all finished\n");
    }
    else if (!rt_strcmp(cmd, "stress"))
    {
        if (argc != 3)
            goto out;
        loop = atoi(argv[2]);

        nand_utils_test(loop);
    }
    else
    {

        goto out;
    }

    return;
out:
    spinand_test_show_usage();
    return;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(spinand_test, spt test cmd);
#endif

#endif
