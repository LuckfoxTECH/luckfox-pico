SYSDRV_DIR=$(shell pwd)

export LC_ALL=C
SHELL:=/bin/bash
MAKE=make
AT?=@

################################################################################
##   Input Configuraton Overlay
################################################################################
# SYSDRV_BUILD_VERSION (RELEASE/DEBUG)
SYSDRV_BUILD_VERSION ?= RELEASE
ifneq ($(RK_BUILD_VERSION_TYPE),)
SYSDRV_BUILD_VERSION := $(RK_BUILD_VERSION_TYPE)
endif

ifneq ($(RK_JOBS),)
SYSDRV_JOBS := $(RK_JOBS)
endif

ifneq ($(RK_TOOLCHAIN_CROSS),)
SYSDRV_CROSS:= $(RK_TOOLCHAIN_CROSS)
endif

ifneq ($(RK_LIBC_TPYE),)
SYSDRV_LIB_TYPE := $(RK_LIBC_TPYE)
else
SYSDRV_LIB_TYPE ?= glibc
endif

ifneq ($(RK_BUILDROOT_DEFCONFIG),)
BUILDROOT_DEFCONFIG := $(RK_BUILDROOT_DEFCONFIG)
endif
ifeq ($(BUILDROOT_DEFCONFIG),)
BUILDROOT_DEFCONFIG := luckfox_pico_defconfig
endif

SYSDRV_ROOTFS_FS_TYPE ?= $(fs_type)
ifneq ($(RK_ROOTFS_FS_TPYE),)
SYSDRV_ROOTFS_FS_TYPE := $(RK_ROOTFS_FS_TPYE)
endif

SYSDRV_ROOTFS_FS_COMP ?= $(fs_comp)
ifneq ($(RK_ROOTFS_FS_COMP),)
SYSDRV_ROOTFS_FS_COMP := $(RK_ROOTFS_FS_COMP)
endif

ifneq ($(RK_BOOT_MEDIUM),)
BOOT_MEDIUM := $(RK_BOOT_MEDIUM)
endif

ifeq ($(BOOT_MEDIUM),)
BOOT_MEDIUM := emmc
endif

ifneq ($(RK_KERNEL_DTS),)
KERNEL_DTS := $(RK_KERNEL_DTS)
endif
ifneq ($(RK_KERNEL_DEFCONFIG),)
KERNEL_CFG := $(RK_KERNEL_DEFCONFIG)
endif
ifneq ($(RK_KERNEL_DEFCONFIG_FRAGMENT),)
KERNEL_CFG_FRAGMENT := $(RK_KERNEL_DEFCONFIG_FRAGMENT)
endif
ifneq ($(RK_UBOOT_DEFCONFIG),)
UBOOT_CFG := $(RK_UBOOT_DEFCONFIG)
endif
ifneq ($(RK_UBOOT_DEFCONFIG_FRAGMENT),)
UBOOT_CFG_FRAGMENT := $(RK_UBOOT_DEFCONFIG_FRAGMENT)
endif

ifneq ($(RK_PROJECT_OUTPUT_IMAGE),)
OUTPUT_IMAGE_PATH := $(RK_PROJECT_OUTPUT_IMAGE)
else
OUTPUT_IMAGE_PATH :=
endif

ifneq ($(RK_PROJECT_PATH_BOARD_BIN),)
OUTPUT_BOARD_BIN_PATH := $(RK_PROJECT_PATH_BOARD_BIN)
else
OUTPUT_BOARD_BIN_PATH :=
endif

ifneq ($(RK_PROJECT_PATH_SYSDRV),)
OUTPUT_ROOTFS_SOURCE_DIR := $(RK_PROJECT_PATH_SYSDRV)
OUTPUT_ROOTFS_TOOLS_DIR := $(RK_PROJECT_PATH_SYSDRV)
else
OUTPUT_ROOTFS_SOURCE_DIR :=
OUTPUT_ROOTFS_TOOLS_DIR :=
endif

ifeq ($(RK_ENABLE_FASTBOOT),y)
OUTPUT_FASTBOOT_IMAGE_PATH := $(RK_PROJECT_PATH_FASTBOOT)
else
OUTPUT_FASTBOOT_IMAGE_PATH :=
endif

ifneq ($(OUTPUT_SYSDRV_RAMDISK_DIR),)
SYSDRV_BUILD_RAMDISK_FLAG := YES
endif

################################################################################
##   rootfs Select
################################################################################
ifneq ($(LF_TARGET_ROOTFS),)
   TARGET_ROOTFS := $(LF_TARGET_ROOTFS)
else
#    TARGET_ROOTFS := ubuntu
   TARGET_ROOTFS := buildroot
#    TARGET_ROOTFS := busybox
#    TARGET_ROOTFS := custom
endif

ifneq ($(findstring $(TARGET_ROOTFS),custom),)
   ROOTFS_BUILD_ENV := 
else ifneq ($(findstring $(TARGET_ROOTFS),ubuntu),)
   ROOTFS_BUILD_ENV := ubuntu
else ifneq ($(findstring $(TARGET_ROOTFS),buildroot),)
   ROOTFS_BUILD_ENV := rootfs_prepare pctools buildroot boardtools drv
else
   ROOTFS_BUILD_ENV := rootfs_prepare pctools busybox boardtools drv
endif

################################################################################
##   Public Configuraton
################################################################################
include $(SYSDRV_DIR)/Makefile.param

ifneq ($(RK_PARTITION_CMD_IN_ENV),)
CONFIG_SYSDRV_PARTITION := $(RK_PARTITION_CMD_IN_ENV)
endif

