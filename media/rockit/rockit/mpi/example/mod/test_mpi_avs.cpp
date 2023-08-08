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
#undef DBG_MOD_ID
#define DBG_MOD_ID       RK_ID_AVS

#include <stdio.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>

#include "rk_defines.h"
#include "rk_debug.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mmz.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_vpss.h"
#include "rk_mpi_avs.h"
#include "rk_mpi_vo.h"
#include "test_comm_argparse.h"

/* for RK356x */
#define RK356X_VO_DEV_HD0          0
#define RK356X_VO_DEV_HD1          1
#define RK356X_VOP_LAYER_CLUSTER_0 0
#define RK356X_VOP_LAYER_CLUSTER_1 2
#define RK356X_VOP_LAYER_ESMART_0  4
#define RK356X_VOP_LAYER_ESMART_1  5
#define RK356X_VOP_LAYER_SMART_0   6
#define RK356X_VOP_LAYER_SMART_1   7

/* for RK3588 */
#define RK3588_VO_DEV_HDMI         0
#define RK3588_VO_DEV_MIPI         3

#define AVS_STITCH_NUM 6

#ifndef RK_SAFE_FREE
#define RK_SAFE_FREE(p)    { if (p) {free(p); (p)=RK_NULL;} }
#endif

#ifndef RK_SAFE_DELETE
#define RK_SAFE_DELETE(p)  { if (p) {delete(p); (p)=RK_NULL;} }
#endif

#ifndef RK_AVS_RK356x
#define RK_AVS_RK356x 0
#endif

#ifndef RK_AVS_RK3588
#define RK_AVS_RK3588 1
#endif

#define MAX_FILE_NAME_LEN 64
#define MAX_FILE_PATH_LEN 256

#ifndef rk_safe_free
#define rk_safe_free(p)              { if (p) {free(p); (p)=RK_NULL;} }
#endif

#ifndef rk_safe_delete
#define rk_safe_delete(p)            { if (p) {delete(p); (p)=RK_NULL;} }
#endif

typedef struct rkVI_CFG_S {
    const RK_CHAR *dstFilePath;
    VI_DEV              s32DevId;
    VI_CHN              s32ChnId;
    VI_PIPE             s32PipeId;
    RK_U32              u32ViChnCnt;
    RK_U32              u32ViPipeCnt;
    RK_S32              s32SelectFd;
    RK_BOOL             bFreeze;
    VI_DEV_ATTR_S       stViDevAttr;
    VI_CHN_ATTR_S       stViChnAttr;
    VI_CHN_STATUS_S     stViChnStatus;
    VI_DEV_BIND_PIPE_S  stBindPipe;
    VI_SAVE_FILE_INFO_S stDebugFile;
    VI_FRAME_S          stViFrame;
} VI_CFG_S;

typedef struct rkVPSS_CFG_S {
    VPSS_GRP s32GrpId;
    VPSS_CHN s32ChnId;
    RK_U32 u32VpssChnCnt;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
} VPSS_CFG_S;

typedef struct rkAVS_CFG_S {
    AVS_GRP         s32GrpId;
    AVS_CHN         s32ChnId;
    AVS_PIPE        s32PipeId;
    RK_U32          u32AvsPipeCnt;
    RK_U32          u32AvsChnCnt;
    RK_U32          u32OutW;
    RK_U32          u32OutH;
    COMPRESS_MODE_E enOutCmpMode;
    AVS_MOD_PARAM_S stAvsModParam;
    AVS_GRP_ATTR_S  stAvsGrpAttr;
    AVS_OUTPUT_ATTR_S stAvsOutAttr;
    AVS_CHN_ATTR_S  stAvsChnAttr[AVS_MAX_CHN_NUM];
} AVS_CFG_S;

typedef struct _rkVO_CFG_S {
    VO_DEV                s32DevId;
    VO_CHN                s32ChnId;
    VO_LAYER              s32LayerId;
    VO_VIDEO_LAYER_ATTR_S stVoLayerAttr;
    VO_CHN_ATTR_S         stVoChnAttr;
    VO_CSC_S              stVoCscAttr;
} VO_CFG_S;

typedef enum rkTEST_MODE_E {
    TEST_MODE_AVS_ONLY       = 0,
    TEST_MODE_VI_VPSS_VO     = 1,
    TEST_MODE_VI_AVS_VO      = 2,
    TEST_MODE_AFBC_VI_AVS_VO = 3,
} TEST_MODE_E;

typedef struct _rkMPI_CTX_S {
    RK_BOOL bThreadExit;
    char srcPath[MAX_FILE_PATH_LEN];
    char dstPath[MAX_FILE_PATH_LEN];
    FILE *srcFp[AVS_PIPE_NUM];
    FILE *dstSaveFp;

    VI_CFG_S   viContext;
    VPSS_CFG_S vpssContext;
    AVS_CFG_S  avsContext;
    VO_CFG_S   voContext;
} MPI_CTX_S;

static RK_U32 test_get_file_len(const RK_CHAR* File) {
    FILE* FILE;
    RK_U32 u32FileLen;
    RK_S32 s32Ret;

    FILE = fopen(File, "rb");

    if (NULL != FILE) {
        s32Ret = fseek(FILE, 0L, SEEK_END);
        if (0 != s32Ret) {
            RK_LOGE("fseek end err!");
            fclose(FILE);
            return 0;
        }

        u32FileLen = ftell(FILE);

        s32Ret = fseek(FILE, 0L, SEEK_SET);
        if (0 != s32Ret) {
            RK_LOGE("fseek begin err!");
            fclose(FILE);
            return 0;
        }
        fclose(FILE);
    } else {
        RK_LOGE("open file %s fail!", File);
        u32FileLen = 0;
    }

    return u32FileLen;
}

static RK_S32 test_open_and_read_file(const RK_CHAR *path, RK_VOID *pu8SrcData, RK_U32 u32SrcSize) {
    FILE  *pFile        = RK_NULL;
    RK_U32 u32ReadSize  = 0;

    pFile = fopen(path, "rb+");
    if (pFile == RK_NULL) {
        RK_LOGE("open path %s failed because %s.", path, strerror(errno));
        return RK_ERR_NULL_PTR;
    }
    if (pFile) {
         u32ReadSize = fread(pu8SrcData, 1, u32SrcSize, pFile);
         fflush(pFile);
         fclose(pFile);
    }
    RK_LOGD("unit_test_open_source u32ReadSize:%d", u32ReadSize);
    if (u32ReadSize != u32SrcSize) {
        RK_LOGE("read error read %d, request %d", u32ReadSize, u32SrcSize);
        return RK_ERR_NULL_PTR;
    }
    return RK_SUCCESS;
}

static RK_S32 test_avs_generate_param_source(const char *srcFilePath, MB_BLK *pstSrcBlk, RK_U32 u32SrcSize) {
    RK_S32 s32Ret            = RK_SUCCESS;
    RK_VOID  *pSrcData       = RK_NULL;

    s32Ret = RK_MPI_SYS_MmzAlloc(pstSrcBlk, RK_NULL, RK_NULL, u32SrcSize);
    if (s32Ret == RK_SUCCESS) {
       pSrcData = RK_MPI_MB_Handle2VirAddr(*pstSrcBlk);
       s32Ret = test_open_and_read_file(srcFilePath, pSrcData, u32SrcSize);
       RK_MPI_SYS_MmzFlushCache(*pstSrcBlk, RK_FALSE);
    }
    return s32Ret;
}

