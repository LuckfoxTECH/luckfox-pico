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
#include "fw_analysis.h"

#include "rkpart.h"
#include "vendor_ops.h"

#define RK_PARTITION_TAG 0x50464B52

#define VENDOR_START_ADDR   8
#define VENDOR_END_ADDR     56

extern int vendor_info_init;

//STRUCT_PART_INFO g_partition;  //size 2KB
uint32_t firmware_addr1;
uint32_t firmware_addr2;
uint32_t fw_part_size1;
uint32_t fw_part_size2;
uint32_t vendor_start_addr;
uint32_t vendor_end_addr;
uint32_t idb_start_addr;
uint32_t idb_part_size;
uint32_t data_start_addr;
uint32_t data_part_size;
uint32_t user_start_addr1;
uint32_t user_start_addr2;
uint32_t user_part_size;

#if 0
static struct rt_mtd_nor_device *snor_device = RT_NULL;

int32_t rk_partition_init(void)
{
    int32_t ret = -1;
    STRUCT_PART_ENTRY *entry;
    STRUCT_PART_INFO *partition;
    rt_uint32_t size;

    //idb_start_addr = 0;
    firmware_addr1 = firmware_addr2 = 0;
    vendor_start_addr = VENDOR_START_ADDR;
    vendor_end_addr = VENDOR_END_ADDR;
    vendor_info_init = 0;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }

    partition = (STRUCT_PART_INFO *)rt_malloc_align(512 * 4, 64);
    size = 512 * 4;
    if (size == rt_mtd_nor_read(snor_device, 0, (uint8_t *)partition, size))
    {
        if (partition->hdr.uiFwTag == RK_PARTITION_TAG)
        {
            ret = 0;
        }
    }

    if (0 != ret)
    {
        rt_kprintf("no find rkpart:0x%x\n", partition->hdr.uiFwTag);
        goto end;
    }

    ret = -1;
    entry = get_part_entry(partition, PART_IDBLOCK);
    if (entry)
    {
        idb_start_addr = entry->uiPartOffset;
        idb_part_size = entry->uiPartSize;
        rt_kprintf("IDB: Addr Start: 0x%x, size = %d (0x%x sector)\n", idb_start_addr,
                   idb_part_size * 512, idb_part_size);
    }
    else
    {
        rt_kprintf("NO find PART_IDBLOCK!\n");
        goto end;
    }

    entry = get_part_entry(partition, PART_VENDOR);
    if (entry)
    {
        vendor_start_addr = entry->uiPartOffset << 9;
        vendor_end_addr = vendor_start_addr + (entry->uiPartSize << 9);
        rt_kprintf("VENDOR: Addr Start: 0x%x, size = %d (0x%x sector)\n", entry->uiPartOffset,
                   entry->uiPartSize * 512, entry->uiPartSize);
    }
    else
    {
        rt_kprintf("NO find PART_VENDOR!\n");
        goto end;
    }

    entry = get_part_entry(partition, PART_FW1);
    if (entry)
    {
        firmware_addr1 = entry->uiPartOffset;
        fw_part_size = entry->uiPartSize;
        rt_kprintf("%s :\n", entry->szName);
        rt_kprintf("FW1: Addr:0x%x, size:%d (0x%x sector)\n", firmware_addr1,
                   fw_part_size * 512, fw_part_size);
    }
    else
    {
        rt_kprintf("NO find PART_FW1!\n");
    }

    entry = get_part_entry(partition, PART_FW2);
    if (entry)
    {
        firmware_addr2 = entry->uiPartOffset;
        rt_kprintf("%s :\n", entry->szName);
        rt_kprintf("FW2: Addr:0x%x, size:%d (0x%x sector)\n", firmware_addr2,
                   entry->uiPartSize * 512, entry->uiPartSize);
    }
    else
    {
        rt_kprintf("NO find PART_FW2!\n");
    }

    entry = get_part_entry(partition, PART_DATA);
    if (entry)
    {
        data_start_addr = entry->uiPartOffset;
        data_part_size = entry->uiPartSize;
        rt_kprintf("DATA: Addr Start: 0x%x, size = %d (0x%x sector)\n", data_start_addr,
                   data_part_size * 512, data_part_size);
    }
    else
    {
        rt_kprintf("NO find PART_DATA!\n");
    }

    /*root 1*/
    entry = get_part_entry(partition, PART_USER1);
    if (entry)
    {
        user_start_addr1 = entry->uiPartOffset;
        user_part_size = entry->uiPartSize;

        rt_kprintf("%s :\n", entry->szName);
        rt_kprintf("USER1: Addr Start: 0x%x\n", user_start_addr1);
        rt_kprintf("USER1:size:%d (0x%x sector)\n", user_part_size << 9, user_part_size);
    }
    else
    {
        rt_kprintf("NO find PART_USER1!\n");
    }

    /*root 2*/
    entry = get_part_entry(partition, PART_USER2);
    if (entry)
    {
        user_start_addr2 = entry->uiPartOffset;
        rt_kprintf("%s :\n", entry->szName);
        rt_kprintf("USER2: Addr Start: 0x%x, size: %d\n", user_start_addr2, user_part_size << 9);
    }
    else
    {
        rt_kprintf("NO find PART_USER2!\n");
    }

    ret = 0;
