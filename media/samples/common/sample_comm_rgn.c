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

static RK_S32 test_rgn_load_bmp(SAMPLE_RGN_CTX_S *ctx) {
	OSD_SURFACE_S Surface;
	OSD_BITMAPFILEHEADER bmpFileHeader;
	OSD_BITMAPINFO bmpInfo;
	RK_BOOL bCPU = RK_FALSE;

	if (get_bmp_info(ctx->srcFileBmpName, &bmpFileHeader, &bmpInfo) < 0) {
		RK_LOGE("GetBmpInfo err, generate from cpu!\n");
		bCPU = RK_TRUE;
		// return RK_FAILURE;
	}

	switch (ctx->u32BmpFormat) {
	case RK_FMT_ARGB8888:
		Surface.enColorFmt = OSD_COLOR_FMT_ARGB8888;
		break;
	case RK_FMT_BGRA8888:
		Surface.enColorFmt = OSD_COLOR_FMT_BGRA8888;
		break;
	case RK_FMT_ARGB1555:
		Surface.enColorFmt = OSD_COLOR_FMT_ARGB1555;
		break;
	case RK_FMT_BGRA5551:
		Surface.enColorFmt = OSD_COLOR_FMT_BGRA5551;
		break;
	default:
		return RK_FAILURE;
	}

	if (bCPU) {
		ctx->stBitmap.pData =
		    malloc(4 * (ctx->stRegion.u32Width) * (ctx->stRegion.u32Height));

		if (RK_NULL == ctx->stBitmap.pData) {
			RK_LOGE("malloc osd memroy err!");
			return RK_FAILURE;
		}
		SAMPLE_COMM_FillImage(ctx->stBitmap.pData, ctx->stRegion.u32Width,
		                      ctx->stRegion.u32Height, ctx->stRegion.u32Width,
		                      ctx->stRegion.u32Height, ctx->u32BmpFormat, 0);
		ctx->stBitmap.u32Width = ctx->stRegion.u32Width;
		ctx->stBitmap.u32Height = ctx->stRegion.u32Height;
		ctx->stBitmap.enPixelFormat = ctx->u32BmpFormat;
		return RK_SUCCESS;
	}

	ctx->stBitmap.pData =
	    malloc(4 * (bmpInfo.bmiHeader.biWidth) * (bmpInfo.bmiHeader.biHeight));

	if (RK_NULL == ctx->stBitmap.pData) {
		RK_LOGE("malloc osd memroy err!");
		return RK_FAILURE;
	}

	create_surface_by_bitmap(ctx->srcFileBmpName, &Surface,
	                         (RK_U8 *)(ctx->stBitmap.pData));

	ctx->stBitmap.u32Width = Surface.u16Width;
	ctx->stBitmap.u32Height = Surface.u16Height;
	ctx->stBitmap.enPixelFormat = (PIXEL_FORMAT_E)ctx->u32BmpFormat;

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_RGN_CreateChn(SAMPLE_RGN_CTX_S *ctx) {
	RK_S64 s64TimeStart;
	RK_S64 s64TimeEnd;
	RK_S32 s32Ret = RK_SUCCESS;

	switch (ctx->stRgnAttr.enType) {
	case OVERLAY_RGN: {
		ctx->stRgnAttr.enType = OVERLAY_RGN;
		ctx->stRgnAttr.unAttr.stOverlay.enPixelFmt = (PIXEL_FORMAT_E)ctx->u32BmpFormat;
		ctx->stRgnAttr.unAttr.stOverlay.stSize.u32Width = ctx->stRegion.u32Width;
		ctx->stRgnAttr.unAttr.stOverlay.stSize.u32Height = ctx->stRegion.u32Height;

		ctx->stRgnChnAttr.bShow = RK_TRUE;
		ctx->stRgnChnAttr.enType = OVERLAY_RGN;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = ctx->stRegion.s32X;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = ctx->stRegion.s32Y;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = ctx->u32BgAlpha;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = ctx->u32FgAlpha;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = ctx->u32Layer;

		s32Ret = test_rgn_load_bmp(ctx);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("test_rgn_load_bmp failure:%#X", s32Ret);
			return s32Ret;
		}
	} break;
	case COVER_RGN: {
		ctx->stRgnAttr.enType = COVER_RGN;
		ctx->stRgnChnAttr.bShow = RK_TRUE;
		ctx->stRgnChnAttr.enType = COVER_RGN;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X = ctx->stRegion.s32X;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y = ctx->stRegion.s32Y;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width = ctx->stRegion.u32Width;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height = ctx->stRegion.u32Height;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.u32Color = ctx->u32Color;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.u32Layer = ctx->u32Layer;
	} break;
	case MOSAIC_RGN: {
		ctx->stRgnAttr.enType = MOSAIC_RGN;
		ctx->stRgnChnAttr.bShow = RK_TRUE;
		ctx->stRgnChnAttr.enType = MOSAIC_RGN;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.s32X = ctx->stRegion.s32X;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.s32Y = ctx->stRegion.s32Y;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.u32Width = ctx->stRegion.u32Width;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.u32Height =
		    ctx->stRegion.u32Height;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.enBlkSize = MOSAIC_BLK_SIZE_64;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.u32Layer = ctx->u32Layer;
	} break;
	default:
		RK_LOGE("unsupport type %d.", ctx->stRgnAttr.enType);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_RGN_Create(ctx->rgnHandle, &ctx->stRgnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", ctx->rgnHandle, s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_RGN_AttachToChn(ctx->rgnHandle, &ctx->stMppChn, &ctx->stRgnChnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgnHandle, s32Ret);
		return RK_FAILURE;
	}

	if (ctx->stRgnAttr.enType == OVERLAY_RGN) {
		// s64TimeStart = mpi_test_utils_get_now_us();

		s32Ret = RK_MPI_RGN_SetBitMap(ctx->rgnHandle, &ctx->stBitmap);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_RGN_SetBitMap failed with %#x!", s32Ret);
			if (RK_NULL != ctx->stBitmap.pData) {
				free(ctx->stBitmap.pData);
				ctx->stBitmap.pData = NULL;
			}
			return RK_FAILURE;
		}

		if (RK_NULL != ctx->stBitmap.pData) {
			free(ctx->stBitmap.pData);
			ctx->stBitmap.pData = NULL;
		}

		// s64TimeEnd = mpi_test_utils_get_now_us();
		// RK_LOGI("Handle:%d, space time %lld us, load bmp success!",
		// ctx->rgnHandle, s64TimeEnd -
		// s64TimeStart);
	}

	s32Ret =
	    RK_MPI_RGN_GetDisplayAttr(ctx->rgnHandle, &ctx->stMppChn, &ctx->stRgnChnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!", ctx->rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}

	switch (ctx->stRgnAttr.enType) {
	case OVERLAY_RGN: {
		ctx->stRgnChnAttr.bShow = RK_TRUE;
		ctx->stRgnChnAttr.enType = OVERLAY_RGN;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = ctx->stRegion.s32X;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = ctx->stRegion.s32Y;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = ctx->u32BgAlpha;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = ctx->u32FgAlpha;
		ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = ctx->u32Layer;
		RK_LOGE("resize the overlay region %d to <%d, %d> BgAlpha %d "
		        "FgAlpha%d, color<0x%x>",
		        ctx->rgnHandle, ctx->stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X,
		        ctx->stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y,
		        ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha,
		        ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha,
		        ctx->stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer);
	} break;
	case COVER_RGN: {
		ctx->stRgnChnAttr.bShow = RK_TRUE;
		ctx->stRgnChnAttr.enType = COVER_RGN;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X = ctx->stRegion.s32X;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y = ctx->stRegion.s32Y;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width = ctx->stRegion.u32Width;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height = ctx->stRegion.u32Height;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.u32Color = ctx->u32Color;
		ctx->stRgnChnAttr.unChnAttr.stCoverChn.u32Layer = ctx->u32Layer;
		RK_LOGE("resize the cover region %d to <%d, %d, %d, %d>, color<0x%x>",
		        ctx->rgnHandle, ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X,
		        ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y,
		        ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width,
		        ctx->stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height,
		        ctx->stRgnChnAttr.unChnAttr.stCoverChn.u32Color);
	} break;
	case MOSAIC_RGN: {
		ctx->stRgnChnAttr.bShow = RK_TRUE;
		ctx->stRgnChnAttr.enType = MOSAIC_RGN;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.s32X = ctx->stRegion.s32X;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.s32Y = ctx->stRegion.s32Y;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.u32Width = ctx->stRegion.u32Width;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.stRect.u32Height =
		    ctx->stRegion.u32Height;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.enBlkSize = MOSAIC_BLK_SIZE_64;
		ctx->stRgnChnAttr.unChnAttr.stMosaicChn.u32Layer = ctx->u32Layer;
	} break;
	default:
		RK_LOGE("unsupport type %d.", ctx->stRgnAttr.enType);
		return RK_FAILURE;
	}

	s32Ret =
	    RK_MPI_RGN_SetDisplayAttr(ctx->rgnHandle, &ctx->stMppChn, &ctx->stRgnChnAttr);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!", ctx->rgnHandle,
		        s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_RGN_DestroyChn(SAMPLE_RGN_CTX_S *ctx) {
	RK_S32 s32Ret = RK_SUCCESS;

	s32Ret = RK_MPI_RGN_DetachFromChn(ctx->rgnHandle, &ctx->stMppChn);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", ctx->rgnHandle, s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_RGN_Destroy(ctx->rgnHandle);
	if (RK_SUCCESS != s32Ret) {
		RK_LOGE("RK_MPI_RGN_Destroy [%d] failed with %#x", ctx->rgnHandle, s32Ret);
		return RK_FAILURE;
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
