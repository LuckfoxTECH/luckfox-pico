// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * maxim-max96745.c  --  I2C register interface access for max96745 serdes chip
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */
#include "../core.h"
#include "maxim-max96745.h"
struct serdes_function_data {
	u8 gpio_out_dis:1;
	u8 gpio_io_rx_en:1;
	u8 gpio_tx_en_a:1;
	u8 gpio_tx_en_b:1;
	u8 gpio_rx_en_a:1;
	u8 gpio_rx_en_b:1;
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

static int MAX96745_MFP0_pins[] = {0};
static int MAX96745_MFP1_pins[] = {1};
static int MAX96745_MFP2_pins[] = {2};
static int MAX96745_MFP3_pins[] = {3};
static int MAX96745_MFP4_pins[] = {4};
static int MAX96745_MFP5_pins[] = {5};
static int MAX96745_MFP6_pins[] = {6};
static int MAX96745_MFP7_pins[] = {7};

static int MAX96745_MFP8_pins[] = {8};
static int MAX96745_MFP9_pins[] = {9};
static int MAX96745_MFP10_pins[] = {10};
static int MAX96745_MFP11_pins[] = {11};
static int MAX96745_MFP12_pins[] = {12};
static int MAX96745_MFP13_pins[] = {13};
static int MAX96745_MFP14_pins[] = {14};
static int MAX96745_MFP15_pins[] = {15};

static int MAX96745_MFP16_pins[] = {16};
static int MAX96745_MFP17_pins[] = {17};
static int MAX96745_MFP18_pins[] = {18};
static int MAX96745_MFP19_pins[] = {19};
static int MAX96745_MFP20_pins[] = {20};
static int MAX96745_MFP21_pins[] = {21};
static int MAX96745_MFP22_pins[] = {22};
static int MAX96745_MFP23_pins[] = {23};

static int MAX96745_MFP24_pins[] = {24};
static int MAX96745_MFP25_pins[] = {25};
static int MAX96745_I2C_pins[] = {3, 7};
static int MAX96745_UART_pins[] = {3, 7};

#define GROUP_DESC(nm) \
{ \
	.name = #nm, \
	.pins = nm ## _pins, \
	.num_pins = ARRAY_SIZE(nm ## _pins), \
}

static const char *serdes_gpio_groups[] = {
	"MAX96745_MFP0", "MAX96745_MFP1", "MAX96745_MFP2", "MAX96745_MFP3",
	"MAX96745_MFP4", "MAX96745_MFP5", "MAX96745_MFP6", "MAX96745_MFP7",

	"MAX96745_MFP8", "MAX96745_MFP9", "MAX96745_MFP10", "MAX96745_MFP11",
	"MAX96745_MFP12", "MAX96745_MFP13", "MAX96745_MFP14", "MAX96745_MFP15",

	"MAX96745_MFP16", "MAX96745_MFP17", "MAX96745_MFP18", "MAX96745_MFP19",
	"MAX96745_MFP20", "MAX96745_MFP21", "MAX96745_MFP22", "MAX96745_MFP23",

	"MAX96745_MFP24", "MAX96745_MFP25",
};

static const char *MAX96745_I2C_groups[] = { "MAX96745_I2C" };
static const char *MAX96745_UART_groups[] = { "MAX96745_UART" };

#define FUNCTION_DESC(nm) \
{ \
	.name = #nm, \
	.group_names = nm##_groups, \
	.num_group_names = ARRAY_SIZE(nm##_groups), \
} \

#define FUNCTION_DESC_GPIO_OUTPUT_A(id) \
{ \
	.name = "SER_TXID"#id"_TO_DES_LINKA", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_out_dis = 1, .gpio_tx_en_a = 1, \
		  .gpio_io_rx_en = 1, .gpio_tx_id = id } \
	}, \
} \

#define FUNCTION_DESC_GPIO_OUTPUT_B(id) \
{ \
	.name = "SER_TXID"#id"_TO_DES_LINKB", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_out_dis = 1, .gpio_tx_en_b = 1, \
		  .gpio_io_rx_en = 1, .gpio_tx_id = id } \
	}, \
} \

