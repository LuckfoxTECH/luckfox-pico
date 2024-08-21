/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance: the License.
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
#undef DBG_MOD_ID
#define DBG_MOD_ID       RK_ID_AVS

#include <string>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>

#include "rk_mpi_avs.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_mmz.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vpss.h"
#ifdef HAVE_VO
#include "rk_mpi_vo.h"
#endif
#include "rk_mpi_sys.h"

#include "test_common.h"
#include "test_comm_argparse.h"
#include "test_comm_avs.h"
#include "test_comm_vpss.h"
#include "test_comm_sys.h"
#include "test_comm_utils.h"

/* for RK3588 */
#define RK3588_VO_DEV_HDMI         0
#define RK3588_VO_DEV_MIPI         3

static RK_BOOL bExit = RK_FALSE;

typedef struct rkVPSS_CFG_S {
    VPSS_GRP        VpssGrp;
    RK_U32          u32VpssChnCnt;
    VPSS_GRP_ATTR_S stGrpVpssAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
} VPSS_CFG_S;

typedef struct rkVENC_CFG_S {
    VENC_CHN VencChn;
    VENC_CHN_ATTR_S stAttr;
} VENC_CFG_S;

#ifdef HAVE_VO
typedef struct _rkVO_CFG_S {
    VO_DEV                s32DevId;
    VO_CHN                s32ChnId;
    VO_LAYER              s32LayerId;
    VO_VIDEO_LAYER_ATTR_S stVoLayerAttr;
    VO_CHN_ATTR_S         stVoChnAttr;
    VO_CSC_S              stVoCscAttr;
} VO_CFG_S;
#endif

