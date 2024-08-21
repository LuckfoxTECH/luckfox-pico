// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * maxim-max96755.c  --  I2C register interface access for max96755 serdes chip
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "../core.h"
#include "maxim-max96755.h"

struct serdes_function_data {
	u8 gpio_out_dis:1;
	u8 gpio_tx_en:1;
	u8 gpio_rx_en:1;
	u8 gpio_tx_id;
	u8 gpio_rx_id;
	u16 mdelay;
};

struct config_desc {
	u16 reg;
	u8 mask;
	u8 val;
};

struct serdes_group_data {
	const struct config_desc *configs;
	int num_configs;
};

static int MAX96755_MFP0_pins[] = {0};
static int MAX96755_MFP1_pins[] = {1};
static int MAX96755_MFP2_pins[] = {2};
static int MAX96755_MFP3_pins[] = {3};
static int MAX96755_MFP4_pins[] = {4};
static int MAX96755_MFP5_pins[] = {5};
static int MAX96755_MFP6_pins[] = {6};
static int MAX96755_MFP7_pins[] = {7};

static int MAX96755_MFP8_pins[] = {8};
static int MAX96755_MFP9_pins[] = {9};
static int MAX96755_MFP10_pins[] = {10};
static int MAX96755_MFP11_pins[] = {11};
static int MAX96755_MFP12_pins[] = {12};
static int MAX96755_MFP13_pins[] = {13};
static int MAX96755_MFP14_pins[] = {14};
static int MAX96755_MFP15_pins[] = {15};

static int MAX96755_MFP16_pins[] = {16};
static int MAX96755_MFP17_pins[] = {17};
static int MAX96755_MFP18_pins[] = {18};
static int MAX96755_MFP19_pins[] = {19};
static int MAX96755_MFP20_pins[] = {20};
static int MAX96755_I2C_pins[] = {19, 20};
static int MAX96755_UART_pins[] = {19, 20};

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
	.data = (void *)(const struct serdes_group_data []) { \
		{ \
			.configs = nm ## _configs, \
			.num_configs = ARRAY_SIZE(nm ## _configs), \
		} \
	}, \
}

static const struct config_desc MAX96755_MFP0_configs[] = {
	{ 0x0005, LOCK_EN, 0 },
	{ 0x0048, LOC_MS_EN, 0 },
};

static const struct config_desc MAX96755_MFP1_configs[] = {
	{ 0x0005, ERRB_EN, 0 },
};

static const struct config_desc MAX96755_MFP4_configs[] = {
	{ 0x070, SPI_EN, 0 },
};

static const struct config_desc MAX96755_MFP5_configs[] = {
	{ 0x006, RCLKEN, 0 },
};

static const struct config_desc MAX96755_MFP7_configs[] = {
	{ 0x0002, AUD_TX_EN_X, 0 },
	{ 0x0002, AUD_TX_EN_Y, 0 }
};

static const struct config_desc MAX96755_MFP8_configs[] = {
	{ 0x0002, AUD_TX_EN_X, 0 },
	{ 0x0002, AUD_TX_EN_Y, 0 }
};

static const struct config_desc MAX96755_MFP9_configs[] = {
	{ 0x0002, AUD_TX_EN_X, 0 },
	{ 0x0002, AUD_TX_EN_Y, 0 }
};

static const struct config_desc MAX96755_MFP10_configs[] = {
	{ 0x0001, IIC_2_EN, 0 },
	{ 0x0003, UART_2_EN, 0 },
	{ 0x0140, AUD_RX_EN, 0 },
};

static const struct config_desc MAX96755_MFP11_configs[] = {
	{ 0x0001, IIC_2_EN, 0 },
	{ 0x0003, UART_2_EN, 0 },
	{ 0x0140, AUD_RX_EN, 0 },
};

static const struct config_desc MAX96755_MFP12_configs[] = {
	{ 0x0140, AUD_RX_EN, 0 },
};

