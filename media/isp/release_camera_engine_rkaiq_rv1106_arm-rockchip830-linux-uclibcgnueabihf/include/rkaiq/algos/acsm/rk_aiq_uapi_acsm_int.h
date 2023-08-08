/*
 * rk_aiq_algo_acsm_int.h
 *
 *  Copyright (c) 2022 Rockchip Corporation
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
#ifndef _RK_AIQ_UAPI_ACSM_INT_H_
#define _RK_AIQ_UAPI_ACSM_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_acsm_algo.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAdebayerHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

// same as type Csm_Param_t defined in csm_head.h
typedef struct rk_aiq_uapi_acsm_attrib_s {
    rk_aiq_uapi_sync_t sync;
    rk_aiq_acsm_params_t param;
} rk_aiq_uapi_acsm_attrib_t;

XCamReturn
rk_aiq_uapi_acsm_SetAttrib(RkAiqAlgoContext* ctx,
                           const rk_aiq_uapi_acsm_attrib_t* attr,
                           bool need_sync);

XCamReturn
rk_aiq_uapi_acsm_GetAttrib(RkAiqAlgoContext*  ctx,
                           rk_aiq_uapi_acsm_attrib_t* attr);

#endif // _RK_AIQ_UAPI_ACSM_INT_H_
