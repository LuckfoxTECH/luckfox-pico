// SPDX-License-Identifier:     GPL-2.0
/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd
 */

#include <asm/io.h>
#include <usb.h>
#include <dwc3-uboot.h>
#include <linux/usb/phy-rockchip-inno-usb3.h>

#if CONFIG_IS_ENABLED(USB_DWC3_GADGET)
static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_SUPER,
	.base = 0xff600000,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
	.dis_u2_susphy_quirk = 1,
	.dis_u1u2_quirk = 1,
	.usb2_phyif_utmi_width = 16,
};

int usb_gadget_handle_interrupts(void)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	rockchip_u3phy_uboot_init();
	return dwc3_uboot_init(&dwc3_device_data);
}
#endif
