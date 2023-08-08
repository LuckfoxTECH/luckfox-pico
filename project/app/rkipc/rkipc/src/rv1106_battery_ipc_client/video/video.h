// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "isp.h"
#include "osd.h"
#include "region_clip.h"
#include "rockiva.h"
#include "roi.h"
#include "rtmp.h"
#include "rtsp_demo.h"
#include "storage.h"

#include "rk_mpi_mmz.h"
#include <rk_debug.h>
#include <rk_mpi_ivs.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_rgn.h>
#include <rk_mpi_sys.h>
#include <rk_mpi_venc.h>
#include <rk_mpi_vi.h>
#include <rk_mpi_vpss.h>
#include <sys/mman.h>

#include <inttypes.h> // PRId64

#include "rk_comm_tde.h"
#include "rk_meta_app_param.h"
#include "rk_mpi_tde.h"
#include <rga/im2d.h>
#include <rga/rga.h>

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
int rk_video_get_smartp_viridrlen(int stream_id, int *value);
int rk_video_set_smartp_viridrlen(int stream_id, int value);
int rk_video_get_md_switch(int *value);
int rk_video_set_md_switch(int value);
int rk_video_get_md_sensebility(int *value);
int rk_video_set_md_sensebility(int value);
int rk_video_get_od_switch(int *value);
int rk_video_set_od_switch(int value);
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
int rkipc_read_venctype_from_meta();
