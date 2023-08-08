/*
 * Copyright 2022 Rockchip Electronics Co. LTD
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
#endif

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

#define VI_CHN_MAX 2
#define CAM_NUM_MAX 2

#define BUFFER_SIZE 255
#define SEND_FRAME_TIMEOUT 2000
#define GET_FRAME_TIMEOUT 2000
typedef struct _rkModeTest {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfViHandleThreadQuit;
	RK_BOOL bIfVencThreadQuit;
	RK_S32 s32EptzTestLoop;
	RK_S32 s32EptzViIndex;
	RK_U32 u32ViTestFrame;
	RK_U32 u32ZoomStep;
	RK_U32 u32ZoomSwitch;
	RK_U32 u32ZoomLimit;
	RK_U32 u32CamNum;
} RK_MODE_TEST_S;

typedef struct _rkIspInitParam {
	RK_BOOL bMultictx;
	RK_BOOL bIfIspGroupInit;
	RK_CHAR *pIqFileDir;
	RK_S32 s32CamId[CAM_NUM_MAX];
	RK_S32 s32CamGroupId;
	RK_U32 u32CamNum;
#if (defined RKAIQ) && (defined UAPI2)
	rk_aiq_working_mode_t eHdrMode;
#endif
} RK_ISP_INIT_PARAM_S;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[VI_CHN_MAX];
	SAMPLE_VENC_CTX_S venc;
} SAMPLE_MPI_CTX_S;

static RK_BOOL g_rtsp_ifenbale = RK_FALSE;
static RK_S32 g_ProExitReturnValue = RK_SUCCESS;
static RK_MODE_TEST_S *gModeTest = RK_NULL;
static rtsp_demo_handle g_rtsplive = RK_NULL;
static rtsp_session_handle g_rtsp_session = RK_NULL;

static void program_handle_error(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> error exit!", func, line);
	g_ProExitReturnValue = RK_FAILURE;
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

static RK_CHAR optstr[] = "?::a::w:h:o:e:s:t:l:f:i:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"output_path", required_argument, NULL, 'o'},
    {"encode", required_argument, NULL, 'e'},
    {"zoom_step", required_argument, NULL, 's'},
    {"zoom_switch", required_argument, NULL, 't'},
    {"zoom_limit", required_argument, NULL, 't' + 'l'},
    {"test_frame", required_argument, NULL, 't' + 'f'},
    {"ispLaunchMode", required_argument, NULL, 'i'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -w 1920 -h 1080 -a /etc/iqfiles/ -s -1 -o /userdata/\n", name);
#if (defined RKAIQ) && (defined UAPI2)
	printf(
	    "\t-a | --aiq : enable aiq with dirpath provided, eg:-a /etc/iqfiles/, \n"
	    "\t		set dirpath empty to using path by default, without this option aiq \n"
	    "\t		should run in other application\n");
#endif
	printf("\t-w | --width : camera width, Default: 1920\n");
	printf("\t-h | --height : camera height, Default: 1080\n");
	printf("\t-o | --output_path : encode output file path, Default: NULL\n");
	printf("\t-e | --encode : set encode type, Value: h264cbr, h264vbr, h265cbr, "
	       "h265vbr, default: h264cbr \n");
	printf("\t-s | --zoom_step : zoom step, 1: 0.1, 2: 0.2, 3: 0.3, 4: 0.4, 5: 0.5, 6:0.6"
	       "default: 1\n");
	printf("\t-t --zoom_switch : Switching to camera 1, Default: 20\n");
	printf("\t-l --zoom_limit : max zoom, Default: 60\n");
	printf("\t-f --test_frame : when Vi outputs frameCount equal to "
	       "<test_frame>\n");
	printf(
	    "\t-i --ispLaunchMode : 0: single cam init, 1: camera group init. default: 0\n");
}

static RK_S32 isp_init(RK_ISP_INIT_PARAM_S *pstIspParam) {
	RK_S32 s32Ret = RK_FAILURE;

#if (defined RKAIQ) && (defined UAPI2)
	if (pstIspParam->bIfIspGroupInit == RK_FALSE) {
		for (RK_S32 i = 0; i < pstIspParam->u32CamNum; i++) {
			s32Ret =
			    SAMPLE_COMM_ISP_Init(pstIspParam->s32CamId[i], pstIspParam->eHdrMode,
			                         pstIspParam->bMultictx, pstIspParam->pIqFileDir);
			s32Ret |= SAMPLE_COMM_ISP_Run(pstIspParam->s32CamId[i]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("ISP init failure camid:%d", i);
				return RK_FAILURE;
			}
		}
	} else if (pstIspParam->bIfIspGroupInit == RK_TRUE) {
		rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
		memset(&camgroup_cfg, 0, sizeof(rk_aiq_camgroup_instance_cfg_t));
		camgroup_cfg.sns_num = pstIspParam->u32CamNum;
		camgroup_cfg.config_file_dir = pstIspParam->pIqFileDir;
		s32Ret = SAMPLE_COMM_ISP_CamGroup_Init(pstIspParam->s32CamGroupId,
		                                       pstIspParam->eHdrMode,
		                                       pstIspParam->bMultictx, &camgroup_cfg);
		if (s32Ret != RK_SUCCESS) {
			printf("%s : isp cam group init\n", __func__);
			return RK_FAILURE;
		}
	} else {
		RK_LOGE("ISP dosen't support this launch mode %d", pstIspParam->bIfIspGroupInit);
		return RK_FAILURE;
	}
#endif
	return RK_SUCCESS;
}

static RK_S32 isp_deinit(RK_ISP_INIT_PARAM_S *pstIspParam) {
	RK_S32 s32Ret = RK_SUCCESS;
#if (defined RKAIQ) && (defined UAPI2)
	if (pstIspParam->bIfIspGroupInit == RK_FALSE) {
		for (RK_S32 i = 0; i < pstIspParam->u32CamNum; i++) {
			s32Ret = SAMPLE_COMM_ISP_Stop(i);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_Stop failure:%#X", s32Ret);
				return s32Ret;
			}
		}
	} else if (pstIspParam->bIfIspGroupInit == RK_TRUE) {
		s32Ret = SAMPLE_COMM_ISP_CamGroup_Stop(pstIspParam->s32CamGroupId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Stop failure:%#X", s32Ret);
			return s32Ret;
		}
	} else {
		RK_LOGE("ISP deinit dosen't support this mode %d", pstIspParam->bIfIspGroupInit);
		return RK_FAILURE;
	}
#endif
	return RK_SUCCESS;
}

static RK_S32 sample_eptz_switch(SAMPLE_VI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32SrcWidth = ctx->u32Width * 10;
	RK_U32 u32SrcHeight = ctx->u32Height * 10;
	static RK_U32 u32MultipleOfZoom = 10;
	VI_CROP_INFO_S stCropInfo;
	memset(&stCropInfo, 0, sizeof(VI_CROP_INFO_S));
	s32Ret = RK_MPI_VI_GetEptz(ctx->u32PipeId, ctx->s32ChnId, &stCropInfo);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_GetEptz failure:%#X  pipe:%d chnid:%d index:%d", s32Ret,
		        ctx->u32PipeId, ctx->s32ChnId, gModeTest->s32EptzViIndex);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	stCropInfo.stCropRect.u32Width = RK_ALIGN_2(u32SrcWidth / u32MultipleOfZoom);
	stCropInfo.stCropRect.u32Height = RK_ALIGN_2(u32SrcHeight / u32MultipleOfZoom);
	stCropInfo.stCropRect.s32X =
	    RK_ALIGN_2((ctx->u32Width - stCropInfo.stCropRect.u32Width) / 2);
	stCropInfo.stCropRect.s32Y =
	    RK_ALIGN_2((ctx->u32Height - stCropInfo.stCropRect.u32Height) / 2);
	s32Ret = RK_MPI_VI_SetEptz(ctx->u32PipeId, ctx->s32ChnId, stCropInfo);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_SetEptz failure:%#X  pipe:%d chnid:%d", s32Ret, ctx->u32PipeId,
		        ctx->s32ChnId);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}
	u32MultipleOfZoom += gModeTest->u32ZoomStep;
	if (u32MultipleOfZoom > gModeTest->u32ZoomLimit) {
		u32MultipleOfZoom = 10;
	}

	if (u32MultipleOfZoom > gModeTest->u32ZoomSwitch) {
		gModeTest->s32EptzViIndex = 1;
	} else {
		gModeTest->s32EptzViIndex = 0;
	}
	return RK_SUCCESS;
}

static RK_VOID *handle_vi_stream(RK_VOID *pArgs) {
	SAMPLE_MPI_CTX_S *ctx = (SAMPLE_MPI_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32GetStreamCount = 0;
	RK_S32 s32EptzTestloop = 0;

	while (!gModeTest->bIfViHandleThreadQuit) {

		s32Ret = RK_MPI_VI_GetChnFrame(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId,
		                               &ctx->vi[0].stViFrame, GET_FRAME_TIMEOUT);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_GetChnFrame failure:%#X, pipe:%d chnid:%d ", s32Ret,
			        ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);
			continue;
		}
		s32Ret = RK_MPI_VI_GetChnFrame(ctx->vi[1].u32PipeId, ctx->vi[1].s32ChnId,
		                               &ctx->vi[1].stViFrame, GET_FRAME_TIMEOUT);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_GetChnFrame failure:%#X, pipe:%d chnid:%d ", s32Ret,
			        ctx->vi[1].u32PipeId, ctx->vi[1].s32ChnId);
			RK_MPI_VI_ReleaseChnFrame(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId,
			                          &ctx->vi[0].stViFrame);
			continue;
		}

		/* eptz handle */
		if (u32GetStreamCount == gModeTest->u32ViTestFrame) {
			u32GetStreamCount = 0;
			s32Ret = sample_eptz_switch(&ctx->vi[gModeTest->s32EptzViIndex]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("sample_eptz_switch failure:%#X Vi index:%d", s32Ret,
				        gModeTest->s32EptzViIndex);
				program_handle_error(__func__, __LINE__);
			}

			s32EptzTestloop++;

			if (gModeTest->s32EptzTestLoop > 0 &&
			    s32EptzTestloop > gModeTest->s32EptzTestLoop) {
				RK_LOGE("---------------Eptz test end:%d ", gModeTest->s32EptzTestLoop);
				program_normal_exit(__func__, __LINE__);
			}
		}

		/* send frame to venc */
		s32Ret = RK_MPI_VENC_SendFrame(0, &ctx->vi[gModeTest->s32EptzViIndex].stViFrame,
		                               SEND_FRAME_TIMEOUT);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_SendFrame timeout:%#X vi index:%d", s32Ret,
			        gModeTest->s32EptzViIndex);
			program_handle_error(__func__, __LINE__);
		}

		/* release vi frame */
		for (RK_U32 i = 0; i < gModeTest->u32CamNum; i++) {
			RK_MPI_VI_ReleaseChnFrame(ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId,
			                          &ctx->vi[i].stViFrame);
		}
		u32GetStreamCount++;
		RK_LOGD("-------------------------------------------------- vi handle "
		        "u32GetStreamCount:%d",
		        u32GetStreamCount);
	}

	RK_LOGD("-------------------------handle vi thread eixt");
	return RK_NULL;
}

