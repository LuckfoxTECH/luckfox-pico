#!/bin/bash
set -eE

export LC_ALL=C
export LD_LIBRARY_PATH=

function unset_env_config_rk()
{
	local tmp_file=`mktemp`
	env | grep -oh "^RK_.*=" > $tmp_file || true
	source $tmp_file 2>/dev/null || true
	rm -f $tmp_file
}
unset_env_config_rk

################################################################################
# Global Variable Configure
################################################################################
_FDS="\\ \n"
cmd=`realpath $0`
COMMON_DIR=`dirname $cmd`
PROJECT_TOP_DIR=$(realpath $COMMON_DIR/)
SDK_ROOT_DIR=$(realpath $COMMON_DIR/..)

export RK_JOBS=$((`getconf _NPROCESSORS_ONLN` / 2 + 1 ))
export RK_BUILD_VERSION_TYPE=RELEASE

export SDK_ROOT_DIR=$SDK_ROOT_DIR
export RK_PROJECT_OUTPUT=$SDK_ROOT_DIR/output/out
export RK_PROJECT_TOP_DIR=$PROJECT_TOP_DIR
export RK_PROJECT_OUTPUT_IMAGE=$SDK_ROOT_DIR/output/image
export TARGET_PRODUCT_DIR=$SDK_ROOT_DIR/rt-thread/bsp/rockchip/rv1106-mcu/
################################################################################
# Plubic Configure
################################################################################
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

function choose_default_board
{
	RK_TARGET_BOARD_ARRAYtmp=( $(cd ${TARGET_PRODUCT_DIR}/board;find -type d -name "*evb*"| sort) )
	RK_TARGET_BOARD_ARRAY=( "${RK_TARGET_BOARD_ARRAYtmp[@]}" $(cd ${TARGET_PRODUCT_DIR}/board;find -type d -name "*-*-*" | grep -v "evb" | sort));

	if [  -f "${TARGET_PRODUCT_DIR}/rtconfig.h" ]; then
		CONFIGED_BOARD_NAME=$(cd ${TARGET_PRODUCT_DIR};cat rtconfig.h | grep RT_BOARD_NAME)
		CONFIGED_BOARD_NAME=${CONFIGED_BOARD_NAME:23:0-1}
		if [  -d "${TARGET_PRODUCT_DIR}/board/${CONFIGED_BOARD_NAME}" ]; then
			echo "find configed board ${CONFIGED_BOARD_NAME}"
			cd ${TARGET_PRODUCT_DIR}
			cp board/${CONFIGED_BOARD_NAME}/defconfig .config
			scons --useconfig=board/${CONFIGED_BOARD_NAME}/defconfig
			cd ${SDK_ROOT_DIR}
			return
		fi
		echo "not find configed board ${CONFIGED_BOARD_NAME},use default"
	else
		touch "${TARGET_PRODUCT_DIR}/rtconfig.h"
	fi
	cd ${TARGET_PRODUCT_DIR}
	cp board/${RK_TARGET_BOARD_ARRAY[0]}/defconfig .config
	scons --useconfig=board/${RK_TARGET_BOARD_ARRAY[0]}/defconfig
	cd ${SDK_ROOT_DIR}
}

err_handler() {
	ret=$?
	[ "$ret" -eq 0 ] && return

	msg_error "Running ${FUNCNAME[1]} failed!"
	msg_error "exit code $ret from line ${BASH_LINENO[0]}:"
	msg_info "    $BASH_COMMAND"
	exit $ret
}

function finish_build(){
	msg_info "Running ${FUNCNAME[1]} succeeded."
	cd $PROJECT_TOP_DIR
}

function check_config(){
	unset missing
	for var in $@; do
		eval [ \$$var ] && continue

		missing="$missing $var"
	done

	[ -z "$missing" ] && return 0

	msg_info "Skipping ${FUNCNAME[1]} for missing configs: $missing."
	return 1
}

