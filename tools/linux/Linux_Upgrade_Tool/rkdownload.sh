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

function msg_dbg() {
	if [ "$dbg" = "yes" ]; then
		echo -e "${C_YELLOW}[$(basename $0):dbg] $1${C_NORMAL}"
	fi
}

function msg_error() {
	echo -e "${C_RED}[$(basename $0):error] $1${C_NORMAL}"
}

function usage() {
	prog_name=$(basename $_CMD)
	echo "command format:"
	echo "     $prog_name -d [dirctory path]      #--> download firmware from a dirctory"
	echo "     $prog_name -f [images path]        #--> download firmware from some images"
	echo "     $prog_name -e download.bin         #--> erase all flash"
	echo ""
	echo "like this:"
	echo "$prog_name -d output/image"
	echo "$prog_name -f output/image/download.bin output/image/env.img output/image/boot.img output/image/rootfs.img"
	echo "$prog_name -e output/image/download.bin"
}

function parse_partition_env() {
	local part_size part_offset part_name part_final partitions tmp_part_offset tmp_part_offset_b16
	local part_size_bytes part_offset_bytes size_final_char offset_final_char part_size_bytes_b16

	msg_dbg "env_part $RK_PARTITION_CMD_IN_ENV"
	if [[ -z $RK_PARTITION_CMD_IN_ENV ]]; then
		msg_error "No found partition, please check RK_PARTITION_CMD_IN_ENV "
		exit 1
	fi

	tmp_part_offset=0
	IFS=,
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
					msg_error "Partition($part_name) offset error, exit !!!"
					exit 1
				fi
				part_offset_bytes=$part_offset
				;;
			*)
				part_offset_bytes=$(($part_offset))
				if [[ $part_offset_bytes == 0 ]]; then
					msg_error "Partition($part_name) offset error, exit !!!"
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
				msg_error "Partition($part_name) size error, exit !!!"
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
			msg_error "Partition($part_name) size error, exit !!!"
			exit 1
		fi
		if [[ -n "${part_offset_bytes}" ]]; then
			if [[ $((part_offset_bytes)) -ge $((tmp_part_offset)) ]]; then
				tmp_part_offset=$part_offset_bytes
			else
				msg_error "Partition($part_name) offset set too small, exit !!!"
				exit 1
			fi
		fi

		# Convert base 10 to base 16
		if [[ $part_size_bytes =~ "-" ]]; then
			part_size_bytes_b16="-"
		else
			part_size_bytes_b16="0x$(echo "obase=16;$part_size_bytes/512" | bc)"
		fi
		if [[ $tmp_part_offset =~ "-" ]]; then
			tmp_part_offset_b16="-"
		else
			tmp_part_offset_b16="0x$(echo "obase=16;$tmp_part_offset/512" | bc)"
		fi
		if [ "$tmp_part_offset_b16" = "0x" ]; then
			tmp_part_offset_b16="0"
		fi

		partitions="$partitions,$part_size_bytes_b16@$tmp_part_offset_b16($part_name)"
		GLOBAL_PARTNAMES="$GLOBAL_PARTNAMES ${part_name}"
		[[ $part_size_bytes =~ "-" || $tmp_part_offset =~ "-" ]] || tmp_part_offset=$((tmp_part_offset + part_size_bytes))
	done
	IFS=
	GLOBAL_PARTITIONS="${partitions/,/}"
	echo "GLOBAL_PARTITIONS: $GLOBAL_PARTITIONS"
}

function __RUN_CMD() {
	if [ "$dbg" = "yes" ]; then
		echo $1
	else
		eval $1
	fi
}

function burn() {
	local part imgs
	imgs="$1"

	IFS=,
	for part in $GLOBAL_PARTITIONS; do
		part_size=$(echo $part | cut -d '@' -f1)
		part_offset=$(echo $part | cut -d '(' -f1 | cut -d '@' -f2)
		part_name=$(echo $part | cut -d '(' -f2 | cut -d ')' -f1)

		if [[ $part_size =~ "-" ]]; then
			part_name=${part_name%%:*}
		fi

		IFS=' '
		for _img in $imgs; do
			img="$(basename $_img)"
			if [ "$img" = "${part_name}.img" ]; then
				msg_info "start to flash $_img"
				__RUN_CMD "upgrade_tool wl $part_offset  $_img"
			fi
			shift
		done
		IFS=
	done
	IFS=

	msg_info "Start to reset device..."
	__RUN_CMD "upgrade_tool rd"
}

