/*
 * Copyright (c) 2011 Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <image.h>
#include <android_image.h>
#include <android_bootloader.h>
#include <malloc.h>
#include <mapmem.h>
#include <errno.h>
#include <boot_rkimg.h>
#include <crypto.h>
#include <sysmem.h>
#include <u-boot/sha1.h>
#ifdef CONFIG_RKIMG_BOOTLOADER
#include <asm/arch/resource_img.h>
#endif
#ifdef CONFIG_RK_AVB_LIBAVB_USER
#include <android_avb/avb_slot_verify.h>
#include <android_avb/avb_ops_user.h>
#include <android_avb/rk_avb_ops_user.h>
#endif
#include <optee_include/OpteeClientInterface.h>

DECLARE_GLOBAL_DATA_PTR;

#define ANDROID_IMAGE_DEFAULT_KERNEL_ADDR	0x10008000
#define ANDROID_PARTITION_VENDOR_BOOT		"vendor_boot"
#define ANDROID_PARTITION_INIT_BOOT		"init_boot"

#define BLK_CNT(_num_bytes, _block_size)	\
		((_num_bytes + _block_size - 1) / _block_size)

static char andr_tmp_str[ANDR_BOOT_ARGS_SIZE + 1];
static u32 android_kernel_comp_type = IH_COMP_NONE;

u32 android_image_major_version(void)
{
	/* MSB 7-bits */
	return gd->bd->bi_andr_version >> 25 & 0x7f;
}

u32 android_bcb_msg_sector_offset(void)
{
	/*
	 * Rockchip platforms defines BCB message at the 16KB offset of
	 * misc partition while the Google defines it at 0x00 offset.
	 *
	 * From Android-Q, the 0x00 offset is mandary on Google VTS, so that
	 * this is a compatibility according to android image 'os_version'.
	 */
#ifdef CONFIG_RKIMG_BOOTLOADER
	return (android_image_major_version() >= 10) ? 0x00 : 0x20;
#else
	return 0x00;
#endif
}

static ulong android_image_get_kernel_addr(const struct andr_img_hdr *hdr)
{
	/*
	 * All the Android tools that generate a boot.img use this
	 * address as the default.
	 *
	 * Even though it doesn't really make a lot of sense, and it
	 * might be valid on some platforms, we treat that address as
	 * the default value for this field, and try to execute the
	 * kernel in place in such a case.
	 *
	 * Otherwise, we will return the actual value set by the user.
	 */
	if (hdr->kernel_addr == ANDROID_IMAGE_DEFAULT_KERNEL_ADDR)
		return (ulong)hdr + hdr->page_size;

#ifdef CONFIG_ARCH_ROCKCHIP
	/*
	 * If kernel is compressed, kernel_addr is set as decompressed address
	 * after compressed being loaded to ram, so let's use it.
	 */
	if (android_kernel_comp_type != IH_COMP_NONE &&
	    android_kernel_comp_type != IH_COMP_ZIMAGE)
		return hdr->kernel_addr;

	/*
	 * Compatble with rockchip legacy packing with kernel/ramdisk/second
	 * address base from 0x60000000(SDK versiont < 8.1), these are invalid
	 * address, so we calc it by real size.
	 */
	return (ulong)hdr + hdr->page_size;
#else
	return hdr->kernel_addr;
#endif

}

void android_image_set_comp(struct andr_img_hdr *hdr, u32 comp)
{
	android_kernel_comp_type = comp;
}

u32 android_image_get_comp(const struct andr_img_hdr *hdr)
{
	return android_kernel_comp_type;
}

int android_image_parse_kernel_comp(const struct andr_img_hdr *hdr)
{
	ulong kaddr = android_image_get_kernel_addr(hdr);
	return bootm_parse_comp((const unsigned char *)kaddr);
}

/**
 * android_image_get_kernel() - processes kernel part of Android boot images
 * @hdr:	Pointer to image header, which is at the start
 *			of the image.
 * @verify:	Checksum verification flag. Currently unimplemented.
 * @os_data:	Pointer to a ulong variable, will hold os data start
 *			address.
 * @os_len:	Pointer to a ulong variable, will hold os data length.
 *
 * This function returns the os image's start address and length. Also,
 * it appends the kernel command line to the bootargs env variable.
 *
 * Return: Zero, os start address and length on success,
 *		otherwise on failure.
 */
int android_image_get_kernel(const struct andr_img_hdr *hdr, int verify,
			     ulong *os_data, ulong *os_len)
{
	u32 kernel_addr = android_image_get_kernel_addr(hdr);
	const char *cmdline = hdr->header_version < 3 ?
			      hdr->cmdline : hdr->total_cmdline;
	/*
	 * Not all Android tools use the id field for signing the image with
	 * sha1 (or anything) so we don't check it. It is not obvious that the
	 * string is null terminated so we take care of this.
	 */
	strncpy(andr_tmp_str, hdr->name, ANDR_BOOT_NAME_SIZE);
	andr_tmp_str[ANDR_BOOT_NAME_SIZE] = '\0';
	if (strlen(andr_tmp_str))
		printf("Android's image name: %s\n", andr_tmp_str);

	printf("Kernel: 0x%08x - 0x%08x (%u KiB)\n",
	       kernel_addr, kernel_addr + hdr->kernel_size,
	       DIV_ROUND_UP(hdr->kernel_size, 1024));

	int len = 0;
	if (cmdline) {
		debug("Kernel command line: %s\n", cmdline);
		len += strlen(cmdline);
	}

	char *bootargs = env_get("bootargs");
	if (bootargs)
		len += strlen(bootargs);

	char *newbootargs = malloc(len + 2);
	if (!newbootargs) {
		puts("Error: malloc in android_image_get_kernel failed!\n");
		return -ENOMEM;
	}
	*newbootargs = '\0';

	if (bootargs) {
		strcpy(newbootargs, bootargs);
		strcat(newbootargs, " ");
	}
	if (cmdline)
		strcat(newbootargs, cmdline);

	env_set("bootargs", newbootargs);

	if (os_data) {
		*os_data = (ulong)hdr;
		*os_data += hdr->page_size;
	}
	if (os_len)
		*os_len = hdr->kernel_size;
	return 0;
}

