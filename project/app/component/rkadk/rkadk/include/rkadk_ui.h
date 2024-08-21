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

#ifndef __RKADK_UI_H__
#define __RKADK_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rkadk_common.h"

typedef struct {
  RKADK_U32 u32DispX;
  RKADK_U32 u32DispY;
  RKADK_U32 u32DispWidth;
  RKADK_U32 u32DispHeight;
  RKADK_U32 u32DispFrmRt;
  RKADK_U32 u32ImgWidth;
  RKADK_U32 u32ImgHeight;
  RKADK_U32 u32VoLay;
  RKADK_U32 u32VoDev;
  RKADK_U32 u32VoChn;
  RKADK_U32 u32Rotation; //0: 0, 1: 90, 2: 180, 3: 270
  RKADK_BOOL bMirror;
  RKADK_BOOL bFlip;
  RKADK_VO_FORMAT_E enUiVoFormat;
  RKADK_VO_INTF_TYPE_E enUiVoIntfTye;
  RKADK_VO_SPLICE_MODE_E enVoSpliceMode;
} RKADK_UI_ATTR_S;

typedef struct {
  RKADK_U32 u32Width;
  RKADK_U32 u32Height;
  RKADK_FORMAT_E Format;
  RKADK_VOID *pMblk;
} RKADK_UI_FRAME_INFO;

RKADK_S32 RKADK_UI_Create(RKADK_UI_ATTR_S *pstUiAttr, RKADK_MW_PTR *ppUi);

RKADK_S32 RKADK_UI_Destroy(RKADK_MW_PTR pUi);

RKADK_S32 RKADK_UI_Update(RKADK_MW_PTR pUi, RKADK_UI_FRAME_INFO *pstUiFrameInfo);

#ifdef __cplusplus
}
#endif
#endif
