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

#ifndef __ACP_UAPI_HEAD_H__
#define __ACP_UAPI_HEAD_H__

#include "rk_aiq_comm.h"

typedef struct acp_attrib_s {
  // M4_STRUCT_DESC("sync", "normal_ui_style", M4_HIDE(1))
  rk_aiq_uapi_sync_t sync;

  // M4_NUMBER_DESC("brightness", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
  uint8_t brightness; /* 0 ~ 255 */
  // M4_NUMBER_DESC("contrast", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
  uint8_t contrast; /* 0 ~ 255 */
  // M4_NUMBER_DESC("saturation", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
  uint8_t saturation; /* 0 ~ 255 */
  // M4_NUMBER_DESC("hue", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
  uint8_t hue; /* 0 ~ 255 */
} acp_attrib_t;

#endif  /*__ACP_UAPI_HEAD_H__*/
