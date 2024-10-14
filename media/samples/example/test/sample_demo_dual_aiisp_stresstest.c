#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "rtsp_demo.h"
#include "sample_comm.h"
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
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#define VI_NUM_MAX 4
#define VENC_CHN_MAX 4
#define VPSS_GRP_MAX 2
#define BUFFER_SIZE 255
#define RGN_NUM_MAX 4
#define GET_STREAM_TIMEOUT 2000
#define SEND_STREAM_TIMEOUT 2000
#define RGN_ATTACH_VPSS 0
#define RGN_ATTACH_VENC 1
#define RGN_ATTACH_NONE 2
#define CAM_0_INDEX 0
#define CAM_1_INDEX 1

#define TRACE_BEGIN() RK_LOGW("Enter\n")
#define TRACE_END() RK_LOGW("Exit\n")

typedef struct _rkThreadStatus {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfVencThreadQuit[VENC_CHN_MAX];
	RK_BOOL bIfViThreadQuit;
} ThreadStatus;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[VI_NUM_MAX];
	SAMPLE_VENC_CTX_S venc[VENC_CHN_MAX];
	SAMPLE_RGN_CTX_S rgn[RGN_NUM_MAX];
	SAMPLE_VPSS_CTX_S vpss[VPSS_GRP_MAX];
} SAMPLE_MPI_CTX_S;

typedef struct _rkCmdArgs {
	RK_U32 u32Main0Width;
	RK_U32 u32Main0Height;
	RK_U32 u32Main1Width;
	RK_U32 u32Main1Height;
	RK_U32 u32Sub0Width;
	RK_U32 u32Sub0Height;
	RK_U32 u32Sub1Width;
	RK_U32 u32Sub1Height;
	RK_U32 u32ViBuffCnt;
	RK_U32 u32Gop;
	RK_CHAR *pInPathBmp1;
	RK_CHAR *pInPathBmp2;
	RK_CHAR *pOutPathVenc;
	RK_CHAR *pIqFileDir;
	RK_CHAR *pAiispModelPath;
	RK_U32 u32AiispBuffCnt;
	RK_BOOL bMultictx;
	CODEC_TYPE_E enCodecType;
	VENC_RC_MODE_E enRcMode;
	RK_CHAR *pCodecName;
	RK_S32 s32CamId;
	RK_S32 s32loopCnt;
	RK_BOOL bEnableAIIsp;
	RK_S32 s32BitRate;
	RK_U32 u32VencFps;
	rk_aiq_working_mode_t eHdrMode;
	RK_U32 s32RgnAttachModule; // 0:vpss,1:venc
} RkCmdArgs;

typedef struct _rkModeTest {
	sem_t astSem[VENC_CHN_MAX];
	pthread_mutex_t astMutex[VENC_CHN_MAX];
	pthread_t s32ThreadId;
	RK_S32 s32ModuleTestType;
	RK_S32 s32ModuleTestLoop;
	RK_U32 u32TestFrameCount;
	RK_U32 u32VencGetFrameCount[VENC_CHN_MAX];
	RK_BOOL bIfModuleTestopen;
} RkModeTest;

/* global param */
static RkCmdArgs *g_cmd_args = RK_NULL;
static SAMPLE_MPI_CTX_S *g_mpi_ctx = RK_NULL;
static ThreadStatus *g_thread_status = RK_NULL;
static RkModeTest *g_mode_test = NULL;
static RK_S32 g_exit_result = RK_SUCCESS;

static pthread_mutex_t g_rtsp_mutex = {0};
static RK_BOOL g_rtsp_ifenbale = RK_FALSE;
rtsp_demo_handle g_rtsplive = RK_NULL;
static rtsp_session_handle g_rtsp_session[VENC_CHN_MAX] = {RK_NULL};

/* Forward function declaration used for stresstest code */
static RK_S32 isp_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 isp_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 vi_chn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 vi_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 vpss_chn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 vpss_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 venc_chn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 venc_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 bind_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 bind_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 rgn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 rgn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs);
static RK_S32 rtsp_init(CODEC_TYPE_E enCodecType);
static RK_S32 rtsp_deinit();

static RK_S32 aiisp_callback(rk_ainr_param *pAinrParam, RK_VOID *pPrivateData) {
	if (pAinrParam == RK_NULL) {
		RK_LOGE("pAinrParam is nullptr!\n");
		return RK_FAILURE;
	}
	RK_S32 s32Ret = RK_SUCCESS, s32CamId = (RK_S32)pPrivateData;
	memset(pAinrParam, 0, sizeof(rk_ainr_param));
	s32Ret = SAMPLE_COMM_ISP_GetAINrParams(s32CamId, pAinrParam);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Can't get ainr param!\n");
		return s32Ret;
	}
	RK_LOGD("aiisp cam %d enable %d\n", s32CamId, ((rk_ainr_param *)pAinrParam)->enable);
	return s32Ret;
}

static void program_handle_error(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> error exit!", func, line);
	g_exit_result = RK_FAILURE;
	g_thread_status->bIfMainThreadQuit = RK_TRUE;
}

static void program_normal_exit(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> normal exit!", func, line);
	g_thread_status->bIfMainThreadQuit = RK_TRUE;
}

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	program_normal_exit(__func__, __LINE__);
}

static void *venc_get_stream(void *pArgs) {
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 s32fd = 0;
	RK_S32 loopCount = 0;
	RK_VOID *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};
	sprintf(name, "venc_%d_get_stream", ctx->s32ChnId);
	prctl(PR_SET_NAME, name);
	RK_LOGE("venc_get_stream chnid:%d start\n", ctx->s32ChnId);

	if (ctx->dstFilePath) {
		memset(name, 0, BUFFER_SIZE);
		snprintf(name, sizeof(name), "/%s/venc_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
		s32fd = fileno(fp);
	}
	while (!g_thread_status->bIfVencThreadQuit[ctx->s32ChnId]) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					program_normal_exit(__func__, __LINE__);
					break;
				}
			}

			if (fp && !g_thread_status->bIfMainThreadQuit) {
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}

			// PrintStreamDetails(ctx->s32ChnId, ctx->stFrame.pstPack->u32Len);
			if (g_rtsp_ifenbale) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session[ctx->s32ChnId], pData,
				              ctx->stFrame.pstPack->u32Len, ctx->stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			} else {
				RK_LOGI("venc %d get_stream count: %d", ctx->s32ChnId, loopCount);
			}

			if (g_mode_test->bIfModuleTestopen) {
				pthread_mutex_lock(&g_mode_test->astMutex[ctx->s32ChnId]);
				g_mode_test->u32VencGetFrameCount[ctx->s32ChnId]++;
				pthread_mutex_unlock(&g_mode_test->astMutex[ctx->s32ChnId]);

				if (g_mode_test->u32VencGetFrameCount[ctx->s32ChnId] >=
				    g_mode_test->u32TestFrameCount) {
					sem_post(&g_mode_test->astSem[ctx->s32ChnId]);
				}
			}

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
	}

	if (fp) {
		fsync(s32fd);
		fclose(fp);
		fp = RK_NULL;
	}

	RK_LOGE("venc_get_stream chnid:%d exit\n", ctx->s32ChnId);
	return RK_NULL;
}

static RK_S32 pnMode_stressTest() {
	RK_S32 s32Ret = RK_SUCCESS;

	RK_MPI_VI_PauseChn(g_mpi_ctx->vi[0].u32PipeId, g_mpi_ctx->vi[0].s32ChnId);
	RK_MPI_VI_PauseChn(g_mpi_ctx->vi[1].u32PipeId, g_mpi_ctx->vi[1].s32ChnId);

	isp_deinit(g_mpi_ctx, g_cmd_args);
	s32Ret = isp_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("isp_init failure\n");
		return s32Ret;
	}

	s32Ret = RK_MPI_VI_ResumeChn(g_mpi_ctx->vi[0].u32PipeId, g_mpi_ctx->vi[0].s32ChnId);
	s32Ret |= RK_MPI_VI_ResumeChn(g_mpi_ctx->vi[1].u32PipeId, g_mpi_ctx->vi[1].s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_ResumeChn failure\n");
		return s32Ret;
	}

	RK_LOGE("<--------------PN mode switch success---------->");
	return RK_SUCCESS;
}

static RK_S32 hdrMode_stressTest() {

	RK_S32 s32Ret = RK_SUCCESS;
	// static rk_aiq_working_mode_t eNowHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
#if defined(RV1106)
// FIXME: Just return now for 1106, wait for HDR enable...
#if 0
	RK_MPI_VI_PauseChn(CAM_0_INDEX, 0);
	RK_MPI_VI_PauseChn(CAM_1_INDEX, 0);

	isp_deinit(g_mpi_ctx, g_cmd_args); 

	if (g_cmd_args->eHdrMode == RK_AIQ_WORKING_MODE_NORMAL)
		g_cmd_args->eHdrMode = RK_AIQ_WORKING_MODE_ISP_HDR2;
	else
		g_cmd_args->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
	s32Ret = isp_init(g_mpi_ctx, g_cmd_args); 
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_ISP_Init_Run failure\n");
		return s32Ret;
	}
	RK_MPI_VI_ResumeChn(CAM_0_INDEX, 0);
	RK_MPI_VI_ResumeChn(CAM_1_INDEX, 0);
#endif

