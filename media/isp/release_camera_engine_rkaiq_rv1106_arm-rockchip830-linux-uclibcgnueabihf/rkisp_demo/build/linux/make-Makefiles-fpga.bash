#!/bin/bash
# Run this from within a bash shell
# x86_64 is for simulation do not enable RK platform
export AIQ_BUILD_HOST_DIR=/home/hkj/isp2-fpga/buildroot_xhf/buildroot/output/host
TOOLCHAIN_FILE=$(pwd)/../../../rk_aiq/cmake/toolchains/arm_fpga_linux_buildroot.cmake
OUTPUT=$(pwd)/output/arm_fpga
SOURCE_PATH=$OUTPUT/../../../../

FPGA_RGA_INCLUDE_PATH=/home/camera/camera/rv1109_sdk/buildroot/output/rockchip_rv1126_rv1109/host/arm-buildroot-linux-gnueabihf/sysroot/usr/

mkdir -p $OUTPUT
pushd $OUTPUT

cmake -G "Ninja" \
    -DCMAKE_BUILD_TYPE=debug \
    -DARCH="fpga" \
    -DRKPLATFORM=OFF \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
    -DCMAKE_SKIP_RPATH=TRUE \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES \
	-DISP_HW_VERSION=${ISP_HW_VERSION} \
	-DFPGA_RGA_INCLUDE_PATH=${FPGA_RGA_INCLUDE_PATH}\
    $SOURCE_PATH \
&& ninja -j$(nproc)

popd
