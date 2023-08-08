/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
 */
#ifndef _RK_AIQ_TYPES_ABLC_ALGO_INT_V32_H_
#define _RK_AIQ_TYPES_ABLC_ALGO_INT_V32_H_
#include "RkAiqCalibDbTypes.h"
#include "ablcV32/rk_aiq_types_ablc_algo_v32.h"
#include "ablc_head_V32.h"
#include "ablc_uapi_head_v32.h"

#define ABLC_V32_RECALCULATE_DELTE_ISO (10)

typedef enum AblcResult_V32_e {
    ABLC_V32_RET_SUCCESS          = 0,  // this has to be 0, if clauses rely on it
    ABLC_V32_RET_FAILURE          = 1,  // process failure
    ABLC_V32_RET_INVALID_PARM     = 2,  // invalid parameter
    ABLC_V32_RET_WRONG_CONFIG     = 3,  // feature not supported
    ABLC_V32_RET_BUSY             = 4,  // there's already something going on...
    ABLC_V32_RET_CANCELED         = 5,  // operation canceled
    ABLC_V32_RET_OUTOFMEM         = 6,  // out of memory
    ABLC_V32_RET_OUTOFRANGE       = 7,  // parameter/value out of range
    ABLC_V32_RET_NULL_POINTER     = 8,  // the/one/all parameter(s) is a(are) NULL pointer(s)
    ABLC_V32_RET_DIVISION_BY_ZERO = 9,  // a divisor equals ZERO
    ABLC_V32_RET_NO_INPUTIMAGE    = 10  // no input image
} AblcResult_V32_t;

typedef enum AblcState_V32_e {
    ABLC_V32_STATE_INVALID     = 0, /**< initialization value */
    ABLC_V32_STATE_INITIALIZED = 1, /**< instance is created, but not initialized */
    ABLC_V32_STATE_STOPPED     = 2, /**< instance is confiured (ready to start) or stopped */
    ABLC_V32_STATE_RUNNING     = 3, /**< instance is running (processes frames) */
    ABLC_V32_STATE_LOCKED      = 4, /**< instance is locked (for taking snapshots) */
    ABLC_V32_STATE_MAX              /**< max */
} AblcState_V32_t;

typedef enum AblcOPMode_V32_e {
    ABLC_V32_OP_MODE_OFF    = 0, /**< initialization value */
    ABLC_V32_OP_MODE_AUTO   = 1, /**< instance is created, but not initialized */
    ABLC_V32_OP_MODE_MANUAL = 2, /**< instance is confiured (ready to start) or stopped */
    ABLC_V32_OP_MODE_MAX         /**< max */
} AblcOPMode_V32_t;

typedef enum AblcParamMode_V32_e {
    ABLC_V32_PARAM_MODE_INVALID = 0, /**< initialization value */
    ABLC_V32_PARAM_MODE_NORMAL  = 1, /**< instance is created, but not initialized */
    ABLC_V32_PARAM_MODE_HDR     = 2, /**< instance is confiured (ready to start) or stopped */
    ABLC_V32_PARAM_MODE_MAX          /**< max */
} AblcParamMode_V32_t;

typedef struct AblcParams_V32_s {
    bool enable;
    int len;
    float* iso;
    float* blc_r;
    float* blc_gr;
    float* blc_gb;
    float* blc_b;
} AblcParams_V32_t;

typedef struct AblcRefParams_V32_s {
    int len;
    float* iso;
    float* Reference_r;
    float* Reference_gr;
    float* Reference_gb;
    float* Reference_b;
} AblcRefParams_V32_t;


typedef struct AblcOBParams_V32_s {
    bool enable;
    int len;
    float* iso;
    float* ob_offset;
    float* ob_predgain;
} AblcOBParams_V32_t;


typedef AblcSelect_V32_t AblcManualAttr_V32_t;
typedef AblcOBSelect_V32_t AblcManualOBAttr_V32_t;
typedef struct AblcProc_V32_s {
    bool enable;
    short int blc_r;
    short int blc_gr;
    short int blc_gb;
    short int blc_b;

    bool blc1_enable;
    short int blc1_r;
    short int blc1_gr;
    short int blc1_gb;
    short int blc1_b;

    bool blc_ob_enable;
    short int isp_ob_offset;
    float isp_ob_predgain;
    int isp_ob_max;

    bool isNeedUpdate;
} AblcProc_V32_t;

typedef struct rk_aiq_blc_attrib_V32_s {
    rk_aiq_uapi_sync_t sync;
    AblcOPMode_V32_t eMode;
    AblcParams_V32_t stBlc0Auto;
    AblcParams_V32_t stBlc1Auto;
    AblcOBParams_V32_t stBlcOBAuto;
    AblcManualAttr_V32_t stBlc0Manual;
    AblcManualAttr_V32_t stBlc1Manual;
    AblcManualOBAttr_V32_t stBlcOBManual;
} rk_aiq_blc_attrib_V32_t;


#endif