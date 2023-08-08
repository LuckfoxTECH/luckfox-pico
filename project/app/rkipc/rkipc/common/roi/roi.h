// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _RKIPC_ROI_H_
#define _RKIPC_ROI_H_

#define MAX_ROI_NUM 6

typedef struct roi_data {
	const char *stream_type;
	int id;
	int enabled;
	int position_x;
	int position_y;
	int width;
	int height;
	int quality_level;
} roi_data_s;

typedef int (*rk_roi_set_callback)(roi_data_s *);

void rk_roi_set_callback_register(rk_roi_set_callback callback_ptr);

int rk_roi_set_all();
// roi.x
int rk_roi_get_stream_type(int id, const char **value);
int rk_roi_set_stream_type(int id, const char *value);
int rk_roi_get_name(int id, const char **value);
int rk_roi_set_name(int id, const char *value);
int rk_roi_get_id(int id, int *value);
int rk_roi_set_id(int id, int value);
int rk_roi_get_enabled(int id, int *value);
int rk_roi_set_enabled(int id, int value);
int rk_roi_get_position_x(int id, int *value);
int rk_roi_set_position_x(int id, int value);
int rk_roi_get_position_y(int id, int *value);
int rk_roi_set_position_y(int id, int value);
int rk_roi_get_height(int id, int *value);
int rk_roi_set_height(int id, int value);
int rk_roi_get_width(int id, int *value);
int rk_roi_set_width(int id, int value);
int rk_roi_get_quality_level(int id, int *value);
int rk_roi_set_quality_level(int id, int value);

#endif