static const struct config_desc MAX96755_MFP13_configs[] = {
	{ 0x0005, PU_LF0, 0 },
};

static const struct config_desc MAX96755_MFP14_configs[] = {
	{ 0x0005, PU_LF1, 0 },
};

static const struct config_desc MAX96755_MFP15_configs[] = {
	{ 0x0005, PU_LF2, 0 },
};

static const struct config_desc MAX96755_MFP16_configs[] = {
	{ 0x0005, PU_LF3, 0 },
};

static const struct config_desc MAX96755_MFP17_configs[] = {
	{ 0x0001, IIC_1_EN, 0 },
	{ 0x0003, UART_1_EN, 0 },
};

static const struct config_desc MAX96755_MFP18_configs[] = {
	{ 0x0001, IIC_1_EN, 0 },
	{ 0x0003, UART_1_EN, 0 },
};

static const char *serdes_gpio_groups[] = {
	"MAX96755_MFP0", "MAX96755_MFP1", "MAX96755_MFP2", "MAX96755_MFP3",
	"MAX96755_MFP4", "MAX96755_MFP5", "MAX96755_MFP6", "MAX96755_MFP7",

	"MAX96755_MFP8", "MAX96755_MFP9", "MAX96755_MFP10", "MAX96755_MFP11",
	"MAX96755_MFP12", "MAX96755_MFP13", "MAX96755_MFP14", "MAX96755_MFP15",

	"MAX96755_MFP16", "MAX96755_MFP17", "MAX96755_MFP18", "MAX96755_MFP19",
	"MAX96755_MFP20",
};

static const char *MAX96755_I2C_groups[] = { "MAX96755_I2C" };
static const char *MAX96755_UART_groups[] = { "MAX96755_UART" };

#define FUNCTION_DESC(nm) \
{ \
	.name = #nm, \
	.group_names = nm##_groups, \
	.num_group_names = ARRAY_SIZE(nm##_groups), \
} \

#define FUNCTION_DESC_GPIO_INPUT(id) \
{ \
	.name = "DES_RXID"#id"_TO_SER", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_out_dis = 0, .gpio_rx_en = 1, .gpio_rx_id = id } \
	}, \
} \

#define FUNCTION_DESC_GPIO_OUTPUT(id) \
{ \
	.name = "SER_TXID"#id"_TO_DES", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_out_dis = 1, .gpio_tx_en = 1, .gpio_tx_id = id } \
	}, \
} \

static struct pinctrl_pin_desc max96755_pins_desc[] = {
	PINCTRL_PIN(MAXIM_MAX96755_MFP0, "MAX96755_MFP0"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP1, "MAX96755_MFP1"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP2, "MAX96755_MFP2"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP3, "MAX96755_MFP3"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP4, "MAX96755_MFP4"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP5, "MAX96755_MFP5"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP6, "MAX96755_MFP6"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP7, "MAX96755_MFP7"),

	PINCTRL_PIN(MAXIM_MAX96755_MFP8, "MAX96755_MFP8"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP9, "MAX96755_MFP9"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP10, "MAX96755_MFP10"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP11, "MAX96755_MFP11"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP12, "MAX96755_MFP12"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP13, "MAX96755_MFP13"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP14, "MAX96755_MFP14"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP15, "MAX96755_MFP15"),

	PINCTRL_PIN(MAXIM_MAX96755_MFP16, "MAX96755_MFP16"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP17, "MAX96755_MFP17"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP18, "MAX96755_MFP18"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP19, "MAX96755_MFP19"),
	PINCTRL_PIN(MAXIM_MAX96755_MFP20, "MAX96755_MFP20"),
};

static struct group_desc max96755_groups_desc[] = {
	GROUP_DESC_CONFIG(MAX96755_MFP0),
	GROUP_DESC_CONFIG(MAX96755_MFP1),
	GROUP_DESC(MAX96755_MFP2),
	GROUP_DESC(MAX96755_MFP3),
	GROUP_DESC_CONFIG(MAX96755_MFP4),
	GROUP_DESC_CONFIG(MAX96755_MFP5),
	GROUP_DESC(MAX96755_MFP6),
	GROUP_DESC_CONFIG(MAX96755_MFP7),

