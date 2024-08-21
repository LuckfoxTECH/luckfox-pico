/*
 * Copyright (C) 2022  Rockchip Electronics Co., Ltd.
 * Authors:
 *     YuQiaowei <cerf.yu@rock-chips.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RGA_SAMPLES_UTILS_H__
#define __RGA_SAMPLES_UTILS_H__

#include <stdlib.h>

int64_t get_cur_us();
int64_t get_cur_ms();
void draw_rgba(char *buffer, int width, int height);
void draw_YUV420(char *buffer, int width, int height);
void draw_YUV422(char *buffer, int width, int height);
void draw_gray256(char *buffer, int width, int height);
int read_image_from_fbc_file(void *buf, const char *path, int sw, int sh, int fmt, int index);
int read_image_from_file(void *buf, const char *path, int sw, int sh, int fmt, int index);
int write_image_to_fbc_file(void *buf, const char *path, int sw, int sh, int fmt, int index);
int write_image_to_file(void *buf, const char *path, int sw, int sh, int fmt, int index);

#endif /* #ifndef __RGA_SAMPLES_UTILS_H__ */
