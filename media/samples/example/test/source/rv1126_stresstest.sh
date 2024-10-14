#!/bin/sh

set -x
#test loop
test_loop=10000

#test frame
test_frame=10

#test result path
test_result_path=/tmp/rv1126_test_result.log

#vi framerate switch loop
vi_framerate_switch_loop=5000

#rv1103 set wrap
ifEnableWrap=0

# samartP mode
ifEnableSmartP=0

#ordinary_stream_test_framecount
ordinary_stream_test_framecount=450000

# get script path
script_file_path=$(dirname $0)

if [ "$1" = "os04a10" ]; then
    echo "-----------Now your sensor model: os04a10"
    sensor_width=2688
    sensor_height=1520
    iqfilePath=/etc/iqfiles/os04a10_CMK-OT1607-FV1_M12-40IRC-4MP-F16.xml
    echo "sensor_width: $sensor_width, sensor_height: $sensor_height, iqfilePath: $iqfilePath"
elif [ "$1" = "imx335" ]; then
    echo "-----------Now your sensor model: imx335"
    sensor_width=2592
    sensor_height=1944
    iqfilePath=/etc/iqfiles/imx335_MTV4-IR-E-P_40IRC-4MP-F16.xml
    echo "sensor_width: $sensor_width, sensor_height: $sensor_height, iqfilePath: $iqfilePath"
elif [ "$1" = "imx415" ]; then
    echo "-----------Now your sensor model: imx415"
    sensor_width=3840
    sensor_height=2160
    iqfilePath=/etc/iqfiles/imx415_YT10092_IR0147-36IRC-8M-F20.xml
    echo "sensor_width: $sensor_width, sensor_height: $sensor_height, iqfilePath: $iqfilePath"
else
    echo "------- error, must input sensor model: os04a10, imx335, imx415 ----"
    echo "example: $0 os04a10"
    exit 0
fi

isp_stresstest()
{
    # ./script test_type if_open test_loop test_frame iq_file
    # $1 --------test_result_path
    # $2 --------test_loop
    # $3 --------test_frame
    # $4 --------vi_frame_switch_test_loop
    # $5 --------iq_file

    echo "-----------------enter isp stresstest-----------------" >> $test_result_path

    eval $1 $script_file_path/isp_stresstest.sh $test_result_path $test_loop $test_frame $vi_framerate_switch_loop $iqfilePath
    if [ $? != 0 ]; then
        echo "$1 $script_file_path/isp_stresstest.sh $test_result_path $test_loop $test_frame $vi_framerate_switch_loop $iqfilePath   -----test failure"
        exit 1
    fi

    echo "-----------------exit isp stresstest-----------------" >> $test_result_path
}

venc_stresstest()
{
    #example: $0 <test_result_path> <test_loop> <test_frame> <ifEnableWrap>"
    #      $1 --------test_result_path
    #      $2 --------test_loop
    #      $3 --------test_frame
    #      $4 --------ifEnableWrap

    echo "-----------------enter venc stresstest-----------------" >> $test_result_path

    eval $1 $script_file_path/venc_stresstest.sh $test_result_path $test_loop $test_frame $ifEnableWrap
    if [ $? != 0 ]; then
        echo "$1 $script_file_path/venc_stresstest.sh $test_result_path $test_loop $test_frame $ifEnableWrap ----------test failure"
        exit 1
    fi

    echo "-----------------exit venc stresstest-----------------" >> $test_result_path
}

rgn_stresstest()
{
    #example: $0 <test_type> <test_result_path> <test_loop> <test_frame> <ifEnableWrap>"
    #      $1 --------test_result_path
    #      $2 --------test_loop
    #      $3 --------test_frame
    #      $4 --------ifEnableWrap

    echo "-----------------enter rgn stresstest-----------------" >> $test_result_path

    eval $1 $script_file_path/rgn_stresstest.sh $test_result_path $test_loop $test_frame $ifEnableWrap
    if [ $? != 0 ]; then
        echo "$1 $script_file_path/rgn_stresstest.sh $test_result_path $test_loop $test_frame $ifEnableWrap ----------test failure"
        exit 1
    fi

    echo "-----------------exit rgn stresstest-----------------" >> $test_result_path
}

