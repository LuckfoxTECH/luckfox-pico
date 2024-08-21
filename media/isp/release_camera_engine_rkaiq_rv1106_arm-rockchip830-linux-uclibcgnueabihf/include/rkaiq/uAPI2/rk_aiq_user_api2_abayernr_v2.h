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

#ifndef _RK_AIQ_USER_API2_ABAYERNRV2_H_
#define _RK_AIQ_USER_API2_ABAYERNRV2_H_

#include "arawnr2/rk_aiq_uapi_abayernr_int_v2.h"

#ifndef RK_AIQ_SYS_CTX_T
#define RK_AIQ_SYS_CTX_T
typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;
#endif


RKAIQ_BEGIN_DECLARE

XCamReturn
rk_aiq_user_api2_abayernrV2_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_bayernr_attrib_v2_t* attr);

XCamReturn
rk_aiq_user_api2_abayernrV2_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_bayernr_attrib_v2_t* attr);

XCamReturn
rk_aiq_user_api2_abayernrV2_SetSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float fPercnt);

XCamReturn
rk_aiq_user_api2_abayernrV2_SetTFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float fPercnt);

XCamReturn
rk_aiq_user_api2_abayernrV2_GetSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float *pPercnt);

XCamReturn
rk_aiq_user_api2_abayernrV2_GetTFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float *pPercnt);






RKAIQ_END_DECLARE

#endif
