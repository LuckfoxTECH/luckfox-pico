/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <android_image.h>
#include <crypto.h>
#include <image.h>
#include <mp_boot.h>
#include <part.h>
#include <spl.h>
#include <asm/io.h>

#define BLK_CNT(_num_bytes, _block_size) \
		((_num_bytes + _block_size - 1) / _block_size)

#ifdef CONFIG_ANDROID_BOOT_IMAGE
static int android_check_header(const struct andr_img_hdr *hdr)
{
	return memcmp(ANDR_BOOT_MAGIC, hdr->magic, ANDR_BOOT_MAGIC_SIZE);
}

static void print_hash(const char *label, u8 *hash, int len)
{
	int i;

	printf("%s:\n    0x", label ? : "Hash");
	for (i = 0; i < len; i++)
		printf("%02x", hash[i]);
	printf("\n");
}

#if 0
static void spl_android_print_contents(const struct andr_img_hdr *hdr)
{
	const char * const p = IMAGE_INDENT_STRING;
	/* os_version = ver << 11 | lvl */
	u32 os_ver = hdr->os_version >> 11;
	u32 os_lvl = hdr->os_version & ((1U << 11) - 1);
	u32 header_version = hdr->header_version;

	printf("%skernel size:      %x\n", p, hdr->kernel_size);
	printf("%skernel address:   %x\n", p, hdr->kernel_addr);
	printf("%sramdisk size:     %x\n", p, hdr->ramdisk_size);
	printf("%sramdisk address: %x\n", p, hdr->ramdisk_addr);
	printf("%ssecond size:      %x\n", p, hdr->second_size);
	printf("%ssecond address:   %x\n", p, hdr->second_addr);
	printf("%stags address:     %x\n", p, hdr->tags_addr);
	printf("%spage size:        %x\n", p, hdr->page_size);
	printf("%sheader_version:   %x\n", p, header_version);
	/* ver = A << 14 | B << 7 | C         (7 bits for each of A, B, C)
	 * lvl = ((Y - 2000) & 127) << 4 | M  (7 bits for Y, 4 bits for M) */
	printf("%sos_version:       %x (ver: %u.%u.%u, level: %u.%u)\n",
	       p, hdr->os_version,
	       (os_ver >> 7) & 0x7F, (os_ver >> 14) & 0x7F, os_ver & 0x7F,
	       (os_lvl >> 4) + 2000, os_lvl & 0x0F);
	printf("%sname:             %s\n", p, hdr->name);
	printf("%scmdline:          %s\n", p, hdr->cmdline);

	if (header_version == 1 || header_version == 2) {
		printf("%srecovery dtbo size:    %x\n", p, hdr->recovery_dtbo_size);
		printf("%srecovery dtbo offset:  %llx\n", p, hdr->recovery_dtbo_offset);
		printf("%sheader size:           %x\n", p, hdr->header_size);
	}

	if (header_version == 2 || header_version == 3) {
		printf("%sdtb size:              %x\n", p, hdr->dtb_size);
		printf("%sdtb addr:              %llx\n", p, hdr->dtb_addr);
	}

	if (header_version >= 3) {
		printf("%scmdline:               %s\n", p, hdr->total_cmdline);
		printf("%svendor ramdisk size:   %x\n", p, hdr->vendor_ramdisk_size);
		printf("%svendor page size:      %x\n", p, hdr->vendor_page_size);
		printf("%svendor header version: %d\n", p, hdr->vendor_header_version);
		printf("%svendor header size:    %x\n", p, hdr->vendor_header_size);
	}

	if (header_version >= 4) {
		printf("%svendor ramdisk table size:        %x\n",
		       p, hdr->vendor_ramdisk_table_size);
		printf("%svendor ramdisk table entry num:   %x\n",
		       p, hdr->vendor_ramdisk_table_entry_num);
		printf("%svendor ramdisk table entry size:  %x\n",
		       p, hdr->vendor_ramdisk_table_entry_size);
		printf("%svendor bootconfig size:           %d\n",
		       p, hdr->vendor_bootconfig_size);
	}
}
#endif

