// ai->aenc->adec_ao
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>

#include "rk_debug.h"
#include "rk_defines.h"
#include "rk_mpi_adec.h"
#include "rk_mpi_aenc.h"
#include "rk_mpi_ai.h"
#include "rk_mpi_ao.h"
#include "rk_mpi_amix.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "sample_comm.h"

#define AI_ALGO_FRAMES 256 // baed on 16kHz, it's  128 during 8kHz

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_U32 code_type = RK_AUDIO_ID_ADPCM_G726;

typedef struct _rkTestMpiAICtx {
	const char *srcFilePath;
	const char *dstFilePath;
	RK_S32 s32LoopCount;
	RK_S32 s32ChnNum;
	RK_S32 s32DeviceSampleRate;
	RK_S32 s32SampleRate;
	RK_S32 s32DeviceChannel;
	RK_S32 s32Channel;
	RK_S32 s32BitWidth;
	RK_S32 s32DevId;
	RK_S32 s32FrameNumber;
	RK_S32 s32FrameLength;
	char *chCardName;
	RK_S32 s32ChnIndex;
	RK_S32 s32SetVolumeCurve;
	RK_S32 s32SetVolume;
	RK_S32 s32SetMute;
	RK_S32 s32SetFadeRate;
	RK_S32 s32SetTrackMode;
	RK_S32 s32GetVolume;
	RK_S32 s32GetMute;
	RK_S32 s32GetTrackMode;
	RK_S32 s32LoopbackMode;
	RK_S32 s32DevFd;
	RK_S32 s32DataReadEnable;
	RK_S32 s32AedEnable;
	RK_S32 s32BcdEnable;
	RK_S32 s32BuzEnable;
	RK_S32 s32GbsEnable;
	RK_S32 s32AedLoudCount;
	RK_S32 s32BcdCount;
	RK_S32 s32BuzCount;
	RK_S32 s32GbsCount;
	RK_S32 s32VqeGapMs;
	RK_S32 s32VqeEnable;
	RK_S32 s32DumpAlgo;
	const char *pVqeCfgPath;
} TEST_AI_CTX_S;

