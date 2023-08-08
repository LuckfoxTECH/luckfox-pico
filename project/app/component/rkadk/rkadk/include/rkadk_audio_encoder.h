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

#ifndef __RKADK_AUDIO_ENCODER_H__
#define __RKADK_AUDIO_ENCODER_H__

#include "rkadk_common.h"

/**
 * @brief register extra audio encoder
 * @param[in] eCodecType : codec type of audio encoder
 * @retval  0 success, others failed
 */
RKADK_S32 RKADK_AUDIO_ENCODER_Register(RKADK_CODEC_TYPE_E eCodecType);

/**
 * @brief unregister extra audio encoder
 * @param[in] eCodecType : codec type of the audio encoder
 * @retval  0 success, others failed
 */
RKADK_S32 RKADK_AUDIO_ENCODER_UnRegister(RKADK_CODEC_TYPE_E eCodecType);

#endif
