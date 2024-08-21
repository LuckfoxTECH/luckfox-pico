/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#include <rtdevice.h>
#include <rtthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <drivers/mtd_nor.h>
#include "hal_base.h"

#include "rkpart.h"
#include "vendor_ops.h"

/* vendor info */
static struct vendor_info vendor_info;
static rt_uint8_t *vendor_buf;
int vendor_info_init;

static struct rt_mtd_nor_device *snor_device = RT_NULL;

static int vendor_ops(rt_uint8_t *buffer, rt_uint32_t addr, rt_uint32_t n_sec, int write)
{
    int ret = -1;

    if ((addr + n_sec) * 512 > snor_device->block_size * snor_device->block_end)
    {
        rt_kprintf("write over: 0x%lx 0x%lx\n", (addr + n_sec) * 512, snor_device->block_size * snor_device->block_end);
        return -1;
    }

    if (write)
    {
        /* erease one block size (4K) first*/
        rt_mtd_nor_erase_block(snor_device, (addr + VENDOR_PART_OFFSET) * 512, snor_device->block_size);
        if ((n_sec * 512) == rt_mtd_nor_write(snor_device, (addr + VENDOR_PART_OFFSET) * 512, (const rt_uint8_t *)buffer, n_sec * 512))
            ret = 0;
    }
    else
    {
        if ((n_sec * 512) == rt_mtd_nor_read(snor_device, (addr + VENDOR_PART_OFFSET) * 512, buffer, n_sec * 512))
            ret = 0;
    }

    if (0 == ret)
        return n_sec;
    else
        return (-1);
}

int vendor_storage_init(void)
{
    int ret = 0;
    int ret_size;
    rt_uint8_t *buffer;
    rt_uint32_t i;
    rt_uint32_t max_ver = 0;
    rt_uint32_t max_index = 0;
    rt_uint16_t data_offset, hash_offset, part_num;
    rt_uint16_t version2_offset, part_size;

    part_size =       VENDOR_PART_BLKS;
    data_offset =     VENDOR_DATA_OFFSET;
    hash_offset =     VENDOR_HASH_OFFSET;
    version2_offset = VENDOR_VERSION2_OFFSET;
    part_num =        VENDOR_PART_NUM;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }

    /* Initialize */
    //bootdev_type = dev_desc;

    /* Always use, no need to release */
    vendor_buf = (rt_uint8_t *)rt_malloc_align(512 * VENDOR_PART_BLKS, 64);
    buffer = (rt_uint8_t *)vendor_buf;

    /* Pointer initialization */
    vendor_info.hdr = (struct vendor_hdr *)buffer;
    vendor_info.item = (struct vendor_item *)(buffer + sizeof(struct vendor_hdr));
    vendor_info.data = buffer + data_offset;
    vendor_info.hash = (uint32_t *)(buffer + hash_offset);
    vendor_info.version2 = (uint32_t *)(buffer + version2_offset);

    /* Find valid and up-to-date one from (vendor0 - vendor3) */
    for (i = 0; i < part_num; i++)
    {
        ret_size = vendor_ops((uint8_t *)vendor_info.hdr,
                              part_size * i, part_size, 0);
        if (ret_size != part_size)
        {
            ret = (-1);
            goto out;
        }

        if ((vendor_info.hdr->tag == VENDOR_TAG) &&
            (*(vendor_info.version2) == vendor_info.hdr->version))
        {
            if (max_ver < vendor_info.hdr->version)
            {
                max_index = i;
                max_ver = vendor_info.hdr->version;
            }
        }
    }

    if (max_ver)
    {
        /*
        * Keep vendor_info the same as the largest
        * version of vendor
        */
        if (max_index != (part_num - 1))
        {
            ret_size = vendor_ops((uint8_t *)vendor_info.hdr,
                                  part_size * max_index, part_size, 0);
            if (ret_size != part_size)
            {
                ret = (-3);
                goto out;
            }
        }
    }
    else
    {
        vendor_info.hdr->tag = VENDOR_TAG;
        vendor_info.hdr->version = 1;
        vendor_info.hdr->next_index = 1;
        vendor_info.hdr->item_num = 0;
        vendor_info.hdr->free_offset = 0;
        vendor_info.hdr->free_size = 512 * VENDOR_PART_BLKS;
        ret_size = vendor_ops((uint8_t *)vendor_info.hdr,
                              part_size * 0, part_size, 1);
        if (ret_size != part_size)
        {
            ret = (-2);
            goto out;
        }
    }

