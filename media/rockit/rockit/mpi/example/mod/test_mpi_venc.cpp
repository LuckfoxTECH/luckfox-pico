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
    const char     *srcFileUri;
    const char     *dstFilePath;
    RK_U32          u32SrcWidth;
    RK_U32          u32SrcHeight;
    RK_U32          u32srcVirWidth;
    RK_U32          u32srcVirHeight;
    RK_S32          s32LoopCount;
    RK_U32          u32ChnIndex;
    RK_U32          u32ChnStart;
    RK_U32          u32ChNum;
    RK_U32          u32SrcPixFormat;
    RK_U32          u32DstCodec;
    RK_U32          u32BufferSize;
    RK_U32          u32StreamSize;
    RK_U32          u32StreamBufCnt;
    RK_U32          u32BitRateKb;
    RK_U32          u32BitRateKbMax;
    RK_U32          u32BitRateKbMin;
    RK_U32          u32GopSize;
    RK_U32          u32RoiTestCount;
    RK_U32          u32FixQp;
    RK_U32          u32DeBreath;
    RK_U32          u32StatTime;
    RK_S32          s32SnapPicCount;
    RK_BOOL         threadExit;
    RK_BOOL         bFrameRate;
    RK_BOOL         bInsertUserData;
    COMPRESS_MODE_E enCompressMode;
    VENC_RC_MODE_E  enRcMode;
    VENC_GOP_MODE_E enGopMode;
    VENC_CROP_TYPE_E enCropType;
    ROTATION_E enRotation;
    MIRROR_E   enMirror;
    RK_BOOL    bSuperFrm;
    RK_BOOL    bFrmLost;
    RK_BOOL    bIntraRefresh;
    RK_BOOL    bHierarchicalQp;
    RK_BOOL    bMjpegParam;
    RK_BOOL    bMjpegRcParam;
    RK_BOOL    bForceIdr;
    RK_BOOL    bFullRange;
    RK_BOOL    bSendFrameEx;
    RK_BOOL    bQpmap;
    RK_BOOL    bMotionDeblur;
    RK_BOOL    bMotionStaticSwitch;
    MB_POOL    vencPoolInput;
    MB_POOL    vencPoolOutput[VENC_MAX_CHN_NUM];
    RK_BOOL    bAttachPool;
    RK_BOOL    bRefBufShare;
    FILE      *fp_ivs_result;
    RK_BOOL    bIvs;
    RK_BOOL    bSvc;
    RK_BOOL    bSao;
    RK_BOOL    bGetStream;
    RK_U32     u32SceneMode;
} TEST_VENC_CTX_S;

static RK_S32 create_ivs(TEST_VENC_CTX_S *ctx) {
    IVS_CHN_ATTR_S attr;
    memset(&attr, 0, sizeof(attr));
    attr.enMode = IVS_MODE_MD_OD;
    attr.u32PicWidth = ctx->u32SrcWidth;
    attr.u32PicHeight = ctx->u32SrcHeight;
    attr.enPixelFormat = (PIXEL_FORMAT_E)ctx->u32SrcPixFormat;
    attr.s32Gop = 30;
    attr.bSmearEnable = RK_FALSE;
    attr.bWeightpEnable = RK_FALSE;
    attr.bMDEnable = RK_TRUE;
    attr.s32MDInterval = 1;
    attr.bMDNightMode = RK_FALSE;
    attr.u32MDSensibility = 1;
    attr.bODEnable = RK_TRUE;
    attr.s32ODInterval = 1;
    attr.s32ODPercent = 7;
    return RK_MPI_IVS_CreateChn(0, &attr);
}

static RK_S32 destroy_ivs(RK_VOID) {
    RK_MPI_IVS_DestroyChn(0);
    return RK_SUCCESS;
}

static RK_S32 read_with_pixel_width(RK_U8 *pBuf, RK_U32 u32Width, RK_U32 u32VirHeight,
                                     RK_U32 u32VirWidth, RK_U32 u32PixWidth, FILE *fp) {
    RK_U32 u32Row;
    RK_S32 s32ReadSize;

    for (u32Row = 0; u32Row < u32VirHeight; u32Row++) {
        s32ReadSize = fread(pBuf + u32Row * u32VirWidth * u32PixWidth, 1, u32Width * u32PixWidth, fp);
        if (s32ReadSize != u32Width * u32PixWidth) {
            RK_LOGE("read file failed expect %d vs %d\n",
                      u32Width * u32PixWidth, s32ReadSize);
            return RK_FAILURE;
        }
    }

    return RK_SUCCESS;
}

static RK_S32 read_image(RK_U8 *pVirAddr, RK_U32 u32Width, RK_U32 u32Height,
                                  RK_U32 u32VirWidth, RK_U32 u32VirHeight, RK_U32 u32PixFormat, FILE *fp) {
    RK_U32 u32Row = 0;
    RK_U32 u32ReadSize = 0;
    RK_S32 s32Ret = RK_SUCCESS;

    RK_U8 *pBufy = pVirAddr;
    RK_U8 *pBufu = pBufy + u32VirWidth * u32VirHeight;
    RK_U8 *pBufv = pBufu + u32VirWidth * u32VirHeight / 4;

    switch (u32PixFormat) {
        case RK_FMT_YUV420P:
        case RK_FMT_YUV420SP: {
            for (u32Row = 0; u32Row < u32VirHeight; u32Row++) {
                u32ReadSize = fread(pBufy + u32Row * u32VirWidth, 1, u32Width, fp);
                if (u32ReadSize != u32Width) {
                     return RK_FAILURE;
                }
            }

            for (u32Row = 0; u32Row < u32VirHeight / 2; u32Row++) {
                u32ReadSize = fread(pBufu + u32Row * u32VirWidth, 1, u32Width, fp);
                if (u32ReadSize != u32Width) {
                    return RK_FAILURE;
                }
            }
        } break;
        case RK_FMT_RGB888:
        case RK_FMT_BGR888: {
            s32Ret = read_with_pixel_width(pBufy, u32Width, u32VirHeight, u32VirWidth, 3, fp);
        } break;
        default : {
            RK_LOGE("read image do not support fmt %d\n", u32PixFormat);
            return RK_FAILURE;
        } break;
    }

    return s32Ret;
}

static RK_S32 check_options(const TEST_VENC_CTX_S *ctx) {
    if (ctx->u32SrcPixFormat == RK_FMT_BUTT ||
        ctx->u32DstCodec <= RK_VIDEO_ID_Unused ||
        ctx->u32SrcWidth <= 0 ||
        ctx->u32SrcHeight <= 0) {
        goto __FAILED;
    }

    return RK_SUCCESS;

__FAILED:
    return RK_ERR_VENC_ILLEGAL_PARAM;
}