int android_image_check_header(const struct andr_img_hdr *hdr)
{
	return memcmp(ANDR_BOOT_MAGIC, hdr->magic, ANDR_BOOT_MAGIC_SIZE);
}

ulong android_image_get_end(const struct andr_img_hdr *hdr)
{
	ulong end;
	/*
	 * The header takes a full page, the remaining components are aligned
	 * on page boundary
	 */
	end = (ulong)hdr;
	if (hdr->header_version < 3) {
		end += hdr->page_size;
		end += ALIGN(hdr->kernel_size, hdr->page_size);
		end += ALIGN(hdr->ramdisk_size, hdr->page_size);
		end += ALIGN(hdr->second_size, hdr->page_size);
		if (hdr->header_version == 1) {
			end += ALIGN(hdr->recovery_dtbo_size, hdr->page_size);
		} else if (hdr->header_version == 2) {
			end += ALIGN(hdr->recovery_dtbo_size, hdr->page_size);
			end += ALIGN(hdr->dtb_size, hdr->page_size);
		}
	} else {
		/* boot_img_hdr_v34 */
		end += hdr->page_size;
		end += ALIGN(hdr->kernel_size, hdr->page_size);
		end += ALIGN(hdr->ramdisk_size, hdr->page_size);
	}

	return end;
}

u32 android_image_get_ksize(const struct andr_img_hdr *hdr)
{
	return hdr->kernel_size;
}

void android_image_set_kload(struct andr_img_hdr *hdr, u32 load_address)
{
	hdr->kernel_addr = load_address;
}

ulong android_image_get_kload(const struct andr_img_hdr *hdr)
{
	return android_image_get_kernel_addr(hdr);
}

int android_image_get_ramdisk(const struct andr_img_hdr *hdr,
			      ulong *rd_data, ulong *rd_len)
{
	ulong ramdisk_addr_r;
	ulong start, end;

	if (!hdr->ramdisk_size) {
		*rd_data = *rd_len = 0;
		return -1;
	}

	/* Have been loaded by android_image_load_separate() on ramdisk_addr_r */
	ramdisk_addr_r = env_get_ulong("ramdisk_addr_r", 16, 0);
	if (!ramdisk_addr_r) {
		printf("No Found Ramdisk Load Address.\n");
		return -1;
	}

	*rd_data = ramdisk_addr_r;
	*rd_len = hdr->ramdisk_size;
	if (hdr->header_version >= 3)
		*rd_len += hdr->vendor_ramdisk_size;
	if (hdr->header_version >= 4) {
		 *rd_len += hdr->vendor_bootconfig_size +
		  ANDROID_ADDITION_BOOTCONFIG_PARAMS_MAX_SIZE;
	}

	/* just for print msg */
	start = ramdisk_addr_r;
	if (hdr->header_version >= 3) {
		end = start + (ulong)hdr->vendor_ramdisk_size;
		printf("v-ramdisk:  0x%08lx - 0x%08lx (%u KiB)\n",
		       start, end, DIV_ROUND_UP(hdr->vendor_ramdisk_size, 1024));
		start = end;
	}
	{
		end = start + (ulong)hdr->ramdisk_size;
		printf("ramdisk:    0x%08lx - 0x%08lx (%u KiB)\n",
		       start, end, DIV_ROUND_UP(hdr->ramdisk_size, 1024));
		start = end;
	}
	if (hdr->header_version >= 4) {
		end = start + (ulong)hdr->vendor_bootconfig_size;
		printf("bootconfig: 0x%08lx - 0x%08lx (%u KiB)\n",
		       start, end, DIV_ROUND_UP(hdr->vendor_bootconfig_size, 1024));
		start = end;
		end = start + ANDROID_ADDITION_BOOTCONFIG_PARAMS_MAX_SIZE;
		printf("bootparams: 0x%08lx - 0x%08lx\n", start, end);
	}

	return 0;
}

int android_image_get_fdt(const struct andr_img_hdr *hdr,
			      ulong *rd_data)
{
	ulong fdt_addr_r;

	if (!hdr->second_size) {
		*rd_data = 0;
		return -1;
	}

	/* Have been loaded by android_image_load_separate() on fdt_addr_r */
	fdt_addr_r = env_get_ulong("fdt_addr_r", 16, 0);
	if (!fdt_addr_r) {
		printf("No Found FDT Load Address.\n");
		return -1;
	}

	*rd_data = fdt_addr_r;

	debug("FDT load addr 0x%08x size %u KiB\n",
	      hdr->second_addr, DIV_ROUND_UP(hdr->second_size, 1024));

	return 0;
}

#ifdef CONFIG_ANDROID_BOOT_IMAGE_HASH
static void print_hash(const char *label, u8 *hash, int len)
{
	int i;

	printf("%s:\n    0x", label ? : "Hash");
	for (i = 0; i < len; i++)
		printf("%02x", hash[i]);
	printf("\n");
}
#endif

typedef enum {
	IMG_KERNEL,
	IMG_RAMDISK,	/* within boot.img or init_boot.img(Android-13 or later) */
	IMG_SECOND,
	IMG_RECOVERY_DTBO,
	IMG_RK_DTB,	/* within resource.img in second position */
	IMG_DTB,
	IMG_VENDOR_RAMDISK,
	IMG_BOOTCONFIG,
	IMG_MAX,
} img_t;

