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

#ifndef __RKADK_MP3_HEADER_H__
#define __RKADK_MP3_HEADER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rkadk_common.h"

#define MP3_PROFILE_LOW 1

typedef struct {
  RKADK_S32 u32SmpRate;
  RKADK_U8 u8FreqIdx;
} MP3_PREQ_IDX_S;

void GetMp3Header(RKADK_U8 *pu8Mp3Hdr, RKADK_S32 u32SmpRate,
                   RKADK_U8 u8Channel, RKADK_U32 u32DataLen);

#ifdef __cplusplus
}
#endif
#endif
