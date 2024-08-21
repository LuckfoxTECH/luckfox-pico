/*
 * Copyright 2023 Rockchip Electronics Co. LTD
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
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>

#include "rtsp_demo.h"
#include "sample_comm.h"

#define VI_NUM_MAX 8
#define VENC_NUM_MAX 2
#define BUFF_SIZE 255

typedef struct _rkModeTest {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfVencThreadQuit[VENC_NUM_MAX];
	RK_BOOL bIfModuleTestThreadQuit;
	RK_BOOL bIfModuleTestopen;
	RK_BOOL bIfEnableRtsp;
	RK_BOOL bIfIspGroupInit;
	RK_BOOL bIfOsdDisplay;
	RK_S32 s32ModuleTestType;
	RK_S32 s32CamGrpId;
	RK_S32 s32CamNum;
	RK_S32 s32IspFps;
	RK_U32 u32ModuleTestLoop;
	RK_U32 u32TestFrameCount;
	RK_U32 u32VencGetFrameCount[VENC_NUM_MAX];
	GET_LDCH_MODE_E eGetLdchMode; /*0: no ldch, 1: get ldch mesh for file, 2: get ldch
	                                 mesh by buffer */
	CODEC_TYPE_E enCodecType;
	rk_aiq_working_mode_t hdr_mode;
#ifdef RKAIQ_GRP
	rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
#endif
	RK_VOID *pLdchMeshData[VI_NUM_MAX];
} g_mode_test;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[VI_NUM_MAX];
	SAMPLE_AVS_CTX_S avs;
	SAMPLE_VENC_CTX_S venc[VENC_NUM_MAX];
	SAMPLE_RGN_CTX_S rgn;
} SAMPLE_MPI_CTX_S;

static RK_S32 g_exit_result = RK_SUCCESS;
static rtsp_demo_handle g_rtsplive = RK_NULL;
static rtsp_session_handle g_rtsp_session[VENC_NUM_MAX] = {RK_NULL};
static pthread_mutex_t g_rtsp_mutex = {0};
static pthread_mutex_t g_frame_count_mutex[VENC_NUM_MAX] = {0};
static sem_t g_sem_module_test[VENC_NUM_MAX];
static g_mode_test *gModeTest = RK_NULL;
static SAMPLE_MPI_CTX_S *ctx = RK_NULL;

static void program_handle_error(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> error exit!", func, line);
	g_exit_result = RK_FAILURE;
	gModeTest->bIfMainThreadQuit = RK_TRUE;
}

static void program_normal_exit(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> normal exit!", func, line);
	gModeTest->bIfMainThreadQuit = RK_TRUE;
}

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	program_normal_exit(__func__, __LINE__);
}

static RK_CHAR optstr[] = "?::a::A:n:b:l:o:e:F:h:m:d:L:v:s:e:t:f:c:i:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"calib_file_path", required_argument, NULL, 'A'},
    {"camera_num", required_argument, NULL, 'n'},
    {"bitrate", required_argument, NULL, 'b'},
    {"vi_size", required_argument, NULL, 'v' + 's'},
    {"avs_chn0_size", required_argument, NULL, 'a' + 's'},
    {"avs_chn1_size", required_argument, NULL, 'a' + 'v'},
    {"loop_count", required_argument, NULL, 'l'},
    {"output_path", required_argument, NULL, 'o'},
    {"encode", required_argument, NULL, 'e'},
    {"dstfps", required_argument, NULL, 'F'},
    {"hdr_mode", required_argument, NULL, 'h' + 'm'},
    {"stitch_distance", required_argument, NULL, 'd'},
    {"cam0_ldch_path", required_argument, NULL, 'L'},
    {"cam1_ldch_path", required_argument, NULL, 'L' + 'm'},
    {"set_ldch", required_argument, NULL, 'l' + 'd'},
    {"test_type", required_argument, NULL, 't' + 't'},
    {"test_loop", required_argument, NULL, 't' + 'l'},
    {"test_frame", required_argument, NULL, 't' + 'f'},
    {"chn_id", required_argument, NULL, 'c' + 'i'},
    {"vi_chn_buf_cnt", required_argument, NULL, 'v' + 'b'},
    {"ispLaunchMode", required_argument, NULL, 'i'},
    {"input_bmp_path", required_argument, NULL, 'i' + 'b'},
    {"osd_display", required_argument, RK_NULL, 'o' + 'd'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s --vi_size 1920x1080 --avs_chn0_size 3840x1080 --avs_chn1_size 1920x544 "
	       "-a /etc/iqfiles/ -e h265cbr -b "
	       "4096 -n 2\n",
	       name);
	printf("\trtsp://xx.xx.xx.xx/live/0, Default OPEN\n");
