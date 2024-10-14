#!/bin/sh

if [ $# -gt 0 ];then
	echo "param $@"
	if [ $1 -eq 2 ];then
	  echo "pp oneframe"
	# link ispp input image, so pp work as one frame mode
	media-ctl -d /dev/media1 -l '"rkispp_input_image":0->"rkispp-subdev":0[1]'
	else
	  echo "pp direct"
	media-ctl -d /dev/media1 -l '"rkispp_input_image":0->"rkispp-subdev":0[0]'
        fi

	# enable ispp scale0
	media-ctl -d /dev/media1 -l '"rkispp-subdev":2->"rkispp_scale0":0[1]'
	# disable ispp sharp
	media-ctl -d /dev/media1 -l '"rkispp-subdev":2->"rkispp_m_bypass":0[0]'
	# set ispp input image video format 
	v4l2-ctl -d /dev/video13 --set-fmt-video-out=width=2688,height=1520,pixelformat=NV12
	# set isp mp video format 
	v4l2-ctl -d /dev/video0 --set-fmt-video=width=2688,height=1520,pixelformat=NV12
else
	media-ctl -d /dev/media1 -l '"rkispp-subdev":2->"rkispp_scale0":0[0]'
	media-ctl -d /dev/media1 -l '"rkispp-subdev":2->"rkispp_m_bypass":0[1]'
fi
