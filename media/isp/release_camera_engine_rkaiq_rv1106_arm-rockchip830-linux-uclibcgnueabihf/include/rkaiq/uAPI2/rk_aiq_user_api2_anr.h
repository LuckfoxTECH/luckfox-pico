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

#ifndef _RK_AIQ_USER_API2_ANR_H_
#define _RK_AIQ_USER_API2_ANR_H_

#include "anr/rk_aiq_uapi_anr_int.h"
#include "arawnr/rk_aiq_uapi_abayernr_int_v1.h"
#include "aynr/rk_aiq_uapi_aynr_int_v1.h"
#include "auvnr/rk_aiq_uapi_auvnr_int_v1.h"
#include "amfnr/rk_aiq_uapi_amfnr_int_v1.h"

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;

RKAIQ_BEGIN_DECLARE

XCamReturn
rk_aiq_user_api2_anr_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_nr_attrib_t *attr);
XCamReturn
rk_aiq_user_api2_anr_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_nr_attrib_t *attr);
XCamReturn
rk_aiq_user_api2_anr_SetIQPara(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_nr_IQPara_t *para);
XCamReturn
rk_aiq_user_api2_anr_GetIQPara(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_nr_IQPara_t *para);
XCamReturn
rk_aiq_user_api2_anr_SetLumaSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float fPercnt);
XCamReturn
rk_aiq_user_api2_anr_SetLumaTFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float fPercnt);
XCamReturn
rk_aiq_user_api2_anr_GetLumaSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float *pPercnt);
XCamReturn
rk_aiq_user_api2_anr_GetLumaTFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float *pPercnt);
XCamReturn
rk_aiq_user_api2_anr_SetChromaSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float fPercnt);
XCamReturn
rk_aiq_user_api2_anr_SetChromaTFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float fPercnt);
XCamReturn
rk_aiq_user_api2_anr_GetChromaSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float *pPercnt);
XCamReturn
rk_aiq_user_api2_anr_GetChromaTFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float *pPercnt);
XCamReturn
rk_aiq_user_api2_anr_SetRawnrSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float fPercnt);
XCamReturn
rk_aiq_user_api2_anr_GetRawnrSFStrength(const rk_aiq_sys_ctx_t* sys_ctx, float *pPercnt);




RKAIQ_END_DECLARE

#endif

