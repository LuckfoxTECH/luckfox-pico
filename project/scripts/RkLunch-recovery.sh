#!/bin/sh

SD_DEVICES="mmcblk1 mmcblk2"
DEV_NAME=""
MOUNT_PATH=""
UPDATE_PACKAGE=""
UPDATE_OTA_DIR_PATH=""
delay_time=""
input_file="$1"
input_path="$2"

sleep 1
echo "Ready to update."

# Use the specified update package in preference.
if [ -f "$input_file" ]; then
  UPDATE_PACKAGE=$(realpath $input_file)
  UPDATE_OTA_DIR_PATH=$(dirname $UPDATE_PACKAGE)/update
  [ -d "$input_path" ] && UPDATE_OTA_DIR_PATH="$input_path/update"

  echo "Update package: $UPDATE_PACKAGE"
  mkdir -p $UPDATE_OTA_DIR_PATH
  tar -xvf $UPDATE_PACKAGE -C $UPDATE_OTA_DIR_PATH
  if [ $? -ne 0 ]; then
    echo "Error: Decompression failed, please check the update package(tar)."
    exit 1
  fi

  sh $UPDATE_OTA_DIR_PATH/RK_OTA_update.sh
  if [ $? -eq 0 ]; then
    echo "Upgrade successfully."
    rm -rf $UPDATE_OTA_DIR_PATH
    reboot
    return
  else
    echo "Error: Upgrade failed, please check the partition."
    rm -rf $UPDATE_OTA_DIR_PATH
    exit 1
  fi
else
  echo "Warning: Update package ($1) was not found, checking SD card..."
fi

# Check SD card when not input update package path.
for DEV_NAME in $SD_DEVICES
do
  echo "************** DEV_NAME=$DEV_NAME **************"
  delay_time=1
  mount | grep -i "$DEV_NAME"
  if [ $? -eq 0 ]; then
    MOUNT_PATH=`mount | grep -i "$DEV_NAME" | awk '{printf $3}'`
    echo "SDcard is already mounted in $MOUNT_PATH"
  else
    MOUNT_PATH=/mnt/sdcard
  fi

  while [ $delay_time -le 5 ]
  do
    usleep 200000
    dev=$(ls /dev/$DEV_NAME*)
    if [ -n "$dev" ]; then
      echo Find the device: $dev
      break 2
    fi
    echo "Find $DEV_NAME failed $delay_time times"
    delay_time=$(($delay_time+1))
  done
done

if [ -z "$dev" ]; then
  echo "Error: Not found device, skipping upgrade."
  sh /usr/bin/RK_OTA_erase_misc.sh
  [ $? -eq 0 ] && reboot
  exit 1
fi

mkdir -p $MOUNT_PATH
UPDATE_OTA_DIR_PATH=$MOUNT_PATH/update
UPDATE_PACKAGE=$MOUNT_PATH/update_ota.tar

for SD_DEV in $dev
do
  echo "************** SD_DEV=$SD_DEV **************"
  if mount -t vfat $SD_DEV $MOUNT_PATH ; then
    if [ -f $UPDATE_PACKAGE ]; then
      echo "Update package: $UPDATE_PACKAGE"
      mkdir -p $UPDATE_OTA_DIR_PATH
      tar -xvf $UPDATE_PACKAGE -C $UPDATE_OTA_DIR_PATH
      if [ $? -ne 0 ]; then
        echo "Error: Decompression failed, please check the update package(tar)."
        exit 1
      fi

      sh $UPDATE_OTA_DIR_PATH/RK_OTA_update.sh
      if [ $? -eq 0 ]; then
        echo "Upgrade successfully."
        rm -rf $UPDATE_OTA_DIR_PATH
        reboot
        return
      else
        echo "Error: Upgrade failed, please check the partition."
        rm -rf $UPDATE_OTA_DIR_PATH
        exit 1
      fi
    else
      echo "Warning: Not found update package: $UPDATE_PACKAGE"
    fi
    umount $MOUNT_PATH
  fi
done

echo "Recovery upgrade failed, skipping."
sh /usr/bin/RK_OTA_erase_misc.sh
[ $? -eq 0 ] && reboot
exit 1
