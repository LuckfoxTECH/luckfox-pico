/*
 *rk_aiq_uapi_camgroup_asahrp_int.h
 *
 *  Copyright (c) 2023 Rockchip Corporation
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
#ifndef _RK_AIQ_UAPA_CAMGROUP_ASHARP_INT_H_
#define _RK_AIQ_UAPA_CAMGROUP_ASHARP_INT_H_

#include "asharp/rk_aiq_types_asharp_algo_int.h"
#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn rk_aiq_uapi_camgroup_asharp_SetAttrib(RkAiqAlgoContext *ctx,
                                                 rk_aiq_sharp_attrib_t *attr,
                                                 bool need_sync);

XCamReturn rk_aiq_uapi_camgroup_asharp_GetAttrib(const RkAiqAlgoContext *ctx,
                                                 rk_aiq_sharp_attrib_t *attr);

XCamReturn rk_aiq_uapi_camgroup_asharp_SetIQpara(RkAiqAlgoContext *ctx,
                                                 const rk_aiq_sharp_IQpara_t *para,
                                                 bool need_sync);

XCamReturn rk_aiq_uapi_camgroup_asharp_GetIQpara(const RkAiqAlgoContext *ctx,
                                                 rk_aiq_sharp_IQpara_t *para);

XCamReturn rk_aiq_uapi_camgroup_asharp_SetStrength(const RkAiqAlgoContext *ctx,
                                                   float fPercent);

XCamReturn rk_aiq_uapi_camgroup_asharp_GetStrength(const RkAiqAlgoContext *ctx,
                                                   float *pPercent);

#endif