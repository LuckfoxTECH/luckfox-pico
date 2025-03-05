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

#ifndef __UVC_MPI_CONFIG_H__
#define __UVC_MPI_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rk_mpi_vi.h"
#include "rk_mpi_vpss.h"
#include <rk_mpi_venc.h>

#define UVC_MPI_CFG_CHANGE_BIT(x) (1 << x)
#define UVC_MPI_STREAM_SAVE_FILE_LEN 32
#define VPSS_MAX_CHN 1 // now only support one chn.

typedef enum _MpiUvcCommonCfgChange {
  MPI_UVC_COMMON_CFG_NEED_CHECK = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_UVC_COMMON_CFG_UVC_DEBUG = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_UVC_COMMON_CFG_UVC_DEBUG_FILE_NAME = UVC_MPI_CFG_CHANGE_BIT(2),
  MPI_UVC_COMMON_CFG_UVC_DEBUG_FILE_CNT = UVC_MPI_CFG_CHANGE_BIT(3),
  MPI_UVC_COMMON_CFG_GEOMETRIC_OUT = UVC_MPI_CFG_CHANGE_BIT(4),
  MPI_UVC_COMMON_CFG_UVC_YUYV_DEBUG = UVC_MPI_CFG_CHANGE_BIT(5),
  MPI_UVC_COMMON_CFG_UVC_NN_ENABLE = UVC_MPI_CFG_CHANGE_BIT(6),
  MPI_UVC_COMMON_CFG_NN_DEBUG_FILE_NAME = UVC_MPI_CFG_CHANGE_BIT(7),
  MPI_UVC_COMMON_CFG_NN_DEBUG_FILE_CNT = UVC_MPI_CFG_CHANGE_BIT(8),
  MPI_UVC_COMMON_CFG_UVC_ENABLE_VPSS = UVC_MPI_CFG_CHANGE_BIT(9),
  MPI_UVC_COMMON_CFG_CHANGE_ALL = (0xffffffff),
} MpiUvcCommonCfgChange;

typedef enum _MpiViCfgChange {
  MPI_VI_CFG_CHANGE_DEV_ID = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_VI_CFG_CHANGE_CHANNEL_ID = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_VI_CFG_CHANGE_BUF_CNT = UVC_MPI_CFG_CHANGE_BIT(2),
  MPI_VI_CFG_CHANGE_FBC = UVC_MPI_CFG_CHANGE_BIT(3),
  MPI_VI_CFG_CHANGE_MEMORY_TYPE = UVC_MPI_CFG_CHANGE_BIT(4),
  MPI_VI_CFG_CHANGE_ROTATION = UVC_MPI_CFG_CHANGE_BIT(5),
  MPI_VI_CFG_CHANGE_MIRROR = UVC_MPI_CFG_CHANGE_BIT(6),
  MPI_VI_CFG_CHANGE_DEV_NAME = UVC_MPI_CFG_CHANGE_BIT(7),
  MPI_VI_CFG_CHANGE_FPS = UVC_MPI_CFG_CHANGE_BIT(8),
  MPI_VI_CFG_CHANGE_ASSIGN_WIDTH = UVC_MPI_CFG_CHANGE_BIT(9),
  MPI_VI_CFG_CHANGE_ASSIGN_HEIGHT = UVC_MPI_CFG_CHANGE_BIT(10),
  MPI_VI_CFG_CHANGE_MIN_WIDTH = UVC_MPI_CFG_CHANGE_BIT(11),
  MPI_VI_CFG_CHANGE_MIN_HEIGHT = UVC_MPI_CFG_CHANGE_BIT(12),
  MPI_VI_CFG_CHANGE_MAX_WIDTH = UVC_MPI_CFG_CHANGE_BIT(13),
  MPI_VI_CFG_CHANGE_MAX_HEIGHT = UVC_MPI_CFG_CHANGE_BIT(14),
  MPI_VI_CFG_CHANGE_FORMAT = UVC_MPI_CFG_CHANGE_BIT(15),
  MPI_VI_CFG_CHANGE_ALL = (0xffffffff),
} MpiViCfgChange;

