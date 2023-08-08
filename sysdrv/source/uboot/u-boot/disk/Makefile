#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

#ccflags-y += -DET_DEBUG -DDEBUG

obj-$(CONFIG_PARTITIONS) 	+= part.o
ifndef CONFIG_TPL_BUILD
obj-$(CONFIG_$(SPL_)MAC_PARTITION)   += part_mac.o
obj-$(CONFIG_$(SPL_)DOS_PARTITION)   += part_dos.o
obj-$(CONFIG_$(SPL_)ISO_PARTITION)   += part_iso.o
obj-$(CONFIG_$(SPL_)AMIGA_PARTITION) += part_amiga.o
obj-$(CONFIG_$(SPL_)EFI_PARTITION)   += part_efi.o
obj-$(CONFIG_$(SPL_)ENV_PARTITION)   += part_env.o
obj-$(CONFIG_$(SPL_)RKPARM_PARTITION)   += part_rkparm.o
obj-$(CONFIG_$(SPL_)RKRAM_PARTITION)   += part_rkram.o
endif