#ifdef RKAIQ
	printf("\t-a | --aiq : enable aiq with dirpath provided, eg:-a "
	       "/etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
#endif
	printf("\t-A | --calib_file_path : input file path of xxx.xml "
	       "Default /oem/usr/share/avs_calib/calib_file.xml\n");
	printf("\t-n | --camera_num : camera number, Default 2\n");
	printf("\t-b | --bitrate : encode bitrate, Default 4096\n");
	printf("\t-l | --loop_count : loop count, Default -1\n");
	printf("\t-o | --output_path : encode output file path, Default NULL\n");
	printf("\t-e | --encode : encode type, Default:h265cbr, Value:h264cbr, "
	       "h264vbr, h265cbr, h265vbr\n");
	printf("\t-F | --dstfps : set venc output fps, Default: 15\n");
	printf("\t-i | --ispLaunchMode : 0: single cam init, 1: camera group init. default: "
	       "1\n");
	printf("\t--vi_size : set vi resolution WidthxHeight, default: 1920x1080\n");
	printf(
	    "\t--avs_chn0_size : set avs chn0 resolution WidthxHeight, default: 3840x1080\n");
	printf(
	    "\t--avs_chn1_size : set avs chn1 resolution WidthxHeight, default: 1920x544\n");
	printf("\t--hdr_mode : set hdr mode, 0: normal 1: HDR2, 2: HDR3, Default: 0\n");
	printf("\t--stitch_distance : set stitch distance, default: 5.0(m)\n");
	printf("\t--cam0_ldch_path : cam0 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin\n");
	printf("\t--cam1_ldch_path : cam1 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin\n");
	printf("\t--set_ldch : set ldch, -1: disable, 1: read_file_set_ldch, 2: "
	       "read_buff_set_ldch. Default: 2\n");
	printf("\t--test_type : test type, 0:none, 1: media_deinit_init 2: "
	       "avs_framerate_switch. dafault: 0\n");
	printf("\t--test_loop : module test loop, default: -1\n");
	printf("\t--test_frame : when encoder outputs frameCount equal to "
	       "<test_frame>, mode_test start next loop, default: 10\n");
	printf("\t--chn_id : set vi channel id, default: 1\n");
	printf("\t--vi_chn_buf_cnt : set vi chn buff cnt, default: 2\n");
	printf("\t--input_bmp_path : set bmp path for osd, default: NULL\n");
	printf("\t--osd_display : osd if display, 0: no-display, 1: display. default: 1\n");
}

/******************************************************************************
 * function : venc thread
 ******************************************************************************/
static void *venc_get_stream(void *pArgs) {
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	void *pData = RK_NULL;
	char name[256] = {0};
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 loopCount = 0;
	FILE *fp = RK_NULL;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/venc_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			return RK_NULL;
		}
	}

	while (!gModeTest->bIfVencThreadQuit[ctx->s32ChnId]) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			/*  exit when complete */
			if (ctx->s32loopCount > 0 && loopCount >= ctx->s32loopCount) {
				SAMPLE_COMM_VENC_ReleaseStream(ctx);
				program_normal_exit(__func__, __LINE__);
				break;
			}

			if (fp) {
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}
			if (gModeTest->bIfEnableRtsp) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session[ctx->s32ChnId], pData,
				              ctx->stFrame.pstPack->u32Len, ctx->stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			}

			RK_LOGD("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.pstPack->u32Len);

			if (gModeTest->bIfModuleTestopen) {
				pthread_mutex_lock(&g_frame_count_mutex[ctx->s32ChnId]);
				gModeTest->u32VencGetFrameCount[ctx->s32ChnId]++;
				pthread_mutex_unlock(&g_frame_count_mutex[ctx->s32ChnId]);

				if (gModeTest->u32VencGetFrameCount[ctx->s32ChnId] ==
				    gModeTest->u32TestFrameCount) {
					sem_post(&g_sem_module_test[ctx->s32ChnId]);
				}
			}

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	if (fp) {
		fclose(fp);
		fp = RK_NULL;
	}
	return RK_NULL;
}

static RK_S32 rtsp_init(CODEC_TYPE_E enCodecType) {
	RK_S32 i = 0;
	g_rtsplive = create_rtsp_demo(554);
	RK_CHAR rtspAddr[BUFF_SIZE] = {0};

	for (i = 0; i < VENC_NUM_MAX; i++) {
		sprintf(rtspAddr, "/live/%d", i);
		g_rtsp_session[i] = rtsp_new_session(g_rtsplive, rtspAddr);
		if (enCodecType == RK_CODEC_TYPE_H264) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H264, RK_NULL, 0);
		} else if (enCodecType == RK_CODEC_TYPE_H265) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H265, RK_NULL, 0);
		} else {
			RK_LOGE("not support other type\n");
			gModeTest->bIfEnableRtsp = RK_FALSE;
			return RK_SUCCESS;
		}
		rtsp_sync_video_ts(g_rtsp_session[i], rtsp_get_reltime(), rtsp_get_ntptime());
		RK_LOGE("rtsp <%s> init success", rtspAddr);
	}
	gModeTest->bIfEnableRtsp = RK_TRUE;
	return RK_SUCCESS;
}

