/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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

#include <stdio.h>
#include <sys/poll.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>

#include "rk_defines.h"
#include "rk_debug.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vpss.h"
#include "rk_mpi_vo.h"
#include "rk_mpi_rgn.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_ivs.h"

#include "rk_common.h"
#include "rk_comm_rgn.h"
#include "rk_comm_vi.h"
#include "rk_comm_vo.h"
#include "rk_comm_ivs.h"

#include "test_common.h"
#include "test_comm_utils.h"
#include "test_comm_argparse.h"
#include "test_comm_bmp.h"

#define TEST_VENC_MAX 2
#define TEST_WITH_FD 0
#define TEST_WITH_FD_SWITCH 0

#ifdef DBG_MOD_ID
#undef DBG_MOD_ID
#endif
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_VI)

// for 356x vo
#define RK356X_VO_DEV_HD0 0
#define RK356X_VO_DEV_HD1 1
#define RK356X_VOP_LAYER_CLUSTER_0 0
#define RK356X_VOP_LAYER_CLUSTER_1 2
#define RK356X_VOP_LAYER_ESMART_0 4
#define RK356X_VOP_LAYER_ESMART_1 5
#define RK356X_VOP_LAYER_SMART_0 6
#define RK356X_VOP_LAYER_SMART_1 7

#define TEST_VI_SENSOR_NUM 6

#define COMBO_START_CHN 4

#define THUMBNAIL_VI_CHN 1
#define THUMBNAIL_VENC_CHN 5
#define THUMBNAIL_WIDTH 640
#define THUMBNAIL_HEIGHT 360

#define RKISP_DEV  "/proc/rkisp-vir0"
typedef struct _SENSOR_INFO {
    int width;
    int height;
    int fps;
    int type;   //2MP,3MP,4MP,5MP
} SENSOR_INFO;

typedef struct _rkTestVencCfg {
    RK_BOOL bOutDebugCfg;
    VENC_CHN_ATTR_S stAttr;
    RK_CHAR dstFilePath[128];
    RK_CHAR dstFileName[128];
    RK_S32 s32ChnId;
    FILE *fp;
    RK_S32 selectFd;
} TEST_VENC_CFG;

typedef struct rkVPSS_CFG_S {
    RK_BOOL bOutDebugCfg;
    FILE *fp;
    RK_CHAR dstFilePath[128];
    RK_CHAR dstFileName[128];
    RK_S32 s32DevId;
    RK_S32 s32ChnId;
    RK_U32 u32VpssChnCnt;
    VPSS_GRP_ATTR_S stGrpVpssAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
} VPSS_CFG_S;

typedef struct rkRGN_CFG_S {
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stRgnChnAttr;
} RGN_CFG_S;

typedef enum sensorResoType_E {
    _1MEGA = 0,
    _2MEGA,
    _3MEGA,
    _4MEGA,
    _5MEGA,
    _8MEGA,
    _4K,
} SENSOR_RESO_TYPE_E;

typedef enum rkTestVIMODE_E {
    TEST_VI_MODE_VI_ONLY = 0,
    TEST_VI_MODE_BIND_VENC = 1,
    TEST_VI_MODE_BIND_VENC_MULTI = 2,
    TEST_VI_MODE_BIND_VPSS_BIND_VENC = 3,
    TEST_VI_MODE_BIND_VO = 4,
    TEST_VI_MODE_MULTI_VI = 5,   // multiple sensor devices as vi input
    TEST_VI_GET_SEND_VENC = 6,
    TEST_VI_GET_SEND_VO = 7,
    TEST_VI_MODE_BIND_VPSS_BIND_VO = 8,
    TEST_VI_MODE_BIND_VPSS_SEND_VO = 9,
    TEST_VI_CHANGE_RESOLUTION = 10,
    TEST_VI_BIND_VENC_CHANGE_RESOLUTION = 11,
    TEST_VI_BIND_VENC_WITH_THUMBNAIL = 12,
    TEST_VI_MODE_MULTI_CHN = 13,
    TEST_VI_EPTZ_VI_ONLY = 14,
    TEST_VI_BUTT,
} TEST_VI_MODE_E;

struct combo_th {
    pthread_t th;
    RK_BOOL run;
    RK_S32 chn;
};

typedef struct rkResolution_st
{
    SIZE_S size;
    const char  *resStr;
}RK_RESOLUTION_ST;

typedef struct _rkMpiVICtx {
    RK_S32 width;
    RK_S32 height;
    RK_S32 devId;
    RK_S32 pipeId;
    RK_S32 channelId;
    RK_S32 vencChId;
    RK_S32 loopCountSet;
    RK_S32 selectFd;
    RK_BOOL bFreeze;
    RK_BOOL bEnRgn;
    RK_BOOL bEnOverlay;
    RK_S32 s32RgnCnt;
    RK_S32 rgnType;
    RK_BOOL bMirror;
    RK_BOOL bFlip;
    RK_BOOL bAttachPool;
    MB_POOL attachPool;
    RK_BOOL bEptz;          // enable eptz function
    RK_BOOL bUseExt;        // use ext api with pipe
    RK_BOOL bRgnOnPipe;     // rgn effect on pipe

    COMPRESS_MODE_E enCompressMode;
    VI_DEV_STATUS_S stDevStatus;
    VI_DEV_ATTR_S stDevAttr;
    VI_DEV_BIND_PIPE_S stBindPipe;
    VI_CHN_ATTR_S stChnAttr;

    VI_SAVE_FILE_INFO_S stDebugFile;
    VIDEO_FRAME_INFO_S stViFrame;
    VI_CHN_STATUS_S stChnStatus;
    TEST_VI_MODE_E enMode;
    const char *aEntityName;
    VI_CHN_BUF_WRAP_S stChnWrap;
    // for vi
    RGN_CFG_S stViRgn;
    // for venc
    TEST_VENC_CFG stVencCfg[TEST_VENC_MAX];
    VENC_STREAM_S stFrame[TEST_VENC_MAX];
    VPSS_CFG_S stVpssCfg;
    // for vo
    VO_LAYER s32VoLayer;
    VO_DEV s32VoDev;

    RK_U32 u32DstCodec;
    RK_BOOL bCombo;
    RK_S32  s32Snap;

    RK_BOOL bRefBufShare;

    RK_U32 u32DeBreath;
    RK_U32 u32BitRateKb;
    RK_U32 u32BitRateKbMin;
    RK_U32 u32BitRateKbMax;
    RK_U32 u32DelayMsGet;
    RK_U32 u32Ivs;
    RK_BOOL bIvsDebug;
    RK_BOOL bSvc;
    RK_U32 u32GopSize;
    RK_U32 maxWidth;
    RK_U32 maxHeight;
    RK_U32 u32ChangeRes;
    RK_BOOL bGetStream;
    RK_BOOL bSecondThr;

    pthread_t thumbnail_th;
    RK_BOOL thumbnail_exit;
} TEST_VI_CTX_S;

static struct combo_th g_combo_th[TEST_VENC_MAX];
static SENSOR_INFO g_sensorInfo;
static RK_BOOL bquit = RK_FALSE;
static void sigterm_handler(int sig) {
    bquit = RK_TRUE;
}

static RK_S32 create_ivs(TEST_VI_CTX_S *ctx) {
    IVS_CHN_ATTR_S attr;
    memset(&attr, 0, sizeof(attr));
    attr.enMode = IVS_MODE_MD_OD;
    attr.u32PicWidth = ctx->width;
    attr.u32PicHeight = ctx->height;
    attr.enPixelFormat = ctx->stChnAttr.enPixelFormat;
    attr.s32Gop = 30;
    attr.bSmearEnable = RK_FALSE;
    attr.bWeightpEnable = RK_FALSE;
    attr.bMDEnable = RK_TRUE;
    attr.s32MDInterval = 1;
    attr.bMDNightMode = RK_FALSE;
    attr.u32MDSensibility = ctx->u32Ivs;
    attr.bODEnable = RK_TRUE;
    attr.s32ODInterval = 1;
    attr.s32ODPercent = 7;

    return RK_MPI_IVS_CreateChn(0, &attr);
}

static RK_S32 destroy_ivs(RK_VOID) {
    RK_MPI_IVS_DestroyChn(0);
    return RK_SUCCESS;
}

#ifdef HAVE_VPSS
static RK_S32 create_vpss(VPSS_CFG_S *pstVpssCfg, RK_S32 s32Grp, RK_S32 s32OutChnNum) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = { VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3 };
    VPSS_CROP_INFO_S stCropInfo;

    s32Ret = RK_MPI_VPSS_CreateGrp(s32Grp, &pstVpssCfg->stGrpVpssAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    for (RK_S32 i = 0; i < s32OutChnNum; i++) {
        s32Ret = RK_MPI_VPSS_SetChnAttr(s32Grp, VpssChn[i], &pstVpssCfg->stVpssChnAttr[i]);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VPSS_EnableChn(s32Grp, VpssChn[i]);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VPSS_EnableBackupFrame(s32Grp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RK_MPI_VPSS_StartGrp(s32Grp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return  RK_SUCCESS;
}

static RK_S32 destory_vpss(RK_S32 s32Grp, RK_S32 s32OutChnNum) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = { VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3 };

    s32Ret = RK_MPI_VPSS_StopGrp(s32Grp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    for (RK_S32 i = 0; i < s32OutChnNum; i++) {
        s32Ret = RK_MPI_VPSS_DisableChn(s32Grp, VpssChn[i]);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VPSS_DisableBackupFrame(s32Grp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RK_MPI_VPSS_DestroyGrp(s32Grp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return  RK_SUCCESS;
}
#endif

static RK_S32 create_overlay(TEST_VI_CTX_S *ctx, RK_U32 u32DstCount) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stDestChn[TEST_VENC_MAX];
    RGN_CANVAS_INFO_S stCanvasInfo;
    RGN_HANDLE RgnHandle = 0;

    ctx->stViRgn.stRgnAttr.enType = OVERLAY_RGN;
    ctx->stViRgn.stRgnAttr.unAttr.stOverlay.u32ClutNum = 0;
    ctx->stViRgn.stRgnAttr.unAttr.stOverlay.stSize.u32Width = 512;
    ctx->stViRgn.stRgnAttr.unAttr.stOverlay.stSize.u32Height = 512;
    ctx->stViRgn.stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_ARGB8888;
    s32Ret = RK_MPI_RGN_Create(RgnHandle, &ctx->stViRgn.stRgnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", RgnHandle, s32Ret);
        RK_MPI_RGN_Destroy(RgnHandle);
        return RK_FAILURE;
    }
    RK_LOGI("The handle: %d, create success!", RgnHandle);


    for (RK_S32 i = 0; i < u32DstCount; i++) {
        stDestChn[i].enModId   = RK_ID_VENC;
        stDestChn[i].s32DevId  = 0;
        stDestChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;
        ctx->stViRgn.stRgnChnAttr.bShow = RK_TRUE;
        ctx->stViRgn.stRgnChnAttr.enType = OVERLAY_RGN;
        ctx->stViRgn.stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
        ctx->stViRgn.stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
        ctx->stViRgn.stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
        ctx->stViRgn.stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
        ctx->stViRgn.stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = i;
        ctx->stViRgn.stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bEnable = RK_FALSE;
        ctx->stViRgn.stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = RK_FALSE;
        s32Ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stDestChn[i], &ctx->stViRgn.stRgnChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    memset(&stCanvasInfo, 0, sizeof(RGN_CANVAS_INFO_S));

    s32Ret = RK_MPI_RGN_GetCanvasInfo(RgnHandle, &stCanvasInfo);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_RGN_GetCanvasInfo failed with %#x!", s32Ret);
        return RK_FAILURE;
    }
    memset(reinterpret_cast<void *>(stCanvasInfo.u64VirAddr), 0xff,
            stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight * 4);
    s32Ret = RK_MPI_RGN_UpdateCanvas(RgnHandle);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_RGN_UpdateCanvas failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

static RK_S32 resize_overlay(TEST_VI_CTX_S *ctx, RK_U32 u32DstCount, SIZE_S size) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stDestChn[TEST_VENC_MAX];
    RGN_CANVAS_INFO_S stCanvasInfo;
    RGN_ATTR_S stRgnAttr;
    RGN_HANDLE RgnHandle = 0;

    for (RK_S32 i = 0; i < u32DstCount; i++) {
        stDestChn[i].enModId   = RK_ID_VENC;
        stDestChn[i].s32DevId  = 0;
        stDestChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;
        s32Ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stDestChn[i]);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    s32Ret = RK_MPI_RGN_GetAttr(RgnHandle, &stRgnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_GetAttr (%d) failed with %#x!", RgnHandle, s32Ret);
        return RK_FAILURE;
    }

    stRgnAttr.unAttr.stOverlay.stSize.u32Width  = size.u32Width;
    stRgnAttr.unAttr.stOverlay.stSize.u32Height = size.u32Height;
    s32Ret = RK_MPI_RGN_SetAttr(RgnHandle, &stRgnAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_SetAttr (%d) failed with %#x!", RgnHandle, s32Ret);
        return RK_FAILURE;
    }

    for (RK_S32 i = 0; i < u32DstCount; i++) {
        stDestChn[i].enModId   = RK_ID_VENC;
        stDestChn[i].s32DevId  = 0;
        stDestChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;
        s32Ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stDestChn[i], &ctx->stViRgn.stRgnChnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    memset(&stCanvasInfo, 0, sizeof(RGN_CANVAS_INFO_S));
    s32Ret = RK_MPI_RGN_GetCanvasInfo(RgnHandle, &stCanvasInfo);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_RGN_GetCanvasInfo failed with %#x!", s32Ret);
        return RK_FAILURE;
    }
    memset(reinterpret_cast<void *>(stCanvasInfo.u64VirAddr), 0xff,
            stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight * 4);
    s32Ret = RK_MPI_RGN_UpdateCanvas(RgnHandle);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_RGN_UpdateCanvas failed with %#x!", s32Ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

static RK_S32 destroy_overlay(TEST_VI_CTX_S *ctx, RK_U32 u32DstCount) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stDestChn[TEST_VENC_MAX];
    RGN_HANDLE RgnHandle = 0;

    for (RK_S32 i = 0; i < u32DstCount; i++) {
        stDestChn[i].enModId   = RK_ID_VENC;
        stDestChn[i].s32DevId  = 0;
        stDestChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;
        s32Ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stDestChn[i]);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }

    s32Ret = RK_MPI_RGN_Destroy(RgnHandle);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_RGN_Destroy [%d] failed with %#x", RgnHandle, s32Ret);
    }

    return RK_SUCCESS;
}

static RK_S32 create_venc(TEST_VI_CTX_S *ctx, RK_U32 u32Ch) {
    VENC_RECV_PIC_PARAM_S stRecvParam;
    VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
    VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;

    memset(&stVencChnBufWrap, 0, sizeof(stVencChnBufWrap));
    stVencChnBufWrap.bEnable = ctx->stChnWrap.bEnable;
    stVencChnBufWrap.u32BufLine = ctx->stChnWrap.u32BufLine;

    memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
    stVencChnRefBufShare.bEnable = ctx->bRefBufShare;

    stRecvParam.s32RecvPicNum = ctx->loopCountSet;
    RK_MPI_VENC_CreateChn(ctx->stVencCfg[u32Ch].s32ChnId, &ctx->stVencCfg[u32Ch].stAttr);
    RK_MPI_VENC_SetChnBufWrapAttr(ctx->stVencCfg[u32Ch].s32ChnId, &stVencChnBufWrap);
    RK_MPI_VENC_SetChnRefBufShareAttr(ctx->stVencCfg[u32Ch].s32ChnId, &stVencChnRefBufShare);

    if (ctx->u32DeBreath > 0) {
        VENC_DEBREATHEFFECT_S stDeBreathEffect;
        memset(&stDeBreathEffect, 0, sizeof(stDeBreathEffect));
        stDeBreathEffect.bEnable = RK_TRUE;
        stDeBreathEffect.s32Strength1 = ctx->u32DeBreath;
        RK_MPI_VENC_SetDeBreathEffect(u32Ch, &stDeBreathEffect);
    }

    if (ctx->bSvc)
        RK_MPI_VENC_EnableSvc(u32Ch, ctx->bSvc);

    RK_MPI_VENC_StartRecvFrame(ctx->stVencCfg[u32Ch].s32ChnId, &stRecvParam);

    return RK_SUCCESS;
}

static RK_S32 create_venc_thumbnail(TEST_VI_CTX_S *ctx) {
    VENC_RECV_PIC_PARAM_S stRecvParam;
    VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
    VENC_CHN_ATTR_S stAttr;

    memset(&stAttr, 0, sizeof(stAttr));
    stAttr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
    stAttr.stVencAttr.enPixelFormat = ctx->stChnAttr.enPixelFormat;
    stAttr.stVencAttr.u32MaxPicWidth = THUMBNAIL_WIDTH;
    stAttr.stVencAttr.u32MaxPicHeight = THUMBNAIL_HEIGHT;
    stAttr.stVencAttr.u32PicWidth = THUMBNAIL_WIDTH;
    stAttr.stVencAttr.u32PicHeight = THUMBNAIL_HEIGHT;
    stAttr.stVencAttr.u32VirWidth = THUMBNAIL_WIDTH;
    stAttr.stVencAttr.u32VirHeight = THUMBNAIL_HEIGHT;
    stAttr.stVencAttr.u32StreamBufCnt = 5;
    stAttr.stVencAttr.u32BufSize = THUMBNAIL_WIDTH * THUMBNAIL_HEIGHT * 3 / 2;

    memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
    stVencChnRefBufShare.bEnable = ctx->bRefBufShare;

    RK_MPI_VENC_CreateChn(THUMBNAIL_VENC_CHN, &stAttr);
    RK_MPI_VENC_SetChnRefBufShareAttr(THUMBNAIL_VENC_CHN, &stVencChnRefBufShare);
    RK_MPI_VENC_EnableThumbnail(THUMBNAIL_VENC_CHN);

    stRecvParam.s32RecvPicNum = -1;
    RK_MPI_VENC_StartRecvFrame(THUMBNAIL_VENC_CHN, &stRecvParam);

    return RK_SUCCESS;
}

static void destroy_venc_thumbnail(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret;
    s32Ret = RK_MPI_VENC_StopRecvFrame(THUMBNAIL_VENC_CHN);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("stop venc thumbnail fail %x", s32Ret);
    }
    s32Ret = RK_MPI_VENC_DestroyChn(THUMBNAIL_VENC_CHN);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
    }
}

static RK_S32 open_venc_file(TEST_VI_CTX_S *ctx, RK_U32 u32DstCount) {
    RK_U32 i;
    for (i = 0; i < u32DstCount; i++) {
        if (ctx->stVencCfg[i].bOutDebugCfg) {
            char name[256] = {0};
            memcpy(&ctx->stVencCfg[i].dstFilePath, "data", strlen("data"));
            snprintf(ctx->stVencCfg[i].dstFileName, sizeof(ctx->stVencCfg[i].dstFileName),
                   "venc_%d.bin", ctx->vencChId + i);
            snprintf(name, sizeof(name), "/%s/%s",
                     ctx->stVencCfg[i].dstFilePath, ctx->stVencCfg[i].dstFileName);
            ctx->stVencCfg[i].fp = fopen(name, "wb");
            if (ctx->stVencCfg[i].fp == RK_NULL) {
                RK_LOGE("chn %d can't open file %s in get picture thread!\n", i, name);
                return RK_FAILURE;
            }
        }
    }
    return RK_SUCCESS;
}

void init_venc_cfg(TEST_VI_CTX_S *ctx, RK_U32 u32Ch, RK_CODEC_ID_E enType) {
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.enType = enType;
    ctx->stVencCfg[u32Ch].s32ChnId = ctx->vencChId + u32Ch;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.enPixelFormat = ctx->stChnAttr.enPixelFormat;
    if (ctx->bSvc) {
        ctx->stVencCfg[u32Ch].stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
        ctx->stVencCfg[u32Ch].stAttr.stRcAttr.stH264Vbr.u32BitRate = ctx->u32BitRateKb;
        ctx->stVencCfg[u32Ch].stAttr.stRcAttr.stH264Vbr.u32MinBitRate = ctx->u32BitRateKbMin;
        ctx->stVencCfg[u32Ch].stAttr.stRcAttr.stH264Vbr.u32MaxBitRate = ctx->u32BitRateKbMax;
    } else {
        ctx->stVencCfg[u32Ch].stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
        ctx->stVencCfg[u32Ch].stAttr.stRcAttr.stH264Cbr.u32BitRate = ctx->u32BitRateKb;
    }
    ctx->stVencCfg[u32Ch].stAttr.stRcAttr.stH264Cbr.u32Gop = ctx->u32GopSize;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32MaxPicWidth = ctx->maxWidth;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32MaxPicHeight = ctx->maxHeight;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32PicWidth = ctx->width;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32PicHeight = ctx->height;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32VirWidth = ctx->width;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32VirHeight = ctx->height;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32StreamBufCnt = 5;
    ctx->stVencCfg[u32Ch].stAttr.stVencAttr.u32BufSize = ctx->maxWidth * ctx->maxHeight / 2;
    ctx->stVencCfg[u32Ch].stAttr.stGopAttr.u32MaxLtrCount = 1;
}

static void *venc_jpeg_get_packet(void *arg) {
    RK_S32 s32Ret = RK_SUCCESS;
    VENC_STREAM_S stFrame;
    memset(&stFrame, 0, sizeof(VENC_STREAM_S));
    stFrame.pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));
    struct combo_th *combo_th = (struct combo_th*)arg;
    RK_S32 s32ChnId = combo_th->chn;
    RK_S32 loopCount = 0;

    while (combo_th->run) {
        s32Ret = RK_MPI_VENC_GetStream(s32ChnId, &stFrame, 1000);
        if (s32Ret == RK_SUCCESS) {
            if (1) {
                char filename[128];
                FILE *fp;
                snprintf(filename, sizeof(filename), "/data/venc_ch%d_%lld.jpg", s32ChnId, stFrame.pstPack->u64PTS);
                fp = fopen(filename, "wb");
                if (fp) {
                    RK_VOID *pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
                    fwrite(pData, 1, stFrame.pstPack->u32Len, fp);
                    fflush(fp);
                    fclose(fp);
                }
            }
            RK_U64 nowUs = TEST_COMM_GetNowUs();

            RK_LOGD("chn:%d, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n", s32ChnId, loopCount,
                    stFrame.u32Seq, stFrame.pstPack->u32Len,
                    stFrame.pstPack->u64PTS,
                    nowUs - stFrame.pstPack->u64PTS);
            s32Ret = RK_MPI_VENC_ReleaseStream(s32ChnId, &stFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
            }
            loopCount++;
        } else {
            // RK_LOGE("RK_MPI_VENC_GetChnFrame fail %x", s32Ret);
        }
    }
    pthread_exit(NULL);
}