#if defined(CONFIG_ANDROID_BOOT_IMAGE_HASH) && !defined(CONFIG_DM_CRYPTO)
static sha1_context sha1_ctx;
#endif

static int image_load(img_t img, struct andr_img_hdr *hdr,
		      ulong blkstart, void *ram_base,
		      struct udevice *crypto)
{
	struct blk_desc *desc = rockchip_get_bootdev();
	disk_partition_t part_vendor_boot;
	disk_partition_t part_init_boot;
	__maybe_unused u32 typesz;
	u32 andr_version = (hdr->os_version >> 25) & 0x7f;
	ulong pgsz = hdr->page_size;
	ulong blksz = desc->blksz;
	ulong blkcnt, blkoff;
	ulong memmove_dst = 0;
	ulong bsoffs = 0;
	ulong extra = 0;
	ulong length;
	void *buffer;
	void *tmp = NULL;
	int ret = 0;

	switch (img) {
	case IMG_KERNEL:
		bsoffs = 0; /* include a page_size(image header) */
		length = hdr->kernel_size + pgsz;
		buffer = (void *)env_get_ulong("android_addr_r", 16, 0);
		blkcnt = DIV_ROUND_UP(hdr->kernel_size + pgsz, blksz);
		typesz = sizeof(hdr->kernel_size);
		if (!sysmem_alloc_base(MEM_KERNEL,
			(phys_addr_t)buffer, blkcnt * blksz))
			return -ENOMEM;
		break;
	case IMG_VENDOR_RAMDISK:
		if (hdr->vendor_boot_buf) {
			ram_base = hdr->vendor_boot_buf;
		} else {
			if (part_get_info_by_name(desc,
						  ANDROID_PARTITION_VENDOR_BOOT,
						  &part_vendor_boot) < 0) {
				printf("No vendor boot partition\n");
				return -ENOENT;
			}
			ram_base = 0;
		}

		blkstart = part_vendor_boot.start;
		pgsz = hdr->vendor_page_size;
		bsoffs = ALIGN(VENDOR_BOOT_HDRv3_SIZE, pgsz);
		length = hdr->vendor_ramdisk_size;
		buffer = (void *)env_get_ulong("ramdisk_addr_r", 16, 0);
		blkcnt = DIV_ROUND_UP(hdr->vendor_ramdisk_size, blksz);
		typesz = sizeof(hdr->vendor_ramdisk_size);
		/*
		 * Add extra memory for generic ramdisk space.
		 *
		 * In case of unaligned vendor ramdisk size, reserve
		 * 1 more blksz.
		 *
		 * Reserve 8KB for bootloader cmdline.
		 */
		if (hdr->header_version >= 3)
			extra += ALIGN(hdr->ramdisk_size, blksz) + blksz;
		if (hdr->header_version >= 4)
			extra += ALIGN(hdr->vendor_bootconfig_size, blksz) +
				 ANDROID_ADDITION_BOOTCONFIG_PARAMS_MAX_SIZE;
		if (length && !sysmem_alloc_base(MEM_RAMDISK,
			(phys_addr_t)buffer, blkcnt * blksz + extra))
			return -ENOMEM;
		break;
	case IMG_RAMDISK:
		/* get ramdisk from init_boot.img ? */
		if (hdr->header_version >= 4 && andr_version >= 13) {
			if (hdr->init_boot_buf) {
				ram_base = hdr->init_boot_buf;
			} else {
				if (part_get_info_by_name(desc,
				    ANDROID_PARTITION_INIT_BOOT, &part_init_boot) < 0) {
					printf("No init boot partition\n");
					return -ENOENT;
				}
				blkstart = part_init_boot.start;
				ram_base = 0;
			}
			bsoffs = pgsz;
		} else {
			/* get ramdisk from generic boot.img */
			bsoffs = pgsz + ALIGN(hdr->kernel_size, pgsz);
		}

		length = hdr->ramdisk_size;
		buffer = (void *)env_get_ulong("ramdisk_addr_r", 16, 0);
		blkcnt = DIV_ROUND_UP(hdr->ramdisk_size, blksz);
		typesz = sizeof(hdr->ramdisk_size);

		/*
		 * ramdisk_addr_r v012:
		 *	|----------------|
		 *	|    ramdisk     |
		 *	|----------------|
		 *
		 * ramdisk_addr_r v3 (Android-11 and later):
		 *	|----------------|---------|
		 *	| vendor-ramdisk | ramdisk |
		 *	|----------------|---------|
		 *
		 * ramdisk_addr_r v4 (Android-12 and later):
		 *	|----------------|---------|------------|------------|
		 *	| vendor-ramdisk | ramdisk | bootconfig | bootparams |
		 *	|----------------|---------|------------|------------|
		 *
		 * ramdisk_addr_r v4 + init_boot(Android-13 and later):
		 *	|----------------|----------------|------------|------------|
		 *	| vendor-ramdisk | (init_)ramdisk | bootconfig | bootparams |
		 *	|----------------|----------------|------------|------------|
		 */
		if (hdr->header_version >= 3) {
			buffer += hdr->vendor_ramdisk_size;
			if (!IS_ALIGNED((ulong)buffer, blksz)) {
				memmove_dst = (ulong)buffer;
				buffer = (void *)ALIGN(memmove_dst, blksz);
			}
		}
		/* sysmem has been alloced by vendor ramdisk */
		if (hdr->header_version < 3) {
			if (length && !sysmem_alloc_base(MEM_RAMDISK,
				(phys_addr_t)buffer, blkcnt * blksz))
				return -ENOMEM;
		}
		break;
	case IMG_BOOTCONFIG:
		if (hdr->header_version < 4)
			return 0;

		if (hdr->vendor_boot_buf) {
			ram_base = hdr->vendor_boot_buf;
		} else {
			if (part_get_info_by_name(desc,
						  ANDROID_PARTITION_VENDOR_BOOT,
						  &part_vendor_boot) < 0) {
				printf("No vendor boot partition\n");
				return -ENOENT;
			}
			ram_base = 0;
		}
		blkstart = part_vendor_boot.start;
		pgsz = hdr->vendor_page_size;
		bsoffs = ALIGN(VENDOR_BOOT_HDRv4_SIZE, pgsz) +
			 ALIGN(hdr->vendor_ramdisk_size, pgsz) +
			 ALIGN(hdr->dtb_size, pgsz) +
			 ALIGN(hdr->vendor_ramdisk_table_size, pgsz);
		length = hdr->vendor_bootconfig_size;
		buffer = (void *)env_get_ulong("ramdisk_addr_r", 16, 0);
		blkcnt = DIV_ROUND_UP(hdr->vendor_bootconfig_size, blksz);
		typesz = sizeof(hdr->vendor_bootconfig_size);

		buffer += hdr->vendor_ramdisk_size + hdr->ramdisk_size;
		if (!IS_ALIGNED((ulong)buffer, blksz)) {
			memmove_dst = (ulong)buffer;
			buffer = (void *)ALIGN(memmove_dst, blksz);
		}
		break;
	case IMG_SECOND:
		bsoffs = pgsz +
			 ALIGN(hdr->kernel_size, pgsz) +
			 ALIGN(hdr->ramdisk_size, pgsz);
		length = hdr->second_size;
		blkcnt = DIV_ROUND_UP(hdr->second_size, blksz);
		buffer = tmp = malloc(blkcnt * blksz);
		typesz = sizeof(hdr->second_size);
		break;
	case IMG_RECOVERY_DTBO:
		bsoffs = pgsz +
			 ALIGN(hdr->kernel_size, pgsz) +
			 ALIGN(hdr->ramdisk_size, pgsz) +
			 ALIGN(hdr->second_size, pgsz);
		length = hdr->recovery_dtbo_size;
		blkcnt = DIV_ROUND_UP(hdr->recovery_dtbo_size, blksz);
		buffer = tmp = malloc(blkcnt * blksz);
		typesz = sizeof(hdr->recovery_dtbo_size);
		break;
	case IMG_DTB:
		bsoffs = pgsz +
			 ALIGN(hdr->kernel_size, pgsz) +
			 ALIGN(hdr->ramdisk_size, pgsz) +
			 ALIGN(hdr->second_size, pgsz) +
			 ALIGN(hdr->recovery_dtbo_size, pgsz);
		length = hdr->dtb_size;
		blkcnt = DIV_ROUND_UP(hdr->dtb_size, blksz);
		buffer = tmp = malloc(blkcnt * blksz);
		typesz = sizeof(hdr->dtb_size);
		break;
	case IMG_RK_DTB:
#ifdef CONFIG_RKIMG_BOOTLOADER
		/* No going further, it handles DTBO, HW-ID, etc */
		buffer = (void *)env_get_ulong("fdt_addr_r", 16, 0);
		if (gd->fdt_blob != (void *)buffer)
			ret = rockchip_read_dtb_file(buffer);
#endif
		return ret < 0 ? ret : 0;
	default:
		return -EINVAL;
	}