#elif defined(RV1126)
	if (g_cmd_args->eHdrMode == RK_AIQ_WORKING_MODE_NORMAL)
		g_cmd_args->eHdrMode = RK_AIQ_WORKING_MODE_ISP_HDR2;
	else
		g_cmd_args->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
	s32Ret |= SAMPLE_COMM_ISP_SetWDRModeDyn(CAM_0_INDEX, g_cmd_args->eHdrMode);
	s32Ret |= SAMPLE_COMM_ISP_SetWDRModeDyn(CAM_1_INDEX, g_cmd_args->eHdrMode);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_ISP_SetWDRModeDyn failure:%#X cam:%d hdrmodr:%d", s32Ret,
		        g_cmd_args->s32CamId, g_cmd_args->eHdrMode);
		return s32Ret;
	}
#endif
	RK_LOGE("<------------switch to %d(normal:0 HDR2:16)--------------->",
	        g_cmd_args->eHdrMode);
	return s32Ret;
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

	srand(time(NULL));
	pstChnAttr.stFrameRate.s32DstFrameRate = rand() % 25 + 3;
	pstChnAttr.stFrameRate.s32SrcFrameRate = 25;
	s32Ret = RK_MPI_VI_SetChnAttr(ctx->u32PipeId, ctx->s32ChnId, &pstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_GetChnAttr failure:%X", s32Ret);
		return s32Ret;
	}

	RK_LOGE("---------------Framerate switch to: %d",
	        pstChnAttr.stFrameRate.s32DstFrameRate);

	return RK_SUCCESS;
}

static RK_S32 vpss_ai_isp_switchTest(SAMPLE_VPSS_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	AIISP_ATTR_S pstAIISPAttr;
	memset(&pstAIISPAttr, 0, sizeof(AIISP_ATTR_S));
	s32Ret = RK_MPI_VPSS_GetGrpAIISPAttr(ctx->s32GrpId, &pstAIISPAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_GetGrpAIISPAttr failure:%X", s32Ret);
		return s32Ret;
	}
	if (pstAIISPAttr.bEnable == RK_TRUE) {
		pstAIISPAttr.bEnable = RK_FALSE;
	} else {
		pstAIISPAttr.bEnable = RK_TRUE;
	}

	pstAIISPAttr.stAiIspCallback.pfUpdateCallback = (AIISP_CALLBACK)aiisp_callback;
	if (ctx->s32GrpId == 0)
		pstAIISPAttr.stAiIspCallback.pPrivateData = (void *)CAM_0_INDEX;
	else
		pstAIISPAttr.stAiIspCallback.pPrivateData = (void *)CAM_1_INDEX;
	pstAIISPAttr.pModelFilePath = g_cmd_args->pAiispModelPath;
	pstAIISPAttr.u32FrameBufCnt = g_cmd_args->u32AiispBuffCnt;

	s32Ret = RK_MPI_VPSS_SetGrpAIISPAttr(ctx->s32GrpId, &pstAIISPAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("VPSS GRP 0 RK_MPI_VPSS_SetGrpAIISPAttr failed with %#x!", s32Ret);
		return s32Ret;
	}

	RK_LOGE("---------------AIISP GRP %d switch to: %d(0->close, 1->enable)",
	        ctx->s32GrpId, pstAIISPAttr.bEnable);
	return RK_SUCCESS;
}

static RK_S32 vpss_chn0_resolution_switch_test(SAMPLE_VPSS_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	VPSS_CHN_ATTR_S pstChnAttr;
	RK_U32 u32SrcWidth = ctx->stVpssChnAttr[0].u32Width;
	RK_U32 u32SrcHeight = ctx->stVpssChnAttr[0].u32Height;
	RK_U32 u32DstWidth = 1280;
	RK_U32 u32DstHeight = 720;

	memset(&pstChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));
	s32Ret = RK_MPI_VPSS_GetChnAttr(ctx->s32GrpId, 0, &pstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_GetChnAttr failure:%X", s32Ret);
		return s32Ret;
	}
	if (pstChnAttr.u32Width == u32SrcWidth) {
		pstChnAttr.u32Width = u32DstWidth;
		pstChnAttr.u32Height = u32DstHeight;
	} else {
		pstChnAttr.u32Width = u32SrcWidth;
		pstChnAttr.u32Height = u32SrcHeight;
	}
	s32Ret = RK_MPI_VPSS_SetChnAttr(ctx->s32GrpId, 0, &pstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss set chn resolution failure");
		return s32Ret;
	}
	RK_LOGE("---------------VPSS %d resolution switch to: %dx%d", ctx->s32GrpId,
	        pstChnAttr.u32Width, pstChnAttr.u32Height);

	return RK_SUCCESS;
}

static RK_S32 vpss_venc_chn0_resolution_switch_test(RK_S32 s32VpssGrp, RK_S32 s32VpssChn,
                                                    RK_S32 s32VencChn) {
	RK_S32 s32Ret = RK_FAILURE;
	SAMPLE_VPSS_CTX_S *pVpssCtx = &g_mpi_ctx->vpss[s32VpssGrp];
	SAMPLE_VENC_CTX_S *pVencCtx = &g_mpi_ctx->venc[s32VencChn];
	RK_U32 u32SrcWidth = pVpssCtx->stVpssChnAttr[s32VpssChn].u32Width;
	RK_U32 u32SrcHeight = pVpssCtx->stVpssChnAttr[s32VpssChn].u32Height;
	RK_U32 u32DstWidth = 1280;
	RK_U32 u32DstHeight = 720;
	VPSS_CHN_ATTR_S VpsspstChnAttr;
	VENC_CHN_ATTR_S VencChnAttr;
	MPP_CHN_S stSrcChn, stDestChn;

	// rgn detach venc
	if (g_cmd_args->s32RgnAttachModule == RGN_ATTACH_VENC) {
		s32Ret = rgn_deinit(g_mpi_ctx, g_cmd_args);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("venc_rgn_detach failure");
			return s32Ret;
		}
	}
	// unBind vpss and venc
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = s32VpssGrp;
	stSrcChn.s32ChnId = s32VpssChn;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = s32VencChn;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss unbind to venc[0] failure");
		return s32Ret;
	}

	memset(&VpsspstChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

	/* vpss reset resolution */
	s32Ret = RK_MPI_VPSS_GetChnAttr(s32VpssGrp, s32VpssChn, &VpsspstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_GetChnAttr failure:%X", s32Ret);
		return s32Ret;
	}
	if (VpsspstChnAttr.u32Width == u32SrcWidth) {
		VpsspstChnAttr.u32Width = u32DstWidth;
		VpsspstChnAttr.u32Height = u32DstHeight;
	} else {
		VpsspstChnAttr.u32Width = u32SrcWidth;
		VpsspstChnAttr.u32Height = u32SrcHeight;
	}
	s32Ret = RK_MPI_VPSS_SetChnAttr(s32VpssGrp, s32VpssChn, &VpsspstChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss set chn resolution failure");
		return s32Ret;
	}
	RK_LOGE("vpss set chn resolution %dx%d-----\n", VpsspstChnAttr.u32Width,
	        VpsspstChnAttr.u32Height);

	memset(&VencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
	/* venc reset resolution */
	s32Ret = RK_MPI_VENC_GetChnAttr(s32VencChn, &VencChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_GetChnAttr failure:%X", s32Ret);
		return s32Ret;
	}
	pVencCtx->u32Width = VpsspstChnAttr.u32Width;
	pVencCtx->u32Height = VpsspstChnAttr.u32Height;
	VencChnAttr.stVencAttr.u32PicWidth = VpsspstChnAttr.u32Width;
	VencChnAttr.stVencAttr.u32VirWidth = VpsspstChnAttr.u32Width;
	VencChnAttr.stVencAttr.u32PicHeight = VpsspstChnAttr.u32Height;
	VencChnAttr.stVencAttr.u32VirHeight = VpsspstChnAttr.u32Height;
	s32Ret = RK_MPI_VENC_SetChnAttr(s32VencChn, &VencChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc set chn resolution failure");
		return s32Ret;
	}
	RK_LOGE("venc set chn resolution %dx%d-----\n", VpsspstChnAttr.u32Width,
	        VpsspstChnAttr.u32Height);

	// Bind vpss and venc
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = s32VpssGrp;
	stSrcChn.s32ChnId = s32VpssChn;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = s32VencChn;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss bind to venc[0] failure");
		return s32Ret;
	}
	// rgn attach venc
	if (g_cmd_args->s32RgnAttachModule == RGN_ATTACH_VENC) {
		s32Ret = rgn_init(g_mpi_ctx, g_cmd_args);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("venc_rgn_attach failure");
			return s32Ret;
		}
	}

	return RK_SUCCESS;
}

