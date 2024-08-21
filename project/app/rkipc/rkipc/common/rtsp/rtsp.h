// Copyright 2020-2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RTSP_DEMO_H__
#define __RTSP_DEMO_H__

#ifdef __cplusplus
extern "C" {
#endif

int rkipc_rtsp_init(const char *rtsp_url_0, const char *rtsp_url_1, const char *rtsp_url_2);
int rkipc_rtsp_deinit();
int rkipc_rtsp_write_video_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time);
int rkipc_rtsp_write_audio_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time);

#ifdef __cplusplus
}
#endif
#endif