#define FUNCTION_DESC_GPIO_INPUT_A(id) \
{ \
	.name = "DES_RXID"#id"_TO_SER_LINKA", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_rx_en_a = 1, .gpio_rx_id = id } \
	}, \
} \

#define FUNCTION_DESC_GPIO_INPUT_B(id) \
{ \
	.name = "DES_RXID"#id"_TO_SER_LINKB", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ .gpio_rx_en_b = 1, .gpio_rx_id = id } \
	}, \
} \

#define FUNCTION_DESC_GPIO() \
{ \
	.name = "MAX96745_GPIO", \
	.group_names = serdes_gpio_groups, \
	.num_group_names = ARRAY_SIZE(serdes_gpio_groups), \
	.data = (void *)(const struct serdes_function_data []) { \
		{ } \
	}, \
} \

static struct pinctrl_pin_desc max96745_pins_desc[] = {
	PINCTRL_PIN(MAXIM_MAX96745_MFP0, "MAX96745_MFP0"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP1, "MAX96745_MFP1"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP2, "MAX96745_MFP2"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP3, "MAX96745_MFP3"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP4, "MAX96745_MFP4"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP5, "MAX96745_MFP5"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP6, "MAX96745_MFP6"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP7, "MAX96745_MFP7"),

	PINCTRL_PIN(MAXIM_MAX96745_MFP8, "MAX96745_MFP8"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP9, "MAX96745_MFP9"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP10, "MAX96745_MFP10"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP11, "MAX96745_MFP11"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP12, "MAX96745_MFP12"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP13, "MAX96745_MFP13"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP14, "MAX96745_MFP14"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP15, "MAX96745_MFP15"),

	PINCTRL_PIN(MAXIM_MAX96745_MFP16, "MAX96745_MFP16"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP17, "MAX96745_MFP17"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP18, "MAX96745_MFP18"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP19, "MAX96745_MFP19"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP20, "MAX96745_MFP20"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP21, "MAX96745_MFP21"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP22, "MAX96745_MFP22"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP23, "MAX96745_MFP23"),

	PINCTRL_PIN(MAXIM_MAX96745_MFP24, "MAX96745_MFP24"),
	PINCTRL_PIN(MAXIM_MAX96745_MFP25, "MAX96745_MFP25"),
};

static struct group_desc max96745_groups_desc[] = {
	GROUP_DESC(MAX96745_MFP0),
	GROUP_DESC(MAX96745_MFP1),
	GROUP_DESC(MAX96745_MFP2),
	GROUP_DESC(MAX96745_MFP3),
	GROUP_DESC(MAX96745_MFP4),
	GROUP_DESC(MAX96745_MFP5),
	GROUP_DESC(MAX96745_MFP6),
	GROUP_DESC(MAX96745_MFP7),

	GROUP_DESC(MAX96745_MFP8),
	GROUP_DESC(MAX96745_MFP9),
	GROUP_DESC(MAX96745_MFP10),
	GROUP_DESC(MAX96745_MFP11),
	GROUP_DESC(MAX96745_MFP12),
	GROUP_DESC(MAX96745_MFP13),
	GROUP_DESC(MAX96745_MFP14),
	GROUP_DESC(MAX96745_MFP15),

	GROUP_DESC(MAX96745_MFP16),
	GROUP_DESC(MAX96745_MFP17),
	GROUP_DESC(MAX96745_MFP18),
	GROUP_DESC(MAX96745_MFP19),
	GROUP_DESC(MAX96745_MFP20),
	GROUP_DESC(MAX96745_MFP21),
	GROUP_DESC(MAX96745_MFP22),
	GROUP_DESC(MAX96745_MFP23),

	GROUP_DESC(MAX96745_MFP24),
	GROUP_DESC(MAX96745_MFP25),

	GROUP_DESC(MAX96745_I2C),
	GROUP_DESC(MAX96745_UART),
};

