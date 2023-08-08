#!/bin/sh
WIFISSID=$1
WIFIPWD=$2
CONF=/tmp/wpa_supplicant.conf

cp /etc/wpa_supplicant.conf /tmp/
echo "connect to WiFi ssid: $WIFISSID, Passwd: $WIFIPWD"
sed -i "s/SSID/$WIFISSID/g" $CONF
sed -i "s/PASSWORD/$WIFIPWD/g" $CONF
killall wpa_supplicant
killall wpa_supplicant_nl80211
sleep 1

#rtl8189fs
cat /sys/bus/sdio/devices/*/uevent | grep "024C:F179"
if [ $? -eq 0 ];then
	wpa_supplicant_rtk -B -i wlan0 -c $CONF
fi

#rtl8188fu
cat /sys/bus/usb/devices/*/uevent | grep "bda\/f179"
if [ $? -eq 0 ];then
	wpa_supplicant_rtk -B -i wlan0 -c $CONF
fi

#ssv6155p
cat /sys/bus/usb/devices/*/uevent | grep "8065\/6000"
if [ $? -eq 0 ];then
	wpa_supplicant -B -D nl80211 -i wlan0 -c $CONF
fi

#atbm603x
cat /sys/bus/usb/devices/*/uevent | grep "007A:6011"
if [ $? -eq 0 ];then
	wpa_supplicant -B -D nl80211 -i wlan0 -c $CONF
fi