	GROUP_DESC_CONFIG(MAX96755_MFP8),
	GROUP_DESC_CONFIG(MAX96755_MFP9),
	GROUP_DESC_CONFIG(MAX96755_MFP10),
	GROUP_DESC_CONFIG(MAX96755_MFP11),
	GROUP_DESC_CONFIG(MAX96755_MFP12),
	GROUP_DESC_CONFIG(MAX96755_MFP13),
	GROUP_DESC_CONFIG(MAX96755_MFP14),
	GROUP_DESC_CONFIG(MAX96755_MFP15),

	GROUP_DESC_CONFIG(MAX96755_MFP16),
	GROUP_DESC_CONFIG(MAX96755_MFP17),
	GROUP_DESC_CONFIG(MAX96755_MFP18),
	GROUP_DESC(MAX96755_MFP19),
	GROUP_DESC(MAX96755_MFP20),
	GROUP_DESC(MAX96755_I2C),
	GROUP_DESC(MAX96755_UART),
};

static struct function_desc max96755_functions_desc[] = {
	FUNCTION_DESC_GPIO_INPUT(0),
	FUNCTION_DESC_GPIO_INPUT(1),
	FUNCTION_DESC_GPIO_INPUT(2),
	FUNCTION_DESC_GPIO_INPUT(3),
	FUNCTION_DESC_GPIO_INPUT(4),
	FUNCTION_DESC_GPIO_INPUT(5),
	FUNCTION_DESC_GPIO_INPUT(6),
	FUNCTION_DESC_GPIO_INPUT(7),

	FUNCTION_DESC_GPIO_INPUT(8),
	FUNCTION_DESC_GPIO_INPUT(9),
	FUNCTION_DESC_GPIO_INPUT(10),
	FUNCTION_DESC_GPIO_INPUT(11),
	FUNCTION_DESC_GPIO_INPUT(12),
	FUNCTION_DESC_GPIO_INPUT(13),
	FUNCTION_DESC_GPIO_INPUT(14),
	FUNCTION_DESC_GPIO_INPUT(15),

	FUNCTION_DESC_GPIO_INPUT(16),
	FUNCTION_DESC_GPIO_INPUT(17),
	FUNCTION_DESC_GPIO_INPUT(18),
	FUNCTION_DESC_GPIO_INPUT(19),
	FUNCTION_DESC_GPIO_INPUT(20),

	FUNCTION_DESC_GPIO_OUTPUT(0),
	FUNCTION_DESC_GPIO_OUTPUT(1),
	FUNCTION_DESC_GPIO_OUTPUT(2),
	FUNCTION_DESC_GPIO_OUTPUT(3),
	FUNCTION_DESC_GPIO_OUTPUT(4),
	FUNCTION_DESC_GPIO_OUTPUT(5),
	FUNCTION_DESC_GPIO_OUTPUT(6),
	FUNCTION_DESC_GPIO_OUTPUT(7),

	FUNCTION_DESC_GPIO_OUTPUT(8),
	FUNCTION_DESC_GPIO_OUTPUT(9),
	FUNCTION_DESC_GPIO_OUTPUT(10),
	FUNCTION_DESC_GPIO_OUTPUT(11),
	FUNCTION_DESC_GPIO_OUTPUT(12),
	FUNCTION_DESC_GPIO_OUTPUT(13),
	FUNCTION_DESC_GPIO_OUTPUT(14),
	FUNCTION_DESC_GPIO_OUTPUT(15),

	FUNCTION_DESC_GPIO_OUTPUT(16),
	FUNCTION_DESC_GPIO_OUTPUT(17),
	FUNCTION_DESC_GPIO_OUTPUT(18),
	FUNCTION_DESC_GPIO_OUTPUT(19),
	FUNCTION_DESC_GPIO_OUTPUT(20),

