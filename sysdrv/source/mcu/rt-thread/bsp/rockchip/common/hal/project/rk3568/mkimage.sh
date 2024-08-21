#!/bin/bash
# SPDX-License-Identifier: BSD-3-Clause */
# Copyright (c) 2022 Rockchip Electronics Co., Ltd.

set -e

export LC_ALL=C.UTF-8
export LANG=C.UTF-8

usage() {
	echo "usage: ./mkimage.sh"
}

CUR_DIR=$(pwd)
HAL_DIR=$(pwd)/../..
IMAGE=$(pwd)/Image

cp -r $CUR_DIR/GCC/hal*.bin $IMAGE/
$HAL_DIR/tools/mkimage -f $IMAGE/amp.its -E -p 0xe00 $IMAGE/amp.img

echo 'Image: amp.img is ready.'
