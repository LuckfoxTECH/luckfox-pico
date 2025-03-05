#!/bin/sh

# UVC_MULTI_OPTIONS: Defines the options for UVC_MULTI variable.
# Available options: off (close), one (Expand a device), two (Expand two devices)
UVC_MULTI=one

CDC_ENABLE=NO
USB_FUNCTIONS_DIR=/sys/kernel/config/usb_gadget/rockchip/functions
USB_CONFIGS_DIR=/sys/kernel/config/usb_gadget/rockchip/configs/b.1

configure_uvc_resolution_yuyv()
{
    UVC_DISPLAY_W=$1
    UVC_DISPLAY_H=$2
    UVC_DISPLAY_DIR=${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/uncompressed/u/${UVC_DISPLAY_W}_${UVC_DISPLAY_H}p
    mkdir ${UVC_DISPLAY_DIR}
    echo $UVC_DISPLAY_W > ${UVC_DISPLAY_DIR}/wWidth
    echo $UVC_DISPLAY_H > ${UVC_DISPLAY_DIR}/wHeight
    echo 333333 > ${UVC_DISPLAY_DIR}/dwDefaultFrameInterval
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMinBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMaxBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*2)) > ${UVC_DISPLAY_DIR}/dwMaxVideoFrameBufferSize
    echo -e "333333\n666666\n1000000\n2000000" > ${UVC_DISPLAY_DIR}/dwFrameInterval
}

configure_uvc_resolution_yuyv_720p()
{
    UVC_DISPLAY_W=$1
    UVC_DISPLAY_H=$2
    UVC_DISPLAY_DIR=${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/uncompressed/u/${UVC_DISPLAY_W}_${UVC_DISPLAY_H}p
    mkdir ${UVC_DISPLAY_DIR}
    echo $UVC_DISPLAY_W > ${UVC_DISPLAY_DIR}/wWidth
    echo $UVC_DISPLAY_H > ${UVC_DISPLAY_DIR}/wHeight
    echo 1000000 > ${UVC_DISPLAY_DIR}/dwDefaultFrameInterval
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMinBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMaxBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*2)) > ${UVC_DISPLAY_DIR}/dwMaxVideoFrameBufferSize
    echo -e "1000000\n2000000" > ${UVC_DISPLAY_DIR}/dwFrameInterval
}

configure_uvc_resolution_yuyv_1080p()
{
    UVC_DISPLAY_W=$1
    UVC_DISPLAY_H=$2
    UVC_DISPLAY_DIR=${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/uncompressed/u/${UVC_DISPLAY_W}_${UVC_DISPLAY_H}p
    mkdir ${UVC_DISPLAY_DIR}
    echo $UVC_DISPLAY_W > ${UVC_DISPLAY_DIR}/wWidth
    echo $UVC_DISPLAY_H > ${UVC_DISPLAY_DIR}/wHeight
    echo 2500000 > ${UVC_DISPLAY_DIR}/dwDefaultFrameInterval
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMinBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMaxBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*2)) > ${UVC_DISPLAY_DIR}/dwMaxVideoFrameBufferSize
    echo -e "2500000\n5000000" > ${UVC_DISPLAY_DIR}/dwFrameInterval
}

