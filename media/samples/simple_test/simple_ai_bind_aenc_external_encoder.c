// ai->aenc
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
#include "rk_mpi_aenc.h"
#include "rk_mpi_ai.h"
#include "rk_mpi_amix.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"

#include "mp3_enc_types.h"
#include "mp3_enc_table1.h"
#include "mp3_enc_table2.h"
#include "aenc_mp3_register.h"

static RK_S32 extCodecHandle = -1;
static RK_U32 mp3InitCnt = 0;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_U32 code_type = RK_AUDIO_ID_MP3;

static FILE *save_file;

typedef struct _RK_AENC_MP3_CTX_S {
	mp3_enc *pMp3Enc;
	RK_S32 frameLength;
} RK_AENC_MP3_CTX_S;

RK_S32 RKAduioMp3EncoderOpen(RK_VOID *pEncoderAttr, RK_VOID **ppEncoder) {
	int bitrate = 0;
	if (pEncoderAttr == NULL) {
		RK_LOGE("pEncoderAttr is NULL");
		return RK_FAILURE;
	}

	AENC_ATTR_CODEC_S *attr = (AENC_ATTR_CODEC_S *)pEncoderAttr;
	if(attr->enType != RK_AUDIO_ID_MP3) {
		RK_LOGE("Invalid enType[%d]", attr->enType);
		return RK_FAILURE;
	}

	RK_AENC_MP3_CTX_S *ctx = (RK_AENC_MP3_CTX_S *)malloc(sizeof(RK_AENC_MP3_CTX_S));
	if (!ctx) {
		RK_LOGE("malloc aenc mp3 ctx failed");
		return RK_FAILURE;
	}

	memset(ctx, 0, sizeof(RK_AENC_MP3_CTX_S));
	if (attr->u32Resv[0] > 1152) {
		RK_LOGE("error: MP3 FrameLength is too large, FrameLength = %d", attr->u32Resv[0]);
		goto __FAILED;
	}

	ctx->frameLength = attr->u32Resv[0];
	bitrate = attr->u32Resv[1] / 1000;
	RK_LOGD("MP3Encode: sample_rate = %d, channel = %d, bitrate = %d.", attr->u32SampleRate, attr->u32Channels, bitrate);
	ctx->pMp3Enc = Mp3EncodeVariableInit(attr->u32SampleRate, attr->u32Channels, bitrate);
	if (ctx->pMp3Enc->frame_size <= 0){
		RK_LOGE("MP3Encode init failed! r:%d c:%d\n", attr->u32SampleRate, attr->u32Channels);
		goto __FAILED;
	}

	RK_LOGD("MP3Encode FrameSize = %d", ctx->pMp3Enc->frame_size);
	*ppEncoder = (RK_VOID *)ctx;

	return RK_SUCCESS;

	__FAILED:
	RKAduioMp3EncoderClose((RK_VOID *)ctx);
	*ppEncoder = RK_NULL;
	return RK_FAILURE;
}

RK_S32 RKAduioMp3EncoderClose(RK_VOID *pEncoder) {
	RK_AENC_MP3_CTX_S *ctx = (RK_AENC_MP3_CTX_S *)pEncoder;
	if (ctx == NULL)
		return RK_SUCCESS;

	Mp3EncodeDeinit(ctx->pMp3Enc);
	free(ctx);
	ctx = NULL;
	return RK_SUCCESS;
}