	FUNCTION_DESC(MAX96755_I2C),
	FUNCTION_DESC(MAX96755_UART),
};

static struct serdes_chip_pinctrl_info max96755_pinctrl_info = {
	.pins = max96755_pins_desc,
	.num_pins = ARRAY_SIZE(max96755_pins_desc),
	.groups = max96755_groups_desc,
	.num_groups = ARRAY_SIZE(max96755_groups_desc),
	.functions = max96755_functions_desc,
	.num_functions = ARRAY_SIZE(max96755_functions_desc),
};

static bool max96755_bridge_link_locked(struct serdes *serdes)
{
	u32 val;

	if (dm_gpio_is_valid(&serdes->lock_gpio)) {
		val = dm_gpio_get_value(&serdes->lock_gpio);
		SERDES_DBG_CHIP("serdes %s:val=%d\n", __func__, val);
		return val;
	}

	if (serdes_reg_read(serdes, 0x0013, &val)) {
		SERDES_DBG_CHIP("serdes %s: false val=%d\n", __func__, val);
		return false;
	}

	if (!FIELD_GET(LOCKED, val)) {
		SERDES_DBG_CHIP("serdes %s: false val=%d\n", __func__, val);
		return false;
	}

	return true;
}

static bool max96755_bridge_detect(struct serdes *serdes)
{
	return max96755_bridge_link_locked(serdes);
}

static int max96755_bridge_enable(struct serdes *serdes)
{
	int ret = 0;

	SERDES_DBG_CHIP("%s: serdes chip %s ret=%d\n",
			__func__, serdes->chip_data->name, ret);
	return ret;
}

static int max96755_bridge_disable(struct serdes *serdes)
{
	int ret = 0;

	ret = serdes_set_bits(serdes, 0x0002, VID_TX_EN_X,
			      FIELD_PREP(VID_TX_EN_X, 0));

	return ret;
}

static struct serdes_chip_bridge_ops max96755_bridge_ops = {
	.detect = max96755_bridge_detect,
	.enable = max96755_bridge_enable,
	.disable = max96755_bridge_disable,
};

static int max96755_pinctrl_set_pin_mux(struct serdes *serdes,
					unsigned int pin_selector,
					unsigned int func_selector)
{
	struct function_desc *func;
	struct pinctrl_pin_desc *pin;
	int offset;
	u16 ms;

	func = &serdes->chip_data->pinctrl_info->functions[func_selector];
	if (!func) {
		printf("%s: func is null\n", __func__);
		return -EINVAL;
	}

	pin = &serdes->chip_data->pinctrl_info->pins[pin_selector];
	if (!pin) {
		printf("%s: pin is null\n", __func__);
		return -EINVAL;
	}

	SERDES_DBG_CHIP("%s: serdes %s func=%s data=%p pin=%s num=%d\n",
			__func__, serdes->dev->name,
			func->name, func->data,
			pin->name, pin->number);

	if (func->data) {
		struct serdes_function_data *fdata = func->data;

		ms = fdata->mdelay;
		offset = pin->number;
		if (offset > 32)
			dev_err(serdes->dev, "%s offset=%d > 32\n",
				serdes->dev->name, offset);
		else
			SERDES_DBG_CHIP("%s: serdes %s txid=%d rxid=%d off=%d\n",
					__func__, serdes->dev->name,
					fdata->gpio_tx_id, fdata->gpio_rx_id, offset);

		if (!ms) {
			serdes_set_bits(serdes, GPIO_A_REG(offset),
					GPIO_OUT_DIS | GPIO_RX_EN | GPIO_TX_EN,
					FIELD_PREP(GPIO_OUT_DIS, fdata->gpio_out_dis) |
					FIELD_PREP(GPIO_RX_EN, fdata->gpio_rx_en) |
					FIELD_PREP(GPIO_TX_EN, fdata->gpio_tx_en));

			if (fdata->gpio_tx_en)
				serdes_set_bits(serdes,
						GPIO_B_REG(offset),
						GPIO_TX_ID,
						FIELD_PREP(GPIO_TX_ID, fdata->gpio_tx_id));
			if (fdata->gpio_rx_en)
				serdes_set_bits(serdes,
						GPIO_C_REG(offset),
						GPIO_RX_ID,
						FIELD_PREP(GPIO_RX_ID, fdata->gpio_rx_id));
		} else {
			mdelay(ms);
			SERDES_DBG_CHIP("%s: delay %dms\n", __func__, ms);
		}
	}

	return 0;
}

