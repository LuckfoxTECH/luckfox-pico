/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
 */
#ifndef __CALIBDBV2_ABLC_HEADER_V32_H__
#define __CALIBDBV2_ABLC_HEADER_V32_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define ABLCV32_MAX_ISO_LEVEL   13
#define ABLCV32_MAX_CHANNEL_NUM 4

/*****************************************************************************/
/**
 * @brief   ISP3.2 ABLC Config Params
 */
/*****************************************************************************/

typedef struct Blc_data_V32_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("R_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* R_Channel;
    int R_Channel_len;
    // M4_ARRAY_DESC("Gr_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* Gr_Channel;
    int Gr_Channel_len;
    // M4_ARRAY_DESC("Gb_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* Gb_Channel;
    int Gb_Channel_len;
    // M4_ARRAY_DESC("B_Channel", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* B_Channel;
    int B_Channel_len;
} Blc_data_V32_t;

typedef struct AblcV32ParaV2_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_ARRAY_TABLE_DESC("BLC_Data", "array_table_ui", "none")
    Blc_data_V32_t BLC_Data;
} AblcV32ParaV2_t;

typedef struct Blc_ob_dataV32_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("isp_ob_Offset", "f32", M4_SIZE(1,100), M4_RANGE(0,511), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* isp_ob_Offset;
    int isp_ob_Offset_len;
    // M4_ARRAY_DESC("isp_ob_preDgain", "f32", M4_SIZE(1,100), M4_RANGE(0,256), "1",M4_DIGIT(2), M4_DYNAMIC(1))
    float* isp_ob_preDgain;
    int isp_ob_preDgain_len;
} Blc_ob_dataV32_t;

typedef struct AblcV32OBPara_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_ARRAY_TABLE_DESC("BLC_OB_Data", "array_table_ui", "none")
    Blc_ob_dataV32_t BLC_OB_Data;
} AblcV32OBPara_t;

typedef struct CalibDbV2_Blc_V32_s {
    // M4_STRUCT_DESC("Blc0TuningPara", "normal_ui_style")
    AblcV32ParaV2_t Blc0TuningPara;
    // M4_STRUCT_DESC("Blc1TuningPara", "normal_ui_style")
    AblcV32ParaV2_t Blc1TuningPara;
    // M4_STRUCT_DESC("BlcObPara", "normal_ui_style")
    AblcV32OBPara_t BlcObPara;
} CalibDbV2_Blc_V32_t;

RKAIQ_END_DECLARE

#endif
