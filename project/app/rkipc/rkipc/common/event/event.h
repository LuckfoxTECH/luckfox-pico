// Copyright 2021-2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
int rk_event_init();
int rk_event_deinit();
// regional invasion
int rk_event_ri_get_enabled(int *value);
int rk_event_ri_set_enabled(int value);
int rk_event_ri_get_position_x(int *value);
int rk_event_ri_set_position_x(int value);
int rk_event_ri_get_position_y(int *value);
int rk_event_ri_set_position_y(int value);
int rk_event_ri_get_width(int *value);
int rk_event_ri_set_width(int value);
int rk_event_ri_get_height(int *value);
int rk_event_ri_set_height(int value);
int rk_event_ri_get_proportion(int *value);
int rk_event_ri_set_proportion(int value);
int rk_event_ri_get_sensitivity_level(int *value);
int rk_event_ri_set_sensitivity_level(int value);
int rk_event_ri_get_time_threshold(int *value);
int rk_event_ri_set_time_threshold(int value);
