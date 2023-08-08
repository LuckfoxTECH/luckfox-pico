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
#include "agamma/rk_aiq_types_agamma_algo.h"
#include "RkAiqCalibDbTypes.h"




RKAIQ_BEGIN_DECLARE

typedef enum rk_gamma_curve_type_s {
    RK_GAMMA_CURVE_TYPE_DEFUALT                     = 0,        /**< USE IQ GAMMA CURVE */
    RK_GAMMA_CURVE_TYPE_SRGB                        = 1,        /**< USE GAMMA 2.2 */
    RK_GAMMA_CURVE_TYPE_HDR                         = 2,        /**< USE GAMMA HDR DEFAULT */
    RK_GAMMA_CURVE_TYPE_USER_DEFINE1                = 3,        /**< USE USER DEFINE GAMMA, 2 PARAS DEFINE*/
    RK_GAMMA_CURVE_TYPE_USER_DEFINE2                = 4,        /**< USE USER DEFINE GAMMA, 45 DOT GAMMA*/
} rk_gamma_curve_type_t;


enum {
    GAMMA_OUT_LOG_SEGMENT = 0,
    GAMMA_OUT_EQ_SEGMENT = 1
};

enum {
    GAMMA_OUT_NORMAL = 0,
    GAMMA_OUT_HDR = 1,
    GAMMA_OUT_NIGHT = 2,
    GAMMA_OUT_USR1 = 3,
    GAMMA_OUT_USR2 = 4
};

typedef enum rk_aiq_gamma_op_mode_s {
    RK_AIQ_GAMMA_MODE_OFF                     = 0,        /**< run iq gamma */
    RK_AIQ_GAMMA_MODE_MANUAL                  = 1,        /**< run manual gamma */
    RK_AIQ_GAMMA_MODE_TOOL                    = 2,        /**< for tool*/
} rk_aiq_gamma_op_mode_t;

typedef struct rk_gamma_curve_usr_define1_para_s {
    float coef1;
    float coef2;
}  rk_gamma_curve_usr_define1_para_t;

typedef struct rk_gamma_curve_usr_define2_para_s {
    int gamma_out_segnum;//0:log segment ; 1:equal segment ;
    int gamma_out_offset;
    int gamma_table[45];
}  rk_gamma_curve_usr_define2_para_t;

typedef struct Agamma_api_manual_s {
    bool en;
    rk_gamma_curve_type_t CurveType;
    rk_gamma_curve_usr_define1_para_t user1;
    rk_gamma_curve_usr_define2_para_t user2;
}  Agamma_api_manual_t;

typedef struct rk_aiq_gamma_cfg_s {
    bool gamma_en;
    int gamma_out_segnum;//0:log segment ; 1:equal segment ;
    int gamma_out_offset;
    int gamma_table[45];
}  rk_aiq_gamma_cfg_t;

typedef struct rk_aiq_gamma_attr_s {
    rk_aiq_gamma_op_mode_t mode;
    Agamma_api_manual_t stManual;
    CalibDb_Gamma_t stTool;
    int Scene_mode;
}  rk_aiq_gamma_attr_t;

typedef struct AgammaProcRes_s {
    bool gamma_en;
    int equ_segm;
    int offset;
    int gamma_y[45];
}  AgammaProcRes_t;


RKAIQ_END_DECLARE

#endif