configure_uvc_resolution_mjpeg()
{
    UVC_DISPLAY_W=$1
    UVC_DISPLAY_H=$2
    UVC_DISPLAY_DIR=${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/mjpeg/m/${UVC_DISPLAY_W}_${UVC_DISPLAY_H}p
    mkdir ${UVC_DISPLAY_DIR}
    echo $UVC_DISPLAY_W > ${UVC_DISPLAY_DIR}/wWidth
    echo $UVC_DISPLAY_H > ${UVC_DISPLAY_DIR}/wHeight
    echo 333333 > ${UVC_DISPLAY_DIR}/dwDefaultFrameInterval
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMinBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*20)) > ${UVC_DISPLAY_DIR}/dwMaxBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*2)) > ${UVC_DISPLAY_DIR}/dwMaxVideoFrameBufferSize
    echo -e "333333\n666666\n1000000\n2000000" > ${UVC_DISPLAY_DIR}/dwFrameInterval
}
configure_uvc_resolution_h264()
{
    UVC_DISPLAY_W=$1
    UVC_DISPLAY_H=$2
    UVC_DISPLAY_DIR=${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f1/${UVC_DISPLAY_W}_${UVC_DISPLAY_H}p
    mkdir ${UVC_DISPLAY_DIR}
    echo $UVC_DISPLAY_W > ${UVC_DISPLAY_DIR}/wWidth
    echo $UVC_DISPLAY_H > ${UVC_DISPLAY_DIR}/wHeight
    echo 333333 > ${UVC_DISPLAY_DIR}/dwDefaultFrameInterval
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*10)) > ${UVC_DISPLAY_DIR}/dwMinBitRate
    echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*10)) > ${UVC_DISPLAY_DIR}/dwMaxBitRate
    echo -e "333333\n400000\n500000\n666666\n1000000\n2000000" > ${UVC_DISPLAY_DIR}/dwFrameInterval
    echo -ne \\x48\\x32\\x36\\x34\\x00\\x00\\x10\\x00\\x80\\x00\\x00\\xaa\\x00\\x38\\x9b\\x71 > ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f1/guidFormat
}
configure_uvc_resolution_h265()
{
	UVC_DISPLAY_W=$1
	UVC_DISPLAY_H=$2
	UVC_DISPLAY_DIR=${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f2/${UVC_DISPLAY_W}_${UVC_DISPLAY_H}p
	mkdir ${UVC_DISPLAY_DIR}
	echo $UVC_DISPLAY_W > ${UVC_DISPLAY_DIR}/wWidth
	echo $UVC_DISPLAY_H > ${UVC_DISPLAY_DIR}/wHeight
	echo 333333 > ${UVC_DISPLAY_DIR}/dwDefaultFrameInterval
	echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*10)) > ${UVC_DISPLAY_DIR}/dwMinBitRate
	echo $((UVC_DISPLAY_W*UVC_DISPLAY_H*10)) > ${UVC_DISPLAY_DIR}/dwMaxBitRate
	echo -e "333333\n400000\n500000\n666666" > ${UVC_DISPLAY_DIR}/dwFrameInterval
	echo -ne \\x48\\x32\\x36\\x35\\x00\\x00\\x10\\x00\\x80\\x00\\x00\\xaa\\x00\\x38\\x9b\\x71 > ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f2/guidFormat
}

cdc_device_config()
{
	mkdir  ${USB_FUNCTIONS_DIR}/acm.g0
	CONFIG_STR=`cat /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409/configuration`
	STR=${CONFIG_STR}_acm
	echo $STR > ${USB_CONFIGS_DIR}/strings/0x409/configuration
	USB_CNT=`echo $STR | awk -F"_" '{print NF-1}'`
	let USB_CNT=USB_CNT+2
	if [ $UVC_MULTI = one ];then
	let USB_CNT=USB_CNT+1
	elif [ $UVC_MULTI = two ];then
	let USB_CNT=USB_CNT+2
	fi
	echo "acm on++++++ ${USB_CNT}"
	ln -s ${USB_FUNCTIONS_DIR}/acm.g0 ${USB_CONFIGS_DIR}/f${USB_CNT}
}

