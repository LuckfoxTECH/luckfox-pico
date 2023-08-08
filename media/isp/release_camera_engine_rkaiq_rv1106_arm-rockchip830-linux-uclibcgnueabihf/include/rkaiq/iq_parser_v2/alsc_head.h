/*
 * alsc_head.h
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

#ifndef __CALIBDBV2_ALSC_HEAD_H__
#define __CALIBDBV2_ALSC_HEAD_H__

#include "rk_aiq_comm.h"
#include "alsc/rk_aiq_types_alsc_hw.h"

RKAIQ_BEGIN_DECLARE

#ifndef LSC_NAME
#define LSC_NAME        ( 32U )
#endif

#ifndef CIFISP_LSC_SIZE_TBL_SIZE
#define CIFISP_LSC_SIZE_TBL_SIZE    8
#endif
#ifndef LSC_PROFILES_NUM_MAX
#define LSC_PROFILES_NUM_MAX        5
#endif

typedef struct lsc_name_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char name[LSC_NAME];
} lsc_name_t;

typedef struct resolution_s {
    char name[LSC_NAME];
    uint32_t width;
    uint32_t height;
} resolution_t;

/**
 * @brief illumination case in different <using case>, <resolution>, <color temperature>
*/
typedef struct CalibDbV2_AlscCof_ill_s {
    // M4_NUMBER_DESC("usedForCase", "u32", M4_RANGE(0,100), "0", M4_DIGIT(0))
    uint32_t    usedForCase;
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char        name[LSC_NAME];
    // M4_ARRAY_DESC("wbGain", "f32", M4_SIZE(1,2), M4_RANGE(0,10000), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float       wbGain[2];
    // M4_STRUCT_LIST_DESC("tableUsed", M4_SIZE_DYNAMIC, "normal_ui_style")
    lsc_name_t* tableUsed;
    int         tableUsed_len;
    // M4_ARRAY_DESC("gains", "f32", M4_SIZE(1,100), M4_RANGE(0,10000), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float*      gains;
    int         gains_len;
    // M4_ARRAY_DESC("vig", "f32", M4_SIZE(1,100), M4_RANGE(0,100), "0", M4_DIGIT(2), M4_DYNAMIC(1))
    float*      vig;
    int         vig_len;
} CalibDbV2_AlscCof_ill_t;

typedef struct CalibDbV2_Lsc_Resolution_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char        name[LSC_NAME];
#if defined(ISP_HW_V20)
    // M4_ARRAY_DESC("lsc_sect_size_x", "u16", M4_SIZE(1,8), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
#elif defined(ISP_HW_V21)
    // M4_ARRAY_DESC("lsc_sect_size_x", "u16", M4_SIZE(1,8), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
#elif defined(ISP_HW_V30) || defined(ISP_HW_V32)
    // M4_ARRAY_DESC("lsc_sect_size_x", "u16", M4_SIZE(1,16), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
#else
#error "WRONG ISP_HW_VERSION, ONLY SUPPORT V20 AND V21 AND V30 NOW !"
#endif
    uint16_t    lsc_sect_size_x[LSC_SIZE_TBL_SIZE];
#if defined(ISP_HW_V20)
    // M4_ARRAY_DESC("lsc_sect_size_y", "u16", M4_SIZE(1,8), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
#elif defined(ISP_HW_V21)
    // M4_ARRAY_DESC("lsc_sect_size_y", "u16", M4_SIZE(1,8), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
#elif defined(ISP_HW_V30) || defined(ISP_HW_V32)
    // M4_ARRAY_DESC("lsc_sect_size_y", "u16", M4_SIZE(1,16), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
#else
#error "WRONG ISP_HW_VERSION, ONLY SUPPORT V20 AND V21 AND V30 NOW !"
#endif
    uint16_t    lsc_sect_size_y[LSC_SIZE_TBL_SIZE];
} CalibDbV2_Lsc_Resolution_t;

typedef struct CalibDbV2_Lsc_Common_s {
    // M4_BOOL_DESC("enable", "1")
    bool                            enable;
    // M4_STRUCT_LIST_DESC("resolutionAll", M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_Lsc_Resolution_t*     resolutionAll;
    int                             resolutionAll_len;
} CalibDbV2_Lsc_Common_t;

typedef struct CalibDbV2_AlscCof_s {
    // M4_BOOL_DESC("damp_enable", "1")
    bool                            damp_enable;
    // M4_STRUCT_LIST_DESC("illAll", M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_AlscCof_ill_t*        illAll;
    int                             illAll_len;
} CalibDbV2_AlscCof_t;

//represent a cell of tableAll in xml/json
typedef struct CalibDbV2_LscTableProfile_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char        name[LSC_NAME];
    // M4_STRING_DESC("resolution", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char        resolution[LSC_NAME];
    // M4_STRING_DESC("illumination", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char        illumination[LSC_NAME];
    // M4_NUMBER_DESC("vignetting", "f32", M4_RANGE(0,100), "100", M4_DIGIT(0))
    float       vignetting;

    // M4_STRUCT_DESC("lsc_samples_red", "normal_ui_style")
    Cam17x17UShortMatrix_t  lsc_samples_red;
    // M4_STRUCT_DESC("lsc_samples_greenR", "normal_ui_style")
    Cam17x17UShortMatrix_t  lsc_samples_greenR;
    // M4_STRUCT_DESC("lsc_samples_greenB", "normal_ui_style")
    Cam17x17UShortMatrix_t  lsc_samples_greenB;
    // M4_STRUCT_DESC("lsc_samples_blue", "normal_ui_style")
    Cam17x17UShortMatrix_t  lsc_samples_blue;
} CalibDbV2_LscTableProfile_t;

typedef struct CalibDbV2_LscTable {
    // M4_STRUCT_LIST_DESC("tableAll", M4_SIZE_DYNAMIC, "normal_ui_style")
    CalibDbV2_LscTableProfile_t*    tableAll;
    int                             tableAll_len;
} CalibDbV2_LscTable_t;

typedef struct CalibDb_LscV2_s {
    // M4_STRUCT_DESC("common", "normal_ui_style")
    CalibDbV2_Lsc_Common_t          common;
    // M4_STRUCT_DESC("alscCoef", "normal_ui_style")
    CalibDbV2_AlscCof_t             alscCoef;
    // M4_STRUCT_DESC("table_list", "normal_ui_style")
    CalibDbV2_LscTable_t            tbl;
} CalibDbV2_LSC_t;

RKAIQ_END_DECLARE

#endif
