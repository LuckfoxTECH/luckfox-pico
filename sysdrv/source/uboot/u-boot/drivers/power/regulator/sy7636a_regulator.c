// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd.
 */

#include <common.h>
#include <dm.h>
#include <power/pmic.h>
#include <power/regulator.h>
#include <power/sy7636a.h>

DECLARE_GLOBAL_DATA_PTR;

static int sy7636a_get_enable(struct udevice *dev)
{
        struct udevice *pmic = dev_get_parent(dev);
        int ret;

        ret = pmic_reg_read(pmic, SY7636A_REG_OPERATION_MODE_CRL);
        if (ret < 0)
                return ret;

        return !!(ret & SY7636A_OPERATION_MODE_CRL_VCOMCTL);
}

static int sy7636a_set_enable(struct udevice *dev, bool enable)
{
        struct udevice *pmic = dev_get_parent(dev);
        int ret;

        if (enable)
                ret = pmic_clrsetbits(pmic, SY7636A_REG_OPERATION_MODE_CRL,
                                      0, SY7636A_OPERATION_MODE_CRL_ONOFF);
        else
                ret = pmic_clrsetbits(pmic, SY7636A_REG_OPERATION_MODE_CRL,
                                      SY7636A_OPERATION_MODE_CRL_ONOFF, 0);

        return ret;
}

static int sy7636a_set_value(struct udevice *dev, int uV)
{
        struct udevice *pmic = dev_get_parent(dev);
        int mV;
        u32 val, origin_val;
        int ret;

        mV = uV / 1000;
	val = mV / 10;

        /* VCOM[7:0] */
        ret = pmic_reg_read(pmic, SY7636A_REG_VCOM_ADJUST_CTRL_L);
        if (ret < 0)
                return ret;

        origin_val = ret & 0xFF;

        /* VCOM[8] */
        ret = pmic_reg_read(pmic, SY7636A_REG_VCOM_ADJUST_CTRL_H);
        if (ret < 0)
                return ret;

        origin_val |= (ret & VCOM_ADJUST_BIT_8) << 1;
        if (val == origin_val)
                return 0;

        /* turn off power rails */
        ret = pmic_clrsetbits(pmic, SY7636A_REG_OPERATION_MODE_CRL,
                              SY7636A_OPERATION_MODE_CRL_ONOFF, 0);
        if (ret < 0)
                return ret;

	ret = pmic_reg_write(pmic, SY7636A_REG_VCOM_ADJUST_CTRL_L, val & 0xFF);
	if (ret < 0)
		return ret;

	ret = pmic_reg_write(pmic, SY7636A_REG_VCOM_ADJUST_CTRL_H, (val >> 1) & 0x80);
	if (ret < 0)
		return ret;

        /* SY7636A has 50ms protection mask for power rails */
        udelay(50 * 1000);

        /* turn on power rails */
        ret = pmic_clrsetbits(pmic, SY7636A_REG_OPERATION_MODE_CRL, 0,
                              SY7636A_OPERATION_MODE_CRL_ONOFF);

        return ret;
}

static int sy7636a_regulator_probe(struct udevice *dev)
{
        struct udevice *pmic = dev_get_parent(dev);
        int ret;

        ret = pmic_reg_write(pmic, SY7636A_REG_POWER_ON_DELAY_TIME, 0x0);
	if (ret) {
		dev_err(dev, "Failed to initialize regulator: %d\n", ret);
		return ret;
	}

        return 0;
}

static const struct dm_regulator_ops sy7636a_ops = {
        .get_enable = sy7636a_get_enable,
        .set_enable = sy7636a_set_enable,
        .set_value  = sy7636a_set_value,
};

U_BOOT_DRIVER(sy7636a_regulator) = {
	.name = SY7636A_REGULATOR_DRIVER_NAME,
	.id = UCLASS_REGULATOR,
	.ops = &sy7636a_ops,
	.probe = sy7636a_regulator_probe,
};