static RK_S32 create_venc_combo(TEST_VI_CTX_S *ctx, RK_U32 u32Ch, RK_U32 s32ComboChnId) {
    VENC_CHN_ATTR_S stAttr;
    VENC_RECV_PIC_PARAM_S stRecvParam;
    VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
    memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    memset(&stVencChnBufWrap, 0, sizeof(stVencChnBufWrap));

    stAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
    stAttr.stRcAttr.stH264Cbr.u32Gop = 30;
    stAttr.stRcAttr.stH264Cbr.u32BitRate = 1024;

    stAttr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
    stAttr.stVencAttr.enPixelFormat = ctx->stChnAttr.enPixelFormat;
    stAttr.stVencAttr.u32PicWidth = ctx->width;
    stAttr.stVencAttr.u32PicHeight = ctx->height;
    stAttr.stVencAttr.u32VirWidth = ctx->width;
    stAttr.stVencAttr.u32VirHeight = ctx->height;
    stAttr.stVencAttr.u32StreamBufCnt = 1;
    stAttr.stVencAttr.u32BufSize = ctx->width * ctx->height * 3 / 2;

    if (stAttr.stVencAttr.enType == RK_VIDEO_ID_JPEG) {
        stAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
        stAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
        stAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
    }

    RK_MPI_VENC_CreateChn(u32Ch, &stAttr);

    stVencChnBufWrap.bEnable = ctx->stChnWrap.bEnable;
    RK_MPI_VENC_SetChnBufWrapAttr(u32Ch, &stVencChnBufWrap);

    stRecvParam.s32RecvPicNum = ctx->s32Snap;
    RK_MPI_VENC_StartRecvFrame(u32Ch, &stRecvParam);

    VENC_COMBO_ATTR_S stComboAttr;
    memset(&stComboAttr, 0, sizeof(VENC_COMBO_ATTR_S));
    stComboAttr.bEnable = RK_TRUE;
    stComboAttr.s32ChnId = s32ComboChnId;
    RK_MPI_VENC_SetComboAttr(u32Ch, &stComboAttr);

    VENC_JPEG_PARAM_S stJpegParam;
    memset(&stJpegParam, 0, sizeof(stJpegParam));
    stJpegParam.u32Qfactor = 77;
    RK_MPI_VENC_SetJpegParam(u32Ch, &stJpegParam);

    g_combo_th[u32Ch].run = RK_TRUE;
    g_combo_th[u32Ch].chn = u32Ch;
    pthread_create(&g_combo_th[u32Ch].th, NULL, venc_jpeg_get_packet, &g_combo_th[u32Ch]);

    return RK_SUCCESS;
}

static RK_S32 destroy_venc_combo(TEST_VI_CTX_S *ctx, RK_U32 u32Ch) {
    RK_S32 s32Ret = RK_SUCCESS;
    g_combo_th[u32Ch].run = RK_FALSE;
    pthread_join(g_combo_th[u32Ch].th, NULL);
    s32Ret = RK_MPI_VENC_StopRecvFrame(u32Ch);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    RK_LOGE("destroy enc chn:%d", u32Ch);
    s32Ret = RK_MPI_VENC_DestroyChn(u32Ch);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
    }
    return RK_SUCCESS;
}

#ifdef HAVE_VO
static RK_S32 create_vo(TEST_VI_CTX_S *ctx, RK_U32 u32Ch) {
    /* Enable VO */
    VO_PUB_ATTR_S VoPubAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    RK_S32 s32Ret = RK_SUCCESS;
    VO_CHN_ATTR_S stChnAttr;
    VO_LAYER VoLayer = ctx->s32VoLayer;
    VO_DEV VoDev = ctx->s32VoDev;
    RK_MPI_VO_DisableLayer(VoLayer);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_1);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_1);
    RK_MPI_VO_Disable(VoDev);

    memset(&VoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
    memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));

    stLayerAttr.enPixFormat = RK_FMT_RGB888;
    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    stLayerAttr.u32DispFrmRt = 30;
    stLayerAttr.stDispRect.u32Width = 1920;
    stLayerAttr.stDispRect.u32Height = 1080;
    stLayerAttr.stImageSize.u32Width = 1920;
    stLayerAttr.stImageSize.u32Height = 1080;

    s32Ret = RK_MPI_VO_GetPubAttr(VoDev, &VoPubAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    VoPubAttr.enIntfType = VO_INTF_HDMI;
    VoPubAttr.enIntfSync = VO_OUTPUT_1080P60;

    s32Ret = RK_MPI_VO_SetPubAttr(VoDev, &VoPubAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VO_Enable(VoDev);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RK_MPI_VO_SetLayerAttr(VoLayer, &stLayerAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_SetLayerAttr failed,s32Ret:%d\n", s32Ret);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_GRAPHIC);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_BindLayer failed,s32Ret:%d\n", s32Ret);
        return RK_FAILURE;
    }


    s32Ret = RK_MPI_VO_EnableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_EnableLayer failed,s32Ret:%d\n", s32Ret);
        return RK_FAILURE;
    }

    stChnAttr.stRect.s32X = 0;
    stChnAttr.stRect.s32Y = 0;
    stChnAttr.stRect.u32Width = stLayerAttr.stImageSize.u32Width;
    stChnAttr.stRect.u32Height = stLayerAttr.stImageSize.u32Height;
    stChnAttr.u32Priority = 0;
    stChnAttr.u32FgAlpha = 128;
    stChnAttr.u32BgAlpha = 0;

    s32Ret = RK_MPI_VO_SetChnAttr(VoLayer, u32Ch, &stChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("set chn Attr failed,s32Ret:%d\n", s32Ret);
        return RK_FAILURE;
    }
    return s32Ret;
}
#endif

RK_S32 test_vi_poll_event(RK_S32 timeoutMsec, RK_S32 fd) {
    RK_S32 num_fds = 1;
    struct pollfd pollFds[num_fds];
    RK_S32 ret = 0;

    RK_ASSERT(fd > 0);
    memset(pollFds, 0, sizeof(pollFds));
    pollFds[0].fd = fd;
    pollFds[0].events = (POLLPRI | POLLIN | POLLERR | POLLNVAL | POLLHUP);

    ret = poll(pollFds, num_fds, timeoutMsec);

    if (ret > 0 && (pollFds[0].revents & (POLLERR | POLLNVAL | POLLHUP))) {
        RK_LOGE("fd:%d polled error", fd);
        return -1;
    }

    return ret;
}

static RK_S32 load_bmp(const RK_CHAR *filename, BITMAP_S *pstBitmap) {
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    if (TEST_COMM_GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0) {
        RK_LOGE("GetBmpInfo err!\n");
        return RK_FAILURE;
    }

    Surface.enColorFmt = OSD_COLOR_FMT_BGRA8888;

    pstBitmap->pData = malloc(4 * (bmpInfo.bmiHeader.biWidth) * (bmpInfo.bmiHeader.biHeight));

    if (RK_NULL == pstBitmap->pData) {
        RK_LOGE("malloc osd memroy err!");
        return RK_FAILURE;
    }

    TEST_COMM_CreateSurfaceByBitmap(filename, &Surface,  reinterpret_cast<RK_U8 *>(pstBitmap->pData));

    pstBitmap->u32Width = Surface.u16Width;
    pstBitmap->u32Height = Surface.u16Height;
    pstBitmap->enPixelFormat = RK_FMT_BGRA8888/* RK_FMT_ARGB8888 */;

    return RK_SUCCESS;
}

static RK_S32 create_rgn(TEST_VI_CTX_S *ctx) {
    RK_S32 i;
    RK_S32 s32Ret = RK_SUCCESS;
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stRgnChnAttr;
    RGN_HANDLE RgnHandle = 0;
    MPP_CHN_S stMppChn;
    RGN_TYPE_E rgnType = (RGN_TYPE_E)ctx->rgnType;

    stMppChn.enModId = RK_ID_VI;
    stMppChn.s32DevId = ctx->devId;
    // NOTE: rv1106 rgn chn use max chn num as distinguish acting on pipe or chn.
    if (ctx->bRgnOnPipe)
        stMppChn.s32ChnId = VI_MAX_CHN_NUM;
    else
        stMppChn.s32ChnId = ctx->channelId;

    /****************************************
     step 1: create overlay regions
    ****************************************/
    for (i = 0; i < ctx->s32RgnCnt; i++) {
        ctx->stViRgn.stRgnAttr.unAttr.stOverlay.u32ClutNum = 0;
        ctx->stViRgn.stRgnAttr.unAttr.stOverlay.stSize.u32Width = 128;
        ctx->stViRgn.stRgnAttr.unAttr.stOverlay.stSize.u32Height = 128;
        RgnHandle = i;
        s32Ret = RK_MPI_RGN_Create(RgnHandle, &ctx->stViRgn.stRgnAttr);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", RgnHandle, s32Ret);
            RK_MPI_RGN_Destroy(RgnHandle);
            return RK_FAILURE;
        }
        RK_LOGI("The handle: %d, create success!", RgnHandle);
    }

    /*********************************************
     step 2: display overlay regions to vi
    *********************************************/
    for (i = 0; i < ctx->s32RgnCnt; i++) {
        RgnHandle = i;

        memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
        stRgnChnAttr.bShow = RK_TRUE;
        stRgnChnAttr.enType = rgnType;

        BITMAP_S stBitmap;
        switch (rgnType) {
            case COVER_RGN: {
                RGN_CHN_ATTR_S stCoverChnAttr;
                RK_LOGD("cover rgn attach");
                stCoverChnAttr.bShow = RK_TRUE;
                stCoverChnAttr.enType = COVER_RGN;
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = 128 * i;
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 128 * i;
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 128;
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 128;
                stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0xffffff;  // white
                stCoverChnAttr.unChnAttr.stCoverChn.enCoordinate = RGN_ABS_COOR;
                stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = i;

                s32Ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stCoverChnAttr);
                if (RK_SUCCESS != s32Ret) {
                    RK_LOGE("failed with %#x!", s32Ret);
                    goto __EXIT;
                }
                s32Ret = RK_MPI_RGN_GetDisplayAttr(RgnHandle, &stMppChn, &stCoverChnAttr);
                if (RK_SUCCESS != s32Ret) {
                    RK_LOGE("failed with %#x!", s32Ret);
                    goto __EXIT;
                }

                /* test one full resolution cover*/
                if (ctx->s32RgnCnt == 1) {
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = 0;
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 0;
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = ctx->width;
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = ctx->height;
                    stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0xff0000;  // red
                    stCoverChnAttr.unChnAttr.stCoverChn.enCoordinate = RGN_ABS_COOR;
                    stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = 0;
                } else {
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = 128 * i;
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 128 * i;
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 128;
                    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 128;
                    stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0x0000ff;  // blue
                    stCoverChnAttr.unChnAttr.stCoverChn.enCoordinate = RGN_ABS_COOR;
                    stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = i;
                }

                int color = 0xff0000 >> ((i % 3) * 8);  // cover win color from red,green blue to red.
                stCoverChnAttr.unChnAttr.stCoverChn.u32Color = color;

                // change cover channel attribute below.
                s32Ret = RK_MPI_RGN_SetDisplayAttr(RgnHandle, &stMppChn, &stCoverChnAttr);
                if (RK_SUCCESS != s32Ret) {
                    RK_LOGE("failed with %#x!", s32Ret);
                    goto __EXIT;
                }
                RK_LOGI("the cover region %d to <%d, %d, %d, %d>, color<0x%x>",
                        RgnHandle,
                        stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X,
                        stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y,
                        stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width,
                        stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height,
                        stCoverChnAttr.unChnAttr.stCoverChn.u32Color);
            } break;
            case MOSAIC_RGN: {
                RGN_CHN_ATTR_S stMoscaiChnAttr;
                RK_LOGD("mosaic rgn attach");
                stMoscaiChnAttr.bShow = RK_TRUE;
                stMoscaiChnAttr.enType = MOSAIC_RGN;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.s32X = 128 * i;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.s32Y = 128 * i;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.u32Width = 128;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.u32Height = 128;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.enBlkSize = MOSAIC_BLK_SIZE_8;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.u32Layer = i;

                s32Ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stMoscaiChnAttr);
                if (RK_SUCCESS != s32Ret) {
                    RK_LOGE("failed with %#x!", s32Ret);
                    goto __EXIT;
                }
                s32Ret = RK_MPI_RGN_GetDisplayAttr(RgnHandle, &stMppChn, &stMoscaiChnAttr);
                if (RK_SUCCESS != s32Ret) {
                    RK_LOGE("failed with %#x!", s32Ret);
                    goto __EXIT;
                }

                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.s32X = 128 * i;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.s32Y = 128 * i;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.u32Width = 128;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.u32Height = 128;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.enBlkSize = MOSAIC_BLK_SIZE_8;
                stMoscaiChnAttr.unChnAttr.stMosaicChn.u32Layer = i;

                // change mosaic channel attribute below.
                s32Ret = RK_MPI_RGN_SetDisplayAttr(RgnHandle, &stMppChn, &stMoscaiChnAttr);
                if (RK_SUCCESS != s32Ret) {
                    RK_LOGE("failed with %#x!", s32Ret);
                    goto __EXIT;
                }
                RK_LOGI("the mosaic region:%d to <%d, %d, %d, %d>",
                        RgnHandle,
                        stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.s32X,
                        stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.s32Y,
                        stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.u32Width,
                        stMoscaiChnAttr.unChnAttr.stMosaicChn.stRect.u32Height);
            } break;
            default:
                break;
        }
    }

    return RK_SUCCESS;