static RK_S32 encode_destroy_and_restart(CODEC_TYPE_E enCodecType,
                                         VENC_RC_MODE_E enRcMode, RK_U32 u32Profile,
                                         RK_S32 s32VpssGrp, RK_S32 s32VpssChn,
                                         RK_S32 s32VencChn) {
	RK_S32 s32Ret = RK_FAILURE;
	SAMPLE_VENC_CTX_S *pVencCtx = &g_mpi_ctx->venc[s32VencChn];
	MPP_CHN_S stSrcChn, stDestChn;

	// rgn detach venc
	if (g_cmd_args->s32RgnAttachModule == RGN_ATTACH_VENC) {
		s32Ret = rgn_deinit(g_mpi_ctx, g_cmd_args);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("venc_rgn_detach failure");
			return s32Ret;
		}
	}

	// Stop venc thread.
	g_thread_status->bIfVencThreadQuit[s32VencChn] = RK_TRUE;
	if (pVencCtx->getStreamCbFunc) {
		pthread_join(pVencCtx->getStreamThread, RK_NULL);
		pVencCtx->getStreamThread = 0;
	}

	// unBind vpss and venc
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = s32VpssGrp;
	stSrcChn.s32ChnId = s32VpssChn;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = s32VencChn;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss unbind to venc[%d] failure", pVencCtx->s32ChnId);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}
	// Destroy venc
	s32Ret = SAMPLE_COMM_VENC_DestroyChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn %d Failure s32Ret:%#X", s32VencChn, s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	pVencCtx->enCodecType = enCodecType;
	pVencCtx->enRcMode = enRcMode;
	pVencCtx->stChnAttr.stVencAttr.u32Profile = u32Profile;

	// Init VENC
	g_thread_status->bIfVencThreadQuit[pVencCtx->s32ChnId] = RK_FALSE;
	s32Ret = SAMPLE_COMM_VENC_CreateChn(pVencCtx);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_DestroyChn %d Failure s32Ret:%#X", pVencCtx->s32ChnId,
		        s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	// Bind vpss and venc
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = s32VpssGrp;
	stSrcChn.s32ChnId = s32VpssChn;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = s32VencChn;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss unbind to venc[%d] failure", pVencCtx->s32ChnId);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	// rgn attach venc
	if (g_cmd_args->s32RgnAttachModule == RGN_ATTACH_VENC) {
		s32Ret = rgn_init(g_mpi_ctx, g_cmd_args);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("venc_rgn_attach failure");
			return s32Ret;
		}
	}
	return s32Ret;
}

static RK_S32 encode_type_switch(RK_S32 s32VpssGrp, RK_S32 s32VpssChn,
                                 RK_S32 s32VencChn) {
	RK_S32 s32Ret = RK_FAILURE;
	static RK_U32 now_test_loop = 0;

	switch (now_test_loop % 2) {
	case 0: /* H264 CBR */
		RK_LOGE("-------------Switch To H264CBR---------------");

		s32Ret = encode_destroy_and_restart(RK_CODEC_TYPE_H264, VENC_RC_MODE_H264CBR, 100,
		                                    s32VpssGrp, s32VpssChn, s32VencChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("switch to 264_cbr failure");
			program_handle_error(__func__, __LINE__);
			return RK_FAILURE;
		}

	case 1: /* H265 CBR */
		RK_LOGE("-------------Switch To H265CBR---------------");

		s32Ret = encode_destroy_and_restart(RK_CODEC_TYPE_H265, VENC_RC_MODE_H265CBR, 0,
		                                    s32VpssGrp, s32VpssChn, s32VencChn);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("switch to 265_cbr failure");
			program_handle_error(__func__, __LINE__);
			return RK_FAILURE;
		}

	default:
		return RK_SUCCESS;
	}

	now_test_loop++;

	RK_LOGE("encode_type_switch-------------");
	return RK_SUCCESS;
}

static RK_S32 rgn_CreateAndDestory() {

	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = rgn_deinit(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rgn_deinit failed\n!");
		return s32Ret;
	}
	s32Ret = rgn_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rgn_init failed\n!");
		return s32Ret;
	}

	RK_LOGE("rgn_initAndDeinit switch test");
	return s32Ret;
}

static RK_S32 media_init(void) {
	RK_S32 s32Ret = RK_FAILURE;
	s32Ret = isp_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		printf("isp_init failure!\n");
		return s32Ret;
	}

	s32Ret = RK_MPI_SYS_Init();
	if (s32Ret != RK_SUCCESS) {
		printf("RK_MPI_SYS_Init failure!\n");
		return s32Ret;
	}

	// Initialize rtsp server.
	rtsp_init(g_cmd_args->enCodecType);
	// Initialize all pipeline nodes.
	s32Ret = vi_chn_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi init failed!\n");
		return s32Ret;
	}
	s32Ret = vpss_chn_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss init failed!\n");
		return s32Ret;
	}
	for (int i = 0; i < VENC_CHN_MAX; i++)
		g_thread_status->bIfVencThreadQuit[i] = RK_FALSE;
	s32Ret = venc_chn_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc init failed!\n");
		return s32Ret;
	}
	s32Ret = rgn_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rgn init failed!\n");
		return s32Ret;
	}
	// Bind all pipeline nodes.
	s32Ret = bind_init(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("bind init failed!\n");
		return s32Ret;
	}

	return s32Ret;
}

static RK_S32 media_deinit(void) {
	RK_S32 s32Ret = RK_FAILURE;
	// Destroy pipeline.
	for (int i = 0; i < VENC_CHN_MAX; i++) {
		g_thread_status->bIfVencThreadQuit[i] = RK_TRUE;
		pthread_join(g_mpi_ctx->venc[i].getStreamThread, RK_NULL);
		g_mpi_ctx->venc[i].getStreamThread = 0;
	}
	s32Ret = bind_deinit(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("bind deinit failed!\n");
		return s32Ret;
	}
	s32Ret = rgn_deinit(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rgn deinit failed!\n");
		return s32Ret;
	}
	s32Ret = venc_chn_deinit(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("venc deinit failed!\n");
		return s32Ret;
	}
	s32Ret = vpss_chn_deinit(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vpss deinit failed!\n");
		return s32Ret;
	}
	s32Ret = vi_chn_deinit(g_mpi_ctx, g_cmd_args);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("vi deinit failed!\n");
		return s32Ret;
	}
	rtsp_deinit();
	RK_MPI_SYS_Exit();
	isp_deinit(g_mpi_ctx, g_cmd_args);
	return s32Ret;
}

static RK_S32 media_deinit_init(void) {
	RK_S32 s32Ret = RK_FAILURE;
	TRACE_BEGIN();
	s32Ret = media_deinit();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("media_deinit failure\n");
		return s32Ret;
	}

	s32Ret = media_init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("media_init failure\n");
		return s32Ret;
	}
	TRACE_END();

	RK_LOGE("---------------------media_deinit_init StressTest");
	return s32Ret;
}

static void wait_module_test_switch_success(void) {
	for (RK_U32 i = 0; i < VENC_CHN_MAX; i++) {
		pthread_mutex_lock(&g_mode_test->astMutex[i]);
		g_mode_test->u32VencGetFrameCount[i] = 0;
		pthread_mutex_unlock(&g_mode_test->astMutex[i]);
		sem_wait(&g_mode_test->astSem[i]);
	}
}

