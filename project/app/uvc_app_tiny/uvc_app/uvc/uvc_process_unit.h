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

#ifndef __UVC_PROCESS_UNIT_H__
#define __UVC_PROCESS_UNIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CAM_MAX_NUM 3

typedef enum _UVC_EPTZ_TYPE {
  UVC_EPTZ_ZOOM = 0,
  UVC_EPTZ_PAN = 1,
  UVC_EPTZ_TILT = 2,
  UVC_EPTZ_ROLL = 3, // reserve
  UVC_EPTZ_AUTO = 4,
  UVC_EPTZ_MAX,
} UVC_EPTZ_TYPE;

typedef enum _UVC_RATIO_TYPE {
  VERTICAL = 0,
  HORIZONTAL = 1,
  UVC_RATIO_MAX,
} UVC_RATIO_TYPE;

typedef enum _UVC_RET_TYPE {
  UVC_RET_ERR = -1,
  UVC_RET_OK = 0,
  UVC_RET_WAT = 1,
  UVC_RET_MAX,
} UVC_RET_TYPE;

typedef struct uvc_ctrl_info {
  int index;
  int width;
  int height;
  int video_id;
  int fcc;
  unsigned int fps;
  UVC_EPTZ_TYPE type;
  int eptz[UVC_EPTZ_MAX];
} UVC_CTRL_INFO;

int uvc_process_config(UVC_CTRL_INFO uvc_ctrl_info);
int uvc_process_start(UVC_CTRL_INFO uvc_ctrl_info);
void uvc_process_stop(UVC_CTRL_INFO uvc_ctrl_info);
void uvc_process_delete(UVC_CTRL_INFO uvcCtrlInfo);
int uvc_process_release_frame(UVC_CTRL_INFO uvcCtrlInfo, void *frame);
int nn_process_release_frame_by_id(int frameId);

#ifdef __cplusplus
}
#endif

#endif