static RK_S32 create_vpss(VPSS_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = { VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3 };
    VPSS_GRP VpssGrp = ctx->s32GrpId;

    s32Ret = RK_MPI_VPSS_CreateGrp(VpssGrp, &ctx->stVpssGrpAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    for (RK_S32 i = 0; i < ctx->u32VpssChnCnt; i++) {
        s32Ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[i], &ctx->stVpssChnAttr[i]);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[i]);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VPSS_EnableBackupFrame(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RK_MPI_VPSS_StartGrp(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return  RK_SUCCESS;
}

static RK_S32 destroy_vpss(VPSS_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = { VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3 };
    VPSS_GRP VpssGrp = ctx->s32GrpId;
    s32Ret = RK_MPI_VPSS_StopGrp(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    for (RK_S32 i = 0; i < ctx->u32VpssChnCnt; i++) {
        s32Ret = RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[i]);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VPSS_DisableBackupFrame(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RK_MPI_VPSS_DestroyGrp(VpssGrp);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return  RK_SUCCESS;
}

static RK_S32 destroy_vo(VO_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VO_LAYER VoLayer = ctx->s32LayerId;
    VO_DEV VoDev = ctx->s32DevId;
    VO_CHN VoChn = ctx->s32ChnId;

    RK_MPI_VO_DisableLayer(VoLayer);
    RK_MPI_VO_DisableChn(VoDev, VoChn);
    RK_MPI_VO_Disable(VoDev);

    return s32Ret;
}

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
        return s32Ret;
    }

#if RK_AVS_RK356x
    VoPubAttr.enIntfType = VO_INTF_HDMI;
    VoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
#endif
#if RK_AVS_RK3588
    VoPubAttr.enIntfType = VO_INTF_HDMI;
    VoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
#endif

    s32Ret = RK_MPI_VO_SetPubAttr(VoDev, &VoPubAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RK_MPI_VO_Enable(VoDev);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = RK_MPI_VO_GetLayerDispBufLen(VoLayer, &u32DispBufLen);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("Get display buf len failed: %d!", s32Ret);
        return s32Ret;
    }

    u32DispBufLen = 3;
    s32Ret = RK_MPI_VO_SetLayerDispBufLen(VoLayer, u32DispBufLen);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("Set display buf len %d failed: %d!", u32DispBufLen, s32Ret);
        return s32Ret;
    }

    s32Ret = RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_GRAPHIC);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_BindLayer failed: %#x", s32Ret);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_VO_SetLayerAttr(VoLayer, &ctx->stVoLayerAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_SetLayerAttr failed: %#x", s32Ret);
        return RK_FAILURE;
    }


    s32Ret = RK_MPI_VO_EnableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_EnableLayer failed: %#x", s32Ret);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_VO_SetChnAttr(VoLayer, VoChn, &ctx->stVoChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_SetChnAttr failed: %#x", s32Ret);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_VO_SetLayerCSC(VoLayer, &ctx->stVoCscAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_SetLayerCSC failed: %#x", s32Ret);
        return s32Ret;
    }

    s32Ret = RK_MPI_VO_EnableChn(VoLayer, VoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VO_EnableChn failed: %#x", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

static RK_S32 create_vi(VI_CFG_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;

    /* 0. get dev config status */
    s32Ret = RK_MPI_VI_GetDevAttr(ctx->s32DevId, &ctx->stViDevAttr);
    if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
        /* 0-1.config dev */
        s32Ret = RK_MPI_VI_SetDevAttr(ctx->s32DevId, &ctx->stViDevAttr);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevAttr %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_SetDevAttr already");
    }
    /* 1.get  dev enable status */
    s32Ret = RK_MPI_VI_GetDevIsEnable(ctx->s32DevId);
    if (s32Ret != RK_SUCCESS) {
        /* 1-2.enable dev */
        s32Ret = RK_MPI_VI_EnableDev(ctx->s32DevId);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_EnableDev %x", s32Ret);
            goto __FAILED;
        }
        /* 1-3.bind dev/pipe */
        ctx->stBindPipe.u32Num = ctx->s32PipeId;
        ctx->stBindPipe.PipeId[0] = ctx->s32PipeId;
        s32Ret = RK_MPI_VI_SetDevBindPipe(ctx->s32DevId, &ctx->stBindPipe);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("RK_MPI_VI_SetDevBindPipe %x", s32Ret);
            goto __FAILED;
        }
    } else {
        RK_LOGE("RK_MPI_VI_EnableDev already");
    }
    /* 2.config channel */
    s32Ret = RK_MPI_VI_SetChnAttr(ctx->s32PipeId, ctx->s32ChnId, &ctx->stViChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_SetChnAttr %x", s32Ret);
        goto __FAILED;
    }
    /* 3.enable channel */
    s32Ret = RK_MPI_VI_EnableChn(ctx->s32PipeId, ctx->s32ChnId);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_EnableChn %x", s32Ret);
        goto __FAILED;
    }
    /* 4.save debug file */
    if (ctx->stDebugFile.bCfg) {
        s32Ret = RK_MPI_VI_ChnSaveFile(ctx->s32PipeId, ctx->s32ChnId, &ctx->stDebugFile);
        RK_LOGD("RK_MPI_VI_ChnSaveFile %x", s32Ret);
    }

__FAILED:
    return s32Ret;
}

static RK_S32 destroy_vi(VI_CFG_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    s32Ret = RK_MPI_VI_DisableChn(ctx->s32PipeId, ctx->s32ChnId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

    s32Ret = RK_MPI_VI_DisableDev(ctx->s32DevId);
    RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);

    return s32Ret;
}

static RK_S32 create_avs(AVS_CFG_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_AVS_SetModParam(&ctx->stAvsModParam);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("AVS set mod params failed with %#x!", s32Ret);
        goto __FAILED;
    }

    s32Ret = RK_MPI_AVS_CreateGrp(ctx->s32GrpId, &ctx->stAvsGrpAttr);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("AVS creat grp failed with %#x!", s32Ret);
        goto __FAILED;
    }

    s32Ret = RK_MPI_AVS_SetChnAttr(ctx->s32GrpId, ctx->s32ChnId, &ctx->stAvsChnAttr[0]);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("AVS set chn attr failed with %#x!", s32Ret);
        goto __FAILED;
    }

    s32Ret = RK_MPI_AVS_EnableChn(ctx->s32GrpId, ctx->s32ChnId);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("AVS enable chn failed with %#x!", s32Ret);
        goto __FAILED;
    }

    s32Ret = RK_MPI_AVS_StartGrp(ctx->s32GrpId);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("AVS start grp failed with %#x!", s32Ret);
        goto __FAILED;
    }

__FAILED:
    return s32Ret;
}

static RK_S32 destroy_avs(AVS_CFG_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;

    s32Ret = RK_MPI_AVS_StopGrp(ctx->s32GrpId);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_AVS_StopGrp failed with %#x!\n", s32Ret);
        goto __FAILED;
    }

    s32Ret = RK_MPI_AVS_DisableChn(ctx->s32GrpId, ctx->s32ChnId);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_AVS_DisableChn failed with %#x!\n", s32Ret);
        goto __FAILED;
    }

    s32Ret = RK_MPI_AVS_DestroyGrp(ctx->s32GrpId);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_AVS_DestroyGrp failed with %#x!\n", s32Ret);
        goto __FAILED;
    }

__FAILED:
    return s32Ret;
}


static RK_VOID* avs_send_get_frame_thread(RK_VOID *pArgs) {
    prctl(PR_SET_NAME, "avs_send_get_frame_thread");

    MPI_CTX_S *pstCtx = reinterpret_cast<MPI_CTX_S *>(pArgs);
    RK_S32     s32Ret = RK_SUCCESS;
    MB_BLK  srcBlk[2] = { MB_INVALID_HANDLE, MB_INVALID_HANDLE };
    RK_S32 s32ReadLen = 0;
    RK_S32 s32FrameInputCount = 0;
    RK_S32 s32FrameOutputCount = 0;
    VIDEO_FRAME_INFO_S frameIn[2] = { 0, 0 };
    VIDEO_FRAME_INFO_S frameOut = {0};
    PIC_BUF_ATTR_S  stPicBufAttr;
    MB_PIC_CAL_S    stMbPicCalResult;

    stPicBufAttr.u32Width = 2688;
    stPicBufAttr.u32Height = 1520;
    stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    stPicBufAttr.enPixelFormat = RK_FMT_YUV420SP;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return RK_NULL;
    }

    for (RK_S32 i = 0; i < pstCtx->avsContext.u32AvsPipeCnt; i++) {
        RK_MPI_MMZ_Alloc(&srcBlk[i], stMbPicCalResult.u32MBSize, RK_MMZ_ALLOC_CACHEABLE);
    }

    while (pstCtx->bThreadExit != RK_TRUE) {
        for (RK_S32 i = 0; i < pstCtx->avsContext.u32AvsPipeCnt; i++) {
            if (pstCtx->srcFp[i] != RK_NULL) {
                s32Ret = fseek(pstCtx->srcFp[i], 0, SEEK_SET);
                if (s32Ret != 0) {
                    break;
                }
                s32ReadLen = fread(RK_MPI_MB_Handle2VirAddr(srcBlk[i]),
                                   1, stMbPicCalResult.u32MBSize, pstCtx->srcFp[i]);
                if (s32ReadLen <= 0) {
                    RK_LOGE("read size is not enough. read %d, request %d",
                            s32ReadLen, stMbPicCalResult.u32MBSize);
                    break;
                }
            }
            frameIn[i].stVFrame.pMbBlk = srcBlk;
            frameIn[i].stVFrame.u32Width = 2688;
            frameIn[i].stVFrame.u32Height = 1520;
            frameIn[i].stVFrame.u32VirWidth = stMbPicCalResult.u32VirWidth;
            frameIn[i].stVFrame.u32VirHeight = stMbPicCalResult.u32VirHeight;
            frameIn[i].stVFrame.enCompressMode = COMPRESS_MODE_NONE;
            frameIn[i].stVFrame.enPixelFormat = RK_FMT_YUV420SP;
        }
        for (RK_U32 i = 0; i < pstCtx->avsContext.u32AvsPipeCnt; i++) {
            s32Ret = RK_MPI_AVS_SendPipeFrame(pstCtx->avsContext.s32GrpId, i, &frameIn[i], -1);
            if (s32Ret != RK_SUCCESS) {
                RK_MPI_MB_ReleaseMB(srcBlk);
                break;
            }
            s32FrameInputCount++;
        }
        for (RK_S32 i = 0; i < pstCtx->avsContext.u32AvsChnCnt; i++) {
            s32Ret = RK_MPI_AVS_GetChnFrame(pstCtx->avsContext.s32GrpId, i, &frameOut, -1);
            if (s32Ret != RK_SUCCESS) {
                break;
            }
            s32FrameOutputCount++;
            RK_LOGI("get grp[%d] chn[%d] frame %p length %d",
                    pstCtx->avsContext.s32GrpId, i,
                    frameOut.stVFrame.pMbBlk, stMbPicCalResult.u32MBSize);
            if (pstCtx->dstSaveFp != RK_NULL) {
                fwrite(RK_MPI_MB_Handle2VirAddr(frameOut.stVFrame.pMbBlk),
                        1, stMbPicCalResult.u32MBSize, pstCtx->dstSaveFp);
                fflush(pstCtx->dstSaveFp);
            }
        }
    }

    return RK_NULL;
}

