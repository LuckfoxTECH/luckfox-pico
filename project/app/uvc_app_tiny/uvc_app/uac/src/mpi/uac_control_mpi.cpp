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

#include "uac_control_mpi.h"
#include "mpi_control_common.h"
#include "uac_amixer.h"
#include "uac_log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "uac_mpi"
#endif

#define OPEN_VQE 1

typedef struct _UacControlMpi {
  int mode;
  UacMpiStream stream;
} UacControlMpi;

UacControlMpi *getContextMpi(void *context) {
  UacControlMpi *ctx = reinterpret_cast<UacControlMpi *>(context);
  return ctx;
}

UACControlMpi::UACControlMpi(int mode) {
  UacControlMpi *ctx = (UacControlMpi *)calloc(1, sizeof(UacControlMpi));
  memset(ctx, 0, sizeof(UacControlMpi));

  ctx->mode = mode;
  ctx->stream.config.intVol = 100;
  ctx->stream.config.mute = 0;
  ctx->stream.config.ppm = 0;

  if (mode == UAC_STREAM_PLAYBACK) {
    ctx->stream.idCfg.aiDevId = (AUDIO_DEV)AI_MIC_DEV;
    ctx->stream.idCfg.aoDevId = (AUDIO_DEV)AO_USB_DEV;
    ctx->stream.config.samplerate =
        UacMpiUtil::getDataSamplerate(UAC_MPI_TYPE_AO, ctx->mode);
  } else if (mode == UAC_STREAM_RECORD) {
    ctx->stream.idCfg.aiDevId = (AUDIO_DEV)AI_USB_DEV;
    ctx->stream.idCfg.aoDevId = (AUDIO_DEV)AO_SPK_DEV;
    ctx->stream.config.samplerate =
        UacMpiUtil::getDataSamplerate(UAC_MPI_TYPE_AI, ctx->mode);

    // always set index of spk's sound card(hw:0,0)
    amix_set_ref_mode(MIC_SPK_SOUNDCARD_INDEX);
  }

  mCtx = reinterpret_cast<void *>(ctx);
}

UACControlMpi::~UACControlMpi() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  if (ctx) {
    free(ctx);
  }

  mCtx = NULL;
}

void UACControlMpi::uacSetSampleRate(int sampleRate) {
  UacControlMpi *ctx = getContextMpi(mCtx);
  ctx->stream.config.samplerate = sampleRate;
  ALOGD("mode = %d, sampleRate = %d\n", ctx->mode, sampleRate);
  if ((ctx->stream.flag &= UAC_MPI_ENABLE) == UAC_MPI_ENABLE) {
    mpi_set_samplerate(ctx->mode, ctx->stream);
  }
}

void UACControlMpi::uacSetVolume(int volume) {
  UacControlMpi *ctx = getContextMpi(mCtx);
  ALOGD("mode = %d, volume = %d\n", ctx->mode, volume);
  ctx->stream.config.intVol = volume;
  if ((ctx->stream.flag &= UAC_MPI_ENABLE) == UAC_MPI_ENABLE) {
    mpi_set_volume(ctx->mode, ctx->stream);
  }
}

void UACControlMpi::uacSetMute(int mute) {
  UacControlMpi *ctx = getContextMpi(mCtx);
  ALOGD("mode = %d, mute = %d\n", ctx->mode, mute);
  ctx->stream.config.mute = mute;
  if ((ctx->stream.flag &= UAC_MPI_ENABLE) == UAC_MPI_ENABLE) {
    mpi_set_volume(ctx->mode, ctx->stream);
  }
}

void UACControlMpi::uacSetPpm(int ppm) {
  ALOGD("ppm = %d\n", ppm);
  UacControlMpi *ctx = getContextMpi(mCtx);
  ctx->stream.config.ppm = ppm;
  if ((ctx->stream.flag &= UAC_MPI_ENABLE) == UAC_MPI_ENABLE) {
    mpi_set_ppm(ctx->mode, ctx->stream);
  }
}

