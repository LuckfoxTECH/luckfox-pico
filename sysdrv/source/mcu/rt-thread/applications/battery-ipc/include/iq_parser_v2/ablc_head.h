/*
 * ablc_head.h
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

#ifndef __CALIBDBV2_ABLC_HEADER_H__
#define __CALIBDBV2_ABLC_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define ABLC_MAX_ISO_LEVEL (13)
#define ABLC_MAX_CHANNEL_NUM 4


/*****************************************************************************/
/**
 * @brief   ISP2.0 ABLC Config Params
 */
/*****************************************************************************/

typedef struct Blc_data_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,13), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(0))
    float ISO[ABLC_MAX_ISO_LEVEL];
    // M4_ARRAY_DESC("R_Channel", "f32", M4_SIZE(1,13), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(0))
    float R_Channel[ABLC_MAX_ISO_LEVEL];
    // M4_ARRAY_DESC("Gr_Channel", "f32", M4_SIZE(1,13), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(0))
    float Gr_Channel[ABLC_MAX_ISO_LEVEL];
    // M4_ARRAY_DESC("Gb_Channel", "f32", M4_SIZE(1,13), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(0))
    float Gb_Channel[ABLC_MAX_ISO_LEVEL];
    // M4_ARRAY_DESC("B_Channel", "f32", M4_SIZE(1,13), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(0))
    float B_Channel[ABLC_MAX_ISO_LEVEL];
} Blc_data_t;

typedef struct AblcParaV2_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_ARRAY_TABLE_DESC("BLC_Data", "array_table_ui", "none")
    Blc_data_t BLC_Data;
} AblcParaV2_t;


typedef struct CalibDbV2_Ablc_s {
    // M4_STRUCT_DESC("BlcTuningPara", "normal_ui_style")
    AblcParaV2_t BlcTuningPara;
    // M4_STRUCT_DESC("Blc1TuningPara", "normal_ui_style")
    AblcParaV2_t Blc1TuningPara;
} CalibDbV2_Ablc_t;


RKAIQ_END_DECLARE

#endif
