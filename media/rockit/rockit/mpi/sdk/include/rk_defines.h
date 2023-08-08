/*
 * Copyright 2021 Rockchip Electronics Co. LTD
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
 *
 */

#ifndef INCLUDE_RT_MPI_RK_DEFINES_H_
#define INCLUDE_RT_MPI_RK_DEFINES_H_

/* Automatic generated [rv1106] config */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* For VENC */
#define VENC_MAX_CHN_NUM 64
#define VENC_MAX_ROI_NUM 8

/* For VDEC */
#define VDEC_MAX_CHN_NUM 8

/* For Region */
#define RGN_MAX_HANDLE_NUM 128

/* For VI */
/* number of channle and device on video input unit of chip
 * Note! VI_MAX_CHN_NUM is NOT equal to VI_MAX_DEV_NUM
 * multiplied by VI_MAX_CHN_NUM, because all VI devices
 * can't work at mode of 4 channles at the same time.
 */
#define VI_MAX_DEV_NUM 2
#define VI_MAX_PHY_PIPE_NUM VI_MAX_DEV_NUM
#define VI_MAX_VIR_PIPE_NUM 0
#define VI_MAX_PIPE_NUM (VI_MAX_PHY_PIPE_NUM + VI_MAX_VIR_PIPE_NUM)
#define VI_MAX_PHY_CHN_NUM 6
#define VI_MAX_EXT_CHN_NUM 0
#define VI_MAX_CHN_NUM (VI_MAX_PHY_CHN_NUM + VI_MAX_EXT_CHN_NUM)

/* For VO */
#define VO_SPLICE_DEVICE_TYPE 1
#define VO_MAX_DEV_NUM 1   /* max dev num */
#define VO_MAX_LAYER_NUM 1 /* max layer num */
#define VO_MAX_CHN_NUM 128 /* max chn num */
#define VO_MAX_WBC_NUM 0
#define VO_MAX_BUF_NUM 15
#define VO_MAX_PRIORITY 7    /* max layer priority */
#define VO_MAX_PHY_DEV_NUM 1 /* max physical dev num */

/* For AUDIO */
#define AI_DEV_MAX_NUM 2
#define AI_MAX_CHN_NUM 1
#define AO_DEV_MAX_NUM 2
#define AO_MAX_CHN_NUM 3
#define AENC_MAX_CHN_NUM 32
#define ADEC_MAX_CHN_NUM 32
#define AF_MAX_CHN_NUM 8

/* For VPSS */
#define VPSS_MAX_GRP_NUM 256
#define VPSS_MAX_CHN_NUM 4
#define VPSS_MIN_IMAGE_WIDTH 64
#define VPSS_MIN_IMAGE_HEIGHT 64
#define VPSS_MAX_IMAGE_WIDTH 4096
#define VPSS_MAX_IMAGE_HEIGHT 4096
#define VPSS_VIDEO_PROC_DEVICE_TYPE 1

/* For VGS */
#define VGS_MAX_JOB_NUM 128
#define VGS_MAX_TASK_NUM 200

/* For TDE */
#define TDE_MAX_JOB_NUM 128
#define TDE_MAX_TASK_NUM 200

/* For IVS */
#define IVS_MAX_CHN_NUM 32

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_DEFINES_H_ */