static RK_S32 test_avs_get_release_frame_loop(RK_VOID) {
    RK_S32 s32Ret = RK_FAILURE;
    AVS_CFG_S *pstAvsCtx = reinterpret_cast<AVS_CFG_S *>(malloc(sizeof(AVS_CFG_S)));
    MPI_CTX_S *avsGetReleaseCtx = reinterpret_cast<MPI_CTX_S *>(malloc(sizeof(MPI_CTX_S)));

    RK_U32 u32LUTSize = 0;
    MB_BLK pLUTVirAddr[AVS_PIPE_NUM] = { RK_NULL };
    memset(pstAvsCtx, 0, sizeof(AVS_CFG_S));
    memset(avsGetReleaseCtx, 0, sizeof(avsGetReleaseCtx));

    /* avs config init */
    pstAvsCtx->s32GrpId = 0;
    pstAvsCtx->s32PipeId = 0;
    pstAvsCtx->s32ChnId = 0;
    pstAvsCtx->u32AvsPipeCnt = AVS_STITCH_NUM;
    pstAvsCtx->u32AvsChnCnt = 1;
    if (AVS_STITCH_NUM == 2) {
        pstAvsCtx->u32OutW = 5088;
        pstAvsCtx->u32OutH = 1520;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
            sizeof("/usr/data/demo_data/hk_data/mesh_data_multiBand/"),
            "/usr/data/demo_data/hk_data/mesh_data_multiBand/");
    } else if (AVS_STITCH_NUM == 4) {
        pstAvsCtx->u32OutW = 5440;
        pstAvsCtx->u32OutH = 2700;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
            sizeof("/usr/data/demo_data/qk_data/mesh_data_multiBand/"),
            "/usr/data/demo_data/qk_data/mesh_data_multiBand/");
    } else if (AVS_STITCH_NUM == 6) {
        pstAvsCtx->u32OutW = 8192;
        pstAvsCtx->u32OutH = 2700;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
            sizeof("/usr/data/demo_data/6x/multiBand_8192x2700/"),
            "/usr/data/demo_data/6x/multiBand_8192x2700/");
    }
    pstAvsCtx->stAvsGrpAttr.stLUT.enAccuracy                 = AVS_LUT_ACCURACY_HIGH;
    pstAvsCtx->enOutCmpMode                                  = COMPRESS_MODE_NONE;

    pstAvsCtx->stAvsModParam.u32WorkingSetSize = 67 * 1024;
    pstAvsCtx->stAvsGrpAttr.enMode                           = AVS_MODE_BLEND;
    pstAvsCtx->stAvsGrpAttr.u32PipeNum                       = pstAvsCtx->u32AvsPipeCnt;
    pstAvsCtx->stAvsGrpAttr.stGainAttr.enMode                = AVS_GAIN_MODE_AUTO;

    pstAvsCtx->stAvsGrpAttr.stOutAttr.enPrjMode              = AVS_PROJECTION_EQUIRECTANGULAR;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stCenter.s32X          = pstAvsCtx->u32OutW / 2;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stCenter.s32Y          = pstAvsCtx->u32OutH / 2;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stFOV.u32FOVX          = 36000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stFOV.u32FOVY          = 18000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Roll  = 9000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Pitch = 9000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Yaw   = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Roll     = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Pitch    = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Yaw      = 0;

    pstAvsCtx->stAvsGrpAttr.bSyncPipe                     = RK_TRUE;
    pstAvsCtx->stAvsGrpAttr.stFrameRate.s32SrcFrameRate   = -1;
    pstAvsCtx->stAvsGrpAttr.stFrameRate.s32DstFrameRate   = -1;

    pstAvsCtx->stAvsChnAttr[0].enCompressMode              = pstAvsCtx->enOutCmpMode;
    pstAvsCtx->stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
    pstAvsCtx->stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
    pstAvsCtx->stAvsChnAttr[0].u32Depth                    = 0;
    pstAvsCtx->stAvsChnAttr[0].u32Width                    = pstAvsCtx->u32OutW;
    pstAvsCtx->stAvsChnAttr[0].u32Height                   = pstAvsCtx->u32OutH;
    pstAvsCtx->stAvsChnAttr[0].enDynamicRange              = DYNAMIC_RANGE_SDR8;

    /* avs create */
    s32Ret = create_avs(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create avs [%d, %d, %d] error %x: ",
                pstAvsCtx->s32GrpId,
                pstAvsCtx->s32PipeId,
                pstAvsCtx->s32ChnId,
                s32Ret);
        return s32Ret;
    }
    /* do somethings */

    MB_BLK           srcBlk[2] = { MB_INVALID_HANDLE, MB_INVALID_HANDLE };
    RK_S32           s32ReadLen = 0;
    RK_S32           s32FrameInputCount = 0;
    RK_S32           s32FrameOutputCount = 0;
    VIDEO_FRAME_INFO_S frameIn[2] = { 0, 0 };
    VIDEO_FRAME_INFO_S frameOut = {0};
    PIC_BUF_ATTR_S  stSrcPicBufAttr;
    MB_PIC_CAL_S    stSrcMbPicCalResult;
    PIC_BUF_ATTR_S  stDstPicBufAttr;
    MB_PIC_CAL_S    stDstMbPicCalResult;

    stSrcPicBufAttr.u32Width = 2688;
    stSrcPicBufAttr.u32Height = 1520;
    stSrcPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    stSrcPicBufAttr.enPixelFormat = RK_FMT_YUV420SP;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stSrcPicBufAttr, &stSrcMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return RK_NULL;
    }
    stDstPicBufAttr.u32Width = 5088;
    stDstPicBufAttr.u32Height = 1520;
    stDstPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    stDstPicBufAttr.enPixelFormat = RK_FMT_YUV420SP;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stDstPicBufAttr, &stDstMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return RK_NULL;
    }

    for (RK_S32 i = 0; i < pstAvsCtx->u32AvsPipeCnt; i++) {
        RK_MPI_MMZ_Alloc(&(srcBlk[i]), stSrcMbPicCalResult.u32MBSize, RK_MMZ_ALLOC_CACHEABLE);
        snprintf(avsGetReleaseCtx->srcPath, sizeof(avsGetReleaseCtx->srcPath) ,
                    "/usr/data/demo_data/hk_data/image_data/camera%d.nv12", i);
        avsGetReleaseCtx->srcFp[i] = fopen(avsGetReleaseCtx->srcPath, "r");
        if (RK_NULL == avsGetReleaseCtx->srcFp[i]) {
            RK_LOGE("pipe %d src file can not find file", i);
            return RK_ERR_NULL_PTR;
        }
    }
    snprintf(avsGetReleaseCtx->dstPath, sizeof(avsGetReleaseCtx->dstPath), "/usr/data/demo_data/hk_data/test_output/");
    avsGetReleaseCtx->dstSaveFp = fopen(avsGetReleaseCtx->dstPath, "wb");
    if (RK_NULL == avsGetReleaseCtx->dstSaveFp) {
        RK_LOGE("chn %d src file can not find file", 0);
        return RK_ERR_NULL_PTR;
    }

    for (RK_S32 loopNum = 0; loopNum < 100; loopNum++) {
        for (RK_S32 i = 0; i < pstAvsCtx->u32AvsPipeCnt; i++) {
            if (avsGetReleaseCtx->srcFp[i] != RK_NULL) {
                s32Ret = fseek(avsGetReleaseCtx->srcFp[i], 0, SEEK_SET);
                if (s32Ret != 0) {
                    break;
                }
                s32ReadLen = fread(RK_MPI_MB_Handle2VirAddr(srcBlk[i]),
                                   1, stSrcMbPicCalResult.u32MBSize, avsGetReleaseCtx->srcFp[i]);
                if (s32ReadLen <= 0) {
                    RK_LOGE("read size is not enough. read %d, request %d",
                            s32ReadLen, stSrcMbPicCalResult.u32MBSize);
                    break;
                }
            }
            frameIn[i].stVFrame.pMbBlk = srcBlk[i];
            frameIn[i].stVFrame.u32Width = 2688;
            frameIn[i].stVFrame.u32Height = 1520;
            frameIn[i].stVFrame.u32VirWidth = stSrcMbPicCalResult.u32VirWidth;
            frameIn[i].stVFrame.u32VirHeight = stSrcMbPicCalResult.u32VirHeight;
            frameIn[i].stVFrame.enCompressMode = COMPRESS_MODE_NONE;
            frameIn[i].stVFrame.enPixelFormat = RK_FMT_YUV420SP;

            s32Ret = RK_MPI_AVS_SendPipeFrame(pstAvsCtx->s32GrpId, i, &(frameIn[i]), -1);
            if (s32Ret != RK_SUCCESS) {
                RK_MPI_MB_ReleaseMB(srcBlk[i]);
                RK_LOGE("send failed grp[%d] pipe[%d] frame %p length %d",
                        pstAvsCtx->s32GrpId, i,
                        frameOut.stVFrame.pMbBlk, stSrcMbPicCalResult.u32MBSize);
                break;
            }
            RK_LOGE("send successful grp[%d] pipe[%d] frame %p length %d",
                    pstAvsCtx->s32GrpId, i,
                    frameOut.stVFrame.pMbBlk, stSrcMbPicCalResult.u32MBSize);
            s32FrameInputCount++;
        }
        for (RK_S32 i = 0; i < pstAvsCtx->u32AvsChnCnt; i++) {
            s32Ret = RK_MPI_AVS_GetChnFrame(pstAvsCtx->s32GrpId, i, &frameOut, -1);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("get failed grp[%d] chn[%d] frame %p length %d",
                        pstAvsCtx->s32GrpId, i,
                        frameOut.stVFrame.pMbBlk, stDstMbPicCalResult.u32MBSize);
                break;
            }
            s32FrameOutputCount++;
            RK_LOGE("get successful grp[%d] chn[%d] frame %p length %d",
                    pstAvsCtx->s32GrpId, i,
                    frameOut.stVFrame.pMbBlk, stDstMbPicCalResult.u32MBSize);
            if (avsGetReleaseCtx->dstSaveFp != RK_NULL) {
                s32Ret = fseek(avsGetReleaseCtx->dstSaveFp, 0, SEEK_END);
                fwrite(RK_MPI_MB_Handle2VirAddr(frameOut.stVFrame.pMbBlk),
                        1, stDstMbPicCalResult.u32MBSize, avsGetReleaseCtx->dstSaveFp);
                fflush(avsGetReleaseCtx->dstSaveFp);
            }
            s32Ret = RK_MPI_AVS_ReleaseChnFrame(pstAvsCtx->s32GrpId, i, &frameOut);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("release failed grp[%d] chn[%d] frame %p length %d",
                        pstAvsCtx->s32GrpId, i,
                        frameOut.stVFrame.pMbBlk, stDstMbPicCalResult.u32MBSize);
                break;
            }
            RK_LOGE("release successful grp[%d] chn[%d] frame %p length %d",
                    pstAvsCtx->s32GrpId, i,
                    frameOut.stVFrame.pMbBlk, stDstMbPicCalResult.u32MBSize);
        }
    }
    for (RK_S32 i = 0; i < pstAvsCtx->u32AvsPipeCnt; i++) {
        fclose(avsGetReleaseCtx->srcFp[i]);
    }

    fclose(avsGetReleaseCtx->dstSaveFp);

    /* avs destroy */
    s32Ret = destroy_avs(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("destroy avs [%d, %d, %d] error %x: ",
                pstAvsCtx->s32GrpId,
                pstAvsCtx->s32PipeId,
                pstAvsCtx->s32ChnId,
                s32Ret);
        return s32Ret;
    }

