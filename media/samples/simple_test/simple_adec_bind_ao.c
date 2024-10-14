// adec->ao
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
#include "rk_mpi_amix.h"
#include "rk_mpi_ao.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_U32 code_type = RK_AUDIO_ID_ADPCM_G726;

static FILE *adec_file;

RK_S32 ao_set_other(RK_S32 s32SetVolume) {
	printf("\n=======%s=======\n", __func__);
	int s32DevId = 0;
	RK_S32 volume = 0;

	RK_MPI_AO_SetVolume(s32DevId, s32SetVolume);
	RK_MPI_AO_GetVolume(s32DevId, &volume);
	RK_LOGI("test info : get volume = %d", volume);

	AUDIO_TRACK_MODE_E trackMode;
	RK_MPI_AO_GetTrackMode(s32DevId, &trackMode);
	RK_LOGI("test info : get track mode = %d", trackMode);

	return 0;
}

RK_S32 open_device_ao(RK_S32 s32SampleRate, RK_S32 channel, RK_S32 u32FrameCnt) {
	printf("\n=======%s=======\n", __func__);
	RK_S32 result = 0;
	AUDIO_DEV aoDevId = 0;
	AO_CHN aoChn = 0;
	AIO_ATTR_S aoAttr;
	AO_CHN_PARAM_S pstParams;

	memset(&pstParams, 0, sizeof(AO_CHN_PARAM_S));
	memset(&aoAttr, 0, sizeof(AIO_ATTR_S));
#ifdef RV1126_RV1109
	/*==============================================================================*/
	//这是RV1126 声卡打开设置，RV1106设置无效，可以不设置
	result = RK_MPI_AMIX_SetControl(aoDevId, "Playback Path", (char *)"SPK");
	if (result != RK_SUCCESS) {
		RK_LOGE("ao set Playback Path fail, reason = %x", result);
		return RK_FAILURE;
	}
#endif
	sprintf((char *)aoAttr.u8CardName, "%s", "hw:0,0");

	aoAttr.soundCard.channels = 2;       // 2
	aoAttr.soundCard.sampleRate = 48000; // s32SampleRate;       16000
	aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

	aoAttr.enBitwidth = AUDIO_BIT_WIDTH_16;                   // AUDIO_BIT_WIDTH_16
	aoAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)s32SampleRate; // 16000

	if (channel == 1)
		aoAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	else if (channel == 2)
		aoAttr.enSoundmode = AUDIO_SOUND_MODE_STEREO;
	else {
		RK_LOGE("unsupport = %d", channel);
		return RK_FAILURE;
	}

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

	if (channel == 1)
		RK_MPI_AO_SetTrackMode(aoDevId, AUDIO_TRACK_OUT_STEREO);
	else
		RK_MPI_AO_SetTrackMode(aoDevId, AUDIO_TRACK_NORMAL);
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

RK_S32 init_mpi_adec(RK_S32 s32SampleRate, RK_S32 channel) {
	printf("\n=======%s=======\n", __func__);
	RK_S32 s32ret = 0;
	ADEC_CHN AdChn = 0;
	ADEC_CHN_ATTR_S pstChnAttr;
	memset(&pstChnAttr, 0, sizeof(ADEC_CHN_ATTR_S));

	pstChnAttr.stCodecAttr.enType = (RK_CODEC_ID_E)code_type;
	pstChnAttr.stCodecAttr.u32Channels = channel; // default 1
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

static RK_S32 adec_data_free(void *opaque) {
	if (opaque) {
		free(opaque);
		opaque = RK_NULL;
	}
	return 0;
}

static RK_CHAR optstr[] = "?::d:r:i:t:c:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -r 8000 -t g726 -i /tmp/aenc.g726\n", name);
	printf("\t-r: sample rate, Default:16000\n");
	printf("\t-t: --decode: decode type, Default:g726, Value:g711a, g711u, g726\n");
	printf("\t-i: input path, Default:\"/tmp/aenc.g726\"\n");
}

