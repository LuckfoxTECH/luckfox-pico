// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <errno.h>
#include <i2c.h>
#include <max96745.h>

struct function_desc {
	const char *name;
	const char **group_names;
	int num_group_names;

	u8 gpio_out_dis:1;
	u8 gpio_io_rx_en:1;
	u8 gpio_tx_en_a:1;
	u8 gpio_tx_en_b:1;
	u8 gpio_rx_en_a:1;
	u8 gpio_rx_en_b:1;
	u8 gpio_tx_id;
	u8 gpio_rx_id;
};

struct group_desc {
	const char *name;
	int *pins;
	int num_pins;
	void *data;
};

struct pin_desc {
	unsigned number;
	const char *name;
};

static const struct pin_desc max96745_pins[] = {
	{0, "gpio0"},
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
	{16, "gpio16"},
	{17, "gpio17"},
	{18, "gpio18"},
	{19, "gpio19"},
	{20, "gpio20"},
	{21, "gpio21"},
	{22, "gpio22"},
	{23, "gpio23"},
	{24, "gpio24"},
	{25, "gpio25"},
};

static int gpio0_pins[] = {0};
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
static int gpio16_pins[] = {16};
static int gpio17_pins[] = {17};
static int gpio18_pins[] = {18};
static int gpio19_pins[] = {19};
static int gpio20_pins[] = {20};
static int gpio21_pins[] = {21};
static int gpio22_pins[] = {22};
static int gpio23_pins[] = {23};
static int gpio24_pins[] = {24};
static int gpio25_pins[] = {25};

