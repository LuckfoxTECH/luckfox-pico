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

typedef struct _rkModeTest {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfVpssTHreadQuit;
	RK_BOOL bModuleTestThreadQuit;
	RK_BOOL bModuleTestIfopen;
	RK_S32 s32ModuleTestType;
	RK_S32 s32ModuleTestLoop;
	RK_U32 u32TestFrameCount;
	RK_U32 u32VpssGetFrameCount;
	RK_U32 u32VpssModeTestDstWidth;
	RK_U32 u32VpssModeTestDstHeight;
	pthread_t vpss_thread_id;
} g_mode_test;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
	SAMPLE_VPSS_CTX_S vpss;
} SAMPLE_MPI_CTX_S;

/* global param */
g_mode_test *gModeTest = {0};
SAMPLE_MPI_CTX_S *ctx = {0};
RK_S32 g_exit_result = RK_SUCCESS;
sem_t g_sem_module_test = {0};
pthread_mutex_t g_frame_count_mutex = {0};

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

static RK_CHAR optstr[] = "?::a::v:p:l:o:f:m:t:c:s:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"vi_size", required_argument, NULL, 'v' + 's'},
    {"vpss_size", required_argument, NULL, 'p' + 's'},
    {"loop_count", required_argument, NULL, 'l'},
    {"output_path", required_argument, NULL, 'o'},
    {"fps", required_argument, NULL, 'f'},
    {"mode_test_type", required_argument, NULL, 'm'},
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
	printf("\t%s --vi_size 1920x1080 --vpss_size 1920x1080 -a /etc/iqfiles/ -o /data/\n",
	       name);
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
	       "/etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
#endif
	printf("\t-l | --loop_count : loop count. Default -1\n");
	printf("\t-o | --output_path : vpss output file path. Default NULL\n");
	printf("\t-f | --fps : vi framerate. Default: 15\n");
	printf("\t-m | --mode_test_type : test type, 0: none, 1: vpss_deinit_ubind_test,"
	       "2: vpss_resolution_switch_test. Default: 0\n");
	printf("\t--vi_size : set vi resolution WidthxHeight, default: 1920x1080\n");
	printf("\t--vpss_size : set vpss resolution WidthxHeight, default: 1920x1080\n");
	printf("\t--mode_test_loop : module test loop, default: -1\n");
	printf("\t--test_frame_count : set the venc reveive frame count for every test "
	       "loop, default: 10\n");
}

/******************************************************************************
 * function : vpss thread
 ******************************************************************************/
