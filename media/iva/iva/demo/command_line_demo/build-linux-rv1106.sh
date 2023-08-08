#!/bin/bash

ARM_GCC_COMPILER_PATH=~/opts/arm-rockchip830-linux-uclibcgnueabihf

#BUILD_TYPE=Release
BUILD_TYPE=Debug

ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

BUILD_DIR=${ROOT_PWD}/build/build_rv1106_linux

if [[ ! -d "${BUILD_DIR}" ]]; then
  mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake ../.. \
    -DCMAKE_C_COMPILER=${ARM_GCC_COMPILER_PATH}/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc \
    -DCMAKE_CXX_COMPILER=${ARM_GCC_COMPILER_PATH}/bin/arm-rockchip830-linux-uclibcgnueabihf-g++ \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_INSTALL_PREFIX=${ROOT_PWD}/install/rockiva_rv1106_linux
make
make install
cd -
