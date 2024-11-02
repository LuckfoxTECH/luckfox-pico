#!/bin/sh

rcS() {
	for i in /oem/usr/etc/init.d/S??*; do

		# Ignore dangling symlinks (if any).
		[ ! -f "$i" ] && continue

		case "$i" in
		*.sh)
			# Source shell script for speed.
			(
				trap - INT QUIT TSTP
				set start
				. $i
			)
			;;
		*)
			# No sh extension, so fork subprocess.
			$i start
			;;
		esac
	done
}

check_linker() {
	[ ! -L "$2" ] && ln -sf $1 $2
}

network_init() {
	ethaddr1=$(ifconfig -a | grep "eth.*HWaddr" | awk '{print $5}')

	if [ -f /data/ethaddr.txt ]; then
		ethaddr2=$(cat /data/ethaddr.txt)
		if [ $ethaddr1 == $ethaddr2 ]; then
			echo "eth HWaddr cfg ok"
		else
			ifconfig eth0 down
			ifconfig eth0 hw ether $ethaddr2
		fi
	else
		echo $ethaddr1 >/data/ethaddr.txt
	fi
	ifconfig eth0 up && udhcpc -i eth0 >/dev/null 2>&1
}

post_chk() {
	#TODO: ensure /userdata mount done
	cnt=0
	while [ $cnt -lt 30 ]; do
		cnt=$((cnt + 1))
		if mount | grep -w userdata; then
			break
		fi
		sleep .1
	done

	# if ko exist, install ko first
	default_ko_dir=/ko
	if [ -f "/oem/usr/ko/insmod_ko.sh" ]; then
		default_ko_dir=/oem/usr/ko
	fi
	if [ -f "$default_ko_dir/insmod_ko.sh" ]; then
		cd $default_ko_dir && sh insmod_ko.sh && cd -
	fi

	network_init &
	check_linker /userdata /oem/usr/www/userdata
	check_linker /media/usb0 /oem/usr/www/usb0
	check_linker /mnt/sdcard /oem/usr/www/sdcard
	# if /data/rkipc not exist, cp /usr/share
	rkipc_ini=/userdata/rkipc.ini
	default_rkipc_ini=/tmp/rkipc-factory-config.ini

	if [ ! -f "/oem/usr/share/rkipc.ini" ]; then
		lsmod | grep sc530ai
		if [ $? -eq 0 ]; then
			ln -s -f /oem/usr/share/rkipc-500w.ini $default_rkipc_ini
		fi
		lsmod | grep sc4336
		if [ $? -eq 0 ]; then
			ln -s -f /oem/usr/share/rkipc-400w.ini $default_rkipc_ini
		fi
		lsmod | grep sc3336
		if [ $? -eq 0 ]; then
			ln -s -f /oem/usr/share/rkipc-300w.ini $default_rkipc_ini
		fi
	fi

	# check md5sum
	# tmp_md5=/tmp/.rkipc-ini.md5sum
	# data_md5=/userdata/.rkipc-default.md5sum
	# md5sum $default_rkipc_ini > $tmp_md5
	# chk_rkipc=`cat $tmp_md5|awk '{print $1}'`
	# rm $tmp_md5
	# if [ ! -f $data_md5 ];then
	# 	md5sum $default_rkipc_ini > $data_md5
	# fi
	# grep -w $chk_rkipc $data_md5
	# if [ $? -ne 0 ] ;then
	# 	rm -f $rkipc_ini
	# 	echo "$chk_rkipc" > $data_md5
	# fi

	if [ ! -f "$default_rkipc_ini" ]; then
		echo "Error: not found rkipc.ini !!!"
		exit -1
	fi
	if [ ! -f "$rkipc_ini" ]; then
		cp $default_rkipc_ini $rkipc_ini -f
	fi

	if [ ! -f "/userdata/image.bmp" ]; then
		cp -fa /oem/usr/share/image.bmp /userdata/
	fi

	if [ -d "/oem/usr/share/iqfiles" ]; then
		rkipc -a /oem/usr/share/iqfiles &
	else
		rkipc &
	fi
}

rcS

ulimit -c unlimited
echo "/data/core-%p-%e" >/proc/sys/kernel/core_pattern
# echo 0 > /sys/devices/platform/rkcif-mipi-lvds/is_use_dummybuf

echo 1 >/proc/sys/vm/overcommit_memory

post_chk &
