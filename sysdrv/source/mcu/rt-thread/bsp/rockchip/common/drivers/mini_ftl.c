/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    mini_ftl.c
  * @version V1.0
  * @brief   spi nand special mini ftl
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-23     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/mtd.h>

#include "hal_base.h"
#include "mini_ftl.h"

#ifdef RT_USING_SPINAND

/********************* Private MACRO Definition ******************************/
#ifdef FTL_DEBUG
#define ftl_dbg(...)     rt_kprintf(__VA_ARGS__)
#else
#define ftl_dbg(...)
#endif

#define MTD_TO_SPINAND(mtd) ((struct SPI_NAND *)mtd->priv)

#ifndef EUCLEAN
#define EUCLEAN                         117
#endif

#define MTD_BLK_TABLE_BLOCK_UNKNOWN (-2)
#define MTD_BLK_TABLE_BLOCK_SHIFT   (-1)

/********************* Private Variable Definition **************************/
static int16_t *mtd_map_blk_table;

/********************* Private Function Definition ****************************/

static int map_table_init(rt_mtd_t *mtd, loff_t offset, size_t length)
{
    uint32_t blk_total, blk_begin, blk_cnt, bad_blk = 0;
    int i, j;

    if (!mtd)
    {
        return -RT_ERROR;
    }
    else
    {
        blk_total = (mtd->size + mtd->block_size - 1) >> mtd->erasesize_shift;
        if (!mtd_map_blk_table)
        {
            mtd_map_blk_table = (int16_t *)rt_malloc(blk_total * sizeof(int16_t));
            for (i = 0; i < blk_total; i++)
                mtd_map_blk_table[i] = MTD_BLK_TABLE_BLOCK_UNKNOWN;
        }

        blk_begin = (uint32_t)offset >> mtd->erasesize_shift;
        blk_cnt = ((uint32_t)((offset & mtd->erasesize_mask) + length + \
                              mtd->block_size - 1) >> mtd->erasesize_shift);
        if (blk_begin >= blk_total)
        {
            rt_kprintf("map table blk begin[%d] overflow\n", blk_begin);
            return -EINVAL;
        }
        if ((blk_begin + blk_cnt) > blk_total)
            blk_cnt = blk_total - blk_begin;

        if (mtd_map_blk_table[blk_begin] != MTD_BLK_TABLE_BLOCK_UNKNOWN)
            return 0;

        j = 0;
        /* should not across blk_cnt */
        for (i = 0; i < blk_cnt; i++)
        {
            if (j >= blk_cnt)
                mtd_map_blk_table[blk_begin + i] = MTD_BLK_TABLE_BLOCK_SHIFT;
            for (; j < blk_cnt; j++)
            {
                if (!rt_mtd_block_isbad(mtd, (blk_begin + j)))
                {
                    mtd_map_blk_table[blk_begin + i] = blk_begin + j;
                    j++;
                    if (j == blk_cnt)
                        j++;
                    break;
                }
                else
                {
                    rt_kprintf("blk[%d] is bad block\n", blk_begin + j);
                    bad_blk++;
                }
            }
        }

        return bad_blk;
    }
}

static loff_t get_map_address(rt_mtd_t *mtd, loff_t off)
{
    loff_t offset = off;
    size_t block_offset = offset & (mtd->block_size - 1);

    RT_ASSERT(mtd_map_blk_table &&
              mtd_map_blk_table[(uint64_t)offset >> mtd->erasesize_shift] != MTD_BLK_TABLE_BLOCK_UNKNOWN &&
              mtd_map_blk_table[(uint64_t)offset >> mtd->erasesize_shift] != 0xffff);

    return (loff_t)(((uint32_t)mtd_map_blk_table[(uint64_t)offset >> mtd->erasesize_shift] <<
                     mtd->erasesize_shift) + block_offset);
}

static RT_UNUSED void flash_erase_all_block(rt_mtd_t *mtd)
{
    struct SPI_NAND *spinand = MTD_TO_SPINAND(mtd);
    uint32_t blk;
    uint32_t end_blk = mtd->size >> mtd->erasesize_shift;

    for (blk = 0; blk < end_blk; blk++)
    {
        rt_kprintf("erase blk %d\n", blk);
        HAL_SPINAND_EraseBlock(spinand, blk);
    }

}

