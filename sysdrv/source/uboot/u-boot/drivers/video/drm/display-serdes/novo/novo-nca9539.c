// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * maxim-nca9539.c  --  I2C register interface access for nca9539 chip
 *
 * Copyright (c) 2023-2028 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "../core.h"

struct serdes_chip_data serdes_nca9539_data = {
	.name			= "nca9539",
	.serdes_type	= TYPE_OTHER,
	.serdes_id		= NOVO_ID_NCA9539,
	.reg_val_type	= REG_8BIT_VAL_8IT,
};
EXPORT_SYMBOL_GPL(serdes_nca9539_data);
