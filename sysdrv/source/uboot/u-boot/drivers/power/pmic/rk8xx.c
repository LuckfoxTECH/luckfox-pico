/*
 * Copyright (C) 2015 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <irq-generic.h>
#include <power/rk8xx_pmic.h>
#include <power/pmic.h>

DECLARE_GLOBAL_DATA_PTR;

#if CONFIG_IS_ENABLED(IRQ)
/* RK805 */
static const struct virq_reg rk805_irqs[] = {
	[RK8XX_IRQ_PWRON_FALL] = {
		.mask = RK805_IRQ_PWRON_FALL_MSK,
		.reg_offset = 0,
	},
	[RK8XX_IRQ_PWRON_RISE] = {
		.mask = RK805_IRQ_PWRON_RISE_MSK,
		.reg_offset = 0,
	},
};

static struct virq_chip rk805_irq_chip = {
	.status_base		= RK805_INT_STS_REG,
	.mask_base		= RK805_INT_MSK_REG,
	.num_regs		= 1,
	.read			= pmic_reg_read,
	.write			= pmic_reg_write,
	.irqs			= rk805_irqs,
	.num_irqs		= ARRAY_SIZE(rk805_irqs),
};

/* RK806 */
static const struct virq_reg rk806_irqs[] = {
	[RK8XX_IRQ_PWRON_FALL] = {
		.mask = RK806_IRQ_PWRON_FALL_MSK,
		.reg_offset = 0,
	},
	[RK8XX_IRQ_PWRON_RISE] = {
		.mask = RK806_IRQ_PWRON_RISE_MSK,
		.reg_offset = 0,
	},
};

static struct virq_chip rk806_irq_chip = {
	.status_base		= RK806_INT_STS0,
	.mask_base		= RK806_INT_MSK0,
	.irq_reg_stride		= 2,
	.num_regs		= 2,
	.read			= pmic_reg_read,
	.write			= pmic_reg_write,
	.irqs			= rk806_irqs,
	.num_irqs		= ARRAY_SIZE(rk806_irqs),
};

/* RK808 */
static const struct virq_reg rk808_irqs[] = {
	[RK8XX_IRQ_PLUG_OUT] = {
		.mask = RK808_IRQ_PLUG_OUT_MSK,
		.reg_offset = 1,
	},
};

static struct virq_chip rk808_irq_chip = {
	.status_base		= RK808_INT_STS_REG1,
	.mask_base		= RK808_INT_MSK_REG1,
	.irq_reg_stride		= 2,
	.num_regs		= 2,
	.read			= pmic_reg_read,
	.write			= pmic_reg_write,
	.irqs			= rk808_irqs,
	.num_irqs		= ARRAY_SIZE(rk808_irqs),
};

/* RK816 */
static const struct virq_reg rk816_irqs[] = {
	[RK8XX_IRQ_PWRON_FALL] = {
		.mask = RK816_IRQ_PWRON_FALL_MSK,
		.reg_offset = 0,
	},
	[RK8XX_IRQ_PWRON_RISE] = {
		.mask = RK816_IRQ_PWRON_RISE_MSK,
		.reg_offset = 0,
	},
	[RK8XX_IRQ_PLUG_OUT] = {
		.mask = RK816_IRQ_PLUG_OUT_MSK,
		.reg_offset = 2,
	},
	[RK8XX_IRQ_CHG_OK] = {
		.mask = RK816_IRQ_CHR_OK_MSK,
		.reg_offset = 2,
	},
};

static struct virq_chip rk816_irq_chip = {
	.status_base		= RK816_INT_STS_REG1,
	.mask_base		= RK816_INT_MSK_REG1,
	.irq_unalign_reg_idx	= 1,	/* idx <= 1, stride = 3 */
	.irq_unalign_reg_stride	= 3,
	.irq_reg_stride		= 2,	/* idx > 1, stride = 2 */
	.num_regs		= 3,
	.read			= pmic_reg_read,
	.write			= pmic_reg_write,
	.irqs			= rk816_irqs,
	.num_irqs		= ARRAY_SIZE(rk816_irqs),
};

/* RK818 */
static const struct virq_reg rk818_irqs[] = {
	[RK8XX_IRQ_PLUG_OUT] = {
		.mask = RK818_IRQ_PLUG_OUT_MSK,
		.reg_offset = 1,
	},
	[RK8XX_IRQ_CHG_OK] = {
		.mask = RK818_IRQ_CHR_OK_MSK,
		.reg_offset = 1,
	},
};

