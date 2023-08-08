/*
 * rk_aiq_types_accm_algo.h
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

#ifndef _RK_AIQ_TYPES_ACCM_ALGO_H_
#define _RK_AIQ_TYPES_ACCM_ALGO_H_

#include "accm/rk_aiq_types_accm_hw.h"

RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_ccm_matrix_s {
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "[1.0000,0.0000,0.0000,0.0000,1.0000,0.0000,0.0000,0.0000,1.0000]", M4_DIGIT(4), M4_DYNAMIC(0))
    float  ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float  ccOffsets[3];
} rk_aiq_ccm_matrix_t;


RKAIQ_END_DECLARE

#endif

