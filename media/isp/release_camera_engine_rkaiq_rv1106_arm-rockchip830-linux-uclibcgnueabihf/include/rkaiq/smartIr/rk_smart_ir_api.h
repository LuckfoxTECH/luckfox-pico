/*
 *  Copyright (c) 2022 Rockchip Corporation
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

#ifndef _RK_AIQ_SMART_IR_API_H_
#define _RK_AIQ_SMART_IR_API_H_

#include "common/rk_aiq_types.h"

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;
typedef struct rk_smart_ir_ctx_s rk_smart_ir_ctx_t;

RKAIQ_BEGIN_DECLARE

typedef enum RK_SMART_IR_STATUS_E {
    RK_SMART_IR_STATUS_DAY,
    RK_SMART_IR_STATUS_NIGHT,
} RK_SMART_IR_STATUS_t;

typedef struct rk_smart_ir_params_s {
    float d2n_envL_th;
    float n2d_envL_th;
    float rggain_base;
    float bggain_base;
    float awbgain_rad;
    float awbgain_dis;
    uint16_t switch_cnts_th;
} rk_smart_ir_params_t;

typedef struct rk_smart_ir_result_s {
    RK_SMART_IR_STATUS_t status;
} rk_smart_ir_result_t;

rk_smart_ir_ctx_t*
rk_smart_ir_init(const rk_aiq_sys_ctx_t* ctx);

XCamReturn
rk_smart_ir_deInit(const rk_smart_ir_ctx_t* ir_ctx);

XCamReturn
rk_smart_ir_config(rk_smart_ir_ctx_t* ctx, rk_smart_ir_params_t* config);

XCamReturn
rk_smart_ir_runOnce(rk_smart_ir_ctx_t* ctx, rk_aiq_isp_stats_t* stats_ref, rk_smart_ir_result_t* result);

RKAIQ_END_DECLARE

#endif
