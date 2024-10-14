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
    rk_aiq_ccm_op_mode_t mode;
    rk_aiq_ccm_mccm_attrib_t stManual;
    rk_aiq_ccm_accm_attrib_t stAuto;
} rk_aiq_ccm_attrib_t;

typedef struct rk_aiq_ccm_v2_attrib_s {
    rk_aiq_uapi_sync_t sync;
    bool byPass;
    rk_aiq_ccm_op_mode_t mode;
    rk_aiq_ccm_mccm_attrib_v2_t stManual;
    rk_aiq_ccm_accm_attrib_t stAuto;
} rk_aiq_ccm_v2_attrib_t;

typedef struct CalibDbV2_Ccm_Control_Para_s rk_aiq_ccm_ctrlcfg_t;
typedef struct CalibDbV2_Ccm_Luma_Ccm_s     rk_aiq_ccm_yalpcfg_t;
typedef struct CalibDbV2_Ccm_Luma_Ccm_V2_s  rk_aiq_ccm_v2_yalpcfg_t;
typedef struct rk_aiq_ccm_illucfg_s {
    char  name[CCM_ILLUMINATION_NAME];
    float awbGain[2];
    float minDist;
    char  matrixUsed[CCM_PROFILES_NUM_MAX][CCM_PROFILE_NAME];
    int   matrixUsed_len;
    CalibDbV2_Ccm_Gain_Sat_Curve_t gain_sat_curve;
} rk_aiq_ccm_illucfg_t;

typedef struct rk_aiq_ccm_matrixcfg_s {
    char  name[CCM_PROFILE_NAME];
    char  illumination[CCM_ILLUMINATION_NAME];
    float saturation;
    float ccMatrix[9];
    float ccOffsets[3];
} rk_aiq_ccm_matrixcfg_t;

typedef struct rk_aiq_ccm_iqparam_attrib_s {
    CalibDbV2_Ccm_Control_Para_t control;
    CalibDbV2_Ccm_Luma_Ccm_t     lumaCCM;
    bool                         damp_enable;
    rk_aiq_ccm_illucfg_t         aCcmCof[CCM_ILLUMINATION_MAX];
    int                          aCcmCof_len;
    rk_aiq_ccm_matrixcfg_t       matrixAll[CCM_ILLUMINATION_MAX*CCM_PROFILES_NUM_MAX];
    int                          matrixAll_len;
} rk_aiq_ccm_iqparam_attrib_t;

typedef struct rk_aiq_ccm_v2_iqparam_attrib_s {
    CalibDbV2_Ccm_Control_Para_t control;
    CalibDbV2_Ccm_Luma_Ccm_V2_t  lumaCCM;
    CalibDbV2_Ccm_Enhance_Para_t enhCCM;
    bool                         damp_enable;
    rk_aiq_ccm_illucfg_t         aCcmCof[CCM_ILLUMINATION_MAX];
    int                          aCcmCof_len;
    rk_aiq_ccm_matrixcfg_t       matrixAll[CCM_ILLUMINATION_MAX*CCM_PROFILES_NUM_MAX];
    int                          matrixAll_len;
} rk_aiq_ccm_v2_iqparam_attrib_t;

typedef struct rk_aiq_ccm_calib_attrib_s {
    rk_aiq_uapi_sync_t           sync;
    rk_aiq_ccm_iqparam_attrib_t  iqparam;
} rk_aiq_ccm_calib_attrib_t;

typedef struct rk_aiq_ccm_v2_calib_attrib_s {
    rk_aiq_uapi_sync_t             sync;
    rk_aiq_ccm_v2_iqparam_attrib_t iqparam;
} rk_aiq_ccm_v2_calib_attrib_t;


RKAIQ_END_DECLARE

#endif

