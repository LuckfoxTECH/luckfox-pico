#!/bin/bash

DIR="$( cd "$( dirname "$0"  )" && pwd  )"
src_path=$1
dst_path=$2
soc=$3
block_size=$4
page_size=$5
oob_size=$6
is_slc_nand=$7

temp_path=./IMAGES_T
ddr=
spl=

transfer_4K_2_2K=$DIR/tools/transfer_4K_2_2K.sh
rk_bch=$DIR/tools/rk_bch
mkimage=$DIR/../mkimage
upgrade_tool=$DIR/../upgrade_tool
align_to_flash_block_size=$DIR/tools/align_to_flash_block_size.sh
boot_merger=$DIR/../boot_merger

function gen_idblock()
{
	$mkimage -n $soc -T rksd -d $1:$2 idblock1.img.temp > /dev/null
	echo $3": gen_idblock: success!"
	if [[ $page_size == 4096 ]]; then
		$transfer_4K_2_2K idblock1.img.temp $3
		rm idblock1.img.temp
	else
		mv idblock1.img.temp $3
	fi
}

function is_miniloader_or_update_or_parameter()
{
	ret=0
	ls $1 | grep "MiniLoaderAll.bin" > /dev/null
	if [ $? -eq 0 ] ;then
		$boot_merger unpack --loader $1 --output $temp_path > /dev/null
		ddr=$temp_path"/"FlashData.bin
		spl=$temp_path"/"FlashBoot.bin
		gen_idblock $ddr $spl $temp_path"/"idblock.img
		is_img_and_gen_file_from_src_2_dst $temp_path"/"idblock.img idblock.img
		cat $dst_path"/"idblock.img >> $dst_path"/"idblocks.img
		cat $dst_path"/"idblock.img >> $dst_path"/"idblocks.img
		cat $dst_path"/"idblock.img >> $dst_path"/"idblocks.img
		mv $dst_path"/"idblock.img $dst_path"/"idblock.img.bak
		ret=1
	fi

	ls $1 | grep "update" > /dev/null
	if [ $? -eq 0 ] ;then
		ret=1
	fi

	ls $1 | grep "parameter.txt" > /dev/null
	if [ $? -eq 0 ] ;then
		$upgrade_tool gpt $1 $temp_path"/"gpt.img > /dev/null
		is_img_and_gen_file_from_src_2_dst $temp_path"/"gpt.img gpt.img
		ret=1
	fi

	return $ret
}

# source file absolute direction and name
# output name
function is_img_and_gen_file_from_src_2_dst()
{
	ls $1 | grep "img" > /dev/null
	if [ $? -eq 0 ] ;then
		$align_to_flash_block_size $1 $dst_path"/"$2 $block_size
		if [ $is_slc_nand -eq 1 ] ;then
			$rk_bch $dst_path"/"$2 $dst_path"/"$2".bch" $page_size $oob_size 0
			mv  $dst_path"/"$2".bch" $dst_path"/"$2
			echo "$1: rk_bch: success!"
		fi
	fi
}

if [ -f "$src_path" ]; then
	echo "input error, $src_path is a file!"
	exit
fi

if [ ! -x "$src_path" ]; then
	echo "input error, $src_path not exit!"
	exit
fi

if [[ $is_slc_nand != 0 && $is_slc_nand != 1 ]]; then
	echo "param is_slc_nand: $is_slc_nand not support!"
	echo "support:"
	echo "  1(for SLC Nand, 8 pins io)"
	echo "  0(others)"
	exit
fi

if [ $is_slc_nand -eq 1 ] ;then
	if [[ $oob_size != 64 && $oob_size != 128 && $oob_size != 224 && $oob_size != 256 ]]; then
	echo "param oob_size: $oob_size not support!"
	echo "support:"
	echo "  64(B)"
	echo "  128(B)"
	echo "  224(B)"
	echo "  256(B)"
	exit
fi
fi

if [[ $page_size != 2048 && $page_size != 4096 ]]; then
	echo "param page_size: $page_size not support!"
	echo "support:"
	echo "  2048(B)"
	echo "  4096(B)"
	exit
fi

if [[ $block_size != 128 && $block_size != 256 ]]; then
	echo "param block_size: $block_size not support!"
	echo "support:"
	echo "  128(KB)"
	echo "  256(KB)"
	exit
fi

if [[ $soc != "rk3308" && $soc != "rv1126" ]]; then
	echo "param soc: $soc not support!"
	echo "support:"
	echo "  rk3308"
	echo "  rv1126"
	exit
fi

if [ -x "$dst_path" ]; then
	rm -rf $dst_path
fi

if [ -x "$temp_path" ]; then
	rm -rf $temp_path
fi
mkdir $temp_path

dst_path=$dst_path"/"$page_size"B_"$block_size"KB"
if [[ $is_slc_nand == 1 ]]; then
	dst_path=$dst_path"_SLC"
else
	dst_path=$dst_path"_SPI"
fi
mkdir -p $dst_path

for file in `ls -a $src_path`
do
	if [ -f $src_path"/"$file ] ;then
		a=$src_path"/"$file
		cp $a $temp_path"/"$file
		# get soft link address
		if [ -h $src_path"/"$file ]
		then
			a=$src_path"/"$file
			b=`ls -ld $a|awk '{print $NF}'`
			c=`ls -ld $a|awk '{print $(NF-2)}'`
			[[ $b =~ ^/ ]] && a=$b  || a=`dirname $c`/$b
			cp -f $a $temp_path"/"$file
		fi
		#get internal address
		filesize=`stat -c "%s" $a`
		if [[ $filesize -lt 256 ]]; then
			line=`sed -n '1p' $a`
			if [ -f $line ] ;then
				cp $line $temp_path"/"$file
			else
				echo "error: $file first line '$line' is not exit!"
			fi
		fi
		is_miniloader_or_update_or_parameter $temp_path"/"$file
		if [ $? -eq 0 ] ;then
			is_img_and_gen_file_from_src_2_dst $temp_path"/"$file $file
		fi
	fi
done

echo "rm -rf $temp_path"
rm -rf $temp_path
