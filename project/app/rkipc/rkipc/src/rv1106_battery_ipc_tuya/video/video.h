// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "isp.h"
#include "rkbar_scan_api.h"
#include "rtsp.h"
//#include "tuya_ipc.h"
#include "rk_adc.h"
#include "rockiva.h"
#include "tools.h"
#include "tuya_ipc.h"

#include <fcntl.h>
#include <inttypes.h> // PRId64
#include <linux/input.h>
#include <rga/im2d.h>
#include <rga/rga.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <rk_debug.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_rgn.h>
#include <rk_mpi_sys.h>
#include <rk_mpi_venc.h>
#include <rk_mpi_vi.h>
#include <rk_mpi_vo.h>
#include <rk_mpi_vpss.h>

int rk_video_init();
int rk_video_deinit();

int rkipc_vi_dev_init();
int rkipc_pipe_0_vi_init();
int rkipc_pipe_0_venc_init();
int rkipc_pipe_0_bind();