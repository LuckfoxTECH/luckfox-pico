/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 * Author: Wenping Zhang <wenping.zhang@rock-chips.com>
 */
#include <common.h>
#include <dm.h>
#include <stdio.h>
#include <errno.h>
#include <mapmem.h>
#include <stdlib.h>
#include <asm/arch/vendor.h>
#include <dm/of_access.h>
#include <dm/uclass.h>
#include <dm/uclass-id.h>
#include <boot_rkimg.h>
#include <rk_eink.h>
#include <backlight.h>
#include "rk_ebc.h"
#include "epdlut/epd_lut.h"

#define PART_WAVEFORM		"waveform"
#define EINK_LOGO_PART_MAGIC	"RKEL"
#define EINK_LOGO_IMAGE_MAGIC	"GR04"
/*
 * grayscale logo partition format:
 * block0:
 * struct logo_part_header part_header;
 * struct grayscale_header logo1_header;
 * struct grayscale_header logo2_header;
 * struct grayscale_header logo3_header;
 * struct grayscale_header logo4_header;
 * ....
 *
 * block 1:
 * logo1_image
 *
 * .....
 * block m:
 * logo2_image
 *
 * ........
 * block n:
 * logo3_image
 *
 * ........
 * block i:
 * logoi_image
 */

//logo partition Header, 64byte
struct logo_part_header {
	char magic[4]; /* must be "RKEL" */
	u32  totoal_size;
	u32  screen_width;
	u32  screen_height;
	u32  logo_count;
	u8   version[4];
	u32  rsv[10];
} __packed;

// logo image header,32 byte
struct grayscale_header {
	char magic[4]; /* must be "GR04" */
	u16 x;
	u16 y;
	u16 w;
	u16 h;
	u32 logo_type;
	u32 data_offset; /* image offset in byte */
	u32 data_size; /* image size in byte */
	u32 rsv[2];
} __packed;

/*
 * The start address of logo image in logo.img must be aligned in 512 bytes,
 * so the header size must be times of 512 bytes. Here we fix the size to 512
 * bytes, so the count of logo image can only support up to 14.
 */
struct logo_info {
	struct logo_part_header part_hdr;
	struct grayscale_header img_hdr[14];
} __packed;

struct rockchip_eink_display_priv {
	struct udevice *dev;
	struct udevice *ebc_tcon_dev;
	struct udevice *ebc_pwr_dev;
	int vcom;
	struct udevice *backlight;
};

enum {
	EBC_PWR_DOWN = 0,
	EBC_PWR_ON = 1,
};

#define EINK_VCOM_ID		17
#define EINK_VCOM_MAX		64
#define VCOM_DEFAULT_VALUE	1650

static struct logo_info eink_logo_info;
static struct udevice *eink_dev;
static volatile int last_logo_type = -1;
static int read_vcom_from_vendor(void)
{
	int ret = 0;
	char vcom_str[EINK_VCOM_MAX] = {0};
	char vcom_args[EINK_VCOM_MAX] = {0};

	/* Read vcom value from vendor storage part */
	ret = vendor_storage_read(EINK_VCOM_ID, vcom_str, (EINK_VCOM_MAX - 1));
	if (ret > 0) {
		snprintf(vcom_args, strlen(vcom_str) + 15, "ebc_pmic.vcom=%s", vcom_str);
		printf("eink update bootargs: %s\n", vcom_args);
		env_update("bootargs", vcom_args);
	} else {
		return ret;
	}

	return atoi(vcom_str);
}

