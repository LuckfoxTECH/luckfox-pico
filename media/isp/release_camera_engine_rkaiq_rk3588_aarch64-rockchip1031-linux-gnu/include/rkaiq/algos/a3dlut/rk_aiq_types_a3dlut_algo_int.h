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
#include "rk_aiq_types_a3dlut_ext.h"
RKAIQ_BEGIN_DECLARE

typedef struct alut3d_sw_info_s {
    float sensorGain;
    float awbGain[2];
    float awbIIRDampCoef;
    bool awbConverged;
    bool lut3dConverged;
    uint8_t invarMode;
    uint32_t count;
} alut3d_sw_info_t;


RKAIQ_END_DECLARE

#endif