int UACControlMpi::uacStart() {
  uacStop();
  int ret = 0;
  UacControlMpi *ctx = getContextMpi(mCtx);
  ret = startAi();
  if (ret != 0) {
    goto __FAILED;
  }

  /*
   * only add 3A process in playback(data flow: 1106's mic-->pc/host),
   * do not add 3A process in record(data flow: pc/host-->1106's spk )
   * for avoid sound like music process by 3A
   */
  if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
    ret = startVqe();
    if (ret != 0) {
      goto __FAILED;
    }
  }

  ret = startAo();
  if (ret != 0) {
    goto __FAILED;
  }

  mpi_set_samplerate(ctx->mode, ctx->stream);
  mpi_set_volume(ctx->mode, ctx->stream);
  mpi_set_ppm(ctx->mode, ctx->stream);

  streamBind();
  ctx->stream.flag |= UAC_MPI_ENABLE;
  return 0;

__FAILED:
  return -1;
}

void UACControlMpi::uacStop() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  ALOGD("stop mode = %d, flag = %d\n", ctx->mode, ctx->stream.flag);
  if ((ctx->stream.flag &= UAC_MPI_ENABLE) == UAC_MPI_ENABLE) {
    streamUnBind();
    stopAi();
    if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
      stopVqe();
    }
    stopAo();
    ctx->stream.flag &= (~UAC_MPI_ENABLE);
  }
}

int UACControlMpi::startAi() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  AUDIO_DEV aiDevId = ctx->stream.idCfg.aiDevId;
  AI_CHN aiChn = ctx->stream.idCfg.aiChnId;
  ALOGD("this:%p, startAi(dev:%d, chn:%d), mode : %d\n", this, aiDevId, aiChn,
        ctx->mode);
  const char *cardName = RK_NULL;
  RK_S32 result = 0;
  AUDIO_SAMPLE_RATE_E rate;
  AIO_ATTR_S aiAttr;
  memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

  cardName = UacMpiUtil::getSndCardName(UAC_MPI_TYPE_AI, ctx->mode);
  snprintf(reinterpret_cast<char *>(aiAttr.u8CardName),
           sizeof(aiAttr.u8CardName), "%s", cardName);
  aiAttr.soundCard.channels =
      UacMpiUtil::getSndCardChannels(UAC_MPI_TYPE_AI, ctx->mode);
  aiAttr.soundCard.sampleRate =
      UacMpiUtil::getSndCardSampleRate(UAC_MPI_TYPE_AI, ctx->mode);
  aiAttr.soundCard.bitWidth =
      UacMpiUtil::getSndCardbitWidth(UAC_MPI_TYPE_AI, ctx->mode);

  /*
   * 1. if datas are sended from pc to uac device, the ai device is usb,
   *    we set the samplerate which uevent report.
   * 2. if datas are sended from uac device to pc, the ai device is mic,
        we use the a fix samplerate like 16000.
   */
  if (ctx->mode == UAC_STREAM_PLAYBACK) {
    if (OPEN_VQE) {
      rate = (AUDIO_SAMPLE_RATE_E)UacMpiUtil::getVqeSampleRate();
    } else {
      rate = (AUDIO_SAMPLE_RATE_E)UacMpiUtil::getSndCardSampleRate(
          UAC_MPI_TYPE_AI, ctx->mode);
    }
  } else {
    rate = (AUDIO_SAMPLE_RATE_E)ctx->stream.config.samplerate;
    // update samplerate of usb cards which coming from uevent
    aiAttr.soundCard.sampleRate = rate;
  }

  aiAttr.enBitwidth = UacMpiUtil::getDataBitwidth(UAC_MPI_TYPE_AI, ctx->mode);
  aiAttr.enSamplerate = rate;
  aiAttr.enSoundmode = UacMpiUtil::getDataSoundmode(UAC_MPI_TYPE_AI, ctx->mode);
  ALOGD("this:%p, startAi(dev:%d, chn:%d), enSamplerate : %d\n", this, aiDevId,
        aiChn, aiAttr.enSamplerate);
  aiAttr.u32FrmNum = 4;
  aiAttr.u32PtNumPerFrm = 1024;
  aiAttr.u32EXFlag = 0;
  aiAttr.u32ChnCnt = 2;
  result = RK_MPI_AI_SetPubAttr(aiDevId, &aiAttr);
  if (result != 0) {
    ALOGE("ai set attr(dev:%d) fail, reason = %x\n", aiDevId, result);
    goto __FAILED;
  }

  result = RK_MPI_AI_Enable(aiDevId);
  if (result != 0) {
    ALOGE("ai enable(dev:%d) fail, reason = %x\n", aiDevId, result);
    goto __FAILED;
  }

  result = RK_MPI_AI_EnableChn(aiDevId, aiChn);
  if (result != 0) {
    ALOGE("ai enable channel(dev:%d, chn:%d) fail, reason = %x\n", aiDevId,
          aiChn, result);
    return RK_FAILURE;
  }

  /*
   * If 3A filter is after ai, and the samplerate of ai's datas is not support
   * by 3A,
   * enable resample to convert the samplerate.
   */
  if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
    result = RK_MPI_AI_EnableReSmp(aiDevId, aiChn, aiAttr.enSamplerate);
    if (result != 0) {
      ALOGE("ai enable resample(dev:%d, chn:%d) fail, reason = %x\n", aiDevId,
            aiChn, result);
      return RK_FAILURE;
    }
  } else {
    // disable resample in ai, this means the samplerate of output of ai is the
    // samplerate of sound card
    result = RK_MPI_AI_DisableReSmp(aiDevId, aiChn);
    if (result != 0) {
      ALOGE("ai disable resample(dev:%d, chn:%d) fail, reason = %x\n", aiDevId,
            aiChn, result);
      return RK_FAILURE;
    }
  }
  return 0;
