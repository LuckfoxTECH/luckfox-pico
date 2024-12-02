#!/bin/bash
set -eE

export LC_ALL=C
export LD_LIBRARY_PATH=
RECORD_IFS="$IFS"

function unset_env_config_rk() {
	local tmp_file=$(mktemp)
	env | grep -oh "^RK_.*=" >$tmp_file || true
	source $tmp_file
	rm -f $tmp_file
}
unset_env_config_rk

################################################################################
# Global Variable Configure
################################################################################
_FDS="\\ \n"
cmd=$(realpath $0)
COMMON_DIR=$(dirname $cmd)
PROJECT_TOP_DIR=$(realpath $COMMON_DIR/)
SDK_ROOT_DIR=$(realpath $COMMON_DIR/..)
SDK_SYSDRV_DIR=${SDK_ROOT_DIR}/sysdrv
SDK_MEDIA_DIR=${SDK_ROOT_DIR}/media
SDK_APP_DIR=${PROJECT_TOP_DIR}/app
BOARD_CONFIG=$SDK_ROOT_DIR/.BoardConfig.mk
TARGET_PRODUCT_DIR=${PROJECT_TOP_DIR}/cfg
GLOBAL_ROOT_FILESYSTEM_NAME=rootfs
GLOBAL_OEM_NAME=oem
GLOBAL_FS_TYPE_SUFFIX=_fs_type
GLOBAL_INITRAMFS_BOOT_NAME=""
GLOBAL_PARTITIONS=""
GLOBAL_SDK_VERSION=""
WIFI_NEW_CONF=${SDK_APP_DIR}/wifi_app/wpa_supplicant_new.conf
WIFI_CONF=${SDK_APP_DIR}/wifi_app/wpa_supplicant.conf
BUILDROOT_PATH=${SDK_SYSDRV_DIR}/source/buildroot/buildroot-2023.02.6
BUILDROOT_CONFIG_FILE=${BUILDROOT_PATH}/.config
SDK_CONFIG_DIR=${SDK_ROOT_DIR}/config
DTS_CONFIG=${SDK_CONFIG_DIR}/dts_config
KERNEL_DEFCONFIG=${SDK_CONFIG_DIR}/kernel_defconfig
BUILDROOT_DEFCONFIG=${SDK_CONFIG_DIR}/buildroot_defconfig
UBUNTU_DIR=${SDK_SYSDRV_DIR}/tools/board/ubuntu
KERNEL_PATH=${SDK_SYSDRV_DIR}/source/kernel
UBOOT_PATH=${SDK_SYSDRV_DIR}/source/uboot/u-boot
#for custom rootfs
CUSTOM_ROOT=${SDK_ROOT_DIR}/custom_root

export RK_JOBS=$(($(getconf _NPROCESSORS_ONLN) / 2 + 1))
export RK_BUILD_VERSION_TYPE=RELEASE

export SDK_ROOT_DIR=$SDK_ROOT_DIR
export RK_PROJECT_OUTPUT=$SDK_ROOT_DIR/output/out
export RK_PROJECT_TOP_DIR=$PROJECT_TOP_DIR
export RK_PROJECT_PATH_MEDIA=$SDK_ROOT_DIR/output/out/media_out
export RK_PROJECT_PATH_SYSDRV=$SDK_ROOT_DIR/output/out/sysdrv_out
export RK_PROJECT_PATH_APP=$SDK_ROOT_DIR/output/out/app_out
export RK_PROJECT_PATH_MCU=$SDK_ROOT_DIR/output/out/mcu_out
export RK_PROJECT_PATH_PC_TOOLS=$SDK_ROOT_DIR/output/out/sysdrv_out/pc
export RK_PROJECT_OUTPUT_IMAGE=$SDK_ROOT_DIR/output/image
export RK_PROJECT_PATH_RAMDISK=$SDK_ROOT_DIR/output/out/ramdisk
export RK_PROJECT_PATH_FASTBOOT=$SDK_ROOT_DIR/output/out/fastboot
export RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS=$RK_PROJECT_PATH_RAMDISK/tiny_rootfs

export PATH=$RK_PROJECT_PATH_PC_TOOLS:$PATH

export RK_PROJECT_FILE_ROOTFS_SCRIPT=$RK_PROJECT_OUTPUT/S20linkmount
export RK_PROJECT_FILE_OEM_SCRIPT=$RK_PROJECT_OUTPUT/S21appinit
export RK_PROJECT_FILE_RECOVERY_SCRIPT=$RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS/etc/init.d/S15linkmount_recovery
export RK_PROJECT_FILE_RECOVERY_LUNCH_SCRIPT=$RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS/etc/init.d/S99lunch_recovery
export RK_PROJECT_FILE_SYSDRV_MCU_BIN=$RK_PROJECT_PATH_MCU/rtthread.bin
export RK_PROJECT_TOOLS_MKFS_SQUASHFS=mkfs_squashfs.sh
export RK_PROJECT_TOOLS_MKFS_EXT4=mkfs_ext4.sh
export RK_PROJECT_TOOLS_MKFS_UBIFS=mkfs_ubi.sh
export RK_PROJECT_TOOLS_MKFS_JFFS2=mkfs_jffs2.sh
export RK_PROJECT_TOOLS_MKFS_ROMFS=mkfs_romfs.sh
export RK_PROJECT_TOOLS_MKFS_EROFS=mkfs_erofs.sh
export RK_PROJECT_TOOLS_MKFS_INITRAMFS=mkfs_initramfs.sh

RK_PROJECT_ROOTFS_TYPE=""
OTA_SCRIPT_PATH=$RK_PROJECT_PATH_RAMDISK
ENV_CFG_FILE=$RK_PROJECT_OUTPUT_IMAGE/.env.txt
ENV_SIZE=""
ENV_OFFSET=""

################################################################################
# Public Configure
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

function msg_info() {
	echo -e "${C_GREEN}[$(basename $0):info] $1${C_NORMAL}"
}

function msg_warn() {
	echo -e "${C_YELLOW}[$(basename $0):warn] $1${C_NORMAL}"
}

function msg_error() {
	echo -e "${C_RED}[$(basename $0):error] $1${C_NORMAL}"
}

err_handler() {
	ret=$?
	[ "$ret" -eq 0 ] && return

	msg_error "Running ${FUNCNAME[1]} failed!"
	msg_error "exit code $ret from line ${BASH_LINENO[0]}:"
	msg_info "    $BASH_COMMAND"
	exit $ret
}

function finish_build() {
	msg_info "Running ${FUNCNAME[1]} succeeded."
	cd $PROJECT_TOP_DIR
}

function check_config() {
	unset missing
	for var in $@; do
		eval [ \$$var ] && continue

		missing="$missing $var"
	done

	[ -z "$missing" ] && return 0

	msg_info "Skipping ${FUNCNAME[1]} for missing configs: $missing."
	return 1
}

function __IS_IN_ARRAY() {
	local value="$1"
	shift
	for item in "$@"; do
		if [[ "$item" == "$value" ]]; then
			return 0
		fi
	done
	return 1
}