typedef enum _MpiVpssCfgChange {
  MPI_VPSS_CFG_CHANGE_GROUP_ID = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_VPSS_CFG_CHANGE_CHANNEL_ID = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_VPSS_CFG_CHANGE_BUF_CNT = UVC_MPI_CFG_CHANGE_BIT(2),
  MPI_VPSS_CFG_CHANGE_FBC = UVC_MPI_CFG_CHANGE_BIT(3),
  MPI_VPSS_CFG_CHANGE_ROTATION = UVC_MPI_CFG_CHANGE_BIT(4),
  MPI_VPSS_CFG_CHANGE_MIRROR = UVC_MPI_CFG_CHANGE_BIT(5),
  MPI_VPSS_CFG_CHANGE_FPS = UVC_MPI_CFG_CHANGE_BIT(6),
  MPI_VPSS_CFG_CHANGE_FORMAT = UVC_MPI_CFG_CHANGE_BIT(7),
  MPI_VPSS_CFG_CHANGE_ASSIGN_WIDTH = UVC_MPI_CFG_CHANGE_BIT(8),
  MPI_VPSS_CFG_CHANGE_ASSIGN_HEIGHT = UVC_MPI_CFG_CHANGE_BIT(9),
  MPI_VPSS_CFG_CHANGE_ALL = (0xffffffff),
} MpiVpssCfgChange;

typedef enum _MpiVencCommonCfgChange {
  MPI_VENC_COMMON_CFG_CHANGE_CREATE = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_VENC_COMMON_CFG_CHANGE_ROTATION = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_VENC_COMMON_CFG_CHANGE_MIRROR = UVC_MPI_CFG_CHANGE_BIT(2),
  MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR = UVC_MPI_CFG_CHANGE_BIT(3),
  MPI_VENC_COMMON_CFG_CHANGE_ALL = (0xffffffff),
} MpiVencCommonCfgChange;

typedef enum _MpiVencMjpegCfgChange {
  MPI_VENC_MJPEG_CFG_CHANGE_RC_MODE = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR_MIN = UVC_MPI_CFG_CHANGE_BIT(2), // reserver
  MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR_MAX = UVC_MPI_CFG_CHANGE_BIT(3), // reserver
  MPI_VENC_MJPEG_CFG_CHANGE_BPS = UVC_MPI_CFG_CHANGE_BIT(4),
  MPI_VENC_MJPEG_CFG_CHANGE_FPS = UVC_MPI_CFG_CHANGE_BIT(5),
  MPI_VENC_MJPEG_CFG_CHANGE_BUF_CNT = UVC_MPI_CFG_CHANGE_BIT(6),
  MPI_VENC_MJPEG_CFG_CHANGE_SEI = UVC_MPI_CFG_CHANGE_BIT(7),
  MPI_VENC_MJPEG_CFG_CHANGE_RANGE = UVC_MPI_CFG_CHANGE_BIT(8),
  MPI_VENC_MJPEG_CFG_CHANGE_FBC = UVC_MPI_CFG_CHANGE_BIT(9),
  MPI_VENC_MJPEG_CFG_CHANGE_ALL = (0xffffffff),
} MpiVencMjpegCfgChange;

