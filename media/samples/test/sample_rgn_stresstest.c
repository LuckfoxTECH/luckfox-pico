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
#define RGN_CHN_MAX 7

typedef struct _rkModeTest {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfVencThreadQuit;
	RK_BOOL bModuleTestThreadQuit;
	RK_BOOL bModuleTestIfopen;
	RK_BOOL bWrapIfEnable;
	RK_S32 s32ModuleTestType;
	RK_S32 s32ModuleTestLoop;
	RK_U32 u32TestFrameCount;
	RK_U32 u32VencGetFrameCount;
	RK_CHAR *inputBmp1Path;
	RK_CHAR *inputBmp2Path;
} g_mode_test;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
	SAMPLE_VENC_CTX_S venc;
	SAMPLE_RGN_CTX_S rgn[RGN_CHN_MAX];
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

static RK_CHAR optstr[] = "?::a::w:h:o:l:m:e:r:f:t:c:i:I:";
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
    {"inputBmp1Path", required_argument, NULL, 'i'},
    {"inputBmp2Path", required_argument, NULL, 'I'},
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
	printf("\t%s -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 --inputBmp1Path "
	       "/userdata/160x96.bmp --inputBmp2Path /userdata/192x96.bmp -o /userdata/\n",
	       name);
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
	printf("\t-i | --inputBmp1Path : input bmp file path. default: NULL\n");
	printf("\t-I | --inputBmp2Path : input bmp file path. default: NULL\n");
	printf("\t-m | --mode_test_type : test type, 0:none, 1:rgn attach and detach. "
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

			RK_LOGD("venc get_stream count: %d", loopCount);

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

static RK_S32 get_bmp_resolution(RK_CHAR *pBmpFile, RK_U32 *width, RK_U32 *height) {

	FILE *fp = RK_NULL;
	RK_U16 bfType;
	OSD_BITMAPINFO pBmpInfo;

	if (pBmpFile == RK_NULL) {
		RK_LOGE("bmp file not exist");
		return RK_FAILURE;
	}

	fp = fopen(pBmpFile, "rb");
	if (fp == RK_NULL) {
		RK_LOGE("open file:%s failure", pBmpFile);
		return RK_FAILURE;
	}

	(void)fread(&bfType, 1, sizeof(bfType), fp);
	if (bfType != 0x4d42) {
		RK_LOGE("is not bmp file");
		fclose(fp);
		return RK_FAILURE;
	}
	fseek(fp, sizeof(OSD_BITMAPFILEHEADER), SEEK_CUR);
	(void)fread(&pBmpInfo, 1, sizeof(OSD_BITMAPINFO), fp);

	*width = pBmpInfo.bmiHeader.biWidth;
	*height = pBmpInfo.bmiHeader.biHeight;
	RK_LOGE("get_bmp_resolution w:%d  h:%d", *width, *height);

	if (fp) {
		fclose(fp);
		fp = RK_NULL;
	}

	return RK_SUCCESS;
}

static RK_S32 rgn_init(void) {

	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 0;
	RK_U32 u32Height = 0;

	/* cover for vi*/
	ctx->rgn[0].rgnHandle = 0;
	ctx->rgn[0].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[0].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[0].stMppChn.s32ChnId = VI_MAX_CHN_NUM;
	ctx->rgn[0].stMppChn.s32DevId = ctx->vi.s32DevId;
	ctx->rgn[0].stRegion.s32X = 0;        /* must be 2 aligned */
	ctx->rgn[0].stRegion.s32Y = 0;        /* must be 2 aligned */
	ctx->rgn[0].stRegion.u32Width = 128;  /* must be 8 aligned */
	ctx->rgn[0].stRegion.u32Height = 128; /* must be 8 aligned */
	ctx->rgn[0].u32Color = 0xFF0000;
	ctx->rgn[0].u32BgAlpha = 128;
	ctx->rgn[0].u32FgAlpha = 128;
	ctx->rgn[0].u32Layer = 0;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[0]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[0].rgnHandle);
		return s32Ret;
	}

	/* cover for vi */
	ctx->rgn[1].rgnHandle = 1;
	ctx->rgn[1].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[1].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[1].stMppChn.s32ChnId = VI_MAX_CHN_NUM;
	ctx->rgn[1].stMppChn.s32DevId = ctx->vi.s32DevId;
	ctx->rgn[1].stRegion.s32X = 0;               /* must be 2 aligned */
	ctx->rgn[1].stRegion.s32Y = RK_ALIGN_2(256); /* must be 2 aligned */
	ctx->rgn[1].stRegion.u32Width = 128;         /* must be 8 aligned */
	ctx->rgn[1].stRegion.u32Height = 128;        /* must be 8 aligned */
	ctx->rgn[1].u32Color = 0xFFFF00;
	ctx->rgn[1].u32BgAlpha = 128;
	ctx->rgn[1].u32FgAlpha = 128;
	ctx->rgn[1].u32Layer = 1;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[1]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[1].rgnHandle);
		return s32Ret;
	}

	/* mosaic for vi*/
	ctx->rgn[2].rgnHandle = 2;
	ctx->rgn[2].stRgnAttr.enType = MOSAIC_RGN;
	ctx->rgn[2].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[2].stMppChn.s32ChnId = VI_MAX_CHN_NUM;
	ctx->rgn[2].stMppChn.s32DevId = ctx->vi.s32DevId;
	ctx->rgn[2].stRegion.s32X =
	    RK_ALIGN_2(ctx->vi.u32Width - 128); /* must be 2 aligned */
	ctx->rgn[2].stRegion.s32Y = 0;          /* must be 2 aligned */
	ctx->rgn[2].stRegion.u32Width = 128;    /* must be 8 aligned */
	ctx->rgn[2].stRegion.u32Height = 128;   /* must be 8 aligned */
	ctx->rgn[2].u32BgAlpha = 128;
	ctx->rgn[2].u32FgAlpha = 128;
	ctx->rgn[2].u32Layer = 2;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[2]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[2].rgnHandle);
		return s32Ret;
	}

	/* mosaic for vi*/
	ctx->rgn[3].rgnHandle = 3;
	ctx->rgn[3].stRgnAttr.enType = MOSAIC_RGN;
	ctx->rgn[3].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[3].stMppChn.s32ChnId = VI_MAX_CHN_NUM;
	ctx->rgn[3].stMppChn.s32DevId = ctx->vi.s32DevId;
	ctx->rgn[3].stRegion.s32X =
	    RK_ALIGN_2(ctx->vi.u32Width - 128);      /* must be 2 aligned */
	ctx->rgn[3].stRegion.s32Y = RK_ALIGN_2(256); /* must be 2 aligned */
	ctx->rgn[3].stRegion.u32Width = 128;         /* must be 8 aligned */
	ctx->rgn[3].stRegion.u32Height = 128;        /* must be 8 aligned */
	ctx->rgn[3].u32BgAlpha = 128;
	ctx->rgn[3].u32FgAlpha = 128;
	ctx->rgn[3].u32Layer = 3;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[3]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[3].rgnHandle);
		return s32Ret;
	}

	// overlay for venc
	s32Ret = get_bmp_resolution(gModeTest->inputBmp1Path, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("get_bmp_resolution failure");
		return RK_FAILURE;
	}
	ctx->rgn[4].rgnHandle = 4;
	ctx->rgn[4].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[4].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[4].stMppChn.s32ChnId = ctx->venc.s32ChnId;
	ctx->rgn[4].stMppChn.s32DevId = 0;
	ctx->rgn[4].stRegion.s32X = RK_ALIGN_2(ctx->venc.u32Width / 2); // must be 2 aligned
	ctx->rgn[4].stRegion.s32Y = 0;                                  // must be 2 aligned
	ctx->rgn[4].stRegion.u32Width = u32Width;                       // must be 8 aligned
	ctx->rgn[4].stRegion.u32Height = u32Height;                     // must be 8 aligned
	ctx->rgn[4].u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn[4].u32BgAlpha = 128;
	ctx->rgn[4].u32FgAlpha = 128;
	ctx->rgn[4].u32Layer = 4;
	ctx->rgn[4].srcFileBmpName = gModeTest->inputBmp1Path;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[4]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[4].rgnHandle);
		return s32Ret;
	}

	// overlay for venc
	s32Ret = get_bmp_resolution(gModeTest->inputBmp2Path, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("get_bmp_resolution failure");
		return RK_FAILURE;
	}
	ctx->rgn[5].rgnHandle = 5;
	ctx->rgn[5].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[5].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[5].stMppChn.s32ChnId = ctx->venc.s32ChnId;
	ctx->rgn[5].stMppChn.s32DevId = 0;
	ctx->rgn[5].stRegion.s32X = RK_ALIGN_2(ctx->venc.u32Width / 2);  // must be 2 aligned
	ctx->rgn[5].stRegion.s32Y = RK_ALIGN_2(ctx->venc.u32Height / 2); // must be 2 aligned
	ctx->rgn[5].stRegion.u32Width = u32Width;                        // must be 8 aligned
	ctx->rgn[5].stRegion.u32Height = u32Height;                      // must be 8 aligned
	ctx->rgn[5].u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn[5].u32BgAlpha = 128;
	ctx->rgn[5].u32FgAlpha = 128;
	ctx->rgn[5].u32Layer = 5;
	ctx->rgn[5].srcFileBmpName = gModeTest->inputBmp2Path;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[5]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[5].rgnHandle);
		return s32Ret;
	}

	// overlay for venc
	s32Ret = get_bmp_resolution(gModeTest->inputBmp2Path, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("get_bmp_resolution failure");
		return RK_FAILURE;
	}
	ctx->rgn[6].rgnHandle = 6;
	ctx->rgn[6].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[6].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[6].stMppChn.s32ChnId = ctx->venc.s32ChnId;
	ctx->rgn[6].stMppChn.s32DevId = 0;
	ctx->rgn[6].stRegion.s32X = RK_ALIGN_2(ctx->venc.u32Width / 2); // must be 2 aligned
	ctx->rgn[6].stRegion.s32Y =
	    RK_ALIGN_2(ctx->venc.u32Height - u32Height); // must be 2 aligned
	ctx->rgn[6].stRegion.u32Width = u32Width;        // must be 8 aligned
	ctx->rgn[6].stRegion.u32Height = u32Height;      // must be 8 aligned
	ctx->rgn[6].u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn[6].u32BgAlpha = 128;
	ctx->rgn[6].u32FgAlpha = 128;
	ctx->rgn[6].u32Layer = 6;
	ctx->rgn[6].srcFileBmpName = gModeTest->inputBmp2Path;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[6]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[6].rgnHandle);
		return s32Ret;
	}

	return s32Ret;
}