__FAILED:
    RK_SAFE_FREE(pstAvsCtx);
    RK_SAFE_FREE(avsGetReleaseCtx);

    return s32Ret;
}

static RK_S32 test_vi_vpss_vo_loop(RK_VOID) {
    RK_S32 s32Ret = RK_FAILURE;
    VI_CFG_S   *pstViCtx[AVS_STITCH_NUM];
    VPSS_CFG_S *pstVpssCtx[AVS_STITCH_NUM];
    VO_CFG_S   *pstVoCtx[AVS_STITCH_NUM];
    MPP_CHN_S stViChn[AVS_STITCH_NUM],
              stVpssChn[AVS_STITCH_NUM],
              stVoChn[AVS_STITCH_NUM];

    RK_S32 loopCount = 0;
    RK_S32 i = 0;

    for (i = 0; i < AVS_STITCH_NUM; i++) {
        pstViCtx[i] = reinterpret_cast<VI_CFG_S *>(malloc(sizeof(VI_CFG_S)));
        pstVpssCtx[i] = reinterpret_cast<VPSS_CFG_S *>(malloc(sizeof(VPSS_CFG_S)));
        pstVoCtx[i] = reinterpret_cast<VO_CFG_S *>(malloc(sizeof(VO_CFG_S)));
        memset(pstViCtx[i], 0, sizeof(VI_CFG_S));
        memset(pstVpssCtx[i], 0, sizeof(VPSS_CFG_S));
        memset(pstVoCtx[i], 0, sizeof(VO_CFG_S));
    }

    for (i = 0; i < AVS_STITCH_NUM; i++) {
    /* vi config init */
        pstViCtx[i]->s32DevId = i;
        pstViCtx[i]->s32PipeId = pstViCtx[i]->s32DevId;
        pstViCtx[i]->s32ChnId = 1;
        pstViCtx[i]->stViChnAttr.stSize.u32Width = 2688;
        pstViCtx[i]->stViChnAttr.stSize.u32Height = 1520;
        pstViCtx[i]->stViChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
#if RK_AVS_RK356x
        if (i == 0) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video5", strlen("/dev/video5"));    // 15FPS
        } else if (i == 1) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 15;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video14", strlen("/dev/video14"));  // 30FPS
        }
#endif
#if RK_AVS_RK3588
        if (i == 0) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video32", strlen("/dev/video32"));
        } else if (i == 1) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video33", strlen("/dev/video33"));
        } else if (i == 2) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video40", strlen("/dev/video40"));
        } else if (i == 3) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video41", strlen("/dev/video41"));
        }
#endif
        pstViCtx[i]->stViChnAttr.u32Depth = 2;
        pstViCtx[i]->stViChnAttr.enPixelFormat = RK_FMT_YUV420SP;
        pstViCtx[i]->stViChnAttr.stFrameRate.s32SrcFrameRate = -1;
        pstViCtx[i]->stViChnAttr.stFrameRate.s32DstFrameRate = -1;

    /* vi create */
        s32Ret = create_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vi [%d, %d] init failed: %x",
                    pstViCtx[i]->s32DevId, pstViCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED_VI;
        }
    }

    /* vpss config init */
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        pstVpssCtx[i]->s32GrpId = i;
        pstVpssCtx[i]->s32ChnId = 0;
        pstVpssCtx[i]->u32VpssChnCnt = 1;
        pstVpssCtx[i]->stVpssGrpAttr.u32MaxW = 4096;
        pstVpssCtx[i]->stVpssGrpAttr.u32MaxH = 4096;
        pstVpssCtx[i]->stVpssGrpAttr.enPixelFormat = RK_FMT_YUV420SP;
        pstVpssCtx[i]->stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
        pstVpssCtx[i]->stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
        pstVpssCtx[i]->stVpssGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
        for (RK_S32 j = 0; j < VPSS_MAX_CHN_NUM; j ++) {
            pstVpssCtx[i]->stVpssChnAttr[j].enChnMode = VPSS_CHN_MODE_USER;
            pstVpssCtx[i]->stVpssChnAttr[j].enDynamicRange = DYNAMIC_RANGE_SDR8;
            pstVpssCtx[i]->stVpssChnAttr[j].enPixelFormat = RK_FMT_YUV420SP;
            pstVpssCtx[i]->stVpssChnAttr[j].stFrameRate.s32SrcFrameRate = -1;
            pstVpssCtx[i]->stVpssChnAttr[j].stFrameRate.s32DstFrameRate = -1;
            pstVpssCtx[i]->stVpssChnAttr[j].u32Width = 2688;
            pstVpssCtx[i]->stVpssChnAttr[j].u32Height = 1520;
            pstVpssCtx[i]->stVpssChnAttr[j].enCompressMode = COMPRESS_MODE_NONE;
        }
        /* vpss create */
        s32Ret = create_vpss(pstVpssCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vpss [%d, %d] create failed: %x",
                    pstVpssCtx[i]->s32GrpId,
                    pstVpssCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED_VPSS;
        }
    }

    /* bind vi to vpss */
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        stViChn[i].enModId    = RK_ID_VI;
        stViChn[i].s32DevId   = pstViCtx[i]->s32DevId;
        stViChn[i].s32ChnId   = pstViCtx[i]->s32ChnId;

        stVpssChn[i].enModId = RK_ID_VPSS;
        stVpssChn[i].s32DevId = pstVpssCtx[i]->s32GrpId;
        stVpssChn[i].s32ChnId = 0;

        RK_LOGI("vi [%d, %d] -> vpss [%d, %d]",
                stViChn[i].s32DevId, stViChn[i].s32ChnId,
                stVpssChn[i].s32DevId, stVpssChn[i].s32ChnId);
        s32Ret = RK_MPI_SYS_Bind(&stViChn[i], &stVpssChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind error %x: vi [%d, %d] -> vpss [%d, %d]",
                    s32Ret,
                    stViChn[i].s32DevId, stViChn[i].s32ChnId,
                    stVpssChn[i].s32DevId, stVpssChn[i].s32ChnId);
            goto __FAILED_VPSS;
        }
    }

    /* vo config init */
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        pstVoCtx[i]->s32LayerId = RK356X_VOP_LAYER_CLUSTER_0;
#if RK_AVS_RK356x
        pstVoCtx[i]->s32DevId = RK356X_VO_DEV_HD0;
        pstVoCtx[i]->stVoLayerAttr.enPixFormat = RK_FMT_YUV420SP;
        pstVoCtx[i]->stVoLayerAttr.stDispRect.u32Width = 1920;
        pstVoCtx[i]->stVoLayerAttr.stDispRect.u32Height = 1080;