__EXIT:
    for (i = 0; i < ctx->s32RgnCnt; i++) {
        RgnHandle = i;
        s32Ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
    }
    return RK_FAILURE;
}

static RK_S32 destory_rgn(TEST_VI_CTX_S *ctx) {
    RK_S32 i;
    RK_S32 s32Ret = RK_SUCCESS;
    RGN_HANDLE RgnHandle = 0;
    MPP_CHN_S stMppChn;

    stMppChn.enModId = RK_ID_VI;
    stMppChn.s32DevId = ctx->devId;
    // NOTE: rv1106 rgn chn use max chn num as distinguish acting on pipe or chn.
    if (ctx->bRgnOnPipe)
        stMppChn.s32ChnId = VI_MAX_CHN_NUM;
    else
        stMppChn.s32ChnId = ctx->channelId;

    // detach rgn
    for (RK_S32 i = 0; i < ctx->s32RgnCnt; i++) {
        RgnHandle = i;
        s32Ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }

        RK_LOGI("Detach handle:%d from chn success", RgnHandle);
    }

    //destory rgn
    for (RK_S32 i = 0; i < ctx->s32RgnCnt; i++) {
        RgnHandle = i;
        s32Ret = RK_MPI_RGN_Destroy(RgnHandle);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_Destroy (%d) failed with %#x!", RgnHandle, s32Ret);
        }
        RK_LOGI("Destory handle:%d success", RgnHandle);
    }

    RK_LOGD("vi RK_MPI_RGN_Destroy OK");
    return RK_SUCCESS;
}

static MB_POOL create_pool(TEST_VI_CTX_S *pstCtx) {
    MB_POOL_CONFIG_S stMbPoolCfg;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;
    RK_S32 s32Ret = RK_SUCCESS;

    if (pstCtx->stChnAttr.stIspOpt.stMaxSize.u32Width && \
         pstCtx->stChnAttr.stIspOpt.stMaxSize.u32Height) {
        stPicBufAttr.u32Width = pstCtx->stChnAttr.stIspOpt.stMaxSize.u32Width;
        stPicBufAttr.u32Height = pstCtx->stChnAttr.stIspOpt.stMaxSize.u32Height;
    } else {
        stPicBufAttr.u32Width = pstCtx->width;
        stPicBufAttr.u32Height = pstCtx->height;
    }
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)pstCtx->enCompressMode;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)pstCtx->stChnAttr.enPixelFormat;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return MB_INVALID_POOLID;
    }

    memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
    stMbPoolCfg.u64MBSize = stMbPicCalResult.u32MBSize;
    stMbPoolCfg.u32MBCnt  = 3;
    stMbPoolCfg.enRemapMode = MB_REMAP_MODE_CACHED;
    stMbPoolCfg.bPreAlloc = RK_TRUE;

    return RK_MPI_MB_CreatePool(&stMbPoolCfg);
}

static RK_S32 test_vi_init(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;

    s32Ret = RK_MPI_VI_QueryDevStatus(ctx->devId, &ctx->stDevStatus);
    if (s32Ret == RK_SUCCESS) {
        if (!ctx->stDevStatus.bProbeOk) {
            RK_LOGE("sensor probe fail");
            return RK_FAILURE;
        }
    }

    // 0. get dev config status
    s32Ret = RK_MPI_VI_GetDevAttr(ctx->devId, &ctx->stDevAttr);
    if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
        // 0-1.config dev
        s32Ret = RK_MPI_VI_SetDevAttr(ctx->devId, &ctx->stDevAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevAttr %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_SetDevAttr already");
    }
    // 1.get  dev enable status
    s32Ret = RK_MPI_VI_GetDevIsEnable(ctx->devId);
    if (s32Ret != RK_SUCCESS) {
        // 1-2.enable dev
        s32Ret = RK_MPI_VI_EnableDev(ctx->devId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_EnableDev %x", s32Ret);
            goto __FAILED;
        }
        // 1-3.bind dev/pipe
        ctx->stBindPipe.u32Num = ctx->pipeId;
        ctx->stBindPipe.PipeId[0] = ctx->pipeId;
        s32Ret = RK_MPI_VI_SetDevBindPipe(ctx->devId, &ctx->stBindPipe);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevBindPipe %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_EnableDev already");
    }

    // 2.config channel
    ctx->stChnAttr.stSize.u32Width  = ctx->width;
    ctx->stChnAttr.stSize.u32Height = ctx->height;
    ctx->stChnAttr.enCompressMode   = ctx->enCompressMode;
    ctx->stChnAttr.u32Depth         = 1;
    ctx->stChnAttr.bMirror          = ctx->bMirror;
    ctx->stChnAttr.bFlip            = ctx->bFlip;
    ctx->stChnAttr.stIspOpt.stMaxSize.u32Width  = RK_MAX(ctx->maxWidth, ctx->width);
    ctx->stChnAttr.stIspOpt.stMaxSize.u32Height = RK_MAX(ctx->maxHeight, ctx->height);

    s32Ret = RK_MPI_VI_SetChnAttr(ctx->pipeId, ctx->channelId, &ctx->stChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_SetChnAttr %x", s32Ret);
        goto __FAILED;
    }

    if (ctx->bAttachPool) {
        ctx->attachPool = create_pool(ctx);
        if (ctx->attachPool == MB_INVALID_POOLID) {
            RK_LOGE("create pool failure");
            s32Ret = RK_FAILURE;
            goto __FAILED;
        }

        s32Ret = RK_MPI_VI_AttachMbPool(ctx->pipeId, ctx->channelId, ctx->attachPool);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
    }

    // set wrap mode attr
    VI_CHN_BUF_WRAP_S stViWrap;
    memset(&stViWrap, 0, sizeof(VI_CHN_BUF_WRAP_S));
    if (ctx->stChnWrap.bEnable) {
        if (ctx->stChnWrap.u32BufLine < 128 || ctx->stChnWrap.u32BufLine > ctx->height) {
            RK_LOGE("wrap mode buffer line must between [128, H]");
            goto __FAILED;
        }
        RK_U32                maxW = RK_MAX(ctx->width, ctx->maxWidth);
        stViWrap.bEnable           = ctx->stChnWrap.bEnable;
        stViWrap.u32BufLine        = ctx->stChnWrap.u32BufLine;
        stViWrap.u32WrapBufferSize = stViWrap.u32BufLine * maxW * 3 / 2;  // nv12 (w * wrapLine *3 / 2)
        RK_LOGD("set channel wrap line: %d, wrapBuffSize = %d", ctx->stChnWrap.u32BufLine, stViWrap.u32WrapBufferSize);
        RK_MPI_VI_SetChnWrapBufAttr(ctx->pipeId, ctx->channelId, &stViWrap);
    }

    ctx->stViRgn.stRgnAttr.enType = (RGN_TYPE_E)ctx->rgnType;
    RK_LOGD("RGN Type = %d", ctx->stViRgn.stRgnAttr.enType);
    ctx->stViRgn.stRgnChnAttr.bShow = RK_TRUE;

    // open fd before enable chn will be better
#if TEST_WITH_FD
     ctx->selectFd = RK_MPI_VI_GetChnFd(ctx->pipeId, ctx->channelId);
     RK_LOGE("ctx->pipeId=%d, ctx->channelId=%d, ctx->selectFd:%d ", ctx->pipeId, ctx->channelId, ctx->selectFd);
#endif

    if (ctx->bEnRgn) {
        s32Ret = create_rgn(ctx);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("Error:create rgn failed!");
            goto __FAILED;
        }
    }

    // 3.enable channel
    s32Ret = RK_MPI_VI_EnableChn(ctx->pipeId, ctx->channelId);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_EnableChn %x", s32Ret);
        goto __FAILED;
    }

    // 4.save debug file
    if (ctx->stDebugFile.bCfg) {
        s32Ret = RK_MPI_VI_ChnSaveFile(ctx->pipeId, ctx->channelId, &ctx->stDebugFile);
        RK_LOGD("RK_MPI_VI_ChnSaveFile %x", s32Ret);
    }

__FAILED:
    return s32Ret;
}

static RK_S32 test_vi_thumbnail_init(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;

    // 0. get dev config status
    s32Ret = RK_MPI_VI_GetDevAttr(ctx->devId, &ctx->stDevAttr);
    if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
        // 0-1.config dev
        s32Ret = RK_MPI_VI_SetDevAttr(ctx->devId, &ctx->stDevAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevAttr %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_SetDevAttr already");
    }
    // 1.get  dev enable status
    s32Ret = RK_MPI_VI_GetDevIsEnable(ctx->devId);
    if (s32Ret != RK_SUCCESS) {
        // 1-2.enable dev
        s32Ret = RK_MPI_VI_EnableDev(ctx->devId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_EnableDev %x", s32Ret);
            goto __FAILED;
        }
        // 1-3.bind dev/pipe
        ctx->stBindPipe.u32Num = ctx->pipeId;
        ctx->stBindPipe.PipeId[0] = ctx->pipeId;
        s32Ret = RK_MPI_VI_SetDevBindPipe(ctx->devId, &ctx->stBindPipe);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevBindPipe %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_EnableDev already");
    }

    // 2.config channel
    VI_CHN_ATTR_S stChnAttr;
    memset(&stChnAttr, 0, sizeof(stChnAttr));
    stChnAttr.stIspOpt.u32BufCount = 5;
    stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
    stChnAttr.stIspOpt.bNoUseLibV4L2 = RK_TRUE;
    stChnAttr.u32Depth = 0;
    stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
    stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    stChnAttr.stFrameRate.s32DstFrameRate = -1;
    stChnAttr.stSize.u32Width  = THUMBNAIL_WIDTH;
    stChnAttr.stSize.u32Height = THUMBNAIL_HEIGHT;
    stChnAttr.u32Depth         = 1;
    stChnAttr.bMirror          = ctx->bMirror;
    stChnAttr.bFlip            = ctx->bFlip;
    stChnAttr.stIspOpt.stMaxSize.u32Width  = THUMBNAIL_WIDTH;
    stChnAttr.stIspOpt.stMaxSize.u32Height = THUMBNAIL_HEIGHT;

    s32Ret = RK_MPI_VI_SetChnAttr(ctx->pipeId, THUMBNAIL_VI_CHN, &stChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_SetChnAttr %x", s32Ret);
        goto __FAILED;
    }

    // 3.enable channel
    s32Ret = RK_MPI_VI_EnableChn(ctx->pipeId, THUMBNAIL_VI_CHN);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_EnableChn %x", s32Ret);
        goto __FAILED;
    }

__FAILED:
    return s32Ret;
}

static void test_vi_thumbnail_exit(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret;
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, THUMBNAIL_VI_CHN);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);
    }
}

static RK_S32 test_vi_bind_vo_loop(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stSrcChn, stDestChn;
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;

    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

#ifdef HAVE_VO
    // vo  init and create
    ctx->s32VoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    ctx->s32VoDev = RK356X_VO_DEV_HD0;
    s32Ret = create_vo(ctx, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create vo ch:%d failed", ctx->channelId);
        goto __FAILED;
    }
    // bind vi to vo
    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = ctx->channelId;

    stDestChn.enModId   = RK_ID_VO;
    stDestChn.s32DevId  = ctx->s32VoLayer;
    stDestChn.s32ChnId  = 0;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi band vo fail:%x", s32Ret);
        goto __FAILED;
    }

    // enable vo
    s32Ret = RK_MPI_VO_EnableChn(ctx->s32VoLayer, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("Enable vo chn failed, s32Ret = %d\n", s32Ret);
        goto __FAILED;
    }
#endif

    while (loopCount < ctx->loopCountSet) {
        loopCount++;
        RK_LOGE("loopCount:%d", loopCount);
        // can not get the vo frameout count . so here regard as 33ms one frame.
        usleep(33*1000);
    }

__FAILED:
    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
    }
#ifdef HAVE_VO
    // disable vo
    RK_MPI_VO_DisableLayer(ctx->s32VoLayer);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_1);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_1);
    RK_MPI_VO_DisableChn(ctx->s32VoLayer, 0);
    RK_MPI_VO_Disable(ctx->s32VoDev);
    RK_MPI_VO_DisableChn(ctx->s32VoLayer, 0);
#endif
    // 5. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // 6.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);

    return s32Ret;
}

static RK_S32 test_vi_bind_vpss_vo_loop(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stSrcChn, stDestChn;
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;

    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

#ifdef HAVE_VPSS
    /* vpss */
    ctx->stVpssCfg.u32VpssChnCnt = 1;
    ctx->stVpssCfg.stGrpVpssAttr.u32MaxW = 4096;
    ctx->stVpssCfg.stGrpVpssAttr.u32MaxH = 4096;
    ctx->stVpssCfg.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
    ctx->stVpssCfg.stGrpVpssAttr.stFrameRate.s32SrcFrameRate = -1;
    ctx->stVpssCfg.stGrpVpssAttr.stFrameRate.s32DstFrameRate = -1;
    ctx->stVpssCfg.stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE;
    for (i = 0; i < VPSS_MAX_CHN_NUM; i ++) {
        ctx->stVpssCfg.stVpssChnAttr[i].enChnMode = VPSS_CHN_MODE_USER;
        ctx->stVpssCfg.stVpssChnAttr[i].enDynamicRange = DYNAMIC_RANGE_SDR8;
        ctx->stVpssCfg.stVpssChnAttr[i].enPixelFormat = RK_FMT_YUV420SP;
        ctx->stVpssCfg.stVpssChnAttr[i].stFrameRate.s32SrcFrameRate = -1;
        ctx->stVpssCfg.stVpssChnAttr[i].stFrameRate.s32DstFrameRate = -1;
        ctx->stVpssCfg.stVpssChnAttr[i].u32Width = ctx->width / 2;
        ctx->stVpssCfg.stVpssChnAttr[i].u32Height = ctx->height / 2;
        ctx->stVpssCfg.stVpssChnAttr[i].enCompressMode = COMPRESS_MODE_NONE;
    }

    // init vpss
    s32Ret = create_vpss(&ctx->stVpssCfg, 0, ctx->stVpssCfg.u32VpssChnCnt);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("creat 0 grp vpss failed!");
        goto __FAILED;
    }
    // bind vi to vpss
    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = ctx->channelId;
    stDestChn.enModId = RK_ID_VPSS;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = 0;

    RK_LOGD("vi to vpss ch %d vpss group %d", stDestChn.s32ChnId , stDestChn.s32DevId);
    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi and vpss bind error ");
        goto __FAILED;
    }
#endif

#ifdef HAVE_VO
    // vo  init and create
    ctx->s32VoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    ctx->s32VoDev = RK356X_VO_DEV_HD0;
    s32Ret = create_vo(ctx, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create vo ch:%d failed", ctx->channelId);
        goto __FAILED;
    }
    // bind vpss to vo
    stSrcChn.enModId    = RK_ID_VPSS;
    stSrcChn.s32DevId   = 0;
    stSrcChn.s32ChnId   = 0;

    stDestChn.enModId   = RK_ID_VO;
    stDestChn.s32DevId  = ctx->s32VoLayer;
    stDestChn.s32ChnId  = 0;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi band vo fail:%x", s32Ret);
        goto __FAILED;
    }

    // enable vo
    s32Ret = RK_MPI_VO_EnableChn(ctx->s32VoLayer, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("Enable vo chn failed, s32Ret = %d\n", s32Ret);
        goto __FAILED;
    }
#endif
    while (loopCount < ctx->loopCountSet) {
        loopCount++;
        RK_LOGE("loopCount:%d", loopCount);
        // can not get the vo frameout count . so here regard as 33ms one frame.
        usleep(33*1000);
    }

__FAILED:
    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
    }

    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = ctx->channelId;
    stDestChn.enModId = RK_ID_VPSS;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = 0;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
    }

#ifdef HAVE_VPSS
    // destory vpss
    s32Ret = destory_vpss(0, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("destory vpss error");
        return s32Ret;
    }
#endif

#ifdef HAVE_VO
    // disable vo
    RK_MPI_VO_DisableLayer(ctx->s32VoLayer);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_1);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_1);
    RK_MPI_VO_DisableChn(ctx->s32VoLayer, 0);
    RK_MPI_VO_Disable(ctx->s32VoDev);
    RK_MPI_VO_DisableChn(ctx->s32VoLayer, 0);
#endif
    // 5. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // 6.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);

    return s32Ret;
}


static int test_vi_bind_vpss_send_vo_loop(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stViChn, stVpssChn;
    RK_S32 loopCount = 0;
    RK_U32 i = 0;
    RK_U32 u32DstCount = 1;
    void *pData = RK_NULL;
    char cWritePath[256] = {0};
    RK_S32 s32Ret = RK_FAILURE;
    VIDEO_FRAME_INFO_S pstVideoFrame[VPSS_MAX_CHN_NUM];
    memset(pstVideoFrame, 0, sizeof(pstVideoFrame));
    ctx->stDebugFile.bCfg = RK_FALSE;
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }
#ifdef HAVE_VPSS
    /* vpss */
    ctx->stVpssCfg.u32VpssChnCnt = 1;
    ctx->stVpssCfg.stGrpVpssAttr.u32MaxW = 4096;
    ctx->stVpssCfg.stGrpVpssAttr.u32MaxH = 4096;
    ctx->stVpssCfg.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
    ctx->stVpssCfg.stGrpVpssAttr.stFrameRate.s32SrcFrameRate = -1;
    ctx->stVpssCfg.stGrpVpssAttr.stFrameRate.s32DstFrameRate = -1;
    ctx->stVpssCfg.stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE;
    for (i = 0; i < VPSS_MAX_CHN_NUM; i ++) {
        ctx->stVpssCfg.stVpssChnAttr[i].enChnMode = VPSS_CHN_MODE_USER;
        ctx->stVpssCfg.stVpssChnAttr[i].enDynamicRange = DYNAMIC_RANGE_SDR8;
        ctx->stVpssCfg.stVpssChnAttr[i].enPixelFormat = RK_FMT_YUV420SP;
        ctx->stVpssCfg.stVpssChnAttr[i].stFrameRate.s32SrcFrameRate = -1;
        ctx->stVpssCfg.stVpssChnAttr[i].stFrameRate.s32DstFrameRate = -1;
        ctx->stVpssCfg.stVpssChnAttr[i].u32Width = 1920;
        ctx->stVpssCfg.stVpssChnAttr[i].u32Height = 1080;
        ctx->stVpssCfg.stVpssChnAttr[i].enCompressMode = COMPRESS_MODE_NONE;
        ctx->stVpssCfg.stVpssChnAttr[i].u32Depth = 1;
        ctx->stVpssCfg.stVpssChnAttr[i].u32FrameBufCnt = ctx->stChnAttr.stIspOpt.u32BufCount;
    }

    // init vpss
    s32Ret = create_vpss(&ctx->stVpssCfg, 0, ctx->stVpssCfg.u32VpssChnCnt);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("creat 0 grp vpss failed = 0x%x", s32Ret);
        goto __FAILED;
    }

    // bind vi to vpss
    stViChn.enModId    = RK_ID_VI;
    stViChn.s32DevId   = ctx->devId;
    stViChn.s32ChnId   = ctx->channelId;
    stVpssChn.enModId  = RK_ID_VPSS;
    stVpssChn.s32DevId = 0;
    stVpssChn.s32ChnId = 0;

    RK_LOGD("vi to vpss ch %d vpss group %d", stVpssChn.s32ChnId , stVpssChn.s32DevId);
    s32Ret = RK_MPI_SYS_Bind(&stViChn, &stVpssChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi and vpss bind error 0x%x", s32Ret);
        goto __FAILED;
    }
