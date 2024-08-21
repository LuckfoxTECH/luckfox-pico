#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "rockiva/rockiva_ba_api.h"
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

#define VENC_CHN_MAX 3
#define VPSS_GRP_MAX 2
#define BUFFER_SIZE 255
#define RGN_NUM_MAX 4
#define GET_STREAM_TIMEOUT 2000
#define SEND_STREAM_TIMEOUT 2000
#define RGN_ATTACH_VPSS 0
#define RGN_ATTACH_VENC 1
#define RGN_ATTACH_NONE 2

#define TRACE_BEGIN() RK_LOGW("Enter\n")
#define TRACE_END() RK_LOGW("Exit\n")

typedef struct _rkThreadStatus {
	RK_BOOL bIfMainThreadQuit;
	RK_BOOL bIfVencThreadQuit[VENC_CHN_MAX];
	RK_BOOL bIfViThreadQuit;
	RK_BOOL bIvsDetectThreadQuit;
	RK_BOOL bIfVpssIvaTHreadQuit;
} ThreadStatus;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
	SAMPLE_VENC_CTX_S venc[VENC_CHN_MAX];
	SAMPLE_RGN_CTX_S rgn[RGN_NUM_MAX];
	SAMPLE_VPSS_CTX_S vpss[VPSS_GRP_MAX];
	SAMPLE_IVS_CTX_S ivs;
	SAMPLE_IVA_CTX_S iva;
} SAMPLE_MPI_CTX_S;

typedef struct _rkCmdArgs {
	RK_U32 u32VideoWidth;
	RK_U32 u32VideoHeight;
	RK_U32 u32SubVideoWidth;
	RK_U32 u32SubVideoHeight;
	RK_U32 u32ThirdVideoWidth;
	RK_U32 u32ThirdVideoHeight;
	RK_U32 u32ViBuffCnt;
	RK_U32 u32IvsWidth;
	RK_U32 u32IvsHeight;
	RK_U32 u32Gop;
	RK_U32 u32IvaDetectFrameRate;
	RK_CHAR *pInPathBmp1;
	RK_CHAR *pInPathBmp2;
	RK_CHAR *pOutPathVenc;
	RK_CHAR *pIvaModelPath;
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
	RK_BOOL bEnableIva;
	RK_BOOL bEnableIvs;
	RK_S32 s32BitRate;
	RK_U32 u32VencFps;
	rk_aiq_working_mode_t eHdrMode;
	RK_U32 s32RgnAttachModule; // 0:vpss,1:venc
} RkCmdArgs;

/* global param */
static ThreadStatus *gPThreadStatus = RK_NULL;
static pthread_t g_iva_thread_id;
static pthread_t g_ivs_thread_id;
static RK_S32 g_exit_result = RK_SUCCESS;
static pthread_mutex_t g_rtsp_mutex = {0};
static RK_BOOL g_rtsp_ifenbale = RK_FALSE;
rtsp_demo_handle g_rtsplive = RK_NULL;
static rtsp_session_handle g_rtsp_session[VENC_CHN_MAX] = {RK_NULL};

static RK_S32 aiisp_callback(RK_VOID *pAinrParam, RK_VOID *pPrivateData) {
	if (pAinrParam == RK_NULL) {
		RK_LOGE("pAinrParam is nullptr!\n");
		return RK_FAILURE;
	}
	RK_S32 s32Ret = RK_SUCCESS;
	memset(pAinrParam, 0, sizeof(rk_ainr_param));
	s32Ret = SAMPLE_COMM_ISP_GetAINrParams(0, pAinrParam);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Can't get ainr param!\n");
		return s32Ret;
	}
	RK_LOGD("aiisp enable %d\n", ((rk_ainr_param *)pAinrParam)->enable);
	return s32Ret;
}

static void program_handle_error(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> error exit!", func, line);
	g_exit_result = RK_FAILURE;
	gPThreadStatus->bIfMainThreadQuit = RK_TRUE;
}

static void program_normal_exit(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> normal exit!", func, line);
	gPThreadStatus->bIfMainThreadQuit = RK_TRUE;
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
	while (!gPThreadStatus->bIfVencThreadQuit[ctx->s32ChnId]) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					program_normal_exit(__func__, __LINE__);
					break;
				}
			}

			if (fp && !gPThreadStatus->bIfMainThreadQuit) {
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
	gPThreadStatus = (ThreadStatus *)malloc(sizeof(ThreadStatus));
	if (!gPThreadStatus) {
		RK_LOGI("malloc for gPThreadStatus failure\n");
		goto __global_init_fail;
	}
	memset(gPThreadStatus, 0, sizeof(ThreadStatus));

	if (RK_SUCCESS != pthread_mutex_init(&g_rtsp_mutex, RK_NULL)) {
		RK_LOGE("pthread_mutex_init failure");
		goto __global_init_fail;
	}
	TRACE_END();
	return RK_SUCCESS;

__global_init_fail:
	if (gPThreadStatus) {
		free(gPThreadStatus);
		gPThreadStatus = RK_NULL;
	}
	TRACE_END();
	return RK_FAILURE;
}

