// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd.
 */

#include <common.h>
#include <asm/gpio.h>
#include <dm.h>
#include <dm/lists.h>
#include <dm/device-internal.h>
#include <dm/of_access.h>
#include <dm/pinctrl.h>
#include <i2c.h>
#include <power/pmic.h>
#include <power/regulator.h>
#include <power/sy7636a.h>

DECLARE_GLOBAL_DATA_PTR;

static const struct pmic_child_info pmic_children_info[] = {
	{ .prefix = "vcom", .driver = SY7636A_REGULATOR_DRIVER_NAME },
	{ },
};

static const struct pmic_child_info thermal_child_info[] = {
	{ .prefix = "sy7636a_thermal", .driver = SY7636A_THERMAL_COMTATIBLE_NAME },
	{ },
};

static int sy7636a_reg_count(struct udevice *dev)
{
        return SY7636A_REG_MAX;
}

static int sy7636a_write(struct udevice *dev, uint reg, const uint8_t *buff, int len)
{
	int ret;

	ret = dm_i2c_write(dev, reg, buff, len);
	if (ret) {
		pr_err("sy7636a failed to write register: %#x, ret:%d\n", reg, ret);
		return ret;
	}

	return 0;
}

static int sy7636a_read(struct udevice *dev, uint reg, uint8_t *buff, int len)
{
	int ret;

	ret = dm_i2c_read(dev, reg, buff, len);
	if (ret) {
		pr_err("sy7636a failed to write register: %#x, ret:%d\n", reg, ret);
		return ret;
	}

	return 0;
}

static int pmic_sy7636a_probe(struct udevice *dev)
{
	struct gpio_desc gpio_desc[8];
	uint8_t data;
	int num;
	int ret;

	num = gpio_request_list_by_name(dev, "enable-gpios", gpio_desc, 8,
					GPIOD_IS_OUT | GPIOD_IS_OUT_ACTIVE);
	if (num < 0)
		dev_warn(dev, "sy7636a failed to get enable gpios:%d\n", ret);

	/* After enable, SY7636A requires 2.5ms delay time to enter active mode */
	udelay(2500);

	/* check is device i2c present */
	ret = dm_i2c_read(dev, SY7636A_REG_OPERATION_MODE_CRL, &data, 1);
	if (ret) {
		dev_warn(dev, "sy7636a i2c not present: %d\n", ret);
		goto fail;
	}

	return 0;

fail:
	if (num > 0)
		gpio_free_list(dev, gpio_desc, num);

	return ret;
}

static int pmic_sy7636a_bind(struct udevice *dev)
{
	ofnode regulators_node;
	int children;

	regulators_node = dev_read_subnode(dev, "regulators");
	if (!ofnode_valid(regulators_node)) {
		dev_err(dev, "Regulators subnode not found!");
		return -ENXIO;
	}

	children = pmic_bind_children(dev, regulators_node, pmic_children_info);
	if (!children)
		dev_err(dev, "Failed to bind sy7636a regulator\n");

	children = pmic_bind_children(dev, dev->node, thermal_child_info);
	if (!children)
		dev_err(dev, "Failed to bind sy7636a thermal\n");

        return 0;
}

static struct dm_pmic_ops sy7636a_ops = {
	.reg_count = sy7636a_reg_count,
	.read = sy7636a_read,
	.write = sy7636a_write,
};

static const struct udevice_id pmic_sy7636a_of_match[] = {
	{ .compatible = "silergy,sy7636a" },
	{ .compatible = "silergy,sy7636a-pmic" },
	{}
};

U_BOOT_DRIVER(pmic_sy7636a) = {
	.name = "pmic_sy7636a",
	.id = UCLASS_PMIC,
	.of_match = pmic_sy7636a_of_match,
	.probe = pmic_sy7636a_probe,
	.ops = &sy7636a_ops,
	.bind = pmic_sy7636a_bind,
};