static RK_S32 create_venc(VENC_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S   stRecvParam;

    memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    stRecvParam.s32RecvPicNum = -1;

    s32Ret = RK_MPI_VENC_CreateChn(ctx->VencChn, &ctx->stAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("venc [%d] RK_MPI_VENC_CreateChn failed: %#x!",
                ctx->VencChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("venc [%d] RK_MPI_VENC_CreateChn already.", ctx->VencChn);

    s32Ret = RK_MPI_VENC_StartRecvFrame(ctx->VencChn, &stRecvParam);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("venc [%d] RK_MPI_VENC_StartRecvFrame failed: %#x!",
                ctx->VencChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("venc [%d] RK_MPI_VENC_StartRecvFrame already.", ctx->VencChn);

    return RK_SUCCESS;
}

static RK_S32 destroy_venc(VENC_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VENC_StopRecvFrame(ctx->VencChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("venc [%d] RK_MPI_VENC_StopRecvFrame failed: %#x!",
                ctx->VencChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("venc [%d] RK_MPI_VENC_StopRecvFrame already.", ctx->VencChn);

    s32Ret = RK_MPI_VENC_DestroyChn(ctx->VencChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("venc [%d] RK_MPI_VENC_DestroyChn failed: %#x!",
                ctx->VencChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("venc [%d] RK_MPI_VENC_DestroyChn already.", ctx->VencChn);

    return RK_SUCCESS;
}

#ifdef HAVE_VO

static RK_S32 create_vo(VO_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_U32 u32DispBufLen;
    VO_PUB_ATTR_S VoPubAttr;
    VO_LAYER VoLayer = ctx->s32LayerId;
    VO_DEV VoDev = ctx->s32DevId;
    VO_CHN VoChn = ctx->s32ChnId;

    memset(&VoPubAttr, 0, sizeof(VO_PUB_ATTR_S));

    s32Ret = RK_MPI_VO_GetPubAttr(VoDev, &VoPubAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_GetPubAttr failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_GetPubAttr already.",
            VoDev, VoLayer, VoChn);

    if (RK3588_VO_DEV_HDMI == VoDev) {
        VoPubAttr.enIntfType = VO_INTF_HDMI;
        VoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
    } else if (RK3588_VO_DEV_MIPI == VoDev) {
        VoPubAttr.enIntfType = VO_INTF_MIPI;
        VoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;
    }

    s32Ret = RK_MPI_VO_SetPubAttr(VoDev, &VoPubAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_GetPubAttr failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_SetPubAttr already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_Enable(VoDev);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_Enable failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_Enable already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_GetLayerDispBufLen(VoLayer, &u32DispBufLen);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_GetLayerDispBufLen failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_GetLayerDispBufLen already.",
            VoDev, VoLayer, VoChn);

    u32DispBufLen = 3;
    s32Ret = RK_MPI_VO_SetLayerDispBufLen(VoLayer, u32DispBufLen);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_GetLayerDispBufLen %d failed: %#x!",
                VoDev, VoLayer, VoChn, u32DispBufLen, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_GetLayerDispBufLen %d already.",
            VoDev, VoLayer, VoChn, u32DispBufLen);

    s32Ret = RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_GRAPHIC);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_BindLayer failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_BindLayer already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_SetLayerAttr(VoLayer, &ctx->stVoLayerAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_SetLayerAttr failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_SetLayerAttr already.",
            VoDev, VoLayer, VoChn);

#if VO_RGA
    s32Ret = RK_MPI_VO_SetLayerSpliceMode(VoLayer, VO_SPLICE_MODE_RGA);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_SetLayerSpliceMode failed: %#x", s32Ret);
        return RK_FAILURE;
    }
#endif

    s32Ret = RK_MPI_VO_EnableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_EnableLayer failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_EnableLayer already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_SetChnAttr(VoLayer, VoChn, &ctx->stVoChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_SetChnAttr failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_SetChnAttr already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_SetLayerCSC(VoLayer, &ctx->stVoCscAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_SetChnAttr failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_SetChnAttr already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_EnableChn(VoLayer, VoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_EnableChn failed: %#x!",
                VoDev, VoLayer, VoChn, s32Ret);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_EnableChn already.",
            VoDev, VoLayer, VoChn);

    return s32Ret;
}

static RK_S32 destroy_vo(VO_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VO_LAYER VoLayer = ctx->s32LayerId;
    VO_DEV VoDev = ctx->s32DevId;
    VO_CHN VoChn = ctx->s32ChnId;

    s32Ret = RK_MPI_VO_DisableChn(VoDev, VoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_DisableChn failed: %#x!",
                VoDev, VoLayer, VoChn);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_DisableChn already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_DisableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_DisableLayer failed: %#x!",
                VoDev, VoLayer, VoChn);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_DisableLayer already.",
            VoDev, VoLayer, VoChn);

    s32Ret = RK_MPI_VO_Disable(VoDev);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] RK_MPI_VO_Disable failed: %#x!",
                VoDev, VoLayer, VoChn);
        return s32Ret;
    }
    RK_LOGV("vo [%d, %d, %d] RK_MPI_VO_Disable already.",
            VoDev, VoLayer, VoChn);

    return s32Ret;
}
#endif

static RK_S32 test_single_avs_loop(TEST_AVS_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = TEST_AVS_UnitTest(ctx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("TEST_AVS_UnitTest failed: %#x!", s32Ret);
        goto __FAILED;
    }

__FAILED:

    return s32Ret;
}

static RK_VOID sigterm_handler(int sig) {
    RK_PRINT("signal %d\n", sig);
    bExit = RK_TRUE;
}

