// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "rtsp_demo.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "rtsp.c"

pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
rtsp_demo_handle g_rtsplive = NULL;
rtsp_session_handle g_rtsp_session_0 = NULL;
rtsp_session_handle g_rtsp_session_1 = NULL;
rtsp_session_handle g_rtsp_session_2 = NULL;

int rkipc_rtsp_init(const char *rtsp_url_0, const char *rtsp_url_1, const char *rtsp_url_2) {
	const char *tmp_output_data_type = "H.264";

	LOG_DEBUG("start\n");
	pthread_mutex_lock(&g_rtsp_mutex);
	g_rtsplive = create_rtsp_demo(554);
	if (rtsp_url_0) {
		g_rtsp_session_0 = rtsp_new_session(g_rtsplive, rtsp_url_0);
		tmp_output_data_type = rk_param_get_string("video.0:output_data_type", "H.264");
		if (!strcmp(tmp_output_data_type, "H.264"))
			rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		else if (!strcmp(tmp_output_data_type, "H.265"))
			rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		else
			LOG_DEBUG("0 tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
		rtsp_set_audio(g_rtsp_session_0, RTSP_CODEC_ID_AUDIO_G711A, NULL, 0);
		rtsp_sync_audio_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
		rtsp_set_audio_sample_rate(g_rtsp_session_0,
		                           rk_param_get_int("audio.0:sample_rate", 16000));
		rtsp_set_audio_channels(g_rtsp_session_0, rk_param_get_int("audio.0:channels", 2));
	}
	if (rtsp_url_1) {
		g_rtsp_session_1 = rtsp_new_session(g_rtsplive, rtsp_url_1);
		tmp_output_data_type = rk_param_get_string("video.1:output_data_type", "H.264");
		if (!strcmp(tmp_output_data_type, "H.264"))
			rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		else if (!strcmp(tmp_output_data_type, "H.265"))
			rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		else
			LOG_DEBUG("1 tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		rtsp_sync_video_ts(g_rtsp_session_1, rtsp_get_reltime(), rtsp_get_ntptime());
		rtsp_set_audio(g_rtsp_session_1, RTSP_CODEC_ID_AUDIO_G711A, NULL, 0);
		rtsp_sync_audio_ts(g_rtsp_session_1, rtsp_get_reltime(), rtsp_get_ntptime());
		rtsp_set_audio_sample_rate(g_rtsp_session_1,
		                           rk_param_get_int("audio.0:sample_rate", 16000));
		rtsp_set_audio_channels(g_rtsp_session_1, rk_param_get_int("audio.0:channels", 2));
	}
	if (rtsp_url_2) {
		g_rtsp_session_2 = rtsp_new_session(g_rtsplive, rtsp_url_2);
		tmp_output_data_type = rk_param_get_string("video.2:output_data_type", "H.264");
		if (!strcmp(tmp_output_data_type, "H.264"))
			rtsp_set_video(g_rtsp_session_2, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		else if (!strcmp(tmp_output_data_type, "H.265"))
			rtsp_set_video(g_rtsp_session_2, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		else
			LOG_DEBUG("2 tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		rtsp_sync_video_ts(g_rtsp_session_2, rtsp_get_reltime(), rtsp_get_ntptime());
		rtsp_set_audio(g_rtsp_session_2, RTSP_CODEC_ID_AUDIO_G711A, NULL, 0);
		rtsp_sync_audio_ts(g_rtsp_session_2, rtsp_get_reltime(), rtsp_get_ntptime());
		rtsp_set_audio_sample_rate(g_rtsp_session_2,
		                           rk_param_get_int("audio.0:sample_rate", 16000));
		rtsp_set_audio_channels(g_rtsp_session_2, rk_param_get_int("audio.0:channels", 2));
	}

	pthread_mutex_unlock(&g_rtsp_mutex);
	LOG_DEBUG("end\n");

	return 0;
}

int rkipc_rtsp_deinit() {
	LOG_DEBUG("%s\n", __func__);
	pthread_mutex_lock(&g_rtsp_mutex);
	if (g_rtsp_session_0) {
		rtsp_del_session(g_rtsp_session_0);
		g_rtsp_session_0 = NULL;
	}
	if (g_rtsp_session_1) {
		rtsp_del_session(g_rtsp_session_1);
		g_rtsp_session_1 = NULL;
	}
	if (g_rtsp_session_2) {
		rtsp_del_session(g_rtsp_session_2);
		g_rtsp_session_2 = NULL;
	}
	if (g_rtsplive) {
		rtsp_del_demo(g_rtsplive);
		g_rtsplive = NULL;
	}
	pthread_mutex_unlock(&g_rtsp_mutex);

	return 0;
}

int rkipc_rtsp_write_video_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time) {
	pthread_mutex_lock(&g_rtsp_mutex);
	if (g_rtsplive == NULL) {
		pthread_mutex_unlock(&g_rtsp_mutex);
		return -1;
	}
	if ((id == 0) && g_rtsp_session_0)
		rtsp_tx_video(g_rtsp_session_0, buffer, buffer_size, present_time);
	if ((id == 1) && g_rtsp_session_1)
		rtsp_tx_video(g_rtsp_session_1, buffer, buffer_size, present_time);
	if ((id == 2) && g_rtsp_session_2)
		rtsp_tx_video(g_rtsp_session_2, buffer, buffer_size, present_time);
	rtsp_do_event(g_rtsplive);
	pthread_mutex_unlock(&g_rtsp_mutex);

	return 0;
}

int rkipc_rtsp_write_audio_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time) {
	pthread_mutex_lock(&g_rtsp_mutex);
	if (g_rtsplive == NULL) {
		pthread_mutex_unlock(&g_rtsp_mutex);
		return -1;
	}
	if (g_rtsp_session_0)
		rtsp_tx_audio(g_rtsp_session_0, buffer, buffer_size, present_time);
	if (g_rtsp_session_1)
		rtsp_tx_audio(g_rtsp_session_1, buffer, buffer_size, present_time);
	if (g_rtsp_session_2)
		rtsp_tx_audio(g_rtsp_session_2, buffer, buffer_size, present_time);
	rtsp_do_event(g_rtsplive);
	pthread_mutex_unlock(&g_rtsp_mutex);

	return 0;
}