RK_S32 RKAduioMp3EncoderEncode(RK_VOID *pEncoder, RK_VOID *pEncParam) {
	RK_AENC_MP3_CTX_S *ctx = (RK_AENC_MP3_CTX_S *)pEncoder;
	AUDIO_ADENC_PARAM_S *pParam = (AUDIO_ADENC_PARAM_S *)pEncParam;

	if(ctx == NULL || pParam == NULL) {
		RK_LOGE("Invalid ctx or pParam");
		return AENC_ENCODER_ERROR;
	}

	RK_U32 u32EncSize = 0;
	RK_U8 *inData = pParam->pu8InBuf;
	RK_U64 inPts = pParam->u64InTimeStamp;
	RK_U32 inbufSize = 0;
	RK_U32 copySize = 0;

	// if input buffer is NULL, this means eos(end of stream)
	if (inData == NULL)
		pParam->u64OutTimeStamp = inPts;

	if (pParam->u32InLen == 0)
		return AENC_ENCODER_EOS;

	inbufSize = 2 * ctx->pMp3Enc->frame_size;
	copySize = (pParam->u32InLen > inbufSize) ? inbufSize : pParam->u32InLen;
	memcpy(ctx->pMp3Enc->config.in_buf, inData, copySize);
	pParam->u32InLen = pParam->u32InLen - copySize;

	u32EncSize = L3_compress(ctx->pMp3Enc, 0, (unsigned char **)(&ctx->pMp3Enc->config.out_buf));

	u32EncSize = (u32EncSize > pParam->u32OutLen) ? pParam->u32OutLen : u32EncSize;
	memcpy(pParam->pu8OutBuf, ctx->pMp3Enc->config.out_buf, u32EncSize);
	pParam->u64OutTimeStamp = inPts;
	pParam->u32OutLen = u32EncSize;

	return AENC_ENCODER_OK;
}

RK_S32 RegisterAencMp3(void) {
	if (!mp3InitCnt) {
		RK_S32 ret;
		AENC_ENCODER_S aencCtx;
		memset(&aencCtx, 0, sizeof(AENC_ENCODER_S));

		extCodecHandle = -1;
		aencCtx.enType = RK_AUDIO_ID_MP3;
		snprintf((RK_CHAR*)(aencCtx.aszName),
				sizeof(aencCtx.aszName), "rkaudio");
		aencCtx.u32MaxFrmLen = 2048;
		aencCtx.pfnOpenEncoder = RKAduioMp3EncoderOpen;
		aencCtx.pfnEncodeFrm = RKAduioMp3EncoderEncode;
		aencCtx.pfnCloseEncoder = RKAduioMp3EncoderClose;

		RK_LOGD("register external aenc(%s)", aencCtx.aszName);
		ret = RK_MPI_AENC_RegisterEncoder(&extCodecHandle, &aencCtx);
		if (ret != RK_SUCCESS) {
		RK_LOGE("aenc %s register decoder fail %x", aencCtx.aszName, ret);
		return RK_FAILURE;
		}
	}

	mp3InitCnt++;
	return RK_SUCCESS;
}

RK_S32 UnRegisterAencMp3(void) {
	if (extCodecHandle == -1) {
		return RK_SUCCESS;
	}

	if (0 == mp3InitCnt) {
		return 0;
	} else if (1 == mp3InitCnt) {
		RK_LOGD("unregister external aenc");
		RK_S32 ret = RK_MPI_AENC_UnRegisterEncoder(extCodecHandle);
		if (ret != RK_SUCCESS) {
		RK_LOGE("aenc unregister decoder fail %x", ret);
		return RK_FAILURE;
		}

		extCodecHandle = -1;
	}
	mp3InitCnt--;
	return RK_SUCCESS;
}

//声音质量增强
RK_S32 init_ai_vqe(RK_S32 s32SampleRate) {
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

	AUDIO_TRACK_MODE_E trackMode;
	RK_MPI_AI_GetTrackMode(s32DevId, &trackMode);
	RK_LOGI("test info : get track mode = %d", trackMode);

	return 0;
}