static RK_S32 global_param_deinit(void) {
	TRACE_BEGIN();
	if (gPThreadStatus) {
		free(gPThreadStatus);
		gPThreadStatus = RK_NULL;
	}

	pthread_mutex_destroy(&g_rtsp_mutex);

	TRACE_END();
	return RK_SUCCESS;
}

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

static void rkIvaFrame_releaseCallBack(const RockIvaReleaseFrames *releaseFrames,
                                       void *userdata) {
	/* when iva handle out of the video frame，this func will be called*/
	RK_S32 s32Ret = RK_SUCCESS;
	for (RK_S32 i = 0; i < releaseFrames->count; i++) {
		if (!releaseFrames->frames[i].extData) {
			RK_LOGE("---------error release frame is null");
			program_handle_error(__func__, __LINE__);
			continue;
		}
		s32Ret = RK_MPI_VPSS_ReleaseChnFrame(1, 1, releaseFrames->frames[i].extData);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_ReleaseChnFrame failure:%#X", s32Ret);
			program_handle_error(__func__, __LINE__);
		}
		free(releaseFrames->frames[i].extData);
	}
}

static void *vpss_iva_thread(void *pArgs) {
	prctl(PR_SET_NAME, "vpss_iva_thread");
	SAMPLE_MPI_CTX_S *ctx = (SAMPLE_MPI_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32Fd = 0;
	RockIvaImage ivaImage;
	RK_U32 u32Loopcount = 0;
	RK_U32 u32GetOneFrameTime = 1000 / ctx->iva.u32IvaDetectFrameRate;
	VIDEO_FRAME_INFO_S *stVpssFrame = NULL;

	// IVA attach with VPSS[1, 1]
	while (!gPThreadStatus->bIfVpssIvaTHreadQuit) {
		s32Ret = RK_MPI_VPSS_GetChnFrame(1, 1, &ctx->vpss[1].stChnFrameInfos, -1);
		if (s32Ret == RK_SUCCESS) {
			stVpssFrame = (VIDEO_FRAME_INFO_S *)malloc(sizeof(VIDEO_FRAME_INFO_S));
			if (!stVpssFrame) {
				RK_LOGE("-----error malloc fail for stVpssFrame");
				RK_MPI_VPSS_ReleaseChnFrame(1, 1, &ctx->vpss[1].stChnFrameInfos);
				continue;
			}
			memcpy(stVpssFrame, &ctx->vpss[1].stChnFrameInfos,
			       sizeof(VIDEO_FRAME_INFO_S));
			s32Fd = RK_MPI_MB_Handle2Fd(stVpssFrame->stVFrame.pMbBlk);
			memset(&ivaImage, 0, sizeof(RockIvaImage));
			ivaImage.info.transformMode = ctx->iva.eImageTransform;
			ivaImage.info.width = stVpssFrame->stVFrame.u32Width;
			ivaImage.info.height = stVpssFrame->stVFrame.u32Height;
			ivaImage.info.format = ctx->iva.eImageFormat;
			ivaImage.frameId = u32Loopcount;
			ivaImage.dataAddr = NULL;
			ivaImage.dataPhyAddr = NULL;
			ivaImage.dataFd = s32Fd;
			ivaImage.extData = stVpssFrame;
			s32Ret = ROCKIVA_PushFrame(ctx->iva.ivahandle, &ivaImage, NULL);
			u32Loopcount++;
		}
		usleep(u32GetOneFrameTime * 1000);
	}

	RK_LOGE("vpss_iva_thread exit !!!");
	return RK_NULL;
}

static void *ivs_detect_thread(void *pArgs) {
	RK_LOGI("enter ive detect thread------------>\n");
	prctl(PR_SET_NAME, "ivs_detect_thread");
	SAMPLE_IVS_CTX_S *ctx = (SAMPLE_IVS_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	IVS_RESULT_INFO_S stResults;
	RK_U32 u32IvsDetectCount = 0;
	IVS_CHN_ATTR_S pstAttr;

	memset(&pstAttr, 0, sizeof(IVS_CHN_ATTR_S));

	RK_MPI_IVS_GetChnAttr(ctx->s32ChnId, &pstAttr);

	RK_LOGE("odIfEnable:%d ", pstAttr.bODEnable);

	while (!gPThreadStatus->bIvsDetectThreadQuit) {
		memset(&stResults, 0, sizeof(IVS_RESULT_INFO_S));
		s32Ret = RK_MPI_IVS_GetResults(ctx->s32ChnId, &stResults, GET_STREAM_TIMEOUT);
		if (s32Ret == RK_SUCCESS) {
			u32IvsDetectCount++;
			// RK_LOGD("s32ReNum: %d", stResults.s32ResultNum);
			if (stResults.s32ResultNum == 1) {
				for (int i = 0; i < stResults.pstResults->stMdInfo.u32RectNum; i++) {
					RK_LOGD("%d: [%d, %d, %d, %d]\n", i,
					        stResults.pstResults->stMdInfo.stRect[i].s32X,
					        stResults.pstResults->stMdInfo.stRect[i].s32Y,
					        stResults.pstResults->stMdInfo.stRect[i].u32Width,
					        stResults.pstResults->stMdInfo.stRect[i].u32Height);
				}
			}

			RK_MPI_IVS_ReleaseResults(ctx->s32ChnId, &stResults);

		} else {
			RK_LOGE("RK_MPI_IVS_GetResults failure:%X", s32Ret);
		}
	}

	RK_LOGE("ivs_detect_thread exit");
	return RK_NULL;
}

static RK_S32 rgn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_FAILURE;
	MPP_CHN_S RGN_CHN;
	RK_U32 u32Width = 0;
	RK_U32 u32Height = 0;

	TRACE_BEGIN();
	if (pArgs->s32RgnAttachModule == RGN_ATTACH_VENC) {
		RGN_CHN.enModId = RK_ID_VENC;
		RGN_CHN.s32ChnId = 0;
		RGN_CHN.s32DevId = 0;
	} else if (pArgs->s32RgnAttachModule == RGN_ATTACH_VPSS) {
		RGN_CHN.enModId = RK_ID_VPSS;
		RGN_CHN.s32ChnId = VPSS_MAX_CHN_NUM;
		RGN_CHN.s32DevId = 0;
	} else {
		RK_LOGE("RGN closed");
		TRACE_END();
		return s32Ret;
	}

	/* Init RGN[0] */
	ctx->rgn[0].rgnHandle = 0;
	ctx->rgn[0].stRgnAttr.enType = COVER_RGN;
// RV1106 just support attach cover in VI channel.
#if defined(RV1106)
	ctx->rgn[0].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[0].stMppChn.s32ChnId = 0;
	ctx->rgn[0].stMppChn.s32DevId = 0;
#else
	ctx->rgn[0].stMppChn.enModId = RGN_CHN.enModId;
	ctx->rgn[0].stMppChn.s32ChnId = RGN_CHN.s32ChnId;
	ctx->rgn[0].stMppChn.s32DevId = RGN_CHN.s32DevId;
#endif
	ctx->rgn[0].stRegion.s32X = 0;        // must be 16 aligned
	ctx->rgn[0].stRegion.s32Y = 0;        // must be 16 aligned
	ctx->rgn[0].stRegion.u32Width = 256;  // must be 16 aligned
	ctx->rgn[0].stRegion.u32Height = 256; // must be 16 aligned
	ctx->rgn[0].u32Color = 0x00f800;      // green
	ctx->rgn[0].u32Layer = 1;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[0]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[0].rgnHandle);
		TRACE_END();
		return s32Ret;
	}

	ctx->rgn[1].rgnHandle = 1;
	ctx->rgn[1].stRgnAttr.enType = COVER_RGN;
