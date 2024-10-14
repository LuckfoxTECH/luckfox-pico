#
#(C) Copyright 2023-2025 Rockchip Electronics Co., Ltd
#
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for multifunction miscellaneous devices that just used for display
#
ifndef CONFIG_SPL_BUILD
obj-$(CONFIG_SERDES_DISPLAY_CHIP_MAXIM)		+= maxim/
obj-$(CONFIG_SERDES_DISPLAY_CHIP_ROHM)		+= rohm/
obj-$(CONFIG_SERDES_DISPLAY_CHIP_ROCKCHIP)	+= rockchip/
obj-$(CONFIG_SERDES_DISPLAY_CHIP_NOVO)		+= novo/

obj-$(CONFIG_SERDES_DISPLAY) +=serdes-i2c.o serdes-core.o serdes-bridge.o serdes-bridge-split.o serdes-panel.o serdes-panel-split.o serdes-pinctrl.o serdes-gpio.o
endif