typedef enum _MpiVencH264CfgChange {
  MPI_VENC_H264_CFG_CHANGE_GOP = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_VENC_H264_CFG_CHANGE_RC_MODE = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_VENC_H264_CFG_CHANGE_BPS = UVC_MPI_CFG_CHANGE_BIT(2),
  MPI_VENC_H264_CFG_CHANGE_GOP_MODE = UVC_MPI_CFG_CHANGE_BIT(3),
  MPI_VENC_H264_CFG_CHANGE_VI_LEN = UVC_MPI_CFG_CHANGE_BIT(4),
  MPI_VENC_H264_CFG_CHANGE_FPS = UVC_MPI_CFG_CHANGE_BIT(5),
  MPI_VENC_H264_CFG_CHANGE_BUF_CNT = UVC_MPI_CFG_CHANGE_BIT(6),
  MPI_VENC_H264_CFG_CHANGE_RANGE = UVC_MPI_CFG_CHANGE_BIT(7),
  MPI_VENC_H264_CFG_CHANGE_SEI = UVC_MPI_CFG_CHANGE_BIT(8),
  MPI_VENC_H264_CFG_CHANGE_PROFILE = UVC_MPI_CFG_CHANGE_BIT(9),
  MPI_VENC_H264_CFG_CHANGE_FBC = UVC_MPI_CFG_CHANGE_BIT(10),
  MPI_VENC_H264_CFG_CHANGE_QP = UVC_MPI_CFG_CHANGE_BIT(11),
  MPI_VENC_H264_CFG_CHANGE_ALL = (0xffffffff),
} MpiVencH264CfgChange;

typedef enum _MpiVencH265CfgChange {
  MPI_VENC_H265_CFG_CHANGE_GOP = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_VENC_H265_CFG_CHANGE_RC_MODE = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_VENC_H265_CFG_CHANGE_BPS = UVC_MPI_CFG_CHANGE_BIT(2),
  MPI_VENC_H265_CFG_CHANGE_GOP_MODE = UVC_MPI_CFG_CHANGE_BIT(3),
  MPI_VENC_H265_CFG_CHANGE_VI_LEN = UVC_MPI_CFG_CHANGE_BIT(4),
  MPI_VENC_H265_CFG_CHANGE_FPS = UVC_MPI_CFG_CHANGE_BIT(5),
  MPI_VENC_H265_CFG_CHANGE_BUF_CNT = UVC_MPI_CFG_CHANGE_BIT(6),
  MPI_VENC_H265_CFG_CHANGE_RANGE = UVC_MPI_CFG_CHANGE_BIT(7),
  MPI_VENC_H265_CFG_CHANGE_SEI = UVC_MPI_CFG_CHANGE_BIT(8),
  MPI_VENC_H265_CFG_CHANGE_PROFILE = UVC_MPI_CFG_CHANGE_BIT(9),
  MPI_VENC_H265_CFG_CHANGE_FBC = UVC_MPI_CFG_CHANGE_BIT(10),
  MPI_VENC_H265_CFG_CHANGE_QP = UVC_MPI_CFG_CHANGE_BIT(11),
  MPI_VENC_H265_CFG_CHANGE_ALL = (0xffffffff),
} MpiVencH265CfgChange;

typedef enum _MpiEptzCfgChange {
  MPI_EPTZ_CFG_CHANGE_ENABLE_EPTZ = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_EPTZ_CFG_CHANGE_ENABLE_EPTZ_BOOT = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_EPTZ_CFG_CHANGE_ENABLE_DEBUG = UVC_MPI_CFG_CHANGE_BIT(2),
  MPI_EPTZ_CFG_CHANGE_ALIGN = UVC_MPI_CFG_CHANGE_BIT(3),
  MPI_EPTZ_CFG_CHANGE_ZOOM_SPEED = UVC_MPI_CFG_CHANGE_BIT(4),
  MPI_EPTZ_CFG_CHANGE_FAST_MOVE_FRAME_JUDGE = UVC_MPI_CFG_CHANGE_BIT(5),
  MPI_EPTZ_CFG_CHANGE_ZOOM_FRAME_JUDGE = UVC_MPI_CFG_CHANGE_BIT(6),
  MPI_EPTZ_CFG_CHANGE_ITERATE_X = UVC_MPI_CFG_CHANGE_BIT(7),
  MPI_EPTZ_CFG_CHANGE_ITERATE_Y = UVC_MPI_CFG_CHANGE_BIT(8),
  MPI_EPTZ_CFG_CHANGE_RATIO = UVC_MPI_CFG_CHANGE_BIT(9),
  MPI_EPTZ_CFG_CHANGE_ALL = (0xffffffff),
} MpiEptzCfgChange;

