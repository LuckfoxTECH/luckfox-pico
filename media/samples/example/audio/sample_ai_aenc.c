/*
 * Copyright 2021 Rockchip Electronics Co. LTD
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
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

typedef struct _rkMpiCtx {
	SAMPLE_AI_CTX_S ai;
	SAMPLE_AENC_CTX_S aenc;
} SAMPLE_MPI_CTX_S;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_CODEC_ID_E code_type = RK_AUDIO_ID_MP2;

#define MP3_PROFILE_LOW 1

typedef struct FreqIdx_ {
	RK_S32 u32SmpRate;
	RK_U8 u8FreqIdx;
} FreqIdx;

FreqIdx FreqIdxTbl[13] = {{96000, 0},  {88200, 1}, {64000, 2}, {48000, 3}, {44100, 4},
                          {32000, 5},  {24000, 6}, {22050, 7}, {16000, 8}, {12000, 9},
                          {11025, 10}, {8000, 11}, {7350, 12}};

static void GetHeader(RK_U8 *pu8Hdr, RK_S32 u32SmpRate, RK_U8 u8Channel,
                      RK_U32 u32DataLen) {
	RK_U8 u8FreqIdx = 0;
	for (int i = 0; i < 13; i++) {
		if (u32SmpRate == FreqIdxTbl[i].u32SmpRate) {
			u8FreqIdx = FreqIdxTbl[i].u8FreqIdx;
			break;
		}
	}

	RK_U32 u32PacketLen = u32DataLen + 7;
	pu8Hdr[0] = 0xFF;
	pu8Hdr[1] = 0xF1;
	pu8Hdr[2] = ((MP3_PROFILE_LOW) << 6) + (u8FreqIdx << 2) + (u8Channel >> 2);
	pu8Hdr[3] = (((u8Channel & 3) << 6) + (u32PacketLen >> 11));
	pu8Hdr[4] = ((u32PacketLen & 0x7FF) >> 3);
	pu8Hdr[5] = (((u32PacketLen & 7) << 5) + 0x1F);
	pu8Hdr[6] = 0xFC;
}

/******************************************************************************
 * function : aenc thread
 ******************************************************************************/
static void *aenc_get_stream(void *pArgs) {
	SAMPLE_AENC_CTX_S *ctx = (SAMPLE_AENC_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	char name[256] = {0};
	FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;
	RK_U8 header[7];

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/aenc_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			quit = true;
			return RK_NULL;
		}
	}

	while (!quit) {
		s32Ret = SAMPLE_COMM_AENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_AENC_ReleaseStream(ctx);
					quit = true;
					break;
				}
			}

			if (fp) {
				if (code_type == RK_AUDIO_ID_MP3) {
					GetHeader(header, ctx->stChnAttr.stCodecAttr.u32SampleRate,
					          ctx->stChnAttr.stCodecAttr.u32Channels,
					          ctx->stFrame.u32Len);
					fwrite(header, 1, 7, fp);
				}
				fwrite(pData, 1, ctx->stFrame.u32Len, fp);
				fflush(fp);
			}

			RK_LOGE("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.u32Len);

			SAMPLE_COMM_AENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	if (fp)
		fclose(fp);

	return NULL;
}

static RK_CHAR optstr[] = "?::d:w:R:r:C:c:e:l:o:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -d default:CARD=rockchipes8388 -w 16bit -R 16000 -r 16000 -C 2 -c 2 -e "
	       "mp2 -o /data/\n",
	       name);
	printf("\t-d | --device_name: the sound name for open sound card, eg: "
	       "default:CARD=rockchipes8388c Default(NULL)\n");
	printf("\t-w | --bit_width: the bit width of open sound card, Default:16bit, "
	       "Value:8bit, 16bit, 24bit\n");
	printf("\t-R | --device_rate: the sample rate of open sound card, Default:16000\n");
	printf("\t-r | --out_rate: the sample rate of out data, Default:16000\n");
	printf("\t-C | --device_ch: the number of sound card channels, Default:2\n");
	printf("\t-c | --out_ch: the channels of out data, Default:2\n");
	printf("\t-e | --encode: encode type, Defaule:mp3, Value:mp3, mp2, g711a, g711u, "
	       "g726\n");
	printf("\t-l | --loop_count: loop running count, Default -1\n");
	printf("\t-o | --output_path: encode save file path, Default /data/\n");
}