static void *vpss_get_stream(void *pArgs) {
	SAMPLE_VPSS_CTX_S *ctx = (SAMPLE_VPSS_CTX_S *)(pArgs);
	void *pData = RK_NULL;
	char name[256] = {0};
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 loopCount = 0;
	FILE *fp = RK_NULL;
	PIC_BUF_ATTR_S stBufAttr;
	MB_PIC_CAL_S stPicCal;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/vpss_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
	}

	while (!gModeTest->bIfVpssTHreadQuit) {
		s32Ret = SAMPLE_COMM_VPSS_GetChnFrame(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VPSS_ReleaseChnFrame(ctx);
					program_normal_exit(__func__, __LINE__);
					break;
				}
			}

			if (fp) {
				/* cal frame size */
				stBufAttr.u32Width = ctx->stChnFrameInfos.stVFrame.u32Width;
				stBufAttr.u32Height = ctx->stChnFrameInfos.stVFrame.u32Height;
				stBufAttr.enCompMode = ctx->stChnFrameInfos.stVFrame.enCompressMode;
				stBufAttr.enPixelFormat = ctx->stChnFrameInfos.stVFrame.enPixelFormat;
				memset(&stPicCal, 0, sizeof(MB_PIC_CAL_S));
				s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stBufAttr, &stPicCal);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_CAL_VGS_GetPicBufferSize failure s32Ret:%#X", s32Ret);
				} else {
					fwrite(pData, 1, stPicCal.u32MBSize, fp);
					fflush(fp);
				}
			}

			if (ctx->stVpssChnAttr[0].u32Width ==
			    ctx->stChnFrameInfos.stVFrame.u32Width) {
				if (ctx->stChnFrameInfos.stVFrame.u32Height !=
				        ctx->stVpssChnAttr[0].u32Height ||
				    ctx->stChnFrameInfos.stVFrame.u32VirWidth !=
				        ctx->stVpssChnAttr[0].u32Width ||
				    ctx->stChnFrameInfos.stVFrame.u32VirHeight !=
				        ctx->stVpssChnAttr[0].u32Height ||
				    ctx->stChnFrameInfos.stVFrame.enPixelFormat !=
				        ctx->stVpssChnAttr[0].enPixelFormat) {
					RK_LOGE("Avs Current resolution is:%dX%d, get frame's resolution is "
					        ":%dX%d, the frame's PixelFormat"
					        "is :%d need to equal to %d,error exit!!!",
					        ctx->stVpssChnAttr[0].u32Width,
					        ctx->stVpssChnAttr[0].u32Height,
					        ctx->stChnFrameInfos.stVFrame.u32Width,
					        ctx->stChnFrameInfos.stVFrame.u32Height,
					        ctx->stChnFrameInfos.stVFrame.enPixelFormat,
					        ctx->stVpssChnAttr[0].enPixelFormat);
					program_handle_error(__func__, __LINE__);
				}
			} else if (gModeTest->u32VpssModeTestDstWidth ==
			           ctx->stChnFrameInfos.stVFrame.u32Width) {
				if (ctx->stChnFrameInfos.stVFrame.u32Height !=
				        gModeTest->u32VpssModeTestDstHeight ||
				    ctx->stChnFrameInfos.stVFrame.u32VirWidth !=
				        gModeTest->u32VpssModeTestDstWidth ||
				    ctx->stChnFrameInfos.stVFrame.u32VirHeight !=
				        gModeTest->u32VpssModeTestDstHeight ||
				    ctx->stChnFrameInfos.stVFrame.enPixelFormat !=
				        ctx->stVpssChnAttr[0].enPixelFormat) {
					RK_LOGE("Avs Current resolution is:%dX%d, get frame's resolution is "
					        ":%dX%d, the frame's PixelFormat"
					        "is :%d need to equal to %d,error exit!!!",
					        gModeTest->u32VpssModeTestDstWidth,
					        gModeTest->u32VpssModeTestDstHeight,
					        ctx->stChnFrameInfos.stVFrame.u32Width,
					        ctx->stChnFrameInfos.stVFrame.u32Height,
					        ctx->stChnFrameInfos.stVFrame.enPixelFormat,
					        ctx->stVpssChnAttr[0].enPixelFormat);
					program_handle_error(__func__, __LINE__);
				}
			} else {
				RK_LOGE("avs frame's resolution:%xX%d isn't equal to Setting's",
				        ctx->stChnFrameInfos.stVFrame.u32VirWidth,
				        ctx->stChnFrameInfos.stVFrame.u32VirHeight);
				program_handle_error(__func__, __LINE__);
			}

			SAMPLE_COMM_VPSS_ReleaseChnFrame(ctx);
			loopCount++;

			if (gModeTest->bModuleTestIfopen) {

				pthread_mutex_lock(&g_frame_count_mutex);
				gModeTest->u32VpssGetFrameCount++;
				pthread_mutex_unlock(&g_frame_count_mutex);

				if (gModeTest->u32VpssGetFrameCount == gModeTest->u32TestFrameCount) {
					sem_post(&g_sem_module_test);
				}
			}
			RK_LOGE("vpss get_stream count: %d", loopCount);
		}
		usleep(1000);
	}

	if (fp) {
		fclose(fp);
		fp = RK_NULL;
	}
	RK_LOGE("-----------vpss_get_stream thread exit!!!");
	return RK_NULL;
}

static void wait_module_test_switch_success(void) {

	pthread_mutex_lock(&g_frame_count_mutex);
	gModeTest->u32VpssGetFrameCount = 0;
	pthread_mutex_unlock(&g_frame_count_mutex);
	sem_wait(&g_sem_module_test);
}

