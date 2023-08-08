// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "bmp_reader.h"
#include "osd_common.h"

typedef int (*rk_osd_cover_create_callback)(int, osd_data_s *);
typedef int (*rk_osd_cover_destroy_callback)(int);
typedef int (*rk_osd_mosaic_create_callback)(int, osd_data_s *);
typedef int (*rk_osd_mosaic_destroy_callback)(int);
typedef int (*rk_osd_bmp_create_callback)(int, osd_data_s *);
typedef int (*rk_osd_bmp_destroy_callback)(int);
typedef int (*rk_osd_bmp_change_callback)(int, osd_data_s *);

void rk_osd_cover_create_callback_register(rk_osd_cover_create_callback callback_ptr);
void rk_osd_cover_destroy_callback_register(rk_osd_cover_destroy_callback callback_ptr);
void rk_osd_mosaic_create_callback_register(rk_osd_mosaic_create_callback callback_ptr);
void rk_osd_mosaic_destroy_callback_register(rk_osd_mosaic_destroy_callback callback_ptr);
void rk_osd_bmp_create_callback_register(rk_osd_bmp_create_callback callback_ptr);
void rk_osd_bmp_destroy_callback_register(rk_osd_bmp_destroy_callback callback_ptr);
void rk_osd_bmp_change_callback_register(rk_osd_bmp_change_callback callback_ptr);
int rk_osd_init();
int rk_osd_deinit();
int rk_osd_restart();

int fill_image(osd_data_s *data);
int fill_text(osd_data_s *data);
int generate_date_time(const char *fmt, wchar_t *result, int r_size);

// osd.common
int rk_osd_get_is_presistent_text(int *value);
int rk_osd_set_is_presistent_text(int value);
int rk_osd_get_font_size(int *value);
int rk_osd_set_font_size(int value);
int rk_osd_get_boundary(int *value);
int rk_osd_set_boundary(int value);
int rk_osd_get_normalized_screen_width(int *value);
int rk_osd_get_normalized_screen_height(int *value);
int rk_osd_get_attribute(const char **value);
int rk_osd_set_attribute(const char *value);
int rk_osd_get_font_color_mode(const char **value);
int rk_osd_set_font_color_mode(const char *value);
int rk_osd_get_font_color(const char **value);
int rk_osd_set_font_color(const char *value);
int rk_osd_get_alignment(const char **value);
int rk_osd_set_alignment(const char *value);
int rk_osd_get_font_path(const char **value);
int rk_osd_set_font_path(const char *value);
// osd.x
int rk_osd_get_enabled(int id, int *value);
int rk_osd_set_enabled(int id, int value);
int rk_osd_get_position_x(int id, int *value);
int rk_osd_set_position_x(int id, int value);
int rk_osd_get_position_y(int id, int *value);
int rk_osd_set_position_y(int id, int value);
int rk_osd_get_height(int id, int *value);
int rk_osd_set_height(int id, int value);
int rk_osd_get_width(int id, int *value);
int rk_osd_set_width(int id, int value);
int rk_osd_get_display_week_enabled(int id, int *value);
int rk_osd_set_display_week_enabled(int id, int value);
int rk_osd_get_date_style(int id, const char **value);
int rk_osd_set_date_style(int id, const char *value);
int rk_osd_get_time_style(int id, const char **value);
int rk_osd_set_time_style(int id, const char *value);
int rk_osd_get_type(int id, const char **value);
int rk_osd_set_type(int id, const char *value);
int rk_osd_get_display_text(int id, const char **value);
int rk_osd_set_display_text(int id, const char *value);
int rk_osd_get_image_path(int id, const char **value);
int rk_osd_set_image_path(int id, const char *value);
int rk_osd_get_style(int id, const char **value);
int rk_osd_set_style(int id, const char *value);