static struct function_desc max96745_functions_desc[] = {
	FUNCTION_DESC_GPIO_INPUT_A(0),
	FUNCTION_DESC_GPIO_INPUT_A(1),
	FUNCTION_DESC_GPIO_INPUT_A(2),
	FUNCTION_DESC_GPIO_INPUT_A(3),
	FUNCTION_DESC_GPIO_INPUT_A(4),
	FUNCTION_DESC_GPIO_INPUT_A(5),
	FUNCTION_DESC_GPIO_INPUT_A(6),
	FUNCTION_DESC_GPIO_INPUT_A(7),

	FUNCTION_DESC_GPIO_INPUT_A(8),
	FUNCTION_DESC_GPIO_INPUT_A(9),
	FUNCTION_DESC_GPIO_INPUT_A(10),
	FUNCTION_DESC_GPIO_INPUT_A(11),
	FUNCTION_DESC_GPIO_INPUT_A(12),
	FUNCTION_DESC_GPIO_INPUT_A(13),
	FUNCTION_DESC_GPIO_INPUT_A(14),
	FUNCTION_DESC_GPIO_INPUT_A(15),

	FUNCTION_DESC_GPIO_INPUT_A(16),
	FUNCTION_DESC_GPIO_INPUT_A(17),
	FUNCTION_DESC_GPIO_INPUT_A(18),
	FUNCTION_DESC_GPIO_INPUT_A(19),
	FUNCTION_DESC_GPIO_INPUT_A(20),
	FUNCTION_DESC_GPIO_INPUT_A(21),
	FUNCTION_DESC_GPIO_INPUT_A(22),
	FUNCTION_DESC_GPIO_INPUT_A(23),

	FUNCTION_DESC_GPIO_INPUT_A(24),
	FUNCTION_DESC_GPIO_INPUT_A(25),

	FUNCTION_DESC_GPIO_OUTPUT_A(0),
	FUNCTION_DESC_GPIO_OUTPUT_A(1),
	FUNCTION_DESC_GPIO_OUTPUT_A(2),
	FUNCTION_DESC_GPIO_OUTPUT_A(3),
	FUNCTION_DESC_GPIO_OUTPUT_A(4),
	FUNCTION_DESC_GPIO_OUTPUT_A(5),
	FUNCTION_DESC_GPIO_OUTPUT_A(6),
	FUNCTION_DESC_GPIO_OUTPUT_A(7),

	FUNCTION_DESC_GPIO_OUTPUT_A(8),
	FUNCTION_DESC_GPIO_OUTPUT_A(9),
	FUNCTION_DESC_GPIO_OUTPUT_A(10),
	FUNCTION_DESC_GPIO_OUTPUT_A(11),
	FUNCTION_DESC_GPIO_OUTPUT_A(12),
	FUNCTION_DESC_GPIO_OUTPUT_A(13),
	FUNCTION_DESC_GPIO_OUTPUT_A(14),
	FUNCTION_DESC_GPIO_OUTPUT_A(15),

	FUNCTION_DESC_GPIO_OUTPUT_A(16),
	FUNCTION_DESC_GPIO_OUTPUT_A(17),
	FUNCTION_DESC_GPIO_OUTPUT_A(18),
	FUNCTION_DESC_GPIO_OUTPUT_A(19),
	FUNCTION_DESC_GPIO_OUTPUT_A(20),
	FUNCTION_DESC_GPIO_OUTPUT_A(21),
	FUNCTION_DESC_GPIO_OUTPUT_A(22),
	FUNCTION_DESC_GPIO_OUTPUT_A(23),

	FUNCTION_DESC_GPIO_OUTPUT_A(24),
	FUNCTION_DESC_GPIO_OUTPUT_A(25),

	FUNCTION_DESC_GPIO_INPUT_B(0),
	FUNCTION_DESC_GPIO_INPUT_B(1),
	FUNCTION_DESC_GPIO_INPUT_B(2),
	FUNCTION_DESC_GPIO_INPUT_B(3),
	FUNCTION_DESC_GPIO_INPUT_B(4),
	FUNCTION_DESC_GPIO_INPUT_B(5),
	FUNCTION_DESC_GPIO_INPUT_B(6),
	FUNCTION_DESC_GPIO_INPUT_B(7),

