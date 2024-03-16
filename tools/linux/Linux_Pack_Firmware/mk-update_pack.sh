#!/bin/bash

cmd=`realpath $0`
cwd=`dirname $cmd`
CMD_VERSION=1.0.1

C_BLACK="\e[30;1m"
C_RED="\e[31;1m"
C_GREEN="\e[32;1m"
C_YELLOW="\e[33;1m"
C_BLUE="\e[34;1m"
C_PURPLE="\e[35;1m"
C_CYAN="\e[36;1m"
C_WHITE="\e[37;1m"
C_NORMAL="\033[0m"

function msg_info()
{
	echo -e "${C_GREEN}[$(basename $0):info] $1${C_NORMAL}"
}

function msg_warn()
{
	echo -e "${C_YELLOW}[$(basename $0):warn] $1${C_NORMAL}"
}

function msg_error()
{
	echo -e "${C_RED}[$(basename $0):error] $1${C_NORMAL}"
}

function pause()
{
	msg_error "package update.img fail..."
	msg_error "Press any key to quit:"
	read -n1 -t10 -s key
	exit -1
}

function msg_help()
{
	echo "command format:"
	echo "    `basename $cmd` -v  -h  -i  -id  -p  -t"
	echo "        -v|--version      (Optional) view this version."
	echo "        -h|--help         (Optional) view this help message."
	echo "       -id|--chip-id      (Essential) input chip id."
	echo "        -i|--image        (Essential) input firmware image path."
	echo "        -p|--parameter    (Optional) the partition file path (Not added by default)."
}

function chk_param()
{
	local cnt
	cnt=0
	while [ $# -ne 0 ]
	do
		case $1 in
			-v|--version)
				msg_info "V$CMD_VERSION"
				exit 0
				;;
			-h|--help)
				msg_help
				exit 0
				;;
			-i|--image)
				IMAGE_PATH=$2
				cnt=$(( cnt + 1 ))
				;;
			-id|--chip-id)
				CHIP_ID=$2
				case $CHIP_ID in
				rv1126|rv1109|RV1126|RV1109)
					CHIP_ID=-RK1126
					;;
				rk3588|RK3588)
					CHIP_ID=-RK3588
					;;
				rv1106|rv1103|RV1106|RV1103)
					CHIP_ID=-RK1106
					;;
				*)
					msg_error "Invalid paramter: -id $CHIP_ID"
					exit -1
					;;
				esac
				cnt=$(( cnt + 1 ))
				;;
			-p|--parameter)
				partition_file=$2
				;;
			*)
				;;
		esac
		shift
	done

	if [ $cnt -ne 2 -o -z "$IMAGE_PATH" -o -z "$CHIP_ID" ]; then
		msg_error "Invalid parameter..."
		msg_help
		exit -1
	fi
	msg_info "image input dir: $IMAGE_PATH"

	if [ -f "$partition_file" ]; then
		PARTITION_FILE=`basename $partition_file`
		return
	fi

	PARTITION_FILE=$partition_file
	if [ -f "$IMAGE_PATH/$PARTITION_FILE" ]; then
		return
	fi

	if [ -f "$IMAGE_PATH/env.img" ]; then
		PARTITION_FILE=env.img
		return
	else
		msg_error "Not found file named $PARTITION_FILE in the dir $IMAGE_PATH"
		exit -1
	fi
}

function get_partition_names()
{
	local part_file part_lines boot_mediums

	part_file=$IMAGE_PATH/$PARTITION_FILE
	boot_mediums="blkdevparts=mmcblk0,blkdevparts=mmcblk1,mtdparts=sfc_nor,mtdparts=spi-nand0,mtdparts=rk-nand"

	IFS=,
	for medium in $boot_mediums;
	do
		part_lines=$(strings $part_file | grep -i $medium)
		[ ! -z "$part_lines" ] && part_lines=${part_lines##*${medium}:} && break
	done
	IFS=

	[ -z "$part_lines" ] && msg_error "Not found partition line in $part_file" && msg_help && exit -1

	IFS=,
	local part_name part_size
	for part in $part_lines;
	do
		part_size=`echo $part | cut -d '@' -f1`
		part_name=`echo $part | cut -d '(' -f2|cut -d ')' -f1`

		[[ $part_size =~ "-" ]] && continue
		PARTITIONS="$part_name,$PARTITIONS"
	done
	IFS=

	if [ -z "$PARTITIONS" ];then
		msg_error "Invalid partition..."
		exit -1
	fi
	# msg_info "partitions:\n$PARTITIONS"
}

function __package_file()
{
	if [ -f "$IMAGE_PATH/$1" ];then
		PACKAGE_CFG="$part	$1\n$PACKAGE_CFG"
	elif [ -f "$IMAGE_PATH/${1}.img" ];then
		PACKAGE_CFG="$part	${1}.img\n$PACKAGE_CFG"
	elif [ -f "$IMAGE_PATH/${1%_[ab]}.img" ];then
		PACKAGE_CFG="$part	${1%_[ab]}.img\n$PACKAGE_CFG"
	else
		msg_error "Not found image file: $IMAGE_PATH/$1"
		[ "$2" = "Essential" ] && exit -1
		return 1
	fi
}

function create_package_file()
{
	IFS=,
	for part in $SPECIAL_PARTITIONS $PARTITIONS;
	do
		case $part in
			package-file)
				PACKAGE_CFG="$part	package-file\n$PACKAGE_CFG"
				;;
			bootloader)
				__package_file download.bin Essential
				;;
			parameter)
				__package_file $PARTITION_FILE Essential
				;;
			rootfs)
				__package_file $part
				[ $? -eq 1 ] && __package_file rootfs_base.img
				;;
			*)
				__package_file $part
				;;
		esac
	done
	IFS=

	msg_info "partitions:\n$PACKAGE_CFG"
	echo -e $PACKAGE_CFG > $IMAGE_PATH/package-file
}

CHIP_ID=""
IMAGE_PATH=""
PARTITION_FILE=""
SPECIAL_PARTITIONS="bootloader,package-file"
PARTITIONS=""
PACKAGE_CFG=""

chk_param $@

msg_info "partition file name: $PARTITION_FILE"
msg_info "start to package update.img..."

get_partition_names

create_package_file

echo "TOOL_PATH=$cwd"

$cwd/afptool -pack $IMAGE_PATH $IMAGE_PATH/update_tmp.img || pause

$cwd/rkImageMaker $CHIP_ID $IMAGE_PATH/download.bin $IMAGE_PATH/update_tmp.img  $IMAGE_PATH/update.img -os_type:androidos || pause

msg_info "Making $CHIP_ID update.img OK."

# remove temporay image
rm -f $IMAGE_PATH/update_tmp.img
rm -f $IMAGE_PATH/package-file
exit $?
