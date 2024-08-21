/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Rockchip USBDP Combo PHY with Samsung IP block driver
 *
 * Copyright (C) 2021 Rockchip Electronics Co., Ltd
 */

#ifndef __PHY_ROCKCHIP_USBDP_H_
#define __PHY_ROCKCHIP_USBDP_H_

#if CONFIG_IS_ENABLED(PHY_ROCKCHIP_USBDP)
int rockchip_u3phy_uboot_init(void);
#else
static inline int rockchip_u3phy_uboot_init(void)
{
	return -ENOTSUPP;
}
#endif

#endif