static struct virq_chip rk818_irq_chip = {
	.status_base		= RK818_INT_STS_REG1,
	.mask_base		= RK818_INT_MSK_REG1,
	.irq_reg_stride		= 2,
	.num_regs		= 2,
	.read			= pmic_reg_read,
	.write			= pmic_reg_write,
	.irqs			= rk818_irqs,
	.num_irqs		= ARRAY_SIZE(rk818_irqs),
};

/* RK817/RK809 */
static const struct virq_reg rk817_irqs[] = {
	[RK8XX_IRQ_PWRON_FALL] = {
		.mask = RK817_IRQ_PWRON_FALL_MSK,
		.reg_offset = 0,
	},
	[RK8XX_IRQ_PWRON_RISE] = {
		.mask = RK817_IRQ_PWRON_RISE_MSK,
		.reg_offset = 0,
	},
	[RK8XX_IRQ_PLUG_OUT] = {
		.mask = RK817_IRQ_PLUG_OUT_MSK,
		.reg_offset = 1,
	},
	[RK8XX_IRQ_PLUG_IN] = {
		.mask = RK817_IRQ_PLUG_IN_MSK,
		.reg_offset = 1,
	},
};

static struct virq_chip rk817_irq_chip = {
	.status_base		= RK817_INT_STS_REG0,
	.mask_base		= RK817_INT_MSK_REG0,
	.irq_reg_stride		= 2,
	.num_regs		= 3,
	.read			= pmic_reg_read,
	.write			= pmic_reg_write,
	.irqs			= rk817_irqs,
	.num_irqs		= ARRAY_SIZE(rk817_irqs),
};
#endif

static struct reg_data rk817_init_reg[] = {
/* enable the under-voltage protection,
 * the under-voltage protection will shutdown the LDO3 and reset the PMIC
 */
	{ RK817_BUCK4_CMIN, 0x6b, 0x6e},
	{ RK817_PMIC_SYS_CFG1, 0x20, 0x70},
	/* Set pmic_sleep as none function */
	{ RK817_PMIC_SYS_CFG3, 0x00, 0x18 },
	/* GATE pin function: gate function */
	{ RK817_GPIO_INT_CFG, 0x00, 0x20 },
#ifdef CONFIG_DM_CHARGE_DISPLAY
	/* Set pmic_int active low */
	{ RK817_GPIO_INT_CFG,  0x00, 0x02 },
#endif
};

static struct reg_data rk818_init_current[] = {
	{ REG_USB_CTRL, 0x07, 0x0f}, /* 2A */
};

/*
 * Order WARNING: Must put "LDO" after the "NLDO" and "PLDO" !
 *
 * See: pmic_bind_children()
 *	    if (!strstr(node_name, info->prefix)) {
 *		    ......
 *	    }
 *
 * Without this order, the prefix "LDO" will be matched if a regulator
 * dts node name contains "NLDO" or "PLDO".
 */
static const struct pmic_child_info pmic_children_info[] = {
	{ .prefix = "DCDC", .driver = "rk8xx_buck"},
	{ .prefix = "NLDO", .driver = "rk8xx_ldo"},
	{ .prefix = "PLDO", .driver = "rk8xx_pldo"},
	{ .prefix = "LDO", .driver = "rk8xx_ldo"},
	{ .prefix = "SWITCH", .driver = "rk8xx_switch"},
	{ },
};

static const struct pmic_child_info power_key_info[] = {
	{ .prefix = "pwrkey", .driver = "rk8xx_pwrkey"},
	{ },
};

static const struct pmic_child_info rtc_info[] = {
	{ .prefix = "rtc", .driver = "rk8xx_rtc"},
	{ },
};

static const struct pmic_child_info fuel_gauge_info[] = {
	{ .addr = "1c", .prefix = "battery", .driver = "rk818_fg"},
	{ .addr = "20", .prefix = "battery", .driver = "rk817_fg"},
	{ .addr = "1a", .prefix = "battery", .driver = "rk816_fg"},
	{ },
};

static const struct pmic_child_info rk817_codec_info[] = {
	{ .prefix = "codec", .driver = "rk817_codec"},
	{ },
};

