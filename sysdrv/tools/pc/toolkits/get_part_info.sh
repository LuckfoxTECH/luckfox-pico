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

function msg_info()
{
	echo -e "${C_GREEN}[$(basename $0):info] $1${C_NORMAL}" >&2
}

function msg_warn()
{
	echo -e "${C_YELLOW}[$(basename $0):warn] $1${C_NORMAL}" >&2
	echo "FAIL"
}

function msg_error()
{
	echo -e "${C_RED}[$(basename $0):error] $1${C_NORMAL}" >&2
	echo "FAIL"
}

function get_partition_info()
{
	local info_type target_part_name partitions part_num
	part_num=0
	info_type=$1
	target_part_name=$2
	partitions=${GLOBAL_PARTITIONS}
	if [ -z "$target_part_name" -o -z "$partitions" ]; then
		msg_error "Invalid paramter, exit !!!"
		return 1
	fi

	IFS=,
	local part_size part_offset part_name
	for part in $partitions;
	do
		part_size=`echo $part | cut -d '@' -f1`
		part_offset=`echo $part | cut -d '(' -f1|cut -d '@' -f2`
		part_name=`echo $part | cut -d '(' -f2|cut -d ')' -f1`

		part_num=$(( part_num + 1 ))

		if [ "$part_name" = "$target_part_name" ];then
			case $info_type in
				PART_NUM)
					echo "$(( part_num - PARTITION_FIRST_NO ))"
					;;
				PART_SIZE)
					if [[ $part_size =~ "-" ]];then
						echo "FAIL"
					else
						echo "$part_size"
					fi
					;;
				PART_OFFSET)
					echo "$part_offset"
					;;
				*)
					msg_error "Invalid info type, exit !!!"
					return 1
			esac
			IFS=
			return
		fi
	done
	IFS=

	echo "FAIL"
	return
}

function parse_partition_env()
{
	local part_size part_offset part_name part_final partitions tmp_part_offset tmp_part_offset_b16
	local part_size_bytes part_offset_bytes size_final_char offset_final_char part_size_bytes_b16

	if [[ -z $1 ]]; then
		msg_error "No found partition, please check the partition parameter."
		exit 1
	fi

	# format be like: "4M(uboot),32K(env),32M(boot),1G(rootfs),-(userdata)"
	IFS=,
	tmp_part_offset=
	for part in $1;
	do
		part_size=`echo $part | cut -s -d '(' -f1|cut -d '@' -f1`
		part_name=`echo $part | cut -s -d '(' -f2|cut -s -d ')' -f1`
		part_final=`echo $part | cut -s -d '(' -f2|cut -s -d ')' -f2`

		if [[ -z $part_size || -z $part_name || -n $part_final ]]; then
			msg_error "Parse partition failed, exit !!!"
			msg_error "Please check the partition format: $1"
			exit 1
		fi

		# parse offset
		if [[ $part =~ "@" ]]; then
			part_offset=`echo $part | cut -s -d '(' -f1|cut -s -d '@' -f2`
			offset_final_char=${part_offset: -1}
			case $offset_final_char in
				K|k)
					part_offset=$((${part_offset/%${offset_final_char}/}))
					part_offset_bytes=$[$part_offset*1024]
					;;
				M|m)
					part_offset=$((${part_offset/%${offset_final_char}/}))
					part_offset_bytes=$[$part_offset*1024*1024]
					;;
				G|g)
					part_offset=$((${part_offset/%${offset_final_char}/}))
					part_offset_bytes=$[$part_offset*1024*1024*1024]
					;;
				T|t)
					part_offset=$((${part_offset/%${offset_final_char}/}))
					part_offset_bytes=$[$part_offset*1024*1024*1024*1024]
					;;
				P|p)
					part_offset=$((${part_offset/%${offset_final_char}/}))
					part_offset_bytes=$[$part_offset*1024*1024*1024*1024*1024]
					;;
				E|e)
					part_offset=$((${part_offset/%${offset_final_char}/}))
					part_offset_bytes=$[$part_offset*1024*1024*1024*1024*1024*1024]
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
			K|k)
				part_size=$((${part_size/%${size_final_char}/}))
				part_size_bytes=$[$part_size*1024]
				;;
			M|m)
				part_size=$((${part_size/%${size_final_char}/}))
				part_size_bytes=$[$part_size*1024*1024]
				;;
			G|g)
				part_size=$((${part_size/%${size_final_char}/}))
				part_size_bytes=$[$part_size*1024*1024*1024]
				;;
			T|t)
				part_size=$((${part_size/%${size_final_char}/}))
				part_size_bytes=$[$part_size*1024*1024*1024*1024]
				;;
			P|p)
				part_size=$((${part_size/%${size_final_char}/}))
				part_size_bytes=$[$part_size*1024*1024*1024*1024*1024]
				;;
			E|e)
				part_size=$((${part_size/%${size_final_char}/}))
				part_size_bytes=$[$part_size*1024*1024*1024*1024*1024*1024]
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
			part_size_bytes_b16="0x`echo "obase=16;$part_size_bytes"|bc`"
		fi
		if [[ $tmp_part_offset =~ "-" ]]; then
			tmp_part_offset_b16="-"
		else
			tmp_part_offset_b16="0x`echo "obase=16;$tmp_part_offset"|bc`"
		fi
		if [ "$tmp_part_offset_b16" = "0x" ]; then
			tmp_part_offset_b16="0"
		fi

		partitions="$partitions,$part_size_bytes_b16@$tmp_part_offset_b16($part_name)"
		[[ $part_size_bytes =~ "-" || $tmp_part_offset =~ "-" ]] || tmp_part_offset=$((tmp_part_offset + part_size_bytes))
	done
	IFS=
	GLOBAL_PARTITIONS="${partitions/,/}"
}

function help_msg()
{
	msg_info "Command format:"
	msg_info "    $(basename $0) [PART_NUM|PART_SIZE|PART_OFFSET] [parameter file path] [partition name] [medium type]"
	echo "FAIL"
	return
}

################################################################################
# Start
################################################################################
bc --version &>/dev/null
if [ "$?" -ne 0 ]; then
	msg_error "No found bc tool, please install it. (run \"sudo apt-get install bc\")"
	return
fi
if [ -z "$1" -o -z "$2" -o -z "$3" ]; then
	msg_error "Invalid Parameter, type is [$1] partition is [$2] target part name is [$3]"
	help_msg
fi

PARTITION_FIRST_NO=1
if [ "$4" = "emmc" ];then
PARTITION_FIRST_NO=0
fi

parse_partition_env $2
echo "`get_partition_info $1 $3`"
