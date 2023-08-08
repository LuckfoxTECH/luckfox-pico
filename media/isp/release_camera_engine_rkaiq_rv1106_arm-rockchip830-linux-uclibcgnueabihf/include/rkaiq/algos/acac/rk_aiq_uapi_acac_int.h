/*
 *  Copyright (c) 2022 Rockchip Electronics Co., Ltd
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
 * Author: Cody Xie <cody.xie@rock-chips.com>
 */
#ifndef _RK_AIQ_UAPI_ACAC_INT_H_
#define _RK_AIQ_UAPI_ACAC_INT_H_

#include "algos/acac/rk_aiq_types_acac_algo_int.h"
#include "algos/rk_aiq_algo_des.h"
#include "base/xcam_common.h"

XCamReturn rk_aiq_uapi_acac_v03_SetAttrib(RkAiqAlgoContext* ctx,
                                          const rkaiq_cac_v03_api_attr_t* attr, bool need_sync);
XCamReturn rk_aiq_uapi_acac_v03_GetAttrib(const RkAiqAlgoContext* ctx,
                                          rkaiq_cac_v03_api_attr_t* attr);

XCamReturn rk_aiq_uapi_acac_v10_SetAttrib(RkAiqAlgoContext* ctx,
                                          const rkaiq_cac_v10_api_attr_t* attr, bool need_sync);
XCamReturn rk_aiq_uapi_acac_v10_GetAttrib(const RkAiqAlgoContext* ctx,
                                          rkaiq_cac_v10_api_attr_t* attr);

XCamReturn rk_aiq_uapi_acac_v11_SetAttrib(RkAiqAlgoContext* ctx,
                                          const rkaiq_cac_v11_api_attr_t* attr, bool need_sync);
XCamReturn rk_aiq_uapi_acac_v11_GetAttrib(const RkAiqAlgoContext* ctx,
                                          rkaiq_cac_v11_api_attr_t* attr);

#endif  // _RK_AIQ_UAPI_ACAC_INT_H_
