#!/bin/bash

#function copy_data() {
	# for writable, package them to userdata parition
#	mkdir -p $RK_PROJECT_PACKAGE_USERDATA_DIR
	#cp -r $RK_PROJECT_PATH_APP/userdata/* $RK_PROJECT_PACKAGE_USERDATA_DIR
#}

function change_mode() {
    chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/firstboot
    chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/init.d/ubiattach
    chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/local.d/crond.start
    chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/sbin/firstboot.sh
    chmod +x $RK_PROJECT_PACKAGE_ROOTFS_DIR/usr/bin/iomux	
}

#=========================
# post
#=========================
#remove_data
#copy_data
change_mode
#rewrite_script
