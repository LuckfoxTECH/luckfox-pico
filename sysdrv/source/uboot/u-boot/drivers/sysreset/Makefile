#
# (C) Copyright 2016 Cadence Design Systems Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-$(CONFIG_$(SPL_TPL_)SYSRESET) += sysreset-uclass.o

ifndef CONFIG_SUPPORT_USBPLUG
obj-$(CONFIG_$(SPL_TPL_)SYSRESET_SYSCON_REBOOT) += sysreset-syscon-reboot.o
obj-$(CONFIG_$(SPL_TPL_)SYSRESET_PSCI) += sysreset_psci.o
endif

obj-$(CONFIG_SYSRESET_SYSCON) += sysreset_syscon.o
obj-$(CONFIG_SYSRESET_WATCHDOG) += sysreset_watchdog.o

obj-$(CONFIG_SANDBOX) += sysreset_sandbox.o
obj-$(CONFIG_ARCH_SNAPDRAGON) += sysreset_snapdragon.o
obj-$(CONFIG_ARCH_STI) += sysreset_sti.o
obj-$(CONFIG_TARGET_XTFPGA) += sysreset_xtfpga.o
obj-$(CONFIG_ARCH_ASPEED) += sysreset_ast.o

ifndef CONFIG_TPL_BUILD
obj-$(CONFIG_ARCH_ROCKCHIP) += sysreset_rockchip.o
endif