static RK_S32 rtsp_deinit(void) {
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	return RK_SUCCESS;
}

static RK_S32 media_init(g_mode_test *gModeTest) {
	RK_S32 s32Ret = RK_FAILURE;
	MPP_CHN_S stSrcChn, stDestChn;

	s32Ret = RK_MPI_SYS_Init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_Init failure:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	/* avs create */
	s32Ret = SAMPLE_COMM_AVS_CreateGrp(&ctx->avs);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_AVS_CreateGrp failure:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	/* isp init */
	if (gModeTest->bIfIspGroupInit) {
#ifdef RKAIQ_GRP
		s32Ret = SAMPLE_COMM_ISP_CamGroup_Init(
		    gModeTest->s32CamGrpId, gModeTest->hdr_mode, RK_TRUE, gModeTest->eGetLdchMode,
		    gModeTest->pLdchMeshData, &gModeTest->camgroup_cfg);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
		/* set isp framerate */
		s32Ret = SAMPLE_COMM_ISP_CamGroup_SetFrameRate(gModeTest->s32CamGrpId,
		                                               gModeTest->s32IspFps);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_ISP_CamGroup_SetFrameRate failure");
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
#endif
	} else {
		for (RK_S32 i = 0; i < gModeTest->s32CamNum; i++) {
			s32Ret =
			    SAMPLE_COMM_ISP_Init(i, gModeTest->hdr_mode, RK_FALSE, "/etc/iqfiles/");
			s32Ret |= SAMPLE_COMM_ISP_Run(i);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("ISP init failure camid:%d", i);
				return RK_FAILURE;
			}
		}
	}

	/* rtsp init */
	s32Ret = rtsp_init(gModeTest->enCodecType);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rtsp_init failure:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	/* vi init */
	for (RK_S32 i = 0; i < gModeTest->s32CamNum; i++) {
		s32Ret = SAMPLE_COMM_VI_CreateChn(&ctx->vi[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
	}

	for (RK_S32 i = 0; i < gModeTest->s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
		s32Ret = RK_MPI_VI_StartPipe(ctx->vi[i].u32PipeId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_StartPipe failure:$#X pipe:%d", s32Ret,
			        ctx->vi[i].u32PipeId);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
	}

	/* avs startgrp*/
	s32Ret = SAMPLE_COMM_AVS_StartGrp(&ctx->avs);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_AVS_StartGrp failure:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	/* venc init*/
	for (RK_S32 i = 0; i < VENC_NUM_MAX; i++) {
		gModeTest->bIfVencThreadQuit[i] = RK_FALSE;
		s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_CreateChn failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
	}

	/* rgn init */
	if (gModeTest->bIfOsdDisplay) {
		s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_CreateChn failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
	}

	// Bind VI[0] and avs[0]
	for (RK_S32 i = 0; i < gModeTest->s32CamNum; i++) {
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi[i].s32DevId;
		stSrcChn.s32ChnId = ctx->vi[i].s32ChnId;
		stDestChn.enModId = RK_ID_AVS;
		stDestChn.s32DevId = ctx->avs.s32GrpId;
		stDestChn.s32ChnId = i;
		SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	}

	/* Bind AVS[0,0] and VENC[0,0] */
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = ctx->avs.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[0].s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	/* Bind AVS[0,1] and VENC[0,1] */
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[1].s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	RK_LOGE("----------------media_init finish  !!!");
	return s32Ret;
}

static RK_S32 media_deinit(g_mode_test *gModeTest) {
	RK_S32 s32Ret = RK_FAILURE;
	MPP_CHN_S stSrcChn, stDestChn;

	/* Destroy RGN[0] */
	if (gModeTest->bIfOsdDisplay) {
		SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn);
	}
	for (RK_S32 i = 0; i < VENC_NUM_MAX; i++) {
		// Destroy VENC
		gModeTest->bIfVencThreadQuit[i] = RK_TRUE;
		pthread_join(ctx->venc[i].getStreamThread, NULL);
		/* Bind AVS[0,i] and VENC[0,i] */
		stSrcChn.enModId = RK_ID_AVS;
		stSrcChn.s32DevId = ctx->avs.s32GrpId;
		stSrcChn.s32ChnId = i;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc[i].s32ChnId;
		s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_UnBind failure:%#X", s32Ret);
		}
		s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[i]);
		if (s32Ret != RK_SUCCESS) {
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
	}

	/* rtsp deinit */
	s32Ret = rtsp_deinit();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rtsp_deinit failure:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	for (RK_S32 i = 0; i < gModeTest->s32CamNum; i++) {
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi[i].s32DevId;
		stSrcChn.s32ChnId = ctx->vi[i].s32ChnId;
		stDestChn.enModId = RK_ID_AVS;
		stDestChn.s32DevId = ctx->avs.s32GrpId;
		stDestChn.s32ChnId = i;
		SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	}

	/* Destroy AVS[0] */
	SAMPLE_COMM_AVS_StopGrp(&ctx->avs);
	SAMPLE_COMM_AVS_DestroyGrp(&ctx->avs);

	for (RK_S32 i = 0; i < gModeTest->s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
		s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_StopPipe failure:$#X pipe:%d", s32Ret,
			        ctx->vi[i].u32PipeId);
		}
	}

	/* Destroy VI[0] */
	for (RK_S32 i = 0; i < gModeTest->s32CamNum; i++) {
		SAMPLE_COMM_VI_DestroyChn(&ctx->vi[i]);
	}

	/* SYS deinit */
	RK_MPI_SYS_Exit();

	/* isp deinit */
	if (gModeTest->bIfIspGroupInit == RK_FALSE) {
		for (RK_S32 i = 0; i < gModeTest->s32CamNum; i++) {
			s32Ret = SAMPLE_COMM_ISP_Stop(i);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_Stop failure:%#X", s32Ret);
				return s32Ret;
			}
		}
	} else {
#ifdef RKAIQ_GRP
		s32Ret = SAMPLE_COMM_ISP_CamGroup_Stop(gModeTest->s32CamGrpId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Stop failure:%#X", s32Ret);
			return s32Ret;
		}
#endif
	}

	RK_LOGE("----------------media_deinit finish  !!!");
	return s32Ret;
}

