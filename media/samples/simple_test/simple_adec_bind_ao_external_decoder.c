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
#include "mp3dec.h"
#include "rkdemuxer.h"

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_U32 code_type = RK_AUDIO_ID_MP3;

static FILE *adec_file;
#define MP3MAXFRAMESIZE 4608
static RK_S32 extCodecHandle = -1;
static RK_U32 mp3DecInitCnt = 0;

typedef struct _MP3DecInfo
{
	/* pointers to platform-specific data structures */
	void *FrameHeaderPS;
	void *SideInfoPS;
	void *ScaleFactorInfoPS;
	void *HuffmanInfoPS;
	void *DequantInfoPS;
	void *IMDCTInfoPS;
	void *SubbandInfoPS;

	/* buffer which must be large enough to hold largest possible main_data section */
	unsigned char mainBuf[MAINBUF_SIZE];

	/* special info for "free" bitrate files */
	int freeBitrateFlag;
	int freeBitrateSlots;

	/* user-accessible info */
	int bitrate;
	int nChans;
	int samprate;
	int nGrans;                /* granules per frame */
	int nGranSamps;            /* samples per granule */
	int nSlots;
	int layer;
	int vbr;                    /* is vbr flag */
	uint32_t fSize;             /* file length, no tags */
	uint32_t fCount;            /* frame count */
	char TOC[100];              /* TOC */

	MPEGVersion version;

	int mainDataBegin;
	int mainDataBytes;

	int part23Length[MAX_NGRAN][MAX_NCHAN];
} MP3DecInfo;

typedef struct {
	HMP3Decoder pMp3Dec;
	MP3FrameInfo frameInfo;
	RK_U8 decMp3buf[MP3MAXFRAMESIZE];
	short int decPCMbuf[MP3MAXFRAMESIZE];
	RK_U64 audioPts;
} RK_ADEC_MP3_CTX_S;

static RK_S32 RKAduioMp3DecoderOpen(RK_VOID *pDecoderAttr, RK_VOID **ppDecoder) {
	if (pDecoderAttr == NULL) {
		RK_LOGE("pDecoderAttr is NULL");
		return RK_FAILURE;
	}

	RK_ADEC_MP3_CTX_S *ctx = (RK_ADEC_MP3_CTX_S *)malloc(sizeof(RK_ADEC_MP3_CTX_S));
	if (!ctx) {
		RK_LOGE("malloc adec mp3 ctx failed");
		return RK_FAILURE;
	}

	memset(ctx, 0, sizeof(RK_ADEC_MP3_CTX_S));
	ctx->pMp3Dec = MP3InitDecoder();
	if (!ctx->pMp3Dec) {
		RK_LOGE("malloc adec pMp3Dec failed");
		free(ctx);
		ctx = NULL;
		return RK_FAILURE;
	}

	memset(ctx->decMp3buf, 0, sizeof(ctx->decMp3buf));
	memset(ctx->decPCMbuf, 0, sizeof(ctx->decPCMbuf));

	*ppDecoder = (RK_VOID *)ctx;
	return RK_SUCCESS;
}

static RK_S32 RKAduioMp3DecoderClose(RK_VOID *pDecoder) {
	RK_ADEC_MP3_CTX_S *ctx = (RK_ADEC_MP3_CTX_S *)pDecoder;
	if (ctx == NULL)
		return RK_FAILURE;

	if (ctx->pMp3Dec) {
		MP3FreeDecoder(ctx->pMp3Dec);
		ctx->pMp3Dec = NULL;
	}

	if (ctx)
		free(ctx);

	return RK_SUCCESS;
}

