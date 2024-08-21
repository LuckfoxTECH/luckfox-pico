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

typedef enum DisableAlgoType_e {
    DISABLE_AE = 0x0,
    DISABLE_AWB,
    DISABLE_AF,
    DISABLE_ABLC,
    DISABLE_ADPCC,
    DISABLE_AMERGE,
    DISABLE_ATMO,
    DISABLE_ANR,
    DISABLE_ALSC,
    DISABLE_AGIC,
    DISABLE_ADEBAYER,
    DISABLE_ACCM,
    DISABLE_AGAMMA,
    DISABLE_AWDR,
    DISABLE_ADHAZ,
    DISABLE_A3DLUT,
    DISABLE_ALDCH,
    DISABLE_ACSM,
    DISABLE_ACP,
    DISABLE_AIE,
    DISABLE_ASHARP,
    DISABLE_AORB,
    DISABLE_ACGC,
    DISABLE_ASD,
    DISABLE_ADRC,
    DISABLE_ADEGAMMA,

    DISABLE_ARAWNR,
    DISABLE_AMFNR,
    DISABLE_AYNR,
    DISABLE_ACNR,
    DISABLE_AEIS,
    DISABLE_AFEC,
    DISABLE_AMD,
    DISABLE_AGAIN,
    DISABLE_ACAC,
    DISABLE_AFD
} DisableAlgoType_t;

typedef struct CalibDb_AlgoSwitch_s {
    // M4_BOOL_DESC("enable", "0")
    bool                enable;

    // M4_STRING_DESC("DisableAlgos", M4_SIZE(1,1), M4_RANGE(0, 32), "DISABLE_AF", M4_DYNAMIC(1))
    DisableAlgoType_t*   disable_algos;
    int16_t             disable_algos_len;
} CalibDb_AlgoSwitch_t;

typedef struct CalibDb_SysStaticCfg_ParaV2_s {
    // M4_STRUCT_DESC("AlgoSwitch", "normal_ui_style")
    CalibDb_AlgoSwitch_t algoSwitch;
} CalibDb_SysStaticCfg_ParaV2_t;

RKAIQ_END_DECLARE

#endif
