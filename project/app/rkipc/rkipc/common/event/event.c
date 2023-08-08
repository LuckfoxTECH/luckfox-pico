// Copyright 2021-2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "common.h"
#include "rockiva.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "event.c"

int rk_event_init() {
	LOG_INFO("%s\n", __func__);

	return 0;
}

int rk_event_deinit() {
	LOG_INFO("%s\n", __func__);

	return 0;
}

// regional invasion
int rk_event_ri_get_enabled(int *value) {
	*value = rk_param_get_int("event.regional_invasion:enabled", 0);

	return 0;
}

int rk_event_ri_set_enabled(int value) {
	rk_param_set_int("event.regional_invasion:enabled", value);

	return 0;
}

int rk_event_ri_get_position_x(int *value) {
	*value = rk_param_get_int("event.regional_invasion:position_x", 0);

	return 0;
}

int rk_event_ri_set_position_x(int value) {
	rk_param_set_int("event.regional_invasion:position_x", value);

	return 0;
}

int rk_event_ri_get_position_y(int *value) {
	*value = rk_param_get_int("event.regional_invasion:position_y", 0);

	return 0;
}

int rk_event_ri_set_position_y(int value) {
	rk_param_set_int("event.regional_invasion:position_y", value);

	return 0;
}

int rk_event_ri_get_width(int *value) {
	*value = rk_param_get_int("event.regional_invasion:width", 256);

	return 0;
}

int rk_event_ri_set_width(int value) {
	rk_param_set_int("event.regional_invasion:width", value);

	return 0;
}

int rk_event_ri_get_height(int *value) {
	*value = rk_param_get_int("event.regional_invasion:height", 256);

	return 0;
}

int rk_event_ri_set_height(int value) {
	rk_param_set_int("event.regional_invasion:height", value);

	return 0;
}

int rk_event_ri_get_proportion(int *value) {
	*value = rk_param_get_int("event.regional_invasion:proportion", 5);

	return 0;
}

int rk_event_ri_set_proportion(int value) {
	rk_param_set_int("event.regional_invasion:proportion", value);

	return 0;
}

int rk_event_ri_get_sensitivity_level(int *value) {
	*value = rk_param_get_int("event.regional_invasion:sensitivity_level", 50);

	return 0;
}

int rk_event_ri_set_sensitivity_level(int value) {
	rk_param_set_int("event.regional_invasion:sensitivity_level", value);

	return 0;
}

int rk_event_ri_get_time_threshold(int *value) {
	*value = rk_param_get_int("event.regional_invasion:time_threshold", 1);

	return 0;
}

int rk_event_ri_set_time_threshold(int value) {
	rk_param_set_int("event.regional_invasion:time_threshold", value);
	rkipc_rockiva_deinit();
	rkipc_rockiva_init();

	return 0;
}
