// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "log.h"
#include "rtsp.h"
#include "storage.h"

#include <rk_debug.h>
#include <rk_mpi_aenc.h>
#include <rk_mpi_ai.h>
#include <rk_mpi_ao.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_sys.h>

#include "mp3_enc_table1.h"
#include "mp3_enc_table2.h"
#include "mp3_enc_types.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "audio.c"

pthread_t save_ai_tid, save_aenc_tid, ai_get_detect_result_tid;
static int ai_dev_id = 0;
static int ai_chn_id = 0;
static int aenc_dev_id = 0;
static int aenc_chn_id = 0;
static int g_audio_run_ = 1;
static int enable_aed, enable_bcd, enable_vqe;
MPP_CHN_S ai_chn, aenc_chn;

static void *ai_get_detect_result(void *arg);

void *save_ai_thread(void *ptr) {
	int ret = 0;
	int s32MilliSec = -1;
	AUDIO_FRAME_S frame;

	AUDIO_SAVE_FILE_INFO_S save;
	save.bCfg = RK_TRUE;
	save.u32FileSize = 1024;
	snprintf(save.aFilePath, sizeof(save.aFilePath), "%s", "/tmp/");
	snprintf(save.aFileName, sizeof(save.aFileName), "%s", "cap_out.pcm");
	RK_MPI_AI_SaveFile(ai_dev_id, ai_chn_id, &save);

	while (g_audio_run_) {
		ret = RK_MPI_AI_GetFrame(ai_dev_id, ai_chn_id, &frame, RK_NULL, s32MilliSec);
		if (ret == 0) {
			void *data = RK_MPI_MB_Handle2VirAddr(frame.pMbBlk);
			LOG_DEBUG("data = %p, len = %d\n", data, frame.u32Len);
			RK_MPI_AI_ReleaseFrame(ai_dev_id, ai_chn_id, &frame, RK_NULL);
		}
	}

	return RK_NULL;
}
static RK_S64 fake_time = 0;
void *save_aenc_thread(void *ptr) {
	prctl(PR_SET_NAME, "save_aenc_thread", 0, 0, 0);
	int s32ret = 0;
	FILE *file = RK_NULL;
	AUDIO_STREAM_S pstStream;
	int eos = 0;
	int count = 0;
	const char *encode_type = rk_param_get_string("audio.0:encode_type", NULL);

	// file = fopen("/tmp/aenc.mp3", "wb+");
	// if (file == RK_NULL) {
	// 	LOG_ERROR("failed to open /tmp/aenc.mp3, error: %s\n", strerror(errno));
	// 	return RK_NULL;
	// }

	while (g_audio_run_) {
		s32ret = RK_MPI_AENC_GetStream(aenc_chn_id, &pstStream, 1000);
		if (s32ret == 0) {
			MB_BLK bBlk = pstStream.pMbBlk;
			void *buffer = RK_MPI_MB_Handle2VirAddr(bBlk);
			eos = (pstStream.u32Len <= 0) ? 1 : 0;
			if (buffer) {
				// LOG_INFO("get frame data = %p, size = %d, pts is %lld, seq is %d\n", buffer,
				//          pstStream.u32Len, pstStream.u64TimeStamp, pstStream.u32Seq);
				if (!strcmp(encode_type, "MP2") || !strcmp(encode_type, "MP3")) {
					rk_storage_write_audio_frame(0, buffer, pstStream.u32Len, pstStream.u64TimeStamp);
					rk_storage_write_audio_frame(1, buffer, pstStream.u32Len, pstStream.u64TimeStamp);
					rk_storage_write_audio_frame(2, buffer, pstStream.u32Len, pstStream.u64TimeStamp);
				} else if (!strcmp(encode_type, "G711A")) {
					rkipc_rtsp_write_audio_frame(0, buffer, pstStream.u32Len, pstStream.u64TimeStamp);
				}
				// if (file) {
				// 	fwrite(buffer, pstStream.u32Len, 1, file);
				// 	fflush(file);
				// }
				RK_MPI_AENC_ReleaseStream(aenc_chn_id, &pstStream);
				count++;
			}
		} else {
			LOG_ERROR("fail to get aenc frame\n");
		}
		if (eos) {
			LOG_INFO("get eos stream\n");
			break;
		}
	}
	// if (file) {
	// 	fclose(file);
	// 	file = RK_NULL;
	// }

	return RK_NULL;
}