static RK_S32 RKAduioMp3DecoderDecode(RK_VOID *pDecoder, RK_VOID *pDecParam) {
	RK_S32 ret = 0;
	RK_ADEC_MP3_CTX_S *ctx = (RK_ADEC_MP3_CTX_S *)pDecoder;
	RK_S32 skip = 0;

	if (ctx == NULL || ctx->pMp3Dec == NULL || pDecParam == NULL)
		return ADEC_DECODER_ERROR;

	AUDIO_ADENC_PARAM_S *pParam = (AUDIO_ADENC_PARAM_S *)pDecParam;
	RK_BOOL eos = RK_FALSE;
	RK_U8 *pInput = NULL;
	RK_S32 inLength = pParam->u32InLen;
	RK_S32 copySize = 0;
	RK_U64 calcPts = 0;

	if ((pParam->pu8InBuf == NULL) || (inLength == 0))
		eos = RK_TRUE;

	copySize =  (inLength <= MP3MAXFRAMESIZE) ? inLength : MP3MAXFRAMESIZE;
	inLength -= copySize;
	memcpy(ctx->decMp3buf, pParam->pu8InBuf, copySize);
	pInput = (RK_U8 *)ctx->decMp3buf;
	if (copySize && (skip = MP3FindSyncWord(pInput, copySize)) < 0) {
		printf("mp3 decode don't find sync word\n");
		pParam->u32InLen = 0;
		return ADEC_DECODER_ERROR;
	}

	copySize -= skip;
	pInput += skip;

	ret = MP3Decode(ctx->pMp3Dec, &pInput, &copySize, ctx->decPCMbuf, 0);
	pParam->u32InLen = inLength + copySize;

	if (ret) {
		if(eos)
		return ADEC_DECODER_EOS;

		pParam->u32InLen = inLength;
		if (ret == ERR_MP3_INDATA_UNDERFLOW) {
		RK_LOGE("mp3 decode input data underflow");
		return ADEC_DECODER_ERROR;
		}

		if(ret == -2) {
		RK_LOGW("mp3 encoded data does not start from the first frame");
		} else {
		RK_LOGE("mp3 decode error, ret = %d", ret);
		return ADEC_DECODER_ERROR;
		}
	}

	MP3GetLastFrameInfo(ctx->pMp3Dec, &ctx->frameInfo);

	MP3DecInfo *mp3DecInfo = (MP3DecInfo *)ctx->pMp3Dec;
	pParam->u32OutLen = mp3DecInfo->nGrans * mp3DecInfo->nGranSamps * ctx->frameInfo.nChans * 2;
	memcpy(pParam->pu8OutBuf, (RK_U8 *)ctx->decPCMbuf, pParam->u32OutLen);

	calcPts = (((RK_U64)pParam->u32OutLen / 2) * 1000000) / ctx->frameInfo.samprate;
	if (pParam->u64InTimeStamp)
		pParam->u64OutTimeStamp = pParam->u64InTimeStamp;
	else if (ctx->audioPts)
		pParam->u64OutTimeStamp = ctx->audioPts + calcPts;

	if (pParam->u64OutTimeStamp)
		ctx->audioPts = pParam->u64OutTimeStamp;
	else {
		ctx->audioPts += calcPts;
	}

	return ADEC_DECODER_OK;
}

static RK_S32 RKAduioMp3DecoderGetFrameInfo(RK_VOID *pDecoder, RK_VOID *pInfo) {
	ADEC_FRAME_INFO_S stFrameInfo;
	RK_ADEC_MP3_CTX_S *ctx = (RK_ADEC_MP3_CTX_S *)pDecoder;

	if (ctx == NULL || pInfo == NULL)
		return RK_FAILURE;

	MP3GetLastFrameInfo(ctx->pMp3Dec, &ctx->frameInfo);

	if (!ctx->frameInfo.bitsPerSample) {
		RK_LOGE("mp3 decode get info failed");
		return RK_FAILURE;
	}

	memset(&stFrameInfo, 0, sizeof(ADEC_FRAME_INFO_S));
	stFrameInfo.u32Channels = ctx->frameInfo.nChans;
	stFrameInfo.u32SampleRate = ctx->frameInfo.samprate;
	stFrameInfo.enBitWidth = AUDIO_BIT_WIDTH_16;
	memcpy(pInfo, &stFrameInfo, sizeof(ADEC_FRAME_INFO_S));
	return RK_SUCCESS;
}

RK_S32 RegisterAdecMp3() {
	if (!mp3DecInitCnt) {
		ADEC_DECODER_S adecCtx;
		memset(&adecCtx, 0, sizeof(ADEC_DECODER_S));

		extCodecHandle = -1;
		adecCtx.enType = RK_AUDIO_ID_MP3;
		snprintf((RK_CHAR*)(adecCtx.aszName), sizeof(adecCtx.aszName), "rkaudio1");
		adecCtx.pfnOpenDecoder  = RKAduioMp3DecoderOpen;
		adecCtx.pfnDecodeFrm    = RKAduioMp3DecoderDecode;
		adecCtx.pfnGetFrmInfo   = RKAduioMp3DecoderGetFrameInfo;
		adecCtx.pfnCloseDecoder = RKAduioMp3DecoderClose;
		adecCtx.pfnResetDecoder = RK_NULL;

		RK_LOGD("register ext audio decoder");
		RK_S32 ret = RK_MPI_ADEC_RegisterDecoder(&extCodecHandle, &adecCtx);
		if (ret != RK_SUCCESS) {
			RK_LOGE("adec register decoder fail, ret = 0x%x", ret);
			return RK_FAILURE;
		}
	}

	mp3DecInitCnt++;
	return RK_SUCCESS;
}

