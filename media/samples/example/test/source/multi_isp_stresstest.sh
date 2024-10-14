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
    echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <isp_group_mode> <vi_frame_switch_test_loop> <vi_chnid> <vi_buff_cnt> <vi_resolution>"
    echo "mod: 1.PN_MODE 2.HDR 3.FRAMERATE 4.LDCH 5.RESTART"
    echo -e "
          \$1 --------test_result_path: /tmp/stresstest.log\n
          \$2 --------test_loop: 10000\n
          \$3 --------test_frame: 10\n
          \$4 --------isp_group_mode: 0: no-group, 1: group\n
          \$5 --------vi_frame_switch_test_loop: 5000\n
          \$6 --------vi_chnid: 0\n
          \$7 --------vi_buff_cnt: 2\n
          \$8 --------vi_resolution: 1920x1080\n"

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

isp_group_mode=$4
if [ ! -n "$4" ]; then
    echo "----------------- error!!!!, lack isp group mode seting, please input isp_group_mode"
    print_help
    exit 1
fi

vi_framerate_switch_loop=$5
if [ ! -n "$5" ]; then
    echo "----------------- warning!!!!, lack vi_framerate_switch_loop"
    print_help
    exit 1
fi

#set vi channel id
vi_chnid=$6
if [ ! -n "$6" ]; then
    echo "----------------- error!!!!, lack vi_chnid, please input vi_chnid"
    print_help
    exit 1
fi

#set vi buff cnt
vi_buff_cnt=$7
if [ ! -n "$7" ]; then
    echo "----------------- error!!!!, lack vi_buff_cnt, please input vi_buff_cnt"
    print_help
    exit 1
fi

#set vi_resolution
vi_resolution=$8
if [ ! -n "$8" ]; then
    echo "----------------- error!!!!, lack vi_resolution, please input vi_resolution"
    print_help
    exit 1
fi

test_case()
{
    if [ "$PN_MODE" = "on" ]; then
        #1: PN mode switch
        test_cmd sample_mulit_isp_stresstest -a /etc/iqfiles/ -c 2 --vi_size $vi_resolution --vi_chnid $vi_chnid --vi_buffcnt $vi_buff_cnt --modeTestType 1 --modeTestLoop $test_loop --testFrameCount $frame_count --ispLaunchMode $isp_group_mode
    fi

    if [ "$HDR" = "on" ]; then
        #2: hdr mode switch test
        test_cmd sample_mulit_isp_stresstest -a /etc/iqfiles/ -c 2 --vi_size $vi_resolution --vi_chnid $vi_chnid --vi_buffcnt $vi_buff_cnt --modeTestType 2 --modeTestLoop $test_loop --testFrameCount $frame_count --ispLaunchMode $isp_group_mode
    fi

    if [ "$FRAMERATE" = "on" ]; then
        #3: frameRate switch test
        test_cmd sample_mulit_isp_stresstest -a /etc/iqfiles/ -c 2 --vi_size $vi_resolution --vi_chnid $vi_chnid --vi_buffcnt $vi_buff_cnt --modeTestType 3 --modeTestLoop $vi_framerate_switch_loop --testFrameCount $frame_count --ispLaunchMode $isp_group_mode
    fi

    if [ "$LDCH" = "on" ]; then
        #4: LDCH mode test
        test_cmd sample_mulit_isp_stresstest -a /etc/iqfiles/ -c 2 --vi_size $vi_resolution --vi_chnid $vi_chnid --vi_buffcnt $vi_buff_cnt --modeTestType 4 --modeTestLoop $test_loop --testFrameCount $frame_count --ispLaunchMode $isp_group_mode
    fi

    if [ "$RESTART" = "on" ]; then
        #4: isp_deinit_init test
        test_cmd sample_mulit_isp_stresstest -a /etc/iqfiles/ -c 2 --vi_size $vi_resolution --vi_chnid $vi_chnid --vi_buffcnt $vi_buff_cnt --modeTestType 5 --modeTestLoop $test_loop --testFrameCount $frame_count --ispLaunchMode $isp_group_mode
    fi

    sleep 3
    echo 3 > /proc/sys/vm/drop_caches
    cat /proc/meminfo | grep MemAvailable >> $test_result_path

}

test_case

exit 0