#endif
#ifdef HAVE_VO
    // vo  init and create
    ctx->s32VoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    ctx->s32VoDev = RK356X_VO_DEV_HD0;
    s32Ret = create_vo(ctx, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create vo ch:%d failed", ctx->channelId);
        goto __FAILED;
    }

    // enable vo
    s32Ret = RK_MPI_VO_EnableChn(ctx->s32VoLayer, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("Enable vo chn failed, s32Ret = %d\n", s32Ret);
        goto __FAILED;
    }
#endif

    while (loopCount < ctx->loopCountSet) {
        for (RK_S32 i = 0; i < u32DstCount; i++) {
#ifdef HAVE_VPSS
            s32Ret = RK_MPI_VPSS_GetChnFrame(0, i, &(pstVideoFrame[i]), 200);
#else
            s32Ret = RK_FAILURE;
#endif
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("%d RK_MPI_VPSS_GetChnFrame failed %d with 0x%x", loopCount, i, s32Ret);
                continue;
            }
            RK_LOGD("%d RK_MPI_VPSS_GetChnFrame succeed %d: %p pixelFormate: %d, w: %d, h: %d, vw: %d, vh: %d",
                    loopCount, i, pstVideoFrame[i].stVFrame.pMbBlk,
                    pstVideoFrame[i].stVFrame.enPixelFormat,
                    pstVideoFrame[i].stVFrame.u32Width,
                    pstVideoFrame[i].stVFrame.u32Height,
                    pstVideoFrame[i].stVFrame.u32VirWidth,
                    pstVideoFrame[i].stVFrame.u32VirHeight);

            if (ctx->stVpssCfg.bOutDebugCfg) {
                pData = RK_MPI_MB_Handle2VirAddr(pstVideoFrame[i].stVFrame.pMbBlk);
                int size = RK_MPI_MB_GetSize(pstVideoFrame[i].stVFrame.pMbBlk);
                fwrite(pData, 1, size, ctx->stVpssCfg.fp);
                fflush(ctx->stVpssCfg.fp);
            }
#ifdef HAVE_VO
            s32Ret = RK_MPI_VO_SendFrame(ctx->s32VoLayer, 0, &(pstVideoFrame[i]), -1);
#else
            sleep(1);
#endif
#ifdef HAVE_VPSS
            s32Ret = RK_MPI_VPSS_ReleaseChnFrame(0, i, &(pstVideoFrame[i]));
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("%d RK_MPI_VPSS_ReleaseChnFrame failed %d with 0x%x", loopCount, i, s32Ret);
                continue;
            }
#endif
            RK_LOGD("%d RK_MPI_VPSS_ReleaseChnFrame succeed %d: %p", loopCount, i, pstVideoFrame[i].stVFrame.pMbBlk);
        }
        loopCount++;
    }

__FAILED:
    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    s32Ret = RK_MPI_SYS_UnBind(&stViChn, &stVpssChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_UnBind failed with 0x%x", s32Ret);
    }

#ifdef HAVE_VPSS
    s32Ret = destory_vpss(0, ctx->stVpssCfg.u32VpssChnCnt);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("destory vpss failed with 0x%x", s32Ret);
        return s32Ret;
    }
#endif

#ifdef HAVE_VO
     // disable vo
    RK_MPI_VO_DisableLayer(ctx->s32VoLayer);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_1);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_1);
    RK_MPI_VO_DisableChn(ctx->s32VoLayer, 0);
    RK_MPI_VO_Disable(ctx->s32VoDev);
#endif

    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGD("RK_MPI_VI_DisableChn return with 0x%x", s32Ret);

    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGD("RK_MPI_VI_DisableDev return with 0x%x", s32Ret);

    return s32Ret;
}


static RK_S32 test_vi_bind_vpss_venc_loop(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stViChn, stVencChn[TEST_VENC_MAX], stVpssChn;
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;
    RK_U32 u32DstCount = 1;
    ctx->stDebugFile.bCfg = RK_FALSE;
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

#ifdef HAVE_VPSS
    /* vpss */
    ctx->width = ctx->width / 2;
    ctx->height = ctx->height / 2;
    ctx->stVpssCfg.u32VpssChnCnt = 1;
    ctx->stVpssCfg.stGrpVpssAttr.u32MaxW = 4096;
    ctx->stVpssCfg.stGrpVpssAttr.u32MaxH = 4096;
    ctx->stVpssCfg.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
    ctx->stVpssCfg.stGrpVpssAttr.stFrameRate.s32SrcFrameRate = -1;
    ctx->stVpssCfg.stGrpVpssAttr.stFrameRate.s32DstFrameRate = -1;
    ctx->stVpssCfg.stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE;
    for (i = 0; i < VPSS_MAX_CHN_NUM; i ++) {
        ctx->stVpssCfg.stVpssChnAttr[i].enChnMode = VPSS_CHN_MODE_USER;
        ctx->stVpssCfg.stVpssChnAttr[i].enDynamicRange = DYNAMIC_RANGE_SDR8;
        ctx->stVpssCfg.stVpssChnAttr[i].enPixelFormat = RK_FMT_YUV420SP;
        ctx->stVpssCfg.stVpssChnAttr[i].stFrameRate.s32SrcFrameRate = -1;
        ctx->stVpssCfg.stVpssChnAttr[i].stFrameRate.s32DstFrameRate = -1;
        ctx->stVpssCfg.stVpssChnAttr[i].u32Width = ctx->width;
        ctx->stVpssCfg.stVpssChnAttr[i].u32Height = ctx->height;
        ctx->stVpssCfg.stVpssChnAttr[i].enCompressMode = COMPRESS_MODE_NONE;
    }

    // init vpss
    s32Ret = create_vpss(&ctx->stVpssCfg, 0, ctx->stVpssCfg.u32VpssChnCnt);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("creat 0 grp vpss failed!");
        goto __FAILED;
    }
    // bind vi to vpss
    stViChn.enModId    = RK_ID_VI;
    stViChn.s32DevId   = ctx->devId;
    stViChn.s32ChnId   = ctx->channelId;
    stVpssChn.enModId = RK_ID_VPSS;
    stVpssChn.s32DevId = 0;
    stVpssChn.s32ChnId = 0;

    RK_LOGD("vi to vpss ch %d vpss group %d", stVpssChn.s32ChnId , stVpssChn.s32DevId);
    s32Ret = RK_MPI_SYS_Bind(&stViChn, &stVpssChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi and vpss bind error ");
        goto __FAILED;
    }
#endif
    if (open_venc_file(ctx, u32DstCount) != RK_SUCCESS)
        goto __FAILED;

    /* venc */
    for (i = 0; i < u32DstCount; i++) {
        // venc  init and create
        init_venc_cfg(ctx, i, (RK_CODEC_ID_E)ctx->u32DstCodec);
        s32Ret = create_venc(ctx, ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("create %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            return s32Ret;
        }
        // bind vpss to venc
        stVencChn[i].enModId   = RK_ID_VENC;
        stVencChn[i].s32DevId  = 0;
        stVencChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;

        s32Ret = RK_MPI_SYS_Bind(&stVpssChn, &stVencChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("create %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            goto __FAILED;
        }
        ctx->stFrame[i].pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));
#if TEST_WITH_FD
        ctx->stVencCfg[i].selectFd = RK_MPI_VENC_GetFd(ctx->stVencCfg[i].s32ChnId);
        RK_LOGE("venc chn:%d, ctx->selectFd:%d ", ctx->stVencCfg[i].s32ChnId, ctx->stVencCfg[i].selectFd);
#endif
    }

    while (loopCount < ctx->loopCountSet) {
        for (i = 0; i < u32DstCount; i++) {
#if TEST_WITH_FD
            test_vi_poll_event(-1, ctx->stVencCfg[i].selectFd);
#endif
            // freeze test
            RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);

            s32Ret = RK_MPI_VENC_GetStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i], -1);
            if (s32Ret == RK_SUCCESS) {
                if (ctx->stVencCfg[i].bOutDebugCfg) {
                    pData = RK_MPI_MB_Handle2VirAddr(ctx->stFrame[i].pstPack->pMbBlk);
                    fwrite(pData, 1, ctx->stFrame[i].pstPack->u32Len, ctx->stVencCfg[i].fp);
                    fflush(ctx->stVencCfg[i].fp);
                }
                RK_LOGD("chn:%d, loopCount:%d enc->seq:%d wd:%d\n", i, loopCount,
                         ctx->stFrame[i].u32Seq, ctx->stFrame[i].pstPack->u32Len);
                s32Ret = RK_MPI_VENC_ReleaseStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i]);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
                }
                loopCount++;
            } else {
                RK_LOGE("RK_MPI_VI_GetChnFrame fail %x", s32Ret);
            }
        }
        usleep(10*1000);
    }

__FAILED:
    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    // unbind vi->vpss
    s32Ret = RK_MPI_SYS_UnBind(&stViChn, &stVpssChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
    }
    // unbind vpss->venc
    for (i = 0; i < u32DstCount; i++) {
        s32Ret = RK_MPI_SYS_UnBind(&stVpssChn, &stVencChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
        }
    }
#ifdef HAVE_VPSS
    // destory vpss
    s32Ret = destory_vpss(0, ctx->stVpssCfg.u32VpssChnCnt);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("destory vpss error");
        return s32Ret;
    }
#endif
    // 5. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    for (i = 0; i < u32DstCount; i++) {
        s32Ret = RK_MPI_VENC_StopRecvFrame(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        RK_LOGE("destroy enc chn:%d", ctx->stVencCfg[i].s32ChnId);
        s32Ret = RK_MPI_VENC_DestroyChn(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
        }
    }

    // 6.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    for (i = 0; i < u32DstCount; i++) {
      if (ctx->stFrame[i].pstPack)
          free(ctx->stFrame[i].pstPack);
      if (ctx->stVencCfg[i].fp)
          fclose(ctx->stVencCfg[i].fp);
    }
    return s32Ret;
}

static RK_S32 test_vi_bind_venc_loop(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stSrcChn, stDestChn[TEST_VENC_MAX], stIvsChn;
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;
    RK_U32 u32DstCount = ((ctx->enMode == TEST_VI_MODE_BIND_VENC_MULTI) ? 2 : 1);
    ctx->stDebugFile.bCfg = RK_FALSE;
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

    if (open_venc_file(ctx, u32DstCount) != RK_SUCCESS)
        goto __FAILED;

    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = ctx->channelId;

    if (ctx->u32Ivs) {
        s32Ret = create_ivs(ctx);
        if (s32Ret) {
            RK_LOGE("ivs init failed:%x", s32Ret);
            goto __FAILED;
        }
        stIvsChn.enModId   = RK_ID_IVS;
        stIvsChn.s32DevId  = 0;
        stIvsChn.s32ChnId  = 0;
        s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stIvsChn);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind vi to ivs failed");
            goto __FAILED;
        }
    }

    /* venc */
    for (i = 0; i < u32DstCount; i++) {
        // venc  init and create
        init_venc_cfg(ctx, i, (RK_CODEC_ID_E)ctx->u32DstCodec);
        s32Ret = create_venc(ctx, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("create %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            return s32Ret;
        }
         // bind vi to venc
        stDestChn[i].enModId   = RK_ID_VENC;
        stDestChn[i].s32DevId  = 0;
        stDestChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;

        s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            goto __FAILED;
        }
        ctx->stFrame[i].pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));
#if TEST_WITH_FD
        ctx->stVencCfg[i].selectFd = RK_MPI_VENC_GetFd(ctx->stVencCfg[i].s32ChnId);
        RK_LOGE("venc chn:%d, ctx->selectFd:%d ", ctx->stVencCfg[i].s32ChnId, ctx->stVencCfg[i].selectFd);
#endif

        if (ctx->bCombo)
            create_venc_combo(ctx, ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN, ctx->stVencCfg[i].s32ChnId);
    }

    while (loopCount < ctx->loopCountSet) {
        for (i = 0; i < u32DstCount; i++) {
#if TEST_WITH_FD
            test_vi_poll_event(-1, ctx->stVencCfg[i].selectFd);
#endif
            // freeze test
            RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);

            s32Ret = RK_MPI_VENC_GetStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i], -1);
            if (s32Ret == RK_SUCCESS) {
                if (ctx->stVencCfg[i].bOutDebugCfg) {
                    pData = RK_MPI_MB_Handle2VirAddr(ctx->stFrame[i].pstPack->pMbBlk);
                    fwrite(pData, 1, ctx->stFrame[i].pstPack->u32Len, ctx->stVencCfg[i].fp);
                    fflush(ctx->stVencCfg[i].fp);
                }
                RK_U64 nowUs = TEST_COMM_GetNowUs();

                RK_LOGD("chn:%d, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n", i, loopCount,
                         ctx->stFrame[i].u32Seq, ctx->stFrame[i].pstPack->u32Len,
                         ctx->stFrame[i].pstPack->u64PTS,
                         nowUs - ctx->stFrame[i].pstPack->u64PTS);
                usleep(ctx->u32DelayMsGet * 1000);
                s32Ret = RK_MPI_VENC_ReleaseStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i]);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
                }
                loopCount++;
            } else {
                RK_LOGE("RK_MPI_VI_GetChnFrame fail %x", s32Ret);
            }

            if (ctx->u32Ivs) {
                IVS_RESULT_INFO_S stResults;
                memset(&stResults, 0, sizeof(IVS_RESULT_INFO_S));
                s32Ret = RK_MPI_IVS_GetResults(0, &stResults, -1);
                if (s32Ret == RK_SUCCESS) {
                    if (loopCount % 10 == 0 && stResults.s32ResultNum == 1) {
                        int x = ctx->width / 8 / 8;
                        int y = ctx->height / 8;
                        if (ctx->bIvsDebug && stResults.pstResults->stMdInfo.pData) {
                            for (int n = 0; n < x * 8; n++)
                                printf("-");
                            printf("\n");
                            for (int j = 0; j < y; j++) {
                                for (int i = 0; i < x; i++) {
                                    for (int k = 0; k < 8; k++) {
                                        if (stResults.pstResults->stMdInfo.pData[j * 64 + i] & (1 << k))
                                            printf("1");
                                        else
                                            printf("0");
                                    }
                                }
                                printf("\n");
                            }
                            for (int n = 0; n < x * 8; n++)
                                printf("-");
                            printf("\n");
                        }
                        printf("MD u32RectNum: %u\n", stResults.pstResults->stMdInfo.u32RectNum);
                        for (int i = 0; i < stResults.pstResults->stMdInfo.u32RectNum; i++) {
                            printf("%d: [%d, %d, %d, %d]\n", i,
                                   stResults.pstResults->stMdInfo.stRect[i].s32X,
                                   stResults.pstResults->stMdInfo.stRect[i].s32Y,
                                   stResults.pstResults->stMdInfo.stRect[i].u32Width,
                                   stResults.pstResults->stMdInfo.stRect[i].u32Height);
                        }
                    }
                    RK_MPI_IVS_ReleaseResults(0, &stResults);
                } else {
                    RK_LOGE("RK_MPI_IVS_GetResults fail %x", s32Ret);
                }
            }

        }
        usleep(10*1000);
    }

__FAILED:
    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    for (i = 0; i < u32DstCount; i++) {
        s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
        }
    }

    // 5. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    for (i = 0; i < u32DstCount; i++) {
        if (ctx->bCombo)
            destroy_venc_combo(ctx, ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN);

        s32Ret = RK_MPI_VENC_StopRecvFrame(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        RK_LOGE("destroy enc chn:%d", ctx->stVencCfg[i].s32ChnId);
        s32Ret = RK_MPI_VENC_DestroyChn(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
        }
    }

    if (ctx->u32Ivs) {
        s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stIvsChn);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_SYS_UnBind vi ivs fail %x", s32Ret);
        }
        destroy_ivs();
    }

    // 6.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    for (i = 0; i < u32DstCount; i++) {
      if (ctx->stFrame[i].pstPack)
          free(ctx->stFrame[i].pstPack);
      if (ctx->stVencCfg[i].fp)
          fclose(ctx->stVencCfg[i].fp);
    }
    return s32Ret;
}

static void *thumbnail_thread(void *arg) {
    TEST_VI_CTX_S *ctx = (TEST_VI_CTX_S *)arg;
    VENC_STREAM_S stFrame;
    void *pData = RK_NULL;
    RK_S32 s32Ret;

    memset(&stFrame, 0, sizeof(stFrame));
    stFrame.pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));

    if (!stFrame.pstPack) {
        RK_LOGE("no memory!");
        pthread_exit(NULL);
    }

    while (!ctx->thumbnail_exit) {
        s32Ret = RK_MPI_VENC_GetStream(THUMBNAIL_VENC_CHN, &stFrame, 1000);
        if (s32Ret == RK_SUCCESS) {
            FILE *fp = NULL;
            char name[128];
            snprintf(name, sizeof(name), "/data/tb_%lld.jpg", stFrame.pstPack->u64PTS);
            fp = fopen(name, "wb");
            if (fp) {
                pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
                fwrite(pData, 1, stFrame.pstPack->u32Len, fp);
                fclose(fp);
            }

            s32Ret = RK_MPI_VENC_ReleaseStream(THUMBNAIL_VENC_CHN, &stFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
            }
        }
    }

    free(stFrame.pstPack);

    pthread_exit(NULL);
}

