#!/bin/bash

# Check if the filesystem has been resized previously
if [ ! -f /etc/.filesystem_resized ]; then
	# Perform filesystem resize
	sudo resize2fs /dev/mmcblk0p5
	sudo resize2fs /dev/mmcblk0p6
	sudo resize2fs /dev/mmcblk0p7

	# Create a marker file indicating filesystem resize has been done
	sudo touch /etc/.filesystem_resized

	echo "Filesystem resized successfully."
fi

if [ ! -f /etc/.filesystem_swap ]; then
	sudo fallocate -l 1G /swapfile
	sudo chmod 600 /swapfile
	sudo mkswap /swapfile >/dev/null
	sudo swapon /swapfile >/dev/null
	echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab >/dev/null

	sudo touch /etc/.filesystem_swap
	echo "Swap successfully."
fi
