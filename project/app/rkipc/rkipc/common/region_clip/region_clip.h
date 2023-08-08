// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _RKIPC_REGION_CLIP_H_
#define _RKIPC_REGION_CLIP_H_

typedef struct region_clip_data {
	int enabled;
	int position_x;
	int position_y;
	int width;
	int height;
} region_clip_data_s;

typedef int (*rk_region_clip_set_callback)(int, region_clip_data_s *);

void rk_region_clip_set_callback_register(rk_region_clip_set_callback callback_ptr);

int rk_region_clip_set_all();
// region_clip.x
int rk_region_clip_get_enabled(int id, int *value);
int rk_region_clip_set_enabled(int id, int value);
int rk_region_clip_get_position_x(int id, int *value);
int rk_region_clip_set_position_x(int id, int value);
int rk_region_clip_get_position_y(int id, int *value);
int rk_region_clip_set_position_y(int id, int value);
int rk_region_clip_get_height(int id, int *value);
int rk_region_clip_set_height(int id, int value);
int rk_region_clip_get_width(int id, int *value);
int rk_region_clip_set_width(int id, int value);

#endif