static RK_S32 test_thumbnail_init(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stSrcChn, stDestChn;
    RK_S32 loopCount = 0;
    RK_S32 s32Ret = RK_FAILURE;
    s32Ret = test_vi_thumbnail_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, THUMBNAIL_VI_CHN, s32Ret);
        goto __FAILED;
    }
    s32Ret = create_venc_thumbnail(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create %d ch venc failed", THUMBNAIL_VENC_CHN);
        goto __FAILED;
    }

    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = THUMBNAIL_VI_CHN;

    stDestChn.enModId   = RK_ID_VENC;
    stDestChn.s32DevId  = 0;
    stDestChn.s32ChnId  = THUMBNAIL_VENC_CHN;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind %d ch venc failed", THUMBNAIL_VENC_CHN);
        goto __FAILED;
    }

    if (pthread_create(&ctx->thumbnail_th, NULL, thumbnail_thread, ctx)) {
        RK_LOGE("create thumbnail_thread failed!");
        goto __FAILED;
    }

    return RK_SUCCESS;

__FAILED:
    return s32Ret;
}

static void test_thumbnail_exit(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret;
    MPP_CHN_S stSrcChn, stDestChn;
    ctx->thumbnail_exit = RK_TRUE;
    RK_LOGI("join thumbnail thread");
    pthread_join(ctx->thumbnail_th, NULL);

    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = THUMBNAIL_VI_CHN;

    stDestChn.enModId   = RK_ID_VENC;
    stDestChn.s32DevId  = 0;
    stDestChn.s32ChnId  = THUMBNAIL_VENC_CHN;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind %d ch venc failed", THUMBNAIL_VENC_CHN);
    }

    test_vi_thumbnail_exit(ctx);
    destroy_venc_thumbnail(ctx);
}

static RK_S32 test_vi_bind_venc_with_thumbnail(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stSrcChn, stDestChn[TEST_VENC_MAX];
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;
    RK_U32 u32DstCount = ((ctx->enMode == TEST_VI_MODE_BIND_VENC_MULTI) ? 2 : 1);
    ctx->stDebugFile.bCfg = RK_FALSE;
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

    if (open_venc_file(ctx, u32DstCount) != RK_SUCCESS)
        goto __FAILED;

    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = ctx->channelId;

    test_thumbnail_init(ctx);

    /* venc */
    for (i = 0; i < u32DstCount; i++) {
        // venc  init and create
        init_venc_cfg(ctx, i, (RK_CODEC_ID_E)ctx->u32DstCodec);
        s32Ret = create_venc(ctx, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("create %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            return s32Ret;
        }
        s32Ret = RK_MPI_VENC_ThumbnailBind(ctx->stVencCfg[i].s32ChnId, THUMBNAIL_VENC_CHN);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("thumbnail bind %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            return s32Ret;
        }
         // bind vi to venc
        stDestChn[i].enModId   = RK_ID_VENC;
        stDestChn[i].s32DevId  = 0;
        stDestChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;

        s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            goto __FAILED;
        }
        ctx->stFrame[i].pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));
#if TEST_WITH_FD
        ctx->stVencCfg[i].selectFd = RK_MPI_VENC_GetFd(ctx->stVencCfg[i].s32ChnId);
        RK_LOGE("venc chn:%d, ctx->selectFd:%d ", ctx->stVencCfg[i].s32ChnId, ctx->stVencCfg[i].selectFd);
#endif

        if (ctx->bCombo) {
            create_venc_combo(ctx, ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN, ctx->stVencCfg[i].s32ChnId);
            s32Ret = RK_MPI_VENC_ThumbnailBind(ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN, THUMBNAIL_VENC_CHN);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("thumbnail bind %d ch venc failed", ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN);
                return s32Ret;
            }
        }
    }

    while (loopCount < ctx->loopCountSet) {
        for (i = 0; i < u32DstCount; i++) {
#if TEST_WITH_FD
            test_vi_poll_event(-1, ctx->stVencCfg[i].selectFd);
#endif
            // freeze test
            RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);

            s32Ret = RK_MPI_VENC_GetStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i], -1);
            if (s32Ret == RK_SUCCESS) {
                if (ctx->stVencCfg[i].bOutDebugCfg) {
                    pData = RK_MPI_MB_Handle2VirAddr(ctx->stFrame[i].pstPack->pMbBlk);
                    fwrite(pData, 1, ctx->stFrame[i].pstPack->u32Len, ctx->stVencCfg[i].fp);
                    fflush(ctx->stVencCfg[i].fp);
                }
                RK_U64 nowUs = TEST_COMM_GetNowUs();

                RK_LOGD("chn:%d, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n", i, loopCount,
                         ctx->stFrame[i].u32Seq, ctx->stFrame[i].pstPack->u32Len,
                         ctx->stFrame[i].pstPack->u64PTS,
                         nowUs - ctx->stFrame[i].pstPack->u64PTS);
                usleep(ctx->u32DelayMsGet * 1000);
                s32Ret = RK_MPI_VENC_ReleaseStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i]);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
                }
                loopCount++;
            } else {
                RK_LOGE("RK_MPI_VI_GetChnFrame fail %x", s32Ret);
            }
        }
        usleep(10*1000);
    }

__FAILED:
    test_thumbnail_exit(ctx);

    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    for (i = 0; i < u32DstCount; i++) {
        s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
        }
    }

    // 5. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    for (i = 0; i < u32DstCount; i++) {
        if (ctx->bCombo)
            destroy_venc_combo(ctx, ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN);

        s32Ret = RK_MPI_VENC_StopRecvFrame(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        RK_LOGE("destroy enc chn:%d", ctx->stVencCfg[i].s32ChnId);
        s32Ret = RK_MPI_VENC_DestroyChn(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
        }
    }

    // 6.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    for (i = 0; i < u32DstCount; i++) {
      if (ctx->stFrame[i].pstPack)
          free(ctx->stFrame[i].pstPack);
      if (ctx->stVencCfg[i].fp)
          fclose(ctx->stVencCfg[i].fp);
    }
    return s32Ret;
}

static void *SecondGetFrame(void *arg) {
    int s32Ret;
    int loopCnt = 0;
    RK_S32 waitTime = 100;
    VIDEO_FRAME_INFO_S stViFrame;
    TEST_VI_CTX_S *ctx = (TEST_VI_CTX_S*)arg;

    if (!ctx->bGetStream) { // get frames in another process.
        while (!bquit) {
            usleep(1000);
        }
    }

    while (loopCnt < ctx->loopCountSet && ctx->bGetStream) {
        // get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->channelId, &stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p >>> loop:%d <<< \tseq:%d pts:%lld ms len=%d", data, loopCnt,
                     stViFrame.stVFrame.u32TimeRef, stViFrame.stVFrame.u64PTS / 1000,
                     stViFrame.stVFrame.u64PrivateData);

            // release the frame
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->channelId, &stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            } else {
                RK_LOGD("RK_MPI_VI_ReleaseChnFrame OK pid = %lu", pthread_self());
            }
            loopCnt++;
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }

        usleep(10*1000);
    }
    return RK_NULL;
}

static RK_S32 test_vi_get_release_frame_loop(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret;
    RK_S32 loopCount = 0;
    RK_S32 waitTime = 100;

    RK_BOOL bChangeRgn = RK_TRUE;

    /* test use getframe&release_frame */
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

    // freeze test
    RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);

    if (!ctx->bGetStream) { // get frames in another process.
        while (!bquit) {
            usleep(1000);
        }
    }

    /*below test for second thread to get video frame*/
    pthread_t another_getframe_thread;
    if (ctx->bSecondThr) {
        pthread_create(&another_getframe_thread, RK_NULL, SecondGetFrame, (void *)ctx);
    }

    while (loopCount < ctx->loopCountSet && ctx->bGetStream && !bquit) {
#if TEST_WITH_FD_SWITCH
        if (loopCount % 10 == 0 && ctx->selectFd != -1) {  // test close/reopen the fd
            RK_MPI_VI_CloseChnFd(ctx->pipeId, ctx->channelId);
            RK_LOGE("close ctx->pipeId=%d, ctx->channelId=%d, ctx->selectFd:%d",
                     ctx->pipeId, ctx->channelId, ctx->selectFd);
            ctx->selectFd = -1;
        } else {
            if (ctx->selectFd < 0) {
                ctx->selectFd = RK_MPI_VI_GetChnFd(ctx->pipeId, ctx->channelId);
                RK_LOGE("getfd ctx->pipeId=%d, ctx->channelId=%d, ctx->selectFd:%d",
                         ctx->pipeId, ctx->channelId, ctx->selectFd);
                // do not use non-block polling for the first time after switching fd
                test_vi_poll_event(33, ctx->selectFd);
            } else {
                test_vi_poll_event(-1, ctx->selectFd);
            }
        }
#elif TEST_WITH_FD
        test_vi_poll_event(-1, ctx->selectFd);
#endif

        // 5.get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_U64 phyAddr = RK_MPI_MB_Handle2PhysAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p, phyaddr:0x%08x, loop:%d seq:%d pts:%lld ms len=%d",
                     data,
                     (RK_U32)phyAddr,
                     loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef,
                     ctx->stViFrame.stVFrame.u64PTS / 1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);
            // 6.get the channel status
            s32Ret = RK_MPI_VI_QueryChnStatus(ctx->pipeId, ctx->channelId, &ctx->stChnStatus);
            RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d," \
                    "current frame id:%d,input lost:%d,output lost:%d," \
                    "framerate:%d,vbfail:%d delay=%lldus",
                     s32Ret,
                     ctx->stChnStatus.stSize.u32Width,
                     ctx->stChnStatus.stSize.u32Height,
                     ctx->stChnStatus.bEnable,
                     ctx->stChnStatus.u32CurFrameID,
                     ctx->stChnStatus.u32InputLostFrame,
                     ctx->stChnStatus.u32OutputLostFrame,
                     ctx->stChnStatus.u32FrameRate,
                     ctx->stChnStatus.u32VbFail,
                     nowUs - ctx->stViFrame.stVFrame.u64PTS);

            // 7.release the frame
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }

        usleep(10*1000);
    }

    if (ctx->bSecondThr)
        pthread_join(another_getframe_thread, RK_NULL);

__FAILED:
    // if enable rgn,deinit it and destory it.
    if (ctx->bEnRgn) {
        destory_rgn(ctx);
    }

    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    // 9. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // 10.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    return s32Ret;
}

static RK_S32 create_vi(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;

    // 0. get dev config status
    s32Ret = RK_MPI_VI_GetDevAttr(ctx->devId, &ctx->stDevAttr);
    if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
        // 0-1.config dev
        s32Ret = RK_MPI_VI_SetDevAttr(ctx->devId, &ctx->stDevAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevAttr %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_SetDevAttr already");
    }
    // 1.get  dev enable status
    s32Ret = RK_MPI_VI_GetDevIsEnable(ctx->devId);
    if (s32Ret != RK_SUCCESS) {
        // 1-2.enable dev
        s32Ret = RK_MPI_VI_EnableDev(ctx->devId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_EnableDev %x", s32Ret);
            goto __FAILED;
        }
        // 1-3.bind dev/pipe
        ctx->stBindPipe.u32Num = ctx->pipeId;
        ctx->stBindPipe.PipeId[0] = ctx->pipeId;
        s32Ret = RK_MPI_VI_SetDevBindPipe(ctx->devId, &ctx->stBindPipe);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevBindPipe %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_EnableDev already");
    }
    // 2.config channel
    s32Ret = RK_MPI_VI_SetChnAttr(ctx->pipeId, ctx->channelId, &ctx->stChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_SetChnAttr %x", s32Ret);
        goto __FAILED;
    }
    // 3.enable channel
    RK_LOGD("RK_MPI_VI_EnableChn %x %d %d", ctx->devId, ctx->pipeId, ctx->channelId);
    s32Ret = RK_MPI_VI_EnableChn(ctx->pipeId, ctx->channelId);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_EnableChn %x", s32Ret);
        goto __FAILED;
    }
    // 4.save debug file
    if (ctx->stDebugFile.bCfg) {
        s32Ret = RK_MPI_VI_ChnSaveFile(ctx->pipeId, ctx->channelId, &ctx->stDebugFile);
        RK_LOGD("RK_MPI_VI_ChnSaveFile %x", s32Ret);
    }

__FAILED:
    return s32Ret;
}

static RK_S32 destroy_vi(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn pipe=%d, chn:%d, ret:%x", ctx->pipeId, ctx->channelId, s32Ret);

    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev pipe=%d ret:%x", ctx->pipeId, s32Ret);

    RK_SAFE_FREE(ctx);
    return s32Ret;
}

static RK_S32 test_vi_multi_sensor_vi_loop(TEST_VI_CTX_S *ctx_out) {
    RK_S32 s32Ret = RK_FAILURE;
    TEST_VI_CTX_S *pstViCtx[TEST_VI_SENSOR_NUM];
    RK_S32 loopCount = 0;
    RK_S32 waitTime = 100;
    RK_S32 i = 0;

    for (i = 0; i < TEST_VI_SENSOR_NUM; i++) {
        pstViCtx[i] = reinterpret_cast<TEST_VI_CTX_S *>(malloc(sizeof(TEST_VI_CTX_S)));
        memset(pstViCtx[i], 0, sizeof(TEST_VI_CTX_S));

        /* vi config init */
        pstViCtx[i]->devId = i;
        pstViCtx[i]->pipeId = pstViCtx[i]->devId;
        pstViCtx[i]->channelId = 2;

        if (TEST_VI_SENSOR_NUM == 2) {
            pstViCtx[i]->stChnAttr.stSize.u32Width = 2688;
            pstViCtx[i]->stChnAttr.stSize.u32Height = 1520;
        } else if (TEST_VI_SENSOR_NUM == 4 || TEST_VI_SENSOR_NUM == 6) {
            pstViCtx[i]->stChnAttr.stSize.u32Width = 2560;
            pstViCtx[i]->stChnAttr.stSize.u32Height = 1520;
        }
        pstViCtx[i]->stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;

        pstViCtx[i]->stChnAttr.stIspOpt.u32BufCount = 10;
        pstViCtx[i]->stChnAttr.stIspOpt.bNoUseLibV4L2 = RK_TRUE;
        pstViCtx[i]->stChnAttr.u32Depth = 2;
        pstViCtx[i]->stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
        pstViCtx[i]->stChnAttr.enCompressMode = COMPRESS_AFBC_16x16;
        pstViCtx[i]->stChnAttr.stFrameRate.s32SrcFrameRate = -1;
        pstViCtx[i]->stChnAttr.stFrameRate.s32DstFrameRate = -1;

        /* vi create */
        s32Ret = create_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vi [%d, %d] init failed: %x",
                    pstViCtx[i]->devId, pstViCtx[i]->channelId,
                    s32Ret);
            goto __FAILED_VI;
        }
    }

    while (loopCount < ctx_out->loopCountSet) {
        for (i = 0; i < TEST_VI_SENSOR_NUM; i++) {
            // get the frame
            s32Ret = RK_MPI_VI_GetChnFrame(pstViCtx[i]->pipeId, pstViCtx[i]->channelId,
                                            &pstViCtx[i]->stViFrame, waitTime);
            if (s32Ret == RK_SUCCESS) {
                RK_U64 nowUs = TEST_COMM_GetNowUs();
                void *data = RK_MPI_MB_Handle2VirAddr(pstViCtx[i]->stViFrame.stVFrame.pMbBlk);
                RK_LOGD("dev %d GetChnFrame Success!", i);
                RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d",
                        data, loopCount, pstViCtx[i]->stViFrame.stVFrame.u32TimeRef,
                        pstViCtx[i]->stViFrame.stVFrame.u64PTS/1000,
                        pstViCtx[i]->stViFrame.stVFrame.u64PrivateData);
                // get the channel status
                s32Ret = RK_MPI_VI_QueryChnStatus(pstViCtx[i]->pipeId, pstViCtx[i]->channelId,
                                                    &pstViCtx[i]->stChnStatus);
                RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d," \
                        "input lost:%d, output lost:%d, framerate:%d,vbfail:%d delay=%lldus",
                        s32Ret,
                        pstViCtx[i]->stChnStatus.stSize.u32Width,
                        pstViCtx[i]->stChnStatus.stSize.u32Height,
                        pstViCtx[i]->stChnStatus.bEnable,
                        pstViCtx[i]->stChnStatus.u32InputLostFrame,
                        pstViCtx[i]->stChnStatus.u32OutputLostFrame,
                        pstViCtx[i]->stChnStatus.u32FrameRate,
                        pstViCtx[i]->stChnStatus.u32VbFail,
                        nowUs - pstViCtx[i]->stViFrame.stVFrame.u64PTS);
                // release the frame
                s32Ret = RK_MPI_VI_ReleaseChnFrame(pstViCtx[i]->pipeId, pstViCtx[i]->channelId,
                                                    &pstViCtx[i]->stViFrame);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
                }
            } else {
                RK_LOGE("dev %d RK_MPI_VI_GetChnFrame timeout %x", i, s32Ret);
            }
            usleep(3*1000);
        }
        loopCount++;
        usleep(10*1000);
    }

__FAILED_VI:
    /* destroy vi*/
    for (RK_S32 i = 0; i < TEST_VI_SENSOR_NUM; i++) {
        s32Ret = destroy_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("destroy vi [%d, %d] error %x",
                    pstViCtx[i]->devId, pstViCtx[i]->channelId,
                    s32Ret);
            goto __FAILED;
        }
    }
__FAILED:
    return s32Ret;
}

