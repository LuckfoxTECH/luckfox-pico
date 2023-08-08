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

typedef struct rkResolution_st
{
    SIZE_S size;
    const char  *resStr;
}RK_RESOLUTION_ST;

typedef struct rkRGN_CFG_S {
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stRgnChnAttr;
} RGN_CFG_S;

typedef enum rkTestVIMODE_E {
    TEST_VI_MODE_VI_ONLY = 0,
    TEST_VI_CHANGE_RESOLUTION = 1,
    TEST_VI_MULTI_VI = 2,
    TEST_VI_RGN = 3,
} TEST_VI_MODE_E;

typedef struct _rkMpiVICtx {
    RK_S32 devId;
    RK_S32 pipeId;
    RK_S32 chnId;
    RK_S32 loopCountSet;
    VI_DEV_STATUS_S stDevStatus;
    VI_DEV_ATTR_S stDevAttr;
    VI_CHN_ATTR_S stChnAttr;

    VI_SAVE_FILE_INFO_S stDebugFile;
    VIDEO_FRAME_INFO_S stViFrame;
    VI_CHN_STATUS_S stChnStatus;
    TEST_VI_MODE_E enMode;
    // for vi
    RGN_CFG_S stViRgn;
    RK_S32 s32RgnCnt;
    RK_S32 rgnType;

    RK_U32 maxWidth;
    RK_U32 maxHeight;
    RK_U32 u32ChangeRes;
    RK_BOOL bEnRgn;
} TEST_VI_CTX_S;

static void mpi_vi_test_show_options(const TEST_VI_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("output file open      : %d\n", ctx->stDebugFile.bCfg);
    RK_PRINT("yuv output file name  : %s/%s\n", ctx->stDebugFile.aFilePath, ctx->stDebugFile.aFileName);
    RK_PRINT("loop count            : %d\n", ctx->loopCountSet);
    RK_PRINT("ctx->enMode           : %d\n", ctx->enMode);
    RK_PRINT("ctx->dev              : %d\n", ctx->devId);
    RK_PRINT("ctx->pipe             : %d\n", ctx->pipeId);
    RK_PRINT("ctx->channel          : %d\n", ctx->chnId);
    RK_PRINT("width                 : %d\n", ctx->stChnAttr.stSize.u32Width);
    RK_PRINT("height                : %d\n", ctx->stChnAttr.stSize.u32Height);
    RK_PRINT("out buf count         : %d\n", ctx->stChnAttr.stIspOpt.u32BufCount);
    RK_PRINT("max width             : %d\n", ctx->maxWidth);
    RK_PRINT("max height            : %d\n", ctx->maxHeight);
    RK_PRINT("get echo res frame    : %d\n", ctx->u32ChangeRes);
    RK_PRINT("bEnRgn                : %d\n", ctx->bEnRgn);
    RK_PRINT("rgn count             : %d\n", ctx->s32RgnCnt);
    RK_PRINT("rgn type              : %d\n", ctx->rgnType);
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

static RK_S32 vi_dup_get_frame(TEST_VI_CTX_S *ctx) {
    RK_S32  s32Ret            = 0;
    RK_S32  loopCount         = 0;
    RK_S32  waitTime          = 100;
    FILE    *fp               = RK_NULL;
    RK_CHAR output[256]       = {0};

    snprintf(output, sizeof(output), "%s/%s",
            ctx->stDebugFile.aFilePath, ctx->stDebugFile.aFileName);
    fp = fopen(output, "wb");
    if (!fp) {
        RK_LOGE("CHN:%d can't open file %s", ctx->chnId, output);
        return RK_FAILURE;
    }

    while (loopCount < ctx->loopCountSet) {
        // get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->chnId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *pData = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGI("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d",
                     pData, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef, ctx->stViFrame.stVFrame.u64PTS / 1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);
            RK_LOGD("w:%d,h:%d, vw:%d,vh:%d, delay=%lldus",
                     ctx->stViFrame.stVFrame.u32Width,
                     ctx->stViFrame.stVFrame.u32Height,
                     ctx->stViFrame.stVFrame.u32VirWidth,
                     ctx->stViFrame.stVFrame.u32VirHeight,
                     nowUs - ctx->stViFrame.stVFrame.u64PTS);

            fwrite(pData, 1, ctx->stViFrame.stVFrame.u64PrivateData, fp);
            fflush(fp);
            // get the channel status
            // s32Ret = RK_MPI_VI_QueryChnStatus(ctx->pipeId, ctx->chnId, &ctx->stChnStatus);
            // RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d," \
            //         "delay=%lldus",
            //          s32Ret,
            //          ctx->stViFrame.stVFrame.u32Width,
            //          ctx->stViFrame.stVFrame.u32Height,
            //          nowUs - ctx->stViFrame.stVFrame.u64PTS);

            // release the frame
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->chnId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
        } else {
            static int lessPrintCnt = 0;
            lessPrintCnt++;
            if (lessPrintCnt < 10)
                RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }
        usleep(1*1000);
    }

    if (fp)
        fclose(fp);

