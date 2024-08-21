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
#include "rk_mpi_sys.h"
#include "rk_mpi_adec.h"
#include "rk_mpi_aenc.h"
#include "rk_mpi_ai.h"
#include "rk_mpi_amix.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static FILE *save_file;

static void *GetMediaBuffer(void *arg) {
	RK_S32 s32MilliSec = -1;
	AUDIO_FRAME_S frame;
	RK_S32 result = 0;

#if 0
	//Other ways to save pcm
	AUDIO_SAVE_FILE_INFO_S save;
	save.bCfg = RK_TRUE;
	save.u32FileSize = 1024;
	snprintf(save.aFilePath, sizeof(save.aFilePath), "%s", "/userdata");
	snprintf(save.aFileName, sizeof(save.aFileName), "%s", "ai.pcm");
	RK_MPI_AI_SaveFile(0, 0, &save);
#endif
	while (!quit) {
		result = RK_MPI_AI_GetFrame(0, 0, &frame, RK_NULL, s32MilliSec);
		if (result == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(frame.pMbBlk);
			RK_U32 len = frame.u32Len;
			RK_LOGI("data = %p, len = %d", data, len);
			if (save_file) {
				fwrite(data, len, 1, save_file);
				fflush(save_file);
			}
			RK_MPI_AI_ReleaseFrame(0, 0, &frame, RK_NULL);
		}
	}

	if (save_file)
		fclose(save_file);

	return NULL;
}

//声音质量增强
RK_S32 test_init_ai_vqe(RK_S32 s32SampleRate) {
	AI_VQE_CONFIG_S stAiVqeConfig, stAiVqeConfig2;
	RK_S32 result;
	RK_S32 s32VqeGapMs = 16;
	int s32DevId = 0;
	int s32ChnIndex = 0;
	const char *pVqeCfgPath = "/oem/usr/share/vqefiles/config_aivqe.json";

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

RK_S32 ai_set_other(RK_S32 s32SetVolume) {
	printf("\n=======%s=======\n", __func__);
	int s32DevId = 0;

	RK_MPI_AI_SetVolume(s32DevId, s32SetVolume);

	//双声道，左声道为MIC拾⾳数据，右声道为播放的右声道的回采数据
	RK_MPI_AI_SetTrackMode(s32DevId, AUDIO_TRACK_NORMAL);
	AUDIO_TRACK_MODE_E trackMode;
	RK_MPI_AI_GetTrackMode(s32DevId, &trackMode);
	RK_LOGI("test info : get track mode = %d", trackMode);

	return 0;
}

RK_S32 open_device_ai(RK_S32 deviceSampleRate, RK_S32 outputSampleRate, RK_S32 u32FrameCnt,
                      RK_S32 vqeEnable, RK_S32 bcdEnable) {
	printf("\n=======%s=======\n", __func__);
	AIO_ATTR_S aiAttr;
	AI_CHN_PARAM_S pstParams;
	RK_S32 result;
	int aiDevId = 0;
	int aiChn = 0;
	memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

	RK_BOOL needResample = (deviceSampleRate != outputSampleRate) ? RK_TRUE : RK_FALSE;
#ifdef RV1126_RV1109
	//这是RV1126 声卡打开设置，RV1106设置无效，可以不设置
	result = RK_MPI_AMIX_SetControl(aiDevId, "Capture MIC Path", (char *)"Main Mic");
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set Capture MIC Path fail, reason = %x", result);
		goto __FAILED;
	}
#endif

	sprintf((char *)aiAttr.u8CardName, "%s", "hw:0,0");

	// s32DeviceSampleRate和s32SampleRate,s32SampleRate可以使用其他采样率，需要调用重采样函数。默认一样采样率。
	aiAttr.soundCard.channels = 2;
	aiAttr.soundCard.sampleRate = deviceSampleRate;
	aiAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)outputSampleRate;
	aiAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aiAttr.u32PtNumPerFrm = u32FrameCnt;
	//以下参数无特殊需求，无需变动，保持默认值即可
	aiAttr.u32FrmNum = 4;
	aiAttr.u32EXFlag = 0;
	aiAttr.u32ChnCnt = 2;

	result = RK_MPI_AI_SetPubAttr(aiDevId, &aiAttr);
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set attr fail, reason = %x", result);
		goto __FAILED;
	}

	//这是RV1106 回采设置，适用于左mic，右回采
	// RV1126设置无效，可以不设置，RV1126需要配置asound.conf文件或者内核驱动配置软件回采
	result =
	    RK_MPI_AMIX_SetControl(aiDevId, "I2STDM Digital Loopback Mode", (char *)"Mode2");
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set I2STDM Digital Loopback Mode fail, reason = %x", result);
		goto __FAILED;
	}

	//这是RV1106 ALC设置，而RV1126设置无效，可以不设置
	result = RK_MPI_AMIX_SetControl(aiDevId, "ADC ALC Left Volume", (char *)"22");
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set alc left voulme fail, reason = %x", result);
		goto __FAILED;
	}

	result = RK_MPI_AMIX_SetControl(aiDevId, "ADC ALC Right Volume", (char *)"22");
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set alc right voulme fail, reason = %x", result);
		goto __FAILED;
	}

	result = RK_MPI_AI_Enable(aiDevId);
	if (result != RK_SUCCESS) {
		RK_LOGE("ai enable fail, reason = %x", result);
		goto __FAILED;
	}

	memset(&pstParams, 0, sizeof(AI_CHN_PARAM_S));
	pstParams.s32UsrFrmDepth = 4; //0, get fail, 1 - u32ChnCnt, can get, if bind to other device, must be < u32ChnCnt
	result = RK_MPI_AI_SetChnParam(aiDevId, aiChn, &pstParams);
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set channel params, aiChn = %d", aiChn);
		return RK_FAILURE;
	}

	//使用声音增强功能，默认开启
	if (vqeEnable)
		test_init_ai_vqe(deviceSampleRate);

	if (bcdEnable) {
		result = init_ai_bcd();
		if (result != 0) {
			RK_LOGE("ai bcd init fail, reason = %x, aiChn = %d", result, aiChn);
			return RK_FAILURE;
		}
	}

	result = RK_MPI_AI_EnableChn(aiDevId, aiChn);
	if (result != 0) {
		RK_LOGE("ai enable channel fail, aiChn = %d, reason = %x", aiChn, result);
		return RK_FAILURE;
	}

	//重采样功能
	if (needResample == RK_TRUE) {
		RK_LOGI("need to resample %d -> %d", deviceSampleRate, outputSampleRate);
		result =
		    RK_MPI_AI_EnableReSmp(aiDevId, aiChn, (AUDIO_SAMPLE_RATE_E)outputSampleRate);
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

static RK_CHAR optstr[] = "?::r:R:t:o:v:b:l:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s [-r 16000] -o /tmp/ai.pcm\n", name);
	printf("\t-r: device sample rate, Default:16000\n");
	printf("\t-R: output sample rate, Default:16000\n");
	printf("\t-o: output path, Default:\"/tmp/ai.pcm\"\n");
	printf("\t-v: vqe enable, Default:1\n");
}