void* venc_force_idr(void *pArgs) {
    TEST_VENC_CTX_S *pstCtx     = reinterpret_cast<TEST_VENC_CTX_S *>(pArgs);
    RK_U32           u32Ch      = pstCtx->u32ChnIndex;

    while (!pstCtx->threadExit) {
        RK_MPI_VENC_RequestIDR(u32Ch, RK_FALSE);
        usleep(30000);
    }

    return RK_NULL;
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

void h265_set_qpmap_64x64(RK_S16 *dst, RK_S16 *src, RK_S32 w, RK_S32 h) {
    RK_S32 mb_w = RK_ALIGN(w, 64) / 64;
    RK_S32 mb_h = RK_ALIGN(h, 64) / 64;
    RK_S32 ctu_line = mb_w;
    RK_S32 i, j, cu16cnt;

    for (j = 0; j < mb_h; j++) {
        for (i = 0; i < mb_w; i++) {
            RK_S32 ctu_addr = j * ctu_line + i;
            RK_S32 cu16_num_line = ctu_line * 4;
            for (cu16cnt = 0; cu16cnt < 16; cu16cnt++) {
                RK_S32 cu16_x;
                RK_S32 cu16_y;
                RK_S32 cu16_addr_in_frame;
                cu16_x = cu16cnt & 3;
                cu16_y = cu16cnt / 4;
                cu16_x += i * 4;
                cu16_y += j * 4;
                cu16_addr_in_frame = cu16_x + cu16_y * cu16_num_line;
                dst[ctu_addr * 16 + cu16cnt] = src[cu16_addr_in_frame];
            }
        }
    }
}

void* venc_send_frame(void *pArgs) {
    TEST_VENC_CTX_S     *pstCtx        = reinterpret_cast<TEST_VENC_CTX_S *>(pArgs);
    RK_S32               s32Ret         = RK_SUCCESS;
    RK_U8               *pVirAddr       = RK_NULL;
    FILE                *fp             = RK_NULL;
    MB_BLK               blk            = RK_NULL;
    RK_S32               s32LoopCount   = pstCtx->s32LoopCount;
    MB_POOL              pool           = pstCtx->vencPoolInput;
    RK_U32               u32Ch          = pstCtx->u32ChnIndex;
    RK_S32               s32FrameCount  = 0;
    RK_S32               s32ReachEOS    = 0;
    VIDEO_FRAME_INFO_S   stFrame;
    MB_BLK               qpmapBlk = RK_NULL;
    RK_S16              *qpmapData = RK_NULL;
    RK_S16              *qpmapTmp = RK_NULL;

    if (pstCtx->bSendFrameEx || pstCtx->bQpmap) {
        if (pstCtx->u32DstCodec == RK_VIDEO_ID_AVC || pstCtx->u32DstCodec == RK_VIDEO_ID_HEVC) {
            RK_U8 align = (pstCtx->u32DstCodec == RK_VIDEO_ID_AVC ? 16 : 64);
            RK_U32 qpmapWidth = RK_ALIGN(pstCtx->u32SrcWidth, align) / 16;
            RK_U32 qpmapHeight = RK_ALIGN(pstCtx->u32SrcHeight, align) / 16;
            RK_U32 qpmapSize = qpmapWidth * qpmapHeight * 2;
            RK_S16 *qpmap = RK_NULL;
            if (!qpmapBlk) {
                s32Ret = RK_MPI_SYS_MmzAlloc(&qpmapBlk, RK_NULL, RK_NULL, qpmapSize);
                if (s32Ret != RK_SUCCESS)
                    goto __EXIT;
                qpmapData = (RK_S16 *)RK_MPI_MB_Handle2VirAddr(qpmapBlk);
            }
            if (pstCtx->u32DstCodec == RK_VIDEO_ID_HEVC && !qpmapTmp) {
                qpmapTmp = (RK_S16 *)malloc(qpmapSize);
                if (!qpmapTmp)
                    goto __EXIT;
            }
            qpmap = (pstCtx->u32DstCodec == RK_VIDEO_ID_AVC ? qpmapData : qpmapTmp);
            for (RK_U32 j = 0; j < qpmapHeight; j++) {
                for (RK_U32 i = 0; i < qpmapWidth; i++) {
                    qpmap[j * qpmapHeight + i] = 0x8F80;  // 0x8F: abs qp, qp 15, 0x80: must
                }
            }
            if (pstCtx->u32DstCodec == RK_VIDEO_ID_HEVC) {
                h265_set_qpmap_64x64(qpmapData, qpmapTmp, pstCtx->u32SrcWidth, pstCtx->u32SrcHeight);
            }
        }
    }

    if (pstCtx->bQpmap) {
        RK_MPI_VENC_SetQpmap(pstCtx->u32ChnIndex, qpmapBlk);
    }

    memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));



    while (!pstCtx->threadExit) {
        blk = RK_MPI_MB_GetMB(pool, pstCtx->u32BufferSize, RK_TRUE);

        if (RK_NULL == blk) {
            RK_LOGE("RK_MPI_MB_GetMB fail:%d", u32Ch);
            usleep(2000llu);
            continue;
        }
        pVirAddr = reinterpret_cast<RK_U8 *>(RK_MPI_MB_Handle2VirAddr(blk));

        if (pstCtx->srcFileUri) {
            if (fp == RK_NULL) {
                fp = fopen(pstCtx->srcFileUri, "r");
                if (fp == RK_NULL) {
                    RK_LOGE("chn %d can't open file %s!\n", u32Ch, pstCtx->srcFileUri);
                    return RK_NULL;
                }
            }
            if (pstCtx->enCompressMode == COMPRESS_AFBC_16x16) {
                if (fread(pVirAddr, 1, pstCtx->u32BufferSize, fp) != pstCtx->u32BufferSize)
                    s32Ret = RK_FAILURE;
                else
                    s32Ret = RK_SUCCESS;
            } else {
                s32Ret = read_image(pVirAddr, pstCtx->u32SrcWidth, pstCtx->u32SrcHeight,
                         pstCtx->u32srcVirWidth, pstCtx->u32srcVirHeight, pstCtx->u32SrcPixFormat, fp);
            }
        } else {
            s32Ret = TEST_COMM_FillImage(pVirAddr, pstCtx->u32SrcWidth,
                            pstCtx->u32SrcHeight,
                            pstCtx->u32srcVirWidth,
                            pstCtx->u32srcVirHeight,
                            (PIXEL_FORMAT_E)pstCtx->u32SrcPixFormat, s32FrameCount);
            if (s32Ret != RK_SUCCESS) {
                RK_MPI_MB_ReleaseMB(blk);
                RK_LOGE("TEST_COMM_FillImage fail ch %d", u32Ch);
                return RK_NULL;
            }
        }

        if (s32Ret != RK_SUCCESS) {
             s32ReachEOS = 1;
             if (s32LoopCount > 0) {
                s32LoopCount--;
                RK_LOGI("finish venc count %d\n", pstCtx->s32LoopCount - s32LoopCount);
                if (s32LoopCount > 0) {
                    s32ReachEOS = 0;
                    RK_MPI_MB_ReleaseMB(blk);

                    fseek(fp, 0L, SEEK_SET);
                    RK_LOGI("seek finish ch %d", u32Ch);
                    continue;
                }
             }
        }

#if TEST_CROP_PARAM_CHANGE
        if (s32FrameCount == 50) {
            VENC_CHN_PARAM_S stParam;
            RK_MPI_VENC_GetChnParam(u32Ch, &stParam);
            // for crop test
            if (stParam.stCropCfg.enCropType == VENC_CROP_ONLY) {
                stParam.stCropCfg.stCropRect.s32X = 100;
                stParam.stCropCfg.stCropRect.s32Y = 100;
                stParam.stCropCfg.stCropRect.u32Height = 400;
                stParam.stCropCfg.stCropRect.u32Width = 300;
            } else if (stParam.stCropCfg.enCropType == VENC_CROP_SCALE) {
            // for crop/scale test
                stParam.stCropCfg.stScaleRect.stSrc.s32X = 300;
                stParam.stCropCfg.stScaleRect.stSrc.s32Y = 500;
                stParam.stCropCfg.stScaleRect.stSrc.u32Width = stParam.stCropCfg.stScaleRect.stSrc.u32Width / 4;
                stParam.stCropCfg.stScaleRect.stSrc.u32Height = stParam.stCropCfg.stScaleRect.stSrc.u32Height;
                stParam.stCropCfg.stScaleRect.stDst.s32X = 0;
                stParam.stCropCfg.stScaleRect.stDst.s32Y = 0;
                stParam.stCropCfg.stScaleRect.stDst.u32Width = 400;
                stParam.stCropCfg.stScaleRect.stDst.u32Height = 400;
            }
            RK_MPI_VENC_SetChnParam(u32Ch, &stParam);
        }
#endif

        RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);

        stFrame.stVFrame.pMbBlk = blk;
        stFrame.stVFrame.u32Width = pstCtx->u32SrcWidth;
        stFrame.stVFrame.u32Height = pstCtx->u32SrcHeight;
        stFrame.stVFrame.u32VirWidth = pstCtx->u32srcVirWidth;
        stFrame.stVFrame.u32VirHeight = pstCtx->u32srcVirHeight;
        stFrame.stVFrame.enPixelFormat = (PIXEL_FORMAT_E)pstCtx->u32SrcPixFormat;
        stFrame.stVFrame.u32FrameFlag |= s32ReachEOS ? FRAME_FLAG_SNAP_END : 0;
        stFrame.stVFrame.enCompressMode = pstCtx->enCompressMode;

        if (pstCtx->bInsertUserData) {
            RK_U32 user_len;
            RK_U8 user_data[] = "this is rockchip user data";
            user_len = sizeof(user_data);
            RK_MPI_VENC_InsertUserData(u32Ch, user_data, user_len);
            RK_CHAR user_data2[1024];
            snprintf(user_data2, sizeof(user_data2), "rockchip continuous user data:%d", s32FrameCount);
            user_len = strlen(user_data2);
            RK_MPI_VENC_InsertUserData(u32Ch, reinterpret_cast<RK_U8 *>(user_data2), user_len);
        }
