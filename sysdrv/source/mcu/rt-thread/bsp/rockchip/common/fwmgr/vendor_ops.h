/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _VENDOR_OPS_H_
#define _VENDOR_OPS_H_

#include <rtdevice.h>
#include <rtthread.h>


#define VENDOR_SN_ID           1 /* serialno */
#define VENDOR_WIFI_MAC_ID     2 /* wifi mac */
#define VENDOR_LAN_MAC_ID      3 /* lan mac */
#define VENDOR_BLUETOOTH_ID    4 /* bluetooth mac */

/* tag for vendor check */
#define VENDOR_TAG      0x524B5644
/* The Vendor partition contains the number of Vendor blocks */
#define VENDOR_PART_NUM     2
/* align to 64 bytes */
#define VENDOR_BTYE_ALIGN   0x3F
#define VENDOR_BLOCK_SIZE   512

/* Starting address of the Vendor in memory. */
#define VENDOR_PART_OFFSET      (1024 * 4/512)
/*
 * The number of memory blocks used by each
 * Vendor structure(128 * 512B = 64KB)
 */
#define VENDOR_PART_BLKS        8
/* The maximum number of items in each Vendor block */
#define VENDOR_ITEM_NUM     32

/*
 * Calculate the offset of each field for emmc.
 * Emmc vendor info size: 64KB
 */
#define VENDOR_INFO_SIZE    (VENDOR_PART_BLKS * VENDOR_BLOCK_SIZE)
#define VENDOR_DATA_OFFSET  (16 + VENDOR_ITEM_NUM * sizeof(struct vendor_item))
#define VENDOR_HASH_OFFSET (VENDOR_INFO_SIZE - 8)
#define VENDOR_VERSION2_OFFSET (VENDOR_INFO_SIZE - 4)

struct vendor_hdr
{
    uint32_t  tag;
    uint32_t  version;
    uint16_t  next_index;
    uint16_t  item_num;
    uint16_t  free_offset; /* Free space offset */
    uint16_t  free_size; /* Free space size */
};

struct vendor_item
{
    uint16_t  id;
    uint16_t  offset;
    uint16_t  size;
    uint16_t  flag;
};

/*
 * Different types of Flash vendor info are different.
 * EMMC:EMMC_VENDOR_PART_BLKS * BLOCK_SIZE(512) = 64KB;
 * Spi Nor/Spi Nand/SLC/MLC: FLASH_VENDOR_PART_BLKS *
 * BLOCK_SIZE(512) = 4KB.
 * hash: For future expansion.
 * version2: Together with hdr->version, it is used to
 * ensure the current Vendor block content integrity.
 *   (version2 == hdr->version):Data valid;
 *   (version2 != hdr->version):Data invalid.
 */
struct vendor_info
{
    struct vendor_hdr *hdr;
    struct vendor_item *item;
    uint8_t *data;
    uint32_t *hash;
    uint32_t *version2;
};

/*
*-------------------------------------------------------------------------------
*
*                            Function Declaration
*
*-------------------------------------------------------------------------------
*/
extern int vendor_info_init;

extern int vendor_storage_init(void);
extern int vendor_storage_read(uint32_t id, void *pbuf, uint32_t size);
extern int vendor_storage_write(uint32_t id, void *pbuf, uint32_t size);
extern int vendor_storage_deinit(void);

/*
********************************************************************************
*
*                         End of NandBoot.h
*
********************************************************************************
*/
#endif