#endif
#if RK_AVS_RK3588
        pstVoCtx[i]->s32DevId = RK3588_VO_DEV_MIPI;     // RK3588_VO_DEV_HDMI;
        pstVoCtx[i]->stVoLayerAttr.enPixFormat = RK_FMT_RGB888;
        pstVoCtx[i]->stVoLayerAttr.bDoubleFrame = RK_TRUE;
        pstVoCtx[i]->stVoLayerAttr.stDispRect.u32Width = 1080;
        pstVoCtx[i]->stVoLayerAttr.stDispRect.u32Height = 1920;
#endif
        pstVoCtx[i]->s32ChnId = i;

        pstVoCtx[i]->stVoCscAttr.enCscMatrix = VO_CSC_MATRIX_IDENTITY;
        pstVoCtx[i]->stVoCscAttr.u32Contrast = 50;
        pstVoCtx[i]->stVoCscAttr.u32Hue = 50;
        pstVoCtx[i]->stVoCscAttr.u32Luma = 50;
        pstVoCtx[i]->stVoCscAttr.u32Satuature = 50;

        pstVoCtx[i]->stVoLayerAttr.stDispRect.s32X = 0;
        pstVoCtx[i]->stVoLayerAttr.stDispRect.s32Y = 0;
        pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Width = pstVoCtx[i]->stVoLayerAttr.stDispRect.u32Width;
        pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Height = pstVoCtx[i]->stVoLayerAttr.stDispRect.u32Height;
        if (i == 0) {
            pstVoCtx[i]->stVoLayerAttr.u32DispFrmRt = 30;
            pstVoCtx[i]->stVoChnAttr.stRect.s32X = 0;
            pstVoCtx[i]->stVoChnAttr.stRect.s32Y = 0;
        } else if (i == 1) {
            pstVoCtx[i]->stVoLayerAttr.u32DispFrmRt = 30;
            pstVoCtx[i]->stVoChnAttr.stRect.s32X =
                pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Width / 2;
            pstVoCtx[i]->stVoChnAttr.stRect.s32Y = 0;
        } else if (i == 2) {
            pstVoCtx[i]->stVoLayerAttr.u32DispFrmRt = 30;
            pstVoCtx[i]->stVoChnAttr.stRect.s32X = 0;
            pstVoCtx[i]->stVoChnAttr.stRect.s32Y =
                pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Height / 2;
        } else if (i == 3) {
            pstVoCtx[i]->stVoLayerAttr.u32DispFrmRt = 30;
            pstVoCtx[i]->stVoChnAttr.stRect.s32X =
                pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Width / 2;
            pstVoCtx[i]->stVoChnAttr.stRect.s32Y =
                pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Height / 2;
        }
        pstVoCtx[i]->stVoChnAttr.stRect.u32Width =
                pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Width / 2;
            pstVoCtx[i]->stVoChnAttr.stRect.u32Height =
                pstVoCtx[i]->stVoLayerAttr.stImageSize.u32Height / 2;
        pstVoCtx[i]->stVoChnAttr.bDeflicker = RK_FALSE;
        pstVoCtx[i]->stVoChnAttr.u32Priority = 1;
        pstVoCtx[i]->stVoChnAttr.u32FgAlpha = 128;
        pstVoCtx[i]->stVoChnAttr.u32BgAlpha = 0;

        /* vo creat */
        s32Ret = create_vo(pstVoCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vo [%d, %d, %d] create failed: %x",
                    pstVoCtx[i]->s32DevId,
                    pstVoCtx[i]->s32LayerId,
                    pstVoCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED_VO;
        }
    }

    /* bind vpss -> vo */
    for (RK_S32 i = 0; i < AVS_STITCH_NUM; i++) {
        stVpssChn[i].enModId  = RK_ID_VPSS;
        stVpssChn[i].s32DevId = pstVpssCtx[i]->s32GrpId;
        stVpssChn[i].s32ChnId = pstVpssCtx[i]->s32ChnId;

        stVoChn[i].enModId   = RK_ID_VO;
        stVoChn[i].s32DevId  = 0;
        stVoChn[i].s32ChnId  = pstVoCtx[i]->s32ChnId;

        RK_LOGD("vpss [%d, %d] -> vo [%d, %d]",
                stVpssChn[i].s32DevId , stVpssChn[i].s32ChnId,
                stVoChn[i].s32DevId , stVoChn[i].s32ChnId);
        s32Ret = RK_MPI_SYS_Bind(&stVpssChn[i], &stVoChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind error %x: vpss [%d, %d] -> vo [%d, %d]",
                    s32Ret,
                    stVpssChn[i].s32DevId , stVpssChn[i].s32ChnId,
                    stVoChn[i].s32DevId , stVoChn[i].s32ChnId);
            goto __FAILED_VO;
        }
    }
    /* do somethings */
    while (loopCount < 100000) {
        usleep(30*1000);
        loopCount++;
    }

    /* unbind vpss -> vo */
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        s32Ret = RK_MPI_SYS_UnBind(&stVpssChn[i], &stVoChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("unbind error %x: vpss [%d, %d] -> avs [%d, %d]",
                    s32Ret,
                    stVpssChn[i].s32DevId, stVpssChn[i].s32ChnId,
                    stVoChn[i].s32DevId, stVoChn[i].s32ChnId);
            goto __FAILED;
        }
    }

    /* unbind vi -> vpss */
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        s32Ret = RK_MPI_SYS_UnBind(&stViChn[i], &stVpssChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("unbind error %x: vi [%d, %d] -> vpss [%d, %d]",
                    s32Ret,
                    stViChn[i].s32DevId, stViChn[i].s32ChnId,
                    stVpssChn[i].s32DevId, stVpssChn[i].s32ChnId);
            goto __FAILED;
        }
    }

__FAILED_VO:
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        /* destroy vo */
        s32Ret = destroy_vo(pstVoCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vo [%d, %d, %d] create failed: %x",
                    pstVoCtx[i]->s32DevId,
                    pstVoCtx[i]->s32ChnId,
                    pstVoCtx[i]->s32LayerId,
                    s32Ret);
            goto __FAILED;
        }
    }

__FAILED_VPSS:
    /* destroy vpss*/
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        s32Ret = destroy_vpss(pstVpssCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("destroy vpss [%d, %d] error %x ", s32Ret);
            goto __FAILED;
        }
    }

__FAILED_VI:
    /* destroy vi*/
    for (RK_S32 i = 0; i < AVS_STITCH_NUM; i++) {
        s32Ret = destroy_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("destroy vi [%d, %d] error %x",
                    pstViCtx[i]->s32DevId, pstViCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED;
        }
    }

__FAILED:
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        rk_safe_free(pstViCtx[i]);
        rk_safe_free(pstVpssCtx[i]);
        rk_safe_free(pstVoCtx[i]);
    }

    return s32Ret;
}

