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
	ifconfig enP2p33s0 up && udhcpc -i  enP2p33s0 #evb7 enable eth
}

post_chk()
{
	#TODO: ensure /userdata mount done
	cnt=0
	while [ $cnt -lt 30 ];
	do
		cnt=$(( cnt + 1 ))
		if mount | grep -w userdata; then
			break
		fi
		sleep .1
	done

	network_init &
	check_linker /userdata   /oem/usr/www/userdata
	check_linker /media/usb0 /oem/usr/www/usb0
	check_linker /mnt/sdcard /oem/usr/www/sdcard

	# if /data/rkipc not exist, cp /usr/share
	rkipc_ini=/userdata/rkipc.ini
	default_rkipc_ini=/tmp/rkipc-factory-config.ini

	result=`ls -l /proc/rkisp* | wc -l`
	if [ "$result"x == "8"x ] ;then
		ln -s -f /oem/usr/share/rkipc-8x.ini $default_rkipc_ini
	fi
	if [ "$result"x == "6"x ] ;then
		ln -s -f /oem/usr/share/rkipc-6x.ini $default_rkipc_ini
	fi

	if [ ! -f "$default_rkipc_ini" ];then
		echo "Error: not found rkipc.ini !!!"
		exit -1
	fi

	if [ ! -f "$rkipc_ini" ]; then
		cp $default_rkipc_ini $rkipc_ini -f
	fi

	if [ -d "/oem/usr/share/iqfiles" ];then
		rkipc -a /oem/usr/share/iqfiles &
	else
		rkipc &
	fi
}

rcS

ulimit -c unlimited
echo "/data/core-%p-%e" > /proc/sys/kernel/core_pattern

# cpu
echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
echo userspace > /sys/devices/system/cpu/cpufreq/policy4/scaling_governor
echo 2200000 > /sys/devices/system/cpu/cpufreq/policy4/scaling_setspeed
echo userspace > /sys/devices/system/cpu/cpufreq/policy6/scaling_governor
echo 2200000 > /sys/devices/system/cpu/cpufreq/policy6/scaling_setspeed
# gpu
# echo 900000 > /sys/kernel/debug/regulator/vdd_gpu_mem_s0/voltage
# echo 900000 > /sys/kernel/debug/regulator/vdd_gpu_s0/voltage
# echo 1000000000 > /sys/kernel/debug/clk/clk_gpu_coregroup/clk_rate
echo userspace > /sys/class/devfreq/fb000000.gpu/governor
echo 1000000000 > /sys/class/devfreq/fb000000.gpu/max_freq
echo 1000000000 > /sys/class/devfreq/fb000000.gpu/min_freq
cat /sys/class/devfreq/fb000000.gpu/cur_freq

# isp
echo 702000000  > /sys/kernel/debug/clk/clk_isp0_core/clk_rate
echo 702000000  > /sys/kernel/debug/clk/clk_isp1_core/clk_rate
post_chk &