static void vpss_destroy_ubind_test(RK_S32 s32Testloop) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32TestCount = 0;
	MPP_CHN_S stSrcChn, stDestChn;

	while (!gModeTest->bModuleTestThreadQuit) {

		/* vpss get frame thread exit */
		gModeTest->bIfVpssTHreadQuit = RK_TRUE;
		pthread_join(gModeTest->vpss_thread_id, NULL);

		/* ubind vi and vpss */
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi.s32DevId;
		stSrcChn.s32ChnId = ctx->vi.s32ChnId;
		stDestChn.enModId = RK_ID_VPSS;
		stDestChn.s32DevId = ctx->vpss.s32GrpId;
		stDestChn.s32ChnId = ctx->vpss.s32ChnId;
		SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

		/* destroy vpss */
		s32Ret = SAMPLE_COMM_VPSS_DestroyChn(&ctx->vpss);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("destroy vpss failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		/* create vpss */
		s32Ret = SAMPLE_COMM_VPSS_CreateChn(&ctx->vpss);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("create vpss failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		/* bind vi and vpss */
		SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

		/* launch vpss get frame thread */
		gModeTest->bIfVpssTHreadQuit = RK_FALSE;
		pthread_create(&gModeTest->vpss_thread_id, 0, vpss_get_stream,
		               (void *)(&ctx->vpss));

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------vpss_destroy_ubind_test success Total: %d Now Count: "
		        "%d-------------------",
		        s32Testloop, s32TestCount);
		if (s32Testloop > 0 && s32TestCount >= s32Testloop) {
			RK_LOGE("------------------vpss_destroy_ubind_test end(pass/success) count: "
			        "%d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	RK_LOGE("vpss_destroy_ubind_test exit !!!!");
	return;
}

static void vpss_switch_resolution_test(RK_S32 s32Testloop) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32TestCount = 0;
	RK_S32 s32SrcWidth = ctx->vpss.stVpssChnAttr[0].u32Width;
	RK_S32 s32SrcHeight = ctx->vpss.stVpssChnAttr[0].u32Height;
	VPSS_CHN_ATTR_S stChnAttr;

	while (!gModeTest->bModuleTestThreadQuit) {
		memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));
		s32Ret =
		    RK_MPI_VPSS_GetChnAttr(ctx->vpss.s32GrpId, ctx->vpss.s32ChnId, &stChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VPSS_GetChnAttr failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		/* set vpss resolution */
		if (stChnAttr.u32Width == s32SrcWidth) {
			stChnAttr.u32Width = gModeTest->u32VpssModeTestDstWidth;
			stChnAttr.u32Height = gModeTest->u32VpssModeTestDstHeight;
		} else {
			stChnAttr.u32Width = s32SrcWidth;
			stChnAttr.u32Height = s32SrcHeight;
		}

		s32Ret =
		    RK_MPI_VPSS_SetChnAttr(ctx->vpss.s32GrpId, ctx->vpss.s32ChnId, &stChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VPSS_SetChnAttr failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
			break;
		}

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("-------------------vpss_switch_resolution_test:%dX%d success Total: %d "
		        "Now Count: "
		        "%d-------------------",
		        stChnAttr.u32Width, stChnAttr.u32Height, s32Testloop, s32TestCount);
		if (s32Testloop > 0 && s32TestCount >= s32Testloop) {
			RK_LOGE("------------------vpss_switch_resolution_test end(pass/success) "
			        "count: %d-----------------",
			        s32TestCount);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}

	RK_LOGE("vpss_switch_resolution_test exit !!!");
	return;
}

static void *sample_vpss_stress_test(void *pArgs) {

	prctl(PR_SET_NAME, "vpss_stress_test");
	wait_module_test_switch_success();

	SAMPLE_COMM_DumpMeminfo("Enter sample_vpss_stress_test",
	                        gModeTest->s32ModuleTestType);
	switch (gModeTest->s32ModuleTestType) {
	case 1:
		vpss_destroy_ubind_test(gModeTest->s32ModuleTestLoop);
		break;
	case 2:
		vpss_switch_resolution_test(gModeTest->s32ModuleTestLoop);
		break;
	default:
		RK_LOGE("mode test type:%d is unsupported", gModeTest->s32ModuleTestType);
	}
	SAMPLE_COMM_DumpMeminfo("Exit sample_vpss_stress_test", gModeTest->s32ModuleTestType);
	RK_LOGE("sample_vpss_stress_test exit");
	return RK_NULL;
}

static RK_S32 global_param_init(void) {

	ctx = (SAMPLE_MPI_CTX_S *)malloc(sizeof(SAMPLE_MPI_CTX_S));
	if (ctx == RK_NULL) {
		RK_LOGE("malloc for ctx failure");
		goto INIT_FAIL;
	}
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	gModeTest = (g_mode_test *)malloc(sizeof(g_mode_test));
	if (gModeTest == RK_NULL) {
		RK_LOGE("malloc for gModeTest failure");
		goto INIT_FAIL;
	}
	memset(gModeTest, 0, sizeof(g_mode_test));

	gModeTest->s32ModuleTestLoop = -1;
	gModeTest->u32TestFrameCount = 10;
	gModeTest->u32VpssModeTestDstWidth = 1280;
	gModeTest->u32VpssModeTestDstHeight = 800;

	sem_init(&g_sem_module_test, 0, 0);

	if (pthread_mutex_init(&g_frame_count_mutex, NULL) != 0) {
		RK_LOGE("mutex init failure \n");
		goto INIT_FAIL;
	}

	return RK_SUCCESS;

INIT_FAIL:
	if (ctx) {
		free(ctx);
		ctx = RK_NULL;
	}
	if (gModeTest) {
		free(gModeTest);
		gModeTest = RK_NULL;
	}

	return RK_FAILURE;
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

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32ViWidth = 1920;
	RK_U32 u32ViHeight = 1080;
	RK_U32 u32VpssWidth = 1920;
	RK_U32 u32VpssHeight = 1080;
	RK_CHAR *pOutPath = NULL;
	RK_S32 s32CamId = 0;
	RK_S32 s32loopCnt = -1;
	MPP_CHN_S stSrcChn, stDestChn;
	pthread_t modeTest_thread_id = 0;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	SAMPLE_COMM_CheckFd(RK_TRUE);
	s32Ret = global_param_init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("global_param_init %#X", s32Ret);
		return s32Ret;
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
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'o':
			pOutPath = optarg;
			break;
#ifdef RKAIQ
		case 'M':
			if (atoi(optarg)) {
				bMultictx = RK_TRUE;
			}
			break;
#endif
		case 'v' + 's':
			u32ViWidth = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32ViHeight = atoi(tmp_optarg + 1);
			break;
		case 'p' + 's':
			u32VpssWidth = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32VpssHeight = atoi(tmp_optarg + 1);
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
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#Output Path: %s\n", pOutPath);
	printf("#IQ Path: %s\n", iq_file_dir);

	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		rk_aiq_working_mode_t eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;

		s32Ret = SAMPLE_COMM_ISP_Init(s32CamId, eHdrMode, bMultictx, iq_file_dir);
		s32Ret |= SAMPLE_COMM_ISP_Run(s32CamId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("ISP init failure:#%X", s32Ret);
			g_exit_result = RK_FAILURE;
			goto __FAILED2;
		}
#endif
	}

	/* SYS Init */
	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	/* Init VI */
	ctx->vi.u32Width = u32ViWidth;
	ctx->vi.u32Height = u32ViHeight;
	ctx->vi.s32DevId = s32CamId;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = 1;
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 2;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 0;
	ctx->vi.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi.stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = -1;
	SAMPLE_COMM_VI_CreateChn(&ctx->vi);

	/* Init VPSS */
	ctx->vpss.s32GrpId = 0;
	ctx->vpss.s32ChnId = 0;
	ctx->vpss.s32loopCount = s32loopCnt;
	ctx->vpss.dstFilePath = pOutPath;
	/* RGA_device: VIDEO_PROC_DEV_RGA */
	ctx->vpss.enVProcDevType = VIDEO_PROC_DEV_RGA;
	ctx->vpss.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE; /* no compress */
	ctx->vpss.stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
	ctx->vpss.stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss.stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss.stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].u32Width = u32VpssWidth;
	ctx->vpss.stVpssChnAttr[0].u32Height = u32VpssHeight;
	ctx->vpss.stVpssChnAttr[0].u32Depth = 1;
	s32Ret = SAMPLE_COMM_VPSS_CreateChn(&ctx->vpss);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VPSS_CreateChn failure:%#X", s32Ret);
		program_handle_error(__func__, __LINE__);
	}

	/* Bind VI and VPSS */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = ctx->vpss.s32GrpId;
	stDestChn.s32ChnId = ctx->vpss.s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	/* launch vpss get frame thread */
	pthread_create(&gModeTest->vpss_thread_id, 0, vpss_get_stream, (void *)(&ctx->vpss));

	/* launch mode test thread */
	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestIfopen = RK_TRUE;
		pthread_create(&modeTest_thread_id, 0, sample_vpss_stress_test,
		               (void *)(gModeTest));
	}

	printf("%s initial finish\n", __func__);

	while (!gModeTest->bIfMainThreadQuit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	/* modetest thread exit */
	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestThreadQuit = RK_TRUE;
		pthread_create(&modeTest_thread_id, 0, sample_vpss_stress_test,
		               (void *)(gModeTest));
	}
	/* vpss get frame thread exit */
	gModeTest->bIfVpssTHreadQuit = RK_TRUE;
	pthread_join(gModeTest->vpss_thread_id, NULL);

	/* UnBind Bind VI and VPSS */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = ctx->vpss.s32GrpId;
	stDestChn.s32ChnId = ctx->vpss.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	/* Destroy VPSS */
	SAMPLE_COMM_VPSS_DestroyChn(&ctx->vpss);
	/* Destroy VI[0] */
	SAMPLE_COMM_VI_DestroyChn(&ctx->vi);

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}
__FAILED2:
	global_param_deinit();
	SAMPLE_COMM_CheckFd(RK_FALSE);

	return g_exit_result;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
