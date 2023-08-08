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
#define LDCH_MAX_LEVEL 255
#define MODULE_TEST_DELAY_SECOND_TIME 3 //(unit: second)

typedef struct _rkModeTest {
	pthread_t vi_thread_id;
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfViTHreadQuit;
	RK_BOOL bModuleTestThreadQuit;
	RK_BOOL bModuleTestIfopen;
	RK_BOOL bMultictx;
	RK_S32 s32ModuleTestType;
	RK_S32 s32ModuleTestLoop;
	RK_S32 s32CamId;
	RK_U32 u32TestFrameCount;
	RK_U32 u32ViGetFrameCount;
	rk_aiq_working_mode_t eHdrMode;
	RK_CHAR *pIqFileDir;
} g_mode_test;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
} SAMPLE_MPI_CTX_S;

/* global param */
g_mode_test *gModeTest;
SAMPLE_MPI_CTX_S *ctx;
RK_S32 g_exit_result = RK_SUCCESS;
sem_t g_sem_module_test;
pthread_mutex_t g_frame_count_mutex;

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

static RK_CHAR optstr[] = "?::a::d:f:w:h:o:I:l:p:m:c:i:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"device_name", required_argument, NULL, 'd'},
    {"pixel_format", optional_argument, NULL, 'p' + 'f'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"output_path", required_argument, NULL, 'o'},
    {"camid", required_argument, NULL, 'I'},
    {"loop_count", required_argument, NULL, 'l'},
    {"fps", required_argument, NULL, 'f'},
    {"hdr_mode", required_argument, NULL, 'h' + 'm'},
    {"mode_test_type", required_argument, NULL, 'm'},
    {"mode_test_loop", required_argument, NULL, 't' + 'l'},
    {"test_frame_count", required_argument, NULL, 'c'},
    {"chn_id", required_argument, NULL, 'c' + 'i'},
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
	printf("\t-a | --aiq : enable aiq with dirpath provided, eg:-a /etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
#endif
	printf("\t-d | --device_name : set pcDeviceName, eg: /dev/video0 Default: "
	       "NULL\n");
	printf("\t-w | --width : camera width, Default: 1920\n");
	printf("\t-h | --height : camera height, Default: 1080\n");
	printf("\t-o | --output_path : vi output file path, Default: NULL\n");
	printf("\t-I | --camid : camera id, Default: 0\n");
	printf("\t-l | --loop_count : loop count, Default: -1\n");
	printf("\t-f | --fps : isp output fps, Default: -1\n");
	printf("\t-m | --mode_test_type : test type, 0:none, 1:P/N switch test, 2:HDR switch "
	       "test, 3:frame rate switch test, 4: LDCH mode test. Default: 0\n");
	printf("\t--pixel_format : camera Format, Value:nv12,nv16,uyvy,rgb565,xbgr8888. "
	       "Default: "
	       "nv12\n");
	printf("\t--hdr_mode : set camera hdr mode, 0: normal, 1: HDR2. Default: "
	       "0\n");
	printf("\t--mode_test_loop : module test loop, default: -1\n");
	printf("\t--test_frame_count : set the venc reveive frame count for every test "
	       "loop, default: 500\n");
	printf("\t--chn_id : channel id, default: 0\n");
}

static void *vi_get_stream(void *pArgs) {

	SAMPLE_VI_CTX_S *ctx = (SAMPLE_VI_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 s32FrameSize = 0;
	RK_S32 loopCount = 0;
	RK_VOID *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/vi_%d.bin", ctx->dstFilePath, ctx->s32DevId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("chn %d can't open %s file !\n", ctx->s32DevId, ctx->dstFilePath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
	}

	while (!gModeTest->bIfViTHreadQuit) {
		s32Ret = SAMPLE_COMM_VI_GetChnFrame(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {

			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VI_ReleaseChnFrame(ctx);
					program_normal_exit(__func__, __LINE__);
					break;
				}
			}

			if (fp) {
				s32FrameSize = ctx->stViFrame.stVFrame.u32Height *
				               ctx->stViFrame.stVFrame.u32Width * 3 / 2;
				fwrite(pData, 1, s32FrameSize, fp);
				fflush(fp);
			}

			SAMPLE_COMM_VI_ReleaseChnFrame(ctx);
			loopCount++;

			if (gModeTest->bModuleTestIfopen) {

				pthread_mutex_lock(&g_frame_count_mutex);
				gModeTest->u32ViGetFrameCount++;
				pthread_mutex_unlock(&g_frame_count_mutex);

				if (gModeTest->u32ViGetFrameCount == gModeTest->u32TestFrameCount) {
					sem_post(&g_sem_module_test);
				}
			}
			RK_LOGE("vi get_stream count: %d", loopCount);
		}
	}

	if (fp)
		fclose(fp);

	return RK_NULL;
}

