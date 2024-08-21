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

RK_S32 SAMPLE_COMM_VO_CreateChn(SAMPLE_VO_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	VO_PUB_ATTR_S pstA;
#ifdef HAVE_VO
	// 0. Set/Enable vo PubAttr
	s32Ret = RK_MPI_VO_SetPubAttr(ctx->s32DevId, &ctx->stVoPubAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_SetPubAttr failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_VO_Enable(ctx->s32DevId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_Enable failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	} else {
		RK_LOGE("RK_MPI_VO_Enable already");
	}

	// 1. Set dispBufLen
	RK_U32 u32DispBufLen;
	s32Ret = RK_MPI_VO_GetLayerDispBufLen(ctx->s32LayerId, &u32DispBufLen);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_GetLayerDispBufLen failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}
	RK_LOGD("Get s32LayerId %d disp buf len is %d", ctx->s32LayerId, u32DispBufLen);

	s32Ret = RK_MPI_VO_SetLayerDispBufLen(ctx->s32LayerId, ctx->u32DispBufLen);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_SetLayerDispBufLen failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}
	RK_LOGD("Set s32LayerId %d disp buf len is %d", ctx->s32LayerId, ctx->u32DispBufLen);

	// 2. Set/Enable Layer
	s32Ret = RK_MPI_VO_GetPubAttr(ctx->s32DevId, &pstA);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_GetPubAttr failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}
	RK_LOGD("pstA.stSyncInfo.u16Hact %d", pstA.stSyncInfo.u16Hact);
	RK_LOGD("pstA.stSyncInfo.u16Vact %d", pstA.stSyncInfo.u16Vact);

	if ((ctx->stLayerAttr.stDispRect.u32Width <= 0) &&
	    (ctx->stLayerAttr.stDispRect.u32Height <= 0) &&
	    (ctx->stLayerAttr.stImageSize.u32Width <= 0) &&
	    (ctx->stLayerAttr.stImageSize.u32Height <= 0)) {

		ctx->stLayerAttr.stDispRect.u32Width = pstA.stSyncInfo.u16Hact;
		ctx->stLayerAttr.stDispRect.u32Height = pstA.stSyncInfo.u16Vact;
		ctx->stLayerAttr.stImageSize.u32Width = pstA.stSyncInfo.u16Hact;
		ctx->stLayerAttr.stImageSize.u32Height = pstA.stSyncInfo.u16Vact;
	}

	s32Ret = RK_MPI_VO_BindLayer(ctx->s32LayerId, ctx->s32DevId, ctx->Volayer_mode);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_BindLayer failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_VO_SetLayerAttr(ctx->s32LayerId, &ctx->stLayerAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_SetLayerAttr failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_VO_EnableLayer(ctx->s32LayerId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_EnableLayer failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	} else {
		RK_LOGE("RK_MPI_VO_EnableLayer already");
	}

	// 3. Set/Enable ChnAttr
	if ((ctx->stChnAttr.stRect.u32Width <= 0) && (ctx->stChnAttr.stRect.u32Height <= 0)) {
		ctx->stChnAttr.stRect.u32Width = pstA.stSyncInfo.u16Hact;
		ctx->stChnAttr.stRect.u32Height = pstA.stSyncInfo.u16Vact;
	}
	s32Ret = RK_MPI_VO_SetChnAttr(ctx->s32LayerId, ctx->s32ChnId, &ctx->stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_SetChnAttr failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_VO_EnableChn(ctx->s32LayerId, ctx->s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_EnableChn failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	} else {
		RK_LOGE("RK_MPI_VO_EnableChn already");
	}
#endif
	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VO_DestroyChn(SAMPLE_VO_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
#ifdef HAVE_VO
	s32Ret = RK_MPI_VO_DisableChn(ctx->s32LayerId, ctx->s32ChnId);
	s32Ret |= RK_MPI_VO_DisableLayer(ctx->s32LayerId);
	s32Ret |= RK_MPI_VO_Disable(ctx->s32DevId);
	s32Ret |= RK_MPI_VO_UnBindLayer(ctx->s32LayerId, ctx->s32DevId);
	s32Ret |= RK_MPI_VO_CloseFd();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_Close failed with %#x!\n", s32Ret);
		return RK_FAILURE;
	} else {
		RK_LOGE("RK_MPI_VO_Close success");
	}
#endif
	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
