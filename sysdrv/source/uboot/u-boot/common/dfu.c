/*
 * dfu.c -- dfu command
 *
 * Copyright (C) 2015
 * Lukasz Majewski <l.majewski@majess.pl>
 *
 * Copyright (C) 2012 Samsung Electronics
 * authors: Andrzej Pietrasiewicz <andrzej.p@samsung.com>
 *	    Lukasz Majewski <l.majewski@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <watchdog.h>
#include <dfu.h>
#include <console.h>
#include <g_dnl.h>
#include <usb.h>
#include <net.h>
#include <android_avb/rk_avb_ops_user.h>

int run_usb_dnl_gadget(int usbctrl_index, char *usb_dnl_gadget)
{
	bool dfu_reset = false;
	int ret, i = 0;
#ifdef CONFIG_ANDROID_AB
	char select_slot[3] = {0};
	unsigned int slot_number[2] = {0, 1};
#endif

	ret = usb_gadget_initialize(usbctrl_index);
	if (ret) {
		pr_err("usb_gadget_initialize failed\n");
		return CMD_RET_FAILURE;
	}
	g_dnl_clear_detach();
	ret = g_dnl_register(usb_dnl_gadget);
	if (ret) {
		pr_err("g_dnl_register failed");
		return CMD_RET_FAILURE;
	}

	while (1) {
		if (g_dnl_detach()) {
			/*
			 * Check if USB bus reset is performed after detach,
			 * which indicates that -R switch has been passed to
			 * dfu-util. In this case reboot the device
			 */
			if (dfu_usb_get_reset()) {
				dfu_reset = true;
#ifdef CONFIG_ANDROID_AB
				if (rk_avb_get_current_slot(select_slot))
					printf("Obtain current slot failed!\n");
				/*
				 * After the firmware is successfully upgrade,
				 * the device changes the slot priority during
				 * reboot based on the current slot
				 */
				if (strcmp(select_slot, "_a") == 0)
					rk_avb_set_slot_active(&slot_number[1]);
				else
					rk_avb_set_slot_active(&slot_number[0]);
#endif
				goto exit;
			}

			/*
			 * This extra number of usb_gadget_handle_interrupts()
			 * calls is necessary to assure correct transmission
			 * completion with dfu-util
			 */
			if (++i == 10000)
				goto exit;
		}

		if (ctrlc())
			goto exit;

		if (dfu_get_defer_flush()) {
			/*
			 * Call to usb_gadget_handle_interrupts() is necessary
			 * to act on ZLP OUT transaction from HOST PC after
			 * transmitting the whole file.
			 *
			 * If this ZLP OUT packet is NAK'ed, the HOST libusb
			 * function fails after timeout (by default it is set to
			 * 5 seconds). In such situation the dfu-util program
			 * exits with error message.
			 */
			usb_gadget_handle_interrupts(usbctrl_index);
			ret = dfu_flush(dfu_get_defer_flush(), NULL, 0, 0);
			dfu_set_defer_flush(NULL);
			if (ret) {
				pr_err("Deferred dfu_flush() failed!");
				goto exit;
			}
		}

		WATCHDOG_RESET();
		usb_gadget_handle_interrupts(usbctrl_index);
	}
exit:
	g_dnl_unregister();
	usb_gadget_release(usbctrl_index);

	if (dfu_reset)
		do_reset(NULL, 0, 0, NULL);

	g_dnl_clear_detach();

	return ret;
}
