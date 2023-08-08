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
#define RGN_CHN_MAX 7
#define VI_CHN_MAX 3
#define VENC_CHN_MAX 4
#define GET_STREAM_TIMEOUT 2000
#define SEND_STREAM_TIMEOUT 2000
#define TDE_JPEG_CHNID 2
#define COMBO_JPEG_CHNID 3
#define LDCH_MAX_CORRECT_LEVEL 255
#define VI_RGN_NUM 4
#define VENC_RGN_NUM 3

typedef struct _rkModeTest {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfVencThreadQuit[VENC_CHN_MAX];
	RK_BOOL bIfViThreadQuit;
	RK_BOOL bModuleTestThreadQuit;
	RK_BOOL bIvsDetectThreadQuit;
	RK_BOOL bIfViIvaTHreadQuit;
	RK_BOOL bModuleTestIfopen;
	RK_BOOL bWrapIfEnable;
	RK_S32 s32ModuleTestType;
	RK_U32 u32ModuleTestLoop;
	RK_U32 u32TestFrameCount;
	RK_U32 u32VencGetFrameCount[VENC_CHN_MAX];
	RK_CHAR *inputBmp1Path;
	RK_CHAR *inputBmp2Path;
	RK_S32 s32CamId;
	RK_S32 bMultictx;
	rk_aiq_working_mode_t eHdrMode;
	RK_CHAR *pIqFileDir;
} g_mode_test;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[VI_CHN_MAX];
	SAMPLE_VENC_CTX_S venc[VENC_CHN_MAX];
	SAMPLE_RGN_CTX_S rgn[RGN_CHN_MAX];
	SAMPLE_TDE_CTX_S tde;
	SAMPLE_IVS_CTX_S ivs;
#ifdef ROCKIVA
	SAMPLE_IVA_CTX_S iva;
#endif
} SAMPLE_MPI_CTX_S;

/* global param */
g_mode_test *gModeTest;
SAMPLE_MPI_CTX_S *ctx;
RK_S32 g_exit_result = RK_SUCCESS;
sem_t g_sem_module_test[VENC_CHN_MAX];
pthread_mutex_t g_frame_count_mutex[VENC_CHN_MAX];
RK_BOOL g_rtsp_ifenbale = RK_FALSE;
rtsp_demo_handle g_rtsplive = RK_NULL;
static rtsp_session_handle g_rtsp_session[VENC_CHN_MAX] = {0};

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

static RK_CHAR optstr[] = "?::a::w:h:o:l:m:e:r:f:t:i:I:p:v:c:d:s:";
static const struct option long_options[] = {
    {"aiq", optional_argument, RK_NULL, 'a'},
    {"width", required_argument, RK_NULL, 'w'},
    {"height", required_argument, RK_NULL, 'h'},
    {"output_path", required_argument, RK_NULL, 'o'},
    {"loop_count", required_argument, RK_NULL, 'l'},
    {"mode_test_type", required_argument, RK_NULL, 'm'},
    {"encode", required_argument, RK_NULL, 'e'},
    {"wrap", required_argument, RK_NULL, 'r'},
    {"fps", required_argument, RK_NULL, 'f'},
    {"inputBmp1Path", required_argument, RK_NULL, 'i'},
    {"inputBmp2Path", required_argument, RK_NULL, 'I'},
    {"smartP", required_argument, RK_NULL, 'p'},
    {"vi_buff_cnt", required_argument, RK_NULL, 'v'},
    {"mode_test_loop", required_argument, RK_NULL, 't' + 'l'},
    {"test_frame_count", required_argument, RK_NULL, 'c'},
    {"iva_detect_speed", required_argument, RK_NULL, 'd'},
    {"venc_buff_size", required_argument, RK_NULL, 'v' + 's'},
    {"wrap_lines", required_argument, RK_NULL, 'w' + 'l'},
    {"help", optional_argument, RK_NULL, '?'},
    {RK_NULL, 0, RK_NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 \n", name);
#ifdef RKAIQ
	printf(
	    "\t-a | --aiq : enable aiq with dirpath provided, eg:-a /etc/iqfiles/, \n"
	    "\t		set dirpath empty to using path by default, without this option aiq \n"
	    "\t		should run in other application\n");
#endif
	printf("\t-w | --width : mainStream width, Default: 1920\n");
	printf("\t-h | --height : mainStream height, Default: 1080\n");
	printf("\t-o | --output_path : encode output file path, Default: RK_NULL\n");
	printf("\t-l | --loop_count : when encoder output frameCounts equal to <loop_count>, "
	       "process will exit. Default: -1\n");
	printf(
	    "\t-m | --mode_test_type : test type, 0:none, 1: pn_mode_test 2: hdr_mode_test \n"
	    " \t           3: framerate_switch_test, 4: ldch_mode_test, 5: "
	    "encode_resolution_switch, 6: encode_type_switch\n"
	    "\t            7: smartP_mode_test, 8: SVC_mode_test, 9: motion_deblur_test, 10: "
	    "force_idr_test, 11: venc_chn_rotation_test, \n"
	    "\t            12: rgn_attach_and_detach Default: 0\n");
	printf(
	    "\t-e | --encode : set encode type, Value: h264cbr, h264vbr, h264avbr, h265cbr, "
	    "h265vbr, h265avbr, default: h264cbr \n");
	printf("\t-r | --wrap : wrap for mainStream, 0: close 1: open, Default: 0\n");
	printf("\t-i | --inputBmp1Path : input bmp file path. default: RK_NULL\n");
	printf("\t-I | --inputBmp2Path : input bmp file path. default: RK_NULL\n");
	printf("\t-p | --smartP : smartp mode for mainStream, 0: Disable 1: Enable. Default: "
	       "0\n");
	printf("\t-v | --vi_buff_cnt : main stream vi buffer num, Default: 2\n");
	printf("\t--mode_test_loop : module test loop, default: -1\n");
	printf("\t--test_frame_count : when encoder outputs frameCount equal to "
	       "<test_frame_count>, mode_test start next loop, default: 500\n");
	printf("\t--iva_detect_speed : iva detect framerate. default: 10\n");
	printf("\t--venc_buff_size : main stream venc output buffer size. default value is "
	       "vencWidth*vencHeigth/2(byte)\n");
	printf("\t--wrap_lines : 0: height/2, 1: height/4, 2: height/8. default: 1\n");
}

static void vi_venc_thread_error_handle(const char *func, RK_U32 line, MB_BLK mb,
                                        RK_BOOL ifrelease) {
	if (mb && ifrelease) {
		SAMPLE_COMM_TDE_ReleaseMB(&ctx->tde);
	}

	RK_MPI_VI_ReleaseChnFrame(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId,
	                          &ctx->vi[0].stViFrame);
	if (gModeTest->bIfMainThreadQuit) {
		program_normal_exit(func, line);
	} else {
		program_handle_error(func, line);
	}
}

/* vi get stream send tde and tde send venc*/
static void *vi_venc_thread(void *pArgs) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_LOGE("into vi_venc_thread------------------------------------");
	while (!gModeTest->bIfViThreadQuit) {

		s32Ret = RK_MPI_VI_GetChnFrame(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId,
		                               &ctx->vi[0].stViFrame, GET_STREAM_TIMEOUT);
		if (s32Ret == RK_SUCCESS) {

			s32Ret = SAMPLE_COMM_TDE_GetMB(&ctx->tde);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_TDE_GetMB failure");
				vi_venc_thread_error_handle(__func__, __LINE__, RK_NULL, RK_FALSE);
				continue;
			}

			s32Ret = SAMPLE_COMM_TDE_Handle(&ctx->tde, &ctx->vi[0].stViFrame.stVFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_TDE_Handle failure:%X", s32Ret);
				vi_venc_thread_error_handle(
				    __func__, __LINE__, ctx->tde.stVideoFrames.stVFrame.pMbBlk, RK_TRUE);
				continue;
			}

			s32Ret = RK_MPI_VENC_SendFrame(ctx->venc[2].s32ChnId, &ctx->tde.stVideoFrames,
			                               SEND_STREAM_TIMEOUT);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VENC_SendFrame Failure:%X chnid:%d", s32Ret,
				        ctx->venc[2].s32ChnId);
				vi_venc_thread_error_handle(
				    __func__, __LINE__, ctx->tde.stVideoFrames.stVFrame.pMbBlk, RK_TRUE);
				continue;
			}

			s32Ret = SAMPLE_COMM_TDE_ReleaseMB(&ctx->tde);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_TDE_ReleaseMB failure:%X", s32Ret);
				RK_MPI_VI_ReleaseChnFrame(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId,
				                          &ctx->vi[0].stViFrame);
				program_handle_error(__func__, __LINE__);
				continue;
			}

			s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId,
			                                   &ctx->vi[0].stViFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_ReleaseChnFrame failure:%X pipe:%d chnid:%d", s32Ret,
				        ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);
				program_handle_error(__func__, __LINE__);
				continue;
			}

		} else {
			RK_LOGE("RK_MPI_VI_GetChnFrame Failure:%X pipe:%d chnid:%d", s32Ret,
			        ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);
		}
	}
	RK_LOGE("vi %d get stream exit", ctx->vi[0].s32ChnId);
	return RK_NULL;
}

