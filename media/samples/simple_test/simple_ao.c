// AO

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>

#include "sample_comm.h"

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static FILE *ao_file;

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

RK_S32 open_device_ao(RK_S32 s32ReSmpSampleRate, RK_S32 s32SampleRate,
                      RK_S32 u32FrameCnt) {
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

	aoAttr.soundCard.channels = 2;               // 2
	aoAttr.soundCard.sampleRate = s32SampleRate; // s32SampleRate;       16000
	aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

	aoAttr.enBitwidth = AUDIO_BIT_WIDTH_16;                   // AUDIO_BIT_WIDTH_16
	aoAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)s32SampleRate; // 16000

	aoAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aoAttr.u32PtNumPerFrm = u32FrameCnt; // 1024
	//以下参数没有特殊需要，无需修改
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
	if (s32ReSmpSampleRate != s32SampleRate) {
		printf("********RK_MPI_AO_EnableReSmp*********\n");
		result = RK_MPI_AO_EnableReSmp(aoDevId, aoChn,
		                               (AUDIO_SAMPLE_RATE_E)s32ReSmpSampleRate);
		if (result != 0) {
			RK_LOGE("ao enable channel fail, reason = %x, aoChn = %d", result, aoChn);
			return RK_FAILURE;
		}
	}

	ao_set_other(100);

	return RK_SUCCESS;
}

static RK_CHAR optstr[] = "?::d:r:i:t:s:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -r 16000 -s 16000 -i /tmp/16000.pcm\n",
	       name); //正常情况-r -s 配置成一样的就好
	printf("\t-r: Resample rate, Default:16000\n");
	printf("\t-s: sample rate, Default:16000\n");
	printf("\t-i: input path, Default:\"/tmp/16000.pcm\"\n");
}

int main(int argc, char *argv[]) {
	RK_S32 u32SampleRate = 16000;      // device samplerate
	RK_S32 s32ReSmpSampleRate = 16000; // input pcm samplerate
	RK_U32 u32FrameCnt = 1024;
	RK_CHAR *pInPath = "/tmp/16000.pcm";
	int c;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'r':
			s32ReSmpSampleRate = atoi(optarg);
			break;
		case 's':
			u32SampleRate = atoi(optarg);
			break;
		case 'i':
			pInPath = optarg;
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#SampleRate: %d\n", u32SampleRate);
	printf("#s32ReSmpSampleRate: %d\n", s32ReSmpSampleRate);
	printf("#Frame Count: %d\n", u32FrameCnt);
	printf("#Input Path: %s\n", pInPath);

	if (pInPath) {
		ao_file = fopen(pInPath, "rb");
		if (!ao_file) {
			printf("ERROR: open file: %s fail, exit\n", pInPath);
			return 0;
		}
	}

	signal(SIGINT, sigterm_handler);

	RK_MPI_SYS_Init();
	open_device_ao(s32ReSmpSampleRate, u32SampleRate, u32FrameCnt);

	printf("%s initial finish\n", __func__);

	RK_U8 *srcData = RK_NULL;
	RK_S32 srcSize = 0;
	RK_U64 timeStamp = 0;
	AUDIO_FRAME_S frame;
	RK_S32 result = 0;
	RK_S32 s32MilliSec = -1;
	while (!quit) {
		srcData = calloc(1024, sizeof(RK_U8));
		memset(srcData, 0, 1024);

		srcSize = fread(srcData, 1, 1024, ao_file);
		frame.u32Len = srcSize;
		frame.u64TimeStamp = timeStamp++;
		frame.enBitWidth = AUDIO_BIT_WIDTH_16;
		frame.enSoundMode = AUDIO_SOUND_MODE_MONO;
		frame.bBypassMbBlk = RK_FALSE;

		MB_EXT_CONFIG_S extConfig;
		memset(&extConfig, 0, sizeof(extConfig));
		extConfig.pOpaque = srcData;
		extConfig.pu8VirAddr = srcData;
		extConfig.u64Size = srcSize;
		RK_MPI_SYS_CreateMB(&(frame.pMbBlk), &extConfig);
	__RETRY:
		result = RK_MPI_AO_SendFrame(0, 0, &frame, s32MilliSec);
		if (result < 0) {
			RK_LOGE("send frame fail, result = %d, TimeStamp = %lld, s32MilliSec = %d",
			        result, frame.u64TimeStamp, s32MilliSec);
			goto __RETRY;
		}
		RK_MPI_MB_ReleaseMB(frame.pMbBlk);

		if (srcSize <= 0) {
			printf("eof");
			RK_MPI_AO_WaitEos(0, 0, s32MilliSec);
			break;
		}
	}

	if (ao_file) {
		fclose(ao_file);
		ao_file = RK_NULL;
	}
	free(srcData);

	RK_MPI_AO_DisableReSmp(0, 0);
	RK_MPI_AO_DisableChn(0, 0);
	RK_MPI_AO_Disable(0);
	RK_MPI_SYS_Exit();
	return 0;
}
