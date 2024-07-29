#!/bin/bash

COMMON_DIR=$(cd "$(dirname "$(realpath "$0")")/.."; pwd)
SDK_DIR=$(realpath $COMMON_DIR)
UPGRADETOOL=$SDK_DIR/tools/linux/Linux_Upgrade_Tool/upgrade_tool
ROCKDEV_DIR=$SDK_DIR/output/image
LOADER=$ROCKDEV_DIR/download.bin
PARAMETER=$ROCKDEV_DIR/parameter.txt
UBOOT=$ROCKDEV_DIR/uboot.img
TRUST=$ROCKDEV_DIR/trust.img
BOOT=$ROCKDEV_DIR/boot.img
RECOVERY=$ROCKDEV_DIR/recovery.img
OEM=$ROCKDEV_DIR/oem.img
MISC=$ROCKDEV_DIR/misc.img
ROOTFS=$ROCKDEV_DIR/rootfs.img
USERDATA=$ROCKDEV_DIR/userdata.img
UPDATE=$ROCKDEV_DIR/update.img

if [ ! -n "$1" ];then
echo "flash all images as default"
FLASH_TYPE=all
else
FLASH_TYPE="$1"
fi

if [ $FLASH_TYPE = tb ]
then
	$UPGRADETOOL ul -noreset $LOADER
	$UPGRADETOOL di -p $PARAMETER
	$UPGRADETOOL di -uboot $UBOOT
	$UPGRADETOOL di -b $BOOT
	$UPGRADETOOL rd
fi

if [ $FLASH_TYPE = loader ]
then
	if [ -n "$2" ];then
		LOADER=$2
	fi
	echo "flash loader: $LOADER"
	$UPGRADETOOL ul $LOADER
	exit 0
fi


if [ $FLASH_TYPE = uboot ]
then
	if [ -n "$2" ];then
		UBOOT=$2
	fi
	echo "flash uboot: $UBOOT"
	$UPGRADETOOL di -uboot $UBOOT
fi

if [ $FLASH_TYPE = boot ]
then
	if [ -n "$2" ];then
		BOOT=$2
	fi
	echo "flash boot: $BOOT"
	$UPGRADETOOL di -b $BOOT
	$UPGRADETOOL rd

fi

if [ $FLASH_TYPE = oem ]
then
	if [ -n "$2" ];then
		OEM=$2
	fi
	echo "flash oem: $OEM"
	$UPGRADETOOL di -oem $OEM
fi

if [ $FLASH_TYPE = userdata ]
then
	if [ -n "$2" ];then
		USERDATA=$2
	fi
	echo "flash userdata: $USERDATA"
	$UPGRADETOOL di -userdata $USERDATA
fi

if [ $FLASH_TYPE = rootfs ]
then
	if [ -n "$2" ];then
		ROOTFS=$2
	fi
	echo "flash rootfs: $ROOTFS"
	$UPGRADETOOL di -rootfs $ROOTFS
fi

if [ $FLASH_TYPE = update ]
then
	$UPGRADETOOL uf $UPDATE
fi

if [ $FLASH_TYPE = rd ]
then
	$UPGRADETOOL rd
fi

if [ $FLASH_TYPE = erase ]
then
	$UPGRADETOOL EF $LOADER
fi