CONFIG_SYSDRV_PARTITION ?=
ROOTFS_PART_SIZE ?= $(shell $(SYSDRV_DIR)/tools/pc/toolkits/get_part_info.sh \
					PART_SIZE "$(CONFIG_SYSDRV_PARTITION)" rootfs)
ROOTFS_PART_NUM ?= $(shell $(SYSDRV_DIR)/tools/pc/toolkits/get_part_info.sh \
				   PART_NUM "$(CONFIG_SYSDRV_PARTITION)" rootfs $(BOOT_MEDIUM))
ENV_PART_SIZE ?= $(shell $(SYSDRV_DIR)/tools/pc/toolkits/get_part_info.sh \
				 PART_SIZE "$(CONFIG_SYSDRV_PARTITION)" env)
ENV_PART_OFFSET ?= $(shell $(SYSDRV_DIR)/tools/pc/toolkits/get_part_info.sh \
				   PART_OFFSET "$(CONFIG_SYSDRV_PARTITION)" env)

ifneq ($(findstring ext4 ext2 jffs2 squashfs ubifs erofs,$(RK_PROJECT_ROOTFS_TYPE)),)
ifneq ($(findstring FAIL,$(ROOTFS_PART_SIZE)),)
$(error ## ROOTFS_PART_SIZE is invalid)
endif
endif
ifneq ($(findstring FAIL,$(ENV_PART_SIZE)),)
$(warning ## ENV_PART_SIZE is invalid)
endif
ifneq ($(findstring FAIL,$(ENV_PART_OFFSET)),)
$(warning ## ENV_PART_OFFSET is invalid)
endif

ifneq ($(findstring $(BOOT_MEDIUM),spi_nand slc_nand),)
SYSDRV_ROOTFS_BUILD_TARGET := rootfs_ubi
ifeq ($(SYSDRV_ROOTFS_FS_TYPE),)
SYSDRV_ROOTFS_FS_TYPE := ubifs
endif
ifeq ($(SYSDRV_ROOTFS_FS_TYPE),ubifs)
KERNEL_CMDLINE_FRAGMENT += root=ubi0:rootfs
endif
ifneq ($(findstring $(SYSDRV_ROOTFS_FS_TYPE),squashfs erofs),)
KERNEL_CMDLINE_FRAGMENT += ubi.block=0,rootfs root=/dev/ubiblock0_0
endif
KERNEL_CMDLINE_FRAGMENT += rootfstype=$(SYSDRV_ROOTFS_FS_TYPE) \
						   ubi.mtd=$(ROOTFS_PART_NUM)
endif

ifeq ($(BOOT_MEDIUM), emmc)
SYSDRV_ROOTFS_BUILD_TARGET := rootfs_ext4
ifeq ($(SYSDRV_ROOTFS_FS_TYPE),)
SYSDRV_ROOTFS_FS_TYPE := ext4
endif
KERNEL_CMDLINE_FRAGMENT += rootfstype=$(SYSDRV_ROOTFS_FS_TYPE) \
						   root=/dev/mmcblk0p$(ROOTFS_PART_NUM)
endif

ifeq ($(BOOT_MEDIUM), spi_nor)
SYSDRV_ROOTFS_BUILD_TARGET := rootfs_jffs2
ifeq ($(SYSDRV_ROOTFS_FS_TYPE),)
SYSDRV_ROOTFS_FS_TYPE := jffs2
endif
KERNEL_CMDLINE_FRAGMENT += rootfstype=$(SYSDRV_ROOTFS_FS_TYPE) \
						   root=/dev/mtdblock$(ROOTFS_PART_NUM)
endif

ifneq ($(RK_KERNEL_CMDLINE_FRAGMENT),)
KERNEL_CMDLINE_FRAGMENT := $(RK_KERNEL_CMDLINE_FRAGMENT)
endif
ifeq ($(SYSDRV_ROOTFS_FS_TYPE),squashfs)
SYSDRV_ROOTFS_BUILD_TARGET := rootfs_squashfs
ifneq ($(findstring $(BOOT_MEDIUM),spi_nand slc_nand),)
SYSDRV_ROOTFS_BUILD_TARGET := rootfs_ubi
endif
endif

ifeq ($(SYSDRV_ROOTFS_FS_TYPE),initramfs)
SYSDRV_ROOTFS_BUILD_TARGET := rootfs_initramfs
endif

ifeq ($(ENABLE_RNDIS),y)
ifeq ($(CHIP),rv1106)
override KERNEL_CFG_FRAGMENT += rv1106-rndis.config
else
$(error TODO rndis for $(CHIP) ...)
endif
endif

# TODO: rename defconfig with BOOT_MEDIUM
KERNEL_CFG ?=
KERNEL_DTS ?=
KERNEL_DIR ?= $(SYSDRV_DIR)/source/kernel
KERNEL_DIR_DRV_KO ?= $(KERNEL_DIR)/drv_ko
KERNEL_DTB :=
ifeq ($(ARCH),arm)
KERNEL_DTB := $(KERNEL_DIR)/arch/$(ARCH)/boot/dts/$(KERNEL_DTS:dts=dtb)
endif
ifeq ($(ARCH),arm64)
KERNEL_DTB := $(KERNEL_DIR)/arch/$(ARCH)/boot/dts/rockchip/$(KERNEL_DTS:dts=dtb)
endif

TOOLCHAIN_FILE := $(shell which $(SYSDRV_CROSS)-gcc )
TOOLCHAIN_DIR := $(shell dirname $(shell dirname $(TOOLCHAIN_FILE)))
RUNTIMELIB_DIR = $(shell dirname $(TOOLCHAIN_DIR))/$(RUNTIME_LIB)

TOOLCHAIN_RUNTIME_LIB:=runtime_lib

RKBIN_DIR := $(SYSDRV_DIR)/source/uboot/rkbin
UBOOT_DIR := $(SYSDRV_DIR)/source/uboot/u-boot
DOWNLOAD_SRC_BIN := *_download_v*.bin
DOWNLOAD_BIN := download.bin
IDBLOCK_IMG := idblock.img
IDBLOCK_SRC_BIN := *_idblock_v*.img
UBOOT_COMPILE_MAKE := $(UBOOT_DIR)/make.sh

ifneq ($(RK_UBOOT_RKBIN_INI_OVERLAY),)
UBOOT_COMPILE_MAKE_OPTS ?= --spl-new $(RKBIN_DIR)/$(RK_UBOOT_RKBIN_INI_OVERLAY)
else
UBOOT_COMPILE_MAKE_OPTS ?= --spl-new
endif

UBOOT_CFG ?=
UBOOT_CFG_FRAGMENT ?=

ifeq ($(SYSDRV_BUILD_RAMDISK_FLAG),YES)
ifneq ($(TINY_ROOTFS_BUSYBOX_CFG),)
BUSYBOX_CFG ?= $(TINY_ROOTFS_BUSYBOX_CFG)
else
$(error Please config TINY_ROOTFS_BUSYBOX_CFG on cfg/cfg.mk)
endif
endif

BUSYBOX_CFG ?= config_normal
BUSYBOX_VER ?= busybox-1.27.2
BUSYBOX_DIR ?= $(SYSDRV_DIR)/source/busybox

BUILDROOT_CFG ?= config_normal
BUILDROOT_VER ?= buildroot-2023.02.6
BUILDROOT_DIR := $(SYSDRV_DIR)/source/buildroot

UBUNTU_VER ?= luckfox-ubuntu-22.04.3

TOOLCHAIN_RUNTIME_LIB_C:=lib.tar.bz2

EXT4_TOOL:=mkfs_ext4.sh
UBI_TOOL:=mkfs_ubi.sh
SQUASHFS_TOOL:=mkfs_squashfs.sh
JFFS2_TOOL:=mkfs_jffs2.sh
EROFS_TOOL:=mkfs_erofs.sh
INITRAMFS_TOOL:=mkfs_initramfs.sh

export OUT_BOARD:=board_$(SYSDRV_LIB_TYPE)_$(CHIP)
export OUT_IMAGE:=image_$(SYSDRV_LIB_TYPE)_$(CHIP)
export OUT_ROOTFS:=rootfs_$(SYSDRV_LIB_TYPE)_$(CHIP)

SYSDRV_DIR_OUT_IMAGE := $(SYSDRV_DIR)/out/$(OUT_IMAGE)
SYSDRV_DIR_OUT_BOARD := $(SYSDRV_DIR)/out/bin/$(OUT_BOARD)
SYSDRV_DIR_OUT_PC := $(SYSDRV_DIR)/out/bin/pc
SYSDRV_DIR_OUT_ROOTFS:=$(SYSDRV_DIR)/out/$(OUT_ROOTFS)

SYSDRV_KERNEL_MOD_PATH:=$(SYSDRV_DIR)/out/kernel_drv_ko
SYSDRV_ROOTFS_OUT_IMAGE := $(SYSDRV_DIR_OUT_IMAGE)/rootfs_base.img
SYSDRV_UBOOT_ENV_CFG_FILE :=$(SYSDRV_DIR_OUT_IMAGE)/.env.txt

export SYSDRV_DIR_OUT_PC
export SYSDRV_DIR_OUT_BOARD
export SYSDRV_DIR_OUT_ROOTFS

export SYSDRV_KERNEL_MOD_PATH
################################################################################
##   Check Configuraton
################################################################################
ifeq ($(ROOTFS_PART_SIZE),0)
$(warning Check ROOTFS_PART_SIZE is 0 on $(CONFIG_SYSDRV_PARTITION))
# default build rootfs partition size 32MB
ROOTFS_PART_SIZE := 32*0x100000
endif

################################################################################
##    set task
################################################################################
all: uboot kernel rootfs env

clean: uboot_clean kernel_clean buildroot_clean pctools_clean boardtools_clean rootfs_clean drv_clean

distclean: clean out_clean

info:
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "$(C_YELLOW)$(CHIP) sysdrv info:"
	@echo -e "$(C_YELLOW)    BOOT_MEDIUM:         $(BOOT_MEDIUM) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    KERNEL_CFG:          $(KERNEL_CFG)         $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    KERNEL_DTS:          $(KERNEL_DTS)         $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    KERNEL_CFG_FRAGMENT: $(KERNEL_CFG_FRAGMENT)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)    KERNEL_CMDLINE_FRAGMENT: $(KERNEL_CMDLINE_FRAGMENT)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)    UBOOT_CFG:           $(UBOOT_CFG)          $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    UBOOT_CFG_FRAGMENT:  $(UBOOT_CFG_FRAGMENT) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    UBOOT_ENV_SIZE:      $(ENV_PART_SIZE) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    UBOOT_ENV_OFFSET:    $(ENV_PART_OFFSET) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    ROOTFS_PART_SIZE:    $(ROOTFS_PART_SIZE) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    ROOTFS_PART_NUM:     $(ROOTFS_PART_NUM) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    SYSDRV_PARTITION:    $(CONFIG_SYSDRV_PARTITION) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    C LIBRARY TYPE:      $(SYSDRV_LIB_TYPE) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    <> kernel compile: $(C_NORMAL)"
	@echo -e "$(C_YELLOW)       cd $(KERNEL_DIR)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)       $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $(KERNEL_CFG) $(KERNEL_CFG_FRAGMENT)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)       $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) BOOT_ITS=boot.its $(KERNEL_DTS:dts=img) -j$(SYSDRV_JOBS)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)    <> kernel out image: boot.img $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    <> uboot compile: $(C_NORMAL)"
	@echo -e "$(C_YELLOW)       cd $(UBOOT_DIR)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)       $(MAKE) $(UBOOT_CFG) $(UBOOT_CFG_FRAGMENT)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)       ./make.sh $(UBOOT_COMPILE_MAKE_OPTS) CROSS_COMPILE=$(CROSS_COMPILE) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)       cp -fv ./$(IDBLOCK_SRC_BIN) idblock.img $(C_NORMAL)"
	@echo -e "$(C_YELLOW)       cp -fv ./$(DOWNLOAD_SRC_BIN) $(DOWNLOAD_BIN) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)    <> uboot out image: uboot.img idblock.img $(DOWNLOAD_BIN) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"

help:
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "$(C_YELLOW)help: 	specified parameters:                              $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        BOOT_MEDIUM: emmc/spi_nor/spi_nand/slc_nand (default emmc) $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        KERNEL_DTS:                $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        KERNEL_CFG:                $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        KERNEL_CFG_FRAGMENT:       $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        UBOOT_CFG:              $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        UBOOT_CFG_FRAGMENT:     $(C_NORMAL)"
	@echo -e "$(C_YELLOW)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)e.g.:$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make BOOT_MEDIUM=emmc     # default build uboot/kernel/rootfs   $(C_NORMAL)"
	@echo -e "$(C_YELLOW)$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make uboot   [BOOT_MEDIUM=...]  # build uboot $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make kernel  [BOOT_MEDIUM=...]  # build kernel $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make rootfs  [BOOT_MEDIUM=...]  # build rootfs $(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make rootfs_ubi          	# force package ubi rootfs image$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make rootfs_ext4         	# force package ext4 rootfs image$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make rootfs_jffs2        	# force package jffs2 rootfs image$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make rootfs_squashfs     	# force package squashfs rootfs image$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make rootfs_erofs        	# force package erofs rootfs image$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make rootfs_initramfs    	# force package initramfs rootfs image$(C_NORMAL)"
	@echo -e "$(C_YELLOW)        make buildroot_menuconfig  # config buildroot $(C_NORMAL)"
	@echo -e "$(C_YELLOW)------------------------------------------------------------------------- $(C_NORMAL)"

################################################################################
#	prepare
################################################################################
prepare:
	@mkdir -p $(SYSDRV_DIR_OUT_IMAGE)
	@mkdir -p $(SYSDRV_DIR_OUT_BOARD)
	@mkdir -p $(SYSDRV_DIR_OUT_PC)
	@mkdir -p $(SYSDRV_DIR_OUT_ROOTFS)

##########################################################################################
#	build uboot
##########################################################################################
uboot: prepare
	@echo -e "$(C_GREEN) ==sysdrv== build uboot  $(C_NORMAL)"
	@$(MAKE) -C $(UBOOT_DIR) $(UBOOT_CFG) $(UBOOT_CFG_FRAGMENT)
	pushd $(UBOOT_DIR);$(UBOOT_COMPILE_MAKE) $(UBOOT_COMPILE_MAKE_OPTS) CROSS_COMPILE=$(CROSS_COMPILE);popd
	$(AT)test ! -f $(UBOOT_DIR)/uboot.img || cp -fv $(UBOOT_DIR)/uboot.img $(SYSDRV_DIR_OUT_IMAGE)
	$(AT)cp -fv $(UBOOT_DIR)/$(IDBLOCK_SRC_BIN) $(SYSDRV_DIR_OUT_IMAGE)/$(IDBLOCK_IMG)
	$(AT)cp -fv $(UBOOT_DIR)/$(DOWNLOAD_SRC_BIN) $(SYSDRV_DIR_OUT_IMAGE)/$(DOWNLOAD_BIN)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_IMAGE_PATH), \
		$(SYSDRV_DIR_OUT_IMAGE)/$(IDBLOCK_IMG) \
		$(SYSDRV_DIR_OUT_IMAGE)/uboot.img \
		$(SYSDRV_DIR_OUT_IMAGE)/$(DOWNLOAD_BIN))

uboot_clean:
	pushd $(UBOOT_DIR);$(MAKE) CROSS_COMPILE=$(CROSS_COMPILE) distclean >/dev/null;popd

##########################################################################################
#	build kernel
##########################################################################################
kernel: prepare
	@echo -e "$(C_GREEN) ==sysdrv== build kernel  $(C_NORMAL)"
ifeq ($(RK_ENABLE_FASTBOOT), y)
	$(call MACRO_CHK_DRIVER_MODULE, $(SYSDRV_DIR)/drv_ko/rockit/, rockit-ko)
	$(call MACRO_CHK_DRIVER_MODULE, $(SYSDRV_DIR)/drv_ko/kmpp/, kmpp)
endif
	@rm -rf $(KERNEL_DTB)
	$(MAKE) -C $(KERNEL_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $(KERNEL_CFG) $(KERNEL_CFG_FRAGMENT)
	$(MAKE) -C $(KERNEL_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $(KERNEL_DTS:dts=img) BOOT_ITS=boot.its -j$(SYSDRV_JOBS)
ifeq ($(RK_ENABLE_FASTBOOT), y)
	$(call MACRO_CLEAN_DRIVER_MODULE, rockit-ko)
	$(call MACRO_CLEAN_DRIVER_MODULE, kmpp)
endif
	cp -fv $(KERNEL_DIR)/vmlinux $(SYSDRV_DIR_OUT_BOARD)
	update_dtb_bootargs.sh --cmdline '$(KERNEL_CMDLINE_FRAGMENT)' --dtb $(KERNEL_DTB) --output $(KERNEL_DTB)
ifneq ($(RK_ENABLE_FASTBOOT), y)
	$(MAKE) -C $(KERNEL_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $(KERNEL_DTS:dts=img) BOOT_ITS=boot.its -j$(SYSDRV_JOBS)
endif
ifneq ($(KERNEL_DTB),)
	cp -fv $(KERNEL_DTB) $(SYSDRV_DIR_OUT_BOARD)
endif
	# Start install kernel drivers
ifeq ($(SYSDRV_BUILD_RAMDISK_FLAG),YES)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_SYSDRV_RAMDISK_DIR), \
		$(KERNEL_DIR)/arch/$(ARCH)/boot/Image* \
		$(KERNEL_DTB) \
		$(KERNEL_DIR)/arch/$(ARCH)/boot/zImage \
		$(KERNEL_DIR)/resource.img)
else
ifneq ($(RK_ENABLE_FASTBOOT), y)
	cp -fv $(KERNEL_DIR)/boot.img $(SYSDRV_DIR_OUT_IMAGE)/boot.img
endif
ifneq ($(OUTPUT_FASTBOOT_IMAGE_PATH),)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_FASTBOOT_IMAGE_PATH), $(KERNEL_DIR)/arch/$(ARCH)/boot/Image)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_FASTBOOT_IMAGE_PATH), $(KERNEL_DIR)/resource.img)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_FASTBOOT_IMAGE_PATH), $(KERNEL_DTB))
endif
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_IMAGE_PATH), $(SYSDRV_DIR_OUT_IMAGE)/boot.img)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_BOARD_BIN_PATH), \
		$(SYSDRV_DIR_OUT_BOARD)/$(KERNEL_DTS:dts=dtb) $(SYSDRV_DIR_OUT_BOARD)/vmlinux)