int main(int argc, char *argv[]) {
	RK_S32 u32SampleRate = 8000;
	RK_S32 u32Channel = 1;
	RK_U32 u32FrameCnt = 1024;
	RK_CHAR *pInPath = "/tmp/aenc.g726";
	RK_CHAR *pCodecName = "g726";
	int ret = 0;
	int c;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'r':
			u32SampleRate = atoi(optarg);
			break;
		case 'c':
			u32Channel = atoi(optarg);
			break;
		case 'i':
			pInPath = optarg;
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
				return -1;
			}
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	printf("#SampleRate: %d\n", u32SampleRate);
	printf("#Channel: %d\n", u32Channel);
	printf("#Frame Count: %d\n", u32FrameCnt);
	printf("#CodecName:%s\n", pCodecName);
	printf("#Input Path: %s\n", pInPath);

	if (pInPath) {
		adec_file = fopen(pInPath, "rb");
		if (!adec_file) {
			printf("ERROR: open file: %s fail, exit\n", pInPath);
			return -1;
		}
	}

	signal(SIGINT, sigterm_handler);

	RK_MPI_SYS_Init();
	if (open_device_ao(u32SampleRate, u32Channel, u32FrameCnt))
		return -1;

	if (init_mpi_adec(u32SampleRate, u32Channel))
		return -1;

	// adec bind ao
	MPP_CHN_S stSrcChn, stDestChn;
	stSrcChn.enModId = RK_ID_ADEC;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 0;

	stDestChn.enModId = RK_ID_AO;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;

	// 3. bind ADEC-AO
	ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (ret) {
		printf("Bind ADEC[0] to AO[0] failed! ret=%d\n", ret);
		return -1;
	}

	printf("%s initial finish\n", __func__);

	RK_U8 *srcData = RK_NULL;
	RK_S32 srcSize = 0;
	RK_S32 pktEos = 0;
	RK_U64 timeStamp = 0;
	RK_S32 count = 0;
	AUDIO_STREAM_S stAudioStream;

	while (!quit) {
		srcData = calloc(u32FrameCnt, sizeof(RK_U8));
		memset(srcData, 0, u32FrameCnt);

		srcSize = fread(srcData, 1, u32FrameCnt, adec_file);
		if (srcSize == 0 || srcData == RK_NULL) {
			RK_LOGI("read eos packet, now send eos packet!");
			pktEos = 1;
		}

		if (pktEos) {
			RK_MPI_ADEC_SendEndOfStream(0, RK_FALSE);
			// free srcData here, otherwise it will be leaked
			free(srcData);
			break;
		} else {
			stAudioStream.u32Len = srcSize;
			stAudioStream.u64TimeStamp = timeStamp;
			stAudioStream.u32Seq = ++count;
			stAudioStream.bBypassMbBlk = RK_TRUE;
			MB_EXT_CONFIG_S extConfig = {0};
			extConfig.pFreeCB = adec_data_free;
			extConfig.pOpaque = srcData;
			extConfig.pu8VirAddr = srcData;
			extConfig.u64Size = srcSize;
			RK_MPI_SYS_CreateMB(&(stAudioStream.pMbBlk), &extConfig);
		__RETRY:
			ret = RK_MPI_ADEC_SendStream(0, &stAudioStream, RK_TRUE);
			if (ret != RK_SUCCESS) {
				RK_LOGE("fail to send adec stream.");
				goto __RETRY;
			}
			RK_MPI_MB_ReleaseMB(stAudioStream.pMbBlk);
		}
		timeStamp++;
	}

	if (!quit)
		RK_MPI_AO_WaitEos(0, 0, -1);

	if (adec_file) {
		fclose(adec_file);
		adec_file = RK_NULL;
	}

	RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	RK_MPI_AO_DisableReSmp(0, 0);
	RK_MPI_AO_DisableChn(0, 0);
	RK_MPI_AO_Disable(0);
	RK_MPI_ADEC_DestroyChn(0);
	RK_MPI_SYS_Exit();
	return 0;
}