function choose_target_board()
{
	echo
	echo "You're building on Linux"
	echo "Lunch menu...pick a combo:"
	echo "naming rules:[board]-[sensor]-[lightsensor]"
	echo "命名规则:[板级]-[传感器型号]-[光敏类型]"
	echo ""
	count=0;
	for item in ${RK_TARGET_BOARD_ARRAY[@]}
	do
		item=${item:2};
		name=${item};
		board=${item%%-*};
		item=${item#*-};
		sensor=${item%%-*};
		lsensor=${item##*-};
		echo "-----------------------------------------------------"
		echo -e "\e[32;1m$count.${name} \033[0m"
		echo "      Board(板级) :${board}"
		echo "   Sensor(传感器) :${sensor}"
		echo "LightSensor(光敏) :${lsensor}"
		echo "-----------------------------------------------------"
		echo ""
		count=$[ $count+1 ];
	done
	local INDEX
	cd ${TARGET_PRODUCT_DIR}

	INDEX=0
	if [ -n "$boardconfig_lunch_name" ]; then
		for item in "${RK_TARGET_BOARD_ARRAY[@]}"; do
			if echo $item |grep $(basename $boardconfig_lunch_name); then
				break
			fi
			INDEX=$(( INDEX + 1 ))
		done
		if [ $INDEX -gt $(( count - 1 )) ];then
			msg_error "Not found $boardconfig_lunch_name"
			exit 1
		fi
	else
		read -p "Which would you like? [0]: " INDEX
		INDEX=$((${INDEX:-0}))
	fi
	if echo $INDEX | grep -vq [^0-9]; then
		echo "Lunching for ${RK_TARGET_BOARD_ARRAY[$INDEX]:2}"
		cp board/${RK_TARGET_BOARD_ARRAY[$INDEX]}/defconfig .config
		if [ ! -f "${TARGET_PRODUCT_DIR}/rtconfig.h" ]; then
			touch "${TARGET_PRODUCT_DIR}/rtconfig.h"
		fi
		scons --useconfig=board/${RK_TARGET_BOARD_ARRAY[$INDEX]}/defconfig
	else
		echo "Lunching for Default boards ${RK_TARGET_BOARD_ARRAY[0]:2}"
		cp board/${RK_TARGET_BOARD_ARRAY[0]}/defconfig .config
		scons --useconfig=board/${RK_TARGET_BOARD_ARRAY[0]}/defconfig
	fi
	cd ${SDK_ROOT_DIR}
	exit 0
}

function build_select_board()
{
	RK_TARGET_BOARD_ARRAYtmp=( $(cd ${TARGET_PRODUCT_DIR}/board;find -type d -name "*evb*"| sort) )
	RK_TARGET_BOARD_ARRAY=( "${RK_TARGET_BOARD_ARRAYtmp[@]}" $(cd ${TARGET_PRODUCT_DIR}/board;find -type d -name "*-*-*" | grep -v "evb" | sort));
	RK_TARGET_BOARD_ARRAY_LEN=${#RK_TARGET_BOARD_ARRAY[@]}
	if [ $RK_TARGET_BOARD_ARRAY_LEN -eq 0 ]; then
		msg_error "No available Board Config"
		return
	fi

	if [ "$1" = "lunch" ];then
		choose_target_board
	elif [ "$1" = "info" ];then
		local number=0
		for item in "${RK_TARGET_BOARD_ARRAY[@]}"; do
			echo "$number  $(basename ${item})"
			number=$(( number + 1 ))
		done
	fi

}

function unset_board_config_all()
{
	local tmp_file=`mktemp`
	grep -oh "^export.*RK_.*=" `find cfg -name "BoardConfig*.mk"` > $tmp_file
	source $tmp_file
	rm -f $tmp_file
}

function usage()
{
	echo "Usage: build.sh [OPTIONS]"
	echo "Available options:"
	echo "lunch              -Select Board Configure"
	echo "tool               -build tool"
	echo "all                -build rtthread.bin "
	echo "allsave            -build all & firmware & save"
	echo "clean              -clean all"
	echo "menuconfig         -menuconfig config RTThread"
	echo "firmware           -pack all the image we need to boot up system"
	echo "save               -save images, patches, commands used to debug"
	echo "check              -check the environment of building"
	echo "info               -see the current board building information"
	echo ""
	echo "Default option is 'allsave'."
	finish_build
	exit 0
}

function build_get_sdk_version(){
	if [ -f ${SDK_ROOT_DIR}/.repo/manifest.xml ]; then
		local sdk_ver=""
		sdk_ver=`grep "include name" ${SDK_ROOT_DIR}/.repo/manifest.xml | awk -F\" '{print $2}'`
		sdk_ver=`realpath ${SDK_ROOT_DIR}/.repo/manifests/${sdk_ver}`
		echo "Build SDK version: `basename ${sdk_ver}`"
		GLOBAL_SDK_VERSION="`basename ${sdk_ver}`"
	else
		echo "Not found ${SDK_ROOT_DIR}/.repo/manifest.xml [ignore] !!!"
		GLOBAL_SDK_VERSION="NONE"
	fi
}

function build_info(){
	build_get_sdk_version

	echo "Current Building Information:"
	echo "Target Misc config:"
	msg_info "Build ==> $BOARD_INFO"
	echo "`env |grep "^RK_" | grep -v "=$" | sort`"
}

function build_tool(){
	finish_build
}

function build_check(){
	common_product_build_tools="${PROJECT_TOP_DIR}/scripts/build-depend-tools.txt"
	cat $common_product_build_tools 2>/dev/null | while read chk_item
		do
			chk_item=${chk_item###*}
			if [ -z "$chk_item" ]; then
				continue
			fi

			dst=${chk_item%%,*}
			src=${chk_item##*,}
			echo "**************************************"
			if eval $dst &>/dev/null;then
				echo "Check [OK]: $dst"
			else
				echo "Please install ${dst%% *} first"
				echo "    sudo apt-get install $src"
			fi
		done
}

function build_all() {
	echo "============================================"
	choose_default_board

	cd ${SDK_ROOT_DIR}
	scons -C ${SDK_ROOT_DIR}/rt-thread/bsp/rockchip/rv1106-mcu -j${RK_JOBS}

	local _opt
	_opt=""
	if [ -d "$1" ];then
		_opt=$1
	fi
	build_firmware $_opt

	finish_build
}

function build_menuconfig() {
	scons -C ${SDK_ROOT_DIR}/rt-thread/bsp/rockchip/rv1106-mcu --menuconfig
}

function build_clean(){
	choose_default_board
	param="${1:-all}"
	msg_info "clean ${param}"
	case $param in
		all)
			scons -C ${SDK_ROOT_DIR}/rt-thread/bsp/rockchip/rv1106-mcu -c
			rm -rf ${RK_PROJECT_OUTPUT_IMAGE} ${RK_PROJECT_OUTPUT}
			;;
		*)
			msg_warn "clean [$1] not support, ignore"
			;;
	esac

	finish_build
}

function build_mkimg()
{
	finish_build
}

function build_firmware(){
	mkdir -p ${RK_PROJECT_OUTPUT_IMAGE}

	if [ -d "$1" ];then
		cp -rfa ${SDK_ROOT_DIR}/rt-thread/bsp/rockchip/rv1106-mcu/rtthread.bin  $1
	fi
	cp -rfa ${SDK_ROOT_DIR}/rt-thread/bsp/rockchip/rv1106-mcu/rtthread.bin  ${RK_PROJECT_OUTPUT_IMAGE}
	finish_build
}

function __GET_REPO_INFO()
{
	local repo_tool _date _stub_path _stub_patch_path

	_date=$1
	_stub_path=$2
	_stub_patch_path=$3

	repo_tool="$SDK_ROOT_DIR/.repo/repo/repo"

	test -f $repo_tool || (echo "Not found repo... skip" &&  return 0)

	if ! $repo_tool version &>/dev/null; then
		repo_tool="repo"
		if ! $repo_tool version &>/dev/null; then
			msg_warn "Not found repo tool, ignore"
			return 0
		fi
	fi

	#Generate patches
	$repo_tool forall -c "$PROJECT_TOP_DIR/scripts/gen_patches_body.sh" || return 0

	#Copy stubs
	$repo_tool manifest -r -o $SDK_ROOT_DIR/manifest_${_date}.xml || return 0

	local tmp_merge=`mktemp` tmp_merge_new=`mktemp` tmp_path tmp_commit
	find $_stub_patch_path -name git-merge-base.txt > $tmp_merge_new
	while read line
	do
		tmp_path="${line##*PATCHES/}"
		tmp_path=$(dirname $tmp_path)
		tmp_commit=$(grep -w "^commit" $line |awk -F ' ' '{print $2}')
		echo "$tmp_path $tmp_commit" >> $tmp_merge
	done < $tmp_merge_new
	rm -f $tmp_merge_new

	mv $SDK_ROOT_DIR/manifest_${_date}.xml $_stub_path/

	while IFS=' ' read line_project line_commit;do
		chkcmd="sed -i \"/\<path=\\\"${line_project//\//\\\/}\\\"/s/revision=\\\"\w\{40\}\\\" /revision=\\\"$line_commit\\\" /\"  $_stub_path/manifest_${_date}.xml"
		eval $chkcmd
	done < $tmp_merge
	rm $tmp_merge
}

function build_save(){
	IMAGE_PATH=$RK_PROJECT_OUTPUT_IMAGE
	DATE=$(date  +%Y%m%d.%H%M)
	STUB_PATH=Image/"${BOARD_INFO}_$DATE"_RELEASE_TEST
	STUB_PATH="$(echo $STUB_PATH | tr '[:lower:]' '[:upper:]')"
	export STUB_PATH=$SDK_ROOT_DIR/$STUB_PATH
	export STUB_PATCH_PATH=$STUB_PATH/PATCHES
	STUB_DEBUG_FILES_PATH="$STUB_PATH/DEBUG_FILES"
	mkdir -p $STUB_PATH $STUB_PATCH_PATH $STUB_DEBUG_FILES_PATH

	mkdir -p $STUB_PATH/IMAGES/
	test -d $IMAGE_PATH && \
		(cp $IMAGE_PATH/* $STUB_PATH/IMAGES/ || msg_warn "Not found images... ignore")

	test -d ${SDK_ROOT_DIR}/rt-thread/bsp/rockchip/rv1106-mcu/ && \
		(cd ${SDK_ROOT_DIR}/rt-thread/bsp/rockchip/rv1106-mcu/ && \
		 tar cjf $STUB_DEBUG_FILES_PATH/rtthread.tar.bz2 rtthread.elf rtthread.map)

	__GET_REPO_INFO $DATE $STUB_PATH $STUB_PATCH_PATH

	#Save build command info
	echo "BUILD-ID: $(hostname):$(whoami)" >> $STUB_PATH/build_info.txt
	build_info >> $STUB_PATH/build_info.txt

	echo "save to $STUB_PATH"

	finish_build
}

function build_allsave(){
	# rm -rf ${RK_PROJECT_OUTPUT_IMAGE} ${RK_PROJECT_OUTPUT}
	build_all
	build_save

	finish_build
}

#=========================
# build targets
#=========================
trap 'err_handler' ERR
cd $PROJECT_TOP_DIR
# unset_board_config_all
if [ "$1" = "lunch" ];then
	if [ -n "$2" ];then
		boardconfig_lunch_name=$2
	fi
	build_select_board $1
	exit 0
elif [ "$1" = "info" ];then
	build_select_board $1
	exit 0
fi

if echo $@|grep -wqE "help|-h"; then
	if [ -n "$2" -a "$(type -t usage$2)" == function ]; then
		echo "###Current Configure [ $2 ] Build Command###"
		eval usage$2
	else
		usage
	fi
	exit 0
fi

BOARD_INFO=$(cd ${TARGET_PRODUCT_DIR};cat rtconfig.h | grep RT_BOARD_NAME)
BOARD_INFO=${BOARD_INFO:23:0-1}

build_check

num=$#
option=""
while [ $# -ne 0 ]
do
	case $1 in
		DEBUG) export RK_BUILD_VERSION_TYPE=DEBUG;;
		all) option="build_all $2";break ;;
		save) option=build_save ;;
		allsave) option=build_allsave ;;
		check) option=build_check ;;
		clean) option="build_clean $2";break;;
		firmware) option=build_firmware ;;
		info) option=build_info ;;
		tool) option=build_tool ;;
		menuconfig) option=build_menuconfig ;;
		*) option=usage ;;
	esac
	if [ $((num)) -gt 0 ]; then
		shift
	fi
done

eval "${option:-build_allsave}"