static RK_S32 test_vi_avs_vo_loop(RK_VOID) {
    RK_S32 s32Ret = RK_FAILURE;
    VI_CFG_S  *pstViCtx[AVS_STITCH_NUM];
    AVS_CFG_S *pstAvsCtx;
    VO_CFG_S  *pstVoCtx;
    MPP_CHN_S stViChn[AVS_STITCH_NUM],
              stAvsChn[AVS_STITCH_NUM],
              stVoChn;
    RK_S32 loopCount = 0;
    RK_S32 i = 0;

    for (i = 0; i < AVS_STITCH_NUM; i++) {
        pstViCtx[i] = reinterpret_cast<VI_CFG_S *>(malloc(sizeof(VI_CFG_S)));
        memset(pstViCtx[i], 0, sizeof(VI_CFG_S));
    }

    pstAvsCtx = reinterpret_cast<AVS_CFG_S *>(malloc(sizeof(AVS_CFG_S)));
    pstVoCtx  = reinterpret_cast<VO_CFG_S *>(malloc(sizeof(VO_CFG_S)));
    memset(pstAvsCtx, 0, sizeof(AVS_CFG_S));
    memset(pstVoCtx, 0, sizeof(VO_CFG_S));

    for (i = 0; i < AVS_STITCH_NUM; i++) {
    /* vi config init */
        pstViCtx[i]->s32DevId = i;
        pstViCtx[i]->s32PipeId = pstViCtx[i]->s32DevId;
        pstViCtx[i]->s32ChnId = 0;      // main path
        if (AVS_STITCH_NUM == 2) {
            pstViCtx[i]->stViChnAttr.stSize.u32Width = 2688;
            pstViCtx[i]->stViChnAttr.stSize.u32Height = 1520;
        } else if (AVS_STITCH_NUM == 4 || AVS_STITCH_NUM == 6) {
            pstViCtx[i]->stViChnAttr.stSize.u32Width = 2560;
            pstViCtx[i]->stViChnAttr.stSize.u32Height = 1520;
        }
        pstViCtx[i]->stViChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
#if RK_AVS_RK356x
        if (i == 0) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video5", strlen("/dev/video5"));  // 15FPS
        } else if (i == 1) {
            pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 15;
            memcpy(pstViCtx[i]->stViChnAttr.stIspOpt.aEntityName, "/dev/video14", strlen("/dev/video14"));  // 30FPS
        }
#endif
#if RK_AVS_RK3588
        pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
#endif
        pstViCtx[i]->stViChnAttr.u32Depth = 2;
        pstViCtx[i]->stViChnAttr.enPixelFormat = RK_FMT_YUV420SP;
        pstViCtx[i]->stViChnAttr.enCompressMode = COMPRESS_MODE_NONE;
        pstViCtx[i]->stViChnAttr.stFrameRate.s32SrcFrameRate = -1;
        pstViCtx[i]->stViChnAttr.stFrameRate.s32DstFrameRate = -1;

    /* vi create */
        s32Ret = create_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vi [%d, %d] init failed: %x",
                    pstViCtx[i]->s32DevId, pstViCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED_VI;
        }
    }

    /* avs config init */
    pstAvsCtx->s32GrpId = 0;
    pstAvsCtx->s32PipeId = 0;
    pstAvsCtx->s32ChnId = 0;
    pstAvsCtx->u32AvsPipeCnt = AVS_STITCH_NUM;
    pstAvsCtx->u32AvsChnCnt = 1;

    pstAvsCtx->stAvsGrpAttr.enMode = AVS_MODE_BLEND;
    if (AVS_STITCH_NUM == 2) {
        pstAvsCtx->u32OutW = 5088;
        pstAvsCtx->u32OutH = 1520;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
            sizeof("/usr/data/demo_data/2x/multiBand_5088x1520/"),
            "/usr/data/demo_data/2x/multiBand_5088x1520/");
    } else if (AVS_STITCH_NUM == 4) {
        pstAvsCtx->u32OutW = 5440;
        pstAvsCtx->u32OutH = 2700;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
            sizeof("/usr/data/demo_data/4x/multiBand_5440x2700/"),
            "/usr/data/demo_data/4x/multiBand_5440x2700/");
    } else if (AVS_STITCH_NUM == 6) {
        // pstAvsCtx->u32OutW = 9120;
        // pstAvsCtx->u32OutH = 2560;
        // snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
        //     sizeof("/usr/data/demo_data/6x/no_fuse_9120x2560/"),
        //     "/usr/data/demo_data/6x/no_fuse_9120x2560/");
        // pstAvsCtx->stAvsGrpAttr.enMode = AVS_MODE_NOBLEND_HOR;
        pstAvsCtx->u32OutW = 8192;
        pstAvsCtx->u32OutH = 2700;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
                 sizeof("/usr/share/avs_mesh/"),
                 "/usr/share/avs_mesh/");
    }
    pstAvsCtx->stAvsGrpAttr.stLUT.enAccuracy                 = AVS_LUT_ACCURACY_HIGH;
    pstAvsCtx->enOutCmpMode                                  = COMPRESS_MODE_NONE;

    pstAvsCtx->stAvsModParam.u32WorkingSetSize = 67 * 1024;
    pstAvsCtx->stAvsGrpAttr.u32PipeNum                       = pstAvsCtx->u32AvsPipeCnt;
    pstAvsCtx->stAvsGrpAttr.stGainAttr.enMode                = AVS_GAIN_MODE_AUTO;

    pstAvsCtx->stAvsGrpAttr.stOutAttr.enPrjMode              = AVS_PROJECTION_EQUIRECTANGULAR;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stCenter.s32X          = pstAvsCtx->u32OutW / 2;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stCenter.s32Y          = pstAvsCtx->u32OutH / 2;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stFOV.u32FOVX          = 36000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stFOV.u32FOVY          = 18000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Roll  = 9000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Pitch = 9000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Yaw   = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Roll     = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Pitch    = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Yaw      = 0;

    pstAvsCtx->stAvsGrpAttr.bSyncPipe                     = RK_TRUE;
    pstAvsCtx->stAvsGrpAttr.stFrameRate.s32SrcFrameRate   = -1;
    pstAvsCtx->stAvsGrpAttr.stFrameRate.s32DstFrameRate   = -1;

    pstAvsCtx->stAvsChnAttr[0].enCompressMode              = pstAvsCtx->enOutCmpMode;
    pstAvsCtx->stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
    pstAvsCtx->stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
    pstAvsCtx->stAvsChnAttr[0].u32Depth                    = 0;
    pstAvsCtx->stAvsChnAttr[0].u32Width                    = pstAvsCtx->u32OutW;
    pstAvsCtx->stAvsChnAttr[0].u32Height                   = pstAvsCtx->u32OutH;
    pstAvsCtx->stAvsChnAttr[0].enDynamicRange              = DYNAMIC_RANGE_SDR8;

    /* avs create */
    s32Ret = create_avs(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create avs [%d, %d, %d] error %x: ",
                pstAvsCtx->s32GrpId,
                pstAvsCtx->s32PipeId,
                pstAvsCtx->s32ChnId,
                s32Ret);
        return s32Ret;
    }

    /* bind vi -> avs */
    for (RK_S32 i = 0; i < AVS_STITCH_NUM; i++) {
        stViChn[i].enModId    = RK_ID_VI;
        stViChn[i].s32DevId   = pstViCtx[i]->s32DevId;
        stViChn[i].s32ChnId   = pstViCtx[i]->s32ChnId;

        stAvsChn[i].enModId   = RK_ID_AVS;
        stAvsChn[i].s32DevId  = pstAvsCtx->s32GrpId;
        stAvsChn[i].s32ChnId  = i;

        RK_LOGD("vi [%d, %d] -> avs [%d, %d]",
                stViChn[i].s32DevId , stViChn[i].s32ChnId,
                stAvsChn[i].s32DevId , stAvsChn[i].s32ChnId);
        s32Ret = RK_MPI_SYS_Bind(&stViChn[i], &stAvsChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind error %x: vi [%d, %d] -> avs [%d, %d]",
                    s32Ret,
                    stViChn[i].s32DevId , stViChn[i].s32ChnId,
                    stAvsChn[i].s32DevId , stAvsChn[i].s32ChnId);
            goto __FAILED_AVS;
        }
    }

    /* vo config init */
    pstVoCtx->s32LayerId = RK356X_VOP_LAYER_CLUSTER_0;
#if RK_AVS_RK356x
    pstVoCtx->s32DevId = RK356X_VO_DEV_HD0;
    pstVoCtx->stVoLayerAttr.enPixFormat = RK_FMT_YUV420SP;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Width = 1920;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Height = 1080;
#endif
#if RK_AVS_RK3588
    pstVoCtx->s32DevId = RK3588_VO_DEV_HDMI;     // RK3588_VO_DEV_MIPI;
    pstVoCtx->stVoLayerAttr.enPixFormat = RK_FMT_RGB888;
    pstVoCtx->stVoLayerAttr.bDoubleFrame = RK_TRUE;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Width = 1920;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Height = 1080;
#endif
    pstVoCtx->s32ChnId = 0;
    pstVoCtx->stVoCscAttr.enCscMatrix = VO_CSC_MATRIX_IDENTITY;
    pstVoCtx->stVoCscAttr.u32Contrast = 50;
    pstVoCtx->stVoCscAttr.u32Hue = 50;
    pstVoCtx->stVoCscAttr.u32Luma = 50;
    pstVoCtx->stVoCscAttr.u32Satuature = 50;

    pstVoCtx->stVoLayerAttr.stDispRect.s32X = 0;
    pstVoCtx->stVoLayerAttr.stDispRect.s32Y = 0;
    pstVoCtx->stVoLayerAttr.stImageSize.u32Width =
        pstVoCtx->stVoLayerAttr.stDispRect.u32Width;
    pstVoCtx->stVoLayerAttr.stImageSize.u32Height =
        pstVoCtx->stVoLayerAttr.stDispRect.u32Height;

    pstVoCtx->stVoLayerAttr.u32DispFrmRt = 30;
    pstVoCtx->stVoChnAttr.stRect.s32X = 0;
    pstVoCtx->stVoChnAttr.stRect.s32Y = 0;
    pstVoCtx->stVoChnAttr.stRect.u32Width =
        pstVoCtx->stVoLayerAttr.stImageSize.u32Width;
    pstVoCtx->stVoChnAttr.stRect.u32Height =
        pstVoCtx->stVoLayerAttr.stImageSize.u32Height;
    pstVoCtx->stVoChnAttr.bDeflicker = RK_FALSE;
    pstVoCtx->stVoChnAttr.u32Priority = 1;
    pstVoCtx->stVoChnAttr.u32FgAlpha = 128;
    pstVoCtx->stVoChnAttr.u32BgAlpha = 0;

    /* vo creat */
    s32Ret = create_vo(pstVoCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] create failed: %x",
                pstVoCtx->s32DevId,
                pstVoCtx->s32LayerId,
                pstVoCtx->s32ChnId,
                s32Ret);
        goto __FAILED_VO;
    }

    /* bind avs -> vo */
    stAvsChn[0].enModId  = RK_ID_AVS;
    stAvsChn[0].s32DevId = 0;
    stAvsChn[0].s32ChnId = 0;
    stVoChn.enModId   = RK_ID_VO;
    stVoChn.s32DevId  = 0;
    stVoChn.s32ChnId  = 0;

    RK_LOGD("avs [%d, %d] -> vo [%d, %d]",
            stAvsChn[0].s32DevId , stAvsChn[0].s32ChnId,
            stVoChn.s32DevId , stVoChn.s32ChnId);
    s32Ret = RK_MPI_SYS_Bind(&stAvsChn[0], &stVoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind error %x: avs [%d, %d] -> vo [%d, %d]",
                s32Ret,
                stAvsChn[0].s32DevId, stAvsChn[0].s32ChnId,
                stVoChn.s32DevId, stVoChn.s32ChnId);
        goto __FAILED_VO;
    }

    /* thread: do somethings */
    while (loopCount < 100000) {
        usleep(30*1000);
        loopCount++;
    }

    /* unbind avs -> vo */
    s32Ret = RK_MPI_SYS_UnBind(&stAvsChn[0], &stVoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind error %x: avs [%d, %d] -> vo [%d, %d]",
                s32Ret,
                stAvsChn[0].s32DevId, stAvsChn[0].s32ChnId,
                stVoChn.s32DevId, stVoChn.s32ChnId);
        goto __FAILED;
    }

    /* unbind vi -> avs */
    s32Ret = RK_MPI_SYS_UnBind(&stViChn[0], &stAvsChn[0]);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind error %x: vi [%d, %d] -> avs [%d, %d]",
                s32Ret,
                stViChn[0].s32DevId, stViChn[0].s32ChnId,
                stAvsChn[0].s32DevId, stAvsChn[0].s32ChnId);
        goto __FAILED;
    }

