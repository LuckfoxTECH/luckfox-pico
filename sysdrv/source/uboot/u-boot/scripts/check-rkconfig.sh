#!/bin/bash
#
# Copyright (c) 2023 Rockchip Electronics Co., Ltd
#
# SPDX-License-Identifier: GPL-2.0
#

set -e

if [ -f dts/kern.dtb ]; then
	if ! grep -Eq 'CONFIG_EMBED_KERNEL_DTB=y' .config ; then
		echo "ERROR: dts/kern.dtb was found, but CONFIG_EMBED_KERNEL_DTB is disabled."
		exit 1
	fi
fi

if grep -Eq 'CONFIG_EMBED_KERNEL_DTB=y' .config ; then
	KDTB=`sed -n "/CONFIG_EMBED_KERNEL_DTB_PATH=/s/CONFIG_EMBED_KERNEL_DTB_PATH=//p" .config | tr -d '\r' | tr -d '"'`
	if [ ! -f ${KDTB} ]; then
		echo "ERROR: '${KDTB}' was not found assigned by CONFIG_EMBED_KERNEL_DTB_PATH."
		exit 1
	fi
fi