__FAILED:
    return s32Ret;
}

static int getResolution(const RK_RESOLUTION_ST *resos, int size, RK_U32 w, RK_U32 h) {
    int i = 0;
    for (; i < size; i++) {
        if ( resos[i].size.u32Width == w && resos[i].size.u32Height == h) {
            return i;
        }
    }

    return 0;
}

static int vi_dup_change_resolution(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret;
    RK_S32 loopCount = 0;
    RK_S32 waitTime = 100;
    RK_U32 resMaxCnt = 0;
    RK_U32 resoIdx = -1;
    RK_U32 curW = 0, curH = 0;
    FILE   *fp = RK_NULL;
    RK_CHAR output[256] = {0};

    RK_U32 getTimesToChgRes = ctx->u32ChangeRes;
    resMaxCnt = RK_ARRAY_ELEMS(test_res);

    if (ctx->loopCountSet == 0)
        ctx->loopCountSet = getTimesToChgRes * (resMaxCnt + 1);  // all resolution covered

    ctx->stChnAttr.stIspOpt.stMaxSize.u32Width  = ctx->maxWidth;
    ctx->stChnAttr.stIspOpt.stMaxSize.u32Height = ctx->maxHeight;

    while (loopCount < ctx->loopCountSet) {
        // 5.get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->chnId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *pData = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGD("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d", pData, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef, ctx->stViFrame.stVFrame.u64PTS / 1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);
            RK_LOGD("w:%d,h:%d, vw:%d,vh:%d, delay=%lldus",
                     ctx->stViFrame.stVFrame.u32Width,
                     ctx->stViFrame.stVFrame.u32Height,
                     ctx->stViFrame.stVFrame.u32VirWidth,
                     ctx->stViFrame.stVFrame.u32VirHeight,
                     nowUs - ctx->stViFrame.stVFrame.u64PTS);
            curW = ctx->stViFrame.stVFrame.u32Width;
            curH = ctx->stViFrame.stVFrame.u32Height;

            if (!fp) {
                //output file name
                snprintf(ctx->stDebugFile.aFileName, MAX_VI_FILE_PATH_LEN,
                        "reso_%d_%d_%d_%d.bin", ctx->devId, ctx->chnId,
                        curW, curH);
                //output file full name
                snprintf(output, sizeof(output), "%s/%s",
                        ctx->stDebugFile.aFilePath, ctx->stDebugFile.aFileName);
                if (ctx->stDebugFile.bCfg) {
                    fp = fopen(output, "wb");
                    if (!fp) {
                        RK_LOGE("CHN:%d can't open file %s", ctx->chnId, output);
                        return RK_FAILURE;
                    }
                }
            }
            //write current frame to fp
            if (ctx->stDebugFile.bCfg) {
                fwrite(pData, 1, ctx->stViFrame.stVFrame.u64PrivateData, fp);
                fflush(fp);
            }
            // s32Ret = RK_MPI_VI_QueryChnStatus(ctx->pipeId, ctx->chnId, &ctx->stChnStatus);
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->chnId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
__RETRY:
            if ((loopCount % getTimesToChgRes) == 0) {
                if (resoIdx >= resMaxCnt && resoIdx > 0) {
                    resoIdx = 0;
                }

                if (fp) {
                    fclose(fp);     // close curr resolution file handle
                    fp = RK_NULL;
                }

                if (resoIdx < 0) {
                    // get curr reso index
                    resoIdx = getResolution(test_res, resMaxCnt, curW, curH);
                } else {
                    if (test_res[resoIdx].size.u32Width > ctx->maxWidth ||
                        test_res[resoIdx].size.u32Height > ctx->maxHeight) {
                        resoIdx++;
                        goto __RETRY;
                    }
                }

                if (loopCount != ctx->loopCountSet) {
                    if (resoIdx >= 0 && loopCount != getTimesToChgRes) {
                        if (resoIdx != 0) {
                            RK_LOGD("res index: %d, write %s done", resoIdx , test_res[resoIdx].resStr);
                            resoIdx++;
                        } else {
                            RK_LOGD("res index: 0, write %s done", test_res[0].resStr);
                        }
                    } else {
                        // current reso have done
                        if (test_res[resoIdx].size.u32Width == curW && test_res[resoIdx].size.u32Height == curH) {
                            RK_LOGD("write %s/%s done", ctx->stDebugFile.aFilePath, ctx->stDebugFile.aFileName);
                            resoIdx++;
                        }
                    }

                    if (test_res[resoIdx].size.u32Width > ctx->maxWidth ||
                        test_res[resoIdx].size.u32Height > ctx->maxHeight) {
                        resoIdx++;
                        goto __RETRY;
                    }
                } else {
                    RK_LOGD("res index: %d, write %s done", resoIdx - 1, test_res[resoIdx-1].resStr);
                    break;
                }

                if (test_res[resoIdx].size.u32Width == curW &&
                     test_res[resoIdx].size.u32Height == curH &&
                     resoIdx < resMaxCnt)
                        resoIdx++;
                RK_LOGI("+++ Current resoIdx = %d +++", resoIdx);
                ctx->stChnAttr.stSize.u32Width  = test_res[resoIdx].size.u32Width;
                ctx->stChnAttr.stSize.u32Height = test_res[resoIdx].size.u32Height;

                s32Ret = RK_MPI_VI_SetChnAttr(ctx->pipeId, ctx->chnId, &ctx->stChnAttr);
                if (s32Ret != RK_SUCCESS) {
                    RK_LOGE("RK_MPI_VI_SetChnAttr %d x %d fail, ret: %x",
                            test_res[resoIdx].size.u32Width, test_res[resoIdx].size.u32Height, s32Ret);
                }
                else {
                    RK_LOGD("RK_MPI_VI_SetChnAttr %d x %d success",
                            test_res[resoIdx].size.u32Width, test_res[resoIdx].size.u32Height);
                }
            }
        } else {
            RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }

        usleep(1*1000);
    }

