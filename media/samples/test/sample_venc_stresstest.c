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

#define BUFFER_SIZE 255
#define MODULE_TEST_DELAY_SECOND_TIME 3 /* (unit: second) */

typedef struct _rkModeTest {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfVencThreadQuit;
	RK_BOOL bModuleTestThreadQuit;
	RK_BOOL bModuleTestIfopen;
	RK_S32 s32ModuleTestType;
	RK_S32 s32ModuleTestLoop;
	RK_U32 u32TestFrameCount;
	RK_U32 u32VencGetFrameCount;
} g_mode_test;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
	SAMPLE_VENC_CTX_S venc;
} SAMPLE_MPI_CTX_S;

/* global param */
g_mode_test *gModeTest;
SAMPLE_MPI_CTX_S *ctx;
RK_S32 g_exit_result = RK_SUCCESS;
sem_t g_sem_module_test;
pthread_mutex_t g_frame_count_mutex;
RK_BOOL g_rtsp_ifenbale = RK_FALSE;
rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session;

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

static RK_CHAR optstr[] = "?::a::w:h:o:l:m:e:r:f:t:c:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"output_path", required_argument, NULL, 'o'},
    {"loop_count", required_argument, NULL, 'l'},
    {"mode_test_type", required_argument, NULL, 'm'},
    {"encode", required_argument, NULL, 'e'},
    {"wrap", required_argument, NULL, 'r'},
    {"fps", required_argument, NULL, 'f'},
    {"mode_test_loop", required_argument, NULL, 't' + 'l'},
    {"test_frame_count", required_argument, NULL, 'c'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -w 1920 -h 1080 -a /etc/iqfiles/ -l 10 -o /userdata/\n", name);
#ifdef RKAIQ
	printf(
	    "\t-a | --aiq : enable aiq with dirpath provided, eg:-a /etc/iqfiles/, \n"
	    "\t		set dirpath empty to using path by default, without this option aiq \n"
	    "\t		should run in other application\n");
#endif
	printf("\t-w | --width : camera with, Default: 1920\n");
	printf("\t-h | --height : camera height, Default: 1080\n");
	printf("\t-o | --output_path : encode output file path, Default: NULL\n");
	printf("\t-l | --loop_count : loop count, Default: -1\n");
	printf("\t-m | --mode_test_type : test type, 0:none, 1:encode resolution switch, \n"
	       "\t		2:encode type switch, 3:smartp switch test, 4:SVC mode switch, \n"
	       "\t		5:motion deblur test, 6:force IDR test, 7: venc chn Rotation test, "
	       "Default: 0\n");
	printf("\t-e | --encode : set encode type, Value: h264cbr, h264vbr, h265cbr, "
	       "h265vbr, default: h264cbr \n");
	printf("\t-r | --wrap : wrap for Vi and Venc, 0: close 1: open, Default: 0\n");
	printf("\t-f | --fps : encode output frame rate, Default: 25\n");
	printf("\t--mode_test_loop : module test loop, default: -1\n");
	printf("\t--test_frame_count : set the venc reveive frame count for every test "
	       "loop, default: 500\n");
}

static void *venc_get_stream(void *pArgs) {

	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 loopCount = 0;
	RK_VOID *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/vi_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
	}
	while (!gModeTest->bIfVencThreadQuit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {

			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					program_normal_exit(__func__, __LINE__);
					break;
				}
			}

			if (fp) {
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}

			if (g_rtsp_ifenbale) {
				rtsp_tx_video(g_rtsp_session, pData, ctx->stFrame.pstPack->u32Len,
				              ctx->stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
			} else {
				RK_LOGE("venc get_stream count: %d", loopCount);
			}

			if (gModeTest->bModuleTestIfopen) {
				pthread_mutex_lock(&g_frame_count_mutex);
				gModeTest->u32VencGetFrameCount++;
				pthread_mutex_unlock(&g_frame_count_mutex);

				if (gModeTest->u32VencGetFrameCount == gModeTest->u32TestFrameCount) {
					sem_post(&g_sem_module_test);
				}
			}
			if (ctx->enCodecType == RK_CODEC_TYPE_H264) {
				RK_LOGD("venc get_stream count: %d pack_type:%d", loopCount,
				        ctx->stFrame.pstPack->DataType.enH264EType);
			} else if (ctx->enCodecType == RK_CODEC_TYPE_H265) {
				RK_LOGD("venc get_stream count: %d pack_type:%d", loopCount,
				        ctx->stFrame.pstPack->DataType.enH265EType);
			} else {
				RK_LOGD("venc get_stream count: %d", loopCount);
			}

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
	}

	if (fp)
		fclose(fp);

	RK_LOGE("venc_get_stream exit");
	return RK_NULL;
}

