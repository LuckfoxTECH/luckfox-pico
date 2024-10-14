/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __RKADK_DISP_H__
#define __RKADK_DISP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rkadk_common.h"

typedef struct {
  RKADK_RECT_S stVpssCropRect;
  RKADK_RECT_S stVoRect;
} RKADK_DISP_ATTR_S;

RKADK_S32 RKADK_DISP_Init(RKADK_U32 u32CamId);

RKADK_S32 RKADK_DISP_DeInit(RKADK_U32 u32CamId);

RKADK_S32 RKADK_DISP_SetAttr(RKADK_U32 u32CamId, RKADK_DISP_ATTR_S *pstAttr);

#ifdef __cplusplus
}
#endif
#endif