__FAILED:
  return -1;
}

// init 3A filter
int UACControlMpi::startVqe() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  AF_CHN vqeChnId = ctx->stream.idCfg.vqeChnId;
  RK_S32 result;
  AF_ATTR_S attr;
  ALOGD("this:%p, startVqe(chn:%d), mode : %d\n", this, vqeChnId, ctx->mode);
  memset(&attr, 0, sizeof(AF_ATTR_S));

  attr.enType = AUDIO_FILTER_3A;
  attr.u32InBufCount = 2;
  attr.u32OutBufCount = 2;

  snprintf(reinterpret_cast<char *>(attr.st3AAttr.cfgPath),
           sizeof(attr.st3AAttr.cfgPath), "%s", UacMpiUtil::getVqeCfgPath());
  attr.st3AAttr.u32SampleRate = UacMpiUtil::getVqeSampleRate();
  attr.st3AAttr.enBitWidth = AUDIO_BIT_WIDTH_16;
  attr.st3AAttr.u32Channels = UacMpiUtil::getVqeChannels();
  attr.st3AAttr.u32ChnLayout = UacMpiUtil::getVqeChnLayout();
  attr.st3AAttr.u32RecLayout = UacMpiUtil::getVqeMicLayout();
  attr.st3AAttr.u32RefLayout = UacMpiUtil::getVqeRefLayout();
  result = RK_MPI_AF_Create(vqeChnId, &attr);
  if (result != RK_SUCCESS) {
    ALOGE("create af vqe(chn:%d) fail, reason = %x\n", vqeChnId, result);
    return RK_FAILURE;
  }

  return 0;
__FAILED:
  return -1;
}