RK_S32 init_ai_aed() {
	int s32DevId = 0;
	int s32ChnIndex = 0;
	RK_S32 result;

	AI_AED_CONFIG_S stAiAedConfig, stAiAedConfig2;
	memset(&stAiAedConfig, 0, sizeof(AI_AED_CONFIG_S));
	memset(&stAiAedConfig2, 0, sizeof(AI_AED_CONFIG_S));

	stAiAedConfig.fSnrDB = 10.0f;
	stAiAedConfig.fLsdDB = -35.0f;
	stAiAedConfig.s32Policy = 1;

	result = RK_MPI_AI_SetAedAttr(s32DevId, s32ChnIndex, &stAiAedConfig);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	result = RK_MPI_AI_GetAedAttr(s32DevId, s32ChnIndex, &stAiAedConfig2);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: SetAedAttr(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	result = memcmp(&stAiAedConfig, &stAiAedConfig2, sizeof(AI_AED_CONFIG_S));
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
		return result;
	}

	result = RK_MPI_AI_EnableAed(s32DevId, s32ChnIndex);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: EnableAed(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	return RK_SUCCESS;
}

static RK_S32 init_ai_bcd() {
	int s32DevId = 0;
	int s32ChnIndex = 0;
	int s32DeviceChannel = 2;
	RK_S32 result;

	AI_BCD_CONFIG_S stAiBcdConfig, stAiBcdConfig2;
	memset(&stAiBcdConfig, 0, sizeof(AI_BCD_CONFIG_S));
	memset(&stAiBcdConfig2, 0, sizeof(AI_BCD_CONFIG_S));

	stAiBcdConfig.mFrameLen = 60;
	stAiBcdConfig.mConfirmProb = 0.85f;
        switch (s32DeviceChannel) {
        case 4:
            // just for example: 2mic + 2ref
            stAiBcdConfig.stSedCfg.s64RecChannelType = 0x03;
            break;
        case 6:
            // just for example: 4mic + 2ref
            stAiBcdConfig.stSedCfg.s64RecChannelType = 0x0f;
            break;
        case 8:
            // just for example: 6mic + 2ref
            stAiBcdConfig.stSedCfg.s64RecChannelType = 0x3f;
            break;
        default:
            // by default is 1mic + 1ref, it will be set by internal if is not specified.
            // stAiBcdConfig.stSedCfg.s64RecChannelType = 0x01;
            break;
        }

	// stAiBcdConfig.stSedCfg.s32FrameLen = 90; // by default is 90 if is not specified.
	if (stAiBcdConfig.stSedCfg.s64RecChannelType != 0 ||
		stAiBcdConfig.stSedCfg.s32FrameLen != 0)
		stAiBcdConfig.stSedCfg.bUsed = RK_TRUE;

	char *pBcdModelPath = "/oem/usr/share/vqefiles/rkaudio_model_sed_bcd.rknn";
	memcpy(stAiBcdConfig.aModelPath, pBcdModelPath, strlen(pBcdModelPath));

	result = RK_MPI_AI_SetBcdAttr(s32DevId, s32ChnIndex, &stAiBcdConfig);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	result = RK_MPI_AI_GetBcdAttr(s32DevId, s32ChnIndex, &stAiBcdConfig2);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: SetBcdAttr(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	result = memcmp(&stAiBcdConfig, &stAiBcdConfig2, sizeof(AI_BCD_CONFIG_S));
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: set/get aed config is different: %d", __FUNCTION__, result);
		return result;
	}

	result = RK_MPI_AI_EnableBcd(s32DevId, s32ChnIndex);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: EnableBcd(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	return RK_SUCCESS;
}

// 声音质量增强
RK_S32 init_ai_vqe(RK_S32 s32SampleRate) {
	AI_VQE_CONFIG_S stAiVqeConfig, stAiVqeConfig2;
	RK_S32 result;
	RK_S32 s32VqeGapMs = 16;
	int s32DevId = 0;
	int s32ChnIndex = 0;
	const char *pVqeCfgPath = "/oem/usr/share/vqefiles/config_aivqe.json";

#if defined(RV1106)
	result =
	    RK_MPI_AMIX_SetControl(s32DevId, "I2STDM Digital Loopback Mode", (char *)"Mode2");
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set I2STDM Digital Loopback Mode fail, reason = %x", result);
		return RK_FAILURE;
	}

	result = RK_MPI_AMIX_SetControl(s32DevId, "DAC LINEOUT Volume", (char *)"26");
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set alc left voulme fail, reason = %x", result);
		return RK_FAILURE;
	}
#endif

	// Need to config enCfgMode to VQE attr even the VQE is not enabled
	memset(&stAiVqeConfig, 0, sizeof(AI_VQE_CONFIG_S));
	if (pVqeCfgPath != RK_NULL) {
		stAiVqeConfig.enCfgMode = AIO_VQE_CONFIG_LOAD_FILE;
		memcpy(stAiVqeConfig.aCfgFile, pVqeCfgPath, strlen(pVqeCfgPath));
	}

	if (s32VqeGapMs != 16 && s32VqeGapMs != 10) {
		RK_LOGE("Invalid gap: %d, just supports 16ms or 10ms for AI VQE", s32VqeGapMs);
		return RK_FAILURE;
	}

	stAiVqeConfig.s32WorkSampleRate = s32SampleRate;
	stAiVqeConfig.s32FrameSample = s32SampleRate * s32VqeGapMs / 1000;
	result = RK_MPI_AI_SetVqeAttr(s32DevId, s32ChnIndex, 0, 0, &stAiVqeConfig);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: SetVqeAttr(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	result = RK_MPI_AI_GetVqeAttr(s32DevId, s32ChnIndex, &stAiVqeConfig2);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: SetVqeAttr(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	result = memcmp(&stAiVqeConfig, &stAiVqeConfig2, sizeof(AI_VQE_CONFIG_S));
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: set/get vqe config is different: %d", __FUNCTION__, result);
		return result;
	}

	result = RK_MPI_AI_EnableVqe(s32DevId, s32ChnIndex);
	if (result != RK_SUCCESS) {
		RK_LOGE("%s: EnableVqe(%d,%d) failed with %#x", __FUNCTION__, s32DevId,
		        s32ChnIndex, result);
		return result;
	}

	return RK_SUCCESS;
}

RK_S32 ai_set_other(RK_S32 s32SetVolume) {
	printf("\n=======%s=======\n", __func__);
	int s32DevId = 0;

	RK_MPI_AI_SetVolume(s32DevId, s32SetVolume);

	// 左声道，无需修改
	RK_MPI_AI_SetTrackMode(s32DevId, AUDIO_TRACK_FRONT_LEFT);
	AUDIO_TRACK_MODE_E trackMode;
	RK_MPI_AI_GetTrackMode(s32DevId, &trackMode);
	RK_LOGI("test info : get track mode = %d", trackMode);

	return 0;
}

RK_S32 open_device_ai(RK_S32 InputSampleRate, RK_S32 OutputSampleRate, RK_U32 u32FrameCnt,
                      RK_S32 aiVqe, RK_S32 aiSed) {
	printf("\n=======%s=======\n", __func__);
	AIO_ATTR_S aiAttr;
	AI_CHN_PARAM_S pstParams;
	RK_S32 result;
	int aiDevId = 0;
	int aiChn = 0;
	memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

	RK_BOOL needResample = (InputSampleRate != OutputSampleRate) ? RK_TRUE : RK_FALSE;

#if defined(RV1106)
	sprintf((char *)aiAttr.u8CardName, "%s", "hw:0,0");
#else
	sprintf((char *)aiAttr.u8CardName, "%s", "default");
#endif

	// s32DeviceSampleRate 和 OutputSampleRate,OutputSampleRate
	// 可以使用其他采样率，需要调用重采样函数。默认一样采样率。
	aiAttr.soundCard.channels = 2;
	aiAttr.soundCard.sampleRate = InputSampleRate;
	aiAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)OutputSampleRate;

	aiAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aiAttr.u32PtNumPerFrm = u32FrameCnt;
	// 以下参数无特殊需求，无需变动，保持默认值即可
	aiAttr.u32FrmNum = 4;
	aiAttr.u32EXFlag = 0;
	aiAttr.u32ChnCnt = 2;

	result = RK_MPI_AI_SetPubAttr(aiDevId, &aiAttr);
	if (result != 0) {
		RK_LOGE("ai set attr fail, reason = %d", result);
		goto __FAILED;
	}

	result = RK_MPI_AI_Enable(aiDevId);
	if (result != 0) {
		RK_LOGE("ai enable fail, reason = %d", result);
		goto __FAILED;
	}

	memset(&pstParams, 0, sizeof(AI_CHN_PARAM_S));
	pstParams.enLoopbackMode = AUDIO_LOOPBACK_NONE;
	pstParams.s32UsrFrmDepth = 2;
	result = RK_MPI_AI_SetChnParam(aiDevId, aiChn, &pstParams);
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set channel params, aiChn = %d", aiChn);
		return RK_FAILURE;
	}

	if (aiSed) {
		result = init_ai_aed();
		if (result != 0) {
			RK_LOGE("ai aed init fail, reason = %x, aiChn = %d", result, aiChn);
			return RK_FAILURE;
		}

		result = init_ai_bcd();
		if (result != 0) {
			RK_LOGE("ai bcd init fail, reason = %x, aiChn = %d", result, aiChn);
			return RK_FAILURE;
		}
	}

	if (aiVqe)
		init_ai_vqe(OutputSampleRate);

	result = RK_MPI_AI_EnableChn(aiDevId, aiChn);
	if (result != 0) {
		RK_LOGE("ai enable channel fail, aiChn = %d, reason = %x", aiChn, result);
		return RK_FAILURE;
	}

	if (needResample == RK_TRUE) {
		RK_LOGI("need to resample %d -> %d", InputSampleRate, OutputSampleRate);
		result =
		    RK_MPI_AI_EnableReSmp(aiDevId, aiChn, (AUDIO_SAMPLE_RATE_E)OutputSampleRate);
		if (result != 0) {
			RK_LOGE("ai enable channel fail, reason = %x, aiChn = %d", result, aiChn);
			return RK_FAILURE;
		}
	}

	ai_set_other(100);

	return RK_SUCCESS;
