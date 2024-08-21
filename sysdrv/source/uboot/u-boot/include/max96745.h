/* SPDX-License-Identifier: GPL-2.0 */
/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 */

#ifndef _MAX96745_H_
#define _MAX96745_H_

#include <linux/bitfield.h>

#define GPIO_A_REG(gpio)	(0x0200 + ((gpio) * 8))
#define GPIO_B_REG(gpio)	(0x0201 + ((gpio) * 8))
#define GPIO_C_REG(gpio)	(0x0202 + ((gpio) * 8))
#define GPIO_D_REG(gpio)	(0x0203 + ((gpio) * 8))

/* 0010h */
#define RESET_ALL		BIT(7)
#define SLEEP			BIT(3)

/* 0011h */
#define CXTP_B			BIT(2)
#define CXTP_A			BIT(0)

/* 0028h, 0032h */
#define LINK_EN			BIT(7)
#define TX_RATE			GENMASK(3, 2)

/* 0029h, 0033h */
#define RESET_LINK		BIT(0)
#define RESET_ONESHOT		BIT(1)

/* 002Ah, 0034h */
#define LINK_LOCKED		BIT(0)

/* 0076h, 0086h */
#define DIS_REM_CC		BIT(7)

/* 0100h */
#define VID_LINK_SEL		GENMASK(2, 1)
#define VID_TX_EN		BIT(0)

/* 0200h */
#define RES_CFG			BIT(7)
#define TX_COM_EN		BIT(5)
#define GPIO_OUT		BIT(4)
#define GPIO_IN			BIT(3)
#define GPIO_OUT_DIS		BIT(0)

/* 0201h */
#define PULL_UPDN_SEL		GENMASK(7, 6)
#define OUT_TYPEC		BIT(5)
#define GPIO_TX_ID		GENMASK(4, 0)

/* 0202h */
#define OVR_RES_CFG		BIT(7)
#define IO_EDGE_RATE		GENMASK(6, 5)
#define GPIO_RX_ID		GENMASK(4, 0)

/* 0203h */
#define GPIO_IO_RX_EN		BIT(5)
#define GPIO_OUT_LGC		BIT(4)
#define GPIO_RX_EN_B		BIT(3)
#define GPIO_TX_EN_B		BIT(2)
#define GPIO_RX_EN_A		BIT(1)
#define GPIO_TX_EN_A		BIT(0)

#endif