static void *sample_demo_stresstest(void *pArgs) {
	prctl(PR_SET_NAME, "sample_demo_stress");
	RK_CHAR *pCTestModel = RK_NULL;
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32TestCount = 0;
	RK_S32 s32VpssGrp, s32VpssChn, s32VencChn;
	TRACE_BEGIN();
	wait_module_test_switch_success();
	SAMPLE_COMM_DumpMeminfo("Enter sample_demo_stresstest",
	                        g_mode_test->s32ModuleTestType);
	while (g_mode_test->bIfModuleTestopen) {
		switch (g_mode_test->s32ModuleTestType) {
		case 1:
			s32Ret = pnMode_stressTest();
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("pnMode_stressTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "pnMode_stressTest";
			break;
		case 2:
			s32Ret = hdrMode_stressTest();
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("hdrMode_stressTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "hdrMode_stressTest";
			break;
		case 3:
			s32Ret = frameRate_switchTest(&g_mpi_ctx->vi[0]);
			s32Ret |= frameRate_switchTest(&g_mpi_ctx->vi[1]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("frameRate_stressTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "frameRate_switchTest";
			break;
		case 4:
			s32Ret = RK_SUCCESS;
			s32Ret |= vpss_ai_isp_switchTest(&g_mpi_ctx->vpss[0]);
			s32Ret |= vpss_ai_isp_switchTest(&g_mpi_ctx->vpss[1]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vpss_aiisp switchTest failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "vpss_ai_isp_switchTest";
			break;
		case 5:
			s32Ret = vpss_chn0_resolution_switch_test(&g_mpi_ctx->vpss[0]);
			s32Ret |= vpss_chn0_resolution_switch_test(&g_mpi_ctx->vpss[1]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vpss_chn0_resolution_switch_test failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "vpss_chn0_resolution_switch_test";
			break;
		case 6:
			g_rtsp_ifenbale = RK_FALSE;
			s32VpssGrp = 0;
			s32VpssChn = 0;
			s32VencChn = 0;
			s32Ret =
			    vpss_venc_chn0_resolution_switch_test(s32VpssGrp, s32VpssChn, s32VencChn);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vpss_venc_chn0_resolution_switch_test failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			s32VpssGrp = 1;
			s32VpssChn = 0;
			s32VencChn = 2;
			s32Ret =
			    vpss_venc_chn0_resolution_switch_test(s32VpssGrp, s32VpssChn, s32VencChn);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vpss_venc_chn0_resolution_switch_test failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "vpss_venc_chn0_resolution_switch_test";
			break;
		case 7:
			s32VpssGrp = 0;
			s32VpssChn = 0;
			s32VencChn = 0;
			s32Ret = encode_type_switch(s32VpssGrp, s32VpssChn, s32VencChn);

			s32VpssGrp = 0;
			s32VpssChn = 1;
			s32VencChn = 1;
			s32Ret |= encode_type_switch(s32VpssGrp, s32VpssChn, s32VencChn);

			s32VpssGrp = 1;
			s32VpssChn = 0;
			s32VencChn = 2;
			s32Ret |= encode_type_switch(s32VpssGrp, s32VpssChn, s32VencChn);

			s32VpssGrp = 1;
			s32VpssChn = 1;
			s32VencChn = 3;
			s32Ret |= encode_type_switch(s32VpssGrp, s32VpssChn, s32VencChn);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("encode_type_switch failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "encode_type_switch";
			break;
		case 8:
			s32Ret = rgn_CreateAndDestory();
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("vpss rgn_initAndDeinit failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "rgn_initAndDeinit_test";
			break;
		case 9:
			s32Ret = media_deinit_init();
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("Media_deinit_and_init failure %X", s32Ret);
				program_handle_error(__func__, __LINE__);
				return RK_NULL;
			}
			pCTestModel = "Media_deinit_and_init";
			break;
		default:
			RK_LOGE("this test type is not support");
		}

		wait_module_test_switch_success();
		u32TestCount++;
		RK_LOGE("-----------------moduleTest:%s switch success total:%d  now_count:%d",
		        pCTestModel, g_mode_test->s32ModuleTestLoop, u32TestCount);
		if (g_mode_test->s32ModuleTestLoop > 0 &&
		    u32TestCount >= g_mode_test->s32ModuleTestLoop) {
			RK_LOGE("------------------moduleTest: %s end(pass)", pCTestModel);
			g_mode_test->bIfModuleTestopen = RK_FALSE;
			g_rtsp_ifenbale = RK_TRUE;
			program_normal_exit(__func__, __LINE__);
			break;
		}
	}
	SAMPLE_COMM_DumpMeminfo("Exit sample_demo_stresstest",
	                        g_mode_test->s32ModuleTestType);
	RK_LOGE("sample_demo_stresstest exit!!!");
	TRACE_END();
	return RK_NULL;
}

static RK_S32 rtsp_init(CODEC_TYPE_E enCodecType) {
	TRACE_BEGIN();
	RK_S32 i = 0;
	g_rtsplive = create_rtsp_demo(554);
	RK_CHAR rtspAddr[BUFFER_SIZE] = {0};

	for (i = 0; i < VENC_CHN_MAX; i++) {
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
	TRACE_END();
	return RK_SUCCESS;
}

static RK_S32 rtsp_deinit(void) {
	TRACE_BEGIN();
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	TRACE_END();
	return RK_SUCCESS;
}

static RK_S32 global_param_init(void) {
	TRACE_BEGIN();
	g_thread_status = (ThreadStatus *)malloc(sizeof(ThreadStatus));
	if (!g_thread_status) {
		RK_LOGI("malloc for g_thread_status failure\n");
		goto __global_init_fail;
	}
	memset(g_thread_status, 0, sizeof(ThreadStatus));

	// Allocate global ctx.
	g_mpi_ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	if (!g_mpi_ctx) {
		printf("ctx is null, malloc failure\n");
		goto __global_init_fail;
	}
	memset(g_mpi_ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	g_cmd_args = malloc(sizeof(RkCmdArgs));
	if (!g_cmd_args) {
		printf("g_cmd_args is null, malloc failure\n");
		goto __global_init_fail;
	}
	memset(g_cmd_args, 0, sizeof(RkCmdArgs));

	g_mode_test = malloc(sizeof(RkModeTest));
	if (!g_mode_test) {
		printf("g_mode_test is null, malloc failure\n");
		goto __global_init_fail;
	}
	memset(g_mode_test, 0, sizeof(RkModeTest));

	if (RK_SUCCESS != pthread_mutex_init(&g_rtsp_mutex, RK_NULL)) {
		RK_LOGE("pthread_mutex_init failure");
		goto __global_init_fail;
	}

	for (int i = 0; i != VENC_CHN_MAX; ++i) {
		pthread_mutex_init(&g_mode_test->astMutex[i], RK_NULL);
		sem_init(&g_mode_test->astSem[i], 0, 0);
	}

	TRACE_END();
	return RK_SUCCESS;

__global_init_fail:
	if (g_thread_status) {
		free(g_thread_status);
		g_thread_status = RK_NULL;
	}
	if (g_mpi_ctx) {
		free(g_mpi_ctx);
		g_mpi_ctx = NULL;
	}
	if (g_cmd_args) {
		free(g_cmd_args);
		g_cmd_args = NULL;
	}
	if (g_mode_test) {
		free(g_mode_test);
		g_mode_test = NULL;
	}
	TRACE_END();
	return RK_FAILURE;
}

static RK_S32 global_param_deinit(void) {
	TRACE_BEGIN();

	pthread_mutex_destroy(&g_rtsp_mutex);

	for (int i = 0; i != VENC_CHN_MAX; ++i) {
		pthread_mutex_destroy(&g_mode_test->astMutex[i]);
		sem_destroy(&g_mode_test->astSem[i]);
	}

	if (g_thread_status) {
		free(g_thread_status);
		g_thread_status = RK_NULL;
	}
	if (g_mpi_ctx) {
		free(g_mpi_ctx);
		g_mpi_ctx = NULL;
	}
	if (g_cmd_args) {
		free(g_cmd_args);
		g_cmd_args = NULL;
	}
	if (g_mode_test) {
		free(g_mode_test);
		g_mode_test = NULL;
	}

	TRACE_END();
	return RK_SUCCESS;
}

static RK_S32 rgn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 0;
	RK_U32 u32Height = 0;
	MPP_CHN_S stDstChn;
	OSD_SURFACE_S Surface;
	OSD_BITMAPFILEHEADER bmpFileHeader;
	OSD_BITMAPINFO bmpInfo;

	TRACE_BEGIN();
	/* Init RGN[0] */
	ctx->rgn[0].rgnHandle = 0;
	ctx->rgn[0].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[0].stRgnChnAttr.bShow = RK_TRUE;
	ctx->rgn[0].stRgnChnAttr.enType = COVER_RGN;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X = 0;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 0;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 256;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 256;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Layer = 1;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Color = 0x00f800; // green

	s32Ret = RK_MPI_RGN_Create(ctx->rgn[0].rgnHandle, &ctx->rgn[0].stRgnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", ctx->rgn[0].rgnHandle, s32Ret);
		return RK_FAILURE;
	}
	stDstChn.enModId = RK_ID_VI;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 0;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[0].rgnHandle, &stDstChn,
	                                &ctx->rgn[0].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[0].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[0].rgnHandle, &stDstChn,
	                                   &ctx->rgn[0].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[0].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}

	stDstChn.enModId = RK_ID_VI;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 1;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[0].rgnHandle, &stDstChn,
	                                &ctx->rgn[0].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[0].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[0].rgnHandle, &stDstChn,
	                                   &ctx->rgn[0].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[0].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}

	/* Init RGN[1] */
	ctx->rgn[1].rgnHandle = 1;
	ctx->rgn[1].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[1].stRgnChnAttr.bShow = RK_TRUE;
	ctx->rgn[1].stRgnChnAttr.enType = COVER_RGN;
	ctx->rgn[1].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X = 0;
	ctx->rgn[1].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 0;
	ctx->rgn[1].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 128;
	ctx->rgn[1].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 128;
	ctx->rgn[1].stRgnChnAttr.unChnAttr.stCoverChn.u32Layer = 2;
	ctx->rgn[1].stRgnChnAttr.unChnAttr.stCoverChn.u32Color = 0x00ffff; // blue

	s32Ret = RK_MPI_RGN_Create(ctx->rgn[1].rgnHandle, &ctx->rgn[1].stRgnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", ctx->rgn[1].rgnHandle, s32Ret);
		return RK_FAILURE;
	}
	stDstChn.enModId = RK_ID_VI;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 0;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[1].rgnHandle, &stDstChn,
	                                &ctx->rgn[1].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[1].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[1].rgnHandle, &stDstChn,
	                                   &ctx->rgn[1].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[0].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	stDstChn.enModId = RK_ID_VI;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 1;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[1].rgnHandle, &stDstChn,
	                                &ctx->rgn[1].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[1].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	s32Ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[1].rgnHandle, &stDstChn,
	                                   &ctx->rgn[1].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[1].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}

	/* Init RGN[2] */
	s32Ret = SAMPLE_COMM_GetBmpResolution(pArgs->pInPathBmp1, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		u32Width = 256;
		u32Height = 256;
	}
	ctx->rgn[2].rgnHandle = 2;
	ctx->rgn[2].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[2].stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_BGRA5551;
	ctx->rgn[2].stRgnAttr.unAttr.stOverlay.stSize.u32Width = u32Width;
	ctx->rgn[2].stRgnAttr.unAttr.stOverlay.stSize.u32Height = u32Width;
	ctx->rgn[2].stRgnChnAttr.bShow = RK_TRUE;
	ctx->rgn[2].stRgnChnAttr.enType = OVERLAY_RGN;
	ctx->rgn[2].stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 256;
	ctx->rgn[2].stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 256;
	ctx->rgn[2].stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
	ctx->rgn[2].stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	ctx->rgn[2].stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = 3;
	s32Ret = RK_MPI_RGN_Create(ctx->rgn[2].rgnHandle, &ctx->rgn[2].stRgnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", ctx->rgn[2].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	stDstChn.enModId = RK_ID_VENC;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 0;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[2].rgnHandle, &stDstChn,
	                                &ctx->rgn[2].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[2].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	stDstChn.enModId = RK_ID_VENC;
	stDstChn.s32ChnId = 2;
	stDstChn.s32DevId = 0;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[2].rgnHandle, &stDstChn,
	                                &ctx->rgn[2].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[2].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	if (pArgs->pInPathBmp1 &&
			get_bmp_info(pArgs->pInPathBmp1, &bmpFileHeader, &bmpInfo) == RK_SUCCESS) {

		ctx->rgn[2].stBitmap.pData =
		    malloc(4 * (bmpInfo.bmiHeader.biWidth) * (bmpInfo.bmiHeader.biHeight));
		if (RK_NULL == ctx->rgn[2].stBitmap.pData) {
			RK_LOGE("malloc osd memroy err!");
			return RK_FAILURE;
		}
		Surface.enColorFmt = OSD_COLOR_FMT_BGRA5551;
		create_surface_by_bitmap(pArgs->pInPathBmp1, &Surface,
		                         (RK_U8 *)(ctx->rgn[2].stBitmap.pData));
		ctx->rgn[2].stBitmap.u32Width = Surface.u16Width;
		ctx->rgn[2].stBitmap.u32Height = Surface.u16Height;
		ctx->rgn[2].stBitmap.enPixelFormat = RK_FMT_BGRA5551;

		s32Ret = RK_MPI_RGN_SetBitMap(ctx->rgn[2].rgnHandle, &ctx->rgn[2].stBitmap);
		if (s32Ret != RK_SUCCESS)
			RK_LOGE("RK_MPI_RGN_SetBitMap failed %#X\n", s32Ret);
	} else {
		ctx->rgn[2].stBitmap.pData = malloc(4 * u32Width * u32Height);

		if (RK_NULL == ctx->rgn[2].stBitmap.pData) {
			RK_LOGE("malloc osd memroy err!");
			return RK_FAILURE;
		}
		SAMPLE_COMM_FillImage(ctx->rgn[2].stBitmap.pData, u32Width,
		                      u32Height, u32Width,
		                      u32Height, RK_FMT_BGRA5551, 0);
		ctx->rgn[2].stBitmap.u32Width = u32Width;
		ctx->rgn[2].stBitmap.u32Height = u32Height;
		ctx->rgn[2].stBitmap.enPixelFormat = RK_FMT_BGRA5551;
		s32Ret = RK_MPI_RGN_SetBitMap(ctx->rgn[2].rgnHandle, &ctx->rgn[2].stBitmap);
		if (s32Ret != RK_SUCCESS)
			RK_LOGE("RK_MPI_RGN_SetBitMap failed %#X\n", s32Ret);
	}

	/* Init RGN[3] */
	s32Ret = SAMPLE_COMM_GetBmpResolution(pArgs->pInPathBmp2, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		u32Width = 256;
		u32Height = 256;
	}
	ctx->rgn[3].rgnHandle = 3;
	ctx->rgn[3].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[3].stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_BGRA5551;
	ctx->rgn[3].stRgnAttr.unAttr.stOverlay.stSize.u32Width = u32Width;
	ctx->rgn[3].stRgnAttr.unAttr.stOverlay.stSize.u32Height = u32Width;
	ctx->rgn[3].stRgnChnAttr.bShow = RK_TRUE;
	ctx->rgn[3].stRgnChnAttr.enType = OVERLAY_RGN;
	ctx->rgn[3].stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 512;
	ctx->rgn[3].stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 512;
	ctx->rgn[3].stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 255;
	ctx->rgn[3].stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 255;
	ctx->rgn[3].stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;
	s32Ret = RK_MPI_RGN_Create(ctx->rgn[3].rgnHandle, &ctx->rgn[3].stRgnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", ctx->rgn[3].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	stDstChn.enModId = RK_ID_VENC;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 0;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[3].rgnHandle, &stDstChn,
	                                &ctx->rgn[3].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[3].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	stDstChn.enModId = RK_ID_VENC;
	stDstChn.s32ChnId = 2;
	stDstChn.s32DevId = 0;
	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgn[3].rgnHandle, &stDstChn,
	                                &ctx->rgn[3].stRgnChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[3].rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}
	if (pArgs->pInPathBmp2 &&
			get_bmp_info(pArgs->pInPathBmp2, &bmpFileHeader, &bmpInfo) == RK_SUCCESS) {

		ctx->rgn[3].stBitmap.pData =
		    malloc(4 * (bmpInfo.bmiHeader.biWidth) * (bmpInfo.bmiHeader.biHeight));
		if (RK_NULL == ctx->rgn[3].stBitmap.pData) {
			RK_LOGE("malloc osd memroy err!");
			return RK_FAILURE;
		}
		Surface.enColorFmt = OSD_COLOR_FMT_BGRA5551;
		create_surface_by_bitmap(pArgs->pInPathBmp2, &Surface,
		                         (RK_U8 *)(ctx->rgn[3].stBitmap.pData));
		ctx->rgn[3].stBitmap.u32Width = Surface.u16Width;
		ctx->rgn[3].stBitmap.u32Height = Surface.u16Height;
		ctx->rgn[3].stBitmap.enPixelFormat = RK_FMT_BGRA5551;

		s32Ret = RK_MPI_RGN_SetBitMap(ctx->rgn[3].rgnHandle, &ctx->rgn[3].stBitmap);
		if (s32Ret != RK_SUCCESS)
			RK_LOGE("RK_MPI_RGN_SetBitMap failed %#X\n", s32Ret);
	} else {
		ctx->rgn[3].stBitmap.pData = malloc(4 * u32Width * u32Height);

		if (RK_NULL == ctx->rgn[3].stBitmap.pData) {
			RK_LOGE("malloc osd memroy err!");
			return RK_FAILURE;
		}
		SAMPLE_COMM_FillImage(ctx->rgn[3].stBitmap.pData, u32Width,
		                      u32Height, u32Width,
		                      u32Height, RK_FMT_BGRA5551, 0);
		ctx->rgn[3].stBitmap.u32Width = u32Width;
		ctx->rgn[3].stBitmap.u32Height = u32Height;
		ctx->rgn[3].stBitmap.enPixelFormat = RK_FMT_BGRA5551;
		s32Ret = RK_MPI_RGN_SetBitMap(ctx->rgn[3].rgnHandle, &ctx->rgn[3].stBitmap);
		if (s32Ret != RK_SUCCESS)
			RK_LOGE("RK_MPI_RGN_SetBitMap failed %#X\n", s32Ret);
	}

	TRACE_END();
	return s32Ret;
}

static RK_S32 rgn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	MPP_CHN_S stDstChn;
	TRACE_BEGIN();
	/* Destroy RGN[0] */
	stDstChn.enModId = RK_ID_VI;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 0;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[0].rgnHandle, &stDstChn);
	stDstChn.s32DevId = 1;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[0].rgnHandle, &stDstChn);
	RK_MPI_RGN_Destroy(ctx->rgn[0].rgnHandle);

	/* Destroy RGN[1] */
	stDstChn.s32DevId = 0;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[1].rgnHandle, &stDstChn);
	stDstChn.s32DevId = 1;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[1].rgnHandle, &stDstChn);
	RK_MPI_RGN_Destroy(ctx->rgn[1].rgnHandle);

	/* Destroy RGN[2] */
	stDstChn.enModId = RK_ID_VENC;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 0;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[2].rgnHandle, &stDstChn);
	stDstChn.s32ChnId = 2;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[2].rgnHandle, &stDstChn);
	RK_MPI_RGN_Destroy(ctx->rgn[2].rgnHandle);
	if (ctx->rgn[2].stBitmap.pData) {
		free(ctx->rgn[2].stBitmap.pData);
		ctx->rgn[2].stBitmap.pData = NULL;
	}

	/* Destroy RGN[3] */
	stDstChn.enModId = RK_ID_VENC;
	stDstChn.s32ChnId = 0;
	stDstChn.s32DevId = 0;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[3].rgnHandle, &stDstChn);
	stDstChn.s32ChnId = 2;
	RK_MPI_RGN_DetachFromChn(ctx->rgn[3].rgnHandle, &stDstChn);
	RK_MPI_RGN_Destroy(ctx->rgn[3].rgnHandle);
	if (ctx->rgn[3].stBitmap.pData) {
		free(ctx->rgn[3].stBitmap.pData);
		ctx->rgn[3].stBitmap.pData = NULL;
	}
	TRACE_END();
	return s32Ret;
}

