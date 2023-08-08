/*
 * sys_static_cfg_head.h
 *
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef __CALIBDBV2_SYS_STATIC_CFG_HEADER_H__
#define __CALIBDBV2_SYS_STATIC_CFG_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

typedef enum EnableAlgoType_e {
    ENABLE_AE = 0x0,
    ENABLE_AWB,
    ENABLE_AF,
    ENABLE_ABLC,
    ENABLE_ADPCC,
    ENABLE_AMERGE,
    ENABLE_ATMO,
    ENABLE_ANR,
    ENABLE_ALSC,
    ENABLE_AGIC,
    ENABLE_ADEBAYER,
    ENABLE_ACCM,
    ENABLE_AGAMMA,
    ENABLE_AWDR,
    ENABLE_ADHAZ,
    ENABLE_A3DLUT,
    ENABLE_ALDCH,
    ENABLE_AR2Y,
    ENABLE_ACP,
    ENABLE_AIE,
    ENABLE_ASHARP,
    ENABLE_AORB,
    ENABLE_ACGC,
    ENABLE_ASD,
    ENABLE_ADRC,
    ENABLE_ADEGAMMA,

    ENABLE_ARAWNR,
    ENABLE_AMFNR,
    ENABLE_AYNR,
    ENABLE_ACNR,
    ENABLE_AEIS,
    ENABLE_AFEC,
    ENABLE_AMD,
    ENABLE_AGAIN
} EnableAlgoType_t;

typedef struct CalibDb_AlgoSwitch_s {
    // M4_BOOL_DESC("enable", "0")
    bool                enable;

    // M4_STRING_DESC("EnableAlgos", M4_SIZE(1,1), M4_RANGE(0, 32), "ENABLE_AE", M4_DYNAMIC(1))
    EnableAlgoType_t*   enable_algos;
    int16_t             enable_algos_len;
} CalibDb_AlgoSwitch_t;

typedef struct CalibDb_SysStaticCfg_ParaV2_s {
    // M4_STRUCT_DESC("AlgoSwitch", "normal_ui_style")
    CalibDb_AlgoSwitch_t algoSwitch;
} CalibDb_SysStaticCfg_ParaV2_t;

RKAIQ_END_DECLARE

#endif