int main(int argc, char *argv[]) {
	SAMPLE_MPI_CTX_S *ctx;
	MPP_CHN_S stSrcChn, stDestChn;
	RK_S32 s32DeviceChannel = 2;
	RK_S32 s32DeviceSampleRate = 16000;
	AUDIO_BIT_WIDTH_E bitWidth = AUDIO_BIT_WIDTH_16;
	RK_S32 s32SampleRate = 8000;
	AUDIO_SOUND_MODE_E soundMode = AUDIO_SOUND_MODE_STEREO;
	RK_S32 s32ChnCnt = 2;
	RK_S32 s32PeriodCount = 4;
	RK_S32 s32PeriodSize = 1024;
	RK_S32 s32loopCnt = -1;
	RK_CHAR *pCardName = NULL;
	RK_CHAR *pOutPath = NULL;
	int c;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	signal(SIGINT, sigterm_handler);

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'd':
			pCardName = optarg;
			break;
		case 'r':
			s32SampleRate = atoi(optarg);
			break;
		case 'R':
			s32DeviceSampleRate = atoi(optarg);
			break;
		case 'c':
			s32ChnCnt = atoi(optarg);
			break;
		case 'C':
			s32DeviceChannel = atoi(optarg);
			break;
		case 'w':
			if (!strcmp(optarg, "8bit")) {
				bitWidth = AUDIO_BIT_WIDTH_8;
			} else if (!strcmp(optarg, "16bit")) {
				bitWidth = AUDIO_BIT_WIDTH_16;
			} else if (!strcmp(optarg, "24bit")) {
				bitWidth = AUDIO_BIT_WIDTH_24;
			}
			break;
		case 'o':
			pOutPath = optarg;
			break;
		case 'e':
			if (!strcmp(optarg, "mp2")) {
				code_type = RK_AUDIO_ID_MP2;
			} else if (!strcmp(optarg, "mp3")) {
				code_type = RK_AUDIO_ID_MP3;
			} else if (!strcmp(optarg, "g726")) {
				code_type = RK_AUDIO_ID_ADPCM_G726;
			} else if (!strcmp(optarg, "g711a")) {
				code_type = RK_AUDIO_ID_PCM_ALAW;
			} else if (!strcmp(optarg, "g711u")) {
				code_type = RK_AUDIO_ID_PCM_MULAW;
			}
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	if (s32ChnCnt == 1) {
		soundMode = AUDIO_SOUND_MODE_MONO;
	} else if (s32ChnCnt == 2) {
		soundMode = AUDIO_SOUND_MODE_STEREO;
	} else {
		printf("%s channel = %d not support\n", __func__, s32ChnCnt);
		return 0;
	}

	printf("#Device: %s\n", pCardName);
	printf("#SampleRate: %d\n", s32SampleRate);
	printf("#Channel Count: %d\n", s32ChnCnt);
	printf("#Loop Count: %d\n", s32loopCnt);
	printf("#Output Path: %s\n", pOutPath);
	printf("#code_type: %d\n", code_type);

	RK_MPI_SYS_Init();

	// Init AI[0]
	ctx->ai.s32DevId = 0;
	ctx->ai.s32ChnId = 0;
	ctx->ai.stAiAttr.soundCard.channels = s32DeviceChannel;
	ctx->ai.stAiAttr.soundCard.sampleRate = s32DeviceSampleRate;
	ctx->ai.stAiAttr.soundCard.bitWidth = bitWidth;
	ctx->ai.stAiAttr.enBitwidth = bitWidth;
	ctx->ai.stAiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)s32SampleRate;
	ctx->ai.stAiAttr.enSoundmode = soundMode;
	ctx->ai.stAiAttr.u32FrmNum = s32PeriodCount;
	ctx->ai.stAiAttr.u32PtNumPerFrm = s32PeriodSize;
	ctx->ai.stAiAttr.u32EXFlag = 0;
	ctx->ai.stAiAttr.u32ChnCnt = s32ChnCnt;
	if (pCardName) {
		strcpy((char *)ctx->ai.stAiAttr.u8CardName, pCardName);
	}
	SAMPLE_COMM_AI_CreateChn(&ctx->ai);

	// Init AENC[0]
	ctx->aenc.s32ChnId = 0;
	ctx->aenc.s32loopCount = s32loopCnt;
	ctx->aenc.stChnAttr.enType = code_type;
	ctx->aenc.stChnAttr.stCodecAttr.u32Channels = s32ChnCnt;
	ctx->aenc.stChnAttr.stCodecAttr.u32SampleRate = s32SampleRate;
	ctx->aenc.stChnAttr.stCodecAttr.enBitwidth = bitWidth;
	ctx->aenc.stChnAttr.u32BufCount = 4;
	ctx->aenc.dstFilePath = pOutPath;
	SAMPLE_COMM_AENC_CreateChn(&ctx->aenc);

	// Bind AI[0] and AENC[0]
	stSrcChn.enModId = RK_ID_AI;
	stSrcChn.s32DevId = ctx->ai.s32DevId;
	stSrcChn.s32ChnId = ctx->ai.s32ChnId;
	stDestChn.enModId = RK_ID_AENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->aenc.s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	pthread_t getStreamThread;
	pthread_create(&getStreamThread, NULL, aenc_get_stream, (void *)(&ctx->aenc));

	printf("%s initial finish\n", __func__);

	while (!quit) {
		usleep(500000);
	}

	printf("%s exit!\n", __func__);

	pthread_join(getStreamThread, NULL);

	// UnBind AI[0] and AENC[0]
	stSrcChn.enModId = RK_ID_AI;
	stSrcChn.s32DevId = ctx->ai.s32DevId;
	stSrcChn.s32ChnId = ctx->ai.s32ChnId;
	stDestChn.enModId = RK_ID_AENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->aenc.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	// Destroy AENC[0]
	SAMPLE_COMM_AENC_DestroyChn(&ctx->aenc);
	// Destroy AI[0]
	SAMPLE_COMM_AI_DestroyChn(&ctx->ai);

	RK_MPI_SYS_Exit();

	return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
