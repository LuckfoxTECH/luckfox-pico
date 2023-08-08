/*
 * rk_aiq_types_acac_algo_prvt.h
 *
 *  Copyright (c) 2021 Rockchip Electronics Co., Ltd
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

#include "acac/rk_aiq_types_acac_algo_int.h"
#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

XCamReturn rk_aiq_uapi_acac_SetAttrib(RkAiqAlgoContext* ctx, rk_aiq_cac_attrib_t attr,
                                      bool need_sync);
XCamReturn rk_aiq_uapi_acac_GetAttrib(const RkAiqAlgoContext* ctx, rk_aiq_cac_attrib_t* attr);
XCamReturn rk_aiq_uapi_acac_QueryCacInfo(const RkAiqAlgoContext* ctx,
                                         rk_aiq_cac_querry_info_t* cac_querry_info);

#endif