static int test_vi_vo_get_send_loop(TEST_VI_CTX_S *ctx) {
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;
    RK_S32 waitTime = 100;
    RK_U32 u32DstCount = ((ctx->enMode == TEST_VI_MODE_BIND_VENC_MULTI) ? 2 : 1);
    VIDEO_FRAME_INFO_S   stFrame;
    /* test use getframe&release_frame */
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

#ifdef HAVE_VO
    // vo  init and create
    ctx->s32VoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    ctx->s32VoDev = RK356X_VO_DEV_HD0;
    s32Ret = create_vo(ctx, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create vo ch:%d failed", ctx->channelId);
        goto __FAILED;
    }

    // enable vo
    s32Ret = RK_MPI_VO_EnableChn(ctx->s32VoLayer, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("Enable vo chn failed, s32Ret = %d\n", s32Ret);
        goto __FAILED;
    }
#endif

    while (loopCount < ctx->loopCountSet) {
        // freeze test
        RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);

        // 5.get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d",
                     data, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef,
                     ctx->stViFrame.stVFrame.u64PTS/1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);

            // 6.get the channel status
            s32Ret = RK_MPI_VI_QueryChnStatus(ctx->pipeId, ctx->channelId, &ctx->stChnStatus);
            RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d,lost:%d,framerate:%d,vbfail:%d delay=%lldus",
                     s32Ret,
                     ctx->stChnStatus.stSize.u32Width,
                     ctx->stChnStatus.stSize.u32Height,
                     ctx->stChnStatus.bEnable,
                     ctx->stChnStatus.u32OutputLostFrame,
                     ctx->stChnStatus.u32FrameRate,
                     ctx->stChnStatus.u32VbFail,
                     nowUs - ctx->stViFrame.stVFrame.u64PTS);


            RK_LOGD("RK_MPI_VO_SendFrame mpi_buf id:%d", ctx->stViFrame.stVFrame.u64PrivateData);
#ifdef HAVE_VO
            s32Ret = RK_MPI_VO_SendFrame(ctx->s32VoLayer, 0, &ctx->stViFrame, -1);
#endif
            // 7.release the frame
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }
        // usleep(10*1000);
    }

__FAILED:
    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    // 9. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // 10.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    // disable vo
#ifdef HAVE_VO
    RK_MPI_VO_DisableLayer(ctx->s32VoLayer);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_ESMART_1);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_0);
    RK_MPI_VO_DisableLayer(RK356X_VOP_LAYER_SMART_1);
    RK_MPI_VO_DisableChn(ctx->s32VoLayer, 0);
    RK_MPI_VO_Disable(ctx->s32VoDev);
    RK_MPI_VO_DisableChn(ctx->s32VoLayer, 0);
#endif
    return s32Ret;
}

static int test_vi_venc_get_send_loop(TEST_VI_CTX_S *ctx) {
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;
    RK_S32 waitTime = 100;
    RK_U32 u32DstCount = ((ctx->enMode == TEST_VI_MODE_BIND_VENC_MULTI) ? 2 : 1);
    VIDEO_FRAME_INFO_S   stFrame;

    /* test use getframe&release_frame */
    ctx->stDebugFile.bCfg = RK_FALSE;
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

    if (open_venc_file(ctx, u32DstCount) != RK_SUCCESS)
        goto __FAILED;

    /* venc */
    for (i = 0; i < u32DstCount; i++) {
        // venc  init and create
        init_venc_cfg(ctx, i, (RK_CODEC_ID_E)ctx->u32DstCodec);
        s32Ret = create_venc(ctx, ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("create %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            return s32Ret;
        }

        ctx->stFrame[i].pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));
#if TEST_WITH_FD
        ctx->stVencCfg[i].selectFd = RK_MPI_VENC_GetFd(ctx->stVencCfg[i].s32ChnId);
        RK_LOGE("venc chn:%d, ctx->selectFd:%d ", ctx->stVencCfg[i].s32ChnId, ctx->stVencCfg[i].selectFd);
#endif
    }

    while (loopCount < ctx->loopCountSet) {
        // freeze test
        RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);

        // 5.get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d",
                     data, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef,
                     ctx->stViFrame.stVFrame.u64PTS/1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);

            // 6.get the channel status
            s32Ret = RK_MPI_VI_QueryChnStatus(ctx->pipeId, ctx->channelId, &ctx->stChnStatus);
            RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d,lost:%d,framerate:%d,vbfail:%d delay=%lldus",
                     s32Ret,
                     ctx->stChnStatus.stSize.u32Width,
                     ctx->stChnStatus.stSize.u32Height,
                     ctx->stChnStatus.bEnable,
                     ctx->stChnStatus.u32OutputLostFrame,
                     ctx->stChnStatus.u32FrameRate,
                     ctx->stChnStatus.u32VbFail,
                     nowUs - ctx->stViFrame.stVFrame.u64PTS);

            for (i = 0; i < u32DstCount; i++) {
                // TODO(chad): send vi frame to venc to encode.
                RK_LOGD("RK_MPI_VENC_SendFrame mpi_buf id:%d", ctx->stViFrame.stVFrame.u64PrivateData);
                s32Ret = RK_MPI_VENC_SendFrame(ctx->stVencCfg[i].s32ChnId, &ctx->stViFrame, -1);

                // save encode to file
                s32Ret = RK_MPI_VENC_GetStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i], -1);
                if (s32Ret == RK_SUCCESS) {
                    if (ctx->stVencCfg[i].bOutDebugCfg) {
                        pData = RK_MPI_MB_Handle2VirAddr(ctx->stFrame[i].pstPack->pMbBlk);
                        fwrite(pData, 1, ctx->stFrame[i].pstPack->u32Len, ctx->stVencCfg[i].fp);
                        fflush(ctx->stVencCfg[i].fp);
                    }
                    RK_U64 nowUs = TEST_COMM_GetNowUs();
                    RK_LOGD("chn:%d, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n", i, loopCount,
                            ctx->stFrame[i].u32Seq, ctx->stFrame[i].pstPack->u32Len,
                            ctx->stFrame[i].pstPack->u64PTS,
                            nowUs - ctx->stFrame[i].pstPack->u64PTS);
                    s32Ret = RK_MPI_VENC_ReleaseStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i]);
                    if (s32Ret != RK_SUCCESS) {
                        RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
                    }
                }
            }
            // 7.release the frame
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }
        // usleep(10*1000);
    }

__FAILED:

    // 9. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // 10.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    return s32Ret;
}

#define TEST_VI_CHANNEL_NUM     3

static int ParseSensorInfo() {
    int ret = 0;
    int fd = -1;
    char buf[1024];
    ssize_t nbytes;
    char *pBuf;

    fd = open(RKISP_DEV, O_RDONLY);
    if (fd < 0) {
        goto __FAIL;
    }

    nbytes = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (nbytes < 0) {
        goto __FAIL;
    }
    buf[nbytes] = '\0';
    /* Parse the contents of the file, which looks like:
     *
     *     # cat /proc/rkisp-vir0
     *     rkisp-vir0 Version:v01.09.00
     *     clk_isp_core 339428572
     *     aclk_isp   339428572
     *     hclk_isp   148500000
     *     clk_isp_core_vicap 0
     *     Interrupt  Cnt:0 ErrCnt:0
     *     Input      rkcif-mipi-lvds Format:SBGGR10_1X10 Size:2880x1616@30fps Offset(0,0)
     */
    pBuf = buf;
    while (nbytes > 0) {
        int matches;
        char format[32], sizeStr[32], offSet[8];
        matches = sscanf(pBuf, "Input      rkcif-mipi-lvds %s %s %s",
                format, sizeStr, offSet);
        if (matches > 0) {
            // printf("//////////////// matches:%d ///////////////\n", matches);
            if (strstr(sizeStr, "Size")) {
                int w, h, fps;
                sscanf(sizeStr, "Size:%dx%d@%dfps", &w, &h, &fps);
                // printf("//////////////// w:%d, h:%d, fps:%d ///////////////\n", w, h, fps);
                g_sensorInfo.width  = w;
                g_sensorInfo.height = h;
                g_sensorInfo.fps    = fps;
                if (w == 2880 && h == 1616) {
                    g_sensorInfo.type = _5MEGA;
                } else if (w == 2560 && h == 1440) {
                    g_sensorInfo.type = _4MEGA;
                } else if (w == 2304 && h == 1296) {
                    g_sensorInfo.type = _3MEGA;
                } else if (w == 1920 && h == 1080) {
                    g_sensorInfo.type = _2MEGA;
                } else if (w == 1280 && h == 720) {
                    g_sensorInfo.type = _1MEGA;
                } else if (w == 3140 && h == 2160) {
                    g_sensorInfo.type = _8MEGA;
                }
                break;
            }
        }
        /* Eat the line.
         */
        while (nbytes > 0 && *pBuf != '\n') {
            pBuf++;
            nbytes--;
        }
        if (nbytes > 0) {
            pBuf++;
            nbytes--;
        }
    }

    return 0;

__FAIL:
    return -1;
}

static int SetViChnReso(int chn, uint32_t &w, uint32_t &h) {
    if (chn < 0 || chn > TEST_VI_CHANNEL_NUM) {
        printf("Error: chn %d invalid or exceed the TEST_VI_CHANNEL_NUM", chn);
        return -1;
    }
    switch (g_sensorInfo.type) {
        case _1MEGA:
            w = g_sensorInfo.width; h = g_sensorInfo.height;
            break;
        case _2MEGA:
            if (chn == VI_CHN0) {
                w = g_sensorInfo.width; h = g_sensorInfo.height;
            } else if (chn == VI_CHN1 || chn == VI_CHN2) {
                w = 1280; h = 720;
            } break;
        case _3MEGA:
            if (chn == VI_CHN0) {
                w = g_sensorInfo.width; h = g_sensorInfo.height;
            } else if (chn == VI_CHN1) {
                w = 1920; h = 1080;
            } else if(chn == VI_CHN2) {
                 w = 1280; h = 720;
            } break;
        case _4MEGA:
            if (chn == VI_CHN0) {
                w = g_sensorInfo.width; h = g_sensorInfo.height;
            } else if (chn == VI_CHN1) {
                w = 1920; h = 1080;
            } else if(chn == VI_CHN2) {
                 w = 1280; h = 720;
            } break;
        case _5MEGA:
            if (chn == VI_CHN0) {
                w = g_sensorInfo.width; h = g_sensorInfo.height;
            } else if (chn == VI_CHN1) {
                w = 2560; h = 1440;
            } else if(chn == VI_CHN2) {
                 w = 1920; h = 1080;
            } break;
        case _8MEGA:
            if (chn == VI_CHN0) {
                w = g_sensorInfo.width; h = g_sensorInfo.height;
            } else {
                w = 1920; h = 1080;
            } break;
        case _4K:
            if (chn == VI_CHN0) {
                w = g_sensorInfo.width; h = g_sensorInfo.height;
            } else {
                w = 1920; h = 1080;
            } break;
        default:
            w = g_sensorInfo.width; h = g_sensorInfo.height;
            break;
    }

    return 0;
}

static void *GetViBuffer(void *arg) {
    void *pData = RK_NULL;
    int loopCount = 0;
    int s32Ret;
    int chn, pipe;
    TEST_VI_CTX_S *ctx = (TEST_VI_CTX_S *)arg;

    pipe = ctx->pipeId;
    chn = ctx->channelId;
    while (loopCount < ctx->loopCountSet) {
        s32Ret = RK_MPI_VI_GetChnFrame(pipe, chn, &ctx->stViFrame, 100);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d",
                     data, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef,
                     ctx->stViFrame.stVFrame.u64PTS/1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);
            s32Ret = RK_MPI_VI_ReleaseChnFrame(pipe, chn, &ctx->stViFrame);
            RK_ASSERT(s32Ret==RK_SUCCESS);
            loopCount++;
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }
    }
    return NULL;
}

/// @brief get frame and release it from one sensor multiple channels for
///        rv1106/rv1103
/// @param ctx
/// @return 0 - Success, < 0 - Failure
static int test_vi_one_sensor_multi_chn_loop(const TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    RK_S32 loopCount = 0;
    RK_S32 waitTime = 100;
    RK_S32 i = 0;
    RK_U32 chn_w = 0, chn_h = 0;
    TEST_VI_CTX_S *pstViCtx[TEST_VI_CHANNEL_NUM];

    ParseSensorInfo();

    for (i = 0; i < TEST_VI_CHANNEL_NUM; i++) {
        pstViCtx[i] = reinterpret_cast<TEST_VI_CTX_S *>(malloc(sizeof(TEST_VI_CTX_S)));
        memset(pstViCtx[i], 0, sizeof(TEST_VI_CTX_S));

        /* vi config init */
        pstViCtx[i]->devId = 0;
        pstViCtx[i]->pipeId = pstViCtx[i]->devId;
        pstViCtx[i]->channelId = i;
        pstViCtx[i]->loopCountSet = ctx->loopCountSet;

        // TODO(@Chad): according to sensor type to config vi channels resolution.
        if (SetViChnReso(i, chn_w, chn_h)) goto __FAILED_VI;
        RK_LOGD("+++VI_CHN%d: reso: %u x %u +++", i, chn_w, chn_h);
        pstViCtx[i]->stChnAttr.stSize.u32Width = chn_w;
        pstViCtx[i]->stChnAttr.stSize.u32Height = chn_h;
        pstViCtx[i]->stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
        pstViCtx[i]->stChnAttr.stIspOpt.u32BufCount = 3;
        pstViCtx[i]->stChnAttr.stIspOpt.bNoUseLibV4L2 = RK_TRUE;
        pstViCtx[i]->stChnAttr.u32Depth = 0;
        pstViCtx[i]->stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
        pstViCtx[i]->stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
        pstViCtx[i]->stChnAttr.stFrameRate.s32SrcFrameRate = ctx->stChnAttr.stFrameRate.s32SrcFrameRate;
        pstViCtx[i]->stChnAttr.stFrameRate.s32DstFrameRate = ctx->stChnAttr.stFrameRate.s32DstFrameRate;

        /* vi create */
        s32Ret = create_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vi [%d, %d] init failed: %x",
                    pstViCtx[i]->devId, pstViCtx[i]->channelId,
                    s32Ret);
            goto __FAILED_VI;
        }
    }

    pthread_t chn0_thread, chn1_thread, chn2_thread;
	pthread_create(&chn0_thread, NULL, GetViBuffer, pstViCtx[0]);
    pthread_create(&chn1_thread, NULL, GetViBuffer, pstViCtx[1]);
    pthread_create(&chn2_thread, NULL, GetViBuffer, pstViCtx[2]);

    pthread_join(chn0_thread, RK_NULL);
    pthread_join(chn1_thread, RK_NULL);
    pthread_join(chn2_thread, RK_NULL);

__FAILED_VI:
    /* destroy vi*/
    for (i = 0; i < TEST_VI_CHANNEL_NUM; i++) {
        s32Ret = RK_MPI_VI_DisableChn(pstViCtx[i]->pipeId, pstViCtx[i]->channelId);
        RK_LOGE("RK_MPI_VI_DisableChn pipe=%d, chn:%d, ret:%x", pstViCtx[i]->pipeId, pstViCtx[i]->channelId, s32Ret);

        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("disable vi dev:%d, chn %d error %x",pstViCtx[i]->devId, pstViCtx[i]->channelId, s32Ret);
        }
    }

    s32Ret = RK_MPI_VI_DisableDev(0);
    RK_LOGE("RK_MPI_VI_DisableDev dev 0: ret:%x", s32Ret);

    for (i = 0; i < TEST_VI_CHANNEL_NUM; i++) {
        RK_SAFE_FREE(pstViCtx[i]);
    }

__FAILED:
    return s32Ret;
}

static void mpi_vi_test_show_options(const TEST_VI_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");

    RK_PRINT("output file open      : %d\n", ctx->stDebugFile.bCfg);
    RK_PRINT("yuv output file name  : %s/%s\n", ctx->stDebugFile.aFilePath, ctx->stDebugFile.aFileName);
    RK_PRINT("enc0 output file path : /%s/%s\n", ctx->stVencCfg[0].dstFilePath, ctx->stVencCfg[0].dstFileName);
    RK_PRINT("enc1 output file path : /%s/%s\n", ctx->stVencCfg[1].dstFilePath, ctx->stVencCfg[1].dstFileName);
    RK_PRINT("loop count            : %d\n", ctx->loopCountSet);
    RK_PRINT("ctx->enMode           : %d\n", ctx->enMode);
    RK_PRINT("ctx->dev              : %d\n", ctx->devId);
    RK_PRINT("ctx->pipe             : %d\n", ctx->pipeId);
    RK_PRINT("ctx->channel          : %d\n", ctx->channelId);
    RK_PRINT("ctx->width            : %d\n", ctx->width);
    RK_PRINT("ctx->height           : %d\n", ctx->height);
    RK_PRINT("enCompressMode        : %d\n", ctx->enCompressMode);
    RK_PRINT("enMemoryType          : %d\n", ctx->stChnAttr.stIspOpt.enMemoryType);
    RK_PRINT("aEntityName           : %s\n", ctx->stChnAttr.stIspOpt.aEntityName);
    RK_PRINT("depth                 : %d\n", ctx->stChnAttr.u32Depth);
    RK_PRINT("enPixelFormat         : %d\n", ctx->stChnAttr.enPixelFormat);
    RK_PRINT("bFreeze               : %d\n", ctx->bFreeze);
    RK_PRINT("src_frame rate        : %d\n", ctx->stChnAttr.stFrameRate.s32SrcFrameRate);
    RK_PRINT("dst frame rate        : %d\n", ctx->stChnAttr.stFrameRate.s32DstFrameRate);
    RK_PRINT("out buf count         : %d\n", ctx->stChnAttr.stIspOpt.u32BufCount);
    RK_PRINT("venc ch start         : %d\n", ctx->vencChId);
    RK_PRINT("wrap mode enable      : %d\n", ctx->stChnWrap.bEnable);
    RK_PRINT("wrap line             : %d\n", ctx->stChnWrap.u32BufLine);

    RK_PRINT("bEnRgn                : %d\n", ctx->bEnRgn);
    RK_PRINT("bEnOverlay            : %d\n", ctx->bEnOverlay);
    RK_PRINT("rgn count             : %d\n", ctx->s32RgnCnt);
    RK_PRINT("rgn type              : %d\n", ctx->rgnType);
    RK_PRINT("enable mirror         : %d\n", ctx->bMirror);
    RK_PRINT("enable flip           : %d\n", ctx->bFlip);
    RK_PRINT("enable combo          : %d\n", ctx->bCombo);
    RK_PRINT("delay Ms Get          : %d\n", ctx->u32DelayMsGet);
    RK_PRINT("enable venc ref buf share : %d\n", ctx->bRefBufShare);
    RK_PRINT("delay Ms Get          : %d\n", ctx->u32DelayMsGet);
    RK_PRINT("max width             : %d\n", ctx->maxWidth);
    RK_PRINT("max height            : %d\n", ctx->maxHeight);
    RK_PRINT("get echo res frame    : %d\n", ctx->u32ChangeRes);
    RK_PRINT("enable get stream     : %d\n", ctx->bGetStream);
    RK_PRINT("enable second thread  : %d\n", ctx->bSecondThr);
    RK_PRINT("enable eptz           : %d\n", ctx->bEptz);
    RK_PRINT("enable use ext chn API: %d\n", ctx->bUseExt);
    RK_PRINT("enable rgn on pipe    : %d\n", ctx->bRgnOnPipe);
}