typedef enum _MpiOsdCfgChange {
  MPI_OSD_CFG_CHANGE_ENABLE_OSD = UVC_MPI_CFG_CHANGE_BIT(0),
  MPI_OSD_CFG_CHANGE_FORCE_USE_VPSS = UVC_MPI_CFG_CHANGE_BIT(1),
  MPI_OSD_CFG_CHANGE_ALL = (0xffffffff),
} MpiOsdCfgChange;

typedef enum _MpiViChannelType {
  MPI_VI_CHANNEL_TYPE_UVC = 0,
  MPI_VI_CHANNEL_TYPE_NN = 1,
  MPI_VI_CHANNEL_TYPE_MAX = 2,
} MpiViChannelType;

typedef enum _MpiVpssChannelType {
  MPI_VPSS_CHANNEL_TYPE_UVC = 0,
  MPI_VPSS_CHANNEL_TYPE_NN = 1,
  MPI_VPSS_CHANNEL_TYPE_MAX = 2,
} MpiVpssChannelType;

typedef struct {
  RK_U32 change;
  RK_U32 channel_id;
  // idr begin
  RK_U32 idr_gop;
  RK_U32 idr_cnt;
  RK_U32 idr_bps;
  RK_U32 normal_gop;
  RK_U32 normal_bps;
  // idr end
  RK_U32 enable_vpss;
  ROTATION_E rotation;
  MIRROR_E mirror;
} MpiVencCommonCfg;

typedef struct {
  RK_U32 change;
  RK_U32 qfactor_min;
  RK_U32 qfactor_max;
  RK_U32 range_full;
  RK_U32 bps;
  RK_U32 sei;
  RK_U32 buf_cnt;
  RK_S32 fps_in;
  RK_S32 fps_out;
  COMPRESS_MODE_E fbc;
  VENC_RC_MODE_E rc_mode;
  VENC_RC_PARAM_S qp;
} MpiVencMjpegCfg;

typedef struct {
  RK_U32 change;
  RK_U32 gop;
  RK_U32 range_full;
  RK_U32 sei;
  RK_U32 profile;
  RK_U32 bps;
  RK_U32 buf_cnt;
  RK_S32 fps_in;
  RK_S32 fps_out;
  COMPRESS_MODE_E fbc;
  VENC_RC_MODE_E rc_mode;
  VENC_RC_PARAM_S qp;
  VENC_GOP_ATTR_S gop_mode;
} MpiVencH264Cfg;

typedef struct {
  RK_U32 change;
  RK_U32 gop;
  RK_U32 range_full;
  RK_U32 sei;
  RK_U32 profile;
  RK_U32 bps;
  RK_U32 buf_cnt;
  RK_S32 fps_in;
  RK_S32 fps_out;
  COMPRESS_MODE_E fbc;
  VENC_RC_MODE_E rc_mode;
  VENC_RC_PARAM_S qp;
  VENC_GOP_ATTR_S gop_mode;
} MpiVencH265Cfg;

#if 0
#define MPP_ENC_OSD_IMAGE_PATH_LEN 32
typedef struct {
    bool set_ok;
    bool enable; //dynamic on/off set this
    enum OSD_REGION_TYPE type;
    RK_U32 start_x;
    RK_U32 start_y;
    char image_path[MPP_ENC_OSD_IMAGE_PATH_LEN];//*image_path;//
// for mjpeg rga osd
    RK_U32 width;
    RK_U32 height;
    int rga_osd_fd;
    unsigned int handle; // for drm handle
    uint8_t *buffer;
    int drm_size;
// for mjpeg rga osd
} MpiEncOSDCfg;
#endif

