/*
 * rk_aiq_types_accm_int.h
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

#ifndef _RK_AIQ_TYPES_ACCM_ALGO_INT_H_
#define _RK_AIQ_TYPES_ACCM_ALGO_INT_H_
#include "RkAiqCalibDbTypes.h"
#include "ccm_head.h"
#include "ccm_uapi_head.h"
#include "accm/rk_aiq_types_accm_algo.h"

RKAIQ_BEGIN_DECLARE

#define RK_AIQ_ACCM_COLOR_GAIN_NUM 4

typedef struct accm_sw_info_s {
    float sensorGain;
    float awbGain[2];
    float awbIIRDampCoef;
    float varianceLuma;
    bool grayMode;
    bool awbConverged;
    int prepare_type;
    bool ccmConverged;
} accm_sw_info_t;

typedef struct rk_aiq_ccm_color_inhibition_s {
    float sensorGain[RK_AIQ_ACCM_COLOR_GAIN_NUM];
    float level[RK_AIQ_ACCM_COLOR_GAIN_NUM];//max value 100,default value 0
} rk_aiq_ccm_color_inhibition_t;

typedef struct rk_aiq_ccm_color_saturation_s {
    float sensorGain[RK_AIQ_ACCM_COLOR_GAIN_NUM];
    float level[RK_AIQ_ACCM_COLOR_GAIN_NUM];//max value 100, default value 100
} rk_aiq_ccm_color_saturation_t;

typedef struct rk_aiq_ccm_accm_attrib_s {
    rk_aiq_ccm_color_inhibition_t color_inhibition;
    rk_aiq_ccm_color_saturation_t color_saturation;
} rk_aiq_ccm_accm_attrib_t;
typedef struct rk_aiq_ccm_attrib_s {
    rk_aiq_uapi_sync_t sync;
    bool byPass;
    // M4_ENUM_DESC("mode", "rk_aiq_ccm_op_mode_t", "RK_AIQ_CCM_MODE_AUTO");
    rk_aiq_ccm_op_mode_t mode;
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    rk_aiq_ccm_mccm_attrib_t stManual;
    rk_aiq_ccm_accm_attrib_t stAuto;
} rk_aiq_ccm_attrib_t;

typedef struct rk_aiq_ccm_v2_attrib_s {
    rk_aiq_uapi_sync_t sync;
    bool byPass;
    // M4_ENUM_DESC("mode", "rk_aiq_ccm_op_mode_t", "RK_AIQ_CCM_MODE_AUTO");
    rk_aiq_ccm_op_mode_t mode;
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    rk_aiq_ccm_mccm_attrib_v2_t stManual;
    rk_aiq_ccm_accm_attrib_t stAuto;
} rk_aiq_ccm_v2_attrib_t;


RKAIQ_END_DECLARE

#endif

