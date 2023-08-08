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

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>

#include "sample_comm.h"

RK_S32 SAMPLE_COMM_VENC_CreateChn(SAMPLE_VENC_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_RC_PARAM_S pstRcParam;
	VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;

	switch (ctx->enCodecType) {
	case RK_CODEC_TYPE_H265:
		ctx->stChnAttr.stVencAttr.enType = RK_VIDEO_ID_HEVC;
		if (ctx->enRcMode == VENC_RC_MODE_H265CBR) {
			ctx->stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			ctx->stChnAttr.stRcAttr.stH265Cbr.u32Gop = ctx->u32Gop;
			ctx->stChnAttr.stRcAttr.stH265Cbr.u32BitRate = ctx->u32BitRate;
			// frame rate: in u32Fps/1, out u32Fps/1.
			ctx->stChnAttr.stRcAttr.stH265Cbr.fr32DstFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH265Cbr.fr32DstFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stH265Cbr.u32SrcFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH265Cbr.u32SrcFrameRateNum = ctx->u32Fps;
		} else if (ctx->enRcMode == VENC_RC_MODE_H265VBR) {
			ctx->stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
			ctx->stChnAttr.stRcAttr.stH265Vbr.u32Gop = ctx->u32Gop;
			ctx->stChnAttr.stRcAttr.stH265Vbr.u32BitRate = ctx->u32BitRate;
			// frame rate: in u32Fps/1, out u32Fps/1.
			ctx->stChnAttr.stRcAttr.stH265Vbr.fr32DstFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH265Vbr.fr32DstFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stH265Vbr.u32SrcFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH265Vbr.u32SrcFrameRateNum = ctx->u32Fps;
		}
		break;
	case RK_CODEC_TYPE_MJPEG:
		ctx->stChnAttr.stVencAttr.enType = RK_VIDEO_ID_MJPEG;
		if (ctx->enRcMode == VENC_RC_MODE_MJPEGCBR) {
			ctx->stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
			// frame rate: in u32Fps/1, out u32Fps/1.
			ctx->stChnAttr.stRcAttr.stMjpegCbr.fr32DstFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stMjpegCbr.fr32DstFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stMjpegCbr.u32SrcFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stMjpegCbr.u32SrcFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stMjpegCbr.u32BitRate =
			    ctx->u32Width * ctx->u32Height * 8;
		} else if (ctx->enRcMode == VENC_RC_MODE_MJPEGVBR) {
			ctx->stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGVBR;
			// frame rate: in u32Fps/1, out u32Fps/1.
			ctx->stChnAttr.stRcAttr.stMjpegVbr.fr32DstFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stMjpegVbr.fr32DstFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stMjpegVbr.u32SrcFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stMjpegVbr.u32SrcFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stMjpegVbr.u32BitRate =
			    ctx->u32Width * ctx->u32Height * 8;
		}
		break;
	case RK_CODEC_TYPE_JPEG:
		RK_LOGE("RK_CODEC_TYPE_JPEG setting");
		ctx->stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
		ctx->stChnAttr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
		ctx->stChnAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
		ctx->stChnAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
		ctx->stChnAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
		break;
	case RK_CODEC_TYPE_H264:
	default:
		ctx->stChnAttr.stVencAttr.enType = RK_VIDEO_ID_AVC;
		if (ctx->enRcMode == VENC_RC_MODE_H264CBR) {
			ctx->stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
			ctx->stChnAttr.stRcAttr.stH264Cbr.u32Gop = ctx->u32Gop;
			ctx->stChnAttr.stRcAttr.stH264Cbr.u32BitRate = ctx->u32BitRate;
			// frame rate: in u32Fps/1, out u32Fps/1.
			ctx->stChnAttr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = ctx->u32Fps;
		} else if (ctx->enRcMode == VENC_RC_MODE_H264VBR) {
			ctx->stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
			ctx->stChnAttr.stRcAttr.stH264Vbr.u32Gop = ctx->u32Gop;
			ctx->stChnAttr.stRcAttr.stH264Vbr.u32BitRate = ctx->u32BitRate;
			// frame rate: in u32Fps/1, out u32Fps/1.
			ctx->stChnAttr.stRcAttr.stH264Vbr.fr32DstFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH264Vbr.fr32DstFrameRateNum = ctx->u32Fps;
			ctx->stChnAttr.stRcAttr.stH264Vbr.u32SrcFrameRateDen = 1;
			ctx->stChnAttr.stRcAttr.stH264Vbr.u32SrcFrameRateNum = ctx->u32Fps;
		}
		break;
	}

	ctx->stChnAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->stChnAttr.stVencAttr.u32MaxPicWidth = ctx->u32Width;
	ctx->stChnAttr.stVencAttr.u32MaxPicHeight = ctx->u32Height;
	ctx->stChnAttr.stVencAttr.u32PicWidth = ctx->u32Width;
	ctx->stChnAttr.stVencAttr.u32PicHeight = ctx->u32Height;
	ctx->stChnAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(ctx->u32Width);
	ctx->stChnAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(ctx->u32Height);
	if (ctx->u32StreamBufCnt) {
		ctx->stChnAttr.stVencAttr.u32StreamBufCnt = ctx->u32StreamBufCnt;
	} else {
		ctx->stChnAttr.stVencAttr.u32StreamBufCnt = 3;
	}
	RK_LOGE("V_C:%d bufferCnt:%d", ctx->s32ChnId,
	        ctx->stChnAttr.stVencAttr.u32StreamBufCnt);
	if (ctx->u32BuffSize) {
		ctx->stChnAttr.stVencAttr.u32BufSize = ctx->u32BuffSize;
	} else {
		ctx->stChnAttr.stVencAttr.u32BufSize = ctx->u32Width * ctx->u32Height * 3 / 2;
	}

	s32Ret = RK_MPI_VENC_CreateChn(ctx->s32ChnId, &ctx->stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_CreateChn failed %x", s32Ret);
		return s32Ret;
	}
	if (ctx->enable_buf_share) {
		VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
		memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
		stVencChnRefBufShare.bEnable = 1;
		s32Ret = RK_MPI_VENC_SetChnRefBufShareAttr(ctx->s32ChnId, &stVencChnRefBufShare);
		if (s32Ret != RK_SUCCESS) {
			printf("RK_MPI_VENC_SetChnRefBufShareAttr failed,ret=%#x\n", s32Ret);
		}
	}

	if (ctx->bWrapIfEnable) {
		memset(&stVencChnBufWrap, 0, sizeof(stVencChnBufWrap));
		stVencChnBufWrap.bEnable = RK_TRUE;
		stVencChnBufWrap.u32BufLine = ctx->u32BufferLine;

		memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
		stVencChnRefBufShare.bEnable = RK_TRUE;

		s32Ret = RK_MPI_VENC_SetChnBufWrapAttr(ctx->s32ChnId, &stVencChnBufWrap);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_SetChnBufWrapAttr failure:%X", s32Ret);
			return RK_FAILURE;
		}
		RK_MPI_VENC_SetChnRefBufShareAttr(ctx->s32ChnId, &stVencChnRefBufShare);
	}

	memset(&pstRcParam, 0, sizeof(VENC_RC_PARAM_S));
	if (ctx->enCodecType == RK_CODEC_TYPE_H264) {
		pstRcParam.stParamH264.u32MinQp = 10;
		pstRcParam.stParamH264.u32MaxQp = 51;
		pstRcParam.stParamH264.u32MinIQp = 10;
		pstRcParam.stParamH264.u32MaxIQp = 51;
		pstRcParam.stParamH264.u32FrmMinQp = 28;
		pstRcParam.stParamH264.u32FrmMinIQp = 28;
	} else if (ctx->enCodecType == RK_CODEC_TYPE_H265) {
		pstRcParam.stParamH265.u32MinQp = 10;
		pstRcParam.stParamH265.u32MaxQp = 51;
		pstRcParam.stParamH265.u32MinIQp = 10;
		pstRcParam.stParamH265.u32MaxIQp = 51;
		pstRcParam.stParamH265.u32FrmMinQp = 28;
		pstRcParam.stParamH265.u32FrmMinIQp = 28;
	}
	s32Ret = RK_MPI_VENC_SetRcParam(ctx->s32ChnId, &pstRcParam);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_SetRcParam failed %X", s32Ret);
		return s32Ret;
	}

	/* ifenable Svc */
	s32Ret = RK_MPI_VENC_EnableSvc(ctx->s32ChnId, ctx->bSvcIfEnable);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_EnableSvc failed %X", s32Ret);
		return s32Ret;
	}
	/* ifenable Motion Deblur */
	s32Ret = RK_MPI_VENC_EnableMotionDeblur(ctx->s32ChnId, ctx->bMotionDeblurIfEnable);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_EnableSvc failed %X", s32Ret);
		return s32Ret;
	}

	stRecvParam.s32RecvPicNum = -1;
	s32Ret = RK_MPI_VENC_StartRecvFrame(ctx->s32ChnId, &stRecvParam);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("create %d ch venc failed", ctx->s32ChnId);
		return s32Ret;
	}

	if (ctx->stChnAttr.stVencAttr.enType == RK_VIDEO_ID_JPEG) {
		VENC_JPEG_PARAM_S stJpegParam;
		memset(&stJpegParam, 0, sizeof(VENC_JPEG_PARAM_S));
		stJpegParam.u32Qfactor = ctx->u32Qfactor;
		s32Ret = RK_MPI_VENC_SetJpegParam(ctx->s32ChnId, &stJpegParam);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_SetJpegParam failure:%X", s32Ret);
			return s32Ret;
		}
	} else if (ctx->stChnAttr.stVencAttr.enType == RK_VIDEO_ID_MJPEG) {
		VENC_MJPEG_PARAM_S stMjpegParam;
		memset(&stMjpegParam, 0, sizeof(stMjpegParam));
		memset(&stMjpegParam.u8YQt, 1, sizeof(stMjpegParam.u8YQt));
		memset(&stMjpegParam.u8CbQt, 1, sizeof(stMjpegParam.u8CbQt));
		memset(&stMjpegParam.u8CrQt, 1, sizeof(stMjpegParam.u8CrQt));
		stMjpegParam.u32MCUPerECS = 100;
		RK_MPI_VENC_SetMjpegParam(ctx->s32ChnId, &stMjpegParam);
	}

	if (ctx->bComboIfEnable) {
		VENC_COMBO_ATTR_S stComboAttr;
		memset(&stComboAttr, 0, sizeof(VENC_COMBO_ATTR_S));
		stComboAttr.bEnable = RK_TRUE;
		stComboAttr.s32ChnId = ctx->u32ComboChnId;
		RK_MPI_VENC_SetComboAttr(ctx->s32ChnId, &stComboAttr);
	}

	if (ctx->getStreamCbFunc) {
		ctx->stFrame.pstPack = (VENC_PACK_S *)(malloc(sizeof(VENC_PACK_S)));
		pthread_create(&ctx->getStreamThread, 0, ctx->getStreamCbFunc, (void *)(ctx));
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VENC_SendStream(SAMPLE_VENC_CTX_S *ctx, void *pdata, RK_S32 width,
                                   RK_S32 height, RK_S32 size,
                                   COMPRESS_MODE_E enCompressMode) {
	RK_S32 s32Ret = RK_FAILURE;
	MB_BLK blk = RK_NULL;
	RK_U8 *pVirAddr = RK_NULL;
	RK_S32 s32ReachEOS = 0;
	VIDEO_FRAME_INFO_S stFrame;

__RETRY0:
	blk = RK_MPI_MB_GetMB(ctx->pool, size, RK_TRUE);
	if (RK_NULL == blk) {
		RK_LOGE("RK_MPI_MB_GetMB fail %x", blk);
		usleep(2000llu);
		goto __RETRY0;
	}

	pVirAddr = (RK_U8 *)(RK_MPI_MB_Handle2VirAddr(blk));

	pVirAddr = pdata;

	RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);

	stFrame.stVFrame.pMbBlk = blk;
	stFrame.stVFrame.u32Width = width;
	stFrame.stVFrame.u32Height = height;
	stFrame.stVFrame.u32VirWidth = width;
	stFrame.stVFrame.u32VirHeight = height;
	stFrame.stVFrame.enPixelFormat = RK_FMT_YUV420SP;
	stFrame.stVFrame.u32FrameFlag |= s32ReachEOS ? FRAME_FLAG_SNAP_END : 0;
	stFrame.stVFrame.enCompressMode = enCompressMode;

__RETRY1:
	s32Ret = RK_MPI_VENC_SendFrame(ctx->s32ChnId, &stFrame, -1);
	if (s32Ret == RK_SUCCESS) {
		RK_MPI_MB_ReleaseMB(blk);
	} else {
		RK_LOGE("RK_MPI_VENC_SendFrame fail %x", s32Ret);
		usleep(10000llu);
		goto __RETRY1;
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_VENC_GetStream(SAMPLE_VENC_CTX_S *ctx, void **pdata) {
	RK_S32 s32Ret = RK_FAILURE;

	while (1) {
		s32Ret = RK_MPI_VENC_GetStream(ctx->s32ChnId, &ctx->stFrame, 2000);
		if (s32Ret == RK_SUCCESS) {
			*pdata = RK_MPI_MB_Handle2VirAddr(ctx->stFrame.pstPack->pMbBlk);
			break;
		} else {
			RK_LOGE("RK_MPI_VENC_GetStream fail %x", s32Ret);
		}
	}
	return s32Ret;
}

RK_S32 SAMPLE_COMM_VENC_ReleaseStream(SAMPLE_VENC_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_VENC_ReleaseStream(ctx->s32ChnId, &ctx->stFrame);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_VENC_DestroyChn(SAMPLE_VENC_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_VENC_StopRecvFrame(ctx->s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}
	RK_LOGE("destroy enc chn:%d", ctx->s32ChnId);
	s32Ret = RK_MPI_VENC_DestroyChn(ctx->s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
	}

	if (ctx->stFrame.pstPack) {
		free(ctx->stFrame.pstPack);
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