__FAILED:
    return s32Ret;
}

int vi_chn_init(int channelId, int width, int height, int bufCnt) {
    int ret = RK_SUCCESS;

    // VI init
    VI_CHN_ATTR_S vi_chn_attr;
    memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
    printf("++++++ chn:%d: %d x %d, bufCnt:%d ++++++\n",channelId, width, height, bufCnt);
    vi_chn_attr.stIspOpt.u32BufCount = bufCnt; // 1 | 2 | 3 | 4
    vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
    vi_chn_attr.stSize.u32Width = width;
    vi_chn_attr.stSize.u32Height = height;
    vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
    vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
    vi_chn_attr.u32Depth = 2;

    ret = RK_MPI_VI_GetDevIsEnable(0);
    if (ret)
        ret = RK_MPI_VI_EnableDev(0);

    ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);

    RK_S64 s64ViEnSta = TEST_COMM_GetNowUs();
    ret |= RK_MPI_VI_EnableChn(0, channelId);
    RK_S64 s64ViEnEnd = TEST_COMM_GetNowUs();
    printf("  vi en: %lld us\n", s64ViEnEnd - s64ViEnSta);

    if (ret) {
        printf("ERROR: create VI  %d, error! ret=%d\n", channelId, ret);
        return ret;
    }

    return ret;
}