#if defined(RV1106)
	ctx->rgn[1].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[1].stMppChn.s32ChnId = 0;
	ctx->rgn[1].stMppChn.s32DevId = 0;
#else
	ctx->rgn[1].stMppChn.enModId = RGN_CHN.enModId;
	ctx->rgn[1].stMppChn.s32ChnId = RGN_CHN.s32ChnId;
	ctx->rgn[1].stMppChn.s32DevId = RGN_CHN.s32DevId;
#endif
	ctx->rgn[1].stRegion.s32X = 0;
	ctx->rgn[1].stRegion.s32Y = 0;        // must be 16 aligned
	ctx->rgn[1].stRegion.u32Width = 128;  // must be 16 aligned
	ctx->rgn[1].stRegion.u32Height = 128; // must be 16 aligned
	ctx->rgn[1].u32Color = 0x00ffff;      // blue
	ctx->rgn[1].u32Layer = 2;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[1]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[1].rgnHandle);
		TRACE_END();
		return s32Ret;
	}

	s32Ret = SAMPLE_COMM_GetBmpResolution(pArgs->pInPathBmp1, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		u32Width = 256;
		u32Height = 256;
	}
	ctx->rgn[2].rgnHandle = 2;
	ctx->rgn[2].stRgnAttr.unAttr.stOverlay.u32CanvasNum = 2;
	ctx->rgn[2].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[2].stMppChn.enModId = RGN_CHN.enModId;
	ctx->rgn[2].stMppChn.s32ChnId = RGN_CHN.s32ChnId;
	ctx->rgn[2].stMppChn.s32DevId = RGN_CHN.s32DevId;
	ctx->rgn[2].stRegion.s32X = 256;            // must be 16 aligned
	ctx->rgn[2].stRegion.s32Y = 256;            // must be 16 aligned
	ctx->rgn[2].stRegion.u32Width = u32Width;   // must be 8 aligned
	ctx->rgn[2].stRegion.u32Height = u32Height; // must be 8 aligned
	ctx->rgn[2].u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn[2].u32BgAlpha = 128;
	ctx->rgn[2].u32FgAlpha = 128;
	ctx->rgn[2].u32Layer = 3;
	ctx->rgn[2].srcFileBmpName = pArgs->pInPathBmp1;
	if (pArgs->pInPathBmp1) {
		s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[2]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[2].rgnHandle);
			TRACE_END();
			return s32Ret;
		}
	} else {
		RK_LOGE("input_bmp1 file is NULL, overlay rgn skips");
	}

	s32Ret = SAMPLE_COMM_GetBmpResolution(pArgs->pInPathBmp2, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		u32Width = 128;
		u32Height = 128;
	}
	/* Init RGN[3] */
	ctx->rgn[3].rgnHandle = 3;
	ctx->rgn[3].stRgnAttr.unAttr.stOverlay.u32CanvasNum = 2;
	ctx->rgn[3].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[3].stMppChn.enModId = RGN_CHN.enModId;
	ctx->rgn[3].stMppChn.s32ChnId = RGN_CHN.s32ChnId;
	ctx->rgn[3].stMppChn.s32DevId = RGN_CHN.s32DevId;
	ctx->rgn[3].stRegion.s32X = 512;            // must be 16 aligned
	ctx->rgn[3].stRegion.s32Y = 512;            // must be 16 aligned
	ctx->rgn[3].stRegion.u32Width = u32Width;   // must be 8 aligned
	ctx->rgn[3].stRegion.u32Height = u32Height; // must be 8 aligned
	ctx->rgn[3].u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn[3].u32BgAlpha = 128;
	ctx->rgn[3].u32FgAlpha = 128;
	ctx->rgn[3].u32Layer = 4;
	ctx->rgn[3].srcFileBmpName = pArgs->pInPathBmp2;
	if (pArgs->pInPathBmp2) {
		s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[3]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[3].rgnHandle);
			TRACE_END();
			return s32Ret;
		}
	} else {
		RK_LOGE("input_bmp2 file is NULL, overlay rgn skips");
	}

	TRACE_END();
	return s32Ret;
}