static void wait_module_test_switch_success(void) {
	pthread_mutex_lock(&g_frame_count_mutex);
	gModeTest->u32VencGetFrameCount = 0;
	pthread_mutex_unlock(&g_frame_count_mutex);
	sem_wait(&g_sem_module_test);
}

static void venc_resolution_switch(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32DstWidth = 704;
	RK_U32 u32DstHeight = 576;
	VENC_CHN_ATTR_S pstChnAttr;
	VI_CHN_ATTR_S vipstChnAttr;
	MPP_CHN_S stSrcChn, stDestChn;

	while (!gModeTest->bModuleTestThreadQuit) {

		// unBind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			break;
		}

		memset(&pstChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
		s32Ret = RK_MPI_VENC_GetChnAttr(ctx->venc.s32ChnId, &pstChnAttr);
		RK_LOGD("w: %d h: %d", pstChnAttr.stVencAttr.u32PicWidth,
		        pstChnAttr.stVencAttr.u32PicHeight);
		if (pstChnAttr.stVencAttr.u32PicWidth == ctx->venc.u32Width) {
			pstChnAttr.stVencAttr.u32PicWidth = u32DstWidth;
			pstChnAttr.stVencAttr.u32PicHeight = u32DstHeight;
			pstChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(u32DstWidth);
			pstChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(u32DstHeight);
		} else {
			pstChnAttr.stVencAttr.u32PicWidth = ctx->venc.u32Width;
			pstChnAttr.stVencAttr.u32PicHeight = ctx->venc.u32Height;
			pstChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(ctx->venc.u32Width);
			pstChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(ctx->venc.u32Height);
		}

		s32Ret |= RK_MPI_VENC_SetChnAttr(ctx->venc.s32ChnId, &pstChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("venc set chn resolution failure");
			program_handle_error(__func__, __LINE__);
			break;
		}

		memset(&vipstChnAttr, 0, sizeof(VI_CHN_ATTR_S));
		s32Ret = RK_MPI_VI_GetChnAttr(ctx->vi.u32PipeId, ctx->vi.s32ChnId, &vipstChnAttr);
		RK_LOGD("w: %d h: %d", vipstChnAttr.stSize.u32Width,
		        vipstChnAttr.stSize.u32Height);
		if (vipstChnAttr.stSize.u32Width == ctx->vi.u32Width) {
			vipstChnAttr.stSize.u32Width = u32DstWidth;
			vipstChnAttr.stSize.u32Height = u32DstHeight;
		} else {
			vipstChnAttr.stSize.u32Width = ctx->vi.u32Width;
			vipstChnAttr.stSize.u32Height = ctx->vi.u32Height;
		}

		s32Ret |=
		    RK_MPI_VI_SetChnAttr(ctx->vi.u32PipeId, ctx->vi.s32ChnId, &vipstChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE(" set resolution failure");
			program_handle_error(__func__, __LINE__);
			break;
		}

		// Bind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			break;
		}

		RK_LOGE("------------------------Venc resolution switch to %dx%d",
		        vipstChnAttr.stSize.u32Width, vipstChnAttr.stSize.u32Height);
		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE(
		    "-------------------Venc resolution Switch Test success Total: %d Now Count: "
		    "%d-------------------",
		    test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------Venc resolution switch test end(pass/success) "
			        "count: %d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("venc_resolution_switch exit");
}

static RK_S32 encode_destroy_and_restart(CODEC_TYPE_E enCodecType,
                                         VENC_RC_MODE_E enRcMode, RK_U32 u32Profile,
                                         RK_BOOL bIfSliceSplit) {
	RK_S32 s32Ret = RK_FAILURE;
	MPP_CHN_S stSrcChn, stDestChn;

	gModeTest->bIfVencThreadQuit = RK_TRUE;
	if (ctx->venc.getStreamCbFunc) {
		pthread_join(ctx->venc.getStreamThread, RK_NULL);
	}

	// unBind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure", ctx->vi.s32DevId,
		        ctx->vi.s32ChnId, ctx->venc.s32ChnId);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}
	// Destroy venc
	s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	ctx->venc.enCodecType = enCodecType;
	ctx->venc.enRcMode = enRcMode;
	ctx->venc.stChnAttr.stVencAttr.u32Profile = u32Profile;
	gModeTest->bIfVencThreadQuit = RK_FALSE;

	// Init VENC
	s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	// Bind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure", ctx->vi.s32DevId,
		        ctx->vi.s32ChnId, ctx->venc.s32ChnId);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}
	return s32Ret;
}