static ulong android_size(struct andr_img_hdr *hdr)
{
	ulong len;

	len = hdr->page_size +
		ALIGN(hdr->kernel_size, hdr->page_size) +
		ALIGN(hdr->ramdisk_size, hdr->page_size) +
		ALIGN(hdr->second_size, hdr->page_size);
	if (hdr->header_version > 0)
		len += ALIGN(hdr->recovery_dtbo_size, hdr->page_size);
	if (hdr->header_version > 1)
		len += ALIGN(hdr->dtb_size, hdr->page_size);
#if 0
	spl_android_print_contents(hdr);
#endif

	return len;
}

int spl_load_android(struct task_data *data)
{
	struct spl_load_info *info = &data->info;
	void *buf = (void *)CONFIG_SPL_BOOT_IMAGE_BUF;
	disk_partition_t part;
	ulong blkcnt;

	debug("== Android: load start\n");

	if (part_get_info_by_name(info->dev, "boot", &part) < 0) {
		printf("No boot partition\n");
		return -ENOENT;
	}

	blkcnt = BLK_CNT(sizeof(struct andr_img_hdr), info->bl_len);
	if (info->read(info, part.start, blkcnt, buf) != blkcnt)
		return -EIO;

	if (android_check_header(buf))
		return -EINVAL;

	blkcnt = BLK_CNT(android_size(buf), info->bl_len);
	if (info->read(info, part.start, blkcnt, buf) != blkcnt)
		return -EIO;

	data->boot_addr = (void *)CONFIG_SPL_BOOT_IMAGE_BUF;
	data->boot_size = blkcnt * info->bl_len;

	flush_dcache_range((ulong)data, (ulong)data + sizeof(*data));
	flush_dcache_range((ulong)buf, (ulong)buf + blkcnt);

	debug("== Android: load 0x%08lx size OK\n", blkcnt * info->bl_len);

	return 0;
}

#ifdef CONFIG_ARMV8_CE_SHA1
int spl_hash_android(struct task_data *data)
{
	struct andr_img_hdr *hdr = (void *)CONFIG_SPL_BOOT_IMAGE_BUF;
	sha1_context ctx;
	uchar hash[32];
	void *buf;

	printf("== Android: hash start\n");

	if (hdr->header_version >= 3)
		return -EINVAL;

	sha1_starts(&ctx);

	buf = (void *)hdr + hdr->page_size;
	sha1_update(&ctx, (const uchar *)buf, hdr->kernel_size);
	sha1_update(&ctx, (const uchar *)&hdr->kernel_size, sizeof(hdr->kernel_size));

	buf += ALIGN(hdr->kernel_size, hdr->page_size);
	sha1_update(&ctx, (const uchar *)buf, hdr->ramdisk_size);
	sha1_update(&ctx, (const uchar *)&hdr->ramdisk_size, sizeof(hdr->ramdisk_size));

	buf += ALIGN(hdr->ramdisk_size, hdr->page_size);
	sha1_update(&ctx, (const uchar *)buf, hdr->second_size);
	sha1_update(&ctx, (const uchar *)&hdr->second_size, sizeof(hdr->second_size));

	if (hdr->header_version > 0) {
		buf += ALIGN(hdr->second_size, hdr->page_size);
		sha1_update(&ctx, (const uchar *)buf, hdr->recovery_dtbo_size);
		sha1_update(&ctx, (const uchar *)&hdr->recovery_dtbo_size, sizeof(hdr->recovery_dtbo_size));
	}
	if (hdr->header_version > 1) {
		buf += ALIGN(hdr->recovery_dtbo_size, hdr->page_size);
		sha1_update(&ctx, (const uchar *)buf, hdr->dtb_size);
		sha1_update(&ctx, (const uchar *)&hdr->dtb_size, sizeof(hdr->dtb_size));
	}

	sha1_finish(&ctx, hash);

	if (memcmp(hash, hdr->id, 20)) {
		print_hash("Hash from header", (u8 *)hdr->id, 20);
		print_hash("Hash real", (u8 *)hash, 20);
		return -EBADFD;
	}

	printf("== Android: hash OK, 0x%08lx\n", (ulong)data->boot_addr);

	return 0;
}

