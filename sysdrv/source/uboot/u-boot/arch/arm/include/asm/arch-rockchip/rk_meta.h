/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 *
 */

#ifndef _RK_CFG_H_
#define _RK_CFG_H_

/*      META storage layout        */
/***************************/      /* ----- ITEM start -----*/
/**     META_HEAD         **/
/***************************/
/**     ITEM 1            **/
/***************************/
/**     ITEM 2            **/
/***************************/
/**     ITEM 3            **/
/***************************/
/**       ....            **/
/***************************/      /* --- ITEM backup start 32KB ---*/
/**     ITEM 1 BACKUP     **/
/***************************/
/**     ITEM 2 BACKUP     **/
/***************************/
/**     ITEM 3 BACKUP     **/
/***************************/
/**       ....            **/
/***************************/
/**     META_END          **/
/***************************/      /* ----- ITEM backup end -----*/
/**     IQ FILE BIN       **/       /* 320K sensor iq file bin */
/***************************/

/*      META memory layout        */
/***************************/      /* ----- ITEM start -----*/
/**     META_HEAD         **/
/***************************/
/**     ITEM 1            **/
/***************************/
/**     ITEM 2            **/
/***************************/
/**     ITEM 3            **/
/***************************/
/**       ....            **/
/***************************/      /* --- ITEM backup start 32KB ---*/
/**     ITEM 1 BACKUP     **/
/***************************/
/**     ITEM 2 BACKUP     **/
/***************************/
/**     ITEM 3 BACKUP     **/
/***************************/
/**       ....            **/
/***************************/
/**     META_END          **/
/***************************/      /* ----- ITEM backup end -----*/
/**     IQ FILE BIN       **/       /* 320K sensor iq file bin */
/***************************/

#define RK_META			0x544d4b52
#define RK_META_END		0x55aabbcc
#define RK_CMDLINE		0x4c444d43

#define ITEM_SIZE		(1 * 1024)
#define MAX_CMDLINE_LENGTH	(1024 / 2)
#define MAX_HEAD_SIZE 4
#define MAX_META_SEGMENT_SIZE (16 * 1024)
#define BACKUP_META_SIZE (MAX_META_SEGMENT_SIZE / 2)
#define META_INFO_HEAD_OFFSET	0
#define META_INFO_SIZE		ITEM_SIZE
#define PARAM_SHARE2KERNEL_OFFSET	(META_INFO_HEAD_OFFSET + META_INFO_SIZE)
#define PARAM_SHARE2KERNEL_SIZE (ITEM_SIZE)
#define SENSOR_INIT_OFFSET	(PARAM_SHARE2KERNEL_OFFSET + PARAM_SHARE2KERNEL_SIZE)
#define SENSOR_INIT_MAX_SIZE	ITEM_SIZE
#define CMDLINE_OFFSET		(SENSOR_INIT_OFFSET + SENSOR_INIT_MAX_SIZE)
#define CMDLINE_MAX_SIZE	ITEM_SIZE
#define AE_TABLE_OFFSET		(CMDLINE_OFFSET + CMDLINE_MAX_SIZE)
#define AE_TABLE_MAX_SIZE	ITEM_SIZE

#define APP_PARAM_OFFSET	(AE_TABLE_OFFSET + AE_TABLE_MAX_SIZE)
#define APP_PARAM_MAX_SIZE	ITEM_SIZE

#define SENSOR_IQ_BIN_OFFSET			(MAX_META_SEGMENT_SIZE)
#define SENSOR_IQ_BIN_MAX_SIZE			(320 * 1024)
#define MAX_META_BIN_SIZE (MAX_META_SEGMENT_SIZE + SENSOR_IQ_BIN_MAX_SIZE)
#define META_SIZE						MAX_META_BIN_SIZE

/* 512 - sizeof(tag/load/size/comp_type/comp_size/comp_off/crc32/meta_flags) */
#define	META_HEAD_RESERVED_SIZE	   (120*4)
#define META_READ_DONE_FLAG (1 << 0)

#define AE_TABLE_SHARE2KERNEL_OFFSET	(PARAM_SHARE2KERNEL_OFFSET)

#define	META_COMPRESS_TYPE_GZ 0x1

struct meta_head {
	uint32_t tag;
	uint32_t load;
	uint32_t size;
	uint32_t comp_type;
	uint32_t comp_size;
	uint32_t comp_off;
	uint8_t  reserved[META_HEAD_RESERVED_SIZE];
	uint32_t crc32;
	uint32_t meta_flags;
};

struct cmdline_info {
	uint32_t tag;
	uint8_t data[MAX_CMDLINE_LENGTH];
	uint32_t crc32;
};

#endif