int rkipc_audio_aed_init() {
	int result;
	AI_AED_CONFIG_S ai_aed_config;

	ai_aed_config.fSnrDB = 10.0f;
	ai_aed_config.fLsdDB = -25.0f;
	ai_aed_config.s32Policy = 1;
	result = RK_MPI_AI_SetAedAttr(ai_dev_id, ai_chn_id, &ai_aed_config);
	if (result != 0) {
		LOG_ERROR("RK_MPI_AI_SetAedAttr(%d,%d) failed with %#x\n", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_SetAedAttr(%d,%d) success\n", ai_dev_id, ai_chn_id);
	result = RK_MPI_AI_EnableAed(ai_dev_id, ai_chn_id);
	if (result != 0) {
		LOG_ERROR("RK_MPI_AI_EnableAed(%d,%d) failed with %#x\n", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_EnableAed(%d,%d) success\n", ai_dev_id, ai_chn_id);

	return result;
}

int rkipc_audio_bcd_init() {
	int result;
	AI_BCD_CONFIG_S ai_bcd_config;

	ai_bcd_config.mFrameLen = 120;
	ai_bcd_config.mBlankFrameMax = 50;
	ai_bcd_config.mCryEnergy = -1.25f;
	ai_bcd_config.mJudgeEnergy = -0.75f;
	ai_bcd_config.mCryThres1 = 0.70f;
	ai_bcd_config.mCryThres2 = 0.55f;
	result = RK_MPI_AI_SetBcdAttr(ai_dev_id, ai_chn_id, &ai_bcd_config);
	if (result != 0) {
		LOG_ERROR("RK_MPI_AI_SetBcdAttr(%d,%d) failed with %#x\n", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_SetBcdAttr(%d,%d) success\n", ai_dev_id, ai_chn_id);
	result = RK_MPI_AI_EnableBcd(ai_dev_id, ai_chn_id);
	if (result != 0) {
		LOG_ERROR("RK_MPI_AI_EnableBcd(%d,%d) failed with %#x\n", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_EnableBcd(%d,%d) success\n", ai_dev_id, ai_chn_id);

	return result;
}

int rkipc_audio_vqe_init() {
	int result;
	AI_VQE_CONFIG_S stAiVqeConfig;
	int vqe_gap_ms = 16;
	if (vqe_gap_ms != 16 && vqe_gap_ms != 10) {
		LOG_ERROR("Invalid gap: %d, just supports 16ms or 10ms for AI VQE", vqe_gap_ms);
		return RK_FAILURE;
	}
	memset(&stAiVqeConfig, 0, sizeof(AI_VQE_CONFIG_S));
	stAiVqeConfig.enCfgMode = AIO_VQE_CONFIG_LOAD_FILE;
	memcpy(stAiVqeConfig.aCfgFile, "/oem/usr/share/vqefiles/config_aivqe.json",
	       strlen("/oem/usr/share/vqefiles/config_aivqe.json"));

	const char *vqe_cfg =
	    rk_param_get_string("audio.0:vqe_cfg", "/oem/usr/share/vqefiles/config_aivqe.json");
	memcpy(stAiVqeConfig.aCfgFile, vqe_cfg, strlen(vqe_cfg) + 1);
	memset(stAiVqeConfig.aCfgFile + strlen(vqe_cfg) + 1, '\0', sizeof(char));
	LOG_INFO("stAiVqeConfig.aCfgFile = %s\n", stAiVqeConfig.aCfgFile);

	stAiVqeConfig.s32WorkSampleRate = rk_param_get_int("audio.0:sample_rate", 16000);
	stAiVqeConfig.s32FrameSample =
	    rk_param_get_int("audio.0:sample_rate", 16000) * vqe_gap_ms / 1000;
	result = RK_MPI_AI_SetVqeAttr(ai_dev_id, ai_chn_id, 0, 0, &stAiVqeConfig);
	if (result != 0) {
		LOG_ERROR("RK_MPI_AI_SetVqeAttr(%d,%d) failed with %#x", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_SetVqeAttr(%d,%d) success\n", ai_dev_id, ai_chn_id);
	result = RK_MPI_AI_EnableVqe(ai_dev_id, ai_chn_id);
	if (result != 0) {
		LOG_ERROR("RK_MPI_AI_EnableVqe(%d,%d) failed with %#x", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_EnableVqe(%d,%d) success\n", ai_dev_id, ai_chn_id);

	return result;
}

static void *ai_get_detect_result(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "ai_get_detect_result", 0, 0, 0);
	int result;

	while (g_audio_run_) {
		usleep(1000 * 1000);
		AI_AED_RESULT_S aed_result;
		AI_BCD_RESULT_S bcd_result;
		memset(&aed_result, 0, sizeof(aed_result));
		memset(&bcd_result, 0, sizeof(bcd_result));
		if (enable_aed) {
			result = RK_MPI_AI_GetAedResult(ai_dev_id, ai_chn_id, &aed_result);
			if (result == 0) {
				LOG_DEBUG("aed_result: %d, %d", aed_result.bAcousticEventDetected,
				          aed_result.bLoudSoundDetected);
			}
		}
		if (enable_bcd) {
			result = RK_MPI_AI_GetBcdResult(ai_dev_id, ai_chn_id, &bcd_result);
			if (result == 0) {
				LOG_DEBUG("bcd_result: %d", bcd_result.bBabyCry);
			}
		}
	}

	return 0;
}

int rkipc_ai_init() {
	int ret;
	AUDIO_DEV aiDevId = ai_dev_id;
	AIO_ATTR_S aiAttr;

	memset(&aiAttr, 0, sizeof(AIO_ATTR_S));
	const char *card_name = rk_param_get_string("audio.0:card_name", "default");
	snprintf(aiAttr.u8CardName, sizeof(aiAttr.u8CardName), "%s", card_name);
	LOG_INFO("aiAttr.u8CardName is %s\n", aiAttr.u8CardName);
	aiAttr.soundCard.channels = 2;
	aiAttr.soundCard.sampleRate = rk_param_get_int("audio.0:sample_rate", 16000);
	const char *format = rk_param_get_string("audio.0:format", NULL);
	if (!strcmp(format, "S16")) {
		aiAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;
		aiAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	} else if (!strcmp(format, "U8")) {
		aiAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_8;
		aiAttr.enBitwidth = AUDIO_BIT_WIDTH_8;
	} else {
		LOG_ERROR("not support %s\n", format);
	}
	aiAttr.enSamplerate = rk_param_get_int("audio.0:sample_rate", 16000);
	aiAttr.u32FrmNum = 4;
	aiAttr.u32PtNumPerFrm = rk_param_get_int("audio.0:frame_size", 1024);
	aiAttr.u32EXFlag = 0;
	aiAttr.u32ChnCnt = 2;
	if (rk_param_get_int("audio.0:channels", 2) == 2)
		aiAttr.enSoundmode = AUDIO_SOUND_MODE_STEREO;
	else
		aiAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;

	ret = RK_MPI_AI_SetPubAttr(ai_dev_id, &aiAttr);
	if (ret != 0) {
		LOG_ERROR("ai set attr fail, reason = %d\n", ret);
		return RK_FAILURE;
	}

	ret = RK_MPI_AI_Enable(ai_dev_id);
	if (ret != 0) {
		LOG_ERROR("ai enable fail, reason = %d\n", ret);
		return RK_FAILURE;
	}

	// aed bcd vqe
	enable_aed = rk_param_get_int("audio.0:enable_aed", 0);
	enable_bcd = rk_param_get_int("audio.0:enable_bcd", 0);
	enable_vqe = rk_param_get_int("audio.0:enable_vqe", 0);
	if (enable_aed)
		rkipc_audio_aed_init();
	if (enable_bcd)
		rkipc_audio_bcd_init();
	if (enable_vqe)
		rkipc_audio_vqe_init();
	if (enable_aed || enable_bcd)
		pthread_create(&ai_get_detect_result_tid, RK_NULL, ai_get_detect_result, NULL);

	ret = RK_MPI_AI_EnableChn(ai_dev_id, ai_chn_id);
	if (ret != 0) {
		LOG_ERROR("ai enable channel fail, aoChn = %d, reason = %x\n", ai_chn_id, ret);
		return RK_FAILURE;
	}

	// ret = RK_MPI_AI_EnableReSmp(ai_dev_id, ai_chn_id,
	//                               (AUDIO_SAMPLE_RATE_E)params->s32SampleRate);
	// if (ret != 0) {
	//     LOG_ERROR("ai enable resample fail, reason = %x, aoChn = %d\n", ret, ai_chn_id);
	//     return RK_FAILURE;
	// }

	RK_MPI_AI_SetVolume(ai_dev_id, rk_param_get_int("audio.0:volume", 50));
	if (rk_param_get_int("audio.0:channels", 2) == 1) {
		RK_MPI_AI_SetTrackMode(ai_dev_id, AUDIO_TRACK_FRONT_LEFT);
	}

	// pthread_create(&save_ai_tid, RK_NULL, save_ai_thread, NULL);

	return 0;
}

int rkipc_ai_deinit() {
	// pthread_join(save_ai_tid, RK_NULL);
	// RK_MPI_AI_DisableReSmp(ai_dev_id, ai_chn_id);
	int ret = RK_MPI_AI_DisableChn(ai_dev_id, ai_chn_id);
	if (ret != 0) {
		LOG_ERROR("ai disable channel fail, reason = %d\n", ret);
		return RK_FAILURE;
	}
	LOG_DEBUG("RK_MPI_AI_DisableChn success\n");

	ret = RK_MPI_AI_Disable(ai_dev_id);
	if (ret != 0) {
		LOG_ERROR("ai disable fail, reason = %d\n", ret);
		return RK_FAILURE;
	}
	LOG_DEBUG("RK_MPI_AI_Disable success\n");

	return 0;
}

// mp3
static RK_S32 extCodecHandle = -1;

typedef struct _RK_AENC_MP3_CTX_S {
	mp3_enc *pMp3Enc;
	int frameLength;
} RK_AENC_MP3_CTX_S;

int rkaudio_mp3_encoder_close(RK_VOID *pEncoder) {
	RK_AENC_MP3_CTX_S *ctx = (RK_AENC_MP3_CTX_S *)pEncoder;
	if (ctx == NULL)
		return 0;

	Mp3EncodeDeinit(ctx->pMp3Enc);
	free(ctx);
	ctx = NULL;
	return 0;
}

int rkaudio_mp3_encoder_open(RK_VOID *pEncoderAttr, RK_VOID **ppEncoder) {
	int bitrate = 0;
	if (pEncoderAttr == NULL) {
		LOG_ERROR("pEncoderAttr is NULL");
		return RK_FAILURE;
	}

	AENC_ATTR_CODEC_S *attr = (AENC_ATTR_CODEC_S *)pEncoderAttr;
	if (attr->enType != RK_AUDIO_ID_MP3) {
		LOG_ERROR("Invalid enType[%d]", attr->enType);
		return RK_FAILURE;
	}

	RK_AENC_MP3_CTX_S *ctx = (RK_AENC_MP3_CTX_S *)malloc(sizeof(RK_AENC_MP3_CTX_S));
	if (!ctx) {
		LOG_ERROR("malloc aenc mp3 ctx failed");
		return RK_FAILURE;
	}

	memset(ctx, 0, sizeof(RK_AENC_MP3_CTX_S));
	if (attr->u32Resv[0] > 1152) {
		LOG_ERROR("error: MP3 FrameLength is too large, FrameLength = %d", attr->u32Resv[0]);
		goto __FAILED;
	}

	ctx->frameLength = attr->u32Resv[0];
	bitrate = attr->u32Resv[1] / 1000;
	LOG_DEBUG("MP3Encode: sample_rate = %d, channel = %d, bitrate = %d.", attr->u32SampleRate,
	          attr->u32Channels, bitrate);
	ctx->pMp3Enc = Mp3EncodeVariableInit(attr->u32SampleRate, attr->u32Channels, bitrate);
	if (ctx->pMp3Enc->frame_size <= 0) {
		LOG_ERROR("MP3Encode init failed! r:%d c:%d\n", attr->u32SampleRate, attr->u32Channels);
		goto __FAILED;
	}

	LOG_DEBUG("MP3Encode FrameSize = %d", ctx->pMp3Enc->frame_size);
	*ppEncoder = (RK_VOID *)ctx;

	return 0;

__FAILED:
	rkaudio_mp3_encoder_close((RK_VOID *)ctx);
	*ppEncoder = RK_NULL;
	return RK_FAILURE;
}

int rkaudio_mp3_encoder_encode(RK_VOID *pEncoder, RK_VOID *pEncParam) {
	RK_AENC_MP3_CTX_S *ctx = (RK_AENC_MP3_CTX_S *)pEncoder;
	AUDIO_ADENC_PARAM_S *pParam = (AUDIO_ADENC_PARAM_S *)pEncParam;

	if (ctx == NULL || pParam == NULL) {
		LOG_ERROR("Invalid ctx or pParam");
		return AENC_ENCODER_ERROR;
	}

	RK_U32 u32EncSize = 0;
	RK_U8 *inData = pParam->pu8InBuf;
	RK_U64 inPts = pParam->u64InTimeStamp;
	RK_U32 inbufSize = 0;
	RK_U32 copySize = 0;

	// if input buffer is NULL, this means eos(end of stream)
	if (inData == NULL) {
		pParam->u64OutTimeStamp = inPts;
	}

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

int register_aenc_mp3(void) {
	int ret;
	AENC_ENCODER_S aencCtx;
	memset(&aencCtx, 0, sizeof(AENC_ENCODER_S));

	extCodecHandle = -1;
	aencCtx.enType = RK_AUDIO_ID_MP3;
	snprintf((RK_CHAR *)(aencCtx.aszName), sizeof(aencCtx.aszName), "rkaudio");
	aencCtx.u32MaxFrmLen = 2048;
	aencCtx.pfnOpenEncoder = rkaudio_mp3_encoder_open;
	aencCtx.pfnEncodeFrm = rkaudio_mp3_encoder_encode;
	aencCtx.pfnCloseEncoder = rkaudio_mp3_encoder_close;

	LOG_DEBUG("register external aenc(%s)", aencCtx.aszName);
	ret = RK_MPI_AENC_RegisterEncoder(&extCodecHandle, &aencCtx);
	if (ret != 0) {
		LOG_ERROR("aenc %s register decoder fail %x", aencCtx.aszName, ret);
		return RK_FAILURE;
	}

	return 0;
}

int unregister_aenc_mp3(void) {
	if (extCodecHandle == -1) {
		return 0;
	}

	LOG_DEBUG("unregister external aenc");
	int ret = RK_MPI_AENC_UnRegisterEncoder(extCodecHandle);
	if (ret != 0) {
		LOG_ERROR("aenc unregister decoder fail %x", ret);
		return RK_FAILURE;
	}
	extCodecHandle = -1;

	return 0;
}

int rkipc_aenc_init() {
	AENC_CHN_ATTR_S stAencAttr;
	const char *encode_type = rk_param_get_string("audio.0:encode_type", NULL);

	memset(&stAencAttr, 0, sizeof(AENC_CHN_ATTR_S));
	if (!strcmp(encode_type, "MP2")) {
		stAencAttr.enType = RK_AUDIO_ID_MP2;
		stAencAttr.stCodecAttr.enType = RK_AUDIO_ID_MP2;
	} else if (!strcmp(encode_type, "G711A")) {
		stAencAttr.enType = RK_AUDIO_ID_PCM_ALAW;
		stAencAttr.stCodecAttr.enType = RK_AUDIO_ID_PCM_ALAW;
	} else if (!strcmp(encode_type, "MP3")) {
		stAencAttr.enType = RK_AUDIO_ID_MP3;
		stAencAttr.stCodecAttr.enType = RK_AUDIO_ID_MP3;
		stAencAttr.stCodecAttr.u32Resv[0] = 1152;
		stAencAttr.stCodecAttr.u32Resv[1] = 160000;
		register_aenc_mp3();
	} else {
		LOG_ERROR("not support %s\n", encode_type);
	}
	stAencAttr.stCodecAttr.u32Channels = rk_param_get_int("audio.0:channels", 2);
	stAencAttr.stCodecAttr.u32SampleRate = rk_param_get_int("audio.0:sample_rate", 16000);
	const char *format = rk_param_get_string("audio.0:format", NULL);
	if (!strcmp(format, "S16")) {
		stAencAttr.stCodecAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	} else if (!strcmp(format, "U8")) {
		stAencAttr.stCodecAttr.enBitwidth = AUDIO_BIT_WIDTH_8;
	} else {
		LOG_ERROR("not support %s\n", format);
	}
	stAencAttr.u32BufCount = 4;

	int ret = RK_MPI_AENC_CreateChn(aenc_chn_id, &stAencAttr);
	if (ret) {
		LOG_ERROR("create aenc chn %d err:0x%x\n", aenc_chn_id, ret);
		return RK_FAILURE;
	}
	LOG_DEBUG("create aenc chn %d success\n", aenc_chn_id);

	pthread_create(&save_aenc_tid, RK_NULL, save_aenc_thread, NULL);

	return 0;
}

int rkipc_aenc_deinit() {
	int ret = RK_MPI_AENC_DestroyChn(aenc_chn_id);
	if (ret)
		LOG_ERROR("RK_MPI_AI_DisableChn fail\n");
	LOG_DEBUG("RK_MPI_AI_DisableChn success\n");

	return 0;
}

int rkipc_ao_init() {
	int ret;
	AIO_ATTR_S aoAttr;
	memset(&aoAttr, 0, sizeof(AIO_ATTR_S));

	const char *card_name = rk_param_get_string("audio.0:card_name", "default");
	snprintf(aoAttr.u8CardName, sizeof(aoAttr.u8CardName), "%s", card_name);
	LOG_INFO("aoAttr.u8CardName is %s\n", aoAttr.u8CardName);

	aoAttr.soundCard.channels = 2;
	aoAttr.soundCard.sampleRate = rk_param_get_int("audio.0:sample_rate", 8000);

	const char *format = rk_param_get_string("audio.0:format", NULL);
	if (!strcmp(format, "S16")) {
		aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;
		aoAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	} else if (!strcmp(format, "U8")) {
		aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_8;
		aoAttr.enBitwidth = AUDIO_BIT_WIDTH_8;
	} else {
		LOG_ERROR("not support %s\n", format);
	}

	aoAttr.enSamplerate = rk_param_get_int("audio.0:sample_rate", 16000);
	if (rk_param_get_int("audio.0:channels", 2) == 2)
		aoAttr.enSoundmode = AUDIO_SOUND_MODE_STEREO;
	else
		aoAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aoAttr.u32FrmNum = 4;
	aoAttr.u32PtNumPerFrm = rk_param_get_int("audio.0:frame_size", 1024);
	aoAttr.u32EXFlag = 0;
	aoAttr.u32ChnCnt = 2;

	ret = RK_MPI_AO_SetPubAttr(0, &aoAttr);
	if (ret)
		LOG_ERROR("RK_MPI_AO_SetPubAttr fail %#x\n", ret);
	ret = RK_MPI_AO_Enable(0);
	if (ret)
		LOG_ERROR("RK_MPI_AO_Enable fail %#x\n", ret);
	ret = RK_MPI_AO_EnableChn(0, 0);
	if (ret)
		LOG_ERROR("RK_MPI_AO_EnableChn fail %#x\n", ret);
	ret = RK_MPI_AO_SetTrackMode(0, AUDIO_TRACK_OUT_STEREO);
	if (ret)
		LOG_ERROR("RK_MPI_AO_EnableChn fail %#x\n", ret);

	return 0;
}

int rkipc_ao_deinit() {
	int ret;
	ret = RK_MPI_AO_DisableChn(0, 0);
	if (ret)
		LOG_ERROR("RK_MPI_AO_DisableChn fail %#x\n", ret);
	ret = RK_MPI_AO_Disable(0);
	if (ret)
		LOG_ERROR("RK_MPI_AO_Disable fail %#x\n", ret);

	return 0;
}

int rkipc_ao_write(unsigned char *data, int data_len) {
	int ret;
	AUDIO_FRAME_S frame;
	MB_EXT_CONFIG_S extConfig;
	memset(&frame, 0, sizeof(frame));
	frame.u32Len = data_len;
	frame.u64TimeStamp = 0;
	frame.enBitWidth = AUDIO_BIT_WIDTH_16;
	frame.enSoundMode = AUDIO_SOUND_MODE_STEREO;
	frame.bBypassMbBlk = RK_FALSE;

	memset(&extConfig, 0, sizeof(extConfig));
	extConfig.pOpaque = data;
	extConfig.pu8VirAddr = data;
	extConfig.u64Size = data_len;
	RK_MPI_SYS_CreateMB(&(frame.pMbBlk), &extConfig);

	ret = RK_MPI_AO_SendFrame(0, 0, &frame, 1000);
	if (ret)
		LOG_ERROR("send frame fail, result = %#x\n", ret);
	RK_MPI_MB_ReleaseMB(frame.pMbBlk);

	if (data_len <= 0) {
		LOG_INFO("eof\n");
		RK_MPI_AO_WaitEos(0, 0, -1);
	}

	return 0;
}

int rkipc_audio_init() {
	LOG_DEBUG("%s\n", __func__);
	int ret = rkipc_ai_init();
	ret |= rkipc_aenc_init();

	// bind ai to aenc
	ai_chn.enModId = RK_ID_AI;
	ai_chn.s32DevId = ai_dev_id;
	ai_chn.s32ChnId = ai_chn_id;

	aenc_chn.enModId = RK_ID_AENC;
	aenc_chn.s32DevId = aenc_dev_id;
	aenc_chn.s32ChnId = aenc_chn_id;

	ret |= RK_MPI_SYS_Bind(&ai_chn, &aenc_chn);
	if (ret != 0) {
		LOG_ERROR("RK_MPI_SYS_Bind fail %x\n", ret);
	}
	LOG_DEBUG("RK_MPI_SYS_Bind success\n");

	return ret;
}

int rkipc_audio_deinit() {
	LOG_DEBUG("%s\n", __func__);
	int ret;
	g_audio_run_ = 0;
	if (enable_aed || enable_bcd)
		pthread_join(ai_get_detect_result_tid, NULL);
	// if (enable_aed)
	// 	rkipc_audio_aed_deinit();
	// if (enable_bcd)
	// 	rkipc_audio_bcd_deinit();
	// if (enable_vqe)
	// 	rkipc_audio_vqe_deinit();
	pthread_join(save_aenc_tid, RK_NULL);
	ret = RK_MPI_SYS_UnBind(&ai_chn, &aenc_chn);
	if (ret != 0) {
		LOG_ERROR("RK_MPI_SYS_UnBind fail %x\n", ret);
	}
	LOG_DEBUG("RK_MPI_SYS_UnBind success\n");
	ret |= rkipc_aenc_deinit();
	ret |= rkipc_ai_deinit();
	const char *encode_type = rk_param_get_string("audio.0:encode_type", NULL);
	if (!strcmp(encode_type, "MP3"))
		unregister_aenc_mp3();

	return ret;
}

// export api

int rk_audio_restart() {
	int ret;
	ret |= rkipc_audio_deinit();
	ret |= rkipc_audio_init();

	return ret;
}

int rk_audio_get_bit_rate(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:bit_rate", stream_id);
	*value = rk_param_get_int(entry, 16000);

	return 0;
}

int rk_audio_set_bit_rate(int stream_id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:bit_rate", stream_id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_audio_get_sample_rate(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:sample_rate", stream_id);
	*value = rk_param_get_int(entry, 8000);

	return 0;
}

int rk_audio_set_sample_rate(int stream_id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:sample_rate", stream_id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_audio_get_volume(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:volume", stream_id);
	*value = rk_param_get_int(entry, 50);

	return 0;
}

int rk_audio_set_volume(int stream_id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:volume", stream_id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_audio_get_enable_vqe(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:enable_vqe", stream_id);
	*value = rk_param_get_int(entry, 1);

	return 0;
}

int rk_audio_set_enable_vqe(int stream_id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:enable_vqe", stream_id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_audio_get_encode_type(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:encode_type", stream_id);
	*value = rk_param_get_string(entry, "G711A");

	return 0;
}

int rk_audio_set_encode_type(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "audio.%d:encode_type", stream_id);
	rk_param_set_string(entry, value);

	return 0;
}