static RK_S32 rgn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	TRACE_BEGIN();
	if (pArgs->s32RgnAttachModule != RGN_ATTACH_VENC &&
	    pArgs->s32RgnAttachModule != RGN_ATTACH_VPSS)
		return s32Ret;
	for (RK_S32 i = 0; i < RGN_NUM_MAX; i++) {
		if (!ctx->rgn[i].srcFileBmpName && ctx->rgn[i].stRgnAttr.enType == OVERLAY_RGN)
			continue;
		s32Ret = SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn[i]);
		if (s32Ret != RK_SUCCESS)
			RK_LOGE("SAMPLE_COMM_RGN_DestroyChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[i].rgnHandle);
	}
	TRACE_END();
	return s32Ret;
}

static RK_S32 vi_chn_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	TRACE_BEGIN();
	/* Init VI[0] */
	ctx->vi.u32Width = pArgs->u32VideoWidth;
	ctx->vi.u32Height = pArgs->u32VideoHeight;
	ctx->vi.s32DevId = 0;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = 0;
	ctx->vi.stChnAttr.stIspOpt.stMaxSize.u32Width = pArgs->u32VideoWidth;
	ctx->vi.stChnAttr.stIspOpt.stMaxSize.u32Height = pArgs->u32VideoHeight;
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = pArgs->u32ViBuffCnt;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	// pCtx->vi.stChnAttr.u32Depth = 4;
	ctx->vi.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi.stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = 25;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = 25;
	s32Ret = SAMPLE_COMM_VI_CreateChn(&(ctx->vi));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%d", s32Ret);
	TRACE_END();
	return s32Ret;
}

