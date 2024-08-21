#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# Copyright (c) 2020, Linaro Limited.

if [ $# -ne 2 ]; then
	echo "usage: prepare_suite.sh <optee_test base dir> <gp suite archive>"
	exit 1
fi

set -e

if [ -e $1/host/xtest/gp-suite ]; then
	# Clean up last patching in case it failed in some way
	rm -rf $1/host/xtest/gp-suite
fi
mkdir -p $1/host/xtest/gp-suite
7z x -o$1/host/xtest/gp-suite $2
cd $1/host/xtest/gp-suite
mv TEE_Initial_Configuration-Test_Suite_v2_0_0_2-2017_06_09/* .
rmdir TEE_Initial_Configuration-Test_Suite_v2_0_0_2-2017_06_09
rm -f TTAs_Internal_API_1_1_1/TEE_include/tee_internal_api.h
chmod -R u+w .
for p in `echo ../gp/patches/*.patch | sort -n` ;do
	echo Applying $p
	patch -p1 < $p
done