static void *venc_get_stream(void *pArgs) {

	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 s32fd = 0;
	RK_S32 loopCount = 0;
	RK_VOID *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};

	if (ctx->dstFilePath) {
		if (ctx->s32ChnId == TDE_JPEG_CHNID || ctx->s32ChnId == COMBO_JPEG_CHNID) {
			snprintf(name, sizeof(name), "/%s/venc_%d.jpeg", ctx->dstFilePath,
			         ctx->s32ChnId);
		} else {
			snprintf(name, sizeof(name), "/%s/venc_%d.bin", ctx->dstFilePath,
			         ctx->s32ChnId);
		}
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
		s32fd = fileno(fp);
	}
	while (!gModeTest->bIfVencThreadQuit[ctx->s32ChnId]) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {

			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					program_normal_exit(__func__, __LINE__);
					break;
				}
			}

			if (fp && !gModeTest->bIfMainThreadQuit) {
				if (ctx->s32ChnId == TDE_JPEG_CHNID ||
				    ctx->s32ChnId == COMBO_JPEG_CHNID) {
					fseek(fp, 0, SEEK_SET);
				}
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}

			if (g_rtsp_ifenbale && ctx->s32ChnId != TDE_JPEG_CHNID &&
			    ctx->s32ChnId != COMBO_JPEG_CHNID) {
				rtsp_tx_video(g_rtsp_session[ctx->s32ChnId], pData,
				              ctx->stFrame.pstPack->u32Len, ctx->stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
			} else {
				RK_LOGD("venc %d get_stream count: %d", ctx->s32ChnId, loopCount);
			}

			if (gModeTest->bModuleTestIfopen) {
				pthread_mutex_lock(&g_frame_count_mutex[ctx->s32ChnId]);
				gModeTest->u32VencGetFrameCount[ctx->s32ChnId]++;
				pthread_mutex_unlock(&g_frame_count_mutex[ctx->s32ChnId]);

				if (gModeTest->u32VencGetFrameCount[ctx->s32ChnId] ==
				    gModeTest->u32TestFrameCount) {
					sem_post(&g_sem_module_test[ctx->s32ChnId]);
				}
			}

			RK_LOGD("venc %d get_stream count: %d", ctx->s32ChnId, loopCount);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
	}

	if (fp) {
		fsync(s32fd);
		fclose(fp);
		fp = RK_NULL;
	}

	RK_LOGE("venc_get_stream chnid:%d exit", ctx->s32ChnId);
	return RK_NULL;
}

#ifdef ROCKIVA
static void rkIvaEvent_callback(const RockIvaBaResult *result,
                                const RockIvaExecuteStatus status, void *userData) {
	if (result->objNum == 0)
		return;

	for (int i = 0; i < result->objNum; i++) {
		RK_LOGD("topLeft:[%d,%d], bottomRight:[%d,%d],"
		        "objId is %d, frameId is %d, score is %d, type is %d\n",
		        result->triggerObjects[i].objInfo.rect.topLeft.x,
		        result->triggerObjects[i].objInfo.rect.topLeft.y,
		        result->triggerObjects[i].objInfo.rect.bottomRight.x,
		        result->triggerObjects[i].objInfo.rect.bottomRight.y,
		        result->triggerObjects[i].objInfo.objId,
		        result->triggerObjects[i].objInfo.frameId,
		        result->triggerObjects[i].objInfo.score,
		        result->triggerObjects[i].objInfo.type);
	}
}

static void *vi_iva_thread(void *pArgs) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_CHAR *pData = RK_NULL;
	RK_S32 s32Fd = 0;
	RockIvaImage ivaImage;
	RK_U32 u32Loopcount = 0;
	RK_U32 u32GetOneFrameTime = 1000 / ctx->iva.u32IvaDetectFrameRate;

	while (!gModeTest->bIfViIvaTHreadQuit) {
		s32Ret = SAMPLE_COMM_VI_GetChnFrame(&ctx->vi[2], &pData);
		if (s32Ret == RK_SUCCESS) {
			s32Fd = RK_MPI_MB_Handle2Fd(ctx->vi[2].stViFrame.stVFrame.pMbBlk);
			memset(&ivaImage, 0, sizeof(RockIvaImage));
			ivaImage.info.transformMode = ctx->iva.eImageTransform;
			ivaImage.info.width = ctx->vi[2].stViFrame.stVFrame.u32Width;
			ivaImage.info.height = ctx->vi[2].stViFrame.stVFrame.u32Height;
			ivaImage.info.format = ctx->iva.eImageFormat;
			ivaImage.frameId = u32Loopcount;
			ivaImage.dataAddr = NULL;
			ivaImage.dataPhyAddr = NULL;
			ivaImage.dataFd = s32Fd;
			s32Ret = ROCKIVA_PushFrame(ctx->iva.ivahandle, &ivaImage, NULL);
			u32Loopcount++;

			SAMPLE_COMM_VI_ReleaseChnFrame(&ctx->vi[2]);
		}
		usleep(u32GetOneFrameTime * 1000);
	}

	RK_LOGE("vi_iva_thread exit !!!");
	return RK_NULL;
}
#endif

static void *ivs_detect_thread(void *pArgs) {
	RK_S32 s32Ret = RK_FAILURE;
	IVS_RESULT_INFO_S stResults;
	RK_U32 u32IvsDetectCount = 0;
	RK_U32 width = ctx->ivs.stIvsAttr.u32PicWidth;
	RK_U32 u32Count = 0;
	RK_U32 x, y;
	IVS_CHN_ATTR_S pstAttr;

	memset(&pstAttr, 0, sizeof(IVS_CHN_ATTR_S));

	RK_MPI_IVS_GetChnAttr(0, &pstAttr);

	RK_LOGE("odIfEnable:%d ", pstAttr.bODEnable);

	while (!gModeTest->bIvsDetectThreadQuit) {
		memset(&stResults, 0, sizeof(IVS_RESULT_INFO_S));
		s32Ret = RK_MPI_IVS_GetResults(0, &stResults, GET_STREAM_TIMEOUT);
		if (s32Ret == RK_SUCCESS) {
			u32IvsDetectCount++;
			RK_LOGD("s32ReNum: %d", stResults.s32ResultNum);
			if (u32IvsDetectCount % 10 == 0 && stResults.s32ResultNum == 1) {
				x = width / 64;
				y = stResults.pstResults->stMdInfo.u32Size / 64;
				if (stResults.pstResults->stMdInfo.pData) {
					u32Count = 0;
					for (int j = 0; j < y; j++) {
						for (int i = 0; i < x; i++) {
							for (int k = 0; k < 8; k++) {
								if (stResults.pstResults->stMdInfo.pData[j * 64 + i] &
								    (1 << k))
									u32Count++;
							}
						}
					}
				}
				if (u32Count > (x * y * 8 / 5)) {
					RK_LOGE("Detect movement\n");
				}
				u32IvsDetectCount = 0;
			}
			if (stResults.s32ResultNum > 0) {
				if (stResults.pstResults->stOdInfo.u32Flag) {
					RK_LOGE("OD flag:%d\n", stResults.pstResults->stOdInfo.u32Flag);
				}
			}

			RK_MPI_IVS_ReleaseResults(0, &stResults);

		} else {
			RK_LOGE("RK_MPI_IVS_GetResults failure:%X", s32Ret);
		}
	}

	RK_LOGE("ivs_detect_thread exit");
	return RK_NULL;
}

static RK_S32 pnMode_stressTest(RK_S32 s32CamId, rk_aiq_working_mode_t WDRMode,
                                RK_BOOL MultiCam, const char *iq_file_dir) {
	RK_S32 s32Ret = RK_FAILURE;

	RK_MPI_VI_PauseChn(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);

	RK_MPI_VI_PauseChn(ctx->vi[1].u32PipeId, ctx->vi[1].s32ChnId);

	RK_MPI_VI_PauseChn(ctx->vi[2].u32PipeId, ctx->vi[2].s32ChnId);

	SAMPLE_COMM_ISP_Stop(s32CamId);

	s32Ret = SAMPLE_COMM_ISP_Init(s32CamId, WDRMode, MultiCam, iq_file_dir);
	s32Ret |= SAMPLE_COMM_ISP_Run(s32CamId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init failure\n");
		return s32Ret;
	}

	RK_MPI_VI_ResumeChn(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);

	RK_MPI_VI_ResumeChn(ctx->vi[1].u32PipeId, ctx->vi[1].s32ChnId);

	RK_MPI_VI_ResumeChn(ctx->vi[2].u32PipeId, ctx->vi[2].s32ChnId);

	RK_LOGE("-----------------PN mode switch success");
	return RK_SUCCESS;
}

static RK_S32 hdrMode_stressTest(RK_S32 s32CamId, rk_aiq_working_mode_t WDRMode,
                                 RK_BOOL MultiCam, const char *iq_file_dir) {

	RK_S32 s32Ret = RK_FAILURE;
	static rk_aiq_working_mode_t eNowHdrMode = RK_AIQ_WORKING_MODE_NORMAL;

	RK_MPI_VI_PauseChn(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);

	RK_MPI_VI_PauseChn(ctx->vi[1].u32PipeId, ctx->vi[1].s32ChnId);

	RK_MPI_VI_PauseChn(ctx->vi[2].u32PipeId, ctx->vi[2].s32ChnId);

	SAMPLE_COMM_ISP_Stop(s32CamId);

	if (eNowHdrMode == RK_AIQ_WORKING_MODE_NORMAL) {
		eNowHdrMode = RK_AIQ_WORKING_MODE_ISP_HDR2;
	} else if (eNowHdrMode == RK_AIQ_WORKING_MODE_ISP_HDR2) {
		eNowHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
	} else {
		eNowHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
	}
	s32Ret = SAMPLE_COMM_ISP_Init(s32CamId, eNowHdrMode, MultiCam, iq_file_dir);
	s32Ret |= SAMPLE_COMM_ISP_Run(s32CamId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("ISP init failure\n");
		return s32Ret;
	}

	RK_MPI_VI_ResumeChn(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);

	RK_MPI_VI_ResumeChn(ctx->vi[1].u32PipeId, ctx->vi[1].s32ChnId);

	RK_MPI_VI_ResumeChn(ctx->vi[2].u32PipeId, ctx->vi[2].s32ChnId);

	RK_LOGE("------------switch to %d(normal:0 HDR2:16)", eNowHdrMode);

	return RK_SUCCESS;
}

static RK_S32 frameRate_switchTest(SAMPLE_VI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	VI_CHN_ATTR_S pstChnAttr;

	memset(&pstChnAttr, 0, sizeof(VI_CHN_ATTR_S));
	s32Ret = RK_MPI_VI_GetChnAttr(ctx->u32PipeId, ctx->s32ChnId, &pstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_GetChnAttr failure:%X", s32Ret);
		return s32Ret;
	}

	if (pstChnAttr.stFrameRate.s32SrcFrameRate == -1) {
		pstChnAttr.stFrameRate.s32SrcFrameRate = 1;
	}

	pstChnAttr.stFrameRate.s32SrcFrameRate += 1;
	if (pstChnAttr.stFrameRate.s32SrcFrameRate > 25) {
		pstChnAttr.stFrameRate.s32SrcFrameRate = 1;
	}
	pstChnAttr.stFrameRate.s32DstFrameRate = pstChnAttr.stFrameRate.s32SrcFrameRate;

	s32Ret = RK_MPI_VI_SetChnAttr(ctx->u32PipeId, ctx->s32ChnId, &pstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_GetChnAttr failure:%X", s32Ret);
		return s32Ret;
	}

	RK_LOGE("---------------Framerate switch to: %d",
	        pstChnAttr.stFrameRate.s32DstFrameRate);

	return RK_SUCCESS;
}

static RK_S32 ldchMode_test(RK_S32 s32CamId) {
	RK_S32 s32Ret = RK_FAILURE;
	static RK_U32 u32LdchLevel = 1;
	static RK_BOOL bIfLDCHEnable = RK_TRUE;

	s32Ret = SAMPLE_COMM_ISP_SetLDCH(s32CamId, u32LdchLevel, bIfLDCHEnable);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_ISP_SetLDCH failure");
		return s32Ret;
	}

	if (bIfLDCHEnable) {
		u32LdchLevel++;
		if (u32LdchLevel > LDCH_MAX_CORRECT_LEVEL) {
			u32LdchLevel = 0;
		}
	}
	bIfLDCHEnable = !bIfLDCHEnable;

	RK_LOGE("-----------------LDCH state: %d(0:close 1:open)  level:%d", bIfLDCHEnable,
	        u32LdchLevel);
	return RK_SUCCESS;
}

static RK_S32 venc_rgn_detach(void) {
	RK_S32 s32Ret = RK_FAILURE;

	for (RK_S32 i = 0; i < VENC_RGN_NUM; i++) {
		s32Ret = RK_MPI_RGN_DetachFromChn(ctx->rgn[i + VI_RGN_NUM].rgnHandle,
		                                  &ctx->rgn[i + VI_RGN_NUM].stMppChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_RGN_DetachFromChn handle:%d  failure:%#X",
			        ctx->rgn[i + VI_RGN_NUM].rgnHandle, s32Ret);
			return s32Ret;
		}
	}
	return s32Ret;
}

static RK_S32 venc_rgn_attach(void) {
	RK_S32 s32Ret = RK_FAILURE;

	for (RK_S32 i = 0; i < VENC_RGN_NUM; i++) {
		s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[i + VI_RGN_NUM].rgnHandle,
		                                &ctx->rgn[i + VI_RGN_NUM].stMppChn,
		                                &ctx->rgn[i + VI_RGN_NUM].stRgnChnAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_RGN_AttachToChn handle:%d failure:%#X",
			        ctx->rgn[i + VI_RGN_NUM].rgnHandle, s32Ret);
			return s32Ret;
		}
	}
	return s32Ret;
}

static RK_S32 vencResolution_switchTest(SAMPLE_TDE_CTX_S *pTdeCtx,
                                        SAMPLE_VENC_CTX_S *pVencCtx,
                                        SAMPLE_VI_CTX_S *pViCtx,
                                        SAMPLE_VENC_CTX_S *pComboVencCtx) {

	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32DstWidth = 704;
	RK_U32 u32DstHeight = 576;
	VENC_CHN_ATTR_S pstChnAttr;
	VENC_CHN_ATTR_S pstComboChnAttr;
	VI_CHN_ATTR_S vipstChnAttr;
	MPP_CHN_S stSrcChn, stDestChn;

	/* rgn detach */
	s32Ret = venc_rgn_detach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_detach failure");
		return s32Ret;
	}

	// unBind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}

	memset(&pstChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
	memset(&pstComboChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
	s32Ret = RK_MPI_VENC_GetChnAttr(pVencCtx->s32ChnId, &pstChnAttr);
	s32Ret |= RK_MPI_VENC_GetChnAttr(pComboVencCtx->s32ChnId, &pstComboChnAttr);
	RK_LOGD("w: %d h: %d", pstChnAttr.stVencAttr.u32PicWidth,
	        pstChnAttr.stVencAttr.u32PicHeight);
	if (pstChnAttr.stVencAttr.u32PicWidth == pVencCtx->u32Width) {
		pstChnAttr.stVencAttr.u32PicWidth = u32DstWidth;
		pstChnAttr.stVencAttr.u32PicHeight = u32DstHeight;
		pstComboChnAttr.stVencAttr.u32PicWidth = u32DstWidth;
		pstComboChnAttr.stVencAttr.u32PicHeight = u32DstHeight;
		pstChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(u32DstWidth);
		pstChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(u32DstHeight);
		pstComboChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(u32DstWidth);
		pstComboChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(u32DstHeight);
	} else {
		pstChnAttr.stVencAttr.u32PicWidth = pVencCtx->u32Width;
		pstChnAttr.stVencAttr.u32PicHeight = pVencCtx->u32Height;
		pstComboChnAttr.stVencAttr.u32PicWidth = pVencCtx->u32Width;
		pstComboChnAttr.stVencAttr.u32PicHeight = pVencCtx->u32Height;
		pstChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(pVencCtx->u32Width);
		pstChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(pVencCtx->u32Height);
		pstComboChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(pVencCtx->u32Width);
		pstComboChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(pVencCtx->u32Height);
	}

	pTdeCtx->pstSrc.u32Width = pstChnAttr.stVencAttr.u32PicWidth;
	pTdeCtx->pstSrc.u32Height = pstChnAttr.stVencAttr.u32PicHeight;

	s32Ret |= RK_MPI_VENC_SetChnAttr(pVencCtx->s32ChnId, &pstChnAttr);
	s32Ret |= RK_MPI_VENC_SetChnAttr(pComboVencCtx->s32ChnId, &pstComboChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc set chn resolution failure");
		return s32Ret;
	}

	memset(&vipstChnAttr, 0, sizeof(VI_CHN_ATTR_S));
	s32Ret = RK_MPI_VI_GetChnAttr(pViCtx->u32PipeId, pViCtx->s32ChnId, &vipstChnAttr);
	RK_LOGD("w: %d h: %d", vipstChnAttr.stSize.u32Width, vipstChnAttr.stSize.u32Height);
	if (vipstChnAttr.stSize.u32Width == pViCtx->u32Width) {
		vipstChnAttr.stSize.u32Width = u32DstWidth;
		vipstChnAttr.stSize.u32Height = u32DstHeight;
	} else {
		vipstChnAttr.stSize.u32Width = pViCtx->u32Width;
		vipstChnAttr.stSize.u32Height = pViCtx->u32Height;
	}

	s32Ret |= RK_MPI_VI_SetChnAttr(pViCtx->u32PipeId, pViCtx->s32ChnId, &vipstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE(" set resolution failure");
		return s32Ret;
	}

	// Bind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}
	/* rgn attach */
	s32Ret = venc_rgn_attach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_attach failure");
		return s32Ret;
	}

	RK_LOGE("------------------------Venc resolution switch to %dx%d",
	        vipstChnAttr.stSize.u32Width, vipstChnAttr.stSize.u32Height);

	return RK_SUCCESS;
}

static RK_S32 encode_destroy_and_restart(CODEC_TYPE_E enCodecType,
                                         VENC_RC_MODE_E enRcMode, RK_U32 u32Profile,
                                         RK_BOOL bIfSliceSplit, RK_BOOL *bVencThreadQuit,
                                         SAMPLE_VENC_CTX_S *pVencCtx,
                                         SAMPLE_VI_CTX_S *pViCtx) {
	RK_S32 s32Ret = RK_FAILURE;
	MPP_CHN_S stSrcChn, stDestChn;

	/* rgn detach */
	s32Ret = venc_rgn_detach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_detach failure");
		return s32Ret;
	}

	*bVencThreadQuit = RK_TRUE;
	if (pVencCtx->getStreamCbFunc) {
		pthread_join(pVencCtx->getStreamThread, RK_NULL);
	}

	// unBind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}
	// Destroy venc
	s32Ret = SAMPLE_COMM_VENC_DestroyChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	pVencCtx->enCodecType = enCodecType;
	pVencCtx->enRcMode = enRcMode;
	pVencCtx->stChnAttr.stVencAttr.u32Profile = u32Profile;
	*bVencThreadQuit = RK_FALSE;

	// Init VENC
	s32Ret = SAMPLE_COMM_VENC_CreateChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	// Bind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}

	/* rgn attach */
	s32Ret = venc_rgn_attach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_attach failure");
		return s32Ret;
	}

	return s32Ret;
}

static RK_S32 encode_typeSwitch(RK_BOOL *bVencThreadQuit, SAMPLE_VENC_CTX_S *pVencCtx,
                                SAMPLE_VI_CTX_S *pViCtx) {
	RK_S32 s32Ret = RK_FAILURE;
	static RK_U32 now_test_loop = 0;

	switch (now_test_loop % 2) {
	case 0: /* H264 CBR */
		RK_LOGE("---------------------------Switch To H264CBR");

		s32Ret = encode_destroy_and_restart(RK_CODEC_TYPE_H264, VENC_RC_MODE_H264CBR, 100,
		                                    RK_FALSE, bVencThreadQuit, pVencCtx, pViCtx);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("switch to 264_cbr failure");
			return s32Ret;
		}
		break;
	case 1: /* H265 CBR */
		RK_LOGE("---------------------------Switch To H265CBR");

		s32Ret = encode_destroy_and_restart(RK_CODEC_TYPE_H265, VENC_RC_MODE_H265CBR, 0,
		                                    RK_FALSE, bVencThreadQuit, pVencCtx, pViCtx);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("switch to 265_cbr failure");
			return s32Ret;
		}
		break;
	default:
		break;
	}

	now_test_loop++;

	return RK_SUCCESS;
}

static RK_S32 smartP_switchTest(RK_BOOL *bVencThreadQuit, SAMPLE_VENC_CTX_S *pVencCtx,
                                SAMPLE_VI_CTX_S *pViCtx) {
	RK_S32 s32Ret = RK_FAILURE;
	MPP_CHN_S stSrcChn, stDestChn;
	static RK_BOOL eSmartpIfEnable = RK_TRUE;

	/* rgn detach */
	s32Ret = venc_rgn_detach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_detach failure");
		return s32Ret;
	}

	*bVencThreadQuit = RK_TRUE;
	if (pVencCtx->getStreamCbFunc) {
		pthread_join(pVencCtx->getStreamThread, RK_NULL);
	}

	// unBind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}
	// Destroy venc
	s32Ret = SAMPLE_COMM_VENC_DestroyChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}
	if (eSmartpIfEnable) {
		pVencCtx->stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		pVencCtx->stChnAttr.stGopAttr.s32VirIdrLen = pVencCtx->u32Gop / 2;
		RK_LOGE("------------------GopMode set to VENC_GOPMODE_SMARTP");
	} else {
		pVencCtx->stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
		RK_LOGE("------------------GopMode set to VENC_GOPMODE_NORMALP");
	}
	*bVencThreadQuit = RK_FALSE;
	eSmartpIfEnable = !eSmartpIfEnable;

	// Init VENC
	s32Ret = SAMPLE_COMM_VENC_CreateChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	// Bind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}

	/* rgn attach */
	s32Ret = venc_rgn_attach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_attach failure");
		return s32Ret;
	}

	return RK_SUCCESS;
}

static RK_S32 smartEncode_switchTest(RK_BOOL *bVencThreadQuit,
                                     SAMPLE_VENC_CTX_S *pVencCtx,
                                     SAMPLE_VI_CTX_S *pViCtx) {
	RK_S32 s32Ret = RK_FAILURE;
	static RK_BOOL eSvcIfEnable = RK_TRUE;
	MPP_CHN_S stSrcChn, stDestChn;

	/* rgn detach */
	s32Ret = venc_rgn_detach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_detach failure");
		return s32Ret;
	}

	*bVencThreadQuit = RK_TRUE;
	if (pVencCtx->getStreamCbFunc) {
		pthread_join(pVencCtx->getStreamThread, RK_NULL);
	}

	// unBind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}

	// Destroy venc
	s32Ret = SAMPLE_COMM_VENC_DestroyChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	if (eSvcIfEnable) {
		pVencCtx->bSvcIfEnable = eSvcIfEnable;
		if (pVencCtx->enCodecType == RK_CODEC_TYPE_H265) {
			pVencCtx->enRcMode = VENC_RC_MODE_H265VBR;
		} else if (pVencCtx->enCodecType == RK_CODEC_TYPE_H264) {
			pVencCtx->enRcMode = VENC_RC_MODE_H264VBR;
		}
		RK_LOGE("---------------------Smart video coding enable");
	} else {
		pVencCtx->bSvcIfEnable = eSvcIfEnable;
		RK_LOGE("---------------------Smart video coding disable");
	}

	eSvcIfEnable = !eSvcIfEnable;
	*bVencThreadQuit = RK_FALSE;

	// Init VENC
	s32Ret = SAMPLE_COMM_VENC_CreateChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	// Bind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}

	/* rgn attach */
	s32Ret = venc_rgn_attach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_attach failure");
		return s32Ret;
	}

	return RK_SUCCESS;
}

static RK_S32 motionDeblur_test(RK_BOOL *bVencThreadQuit, SAMPLE_VENC_CTX_S *pVencCtx,
                                SAMPLE_VI_CTX_S *pViCtx) {

	RK_S32 s32Ret = RK_FAILURE;
	static RK_BOOL eMotionDeblurIfEnable = RK_TRUE;
	MPP_CHN_S stSrcChn, stDestChn;

	/* rgn detach */
	s32Ret = venc_rgn_detach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_detach failure");
		return s32Ret;
	}

	*bVencThreadQuit = RK_TRUE;
	if (pVencCtx->getStreamCbFunc) {
		pthread_join(pVencCtx->getStreamThread, RK_NULL);
	}

	// unBind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d unband to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}
	// Destroy venc
	s32Ret = SAMPLE_COMM_VENC_DestroyChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	if (eMotionDeblurIfEnable) {
		pVencCtx->bMotionDeblurIfEnable = eMotionDeblurIfEnable;
		RK_LOGE("------------------------------Motion Deblur enable");
	} else {
		pVencCtx->bMotionDeblurIfEnable = eMotionDeblurIfEnable;
		RK_LOGE("------------------------------Motion Deblur disable");
	}
	eMotionDeblurIfEnable = !eMotionDeblurIfEnable;
	*bVencThreadQuit = RK_FALSE;

	// Init VENC
	s32Ret = SAMPLE_COMM_VENC_CreateChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn 0 Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	// Bind vi and venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = pViCtx->s32DevId;
	stSrcChn.s32ChnId = pViCtx->s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = pVencCtx->s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi devid:%d chnid:%d band to venc chnid:%d failure", pViCtx->s32DevId,
		        pViCtx->s32ChnId, pVencCtx->s32ChnId);
		return s32Ret;
	}

	/* rgn attach */
	s32Ret = venc_rgn_attach();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc_rgn_attach failure");
		return s32Ret;
	}

	return RK_SUCCESS;
}

static RK_S32 vencForceIdr_test(SAMPLE_VENC_CTX_S *pVencCtx) {
	RK_S32 s32Ret = RK_FAILURE;

	RK_LOGE("-------------------venc set force idr");
	s32Ret = RK_MPI_VENC_RequestIDR(pVencCtx->s32ChnId, RK_FALSE);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_RequestIDR failure: %X", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

static RK_S32 vencSetRotation_test(SAMPLE_VENC_CTX_S *pVencCtx) {

	RK_S32 s32Ret = RK_FAILURE;
	ROTATION_E enRotation;

	memset(&enRotation, 0, sizeof(ROTATION_E));
	s32Ret = RK_MPI_VENC_GetChnRotation(pVencCtx->s32ChnId, &enRotation);
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
	s32Ret = RK_MPI_VENC_SetChnRotation(pVencCtx->s32ChnId, enRotation);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_SetChnRotation failure:%X", s32Ret);
		return RK_FAILURE;
	}
	RK_LOGE("--------------ChnRotation switch to %d (0->0, 1->90, 2->180, 3->270)",
	        enRotation);
	return RK_SUCCESS;
}

static RK_S32 rgn_init(void) {

	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 0;
	RK_U32 u32Height = 0;

	/* cover for vi[0]*/
	ctx->rgn[0].rgnHandle = 0;
	ctx->rgn[0].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[0].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[0].stMppChn.s32ChnId = VI_MAX_CHN_NUM;
	ctx->rgn[0].stMppChn.s32DevId = ctx->vi[0].s32DevId;
	ctx->rgn[0].stRegion.s32X = 0;        /* must be 2 aligned */
	ctx->rgn[0].stRegion.s32Y = 0;        /* must be 2 aligned */
	ctx->rgn[0].stRegion.u32Width = 256;  /* must be 8 aligned */
	ctx->rgn[0].stRegion.u32Height = 256; /* must be 8 aligned */
	ctx->rgn[0].u32Color = 0xFF0000;
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
	ctx->rgn[1].stMppChn.s32DevId = ctx->vi[0].s32DevId;
	ctx->rgn[1].stRegion.s32X = 0; /* must be 2 aligned */
	ctx->rgn[1].stRegion.s32Y =
	    RK_ALIGN_2(ctx->vi[0].u32Height - 256); /* must be 2 aligned */
	ctx->rgn[1].stRegion.u32Width = 256;        /* must be 8 aligned */
	ctx->rgn[1].stRegion.u32Height = 256;       /* must be 8 aligned */
	ctx->rgn[1].u32Color = 0xFFFF00;
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
	ctx->rgn[2].stMppChn.s32DevId = ctx->vi[0].s32DevId;
	ctx->rgn[2].stRegion.s32X =
	    RK_ALIGN_2(ctx->vi[0].u32Width - 256); /* must be 2 aligned */
	ctx->rgn[2].stRegion.s32Y = 0;             /* must be 2 aligned */
	ctx->rgn[2].stRegion.u32Width = 256;       /* must be 8 aligned */
	ctx->rgn[2].stRegion.u32Height = 256;      /* must be 8 aligned */
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
	ctx->rgn[3].stMppChn.s32DevId = ctx->vi[0].s32DevId;
	ctx->rgn[3].stRegion.s32X =
	    RK_ALIGN_2(ctx->vi[0].u32Width - 256); /* must be 2 aligned */
	ctx->rgn[3].stRegion.s32Y =
	    RK_ALIGN_2(ctx->vi[0].u32Height - 256); /* must be 2 aligned */
	ctx->rgn[3].stRegion.u32Width = 256;        /* must be 8 aligned */
	ctx->rgn[3].stRegion.u32Height = 256;       /* must be 8 aligned */
	ctx->rgn[3].u32Layer = 3;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[3]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[3].rgnHandle);
		return s32Ret;
	}

	// overlay for venc
	s32Ret =
	    SAMPLE_COMM_GetBmpResolution(gModeTest->inputBmp1Path, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
	}
	ctx->rgn[4].rgnHandle = 4;
	ctx->rgn[4].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[4].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[4].stMppChn.s32ChnId = ctx->venc[0].s32ChnId;
	ctx->rgn[4].stMppChn.s32DevId = 0;
	ctx->rgn[4].stRegion.s32X =
	    RK_ALIGN_2(ctx->venc[0].u32Width / 2);  // must be 2 aligned
	ctx->rgn[4].stRegion.s32Y = 0;              // must be 2 aligned
	ctx->rgn[4].stRegion.u32Width = u32Width;   // must be 8 aligned
	ctx->rgn[4].stRegion.u32Height = u32Height; // must be 8 aligned
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
	s32Ret =
	    SAMPLE_COMM_GetBmpResolution(gModeTest->inputBmp2Path, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
	}
	ctx->rgn[5].rgnHandle = 5;
	ctx->rgn[5].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[5].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[5].stMppChn.s32ChnId = ctx->venc[0].s32ChnId;
	ctx->rgn[5].stMppChn.s32DevId = 0;
	ctx->rgn[5].stRegion.s32X =
	    RK_ALIGN_2(ctx->venc[0].u32Width / 2); // must be 2 aligned
	ctx->rgn[5].stRegion.s32Y =
	    RK_ALIGN_2(ctx->venc[0].u32Height / 2); // must be 2 aligned
	ctx->rgn[5].stRegion.u32Width = u32Width;   // must be 8 aligned
	ctx->rgn[5].stRegion.u32Height = u32Height; // must be 8 aligned
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
	s32Ret =
	    SAMPLE_COMM_GetBmpResolution(gModeTest->inputBmp2Path, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
	}
	ctx->rgn[6].rgnHandle = 6;
	ctx->rgn[6].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[6].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[6].stMppChn.s32ChnId = ctx->venc[0].s32ChnId;
	ctx->rgn[6].stMppChn.s32DevId = 0;
	ctx->rgn[6].stRegion.s32X =
	    RK_ALIGN_2(ctx->venc[0].u32Width / 2); // must be 2 aligned
	ctx->rgn[6].stRegion.s32Y =
	    RK_ALIGN_2(ctx->venc[0].u32Height - u32Height); // must be 2 aligned
	ctx->rgn[6].stRegion.u32Width = u32Width;           // must be 8 aligned
	ctx->rgn[6].stRegion.u32Height = u32Height;         // must be 8 aligned
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

static RK_S32 rgn_attachAndDetach(RK_U32 s32RgnChnNum) {

	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 i = 0;

	for (i = 0; i < s32RgnChnNum; i++) {
		s32Ret = SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_DestroyChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[i].rgnHandle);
		}
	}

	/* rgn init */
	for (i = 0; i < s32RgnChnNum; i++) {
		s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_CreateChn Handle:%d Failure Ret:%#X",
			        ctx->rgn[i].rgnHandle, s32Ret);
		}
	}

	return RK_SUCCESS;
}

static RK_S32 rgn_change_posit(void) {
	RK_S32 s32Ret = RK_FAILURE;
	RGN_CHN_ATTR_S stChnAttr;
	memset(&stChnAttr, 0, sizeof(RGN_CHN_ATTR_S));

	if (gModeTest->s32ModuleTestType != 0) {
		return RK_SUCCESS;
	}

	/* change rgn[0] posit */
	s32Ret = RK_MPI_RGN_GetDisplayAttr(ctx->rgn[0].rgnHandle, &ctx->rgn[0].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[0] position ----RK_MPI_RGN_GetDisplayAttr failure:%X",
		        s32Ret);
		return s32Ret;
	}

	stChnAttr.unChnAttr.stCoverChn.stRect.s32X += 10;
	if (stChnAttr.unChnAttr.stCoverChn.stRect.s32X >
	    ctx->vi[0].u32Width - ctx->rgn[0].stRegion.u32Width - 50) {
		stChnAttr.unChnAttr.stCoverChn.stRect.s32X = ctx->rgn[0].stRegion.s32X;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[0].rgnHandle, &ctx->rgn[0].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[0] position ----RK_MPI_RGN_SetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	/* change rgn[1] posit*/
	s32Ret = RK_MPI_RGN_GetDisplayAttr(ctx->rgn[1].rgnHandle, &ctx->rgn[1].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[1] position ----RK_MPI_RGN_GetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	stChnAttr.unChnAttr.stCoverChn.stRect.s32Y -= 10;
	if (stChnAttr.unChnAttr.stCoverChn.stRect.s32Y < 0) {
		stChnAttr.unChnAttr.stCoverChn.stRect.s32Y = ctx->rgn[1].stRegion.s32Y;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[1].rgnHandle, &ctx->rgn[1].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[1] position ----RK_MPI_RGN_SetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	/* change rgn[2] posit*/
	s32Ret = RK_MPI_RGN_GetDisplayAttr(ctx->rgn[2].rgnHandle, &ctx->rgn[2].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[2] position ----RK_MPI_RGN_GetDisplayAttrs Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	stChnAttr.unChnAttr.stCoverChn.stRect.s32Y += 10;
	if (stChnAttr.unChnAttr.stCoverChn.stRect.s32Y >
	    ctx->vi[0].u32Height - ctx->rgn[2].stRegion.u32Height) {
		stChnAttr.unChnAttr.stCoverChn.stRect.s32Y = ctx->rgn[2].stRegion.s32Y;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[2].rgnHandle, &ctx->rgn[2].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[2] position ----RK_MPI_RGN_SetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	/* change rgn[3] posit */
	s32Ret = RK_MPI_RGN_GetDisplayAttr(ctx->rgn[3].rgnHandle, &ctx->rgn[3].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[3] position ----RK_MPI_RGN_GetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	stChnAttr.unChnAttr.stMosaicChn.stRect.s32X -= 10;
	if (stChnAttr.unChnAttr.stMosaicChn.stRect.s32X < 0) {
		stChnAttr.unChnAttr.stMosaicChn.stRect.s32X = ctx->rgn[3].stRegion.s32X;
	}

	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[3].rgnHandle, &ctx->rgn[3].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[3] position ----RK_MPI_RGN_SetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	/* change rgn[4] posit*/
	s32Ret = RK_MPI_RGN_GetDisplayAttr(ctx->rgn[4].rgnHandle, &ctx->rgn[4].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[4] position ----RK_MPI_RGN_GetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y += 10;
	if (stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y >
	    ctx->vi[0].u32Height - ctx->rgn[4].stRegion.u32Height - 50) {
		stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = ctx->rgn[4].stRegion.s32Y;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[4].rgnHandle, &ctx->rgn[4].stMppChn,
	                                   &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Change RGN[4] position ----RK_MPI_RGN_SetDisplayAttr Failure:%X",
		        s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

static void wait_module_test_switch_success(void) {
	for (RK_U32 i = 0; i < VENC_CHN_MAX; i++) {
		if (gModeTest->bWrapIfEnable && i == TDE_JPEG_CHNID) {
			continue;
		}
		pthread_mutex_lock(&g_frame_count_mutex[i]);
		gModeTest->u32VencGetFrameCount[i] = 0;
		pthread_mutex_unlock(&g_frame_count_mutex[i]);
		sem_wait(&g_sem_module_test[i]);
	}
}

static void *sample_demo_stresstest(void *pArgs) {
	prctl(PR_SET_NAME, "sample_demo_stress");
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32TestCount = 0;

	wait_module_test_switch_success();
	SAMPLE_COMM_DumpMeminfo("Enter sample_demo_stresstest", gModeTest->s32ModuleTestType);
	while (!gModeTest->bModuleTestThreadQuit) {

		switch (gModeTest->s32ModuleTestType) {
		case 1:
			s32Ret = pnMode_stressTest(gModeTest->s32CamId, gModeTest->eHdrMode,
			                           gModeTest->bMultictx, gModeTest->pIqFileDir);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("pnMode_stressTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 2:
			s32Ret = hdrMode_stressTest(gModeTest->s32CamId, gModeTest->eHdrMode,
			                            gModeTest->bMultictx, gModeTest->pIqFileDir);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("hdrMode_stressTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 3:
			s32Ret = frameRate_switchTest(&ctx->vi[0]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("frameRate_switchTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 4:
			s32Ret = ldchMode_test(gModeTest->s32CamId);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("ldchMode_test failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 5:
			s32Ret = vencResolution_switchTest(&ctx->tde, &ctx->venc[0], &ctx->vi[0],
			                                   &ctx->venc[3]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vencResolution_switchTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 6:
			g_rtsp_ifenbale = RK_FALSE;
			s32Ret = encode_typeSwitch(&gModeTest->bIfVencThreadQuit[0], &ctx->venc[0],
			                           &ctx->vi[0]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("encode_typeSwitch failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 7:
			s32Ret = smartP_switchTest(&gModeTest->bIfVencThreadQuit[0], &ctx->venc[0],
			                           &ctx->vi[0]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("smartP_switchTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 8:
			s32Ret = smartEncode_switchTest(&gModeTest->bIfVencThreadQuit[0],
			                                &ctx->venc[0], &ctx->vi[0]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("smartEncode_switchTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 9:
			s32Ret = motionDeblur_test(&gModeTest->bIfVencThreadQuit[0], &ctx->venc[0],
			                           &ctx->vi[0]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("motionDeblur_test failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 10:
			s32Ret = vencForceIdr_test(&ctx->venc[0]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vencForceIdr_test failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 11:
			s32Ret = vencSetRotation_test(&ctx->venc[0]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vencSetRotation_test failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		case 12:
			s32Ret = rgn_attachAndDetach(RGN_CHN_MAX);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("rgn_attachAndDetach failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			break;
		default:
			RK_LOGE("this test type is not support");
		}

		wait_module_test_switch_success();
		u32TestCount++;
		RK_LOGE("-----------------moduleTest switch success total:%d  now_count:%d",
		        gModeTest->u32ModuleTestLoop, u32TestCount);
		if (gModeTest->u32ModuleTestLoop > 0 &&
		    u32TestCount >= gModeTest->u32ModuleTestLoop) {
			RK_LOGE("------------------moduleTest: end", gModeTest->s32ModuleTestType);
			gModeTest->bModuleTestIfopen = RK_FALSE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	SAMPLE_COMM_DumpMeminfo("Exit sample_demo_stresstest", gModeTest->s32ModuleTestType);
	RK_LOGE("sample_demo_stresstest exit!!!");
	return RK_NULL;
}

static RK_S32 rtsp_init(CODEC_TYPE_E enCodecType) {
	RK_S32 i = 0;
	g_rtsplive = create_rtsp_demo(554);
	RK_CHAR rtspAddr[BUFFER_SIZE] = {0};

	for (i = 0; i < VENC_CHN_MAX; i++) {
		if (i == TDE_JPEG_CHNID || i == COMBO_JPEG_CHNID) {
			continue;
		}
		sprintf(rtspAddr, "/live/%d", i);
		g_rtsp_session[i] = rtsp_new_session(g_rtsplive, rtspAddr);
		if (enCodecType == RK_CODEC_TYPE_H264) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H264, RK_NULL, 0);
		} else if (enCodecType == RK_CODEC_TYPE_H265) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H265, RK_NULL, 0);
		} else {
			RK_LOGE("not support other type\n");
			g_rtsp_ifenbale = RK_FALSE;
			return RK_SUCCESS;
		}
		rtsp_sync_video_ts(g_rtsp_session[i], rtsp_get_reltime(), rtsp_get_ntptime());
		RK_LOGE("rtsp <%s> init success", rtspAddr);
	}
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

	gModeTest->u32ModuleTestLoop = -1;
	gModeTest->u32TestFrameCount = 500;

	for (RK_S32 i = 0; i < VENC_CHN_MAX; i++) {
		sem_init(&g_sem_module_test[i], 0, 0);

		if (pthread_mutex_init(&g_frame_count_mutex[i], RK_NULL) != 0) {
			RK_LOGE("mutex init failure \n");
			return RK_FAILURE;
		}
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

	for (RK_S32 i = 0; i < VENC_CHN_MAX; i++) {
		sem_destroy(&g_sem_module_test[i]);
		pthread_mutex_destroy(&g_frame_count_mutex[i]);
	}
	return RK_SUCCESS;
}

int main(int argc, char *argv[]) {

	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32CamId = 0;
	RK_S32 s32LoopCnt = -1;
	RK_U32 u32VideoWidth = 1920;
	RK_U32 u32VideoHeight = 1080;
	RK_U32 u32SubVideoWidth = 704;
	RK_U32 u32SubVideoHeight = 576;
	RK_U32 u32JpegWidth = 704;
	RK_U32 u32JpegHight = 576;
	RK_U32 u32IvsWidth = 576;
	RK_U32 u32IvsHeight = 324;
	RK_U32 u32IvaDetectFrameRate = 10;
	RK_U32 u32VencFps = 25;
	RK_U32 u32BitRate = 2 * 1024;
	RK_U32 u32ViBuffCnt = 2;
	RK_U32 u32VencBuffSize = 0;
	RK_U32 u32WrapLine = 4;
	RK_CHAR *pOutPathVenc = RK_NULL;
	RK_CHAR *pIqFileDir = RK_NULL;
	RK_BOOL bMultictx = RK_FALSE;
	RK_BOOL bWrapIfEnable = RK_FALSE;
	RK_BOOL bIfSmartpEnable = RK_FALSE;
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;
	MPP_CHN_S stSrcChn, stDestChn;
	rk_aiq_working_mode_t eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;

	pthread_t modeTest_thread_id, vi_venc_thread_id, ivs_detect_thread_id,
	    vi_iva_thread_id;

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
		case 'p':
			if (atoi(optarg)) {
				bIfSmartpEnable = RK_TRUE;
			} else {
				bIfSmartpEnable = RK_FALSE;
			}
			break;
		case 'v':
			u32ViBuffCnt = atoi(optarg);
			break;
		case 't' + 'l':
			gModeTest->u32ModuleTestLoop = atoi(optarg);
			break;
		case 'c':
			gModeTest->u32TestFrameCount = atoi(optarg);
			break;
		case 'd':
			u32IvaDetectFrameRate = atoi(optarg);
			break;
		case 'v' + 's':
			u32VencBuffSize = atoi(optarg);
			break;
		case 'w' + 'l':
			if (0 == atoi(optarg)) {
				u32WrapLine = 2;
			} else if (1 == atoi(optarg)) {
				u32WrapLine = 4;
			} else if (2 == atoi(optarg)) {
				u32WrapLine = 8;
			} else {
				RK_LOGE("ERROR: Invalid WrapLine Value.");
				print_usage(argv[0]);
				g_exit_result = RK_FAILURE;
				goto __PARAM_INIT_FAILED;
			}
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
		gModeTest->s32CamId = s32CamId;
		gModeTest->eHdrMode = eHdrMode;
		gModeTest->bMultictx = bMultictx;
		gModeTest->pIqFileDir = pIqFileDir;

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

	/* Init VI[0] */
	ctx->vi[0].u32Width = u32VideoWidth;
	ctx->vi[0].u32Height = u32VideoHeight;
	ctx->vi[0].s32DevId = 0;
	ctx->vi[0].u32PipeId = ctx->vi[0].s32DevId;
	ctx->vi[0].s32ChnId = 0;
	ctx->vi[0].stChnAttr.stIspOpt.stMaxSize.u32Width = u32VideoWidth;
	ctx->vi[0].stChnAttr.stIspOpt.stMaxSize.u32Height = u32VideoHeight;
	ctx->vi[0].stChnAttr.stIspOpt.u32BufCount = u32ViBuffCnt;
	ctx->vi[0].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi[0].stChnAttr.u32Depth = 1;
	ctx->vi[0].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi[0].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi[0].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi[0].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->vi[0].bWrapIfEnable = bWrapIfEnable;
	ctx->vi[0].u32BufferLine = ctx->vi[0].u32Height / u32WrapLine;
	s32Ret = SAMPLE_COMM_VI_CreateChn(&ctx->vi[0]);
	if (s32Ret != RK_SUCCESS) {
		g_exit_result = RK_FAILURE;
		RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%d", s32Ret);
		goto __FAILED;
	}

	/* Init VI[1] */
	ctx->vi[1].u32Width = u32SubVideoWidth;
	ctx->vi[1].u32Height = u32SubVideoHeight;
	ctx->vi[1].s32DevId = 0;
	ctx->vi[1].u32PipeId = ctx->vi[1].s32DevId;
	ctx->vi[1].s32ChnId = 1;
	ctx->vi[1].stChnAttr.stIspOpt.stMaxSize.u32Width = u32SubVideoWidth;
	ctx->vi[1].stChnAttr.stIspOpt.stMaxSize.u32Height = u32SubVideoHeight;
	ctx->vi[1].stChnAttr.stIspOpt.u32BufCount = 2;
	ctx->vi[1].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi[1].stChnAttr.u32Depth = 0;
	ctx->vi[1].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi[1].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi[1].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi[1].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	s32Ret = SAMPLE_COMM_VI_CreateChn(&ctx->vi[1]);
	if (s32Ret != RK_SUCCESS) {
		g_exit_result = RK_FAILURE;
		RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%d", s32Ret);
		goto __FAILED;
	}

	/* Init VI[2] */
	ctx->vi[2].u32Width = u32IvsWidth;
	ctx->vi[2].u32Height = u32IvsHeight;
	ctx->vi[2].s32DevId = 0;
	ctx->vi[2].u32PipeId = ctx->vi[2].s32DevId;
	ctx->vi[2].s32ChnId = 2;
	ctx->vi[2].stChnAttr.stIspOpt.stMaxSize.u32Width = u32IvsWidth;
	ctx->vi[2].stChnAttr.stIspOpt.stMaxSize.u32Height = u32IvsHeight;
	ctx->vi[2].stChnAttr.stIspOpt.u32BufCount = 2;
	ctx->vi[2].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi[2].stChnAttr.u32Depth = 2;
	ctx->vi[2].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi[2].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi[2].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi[2].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	s32Ret = SAMPLE_COMM_VI_CreateChn(&ctx->vi[2]);
	if (s32Ret != RK_SUCCESS) {
		g_exit_result = RK_FAILURE;
		RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%d", s32Ret);
		goto __FAILED;
	}

	/* Init ivs */
	ctx->ivs.s32ChnId = 0;
	ctx->ivs.stIvsAttr.enMode = IVS_MODE_MD_OD;
	ctx->ivs.stIvsAttr.u32PicWidth = u32IvsWidth;
	ctx->ivs.stIvsAttr.u32PicHeight = u32IvsHeight;
	ctx->ivs.stIvsAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->ivs.stIvsAttr.s32Gop = 30;
	ctx->ivs.stIvsAttr.bSmearEnable = RK_FALSE;
	ctx->ivs.stIvsAttr.bWeightpEnable = RK_FALSE;
	ctx->ivs.stIvsAttr.bMDEnable = RK_TRUE;
	ctx->ivs.stIvsAttr.s32MDInterval = 1;
	ctx->ivs.stIvsAttr.bMDNightMode = RK_TRUE;
	ctx->ivs.stIvsAttr.u32MDSensibility = 3;
	ctx->ivs.stIvsAttr.bODEnable = RK_TRUE;
	ctx->ivs.stIvsAttr.s32ODInterval = 1;
	ctx->ivs.stIvsAttr.s32ODPercent = 7;
	s32Ret = SAMPLE_COMM_IVS_Create(&ctx->ivs);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_IVS_Create failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
	}

#ifdef ROCKIVA
	/* Init iva */
	ctx->iva.u32ImageHeight = u32IvsWidth;
	ctx->iva.u32ImageWidth = u32IvsHeight;
	ctx->iva.u32DetectStartX = 0;
	ctx->iva.u32DetectStartY = 0;
	ctx->iva.u32DetectWidth = u32IvsWidth;
	ctx->iva.u32DetectHight = u32IvsHeight;
	ctx->iva.eImageTransform = ROCKIVA_IMAGE_TRANSFORM_NONE;
	ctx->iva.eImageFormat = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
	ctx->iva.eModeType = ROCKIVA_OBJECT_TYPE_PERSON;
	ctx->iva.u32IvaDetectFrameRate = u32IvaDetectFrameRate;
	ctx->iva.resultCallback = rkIvaEvent_callback;
	SAMPLE_COMM_IVA_Create(&ctx->iva);
#endif

	/* Init VENC[0] */
	ctx->venc[0].s32ChnId = 0;
	ctx->venc[0].u32Width = u32VideoWidth;
	ctx->venc[0].u32Height = u32VideoHeight;
	ctx->venc[0].u32Fps = u32VencFps;
	ctx->venc[0].u32Gop = 50;
	ctx->venc[0].u32BitRate = u32BitRate;
	ctx->venc[0].enCodecType = enCodecType;
	ctx->venc[0].enRcMode = enRcMode;
	ctx->venc[0].getStreamCbFunc = venc_get_stream;
	ctx->venc[0].s32loopCount = s32LoopCnt;
	ctx->venc[0].dstFilePath = pOutPathVenc;
	ctx->venc[0].bWrapIfEnable = bWrapIfEnable;
	ctx->venc[0].u32BufferLine = ctx->venc[0].u32Height / u32WrapLine;
	if (u32VencBuffSize) {
		ctx->venc[0].u32BuffSize = u32VencBuffSize;
	} else {
		ctx->venc[0].u32BuffSize = u32VideoWidth * u32VideoHeight / 2;
	}

	/*
	H264  66：Baseline  77：Main Profile 100：High Profile
	H265  0：Main Profile  1：Main 10 Profile
	MJPEG 0：Baseline
	*/
	if (RK_CODEC_TYPE_H264 != enCodecType) {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 100;
	}
	/* VENC_GOPMODE_SMARTP */
	if (bIfSmartpEnable) {
		ctx->venc[0].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		ctx->venc[0].stChnAttr.stGopAttr.s32VirIdrLen = ctx->venc[0].u32Gop / 2;
	} else {
		ctx->venc[0].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	}
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc[0]);

	/* Init VENC[1] */
	ctx->venc[1].s32ChnId = 1;
	ctx->venc[1].u32Width = u32SubVideoWidth;
	ctx->venc[1].u32Height = u32SubVideoHeight;
	ctx->venc[1].u32Fps = u32VencFps;
	ctx->venc[1].u32Gop = 50;
	ctx->venc[1].u32BitRate = u32BitRate;
	ctx->venc[1].enCodecType = enCodecType;
	ctx->venc[1].enRcMode = enRcMode;
	ctx->venc[1].getStreamCbFunc = venc_get_stream;
	ctx->venc[1].s32loopCount = s32LoopCnt;
	ctx->venc[1].dstFilePath = pOutPathVenc;
	ctx->venc[1].bWrapIfEnable = RK_FALSE;
	ctx->venc[1].u32BuffSize = u32SubVideoWidth * u32SubVideoHeight / 2;
	/*
	H264  66：Baseline  77：Main Profile 100：High Profile
	H265  0：Main Profile  1：Main 10 Profile
	MJPEG 0：Baseline
	*/
	if (RK_CODEC_TYPE_H264 != enCodecType) {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 100;
	}
	/* VENC_GOPMODE_SMARTP */
	if (bIfSmartpEnable) {
		ctx->venc[1].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		ctx->venc[1].stChnAttr.stGopAttr.s32VirIdrLen = ctx->venc[1].u32Gop / 2;
	} else {
		ctx->venc[1].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	}
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc[1]);

	/* Init VENC[2] */
	ctx->venc[2].s32ChnId = 2;
	ctx->venc[2].u32Width = u32JpegWidth;
	ctx->venc[2].u32Height = u32JpegHight;
	ctx->venc[2].u32Fps = 1;
	ctx->venc[2].u32Gop = 50;
	ctx->venc[2].u32Qfactor = 50;
	ctx->venc[2].u32BitRate = u32BitRate;
	ctx->venc[2].enCodecType = RK_CODEC_TYPE_JPEG;
	ctx->venc[2].enRcMode = VENC_RC_MODE_MJPEGCBR;
	ctx->venc[2].getStreamCbFunc = venc_get_stream;
	ctx->venc[2].s32loopCount = s32LoopCnt;
	ctx->venc[2].dstFilePath = pOutPathVenc;
	ctx->venc[2].bWrapIfEnable = RK_FALSE;
	/*
	H264  66：Baseline  77：Main Profile 100：High Profile
	H265  0：Main Profile  1：Main 10 Profile
	MJPEG 0：Baseline
	*/
	if (RK_CODEC_TYPE_H264 != enCodecType) {
		ctx->venc[2].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[2].stChnAttr.stVencAttr.u32Profile = 100;
	}
	/* VENC_GOPMODE_SMARTP */
	ctx->venc[2].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_INIT;
	if (!bWrapIfEnable) {
		SAMPLE_COMM_VENC_CreateChn(&ctx->venc[2]);
	}

	/* Init VENC[3] */
	ctx->venc[3].s32ChnId = 3;
	ctx->venc[3].u32Width = u32VideoWidth;
	ctx->venc[3].u32Height = u32VideoHeight;
	ctx->venc[3].u32Fps = u32VencFps;
	ctx->venc[3].u32Gop = 50;
	ctx->venc[3].u32Qfactor = 50;
	ctx->venc[3].u32BitRate = u32BitRate;
	ctx->venc[3].enCodecType = RK_CODEC_TYPE_JPEG;
	ctx->venc[3].enRcMode = VENC_RC_MODE_MJPEGCBR;
	ctx->venc[3].getStreamCbFunc = venc_get_stream;
	ctx->venc[3].s32loopCount = s32LoopCnt;
	ctx->venc[3].dstFilePath = pOutPathVenc;
	ctx->venc[3].bWrapIfEnable = bWrapIfEnable;
	ctx->venc[3].u32BufferLine = ctx->venc[3].u32Height / u32WrapLine;
	ctx->venc[3].bComboIfEnable = RK_TRUE;
	ctx->venc[3].u32ComboChnId = ctx->venc[0].s32ChnId;

	/*
	H264  66：Baseline  77：Main Profile 100：High Profile
	H265  0：Main Profile  1：Main 10 Profile
	MJPEG 0：Baseline
	*/
	if (RK_CODEC_TYPE_H264 != enCodecType) {
		ctx->venc[3].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[3].stChnAttr.stVencAttr.u32Profile = 100;
	}
	/* VENC_GOPMODE_SMARTP */
	ctx->venc[3].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_INIT;
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc[3]);

	/* tde init */
	ctx->tde.u32TdeWidth = u32JpegWidth;
	ctx->tde.u32TdeHeight = u32JpegHight;
	ctx->tde.u32SrcWidth = ctx->vi[0].u32Width;
	ctx->tde.u32SrcHeight = ctx->vi[0].u32Height;
	ctx->tde.enSrcPixelFormat = ctx->vi[0].stChnAttr.enPixelFormat;
	ctx->tde.enSrcCompMode = ctx->vi[0].stChnAttr.enCompressMode;
	if (!bWrapIfEnable) {
		SAMPLE_COMM_TDE_Create(&ctx->tde);
	}

	/* rgn init*/
	rgn_init();

	/* VI[0] bind VENC[0] */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[0].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[0].s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[0].s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
	}

	/* VI[1] bind VENC[1] */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[1].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[1].s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[1].s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
	}

	/* VI[2] bind IVS[0]*/
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[2].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[2].s32ChnId;
	stDestChn.enModId = RK_ID_IVS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->ivs.s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and IVS bind failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
	}

	if (!bWrapIfEnable) {
		RK_LOGE("wrap close");
		/* VI[0] Venc[2] therad launch */
		pthread_create(&vi_venc_thread_id, 0, vi_venc_thread, (void *)(&ctx->vi[0]));
	}

	/* ivs detect thread launch */
	pthread_create(&ivs_detect_thread_id, 0, ivs_detect_thread, (void *)&ctx->ivs);

#ifdef ROCKIVA
	// /* VI[2] IVA thread launch */
	pthread_create(&vi_iva_thread_id, 0, vi_iva_thread, RK_NULL);
#endif

	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestIfopen = RK_TRUE;
		pthread_create(&modeTest_thread_id, 0, sample_demo_stresstest,
		               (void *)(gModeTest));
	}

	printf("%s initial finish\n", __func__);

	while (!gModeTest->bIfMainThreadQuit) {
		sleep(1);
		rgn_change_posit();
	}

	/* rgn deinit*/
	rgn_deinit();

	/* mode_test_deinit */
	if (gModeTest->s32ModuleTestType) {
		gModeTest->bModuleTestThreadQuit = RK_TRUE;
		pthread_join(modeTest_thread_id, RK_NULL);
	}

#ifdef ROCKIVA
	/* Destroy IVA */
	gModeTest->bIfViIvaTHreadQuit = RK_TRUE;
	pthread_join(vi_iva_thread_id, RK_NULL);
	SAMPLE_COMM_IVA_Destroy(&ctx->iva);
#endif

	/* VI[2] unbind IVS[0]*/
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[2].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[2].s32ChnId;
	stDestChn.enModId = RK_ID_IVS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->ivs.s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and IVS bind failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}

	/* ivs detect thread exit*/
	gModeTest->bIvsDetectThreadQuit = RK_TRUE;
	pthread_join(ivs_detect_thread_id, RK_NULL);

	/* ivs chn destroy*/
	s32Ret = RK_MPI_IVS_DestroyChn(ctx->ivs.s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_IVS_DestroyChn failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}

	if (!bWrapIfEnable) {
		RK_LOGE("wrap close");
		/* venc[2] deinit and Destroy*/
		gModeTest->bIfVencThreadQuit[2] = RK_TRUE;
		pthread_join(ctx->venc[2].getStreamThread, RK_NULL);
		SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[2]);

		/* vi_venc thread quit*/
		gModeTest->bIfViThreadQuit = RK_TRUE;
		pthread_join(vi_venc_thread_id, RK_NULL);

		/* tde deinit*/
		SAMPLE_COMM_TDE_Destroy(&ctx->tde);
	}

	/* venc[3] deinit and Destroy */
	gModeTest->bIfVencThreadQuit[3] = RK_TRUE;
	pthread_join(ctx->venc[3].getStreamThread, RK_NULL);
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[3]);

	/* Venc[0] deinit */
	gModeTest->bIfVencThreadQuit[0] = RK_TRUE;
	pthread_join(ctx->venc[0].getStreamThread, RK_NULL);

	/* VI[0] unbind VENC[0] and destroy venc*/
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[0].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[0].s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[0].s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[0]);

	/* Venc[1] deinit */
	gModeTest->bIfVencThreadQuit[1] = RK_TRUE;
	pthread_join(ctx->venc[1].getStreamThread, RK_NULL);

	/* VI[1] unbind VENC[1] and destroy venc*/
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[1].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[1].s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[1].s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[1]);

	/* Destroy VI[0] VI[1] VI[2] */
	s32Ret = RK_MPI_VI_DisableChn(ctx->vi[0].u32PipeId, ctx->vi[0].s32ChnId);
	s32Ret |= RK_MPI_VI_DisableChn(ctx->vi[1].u32PipeId, ctx->vi[1].s32ChnId);
	s32Ret |= RK_MPI_VI_DisableChn(ctx->vi[2].u32PipeId, ctx->vi[2].s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_DisableChn failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}
	s32Ret = RK_MPI_VI_DisableDev(ctx->vi[0].s32DevId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_DisableChn failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}
	/* rtsp deinit */
	rtsp_deinit();

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