	FUNCTION_DESC_GPIO_INPUT_B(8),
	FUNCTION_DESC_GPIO_INPUT_B(9),
	FUNCTION_DESC_GPIO_INPUT_B(10),
	FUNCTION_DESC_GPIO_INPUT_B(11),
	FUNCTION_DESC_GPIO_INPUT_B(12),
	FUNCTION_DESC_GPIO_INPUT_B(13),
	FUNCTION_DESC_GPIO_INPUT_B(14),
	FUNCTION_DESC_GPIO_INPUT_B(15),

	FUNCTION_DESC_GPIO_INPUT_B(16),
	FUNCTION_DESC_GPIO_INPUT_B(17),
	FUNCTION_DESC_GPIO_INPUT_B(18),
	FUNCTION_DESC_GPIO_INPUT_B(19),
	FUNCTION_DESC_GPIO_INPUT_B(20),
	FUNCTION_DESC_GPIO_INPUT_B(21),
	FUNCTION_DESC_GPIO_INPUT_B(22),
	FUNCTION_DESC_GPIO_INPUT_B(23),

	FUNCTION_DESC_GPIO_INPUT_B(24),
	FUNCTION_DESC_GPIO_INPUT_B(25),

	FUNCTION_DESC_GPIO_OUTPUT_B(0),
	FUNCTION_DESC_GPIO_OUTPUT_B(1),
	FUNCTION_DESC_GPIO_OUTPUT_B(2),
	FUNCTION_DESC_GPIO_OUTPUT_B(3),
	FUNCTION_DESC_GPIO_OUTPUT_B(4),
	FUNCTION_DESC_GPIO_OUTPUT_B(5),
	FUNCTION_DESC_GPIO_OUTPUT_B(6),
	FUNCTION_DESC_GPIO_OUTPUT_B(7),

	FUNCTION_DESC_GPIO_OUTPUT_B(8),
	FUNCTION_DESC_GPIO_OUTPUT_B(9),
	FUNCTION_DESC_GPIO_OUTPUT_B(10),
	FUNCTION_DESC_GPIO_OUTPUT_B(11),
	FUNCTION_DESC_GPIO_OUTPUT_B(12),
	FUNCTION_DESC_GPIO_OUTPUT_B(13),
	FUNCTION_DESC_GPIO_OUTPUT_B(14),
	FUNCTION_DESC_GPIO_OUTPUT_B(15),

	FUNCTION_DESC_GPIO_OUTPUT_B(16),
	FUNCTION_DESC_GPIO_OUTPUT_B(17),
	FUNCTION_DESC_GPIO_OUTPUT_B(18),
	FUNCTION_DESC_GPIO_OUTPUT_B(19),
	FUNCTION_DESC_GPIO_OUTPUT_B(20),
	FUNCTION_DESC_GPIO_OUTPUT_B(21),
	FUNCTION_DESC_GPIO_OUTPUT_B(22),
	FUNCTION_DESC_GPIO_OUTPUT_B(23),

	FUNCTION_DESC_GPIO_OUTPUT_B(24),
	FUNCTION_DESC_GPIO_OUTPUT_B(25),

	FUNCTION_DESC_GPIO(),

	FUNCTION_DESC(MAX96745_I2C),
	FUNCTION_DESC(MAX96745_UART),
};

static struct serdes_chip_pinctrl_info max96745_pinctrl_info = {
	.pins = max96745_pins_desc,
	.num_pins = ARRAY_SIZE(max96745_pins_desc),
	.groups = max96745_groups_desc,
	.num_groups = ARRAY_SIZE(max96745_groups_desc),
	.functions = max96745_functions_desc,
	.num_functions = ARRAY_SIZE(max96745_functions_desc),
};

