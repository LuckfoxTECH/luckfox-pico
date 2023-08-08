// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "log.h"
#include "storage.h"

#include <rk_debug.h>
#include <rk_mpi_aenc.h>
#include <rk_mpi_ai.h>
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
		s32ret = RK_MPI_AENC_GetStream(aenc_chn_id, &pstStream, 1000);
		if (s32ret == RK_SUCCESS) {
			MB_BLK bBlk = pstStream.pMbBlk;
			void *buffer = RK_MPI_MB_Handle2VirAddr(bBlk);
			eos = (pstStream.u32Len <= 0) ? 1 : 0;
			if (buffer) {
				// LOG_INFO("get frame data = %p, size = %d, pts is %lld\n", buffer,
				// pstStream.u32Len, pstStream.u64TimeStamp);
				// fake 72ms
				fake_time += 72000;
				// LOG_INFO("fake pts is %lld\n", fake_time);
				rk_storage_write_audio_frame(0, buffer, pstStream.u32Len, fake_time);
				rk_storage_write_audio_frame(1, buffer, pstStream.u32Len, fake_time);
				rk_storage_write_audio_frame(2, buffer, pstStream.u32Len, fake_time);
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

int rkipc_ai_init() {
	int ret;
	AUDIO_DEV aiDevId = ai_dev_id;
	AIO_ATTR_S aiAttr;
	memset(&aiAttr, 0, sizeof(AIO_ATTR_S));

	const char *card_name = rk_param_get_string("audio.0:card_name", "default");
	snprintf(aiAttr.u8CardName, sizeof(aiAttr.u8CardName), "%s", card_name);
	LOG_INFO("aiAttr.u8CardName is %s\n", aiAttr.u8CardName);
	aiAttr.soundCard.channels = rk_param_get_int("audio.0:channels", 2);
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

int rkipc_ai_deinit() {
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

int rkipc_aenc_init() {
	AENC_CHN_ATTR_S stAencAttr;
	const char *encode_type = rk_param_get_string("audio.0:encode_type", NULL);
	if (!strcmp(encode_type, "MP2")) {
		stAencAttr.enType = RK_AUDIO_ID_MP2;
		stAencAttr.stCodecAttr.enType = RK_AUDIO_ID_MP2;
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
	LOG_INFO("create aenc chn %d success\n", aenc_chn_id);

	pthread_create(&save_aenc_tid, RK_NULL, save_aenc_thread, NULL);

	return 0;
}

int rkipc_aenc_deinit() {
	pthread_join(save_aenc_tid, RK_NULL);
	int ret = RK_MPI_AENC_DestroyChn(aenc_chn_id);
	if (ret)
		LOG_ERROR("RK_MPI_AI_DisableChn fail\n");
	LOG_INFO("RK_MPI_AI_DisableChn success\n");

	return 0;
}

int rkipc_audio_init() {
	LOG_INFO("%s\n", __func__);
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
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_SYS_Bind fail %x\n", ret);
	}
	LOG_INFO("RK_MPI_SYS_Bind success\n");

	return ret;
}

int rkipc_audio_deinit() {
	LOG_INFO("%s\n", __func__);
	int ret;
	g_audio_run_ = 0;
	ret = RK_MPI_SYS_UnBind(&ai_chn, &aenc_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_SYS_UnBind fail %x\n", ret);
	}
	LOG_INFO("RK_MPI_SYS_UnBind success\n");
	ret |= rkipc_aenc_deinit();
	ret |= rkipc_ai_deinit();

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