endif

kernel_clean:
	$(MAKE) -C $(KERNEL_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) distclean
	@-rm -rf $(SYSDRV_KERNEL_MOD_PATH) $(KERNEL_DIR_DRV_KO)

##########################################################################################
#	prepare rootfs
##########################################################################################
rootfs_prepare: prepare
	@echo -e "$(C_GREEN) ==sysdrv== prepare rootfs   $(C_NORMAL)"
	tar xf $(SYSDRV_DIR)/tools/board/rootfs_script.tar -C $(SYSDRV_DIR_OUT_ROOTFS)
	tar xjf $(TOOLCHAIN_DIR)/$(TOOLCHAIN_RUNTIME_LIB)/$(TOOLCHAIN_RUNTIME_LIB_C) -C $(SYSDRV_DIR_OUT_ROOTFS)
	pushd $(SYSDRV_DIR_OUT_ROOTFS)/lib/ ; mv -fv \
		libasan_preinit.o \
		libasan.so* \
		libtsan.so* \
		libubsan.so* \
		libSegFault.so \
		libmemusage.so* \
		libpcprofile.so* \
		libthread_db*.so* \
		libBrokenLocale* \
		libinproctrace.so \
		$(SYSDRV_DIR_OUT_BOARD); \
	popd

##########################################################################################
#	build busybox
##########################################################################################
busybox_menuconfig:
	@$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) menuconfig \
		-C $(BUSYBOX_DIR)/$(BUSYBOX_VER)/
	@cp -f $(BUSYBOX_DIR)/$(BUSYBOX_VER)/.config \
			$(SYSDRV_DIR)/tools/board/busybox/$(BUSYBOX_CFG);