RK_S32 open_device_ai(RK_S32 InputSampleRate, RK_S32 OutputSampleRate,
                      RK_S32 channel, RK_S32 u32FrameCnt) {
	printf("\n=======%s=======\n", __func__);
	AIO_ATTR_S aiAttr;
	AI_CHN_PARAM_S pstParams;
	RK_S32 result;
	int aiDevId = 0;
	int aiChn = 0;
	memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

	RK_BOOL needResample = (InputSampleRate != OutputSampleRate) ? RK_TRUE : RK_FALSE;
#ifdef RV1126_RV1109
	//这是RV1126 声卡打开设置，RV1106设置无效，可以不设置
	result = RK_MPI_AMIX_SetControl(aiDevId, "Capture MIC Path", (char *)"Main Mic");
	if (result != RK_SUCCESS) {
		RK_LOGE("ai set Capture MIC Path fail, reason = %x", result);
		goto __FAILED;
	}
#endif
	sprintf((char *)aiAttr.u8CardName, "%s", "hw:0,0");

	// s32DeviceSampleRate 和 OutputSampleRate,OutputSampleRate
	// 可以使用其他采样率，需要调用重采样函数。默认一样采样率。
	aiAttr.soundCard.channels = 2;
	aiAttr.soundCard.sampleRate = InputSampleRate;
	aiAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	aiAttr.enSamplerate = (AUDIO_SAMPLE_RATE_E)OutputSampleRate;

	if (channel == 1)
		aiAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	else if (channel == 2)
		aiAttr.enSoundmode = AUDIO_SOUND_MODE_STEREO;
	else {
		RK_LOGE("unsupport = %d", channel);
		return RK_FAILURE;
	}

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

	// init_ai_vqe(OutputSampleRate);
	if (channel == 1)
		RK_MPI_AI_SetTrackMode(aiDevId, AUDIO_TRACK_FRONT_LEFT);
	else
		RK_MPI_AI_SetTrackMode(aiDevId, AUDIO_TRACK_NORMAL);
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

RK_S32 init_mpi_aenc(RK_S32 s32SampleRate, RK_S32 channel) {
	printf("\n=======%s=======\n", __func__);
	RK_S32 s32ret = 0;
	AENC_CHN_ATTR_S pstChnAttr;
	RK_CODEC_ID_E enCodecId =
	    (RK_CODEC_ID_E)code_type; // RK_AUDIO_ID_ADPCM_G726 RK_AUDIO_ID_PCM_ALAW
	AUDIO_BIT_WIDTH_E enBitwidth = AUDIO_BIT_WIDTH_16;

	memset(&pstChnAttr, 0, sizeof(AENC_CHN_ATTR_S));

	printf("codecId=%d\n", (int)enCodecId);

	pstChnAttr.stCodecAttr.enType = enCodecId;
	pstChnAttr.stCodecAttr.u32Channels = channel; // default MONO
	pstChnAttr.stCodecAttr.u32SampleRate = s32SampleRate;
	pstChnAttr.stCodecAttr.enBitwidth = enBitwidth;
	pstChnAttr.stCodecAttr.pstResv = RK_NULL;
    if (enCodecId == RK_AUDIO_ID_MP3) {
        pstChnAttr.stCodecAttr.u32Resv[0] = 1152;
        pstChnAttr.stCodecAttr.u32Resv[1] = 160000;
    }

	pstChnAttr.enType = enCodecId;
	pstChnAttr.u32BufCount = 4;
	pstChnAttr.u32Depth    = 4;

	s32ret = RK_MPI_AENC_CreateChn(0, &pstChnAttr);
	if (s32ret) {
		RK_LOGE("create aenc chn %d err:0x%x\n", enCodecId, s32ret);
	}
	return s32ret;
}

static RK_CHAR optstr[] = "?::r:o:t:c:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s [-r 8000] [-t g726] -o /tmp/aenc.mp3\n", name);
	printf("\t-r: sample rate, Default:8000\n");
	printf("\t-t: --encode: encode type, Default:mp3, Value:mp3, g711a, g711u, g726\n");
	printf("\t-o: output path, Default:\"/tmp/aenc.mp3\"\n");
}