int UACControlMpi::startAo() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  AUDIO_DEV aoDevId = ctx->stream.idCfg.aoDevId;
  AO_CHN aoChn = ctx->stream.idCfg.aoChnId;
  ALOGD("this:%p, startAo(dev:%d, chn:%d), mode : %d\n", this, aoDevId, aoChn,
        ctx->mode);
  const char *cardName = RK_NULL;
  RK_S32 result;
  AIO_ATTR_S aoAttr;
  AUDIO_SAMPLE_RATE_E rate;
  AUDIO_SOUND_MODE_E soundMode;
  memset(&aoAttr, 0, sizeof(AIO_ATTR_S));

  cardName = UacMpiUtil::getSndCardName(UAC_MPI_TYPE_AO, ctx->mode);
  snprintf(reinterpret_cast<char *>(aoAttr.u8CardName),
           sizeof(aoAttr.u8CardName), "%s", cardName);

  /*
   * 1. if datas is from pc to uac device, the ao device is spk,
        we use the a fix samplerate like 48000.
   * 2. if datas is from uac device to pc, the ao device is usb,
   *    we set the samplerate which uevent report.
   */
  if (ctx->mode == UAC_STREAM_RECORD) {
    rate = (AUDIO_SAMPLE_RATE_E)UacMpiUtil::getSndCardSampleRate(
        UAC_MPI_TYPE_AO, ctx->mode);
  } else {
    rate = (AUDIO_SAMPLE_RATE_E)ctx->stream.config.samplerate;
  }

  aoAttr.soundCard.channels =
      UacMpiUtil::getSndCardChannels(UAC_MPI_TYPE_AO, ctx->mode);
  aoAttr.soundCard.sampleRate = rate;
  aoAttr.soundCard.bitWidth =
      UacMpiUtil::getSndCardbitWidth(UAC_MPI_TYPE_AO, ctx->mode);

  aoAttr.enBitwidth = UacMpiUtil::getDataBitwidth(UAC_MPI_TYPE_AO, ctx->mode);
  if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
    rate = (AUDIO_SAMPLE_RATE_E)UacMpiUtil::getVqeSampleRate();
  }
  aoAttr.enSamplerate = rate;
  soundMode = UacMpiUtil::getDataSoundmode(UAC_MPI_TYPE_AO, ctx->mode);
  if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
    soundMode = AUDIO_SOUND_MODE_MONO;
  }
  aoAttr.enSoundmode = soundMode;
  ALOGD("this:%p, startAo(dev:%d, chn:%d), mode : %d, enSamplerate = %d\n",
        this, aoDevId, aoChn, ctx->mode, aoAttr.enSamplerate);
  aoAttr.u32FrmNum = 4;
  aoAttr.u32PtNumPerFrm = 1024;
  aoAttr.u32EXFlag = 0;
  aoAttr.u32ChnCnt = 2;
  result = RK_MPI_AO_SetPubAttr(aoDevId, &aoAttr);
  if (result != 0) {
    ALOGE("ao set attr(dev:%d) fail, reason = %x\n", aoDevId, result);
    goto __FAILED;
  }

  result = RK_MPI_AO_Enable(aoDevId);
  if (result != 0) {
    ALOGE("ao enable(dev:%d) fail, reason = %x\n", aoDevId, result);
    goto __FAILED;
  }

  result = RK_MPI_AO_EnableChn(aoDevId, aoChn);
  if (result != 0) {
    ALOGE("ao enable channel(dev:%d, chn:%d) fail, reason = %x\n", aoDevId,
          aoChn, result);
    return RK_FAILURE;
  }

  result = RK_MPI_AO_EnableReSmp(aoDevId, aoChn, aoAttr.enSamplerate);
  if (result != 0) {
    ALOGE("ao enable resample(dev:%d, chn:%d) fail, reason = %x\n", aoDevId,
          aoChn, result);
    return RK_FAILURE;
  }

  if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
    result = RK_MPI_AO_SetTrackMode(aoDevId, AUDIO_TRACK_OUT_STEREO);
    if (result != 0) {
      ALOGE("ao enable track mode(dev:%d, chn:%d) fail, reason = %x\n", aoDevId,
            aoChn, result);
      return RK_FAILURE;
    }
  }
  return 0;
