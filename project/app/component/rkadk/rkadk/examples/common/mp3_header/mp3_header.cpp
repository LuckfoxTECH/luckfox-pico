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

#include "mp3_header.h"

static MP3_PREQ_IDX_S MP3FreqIdxTbl[13] = {
    {96000, 0},  {88200, 1}, {64000, 2}, {48000, 3}, {44100, 4},
    {32000, 5},  {24000, 6}, {22050, 7}, {16000, 8}, {12000, 9},
    {11025, 10}, {8000, 11}, {7350, 12}};

void GetMp3Header(RKADK_U8 *pu8Mp3Hdr, RKADK_S32 u32SmpRate,
                   RKADK_U8 u8Channel, RKADK_U32 u32DataLen) {
  RKADK_U8 u8FreqIdx = 0;
  for (int i = 0; i < 13; i++) {
    if (u32SmpRate == MP3FreqIdxTbl[i].u32SmpRate) {
      u8FreqIdx = MP3FreqIdxTbl[i].u8FreqIdx;
      break;
    }
  }

  RKADK_U32 u32PacketLen = u32DataLen + 7;
  pu8Mp3Hdr[0] = 0xFF;
  pu8Mp3Hdr[1] = 0xF1;
  pu8Mp3Hdr[2] =
      ((MP3_PROFILE_LOW) << 6) + (u8FreqIdx << 2) + (u8Channel >> 2);
  pu8Mp3Hdr[3] = (((u8Channel & 3) << 6) + (u32PacketLen >> 11));
  pu8Mp3Hdr[4] = ((u32PacketLen & 0x7FF) >> 3);
  pu8Mp3Hdr[5] = (((u32PacketLen & 7) << 5) + 0x1F);
  pu8Mp3Hdr[6] = 0xFC;
}
