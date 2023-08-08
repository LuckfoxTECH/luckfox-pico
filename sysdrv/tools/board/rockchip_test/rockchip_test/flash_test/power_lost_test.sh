#!/bin/sh

cmd=`realpath $0`
CURRENT_DIR=`dirname $cmd`

cat /data/cfg/rockchip_test/reboot_cnt 2>/dev/null

test_dir=/data/cfg/rockchip_test/power_lost
count_file=/data/cfg/rockchip_test/reboot_cnt
test_log=/data/cfg/rockchip_test/power_lost_test.log

total=5000
delay=3
no=0

if [ ! -e "/data/cfg/rockchip_test" ]; then
	echo "No /data/cfg/rockchip_test, mkdir it."
	mkdir -p /data/cfg/rockchip_test
fi

if [ ! -e "/data/cfg/rockchip_test/power_lost_test.sh" ]; then
	cp -fa ${CURRENT_DIR}/power_lost_test.sh /data/cfg/rockchip_test/
fi

# get userdata free size
data_free_size=$(echo $(df | grep -i userdata) | awk '{print $4}')

# set test file block size
if [ $data_free_size -gt "4160" ]; then
	block_size="1024K"
else
	block_size="$((($data_free_size-64)/4))K"
fi

# read/set count number
if [ -e $count_file ]; then
	count_num=`cat $count_file`
	echo -e "$(date)_power_up_$count_num" >> $test_log
else
	echo "Reset power lost count."
	count_num=0
	echo 0 > $count_file
	rm -rf $test_log
	echo "$(date) power lost test begin" >> $test_log
	mkdir -p $test_dir
fi

# auto quit
if [ $count_num -ge $total ]; then
	echo "Power loat test finished!"
	echo "off" > $count_file
	echo "Do clean ..."
	rm -rf /data/cfg/rockchip_test/power_lost_test.sh
	rm -rf /data/cfg/rockchip_test/power_lost
	rm -rf $count_file
	echo "Clean finished."
	exit 0
fi

echo "Current count_num = $count_num, Total count_num = $total"
echo "You can stop reboot by: echo off > /data/cfg/rockchip_test/reboot_cnt"
echo "Power lost test, loop $count_num:" >> $test_log
sleep $delay

# manual quit
if [ $count_num != "off" ]; then
	echo "Loop $count_num begin dd."
else
	echo "Power lost test is off."
	echo "Do clean ..."
	rm -rf /data/cfg/rockchip_test/power_lost_test.sh
	rm -rf /data/cfg/rockchip_test/power_lost
	rm -rf $count_file
	echo "Clean finished."
	exit 0
fi

count_num=$(($count_num+1))
echo $count_num > $count_file

rm -rf $test_dir/*
sync

dd if=/dev/random of=$test_dir/test_src bs=$block_size count=2
while true
do
	echo "Power lost test, loop $(($count_num-1)):"
	dd if=$test_dir/test_src of=$test_dir/test_dst
	sync

	test_src_md5=$(md5sum $test_dir/test_src | awk '{print $1}')
	test_dst_md5=$(md5sum $test_dir/test_dst | awk '{print $1}')
	if [ "$test_src_md5" = "$test_dst_md5" ];then
		echo "Compare equally."
		rm -rf $test_dir/test_dst
		sync
	else
		echo "Compare not equally."
		echo "Compare not equally." >> $test_log
		exit
	fi

	sleep 0.1
done