__FAILED:
	return RK_FAILURE;
}

RK_S32 init_mpi_aenc(RK_S32 s32SampleRate) {
	printf("\n=======%s=======\n", __func__);
	RK_S32 s32ret = 0;
	AENC_CHN_ATTR_S pstChnAttr;
	RK_CODEC_ID_E enCodecId =
	    (RK_CODEC_ID_E)code_type; // RK_AUDIO_ID_ADPCM_G726 RK_AUDIO_ID_PCM_ALAW
	AUDIO_BIT_WIDTH_E enBitwidth = AUDIO_BIT_WIDTH_16;

	memset(&pstChnAttr, 0, sizeof(AENC_CHN_ATTR_S));

	printf("codecId=%d\n", (int)enCodecId);

	pstChnAttr.stCodecAttr.enType = enCodecId;
	pstChnAttr.stCodecAttr.u32Channels = 1; // default MONO
	pstChnAttr.stCodecAttr.u32SampleRate = s32SampleRate;
	pstChnAttr.stCodecAttr.enBitwidth = enBitwidth;
	pstChnAttr.stCodecAttr.pstResv = RK_NULL;

	pstChnAttr.enType = enCodecId;
	pstChnAttr.u32BufCount = 4;

	s32ret = RK_MPI_AENC_CreateChn(0, &pstChnAttr);
	if (s32ret) {
		RK_LOGE("create aenc chn %d err:0x%x\n", enCodecId, s32ret);
	}
	return s32ret;
}

