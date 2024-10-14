/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * core.h -- core define for mfd display arch
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 *
 */
#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <errno.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_dsc.h>
#include <drm_modes.h>
#include <video_bridge.h>
#include <asm/unaligned.h>
#include <linux/media-bus-format.h>
#include <linux/bitfield.h>
#include <linux/iopoll.h>
#include <power/regulator.h>
#include <config.h>
#include <backlight.h>
#include <malloc.h>
#include <video.h>

#include <asm/gpio.h>
#include <dm/device.h>
#include <dm/read.h>
#include <dm/pinctrl.h>
#include <dm/uclass-id.h>
#include <dm/lists.h>

#include <dm/device-internal.h>
#include <dm/root.h>
#include <fdtdec.h>
#include <regmap.h>
#include <asm/gpio.h>
#include <asm/system.h>
#include <asm/io.h>

#include "gpio.h"

#include "../drivers/video/drm/rockchip_bridge.h"
#include "../drivers/video/drm/rockchip_display.h"
#include "../drivers/video/drm/rockchip_panel.h"

#ifndef __SERDES_DISPLAY_CORE_H__
#define __SERDES_DISPLAY_CORE_H__

//#define SERDES_DEBUG_MFD
//#define SERDES_DEBUG_I2C
//#define SERDES_DEBUG_CHIP

#ifdef SERDES_DEBUG_MFD
#define  SERDES_DBG_MFD(x...) printf(x)
#else
#define  SERDES_DBG_MFD(x...)
#endif

#ifdef SERDES_DEBUG_I2C
#define  SERDES_DBG_I2C(x...) printf(x)
#else
#define  SERDES_DBG_I2C(x...)
#endif

#ifdef SERDES_DEBUG_CHIP
#define  SERDES_DBG_CHIP(x...) printf(x)
#else
#define  SERDES_DBG_CHIP(x...)
#endif

#define SERDES_UBOOT_DISPLAY_VERSION "serdes-uboot-displaly-v10-230920"
#define MAX_NUM_SERDES_SPLIT 8
struct serdes;

enum ser_link_mode {
	SER_DUAL_LINK,
	SER_LINKA,
	SER_LINKB,
	SER_SPLITTER_MODE,
};

/* Convenience macro to define a single named or anonymous pin descriptor */
#define PINCTRL_PIN(a, b) { .number = a, .name = b }

/**
 * struct pinctrl_pin_desc - boards/machines provide information on their
 * pins, pads or other muxable units in this struct
 * @number: unique pin number from the global pin number space
 * @name: a name for this pin
 * @drv_data: driver-defined per-pin data. pinctrl core does not touch this
 */
struct pinctrl_pin_desc {
	unsigned int number;
	const char *name;
	void *drv_data;
};

/**
 * struct group_desc - generic pin group descriptor
 * @name: name of the pin group
 * @pins: array of pins that belong to the group
 * @num_pins: number of pins in the group
 * @data: pin controller driver specific data
 */
struct group_desc {
	const char *name;
	int *pins;
	int num_pins;
	void *data;
};

/**
 * struct function_desc - generic function descriptor
 * @name: name of the function
 * @group_names: array of pin group names
 * @num_group_names: number of pin group names
 * @data: pin controller driver specific data
 */
struct function_desc {
	const char *name;
	const char **group_names;
	int num_group_names;
	void *data;
};

struct serdes_chip_pinctrl_info {
	struct pinctrl_pin_desc *pins;
	unsigned int num_pins;
	struct group_desc *groups;
	unsigned int num_groups;
	struct function_desc *functions;
	unsigned int num_functions;
};

struct serdes_chip_bridge_ops {
	/*serdes chip function for bridge*/
	int (*power_on)(struct serdes *serdes);
	int (*init)(struct serdes *serdes);
	int (*attach)(struct serdes *serdes);
	bool (*detect)(struct serdes *serdes);
	int (*get_modes)(struct serdes *serdes);
	int (*pre_enable)(struct serdes *serdes);
	int (*enable)(struct serdes *serdes);
	int (*disable)(struct serdes *serdes);
	int (*post_disable)(struct serdes *serdes);
};

