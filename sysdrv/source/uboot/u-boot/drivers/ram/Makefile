#
# Copyright (c) 2015 Google, Inc
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:      GPL-2.0+
#
obj-$(CONFIG_$(SPL_TPL_)DM) += ram-uclass.o
obj-$(CONFIG_SANDBOX) += sandbox_ram.o
obj-$(CONFIG_STM32_SDRAM) += stm32_sdram.o
obj-$(CONFIG_ARCH_BMIPS) += bmips_ram.o

obj-$(CONFIG_ARCH_ROCKCHIP) += rockchip/

ifndef CONFIG_SPL_BUILD
obj-$(CONFIG_DM_DMC) += dmc-uclass.o
obj-$(CONFIG_DM_RAMDISK) += ramdisk-uclass.o
obj-$(CONFIG_RAMDISK_RO) += ramdisk_ro.o
endif
