/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>

/* Firmware access is platform-dependent.  No generic code in uclass */
UCLASS_DRIVER(firmware) = {
	.id		= UCLASS_FIRMWARE,
	.name		= "firmware",
};

U_BOOT_DRIVER(firmware) = {
	.name	= "firmware",
	.id	= UCLASS_FIRMWARE,
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	.bind = dm_scan_fdt_dev,
#endif
};
