#!/bin/sh
cmd=$(realpath $0)
_DIR=$(dirname $cmd)
cd $_DIR

export PATH=$PATH:/ko

#aic8800
if [ -f /usr/ko/aic8800_fdrv.ko ]; then
	mkdir -p /oem/usr/ko
	cp /usr/ko/aic8800dc_fw /oem/usr/ko -r
	insmod cfg80211.ko
	insmod libarc4.ko
	insmod ctr.ko
	insmod ccm.ko
	#insmod libaes.ko
	#insmod aes_generic.ko
	insmod aic8800_bsp.ko
	sleep 0.2
	insmod aic8800_fdrv.ko
	sleep 2
	insmod aic8800_btlpm.ko
	sleep 0.1
fi