	if (!buffer) {
		printf("No memory for image(%d)\n", img);
		return -ENOMEM;
	}

	if (!blksz || !length)
		goto crypto_calc;

	/* load */
	if (ram_base) {
		memcpy(buffer, (char *)((ulong)ram_base + bsoffs), length);
	} else {
		blkoff = DIV_ROUND_UP(bsoffs, blksz);
		ret = blk_dread(desc, blkstart + blkoff, blkcnt, buffer);
		if (ret != blkcnt) {
			printf("Failed to read img(%d), ret=%d\n", img, ret);
			return -EIO;
		}
	}

	if (memmove_dst)
		memmove((char *)memmove_dst, buffer, length);

crypto_calc:
	if (img == IMG_KERNEL) {
		buffer += pgsz;
		length -= pgsz;
	}

	/* sha1 */
	if (hdr->header_version < 3) {
#ifdef CONFIG_ANDROID_BOOT_IMAGE_HASH
#ifdef CONFIG_DM_CRYPTO
		crypto_sha_update(crypto, (u32 *)buffer, length);
		crypto_sha_update(crypto, (u32 *)&length, typesz);
#else
		sha1_update(&sha1_ctx, (void *)buffer, length);
		sha1_update(&sha1_ctx, (void *)&length, typesz);
#endif
#endif
	}

	if (tmp)
		free(tmp);

	return 0;
}

static int images_load_verify(struct andr_img_hdr *hdr, ulong part_start,
			      void *ram_base, struct udevice *crypto)
{
	/* load, never change order ! */
	if (image_load(IMG_KERNEL, hdr, part_start, ram_base, crypto))
		return -1;
	if (image_load(IMG_RAMDISK, hdr, part_start, ram_base, crypto))
		return -1;
	if (image_load(IMG_SECOND, hdr, part_start, ram_base, crypto))
		return -1;
	if (hdr->header_version > 0) {
		if (image_load(IMG_RECOVERY_DTBO, hdr, part_start,
			       ram_base, crypto))
			return -1;
	}
	if (hdr->header_version > 1) {
		if (image_load(IMG_DTB, hdr, part_start, ram_base, crypto))
			return -1;
	}

	return 0;
}

/*
 * @ram_base: !NULL means require memcpy for an exist full android image.
 */