static RK_S32 isp_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	if (g_cmd_args->pIqFileDir) {
		s32Ret = SAMPLE_COMM_ISP_Init(CAM_0_INDEX, g_cmd_args->eHdrMode,
		                              g_cmd_args->bMultictx, g_cmd_args->pIqFileDir);
		s32Ret |= SAMPLE_COMM_ISP_Run(CAM_0_INDEX);
		if (s32Ret != RK_SUCCESS) {
			printf("#ISP cam %d init failed!\n", CAM_0_INDEX);
			return s32Ret;
		}
		s32Ret = SAMPLE_COMM_ISP_Init(CAM_1_INDEX, g_cmd_args->eHdrMode,
		                              g_cmd_args->bMultictx, g_cmd_args->pIqFileDir);
		s32Ret |= SAMPLE_COMM_ISP_Run(CAM_1_INDEX);
		if (s32Ret != RK_SUCCESS) {
			printf("#ISP cam %d init failed!\n", CAM_1_INDEX);
			return s32Ret;
		}
	}
	return s32Ret;
}

static RK_S32 isp_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	if (g_cmd_args->pIqFileDir) {
		SAMPLE_COMM_ISP_Stop(CAM_0_INDEX);
		SAMPLE_COMM_ISP_Stop(CAM_1_INDEX);
	}
	return s32Ret;
}

