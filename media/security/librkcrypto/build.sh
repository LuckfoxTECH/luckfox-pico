#/bin/bash

set -e

collect_bin()
{
	cp $BUILD_DIR/librkcrypto.so $TARGET_LIB_DIR
	cp $BUILD_DIR/librkcrypto.a $TARGET_LIB_DIR
	cp $BUILD_DIR/test/librkcrypto_test $TARGET_BIN_DIR
	echo "copy target files to $TARGET_LIB_DIR success"

	# export head files
	cp $SCRIPT_DIR/include/rkcrypto_otp_key.h $TARGET_INCLUDE_DIR
	cp $SCRIPT_DIR/include/rkcrypto_common.h $TARGET_INCLUDE_DIR
	cp $SCRIPT_DIR/include/rkcrypto_core.h $TARGET_INCLUDE_DIR
	cp $SCRIPT_DIR/include/rkcrypto_mem.h $TARGET_INCLUDE_DIR
	echo "copy head files to $TARGET_INCLUDE_DIR success"
}

build()
{
	echo "build $ARM_BIT libraries and binaries"
	TARGET_LIB_DIR=$SCRIPT_DIR/out/target/lib/$ARM_BIT/
	TARGET_BIN_DIR=$SCRIPT_DIR/out/target/bin/$ARM_BIT/
	TARGET_INCLUDE_DIR=$SCRIPT_DIR/out/target/include/
	BUILD_DIR=$SCRIPT_DIR/out/build/$ARM_BIT/
	mkdir -p $TARGET_LIB_DIR
	mkdir -p $TARGET_BIN_DIR
	mkdir -p $TARGET_INCLUDE_DIR
	mkdir -p $BUILD_DIR
	cd $BUILD_DIR
	cmake $SCRIPT_DIR $DBUILD
	make -j12
}

BUILD_PARA="$1"
SCRIPT_DIR=$(pwd)

TOOLCHAIN_PREBUILTS=$SCRIPT_DIR/../../../prebuilts
TOOLCHAIN_PATH_ARM32=gcc/linux-x86/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin
TOOLCHAIN_PATH_AARCH64=gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin
CROSS_COMPILE32=$TOOLCHAIN_PREBUILTS/$TOOLCHAIN_PATH_ARM32/arm-none-linux-gnueabihf-gcc
CROSS_COMPILE64=$TOOLCHAIN_PREBUILTS/$TOOLCHAIN_PATH_AARCH64/aarch64-none-linux-gnu-gcc
CROSS_COMPILE32_GXX=$TOOLCHAIN_PREBUILTS/$TOOLCHAIN_PATH_ARM32/arm-none-linux-gnueabihf-g++
CROSS_COMPILE64_GXX=$TOOLCHAIN_PREBUILTS/$TOOLCHAIN_PATH_AARCH64/aarch64-none-linux-gnu-g++

if [ $# -eq 0 ]; then
	# build both 32-bit and 64-bit
	DBUILD="-DCMAKE_C_COMPILER=$CROSS_COMPILE32 \
		-DCMAKE_CXX_COMPILER=$CROSS_COMPILE32_GXX"
	ARM_BIT="arm"
	build
	collect_bin

	DBUILD="-DCMAKE_C_COMPILER=$CROSS_COMPILE64 \
		-DCMAKE_CXX_COMPILER=$CROSS_COMPILE64_GXX"
	ARM_BIT="arm64"
	build
	collect_bin
else
	if [ $BUILD_PARA == "32" ]; then
		DBUILD="-DCMAKE_C_COMPILER=$CROSS_COMPILE32 \
			-DCMAKE_CXX_COMPILER=$CROSS_COMPILE32_GXX"
		ARM_BIT="arm"
	else
		DBUILD="-DCMAKE_C_COMPILER=$CROSS_COMPILE64 \
			-DCMAKE_CXX_COMPILER=$CROSS_COMPILE64_GXX"
		ARM_BIT="arm64"
	fi

	build
	collect_bin
fi
