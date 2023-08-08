/*
 * (C) Copyright 2008 - 2009
 * Windriver, <www.windriver.com>
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * Copyright 2011 Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <config.h>
#include <common.h>
#include <console.h>
#include <android_bootloader.h>
#include <errno.h>
#include <fastboot.h>
#include <malloc.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/composite.h>
#include <linux/compiler.h>
#include <u-boot/sha256.h>
#include <version.h>
#include <g_dnl.h>
#include <fs.h>
#include <android_avb/avb_ops_user.h>
#include <android_avb/rk_avb_ops_user.h>
#include <dm/uclass.h>
#include <power/fuel_gauge.h>
#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
#include <fb_mmc.h>
#endif
#ifdef CONFIG_FASTBOOT_FLASH_NAND_DEV
#include <fb_nand.h>
#endif
#ifdef CONFIG_OPTEE_CLIENT
#include <optee_include/OpteeClientInterface.h>
#endif
#include <boot_rkimg.h>
#include <optee_include/tee_client_api.h>
#ifdef CONFIG_FASTBOOT_OEM_UNLOCK
#include <keymaster.h>
#endif
#ifdef CONFIG_ANDROID_AB
#include <android_ab.h>
#endif

#define FASTBOOT_VERSION		"0.4"

#define FASTBOOT_INTERFACE_CLASS	0xff
#define FASTBOOT_INTERFACE_SUB_CLASS	0x42
#define FASTBOOT_INTERFACE_PROTOCOL	0x03

#define RX_ENDPOINT_MAXIMUM_PACKET_SIZE_2_0  (0x0200)
#define RX_ENDPOINT_MAXIMUM_PACKET_SIZE_1_1  (0x0040)
#define TX_ENDPOINT_MAXIMUM_PACKET_SIZE      (0x0040)

#define EP_BUFFER_SIZE			4096
#define SLEEP_COUNT 20000
#define MAX_PART_NUM_STR_SIZE 4
#define PARTITION_TYPE_STRINGS "partition-type"

/*
 * EP_BUFFER_SIZE must always be an integral multiple of maxpacket size
 * (64 or 512 or 1024), else we break on certain controllers like DWC3
 * that expect bulk OUT requests to be divisible by maxpacket size.
 */

struct f_fastboot {
	struct usb_function usb_function;

	/* IN/OUT EP's and corresponding requests */
	struct usb_ep *in_ep, *out_ep;
	struct usb_request *in_req, *out_req;
};

static inline struct f_fastboot *func_to_fastboot(struct usb_function *f)
{
	return container_of(f, struct f_fastboot, usb_function);
}

static struct f_fastboot *fastboot_func;
static unsigned int download_size;
static unsigned int download_bytes;
static unsigned int upload_size;
static unsigned int upload_bytes;
static bool start_upload;
static unsigned intthread_wakeup_needed;

static struct usb_endpoint_descriptor fs_ep_in = {
	.bLength            = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType    = USB_DT_ENDPOINT,
	.bEndpointAddress   = USB_DIR_IN,
	.bmAttributes       = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize     = cpu_to_le16(64),
};

static struct usb_endpoint_descriptor fs_ep_out = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(64),
};

static struct usb_endpoint_descriptor hs_ep_in = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(512),
};

static struct usb_endpoint_descriptor hs_ep_out = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(512),
};

static struct usb_endpoint_descriptor ss_ep_in = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_IN,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor ss_ep_in_comp_desc = {
	.bLength		= sizeof(ss_ep_in_comp_desc),
	.bDescriptorType	= USB_DT_SS_ENDPOINT_COMP,
	/* .bMaxBurst		= DYNAMIC, */
};

static struct usb_endpoint_descriptor ss_ep_out = {
	.bLength		= USB_DT_ENDPOINT_SIZE,
	.bDescriptorType	= USB_DT_ENDPOINT,
	.bEndpointAddress	= USB_DIR_OUT,
	.bmAttributes		= USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize		= cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor ss_ep_out_comp_desc = {
	.bLength		= sizeof(ss_ep_out_comp_desc),
	.bDescriptorType	= USB_DT_SS_ENDPOINT_COMP,
	/* .bMaxBurst		= DYNAMIC, */
};

static struct usb_interface_descriptor interface_desc = {
	.bLength		= USB_DT_INTERFACE_SIZE,
	.bDescriptorType	= USB_DT_INTERFACE,
	.bInterfaceNumber	= 0x00,
	.bAlternateSetting	= 0x00,
	.bNumEndpoints		= 0x02,
	.bInterfaceClass	= FASTBOOT_INTERFACE_CLASS,
	.bInterfaceSubClass	= FASTBOOT_INTERFACE_SUB_CLASS,
	.bInterfaceProtocol	= FASTBOOT_INTERFACE_PROTOCOL,
};

static struct usb_descriptor_header *fb_fs_function[] = {
	(struct usb_descriptor_header *)&interface_desc,
	(struct usb_descriptor_header *)&fs_ep_in,
	(struct usb_descriptor_header *)&fs_ep_out,
};

static struct usb_descriptor_header *fb_hs_function[] = {
	(struct usb_descriptor_header *)&interface_desc,
	(struct usb_descriptor_header *)&hs_ep_in,
	(struct usb_descriptor_header *)&hs_ep_out,
	NULL,
};

static struct usb_descriptor_header *fb_ss_function[] = {
	(struct usb_descriptor_header *)&interface_desc,
	(struct usb_descriptor_header *)&ss_ep_in,
	(struct usb_descriptor_header *)&ss_ep_in_comp_desc,
	(struct usb_descriptor_header *)&ss_ep_out,
	(struct usb_descriptor_header *)&ss_ep_out_comp_desc,
	NULL,
};

static struct usb_endpoint_descriptor *
fb_ep_desc(struct usb_gadget *g, struct usb_endpoint_descriptor *fs,
	   struct usb_endpoint_descriptor *hs,
	   struct usb_endpoint_descriptor *ss,
	   struct usb_ss_ep_comp_descriptor *comp_desc,
	   struct usb_ep *ep)
{
	struct usb_endpoint_descriptor *speed_desc = NULL;

	/* select desired speed */
	switch (g->speed) {
	case USB_SPEED_SUPER:
		if (gadget_is_superspeed(g)) {
			speed_desc = ss;
			ep->comp_desc = comp_desc;
			break;
		}
		/* else: Fall trough */
	case USB_SPEED_HIGH:
		if (gadget_is_dualspeed(g)) {
			speed_desc = hs;
			break;
		}
		/* else: fall through */
	default:
		speed_desc = fs;
	}

	return speed_desc;
}

/*
 * static strings, in UTF-8
 */
static const char fastboot_name[] = "Android Fastboot";

static struct usb_string fastboot_string_defs[] = {
	[0].s = fastboot_name,
	{  }			/* end of list */
};

static struct usb_gadget_strings stringtab_fastboot = {
	.language	= 0x0409,	/* en-us */
	.strings	= fastboot_string_defs,
};

static struct usb_gadget_strings *fastboot_strings[] = {
	&stringtab_fastboot,
	NULL,
};

static void rx_handler_command(struct usb_ep *ep, struct usb_request *req);
static int strcmp_l1(const char *s1, const char *s2);
static void wakeup_thread(void)
{
	intthread_wakeup_needed = false;
}

static void busy_indicator(void)
{
	static int state;

	switch (state) {
	case 0:
		puts("\r|"); break;
	case 1:
		puts("\r/"); break;
	case 2:
		puts("\r-"); break;
	case 3:
		puts("\r\\"); break;
	case 4:
		puts("\r|"); break;
	case 5:
		puts("\r/"); break;
	case 6:
		puts("\r-"); break;
	case 7:
		puts("\r\\"); break;
	default:
		state = 0;
	}
	if (state++ == 8)
		state = 0;
}

static int fb_get_fstype(const char *ifname, const int part_num,
			 const char **fs_type)
{
	char part_num_str[MAX_PART_NUM_STR_SIZE] = {0};

	snprintf(part_num_str, ARRAY_SIZE(part_num_str), ":%x", part_num);

	if (fs_set_blk_dev(ifname, part_num_str, FS_TYPE_ANY))
		return -1;

	if (fs_get_fstype(fs_type))
		return -1;

	return 0;
}

static int sleep_thread(void)
{
	int rc = 0;
	int i = 0, k = 0;

	/* Wait until a signal arrives or we are woken up */
	for (;;) {
		if (!intthread_wakeup_needed)
			break;

		if (++i == SLEEP_COUNT) {
			busy_indicator();
			i = 0;
			k++;
		}

		if (k == 10) {
			/* Handle CTRL+C */
			if (ctrlc())
				return -EPIPE;

			/* Check cable connection */
			if (!g_dnl_board_usb_cable_connected())
				return -EIO;

			k = 0;
		}

		usb_gadget_handle_interrupts(0);
	}
	intthread_wakeup_needed = true;
	return rc;
}

static void fastboot_complete(struct usb_ep *ep, struct usb_request *req)
{
	int status = req->status;

	wakeup_thread();
	if (!status)
		return;
	printf("status: %d ep '%s' trans: %d\n", status, ep->name, req->actual);
}

static int fastboot_bind(struct usb_configuration *c, struct usb_function *f)
{
	int id;
	struct usb_gadget *gadget = c->cdev->gadget;
	struct f_fastboot *f_fb = func_to_fastboot(f);
	const char *s;

	/* DYNAMIC interface numbers assignments */
	id = usb_interface_id(c, f);
	if (id < 0)
		return id;
	interface_desc.bInterfaceNumber = id;

	id = usb_string_id(c->cdev);
	if (id < 0)
		return id;
	fastboot_string_defs[0].id = id;
	interface_desc.iInterface = id;

	f_fb->in_ep = usb_ep_autoconfig(gadget, &fs_ep_in);
	if (!f_fb->in_ep)
		return -ENODEV;
	f_fb->in_ep->driver_data = c->cdev;

	f_fb->out_ep = usb_ep_autoconfig(gadget, &fs_ep_out);
	if (!f_fb->out_ep)
		return -ENODEV;
	f_fb->out_ep->driver_data = c->cdev;

	f->descriptors = fb_fs_function;

	if (gadget_is_dualspeed(gadget)) {
		/* Assume endpoint addresses are the same for both speeds */
		hs_ep_in.bEndpointAddress = fs_ep_in.bEndpointAddress;
		hs_ep_out.bEndpointAddress = fs_ep_out.bEndpointAddress;
		/* copy HS descriptors */
		f->hs_descriptors = fb_hs_function;
	}

	if (gadget_is_superspeed(gadget)) {
		/* Assume endpoint addresses are the same as full speed */
		ss_ep_in.bEndpointAddress = fs_ep_in.bEndpointAddress;
		ss_ep_out.bEndpointAddress = fs_ep_out.bEndpointAddress;
		/* copy SS descriptors */
		f->ss_descriptors = fb_ss_function;
	}

	s = env_get("serial#");
	if (s)
		g_dnl_set_serialnumber((char *)s);

	return 0;
}

static void fastboot_unbind(struct usb_configuration *c, struct usb_function *f)
{
	memset(fastboot_func, 0, sizeof(*fastboot_func));
}

static void fastboot_disable(struct usb_function *f)
{
	struct f_fastboot *f_fb = func_to_fastboot(f);

	usb_ep_disable(f_fb->out_ep);
	usb_ep_disable(f_fb->in_ep);

	if (f_fb->out_req) {
		free(f_fb->out_req->buf);
		usb_ep_free_request(f_fb->out_ep, f_fb->out_req);
		f_fb->out_req = NULL;
	}
	if (f_fb->in_req) {
		free(f_fb->in_req->buf);
		usb_ep_free_request(f_fb->in_ep, f_fb->in_req);
		f_fb->in_req = NULL;
	}
}

static struct usb_request *fastboot_start_ep(struct usb_ep *ep)
{
	struct usb_request *req;