static int android_image_separate(struct andr_img_hdr *hdr,
				  const disk_partition_t *part,
				  void *load_address,
				  void *ram_base)
{
	ulong bstart;
	int ret;

	if (android_image_check_header(hdr)) {
		printf("Bad android image header\n");
		return -EINVAL;
	}

	/* set for image_load(IMG_KERNEL, ...) */
	env_set_hex("android_addr_r", (ulong)load_address);
	bstart = part ? part->start : 0;

	/*
	 * 1. Load images to their individual target ram position
	 *    in order to disable fdt/ramdisk relocation.
	 */

	/* load rk-kernel.dtb alone */
	if (image_load(IMG_RK_DTB, hdr, bstart, ram_base, NULL))
		return -1;

#ifdef CONFIG_ANDROID_BOOT_IMAGE_HASH
	if (hdr->header_version < 3) {
		struct udevice *dev = NULL;
		uchar hash[20];
#ifdef CONFIG_DM_CRYPTO
		sha_context ctx;

		ctx.length = 0;
		ctx.algo = CRYPTO_SHA1;
		dev = crypto_get_device(ctx.algo);
		if (!dev) {
			printf("Can't find crypto device for SHA1\n");
			return -ENODEV;
		}

		/* v1 & v2: requires total length before sha init */
		ctx.length += hdr->kernel_size + sizeof(hdr->kernel_size) +
			      hdr->ramdisk_size + sizeof(hdr->ramdisk_size) +
			      hdr->second_size + sizeof(hdr->second_size);
		if (hdr->header_version > 0)
			ctx.length += hdr->recovery_dtbo_size +
						sizeof(hdr->recovery_dtbo_size);
		if (hdr->header_version > 1)
			ctx.length += hdr->dtb_size + sizeof(hdr->dtb_size);
		crypto_sha_init(dev, &ctx);
#else
		sha1_starts(&sha1_ctx);
#endif
		ret = images_load_verify(hdr, bstart, ram_base, dev);
		if (ret)
			return ret;

#ifdef CONFIG_DM_CRYPTO
		crypto_sha_final(dev, &ctx, hash);
#else
		sha1_finish(&sha1_ctx, hash);
#endif
		if (memcmp(hash, hdr->id, 20)) {
			print_hash("Hash from header", (u8 *)hdr->id, 20);
			print_hash("Hash real", (u8 *)hash, 20);
			return -EBADFD;
		} else {
			printf("ANDROID: Hash OK\n");
		}
	} else
#endif
	{
		ret = images_load_verify(hdr, bstart, ram_base, NULL);
		if (ret)
			return ret;
	}

	/* 2. Disable fdt/ramdisk relocation, it saves boot time */
	env_set("bootm-no-reloc", "y");

	return 0;
}

static int android_image_separate_v34(struct andr_img_hdr *hdr,
				      const disk_partition_t *part,
				      void *load_address, void *ram_base)
{
	ulong bstart;

	if (android_image_check_header(hdr)) {
		printf("Bad android image header\n");
		return -EINVAL;
	}

	/* set for image_load(IMG_KERNEL, ...) */
	env_set_hex("android_addr_r", (ulong)load_address);
	bstart = part ? part->start : 0;

	/*
	 * 1. Load images to their individual target ram position
	 *    in order to disable fdt/ramdisk relocation.
	 */
	if (image_load(IMG_RK_DTB,  hdr, bstart, ram_base, NULL))
		return -1;
	if (image_load(IMG_KERNEL,  hdr, bstart, ram_base, NULL))
		return -1;
	if (image_load(IMG_VENDOR_RAMDISK, hdr, bstart, ram_base, NULL))
		return -1;
	if (image_load(IMG_RAMDISK, hdr, bstart, ram_base, NULL))
		return -1;
	if (image_load(IMG_BOOTCONFIG, hdr, bstart, ram_base, NULL))
		return -1;
	/*
	 * Copy the populated hdr to load address after image_load(IMG_KERNEL)
	 *
	 * The image_load(IMG_KERNEL) only reads boot_img_hdr_v34 while
	 * vendor_boot_img_hdr_v34 is not included, so fix it here.
	 */
	memcpy((char *)load_address, hdr, hdr->page_size);

	/* 2. Disable fdt/ramdisk relocation, it saves boot time */
	env_set("bootm-no-reloc", "y");

	return 0;
}

static ulong android_image_get_comp_addr(struct andr_img_hdr *hdr, int comp)
{
	ulong kernel_addr_c;
	ulong load_addr = 0;

	kernel_addr_c = env_get_ulong("kernel_addr_c", 16, 0);

#ifdef CONFIG_ARM64
	/*
	 * On 64-bit kernel, assuming use IMAGE by default.
	 *
	 * kernel_addr_c is for LZ4-IMAGE but maybe not defined.
	 * kernel_addr_r is for IMAGE.
	 */
	if (comp != IH_COMP_NONE) {
		ulong comp_addr;

		if (kernel_addr_c) {
			comp_addr = kernel_addr_c;
		} else {
			printf("Warn: No \"kernel_addr_c\"\n");
			comp_addr = CONFIG_SYS_SDRAM_BASE + 0x2000000;/* 32M */
			env_set_hex("kernel_addr_c", comp_addr);
		}

		load_addr = comp_addr - hdr->page_size;
	}
#else
	/*
	 * On 32-bit kernel:
	 *
	 * The input load_addr is from env value: "kernel_addr_r", it has
	 * different role depends on whether kernel_addr_c is defined:
	 *
	 * - kernel_addr_r is for lz4/zImage if kernel_addr_c if [not] defined.
	 * - kernel_addr_r is for IMAGE if kernel_addr_c is defined.
	 */
	if (comp == IH_COMP_NONE) {
		if (kernel_addr_c) {
			/* input load_addr is for Image, nothing to do */
		} else {
			/* input load_addr is for lz4/zImage, set default addr for Image */
			load_addr = CONFIG_SYS_SDRAM_BASE + 0x8000;
			env_set_hex("kernel_addr_r", load_addr);

			load_addr -= hdr->page_size;
		}
	} else {
		if (kernel_addr_c) {
			/* input load_addr is for Image, so use another for lz4/zImage */
			load_addr = kernel_addr_c - hdr->page_size;
		} else {
			/* input load_addr is for lz4/zImage, nothing to do */
		}
	}
#endif

	return load_addr;
}

