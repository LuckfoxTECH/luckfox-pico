/*
 *rk_aiq_types_adehaze_algo_int.h
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

#ifndef _RK_AIQ_TYPE_ADEHAZE_ALGO_INT_H_
#define _RK_AIQ_TYPE_ADEHAZE_ALGO_INT_H_
#include "RkAiqCalibDbTypes.h"
#include "adehaze/rk_aiq_types_adehaze_algo.h"


RKAIQ_BEGIN_DECLARE


#define RK_DEHAZE_ISO_NUM 9
#define FUNCTION_ENABLE 1
#define FUNCTION_DISABLE 0

#define LIMIT_VALUE(value,max_value,min_value)      (value > max_value? max_value : value < min_value ? min_value : value)



enum {
    DEHAZE_NORMAL = 0,
    DEHAZE_HDR = 1,
    DEHAZE_NIGHT = 2
};

typedef struct RKAiqAdhazConfig_s
{
    float dehaze_en         [5];
    float dehaze_self_adp   [7]; //-
    float dehaze_range_adj  [6];
    float dehaze_hist_para  [4];
    float dehaze_enhance    [4];
    float dehaze_iir_control[5];
    float dehaze_user_config[5];
    float dehaze_bi_para    [4]; //-
    float dehaze_dc_bf_h   [25];
    float dehaze_air_bf_h   [9];
    float dehaze_gaus_h     [9];
    float dehaze_hist_t0    [6]; //-
    float dehaze_hist_t1    [6]; //-
    float dehaze_hist_t2    [6]; //-
} RKAiqAdhazConfig_t;

typedef struct rk_aiq_dehaze_M_attrib_s {
    int strength;
} rk_aiq_dehaze_M_attrib_t;

typedef enum rk_aiq_dehaze_op_mode_s {
    RK_AIQ_DEHAZE_MODE_INVALID                     = 0,        /**< invalid mode, run iq file */
    RK_AIQ_DEHAZE_MODE_MANUAL                      = 1,        /**< run manual dehaze */
    RK_AIQ_DEHAZE_MODE_AUTO                        = 2,        /**< run auto dehaze, dehaze para from iq file */
    RK_AIQ_DEHAZE_MODE_OFF                         = 3,        /**< dehaze off, enhance follow IQ setting*/
    RK_AIQ_DEHAZE_MODE_TOOL                        = 4,        /**< dehaze tool*/
    RK_AIQ_ENHANCE_MODE_MANUAL                     = 5,        /**< enhance on*/
} rk_aiq_dehaze_op_mode_t;

typedef struct adehaze_sw_s {
    bool byPass;
    rk_aiq_dehaze_op_mode_t mode;
    rk_aiq_dehaze_M_attrib_t stManual;
    CalibDb_Dehaze_t  stTool;
} adehaze_sw_t;

typedef struct AdehazeExpInfo_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
} AdehazeExpInfo_t;

RKAIQ_END_DECLARE

#endif