	req = usb_ep_alloc_request(ep, 0);
	if (!req)
		return NULL;

	req->length = EP_BUFFER_SIZE;
	req->buf = memalign(CONFIG_SYS_CACHELINE_SIZE, EP_BUFFER_SIZE);
	if (!req->buf) {
		usb_ep_free_request(ep, req);
		return NULL;
	}

	memset(req->buf, 0, req->length);
	return req;
}

static int fastboot_set_alt(struct usb_function *f,
			    unsigned interface, unsigned alt)
{
	int ret;
	struct usb_composite_dev *cdev = f->config->cdev;
	struct usb_gadget *gadget = cdev->gadget;
	struct f_fastboot *f_fb = func_to_fastboot(f);
	const struct usb_endpoint_descriptor *d;

	debug("%s: func: %s intf: %d alt: %d\n",
	      __func__, f->name, interface, alt);

	d = fb_ep_desc(gadget, &fs_ep_out, &hs_ep_out, &ss_ep_out,
		       &ss_ep_out_comp_desc, f_fb->out_ep);
	ret = usb_ep_enable(f_fb->out_ep, d);
	if (ret) {
		puts("failed to enable out ep\n");
		return ret;
	}

	f_fb->out_req = fastboot_start_ep(f_fb->out_ep);
	if (!f_fb->out_req) {
		puts("failed to alloc out req\n");
		ret = -EINVAL;
		goto err;
	}
	f_fb->out_req->complete = rx_handler_command;

	d = fb_ep_desc(gadget, &fs_ep_in, &hs_ep_in, &ss_ep_in,
		       &ss_ep_in_comp_desc, f_fb->in_ep);
	ret = usb_ep_enable(f_fb->in_ep, d);
	if (ret) {
		puts("failed to enable in ep\n");
		goto err;
	}

	f_fb->in_req = fastboot_start_ep(f_fb->in_ep);
	if (!f_fb->in_req) {
		puts("failed alloc req in\n");
		ret = -EINVAL;
		goto err;
	}
	f_fb->in_req->complete = fastboot_complete;

	ret = usb_ep_queue(f_fb->out_ep, f_fb->out_req, 0);
	if (ret)
		goto err;

	return 0;
err:
	fastboot_disable(f);
	return ret;
}

static int fastboot_add(struct usb_configuration *c)
{
	struct f_fastboot *f_fb = fastboot_func;
	int status;

	debug("%s: cdev: 0x%p\n", __func__, c->cdev);

	if (!f_fb) {
		f_fb = memalign(CONFIG_SYS_CACHELINE_SIZE, sizeof(*f_fb));
		if (!f_fb)
			return -ENOMEM;

		fastboot_func = f_fb;
		memset(f_fb, 0, sizeof(*f_fb));
	}

	f_fb->usb_function.name = "f_fastboot";
	f_fb->usb_function.bind = fastboot_bind;
	f_fb->usb_function.unbind = fastboot_unbind;
	f_fb->usb_function.set_alt = fastboot_set_alt;
	f_fb->usb_function.disable = fastboot_disable;
	f_fb->usb_function.strings = fastboot_strings;

	status = usb_add_function(c, &f_fb->usb_function);
	if (status) {
		free(f_fb);
		fastboot_func = f_fb;
	}

	return status;
}
DECLARE_GADGET_BIND_CALLBACK(usb_dnl_fastboot, fastboot_add);

static int fastboot_tx_write(const char *buffer, unsigned int buffer_size)
{
	struct usb_request *in_req = fastboot_func->in_req;
	int ret;

	memcpy(in_req->buf, buffer, buffer_size);
	in_req->length = buffer_size;

	usb_ep_dequeue(fastboot_func->in_ep, in_req);

	ret = usb_ep_queue(fastboot_func->in_ep, in_req, 0);
	if (ret)
		printf("Error %d on queue\n", ret);
	return 0;
}

static int fastboot_tx_write_str(const char *buffer)
{
	int ret;

	ret = sleep_thread();
	if (ret < 0)
		printf("warning: 0x%x, usb transmission is abnormal!\n", ret);

	return fastboot_tx_write(buffer, strlen(buffer));
}

static void compl_do_reset(struct usb_ep *ep, struct usb_request *req)
{
	do_reset(NULL, 0, 0, NULL);
}

int __weak fb_set_reboot_flag(void)
{
	return -ENOSYS;
}

static void cb_reboot(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;

	if (!strcmp_l1("reboot-bootloader", cmd)) {
		if (fb_set_reboot_flag()) {
			fastboot_tx_write_str("FAILCannot set reboot flag");
			return;
		}
	}
#ifdef CONFIG_ANDROID_BOOTLOADER
	if (!strcmp_l1("reboot-fastboot", cmd)) {
		if (android_bcb_write("boot-fastboot")) {
			fastboot_tx_write_str("FAILCannot set boot-fastboot");
			return;
		}
	}

	if (!strcmp_l1("reboot-recovery", cmd)) {
		if (android_bcb_write("boot-recovery")) {
			fastboot_tx_write_str("FAILCannot set boot-recovery");
			return;
		}
	}
#endif
	fastboot_func->in_req->complete = compl_do_reset;
	fastboot_tx_write_str("OKAY");
}

static int strcmp_l1(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return -1;
	return strncmp(s1, s2, strlen(s1));
}

struct name_string {
	const char *str;
	int expects_args;
	char delim;
};

#define NAME_NO_ARGS(s)	{.str = s, .expects_args = 0}
#define NAME_ARGS(s, d)	{.str = s, .expects_args = 1, .delim = d}

static size_t name_check_match(const char *str, size_t len,
			       const struct name_string *name)
{
	size_t str_len = strlen(name->str);

	/* If name len is greater than input, return 0. */
	if (str_len > len)
		return 0;

	/* If name str does not match input string, return 0. */
	if (memcmp(name->str, str, str_len))
		return 0;

	if (name->expects_args) {
		/* string should have space for delim */
		if (len == str_len)
			return 0;

		/* Check delim match */
		if (name->delim != str[str_len])
			return 0;
	} else {
		/* Name str len should match input len */
		if (str_len != len)
			return 0;
	}

	return str_len + name->expects_args;
}

static void fb_add_string(char *dst, size_t chars_left,
			  const char *str, const char *args)
{
	if (!str)
		return;

	int ret = snprintf(dst, chars_left, str, args);

	if (ret < 0)
		pr_err("snprintf is error!");
}

static void fb_add_number(char *dst, size_t chars_left,
			  const char *format, size_t num)
{
	if (!format)
		return;

	int ret = snprintf(dst, chars_left, format, num);

	if (ret > chars_left)
		pr_err("snprintf is error!");
}

static int fb_read_var(char *cmd, char *response,
		       fb_getvar_t var, size_t chars_left)
{
	const char *s;
	int ret = 0;

	switch (var) {
	case FB_VERSION:
		fb_add_string(response, chars_left, FASTBOOT_VERSION, NULL);
		break;
	case FB_BOOTLOADER_VERSION:
		fb_add_string(response, chars_left, U_BOOT_VERSION, NULL);
		break;
	case FB_BASEBAND_VERSION:
		fb_add_string(response, chars_left, "N/A", NULL);
		break;
	case FB_PRODUCT:
		fb_add_string(response, chars_left, CONFIG_SYS_BOARD, NULL);
		break;
	case FB_SERIAL_NO:
		s = env_get("serial#");
		if (s)
			fb_add_string(response, chars_left, s, NULL);
		else
			ret = -1;
		break;
	case FB_SECURE:
		fb_add_string(response, chars_left, "yes", NULL);
		break;
	case FB_VARIANT:
		fb_add_string(response, chars_left, "userdebug", NULL);
		break;
	case FB_DWNLD_SIZE:
		fb_add_number(response, chars_left, "0x%08x",
			      CONFIG_FASTBOOT_BUF_SIZE);
		break;
	case FB_PART_SIZE:
	case FB_PART_TYPE: {
		char *part_name = cmd;

		cmd = strsep(&part_name, ":");
		if (!cmd || !part_name) {
			fb_add_string(response, chars_left,
				      "argument Invalid!", NULL);
			ret = -1;
			break;
		}

#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
		disk_partition_t part_info;
		struct blk_desc *dev_desc;
		int part_num = -1;
		const char *fs_type = NULL;

#ifdef CONFIG_RKIMG_BOOTLOADER
		dev_desc = rockchip_get_bootdev();
#else
		dev_desc = NULL;
#endif
		if (!dev_desc) {
			fb_add_string(response, chars_left,
				      "block device not found", NULL);
			ret = -1;
			break;
		}

		part_num = part_get_info_by_name(dev_desc, part_name,
						 &part_info);
		if (part_num < 0) {
			fb_add_string(response, chars_left,
				      "partition not found", NULL);
			ret = -1;
		} else if (!strncmp(PARTITION_TYPE_STRINGS, cmd,
					strlen(PARTITION_TYPE_STRINGS))) {
			if (fb_get_fstype("mmc", part_num, &fs_type)) {
				fb_add_string(response, chars_left,
					      (char *)part_info.type, NULL);
			} else {
				fb_add_string(response, chars_left,
					      fs_type, NULL);
			}
		} else if (!strncmp("partition-size", cmd, 14)) {
			u64 part_size;

			part_size = (uint64_t)part_info.size;
			snprintf(response, chars_left, "0x%llx",
				 part_size * dev_desc->blksz);
		}
#else
		fb_add_string(response, chars_left, "not implemented", NULL);
		ret = -1;
#endif
		break;
	}
	case FB_BLK_SIZE: {
#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
		struct blk_desc *dev_desc;

#ifdef CONFIG_RKIMG_BOOTLOADER
		dev_desc = rockchip_get_bootdev();
#else
		dev_desc = NULL;
#endif
		if (!dev_desc) {
			fb_add_string(response, chars_left,
				      "block device not found", NULL);
			ret = -1;
		} else {
			fb_add_number(response, chars_left,
				      "0x%lx", dev_desc->blksz);
		}
#else
		fb_add_string(response, chars_left, "not implemented", NULL);
		ret = -1;
#endif
		break;
	}
	case FB_ERASE_SIZE: {
#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
		lbaint_t erase_grp_size;

		erase_grp_size = fb_mmc_get_erase_grp_size();
		if (erase_grp_size < 0) {
			fb_add_string(response, chars_left,
				      "block device not found", NULL);
			ret = -1;
		} else {
			fb_add_number(response, chars_left, "0x"LBAF"",
				      erase_grp_size);
		}
#else
		fb_add_string(response, chars_left, "not implemented", NULL);
		ret = -1;
#endif
		break;
	}
	case FB_UNLOCKED: {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		uint8_t flash_lock_state = 0;

		if (rk_avb_read_flash_lock_state(&flash_lock_state))
			fb_add_string(response, chars_left, "yes", NULL);
		else
			fb_add_string(response, chars_left, "no", NULL);
#else
		fb_add_string(response, chars_left, "not implemented", NULL);
		ret = -1;
#endif
		break;
	}
	case  FB_OFF_MODE_CHARGE: {
		fb_add_string(response, chars_left, "not implemented", NULL);
		break;
	}
	case FB_BATT_VOLTAGE: {
		fb_add_string(response, chars_left, "not implemented", NULL);
		break;
	}
	case FB_BATT_SOC_OK: {
		fb_add_string(response, chars_left, "no", NULL);
		break;
	}
	case FB_IS_USERSPACE: {
		fb_add_string(response, chars_left, "no", NULL);
		break;
	}
#ifdef CONFIG_RK_AVB_LIBAVB_USER
	case FB_HAS_COUNT: {
		char slot_count[2];
		char temp;

		slot_count[1] = '\0';
		if (rk_avb_read_slot_count(&temp) < 0) {
			fb_add_number(response, chars_left, "%d", 0);
			ret = -1;
			break;
		}
		slot_count[0] = temp + 0x30;
		fb_add_string(response, chars_left, slot_count, NULL);
		break;
	}
	case FB_HAS_SLOT: {
		char *part_name = cmd;
		int has_slot = -1;

		cmd = strsep(&part_name, ":");
		if (!cmd || !part_name) {
			fb_add_string(response, chars_left,
				      "argument Invalid!", NULL);
			ret = -1;
			break;
		}

		has_slot = rk_avb_get_part_has_slot_info(part_name);
		if (has_slot < 0)
			fb_add_string(response, chars_left, "no", NULL);
		else
			fb_add_string(response, chars_left, "yes", NULL);
		break;
	}
	case FB_CURR_SLOT: {
		char slot_surrent[8] = {0};

		if (!rk_avb_get_current_slot(slot_surrent)) {
			fb_add_string(response, chars_left,
				      slot_surrent + 1, NULL);
		} else {
			fb_add_string(response, chars_left, "get error", NULL);
			ret = -1;
		}
		break;
	}
	case FB_SLOT_SUFFIXES: {
		char slot_suffixes_temp[4] = {0};
		char slot_suffixes[9] = {0};
		int slot_cnt = 0;

		rk_avb_read_slot_suffixes(slot_suffixes_temp);
		while (slot_suffixes_temp[slot_cnt] != '\0') {
			slot_suffixes[slot_cnt * 2]
				= slot_suffixes_temp[slot_cnt];
			slot_suffixes[slot_cnt * 2 + 1] = ',';
			slot_cnt++;
		}

		slot_suffixes[(slot_cnt - 1) * 2 + 1] = '\0';
		fb_add_string(response, chars_left, slot_suffixes, NULL);
		break;
	}
	case FB_SLOT_SUCCESSFUL:{
		char *slot_name = cmd;
		AvbABData ab_info;

		cmd = strsep(&slot_name, ":");
		if (!cmd || !slot_name) {
			fb_add_string(response, chars_left,
				      "argument Invalid!", NULL);
			ret = -1;
			break;
		}

		if (rk_avb_get_ab_info(&ab_info) < 0) {
			fb_add_string(response, chars_left,
				      "get ab info failed!", NULL);
			ret = -1;
			break;
		}

		if (!strcmp(slot_name, "a")) {
			if (ab_info.slots[0].successful_boot)
				fb_add_string(response, chars_left,
					      "yes", NULL);
			else
				fb_add_string(response, chars_left,
					      "no", NULL);
		} else if (!strcmp(slot_name, "b")) {
			if (ab_info.slots[1].successful_boot)
				fb_add_string(response, chars_left,
					      "yes", NULL);
			else
				fb_add_string(response, chars_left,
					      "no", NULL);
		} else {
			fb_add_string(response, chars_left, "no", NULL);
		}
		break;
	}
	case FB_SLOT_UNBOOTABLE: {
		char *slot_name = cmd;
		AvbABData ab_info;

		cmd = strsep(&slot_name, ":");

		if (!cmd || !slot_name) {
			fb_add_string(response, chars_left,
				      "argument Invalid!", NULL);
			ret = -1;
			break;
		}

		if (rk_avb_get_ab_info(&ab_info) < 0) {
			fb_add_string(response, chars_left,
				      "get ab info failed!", NULL);
			ret = -1;
			break;
		}

		if (!strcmp(slot_name, "a")) {
			if (!ab_info.slots[0].successful_boot &&
			    !ab_info.slots[0].tries_remaining &&
			    !ab_info.slots[0].priority)
				fb_add_string(response, chars_left,
					      "yes", NULL);
			else
				fb_add_string(response, chars_left, "no", NULL);
		} else if (!strcmp(slot_name, "b")) {
			if (!ab_info.slots[1].successful_boot &&
			    !ab_info.slots[1].tries_remaining &&
			    !ab_info.slots[1].priority)
				fb_add_string(response, chars_left,
					      "yes", NULL);
			else
				fb_add_string(response, chars_left, "no", NULL);
		} else {
			fb_add_string(response, chars_left, "no", NULL);
		}
		break;
	}
	case FB_SLOT_RETRY_COUNT: {
		char *slot_name = cmd;
		AvbABData ab_info;

		cmd = strsep(&slot_name, ":");
		if (!cmd || !slot_name) {
			fb_add_string(response, chars_left,
				      "argument Invalid!", NULL);
			ret = -1;
			break;
		}

		if (rk_avb_get_ab_info(&ab_info) < 0) {
			fb_add_string(response, chars_left,
				      "get ab info failed!", NULL);
			ret = -1;
			break;
		}

		if (!strcmp(slot_name, "a")) {
			fb_add_number(response, chars_left,
				      "%d", ab_info.slots[0].tries_remaining);
		} else if (!strcmp(slot_name, "b")) {
			fb_add_number(response, chars_left, "%d",
				      ab_info.slots[1].tries_remaining);

		} else {
			strcpy(response, "FAILno");
		}
		break;
	}
	case FB_AT_VBST: {
		char vbst[VBOOT_STATE_SIZE] = {0};
		char *p_vbst;

		strcpy(response, "INFO");
		rk_avb_get_at_vboot_state(vbst);
		p_vbst = vbst;
		do {
			cmd = strsep(&p_vbst, "\n");
			if (strlen(cmd) > 0) {
				memcpy(&response[4], cmd, chars_left);
				fastboot_tx_write_str(response);
			}
		} while (strlen(cmd));
		break;
	}
	case FB_SNAPSHOT_STATUS: {
#ifdef CONFIG_ANDROID_AB
		struct misc_virtual_ab_message state;

		memset(&state, 0x0, sizeof(state));
		if (read_misc_virtual_ab_message(&state) != 0) {
			printf("FB_SNAPSHOT_STATUS read_misc_virtual_ab_message failed!\n");
			fb_add_string(response, chars_left, "get error", NULL);
			ret = -1;
		}

		if (state.magic != MISC_VIRTUAL_AB_MAGIC_HEADER) {
			printf("FB_SNAPSHOT_STATUS not virtual A/B metadata!\n");
			fb_add_string(response, chars_left, "get error", NULL);
			ret = -1;
		}

		if (state.merge_status == ENUM_MERGE_STATUS_MERGING) {
			fb_add_string(response, chars_left, "merging", NULL);
		} else if (state.merge_status == ENUM_MERGE_STATUS_SNAPSHOTTED) {
			fb_add_string(response, chars_left, "snapshotted", NULL);
		} else {
			fb_add_string(response, chars_left, "none", NULL);
		}
#else
		fb_add_string(response, chars_left, "get error", NULL);
		ret = -1;
#endif
		break;
	}

#endif
#ifdef CONFIG_OPTEE_CLIENT
	case FB_AT_DH: {
		char dhbuf[ATTEST_DH_SIZE];
		uint32_t dh_len = ATTEST_DH_SIZE;
		uint32_t res = trusty_attest_dh((uint8_t *)dhbuf, &dh_len);

		if (res) {
			fb_add_string(response, chars_left, "dh not set", NULL);
			ret = -1;
		} else {
			fb_add_string(response, chars_left, dhbuf, NULL);
		}
		break;
	}
	case FB_AT_UUID: {
		char uuid[ATTEST_UUID_SIZE] = {0};
		uint32_t uuid_len = ATTEST_UUID_SIZE;
		uint32_t res = trusty_attest_uuid((uint8_t *)uuid, &uuid_len);

		uuid[ATTEST_UUID_SIZE - 1] = 0;
		if (res) {
			fb_add_string(response, chars_left, "dh not set", NULL);
			ret = -1;
		} else {
			fb_add_string(response, chars_left, uuid, NULL);
		}
		break;
	}
#endif
	default: {
			char *envstr;

			envstr = malloc(strlen("fastboot.") + strlen(cmd) + 1);
			if (!envstr) {
				fb_add_string(response, chars_left,
					      "malloc error", NULL);
				ret = -1;
				break;
			}

			sprintf(envstr, "fastboot.%s", cmd);
			s = env_get(envstr);
			if (s) {
				strncat(response, s, chars_left);
			} else {
				printf("WARNING: unknown variable: %s\n", cmd);
				fb_add_string(response, chars_left,
					      "not implemented", NULL);
			}

			free(envstr);
			break;
		}
	}

	return ret;
}

static const struct {
	/*
	 *any changes to this array require an update to the corresponding
	 *enum in fastboot.h
	 */
	struct name_string name;
	fb_getvar_t var;
} getvar_table[] = {
	{ NAME_NO_ARGS("version"), FB_VERSION},
	{ NAME_NO_ARGS("version-bootloader"), FB_BOOTLOADER_VERSION},
	{ NAME_NO_ARGS("version-baseband"), FB_BASEBAND_VERSION},
	{ NAME_NO_ARGS("product"), FB_PRODUCT},
	{ NAME_NO_ARGS("serialno"), FB_SERIAL_NO},
	{ NAME_NO_ARGS("secure"), FB_SECURE},
	{ NAME_NO_ARGS("max-download-size"), FB_DWNLD_SIZE},
	{ NAME_NO_ARGS("logical-block-size"), FB_BLK_SIZE},
	{ NAME_NO_ARGS("erase-block-size"), FB_ERASE_SIZE},
	{ NAME_ARGS("partition-type", ':'), FB_PART_TYPE},
	{ NAME_ARGS("partition-size", ':'), FB_PART_SIZE},
	{ NAME_NO_ARGS("unlocked"), FB_UNLOCKED},
	{ NAME_NO_ARGS("off-mode-charge"), FB_OFF_MODE_CHARGE},
	{ NAME_NO_ARGS("battery-voltage"), FB_BATT_VOLTAGE},
	{ NAME_NO_ARGS("variant"), FB_VARIANT},
	{ NAME_NO_ARGS("battery-soc-ok"), FB_BATT_SOC_OK},
	{ NAME_NO_ARGS("is-userspace"), FB_IS_USERSPACE},
#ifdef CONFIG_RK_AVB_LIBAVB_USER
	/* Slots related */
	{ NAME_NO_ARGS("slot-count"), FB_HAS_COUNT},
	{ NAME_ARGS("has-slot", ':'), FB_HAS_SLOT},
	{ NAME_NO_ARGS("current-slot"), FB_CURR_SLOT},
	{ NAME_NO_ARGS("slot-suffixes"), FB_SLOT_SUFFIXES},
	{ NAME_ARGS("slot-successful", ':'), FB_SLOT_SUCCESSFUL},
	{ NAME_ARGS("slot-unbootable", ':'), FB_SLOT_UNBOOTABLE},
	{ NAME_ARGS("slot-retry-count", ':'), FB_SLOT_RETRY_COUNT},
	{ NAME_NO_ARGS("at-vboot-state"), FB_AT_VBST},
	{ NAME_NO_ARGS("snapshot-update-status"), FB_SNAPSHOT_STATUS},
#endif
	/*
	 * OEM specific :
	 * Spec says names starting with lowercase letter are reserved.
	 */
#ifdef CONFIG_OPTEE_CLIENT
	{ NAME_NO_ARGS("at-attest-dh"), FB_AT_DH},
	{ NAME_NO_ARGS("at-attest-uuid"), FB_AT_UUID},
#endif
};

static int fb_getvar_single(char *cmd, char *response, size_t chars_left)
{
	int i;
	size_t match_len = 0;
	size_t len = strlen(cmd);

	for (i = 0; i < ARRAY_SIZE(getvar_table); i++) {
		match_len = name_check_match(cmd, len, &getvar_table[i].name);
		if (match_len)
			break;
	}

	if (match_len == 0) {
		fb_add_string(response, chars_left, "unknown variable", NULL);
		return -1;
	}

	if (fb_read_var(cmd, response, getvar_table[i].var, chars_left) < 0)
		return -1;

	return 0;
}

static void fb_getvar_all(void)
{
	char response[FASTBOOT_RESPONSE_LEN] = {0};
	char resp_tmp[FASTBOOT_RESPONSE_LEN] = {0};
	char *actual_resp;
	size_t chars_left;
	int i, p;
	disk_partition_t part_info;
	struct blk_desc *dev_desc;

	strcpy(response, "INFO");
	chars_left = sizeof(response) - strlen(response) - 1;
	actual_resp = response + strlen(response);

	for (i = 0; i < ARRAY_SIZE(getvar_table); i++) {
		fb_getvar_t var = getvar_table[i].var;

		switch (var) {
		case FB_PART_TYPE:
		case FB_PART_SIZE: {
			const char *fs_type = NULL;
#ifdef CONFIG_RKIMG_BOOTLOADER
			dev_desc = rockchip_get_bootdev();
#else
			dev_desc = NULL;
#endif
			if (!dev_desc) {
				fb_add_string(actual_resp, chars_left,
					      "%s:block device not found",
					      getvar_table[i].name.str);
				fastboot_tx_write_str(response);
				break;
			}

			for (p = 1; p <= MAX_SEARCH_PARTITIONS; p++) {
				if (part_get_info(dev_desc, p,
						  &part_info) < 0) {
					break;
				}

				if (var == FB_PART_TYPE) {
					fs_type = NULL;
					if (fb_get_fstype("mmc", p,
							  &fs_type)) {
						fb_add_string(
							resp_tmp,
							FASTBOOT_RESPONSE_LEN,
							(char *)part_info.type,
							NULL);
					} else {
						fb_add_string(
							resp_tmp,
							FASTBOOT_RESPONSE_LEN,
							fs_type,
							NULL);
					}

					snprintf(actual_resp,
						 chars_left,
						 "%s:%s:%s",
						 getvar_table[i].name.str,
						 part_info.name,
						 resp_tmp);
				} else {
					uint64_t part_size;

					part_size = (uint64_t)part_info.size;
					snprintf(actual_resp,
						 chars_left,
						 "%s:%s:0x%llx",
						 getvar_table[i].name.str,
						 part_info.name,
						 part_size * dev_desc->blksz);
				}
				fastboot_tx_write_str(response);
			}
			break;
		}
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		case FB_HAS_SLOT: {
			uchar *ptr_name_tmp;
			char c = '_';
			int has_slot = -1;

#ifdef CONFIG_RKIMG_BOOTLOADER
			dev_desc = rockchip_get_bootdev();
#else
			dev_desc = NULL;
#endif
			if (!dev_desc) {
				fb_add_string(actual_resp, chars_left,
					      "%s:block device not found",
					      getvar_table[i].name.str);
				fastboot_tx_write_str(response);
				break;
			}

			for (p = 1; p <= MAX_SEARCH_PARTITIONS; p++) {
				if (part_get_info(dev_desc, p,
						  &part_info) < 0) {
					break;
				} else {
					ptr_name_tmp = (uchar *)strrchr(
						(char *)part_info.name, c);
					if (ptr_name_tmp &&
					    part_info.name[ptr_name_tmp -
						part_info.name + 2] == '\0')
						fb_add_string(
							resp_tmp,
							ptr_name_tmp -
							part_info.name + 1,
							(char *)part_info.name,
							NULL);
					else
						strcpy(resp_tmp,
						       (char *)part_info.name);

					has_slot = rk_avb_get_part_has_slot_info(
						resp_tmp);
					if (has_slot < 0) {
						snprintf(actual_resp,
							 chars_left,
							 "%s:%s:no",
							 getvar_table[i].name.str,
							 resp_tmp);
					} else {
						snprintf(actual_resp,
							 chars_left,
							 "%s:%s:yes",
							 getvar_table[i].name.str,
							 resp_tmp);
						p++;
					}

					fastboot_tx_write_str(response);
				}
			}
			break;
		}

		case FB_SLOT_SUCCESSFUL: {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
			AvbABData ab_info;

			if (rk_avb_get_ab_info(&ab_info) < 0) {
				fb_add_string(actual_resp,
					      chars_left,
					      "%s:get ab info failed!",
					      getvar_table[i].name.str);
				fastboot_tx_write_str(response);
				break;
			}

			if (ab_info.slots[0].successful_boot)
				fb_add_string(actual_resp, chars_left,
					      "%s:a:yes",
					      getvar_table[i].name.str);
			else
				fb_add_string(actual_resp, chars_left,
					      "%s:a:no",
					      getvar_table[i].name.str);
			fastboot_tx_write_str(response);

			if (ab_info.slots[1].successful_boot)
				fb_add_string(actual_resp, chars_left,
					      "%s:b:yes",
					      getvar_table[i].name.str);
			else
				fb_add_string(actual_resp, chars_left,
					      "%s:b:no",
					      getvar_table[i].name.str);
			fastboot_tx_write_str(response);
#else
			fb_add_string(actual_resp, chars_left,
				      "%s:not find ab info!",
				      getvar_table[i].name.str);
			fastboot_tx_write_str(response);
#endif
			break;
		}

		case FB_SLOT_UNBOOTABLE: {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
			AvbABData ab_info;

			if (rk_avb_get_ab_info(&ab_info) < 0) {
				fb_add_string(actual_resp, chars_left,
					      "%s:not find ab info!",
					      getvar_table[i].name.str);
				fastboot_tx_write_str(response);
				break;
			}

			if (!ab_info.slots[0].successful_boot &&
			    !ab_info.slots[0].tries_remaining &&
			    !ab_info.slots[0].priority)
				fb_add_string(actual_resp, chars_left,
					      "%s:a:yes",
					      getvar_table[i].name.str);
			else
				fb_add_string(actual_resp, chars_left,
					      "%s:a:no",
					      getvar_table[i].name.str);
			fastboot_tx_write_str(response);

			if (!ab_info.slots[1].successful_boot &&
			    !ab_info.slots[1].tries_remaining &&
			    !ab_info.slots[1].priority)
				fb_add_string(actual_resp, chars_left,
					      "%s:b:yes",
					      getvar_table[i].name.str);
			else
				fb_add_string(actual_resp, chars_left,
					      "%s:b:no",
					      getvar_table[i].name.str);

			fastboot_tx_write_str(response);
#else
			fb_add_string(actual_resp, chars_left,
				      "%s:not find ab info!",
				      getvar_table[i].name.str);
			fastboot_tx_write_str(response);
#endif
			break;
		}

		case FB_SLOT_RETRY_COUNT: {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
			AvbABData ab_info;

			if (rk_avb_get_ab_info(&ab_info) < 0) {
				fb_add_string(actual_resp, chars_left,
					      "%s:not find ab info!",
					      getvar_table[i].name.str);
				fastboot_tx_write_str(response);
				break;
			}

			snprintf(actual_resp, chars_left, "%s:a:%d",
				 getvar_table[i].name.str,
				 ab_info.slots[1].tries_remaining);
			fastboot_tx_write_str(response);
			snprintf(actual_resp, chars_left, "%s:b:%d",
				 getvar_table[i].name.str,
				 ab_info.slots[1].tries_remaining);
			fastboot_tx_write_str(response);
#else
			fb_add_string(actual_resp, chars_left,
				      "%s:not find ab info!",
				      getvar_table[i].name.str);
			fastboot_tx_write_str(response);
#endif
			break;
		}
#endif
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		case FB_AT_VBST:
			break;
#endif
		default:
			fb_getvar_single((char *)getvar_table[i].name.str,
					 resp_tmp, FASTBOOT_RESPONSE_LEN);
			snprintf(actual_resp, chars_left, "%s:%s",
				 getvar_table[i].name.str, resp_tmp);
			fastboot_tx_write_str(response);
		}
	}
}

#ifdef CONFIG_ANDROID_AB
static int get_current_slot(void)
{
#ifdef CONFIG_RK_AVB_LIBAVB_USER
	char cmd[8] = {0};
	unsigned int slot_number = -1;

	memset(cmd, 0x0, sizeof(cmd));
	rk_avb_get_current_slot(cmd);
	if (strncmp("_a", cmd, 2) == 0) {
		slot_number = 0;
	} else if (strncmp("_b", cmd, 2) == 0) {
		slot_number = 1;
	} else {
		pr_err("%s: FAILunkown slot name\n", __func__);
		return -1;
	}

	return slot_number;
#else
	pr_err("%s: FAILnot implemented\n", __func__);
	return -1;
#endif
}

#ifdef CONFIG_FASTBOOT_FLASH
static int should_prevent_userdata_wipe(void)
{
	struct misc_virtual_ab_message state;

	memset(&state, 0x0, sizeof(state));
	if (read_misc_virtual_ab_message(&state) != 0) {
		pr_err("%s: read_misc_virtual_ab_message failed!\n", __func__);
		return 0;
	}

	if (state.magic != MISC_VIRTUAL_AB_MAGIC_HEADER) {
		pr_err("%s: NOT virtual A/B metadata!\n", __func__);
		return 0;
	}

	if (state.merge_status == (uint8_t)ENUM_MERGE_STATUS_MERGING ||
		(state.merge_status == (uint8_t)ENUM_MERGE_STATUS_SNAPSHOTTED &&
		state.source_slot != get_current_slot())) {
		return 1;
	}

	return 0;
}
#endif

static int get_virtual_ab_merge_status(void)
{
	struct misc_virtual_ab_message state;

	memset(&state, 0x0, sizeof(state));
	if (read_misc_virtual_ab_message(&state) != 0) {
		pr_err("%s: read_misc_virtual_ab_message failed!\n", __func__);
		return -1;
	}

	if (state.magic != MISC_VIRTUAL_AB_MAGIC_HEADER) {
		pr_err("%s: NOT virtual A/B metadata!\n", __func__);
		return -1;
	}

	return state.merge_status;
}
#endif

static void cb_getvar(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;
	char response[FASTBOOT_RESPONSE_LEN] = {0};
	const char *str_read_all = "all";
	size_t len = 0;
	size_t chars_left;

	strsep(&cmd, ":");
	if (!cmd) {
		pr_err("missing variable");
		fastboot_tx_write_str("FAILmissing var");
		return;
	}

	len = strlen(cmd);
	if (len == strlen(str_read_all) &&
	    (strncmp(cmd, str_read_all, len) == 0)) {
		fb_getvar_all();
		fastboot_tx_write_str("OKAYDone!");
	} else {
		strcpy(response, "OKAY");
		chars_left = sizeof(response) - strlen(response) - 1;

		if (fb_getvar_single(cmd, &response[strlen(response)],
				     chars_left) < 0) {
			strcpy(cmd, "FAILunknown variable");
			strncat(cmd, &response[strlen(response)], chars_left);
			fastboot_tx_write_str(cmd);
			return;
		}
		fastboot_tx_write_str(response);
	}

	return;
}

static unsigned int rx_bytes_expected(struct usb_ep *ep)
{
	int rx_remain = download_size - download_bytes;
	unsigned int rem;
	unsigned int maxpacket = ep->maxpacket;

	if (rx_remain <= 0)
		return 0;
	else if (rx_remain > EP_BUFFER_SIZE)
		return EP_BUFFER_SIZE;

	/*
	 * Some controllers e.g. DWC3 don't like OUT transfers to be
	 * not ending in maxpacket boundary. So just make them happy by
	 * always requesting for integral multiple of maxpackets.
	 * This shouldn't bother controllers that don't care about it.
	 */
	rem = rx_remain % maxpacket;
	if (rem > 0)
		rx_remain = rx_remain + (maxpacket - rem);

	return rx_remain;
}

#define BYTES_PER_DOT	0x20000
static void rx_handler_dl_image(struct usb_ep *ep, struct usb_request *req)
{
	char response[FASTBOOT_RESPONSE_LEN];
	unsigned int transfer_size = download_size - download_bytes;
	const unsigned char *buffer = req->buf;
	unsigned int buffer_size = req->actual;
	unsigned int pre_dot_num, now_dot_num;

	if (req->status != 0) {
		printf("Bad status: %d\n", req->status);
		return;
	}

	if (buffer_size < transfer_size)
		transfer_size = buffer_size;

	memcpy((void *)CONFIG_FASTBOOT_BUF_ADDR + download_bytes,
	       buffer, transfer_size);

	pre_dot_num = download_bytes / BYTES_PER_DOT;
	download_bytes += transfer_size;
	now_dot_num = download_bytes / BYTES_PER_DOT;

	if (pre_dot_num != now_dot_num) {
		putc('.');
		if (!(now_dot_num % 74))
			putc('\n');
	}

	/* Check if transfer is done */
	if (download_bytes >= download_size) {
		/*
		 * Reset global transfer variable, keep download_bytes because
		 * it will be used in the next possible flashing command
		 */
		download_size = 0;
		req->complete = rx_handler_command;
		req->length = EP_BUFFER_SIZE;

		strcpy(response, "OKAY");
		fastboot_tx_write_str(response);

		printf("\ndownloading of %d bytes finished\n", download_bytes);
	} else {
		req->length = rx_bytes_expected(ep);
	}

	req->actual = 0;
	usb_ep_queue(ep, req, 0);
}

static void cb_download(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;
	char response[FASTBOOT_RESPONSE_LEN];

	strsep(&cmd, ":");
	download_size = simple_strtoul(cmd, NULL, 16);
	download_bytes = 0;

	printf("Starting download of %d bytes\n", download_size);

	if (0 == download_size) {
		strcpy(response, "FAILdata invalid size");
	} else if (download_size > CONFIG_FASTBOOT_BUF_SIZE) {
		download_size = 0;
		strcpy(response, "FAILdata too large");
	} else {
		sprintf(response, "DATA%08x", download_size);
		req->complete = rx_handler_dl_image;
		req->length = rx_bytes_expected(ep);
	}

	fastboot_tx_write_str(response);
}

static void tx_handler_ul(struct usb_ep *ep, struct usb_request *req)
{
	unsigned int xfer_size = 0;
	unsigned int pre_dot_num, now_dot_num;
	unsigned int remain_size = 0;
	unsigned int transferred_size = req->actual;

	if (req->status != 0) {
		printf("Bad status: %d\n", req->status);
		return;
	}

	if (start_upload) {
		pre_dot_num = upload_bytes / BYTES_PER_DOT;
		upload_bytes += transferred_size;
		now_dot_num = upload_bytes / BYTES_PER_DOT;

		if (pre_dot_num != now_dot_num) {
			putc('.');
			if (!(now_dot_num % 74))
				putc('\n');
		}
	}

	remain_size = upload_size - upload_bytes;
	xfer_size = (remain_size > EP_BUFFER_SIZE) ?
		    EP_BUFFER_SIZE : remain_size;

	debug("%s: remain_size=%d, transferred_size=%d",
	      __func__, remain_size, transferred_size);
	debug("xfer_size=%d, upload_bytes=%d, upload_size=%d!\n",
	      xfer_size, upload_bytes, upload_size);

	if (remain_size <= 0) {
		fastboot_func->in_req->complete = fastboot_complete;
		wakeup_thread();
		fastboot_tx_write_str("OKAY");
		printf("\nuploading of %d bytes finished\n", upload_bytes);
		upload_bytes = 0;
		upload_size = 0;
		start_upload = false;
		return;
	}

	/* Remove the transfer callback which response the upload */
	/* request from host */
	if (!upload_bytes)
		start_upload = true;

	fastboot_tx_write((char *)((phys_addr_t)CONFIG_FASTBOOT_BUF_ADDR + \
			  upload_bytes),
			  xfer_size);
}

static void cb_upload(struct usb_ep *ep, struct usb_request *req)
{
	char response[FASTBOOT_RESPONSE_LEN];

	printf("Starting upload of %d bytes\n", upload_size);

	if (0 == upload_size) {
		strcpy(response, "FAILdata invalid size");
	} else {
		start_upload = false;
		sprintf(response, "DATA%08x", upload_size);
		fastboot_func->in_req->complete = tx_handler_ul;
	}

	fastboot_tx_write_str(response);
}

static void do_bootm_on_complete(struct usb_ep *ep, struct usb_request *req)
{
	char boot_addr_start[12];
	char *bootm_args[] = { "bootm", boot_addr_start, NULL };

	puts("Booting kernel..\n");

	sprintf(boot_addr_start, "0x%lx", (long)CONFIG_FASTBOOT_BUF_ADDR);
	do_bootm(NULL, 0, 2, bootm_args);

	/* This only happens if image is somehow faulty so we start over */
	do_reset(NULL, 0, 0, NULL);
}

static void cb_boot(struct usb_ep *ep, struct usb_request *req)
{
	fastboot_func->in_req->complete = do_bootm_on_complete;
	fastboot_tx_write_str("OKAY");
}

static void do_exit_on_complete(struct usb_ep *ep, struct usb_request *req)
{
	g_dnl_trigger_detach();
}

static void cb_continue(struct usb_ep *ep, struct usb_request *req)
{
	fastboot_func->in_req->complete = do_exit_on_complete;
	fastboot_tx_write_str("OKAY");
}

static void cb_set_active(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;

	debug("%s: %s\n", __func__, cmd);

	strsep(&cmd, ":");
	if (!cmd) {
		pr_err("missing slot name");
		fastboot_tx_write_str("FAILmissing slot name");
		return;
	}
#ifdef CONFIG_ANDROID_AB
	if (get_virtual_ab_merge_status() == ENUM_MERGE_STATUS_MERGING) {
		pr_err("virtual A/B is merging, abort the operation");
		fastboot_tx_write_str("FAILvirtual A/B is merging, abort");
		return;
	}
#endif
#ifdef CONFIG_RK_AVB_LIBAVB_USER
	unsigned int slot_number;
	if (strncmp("a", cmd, 1) == 0) {
		slot_number = 0;
		rk_avb_set_slot_active(&slot_number);
	} else if (strncmp("b", cmd, 1) == 0) {
		slot_number = 1;
		rk_avb_set_slot_active(&slot_number);
	} else {
		fastboot_tx_write_str("FAILunkown slot name");
		return;
	}

	fastboot_tx_write_str("OKAY");
	return;
#else
	fastboot_tx_write_str("FAILnot implemented");
	return;
#endif
}

#ifdef CONFIG_FASTBOOT_FLASH
static void cb_flash(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;
	char response[FASTBOOT_RESPONSE_LEN] = {0};
#ifdef CONFIG_RK_AVB_LIBAVB_USER
	uint8_t flash_lock_state;

	if (rk_avb_read_flash_lock_state(&flash_lock_state)) {
		/* write the device flashing unlock when first read */
		if (rk_avb_write_flash_lock_state(1)) {
			fastboot_tx_write_str("FAILflash lock state write failure");
			return;
		}
		if (rk_avb_read_flash_lock_state(&flash_lock_state)) {
			fastboot_tx_write_str("FAILflash lock state read failure");
			return;
		}
	}

	if (flash_lock_state == 0) {
		fastboot_tx_write_str("FAILThe device is locked, can not flash!");
		printf("The device is locked, can not flash!\n");
		return;
	}
#endif
	strsep(&cmd, ":");
	if (!cmd) {
		pr_err("missing partition name");
		fastboot_tx_write_str("FAILmissing partition name");
		return;
	}
#ifdef CONFIG_ANDROID_AB
	if ((strcmp(cmd, PART_USERDATA) == 0) || (strcmp(cmd, PART_METADATA) == 0)) {
		if (should_prevent_userdata_wipe()) {
			pr_err("FAILThe virtual A/B merging, can not flash userdata or metadata!\n");
			fastboot_tx_write_str("FAILvirtual A/B merging,abort flash!");
			return;
		}
	}
#endif
	fastboot_fail("no flash device defined", response);
#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
	fb_mmc_flash_write(cmd, (void *)CONFIG_FASTBOOT_BUF_ADDR,
				download_bytes, response);
#endif
#ifdef CONFIG_FASTBOOT_FLASH_NAND_DEV
	fb_nand_flash_write(cmd, (void *)CONFIG_FASTBOOT_BUF_ADDR,
				download_bytes, response);
#endif
	fastboot_tx_write_str(response);
}

static void cb_flashing(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;

	if (strncmp("lock", cmd + 9, 4) == 0) {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		uint8_t flash_lock_state;
		flash_lock_state = 0;
		if (rk_avb_write_flash_lock_state(flash_lock_state))
			fastboot_tx_write_str("FAILflash lock state"
					      " write failure");
		else
			fastboot_tx_write_str("OKAY");
#else
		fastboot_tx_write_str("FAILnot implemented");
#endif
	} else if (strncmp("unlock", cmd + 9, 6) == 0) {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		uint8_t flash_lock_state;
		flash_lock_state = 1;
		if (rk_avb_write_flash_lock_state(flash_lock_state))
			fastboot_tx_write_str("FAILflash lock state"
					      " write failure");
		else
			fastboot_tx_write_str("OKAY");
#else
		fastboot_tx_write_str("FAILnot implemented");
#endif
	} else if (strncmp("lock_critical", cmd + 9, 12) == 0) {
		fastboot_tx_write_str("FAILnot implemented");
	} else if (strncmp("unlock_critical", cmd + 9, 14) == 0) {
		fastboot_tx_write_str("FAILnot implemented");
	} else if (strncmp("get_unlock_ability", cmd + 9, 17) == 0) {
		fastboot_tx_write_str("FAILnot implemented");
	} else if (strncmp("get_unlock_bootloader_nonce", cmd + 4, 27) == 0) {
		fastboot_tx_write_str("FAILnot implemented");
	} else if (strncmp("unlock_bootloader", cmd + 9, 17) == 0) {
		fastboot_tx_write_str("FAILnot implemented");
	} else if (strncmp("lock_bootloader", cmd + 9, 15) == 0) {
		fastboot_tx_write_str("FAILnot implemented");
	} else {
		fastboot_tx_write_str("FAILunknown flashing command");
	}
}
#endif

static void cb_oem_perm_attr(void)
{
#ifdef CONFIG_RK_AVB_LIBAVB_USER
#ifndef CONFIG_ROCKCHIP_PRELOADER_PUB_KEY
	sha256_context ctx;
	uint8_t digest[SHA256_SUM_LEN] = {0};
	uint8_t digest_temp[SHA256_SUM_LEN] = {0};
	uint8_t perm_attr_temp[PERM_ATTR_TOTAL_SIZE] = {0};
	uint8_t flag = 0;
#endif
	if (PERM_ATTR_TOTAL_SIZE != download_bytes) {
		printf("Permanent attribute size is not equal!\n");
		fastboot_tx_write_str("FAILincorrect perm attribute size");
		return;
	}
#ifndef CONFIG_ROCKCHIP_PRELOADER_PUB_KEY
	if (rk_avb_read_perm_attr_flag(&flag)) {
		printf("rk_avb_read_perm_attr_flag error!\n");
		fastboot_tx_write_str("FAILperm attr read failed");
		return;
	}

	if (flag == PERM_ATTR_SUCCESS_FLAG) {
		if (rk_avb_read_attribute_hash(digest_temp,
					       SHA256_SUM_LEN)) {
			printf("The efuse IO can not be used!\n");
			fastboot_tx_write_str("FAILefuse IO can not be used");
			return;
		}

		if (memcmp(digest, digest_temp, SHA256_SUM_LEN) != 0) {
			if (rk_avb_read_permanent_attributes(perm_attr_temp,
							     PERM_ATTR_TOTAL_SIZE)) {
				printf("rk_avb_write_permanent_attributes error!\n");
				fastboot_tx_write_str("FAILread perm attr error");
				return;
			}

			sha256_starts(&ctx);
			sha256_update(&ctx,
				      (const uint8_t *)perm_attr_temp,
				      PERM_ATTR_TOTAL_SIZE);
			sha256_finish(&ctx, digest);
			if (memcmp(digest, digest_temp, SHA256_SUM_LEN) == 0) {
				printf("The hash has been written!\n");
				fastboot_tx_write_str("OKAY");
				return;
			}
		}

		if (rk_avb_write_perm_attr_flag(0)) {
			fastboot_tx_write_str("FAILperm attr flag write failure");
			return;
		}
	}
#endif
	if (rk_avb_write_permanent_attributes((uint8_t *)
					      CONFIG_FASTBOOT_BUF_ADDR,
					      download_bytes)) {
		if (rk_avb_write_perm_attr_flag(0)) {
			fastboot_tx_write_str("FAILperm attr flag write failure");
			return;
		}
		fastboot_tx_write_str("FAILperm attr write failed");
		return;
	}
#ifndef CONFIG_ROCKCHIP_PRELOADER_PUB_KEY
	memset(digest, 0, SHA256_SUM_LEN);
	sha256_starts(&ctx);
	sha256_update(&ctx, (const uint8_t *)CONFIG_FASTBOOT_BUF_ADDR,
		      PERM_ATTR_TOTAL_SIZE);
	sha256_finish(&ctx, digest);

	if (rk_avb_write_attribute_hash((uint8_t *)digest,
					SHA256_SUM_LEN)) {
		if (rk_avb_read_attribute_hash(digest_temp,
						SHA256_SUM_LEN)) {
			printf("The efuse IO can not be used!\n");
			fastboot_tx_write_str("FAILefuse IO can not be used");
			return;
		}
		if (memcmp(digest, digest_temp, SHA256_SUM_LEN) != 0) {
			if (rk_avb_write_perm_attr_flag(0)) {
				fastboot_tx_write_str("FAILperm attr flag write failure");
				return;
			}
			printf("The hash has been written, but is different!\n");
			fastboot_tx_write_str("FAILhash comparison failure");
			return;
		}
	}
#endif
	if (rk_avb_write_perm_attr_flag(PERM_ATTR_SUCCESS_FLAG)) {
		fastboot_tx_write_str("FAILperm attr flag write failure");
		return;
	}

	fastboot_tx_write_str("OKAY");
#else
	fastboot_tx_write_str("FAILnot implemented");
#endif
}

static void cb_oem_perm_attr_rsa_cer(void)
{
#ifdef CONFIG_RK_AVB_LIBAVB_USER
	if (download_bytes != 256) {
		printf("Permanent attribute rsahash size is not equal!\n");
		fastboot_tx_write_str("FAILperm attribute rsahash size error");
		return;
	}

	if (rk_avb_set_perm_attr_cer((uint8_t *)CONFIG_FASTBOOT_BUF_ADDR,
				     download_bytes)) {
		fastboot_tx_write_str("FAILSet perm attr cer fail!");
		return;
	}

	fastboot_tx_write_str("OKAY");
#else
	fastboot_tx_write_str("FAILnot implemented");
#endif
}

static void cb_oem(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;

#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
	if (strncmp("format", cmd + 4, 6) == 0) {
		char cmdbuf[32];
		sprintf(cmdbuf, "gpt write mmc %x $partitions",
			CONFIG_FASTBOOT_FLASH_MMC_DEV);
#ifdef CONFIG_ANDROID_AB
		if (should_prevent_userdata_wipe()) {
			printf("FAILThe virtual A/B merging, can not format!\n");
			fastboot_tx_write_str("FAILvirtual A/B merging,abort format!");
		} else {
			if (run_command(cmdbuf, 0))
				fastboot_tx_write_str("FAILmmc write failure");
			else
				fastboot_tx_write_str("OKAY");
		}
#else
	if (run_command(cmdbuf, 0))
		fastboot_tx_write_str("FAILmmc write failure");
	else
		fastboot_tx_write_str("OKAY");
#endif
	} else
#endif
	if (strncmp("unlock", cmd + 4, 8) == 0) {
#ifdef CONFIG_FASTBOOT_OEM_UNLOCK
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		fastboot_tx_write_str("FAILnot implemented");
		return;
#else
		uint8_t unlock = 0;
		TEEC_Result result;
		debug("oem unlock\n");
		result = trusty_read_oem_unlock(&unlock);
		if (result) {
			printf("read oem unlock status with error : 0x%x\n", result);
			fastboot_tx_write_str("FAILRead oem unlock status failed");
			return;
		}
		if (unlock) {
			printf("oem unlock ignored, device already unlocked\n");
			fastboot_tx_write_str("FAILalready unlocked");
			return;
		}
		printf("oem unlock requested:\n");
		printf("\tUnlocking forces a factory reset and could\n");
		printf("\topen your device up to a world of hurt.  If you\n");
		printf("\tare sure you know what you're doing, then accept\n");
		printf("\tvia 'fastboot oem unlock_accept'.\n");
		env_set("unlock", "unlock");
		fastboot_tx_write_str("OKAY");
#endif
#else
		fastboot_tx_write_str("FAILnot implemented");
		return;
#endif
	} else if (strncmp("unlock_accept", cmd + 4, 13) == 0) {
#ifdef CONFIG_FASTBOOT_OEM_UNLOCK
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		fastboot_tx_write_str("FAILnot implemented");
		return;
#else
		char *unlock = env_get("unlock");
		TEEC_Result result;
		debug("oem unlock_accept\n");
		if (unlock == NULL || strncmp("unlock", unlock, 6) != 0) {
			printf("oem unlock_accept ignored, not pending\n");
			fastboot_tx_write_str("FAILoem unlock not requested");
			return;
		}
		env_set("unlock", "");
		printf("Erasing userdata partition\n");
		struct blk_desc *dev_desc;
		disk_partition_t part_info;
		dev_desc = rockchip_get_bootdev();
		if (!dev_desc) {
			printf("%s: dev_desc is NULL!\n", __func__);
			return;
		}
		int ret = part_get_info_by_name(dev_desc, "userdata",
				&part_info);
		if (ret < 0) {
			printf("not found userdata partition");
			printf("Erase failed with error %d\n", ret);
			fastboot_tx_write_str("FAILErasing userdata failed");
			return;
		}
		ret = blk_derase(dev_desc, part_info.start, part_info.size);
		if (ret != part_info.size) {
			printf("Erase failed with error %d\n", ret);
			fastboot_tx_write_str("FAILErasing userdata failed");
			return;
		}
		printf("Erasing succeeded\n");

		result = trusty_write_oem_unlock(1);
		if (result) {
			printf("write oem unlock status with error : 0x%x\n", result);
			fastboot_tx_write_str("FAILWrite oem unlock status failed");
			return;
		}
		fastboot_tx_write_str("OKAY");

		/*
		 * now reboot into recovery to do a format of the
		 * userdata partition so it's ready to use on next boot
		 */
		board_run_recovery_wipe_data();
#endif
#else
		fastboot_tx_write_str("FAILnot implemented");
		return;
#endif
	} else if (strncmp("lock", cmd + 4, 8) == 0) {
#ifdef CONFIG_FASTBOOT_OEM_UNLOCK
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		fastboot_tx_write_str("FAILnot implemented");
		return;
#else
		TEEC_Result result;
		uint8_t unlock = 0;
		trusty_read_oem_unlock(&unlock);
		if (!unlock) {
			printf("oem lock ignored, already locked\n");
			fastboot_tx_write_str("FAILalready locked");
			return;
		}

		result = trusty_write_oem_unlock(0);
		if (result) {
			printf("write oem unlock status with error : 0x%x\n", result);
			fastboot_tx_write_str("FAILWrite oem unlock status failed");
			return;
		}
		fastboot_tx_write_str("OKAY");
#endif
#else
		fastboot_tx_write_str("FAILnot implemented");
		return;
#endif
	} else if (strncmp("at-get-ca-request", cmd + 4, 17) == 0) {
#ifdef CONFIG_OPTEE_CLIENT
		uint8_t out[ATTEST_CA_OUT_SIZE];
		uint32_t operation_size = download_bytes;
		uint32_t out_len = ATTEST_CA_OUT_SIZE;
		uint32_t res = 0;

		res = trusty_attest_get_ca((uint8_t *)CONFIG_FASTBOOT_BUF_ADDR,
					   &operation_size, out, &out_len);
		if (res) {
			fastboot_tx_write_str("FAILtrusty_attest_get_ca failed");
			return;
		}
		upload_size = out_len;
		memcpy((void *)CONFIG_FASTBOOT_BUF_ADDR, out, out_len);
		fastboot_tx_write_str("OKAY");
#else
		fastboot_tx_write_str("FAILnot implemented");
		return;
#endif
	} else if (strncmp("at-set-ca-response", cmd + 4, 18) == 0) {
#ifdef CONFIG_OPTEE_CLIENT
		uint32_t ca_response_size = download_bytes;
		uint32_t res = 0;

		res = trusty_attest_set_ca((uint8_t *)CONFIG_FASTBOOT_BUF_ADDR,
					   &ca_response_size);
		if (res)
			fastboot_tx_write_str("FAILtrusty_attest_set_ca failed");
		else
			fastboot_tx_write_str("OKAY");
#else
		fastboot_tx_write_str("FAILnot implemented");
		return;
#endif
	} else if (strncmp("at-get-vboot-unlock-challenge", cmd + 4, 29) == 0) {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		uint32_t challenge_len = 0;
		int ret = 0;

		ret = rk_generate_unlock_challenge((void *)CONFIG_FASTBOOT_BUF_ADDR, &challenge_len);
		if (ret == 0) {
			upload_size = challenge_len;
			fastboot_tx_write_str("OKAY");
		} else {
			fastboot_tx_write_str("FAILgenerate unlock challenge fail!");
		}
#else
		fastboot_tx_write_str("FAILnot implemented");
		return;
#endif
	} else if (strncmp("at-lock-vboot", cmd + 4, 13) == 0) {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		uint8_t lock_state;
		lock_state = 0;
		if (rk_avb_write_lock_state(lock_state))
			fastboot_tx_write_str("FAILwrite lock state failed");
		else
			fastboot_tx_write_str("OKAY");
#else
		fastboot_tx_write_str("FAILnot implemented");
#endif
	} else if (strncmp("at-unlock-vboot", cmd + 4, 15) == 0) {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		uint8_t lock_state;
		char out_is_trusted = true;

		if (rk_avb_read_lock_state(&lock_state))
			fastboot_tx_write_str("FAILlock sate read failure");
		if (lock_state >> 1 == 1) {
			fastboot_tx_write_str("FAILThe vboot is disable!");
		} else {
			lock_state = 1;
#ifdef CONFIG_RK_AVB_LIBAVB_ENABLE_ATH_UNLOCK
			if (rk_auth_unlock((void *)CONFIG_FASTBOOT_BUF_ADDR,
					   &out_is_trusted)) {
				printf("rk_auth_unlock ops error!\n");
				fastboot_tx_write_str("FAILrk_auth_unlock ops error!");
				return;
			}
#endif
			if (out_is_trusted == true) {
				if (rk_avb_write_lock_state(lock_state))
					fastboot_tx_write_str("FAILwrite lock state failed");
				else
					fastboot_tx_write_str("OKAY");
			} else {
				fastboot_tx_write_str("FAILauthenticated unlock fail");
			}
		}
#else
		fastboot_tx_write_str("FAILnot implemented");
#endif
	} else if (strncmp("fuse at-perm-attr", cmd + 4, 16) == 0) {
		cb_oem_perm_attr();
	} else if (strncmp("fuse at-rsa-perm-attr", cmd + 4, 25) == 0) {
		cb_oem_perm_attr_rsa_cer();
	} else if (strncmp("fuse at-bootloader-vboot-key", cmd + 4, 27) == 0) {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		sha256_context ctx;
		uint8_t digest[SHA256_SUM_LEN];

		if (download_bytes != VBOOT_KEY_HASH_SIZE) {
			fastboot_tx_write_str("FAILinvalid vboot key length");
			printf("The vboot key size error!\n");
			return;
		}

		sha256_starts(&ctx);
		sha256_update(&ctx, (const uint8_t *)CONFIG_FASTBOOT_BUF_ADDR,
			      VBOOT_KEY_SIZE);
		sha256_finish(&ctx, digest);

		if (rk_avb_write_vbootkey_hash((uint8_t *)digest,
					       SHA256_SUM_LEN)) {
			fastboot_tx_write_str("FAILvbootkey hash write failure");
			return;
		}
		fastboot_tx_write_str("OKAY");
#else
		fastboot_tx_write_str("FAILnot implemented");
#endif
	} else if (strncmp("init-ab-metadata", cmd + 4, 16) == 0) {
#ifdef CONFIG_RK_AVB_LIBAVB_USER
		if (rk_avb_init_ab_metadata()) {
			fastboot_tx_write_str("FAILinit ab data fail!");
			return;
		}
		fastboot_tx_write_str("OKAY");
#else
		fastboot_tx_write_str("FAILnot implemented");
#endif
	} else {
		fastboot_tx_write_str("FAILunknown oem command");
	}
}

#ifdef CONFIG_FASTBOOT_FLASH
static void cb_erase(struct usb_ep *ep, struct usb_request *req)
{
	char *cmd = req->buf;
	char response[FASTBOOT_RESPONSE_LEN] = {0};

	strsep(&cmd, ":");
	if (!cmd) {
		pr_err("missing partition name");
		fastboot_tx_write_str("FAILmissing partition name");
		return;
	}
#ifdef CONFIG_ANDROID_AB
	if ((strcmp(cmd, PART_USERDATA) == 0) || (strcmp(cmd, PART_METADATA) == 0)) {
		if (should_prevent_userdata_wipe()) {
			pr_err("virtual A/B merging, can not erase userdata or metadata!\n");
			fastboot_tx_write_str("FAILvirtual A/B merging,abort erase!");
			return;
		}
	}
#endif
	fastboot_fail("no flash device defined", response);
#ifdef CONFIG_FASTBOOT_FLASH_MMC_DEV
	fb_mmc_erase(cmd, response);
#endif
#ifdef CONFIG_FASTBOOT_FLASH_NAND_DEV
	fb_nand_erase(cmd, response);
#endif
	fastboot_tx_write_str(response);
}
#endif

struct cmd_dispatch_info {
	char *cmd;
	void (*cb)(struct usb_ep *ep, struct usb_request *req);
};

static const struct cmd_dispatch_info cmd_dispatch_info[] = {
	{
		.cmd = "reboot",
		.cb = cb_reboot,
	}, {
		.cmd = "getvar:",
		.cb = cb_getvar,
	}, {
		.cmd = "download:",
		.cb = cb_download,
	}, {
		.cmd = "upload",
		.cb = cb_upload,
	}, {
		.cmd = "boot",
		.cb = cb_boot,
	}, {
		.cmd = "continue",
		.cb = cb_continue,
	}, {
		.cmd = "set_active",
		.cb = cb_set_active,
	},
#ifdef CONFIG_FASTBOOT_FLASH
	{
		.cmd = "flashing",
		.cb = cb_flashing,
	},
	{
		.cmd = "flash",
		.cb = cb_flash,
	}, {
		.cmd = "erase",
		.cb = cb_erase,
	},
#endif
	{
		.cmd = "oem",
		.cb = cb_oem,
	},
};

static void rx_handler_command(struct usb_ep *ep, struct usb_request *req)
{
	char *cmdbuf = req->buf;
	void (*func_cb)(struct usb_ep *ep, struct usb_request *req) = NULL;
	int i;

	if (req->status != 0 || req->length == 0)
		return;

	for (i = 0; i < ARRAY_SIZE(cmd_dispatch_info); i++) {
		if (!strcmp_l1(cmd_dispatch_info[i].cmd, cmdbuf)) {
			func_cb = cmd_dispatch_info[i].cb;
			break;
		}
	}

	if (!func_cb) {
		pr_err("unknown command: %.*s", req->actual, cmdbuf);
		fastboot_tx_write_str("FAILunknown command");
	} else {
		if (req->actual < req->length) {
			u8 *buf = (u8 *)req->buf;
			buf[req->actual] = 0;
			func_cb(ep, req);
		} else {
			pr_err("buffer overflow");
			fastboot_tx_write_str("FAILbuffer overflow");
		}
	}

	*cmdbuf = '\0';
	req->actual = 0;
	usb_ep_queue(ep, req, 0);
}
