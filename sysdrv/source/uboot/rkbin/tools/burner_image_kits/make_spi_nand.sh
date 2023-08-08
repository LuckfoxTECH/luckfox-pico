#!/bin/bash

DIR="$( cd "$( dirname "$0"  )" && pwd  )"
src_path=$1
dst_path=$2
soc=$3
block_size=$4
page_size=2048
oob_size=64
is_slc_nand=0

if [[ $5 == 4096 ]]; then
	page_size=4096
elif [[ -n "$5" ]]; then
	echo "param page_size: $5 not support!"
	echo "support:"
	echo "  2048(B)"
	echo "  4096(B)"
	exit 0
fi

$DIR/make.sh $src_path $dst_path $soc $block_size $page_size $oob_size $is_slc_nand