static int max96755_pinctrl_set_grp_mux(struct serdes *serdes,
					unsigned int group_selector,
					unsigned int func_selector)
{
	struct serdes_pinctrl *pinctrl = serdes->serdes_pinctrl;
	struct function_desc *func;
	struct group_desc *grp;
	int i, offset;
	u16 ms;

	func = &serdes->chip_data->pinctrl_info->functions[func_selector];
	if (!func) {
		printf("%s: func is null\n", __func__);
		return -EINVAL;
	}

	grp = &serdes->chip_data->pinctrl_info->groups[group_selector];
	if (!grp) {
		printf("%s: grp is null\n", __func__);
		return -EINVAL;
	}

	SERDES_DBG_CHIP("%s: serdes %s func=%s data=%p grp=%s data=%p, num=%d\n",
			__func__, serdes->chip_data->name, func->name,
			func->data, grp->name, grp->data, grp->num_pins);

	if (func->data) {
		struct serdes_function_data *fdata = func->data;

		ms = fdata->mdelay;
		for (i = 0; i < grp->num_pins; i++) {
			offset = grp->pins[i] - pinctrl->pin_base;
			if (offset > 32)
				dev_err(serdes->dev, "%s offset=%d > 32\n",
					serdes->dev->name, offset);
			else
				SERDES_DBG_CHIP("%s: serdes %s txid=%d rxid=%d off=%d\n",
						__func__, serdes->dev->name,
						fdata->gpio_tx_id, fdata->gpio_rx_id, offset);

			if (!ms) {
				serdes_set_bits(serdes, GPIO_A_REG(offset),
						GPIO_OUT_DIS | GPIO_RX_EN | GPIO_TX_EN,
						FIELD_PREP(GPIO_OUT_DIS, fdata->gpio_out_dis) |
						FIELD_PREP(GPIO_RX_EN, fdata->gpio_rx_en) |
						FIELD_PREP(GPIO_TX_EN, fdata->gpio_tx_en));
				if (fdata->gpio_tx_en)
					serdes_set_bits(serdes,
							GPIO_B_REG(offset),
							GPIO_TX_ID,
							FIELD_PREP(GPIO_TX_ID, fdata->gpio_tx_id));
				if (fdata->gpio_rx_en)
					serdes_set_bits(serdes,
							GPIO_C_REG(offset),
							GPIO_RX_ID,
							FIELD_PREP(GPIO_RX_ID, fdata->gpio_rx_id));
			} else {
				mdelay(ms);
				SERDES_DBG_CHIP("%s: delay %dms\n", __func__, ms);
			}
		}
	}

	if (grp->data) {
		struct serdes_group_data *gdata = grp->data;

		for (i = 0; i < gdata->num_configs; i++) {
			const struct config_desc *config = &gdata->configs[i];
			serdes_set_bits(serdes, config->reg,
					config->mask, config->val);
		}
	}

	return 0;
}