void android_image_set_decomp(struct andr_img_hdr *hdr, int comp)
{
	ulong kernel_addr_r;

	env_set_ulong("os_comp", comp);

	/* zImage handles decompress itself */
	if (comp != IH_COMP_NONE && comp != IH_COMP_ZIMAGE) {
		kernel_addr_r = env_get_ulong("kernel_addr_r", 16, 0x02080000);
		android_image_set_kload(hdr, kernel_addr_r);
		android_image_set_comp(hdr, comp);
	} else {
		android_image_set_comp(hdr, IH_COMP_NONE);
	}
}

static int android_image_load_separate(struct andr_img_hdr *hdr,
				       const disk_partition_t *part,
				       void *load_addr)
{
	if (hdr->header_version < 3)
		return android_image_separate(hdr, part, load_addr, NULL);
	else
		return android_image_separate_v34(hdr, part, load_addr, NULL);
}

int android_image_memcpy_separate(struct andr_img_hdr *hdr, ulong *load_addr)
{
	ulong comp_addr;
	int comp;

	comp = bootm_parse_comp((void *)(ulong)hdr + hdr->page_size);
	comp_addr = android_image_get_comp_addr(hdr, comp);

	/* non-compressed image: already in-place */
	if ((ulong)hdr == *load_addr)
		return 0;

	/* compressed image */
	if (comp_addr) {
		*load_addr = comp_addr;
		if ((ulong)hdr == comp_addr)	/* already in-place */
			return 0;
	}

	/*
	 * The most possible reason to arrive here is:
	 *
	 * VBoot=1 and AVB load full partition to a temp memory buffer, now we
	 * separate(memcpy) subimages from boot.img to where they should be.
	 */
	if (hdr->header_version < 3) {
		if (android_image_separate(hdr, NULL, (void *)(*load_addr), hdr))
			return -1;
	} else {
		if (android_image_separate_v34(hdr, NULL, (void *)(*load_addr), hdr))
			return -1;
	}

	android_image_set_decomp((void *)(*load_addr), comp);

	return 0;
}

long android_image_load(struct blk_desc *dev_desc,
			const disk_partition_t *part_info,
			unsigned long load_address,
			unsigned long max_size) {
	struct andr_img_hdr *hdr;
	ulong comp_addr;
	int comp, ret;
	int blk_off;

	if (max_size < part_info->blksz)
		return -1;

	hdr = populate_andr_img_hdr(dev_desc, (disk_partition_t *)part_info);
	if (!hdr) {
		printf("No valid android hdr\n");
		return -1;
	}

	/*
	 * create the layout:
	 *
	 * |<- page_size ->|1-blk |
	 * |-----|---------|------|-----|
	 * | hdr |   ...   |   kernel   |
	 * |-----|----- ---|------------|
	 *
	 * Alloc page_size and 1 more blk for reading kernel image to
	 * get it's compression type, then fill the android hdr what
	 * we have populated before.
	 *
	 * Why? see: android_image_get_kernel_addr().
	 */
	blk_off = BLK_CNT(hdr->page_size, dev_desc->blksz);
	hdr = (struct andr_img_hdr *)
			realloc(hdr, (blk_off + 1) * dev_desc->blksz);
	if (!hdr)
		return -1;

	if (blk_dread(dev_desc, part_info->start + blk_off, 1,
		      (char *)hdr + hdr->page_size) != 1) {
		free(hdr);
		return -1;
	}

	/* Changed to compressed address ? */
	comp = bootm_parse_comp((void *)(ulong)hdr + hdr->page_size);
	comp_addr = android_image_get_comp_addr(hdr, comp);
	if (comp_addr)
		load_address = comp_addr;
	else
		load_address -= hdr->page_size;

	ret = android_image_load_separate(hdr, part_info, (void *)load_address);
	if (ret) {
		printf("Failed to load android image\n");
		goto fail;
	}
	android_image_set_decomp((void *)load_address, comp);

	debug("Loading Android Image to 0x%08lx\n", load_address);

	free(hdr);
	return load_address;

fail:
	free(hdr);
	return -1;
}

static struct andr_img_hdr *
extract_boot_image_v012_header(struct blk_desc *dev_desc,
			       const disk_partition_t *boot_img)
{
	struct andr_img_hdr *hdr;
	long blk_cnt, blks_read;

	blk_cnt = BLK_CNT(sizeof(struct andr_img_hdr), dev_desc->blksz);
	hdr = (struct andr_img_hdr *)malloc(blk_cnt * dev_desc->blksz);

	if (!blk_cnt || !hdr)
		return NULL;

	blks_read = blk_dread(dev_desc, boot_img->start, blk_cnt, hdr);
	if (blks_read != blk_cnt) {
		debug("boot img header blk cnt is %ld and blks read is %ld\n",
		      blk_cnt, blks_read);
		return NULL;
	}

	if (android_image_check_header((void *)hdr)) {
		printf("boot header magic is invalid.\n");
		return NULL;
	}

	if (hdr->page_size < sizeof(*hdr)) {
		printf("android hdr is over size\n");
		return NULL;
	}

	return hdr;
}

