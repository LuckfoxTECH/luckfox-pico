#!/bin/bash

# A helper script to analyze coredump files
# Usage: ./analyze_coredump.sh core-PID-NAME
# GDB require several infomations
# 1. The SYSROOT path for glibc/stdc++ and other library symbols
# 2. The solib-search-path, the script set it to current build outputs
# 3. The coredump files, can be pulled from device's /data/core-PID-NAME
# 4. make sure the demo and aiq library is matched to the ones running on device

export SDK_DIR=/home/camera/camera/rv1109_sdk
export SDK_OUT_DIR=$SDK_DIR/buildroot/output/rockchip_rv1126_rv1109
export SYSROOT_DIR=$SDK_OUT_DIR/host/arm-buildroot-linux-gnueabihf/sysroot
export TOOLCHAIN_DIR=$SDK_DIR/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf
export TARGET_GDB=$TOOLCHAIN_DIR/bin/arm-linux-gnueabihf-gdb
export AIQ_OUT_DIR=$(pwd)/../../../rk_aiq/build/linux/all_lib/debug
export DEMO_PATH=$(pwd)/exe/debug/rkisp_demo

COREDUMP_FILE=$1

[ -d "$SYSROOT_DIR" ] || { echo "sysroot is not found" ; exit 1; }
[ -f "$TARGET_GDB" ] || { echo "gdb is not found" ; exit 1; }
[ -f "$AIQ_OUT_DIR/librkaiq.so" ] || { echo "librkaiq.so is not found" ; exit 1; }
[ -f "$DEMO_PATH" ] || { echo "demo is not found" ; exit 1; }
[ -f "$COREDUMP_FILE" ] || { echo "coredump is not found, please do adb pull /data/core-foo from device" ; exit 1; }

$TARGET_GDB -x gdbinit $DEMO_PATH $COREDUMP_FILE
