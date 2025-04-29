#!/bin/bash

# remove unused files
function lf_rm() {
	for file in "$@"; do
		if [ -e "$file" ]; then
			echo "Deleting: $file"
			rm -rf "$file"
			#else
			#echo "File not found: $file"
		fi
	done
}

# remove unused links
function lf_rm_link() {
	for link in "$@"; do
		if [ -L "$link" ]; then
			echo "Deleting: $link"
			rm -rf "$link"
			#else
			#echo "File not found: $file"
		fi
	done
}

# remove ModemManager Service
function remove_modem_service() {
	lf_rm_link $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/systemd/system/dbus-org.freedesktop.ModemManager1.service
	lf_rm_link $RK_PROJECT_PACKAGE_ROOTFS_DIR/etc/systemd/system/multi-user.target.wants/ModemManager.service
}

#=========================
# run
#=========================
remove_modem_service
