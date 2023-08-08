// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "log.h"

#include "rkmedia_aenc.h"
#include "rkmedia_ai.h"
#include "rkmedia_api.h"
#include "rkmedia_common.h"
#include "storage.h"

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

/*
void *save_ai_thread(void *ptr) {
        RK_S32 ret = 0;
        RK_S32 s32MilliSec = -1;
        AUDIO_FRAME_S frame;
        MEDIA_BUFFER mb = NULL;

        AUDIO_SAVE_FILE_INFO_S save;
        save.bCfg = RK_TRUE;
        save.u32FileSize = 1024;
        snprintf(save.aFilePath, sizeof(save.aFilePath), "%s", "/tmp/");
        snprintf(save.aFileName, sizeof(save.aFileName), "%s", "cap_out.pcm");
        RK_MPI_AI_SaveFile(ai_dev_id, ai_chn_id, &save);

        while (g_audio_run_) {
                mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_AI, 0, -1);
                if (!mb) {
                        printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
                        break;
                }
                else {
                        void *data = RK_MPI_MB_GetPtr(mb);
                        RK_U32 len = RK_MPI_MB_GetSize(mb);
                        LOG_INFO("data = %p, len = %d\n", data, len);
                        RK_MPI_MB_ReleaseBuffer(mb);
                }
        }

        return RK_NULL;
}*/

void *save_aenc_thread(void *ptr) {
	RK_S32 s32ret = 0;
	FILE *file = NULL;
	RK_S32 eos = 0;
	RK_S32 count = 0;
	MEDIA_BUFFER mb = NULL;
	const char *code_type;
	code_type = rk_param_get_string("audio.0:encode_type", NULL);

	// file = fopen("/tmp/aenc.mp2", "wb+");
	// if (!file) {
	// 	LOG_ERROR("failed to open /tmp/aenc.mp2, error: %s\n", strerror(errno));
	// 	// return -1;
	// }

	while (g_audio_run_) {
		mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_AENC, aenc_chn_id, 1000);
		if (!mb) {
			printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
			break;
		} else {
			// LOG_INFO("----AENC2 PTS:%lld\n",RK_MPI_MB_GetTimestamp(mb));
			eos = (RK_MPI_MB_GetSize(mb) <= 0) ? 1 : 0;
			rk_storage_write_audio_frame(0, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
			                             RK_MPI_MB_GetTimestamp(mb));
			rk_storage_write_audio_frame(1, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
			                             RK_MPI_MB_GetTimestamp(mb));
			rk_storage_write_audio_frame(2, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
			                             RK_MPI_MB_GetTimestamp(mb));
			// if (file && !strcmp(code_type, "MP2")) {
			//     // fwrite(buffer, RK_MPI_MB_GetSize(mb), 1, file);
			//     fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), file);
			//     fflush(file);
			//     fclose(file);
			//     file = NULL;
			// }
			RK_MPI_MB_ReleaseBuffer(mb);
			count++;
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
	AI_CHN_ATTR_S ai_attr;
	RK_U32 u32FrameCnt = rk_param_get_int("audio.0:frame_size", 1024);
	RK_U32 u32ChnCnt = rk_param_get_int("audio.0:channels", 2);
	RK_U32 u32SampleRate = rk_param_get_int("audio.0:sample_rate", 16000);

	const char *card_name = rk_param_get_string("audio.0:card_name", "default");
	// snprintf(ai_attr.pcAudioNode, 8, "%s", card_name);
	// LOG_INFO("ai_attr.pcAudioNode is %s\n", ai_attr.pcAudioNode);
	const char *format = rk_param_get_string("audio.0:format", NULL);
	if (!strcmp(format, "S16")) {
		ai_attr.enSampleFormat = RK_SAMPLE_FMT_S16;
	} else if (!strcmp(format, "U8")) {
		ai_attr.enSampleFormat = RK_SAMPLE_FMT_U8;
	} else {
		LOG_ERROR("not support %s\n", format);
	}

	ai_attr.pcAudioNode = card_name;
	// ai_attr.enSampleFormat = format;
	ai_attr.u32NbSamples = u32FrameCnt;
	ai_attr.u32SampleRate = u32SampleRate;
	ai_attr.u32Channels = u32ChnCnt;
	ai_attr.enAiLayout = AI_LAYOUT_NORMAL;
	ret = RK_MPI_AI_SetChnAttr(ai_chn_id, &ai_attr);
	ret |= RK_MPI_AI_EnableChn(ai_chn_id);
	if (ret) {
		printf("Create AI[0] failed! ret=%d\n", ret);
		return -1;
	}

	return 0;
}

int rk_ai_deinit() {
	// pthread_join(save_ai_tid, RK_NULL);
	// RK_MPI_AI_DisableReSmp(ai_dev_id, ai_chn_id);
	int ret = RK_MPI_AI_DisableChn(ai_chn_id);
	if (ret != 0) {
		LOG_ERROR("ai disable channel fail, reason = %d\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("RK_MPI_AI_DisableChn success\n");
}

int rk_aenc_init() {
	AENC_CHN_ATTR_S stAencAttr;
	const char *encode_type = rk_param_get_string("audio.0:encode_type", NULL);
	if (!strcmp(encode_type, "MP2")) {
		stAencAttr.enCodecType = RK_CODEC_TYPE_MP2;
	} else {
		LOG_ERROR("not support %s\n", encode_type);
	}
	stAencAttr.u32Quality = 1;
	stAencAttr.u32Bitrate = rk_param_get_int("audio.0:bit_rate", 32000);
	stAencAttr.stAencMP2.u32Channels = rk_param_get_int("audio.0:channels", 2);
	stAencAttr.stAencMP2.u32SampleRate = rk_param_get_int("audio.0:sample_rate", 16000);

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

int rkipc_audio_init() {
	LOG_INFO("%s\n", __func__);
	RK_MPI_SYS_Init();
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
	ret |= rk_aenc_deinit();
	ret |= rk_ai_deinit();

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