static RK_VOID *venc_get_stream(RK_VOID *pArgs) {
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32Fd = 0;
	RK_S32 s32LoopCount = 0;
	RK_U8 *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};
	FILE *fp = RK_NULL;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/venc_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
		s32Fd = fileno(fp);
	}
	while (!gModeTest->bIfVencThreadQuit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {

			if (fp && !gModeTest->bIfMainThreadQuit) {
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}

			if (g_rtsp_ifenbale) {
				rtsp_tx_video(g_rtsp_session, pData, ctx->stFrame.pstPack->u32Len,
				              ctx->stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
			} else {
				RK_LOGD("venc %d get_stream count: %d", ctx->s32ChnId, s32LoopCount);
			}
			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			s32LoopCount++;

			if (ctx->s32loopCount > 0 && s32LoopCount >= ctx->s32loopCount) {
				program_normal_exit(__func__, __LINE__);
				break;
			}
		}
	}

	if (fp) {
		fsync(s32Fd);
		fclose(fp);
		fp = RK_NULL;
	}

	RK_LOGD("chnId:%d venc_get_stream exit!!!", ctx->s32ChnId);
	return RK_NULL;
}

static RK_S32 rtsp_init(CODEC_TYPE_E enCodecType) {
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
	if (enCodecType == RK_CODEC_TYPE_H264) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, RK_NULL, 0);
	} else if (enCodecType == RK_CODEC_TYPE_H265) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H265, RK_NULL, 0);
	} else {
		RK_LOGE("not support other type\n");
		return RK_SUCCESS;
	}
	rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());
	RK_LOGD("rtsp <%s> init success", "/live/0");
	g_rtsp_ifenbale = RK_TRUE;
	return RK_SUCCESS;
}

