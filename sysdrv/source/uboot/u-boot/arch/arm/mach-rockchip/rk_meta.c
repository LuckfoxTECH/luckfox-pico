/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 *
 */

#include <common.h>
#include <boot_rkimg.h>
#include <crc.h>
#include <errno.h>
#include <fdt_support.h>
#include <image.h>
#include <misc.h>
#include <spl.h>
#include <asm/arch/rk_meta.h>

static char *cmdline;

#if (IS_ENABLED(CONFIG_ROCKCHIP_RV1106))
#define COMPRESS_LOAD_ADDR 0xa0000
#else
#error	"Please Define COMPRESS_LOAD_ADDR !!!"
#endif

__weak void rk_meta_process(void) { }

static int rk_meta_get_startup_part_info(struct spl_load_info *info, disk_partition_t part_info,
	                                       uint32_t *per_part_size, struct meta_head *meta_p)
{
	struct meta_head meta_next;
	uint32_t total_part_num = 1;
	uint32_t i = 1;
	ulong sector = part_info.start;

	if (info->read(info, sector, 1, meta_p) != 1) {
		printf("%s: Failed to read header\n", __func__);
		return -EIO;
	}

	if (meta_p->tag != RK_META) {
		printf("Invalid meta tag is %x.\n", meta_p->tag);
		return -EINVAL;
	}

	total_part_num = meta_p->total_part_num;
	*per_part_size = meta_p->size + meta_p->part_reserved_size;
	if (*per_part_size * meta_p->total_part_num > part_info.size * part_info.blksz) {
		printf("Error: Total part size (0x%08x * %d) is larger than partition size (0x%08lx).\n",
		       *per_part_size, meta_p->total_part_num, part_info.size * part_info.blksz);
		return -EINVAL;
	}

	memset(&meta_next, 0, sizeof(struct meta_head));
	for (i = 1; i < total_part_num; i++) {
		if (info->read(info, sector + i * (*per_part_size / info->bl_len),
			             1, &meta_next) != 1) {
			printf("%s: Failed to read header\n", __func__);
			return -EIO;
		}

		if (meta_next.tag != RK_META) {
			debug("Not read meta tag from meta part[%d], read from part[%d] by default.\n", i, i - 1);
			break;
		}

		debug("Check part[%d]:part_flag=%d  part[%d]:part_flag=%d\n", i - 1, meta_p->part_flag,
		                                                              i, meta_next.part_flag);
		if (meta_next.part_flag != meta_p->part_flag)
			break;

		memcpy(meta_p, &meta_next, sizeof(struct meta_head));
	}

	if (meta_p->crc32 != crc32(0, (const unsigned char *)meta_p, sizeof(struct meta_head) - 4 - 4)) {
		printf("Invalid meta crc32.\n");
		return -EINVAL;
	}

	return i;
}

static int rk_meta_iq_decom(unsigned long dst, unsigned long src,
			    unsigned long src_len, u64 *len)
{
	int ret;
#if CONFIG_IS_ENABLED(MISC_DECOMPRESS)

	ret = misc_decompress_process(dst, src, src_len, DECOM_GZIP, true, len, 0);
	misc_decompress_sync(IH_COMP_GZIP);
#else
	ret = gunzip((void *)&dst, ALIGN(len, FIT_MAX_SPL_IMAGE_SZ),
		     (void *)&src, (void *)len);
#endif

	return ret;
}

int spl_load_meta(struct spl_image_info *spl_image, struct spl_load_info *info)
{
	const char *part_name = PART_META;
	disk_partition_t part_info;
	struct meta_head meta;
	struct meta_head *meta_p;
	struct cmdline_info *cmd;
	ulong sector;
	char *data;
	u64 len;
	int meta_iq_item_size = 0;
	int meta_startup_part_num = 0;
	uint32_t meta_per_part_size = 0;
	uint32_t meta_startup_part_offset = 0;

	if (part_get_info_by_name(info->dev, part_name, &part_info) <= 0) {
		debug("%s: no partition\n", __func__);
		return -EINVAL;
	}
	sector = part_info.start;

	printf("\n");
	memset(&meta, 0, sizeof(struct meta_head));

	meta_startup_part_num = rk_meta_get_startup_part_info(info, part_info, &meta_per_part_size, &meta);
	if (meta_startup_part_num <= 0) {
		debug("Get startup part number failed.[%d]\n", meta_startup_part_num);
		return -EINVAL;
	}
	printf("Meta: Found meta partition. Current part number: %d, total: %d\n",
	        meta_startup_part_num, meta.total_part_num);

	/* The first part offset starts at 0x0 offset. */
	meta_startup_part_offset = (meta_startup_part_num - 1) * (meta_per_part_size / info->bl_len);

	data = (char *)meta.load;
	printf("Meta: 0x%08x - 0x%08x\n", meta.load, meta.load + meta.size);
	if (info->read(info, sector + meta_startup_part_offset,
		             DIV_ROUND_UP(MAX_META_SEGMENT_SIZE, info->bl_len), data)
		  != DIV_ROUND_UP(MAX_META_SEGMENT_SIZE, info->bl_len)) {
		debug("%s: Failed to read header\n", __func__);
		return -EIO;
	}

	meta_p = (struct meta_head *)meta.load;

	cmd = (struct cmdline_info *)(meta_p->load + CMDLINE_OFFSET);
	if (cmd->tag == RK_CMDLINE) {
		if (cmd->crc32 == crc32(0, (const unsigned char *)cmd, sizeof(struct cmdline_info) - 4))
			cmdline = (char *)cmd->data;
	}

	/* load compress data */
	data = (char *)COMPRESS_LOAD_ADDR;
	meta_iq_item_size = meta_p->iq_item_size + meta.comp_size;
	if (meta_p->comp_type == META_COMPRESS_TYPE_GZ) {
		if (info->read(info,
			             sector + meta_startup_part_offset + (MAX_META_SEGMENT_SIZE / info->bl_len),
			             DIV_ROUND_UP(meta_iq_item_size, info->bl_len),
			             data)
			  != DIV_ROUND_UP(meta_iq_item_size, info->bl_len)) {
			printf("%s: Failed to read compress data.\n", __func__);
			return -EIO;
		}

		memcpy((void *)(meta_p->load + SENSOR_IQ_BIN_OFFSET), data, meta_p->iq_item_size);

		if (rk_meta_iq_decom((meta_p->load + meta_p->comp_off),
				     (unsigned long)(data + meta_p->comp_off -
							MAX_META_SEGMENT_SIZE),
					 meta.comp_size, &len)) {
			printf("%s: Failed to decompress.\n", __func__);
			return -EIO;
		}

	} else {
		if (info->read(info,
			             sector + meta_startup_part_offset + (MAX_META_SEGMENT_SIZE / info->bl_len),
			             DIV_ROUND_UP(meta_iq_item_size, info->bl_len),
			             (void *)(meta_p->load + MAX_META_SEGMENT_SIZE))
			  != DIV_ROUND_UP(meta_iq_item_size, info->bl_len)) {
			printf("%s: Failed to read\n", __func__);
			return -EIO;
		}
	}

	meta_p->meta_flags = META_READ_DONE_FLAG;
	flush_cache(meta_p->load, meta_p->size);
	rk_meta_process();

	printf("\nMeta: ok\n");
	return 0;
}

void rk_meta_bootargs_append(void *fdt)
{
	if (!cmdline || (!fdt || fdt_check_header(fdt)))
		return;

	fdt_bootargs_append(fdt, cmdline);
}
