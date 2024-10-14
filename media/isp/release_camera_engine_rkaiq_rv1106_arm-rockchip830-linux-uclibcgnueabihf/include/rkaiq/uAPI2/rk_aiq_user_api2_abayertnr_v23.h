/*
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

#ifndef _RK_AIQ_USER_API2_ABAYERTNRV23_H_
#define _RK_AIQ_USER_API2_ABAYERTNRV23_H_

#include "abayertnrV23/rk_aiq_uapi_abayertnr_int_v23.h"

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;

RKAIQ_BEGIN_DECLARE

XCamReturn
rk_aiq_user_api2_abayertnrV23_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, const rk_aiq_bayertnr_attrib_v23_t* attr);

XCamReturn
rk_aiq_user_api2_abayertnrV23_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_bayertnr_attrib_v23_t* attr);

XCamReturn rk_aiq_user_api2_abayertnrV23Lite_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                                       const rk_aiq_bayertnr_attrib_v23L_t* attr);

XCamReturn rk_aiq_user_api2_abayertnrV23Lite_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                                       rk_aiq_bayertnr_attrib_v23L_t* attr);

XCamReturn rk_aiq_user_api2_abayertnrV23_SetStrength(
    const rk_aiq_sys_ctx_t* sys_ctx, const rk_aiq_bayertnr_strength_v23_t* pStrength);

XCamReturn
rk_aiq_user_api2_abayertnrV23_GetStrength(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_bayertnr_strength_v23_t *pStrength);


XCamReturn
rk_aiq_user_api2_abayertnrV23_GetInfo(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_bayertnr_info_v23_t *pInfo);




RKAIQ_END_DECLARE

#endif
