#!/bin/bash

cmd=`realpath $0`
cwd=`dirname $cmd`
CMD_VERSION=0.0.1

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
	msg_error "unpack update.img fail..."
	msg_error "Press any key to quit:"
	read -n1 -t10 -s key
	exit 1
}

function msg_help()
{
	echo "command format:"
	echo "    `basename $cmd` -v  -h  -i  -o"
	echo "        -v|--version      (Optional) view this version."
	echo "        -h|--help         (Optional) view this help message."
	echo "        -i|--image        (Essential) input firmware image path."
	echo "        -o|--output       (Essential) output unpacked image directory."
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
			-o|--output)
				OUTPUT_DIR=$2
				cnt=$(( cnt + 1 ))
				;;
			*)
				;;
		esac
		shift
	done

	if [ $cnt -ne 2 -o -z "$IMAGE_PATH" -o -z "$OUTPUT_DIR" ]; then
		msg_error "Invalid parameter..."
		msg_help
		exit -1
	fi
	msg_info "image input dir: $IMAGE_PATH"
  msg_info "image output dir: $OUTPUT_DIR"
}

IMAGE_PATH=""
OUTPUT_DIR=""

chk_param $@

msg_info "start to unpack update.img..."

$cwd/rkImageMaker -unpack $IMAGE_PATH $OUTPUT_DIR || pause

$cwd/afptool -unpack $OUTPUT_DIR/firmware.img $OUTPUT_DIR || pause

msg_info "Unpacking update.img OK."

# remove temporay image
rm -f $OUTPUT_DIR/firmware.img
rm -f $OUTPUT_DIR/boot.bin
rm -f $OUTPUT_DIR/package-file
exit $?