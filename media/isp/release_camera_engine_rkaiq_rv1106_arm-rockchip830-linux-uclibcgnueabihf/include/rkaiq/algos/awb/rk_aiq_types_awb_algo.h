/*
* rk_aiq_types_awb_algo.h

* for rockchip v2.0.0
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
/* for rockchip v2.0.0*/

#ifndef __RK_AIQ_TYPE_AWB_ALGO_H__
#define __RK_AIQ_TYPE_AWB_ALGO_H__

#include "rk_aiq_types_awb_stat_v200.h"
#include "rk_aiq_types_awb_stat_v201.h"
#include "rk_aiq_types_awb_stat_v32.h"
typedef struct rk_aiq_wb_gain_s {
    float rgain;
    float grgain;
    float gbgain;
    float bgain;
} rk_aiq_wb_gain_t;

typedef enum rk_aiq_wb_gapin_aplly_pos_s{
    IN_AWBGAIN0 =0,
    IN_AWBGAIN1
}rk_aiq_wb_gapin_aplly_pos_e;

typedef struct rk_aiq_wb_gain_v32_s {
    float rgain;
    float grgain;
    float gbgain;
    float bgain;
    rk_aiq_wb_gapin_aplly_pos_e applyPosition;
} rk_aiq_wb_gain_v32_t;

#endif