RK_S32 ao_set_other(RK_S32 s32SetVolume) {
	printf("\n=======%s=======\n", __func__);
	int s32DevId = 0;
	RK_S32 volume = 0;

	RK_MPI_AO_SetVolume(s32DevId, s32SetVolume);
	RK_MPI_AO_GetVolume(s32DevId, &volume);
	RK_LOGI("test info : get volume = %d", volume);

	RK_MPI_AO_SetTrackMode(s32DevId, AUDIO_TRACK_OUT_STEREO);

	AUDIO_TRACK_MODE_E trackMode;
	RK_MPI_AO_GetTrackMode(s32DevId, &trackMode);
	RK_LOGI("test info : get track mode = %d", trackMode);

	return 0;
}

RK_S32 open_device_ao(RK_S32 s32SampleRate, RK_U32 u32FrameCnt) {
	printf("\n=======%s=======\n", __func__);
	RK_S32 result = 0;
	AUDIO_DEV aoDevId = 0;
	AO_CHN aoChn = 0;
	AIO_ATTR_S aoAttr;
	AO_CHN_PARAM_S pstParams;

	memset(&pstParams, 0, sizeof(AO_CHN_PARAM_S));
	memset(&aoAttr, 0, sizeof(AIO_ATTR_S));
	/*==============================================================================*/
	sprintf((char *)aoAttr.u8CardName, "%s", "hw:0,0");

	aoAttr.soundCard.channels = 2;
	aoAttr.soundCard.sampleRate = s32SampleRate;
	aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

	aoAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	aoAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)s32SampleRate;

	aoAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aoAttr.u32PtNumPerFrm = u32FrameCnt; // 1024
	// 以下参数没有特殊需要，无需修改
	aoAttr.u32FrmNum = 4;
	aoAttr.u32EXFlag = 0;
	aoAttr.u32ChnCnt = 2;

	RK_MPI_AO_SetPubAttr(aoDevId, &aoAttr);
	RK_MPI_AO_Enable(aoDevId);
	/*==============================================================================*/
	pstParams.enLoopbackMode = AUDIO_LOOPBACK_NONE;
	result = RK_MPI_AO_SetChnParams(aoDevId, aoChn, &pstParams);
	if (result != RK_SUCCESS) {
		RK_LOGE("ao set channel params, aoChn = %d", aoChn);
		return RK_FAILURE;
	}
	/*==============================================================================*/
	result = RK_MPI_AO_EnableChn(aoDevId, aoChn);
	if (result != 0) {
		RK_LOGE("ao enable channel fail, aoChn = %d, reason = %x", aoChn, result);
		return RK_FAILURE;
	}
	/*==============================================================================*/
	// set sample rate of input data
	result = RK_MPI_AO_EnableReSmp(aoDevId, aoChn, (AUDIO_SAMPLE_RATE_E)s32SampleRate);
	if (result != 0) {
		RK_LOGE("ao enable channel fail, reason = %x, aoChn = %d", result, aoChn);
		return RK_FAILURE;
	}

	ao_set_other(100);

	return RK_SUCCESS;
}

