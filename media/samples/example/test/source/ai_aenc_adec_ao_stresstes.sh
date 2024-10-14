#!/bin/sh

set -x

__echo_test_cmd_msg()
{
	echo -e "$1" | tee -a $test_result_path
	if [ $? -ne 0 ]; then
		echo -e "$1"
	fi
}
__chk_cma_free()
{
	local f
	if [ ! -f "/proc/rk_dma_heap/alloc_bitmap" ];then
		echo "[$0] not found /proc/rk_dma_heap/alloc_bitmap, ignore"
		return
	fi
	f=`head  /proc/rk_dma_heap/alloc_bitmap |grep Used|awk '{print $2}'`
	if [ $f -gt 12 ];then
		echo "[$0] free cma error"
		exit 2
	fi
}

test_cmd()
{
	if [ -z "$*" ];then
		echo "not found cmd, return"
		return
	fi
	__echo_test_cmd_msg "TEST    [$*]"
	eval $*
	__chk_cma_free
	if [ $? -eq 0 ]; then
		__echo_test_cmd_msg "SUCCESS [$*]"
	else
		__echo_test_cmd_msg "FAILURE [$*]"
		exit 1
	fi
}


print_help()
{
    echo "example: $0  <test_loop>"
    echo -e "
          \$1 --------test_loop: 10000\n"
}

#set test mode
test_mode=$1
if [ ! -n "$1" ]; then
    echo "----------------- error!!!, lack test_mode, please input test mode"
    print_help
    exit 1
else
    echo --------test_mode: $test_mode
fi

#test result path
test_result_path=/tmp/sample_ai_aenc_adec_ao_stresstest.log

#set test loop
test_loop=$2
if [ ! -n "$2" ]; then
    echo "----------------- error!!!, lack test_loop, please input test loop"
    print_help
    exit 1
else
    echo --------test_loop: $test_loop
fi

killall rkipc
while true
do
    ps|grep rkipc |grep -v grep
    if [ $? -ne 0 ]; then
        echo "rkipc exit"
        break
    else
        sleep 1
        echo "rkipc active"
    fi
done

test_case()
{
    if [ $test_mode == 0 ]; then
        test_cmd sample_ai_aenc_adec_ao_stresstest -r 8000 -t g711a -v 1
    elif [ $test_mode == 1 ]; then
        test_cmd sample_ai_aenc_adec_ao_stresstest -r 16000 -t g711a -v 1
    elif [ $test_mode == 2 ]; then
        test_cmd sample_ai_aenc_adec_ao_stresstest -r 8000 -t g711u -v 1
    elif [ $test_mode == 3 ]; then
        test_cmd sample_ai_aenc_adec_ao_stresstest -r 16000 -t g711u -v 1
    elif [ $test_mode == 4 ]; then
        test_cmd sample_ai_aenc_adec_ao_stresstest -r 8000 -t g726 -v 1
    elif [ $test_mode == 5 ]; then
        test_cmd sample_ai_aenc_adec_ao_stresstest -r 16000 -t g711a -v 1 -s 1 -l $test_loop
    else
        __echo_test_cmd_msg "---------------------------------------test_mode($test_mode) is invalid parameter, parameter range is [0,5]-------------------------------------------\n"
    fi
}

test_case
echo "sample_ai_aenc_adec_ao_stresstest is:"
cat /tmp/sample_ai_aenc_adec_ao_stresstest.log

exit 0
