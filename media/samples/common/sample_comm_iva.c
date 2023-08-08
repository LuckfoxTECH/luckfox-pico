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

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>

#include "sample_comm.h"
#define ROCKIVA_PATH_LENGTH (128)
#define MODEL_DATA_PATH "/usr/lib/"

#ifdef ROCKIVA
RK_S32 SAMPLE_COMM_IVA_Create(SAMPLE_IVA_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	snprintf(ctx->commonParams.modelPath, ROCKIVA_PATH_LENGTH, MODEL_DATA_PATH);
	ctx->commonParams.coreMask = 0x04;
	ctx->commonParams.logLevel = ROCKIVA_LOG_ERROR;
	ctx->eModeType |= ROCKIVA_OBJECT_TYPE_PET;        /* use small model */
	ctx->commonParams.detObjectType = ctx->eModeType; /* Detect type */
	ctx->commonParams.imageInfo.width = ctx->u32ImageWidth;
	ctx->commonParams.imageInfo.height = ctx->u32ImageHeight;
	ctx->commonParams.imageInfo.format = ctx->eImageFormat;
	ctx->commonParams.imageInfo.transformMode = ctx->eImageTransform;

	/* IVA init */
	s32Ret = ROCKIVA_Init(&ctx->ivahandle, ROCKIVA_MODE_VIDEO, &ctx->commonParams, NULL);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("ROCKIVA_Init failure:%X", s32Ret);
		return s32Ret;
	}

	/* Set Detect area */
	ctx->baParams.baRules.areaInBreakRule[0].ruleEnable = RK_TRUE;
	ctx->baParams.baRules.areaInBreakRule[0].sense = 90;
	ctx->baParams.baRules.areaInBreakRule[0].alertTime = 1000; /* 1000 ms */
	ctx->baParams.baRules.areaInBreakRule[0].minObjSize[2].width =
	    ctx->u32ImageWidth / 100;
	ctx->baParams.baRules.areaInBreakRule[0].minObjSize[2].height =
	    ctx->u32ImageHeight / 100;
	ctx->baParams.baRules.areaInBreakRule[0].event = ROCKIVA_BA_TRIP_EVENT_STAY;
	ctx->baParams.baRules.areaInBreakRule[0].ruleID = 0;
	ctx->baParams.baRules.areaInBreakRule[0].objType = ROCKIVA_BA_RULE_OBJ_FULL;
	ctx->baParams.baRules.areaInBreakRule[0].area.pointNum = 4;
	ctx->baParams.baRules.areaInBreakRule[0].area.points[0].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageWidth, ctx->u32DetectStartX);
	ctx->baParams.baRules.areaInBreakRule[0].area.points[0].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageHeight, ctx->u32DetectStartY);
	ctx->baParams.baRules.areaInBreakRule[0].area.points[1].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageWidth,
	                                 ctx->u32DetectStartX + ctx->u32DetectWidth);
	ctx->baParams.baRules.areaInBreakRule[0].area.points[1].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageHeight, ctx->u32DetectStartY);
	ctx->baParams.baRules.areaInBreakRule[0].area.points[2].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageWidth,
	                                 ctx->u32DetectStartX + ctx->u32DetectWidth);
	ctx->baParams.baRules.areaInBreakRule[0].area.points[2].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageHeight,
	                                 ctx->u32DetectStartY + ctx->u32DetectHight);
	ctx->baParams.baRules.areaInBreakRule[0].area.points[3].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageWidth, ctx->u32DetectStartX);
	ctx->baParams.baRules.areaInBreakRule[0].area.points[3].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(ctx->u32ImageHeight,
	                                 ctx->u32DetectStartY + ctx->u32DetectHight);
	ctx->baParams.aiConfig.detectResultMode = 0;

	s32Ret = ROCKIVA_BA_Init(ctx->ivahandle, &ctx->baParams, ctx->resultCallback);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("ROCKIVA_BA_Init failure:%X", s32Ret);
		return s32Ret;
	}
}

RK_S32 SAMPLE_COMM_IVA_Destroy(SAMPLE_IVA_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;

	s32Ret = ROCKIVA_BA_Release(ctx->ivahandle);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("ROCKIVA_BA_Release failure:%X", s32Ret);
		return s32Ret;
	}

	s32Ret = ROCKIVA_Release(ctx->ivahandle);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("ROCKIVA_Release failure:%X", s32Ret);
	}

	return s32Ret;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
