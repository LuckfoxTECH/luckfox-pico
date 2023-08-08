/*
 * rk_aiq_types_aeis_algo_prvt.h
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

#ifndef _RK_AIQ_UAPI_AEIS_INT_H_
#define _RK_AIQ_UAPI_AEIS_INT_H_

#include "aeis/rk_aiq_types_aeis_algo_int.h"
#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAeisHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.
XCamReturn rk_aiq_uapi_aeis_SetAttrib(RkAiqAlgoContext* ctx, rk_aiq_eis_attrib_t attr,
                                      bool need_sync);
XCamReturn rk_aiq_uapi_aeis_GetAttrib(const RkAiqAlgoContext* ctx, rk_aiq_eis_attrib_t* attr);
XCamReturn rk_aiq_uapi_aeis_QueryEisInfo(const RkAiqAlgoContext* ctx,
                                         rk_aiq_eis_querry_info_t* eis_querry_info);

#endif
