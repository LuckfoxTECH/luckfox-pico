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

RK_S32 SAMPLE_COMM_AI_CreateChn(SAMPLE_AI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AI_SetPubAttr(ctx->s32DevId, &ctx->stAiAttr);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AI_SetPubAttr failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_AI_Enable(ctx->s32DevId);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AI_Enable failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_AI_EnableChn(ctx->s32DevId, ctx->s32ChnId);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AI_EnableChn failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_AI_EnableReSmp(ctx->s32DevId, ctx->s32ChnId,
	                               (AUDIO_SAMPLE_RATE_E)ctx->s32ChnSampleRate);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AI_EnableReSmp failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_AI_DestroyChn(SAMPLE_AI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AI_DisableReSmp(ctx->s32DevId, ctx->s32ChnId);
	s32Ret |= RK_MPI_AI_DisableChn(ctx->s32DevId, ctx->s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AI_DisableChn failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_AI_Disable(ctx->s32DevId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AI_Disable failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_AI_GetFrame(SAMPLE_AI_CTX_S *ctx, void **pdata) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32MilliSec = -1;

	s32Ret = RK_MPI_AI_GetFrame(ctx->s32DevId, ctx->s32ChnId, &ctx->stFrame, RK_NULL,
	                            s32MilliSec);
	if (s32Ret == RK_SUCCESS) {
		*pdata = RK_MPI_MB_Handle2VirAddr(ctx->stFrame.pMbBlk);
	} else {
		RK_LOGE("RK_MPI_AI_GetFrame failed with %#x!\n", s32Ret);
	}

	return 0;
}

RK_S32 SAMPLE_COMM_AI_ReleaseFrame(SAMPLE_AI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AI_ReleaseFrame(ctx->s32DevId, ctx->s32ChnId, &ctx->stFrame, RK_NULL);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AI_ReleaseFrame failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
