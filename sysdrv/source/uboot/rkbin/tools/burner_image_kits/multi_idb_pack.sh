#!/bin/bash

storage=$1
file_name=$2
idb_num=$3

function gen_multi_idb()
{
	idb_align_kb=$1
	# remove suffix
	temp_name=`basename ${file_name} .img`
	platform_name=`basename ${temp_name} .bin`
	rm -f ${platform_name}_${storage}_${idb_align_kb}_x${idb_num}.img
	cp ${file_name} ./idblock_${idb_align_kb}.img
	truncate -s ${idb_align_kb} ./idblock_${storage}_${idb_align_kb}.img
	for ((i=0; i<${idb_num}; i++))
	do
		cat ./idblock_${storage}_${idb_align_kb}.img >> ${platform_name}_${storage}_${idb_align_kb}_x${idb_num}.img
	done
	echo "Successfully generate ${platform_name}_${storage}_${idb_align_kb}_x${idb_num}.img include ${idb_num} IDBs, each aligned with ${idb_align_kb}, used in ${storage}."
	rm -f ./idblock_${storage}_${idb_align_kb}.img
}

function help()
{
	echo "Usage:"
	echo "    ./multi_idb_pack.sh <storage> <IDB file> [IDB multi]"
	echo
	echo "    Mandory args:"
	echo "        storage: IDB files are stored in"
	echo "        IDB file: Original single copy of IDB file"
	echo
	echo "    Optional args:"
	echo "        IDB multi: The number of IDB copies that need to be packaged"
	echo
	echo "    Example:"
	echo "        ./multi_idb_pack.sh spi-nand rk3588_idblock.img 5"
	echo
	echo "    If there is no IDB multi, the default value in the script"
	echo "    will be used depending on storage."
}

function prepare()
{
	# enter help?
	if [[ ${storage} == *help ]] || [ "${storage}" == "-h" ] || [ "${storage}" == "--h" ]; then
		help
		exit 1
	fi
	# check args
	if [ -z "${file_name}" ]; then
		help
		exit 1
	fi
	# use default value if without IDB multi
	if [ -z "${idb_num}" ]; then
		echo "No specified IDB multi, use default value instead"
		# emmc
		if [ "${storage}" == "emmc" ] || [ "${storage}" == "EMMC" ]; then
			idb_num=5;
		# spi-nand & spi-nor
		elif [[ ${storage} == spi*nor ]] || [[ ${storage} == spi*nand ]] ||
		 	 [[ ${storage} == SPI*NOR ]] || [[ ${storage} == SPI*NAND ]]; then
			idb_num=2;
		fi
	fi
}
function main()
{
	size=$(ls -lh ${file_name} | awk '{split($5,a,"K"); print a[1]}')
	# emmc
	if [ "${storage}" == "emmc" ] || [ "${storage}" == "EMMC" ]; then
		# check size
		if [ ${size} -gt 512 ]; then
			echo "ERROR: ${file_name} size is over max 512KB limit."
			exit 1
		fi
		gen_multi_idb 512K
	# spi-nand & spi-nor
	elif [[ ${storage} == spi*nor ]] || [[ ${storage} == spi*nand ]] ||
		 [[ ${storage} == SPI*NOR ]] || [[ ${storage} == SPI*NAND ]]; then
		if [ ${size} -le 128 ]; then
			gen_multi_idb 128K
			gen_multi_idb 256K
		# check size
		elif [ ${size} -le 256 ]; then
			gen_multi_idb 256K
		else
			echo "ERROR: ${file_name} size is over max 256KB limit."
			exit 1
		fi
	else
		help
		exit 1
	fi
}

prepare
main
