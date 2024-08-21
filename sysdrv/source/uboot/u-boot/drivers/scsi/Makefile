#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifndef CONFIG_SPL_BUILD
obj-$(CONFIG_DM_SCSI) += scsi-uclass.o
obj-$(CONFIG_SCSI) += scsi.o
endif

ifdef CONFIG_SPL_BUILD
ifeq ($(CONFIG_SPL_SATA_SUPPORT)$(CONFIG_SPL_UFS_SUPPORT), y)
obj-$(CONFIG_DM_SCSI) += scsi-uclass.o
obj-$(CONFIG_SCSI) += scsi.o
endif
endif
obj-$(CONFIG_SANDBOX) += sandbox_scsi.o
