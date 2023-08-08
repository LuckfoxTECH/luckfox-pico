// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "rkmuxer.h"
#include <sys/time.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "rtmp.c"

static int g_rtmp_enable[3] = {0, 0, 0};
static VideoParam g_video_param;
static pthread_mutex_t g_rtmp_mutex = PTHREAD_MUTEX_INITIALIZER;
// static AudioParam g_audio_param;

int rk_rtmp_init(int id, const char *rtmp_url) {
	int ret = 0;
	char entry[128] = {'\0'};
	LOG_DEBUG("begin\n");
	system("ifconfig lo up");

	// set g_video_param
	memset(&g_video_param, 0, sizeof(g_video_param));
	g_video_param.level = 52;
	snprintf(entry, 127, "video.%d:width", id);
	g_video_param.width = rk_param_get_int(entry, 1920);
	snprintf(entry, 127, "video.%d:height", id);
	g_video_param.height = rk_param_get_int(entry, 1080);
	snprintf(entry, 127, "video.%d:max_rate", id);
	g_video_param.bit_rate = rk_param_get_int(entry, 1024) * 1024;
	snprintf(entry, 127, "video.%d:dst_frame_rate_den", id);
	g_video_param.frame_rate_den = rk_param_get_int(entry, 1);
	snprintf(entry, 127, "video.%d:dst_frame_rate_num", id);
	g_video_param.frame_rate_num = rk_param_get_int(entry, 30);
	snprintf(entry, 127, "video.%d:output_data_type", id);
	const char *output_data_type = rk_param_get_string(entry, "H.264");
	if (output_data_type)
		memcpy(g_video_param.codec, output_data_type, strlen(output_data_type));
	snprintf(entry, 127, "video.%d:h264_profile", id);
	const char *h264_profile = rk_param_get_string(entry, "high");
	if (!strcmp(h264_profile, "high"))
		g_video_param.profile = 100;
	else if (!strcmp(h264_profile, "main"))
		g_video_param.profile = 77;
	else if (!strcmp(h264_profile, "baseline"))
		g_video_param.profile = 66;
	memcpy(g_video_param.format, "NV12", strlen("NV12"));
	// set g_audio_param
	// g_audio_param.channels = rk_param_get_int("audio.0:channels", 2);
	// g_audio_param.sample_rate = rk_param_get_int("audio.0:sample_rate", 16000);
	// g_audio_param.frame_size = rk_param_get_int("audio.0:frame_size", 1024);
	// const char *format = rk_param_get_string("audio.0:format", NULL);
	// if (format)
	// 	memcpy(g_audio_param.format, format, strlen(format));
	// const char *codec = rk_param_get_string("audio.0:encode_type", NULL);
	// if (codec)
	// 	memcpy(g_audio_param.codec, codec, strlen(codec));
	pthread_mutex_lock(&g_rtmp_mutex);
	rkmuxer_init(id + 3, "flv", rtmp_url, &g_video_param, NULL);
	g_rtmp_enable[id] = 1;
	pthread_mutex_unlock(&g_rtmp_mutex);

	return ret;
}

int rk_rtmp_deinit(int id) {
	LOG_DEBUG("begin\n");
	pthread_mutex_lock(&g_rtmp_mutex);
	g_rtmp_enable[id] = 0;
	rkmuxer_deinit(id + 3);
	pthread_mutex_unlock(&g_rtmp_mutex);
	LOG_DEBUG("end\n");

	return 0;
}

int rk_rtmp_write_video_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                              int64_t present_time, int key_frame) {
	pthread_mutex_lock(&g_rtmp_mutex);
	if (g_rtmp_enable[id])
		rkmuxer_write_video_frame(id + 3, buffer, buffer_size, present_time, key_frame);
	pthread_mutex_unlock(&g_rtmp_mutex);

	return 0;
}

int rk_rtmp_write_audio_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                              int64_t present_time) {
	if (g_rtmp_enable[id]) {
		pthread_mutex_lock(&g_rtmp_mutex);
		rkmuxer_write_audio_frame(id + 3, buffer, buffer_size, present_time);
		pthread_mutex_unlock(&g_rtmp_mutex);
	}
	return 0;
}