vpss_stresstest()
{
    # echo "example: <>$0 <test_result_path> <test_loop> <test_frame>"
    # test_mod: VPSS_RESTART RESOLUTION
    # echo -e "
    #       \$1 --------test_result_path\n
    #       \$2 --------test_loop\n
    #       \$3 --------test_frame\n"

    echo "-----------------enter vpss stresstest-----------------" >> $test_result_path

    eval $1 $script_file_path/vpss_stresstest.sh $test_result_path $test_loop $test_frame
    if [ $? != 0 ]; then
        echo "$1 $script_file_path/vpss_stresstest.sh $test_result_path $test_loop $test_frame----------test failure"
        exit 1
    fi

    echo "-----------------exit vpss stresstest-----------------" >> $test_result_path
}

demo_vi_venc_stresstest()
{
    # "example: $0 <test_result_path> <test_loop> <test_frame> <ifEnableWrap> <ifEnableSmartP> <ordinary_stream_test_framecount> <vi_framerate_switch_loop> <sensor_width> <sensor_height>"
    #   mod: PN_MODE HDR FRAMERATE LDCH RESOLUTION ENCODE_TYPE SMART_P SVC MOTION IDR ROTATION DETACH_ATTACH ORDINARY RESOLUTION_RV1126
    #   \$1 --------test_result_path: /tmp/xxxx.log\n
    #   \$2 --------test_loop: 10000\n
    #   \$3 --------test_frame: 10\n
    #   \$4 --------ifEnableWrap: 0:close, 1:open\n
    #   \$5 --------ifEnableSmartP: 0:close, 1:open\n
    #   \$6 --------ordinary_stream_test_framecount: 450000\n
    #   \$7 --------vi_framerate_switch_loop\n
    #   \$8 --------sensor_width\n
    #   \$9 --------sensor_height\n"

    echo "-----------------enter demo_vi_venc stresstest-----------------" >> $test_result_path

    eval $1 $script_file_path/demo_vi_venc_stresstest.sh $test_result_path $test_loop $test_frame $ifEnableWrap $ifEnableSmartP $ordinary_stream_test_framecount $vi_framerate_switch_loop $sensor_width $sensor_height
    if [ $? != 0 ]; then
        echo -e "$1 $script_file_path/demo_vi_venc_stresstest.sh $test_result_path $test_loop $test_frame $ifEnableWrap $ifEnableSmartP $ordinary_stream_test_framecount  $vi_framerate_switch_loop $sensor_width $sensor_height ----------test failure"
        exit 1
    fi

    echo "-----------------exit demo_vi_venc stresstest-----------------" >> $test_result_path
}

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

echo "start rv1126 stresstest" > $test_result_path
echo "start record rv1126 meminfo" > /tmp/testLog.txt

#1.isp stresstest
test_mod="PN_MODE=on HDR=on FRAMERATE=on LDCH=on IQFILE=on ISP_RESTART=on"
isp_stresstest "$test_mod"

#2.venc stresstest
test_mod="ENCODE_TYPE=on SMART_P=on IDR=on"
venc_stresstest "$test_mod"

#3.rgn stresstest
test_mod="DETACH_ATTACH=on"
rgn_stresstest "$test_mod"

#4.vpss stresstest
test_mod="RESTART=on RESOLUTION=on"
vpss_stresstest "$test_mod"

#5.demo vi venc stresstest
test_mod="PN_MODE=on HDR=on FRAMERATE=on LDCH=on ENCODE_TYPE=on SMART_P=on IDR=on DETACH_ATTACH=on ORDINARY=on RESOLUTION_RV1126=on RESTART=on"
demo_vi_venc_stresstest "$test_mod"

#print test result
cat $test_result_path
cat /tmp/testLog.txt
