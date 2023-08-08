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
#ifndef _RK_AIQ_TYPE_ABLC_ALGO_INT_V32_H_
#define _RK_AIQ_TYPE_ABLC_ALGO_INT_V32_H_

#include "ablcV32/rk_aiq_types_ablc_algo_int_v32.h"
#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn rk_aiq_uapi_ablc_V32_SetAttrib(RkAiqAlgoContext* ctx, rk_aiq_blc_attrib_V32_t* attr,
                                         bool need_sync);
XCamReturn rk_aiq_uapi_ablc_V32_GetAttrib(const RkAiqAlgoContext* ctx,
                                         rk_aiq_blc_attrib_V32_t* attr);

XCamReturn rk_aiq_uapi_ablc_V32_GetProc(const RkAiqAlgoContext* ctx, AblcProc_V32_t* ProcRes);

XCamReturn rk_aiq_uapi_ablc_V32_GetInfo(const RkAiqAlgoContext* ctx, rk_aiq_blc_info_v32_t* pInfo);

#endif