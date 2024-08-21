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

RK_S32 SAMPLE_COMM_AENC_CreateChn(SAMPLE_AENC_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AENC_CreateChn(ctx->s32ChnId, &ctx->stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AENC_CreateChn failed %x", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_AENC_GetStream(SAMPLE_AENC_CTX_S *ctx, void **pdata) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AENC_GetStream(ctx->s32ChnId, &ctx->stFrame, -1);
	if (s32Ret == RK_SUCCESS) {
		*pdata = RK_MPI_MB_Handle2VirAddr(ctx->stFrame.pMbBlk);
	} else {
		RK_LOGE("RK_MPI_AENC_GetStream fail %x", s32Ret);
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_AENC_ReleaseStream(SAMPLE_AENC_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AENC_ReleaseStream(ctx->s32ChnId, &ctx->stFrame);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AENC_ReleaseStream fail %x", s32Ret);
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_AENC_DestroyChn(SAMPLE_AENC_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AENC_DestroyChn(ctx->s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