static int read_waveform(struct udevice *dev)
{
	int cnt, start, ret;
	disk_partition_t part;
	struct blk_desc *dev_desc;
	struct ebc_panel *plat = dev_get_platdata(dev);

	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: Could not find device\n", __func__);
		return -EIO;
	}
	if (part_get_info_by_name(dev_desc, PART_WAVEFORM, &part) < 0) {
		printf("Get waveform partition failed\n");
		return -ENODEV;
	}
	cnt = plat->lut_pbuf_size / RK_BLK_SIZE;
	start = part.start;
	ret = blk_dread(dev_desc, start, cnt, (void *)plat->lut_pbuf);
	if (ret != cnt)
		printf("Try to read %d blocks failed, only read %d\n",
		       cnt, ret);

	flush_dcache_range((ulong)plat->lut_pbuf,
			   ALIGN((ulong)plat->lut_pbuf + cnt,
				 CONFIG_SYS_CACHELINE_SIZE));
	ret = epd_lut_from_mem_init(plat->lut_pbuf, &plat->lut_ops);
	if (ret < 0) {
		printf("lut init failed\n");
		return -EINVAL;
	}

	return 0;
}

static u32 aligned_image_size_4k(struct udevice *dev)
{
	struct ebc_panel *plat = dev_get_platdata(dev);
	u32 w = plat->vir_width;
	u32 h = plat->vir_height;

	return ALIGN((w * h) >> 1, 4096);
}

/*
 * This driver load the grayscale image from flash,
 * and put it in the reserve memory which define in dts:
 * display_reserved: framebuffer@10000000 {
 *	reg = <0x0 0x10000000 0x0 0x2000000>;
 *	no-map;
 * };
 * Every image logo size must be aligned in 4K, make sure
 * kernel can use it rightly, the buffer of LOGO image is
 * put in order of below map:
 *  |---reset logo        ---|
 *  |---uboot logo        ---|
 *  |---kernel logo       ---|
 *  |---charge_0 logo   ---|
 *  |---charge_1 logo   ---|
 *  |---charge_2 logo   ---|
 *  |---charge_3 logo   ---|
 *  |---charge_4 logo   ---|
 *  |---charge_5 logo   ---|
 *  |---battery low logo---|
 *  |---temp un-mirror buffer--|
 */
static int get_addr_by_type(struct udevice *dev, u32 logo_type)
{
	u32 offset, indx, img_size;
	struct ebc_panel *plat = dev_get_platdata(dev);

	if (plat->disp_pbuf_size == 0 || !plat->disp_pbuf) {
		printf("invalid display buffer, please check dts\n");
		return -EINVAL;
	}
	indx = ffs(logo_type);
	img_size = aligned_image_size_4k(dev);
	offset = img_size * indx;
	if (offset + img_size > plat->disp_pbuf_size) {
		printf("reserve display memory size is not enough\n");
		return -EINVAL;
	}

	switch (logo_type) {
	case EINK_LOGO_RESET:
	case EINK_LOGO_UBOOT:
	case EINK_LOGO_KERNEL:
	case EINK_LOGO_CHARGING_0:
	case EINK_LOGO_CHARGING_1:
	case EINK_LOGO_CHARGING_2:
	case EINK_LOGO_CHARGING_3:
	case EINK_LOGO_CHARGING_4:
	case EINK_LOGO_CHARGING_5:
	case EINK_LOGO_CHARGING_LOWPOWER:
	case EINK_LOGO_POWEROFF:
	/*
	 * The MIRROR_TEMP_BUF is used to save the
	 * non-mirror image data.
	 */
	case EINK_LOGO_UNMIRROR_TEMP_BUF:
		return (plat->disp_pbuf + offset);
	default:
		printf("invalid logo type[%d]\n", logo_type);
	}

	return -EINVAL;
}

static int read_header(struct blk_desc *dev_desc,
		       disk_partition_t *part,
		       struct logo_info *header)
{
	int i;
	struct logo_part_header *part_hdr = &header->part_hdr;

	if (blk_dread(dev_desc, part->start, 1, header) != 1)
		return -EIO;

	if (memcmp(part_hdr->magic, EINK_LOGO_PART_MAGIC, 4)) {
		printf("partition header is invalid\n");
		return -EINVAL;
	}
	if (part_hdr->logo_count == 0) {
		printf("the count of logo image is 0\n");
		return -EINVAL;
	}
	for (i = 0; i < part_hdr->logo_count; i++) {
		struct grayscale_header *img_hdr = &header->img_hdr[i];

		if (memcmp(img_hdr->magic, EINK_LOGO_IMAGE_MAGIC, 4)) {
			printf("image[%d] header '%s' is invalid\n", i,
			       img_hdr->magic);
			return -EINVAL;
		}
	}

