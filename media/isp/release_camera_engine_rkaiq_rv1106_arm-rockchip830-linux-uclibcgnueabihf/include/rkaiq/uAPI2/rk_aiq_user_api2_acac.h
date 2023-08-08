/*
 *  Copyright (c) 2021 Rockchip Electronics Co., Ltd.
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

#ifndef _RK_AIQ_USER_API2_ACAC_H_
#define _RK_AIQ_USER_API2_ACAC_H_

#include "algos/acac/rk_aiq_types_acac_algo_int.h"

RKAIQ_BEGIN_DECLARE

XCamReturn rk_aiq_user_api2_acac_v03_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                               const rkaiq_cac_v03_api_attr_t* attr);
XCamReturn rk_aiq_user_api2_acac_v03_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                               rkaiq_cac_v03_api_attr_t* attr);

XCamReturn rk_aiq_user_api2_acac_v10_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                               const rkaiq_cac_v10_api_attr_t* attr);
XCamReturn rk_aiq_user_api2_acac_v10_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                               rkaiq_cac_v10_api_attr_t* attr);

XCamReturn rk_aiq_user_api2_acac_v11_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                               const rkaiq_cac_v11_api_attr_t* attr);
XCamReturn rk_aiq_user_api2_acac_v11_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx,
                                               rkaiq_cac_v11_api_attr_t* attr);
RKAIQ_END_DECLARE

#endif