uvc_device_config()
{
  UVC_GS=$1
  UVC_NAME=$2
  mkdir ${USB_FUNCTIONS_DIR}/$UVC_GS
  echo $UVC_NAME > ${USB_FUNCTIONS_DIR}/$UVC_GS/device_name
  echo $UVC_NAME > ${USB_FUNCTIONS_DIR}/$UVC_GS/function_name
  if [ $UVC_MULTI = one ];then
     echo 2048 > ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming_maxpacket
  elif [ $UVC_MULTI = two ];then
     echo 1024 > ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming_maxpacket
  else
     echo 3072 > ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming_maxpacket
  fi
  echo 2 > ${USB_FUNCTIONS_DIR}/$UVC_GS/uvc_num_request
  #echo 1 > /sys/kernel/config/usb_gadget/rockchip/functions/$UVC_GS/streaming_bulk

  mkdir ${USB_FUNCTIONS_DIR}/$UVC_GS/control/header/h
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/control/header/h ${USB_FUNCTIONS_DIR}/$UVC_GS/control/class/fs/h
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/control/header/h ${USB_FUNCTIONS_DIR}/$UVC_GS/control/class/ss/h
  ##YUYV support config
  mkdir /sys/kernel/config/usb_gadget/rockchip/functions/$UVC_GS/streaming/uncompressed/u
  configure_uvc_resolution_yuyv 320 240
  configure_uvc_resolution_yuyv 640 480
  configure_uvc_resolution_yuyv_720p 1280 720
  #configure_uvc_resolution_yuyv_1080p 1920 1080

  ##mjpeg support config
  mkdir ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/mjpeg/m
  configure_uvc_resolution_mjpeg 320 240
  configure_uvc_resolution_mjpeg 640 360
  configure_uvc_resolution_mjpeg 640 480
  configure_uvc_resolution_mjpeg 768 448
  configure_uvc_resolution_mjpeg 1280 720
  configure_uvc_resolution_mjpeg 1024 768
  configure_uvc_resolution_mjpeg 1920 1080
  configure_uvc_resolution_mjpeg 2048 1536
#  configure_uvc_resolution_mjpeg 2560 1440

  ## h.264 support config
  mkdir ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f1
  configure_uvc_resolution_h264 640 480
  configure_uvc_resolution_h264 1280 720
  configure_uvc_resolution_h264 1920 1080
  configure_uvc_resolution_h264 2048 1536
#  configure_uvc_resolution_h264 2560 1440

  ## h.265 support config
  mkdir ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f2
  configure_uvc_resolution_h265 640 480
  configure_uvc_resolution_h265 1280 720
  configure_uvc_resolution_h265 1920 1080
  configure_uvc_resolution_h265 2048 1536
  #configure_uvc_resolution_h265 2560 1440

  mkdir /sys/kernel/config/usb_gadget/rockchip/functions/$UVC_GS/streaming/header/h
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/uncompressed/u ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/header/h/u
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/mjpeg/m ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/header/h/m
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f1 ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/header/h/f1
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/framebased/f2 ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/header/h/f2
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/header/h ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/class/fs/h
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/header/h ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/class/hs/h
  ln -s ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/header/h ${USB_FUNCTIONS_DIR}/$UVC_GS/streaming/class/ss/h
}
uac1_device_config()
{
  UAC=$1
  mkdir ${USB_FUNCTIONS_DIR}/${UAC}.gs0
  UAC_GS0=${USB_FUNCTIONS_DIR}/${UAC}.gs0
  echo 3 > ${UAC_GS0}/p_chmask
  echo 2 > ${UAC_GS0}/p_ssize
  echo 1 > ${UAC_GS0}/p_mute_present
  echo 1 > ${UAC_GS0}/p_volume_present
  echo -5120 > ${UAC_GS0}/p_volume_min #-20db min must > -96db
  echo 8000,16000,44100,48000 > ${UAC_GS0}/p_srate

  echo 3 > ${UAC_GS0}/c_chmask
  echo 2 > ${UAC_GS0}/c_ssize
  echo 4 > ${UAC_GS0}/req_number
  echo 1 > ${UAC_GS0}/c_mute_present
  echo 1 > ${UAC_GS0}/c_volume_present
  echo -3200 > ${UAC_GS0}/c_volume_min #-12.5db
  echo 0 > ${UAC_GS0}/c_volume_max   #0db
  echo 32 > ${UAC_GS0}/c_volume_res #0.125db
  echo 8000,16000,44100,48000 > ${UAC_GS0}/c_srate

  if [ $UVC_MULTI = one ];then
   ln -s ${UAC_GS0} ${USB_CONFIGS_DIR}/f3
  elif [ $UVC_MULTI = two ];then
   ln -s ${UAC_GS0} ${USB_CONFIGS_DIR}/f4
  else
   ln -s ${UAC_GS0} ${USB_CONFIGS_DIR}/f2
  fi
}
uac2_device_config()
{
  UAC=$1
  mkdir ${USB_FUNCTIONS_DIR}/${UAC}.gs0
  UAC_GS0=${USB_FUNCTIONS_DIR}/${UAC}.gs0
  echo 3 > ${UAC_GS0}/p_chmask
  echo 2 > ${UAC_GS0}/p_ssize
  echo 1 > ${UAC_GS0}/p_mute_present
  echo 1 > ${UAC_GS0}/p_volume_present
  echo -5120 > ${UAC_GS0}/p_volume_min #-20db min must > -96db
  echo 8000,16000,44100,48000 > ${UAC_GS0}/p_srate

  echo 3 > ${UAC_GS0}/c_chmask
  echo 2 > ${UAC_GS0}/c_ssize
  echo 4 > ${UAC_GS0}/req_number
  echo 1 > ${UAC_GS0}/c_mute_present
  echo 1 > ${UAC_GS0}/c_volume_present
  echo -3200 > ${UAC_GS0}/c_volume_min #-12.5db
  echo 0 > ${UAC_GS0}/c_volume_max   #0db
  echo 32 > ${UAC_GS0}/c_volume_res #0.125db
  echo 8000,16000,44100,48000 > ${UAC_GS0}/c_srate

  if [ $UVC_MULTI = one ];then
   ln -s ${UAC_GS0} ${USB_CONFIGS_DIR}/f3
  elif [ $UVC_MULTI = two ];then
   ln -s ${UAC_GS0} ${USB_CONFIGS_DIR}/f4
  else
   ln -s ${UAC_GS0} ${USB_CONFIGS_DIR}/f2
  fi
}
pre_run_rndis()
{
  RNDIS_STR="rndis"
  if ( echo $1 |grep -q "rndis" ); then
   #sleep 1
   IP_FILE=/data/uvc_xu_ip_save
   echo "config usb0 IP..."
   if [ -f $IP_FILE ]; then
      for line in `cat $IP_FILE`
      do
        echo "save ip is: $line"
        ifconfig usb0 $line
      done
   else
    ifconfig usb0 172.16.110.6
   fi
   ifconfig usb0 up
  fi
}

