/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: BSD-3-Clause
  ******************************************************************************
  * @file    drv_flash_partition.c
  * @version V1.0
  * @brief   flash partition device driver headfile
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Dingqiang Lin   the first version
  * 2019-12-27     Dingqiang Lin   add rk_partition definition
  *
  ******************************************************************************
  */

/*
 * rk_partition is the universal partition format in rk firmware merge tool.
 *
 * rk flash partition is the partition format for RTT OS partition management.
 */

#ifndef __FLASH_PARTITION_H__
#define __FLASH_PARTITION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <drivers/mtd_nor.h>
#include <drivers/mtd_nand.h>

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

/* ================================================================================ */
/* ================                 rk_partition                  ================= */
/* ================================================================================ */
#define RK_PARTITION_SIZE   2048    /* rk_partition total size */
#define RK_PARTITION_TAG    0x50464B52
#define RK_PARTITION_NAME_SIZE  32

/* rk_partition: ui_part_property bits filed */
#define RK_PARTITION_NO_PARTITION_SIZE BIT(2)
#define RK_PARTITION_PROPERTY_SHIFT    (8)
#define RK_PARTITION_PROPERTY_MASK     (0x3 << RK_PARTITION_PROPERTY_SHIFT)
#define RK_PARTITION_PROPERTY_ROONLY   (PART_FLAG_RDONLY << RK_PARTITION_PROPERTY_SHIFT)
#define RK_PARTITION_PROPERTY_WRONLY   (PART_FLAG_WRONLY << RK_PARTITION_PROPERTY_SHIFT)
#define RK_PARTITION_PROPERTY_RDWR     (PART_FLAG_RDWR << RK_PARTITION_PROPERTY_SHIFT)

#define RK_PARTITION_REGISTER_TYPE_BLK (0 << 10)
#define RK_PARTITION_REGISTER_TYPE_MTD (1 << 10)

/* rk_partition: partition date time */
typedef enum
{
    PART_VENDOR     = 1 << 0,
    PART_IDBLOCK    = 1 << 1,
    PART_MISC       = 1 << 2,
    PART_FW1        = 1 << 3,
    PART_FW2        = 1 << 4,
    PART_DATA       = 1 << 5,
    PART_FONT1      = 1 << 6,
    PART_FONT2      = 1 << 7,
    PART_CHAR       = 1 << 8,
    PART_MENU       = 1 << 9,
    PART_UI         = 1 << 10,
    PART_USER1      = 1 << 30,
    PART_USER2      = 1 << 31
} enum_partition_type;

/* rk_partition : partition date time */
struct rk_parttion_date_time
{
    uint16_t  year;
    uint8_t   month;
    uint8_t   day;
    uint8_t   hour;
    uint8_t   min;
    uint8_t   sec;
    uint8_t   reserve;
};

/* rk_partition: partition head file */
struct rk_partition_header
{
    uint32_t    ui_fw_tag;  /* "RKFP" */
    struct rk_parttion_date_time    dt_release_data_time;
    uint32_t    ui_fw_ver;
    uint32_t    ui_size;    /* size of sturct,unit of u8 */
    uint32_t    ui_part_entry_offset;   /* unit of sector */
    uint32_t    ui_backup_part_entry_offset;
    uint32_t    ui_part_entry_size; /* unit of u8 */
    uint32_t    ui_part_entry_count;
    uint32_t    ui_fw_size; /* unit of u8 */
    uint8_t     reserved[464];
    uint32_t    ui_part_entry_crc;
    uint32_t    ui_header_crc;
};

/* rk_partition: partition item */
typedef struct rk_parttion_item
{
    uint8_t     sz_name[32];
    enum_partition_type em_part_type;
    uint32_t    ui_pt_off;  /* unit of sector */
    uint32_t    ui_pt_sz;   /* unit of sector */
    uint32_t    ui_data_length; /* ui_data_length low 32 */
    uint32_t    reserved1;  /* ui_data_length high 32 */
    uint32_t    ui_part_property;
    uint8_t     reserved2[72];
} STRUCT_PART_ITEM, *PSTRUCT_PART_ITEM;

typedef struct rk_partition_info
{
    struct rk_partition_header hdr; /* 0.5KB */
    struct rk_parttion_item part[12];   /* 1.5KB */
} STRUCT_PART_INFO, *PSTRUCT_PART_INFO;;

/* ================================================================================ */
/* ================                 rt_flash_partition            ================= */
/* ================================================================================ */
#define PART_FLAG_RDONLY    0x0001
#define PART_FLAG_WRONLY    0x0002
#define PART_FLAG_RDWR      0x0003
#define PART_FLAG_BLK       (1 << 2)
#define PART_FLAG_MTD       (1 << 3)
#define PART_MAX_NUM        (10)

struct rt_flash_partition
{
    union
    {
        struct rt_mtd_nor_device    mtd_nor;
        struct rt_mtd_nand_device   mtd_nand;
        struct rt_device            blk;
    };
    char name[RK_PARTITION_NAME_SIZE];
    uint32_t offset;             /* offset within the master MTD space */
    uint32_t size;               /* partition size */
    uint32_t type;               /* follow rk_partition type */
    uint32_t mask_flags;         /* master MTD flags to mask out for this partition */
    void *user_data;
};

rt_err_t mtd_nor_init_partition(struct rt_mtd_nor_device *mtd_nor, struct rt_flash_partition *parts);
rt_err_t blk_init_partition(struct rt_mtd_nor_device *mtd_nor, struct rt_flash_partition *parts);
uint32_t rk_partition_init(struct rt_mtd_nor_device *mtd);
int32_t get_rk_partition(struct rt_flash_partition **part);
int change_part_name(int em_part_type, char *new_name);

#ifdef __cplusplus
}
#endif

#endif /* _SPI_FLASH_MTD_PARTITION_H_ */