RK_S32 init_mpi_adec(RK_S32 s32SampleRate) {
	printf("\n=======%s=======\n", __func__);
	RK_S32 s32ret = 0;
	ADEC_CHN AdChn = 0;
	ADEC_CHN_ATTR_S pstChnAttr;
	memset(&pstChnAttr, 0, sizeof(ADEC_CHN_ATTR_S));

	pstChnAttr.stCodecAttr.enType = (RK_CODEC_ID_E)code_type;
	pstChnAttr.stCodecAttr.u32Channels = 1; // default 1
	pstChnAttr.stCodecAttr.u32SampleRate = s32SampleRate;
	pstChnAttr.stCodecAttr.u32BitPerCodedSample = 4;

	pstChnAttr.enType = (RK_CODEC_ID_E)code_type;
	pstChnAttr.enMode = ADEC_MODE_STREAM; // ADEC_MODE_PACK
	pstChnAttr.u32BufCount = 4;
	pstChnAttr.u32BufSize = 50 * 1024;
	s32ret = RK_MPI_ADEC_CreateChn(AdChn, &pstChnAttr);
	if (s32ret) {
		RK_LOGE("create adec chn %d err:0x%x\n", AdChn, s32ret);
		s32ret = RK_FAILURE;
	}
	return s32ret;
}

/*
static RK_S32 adec_data_free(void *opaque) {
	if (opaque) {
		free(opaque);
		opaque = RK_NULL;
	}
	return 0;
}
*/

void *getDataThread(void *ptr) {
	TEST_AI_CTX_S *params = (TEST_AI_CTX_S *)(ptr);
	RK_S32 result = 0, count = 2;
	RK_S32 s32MilliSec = -1;
	AUDIO_FRAME_S getFrame;

	while (!quit && count > 0) {
		if (params->s32LoopCount > 1)
			count--;

#if TEST_AI_WITH_FD
		test_ai_poll_event(-1, params->s32DevFd);
#endif
		result = RK_MPI_AI_GetFrame(params->s32DevId, params->s32ChnIndex, &getFrame,
		                            RK_NULL, s32MilliSec);
		if (result == 0) {
			void *data = RK_MPI_MB_Handle2VirAddr(getFrame.pMbBlk);
			RK_LOGV("data = %p, len = %d", data, getFrame.u32Len);
			RK_MPI_AI_ReleaseFrame(params->s32DevId, params->s32ChnIndex, &getFrame,
			                       RK_NULL);
		}

		// dump results of SED(AED/BCD) modules
		if (params->s32AedEnable) {
			AI_AED_RESULT_S aed_result;

			memset(&aed_result, 0, sizeof(aed_result));
			result = RK_MPI_AI_GetAedResult(params->s32DevId, params->s32ChnIndex,
			                                &aed_result);
			if (result == 0) {
				if (aed_result.bAcousticEventDetected)
					RK_LOGI("AED Result: AcousticEvent:%d",
					        aed_result.bAcousticEventDetected);
				if (aed_result.bLoudSoundDetected) {
					params->s32AedLoudCount++;
					RK_LOGI("AED Result: LoudSound:%d", aed_result.bLoudSoundDetected);
				}

				if (aed_result.bLoudSoundDetected)
					RK_LOGI("AED Result: LoudSound Volume Result:%f db",
					        aed_result.lsdResult);
			}
		}

		if (params->s32BcdEnable) {
			AI_BCD_RESULT_S bcd_result;

			memset(&bcd_result, 0, sizeof(bcd_result));
			result = RK_MPI_AI_GetBcdResult(params->s32DevId, params->s32ChnIndex,
			                                &bcd_result);
			if (result == 0 && bcd_result.bBabyCry) {
				params->s32BcdCount++;
				RK_LOGI("BCD Result: BabyCry:%d", bcd_result.bBabyCry);
			}
		}

		if (params->s32BuzEnable) {
			AI_BUZ_RESULT_S buz_result;

			memset(&buz_result, 0, sizeof(buz_result));
			result = RK_MPI_AI_GetBuzResult(params->s32DevId, params->s32ChnIndex,
			                                &buz_result);
			if (result == 0 && buz_result.bBuzz) {
				params->s32BuzCount++;
				RK_LOGI("BUZ Result: Buzz:%d", buz_result.bBuzz);
			}
		}

		if (params->s32GbsEnable) {
			AI_GBS_RESULT_S gbs_result;

			memset(&gbs_result, 0, sizeof(gbs_result));
			result = RK_MPI_AI_GetGbsResult(params->s32DevId, params->s32ChnIndex,
			                                &gbs_result);
			if (result == 0 && gbs_result.bGbs) {
				params->s32GbsCount++;
				RK_LOGI("GBS Result: Gbs:%d", gbs_result.bGbs);
			}
		}
	}

	pthread_exit(NULL);
	return RK_NULL;
}