static int max96755_pinctrl_config_set(struct serdes *serdes,
				       unsigned int pin_selector,
				       unsigned int param, unsigned int argument)
{
	u8 res_cfg;

	switch (param) {
	case PIN_CONFIG_DRIVE_OPEN_DRAIN:
		serdes_set_bits(serdes, GPIO_B_REG(pin_selector),
				OUT_TYPE, FIELD_PREP(OUT_TYPE, 0));
		break;
	case PIN_CONFIG_DRIVE_PUSH_PULL:
		serdes_set_bits(serdes, GPIO_B_REG(pin_selector),
				OUT_TYPE, FIELD_PREP(OUT_TYPE, 1));
		break;
	case PIN_CONFIG_BIAS_DISABLE:
		serdes_set_bits(serdes, GPIO_C_REG(pin_selector),
				PULL_UPDN_SEL,
				FIELD_PREP(PULL_UPDN_SEL, 0));
		break;
	case PIN_CONFIG_BIAS_PULL_UP:
		switch (argument) {
		case 40000:
			res_cfg = 0;
			break;
		case 1000000:
			res_cfg = 1;
			break;
		default:
			return -EINVAL;
		}

		serdes_set_bits(serdes, GPIO_A_REG(pin_selector),
				RES_CFG, FIELD_PREP(RES_CFG, res_cfg));
		serdes_set_bits(serdes, GPIO_C_REG(pin_selector),
				PULL_UPDN_SEL,
				FIELD_PREP(PULL_UPDN_SEL, 1));
		break;
	case PIN_CONFIG_BIAS_PULL_DOWN:
		switch (argument) {
		case 40000:
			res_cfg = 0;
			break;
		case 1000000:
			res_cfg = 1;
			break;
		default:
			return -EINVAL;
		}

		serdes_set_bits(serdes, GPIO_A_REG(pin_selector),
				RES_CFG, FIELD_PREP(RES_CFG, res_cfg));
		serdes_set_bits(serdes, GPIO_C_REG(pin_selector),
				PULL_UPDN_SEL,
				FIELD_PREP(PULL_UPDN_SEL, 2));
		break;
	case PIN_CONFIG_OUTPUT:
			serdes_set_bits(serdes, GPIO_A_REG(pin_selector),
					GPIO_OUT_DIS | GPIO_OUT,
					FIELD_PREP(GPIO_OUT_DIS, 0) |
					FIELD_PREP(GPIO_OUT, argument));
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static struct serdes_chip_pinctrl_ops max96755_pinctrl_ops = {
	.pinconf_set = max96755_pinctrl_config_set,
	.pinmux_set = max96755_pinctrl_set_pin_mux,
	.pinmux_group_set = max96755_pinctrl_set_grp_mux,
};

static int max96755_gpio_direction_input(struct serdes *serdes, int gpio)
{
	return 0;
}

static int max96755_gpio_direction_output(struct serdes *serdes,
					  int gpio, int value)
{
	return 0;
}

static int max96755_gpio_get_level(struct serdes *serdes, int gpio)
{
	return 0;
}

static int max96755_gpio_set_level(struct serdes *serdes, int gpio, int value)
{
	return 0;
}

static int max96755_gpio_set_config(struct serdes *serdes,
				    int gpio, unsigned long config)
{
	return 0;
}

static int max96755_gpio_to_irq(struct serdes *serdes, int gpio)
{
	return 0;
}

static struct serdes_chip_gpio_ops max96755_gpio_ops = {
	.direction_input = max96755_gpio_direction_input,
	.direction_output = max96755_gpio_direction_output,
	.get_level = max96755_gpio_get_level,
	.set_level = max96755_gpio_set_level,
	.set_config = max96755_gpio_set_config,
	.to_irq = max96755_gpio_to_irq,
};

static int max96755_select(struct serdes *serdes, int chan)
{
	u32 link_cfg;
	int ret = 0;

	serdes_set_bits(serdes, 0x0001, DIS_REM_CC,
			FIELD_PREP(DIS_REM_CC, 0));

	serdes_reg_read(serdes, 0x0010, &link_cfg);
	if ((link_cfg & LINK_CFG) == SPLITTER_MODE)
		SERDES_DBG_CHIP("%s: serdes %s is split mode cfg=0x%x\n",
				__func__,
				serdes->chip_data->name, link_cfg);

	if (chan == 0 && (link_cfg & LINK_CFG) != DUAL_LINK) {
		ret = serdes_set_bits(serdes, 0x0004,
				      LINK_EN_B | LINK_EN_A,
				      FIELD_PREP(LINK_EN_A, 1) |
				      FIELD_PREP(LINK_EN_B, 1));
		ret = serdes_set_bits(serdes, 0x0010,
				      RESET_ONESHOT | AUTO_LINK | LINK_CFG,
				      FIELD_PREP(RESET_ONESHOT, 1) |
				      FIELD_PREP(AUTO_LINK, 0) |
				      FIELD_PREP(LINK_CFG, DUAL_LINK));
		SERDES_DBG_CHIP("%s: change to use dual link\n", __func__);
	} else if (chan == 1 && (link_cfg & LINK_CFG) != LINKA) {
		ret = serdes_set_bits(serdes, 0x0004,
				      LINK_EN_B | LINK_EN_A,
				      FIELD_PREP(LINK_EN_A, 1) |
				      FIELD_PREP(LINK_EN_B, 0));
		ret = serdes_set_bits(serdes, 0x0010,
				      RESET_ONESHOT | AUTO_LINK | LINK_CFG,
				      FIELD_PREP(RESET_ONESHOT, 1) |
				      FIELD_PREP(AUTO_LINK, 0) |
				      FIELD_PREP(LINK_CFG, LINKA));
		SERDES_DBG_CHIP("%s: change to use linkA\n", __func__);
	} else if (chan == 2 && (link_cfg & LINK_CFG) != LINKB) {
		ret = serdes_set_bits(serdes, 0x0004,
				      LINK_EN_B | LINK_EN_A,
				      FIELD_PREP(LINK_EN_A, 0) |
				      FIELD_PREP(LINK_EN_B, 1));
		ret = serdes_set_bits(serdes, 0x0010,
				      RESET_ONESHOT | AUTO_LINK | LINK_CFG,
				      FIELD_PREP(RESET_ONESHOT, 1) |
				      FIELD_PREP(AUTO_LINK, 0) |
				      FIELD_PREP(LINK_CFG, LINKB));
		SERDES_DBG_CHIP("%s: change to use linkB\n", __func__);
	} else if (chan == 3 && (link_cfg & LINK_CFG) != SPLITTER_MODE) {
		ret = serdes_set_bits(serdes, 0x0004,
				      LINK_EN_B | LINK_EN_A,
				      FIELD_PREP(LINK_EN_A, 1) |
				      FIELD_PREP(LINK_EN_B, 1));
		ret = serdes_set_bits(serdes, 0x0010,
				      RESET_ONESHOT | AUTO_LINK | LINK_CFG,
				      FIELD_PREP(RESET_ONESHOT, 1) |
				      FIELD_PREP(AUTO_LINK, 0) |
				      FIELD_PREP(LINK_CFG, SPLITTER_MODE));
		SERDES_DBG_CHIP("%s: change to use split mode\n", __func__);
	}

	return ret;
}

static int max96755_deselect(struct serdes *serdes, int chan)
{
	//serdes_set_bits(serdes, 0x0001, DIS_REM_CC,
	//		   FIELD_PREP(DIS_REM_CC, 1));

	return 0;
}

static struct serdes_chip_split_ops max96755_split_ops = {
	.select = max96755_select,
	.deselect = max96755_deselect,
};

struct serdes_chip_data serdes_max96755_data = {
	.name		= "max96755",
	.serdes_type	= TYPE_SER,
	.serdes_id	= MAXIM_ID_MAX96755,
	.connector_type	= DRM_MODE_CONNECTOR_DSI,
	.pinctrl_info	= &max96755_pinctrl_info,
	.bridge_ops	= &max96755_bridge_ops,
	.pinctrl_ops	= &max96755_pinctrl_ops,
	.gpio_ops	= &max96755_gpio_ops,
	.split_ops	= &max96755_split_ops,
};
EXPORT_SYMBOL_GPL(serdes_max96755_data);

MODULE_LICENSE("GPL");