__RETRY:
        if (pstCtx->bSendFrameEx) {
            if (pstCtx->u32DstCodec == RK_VIDEO_ID_AVC || pstCtx->u32DstCodec == RK_VIDEO_ID_HEVC) {
                USER_FRAME_INFO_S stUser;
                memset(&stUser, 0, sizeof(stUser));
                memcpy(&stUser.stUserFrame, &stFrame, sizeof(stFrame));
                stUser.stUserRcInfo.bQpMapValid = RK_TRUE;
                stUser.stUserRcInfo.pMbBlkQpMap = qpmapBlk;
                s32Ret = RK_MPI_VENC_SendFrameEx(u32Ch, &stUser, -1);
            } else {
                s32Ret = RK_MPI_VENC_SendFrame(u32Ch, &stFrame, -1);
            }
        } else {
            s32Ret = RK_MPI_VENC_SendFrame(u32Ch, &stFrame, -1);
            if (pstCtx->bIvs) {
                s32Ret = RK_MPI_IVS_SendFrame(0, &stFrame, -1);
                IVS_RESULT_INFO_S stResults;
                memset(&stResults, 0, sizeof(IVS_RESULT_INFO_S));
                s32Ret = RK_MPI_IVS_GetResults(0, &stResults, -1);
                if (s32Ret == RK_SUCCESS) {
                    if (stResults.s32ResultNum == 1) {
                        int x = pstCtx->u32SrcWidth / 8 / 8;
                        int y = pstCtx->u32SrcHeight / 8;
                        if (stResults.pstResults->stMdInfo.pData) {
                            for (int n = 0; n < x * 8; n++)
                                fprintf(pstCtx->fp_ivs_result, "-");
                            fprintf(pstCtx->fp_ivs_result, "\n");
                            for (int j = 0; j < y; j++) {
                                for (int i = 0; i < x; i++) {
                                    for (int k = 0; k < 8; k++) {
                                        if (stResults.pstResults->stMdInfo.pData[j * 64 + i] & (1 << k))
                                            fprintf(pstCtx->fp_ivs_result, "1");
                                        else
                                            fprintf(pstCtx->fp_ivs_result, "0");
                                    }
                                }
                                fprintf(pstCtx->fp_ivs_result, "\n");
                            }
                            for (int n = 0; n < x * 8; n++)
                                fprintf(pstCtx->fp_ivs_result, "-");
                            fprintf(pstCtx->fp_ivs_result, "\n");
                            printf("MD u32RectNum: %u\n", stResults.pstResults->stMdInfo.u32RectNum);
                            for (int i = 0; i < stResults.pstResults->stMdInfo.u32RectNum; i++) {
                                printf("%d: [%d, %d, %d, %d]\n", i,
                                       stResults.pstResults->stMdInfo.stRect[i].s32X,
                                       stResults.pstResults->stMdInfo.stRect[i].s32Y,
                                       stResults.pstResults->stMdInfo.stRect[i].u32Width,
                                       stResults.pstResults->stMdInfo.stRect[i].u32Height);
                            }
                        }
                        fprintf(pstCtx->fp_ivs_result, "od flag: %d\n", stResults.pstResults->stOdInfo.u32Flag);
                        fsync(fileno(pstCtx->fp_ivs_result));
                    }
                    RK_MPI_IVS_ReleaseResults(0, &stResults);
                } else {
                    RK_LOGE("RK_MPI_IVS_GetResults fail %x", s32Ret);
                }
            }
        }
        if (s32Ret < 0) {
            if (pstCtx->threadExit) {
                RK_MPI_MB_ReleaseMB(blk);
                break;
            }

            usleep(10000llu);
            goto  __RETRY;
        } else {
            RK_MPI_MB_ReleaseMB(blk);
            s32FrameCount++;
            RK_LOGI("chn %d frame %d", u32Ch, s32FrameCount);
            usleep(33000);
        }
        if (s32ReachEOS ||
           (pstCtx->s32SnapPicCount != -1 && s32FrameCount >= pstCtx->s32SnapPicCount)) {
            RK_LOGI("chn %d reach EOS.", u32Ch);
            break;
        }
    }

