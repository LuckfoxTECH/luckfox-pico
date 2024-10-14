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
    echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <ordinary_stream_test_framecount> <sensor_width> <sensor_height>"
    echo "example: PN_MODE=on HDR=off FRAMERATE=on AIISP=on VPSS_CHN0_RESOLUTION=on VPSS_VENC_CHN0_RESOLUTION=on ENCODE_TYPE=on RGN_INIT_DEINIT=on RESTART=on ./demo_dual_aiisp_stresstest.sh /tmp/test.log 100 10 450000 1920 1080"
    echo "mod: 0.ORDINARY 1.PN_MODE 2.HDR 3.FRAMERATE 4.AIISP 5.VPSS_CHN0_RESOLUTION 6.VPSS_VENC_CHN0_RESOLUTION 7.ENCODE_TYPE 8.RGN_INIT_DEINIT 9.RESTART"
    echo -e "
          \$1 --------test_result_path: /tmp/xxxx.log (require argument)\n
          \$2 --------test_loop: 10000 (require argument)\n
          \$3 --------test_frame: 10 (require argument)\n
          \$4 --------ordinary_stream_test_framecount: 450000\n
          \$5 --------sensor_width: 1920(require argument\n
          \$6 --------sensor_height: 1080(require argument)\n"

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

#set ordinary_stream_test_framecount
ordinary_stream_test_framecount=$4
if [ ! -n "$4" ]; then
    echo "----------------- error !!!!, lack ordinary_stream_test_framecount setting, please input setting"
fi

#set sensor width
sensor_width=$5
if [ ! -n "$5" ]; then
    echo "----------------- error!!!!, lack sensor width, please input sensor width"
    print_help
    exit 1
fi

#set sensor height
sensor_height=$6
if [ ! -n "$6" ]; then
    echo "----------------- error!!!!, lack sensor height, please input sensor height"
    print_help
    exit 1
fi

test_case()
{

    if [ "$PN_MODE" = "on" ]; then
        #isp p/n mode switch
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/ -e h264cbr -b 4096 -i /userdata/160x96.bmp -I /userdata/192x96.bmp --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 1
    fi

    if [ "$HDR" = "on" ]; then
        #isp hdr mode switch test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/  -i /userdata/160x96.bmp -I /userdata/192x96.bmp  --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 2 
    fi

    if [ "$FRAMERATE" = "on" ]; then
        #isp framerate switch test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/  -i /userdata/160x96.bmp -I /userdata/192x96.bmp  --test_frame_count $frame_count --mode_test_loop  $test_loop --mode_test_type 3 
    fi

    if [ "$AIISP" = "on" ]; then
        #aiisp mode switch test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/  -i /userdata/160x96.bmp -I /userdata/192x96.bmp  --test_frame_count $frame_count --mode_test_loop  $test_loop --mode_test_type 4
    fi


    if [ "$VPSS_CHN0_RESOLUTION" = "on" ]; then
        #venc_chn0_resolution switch test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/ -i /userdata/160x96.bmp -I /userdata/192x96.bmp  --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 5 
    fi

    if [ "$VPSS_VENC_CHN0_RESOLUTION" = "on" ]; then
        #vpss_venc_chn0_resolution switch test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/  -i /userdata/160x96.bmp -I /userdata/192x96.bmp --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 6 
    fi

	if [ "$ENCODE_TYPE" = "on" ]; then
		# encode type switch test
		test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/  -i /userdata/160x96.bmp -I /userdata/192x96.bmp  --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 7
	fi

    if [ "$RGN_INIT_DEINIT" = "on" ]; then
        #rgn init and deinit test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/  -i /userdata/160x96.bmp -I /userdata/192x96.bmp  --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 8 
    fi

    if [ "$RESTART" = "on" ]; then
        #media_deinit_init test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/ --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 9  
    fi

    if [ "$ORDINARY" = "on" ]; then
        #ordinary stream test
        test_cmd sample_demo_dual_aiisp_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/ -l $ordinary_stream_test_framecount -i /userdata/160x96.bmp -I /userdata/192x96.bmp  --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 0 
    fi

    sleep 1
    echo 3 > /proc/sys/vm/drop_caches
    cat /proc/meminfo | grep MemAvailable >> $test_result_path

}

test_case

exit 0
