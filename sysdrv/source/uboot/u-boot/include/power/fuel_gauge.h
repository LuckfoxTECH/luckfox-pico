/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _FUEL_GAUGE_H_
#define _FUEL_GAUGE_H_

/* Capability */
#define FG_CAP_FUEL_GAUGE	BIT(0)
#define FG_CAP_CHARGER		BIT(1)

struct dm_fuel_gauge_ops {
	int (*capability)(struct udevice *dev);
	int (*bat_is_exist)(struct udevice *dev);
	int (*get_soc)(struct udevice *dev);
	int (*get_voltage)(struct udevice *dev);
	int (*get_current)(struct udevice *dev);
	int (*get_temperature)(struct udevice *dev, int *temp);
	bool (*get_chrg_online)(struct udevice *dev);
	int (*set_charger_voltage)(struct udevice *dev, int uV);
	int (*set_charger_enable)(struct udevice *dev);
	int (*set_charger_disable)(struct udevice *dev);
	int (*set_iprechg_current)(struct udevice *dev, int iprechrg_uA);
	int (*set_charger_current)(struct udevice *dev, int ichrg_uA);
};

int fuel_gauge_capability(struct udevice *dev);
int fuel_gauge_bat_is_exist(struct udevice *dev);
int fuel_gauge_update_get_soc(struct udevice *dev);
int fuel_gauge_get_voltage(struct udevice *dev);
int fuel_gauge_get_current(struct udevice *dev);
bool fuel_gauge_get_chrg_online(struct udevice *dev);
int fuel_gauge_get_temperature(struct udevice *dev, int *temp);
int charger_set_charger_voltage(struct udevice *dev, int uV);
int charger_set_iprechg_current(struct udevice *dev, int iprechrg_uA);
int charger_set_current(struct udevice *dev, int ichrg_uA);
int charger_set_enable(struct udevice *dev);
int charger_set_disable(struct udevice *dev);

#endif
