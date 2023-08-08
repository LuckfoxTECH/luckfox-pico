// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <errno.h>
#include <i2c.h>
#include <max96752f.h>

struct config_desc {
	u16 reg;
	u8 mask;
	u8 val;
};

struct function_desc {
	const char *name;
	const char **group_names;
	int num_group_names;

	u8 gpio_out_dis:1;
	u8 gpio_tx_en:1;
	u8 gpio_rx_en:1;
	u8 oldi:1;
	u8 gpio_tx_id;
	u8 gpio_rx_id;
};

struct group_desc {
	const char *name;
	int *pins;
	int num_pins;

	const struct config_desc *configs;
	int num_configs;
};

struct pin_desc {
	unsigned number;
	const char *name;
};

static const struct pin_desc max96752f_pins[] = {
	{0, "oldi"},
	{1, "gpio1"},
	{2, "gpio2"},
	{3, "gpio3"},
	{4, "gpio4"},
	{5, "gpio5"},
	{6, "gpio6"},
	{7, "gpio7"},
	{8, "gpio8"},
	{9, "gpio9"},
	{10, "gpio10"},
	{11, "gpio11"},
	{12, "gpio12"},
	{13, "gpio13"},
	{14, "gpio14"},
	{15, "gpio15"},
};

static int oldi_pins[] = {0};
static int gpio1_pins[] = {1};
static int gpio2_pins[] = {2};
static int gpio3_pins[] = {3};
static int gpio4_pins[] = {4};
static int gpio5_pins[] = {5};
static int gpio6_pins[] = {6};
static int gpio7_pins[] = {7};
static int gpio8_pins[] = {8};
static int gpio9_pins[] = {9};
static int gpio10_pins[] = {10};
static int gpio11_pins[] = {11};
static int gpio12_pins[] = {12};
static int gpio13_pins[] = {13};
static int gpio14_pins[] = {14};
static int gpio15_pins[] = {15};