static RK_S32 vi_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	TRACE_BEGIN();
	s32Ret = SAMPLE_COMM_VI_DestroyChn(&(ctx->vi));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VI_DestroyChn failure:%d", s32Ret);
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
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_PASSTHROUGH;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Width = pArgs->u32VideoWidth;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Height = pArgs->u32VideoHeight;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Depth = 0;

	// SET VPSS[0,1]
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enChnMode = VPSS_CHN_MODE_AUTO;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Width = pArgs->u32SubVideoWidth;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Height = pArgs->u32SubVideoHeight;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Depth = 0;

	s32Ret = SAMPLE_COMM_VPSS_CreateChn(&(ctx->vpss[vpssGrpId]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VPSS_CreateChn group 0 failed %#X\n", s32Ret);

	// Attach aiisp to vpss group 0.
	if (pArgs->bEnableAIIsp) {
		AIISP_ATTR_S stAIISPAttr;
		memset(&stAIISPAttr, 0, sizeof(AIISP_ATTR_S));
		stAIISPAttr.bEnable = RK_TRUE;
		stAIISPAttr.stAiIspCallback.pfUpdateCallback = aiisp_callback;
		stAIISPAttr.stAiIspCallback.pPrivateData = RK_NULL;
		stAIISPAttr.pModelFilePath = pArgs->pAiispModelPath;
		stAIISPAttr.u32FrameBufCnt = pArgs->u32AiispBuffCnt;

		s32Ret = RK_MPI_VPSS_SetGrpAIISPAttr(0, &stAIISPAttr);
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
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Width = pArgs->u32ThirdVideoWidth;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Height = pArgs->u32ThirdVideoHeight;
	ctx->vpss[vpssGrpId].stVpssChnAttr[0].u32Depth = 0;

	// SET VPSS[1,1]
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enChnMode = VPSS_CHN_MODE_AUTO;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Width = pArgs->u32IvsWidth;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Height = pArgs->u32IvsHeight;
	ctx->vpss[vpssGrpId].stVpssChnAttr[1].u32Depth = 1;

	s32Ret = SAMPLE_COMM_VPSS_CreateChn(&(ctx->vpss[vpssGrpId]));
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_VPSS_CreateChn group 1 failed %#X\n", s32Ret);
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
	ctx->venc[0].u32Width = pArgs->u32VideoWidth;
	ctx->venc[0].u32Height = pArgs->u32VideoHeight;
	ctx->venc[0].u32Fps = pArgs->u32VencFps;
	ctx->venc[0].u32Gop = pArgs->u32Gop;
	ctx->venc[0].u32BitRate = pArgs->s32BitRate;
	ctx->venc[0].enCodecType = pArgs->enCodecType;
	ctx->venc[0].enRcMode = pArgs->enRcMode;
	ctx->venc[0].getStreamCbFunc = venc_get_stream;
	ctx->venc[0].s32loopCount = pArgs->s32loopCnt;
	ctx->venc[0].dstFilePath = pArgs->pOutPathVenc;
	ctx->venc[0].u32BuffSize = pArgs->u32VideoWidth * pArgs->u32VideoHeight / 2;
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
	ctx->venc[1].u32Width = pArgs->u32SubVideoWidth;
	ctx->venc[1].u32Height = pArgs->u32SubVideoHeight;
	ctx->venc[1].u32Fps = pArgs->u32VencFps;
	ctx->venc[1].u32Gop = pArgs->u32Gop;
	ctx->venc[1].u32BitRate = pArgs->s32BitRate;
	ctx->venc[1].enCodecType = pArgs->enCodecType;
	ctx->venc[1].enRcMode = pArgs->enRcMode;
	ctx->venc[1].getStreamCbFunc = venc_get_stream;
	ctx->venc[1].s32loopCount = pArgs->s32loopCnt;
	ctx->venc[1].dstFilePath = pArgs->pOutPathVenc;
	ctx->venc[1].u32BuffSize = pArgs->u32SubVideoWidth * pArgs->u32SubVideoHeight / 2;
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
	ctx->venc[2].u32Width = pArgs->u32ThirdVideoWidth;
	ctx->venc[2].u32Height = pArgs->u32ThirdVideoHeight;
	ctx->venc[2].u32Fps = pArgs->u32VencFps;
	ctx->venc[2].u32Gop = pArgs->u32Gop;
	ctx->venc[2].u32BitRate = pArgs->s32BitRate;
	ctx->venc[2].enCodecType = pArgs->enCodecType;
	ctx->venc[2].enRcMode = pArgs->enRcMode;
	ctx->venc[2].getStreamCbFunc = venc_get_stream;
	ctx->venc[2].s32loopCount = pArgs->s32loopCnt;
	ctx->venc[2].dstFilePath = pArgs->pOutPathVenc;
	ctx->venc[2].u32BuffSize = pArgs->u32ThirdVideoWidth * pArgs->u32ThirdVideoHeight / 2;
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
	TRACE_END();

	return s32Ret;
}

static RK_S32 venc_chn_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;

	TRACE_BEGIN();
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

static RK_S32 ivs_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;

	TRACE_BEGIN();
	/* Init ivs */
	ctx->ivs.s32ChnId = 0;
	ctx->ivs.stIvsAttr.enMode = IVS_MODE_MD_OD;
	ctx->ivs.stIvsAttr.u32PicWidth = pArgs->u32IvsWidth;
	ctx->ivs.stIvsAttr.u32PicHeight = pArgs->u32IvsHeight;
	ctx->ivs.stIvsAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->ivs.stIvsAttr.s32Gop = 30;
	ctx->ivs.stIvsAttr.bSmearEnable = RK_FALSE;
	ctx->ivs.stIvsAttr.bWeightpEnable = RK_FALSE;
	ctx->ivs.stIvsAttr.bMDEnable = RK_TRUE;
	ctx->ivs.stIvsAttr.s32MDInterval = 5;
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
	TRACE_END();

	return s32Ret;
}

static RK_S32 ivs_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;

	TRACE_BEGIN();
	s32Ret = SAMPLE_COMM_IVS_Destroy(ctx->ivs.s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_IVS_Destroy failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
	}
	TRACE_END();

	return s32Ret;
}

