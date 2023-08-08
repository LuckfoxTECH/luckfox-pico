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

#ifndef __RKADK_OSD_H__
#define __RKADK_OSD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rkadk_common.h"

typedef struct {
  RKADK_U32 Width;
  RKADK_U32 Height;
  RKADK_VOID *pData;
  RKADK_FORMAT_E Format;
} RKADK_OSD_ATTR_S;

typedef struct {
  RKADK_BOOL bEnableShow;
  RKADK_U32 Origin_X;
  RKADK_U32 Origin_Y;
} RKADK_OSD_STREAM_ATTR_S;

RKADK_S32 RKADK_OSD_Init(RKADK_U32 u32OsdId, RKADK_OSD_ATTR_S *pstOsdAttr);

RKADK_S32 RKADK_OSD_Deinit(RKADK_U32 u32OsdId);

RKADK_S32 RKADK_OSD_AttachToStream(RKADK_U32 u32OsdId, RKADK_U32 u32CamId,
                                   RKADK_STREAM_TYPE_E enStrmType,
                                   RKADK_OSD_STREAM_ATTR_S *pstOsdStreamAttr);

RKADK_S32 RKADK_OSD_DettachFromStream(RKADK_U32 u32OsdId, RKADK_U32 u32CamId,
                                      RKADK_STREAM_TYPE_E enStrmType);

RKADK_S32 RKADK_OSD_UpdateBitMap(RKADK_U32 u32OsdId,
                                 RKADK_OSD_ATTR_S *pstOsdAttr);

RKADK_S32
RKADK_OSD_UpdateDisplayAttr(RKADK_U32 u32OsdId, RKADK_U32 u32CamId,
                            RKADK_STREAM_TYPE_E enStrmType,
                            RKADK_OSD_STREAM_ATTR_S *pstOsdStreamAttr);

RKADK_S32 RKADK_OSD_UpdateOsdSize(RKADK_U32 u32OsdId,
                                  RKADK_OSD_ATTR_S *pstOsdAttr);

#ifdef __cplusplus
}
#endif
#endif