#define GROUP_DESC(nm) \
{ \
	.name = #nm, \
	.pins = nm ## _pins, \
	.num_pins = ARRAY_SIZE(nm ## _pins), \
}

#define GROUP_DESC_CONFIG(nm) \
{ \
	.name = #nm, \
	.pins = nm ## _pins, \
	.num_pins = ARRAY_SIZE(nm ## _pins), \
	.configs = nm ## _configs, \
	.num_configs = ARRAY_SIZE(nm ## _configs), \
}

static const struct config_desc gpio6_configs[] = {
	{ 0x0002, AUD_TX_EN, 0 },
};

static const struct config_desc gpio7_configs[] = {
	{ 0x0002, AUD_TX_EN, 0 },
};

static const struct config_desc gpio8_configs[] = {
	{ 0x0002, AUD_TX_EN, 0 },
};

static const struct config_desc gpio11_configs[] = {
	{ 0x0140, AUD_RX_EN, 0 },
};

static const struct config_desc gpio12_configs[] = {
	{ 0x0140, AUD_RX_EN, 0 },
};

static const struct config_desc gpio13_configs[] = {
	{ 0x0140, AUD_RX_EN, 0 },
};

static const struct group_desc max96752f_groups[] = {
	GROUP_DESC(oldi),
	GROUP_DESC(gpio1),
	GROUP_DESC(gpio2),
	GROUP_DESC(gpio3),
	GROUP_DESC(gpio4),
	GROUP_DESC(gpio5),
	GROUP_DESC_CONFIG(gpio6),
	GROUP_DESC_CONFIG(gpio7),
	GROUP_DESC_CONFIG(gpio8),
	GROUP_DESC(gpio9),
	GROUP_DESC(gpio10),
	GROUP_DESC_CONFIG(gpio11),
	GROUP_DESC_CONFIG(gpio12),
	GROUP_DESC_CONFIG(gpio13),
	GROUP_DESC(gpio14),
	GROUP_DESC(gpio15),
};

static const char *gpio_groups[] = {
	"reserved", "gpio1", "gpio2", "gpio3", "gpio4",
	"gpio5", "gpio6", "gpio7", "gpio8", "gpio9",
	"gpio10", "gpio11", "gpio12", "gpio13", "gpio14",
	"gpio15",
};

static const char *oldi_groups[] = { "oldi" };

#define FUNCTION_DESC_GPIO_RX(id) \
{ \
	.name = "GPIO_RX_"#id, \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
	.gpio_rx_en = 1, \
	.gpio_rx_id = id, \
} \

#define FUNCTION_DESC_GPIO_TX(id) \
{ \
	.name = "GPIO_TX_"#id, \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
	.gpio_out_dis = 1, \
	.gpio_tx_en = 1, \
	.gpio_tx_id = id \
} \

#define FUNCTION_DESC_GPIO() \
{ \
	.name = "GPIO", \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
} \

#define FUNCTION_DESC_OLDI() \
{ \
	.name = "OLDI", \
	.group_names = oldi_groups, \
	.num_group_names = ARRAY_SIZE(oldi_groups), \
	.oldi = 1, \
} \

static const struct function_desc max96752f_functions[] = {
	FUNCTION_DESC_GPIO_TX(0),
	FUNCTION_DESC_GPIO_TX(1),
	FUNCTION_DESC_GPIO_TX(2),
	FUNCTION_DESC_GPIO_TX(3),
	FUNCTION_DESC_GPIO_TX(4),
	FUNCTION_DESC_GPIO_TX(5),
	FUNCTION_DESC_GPIO_TX(6),
	FUNCTION_DESC_GPIO_TX(7),
	FUNCTION_DESC_GPIO_TX(8),
	FUNCTION_DESC_GPIO_TX(9),
	FUNCTION_DESC_GPIO_TX(10),
	FUNCTION_DESC_GPIO_TX(11),
	FUNCTION_DESC_GPIO_TX(12),
	FUNCTION_DESC_GPIO_TX(13),
	FUNCTION_DESC_GPIO_TX(14),
	FUNCTION_DESC_GPIO_TX(15),
	FUNCTION_DESC_GPIO_TX(16),
	FUNCTION_DESC_GPIO_TX(17),
	FUNCTION_DESC_GPIO_TX(18),
	FUNCTION_DESC_GPIO_TX(19),
	FUNCTION_DESC_GPIO_TX(20),
	FUNCTION_DESC_GPIO_TX(21),
	FUNCTION_DESC_GPIO_TX(22),
	FUNCTION_DESC_GPIO_TX(23),
	FUNCTION_DESC_GPIO_TX(24),
	FUNCTION_DESC_GPIO_TX(25),
	FUNCTION_DESC_GPIO_TX(26),
	FUNCTION_DESC_GPIO_TX(27),
	FUNCTION_DESC_GPIO_TX(28),
	FUNCTION_DESC_GPIO_TX(29),
	FUNCTION_DESC_GPIO_TX(30),
	FUNCTION_DESC_GPIO_TX(31),
	FUNCTION_DESC_GPIO_RX(0),
	FUNCTION_DESC_GPIO_RX(1),
	FUNCTION_DESC_GPIO_RX(2),
	FUNCTION_DESC_GPIO_RX(3),
	FUNCTION_DESC_GPIO_RX(4),
	FUNCTION_DESC_GPIO_RX(5),
	FUNCTION_DESC_GPIO_RX(6),
	FUNCTION_DESC_GPIO_RX(7),
	FUNCTION_DESC_GPIO_RX(8),
	FUNCTION_DESC_GPIO_RX(9),
	FUNCTION_DESC_GPIO_RX(10),
	FUNCTION_DESC_GPIO_RX(11),
	FUNCTION_DESC_GPIO_RX(12),
	FUNCTION_DESC_GPIO_RX(13),
	FUNCTION_DESC_GPIO_RX(14),
	FUNCTION_DESC_GPIO_RX(15),
	FUNCTION_DESC_GPIO_RX(16),
	FUNCTION_DESC_GPIO_RX(17),
	FUNCTION_DESC_GPIO_RX(18),
	FUNCTION_DESC_GPIO_RX(19),
	FUNCTION_DESC_GPIO_RX(20),
	FUNCTION_DESC_GPIO_RX(21),
	FUNCTION_DESC_GPIO_RX(22),
	FUNCTION_DESC_GPIO_RX(23),
	FUNCTION_DESC_GPIO_RX(24),
	FUNCTION_DESC_GPIO_RX(25),
	FUNCTION_DESC_GPIO_RX(26),
	FUNCTION_DESC_GPIO_RX(27),
	FUNCTION_DESC_GPIO_RX(28),
	FUNCTION_DESC_GPIO_RX(29),
	FUNCTION_DESC_GPIO_RX(30),
	FUNCTION_DESC_GPIO_RX(31),
	FUNCTION_DESC_GPIO(),
	FUNCTION_DESC_OLDI(),
};

static int max96752f_get_pins_count(struct udevice *dev)
{
	return ARRAY_SIZE(max96752f_pins);
}

static const char *max96752f_get_pin_name(struct udevice *dev,
					  unsigned selector)
{
	return max96752f_pins[selector].name;
}

static int max96752f_pinctrl_get_groups_count(struct udevice *dev)
{
	return ARRAY_SIZE(max96752f_groups);
}

static const char *max96752f_pinctrl_get_group_name(struct udevice *dev,
						    unsigned selector)
{
	return max96752f_groups[selector].name;
}

static int max96752f_pinctrl_get_functions_count(struct udevice *dev)
{
	return ARRAY_SIZE(max96752f_functions);
}

static const char *max96752f_pinctrl_get_function_name(struct udevice *dev,
						       unsigned selector)
{
	return max96752f_functions[selector].name;
}

static int max96752f_pinmux_set(struct udevice *dev, unsigned group_selector,
				unsigned func_selector)
{
	const struct group_desc *grp = &max96752f_groups[group_selector];
	const struct function_desc *func = &max96752f_functions[func_selector];
	int i, ret;

	if (func->oldi)
		return 0;

	for (i = 0; i < grp->num_configs; i++) {
		const struct config_desc *config = &grp->configs[i];

		ret = dm_i2c_reg_clrset(dev->parent, config->reg, config->mask,
					config->val);
		if (ret < 0)
			return ret;
	}

	for (i = 0; i < grp->num_pins; i++) {
		ret = dm_i2c_reg_clrset(dev->parent, GPIO_A_REG(grp->pins[i]),
					GPIO_OUT_DIS | GPIO_RX_EN | GPIO_TX_EN,
					FIELD_PREP(GPIO_OUT_DIS, func->gpio_out_dis) |
					FIELD_PREP(GPIO_RX_EN, func->gpio_rx_en) |
					FIELD_PREP(GPIO_TX_EN, func->gpio_tx_en));
		if (ret < 0)
			return ret;

		if (func->gpio_tx_en) {
			ret = dm_i2c_reg_clrset(dev->parent, GPIO_B_REG(grp->pins[i]),
						GPIO_TX_ID,
						FIELD_PREP(GPIO_TX_ID, func->gpio_tx_id));
			if (ret < 0)
				return ret;
		}

		if (func->gpio_rx_en) {
			ret = dm_i2c_reg_clrset(dev->parent,
						GPIO_C_REG(grp->pins[i]),
						GPIO_RX_ID,
						FIELD_PREP(GPIO_RX_ID, func->gpio_rx_id));
			if (ret < 0)
				return ret;
		}
	}

	return 0;
}

#define PIN_CONFIG_OLDI_SPL_EN		(PIN_CONFIG_END + 1)
#define PIN_CONFIG_OLDI_SWAP_AB		(PIN_CONFIG_END + 2)

static const struct pinconf_param max96752f_pinconf_params[] = {
	{ "drive-open-drain", PIN_CONFIG_DRIVE_OPEN_DRAIN, 0 },
	{ "drive-push-pull", PIN_CONFIG_DRIVE_PUSH_PULL, 0 },
	{ "bias-disable", PIN_CONFIG_BIAS_DISABLE, 0 },
	{ "bias-pull-up", PIN_CONFIG_BIAS_PULL_UP, 40000 },
	{ "bias-pull-down", PIN_CONFIG_BIAS_PULL_DOWN, 40000 },
	{ "output-low", PIN_CONFIG_OUTPUT, 0 },
	{ "output-high", PIN_CONFIG_OUTPUT, 1 },
	{ "oldi-spl-en", PIN_CONFIG_OLDI_SPL_EN, 0 },
	{ "oldi-swap-ab", PIN_CONFIG_OLDI_SWAP_AB, 0 },
};

static int max96752f_pinconf_set(struct udevice *dev, unsigned int pin,
				 unsigned int param, unsigned int arg)
{
	u8 res_cfg;
	int ret;

	switch (param) {
	case PIN_CONFIG_DRIVE_OPEN_DRAIN:
		ret = dm_i2c_reg_clrset(dev->parent, GPIO_B_REG(pin), OUT_TYPE,
					FIELD_PREP(OUT_TYPE, 0));
		if (ret < 0)
			return ret;

		break;
	case PIN_CONFIG_DRIVE_PUSH_PULL:
		ret = dm_i2c_reg_clrset(dev->parent, GPIO_B_REG(pin), OUT_TYPE,
					FIELD_PREP(OUT_TYPE, 1));
		if (ret < 0)
			return ret;

		break;
	case PIN_CONFIG_BIAS_DISABLE:
		ret = dm_i2c_reg_clrset(dev->parent, GPIO_C_REG(pin),
					PULL_UPDN_SEL,
					FIELD_PREP(PULL_UPDN_SEL, 0));
		if (ret < 0)
			return ret;

		break;
	case PIN_CONFIG_BIAS_PULL_UP:
		switch (arg) {
		case 40000:
			res_cfg = 0;
			break;
		case 1000000:
			res_cfg = 1;
			break;
		default:
			return -EINVAL;
		}

		ret = dm_i2c_reg_clrset(dev->parent, GPIO_A_REG(pin), RES_CFG,
					FIELD_PREP(RES_CFG, res_cfg));
		if (ret < 0)
			return ret;

		ret = dm_i2c_reg_clrset(dev->parent, GPIO_C_REG(pin),
					PULL_UPDN_SEL,
					FIELD_PREP(PULL_UPDN_SEL, 1));
		if (ret < 0)
			return ret;

		break;
	case PIN_CONFIG_BIAS_PULL_DOWN:
		switch (arg) {
		case 40000:
			res_cfg = 0;
			break;
		case 1000000:
			res_cfg = 1;
			break;
		default:
			return -EINVAL;
		}

		ret = dm_i2c_reg_clrset(dev->parent, GPIO_A_REG(pin), RES_CFG,
					FIELD_PREP(RES_CFG, res_cfg));
		if (ret < 0)
			return ret;

		ret = dm_i2c_reg_clrset(dev->parent, GPIO_C_REG(pin),
					PULL_UPDN_SEL,
					FIELD_PREP(PULL_UPDN_SEL, 2));
		if (ret < 0)
			return ret;

		break;
	case PIN_CONFIG_OUTPUT:
		ret = dm_i2c_reg_clrset(dev->parent, GPIO_A_REG(pin),
					GPIO_OUT_DIS | GPIO_OUT,
					FIELD_PREP(GPIO_OUT_DIS, 0) |
					FIELD_PREP(GPIO_OUT, arg));
		if (ret < 0)
			return ret;

		break;
	case PIN_CONFIG_OLDI_SPL_EN:
		if (pin > 0)
			return -EINVAL;

		ret = dm_i2c_reg_clrset(dev->parent, OLDI_REG(1),
					OLDI_SPL_EN | OLDI_SPL_POL,
					FIELD_PREP(OLDI_SPL_EN, 1) |
					FIELD_PREP(OLDI_SPL_POL, 0));
		if (ret < 0)
			return ret;

		break;
	case PIN_CONFIG_OLDI_SWAP_AB:
		if (pin > 0)
			return -EINVAL;

		ret = dm_i2c_reg_clrset(dev->parent, OLDI_REG(1), OLDI_SWAP_AB,
					FIELD_PREP(OLDI_SWAP_AB, 1));
		if (ret < 0)
			return ret;

		break;
	default:
		dev_err(dev, "unsupported configuration parameter %u\n", param);
		return -ENOTSUPP;
	}

	return 0;
}

static const struct pinctrl_ops max96752f_pinctrl_ops = {
	.get_pins_count = max96752f_get_pins_count,
	.get_pin_name = max96752f_get_pin_name,
	.get_groups_count = max96752f_pinctrl_get_groups_count,
	.get_group_name = max96752f_pinctrl_get_group_name,
	.get_functions_count = max96752f_pinctrl_get_functions_count,
	.get_function_name = max96752f_pinctrl_get_function_name,
	.set_state = pinctrl_generic_set_state,
	.pinmux_set = max96752f_pinmux_set,
	.pinmux_group_set = max96752f_pinmux_set,
	.pinconf_num_params = ARRAY_SIZE(max96752f_pinconf_params),
	.pinconf_params = max96752f_pinconf_params,
	.pinconf_set = max96752f_pinconf_set,
};

static const struct udevice_id max96752f_pinctrl_of_match[] = {
	{ .compatible = "maxim,max96752f-pinctrl" },
	{ }
};

U_BOOT_DRIVER(max96752f_pinctrl) = {
	.name = "pinctrl-max96752f",
	.id = UCLASS_PINCTRL,
	.of_match = max96752f_pinctrl_of_match,
	.ops = &max96752f_pinctrl_ops,
};