static RK_S32 iva_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;

	TRACE_BEGIN();
	/* Init iva */
	ctx->iva.pModelDataPath = pArgs->pIvaModelPath;
	ctx->iva.u32ImageWidth = pArgs->u32IvsWidth;
	ctx->iva.u32ImageHeight = pArgs->u32IvsHeight;
	ctx->iva.u32DetectStartX = 0;
	ctx->iva.u32DetectStartY = 0;
	ctx->iva.u32DetectWidth = pArgs->u32IvsWidth;
	ctx->iva.u32DetectHight = pArgs->u32IvsHeight;
	ctx->iva.eImageTransform = ROCKIVA_IMAGE_TRANSFORM_NONE;
	ctx->iva.eImageFormat = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
	ctx->iva.eModeType = ROCKIVA_DET_MODEL_PFP;
	ctx->iva.u32IvaDetectFrameRate = pArgs->u32IvaDetectFrameRate;
	ctx->iva.resultCallback = rkIvaEvent_callback;
	ctx->iva.releaseCallback = rkIvaFrame_releaseCallBack;
	s32Ret = SAMPLE_COMM_IVA_Create(&ctx->iva);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_IVA_Create failure:%#X", s32Ret);
	TRACE_END();

	return s32Ret;
}

static RK_S32 iva_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	TRACE_BEGIN();
	RK_S32 s32Ret = RK_SUCCESS;
	s32Ret = SAMPLE_COMM_IVA_Destroy(&ctx->iva);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("SAMPLE_COMM_IVA_Destroy failure:%#X", s32Ret);
	TRACE_END();
	return s32Ret;
}

static RK_S32 bind_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	MPP_CHN_S stSrcChn, stDestChn;

	TRACE_BEGIN();
	// Bind VI and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vi to vpss0 failed");

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

	/* Bind VPSS[0, 1] and VPSS[1] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = 1;
	stDestChn.s32ChnId = 0;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vpss[0,1] to vpss1 fail");

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

	/* Bind VPSS[1, 1] and IVS */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 1;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_IVS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS)
		RK_LOGE("bind vpss[1,1] to ivs fail");
	TRACE_END();

	return s32Ret;
}

static RK_S32 bind_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	RK_S32 s32Ret = RK_SUCCESS;
	MPP_CHN_S stSrcChn, stDestChn;

	TRACE_BEGIN();
	/* UnBind VPSS[1, 1] and IVS */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 1;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_IVS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	/* UnBind VPSS[1, 0] and VENC[2] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 1;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 2;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	/* UnBind VPSS[0, 1] and VPSS[1] */
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 1;
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
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	TRACE_END();

	return s32Ret;
}

static RK_S32 sub_threads_init(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	TRACE_BEGIN();
	// ivs detect thread launch
	if (pArgs->bEnableIvs)
		pthread_create(&g_ivs_thread_id, 0, ivs_detect_thread, (void *)&ctx->ivs);
	// vpss iva thread launch
	if (pArgs->bEnableIva)
		pthread_create(&g_iva_thread_id, 0, vpss_iva_thread, (void *)ctx);
	TRACE_END();
	return RK_SUCCESS;
}

static RK_S32 sub_threads_deinit(SAMPLE_MPI_CTX_S *ctx, RkCmdArgs *pArgs) {
	TRACE_BEGIN();
	for (int i = 0; i < VENC_CHN_MAX; i++) {
		gPThreadStatus->bIfVencThreadQuit[i] = true;
		pthread_join(ctx->venc[i].getStreamThread, RK_NULL);
		ctx->venc[i].getStreamThread = 0;
	}
	if (pArgs->bEnableIva) {
		gPThreadStatus->bIfVpssIvaTHreadQuit = RK_TRUE;
		pthread_join(g_iva_thread_id, NULL);
		g_iva_thread_id = 0;
	}
	if (pArgs->bEnableIvs) {
		gPThreadStatus->bIvsDetectThreadQuit = RK_TRUE;
		pthread_join(g_ivs_thread_id, NULL);
		g_ivs_thread_id = 0;
	}
	TRACE_END();
	return RK_SUCCESS;
}

static RK_CHAR optstr[] = "?::a::w:h:o:l:b:f:r:g:v:e:i:s:I:";

