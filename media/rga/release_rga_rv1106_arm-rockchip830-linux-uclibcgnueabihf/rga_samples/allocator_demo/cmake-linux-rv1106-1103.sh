#!/bin/bash

SCRIPT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
SAMPLES_DIR=${SCRIPT_DIR}/..

# Modify to the local toolchain path.
TOOLCHAIN_PATH=${SAMPLES_DIR}/../toolchains/toolchain_linux_1106.cmake
LIBRGA_PATH=${SAMPLES_DIR}/../build/build_linux_1106/install/lib
BUILD_DIR=build/build_linux_1106
BUILD_TYPE=Release

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
pushd $BUILD_DIR

cmake ../.. \
	-DTARGET_SOC=RV1106 \
	-DLIBRGA_FILE_LIB=${LIBRGA_PATH} \
	-DBUILD_TOOLCHAINS_PATH=${TOOLCHAIN_PATH} \
	-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
	-DCMAKE_INSTALL_PREFIX=install \

make -j8
make install

popd
