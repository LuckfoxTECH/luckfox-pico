/*
 * Copyright 2022 Rockchip Electronics Co. LTD
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

#ifndef SRC_MPI_MPI_UAC_AMIXER_H_
#define SRC_MPI_MPI_UAC_AMIXER_H_

#include "rk_common.h"

typedef enum _HARDWARE_REF_MODE {
  HARDWARE_REF_DISABLE = 0, // disable hardware ref channel
  HARDWARE_REF_MODE1,       // 4chn: 2 mic + 1 ref
  HARDWARE_REF_MODE2,       // 2chn: 1 mic + 1 ref
  HARDWARE_REF_MODE2_SWAP   // 2chn: 1 ref + 1 mic
} HARDWARE_REF_MODE;

typedef struct _MpiChnAttrConfigMap {
  RK_U32 u32Index;
  RK_U32 u32Channels;
  RK_U32 u32MicLayout;
  RK_U32 u32RefLayout;
  const char *chMode;
} MpiChnAttrConfigMap;

// the hardware of our RV1106's evb board support mode2
const static int sRefMode = HARDWARE_REF_MODE2;
const static MpiChnAttrConfigMap sChnAttrCfgs[] = {
    {HARDWARE_REF_DISABLE, 2, 0x3, 0x0,
     "Disable"},                                // disable hardware ref channel
    {HARDWARE_REF_MODE1, 4, 0x3, 0xc, "Mode1"}, // 4chn: 2 mic + 1 ref
    {HARDWARE_REF_MODE2, 2, 0x1, 0x2,
     "Mode2"}, // 2chn: 1 mic(ADC Left) + 1 ref(DAC Right)
    {HARDWARE_REF_MODE2_SWAP, 2, 0x2, 0x1,
     "Mode2 Swap"}, // 2chn: 1 ref(DAC Left) + 1 mic(ADC Right)
};

class UACAmixer {
public:
  static RK_S32 set(AUDIO_DEV AmixDevId, const char *ctrlName,
                    const char *value);
  static RK_S32 get(AUDIO_DEV AmixDevId, const char *ctrlName);
  static void dumpContents(AUDIO_DEV AmixDevId);
};

RK_S32 amix_set_ref_mode(AUDIO_DEV AoDevId);
RK_S32 amix_set_ppm(AUDIO_DEV AoDevId, RK_S32 ppm);

#endif // SRC_MPI_MPI_UAC_AMIXER_H_
