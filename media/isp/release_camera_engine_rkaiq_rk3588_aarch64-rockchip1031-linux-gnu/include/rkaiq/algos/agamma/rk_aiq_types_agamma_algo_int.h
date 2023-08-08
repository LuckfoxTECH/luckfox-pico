/*
 *rk_aiq_types_agamma_algo_int.h
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

#ifndef _RK_AIQ_TYPE_AGAMMA_ALGO_INT_H_
#define _RK_AIQ_TYPE_AGAMMA_ALGO_INT_H_
#include "RkAiqCalibDbTypes.h"
#include "agamma/rk_aiq_types_agamma_algo.h"
#include "agamma_head.h"
#include "agamma_uapi_head.h"

typedef enum rk_aiq_gamma_op_mode_s {
    RK_AIQ_GAMMA_MODE_OFF                     = 0,        /**< run iq gamma */
    RK_AIQ_GAMMA_MODE_MANUAL                  = 1,        /**< run manual gamma */
    RK_AIQ_GAMMA_MODE_FAST                    = 2,        /**< for tool*/
} rk_aiq_gamma_op_mode_t;

typedef struct rk_aiq_gamma_attrV21_s {
    rk_aiq_gamma_op_mode_t mode;
    Agamma_api_manualV21_t stManual;
    Agamma_api_Fast_t stFast;
}  rk_aiq_gamma_attrV21_t;

typedef struct rk_aiq_gamma_attrV30_s {
    rk_aiq_gamma_op_mode_t mode;
    Agamma_api_manualV30_t stManual;
    Agamma_api_Fast_t stFast;
}  rk_aiq_gamma_attrV30_t;

typedef struct rk_aiq_gamma_attr_s {
    rk_aiq_uapi_sync_t sync;

    rk_aiq_gamma_attrV21_t atrrV21;
    rk_aiq_gamma_attrV30_t atrrV30;
}  rk_aiq_gamma_attr_t;

typedef struct AgammaProcResV20_s {
    bool gamma_en;
    int equ_segm;
    int offset;
    int gamma_y[45];
}  AgammaProcResV20_t;

typedef struct AgammaProcResV21_s {
    bool gamma_en;
    int equ_segm;
    bool EnableDot49;
    int offset;
    int gamma_y[49];
}  AgammaProcResV21_t;

typedef struct AgammaProcRes_s {
    union {
        AgammaProcResV20_t Gamma_v20;
        AgammaProcResV21_t Gamma_v30;
    };
} AgammaProcRes_t;


#endif

