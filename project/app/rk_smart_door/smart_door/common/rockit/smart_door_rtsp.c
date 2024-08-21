// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"
#include "rtsp_demo.h"
#include <signal.h>

#define RTSP_URL_0 "/live/0"
rtsp_demo_handle g_rtsplive = NULL;
rtsp_session_handle g_rtsp_session_0;
pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;

void handle_pipe(int sig) { RK_LOGI("%s sig = %d\n", __func__, sig); }

static void smart_door_rtsp_get_sigpipe(void) {
	struct sigaction action;

	action.sa_handler = handle_pipe;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGPIPE, &action, NULL);
}

void smart_door_rtsp_init() {
    RK_LOGI("%s\n", __func__);

    smart_door_rtsp_get_sigpipe();

    g_rtsplive = create_rtsp_demo(554);

    g_rtsp_session_0 = rtsp_new_session(g_rtsplive, RTSP_URL_0);

    rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);

    rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());

    return;
}

void smart_door_rtsp_deinit() {
    RK_LOGI("%s\n", __func__);

    if (g_rtsp_session_0) {
        rtsp_del_session(g_rtsp_session_0);
        g_rtsp_session_0 = NULL;
    }

    if (g_rtsplive) {
        rtsp_del_demo(g_rtsplive);
        g_rtsplive = NULL;
    }

    return;
}

void smart_door_rtsp_send_live0(void *data, RK_U32 u32Len, RK_U64 u64PTS) {
    if (g_rtsplive && g_rtsp_session_0) {
        pthread_mutex_lock(&g_rtsp_mutex);
        rtsp_tx_video(g_rtsp_session_0, data, u32Len, u64PTS);
        rtsp_do_event(g_rtsplive);
        pthread_mutex_unlock(&g_rtsp_mutex);
    }
}