static RK_S32 test_avs_vpss_venc_loop(TEST_AVS_CTX_S *pstAvsCtx) {
    RK_PRINT(" Media link: \n");
    RK_PRINT(" %d pool -> avs -> vpss -> venc(H264) \n", pstAvsCtx->s32PipeNum);
    RK_PRINT("                     -> venc(H265) \n");

    RK_S32 s32Ret     = RK_SUCCESS;
    RK_S32 loopCount  = 0;
    RK_S32 vpssChnCnt = 1;
    RK_S32 vencChnCnt = vpssChnCnt;
    RK_S32 vpssOutputWidth[vpssChnCnt] = {3840};
    RK_S32 vpssOutputHeight[vpssChnCnt] = {2160};

    VPSS_CFG_S  pstVpssCtx;
    VENC_CFG_S *pstVencCtx;
    VIDEO_FRAME_INFO_S **pstPipeFrames;

    pstVencCtx = reinterpret_cast<VENC_CFG_S *>
                                (malloc(sizeof(VENC_CFG_S) * vencChnCnt));
    memset(pstVencCtx, 0, sizeof(VENC_CFG_S) * vencChnCnt);
    pstPipeFrames = reinterpret_cast<VIDEO_FRAME_INFO_S **>(
                        malloc(sizeof(VIDEO_FRAME_INFO_S *) * AVS_PIPE_NUM));
    for (RK_S32 i = 0; i < AVS_PIPE_NUM; i++) {
        pstPipeFrames[i] = reinterpret_cast<VIDEO_FRAME_INFO_S *>(malloc(sizeof(VIDEO_FRAME_INFO_S)));
        memset(pstPipeFrames[i], 0, sizeof(VIDEO_FRAME_INFO_S));
    }

    s32Ret = TEST_AVS_ModCreateFramePool(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FREE_PIPE_FRAME;
    }

    /* avs create */
    s32Ret = TEST_AVS_ModInit(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __DESTROY_FRAME_POOL;
    }

    /* vpss config init */
    pstVpssCtx.VpssGrp                                   = 0;
    pstVpssCtx.u32VpssChnCnt                             = vpssChnCnt;
    pstVpssCtx.stGrpVpssAttr.u32MaxW                     = 8192;
    pstVpssCtx.stGrpVpssAttr.u32MaxH                     = 8192;
    pstVpssCtx.stGrpVpssAttr.enPixelFormat               = RK_FMT_YUV420SP;
    pstVpssCtx.stGrpVpssAttr.stFrameRate.s32SrcFrameRate = -1;
    pstVpssCtx.stGrpVpssAttr.stFrameRate.s32DstFrameRate = -1;
    pstVpssCtx.stGrpVpssAttr.enCompressMode              = COMPRESS_MODE_NONE;
    for (RK_S32 i = 0; i < vpssChnCnt; i ++) {
        pstVpssCtx.stVpssChnAttr[i].enChnMode                   = VPSS_CHN_MODE_USER;
        pstVpssCtx.stVpssChnAttr[i].enDynamicRange              = DYNAMIC_RANGE_SDR8;
        pstVpssCtx.stVpssChnAttr[i].enPixelFormat               = RK_FMT_YUV420SP;
        pstVpssCtx.stVpssChnAttr[i].stFrameRate.s32SrcFrameRate = -1;
        pstVpssCtx.stVpssChnAttr[i].stFrameRate.s32DstFrameRate = -1;
        pstVpssCtx.stVpssChnAttr[i].u32Width                    = vpssOutputWidth[i];
        pstVpssCtx.stVpssChnAttr[i].u32Height                   = vpssOutputHeight[i];
        pstVpssCtx.stVpssChnAttr[i].enCompressMode              = COMPRESS_AFBC_16x16;
        pstVpssCtx.stVpssChnAttr[i].u32FrameBufCnt              = 3;
        pstVpssCtx.stVpssChnAttr[i].u32Depth                    = 1;
    }

    /* vpss create */
    s32Ret = TEST_VPSS_Start(pstVpssCtx.VpssGrp, pstVpssCtx.u32VpssChnCnt,
                        &pstVpssCtx.stGrpVpssAttr, pstVpssCtx.stVpssChnAttr);
    if (s32Ret != RK_SUCCESS) {
        goto __DESTROY_AVS;
    }

    for (RK_S32 i = 0; i < vencChnCnt; i++) {
        /* venc config init */
        PIC_BUF_ATTR_S   stPicBufAttr;
        MB_PIC_CAL_S     stMbPicCalResult;

        stPicBufAttr.u32Width      = vpssOutputWidth[i];
        stPicBufAttr.u32Height     = vpssOutputHeight[i];
        stPicBufAttr.enCompMode    = COMPRESS_AFBC_16x16;
        stPicBufAttr.enPixelFormat = RK_FMT_YUV420SP;

        s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("get picture buffer size failed: %#x!", s32Ret);
            goto __DESTROY_VENC;
        }

        pstVencCtx[i].VencChn = i;
        pstVencCtx[i].stAttr.stVencAttr.enPixelFormat   = RK_FMT_YUV420SP;
        pstVencCtx[i].stAttr.stVencAttr.u32PicWidth     = vpssOutputWidth[i];
        pstVencCtx[i].stAttr.stVencAttr.u32PicHeight    = vpssOutputHeight[i];
        pstVencCtx[i].stAttr.stVencAttr.u32VirWidth     = vpssOutputWidth[i];
        pstVencCtx[i].stAttr.stVencAttr.u32VirHeight    = vpssOutputHeight[i];
        pstVencCtx[i].stAttr.stVencAttr.u32StreamBufCnt = 3;
        pstVencCtx[i].stAttr.stVencAttr.u32BufSize      = stMbPicCalResult.u32MBSize;
        if (i % 2 == 0) {
            pstVencCtx[i].stAttr.stVencAttr.enType             = RK_VIDEO_ID_AVC;
            pstVencCtx[i].stAttr.stVencAttr.u32Profile         = H264E_PROFILE_HIGH;
            pstVencCtx[i].stAttr.stRcAttr.enRcMode             = VENC_RC_MODE_H264CBR;
            pstVencCtx[i].stAttr.stRcAttr.stH264Cbr.u32Gop     = 60;
            pstVencCtx[i].stAttr.stRcAttr.stH264Cbr.u32BitRate = 65536;
        } else {
            pstVencCtx[i].stAttr.stVencAttr.enType             = RK_VIDEO_ID_HEVC;
            pstVencCtx[i].stAttr.stVencAttr.u32Profile         = H265E_PROFILE_MAIN;
            pstVencCtx[i].stAttr.stRcAttr.enRcMode             = VENC_RC_MODE_H265CBR;
            pstVencCtx[i].stAttr.stRcAttr.stH265Cbr.u32Gop     = 60;
            pstVencCtx[i].stAttr.stRcAttr.stH264Cbr.u32BitRate = 16384;
        }
        /* venc create */
        s32Ret = create_venc(&pstVencCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            goto __DESTROY_VENC;
        }
    }

    /* bind avs -> vpss */
    s32Ret = TEST_SYS_AvsBindVpss(0, 0, pstVpssCtx.VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind failed: %x, avs [%d, %d] -> vpss [%d]",
                s32Ret, 0, 0, pstVpssCtx.VpssGrp);
        goto __DESTROY_VENC;
    }
    RK_LOGV("bind success, avs [%d, %d] -> vpss [%d]",
            0, 0, pstVpssCtx.VpssGrp);

    /* bind vpss -> venc */
    for (RK_S32 i = 0; i < vpssChnCnt; i++) {
        s32Ret = TEST_SYS_VpssBindVenc(0, i, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind failed: %x, vpss [%d, %d] -> venc [%d]",
                    s32Ret, 0, i, i);
            goto __UNBIND_AVS_VPSS;
        }
        RK_LOGV("bind success, vpss [%d, %d] -> venc [%d]",
                0, i, i);
    }

    /* thread: do somethings */
    signal(SIGINT, sigterm_handler);
    if (pstAvsCtx->s32LoopCount > 0) {
        VENC_STREAM_S stVencStream;
        stVencStream.pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));

        while (!bExit && loopCount < pstAvsCtx->s32LoopCount) {
            s32Ret = TEST_AVS_ModGetFrameFromPool(pstAvsCtx, pstPipeFrames);

            s32Ret = TEST_AVS_ModSendFrame(pstAvsCtx->s32GrpIndex, pstAvsCtx->s32PipeNum, pstPipeFrames);
            if (s32Ret != RK_SUCCESS) {
                TEST_AVS_ModReleaseFrameToPool(pstAvsCtx, pstPipeFrames);
                continue;
            }

            TEST_AVS_ModReleaseFrameToPool(pstAvsCtx, pstPipeFrames);

            for (RK_S32 i = 0; i < vencChnCnt; i++) {
                s32Ret = RK_MPI_VENC_GetStream(i, &stVencStream, 100);
                if (s32Ret >= 0) {
                    RK_MPI_VENC_ReleaseStream(i, &stVencStream);
                }
            }

            usleep(33 * 1000);
            loopCount++;
        }

        if (stVencStream.pstPack) {
            free(stVencStream.pstPack);
        }
    }