static struct boot_img_hdr_v34 *
extract_boot_image_v34_header(struct blk_desc *dev_desc,
			      const disk_partition_t *boot_img)
{
	struct boot_img_hdr_v34 *boot_hdr;
	disk_partition_t part;
	long blk_cnt, blks_read;

	blk_cnt = BLK_CNT(sizeof(struct boot_img_hdr_v34), dev_desc->blksz);
	boot_hdr = (struct boot_img_hdr_v34 *)malloc(blk_cnt * dev_desc->blksz);

	if (!blk_cnt || !boot_hdr)
		return NULL;

	blks_read = blk_dread(dev_desc, boot_img->start, blk_cnt, boot_hdr);
	if (blks_read != blk_cnt) {
		debug("boot img header blk cnt is %ld and blks read is %ld\n",
		      blk_cnt, blks_read);
		return NULL;
	}

	if (android_image_check_header((void *)boot_hdr)) {
		printf("boot header magic is invalid.\n");
		return NULL;
	}

	if (boot_hdr->header_version < 3) {
		printf("boot header %d, is not >= v3.\n",
		       boot_hdr->header_version);
		return NULL;
	}

	/* Start from android-13 GKI, it doesn't assign 'os_version' */
	if (boot_hdr->header_version >= 4 && boot_hdr->os_version == 0) {
		if (part_get_info_by_name(dev_desc,
				ANDROID_PARTITION_INIT_BOOT, &part) > 0)
			boot_hdr->os_version = 13 << 25;
	}

	return boot_hdr;
}

static struct vendor_boot_img_hdr_v34 *
extract_vendor_boot_image_v34_header(struct blk_desc *dev_desc,
				     const disk_partition_t *part_vendor_boot)
{
	struct vendor_boot_img_hdr_v34 *vboot_hdr;
	long blk_cnt, blks_read;

	blk_cnt = BLK_CNT(sizeof(struct vendor_boot_img_hdr_v34),
				part_vendor_boot->blksz);
	vboot_hdr = (struct vendor_boot_img_hdr_v34 *)
				malloc(blk_cnt * part_vendor_boot->blksz);

	if (!blk_cnt || !vboot_hdr)
		return NULL;

	blks_read = blk_dread(dev_desc, part_vendor_boot->start,
			      blk_cnt, vboot_hdr);
	if (blks_read != blk_cnt) {
		debug("vboot img header blk cnt is %ld and blks read is %ld\n",
		      blk_cnt, blks_read);
		return NULL;
	}

	if (strncmp(VENDOR_BOOT_MAGIC, (void *)vboot_hdr->magic,
		    VENDOR_BOOT_MAGIC_SIZE)) {
		printf("vendor boot header is invalid.\n");
		return NULL;
	}

	if (vboot_hdr->header_version < 3) {
		printf("vendor boot header %d, is not >= v3.\n",
		       vboot_hdr->header_version);
		return NULL;
	}

	return vboot_hdr;
}

int populate_boot_info(const struct boot_img_hdr_v34 *boot_hdr,
		       const struct vendor_boot_img_hdr_v34 *vendor_boot_hdr,
		       const struct boot_img_hdr_v34 *init_boot_hdr,
		       struct andr_img_hdr *hdr, bool save_hdr)
{
	memset(hdr->magic, 0, ANDR_BOOT_MAGIC_SIZE);
	memcpy(hdr->magic, boot_hdr->magic, ANDR_BOOT_MAGIC_SIZE);

	hdr->kernel_size = boot_hdr->kernel_size;
	/* don't use vendor_boot_hdr->kernel_addr, we prefer "hdr + hdr->page_size" */
	hdr->kernel_addr = ANDROID_IMAGE_DEFAULT_KERNEL_ADDR;

	/*
	 * generic ramdisk: immediately following the vendor ramdisk.
	 * It can be from init_boot.img or boot.img.
	 */
	if (init_boot_hdr)
		hdr->ramdisk_size = init_boot_hdr->ramdisk_size;
	else
		hdr->ramdisk_size = boot_hdr->ramdisk_size;

	/* actually, useless */
	hdr->ramdisk_addr = env_get_ulong("ramdisk_addr_r", 16, 0);

	/* removed in v3 */
	hdr->second_size = 0;
	hdr->second_addr = 0;

	hdr->tags_addr = vendor_boot_hdr->tags_addr;

	/* fixed in v3 */
	hdr->page_size = 4096;
	hdr->header_version = boot_hdr->header_version;
	hdr->os_version = boot_hdr->os_version;

	memset(hdr->name, 0, ANDR_BOOT_NAME_SIZE);
	strncpy(hdr->name, (const char *)vendor_boot_hdr->name, ANDR_BOOT_NAME_SIZE);

	/* removed in v3 */
	memset(hdr->cmdline, 0, ANDR_BOOT_ARGS_SIZE);
	memset(hdr->id, 0, 32);
	memset(hdr->extra_cmdline, 0, ANDR_BOOT_EXTRA_ARGS_SIZE);
	hdr->recovery_dtbo_size = 0;
	hdr->recovery_dtbo_offset = 0;

	hdr->header_size = boot_hdr->header_size;
	hdr->dtb_size = vendor_boot_hdr->dtb_size;
	hdr->dtb_addr = vendor_boot_hdr->dtb_addr;

	/* boot_img_hdr_v34 fields */
	hdr->vendor_ramdisk_size = vendor_boot_hdr->vendor_ramdisk_size;
	hdr->vendor_page_size = vendor_boot_hdr->page_size;
	hdr->vendor_header_version = vendor_boot_hdr->header_version;
	hdr->vendor_header_size = vendor_boot_hdr->header_size;

	hdr->total_cmdline = calloc(1, TOTAL_BOOT_ARGS_SIZE);
	if (!hdr->total_cmdline)
		return -ENOMEM;
	strncpy(hdr->total_cmdline, (const char *)boot_hdr->cmdline,
		sizeof(boot_hdr->cmdline));
	strncat(hdr->total_cmdline, " ", 1);
	strncat(hdr->total_cmdline, (const char *)vendor_boot_hdr->cmdline,
		sizeof(vendor_boot_hdr->cmdline));

