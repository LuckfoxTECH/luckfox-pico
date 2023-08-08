/*
 *rk_aiq_types_adehaze_hw.h
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

#ifndef _RK_AIQ_TYPE_ADEHAZE_HW_H_
#define _RK_AIQ_TYPE_ADEHAZE_HW_H_

#include "rk_aiq_comm.h"



RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_dehaze_cfg_s
{
    float  dehaze_en         [5];
    float dehaze_self_adp   [7];
    float dehaze_range_adj  [6];
    float dehaze_hist_para  [4];
    float  dehaze_enhance    [4];
    float dehaze_iir_control[5];
    float dehaze_user_config[5];
    float dehaze_bi_para    [4];
    float dehaze_dc_bf_h   [25];
    float dehaze_air_bf_h   [9];
    float dehaze_gaus_h     [9];
    float dehaze_hist_t0    [6];
    float dehaze_hist_t1    [6];
    float dehaze_hist_t2    [6];
} rk_aiq_dehaze_cfg_t;


RKAIQ_END_DECLARE

#endif

