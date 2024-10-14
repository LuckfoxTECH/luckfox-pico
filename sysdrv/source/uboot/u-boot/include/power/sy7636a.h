// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd.
 */

#ifndef SY7636A_H
#define SY7636A_H

#include <linux/bitops.h>

#define SY7636A_REG_OPERATION_MODE_CRL		0x00
/* It is set if a gpio is used to control the regulator */
#define SY7636A_OPERATION_MODE_CRL_VCOMCTL	BIT(6)
#define SY7636A_OPERATION_MODE_CRL_ONOFF	BIT(7)
#define SY7636A_REG_VCOM_ADJUST_CTRL_L		0x01
#define SY7636A_REG_VCOM_ADJUST_CTRL_H		0x02
#define SY7636A_REG_VCOM_ADJUST_CTRL_MASK	0x01ff
#define SY7636A_REG_VLDO_VOLTAGE_ADJULST_CTRL	0x03
#define SY7636A_REG_POWER_ON_DELAY_TIME		0x06
#define SY7636A_REG_FAULT_FLAG			0x07
#define SY7636A_FAULT_FLAG_PG			BIT(0)
#define SY7636A_REG_TERMISTOR_READOUT		0x08

#define SY7636A_REG_MAX				0x08

#define VCOM_ADJUST_CTRL_MASK                   0x1ff
/* Used to shift the high byte */
#define VCOM_ADJUST_CTRL_SHIFT	                8
/* Used to scale from VCOM_ADJUST_CTRL to mv */
#define VCOM_ADJUST_CTRL_SCAL	                10000
/* VCOM[7:0] in ADJUST_CTRL_L, VCOM[8] in ADJUST_CTRL_H Bit 7 */
#define VCOM_ADJUST_BIT_8                       BIT(7)

#define FAULT_FLAG_SHIFT	                1

#define SY7636A_REGULATOR_DRIVER_NAME           "sy7636a-regulator"
#define SY7636A_THERMAL_COMTATIBLE_NAME         "sy7636a-thermal"

#endif /* SY7636A_H */
