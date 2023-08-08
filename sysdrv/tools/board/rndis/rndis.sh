#!/bin/sh

ADB_EN=off
USB_FUNCTIONS_DIR=/sys/kernel/config/usb_gadget/rockchip/functions
USB_CONFIGS_DIR=/sys/kernel/config/usb_gadget/rockchip/configs/b.1


##main
#init usb config

insmod /oem/usr/ko/usb-common.ko
insmod /oem/usr/ko/usbcore.ko
insmod /oem/usr/ko/udc-core.ko
insmod /oem/usr/ko/libcomposite.ko
insmod /oem/usr/ko/u_ether.ko
insmod /oem/usr/ko/usb_f_fs.ko
insmod /oem/usr/ko/usb_f_rndis.ko
insmod /oem/usr/ko/phy-rockchip-inno-usb2.ko
insmod /oem/usr/ko/dwc3-of-simple.ko
insmod /oem/usr/ko/dwc3.ko

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
echo 0x0016 > /sys/kernel/config/usb_gadget/rockchip/idProduct

mkdir /sys/kernel/config/usb_gadget/rockchip/functions/rndis.gs0
echo "rndis" > ${USB_CONFIGS_DIR}/strings/0x409/configuration
ln -s ${USB_FUNCTIONS_DIR}/rndis.gs0 ${USB_CONFIGS_DIR}/f2


UDC=`ls /sys/class/udc/| awk '{print $1}'`
echo $UDC > /sys/kernel/config/usb_gadget/rockchip/UDC

#sleep 1
echo "config usb0 IP..."
ifconfig usb0 192.168.1.100
ifconfig usb0 up
