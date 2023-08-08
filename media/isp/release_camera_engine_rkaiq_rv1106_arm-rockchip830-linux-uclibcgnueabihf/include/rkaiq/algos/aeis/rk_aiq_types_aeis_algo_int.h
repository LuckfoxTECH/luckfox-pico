/*
 * rk_aiq_types_aeis_algo_int.h
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
#ifndef _RK_AIQ_TYPE_AEIS_ALGO_INT_H_
#define _RK_AIQ_TYPE_AEIS_ALGO_INT_H_

#include "aeis/rk_aiq_types_aeis_algo.h"

RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_eis_attrib_s {
    bool en;
    bool byPass;
    // TODO(Cody): Fill EIS required datas
} rk_aiq_eis_attrib_t;

typedef struct rk_aiq_eis_querry_info_s {
    bool eis_en;
    // TODO(Cody): Fill EIS required datas
} rk_aiq_eis_querry_info_t;

RKAIQ_END_DECLARE

#endif