insmod /oem/usr/ko/usb-common.ko
insmod /oem/usr/ko/usbcore.ko
insmod /oem/usr/ko/udc-core.ko
insmod /oem/usr/ko/libcomposite.ko
insmod /oem/usr/ko/usb_f_fs.ko

#UAC
insmod /oem/usr/ko/u_audio.ko
insmod /oem/usr/ko/usb_f_uac2.ko
insmod /oem/usr/ko/usb_f_uac1.ko

#UVC
insmod /oem/usr/ko/usb_f_uvc.ko

if [ "$CDC_ENABLE" = "YES" ];then
	insmod /oem/usr/ko/u_serial.ko
	insmod /oem/usr/ko/usb_f_acm.ko
fi
insmod /oem/usr/ko/phy-rockchip-inno-usb2.ko
insmod /oem/usr/ko/dwc3-of-simple.ko
insmod /oem/usr/ko/dwc3.ko

##main
#init usb config
ifconfig lo up   # for adb ok
/etc/init.d/S10udev stop
umount /sys/kernel/config
mkdir /dev/usb-ffs
mount -t configfs none /sys/kernel/config
mkdir -p /sys/kernel/config/usb_gadget/rockchip
mkdir -p /sys/kernel/config/usb_gadget/rockchip/strings/0x409
mkdir -p ${USB_CONFIGS_DIR}/strings/0x409
echo 0x2207 > /sys/kernel/config/usb_gadget/rockchip/idVendor
echo 0x0310 > /sys/kernel/config/usb_gadget/rockchip/bcdDevice
echo 0x0200 > /sys/kernel/config/usb_gadget/rockchip/bcdUSB
echo 239 > /sys/kernel/config/usb_gadget/rockchip/bDeviceClass
echo 2 > /sys/kernel/config/usb_gadget/rockchip/bDeviceSubClass
echo 1 > /sys/kernel/config/usb_gadget/rockchip/bDeviceProtocol
SERIAL_NUM=`cat /proc/cpuinfo |grep Serial | awk -F ":" '{print $2}'`
echo "serialnumber is $SERIAL_NUM"
echo $SERIAL_NUM > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/serialnumber
echo "rockchip" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/manufacturer
echo "UVC" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/product
echo 0x1 > /sys/kernel/config/usb_gadget/rockchip/os_desc/b_vendor_code
echo "MSFT100" > /sys/kernel/config/usb_gadget/rockchip/os_desc/qw_sign
echo 500 > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/MaxPower
#ln -s /sys/kernel/config/usb_gadget/rockchip/configs/b.1 /sys/kernel/config/usb_gadget/rockchip/os_desc/b.1

