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
#ifdef AVS_ENABLE
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

RK_S32 SAMPLE_COMM_AVS_CreateGrp(SAMPLE_AVS_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AVS_CreateGrp(ctx->s32GrpId, &ctx->stAvsGrpAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AVS_CreateGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	if (ctx->eGetLdchMode == RK_GET_LDCH_BY_BUFF) {
		AVS_FINAL_LUT_S pstFinalLut;
		MB_EXT_CONFIG_S stMbExtConfig;
		PIC_BUF_ATTR_S stBufAttr = {0};
		MB_PIC_CAL_S stPicCal = {0};
		stBufAttr.u32Width = ctx->u32SrcWidth;
		stBufAttr.u32Height = ctx->u32SrcHeight;
		s32Ret = RK_MPI_CAL_AVS_GetFinalLutBufferSize(&stBufAttr, &stPicCal);
		if (s32Ret != RK_SUCCESS || stPicCal.u32MBSize == 0) {
			RK_LOGE("RK_MPI_CAL_AVS_GetFinalLutBufferSize failure:%#X", s32Ret);
			return s32Ret;
		}

		memset(&pstFinalLut, 0, sizeof(AVS_FINAL_LUT_S));
		for (RK_S32 i = 0; i < ctx->stAvsGrpAttr.u32PipeNum; i++) {
			memset(&stMbExtConfig, 0, sizeof(MB_EXT_CONFIG_S));
			ctx->pLdchMeshData[i] = malloc(stPicCal.u32MBSize);
			if (!ctx->pLdchMeshData[i]) {
				RK_LOGE("malloc for pLdchMeshData failure");
				return RK_FAILURE;
			}
			RK_LOGD("pLdchMeshData:%p pLdchMeshData[%d]:%p", ctx->pLdchMeshData, i,
			        ctx->pLdchMeshData[i]);
			stMbExtConfig.pOpaque = ctx->pLdchMeshData[i];
			stMbExtConfig.pu8VirAddr = (RK_U8 *)ctx->pLdchMeshData[i];
			stMbExtConfig.u64Size = stPicCal.u32MBSize;
			RK_LOGD("pLdchMeshData:%p, ctx->pLdchMeshData[%d]:%p", ctx->pLdchMeshData, i,
			        ctx->pLdchMeshData[i]);
			s32Ret = RK_MPI_SYS_CreateMB(&(pstFinalLut.pLdchBlk[i]), &stMbExtConfig);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_SYS_CreateMB for pstMesh.pLdchBlk failure, s32Ret:%#X",
				        s32Ret);
				return s32Ret;
			}
		}
		RK_LOGE("-----------------RK_MPI_AVS_GetFinalLut---------");
		s32Ret = RK_MPI_AVS_GetFinalLut(ctx->s32GrpId, &pstFinalLut);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_AVS_GetFinalLut failure s32Ret:%#X", s32Ret);
			return s32Ret;
		}

		for (RK_S32 i = 0; i < ctx->stAvsGrpAttr.u32PipeNum; i++) {
			RK_MPI_SYS_Free(pstFinalLut.pLdchBlk[i]);
		}
	}
	RK_LOGE("-----------------RK_MPI_AVS_GetFinalLut");
	return s32Ret;
}

RK_S32 SAMPLE_COMM_AVS_StartGrp(SAMPLE_AVS_CTX_S *ctx) {
	RK_S32 chnIndex;
	RK_S32 s32Ret = RK_SUCCESS;

	if (ctx->s32GrpId >= AVS_MAX_GRP_NUM) {
		RK_LOGE("s32GrpNum is less than the maximum channel: %d", AVS_MAX_GRP_NUM);
		return RK_FAILURE;
	}

	if (ctx->s32ChnId >= AVS_MAX_CHN_NUM) {
		RK_LOGE("s32ChnNum is less than the maximum channel: %d", AVS_MAX_CHN_NUM);
		return RK_FAILURE;
	}

	for (chnIndex = 0; chnIndex < AVS_MAX_CHN_NUM; chnIndex++) {
		if (ctx->stAvsChnAttr[chnIndex].u32Width &&
		    ctx->stAvsChnAttr[chnIndex].u32Height) {
			s32Ret = RK_MPI_AVS_SetChnAttr(ctx->s32GrpId, chnIndex,
			                               &ctx->stAvsChnAttr[chnIndex]);
			if (RK_SUCCESS != s32Ret) {
				RK_LOGE("RK_MPI_AVS_SetChnAttr failed with %#x!\n", s32Ret);
				return s32Ret;
			}

			s32Ret = RK_MPI_AVS_EnableChn(ctx->s32GrpId, chnIndex);
			if (RK_SUCCESS != s32Ret) {
				RK_LOGE("RK_MPI_AVS_EnableChn failed with %#x!\n", s32Ret);
				return s32Ret;
			}
		}
	}

	s32Ret = RK_MPI_AVS_StartGrp(ctx->s32GrpId);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_AVS_StartGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_AVS_GetChnFrame(SAMPLE_AVS_CTX_S *ctx, void **pdata) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 waitTime = -1;

	s32Ret = RK_MPI_AVS_GetChnFrame(ctx->s32GrpId, ctx->s32ChnId, &ctx->stVideoFrame,
	                                waitTime);
	if (s32Ret == RK_SUCCESS) {
		*pdata = RK_MPI_MB_Handle2VirAddr(ctx->stVideoFrame.stVFrame.pMbBlk);
	} else {
		RK_LOGE("RK_MPI_AVS_GetChnFrame GrpId %d timeout %x", ctx->s32GrpId, s32Ret);
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_AVS_ReleaseChnFrame(SAMPLE_AVS_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AVS_ReleaseChnFrame(ctx->s32GrpId, ctx->s32ChnId, &ctx->stVideoFrame);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AVS_ReleaseChnFrame fail %x", s32Ret);
	}
	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_AVS_StopGrp(SAMPLE_AVS_CTX_S *ctx) {
	RK_S32 chnIndex;
	RK_S32 s32Ret = RK_FAILURE;
	s32Ret = RK_MPI_AVS_StopGrp(ctx->s32GrpId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AVS_StopGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	for (chnIndex = 0; chnIndex < AVS_MAX_CHN_NUM; chnIndex++) {
		if (ctx->stAvsChnAttr[chnIndex].u32Width &&
		    ctx->stAvsChnAttr[chnIndex].u32Height) {
			s32Ret = RK_MPI_AVS_DisableChn(ctx->s32GrpId, chnIndex);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_AVS_DisableChn failed with %#x!\n", s32Ret);
				return s32Ret;
			}
		}
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_AVS_DestroyGrp(SAMPLE_AVS_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = RK_MPI_AVS_DestroyGrp(ctx->s32GrpId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_AVS_DestroyGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	if (ctx->eGetLdchMode == RK_GET_LDCH_BY_BUFF) {
		for (RK_S32 i = 0; i < ctx->stAvsGrpAttr.u32PipeNum; i++) {
			if (ctx->pLdchMeshData[i] != RK_NULL) {
				free(ctx->pLdchMeshData[i]);
				ctx->pLdchMeshData[i] = RK_NULL;
			}
		}
	}
#if defined(RV1106)
	RK_MPI_SYS_WaitFreeMB();
#endif
	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif
