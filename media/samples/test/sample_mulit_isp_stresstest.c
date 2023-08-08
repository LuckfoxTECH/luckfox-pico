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

#include "sample_comm.h"

#define CAM_NUM_MAX 2
#define BUFFER_SIZE 255
#define LDCH_MAX_LEVEL 255

typedef struct _rkModeTest {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfViThreadQuit[CAM_NUM_MAX];
	RK_BOOL bModuleTestThreadQuit;
	RK_BOOL bModuleTestIfopen;
	RK_BOOL bMultictx;
	RK_BOOL bIfIspGroupInit;
	RK_S32 s32ModuleTestType;
	RK_S32 s32ModuleTestLoop;
	RK_S32 s32CamId[CAM_NUM_MAX];
	RK_S32 s32CamGroupId;
	RK_U32 u32TestFrameCount;
	RK_U32 u32ViGetFrameCount[CAM_NUM_MAX];
	RK_U32 u32CamNum;
	rk_aiq_working_mode_t eHdrMode;
	RK_CHAR *pIqFileDir;
} g_mode_test;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[CAM_NUM_MAX];
} SAMPLE_MPI_CTX_S;

static SAMPLE_MPI_CTX_S *ctx = RK_NULL;
static g_mode_test *gModeTest = RK_NULL;
static RK_S32 g_exit_result = RK_SUCCESS;
static sem_t g_sem_module_test[CAM_NUM_MAX];
static pthread_mutex_t g_frame_count_mutex[CAM_NUM_MAX];

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

static RK_CHAR optstr[] = "?::a:w:h:c:o:m:t:l:i:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"camNum", required_argument, NULL, 'c'},
    {"viOutputPath", required_argument, NULL, 'o'},
    {"loopCount", required_argument, NULL, 'l'},
    {"modeTestType", required_argument, NULL, 'm'},
    {"modeTestLoop", required_argument, NULL, 't' + 'l'},
    {"testFrameCount", required_argument, NULL, 't'},
    {"ispLaunchMode", required_argument, NULL, 'i'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -a /oem/usr/share/iqfiles -c 2 -w 1920 -h 1080\n", name);
	printf("\t-a | --aiq : enable aiq with dirpath provided, default: "
	       "/oem/usr/share/iqfiles\n");
	printf("\t-w | --width: Vi width. Default: 1920\n");
	printf("\t-h | --height: Vi height. Default: 1080\n");
	printf("\t-c | --camNum: camera Num. default: 2\n");
	printf("\t-o | --viOutputPath: save vi output yuv file. default: NULL\n");
	printf("\t-l | --loopcount: vi get stream loopcount. default: -1\n");
	printf("\t-m | --modeTestType : test type, 0:none, 1:P/N switch test, 2:HDR switch "
	       "test, 3:frame rate switch test, 4: LDCH mode test. Default: 0\n");
	printf("\t--modeTestLoop : module test loop, default: -1\n");
	printf("\t--testFrameCount : set the venc reveive frame count for every test "
	       "loop, default: 500\n");
	printf("\t--ispLaunchMode : 0: single cam init, 1: camera group init. default: 0\n");
}

static RK_S32 isp_init(void) {
	RK_S32 s32Ret = RK_FAILURE;
#if (defined RKAIQ) && (defined UAPI2)
	if (gModeTest->bIfIspGroupInit == RK_FALSE) {
		for (RK_S32 i = 0; i < gModeTest->u32CamNum; i++) {
			s32Ret = SAMPLE_COMM_ISP_Init(gModeTest->s32CamId[i], gModeTest->eHdrMode,
			                              gModeTest->bMultictx, gModeTest->pIqFileDir);
			s32Ret |= SAMPLE_COMM_ISP_Run(gModeTest->s32CamId[i]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("ISP init failure camid:%d", i);
				return RK_FAILURE;
			}
		}
	} else if (gModeTest->bIfIspGroupInit == RK_TRUE) {
		rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
		memset(&camgroup_cfg, 0, sizeof(rk_aiq_camgroup_instance_cfg_t));
		camgroup_cfg.sns_num = gModeTest->u32CamNum;
		camgroup_cfg.config_file_dir = gModeTest->pIqFileDir;
		s32Ret =
		    SAMPLE_COMM_ISP_CamGroup_Init(gModeTest->s32CamGroupId, gModeTest->eHdrMode,
		                                  gModeTest->bMultictx, &camgroup_cfg);
		if (s32Ret != RK_SUCCESS) {
			printf("%s : isp cam group init\n", __func__);
			return RK_FAILURE;
		}
	} else {
		RK_LOGE("ISP dosen't support this launch mode %d", gModeTest->bIfIspGroupInit);
		return RK_FAILURE;
	}
#endif

	return RK_SUCCESS;
}

static RK_S32 isp_deinit(void) {
	RK_S32 s32Ret = RK_SUCCESS;
#if (defined RKAIQ) && (defined UAPI2)
	if (gModeTest->bIfIspGroupInit == RK_FALSE) {
		for (RK_S32 i = 0; i < gModeTest->u32CamNum; i++) {
			SAMPLE_COMM_ISP_Stop(i);
		}
	} else if (gModeTest->bIfIspGroupInit == RK_TRUE) {
		s32Ret = SAMPLE_COMM_ISP_CamGroup_Stop(gModeTest->s32CamGroupId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Stop failure:%#X", s32Ret);
			return s32Ret;
		}
	} else {
		RK_LOGE("ISP deinit dosen't support this mode %d", gModeTest->bIfIspGroupInit);
		return RK_FAILURE;
	}
#endif
	return RK_SUCCESS;
}

static void *vi_get_stream(void *pArgs) {

	SAMPLE_VI_CTX_S *ctx = (SAMPLE_VI_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 s32Fd = 0;
	RK_S32 s32FrameSize = ctx->u32Height * ctx->u32Width * 3 / 2;
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
		s32Fd = fileno(fp);
	}

	while (!gModeTest->bIfViThreadQuit[ctx->s32DevId]) {
		s32Ret = SAMPLE_COMM_VI_GetChnFrame(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {

			if (fp) {
				fwrite(pData, 1, s32FrameSize, fp);
				fflush(fp);
			}

			SAMPLE_COMM_VI_ReleaseChnFrame(ctx);
			loopCount++;

			if (gModeTest->bModuleTestIfopen) {

				pthread_mutex_lock(&g_frame_count_mutex[ctx->s32DevId]);
				gModeTest->u32ViGetFrameCount[ctx->s32DevId]++;
				pthread_mutex_unlock(&g_frame_count_mutex[ctx->s32DevId]);

				if (gModeTest->u32ViGetFrameCount[ctx->s32DevId] ==
				    gModeTest->u32TestFrameCount) {
					sem_post(&g_sem_module_test[ctx->s32DevId]);
				}
			}
			RK_LOGD("vi chn:%d get_stream count: %d", ctx->s32DevId, loopCount);

			if (ctx->s32loopCount > 0 && loopCount >= ctx->s32loopCount) {
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

	RK_LOGE("vi_get_stream chn:%d exit", ctx->s32DevId);
	return RK_NULL;
}

static void wait_module_test_switch_success(void) {
	for (RK_U32 i = 0; i < gModeTest->u32CamNum; i++) {
		pthread_mutex_lock(&g_frame_count_mutex[i]);
		gModeTest->u32ViGetFrameCount[i] = 0;
		pthread_mutex_unlock(&g_frame_count_mutex[i]);
		sem_wait(&g_sem_module_test[i]);
	}
}

static void pn_mode_switch(RK_S32 test_loop) {
	RK_S32 i = 0;
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;

	while (!gModeTest->bModuleTestThreadQuit) {

		if (gModeTest->bIfIspGroupInit == RK_FALSE) {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_PauseChn(ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_PauseChn failure:%#X pipe:%d chnid:%d", s32Ret,
					        ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
					program_handle_error(__func__, __LINE__);
					break;
				}
			}
		} else {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_StopPipe failure:%#X pipe:%d", s32Ret,
					        ctx->vi[i].u32PipeId);
					program_handle_error(__func__, __LINE__);
					break;
				}
			}
		}

		s32Ret = isp_deinit();
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("isp_deinit failure");
			program_handle_error(__func__, __LINE__);
			break;
		}

		s32Ret = isp_init();
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("isp_init failure");
			program_handle_error(__func__, __LINE__);
			break;
		}
		if (gModeTest->bIfIspGroupInit == RK_FALSE) {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_ResumeChn(ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_ResumeChn failure:%#X pipe:%d chnid:%d", s32Ret,
					        ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
				}
			}
		} else {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_StartPipe(ctx->vi[i].u32PipeId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_StartPipe failure:%#X pipe:%d", s32Ret,
					        ctx->vi[i].u32PipeId);
					program_handle_error(__func__, __LINE__);
					break;
				}
			}
		}

		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("--VI_PAUSE--ISP_STOP--ISP_INIT_RUN--VI_RESUME-----------PN Switch Test "
		        "success Total: %d Now Count: "
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
	RK_S32 i = 0;
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;

	while (!gModeTest->bModuleTestThreadQuit) {

		if (gModeTest->bIfIspGroupInit == RK_FALSE) {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_PauseChn(ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_PauseChn failure:%#X pipe:%d chnid:%d", s32Ret,
					        ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
					program_handle_error(__func__, __LINE__);
					break;
				}
			}
		} else {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_StopPipe failure:%#X pipe:%d", s32Ret,
					        ctx->vi[i].u32PipeId);
					program_handle_error(__func__, __LINE__);
					break;
				}
			}
		}

		s32Ret = isp_deinit();
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("isp_deinit failure");
			program_handle_error(__func__, __LINE__);
			break;
		}

		if (gModeTest->eHdrMode == RK_AIQ_WORKING_MODE_NORMAL) {
			gModeTest->eHdrMode = RK_AIQ_WORKING_MODE_ISP_HDR2;
		} else if (gModeTest->eHdrMode == RK_AIQ_WORKING_MODE_ISP_HDR2) {
			gModeTest->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
		} else {
			gModeTest->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
		}

		s32Ret = isp_init();
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("isp_init failure");
			program_handle_error(__func__, __LINE__);
			break;
		}

		if (gModeTest->bIfIspGroupInit == RK_FALSE) {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_ResumeChn(ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_ResumeChn failure:%#X pipe:%d chnid:%d", s32Ret,
					        ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId);
				}
			}
		} else {
			for (i = 0; i < gModeTest->u32CamNum; i++) {
				s32Ret = RK_MPI_VI_StartPipe(ctx->vi[i].u32PipeId);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_StartPipe failure:%#X pipe:%d", s32Ret,
					        ctx->vi[i].u32PipeId);
					program_handle_error(__func__, __LINE__);
					break;
				}
			}
		}
		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("--VI_PAUSE--ISP_STOP--SET_HDR_MODE--ISP_INIT_RUN--VI_RESUME-------------"
		        "HDR Mode Switch Test success Total: %d Now Count: "
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

	RK_S32 i = 0;
	RK_S32 s32TestCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	VI_CHN_ATTR_S pstChnAttr;
	while (!gModeTest->bModuleTestThreadQuit) {

		for (i = 0; i < gModeTest->u32CamNum; i++) {
			memset(&pstChnAttr, 0, sizeof(VI_CHN_ATTR_S));
			s32Ret = RK_MPI_VI_GetChnAttr(ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId,
			                              &pstChnAttr);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_GetChnAttr failure:%X DevId:%d", s32Ret,
				        ctx->vi[i].s32DevId);
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
			pstChnAttr.stFrameRate.s32DstFrameRate =
			    pstChnAttr.stFrameRate.s32SrcFrameRate;

			s32Ret = RK_MPI_VI_SetChnAttr(ctx->vi[i].u32PipeId, ctx->vi[i].s32ChnId,
			                              &pstChnAttr);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_GetChnAttr failure:%X DevId:%d", s32Ret,
				        ctx->vi[i].s32DevId);
				program_handle_error(__func__, __LINE__);
				break;
			}
			RK_LOGE("Framerate switch to: %d", pstChnAttr.stFrameRate.s32DstFrameRate);
		}
		wait_module_test_switch_success();

		s32TestCount++;
		RK_LOGE("--GET_VI_CHNATTR--SET_FR--SET_VI_CHNATTR----------Framerate switch "
		        "success Total: %d Now Count: "
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
	RK_S32 i = 0;

	while (!gModeTest->bModuleTestThreadQuit) {

		for (i = 0; i < gModeTest->u32CamNum; i++) {
			s32Ret = SAMPLE_COMM_ISP_SetLDCH(gModeTest->s32CamId[i], u32LdchLevel,
			                                 bIfLDCHEnable);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_SetLDCH failure camId:%d",
				        gModeTest->s32CamId[i]);
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
		}
		wait_module_test_switch_success();
		test_count++;

		RK_LOGE("--SET_LDCH---------------LDCH state: %d(0:close 1:open)  level:%d",
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
	wait_module_test_switch_success();

	SAMPLE_COMM_DumpMeminfo("Enter sample_multi_isp_stress_test",
	                        gModeTest->s32ModuleTestType);
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

	SAMPLE_COMM_DumpMeminfo("Exit sample_multi_isp_stress_test",
	                        gModeTest->s32ModuleTestType);
	RK_LOGE("sample_isp_stress_test exit");
	return RK_NULL;
}

RK_S32 global_param_init(void) {

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

	for (RK_S32 i = 0; i < CAM_NUM_MAX; i++) {
		sem_init(&g_sem_module_test[i], 0, 0);

		if (pthread_mutex_init(&g_frame_count_mutex[i], NULL) != 0) {
			RK_LOGE("mutex init failure \n");
			return RK_FAILURE;
		}
	}

	return RK_SUCCESS;
}

RK_S32 global_param_deinit(void) {

	if (ctx) {
		free(ctx);
		ctx = RK_NULL;
	}

	if (gModeTest) {
		free(gModeTest);
		gModeTest = RK_NULL;
	}

	for (RK_S32 i; i < CAM_NUM_MAX; i++) {
		sem_destroy(&g_sem_module_test[i]);
		pthread_mutex_destroy(&g_frame_count_mutex[i]);
	}

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
	RK_CHAR *pViOutPath = RK_NULL;
	RK_U32 u32CamNum = 2;
	RK_S32 s32loopCnt = -1;
	RK_S32 i;
	char *iq_file_dir = "/oem/usr/share/iqfiles";
	pthread_t vi_get_stream_thread_id[CAM_NUM_MAX], modeTest_thread_id;
	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	s32Ret = global_param_init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("global_param_init failure");
		return RK_FAILURE;
	}

	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);

