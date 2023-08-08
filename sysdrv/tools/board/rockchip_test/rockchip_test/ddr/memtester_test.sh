#!/bin/sh

RESULT_DIR=/data/cfg/rockchip_test
RESULT_LOG=${RESULT_DIR}/memtester.log

if [ ! -e "/data/cfg/rockchip_test" ]; then
	echo "no /data/cfg/rockchip_test"
	mkdir -p /data/cfg/rockchip_test
fi

# get memory free size
mem_free_size=$(cat /proc/meminfo | grep MemAvailable | awk '{print $2}')
mem_test_size="$((mem_free_size/2))K"

# run memtester test
echo "**********************DDR MEMTESTER TEST****************************"
echo "**********************run: memtester $mem_test_size***************************"
echo "**********************DDR MEMTESTER TEST****************************"
memtester $mem_test_size 2>&1 | tee $RESULT_LOG &

echo "***DDR MEMTESTER TEST START: you can see the log at $RESULT_LOG*****"
