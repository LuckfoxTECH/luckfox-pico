#
# Copyright (c) 2017 Theobroma Systems Design und Consulting GmbH
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-$(CONFIG_ROCKCHIP_SDRAM_COMMON) += sdram_common.o
obj-$(CONFIG_ROCKCHIP_RK3368) += dmc-rk3368.o
obj-$(CONFIG_ROCKCHIP_RV1108) += sdram_rv1108_pctl_phy.o sdram_rv1108.o
obj-$(CONFIG_ROCKCHIP_RV1106) += sdram_rv1106.o
obj-$(CONFIG_ROCKCHIP_RV1126) += sdram_rv1126.o sdram_pctl_px30.o
obj-$(CONFIG_ROCKCHIP_RK3128) += sdram_rk3128.o
obj-$(CONFIG_ROCKCHIP_RK3188) += sdram_rk3188.o
obj-$(CONFIG_ROCKCHIP_RK322X) += sdram_rk322x.o
obj-$(CONFIG_ROCKCHIP_RK3288) += sdram_rk3288.o
obj-$(CONFIG_ROCKCHIP_RK3308) += sdram_rv1108_pctl_phy.o sdram_rk3308.o
obj-$(CONFIG_ROCKCHIP_RK3328) += sdram_rk3328.o sdram_pctl_px30.o sdram_phy_px30.o
obj-$(CONFIG_ROCKCHIP_RK3399) += sdram_rk3399.o
obj-$(CONFIG_ROCKCHIP_RK3528) += sdram_rk3528.o
obj-$(CONFIG_ROCKCHIP_RK3562) += sdram_rk3562.o
obj-$(CONFIG_ROCKCHIP_RK3568) += sdram_rk3568.o
obj-$(CONFIG_ROCKCHIP_RK3576) += sdram_rk3576.o
obj-$(CONFIG_ROCKCHIP_RK3588) += sdram_rk3588.o
obj-$(CONFIG_ROCKCHIP_PX30) += sdram_px30.o sdram_pctl_px30.o sdram_phy_px30.o
obj-$(CONFIG_ROCKCHIP_PX30) += rockchip_sdram.o
obj-$(CONFIG_ROCKCHIP_RK1808) += rockchip_sdram.o
obj-$(CONFIG_ROCKCHIP_RV1108) += rockchip_sdram.o
obj-$(CONFIG_ROCKCHIP_RK3036) += rockchip_sdram.o
obj-$(CONFIG_ROCKCHIP_RK3308) += rockchip_sdram.o
ifeq ($(CONFIG_SPL_BUILD)$(CONFIG_TPL_BUILD),)
obj-$(CONFIG_ROCKCHIP_DMC) += rockchip_dmc.o
obj-$(CONFIG_ROCKCHIP_DMC_FSP) += dmc_fsp.o
endif
