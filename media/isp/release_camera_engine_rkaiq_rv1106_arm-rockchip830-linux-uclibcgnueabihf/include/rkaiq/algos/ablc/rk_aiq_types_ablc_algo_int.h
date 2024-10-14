
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

#ifndef _RK_AIQ_TYPE_ABLC_ALGO_INT_H_
#define _RK_AIQ_TYPE_ABLC_ALGO_INT_H_
#include "ablc/rk_aiq_types_ablc_algo.h"
#include "RkAiqCalibDbTypes.h"
#include "ablc_head.h"
#include "ablc_uapi_head.h"


#define ABLC_RECALCULATE_DELTE_ISO  (10)

typedef enum AblcResult_e {
    ABLC_RET_SUCCESS            = 0,   // this has to be 0, if clauses rely on it
    ABLC_RET_FAILURE            = 1,   // process failure
    ABLC_RET_INVALID_PARM       = 2,   // invalid parameter
    ABLC_RET_WRONG_CONFIG       = 3,   // feature not supported
    ABLC_RET_BUSY               = 4,   // there's already something going on...
    ABLC_RET_CANCELED           = 5,   // operation canceled
    ABLC_RET_OUTOFMEM           = 6,   // out of memory
    ABLC_RET_OUTOFRANGE         = 7,   // parameter/value out of range
    ABLC_RET_NULL_POINTER       = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ABLC_RET_DIVISION_BY_ZERO   = 9,   // a divisor equals ZERO
    ABLC_RET_NO_INPUTIMAGE      = 10   // no input image
} AblcResult_t;

typedef enum AblcState_e {
    ABLC_STATE_INVALID           = 0,                   /**< initialization value */
    ABLC_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ABLC_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABLC_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ABLC_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ABLC_STATE_MAX                                      /**< max */
} AblcState_t;

typedef enum AblcOPMode_e {
    ABLC_OP_MODE_OFF         = 0,                    /**< initialization value */
    ABLC_OP_MODE_AUTO             = 1,                   /**< instance is created, but not initialized */
    ABLC_OP_MODE_MANUAL           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABLC_OP_MODE_MAX                                      /**< max */
} AblcOPMode_t;

typedef enum AblcParamMode_e {
    ABLC_PARAM_MODE_INVALID          = 0,                    /**< initialization value */
    ABLC_PARAM_MODE_NORMAL             = 1,                   /**< instance is created, but not initialized */
    ABLC_PARAM_MODE_HDR           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ABLC_PARAM_MODE_MAX                                        /**< max */
} AblcParamMode_t;

typedef struct AblcParams_s {
    bool enable;
    int len;
    float iso[ABLC_MAX_ISO_LEVEL];
    float blc_r[ABLC_MAX_ISO_LEVEL];
    float blc_gr[ABLC_MAX_ISO_LEVEL];
    float blc_gb[ABLC_MAX_ISO_LEVEL];
    float blc_b[ABLC_MAX_ISO_LEVEL];
} AblcParams_t;

#if 0
typedef struct AblcSelect_s {
    bool enable;
    short int blc_r;
    short int blc_gr;
    short int blc_gb;
    short int blc_b;
} AblcSelect_t;


typedef AblcSelect_t AblcManualAttr_t;
#endif


typedef struct AblcProc_s {
    bool enable;
    short int blc_r;
    short int blc_gr;
    short int blc_gb;
    short int blc_b;

    bool blc1_enable;

    float blc1_r;
    float blc1_gr;
    float blc1_gb;
    float blc1_b;
} AblcProc_t;



typedef struct rk_aiq_blc_attrib_s {
    rk_aiq_uapi_sync_t sync;
    AblcOPMode_t eMode;
    AblcParams_t stBlc0Auto;
    AblcParams_t stBlc1Auto;
    AblcManualAttr_t stBlc0Manual;
    AblcManualAttr_t stBlc1Manual;
} rk_aiq_blc_attrib_t;



#endif

