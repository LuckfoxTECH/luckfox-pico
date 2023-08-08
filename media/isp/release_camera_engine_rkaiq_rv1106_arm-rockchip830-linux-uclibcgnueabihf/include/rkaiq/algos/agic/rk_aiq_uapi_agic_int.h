/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
 */
#ifndef _RK_AIQ_UAPI_AGIC_INT_H_
#define _RK_AIQ_UAPI_AGIC_INT_H_

#include "algos/agic/rk_aiq_types_algo_agic_int.h"
#include "algos/rk_aiq_algo_des.h"
#include "xcore/base/xcam_common.h"

XCamReturn rk_aiq_uapi_agic_v1_SetAttrib(RkAiqAlgoContext* ctx, const rkaiq_gic_v1_api_attr_t* attr,
                                         bool need_sync);
XCamReturn rk_aiq_uapi_agic_v1_GetAttrib(RkAiqAlgoContext* ctx, rkaiq_gic_v1_api_attr_t* attr);

XCamReturn rk_aiq_uapi_agic_v2_SetAttrib(RkAiqAlgoContext* ctx, const rkaiq_gic_v2_api_attr_t* attr,
                                         bool need_sync);
XCamReturn rk_aiq_uapi_agic_v2_GetAttrib(RkAiqAlgoContext* ctx, rkaiq_gic_v2_api_attr_t* attr);

#endif  //_RK_AIQ_UAPI_AGIC_INT_H_
