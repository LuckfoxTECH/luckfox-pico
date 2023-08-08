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
#include "accm/rk_aiq_types_accm_algo.h"
#include "accm/rk_aiq_types_accm_ext.h"

RKAIQ_BEGIN_DECLARE

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

typedef struct rk_aiq_ccm_attrib_s {
    rk_aiq_uapi_sync_t sync;
    bool byPass;
    rk_aiq_ccm_op_mode_t mode;
    rk_aiq_ccm_mccm_attrib_t stManual;
    rk_aiq_ccm_accm_attrib_t stAuto;
} rk_aiq_ccm_attrib_t;

typedef struct rk_aiq_ccm_querry_info_s {
    bool ccm_en;
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "[1.0000,0.0000,0.0000,0.0000,1.0000,0.0000,0.0000,0.0000,1.0000]", M4_DIGIT(4), M4_DYNAMIC(0))
    float  ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    float  ccOffsets[3];
    // M4_ARRAY_DESC("y alpha curve", "f32", M4_SIZE(1,17), M4_RANGE(0,1024), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1024]", M4_DIGIT(0), M4_DYNAMIC(0))
    float  y_alpha_curve[CCM_CURVE_DOT_NUM];
    // M4_NUMBER_DESC("bound pos bit", "f32", M4_RANGE(0, 10), "8", M4_DIGIT(0))
    float low_bound_pos_bit;
    float color_inhibition_level;
    float color_saturation_level;
    // M4_NUMBER_DESC("CCM Saturation", "f32", M4_RANGE(0,200), "0", M4_DIGIT(2))
    float finalSat;
    // M4_STRING_DESC("usedCcm1", M4_SIZE(1,1), M4_RANGE(0, 25), "A_100",M4_DYNAMIC(0))
    char  ccmname1[25];
    // M4_STRING_DESC("usedCcm2", M4_SIZE(1,1), M4_RANGE(0, 25), "A_100",M4_DYNAMIC(0))
    char  ccmname2[25];
} rk_aiq_ccm_querry_info_t;

RKAIQ_END_DECLARE

#endif