static int rk8xx_reg_count(struct udevice *dev)
{
	struct rk8xx_priv *priv = dev_get_priv(dev);

	switch (priv->variant) {
	case RK809_ID:
	case RK817_ID:
		return RK817_NUM_OF_REGS;
	default:
		return RK808_NUM_OF_REGS;
	}
}

static int rk8xx_write(struct udevice *dev, uint reg, const uint8_t *buff,
			  int len)
{
	int ret;

	ret = dm_i2c_write(dev, reg, buff, len);
	if (ret) {
		printf("%s: write reg 0x%02x failed, ret=%d\n", __func__, reg, ret);
		return ret;
	}

	return 0;
}

static int rk8xx_read(struct udevice *dev, uint reg, uint8_t *buff, int len)
{
	int ret;

	ret = dm_i2c_read(dev, reg, buff, len);
	if (ret) {
		printf("%s: read reg 0x%02x failed, ret=%d\n", __func__, reg, ret);
		return ret;
	}

	return 0;
}

static int rk8xx_suspend(struct udevice *dev)
{
	struct rk8xx_priv *priv = dev_get_priv(dev);
	int ret = 0;
	u8 i, val;

	switch (priv->variant) {
	case RK806_ID:
		ret = rk8xx_read(dev, RK806_PWRCTRL_CONFIG0, &val, 1);
		if (ret)
			return ret;
		val &= RK806_PWRCTRL_FUN_MSK;
		ret = rk8xx_write(dev, RK806_PWRCTRL_CONFIG0, &val, 1);
		if (ret)
			return ret;

		ret = rk8xx_read(dev, RK806_PWRCTRL_CONFIG1, &val, 1);
		if (ret)
			return ret;

		val &= RK806_PWRCTRL_FUN_MSK;
		ret = rk8xx_write(dev, RK806_PWRCTRL_CONFIG1, &val, 1);
		if (ret)
			return ret;

		for (i = RK806_VSEL_CTR_SEL0; i <= RK806_DVS_CTR_SEL4; i++) {
			ret = rk8xx_read(dev, i, &val, 1);
			if (ret)
				return ret;
			val &= RK806_VSEL_CTRL_MSK;
			ret = rk8xx_write(dev, i, &val, 1);
			if (ret)
				return ret;
		}

		ret = rk8xx_read(dev, RK806_PWRCTRL_CONFIG0, &val, 1);
		if (ret)
			return ret;
		val &= RK806_PWRCTRL_FUN_MSK;
		val |= RK806_ENABLE_PWRCTRL;
		ret = rk8xx_write(dev, RK806_PWRCTRL_CONFIG0, &val, 1);
		if (ret)
			return ret;

		for (i = RK806_VSEL_CTR_SEL0; i <= RK806_DVS_CTR_SEL4; i++) {
			ret = rk8xx_read(dev, i, &val, 1);
			if (ret)
				return ret;
			val &= RK806_VSEL_CTRL_MSK;
			val |= RK806_VSEL_PWRCTRL1;
			ret = rk8xx_write(dev, i, &val, 1);
			if (ret)
				return ret;
		}
		break;
	case RK809_ID:
	case RK817_ID:
		/* pmic_sleep active high */
		ret = rk8xx_read(dev, RK817_PMIC_SYS_CFG3, &val, 1);
		if (ret)
			return ret;
		priv->sleep_pin = val;
		val &= ~0x38;
		val |= 0x28;
		ret = rk8xx_write(dev, RK817_PMIC_SYS_CFG3, &val, 1);
		break;
	default:
		return 0;
	}

	return ret;
}

static int rk8xx_resume(struct udevice *dev)
{
	struct rk8xx_priv *priv = dev_get_priv(dev);
	int ret = 0;
	u8 i, val;

	switch (priv->variant) {
	case RK806_ID:
		for (i = RK806_VSEL_CTR_SEL0; i <= RK806_DVS_CTR_SEL4; i++) {
			ret = rk8xx_read(dev, i, &val, 1);
			if (ret)
				return ret;
			val &= RK806_VSEL_CTRL_MSK;
			ret = rk8xx_write(dev, i, &val, 1);
			if (ret)
				return ret;
		}

		ret = rk8xx_read(dev, RK806_PWRCTRL_CONFIG0, &val, 1);
		if (ret)
			return ret;
		val &= RK806_PWRCTRL_FUN_MSK;
		ret = rk8xx_write(dev, RK806_PWRCTRL_CONFIG0, &val, 1);
		if (ret)
			return ret;

		ret = rk8xx_read(dev, RK806_PWRCTRL_CONFIG1, &val, 1);
		if (ret)
			return ret;
		val &= RK806_PWRCTRL_FUN_MSK;
		ret = rk8xx_write(dev, RK806_PWRCTRL_CONFIG1, &val, 1);
		break;
	case RK809_ID:
	case RK817_ID:
		ret = rk8xx_write(dev, RK817_PMIC_SYS_CFG3, &priv->sleep_pin, 1);
		break;
	default:
		return 0;
	}

	return ret;
}

