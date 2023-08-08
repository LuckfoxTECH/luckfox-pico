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

#include <error.h>
#include <stdio.h>

#include "sample_comm.h"

RK_S32 SAMPLE_COMM_TDE_Create(SAMPLE_TDE_CTX_S *ctx) {

	RK_S32 s32Ret = RK_FAILURE;
	MB_POOL_CONFIG_S stMbPoolCfg;
	PIC_BUF_ATTR_S stPicBufAttr;
	MB_PIC_CAL_S stMbPicCalResult;

	memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
	memset(&stPicBufAttr, 0, sizeof(PIC_BUF_ATTR_S));
	memset(&stMbPicCalResult, 0, sizeof(MB_PIC_CAL_S));

	stPicBufAttr.u32Width = ctx->u32TdeWidth;
	stPicBufAttr.u32Height = ctx->u32TdeHeight;
	stPicBufAttr.enPixelFormat = ctx->enSrcPixelFormat;
	stPicBufAttr.enCompMode = ctx->enSrcPixelFormat;
	s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
	RK_LOGD("u32MBSize is %d", stMbPicCalResult.u32MBSize);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_CAL_TDE_GetPicBufferSize Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
	stMbPoolCfg.u64MBSize = stMbPicCalResult.u32MBSize;
	if (ctx->u32BufferPoolCnt) {
		stMbPoolCfg.u32MBCnt = ctx->u32BufferPoolCnt;
	} else {
		stMbPoolCfg.u32MBCnt = 1;
	}
	stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
	stMbPoolCfg.bPreAlloc = RK_FALSE;
	RK_LOGE("buffer pool one size:%d", stMbPicCalResult.u32MBSize);
	ctx->u32buffersize = stMbPicCalResult.u32MBSize;
	ctx->stVideoFrames.stVFrame.u32Width = ctx->u32TdeWidth;
	ctx->stVideoFrames.stVFrame.u32Height = ctx->u32TdeHeight;
	ctx->stVideoFrames.stVFrame.u32VirWidth = ctx->u32TdeWidth;
	ctx->stVideoFrames.stVFrame.u32VirHeight = ctx->u32TdeHeight;
	ctx->stVideoFrames.stVFrame.enPixelFormat = ctx->enSrcPixelFormat;
	ctx->stVideoFrames.stVFrame.enCompressMode = ctx->enSrcPixelFormat;
	ctx->tde_frame_pool = RK_MPI_MB_CreatePool(&stMbPoolCfg);
	if (ctx->tde_frame_pool == MB_INVALID_POOLID) {
		RK_LOGE("RK_MPI_MB_CreatePool Failure, the pool is MB_INVALID_POOLID");
		return RK_FAILURE;
	}

	ctx->pstSrc.u32Width = ctx->u32SrcWidth;
	ctx->pstSrc.u32Height = ctx->u32SrcHeight;
	ctx->pstSrc.enColorFmt = ctx->enSrcPixelFormat;
	ctx->pstSrc.enComprocessMode = ctx->enSrcPixelFormat;
	ctx->pstSrcRect.s32Xpos = 0;
	ctx->pstSrcRect.s32Ypos = 0;
	ctx->pstSrcRect.u32Width = ctx->u32TdeWidth;
	ctx->pstSrcRect.u32Height = ctx->u32TdeHeight;

	ctx->pstDst.u32Width = ctx->u32TdeWidth;
	ctx->pstDst.u32Height = ctx->u32TdeHeight;
	ctx->pstDst.enColorFmt = RK_FMT_YUV420SP;
	ctx->pstDst.enComprocessMode = COMPRESS_MODE_NONE;
	ctx->pstDstRect.s32Xpos = 0;
	ctx->pstDstRect.s32Ypos = 0;
	ctx->pstDstRect.u32Width = ctx->u32TdeWidth;
	ctx->pstDstRect.u32Height = ctx->u32TdeHeight;

	s32Ret = RK_TDE_Open();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_TDE_Open Failure s32Ret:%#X", s32Ret);
		return s32Ret;
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_TDE_Handle(SAMPLE_TDE_CTX_S *ctx, VIDEO_FRAME_S *pStVFrame) {
	RK_S32 s32Ret = RK_FAILURE;

	ctx->hHandle = RK_TDE_BeginJob();
	if (RK_ERR_TDE_INVALID_HANDLE == ctx->hHandle) {
		RK_LOGE("RK_TDE_BeginJob Failure  chnid%d", ctx->u32ChnId);
		return RK_FAILURE;
	}

	ctx->pstSrc.pMbBlk = pStVFrame->pMbBlk;
	ctx->pstDst.pMbBlk = ctx->stVideoFrames.stVFrame.pMbBlk;
	ctx->stVideoFrames.stVFrame.u32TimeRef = pStVFrame->u32TimeRef;
	ctx->stVideoFrames.stVFrame.u64PTS = pStVFrame->u64PTS;

	s32Ret = RK_TDE_QuickCopy(ctx->hHandle, &ctx->pstSrc, &ctx->pstSrcRect, &ctx->pstDst,
	                          &ctx->pstDstRect);

	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_TDE_QuickCopy Failure %#X chnid: %d", s32Ret, ctx->u32ChnId);
		RK_TDE_CancelJob(ctx->hHandle);
		return s32Ret;
	}
	s32Ret = RK_TDE_EndJob(ctx->hHandle, RK_FALSE, RK_TRUE, -1);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_TDE_EndJob Failure %#X  chnid: %d", s32Ret, ctx->u32ChnId);
		RK_TDE_CancelJob(ctx->hHandle);
		return s32Ret;
	}
	s32Ret = RK_TDE_WaitForDone(ctx->hHandle);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_TDE_WaitForDone Failure s32Ret: %#X  chnid:%d", s32Ret,
		        ctx->u32ChnId);
		return s32Ret;
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_TDE_GetMB(SAMPLE_TDE_CTX_S *ctx) {

	ctx->stVideoFrames.stVFrame.pMbBlk =
	    RK_MPI_MB_GetMB(ctx->tde_frame_pool, ctx->u32buffersize, RK_TRUE);
	if (ctx->stVideoFrames.stVFrame.pMbBlk == MB_INVALID_HANDLE) {
		RK_LOGE("RK_MPI_MB_GetMB Failure pMbBlk is MB_INVALID_HANDLE tde:%d",
		        ctx->u32ChnId);
		return RK_FAILURE;
	}
	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_TDE_ReleaseMB(SAMPLE_TDE_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_MB_ReleaseMB(ctx->stVideoFrames.stVFrame.pMbBlk);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_MB_ReleaseMB Failure tde:%d s32Ret: %#X", ctx->u32ChnId, s32Ret);
		return s32Ret;
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_TDE_Destroy(SAMPLE_TDE_CTX_S *ctx) {

	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_MB_DestroyPool(ctx->tde_frame_pool);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_MB_DestroyPool Failure s32Ret: %#X pool:%d", s32Ret,
		        ctx->tde_frame_pool);
		return s32Ret;
	}
	RK_TDE_Close();
	return s32Ret;
}