// 编码g726的时候，只支持8000 单声道，所以需要设置所以需要设置track_mode为8
int main(int argc, char *argv[]) {
	RK_S32 u32SampleRate = 8000;
	RK_S32 u32Channel = 1;
	RK_U32 u32FrameCnt = 1152;
	RK_CHAR *pOutPath = (RK_CHAR *)"/tmp/aenc.mp3";
	RK_CHAR *pCodecName = (RK_CHAR *)"mp3";
	RegisterAencMp3();
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
		case 'o':
			pOutPath = optarg;
			break;
		case 't':
			if (!strcmp(optarg, "g711a")) {
				code_type = RK_AUDIO_ID_PCM_ALAW;
				pCodecName = (RK_CHAR *)"g711a";
			} else if (!strcmp(optarg, "g711u")) {
				code_type = RK_AUDIO_ID_PCM_MULAW;
				pCodecName = (RK_CHAR *)"g711u";
			} else if (!strcmp(optarg, "g726")) {
				code_type = RK_AUDIO_ID_ADPCM_G726;
				pCodecName = (RK_CHAR *)"g726";
			} else if (!strcmp(optarg, "mp3")) {
				code_type = RK_AUDIO_ID_MP3;
				pCodecName = (RK_CHAR *)"mp3";
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

	if (u32SampleRate == 32000 || u32SampleRate == 44100 || u32SampleRate == 48000)
		u32FrameCnt = 1152 * 2 * u32Channel;
	else
		u32FrameCnt = 576 * 2 * u32Channel;

	printf("#SampleRate: %d\n", u32SampleRate);
	printf("#Channel: %d\n", u32Channel);
	printf("#Frame Count: %d\n", u32FrameCnt);
	printf("#CodecName:%s\n", pCodecName);
	printf("#Output Path: %s\n", pOutPath);

	if (pOutPath) {
		save_file = fopen(pOutPath, "w");
		if (!save_file) {
			printf("ERROR: open file: %s fail, exit\n", pOutPath);
			return -1;
		}
	}

	signal(SIGINT, sigterm_handler);

	RK_MPI_SYS_Init();

	if (open_device_ai(u32SampleRate, u32SampleRate, u32Channel, u32FrameCnt))
		return -1;

	if (init_mpi_aenc(u32SampleRate, u32Channel))
		return -1;

	// ai bind aenc
	MPP_CHN_S stSrcChn, stDestChn;
	stSrcChn.enModId = RK_ID_AI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 0;

	stDestChn.enModId = RK_ID_AENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;

	// 3. bind AI-AENC
	ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (ret) {
		printf("Bind AI[0] to AENC[0] failed! ret=%d\n", ret);
		return -1;
	}

	printf("%s initial finish\n", __func__);

	AUDIO_STREAM_S pstStream;
	RK_S32 eos = 0;
	while (!quit) {
		ret = RK_MPI_AENC_GetStream(0, &pstStream, -1);
		if (ret == RK_SUCCESS) {
			MB_BLK bBlk = pstStream.pMbBlk;
			RK_VOID *pstFrame = RK_MPI_MB_Handle2VirAddr(bBlk);
			RK_S32 frameSize = pstStream.u32Len;
			RK_U64 timeStamp = pstStream.u64TimeStamp;
			eos = (frameSize <= 0) ? 1 : 0;
			if (pstFrame) {
				RK_LOGI("get frame data = %p, size = %d, timeStamp = %llu", pstFrame,
				        frameSize, timeStamp);
				if (save_file) {
					fwrite(pstFrame, frameSize, 1, save_file);
					fflush(save_file);
				}
				RK_MPI_AENC_ReleaseStream(0, &pstStream);
			}
		} else {
			RK_LOGE("fail to get aenc frame.");
		}

		if (eos) {
			RK_LOGI("get eos stream.");
			break;
		}
	}

	RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	// RK_MPI_AI_DisableVqe(0, 0);
	RK_MPI_AI_DisableChn(0, 0);
	RK_MPI_AI_Disable(0);
	RK_MPI_AENC_DestroyChn(0);
	if (code_type == RK_AUDIO_ID_MP3)
		UnRegisterAencMp3();
	RK_MPI_SYS_Exit();
	return 0;
}
