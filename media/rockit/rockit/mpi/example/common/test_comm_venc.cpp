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
 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include "test_comm_venc.h"
#include "test_comm_imgproc.h"

#include "rk_comm_venc.h"
#include "rk_common.h"
#include "rk_debug.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"

#define TEST_VENC_PATH_LENGTH         128
#define TEST_VENC_TIME_OUT_MS         100

typedef struct test_venc_getstream_s {
     RK_BOOL bThreadStart;
     FILE *fp;
     pthread_t VencPid;
     MB_POOL pool;
     COMMON_TEST_VENC_CTX_S stVencCtx;
} TEST_VENC_THREAD_S;

// get stream thread info
static TEST_VENC_THREAD_S gGSThread[VENC_MAX_CHN_NUM];
// send frame thread info
static TEST_VENC_THREAD_S gSFThread[VENC_MAX_CHN_NUM];


static RK_S32 TEST_VENC_StartSendFrame(COMMON_TEST_VENC_CTX_S *vencCtx);
static RK_S32 TEST_VENC_StopSendFrame(VENC_CHN VencChn);
static RK_S32 TEST_VENC_StartGetStream(COMMON_TEST_VENC_CTX_S *vencCtx);
static RK_S32 TEST_VENC_StopGetStream(VENC_CHN VencChn);
static RK_VOID TEST_VECN_DestroyPool(VENC_CHN VencChn);

RK_S32 TEST_VENC_Create(COMMON_TEST_VENC_CTX_S *vencCtx) {
    RK_S32                  s32Ret = RK_SUCCESS;
    VENC_CHN_ATTR_S         stAttr;

    memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));

    stAttr.stVencAttr.enType = vencCtx->enType;
    if (vencCtx->enType == RK_VIDEO_ID_AVC)
        stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
    else if (vencCtx->enType == RK_VIDEO_ID_HEVC)
        stAttr.stVencAttr.u32Profile = H265E_PROFILE_MAIN;
    stAttr.stVencAttr.enPixelFormat = vencCtx->enPixFmt;
    stAttr.stVencAttr.u32PicWidth = vencCtx->u32Width;
    stAttr.stVencAttr.u32PicHeight = vencCtx->u32Height;
    stAttr.stVencAttr.u32VirWidth = vencCtx->u32Width;
    stAttr.stVencAttr.u32VirHeight = vencCtx->u32Height;
    stAttr.stVencAttr.u32StreamBufCnt = vencCtx->u32StreamBufCnt;
    stAttr.stVencAttr.u32BufSize = vencCtx->u32Width * vencCtx->u32Height;
    s32Ret = RK_MPI_VENC_CreateChn(vencCtx->VencChn, &stAttr);

    return s32Ret;
}

RK_S32 TEST_VENC_Start(COMMON_TEST_VENC_CTX_S *vencCtx) {
    RK_S32                  s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S   stRecvParam;

    memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    stRecvParam.s32RecvPicNum = -1;

    s32Ret = TEST_VENC_Create(vencCtx);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VENC_StartRecvFrame(vencCtx->VencChn, &stRecvParam);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = TEST_VENC_StartSendFrame(vencCtx);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = TEST_VENC_StartGetStream(vencCtx);

    return s32Ret;
}

