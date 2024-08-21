// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <inttypes.h> // PRId64
#include <rga/im2d.h>
#include <rga/rga.h>
#include <rk_comm_ivs.h>
#include <rk_debug.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_mmz.h>
#include <rk_mpi_rgn.h>
#include <rk_mpi_sys.h>
#include <rk_mpi_venc.h>
#include <rk_mpi_vi.h>
#include <rk_mpi_vo.h>
#include <rk_mpi_vpss.h>

int rk_video_init();
int rk_video_deinit();
int rk_video_restart();
int rk_video_get_gop(int stream_id, int *value);
int rk_video_set_gop(int stream_id, int value);
int rk_video_get_max_rate(int stream_id, int *value);
int rk_video_set_max_rate(int stream_id, int value);
int rk_video_get_RC_mode(int stream_id, const char **value);
int rk_video_set_RC_mode(int stream_id, const char *value);
int rk_video_get_output_data_type(int stream_id, const char **value);
int rk_video_set_output_data_type(int stream_id, const char *value);
int rk_video_get_rc_quality(int stream_id, const char **value);
int rk_video_set_rc_quality(int stream_id, const char *value);
int rk_video_get_smart(int stream_id, const char **value);
int rk_video_set_smart(int stream_id, const char *value);
int rk_video_get_gop_mode(int stream_id, const char **value);
int rk_video_set_gop_mode(int stream_id, const char *value);
int rk_video_get_stream_type(int stream_id, const char **value);
int rk_video_set_stream_type(int stream_id, const char *value);
int rk_video_get_h264_profile(int stream_id, const char **value);
int rk_video_set_h264_profile(int stream_id, const char *value);
int rk_video_get_resolution(int stream_id, char **value);
int rk_video_set_resolution(int stream_id, const char *value);
int rk_video_get_frame_rate(int stream_id, char **value);
int rk_video_set_frame_rate(int stream_id, const char *value);
int rk_video_get_frame_rate_in(int stream_id, char **value);
int rk_video_set_frame_rate_in(int stream_id, const char *value);
int rk_video_get_rotation(int *value);
int rk_video_set_rotation(int value);
// jpeg
int rk_video_get_enable_cycle_snapshot(int *value);
int rk_video_set_enable_cycle_snapshot(int value);
int rk_video_get_image_quality(int *value);
int rk_video_set_image_quality(int value);
int rk_video_get_snapshot_interval_ms(int *value);
int rk_video_set_snapshot_interval_ms(int value);
int rk_video_get_jpeg_resolution(char **value);
int rk_video_set_jpeg_resolution(const char *value);
int rk_take_photo();
int rkipc_venc_jpeg_init();
int rkipc_venc_jpeg_deinit();
int rkipc_osd_draw_nn_init();