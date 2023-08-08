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

#ifndef _RK_AIQ_UAPI_AGAIN_INT_V2_H_
#define _RK_AIQ_UAPI_AGAIN_INT_V2_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "again2/rk_aiq_types_again_algo_int_v2.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_againV2_SetAttrib(RkAiqAlgoContext *ctx,
                              const rk_aiq_gain_attrib_v2_t *attr,
                              bool need_sync);

XCamReturn
rk_aiq_uapi_againV2_GetAttrib(const RkAiqAlgoContext *ctx,
                              rk_aiq_gain_attrib_v2_t *attr);

XCamReturn
rk_aiq_uapi_againV2_GetInfo(const RkAiqAlgoContext *ctx,
                            rk_aiq_gain_info_v2_t *pInfo);

#endif
