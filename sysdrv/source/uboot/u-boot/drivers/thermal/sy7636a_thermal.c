// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd.
 */

#include <common.h>
#include <dm.h>
#include <thermal.h>
#include <power/pmic.h>
#include <power/sy7636a.h>

DECLARE_GLOBAL_DATA_PTR;

static int sy7636a_get_temp(struct udevice *dev, int *temp)
{
        struct udevice *pmic = dev_get_parent(dev);
        int ret;

        ret = pmic_reg_read(pmic, SY7636A_REG_TERMISTOR_READOUT);
        if (ret < 0)
                return ret;

        *temp = *((signed char *)&ret);

	return 0;
}

static const struct dm_thermal_ops sy7636a_thermal_ops = {
	.get_temp = sy7636a_get_temp,
};

static const struct udevice_id sy7636a_thermal_of_match[] = {
	{ .compatible = SY7636A_THERMAL_COMTATIBLE_NAME },
        { }
};

U_BOOT_DRIVER(sy7636a_thermal) = {
	.name		= SY7636A_THERMAL_COMTATIBLE_NAME,
	.id		= UCLASS_THERMAL,
	.of_match	= sy7636a_thermal_of_match,
	.ops		= &sy7636a_thermal_ops,
};
