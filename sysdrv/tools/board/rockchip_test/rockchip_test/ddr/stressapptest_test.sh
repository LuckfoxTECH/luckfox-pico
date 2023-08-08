#!/bin/sh

RESULT_DIR=/data/cfg/rockchip_test
RESULT_LOG=${RESULT_DIR}/stressapptest.log

if [ ! -e "/data/cfg/rockchip_test" ]; then
	echo "no /data/cfg/rockchip_test"
	mkdir -p /data/cfg/rockchip_test
fi

# get memory free size
mem_avail_size=$(cat /proc/meminfo | grep MemAvailable | awk '{print $2}')
if [ $mem_avail_size -gt "131072" ]; then
	mem_test_size="128"
else
	mem_test_size=$((mem_avail_size/1024/4*4-4))
fi

#run stressapptest_test
echo "**********************DDR STRESSAPPTEST TEST 48H*************************"
echo "***run: stressapptest -s 172800 -i 4 -C 4 -W --stop_on_errors -M $mem_test_size*****"
echo "**********************DDR STRESSAPPTEST TEST****************************"
stressapptest -s 172800 -i 4 -C 4 -W --stop_on_errors -M $mem_test_size -l $RESULT_LOG &

echo "******DDR STRESSAPPTEST START: you can see the log at $RESULT_LOG********"
