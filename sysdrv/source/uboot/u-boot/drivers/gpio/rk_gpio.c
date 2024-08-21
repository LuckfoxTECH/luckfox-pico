/*
 * (C) Copyright 2015 Google, Inc
 *
 * (C) Copyright 2008-2020 Rockchip Electronics
 * Peter, Software Engineering, <superpeter.cai@gmail.com>.
 * Jianqun Xu, Software Engineering, <jay.xu@rock-chips.com>.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <dm/of_access.h>
#include <syscon.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <dm/pinctrl.h>
#include <dt-bindings/clock/rk3288-cru.h>

#include "../pinctrl/rockchip/pinctrl-rockchip.h"

#define OFFSET_TO_BIT(bit)	(1UL << (bit))

#ifdef CONFIG_ROCKCHIP_GPIO_V2
#define REG_L(R)	(R##_l)
#define REG_H(R)	(R##_h)
#define READ_REG(REG)	((readl(REG_L(REG)) & 0xFFFF) | \
			((readl(REG_H(REG)) & 0xFFFF) << 16))
#define WRITE_REG(REG, VAL)	\
{\
	writel(((VAL) & 0xFFFF) | 0xFFFF0000, REG_L(REG)); \
	writel((((VAL) & 0xFFFF0000) >> 16) | 0xFFFF0000, REG_H(REG));\
}
#define CLRBITS_LE32(REG, MASK)	WRITE_REG(REG, READ_REG(REG) & ~(MASK))
#define SETBITS_LE32(REG, MASK)	WRITE_REG(REG, READ_REG(REG) | (MASK))
#define CLRSETBITS_LE32(REG, MASK, VAL)	WRITE_REG(REG, \
				(READ_REG(REG) & ~(MASK)) | (VAL))

#else
#define READ_REG(REG)			readl(REG)
#define WRITE_REG(REG, VAL)		writel(VAL, REG)
#define CLRBITS_LE32(REG, MASK)		clrbits_le32(REG, MASK)
#define SETBITS_LE32(REG, MASK)		setbits_le32(REG, MASK)
#define CLRSETBITS_LE32(REG, MASK, VAL)	clrsetbits_le32(REG, MASK, VAL)
#endif


struct rockchip_gpio_priv {
	struct rockchip_gpio_regs *regs;
	struct udevice *pinctrl;
	int bank;
	char name[2];
};

static int rockchip_gpio_direction_input(struct udevice *dev, unsigned offset)
{
	struct rockchip_gpio_priv *priv = dev_get_priv(dev);
	struct rockchip_gpio_regs *regs = priv->regs;

	CLRBITS_LE32(&regs->swport_ddr, OFFSET_TO_BIT(offset));

	return 0;
}

static int rockchip_gpio_direction_output(struct udevice *dev, unsigned offset,
					  int value)
{
	struct rockchip_gpio_priv *priv = dev_get_priv(dev);
	struct rockchip_gpio_regs *regs = priv->regs;
	int mask = OFFSET_TO_BIT(offset);

	CLRSETBITS_LE32(&regs->swport_dr, mask, value ? mask : 0);
	SETBITS_LE32(&regs->swport_ddr, mask);

	return 0;
}

static int rockchip_gpio_get_value(struct udevice *dev, unsigned offset)
{
	struct rockchip_gpio_priv *priv = dev_get_priv(dev);
	struct rockchip_gpio_regs *regs = priv->regs;

	return readl(&regs->ext_port) & OFFSET_TO_BIT(offset) ? 1 : 0;
}

static int rockchip_gpio_set_value(struct udevice *dev, unsigned offset,
				   int value)
{
	struct rockchip_gpio_priv *priv = dev_get_priv(dev);
	struct rockchip_gpio_regs *regs = priv->regs;
	int mask = OFFSET_TO_BIT(offset);

	CLRSETBITS_LE32(&regs->swport_dr, mask, value ? mask : 0);

	return 0;
}

static int rockchip_gpio_get_function(struct udevice *dev, unsigned offset)
{
#ifdef CONFIG_SPL_BUILD
	return -ENODATA;
#else
	struct rockchip_gpio_priv *priv = dev_get_priv(dev);
	struct rockchip_gpio_regs *regs = priv->regs;
	bool is_output;
	int ret;

	ret = pinctrl_get_gpio_mux(priv->pinctrl, priv->bank, offset);
	if (ret < 0) {
		dev_err(dev, "fail to get gpio mux %d\n", ret);
		return ret;
	}

	/* If it's not 0, then it is not a GPIO */
	if (ret > 0)
		return GPIOF_FUNC;

	is_output = READ_REG(&regs->swport_ddr) & OFFSET_TO_BIT(offset);

	return is_output ? GPIOF_OUTPUT : GPIOF_INPUT;
#endif
}

static int rockchip_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	struct rockchip_gpio_priv *priv = dev_get_priv(dev);
	struct rockchip_pinctrl_priv *pctrl_priv;
	struct rockchip_pin_bank *bank;
	char *end = NULL;
	int id = -1, ret;

	priv->regs = dev_read_addr_ptr(dev);
	ret = uclass_get_device_by_seq(UCLASS_PINCTRL, 0, &priv->pinctrl);
	if (ret) {
		ret = uclass_first_device_err(UCLASS_PINCTRL, &priv->pinctrl);
		if (ret) {
			dev_err(dev, "failed to get pinctrl device %d\n", ret);
			return ret;
		}
	}

	pctrl_priv = dev_get_priv(priv->pinctrl);
	if (!pctrl_priv) {
		dev_err(dev, "failed to get pinctrl priv\n");
		return -EINVAL;
	}

	end = strrchr(dev->name, '@');
	if (end)
		id = trailing_strtoln(dev->name, end);
	if (id < 0)
		dev_read_alias_seq(dev, &id);

	if (id < 0 || id >= pctrl_priv->ctrl->nr_banks) {
		dev_err(dev, "nr_banks=%d, bank id=%d invalid\n",
			pctrl_priv->ctrl->nr_banks, id);
		return -EINVAL;
	}

	bank = &pctrl_priv->ctrl->pin_banks[id];
	if (bank->bank_num != id) {
		dev_err(dev, "bank id mismatch with pinctrl\n");
		return -EINVAL;
	}

	priv->bank = bank->bank_num;
	uc_priv->gpio_count = bank->nr_pins;
	uc_priv->gpio_base = bank->pin_base;
	uc_priv->bank_name = bank->name;

	return 0;
}

static const struct dm_gpio_ops gpio_rockchip_ops = {
	.direction_input	= rockchip_gpio_direction_input,
	.direction_output	= rockchip_gpio_direction_output,
	.get_value		= rockchip_gpio_get_value,
	.set_value		= rockchip_gpio_set_value,
	.get_function		= rockchip_gpio_get_function,
};

static const struct udevice_id rockchip_gpio_ids[] = {
	{ .compatible = "rockchip,gpio-bank" },
	{ }
};

U_BOOT_DRIVER(gpio_rockchip) = {
	.name	= "gpio_rockchip",
	.id	= UCLASS_GPIO,
	.of_match = rockchip_gpio_ids,
	.ops	= &gpio_rockchip_ops,
	.priv_auto_alloc_size = sizeof(struct rockchip_gpio_priv),
	.probe	= rockchip_gpio_probe,
};
