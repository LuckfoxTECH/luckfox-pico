/*
 * rk_aiq_user_api2_asahrp_v33.h
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

#ifndef _RK_AIQ_USER_API2_ASHARP_V33_H_
#define _RK_AIQ_USER_API2_ASHARP_V33_H_

#include "asharpV33/rk_aiq_uapi_asharp_int_v33.h"

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;

RKAIQ_BEGIN_DECLARE

XCamReturn rk_aiq_user_api2_asharpV33_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
        const rk_aiq_sharp_attrib_v33_t* attr);

XCamReturn rk_aiq_user_api2_asharpV33_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
        rk_aiq_sharp_attrib_v33_t* attr);

XCamReturn rk_aiq_user_api2_asharpV33Lite_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                                    const rk_aiq_sharp_attrib_v33LT_t* attr);

XCamReturn rk_aiq_user_api2_asharpV33Lite_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                                    rk_aiq_sharp_attrib_v33LT_t* attr);

XCamReturn rk_aiq_user_api2_asharpV33_SetStrength(const rk_aiq_sys_ctx_t* sys_ctx,
        const rk_aiq_sharp_strength_v33_t* pStrength);

XCamReturn rk_aiq_user_api2_asharpV33_GetStrength(const rk_aiq_sys_ctx_t* sys_ctx,
        rk_aiq_sharp_strength_v33_t* pStrength);
XCamReturn rk_aiq_user_api2_asharpV33_GetInfo(const rk_aiq_sys_ctx_t* sys_ctx,
        rk_aiq_sharp_info_v33_t* pInfo);
RKAIQ_END_DECLARE

#endif