/*
 * Copyright (c) 2025 NeuralSense AI Private Limited
 * Trading as swatah.ai. All rights reserved.
 *
 * This file is part of the swatah.ai software stack and is licensed under
 * the terms defined in the accompanying LICENSE file. Unauthorized copying,
 * distribution, or modification of this file, via any medium, is strictly prohibited.
 *
 * For more information, visit: https://swatah.ai
*/

#ifndef _SAIX_COMMON_DEFINES_H_
#define _SAIX_COMMON_DEFINES_H_

#include <stdint.h>

#define RKISP1_DEV_NAME "/dev/rkisp1"
#define RKISP2_DEV_NAME "/dev/rkisp2"
#define RKISP3_DEV_NAME "/dev/rkisp3"
#define RKISP4_DEV_NAME "/dev/rkisp4"
#define RKISP_MAINPATH 0
#define RKISP_SELFPATH 1
#define RKISP_FBCPATH 2
#define VIDEO_PIPE_0 0
#define VIDEO_PIPE_1 1
#define VIDEO_PIPE_2 2
#define VIDEO_PIPE_3 3
#define JPEG_VENC_CHN 4
#define DRAW_NN_VENC_CHN_ID 0
#define VPSS_ROTATE 6
#define VPSS_BGR 0
#define DRAW_NN_OSD_ID 7
#define RED_COLOR 0x0000FF
#define BLUE_COLOR 0xFF0000

#define RK3588_VO_DEV_HDMI 0
#define RK3588_VO_DEV_MIPI 3
#define RK3588_VOP_LAYER_CLUSTER0 0

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"
#define RTSP_URL_2 "/live/2"
#define RTMP_URL_0 "rtmp://127.0.0.1:1935/live/mainstream"
#define RTMP_URL_1 "rtmp://127.0.0.1:1935/live/substream"
#define RTMP_URL_2 "rtmp://127.0.0.1:1935/live/thirdstream"

#endif /* _SAIX_COMMON_DEFINES_H_ */