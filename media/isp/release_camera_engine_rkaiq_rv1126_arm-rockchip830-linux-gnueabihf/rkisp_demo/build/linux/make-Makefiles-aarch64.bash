#!/bin/bash
# Run this from within a bash shell
# x86_64 is for simulation do not enable RK platform
export AIQ_BUILD_HOST_DIR=/home/camera/camera/rk356x_linux/buildroot/output/rockchip_rk3566/host
TOOLCHAIN_FILE=$(pwd)/aarch64_linux_buildroot.cmake
SOURCE_PATH=$(pwd)/../../
OUTPUT=$(pwd)/output/aarch64

mkdir -p $OUTPUT
pushd $OUTPUT

cmake -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=debug \
    -DARCH="aarch64" \
    -DRKPLATFORM=OFF \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
    -DCMAKE_SKIP_RPATH=TRUE \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES \
    $SOURCE_PATH \
&& make -j$(nproc)

popd
