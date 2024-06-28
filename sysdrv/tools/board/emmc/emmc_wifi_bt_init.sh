#!/bin/bash

if [ "$(cat /proc/device-tree/model)" == "Luckfox Pico Ultra W" ]; then
	systemctl stop wpa_supplicant
	if [ -d /usr/ko ]; then
		cd /usr/ko
		if [ -z "$(ifconfig | grep "wlan0")" ]; then
			./insmod_wifi.sh
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