__FAILED:
  return -1;
}

void UACControlMpi::streamBind() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  MPP_CHN_S stSrcChn, stDstChn;
  // have af, the data flow is ai-->af-->ao
  if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
    // ai bind af, this means data from ai to af
    stSrcChn.enModId = RK_ID_AI;
    stSrcChn.s32DevId = ctx->stream.idCfg.aiDevId;
    stSrcChn.s32ChnId = ctx->stream.idCfg.aiChnId;

    stDstChn.enModId = RK_ID_AF;
    stDstChn.s32DevId = 0;
    stDstChn.s32ChnId = ctx->stream.idCfg.vqeChnId;
    ALOGD("AiBindVqe(mode:%d) : src(%d,%d), dst(%d,%d)\n", ctx->mode,
          stSrcChn.s32DevId, stSrcChn.s32ChnId, stDstChn.s32DevId,
          stDstChn.s32ChnId);
    RK_MPI_SYS_Bind(&stSrcChn, &stDstChn);

    // af bind ao, this means data from af to ao
    stSrcChn.enModId = RK_ID_AF;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = ctx->stream.idCfg.vqeChnId;

    stDstChn.enModId = RK_ID_AO;
    stDstChn.s32DevId = ctx->stream.idCfg.aoDevId;
    stDstChn.s32ChnId = ctx->stream.idCfg.aoChnId;
    ALOGD("VqeBindAo(mode:%d) : src(%d,%d), dst(%d,%d)\n", ctx->mode,
          stSrcChn.s32DevId, stSrcChn.s32ChnId, stDstChn.s32DevId,
          stDstChn.s32ChnId);
    RK_MPI_SYS_Bind(&stSrcChn, &stDstChn);
  } else {
    // no af, the data flow is ai-->ao
    stSrcChn.enModId = RK_ID_AI;
    stSrcChn.s32DevId = ctx->stream.idCfg.aiDevId;
    stSrcChn.s32ChnId = ctx->stream.idCfg.aiChnId;

    stDstChn.enModId = RK_ID_AO;
    stDstChn.s32DevId = ctx->stream.idCfg.aoDevId;
    stDstChn.s32ChnId = ctx->stream.idCfg.aoChnId;
    ALOGD("AiBindAO(mode:%d) : src(%d,%d), dst(%d,%d)\n", ctx->mode,
          stSrcChn.s32DevId, stSrcChn.s32ChnId, stDstChn.s32DevId,
          stDstChn.s32ChnId);
    RK_MPI_SYS_Bind(&stSrcChn, &stDstChn);
  }
}

int UACControlMpi::stopAi() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  AUDIO_DEV aiDevId = ctx->stream.idCfg.aiDevId;
  AI_CHN aiChn = ctx->stream.idCfg.aiChnId;
  ALOGD("this:%p, stopAi(dev:%d, chn:%d), mode : %d\n", this, aiDevId, aiChn,
        ctx->mode);
  RK_MPI_AI_DisableReSmp(aiDevId, aiChn);
  RK_S32 result = RK_MPI_AI_DisableChn(aiDevId, aiChn);
  if (result != 0) {
    ALOGE("ai disable channel(dev:%d, chn:%d) fail, reason = %x\n", aiDevId,
          aiChn, result);
    return RK_FAILURE;
  }

  result = RK_MPI_AI_Disable(aiDevId);
  if (result != 0) {
    ALOGE("ai disable(dev:%d) fail, reason = %x\n", aiDevId, result);
    return RK_FAILURE;
  }

  return 0;
__FAILED:
  return -1;
}