RK_S32 UnRegisterAdecMp3() {
	if (extCodecHandle == -1)
		return RK_SUCCESS;

	if (0 == mp3DecInitCnt) {
		return 0;
	} else if (1 == mp3DecInitCnt) {
		RK_LOGD("unregister ext audio decoder");
		RK_S32 ret = RK_MPI_ADEC_UnRegisterDecoder(extCodecHandle);
		if (ret != RK_SUCCESS) {
			RK_LOGE("adec unregister decoder fail, ret = 0x%x", ret);
			return RK_FAILURE;
		}

		extCodecHandle = -1;
	}

	mp3DecInitCnt--;
	return RK_SUCCESS;
}

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

	aoAttr.u32PtNumPerFrm = u32FrameCnt; // 1152
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
	printf("\t%s -r 8000 -t g726 -i /tmp/aenc.mp3\n", name);
	printf("\t-r: sample rate, Default:16000\n");
	printf("\t-t: --decode: decode type, Default:mp3, Value:mp3, g711a, g711u, g726\n");
	printf("\t-i: input path, Default:\"/tmp/aenc.mp3\"\n");
}

int main(int argc, char *argv[]) {
	RK_S32 u32SampleRate = 8000;
	RK_S32 u32Channel = 1;
	RK_U32 u32FrameCnt = 1152;
	RK_CHAR *pInPath = "/tmp/aenc.mp3";
	RK_CHAR *pCodecName = "mp3";
	int ret = 0;
	int c;
	if (RegisterAdecMp3())
		return -1;

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
			} else if (!strcmp(optarg, "mp3")) {
				code_type = RK_AUDIO_ID_MP3;
				pCodecName = "mp3";
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
	void *demuxerCfg = NULL;
	if (code_type == RK_AUDIO_ID_MP3) {
		u32FrameCnt = 1152;
		DemuxerInput stDemuxerInput;
		DemuxerParam tempDemuxerParam;
		memset(&stDemuxerInput, 0, sizeof(DemuxerInput));
		stDemuxerInput.ptr = NULL;
		stDemuxerInput.s8ReadModeFlag = 1;
		stDemuxerInput.s8VideoEnableFlag = 0;
		stDemuxerInput.s8AudioEnableFlag = 1;
		stDemuxerInput.transport = 0;
		stDemuxerInput.rtsp_io_timeout = 0;

		ret = rkdemuxer_init(&demuxerCfg, &stDemuxerInput);
		if (ret) {
			RK_LOGE("rkdemuxer init failed");
			return -1;
		}

		ret = rkdemuxer_get_param(demuxerCfg, pInPath, &tempDemuxerParam);
		if (ret) {
			RK_LOGE("rkdemuxer get param failed");
			return -1;
		}

		if (open_device_ao(tempDemuxerParam.s32AudioSampleRate, tempDemuxerParam.s32AudioChannels, u32FrameCnt))
			return -1;

		if (init_mpi_adec(tempDemuxerParam.s32AudioSampleRate))
			return -1;
	} else {
		if (open_device_ao(u32SampleRate, u32Channel, u32FrameCnt))
			return -1;

		if (init_mpi_adec(u32SampleRate))
			return -1;
	}

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
		if (code_type == RK_AUDIO_ID_MP3) {
			DemuxerPacket outPacket;
			ret = rkdemuxer_read_one_audio_packet(demuxerCfg, &outPacket);
			if (!ret) {
				srcData = (unsigned char *)outPacket.s8PacketData;
				srcSize = outPacket.s32PacketSize;
			} else
				continue;
		} else {
			srcData = calloc(u32FrameCnt, sizeof(RK_U8));
			memset(srcData, 0, u32FrameCnt);

			srcSize = fread(srcData, 1, u32FrameCnt, adec_file);
		}

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
	if (code_type == RK_AUDIO_ID_MP3) {
		if (demuxerCfg)
			rkdemuxer_deinit(&demuxerCfg);

		UnRegisterAdecMp3();
	}

	RK_MPI_SYS_Exit();
	return 0;
}
