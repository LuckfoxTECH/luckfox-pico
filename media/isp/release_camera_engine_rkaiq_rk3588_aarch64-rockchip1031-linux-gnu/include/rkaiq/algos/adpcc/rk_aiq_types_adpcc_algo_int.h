/*
 *rk_aiq_types_alsc_algo_int.h
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

#ifndef _RK_AIQ_TYPE_ADPCC_ALGO_INT_H_
#define _RK_AIQ_TYPE_ADPCC_ALGO_INT_H_

#include "adpcc/rk_aiq_types_adpcc_algo.h"
#include "adpcc/rk_aiq_types_adpcc_ext.h"
#include "base/xcam_common.h"

typedef enum AdpccResult_e {
    ADPCC_RET_SUCCESS               = 0,   // this has to be 0, if clauses rely on it
    ADPCC_RET_FAILURE               = 1,   // process failure
    ADPCC_RET_INVALID_PARM      = 2,   // invalid parameter
    ADPCC_RET_WRONG_CONFIG      = 3,   // feature not supported
    ADPCC_RET_BUSY              = 4,   // there's already something going on...
    ADPCC_RET_CANCELED          = 5,   // operation canceled
    ADPCC_RET_OUTOFMEM          = 6,   // out of memory
    ADPCC_RET_OUTOFRANGE            = 7,   // parameter/value out of range
    ADPCC_RET_NULL_POINTER      = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ADPCC_RET_DIVISION_BY_ZERO  = 9,   // a divisor equals ZERO
    ADPCC_RET_NO_INPUTIMAGE     = 10   // no input image
} AdpccResult_t;

typedef enum AdpccState_e {
    ADPCC_STATE_INVALID           = 0,                   /**< initialization value */
    ADPCC_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ADPCC_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ADPCC_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ADPCC_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ADPCC_STATE_MAX                                      /**< max */
} AdpccState_t;

typedef struct Adpcc_fast_mode_params_s {
    int fast_mode_en;
    int ISO[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_single_en;
    int fast_mode_single_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_double_en;
    int fast_mode_double_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_triple_en;
    int fast_mode_triple_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
} Adpcc_fast_mode_params_t;

typedef struct Adpcc_sensor_dpcc_s {
    bool en;
    int max_level;
    int level;
} Adpcc_sensor_dpcc_t;

typedef struct Adpcc_html_param_s {
    Adpcc_basic_params_t stBasic;
    Adpcc_bpt_params_t  stBpt;
    Adpcc_pdaf_params_t stPdaf;
} Adpcc_html_param_t;

typedef struct AdpccProcResult_s {
    Adpcc_basic_cfg_params_t stBasic;
    Adpcc_bpt_params_t stBpt;
    Adpcc_pdaf_params_t stPdaf;
} AdpccProcResult_t;

typedef struct AdpccConfig_s {
    AdpccOPMode_t eMode;
    AdpccState_t eState;
} AdpccConfig_t;

typedef struct AdpccExpInfo_s {
    int hdr_mode;
    float arPreResTime[3];
    float arPreResAGain[3];
    float arPreResDGain[3];
    int   arPreResIso[3];

    float arProcResTime[3];
    float arProcResAGain[3];
    float arProcResDGain[3];
    int   arProcResIso[3];
} AdpccExpInfo_t;

typedef struct Sensor_dpcc_res_s {
    int enable;
    int cur_single_dpcc;
    int cur_multiple_dpcc;
    int total_dpcc;
} Sensor_dpcc_res_t;

typedef struct Adpcc_pre_ae_res_s {
    float arProcResTime[3];
    float arProcResAGain[3];
    float arProcResDGain[3];
    int   arProcResIso[3];
} Adpcc_pre_ae_res_t;

#endif