static RK_S32 test_venc_init(int chnId, int width, int height, RK_CODEC_ID_E enType)
{
    VENC_RECV_PIC_PARAM_S stRecvParam;
    VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
    VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
    VENC_CHN_ATTR_S stAttr;

    memset(&stAttr,0,sizeof(VENC_CHN_ATTR_S));
    stVencChnBufWrap.bEnable = RK_FALSE;
    stVencChnBufWrap.u32BufLine = 1080;

    memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
    stVencChnRefBufShare.bEnable = RK_TRUE;

    if (enType == RK_VIDEO_ID_AVC) {
        stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
        stAttr.stRcAttr.stH264Cbr.u32BitRate = 2 * 1024;
        stAttr.stRcAttr.stH264Cbr.u32Gop = 60;
    } else if (enType == RK_VIDEO_ID_HEVC) {
        stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
        stAttr.stRcAttr.stH265Cbr.u32BitRate = 2 * 1024;
        stAttr.stRcAttr.stH265Cbr.u32Gop = 60;
    }

    stAttr.stVencAttr.enType = enType;
    stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
    if (enType == RK_VIDEO_ID_AVC)
        stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
    else if (enType == RK_VIDEO_ID_HEVC)
        stAttr.stVencAttr.u32Profile = H265E_PROFILE_MAIN;
    stAttr.stVencAttr.u32PicWidth = width;
    stAttr.stVencAttr.u32PicHeight = height;
    stAttr.stVencAttr.u32VirWidth = width;
    stAttr.stVencAttr.u32VirHeight = height;
    stAttr.stVencAttr.u32StreamBufCnt = 5;
    stAttr.stVencAttr.u32BufSize = width * height / 2;
    stAttr.stVencAttr.enMirror = MIRROR_NONE;

    RK_MPI_VENC_CreateChn(chnId, &stAttr);
    RK_MPI_VENC_SetChnBufWrapAttr(chnId, &stVencChnBufWrap);
    RK_MPI_VENC_SetChnRefBufShareAttr(chnId, &stVencChnRefBufShare);

    memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    stRecvParam.s32RecvPicNum = -1;
    RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

    return 0;
}

static bool quit = false;
static RK_S32 g_s32FrameCnt = -1;
static void *GetMediaBuffer(void *arg) {
    void *pData = RK_NULL;
    int loopCount = 0;
    int s32Ret;
    VENC_STREAM_S stFrame;
    int chn = (int)arg;

    stFrame.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S));
    while (!quit) {
        s32Ret = RK_MPI_VENC_GetStream(chn, &stFrame, 1000);
        if (s32Ret == RK_SUCCESS) {
            if (loopCount < 10000)
                printf("chn:%d, loopCount:%d enc->seq:%d,pts=%lld\n", chn, loopCount,
                    stFrame.u32Seq, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
            loopCount++;
            s32Ret = RK_MPI_VENC_ReleaseStream(chn, &stFrame);
        }
        if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
            quit = true;
            break;
        }
    }
    free(stFrame.pstPack);
    return NULL;
}

