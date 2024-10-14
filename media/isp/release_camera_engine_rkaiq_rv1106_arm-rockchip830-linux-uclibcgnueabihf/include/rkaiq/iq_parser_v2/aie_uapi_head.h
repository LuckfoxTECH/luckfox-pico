/*
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef __AIE_UAPI_HEAD_H__
#define __AIE_UAPI_HEAD_H__

#include "rk_aiq_comm.h"
#include "aie/rk_aiq_types_aie_algo.h"

typedef struct aie_attrib_s {
  // M4_STRUCT_DESC("sync", "normal_ui_style", M4_HIDE(1))
  rk_aiq_uapi_sync_t sync;

  // M4_ENUM_DESC("mode", "rk_aiq_ie_effect_t", "RK_AIQ_IE_EFFECT_BW");
  rk_aiq_ie_effect_t mode;
} aie_attrib_t;

#endif  /*__AIE_UAPI_HEAD_H__*/
