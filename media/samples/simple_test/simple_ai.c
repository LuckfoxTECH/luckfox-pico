#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>

#include "sample_comm.h"

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
		if (result == 0) {
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

RK_S32 ai_set_other(RK_S32 s32SetVolume) {
	printf("\n=======%s=======\n", __func__);
	int s32DevId = 0;

	RK_MPI_AI_SetVolume(s32DevId, s32SetVolume);

	//左声道，无需修改
	RK_MPI_AI_SetTrackMode(s32DevId, AUDIO_TRACK_FRONT_LEFT);
	AUDIO_TRACK_MODE_E trackMode;
	RK_MPI_AI_GetTrackMode(s32DevId, &trackMode);
	RK_LOGI("test info : get track mode = %d", trackMode);

	return 0;
}

RK_S32 open_device_ai(RK_S32 InputSampleRate, RK_S32 OutputSampleRate,
                      RK_S32 u32FrameCnt) {
	printf("\n=======%s=======\n", __func__);
	AIO_ATTR_S aiAttr;
	AI_CHN_PARAM_S pstParams;
	RK_S32 result;
	int aiDevId = 0;
	int aiChn = 0;
	memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

	RK_BOOL needResample = (InputSampleRate != OutputSampleRate) ? RK_TRUE : RK_FALSE;

	// snprintf(reinterpret_cast<char *>(aiAttr.u8CardName),
	// sizeof(aiAttr.u8CardName), "%s", "hw:0,0");
	sprintf((char *)aiAttr.u8CardName, "%s", "hw:0,0");

	// s32DeviceSampleRate和s32SampleRate,s32SampleRate可以使用其他采样率，需要调用重采样函数。默认一样采样率。
	aiAttr.soundCard.channels = 2;
	aiAttr.soundCard.sampleRate = InputSampleRate;
	aiAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)OutputSampleRate;
	aiAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aiAttr.u32PtNumPerFrm = u32FrameCnt;
	//以下参数无特殊需求，无需变动，保持默认值即可
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
	pstParams.s32UsrFrmDepth = 1;
	result = RK_MPI_AI_SetChnParam(aiDevId, aiChn, &pstParams);
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set channel params, aiChn = %d", aiChn);
		return RK_FAILURE;
	}

	//使用声音增强功能，默认关闭
	// test_init_ai_vqe(OutputSampleRate);

	result = RK_MPI_AI_EnableChn(aiDevId, aiChn);
	if (result != 0) {
		RK_LOGE("ai enable channel fail, aiChn = %d, reason = %x", aiChn, result);
		return RK_FAILURE;
	}

	//重采样功能
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

static RK_CHAR optstr[] = "?::d:r:o:m:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s [-r 8000] -o /tmp/ai.pcm\n", name);
	printf("\t-r: sample rate, Default:16000\n");
	printf("\t-o: output path, Default:\"/tmp/ai.pcm\"\n");
}

int main(int argc, char *argv[]) {
	RK_S32 u32SampleRate = 16000;
	RK_U32 u32FrameCnt = 1024;
	RK_CHAR *pOutPath = "/tmp/ai.pcm";
	int c;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'r':
			u32SampleRate = atoi(optarg);
			break;
		case 'o':
			pOutPath = optarg;
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#SampleRate: %d\n", u32SampleRate);
	printf("#Frame Count: %d\n", u32FrameCnt);
	printf("#Output Path: %s\n", pOutPath);

	if (pOutPath) {
		save_file = fopen(pOutPath, "w");
		if (!save_file) {
			printf("ERROR: open file: %s fail, exit\n", pOutPath);
			return 0;
		}
	}

	signal(SIGINT, sigterm_handler);

	RK_MPI_SYS_Init();

	open_device_ai(u32SampleRate, u32SampleRate, u32FrameCnt);

	pthread_t read_thread;
	pthread_create(&read_thread, NULL, GetMediaBuffer, NULL);

	printf("%s initial finish\n", __func__);

	while (!quit) {
		usleep(500000);
	}
	pthread_join(&read_thread, NULL);
	// RK_MPI_AI_DisableVqe(0, 0);
	RK_MPI_AI_DisableChn(0, 0);
	RK_MPI_AI_Disable(0);
	RK_MPI_SYS_Exit();
	return 0;
}
