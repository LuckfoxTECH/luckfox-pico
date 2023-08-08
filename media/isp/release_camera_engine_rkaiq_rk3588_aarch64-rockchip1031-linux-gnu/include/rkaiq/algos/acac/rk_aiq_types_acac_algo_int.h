/*
 * rk_aiq_types_acac_algo_int.h
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
#ifndef _RK_AIQ_TYPE_ACAC_ALGO_INT_H_
#define _RK_AIQ_TYPE_ACAC_ALGO_INT_H_

#include "acac/rk_aiq_types_acac_algo.h"

RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_cac_attrib_s {
    bool en;
    bool byPass;
    // TODO(Cody): Fill CAC required datas
} rk_aiq_cac_attrib_t;

typedef struct rk_aiq_cac_querry_info_s {
    bool cac_en;
    // TODO(Cody): Fill CAC required datas
} rk_aiq_cac_querry_info_t;

RKAIQ_END_DECLARE

#endif
