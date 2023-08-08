/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <power/rk8xx_pmic.h>

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_r(void)
{
	struct udevice *pmic;
	int ret;

	ret = uclass_first_device_err(UCLASS_PMIC, &pmic);
	if (ret)
		return ret;

	/* Increase USB input current to 2A */
	ret = rk818_spl_configure_usb_input_current(pmic, 2000);
	if (ret)
		return ret;

	return 0;
}