static void wait_module_test_switch_success(void) {

	for (RK_U32 i = 0; i < VENC_NUM_MAX; i++) {
		pthread_mutex_lock(&g_frame_count_mutex[i]);
		gModeTest->u32VencGetFrameCount[i] = 0;
		pthread_mutex_unlock(&g_frame_count_mutex[i]);
		sem_wait(&g_sem_module_test[i]);
	}
}

static RK_S32 media_deinit_init(g_mode_test *gModeTest) {
	prctl(PR_SET_NAME, "media_deinit_init");
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32TestCount = 0;

	while (!gModeTest->bIfModuleTestThreadQuit) {

		s32Ret = media_deinit(gModeTest);
		if (s32Ret != RK_SUCCESS) {
			printf("media_deinit failure:%#X\n", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		s32Ret = media_init(gModeTest);
		if (s32Ret != RK_SUCCESS) {
			printf("media_init failure:%#X\n", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		wait_module_test_switch_success();
		s32TestCount++;

		RK_LOGE("--------------------media_deinit_init switch success, test total:%d now "
		        "count:%d",
		        gModeTest->u32ModuleTestLoop, s32TestCount);
		if (gModeTest->u32ModuleTestLoop > 0 &&
		    s32TestCount >= gModeTest->u32ModuleTestLoop) {
			RK_LOGE("--------------------media_deinit_init switch test pass(success), "
			        "test total:%d now count:%d",
			        gModeTest->u32ModuleTestLoop, s32TestCount);
			gModeTest->bIfModuleTestopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}

	RK_LOGE("--------------media_deinit_init exit !!!");
	return s32Ret;
}

static RK_S32 avs_resolution_switch(g_mode_test *gModeTest) {
	prctl(PR_SET_NAME, "avs_resolution_switch");
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32SrcWidth = ctx->avs.stAvsChnAttr[0].u32Width;
	RK_U32 u32SrcHeight = ctx->avs.stAvsChnAttr[0].u32Height;
	RK_U32 u32DstWidth = 2160;
	RK_U32 u32DstHeight = 540;
	AVS_CHN_ATTR_S stAvsChnAttr;
	VENC_CHN_ATTR_S stVencChnAttr;
	MPP_CHN_S stSrcChn, stDestChn;

	while (!gModeTest->bIfModuleTestThreadQuit) {

		/* ubind avs and venc */
		stSrcChn.enModId = RK_ID_AVS;
		stSrcChn.s32DevId = ctx->avs.s32GrpId;
		stSrcChn.s32ChnId = 0;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc[0].s32ChnId;
		s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_UnBind failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		/* avs destroy and reset resolution */
		memset(&stAvsChnAttr, 0, sizeof(AVS_CHN_ATTR_S));
		s32Ret =
		    RK_MPI_AVS_GetChnAttr(ctx->avs.s32GrpId, ctx->avs.s32ChnId, &stAvsChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_AVS_GetChnAttr failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		/* set avs resolution */
		if (stAvsChnAttr.u32Width == u32SrcWidth) {
			stAvsChnAttr.u32Width = u32DstWidth;
			stAvsChnAttr.u32Height = u32DstHeight;
		} else {
			stAvsChnAttr.u32Width = u32SrcWidth;
			stAvsChnAttr.u32Height = u32SrcHeight;
		}
		s32Ret =
		    RK_MPI_AVS_SetChnAttr(ctx->avs.s32GrpId, ctx->avs.s32ChnId, &stAvsChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_AVS_SetChnAttr failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		/* reset venc resolution */
		memset(&stVencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
		s32Ret = RK_MPI_VENC_GetChnAttr(ctx->venc[0].s32ChnId, &stVencChnAttr);
		RK_LOGD("w: %d h: %d", stVencChnAttr.stVencAttr.u32PicWidth,
		        stVencChnAttr.stVencAttr.u32PicHeight);
		if (stVencChnAttr.stVencAttr.u32PicWidth == ctx->venc[0].u32Width) {
			stVencChnAttr.stVencAttr.u32PicWidth = u32DstWidth;
			stVencChnAttr.stVencAttr.u32PicHeight = u32DstHeight;
			stVencChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(u32DstWidth);
			stVencChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(u32DstHeight);
		} else {
			stVencChnAttr.stVencAttr.u32PicWidth = u32SrcWidth;
			stVencChnAttr.stVencAttr.u32PicHeight = u32SrcHeight;
			stVencChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(u32SrcWidth);
			stVencChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(u32SrcHeight);
		}

		s32Ret |= RK_MPI_VENC_SetChnAttr(ctx->venc[0].s32ChnId, &stVencChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("venc set chn resolution failure");
			program_handle_error(__func__, __LINE__);
			break;
		}

		s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_Bind failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		wait_module_test_switch_success();
		s32TestCount++;

		RK_LOGE("--------------------avs_resolution_switch to:%dX%d success, test "
		        "total:%d now "
		        "count:%d",
		        stAvsChnAttr.u32Width, stAvsChnAttr.u32Height,
		        gModeTest->u32ModuleTestLoop, s32TestCount);
		if (gModeTest->u32ModuleTestLoop > 0 &&
		    s32TestCount >= gModeTest->u32ModuleTestLoop) {
			RK_LOGE("--------------------avs_resolution_switch test pass(success), test "
			        "total:%d now count:%d",
			        gModeTest->u32ModuleTestLoop, s32TestCount);
			gModeTest->bIfModuleTestopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}

	RK_LOGE("--------------avs_resolution_switch exit !!!");
	return s32Ret;
}

static void *sample_demo_stresstest(g_mode_test *gModeTest) {
	RK_S32 s32Ret = RK_FAILURE;
	wait_module_test_switch_success();
	SAMPLE_COMM_DumpMeminfo("Enter sample_vi_avs_venc_stress_test",
	                        gModeTest->s32ModuleTestType);

	switch (gModeTest->s32ModuleTestType) {
	case 1:
		s32Ret = media_deinit_init(gModeTest);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("media_deinit_init test failure:%#X", s32Ret);
		}
		break;
	case 2:
		s32Ret = avs_resolution_switch(gModeTest);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("avs_resolution_switch test failure:%#X", s32Ret);
		}
		break;
	default:
		RK_LOGE("----------------input the test_type:%d is't support",
		        gModeTest->s32ModuleTestType);
	}

	SAMPLE_COMM_DumpMeminfo("exit sample_vi_avs_venc_stress_test",
	                        gModeTest->s32ModuleTestType);
	RK_LOGE("---sample_vi_avs_venc_stress_test exit");
	return NULL;
}

static RK_S32 global_param_init(void) {

	ctx = (SAMPLE_MPI_CTX_S *)malloc(sizeof(SAMPLE_MPI_CTX_S));
	if (ctx == RK_NULL) {
		RK_LOGE("malloc for ctx failure");
		return RK_FAILURE;
	}
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	gModeTest = (g_mode_test *)malloc(sizeof(g_mode_test));
	if (gModeTest == RK_NULL) {
		RK_LOGE("malloc for gModeTest failure");
		return RK_FAILURE;
	}
	memset(gModeTest, 0, sizeof(g_mode_test));

	gModeTest->u32ModuleTestLoop = -1;
	gModeTest->u32TestFrameCount = 10;
	gModeTest->eGetLdchMode = RK_GET_LDCH_BY_BUFF;
	gModeTest->s32IspFps = 15;
	gModeTest->s32CamNum = 2;
	gModeTest->bIfIspGroupInit = RK_TRUE;
	gModeTest->bIfOsdDisplay = RK_TRUE;

	for (RK_S32 i = 0; i < VENC_NUM_MAX; i++) {
		sem_init(&g_sem_module_test[i], 0, 0);

		if (pthread_mutex_init(&g_frame_count_mutex[i], RK_NULL) != 0) {
			RK_LOGE("mutex init failure \n");
			return RK_FAILURE;
		}
	}

	if (RK_SUCCESS != pthread_mutex_init(&g_rtsp_mutex, RK_NULL)) {
		RK_LOGE("pthread_mutex_init failure");
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

static RK_S32 global_param_deinit(void) {

	if (ctx) {
		free(ctx);
		ctx = RK_NULL;
	}

	if (gModeTest) {
		free(gModeTest);
		gModeTest = RK_NULL;
	}

	for (RK_S32 i = 0; i < VENC_NUM_MAX; i++) {
		sem_destroy(&g_sem_module_test[i]);
		pthread_mutex_destroy(&g_frame_count_mutex[i]);
	}

	pthread_mutex_destroy(&g_rtsp_mutex);
	return RK_SUCCESS;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32ViWidth = 1920;
	RK_U32 u32ViHeight = 1080;
	RK_U32 u32AvsChn0Width = 3840;
	RK_U32 u32AvsChn0Height = 1080;
	RK_U32 u32AvsChn1Width = 1920;
	RK_U32 u32AvsChn1Height = 544;
	RK_CHAR *pAvsCalibFilePath = "/oem/usr/share/avs_calib/calib_file.xml";
	RK_CHAR *pAvsMeshAlphaPath = "/tmp/";
	RK_CHAR *pAvsLutFilePath = NULL;
	RK_CHAR *pOutPathVenc = NULL;
	RK_CHAR *pCam0LdchMeshPath = "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin";
	RK_CHAR *pCam1LdchMeshPath = "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin";
	RK_CHAR *pBmpPath = NULL;
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H265;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H265CBR;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	RK_CHAR *pCodecName = "H265";
	RK_S32 s32CamNum = 2;
	RK_S32 s32loopCnt = -1;
	RK_S32 s32BitRate = 4 * 1024;
	RK_S32 s32CamGrpId = 0;
	RK_S32 s32ChnId = 1;
	RK_U32 u32ViChnBuffCnt = 2;
	RK_FLOAT fStitchDistance = 5;
	pthread_t mode_test_thread = 0;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	SAMPLE_COMM_CheckFd(RK_TRUE);
	s32Ret = global_param_init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("global_param_init failure:%#X", s32Ret);
		return RK_FAILURE;
	}

	signal(SIGINT, sigterm_handler);

#ifdef RKAIQ
	RK_BOOL bMultictx = RK_FALSE;
#endif
	int c;
	char *iq_file_dir = "/etc/iqfiles/";
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		const char *tmp_optarg = optarg;
		switch (c) {
		case 'a':
			if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
				tmp_optarg = argv[optind++];
			}
			if (tmp_optarg) {
				iq_file_dir = (char *)tmp_optarg;
			} else {
				iq_file_dir = NULL;
			}
			break;
		case 'A':
			pAvsCalibFilePath = optarg;
			break;
		case 'b':
			s32BitRate = atoi(optarg);
			break;
		case 'e':
			if (!strcmp(optarg, "h264cbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264CBR;
				pCodecName = "H264";
			} else if (!strcmp(optarg, "h264vbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264VBR;
				pCodecName = "H264";
			} else if (!strcmp(optarg, "h265cbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265CBR;
				pCodecName = "H265";
			} else if (!strcmp(optarg, "h265vbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265VBR;
				pCodecName = "H265";
			} else {
				RK_LOGE("this demo isn't support this encoder type.");
				return 0;
			}
			break;
		case 'n':
			s32CamNum = atoi(optarg);
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'o':
			pOutPathVenc = optarg;
			break;
		case 'F':
			gModeTest->s32IspFps = atoi(optarg);
			break;
		case 'd':
			fStitchDistance = atof(optarg);
			break;
		case 'L':
			pCam0LdchMeshPath = optarg;
			break;
		case 'L' + 'm':
			pCam1LdchMeshPath = optarg;
			break;
		case 'v' + 's':
			u32ViWidth = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32ViHeight = atoi(tmp_optarg + 1);
			break;
		case 'a' + 's':
			u32AvsChn0Width = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32AvsChn0Height = atoi(tmp_optarg + 1);
			break;
		case 'a' + 'v':
			u32AvsChn1Width = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32AvsChn1Height = atoi(tmp_optarg + 1);
			break;
		case 'h' + 'm':
			if (atoi(optarg) == 0) {
				hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
			} else if (atoi(optarg) == 1) {
				hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
			} else if (atoi(optarg) == 2) {
				hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR3;
			} else {
				RK_LOGE("input hdr_mode is not support(error)");
				print_usage(argv[0]);
				goto __MEDIA_INIT_FAILURE;
			}
			break;
		case 'l' + 'd':
			gModeTest->eGetLdchMode = atoi(optarg);
			break;
		case 't' + 't':
			gModeTest->s32ModuleTestType = atoi(optarg);
			break;
		case 't' + 'l':
			gModeTest->u32ModuleTestLoop = atoi(optarg);
			break;
		case 't' + 'f':
			gModeTest->u32TestFrameCount = atoi(optarg);
			break;
		case 'c' + 'i':
			s32ChnId = atoi(optarg);
			break;
		case 'v' + 'b':
			u32ViChnBuffCnt = atoi(optarg);
			break;
		case 'i':
			gModeTest->bIfIspGroupInit = atoi(optarg);
			break;
		case 'i' + 'b':
			pBmpPath = optarg;
			break;
		case 'o' + 'd':
			gModeTest->bIfOsdDisplay = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return RK_FAILURE;
		}
	}

	printf("#CameraGrpIdx: %d\n", s32CamGrpId);
	printf("#pAvsLutFilePath: %s\n", pAvsLutFilePath);
	printf("#pAvsCalibFilePath: %s\n", pAvsCalibFilePath);
	printf("#pAvsMeshAlphaPath: %s\n", pAvsMeshAlphaPath);
	printf("#CodecName:%s\n", pCodecName);
	printf("#Output Path: %s\n", pOutPathVenc);
	printf("#IQ Path: %s\n", iq_file_dir);
	printf("#fStitchDistance: %f\n", fStitchDistance);
	printf("#pCam0LdchMeshPath: %s\n", pCam0LdchMeshPath);
	printf("#pCam1LdchMeshPath: %s\n", pCam1LdchMeshPath);

	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
#ifdef RKAIQ_GRP
		gModeTest->camgroup_cfg.sns_num = s32CamNum;
		gModeTest->camgroup_cfg.config_file_dir = iq_file_dir;
		gModeTest->hdr_mode = hdr_mode;
#endif
#endif
	}
	gModeTest->s32CamNum = s32CamNum;
	gModeTest->enCodecType = enCodecType;

	if (gModeTest->eGetLdchMode == RK_GET_LDCH_BY_FILE && pCam0LdchMeshPath &&
	    pCam1LdchMeshPath) {
		gModeTest->pLdchMeshData[0] = pCam0LdchMeshPath;
		gModeTest->pLdchMeshData[1] = pCam1LdchMeshPath;
	}

	/* Init avs[0] */
	ctx->avs.s32GrpId = 0;
	ctx->avs.s32ChnId = 0;
	ctx->avs.u32SrcWidth = u32ViWidth;
	ctx->avs.u32SrcHeight = u32ViHeight;
	ctx->avs.eGetLdchMode = gModeTest->eGetLdchMode;
	ctx->avs.fDistance = fStitchDistance;
	ctx->avs.pLdchMeshData = (RK_U16 **)gModeTest->pLdchMeshData;
	/* GrpAttr setting */
	ctx->avs.stAvsGrpAttr.enMode = 0; /* 0: blend 1: no blend */
	ctx->avs.stAvsGrpAttr.u32PipeNum = s32CamNum;
	ctx->avs.stAvsGrpAttr.stGainAttr.enMode = AVS_GAIN_MODE_AUTO;
	ctx->avs.stAvsGrpAttr.stOutAttr.enPrjMode = AVS_PROJECTION_EQUIRECTANGULAR;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Width = u32AvsChn0Width;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Height = u32AvsChn0Height;
	ctx->avs.stAvsGrpAttr.bSyncPipe = RK_TRUE;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Width = u32ViWidth;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Height = u32ViHeight;
	ctx->avs.stAvsGrpAttr.stOutAttr.fDistance = fStitchDistance;
	ctx->avs.stAvsGrpAttr.stInAttr.enParamSource = AVS_PARAM_SOURCE_CALIB;
	ctx->avs.stAvsGrpAttr.stInAttr.stCalib.pCalibFilePath = pAvsCalibFilePath;
	/* set chn0 */
	ctx->avs.stAvsChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 2;
#ifdef RV1126
	ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 4;
#endif
	ctx->avs.stAvsChnAttr[0].u32Depth = 1;
	ctx->avs.stAvsChnAttr[0].u32Width = u32AvsChn0Width;
	ctx->avs.stAvsChnAttr[0].u32Height = u32AvsChn0Height;
	ctx->avs.stAvsChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	/* set chn1 */
	ctx->avs.stAvsChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
	ctx->avs.stAvsChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsChnAttr[1].stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsChnAttr[1].u32FrameBufCnt = 2;
#ifdef RV1126
	ctx->avs.stAvsChnAttr[1].u32FrameBufCnt = 4;
#endif
	ctx->avs.stAvsChnAttr[1].u32Depth = 1;
	ctx->avs.stAvsChnAttr[1].u32Width = u32AvsChn1Width;
	ctx->avs.stAvsChnAttr[1].u32Height = u32AvsChn1Height;
	ctx->avs.stAvsChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;

	/* init vi */
	for (RK_S32 i = 0; i < s32CamNum; i++) {
		ctx->vi[i].u32Width = u32ViWidth;
		ctx->vi[i].u32Height = u32ViHeight;
		ctx->vi[i].s32DevId = i;
		ctx->vi[i].u32PipeId = i;
		ctx->vi[i].s32ChnId = s32ChnId;
		ctx->vi[i].bIfIspGroupInit = gModeTest->bIfIspGroupInit;
#ifdef RV1126
		ctx->vi[i].bIfIspGroupInit = RK_FALSE;
#endif
		ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = u32ViChnBuffCnt;
		ctx->vi[i].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
		ctx->vi[i].stChnAttr.u32Depth = 0;
		ctx->vi[i].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vi[i].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
		ctx->vi[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
		ctx->vi[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	}

	/* Init VENC[0] */
	ctx->venc[0].s32ChnId = 0;
	ctx->venc[0].u32Width = u32AvsChn0Width;
	ctx->venc[0].u32Height = u32AvsChn0Height;
	ctx->venc[0].u32Fps = gModeTest->s32IspFps;
	ctx->venc[0].u32Gop = gModeTest->s32IspFps * 2;
	ctx->venc[0].u32BitRate = s32BitRate;
	ctx->venc[0].enCodecType = enCodecType;
	ctx->venc[0].enRcMode = enRcMode;
	ctx->venc[0].getStreamCbFunc = venc_get_stream;
	ctx->venc[0].s32loopCount = s32loopCnt;
	ctx->venc[0].dstFilePath = pOutPathVenc;
	ctx->venc[0].u32BuffSize = ctx->venc[0].u32Width * ctx->venc[0].u32Height / 3;
	ctx->venc[0].u32StreamBufCnt = 3;
#ifdef RV1126
	ctx->venc[0].u32StreamBufCnt = 4;
#endif
	ctx->venc[0].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	if (enCodecType == RK_CODEC_TYPE_H264) {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 66;
	} else {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 0;
	}
	ctx->venc[0].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP

	/* Init VENC[1] */
	ctx->venc[1].s32ChnId = 1;
	ctx->venc[1].u32Width = u32AvsChn1Width;
	ctx->venc[1].u32Height = u32AvsChn1Height;
	ctx->venc[1].u32Fps = gModeTest->s32IspFps;
	ctx->venc[1].u32Gop = gModeTest->s32IspFps * 2;
	ctx->venc[1].enCodecType = enCodecType;
	ctx->venc[1].enRcMode = enRcMode;
	ctx->venc[1].getStreamCbFunc = venc_get_stream;
	ctx->venc[1].s32loopCount = s32loopCnt;
	ctx->venc[1].dstFilePath = pOutPathVenc;
	ctx->venc[1].u32BuffSize = ctx->venc[1].u32Width * ctx->venc[1].u32Height / 3;
	ctx->venc[1].u32BitRate = 1024;
	ctx->venc[1].u32StreamBufCnt = 3;
#ifdef RV1126
	ctx->venc[1].u32StreamBufCnt = 4;
#endif
	ctx->venc[1].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	if (enCodecType == RK_CODEC_TYPE_H264) {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 66;
	} else {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 0;
	}
	ctx->venc[1].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP

	/* Init RGN */
	RK_U32 u32BmpWidth = 0;
	RK_U32 u32BmpHeight = 0;
	s32Ret = SAMPLE_COMM_GetBmpResolution(pBmpPath, &u32BmpWidth, &u32BmpHeight);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		u32BmpWidth = 128;
		u32BmpHeight = 128;
	}
	ctx->rgn.rgnHandle = 0;
	ctx->rgn.stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn.stMppChn.enModId = RK_ID_VENC;
	ctx->rgn.stMppChn.s32ChnId = ctx->venc[0].s32ChnId;
	ctx->rgn.stMppChn.s32DevId = 0;
	ctx->rgn.stRegion.s32X = ctx->venc[0].u32Width / 2; // must be 16 aligned
	ctx->rgn.stRegion.s32Y = 0;                         // must be 16 aligned
	ctx->rgn.stRegion.u32Width = u32BmpWidth;           // must be 16 aligned
	ctx->rgn.stRegion.u32Height = u32BmpHeight;         // must be 16 aligned
	ctx->rgn.u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn.u32BgAlpha = 128;
	ctx->rgn.u32FgAlpha = 128;
	ctx->rgn.srcFileBmpName = pBmpPath;
	ctx->rgn.u32Layer = 1;

	/* mode test thread launch */
	if (gModeTest->s32ModuleTestType) {
		gModeTest->bIfModuleTestopen = RK_TRUE;
		pthread_create(&mode_test_thread, RK_NULL, (void * (*)(void *))sample_demo_stresstest, gModeTest);
	}

	s32Ret = media_init(gModeTest);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("media_init failure:%#X", s32Ret);
		g_exit_result = RK_FAILURE;
		goto __MEDIA_INIT_FAILURE;
	}

	printf("%s initial finish\n", __func__);

	while (!gModeTest->bIfMainThreadQuit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	if (gModeTest->s32ModuleTestType && mode_test_thread) {
		gModeTest->bIfModuleTestThreadQuit = RK_TRUE;
		pthread_join(mode_test_thread, RK_NULL);
	}

	s32Ret = media_deinit(gModeTest);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("media_deinit failure:%#X", s32Ret);
		g_exit_result = RK_FAILURE;
	}

__MEDIA_INIT_FAILURE:

	global_param_deinit();
	SAMPLE_COMM_CheckFd(RK_FALSE);

	return g_exit_result;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
