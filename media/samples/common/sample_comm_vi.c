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

RK_S32 SAMPLE_COMM_VI_CreateChn(SAMPLE_VI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	VI_CHN_BUF_WRAP_S stViWrap;

	// 0.get dev config status
	s32Ret = RK_MPI_VI_GetDevAttr(ctx->s32DevId, &ctx->stDevAttr);
	if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
		// 0-1.config dev
		s32Ret = RK_MPI_VI_SetDevAttr(ctx->s32DevId, &ctx->stDevAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
			goto __FAILED1;
		}
	} else {
		RK_LOGE("RK_MPI_VI_SetDevAttr already!");
	}

	// 1.get dev enable status
	s32Ret = RK_MPI_VI_GetDevIsEnable(ctx->s32DevId);
	if (s32Ret != RK_SUCCESS) {
		// 1-2.enable dev
		s32Ret = RK_MPI_VI_EnableDev(ctx->s32DevId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
			goto __FAILED1;
		}
		// 1-3.bind dev/pipe
		ctx->stBindPipe.u32Num = 1;
		ctx->stBindPipe.PipeId[0] = ctx->u32PipeId;
		s32Ret = RK_MPI_VI_SetDevBindPipe(ctx->s32DevId, &ctx->stBindPipe);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetDevBindPipe failed with %#x!\n", s32Ret);
			goto __FAILED2;
		}
	} else {
		RK_LOGE("RK_MPI_VI_EnableDev already!");
	}

	// 2.config channel
	ctx->stChnAttr.stSize.u32Width = ctx->u32Width;
	ctx->stChnAttr.stSize.u32Height = ctx->u32Height;
	s32Ret = RK_MPI_VI_SetChnAttr(ctx->u32PipeId, ctx->s32ChnId, &ctx->stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_SetChnAttr failed with %#x!\n", s32Ret);
		goto __FAILED2;
	}

	if (ctx->bWrapIfEnable) {
		memset(&stViWrap, 0, sizeof(VI_CHN_BUF_WRAP_S));
		if (ctx->u32BufferLine < 128 || ctx->u32BufferLine > ctx->u32Height) {
			RK_LOGE("wrap mode buffer line must between [128, %d]", ctx->u32Height);
			return RK_FAILURE;
		}
		stViWrap.bEnable = RK_TRUE;
		stViWrap.u32BufLine = ctx->u32BufferLine;
		stViWrap.u32WrapBufferSize = stViWrap.u32BufLine * ctx->u32Width * 3 / 2;
		s32Ret = RK_MPI_VI_SetChnWrapBufAttr(ctx->u32PipeId, ctx->s32ChnId, &stViWrap);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetChnWrapBufAttr failure:%X", s32Ret);
			return RK_FAILURE;
		}
	}

	// 3.enable channel
	if (!ctx->bIfIspGroupInit) {
		s32Ret = RK_MPI_VI_EnableChn(ctx->u32PipeId, ctx->s32ChnId);
	} else {
		s32Ret = RK_MPI_VI_EnableChnExt(ctx->u32PipeId, ctx->s32ChnId);
	}
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_EnableChn(Ext) failed with %#x! pipe:%d chn:%d", s32Ret,
		        ctx->u32PipeId, ctx->s32ChnId);
		goto __FAILED2;
	}

	if (ctx->bIfOpenEptz) {
		s32Ret = RK_MPI_VI_SetEptz(ctx->u32PipeId, ctx->s32ChnId, ctx->stCropInfo);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetEptz failure:%#X", s32Ret);
			goto __FAILED2;
		}
	}

	// 4.save debug file
	if (ctx->stDebugFile.bCfg) {
		s32Ret = RK_MPI_VI_ChnSaveFile(ctx->u32PipeId, ctx->s32ChnId, &ctx->stDebugFile);
		RK_LOGD("RK_MPI_VI_ChnSaveFile with %#x!\n", s32Ret);
	}

	return RK_SUCCESS;

// 6.disable dev(will diabled all chn)
__FAILED2:
	s32Ret = RK_MPI_VI_DisableDev(ctx->s32DevId);
	RK_LOGE("RK_MPI_VI_DisableDev with %#x!\n", s32Ret);
__FAILED1:
	return s32Ret;
}

RK_S32 SAMPLE_COMM_VI_GetChnFrame(SAMPLE_VI_CTX_S *ctx, void **pdata) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 waitTime = -1;

	s32Ret =
	    RK_MPI_VI_GetChnFrame(ctx->u32PipeId, ctx->s32ChnId, &ctx->stViFrame, waitTime);
	if (s32Ret == RK_SUCCESS) {
		*pdata = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
		s32Ret =
		    RK_MPI_VI_QueryChnStatus(ctx->u32PipeId, ctx->s32ChnId, &ctx->stChnStatus);
		RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, DevId%d,PipeId:%d,ChnId:%d"
		        "w:%d,h:%d,enable:%d,inputlost:%d,outputlost:%d,framerate:%d,vbfail:%d",
		        ctx->s32DevId, ctx->u32PipeId, ctx->s32ChnId, s32Ret,
		        ctx->stChnStatus.stSize.u32Width, ctx->stChnStatus.stSize.u32Height,
		        ctx->stChnStatus.bEnable, ctx->stChnStatus.u32InputLostFrame,
		        ctx->stChnStatus.u32OutputLostFrame, ctx->stChnStatus.u32FrameRate,
		        ctx->stChnStatus.u32VbFail);
	} else {
		RK_LOGE("RK_MPI_VI_GetChnFrame DevId %d timeout %x", ctx->s32DevId, s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VI_ReleaseChnFrame(SAMPLE_VI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->u32PipeId, ctx->s32ChnId, &ctx->stViFrame);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
	}
	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VI_DestroyChn(SAMPLE_VI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	if (!ctx->bIfIspGroupInit) {
		s32Ret = RK_MPI_VI_DisableChn(ctx->u32PipeId, ctx->s32ChnId);
	} else {
		s32Ret = RK_MPI_VI_DisableChnExt(ctx->u32PipeId, ctx->s32ChnId);
	}
	s32Ret |= RK_MPI_VI_DisableDev(ctx->s32DevId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_Close failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