	return 0;
}

static int read_grayscale(struct blk_desc *dev_desc,
			  disk_partition_t *part, u32 offset,
			  u32 size, void *buf)
{
	u32 blk_start, blk_offset, blk_count;

	blk_offset = DIV_ROUND_UP(offset, dev_desc->blksz);
	blk_start = part->start + blk_offset;
	blk_count = DIV_ROUND_UP(size, dev_desc->blksz);

	debug("blk_offset=%d, blk_start=%d,blk_count=%d,out buf=%p\n",
	      blk_offset, blk_start, blk_count, buf);
	if (blk_dread(dev_desc, blk_start, blk_count, buf) != blk_count) {
		printf("read grayscale data failed\n");
		return -EIO;
	}

	return 0;
}

static int image_rearrange(u8 *in_buf, u8 *out_buf, u16 w, u16 h)
{
	int i, j;
	u8 in_data;
	u8 *out_buf_tmp;

	if (!in_buf || !out_buf) {
		printf("rearrange in buffer or out buffer is NULL\n");
		return -EINVAL;
	}

	for (i = 0; i < h; i += 2) {
		out_buf_tmp = out_buf + (i * w / 2);
		for (j = 0; j < w / 2; j++) {
			in_data = *in_buf++;
			*(out_buf_tmp + j * 2) = in_data & 0x0f;
			*(out_buf_tmp + j * 2 + 1) = (in_data >> 4) & 0x0f;
		}
		for (j = 0; j < w / 2; j++) {
			in_data = *in_buf++;
			*(out_buf_tmp + j * 2) |= (in_data << 4) & 0xf0;
			*(out_buf_tmp + j * 2 + 1) |= in_data & 0xf0;
		}
	}

	return 0;
}

static int image_mirror(u8 *in_buf, u8 *out_buf, u16 w, u16 h)
{
	int i;

	if (!in_buf || !out_buf) {
		printf("mirror in buffer or out buffer is NULL\n");
		return -EINVAL;
	}

	for (i = 0; i < h; i++) {
		u16 column_len = w / 2;
		u8 *column_in = in_buf + i * column_len;
		u8 *column_out = out_buf + (h - i - 1) * column_len;

		memcpy(column_out, column_in, column_len);
	}

	return 0;
}

/*
 * The eink kernel driver need last frame to do part refresh,
 * so we need to transfer two images to kernel, which is kernel
 * logo and the logo displayed in uboot.
 *
 * this function use logo type bitmap to indicate several logo.
 * u32 needed_logo: we only load needed logo image into ram, such as
 *                   uboot logo + kernel logo or charger logo + kernel
 *                   logo
 * u32 *loaded_logo: because the needed logo may not exist in logo.img,
 *                  store the really loaded logo in para loaded_logo.
 */
