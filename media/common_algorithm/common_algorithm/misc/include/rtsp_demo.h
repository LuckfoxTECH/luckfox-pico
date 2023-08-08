// Copyright 2020-2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RTSP_DEMO_H__
#define __RTSP_DEMO_H__
/*
 * a simple RTSP server demo
 * RTP over UDP/TCP H264/G711a
 * */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum rtsp_codec_id {
	RTSP_CODEC_ID_NONE = 0,
	RTSP_CODEC_ID_VIDEO_H264 = 0x0001,  /*codec_data is SPS + PPS frames*/
	RTSP_CODEC_ID_VIDEO_H265,           /*codec_data is VPS + SPS + PPS frames*/
	RTSP_CODEC_ID_VIDEO_MPEG4,          /*now not support*/
	RTSP_CODEC_ID_AUDIO_G711A = 0x4001, /*codec_data is NULL*/
	RTSP_CODEC_ID_AUDIO_G711U,          /*codec_data is NULL*/
	RTSP_CODEC_ID_AUDIO_G726,           /*codec_data is bitrate (int)*/
	RTSP_CODEC_ID_AUDIO_MP3,            /*codec_data is audio specific config (2bytes). frame
	                                   type is MP3*/
};

typedef void *rtsp_demo_handle;
typedef void *rtsp_session_handle;

rtsp_demo_handle rtsp_new_demo(int port);
rtsp_demo_handle create_rtsp_demo(int port);
int rtsp_do_event(rtsp_demo_handle demo);
rtsp_session_handle rtsp_new_session(rtsp_demo_handle demo, const char *path);
rtsp_session_handle create_rtsp_session(rtsp_demo_handle demo, const char *path);
int rtsp_set_video(rtsp_session_handle session, int codec_id, const uint8_t *codec_data,
                   int data_len);

int rtsp_set_audio(rtsp_session_handle session, int codec_id, const uint8_t *codec_data,
                   int data_len);
int rtsp_set_audio_sample_rate (rtsp_session_handle session,int sample_rate);
int rtsp_set_audio_channels (rtsp_session_handle session,int channels);

int rtsp_sever_tx_video(rtsp_demo_handle demo, rtsp_session_handle session, const uint8_t *frame,
                        int len, uint64_t ts);
int rtsp_tx_video(rtsp_session_handle session, const uint8_t *frame, int len, uint64_t ts);
int rtsp_tx_audio(rtsp_session_handle session, const uint8_t *frame, int len, uint64_t ts);
void rtsp_del_session(rtsp_session_handle session);
void rtsp_del_demo(rtsp_demo_handle demo);

uint64_t rtsp_get_reltime(void);
uint64_t rtsp_get_ntptime(void);
int rtsp_sync_video_ts(rtsp_session_handle session, uint64_t ts, uint64_t ntptime);
int rtsp_sync_audio_ts(rtsp_session_handle session, uint64_t ts, uint64_t ntptime);

#ifdef __cplusplus
}
#endif
#endif