static void wait_module_test_switch_success(void) {

	pthread_mutex_lock(&g_frame_count_mutex);
	gModeTest->u32ViGetFrameCount = 0;
	pthread_mutex_unlock(&g_frame_count_mutex);
	sem_wait(&g_sem_module_test);
}

static void pn_mode_switch(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	RK_LOGE("s32CamId: %d hdr: %d", gModeTest->s32CamId, gModeTest->eHdrMode);

	while (!gModeTest->bModuleTestThreadQuit) {

		RK_MPI_VI_PauseChn(ctx->vi.u32PipeId, ctx->vi.s32ChnId);

		SAMPLE_COMM_ISP_Stop(gModeTest->s32CamId);

		s32Ret = SAMPLE_COMM_ISP_Init(gModeTest->s32CamId, gModeTest->eHdrMode,
		                              gModeTest->bMultictx, gModeTest->pIqFileDir);
		s32Ret |= SAMPLE_COMM_ISP_Run(gModeTest->s32CamId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init failure\n");
			program_handle_error(__func__, __LINE__);
			break;
		}

		RK_MPI_VI_ResumeChn(ctx->vi.u32PipeId, ctx->vi.s32ChnId);

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------PN Switch Test success Total: %d Now Count: "
		        "%d-------------------",
		        test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE(
			    "------------------PN test end(pass/success) count: %d-----------------",
			    s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("pn_mode_switch exit");
	return;
}

static void hdr_mode_switch_test(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;

	while (!gModeTest->bModuleTestThreadQuit) {
		RK_MPI_VI_PauseChn(ctx->vi.u32PipeId, ctx->vi.s32ChnId);

		SAMPLE_COMM_ISP_Stop(gModeTest->s32CamId);

		if (gModeTest->eHdrMode == RK_AIQ_WORKING_MODE_NORMAL) {
			gModeTest->eHdrMode = RK_AIQ_WORKING_MODE_ISP_HDR2;
		} else if (gModeTest->eHdrMode == RK_AIQ_WORKING_MODE_ISP_HDR2) {
			gModeTest->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
		} else {
			gModeTest->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
		}
		s32Ret = SAMPLE_COMM_ISP_Init(gModeTest->s32CamId, gModeTest->eHdrMode,
		                              gModeTest->bMultictx, gModeTest->pIqFileDir);
		s32Ret |= SAMPLE_COMM_ISP_Run(gModeTest->s32CamId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("ISP init failure\n");
			program_handle_error(__func__, __LINE__);
			break;
		}

		RK_MPI_VI_ResumeChn(ctx->vi.u32PipeId, ctx->vi.s32ChnId);

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------HDR Mode Switch Test success Total: %d Now Count: "
		        "%d-------------------",
		        test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------HDR Mode test end(pass/success) count: "
			        "%d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("hdr_mode_switch_test exit");
	return;
}

static void frameRate_switch_test(RK_S32 test_loop) {
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	VI_CHN_ATTR_S pstChnAttr;
	while (!gModeTest->bModuleTestThreadQuit) {

		memset(&pstChnAttr, 0, sizeof(VI_CHN_ATTR_S));
		s32Ret = RK_MPI_VI_GetChnAttr(ctx->vi.u32PipeId, ctx->vi.s32ChnId, &pstChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_GetChnAttr failure:%X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		if (pstChnAttr.stFrameRate.s32SrcFrameRate == -1) {
			pstChnAttr.stFrameRate.s32SrcFrameRate = 1;
		}

		pstChnAttr.stFrameRate.s32SrcFrameRate += 1;
		if (pstChnAttr.stFrameRate.s32SrcFrameRate > 25) {
			pstChnAttr.stFrameRate.s32SrcFrameRate = 1;
		}
		pstChnAttr.stFrameRate.s32DstFrameRate = pstChnAttr.stFrameRate.s32SrcFrameRate;

		s32Ret = RK_MPI_VI_SetChnAttr(ctx->vi.u32PipeId, ctx->vi.s32ChnId, &pstChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_GetChnAttr failure:%X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}
		RK_LOGE("Framerate switch to: %d", pstChnAttr.stFrameRate.s32DstFrameRate);

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------Framerate switch success Total: %d Now Count: "
		        "%d-------------------",
		        test_loop, s32TestCount);
		if (test_loop > 0 && s32TestCount >= test_loop) {
			RK_LOGE("------------------Framerate switch end(pass/success) count: "
			        "%d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("frameRate_switch_test exit");
	return;
}

static void ldch_mode_test(RK_S32 test_loop) {
	RK_BOOL bIfLDCHEnable = RK_FALSE;
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32LdchLevel = 0;
	RK_S32 test_count = 0;

	while (!gModeTest->bModuleTestThreadQuit) {

		s32Ret =
		    SAMPLE_COMM_ISP_SetLDCH(gModeTest->s32CamId, u32LdchLevel, bIfLDCHEnable);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_ISP_SetLDCH failure");
			program_handle_error(__func__, __LINE__);
			break;
		}

		if (bIfLDCHEnable) {
			u32LdchLevel++;
			if (u32LdchLevel > LDCH_MAX_LEVEL) {
				u32LdchLevel = 0;
			}
		}
		bIfLDCHEnable = !bIfLDCHEnable;
		wait_module_test_switch_success();
		test_count++;

		RK_LOGE("-----------------LDCH state: %d(0:close 1:open)  level:%d",
		        bIfLDCHEnable, u32LdchLevel);
		RK_LOGE("-----------------------------LDCH switch success total:%d now count:%d",
		        test_loop, test_count);
		if (test_loop > 0 && test_count >= test_loop) {
			RK_LOGE("--------------LDCH switch success test end total:%d", test_loop);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("ldch_mode_test exit");
	return;
}

static void *sample_isp_stress_test(void *pArgs) {

	prctl(PR_SET_NAME, "isp_stress_test");
	sleep(MODULE_TEST_DELAY_SECOND_TIME);

	SAMPLE_COMM_DumpMeminfo("Enter sample_isp_stress_test", gModeTest->s32ModuleTestType);
	switch (gModeTest->s32ModuleTestType) {
	case 1:
		pn_mode_switch(gModeTest->s32ModuleTestLoop);
		break;
	case 2:
		hdr_mode_switch_test(gModeTest->s32ModuleTestLoop);
		break;
	case 3:
		frameRate_switch_test(gModeTest->s32ModuleTestLoop);
		break;
	case 4:
		ldch_mode_test(gModeTest->s32ModuleTestLoop);
		break;
	default:
		RK_LOGE("mode test type:%d is unsupported", gModeTest->s32ModuleTestType);
	}
	SAMPLE_COMM_DumpMeminfo("Exit sample_isp_stress_test", gModeTest->s32ModuleTestType);
	RK_LOGE("sample_isp_stress_test exit");
	return RK_NULL;
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
	RK_CHAR *pDeviceName = RK_NULL;
	RK_CHAR *pOutPath = RK_NULL;
	RK_CHAR *pIqFileDir = RK_NULL;
	RK_S32 s32CamId = 0;
	RK_S32 s32loopCnt = -1;
	RK_S32 s32ChnId = 0;
	RK_S32 s32FrameRate = -1;
	RK_BOOL bMultictx = RK_FALSE;
	rk_aiq_working_mode_t eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
	PIXEL_FORMAT_E ePixelFormat = RK_FMT_YUV420SP;
	COMPRESS_MODE_E eCompressMode = COMPRESS_MODE_NONE;
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
		case 'd':
			pDeviceName = optarg;
			break;
		case 'p' + 'f':
			if (!strcmp(optarg, "nv12")) {
				ePixelFormat = RK_FMT_YUV420SP;
			} else if (!strcmp(optarg, "nv16")) {
				ePixelFormat = RK_FMT_YUV422SP;
			} else if (!strcmp(optarg, "uyvy")) {
				ePixelFormat = RK_FMT_YUV422_UYVY;
			} else if (!strcmp(optarg, "rgb565")) {
				ePixelFormat = RK_FMT_RGB565;
				s32ChnId = 1;
			} else if (!strcmp(optarg, "xbgr8888")) {
				ePixelFormat = RK_FMT_XBGR8888;
				s32ChnId = 1;
			} else {
				RK_LOGE("this pixel_format is not supported in the sample");
				print_usage(argv[0]);
				goto __FAILED2;
			}
			break;
		case 'w':
			s32VideoWidth = atoi(optarg);
			break;
		case 'h':
			s32VideoHeight = atoi(optarg);
			break;
		case 'I':
			s32CamId = atoi(optarg);
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'f':
			s32FrameRate = atoi(optarg);
			break;
		case 'o':
			pOutPath = optarg;
			break;
		case 'h' + 'm':
			if (0 == atoi(optarg)) {
				eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
			} else if (1 == atoi(optarg)) {
				eHdrMode = RK_AIQ_WORKING_MODE_ISP_HDR2;
			} else {
				RK_LOGE("input hdr mode param invaild");
				g_exit_result = RK_FAILURE;
				print_usage(argv[0]);
				goto __FAILED2;
			}
			break;
		case 'm':
			gModeTest->s32ModuleTestType = atoi(optarg);
			break;
		case 't' + 'l':
			gModeTest->s32ModuleTestLoop = atoi(optarg);
			break;
		case 'c':
			gModeTest->u32TestFrameCount = atoi(optarg);
			break;
		case 'c' + 'i':
			s32ChnId = atoi(optarg);
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

		gModeTest->s32CamId = s32CamId;
		gModeTest->eHdrMode = eHdrMode;
		gModeTest->bMultictx = bMultictx;
		gModeTest->pIqFileDir = pIqFileDir;
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
		g_exit_result = RK_FAILURE;
		goto __FAILED;
	}

	// Init VI
	ctx->vi.u32Width = s32VideoWidth;
	ctx->vi.u32Height = s32VideoHeight;
	ctx->vi.s32DevId = s32CamId;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = s32ChnId;
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 2;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 1;
	ctx->vi.stChnAttr.enPixelFormat = ePixelFormat;
	ctx->vi.stChnAttr.enCompressMode = eCompressMode;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = s32FrameRate;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = s32FrameRate;
	ctx->vi.dstFilePath = pOutPath;
	ctx->vi.s32loopCount = s32loopCnt;
	if (pDeviceName) {
		strcpy(ctx->vi.stChnAttr.stIspOpt.aEntityName, pDeviceName);
	}
	s32Ret = SAMPLE_COMM_VI_CreateChn(&ctx->vi);
	if (s32Ret != RK_SUCCESS) {
		g_exit_result = RK_FAILURE;
		RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%d", s32Ret);
		goto __FAILED;
	}

	pthread_create(&gModeTest->vi_thread_id, 0, vi_get_stream, (void *)(&ctx->vi));

	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestIfopen = RK_TRUE;
		pthread_create(&modeTest_thread_id, 0, sample_isp_stress_test,
		               (void *)(gModeTest));
	}

	printf("%s initial finish\n", __func__);

	while (!gModeTest->bIfMainThreadQuit) {
		sleep(1);
	}

	// mode_test_deinit
	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestThreadQuit = RK_TRUE;
		pthread_join(modeTest_thread_id, NULL);
	}

	// VI deinit
	gModeTest->bIfViTHreadQuit = RK_TRUE;
	pthread_join(gModeTest->vi_thread_id, NULL);
	// Destroy VI
	SAMPLE_COMM_VI_DestroyChn(&ctx->vi);

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