static int read_needed_logo_from_partition(struct udevice *dev,
					   u32 needed_logo,
					   u32 *loaded_logo)
{
	int ret, i;
	disk_partition_t part;
	struct blk_desc *dev_desc;
	struct logo_info *hdr = &eink_logo_info;
	struct logo_part_header *part_hdr = &hdr->part_hdr;
	struct ebc_panel *panel = dev_get_platdata(dev);
	u32 logo = needed_logo & (~(*loaded_logo));

	if (!logo) {
		printf("logo[0x%x] is already loaded, just return!\n",
		       needed_logo);
		return 0;
	}
	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: Could not find device\n", __func__);
		return -EIO;
	}

	if (part_get_info_by_name(dev_desc, PART_LOGO, &part) < 0)
		return -ENODEV;

	ret = read_header(dev_desc, &part, hdr);
	if (ret < 0) {
		printf("eink logo read header failed,ret = %d\n", ret);
		return -EINVAL;
	}
	if (part_hdr->screen_width != panel->vir_width ||
	    part_hdr->screen_height != panel->vir_height){
		printf("logo size(%dx%d) is not same as screen size(%dx%d)\n",
		       part_hdr->screen_width, part_hdr->screen_height,
			panel->vir_width, panel->vir_height);
		return -EINVAL;
	}

	for (i = 0; i < part_hdr->logo_count; i++) {
		struct grayscale_header *img_hdr = &hdr->img_hdr[i];
		int pic_buf;
		u32 offset = img_hdr->data_offset;
		u32 size = img_hdr->data_size;
		u32 logo_type = img_hdr->logo_type;

		debug("offset=0x%x, size=%d,logo_type=%d,w=%d,h=%d\n",
		      offset, size, logo_type, img_hdr->w, img_hdr->h);

		if (logo & logo_type) {
			pic_buf = get_addr_by_type(dev, logo_type);

			if (pic_buf <= 0) {
				printf("Get buffer failed for image %d\n",
				       img_hdr->logo_type);
				return -EIO;
			}
			if (!IS_ALIGNED((ulong)pic_buf,
					ARCH_DMA_MINALIGN)) {
				printf("disp buffer is not dma aligned\n");
				return -EINVAL;
			}
			/*
			 * kernel logo is transmitted to kernel to display, and
			 * kernel will do the mirror operation, so skip kernel
			 * logo here.
			 */
			if (panel->mirror && logo_type != EINK_LOGO_KERNEL) {
				u32 w = panel->vir_width;
				u32 h = panel->vir_height;
				u32 mirror_buf = 0;

				mirror_buf = get_addr_by_type(dev,
							      EINK_LOGO_UNMIRROR_TEMP_BUF);
				if (mirror_buf <= 0) {
					printf("get mirror buffer failed\n");
					return -EIO;
				}
				read_grayscale(dev_desc, &part, offset, size,
					       (void *)((ulong)mirror_buf));
				image_mirror((u8 *)((ulong)mirror_buf),
					     (u8 *)((ulong)pic_buf), w, h);
			} else if (panel->rearrange && logo_type != EINK_LOGO_KERNEL) {
				u32 w = panel->vir_width;
				u32 h = panel->vir_height;
				u32 rearrange_buf = 0;

				rearrange_buf = get_addr_by_type(dev,
							      EINK_LOGO_UNMIRROR_TEMP_BUF);
				if (rearrange_buf <= 0) {
					printf("get mirror buffer failed\n");
					return -EIO;
				}
				read_grayscale(dev_desc, &part, offset, size,
					       (void *)((ulong)rearrange_buf));
				image_rearrange((u8 *)((ulong)rearrange_buf),
					     (u8 *)((ulong)pic_buf), w, h);
			} else {
				read_grayscale(dev_desc, &part, offset, size,
					       (void *)((ulong)pic_buf));
			}
			flush_dcache_range((ulong)pic_buf,
					   ALIGN((ulong)pic_buf + size,
						 CONFIG_SYS_CACHELINE_SIZE));
			*loaded_logo |= logo_type;

			logo &= ~logo_type;
			if (!logo)
				break;
		}
	}

	return 0;
}

static int ebc_power_set(struct udevice *dev, int is_on)
{
	int ret;
	struct rockchip_eink_display_priv *priv = dev_get_priv(dev);
	struct ebc_panel *panel = dev_get_platdata(dev);
	struct udevice *ebc_tcon_dev = priv->ebc_tcon_dev;
	struct rk_ebc_tcon_ops *ebc_tcon_ops = ebc_tcon_get_ops(ebc_tcon_dev);
	struct udevice *ebc_pwr_dev = priv->ebc_pwr_dev;
	struct rk_ebc_pwr_ops *pwr_ops = ebc_pwr_get_ops(ebc_pwr_dev);

	if (is_on) {
		ret = pwr_ops->power_on(ebc_pwr_dev);
		if (ret) {
			printf("%s, power on failed\n", __func__);
			return -1;
		}
		ret = ebc_tcon_ops->enable(ebc_tcon_dev, panel);
		if (ret) {
			printf("%s, ebc tcon enabled failed\n", __func__);
			return -1;
		}
	} else {
		ret = ebc_tcon_ops->disable(ebc_tcon_dev);
		if (ret) {
			printf("%s, ebc tcon disable failed\n", __func__);
			return -1;
		}
		ret = pwr_ops->power_down(ebc_pwr_dev);
		if (ret) {
			printf("%s, power_down failed\n", __func__);
			return -1;
		}
	}
	return 0;
}

