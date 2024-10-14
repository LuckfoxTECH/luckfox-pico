// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * serdes-pinctrl.c  -- serdes pin control driver.
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "core.h"

static const struct pinconf_param serdes_conf_params[] = {
	{ "bias-disable", PIN_CONFIG_BIAS_DISABLE, 0 },
	{ "bias-high-impedance", PIN_CONFIG_BIAS_HIGH_IMPEDANCE, 0 },
	{ "bias-bus-hold", PIN_CONFIG_BIAS_BUS_HOLD, 0 },
	{ "bias-pull-up", PIN_CONFIG_BIAS_PULL_UP, 1 },
	{ "bias-pull-down", PIN_CONFIG_BIAS_PULL_DOWN, 1 },
	{ "bias-pull-pin-default", PIN_CONFIG_BIAS_PULL_PIN_DEFAULT, 1 },
	{ "drive-open-drain", PIN_CONFIG_DRIVE_OPEN_DRAIN, 0 },
	{ "drive-open-source", PIN_CONFIG_DRIVE_OPEN_SOURCE, 0 },
	{ "drive-strength", PIN_CONFIG_DRIVE_STRENGTH, 0 },
	{ "input-enable", PIN_CONFIG_INPUT_ENABLE, 1 },
	{ "input-disable", PIN_CONFIG_INPUT_ENABLE, 0 },
};

static int serdes_pinctrl_get_pins_count(struct udevice *dev)
{
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_info->num_pins)
		return serdes->chip_data->pinctrl_info->num_pins;
	else
		return 0;
}

static const char *serdes_pinctrl_get_pin_name(struct udevice *dev,
					       unsigned int selector)
{
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_info->pins[selector].name)
		return serdes->chip_data->pinctrl_info->pins[selector].name;
	else
		return "unknow-pin";
}

static int serdes_pinctrl_get_groups_count(struct udevice *dev)
{
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_info->num_groups)
		return serdes->chip_data->pinctrl_info->num_groups;
	else
		return 0;
}

static const char *serdes_pinctrl_get_group_name(struct udevice *dev,
						 unsigned int selector)
{
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_info->groups[selector].name)
		return serdes->chip_data->pinctrl_info->groups[selector].name;
	else
		return "unknow-group";
}

static int serdes_pinctrl_get_functions_count(struct udevice *dev)
{
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_info->num_functions)
		return serdes->chip_data->pinctrl_info->num_functions;
	else
		return 0;
}

static const char *serdes_pinctrl_get_function_name(struct udevice *dev,
						    unsigned int selector)
{
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_info->functions[selector].name)
		return serdes->chip_data->pinctrl_info->functions[selector].name;
	else
		return "unknow-function";
}

static int serdes_pinmux_set_mux(struct udevice *dev,
				 unsigned int pin_selector,
				 unsigned int func_selector)
{
	struct serdes *serdes = dev_get_priv(dev->parent);
	int ret = 0;

	if (serdes->chip_data->pinctrl_ops->pinmux_set)
		ret = serdes->chip_data->pinctrl_ops->pinmux_set(serdes,
								 pin_selector, func_selector);

	SERDES_DBG_MFD("%s: %s pin=%d,func=%d\n", __func__,
		       serdes->chip_data->name, pin_selector, func_selector);
	return ret;
}

static int serdes_pinmux_group_set_mux(struct udevice *dev,
				       unsigned int group_selector,
				       unsigned int func_selector)
{
	struct serdes *serdes = dev_get_priv(dev->parent);
	int ret = 0;

	if (serdes->chip_data->pinctrl_ops->pinmux_group_set)
		ret = serdes->chip_data->pinctrl_ops->pinmux_group_set(serdes,
								       group_selector, func_selector);

	SERDES_DBG_MFD("%s: %s function=%d,group=%d\n", __func__,
		       serdes->chip_data->name, func_selector, group_selector);
	return ret;
}

static int serdes_pinconf_set(struct udevice *dev, unsigned int pin_selector,
			      unsigned int param, unsigned int argument)
{
	int ret = 0;
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_ops->pinconf_set)
		ret = serdes->chip_data->pinctrl_ops->pinconf_set(serdes,
								  pin_selector, param, argument);

	SERDES_DBG_MFD("serdes: pin = %d (%s), param = %d, arg = %d\n",
		       pin_selector,
		       serdes_pinctrl_get_pin_name(dev, pin_selector),
		       param, argument);

	return ret;
}

static int serdes_pinconf_group_set(struct udevice *dev,
				    unsigned int group_selector,
				    unsigned int param, unsigned int argument)
{
	int ret = 0;
	struct serdes *serdes = dev_get_priv(dev->parent);

	if (serdes->chip_data->pinctrl_ops->pinconf_group_set)
		ret = serdes->chip_data->pinctrl_ops->pinconf_group_set(serdes,
									group_selector, param, argument);

	SERDES_DBG_MFD("serdes: group = %d (%s), param = %d, arg = %d\n",
		       group_selector,
		       serdes_pinctrl_get_group_name(dev, group_selector),
		       param, argument);
	return ret;
}

static struct pinctrl_ops serdes_pinctrl_ops = {
	.get_pins_count = serdes_pinctrl_get_pins_count,
	.get_pin_name = serdes_pinctrl_get_pin_name,
	.get_groups_count = serdes_pinctrl_get_groups_count,
	.get_group_name = serdes_pinctrl_get_group_name,
	.get_functions_count = serdes_pinctrl_get_functions_count,
	.get_function_name = serdes_pinctrl_get_function_name,
	.set_state = pinctrl_generic_set_state,
	.pinmux_set = serdes_pinmux_set_mux,
	.pinmux_group_set = serdes_pinmux_group_set_mux,
	.pinconf_num_params = ARRAY_SIZE(serdes_conf_params),
	.pinconf_params = serdes_conf_params,
	.pinconf_set = serdes_pinconf_set,
	.pinconf_group_set = serdes_pinconf_group_set,
};

int serdes_pinctrl_probe(struct udevice *dev)
{
	struct serdes *serdes = dev_get_priv(dev->parent);
	struct serdes_pinctrl *serdes_pinctrl = serdes->serdes_pinctrl;
	int ret = 0;

	SERDES_DBG_MFD("%s: %s %s start\n", __func__, dev->name,
		       serdes->chip_data->name);

	serdes_pinctrl->parent = serdes;

	ret = serdes_gpio_register(dev, serdes);
	if (ret)
		return ret;

	return 0;
}

static const struct udevice_id serdes_of_match[] = {
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_ROHM_BU18TL82)
	{ .compatible = "rohm,bu18tl82-pinctrl" },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_ROHM_BU18RL82)
	{ .compatible = "rohm,bu18rl82-pinctrl" },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_MAXIM_MAX96745)
	{ .compatible = "maxim,max96745-pinctrl", },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_MAXIM_MAX96755)
	{ .compatible = "maxim,max96755-pinctrl" },
#endif
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_ROCKCHIP_RKX111)
	{ .compatible = "rockchip,rkx111-pinctrl" },
#endif
	{ }
};

U_BOOT_DRIVER(serdes_pinctrl) = {
	.name = "serdes-pinctrl",
	.id = UCLASS_PINCTRL,
	.of_match = serdes_of_match,
	.probe = serdes_pinctrl_probe,
	.ops	= &serdes_pinctrl_ops,
	.priv_auto_alloc_size = sizeof(struct serdes_pinctrl),
};