__EXIT:
    if (fp)
        fclose(fp);

    if (pstCtx->bSendFrameEx || pstCtx->bQpmap) {
        if (qpmapBlk)
            RK_MPI_SYS_MmzFree(qpmapBlk);
        if (qpmapTmp)
            free(qpmapTmp);
    }

    return RK_NULL;
}

RK_S32 unit_test_mpi_venc(TEST_VENC_CTX_S *ctx) {
    RK_S32                  s32Ret = RK_SUCCESS;
    RK_U32                  u32Ch = 0;
    VENC_CHN_ATTR_S         stAttr;
    VENC_CHN_PARAM_S        stParam;
    VENC_RECV_PIC_PARAM_S   stRecvParam;
    VENC_RC_PARAM_S         stRcParam;
    MB_POOL_CONFIG_S        stMbPoolCfg;
    TEST_VENC_CTX_S         stVencCtx[VENC_MAX_CHN_NUM];
    pthread_t               vencThread[VENC_MAX_CHN_NUM] = {0};
    pthread_t               getStreamThread[VENC_MAX_CHN_NUM] = {0};
    pthread_t               forceIdrThread[VENC_MAX_CHN_NUM];

    memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    memset(&stRcParam, 0, sizeof(VENC_RC_PARAM_S));
    memset(&stParam, 0, sizeof(VENC_CHN_PARAM_S));

    if (ctx->bIvs) {
        s32Ret = create_ivs(ctx);
        if (s32Ret) {
            RK_LOGE("ivs init failed:%x", s32Ret);
            return RK_FAILURE;
        }
        ctx->fp_ivs_result = fopen("/data/ivs_result.txt", "wb");
        if (!ctx->fp_ivs_result) {
            RK_LOGE("open /data/ivs_result.txt failed");
            return RK_FAILURE;
        }
    }

    if (ctx->u32BufferSize <= 0) {
        PIC_BUF_ATTR_S stPicBufAttr;
        MB_PIC_CAL_S stMbPicCalResult;

        stPicBufAttr.u32Width = ctx->u32SrcWidth;
        stPicBufAttr.u32Height = ctx->u32SrcHeight;
        stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)ctx->u32SrcPixFormat;
        stPicBufAttr.enCompMode = ctx->enCompressMode;
        s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
            return s32Ret;
        }
        ctx->u32BufferSize = stMbPicCalResult.u32MBSize;
        RK_LOGD("calc picture size: %d", ctx->u32BufferSize);
    }
    if (ctx->u32BufferSize > 8192 * 8192 * 4) {
        RK_LOGE("too large picture size: %d", ctx->u32BufferSize);
        return RK_FAILURE;
    }
    if (ctx->u32StreamSize == 0) {
        ctx->u32StreamSize = ctx->u32BufferSize;
    }

    // for input image pool
    memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
    stMbPoolCfg.u64MBSize = ctx->u32BufferSize;
    stMbPoolCfg.u32MBCnt  = 3;
    stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
    stMbPoolCfg.bPreAlloc = RK_TRUE;
    ctx->vencPoolInput = RK_MPI_MB_CreatePool(&stMbPoolCfg);
    if (ctx->vencPoolInput == MB_INVALID_POOLID) {
        RK_LOGE("create vencPoolInput failed!");
        return RK_FAILURE;
    }

    for (u32Ch = ctx->u32ChnStart; u32Ch < ctx->u32ChnStart + ctx->u32ChNum; u32Ch++) {
        memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
        memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
        memset(&stRcParam, 0, sizeof(VENC_RC_PARAM_S));
        memset(&stParam, 0, sizeof(VENC_CHN_PARAM_S));
        if (ctx->u32ChNum >= 1) {
            ctx->u32ChnIndex = u32Ch;
        }

        stAttr.stRcAttr.enRcMode = ctx->enRcMode;
        stAttr.stRcAttr.stH264Cbr.u32Gop = ctx->u32GopSize;
        stAttr.stRcAttr.stH264Cbr.u32BitRate = ctx->u32BitRateKb;
        TEST_VENC_SET_BitRate(&stAttr.stRcAttr, ctx->u32BitRateKb, ctx->u32BitRateKbMax, ctx->u32BitRateKbMin);
        TEST_VENC_SET_FixQp(&stAttr.stRcAttr, ctx->u32FixQp, ctx->u32FixQp + 2, ctx->u32FixQp);
        TEST_VENC_SET_StatTime(&stAttr.stRcAttr, ctx->u32StatTime);

        stAttr.stVencAttr.enType = (RK_CODEC_ID_E)ctx->u32DstCodec;
        stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
        stAttr.stVencAttr.enPixelFormat = (PIXEL_FORMAT_E)ctx->u32SrcPixFormat;
        stAttr.stVencAttr.u32PicWidth = ctx->u32SrcWidth;
        stAttr.stVencAttr.u32PicHeight = ctx->u32SrcHeight;
        stAttr.stVencAttr.enMirror = ctx->enMirror;

        if (ctx->u32srcVirWidth <= 0) {
            ctx->u32srcVirWidth = ctx->u32SrcWidth;
        }
        stAttr.stVencAttr.u32VirWidth = ctx->u32srcVirWidth;

        if (ctx->u32srcVirHeight <= 0) {
            ctx->u32srcVirHeight = ctx->u32SrcHeight;
        }
        stAttr.stVencAttr.u32VirHeight = ctx->u32srcVirHeight;
        stAttr.stVencAttr.u32StreamBufCnt = ctx->u32StreamBufCnt;
        stAttr.stVencAttr.u32BufSize = ctx->u32StreamSize;

        if (stAttr.stVencAttr.enType == RK_VIDEO_ID_JPEG) {
            stAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
            stAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
            stAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
        }

        RK_MPI_VENC_CreateChn(u32Ch, &stAttr);

        RK_MPI_VENC_EnableMotionDeblur(u32Ch, ctx->bMotionDeblur);

        RK_MPI_VENC_EnableMotionStaticSwitch(u32Ch, ctx->bMotionStaticSwitch);

        stParam.stCropCfg.enCropType = ctx->enCropType;
        // for crop test
        if (stParam.stCropCfg.enCropType == VENC_CROP_ONLY) {
            stParam.stCropCfg.stCropRect.s32X = 10;
            stParam.stCropCfg.stCropRect.s32Y = 10;
            stParam.stCropCfg.stCropRect.u32Height = 100;
            stParam.stCropCfg.stCropRect.u32Width = 100;
        } else if (stParam.stCropCfg.enCropType == VENC_CROP_SCALE) {
        // for crop/scale test
            stParam.stCropCfg.stScaleRect.stSrc.s32X = 100;
            stParam.stCropCfg.stScaleRect.stSrc.s32Y = 200;
            stParam.stCropCfg.stScaleRect.stSrc.u32Width = ctx->u32SrcWidth / 2;
            stParam.stCropCfg.stScaleRect.stSrc.u32Height = ctx->u32SrcHeight / 2;
            stParam.stCropCfg.stScaleRect.stDst.s32X = 0;
            stParam.stCropCfg.stScaleRect.stDst.s32Y = 0;
            stParam.stCropCfg.stScaleRect.stDst.u32Width = 200;
            stParam.stCropCfg.stScaleRect.stDst.u32Height = 200;
        }
        // for framerate test
        {
            stParam.stFrameRate.bEnable = ctx->bFrameRate;
            stParam.stFrameRate.s32SrcFrmRateNum = 25;
            stParam.stFrameRate.s32SrcFrmRateDen = 1;
            stParam.stFrameRate.s32DstFrmRateNum = 10;
            stParam.stFrameRate.s32DstFrmRateDen = 1;
        }
        RK_MPI_VENC_SetChnParam(u32Ch, &stParam);

        VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
        memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
        stVencChnRefBufShare.bEnable = ctx->bRefBufShare;
        RK_MPI_VENC_SetChnRefBufShareAttr(u32Ch, &stVencChnRefBufShare);

#if TEST_RC_MODE
        stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264AVBR;
        stAttr.stRcAttr.stH264Avbr.u32Gop = 99;
        stAttr.stRcAttr.stH264Avbr.u32SrcFrameRateNum = 25;
        stAttr.stRcAttr.stH264Avbr.u32SrcFrameRateDen = 1;
        stAttr.stRcAttr.stH264Avbr.fr32DstFrameRateNum = 25;
        stAttr.stRcAttr.stH264Avbr.fr32DstFrameRateDen = 1;
        stAttr.stRcAttr.stH264Avbr.u32BitRate = 614400;
        RK_MPI_VENC_SetChnAttr(u32Ch, &stAttr);
#endif
        if (ctx->bAttachPool) {
            // for output stream pool
            memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
            stMbPoolCfg.u64MBSize = ctx->u32StreamSize;
            stMbPoolCfg.u32MBCnt  = 3;
            stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
            stMbPoolCfg.bPreAlloc = RK_TRUE;  //  this must prealloc if attach enc output
            ctx->vencPoolOutput[u32Ch] = RK_MPI_MB_CreatePool(&stMbPoolCfg);
            if (ctx->vencPoolOutput[u32Ch] == MB_INVALID_POOLID) {
                RK_LOGE("create vencPoolOutput[%d] failed!", u32Ch);
                goto __FAILED;
            }

            s32Ret = RK_MPI_VENC_AttachMbPool(u32Ch, ctx->vencPoolOutput[u32Ch]);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VENC_AttachMbPool fail:0x%x!", s32Ret);
                goto __FAILED;
            }
        }

        if (ctx->enGopMode) {
            stAttr.stGopAttr.enGopMode = ctx->enGopMode;
            if (!stAttr.stRcAttr.stH264Cbr.u32Gop)
                stAttr.stRcAttr.stH264Cbr.u32Gop = 60;
            stAttr.stGopAttr.s32VirIdrLen = stAttr.stRcAttr.stH264Cbr.u32Gop / 2;
            if (stAttr.stGopAttr.enGopMode > VENC_GOPMODE_NORMALP)
                stAttr.stGopAttr.u32MaxLtrCount = 1;
            RK_MPI_VENC_SetChnAttr(u32Ch, &stAttr);
        }

        if (ctx->enRotation) {
            RK_MPI_VENC_SetChnRotation(u32Ch, ctx->enRotation);
        }

        if (ctx->bSvc)
            RK_MPI_VENC_EnableSvc(u32Ch, ctx->bSvc);

        if (stAttr.stVencAttr.enType == RK_VIDEO_ID_JPEG) {
            VENC_JPEG_PARAM_S stJpegParam;
            memset(&stJpegParam, 0, sizeof(stJpegParam));
            stJpegParam.u32Qfactor = 99;
            RK_MPI_VENC_SetJpegParam(u32Ch, &stJpegParam);
        }

        if (ctx->u32SceneMode) {
            RK_MPI_VENC_SetSceneMode(u32Ch, (VENC_SCENE_MODE_E)ctx->u32SceneMode);
        }

        stRecvParam.s32RecvPicNum = ctx->s32SnapPicCount;
        RK_MPI_VENC_StartRecvFrame(u32Ch, &stRecvParam);

        if (ctx->bSao && stAttr.stVencAttr.enType == RK_VIDEO_ID_HEVC) {
            VENC_H265_SAO_S stH265Sao;
            memset(&stH265Sao, 0, sizeof(stH265Sao));
            stH265Sao.slice_sao_luma_flag = 1;
            stH265Sao.slice_sao_chroma_flag = 1;
            stH265Sao.slice_sao_bit_ratio = 3;
            RK_MPI_VENC_SetH265Sao(u32Ch, &stH265Sao);
        }

        for (RK_U32 i = 0; i < ctx->u32RoiTestCount; i++) {
            VENC_ROI_ATTR_S stRoiAttr;
            stRoiAttr.u32Index = i;
            stRoiAttr.bEnable = RK_TRUE;
            stRoiAttr.bAbsQp = i % 2 ? RK_FALSE : RK_TRUE;
            stRoiAttr.s32Qp = i + 42;
            stRoiAttr.bIntra = i % 2 ? RK_TRUE : RK_FALSE;
            stRoiAttr.stRect.s32X = RK_ALIGN_16((i + 1) * 16);
            stRoiAttr.stRect.s32Y = RK_ALIGN_16((i + 1) * 16);
            stRoiAttr.stRect.u32Width = RK_ALIGN_16(ctx->u32SrcWidth / (i + 2));
            stRoiAttr.stRect.u32Height = RK_ALIGN_16(ctx->u32SrcHeight / (i + 2));
            RK_MPI_VENC_SetRoiAttr(u32Ch, &stRoiAttr);
        }