__FAILED_VO:
    /* destroy vo */
    s32Ret = destroy_vo(pstVoCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] destory failed: %x",
                pstVoCtx->s32DevId,
                pstVoCtx->s32ChnId,
                pstVoCtx->s32LayerId,
                s32Ret);
        goto __FAILED;
    }

__FAILED_AVS:
    /* destroy avs */
    s32Ret = destroy_avs(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("destroy avs [%d, %d, %d] error %x: ",
                pstAvsCtx->s32GrpId, pstAvsCtx->s32PipeId,
                pstAvsCtx->s32ChnId,
                s32Ret);
        goto __FAILED;
    }

__FAILED_VI:
    /* destroy vi*/
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        s32Ret = destroy_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("destroy vi [%d, %d] error %x",
                    pstViCtx[i]->s32DevId, pstViCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED;
        }
    }

__FAILED:
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        rk_safe_free(pstViCtx[i]);
    }
    rk_safe_free(pstAvsCtx);
    rk_safe_free(pstVoCtx);

    return s32Ret;
}

static RK_S32 test_afbc_vi_avs_vo_loop(RK_VOID) {
    RK_S32 s32Ret = RK_FAILURE;
    VI_CFG_S  *pstViCtx[AVS_STITCH_NUM];
    AVS_CFG_S *pstAvsCtx;
    VO_CFG_S  *pstVoCtx;
    MPP_CHN_S stViChn[AVS_STITCH_NUM],
              stAvsChn[AVS_STITCH_NUM],
              stVoChn;
    RK_S32 loopCount = 0;
    RK_S32 i = 0;

    for (i = 0; i < AVS_STITCH_NUM; i++) {
        pstViCtx[i] = reinterpret_cast<VI_CFG_S *>(malloc(sizeof(VI_CFG_S)));
        memset(pstViCtx[i], 0, sizeof(VI_CFG_S));
    }

    pstAvsCtx = reinterpret_cast<AVS_CFG_S *>(malloc(sizeof(AVS_CFG_S)));
    pstVoCtx  = reinterpret_cast<VO_CFG_S *>(malloc(sizeof(VO_CFG_S)));
    memset(pstAvsCtx, 0, sizeof(AVS_CFG_S));
    memset(pstVoCtx, 0, sizeof(VO_CFG_S));

    for (i = 0; i < AVS_STITCH_NUM; i++) {
    /* vi config init */
        pstViCtx[i]->s32DevId = i;
        pstViCtx[i]->s32PipeId = pstViCtx[i]->s32DevId;
        pstViCtx[i]->s32ChnId = 2;  // fbc path
        if (AVS_STITCH_NUM == 2) {
            pstViCtx[i]->stViChnAttr.stSize.u32Width = 2688;
            pstViCtx[i]->stViChnAttr.stSize.u32Height = 1520;
        } else if (AVS_STITCH_NUM == 4 || AVS_STITCH_NUM == 6) {
            pstViCtx[i]->stViChnAttr.stSize.u32Width = 2560;
            pstViCtx[i]->stViChnAttr.stSize.u32Height = 1520;
        }
        pstViCtx[i]->stViChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
#if RK_AVS_RK3588
        pstViCtx[i]->stViChnAttr.stIspOpt.u32BufCount = 10;
#endif
        pstViCtx[i]->stViChnAttr.u32Depth = 2;
        pstViCtx[i]->stViChnAttr.enPixelFormat = RK_FMT_YUV420SP;
        pstViCtx[i]->stViChnAttr.enCompressMode = COMPRESS_AFBC_16x16;
        pstViCtx[i]->stViChnAttr.stFrameRate.s32SrcFrameRate = -1;
        pstViCtx[i]->stViChnAttr.stFrameRate.s32DstFrameRate = -1;

    /* vi create */
        s32Ret = create_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("vi [%d, %d] init failed: %x",
                    pstViCtx[i]->s32DevId, pstViCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED_VI;
        }
    }

    /* avs config init */
    pstAvsCtx->s32GrpId = 0;
    pstAvsCtx->s32PipeId = 0;
    pstAvsCtx->s32ChnId = 0;
    pstAvsCtx->u32AvsPipeCnt = AVS_STITCH_NUM;
    pstAvsCtx->u32AvsChnCnt = 1;

    pstAvsCtx->stAvsGrpAttr.enMode                       = AVS_MODE_BLEND;
    if (AVS_STITCH_NUM == 6) {
        /* pstAvsCtx->u32OutW = 9120;
        pstAvsCtx->u32OutH = 2560;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
            sizeof("/usr/data/demo_data/6x/no_fuse_9120x2560/"),
            "/usr/data/demo_data/6x/no_fuse_9120x2560/");
        pstAvsCtx->stAvsGrpAttr.enMode                       = AVS_MODE_NOBLEND_HOR; */
        pstAvsCtx->u32OutW = 8192;
        pstAvsCtx->u32OutH = 2700;
        snprintf(pstAvsCtx->stAvsGrpAttr.stLUT.aFilePath,
                 sizeof("/usr/share/avs_mesh/"),
                 "/usr/share/avs_mesh/");
    }
    pstAvsCtx->stAvsGrpAttr.stLUT.enAccuracy                 = AVS_LUT_ACCURACY_HIGH;
    pstAvsCtx->enOutCmpMode                                  = COMPRESS_AFBC_16x16;

    pstAvsCtx->stAvsModParam.u32WorkingSetSize               = 67 * 1024;
    pstAvsCtx->stAvsGrpAttr.u32PipeNum                       = pstAvsCtx->u32AvsPipeCnt;
    pstAvsCtx->stAvsGrpAttr.stGainAttr.enMode                = AVS_GAIN_MODE_AUTO;

    pstAvsCtx->stAvsGrpAttr.stOutAttr.enPrjMode              = AVS_PROJECTION_EQUIRECTANGULAR;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stCenter.s32X          = pstAvsCtx->u32OutW / 2;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stCenter.s32Y          = pstAvsCtx->u32OutH / 2;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stFOV.u32FOVX          = 36000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stFOV.u32FOVY          = 18000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Roll  = 9000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Pitch = 9000;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stORIRotation.s32Yaw   = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Roll     = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Pitch    = 0;
    pstAvsCtx->stAvsGrpAttr.stOutAttr.stRotation.s32Yaw      = 0;

    pstAvsCtx->stAvsGrpAttr.bSyncPipe                     = RK_TRUE;
    pstAvsCtx->stAvsGrpAttr.stFrameRate.s32SrcFrameRate   = -1;
    pstAvsCtx->stAvsGrpAttr.stFrameRate.s32DstFrameRate   = -1;

    pstAvsCtx->stAvsChnAttr[0].enCompressMode              = pstAvsCtx->enOutCmpMode;
    pstAvsCtx->stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
    pstAvsCtx->stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
    pstAvsCtx->stAvsChnAttr[0].u32Depth                    = 0;
    pstAvsCtx->stAvsChnAttr[0].u32Width                    = pstAvsCtx->u32OutW;
    pstAvsCtx->stAvsChnAttr[0].u32Height                   = pstAvsCtx->u32OutH;
    pstAvsCtx->stAvsChnAttr[0].enDynamicRange              = DYNAMIC_RANGE_SDR8;

    /* avs create */
    s32Ret = create_avs(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create avs [%d, %d, %d] error %x: ",
                pstAvsCtx->s32GrpId,
                pstAvsCtx->s32PipeId,
                pstAvsCtx->s32ChnId,
                s32Ret);
        return s32Ret;
    }

    /* bind vi -> avs */
    for (RK_S32 i = 0; i < AVS_STITCH_NUM; i++) {
        stViChn[i].enModId    = RK_ID_VI;
        stViChn[i].s32DevId   = pstViCtx[i]->s32DevId;
        stViChn[i].s32ChnId   = pstViCtx[i]->s32ChnId;

        stAvsChn[i].enModId   = RK_ID_AVS;
        stAvsChn[i].s32DevId  = pstAvsCtx->s32GrpId;
        stAvsChn[i].s32ChnId  = i;

        RK_LOGD("vi [%d, %d] -> avs [%d, %d]",
                stViChn[i].s32DevId , stViChn[i].s32ChnId,
                stAvsChn[i].s32DevId , stAvsChn[i].s32ChnId);
        s32Ret = RK_MPI_SYS_Bind(&stViChn[i], &stAvsChn[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("bind error %x: vi [%d, %d] -> avs [%d, %d]",
                    s32Ret,
                    stViChn[i].s32DevId , stViChn[i].s32ChnId,
                    stAvsChn[i].s32DevId , stAvsChn[i].s32ChnId);
            goto __FAILED_AVS;
        }
    }

    /* vo config init */
    pstVoCtx->s32LayerId = RK356X_VOP_LAYER_CLUSTER_0;
#if RK_AVS_RK356x
    pstVoCtx->s32DevId = RK356X_VO_DEV_HD0;
    pstVoCtx->stVoLayerAttr.enPixFormat = RK_FMT_YUV420SP;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Width = 1920;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Height = 1080;
#endif
#if RK_AVS_RK3588
    pstVoCtx->s32DevId = RK3588_VO_DEV_HDMI;     // RK3588_VO_DEV_MIPI;
    pstVoCtx->stVoLayerAttr.enPixFormat = RK_FMT_RGB888;
    pstVoCtx->stVoLayerAttr.bDoubleFrame = RK_TRUE;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Width = 1920;
    pstVoCtx->stVoLayerAttr.stDispRect.u32Height = 1080;
#endif
    pstVoCtx->s32ChnId = 0;
    pstVoCtx->stVoCscAttr.enCscMatrix = VO_CSC_MATRIX_IDENTITY;
    pstVoCtx->stVoCscAttr.u32Contrast = 50;
    pstVoCtx->stVoCscAttr.u32Hue = 50;
    pstVoCtx->stVoCscAttr.u32Luma = 50;
    pstVoCtx->stVoCscAttr.u32Satuature = 50;

    pstVoCtx->stVoLayerAttr.stDispRect.s32X = 0;
    pstVoCtx->stVoLayerAttr.stDispRect.s32Y = 0;
    pstVoCtx->stVoLayerAttr.stImageSize.u32Width =
        pstVoCtx->stVoLayerAttr.stDispRect.u32Width;
    pstVoCtx->stVoLayerAttr.stImageSize.u32Height =
        pstVoCtx->stVoLayerAttr.stDispRect.u32Height;

    pstVoCtx->stVoLayerAttr.u32DispFrmRt = 30;
    pstVoCtx->stVoChnAttr.stRect.s32X = 0;
    pstVoCtx->stVoChnAttr.stRect.s32Y = 0;
    pstVoCtx->stVoChnAttr.stRect.u32Width =
        pstVoCtx->stVoLayerAttr.stImageSize.u32Width;
    pstVoCtx->stVoChnAttr.stRect.u32Height =
        pstVoCtx->stVoLayerAttr.stImageSize.u32Height;
    pstVoCtx->stVoChnAttr.bDeflicker = RK_FALSE;
    pstVoCtx->stVoChnAttr.u32Priority = 1;
    pstVoCtx->stVoChnAttr.u32FgAlpha = 128;
    pstVoCtx->stVoChnAttr.u32BgAlpha = 0;

    /* vo creat */
    s32Ret = create_vo(pstVoCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] create failed: %x",
                pstVoCtx->s32DevId,
                pstVoCtx->s32LayerId,
                pstVoCtx->s32ChnId,
                s32Ret);
        goto __FAILED_VO;
    }

    /* bind avs -> vo */
    stAvsChn[0].enModId  = RK_ID_AVS;
    stAvsChn[0].s32DevId = 0;
    stAvsChn[0].s32ChnId = 0;
    stVoChn.enModId   = RK_ID_VO;
    stVoChn.s32DevId  = 0;
    stVoChn.s32ChnId  = 0;

    RK_LOGD("avs [%d, %d] -> vo [%d, %d]",
            stAvsChn[0].s32DevId , stAvsChn[0].s32ChnId,
            stVoChn.s32DevId , stVoChn.s32ChnId);
    s32Ret = RK_MPI_SYS_Bind(&stAvsChn[0], &stVoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind error %x: avs [%d, %d] -> vo [%d, %d]",
                s32Ret,
                stAvsChn[0].s32DevId, stAvsChn[0].s32ChnId,
                stVoChn.s32DevId, stVoChn.s32ChnId);
        goto __FAILED_VO;
    }

    /* thread: do somethings */
    while (loopCount < 100000) {
        usleep(30*1000);
        loopCount++;
    }

    /* unbind avs -> vo */
    s32Ret = RK_MPI_SYS_UnBind(&stAvsChn[0], &stVoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind error %x: avs [%d, %d] -> vo [%d, %d]",
                s32Ret,
                stAvsChn[0].s32DevId, stAvsChn[0].s32ChnId,
                stVoChn.s32DevId, stVoChn.s32ChnId);
        goto __FAILED;
    }

    /* unbind vi -> avs */
    s32Ret = RK_MPI_SYS_UnBind(&stViChn[0], &stAvsChn[0]);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind error %x: vi [%d, %d] -> avs [%d, %d]",
                s32Ret,
                stViChn[0].s32DevId, stViChn[0].s32ChnId,
                stAvsChn[0].s32DevId, stAvsChn[0].s32ChnId);
        goto __FAILED;
    }