__UNBIND_VPSS_VENC:
    /* unbind avs -> vpss */
    s32Ret = TEST_SYS_AvsUnbindVpss(0, 0, pstVpssCtx.VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind failed: %x, avs [%d, %d] -> vpss [%d]",
                s32Ret, 0, 0, pstVpssCtx.VpssGrp);
    }
    RK_LOGV("unbind success, avs [%d, %d] -> vpss [%d]",
            0 , 0, pstVpssCtx.VpssGrp);

__UNBIND_AVS_VPSS:
    /* unbind vpss -> venc */
    for (RK_S32 i = 0; i < vpssChnCnt; i++) {
        s32Ret = TEST_SYS_VpssUnbindVenc(0, i, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("unbind failed: %x, vpss [%d, %d] -> venc [%d]",
                    s32Ret, pstVpssCtx.VpssGrp, i, i);
        }
        RK_LOGV("unbind success, vpss [%d, %d] -> venc [%d]",
                pstVpssCtx.VpssGrp, i, i);
    }

__DESTROY_VENC:
    /* destroy venc */
    for (RK_S32 i = 0; i < vencChnCnt; i++) {
        s32Ret = destroy_venc(&pstVencCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("venc [%d] destroy_venc failed: %#x", i, s32Ret);
        }
        RK_LOGV("venc [%d] destroy_venc already.", i);
    }

