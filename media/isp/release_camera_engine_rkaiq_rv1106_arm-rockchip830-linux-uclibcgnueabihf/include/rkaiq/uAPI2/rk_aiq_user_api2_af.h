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

#ifndef _RK_AIQ_USER_API2_AF_H_
#define _RK_AIQ_USER_API2_AF_H_
#include "af/rk_aiq_uapi_af_int.h"

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;

RKAIQ_BEGIN_DECLARE

XCamReturn
rk_aiq_user_api2_af_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_af_attrib_t *attr);
XCamReturn
rk_aiq_user_api2_af_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_af_attrib_t *attr);
XCamReturn
rk_aiq_user_api2_af_Lock(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_Unlock(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_Oneshot(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_ManualTriger(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_Tracking(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_SetZoomIndex(const rk_aiq_sys_ctx_t* sys_ctx, int index);
XCamReturn
rk_aiq_user_api2_af_GetZoomIndex(const rk_aiq_sys_ctx_t* sys_ctx, int *index);
XCamReturn
rk_aiq_user_api2_af_EndZoomChg(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_StartZoomCalib(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_resetZoom(const rk_aiq_sys_ctx_t* sys_ctx);
XCamReturn
rk_aiq_user_api2_af_SetVcmCfg(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_lens_vcmcfg* cfg);
XCamReturn
rk_aiq_user_api2_af_GetVcmCfg(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_lens_vcmcfg* cfg);
XCamReturn
rk_aiq_user_api2_af_GetSearchPath(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_af_sec_path_t* path);
XCamReturn
rk_aiq_user_api2_af_GetSearchResult(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_af_result_t* result);
XCamReturn
rk_aiq_user_api2_af_GetZoomRange(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_af_zoomrange* range);
XCamReturn
rk_aiq_user_api2_af_GetFocusRange(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_af_focusrange* range);
XCamReturn
rk_aiq_user_api2_af_setAngleZ(const rk_aiq_sys_ctx_t* sys_ctx, float angleZ);

RKAIQ_END_DECLARE

#endif
