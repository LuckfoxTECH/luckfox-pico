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

RK_S32 SAMPLE_COMM_AO_CreateChn(SAMPLE_AO_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AO_SetPubAttr(ctx->s32DevId, &ctx->stAoAttr);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AO_SetPubAttr failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_AO_Enable(ctx->s32DevId);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AO_Enable failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_AO_EnableChn(ctx->s32DevId, ctx->s32ChnId);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AO_EnableChn failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	// set sample rate of input data
	s32Ret = RK_MPI_AO_EnableReSmp(ctx->s32DevId, ctx->s32ChnId,
	                               (AUDIO_SAMPLE_RATE_E)ctx->s32ChnSampleRate);
	if (s32Ret != 0) {
		RK_LOGE("RK_MPI_AO_EnableReSmp failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_AO_DestroyChn(SAMPLE_AO_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AO_DisableReSmp(ctx->s32DevId, ctx->s32ChnId);
	s32Ret |= RK_MPI_AO_DisableChn(ctx->s32DevId, ctx->s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AO_DisableChn failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_AO_Disable(ctx->s32DevId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AO_Disable failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
