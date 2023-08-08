#!/bin/bash

CURRENT_DIR=$(dirname $(readlink -f $0))
export PATH=$CURRENT_DIR:$PATH
# source files
ROOTFS_SRC_DIR=$1
# generate image output directory
IMAGE_OUTPUT_DIR=$2
# the size of generate image, get info from parameter.txt
# eg. 0x00040000@0x00016000(rootfs)
# calculate size fo rootfs partition: 0x00040000 * 512 = 128*0x100000
ROOTFS_PART_SIZE=$(( $3 ))
# ubifs partition name (volume name)
ROOTFS_PART_NAME=$4
# ubifs type: ubifs(default) or squashfs
FS_TYPE=${5}
UBIFS_TYPE=${FS_TYPE:=ubifs}
# filesystem compression
# when build squashfs on ubi, set default gzip
# when build erofs on ubi, set default lz4hc
# when build ubifs on ubi, set default lzo
UBI_FS_COMP=${6}

# parameter.txt is option
PARAMETER=$7

MKUBIFS_TOOL=mkfs.ubifs
MKUBINIZE_TOOL=ubinize
MKSQUASHFS_TOOL=mksquashfs
MKEROFS_TOOL=mkfs.erofs

TEMP_UBI_CFG_DIR=$IMAGE_OUTPUT_DIR/.ubi_cfg
ROOTFS_IMAGE_FAKEROOT_UBI=$TEMP_UBI_CFG_DIR/fakeroot-ubi-${ROOTFS_PART_NAME}.fs

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

err_handler() {
	ret=$?
	[ "$ret" -eq 0 ] && return

	msg_error "Running ${FUNCNAME[1]} failed!"
	msg_error "exit code $ret from line ${BASH_LINENO[0]}:"
	msg_info "    $BASH_COMMAND"
	exit $ret
}

function help_msg()
{
	echo -e "script version: V1.2.0"
	echo -e "command format:"
	echo -e "$(basename $0) <source dir> <output dir> <partition size> <partition name> <fs type> <fs comp>"
	echo -e ""
	echo -e "<fs type>: filesystem type on ubi (default ubifs)"
	echo -e "           support ubifs or squashfs(readonly) or erofs(readonly)"
	echo -e ""
	echo -e "<fs comp>: filesystem compression"
	echo -e "           <fs type> is ubifs, support lzo|zlib"
	echo -e "           <fs type> is squashfs, support lz4|lzo|lzma|xz|gzip"
	echo -e "           <fs type> is erofs, support lz4|lz4hc (default lz4hc)"
	exit 0
}

