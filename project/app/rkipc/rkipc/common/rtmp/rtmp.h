// Copyright 2020-2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RTMP_DEMO_H__
#define __RTMP_DEMO_H__

#ifdef __cplusplus
extern "C" {
#endif

int rk_rtmp_init(int id, const char *rtmp_url);
int rk_rtmp_deinit(int id);
int rk_rtmp_write_video_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                              int64_t present_time, int key_frame);
int rk_rtmp_write_audio_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                              int64_t present_time);

#ifdef __cplusplus
}
#endif
#endif