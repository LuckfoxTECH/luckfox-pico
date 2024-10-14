/*
 * Copyright (c) 2022 Rockchip, Inc. All Rights Reserved.
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

#include "rkadk_audio_encoder.h"
#include "rkadk_audio_encoder_mp3.h"

RKADK_S32 RKADK_AUDIO_ENCODER_Register(RKADK_CODEC_TYPE_E eCodecType) {
  int ret = 0;
  if (eCodecType == RKADK_CODEC_TYPE_MP3) {
    ret = RegisterAencMp3();
    if (ret) {
      RKADK_LOGE("Register Mp3 encoder failed(%d)", ret);
      return RKADK_FAILURE;
    }
  } else {
    RKADK_LOGE("Unsupported audio codec type[%d]", eCodecType);
    return RKADK_FAILURE;
  }

  return RKADK_SUCCESS;
}

RKADK_S32 RKADK_AUDIO_ENCODER_UnRegister(RKADK_CODEC_TYPE_E eCodecType) {
  int ret = 0;
  if (eCodecType == RKADK_CODEC_TYPE_MP3) {
    ret = UnRegisterAencMp3();
    if (ret) {
      RKADK_LOGE("UnRegister Mp3 encoder failed(%d)", ret);
      return RKADK_FAILURE;
    }
  } else {
    RKADK_LOGE("Unsupported audio codec type[%d]", eCodecType);
    return RKADK_FAILURE;
  }

  return RKADK_SUCCESS;
}
