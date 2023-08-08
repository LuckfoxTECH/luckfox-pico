#include <assert.h>
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
#ifndef __SAMPLE_COMM_ISP_H__
#define __SAMPLE_COMM_ISP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <fcntl.h>
#include <rk_aiq_user_api2_camgroup.h>
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

/*
 * stream on:
 * 1) ISP Init
 * 2) ISP Run
 * 3) VI enable and stream on
 *
 * stream off:
 * 4) VI disable
 * 5) ISP Stop
 */
/*
typedef enum {
 RK_AIQ_WORKING_MODE_NORMAL,
 RK_AIQ_WORKING_MODE_ISP_HDR2    = 0x10,
 RK_AIQ_WORKING_MODE_ISP_HDR3    = 0x20,
 //RK_AIQ_WORKING_MODE_SENSOR_HDR = 10, // sensor built-in hdr mode
} rk_aiq_working_mode_t;
*/
RK_BOOL SAMPLE_COMM_ISP_ShouldQuit();
RK_S32 SAMPLE_COMM_ISP_GetSofCnt(void);
RK_S32 SAMPLE_COMM_ISP_Init(RK_S32 CamId, rk_aiq_working_mode_t WDRMode, RK_BOOL MultiCam,
                            const char *iq_file_dir);
RK_S32
SAMPLE_COMM_ISP_CamGroup_Init(RK_S32 CamGroupId, rk_aiq_working_mode_t WDRMode,
                              bool MultiCam,
                              rk_aiq_camgroup_instance_cfg_t *pCamGroupCfg);
RK_S32 SAMPLE_COMM_ISP_Stop(RK_S32 CamId);
RK_S32 SAMPLE_COMM_ISP_Run(RK_S32 CamId); // isp stop before vi streamoff
RK_S32 SAMPLE_COMM_ISP_CamGroup_Stop(RK_S32 CamGroupId);
RK_S32 SAMPLE_COMM_ISP_SetFrameRate(RK_S32 CamId, RK_U32 uFps);
RK_S32 SAMPLE_COMM_ISP_SetMirrorFlip(int cam_id, int mirror, int flip);
RK_S32 SAMPLE_COMM_ISP_SetLDCH(RK_U32 CamId, RK_U32 u32Level, RK_BOOL bIfEnable);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __SAMPLE_COMMON_ISP_H__ */