static int eink_display(struct udevice *dev, u32 pre_img_buf,
			u32 cur_img_buf, u32 lut_type, int update_mode)
{
	u32 temperature, frame_num;
	struct rockchip_eink_display_priv *priv = dev_get_priv(dev);
	struct ebc_panel *plat = dev_get_platdata(dev);
	struct epd_lut_ops *lut_ops = &plat->lut_ops;
	struct udevice *ebc_pwr_dev = priv->ebc_pwr_dev;
	struct rk_ebc_pwr_ops *pwr_ops = ebc_pwr_get_ops(ebc_pwr_dev);
	struct udevice *ebc_tcon_dev = priv->ebc_tcon_dev;
	struct rk_ebc_tcon_ops *ebc_tcon_ops = ebc_tcon_get_ops(ebc_tcon_dev);

	pwr_ops->temp_get(ebc_pwr_dev, &temperature);
	if (temperature <= 0 || temperature > 50) {
		printf("temperature = %d, out of range0~50 ,use 25\n",
		       temperature);
		temperature = 25;
	}

	if (!lut_ops->lut_get) {
		printf("get lut ops failed\n");
		return -EIO;
	}
	lut_ops->lut_get(&plat->lut_data, lut_type, temperature);
	frame_num = plat->lut_data.frame_num;
	debug("lut_type=%d, frame num=%d, temp=%d\n", lut_type,
	      frame_num, temperature);

	ebc_tcon_ops->wait_for_last_frame_complete(ebc_tcon_dev);
	ebc_tcon_ops->lut_data_set(ebc_tcon_dev, plat->lut_data.data,
				   frame_num, 0);
	ebc_tcon_ops->dsp_mode_set(ebc_tcon_dev, update_mode,
				   LUT_MODE, !THREE_WIN_MODE, !EINK_MODE);
	ebc_tcon_ops->image_addr_set(ebc_tcon_dev, pre_img_buf, cur_img_buf);
	ebc_tcon_ops->frame_start(ebc_tcon_dev, frame_num);
	return 0;
}

static int rk_eink_display_init(void)
{
	int ret;
	struct uclass *uc;
	struct udevice *dev;

	if (eink_dev) {
		printf("ebc-dev is already initialized!\n");
		return 0;
	}

	ret = uclass_get(UCLASS_EINK_DISPLAY, &uc);
	if (ret) {
		printf("can't find uclass eink\n");
		return -ENODEV;
	}
	for (uclass_first_device(UCLASS_EINK_DISPLAY, &dev);
	     dev; uclass_next_device(&dev))
		;

	if (eink_dev) {
		printf("ebc-dev is probed success!\n");
		return 0;
	}
	printf("Can't find ebc-dev\n");
	return -ENODEV;
}

/*
 * Eink display need current and previous image buffer, We assume
 * every type of logo has only one image, so just tell this function
 * last logo type and current logo type, it will find the right images.
 * last_logo_type: -1 means it's first displaying.
 */