busybox: prepare
	@echo -e "$(C_GREEN) ==sysdrv== build busybox   $(C_NORMAL)"
	test -f $(BUSYBOX_DIR)/$(BUSYBOX_VER)/_install/bin/busybox  || (\
		rm $(BUSYBOX_DIR)/$(BUSYBOX_VER) -rf ;\
		mkdir -p $(BUSYBOX_DIR) ;\
		tar xjf $(SYSDRV_DIR)/tools/board/busybox/$(BUSYBOX_VER).tar.bz2 -C $(BUSYBOX_DIR) ;\
		cp $(SYSDRV_DIR)/tools/board/busybox/$(BUSYBOX_CFG) $(BUSYBOX_DIR)/$(BUSYBOX_VER)/.config ;\
		cp -fv $(SYSDRV_DIR)/tools/board/busybox/*.patch $(SYSDRV_DIR)/tools/board/busybox/busybox.patch.sh $(BUSYBOX_DIR)/$(BUSYBOX_VER)/ ;\
		pushd $(BUSYBOX_DIR)/$(BUSYBOX_VER)/;$(SHELL) ./busybox.patch.sh ; $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -j$(SYSDRV_JOBS) ; popd ;\
		$(MAKE) CROSS_COMPILE=$(CROSS_COMPILE) -C $(BUSYBOX_DIR)/$(BUSYBOX_VER) install ;\
	);
ifeq ($(SYSDRV_BUILD_RAMDISK_FLAG),YES)
	tar xf $(SYSDRV_DIR)/tools/board/rootfs_script.tar -C $(BUSYBOX_DIR)/$(BUSYBOX_VER)/_install
ifneq ($(OUTPUT_SYSDRV_RAMDISK_TINY_ROOTFS_DIR),)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_SYSDRV_RAMDISK_TINY_ROOTFS_DIR), \
		$(SYSDRV_DIR)/tools/board/busybox/init \
		$(BUSYBOX_DIR)/$(BUSYBOX_VER)/_install)
else
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), \
		$(SYSDRV_DIR)/tools/board/busybox/init \
		$(BUSYBOX_DIR)/$(BUSYBOX_VER)/_install)
endif
else
	
	cp -af $(BUSYBOX_DIR)/$(BUSYBOX_VER)/_install/* $(SYSDRV_DIR_OUT_ROOTFS)
	# luckfox
	cp $(SYSDRV_DIR)/tools/board/android-tools/S90usb0config $(SYSDRV_DIR_OUT_ROOTFS)/etc/init.d
	touch $(SYSDRV_DIR_OUT_ROOTFS)/userdata/.busybox

endif

busybox_clean:
	$(AT)rm -rf $(BUSYBOX_DIR)/$(BUSYBOX_VER)

##########################################################################################
#   build buildroot
#   author :luckfox team
#	date:2023-11-11
##########################################################################################
buildroot_menuconfig:
	$(MAKE) menuconfig -C $(BUILDROOT_DIR)/$(BUILDROOT_VER) 
	$(MAKE) source -C $(BUILDROOT_DIR)/$(BUILDROOT_VER) 
	
buildroot_savedefconfig:
	$(MAKE) savedefconfig -C $(BUILDROOT_DIR)/$(BUILDROOT_VER) 
    
buildroot: prepare
	@echo -e "$(C_GREEN) ==sysdrv== build buildroot   $(C_NORMAL)" 
	test -f $(BUILDROOT_DIR)/$(BUILDROOT_VER)/output/target/bin/busybox  || (\
		rm $(BUILDROOT_DIR)/$(BUILDROOT_VER) -rf ;\
		mkdir -p $(BUILDROOT_DIR) ;\
		tar xzf $(SYSDRV_DIR)/tools/board/buildroot/$(BUILDROOT_VER).tar.gz -C $(BUILDROOT_DIR) ;\
		cp $(SYSDRV_DIR)/tools/board/buildroot/luckfox_pico_defconfig $(BUILDROOT_DIR)/$(BUILDROOT_VER)/configs/ ;\
		cp $(SYSDRV_DIR)/tools/board/buildroot/busybox.config $(BUILDROOT_DIR)/$(BUILDROOT_VER)/package/busybox/ ;\
	);

	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $(BUILDROOT_DEFCONFIG) -C $(BUILDROOT_DIR)/$(BUILDROOT_VER) 
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) source -C $(BUILDROOT_DIR)/$(BUILDROOT_VER) 
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -j$(SYSDRV_JOBS) -C $(BUILDROOT_DIR)/$(BUILDROOT_VER) 

ifeq ($(SYSDRV_BUILD_RAMDISK_FLAG),YES)
    tar xf $(SYSDRV_DIR)/tools/board/rootfs_script.tar -C $(BUILDROOT_DIR)/$(BUILDROOT_VER)/output/target
	ifneq ($(OUTPUT_SYSDRV_RAMDISK_TINY_ROOTFS_DIR),)
		$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_SYSDRV_RAMDISK_TINY_ROOTFS_DIR), \
			$(SYSDRV_DIR)/tools/board/buildroot/busybox/init \
			$(BUILDROOT_DIR)/$(BUILDROOT_VER)/output/target)
	else
		$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), \
			$(SYSDRV_DIR)/tools/board/buildroot/busybox/init \
			$(BUILDROOT_DIR)/$(BUILDROOT_VER)/output/target)
	endif
else
	tar xf $(BUILDROOT_DIR)/$(BUILDROOT_VER)/output/images/rootfs.tar -C $(SYSDRV_DIR_OUT_ROOTFS)
	cp $(SYSDRV_DIR)/tools/board/buildroot/shadow_defconfig $(SYSDRV_DIR_OUT_ROOTFS)/etc/shadow
	cp $(SYSDRV_DIR)/tools/board/buildroot/sshd_defconfig $(SYSDRV_DIR_OUT_ROOTFS)/etc/ssh/sshd_config
	cp $(SYSDRV_DIR)/tools/board/buildroot/samba_defconfig $(SYSDRV_DIR_OUT_ROOTFS)/etc/samba/smb.conf
	cp $(SYSDRV_DIR)/tools/board/buildroot/smbpasswd_defconfig $(SYSDRV_DIR_OUT_ROOTFS)/etc/samba/smbpasswd
	cp $(SYSDRV_DIR)/tools/board/buildroot/S50sshd $(SYSDRV_DIR_OUT_ROOTFS)/etc/init.d/
	cp $(SYSDRV_DIR)/tools/board/buildroot/S99python $(SYSDRV_DIR_OUT_ROOTFS)/etc/init.d/
	cp $(SYSDRV_DIR)/tools/board/android-tools/S90usb0config $(SYSDRV_DIR_OUT_ROOTFS)/etc/init.d/
	
endif

buildroot_clean:
	$(AT)rm -rf $(BUILDROOT_DIR)/$(BUILDROOT_VER) 

##########################################################################################
#   build ubuntu
#   author :luckfox team
#	date:2023-11-28
##########################################################################################
ubuntu: prepare
	@echo -e "$(C_GREEN) ==sysdrv== build ubuntu   $(C_NORMAL)" 
	echo "====$(LF_TARGET_ROOTFS)===="
	test -f $(SYSDRV_DIR)/tools/board/ubuntu/$(UBUNTU_VER).tar.gz || (\
		pushd  $(SYSDRV_DIR)/tools/board/ubuntu/ ;\
		./split_and_check_md5.sh merge ./$(UBUNTU_VER).tar.gz ;\
		popd ;\
	);
	test -f $(SYSDRV_DIR_OUT_ROOTFS)/userdate/.ubuntu || (\
		mkdir -p $(SYSDRV_DIR_OUT_ROOTFS) ;\
		rm $(SYSDRV_DIR_OUT_ROOTFS)/* -rf ;\
		tar xzf $(SYSDRV_DIR)/tools/board/ubuntu/$(UBUNTU_VER).tar.gz -C $(SYSDRV_DIR_OUT_ROOTFS) ;\
	);
ubuntu_clean:
	$(AT)rm -rf $(SYSDRV_DIR_OUT_ROOTFS)

##########################################################################################
#	build pc tools
##########################################################################################
include $(SYSDRV_DIR)/tools/pc/Makefile.tools.pc.mk
pctools: prepare tools_pc-builds
	@echo -e "$(C_GREEN) ==sysdrv== build tools which run on pc  $(C_NORMAL)"
	-chmod a+x $(SYSDRV_DIR_OUT_PC)/*
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_ROOTFS_TOOLS_DIR), $(SYSDRV_DIR_OUT_PC), SELF)

pctools_clean:
	@echo -e "$(C_GREEN) ==sysdrv== clean tools which run on pc  $(C_NORMAL)"

##########################################################################################
#	build board tools
##########################################################################################
include $(SYSDRV_DIR)/tools/board/Makefile.tools.board.mk
boardtools: tools_board-builds
	@echo -e "$(C_GREEN) ==sysdrv== build tools which run on board   $(C_NORMAL)"

boardtools_clean: tools_board-clean
	@echo -e "$(C_GREEN) ==sysdrv== build $@ done$(C_NORMAL)"

##########################################################################################
#	build driver ko
##########################################################################################
drv: prepare
	@echo -e "$(C_GREEN) ==sysdrv== build drv $(C_NORMAL)"
	@-rm -rf $(SYSDRV_KERNEL_MOD_PATH) $(KERNEL_DIR_DRV_KO)
	@-mkdir -p $(SYSDRV_KERNEL_MOD_PATH)
	$(MAKE) -C $(KERNEL_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -j$(SYSDRV_JOBS) \
		modules_install INSTALL_MOD_STRIP=1 \
		INSTALL_MOD_PATH=$(KERNEL_DIR_DRV_KO)
	$(AT)find $(KERNEL_DIR_DRV_KO)/lib/modules/*/kernel \
		-name "*.ko" -type f -exec cp -fav {} $(SYSDRV_KERNEL_MOD_PATH) \;
	@test ! -d drv_ko || ($(MAKE) -C $(SYSDRV_DIR)/drv_ko KERNEL_DRV_KO_INSTALL_PATH=$(SYSDRV_KERNEL_MOD_PATH))
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_ROOTFS_SOURCE_DIR), $(SYSDRV_KERNEL_MOD_PATH), SELF)