static int rk8xx_shutdown(struct udevice *dev)
{
	struct rk8xx_priv *priv = dev_get_priv(dev);
	u8 val, dev_off, devctrl_reg;
	int ret = 0;

	switch (priv->variant) {
	case RK806_ID:
		devctrl_reg = RK806_SYS_CFG3;
		dev_off = RK806_DEV_OFF;
		break;
	case RK808_ID:
		devctrl_reg = REG_DEVCTRL;
		dev_off = BIT(3);
		break;
	case RK805_ID:
	case RK816_ID:
	case RK818_ID:
		devctrl_reg = REG_DEVCTRL;
		dev_off = BIT(0);
		break;
	case RK809_ID:
	case RK817_ID:
		devctrl_reg = RK817_REG_SYS_CFG3;
		dev_off = BIT(0);
		break;
	default:
		printf("Unknown PMIC: RK%x\n", priv->variant);
		return -EINVAL;
	}

	ret = rk8xx_read(dev, devctrl_reg, &val, 1);
	if (ret)
		return ret;

	val |= dev_off;
	ret = rk8xx_write(dev, devctrl_reg, &val, 1);
	if (ret)
		return ret;

	return 0;
}

#if CONFIG_IS_ENABLED(PMIC_CHILDREN)
static int rk8xx_bind(struct udevice *dev)
{
	ofnode regulators_node;
	int children;

	regulators_node = dev_read_subnode(dev, "regulators");
	if (!ofnode_valid(regulators_node)) {
		debug("%s: %s regulators subnode not found!\n", __func__,
		      dev->name);
		return -ENXIO;
	}

	debug("%s: '%s' - found regulators subnode\n", __func__, dev->name);

	children = pmic_bind_children(dev, regulators_node, pmic_children_info);
	if (!children)
		debug("%s: %s - no child found\n", __func__, dev->name);

	children = pmic_bind_children(dev, dev->node, power_key_info);
	if (!children)
		debug("%s: %s - no child found\n", __func__, dev->name);

	children = pmic_bind_children(dev, dev->node, rtc_info);
	if (!children)
		debug("%s: %s - no child found\n", __func__, dev->name);

	children = pmic_bind_children(dev, dev->node, fuel_gauge_info);
	if (!children)
		debug("%s: %s - no child found\n", __func__, dev->name);

	children = pmic_bind_children(dev, dev->node, rk817_codec_info);
	if (!children)
		debug("%s: %s - no child found\n", __func__, dev->name);

	/* Always return success for this device */
	return 0;
}
#endif

#if CONFIG_IS_ENABLED(IRQ)
/*
 * When system suspend during U-Boot charge, make sure the plugout event
 * be able to wakeup cpu in wfi/wfe state.
 */
#ifdef CONFIG_DM_CHARGE_DISPLAY
static void rk8xx_plug_out_handler(int irq, void *data)
{
	printf("Plug out interrupt\n");
}
#endif