static RK_S32 encode_type_switch(RK_S32 test_loop) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 now_test_loop = 0;
	g_rtsp_ifenbale = RK_FALSE;

	while (!gModeTest->bModuleTestThreadQuit) {

		switch (now_test_loop % 2) {
		case 0: /* H264 CBR */
			RK_LOGE("-------------Switch To H264CBR---------------");

			s32Ret = encode_destroy_and_restart(RK_CODEC_TYPE_H264, VENC_RC_MODE_H264CBR,
			                                    100, RK_FALSE);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("switch to 264_cbr failure");
				program_handle_error(__func__, __LINE__);
				return RK_FAILURE;
			}
			break;
		case 1: /* H265 CBR */
			RK_LOGE("-------------Switch To H265CBR---------------");

			s32Ret = encode_destroy_and_restart(RK_CODEC_TYPE_H265, VENC_RC_MODE_H265CBR,
			                                    0, RK_FALSE);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("switch to 265_cbr failure");
				program_handle_error(__func__, __LINE__);
				return RK_FAILURE;
			}
			break;
		default:
			break;
		}

		wait_module_test_switch_success();

		now_test_loop++;
		RK_LOGE("---------------------------------------switch success total:%d "
		        "time:%d--------------------------------",
		        test_loop, now_test_loop);
		if (test_loop > 0 && now_test_loop > test_loop) {
			RK_LOGE("encode_type_switch_test end and passed(success)!!!");
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}

	RK_LOGE("encode_type_switch exit");
	return s32Ret;
}

static RK_S32 smartp_mode(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	RK_BOOL eSmartpIfEnable = RK_TRUE;
	MPP_CHN_S stSrcChn, stDestChn;

	while (!gModeTest->bModuleTestThreadQuit) {

		gModeTest->bIfVencThreadQuit = RK_TRUE;
		if (ctx->venc.getStreamCbFunc) {
			pthread_join(ctx->venc.getStreamThread, RK_NULL);
		}

		// unBind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
		// Destroy venc
		s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
		if (eSmartpIfEnable) {
			ctx->venc.stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
			ctx->venc.stChnAttr.stGopAttr.s32VirIdrLen = ctx->venc.u32Gop / 2;
			RK_LOGE("------------------GopMode set to VENC_GOPMODE_SMARTP");
		} else {
			ctx->venc.stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
			RK_LOGE("------------------GopMode set to VENC_GOPMODE_NORMALP");
		}
		gModeTest->bIfVencThreadQuit = RK_FALSE;
		eSmartpIfEnable = !eSmartpIfEnable;

		// Init VENC
		s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		// Bind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE(
		    "---------------------------------------Smartp mode switch success total:%d "
		    "time:%d--------------------------------",
		    test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount > test_loop) {
			RK_LOGE("Smartp mode switch test end and passed(success)!!!");
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}

	RK_LOGE("smartp_mode exit");
	return RK_SUCCESS;
}