static void *GetViBuffer(void *arg) {
    void *pData = RK_NULL;
    int loopCount = 0;
    int s32Ret;
    VIDEO_FRAME_INFO_S stFrame;
    int chn = (int)arg;
    int pipe = ((int)arg >> 16);

    while (!quit) {
        s32Ret = RK_MPI_VI_GetChnFrame(pipe, chn, &stFrame, 1000);
        if (s32Ret == RK_SUCCESS) {
            if (loopCount < 10000)
                printf("chn:%d, loopCount:%d vi->seq:%d pts=%lld\n", chn, loopCount,
                    stFrame.stVFrame.u32TimeRef, stFrame.stVFrame.u64PTS);
            loopCount++;
            s32Ret = RK_MPI_VI_ReleaseChnFrame(pipe, chn, &stFrame);
        }
        if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
            quit = true;
            break;
        }
    }
    return NULL;
}

static int vi_dup_muti_vi_get_frame(TEST_VI_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_AVC;//RK_VIDEO_ID_AVC RK_VIDEO_ID_HEVC

    test_venc_init(1, 1280, 720, enCodecType);
    vi_chn_init(1, 1280, 720, ctx->stChnAttr.stIspOpt.u32BufCount);
    vi_chn_init(2, 640, 360, ctx->stChnAttr.stIspOpt.u32BufCount);
    g_s32FrameCnt = ctx->loopCountSet;

    MPP_CHN_S stSrcChn, stDestChn;
    // bind vi to venc
    stSrcChn.enModId    = RK_ID_VI;
    stSrcChn.s32DevId   = 0;
    stSrcChn.s32ChnId   = 1;

    stDestChn.enModId   = RK_ID_VENC;
    stDestChn.s32DevId  = 0;
    stDestChn.s32ChnId  = 1;
    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_SYS_Bind fail:0x%08x", s32Ret);
        goto __FAILED;
    }

    pthread_t main_thread1;
	pthread_create(&main_thread1, NULL, GetMediaBuffer, (void*)1);
    pthread_t main_thread2;
	pthread_create(&main_thread2, NULL, GetViBuffer, (void*)2);
    pthread_t main_thread3;
	pthread_create(&main_thread3, NULL, GetViBuffer, (void*)1);

    pthread_join(main_thread1, RK_NULL);
    pthread_join(main_thread2, RK_NULL);
    pthread_join(main_thread3, RK_NULL);

    s32Ret |= RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    s32Ret |= RK_MPI_VI_DisableChn(0, 1);
    s32Ret |= RK_MPI_VI_DisableChn(0, 2);
    s32Ret |= RK_MPI_VENC_StopRecvFrame(1);
    if (s32Ret != RK_SUCCESS) {
        // return s32Ret;
        RK_LOGE("test fail :0x%08x", s32Ret);
    }

    s32Ret = RK_MPI_VENC_DestroyChn(1);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VENC_DestroyChn :0x%08x", s32Ret);
    }

    s32Ret = RK_MPI_VI_StopPipe(0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VI_StopPipe :0x%08x", s32Ret);
    }

__FAILED:
    RK_LOGE("test running exit:0x%08x", s32Ret);
    RK_MPI_SYS_Exit();

    return 0;
}

