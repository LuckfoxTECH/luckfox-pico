#!/bin/bash

BUILD_DIR=build_linux
BINARY_DIR=$BUILD_DIR/bin

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
mkdir -p $BINARY_DIR
pushd $BUILD_DIR

if [ "$1" = "drm" ];then
	echo "using drm"
	cmake -DCMAKE_BUILD_TARGET=buildroot \
		  -DCMAKE_BUILD_THIRD_PARTY=drm \
		  -DCMAKE_INSTALL_PREFIX=install \
		  ..
else
	echo "Default mode"
	cmake -DCMAKE_BUILD_TARGET=buildroot \
		  -DCMAKE_INSTALL_PREFIX=install \
		  ..
fi

make
make install

popd