drv_clean:
	@test ! -d drv_ko || $(MAKE) -C $(SYSDRV_DIR)/drv_ko clean
	@-rm -rf $(SYSDRV_KERNEL_MOD_PATH) $(KERNEL_DIR_DRV_KO)
	@echo -e "$(C_GREEN) ==sysdrv== clean drv $(C_NORMAL)"

##########################################################################################
#	build rootfs
##########################################################################################
rootfs: $(ROOTFS_BUILD_ENV)
	@echo -e "$(C_GREEN) ==sysdrv== build rootfs  $(C_NORMAL)"
	make -C $(SYSDRV_DIR) strip
	pushd $(SYSDRV_DIR)/out;tar cf $(OUT_ROOTFS).tar $(OUT_ROOTFS);popd
	@# RK_PARTITION_FS_TYPE_CFG is defined outside of sysdrv,
	@# so if defined RK_PARTITION_FS_TYPE_CFG, we ignore SYSDRV_ROOTFS_BUILD_TARGET
	@test "$(ROOTFS_PART_SIZE)" = "FAIL" || \
		test "$(RK_PARTITION_FS_TYPE_CFG)x" != "x" || \
		make -C $(SYSDRV_DIR) $(SYSDRV_ROOTFS_BUILD_TARGET)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_ROOTFS_SOURCE_DIR), $(SYSDRV_DIR)/out/$(OUT_ROOTFS).tar )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(OUTPUT_ROOTFS_TOOLS_DIR), $(SYSDRV_DIR)/out/bin )
	@echo -e "$(C_GREEN) ==sysdrv== finish sysdrv build  $(C_NORMAL)"

