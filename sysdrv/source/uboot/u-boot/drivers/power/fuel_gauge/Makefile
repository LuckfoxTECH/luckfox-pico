#
# Copyright (C) 2012 Samsung Electronics
# Lukasz Majewski <l.majewski@samsung.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-$(CONFIG_$(SPL_)DM_FUEL_GAUGE) += fuel_gauge_uclass.o

obj-$(CONFIG_POWER_FG_CW201X) += fg_cw201x.o
obj-$(CONFIG_POWER_FG_CW221X) += fg_cw221x.o
obj-$(CONFIG_POWER_FG_MAX17042) += fg_max17042.o
obj-$(CONFIG_POWER_FG_RK818) += fg_rk818.o
obj-$(CONFIG_POWER_FG_RK816) += fg_rk816.o

ifdef CONFIG_SPL_BUILD
obj-$(CONFIG_SPL_POWER_FG_RK817) += spl_fg_rk817.o
else
obj-$(CONFIG_POWER_FG_RK817) += fg_rk817.o
endif
