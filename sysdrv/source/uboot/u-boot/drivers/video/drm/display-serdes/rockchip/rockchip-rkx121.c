// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * rockchip-rkx121.c  --  I2C register interface access for rkx121 serdes chip
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "../core.h"

int rkx121_bridge_init(struct serdes *serdes)
{
	return 0;
}

int rkx121_bridge_enable(struct serdes *serdes)
{
	return 0;
}

int rkx121_bridge_disable(struct serdes *serdes)
{
	return 0;
}

static struct serdes_chip_panel_ops rkx121_panel_ops = {
	.init = rkx121_bridge_init,
	.enable = rkx121_bridge_enable,
	.disable = rkx121_bridge_disable,
};

struct serdes_chip_data serdes_rkx121_data = {
	.name			= "rkx121",
	.serdes_type	= TYPE_DES,
	.serdes_id		= ROCKCHIP_ID_RKX121,
	.panel_ops		= &rkx121_panel_ops,
};
EXPORT_SYMBOL_GPL(serdes_rkx121_data);
