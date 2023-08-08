/*
 * Copyright 2008 - 2009 Windriver, <www.windriver.com>
 * Author: Tom Rix <Tom.Rix@windriver.com>
 *
 * (C) Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <command.h>
#include <console.h>
#include <g_dnl.h>
#include <net.h>
#include <usb.h>
#include <sysmem.h>

static int do_fastboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
#ifdef CONFIG_USB_FUNCTION_FASTBOOT
	int controller_index;
	char *usb_controller;
	int ret;
#endif
	if (argc < 2)
		return CMD_RET_USAGE;

	printf("Enter fastboot...");

	if (!strcmp(argv[1], "udp")) {
#ifndef CONFIG_UDP_FUNCTION_FASTBOOT
		pr_err("Fastboot UDP not enabled\n");
		return -1;
#else
		return do_fastboot_udp(cmdtp, flag, argc, argv);
#endif
	}

	if (strcmp(argv[1], "usb") || argc < 3)
		return CMD_RET_USAGE;

#ifndef CONFIG_USB_FUNCTION_FASTBOOT
	pr_err("Fastboot USB not enabled\n");
	return -1;
#else

	usb_controller = argv[2];
	controller_index = simple_strtoul(usb_controller, NULL, 0);

	ret = usb_gadget_initialize(controller_index);
	if (ret) {
		pr_err("USB init failed: %d", ret);
		return CMD_RET_FAILURE;
	}

	g_dnl_clear_detach();
	ret = g_dnl_register("usb_dnl_fastboot");
	if (ret)
		return ret;

	if (!g_dnl_board_usb_cable_connected()) {
		puts("\rUSB cable not detected.\n" \
		     "Command exit.\n");
		ret = CMD_RET_FAILURE;
		goto exit;
	}

	if (!sysmem_alloc_base(MEM_FASTBOOT,
			       CONFIG_FASTBOOT_BUF_ADDR,
			       CONFIG_FASTBOOT_BUF_SIZE)) {
		printf("The fastboot memory space is unusable!\n");
		return CMD_RET_FAILURE;
	}

	printf("OK\n");

	while (1) {
		if (g_dnl_detach())
			break;
		if (ctrlc())
			break;
		usb_gadget_handle_interrupts(controller_index);
	}

	ret = CMD_RET_SUCCESS;

exit:
	sysmem_free(CONFIG_FASTBOOT_BUF_ADDR);
	g_dnl_unregister();
	g_dnl_clear_detach();
	usb_gadget_release(controller_index);

	return ret;
#endif
}

U_BOOT_CMD(
	fastboot, 3, 1, do_fastboot,
	"use USB or UDP Fastboot protocol",
	"[usb,udp] <USB_controller>\n"
	" - run as a fastboot usb or udp device\n"
	"   usb: specify <USB_controller>\n"
	"   udp: requires ip_addr set and ethernet initialized\n"
);
