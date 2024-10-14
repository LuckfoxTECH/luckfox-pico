/*
 * rk_aiq_types_a3dlut_ext.h
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

#ifndef _RK_AIQ_TYPES_A3DLUT_EXT_H_
#define _RK_AIQ_TYPES_A3DLUT_EXT_H_

RKAIQ_BEGIN_DECLARE


typedef struct rk_aiq_lut3d_mlut3d_attrib_s {
    // M4_ARRAY_DESC("LutR", "f32", M4_SIZE(81,9), M4_RANGE(0,1023), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short look_up_table_r[729];//10bit
    // M4_ARRAY_DESC("LutG", "f32", M4_SIZE(81,9), M4_RANGE(0,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short look_up_table_g[729];//12bit
    // M4_ARRAY_DESC("LutB", "f32", M4_SIZE(81,9), M4_RANGE(0,1023), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short look_up_table_b[729];//10bit
} rk_aiq_lut3d_mlut3d_attrib_t;

typedef enum rk_aiq_lut3d_op_mode_s {
    RK_AIQ_LUT3D_MODE_INVALID                     = 0,        /**< initialization value */
    RK_AIQ_LUT3D_MODE_MANUAL                      = 1,        /**< run manual lens shading correction */
    RK_AIQ_LUT3D_MODE_AUTO                        = 2,        /**< run auto lens shading correction */
    RK_AIQ_LUT3D_MODE_MAX
} rk_aiq_lut3d_op_mode_t;


typedef struct rk_aiq_lut3d_attrib_s {
    rk_aiq_uapi_sync_t sync;
    // M4_BOOL_DESC("bypass", "1")
    bool byPass;
    rk_aiq_lut3d_op_mode_t mode;
    // M4_STRUCT_DESC("manual lut", "normal_ui_style")
    rk_aiq_lut3d_mlut3d_attrib_t stManual;
} rk_aiq_lut3d_attrib_t;

typedef struct rk_aiq_lut3d_querry_info_s {
    bool lut3d_en;
    // M4_NUMBER_DESC("alpha", "f32", M4_RANGE(0,1), "0", M4_DIGIT(2))
    float alpha;
    // M4_STRING_DESC("Lut name", M4_SIZE(1,1), M4_RANGE(0, 25), "Normal",M4_DYNAMIC(0))
    char name[25];
    // M4_ARRAY_DESC("LutR", "f32", M4_SIZE(81,9), M4_RANGE(0,1023), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short look_up_table_r[729];//10bit
    // M4_ARRAY_DESC("LutG", "f32", M4_SIZE(81,9), M4_RANGE(0,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short look_up_table_g[729];//12bit
    // M4_ARRAY_DESC("LutB", "f32", M4_SIZE(81,9), M4_RANGE(0,1023), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short look_up_table_b[729];//10bit
} rk_aiq_lut3d_querry_info_t;


RKAIQ_END_DECLARE

#endif
