/*
 * rk_aiq_types_a3dlut_algo_int.h
 *
 *  Copyright (c) 2019 Rockchip Corporation
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

#ifndef _RK_AIQ_TYPES_A3DLUT_ALGO_INT_H_
#define _RK_AIQ_TYPES_A3DLUT_ALGO_INT_H_
#include "rk_aiq_types_a3dlut_algo.h"
RKAIQ_BEGIN_DECLARE

//to do  add api common struct
typedef struct rk_aiq_lut3d_mlut3d_attrib_s {
    unsigned short look_up_table_r[729];//10bit
    unsigned short look_up_table_g[729];//12bit
    unsigned short look_up_table_b[729];//10bit
} rk_aiq_lut3d_mlut3d_attrib_t;

typedef struct rk_aiq_lut3d_alut3d_attrib_s {
    float alpha;
}rk_aiq_lut3d_alut3d_attrib_t;


typedef enum rk_aiq_lut3d_op_mode_s {
    RK_AIQ_LUT3D_MODE_INVALID                     = 0,        /**< initialization value */
    RK_AIQ_LUT3D_MODE_MANUAL                      = 1,        /**< run manual lens shading correction */
    RK_AIQ_LUT3D_MODE_AUTO                        = 2,        /**< run auto lens shading correction */
    RK_AIQ_LUT3D_MODE_MAX
} rk_aiq_lut3d_op_mode_t;


typedef struct rk_aiq_lut3d_attrib_s {
    bool byPass;
    rk_aiq_lut3d_op_mode_t mode;
    rk_aiq_lut3d_alut3d_attrib_t stAuto;
    rk_aiq_lut3d_mlut3d_attrib_t stManual;

} rk_aiq_lut3d_attrib_t;

typedef struct rk_aiq_lut3d_querry_info_s {
    bool lut3d_en;
    unsigned short look_up_table_r[729];//10bit
    unsigned short look_up_table_g[729];//12bit
    unsigned short look_up_table_b[729];//10bit
} rk_aiq_lut3d_querry_info_t;


RKAIQ_END_DECLARE

#endif
