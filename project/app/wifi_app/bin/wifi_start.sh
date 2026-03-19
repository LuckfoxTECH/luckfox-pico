#!/bin/sh
WIFISSID=$1
WIFIPWD=$2
CONF=/tmp/wpa_supplicant.conf

echo "connect to WiFi ssid: $WIFISSID"

cat > $CONF <<EOF
ctrl_interface=/var/run/wpa_supplicant
ap_scan=1
update_config=1

network={
    ssid="$WIFISSID"
    psk="$WIFIPWD"
    key_mgmt=WPA-PSK
}
EOF

killall wpa_supplicant
killall wpa_supplicant_nl80211
sleep 1

#AP6XXX
cat /sys/bus/sdio/devices/*/uevent | grep "02D0"
if [ $? -eq 0 ];then
	insmod /oem/usr/ko/bcmdhd.ko
	wpa_supplicant -B -i wlan0 -c $CONF
fi

#rtl8189fs
cat /sys/bus/sdio/devices/*/uevent | grep "024C:F179"
if [ $? -eq 0 ];then
	wpa_supplicant -B -i wlan0 -c $CONF
fi

#rtl8188fu
cat /sys/bus/usb/devices/*/uevent | grep "bda\/f179"
if [ $? -eq 0 ];then
	wpa_supplicant -B -i wlan0 -c $CONF
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

#aic8800
cat /sys/bus/sdio/devices/*/uevent | grep "C8A1:C18D"
if [ $? -eq 0 ];then
	wpa_supplicant -B -D nl80211 -i wlan0 -c $CONF
fi