__FAILED_VO:
    /* destroy vo */
    s32Ret = destroy_vo(pstVoCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vo [%d, %d, %d] destory failed: %x",
                pstVoCtx->s32DevId,
                pstVoCtx->s32ChnId,
                pstVoCtx->s32LayerId,
                s32Ret);
        goto __FAILED;
    }


__FAILED_AVS:
    /* destroy avs */
    s32Ret = destroy_avs(pstAvsCtx);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("destroy avs [%d, %d, %d] error %x: ",
                pstAvsCtx->s32GrpId, pstAvsCtx->s32PipeId,
                pstAvsCtx->s32ChnId,
                s32Ret);
        goto __FAILED;
    }

__FAILED_VI:
    /* destroy vi*/
    for (RK_S32 i = 0; i < AVS_STITCH_NUM; i++) {
        s32Ret = destroy_vi(pstViCtx[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("destroy vi [%d, %d] error %x",
                    pstViCtx[i]->s32DevId, pstViCtx[i]->s32ChnId,
                    s32Ret);
            goto __FAILED;
        }
    }

__FAILED:
    for (i = 0; i < AVS_STITCH_NUM; i++) {
        rk_safe_free(pstViCtx[i]);
    }
    rk_safe_free(pstVoCtx);
    rk_safe_free(pstAvsCtx);

    return s32Ret;
}

static const char *const usages[] = {
    "./rk_mpi_avs_test -m 2",
    RK_NULL,
};

int main(int argc, const char **argv) {
    RK_S32 s32Ret = RK_FAILURE;
    RK_S32 s32Mode = 0;

    RK_LOGE("avs test running enter!");

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('m', "mode", &s32Mode,
                    "test mode(default 2) \n\t"
                    "[test]0: avs get&release frame \n\t"
                    "[test]1: 2 vi -> 2 vpss -> vo. Quad stack \n\t"
                    "[test]2: 6 vi -> avs -> vo. No AFBC link, Normal projection. \n\t"
                    "[test]3: 2 vi -> avs -> vo. AFBC link, Normal projection. \n\t"
                    , NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");
    argc = argparse_parse(&argparse, argc, argv);

    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        goto __FAILED;
    }
    switch (s32Mode) {
        case TEST_MODE_AVS_ONLY:
            RK_LOGE("enter test_vi_get_release_frame_loop");
            s32Ret = test_avs_get_release_frame_loop();
        break;
        case TEST_MODE_VI_VPSS_VO:
            RK_LOGE("enter test_vi_vpss_vo_loop");
            s32Ret = test_vi_vpss_vo_loop();
        break;
        case TEST_MODE_VI_AVS_VO:
            RK_LOGE("enter test_vi_avs_vo_loop");
            s32Ret = test_vi_avs_vo_loop();
        break;
        case TEST_MODE_AFBC_VI_AVS_VO:
            RK_LOGE("enter test_afbc_vi_avs_vo_loop");
            s32Ret = test_afbc_vi_avs_vo_loop();
        break;
        default:
            RK_LOGE("no support such test mode: %d", s32Mode);
        break;
    }
__FAILED:
    RK_LOGE("test running exit: %d", s32Ret);
    RK_MPI_SYS_Exit();

    return 0;
}