end:
    rt_free_align(partition);
    return ret;
}

STRUCT_PART_ENTRY *get_part_entry(STRUCT_PART_INFO *part, ENUM_PARTITION_TYPE emPartType)
{
    uint32_t i;

    if (part->hdr.uiFwTag == RK_PARTITION_TAG)
    {
        for (i = 0; i < part->hdr.uiPartEntryCount; i++)
        {
            if (part->part[i].emPartType == emPartType)
            {
                return &part->part[i];
            }
        }
    }

    return NULL;
}

int change_part_name(int emPartType, char *new_name)
{
    int ret = -1;
    rt_uint8_t *partition = RT_NULL;
    STRUCT_PART_ENTRY *entry = RT_NULL;
    STRUCT_PART_INFO *part = RT_NULL;;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }

    partition = (rt_uint8_t *)rt_malloc_align(4096, 64);
    if (4096 == rt_mtd_nor_read(snor_device, 0, partition, 4096))
    {
        part = (STRUCT_PART_INFO *)partition;
        if (part->hdr.uiFwTag == RK_PARTITION_TAG)
        {
            ret = 0;
        }
    }

    if (0 != ret)
    {
        rt_kprintf("no find rkpart:0x%x\n", part->hdr.uiFwTag);
        goto End;
    }

    entry = get_part_entry(part, emPartType);
    if (entry)
    {
        rt_kprintf("old part name : %s\n", entry->szName);
        rt_memset(entry->szName, 0, sizeof(entry->szName));
        rt_memcpy(entry->szName, new_name, rt_strlen(new_name) + 1);
        rt_kprintf("new part name : %s\n", entry->szName);

        /* write back to flash*/
        rt_mtd_nor_erase_block(snor_device, 0, snor_device->block_size);
        if (4096 != rt_mtd_nor_write(snor_device, 0, (const rt_uint8_t *)partition, 4096))
        {
            rt_kprintf("mtd nor write partition data Error!");
            ret = -1;
        }
    }

End:
    if (partition)
        rt_free_align(partition);
    return ret;
}
#endif

int get_device_sn(dev_sn_type_t dev_sn_type, char *strBuf, int len)
{
    int ret;
    if (!strBuf)
        return -1;
    rt_memset(strBuf, 0, len);

    ret = vendor_storage_init();
    if (ret < 0)
    {
        rt_kprintf("vendor_storage_init fail \n");
        return  -RT_ERROR;
    }

    ret =  vendor_storage_read(dev_sn_type, strBuf, len);
    if (ret < 0)
    {
        rt_kprintf("vendor_storage_read fail \n");
        return  -RT_ERROR;
    }

    rt_kprintf("vendor read Type %d :%s \n", (int)dev_sn_type, strBuf);

    vendor_storage_deinit();
    return ret;
}

int write_device_sn(dev_sn_type_t dev_sn_type, char *strBuf, int len)
{
    int ret;
    if (!strBuf)
        return -RT_ERROR;

    ret = vendor_storage_init();
    if (ret < 0)
    {
        rt_kprintf("vendor_storage_init fail\n");
        return  -RT_ERROR;
    }

    ret =  vendor_storage_write(dev_sn_type, strBuf, len);
    if (ret < 0)
    {
        rt_kprintf("vendor_storage_read fail\n");
        return  -RT_ERROR;
    }

    rt_kprintf("vendor write %d :%s\n", (int)dev_sn_type, strBuf);
    vendor_storage_deinit();
    return RT_EOK;
}

