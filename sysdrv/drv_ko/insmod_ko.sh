#!/bin/sh
cmd=`realpath $0`
_DIR=`dirname $cmd`
cd $_DIR

udevadm control --stop-exec-queue

__insmod()
{
	if [ -f "$1" ];then
		insmod $@
	fi
}

__insmod rk_dvbm.ko

__insmod videobuf2-memops.ko
__insmod videobuf2-common.ko
__insmod videobuf2-v4l2.ko
__insmod videobuf2-vmalloc.ko
__insmod videobuf2-cma-sg.ko

__insmod video_rkcif.ko
__insmod video_rkisp.ko
__insmod phy-rockchip-csi2-dphy-hw.ko
__insmod phy-rockchip-csi2-dphy.ko

__insmod os04a10.ko
__insmod sc4336.ko
__insmod sc3336.ko
__insmod sc530ai.ko

echo 1 > /sys/module/video_rkcif/parameters/clr_unready_dev
echo 1 > /sys/module/video_rkisp/parameters/clr_unready_dev

__insmod rga3.ko

__insmod mpp_vcodec.ko

__insmod rknpu.ko
__insmod snd-soc-rv1106.ko

__insmod motor.ko

# rmmod non-exist camera driver
for drv_name in `ls /sys/devices/platform/ff470000.i2c/i2c-4/4*/name`; do
	dir=`dirname $drv_name`
	if [ ! -L $dir/driver ]; then
		rmmod `cat $drv_name`
	fi
done

# rv1103 unsupport 5M
grep -q "rockchip,rv1103" /proc/device-tree/compatible
if [ $? == 0 ]; then
	rmmod sc530ai
fi

sensor_height=0
lsmod | grep sc530ai
if [ $? -eq 0 ] ;then
    sensor_height=1616
fi
lsmod | grep sc4336
if [ $? -eq 0 ] ;then
    sensor_height=1440
fi
lsmod | grep sc3336
if [ $? -eq 0 ] ;then
    sensor_height=1296
fi

__insmod rockit.ko mcu_fw_path="./hpmcu_wrap.bin" mcu_fw_addr=0xff6ff000 isp_max_h=$sensor_height

udevadm control --start-exec-queue

# insmod wifi driver background
$(pwd)/insmod_wifi.sh &