static RK_S32 rtsp_deinit(void) {
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	g_rtsp_ifenbale = RK_FALSE;
	return RK_SUCCESS;
}

static RK_S32 globol_param_init(void) {
	gModeTest = (RK_MODE_TEST_S *)malloc(sizeof(RK_MODE_TEST_S));
	if (gModeTest == RK_NULL) {
		RK_LOGE("malloc for gModeTest failure");
		return RK_FAILURE;
	}
	memset(gModeTest, 0, sizeof(RK_MODE_TEST_S));
	gModeTest->s32EptzTestLoop = -1;
	gModeTest->u32ZoomStep = 1;
	gModeTest->u32ZoomSwitch = 20;
	gModeTest->u32ZoomLimit = 60;

	g_ProExitReturnValue = RK_SUCCESS;
	return RK_SUCCESS;
}

static RK_S32 globol_param_deinit(void) {
	if (gModeTest) {
		free(gModeTest);
		gModeTest = RK_NULL;
	}
	return RK_SUCCESS;
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32VideoWidth = 1920;
	RK_U32 u32VideoHeight = 1080;
	RK_U32 u32CamNum = 2;
	RK_U32 i = 0;
	RK_U32 u32BitRate = 2 * 1024;
	RK_U32 u32MultipleOfZoom = 0;
	RK_CHAR *pIqFileDir = "/oem/usr/share/iqfiles/";
	RK_CHAR *pOutPathVenc = RK_NULL;
	SAMPLE_MPI_CTX_S *ctx = RK_NULL;
	RK_ISP_INIT_PARAM_S *pstIspParam = RK_NULL;
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;

	pthread_t vi_handle_thread_id;

	if (argc < 2) {
		print_usage(argv[0]);
		g_ProExitReturnValue = RK_FAILURE;
		goto __PARAM_INIT_FAILED;
	}

	ctx = (SAMPLE_MPI_CTX_S *)malloc(sizeof(SAMPLE_MPI_CTX_S));
	if (ctx == RK_NULL) {
		RK_LOGE("malloc for ctx failure");
		g_ProExitReturnValue = RK_FAILURE;
		goto __FAILED2;
	}
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	pstIspParam = (RK_ISP_INIT_PARAM_S *)malloc(sizeof(RK_ISP_INIT_PARAM_S));
	if (pstIspParam == RK_NULL) {
		RK_LOGE("malloc for pstIspParam failure");
		g_ProExitReturnValue = RK_FAILURE;
		goto __FAILED2;
	}
	memset(pstIspParam, 0, sizeof(RK_ISP_INIT_PARAM_S));

	s32Ret = globol_param_init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("globol_param_init");
		g_ProExitReturnValue = RK_FAILURE;
		goto __FAILED2;
	}

	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);

	RK_S32 c = 0;
	while ((c = getopt_long(argc, argv, optstr, long_options, RK_NULL)) != -1) {
		const char *tmp_optarg = optarg;
		switch (c) {
		case 'a':
			if (!optarg && RK_NULL != argv[optind] && '-' != argv[optind][0]) {
				tmp_optarg = argv[optind++];
			}
			if (tmp_optarg) {
				pIqFileDir = (char *)tmp_optarg;
			} else {
				pIqFileDir = RK_NULL;
			}
			break;
		case 'w':
			u32VideoWidth = atoi(optarg);
			break;
		case 'h':
			u32VideoHeight = atoi(optarg);
			break;
		case 'o':
			pOutPathVenc = optarg;
			break;
		case 'e':
			if (!strcmp(optarg, "h264cbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264CBR;
			} else if (!strcmp(optarg, "h264vbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264VBR;
			} else if (!strcmp(optarg, "h264avbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264AVBR;
			} else if (!strcmp(optarg, "h265cbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265CBR;
			} else if (!strcmp(optarg, "h265vbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265VBR;
			} else if (!strcmp(optarg, "h265avbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265AVBR;
			} else {
				printf("ERROR: Invalid encoder type.\n");
				print_usage(argv[0]);
				g_ProExitReturnValue = RK_FAILURE;
				goto __PARAM_INIT_FAILED;
			}
			break;
		case 'z':
			u32MultipleOfZoom = atoi(optarg);
			break;
		case 's':
			gModeTest->u32ZoomStep = atoi(optarg);
			break;
		case 't':
			gModeTest->u32ZoomSwitch = atoi(optarg);
			break;
		case 't' + 'l':
			gModeTest->u32ZoomLimit = atoi(optarg);
			break;
		case 't' + 'f':
			gModeTest->u32ViTestFrame = atoi(optarg);
			break;
		case 'i':
			pstIspParam->bIfIspGroupInit = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	if (pIqFileDir) {
#if (defined RKAIQ) && (defined UAPI2)
		printf("#Rkaiq XML DirPath: %s\n", pIqFileDir);
		rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
		pstIspParam->bMultictx = RK_TRUE;
		pstIspParam->eHdrMode = hdr_mode;
		pstIspParam->pIqFileDir = pIqFileDir;
		pstIspParam->u32CamNum = u32CamNum;
		gModeTest->u32CamNum = u32CamNum;
		if (pstIspParam->bIfIspGroupInit == RK_FALSE) {
			for (RK_S32 i = 0; i < u32CamNum; i++) {
				pstIspParam->s32CamId[i] = i;
			}
		} else if (pstIspParam->bIfIspGroupInit == RK_TRUE) {
			pstIspParam->s32CamGroupId = 0;
		}
		s32Ret = isp_init(pstIspParam);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("isp init failure");
			goto __FAILED2;
		}
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		g_ProExitReturnValue = RK_FAILURE;
		goto __FAILED;
	}

	/* init rtsp*/
	rtsp_init(enCodecType);

	/* init vi */
	for (i = 0; i < u32CamNum; i++) {
		ctx->vi[i].u32Width = u32VideoWidth;
		ctx->vi[i].u32Height = u32VideoHeight;
		ctx->vi[i].s32DevId = i;
		ctx->vi[i].u32PipeId = i;
		ctx->vi[i].s32ChnId = 0;
		ctx->vi[i].stChnAttr.stIspOpt.stMaxSize.u32Width = u32VideoWidth;
		ctx->vi[i].stChnAttr.stIspOpt.stMaxSize.u32Height = u32VideoHeight;
		ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = 2;
		ctx->vi[i].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
		ctx->vi[i].stChnAttr.u32Depth = 2;
		ctx->vi[i].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vi[i].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
		ctx->vi[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
		ctx->vi[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
		ctx->vi[i].bIfIspGroupInit = pstIspParam->bIfIspGroupInit;
		if (u32MultipleOfZoom && i == 0) {
			ctx->vi[i].bIfOpenEptz = RK_TRUE;
			ctx->vi[i].stCropInfo.stCropRect.u32Width =
			    RK_ALIGN_2(ctx->vi[i].u32Width / u32MultipleOfZoom);
			ctx->vi[i].stCropInfo.stCropRect.u32Height =
			    RK_ALIGN_2(ctx->vi[i].u32Height / u32MultipleOfZoom);
			ctx->vi[i].stCropInfo.stCropRect.s32X = RK_ALIGN_2(
			    (ctx->vi[i].u32Width - ctx->vi[i].stCropInfo.stCropRect.u32Width) / 2);
			ctx->vi[i].stCropInfo.stCropRect.s32Y = RK_ALIGN_2(
			    (ctx->vi[i].u32Height - ctx->vi[i].stCropInfo.stCropRect.u32Height) / 2);
		}
		SAMPLE_COMM_VI_CreateChn(&ctx->vi[i]);
	}

	/* Start pipe */
	if (pstIspParam->bIfIspGroupInit == RK_TRUE) { /* isp group init */
		for (i = 0; i < u32CamNum; i++) {
			s32Ret = RK_MPI_VI_StartPipe(ctx->vi[i].u32PipeId);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_StartPipe failure:$#X pipe:%d", s32Ret,
				        ctx->vi[i].u32PipeId);
				g_ProExitReturnValue = RK_FALSE;
				goto __VI_INITFAIL;
			}
		}
	}

	/* init venc */
	ctx->venc.s32ChnId = 0;
	ctx->venc.u32Width = u32VideoWidth;
	ctx->venc.u32Height = u32VideoHeight;
	ctx->venc.u32Gop = 50;
	ctx->venc.u32BitRate = u32BitRate;
	ctx->venc.enCodecType = enCodecType;
	ctx->venc.enRcMode = enRcMode;
	ctx->venc.enable_buf_share = 1;
	ctx->venc.getStreamCbFunc = venc_get_stream;
	ctx->venc.dstFilePath = pOutPathVenc;
	/*
	H264  66：Baseline  77：Main Profile 100：High Profile
	H265  0：Main Profile  1：Main 10 Profile
	MJPEG 0：Baseline
	*/
	if (RK_CODEC_TYPE_H264 != enCodecType) {
		ctx->venc.stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc.stChnAttr.stVencAttr.u32Profile = 100;
	}
	/* VENC_GOPMODE_SMARTP */
	ctx->venc.stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc);

	/* vi handle thread launch */
	pthread_create(&vi_handle_thread_id, RK_NULL, handle_vi_stream, (void *)ctx);

	RK_LOGD("multi camerm eptz init finish");
	while (!gModeTest->bIfMainThreadQuit) {
		sleep(1);
	}
	RK_LOGD("multi camerm eptz exit");

	/* venc deinit */
	gModeTest->bIfVencThreadQuit = RK_TRUE;
	pthread_join(ctx->venc.getStreamThread, RK_NULL);
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);

	/* rtsp deinit */
	rtsp_deinit();

	/* Vi thread exit */
	gModeTest->bIfViHandleThreadQuit = RK_TRUE;
	pthread_join(vi_handle_thread_id, RK_NULL);

__VI_INITFAIL:
	/* vi deinit */
	if (pstIspParam->bIfIspGroupInit == RK_TRUE) { /* isp group init */
		for (i = 0; i < u32CamNum; i++) {
			s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_StopPipe failure:$#X pipe:%d", s32Ret,
				        ctx->vi[i].u32PipeId);
				g_ProExitReturnValue = RK_FALSE;
			}
		}
	}
	for (i = 0; i < u32CamNum; i++) {
		SAMPLE_COMM_VI_DestroyChn(&ctx->vi[i]);
	}

__FAILED:
	RK_MPI_SYS_Exit();
	if (pIqFileDir) {
#if (defined RKAIQ) && (defined UAPI2)
		isp_deinit(pstIspParam);
#endif
	}

__FAILED2:
	globol_param_deinit();
	if (pstIspParam) {
		free(pstIspParam);
		pstIspParam = RK_NULL;
	}
	if (ctx) {
		free(ctx);
		ctx = RK_NULL;
	}

__PARAM_INIT_FAILED:
	return g_ProExitReturnValue;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