int UACControlMpi::stopVqe() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  AF_CHN vqeChn = ctx->stream.idCfg.vqeChnId;
  ALOGD("this:%p, stopVqe(chn:%d), mode : %d\n", this, vqeChn, ctx->mode);
  RK_S32 result = RK_MPI_AF_Destroy(vqeChn);
  if (result != 0) {
    ALOGE("vqe disable(dev:%d) fail, reason = %x\n", vqeChn, result);
    return RK_FAILURE;
  }

  return 0;
__FAILED:
  return -1;
}

int UACControlMpi::stopAo() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  AUDIO_DEV aoDevId = ctx->stream.idCfg.aoDevId;
  AO_CHN aoChn = ctx->stream.idCfg.aoChnId;
  ALOGD("this:%p, stopAo(dev:%d, chn:%d), mode : %d\n", this, aoDevId, aoChn,
        ctx->mode);
  RK_MPI_AO_DisableReSmp(aoDevId, aoChn);
  RK_S32 result = RK_MPI_AO_DisableChn(aoDevId, aoChn);
  if (result != 0) {
    ALOGE("ao disable channel(dev:%d, chn:%d) fail, reason = %x\n", aoDevId,
          aoChn, result);
    return RK_FAILURE;
  }

  result = RK_MPI_AO_Disable(aoDevId);
  if (result != 0) {
    ALOGE("ao disable(dev:%d) fail, reason = %x\n", aoDevId, result);
    return RK_FAILURE;
  }

  return 0;
__FAILED:
  return -1;
}

void UACControlMpi::streamUnBind() {
  UacControlMpi *ctx = getContextMpi(mCtx);
  MPP_CHN_S stSrcChn, stDstChn;
  if (OPEN_VQE && ctx->mode == UAC_STREAM_PLAYBACK) {
    stSrcChn.enModId = RK_ID_AI;
    stSrcChn.s32DevId = ctx->stream.idCfg.aiDevId;
    stSrcChn.s32ChnId = ctx->stream.idCfg.aiChnId;

    stDstChn.enModId = RK_ID_AF;
    stDstChn.s32DevId = 0;
    stDstChn.s32ChnId = ctx->stream.idCfg.vqeChnId;
    ALOGD("AiUnBindVqe(mode:%d) : src(%d,%d), dst(%d,%d)\n", ctx->mode,
          stSrcChn.s32DevId, stSrcChn.s32ChnId, stDstChn.s32DevId,
          stDstChn.s32ChnId);
    RK_MPI_SYS_UnBind(&stSrcChn, &stDstChn);

    stSrcChn.enModId = RK_ID_AF;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = ctx->stream.idCfg.vqeChnId;

    stDstChn.enModId = RK_ID_AO;
    stDstChn.s32DevId = ctx->stream.idCfg.aoDevId;
    stDstChn.s32ChnId = ctx->stream.idCfg.aoChnId;
    ALOGD("VqeUnBindAo(mode:%d) : src(%d,%d), dst(%d,%d)\n", ctx->mode,
          stSrcChn.s32DevId, stSrcChn.s32ChnId, stDstChn.s32DevId,
          stDstChn.s32ChnId);
    RK_MPI_SYS_UnBind(&stSrcChn, &stDstChn);
  } else {
    stSrcChn.enModId = RK_ID_AI;
    stSrcChn.s32DevId = ctx->stream.idCfg.aiDevId;
    stSrcChn.s32ChnId = ctx->stream.idCfg.aiChnId;

    stDstChn.enModId = RK_ID_AO;
    stDstChn.s32DevId = ctx->stream.idCfg.aoDevId;
    stDstChn.s32ChnId = ctx->stream.idCfg.aoChnId;
    ALOGD("AiUnBindAO(mode:%d) : src(%d,%d), dst(%d,%d)\n", ctx->mode,
          stSrcChn.s32DevId, stSrcChn.s32ChnId, stDstChn.s32DevId,
          stDstChn.s32ChnId);
    RK_MPI_SYS_UnBind(&stSrcChn, &stDstChn);
  }
}
//#endif