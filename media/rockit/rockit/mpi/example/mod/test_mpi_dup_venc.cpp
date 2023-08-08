/*
 * Copyright 2018 Rockchip Electronics Co. LTD
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
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_ivs.h"
#include "rk_comm_ivs.h"

#include "test_comm_argparse.h"
#include "test_comm_imgproc.h"
#include "test_comm_venc.h"
#include "test_comm_utils.h"

#define MAX_TIME_OUT_MS          20
#define TEST_RC_MODE             0
#define TEST_CROP_PARAM_CHANGE   0

typedef struct _rkMpiVENCCtx {
    const char     *dstFilePath;
    RK_U32          u32ChnIndex;
    RK_BOOL         threadExit;
    RK_BOOL         bInsertUserData;
} TEST_VENC_CTX_S;

static void dump_venc_chn_attr(VENC_CHN_ATTR_S *pstAttr) {
    printf("%s:\n", __func__);
    printf("stVencAttr.enType: %d\n", pstAttr->stVencAttr.enType);
    printf("          .u32MaxPicWidth: %d\n", pstAttr->stVencAttr.u32MaxPicWidth);
    printf("          .u32MaxPicHeight: %d\n", pstAttr->stVencAttr.u32MaxPicHeight);
    printf("          .enPixelFormat: %d\n", pstAttr->stVencAttr.enPixelFormat);
    printf("          .enMirror: %d\n", pstAttr->stVencAttr.enMirror);
    printf("          .u32Profile: %d\n", pstAttr->stVencAttr.u32Profile);
    printf("          .u32PicWidth: %d\n", pstAttr->stVencAttr.u32PicWidth);
    printf("          .u32PicHeight: %d\n", pstAttr->stVencAttr.u32PicHeight);
    printf("          .u32VirWidth: %d\n", pstAttr->stVencAttr.u32VirWidth);
    printf("          .u32VirHeight: %d\n", pstAttr->stVencAttr.u32VirHeight);
    printf("          .u32StreamBufCnt: %d\n", pstAttr->stVencAttr.u32StreamBufCnt);
    printf("stRcAttr.enRcMode: %d\n", pstAttr->stRcAttr.enRcMode);
    switch (pstAttr->stRcAttr.enRcMode) {
    case VENC_RC_MODE_H264CBR:
        printf("        .stH264Cbr.u32Gop: %d\n", pstAttr->stRcAttr.stH264Cbr.u32Gop);
        printf("                  .u32BitRate: %d\n", pstAttr->stRcAttr.stH264Cbr.u32BitRate);
        printf("                  .u32StatTime: %d\n", pstAttr->stRcAttr.stH264Cbr.u32StatTime);
        printf("                  .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH264Cbr.u32SrcFrameRateNum);
        printf("                  .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH264Cbr.u32SrcFrameRateDen);
        printf("                  .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH264Cbr.fr32DstFrameRateNum);
        printf("                  .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH264Cbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_H264VBR:
        printf("        .stH264Vbr.u32Gop: %d\n", pstAttr->stRcAttr.stH264Vbr.u32Gop);
        printf("                  .u32BitRate: %d\n", pstAttr->stRcAttr.stH264Vbr.u32BitRate);
        printf("                  .u32MaxBitRate: %d\n", pstAttr->stRcAttr.stH264Vbr.u32MaxBitRate);
        printf("                  .u32MinBitRate: %d\n", pstAttr->stRcAttr.stH264Vbr.u32MinBitRate);
        printf("                  .u32StatTime: %d\n", pstAttr->stRcAttr.stH264Vbr.u32StatTime);
        printf("                  .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH264Vbr.u32SrcFrameRateNum);
        printf("                  .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH264Vbr.u32SrcFrameRateDen);
        printf("                  .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH264Vbr.fr32DstFrameRateNum);
        printf("                  .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH264Vbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_H264AVBR:
        printf("        .stH264Avbr.u32Gop: %d\n", pstAttr->stRcAttr.stH264Avbr.u32Gop);
        printf("                   .u32BitRate: %d\n", pstAttr->stRcAttr.stH264Avbr.u32BitRate);
        printf("                   .u32MaxBitRate: %d\n", pstAttr->stRcAttr.stH264Avbr.u32MaxBitRate);
        printf("                   .u32MinBitRate: %d\n", pstAttr->stRcAttr.stH264Avbr.u32MinBitRate);
        printf("                   .u32StatTime: %d\n", pstAttr->stRcAttr.stH264Avbr.u32StatTime);
        printf("                   .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH264Avbr.u32SrcFrameRateNum);
        printf("                   .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH264Avbr.u32SrcFrameRateDen);
        printf("                   .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH264Avbr.fr32DstFrameRateNum);
        printf("                   .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH264Avbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_H264FIXQP:
        printf("        .stH264FixQp.u32Gop: %d\n", pstAttr->stRcAttr.stH264FixQp.u32Gop);
        printf("                    .u32IQp: %d\n", pstAttr->stRcAttr.stH264FixQp.u32IQp);
        printf("                    .u32PQp: %d\n", pstAttr->stRcAttr.stH264FixQp.u32PQp);
        printf("                    .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH264FixQp.u32SrcFrameRateNum);
        printf("                    .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH264FixQp.u32SrcFrameRateDen);
        printf("                    .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH264FixQp.fr32DstFrameRateNum);
        printf("                    .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH264FixQp.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_H265CBR:
        printf("        .stH265Cbr.u32Gop: %d\n", pstAttr->stRcAttr.stH265Cbr.u32Gop);
        printf("                  .u32BitRate: %d\n", pstAttr->stRcAttr.stH265Cbr.u32BitRate);
        printf("                  .u32StatTime: %d\n", pstAttr->stRcAttr.stH265Cbr.u32StatTime);
        printf("                  .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH265Cbr.u32SrcFrameRateNum);
        printf("                  .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH265Cbr.u32SrcFrameRateDen);
        printf("                  .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH265Cbr.fr32DstFrameRateNum);
        printf("                  .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH265Cbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_H265VBR:
        printf("        .stH265Vbr.u32Gop: %d\n", pstAttr->stRcAttr.stH265Vbr.u32Gop);
        printf("                  .u32BitRate: %d\n", pstAttr->stRcAttr.stH265Vbr.u32BitRate);
        printf("                  .u32MaxBitRate: %d\n", pstAttr->stRcAttr.stH265Vbr.u32MaxBitRate);
        printf("                  .u32MinBitRate: %d\n", pstAttr->stRcAttr.stH265Vbr.u32MinBitRate);
        printf("                  .u32StatTime: %d\n", pstAttr->stRcAttr.stH265Vbr.u32StatTime);
        printf("                  .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH265Vbr.u32SrcFrameRateNum);
        printf("                  .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH265Vbr.u32SrcFrameRateDen);
        printf("                  .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH265Vbr.fr32DstFrameRateNum);
        printf("                  .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH265Vbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_H265AVBR:
        printf("        .stH265Avbr.u32Gop: %d\n", pstAttr->stRcAttr.stH265Avbr.u32Gop);
        printf("                   .u32BitRate: %d\n", pstAttr->stRcAttr.stH265Avbr.u32BitRate);
        printf("                   .u32MaxBitRate: %d\n", pstAttr->stRcAttr.stH265Avbr.u32MaxBitRate);
        printf("                   .u32MinBitRate: %d\n", pstAttr->stRcAttr.stH265Avbr.u32MinBitRate);
        printf("                   .u32StatTime: %d\n", pstAttr->stRcAttr.stH265Avbr.u32StatTime);
        printf("                   .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH265Avbr.u32SrcFrameRateNum);
        printf("                   .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH265Avbr.u32SrcFrameRateDen);
        printf("                   .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH265Avbr.fr32DstFrameRateNum);
        printf("                   .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH265Avbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_H265FIXQP:
        printf("        .stH265FixQp.u32Gop: %d\n", pstAttr->stRcAttr.stH265FixQp.u32Gop);
        printf("                    .u32IQp: %d\n", pstAttr->stRcAttr.stH265FixQp.u32IQp);
        printf("                    .u32PQp: %d\n", pstAttr->stRcAttr.stH265FixQp.u32PQp);
        printf("                    .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stH265FixQp.u32SrcFrameRateNum);
        printf("                    .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stH265FixQp.u32SrcFrameRateDen);
        printf("                    .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stH265FixQp.fr32DstFrameRateNum);
        printf("                    .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stH265FixQp.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_MJPEGCBR:
        printf("        .stMjpegCbr.u32BitRate: %d\n", pstAttr->stRcAttr.stMjpegCbr.u32BitRate);
        printf("                   .u32StatTime: %d\n", pstAttr->stRcAttr.stMjpegCbr.u32StatTime);
        printf("                   .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stMjpegCbr.u32SrcFrameRateNum);
        printf("                   .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stMjpegCbr.u32SrcFrameRateDen);
        printf("                   .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stMjpegCbr.fr32DstFrameRateNum);
        printf("                   .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stMjpegCbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_MJPEGVBR:
        printf("        .stMjpegVbr.u32BitRate: %d\n", pstAttr->stRcAttr.stMjpegVbr.u32BitRate);
        printf("                   .u32MaxBitRate: %d\n", pstAttr->stRcAttr.stMjpegVbr.u32MaxBitRate);
        printf("                   .u32MinBitRate: %d\n", pstAttr->stRcAttr.stMjpegVbr.u32MinBitRate);
        printf("                   .u32StatTime: %d\n", pstAttr->stRcAttr.stMjpegVbr.u32StatTime);
        printf("                   .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stMjpegVbr.u32SrcFrameRateNum);
        printf("                   .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stMjpegVbr.u32SrcFrameRateDen);
        printf("                   .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stMjpegVbr.fr32DstFrameRateNum);
        printf("                   .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stMjpegVbr.fr32DstFrameRateDen);
        break;
    case VENC_RC_MODE_MJPEGFIXQP:
        printf("        .stMjpegFixQp.u32Qfactor: %d\n", pstAttr->stRcAttr.stMjpegFixQp.u32Qfactor);
        printf("                   .u32SrcFrameRateNum: %d\n", pstAttr->stRcAttr.stMjpegFixQp.u32SrcFrameRateNum);
        printf("                   .u32SrcFrameRateDen: %d\n", pstAttr->stRcAttr.stMjpegFixQp.u32SrcFrameRateDen);
        printf("                   .fr32DstFrameRateNum: %d\n", pstAttr->stRcAttr.stMjpegFixQp.fr32DstFrameRateNum);
        printf("                   .fr32DstFrameRateDen: %d\n", pstAttr->stRcAttr.stMjpegFixQp.fr32DstFrameRateDen);
        break;
    }
    printf("stGopAttr.enGopMode: %d\n", pstAttr->stGopAttr.enGopMode);
    printf("         .s32VirIdrLen: %d\n", pstAttr->stGopAttr.s32VirIdrLen);
    printf("         .u32MaxLtrCount: %d\n", pstAttr->stGopAttr.u32MaxLtrCount);
    printf("************************************************************\n");
}

static void dump_venc_chn_param(VENC_CHN_PARAM_S *pstParam) {
    printf("%s:\n", __func__);
    printf("stCropCfg.enCropType: %d\n", pstParam->stCropCfg.enCropType);
    printf("         .stCropRect.s32X: %d\n", pstParam->stCropCfg.stCropRect.s32X);
    printf("                    .s32Y: %d\n", pstParam->stCropCfg.stCropRect.s32Y);
    printf("                    .u32Width: %d\n", pstParam->stCropCfg.stCropRect.u32Width);
    printf("                    .u32Height: %d\n", pstParam->stCropCfg.stCropRect.u32Height);
    printf("stFrameRate.bEnable: %d\n", pstParam->stFrameRate.bEnable);
    printf("           .s32SrcFrmRateNum: %d\n", pstParam->stFrameRate.s32SrcFrmRateNum);
    printf("           .s32SrcFrmRateDen: %d\n", pstParam->stFrameRate.s32SrcFrmRateDen);
    printf("           .s32DstFrmRateNum: %d\n", pstParam->stFrameRate.s32DstFrmRateNum);
    printf("           .s32DstFrmRateDen: %d\n", pstParam->stFrameRate.s32DstFrmRateDen);
    printf("************************************************************\n");
}

static void dump_venc_rc_param(VENC_RC_PARAM_S *pstRcParam, RK_CODEC_ID_E enType) {
    printf("%s:\n", __func__);
    printf("s32FirstFrameStartQp: %d\n", pstRcParam->s32FirstFrameStartQp);
    switch (enType) {
    case RK_VIDEO_ID_AVC:
        printf("stParamH264.u32StepQp: %d\n", pstRcParam->stParamH264.u32StepQp);
        printf("           .u32MaxQp: %d\n", pstRcParam->stParamH264.u32MaxQp);
        printf("           .u32MinQp: %d\n", pstRcParam->stParamH264.u32MinQp);
        printf("           .u32MaxIQp: %d\n", pstRcParam->stParamH264.u32MaxIQp);
        printf("           .u32MinIQp: %d\n", pstRcParam->stParamH264.u32MinIQp);
        printf("           .s32DeltIpQp: %d\n", pstRcParam->stParamH264.s32DeltIpQp);
        printf("           .s32MaxReEncodeTimes: %d\n", pstRcParam->stParamH264.s32MaxReEncodeTimes);
        printf("           .u32FrmMinQp: %d\n", pstRcParam->stParamH264.u32FrmMinQp);
        printf("           .u32FrmMinIQp: %d\n", pstRcParam->stParamH264.u32FrmMinIQp);
        break;
    case RK_VIDEO_ID_HEVC:
        printf("stParamH265.u32StepQp: %d\n", pstRcParam->stParamH265.u32StepQp);
        printf("           .u32MaxQp: %d\n", pstRcParam->stParamH265.u32MaxQp);
        printf("           .u32MinQp: %d\n", pstRcParam->stParamH265.u32MinQp);
        printf("           .u32MaxIQp: %d\n", pstRcParam->stParamH265.u32MaxIQp);
        printf("           .u32MinIQp: %d\n", pstRcParam->stParamH265.u32MinIQp);
        printf("           .s32DeltIpQp: %d\n", pstRcParam->stParamH265.s32DeltIpQp);
        printf("           .s32MaxReEncodeTimes: %d\n", pstRcParam->stParamH265.s32MaxReEncodeTimes);
        printf("           .u32FrmMinQp: %d\n", pstRcParam->stParamH265.u32FrmMinQp);
        printf("           .u32FrmMinIQp: %d\n", pstRcParam->stParamH265.u32FrmMinIQp);
        break;
    case RK_VIDEO_ID_MJPEG:
        printf("stParamMjpeg.u32Qfactor: %d\n", pstRcParam->stParamMjpeg.u32Qfactor);
        printf("stParamMjpeg.u32MaxQfactor: %d\n", pstRcParam->stParamMjpeg.u32MaxQfactor);
        printf("stParamMjpeg.u32MinQfactor: %d\n", pstRcParam->stParamMjpeg.u32MinQfactor);
        break;
    }
    printf("************************************************************\n");
}

void dump_venc_roi_attr(VENC_ROI_ATTR_S *pstRoiAttr, RK_S32 i) {
    printf("%s, index %d:\n", __func__, i);
    printf("bEnable: %d\n", pstRoiAttr->bEnable);
    printf("stRect.s32X: %d\n", pstRoiAttr->stRect.s32X);
    printf("      .s32Y: %d\n", pstRoiAttr->stRect.s32Y);
    printf("      .u32Width: %d\n", pstRoiAttr->stRect.u32Width);
    printf("      .u32Height: %d\n", pstRoiAttr->stRect.u32Height);
    printf("bIntra: %d\n", pstRoiAttr->bIntra);
    printf("bAbsQp: %d\n", pstRoiAttr->bAbsQp);
    printf("u32Index: %d\n", pstRoiAttr->u32Index);
    printf("s32Qp: %d\n", pstRoiAttr->s32Qp);
    printf("************************************************************\n");
}

void* venc_get_stream(void *pArgs) {
    TEST_VENC_CTX_S *pstCtx     = reinterpret_cast<TEST_VENC_CTX_S *>(pArgs);
    void            *pData      = RK_NULL;
    RK_S32           s32Ret     = RK_SUCCESS;
    FILE            *fp         = RK_NULL;
    char             name[256]  = {0};
    RK_U32           u32Ch      = pstCtx->u32ChnIndex;
    RK_S32           s32StreamCnt = 0;
    VENC_STREAM_S    stFrame;

    VENC_CHN_ATTR_S stAttr;
    memset(&stAttr, 0, sizeof(stAttr));
    RK_MPI_VENC_GetChnAttr(u32Ch, &stAttr);
    dump_venc_chn_attr(&stAttr);
    TEST_VENC_SET_BitRate(&stAttr.stRcAttr, 1024, 1024, 1024);
    RK_MPI_VENC_SetChnAttr(u32Ch, &stAttr);

    VENC_CHN_PARAM_S stParam;
    memset(&stParam, 0, sizeof(stParam));
    RK_MPI_VENC_GetChnParam(u32Ch, &stParam);
    dump_venc_chn_param(&stParam);
    RK_MPI_VENC_SetChnParam(u32Ch, &stParam);

    VENC_RC_PARAM_S stRcParam;
    memset(&stRcParam, 0, sizeof(stRcParam));
    RK_MPI_VENC_GetRcParam(u32Ch, &stRcParam);
    dump_venc_rc_param(&stRcParam, stAttr.stVencAttr.enType);
    RK_MPI_VENC_SetRcParam(u32Ch, &stRcParam);

    for (RK_S32 i = 0; i < VENC_MAX_ROI_NUM; i++) {
        VENC_ROI_ATTR_S stRoiAttr;
        memset(&stRoiAttr, 0, sizeof(stRoiAttr));
        RK_MPI_VENC_GetRoiAttr(u32Ch, i, &stRoiAttr);
        dump_venc_roi_attr(&stRoiAttr, i);
        if (stRoiAttr.bEnable)
            RK_MPI_VENC_SetRoiAttr(u32Ch, &stRoiAttr);
    }

    if (pstCtx->bInsertUserData) {
        RK_U32 user_len;
        RK_U8 user_data[] = "this is rockchip user data";
        user_len = sizeof(user_data);
        RK_MPI_VENC_InsertUserData(u32Ch, user_data, user_len);
        RK_CHAR user_data2[1024];
        snprintf(user_data2, sizeof(user_data2), "rockchip continuous user data:%d", 0);
        user_len = strlen(user_data2);
        RK_MPI_VENC_InsertUserData(u32Ch, reinterpret_cast<RK_U8 *>(user_data2), user_len);
    }

    // VENC_SUPERFRAME_CFG_S stSuperFrmParam;
    // memset(&stSuperFrmParam, 0, sizeof(stSuperFrmParam));
    // RK_MPI_VENC_GetSuperFrameStrategy(u32Ch, &stSuperFrmParam);
    // RK_MPI_VENC_SetSuperFrameStrategy(u32Ch, &stSuperFrmParam);

    // VENC_FRAMELOST_S stFrmLostParam;
    // memset(&stFrmLostParam, 0, sizeof(stFrmLostParam));
    // RK_MPI_VENC_GetFrameLostStrategy(u32Ch, &stFrmLostParam);
    // RK_MPI_VENC_SetFrameLostStrategy(u32Ch, &stFrmLostParam);

    // VENC_HIERARCHICAL_QP_S stHierarchicalQp;
    // memset(&stHierarchicalQp, 0, sizeof(stHierarchicalQp));
    // RK_MPI_VENC_GetHierarchicalQp(u32Ch, &stHierarchicalQp);
    // printf("bHierarchicalQpEn: %d\n", stHierarchicalQp.bHierarchicalQpEn);
    // for (int i = 0; i < 4; i++)
    //     printf("s32HierarchicalQpDelta[%d]: %d\n", i, stHierarchicalQp.s32HierarchicalQpDelta[i]);
    // for (int i = 0; i < 4; i++)
    //     printf("s32HierarchicalFrameNum[%d]: %d\n", i, stHierarchicalQp.s32HierarchicalFrameNum[i]);
    // RK_MPI_VENC_SetHierarchicalQp(u32Ch, &stHierarchicalQp);

    // ROTATION_E enRotation = ROTATION_0;
    // RK_MPI_VENC_GetChnRotation(u32Ch, &enRotation);
    // printf("enRotation: %d\n", enRotation);
    // RK_MPI_VENC_SetChnRotation(u32Ch, enRotation);

    if (pstCtx->dstFilePath != RK_NULL) {
        mkdir(pstCtx->dstFilePath, 0777);

        snprintf(name, sizeof(name), "%s/test_%d.bin",
            pstCtx->dstFilePath, pstCtx->u32ChnIndex);

        fp = fopen(name, "wb");
        if (fp == RK_NULL) {
            RK_LOGE("chn %d can't open file %s in get picture thread!\n", u32Ch, name);
            return RK_NULL;
        }
    }
    stFrame.pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));

    while (!pstCtx->threadExit) {
        // test for request idr
        // if (s32StreamCnt % 10 == 0)
        //     RK_MPI_VENC_RequestIDR(u32Ch, RK_FALSE);

        s32Ret = RK_MPI_VENC_GetStream(u32Ch, &stFrame, -1);
        if (s32Ret >= 0) {
            s32StreamCnt++;
            RK_LOGI("get chn %d stream %d", u32Ch, s32StreamCnt);
            if (pstCtx->dstFilePath != RK_NULL) {
                pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
                fwrite(pData, 1, stFrame.pstPack->u32Len, fp);
                fflush(fp);
            }
            RK_MPI_VENC_ReleaseStream(u32Ch, &stFrame);
            if (stFrame.pstPack->bStreamEnd == RK_TRUE) {
                RK_LOGI("chn %d reach EOS stream", u32Ch);
                break;
            }
        } else {
             if (pstCtx->threadExit) {
                break;
             }

             usleep(1000llu);
        }
    }

    if (stFrame.pstPack)
        free(stFrame.pstPack);

    if (fp)
        fclose(fp);

    return RK_NULL;
}

static RK_S32 check_options(const TEST_VENC_CTX_S *ctx) {
    return RK_SUCCESS;
}

static const char *const usages[] = {
    "./rk_mpi_venc_test [-o OUTPUT_PATH]",
    NULL,
};

static void mpi_venc_test_show_options(const TEST_VENC_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("output file name       : %s\n", ctx->dstFilePath);
    RK_PRINT("channel index          : %d\n", ctx->u32ChnIndex);

    return;
}

int main(int argc, const char **argv) {
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_VENC_CTX_S ctx;
    memset(&ctx, 0, sizeof(TEST_VENC_CTX_S));

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('o', "output", &(ctx.dstFilePath),
                   "the directory of encoder output", NULL, 0, 0),
        OPT_INTEGER('\0', "channel_index", &(ctx.u32ChnIndex),
                     "venc channel index. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "insert_user_data", &(ctx.bInsertUserData),
                    "insert user data flag(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_venc_test_show_options(&ctx);

    if (check_options(&ctx)) {
        argparse_usage(&argparse);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    venc_get_stream(&ctx);

    s32Ret = RK_MPI_SYS_Exit();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    RK_LOGE("test running success!");
    return RK_SUCCESS;
__FAILED:
    RK_MPI_SYS_Exit();
    RK_LOGE("test running failed!");
    return s32Ret;
}