function mk_ubi_image_fake()
{
	temp_dir="$TEMP_UBI_CFG_DIR"

	if [ $(( $UBI_BLOCK_SIZE )) -eq $(( 0x20000 )) ]; then
		ubi_block_size_str="128KB"
	elif [ $(( $UBI_BLOCK_SIZE )) -eq $(( 0x40000 )) ]; then
		ubi_block_size_str="256KB"
	else
		msg_error "Please add ubi block size [$UBI_BLOCK_SIZE] to $PWD/$0"
		exit 1
	fi

	if [ $(( $UBI_PAGE_SIZE )) -eq 2048 ]; then
		ubi_page_size_str="2KB"
	elif [ $(( $UBI_PAGE_SIZE )) -eq 4096 ]; then
		ubi_page_size_str="4KB"
	else
		msg_error "Please add ubi block size [$UBI_PAGE_SIZE] to $PWD/$0"
		exit 1
	fi

	if [ -z "$UBI_VOL_NAME" ]; then
		msg_error "Please config ubifs partition volume name"
		exit 1
	fi

	ubifs_lebsize=$(( $UBI_BLOCK_SIZE - 2 * $UBI_PAGE_SIZE ))
	ubifs_miniosize=$UBI_PAGE_SIZE
	partition_size=$(( $UBI_PART_SIZE ))
	partition_size_str="$(( $partition_size / 1024 / 1024 ))MB"
	output_image=${IMAGE_OUTPUT_DIR}/${UBI_VOL_NAME}_${ubi_page_size_str}_${ubi_block_size_str}_${partition_size_str}.ubi
	temp_ubinize_file=$temp_dir/${UBI_VOL_NAME}_${ubi_page_size_str}_${ubi_block_size_str}_${partition_size_str}_ubinize.cfg
	UBI_VOL_TYPE=${UBI_VOL_TYPE:-dynamic}
	UBI_COMPRESSION_TPYE=${UBI_COMPRESSION_TPYE:-lzo}

	if [ $partition_size -le 0 ]; then
		msg_error "ubifs partition MUST set partition size"
		exit 1
	fi
	if [ ! -f $UBI_IMAGE_FAKEROOT ]; then
		msg_error "ubifs $UBI_IMAGE_FAKEROOT not found!!!"
		exit 1
	fi

	ubifs_maxlebcnt=$(( $partition_size / $ubifs_lebsize ))

	msg_info "ubifs_lebsize=$UBI_BLOCK_SIZE"
	msg_info "ubifs_miniosize=$UBI_PAGE_SIZE"
	msg_info "ubifs_maxlebcnt=$ubifs_maxlebcnt"

	case $UBIFS_TYPE in
		squashfs)
			temp_image=$temp_dir/${UBI_VOL_NAME}_${partition_size_str}.squashfs
			parallel_jobs=$((1 + `getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4`))

			case $UBI_FS_COMP in
				lz4|lzo|lzma|xz|gzip)
					squashfs_compression_args=$UBI_FS_COMP
					;;
				*)
					msg_info "squashfs compression support: lz4|lzo|lzma|xz|gzip !!!"
					msg_warn "$UBI_FS_COMP is not support, default use xz !!!"
					squashfs_compression_args=xz
					;;
			esac

			if [ "$squashfs_compression_args" = "lz4" ]; then
				echo "$MKSQUASHFS_TOOL $UBI_SRC_DIR $temp_image -noappend -processors $parallel_jobs -comp $squashfs_compression_args -Xhc" >> $UBI_IMAGE_FAKEROOT
			else
				echo "$MKSQUASHFS_TOOL $UBI_SRC_DIR $temp_image -noappend -processors $parallel_jobs -comp $squashfs_compression_args" >> $UBI_IMAGE_FAKEROOT
			fi
			;;

		erofs)
			temp_image=$temp_dir/${UBI_VOL_NAME}_${partition_size_str}.erofs

			case $UBI_FS_COMP in
				lz4|lz4hc)
					erofs_compression_args=$UBI_FS_COMP
					;;
				*)
					msg_info "erofs compression support: lz4|lz4hc !!!"
					msg_warn "$UBI_FS_COMP is not support, default use lz4hc !!!"
					erofs_compression_args=lz4hc
					;;
			esac

			echo "$MKEROFS_TOOL $temp_image $UBI_SRC_DIR -z$erofs_compression_args" >> $UBI_IMAGE_FAKEROOT
			;;

		ubifs|*)
			temp_image=$temp_dir/${UBI_VOL_NAME}_${ubi_page_size_str}_${ubi_block_size_str}_${partition_size_str}.ubifs
			case $UBI_FS_COMP in
				lzo|zlib)
					UBI_COMPRESSION_TPYE=$UBI_FS_COMP
					;;
				*)
					msg_info "ubifs compression support: lzo|zlib !!!"
					msg_warn "$UBI_FS_COMP is not support, default use lzo !!!"
					UBI_COMPRESSION_TPYE=lzo
					;;
			esac
			echo "$MKUBIFS_TOOL -x $UBI_COMPRESSION_TPYE -e $ubifs_lebsize -m $ubifs_miniosize -c $ubifs_maxlebcnt -d $UBI_SRC_DIR -F -v -o $temp_image" >> $UBI_IMAGE_FAKEROOT
			;;
	esac

	echo "[ubifs]" > $temp_ubinize_file
	echo "mode=ubi" >> $temp_ubinize_file
	echo "vol_id=0" >> $temp_ubinize_file
	echo "vol_type=$UBI_VOL_TYPE" >> $temp_ubinize_file
	echo "vol_name=$UBI_VOL_NAME" >> $temp_ubinize_file
	echo "vol_alignment=1" >> $temp_ubinize_file
	echo "vol_flags=autoresize" >> $temp_ubinize_file
	echo "image=$temp_image" >> $temp_ubinize_file
	echo "$MKUBINIZE_TOOL -o $output_image -m $ubifs_miniosize -p $UBI_BLOCK_SIZE -v $temp_ubinize_file" >> $UBI_IMAGE_FAKEROOT
	echo ""

	chmod a+x $UBI_IMAGE_FAKEROOT

	# Pick a default ubi image
	if [ $(( $DEFAULT_UBI_PAGE_SIZE )) -eq $(( $UBI_PAGE_SIZE )) \
		-a $(( $DEFAULT_UBI_BLOCK_SIZE )) -eq $(( $UBI_BLOCK_SIZE )) ]; then
		echo "ln -rfs $output_image $IMAGE_OUTPUT_DIR/${UBI_VOL_NAME}.img" >> $UBI_IMAGE_FAKEROOT
	fi

	UBI_BLOCK_SIZE=
	UBI_IMAGE_FAKEROOT=
	UBI_PAGE_SIZE=
	UBI_PART_SIZE=
	UBI_SRC_DIR=
	UBI_VOL_NAME=
	UBI_VOL_TYPE=
}