static int rk8xx_irq_chip_init(struct udevice *dev)
{
	struct rk8xx_priv *priv = dev_get_priv(dev);
	struct virq_chip *irq_chip = NULL;
	__maybe_unused int irq_plugout = 1;
	uint8_t value;
	int ret;

	switch (priv->variant) {
	case RK806_ID:
		irq_chip = &rk806_irq_chip;
		irq_plugout = 0;
		ret = rk8xx_read(dev, RK806_GPIO_INT_CONFIG, &value, 1);
		if (ret)
			return ret;
		/* set INT polarity active low */
		value &= (~RK806_INT_POL_HIGH);
		ret = rk8xx_write(dev, RK806_GPIO_INT_CONFIG, &value, 1);
		if (ret)
			return ret;
		break;
	case RK808_ID:
		irq_chip = &rk808_irq_chip;
		break;
	case RK805_ID:
		irq_chip = &rk805_irq_chip;
		irq_plugout = 0;
		break;
	case RK816_ID:
		irq_chip = &rk816_irq_chip;
		break;
	case RK818_ID:
		irq_chip = &rk818_irq_chip;
		break;
	case RK809_ID:
	case RK817_ID:
		irq_chip = &rk817_irq_chip;
		break;
	default:
		return -EINVAL;
	}

	if (irq_chip) {
		ret = virq_add_chip(dev, irq_chip, priv->irq);
		if (ret) {
			printf("Failed to add irqchip(irq=%d), ret=%d\n",
			       priv->irq, ret);
			return ret;
		}

		priv->irq_chip = irq_chip;

#ifdef CONFIG_DM_CHARGE_DISPLAY
		int irq;

		if (irq_plugout) {
			irq = virq_to_irq(irq_chip, RK8XX_IRQ_PLUG_OUT);
			if (irq < 0) {
				printf("Failed to register plugout irq, ret=%d\n", irq);
				return irq;
			}
			irq_install_handler(irq, rk8xx_plug_out_handler, dev);
			irq_handler_enable_suspend_only(irq);
		}
#endif
	}

	return 0;
}
#else
static inline int rk8xx_irq_chip_init(struct udevice *dev) { return 0; }
#endif

static int rk8xx_ofdata_to_platdata(struct udevice *dev)
{
	struct rk8xx_priv *rk8xx = dev_get_priv(dev);
	u32 interrupt, phandle, val;
	int ret;

	phandle = dev_read_u32_default(dev, "interrupt-parent", -ENODATA);
	if (phandle == -ENODATA) {
		printf("Read 'interrupt-parent' failed, ret=%d\n", phandle);
		return phandle;
	}

	ret = dev_read_u32_array(dev, "interrupts", &interrupt, 1);
	if (ret) {
		printf("Read 'interrupts' failed, ret=%d\n", ret);
		return ret;
	}

#if CONFIG_IS_ENABLED(IRQ)
	rk8xx->irq = phandle_gpio_to_irq(phandle, interrupt);
	if (rk8xx->irq < 0) {
		printf("Failed to request rk8xx irq, ret=%d\n", rk8xx->irq);
		return rk8xx->irq;
	}
#endif
	val = dev_read_u32_default(dev, "long-press-off-time-sec", 0);
	if (val <= 6)
		rk8xx->lp_off_time = RK8XX_LP_TIME_6S;
	else if (val <= 8)
		rk8xx->lp_off_time = RK8XX_LP_TIME_8S;
	else if (val <= 10)
		rk8xx->lp_off_time = RK8XX_LP_TIME_10S;
	else
		rk8xx->lp_off_time = RK8XX_LP_TIME_12S;

	val = dev_read_u32_default(dev, "long-press-restart", 0);
	if (val)
		rk8xx->lp_action = RK8XX_LP_RESTART;
	else
		rk8xx->lp_action = RK8XX_LP_OFF;

	rk8xx->not_save_power_en = dev_read_u32_default(dev, "not-save-power-en", 0);
	rk8xx->sys_can_sd = dev_read_bool(dev, "vsys-off-shutdown");
	rk8xx->rst_fun = dev_read_u32_default(dev, "pmic-reset-func", 0);
	/* buck5 external feedback resister disable */
	rk8xx->buck5_feedback_dis = dev_read_bool(dev, "buck5-feedback-disable");

	return 0;
}