static int rockchip_eink_show_logo(int cur_logo_type, int update_mode)
{
	int ret = 0;
	u32 logo_addr;
	u32 last_logo_addr;
	struct ebc_panel *plat;
	struct udevice *dev;
	static u32 loaded_logo = 0;
	struct rockchip_eink_display_priv *priv;

	if (!eink_dev) {
		static bool first_init = true;

		if (first_init) {
			first_init = false;
			ret = rk_eink_display_init();
			if (ret) {
				printf("Get ebc dev failed, check dts\n");
				return -ENODEV;
			}
		} else {
			return -ENODEV;
		}
	}
	dev = eink_dev;

	/*Don't need to update display*/
	if (last_logo_type == cur_logo_type) {
		debug("Same as last picture, Don't need to display\n");
		return 0;
	}

	plat = dev_get_platdata(dev);
	priv = dev_get_priv(dev);

	/*
	 * The last_logo_type is -1 means it's first displaying
	 */
	if (last_logo_type == -1) {
		ret = ebc_power_set(dev, EBC_PWR_ON);
		if (ret) {
			printf("Eink power on failed\n");
			return -1;
		}

		int size = (plat->vir_width * plat->vir_height) >> 1;

		logo_addr = get_addr_by_type(dev, EINK_LOGO_RESET);
		memset((u32 *)(u64)logo_addr, 0xff, size);
		flush_dcache_range((ulong)logo_addr,
				   ALIGN((ulong)logo_addr + size,
					 CONFIG_SYS_CACHELINE_SIZE));
		eink_display(dev, logo_addr, logo_addr,
			     WF_TYPE_RESET, EINK_LOGO_RESET);
		last_logo_type = 0;
		last_logo_addr = logo_addr;
	} else {
		last_logo_addr = get_addr_by_type(dev, last_logo_type);
		if (last_logo_addr < 0) {
			printf("Invalid last logo addr, exit!\n");
			goto out;
		}
	}
	ret = read_needed_logo_from_partition(dev, cur_logo_type,
					      &loaded_logo);
	if (ret || !(loaded_logo & cur_logo_type)) {
		printf("read logo[0x%x] failed, loaded_logo=0x%x\n",
		       cur_logo_type, loaded_logo);
		ret = -EIO;
		goto out;
	}
	logo_addr = get_addr_by_type(dev, cur_logo_type);
	debug("logo_addr=%x, logo_type=%d\n", logo_addr, cur_logo_type);
	if (logo_addr <= 0) {
		printf("get logo buffer failed\n");
		ret = -EIO;
		goto out;
	}

	eink_display(dev, last_logo_addr, logo_addr, WF_TYPE_GC16, update_mode);

	if (priv->backlight)
		backlight_enable(priv->backlight);

	last_logo_type = cur_logo_type;

	if (cur_logo_type == EINK_LOGO_POWEROFF) {
		struct udevice *ebc_tcon_dev = priv->ebc_tcon_dev;
		struct rk_ebc_tcon_ops *ebc_tcon_ops;

		last_logo_type = -1;
		/*
		 * For normal logo display, waiting for the last frame
		 * completion before start a new frame, except one
		 * situation which charging logo display finished,
		 * because device will rebooting or shutdown after
		 * charging logo is competed.
		 *
		 * We should take care of the power sequence,
		 * because ebc can't power off if last frame
		 * data is still sending, so keep the ebc power
		 * during u-boot phase and shutdown the
		 * power only if uboot charging is finished.
		 */
		ebc_tcon_ops = ebc_tcon_get_ops(ebc_tcon_dev);
		ebc_tcon_ops->wait_for_last_frame_complete(ebc_tcon_dev);
		debug("charging logo displaying is complete\n");
		/*
		 *shutdown ebc after charging logo display is complete
		 */
		ret = ebc_power_set(dev, EBC_PWR_DOWN);
		if (ret)
			printf("Eink power down failed\n");
		goto out;
	}

	/*
	 * System will boot up to kernel only when the
	 * logo is uboot logo
	 */
	if (cur_logo_type == EINK_LOGO_UBOOT) {
		char logo_args[64] = {0};
		u32 uboot_logo_buf;

		if (plat->mirror || plat->rearrange)
			uboot_logo_buf = get_addr_by_type(dev,
							  EINK_LOGO_UNMIRROR_TEMP_BUF);
		else
			uboot_logo_buf = logo_addr;
		printf("Transmit uboot logo addr(0x%x) to kernel\n",
		       uboot_logo_buf);
		sprintf(logo_args, "ulogo_addr=0x%x", uboot_logo_buf);
		env_update("bootargs", logo_args);
		ret = read_needed_logo_from_partition(dev, EINK_LOGO_KERNEL,
						      &loaded_logo);
		if (ret || !(loaded_logo & EINK_LOGO_KERNEL)) {
			printf("No invalid kernel logo in logo.img\n");
		} else {
			int klogo_addr = get_addr_by_type(dev,
							  EINK_LOGO_KERNEL);

			if (klogo_addr <= 0) {
				printf("get kernel logo buffer failed\n");
				ret = -EIO;
				goto out;
			}
			printf("Transmit kernel logo addr(0x%x) to kernel\n",
			       klogo_addr);
			sprintf(logo_args, "klogo_addr=0x%x", klogo_addr);
			env_update("bootargs", logo_args);
		}
	}

out:
	return ret;
}