static bool max96745_bridge_link_locked(struct serdes *serdes)
{
	u32 val;

	if (dm_gpio_is_valid(&serdes->lock_gpio)) {
		val = dm_gpio_get_value(&serdes->lock_gpio);
		SERDES_DBG_CHIP("serdes %s:val=%d\n", __func__, val);
		return val;
	}

	if (serdes_reg_read(serdes, 0x002a, &val)) {
		SERDES_DBG_CHIP("serdes %s: false val=%d\n", __func__, val);
		return false;
	}

	if (!FIELD_GET(LOCKED, val)) {
		SERDES_DBG_CHIP("serdes %s: false val=%d\n", __func__, val);
		return false;
	}

	return true;
}

static bool max96745_bridge_detect(struct serdes *serdes)
{
	return max96745_bridge_link_locked(serdes);
}

static int max96745_bridge_enable(struct serdes *serdes)
{
	struct drm_display_mode *mode =
		 &serdes->serdes_bridge->bridge->state->conn_state.mode;
	u8 cxtp, tx_rate;
	int ret;
	unsigned int value;

	ret = serdes_reg_read(serdes, 0x0011, &value);
	if (ret < 0)
		return ret;

	cxtp = FIELD_GET(CXTP_A, value);

	ret = serdes_reg_read(serdes, 0x0028, &value);
	if (ret < 0)
		return ret;

	tx_rate = FIELD_GET(TX_RATE, value);

	if (!cxtp && mode->clock > 95000 && tx_rate == 1) {
		ret = serdes_set_bits(serdes, 0x0028, TX_RATE,
				      FIELD_PREP(TX_RATE, 2));
		if (ret < 0)
			return ret;

		ret = serdes_set_bits(serdes, 0x0029, RESET_ONESHOT,
				      FIELD_PREP(RESET_ONESHOT, 1));
		if (ret < 0)
			return ret;

		if (readx_poll_timeout(max96745_bridge_link_locked, serdes, ret,
				       ret, 200000))
			printf("%s: GMSL link not locked\n", __func__);
	}

	return ret;
}

static int max96745_bridge_disable(struct serdes *serdes)
{
	u8 cxtp, tx_rate;
	int ret;
	unsigned int value;

	ret = serdes_reg_read(serdes, 0x0011, &value);
	if (ret < 0)
		return ret;

	cxtp = FIELD_GET(CXTP_A, value);

	ret = serdes_reg_read(serdes, 0x0028, &value);
	if (ret < 0)
		return ret;

	tx_rate = FIELD_GET(TX_RATE, value);

	if (!cxtp && tx_rate == 2) {
		ret = serdes_set_bits(serdes, 0x0028, TX_RATE,
				      FIELD_PREP(TX_RATE, 1));
		if (ret < 0)
			return ret;

		ret = serdes_set_bits(serdes, 0x0029, RESET_ONESHOT,
				      FIELD_PREP(RESET_ONESHOT, 1));
		if (ret < 0)
			return ret;

		if (readx_poll_timeout(max96745_bridge_link_locked, serdes, ret,
				       ret, 200000))
			printf("%s: GMSL link not locked\n", __func__);
	}

	return ret;
}

static struct serdes_chip_bridge_ops max96745_bridge_ops = {
	.detect = max96745_bridge_detect,
	.enable = max96745_bridge_enable,
	.disable = max96745_bridge_disable,
};