static RK_S32 smart_encode(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	RK_BOOL eSvcIfEnable = RK_TRUE;
	MPP_CHN_S stSrcChn, stDestChn;

	while (!gModeTest->bModuleTestThreadQuit) {
		gModeTest->bIfVencThreadQuit = RK_TRUE;
		if (ctx->venc.getStreamCbFunc) {
			pthread_join(ctx->venc.getStreamThread, RK_NULL);
		}

		// unBind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		// Destroy venc
		s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		if (eSvcIfEnable) {
			ctx->venc.bSvcIfEnable = eSvcIfEnable;
			if (ctx->venc.enCodecType == RK_CODEC_TYPE_H265) {
				ctx->venc.enRcMode = VENC_RC_MODE_H265VBR;
			} else if (ctx->venc.enCodecType == RK_CODEC_TYPE_H264) {
				ctx->venc.enRcMode = VENC_RC_MODE_H264VBR;
			}
			RK_LOGE("---------------------Smart video coding enable");
		} else {
			ctx->venc.bSvcIfEnable = eSvcIfEnable;
			RK_LOGE("---------------------Smart video coding disable");
		}

		eSvcIfEnable = !eSvcIfEnable;
		gModeTest->bIfVencThreadQuit = RK_FALSE;

		// Init VENC
		s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		// Bind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------Smart Video Coding Switch Test success Total: %d Now "
		        "Count: "
		        "%d-------------------",
		        test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------Smart Video Coding Switch test end(pass/success) "
			        "count: "
			        "%d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("smart_encode exit");
	return RK_SUCCESS;
}

static RK_S32 motion_deblur_test(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	RK_BOOL eMotionDeblurIfEnable = RK_TRUE;
	MPP_CHN_S stSrcChn, stDestChn;

	while (!gModeTest->bModuleTestThreadQuit) {

		gModeTest->bIfVencThreadQuit = RK_TRUE;
		if (ctx->venc.getStreamCbFunc) {
			pthread_join(ctx->venc.getStreamThread, RK_NULL);
		}

		// unBind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}
		// Destroy venc
		s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		if (eMotionDeblurIfEnable) {
			ctx->venc.bMotionDeblurIfEnable = eMotionDeblurIfEnable;
			RK_LOGE("------------------------------Motion Deblur enable");
		} else {
			ctx->venc.bMotionDeblurIfEnable = eMotionDeblurIfEnable;
			RK_LOGE("------------------------------Motion Deblur disable");
		}
		eMotionDeblurIfEnable = !eMotionDeblurIfEnable;
		gModeTest->bIfVencThreadQuit = RK_FALSE;

		// Init VENC
		s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		// Bind vi and venc
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VENC;
		stDestChn.s32DevId = 0;
		stDestChn.s32ChnId = ctx->venc.s32ChnId;
		s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure",
			        ctx->vi.s32DevId, ctx->vi.s32ChnId, ctx->venc.s32ChnId);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------Motion Deblur Test success Total: %d Now Count: "
		        "%d-------------------",
		        test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------Motion Deblur test end(pass/success) count: "
			        "%d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("motion_deblur_test exit");
	return RK_SUCCESS;
}

static RK_S32 venc_force_idr_test(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	while (!gModeTest->bModuleTestThreadQuit) {

		RK_LOGE("-------------------venc set force idr");
		s32Ret = RK_MPI_VENC_RequestIDR(ctx->venc.s32ChnId, RK_FALSE);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_RequestIDR failure: %X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return s32Ret;
		}

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------Venc Force IDR Test success Total: %d Now Count: "
		        "%d-------------------",
		        test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------Venc Force IDR test end(pass/success) count: "
			        "%d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("venc_force_idr exit");
	return RK_SUCCESS;
}

static RK_S32 venc_set_rotation(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	ROTATION_E enRotation;

	while (!gModeTest->bModuleTestThreadQuit) {

		memset(&enRotation, 0, sizeof(ROTATION_E));
		s32Ret = RK_MPI_VENC_GetChnRotation(ctx->venc.s32ChnId, &enRotation);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_GetChnRotation failure:%X", s32Ret);
			return RK_FAILURE;
		}
		if (enRotation == ROTATION_0) {
			enRotation = ROTATION_90;
		} else if (enRotation == ROTATION_90) {
			enRotation = ROTATION_180;
		} else if (enRotation == ROTATION_180) {
			enRotation = ROTATION_270;
		} else if (enRotation == ROTATION_270) {
			enRotation = ROTATION_0;
		} else {
			enRotation = ROTATION_0;
		}
		s32Ret = RK_MPI_VENC_SetChnRotation(ctx->venc.s32ChnId, enRotation);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_SetChnRotation failure:%X", s32Ret);
			return RK_FAILURE;
		}

		RK_LOGE("----------------------venc chn Rotation switch to:%d", enRotation);
		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE(
		    "-------------------Venc chn Rotation switch success Total: %d Now Count: "
		    "%d-------------------",
		    test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------Venc chn Rotation test end(pass/success) count: "
			        "%d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}

	RK_LOGE("venc_set_rotation exit");
	return RK_SUCCESS;
}

static void *sample_venc_stress_test(void *pArgs) {

	prctl(PR_SET_NAME, "venc_stress_test");
	sleep(MODULE_TEST_DELAY_SECOND_TIME);

	SAMPLE_COMM_DumpMeminfo("Enter sample_venc_stress_test",
	                        gModeTest->s32ModuleTestType);
	switch (gModeTest->s32ModuleTestType) {
	case 1:
		/* venc resolutio switch */
		venc_resolution_switch(gModeTest->s32ModuleTestLoop);
		break;
	case 2:
		/* encode type switch */
		encode_type_switch(gModeTest->s32ModuleTestLoop);
		break;
	case 3:
		/* smartp test*/
		smartp_mode(gModeTest->s32ModuleTestLoop);
		break;
	case 4:
		/* smart video coding*/
		smart_encode(gModeTest->s32ModuleTestLoop);
		break;
	case 5:
		/*motion deblur test*/
		motion_deblur_test(gModeTest->s32ModuleTestLoop);
		break;
	case 6:
		/*force idr test*/
		venc_force_idr_test(gModeTest->s32ModuleTestLoop);
		break;
	case 7:
		/*	venc set chn rotation*/
		venc_set_rotation(gModeTest->s32ModuleTestLoop);
		break;
	default:
		RK_LOGE("mode test type:%d is unsupported", gModeTest->s32ModuleTestType);
	}

	SAMPLE_COMM_DumpMeminfo("Exit sample_venc_stress_test", gModeTest->s32ModuleTestType);
	RK_LOGE("venc_stress_test exit");
	return RK_NULL;
}

static RK_S32 rtsp_init(CODEC_TYPE_E enCodecType) {

	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
	if (enCodecType == RK_CODEC_TYPE_H264) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	} else if (enCodecType == RK_CODEC_TYPE_H265) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	} else {
		RK_LOGE("not support other type\n");
		g_rtsp_ifenbale = RK_FALSE;
		return RK_SUCCESS;
	}
	rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());

	g_rtsp_ifenbale = RK_TRUE;
	return RK_SUCCESS;
}

