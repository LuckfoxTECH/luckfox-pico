// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * rockchip-rkx111.c  --  I2C register interface access for rkx111 serdes chip
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "../core.h"

int rkx111_bridge_init(struct serdes *serdes)
{
	return 0;
}

int rkx111_bridge_enable(struct serdes *serdes)
{
	return 0;
}

int rkx111_bridge_disable(struct serdes *serdes)
{
	return 0;
}

static struct serdes_chip_bridge_ops rkx111_bridge_ops = {
	.init = rkx111_bridge_init,
	.enable = rkx111_bridge_enable,
	.disable = rkx111_bridge_disable,
};

struct serdes_chip_data serdes_rkx111_data = {
	.name			= "rkx111",
	.serdes_type	= TYPE_SER,
	.serdes_id		= ROCKCHIP_ID_RKX111,
	.bridge_ops		= &rkx111_bridge_ops,
};
EXPORT_SYMBOL_GPL(serdes_rkx111_data);