static RK_RESOLUTION_ST test_res[] = {
    {{1920, 1080}, "1920x1080"},    // 1080p
    {{1280,  720}, "1280x1080"},    // 720p
    {{720,   576}, "720 x 576"},    // 576p
    {{640,   480}, "640 x 480"},    // 480p
    {{640,   360}, "640 x 360"},    // 360p
    {{512,   288}, "512 x 288"},    //
    {{480,   320}, "480 x 320"},    // 320p
    {{320,   240}, "320 x 240"},    // 240p
    {{2304, 1296}, "2304x1296"},    // 300w
    {{2560, 1440}, "2560x1440"},    // 400w
    {{2880, 1616}, "2880x1616"},    // 500w
};

static SIZE_S test_rgn_size[] = {
    {352,  352},
    {304,  304},
    {256,  256},
    {208,  208},
    {208,  160},
    {160,  96 },
    {128,  80 },
    {64,   64 },
    {400,  400},
    {464,  464},
    {512,  512},
};

static RK_S32 test_vi_bind_venc_change_resolution(TEST_VI_CTX_S *ctx) {
    MPP_CHN_S stSrcChn, stDestChn[TEST_VENC_MAX];
    RK_S32 loopCount = 0;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 i;
    RK_U32 u32DstCount = ((ctx->enMode == TEST_VI_MODE_BIND_VENC_MULTI) ? 2 : 1);
    RK_U32 idx = 0;
    RK_U32 resCnt;
    resCnt = RK_ARRAY_ELEMS(test_res);
    ctx->stDebugFile.bCfg = RK_FALSE;
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

    ctx->stChnAttr.stIspOpt.stMaxSize.u32Width  = ctx->maxWidth;
    ctx->stChnAttr.stIspOpt.stMaxSize.u32Height = ctx->maxHeight;
    if (open_venc_file(ctx, u32DstCount) != RK_SUCCESS)
        goto __FAILED;

    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = ctx->devId;
    stSrcChn.s32ChnId   = ctx->channelId;

    /* venc */
    for (i = 0; i < u32DstCount; i++) {
        // venc  init and create
        init_venc_cfg(ctx, i, (RK_CODEC_ID_E)ctx->u32DstCodec);
        s32Ret = create_venc(ctx, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("create %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            return s32Ret;
        }
         // bind vi to venc
        stDestChn[i].enModId   = RK_ID_VENC;
        stDestChn[i].s32DevId  = 0;
        stDestChn[i].s32ChnId  = ctx->stVencCfg[i].s32ChnId;

        s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
            goto __FAILED;
        }
        ctx->stFrame[i].pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));
#if TEST_WITH_FD
        ctx->stVencCfg[i].selectFd = RK_MPI_VENC_GetFd(ctx->stVencCfg[i].s32ChnId);
        RK_LOGE("venc chn:%d, ctx->selectFd:%d ", ctx->stVencCfg[i].s32ChnId, ctx->stVencCfg[i].selectFd);
#endif

        if (ctx->bCombo)
            create_venc_combo(ctx, ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN, ctx->stVencCfg[i].s32ChnId);
    }

    /* creat overlay */
    if (ctx->bEnOverlay) {
        s32Ret = create_overlay(ctx, u32DstCount);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("create overlay failed");
            goto __FAILED;
        }
    }

    while (loopCount < ctx->loopCountSet) {
        for (i = 0; i < u32DstCount; i++) {
#if TEST_WITH_FD
            test_vi_poll_event(-1, ctx->stVencCfg[i].selectFd);
#endif
            // freeze test
            RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);

            s32Ret = RK_MPI_VENC_GetStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i], -1);
            if (s32Ret == RK_SUCCESS) {
                if (ctx->stVencCfg[i].bOutDebugCfg) {
                    pData = RK_MPI_MB_Handle2VirAddr(ctx->stFrame[i].pstPack->pMbBlk);
                    fwrite(pData, 1, ctx->stFrame[i].pstPack->u32Len, ctx->stVencCfg[i].fp);
                    fflush(ctx->stVencCfg[i].fp);
                }
                RK_U64 nowUs = TEST_COMM_GetNowUs();

                RK_LOGD("chn:%d, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n", i, loopCount,
                         ctx->stFrame[i].u32Seq, ctx->stFrame[i].pstPack->u32Len,
                         ctx->stFrame[i].pstPack->u64PTS,
                         nowUs - ctx->stFrame[i].pstPack->u64PTS);
                usleep(ctx->u32DelayMsGet * 1000);
                s32Ret = RK_MPI_VENC_ReleaseStream(ctx->stVencCfg[i].s32ChnId, &ctx->stFrame[i]);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
                }
                loopCount++;
            } else {
                RK_LOGE("RK_MPI_VI_GetChnFrame fail %x", s32Ret);
            }
        }

        if (loopCount % ctx->u32ChangeRes == 0) {
            RK_BOOL res_equal = RK_FALSE;
            if (idx >= resCnt)
                idx = 0;

            while (test_res[idx].size.u32Width > ctx->maxWidth ||
                   test_res[idx].size.u32Height > ctx->maxHeight) {
                idx++;
                if (idx >= resCnt)
                    idx = 0;
            }
            for (i = 0; i < u32DstCount; i++) {
                if (ctx->stVencCfg[i].stAttr.stVencAttr.u32PicWidth == test_res[idx].size.u32Width &&
                    ctx->stVencCfg[i].stAttr.stVencAttr.u32PicHeight == test_res[idx].size.u32Height) {
                    res_equal = RK_TRUE;
                    break;
                }
            }
            if (res_equal) {
                idx++;
                continue;
            }

            for (i = 0; i < u32DstCount; i++) {
                s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn[i]);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
                    goto __FAILED;
                }
            }

            for (i = 0; i < u32DstCount; i++) {
                ctx->stVencCfg[i].stAttr.stVencAttr.u32PicWidth = test_res[idx].size.u32Width;
                ctx->stVencCfg[i].stAttr.stVencAttr.u32PicHeight = test_res[idx].size.u32Height;
                ctx->stVencCfg[i].stAttr.stVencAttr.u32VirWidth = test_res[idx].size.u32Width;
                ctx->stVencCfg[i].stAttr.stVencAttr.u32VirHeight = test_res[idx].size.u32Height;
                RK_MPI_VENC_SetChnAttr(ctx->stVencCfg[i].s32ChnId, &ctx->stVencCfg[i].stAttr);
            }

            ctx->stChnAttr.stSize.u32Width  = test_res[idx].size.u32Width;
            ctx->stChnAttr.stSize.u32Height = test_res[idx].size.u32Height;
            s32Ret = RK_MPI_VI_SetChnAttr(ctx->pipeId, ctx->channelId, &ctx->stChnAttr);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_SetChnAttr %d x %d fail, ret: %x",
                        test_res[idx].size.u32Width, test_res[idx].size.u32Height, s32Ret);
                break;
            } else {
                RK_LOGD("RK_MPI_VI_SetChnAttr %d x %d success",
                        test_res[idx].size.u32Width, test_res[idx].size.u32Height);
            }

            // resize overlay region
            if (ctx->bEnOverlay) {
                s32Ret = resize_overlay(ctx, u32DstCount, test_rgn_size[idx]);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("resize overlay failed");
                    goto __FAILED;
                }
            }

            for (i = 0; i < u32DstCount; i++) {
                s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn[i]);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("bind %d ch venc failed", ctx->stVencCfg[i].s32ChnId);
                    goto __FAILED;
                }
            }

            idx++;
        }
        usleep(10*1000);
    }

__FAILED:
    for (i = 0; i < u32DstCount; i++) {
        s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
        }
    }

    // 5. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // destroy overlay
    if (ctx->bEnOverlay) {
        s32Ret = destroy_overlay(ctx, u32DstCount);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("destroy_overlay failed!");
        }
    }

    for (i = 0; i < u32DstCount; i++) {
        if (ctx->bCombo)
            destroy_venc_combo(ctx, ctx->stVencCfg[i].s32ChnId + COMBO_START_CHN);

        s32Ret = RK_MPI_VENC_StopRecvFrame(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        RK_LOGE("destroy enc chn:%d", ctx->stVencCfg[i].s32ChnId);
        s32Ret = RK_MPI_VENC_DestroyChn(ctx->stVencCfg[i].s32ChnId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
        }
    }

    // 6.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    for (i = 0; i < u32DstCount; i++) {
      if (ctx->stFrame[i].pstPack)
          free(ctx->stFrame[i].pstPack);
      if (ctx->stVencCfg[i].fp)
          fclose(ctx->stVencCfg[i].fp);
    }

    return s32Ret;
}

static int test_vi_change_resolution(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret;
    RK_S32 loopCount = 0;
    RK_S32 waitTime = 100;
    RK_U32 resCnt;
    RK_U32 idx = -1;
    RK_U32 getTimesToChgRes = ctx->u32ChangeRes;

    ctx->stDebugFile.bCfg = RK_TRUE;
    resCnt = RK_ARRAY_ELEMS(test_res);
    /* test use getframe&release_frame */
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

    // freeze test
    RK_MPI_VI_SetChnFreeze(ctx->pipeId, ctx->channelId, ctx->bFreeze);
    if (ctx->loopCountSet == 0)
        ctx->loopCountSet = getTimesToChgRes * (resCnt + 1);  // all resolution covered

    ctx->stChnAttr.stIspOpt.stMaxSize.u32Width  = ctx->maxWidth;
    ctx->stChnAttr.stIspOpt.stMaxSize.u32Height = ctx->maxHeight;

    while (loopCount < ctx->loopCountSet) {
        // 5.get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d", data, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef, ctx->stViFrame.stVFrame.u64PTS / 1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);
            // 6.get the channel status
            s32Ret = RK_MPI_VI_QueryChnStatus(ctx->pipeId, ctx->channelId, &ctx->stChnStatus);
            // 7.release the frame
            s32Ret |= RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
__RETRY:
            if (loopCount % getTimesToChgRes == 0) {
                if (idx >= resCnt) {
                    idx = 0;
                }

                if (loopCount != ctx->loopCountSet) {
                    if (idx >= 0 && loopCount != getTimesToChgRes) {
                        if (idx != 0)
                            RK_LOGD("res index: %d, write %s done", idx - 1, test_res[idx-1].resStr);
                        else
                            RK_LOGD("res index: 0, write %s done", test_res[0].resStr);
                    } else {
                        RK_LOGD("write %s/%s done", ctx->stDebugFile.aFilePath, ctx->stDebugFile.aFileName);
                    }

                    if (test_res[idx].size.u32Width > ctx->maxWidth ||
                        test_res[idx].size.u32Height > ctx->maxHeight) {
                        idx++;
                        goto __RETRY;
                    }
                } else {
                    RK_LOGD("res index: %d, write %s done", idx - 1, test_res[idx-1].resStr);
                    break;
                }

                ctx->stChnAttr.stSize.u32Width  = test_res[idx].size.u32Width;
                ctx->stChnAttr.stSize.u32Height = test_res[idx].size.u32Height;
                s32Ret = RK_MPI_VI_SetChnAttr(ctx->pipeId, ctx->channelId, &ctx->stChnAttr);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_VI_SetChnAttr %d x %d fail, ret: %x",
                            test_res[idx].size.u32Width, test_res[idx].size.u32Height, s32Ret);
                }
                else {
                    RK_LOGD("RK_MPI_VI_SetChnAttr %d x %d success",
                            test_res[idx].size.u32Width, test_res[idx].size.u32Height);

                    memcpy(&ctx->stDebugFile.aFilePath, "/data", strlen("/data"));
                    snprintf(ctx->stDebugFile.aFileName, MAX_VI_FILE_PATH_LEN,
                            "resolution_%d_%d_%d.bin", ctx->devId, test_res[idx].size.u32Width,
                            test_res[idx].size.u32Height);
                    if (ctx->stDebugFile.bCfg) {
                        s32Ret = RK_MPI_VI_ChnSaveFile(ctx->pipeId, ctx->channelId, &ctx->stDebugFile);
                        RK_LOGD("RK_MPI_VI_ChnSaveFile %x", s32Ret);
                    }
                }

                if (loopCount >= getTimesToChgRes)
                    idx++;
            }
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }

        usleep(1*1000);
    }

__FAILED:
    // if enable rgn,deinit it and destory it.
    if (ctx->bEnRgn) {
        destory_rgn(ctx);
    }

    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }


    // 9. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // 10.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    return s32Ret;
}

static RK_S32 test_vi_eptz_vi_only(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret;
    RK_S32 loopCount = 0;
    RK_S32 waitTime = 100;

    /* test use getframe&release_frame */
    s32Ret = test_vi_init(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vi %d:%d init failed:%x", ctx->devId, ctx->channelId, s32Ret);
        goto __FAILED;
    }

    while (loopCount < ctx->loopCountSet) {
        // 5.get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *data = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d", data, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef, ctx->stViFrame.stVFrame.u64PTS / 1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);
            // 6.get the channel status
            s32Ret = RK_MPI_VI_QueryChnStatus(ctx->pipeId, ctx->channelId, &ctx->stChnStatus);
            RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d," \
                    "current frame id:%d,input lost:%d,output lost:%d," \
                    "framerate:%d,vbfail:%d delay=%lldus",
                     s32Ret,
                     ctx->stChnStatus.stSize.u32Width,
                     ctx->stChnStatus.stSize.u32Height,
                     ctx->stChnStatus.bEnable,
                     ctx->stChnStatus.u32CurFrameID,
                     ctx->stChnStatus.u32InputLostFrame,
                     ctx->stChnStatus.u32OutputLostFrame,
                     ctx->stChnStatus.u32FrameRate,
                     ctx->stChnStatus.u32VbFail,
                     nowUs - ctx->stViFrame.stVFrame.u64PTS);
            // 7.release the frame
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->channelId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;

            {   // do eptz process
                RK_BOOL bDoCrop = RK_FALSE;
                VI_CROP_INFO_S stCropInfo;
                memset(&stCropInfo, 0, sizeof(VI_CROP_INFO_S));
                stCropInfo.bEnable = RK_TRUE;
                stCropInfo.enCropCoordinate = VI_CROP_ABS_COOR;
                RK_U32 x, y, w, h;

                switch (loopCount) {
                    case 10:    // crop to 1080p
                        x = 0; y = 0; w = 1920; h = 1080;
                        bDoCrop = RK_TRUE;
                    break;

                    case 30:    // crop to 720
                        x = 0; y = 0; w = 1280; h = 720;
                        bDoCrop = RK_TRUE;
                    break;

                    case 50:    // crop to 576p
                        x = 0; y = 0; w = 720;  h = 576;
                        bDoCrop = RK_TRUE;
                    break;

                    case 70:    // crop to 480p
                        x = 0; y = 0; w = 640;  h = 480;
                        bDoCrop = RK_TRUE;
                    break;
                }

                if (bDoCrop) {
                    stCropInfo.stCropRect.s32X = x;
                    stCropInfo.stCropRect.s32Y = y;
                    stCropInfo.stCropRect.u32Width  = w;
                    stCropInfo.stCropRect.u32Height = h;
                    if (ctx->bEptz) {
                        RK_LOGD("<<< change eptz and set start >>>");
                        RK_LOGD("------ Crop to: w x h (%d x %d) ------", w, h);
                        RK_MPI_VI_SetEptz(ctx->pipeId, ctx->channelId, stCropInfo);
                        RK_LOGD("<<< change eptz and set end >>>");
                    }

                    bDoCrop = RK_FALSE;
                }
            }
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }

        usleep(10*1000);
    }

    {
        /* restore crops setting as beginning resolution,
           avoiding getting video frame fail when run again.
        */
        VI_CROP_INFO_S stCropInfo;
        if (ctx->bEptz) {
            RK_MPI_VI_GetEptz(ctx->pipeId, ctx->channelId, &stCropInfo);
            if (stCropInfo.stCropRect.u32Width != ctx->width ||
                    stCropInfo.stCropRect.u32Height != ctx->height) {
                stCropInfo.stCropRect.u32Width  = ctx->width;
                stCropInfo.stCropRect.u32Height = ctx->height;
                RK_LOGD("<<< change eptz and set start >>>");
                RK_LOGD("------ Crop to: w x h (%d x %d) ------", ctx->width, ctx->height);
                RK_MPI_VI_SetEptz(ctx->pipeId, ctx->channelId, stCropInfo);
                RK_LOGD("<<< change eptz and set end >>>");
            }
        }
    }

__FAILED:
    // if enable rgn,deinit it and destory it.
    if (ctx->bEnRgn) {
        destory_rgn(ctx);
    }

    if (ctx->bAttachPool) {
        RK_MPI_VI_DetachMbPool(ctx->pipeId, ctx->channelId);
        RK_MPI_MB_DestroyPool(ctx->attachPool);
    }

    // 9. disable one chn
    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->channelId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    // 10.disable dev(will diabled all chn)
    s32Ret = RK_MPI_VI_DisableDev(ctx->devId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
    return s32Ret;
}