static int rk8xx_probe(struct udevice *dev)
{
	struct rk8xx_priv *priv = dev_get_priv(dev);
	struct reg_data *init_current = NULL;
	struct reg_data *init_data = NULL;
	int init_current_num = 0;
	int init_data_num = 0;
	int ret = 0, i, show_variant;
	uint8_t msb, lsb, id_msb, id_lsb;
	uint8_t on_source = 0, off_source = 0;
	uint8_t pwron_key = 0, lp_off_msk = 0, lp_act_msk = 0;
	uint8_t power_en0, power_en1, power_en2, power_en3;
	uint8_t on, off;
	uint8_t value;

	/* read Chip variant */
	if (device_is_compatible(dev, "rockchip,rk817") ||
	    device_is_compatible(dev, "rockchip,rk809")) {
		id_msb = RK817_ID_MSB;
		id_lsb = RK817_ID_LSB;
	} else if (device_is_compatible(dev, "rockchip,rk806")) {
		id_msb = RK806_CHIP_NAME;
		id_lsb = RK806_CHIP_VER;
	} else {
		id_msb = ID_MSB;
		id_lsb = ID_LSB;
	}

	ret = rk8xx_read(dev, id_msb, &msb, 1);
	if (ret)
		return ret;
	ret = rk8xx_read(dev, id_lsb, &lsb, 1);
	if (ret)
		return ret;

	priv->variant = ((msb << 8) | lsb) & RK8XX_ID_MSK;
	show_variant = priv->variant;
	switch (priv->variant) {
	case RK806_ID:
		on_source = RK806_ON_SOURCE;
		off_source = RK806_OFF_SOURCE;
		ret = rk8xx_read(dev, RK806_HW_VER, &value, 1);
		if (ret)
			panic("RK806: read RK806_HW_VER error!\n");

		if ((lsb & RK806_VERSION_MSK) == RK806_VERSION_AB) {
			ret = rk8xx_read(dev, RK806_SYS_CFG1, &value, 1);
			if (ret) {
				dev_err(dev, "rk806 RK806_SYS_CFG1 read error: %d\n", ret);
				return ret;
			}
			value |= RK806_ABNORDET_EN;
			rk8xx_write(dev, RK806_SYS_CFG1, &value, 1);
		}

		if (priv->rst_fun) {
			rk8xx_read(dev, RK806_SYS_CFG3, &value, 1);
			value &= RK806_RESET_FUN_CLR;
			if (priv->rst_fun == RK806_RST_MODE1) {
				value |= (RK806_RST_MODE1 << 6);
				rk8xx_write(dev, RK806_SYS_CFG3, &value, 1);
			} else if (priv->rst_fun == RK806_RST_MODE2) {
				value |= (RK806_RST_MODE2 << 6);
				rk8xx_write(dev, RK806_SYS_CFG3, &value, 1);
			}
		}

		if (priv->buck5_feedback_dis) {
			rk8xx_read(dev, RK806_BUCK_RSERVE_REG3, &value, 1);
			value &=( ~RK806_BUCK5_EX_RES_EN);
			rk8xx_write(dev, RK806_BUCK_RSERVE_REG3, &value, 1);
		}
		break;
	case RK808_ID:
		show_variant = 0x808;	/* RK808 hardware ID is 0 */
		pwron_key = RK8XX_DEVCTRL_REG;
		lp_off_msk = RK8XX_LP_OFF_MSK;
		break;
	case RK805_ID:
	case RK816_ID:
		on_source = RK8XX_ON_SOURCE;
		off_source = RK8XX_OFF_SOURCE;
		pwron_key = RK8XX_DEVCTRL_REG;
		lp_off_msk = RK8XX_LP_OFF_MSK;
		lp_act_msk = RK8XX_LP_ACTION_MSK;
		break;
	case RK818_ID:
		on_source = RK8XX_ON_SOURCE;
		off_source = RK8XX_OFF_SOURCE;
		pwron_key = RK8XX_DEVCTRL_REG;
		lp_off_msk = RK8XX_LP_OFF_MSK;
		lp_act_msk = RK8XX_LP_ACTION_MSK;
		/* set current if no fuel gauge */
		if (!ofnode_valid(dev_read_subnode(dev, "battery"))) {
			init_current = rk818_init_current;
			init_current_num = ARRAY_SIZE(rk818_init_current);
		}
		break;
	case RK809_ID:
	case RK817_ID:
		if (device_is_compatible(dev, "rockchip,rk809") && (priv->variant != RK809_ID)) {
			dev_err(dev, "the dts is RK809, the hardware is RK817\n");
			run_command("download", 0);
		}

		if (device_is_compatible(dev, "rockchip,rk817") && (priv->variant != RK817_ID)) {
			dev_err(dev, "the dts is RK817, the hardware is RK809\n");
			run_command("download", 0);
		}

		on_source = RK817_ON_SOURCE;
		off_source = RK817_OFF_SOURCE;
		pwron_key = RK817_PWRON_KEY;
		lp_off_msk = RK8XX_LP_OFF_MSK;
		lp_act_msk = RK8XX_LP_ACTION_MSK;
		init_data = rk817_init_reg;
		init_data_num = ARRAY_SIZE(rk817_init_reg);

		/* whether the system voltage can be shutdown in PWR_off mode */
		if (priv->sys_can_sd) {
			ret = rk8xx_read(dev, RK817_PMIC_CHRG_TERM, &value, 1);
			if (ret)
				return ret;
			value |= 0x80;
			ret = rk8xx_write(dev, RK817_PMIC_CHRG_TERM, &value, 1);
			if (ret)
				return ret;
		} else {
			ret = rk8xx_read(dev, RK817_PMIC_CHRG_TERM, &value, 1);
			if (ret)
				return ret;
			value &= 0x7f;
			ret = rk8xx_write(dev, RK817_PMIC_CHRG_TERM, &value, 1);
			if (ret)
				return ret;
		}

		/* judge whether save the PMIC_POWER_EN register */
		if (!priv->not_save_power_en) {
			ret = rk8xx_read(dev, RK817_POWER_EN0, &power_en0, 1);
			if (ret)
				return ret;
			ret = rk8xx_read(dev, RK817_POWER_EN1, &power_en1, 1);
			if (ret)
				return ret;
			ret = rk8xx_read(dev, RK817_POWER_EN2, &power_en2, 1);
			if (ret)
				return ret;
			ret = rk8xx_read(dev, RK817_POWER_EN3, &power_en3, 1);
			if (ret)
				return ret;

			value = (power_en0 & 0x0f) | ((power_en1 & 0x0f) << 4);
			rk8xx_write(dev, RK817_POWER_EN_SAVE0, &value, 1);
			value = (power_en2 & 0x0f) | ((power_en3 & 0x0f) << 4);
			rk8xx_write(dev, RK817_POWER_EN_SAVE1, &value, 1);
		}
		break;
	default:
		printf("Unknown PMIC: RK%x!!\n", priv->variant);
		return -EINVAL;
	}

	/* common init */
	for (i = 0; i < init_data_num; i++) {
		ret = pmic_clrsetbits(dev,
				      init_data[i].reg,
				      init_data[i].mask,
				      init_data[i].val);
		if (ret < 0) {
			printf("%s: i2c set reg 0x%x failed, ret=%d\n",
			       __func__, init_data[i].reg, ret);
		}
	}

	/* current init */
	for (i = 0; i < init_current_num; i++) {
		ret = pmic_clrsetbits(dev,
				      init_current[i].reg,
				      init_current[i].mask,
				      init_current[i].val);
		if (ret < 0) {
			printf("%s: i2c set reg 0x%x failed, ret=%d\n",
			       __func__, init_current[i].reg, ret);
		}
	}

	printf("PMIC:  RK%x ", show_variant);

	if (on_source && off_source) {
		rk8xx_read(dev, on_source, &on, 1);
		rk8xx_read(dev, off_source, &off, 1);
		printf("(on=0x%02x, off=0x%02x)", on, off);
	}
	printf("\n");

	if (pwron_key) {
		ret = rk8xx_read(dev, pwron_key, &value, 1);
		if (ret)
			return ret;
		value &= ~(lp_off_msk | lp_act_msk);
		if (lp_off_msk)
			value |= priv->lp_off_time;
		if (lp_act_msk)
			value |= priv->lp_action;
		rk8xx_write(dev, pwron_key, &value, 1);
	}

	ret = rk8xx_irq_chip_init(dev);
	if (ret) {
		printf("IRQ chip initial failed\n");
		return ret;
	}

	return 0;
}

static struct dm_pmic_ops rk8xx_ops = {
	.reg_count = rk8xx_reg_count,
	.read = rk8xx_read,
	.write = rk8xx_write,
	.suspend = rk8xx_suspend,
	.resume = rk8xx_resume,
	.shutdown = rk8xx_shutdown,
};

static const struct udevice_id rk8xx_ids[] = {
	{ .compatible = "rockchip,rk805" },
	{ .compatible = "rockchip,rk806" },
	{ .compatible = "rockchip,rk808" },
	{ .compatible = "rockchip,rk809" },
	{ .compatible = "rockchip,rk816" },
	{ .compatible = "rockchip,rk817" },
	{ .compatible = "rockchip,rk818" },
	{ }
};

U_BOOT_DRIVER(pmic_rk8xx) = {
	.name = "rk8xx pmic",
	.id = UCLASS_PMIC,
	.of_match = rk8xx_ids,
#if CONFIG_IS_ENABLED(PMIC_CHILDREN)
	.bind = rk8xx_bind,
#endif
	.ofdata_to_platdata = rk8xx_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct rk8xx_priv),
	.probe = rk8xx_probe,
	.ops = &rk8xx_ops,
};
