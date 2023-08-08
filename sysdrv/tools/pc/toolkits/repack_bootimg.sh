#!/bin/bash
#
# SET ANSI COLORS {{{ START
C_RED="[1;31m"
C_CYAN="[1;36m"
C_BLUE="[1;34m"
C_GREEN="[1;32m"
C_WHITE="[1;37m"
C_YELLOW="[1;33m"
C_MAGENTA="[1;35m"
C_NORMAL="[0;39m"
# SET ANSI COLORS END }}}

set -e
cmd=`realpath $0`
cwd=`dirname $cmd`
CMD_VERSION=1.0.0

function msg_dbg()
{
	if [ "$RK_DEBUG" != "1" ]; then
		return
	fi
	echo -e "${C_CYAN}[$(basename $0):info] $*${C_NORMAL}" >&2
}

function msg_info()
{
	echo -e "${C_GREEN}[$(basename $0):info] $1${C_NORMAL}" >&2
}

function msg_warn()
{
	echo -e "${C_YELLOW}[$(basename $0):warn] $1${C_NORMAL}" >&2
}

function msg_error()
{
	echo -e "${C_RED}[$(basename $0):error] $1${C_NORMAL}" >&2
}

function msg_help()
{
	echo "command format:"
	echo "    `basename $cmd` -v  -h  --kernel  --resource  --its  --dtb  --output --arch"
	echo "        -v|--version      (Optional) view this version."
	echo "        -h|--help         (Optional) view this help message."
	echo "        --kernel          (MUST) input kernel's image path."
	echo "        --resource        (MUST) input resource.img path."
	echo "        --arch            (MUST) kernel's arch (arm or arm64)."
	echo "        --its             (MUST) input its file path."
	echo "        --dtb             (MUST) input dtb file path."
	echo "        --output          (MUST) output file path."
	return
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
			--arch)
				PARAM_ARCH=$2
				cnt=$(( cnt + 1 ))
				;;
			--output)
				PARAM_OUTPUT_PATH=$2
				cnt=$(( cnt + 1 ))
				;;
			--dtb)
				PARAM_DTB_PATH=$2
				cnt=$(( cnt + 1 ))
				;;
			--kernel)
				PARAM_KERNEL_IMAGE=$2
				cnt=$(( cnt + 1 ))
				;;
			--resource)
				PARAM_RESOURCE_IMAGE=$2
				cnt=$(( cnt + 1 ))
				;;
			--its)
				PARAM_ITS=$2
				cnt=$(( cnt + 1 ))
				;;
			*)
				;;
		esac
		shift
	done

	if [ $cnt -ne 6 \
		-o -z "$PARAM_ARCH" \
		-o -z "$PARAM_OUTPUT_PATH" \
		-o ! -f "$PARAM_ITS" \
		-o ! -f "$PARAM_DTB_PATH" \
		-o ! -f "$PARAM_RESOURCE_IMAGE" \
		-o ! -f "$PARAM_KERNEL_IMAGE" ]; then
		msg_error "Invalid parameter..."
		if [ -z "$PARAM_OUTPUT_PATH" ];then
			msg_error "Not config PARAM_OUTPUT_PATH"
		fi
		if [ -z "$PARAM_ARCH" ];then
			msg_error "Not config PARAM_ARCH"
		fi
		if [ ! -f "$PARAM_ITS" ];then
			msg_error "Not found PARAM_ITS file: $PARAM_ITS"
		fi
		if [ ! -f "$PARAM_DTB_PATH" ];then
			msg_error "Not found PARAM_DTB_PATH file: $PARAM_DTB_PATH"
		fi
		if [ ! -f "$PARAM_RESOURCE_IMAGE" ];then
			msg_error "Not found PARAM_RESOURCE_IMAGE file: $PARAM_RESOURCE_IMAGE"
		fi
		if [ ! -f "$PARAM_KERNEL_IMAGE" ];then
			msg_error "Not found PARAM_KERNEL_IMAGE file: $PARAM_KERNEL_IMAGE"
		fi
		msg_help
		exit -1
	fi

}

check_mkimage()
{
	MKIMAGE=${cwd}/mkimage
	MKIMAGE=$(type -p ${MKIMAGE} || true)
	if [ -z "${MKIMAGE}" ]; then
		MKIMAGE=mkimage
		MKIMAGE=$(type -p ${MKIMAGE} || true)
		if [ -n "${MKIMAGE}" ]; then
			msg_info "Found $MKIMAGE"
			return
		else
			# Doesn't exist
			msg_error '"mkimage" command not found - U-Boot images will not be built' >&2
			exit 1;
		fi
	fi
}
################################################################################
# Start
################################################################################
dtc --version &>/dev/null
if [ "$?" -ne 0 ]; then
	msg_error "No found dtc tool, please install it. (run \"sudo apt-get install device-tree-compile\")"
	return
fi

PARAM_ITS=""
PARAM_ARCH=""
PARAM_DTB_PATH=""
PARAM_RESOURCE_IMAGE=""
PARAM_KERNEL_IMAGE=""
PARAM_OUTPUT_PATH=""

check_mkimage
chk_param $@

msg_info "PARAM_ITS = $PARAM_ITS"
msg_info "PARAM_ARCH = $PARAM_ARCH"
msg_info "PARAM_DTB_PATH = $PARAM_DTB_PATH"
msg_info "PARAM_OUTPUT_PATH = $PARAM_OUTPUT_PATH"
msg_info "PARAM_KERNEL_IMAGE = $PARAM_KERNEL_IMAGE"
msg_info "PARAM_RESOURCE_IMAGE = $PARAM_RESOURCE_IMAGE"

__workdir=$(dirname $PARAM_OUTPUT_PATH)/.pack_bootimg_workdir
mkdir -p $__workdir
cp -f $PARAM_ITS $__workdir/boot.its
cp -f $PARAM_DTB_PATH $__workdir/fdt
cp -f $PARAM_RESOURCE_IMAGE $__workdir/resource
cp -f $PARAM_KERNEL_IMAGE $__workdir/kernel

if [ "${PARAM_ARCH}" == "arm64" ]; then
	sed -i -e 's/arch = ""/arch = "arm64"/g' -e 's/compression = ""/compression = "lz4"/' $__workdir/boot.its
else
	sed -i -e 's/arch = ""/arch = "arm"/g' -e 's/compression = ""/compression = "none"/' $__workdir/boot.its
fi

__MKIMAGE_ARG="-E -p 0x800"
${MKIMAGE} ${__MKIMAGE_ARG} -f $__workdir/boot.its $PARAM_OUTPUT_PATH
mkdir -p $(dirname $PARAM_OUTPUT_PATH)
msg_info "Repack $PARAM_OUTPUT_PATH ok !!!"
if [ "$RK_DEBUG" != "1" ]; then
	rm -rf $__workdir
fi
