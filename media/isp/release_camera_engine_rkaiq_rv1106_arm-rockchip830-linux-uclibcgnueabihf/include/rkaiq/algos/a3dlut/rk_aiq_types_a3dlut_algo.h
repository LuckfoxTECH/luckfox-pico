/*
 * rk_aiq_types_a3dlut_algo.h
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

#ifndef _RK_AIQ_TYPES_A3DLUT_ALGO_H_
#define _RK_AIQ_TYPES_A3DLUT_ALGO_H_
#include "a3dlut/rk_aiq_types_a3dlut_hw.h"

RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_lut3d_table_s{
	// M4_ARRAY_DESC("LutR", "f32", M4_SIZE(81,9), M4_RANGE(0,1023), "0", M4_DIGIT(0), M4_DYNAMIC(0))
	unsigned short look_up_table_r[LUT3D_LUT_WSIZE];//10bit
	// M4_ARRAY_DESC("LutG", "f32", M4_SIZE(81,9), M4_RANGE(0,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0))
	unsigned short look_up_table_g[LUT3D_LUT_WSIZE];//12bit
	// M4_ARRAY_DESC("LutB", "f32", M4_SIZE(81,9), M4_RANGE(0,1023), "0", M4_DIGIT(0), M4_DYNAMIC(0))
	unsigned short look_up_table_b[LUT3D_LUT_WSIZE];//10bit
} rk_aiq_lut3d_table_t;



RKAIQ_END_DECLARE

#endif
