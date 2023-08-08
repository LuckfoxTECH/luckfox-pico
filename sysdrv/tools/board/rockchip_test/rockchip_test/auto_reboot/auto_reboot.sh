#!/bin/sh

cmd=`realpath $0`
CURRENT_DIR=`dirname $cmd`

delay=10
total=${1:-10000}
fudev=/dev/sda
count_file=/data/cfg/rockchip_test/reboot_cnt

if [ ! -e "/data/cfg/rockchip_test" ]; then
	echo "No /data/cfg/rockchip_test, mkdir it."
	mkdir -p /data/cfg/rockchip_test
fi

if [ ! -e "/data/cfg/rockchip_test/auto_reboot.sh" ]; then
	cp -fa ${CURRENT_DIR}/auto_reboot.sh /data/cfg/rockchip_test
	echo $total > /data/cfg/rockchip_test/reboot_total_cnt
	sync
fi

while true
do
	#if [ ! -e "$fudev" ]; then
	#	echo "Please insert a U disk to start test!"
	#	exit 0
	#fi

	if [ -e $count_file ]; then
		count_file_info=`cat $count_file`
	else
		echo "Reset reboot count."
		echo 0 > $count_file
	fi

	echo "Reboot after $delay seconds."
	count_file_info=$(($count_file_info+1))

	if [ $count_file_info -ge $total ]; then
		echo "Auto reboot finisned."
		echo "off" > $count_file
		echo "Do clean ..."
		rm -rf /data/cfg/rockchip_test/auto_reboot.sh
		rm -rf /data/cfg/rockchip_test/reboot_total_cnt
		rm -rf $count_file
		sync
		echo "Clean finished."
		exit 0
	fi

	echo $count_file_info > $count_file
	echo "Current count_num = $count_file_info, Total count_num = $total"
	echo "You can stop reboot by: echo off > /data/cfg/rockchip_test/reboot_cnt"
	sleep $delay
	count_file_info=`cat $count_file`

	if [ $count_file_info != "off" ]; then
		sync
		if [ -e /sys/fs/pstore/console-ramoops-0 ]; then
			echo "Check console-ramoops-o message."
			grep -q "Restarting system" /sys/fs/pstore/console-ramoops-0
			if [ $? -ne 0 -a $count_file_info -ge 2 ]; then
				echo "No found 'Restarting system' log in last time kernel message."
				echo "Consider kernel crash in last time reboot test."
				echo "Quit reboot test!"
				rm -rf /data/cfg/rockchip_test/auto_reboot.sh
				rm -rf /data/cfg/rockchip_test/reboot_total_cnt
				sync
				exit 1
			else
				reboot
			fi
		else
			reboot
		fi
	else
		echo "Auto reboot is off."
		rm -rf /data/cfg/rockchip_test/auto_reboot.sh
		rm -rf /data/cfg/rockchip_test/reboot_total_cnt
		rm -rf $count_file
		sync
	fi
	exit 0
done
