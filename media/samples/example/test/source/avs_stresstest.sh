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
    echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <vi_chnid> <vi_buff_cnt> <vi_resolution> <avs_resolution>"
    echo "mod: 1.RESTART 2.RESOLUTION"
    echo -e "
          \$1 --------test_result_path: /tmp/stresstest.log\n
          \$2 --------test_loop: 10000\n
          \$3 --------test_frame: 10\n
          \$4 --------vi_chnid: 0\n
          \$5 --------vi_buff_cnt: 2\n
          \$6 --------vi_resolution: 1920x1080\n
          \$7 --------avs_resolution: 3840x1080\n"

}

test_result_path=$1
if [ "$1" = "help" ]; then
    print_help
    exit 1
elif [ ! -n "$1" ]; then
    echo "----------------- error!!! lack test_result_path, please input test_result_path"
    print_help
    exit 1
else
    echo " the test_result_path your input is: $1"
fi

#set test loop
test_loop=$2
if [ ! -n "$2" ]; then
    echo "----------------- error!!!, lack test_loop, please input test loop"
    print_help
    exit 1
fi

#set frame count for every loop
frame_count=$3
if [ ! -n "$3" ]; then
    echo "----------------- error!!!!, lack frame_count, please input test frame"
    print_help
    exit 1
fi

#set vi channel id
vi_chnid=$4
if [ ! -n "$4" ]; then
    echo "----------------- error!!!!, lack vi_chnid, please input vi_chnid"
    print_help
    exit 1
fi

#set vi buff cnt
vi_buff_cnt=$5
if [ ! -n "$5" ]; then
    echo "----------------- error!!!!, lack vi_buff_cnt, please input vi_buff_cnt"
    print_help
    exit 1
fi

#set vi resulution
vi_resolution=$6
if [ ! -n "$6" ]; then
    echo "----------------- error!!!!, lack vi_resolution, please input vi_resolution"
    print_help
    exit 1
fi

#set avs resulution
avs_resolution=$7
if [ ! -n "$7" ]; then
    echo "----------------- error!!!!, lack avs_resolution, please input avs_resolution"
    print_help
    exit 1
fi

test_case()
{
    if [ "$RESTART" = "on" ]; then
        #1. avs_deinit_ubind_test
        test_cmd sample_avs_stresstest --vi_size $vi_resolution --avs_size $avs_resolution -a /etc/iqfiles/ --vi_chnid $vi_chnid --vi_buffcnt $vi_buff_cnt --mode_test_type 1 --mode_test_loop $test_loop --test_frame_count $frame_count
    fi

    if [ "$RESOLUTION" = "on" ]; then
        #2. avs_resolution_test
        test_cmd sample_avs_stresstest --vi_size $vi_resolution --avs_size $avs_resolution -a /etc/iqfiles/ --vi_chnid $vi_chnid --vi_buffcnt $vi_buff_cnt --mode_test_type 2 --mode_test_loop $test_loop --test_frame_count $frame_count
    fi

    sleep 3
    echo 3 > /proc/sys/vm/drop_caches
    cat /proc/meminfo | grep MemAvailable >> $test_result_path

}

test_case

exit 0