static int max96745_pinctrl_set_pin_mux(struct serdes *serdes,
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
		struct serdes_function_data *data = func->data;

		ms = data->mdelay;
		offset = pin->number;
		if (offset > 32)
			dev_err(serdes->dev, "%s offset=%d > 32\n",
				serdes->dev->name, offset);
		else
			SERDES_DBG_CHIP("%s: serdes %s txid=%d rxid=%d off=%d\n",
					__func__, serdes->dev->name,
					data->gpio_tx_id, data->gpio_rx_id, offset);
		if (!ms) {
			serdes_set_bits(serdes,
					GPIO_A_REG(offset),
					GPIO_OUT_DIS,
					FIELD_PREP(GPIO_OUT_DIS, data->gpio_out_dis));
			if (data->gpio_tx_en_a || data->gpio_tx_en_b)
				serdes_set_bits(serdes,
						GPIO_B_REG(offset),
						GPIO_TX_ID,
						FIELD_PREP(GPIO_TX_ID, data->gpio_tx_id));
			if (data->gpio_rx_en_a || data->gpio_rx_en_b)
				serdes_set_bits(serdes,
						GPIO_C_REG(offset),
						GPIO_RX_ID,
						FIELD_PREP(GPIO_RX_ID, data->gpio_rx_id));
			serdes_set_bits(serdes,
					GPIO_D_REG(offset),
					GPIO_TX_EN_A | GPIO_TX_EN_B | GPIO_IO_RX_EN |
					GPIO_RX_EN_A | GPIO_RX_EN_B,
					FIELD_PREP(GPIO_TX_EN_A, data->gpio_tx_en_a) |
					FIELD_PREP(GPIO_TX_EN_B, data->gpio_tx_en_b) |
					FIELD_PREP(GPIO_RX_EN_A, data->gpio_rx_en_a) |
					FIELD_PREP(GPIO_RX_EN_B, data->gpio_rx_en_b) |
					FIELD_PREP(GPIO_IO_RX_EN, data->gpio_io_rx_en));
		} else {
			mdelay(ms);
			SERDES_DBG_CHIP("%s: delay %dms\n", __func__, ms);
		}
	}

	return 0;
}

static int max96745_pinctrl_set_grp_mux(struct serdes *serdes,
					unsigned int group_selector,
					unsigned int func_selector)
{
	struct serdes_pinctrl *pinctrl = serdes->serdes_pinctrl;
	struct function_desc *func;
	struct group_desc *grp;
	int i, offset;

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

		for (i = 0; i < grp->num_pins; i++) {
			offset = grp->pins[i] - pinctrl->pin_base;
			if (offset > 32)
				dev_err(serdes->dev, "%s offset=%d > 32\n",
					serdes->dev->name, offset);
			else
				SERDES_DBG_CHIP("%s: serdes %s txid=%d rxid=%d off=%d\n",
						__func__, serdes->dev->name,
						fdata->gpio_tx_id,
						fdata->gpio_rx_id, offset);

			serdes_set_bits(serdes,
					GPIO_A_REG(offset),
					GPIO_OUT_DIS,
					FIELD_PREP(GPIO_OUT_DIS, fdata->gpio_out_dis));
			if (fdata->gpio_tx_en_a || fdata->gpio_tx_en_b)
				serdes_set_bits(serdes,
						GPIO_B_REG(offset),
						GPIO_TX_ID,
						FIELD_PREP(GPIO_TX_ID, fdata->gpio_tx_id));
			if (fdata->gpio_rx_en_a || fdata->gpio_rx_en_b)
				serdes_set_bits(serdes,
						GPIO_C_REG(offset),
						GPIO_RX_ID,
						FIELD_PREP(GPIO_RX_ID, fdata->gpio_rx_id));
			serdes_set_bits(serdes,
					GPIO_D_REG(offset),
					GPIO_TX_EN_A | GPIO_TX_EN_B | GPIO_IO_RX_EN |
					GPIO_RX_EN_A | GPIO_RX_EN_B,
					FIELD_PREP(GPIO_TX_EN_A, fdata->gpio_tx_en_a) |
					FIELD_PREP(GPIO_TX_EN_B, fdata->gpio_tx_en_b) |
					FIELD_PREP(GPIO_RX_EN_A, fdata->gpio_rx_en_a) |
					FIELD_PREP(GPIO_RX_EN_B, fdata->gpio_rx_en_b) |
					FIELD_PREP(GPIO_IO_RX_EN, fdata->gpio_io_rx_en));
		}
	}

	return 0;
}

static int max96745_pinctrl_config_set(struct serdes *serdes,
				       unsigned int pin_selector,
				       unsigned int param,
				       unsigned int argument)
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

