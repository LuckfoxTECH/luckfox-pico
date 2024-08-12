#!/bin/bash

if [ "$(cat /proc/device-tree/model)" == "Luckfox Pico Ultra W" ]; then
	systemctl stop wpa_supplicant
	if [ -d /oem/usr/ko ]; then
		cd /oem/usr/ko
		if [ -z "$(ifconfig | grep "wlan0")" ]; then
			insmod cfg80211.ko
			insmod libarc4.ko
			insmod ctr.ko
			insmod ccm.ko
			insmod aes_generic.ko
			insmod aic8800_bsp.ko
			sleep 0.2
			insmod aic8800_fdrv.ko
			sleep 2
			insmod aic8800_btplm.ko
		else
			# wait systemctl
			sleep 0.5
		fi
	else
		echo "Missing ko files!"
	fi

	if [ -d /var/run/wpa_supplicant ]; then
		rm /var/run/wpa_supplicant/ -rf
	fi

	if [ -f /etc/wpa_supplicant.conf ] && [ -n "$(ifconfig | grep "wlan0")" ]; then
		wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf >/dev/null
		chmod a+x /usr/share/udhcpc/default.script
	fi
else
	echo "This Luckfox Pico model don't support WIFI!"
fi
