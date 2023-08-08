// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "log.h"
#include "tuya_ipc.h"

#include <rk_debug.h>
#include <rk_mpi_aenc.h>
#include <rk_mpi_ai.h>
#include <rk_mpi_ao.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_sys.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "audio.c"

pthread_t save_ai_tid;
pthread_t save_aenc_tid;
static int ai_dev_id = 0;
static int ai_chn_id = 0;
static int aenc_dev_id = 0;
static int aenc_chn_id = 0;
static int g_audio_run_ = 1;
MPP_CHN_S ai_chn, aenc_chn;

void *save_ai_thread(void *ptr) {
	RK_S32 ret = 0;
	RK_S32 s32MilliSec = -1;
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
			LOG_INFO("data = %p, len = %d\n", data, frame.u32Len);
			RK_MPI_AI_ReleaseFrame(ai_dev_id, ai_chn_id, &frame, RK_NULL);
		}
	}

	return RK_NULL;
}

static RK_S64 fake_time = 0;
void *save_aenc_thread(void *ptr) {
	RK_S32 s32ret = 0;
	FILE *file = RK_NULL;
	AUDIO_STREAM_S pstStream;
	RK_S32 eos = 0;
	RK_S32 count = 0;

	// file = fopen("/tmp/aenc.mp2", "wb+");
	// if (file == RK_NULL) {
	// 	RK_LOGE("failed to open /tmp/aenc.mp2, error: %s\n", strerror(errno));
	// 	return RK_NULL;
	// }

	while (g_audio_run_) {
		s32ret = RK_MPI_AENC_GetStream(aenc_chn_id, &pstStream, 300);
		if (s32ret == RK_SUCCESS) {
			MB_BLK bBlk = pstStream.pMbBlk;
			void *buffer = RK_MPI_MB_Handle2VirAddr(bBlk);
			eos = (pstStream.u32Len <= 0) ? 1 : 0;
			if (buffer) {
				// LOG_INFO("get frame data = %p, size = %d\n", buffer, pstStream.u32Len);
				// rkmuxer_write_audio_frame(buffer, pstStream.u32Len, pstStream.u64TimeStamp);
				// fake 64ms
				fake_time += 64000;
				rk_tuya_push_audio(buffer, pstStream.u32Len, fake_time);
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

int rk_ai_init() {
	int ret;
	AUDIO_DEV aiDevId = ai_dev_id;
	AIO_ATTR_S aiAttr;
	memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

	const char *card_name = rk_param_get_string("audio.0:card_name", "default");
	snprintf(aiAttr.u8CardName, sizeof(aiAttr.u8CardName), "%s", card_name);
	LOG_INFO("aiAttr.u8CardName is %s\n", aiAttr.u8CardName);
	aiAttr.soundCard.channels = rk_param_get_int("audio.0:channels", 2);
	aiAttr.soundCard.sampleRate = rk_param_get_int("audio.0:sample_rate", 8000);
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
	aiAttr.enSamplerate = rk_param_get_int("audio.0:sample_rate", 8000);
	if (aiAttr.soundCard.channels == 2)
		aiAttr.enSoundmode = AUDIO_SOUND_MODE_STEREO;
	else
		aiAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aiAttr.u32FrmNum = 4;
	aiAttr.u32PtNumPerFrm = rk_param_get_int("audio.0:frame_size", 1024);
	aiAttr.u32EXFlag = 0;
	aiAttr.u32ChnCnt = rk_param_get_int("audio.0:channels", 2);

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

	// pthread_create(&save_ai_tid, RK_NULL, save_ai_thread, NULL);

	return 0;
}

int rk_ai_deinit() {
	// pthread_join(save_ai_tid, RK_NULL);
	// RK_MPI_AI_DisableReSmp(ai_dev_id, ai_chn_id);
	int ret = RK_MPI_AI_DisableChn(ai_dev_id, ai_chn_id);
	if (ret != 0) {
		LOG_ERROR("ai disable channel fail, reason = %d\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("RK_MPI_AI_DisableChn success\n");

	ret = RK_MPI_AI_Disable(ai_dev_id);
	if (ret != 0) {
		LOG_ERROR("ai disable fail, reason = %d\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("RK_MPI_AI_Disable success\n");

	return 0;
}

int rk_aenc_init() {
	AENC_CHN_ATTR_S stAencAttr;
	memset(&stAencAttr, 0, sizeof(stAencAttr));
	const char *encode_type = rk_param_get_string("audio.0:encode_type", NULL);
	if (!strcmp(encode_type, "MP2")) {
		stAencAttr.enType = RK_AUDIO_ID_MP2;
	} else {
		LOG_ERROR("not support %s\n", encode_type);
	}
	stAencAttr.stAencCodec.u32Channels = rk_param_get_int("audio.0:channels", 2);
	stAencAttr.stAencCodec.u32SampleRate = rk_param_get_int("audio.0:sample_rate", 16000);
	const char *format = rk_param_get_string("audio.0:format", NULL);
	if (!strcmp(format, "S16")) {
		stAencAttr.stAencCodec.enBitwidth = AUDIO_BIT_WIDTH_16;
	} else if (!strcmp(format, "U8")) {
		stAencAttr.stAencCodec.enBitwidth = AUDIO_BIT_WIDTH_8;
	} else {
		LOG_ERROR("not support %s\n", format);
	}
	stAencAttr.u32BufCount = 4;

	int ret = RK_MPI_AENC_CreateChn(aenc_chn_id, &stAencAttr);
	if (ret) {
		LOG_ERROR("create aenc chn %d err:0x%x\n", aenc_chn_id, ret);
		return RK_FAILURE;
	}
	LOG_INFO("create aenc chn %d success\n", aenc_chn_id);

	pthread_create(&save_aenc_tid, RK_NULL, save_aenc_thread, NULL);

	return 0;
}

int rk_aenc_deinit() {
	pthread_join(save_aenc_tid, RK_NULL);
	int ret = RK_MPI_AENC_DestroyChn(aenc_chn_id);
	if (ret)
		LOG_ERROR("RK_MPI_AI_DisableChn fail\n");
	LOG_INFO("RK_MPI_AI_DisableChn success\n");

	return 0;
}

int rkipc_ao_init() {
	int ret;
	AIO_ATTR_S aoAttr;
	memset(&aoAttr, 0, sizeof(AIO_ATTR_S));

	const char *card_name = rk_param_get_string("audio.0:card_name", "default");
	snprintf(aoAttr.u8CardName, sizeof(aoAttr.u8CardName), "%s", card_name);
	LOG_INFO("aoAttr.u8CardName is %s\n", aoAttr.u8CardName);

	aoAttr.soundCard.channels = 1;
	aoAttr.soundCard.sampleRate = rk_param_get_int("audio.0:sample_rate", 8000);
	aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

	aoAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	if (aoAttr.soundCard.sampleRate == 8000)
		aoAttr.enSamplerate = AUDIO_SAMPLE_RATE_8000;
	else
		aoAttr.enSamplerate = AUDIO_SAMPLE_RATE_16000;
	aoAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aoAttr.u32FrmNum = 4;
	aoAttr.u32PtNumPerFrm = 1024;

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
	if (data_len <= 0) {
		LOG_INFO("eof");
		return 0;
	}

	memset(&frame, 0, sizeof(frame));
	frame.u32Len = data_len;
	frame.u64TimeStamp = 0;
	frame.enBitWidth = AUDIO_BIT_WIDTH_16;
	frame.enSoundMode = AUDIO_SOUND_MODE_MONO;
	frame.bBypassMbBlk = RK_FALSE;

	memset(&extConfig, 0, sizeof(extConfig));
	extConfig.pu8VirAddr = data;
	extConfig.u64Size = data_len;
	RK_MPI_SYS_CreateMB(&(frame.pMbBlk), &extConfig);

	ret = RK_MPI_AO_SendFrame(0, 0, &frame, 300);
	if (ret < 0)
		LOG_ERROR("send frame fail, result = %#x\n", ret);
	RK_MPI_MB_ReleaseMB(frame.pMbBlk);

	return 0;
}

int rk_audio_init() {
	LOG_INFO("%s\n", __func__);
	int ret = rk_ai_init();
	ret |= rk_aenc_init();
	// bind ai to aenc
	ai_chn.enModId = RK_ID_AI;
	ai_chn.s32DevId = ai_dev_id;
	ai_chn.s32ChnId = ai_chn_id;

	aenc_chn.enModId = RK_ID_AENC;
	aenc_chn.s32DevId = aenc_dev_id;
	aenc_chn.s32ChnId = aenc_chn_id;

	ret |= RK_MPI_SYS_Bind(&ai_chn, &aenc_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_SYS_Bind fail %x\n", ret);
	}
	LOG_INFO("RK_MPI_SYS_Bind success\n");
	rk_tuya_ao_create_register(rkipc_ao_init);
	rk_tuya_ao_write_register(rkipc_ao_write);
	rk_tuya_ao_destroy_register(rkipc_ao_deinit);

	return ret;
}

int rk_audio_deinit() {
	LOG_INFO("%s\n", __func__);
	int ret;
	g_audio_run_ = 0;
	ret = RK_MPI_SYS_UnBind(&ai_chn, &aenc_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_SYS_UnBind fail %x\n", ret);
	}
	LOG_INFO("RK_MPI_SYS_UnBind success\n");
	ret |= rk_aenc_deinit();
	ret |= rk_ai_deinit();
	rk_tuya_ao_create_register(NULL);
	rk_tuya_ao_write_register(NULL);
	rk_tuya_ao_destroy_register(NULL);

	return ret;
}