static RK_S32 vi_chn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	TRACE_BEGIN();
	/* Init VI[0] */
	ctx->vi[0].u32Width = pArgs->u32Main0Width;
	ctx->vi[0].u32Height = pArgs->u32Main0Height;
	ctx->vi[0].s32DevId = CAM_0_INDEX;
	ctx->vi[0].u32PipeId = CAM_0_INDEX;
	ctx->vi[0].s32ChnId = 0;
	ctx->vi[0].stChnAttr.stIspOpt.stMaxSize.u32Width = pArgs->u32Main0Width;
	ctx->vi[0].stChnAttr.stIspOpt.stMaxSize.u32Height = pArgs->u32Main0Height;
	ctx->vi[0].stChnAttr.stIspOpt.u32BufCount = pArgs->u32ViBuffCnt;
	ctx->vi[0].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi[0].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi[0].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi[0].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi[0].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	s32Ret = SAMPLE_COMM_VI_CreateChn(&(ctx->vi[0]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VI_CreateChn 0 failure:%d", s32Ret);
	/* Init VI[1] */
	ctx->vi[1].u32Width = pArgs->u32Main1Width;
	ctx->vi[1].u32Height = pArgs->u32Main1Height;
	ctx->vi[1].s32DevId = CAM_1_INDEX;
	ctx->vi[1].u32PipeId = CAM_1_INDEX;
	ctx->vi[1].s32ChnId = 0;
	ctx->vi[1].stChnAttr.stIspOpt.stMaxSize.u32Width = pArgs->u32Main0Width;
	ctx->vi[1].stChnAttr.stIspOpt.stMaxSize.u32Height = pArgs->u32Main0Height;
	ctx->vi[1].stChnAttr.stIspOpt.u32BufCount = pArgs->u32ViBuffCnt;
	ctx->vi[1].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi[1].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi[1].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi[1].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi[1].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	s32Ret = SAMPLE_COMM_VI_CreateChn(&(ctx->vi[1]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VI_CreateChn 1 failure:%d", s32Ret);
	TRACE_END();
	return s32Ret;
}

static RK_S32 vi_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	TRACE_BEGIN();
	s32Ret = SAMPLE_COMM_VI_DestroyChn(&(ctx->vi[1]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VI_DestroyChn 1 failure:%d", s32Ret);
	s32Ret = SAMPLE_COMM_VI_DestroyChn(&(ctx->vi[0]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VI_DestroyChn 0 failure:%d", s32Ret);
	TRACE_END();
	return s32Ret;
}

static RK_S32 vpss_chn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	RK_S32 vpssGrpId;

	TRACE_BEGIN();
	// Init VPSS[0]
	vpssGrpId = 0;
	ctx->vpss[vpssGrpId].s32GrpId = vpssGrpId;
	ctx->vpss[vpssGrpId].s32ChnId = 0;
	ctx->vpss[vpssGrpId].enVProcDevType = VIDEO_PROC_DEV_RGA;
	ctx->vpss[vpssGrpId].stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE; // no compress
	ctx->vpss[vpssGrpId].s32ChnRotation[0] = ROTATION_0;

	// SET VPSS[0,0]
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_AUTO;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Width = pArgs->u32Main0Width;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Height = pArgs->u32Main0Height;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Depth = 0;

	// SET VPSS[0,1]
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enChnMode = VPSS_CHN_MODE_AUTO;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Width = pArgs->u32Sub0Width;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Height = pArgs->u32Sub0Height;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Depth = 0;

	s32Ret = SAMPLE_COMM_VPSS_CreateChn(&(ctx->vpss[vpssGrpId]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VPSS_CreateChn group 0 failed %#X\n", s32Ret);

	// Attach aiisp to vpss group 0.
	if (pArgs->bEnableAIIsp) {
		AIISP_ATTR_S stAIISPAttr;
		memset(&stAIISPAttr, 0, sizeof(AIISP_ATTR_S));
		stAIISPAttr.bEnable = RK_TRUE;
		stAIISPAttr.stAiIspCallback.pfUpdateCallback = (AIISP_CALLBACK)aiisp_callback;
		stAIISPAttr.stAiIspCallback.pPrivateData = (void *)CAM_0_INDEX;
		stAIISPAttr.pModelFilePath = pArgs->pAiispModelPath;
		stAIISPAttr.u32FrameBufCnt = pArgs->u32AiispBuffCnt;

		s32Ret = RK_MPI_VPSS_SetGrpAIISPAttr(vpssGrpId, &stAIISPAttr);
		if (RK_SUCCESS != s32Ret)
			RK_LOGE("VPSS GRP 0 RK_MPI_VPSS_SetGrpAIISPAttr failed with %#x!", s32Ret);
	}

	// Init VPSS[1]
	vpssGrpId = 1;
	ctx->vpss[vpssGrpId].s32GrpId = vpssGrpId;
	ctx->vpss[vpssGrpId].s32ChnId = 0;
	ctx->vpss[vpssGrpId].enVProcDevType = VIDEO_PROC_DEV_RGA;
	ctx->vpss[vpssGrpId].stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE; // no compress
	ctx->vpss[vpssGrpId].s32ChnRotation[0] = ROTATION_0;

	// SET VPSS[1,0]
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_AUTO;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Width = pArgs->u32Main1Width;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Height = pArgs->u32Main1Height;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Depth = 0;

	// SET VPSS[1,1]
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enChnMode = VPSS_CHN_MODE_AUTO;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Width = pArgs->u32Sub1Width;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Height = pArgs->u32Sub1Height;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Depth = 0;

	s32Ret = SAMPLE_COMM_VPSS_CreateChn(&(ctx->vpss[vpssGrpId]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VPSS_CreateChn group 0 failed %#X\n", s32Ret);
	// Attach aiisp to vpss group 1.
	if (pArgs->bEnableAIIsp) {
		AIISP_ATTR_S stAIISPAttr;
		memset(&stAIISPAttr, 0, sizeof(AIISP_ATTR_S));
		stAIISPAttr.bEnable = RK_TRUE;
		stAIISPAttr.stAiIspCallback.pfUpdateCallback = (AIISP_CALLBACK)aiisp_callback;
		stAIISPAttr.stAiIspCallback.pPrivateData = (void *)CAM_1_INDEX;
		stAIISPAttr.pModelFilePath = pArgs->pAiispModelPath;
		stAIISPAttr.u32FrameBufCnt = pArgs->u32AiispBuffCnt;

		s32Ret = RK_MPI_VPSS_SetGrpAIISPAttr(vpssGrpId, &stAIISPAttr);
		if (RK_SUCCESS != s32Ret)
			RK_LOGE("VPSS GRP 1 RK_MPI_VPSS_SetGrpAIISPAttr failed with %#x!", s32Ret);
	}

	TRACE_END();

	return s32Ret;
}

static RK_S32 vpss_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;

	TRACE_BEGIN();
	s32Ret = SAMPLE_COMM_VPSS_DestroyChn(&(ctx->vpss[1]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VPSS_DestroyChn group 1 failed %#X\n", s32Ret);
	s32Ret = SAMPLE_COMM_VPSS_DestroyChn(&(ctx->vpss[0]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VPSS_DestroyChn group 0 failed %#X\n", s32Ret);
	TRACE_END();

	return s32Ret;
}

static RK_S32 venc_chn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;

	TRACE_BEGIN();
	// Init VENC[0]
	ctx->venc[0].s32ChnId = 0;
	ctx->venc[0].u32Width = pArgs->u32Main0Width;
	ctx->venc[0].u32Height = pArgs->u32Main0Height;
	ctx->venc[0].u32Fps = pArgs->u32VencFps;
	ctx->venc[0].u32Gop = pArgs->u32Gop;
	ctx->venc[0].u32BitRate = pArgs->s32BitRate;
	ctx->venc[0].enCodecType = pArgs->enCodecType;
	ctx->venc[0].enRcMode = pArgs->enRcMode;
	ctx->venc[0].getStreamCbFunc = venc_get_stream;
	ctx->venc[0].s32loopCount = pArgs->s32loopCnt;
	ctx->venc[0].dstFilePath = pArgs->pOutPathVenc;
	ctx->venc[0].u32BuffSize = pArgs->u32Main0Width * pArgs->u32Main0Height / 2;
	ctx->venc[0].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc[0].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	if (RK_CODEC_TYPE_H264 != pArgs->enCodecType) {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 100;
	}
	s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc[0]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc0 failed %#X\n", s32Ret);

	// Init VENC[1]
	ctx->venc[1].s32ChnId = 1;
	ctx->venc[1].u32Width = pArgs->u32Sub0Width;
	ctx->venc[1].u32Height = pArgs->u32Sub0Height;
	ctx->venc[1].u32Fps = pArgs->u32VencFps;
	ctx->venc[1].u32Gop = pArgs->u32Gop;
	ctx->venc[1].u32BitRate = pArgs->s32BitRate;
	ctx->venc[1].enCodecType = pArgs->enCodecType;
	ctx->venc[1].enRcMode = pArgs->enRcMode;
	ctx->venc[1].getStreamCbFunc = venc_get_stream;
	ctx->venc[1].s32loopCount = pArgs->s32loopCnt;
	ctx->venc[1].dstFilePath = pArgs->pOutPathVenc;
	ctx->venc[1].u32BuffSize = pArgs->u32Sub0Width * pArgs->u32Sub0Height / 2;
	ctx->venc[1].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc[1].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	if (RK_CODEC_TYPE_H264 != pArgs->enCodecType) {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 100;
	}
	s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc[1]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc1 failed %#X\n", s32Ret);

	// Init VENC[2]
	ctx->venc[2].s32ChnId = 2;
	ctx->venc[2].u32Width = pArgs->u32Main1Width;
	ctx->venc[2].u32Height = pArgs->u32Main1Height;
	ctx->venc[2].u32Fps = pArgs->u32VencFps;
	ctx->venc[2].u32Gop = pArgs->u32Gop;
	ctx->venc[2].u32BitRate = pArgs->s32BitRate;
	ctx->venc[2].enCodecType = pArgs->enCodecType;
	ctx->venc[2].enRcMode = pArgs->enRcMode;
	ctx->venc[2].getStreamCbFunc = venc_get_stream;
	ctx->venc[2].s32loopCount = pArgs->s32loopCnt;
	ctx->venc[2].dstFilePath = pArgs->pOutPathVenc;
	ctx->venc[2].u32BuffSize = pArgs->u32Main1Width * pArgs->u32Main1Height / 2;
	ctx->venc[2].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc[2].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	if (RK_CODEC_TYPE_H264 != pArgs->enCodecType) {
		ctx->venc[2].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[2].stChnAttr.stVencAttr.u32Profile = 100;
	}
	s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc[2]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc2 failed %#X\n", s32Ret);

	// Init VENC[3]
	ctx->venc[3].s32ChnId = 3;
	ctx->venc[3].u32Width = pArgs->u32Sub1Width;
	ctx->venc[3].u32Height = pArgs->u32Sub1Height;
	ctx->venc[3].u32Fps = pArgs->u32VencFps;
	ctx->venc[3].u32Gop = pArgs->u32Gop;
	ctx->venc[3].u32BitRate = pArgs->s32BitRate;
	ctx->venc[3].enCodecType = pArgs->enCodecType;
	ctx->venc[3].enRcMode = pArgs->enRcMode;
	ctx->venc[3].getStreamCbFunc = venc_get_stream;
	ctx->venc[3].s32loopCount = pArgs->s32loopCnt;
	ctx->venc[3].dstFilePath = pArgs->pOutPathVenc;
	ctx->venc[3].u32BuffSize = pArgs->u32Sub1Width * pArgs->u32Sub1Height / 2;
	ctx->venc[3].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc[3].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	if (RK_CODEC_TYPE_H264 != pArgs->enCodecType) {
		ctx->venc[3].stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc[3].stChnAttr.stVencAttr.u32Profile = 100;
	}
	s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc[3]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc3 failed %#X\n", s32Ret);

	TRACE_END();

	return s32Ret;
}

static RK_S32 venc_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;

	TRACE_BEGIN();
	s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[3]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc3 failed %#X\n", s32Ret);

	s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[2]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc2 failed %#X\n", s32Ret);

	s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[1]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc1 failed %#X\n", s32Ret);

	s32Ret = SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[0]);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn venc0 failed %#X\n", s32Ret);
	TRACE_END();

	return s32Ret;
}

static RK_S32 bind_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	MPP_CHN_S stSrcChn, stDestChn;

	TRACE_BEGIN();
	// Bind VI[0] and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[0].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[0].s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vi0 to vpss0 failed");

	/* Bind VPSS[0, 0] and VENC[0] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vpss[0,0] to venc0 fail");

	/* Bind VPSS[0, 1] and VENC[1] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 1;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vpss[0,1] to venc1 fail");

	// Bind VI[1] and VPSS[1]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[1].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[1].s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = 1;
	stDestChn.s32ChnId = 0;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vi1 to vpss1 failed");

	/* Bind VPSS[1, 0] and VENC[2] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 1;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 2;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vpss[1,0] to venc2 fail");

	/* Bind VPSS[1, 1] and VENC[3] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 1;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 3;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vpss[1,1] to venc3 fail");

	TRACE_END();

	return s32Ret;
}

static RK_S32 bind_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	MPP_CHN_S stSrcChn, stDestChn;

	TRACE_BEGIN();
	/* UnBind VPSS[1, 1] and VENC[3] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 1;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 3;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	/* UnBind VPSS[1, 0] and VENC[2] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 1;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 2;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	// UnBind VI1 and VPSS[1]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[1].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[1].s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = 1;
	stDestChn.s32ChnId = 0;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	/* UnBind VPSS[0, 1] and VENC[1] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 1;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	/* UnBind VPSS[0, 0] and VENC[0] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	// UnBind VI and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi[0].s32DevId;
	stSrcChn.s32ChnId = ctx->vi[0].s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	TRACE_END();

	return s32Ret;
}

static RK_S32 sub_threads_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	TRACE_BEGIN();
	if (g_mode_test->s32ModuleTestType) {
		g_mode_test->bIfModuleTestopen = RK_TRUE;
		pthread_create(&g_mode_test->s32ThreadId, 0, sample_demo_stresstest, NULL);
	}
	TRACE_END();
	return RK_SUCCESS;
}

static RK_S32 sub_threads_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	TRACE_BEGIN();
	if (g_mode_test->s32ModuleTestType) {
		g_mode_test->bIfModuleTestopen = RK_FALSE;
		for (RK_U32 i = 0; i < VENC_CHN_MAX; i++)
			sem_post(&g_mode_test->astSem[i]);
		pthread_join(g_mode_test->s32ThreadId, RK_NULL);
		g_mode_test->s32ThreadId = 0;
	}
	for (int i = 0; i < VENC_CHN_MAX; i++) {
		g_thread_status->bIfVencThreadQuit[i] = true;
		pthread_join(ctx->venc[i].getStreamThread, RK_NULL);
		ctx->venc[i].getStreamThread = 0;
	}
	TRACE_END();
	return RK_SUCCESS;
}

static RK_CHAR optstr[] = "?::a::w:h:o:l:b:f:r:g:v:e:i:s:I:";

static const struct option long_options[] = {
    {"aiq", optional_argument, RK_NULL, 'a'},
    {"sensor_id", required_argument, NULL, 's'},
    {"width", required_argument, RK_NULL, 'w'},
    {"height", required_argument, RK_NULL, 'h'},
    {"encode", required_argument, RK_NULL, 'e'},
    {"output_path", required_argument, RK_NULL, 'o'},
    {"loop_count", required_argument, RK_NULL, 'l'},
    {"bitrate", required_argument, NULL, 'b'},
    {"fps", required_argument, RK_NULL, 'f'},
    {"vi_buff_cnt", required_argument, RK_NULL, 'v'},
    {"vi_chnid", required_argument, RK_NULL, 'v' + 'i'},
    {"rgn_attach_module", required_argument, RK_NULL, 'r'},
    {"gop", required_argument, RK_NULL, 'g'},
    {"enable_aiisp", required_argument, RK_NULL, 'e' + 'a'},
    {"aiisp_model_path", required_argument, RK_NULL, 'e' + 'j'},
    {"aiisp_buff_cnt", required_argument, RK_NULL, 'e' + 'k'},
    {"inputBmpPath1", required_argument, RK_NULL, 'i'},
    {"inputBmpPath2", required_argument, RK_NULL, 'I'},
    {"mode_test_type", required_argument, RK_NULL, 'm'},
    {"mode_test_loop", required_argument, RK_NULL, 't' + 'l'},
    {"test_frame_count", required_argument, RK_NULL, 'c'},
    {"help", optional_argument, RK_NULL, '?'},
    {RK_NULL, 0, RK_NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("example:\n");
	printf("\t%s --test_frame_count 50 --mode_test_loop 50 --mode_test_type 1\n", name);
	printf("\t-a | --aiq : enable aiq with dirpath provided, eg:-a /etc/iqfiles/, \n"
	       "\t		set dirpath empty to using path by default, without "
	       "this option aiq \n"
	       "\t		should run in other application\n");
	printf("\t-s | --sensor_id : sensor id\n");
	printf("\t-w | --width : mainStream width, must is sensor width, default 1920\n");
	printf("\t-h | --height : mainStream height, must is sensor height, default 1080\n");
	printf("\t-e | --encode: encode type, Default:h264cbr, Value:h264cbr, "
	       "h264vbr, h264avbr "
	       "h265cbr, h265vbr, h265avbr, mjpegcbr, mjpegvbr\n");
	printf("\t-b | --bitrate: encode bitrate, Default 4096\n");
	printf("\t-o | --output_path : encode output file path, Default: RK_NULL\n");
	printf("\t-l | --loop_count : when encoder output frameCounts equal to "
	       "<loop_count>, "
	       "process will exit. Default: -1\n");
	printf("\t-v | --vi_buff_cnt : main stream vi buffer num, Default: 2\n");
	printf("\t--vi_chnid : vi channel id, default: 0\n");
	printf("\t-r | --rgn_attach_module : where to attach rgn, 0: vpss, 1: venc, 2: "
	       "close. default: 1\n");
	printf("\t-g | --gop : venc GOP(group of pictures). default: 75\n");
	printf("\t-i | --inputpathbmp1 : input bmp file path. default: RK_NULL\n");
	printf("\t-I | --inputpathbmp2 : input bmp file path. default: RK_NULL\n");
	printf("\t-f | --fps : set fps, default: 25\n");
	printf("\t--enable_aiisp : enable ai isp, 0: close, 1: enable. default: 1\n");
	printf("\t--aiisp_model_path : aiisp model data path, default: /oem/usr/lib\n");
	printf("\t--aiisp_buff_cnt : aiisp buffer count, default: 2\n");
	printf("\t--test_frame_count : when encoder outputs frameCount equal to "
	       "<test_frame_count>, mode_test start next loop, default: 50\n");
	printf("\t--mode_test_loop : module test loop, default: -1\n");
	printf("\t-m | --mode_test_type : test type, 0:none, 1: pn_mode_test"
	       "\n\t 2: hdr_mode_test, 3: frameRate_switchTest"
	       "\n\t 4: vpss_ai_isp_switchTest, 5:vpss_chn0_resolution_switch_test"
	       "\n\t 6: vpss_venc_chn0_resolution_switch_test, 7: encode_type_switch"
	       "\n\t 8: rgn_init_and_deinit, 9: Media_deinit_and_init, Default: 0\n");
}

/******************************************************************************
 * function    : parse_cmd_args()
 * Description : Parse command line arguments.
 ******************************************************************************/
static RK_S32 parse_cmd_args(int argc, char **argv, RkCmdArgs *pArgs) {
	pArgs->u32Main0Width = 1920;
	pArgs->u32Main0Height = 1080;
	pArgs->u32Sub0Width = 720;
	pArgs->u32Sub0Height = 480;
	pArgs->u32Main1Width = 1920;
	pArgs->u32Main1Height = 1080;
	pArgs->u32Sub1Width = 720;
	pArgs->u32Sub1Height = 480;
	pArgs->u32ViBuffCnt = 5;
#if defined(RV1106)
	pArgs->u32ViBuffCnt = 2;
#endif
	pArgs->u32Gop = 75;
	pArgs->pInPathBmp1 = NULL;
	pArgs->pInPathBmp2 = NULL;
	pArgs->pOutPathVenc = NULL;
	pArgs->pIqFileDir = "/etc/iqfiles/";
	pArgs->bMultictx = RK_TRUE;
	pArgs->enCodecType = RK_CODEC_TYPE_H264;
	pArgs->enRcMode = VENC_RC_MODE_H264CBR;
	pArgs->pCodecName = "H264";
	pArgs->s32CamId = 0;
	pArgs->s32loopCnt = -1;
	pArgs->bEnableAIIsp = RK_TRUE;
	pArgs->s32BitRate = 4 * 1024;
	pArgs->u32VencFps = 25;
	pArgs->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
	pArgs->s32RgnAttachModule = RGN_ATTACH_VENC; // 0:vpss,1:venc
	pArgs->pAiispModelPath = "/oem/usr/lib/";
	pArgs->u32AiispBuffCnt = 1;

	// For stress test
	g_mode_test->s32ModuleTestType = 0;
	g_mode_test->u32TestFrameCount = 50;
	g_mode_test->s32ModuleTestLoop = -1;

	RK_S32 c = 0;
	while ((c = getopt_long(argc, argv, optstr, long_options, RK_NULL)) != -1) {
		const char *tmp_optarg = optarg;
		switch (c) {
		case 'a':
			if (!optarg && RK_NULL != argv[optind] && '-' != argv[optind][0]) {
				tmp_optarg = argv[optind++];
			}
			if (tmp_optarg) {
				pArgs->pIqFileDir = (char *)tmp_optarg;
			} else {
				pArgs->pIqFileDir = RK_NULL;
			}
			break;
		case 's':
			pArgs->s32CamId = atoi(optarg);
			break;
		case 'w':
			if (pArgs->s32CamId == 0)
				pArgs->u32Main0Width = atoi(optarg);
			else if (pArgs->s32CamId == 1)
				pArgs->u32Main1Width = atoi(optarg);
			else
				printf("Parse cmd line error, wrong sensor index %d!\n", pArgs->s32CamId);
			break;
		case 'h':
			if (pArgs->s32CamId == 0)
				pArgs->u32Main0Height = atoi(optarg);
			else if (pArgs->s32CamId == 1)
				pArgs->u32Main1Height = atoi(optarg);
			else
				printf("Parse cmd line error, wrong sensor index %d!\n", pArgs->s32CamId);
			break;
		case 'b':
			pArgs->s32BitRate = atoi(optarg);
			break;
		case 'e':
			if (!strcmp(optarg, "h264cbr")) {
				pArgs->enCodecType = RK_CODEC_TYPE_H264;
				pArgs->enRcMode = VENC_RC_MODE_H264CBR;
			} else if (!strcmp(optarg, "h264vbr")) {
				pArgs->enCodecType = RK_CODEC_TYPE_H264;
				pArgs->enRcMode = VENC_RC_MODE_H264VBR;
			} else if (!strcmp(optarg, "h264avbr")) {
				pArgs->enCodecType = RK_CODEC_TYPE_H264;
				pArgs->enRcMode = VENC_RC_MODE_H264AVBR;
			} else if (!strcmp(optarg, "h265cbr")) {
				pArgs->enCodecType = RK_CODEC_TYPE_H265;
				pArgs->enRcMode = VENC_RC_MODE_H265CBR;
			} else if (!strcmp(optarg, "h265vbr")) {
				pArgs->enCodecType = RK_CODEC_TYPE_H265;
				pArgs->enRcMode = VENC_RC_MODE_H265VBR;
			} else if (!strcmp(optarg, "h265avbr")) {
				pArgs->enCodecType = RK_CODEC_TYPE_H265;
				pArgs->enRcMode = VENC_RC_MODE_H265AVBR;
			} else {
				RK_LOGE("Invalid encoder type!");
				return RK_FAILURE;
			}
			break;
		case 'o':
			pArgs->pOutPathVenc = optarg;
			break;
		case 'l':
			pArgs->s32loopCnt = atoi(optarg);
			break;
		case 'f':
			pArgs->u32VencFps = atoi(optarg);
			break;
		case 'v':
			pArgs->u32ViBuffCnt = atoi(optarg);
			break;
		case 'g':
			pArgs->u32Gop = atoi(optarg);
			break;
		case 'r':
			pArgs->s32RgnAttachModule = atoi(optarg);
			break;
		case 'e' + 'a':
			pArgs->bEnableAIIsp = atoi(optarg);
			break;
		case 'e' + 'j':
			pArgs->pAiispModelPath = optarg;
			break;
		case 'e' + 'k':
			pArgs->u32AiispBuffCnt = atoi(optarg);
			break;
		case 'i':
			pArgs->pInPathBmp1 = optarg;
			break;
		case 'I':
			pArgs->pInPathBmp2 = optarg;
			break;
		case 'm':
			g_mode_test->s32ModuleTestType = atoi(optarg);
			break;
		case 'c':
			g_mode_test->u32TestFrameCount = atoi(optarg);
			break;
		case 't' + 'l':
			g_mode_test->s32ModuleTestLoop = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return RK_FAILURE;
		}
	}

	return RK_SUCCESS;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("bad arguments!\n");
		return RK_FAILURE;
	}

	if (global_param_init() != RK_SUCCESS) {
		printf("global_param_init failure\n");
		return RK_FAILURE;
	}

	if (parse_cmd_args(argc, argv, g_cmd_args) != RK_SUCCESS) {
		printf("parse_cmd_args failure\n");
		goto __ISP_INIT_FAILED;
	}


	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);

	printf("#Camera 0: width %d, height %d \n", g_cmd_args->u32Main0Width,
	       g_cmd_args->u32Main0Height);
	printf("#Camera 1: width %d, height %d \n", g_cmd_args->u32Main1Width,
	       g_cmd_args->u32Main1Height);
	printf("#CodecName:%s\n", g_cmd_args->pCodecName);
	printf("#Output Path: %s\n", g_cmd_args->pOutPathVenc);
	printf("#RGN_ATTACH: %d\n", g_cmd_args->s32RgnAttachModule);
	printf("#IQ Path: %s\n", g_cmd_args->pIqFileDir);
	if (g_cmd_args->pIqFileDir) {
		printf("#Rkaiq XML DirPath: %s\n", g_cmd_args->pIqFileDir);
		printf("#bMultictx: %d\n\n", g_cmd_args->bMultictx);
	}

	if (isp_init(g_mpi_ctx, g_cmd_args) != RK_SUCCESS) {
		printf("isp_init failure!\n");
		g_exit_result = RK_FAILURE;
		goto __ISP_INIT_FAILED;
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		printf("RK_MPI_SYS_Init failure!\n");
		g_exit_result = RK_FAILURE;
		goto __MPI_INIT_FAILED;
	}

	// Initialize rtsp server.
	rtsp_init(g_cmd_args->enCodecType);
	// Initialize all pipeline nodes.
	vi_chn_init(g_mpi_ctx, g_cmd_args);
	vpss_chn_init(g_mpi_ctx, g_cmd_args);
	venc_chn_init(g_mpi_ctx, g_cmd_args);
	rgn_init(g_mpi_ctx, g_cmd_args);
	// Bind all pipeline nodes.
	bind_init(g_mpi_ctx, g_cmd_args);
	// Start sub-threads after all initialization done.
	sub_threads_init(g_mpi_ctx, g_cmd_args);

	// Keep running ...
	while (!g_thread_status->bIfMainThreadQuit) {
		sleep(1);
	}

	// Destroy pipeline.
	sub_threads_deinit(g_mpi_ctx, g_cmd_args);
	bind_deinit(g_mpi_ctx, g_cmd_args);
	rgn_deinit(g_mpi_ctx, g_cmd_args);
	venc_chn_deinit(g_mpi_ctx, g_cmd_args);
	vpss_chn_deinit(g_mpi_ctx, g_cmd_args);
	vi_chn_deinit(g_mpi_ctx, g_cmd_args);
	rtsp_deinit();

	RK_MPI_SYS_Exit();

__MPI_INIT_FAILED:
	isp_deinit(g_mpi_ctx, g_cmd_args);
__ISP_INIT_FAILED:
	global_param_deinit();

	return g_exit_result;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
