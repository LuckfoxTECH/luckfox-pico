#!/bin/bash
set -e

cwd=$(dirname $(readlink -f $0))
cd ${cwd}
CMD_VERSION="2.0.0"

function msg_info() {
	echo -e "[$(basename $0):info] $1"
}

function msg_warn() {
	echo -e "[$(basename $0):warn] $1"
}

function msg_error() {
	echo -e "[$(basename $0):error] $1"
}

function msg_help() {
	echo "command format:"
	echo "    $(basename $cwd)"
	echo "        -v|--version         (Optional) view this version."
	echo "        -h|--help            (Optional) view this help message."
	echo "        -c|--clean           (Optional) clean."
	echo "        -e|--export          (Optional) export include files."
	echo "        --main_sensor_iq     (Optional) input main camera sensor's iq bin file."
	echo "        --second_sensor_iq   (Optional) input secondary camera sensor's iq bin file."
	echo "        --cam_iqfile         (Optional) input camera sensor IQ file."
	echo "        --meta_param         (Optional) input meta parameters."
	echo "        --output             (Optional) config output dir for meta.img"
	echo "        --rootfs_dir         (Optional) the dir of make_meta tool install"
	echo "        --media_dir          (Optional) the dir of meta header files install"
	echo "        --pc_tools_dir       (Optional) the dir of make_meta_host tool install"
	echo "        --tiny_meta          (Optional) the flag of using tiny meta"
	exit 0
}

