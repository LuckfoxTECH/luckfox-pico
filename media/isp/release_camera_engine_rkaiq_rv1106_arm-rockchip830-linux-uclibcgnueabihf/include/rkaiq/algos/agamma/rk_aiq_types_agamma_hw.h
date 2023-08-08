/*
 * rk_aiq_algo_agamma_hw.h
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
#include "agamma_head.h"

#ifndef _RK_AIQ_ALGO_AGAMMA_HW_H_
#define _RK_AIQ_ALGO_AGAMMA_HW_H_

typedef struct AgammaProcResV10_s {
    bool gamma_en;
    int equ_segm;
    int offset;
    int gamma_y[CALIBDB_AGAMMA_KNOTS_NUM_V10];
} AgammaProcResV10_t;

typedef struct AgammaProcResV11_s {
    bool gamma_en;
    int equ_segm;
    bool EnableDot49;
    int offset;
    int gamma_y[CALIBDB_AGAMMA_KNOTS_NUM_V11];
} AgammaProcResV11_t;

typedef struct AgammaProcRes_s {
    bool update;
#if RKAIQ_HAVE_GAMMA_V10
    AgammaProcResV10_t Gamma_v10;
#endif
#if RKAIQ_HAVE_GAMMA_V11
    AgammaProcResV11_t Gamma_v11;
#endif
} AgammaProcRes_t;

#endif  //_RK_AIQ_ALGO_AGAMMA_HW_H_