strip:
ifeq ($(SYSDRV_BUILD_VERSION),RELEASE)
	@echo -e "$(C_GREEN) ==sysdrv== strip files of rootfs  $(C_NORMAL)"
	$(call MAROC_STRIP_DEBUG_SYMBOL, $(SYSDRV_DIR_OUT_ROOTFS))
endif

rootfs_ubi:
	@echo -e "$(C_GREEN) ==sysdrv== build ubi rootfs  $(C_NORMAL)"
	# build rootfs as follow:
	# the pagesize = 2k, blocksize = 128k, part_size = 32M
	# the pagesize = 2k, blocksize = 256k, part_size = 32M
	# the pagesize = 4k, blocksize = 256k, part_size = 32M
	# command format:
	# $(UBI_TOOL) <source dir> <output dir> <partition size> <partition name> <fs type> <fs comp>"
	$(SYSDRV_DIR_OUT_PC)/$(UBI_TOOL) $(SYSDRV_DIR_OUT_ROOTFS) $(SYSDRV_DIR_OUT_IMAGE) $(ROOTFS_PART_SIZE) rootfs $(SYSDRV_ROOTFS_FS_TYPE) $(SYSDRV_ROOTFS_FS_COMP)

rootfs_jffs2:
	@echo -e "$(C_GREEN) ==sysdrv== build jffs2 rootfs  $(C_NORMAL)"
	# use for spi nor
	$(SYSDRV_DIR_OUT_PC)/$(JFFS2_TOOL) $(SYSDRV_DIR_OUT_ROOTFS) $(SYSDRV_ROOTFS_OUT_IMAGE) $(ROOTFS_PART_SIZE)