function export_header_files() {
	if [ -n "$1" ]; then
		mkdir -p "${1}/include"
		cp -f ${cwd}/include/*.h $1/include
	else
		msg_error "use format: ./build_meta.sh --export --media_dir ./dir_for_meta_head"
	fi
	exit 0
}

function __chk_param() {
	for item in $BUILD_META_PARAMS; do
		if [ "$item" = "$1" ]; then
			echo ""
			return
		fi
	done
	echo "$1"
}

function chk_param() {
	BUILD_META_PARAMS="-v --version -h --help -c --clean --main_sensor_iq --second_sensor_iq -e --export --cam_iqfile --meta_param --output --rootfs_dir --media_dir --pc_tools_dir --boot_medium --tiny_meta"

	local cnt
	cnt=0
	while [ $# -ne 0 ]; do
		case $1 in
		-e | --export)
			msg_info "export include files"
			BUILD_META_EXPORT_HEADER=YES
			;;
		-v | --version)
			msg_info "V$CMD_VERSION"
			exit 0
			;;
		-h | --help)
			msg_help
			;;
		-c | --clean)
			make -C ${cwd}/sensor_ae_awb_table clean
			make -C ${cwd}/sensor_init clean
			make -C ${cwd}/make_meta clean
			make -C ${cwd}/zlib clean
			make -C ${cwd}/mtd_updateEngeen clean
			rm -rf ${cwd}/meta.img
			exit 0
			;;
		--second_sensor_iq)
			SECONDARY_SENSOR_IQ_BIN="$2"
			;;
		--main_sensor_iq)
			SENSOR_IQ_BIN="$2"
			;;
		--cam_iqfile)
			shift
			for item in $@; do
				ret=$(__chk_param $item)
				if [ "$ret"x = "x" ]; then
					break
				else
					rk_camera_sensor_iqfiles="$rk_camera_sensor_iqfiles $ret"
				fi
			done
			;;
		--meta_param)
			shift
			for item in $@; do
				if [ "${item%=*}" = "--meta_part_size" ]; then
					META_PART_SIZE=${item#*=}
				fi
				ret=$(__chk_param $item)
				if [ "$ret"x = "x" ]; then
					break
				else
					rk_meta_param="$rk_meta_param $ret"
				fi
			done
			local tmp
			tmp=${rk_meta_param##*--cmdline=}
			CMDLINE=${tmp##*--cmdline}
			CMDLINE=${CMDLINE%% --*}
			tmp=${rk_meta_param##*--cmdline}
			tmp=${tmp#*--}
			rk_meta_param=${rk_meta_param%%--cmdline*}
			rk_meta_param="${rk_meta_param} --$tmp"
			echo "rk_meta_param=${rk_meta_param} --cmdline $CMDLINE"
			;;
		--output)
			rk_project_output_image=$2
			;;
		--rootfs_dir)
			rk_project_package_rootfs_dir=$2
			;;
		--media_dir)
			rk_project_path_media=$2
			;;
		--pc_tools_dir)
			rk_project_path_pc_tools=$2
			;;
		--boot_medium)
			rk_boot_medium=$2
			;;
		--tiny_meta)
			rk_tiny_meta=$2
			;;
		*) ;;
		esac
		shift
	done

	if [ "$BUILD_META_EXPORT_HEADER"x = "YES"x ]; then
		export_header_files $rk_project_path_media
	fi

	if [ -f "$SENSOR_IQ_BIN" ]; then
		rk_camera_sensor_iqfiles="$SENSOR_IQ_BIN"
	fi
	if [ -f "$SECONDARY_SENSOR_IQ_BIN" ]; then
		if [ -z "$rk_camera_sensor_iqfiles" ]; then
			msg_error "not found main sensor iqbin"
			msg_info "    add --main_sensor_iq"
			exit 1
		else
			rk_camera_sensor_iqfiles="$rk_camera_sensor_iqfiles $SECONDARY_SENSOR_IQ_BIN"
		fi
	fi

	if [ -z "$rk_camera_sensor_iqfiles" -a ! "$rk_tiny_meta" = "y" ]; then
		msg_warn "invalid parameter"
		msg_help
	fi
}

chk_param $@

build_target_image=meta.img
build_target_bin="${cwd}/make_meta/make_meta"
SENSOR_AE_AWB_TABLE="${cwd}/sensor_ae_awb_table/ae_awb_table.bin"
SENSOR_INIT_BIN="${cwd}/sensor_init/init_sensor.bin"

if [ ! "$rk_tiny_meta" = "y" ]; then
	__META_CAMERA_NUMBERS=0
	for item in $rk_camera_sensor_iqfiles; do
		__META_CAMERA_NUMBERS=$((__META_CAMERA_NUMBERS + 1))
		case $__META_CAMERA_NUMBERS in
		1)
			if [ -f "$rk_project_path_media/isp_iqfiles/${item}" ]; then
				SENSOR_IQ_BIN="$rk_project_path_media/isp_iqfiles/${item}"
			fi
			;;
		2)
			if [ -f "$rk_project_path_media/isp_iqfiles/${item}" ]; then
				SECONDARY_SENSOR_IQ_BIN="$rk_project_path_media/isp_iqfiles/${item}"
			fi
			;;
		*)
			msg_error "Not Support camera numbers!!!"
			exit 1
			;;
		esac
	done

	if [ -z "$META_PART_SIZE" ]; then
		if [ $__META_CAMERA_NUMBERS -eq 2 ]; then
			rk_meta_param="$rk_meta_param --meta_part_size=0xb0000"
		fi
		if [ $__META_CAMERA_NUMBERS -eq 1 ]; then
			rk_meta_param="$rk_meta_param --meta_part_size=0x60000"
		fi
	fi

	if [ $__META_CAMERA_NUMBERS -eq 2 ]; then
		SECONDARY_SENSOR_INIT_BIN="${cwd}/sensor_init/init_secondary_sensor.bin"
	else
		SECONDARY_SENSOR_INIT_BIN=""
	fi

	msg_info "========================================"
	msg_info "rk_camera_param=$rk_camera_param"
	msg_info "rk_camera_sensor_iqfiles=${rk_camera_sensor_iqfiles}"
	msg_info "rk_meta_param=$rk_meta_param"
	msg_info "rk_project_output_image=$rk_project_output_image"
	msg_info "rk_project_package_rootfs_dir=$rk_project_package_rootfs_dir"
	msg_info "rk_project_path_media=$rk_project_path_media"
	msg_info "rk_project_path_pc_tools=$rk_project_path_pc_tools"
	msg_info "rk_boot_medium=$rk_boot_medium"
	msg_info "rk_tiny_meta=$rk_tiny_meta"
	msg_info "SENSOR_IQ_BIN=$SENSOR_IQ_BIN"
	msg_info "SECONDARY_SENSOR_IQ_BIN=$SECONDARY_SENSOR_IQ_BIN"
	msg_info "========================================"

	if [ -n "${rk_camera_param}" ]; then
		META_APP_PARAMETER=${rk_camera_param}
	elif [ -n "${rk_meta_param}" ]; then
		META_APP_PARAMETER=${rk_meta_param}
	else
		META_APP_PARAMETER=""
	fi

	support_sensors="os02k10 sc301iot sc501ai sc230ai sc200ai sc3338 sc4336 gc2093 gc3003 sc231hai sc2336p"

	for item in $support_sensors; do
		if [ -n "${SENSOR_IQ_BIN}" ]; then
			if basename $SENSOR_IQ_BIN | grep -i "$item"; then
				export SENSOR_TARGET=$item
			fi
		fi

		if [ -n "${SECONDARY_SENSOR_IQ_BIN}" ]; then
			if basename $SECONDARY_SENSOR_IQ_BIN | grep "$item"; then
				export SECONDARY_SENSOR_TARGET=$item
				META_APP_PARAMETER="$META_APP_PARAMETER --secondary_sensor_init=${SECONDARY_SENSOR_INIT_BIN}"
			fi
		fi
	done

	if [ -z "${SENSOR_TARGET}" ]; then
		msg_error "Not found main camera sensor config, please add [support_sensors] in $(basename $0)"
		exit 1
	fi

	if [ -n "${SECONDARY_SENSOR_IQ_BIN}" ]; then
		if [ -z "${SECONDARY_SENSOR_TARGET}" ]; then
			msg_error "Not found secondary camera sensor config, please add [support_sensors] in $(basename $0)"
			exit 1
		fi
	fi

	# get secondary camera sensor iq size
	if [ -f "${SECONDARY_SENSOR_IQ_BIN}" ]; then
		SECONDARY_SENSOR_IQ_BIN_SIZE=$(wc -c ${SECONDARY_SENSOR_IQ_BIN} | awk '{print $1}')
		META_APP_PARAMETER="$META_APP_PARAMETER --secondary_sensor_iq_bin=${SECONDARY_SENSOR_IQ_BIN}"
	fi
fi # if [ ! "$rk_tiny_meta" = "y" ];

make -C ${cwd}/sensor_ae_awb_table

make -C ${cwd}/sensor_init main_sensor=$SENSOR_TARGET secondary_sensor=$SECONDARY_SENSOR_TARGET

make -C ${cwd}/zlib

make -C ${cwd}/mtd_updateEngeen

#if [ "$rk_boot_medium" = "spi_nand" ]; then
#	RK_META_IS_NAND_FLASH="YES"
#fi
make -C ${cwd}/make_meta IS_NAND_FLASH=$RK_META_IS_NAND_FLASH

if [ -d "${rk_project_path_pc_tools}" ]; then
	cp -f ${cwd}/make_meta/make_meta_host ${rk_project_path_pc_tools}/
fi

if [ "$rk_tiny_meta" = "y" ]; then
	${cwd}/make_meta/make_meta_host --create \
		--meta_path=${cwd}/${build_target_image} \
		--cmdline="${CMDLINE}"
else
	META_APP_PARAMETER="--create \
	--meta_path=${cwd}/${build_target_image} \
	--sensor_init=${SENSOR_INIT_BIN} \
	--sensor_iq_bin=${SENSOR_IQ_BIN} \
	--ae_awb_tab=${SENSOR_AE_AWB_TABLE} \
	$META_APP_PARAMETER"

	echo "command: ${cwd}/make_meta/make_meta_host $META_APP_PARAMETER"
	${cwd}/make_meta/make_meta_host ${META_APP_PARAMETER} --cmdline="${CMDLINE}"

fi

if [ -f "${cwd}/${build_target_image}" ]; then
	echo "Generate Camera meta data: ${build_target_image} "

	if [ -n "${rk_project_output_image}" ]; then
		mkdir -p "${rk_project_output_image}"
		cp -f ${cwd}/${build_target_image} ${rk_project_output_image}/
	fi
	if [ -n "${rk_project_package_rootfs_dir}" ]; then
		mkdir -p "${rk_project_package_rootfs_dir}/bin"
		cp -f ${build_target_bin} ${rk_project_package_rootfs_dir}/bin/
	fi

	exit 0
else
	msg_error "build meta data failed"
	exit 1
fi
