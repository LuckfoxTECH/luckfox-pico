#!/bin/bash

#################################################
# 	Board Config
#################################################
export LF_ORIGIN_BOARD_CONFIG=BoardConfig-EMMC-Buildroot-RV1106_Luckfox_Pico_Ultra-IPC_FASTBOOT.mk

# Target CHIP
export RK_CHIP=rv1106

# app config
export RK_APP_TYPE="RK_FASTBOOT_SERVER SMART_DOOR_RV1106_NORMAL"

# Config CMA size in environment
export RK_BOOTARGS_CMA_SIZE="38M"

# Kernel dts
export RK_KERNEL_DTS=rv1106g-luckfox-pico-ultra-fastboot.dts

#################################################
#	BOOT_MEDIUM
#################################################

# Target boot medium
export RK_BOOT_MEDIUM=emmc

# Uboot defconfig fragment
export RK_UBOOT_DEFCONFIG_FRAGMENT=rk-emmc.config

# specify post.sh for delete/overlay files
# export RK_PRE_BUILD_OEM_SCRIPT=rv1103-spi_nor-post.sh

# config partition in environment
# RK_PARTITION_CMD_IN_ENV format:
#     <partdef>[,<partdef>]
#       <partdef> := <size>[@<offset>](part-name)
# Note:
#   If the first partition offset is not 0x0, it must be added. Otherwise, it needn't adding.
export RK_PARTITION_CMD_IN_ENV="32K(env),256K@32K(idblock),256K(uboot),384K(meta),36M(boot),3G(userdata)"

# config partition's filesystem type (squashfs is readonly)
# emmc:    squashfs/ext4
# nand:    squashfs/ubifs
# spi nor: squashfs/jffs2
# RK_PARTITION_FS_TYPE_CFG format:
#     AAAA:/BBBB/CCCC@ext4
#         AAAA ----------> partition name
#         /BBBB/CCCC ----> partition mount point
#         ext4 ----------> partition filesystem type
export RK_PARTITION_FS_TYPE_CFG=boot@IGNORE@erofs,userdata@/userdata@ext4

# config filesystem compress (Just for squashfs or ubifs)
# squashfs: lz4/lzo/lzma/xz/gzip, default xz
# ubifs:    lzo/zlib, default lzo
# export RK_SQUASHFS_COMP=xz
# export RK_UBIFS_COMP=lzo

#################################################
#	TARGET_ROOTFS
#################################################

# Target rootfs
export LF_TARGET_ROOTFS=busybox

# Buildroot defconfig
#export RK_BUILDROOT_DEFCONFIG=luckfox_pico_w_defconfig

#################################################
# 	Defconfig
#################################################

# Target arch
export RK_ARCH=arm

# Target Toolchain Cross Compile
export RK_TOOLCHAIN_CROSS=arm-rockchip830-linux-uclibcgnueabihf

#misc image
export RK_MISC=wipe_all-misc.img

# Uboot defconfig
export RK_UBOOT_DEFCONFIG=luckfox_rv1106_uboot_emmc_tb_defconfig

# Uboot compile rkbin ini file path overlay
# export RK_UBOOT_RKBIN_INI_OVERLAY=RKBOOT/RV1106MINIALL_TB_SC3336_SC3338.ini
# build.sh mcu info --> rv1106_evb-SC3338-ADC
export RK_UBOOT_RKBIN_MCU_CFG=rv1106_evb-SC3338-ADC

# Kernel defconfig
export RK_KERNEL_DEFCONFIG=luckfox_rv1106_linux_tb_defconfig

# Kernel defconfig fragment
export RK_KERNEL_DEFCONFIG_FRAGMENT="luckfox_rv1106-tb.config rv1106-smart-door.config rv1106-rndis.config"

# Recovery defconfig fragment
export RK_RECOVERY_KERNEL_DEFCONFIG_FRAGMENT="rv1106-tb.config rv1106-recovery.config"

# Config sensor IQ files
# RK_CAMERA_SENSOR_IQFILES format:
#     "iqfile1 iqfile2 iqfile3 ..."
# ./build.sh media and copy <SDK root dir>/output/out/media_out/isp_iqfiles/$RK_CAMERA_SENSOR_IQFILES
export RK_CAMERA_SENSOR_IQFILES="sc3338_FKO1_30IRC-F16.bin"

# Config sensor lens CAC calibrattion bin files
#export RK_CAMERA_SENSOR_CAC_BIN="CAC_sc4336_OT01_40IRC_F16 CAC_sc530ai_CMK-OT2115-PC1_30IRC-F16"

# enable install app to oem partition
export RK_BUILD_APP_TO_OEM_PARTITION=n

# config AI-ISP model
export RK_AIISP_MODEL=NONE

# config AUDIO model
export RK_AUDIO_MODEL=NONE

# config NPU model
export RK_NPU_MODEL=NONE

# enable rockchip test
#export RK_ENABLE_ROCKCHIP_TEST=y

# enable rockchip wifi
#export RK_ENABLE_WIFI=y

# specify post.sh for delete/overlay files
export RK_POST_BUILD_SCRIPT=luckfox-rv1106-tb-emmc-post.sh

# specify pre-oem.sh for delete/overlay files before packaging oem.img
export RK_PRE_BUILD_OEM_SCRIPT=luckfox-rv1106-tb-pre-oem.sh

# enable fastboot
export RK_ENABLE_FASTBOOT=y

# declare overlay directory
export RK_POST_OVERLAY=overlay-luckfox-fastboot

# enable tiny meta only support cmdline overlay
export RK_TINY_META=n

# set camera features
export RK_META_PARAM=" \
 --rk_cam_mirror_flip=0 \
 --rk_cam_fps=25 \
 --rk_cam_hdr=0 \
 --rk_led_value=20 \
 --rk_night_mode=0 \
"

# fastboot rndis
export RK_ENABLE_RNDIS=y