function parse_para() {
	if ! upgrade_tool -v &>/dev/null; then
		echo "error: not found upgrade_tool !!!"
		return 1
	fi

	case $1 in
	-f)
		shift
		TODO_IMGS=""
		for img in $@; do
			if head -1 $img | grep -e "blkdevparts=mmcblk0:" \
				-e "mtdparts=sfc_nor:" \
				-e "mtdparts=spi-nand0:" \
				-e "mtdparts=rk-nand:" &>/dev/null; then
				IMG_ENV=$img
			fi
			if head -1 $img | head -1 | grep -w "^LDR f" &>/dev/null; then
				DOWNLOAD_BIN=$img
				continue
			fi
			if [ -f $img ]; then
				TODO_IMGS="$TODO_IMGS $img"
			else
				msg_error "Not found [$img] exit..."
				return 1
			fi
		done
		msg_dbg "DOWNLOAD_BIN = $DOWNLOAD_BIN"
		msg_dbg "IMG_ENV = $IMG_ENV"
		msg_dbg "TODO_IMGS = $TODO_IMGS"

		if [ ! -f "$IMG_ENV" ]; then
			msg_error "Not found env.img exit..."
			return 1
		fi
		env_part=$(strings $IMG_ENV | head -1 | grep -e "blkdevparts=mmcblk0:" \
			-e "mtdparts=sfc_nor:" \
			-e "mtdparts=spi-nand0:" \
			-e "mtdparts=rk-nand:")
		RK_PARTITION_CMD_IN_ENV=${env_part##*:}
		msg_dbg "RK_PARTITION_CMD_IN_ENV = $RK_PARTITION_CMD_IN_ENV"
		parse_partition_env

		if [ ! -f "$DOWNLOAD_BIN" ]; then
			msg_error "Not found download.bin exit..."
			return 1
		fi
		__RUN_CMD "upgrade_tool db $DOWNLOAD_BIN"

		burn $TODO_IMGS
		;;
	-d)
		if [ -d "$2" ]; then
			img_dir=$2
			IMG_ENV="$img_dir/env.img"
			DOWNLOAD_BIN="$img_dir/download.bin"
			if [ ! -f "$IMG_ENV" ]; then
				msg_error "Not found env.img exit..."
				return 1
			fi
			msg_dbg "DOWNLOAD_BIN = $DOWNLOAD_BIN"
			msg_dbg "IMG_ENV = $IMG_ENV"

			env_part=$(strings $IMG_ENV | head -1 | grep -e "blkdevparts=mmcblk0:" \
				-e "mtdparts=sfc_nor:" \
				-e "mtdparts=spi-nand0:" \
				-e "mtdparts=rk-nand:")
			RK_PARTITION_CMD_IN_ENV=${env_part##*:}
			msg_dbg "RK_PARTITION_CMD_IN_ENV = $RK_PARTITION_CMD_IN_ENV"
			parse_partition_env

			TODO_IMGS=$IMG_ENV
			IFS=' '
			for img in $GLOBAL_PARTNAMES; do
				if [ -f "$img_dir/${img}.img" ]; then
					TODO_IMGS="$img_dir/${img}.img $TODO_IMGS"
				else
					msg_warn "Not found $img_dir/${img}.img, skip..."
				fi
			done
			IFS=

			msg_dbg "TODO_IMGS = $TODO_IMGS"

			if [ ! -f "$DOWNLOAD_BIN" ]; then
				msg_error "Not found download.bin exit..."
				return 1
			fi
			__RUN_CMD "upgrade_tool db $DOWNLOAD_BIN"

			burn $TODO_IMGS
		else
			usage
		fi
		;;
	-e)
		msg_info "Erase All Flash..."
		if [ -f "$2" ]; then
			download_bin=$2
			__RUN_CMD "upgrade_tool db $download_bin"
			__RUN_CMD "upgrade_tool el 0 -1"
			msg_info "Start to reset device..."
			__RUN_CMD "upgrade_tool rd"
		else
			msg_error "Not found download.bin, erase flash fail"
		fi
		;;
	*)
		usage
		;;
	esac
}

PROG_VER="V2.1.0"
DOWNLOAD_BIN_DONE="no"
_CMD=$(realpath $0)
UPGRADE_TOOL_DIR=$(dirname $_CMD)
export PATH=$UPGRADE_TOOL_DIR:$PATH

if [ "$dbg" = "yes" ]; then
	msg_dbg "Now is debug mode, flash to fake..."
fi
parse_para $@