#if (defined RKAIQ) && (defined UAPI2)
	RK_BOOL bMultictx = RK_TRUE;
#endif
	int c;
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		switch (c) {
		case 'a':
			iq_file_dir = optarg;
			break;
		case 'w':
			u32ViWidth = atoi(optarg);
			break;
		case 'h':
			u32ViHeight = atoi(optarg);
			break;
		case 'c':
			u32CamNum = atoi(optarg);
			gModeTest->u32CamNum = u32CamNum;
			break;
		case 'o':
			pViOutPath = optarg;
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'm':
			gModeTest->s32ModuleTestType = atoi(optarg);
			break;
		case 't' + 'l':
			gModeTest->s32ModuleTestLoop = atoi(optarg);
			break;
		case 't':
			gModeTest->u32TestFrameCount = atoi(optarg);
			break;
		case 'i':
			gModeTest->bIfIspGroupInit = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#IQ Path: %s\n", iq_file_dir);
	if (iq_file_dir) {
#if (defined RKAIQ) && (defined UAPI2)
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
		rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
		if (u32CamNum > 1) {
			gModeTest->bMultictx = RK_TRUE;
		} else {
			gModeTest->bMultictx = RK_FALSE;
		}
		gModeTest->eHdrMode = hdr_mode;
		gModeTest->pIqFileDir = iq_file_dir;
		if (gModeTest->bIfIspGroupInit == RK_FALSE) {
			for (RK_S32 i = 0; i < u32CamNum; i++) {
				gModeTest->s32CamId[i] = i;
			}
		} else if (gModeTest->bIfIspGroupInit == RK_TRUE) {
			gModeTest->s32CamGroupId = 0;
		}
		s32Ret = isp_init();
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("isp init failure");
			goto __FAILED2;
		}
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		g_exit_result = RK_FAILURE;
		goto __FAILED;
	}

	// Init VI[0] ~ VI[1]
	for (i = 0; i < u32CamNum; i++) {
		ctx->vi[i].u32Width = u32ViWidth;
		ctx->vi[i].u32Height = u32ViHeight;
		ctx->vi[i].s32DevId = i;
		ctx->vi[i].u32PipeId = i;
		ctx->vi[i].s32ChnId = 0;
		ctx->vi[i].bIfIspGroupInit = gModeTest->bIfIspGroupInit;
		ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = 3;
		ctx->vi[i].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
		ctx->vi[i].stChnAttr.u32Depth = 2;
		ctx->vi[i].dstFilePath = pViOutPath;
		ctx->vi[i].s32loopCount = s32loopCnt;
		ctx->vi[i].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vi[i].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
		ctx->vi[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
		ctx->vi[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
		SAMPLE_COMM_VI_CreateChn(&ctx->vi[i]);
	}
	/* Start pipe */
	if (gModeTest->bIfIspGroupInit == RK_TRUE) { /* isp group init */
		for (i = 0; i < u32CamNum; i++) {
			s32Ret = RK_MPI_VI_StartPipe(ctx->vi[i].u32PipeId);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_StartPipe failure:$#X pipe:%d", s32Ret,
				        ctx->vi[i].u32PipeId);
				g_exit_result = RK_FALSE;
				goto __VI_INITFAIL;
			}
		}
	}
	for (i = 0; i < u32CamNum; i++) {
		pthread_create(&vi_get_stream_thread_id[i], RK_NULL, vi_get_stream, &ctx->vi[i]);
	}

	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestIfopen = RK_TRUE;
		pthread_create(&modeTest_thread_id, RK_NULL, sample_isp_stress_test,
		               (void *)(gModeTest));
	}

	RK_LOGE("%s initial finish", __func__);

	while (!gModeTest->bIfMainThreadQuit) {
		sleep(1);
	}

	RK_LOGE("%s exit!", __func__);

	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestThreadQuit = RK_TRUE;
		pthread_join(modeTest_thread_id, RK_NULL);
	}
	// Vi thread exit
	for (i = 0; i < u32CamNum; i++) {
		gModeTest->bIfViThreadQuit[i] = RK_TRUE;
		pthread_join(vi_get_stream_thread_id[i], RK_NULL);
	}

__VI_INITFAIL:
	/* vi deinit */
	if (gModeTest->bIfIspGroupInit == RK_TRUE) { /* isp group init */
		for (i = 0; i < u32CamNum; i++) {
			s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_StopPipe failure:$#X pipe:%d", s32Ret,
				        ctx->vi[i].u32PipeId);
				g_exit_result = RK_FALSE;
			}
		}
	}
	for (i = 0; i < u32CamNum; i++) {
		s32Ret = SAMPLE_COMM_VI_DestroyChn(&ctx->vi[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_Close failed with %#x!\n", s32Ret);
			g_exit_result = RK_FALSE;
		}
	}

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#if (defined RKAIQ) && (defined UAPI2)
		isp_deinit();
#endif
	}
__FAILED2:
	global_param_deinit();

	return g_exit_result;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
