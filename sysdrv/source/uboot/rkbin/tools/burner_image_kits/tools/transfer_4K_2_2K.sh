#!/bin/bash
# transfer_4K_2_2K.sh
# param1: path-to-file
# param2: flash block size

input=$1
out=$2

function gen_file() {
	filename=$1
	output_file=$2

	# get file align size
	filesize=`stat -c "%s" $filename`
	filesize=`expr $filesize + 2047`
	filesize=`expr $filesize / 2048 \* 2048`
	filesize_x2=`expr $filesize \* 2`
	page_num=`expr $filesize / 2048`

	echo $filesize $filesize_x2 $page_num

	# gen file
	`dd if=/dev/zero of=$output_file bs=1 count=$filesize_x2 > /dev/null 2>&1`
	for ((i=0;i<$page_num;i++));
	do
		`dd if=$filename of=2KB bs=2K skip=$i count=1 > /dev/null 2>&1`
		`dd if=2KB of=$output_file bs=4K seek=$i count=1 conv=notrunc > /dev/null 2>&1`
	done
	echo $filename": transfer_2KB_2_4KB success"
	rm 2KB
}

if [ ! -f "$input" ]; then
  echo "$input not exist!"
  exit
fi

if [ -f "$out" ]; then
  rm $out
  exit
fi

gen_file $input $out
