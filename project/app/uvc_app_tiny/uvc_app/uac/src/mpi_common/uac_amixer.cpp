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

#include "uac_amixer.h"
#include "rk_mpi_amix.h"
#include "uac_log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "amixer"
#endif

const static char *sRefCtrlName = "I2STDM Digital Loopback Mode";
const static char *sPpmCtrlName = "PCM Clk Compensation In PPM";

RK_S32 UACAmixer::set(AUDIO_DEV AmixDevId, const char *ctrlName,
                      const char *value) {
  ALOGD("AmixDevId = %d, ctrl = %s, value = %s", AmixDevId, ctrlName, value);
  return RK_MPI_AMIX_SetControl(AmixDevId, ctrlName, (char *)value);
}

RK_S32 UACAmixer::get(AUDIO_DEV AmixDevId, const char *ctrlName) {
  return RK_MPI_AMIX_GetControl(AmixDevId, ctrlName, NULL);
}

void UACAmixer::dumpContents(AUDIO_DEV AmixDevId) {
  RK_MPI_AMIX_ListContents(AmixDevId);
}

RK_S32 amix_set_ref_mode(AUDIO_DEV AoDevId) {
  return UACAmixer::set(AoDevId, sRefCtrlName, sChnAttrCfgs[sRefMode].chMode);
}

RK_S32 amix_set_ppm(AUDIO_DEV AoDevId, RK_S32 ppm) {
  char str[64];
  snprintf(str, sizeof(str), "%d", ppm);
  return UACAmixer::set(AoDevId, sPpmCtrlName, str);
}
