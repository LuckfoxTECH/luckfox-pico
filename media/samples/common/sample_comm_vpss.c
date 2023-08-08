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

RK_S32 SAMPLE_COMM_VPSS_CreateChn(SAMPLE_VPSS_CTX_S *ctx) {
	RK_S32 chnIndex;
	RK_S32 s32Ret = RK_SUCCESS;
	ROTATION_E rotation = ROTATION_0;
	VIDEO_PROC_DEV_TYPE_E enTmpVProcDevType;

	if (ctx->s32GrpId >= VPSS_MAX_GRP_NUM) {
		RK_LOGE("s32GrpId is less than the maximum channel: %d", VPSS_MAX_GRP_NUM);
		return RK_FAILURE;
	}

	ctx->stGrpVpssAttr.u32MaxW = 4096;
	ctx->stGrpVpssAttr.u32MaxH = 4096;
	ctx->stGrpVpssAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->stGrpVpssAttr.stFrameRate.s32DstFrameRate = -1;
	s32Ret = RK_MPI_VPSS_CreateGrp(ctx->s32GrpId, &ctx->stGrpVpssAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_CreateGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_SetVProcDev(ctx->s32GrpId, ctx->enVProcDevType);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_SetVProcDev(grp:%d) failed with %#x!", ctx->s32GrpId,
		        s32Ret);
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_GetVProcDev(ctx->s32GrpId, &enTmpVProcDevType);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_GetVProcDev(grp:%d) failed with %#x!", ctx->s32GrpId,
		        s32Ret);
		return s32Ret;
	}
	RK_LOGI("vpss Grp %d's work unit is %d", ctx->s32GrpId, enTmpVProcDevType);

	s32Ret = RK_MPI_VPSS_ResetGrp(ctx->s32GrpId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_ResetGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_SetGrpCrop(ctx->s32GrpId, &ctx->stCropInfo);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_SetGrpCrop failed with %#x!\n", s32Ret);
		return s32Ret;
	}
	s32Ret = RK_MPI_VPSS_GetGrpCrop(ctx->s32GrpId, &ctx->stCropInfo);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_GetGrpCrop failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	for (chnIndex = 0; chnIndex < VPSS_MAX_CHN_NUM; chnIndex++) {
		if (ctx->stVpssChnAttr[chnIndex].u32Width &&
		    ctx->stVpssChnAttr[chnIndex].u32Height) {

			s32Ret = RK_MPI_VPSS_SetChnCrop(ctx->s32GrpId, chnIndex,
			                                &ctx->stChnCropInfo[chnIndex]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_SetChnCrop failed with %#x!\n", s32Ret);
				return s32Ret;
			}
			s32Ret = RK_MPI_VPSS_GetChnCrop(ctx->s32GrpId, chnIndex,
			                                &ctx->stChnCropInfo[chnIndex]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_GetChnCrop failed with %#x!\n", s32Ret);
				return s32Ret;
			}

			s32Ret = RK_MPI_VPSS_SetChnRotation(
			    ctx->s32GrpId, chnIndex, (ROTATION_E)ctx->s32ChnRotation[chnIndex]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_SetChnRotation failed with %#x!\n", s32Ret);
				return s32Ret;
			}
			s32Ret = RK_MPI_VPSS_GetChnRotation(ctx->s32GrpId, chnIndex, &rotation);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_GetChnRotation failed with %#x!\n", s32Ret);
				return s32Ret;
			}
			if (rotation != ctx->s32ChnRotation[chnIndex]) {
				RK_LOGE("RK_MPI_VPSS_SetChnRotation failed!\n");
				s32Ret = RK_FAILURE;
				return s32Ret;
			}
#if 0   
            VPSS_ROTATION_EX_ATTR_S stRotationEx;
            stRotationEx.stRotationEx.u32Angle = ctx->stRotationEx[chnIndex].stRotationEx.u32Angle;
            s32Ret = RK_MPI_VPSS_SetChnRotationEx(ctx->s32GrpId, chnIndex, &stRotationEx);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VPSS_SetChnRotationEx failed with %#x!\n", s32Ret);
                return s32Ret;
            }
            s32Ret = RK_MPI_VPSS_GetChnRotationEx(ctx->s32GrpId, chnIndex, &stRotationEx);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VPSS_GetChnRotationEx failed with %#x!\n", s32Ret);
                return s32Ret;
            }
            if (ctx->stRotationEx[chnIndex].stRotationEx.u32Angle != 
                    stRotationEx.stRotationEx.u32Angle) {
                s32Ret = RK_FAILURE;
                RK_LOGE("Set Angle failed with %#x!\n", s32Ret);
                return s32Ret;
            }
