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

	if (part_get_info_by_name(info->dev, part_name, &part_info) <= 0) {
		debug("%s: no partition\n", __func__);
		return -EINVAL;
	}
	sector = part_info.start;

	memset(&meta, 0, sizeof(struct meta_head));
	if (info->read(info, sector, 1, &meta) != 1) {
		debug("%s: Failed to read header\n", __func__);
		return -EIO;
	}

	if (meta.tag != RK_META) {
		debug("Invalid meta tag is %x.\n", meta.tag);
		return -EINVAL;
	}

	if (meta.crc32 != crc32(0, (const unsigned char *)&meta, sizeof(struct meta_head) - 4 - 4)) {
		debug("Invalid meta crc32.\n");
		return -EINVAL;
	}

	data = (char *)meta.load;
	printf("Meta: 0x%08x - 0x%08x\n", meta.load, meta.load + meta.size);
	if (info->read(info, sector, DIV_ROUND_UP(MAX_META_SEGMENT_SIZE, info->bl_len), data)
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
	if (meta_p->comp_type == META_COMPRESS_TYPE_GZ) {
		if (info->read(info, sector + (MAX_META_SEGMENT_SIZE / info->bl_len),
			       DIV_ROUND_UP(meta.comp_size, info->bl_len), data)
			       != DIV_ROUND_UP(meta.comp_size, info->bl_len)) {
			debug("%s: Failed to read compress data.\n", __func__);
			return -EIO;
		}

		if (rk_meta_iq_decom((meta_p->load + meta_p->comp_off),
				     (unsigned long)(data + meta_p->comp_off -
							MAX_META_SEGMENT_SIZE),
					 meta.comp_size, &len)) {
			debug("%s: Failed to decompress.\n", __func__);
			return -EIO;
		}
		/* update decompress gz's file size */
		unsigned int *p_len = (unsigned int *)
			(meta_p->load + MAX_META_SEGMENT_SIZE + MAX_HEAD_SIZE);
		*p_len = (u32)len;
		/* TODO: update decompress gz's file crc32 */
	} else {
		if (info->read(info, sector + (MAX_META_SEGMENT_SIZE / info->bl_len),
			       DIV_ROUND_UP(meta.comp_size, info->bl_len),
		   (void *)(meta_p->load + MAX_META_SEGMENT_SIZE))
		   != DIV_ROUND_UP(meta.comp_size, info->bl_len)) {
			debug("%s: Failed to read\n", __func__);
			return -EIO;
		}
	}

	meta_p->meta_flags = META_READ_DONE_FLAG;
	flush_cache(meta_p->load, meta_p->size);
	rk_meta_process();

	return 0;
}

void rk_meta_bootargs_append(void *fdt)
{
	if (!cmdline || (!fdt || fdt_check_header(fdt)))
		return;

	fdt_bootargs_append(fdt, cmdline);
}