#Windows computers will remember the device by default.
#Changing the pid does not require re-uninstalling and loading the windows driver.
echo 0x0016 > /sys/kernel/config/usb_gadget/rockchip/idProduct
if [ $UVC_MULTI = one ];then
   echo 0x0018 > /sys/kernel/config/usb_gadget/rockchip/idProduct
elif [ $UVC_MULTI = two ];then
   echo 0x001A > /sys/kernel/config/usb_gadget/rockchip/idProduct
fi

#uvc config init
uvc_device_config uvc.gs1 "UVC RGB"
if [ $UVC_MULTI = one ];then
   uvc_device_config uvc.gs2 "RK UVC"
elif [ $UVC_MULTI = two ];then
   uvc_device_config uvc.gs2 "RK UVC 1"
   uvc_device_config uvc.gs3 "RK UVC 2"
fi
##reset config,del default adb config
if [ -e ${USB_CONFIGS_DIR}/ffs.adb ]; then
   #for rk1808 kernel 4.4
   rm -f ${USB_CONFIGS_DIR}/ffs.adb
else
   ls ${USB_CONFIGS_DIR} | grep f[0-9] | xargs -I {} rm ${USB_CONFIGS_DIR}/{}
fi

case "$1" in
rndis)
    # config rndis
   mkdir /sys/kernel/config/usb_gadget/rockchip/functions/rndis.gs0
   echo "uvc_rndis" > ${USB_CONFIGS_DIR}/strings/0x409/configuration
   if [ $UVC_MULTI = one ];then
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f3
   elif [ $UVC_MULTI = two ];then
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f4
   else
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f2
   fi
   echo "config uvc and rndis..."
   ;;
uac1)
   uac1_device_config uac1
   echo "uvc_uac1" > ${USB_CONFIGS_DIR}/strings/0x409/configuration
   echo "config uvc and uac1..."
   ;;
uac2)
   uac2_device_config uac2
   echo "uvc_uac2" > ${USB_CONFIGS_DIR}/strings/0x409/configuration
   echo "config uvc and uac2..."
   ;;
uac1_rndis)
   #uac_device_config uac1
   mkdir /sys/kernel/config/usb_gadget/rockchip/functions/rndis.gs0
   if [ $UVC_MULTI = one ];then
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f4
   elif [ $UVC_MULTI = two ];then
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f5
   else
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f3
   fi
   uac1_device_config uac1
   echo "uvc_uac1_rndis" > ${USB_CONFIGS_DIR}/strings/0x409/configuration
   echo "config uvc and uac1 rndis..."
   ;;
uac2_rndis)
   #uac_device_config uac2
   mkdir /sys/kernel/config/usb_gadget/rockchip/functions/rndis.gs0
   if [ $UVC_MULTI = one ];then
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f4
   elif [ $UVC_MULTI = two ];then
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f5
   else
     ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f3
   fi
   uac2_device_config uac2
   echo "uvc_uac2_rndis" > ${USB_CONFIGS_DIR}/strings/0x409/configuration
   echo "config uvc and uac2 rndis..."
   ;;
*)
   echo "uvc" > ${USB_CONFIGS_DIR}/strings/0x409/configuration
   echo "config uvc ..."
esac

ln -s ${USB_FUNCTIONS_DIR}/uvc.gs1 ${USB_CONFIGS_DIR}/f1
if [ $UVC_MULTI = one ];then
   ln -s ${USB_FUNCTIONS_DIR}/uvc.gs2 ${USB_CONFIGS_DIR}/f2
elif [ $UVC_MULTI = two ];then
   ln -s ${USB_FUNCTIONS_DIR}/uvc.gs2 ${USB_CONFIGS_DIR}/f2
   ln -s ${USB_FUNCTIONS_DIR}/uvc.gs3 ${USB_CONFIGS_DIR}/f3
fi
if [ "$CDC_ENABLE" = "YES" ];then
	cdc_device_config
fi

UDC=`ls /sys/class/udc/| awk '{print $1}'`
echo $UDC > /sys/kernel/config/usb_gadget/rockchip/UDC

if [ "$1" ]; then
  pre_run_rndis $1
fi