#endif
			s32Ret = RK_MPI_VPSS_SetChnAttr(ctx->s32GrpId, chnIndex,
			                                &ctx->stVpssChnAttr[chnIndex]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_SetChnAttr failed with %#x!\n", s32Ret);
				return s32Ret;
			}
			s32Ret = RK_MPI_VPSS_GetChnAttr(ctx->s32GrpId, chnIndex,
			                                &ctx->stVpssChnAttr[chnIndex]);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_GetChnAttr failed with %#x!\n", s32Ret);
				return s32Ret;
			}
			s32Ret = RK_MPI_VPSS_EnableChn(ctx->s32GrpId, chnIndex);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_EnableChn failed with %#x!\n", s32Ret);
				return s32Ret;
			}
		}
	}

	s32Ret = RK_MPI_VPSS_StartGrp(ctx->s32GrpId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_StartGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VPSS_SendStream(SAMPLE_VPSS_CTX_S *ctx, void *pdata, RK_S32 width,
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
	s32Ret = RK_MPI_VPSS_SendFrame(ctx->s32GrpId, 0, &stFrame, -1);
	if (s32Ret == RK_SUCCESS) {
		RK_MPI_MB_ReleaseMB(blk);
	} else {
		RK_LOGE("RK_MPI_VPSS_SendFrame fail %x", s32Ret);
		usleep(10000llu);
		goto __RETRY1;
	}

	return s32Ret;
}

RK_S32 SAMPLE_COMM_VPSS_GetChnFrame(SAMPLE_VPSS_CTX_S *ctx, void **pdata) {
	RK_S32 s32Ret = RK_FAILURE;
	PIC_BUF_ATTR_S stPicBufAttr;
	MB_PIC_CAL_S stMbPicCalResult;

	s32Ret =
	    RK_MPI_VPSS_GetChnFrame(ctx->s32GrpId, ctx->s32ChnId, &ctx->stChnFrameInfos, -1);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_GetChnFrame fail %x", s32Ret);
	}

	stPicBufAttr.u32Width = ctx->stChnFrameInfos.stVFrame.u32VirWidth;
	stPicBufAttr.u32Height = ctx->stChnFrameInfos.stVFrame.u32VirHeight;
	stPicBufAttr.enPixelFormat = ctx->stChnFrameInfos.stVFrame.enPixelFormat;
	stPicBufAttr.enCompMode = ctx->stChnFrameInfos.stVFrame.enCompressMode;
	s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_CAL_VGS_GetPicBufferSize failed. err=0x%x", s32Ret);
		return s32Ret;
	}

	RK_MPI_SYS_MmzFlushCache(ctx->stChnFrameInfos.stVFrame.pMbBlk, RK_TRUE);

	*pdata = RK_MPI_MB_Handle2VirAddr(ctx->stChnFrameInfos.stVFrame.pMbBlk);

	ctx->stChnFrameInfos.stVFrame.u64PrivateData = stMbPicCalResult.u32MBSize;

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VPSS_ReleaseChnFrame(SAMPLE_VPSS_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret =
	    RK_MPI_VPSS_ReleaseChnFrame(ctx->s32GrpId, ctx->s32ChnId, &ctx->stChnFrameInfos);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
	}
	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VPSS_SetChnAttr(SAMPLE_VPSS_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 chnIndex = ctx->s32ChnId;

	s32Ret =
	    RK_MPI_VPSS_SetChnAttr(ctx->s32GrpId, chnIndex, &ctx->stVpssChnAttr[chnIndex]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_SetChnAttr failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_VPSS_DestroyChn(SAMPLE_VPSS_CTX_S *ctx) {
	RK_S32 chnIndex;
	RK_S32 s32Ret = RK_FAILURE;

	for (chnIndex = 0; chnIndex < VPSS_MAX_CHN_NUM; chnIndex++) {
		if (ctx->stVpssChnAttr[chnIndex].u32Width &&
		    ctx->stVpssChnAttr[chnIndex].u32Height) {

			s32Ret = RK_MPI_VPSS_DisableChn(ctx->s32GrpId, chnIndex);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VPSS_DisableChn failed with %#x!\n", s32Ret);
				return s32Ret;
			}
		}
	}

	s32Ret = RK_MPI_VPSS_StopGrp(ctx->s32GrpId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_StopGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_DestroyGrp(ctx->s32GrpId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VPSS_DestroyGrp failed with %#x!\n", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