static RK_CHAR optstr[] = "?::r:t:v:s:l:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s [-r 8000] [-t g726] -o /tmp/aenc.g726\n", name);
	printf("\t-r: sample rate, Default:16000\n");
	printf("\t-t: --encode: encode type, Default:g726, Value:g711a, g711u, g726\n");
	printf("\t-o: output path, Default:\"/tmp/aenc.g726\"\n");
}

// 编码g726的时候，只支持8000 单声道，所以需要设置所以需要设置track_mode为8
int main(int argc, char *argv[]) {
	RK_S32 s32SampleRate = 8000;
	RK_S32 s32AiVqe = 0;
	RK_S32 s32AiSed = 0;
	RK_S32 loopCount = 1;
	RK_U32 u32FrameCnt = 1024;
	RK_CHAR *pCodecName = "g726";
	int ret = 0;
	int c;
	TEST_AI_CTX_S params;
	pthread_t tidGet;

	params.srcFilePath = RK_NULL;
	params.dstFilePath = RK_NULL;
	params.s32LoopCount = 1;
	params.s32ChnNum = 1;
	params.s32BitWidth = 16;
	params.s32FrameNumber = 4;
	params.s32FrameLength = 1024;
	params.chCardName = RK_NULL;
	params.s32DevId = 0;
	params.s32SetVolumeCurve = 0;
	params.s32SetVolume = 100;
	params.s32SetMute = 0;
	params.s32SetTrackMode = 0;
	params.s32SetFadeRate = 0;
	params.s32GetVolume = 0;
	params.s32GetMute = 0;
	params.s32GetTrackMode = 0;
	params.s32DataReadEnable = 0;
	params.s32AedEnable = 0;
	params.s32BcdEnable = 0;
	params.s32BuzEnable = 0;
	params.s32GbsEnable = 0;
	params.s32VqeGapMs = 16;
	params.s32VqeEnable = 0;
	params.pVqeCfgPath = RK_NULL;
	params.s32LoopbackMode = AUDIO_LOOPBACK_NONE;
	params.s32DumpAlgo = 0;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'r':
			s32SampleRate = atoi(optarg);
			break;
		case 't':
			if (!strcmp(optarg, "g711a")) {
				code_type = RK_AUDIO_ID_PCM_ALAW;
				pCodecName = "g711a";
			} else if (!strcmp(optarg, "g711u")) {
				code_type = RK_AUDIO_ID_PCM_MULAW;
				pCodecName = "g711u";
			} else if (!strcmp(optarg, "g726")) {
				code_type = RK_AUDIO_ID_ADPCM_G726;
				pCodecName = "g726";
			} else {
				printf("ERROR: Invalid encoder type.\n");
				return 0;
			}
			break;
		case 'v':
			s32AiVqe = atoi(optarg);
			break;
		case 's':
			s32AiSed = atoi(optarg);
			break;
		case 'l':
			loopCount = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#SampleRate: %d\n", s32SampleRate);
	printf("#Frame Count: %d\n", u32FrameCnt);
	printf("#CodecName: %s\n", pCodecName);
	printf("#AI VQE enable: %d\n", s32AiVqe);
	printf("#AI SED enable: %d\n", s32AiSed);
	printf("#loop count: %d\n", loopCount);

	params.s32DevId = 0;
	params.s32ChnIndex = 0;
	params.s32SampleRate = s32SampleRate;
	params.s32LoopCount = loopCount;

	if (s32AiVqe)
		params.s32VqeEnable = 1;

	if (s32AiSed) {
		params.s32AedEnable = 1;
		params.s32BcdEnable = 1;
	}

	signal(SIGINT, sigterm_handler);
	SAMPLE_COMM_CheckFd(RK_TRUE);
	RK_MPI_SYS_Init();

	for (int i = 0; i < loopCount; i++) {
		params.s32AedLoudCount = 0;
		params.s32BcdCount = 0;
		params.s32BuzCount = 0;
		params.s32GbsCount = 0;

		init_mpi_aenc(s32SampleRate);
		init_mpi_adec(s32SampleRate);
		open_device_ai(s32SampleRate, s32SampleRate, u32FrameCnt, s32AiVqe, s32AiSed);

		// ai bind aenc
		MPP_CHN_S stChnAI, stChnAENC, stChnADEC, stChnAO;
		stChnAI.enModId = RK_ID_AI;
		stChnAI.s32DevId = 0;
		stChnAI.s32ChnId = 0;

		stChnAENC.enModId = RK_ID_AENC;
		stChnAENC.s32DevId = 0;
		stChnAENC.s32ChnId = 0;

		// bind AI-AENC
		ret = RK_MPI_SYS_Bind(&stChnAI, &stChnAENC);
		if (ret) {
			printf("Bind AI[0] to AENC[0] failed! ret=%d\n", ret);
			return -1;
		}

		// aenc bind adec
		stChnAENC.enModId = RK_ID_AENC;
		stChnAENC.s32DevId = 0;
		stChnAENC.s32ChnId = 0;

		stChnADEC.enModId = RK_ID_ADEC;
		stChnADEC.s32DevId = 0;
		stChnADEC.s32ChnId = 0;

		// bind AENC-ADEC
		ret = RK_MPI_SYS_Bind(&stChnAENC, &stChnADEC);
		if (ret) {
			printf("Bind AENC[0] to ADEC[0] failed! ret=%d\n", ret);
			return -1;
		}

		open_device_ao(s32SampleRate, u32FrameCnt);
		// adec bind ao
		stChnADEC.enModId = RK_ID_ADEC;
		stChnADEC.s32DevId = 0;
		stChnADEC.s32ChnId = 0;

		stChnAO.enModId = RK_ID_AO;
		stChnAO.s32DevId = 0;
		stChnAO.s32ChnId = 0;

		// bind ADEC-AO
		ret = RK_MPI_SYS_Bind(&stChnADEC, &stChnAO);
		if (ret) {
			printf("Bind ADEC[0] to AO[0] failed! ret=%d\n", ret);
			return -1;
		}

		pthread_create(&tidGet, RK_NULL, getDataThread, (void *)(&params));
		printf("%s initial finish\n", __func__);

		while (!quit) {
			if (loopCount > 1) {
				usleep(1000);
				break;
			}

			usleep(1000);
		}

		if (tidGet) {
			pthread_join(tidGet, RK_NULL);
			tidGet = 0;
		}

		if (params.s32AedEnable)
			RK_LOGD("Aed Loud event count: %d", params.s32AedLoudCount);

		if (params.s32BcdEnable)
			RK_LOGD("Bcd event count: %d", params.s32BcdCount);

		if (params.s32BuzEnable)
			RK_LOGD("Buz event count: %d", params.s32BuzCount);

		if (params.s32GbsEnable)
			RK_LOGD("Gbs event count: %d", params.s32GbsCount);

		RK_MPI_SYS_UnBind(&stChnADEC, &stChnAO);
		RK_MPI_SYS_UnBind(&stChnAENC, &stChnADEC);
		RK_MPI_SYS_UnBind(&stChnAI, &stChnAENC);

		if (s32AiVqe) {
			RK_MPI_AI_DisableVqe(0, 0);
#if defined(RV1106)
			ret = RK_MPI_AMIX_SetControl(params.s32DevId, "I2STDM Digital Loopback Mode",
			                             (char *)"Disabled");
			if (ret != RK_SUCCESS) {
				RK_LOGE("ai set I2STDM Digital Loopback Mode fail, reason = %x", ret);
				return RK_FAILURE;
			}
#endif
		}

		if (s32AiSed) {
			ret = RK_MPI_AI_DisableBcd(0, 0);
			if (ret != RK_SUCCESS) {
				RK_LOGE("%s: RK_MPI_AI_DisableBcd failed with %#x", __FUNCTION__, ret);
				return ret;
			}

			ret = RK_MPI_AI_DisableAed(0, 0);
			if (ret != RK_SUCCESS) {
				RK_LOGE("%s: RK_MPI_AI_DisableAed failed with %#x", __FUNCTION__, ret);
				return ret;
			}
		}

		RK_MPI_AO_DisableReSmp(0, 0);
		RK_MPI_AO_DisableChn(0, 0);
		RK_MPI_AO_Disable(0);
		RK_MPI_ADEC_DestroyChn(0);
		RK_MPI_AI_DisableChn(0, 0);
		RK_MPI_AI_Disable(0);
		RK_MPI_AENC_DestroyChn(0);
	}

	RK_MPI_SYS_Exit();
	SAMPLE_COMM_CheckFd(RK_FALSE);
	return 0;
}
