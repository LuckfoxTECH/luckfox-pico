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
    echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <ifEnableWrap> <ifEnableSmartP> <ordinary_stream_test_framecount> <vi_framerate_switch_loop> <sensor_width> <sensor_height>"
    echo "mod: PN_MODE HDR FRAMERATE LDCH RESOLUTION ENCODE_TYPE SMART_P SVC MOTION IDR ROTATION DETACH_ATTACH ORDINARY RESOLUTION_RV1126"
    echo -e "
          \$1 --------test_result_path: /tmp/xxxx.log (require argument)\n
          \$2 --------test_loop: 10000 (require argument)\n
          \$3 --------test_frame: 10 (require argument)\n
          \$4 --------ifEnableWrap: 0:close, 1:open (require argument)\n
          \$5 --------ifEnableSmartP: 0:close, 1:open\n
          \$6 --------ordinary_stream_test_framecount: 450000\n
          \$7 --------vi_framerate_switch_loop: 5000\n
          \$8 ---------sensor_width\n
          \$9 --------sensor_height\n"

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
    echo "----------------- error !!!!, lack ifOpenWrap setting, please input setting"
    print_help
    exit 1
fi

#set SmartP mode
smartP=$5
if [ ! -n "$5" ]; then
    echo "----------------- error !!!!, lack smartP setting, please input setting"
fi

#set ordinary_stream_test_framecount
ordinary_stream_test_framecount=$6
if [ ! -n "$6" ]; then
    echo "----------------- error !!!!, lack ordinary_stream_test_framecount setting, please input setting"
fi

#set vi_framerate_switch_loop
vi_framerate_switch_loop=$7
if [ ! -n "$7" ]; then
    echo "----------------- error !!!!, lack vi_framerate_switch_loop setting, please input setting"
fi

#for 1126 venc resolution switch
sensor_width=$8
sensor_height=$9

test_case()
{

    if [ "$PN_MODE" = "on" ]; then
        #isp p/n mode switch
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 1 --wrap $ifOpenWrap
    fi

    if [ "$HDR" = "on" ]; then
        #isp hdr mode switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 2 --wrap $ifOpenWrap
    fi

    if [ "$FRAMERATE" = "on" ]; then
        #isp framerate switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $vi_framerate_switch_loop --mode_test_type 3 --wrap $ifOpenWrap
    fi

    if [ "$LDCH" = "on" ]; then
        #isp LDCH switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 4 --wrap $ifOpenWrap
    fi

    if [ "$RESOLUTION" = "on" ]; then
        #venc resolution switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 5 --wrap $ifOpenWrap
    fi

    if [ "$ENCODE_TYPE" = "on" ]; then
        # encode type switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 6 --wrap $ifOpenWrap
    fi

    if [ "$SMART_P" = "on" ]; then
        #smartp mode switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 7 --wrap $ifOpenWrap
    fi

    if [ "$SVC" = "on" ]; then
        #SVC mode switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 8 --wrap $ifOpenWrap
    fi

    if [ "$MOTION" = "on" ]; then
        #motion deblur switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 9 --wrap $ifOpenWrap
    fi

    if [ "$IDR" = "on" ]; then
        #force IDR switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 10 --wrap $ifOpenWrap
    fi

    if [ "$ROTATION" = "on" ]; then
        #venc chn rotation switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 11 --wrap $ifOpenWrap
    fi

    if [ "$DETACH_ATTACH" = "on" ]; then
        #rgn detach/attach switch test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 12 --wrap $ifOpenWrap
    fi

    if [ "$ORDINARY" = "on" ]; then
        #ordinary stream test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ -l $ordinary_stream_test_framecount --inputBmp1Path /userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp --smartP $smartP --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 0 --wrap $ifOpenWrap
    fi

    if [ "$RESOLUTION_RV1126" = "on" ]; then
        #venc resolution switch for_RV1126 test
        test_cmd sample_demo_vi_venc_stresstest -w $sensor_width -h $sensor_height -a /etc/iqfiles/ --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 13 --vi_chnid 0
    fi

    if [ "$RESTART" = "on" ]; then
        #media_deinit_init test
        test_cmd sample_demo_vi_venc_stresstest -w 1920 -h 1080 -a /etc/iqfiles/ --test_frame_count $frame_count --mode_test_loop $test_loop --mode_test_type 14 --wrap $ifOpenWrap --smartP $smartP
    fi

    sleep 1
    echo 3 > /proc/sys/vm/drop_caches
    cat /proc/meminfo | grep MemAvailable >> $test_result_path

}

test_case

exit 0
