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
#include "camera_pu_control.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "uvc_log.h"
#include <pthread.h>

int video_record_set_brightness(int brightness) {
  rk_isp_set_brightness(0, brightness);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_contrast(int contrast) {
  rk_isp_set_contrast(0, contrast);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_hue(int hue) {
  rk_isp_set_hue(0, hue);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_staturation(int staturation) {
  rk_isp_set_saturation(0, staturation);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_sharpness(int sharpness) {
  rk_isp_set_sharpness(0, sharpness);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_fps(int fixfps) {
  rk_isp_set_frame_rate(0, fixfps);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_gamma(int gamma) {
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_white_balance_temperature(int balance) {
  //[2800,6500]
  LOG_INFO("WhiteBalance color temperature is %d \n", balance);
  rk_isp_set_white_blance_ct(0, balance);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_white_balance_temperature_auto(int balance) {
  if (balance == 1)
    rk_isp_set_white_blance_style(0, "autoWhiteBalance");
  else
    rk_isp_set_white_blance_style(0, "manualWhiteBalance");
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_gain(int gain) {
  rk_isp_set_exposure_gain(0, gain);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_hue_auto(int hue_auto) {
  rk_isp_set_hue_mode(0, hue_auto);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_frequency_mode(int mode) {
  if (mode == 1)
    rk_isp_set_power_line_frequency_mode(0, "PAL(50HZ)");
  else
    rk_isp_set_power_line_frequency_mode(0, "NTSC(60HZ)");
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_roll_mode(int mode) {
  switch (mode) {
  case 0: {
    rk_isp_set_image_flip(0, "close");
    break;
  }
  case 1: {
    rk_isp_set_image_flip(0, "flip");
    break;
  }
  case 2: {
    rk_isp_set_image_flip(0, "centrosymmetric");
    break;
  }
  case 3: {
    rk_isp_set_image_flip(0, "mirror");
    break;
  }
  default: {
    rk_isp_set_image_flip(0, "close");
    break;
  }
  }
  return 0;
}

int video_record_set_exposure_time(int time) {
  char str[8];
  sprintf(str, "%f", ((float)time / 10000));
  rk_isp_set_exposure_time(0, str);
  LOG_INFO("%s \n", __func__);
  return 0;
}

int video_record_set_ae_mode(int mode) {
  if (mode == 2) {
    LOG_DEBUG("AE:set auto mode!!\n");
    rk_isp_set_exposure_mode(0, "auto");
  } else {
    LOG_DEBUG("AE:set  manual mode!!\n");
    rk_isp_set_exposure_mode(0, "manual");
  }
  LOG_INFO("%s \n", __func__);
  return 0;
}

void camera_pu_control_init(int type, int def, int min, int max) {
  LOG_DEBUG("%s!\n", __func__);
}

int camera_pu_control_get(int type, int def) {
  LOG_DEBUG("%s!\n", __func__);
  return def;
}

int check_ispserver_work() {
  if (rk_param_get_int("video.source:enable_aiq", 1))
    return 1;
  else
    return 0;
}

int camera_pu_control_set(int type, int value) {
  LOG_DEBUG("%s! type is %d,value is %d\n", __func__, type, value);
  if (!check_ispserver_work()) {
    LOG_INFO("check ispserver is no ready,pu set fail!\n");
    return 0;
  }
  switch (type) {
  case UVC_PU_BRIGHTNESS_CONTROL:
    video_record_set_brightness(value);
    break;
  case UVC_PU_CONTRAST_CONTROL:
    video_record_set_contrast(value);
    break;
  case UVC_PU_HUE_CONTROL:
    video_record_set_hue(value);
    break;
  case UVC_PU_SATURATION_CONTROL:
    video_record_set_staturation(value);
    break;
  case UVC_PU_SHARPNESS_CONTROL:
    video_record_set_sharpness(value);
    break;
  case UVC_PU_GAMMA_CONTROL:
    // video_record_set_gamma(value);
    break;
  case UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL:
    video_record_set_white_balance_temperature(value);
    break;
  case UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL:
    video_record_set_white_balance_temperature_auto(value);
    break;
  case UVC_PU_GAIN_CONTROL:
    video_record_set_gain(value);
    break;
  case UVC_PU_HUE_AUTO_CONTROL:
    video_record_set_hue_auto(value);
    break;
  case UVC_PU_POWER_LINE_FREQUENCY_CONTROL:
    video_record_set_frequency_mode(value);
    break;
  case UVC_PU_FPS_CONTROL:
    video_record_set_fps(value);
    break;
  case UVC_PU_ROLL_CONTROL:
    video_record_set_roll_mode(value);
    break;
  case UVC_PU_EXPOSURE_TIME_CONTROL:
    video_record_set_exposure_time(value);
    break;
  case UVC_PU_AE_MODE_CONTROL:
    video_record_set_ae_mode(value);
    break;
  default:
    LOG_DEBUG("====unknow pu cmd.\n");
    break;
  }
  return 0;
}

int camera_pu_control_check(int deviceid) {
  LOG_DEBUG("%s!\n", __func__);
  return 0;
}
