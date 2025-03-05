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

#ifndef __UVC_PROCESS_H__
#define __UVC_PROCESS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "rk_comm_video.h"
#include "uvc_process_unit.h"

#ifdef __cplusplus
}
#endif

class UVCProcess {
public:
  explicit UVCProcess();
  ~UVCProcess();
  void *getCtx() { return mCtx; }
  int configProcess(UVC_CTRL_INFO uvc_ctrl_info);
  int startProcess();
  int stopProcess();
  int doProcess();
  int releaseFrame(void *frame);

private:
  int initialize();
  int deinitialize();
  int configVi();
  int startVi();
  int stopVi();
  int configVenc();
  int startVenc();
  int stopVenc();
  int resetCtx();
  int configVpss();
  int configCropScale(RECT_S src, RECT_S dst);
  int startVpss();
  int stopVpss();
  int bindViVenc();
  int unBindViVenc();
  int bindViVpss();
  int unBindViVpss();
  int bindVpssVenc();
  int unBindVpssVenc();
  static void *threadLoop(void *arg);

private:
  void *mCtx;
};

#endif
