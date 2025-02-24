#!/bin/sh
check_usb_state()
{
   if [[ ! -f "/oem/usr/bin/rk_mpi_uvc"  && ! -f "/usr/bin/rk_mpi_uvc" ]]; then
   return 1
  fi
  PID=`ps |grep rk_mpi_uvc |grep -v grep | wc -l`
  if [ $PID -le 0 ];then
       echo " exit rk_mpi_uvc ,restart it for uvc recovery..."
       killall adbd
       sleep 1
       killall -9 adbd
       rm -rf /sys/kernel/config/usb_gadget/rockchip/configs/b.1/f*
       echo none > /sys/kernel/config/usb_gadget/rockchip/UDC
       rmdir /sys/kernel/config/usb_gadget/rockchip/functions/ffs.adb
       rmdir /sys/kernel/config/usb_gadget/rockchip/functions/uac*
       UDC=`ls /sys/class/udc/| awk '{print $1}'`
       echo $UDC  > /sys/bus/platform/drivers/dwc3/unbind
       echo $UDC  > /sys/bus/platform/drivers/dwc3/bind
       /oem/usr/bin/usb_config.sh uac1 #off #disable adb
       if [ -d "/oem/usr/share/iqfiles" ];then
                rk_mpi_uvc -c /tmp/rkuvc.ini -a /oem/usr/share/iqfiles &
        else
                rk_mpi_uvc &
        fi
  fi
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

	# if ko exist, install ko first
	default_ko_dir=/ko
	if [ -f "/oem/usr/ko/insmod_ko.sh" ];then
		default_ko_dir=/oem/usr/ko
	fi
	if [ -f "$default_ko_dir/insmod_ko.sh" ];then
		cd $default_ko_dir && sh insmod_ko.sh && cd -
	fi

	# if /data/rkuvc not exist, cp /usr/share
	rkuvc_ini=/tmp/rkuvc.ini
	default_rkuvc_ini=/usr/share/rkuvc.ini
	if [ ! -f "$default_rkuvc_ini" ];then
		default_rkuvc_ini=/oem/usr/share/rkuvc.ini
	fi
	if [ ! -f "$default_rkuvc_ini" ];then
		echo "Error: not found rkuvc.ini !!!"
		exit -1
	fi
	if [ ! -f "$rkuvc_ini" ]; then
		cp $default_rkuvc_ini $rkuvc_ini -f
	fi

	export rt_log_level=3
	export rk_mpi_uvc_log_level=2
	export uac_app_log_level=2

	touch /tmp/uvc_no_timeout
	/oem/usr/bin/usb_config.sh uac1
	ifconfig lo 127.0.0.1

	if [ -d "/oem/usr/share/iqfiles" ];then
		rk_mpi_uvc -c /tmp/rkuvc.ini -a /oem/usr/share/iqfiles &
	else
		rk_mpi_uvc &
	fi

        while true
        do
           sleep 2
           if [ -f "/tmp/uvc_stop_check" ]; then
              echo " stop check usb state sh " && break
           else
              check_usb_state
           fi

           if [ -f "/tmp/reboot_loader" ]; then
              echo "exec reboot loader cmd..."
              reboot loader &
           fi
        done
}
start_app()
{
  if [ -f "/etc/init.d/S50usbdevice" ];then
     mv /etc/init.d/S50usbdevice /etc/S50usbdevice -f
  fi
  echo "##### start uvc app ..."
  post_chk &
}
ulimit -c unlimited
echo "/data/core-%p-%e" > /proc/sys/kernel/core_pattern

start_app
