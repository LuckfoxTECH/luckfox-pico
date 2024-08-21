/*
 * cgc_head.h
 *
 *  Copyright (c) 2022 Rockchip Corporation
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

#ifndef __CALIBDBV2_CGC_HEAD_H__
#define __CALIBDBV2_CGC_HEAD_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

typedef struct __cgc_param {
    // M4_ENUM_DESC("op_mode", "RKAiqOPMode_t", "RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t op_mode;
    // M4_BOOL_DESC("ratio_en", "0")
    bool cgc_ratio_en;
    // M4_BOOL_DESC("yuv_limit", "0")
    bool cgc_yuv_limit;
} Cgc_Param_t;

typedef struct __cgc {
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    Cgc_Param_t TuningPara;
} CalibDbV2_cgc_t;

RKAIQ_END_DECLARE

#endif