struct serdes_chip_panel_ops {
	/*serdes chip function for bridge*/
	int (*power_on)(struct serdes *serdes);
	int (*init)(struct serdes *serdes);
	int (*disable)(struct serdes *serdes);
	int (*unprepare)(struct serdes *serdes);
	int (*prepare)(struct serdes *serdes);
	int (*enable)(struct serdes *serdes);
	int (*get_modes)(struct serdes *serdes);
	int (*mode_set)(struct serdes *serdes);
	int (*backlight_enable)(struct serdes *serdes);
	int (*backlight_disable)(struct serdes *serdes);
};

struct serdes_chip_pinctrl_ops {
	int (*pinmux_set)(struct serdes *serdes, unsigned int pin_selector,
			  unsigned int func_selector);
	int (*pinmux_group_set)(struct serdes *serdes,
				unsigned int group_selector,
				unsigned int func_selector);
	int (*pinconf_set)(struct serdes *serdes,
			   unsigned int pin_selector,
			   unsigned int param, unsigned int argument);
	int (*pinconf_group_set)(struct serdes *serdes,
				 unsigned int group_selector,
				 unsigned int param, unsigned int argument);
};

struct serdes_chip_gpio_ops {
	/*serdes chip gpio function*/
	int (*direction_input)(struct serdes *serdes, int gpio);
	int (*direction_output)(struct serdes *serdes, int gpio, int value);
	int (*get_level)(struct serdes *serdes, int gpio);
	int (*set_level)(struct serdes *serdes, int gpio, int value);
	int (*set_config)(struct serdes *serdes,
			  int gpio, unsigned long config);
	int (*to_irq)(struct serdes *serdes, int gpio);
};

struct serdes_chip_split_ops {
	int (*select)(struct serdes *serdes, int chan);
	int (*deselect)(struct serdes *serdes, int chan);
	int (*set_i2c_addr)(struct serdes *serdes, int address, int link);
};
struct serdes_chip_irq_ops {
	/*serdes chip function for lock and err irq*/
	int (*lock_handle)(struct serdes *serdes);
	int (*err_handle)(struct serdes *serdes);
};

struct serdes_chip_data {
	const char *name;
	enum serdes_type serdes_type;
	enum serdes_id serdes_id;
	enum reg_val_type reg_val_type;
	int sequence_init;
	int connector_type;
	int reg_id;
	int id_data;
	int int_status_reg;
	int int_trig;
	int num_gpio;
	int gpio_base;
	int same_chip_count;
	u8 bank_num;

	int (*chip_init)(struct serdes *serdes);
	struct serdes_chip_pinctrl_info *pinctrl_info;
	struct serdes_chip_bridge_ops *bridge_ops;
	struct serdes_chip_panel_ops *panel_ops;
	struct serdes_chip_pinctrl_ops *pinctrl_ops;
	struct serdes_chip_gpio_ops *gpio_ops;
	struct serdes_chip_split_ops *split_ops;
	struct serdes_chip_irq_ops *irq_ops;
};

struct reg_sequence {
	unsigned int reg;
	unsigned int def;
	unsigned int delay_us;
};

struct serdes_init_seq {
	struct reg_sequence *reg_sequence;
	unsigned int reg_seq_cnt;
};

struct serdes_gpio {
	struct udevice *dev;
	struct serdes_pinctrl *parent;
};

struct serdes_pinctrl {
	struct udevice *dev;
	struct serdes *parent;
	struct serdes_gpio *serdes_gpio;
	struct pinctrl_pin_desc *pdesc;
	struct regmap *regmap;
	struct pinctrl_desc *pinctrl_desc;
	int pin_base;
};

struct serdes_panel {
	struct rockchip_panel *panel;
	const char *name;
	u32 width_mm;
	u32 height_mm;
	u32 link_rate;
	u32 lane_count;
	bool ssc;
	struct serdes *parent;
	struct drm_display_mode mode;
	struct udevice *backlight;
	struct rockchip_panel_funcs *panel_ops;
};