static const char *const usages[] = {
    "\t1)./rk_mpi_vi_test -w 1920 -h 1080 -t 4 -c 0 -d 0 -m 0 -l 10 -o 1",
    "\t10)./rk_mpi_vi_test -w 1920 -h 1080 --maxWidth 2560 --maxHeight 1440 -c 0 -m 10 -l 100 -o 1",
    "\t11)./rk_mpi_vi_test -w 1920 -h 1080 --maxWidth 2560 --maxHeight -c 0 -m 11 -l 100 -o 1",
    "\t12)./rk_mpi_vi_test -w 1920 -h 1080 -c 0 -d 0 -m 12",
    "\t13)./rk_mpi_vi_test -w 1920 -h 1080 -d 0 -m 13 -l 100",
    "\t14)./rk_mpi_vi_test -w 2880 -h 1616 -c 0 -d 0 -m 14 --en_eptz 1 -o 1",
    "",
    RK_NULL,
};

static void *QuerySensorState(void *arg) {
    int s32Ret;
    int loopCnt = 0;
    TEST_VI_CTX_S *ctx = (TEST_VI_CTX_S*)arg;

    while (!bquit) {
        s32Ret = RK_MPI_VI_QueryDevStatus(ctx->devId, &ctx->stDevStatus);
        if (s32Ret == RK_SUCCESS) {
            if (!ctx->stDevStatus.bProbeOk) {
                RK_LOGE("sensor probe fail");
            }
        }
        loopCnt++;
        usleep(10*1000);
    }
    return NULL;
}

int main(int argc, const char **argv) {
    RK_S32 i;
    RK_S32 s32Ret = RK_FAILURE;
    TEST_VI_CTX_S *ctx;
    ctx = reinterpret_cast<TEST_VI_CTX_S *>(malloc(sizeof(TEST_VI_CTX_S)));
    memset(ctx, 0, sizeof(TEST_VI_CTX_S));

    ctx->width = 0;
    ctx->height = 0;
    ctx->devId = 0;
    ctx->pipeId = ctx->devId;
    ctx->channelId = 1;
    ctx->loopCountSet = 100;
    ctx->enMode = TEST_VI_MODE_BIND_VENC;
    ctx->stChnAttr.stIspOpt.u32BufCount = 3;
    ctx->stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
    ctx->stChnAttr.stIspOpt.bNoUseLibV4L2 = RK_TRUE;
    ctx->stChnAttr.u32Depth = 0;
    ctx->aEntityName = RK_NULL;
    ctx->stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
    ctx->stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    ctx->stChnAttr.stFrameRate.s32DstFrameRate = -1;
    ctx->u32DstCodec = RK_VIDEO_ID_HEVC;
    ctx->vencChId = 0;
    ctx->s32Snap = 1;

    ctx->bEnRgn = RK_FALSE;
    ctx->bEnOverlay = RK_FALSE;
    ctx->s32RgnCnt = 1;
    ctx->rgnType = RGN_BUTT;
    ctx->u32BitRateKb = 10 * 1024;
    ctx->u32BitRateKbMin = 10 * 1024;
    ctx->u32BitRateKbMax = 10 * 1024;
    ctx->bAttachPool = RK_FALSE;
    ctx->u32DelayMsGet = 0;
    ctx->u32GopSize = 60;
    ctx->u32ChangeRes = 20;

    ctx->maxWidth = 0;
    ctx->maxHeight = 0;

    ctx->bGetStream = RK_TRUE;
    ctx->bSecondThr = RK_FALSE;
    ctx->bRgnOnPipe = RK_FALSE;  // default rgn(cover/mosaic) effect on channel
    RK_LOGD("test running enter!");

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('w', "width", &(ctx->width),
                    "set capture channel width(required, default 0)", NULL, 0, 0),
        OPT_INTEGER('h', "height", &(ctx->height),
                    "set capture channel height(required, default 0)", NULL, 0, 0),
        OPT_INTEGER('d', "dev", &(ctx->devId),
                    "set dev id(default 0)", NULL, 0, 0),
        OPT_INTEGER('p', "pipe", &(ctx->pipeId),
                    "set pipe id(default 0)", NULL, 0, 0),
        OPT_INTEGER('c', "channel", &(ctx->channelId),
                    "set channel id(default 1)", NULL, 0, 0),
        OPT_INTEGER('l', "loopcount", &(ctx->loopCountSet),
                    "set capture frame count(default 100)", NULL, 0, 0),
        OPT_INTEGER('C', "compressmode", &(ctx->enCompressMode),
                    "set capture compressmode(default 0; 0:MODE_NONE 1:AFBC_16x16)", NULL, 0, 0),
        OPT_INTEGER('o', "output", &(ctx->stDebugFile.bCfg),
                    "save output file, file at /data/test_<devid>_<pipeid>_<channelid>.bin"
                    " (default 0; 0:no save 1:save)", NULL, 0, 0),
        OPT_INTEGER('m', "mode", &(ctx->enMode),
                    "test mode(default 1; \n\t"
                    "0:vi get&release frame \n\t"
                    "1:vi bind one venc(h264) \n\t"
                    "2:vi bind two venc(h264)) \n\t"
                    "3:vi bind vpss bind venc \n\t"
                    "4:vi bind vo \n\t"
                    "5:mult vi \n\t"
                    "6:vi get send to venc \n\t"
                    "7:vi get send to vo \n\t"
                    "8:vi bind vpss bind vo \n\t"
                    "9:vi bind vpss send vo \n\t"
                    "10:vi change resolution \n\t"
                    "11:vi bind venc & change resolution \n\t"
                    "12:vi bind venc with thumbnail \n\t"
                    "13:vi one sensor, muti-chns get video frames \n\t"
                    "14:vi set eptz, get & release frame and set eptz \n\t",
                    NULL, 0, 0),
        OPT_INTEGER('t', "memorytype", &(ctx->stChnAttr.stIspOpt.enMemoryType),
                    "set the buf memorytype(required, default 4; 1:mmap(hdmiin/bt1120/sensor input) "
                    "2:userptr(invalid) 3:overlay(invalid) 4:dma(sensor))", NULL, 0, 0),
        OPT_STRING('n', "name", &(ctx->aEntityName),
                   "set the entityName (required, default null;\n\t"
                   "rv1126 sensor:rkispp_m_bypass rkispp_scale0 rkispp_scale1 rkispp_scale2;\n\t"
                   "rv1126 hdmiin/bt1120/sensor:/dev/videox such as /dev/video19 /dev/video20;\n\t"
                   "rk356x hdmiin/bt1120/sensor:/dev/videox such as /dev/video0 /dev/video1", NULL, 0, 0),
        OPT_INTEGER('D', "depth", &(ctx->stChnAttr.u32Depth),
                    "channel output depth, default{u32BufCount(not bind) or 0(bind venc/vpss/...)}", NULL, 0, 0),
        OPT_INTEGER('f', "format", &(ctx->stChnAttr.enPixelFormat),
                    "set the format(default 0; 0:RK_FMT_YUV420SP 10:RK_FMT_YUV422_UYVY"
                    "131080:RK_FMT_RGB_BAYER_SBGGR_12BPP)", NULL, 0, 0),
        OPT_INTEGER('\0', "codec", &(ctx->u32DstCodec),
                     "venc encode codec(8:h264, 9:mjpeg, 12:h265, 15:jpeg, ...). default(12)", NULL, 0, 0),
        OPT_INTEGER('\0', "freeze", &(ctx->bFreeze),
                    "freeze output(default 0; 0:disable freeze 1:enable freeze)", NULL, 0, 0),
        OPT_INTEGER('\0', "src_rate", &(ctx->stChnAttr.stFrameRate.s32SrcFrameRate),
                    "src frame rate(default -1; -1:not control; other:1-max_fps<isp_out_fps>)", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_rate", &(ctx->stChnAttr.stFrameRate.s32DstFrameRate),
                    "dst frame rate(default -1; -1:not control; other:1-src_fps<src_rate>)", NULL, 0, 0),
        OPT_INTEGER('\0', "buf_count", &(ctx->stChnAttr.stIspOpt.u32BufCount),
                    "out buf count, range[1-8] default(3)", NULL, 0, 0),
        OPT_INTEGER('\0', "venc_ch_start", &(ctx->vencChId),
                    "venc chn id start", NULL, 0, 0),
        OPT_INTEGER('\0', "wrap", &(ctx->stChnWrap.bEnable),
                    "wrap enable(0: disable, 1: enable). default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "wrap_line", &(ctx->stChnWrap.u32BufLine),
                    "set wrap line, range [128-H]", NULL, 0, 0),
        OPT_INTEGER('\0', "rgn_type", &(ctx->rgnType),
                    "rgn type. 0:overlay, 1:overlayEx,2:cover,3:coverEx,4:mosaic,5:moscaiEx", NULL, 0, 0),
        OPT_INTEGER('\0', "rgn_cnt", &(ctx->s32RgnCnt),
                    "rgn count. default(1),max:8", NULL, 0, 0),
        OPT_INTEGER('\0', "en_rgn", &(ctx->bEnRgn),
                    "enable rgn. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "en_overlay", &(ctx->bEnOverlay),
                    "enable overlay. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "mirror", &(ctx->bMirror),
                    "picture mirror, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "flip", &(ctx->bFlip),
                    "picture flip, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "maxWidth", &(ctx->maxWidth),
                    "config max resolution width(<= sensor max resolution width)", NULL, 0, 0),
        OPT_INTEGER('\0', "maxHeight", &(ctx->maxHeight),
                    "config max resolution height(<= sensor max resolution height)", NULL, 0, 0),
        OPT_INTEGER('\0', "combo", &(ctx->bCombo),
                    "combo enable(0: disable, 1: enable). default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "snap", &(ctx->s32Snap),
                    "snap jpeg num(-1: save all, > 0: save pic num). default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "venc_ref_buf_share", &(ctx->bRefBufShare),
                    "enable venc ref buf share or not, default(0), 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "de_breath", &(ctx->u32DeBreath),
                    "debreath[0, 35] default(0)", NULL, 0, 0),
        OPT_INTEGER('b', "bit_rate", &(ctx->u32BitRateKb),
                    "bit rate kbps(h264/h265:range[3-200000],jpeg/mjpeg:range[5-800000] default(10*1024kb))",
                    NULL, 0, 0),
        OPT_INTEGER('\0', "bit_rate_max", &(ctx->u32BitRateKbMax),
                    "bit rate kbps max(vbr/avbr valid)(h264/h265:range[3-200000];"
                    "jpeg/mjpeg:range[5-800000] default(0:auto calcu))",
                     NULL, 0, 0),
        OPT_INTEGER('\0', "bit_rate_min", &(ctx->u32BitRateKbMin),
                    "bit rate kbps min(vbr/avbr valid)(h264/h265:range[3-200000];"
                    "jpeg/mjpeg:range[5-800000] default(0:auto calcu))",
                     NULL, 0, 0),
        OPT_INTEGER('\0', "attach_pool", &(ctx->bAttachPool),
                    "enable attach usr pool, default(0), 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "ivs", &(ctx->u32Ivs),
                    "ivs(MD, OD) enable, default(0: disable, 1: low, 2: mid, 3: high)", NULL, 0, 0),
        OPT_INTEGER('\0', "ivs_debug", &(ctx->bIvsDebug),
                    "ivs debug enable, default(0: disable, 1: enable)", NULL, 0, 0),
        OPT_INTEGER('\0', "no_use_v4l2", &(ctx->stChnAttr.stIspOpt.bNoUseLibV4L2),
                    "enable use v4l2, default(0), 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "delay_get", &(ctx->u32DelayMsGet),
                    "delay some ms get, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "svc", &(ctx->bSvc),
                    "svc(smart video coding) enable, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "gop_size", &(ctx->u32GopSize),
                    "gop size(range >= 1 default(60))", NULL, 0, 0),
        OPT_INTEGER('\0', "change_res", &(ctx->u32ChangeRes),
                    "change res by frames(default(20))", NULL, 0, 0),
        OPT_INTEGER('\0', "getstream", &(ctx->bGetStream),
                    "ebable get vi stream,default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "en_thr", &(ctx->bSecondThr),
                    "ebable second thread to get vi stream,default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "en_eptz", &(ctx->bEptz),
                    "enable eptz (default(0))", NULL, 0, 0),
        OPT_INTEGER('e', "use_ext", &(ctx->bUseExt),
                    "enable use channel ext api and pipe (default(0))", NULL, 0, 0),
        OPT_INTEGER('\0', "rgn_pipe", &(ctx->bRgnOnPipe),
                    "enable rgn on pipe (default(0))", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");
    argc = argparse_parse(&argparse, argc, argv);

    if (!ctx->width || !ctx->height) {
        argparse_usage(&argparse);
        goto __FAILED2;
    }

    if (ctx->pipeId != ctx->devId)
        ctx->pipeId = ctx->devId;

    if (ctx->stDebugFile.bCfg) {
        if (ctx->enMode == TEST_VI_MODE_BIND_VENC || ctx->enMode == TEST_VI_MODE_BIND_VPSS_BIND_VENC
            || ctx->enMode == TEST_VI_GET_SEND_VENC || ctx->enMode == TEST_VI_BIND_VENC_CHANGE_RESOLUTION
            || ctx->enMode == TEST_VI_BIND_VENC_WITH_THUMBNAIL) {
            ctx->stVencCfg[0].bOutDebugCfg = ctx->stDebugFile.bCfg;
        } else if (ctx->enMode == TEST_VI_MODE_BIND_VENC_MULTI) {
            ctx->stVencCfg[0].bOutDebugCfg = ctx->stDebugFile.bCfg;
            ctx->stVencCfg[1].bOutDebugCfg = ctx->stDebugFile.bCfg;
        }
        if (ctx->enMode == TEST_VI_MODE_BIND_VPSS_SEND_VO)
            ctx->stVpssCfg.bOutDebugCfg = ctx->stDebugFile.bCfg;
        memcpy(&ctx->stDebugFile.aFilePath, "/data", strlen("/data"));
        snprintf(ctx->stDebugFile.aFileName, MAX_VI_FILE_PATH_LEN,
                 "test_%d_%d_%d.bin", ctx->devId, ctx->pipeId, ctx->channelId);
    }
    if (ctx->stVpssCfg.bOutDebugCfg) {
            char name[256] = {0};
            memcpy(&ctx->stVpssCfg.dstFilePath, "data", strlen("data"));
            snprintf(ctx->stVpssCfg.dstFileName, sizeof(ctx->stVencCfg[i].dstFileName),
                   "vpss_%d.bin", i);
            snprintf(name, sizeof(name), "/%s/%s",
                     ctx->stVpssCfg.dstFilePath, ctx->stVpssCfg.dstFileName);
            ctx->stVpssCfg.fp = fopen(name, "wb");
            if (ctx->stVpssCfg.fp == RK_NULL) {
                RK_LOGE("chn %d can't open file %s in get picture thread!\n", i, name);
                goto __FAILED;
            }
    }

    RK_LOGE("test running enter ctx->aEntityName=%s!", ctx->aEntityName);
    if (ctx->aEntityName != RK_NULL)
        memcpy(ctx->stChnAttr.stIspOpt.aEntityName, ctx->aEntityName, strlen(ctx->aEntityName));

    if (ctx->pipeId != ctx->devId)
        ctx->pipeId = ctx->devId;

    mpi_vi_test_show_options(ctx);

    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        RK_LOGE("rk mpi sys init fail!");
        goto __FAILED;
    }
    switch (ctx->enMode) {
        case TEST_VI_MODE_VI_ONLY:
            signal(SIGINT, sigterm_handler);
            if (!ctx->stChnAttr.u32Depth) {
                RK_LOGE("depth need > 0 when vi not bind any other module!");
                ctx->stChnAttr.u32Depth = ctx->stChnAttr.stIspOpt.u32BufCount;
            }
            s32Ret = test_vi_get_release_frame_loop(ctx);
            /* below test for query sensor state */
            // FIXME(@chad): Now querying the online status of the sensor is not perfect.
            // It is not possible to obtain in real time whether the current sensor is in
            // a physically disconnected state for now.
            #if 0
            pthread_t query_thread;
            pthread_create(&query_thread, RK_NULL, QuerySensorState, (void*)ctx);
            pthread_join(query_thread, RK_NULL);
            #endif
        break;
        case TEST_VI_MODE_BIND_VENC:
        case TEST_VI_MODE_BIND_VENC_MULTI:
            s32Ret = test_vi_bind_venc_loop(ctx);
        break;
        case TEST_VI_MODE_BIND_VPSS_BIND_VENC:
            s32Ret = test_vi_bind_vpss_venc_loop(ctx);
        break;
        case TEST_VI_MODE_BIND_VO:
            s32Ret = test_vi_bind_vo_loop(ctx);
        break;
        case TEST_VI_MODE_MULTI_VI:     // rk356x / rk3588
            s32Ret = test_vi_multi_sensor_vi_loop(ctx);
            break;
        case TEST_VI_GET_SEND_VENC:
            s32Ret = test_vi_venc_get_send_loop(ctx);
            break;
        case TEST_VI_GET_SEND_VO:
            s32Ret = test_vi_vo_get_send_loop(ctx);
            break;
        case TEST_VI_MODE_BIND_VPSS_BIND_VO:
            s32Ret = test_vi_bind_vpss_vo_loop(ctx);
        break;
        case TEST_VI_MODE_BIND_VPSS_SEND_VO:
            s32Ret = test_vi_bind_vpss_send_vo_loop(ctx);
        break;
        case TEST_VI_CHANGE_RESOLUTION:
            s32Ret = test_vi_change_resolution(ctx);
        break;
        case TEST_VI_BIND_VENC_CHANGE_RESOLUTION:
            s32Ret = test_vi_bind_venc_change_resolution(ctx);
        break;
        case TEST_VI_BIND_VENC_WITH_THUMBNAIL:
            s32Ret = test_vi_bind_venc_with_thumbnail(ctx);
        break;
        case TEST_VI_MODE_MULTI_CHN:    // rv1106/ 1103
            s32Ret = test_vi_one_sensor_multi_chn_loop(ctx);
        break;
        case TEST_VI_EPTZ_VI_ONLY:
            s32Ret = test_vi_eptz_vi_only(ctx);
        break;
        default:
            RK_LOGE("no support such test mode:%d", ctx->enMode);
        break;
    }
__FAILED:
    RK_LOGE("test running exit:%d", s32Ret);
    RK_MPI_SYS_Exit();
__FAILED2:
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }

    return 0;
}