rootfs_squashfs:
	@echo -e "$(C_GREEN) ==sysdrv== build squashfs rootfs  $(C_NORMAL)"
	# SYSDRV_ROOTFS_FS_COMP support lz4|lzo|lzma|xz|gzip (default gzip)
	$(SYSDRV_DIR_OUT_PC)/$(SQUASHFS_TOOL) $(SYSDRV_DIR_OUT_ROOTFS) $(SYSDRV_ROOTFS_OUT_IMAGE) $(SYSDRV_ROOTFS_FS_COMP)

rootfs_erofs:
	@echo -e "$(C_GREEN) ==sysdrv== build erofs rootfs  $(C_NORMAL)"
	# SYSDRV_ROOTFS_FS_COMP support lz4|lz4hc (default lz4hc)
	$(SYSDRV_DIR_OUT_PC)/$(EROFS_TOOL) $(SYSDRV_DIR_OUT_ROOTFS) $(SYSDRV_ROOTFS_OUT_IMAGE) $(SYSDRV_ROOTFS_FS_COMP)

rootfs_ext4:
	@echo -e "$(C_GREEN) ==sysdrv== build ext4 rootfs  $(C_NORMAL)"
	$(SYSDRV_DIR_OUT_PC)/$(EXT4_TOOL) $(SYSDRV_DIR_OUT_ROOTFS) $(SYSDRV_ROOTFS_OUT_IMAGE) $(ROOTFS_PART_SIZE)