#if TEST_RC_MODE
        stRcParam.s32FirstFrameStartQp = 25;
        stRcParam.stParamH264.u32StepQp = 4;
        stRcParam.stParamH264.u32MinQp = 10;
        stRcParam.stParamH264.u32MaxQp = 40;
        stRcParam.stParamH264.u32MinIQp = 20;
        stRcParam.stParamH264.u32MaxIQp = 30;
        stRcParam.stParamH264.s32DeltIpQp = -2;
        stRcParam.stParamH264.s32MaxReEncodeTimes = 2;
        stRcParam.stParamH264.u32FrmMinQp = 15;
        stRcParam.stParamH264.u32FrmMinIQp = 25;
        RK_MPI_VENC_SetRcParam(u32Ch, &stRcParam);
#endif

        if (!ctx->bFullRange) {
            if (stAttr.stVencAttr.enType == RK_VIDEO_ID_AVC) {
                VENC_H264_VUI_S stH264Vui;
                RK_MPI_VENC_GetH264Vui(u32Ch, &stH264Vui);
                stH264Vui.stVuiVideoSignal.video_full_range_flag = 0;
                RK_MPI_VENC_SetH264Vui(u32Ch, &stH264Vui);
            } else if (stAttr.stVencAttr.enType == RK_VIDEO_ID_HEVC) {
                VENC_H265_VUI_S stH265Vui;
                RK_MPI_VENC_GetH265Vui(u32Ch, &stH265Vui);
                stH265Vui.stVuiVideoSignal.video_full_range_flag = 0;
                RK_MPI_VENC_SetH265Vui(u32Ch, &stH265Vui);
            }
        }

        if (ctx->bSuperFrm) {
            VENC_SUPERFRAME_CFG_S stSuperFrameCfg;
            memset(&stSuperFrameCfg, 0, sizeof(stSuperFrameCfg));
            stSuperFrameCfg.enSuperFrmMode = SUPERFRM_DISCARD;
            stSuperFrameCfg.u32SuperIFrmBitsThr = 100 * 1024 * 8;  // 100KByte
            stSuperFrameCfg.u32SuperPFrmBitsThr = 20 * 1024 * 8;  // 20KByte
            stSuperFrameCfg.enRcPriority = VENC_RC_PRIORITY_FRAMEBITS_FIRST;
            RK_MPI_VENC_SetSuperFrameStrategy(u32Ch, &stSuperFrameCfg);
        }

        if (ctx->bIntraRefresh) {
            VENC_INTRA_REFRESH_S stIntraRefresh;
            memset(&stIntraRefresh, 0, sizeof(stIntraRefresh));
            stIntraRefresh.bRefreshEnable = RK_TRUE;
            stIntraRefresh.enIntraRefreshMode = INTRA_REFRESH_ROW;
            stIntraRefresh.u32RefreshNum = 5;
            RK_MPI_VENC_SetIntraRefresh(u32Ch, &stIntraRefresh);
        }

        if (ctx->bHierarchicalQp) {
            VENC_HIERARCHICAL_QP_S stHierarchicalQp;
            memset(&stHierarchicalQp, 0, sizeof(stHierarchicalQp));
            stHierarchicalQp.bHierarchicalQpEn = RK_TRUE;
            stHierarchicalQp.s32HierarchicalQpDelta[0] = -10;
            stHierarchicalQp.s32HierarchicalQpDelta[1] = -5;
            stHierarchicalQp.s32HierarchicalFrameNum[0] = 2;
            stHierarchicalQp.s32HierarchicalFrameNum[1] = 3;
            RK_MPI_VENC_SetHierarchicalQp(u32Ch, &stHierarchicalQp);
        }

        if (ctx->bMjpegParam && stAttr.stVencAttr.enType == RK_VIDEO_ID_MJPEG) {
            VENC_MJPEG_PARAM_S stMjpegParam;
            memset(&stMjpegParam, 0, sizeof(stMjpegParam));
            /* set qtable for test only, users need set actual qtable */
            memset(&stMjpegParam.u8YQt, 1, sizeof(stMjpegParam.u8YQt));
            memset(&stMjpegParam.u8CbQt, 1, sizeof(stMjpegParam.u8CbQt));
            memset(&stMjpegParam.u8CrQt, 1, sizeof(stMjpegParam.u8CrQt));
            stMjpegParam.u32MCUPerECS = 100;
            RK_MPI_VENC_SetMjpegParam(u32Ch, &stMjpegParam);
        }

        if (ctx->bMjpegRcParam && stAttr.stVencAttr.enType == RK_VIDEO_ID_MJPEG) {
            stRcParam.stParamMjpeg.u32Qfactor = 80;
            stRcParam.stParamMjpeg.u32MaxQfactor = 90;
            stRcParam.stParamMjpeg.u32MinQfactor = 50;
            RK_MPI_VENC_SetRcParam(u32Ch, &stRcParam);
        }

        if (ctx->bFrmLost) {
            VENC_FRAMELOST_S stFrmLost;
            memset(&stFrmLost, 0, sizeof(stFrmLost));
            stFrmLost.bFrmLostOpen = RK_TRUE;
            stFrmLost.u32FrmLostBpsThr = 1;
            stFrmLost.enFrmLostMode = FRMLOST_NORMAL;
            stFrmLost.u32EncFrmGaps = 0;
            RK_MPI_VENC_SetFrameLostStrategy(u32Ch, &stFrmLost);
        }

        if (ctx->u32DeBreath > 0) {
            VENC_DEBREATHEFFECT_S stDeBreathEffect;
            memset(&stDeBreathEffect, 0, sizeof(stDeBreathEffect));
            stDeBreathEffect.bEnable = RK_TRUE;
            stDeBreathEffect.s32Strength1 = ctx->u32DeBreath;
            RK_MPI_VENC_SetDeBreathEffect(u32Ch, &stDeBreathEffect);
        }

        if (ctx->bForceIdr)
            pthread_create(&forceIdrThread[u32Ch], 0, venc_force_idr, reinterpret_cast<void *>(&stVencCtx[u32Ch]));

        memcpy(&(stVencCtx[u32Ch]), ctx, sizeof(TEST_VENC_CTX_S));
	pthread_create(&vencThread[u32Ch], 0, venc_send_frame, reinterpret_cast<void *>(&stVencCtx[u32Ch]));
        if (ctx->bGetStream)
            pthread_create(&getStreamThread[u32Ch], 0, venc_get_stream, reinterpret_cast<void *>(&stVencCtx[u32Ch]));
    }

