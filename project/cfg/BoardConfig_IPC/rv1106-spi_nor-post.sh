#!/bin/bash

cd $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/
rm face_mask_classify.data
rm face_quality_v2.data
rm object_detection_ipc_cls7.data
rm object_detection_ipc_cls7_v.data
rm object_detection_pfp_896x512.data
cd -

rm $RK_PROJECT_PACKAGE_ROOTFS_DIR/lib/libstdc++.so.6.0.25-gdb.py

# delete nouse ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/gcm.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/ccm.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/sha256_generic.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/libaes.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/libsha256.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/gf128mul.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/cmac.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/rve.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/libarc4.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/aes_generic.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/ctr.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/mac80211.ko
rm -f $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/atmb_iot_supplicant_demo