rootfs_initramfs:
	@echo -e "$(C_GREEN) ==sysdrv== build initramfs rootfs  $(C_NORMAL)"
	$(SYSDRV_DIR_OUT_PC)/$(INITRAMFS_TOOL) $(SYSDRV_DIR_OUT_ROOTFS) $(SYSDRV_ROOTFS_OUT_IMAGE)

rootfs_clean:
	$(AT)rm $(SYSDRV_DIR_OUT_ROOTFS)/ -rf

##########################################################################################
#	build env.img
##########################################################################################
env: prepare
	@$(MAKE) -C $(SYSDRV_DIR)/tools/pc/uboot_tools
ifeq ($(BOOT_MEDIUM),emmc)
	@echo "blkdevparts=mmcblk0:$(CONFIG_SYSDRV_PARTITION)" > $(SYSDRV_UBOOT_ENV_CFG_FILE)
	@test "$(ROOTFS_PART_NUM)" = "FAIL" || \
		echo "sys_bootargs=root=/dev/mmcblk0p$(ROOTFS_PART_NUM)" >> $(SYSDRV_UBOOT_ENV_CFG_FILE)
endif
ifeq ($(BOOT_MEDIUM),spi_nor)
	@echo "mtdparts=sfc_nor:$(CONFIG_SYSDRV_PARTITION)" > $(SYSDRV_UBOOT_ENV_CFG_FILE)
	@test "$(ROOTFS_PART_NUM)" = "FAIL" || \
		echo "sys_bootargs=root=/dev/mtdblock$(ROOTFS_PART_NUM)" >> $(SYSDRV_UBOOT_ENV_CFG_FILE)
endif
ifneq ($(findstring $(BOOT_MEDIUM),spi_nand slc_nand),)
	@echo "mtdparts=rk-nand:$(CONFIG_SYSDRV_PARTITION)" > $(SYSDRV_UBOOT_ENV_CFG_FILE)
	@test "$(ROOTFS_PART_NUM)" = "FAIL" || \
		echo "sys_bootargs=root=ubi.mtd=$(ROOTFS_PART_NUM)" >> $(SYSDRV_UBOOT_ENV_CFG_FILE)
endif
ifeq ($(findstring FAIL,$(ENV_PART_SIZE)),)
	$(SYSDRV_DIR_OUT_PC)/mkenvimage -s $(ENV_PART_SIZE) -p 0x0 -o $(SYSDRV_DIR_OUT_IMAGE)/env.img $(SYSDRV_UBOOT_ENV_CFG_FILE)
endif

##########################################################################################
#	clean out
##########################################################################################
out_clean:
	@rm $(SYSDRV_DIR)/out -rf

# printvars prints all the variables currently defined in our
# Makefiles. Alternatively, if a non-empty VARS variable is passed,
# only the variables matching the make pattern passed in VARS are
# displayed.
.PHONY: printvars
printvars:
	@:$(foreach V, \
		$(sort $(if $(VARS),$(filter $(VARS),$(.VARIABLES)),$(.VARIABLES))), \
		$(if $(filter-out environment% default automatic, \
				$(origin $V)), \
		$(if $(QUOTED_VARS),\
			$(info $V='$(subst ','\'',$(if $(RAW_VARS),$(value $V),$($V)))'), \
			$(info $V=$(if $(RAW_VARS),$(value $V),$($V))))))
