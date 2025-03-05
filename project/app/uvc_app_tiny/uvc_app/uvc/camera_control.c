/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
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

#include "camera_control.h"
#include "isp.h"
#include "param.h"
#include "uvc_log.h"

extern char *rkuvc_iq_file_path_;

struct Camera_Stream {};

void video_record_signal(struct Camera_Stream *stream) {}

static void camera_stop(struct Camera_Stream *stream) {
  LOG_INFO("%s \n", __func__);
}

static void *uvc_camera(void *arg) {}

int camera_control_start(int id, int width, int height, int fps, int format,
                         int eptz) {
  if (rk_param_get_int("video.source:enable_aiq", 1))
    camera_control_set_rate(id, fps);
  return 0;
}

void camera_control_stop(int id) {
  LOG_INFO("%s: id:%d\n", __func__, id);
  int ret = 0;
  if (rk_param_get_int("video.source:enable_aiq", 1))
    ret |= rk_isp_deinit(id);
  if (rk_param_get_int("video.source:enable_aiq", 1))
    ret |= rk_isp_init(id, rkuvc_iq_file_path_);
  return ret;
}

void camera_control_init() {
  LOG_INFO("%s \n", __func__);
  if (rk_param_get_int("video.source:enable_aiq", 1)) {
    rk_isp_init(0, rkuvc_iq_file_path_);
    rk_isp_set_frame_rate(0, rk_param_get_int("isp.0.adjustment:fps", 30));
    if (rk_param_get_int("video.source:enable_2uvc", 0))
      rk_isp_init(1, rkuvc_iq_file_path_);
  } else {
    LOG_INFO("%s disabled aiq \n", __func__);
  }
}

void camera_control_set_rate(int index, int rate) {
  LOG_INFO("camera index:%d rate:%d \n", index, rate);
  if (rk_param_get_int("video.source:enable_aiq", 1)) {
    rk_isp_set_frame_rate(index, rate);
  }
}
void camera_control_set_zoom(int val) { LOG_INFO("set_zoom:%d\n", val); }

void camera_control_set_pan(int val) { LOG_INFO("set_pan:%d\n", val); }

void camera_control_set_tilt(int val) { LOG_INFO("set_tilt:%d\n", val); }

void camera_control_set_roll(int val) {
  LOG_INFO("set_roll:%d\n", val);
  // todo
}

void camera_control_deinit() {
  LOG_INFO("%s \n", __func__);
  if (rk_param_get_int("video.source:enable_aiq", 1)) {
    rk_isp_deinit(0);
    if (rk_param_get_int("video.source:enable_2uvc", 0))
      rk_isp_deinit(1);
  }
}
