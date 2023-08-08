#!/bin/bash
# align_to_flash_block_size.sh
# param1: path-to-file
# param2: flash block size

input=$1
out=$2
block_size=$3

function gen_file() {
	filename=$1
	output_file=$2

	# get file align size
	filesize=`stat -c "%s" $filename`
	filesize=`echo "scale=0; (($filesize + 1023) / 1024 + $block_size - 1) / $block_size * $block_size" | bc`

	# gen file
	`dd if=/dev/zero of=$output_file bs=1K count=$filesize > /dev/null 2>&1`
	`dd if=$filename of=$output_file bs=1K count=$filesize conv=notrunc > /dev/null 2>&1`
	echo $filename": aligned_to_flash_block_size "$block_size"KB: success!"
}

if [[ $block_size != 128 && $block_size != 256 ]]; then
  echo "$block_size not support!"
  echo "support:"
  echo "  128(KB)"
  echo "  256(KB)"
  exit
fi

if [ ! -f "$input" ]; then
  echo "$input not exist!"
  exit
fi

if [ -f "$out" ]; then
  rm $out
  exit
fi

gen_file $input $out