#else
int spl_hash_android(struct task_data *data)
{
	struct andr_img_hdr *hdr = (void *)CONFIG_SPL_BOOT_IMAGE_BUF;
	struct udevice *dev;
	sha_context ctx;
	uchar hash[32];
	void *buf;

	debug("== Android: hash start\n");

	if (hdr->header_version >= 3)
		return -EINVAL;

	ctx.algo = CRYPTO_SHA1;
	dev = crypto_get_device(ctx.algo);
	if (!dev) {
		printf("No crypto device for sha1\n");
		return -ENODEV;
	}

	ctx.length = hdr->kernel_size + sizeof(hdr->kernel_size) +
	      hdr->ramdisk_size + sizeof(hdr->ramdisk_size) +
	      hdr->second_size + sizeof(hdr->second_size);
	if (hdr->header_version > 0)
		ctx.length += hdr->recovery_dtbo_size + sizeof(hdr->recovery_dtbo_size);
	if (hdr->header_version > 1)
		ctx.length += hdr->dtb_size + sizeof(hdr->dtb_size);

	crypto_sha_init(dev, &ctx);

	buf = (void *)hdr + hdr->page_size;
	crypto_sha_update(dev, buf, hdr->kernel_size);
	crypto_sha_update(dev, &hdr->kernel_size, sizeof(hdr->kernel_size));

	buf += ALIGN(hdr->kernel_size, hdr->page_size);
	crypto_sha_update(dev, buf, hdr->ramdisk_size);
	crypto_sha_update(dev, &hdr->ramdisk_size, sizeof(hdr->ramdisk_size));

	buf += ALIGN(hdr->ramdisk_size, hdr->page_size);
	crypto_sha_update(dev, buf, hdr->second_size);
	crypto_sha_update(dev, &hdr->second_size, sizeof(hdr->second_size));

	if (hdr->header_version > 0) {
		buf += ALIGN(hdr->second_size, hdr->page_size);
		crypto_sha_update(dev, buf, hdr->recovery_dtbo_size);
		crypto_sha_update(dev, &hdr->recovery_dtbo_size, sizeof(hdr->recovery_dtbo_size));
	}
	if (hdr->header_version > 1) {
		buf += ALIGN(hdr->recovery_dtbo_size, hdr->page_size);
		crypto_sha_update(dev, buf, hdr->dtb_size);
		crypto_sha_update(dev, &hdr->dtb_size, sizeof(hdr->dtb_size));
	}

	crypto_sha_final(dev, &ctx, hash);

	if (memcmp(hash, hdr->id, 20)) {
		print_hash("Hash from header", (u8 *)hdr->id, 20);
		print_hash("Hash real", (u8 *)hash, 20);
		return -EBADFD;
	}

	debug("== Android: hash OK, 0x%08lx\n", (ulong)data->boot_addr);

	return 0;
}

#endif
#endif

#ifdef CONFIG_ROCKCHIP_FIT_IMAGE
int spl_load_fit(struct task_data *data)
{
	struct spl_load_info *info = &data->info;
	void *buf = (void *)CONFIG_SPL_BOOT_IMAGE_BUF;
	disk_partition_t part;
	ulong blkcnt;
	int size;

	debug("== FIT: load start\n");

	if (part_get_info_by_name(info->dev, "boot", &part) < 0) {
	        printf("No boot partition\n");
		return -ENOENT;
	}

	blkcnt = BLK_CNT(sizeof(struct fdt_header), info->bl_len);
	if (info->read(info, part.start, blkcnt, buf) != blkcnt)
		return -EIO;

	if (fdt_check_header(buf))
		return -EINVAL;

	size = fit_get_totalsize(buf, &size);
	blkcnt = BLK_CNT(size, info->bl_len);
	if (info->read(info, part.start, blkcnt, buf) != blkcnt)
		return -EIO;

	flush_dcache_range((ulong)buf, (ulong)buf + blkcnt);

	debug("== FIT: load 0x%08x size OK\n", size);

	return 0;
}
#endif