static RK_S32 rtsp_deinit(void) {
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	return RK_SUCCESS;
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

	gModeTest->s32ModuleTestLoop = -1;
	gModeTest->u32TestFrameCount = 500;

	sem_init(&g_sem_module_test, 0, 0);

	if (pthread_mutex_init(&g_frame_count_mutex, NULL) != 0) {
		RK_LOGE("mutex init failure \n");
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
	sem_destroy(&g_sem_module_test);
	pthread_mutex_destroy(&g_frame_count_mutex);
	return RK_SUCCESS;
}

int main(int argc, char *argv[]) {

	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32VideoWidth = 1920;
	RK_S32 s32VideoHeight = 1080;
	RK_CHAR *pOutPathVenc = RK_NULL;
	RK_CHAR *pIqFileDir = RK_NULL;
	RK_S32 s32CamId = 0;
	RK_S32 s32LoopCnt = -1;
	RK_U32 u32VencFps = 25;
	RK_U32 u32BitRate = 4 * 1024;
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;
	RK_BOOL bMultictx = RK_FALSE;
	RK_BOOL bWrapIfEnable = RK_FALSE;
	MPP_CHN_S stSrcChn, stDestChn;
	rk_aiq_working_mode_t eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;

	pthread_t modeTest_thread_id;

	if (argc < 2) {
		print_usage(argv[0]);
		g_exit_result = RK_FAILURE;
		goto __PARAM_INIT_FAILED;
	}

	s32Ret = global_param_init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("global_param_init failure");
		g_exit_result = RK_FAILURE;
		goto __PARAM_INIT_FAILED;
	}

	signal(SIGINT, sigterm_handler);

	RK_S32 c = 0;
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		const char *tmp_optarg = optarg;
		switch (c) {
		case 'a':
			if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
				tmp_optarg = argv[optind++];
			}
			if (tmp_optarg) {
				pIqFileDir = (char *)tmp_optarg;
			} else {
				pIqFileDir = NULL;
			}
			break;
		case 'w':
			s32VideoWidth = atoi(optarg);
			break;
		case 'h':
			s32VideoHeight = atoi(optarg);
			break;
		case 'o':
			pOutPathVenc = optarg;
			break;
		case 'l':
			s32LoopCnt = atoi(optarg);
			break;
		case 'm':
			gModeTest->s32ModuleTestType = atoi(optarg);
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
				g_exit_result = RK_FAILURE;
				goto __PARAM_INIT_FAILED;
			}
			break;
		case 'r':
			bWrapIfEnable = atoi(optarg);
			break;
		case 'f':
			u32VencFps = atoi(optarg);
			break;
		case 't' + 'l':
			gModeTest->s32ModuleTestLoop = atoi(optarg);
			break;
		case 'c':
			gModeTest->u32TestFrameCount = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	if (pIqFileDir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", pIqFileDir);
		printf("#bMultictx: %d\n\n", bMultictx);
		RK_LOGE("eHdrMode: %d", eHdrMode);

		s32Ret = SAMPLE_COMM_ISP_Init(s32CamId, eHdrMode, bMultictx, pIqFileDir);
		s32Ret |= SAMPLE_COMM_ISP_Run(s32CamId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("ISP init failure");
			g_exit_result = RK_FAILURE;
			goto __FAILED2;
		}
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_Init failure");
		g_exit_result = RK_FAILURE;
		goto __FAILED;
	}

	s32Ret = rtsp_init(enCodecType);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rtsp_init failure");
		g_exit_result = RK_FAILURE;
		goto __FAILED;
	}

	/* Init VI */
	ctx->vi.u32Width = s32VideoWidth;
	ctx->vi.u32Height = s32VideoHeight;
	ctx->vi.s32DevId = s32CamId;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = 0;
	ctx->vi.stChnAttr.stIspOpt.stMaxSize.u32Width = s32VideoWidth;
	ctx->vi.stChnAttr.stIspOpt.stMaxSize.u32Height = s32VideoHeight;
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 3;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 1;
	ctx->vi.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi.stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = 25;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = u32VencFps;
	ctx->vi.bWrapIfEnable = bWrapIfEnable;
	ctx->vi.u32BufferLine = ctx->vi.u32Height / 4;
	s32Ret = SAMPLE_COMM_VI_CreateChn(&ctx->vi);
	if (s32Ret != RK_SUCCESS) {
		g_exit_result = RK_FAILURE;
		RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%d", s32Ret);
		goto __FAILED;
	}

	/* Init VENC */
	ctx->venc.s32ChnId = 0;
	ctx->venc.u32Width = s32VideoWidth;
	ctx->venc.u32Height = s32VideoHeight;
	ctx->venc.u32Fps = u32VencFps;
	ctx->venc.u32Gop = 50;
	ctx->venc.u32BitRate = u32BitRate;
	ctx->venc.enCodecType = enCodecType;
	ctx->venc.enRcMode = enRcMode;
	ctx->venc.getStreamCbFunc = venc_get_stream;
	ctx->venc.s32loopCount = s32LoopCnt;
	ctx->venc.dstFilePath = pOutPathVenc;
	ctx->venc.bWrapIfEnable = bWrapIfEnable;
	ctx->venc.u32BufferLine = ctx->venc.u32Height / 4;
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

	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestIfopen = RK_TRUE;
		pthread_create(&modeTest_thread_id, 0, sample_venc_stress_test,
		               (void *)(gModeTest));
	}

	/* VI bind VENC */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
		g_exit_result = RK_FAILURE;
	}

	printf("%s initial finish\n", __func__);

	while (!gModeTest->bIfMainThreadQuit) {
		sleep(1);
	}

	/* mode_test_deinit */
	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestThreadQuit = RK_TRUE;
		pthread_join(modeTest_thread_id, NULL);
	}

	/* Venc deinit */
	gModeTest->bIfVencThreadQuit = RK_TRUE;
	pthread_join(ctx->venc.getStreamThread, NULL);

	/* VI unbind VENC */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}
	/* destroy venc */
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);

	/* rtsp deinit */
	rtsp_deinit();

	/* Destroy VI */
	s32Ret = SAMPLE_COMM_VI_DestroyChn(&ctx->vi);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VI_DestroyChn failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}

__FAILED:
	RK_MPI_SYS_Exit();
	if (pIqFileDir) {
#ifdef RKAIQ
		SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}
__FAILED2:
	global_param_deinit();

__PARAM_INIT_FAILED:
	return g_exit_result;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
