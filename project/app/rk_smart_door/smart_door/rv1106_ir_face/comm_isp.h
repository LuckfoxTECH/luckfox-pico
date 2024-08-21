// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __COMM_ISP_H__
#define __COMM_ISP_H__

#include <stdbool.h>
#include "rk_type.h"
#include "rk_aiq_comm.h"
#include "uAPI2/rk_aiq_user_api2_camgroup.h"

RK_S32 SAMPLE_COMM_ISP_Init(RK_S32 CamId, rk_aiq_working_mode_t WDRMode, RK_BOOL MultiCam,
			    const char *iq_file_dir);
RK_S32 SAMPLE_COMM_ISP_Run(RK_S32 CamId);
RK_S32 SAMPLE_COMM_ISP_CamGroup_Init(RK_S32 CamGroupId, rk_aiq_working_mode_t WDRMode,
				     bool MultiCam, int OpenLdch, void *LdchMesh[],
				     rk_aiq_camgroup_instance_cfg_t *pCamGroupCfg);

RK_S32 SAMPLE_COMM_ISP_Stop(RK_S32 CamId);
RK_S32 SAMPLE_COMM_ISP_CamGroup_Stop(RK_S32 CamGroupId);

#endif