__FAILED:
    for (u32Ch = ctx->u32ChnStart; u32Ch < ctx->u32ChnStart + ctx->u32ChNum; u32Ch++) {
        if (vencThread[u32Ch])
            pthread_join(vencThread[u32Ch], RK_NULL);
        if (ctx->bGetStream && getStreamThread[u32Ch])
            pthread_join(getStreamThread[u32Ch], RK_NULL);

        stVencCtx[u32Ch].threadExit = RK_TRUE;
        if (ctx->bForceIdr)
            pthread_join(forceIdrThread[u32Ch], RK_NULL);
        RK_MPI_VENC_StopRecvFrame(u32Ch);
        if (ctx->bAttachPool) {
            s32Ret = RK_MPI_VENC_DetachMbPool(u32Ch);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("ch:%d RK_MPI_VENC_DetachMbPool fail:0x%x!", u32Ch, s32Ret);
            }
        }

        RK_MPI_VENC_DestroyChn(u32Ch);
        if (ctx->bAttachPool)
            RK_MPI_MB_DestroyPool(ctx->vencPoolOutput[u32Ch]);
    }
    RK_MPI_MB_DestroyPool(ctx->vencPoolInput);

    if (ctx->bIvs) {
        if (ctx->fp_ivs_result) {
            fsync(fileno(ctx->fp_ivs_result));
            fclose(ctx->fp_ivs_result);
            ctx->fp_ivs_result = NULL;
        }
        destroy_ivs();
    }

    return RK_SUCCESS;
}