#define GROUP_DESC(nm) \
{ \
	.name = #nm, \
	.pins = nm ## _pins, \
	.num_pins = ARRAY_SIZE(nm ## _pins), \
}

static const struct group_desc max96745_groups[] = {
	GROUP_DESC(gpio0),
	GROUP_DESC(gpio1),
	GROUP_DESC(gpio2),
	GROUP_DESC(gpio3),
	GROUP_DESC(gpio4),
	GROUP_DESC(gpio5),
	GROUP_DESC(gpio6),
	GROUP_DESC(gpio7),
	GROUP_DESC(gpio8),
	GROUP_DESC(gpio9),
	GROUP_DESC(gpio10),
	GROUP_DESC(gpio11),
	GROUP_DESC(gpio12),
	GROUP_DESC(gpio13),
	GROUP_DESC(gpio14),
	GROUP_DESC(gpio15),
	GROUP_DESC(gpio16),
	GROUP_DESC(gpio17),
	GROUP_DESC(gpio18),
	GROUP_DESC(gpio19),
	GROUP_DESC(gpio20),
	GROUP_DESC(gpio21),
	GROUP_DESC(gpio22),
	GROUP_DESC(gpio23),
	GROUP_DESC(gpio24),
	GROUP_DESC(gpio25),
};

static const char *gpio_groups[] = {
	"gpio0", "gpio1", "gpio2", "gpio3", "gpio4",
	"gpio5", "gpio6", "gpio7", "gpio8", "gpio9",
	"gpio10", "gpio11", "gpio12", "gpio13", "gpio14",
	"gpio15", "gpio16", "gpio17", "gpio18", "gpio19",
	"gpio20", "gpio21", "gpio22", "gpio23", "gpio24",
	"gpio25",
};

#define FUNCTION_DESC_GPIO_TX_A(id) \
{ \
	.name = "GPIO_TX_A_"#id, \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
	.gpio_out_dis = 1, \
	.gpio_tx_en_a = 1, \
	.gpio_io_rx_en = 1, \
	.gpio_tx_id = id, \
} \

#define FUNCTION_DESC_GPIO_TX_B(id) \
{ \
	.name = "GPIO_TX_B_"#id, \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
	.gpio_out_dis = 1, \
	.gpio_tx_en_b = 1, \
	.gpio_io_rx_en = 1, \
	.gpio_tx_id = id, \
} \

#define FUNCTION_DESC_GPIO_RX_A(id) \
{ \
	.name = "GPIO_RX_A_"#id, \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
	.gpio_rx_en_a = 1, \
	.gpio_rx_id = id, \
} \

#define FUNCTION_DESC_GPIO_RX_B(id) \
{ \
	.name = "GPIO_RX_B_"#id, \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
	.gpio_rx_en_b = 1, \
	.gpio_rx_id = id, \
} \

#define FUNCTION_DESC_GPIO() \
{ \
	.name = "GPIO", \
	.group_names = gpio_groups, \
	.num_group_names = ARRAY_SIZE(gpio_groups), \
} \

static const struct function_desc max96745_functions[] = {
	FUNCTION_DESC_GPIO_TX_A(0),
	FUNCTION_DESC_GPIO_TX_A(1),
	FUNCTION_DESC_GPIO_TX_A(2),
	FUNCTION_DESC_GPIO_TX_A(3),
	FUNCTION_DESC_GPIO_TX_A(4),
	FUNCTION_DESC_GPIO_TX_A(5),
	FUNCTION_DESC_GPIO_TX_A(6),
	FUNCTION_DESC_GPIO_TX_A(7),
	FUNCTION_DESC_GPIO_TX_A(8),
	FUNCTION_DESC_GPIO_TX_A(9),
	FUNCTION_DESC_GPIO_TX_A(10),
	FUNCTION_DESC_GPIO_TX_A(11),
	FUNCTION_DESC_GPIO_TX_A(12),
	FUNCTION_DESC_GPIO_TX_A(13),
	FUNCTION_DESC_GPIO_TX_A(14),
	FUNCTION_DESC_GPIO_TX_A(15),
	FUNCTION_DESC_GPIO_TX_A(16),
	FUNCTION_DESC_GPIO_TX_A(17),
	FUNCTION_DESC_GPIO_TX_A(18),
	FUNCTION_DESC_GPIO_TX_A(19),
	FUNCTION_DESC_GPIO_TX_A(20),
	FUNCTION_DESC_GPIO_TX_A(21),
	FUNCTION_DESC_GPIO_TX_A(22),
	FUNCTION_DESC_GPIO_TX_A(23),
	FUNCTION_DESC_GPIO_TX_A(24),
	FUNCTION_DESC_GPIO_TX_A(25),
	FUNCTION_DESC_GPIO_TX_A(26),
	FUNCTION_DESC_GPIO_TX_A(27),
	FUNCTION_DESC_GPIO_TX_A(28),
	FUNCTION_DESC_GPIO_TX_A(29),
	FUNCTION_DESC_GPIO_TX_A(30),
	FUNCTION_DESC_GPIO_TX_A(31),
	FUNCTION_DESC_GPIO_TX_B(0),
	FUNCTION_DESC_GPIO_TX_B(1),
	FUNCTION_DESC_GPIO_TX_B(2),
	FUNCTION_DESC_GPIO_TX_B(3),
	FUNCTION_DESC_GPIO_TX_B(4),
	FUNCTION_DESC_GPIO_TX_B(5),
	FUNCTION_DESC_GPIO_TX_B(6),
	FUNCTION_DESC_GPIO_TX_B(7),
	FUNCTION_DESC_GPIO_TX_B(8),
	FUNCTION_DESC_GPIO_TX_B(9),
	FUNCTION_DESC_GPIO_TX_B(10),
	FUNCTION_DESC_GPIO_TX_B(11),
	FUNCTION_DESC_GPIO_TX_B(12),
	FUNCTION_DESC_GPIO_TX_B(13),
	FUNCTION_DESC_GPIO_TX_B(14),
	FUNCTION_DESC_GPIO_TX_B(15),
	FUNCTION_DESC_GPIO_TX_B(16),
	FUNCTION_DESC_GPIO_TX_B(17),
	FUNCTION_DESC_GPIO_TX_B(18),
	FUNCTION_DESC_GPIO_TX_B(19),
	FUNCTION_DESC_GPIO_TX_B(20),
	FUNCTION_DESC_GPIO_TX_B(21),
	FUNCTION_DESC_GPIO_TX_B(22),
	FUNCTION_DESC_GPIO_TX_B(23),
	FUNCTION_DESC_GPIO_TX_B(24),
	FUNCTION_DESC_GPIO_TX_B(25),
	FUNCTION_DESC_GPIO_TX_B(26),
	FUNCTION_DESC_GPIO_TX_B(27),
	FUNCTION_DESC_GPIO_TX_B(28),
	FUNCTION_DESC_GPIO_TX_B(29),
	FUNCTION_DESC_GPIO_TX_B(30),
	FUNCTION_DESC_GPIO_TX_B(31),
	FUNCTION_DESC_GPIO_RX_A(0),
	FUNCTION_DESC_GPIO_RX_A(1),
	FUNCTION_DESC_GPIO_RX_A(2),
	FUNCTION_DESC_GPIO_RX_A(3),
	FUNCTION_DESC_GPIO_RX_A(4),
	FUNCTION_DESC_GPIO_RX_A(5),
	FUNCTION_DESC_GPIO_RX_A(6),
	FUNCTION_DESC_GPIO_RX_A(7),
	FUNCTION_DESC_GPIO_RX_A(8),
	FUNCTION_DESC_GPIO_RX_A(9),
	FUNCTION_DESC_GPIO_RX_A(10),
	FUNCTION_DESC_GPIO_RX_A(11),
	FUNCTION_DESC_GPIO_RX_A(12),
	FUNCTION_DESC_GPIO_RX_A(13),
	FUNCTION_DESC_GPIO_RX_A(14),
	FUNCTION_DESC_GPIO_RX_A(15),
	FUNCTION_DESC_GPIO_RX_A(16),
	FUNCTION_DESC_GPIO_RX_A(17),
	FUNCTION_DESC_GPIO_RX_A(18),
	FUNCTION_DESC_GPIO_RX_A(19),
	FUNCTION_DESC_GPIO_RX_A(20),
	FUNCTION_DESC_GPIO_RX_A(21),
	FUNCTION_DESC_GPIO_RX_A(22),
	FUNCTION_DESC_GPIO_RX_A(23),
	FUNCTION_DESC_GPIO_RX_A(24),
	FUNCTION_DESC_GPIO_RX_A(25),
	FUNCTION_DESC_GPIO_RX_A(26),
	FUNCTION_DESC_GPIO_RX_A(27),
	FUNCTION_DESC_GPIO_RX_A(28),
	FUNCTION_DESC_GPIO_RX_A(29),
	FUNCTION_DESC_GPIO_RX_A(30),
	FUNCTION_DESC_GPIO_RX_A(31),
	FUNCTION_DESC_GPIO_RX_B(0),
	FUNCTION_DESC_GPIO_RX_B(1),
	FUNCTION_DESC_GPIO_RX_B(2),
	FUNCTION_DESC_GPIO_RX_B(3),
	FUNCTION_DESC_GPIO_RX_B(4),
	FUNCTION_DESC_GPIO_RX_B(5),
	FUNCTION_DESC_GPIO_RX_B(6),
	FUNCTION_DESC_GPIO_RX_B(7),
	FUNCTION_DESC_GPIO_RX_B(8),
	FUNCTION_DESC_GPIO_RX_B(9),
	FUNCTION_DESC_GPIO_RX_B(10),
	FUNCTION_DESC_GPIO_RX_B(11),
	FUNCTION_DESC_GPIO_RX_B(12),
	FUNCTION_DESC_GPIO_RX_B(13),
	FUNCTION_DESC_GPIO_RX_B(14),
	FUNCTION_DESC_GPIO_RX_B(15),
	FUNCTION_DESC_GPIO_RX_B(16),
	FUNCTION_DESC_GPIO_RX_B(17),
	FUNCTION_DESC_GPIO_RX_B(18),
	FUNCTION_DESC_GPIO_RX_B(19),
	FUNCTION_DESC_GPIO_RX_B(20),
	FUNCTION_DESC_GPIO_RX_B(21),
	FUNCTION_DESC_GPIO_RX_B(22),
	FUNCTION_DESC_GPIO_RX_B(23),
	FUNCTION_DESC_GPIO_RX_B(24),
	FUNCTION_DESC_GPIO_RX_B(25),
	FUNCTION_DESC_GPIO_RX_B(26),
	FUNCTION_DESC_GPIO_RX_B(27),
	FUNCTION_DESC_GPIO_RX_B(28),
	FUNCTION_DESC_GPIO_RX_B(29),
	FUNCTION_DESC_GPIO_RX_B(30),
	FUNCTION_DESC_GPIO_RX_B(31),
	FUNCTION_DESC_GPIO(),
};

static int max96745_get_pins_count(struct udevice *dev)
{
	return ARRAY_SIZE(max96745_pins);
}

static const char *max96745_get_pin_name(struct udevice *dev, unsigned selector)
{
	return max96745_pins[selector].name;
}

static int max96745_pinctrl_get_groups_count(struct udevice *dev)
{
	return ARRAY_SIZE(max96745_groups);
}

static const char *max96745_pinctrl_get_group_name(struct udevice *dev,
						   unsigned selector)
{
	return max96745_groups[selector].name;
}

static int max96745_pinctrl_get_functions_count(struct udevice *dev)
{
	return ARRAY_SIZE(max96745_functions);
}

static const char *max96745_pinctrl_get_function_name(struct udevice *dev,
						      unsigned selector)
{
	return max96745_functions[selector].name;
}

static int max96745_pinmux_set(struct udevice *dev, unsigned group_selector,
			       unsigned func_selector)
{
	const struct group_desc *grp = &max96745_groups[group_selector];
	const struct function_desc *func = &max96745_functions[func_selector];
	int i;

	for (i = 0; i < grp->num_pins; i++) {
		dm_i2c_reg_clrset(dev->parent, GPIO_A_REG(grp->pins[i]),
				  GPIO_OUT_DIS,
				  FIELD_PREP(GPIO_OUT_DIS, func->gpio_out_dis));

		if (func->gpio_tx_en_a || func->gpio_tx_en_b)
			dm_i2c_reg_clrset(dev->parent, GPIO_B_REG(grp->pins[i]),
					  GPIO_TX_ID,
					  FIELD_PREP(GPIO_TX_ID, func->gpio_tx_id));

		if (func->gpio_rx_en_a || func->gpio_rx_en_b)
			dm_i2c_reg_clrset(dev->parent, GPIO_C_REG(grp->pins[i]),
					  GPIO_RX_ID,
					  FIELD_PREP(GPIO_RX_ID, func->gpio_rx_id));

		dm_i2c_reg_clrset(dev->parent, GPIO_D_REG(grp->pins[i]),
				  GPIO_TX_EN_A | GPIO_TX_EN_B | GPIO_IO_RX_EN |
				  GPIO_RX_EN_A | GPIO_RX_EN_B,
				  FIELD_PREP(GPIO_TX_EN_A, func->gpio_tx_en_a) |
				  FIELD_PREP(GPIO_TX_EN_B, func->gpio_tx_en_b) |
				  FIELD_PREP(GPIO_RX_EN_A, func->gpio_rx_en_a) |
				  FIELD_PREP(GPIO_RX_EN_B, func->gpio_rx_en_b) |
				  FIELD_PREP(GPIO_IO_RX_EN, func->gpio_io_rx_en));
	}

	return 0;
}

static struct pinctrl_ops max96745_pinctrl_ops = {
	.get_pins_count = max96745_get_pins_count,
	.get_pin_name = max96745_get_pin_name,
	.get_groups_count = max96745_pinctrl_get_groups_count,
	.get_group_name = max96745_pinctrl_get_group_name,
	.get_functions_count = max96745_pinctrl_get_functions_count,
	.get_function_name = max96745_pinctrl_get_function_name,
	.set_state = pinctrl_generic_set_state,
	.pinmux_set = max96745_pinmux_set,
	.pinmux_group_set = max96745_pinmux_set,
};

static const struct udevice_id max96745_pinctrl_of_match[] = {
	{ .compatible = "maxim,max96745-pinctrl" },
	{ }
};

U_BOOT_DRIVER(max96745_pinctrl) = {
	.name = "pinctrl-max96745",
	.id = UCLASS_PINCTRL,
	.of_match = max96745_pinctrl_of_match,
	.ops = &max96745_pinctrl_ops,
};
