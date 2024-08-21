#!/bin/sh

set -x
#test loop
test_loop=10000

#test frame
test_frame=10

#test result path
test_result_path=/tmp/rv1126_multi_cam_test_result.log

#vi framerate switch loop
vi_framerate_switch_loop=2000

#ordinary_stream_test_framecount
ordinary_stream_test_framecount=450000

#vi channel id seting
vi_chnid=0

#vi buff cnt
vi_buff_cnt=5

#vi_resolution
vi_resolution=2560x1520

#avs_chn0_resolution
avs_chn0_resolution=4096x1520

#avs_chn1_resolution
avs_chn1_resolution=704x576

#if enable HDR test
if [[ ! -n "$1" ]] || [[ "$1" = "0" ]]; then
    ifEnableHDR=off
else
    ifEnableHDR=on
fi

# get script path
script_path=$(dirname $0)

multi_isp_test()
{
    # echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <isp_group_mode> <vi_frame_switch_test_loop> <vi_chnid> <vi_buff_cnt> <vi_resolution>"
    # echo "mod: 1.PN_MODE 2.HDR 3.FRAMERATE 4.LDCH 5.RESTART"
    # echo -e "
    #       \$1 --------test_result_path: /tmp/stresstest.log\n
    #       \$2 --------test_loop: 10000\n
    #       \$3 --------test_frame: 10\n
    #       \$4 --------isp_group_mode: 0: no-group, 1: group\n
    #       \$5 --------vi_frame_switch_test_loop: 5000\n
    #       \$6 --------vi_chnid: 0\n
    #       \$7 --------vi_buff_cnt: 2\n
    #       \$8 --------vi_resolution: 1920x1080\n"

    echo "--------------------enter multi_isp_test-------------------" >> $test_result_path

    isp_group_mode=0
    echo "--------------------isp no-group mode-------------------" >> $test_result_path
    
    eval $1 $script_path/multi_isp_stresstest.sh $test_result_path $test_loop $test_frame $isp_group_mode $vi_framerate_switch_loop $vi_chnid $vi_buff_cnt $vi_resolution

    if [ $? != 0 ]; then
        echo "$1 $script_path/multi_isp_stresstest.sh $test_result_path $test_loop $test_frame $isp_group_mode $vi_framerate_switch_loop $vi_chnid $vi_buff_cnt $vi_resolution   -----test failure"
        exit 1
    fi

    isp_group_mode=1
    echo "--------------------isp group mode-------------------" >> $test_result_path
    
    eval $1 $script_path/multi_isp_stresstest.sh $test_result_path $test_loop $test_frame $isp_group_mode $vi_framerate_switch_loop $vi_chnid $vi_buff_cnt $vi_resolution

    if [ $? != 0 ]; then
        echo "$1 $script_path/multi_isp_stresstest.sh $test_result_path $test_loop $test_frame $isp_group_mode $vi_framerate_switch_loop $vi_chnid $vi_buff_cnt $vi_resolution -----test failure"
        exit 1
    fi

    echo "--------------------exit multi_isp_test-------------------" >> $test_result_path

}

avs_stresstest()
{
    # echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <vi_chnid> <vi_buff_cnt> <vi_resolution> <avs_resolution>"
    # echo "mod: 1.RESTART 2.RESOLUTION"
    # echo -e "
    #       \$1 --------test_result_path: /tmp/stresstest.log\n
    #       \$2 --------test_loop: 10000\n
    #       \$3 --------test_frame: 10\n
    #       \$4 --------vi_chnid: 0\n
    #       \$5 --------vi_buff_cnt: 2\n
    #       \$6 --------vi_resolution: 1920x1080\n
    #       \$7 --------avs_resolution: 3840x1080\n"
    
    echo "--------------------enter avs_stresstest-------------------" >> $test_result_path
    
    eval $1 $script_path/avs_stresstest.sh $test_result_path $test_loop $test_frame $vi_chnid $vi_buff_cnt $vi_resolution $avs_chn0_resolution

    if [ $? != 0 ]; then
        echo "$1 $script_path/avs_stresstest.sh $test_result_path $test_loop $test_frame $vi_chnid $vi_buff_cnt $vi_resolution $avs_chn0_resolution  -----test failure"
        exit 1
    fi

    echo "--------------------exit avs_stresstest-------------------" >> $test_result_path

}

demo_vi_avs_venc_stresstest()
{
    # echo "example: <test_mod=on> $0 <test_result_path> <test_loop> <test_frame> <vi_chnid> <ordinary_stream_test_framecount> <vi_buff_cnt> <vi_resolution> <avs_chn0_resolution> <avs_chn1_resolution>"
    # echo "mod: 1.RESTART 2.RESOLUTION"
    # echo -e "
    #       \$1 --------test_result_path: /tmp/stresstest.log\n
    #       \$2 --------test_loop: 10000\n
    #       \$3 --------test_frame: 10\n
    #       \$4 --------vi_chnid: 1\n
    #       \$5 --------<ordinary_stream_test_framecount>: 450000\n
    #       \$6 --------vi_buff_cnt: 2\n
    #       \$7 --------vi_resolution: 1920x1080\n
    #       \$8 --------avs_chn0_resolution: 3840x1080\n
    #       \$9 --------avs_chn1_resolution: 1920x544\n"
    
    echo "--------------------enter demo_vi_avs_venc_stresstest-------------------" >> $test_result_path
    
    eval $1 $script_path/demo_vi_avs_venc_stresstest.sh $test_result_path $test_loop $test_frame $vi_chnid $ordinary_stream_test_framecount $vi_buff_cnt $vi_resolution $avs_chn0_resolution $avs_chn1_resolution

    if [ $? != 0 ]; then
        echo "$1 $script_path/demo_vi_avs_venc_stresstest.sh $test_result_path $test_loop $test_frame $vi_chnid $ordinary_stream_test_framecount $vi_buff_cnt $vi_resolution $avs_chn0_resolution $avs_chn1_resolution -----test failure"
        exit 1
    fi

    echo "--------------------exit demo_vi_avs_venc_stresstest-------------------" >> $test_result_path

}

killall rkipc
while true
do
    ps|grep rkipc |grep -v grep
    if [ $? -ne 0 ]; then
        echo "rkipc exit"
        break
    else
        echo "rkipc active"
    fi
done

echo "start stresstest" > $test_result_path
echo "start record meminfo" > /tmp/testLog.txt

#1.multi isp stresstest
test_mod="PN_MODE=on HDR=$ifEnableHDR FRAMERATE=on LDCH=on RESTART=on"
multi_isp_test "$test_mod"

#2.avs stresstest
test_mod="RESTART=on RESOLUTION=on"
avs_stresstest "$test_mod"

#3.demo vi avs venc stresstests
test_mod="RESTART=on RESOLUTION=on ORDINARY=on"
demo_vi_avs_venc_stresstest "$test_mod"

#print test result
cat $test_result_path
cat /tmp/testLog.txt