typedef struct {
  RK_U32 version;
  MpiVencCommonCfg common_cfg;
  MpiVencMjpegCfg mjpeg_cfg;
  MpiVencH264Cfg h264_cfg;
  MpiVencH265Cfg h265_cfg;
} MpiVencCfg;

typedef struct {
  RK_U32 version;
  RK_U32 change;
  RK_U32 dev_id;
  RK_U32 channel_id;
  RK_U32 buf_cnt;
  RK_U32 assign_width;
  RK_U32 assign_height;
  RK_U32 min_width;
  RK_U32 min_height;
  RK_U32 max_width;
  RK_U32 max_height;
  RK_S32 fps_in;
  RK_S32 fps_out;
  PIXEL_FORMAT_E format;
  COMPRESS_MODE_E fbc;
  VI_V4L2_MEMORY_TYPE memory_type;
  ROTATION_E rotation;
  MIRROR_E mirror;
  RK_CHAR dev_name[MAX_VI_ENTITY_NAME_LEN];
} MpiViCfg;

typedef struct {
  RK_U32 version;
  RK_U32 change;
  RK_U32 group_id;
  RK_U32 channel_id;
  RK_U32 buf_cnt;
  RK_S32 fps_in;
  RK_S32 fps_out;
  RK_U32 assign_width;
  RK_U32 assign_height;
  PIXEL_FORMAT_E format;
  COMPRESS_MODE_E fbc;
  ROTATION_E rotation;
  MIRROR_E mirror;
} MpiVpssCfg;

typedef struct {
  RK_U32 version;
  RK_U32 change;
  RK_U32 need_check;
  RK_U32 uvc_debug_cnt;
  RK_U32 nn_debug_cnt;
  RK_U32 uvc_debug;
  RK_U32 yuyv_debug;
  RK_U32 nn_enable;
  RK_U32 geometric_output_num;
  RK_U32 geometric_output_den;
  RK_U32 uvc_enable_vpss;
  RK_U32 eptz_enable;
  RK_U32 eptz_enable_boot;
  RK_CHAR uvc_debug_file[UVC_MPI_STREAM_SAVE_FILE_LEN];
  RK_CHAR nn_debug_file[UVC_MPI_STREAM_SAVE_FILE_LEN];
} MpiUvcCommonCfg;

typedef struct {
  RK_U32 version;
  RK_U32 change;
  RK_U32 enable;
  RK_U32 enable_boot;
  RK_U32 debug;
  RK_S32 align;
  RK_S32 zoom_speed;
  RK_S32 fast_move_frame_judge;
  RK_S32 zoom_frame_judge;
  RK_S32 iterate_x;
  RK_S32 iterate_y;
  RK_FLOAT ratio;
} MpiUvcEptzCfg;

typedef struct {
  RK_U32 version;
  RK_U32 change;
  RK_U32 enable;
  RK_U32 force_use_vpss;
} MpiOsdCfg;

typedef struct {
  RK_S32 width;
  RK_S32 height;
  RK_S32 fcc;
  RK_U32 fps;
} MpiUvcCfg;

typedef struct _rkUvcMpiCfg {
  RK_U32 index;
  RK_U32 version;
  MpiUvcCfg uvc_cfg; // for uvc setting
  MpiUvcCommonCfg common_cfg;
  MpiUvcEptzCfg eptz_cfg;
  MpiOsdCfg osd_cfg;
  MpiViCfg vi_cfg[MPI_VI_CHANNEL_TYPE_MAX];
  MpiVpssCfg vpss_cfg[MPI_VPSS_CHANNEL_TYPE_MAX];
  MpiVencCfg venc_cfg;
} UVC_MPI_CFG;

int check_uvc_mpi_cfg_file_init(UVC_MPI_CFG *cfg);
void uvc_mpi_cfg_set_default(UVC_MPI_CFG *mpiCfg);

#ifdef __cplusplus
}
#endif

#endif
