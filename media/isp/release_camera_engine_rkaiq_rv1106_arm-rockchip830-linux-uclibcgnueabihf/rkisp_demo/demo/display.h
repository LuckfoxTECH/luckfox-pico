/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 * author: Zhihua Wang, hogan.wang@rock-chips.com
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#ifdef __cplusplus
extern "C"
{
#endif

enum display_video_type {
    DISPLAY_VIDEO_RGB,
    DISPLAY_VIDEO_IR,
    DISPLAY_VIDEO_USB,
};

int display_init(int width, int height);
void display_exit(void);
void display_commit(void *ptr, int fd, int fmt, int w, int h, int rotation);
void display_switch(enum display_video_type type);
void display_get_resolution(int *width, int *height);

void display_win1(void *ptr, int fd, int fmt, int w, int h, int rotation);
void display_win2(void *ptr, int fd, int fmt, int w, int h, int rotation);

#ifdef __cplusplus
}
#endif
#endif
