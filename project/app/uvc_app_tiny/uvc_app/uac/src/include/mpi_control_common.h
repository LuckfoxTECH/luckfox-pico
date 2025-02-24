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
#ifndef SRC_MPI_MPI_CONTROL_H_
#define SRC_MPI_MPI_CONTROL_H_

#ifdef __cplusplus

#include "uac_common_def.h"
#include <rk_comm_af.h>
#include <rk_comm_aio.h>
#include <rk_debug.h>
#include <rk_mpi_af.h>
#include <rk_mpi_ai.h>
#include <rk_mpi_amix.h>
#include <rk_mpi_ao.h>
#include <rk_mpi_sys.h>
#include <rk_type.h>

#define UAC_MPI_ENABLE (1 << 1)
#define MIC_SPK_SOUNDCARD_INDEX 0

typedef enum _UacMpiType {
  UAC_MPI_TYPE_AI = 0,
  UAC_MPI_TYPE_AO = 1,
  UAC_MPI_TYPE_AF_VQE = 2,
  UAC_MPI_TYPE_MAX
} UacMpiType;

typedef enum _UacMpiAODevId {
  AO_USB_DEV, // ao[0]
  AO_SPK_DEV, // ao[1]
} UacMpiAODevId;

typedef enum _UacMpiAIDevId {
  AI_MIC_DEV, // ai[0]
  AI_USB_DEV, // ai[1]
} UacMpiAIDevId;

typedef enum _UacMpiAFChnId {
  AF_VQE_CHN, // af[0]
} UacMpiAFChnId;

typedef struct _UacMpiIdConfig {
  AUDIO_DEV aoDevId;
  AUDIO_DEV aiDevId;
  AO_CHN aoChnId;
  AI_CHN aiChnId;
  AF_CHN vqeChnId;
} UacMpiIdConfig;

typedef struct _UacMpiStream {
  int flag;
  UacMpiIdConfig idCfg;
  UacAudioConfig config;
} UacMpiStream;

class UacMpiUtil {
public:
  static const char *getSndCardName(UacMpiType type, int mode);
  static RK_U32 getSndCardChannels(UacMpiType type, int mode);
  static RK_U32 getSndCardSampleRate(UacMpiType type, int mode);
  static AUDIO_BIT_WIDTH_E getSndCardbitWidth(UacMpiType type, int mode);
  static RK_U32 getDataSamplerate(UacMpiType type, int mode);
  static AUDIO_BIT_WIDTH_E getDataBitwidth(UacMpiType type, int mode);
  static AUDIO_SOUND_MODE_E getDataSoundmode(UacMpiType type, int mode);
  static const char *getVqeCfgPath();
  static RK_U32 getVqeSampleRate();
  static RK_U32 getVqeChannels();
  static RK_U32 getVqeChnLayout();
  static RK_U32 getVqeRefLayout();
  static RK_U32 getVqeMicLayout();
};

void mpi_set_samplerate(int type, UacMpiStream &streamCfg);
void mpi_set_volume(int type, UacMpiStream &streamCfg);
void mpi_set_ppm(int type, UacMpiStream &streamCfg);

#endif
#endif // SRC_MPI_MPI_CONTROL_H_