	/* new for header v4 */
	if (vendor_boot_hdr->header_version >= 4) {
		hdr->vendor_ramdisk_table_size =
				vendor_boot_hdr->vendor_ramdisk_table_size;
		hdr->vendor_ramdisk_table_entry_num =
				vendor_boot_hdr->vendor_ramdisk_table_entry_num;
		hdr->vendor_ramdisk_table_entry_size =
				vendor_boot_hdr->vendor_ramdisk_table_entry_size;
		/*
		 * If we place additional "androidboot.xxx" parameters after
		 * bootconfig, this field value should be increased,
		 * but not over than ANDROID_ADDITION_BOOTCONFIG_PARAMS_MAX_SIZE.
		 */
		hdr->vendor_bootconfig_size =
				vendor_boot_hdr->vendor_bootconfig_size;
	} else {
		hdr->vendor_ramdisk_table_size = 0;
		hdr->vendor_ramdisk_table_entry_num = 0;
		hdr->vendor_ramdisk_table_entry_size = 0;
		hdr->vendor_bootconfig_size = 0;
	}

	hdr->init_boot_buf = save_hdr ? (void *)init_boot_hdr : 0;
	hdr->vendor_boot_buf = save_hdr ? (void *)vendor_boot_hdr : 0;

	if (hdr->page_size < sizeof(*hdr)) {
		printf("android hdr is over size\n");
		return -EINVAL;
	}

	return 0;
}

/*
 * The possible cases of boot.img + recovery.img:
 *
 * [N]: 0, 1, 2
 * [M]: 0, 1, 2, 3, 4
 *
 * |--------------------|---------------------|
 * |   boot.img         |    recovery.img     |
 * |--------------------|---------------------|
 * | boot_img_hdr_v[N]  |  boot_img_hdr_v[N]  | <= if A/B is not required
 * |--------------------|---------------------|
 * | boot_img_hdr_v34   |  boot_img_hdr_v2    | <= if A/B is not required
 * |------------------------------------------|
 * | boot_img_hdr_v[M], no recovery.img       | <= if A/B is required
 * |------------------------------------------|
 */
struct andr_img_hdr *populate_andr_img_hdr(struct blk_desc *dev_desc,
					   disk_partition_t *part_boot)
{
	disk_partition_t part_vendor_boot;
	disk_partition_t part_init_boot;
	struct vendor_boot_img_hdr_v34 *vboot_hdr = NULL;
	struct boot_img_hdr_v34 *iboot_hdr = NULL;
	struct boot_img_hdr_v34 *boot_hdr = NULL;
	struct andr_img_hdr *andr_hdr = NULL;
	int header_version;
	int andr_version;

	if (!dev_desc || !part_boot)
		return NULL;

	andr_hdr = (struct andr_img_hdr *)malloc(1 * dev_desc->blksz);
	if (!andr_hdr)
		return NULL;

	if (blk_dread(dev_desc, part_boot->start, 1, andr_hdr) != 1) {
		free(andr_hdr);
		return NULL;
	}

	if (android_image_check_header(andr_hdr)) {
		free(andr_hdr);
		return NULL;
	}

	header_version = andr_hdr->header_version;
	free(andr_hdr);
	andr_hdr = NULL;

	if (header_version < 3) {
		return extract_boot_image_v012_header(dev_desc, part_boot);
	} else {
		if (part_get_info_by_name(dev_desc,
					  ANDROID_PARTITION_VENDOR_BOOT,
					  &part_vendor_boot) < 0) {
			printf("No vendor boot partition\n");
			return NULL;
		}
		boot_hdr = extract_boot_image_v34_header(dev_desc, part_boot);
		vboot_hdr = extract_vendor_boot_image_v34_header(dev_desc,
							&part_vendor_boot);
		if (!boot_hdr || !vboot_hdr)
			goto image_load_exit;

		andr_version = (boot_hdr->os_version >> 25) & 0x7f;
		if (header_version >= 4 && andr_version >= 13) {
			if (part_get_info_by_name(dev_desc,
						  ANDROID_PARTITION_INIT_BOOT,
						  &part_init_boot) < 0) {
				printf("No init boot partition\n");
				return NULL;
			}
			iboot_hdr = extract_boot_image_v34_header(dev_desc, &part_init_boot);
			if (!iboot_hdr)
				goto image_load_exit;
			if (!iboot_hdr->ramdisk_size) {
				printf("No ramdisk in init boot partition\n");
				goto image_load_exit;
			}
		}

		andr_hdr = (struct andr_img_hdr *)
				malloc(sizeof(struct andr_img_hdr));
		if (!andr_hdr) {
			printf("No memory for andr hdr\n");
			goto image_load_exit;
		}

		if (populate_boot_info(boot_hdr, vboot_hdr,
				       iboot_hdr, andr_hdr, false)) {
			printf("populate boot info failed\n");
			goto image_load_exit;
		}

image_load_exit:
		if (boot_hdr)
			free(boot_hdr);
		if (iboot_hdr)
			free(iboot_hdr);
		if (vboot_hdr)
			free(vboot_hdr);

		return andr_hdr;
	}

	return NULL;
}

#if !defined(CONFIG_SPL_BUILD)
/**
 * android_print_contents - prints out the contents of the Android format image
 * @hdr: pointer to the Android format image header
 *
 * android_print_contents() formats a multi line Android image contents
 * description.
 * The routine prints out Android image properties
 *
 * returns:
 *     no returned results
 */
void android_print_contents(const struct andr_img_hdr *hdr)
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
