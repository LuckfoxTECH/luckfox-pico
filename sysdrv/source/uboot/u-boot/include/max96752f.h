/* SPDX-License-Identifier: GPL-2.0 */
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#ifndef _MAX96752F_H_
#define _MAX96752F_H_

#include <linux/bitfield.h>

#define GPIO_A_REG(gpio)	(0x0200 + ((gpio) * 3))
#define GPIO_B_REG(gpio)	(0x0201 + ((gpio) * 3))
#define GPIO_C_REG(gpio)	(0x0202 + ((gpio) * 3))
#define OLDI_REG(x)		(0x01cd + (x))

/* 0002h */
#define AUD_TX_EN		BIT(2)

/* 0010h */
#define RESET_ALL		BIT(7)
#define RESET_LINK		BIT(6)
#define RESET_ONESHOT		BIT(5)
#define AUTO_LINK		BIT(4)
#define SLEEP			BIT(3)
#define LINK_CFG		GENMASK(1, 0)

/* 0050h */
#define STR_SEL			GENMASK(1, 0)

/* 0073h */
#define TX_SRC_ID		GENMASK(2, 0)

/* 0140h */
#define AUD_RX_EN		BIT(0)

/* 0200h */
#define RES_CFG			BIT(7)
#define TX_PRIO			BIT(6)
#define TX_COMP_EN		BIT(5)
#define GPIO_OUT		BIT(4)
#define GPIO_IN			BIT(3)
#define GPIO_RX_EN		BIT(2)
#define GPIO_TX_EN		BIT(1)
#define GPIO_OUT_DIS		BIT(0)

/* 0201h */
#define PULL_UPDN_SEL		GENMASK(7, 6)
#define OUT_TYPE		BIT(5)
#define GPIO_TX_ID		GENMASK(4, 0)

/* 0202h */
#define OVR_RES_CFG		BIT(7)
#define GPIO_RX_ID		GENMASK(4, 0)

/* 01CEh */
#define OLDI_OUTSEL		BIT(7)
#define OLDI_FORMAT		BIT(6)
#define OLDI_4TH_LANE		BIT(5)
#define OLDI_SWAP_AB		BIT(4)
#define OLDI_SPL_EN		BIT(3)
#define OLDI_SPL_MODE		GENMASK(2, 1)
#define OLDI_SPL_POL		BIT(0)

void max96752f_init(struct udevice *dev);

#endif
