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
    echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <ifEnableWrap>"
    echo "mod: DETACH_ATTACH"
    echo -e "
          \$1 --------test_result_path: /tmp/xxx.log\n
          \$2 --------test_loop: 10000\n
          \$3 --------test_frame: 10\n
          \$4 --------ifEnableWrap: 0:close, 1:open\n"
}

test_result_path=$1
if [ "$1" = "help" ]; then
    print_help
    exit 1
elif [ ! -n "$1" ]; then
    echo "------ error!!! lack test_result_path, please input test_result_path"
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

#set wrap mode
ifOpenWrap=$4
if [ ! -n "$4" ]; then
    echo "----------------- warning!!!!, lack ifOpenWrap setting, please input setting"
    print_help
    exit 1
fi

test_case()
{
    if [ "$DETACH_ATTACH" = "on" ]; then
        #rgn detach attach test
        test_cmd sample_rgn_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 1 --wrap $ifOpenWrap
    fi

    sleep 3
    echo 3 > /proc/sys/vm/drop_caches
    cat /proc/meminfo | grep MemAvailable >> $test_result_path

}

test_case

exit 0
