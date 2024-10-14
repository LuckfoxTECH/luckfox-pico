#!/bin/bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020 Rockchip Electronics Co., Ltd.

PROJECT=
project=
ROOT=$(pwd)
HAL_PATH=$2

if [[ $1 == "RK2108" ]] || [[ $1 == "RK2206" ]] ;then
    PROJECT=$1
else
	echo "./hal_cutter.sh PRJECT{Required} HAL_PATH{Required}"
	echo "eg. ./hal_cutter.sh RK2206"
	echo "run script in hal root path"
	echo "support PROJECT RK2108 RK2206"
	echo "for most case, we support runing .gitattributes firstly:"
	echo '  git archive --format tar.gz --prefix=hal/ -o "./hal.tar.gz" master --worktree-attributes'
	echo "  tar -xzvf hal.tar.gz"
	echo "  ./tools/hal_cutter.sh RK2108 ./hal"
	exit 0
fi

project=${PROJECT,,}

function check_and_rm_empty_dir
{
	if [ "`ls -A $1`" = "" ];
	then
		rm -rf $1
	fi
}

function rm_except_dir
{
	for dir in $(ls $1*)
	do
		if [ -d $1/$dir ];then
			if [[ $dir != $PROJECT && $dir != $project &&  $dir != $2 ]];then
				echo "rm $1/$dir"
				rm -rf $1/$dir
			fi
		else
			echo $dir is a file
		fi
	done
}

function rm_except_file
{
	for file in $(ls $1*)
	do
		if [[ `basename $file | grep $project ` == "" ]];then
			if [[ $2  == "" ]];then
				echo "rm $file"
				rm $file
			else
				if [[ `basename $file | grep $2 ` == "" ]];then
					echo "rm $file"
					rm $file
				fi
			fi
		fi
	done
}

if [ "`ls -A $HAL_PATH`" = "" ];
then
	echo "PATH{$HAL_PATH} is empty"
	exit 0
fi

#rm
rm_except_dir $HAL_PATH/lib/CMSIS/Device
rm_except_dir $HAL_PATH/lib/bsp
rm_except_dir $HAL_PATH/project common
rm_except_file $HAL_PATH/lib/hal/src/cru/hal_cru_
rm_except_file $HAL_PATH/lib/hal/src/pm/hal_pm_    cpu
rm_except_file $HAL_PATH/lib/hal/src/dsp/hal_dsp_
rm_except_file $HAL_PATH/lib/hal/src/crypto/hal_crypto_
check_and_rm_empty_dir 'hal/lib/hal/src/crypto/'

#not release
rm -rf $HAL_PATH/lib/hal/src/pm