static const char *const usages[] = {
    "./rk_mpi_venc_test [-i SRC_PATH] [-w SRC_WIDTH] [-h SRC_HEIGHT]",
    NULL,
};

static void mpi_venc_test_show_options(const TEST_VENC_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input  file name       : %s\n", ctx->srcFileUri);
    RK_PRINT("output file name       : %s\n", ctx->dstFilePath);
    RK_PRINT("src width              : %d\n", ctx->u32SrcWidth);
    RK_PRINT("src height             : %d\n", ctx->u32SrcHeight);
    RK_PRINT("src virWidth           : %d\n", ctx->u32srcVirWidth);
    RK_PRINT("src virHeight          : %d\n", ctx->u32srcVirHeight);
    RK_PRINT("src pixel format       : %d\n", ctx->u32SrcPixFormat);
    RK_PRINT("encode codec type      : %d\n", ctx->u32DstCodec);
    RK_PRINT("loop count             : %d\n", ctx->s32LoopCount);
    RK_PRINT("channel index          : %d\n", ctx->u32ChnIndex);
    RK_PRINT("channel start          : %d\n", ctx->u32ChnStart);
    RK_PRINT("channel num            : %d\n", ctx->u32ChNum);
    RK_PRINT("output buffer count    : %d\n", ctx->u32StreamBufCnt);
    RK_PRINT("one picture size       : %d\n", ctx->u32BufferSize);
    RK_PRINT("out stream size        : %d\n", ctx->u32StreamSize);
    RK_PRINT("gop mode               : %d\n", ctx->enGopMode);
    RK_PRINT("snap picture count     : %d\n", ctx->s32SnapPicCount);
    RK_PRINT("insert user data       : %d\n", ctx->bInsertUserData);
    RK_PRINT("rotation               : %d\n", ctx->enRotation);
    RK_PRINT("compress mode          : %d\n", ctx->enCompressMode);
    RK_PRINT("rc mode                : %d\n", ctx->enRcMode);
    RK_PRINT("bitrate                : %d\n", ctx->u32BitRateKb);
    RK_PRINT("bitrate max            : %d\n", ctx->u32BitRateKbMax);
    RK_PRINT("bitrate mix            : %d\n", ctx->u32BitRateKbMin);
    RK_PRINT("gop size               : %d\n", ctx->u32GopSize);
    RK_PRINT("roi count              : %d\n", ctx->u32RoiTestCount);
    RK_PRINT("mirror                 : %d\n", ctx->enMirror);
    RK_PRINT("fixqp                  : %d\n", ctx->u32FixQp);
    RK_PRINT("full range             : %d\n", ctx->bFullRange);
    RK_PRINT("enable attach mb pool  : %d\n", ctx->bAttachPool);
    RK_PRINT("enable venc ref buf share : %d\n", ctx->bRefBufShare);
    RK_PRINT("rc stat time           : %d\n", ctx->u32StatTime);

    return;
}