int rockchip_eink_show_uboot_logo(void)
{
	return rockchip_eink_show_logo(EINK_LOGO_UBOOT, EINK_UPDATE_DIFF);
}

int rockchip_eink_show_charge_logo(int logo_type)
{
	return rockchip_eink_show_logo(logo_type, EINK_UPDATE_DIFF);
}

static int rockchip_eink_display_probe(struct udevice *dev)
{
	int ret, vcom, size, i;
	const fdt32_t *list;
	uint32_t phandle;
	struct rockchip_eink_display_priv *priv = dev_get_priv(dev);

	/* Before relocation we don't need to do anything */
	if (!(gd->flags & GD_FLG_RELOC))
		return 0;

	ret = uclass_get_device_by_phandle(UCLASS_EBC, dev,
					   "ebc_tcon",
					   &priv->ebc_tcon_dev);
	if (ret) {
		dev_err(dev, "Cannot get ebc_tcon: %d\n", ret);
		return ret;
	}

	list = dev_read_prop(dev, "pmic", &size);
	if (!list) {
		dev_err(dev, "Cannot get pmic prop\n");
		return -EINVAL;
	}

	size /= sizeof(*list);
	for (i = 0; i < size; i++) {
		phandle = fdt32_to_cpu(*list++);
		ret = uclass_get_device_by_phandle_id(UCLASS_I2C_GENERIC,
						      phandle,
						      &priv->ebc_pwr_dev);
		if (!ret) {
			printf("Eink: use pmic %s\n", priv->ebc_pwr_dev->name);
			break;
		}
	}
	if (ret) {
		dev_err(dev, "Cannot get pmic: %d\n", ret);
		return ret;
	}

	ret = uclass_get_device_by_phandle(UCLASS_PANEL_BACKLIGHT, dev,
					   "backlight", &priv->backlight);
	if (ret && ret != -ENOENT) {
		printf("%s: Cannot get backlight: %d\n", __func__, ret);
	}

	vcom = read_vcom_from_vendor();
	if (vcom <= 0) {
		printf("read vcom from vendor failed, use default vcom\n");
		priv->vcom = VCOM_DEFAULT_VALUE;
	} else {
		priv->vcom = vcom;
	}

	if (priv->ebc_pwr_dev) {
		struct rk_ebc_pwr_ops *pwr_ops;

		pwr_ops = ebc_pwr_get_ops(priv->ebc_pwr_dev);
		ret = pwr_ops->vcom_set(priv->ebc_pwr_dev, priv->vcom);
		if (ret) {
			printf("%s, vcom_set failed\n", __func__);
			return -EIO;
		}
	}

	// read lut to ram, and get lut ops
	ret = read_waveform(dev);
	if (ret < 0) {
		printf("read wavform failed\n");
		return -EIO;
	}

	eink_dev = dev;

	return 0;
}

