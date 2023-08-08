#!/bin/sh

rcS()
{
	for i in /oem/usr/etc/init.d/S??* ;do

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

check_linker()
{
        [ ! -L "$2" ] && ln -sf $1 $2
}

network_init()
{
	ethaddr1=`ifconfig -a | grep "eth.*HWaddr" | awk '{print $5}'`

	if [ -f /data/ethaddr.txt ]; then
		ethaddr2=`cat /data/ethaddr.txt`
		if [ $ethaddr1 == $ethaddr2 ]; then
			echo "eth HWaddr cfg ok"
		else
			ifconfig eth0 down
			ifconfig eth0 hw ether $ethaddr2
		fi
	else
		echo $ethaddr1 > /data/ethaddr.txt
	fi
	ifconfig eth0 up && udhcpc -i eth0
}

post_chk()
{
	# if ko exist, install ko first
	default_ko_dir=/ko
	if [ -f "/oem/usr/ko/insmod_ko.sh" ];then
		default_ko_dir=/oem/usr/ko
	fi
	if [ -f "$default_ko_dir/insmod_ko.sh" ];then
		cd $default_ko_dir && sh insmod_ko.sh && cd -
	fi

	network_init &
	modetest -M rockchip -s 70:1280x720
	sleep .5
	killall -9 modetest

	if [ -d "/oem/usr/share/iqfiles" ];then
		rkipc -a /oem/usr/share/iqfiles&
	else
		rkipc
	fi
}

rcS

ulimit -c unlimited
echo "/data/core-%p-%e" > /proc/sys/kernel/core_pattern

# cpu
# echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
# echo userspce > /sys/devices/system/cpu/cpufreq/policy4/scaling_governor
# echo 1896000 > /sys/devices/system/cpu/cpufreq/policy4/scaling_setspeed
# echo userspce > /sys/devices/system/cpu/cpufreq/policy6/scaling_governor
# echo 1896000 > /sys/devices/system/cpu/cpufreq/policy6/scaling_setspeed

post_chk &