static RK_S32 create_rgn(TEST_VI_CTX_S *ctx) {
    RK_S32 i;
    RK_S32 s32Ret = RK_SUCCESS;
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stRgnChnAttr;
    RGN_HANDLE RgnHandle = 0;
    MPP_CHN_S stMppChn;
    RGN_TYPE_E rgnType = (RGN_TYPE_E)ctx->rgnType;

    if (rgnType > RGN_BUTT || rgnType < OVERLAY_RGN)
        return RK_FAILURE;
    /****************************************
     step 1: create overlay regions
    ****************************************/
    for (i = 0; i < ctx->s32RgnCnt; i++) {
        ctx->stViRgn.stRgnAttr.enType = rgnType;
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
        stMppChn.enModId = RK_ID_VI;
        stMppChn.s32DevId = ctx->devId;
        stMppChn.s32ChnId = VI_MAX_CHN_NUM;
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
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = 0;
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 128 * i;
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 128;
                stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 128;
                stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0x0000ff << ((i % 3) * 8);  // blue,green,red loop
                stCoverChnAttr.unChnAttr.stCoverChn.enCoordinate = RGN_ABS_COOR;
                stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = i;
                if ((i + 1) % 4 == 0 && i != 0) {
                    stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0xffffff;    // white
                }

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
                RK_LOGE("Unsupport rgn type: %d", rgnType);
                break;
        }
    }

    return RK_SUCCESS;
__EXIT:
    for (i = 0; i < ctx->s32RgnCnt; i++) {
        stMppChn.enModId = RK_ID_VI;
        stMppChn.s32DevId = ctx->devId;
        stMppChn.s32ChnId = ctx->chnId;
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
    /* use vi max chn as distinguish rgn atcing on pipe or chn */
    stMppChn.s32ChnId = VI_MAX_CHN_NUM;

    // detach rgn
    for (RK_S32 i = 0; i < ctx->s32RgnCnt; i++) {
        RgnHandle = i;
        s32Ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
        if (RK_SUCCESS != s32Ret) {
            RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", RgnHandle, s32Ret);
            return RK_FAILURE;
        }
        RK_LOGI("detach handle:%d from channel:%d success", RgnHandle, stMppChn.s32ChnId);
    }

    // destroy rgn
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

static int vi_dup_rgn_test(TEST_VI_CTX_S *ctx) {
    RK_S32  s32Ret            = 0;
    RK_S32  loopCount         = 0;
    RK_S32  waitTime          = 100;
    FILE    *fp               = RK_NULL;
    RK_CHAR output[256]       = {0};

    snprintf(output, sizeof(output), "%s/%s",
            ctx->stDebugFile.aFilePath, ctx->stDebugFile.aFileName);
    fp = fopen(output, "wb");
    if (!fp) {
        RK_LOGE("CHN:%d can't open file %s", ctx->chnId, output);
        return RK_FAILURE;
    }

    if (ctx->bEnRgn) {
        s32Ret = create_rgn(ctx);
        if (RK_FAILURE == s32Ret)
            goto __FAILED;
    }

    while (loopCount < ctx->loopCountSet) {
        // get the frame
        s32Ret = RK_MPI_VI_GetChnFrame(ctx->pipeId, ctx->chnId, &ctx->stViFrame, waitTime);
        if (s32Ret == RK_SUCCESS) {
            RK_U64 nowUs = TEST_COMM_GetNowUs();
            void *pData = RK_MPI_MB_Handle2VirAddr(ctx->stViFrame.stVFrame.pMbBlk);
            RK_LOGI("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%d", pData, loopCount,
                     ctx->stViFrame.stVFrame.u32TimeRef, ctx->stViFrame.stVFrame.u64PTS / 1000,
                     ctx->stViFrame.stVFrame.u64PrivateData);
            RK_LOGD("w:%d,h:%d, vw:%d,vh:%d, delay=%lldus",
                     ctx->stViFrame.stVFrame.u32Width,
                     ctx->stViFrame.stVFrame.u32Height,
                     ctx->stViFrame.stVFrame.u32VirWidth,
                     ctx->stViFrame.stVFrame.u32VirHeight,
                     nowUs - ctx->stViFrame.stVFrame.u64PTS);

            fwrite(pData, 1, ctx->stViFrame.stVFrame.u64PrivateData, fp);
            fflush(fp);
            // release the frame
            s32Ret = RK_MPI_VI_ReleaseChnFrame(ctx->pipeId, ctx->chnId, &ctx->stViFrame);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            }
            loopCount++;
        } else {
            static int lessPrintCnt = 0;
            lessPrintCnt++;
            if (lessPrintCnt < 10)
                RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
        }
        usleep(1*1000);
    }

__FAILED:
    if (fp)
        fclose(fp);

    if (ctx->bEnRgn)
        destory_rgn(ctx);

    s32Ret = RK_MPI_VI_DisableChn(ctx->pipeId, ctx->chnId);
    RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);
    RK_LOGD("vi_dup_rgn_test over");

    return s32Ret;
}

