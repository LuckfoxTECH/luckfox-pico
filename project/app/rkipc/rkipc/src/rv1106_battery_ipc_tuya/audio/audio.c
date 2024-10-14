// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "log.h"
#include "rtsp.h"
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
			LOG_DEBUG("data = %p, len = %d\n", data, frame.u32Len);
			RK_MPI_AI_ReleaseFrame(ai_dev_id, ai_chn_id, &frame, RK_NULL);
		}
	}

	return RK_NULL;
}
static RK_S64 fake_time = 0;
void *save_aenc_thread(void *ptr) {
	prctl(PR_SET_NAME, "save_aenc_thread", 0, 0, 0);
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
				// LOG_INFO("get frame data = %p, size = %d, pts is %lld, seq is %d\n", buffer,
				//          pstStream.u32Len, pstStream.u64TimeStamp, pstStream.u32Seq);
				rkipc_rtsp_write_audio_frame(0, buffer, pstStream.u32Len, pstStream.u64TimeStamp);
				if (rk_param_get_int("tuya:enable", 0))
					rk_tuya_push_audio(buffer, pstStream.u32Len, pstStream.u64TimeStamp);
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
	if (result != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_AI_SetAedAttr(%d,%d) failed with %#x\n", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_SetAedAttr(%d,%d) success\n", ai_dev_id, ai_chn_id);
	result = RK_MPI_AI_EnableAed(ai_dev_id, ai_chn_id);
	if (result != RK_SUCCESS) {
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
	if (result != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_AI_SetBcdAttr(%d,%d) failed with %#x\n", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_SetBcdAttr(%d,%d) success\n", ai_dev_id, ai_chn_id);
	result = RK_MPI_AI_EnableBcd(ai_dev_id, ai_chn_id);
	if (result != RK_SUCCESS) {
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
		RK_LOGE("Invalid gap: %d, just supports 16ms or 10ms for AI VQE", vqe_gap_ms);
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
	if (result != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_AI_SetVqeAttr(%d,%d) failed with %#x", ai_dev_id, ai_chn_id, result);
		return result;
	}
	LOG_DEBUG("RK_MPI_AI_SetVqeAttr(%d,%d) success\n", ai_dev_id, ai_chn_id);
	result = RK_MPI_AI_EnableVqe(ai_dev_id, ai_chn_id);
	if (result != RK_SUCCESS) {
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
				RK_LOGD("aed_result: %d, %d", aed_result.bAcousticEventDetected,
				        aed_result.bLoudSoundDetected);
			}
		}
		if (enable_bcd) {
			result = RK_MPI_AI_GetBcdResult(ai_dev_id, ai_chn_id, &bcd_result);
			if (result == 0) {
				RK_LOGD("bcd_result: %d", bcd_result.bBabyCry);
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

	// RK_MPI_AI_SetVolume(ai_dev_id, rk_param_get_int("audio.0:volume", 50));
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

int rkipc_aenc_init() {
	AENC_CHN_ATTR_S stAencAttr;
	const char *encode_type = rk_param_get_string("audio.0:encode_type", NULL);
	if (!strcmp(encode_type, "MP2")) {
		stAencAttr.enType = RK_AUDIO_ID_MP2;
		stAencAttr.stCodecAttr.enType = RK_AUDIO_ID_MP2;
	} else if (!strcmp(encode_type, "G711A")) {
		stAencAttr.enType = RK_AUDIO_ID_PCM_ALAW;
		stAencAttr.stCodecAttr.enType = RK_AUDIO_ID_PCM_ALAW;
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

	aoAttr.soundCard.channels = 1;
	aoAttr.soundCard.sampleRate = rk_param_get_int("audio.0:sample_rate", 8000);
	aoAttr.soundCard.bitWidth = AUDIO_BIT_WIDTH_16;

	aoAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	aoAttr.enSamplerate = AUDIO_SAMPLE_RATE_8000; // tuya received must 8k
	aoAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	aoAttr.u32FrmNum = 4;
	aoAttr.u32PtNumPerFrm = 1024;
	aoAttr.u32EXFlag = 0;
	aoAttr.u32ChnCnt = 1;

	ret = RK_MPI_AO_SetPubAttr(0, &aoAttr);
	if (ret)
		LOG_ERROR("RK_MPI_AO_SetPubAttr fail %#x\n", ret);
	LOG_INFO("RK_MPI_AO_SetPubAttr success\n");
	ret = RK_MPI_AO_Enable(0);
	if (ret)
		LOG_ERROR("RK_MPI_AO_Enable fail %#x\n", ret);
	LOG_INFO("RK_MPI_AO_Enable success\n");
	ret = RK_MPI_AO_EnableChn(0, 0);
	if (ret)
		LOG_ERROR("RK_MPI_AO_EnableChn fail %#x\n", ret);
	LOG_INFO("RK_MPI_AO_EnableChn success\n");

	ret = RK_MPI_AO_SetTrackMode(0, AUDIO_TRACK_OUT_STEREO);
	if (ret)
		LOG_ERROR("RK_MPI_AO_EnableChn fail %#x\n", ret);
	LOG_INFO("RK_MPI_AO_EnableChn success\n");

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
	extConfig.pOpaque = data;
	extConfig.pu8VirAddr = data;
	extConfig.u64Size = data_len;
	RK_MPI_SYS_CreateMB(&(frame.pMbBlk), &extConfig);

	ret = RK_MPI_AO_SendFrame(0, 0, &frame, 1000);
	if (ret)
		LOG_ERROR("send frame fail, result = %#x\n", ret);
	RK_MPI_MB_ReleaseMB(frame.pMbBlk);

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
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_SYS_Bind fail %x\n", ret);
	}
	LOG_DEBUG("RK_MPI_SYS_Bind success\n");
	if (rk_param_get_int("tuya:enable", 0)) {
		rk_tuya_ao_create_register(rkipc_ao_init);
		rk_tuya_ao_write_register(rkipc_ao_write);
		rk_tuya_ao_destroy_register(rkipc_ao_deinit);
	}

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
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_SYS_UnBind fail %x\n", ret);
	}
	LOG_DEBUG("RK_MPI_SYS_UnBind success\n");
	ret |= rkipc_aenc_deinit();
	ret |= rkipc_ai_deinit();

	return ret;
}
