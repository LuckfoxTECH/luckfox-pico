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

#ifndef __UVC_CONTROL_H__
#define __UVC_CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "uvc_process_unit.h"
#include "uvc-gadget.h"
#include <stdint.h>

#ifdef USE_RK_MODULE
#define ISP_SEQ 1
#define ISP_FMT HAL_FRMAE_FMT_NV12
#define CIF_SEQ 0
#define CIF_FMT HAL_FRMAE_FMT_SBGGR10
#else
#define ISP_SEQ 0
#define ISP_FMT HAL_FRMAE_FMT_SBGGR8
#define CIF_SEQ 1
#define CIF_FMT HAL_FRMAE_FMT_NV12
#endif

#define UVC_CONTROL_LOOP_ONCE (1 << 0)
#define UVC_CONTROL_CHECK_STRAIGHT (1 << 1)
#define UVC_CONTROL_CAMERA (1 << 2)

extern int app_quit;

typedef void (*uvc_pu_control_callback_t)(unsigned char cs, unsigned int val);
void register_uvc_pu_control_callback(uvc_pu_control_callback_t cb);
extern uvc_pu_control_callback_t uvc_pu_control_cb;

void add_uvc_video();
int uvc_control_loop(void);
int check_uvc_video_id(void);
void set_uvc_control_start(int video_id, int width, int height, int fps,
                           int format, int eptz);
void set_uvc_control_stop(void);
void set_uvc_control_restart(void);
void uvc_control_start_setcallback(int (*callback)(int fd, int width,
                                                   int height, int fps,
                                                   int format, int eptz));
void uvc_control_stop_setcallback(void (*callback)());
void uvc_control_config(struct uvc_device *dev);
#if UVC_EPTZ
void uvc_control_config_eptz(struct uvc_device *dev, UVC_EPTZ_TYPE type,
                             unsigned int value);
#endif
void uvc_control_streamon(struct uvc_device *dev);
void uvc_control_streamoff(int video_id);
void uvc_control_delete(int video_id);
void uvc_control_inbuf_deinit();
int uvc_control_run(uint32_t flags);
void set_uvc_control_suspend(int suspend);
void uvc_control_release_frame(struct uvc_device *dev, void *frame);
void uvc_control_release_frame_nonlock(struct uvc_device *dev, void *frame);

#ifdef __cplusplus
}
#endif

#endif