function get_partition_size()
{
	echo $PARAMETER
	if [ ! -f $PARAMETER ];then
		msg_warn "Not found partition file, skip ..."
		return 0
	fi

	PARTITIONS_PREFIX=`echo -n "CMDLINE: mtdparts=rk29xxnand:"`
	while read line
	do
		if [[ $line =~ $PARTITIONS_PREFIX ]]
		then
			partitions=`echo $line | sed "s/$PARTITIONS_PREFIX//g"`
			echo $partitions
			break
		fi
	done < $PARAMETER

	[ -z $"partitions" ] && return

	IFS=,
	for part in $partitions;
	do
		part_size=`echo $part | cut -d '@' -f1`
		part_name=`echo $part | cut -d '(' -f2|cut -d ')' -f1`

		[[ $part_size =~ "-" ]] && continue

		part_size=$(($part_size))
		part_size_bytes=$[$part_size*512]

		case $part_name in
			rootfs | system_[ab])
				ROOTFS_PART_SIZE=$part_size_bytes
				;;
			oem)
				OEM_PART_SIZE=$part_size_bytes
				;;
		esac
	done
}

function mk_ubi_image_fake_for_rootfs()
{
	UBI_BLOCK_SIZE=$1
	UBI_PAGE_SIZE=$2

	UBI_VOL_NAME="${ROOTFS_PART_NAME}"
	UBI_PART_SIZE=$ROOTFS_PART_SIZE
	UBI_IMAGE_FAKEROOT=$ROOTFS_IMAGE_FAKEROOT_UBI
	UBI_SRC_DIR=$ROOTFS_SRC_DIR

	case $UBIFS_TYPE in
		squashfs|erofs)
			UBI_VOL_TYPE=static
			;;
		ubifs|*)
			UBI_VOL_TYPE=dynamic
			;;
	esac

	mk_ubi_image_fake
}

function stash_unused_files()
{
	echo 
}

function pop_unused_files()
{
	echo 
}

# Start
trap 'err_handler' ERR

if [ -z "$ROOTFS_PART_SIZE" -o -z "$IMAGE_OUTPUT_DIR" -o -z "$ROOTFS_PART_NAME" -o ! -d "$ROOTFS_SRC_DIR" ]; then
	help_msg
fi

if [ ! -d $IMAGE_OUTPUT_DIR ]; then
	mkdir -p $IMAGE_OUTPUT_DIR
fi

mkdir -p $TEMP_UBI_CFG_DIR

# default page size 2KB
DEFAULT_UBI_PAGE_SIZE=${UBI_PAGE_SIZE:-2048}
# default block size 128KB
DEFAULT_UBI_BLOCK_SIZE=${UBI_BLOCK_SIZE:-0x20000}

stash_unused_files

msg_info "Start build ubi images..."

# get_partition_size

echo "#!/bin/sh" > $ROOTFS_IMAGE_FAKEROOT_UBI
echo "set -e" >> $ROOTFS_IMAGE_FAKEROOT_UBI

if which fakeroot; then
	FAKEROOT_TOOL="`which fakeroot`"
	echo "chown -h -R 0:0 $ROOTFS_SRC_DIR" >> $ROOTFS_IMAGE_FAKEROOT_UBI
else
	msg_warn "Install fakeroot First."
	msg_warn "   sudo apt-get install fakeroot"
	FAKEROOT_TOOL="NO_FOUND"
fi

mk_ubi_image_fake_for_rootfs 0x20000 2048
mk_ubi_image_fake_for_rootfs 0x40000 2048
mk_ubi_image_fake_for_rootfs 0x40000 4096
if [ "$FAKEROOT_TOOL" = "NO_FOUND" ]; then
	msg_warn "No found fakeroot tool..."
	$ROOTFS_IMAGE_FAKEROOT_UBI
else
	echo "start build fakeroot image"
	$FAKEROOT_TOOL -- $ROOTFS_IMAGE_FAKEROOT_UBI
fi

if [ "$RK_DEBUG" != "1" ]; then
rm -rf $TEMP_UBI_CFG_DIR
fi
msg_info "End build ubi images..."
pop_unused_files

exit 0
