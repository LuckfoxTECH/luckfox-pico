#!/bin/bash

GLOBAL_PARTITIONS="$1"
IMAGE_DIRS="$2"
BOOT_MEDIUM="$3"

TFTP_FILE_NAME=$IMAGE_DIRS/"tftp_update.txt"
SD_FILE_NAME=$IMAGE_DIRS/"sd_update.txt"

function eraseddr(){
	fileSize=$1
	dstname=$2
	echo -n "mw.b \${ramdisk_addr_r} 0xff $fileSize;" >> $dstname
}

function mktftp(){
	fileName=$1
	dstname=$2
	echo -n " tftp \${ramdisk_addr_r} $fileName;" >> $dstname
}

function mkfatload(){
	fileName=$1
	dstname=$2
	echo -n " fatload mmc 1 \${ramdisk_addr_r} $fileName;" >> $dstname
}

function mkemmc(){
	offset=$1
	fileSectorCount=$2
	dstname=$3
	echo " mmc write \${ramdisk_addr_r} $offset $fileSectorCount;" >> $dstname
}

function mkspinand(){
	partitionOffset=$1
	partitionSize=$2
	dstname=$3
	echo -n " mtd erase spi-nand0 $partitionOffset $partitionSize;" >> $dstname
	echo " mtd write spi-nand0 \${ramdisk_addr_r} $partitionOffset ${fileSize};" >> $dstname
}

function mkslcnand(){
	partitionOffset=$1
	partitionSize=$2
	dstname=$3
	echo -n " nand erase $partitionOffset $partitionSize;" >> $dstname
	echo " nand write \${ramdisk_addr_r} $partitionOffset ${fileSize};" >> $dstname
}

function mkspinor(){
	offset=$1
	fileSectorCount=$2
	dstname=$3
	echo -n " mtd_blk dev 2;" >> $dstname
	echo " mtd_blk write \${ramdisk_addr_r} $offset $fileSectorCount;" >> $dstname
}

function mkcmdline(){
	fileName=$1
	partitionOffsetSector=$2
	partitionSizeSector=$3
	fileSize=$4

	partitionOffset=$(($partitionOffsetSector))
	partitionOffset=$(($partitionOffset*512))
	partitionOffset="0x`echo "obase=16;$partitionOffset"|bc`"

	partitionSize=$(($partitionSizeSector))
	partitionSize=$(($partitionSize*512))
	partitionSize="0x`echo "obase=16;$partitionSize"|bc`"

	fileSizeSector=$(($fileSize))
	remainder=$(($fileSizeSector%512))
	fileSizeSector=$(($fileSizeSector/512))
	if [ $remainder -ne 0 ]; then
		fileSizeSector=$(($fileSizeSector+1))
	fi
	fileSizeSector="0x`echo "obase=16;$fileSizeSector"|bc`"

	echo "#$fileName $partitionOffsetSector:$partitionOffset $partitionSizeSector:$partitionSize $fileSizeSector:$fileSize" >> $TFTP_FILE_NAME
	eraseddr $fileSize $TFTP_FILE_NAME
	mktftp $fileName $TFTP_FILE_NAME

	echo "#$fileName $partitionOffsetSector:$partitionOffset $partitionSizeSector:$partitionSize $fileSizeSector:$fileSize" >> $SD_FILE_NAME
	eraseddr $fileSize $SD_FILE_NAME
	mkfatload $fileName $SD_FILE_NAME

	if [ "$BOOT_MEDIUM" == "emmc" ];then
		mkemmc $partitionOffsetSector $fileSizeSector $TFTP_FILE_NAME
		mkemmc $partitionOffsetSector $fileSizeSector $SD_FILE_NAME
	elif [ "$BOOT_MEDIUM" == "spi_nand" ];then
		mkspinand $partitionOffset $partitionSize $TFTP_FILE_NAME
		mkspinand $partitionOffset $partitionSize $SD_FILE_NAME
	elif [ "$BOOT_MEDIUM" == "slc_nand" ];then
		mkslcnand $partitionOffset $partitionSize $TFTP_FILE_NAME
		mkslcnand $partitionOffset $partitionSize $SD_FILE_NAME
	elif [ "$BOOT_MEDIUM" == "spi_nor" ];then
		mkspinor $partitionOffsetSector $fileSizeSector $TFTP_FILE_NAME
		mkspinor $partitionOffsetSector $fileSizeSector $SD_FILE_NAME
	else
		echo "Not support storage medium type: $BOOT_MEDIUM"
		exit 1
	fi

	echo "" >> $TFTP_FILE_NAME
	echo "" >> $SD_FILE_NAME
}

function list_partition(){
	OLD_IFS=$IFS
	IFS=$', \t\n'
	for part in $GLOBAL_PARTITIONS;
	do
		part_size=`echo $part | cut -d '@' -f1`
		part_offset=`echo $part | cut -d '(' -f1|cut -d '@' -f2`
		part_name=`echo $part | cut -d '(' -f2|cut -d ')' -f1`

		if [[ "$part_size" == "-" ]];then
			echo "$0: Partition Name ($part_name) is growup partiton, ignore!!!"
			continue
		fi

		for fileName in `ls $IMAGE_DIRS/`
		do
			if [[ "${part_name%_[ab]}.img" == "$fileName" ]];then
				[[ "${part_name%_[ab]}" == "idblock" ]] && [[ "$BOOT_MEDIUM" == "slc_nand" ]] && continue
				fileSize=`realpath $IMAGE_DIRS/$fileName |xargs stat -c "%s"`

				#partition size (base 10)
				partitionSize=$(($part_size))
				remainder=$(($partitionSize%512))
				#partition sector size (base 10)
				partitionSizeSector=$(($partitionSize/512))
				if [ $remainder -ne 0 ]; then
					partitionSizeSector=$(($partitionSizeSector+1))
				fi

				#partition offset (base 10)
				partitionOffset=$(($part_offset))
				remainder=$(($partitionOffset%512))
				#partition sector offset (base 10)
				partitionOffsetSector=$(($partitionOffset/512))
				if [ $remainder -ne 0 ]; then
					partitionOffsetSector=$(($partitionOffsetSector+1))
				fi

				if [ $fileSize -gt $partitionSize ]; then
					echo -e "\e[35mERROR:\e[0m The \e[35m$fileName\e[0m file size is larger than the partition size! \c"
					echo -e "Please check the image size or the partition in BoardConfig."
					exit -1
				fi
				# Convert base 10 to base 16
				partitionOffsetSectorB16="0x`echo "obase=16;$partitionOffsetSector"|bc`"
				partitionSizeSectorB16="0x`echo "obase=16;$partitionSizeSector"|bc`"
				fileSizeB16="0x`echo "obase=16;$fileSize"|bc`"
				mkcmdline $fileName $partitionOffsetSectorB16 $partitionSizeSectorB16 $fileSizeB16
			fi
		done
	done
	IFS=$OLD_IFS
}

rm -rf $TFTP_FILE_NAME
rm -rf $SD_FILE_NAME

list_partition

echo "reset" >> $TFTP_FILE_NAME
# echo "reset" >> $SD_FILE_NAME
echo "% <- this is end of file symbol" >> $SD_FILE_NAME
echo "% <- this is end of file symbol" >> $TFTP_FILE_NAME
