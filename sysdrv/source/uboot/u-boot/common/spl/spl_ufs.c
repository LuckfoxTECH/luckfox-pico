// SPDX-License-Identifier: GPL-2.0+
/*
 * Rockchip UFS Host Controller driver
 *
 * Copyright (C) 2024 Rockchip Electronics Co.Ltd.
 */

#include <common.h>
#include <asm/u-boot.h>
#include <blk.h>
#include <boot_rkimg.h>
#include <errno.h>
#include <image.h>
#include <part.h>
#include <scsi.h>
#include <ufs.h>
#include <spl.h>

DECLARE_GLOBAL_DATA_PTR;

static ulong h_spl_load_read(struct spl_load_info *load, ulong sector,
			     ulong count, void *buf)
{
	return blk_dread(load->dev, sector, count, buf);
}

static int spl_ufs_load_image(struct spl_image_info *spl_image,
			       struct spl_boot_device *bootdev)
{
	lbaint_t image_sector = CONFIG_SYS_UFS_RAW_MODE_U_BOOT_SECTOR;
	int ret;
	struct blk_desc *desc;
	struct image_header *header;
	struct spl_load_info load;

	/* try to recognize storage devices immediately */
	ufs_probe();
	scsi_scan(true);

	desc = blk_get_devnum_by_type(IF_TYPE_SCSI, 0);
	if (!desc)
		return -ENODEV;

	load.dev = desc;
	load.priv = NULL;
	load.filename = NULL;
	load.bl_len = desc->blksz;
	load.read = h_spl_load_read;

#ifdef CONFIG_SPL_LIBDISK_SUPPORT
	disk_partition_t info;

	ret = part_get_info_by_name(desc, PART_UBOOT, &info);
	if (ret > 0)
		image_sector = info.start;
#endif
	if (IS_ENABLED(CONFIG_SPL_LOAD_FIT)) {
		header = (struct image_header *)(CONFIG_SYS_TEXT_BASE -
					 sizeof(struct image_header));
		ret = blk_dread(desc, image_sector, 1, header);
		if (ret != 1)
			return -ENODEV;

#ifdef CONFIG_SPL_FIT_IMAGE_MULTIPLE
		if (image_get_magic(header) == FDT_MAGIC ||
		    CONFIG_SPL_FIT_IMAGE_MULTIPLE > 1) {
#else
		if (image_get_magic(header) == FDT_MAGIC) {
#endif
			ret = spl_load_simple_fit(spl_image, &load,
						  image_sector,
						  header);
		}
	}

	return ret;
}
SPL_LOAD_IMAGE_METHOD("UFS", 0, BOOT_DEVICE_UFS, spl_ufs_load_image);
