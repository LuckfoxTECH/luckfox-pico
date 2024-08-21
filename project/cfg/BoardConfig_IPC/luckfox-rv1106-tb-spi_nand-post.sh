#!/bin/bash

# remove unused files
function remove_data()
{
	if [ "$RK_BUILD_APP_TO_OEM_PARTITION" != "y" ];then
		# delete except app1, app2, app3, do like this: delete_except=".*/\|.*/app1\|.*/app2\|.*app3"
		delete_except=".*/\|.*/smart_door\|.*/fastboot_server"
		find "$RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/bin" ! -regex $delete_except -delete

		# both smart_door and fastboot_server need the so
		delete_except=".*/\|.*/librkaiq.so"
		find "$RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/usr/lib" ! -regex $delete_except -delete

		local unused_in_oem=(\
			# wifi ko
			# "usr/ko/cfg80211.ko" \
			# "usr/ko/gf128mul.ko" \
			# "usr/ko/ctr.ko"  \
			# "usr/ko/libaes.ko" \
			# "usr/ko/sha256_generic.ko" \
			# "usr/ko/aes_generic.ko" \
			# "usr/ko/ccm.ko" \
			# "usr/ko/libarc4.ko" \
			# "usr/ko/cmac.ko" \
			# "usr/ko/mac80211.ko" \
			# "usr/ko/ghash-generic.ko" \
			# "usr/ko/libsha256.ko" \
			# "usr/ko/gcm.ko" \
			# vendor_storage ko
			"usr/ko/rk_vendor_storage.ko" \
			"usr/ko/mtd_vendor_storage.ko" \
			# rve.ko
			"usr/ko/rve.ko" \
			# emmc flash
			"usr/ko/jbd2.ko" \
			"usr/ko/mbcache.ko" \
			"usr/ko/crc16.ko" \
			"usr/ko/ext4.ko" \
			# unuesd scripts
			"usr/ko/*.sh" \
			# unused data files
			"usr/lib/*.data" \
			# oem 
			"/oem/usr/lib/*.so*" \
			"/oem/usr/lib/*.md" \
			"/oem/usr/lib/librkdemuxer.so" \
			"/oem/usr/bin/j2s4b_dev" \
			)

		for i in ${unused_in_oem[@]}
		do
			if [ -f $RK_PROJECT_PACKAGE_ROOTFS_DIR/$i ] ;then 
				rm -rf $RK_PROJECT_PACKAGE_ROOTFS_DIR/oem/$i
			fi
		done
	fi

	local unused_in_rootfs=(\
		"lib/libatomic.so*" \
		"etc/iqfiles/*" \
		"etc/services" \
		"etc/protocols"  \
		"lib/libstdc++.so.6.0.25-gdb.py" \
		"lib/libitm.so*" \
		"usr/share/udhcpc" \
		"usr/bin/flash*" \
		"usr/bin/dnsmasq" \
		"usr/bin/hostapd" \
		"usr/bin/iperf" \
		"usr/bin/wifi_start.sh" \
		"mnt/sdcard" \
		)

	for i in ${unused_in_rootfs[@]}
	do
		if [ -f $RK_PROJECT_PACKAGE_USERDATA_DIR/$i ] ;then 	
			rm -rf $RK_PROJECT_PACKAGE_ROOTFS_DIR/$i
		fi
	done

	# disable S20linkmount
	mv $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/S20linkmount $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/_S20linkmount

}

function copy_data() {
	# for writable, package them to userdata parition
	mkdir -p $RK_PROJECT_PACKAGE_USERDATA_DIR
	#cp -r $RK_PROJECT_PATH_APP/userdata/* $RK_PROJECT_PACKAGE_USERDATA_DIR
}

function change_mode() {
	# uvc related files
	chmod a+x $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/usb_config.sh
}

function rewrite_script() {
	# export default rk_log_level for app
	echo "export rt_log_level=4" >>$RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/profile.d/RkEnv.sh

	# remove unused scripts
	rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/S21appinit*
	rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/S20urandom*

	# rewite rcS
	cat >$RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcS <<EOF
#!/bin/sh
echo 256 > /proc/sys/kernel/threads-max

if [ -f "/etc/profile.d/RkEnv.sh" ]; then
	source /etc/profile.d/RkEnv.sh
fi

echo "start server" > /dev/kmsg
test -n "\$persist_camera_engine_log" && \
	fastboot_server &>/tmp/rkaiq0.log || \
	fastboot_server &>/tmp/fastboot_dbg.log &

cd /oem/usr/ko
test ! -f jbd2.ko || insmod jbd2.ko
test ! -f crc16.ko || insmod crc16.ko
test ! -f mbcache.ko || insmod mbcache.ko
test ! -f ext4.ko || insmod ext4.ko
test ! -f dw_mmc-rockchip.ko || insmod dw_mmc-rockchip.ko
sh /etc/init.d/_S20linkmount start &>/dev/null &

echo "start app" > /dev/kmsg
test -n "\$persist_camera_engine_log" && \
	smart_door &>/tmp/rkaiq1.log || \
	smart_door &>/tmp/smart_door_dbg.log &

test ! -f snd-soc-rv1106.ko || insmod snd-soc-rv1106.ko
test ! -f rknpu.ko || insmod rknpu.ko

test ! -f usb-common.ko || insmod usb-common.ko
test ! -f udc-core.ko || insmod udc-core.ko
test ! -f dwc3-of-simple.ko || insmod dwc3-of-simple.ko
test ! -f dwc3.ko || insmod dwc3.ko
#test ! -f extcon-core.ko || insmod extcon-core.ko
test ! -f phy-rockchip-inno-usb2.ko || insmod phy-rockchip-inno-usb2.ko
#test ! -f configfs.ko || insmod configfs.ko
test ! -f libcomposite.ko || insmod libcomposite.ko
test ! -f usb_f_uvc.ko || insmod usb_f_uvc.ko
#usb_config.sh &

ifconfig eth0 up
#rndis.sh

#test ! -f /oem/usr/ko/insmod_wifi.sh || /oem/usr/ko/insmod_wifi.sh ${RK_ENABLE_FASTBOOT} ${RK_ENABLE_WIFI_CHIP}
EOF
	chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcS

	# rewite rcK
	cat >$RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcK <<EOF
#!/bin/sh
echo "Start to killall task!!!"

while true
do
        if ps |grep -v grep  |grep smart_door;then
                echo "killall -9 smart_door"
                killall -9 smart_door
        else
                break
        fi
        sleep .5
done

killall fastboot_server
killall rkwifi_server
umount /data
EOF
	chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/rcK

	# rewrite profile
	cat >$RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/profile <<EOF
export PATH="/bin:/sbin:/usr/bin:/usr/sbin"

if [ "\$PS1" ]; then
	if [ "\`id -u\`" -eq 0 ]; then
    	export PS1='[\u@\h \W]# '
	else
		export PS1='[\u@\h \W]$ '
	fi
fi

export PAGER='/bin/more '
export EDITOR='/bin/vi'

# Source configuration files from /etc/profile.d
for i in /etc/profile.d/*.sh ; do
	if [ -r "\$i" ]; then
		. \$i
	fi
	unset i
done
EOF
	chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/profile

	# rewrite hostname
	cat >$RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/hostname <<EOF
luckfox
EOF

}

#=========================
# post
#=========================
remove_data
copy_data
change_mode
rewrite_script