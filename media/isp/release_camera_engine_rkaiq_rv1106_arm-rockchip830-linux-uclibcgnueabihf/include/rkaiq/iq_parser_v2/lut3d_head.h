/*
 * lut3d_head.h
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

#ifndef __CALIBDBV2_LUT3D_HEADER_H__
#define __CALIBDBV2_LUT3D_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

/*****************************************************************************/
/**
 * @brief   Global 3DLUT calibration structure of isp2.0
 */
/*****************************************************************************/
typedef struct CalibDbV2_Lut3D_Table_Para_s {
    // M4_ARRAY_DESC("Lut R", "u16", M4_SIZE(81,9), M4_RANGE(0,4096), "0", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned short look_up_table_r[729];
    // M4_ARRAY_DESC("Lut G", "u16", M4_SIZE(81,9), M4_RANGE(0,4096), "0", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned short look_up_table_g[729];
    // M4_ARRAY_DESC("Lut B", "u16", M4_SIZE(81,9), M4_RANGE(0,4096), "0", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned short look_up_table_b[729];
}CalibDbV2_Lut3D_Table_Para_t;

typedef struct CalibDbV2_Lut3D_Gain_Alp_s {
    // M4_ARRAY_DESC("gain", "f32", M4_SIZE(1,9), M4_RANGE(0,1024), "[1,2,4,8,16,32,64,128,256]", M4_DIGIT(0), M4_DYNAMIC(0))
    float                                                       gain[9];
    // M4_ARRAY_DESC("Alpha", "f32", M4_SIZE(1,9), M4_RANGE(0,1), "1.00", M4_DIGIT(2), M4_DYNAMIC(0))
    float                                                       alpha[9];
} CalibDbV2_Lut3D_Gain_Alp_t;

typedef struct CalibDbV2_Lut3D_LutPara_s {
    //  M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 25), "Normal",M4_DYNAMIC(0))
    char *name;
     // M4_ARRAY_DESC("awbGain", "f32", M4_SIZE(1,2), M4_RANGE(0,5), "[1.0000,1.0000]", M4_DIGIT(4), M4_DYNAMIC(0))
    float     awbGain[2];
    // M4_ARRAY_TABLE_DESC("gain-alpha curve", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDbV2_Lut3D_Gain_Alp_t gain_alpha;
    // M4_STRUCT_DESC("Table", "normal_ui_style")
    CalibDbV2_Lut3D_Table_Para_t Table;
}CalibDbV2_Lut3D_LutPara_t;

typedef struct CalibDbV2_Lut3D_Auto_Para_s {
    // M4_BOOL_DESC("damp enable", "0")
    bool damp_en;
    // M4_STRUCT_LIST_DESC("lutAll", M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_Lut3D_LutPara_t*        lutAll;
    int lutAll_len;
}CalibDbV2_Lut3D_Auto_Para_t;

typedef struct CalibDbV2_Lut3D_Manual_Para_s {
    // M4_STRUCT_DESC("Table", "normal_ui_style")
    CalibDbV2_Lut3D_Table_Para_t Table;
}CalibDbV2_Lut3D_Manual_Para_t;

typedef struct CalibDbV2_Lut3D_Common_Para_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
    // M4_NUMBER_DESC("gain tolerance", "f32", M4_RANGE(0.0, 256), "0.2", M4_DIGIT(4))
    float gain_tolerance;
}CalibDbV2_Lut3D_Common_Para_t;

typedef struct {
    // M4_STRUCT_DESC("common", "normal_ui_style")
    CalibDbV2_Lut3D_Common_Para_t common;
    // M4_STRUCT_DESC("autoLut3D", "normal_ui_style")
    CalibDbV2_Lut3D_Auto_Para_t ALut3D;
} CalibDbV2_Lut3D_Para_V2_t;

RKAIQ_END_DECLARE

#endif