static struct serdes_chip_pinctrl_ops max96745_pinctrl_ops = {
	.pinconf_set = max96745_pinctrl_config_set,
	.pinmux_set = max96745_pinctrl_set_pin_mux,
	.pinmux_group_set = max96745_pinctrl_set_grp_mux,
};

static int max96745_gpio_direction_input(struct serdes *serdes, int gpio)
{
	return 0;
}

static int max96745_gpio_direction_output(struct serdes *serdes, int gpio, int value)
{
	return 0;
}

static int max96745_gpio_get_level(struct serdes *serdes, int gpio)
{
	return 0;
}

static int max96745_gpio_set_level(struct serdes *serdes, int gpio, int value)
{
	return 0;
}

static int max96745_gpio_set_config(struct serdes *serdes,
				    int gpio, unsigned long config)
{
	return 0;
}

static int max96745_gpio_to_irq(struct serdes *serdes, int gpio)
{
	return 0;
}

static struct serdes_chip_gpio_ops max96745_gpio_ops = {
	.direction_input = max96745_gpio_direction_input,
	.direction_output = max96745_gpio_direction_output,
	.get_level = max96745_gpio_get_level,
	.set_level = max96745_gpio_set_level,
	.set_config = max96745_gpio_set_config,
	.to_irq = max96745_gpio_to_irq,
};

static int max96745_select(struct serdes *serdes, int chan)
{
	/*0076 for linkA and 0086 for linkB*/
	if (chan == DUAL_LINK) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		SERDES_DBG_CHIP("%s: enable %s remote i2c of linkA/linkB\n",
				__func__,
				serdes->chip_data->name);
	} else if (chan == LINKA) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		SERDES_DBG_CHIP("%s: only enable %s remote i2c of linkA\n",
				__func__,
				serdes->chip_data->name);
	} else if (chan == LINKB) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		SERDES_DBG_CHIP("%s: only enable %s remote i2c of linkB\n",
				__func__,
				serdes->chip_data->name);
	} else if (chan == SPLITTER_MODE) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		SERDES_DBG_CHIP("%s: enable %s remote i2c of linkA/B\n",
				__func__,
				serdes->chip_data->name);
	}

	return 0;
}

static int max96745_deselect(struct serdes *serdes, int chan)
{
	if (chan == DUAL_LINK) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		SERDES_DBG_CHIP("%s: disable %s remote i2c of linkA/B\n",
				__func__,
				serdes->chip_data->name);
	} else if (chan == LINKA) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		SERDES_DBG_CHIP("%s: only disable %s remote i2c of linkA\n",
				__func__,
				serdes->chip_data->name);
	} else if (chan == LINKB) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 0));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		SERDES_DBG_CHIP("%s: only disable %s remote i2c of linkB\n",
				__func__,
				serdes->chip_data->name);
	} else if (chan == SPLITTER_MODE) {
		serdes_set_bits(serdes, 0x0076, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		serdes_set_bits(serdes, 0x0086, DIS_REM_CC,
				FIELD_PREP(DIS_REM_CC, 1));
		SERDES_DBG_CHIP("%s: disable %s remote i2c of linkA/B\n",
				__func__,
				serdes->chip_data->name);
	}

	return 0;
}

static struct serdes_chip_split_ops max96745_split_ops = {
	.select = max96745_select,
	.deselect = max96745_deselect,
};

struct serdes_chip_data serdes_max96745_data = {
	.name		= "max96745",
	.serdes_type	= TYPE_SER,
	.serdes_id	= MAXIM_ID_MAX96745,
	.connector_type	= DRM_MODE_CONNECTOR_eDP,
	.pinctrl_info	= &max96745_pinctrl_info,
	.bridge_ops	= &max96745_bridge_ops,
	.pinctrl_ops	= &max96745_pinctrl_ops,
	.gpio_ops	= &max96745_gpio_ops,
	.split_ops	= &max96745_split_ops,
};
EXPORT_SYMBOL_GPL(serdes_max96745_data);

MODULE_LICENSE("GPL");