/********************* Public Function Definition ****************************/
/**
 * @brief SPI Nand page mtd page read.
 * @param mtd: mtd info.
 * @param from: source data address in byte.
 * @param ops: mtd io operation structure.
 * @retval -RT_EIO: hardware or devices error;
 *         -RT_ERROR: ECC fail;
 *         RT_EOK.
 * @attention: allow main area non-aligned column address read, not support oob.
 */
int mftl_mtd_read(rt_mtd_t *mtd, loff_t from, struct mtd_io_desc *ops)
{
    struct SPI_NAND *spinand = MTD_TO_SPINAND(mtd);
    uint8_t *data = (uint8_t *)ops->datbuf;
    size_t remaining = ops->datlen;
    int32_t ret = RT_EOK;
    bool ecc_failed = false;

    if ((from + remaining) > mtd->size || remaining & mtd->writesize_mask || ops->ooblen)
    {
        return -RT_EINVAL;
    }

    ops->datretlen = 0;
    while (remaining)
    {
        ret = HAL_SPINAND_ReadPageRaw(spinand, get_map_address(mtd, from) >>
                                      mtd->writesize_shift, data, false);
        if (ret < 0)
            break;

        if (ret == SPINAND_ECC_ERROR)
        {
            rt_kprintf("%s addr %lx ret= %d\n", __func__, (uint32_t)from, ret);
            ecc_failed = true;
        }

        ret = 0;
        data += mtd->sector_size;
        ops->datretlen += mtd->sector_size;
        remaining -= mtd->sector_size;
        from += mtd->sector_size;
    }
    if (ret)
        return -RT_EIO;

    if (ecc_failed)
        return -RT_ERROR;

    return RT_EOK;
}

int mftl_mtd_write(rt_mtd_t *mtd, loff_t to, struct mtd_io_desc *ops)
{
    struct SPI_NAND *spinand = MTD_TO_SPINAND(mtd);
    int32_t ret = RT_EOK;
    uint8_t *data = (uint8_t *)ops->datbuf;
    size_t remaining = ops->datlen;

    if (to & mtd->writesize_mask || remaining & mtd->writesize_mask || ops->ooblen)
    {
        return -RT_EINVAL;
    }

    ops->datretlen = 0;
    while (remaining)
    {
        ret = HAL_SPINAND_ProgPageRaw(spinand, get_map_address(mtd, to) >> mtd->writesize_shift, (uint32_t *)data, false);
        if (ret != HAL_OK)
        {
            ftl_dbg("%s addr %lx ret= %d\n", __func__, to, ret);
            break;
        }

        data += mtd->sector_size;
        ops->datretlen += mtd->sector_size;
        remaining -= mtd->sector_size;
        to += mtd->sector_size;
    }

    if (ret)
        return -RT_EIO;

    return 0;
}

int mftl_mtd_erase(rt_mtd_t *mtd, loff_t addr, size_t len)
{
    struct SPI_NAND *spinand = MTD_TO_SPINAND(mtd);
    int32_t ret = RT_EOK;
    size_t remaining;

    remaining = len;

    if (remaining & mtd->erasesize_mask || addr & mtd->erasesize_mask)
    {
        ret = -RT_EINVAL;
        goto out;
    }

    while (remaining)
    {
        ret = HAL_SPINAND_EraseBlock(spinand, get_map_address(mtd, addr) >>
                                     mtd->writesize_shift);
        if (ret)
        {
            rt_kprintf("%s fail addr 0x%lx ret=%d\n",
                       __func__, addr, ret);

            ret = -RT_EIO;
            goto out;
        }

        addr += mtd->block_size;
        remaining -= mtd->block_size;
    }

out:

    return ret;
}

int mini_ftl_register(struct mtd_info *mtd)
{
    int ret;

    //flash_erase_all_block(mtd);

    ret = map_table_init(mtd, 0, mtd->size);
    if (ret > 0)
        mtd->size -= (ret << mtd->erasesize_shift);

    if (ret < 0 && mtd_map_blk_table)
        rt_free(mtd_map_blk_table);

    return ret;
}

#endif