struct serdes_panel_split {
	struct rockchip_panel *panel;

	const char *name;
	u32 width_mm;
	u32 height_mm;
	u32 link_rate;
	u32 lane_count;
	bool ssc;
	struct serdes *parent;
	struct drm_display_mode mode;
	struct udevice *backlight;
	struct rockchip_panel_funcs *panel_ops;
};

struct serdes_bridge {
	bool sel_mipi;
	struct mipi_dsi_device *dsi;
	struct serdes *parent;
	struct drm_display_mode mode;
	struct rockchip_bridge *bridge;
	struct rockchip_bridge_funcs *bridge_ops;
};

struct serdes_bridge_split {
	bool sel_mipi;
	struct mipi_dsi_device *dsi;
	struct serdes *parent;
	struct drm_display_mode mode;
	struct rockchip_bridge *bridge;
	struct rockchip_bridge_funcs *bridge_ops;
};

struct serdes {
	struct udevice *dev;
	struct udevice *vpower_supply;
	struct serdes_init_seq *serdes_init_seq;
	enum serdes_type type;

	/*serdes power and reset pin*/
	struct gpio_desc reset_gpio;
	struct gpio_desc enable_gpio;

	/* serdes irq pin */
	struct gpio_desc lock_gpio;
	struct gpio_desc err_gpio;
	int lock_irq;
	int err_irq;
	int lock_irq_trig;
	int err_irq_trig;

	bool sel_mipi;
	struct mipi_dsi_device *dsi;

	bool split_mode_enable;
	unsigned int reg_hw;
	unsigned int reg_use;
	unsigned int link_use;
	unsigned int id_serdes_bridge_split;
	unsigned int id_serdes_panel_split;
	struct serdes *g_serdes_bridge_split;

	struct serdes_bridge *serdes_bridge;
	struct serdes_bridge_split *serdes_bridge_split;
	struct serdes_panel *serdes_panel;
	struct serdes_panel_split *serdes_panel_split;
	struct serdes_pinctrl *serdes_pinctrl;
	struct serdes_chip_data *chip_data;
};

/* Device I/O API */
int serdes_reg_read(struct serdes *serdes, unsigned int reg, unsigned int *val);
int serdes_reg_write(struct serdes *serdes, unsigned int reg, unsigned int val);
int serdes_set_bits(struct serdes *serdes, unsigned int reg,
		    unsigned int mask, unsigned int val);
int serdes_multi_reg_write(struct serdes *serdes,
			   const struct reg_sequence *regs,
			   int num_regs);
int serdes_i2c_set_sequence(struct serdes *serdes);
int serdes_parse_init_seq(struct udevice *dev, const u16 *data,
			  int length, struct serdes_init_seq *seq);
int serdes_get_init_seq(struct serdes *serdes);
int serdes_gpio_register(struct udevice *dev, struct serdes *serdes);
int serdes_pinctrl_register(struct udevice *dev, struct serdes *serdes);
int serdes_bridge_register(struct udevice *dev, struct serdes *serdes);
int serdes_bridge_split_register(struct udevice *dev, struct serdes *serdes);
int serdes_power_init(void);
int serdes_video_bridge_init(void);
int serdes_video_bridge_split_init(void);
int serdes_display_init(void);

extern struct serdes_chip_data serdes_bu18tl82_data;
extern struct serdes_chip_data serdes_bu18rl82_data;
extern struct serdes_chip_data serdes_max96745_data;
extern struct serdes_chip_data serdes_max96752_data;
extern struct serdes_chip_data serdes_max96755_data;
extern struct serdes_chip_data serdes_max96772_data;
extern struct serdes_chip_data serdes_max96789_data;
extern struct serdes_chip_data serdes_rkx111_data;
extern struct serdes_chip_data serdes_rkx121_data;
extern struct serdes_chip_data serdes_nca9539_data;

#endif
