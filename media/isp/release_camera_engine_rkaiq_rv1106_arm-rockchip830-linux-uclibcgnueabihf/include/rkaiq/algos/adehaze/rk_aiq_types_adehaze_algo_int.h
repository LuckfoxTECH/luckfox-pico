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
#include "adehaze_head.h"
#include "adehaze_uapi_head.h"

#define DEHAZE_DEFAULT_LEVEL         (50)
#define DEHAZE_DEFAULT_CFG_WT_STEP   (1)
#define DEHAZE_DEFAULT_CFG_AIR_STEP  (1)
#define DEHAZE_DEFAULT_CFG_TMAX_STEP (5)
#define ENHANCE_DEFAULT_LEVEL        (50)
#define ENHANCE_VALUE_DEFAULT_STEP   (80)
#define ENH_LUMA_DEFAULT_STEP        (5)

typedef struct AdehazeOBResV12_s {
    bool blc_ob_enable;
    float isp_ob_predgain;
} AdehazeOBResV12_t;

typedef enum dehaze_api_mode_s {
    DEHAZE_API_AUTO   = 0, /**< run atuo mode */
    DEHAZE_API_MANUAL = 1, /**< run manual mode*/
} dehaze_api_mode_t;

// dehaze v10
typedef struct adehaze_sw_v10_s {
} adehaze_sw_v10_t;

// dehaze v11
typedef struct adehaze_sw_v11_s {
    rk_aiq_uapi_sync_t sync;

    dehaze_api_mode_t mode;
    CalibDbV2_dehaze_v11_t stAuto;
    mDehazeAttrV11_t stManual;
    mDehazeAttrInfoV11_t Info;
} adehaze_sw_v11_t;

// dehaze v12
typedef struct adehaze_sw_v12_s {
    rk_aiq_uapi_sync_t sync;

    dehaze_api_mode_t mode;
    CalibDbV2_dehaze_v12_t stAuto;
    mDehazeAttrV12_t stManual;
    mDehazeAttrInfoV11_t Info;
} adehaze_sw_v12_t;

#endif
