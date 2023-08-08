#
# (C) Copyright 2012
# Texas Instruments Incorporated - http://www.ti.com/
# Aneesh V <aneesh@ti.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#
# Based on common/Makefile.
#

ifeq ($(CONFIG_TPL_BUILD), y)
ifndef CONFIG_TPL_TINY_FRAMEWORK
obj-$(CONFIG_SPL_FRAMEWORK) += spl.o
endif
else
ifdef CONFIG_SPL_BUILD
ifdef CONFIG_SPL_DECOMP_HEADER
obj-y += spl_decomp.o
else
obj-$(CONFIG_SPL_FRAMEWORK) += spl.o
endif
endif
endif

ifdef CONFIG_SPL_BUILD
obj-$(CONFIG_$(SPL_TPL_)BOOTROM_SUPPORT) += spl_bootrom.o
ifdef CONFIG_SPL_KERNEL_BOOT
ifdef CONFIG_ARM64
obj-$(CONFIG_$(SPL_TPL_)LOAD_FIT) += spl_fit_tb_arm64.o
else
obj-$(CONFIG_$(SPL_TPL_)LOAD_FIT) += spl_fit_tb_arm32.o
endif
else
obj-$(CONFIG_$(SPL_TPL_)LOAD_FIT) += spl_fit.o
endif
obj-$(CONFIG_$(SPL_TPL_)NOR_SUPPORT) += spl_nor.o
obj-$(CONFIG_$(SPL_TPL_)XIP_SUPPORT) += spl_xip.o
obj-$(CONFIG_$(SPL_TPL_)YMODEM_SUPPORT) += spl_ymodem.o
ifndef CONFIG_SPL_UBI
obj-$(CONFIG_$(SPL_TPL_)NAND_SUPPORT) += spl_nand.o
obj-$(CONFIG_$(SPL_TPL_)ONENAND_SUPPORT) += spl_onenand.o
endif
obj-$(CONFIG_$(SPL_TPL_)UBI) += spl_ubi.o
obj-$(CONFIG_$(SPL_TPL_)NET_SUPPORT) += spl_net.o
obj-$(CONFIG_$(SPL_TPL_)MMC_SUPPORT) += spl_mmc.o
obj-$(CONFIG_$(SPL_TPL_)MTD_SUPPORT) += spl_mtd_blk.o
obj-$(CONFIG_$(SPL_TPL_)RKNAND_SUPPORT) += spl_rknand.o
obj-$(CONFIG_$(SPL_TPL_)ATF) += spl_atf.o
obj-$(CONFIG_$(SPL_TPL_)OPTEE) += spl_optee.o
obj-$(CONFIG_$(SPL_TPL_)LOAD_RKFW) += spl_rkfw.o
obj-$(CONFIG_$(SPL_TPL_)AB) += spl_ab.o
obj-$(CONFIG_$(SPL_TPL_)USB_SUPPORT) += spl_usb.o
obj-$(CONFIG_$(SPL_TPL_)FAT_SUPPORT) += spl_fat.o
obj-$(CONFIG_$(SPL_TPL_)EXT_SUPPORT) += spl_ext.o
obj-$(CONFIG_$(SPL_TPL_)SATA_SUPPORT) += spl_sata.o
obj-$(CONFIG_$(SPL_TPL_)DFU) += spl_dfu.o
obj-$(CONFIG_$(SPL_TPL_)SPI_LOAD) += spl_spi.o
obj-$(CONFIG_$(SPL_TPL_)RAM_SUPPORT) += spl_ram.o
obj-$(CONFIG_$(SPL_TPL_)USB_SDP_SUPPORT) += spl_sdp.o
endif