out:
    if (!ret)
        vendor_info_init = 1;
    else
    {
        vendor_info_init = 0;
        rt_free_align(vendor_buf);
        vendor_buf = NULL;
        rt_kprintf("vendor_storage_init fail:%d!\n", ret);
    }
    return ret;
}

int vendor_storage_deinit(void)
{
    if (vendor_info_init)
    {
        if (vendor_buf)
        {
            rt_free_align(vendor_buf);
            vendor_buf = NULL;
        }
        vendor_info_init = 0;
    }

    return 0;
}

int vendor_storage_read(uint32_t id, void *pbuf, uint32_t size)
{
    int ret = 0;
    uint32_t i;
    rt_uint16_t offset;
    struct vendor_item *item;

    /* init vendor storage */
    if (!vendor_info_init)
    {
        ret = vendor_storage_init();
        if (ret < 0)
            return ret;
    }

    item = vendor_info.item;
    for (i = 0; i < vendor_info.hdr->item_num; i++)
    {
        if ((item + i)->id == id)
        {
            /* PRINT_E("[Vendor INFO]:Find the matching item, id=%d\n", id); */
            /* Correct the size value */
            if (size > (item + i)->size)
                size = (item + i)->size;
            offset = (item + i)->offset;
            rt_memcpy(pbuf, (vendor_info.data + offset), size);
            return size;
        }
    }
    rt_kprintf("[Vendor ERROR]:No matching item, id=%d\n", id);

    return (-1);
}

int vendor_storage_write(uint32_t id, void *pbuf, uint32_t size)
{
    int cnt, ret = 0;
    uint32_t i, next_index, align_size;
    struct vendor_item *item;
    uint16_t part_size, max_item_num, offset, part_num;

    /* init vendor storage */
    if (!vendor_info_init)
    {
        ret = vendor_storage_init();
        if (ret < 0)
            return ret;
    }

    part_size = VENDOR_PART_BLKS;
    max_item_num = VENDOR_ITEM_NUM;
    part_num = VENDOR_PART_NUM;

    next_index = vendor_info.hdr->next_index;
    /* algin to 64 bytes*/
    align_size = (size + VENDOR_BTYE_ALIGN) & (~VENDOR_BTYE_ALIGN);
    if (size > align_size)
        return (-1);

    item = vendor_info.item;
    /* If item already exist, update the item data */
    for (i = 0; i < vendor_info.hdr->item_num; i++)
    {
        if ((item + i)->id == id)
        {
            rt_kprintf("[Vendor INFO]:Find the matching item, id=%d\n", id);
            offset = (item + i)->offset;
            rt_memcpy((vendor_info.data + offset), pbuf, size);
            (item + i)->size = size;
            vendor_info.hdr->version++;
            *(vendor_info.version2) = vendor_info.hdr->version;
            vendor_info.hdr->next_index++;
            if (vendor_info.hdr->next_index >= part_num)
                vendor_info.hdr->next_index = 0;

            rt_uint32_t addr = part_size * next_index;
            cnt = vendor_ops((uint8_t *)vendor_info.hdr, addr, part_size, 1);
            return (cnt == part_size) ? size : (-1);
        }
    }
    /*
     * If item does not exist, and free size is enough,
     * creat a new one
     */
    if ((vendor_info.hdr->item_num < max_item_num) &&
        (vendor_info.hdr->free_size >= align_size))
    {
        rt_kprintf("[Vendor INFO]:Create new Item, id=%d\n", id);
        item = vendor_info.item + vendor_info.hdr->item_num;
        item->id = id;
        item->offset = vendor_info.hdr->free_offset;
        item->size = size;

        vendor_info.hdr->free_offset += align_size;
        vendor_info.hdr->free_size -= align_size;
        rt_memcpy((vendor_info.data + item->offset), pbuf, size);
        vendor_info.hdr->item_num++;
        vendor_info.hdr->version++;
        vendor_info.hdr->next_index++;
        *(vendor_info.version2) = vendor_info.hdr->version;
        if (vendor_info.hdr->next_index >= part_num)
            vendor_info.hdr->next_index = 0;

        rt_uint32_t addr = part_size * next_index;
        cnt = vendor_ops((uint8_t *)vendor_info.hdr, addr, part_size, 1);
        return (cnt == part_size) ? size : (-1);
    }
    rt_kprintf("[Vendor ERROR]:Vendor has no space left!\n");

    return (-1);
}

/*
********************************************************************************
*
*                         End of vendor_ops.c
*
********************************************************************************
*/