RK_S32 TEST_VENC_Stop(VENC_CHN VencChn) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = TEST_VENC_StopSendFrame(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = TEST_VENC_StopGetStream(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VENC_StopRecvFrame(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VENC_DestroyChn(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    TEST_VECN_DestroyPool(VencChn);

    return s32Ret;
}

// should TEST_VENC_Create first
RK_S32 TEST_VENC_SnapStart(COMMON_TEST_VENC_CTX_S *vencCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    VENC_RECV_PIC_PARAM_S stRecvParam;

    memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    stRecvParam.s32RecvPicNum = vencCtx->s32RecvPicNum;

    s32Ret = RK_MPI_VENC_StartRecvFrame(vencCtx->VencChn, &stRecvParam);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = TEST_VENC_StartSendFrame(vencCtx);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 TEST_VENC_SnapStop(VENC_CHN VencChn) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = TEST_VENC_StopSendFrame(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VENC_StopRecvFrame(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VENC_DestroyChn(VencChn);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    TEST_VECN_DestroyPool(VencChn);

    return s32Ret;
}

RK_S32 TEST_VENC_SnapProcess(COMMON_TEST_VENC_CTX_S *vencCtx) {
    FILE *fp = RK_NULL;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_SUCCESS;
    RK_S32 s32StreamCnt = 0;
    VENC_STREAM_S stVencStream;

    if (vencCtx->pSaveStreamPath != RK_NULL) {
        fp = fopen(vencCtx->pSaveStreamPath, "wb");
        if (fp == RK_NULL) {
            RK_LOGE("can't open file %s!", vencCtx->pSaveStreamPath);
            return RK_FAILURE;
        }
    }
    stVencStream.pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));

    while (1) {
        s32Ret = RK_MPI_VENC_GetStream(vencCtx->VencChn, &stVencStream, TEST_VENC_TIME_OUT_MS);
        if (s32Ret >= 0) {
            if (fp != RK_NULL) {
                pData = RK_MPI_MB_Handle2VirAddr(stVencStream.pstPack->pMbBlk);
                RK_MPI_SYS_MmzFlushCache(stVencStream.pstPack->pMbBlk, RK_TRUE);
                fwrite(pData, 1, stVencStream.pstPack->u32Len, fp);
                fflush(fp);
            }
            if (stVencStream.pstPack->bStreamEnd == RK_TRUE) {
                RK_LOGI("get chn %d stream %d", vencCtx->VencChn, stVencStream.u32Seq + 1);
                RK_LOGI("chn %d reach EOS stream", vencCtx->VencChn);
                RK_MPI_VENC_ReleaseStream(vencCtx->VencChn, &stVencStream);
                break;
            }
            RK_MPI_VENC_ReleaseStream(vencCtx->VencChn, &stVencStream);
        } else {
             usleep(1000llu);
        }
    }

    if (stVencStream.pstPack)
        free(stVencStream.pstPack);
    if (fp)
        fclose(fp);

    return RK_SUCCESS;
}

static RK_VOID TEST_VECN_DestroyPool(VENC_CHN VencChn) {
    if (gSFThread[VencChn].pool)
        RK_MPI_MB_DestroyPool(gSFThread[VencChn].pool);
}

static RK_VOID* TEST_VENC_GetVencStreamProc(RK_VOID *p) {
    TEST_VENC_THREAD_S *pstThreadInfo = (TEST_VENC_THREAD_S *)p;
    void *pData = RK_NULL;
    RK_S32 s32Ret = RK_SUCCESS;
    VENC_STREAM_S stVencStream;
    RK_S32 framecnt = 0;

    stVencStream.pstPack = reinterpret_cast<VENC_PACK_S *>(malloc(sizeof(VENC_PACK_S)));

    while (RK_TRUE == pstThreadInfo->bThreadStart) {
        s32Ret = RK_MPI_VENC_GetStream(pstThreadInfo->stVencCtx.VencChn, &stVencStream, TEST_VENC_TIME_OUT_MS);
        if (s32Ret >= 0) {
            char filename[128];
            FILE *file;
            snprintf(filename, sizeof(filename), "/data/rgn/venc_frame%d.jpg",framecnt);
            file = fopen(filename, "wb");
            if (pstThreadInfo->fp != RK_NULL) {
                pData = RK_MPI_MB_Handle2VirAddr(stVencStream.pstPack->pMbBlk);
                RK_MPI_SYS_MmzFlushCache(stVencStream.pstPack->pMbBlk, RK_TRUE);
                fwrite(pData, 1, stVencStream.pstPack->u32Len, pstThreadInfo->fp);
                fflush(pstThreadInfo->fp);
            }
            if (file) {
                RK_VOID *jpgData = RK_MPI_MB_Handle2VirAddr(stVencStream.pstPack->pMbBlk);
                RK_MPI_SYS_MmzFlushCache(stVencStream.pstPack->pMbBlk, RK_TRUE);
                fwrite(jpgData, 1, stVencStream.pstPack->u32Len, file);
                fflush(file);
                fclose(file);
            }
            if (stVencStream.pstPack->bStreamEnd == RK_TRUE) {
                RK_LOGI("get chn %d stream %d", pstThreadInfo->stVencCtx.VencChn, stVencStream.u32Seq + 1);
                RK_LOGI("chn %d reach EOS stream", pstThreadInfo->stVencCtx.VencChn);
                RK_MPI_VENC_ReleaseStream(pstThreadInfo->stVencCtx.VencChn, &stVencStream);
                break;
            }
            RK_MPI_VENC_ReleaseStream(pstThreadInfo->stVencCtx.VencChn, &stVencStream);
            framecnt++;
            RK_LOGI("==get franme cnt: %d", framecnt);
        } else {
             usleep(1000llu);
        }
    }

    if (stVencStream.pstPack)
        free(stVencStream.pstPack);

    return RK_NULL;
}

static RK_S32 TEST_VENC_StartGetStream(COMMON_TEST_VENC_CTX_S *vencCtx) {
    RK_S32 s32Ret = 0;

    if (vencCtx->pSaveStreamPath != RK_NULL) {
        gGSThread[vencCtx->VencChn].fp = fopen(vencCtx->pSaveStreamPath, "wb");
        if (gGSThread[vencCtx->VencChn].fp == RK_NULL) {
            RK_LOGE("can't open file %s!", vencCtx->pSaveStreamPath);
            return RK_FAILURE;
        }
    }
    gGSThread[vencCtx->VencChn].bThreadStart = RK_TRUE;
    memcpy(&gGSThread[vencCtx->VencChn].stVencCtx, vencCtx, sizeof(COMMON_TEST_VENC_CTX_S));

    s32Ret = pthread_create(&(gGSThread[vencCtx->VencChn].VencPid), 0,
                            TEST_VENC_GetVencStreamProc,
                            (RK_VOID *)&(gGSThread[vencCtx->VencChn]));
    if (s32Ret != 0) {
        return s32Ret;
    }

    return s32Ret;
}

static RK_S32 TEST_VENC_StopGetStream(VENC_CHN VencChn) {
    if (RK_TRUE == gGSThread[VencChn].bThreadStart) {
        gGSThread[VencChn].bThreadStart = RK_FALSE;
        pthread_join(gGSThread[VencChn].VencPid, 0);
        if (gGSThread[VencChn].fp != RK_NULL) {
            fclose(gGSThread[VencChn].fp);
            gGSThread[VencChn].fp = RK_NULL;
        }
    }

    return RK_SUCCESS;
}

static void* TEST_VENC_SendVencFrameProc(void *pArgs) {
    TEST_VENC_THREAD_S  *pstThreadInfo = (TEST_VENC_THREAD_S *)pArgs;
    RK_S32               s32Ret         = RK_SUCCESS;
    VENC_CHN             VencChn        = pstThreadInfo->stVencCtx.VencChn;
    RK_U8               *pVirAddr       = RK_NULL;
    RK_U32               u32SrcSize     = 0;
    MB_BLK               blk            = RK_NULL;
    RK_U32               u32BufferSize  = 0;
    RK_S32               s32FrameCount  = 0;
    RK_S32               s32ReachEOS    = 0;
    VENC_CHN_ATTR_S      stChnAttr;
    VIDEO_FRAME_INFO_S   stFrame;
    MB_POOL_CONFIG_S     stMbPoolCfg;
    PIC_BUF_ATTR_S       stPicBufAttr;
    MB_PIC_CAL_S         stMbPicCalResult;

    s32Ret = RK_MPI_VENC_GetChnAttr(VencChn, &stChnAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VENC_GetChnAttr failed 0x%x", s32Ret);
        return RK_NULL;
    }
    stPicBufAttr.u32Width = stChnAttr.stVencAttr.u32PicWidth;
    stPicBufAttr.u32Height = stChnAttr.stVencAttr.u32PicHeight;
    stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
    stPicBufAttr.enPixelFormat = pstThreadInfo->stVencCtx.enPixFmt;
    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return RK_NULL;
    }
    u32BufferSize = stMbPicCalResult.u32MBSize;
    memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
    stMbPoolCfg.u64MBSize = u32BufferSize;
    stMbPoolCfg.u32MBCnt  = pstThreadInfo->stVencCtx.u32StreamBufCnt;
    stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
    pstThreadInfo->pool = RK_MPI_MB_CreatePool(&stMbPoolCfg);

    while (RK_TRUE == pstThreadInfo->bThreadStart) {
        blk = RK_MPI_MB_GetMB(pstThreadInfo->pool, u32BufferSize, RK_TRUE);
        pVirAddr = reinterpret_cast<RK_U8 *>(RK_MPI_MB_Handle2VirAddr(blk));
        if (pstThreadInfo->fp != RK_NULL) {
            u32SrcSize = fread(pVirAddr, 1, u32BufferSize, pstThreadInfo->fp);
            if (u32SrcSize <= 0) {
                s32ReachEOS = 1;
            }
        } else {
            s32Ret = TEST_COMM_FillImage(pVirAddr, stChnAttr.stVencAttr.u32PicWidth,
                            stChnAttr.stVencAttr.u32PicHeight,
                            RK_MPI_CAL_COMM_GetHorStride(stMbPicCalResult.u32VirWidth,
                                                         pstThreadInfo->stVencCtx.enPixFmt),
                            stMbPicCalResult.u32VirHeight,
                            pstThreadInfo->stVencCtx.enPixFmt,
                            s32FrameCount);
            if (s32Ret != RK_SUCCESS) {
                RK_MPI_MB_ReleaseMB(blk);
                goto __FAILED;
            }
        }
        RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);
        memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
        stFrame.stVFrame.pMbBlk = blk;
        stFrame.stVFrame.u32Width = stChnAttr.stVencAttr.u32PicWidth;
        stFrame.stVFrame.u32Height = stChnAttr.stVencAttr.u32PicHeight;
        stFrame.stVFrame.u32VirWidth = stMbPicCalResult.u32VirWidth;
        stFrame.stVFrame.u32VirHeight = stMbPicCalResult.u32VirHeight;
        stFrame.stVFrame.enPixelFormat = pstThreadInfo->stVencCtx.enPixFmt;
        stFrame.stVFrame.u32FrameFlag |= s32ReachEOS ? FRAME_FLAG_SNAP_END : 0;
__RETRY:
        if (RK_FALSE == pstThreadInfo->bThreadStart) {
            break;
        }
        s32Ret = RK_MPI_VENC_SendFrame(pstThreadInfo->stVencCtx.VencChn,
                                       &stFrame,
                                       TEST_VENC_TIME_OUT_MS);
        if (s32Ret < 0) {
            usleep(10000llu);
            goto  __RETRY;
        } else {
            s32FrameCount++;
            usleep(100000);
            if (!pstThreadInfo->stVencCtx.u32ReadPicNum ||
                (pstThreadInfo->stVencCtx.u32ReadPicNum &&
                 s32FrameCount < pstThreadInfo->stVencCtx.u32ReadPicNum)) {
                RK_MPI_MB_ReleaseMB(blk);
                blk = RK_NULL;
            } else {
                // not need read the input data for performance test.
                if (pstThreadInfo->stVencCtx.s32RecvPicNum > 0 &&
                    s32FrameCount > pstThreadInfo->stVencCtx.s32RecvPicNum) {
                    s32ReachEOS = 1;
                    break;
                }
                goto  __RETRY;
            }
        }
        if (s32ReachEOS) {
            RK_LOGI("chn %d reach EOS.", pstThreadInfo->stVencCtx.VencChn);
            break;
        }
    }

__FAILED:
    if (blk)
        RK_MPI_MB_ReleaseMB(blk);
    RK_LOGI("chn %d frame %d", 0, s32FrameCount);

    return RK_NULL;
}

// TODO(@team): now can't support output frame path
static RK_S32 TEST_VENC_StartSendFrame(COMMON_TEST_VENC_CTX_S *vencCtx) {
    RK_S32 s32Ret = 0;

    if (vencCtx->pSrcFramePath != RK_NULL) {
        gSFThread[vencCtx->VencChn].fp = fopen(vencCtx->pSrcFramePath, "r");
        if (gSFThread[vencCtx->VencChn].fp == RK_NULL) {
            RK_LOGE("can't open file %s!", vencCtx->pSrcFramePath);
            return RK_FAILURE;
        }
    }
    gSFThread[vencCtx->VencChn].bThreadStart = RK_TRUE;
    memcpy(&gSFThread[vencCtx->VencChn].stVencCtx, vencCtx, sizeof(COMMON_TEST_VENC_CTX_S));

    s32Ret = pthread_create(&(gSFThread[vencCtx->VencChn].VencPid), 0,
                            TEST_VENC_SendVencFrameProc,
                            (RK_VOID *)&(gSFThread[vencCtx->VencChn]));
    if (s32Ret != 0) {
        return s32Ret;
    }

    return s32Ret;
}

static RK_S32 TEST_VENC_StopSendFrame(VENC_CHN VencChn) {
    if (RK_TRUE == gSFThread[VencChn].bThreadStart) {
        gSFThread[VencChn].bThreadStart = RK_FALSE;
        pthread_join(gSFThread[VencChn].VencPid, 0);
        if (gSFThread[VencChn].fp != RK_NULL) {
            fclose(gSFThread[VencChn].fp);
            gSFThread[VencChn].fp = RK_NULL;
        }
    }

    return RK_SUCCESS;
}

RK_S32 TEST_VENC_SET_BitRate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32BitRate, RK_U32 u32BitRateMax, RK_U32 u32BitRateMin) {
    switch (pRcAttr->enRcMode) {
      case VENC_RC_MODE_MJPEGCBR:
        pRcAttr->stMjpegCbr.u32BitRate = u32BitRate;
        break;
      case VENC_RC_MODE_H264CBR:
        pRcAttr->stH264Cbr.u32BitRate = u32BitRate;
        break;
      case VENC_RC_MODE_H265CBR:
        pRcAttr->stH265Cbr.u32BitRate = u32BitRate;
        break;
      case VENC_RC_MODE_MJPEGVBR:
        pRcAttr->stMjpegVbr.u32BitRate = u32BitRate;
        pRcAttr->stMjpegVbr.u32MaxBitRate = u32BitRateMax;
        pRcAttr->stMjpegVbr.u32MinBitRate = u32BitRateMin;
        break;
      case VENC_RC_MODE_H264VBR:
        pRcAttr->stH264Vbr.u32BitRate = u32BitRate;
        pRcAttr->stH264Vbr.u32MaxBitRate = u32BitRateMax;
        pRcAttr->stH264Vbr.u32MinBitRate = u32BitRateMin;
        break;
      case VENC_RC_MODE_H265VBR:
        pRcAttr->stH265Vbr.u32BitRate = u32BitRate;
        pRcAttr->stH265Vbr.u32MaxBitRate = u32BitRateMax;
        pRcAttr->stH265Vbr.u32MinBitRate = u32BitRateMin;
        break;
      case VENC_RC_MODE_H264AVBR:
        pRcAttr->stH264Avbr.u32BitRate = u32BitRate;
        pRcAttr->stH264Avbr.u32MaxBitRate = u32BitRateMax;
        pRcAttr->stH264Avbr.u32MinBitRate = u32BitRateMin;
        break;
      case VENC_RC_MODE_H265AVBR:
        pRcAttr->stH265Avbr.u32BitRate = u32BitRate;
        pRcAttr->stH265Avbr.u32MaxBitRate = u32BitRateMax;
        pRcAttr->stH265Avbr.u32MinBitRate = u32BitRateMin;
        break;
      default:
        return RK_ERR_VENC_NOT_SUPPORT;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_VENC_SET_StatTime(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32StatTime) {
    switch (pRcAttr->enRcMode) {
      case VENC_RC_MODE_MJPEGCBR:
        pRcAttr->stMjpegCbr.u32StatTime = u32StatTime;
        break;
      case VENC_RC_MODE_H264CBR:
        pRcAttr->stH264Cbr.u32StatTime = u32StatTime;
        break;
      case VENC_RC_MODE_H265CBR:
        pRcAttr->stH265Cbr.u32StatTime = u32StatTime;
        break;
      case VENC_RC_MODE_MJPEGVBR:
        pRcAttr->stMjpegVbr.u32StatTime = u32StatTime;
        break;
      case VENC_RC_MODE_H264VBR:
        pRcAttr->stH264Vbr.u32StatTime = u32StatTime;
        break;
      case VENC_RC_MODE_H265VBR:
        pRcAttr->stH265Vbr.u32StatTime = u32StatTime;
        break;
      case VENC_RC_MODE_H264AVBR:
        pRcAttr->stH264Avbr.u32StatTime = u32StatTime;
        break;
      case VENC_RC_MODE_H265AVBR:
        pRcAttr->stH265Avbr.u32StatTime = u32StatTime;
        break;
      default:
        return RK_ERR_VENC_NOT_SUPPORT;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_VENC_SET_GopSize(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32GopSize) {
    switch (pRcAttr->enRcMode) {
      case VENC_RC_MODE_H264CBR:
        pRcAttr->stH264Cbr.u32Gop = u32GopSize;
        break;
      case VENC_RC_MODE_H265CBR:
        pRcAttr->stH265Cbr.u32Gop = u32GopSize;
        break;
      case VENC_RC_MODE_H264VBR:
        pRcAttr->stH264Vbr.u32Gop = u32GopSize;
        break;
      case VENC_RC_MODE_H265VBR:
        pRcAttr->stH265Vbr.u32Gop = u32GopSize;
        break;
      case VENC_RC_MODE_H264AVBR:
        pRcAttr->stH264Avbr.u32Gop = u32GopSize;
        break;
      case VENC_RC_MODE_H265AVBR:
        pRcAttr->stH265Avbr.u32Gop = u32GopSize;
        break;
      default:
        return RK_ERR_VENC_NOT_SUPPORT;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_VENC_SET_FrameRate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32Fps) {
    switch (pRcAttr->enRcMode) {
      case VENC_RC_MODE_MJPEGCBR:
        pRcAttr->stMjpegCbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stMjpegCbr.u32SrcFrameRateDen = 1;
        pRcAttr->stMjpegCbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stMjpegCbr.fr32DstFrameRateDen = 1;
        break;
      case VENC_RC_MODE_H264CBR:
        pRcAttr->stH264Cbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stH264Cbr.u32SrcFrameRateDen = 1;
        pRcAttr->stH264Cbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stH264Cbr.fr32DstFrameRateDen = 1;
        break;
      case VENC_RC_MODE_H265CBR:
        pRcAttr->stH265Cbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stH265Cbr.u32SrcFrameRateDen = 1;
        pRcAttr->stH265Cbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stH265Cbr.fr32DstFrameRateDen = 1;
        break;
      case VENC_RC_MODE_MJPEGVBR:
        pRcAttr->stMjpegVbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stMjpegVbr.u32SrcFrameRateDen = 1;
        pRcAttr->stMjpegVbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stMjpegVbr.fr32DstFrameRateDen = 1;
        break;
      case VENC_RC_MODE_H264VBR:
        pRcAttr->stH264Vbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stH264Vbr.u32SrcFrameRateDen = 1;
        pRcAttr->stH264Vbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stH264Vbr.fr32DstFrameRateDen = 1;
        break;
      case VENC_RC_MODE_H265VBR:
        pRcAttr->stH265Vbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stH265Vbr.u32SrcFrameRateDen = 1;
        pRcAttr->stH265Vbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stH265Vbr.fr32DstFrameRateDen = 1;
        break;
      case VENC_RC_MODE_H264AVBR:
        pRcAttr->stH264Avbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stH264Avbr.u32SrcFrameRateDen = 1;
        pRcAttr->stH264Avbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stH264Avbr.fr32DstFrameRateDen = 1;
        break;
      case VENC_RC_MODE_H265AVBR:
        pRcAttr->stH265Avbr.u32SrcFrameRateNum = u32Fps;
        pRcAttr->stH265Avbr.u32SrcFrameRateDen = 1;
        pRcAttr->stH265Avbr.fr32DstFrameRateNum = u32Fps;
        pRcAttr->stH265Avbr.fr32DstFrameRateDen = 1;
        break;
      default:
        return RK_ERR_VENC_NOT_SUPPORT;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_VENC_SET_FixQp(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32FixIQp,
                                   RK_U32 u32FixPQp, RK_U32 u32FixBQp) {
    switch (pRcAttr->enRcMode) {
      case VENC_RC_MODE_MJPEGFIXQP:
        pRcAttr->stMjpegFixQp.u32Qfactor = u32FixIQp;
        break;
      case VENC_RC_MODE_H264FIXQP:
        pRcAttr->stH264FixQp.u32IQp = u32FixIQp;
        pRcAttr->stH264FixQp.u32PQp = u32FixPQp;
        pRcAttr->stH264FixQp.u32BQp = u32FixBQp;
        break;
      case VENC_RC_MODE_H265FIXQP:
        pRcAttr->stH265FixQp.u32IQp = u32FixIQp;
        pRcAttr->stH265FixQp.u32PQp = u32FixPQp;
        pRcAttr->stH265FixQp.u32BQp = u32FixBQp;
        break;
      default:
        return RK_ERR_VENC_NOT_SUPPORT;
    }

    return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