static int rockchip_eink_display_ofdata_to_platdata(struct udevice *dev)
{
	fdt_size_t size;
	fdt_addr_t tmp_addr;
	struct device_node *disp_mem;
	struct device_node *waveform_mem;
	struct ebc_panel *plat = dev_get_platdata(dev);

	plat->width = dev_read_u32_default(dev, "panel,width", 0);
	plat->height = dev_read_u32_default(dev, "panel,height", 0);
	plat->vir_width = dev_read_u32_default(dev, "panel,vir_width", plat->width);
	plat->vir_height = dev_read_u32_default(dev, "panel,vir_height", plat->height);
	plat->sdck = dev_read_u32_default(dev, "panel,sdck", 0);
	plat->lsl = dev_read_u32_default(dev, "panel,lsl", 0);
	plat->lbl = dev_read_u32_default(dev, "panel,lbl", 0);
	plat->ldl = dev_read_u32_default(dev, "panel,ldl", 0);
	plat->lel = dev_read_u32_default(dev, "panel,lel", 0);
	plat->gdck_sta = dev_read_u32_default(dev, "panel,gdck-sta", 0);
	plat->lgonl = dev_read_u32_default(dev, "panel,lgonl", 0);
	plat->fsl = dev_read_u32_default(dev, "panel,fsl", 0);
	plat->fbl = dev_read_u32_default(dev, "panel,fbl", 0);
	plat->fdl = dev_read_u32_default(dev, "panel,fdl", 0);
	plat->fel = dev_read_u32_default(dev, "panel,fel", 0);
	plat->panel_16bit = dev_read_u32_default(dev, "panel,panel_16bit", 0);
	plat->panel_color = dev_read_u32_default(dev, "panel,panel_color", 0);
	plat->mirror = dev_read_u32_default(dev, "panel,mirror", 0);
	plat->rearrange = dev_read_u32_default(dev, "panel,rearrange", 0);
	plat->width_mm = dev_read_u32_default(dev, "panel,width-mm", 0);
	plat->height_mm = dev_read_u32_default(dev, "panel,height-mm", 0);

	disp_mem = of_parse_phandle(ofnode_to_np(dev_ofnode(dev)),
				    "memory-region", 0);
	if (!disp_mem) {
		dev_err(dev, "Cannot get memory-region from dts\n");
		return -ENODEV;
	}
	tmp_addr = ofnode_get_addr_size(np_to_ofnode(disp_mem), "reg", &size);
	if (tmp_addr == FDT_ADDR_T_NONE) {
		printf("get display memory address failed\n");
		return -ENODEV;
	}

	plat->disp_pbuf = (u64)map_sysmem(tmp_addr, 0);
	plat->disp_pbuf_size = size;
	debug("display mem=0x%x, size=%x\n", plat->disp_pbuf,
	      plat->disp_pbuf_size);
	waveform_mem = of_parse_phandle(ofnode_to_np(dev_ofnode(dev)),
					"waveform-region", 0);
	if (!waveform_mem) {
		printf("Cannot get waveform-region from dts\n");
		return -ENODEV;
	}
	tmp_addr = ofnode_get_addr_size(np_to_ofnode(waveform_mem),
					"reg", &size);
	if (tmp_addr == FDT_ADDR_T_NONE) {
		printf("get waveform memory address failed\n");
		return -ENODEV;
	}

	plat->lut_pbuf = map_sysmem(tmp_addr, 0);
	plat->lut_pbuf_size = size;
	debug("lut mem=0x%p, size=%x\n", plat->lut_pbuf, plat->lut_pbuf_size);
	return 0;
}

static const struct udevice_id rockchip_eink_display_ids[] = {
	{ .compatible = "rockchip,ebc-dev", },
	{}
};

U_BOOT_DRIVER(rk_eink_display) = {
	.name = "rockchip_eink_display",
	.id = UCLASS_EINK_DISPLAY,
	.of_match = rockchip_eink_display_ids,
	.ofdata_to_platdata = rockchip_eink_display_ofdata_to_platdata,
	.probe = rockchip_eink_display_probe,
	.priv_auto_alloc_size = sizeof(struct rockchip_eink_display_priv),
	.platdata_auto_alloc_size = sizeof(struct ebc_panel),
};

UCLASS_DRIVER(rk_eink) = {
	.id	= UCLASS_EINK_DISPLAY,
	.name	= "rk_eink",
};