int main(int argc, char *argv[]) {
	RK_S32 s32DeviceSampleRate = 16000;
	RK_S32 u32OutPutSampleRate = 16000;
	RK_S32 ret = 0;
	RK_S32 vqeEnable = 1;
	RK_S32 bcdEnable = 1;
	RK_U32 u32FrameCnt = 1024;
	RK_CHAR *pOutPath = (RK_CHAR *)"/tmp/ai.pcm";
	int c;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'r':
			s32DeviceSampleRate = atoi(optarg);
			break;
		case 'R':
			u32OutPutSampleRate = atoi(optarg);
			break;
		case 'o':
			pOutPath = optarg;
			break;
		case 'v':
			vqeEnable = atoi(optarg);
			break;
		case 'b':
			bcdEnable = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	printf("#Device SampleRate: %d\n", s32DeviceSampleRate);
	printf("#Output SampleRate: %d\n", u32OutPutSampleRate);
	printf("#Frame Count: %d\n", u32FrameCnt);
	printf("#Output Path: %s\n", pOutPath);
	printf("#Vqe enable: %d\n", vqeEnable);
	printf("#Bcd enable: %d\n", bcdEnable);

	if (pOutPath) {
		save_file = fopen(pOutPath, "w");
		if (!save_file) {
			printf("ERROR: open file: %s fail, exit\n", pOutPath);
			return -1;
		}
	}

	signal(SIGINT, sigterm_handler);

	RK_MPI_SYS_Init();

	open_device_ai(s32DeviceSampleRate, u32OutPutSampleRate, u32FrameCnt, vqeEnable, bcdEnable);

	pthread_t read_thread;
	pthread_create(&read_thread, NULL, GetMediaBuffer, NULL);

	printf("%s initial finish\n", __func__);

	while (!quit) {
		usleep(500000);
	}

	pthread_join(read_thread, NULL);

	if (bcdEnable)
		RK_MPI_AI_DisableBcd(0, 0);

	if (vqeEnable) {
		RK_MPI_AI_DisableVqe(0, 0);

		//这是RV1106 回采设置关闭，而RV1126设置无效，可以不配置
		ret =
		    RK_MPI_AMIX_SetControl(0, "I2STDM Digital Loopback Mode", (char *)"Disabled");
		if (ret != RK_SUCCESS) {
			RK_LOGE("ai set I2STDM Digital Loopback Mode fail, reason = %x", ret);
			return -1;
		}
	}

	RK_MPI_AI_DisableChn(0, 0);
	RK_MPI_AI_Disable(0);
	RK_MPI_SYS_Exit();
	return 0;
}