__DESTROY_VPSS:
    /* destroy vpss */
    s32Ret = TEST_VPSS_Stop(pstVpssCtx.VpssGrp, pstVpssCtx.u32VpssChnCnt);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vpss [%d] TEST_VPSS_Stop failed: %#x", pstVpssCtx.VpssGrp, s32Ret);
    }
    RK_LOGV("vpss [%d] TEST_VPSS_Stop already.", pstVpssCtx.VpssGrp);

__DESTROY_AVS:
    /* destroy avs */
    s32Ret = TEST_AVS_ModDeInit(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d] TEST_AVS_ModDeInit failed: %#x", pstAvsCtx->s32GrpIndex, s32Ret);
    }
    RK_LOGV("avs [%d] TEST_AVS_ModDeInit already.", pstAvsCtx->s32GrpIndex);

__DESTROY_FRAME_POOL:

    s32Ret = TEST_AVS_ModDestroyFramePool(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("avs [%d] TEST_AVS_ModDestroyFramePool failed: %#x",
                pstAvsCtx->s32GrpIndex, s32Ret);
    }
    RK_LOGV("avs [%d] TEST_AVS_ModDestroyFramePool already.",
            pstAvsCtx->s32GrpIndex);

__FREE_PIPE_FRAME:

    for (RK_S32 i = 0; i < AVS_PIPE_NUM; i++) {
        RK_SAFE_FREE(pstPipeFrames[i]);
    }

    RK_SAFE_FREE(pstPipeFrames);

__FREE_VENC_CFG:

    RK_SAFE_FREE(pstVencCtx);

    return s32Ret;
}

static const char *const usages[] = {
    "./rk_mpi_avs_test [--test_mode TEST_MODE] [-c COMPRESS_MODE] [-n LOOP_COUNT] [-p PIPE_NUM]",
    RK_NULL,
};