int main(int argc, const char **argv) {
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_VENC_CTX_S ctx;
    memset(&ctx, 0, sizeof(TEST_VENC_CTX_S));

    ctx.s32LoopCount    = 1;
    ctx.u32StreamBufCnt = 8;
    ctx.u32ChNum        = 1;
    ctx.u32SrcPixFormat = RK_FMT_YUV420SP;
    ctx.u32DstCodec     = RK_VIDEO_ID_AVC;
    ctx.enCropType      = VENC_CROP_NONE;
    ctx.bFrameRate      = RK_FALSE;
    ctx.s32SnapPicCount = -1;
    ctx.u32GopSize      = 60;
    ctx.enRcMode        = VENC_RC_MODE_H264CBR;
    ctx.u32BitRateKb    = 10 * 1024;
    ctx.bFullRange      = RK_TRUE;
    ctx.u32StatTime = 3;
    ctx.bGetStream      = RK_TRUE;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx.srcFileUri),
                   "input file name.", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx.dstFilePath),
                   "the directory of encoder output", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx.s32LoopCount),
                    "loop running count. default(1)", NULL, 0, 0),
        OPT_INTEGER('w', "width", &(ctx.u32SrcWidth),
                    "input source width. <required>", NULL, 0, 0),
        OPT_INTEGER('h', "height", &(ctx.u32SrcHeight),
                    "input source height. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "vir_width", &(ctx.u32srcVirWidth),
                    "input source virWidth.", NULL, 0, 0),
        OPT_INTEGER('\0', "vir_height", &(ctx.u32srcVirHeight),
                    "input source virHeight.", NULL, 0, 0),
        OPT_INTEGER('f', "pixel_format", &(ctx.u32SrcPixFormat),
                    "input source pixel format. default(0: NV12).", NULL, 0, 0),
        OPT_INTEGER('C', "codec", &(ctx.u32DstCodec),
                     "venc encode codec(8:h264, 9:mjpeg, 12:h265, 15:jpeg, ...). default(8)", NULL, 0, 0),
        OPT_INTEGER('c', "channel_count", &(ctx.u32ChNum),
                     "venc channel count. default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "channel_index", &(ctx.u32ChnIndex),
                     "venc channel index. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "channel_start", &(ctx.u32ChnStart),
                     "venc channel start. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "enc_buf_cnt", &(ctx.u32StreamBufCnt),
                     "venc encode output buffer count, default(8)", NULL, 0, 0),
        OPT_INTEGER('\0', "src_pic_size", &(ctx.u32BufferSize),
                     "the size of input single picture", NULL, 0, 0),
        OPT_INTEGER('\0', "out_stream_size", &(ctx.u32StreamSize),
                     "the size of out stream buff alloc, default(equal to u32BufferSize)", NULL, 0, 0),
        OPT_INTEGER('\0', "crop", &(ctx.enCropType),
                     "crop type(0:none 1:crop_only 2:crop_scale) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "framerate", &(ctx.bFrameRate),
                     "framerate enable flag(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('g', "gop_mode", &(ctx.enGopMode),
                    "gop mode(0/1:NORMALP 2:TSVC2 3:TSVC3 4:TSVC4 5:SMARTP) default(0)", NULL, 0, 0),
        OPT_INTEGER('s', "snap_pic_cnt", &(ctx.s32SnapPicCount),
                    "snap picture count(effective range[-1,0)(0, 2147483647)"
                    "-1:not limit else:snap count) default(-1)", NULL, 0, 0),
        OPT_INTEGER('\0', "insert_user_data", &(ctx.bInsertUserData),
                    "insert user data flag(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "rotation", &(ctx.enRotation),
                     "rotation output(0:0 1:90 2:180 3:270) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "compress_mode", &(ctx.enCompressMode),
                    "set input compressmode(default 0; 0:MODE_NONE 1:AFBC_16x16)", NULL, 0, 0),
        OPT_INTEGER('\0', "rc_mode", &(ctx.enRcMode),
                    "rc mode(0:NULL 1:H264CBR 2:H264VBR 3:H264AVBR 4:H264FIXQP"
                    "5:MJPEGCBR 6:MJPEGVBR 7:MJPEGFIXQP"
                    "8:H265CBR 9:H265VBR 10:H265AVBR 11:H265FIXQP default(1)",
                    NULL, 0, 0),
        OPT_INTEGER('b', "bit_rate", &(ctx.u32BitRateKb),
                    "bit rate kbps(h264/h265:range[3-200000],jpeg/mjpeg:range[5-800000] default(10*1024kb))",
                    NULL, 0, 0),
        OPT_INTEGER('\0', "bit_rate_max", &(ctx.u32BitRateKbMax),
                    "bit rate kbps max(vbr/avbr valid)(h264/h265:range[3-200000];"
                    "jpeg/mjpeg:range[5-800000] default(0:auto calcu))",
                     NULL, 0, 0),
        OPT_INTEGER('\0', "bit_rate_min", &(ctx.u32BitRateKbMin),
                    "bit rate kbps min(vbr/avbr valid)(h264/h265:range[3-200000];"
                    "jpeg/mjpeg:range[5-800000] default(0:auto calcu))",
                     NULL, 0, 0),
        OPT_INTEGER('\0', "gop_size", &(ctx.u32GopSize),
                    "gop size(range >= 1 default(60))", NULL, 0, 0),
        OPT_INTEGER('\0', "mirror", &(ctx.enMirror),
                    "enMirror output(0:none 1:horizontal 2:vertical) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "super_frm", &(ctx.bSuperFrm),
                    "super frm enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "frm_lost", &(ctx.bFrmLost),
                    "frm lost enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "intra_refresh", &(ctx.bIntraRefresh),
                    "intra refresh enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "hier_qp", &(ctx.bHierarchicalQp),
                    "hierarchical qp enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "mjpeg_param", &(ctx.bMjpegParam),
                    "mjpeg param enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "mjpeg_rc_param", &(ctx.bMjpegRcParam),
                    "mjpeg rc param test enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "force_idr", &(ctx.bForceIdr),
                    "force idr enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "roi_count", &(ctx.u32RoiTestCount),
                    "roi test count(range[0-8] default(0))", NULL, 0, 0),
        OPT_INTEGER('\0', "fix_qp", &(ctx.u32FixQp),
                    "set fix qp(this value valid when rc_mode is fixqp;default(0))", NULL, 0, 0),
        OPT_INTEGER('\0', "full_range", &(ctx.bFullRange),
                    "set clolor range(0:limit color range 1:full color range) default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "send_frame_ex", &(ctx.bSendFrameEx),
                    "send frame ex enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "qpmap", &(ctx.bQpmap),
                    "qpmap enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "attach_mb_pool", &(ctx.bAttachPool),
                    "enable attach mb pool or not, default(0), 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "de_breath", &(ctx.u32DeBreath),
                    "debreath[0, 35] default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "venc_ref_buf_share", &(ctx.bRefBufShare),
                    "enable venc ref buf share or not, default(0), 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "ivs", &(ctx.bIvs),
                    "ivs(MD, OD) enable, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "stat_time", &(ctx.u32StatTime),
                    "rc statistic time(range[1,60]) default(3)", NULL, 0, 0),
        OPT_INTEGER('\0', "svc", &(ctx.bSvc),
                    "svc(smart video coding) enable, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "motion_deblur", &(ctx.bMotionDeblur),
                    "motion deblur enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "motion_static_switch", &(ctx.bMotionStaticSwitch),
                    "motion static switch enable(0:disable 1:enable) default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "sao", &(ctx.bSao),
                    "SAO(H265) enable, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "getstream", &(ctx.bGetStream),
                    "get stream enable, default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "scene_mode", &(ctx.u32SceneMode),
                    "set scene mode(0: ipc, 1: sport dv, 2: cvr), default(0)", NULL, 0, 0),
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

    if (unit_test_mpi_venc(&ctx) < 0) {
        goto __FAILED;
    }
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
