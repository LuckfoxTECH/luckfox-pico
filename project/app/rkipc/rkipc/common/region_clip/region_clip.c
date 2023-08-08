// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// #include <locale.h>

#include "region_clip.h"
#include "common.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "region_clip.c"

#define MAX_REGION_CLIP_NUM 3

rk_region_clip_set_callback rk_region_clip_set_ = NULL;

void rk_region_clip_set_callback_register(rk_region_clip_set_callback callback_ptr) {
	rk_region_clip_set_ = callback_ptr;
}

int rk_region_clip_set_all() {
	region_clip_data_s region_clip_data;
	char entry[128] = {'\0'};
	int video_width, video_height;
	int normalized_screen_width = rk_param_get_int("osd.common:normalized_screen_width", -1);
	int normalized_screen_height = rk_param_get_int("osd.common:normalized_screen_height", -1);

	for (int id = 0; id < MAX_REGION_CLIP_NUM; id++) {
		snprintf(entry, 127, "video.%d:width", id);
		video_width = rk_param_get_int(entry, -1);
		snprintf(entry, 127, "video.%d:height", id);
		video_height = rk_param_get_int(entry, -1);

		snprintf(entry, 127, "region_clip.%d:enabled", id);
		region_clip_data.enabled = rk_param_get_int(entry, -1);
		if (region_clip_data.enabled == -1)
			continue;
		snprintf(entry, 127, "region_clip.%d:position_x", id);
		region_clip_data.position_x =
		    DOWNALIGNTO16((int)(rk_param_get_int(entry, -1) * (double)video_width /
		                        (double)normalized_screen_width));
		if (region_clip_data.position_x < 0)
			region_clip_data.position_x = 0;
		snprintf(entry, 127, "region_clip.%d:position_y", id);
		region_clip_data.position_y =
		    DOWNALIGNTO16((int)(rk_param_get_int(entry, -1) * (double)video_height /
		                        (double)normalized_screen_height));
		if (region_clip_data.position_y < 0)
			region_clip_data.position_y = 0;
		snprintf(entry, 127, "region_clip.%d:width", id);
		region_clip_data.width = rk_param_get_int(entry, -1);
		snprintf(entry, 127, "region_clip.%d:height", id);
		region_clip_data.height = rk_param_get_int(entry, -1);
		if (rk_region_clip_set_ != NULL)
			rk_region_clip_set_(id, &region_clip_data);
	}

	return 0;
}

// for socket server
// region_clip.x

int rk_region_clip_get_enabled(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:enabled", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_region_clip_set_enabled(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:enabled", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_region_clip_get_position_x(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:position_x", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_region_clip_set_position_x(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:position_x", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_region_clip_get_position_y(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:position_y", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_region_clip_set_position_y(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:position_y", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_region_clip_get_height(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:height", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_region_clip_set_height(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:height", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_region_clip_get_width(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:width", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_region_clip_set_width(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "region_clip.%d:width", id);
	rk_param_set_int(entry, value);

	return 0;
}