//-------------------------------------------------------------------------------------------------
static const char *const usages[] = {
    // for vi get frame from chn 0 just
    "\t1)./rk_mpi_vi_dup_test -c 0 -d 0 -m 0 -l 10 -o 1",
    // for vi resolution test, chn can change 0|1|2.
    "\t2)./rk_mpi_vi_dup_test --maxWidth 2560 --maxHeight 1440 -c 0 -m 1 -o 1",
    // for complex scene functional test: get video frame from chn 1 meanwhile encode, get video frame from ch2.
    // buf cnt can config 1-4
    "\t3)./rk_mpi_vi_dup_test -d 0 -m 2 -l 30 --buf_cnt 2",
    // for rgn cross process test
    "\t4)./rk_mpi_vi_dup_test -d 0 -c 0 ([1|2]) -m 3 --en_rgn 1 --rgn_type 2 ([2|3]) --rgn_cnt 2 ([1:8]) -l 10 -o 1",
    "",
    RK_NULL,
};

int main(int argc, const char **argv) {
    RK_S32 i;
    RK_S32 s32Ret = RK_FAILURE;
    TEST_VI_CTX_S *ctx;
    ctx = reinterpret_cast<TEST_VI_CTX_S *>(malloc(sizeof(TEST_VI_CTX_S)));
    memset(ctx, 0, sizeof(TEST_VI_CTX_S));

    ctx->devId = 0;
    ctx->pipeId = ctx->devId;
    ctx->chnId = 0;
    ctx->loopCountSet = 100;
    ctx->enMode = TEST_VI_MODE_VI_ONLY;
    ctx->stChnAttr.stIspOpt.u32BufCount = 3;
    ctx->stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
    ctx->stChnAttr.stIspOpt.bNoUseLibV4L2 = RK_TRUE;
    ctx->stChnAttr.u32Depth = 0;
    ctx->stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
    ctx->stChnAttr.stFrameRate.s32SrcFrameRate = -1;
    ctx->stChnAttr.stFrameRate.s32DstFrameRate = -1;
    ctx->stChnAttr.stSize.u32Width = 1920;
    ctx->stChnAttr.stSize.u32Height = 1080;
    ctx->maxWidth = 0;
    ctx->maxHeight = 0;
    ctx->u32ChangeRes = 20;

    // rgn
    ctx->s32RgnCnt = 1;
    ctx->bEnRgn = RK_FALSE;
    ctx->rgnType = RGN_BUTT;    // for 1106:COVER_RGN / MOSAIC_RGN

    RK_LOGD("test running enter!");

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('w', "dev", &(ctx->stChnAttr.stSize.u32Width),
                    "set dev id(default 0)", NULL, 0, 0),
        OPT_INTEGER('h', "pipe", &(ctx->stChnAttr.stSize.u32Height),
                    "set pipe id(default 0)", NULL, 0, 0),
        OPT_INTEGER('d', "dev", &(ctx->devId),
                    "set dev id(default 0)", NULL, 0, 0),
        OPT_INTEGER('p', "pipe", &(ctx->pipeId),
                    "set pipe id(default 0)", NULL, 0, 0),
        OPT_INTEGER('c', "channel", &(ctx->chnId),
                    "set channel id(default 1)", NULL, 0, 0),
        OPT_INTEGER('l', "loopcount", &(ctx->loopCountSet),
                    "set capture frame count(default 100)", NULL, 0, 0),
        OPT_INTEGER('o', "output", &(ctx->stDebugFile.bCfg),
                    "save output file, file at /data/test_<devid>_<pipeid>_<chnid>.bin"
                    " (default 0; 0:no save 1:save)", NULL, 0, 0),
        OPT_INTEGER('m', "mode", &(ctx->enMode),
                    "test mode(default 1; \n\t"
                    "0:vi get&release frame \n\t", NULL, 0, 0),
        OPT_INTEGER('D', "depth", &(ctx->stChnAttr.u32Depth),
                    "channel output depth, default{u32BufCount(not bind) or 0(bind venc/vpss/...)}", NULL, 0, 0),
        OPT_INTEGER('f', "format", &(ctx->stChnAttr.enPixelFormat),
                    "set the format(default 0; 0:RK_FMT_YUV420SP 10:RK_FMT_YUV422_UYVY"
                    "131080:RK_FMT_RGB_BAYER_SBGGR_12BPP)", NULL, 0, 0),
        OPT_INTEGER('\0', "buf_cnt", &(ctx->stChnAttr.stIspOpt.u32BufCount),
                    "out buf count, range[1-8] default(3)", NULL, 0, 0),
        OPT_INTEGER('\0', "maxWidth", &(ctx->maxWidth),
                    "config max resolution width(<= sensor max resolution width)", NULL, 0, 0),
        OPT_INTEGER('\0', "maxHeight", &(ctx->maxHeight),
                    "config max resolution height(<= sensor max resolution height)", NULL, 0, 0),
        OPT_INTEGER('\0', "change_res", &(ctx->u32ChangeRes),
                    "change res by frames(default(20))", NULL, 0, 0),
        OPT_INTEGER('\0', "en_rgn", &(ctx->bEnRgn),
                    "enable rgn. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "rgn_type", &(ctx->rgnType),
                    "rgn type. 0:overlay,1:overlay-ex 2:cover,3:mosaic", NULL, 0, 0),
        OPT_INTEGER('\0', "rgn_cnt", &(ctx->s32RgnCnt),
                    "rgn count. default(1),max:8", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");
    argc = argparse_parse(&argparse, argc, argv);

    if (ctx->pipeId != ctx->devId)
        ctx->pipeId = ctx->devId;

    if (ctx->pipeId != ctx->devId)
        ctx->pipeId = ctx->devId;

    mpi_vi_test_show_options(ctx);

    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        RK_LOGE("rk mpi sys init fail!");
        goto __FAILED;
    }

    if (ctx->stDebugFile.bCfg) {
        memcpy(&ctx->stDebugFile.aFilePath, "/data", strlen("/data"));
    }

    switch (ctx->enMode) {
        case TEST_VI_MODE_VI_ONLY:
            if (!ctx->stChnAttr.u32Depth) {
                RK_LOGE("depth need > 0 when vi not bind any other module!");
                ctx->stChnAttr.u32Depth = ctx->stChnAttr.stIspOpt.u32BufCount;
            }
            snprintf(ctx->stDebugFile.aFileName, MAX_VI_FILE_PATH_LEN,
                    "test_vi_dup_%d_%d_%d.bin", ctx->devId, ctx->pipeId, ctx->chnId);

            s32Ret = vi_dup_get_frame(ctx);
        break;

        case TEST_VI_CHANGE_RESOLUTION:
            s32Ret = vi_dup_change_resolution(ctx);
        break;

        case TEST_VI_MULTI_VI:
            s32Ret = vi_dup_muti_vi_get_frame(ctx);
        break;

        case TEST_VI_RGN:
            snprintf(ctx->stDebugFile.aFileName, MAX_VI_FILE_PATH_LEN,
                    "test_vi_dup_rgn_%d_%d_%d.bin", ctx->devId, ctx->pipeId, ctx->chnId);
            s32Ret = vi_dup_rgn_test(ctx);
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
