/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
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
#ifndef _RKDRM_DISPLAY_H_
#define _RKDRM_DISPLAY_H_

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <drm_fourcc.h>
#include <sys/mman.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

struct drm_buf {
  int fb_id;
  uint32_t handle;
  uint32_t size;
  uint32_t pitch;
  char *map;
  int dmabuf_fd;
};

struct plane_prop {
  int crtc_id;
  int fb_id;

  int src_x;
  int src_y;
  int src_w;
  int src_h;

  int crtc_x;
  int crtc_y;
  int crtc_w;
  int crtc_h;

  int zpos;
};

struct drm_dev_plane {
  drmModePlanePtr p;
  int zpos_max;
  struct plane_prop plane_prop;
};

struct drm_dev {
  int drm_fd;
  int crtc_index;
  drmModeCrtcPtr crtc;
  drmModeConnectorPtr connector;
  drmModePropertyPtr dpms_prop;

  struct drm_dev_plane plane_primary;
  struct drm_dev_plane plane_overlay;
};

int drmGetBuffer(int fd, int width, int height, int format,
                 struct drm_buf *buffer);
int drmPutBuffer(int fd, struct drm_buf *buffer);
int drmInit(struct drm_dev *dev);
int drmDeinit(struct drm_dev *dev);
int drmCommit(struct drm_buf *buffer, int width, int height, int x_off,
              int y_off, struct drm_dev *dev, int plane_type);
int drmSetDpmsMode(uint32_t dpms_mode, struct drm_dev *dev);

#endif