static const struct option long_options[] = {
    {"aiq", optional_argument, RK_NULL, 'a'},
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
    {"iva_detect_speed", required_argument, RK_NULL, 'd'},
    {"iva_model_path", required_argument, RK_NULL, 'i' + 'm'},
    {"enable_aiisp", required_argument, RK_NULL, 'e' + 'a'},
    {"aiisp_model_path", required_argument, RK_NULL, 'e' + 'j'},
    {"aiisp_buff_cnt", required_argument, RK_NULL, 'e' + 'k'},
    {"enable_iva", required_argument, RK_NULL, 'e' + 'i'},
    {"enable_ivs", required_argument, RK_NULL, 'e' + 's'},
    {"inputBmpPath1", required_argument, RK_NULL, 'i'},
    {"inputBmpPath2", required_argument, RK_NULL, 'I'},
    {"help", optional_argument, RK_NULL, '?'},
    {RK_NULL, 0, RK_NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("RV1126 example:\n");
	printf("\t%s -w 2688 -h 1520 -a /etc/iqfiles/ -e h264cbr -b 2048 "
	       "--enable_aiisp 1 --vi_buff_cnt 4\n",
	       name);
	printf("RV1106 example:\n");
	printf("\t%s -w 2560 -h 1440 -a /etc/iqfiles/ -e h264cbr -b 2048 "
	       "--enable_aiisp 1 --vi_buff_cnt 2\n",
	       name);
	printf("\t-a | --aiq : enable aiq with dirpath provided, eg:-a /etc/iqfiles/, \n"
	       "\t		set dirpath empty to using path by default, without "
	       "this option aiq \n"
	       "\t		should run in other application\n");
	printf("\t-w | --width : mainStream width, must is sensor width\n");
	printf("\t-h | --height : mainStream height, must is sensor height\n");
	printf("\t-e | --encode: encode type, Default:h264cbr, Value:h264cbr, "
	       "h264vbr, h264avbr "
	       "h265cbr, h265vbr, h265avbr, mjpegcbr, mjpegvbr\n");
	printf("\t-b | --bitrate: encode bitrate, Default 4096\n");
	printf("\t-o | --output_path : encode output file path, Default: RK_NULL\n");
	printf("\t-l | --loop_count : when encoder output frameCounts equal to "
	       "<loop_count>, "
	       "process will exit. Default: -1\n");
	printf("\t-v | --vi_buff_cnt : main stream vi buffer num, Default: 6\n");
	printf("\t--vi_chnid : vi channel id, default: 0\n");
	printf("\t-r | --rgn_attach_module : where to attach rgn, 0: vpss, 1: venc, 2: "
	       "close. default: 1\n");
	printf("\t-g | --gop : venc GOP(group of pictures). default: 75\n");
	printf("\t-i | --inputpathbmp1 : input bmp file path. default: RK_NULL\n");
	printf("\t-I | --inputpathbmp2 : input bmp file path. default: RK_NULL\n");
	printf("\t-f | --fps : set fps, default: 25\n");
	printf("\t--enable_aiisp : enable ai isp, 0: close, 1: enable. default: 1\n");
	printf("\t--enable_iva : enable iva, 0: close, 1: enable. default: 1\n");
	printf("\t--enable_ivs : enable ivs, 0: close, 1: enable. default: 0\n");
	printf("\t--iva_detect_speed : iva detect framerate. default: 10\n");
	printf("\t--iva_model_path : iva model data path, default: /oem/usr/lib\n");
	printf("\t--aiisp_model_path : aiisp model data path, default: /oem/usr/lib\n");
	printf("\t--aiisp_buff_cnt : aiisp buffer count, default: 2\n");
}

/******************************************************************************
 * function    : parse_cmd_args()
 * Description : Parse command line arguments.
 ******************************************************************************/
static RK_S32 parse_cmd_args(int argc, char **argv, RkCmdArgs *pArgs) {
	pArgs->u32VideoWidth = 2688;
	pArgs->u32VideoHeight = 1520;
	pArgs->u32SubVideoWidth = 1280;
	pArgs->u32SubVideoHeight = 720;
	pArgs->u32ThirdVideoWidth = 640;
	pArgs->u32ThirdVideoHeight = 480;
	pArgs->u32ViBuffCnt = 5;
#if defined(RV1106)
	pArgs->u32ViBuffCnt = 2;
#endif
	pArgs->u32IvsWidth = 704;
	pArgs->u32IvsHeight = 576;
	pArgs->u32Gop = 75;
	pArgs->u32IvaDetectFrameRate = 10;
	pArgs->pInPathBmp1 = NULL;
	pArgs->pInPathBmp2 = NULL;
	pArgs->pOutPathVenc = NULL;
	pArgs->pIvaModelPath = "/oem/usr/lib/";
	pArgs->pIqFileDir = RK_NULL;
	pArgs->bMultictx = RK_FALSE;
	pArgs->enCodecType = RK_CODEC_TYPE_H264;
	pArgs->enRcMode = VENC_RC_MODE_H264CBR;
	pArgs->pCodecName = "H264";
	pArgs->s32CamId = 0;
	pArgs->s32loopCnt = -1;
	pArgs->bEnableAIIsp = RK_TRUE;
	pArgs->bEnableIva = RK_TRUE;
	pArgs->bEnableIvs = RK_TRUE;
	pArgs->s32BitRate = 4 * 1024;
	pArgs->u32VencFps = 25;
	pArgs->eHdrMode = RK_AIQ_WORKING_MODE_NORMAL;
	pArgs->s32RgnAttachModule = RGN_ATTACH_VENC; // 0:vpss,1:venc
	pArgs->pAiispModelPath = "/oem/usr/lib/";
#if defined(RV1106)
	pArgs->u32AiispBuffCnt = 1;
#else
	pArgs->u32AiispBuffCnt = 2;
#endif

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
		case 'w':
			pArgs->u32VideoWidth = atoi(optarg);
			break;
		case 'h':
			pArgs->u32VideoHeight = atoi(optarg);
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
		case 'i' + 'm':
			pArgs->pIvaModelPath = optarg;
			break;
		case 'd':
			pArgs->u32IvaDetectFrameRate = atoi(optarg);
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
		case 'e' + 's':
			pArgs->bEnableIvs = atoi(optarg);
			break;
		case 'e' + 'i':
			pArgs->bEnableIva = atoi(optarg);
			break;
		case 'i':
			pArgs->pInPathBmp1 = optarg;
			break;
		case 'I':
			pArgs->pInPathBmp2 = optarg;
			break;
		case '?':
		default:
			return RK_SUCCESS;
		}
	}

	return RK_SUCCESS;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_SUCCESS;
	SAMPLE_MPI_CTX_S *ctx;
	RkCmdArgs parsedArgs;

	print_usage(argv[0]);
	if (argc < 2) {
		printf("bad arguments!\n");
		return RK_FAILURE;
	}

	// Allocate global ctx.
	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	if (!ctx) {
		printf("ctx is null, malloc failure\n");
		return RK_FAILURE;
	}
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	// Parse command line.
	memset(&parsedArgs, 0, sizeof(RkCmdArgs));
	parse_cmd_args(argc, argv, &parsedArgs);

	s32Ret = global_param_init();
	if (s32Ret != RK_SUCCESS) {
		printf("global_param_init failure\n");
		g_exit_result = RK_FAILURE;
		goto __PARAM_INIT_FAILED;
	}

	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);

	printf("#CameraIdx: %d\n", parsedArgs.s32CamId);
	printf("#CodecName:%s\n", parsedArgs.pCodecName);
	printf("#Output Path: %s\n", parsedArgs.pOutPathVenc);
	printf("#RGN_ATTACH: %d\n", parsedArgs.s32RgnAttachModule);
	printf("#IQ Path: %s\n", parsedArgs.pIqFileDir);
	if (parsedArgs.pIqFileDir) {
		printf("#Rkaiq XML DirPath: %s\n", parsedArgs.pIqFileDir);
		printf("#bMultictx: %d\n\n", parsedArgs.bMultictx);

		s32Ret = SAMPLE_COMM_ISP_Init(parsedArgs.s32CamId, parsedArgs.eHdrMode,
		                              parsedArgs.bMultictx, parsedArgs.pIqFileDir);
		s32Ret |= SAMPLE_COMM_ISP_Run(parsedArgs.s32CamId);
		if (s32Ret != RK_SUCCESS) {
			printf("ISP init failure");
			g_exit_result = RK_FAILURE;
			goto __ISP_INIT_FAILED;
		}
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		printf("RK_MPI_SYS_Init failure");
		g_exit_result = RK_FAILURE;
		goto __MPI_INIT_FAILED;
	}

	// Initialize rtsp server.
	rtsp_init(parsedArgs.enCodecType);
	// Initialize all pipeline nodes.
	if (parsedArgs.bEnableIva)
		iva_init(ctx, &parsedArgs);
	vi_chn_init(ctx, &parsedArgs);
	vpss_chn_init(ctx, &parsedArgs);
	venc_chn_init(ctx, &parsedArgs);
	if (parsedArgs.bEnableIvs)
		ivs_init(ctx, &parsedArgs);
	rgn_init(ctx, &parsedArgs);
	// Bind all pipeline nodes.
	bind_init(ctx, &parsedArgs);
	// Start sub-threads after all initialization done.
	sub_threads_init(ctx, &parsedArgs);

	// Keep running ...
	while (!gPThreadStatus->bIfMainThreadQuit) {
		sleep(1);
	}

	// Destroy pipeline.
	sub_threads_deinit(ctx, &parsedArgs);
	bind_deinit(ctx, &parsedArgs);
	rgn_deinit(ctx, &parsedArgs);
	if (parsedArgs.bEnableIvs)
		ivs_deinit(ctx, &parsedArgs);
	venc_chn_deinit(ctx, &parsedArgs);
	vpss_chn_deinit(ctx, &parsedArgs);
	vi_chn_deinit(ctx, &parsedArgs);
	if (parsedArgs.bEnableIva)
		iva_deinit(ctx, &parsedArgs);
	rtsp_deinit();

__MPI_INIT_FAILED:
	RK_MPI_SYS_Exit();
__ISP_INIT_FAILED:
	if (parsedArgs.pIqFileDir)
		SAMPLE_COMM_ISP_Stop(parsedArgs.s32CamId);

__PARAM_INIT_FAILED:
	global_param_deinit();

	free(ctx);
	ctx = RK_NULL;

	return g_exit_result;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
