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
//#ifdef __cplusplus

#include "mpi_control_common.h"
#include "uac_amixer.h"
#include "uac_log.h"
#include "uac_log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "mpi_contol_comm"
#endif

typedef struct _MpiAioDeviceAttrConfigMap {
  UacMpiType mpiType;
  int uacMode;

  const char *sndCardname;
  RK_U32 sndCardChannels;
  RK_U32 sndCardSampleRate;
  AUDIO_BIT_WIDTH_E sndCardbitWidth;

  RK_U32 dataSamplerate;
  AUDIO_BIT_WIDTH_E dataBitwidth;
  AUDIO_SOUND_MODE_E dataSoundmode;
} MpiAioDeviceAttrConfigMap;

const static MpiAioDeviceAttrConfigMap sAioDevAttrCfgs[] = {
    // usb
    {UAC_MPI_TYPE_AI, UAC_STREAM_RECORD, "hw:1,0", 2, 44100, AUDIO_BIT_WIDTH_16,
     44100, AUDIO_BIT_WIDTH_16, AUDIO_SOUND_MODE_STEREO},
    // mic
    {UAC_MPI_TYPE_AI, UAC_STREAM_PLAYBACK, "hw:0,0", 2, 16000,
     AUDIO_BIT_WIDTH_16, 16000, AUDIO_BIT_WIDTH_16, AUDIO_SOUND_MODE_STEREO},
    // spk
    {UAC_MPI_TYPE_AO, UAC_STREAM_RECORD, "hw:0,0", 2, 16000, AUDIO_BIT_WIDTH_16,
     16000, AUDIO_BIT_WIDTH_16, AUDIO_SOUND_MODE_STEREO},
    // usb
    {UAC_MPI_TYPE_AO, UAC_STREAM_PLAYBACK, "hw:1,0", 2, 44100,
     AUDIO_BIT_WIDTH_16, 44100, AUDIO_BIT_WIDTH_16, AUDIO_SOUND_MODE_STEREO},
};

const static char *sAfVqeCfgPath = "/oem/usr/share/uac_app/configs_skv.json";
#define VQE_SAMPLERATE 16000
RK_U32 UacMpiUtil::getSndCardSampleRate(UacMpiType type, int mode) {
  GET_ENTRY_VALUE(type, mode, sAioDevAttrCfgs, mpiType, uacMode,
                  sndCardSampleRate);
  return 0;
}

const char *UacMpiUtil::getSndCardName(UacMpiType type, int mode) {
  GET_ENTRY_VALUE(type, mode, sAioDevAttrCfgs, mpiType, uacMode, sndCardname);
  return NULL;
}

RK_U32 UacMpiUtil::getSndCardChannels(UacMpiType type, int mode) {
  GET_ENTRY_VALUE(type, mode, sAioDevAttrCfgs, mpiType, uacMode,
                  sndCardChannels);
  return 0;
}

AUDIO_BIT_WIDTH_E UacMpiUtil::getSndCardbitWidth(UacMpiType type, int mode) {
  GET_ENTRY_VALUE(type, mode, sAioDevAttrCfgs, mpiType, uacMode,
                  sndCardbitWidth);
  return AUDIO_BIT_WIDTH_BUTT;
}

RK_U32 UacMpiUtil::getDataSamplerate(UacMpiType type, int mode) {
  GET_ENTRY_VALUE(type, mode, sAioDevAttrCfgs, mpiType, uacMode,
                  dataSamplerate);
  return 0;
}

AUDIO_BIT_WIDTH_E UacMpiUtil::getDataBitwidth(UacMpiType type, int mode) {
  GET_ENTRY_VALUE(type, mode, sAioDevAttrCfgs, mpiType, uacMode, dataBitwidth);
  return AUDIO_BIT_WIDTH_BUTT;
}

AUDIO_SOUND_MODE_E UacMpiUtil::getDataSoundmode(UacMpiType type, int mode) {
  GET_ENTRY_VALUE(type, mode, sAioDevAttrCfgs, mpiType, uacMode, dataSoundmode);
  return AUDIO_SOUND_MODE_BUTT;
}

const char *UacMpiUtil::getVqeCfgPath() { return sAfVqeCfgPath; }

RK_U32 UacMpiUtil::getVqeSampleRate() { return VQE_SAMPLERATE; }

RK_U32 UacMpiUtil::getVqeChannels() {
  return sChnAttrCfgs[sRefMode].u32Channels;
}

RK_U32 UacMpiUtil::getVqeChnLayout() {
  RK_U32 layout =
      sChnAttrCfgs[sRefMode].u32MicLayout + sChnAttrCfgs[sRefMode].u32RefLayout;
  return layout;
}

RK_U32 UacMpiUtil::getVqeRefLayout() {
  return sChnAttrCfgs[sRefMode].u32RefLayout;
}

RK_U32 UacMpiUtil::getVqeMicLayout() {
  return sChnAttrCfgs[sRefMode].u32MicLayout;
}

void mpi_set_samplerate(int type, UacMpiStream &streamCfg) {
  int sampleRate = streamCfg.config.samplerate;
  if (sampleRate == 0)
    return;
  AUDIO_DEV aiDevId = streamCfg.idCfg.aiDevId;
  AI_CHN aiChn = streamCfg.idCfg.aiChnId;
  AUDIO_DEV aoDevId = streamCfg.idCfg.aoDevId;
  AO_CHN aoChn = streamCfg.idCfg.aoChnId;
  ALOGD("type = %d, sampleRate = %d\n", type, sampleRate);
  /*
   * 1. for usb capture, we update audio config to capture
   * 2. for usb playback, if there is resample before usb playback,
   *    we set audio config to this resample, the new config will
   *    pass to usb playback from resample to usb playback when
   *    the datas move from resample to usb.
   * 3. we alway use samperate=48K to open mic and speaker,
   *    because usually, they use the same group i2s, and
   *    not allowned to use diffrent samplerate.
   */
  if (type == UAC_STREAM_RECORD) {
    // the usb record always the first node
    AI_CHN_ATTR_S params;
    memset(&params, 0, sizeof(AI_CHN_ATTR_S));
    params.u32SampleRate = sampleRate;
    params.enChnAttr = AUDIO_CHN_ATTR_RATE;
    RK_MPI_AI_SetChnAttr(aiDevId, aiChn, &params);
  } else {
    // find the resample before usb playback
    AO_CHN_ATTR_S params;
    memset(&params, 0, sizeof(AO_CHN_ATTR_S));
    params.u32SampleRate = sampleRate;
    params.enChnAttr = AUDIO_CHN_ATTR_RATE;
    RK_MPI_AO_SetChnAttr(aoDevId, aoChn, &params);
  }
}

void mpi_set_volume(int type, UacMpiStream &streamCfg) {
  int mute = streamCfg.config.mute;
  int volume = streamCfg.config.intVol;
  AUDIO_DEV aoDevId = streamCfg.idCfg.aoDevId;
  ALOGD("type = %d, mute = %d, volume = %d\n", type, mute, volume);
  AUDIO_FADE_S aFade;
  memset(&aFade, 0, sizeof(AUDIO_FADE_S));

  RK_BOOL bMute = (mute == 0) ? RK_FALSE : RK_TRUE;
  RK_MPI_AO_SetMute(aoDevId, bMute, &aFade);
  RK_MPI_AO_SetVolume(aoDevId, volume);
}

void mpi_set_ppm(int type, UacMpiStream &streamCfg) {
  int ppm = streamCfg.config.ppm;
  amix_set_ppm(MIC_SPK_SOUNDCARD_INDEX, ppm);
}
//#endif
