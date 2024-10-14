#!/bin/bash

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
sh /etc/init.d/S20linkmount start &

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
copy_data
change_mode
rewrite_script