static void mpi_avs_test_show_options(const TEST_AVS_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input file path      : %s\n", ctx->srcFilePath);
    RK_PRINT("output file path     : %s\n", ctx->dstFilePath);
    RK_PRINT("test mode            : %d\n", ctx->enTestMode);
    RK_PRINT("group number         : %d\n", ctx->s32GrpNum);
    RK_PRINT("pipe number          : %d\n", ctx->s32PipeNum);
    RK_PRINT("channel number       : %d\n", ctx->s32ChnNum);
    RK_PRINT("loop count           : %d\n", ctx->s32LoopCount);
    RK_PRINT("input width          : %d\n", ctx->u32SrcWidth);
    RK_PRINT("input height         : %d\n", ctx->u32SrcHeight);
    RK_PRINT("input virtual width  : %d\n", ctx->u32SrcVirWidth);
    RK_PRINT("input virtual height : %d\n", ctx->u32SrcVirHeight);
    RK_PRINT("input pixel format   : %d\n", ctx->enSrcPixFormat);
    RK_PRINT("input compress mode  : %d\n", ctx->enSrcCompressMode);
    RK_PRINT("output width         : %d\n", ctx->u32DstWidth);
    RK_PRINT("output height        : %d\n", ctx->u32DstHeight);
    RK_PRINT("output pixel format  : %d\n", ctx->enDstPixFormat);
    RK_PRINT("output compress mode : %d\n", ctx->enDstCompressMode);
    RK_PRINT("work mode            : %d\n", ctx->enAvsWorkMode);
    RK_PRINT("projection mode      : %d\n", ctx->enAvsProjMode);
    RK_PRINT("params sources       : %d\n", ctx->enParamSource);
    RK_PRINT("lut file path        : %s\n", ctx->lutFilePath);
    RK_PRINT("lut accuracy         : %d\n", ctx->enLutAccuracy);
    RK_PRINT("lut fuse width       : %d\n", ctx->enLutFuseWidth);
    RK_PRINT("lut step x           : %d\n", ctx->stLutStep.enStepX);
    RK_PRINT("lut step y           : %d\n", ctx->stLutStep.enStepY);
    RK_PRINT("calib file name      : %s\n", ctx->calibFileName);
    RK_PRINT("mesh file path       : %s\n", ctx->meshFilePath);
    RK_PRINT("avs frame sync       : %d\n", ctx->bFrameSync);
    RK_PRINT("center x             : %d\n", ctx->stCenter.s32X);
    RK_PRINT("center x             : %d\n", ctx->stCenter.s32Y);
    RK_PRINT("fov x                : %d\n", ctx->stFov.u32FOVX);
    RK_PRINT("fov y                : %d\n", ctx->stFov.u32FOVY);
    RK_PRINT("rotation ori yaw     : %d\n", ctx->stOriRotation.s32Yaw);
    RK_PRINT("rotation ori pitch   : %d\n", ctx->stOriRotation.s32Pitch);
    RK_PRINT("rotation ori roll    : %d\n", ctx->stOriRotation.s32Roll);
    RK_PRINT("rotation yaw         : %d\n", ctx->stRotation.s32Yaw);
    RK_PRINT("rotation pitch       : %d\n", ctx->stRotation.s32Pitch);
    RK_PRINT("rotation roll        : %d\n", ctx->stRotation.s32Roll);
    RK_PRINT("distance             : %3.2f\n", ctx->fDistance);
    RK_PRINT("enable comm mb pool  : %d\n", ctx->bCommPool);
}