static RK_S32 rgn_deinit(void) {
	RK_S32 s32Ret = RK_SUCCESS;
	for (RK_S32 i = 0; i < RGN_CHN_MAX; i++) {
		s32Ret = SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_DestroyChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[i].rgnHandle);
		}
	}
	return s32Ret;
}

static void rgn_attach_and_detach(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 i = 0;

	while (!gModeTest->bModuleTestThreadQuit) {

		for (i = 0; i < RGN_CHN_MAX; i++) {
			s32Ret = SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn[i]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_RGN_DestroyChn Failure s32Ret:%#X rgn handle:%d",
				        s32Ret, ctx->rgn[i].rgnHandle);
			}
		}

		// rgn init
		for (i = 0; i < RGN_CHN_MAX; i++) {
			s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[i]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_RGN_CreateChn Handle:%d Failure Ret:%#X",
				        ctx->rgn[i].rgnHandle, s32Ret);
			}
		}

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------rgn attach/detach Test success Total: %d Now Count: "
		        "%d-------------------",
		        test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------rgn attach/detach test end(pass/success) "
			        "count: %d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("rgn_attach_and_detach exit");
	return;
}

static void *sample_rgn_stress_test(void *pArgs) {

	prctl(PR_SET_NAME, "rgn_stress_test");
	sleep(MODULE_TEST_DELAY_SECOND_TIME);

	SAMPLE_COMM_DumpMeminfo("Enter sample_rgn_test", gModeTest->s32ModuleTestType);
	switch (gModeTest->s32ModuleTestType) {
	case 1:
		/* rgn attach/detach */
		rgn_attach_and_detach(gModeTest->s32ModuleTestLoop);
		break;
	default:
		RK_LOGE("mode test type:%d is unsupported", gModeTest->s32ModuleTestType);
	}

	SAMPLE_COMM_DumpMeminfo("Exit sample_rgn_test", gModeTest->s32ModuleTestType);
	RK_LOGE("rgn_stress_test exit");
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
			gModeTest->bWrapIfEnable = bWrapIfEnable;
			break;
		case 'f':
			u32VencFps = atoi(optarg);
			break;
		case 'i':
			gModeTest->inputBmp1Path = optarg;
			break;
		case 'I':
			gModeTest->inputBmp2Path = optarg;
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
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = -1;
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
	}

	/* Rgn Init*/
	rgn_init();

	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestIfopen = RK_TRUE;
		pthread_create(&modeTest_thread_id, 0, sample_rgn_stress_test,
		               (void *)(gModeTest));
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

	/* Rgn Deinit*/
	s32Ret = rgn_deinit();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rgn_deinit failure");
		program_handle_error(__func__, __LINE__);
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