function choose_target_board() {
	local LF_HARDWARE=("RV1103_Luckfox_Pico"
		"RV1103_Luckfox_Pico_Mini_A"
		"RV1103_Luckfox_Pico_Mini_B"
		"RV1103_Luckfox_Pico_Plus"
		"RV1103_Luckfox_Pico_WebBee"
		"RV1106_Luckfox_Pico_Pro"
		"RV1106_Luckfox_Pico_Max"
		"RV1106_Luckfox_Pico_Ultra"
		"RV1106_Luckfox_Pico_Ultra_W")
	local LF_BOOT_MEDIA=("SD_CARD" "SPI_NAND" "EMMC")
	local LF_SYSTEM=("Buildroot" "Ubuntu" "Custom")
	local cnt=0 space8="        "

	# Get Hardware Version
	local LUNCH_NUM=0
	local HW_INDEX
	echo "You're building on Linux"
	echo -e "${C_GREEN} "${space8}Lunch menu...pick the Luckfox Pico hardware version:"${C_NORMAL}"
	echo -e "${C_GREEN} "${space8}选择 Luckfox Pico 硬件版本:"${C_NORMAL}"

	echo "${space8}${space8}[${LUNCH_NUM}] RV1103_Luckfox_Pico"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1103_Luckfox_Pico_Mini_A"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1103_Luckfox_Pico_Mini_B"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1103_Luckfox_Pico_Plus"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1103_Luckfox_Pico_WebBee"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1106_Luckfox_Pico_Pro"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1106_Luckfox_Pico_Max"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1106_Luckfox_Pico_Ultra"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] RV1106_Luckfox_Pico_Ultra_W"
	LUNCH_NUM=$((LUNCH_NUM + 1))
	echo "${space8}${space8}[${LUNCH_NUM}] custom"

	read -p "Which would you like? [0~${LUNCH_NUM}][default:0]: " HW_INDEX

	if [ -z "$HW_INDEX" ]; then
		HW_INDEX=0
	fi

	if ! [[ "$HW_INDEX" =~ ^[0-9]+$ ]]; then
		msg_error "Error: HW_INDEX is not a number."
		exit 1
	else
		if (($HW_INDEX < 0 || $HW_INDEX > $LUNCH_NUM)); then
			msg_error "Error: HW_INDEX is not in the range 0-$LUNCH_NUM."
			exit 1
		elif [ $HW_INDEX == $LUNCH_NUM ]; then
			for item in ${RK_TARGET_BOARD_ARRAY[@]}; do
				local f0 boot_medium ddr sys_ver hardware_version product_name
				echo "----------------------------------------------------------------"
				echo -e "${C_GREEN}$cnt. $item${C_NORMAL}"
				cnt=$((cnt + 1))
				f0=${item#BoardConfig*-}
				boot_medium=${f0%%-*}

				f0=${f0#*-}
				sys_ver=${f0%%-*}

				f0=${f0#*-}
				hardware_version=${f0%%-*}

				f0=${f0#*-}
				product_name=${f0%%-*}
				product_name=${product_name%%.mk}
				echo "${space8}${space8}             boot medium(启动介质): ${boot_medium}"
				echo "${space8}${space8}          system version(系统版本): ${sys_ver}"
				echo "${space8}${space8}        hardware version(硬件版本): ${hardware_version}"
				echo "${space8}${space8}             application(应用场景): ${product_name}"
				echo "----------------------------------------------------------------"
				echo ""
			done

			local INDEX
			read -p "Which would you like? [default:0]: " INDEX
			INDEX=$((${INDEX:-0}))

			if [ $RK_TARGET_BOARD_ARRAY_LEN -lt $INDEX ]; then
				msg_error "Input index is error"
				finish_build
				exit 0
			fi

			if echo $INDEX | grep -vq [^0-9]; then
				RK_BUILD_TARGET_BOARD="${RK_TARGET_BOARD_ARRAY[$INDEX]}"
			else
				RK_BUILD_TARGET_BOARD="${RK_TARGET_BOARD_ARRAY[0]}"
				msg_info "Lunching for Default ${RK_BUILD_TARGET_BOARD} boards..."
				return
			fi
			return
		fi
	fi

	# Get Boot Medium Version
	local BM_INDEX MAX_BM_INDEX
	echo -e "${C_GREEN} "${space8}Lunch menu...pick the boot medium:"${C_NORMAL}"
	echo -e "${C_GREEN} "${space8}选择启动媒介:"${C_NORMAL}"

	#if (("$HW_INDEX" >= 0 && "$HW_INDEX" <= 1)); then
	#	echo "${space8}${space8}[0] SD_CARD"
	#	read -p "Which would you like? [0][default:0]: " BM_INDEX
	#	MAX_BM_INDEX=0
	#elif (("$HW_INDEX" >= 2 && "$HW_INDEX" <= 5)); then
	#	echo "${space8}${space8}[0] SD_CARD"
	#	echo "${space8}${space8}[1] SPI_NAND"
	#	read -p "Which would you like? [0~1][default:0]: " BM_INDEX
	#	MAX_BM_INDEX=1
	#elif (("$HW_INDEX" >= 6 && "$HW_INDEX" <= 7)); then
	#	echo "${space8}${space8}[0] EMMC"
	#	read -p "Which would you like? [0][default:0]: " BM_INDEX
	#	MAX_BM_INDEX=0
	#fi

	range_sd_card=(0 1)
	range_sd_card_spi_nand=(2 3 4 5 6)
	range_emmc=(7 8)

	if __IS_IN_ARRAY "$HW_INDEX" "${range_sd_card[@]}"; then
		echo "${space8}${space8}[0] SD_CARD"
		read -p "Which would you like? [0][default:0]: " BM_INDEX
		MAX_BM_INDEX=0
	elif __IS_IN_ARRAY "$HW_INDEX" "${range_sd_card_spi_nand[@]}"; then
		echo "${space8}${space8}[0] SD_CARD"
		echo "${space8}${space8}[1] SPI_NAND"
		read -p "Which would you like? [0~1][default:0]: " BM_INDEX
		MAX_BM_INDEX=1
	elif __IS_IN_ARRAY "$HW_INDEX" "${range_emmc[@]}"; then
		echo "${space8}${space8}[0] EMMC"
		read -p "Which would you like? [0][default:0]: " BM_INDEX
		MAX_BM_INDEX=0
	else
		echo "Invalid HW_INDEX: $HW_INDEX"
		exit 1
	fi

	# Default is 0
	if [ -z "$BM_INDEX" ]; then
		BM_INDEX=0
	fi

	if ! [[ "$BM_INDEX" =~ ^[0-9]+$ ]]; then
		msg_error "Error: BM_INDEX is not a number."
		exit 1
	else
		if (($BM_INDEX < 0 || $BM_INDEX > $MAX_BM_INDEX)); then
			msg_error "Error: BM_INDEX is not in the range ."
			exit 1
		fi
	fi

	# Get System Version
	local SYS_INDEX MAX_SYS_INDEX
	echo -e "${C_GREEN} "${space8}Lunch menu...pick the system version:"${C_NORMAL}"
	echo -e "${C_GREEN} "${space8}选择系统版本:"${C_NORMAL}"

	if (("$BM_INDEX" == 1)); then
		echo "${space8}${space8}[0] Buildroot(Support Rockchip official features) "
		read -p "Which would you like? [0~1][default:0]: " SYS_INDEX
		MAX_SYS_INDEX=0
	elif (("$BM_INDEX" == 0)); then
		echo "${space8}${space8}[0] Buildroot(Support Rockchip official features) "
		echo "${space8}${space8}[1] Ubuntu(Support for the apt package management tool)"
		read -p "Which would you like? [0~1][default:0]: " SYS_INDEX
		MAX_SYS_INDEX=1
	fi

	if [ -z "$SYS_INDEX" ]; then
		SYS_INDEX=0
	fi

	if ! [[ "$SYS_INDEX" =~ ^[0-9]+$ ]]; then
		msg_error "Error: SYS_INDEX is not a number."
		exit 1
	else
		if (($SYS_INDEX < 0 || $SYS_INDEX > $MAX_SYS_INDEX)); then
			msg_error "Error: SYS_INDEX is not in the range 0-1."
			exit 1
		fi
	fi

	# EMMC
	if (("$HW_INDEX" >= range_emmc[0] && "$HW_INDEX" <= range_emmc[${#range_emmc[@]}-1])); then
		BM_INDEX=$BM_INDEX+2 #EMMC
	fi

	RK_BUILD_TARGET_BOARD="BoardConfig_IPC/BoardConfig-${LF_BOOT_MEDIA[$BM_INDEX]}-${LF_SYSTEM[$SYS_INDEX]}-${LF_HARDWARE[$HW_INDEX]}-IPC.mk"
}

function build_select_board() {
	RK_TARGET_BOARD_ARRAY=($(
		cd ${TARGET_PRODUCT_DIR}/
		ls BoardConfig*.mk BoardConfig_*/BoardConfig*.mk | sort
	))

	RK_TARGET_BOARD_ARRAY_LEN=${#RK_TARGET_BOARD_ARRAY[@]}
	if [ $RK_TARGET_BOARD_ARRAY_LEN -eq 0 ]; then
		msg_error "No available Board Config"
		return
	fi

	choose_target_board
	if [ -f "$TARGET_PRODUCT_DIR/$RK_BUILD_TARGET_BOARD" ]; then
		msg_info "Lunching for Default ${RK_BUILD_TARGET_BOARD} boards..."
	else
		msg_error "${RK_BUILD_TARGET_BOARD} is not currently supported"
		exit 0
	fi

	if [ -n $BOARD_CONFIG ]; then
		rm -f $BOARD_CONFIG
	fi
	ln -rfs $TARGET_PRODUCT_DIR/$RK_BUILD_TARGET_BOARD $BOARD_CONFIG
	#cp $TARGET_PRODUCT_DIR/$RK_BUILD_TARGET_BOARD $BOARD_CONFIG
	msg_info "switching to board: $(realpath $BOARD_CONFIG)"

	if [ "$1" = "LUNCH-FORCE" ]; then
		finish_build
		exit 0
	fi
}

function save_board_config() {
	if [ -f $TARGET_PRODUCT_DIR/$ ]; then
		cp $$BOARD_CONFIG $TARGET_PRODUCT_DIR/BoardConfig_IPC/$LF_ORIGIN_BOARD_CONFIG
	fi
}

function unset_board_config_all() {
	local tmp_file=$(mktemp)
	grep -oh "^export.*RK_.*=" $(find cfg -name "BoardConfig*.mk") >$tmp_file
	source $tmp_file
	rm -f $tmp_file
}

function usagemedia() {
	check_config RK_KERNEL_DTS RK_KERNEL_DEFCONFIG || return 0

	echo -e "make -C ${SDK_MEDIA_DIR}"

	finish_build
}

function usagesysdrv() {
	check_config RK_KERNEL_DTS RK_KERNEL_DEFCONFIG || return 0

	usageuboot
	usagekernel
	usagerootfs

	finish_build
}

function usagekernel() {
	check_config RK_KERNEL_DTS RK_KERNEL_DEFCONFIG || return 0

	echo -e "make kernel -C ${SDK_SYSDRV_DIR} ${_FDS} \
		KERNEL_CFG=${RK_KERNEL_DEFCONFIG} ${_FDS} \
		KERNEL_DTS=${RK_KERNEL_DTS} ${_FDS} \
		KERNEL_CFG_FRAGMENT=${RK_KERNEL_DEFCONFIG_FRAGMENT}"

	finish_build
}

function usageuboot() {
	check_config RK_UBOOT_DEFCONFIG || return 0

	echo -e "make uboot -C ${SDK_SYSDRV_DIR} ${_FDS}  \
		UBOOT_CFG=${RK_UBOOT_DEFCONFIG} ${_FDS} \
		UBOOT_CFG_FRAGMENT=${RK_UBOOT_DEFCONFIG_FRAGMENT}"

	finish_build
}

function usagerootfs() {
	# check_config RK_ROOTFS_IMG || return 0
	echo -e "make rootfs -C ${SDK_SYSDRV_DIR} "

	finish_build
}

function usage() {
	echo "Usage: build.sh [OPTIONS]"
	echo "Available options:"
	echo "lunch              -Select Board Configure"
	echo "env                -build env"
	echo "meta               -build meta (optional)"
	echo "uboot              -build uboot"
	echo "kernel             -build kernel"
	echo "rootfs             -build rootfs"
	echo "driver             -build kernel's drivers"
	echo "sysdrv             -build uboot, kernel, rootfs"
	echo "media              -build rockchip media libraries"
	echo "app                -build app"
	echo "recovery           -build recovery"
	echo "tool               -build tool"
	echo "updateimg          -build update image"
	echo "unpackimg          -unpack update image"
	echo "factory            -build factory image"
	echo "all                -build uboot, kernel, rootfs, recovery image"
	echo "allsave            -build all & firmware & save"
	echo ""
	echo "clean              -clean all"
	echo "clean uboot        -clean uboot"
	echo "clean kernel       -clean kernel"
	echo "clean driver       -clean driver"
	echo "clean rootfs       -clean rootfs"
	echo "clean sysdrv       -clean uboot/kernel/rootfs"
	echo "clean media        -clean rockchip media libraries"
	echo "clean app          -clean app"
	echo "clean recovery     -clean recovery"
	echo ""
	echo "firmware           -pack all the image we need to boot up system"
	echo "ota                -pack update_ota.tar"
	echo "save               -save images, patches, commands used to debug"
	echo "check              -check the environment of building"
	echo "info               -see the current board building information"
	echo ""
	echo "buildrootconfig    -config b	# EMMCuildroot and save defconfig"
	echo "kernelconfig       -config kernel and save defconfig"
	echo ""
	echo "Default option is 'allsave'."
	finish_build
	exit 0
}

function build_get_sdk_version() {
	version=$(grep -m 1 -o '## V[0-9]\.[0-9]' "${SDK_ROOT_DIR}/UPDATE_LOG.md" | awk '{print $2}')
	GLOBAL_SDK_VERSION="Luckfox Pico Sdk ${version}"
}

function build_info() {
	if [ ! -L $BOARD_CONFIG ]; then
		echo "No found target board config!!!"
	fi

	build_get_sdk_version

	# Support copy RK-RELEASE-NOTES-V*.txt from project/cfg/BoardConfig*/RK-RELEASE-NOTES-V*.txt to SDK_ROOT_DIR
	local board_dir board_version_notes
	board_dir=$(realpath $BOARD_CONFIG)
	board_dir=$(dirname $board_dir)
	# if ls $board_dir/RK-RELEASE-NOTES-V*.txt &>/dev/null;then
	# 	board_version_notes=$(ls $board_dir/RK-RELEASE-NOTES-V*.txt |sort -r |head -1)
	# 	rm -f $SDK_ROOT_DIR/RK-RELEASE-NOTES-V*.txt
	# 	test ! -f $board_version_notes || cp -f $board_version_notes $SDK_ROOT_DIR
	# fi

	echo "Current Building Information:"
	echo "Target cfg: $(realpath $BOARD_CONFIG)"
	echo "Target Misc config:"
	echo "$(env | grep "^RK_" | grep -v "=$" | sort)"

	make info -C ${SDK_SYSDRV_DIR}
	make info -C ${SDK_MEDIA_DIR}
	make info -C ${PROJECT_TOP_DIR}/app

	build_check_power_domain
}

function build_check_power_domain() {
	local dump_kernel_dtb_file
	local tmp_phandle_file
	local tmp_io_domain_file
	local tmp_regulator_microvolt_file
	local tmp_final_target
	local tmp_none_item
	local kernel_file_dtb_dts

	if [ ! -f "${RK_PROJECT_PATH_BOARD_BIN}/${RK_KERNEL_DTS/%.dts/.dtb}" ]; then
		echo "Not found ${RK_PROJECT_PATH_BOARD_BIN}/${RK_KERNEL_DTS/%.dts/.dtb}, ignore"
		return
	fi
	kernel_file_dtb_dts="${RK_PROJECT_PATH_BOARD_BIN}/${RK_KERNEL_DTS%.dts}"

	dump_kernel_dtb_file=${kernel_file_dtb_dts}.dump.dts

	dtc -I dtb -O dts -o ${dump_kernel_dtb_file} ${kernel_file_dtb_dts}.dtb 2>/dev/null
	tmp_grep_file=$(mktemp)
	if ! grep -Pzo "io-domains\s*{(\n|\w|-|;|=|<|>|\"|_|\s|,)*};" $dump_kernel_dtb_file 1>$tmp_grep_file 2>/dev/null; then
		rm -f $dump_kernel_dtb_file
		rm -f $tmp_grep_file
		return 0
	fi
	tmp_regulator_microvolt_file=$(mktemp)
	tmp_io_domain_file=$(mktemp)
	tmp_final_target=$(mktemp)
	tmp_phandle_file=$(mktemp)
	grep -a supply $tmp_grep_file >$tmp_io_domain_file
	rm -f $tmp_grep_file
	awk '{print "phandle = " $3}' $tmp_io_domain_file >$tmp_phandle_file

	while IFS= read -r item_phandle && IFS= read -u 3 -r item_domain; do
		echo "${item_domain% *}" >>$tmp_regulator_microvolt_file
		tmp_none_item=${item_domain% *}
		cmds="grep -Pzo \"{(\\n|\w|-|;|=|<|>|\\\"|_|\s)*"$item_phandle\"

		eval "$cmds $dump_kernel_dtb_file | strings | grep "regulator-m..-microvolt" >> $tmp_regulator_microvolt_file" ||
			eval "sed -i \"/${tmp_none_item}/d\" $tmp_regulator_microvolt_file" && continue

		echo >>$tmp_regulator_microvolt_file
	done <$tmp_phandle_file 3<$tmp_io_domain_file

	while read -r regulator_val; do
		if echo ${regulator_val} | grep supply &>/dev/null; then
			echo -e "\n\n\e[1;33m${regulator_val%*=}\e[0m" >>$tmp_final_target
		else
			tmp_none_item=${regulator_val##*<}
			tmp_none_item=${tmp_none_item%%>*}
			echo -e "${regulator_val%%<*} \e[1;31m$(($tmp_none_item / 1000))mV\e[0m" >>$tmp_final_target
		fi
	done <$tmp_regulator_microvolt_file

	echo -e "\e[41;1;30m PLEASE CHECK BOARD GPIO POWER DOMAIN CONFIGURATION !!!!!\e[0m"
	echo -e "\e[41;1;30m <<< ESPECIALLY Wi-Fi/Flash/Ethernet IO power domain >>> !!!!!\e[0m"
	echo -e "\e[41;1;30m Check Node [pmu_io_domains] in the file: ${kernel_file_dtb_dts}.dts \e[0m"
	echo
	echo -e "\e[41;1;30m 请再次确认板级的电源域配置！！！！！！\e[0m"
	echo -e "\e[41;1;30m <<< 特别是Wi-Fi，FLASH，以太网这几路IO电源的配置 >>> ！！！！！\e[0m"
	echo -e "\e[41;1;30m 检查内核文件 ${kernel_file_dtb_dts}.dts 的节点 [pmu_io_domains] \e[0m"
	cat $tmp_final_target

	rm -f $dump_kernel_dtb_file
	rm -f $tmp_phandle_file
	rm -f $tmp_io_domain_file
	rm -f $tmp_regulator_microvolt_file
	rm -f $tmp_final_target
}

function build_tool() {
	test -d ${SDK_SYSDRV_DIR} && make pctools -C ${SDK_SYSDRV_DIR}
	if [ $LF_ENABLE_SPI_NAND_FAST_BOOT = "y" ]; then
		cp -fa $PROJECT_TOP_DIR/sfc_scripts/mk-fitimage.sh $RK_PROJECT_PATH_PC_TOOLS
		cp -fa $PROJECT_TOP_DIR/sfc_scripts/compress_tool $RK_PROJECT_PATH_PC_TOOLS
		cp -fa $PROJECT_TOP_DIR/sfc_scripts/mk-tftp_sd_update.sh $RK_PROJECT_PATH_PC_TOOLS
	else
		cp -fa $PROJECT_TOP_DIR/scripts/mk-fitimage.sh $RK_PROJECT_PATH_PC_TOOLS
		cp -fa $PROJECT_TOP_DIR/scripts/compress_tool $RK_PROJECT_PATH_PC_TOOLS
		cp -fa $PROJECT_TOP_DIR/scripts/mk-tftp_sd_update.sh $RK_PROJECT_PATH_PC_TOOLS
	fi
	finish_build
}

function build_check() {
	common_product_build_tools="${PROJECT_TOP_DIR}/scripts/build-depend-tools.txt"
	cat $common_product_build_tools 2>/dev/null | while read chk_item; do
		chk_item=${chk_item###*}
		if [ -z "$chk_item" ]; then
			continue
		fi

		dst=${chk_item%%,*}
		src=${chk_item##*,}
		echo "**************************************"
		if eval $dst &>/dev/null; then
			echo "Check [OK]: $dst"
		else
			echo "Please install ${dst%% *} first"
			echo "    sudo apt-get install $src"
		fi
	done
}

function build_app() {
	if [ "$RK_ENABLE_WIFI" = "y" ]; then
		echo "Set Wifi SSID and PASSWD"
		check_config LF_WIFI_PSK LF_WIFI_SSID || return 0
		touch $WIFI_NEW_CONF
		cat >$WIFI_NEW_CONF <<EOF
ctrl_interface=/var/run/wpa_supplicant
ap_scan=1
update_config=1

network={
	ssid="$LF_WIFI_SSID"
	psk="$LF_WIFI_PSK"
	key_mgmt=WPA-PSK
}
EOF
		mv $WIFI_NEW_CONF $WIFI_CONF
	fi

	check_config RK_APP_TYPE || return 0

	echo "============Start building app============"
	echo "TARGET_APP_CONFIG=$RK_APP_DEFCONFIG $RK_APP_DEFCONFIG_FRAGMENT $RK_APP_TYPE"
	echo "========================================="

	build_meta --export --media_dir $RK_PROJECT_PATH_MEDIA # export meta header files
	#build_meta --export --media_dir $RK_PROJECT_PATH_MEDIA # for rtl8723bs
	test -d ${SDK_APP_DIR} && make -C ${SDK_APP_DIR}

	finish_build
}

function __modify_file() {

	local flag_opt_match_start flag_opt_match_end todo_file replace_str find_str target_file

	todo_file=$1
	target_file=$2
	find_str=$3
	replace_str=$4

	if [ -n "$5" ]; then
		flag_opt_match_start=$5
	fi

	if [ -n "$6" ]; then
		flag_opt_match_end=$6
	fi

	rm -f $target_file

	if [ ! -d "$RK_PROJECT_PATH_FASTBOOT" ]; then
		mkdir -p $RK_PROJECT_PATH_FASTBOOT
	fi

	while read line; do
		if echo "$line" | grep "$flag_opt_match_start$find_str$flag_opt_match_end"; then
			echo "$find_str$replace_str" >>$target_file
			continue
		fi
		echo "$line" >>$target_file
	done <$todo_file
}

function build_uboot() {
	check_config RK_UBOOT_DEFCONFIG || return 0

	echo "============Start building uboot============"
	echo "TARGET_UBOOT_CONFIG=$RK_UBOOT_DEFCONFIG $RK_UBOOT_DEFCONFIG_FRAGMENT"
	echo "========================================="

	#Apply patch
	if [ ! -f ${SDK_SYSDRV_DIR}/source/.uboot_patch ]; then
		echo "============Apply Uboot Patch============"
		cd ${SDK_ROOT_DIR}
		git apply ${SDK_SYSDRV_DIR}/tools/board/uboot/*.patch
		if [ $? -eq 0 ]; then
			msg_info "Patch applied successfully."
			touch ${SDK_SYSDRV_DIR}/source/.uboot_patch
		else
			msg_error "Failed to apply the patch."
			exit 1
		fi
	fi

	cp ${SDK_SYSDRV_DIR}/tools/board/uboot/*_defconfig ${SDK_SYSDRV_DIR}/source/uboot/u-boot/configs
	cp ${SDK_SYSDRV_DIR}/tools/board/uboot/*.dts ${SDK_SYSDRV_DIR}/source/uboot/u-boot/arch/arm/dts
	cp ${SDK_SYSDRV_DIR}/tools/board/uboot/*.dtsi ${SDK_SYSDRV_DIR}/source/uboot/u-boot/arch/arm/dts

	local uboot_rkbin_ini tempfile target_ini_dir
	tempfile="$SDK_SYSDRV_DIR/source/uboot/rkbin/$RK_UBOOT_RKBIN_INI_OVERLAY"
	if [ -f "$tempfile" ]; then
		uboot_rkbin_ini=$tempfile
	fi

	target_ini_dir=$SDK_SYSDRV_DIR/source/uboot/rkbin/RKBOOT/
	if [ "$RK_ENABLE_FASTBOOT" = "y" -a -n "$RK_UBOOT_RKBIN_MCU_CFG" ]; then
		uboot_rkbin_ini=$RK_PROJECT_PATH_FASTBOOT/rk_uboot_rkbin_rkboot_overlay.ini
		build_mcu $RK_UBOOT_RKBIN_MCU_CFG "__MCU_CONTINUE__"
		case $RK_BOOT_MEDIUM in
		emmc)
			tempfile=$target_ini_dir/RV1106MINIALL_EMMC_TB.ini
			;;
		sd_card)
			tempfile=$target_ini_dir/RV1106MINIALL_SDMMC_TB.ini
			;;
		spi_nor)
			tempfile=$target_ini_dir/RV1106MINIALL_SPI_NOR_TB.ini
			;;
		spi_nand | slc_nand)
			tempfile=$target_ini_dir/RV1106MINIALL_SPI_NAND_TB.ini
			;;
		*)
			echo "build uboot Not support storage medium type: $RK_BOOT_MEDIUM"
			finish_build
			exit 1
			;;
		esac

		if [ -f "$RK_PROJECT_FILE_SYSDRV_MCU_BIN" ]; then
			__modify_file $tempfile $uboot_rkbin_ini "Hpmcu=" "$RK_PROJECT_FILE_SYSDRV_MCU_BIN" "^"
		else
			msg_error "build mcu <$RK_UBOOT_RKBIN_MCU_CFG> failed"
			exit 1
		fi
	fi

	make uboot -C ${SDK_SYSDRV_DIR} UBOOT_CFG=${RK_UBOOT_DEFCONFIG} UBOOT_CFG_FRAGMENT=${RK_UBOOT_DEFCONFIG_FRAGMENT} SYSDRV_UBOOT_RKBIN_OVERLAY_INI=$uboot_rkbin_ini

	finish_build
}

function build_meta() {
	msg_info "============Start building meta============"
	if [ -n "$RK_META_SIZE" ]; then
		if [ -d "${RK_PROJECT_TOP_DIR}/make_meta" ]; then
			__meta_param="$RK_META_PARAM $RK_CAMERA_PARAM --meta_part_size=$RK_META_SIZE"
			${RK_PROJECT_TOP_DIR}/make_meta/build_meta.sh $@ \
				--cam_iqfile ${RK_CAMERA_SENSOR_IQFILES} \
				--meta_param $__meta_param \
				--output $RK_PROJECT_OUTPUT_IMAGE \
				--rootfs_dir $RK_PROJECT_PACKAGE_ROOTFS_DIR \
				--media_dir $RK_PROJECT_PATH_MEDIA \
				--pc_tools_dir $RK_PROJECT_PATH_PC_TOOLS \
				--boot_medium $RK_BOOT_MEDIUM \
				--tiny_meta $RK_TINY_META
		fi
	fi
	finish_build
}

function build_env() {
	msg_info "============Start building env============"
	msg_info "$RK_PARTITION_CMD_IN_ENV"

	# Enable building if env partition is no exist
	check_config ENV_SIZE || return 0

	local env_cfg_img
	env_cfg_img=$RK_PROJECT_OUTPUT_IMAGE/env.img

	if [ ! -f "$RK_PROJECT_PATH_PC_TOOLS/mkenvimage" ] || [ "$LF_ENABLE_SPI_NAND_FAST_BOOT" = "y" ]; then
		build_tool
	fi

	echo "$SYS_BOOTARGS" >>$ENV_CFG_FILE
	echo "sd_parts=mmcblk0:16K@512(env),512K@32K(idblock),4M(uboot)" >>$ENV_CFG_FILE
	# build env.img
	$RK_PROJECT_PATH_PC_TOOLS/mkenvimage -s $ENV_SIZE -p 0x0 -o $env_cfg_img $ENV_CFG_FILE
	chmod +r $env_cfg_img

	finish_build
}

function build_media() {
	echo "============Start building media============"

	make -C ${SDK_MEDIA_DIR}

	finish_build
}

function build_driver() {
	echo "============Start building kernel's drivers============"

	mkdir -p ${RK_PROJECT_OUTPUT_IMAGE}
	build_kernel
	make -C ${SDK_SYSDRV_DIR} drv

	finish_build
}

function build_sysdrv() {
	echo "============Start building sysdrv============"

	mkdir -p ${RK_PROJECT_OUTPUT_IMAGE}
	build_uboot
	build_kernel
	build_rootfs
	build_recovery

	rootfs_tarball="$RK_PROJECT_PATH_SYSDRV/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}.tar"
	rootfs_out_dir="$RK_PROJECT_OUTPUT/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}"

	if ! [ -d $RK_PROJECT_OUTPUT ]; then
		mkdir -p $RK_PROJECT_OUTPUT
	fi

	if [ -f $rootfs_tarball ]; then
		if [ -d $rootfs_out_dir ]; then
			rm -rf $rootfs_out_dir
		fi
		tar xf $rootfs_tarball -C $RK_PROJECT_OUTPUT
	else
		msg_error "Not found rootfs tarball: $rootfs_tarball"
		exit 1
	fi

	msg_info "If you need to add custom files, please upload them to <Luckfox Sdk>/output/out/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}."
	finish_build
}

function build_kernel() {
	#Apply patch
	if [ ! -f ${SDK_SYSDRV_DIR}/source/.kernel_patch ]; then
		echo "============Apply Kernel Patch============"
		cd ${SDK_ROOT_DIR}
		git apply --verbose ${SDK_SYSDRV_DIR}/tools/board/kernel/*.patch
		if [ $? -eq 0 ]; then
			msg_info "Patch applied successfully."
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*_defconfig ${KERNEL_PATH}/arch/arm/configs/
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*.config ${KERNEL_PATH}/arch/arm/configs/
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/kernel-drivers-video-logo_linux_clut224.ppm ${KERNEL_PATH}/drivers/video/logo/logo_linux_clut224.ppm
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*.dts ${KERNEL_PATH}/arch/arm/boot/dts
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*.dtsi ${KERNEL_PATH}/arch/arm/boot/dts
			touch ${SDK_SYSDRV_DIR}/source/.kernel_patch
		else
			msg_error "Failed to apply the patch."
		fi
	fi

	check_config RK_KERNEL_DTS RK_KERNEL_DEFCONFIG || return 0

	echo "============Start building kernel============"
	echo "TARGET_ARCH          =$RK_ARCH"
	echo "TARGET_KERNEL_CONFIG =$RK_KERNEL_DEFCONFIG"
	echo "TARGET_KERNEL_DTS    =$RK_KERNEL_DTS"
	echo "TARGET_KERNEL_CONFIG_FRAGMENT =$RK_KERNEL_DEFCONFIG_FRAGMENT"
	echo "=========================================="

	local kernel_build_options
	if [ "$RK_ENABLE_FASTBOOT" = "y" ]; then
		kernel_build_options="OUTPUT_SYSDRV_RAMDISK_DIR=$RK_PROJECT_PATH_FASTBOOT"
		mkdir -p $RK_PROJECT_PATH_FASTBOOT
	fi
	make kernel -C ${SDK_SYSDRV_DIR} \
		$kernel_build_options \
		KERNEL_DTS=${RK_KERNEL_DTS} \
		KERNEL_CFG=${RK_KERNEL_DEFCONFIG} \
		KERNEL_CFG_FRAGMENT=${RK_KERNEL_DEFCONFIG_FRAGMENT}

	finish_build
}

function build_rootfs() {
	check_config RK_BOOT_MEDIUM || return 0

	make rootfs -C ${SDK_SYSDRV_DIR}

	__LINK_DEFCONFIG_FROM_BOARD_CFG

	local rootfs_tarball rootfs_out_dir
	rootfs_tarball="$RK_PROJECT_PATH_SYSDRV/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}.tar"
	rootfs_out_dir="$RK_PROJECT_OUTPUT/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}"

	if ! [ -d $RK_PROJECT_OUTPUT ]; then
		mkdir -p $RK_PROJECT_OUTPUT
	fi

	if [ -f $rootfs_tarball ]; then
		if [ -d $rootfs_out_dir ]; then
			rm -rf $rootfs_out_dir
		fi
		tar xf $rootfs_tarball -C $RK_PROJECT_OUTPUT
	else
		msg_error "Not found rootfs tarball: $rootfs_tarball"
		exit 1
	fi

	msg_info "If you need to add custom files, please upload them to <Luckfox Sdk>/output/out/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}."
	finish_build
}

function build_mcu() {
	if [ ! -d "$SDK_SYSDRV_DIR/source/mcu" ]; then
		msg_info "Not found mcu [$SDK_SYSDRV_DIR/source/mcu], ignore"
		exit 1
	fi

	# sync gcc
	if [ ! -d "$SDK_SYSDRV_DIR/source/mcu/prebuilts/gcc" ]; then
		mkdir -p $SDK_SYSDRV_DIR/source/mcu/prebuilts/gcc/linux-x86/riscv64

		wget -P $SDK_SYSDRV_DIR/source/mcu/prebuilts/gcc/linux-x86/riscv64 \
			https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack/releases/download/v10.2.0-1.2/xpack-riscv-none-embed-gcc-10.2.0-1.2-linux-x64.tar.gz
		if [ ! -f $SDK_SYSDRV_DIR/source/mcu/prebuilts/gcc/linux-x86/riscv64/xpack-riscv-none-embed-gcc-10.2.0-1.2-linux-x64.tar.gz ]; then
			msg_error "Download prebuilts fail!Try downloading manually from https://github.com/xpack-dev-tools/riscv-none-embed-gcc-xpack/releases/download/v10.2.0-1.2/xpack-riscv-none-embed-gcc-10.2.0-1.2-linux-x64.tar.gz"
			exit 1
		fi

		tar -xvf $SDK_SYSDRV_DIR/source/mcu/prebuilts/gcc/linux-x86/riscv64/xpack-riscv-none-embed-gcc-10.2.0-1.2-linux-x64.tar.gz \
			-C $SDK_SYSDRV_DIR/source/mcu/prebuilts/gcc/linux-x86/riscv64
	fi

	local build_opt
	build_opt=""
	if [ "$1" = "info" ]; then
		$SDK_SYSDRV_DIR/source/mcu/build.sh info
		finish_build
		exit 0
	elif [ -n "$1" ]; then
		if find $SDK_SYSDRV_DIR/source/mcu -type d -name "$1"; then
			build_opt="$1"
		fi
	fi

	$SDK_SYSDRV_DIR/source/mcu/build.sh lunch $build_opt
	$SDK_SYSDRV_DIR/source/mcu/build.sh clean
	mkdir -p $RK_PROJECT_PATH_MCU
	$SDK_SYSDRV_DIR/source/mcu/build.sh all $RK_PROJECT_PATH_MCU

	if [ "$2" = "__MCU_CONTINUE__" ]; then
		return
	else
		exit 0
	fi
}

function build_recovery() {
	check_config RK_ENABLE_RECOVERY || return 0

	local kernel_image
	local kernel_dtb_file="$RK_PROJECT_PATH_RAMDISK/${RK_KERNEL_DTS/%.dts/.dtb}"
	local ota_script="$OTA_SCRIPT_PATH/RK_OTA_update.sh"
	local erase_misc_script="$RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS/usr/bin/RK_OTA_erase_misc.sh"
	local ramdisk_file="rootfs.cpio"

	# make busybox and kernel
	mkdir -p $RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS $RK_PROJECT_PATH_RAMDISK

	if [ -z "$RK_RECOVERY_KERNEL_DEFCONFIG_FRAGMENT" ]; then
		msg_error "Please define RK_RECOVERY_KERNEL_DEFCONFIG_FRAGMENT on BoardConfig"
		exit 1
	fi
	RK_KERNEL_DEFCONFIG_FRAGMENT="${RK_RECOVERY_KERNEL_DEFCONFIG_FRAGMENT}"
	make kernel -C ${SDK_SYSDRV_DIR} \
		OUTPUT_SYSDRV_RAMDISK_DIR=$RK_PROJECT_PATH_RAMDISK \
		SYSDRV_BUILD_RECOVERY=y \
		KERNEL_DTS=${RK_KERNEL_DTS} \
		KERNEL_CFG=${RK_KERNEL_DEFCONFIG} \
		KERNEL_CFG_FRAGMENT=${RK_KERNEL_DEFCONFIG_FRAGMENT}

	make -C ${SDK_SYSDRV_DIR} \
		OUTPUT_SYSDRV_RAMDISK_TINY_ROOTFS_DIR=$RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS \
		SYSDRV_BUILD_RECOVERY=y \
		busybox

	# copy tools
	mkdir -p $RK_PROJECT_PATH_PC_TOOLS
	if [ ! -f $RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
		-o ! -f $RK_PROJECT_PATH_PC_TOOLS/mkimage ]; then
		build_tool
	fi
	cp -fa $PROJECT_TOP_DIR/scripts/RkLunch-recovery.sh $RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS/usr/bin/RkLunch.sh
	cp -fa $PROJECT_TOP_DIR/scripts/boot4recovery.its $RK_PROJECT_PATH_RAMDISK

	mkdir -p $(dirname $RK_PROJECT_FILE_RECOVERY_LUNCH_SCRIPT)
	cat >$RK_PROJECT_FILE_RECOVERY_LUNCH_SCRIPT <<EOF
#!/bin/sh
case \$1 in
	start)
		sh /usr/bin/RkLunch.sh
		;;
	stop)
		sh /usr/bin/RkLunch-stop.sh
		;;
	*)
		exit 1
		;;
esac
EOF
	chmod a+x $RK_PROJECT_FILE_RECOVERY_LUNCH_SCRIPT

	mkdir -p $(dirname $erase_misc_script)
	echo "#!/bin/sh" >$erase_misc_script
	echo "set -e" >>$erase_misc_script
	echo "COMMON_DIR=\`dirname \$(realpath \$0)\`" >>$erase_misc_script
	echo "TOP_DIR=\$(realpath \$COMMON_DIR/../..)" >>$erase_misc_script
	echo "cd \$TOP_DIR" >>$erase_misc_script
	echo "echo \"Erase misc partition\"" >>$erase_misc_script

	case $RK_BOOT_MEDIUM in
	emmc | spi_nor | sd_card)
		echo "dd if=/dev/zero of=/dev/block/by-name/misc bs=32 count=1 seek=512" >>$erase_misc_script
		echo "if [ \$? -ne 0 ];then" >>$erase_misc_script
		echo "	echo \"Error: Erase misc partition failed.\"" >>$erase_misc_script
		echo "	exit 2" >>$erase_misc_script
		echo "fi" >>$erase_misc_script
		;;
	spi_nand | slc_nand)
		echo "flash_eraseall /dev/block/by-name/misc" >>$erase_misc_script
		echo "if [ \$? -ne 0 ];then" >>$erase_misc_script
		echo "	echo \"Error: Erase misc partition failed.\"" >>$erase_misc_script
		echo "	exit 2" >>$erase_misc_script
		echo "fi" >>$erase_misc_script
		;;
	*)
		echo "Not support storage medium type: $RK_BOOT_MEDIUM"
		finish_build
		exit 1
		;;
	esac

	chmod a+x $erase_misc_script

	case "$RK_ARCH" in
	arm)
		kernel_image="$RK_PROJECT_PATH_RAMDISK/zImage"
		;;
	arm64)
		kernel_image="$RK_PROJECT_PATH_RAMDISK/Image"
		;;
	*)
		echo "No such kernel image. ($RK_ARCH)"
		;;
	esac

	# build recovery for fastboot
	if [ "$RK_ENABLE_FASTBOOT" = "y" ]; then
		ramdisk_file="recovery_erofs.img"
		case "$RK_ARCH" in
		arm)
			kernel_image="$RK_PROJECT_PATH_RAMDISK/Image.gz"
			gzip -9 -c $RK_PROJECT_PATH_RAMDISK/Image >$kernel_image
			;;
		*)
			echo "No such kernel image for fastboot. ($RK_ARCH)"
			exit 1
			;;
		esac
		cp -fa $PROJECT_TOP_DIR/scripts/$RK_CHIP-boot-tb.its $RK_PROJECT_PATH_RAMDISK/boot4recovery.its

		# package rootfs in erofs for fastboot if necessary
		$RK_PROJECT_TOOLS_MKFS_EROFS $RK_PROJECT_PATH_RAMDISK/tiny_rootfs $RK_PROJECT_PATH_RAMDISK/$ramdisk_file
	else
		# package rootfs in cpio
		(
			cd $RK_PROJECT_PATH_RAMDISK/tiny_rootfs
			find . | cpio --quiet -o -H newc >$RK_PROJECT_PATH_RAMDISK/$ramdisk_file
		)
	fi

	cat $RK_PROJECT_PATH_RAMDISK/$ramdisk_file | gzip -n -f -9 >$RK_PROJECT_PATH_RAMDISK/${ramdisk_file}.gz
	ramdisk_file=${ramdisk_file}.gz

	# package recovery.img
	mkdir -p $RK_PROJECT_OUTPUT_IMAGE
	$RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
		$RK_PROJECT_PATH_RAMDISK/boot4recovery.its \
		$RK_PROJECT_PATH_RAMDISK/${ramdisk_file} \
		$kernel_image \
		$kernel_dtb_file \
		$RK_PROJECT_PATH_RAMDISK/resource.img \
		$RK_PROJECT_OUTPUT_IMAGE/recovery.img \
		$RK_ARCH \
		$fit_target_optional_param

	# strip debug symbol
	__RELEASE_FILESYSTEM_FILES $RK_PROJECT_PATH_RAMDISK_TINY_ROOTFS

	mkdir -p $(dirname $ota_script)
	echo "#!/bin/sh" >$ota_script
	echo "set -e" >>$ota_script
	echo "COMMON_DIR=\`dirname \$(realpath \$0)\`" >>$ota_script
	echo "TOP_DIR=\$(realpath \$COMMON_DIR/../..)" >>$ota_script
	echo "cd \$TOP_DIR" >>$ota_script
	echo "echo \"Start to write partitions\"" >>$ota_script

	case $RK_BOOT_MEDIUM in
	emmc | spi_nor | sd_card)
		echo "for image in \$(ls /dev/block/by-name)" >>$ota_script
		echo "do" >>$ota_script
		echo "	if [ -f \$COMMON_DIR/\${image}.img ];then" >>$ota_script
		echo "		echo \"Writing \$image...\"" >>$ota_script
		echo "		dd if=\$COMMON_DIR/\${image}.img of=/dev/block/by-name/\$image" >>$ota_script
		echo "		if [ \$? -ne 0 ];then" >>$ota_script
		echo "			echo \"Error: \$image write failed.\"" >>$ota_script
		echo "			exit 1" >>$ota_script
		echo "		fi" >>$ota_script
		echo "	fi" >>$ota_script
		echo "done" >>$ota_script
		echo "echo \"Erase misc partition\"" >>$ota_script
		echo "dd if=/dev/zero of=/dev/block/by-name/misc bs=32 count=1 seek=512" >>$ota_script
		echo "if [ \$? -ne 0 ];then" >>$ota_script
		echo "	echo \"Error: Erase misc partition failed.\"" >>$ota_script
		echo "	exit 2" >>$ota_script
		echo "fi" >>$ota_script
		;;
	spi_nand | slc_nand)
		echo "for image in \$(ls /dev/block/by-name)" >>$ota_script
		echo "do" >>$ota_script
		echo "	if [ -f \$COMMON_DIR/\${image}.img ];then" >>$ota_script
		echo "		echo \"Writing \$image...\"" >>$ota_script
		echo "		mtd_path=\$(realpath /dev/block/by-name/\${image})" >>$ota_script
		echo "		flash_eraseall \$mtd_path" >>$ota_script
		echo "		nandwrite -p \$mtd_path \$COMMON_DIR/\${image}.img" >>$ota_script
		echo "		if [ \$? -ne 0 ];then" >>$ota_script
		echo "			echo \"Error: \$image write failed.\"" >>$ota_script
		echo "			exit 1" >>$ota_script
		echo "		fi" >>$ota_script
		echo "	fi" >>$ota_script
		echo "done" >>$ota_script
		echo "echo \"Erase misc partition\"" >>$ota_script
		echo "flash_eraseall /dev/block/by-name/misc" >>$ota_script
		echo "if [ \$? -ne 0 ];then" >>$ota_script
		echo "	echo \"Error: Erase misc partition failed.\"" >>$ota_script
		echo "	exit 2" >>$ota_script
		echo "fi" >>$ota_script
		;;
	*)
		echo "Not support storage medium type: $RK_BOOT_MEDIUM"
		finish_build
		exit 1
		;;
	esac

	chmod a+x $ota_script

	finish_build
}

function build_ota() {
	check_config RK_ENABLE_RECOVERY || check_config RK_ENABLE_OTA || return 0

	local update_img update_script tar_cmd

	if [ -z "$RK_OTA_RESOURCE" ]; then
		for img in uboot.img boot.img rootfs.img; do
			if [ -f "$RK_PROJECT_OUTPUT_IMAGE/$img" ]; then
				update_img="$update_img $img"
			else
				msg_warn "Not found $img, check again!!!"
			fi
		done
	else
		update_img="$RK_OTA_RESOURCE"
	fi

	[[ "$RK_ENABLE_RECOVERY" = "y" ]] && update_script="-C $OTA_SCRIPT_PATH RK_OTA_update.sh"

	tar_cmd="tar -cvhf  $RK_PROJECT_OUTPUT_IMAGE/update_ota.tar \
	             -C $RK_PROJECT_OUTPUT_IMAGE $update_img \
	             $update_script"
	eval $tar_cmd

	finish_build
}

function build_tftp_sd_update() {
	# copy tools
	mkdir -p $RK_PROJECT_PATH_PC_TOOLS
	if [ ! -f $RK_PROJECT_PATH_PC_TOOLS/mk-tftp_sd_update.sh ]; then
		build_tool
	fi

	if [ "$RK_BOOT_MEDIUM" = "sd_card" ]; then
		$RK_PROJECT_PATH_PC_TOOLS/mk-tftp_sd_update.sh $GLOBAL_PARTITIONS $RK_PROJECT_OUTPUT_IMAGE emmc
	else
		$RK_PROJECT_PATH_PC_TOOLS/mk-tftp_sd_update.sh $GLOBAL_PARTITIONS $RK_PROJECT_OUTPUT_IMAGE $RK_BOOT_MEDIUM
	fi

	finish_build
}

function build_updateimg() {
	# Enable building if env partition is no exist
	check_config ENV_SIZE || return 0

	IMAGE_PATH=$RK_PROJECT_OUTPUT_IMAGE
	PACK_TOOL_PATH=$SDK_ROOT_DIR/tools/linux/Linux_Pack_Firmware

	# run update.img package script
	$PACK_TOOL_PATH/mk-update_pack.sh -id $RK_CHIP -i $IMAGE_PATH

	finish_build
}

function build_unpack_updateimg() {
	IMAGE_PATH=$RK_PROJECT_OUTPUT_IMAGE/update.img
	UNPACK_FILE_DIR=$RK_PROJECT_OUTPUT_IMAGE/unpack
	PACK_TOOL_PATH=$SDK_ROOT_DIR/tools/linux/Linux_Pack_Firmware

	# run update.img unpack script
	mkdir -p $UNPACK_FILE_DIR
	$PACK_TOOL_PATH/mk-update_unpack.sh -i $IMAGE_PATH -o $UNPACK_FILE_DIR

	finish_build
}

function build_factory() {
	local nand_block_size nand_page_size nand_oob_size

	IMAGE_PATH=$RK_PROJECT_OUTPUT_IMAGE/update.img
	FACTORY_FILE_DIR=$RK_PROJECT_OUTPUT_IMAGE/factory
	PROGRAMMER_TOOL_PATH=$SDK_ROOT_DIR/tools/linux/SocToolKit/bin/linux

	nand_block_size=${RK_NAND_BLOCK_SIZE:-0x20000}
	nand_block_size=$((nand_block_size / 1024))
	nand_page_size=${RK_NAND_PAGE_SIZE:-2048}
	nand_page_size=$((nand_page_size / 1024))
	nand_oob_size=${RK_NAND_OOB_SIZE:-128}

	# run programmer image tool
	mkdir -p $FACTORY_FILE_DIR
	case $RK_BOOT_MEDIUM in
	emmc | sd_card)
		$PROGRAMMER_TOOL_PATH/programmer_image_tool -i $IMAGE_PATH -o $FACTORY_FILE_DIR -t emmc
		;;
	spi_nor)
		$PROGRAMMER_TOOL_PATH/programmer_image_tool -i $IMAGE_PATH -o $FACTORY_FILE_DIR -t spinor
		;;
	spi_nand)
		msg_info "spi_nand default: block_size = ${nand_block_size}KB, page_size = ${nand_page_size}KB"
		$PROGRAMMER_TOOL_PATH/programmer_image_tool -i $IMAGE_PATH -o $FACTORY_FILE_DIR -t spinand -b $nand_block_size -p $nand_page_size
		;;
	slc_nand)
		msg_info "slc_nand default: block_size = ${nand_block_size}KB, page_size = ${nand_page_size}KB, oob_size = ${nand_oob_size}B"
		$PROGRAMMER_TOOL_PATH/programmer_image_tool -i $IMAGE_PATH -o $FACTORY_FILE_DIR -t slc -b $nand_block_size -p $nand_page_size -s $nand_oob_size
		;;
	*)
		echo "Not support storage medium type: $RK_BOOT_MEDIUM"
		exit 1
		;;
	esac

	finish_build
}

function build_all() {
	echo "============================================"
	echo "TARGET_ARCH=$RK_ARCH"
	echo "TARGET_UBOOT_CONFIG=$RK_UBOOT_DEFCONFIG $RK_UBOOT_DEFCONFIG_FRAGMENT"
	echo "TARGET_SPL_CONFIG=$RK_SPL_DEFCONFIG"
	echo "TARGET_KERNEL_CONFIG=$RK_KERNEL_DEFCONFIG $RK_KERNEL_DEFCONFIG_FRAGMENT"
	echo "TARGET_KERNEL_DTS=$RK_KERNEL_DTS"
	echo "TARGET_RECOVERY_CONFIG=$RK_CFG_RECOVERY"
	echo "TARGET_RAMBOOT_CONFIG=$RK_CFG_RAMBOOT"
	echo "============================================"

	build_sysdrv
	build_media
	build_app
	build_firmware

	finish_build
}

function build_clean() {
	param="${1:-all}"
	msg_info "clean ${param}"
	case $param in
	uboot)
		make uboot_clean -C ${SDK_SYSDRV_DIR}
		;;
	kernel)
		make kernel_clean -C ${SDK_SYSDRV_DIR}
		;;
	rootfs)
		make rootfs_clean -C ${SDK_SYSDRV_DIR}
		rm -rf $RK_PROJECT_PACKAGE_ROOTFS_DIR
		;;
	driver)
		make drv_clean -C ${SDK_SYSDRV_DIR}
		rm -rf $RK_PROJECT_PATH_SYSDRV/kernel_drv_ko
		;;
	sysdrv)
		make distclean -C ${SDK_SYSDRV_DIR}
		rm -rf $RK_PROJECT_PATH_SYSDRV
		;;
	media)
		make distclean -C ${SDK_MEDIA_DIR}
		rm -rf $RK_PROJECT_PATH_MEDIA
		;;
	app)
		make distclean -C ${SDK_APP_DIR}
		rm -rf $RK_PROJECT_PATH_APP
		;;
	recovery)
		make kernel_clean -C ${SDK_SYSDRV_DIR} SYSDRV_BUILD_RECOVERY=y
		;;
	patch)
		cd ${SDK_ROOT_DIR}
		make uboot_clean -C ${SDK_SYSDRV_DIR}
		if [ -f ${SDK_SYSDRV_DIR}/source/.uboot_patch ]; then
			git apply -R --verbose ${SDK_SYSDRV_DIR}/tools/board/uboot/*.patch
			rm -rf ${SDK_SYSDRV_DIR}/source/uboot/u-boot/arch/arm/dts/*luckfox*
			rm -rf ${SDK_SYSDRV_DIR}/source/uboot/u-boot/configs/*luckfox*
			rm ${SDK_SYSDRV_DIR}/source/.uboot_patch
		fi

		make kernel_clean -C ${SDK_SYSDRV_DIR}
		if [ -f ${SDK_SYSDRV_DIR}/source/.kernel_patch ]; then
			git apply -R --verbose ${SDK_SYSDRV_DIR}/tools/board/kernel/*.patch
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/logo_linux_clut224.ppm ${SDK_SYSDRV_DIR}/source/kernel/drivers/video/logo/logo_linux_clut224.ppm
			rm -rf ${SDK_SYSDRV_DIR}/source/kernel/arch/arm/configs/*luckfox*
			rm -rf ${SDK_SYSDRV_DIR}/source/kernel/arch/arm/boot/dts/*luckfox*
			rm ${SDK_SYSDRV_DIR}/source/.kernel_patch
		fi
		;;
	all)
		make distclean -C ${SDK_SYSDRV_DIR}
		make distclean -C ${SDK_MEDIA_DIR}
		make distclean -C ${SDK_APP_DIR}
		rm -rf ${RK_PROJECT_OUTPUT_IMAGE} ${RK_PROJECT_OUTPUT}
		rm -rf ${DTS_CONFIG} ${KERNEL_DEFCONFIG} ${BUILDROOT_DEFCONFIG}
		rm -rf ${SDK_ROOT_DIR}/output ${SDK_ROOT_DIR}/config
		rm -rf ${SDK_ROOT_DIR}/sysdrv/source/kernel/out
		rm -rf ${BOARD_CONFIG}
		if [ -d ${SDK_SYSDRV_DIR}/source/buildroot ] && [ "$LF_TARGET_ROOTFS" = "buildroot" ]; then
			rm -rf ${SDK_SYSDRV_DIR}/source/buildroot
		fi
		if [ -d ${SDK_SYSDRV_DIR}/source/busybox ]; then
			rm -rf ${SDK_SYSDRV_DIR}/source/busybox
		fi
		if [ -d ${SDK_SYSDRV_DIR}/source/objs_kernel ]; then
			rm -rf ${SDK_SYSDRV_DIR}/source/objs_kernel
		fi
		;;
	*)
		msg_warn "clean [$1] not support, ignore"
		;;
	esac

	finish_build
}

function __BUILD_ENABLE_COREDUMP_SCRIPT() {
	local tmp_path coredump2sdcard
	rm -f $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/profile.d/enable_coredump.sh

	tmp_path=$RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/profile.d/enable_coredump.sh
	coredump2sdcard="$RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/coredump2sdcard.sh"
	cat >$coredump2sdcard <<EOF
#!/bin/sh
exec cat -  > "/mnt/sdcard/core-\$1-\$2"
EOF
	chmod a+x $coredump2sdcard

	cat >$tmp_path <<EOF
#!/bin/sh
if [ "\$(id -u)" = "0" ]; then
ulimit -c unlimited
echo "/data/core-%p-%e" > /proc/sys/kernel/core_pattern
echo "| /usr/bin/coredump2sdcard.sh %p %e" > /proc/sys/kernel/core_pattern
fi
EOF
	chmod u+x $tmp_path
}

function __RELEASE_FILESYSTEM_FILES() {
	if [ "$RK_BUILD_VERSION_TYPE" = "DEBUG" ]; then
		msg_info "RK_BUILD_VERSION_TYPE is DEBUG, ignore strip symbols"
		return
	fi
	local _target_dir
	_target_dir=$1
	msg_info "start to strip $_target_dir"
	if [ -d "$_target_dir" -a -n "${RK_PROJECT_TOOLCHAIN_CROSS}" ]; then
		find "$_target_dir" \( -name "lib*.la" -o -name "lib*.a" \) | xargs rm -rf {}
		rm -rf $(find "$_target_dir" -name pkgconfig)
		find "$_target_dir" -type f \( -perm /111 -o -name '*.so*' \) \
			-not \( -name 'libpthread*.so*' -o -name 'ld-*.so*' -o -name '*.ko' \) -print0 |
			xargs -0 ${RK_PROJECT_TOOLCHAIN_CROSS}-strip 2>/dev/null || true
		find "$_target_dir" -type f -name '*.ko' -print0 |
			xargs -0 ${RK_PROJECT_TOOLCHAIN_CROSS}-strip --strip-debug 2>/dev/null || true
	else
		msg_warn "not found target dir: $_target_dir, ignore"
	fi
}

function __COPY_FILES() {
	mkdir -p "$2"
	if [ -d "$1" ]; then
		cp -rfa $1/* $2
	else
		msg_warn "Please check path [$1] [$2] again"
	fi
}

function __PACKAGE_RESOURCES() {
	local _iqfiles_dir _install_dir _target_dir _avs_calib_install_dir _avs_calib_src
	_target_dir="$1"
	if [ ! -d $_target_dir ]; then
		msg_error "Not found target dir: $_target_dir"
		exit 1
	fi

	_install_dir=$_target_dir/usr
	_iqfiles_dir=$_install_dir/share/iqfiles

	__COPY_FILES $RK_PROJECT_PATH_SYSDRV/kernel_drv_ko/ $_install_dir/ko

	__COPY_FILES $RK_PROJECT_PATH_APP/bin $_install_dir/bin/
	__COPY_FILES $RK_PROJECT_PATH_APP/lib $_install_dir/lib/
	__COPY_FILES $RK_PROJECT_PATH_APP/share $_install_dir/share/
	__COPY_FILES $RK_PROJECT_PATH_APP/usr $_install_dir/
	__COPY_FILES $RK_PROJECT_PATH_APP/etc $_install_dir/etc/

	__COPY_FILES $RK_PROJECT_PATH_MEDIA/bin $_install_dir/bin/
	__COPY_FILES $RK_PROJECT_PATH_MEDIA/lib $_install_dir/lib/
	__COPY_FILES $RK_PROJECT_PATH_MEDIA/share $_install_dir/share/
	__COPY_FILES $RK_PROJECT_PATH_MEDIA/usr $_install_dir/

	_avs_calib_install_dir=$_install_dir/share/avs_calib
	mkdir -p $_avs_calib_install_dir
	if [ -n "$RK_AVS_CALIB" ]; then
		IFS=" "
		for item in $(echo $RK_AVS_CALIB); do
			_avs_calib_src=$RK_PROJECT_PATH_MEDIA/avs_calib/$item
			if [ -f "$_avs_calib_src" ]; then
				if [[ $_avs_calib_src =~ .*pto$ ]]; then
					cp -rfa $_avs_calib_src $_avs_calib_install_dir/calib_file.pto
				else
					cp -rfa $_avs_calib_src $_avs_calib_install_dir/calib_file.xml
				fi
			fi
		done
		IFS=
	fi

	if [ -n "$RK_AVS_LUT" ]; then
		_avs_lut_src=$(find $RK_PROJECT_PATH_MEDIA -name $RK_AVS_LUT)
		if [ -n "$_avs_lut_src" ]; then
			_avs_lut_install_dir=$_install_dir/share/middle_lut
			mkdir -p $_avs_lut_install_dir
			cp -rfa $_avs_lut_src $_avs_lut_install_dir/
		fi
	fi

	mkdir -p $_iqfiles_dir
	if [ -n "$RK_CAMERA_SENSOR_IQFILES" ]; then
		IFS=" "
		for item in $(echo $RK_CAMERA_SENSOR_IQFILES); do
			if [ -f "$RK_PROJECT_PATH_MEDIA/isp_iqfiles/$item" ]; then
				cp -rfa $RK_PROJECT_PATH_MEDIA/isp_iqfiles/$item $_iqfiles_dir
			fi
		done
		IFS=
	else
		msg_warn "Not found RK_CAMERA_SENSOR_IQFILES on the $(realpath $BOARD_CONFIG), copy all default for emmc, sd-card or nand"
		if [ "$RK_BOOT_MEDIUM" != "spi_nor" ]; then
			cp -rfa $RK_PROJECT_PATH_MEDIA/isp_iqfiles/* $_iqfiles_dir
		fi
	fi

	if [ -n "$RK_CAMERA_SENSOR_CAC_BIN" ]; then
		IFS=" "
		for item in $(echo $RK_CAMERA_SENSOR_CAC_BIN); do
			if [ -d "$RK_PROJECT_PATH_MEDIA/isp_iqfiles/$item" ]; then
				cp -rfa $RK_PROJECT_PATH_MEDIA/isp_iqfiles/$item $_iqfiles_dir
			fi
		done
		IFS=
	fi
}

function __MAKE_MOUNT_SCRIPT() {
	echo "$1" >>$RK_PROJECT_FILE_ROOTFS_SCRIPT
}

function __PACKAGE_OEM() {
	mkdir -p $RK_PROJECT_PACKAGE_OEM_DIR
	__PACKAGE_RESOURCES $RK_PROJECT_PACKAGE_OEM_DIR
	if [ -d "$RK_PROJECT_PACKAGE_OEM_DIR/usr/share/iqfiles" ]; then
		(
			cd $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc
			ln -sf ../oem/usr/share/iqfiles ./
		)
	fi

	mkdir -p $(dirname $RK_PROJECT_FILE_OEM_SCRIPT)
	cat >$RK_PROJECT_FILE_OEM_SCRIPT <<EOF
#!/bin/sh
[ -f /etc/profile.d/RkEnv.sh ] && source /etc/profile.d/RkEnv.sh
case \$1 in
	start)
		sh /oem/usr/bin/RkLunch.sh
		;;
	stop)
		sh /oem/usr/bin/RkLunch-stop.sh
		;;
	*)
		exit 1
		;;
esac
EOF

	chmod a+x $RK_PROJECT_FILE_OEM_SCRIPT
	cp -f $RK_PROJECT_FILE_OEM_SCRIPT $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d
}

function __PACKAGE_USERDATA() {
	local media_userdata app_userdata
	userdata_media="$RK_PROJECT_PATH_MEDIA/install_to_userdata"
	userdata_app="$RK_PROJECT_PATH_APP/install_to_userdata"

	if [ -d "$userdata_media" ]; then
		if [ "$(ls -A $userdata_media)" ]; then
			cp -rfa $RK_PROJECT_PATH_MEDIA/install_to_userdata/* $RK_PROJECT_PACKAGE_USERDATA_DIR
		fi
	fi
	if [ -d "$userdata_app" ]; then
		if [ "$(ls -A $userdata_app)" ]; then
			cp -rfa $RK_PROJECT_PATH_MEDIA/install_to_userdata/* $RK_PROJECT_PACKAGE_USERDATA_DIR
		fi
	fi
}

function __PACKAGE_ROOTFS() {
	local rootfs_tarball rootfs_out_dir
	rootfs_tarball="$RK_PROJECT_PATH_SYSDRV/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}.tar"

	if [ ! -f $rootfs_tarball ]; then
		msg_error "Build rootfs is not yet complete, packaging cannot proceed!"
		exit 0
	fi

	if [ "$RK_BOOT_MEDIUM" == "emmc" ] && [ "$LF_TARGET_ROOTFS" == "ubuntu" ]; then
		if [ -f $WIFI_CONF ]; then
			cp $WIFI_CONF $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc
		fi
	fi

	if [ "$LF_TARGET_ROOTFS" == "buildroot" ] || [ "$LF_TARGET_ROOTFS" == "busybox" ]; then
		build_get_sdk_version
		cat >$RK_PROJECT_PACKAGE_ROOTFS_DIR/bin/sdkinfo <<EOF
#!/bin/sh
echo Build Time:  $(date "+%Y-%m-%d-%T")
echo SDK Version: ${GLOBAL_SDK_VERSION}
EOF
		chmod a+x $RK_PROJECT_PACKAGE_ROOTFS_DIR/bin/sdkinfo
		__COPY_FILES $RK_PROJECT_PATH_APP/root $RK_PROJECT_PACKAGE_ROOTFS_DIR
	fi
	__COPY_FILES $RK_PROJECT_PATH_MEDIA/root $RK_PROJECT_PACKAGE_ROOTFS_DIR
	__COPY_FILES $SDK_ROOT_DIR/external $RK_PROJECT_PACKAGE_ROOTFS_DIR

	if [ -d "$RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/share/iqfiles" ]; then
		(
			cd $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc
			ln -sf ../usr/share/iqfiles ./
		)
	fi

	if [ -f $RK_PROJECT_FILE_ROOTFS_SCRIPT ]; then
		chmod a+x $RK_PROJECT_FILE_ROOTFS_SCRIPT
		cp -f $RK_PROJECT_FILE_ROOTFS_SCRIPT $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d
	fi
}

function parse_partition_env() {
	local env_all_flag env_final env_final_offset
	local part_size part_offset part_name part_final partitions tmp_part_offset tmp_part_offset_b16
	local part_size_bytes part_offset_bytes size_final_char offset_final_char part_size_bytes_b16

	if [[ -z $RK_PARTITION_CMD_IN_ENV ]]; then
		msg_error "No found partition, please check RK_PARTITION_CMD_IN_ENV in BoardConfig: $BOARD_CONFIG"
		exit 1
	fi

	# format be like: "4M(uboot),32K(env),32M(boot),1G(rootfs),-(userdata)"
	IFS=,
	env_all_flag=0
	tmp_part_offset=0

	read -ra env_arr <<<"$RK_PARTITION_CMD_IN_ENV"
	env_final=${env_arr[-1]}
	if [[ $env_final =~ "@" ]]; then
		env_final_offset=$(echo $env_final | cut -s -d '(' -f1 | cut -s -d '@' -f2)
		if [[ $((env_final_offset)) == 0 ]]; then
			env_all_flag=1
		fi
	fi
	for part in $RK_PARTITION_CMD_IN_ENV; do
		part_size=$(echo $part | cut -s -d '(' -f1 | cut -d '@' -f1)
		part_name=$(echo $part | cut -s -d '(' -f2 | cut -s -d ')' -f1)
		part_final=$(echo $part | cut -s -d '(' -f2 | cut -s -d ')' -f2)

		if [[ -z $part_size || -z $part_name || -n $part_final ]]; then
			msg_error "Parse partition failed, exit !!!"
			msg_error "Please check the partition format: $RK_PARTITION_CMD_IN_ENV"
			exit 1
		fi

		# parse offset
		if [[ $part =~ "@" ]]; then
			part_offset=$(echo $part | cut -s -d '(' -f1 | cut -s -d '@' -f2)
			offset_final_char=${part_offset: -1}
			case $offset_final_char in
			K | k)
				part_offset=$((${part_offset/%${offset_final_char}/}))
				part_offset_bytes=$(($part_offset * 1024))
				;;
			M | m)
				part_offset=$((${part_offset/%${offset_final_char}/}))
				part_offset_bytes=$(($part_offset * 1024 * 1024))
				;;
			G | g)
				part_offset=$((${part_offset/%${offset_final_char}/}))
				part_offset_bytes=$(($part_offset * 1024 * 1024 * 1024))
				;;
			T | t)
				part_offset=$((${part_offset/%${offset_final_char}/}))
				part_offset_bytes=$(($part_offset * 1024 * 1024 * 1024 * 1024))
				;;
			P | p)
				part_offset=$((${part_offset/%${offset_final_char}/}))
				part_offset_bytes=$(($part_offset * 1024 * 1024 * 1024 * 1024 * 1024))
				;;
			E | e)
				part_offset=$((${part_offset/%${offset_final_char}/}))
				part_offset_bytes=$(($part_offset * 1024 * 1024 * 1024 * 1024 * 1024 * 1024))
				;;
			-)
				if [[ ${#part_offset} != 1 ]]; then
					msg_error "Partition($part_name) offset($part_offset) error, exit !!!"
					exit 1
				fi
				part_offset_bytes=$part_offset
				;;
			*)
				part_offset_bytes=$(($part_offset))
				if [[ $part_offset_bytes == 0 && "$part_offset" != "0" && "$part_offset" != "0x0" ]]; then
					msg_error "Partition '$part_name' offset '$part_offset' error, exit !!!"
					exit 1
				fi
				;;
			esac
		else
			part_offset_bytes=
		fi

		# parse partition size
		size_final_char=${part_size: -1}
		case $size_final_char in
		K | k)
			part_size=$((${part_size/%${size_final_char}/}))
			part_size_bytes=$(($part_size * 1024))
			;;
		M | m)
			part_size=$((${part_size/%${size_final_char}/}))
			part_size_bytes=$(($part_size * 1024 * 1024))
			;;
		G | g)
			part_size=$((${part_size/%${size_final_char}/}))
			part_size_bytes=$(($part_size * 1024 * 1024 * 1024))
			;;
		T | t)
			part_size=$((${part_size/%${size_final_char}/}))
			part_size_bytes=$(($part_size * 1024 * 1024 * 1024 * 1024))
			;;
		P | p)
			part_size=$((${part_size/%${size_final_char}/}))
			part_size_bytes=$(($part_size * 1024 * 1024 * 1024 * 1024 * 1024))
			;;
		E | e)
			part_size=$((${part_size/%${size_final_char}/}))
			part_size_bytes=$(($part_size * 1024 * 1024 * 1024 * 1024 * 1024 * 1024))
			;;
		-)
			if [[ ${#part_size} != 1 ]]; then
				msg_error "Partition($part_name) size($part_size) error, exit !!!"
				exit 1
			fi
			part_size_bytes=$part_size
			;;
		*)
			part_size_bytes=$(($part_size))
			;;
		esac

		# Judge the validity of parameters
		if [[ $part_size_bytes == 0 ]]; then
			msg_error "Error: partition($part_name) size equal to 0, exit !!!"
			exit 1
		fi
		if [[ -n "${part_offset_bytes}" ]]; then
			if [[ "$part" == "$env_final" && "$env_all_flag" == 1 ]]; then
				tmp_part_offset=$part_offset_bytes
			else
				if [[ $((part_offset_bytes)) -ge $((tmp_part_offset)) ]]; then
					tmp_part_offset=$part_offset_bytes
				else
					msg_error "Partition($part_name) offset set too small, exit !!!"
					exit 1
				fi
			fi
		fi

		# Convert base 10 to base 16
		if [[ $part_size_bytes =~ "-" ]]; then
			part_size_bytes_b16="-"
		else
			part_size_bytes_b16="0x$(echo "obase=16;$part_size_bytes" | bc)"
		fi
		if [[ $tmp_part_offset =~ "-" ]]; then
			tmp_part_offset_b16="-"
		else
			tmp_part_offset_b16="0x$(echo "obase=16;$tmp_part_offset" | bc)"
		fi
		if [ "$tmp_part_offset_b16" = "0x" ]; then
			tmp_part_offset_b16="0"
		fi

		partitions="$partitions,$part_size_bytes_b16@$tmp_part_offset_b16($part_name)"
		[[ $part_size_bytes =~ "-" || $tmp_part_offset =~ "-" ]] || tmp_part_offset=$((tmp_part_offset + part_size_bytes))
	done
	IFS=
	GLOBAL_PARTITIONS="${partitions/,/}"
	echo "GLOBAL_PARTITIONS: $GLOBAL_PARTITIONS"
}

function parse_partition_file() {
	local part_size part_offset part_name part_num storage_dev_prefix
	export SYS_BOOTARGS
	export RK_PARTITION_ARGS

	parse_partition_env

	mkdir -p $RK_PROJECT_OUTPUT_IMAGE
	case $RK_BOOT_MEDIUM in
	emmc)
		storage_dev_prefix=mmcblk0p
		RK_PARTITION_ARGS="blkdevparts=mmcblk0:$RK_PARTITION_CMD_IN_ENV"
		part_num=1
		;;
	sd_card)
		storage_dev_prefix=mmcblk1p
		RK_PARTITION_ARGS="blkdevparts=mmcblk1:$RK_PARTITION_CMD_IN_ENV"
		part_num=1
		;;
	spi_nor)
		storage_dev_prefix=mtdblock
		RK_PARTITION_ARGS="mtdparts=sfc_nor:$RK_PARTITION_CMD_IN_ENV"
		part_num=0
		;;
	spi_nand)
		storage_dev_prefix=mtd
		RK_PARTITION_ARGS="mtdparts=spi-nand0:$RK_PARTITION_CMD_IN_ENV"
		part_num=0
		fit_target_optional_param="preload_none"
		;;
	slc_nand)
		storage_dev_prefix=mtd
		RK_PARTITION_ARGS="mtdparts=rk-nand:$RK_PARTITION_CMD_IN_ENV"
		part_num=0
		fit_target_optional_param="preload_none"
		;;
	*)
		msg_error "Not support storage medium type: $RK_BOOT_MEDIUM"
		finish_build
		exit 1
		;;
	esac
	echo "${RK_PARTITION_ARGS}" >$ENV_CFG_FILE

	mkdir -p $(dirname $RK_PROJECT_FILE_ROOTFS_SCRIPT)
	echo "#!/bin/sh" >$RK_PROJECT_FILE_ROOTFS_SCRIPT
	echo "bootmedium=$RK_BOOT_MEDIUM" >>$RK_PROJECT_FILE_ROOTFS_SCRIPT
	echo "linkdev(){" >>$RK_PROJECT_FILE_ROOTFS_SCRIPT
	echo 'if [ ! -d "/dev/block/by-name" ];then' >>$RK_PROJECT_FILE_ROOTFS_SCRIPT
	echo "mkdir -p /dev/block/by-name" >>$RK_PROJECT_FILE_ROOTFS_SCRIPT
	echo "cd /dev/block/by-name" >>$RK_PROJECT_FILE_ROOTFS_SCRIPT

	SYS_BOOTARGS="sys_bootargs="
	IFS=,
	for part in $GLOBAL_PARTITIONS; do
		part_size=$(echo $part | cut -d '@' -f1)
		part_offset=$(echo $part | cut -d '(' -f1 | cut -d '@' -f2)
		part_name=$(echo $part | cut -d '(' -f2 | cut -d ')' -f1)

		if [[ $part_size =~ "-" ]]; then
			part_name=${part_name%%:*}
		fi

		if [[ $part_name == "env" ]]; then
			export ENV_SIZE="$part_size"
			export ENV_OFFSET="$part_offset"
		fi

		if [[ $part_name == "meta" ]]; then
			if [ -n "$RK_META_SIZE" ]; then
				msg_info "RK_META_SIZE is $RK_META_SIZE"
			else
				export RK_META_SIZE="$part_size"
			fi
		fi

		if [[ ${part_name%_[a]} == "rootfs" || ${part_name%_[a]} == "system" ]]; then
			case $RK_BOOT_MEDIUM in
			emmc)
				if [ "$RK_ENABLE_FASTBOOT" = "y" -a "$RK_ENABLE_RAMDISK_PARTITION" = "y" ]; then
					SYS_BOOTARGS="${SYS_BOOTARGS} root=/dev/rd0"
				else
					SYS_BOOTARGS="${SYS_BOOTARGS} root=/dev/mmcblk0p${part_num}"
				fi
				;;
			sd_card)
				if [ "$RK_ENABLE_FASTBOOT" = "y" -a "$RK_ENABLE_RAMDISK_PARTITION" = "y" ]; then
					SYS_BOOTARGS="${SYS_BOOTARGS} root=/dev/rd0"
				else
					SYS_BOOTARGS="${SYS_BOOTARGS} root=/dev/mmcblk1p${part_num}"
				fi
				;;
			spi_nor)
				if [ "$RK_ENABLE_FASTBOOT" = "y" -a "$RK_ENABLE_RAMDISK_PARTITION" = "y" ]; then
					SYS_BOOTARGS="${SYS_BOOTARGS} root=/dev/rd0"
				else
					SYS_BOOTARGS="${SYS_BOOTARGS} root=/dev/mtdblock${part_num}"
				fi
				;;
			spi_nand | slc_nand)
				SYS_BOOTARGS="${SYS_BOOTARGS} ubi.mtd=${part_num}"
				;;
			*)
				msg_error "Not support storage medium type: $RK_BOOT_MEDIUM"
				finish_build
				exit 1
				;;
			esac
		fi

		echo "ln -sf /dev/${storage_dev_prefix}${part_num} ${part_name}" >>$RK_PROJECT_FILE_ROOTFS_SCRIPT
		part_num=$((part_num + 1))
	done
	case $RK_BOOT_MEDIUM in
	sd_card)
		cat >>$RK_PROJECT_FILE_ROOTFS_SCRIPT <<EOF
for i in \$(seq 5 8); do
	det_partition="/dev/mmcblk1p\$i"
	mount_point=\$(mount | grep "\$det_partition" | awk '{print \$3}')
	if [ -n "\$mount_point" ]; then
	echo "Unmounting : \$det_partition (\$mount_point)"
	umount "\$det_partition"
	else
	echo "Partition is not mounted: \$det_partition"
	fi
done
EOF
		;;
	*) ;;
	esac
	IFS=
	echo "fi }" >>$RK_PROJECT_FILE_ROOTFS_SCRIPT

	cat >>$RK_PROJECT_FILE_ROOTFS_SCRIPT <<EOF
mount_part(){
if [ -z "\$1" -o -z "\$2" -o -z "\$3" ];then
	echo "Invalid paramter, exit !!!"
	exit 1
fi
root_dev=\$(mountpoint -n /)
root_dev=\${root_dev%% *}
partname=\$1
part_dev=/dev/block/by-name/\$1
mountpt=\$2
part_fstype=\$3
part_realdev=\$(realpath \$part_dev)

if [ ! -d \$mountpt ]; then
	if [ "\$mountpt" = "IGNORE" -a "emmc" = "\$bootmedium" ];then
		if [ "\$root_dev" = "\$part_realdev" ];then
			resize2fs \$part_dev
		fi
		return 0;
	elif [ "\$mountpt" = "IGNORE" -a "sd_card" = "\$bootmedium" ];then
		if [ "\$root_dev" = "\$part_realdev" ];then
			resize2fs \$part_dev
		fi
		return 0;
	else
		echo "\${0} info: mount point path [\$mountpt] not found, skip..."
		return 1;
	fi
fi

if test -h \$part_dev; then
case \$bootmedium in
	emmc|sd_card)
		if [ "\$root_dev" = "\$part_realdev" ];then
			resize2fs \$part_dev
		else
			e2fsck -y \$part_dev
			mount -t \$part_fstype \$part_dev \$mountpt
			if [ \$? -eq 0 ]; then
				resize2fs \$part_dev
				tune2fs \$part_dev -L \$partname
			else
				echo "mount \$partname error, try to format..."
				mke2fs -F -L \$partname \$part_dev  && \
					tune2fs -c 0 -i 0 \$part_dev && \
					mount -t \$part_fstype \$part_dev \$mountpt
			fi
		fi
		;;
	spi_nand|slc_nand)
		if [ \$partname = "rootfs" ];then
			echo "rootfs mount on \$root_dev"
		elif [ "\$part_fstype" = "ubifs" ]; then
			part_no=\$(echo \$part_realdev | grep -oE "[0-9]*$")
			ubi_dev=/dev/ubi\${part_no}
			ubi_vol=\${ubi_dev}_0
			mount | grep \$ubi_vol
			if [ \$? -eq 0 ];then
				echo "***********\$partname has been mounted***********"
			else
				if [ ! -e \$ubi_vol ];then
					echo "***********\$ubi_vol not exist***********"
					if [ ! -e \$ubi_dev ];then
						echo "***********\$ubi_dev not exist***********"
						ubiattach /dev/ubi_ctrl -m \$part_no -d \$part_no
						if [ \$? -ne 0 ];then
							echo "ubiattach \$part_realdev error, try to format..."
							ubiformat -y \$part_realdev
							ubiattach /dev/ubi_ctrl -m \$part_no -d \$part_no
						fi
					fi
					ubi_info_dir=/sys/class/ubi/ubi\${part_no}
					avail_eraseblocks=\$(cat \$ubi_info_dir/avail_eraseblocks)
					eraseblock_size=\$(cat \$ubi_info_dir/eraseblock_size)
					echo "try to make volume: \$ubi_vol ..."
					ubimkvol \$ubi_dev -N \$partname -s \$((avail_eraseblocks*eraseblock_size))
				fi
				mount -t \$part_fstype \$ubi_vol \$mountpt
			fi
		elif [ "\$part_fstype" = "squashfs" ]; then
			part_no=\$(echo \$part_realdev | grep -oE "[0-9]*$")
			ubi_dev=/dev/ubi\${part_no}
			ubi_vol=\${ubi_dev}_0
			ubi_block=/dev/ubiblock\${part_no}_0
			mount | grep \$ubi_block
			if [ \$? -eq 0 ];then
				echo "***********\$partname has been mounted***********"
			else
				if [ ! -e \$ubi_block ];then
					echo "***********\$ubi_block not exist***********"
					ubiattach /dev/ubi_ctrl -m \$part_no -d \$part_no
					if [ \$? -ne 0 ];then
						echo "ubiattach \$part_realdev error, return !!!"
						echo "Please check the device: \$part_realdev"
						return 1
					fi
					ubiblock -c \$ubi_vol
				fi
				mount -t \$part_fstype \$ubi_block \$mountpt
			fi
		elif [ "\$part_fstype" = "erofs" ]; then
			part_no=\$(echo \$part_realdev | grep -oE "[0-9]*$")
			ubi_dev=/dev/ubi\${part_no}
			ubi_vol=\${ubi_dev}_0
			ubi_block=/dev/ubiblock\${part_no}_0
			mount | grep \$ubi_block
			if [ \$? -eq 0 ];then
				echo "***********\$partname has been mounted***********"
			else
				if [ ! -e \$ubi_block ];then
					echo "***********\$ubi_block not exist***********"
					ubiattach /dev/ubi_ctrl -m \$part_no -d \$part_no
					if [ \$? -ne 0 ];then
						echo "ubiattach \$part_realdev error, return !!!"
						echo "Please check the device: \$part_realdev"
						return 1
					fi
					ubiblock -c \$ubi_vol
				fi
				mount -t \$part_fstype \$ubi_block \$mountpt
			fi
		else
			echo "Error: wrong filesystem type: \$part_fstype, return !!!"
			return 1
		fi
		;;
	spi_nor)
		if [ "\$root_dev" = "\$part_realdev" ];then
			echo "***********\$part_dev has been mounted, skipping***********"
		else
			echo "mount -t \$part_fstype \$part_dev \$mountpt"
			mount -t \$part_fstype \$part_dev \$mountpt
			if [ \$? -eq 0 ]; then
				echo "***********succeed in mounting***********"
			elif [ "\$part_fstype" = "jffs2" ]; then
				echo "mount \$partname error, try to format..."
				echo "flash_erase -j \${part_realdev/block/} 0 0 && mount -t \$part_fstype \$part_dev \$mountpt"
				flash_erase -j \${part_realdev/block/} 0 0 && mount -t \$part_fstype \$part_dev \$mountpt
			else
				echo "mount \$partname error, skipping! Please check the filesystem."
			fi
		fi
		;;
	*)
		echo "Invalid Parameter: Check bootmedium !!!"
		exit 1
		;;
esac
fi
}
EOF
}

function get_partition_num() {
	local target_part_name partitions
	target_part_name=$1
	partitions=${GLOBAL_PARTITIONS}
	if [ -z "$target_part_name" -o -z "$partitions" ]; then
		msg_error "Invalid paramter, exit !!!"
		exit 1
	fi

	IFS=,
	local part_size part_name part_size_bytes part_num
	part_num=0
	for part in $partitions; do
		part_size=$(echo $part | cut -d '@' -f1)
		part_name=$(echo $part | cut -d '(' -f2 | cut -d ')' -f1)

		[[ $part_size =~ "-" ]] && continue

		part_size_bytes=$(($part_size))

		if [ "$part_name" = "$target_part_name" ]; then
			echo "$part_num"
			IFS=
			return
		fi
		part_num=$((part_num + 1))
	done
	IFS=

	echo "error"
	return
}

function get_partition_size() {
	local target_part_name partitions
	target_part_name=$1
	partitions=${GLOBAL_PARTITIONS}
	if [ -z "$target_part_name" -o -z "$partitions" ]; then
		msg_error "Invalid paramter, exit !!!"
		exit 1
	fi

	IFS=,
	local part_size part_name part_size_bytes
	for part in $partitions; do
		part_size=$(echo $part | cut -d '@' -f1)
		part_name=$(echo $part | cut -d '(' -f2 | cut -d ')' -f1)

		[[ $part_size =~ "-" ]] && continue

		part_size_bytes=$(($part_size))

		if [ "${part_name%_[ab]}" == "${target_part_name%_[ab]}" ]; then
			echo "$part_size_bytes"
			IFS=
			return
		fi
	done
	IFS=

	echo "0"
	return
}

function __GET_TARGET_PARTITION_FS_TYPE() {
	check_config RK_PARTITION_FS_TYPE_CFG || return 0
	msg_info "Partition Filesystem Type Configure: $RK_PARTITION_FS_TYPE_CFG"
	__MAKE_MOUNT_SCRIPT "case "\$1" in start) linkdev;"

	if [ "$RK_ENABLE_RECOVERY" = "y" ]; then
		mkdir -p $(dirname $RK_PROJECT_FILE_RECOVERY_SCRIPT)
		cp -fa $RK_PROJECT_FILE_ROOTFS_SCRIPT $RK_PROJECT_FILE_RECOVERY_SCRIPT
	fi

	local part_fs_type part_name part_mountpoint
	IFS=,
	for part in $RK_PARTITION_FS_TYPE_CFG; do
		part_name=${part%%@*}
		part_fs_type=${part##*@}
		part_mountpoint=${part%@*}
		part_mountpoint=${part_mountpoint##*@}

		if echo $GLOBAL_PARTITIONS | grep -w $part_name &>/dev/null; then
			eval "export ${part_name%_[ab]}$GLOBAL_FS_TYPE_SUFFIX=$part_fs_type"
		else
			msg_error "Not found $part_name from RK_PARTITION_CMD_IN_ENV in BoardConfig: $BOARD_CONFIG"
			exit 1
		fi

		case $part_fs_type in
		ext[234] | jffs2 | squashfs | ubifs | cramfs | romfs | erofs) ;;
		initramfs)
			export OUTPUT_SYSDRV_RAMDISK_DIR=$RK_PROJECT_PATH_RAMDISK
			[[ $part_name =~ "boot" ]] && GLOBAL_INITRAMFS_BOOT_NAME=$part_name
			;;
		*)
			msg_error "Invalid Parameter,Check $(realpath $BOARD_CONFIG):RK_PARTITION_FS_TYPE_CFG !!!"
			exit 1
			;;
		esac

		if [[ $part_name =~ "rootfs" || $part_name =~ "system" ]]; then
			GLOBAL_ROOT_FILESYSTEM_NAME=${part_name%_[ab]}
			export RK_PROJECT_ROOTFS_TYPE=$part_fs_type
			case $RK_BOOT_MEDIUM in
			emmc | sd_card)
				SYS_BOOTARGS="$SYS_BOOTARGS rootfstype=$part_fs_type"
				;;
			spi_nor)
				SYS_BOOTARGS="$SYS_BOOTARGS rootfstype=$part_fs_type"
				;;
			spi_nand | slc_nand)
				case $part_fs_type in
				erofs)
					if [ "$RK_ENABLE_FASTBOOT" = "y" -a "$RK_ENABLE_RAMDISK_PARTITION" = "y" ]; then
						SYS_BOOTARGS="$SYS_BOOTARGS ubi.block=0,$GLOBAL_ROOT_FILESYSTEM_NAME root=/dev/rd0 rootfstype=$part_fs_type"
					else
						SYS_BOOTARGS="$SYS_BOOTARGS ubi.block=0,$GLOBAL_ROOT_FILESYSTEM_NAME root=/dev/ubiblock0_0 rootfstype=$part_fs_type"
					fi
					;;
				squashfs)
					SYS_BOOTARGS="$SYS_BOOTARGS ubi.block=0,$GLOBAL_ROOT_FILESYSTEM_NAME root=/dev/ubiblock0_0 rootfstype=$part_fs_type"
					;;
				ubifs)
					SYS_BOOTARGS="$SYS_BOOTARGS root=ubi0:$GLOBAL_ROOT_FILESYSTEM_NAME rootfstype=$part_fs_type"
					;;
				*)
					msg_error "Not support rootfs type: $part_fs_type"
					finish_build
					exit 1
					;;
				esac
				;;
			*)
				msg_error "Not support storage medium type: $RK_BOOT_MEDIUM"
				finish_build
				exit 1
				;;
			esac
		fi

		if [[ $part_name =~ "oem" ]]; then
			GLOBAL_OEM_NAME=${part_name%_[ab]}
			export RK_PROJECT_OEM_FS_TYPE=$part_fs_type
			[[ $RK_BUILD_APP_TO_OEM_PARTITION != "y" ]] && continue
		fi

		if [ "$part_name" = "userdata" -a "$RK_ENABLE_RECOVERY" = "y" ]; then
			echo "mount_part $part_name $part_mountpoint $part_fs_type ;" >>$RK_PROJECT_FILE_RECOVERY_SCRIPT
		fi

		__MAKE_MOUNT_SCRIPT "mount_part $part_name $part_mountpoint $part_fs_type ;"
	done
	IFS=

	if [ "$RK_ENABLE_RECOVERY" = "y" ]; then
		echo ";; stop) printf stop \$0 finished\n ;; *) echo Usage: \$0 {start|stop} exit 1 ;; esac" >>$RK_PROJECT_FILE_RECOVERY_SCRIPT
		chmod a+x $RK_PROJECT_FILE_RECOVERY_SCRIPT
	fi

	__MAKE_MOUNT_SCRIPT ";; linkdev) linkdev ;"
	__MAKE_MOUNT_SCRIPT ";; stop) printf stop \$0 finished\n ;; *) echo Usage: \$0 {start|stop} exit 1 ;; esac"
}

__GET_BOOTARGS_FROM_BOARD_CFG() {
	if [ -n "$RK_BOOTARGS_CMA_SIZE" ]; then
		SYS_BOOTARGS="$SYS_BOOTARGS rk_dma_heap_cma=$RK_BOOTARGS_CMA_SIZE"
	fi
}

__LINK_DEFCONFIG_FROM_BOARD_CFG() {
	mkdir -p ${SDK_CONFIG_DIR}
	if [[ "$LF_TARGET_ROOTFS" == "ubuntu" ]]; then
		sudo chmod a+rw $SDK_CONFIG_DIR
	fi

	if [ ! -f ${SDK_SYSDRV_DIR}/source/.kernel_patch ]; then
		echo "============Apply Kernel Patch============"
		cd ${SDK_ROOT_DIR}
		git apply ${SDK_SYSDRV_DIR}/tools/board/kernel/*.patch
		if [ $? -eq 0 ]; then
			msg_info "Patch applied successfully."
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*_defconfig ${KERNEL_PATH}/arch/arm/configs/
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*.config ${KERNEL_PATH}/arch/arm/configs/
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/kernel-drivers-video-logo_linux_clut224.ppm ${KERNEL_PATH}/drivers/video/logo/logo_linux_clut224.ppm
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*.dts ${KERNEL_PATH}/arch/arm/boot/dts
			cp ${SDK_SYSDRV_DIR}/tools/board/kernel/*.dtsi ${KERNEL_PATH}/arch/arm/boot/dts
			touch ${SDK_SYSDRV_DIR}/source/.kernel_patch
		else
			msg_error "Failed to apply the patch."
		fi
	fi

	if [ -n "$RK_KERNEL_DTS" ]; then
		rm -f $DTS_CONFIG
		ln -rfs $SDK_SYSDRV_DIR/source/kernel/arch/arm/boot/dts/$RK_KERNEL_DTS $DTS_CONFIG
		msg_info "switch to DTS: $(realpath $DTS_CONFIG)"
	fi
	if [ -n "$RK_KERNEL_DEFCONFIG" ]; then
		rm -f $KERNEL_DEFCONFIG
		ln -rfs $SDK_SYSDRV_DIR/source/kernel/arch/arm/configs/$RK_KERNEL_DEFCONFIG $KERNEL_DEFCONFIG
		msg_info "switch to kernel defconfig: $(realpath $KERNEL_DEFCONFIG)"
	fi

	if [ "$LF_TARGET_ROOTFS" = "buildroot" ]; then
		if [ -n "$RK_BUILDROOT_DEFCONFIG" ]; then
			if [ -f "$BUILDROOT_PATH/configs/$RK_BUILDROOT_DEFCONFIG" ]; then
				rm -f $BUILDROOT_DEFCONFIG
				ln -rfs $BUILDROOT_PATH/configs/$RK_BUILDROOT_DEFCONFIG $BUILDROOT_DEFCONFIG
				msg_info "switch to buildroot defconfig: $(realpath $BUILDROOT_DEFCONFIG)"
			else
				msg_info "use \" ./build.sh buildrootconfig\" to create buildroot_defconfig"
			fi
		fi
	else
		rm -f $BUILDROOT_DEFCONFIG
		msg_info "The root file system used is not buildroot"
	fi
}

function __PREPARE_BOARD_CFG() {
	parse_partition_file
	__GET_TARGET_PARTITION_FS_TYPE
	if [ "$RK_ENABLE_FASTBOOT" = "y" ]; then
		SYS_BOOTARGS="$SYS_BOOTARGS $RK_PARTITION_ARGS"
	fi
	__GET_BOOTARGS_FROM_BOARD_CFG

	export RK_KERNEL_CMDLINE_FRAGMENT=${SYS_BOOTARGS#sys_bootargs=}
}

function build_mkimg() {
	local src dst fs_type part_size part_name
	part_name=$1
	part_size=$(get_partition_size $part_name)
	if [ $((part_size)) -lt 1 ]; then
		msg_warn "Not found partition named [$part_name]"
		msg_warn "Please check RK_PARTITION_CMD_IN_ENV in BoardConfig: $BOARD_CONFIG"
		return
	fi

	if [ -z "$2" -o ! -d "$2" ]; then
		msg_error "Not exist source dir: $2"
		exit 1
	fi

	if [ ! -f $RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
		-o ! -f $RK_PROJECT_PATH_PC_TOOLS/mkimage \
		-o ! -f $RK_PROJECT_PATH_PC_TOOLS/compress_tool ]; then
		build_tool
	fi

	src=$2
	dst=$RK_PROJECT_OUTPUT_IMAGE/${part_name}.img

	fs_type=${part_name}$GLOBAL_FS_TYPE_SUFFIX
	fs_type="\$${fs_type}"
	fs_type=$(eval "echo ${fs_type}")

	__RELEASE_FILESYSTEM_FILES $src

	msg_info "src=$src"
	msg_info "dst=$dst"
	msg_info "fs_type=$fs_type"
	msg_info "part_name=$part_name"
	msg_info "part_size=$((part_size / 1024 / 1024))MB"

	case $fs_type in
	ext4)
		$RK_PROJECT_TOOLS_MKFS_EXT4 $src $dst $part_size
		;;
	jffs2)
		$RK_PROJECT_TOOLS_MKFS_JFFS2 $src $dst $part_size
		;;
	erofs)
		if [ $part_name == "boot" -o "$RK_ENABLE_RAMDISK_PARTITION" = "y" ]; then
			local kernel_dtb_file="$RK_PROJECT_PATH_FASTBOOT/${RK_KERNEL_DTS/%.dts/.dtb}"

			if [ "$RK_BOOT_MEDIUM" = "spi_nand" ]; then
				cat $RK_PROJECT_PATH_FASTBOOT/Image | $RK_PROJECT_PATH_PC_TOOLS/compress_tool -9 >$RK_PROJECT_PATH_FASTBOOT/Image.gz
			else
				cat $RK_PROJECT_PATH_FASTBOOT/Image | $RK_PROJECT_PATH_PC_TOOLS/compress_tool -f -5 >$RK_PROJECT_PATH_FASTBOOT/Image.gz
			fi
			cp -fa $PROJECT_TOP_DIR/scripts/$RK_CHIP-boot-tb.its $RK_PROJECT_PATH_FASTBOOT/boot-tb.its

			$RK_PROJECT_TOOLS_MKFS_EROFS $src $RK_PROJECT_PATH_FASTBOOT/rootfs_erofs.img
			cat $RK_PROJECT_PATH_FASTBOOT/rootfs_erofs.img | gzip -n -f -9 >$RK_PROJECT_PATH_FASTBOOT/rootfs_erofs.img.gz

			if [ "$RK_ENABLE_RAMDISK_PARTITION" = "y" ]; then
				cp -fa $PROJECT_TOP_DIR/scripts/$RK_CHIP-boot-tb-no-ramdisk.its $RK_PROJECT_PATH_FASTBOOT/boot-tb-no-ramdisk.its
				cp -fa $PROJECT_TOP_DIR/scripts/$RK_CHIP-boot-tb-ramdisk.its $RK_PROJECT_PATH_FASTBOOT/boot-tb-ramdisk.its
				$RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
					$RK_PROJECT_PATH_FASTBOOT/boot-tb-no-ramdisk.its \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/rootfs_erofs.img.gz) \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/Image.gz) \
					$(realpath $kernel_dtb_file) \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/resource.img) \
					$RK_PROJECT_OUTPUT_IMAGE/boot.img \
					$RK_ARCH \
					$fit_target_optional_param
				$RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
					$RK_PROJECT_PATH_FASTBOOT/boot-tb-ramdisk.its \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/rootfs_erofs.img.gz) \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/Image.gz) \
					$(realpath $kernel_dtb_file) \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/resource.img) \
					$dst \
					$RK_ARCH \
					$fit_target_optional_param
			else
				$RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
					$RK_PROJECT_PATH_FASTBOOT/boot-tb.its \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/rootfs_erofs.img.gz) \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/Image.gz) \
					$(realpath $kernel_dtb_file) \
					$(realpath $RK_PROJECT_PATH_FASTBOOT/resource.img) \
					$dst \
					$RK_ARCH \
					$fit_target_optional_param
			fi
		else
			if [ "$RK_BOOT_MEDIUM" = "emmc" -o "$RK_BOOT_MEDIUM" = "spi_nor" -o "$RK_BOOT_MEDIUM" = "sd_card" ]; then
				$RK_PROJECT_TOOLS_MKFS_EROFS $src $dst $RK_EROFS_COMP
			else
				$RK_PROJECT_TOOLS_MKFS_UBIFS $src $(dirname $dst) $part_size $part_name $fs_type $RK_EROFS_COMP
			fi
		fi
		;;
	squashfs)
		if [ "$RK_BOOT_MEDIUM" = "emmc" -o "$RK_BOOT_MEDIUM" = "spi_nor" -o "$RK_BOOT_MEDIUM" = "sd_card" ]; then
			$RK_PROJECT_TOOLS_MKFS_SQUASHFS $src $dst $RK_SQUASHFS_COMP
		else
			$RK_PROJECT_TOOLS_MKFS_UBIFS $src $(dirname $dst) $part_size $part_name $fs_type $RK_SQUASHFS_COMP
		fi
		;;
	ubifs)
		$RK_PROJECT_TOOLS_MKFS_UBIFS $src $(dirname $dst) $part_size $part_name $fs_type $RK_UBIFS_COMP
		;;
	romfs)
		if [ $part_name == "boot" ]; then
			local kernel_dtb_file="$RK_PROJECT_PATH_FASTBOOT/${RK_KERNEL_DTS/%.dts/.dtb}"

			cat $RK_PROJECT_PATH_FASTBOOT/Image | $RK_PROJECT_PATH_PC_TOOLS/compress_tool >$RK_PROJECT_PATH_FASTBOOT/Image.gz
			cp -fa $PROJECT_TOP_DIR/scripts/$RK_CHIP-boot-tb.its $RK_PROJECT_PATH_FASTBOOT/boot-tb.its

			$RK_PROJECT_TOOLS_MKFS_ROMFS $src $RK_PROJECT_PATH_FASTBOOT/rootfs_romfs.img
			cat $RK_PROJECT_PATH_FASTBOOT/rootfs_romfs.img | gzip -n -f -9 >$RK_PROJECT_PATH_FASTBOOT/rootfs_romfs.img.gz

			$RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
				$RK_PROJECT_PATH_FASTBOOT/boot-tb.its \
				$(realpath $RK_PROJECT_PATH_FASTBOOT/rootfs_romfs.img.gz) \
				$(realpath $RK_PROJECT_PATH_FASTBOOT/Image.gz) \
				$(realpath $kernel_dtb_file) \
				$(realpath $RK_PROJECT_PATH_FASTBOOT/resource.img) \
				$dst \
				$RK_ARCH \
				$fit_target_optional_param
		else
			$RK_PROJECT_TOOLS_MKFS_ROMFS $src $dst
		fi
		;;
	initramfs)
		if [[ $part_name =~ "boot" ]]; then
			local kernel_image
			local kernel_dtb_file="$RK_PROJECT_PATH_RAMDISK/${RK_KERNEL_DTS/%.dts/.dtb}"

			cp -fa $PROJECT_TOP_DIR/scripts/boot4recovery.its $RK_PROJECT_PATH_RAMDISK/boot4ramdisk.its

			case "$RK_ARCH" in
			arm)
				kernel_image="$RK_PROJECT_PATH_RAMDISK/zImage"
				;;
			arm64)
				kernel_image="$RK_PROJECT_PATH_RAMDISK/Image.lz4"
				;;
			*)
				echo "No such kernel image. ($RK_ARCH)"
				;;
			esac

			$RK_PROJECT_TOOLS_MKFS_INITRAMFS $src $RK_PROJECT_PATH_RAMDISK/initramfs.cpio
			cat $RK_PROJECT_PATH_RAMDISK/initramfs.cpio | gzip -n -f -9 >$RK_PROJECT_PATH_RAMDISK/initramfs.cpio.gz

			$RK_PROJECT_PATH_PC_TOOLS/mk-fitimage.sh \
				$RK_PROJECT_PATH_RAMDISK/boot4ramdisk.its \
				$RK_PROJECT_PATH_RAMDISK/initramfs.cpio.gz \
				$kernel_image \
				$kernel_dtb_file \
				$RK_PROJECT_PATH_RAMDISK/resource.img \
				$dst \
				$RK_ARCH \
				$fit_target_optional_param
		else
			$RK_PROJECT_TOOLS_MKFS_INITRAMFS $src $dst
		fi
		;;
	*)
		msg_error "Not support fs type: $fs_type"
		;;
	esac

	finish_build
}

function __RUN_POST_CLEAN_FILES() {
	echo "================================================================================"
	IFS=$RECORD_IFS

	local dir_find todo_files
	if [ -d "$RK_PROJECT_PACKAGE_ROOTFS_DIR" ]; then
		dir_find="$RK_PROJECT_PACKAGE_ROOTFS_DIR"
	fi
	if [ -d "$RK_PROJECT_PACKAGE_OEM_DIR" ]; then
		dir_find="$dir_find $RK_PROJECT_PACKAGE_OEM_DIR"
	fi

	if [ -n "$RK_AUDIO_MODEL" ]; then
		if find $dir_find -type f -name "rkaudio*.rknn" | grep -w $RK_AUDIO_MODEL; then
			echo "Found config RK_AUDIO_MODEL: $RK_AUDIO_MODEL"
			todo_files=$(find $dir_find -name "rkaudio*.rknn" | grep -v $RK_AUDIO_MODEL || echo "")
			if [ -n "$todo_files" ]; then
				rm -rfv $todo_files
			fi
		fi
		if [ "$RK_AUDIO_MODEL" = "NONE" ]; then
			todo_files=$(find $dir_find -name "rkaudio*.rknn")
			if [ -n "$todo_files" ]; then
				rm -rfv $todo_files
			fi
		fi
	fi
	echo "================================================================================"

	if [ -n "$RK_AIISP_MODEL" ]; then
		if find $dir_find -type f -name $RK_AIISP_MODEL | grep -w $RK_AIISP_MODEL; then
			echo "Found config RK_AIISP_MODEL: $RK_AIISP_MODEL"
			todo_files=$(find $dir_find -name "*.aiisp" | grep -v $RK_AIISP_MODEL || echo "")
			if [ -n "$todo_files" ]; then
				rm -rfv $todo_files
			fi
		fi
		if [ "$RK_AIISP_MODEL" = "NONE" ]; then
			todo_files=$(find $dir_find -name "librkpostisp.so" -o -name "*.aiisp" || echo "")
			if [ -n "$todo_files" ]; then
				rm -rfv $todo_files
			fi
		fi
	fi
	echo "================================================================================"

	if [ -n "$RK_NPU_MODEL" ]; then
		if find $dir_find -type f -name $RK_NPU_MODEL | grep -w $RK_NPU_MODEL; then
			todo_files=$(find $dir_find -type f -name "object*.data" | grep -v $RK_NPU_MODEL)
			if [ -n "$todo_files" ]; then
				rm -rfv $todo_files
			fi

			local target_npu_data
			target_npu_data=$(find $dir_find -type f -name $RK_NPU_MODEL)
			if [ ! "${RK_NPU_MODEL}x" = "object_detection_pfp.datax" ]; then
				if [ "$RK_BUILD_APP_TO_OEM_PARTITION" = "y" ]; then
					mkdir -p $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib
					mv $target_npu_data $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/object_detection_pfp.data
				else
					mkdir -p $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib
					mv $target_npu_data $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib/object_detection_pfp.data
				fi
			fi
		fi
		if [ "$RK_NPU_MODEL" = "NONE" ]; then
			todo_files=$(find $dir_find -name "object*.data" || echo "")
			if [ -n "$todo_files" ]; then
				rm -rfv $todo_files
			fi
		fi
	fi
	echo "================================================================================"

}

function __RUN_POST_BUILD_SCRIPT() {
	local tmp_path
	tmp_path=$(realpath $BOARD_CONFIG)
	tmp_path=$(dirname $tmp_path)
	if [ -f "$tmp_path/$RK_POST_BUILD_SCRIPT" ]; then
		$tmp_path/$RK_POST_BUILD_SCRIPT
	fi
	__RUN_POST_CLEAN_FILES
}

function post_overlay() {
	[ -n "$RK_POST_OVERLAY" ] || return 0

	local tmp_path
	tmp_path=$(realpath $BOARD_CONFIG)
	tmp_path=$(dirname $tmp_path)

	for overlay_dir in $RK_POST_OVERLAY; do
		if [ -d "$tmp_path/overlay/$overlay_dir" ]; then
			rsync -a --ignore-times --keep-dirlinks --chmod=u=rwX,go=rX --exclude .empty \
				$tmp_path/overlay/$overlay_dir/* $RK_PROJECT_PACKAGE_ROOTFS_DIR/
		fi
	done
}

function __RUN_PRE_BUILD_OEM_SCRIPT() {
	local tmp_path
	tmp_path=$(realpath $BOARD_CONFIG)
	tmp_path=$(dirname $tmp_path)

	__RUN_POST_CLEAN_FILES

	if [ -f "$tmp_path/$RK_PRE_BUILD_OEM_SCRIPT" ]; then
		bash -x $tmp_path/$RK_PRE_BUILD_OEM_SCRIPT
	fi

}

function __RUN_POST_BUILD_USERDATA_SCRIPT() {
	local tmp_path
	tmp_path=$(realpath $BOARD_CONFIG)
	tmp_path=$(dirname $tmp_path)
	if [ -f "$tmp_path/$RK_PRE_BUILD_USERDATA_SCRIPT" ]; then
		bash -x $tmp_path/$RK_PRE_BUILD_USERDATA_SCRIPT
	fi
}

function build_firmware() {
	check_config RK_PARTITION_CMD_IN_ENV || return 0

	build_env

	if [ "$RK_ENABLE_FASTBOOT" = "y" ]; then
		build_meta
	fi

	mkdir -p ${RK_PROJECT_OUTPUT_IMAGE}

	if [ "$RK_ENABLE_RECOVERY" = "y" -a -f $PROJECT_TOP_DIR/scripts/${RK_MISC:=recovery-misc.img} ]; then
		cp -fv $PROJECT_TOP_DIR/scripts/$RK_MISC ${RK_PROJECT_OUTPUT_IMAGE}/misc.img
	fi

	__PACKAGE_ROOTFS
	__PACKAGE_OEM

	__BUILD_ENABLE_COREDUMP_SCRIPT

	__RUN_PRE_BUILD_OEM_SCRIPT

	if [ "$RK_BUILD_APP_TO_OEM_PARTITION" = "y" ]; then
		rm -rf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/*
		mkdir -p $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem
		build_mkimg $GLOBAL_OEM_NAME $RK_PROJECT_PACKAGE_OEM_DIR
	else
		mkdir -p $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem
		__COPY_FILES $RK_PROJECT_PACKAGE_OEM_DIR $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem
		rm -rf $RK_PROJECT_PACKAGE_OEM_DIR
	fi

	__RUN_POST_BUILD_SCRIPT
	post_overlay

	if [ -n "$GLOBAL_INITRAMFS_BOOT_NAME" ]; then
		build_mkimg boot $RK_PROJECT_PACKAGE_ROOTFS_DIR
	fi

	if [ "$RK_ENABLE_FASTBOOT" = "y" ]; then
		if [ "$RK_ENABLE_RAMDISK_PARTITION" = "y" ]; then
			build_mkimg $GLOBAL_ROOT_FILESYSTEM_NAME $RK_PROJECT_PACKAGE_ROOTFS_DIR
		else
			build_mkimg boot $RK_PROJECT_PACKAGE_ROOTFS_DIR
		fi
	else
		build_mkimg $GLOBAL_ROOT_FILESYSTEM_NAME $RK_PROJECT_PACKAGE_ROOTFS_DIR
	fi

	# package a empty userdata parition image
	mkdir -p $RK_PROJECT_PACKAGE_USERDATA_DIR
	if [ "$RK_ENABLE_FASTBOOT" != "y" ]; then
		__PACKAGE_USERDATA
		__RUN_POST_BUILD_USERDATA_SCRIPT
	fi
	build_mkimg userdata $RK_PROJECT_PACKAGE_USERDATA_DIR

	build_tftp_sd_update

	[ "$RK_ENABLE_RECOVERY" = "y" -o "$RK_ENABLE_OTA" = "y" ] && build_ota
	build_updateimg

	# Spi_nand mklink
	if [ "${RK_BOOT_MEDIUM}" == "spi_nand" ]; then
		msg_info "MEDIUM SPI_NAND relink Image"

		if [ "${RK_ENABLE_FASTBOOT}" = "y" ]; then
			files="${RK_PROJECT_OUTPUT_IMAGE}/userdata.img"
		else
			files=("${RK_PROJECT_OUTPUT_IMAGE}/oem.img"
				"${RK_PROJECT_OUTPUT_IMAGE}/rootfs.img"
				"${RK_PROJECT_OUTPUT_IMAGE}/userdata.img")
		fi

		for file in "${files[@]}"; do
			if [ -e "$file" ]; then
				filename=$(basename "$file")
				target=$(readlink -f "$file")
				rm "$file"
				mv "$target" "$RK_PROJECT_OUTPUT_IMAGE/$filename"
			fi
		done
		find "${RK_PROJECT_OUTPUT_IMAGE}" -type f -name "*.ubi" -exec rm {} +
	fi
	finish_build
}

# SAVE ALL UBOOT CONFIG
function _SAVE_UBOOT_CONFIG() {
	cp ${UBOOT_PATH}/configs/*luckfox* ${SDK_SYSDRV_DIR}/tools/board/uboot/
	cp ${UBOOT_PATH}/arch/arm/dts/*luckfox* ${SDK_SYSDRV_DIR}/tools/board/uboot/
}

# SAVE ALL KERNEL CONFIG
function _SAVE_KERNEL_CONFIG() {
	cp ${KERNEL_PATH}/arch/arm/configs/*luckfox* ${SDK_SYSDRV_DIR}/tools/board/kernel/
	cp ${KERNEL_PATH}/arch/arm/boot/dts/*luckfox* ${SDK_SYSDRV_DIR}/tools/board/kernel/
}

# SAVE ALL BUILDROOT CONFIG
function _SAVE_BUILDROOT_CONFIG() {
	if [ -d ${BUILDROOT_PATH} ]; then
		cp ${BUILDROOT_PATH}/configs/*luckfox* ${SDK_SYSDRV_DIR}/tools/board/buildroot
	fi
}

function build_save() {
	case $1 in
	uboot)
		_SAVE_UBOOT_CONFIG
		;;
	kernel)
		_SAVE_KERNEL_CONFIG
		;;
	buildroot)
		_SAVE_BUILDROOT_CONFIG
		;;
	*)
		IMAGE_PATH=$RK_PROJECT_OUTPUT_IMAGE
		DATE=$(date +%Y%m%d.%H%M)
		local BOARD_APP_NAME BOARD_HARDWARE POWER_SOLUTION
		BOARD_APP_NAME=$(realpath "$BOARD_CONFIG")
		BOARD_HARDWARE=${BOARD_APP_NAME%-*}
		POWER_SOLUTION=${BOARD_HARDWARE%-*}
		POWER_SOLUTION=${POWER_SOLUTION##*-}
		BOARD_HARDWARE=${BOARD_HARDWARE##*-}
		BOARD_APP_NAME=${BOARD_APP_NAME##*-}
		BOARD_APP_NAME=${BOARD_APP_NAME%.mk}
		STUB_PATH="Image/${BOARD_APP_NAME}_${RK_BOOT_MEDIUM}_${POWER_SOLUTION}_${BOARD_HARDWARE}_${DATE}_RELEASE_TEST"
		STUB_PATH="$(echo $STUB_PATH | tr '[:lower:]' '[:upper:]')"
		export STUB_PATH=$SDK_ROOT_DIR/$STUB_PATH
		export STUB_PATCH_PATH=$STUB_PATH/PATCHES
		export STUB_PARENT_PATH="$SDK_ROOT_DIR"/IMAGE
		STUB_DEBUG_FILES_PATH="$STUB_PATH/DEBUG_FILES"
		mkdir -p $STUB_PATH $STUB_PATCH_PATH

		mkdir -p $STUB_PATH/IMAGES/
		test -d $IMAGE_PATH &&
			(cp $IMAGE_PATH/* $STUB_PATH/IMAGES/ || msg_warn "Not found images... ignore")

		mkdir -p $STUB_DEBUG_FILES_PATH/kernel
		test -f $RK_PROJECT_PATH_BOARD_BIN/vmlinux && cd $RK_PROJECT_PATH_BOARD_BIN/ &&
			tar -cjf $STUB_DEBUG_FILES_PATH/kernel/vmlinux.tar.bz2 vmlinux

		test -d $RK_PROJECT_PATH_MEDIA && (cd $RK_PROJECT_PATH_MEDIA &&
			tar -cf $STUB_DEBUG_FILES_PATH/media_out.lib.tar lib || msg_warn "Not found media_out... ignore")

		test -d $RK_PROJECT_OUTPUT/app_out && cd $RK_PROJECT_OUTPUT &&
			tar -cf $STUB_DEBUG_FILES_PATH/app_out.tar app_out

		mkdir -p $STUB_DEBUG_FILES_PATH/uboot
		test -f $RK_PROJECT_PATH_BOARD_BIN/uboot.debug.tar.bz2 && cd $RK_PROJECT_PATH_BOARD_BIN/ &&
			cp -fv uboot.debug.tar.bz2 $STUB_DEBUG_FILES_PATH/uboot

		#Save build command info
		echo "BUILD-ID: $(hostname):$(whoami)" >>$STUB_PATH/build_info.txt
		build_info >>$STUB_PATH/build_info.txt
		echo "save to $STUB_PATH"

		if [[ "$LF_TARGET_ROOTFS" == "ubuntu" ]]; then
			sudo chmod a+rw $STUB_PARENT_PATH
		fi
		;;
	esac

	finish_build
}

function build_allsave() {
	# rm -rf ${RK_PROJECT_OUTPUT_IMAGE} ${RK_PROJECT_OUTPUT}
	build_all
	build_save

	build_check_power_domain

	finish_build
}

function buildroot_config() {
	if [ "${LF_TARGET_ROOTFS}" == "buildroot" ]; then
		if [ -d "$BUILDROOT_PATH" ]; then
			msg_info "Buildroot has been created"
		else
			make buildroot_create -C ${SDK_SYSDRV_DIR}
		fi

		__LINK_DEFCONFIG_FROM_BOARD_CFG

		if [ -f $BUILDROOT_CONFIG_FILE ]; then
			BUILDROOT_CONFIG_FILE_MD5=$(md5sum "$BUILDROOT_CONFIG_FILE")
			make buildroot_menuconfig -C ${SDK_SYSDRV_DIR}
			BUILDROOT_CONFIG_FILE_NEW_MD5=$(md5sum "$BUILDROOT_CONFIG_FILE")
			if [ "$BUILDROOT_CONFIG_FILE_MD5" != "$BUILDROOT_CONFIG_FILE_NEW_MD5" ]; then
				msg_info "Buildroot Save Defconfig"
				make buildroot_savedefconfig -C ${SDK_SYSDRV_DIR}
			fi
		else
			make buildroot_menuconfig -C ${SDK_SYSDRV_DIR}
			msg_info "Buildroot create .config and Save Defconfig"
			make buildroot_savedefconfig -C ${SDK_SYSDRV_DIR}
		fi
	else
		msg_error "Target rootfs is not buildroot"
	fi
	finish_build
}

function kernel_config() {
	KERNEL_CONFIG_FILE=${KERNEL_PATH}/defconfig

	if [ -f $KERNEL_CONFIG_FILE ]; then
		KERNEL_CONFIG_FILE_MD5=$(md5sum "$KERNEL_CONFIG_FILE")
		make kernel_menuconfig -C ${SDK_SYSDRV_DIR}
		KERNEL_CONFIG_FILE_NEW_MD5=$(md5sum "$KERNEL_CONFIG_FILE")
		if [ "$KERNEL_CONFIG_FILE_MD5" != "$KERNEL_CONFIG_FILE_NEW_MD5" ]; then
			msg_info "Kernel Save Defconfig"
			make kernel_savedefconfig -C ${SDK_SYSDRV_DIR}
		fi
	else
		make kernel_menuconfig -C ${SDK_SYSDRV_DIR}
		msg_info "Kernel create .config and Save Defconfig"
		make kernel_savedefconfig -C ${SDK_SYSDRV_DIR}
	fi
	finish_build
}

#=========================
# build targets
#=========================
trap 'err_handler' ERR
cd $PROJECT_TOP_DIR
unset_board_config_all
if [ "$1" = "lunch" ]; then
	build_select_board LUNCH-FORCE
fi

if [ "$1" = "mcu" ]; then
	build_mcu $2
fi

if [ ! -e "$BOARD_CONFIG" ]; then
	if [ "$1" = "clean" ]; then
		msg_info "The $BOARD_CONFIG is missing, and the SDK has not been built yet."
		exit 1
	else
		build_select_board
	fi
fi
[ -L "$BOARD_CONFIG" ] && source $BOARD_CONFIG
__LINK_DEFCONFIG_FROM_BOARD_CFG
export RK_PROJECT_BOARD_DIR=$(dirname $(realpath $BOARD_CONFIG))
export RK_PROJECT_TOOLCHAIN_CROSS=$RK_TOOLCHAIN_CROSS
export PATH="${SDK_ROOT_DIR}/tools/linux/toolchain/${RK_PROJECT_TOOLCHAIN_CROSS}/bin":$PATH

if [[ "$LF_TARGET_ROOTFS" = "ubuntu" ]]; then
	if [ "$(id -u)" != "0" ]; then
		msg_error "Error! Please use sudo ./build.sh to build Ubuntu Image!"
		exit 1
	fi
	if [[ "$LF_SUBMODULES_BY" = "github" ]]; then
		cp ${SDK_ROOT_DIR}/.gitmodules.github ${SDK_ROOT_DIR}/.gitmodules
	else
		if [[ "$LF_SUBMODULES_BY" = "gitee" ]]; then
			cp ${SDK_ROOT_DIR}/.gitmodules.gitee ${SDK_ROOT_DIR}/.gitmodules
		else
			exit 0
		fi
	fi

	if [ -d "$UBUNTU_DIR" ] && [ -f ${UBUNTU_DIR}/luckfox-ubuntu-22.04.3.tar.gz.md5 ]; then
		msg_info "${UBUNTU_DIR} is not empty, skipping submodule update!"
	else
		msg_info "${UBUNTU_DIR} is empty or does not exist, updateing submodule!"
		git submodule update --init --recursive
	fi
fi

if echo $@ | grep -wqE "help|-h"; then
	if [ -n "$2" -a "$(type -t usage$2)" == function ]; then
		echo "###Current Configure [ $2 ] Build Command###"
		eval usage$2
	else
		usage
	fi
	exit 0
fi

if ! ${RK_PROJECT_TOOLCHAIN_CROSS}-gcc --version &>/dev/null; then
	msg_error "Not found toolchain ${RK_PROJECT_TOOLCHAIN_CROSS}-gcc for [$RK_CHIP] !!!"
	msg_info "Please run these commands to install ${RK_PROJECT_TOOLCHAIN_CROSS}-gcc"
	echo ""
	echo "    cd ${SDK_ROOT_DIR}/tools/linux/toolchain/${RK_PROJECT_TOOLCHAIN_CROSS}/"
	echo "    source env_install_toolchain.sh"
	echo ""
	exit 1
fi

case $RK_PROJECT_TOOLCHAIN_CROSS in
*-uclibc*)
	export RK_LIBC_TPYE=uclibc
	;;
*)
	export RK_LIBC_TPYE=glibc
	;;
esac
export RK_PROJECT_PACKAGE_ROOTFS_DIR=$RK_PROJECT_OUTPUT/rootfs_${RK_LIBC_TPYE}_${RK_CHIP}
export RK_PROJECT_PACKAGE_OEM_DIR=$RK_PROJECT_OUTPUT/oem
export RK_PROJECT_PACKAGE_USERDATA_DIR=$RK_PROJECT_OUTPUT/userdata
export RK_PROJECT_PATH_BOARD_BIN=$RK_PROJECT_PATH_SYSDRV/board_${RK_LIBC_TPYE}_${RK_CHIP}

build_check
__PREPARE_BOARD_CFG

num=$#
option=""
while [ $# -ne 0 ]; do
	case $1 in
	DEBUG) export RK_BUILD_VERSION_TYPE=DEBUG ;;
	all) option=build_all ;;
	save)
		option="build_save $2"
		break
		;;
	allsave) option=build_allsave ;;
	check) option=build_check ;;
	clean)
		option="build_clean $2"
		break
		;;
	firmware) option=build_firmware ;;
	ota) option=build_ota ;;
	updateimg) option=build_updateimg ;;
	unpackimg) option=build_unpack_updateimg ;;
	factory) option=build_factory ;;
	recovery) option=build_recovery ;;
	env) option=build_env ;;
	meta) option=build_meta ;;
	driver) option=build_driver ;;
	sysdrv) option=build_sysdrv ;;
	uboot) option=build_uboot ;;
	kernel) option=build_kernel ;;
	rootfs) option=build_rootfs ;;
	media) option=build_media ;;
	app) option=build_app ;;
	info) option=build_info ;;
	tool) option=build_tool ;;
	buildrootconfig) option=buildroot_config ;;
	kernelconfig) option=kernel_config ;;
	*) option=usage ;;
	esac
	if [ $((num)) -gt 0 ]; then
		shift
	fi
done

eval "${option:-build_allsave}"