RK_S32 main(int argc, const char **argv) {
    RK_S32 s32Ret = RK_FAILURE;
    TEST_AVS_CTX_S   ctx;

    memset(&ctx, 0, sizeof(TEST_AVS_CTX_S));

    ctx.dstFilePath        = RK_NULL;

    ctx.s32LoopCount       = 1;
    ctx.enTestMode         = TEST_MODE_AVS_ONLY;
    ctx.enParamSource      = AVS_PARAM_SOURCE_CALIB;
    ctx.bFrameSync         = RK_FALSE;
    ctx.enAvsWorkMode      = AVS_MODE_BLEND;
    ctx.s32GrpNum          = 1;
    ctx.s32ChnNum          = 1;
    ctx.s32PipeNum         = 6;
    ctx.s32GrpIndex        = 0;
    ctx.u32ChnDepth        = 1;
    ctx.u32FrameBufCnt     = 3;
    ctx.s32SrcChnRate      = -1;
    ctx.s32DstChnRate      = -1;
    ctx.s32SrcGrpRate      = -1;
    ctx.s32DstGrpRate      = -1;
    ctx.fDistance          = 1.0;

    RK_LOGE("avs test running enter!");

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx.srcFilePath),
                   "input file name. e.g.(/userdata/6x_equirectangular/input_image/). default(NULL).", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx.dstFilePath),
                    "output file path. e.g.(/userdata/res/). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('\0', "test_mode", &(ctx.enTestMode),
                    "test mode. default(0. 0: avs single mod test, 1: multi mods bind test).", NULL, 0, 0),
        OPT_INTEGER('n', "loop count", &(ctx.s32LoopCount),
                    "loop running count. default(1).", NULL, 0, 0),
        OPT_INTEGER('g', "group_count", &(ctx.s32GrpNum),
                    "the count of avs group. default(1).", NULL, 0, 0),
        OPT_INTEGER('p', "pipe_count", &(ctx.s32PipeNum),
                    "the count of avs pipe. default(6).", NULL, 0, 0),
        OPT_INTEGER('c', "channel_count", &(ctx.s32ChnNum),
                    "the count of avs channel. default(1).", NULL, 0, 0),
        OPT_INTEGER('w', "src_width", &(ctx.u32SrcWidth),
                    "src width. e.g.(2560). <required>.", NULL, 0, 0),
        OPT_INTEGER('h', "src_height", &(ctx.u32SrcHeight),
                    "src height. e.g.(1520). <required>.", NULL, 0, 0),
        OPT_INTEGER('\0', "src_vir_width", &(ctx.u32SrcVirWidth),
                    "src virtual width. e.g.(2560). default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "src_vir_height", &(ctx.u32SrcVirHeight),
                    "src virtual height. e.g.(1520). default(0).", NULL, 0, 0),
        OPT_INTEGER('f', "src_format", &(ctx.enSrcPixFormat),
                    "src pixel format. default(only 0. 0 is NV12).", NULL, 0, 0),
        OPT_INTEGER('m', "src_compress", &(ctx.enSrcCompressMode),
                    "src compress mode. default(0. 0: NONE, 1: AFBC).", NULL, 0, 0),
        OPT_INTEGER('W', "dst_width", &(ctx.u32DstWidth),
                    "dst width. e.g.(8192). <required>.", NULL, 0, 0),
        OPT_INTEGER('H', "dst_height", &(ctx.u32DstHeight),
                    "dst height. e.g.(3840). <required>.", NULL, 0, 0),
        OPT_INTEGER('M', "dst_compress", &(ctx.enDstCompressMode),
                    "dst compress mode. default(0. 0: NONE, 1: AFBC).", NULL, 0, 0),
        OPT_INTEGER('F', "dst_format", &(ctx.enDstPixFormat),
                    "dst pixel format. default(only 0. 0 is NV12).", NULL, 0, 0),
        OPT_INTEGER('\0', "work_mode", &(ctx.enAvsWorkMode),
                    "work mode. default(0. 0: BLEND, 1: NOBLEND_VER, 2: NOBLEND_HOR, 3: NOBLEND_QR).",
                    NULL, 0, 0),
        OPT_INTEGER('\0', "proj_mode", &(ctx.enAvsProjMode),
                    "projection mode. default(0. 0: EQUIRECTANGULAR, 1: RECTILINEAR, 2: CYLINDRICAL,"
                    "3: CUBE_MAP, 4: EQUIRECTANGULAR_TRANS).",
                    NULL, 0, 0),
        OPT_INTEGER('\0', "frame_sync", &(ctx.bFrameSync),
                    "whether enable avs frame sync. default(0. 0: Disable, 1: Enable).", NULL, 0, 0),
        OPT_INTEGER('\0', "param_source", &(ctx.enParamSource),
                    "params required for stitch are obtained by lut or calib. default(1. 0: Lut, 1: Calib).",
                    NULL, 0, 0),
        OPT_STRING('\0', "lut_file_path",  &(ctx.lutFilePath),
                    "lut file path. e.g.(/userdata/6x_equirectangular/middle_lut/). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('\0', "lut_accuracy", &(ctx.enLutAccuracy),
                    "lut accuracy. default(0, 0: HIGH, 1: LOW).", NULL, 0, 0),
        OPT_INTEGER('\0', "lut_use_width", &(ctx.enLutFuseWidth),
                    "lut fuse width. default(0, 0: 128 pixel, 1: 256 pixel, 2: 512 pixel).", NULL, 0, 0),
        OPT_INTEGER('\0', "lut_step_x", &(ctx.stLutStep.enStepX),
                    "lut step in the x-axis direction. default(0, 0: 16 pixel, 1: 32 pixel, 2: 64 pixel).", NULL, 0, 0),
        OPT_INTEGER('\0', "lut_step_y", &(ctx.stLutStep.enStepY),
                    "lut step in the y-axis direction. default(0, 0: 16 pixel, 1: 32 pixel, 2: 64 pixel).", NULL, 0, 0),
        OPT_STRING('\0', "calib_file_name",  &(ctx.calibFileName),
                   "calib file name. e.g.(/userdata/avs/6x_equirectangular/avs_calib/calib_file.pto). default(NULL).",
                   NULL, 0, 0),
        OPT_STRING('\0', "mesh_file_path",  &(ctx.meshFilePath),
                   "mesh file path. e.g.(/userdata/avs/6x_equirectangular/avs_mesh/). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('\0', "center_x", &(ctx.stCenter.s32X),
                    "center point in the x-axis direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "center_y", &(ctx.stCenter.s32Y),
                    "center point in the y-axis direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "fov_x", &(ctx.stFov.u32FOVX),
                    "fov size in the x-axis direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "fov_y", &(ctx.stFov.u32FOVY),
                    "fov size in the y-axis direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "ori_yaw", &(ctx.stOriRotation.s32Yaw),
                    "ori rotation in the yaw direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "ori_pitch", &(ctx.stOriRotation.s32Pitch),
                    "ori rotation in the pitch direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "ori_roll", &(ctx.stOriRotation.s32Roll),
                    "ori rotation in the roll direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "yaw", &(ctx.stRotation.s32Yaw),
                    "rotation in the yaw direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "pitch", &(ctx.stRotation.s32Pitch),
                    "rotation in the pitch direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "distance", &(ctx.fDistance),
                    "optimal stitch distance. default(1.0).", NULL, 0, 0),
        OPT_INTEGER('\0', "src_grp_rate", &(ctx.s32SrcGrpRate),
                    "src avs group frame rate, default(-1).", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_grp_rate", &(ctx.s32DstGrpRate),
                    "dst avs group frame rate, default(-1).", NULL, 0, 0),
        OPT_INTEGER('d', "chn_depth", &(ctx.u32ChnDepth),
                    "channel output depth, default(3)", NULL, 0, 0),
        OPT_INTEGER('b', "chn_buf_cnt", &(ctx.u32FrameBufCnt),
                    "channel output buf cnt, default(3)", NULL, 0, 0),
        OPT_INTEGER('\0', "roll", &(ctx.stRotation.s32Roll),
                    "rotation in the roll direction. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "src_chn_rate", &(ctx.s32SrcChnRate),
                    "src avs channel frame rate, default(-1).", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_chn_rate", &(ctx.s32DstChnRate),
                    "dst avs channel frame rate, default(-1).", NULL, 0, 0),
        OPT_BOOLEAN('\0', "comm_mb_pool", &(ctx.bCommPool),
                    "whether enable common mb pool, default(0, 0: Disable, 1: Enable).", NULL, 0, 0),
        OPT_GROUP("external options:"),
        OPT_BOOLEAN('\0', "get_final_lut", &(ctx.bGetFinalLut),
                    "whether get final lut, default(0, 0: Disable, 1: Enable).", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_avs_test_show_options(&ctx);

    if (ctx.u32SrcWidth <= 0
        || ctx.u32SrcHeight <= 0
        || ctx.u32DstWidth <= 0
        || ctx.u32DstHeight <= 0) {
        argparse_usage(&argparse);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    switch (ctx.enTestMode) {
        case TEST_MODE_AVS_ONLY: {
            s32Ret = test_single_avs_loop(&ctx);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        } break;
        case TEST_MODE_AVS_VPSS_VENC: {
            s32Ret = test_avs_vpss_venc_loop(&ctx);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        } break;
        default:
            RK_LOGE("unsupport test mode: %d", ctx.enTestMode);
        break;
    }

    s32Ret = RK_MPI_SYS_Exit();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    RK_LOGV("test running ok.");

    return RK_SUCCESS;

__FAILED:
    RK_LOGE("test running exit: %x", s32Ret);
    RK_MPI_SYS_Exit();

    return s32Ret;
}
