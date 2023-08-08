/*
 * adegamma_head.h
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

#ifndef __CALIBDBV2_ADEGAMMA_HEAD_H__
#define __CALIBDBV2_ADEGAMMA_HEAD_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define ADEGAMMA_CRUVE_KNOTS 17

/*****************************************************************************/
/**
 * @brief   ISP2.0 Adegamma Config Params
 */
/*****************************************************************************/

typedef struct AdegmmaParaV2_s {
    // M4_BOOL_DESC("degamma_en", "0")
    bool degamma_en;
    // M4_ARRAY_DESC("X_axis", "u16", M4_SIZE(1,17),  M4_RANGE(0, 4095), "[0,256,512,768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840,4096]", M4_DIGIT(0), M4_DYNAMIC(0))
    int X_axis[ADEGAMMA_CRUVE_KNOTS];
    // M4_ARRAY_DESC("curve_R", "u16", M4_SIZE(1,17),  M4_RANGE(0, 4095), "[0,256,512,768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840,4096]", M4_DIGIT(0), M4_DYNAMIC(0))
    int curve_R[ADEGAMMA_CRUVE_KNOTS];
    // M4_ARRAY_DESC("curve_G", "u16", M4_SIZE(1,17),  M4_RANGE(0, 4095), "[0,256,512,768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840,4096]", M4_DIGIT(0), M4_DYNAMIC(0))
    int curve_G[ADEGAMMA_CRUVE_KNOTS];
    // M4_ARRAY_DESC("curve_B", "u16", M4_SIZE(1,17),  M4_RANGE(0, 4095), "[0,256,512,768,1024,1280,1536,1792,2048,2304,2560,2816,3072,3328,3584,3840,4096]", M4_DIGIT(0), M4_DYNAMIC(0))
    int curve_B[ADEGAMMA_CRUVE_KNOTS];
} AdegmmaParaV2_t;

typedef struct CalibDbV2_Adegmma_s {
    // M4_STRUCT_DESC("DegammaTuningPara", "normal_ui_style")
    AdegmmaParaV2_t DegammaTuningPara;
} CalibDbV2_Adegmma_t;


RKAIQ_END_DECLARE

#endif
