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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <pthread.h>

#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_vo.h"
#include "rk_mpi_vdec.h"
#include "rk_mpi_mb.h"
#include "rk_common.h"

#include "test_comm_argparse.h"
#include "test_comm_utils.h"

#define MAX_FRAME_QUEUE              8
#define MAX_TIME_OUT_MS              20

#ifndef VDEC_INT64_MIN
#define VDEC_INT64_MIN               (-0x7fffffffffffffffLL-1)
#endif

#ifndef VDEC_INT64_MAX
#define VDEC_INT64_MAX               INT64_C(9223372036854775807)
#endif

#ifndef rk_safe_free
#define rk_safe_free(p)              { if (p) {free(p); (p)=RK_NULL;} }
#endif

#ifndef rk_safe_delete
#define rk_safe_delete(p)            { if (p) {delete(p); (p)=RK_NULL;} }
#endif

#define VDEC_ARRAY_ELEMS(a)          (sizeof(a) / sizeof((a)[0]))

typedef struct rkVO_Timing_S {
    RK_S32  enIntfSync;
    RK_U32  u32Width;
    RK_U32  u32Height;
} VO_Timing_S;

typedef struct _rkWbcCfg {
    VO_WBC_SOURCE_S stWbcSource;
    VO_WBC_ATTR_S stWbcAttr;
    RK_S32 s32ChnId;
    RK_S32 s32VdecChnId;
} WBC_CFG;

typedef struct _rkVdecCfg {
    RK_U32 u32FrameBufferCnt;
    COMPRESS_MODE_E enCompressMode;
} VDEC_CFG;

typedef struct _rkVOCfg {
    RK_U32 u32Screen0VoLayer;
    RK_U32 u32Screen1VoLayer;

    RK_U32 u32Screen0Rows;
    RK_U32 u32Screen1Rows;
    RK_BOOL bDoubleScreen;
} VO_CFG;

typedef struct _rkParserCfg {
    char *srcFileUri[VDEC_MAX_CHN_NUM];
    RK_CODEC_ID_E enCodecId;
    RK_U32 u32SrcWidth;
    RK_U32 u32SrcHeight;
    RK_U32 u32StreamIndex;
    void  *pExtraData;
    RK_U32 u32ExtraDataSize;
} PARSER_CFG;

typedef struct _rkBorderCfg {
    RK_U32 u32LeftWidth;
    RK_U32 u32RightWidth;
    RK_U32 u32TopWidth;
    RK_U32 u32BottomWidth;
} Border_CFG;

#define RK356X_VOP_LAYER_CLUSTER_0      0
#define RK356X_VOP_LAYER_CLUSTER_1      2
#define RK356X_VOP_LAYER_ESMART_0       4
#define RK356X_VOP_LAYER_ESMART_1       5
#define RK356X_VOP_LAYER_SMART_0        6
#define RK356X_VOP_LAYER_SMART_1        7

#define RK356X_VO_DEV_HD0               0
#define RK356X_VO_DEV_HD1               1

#define DISPLAY_TYPE_HDMI               0
#define DISPLAY_TYPE_EDP                1
#define DISPLAY_TYPE_VGA                2
#define DISPLAY_TYPE_DP                 3
#define DISPLAY_TYPE_HDMI_EDP           4
#define DISPLAY_TYPE_MIPI               5

typedef struct _PixelFormatMap {
    RK_U32 index;
    PIXEL_FORMAT_E format;
} PixelFormatMap;

const static PixelFormatMap testPixelFormat[] = {
    // usual yuv/rga formats
    {1,              RK_FMT_YUV420SP},
    {2,              RK_FMT_YUV420SP_VU},
    {3,              RK_FMT_RGB565},
    {4,              RK_FMT_BGR565},
    {5,              RK_FMT_RGB888},
    {6,              RK_FMT_BGR888},
    {7,              RK_FMT_BGRA8888},
    {8,              RK_FMT_RGBA8888},
    {9,              RK_FMT_RGBA5551},
    {10,             RK_FMT_BGRA5551}
};

#define MAX_VO_FORMAT_RGB_NUM        17
#define WBC_FORMAT_BGRA8888          0
#define WBC_FORMAT_RGBA8888          1
#define WBC_FORMAT_RGB888            2
#define WBC_FORMAT_BGR888            3
#define WBC_FORMAT_YUV420SP          4
#define WBC_FORMAT_YUV420SP_VU       5

#define VO_CHANNEL_PLAY_NORMAL           0
#define VO_CHANNEL_PLAY_PAUSE            1
#define VO_CHANNEL_PLAY_STEP             2
#define VO_CHANNEL_PLAY_SPEED            3

#define WBC_SOURCE_DEV                0
#define WBC_SOURCE_VIDEO              1
#define WBC_SOURCE_GRAPHIC            2

#define MAX_WINDOWS_NUM               64
#define MAX_STEP_FRAME_NUM            50

#define ARRAY_LENGTH(a) (sizeof (a) / sizeof (a)[0])

typedef struct _TEST_MODE {
    RK_U32 mode;
    VO_INTF_SYNC_E enIntfSync;
} TEST_MODE_S;

static VO_DEV VoLayer = RK356X_VOP_LAYER_CLUSTER_0;
static VO_DEV VoLayer_second = RK356X_VOP_LAYER_CLUSTER_1;

TEST_MODE_S test_mode_table[] = {
    {0,     VO_OUTPUT_640x480_60},
    {1,     VO_OUTPUT_PAL},
    {2,     VO_OUTPUT_576P50},
    {3,     VO_OUTPUT_NTSC},
    {4,     VO_OUTPUT_480P60},
    {5,     VO_OUTPUT_800x600_60},
    {6,     VO_OUTPUT_1024x768_60},
    {7,     VO_OUTPUT_720P50},
    {8,     VO_OUTPUT_720P60},
    {9,     VO_OUTPUT_1280x1024_60},
    {10,    VO_OUTPUT_1080P24},
    {11,    VO_OUTPUT_1080P25},
    {12,    VO_OUTPUT_1080P30},
    {13,    VO_OUTPUT_1080I50},
    {14,    VO_OUTPUT_1080I60},
    {15,    VO_OUTPUT_1080P50},
    {16,    VO_OUTPUT_1080P60},
    {17,    VO_OUTPUT_3840x2160_24},
    {18,    VO_OUTPUT_3840x2160_25},
    {19,    VO_OUTPUT_3840x2160_30},
    {20,    VO_OUTPUT_3840x2160_50},
    {21,    VO_OUTPUT_3840x2160_60},
    {22,    VO_OUTPUT_4096x2160_24},
    {23,    VO_OUTPUT_4096x2160_25},
    {24,    VO_OUTPUT_4096x2160_30},
    {25,    VO_OUTPUT_4096x2160_50},
    {26,    VO_OUTPUT_4096x2160_60},
    {27,    VO_OUTPUT_DEFAULT},
};

static VO_Timing_S stVoTimings[] = {
    {VO_OUTPUT_640x480_60, 640, 480},
    {VO_OUTPUT_800x600_60, 800, 600},
    {VO_OUTPUT_1024x768_60, 1024, 768},
    {VO_OUTPUT_1280x1024_60, 1280, 1024},
    {VO_OUTPUT_1366x768_60, 1366, 768},
    {VO_OUTPUT_1440x900_60, 1440, 900},
    {VO_OUTPUT_1280x800_60, 1280, 800},
    {VO_OUTPUT_1600x1200_60, 1600, 1200},
    {VO_OUTPUT_1680x1050_60, 1680, 1050},
    {VO_OUTPUT_1920x1200_60, 1920, 1200},
};
static void Sample_VO_GetDisplaySize(RK_S32 enIntfSync, RK_U32 *s32W, RK_U32 *s32H) {
    switch (enIntfSync) {
        case VO_OUTPUT_640x480_60:
            *s32W = 640;
            *s32H = 480;
            break;
        case VO_OUTPUT_PAL:
        case VO_OUTPUT_576P50:
            *s32W = 720;
            *s32H = 576;
            break;
        case VO_OUTPUT_NTSC:
        case VO_OUTPUT_480P60:
            *s32W = 720;
            *s32H = 480;
            break;
        case VO_OUTPUT_800x600_60:
            *s32W = 800;
            *s32H = 600;
            break;
        case VO_OUTPUT_1024x768_60:
            *s32W = 1024;
            *s32H = 768;
            break;
        case VO_OUTPUT_720P50:
        case VO_OUTPUT_720P60:
            *s32W = 1280;
            *s32H = 720;
            break;
        case VO_OUTPUT_1280x1024_60:
            *s32W = 1280;
            *s32H = 1024;
            break;
        case VO_OUTPUT_1080P24:
        case VO_OUTPUT_1080P25:
        case VO_OUTPUT_1080P30:
        case VO_OUTPUT_1080I50:
        case VO_OUTPUT_1080I60:
        case VO_OUTPUT_1080P50:
        case VO_OUTPUT_1080P60:
            *s32W = 1920;
            *s32H = 1080;
            break;
        case VO_OUTPUT_3840x2160_24:
        case VO_OUTPUT_3840x2160_25:
        case VO_OUTPUT_3840x2160_30:
        case VO_OUTPUT_3840x2160_50:
        case VO_OUTPUT_3840x2160_60:
            *s32W = 3840;
            *s32H = 2160;
            break;
        case VO_OUTPUT_4096x2160_24:
        case VO_OUTPUT_4096x2160_25:
        case VO_OUTPUT_4096x2160_30:
        case VO_OUTPUT_4096x2160_50:
        case VO_OUTPUT_4096x2160_60:
            *s32W = 4096;
            *s32H = 2160;
            break;
        default:
            for (RK_S32 i = 0; i < ARRAY_LENGTH(stVoTimings); i++) {
                if (stVoTimings[i].enIntfSync == enIntfSync) {
                    *s32W = stVoTimings[i].u32Width;
                    *s32H = stVoTimings[i].u32Height;
                    break;
                }
            }
            break;
    }
}

#define Sample_Print(format, ...)       RK_LOGI(format, ##__VA_ARGS__)

typedef struct _rkMpiVOCtx {
    RK_S32  VoDev;
    RK_S32  VoLayer;
    RK_S32  VoLayerMode;

    RK_S32  u32Windows;
    RK_U32  enIntfType; /* 0 HDMI 1 edp */
    RK_U32  enIntfSync; /* VO_OUTPUT_1080P50 */
    RK_U32  enIntfSync_second;
    RK_U32  s32X;
    RK_U32  s32Y;

    RK_U32  u32Screen0DisplayWidth;
    RK_U32  u32Screen0DisplayHeight;
    RK_U32  u32Screen1DisplayWidth;
    RK_U32  u32Screen1DisplayHeight;

    RK_U32  u32Screen0ImageWidth;
    RK_U32  u32Screen0ImageHeight;
    RK_U32  u32Screen1ImageWidth;
    RK_U32  u32Screen1ImageHeight;

    RK_U32  bDoubleScreen;
    RK_U32  u32DispWidth;
    RK_U32  u32DispHeight;
    RK_U32  u32ImgeWidth;
    RK_U32  u32ImageHeight;
    RK_U32  s32PixFormat;
    RK_U32  u32DispFrmRt;
    RK_U32  u32DispFrmRtRatio;
    RK_U32  uEnMode;
    RK_BOOL bBorder;
    RK_S32  loopCount;

    RK_U32  wbc_enable;
    RK_BOOL wbc_auto;
    RK_U32  ui;
    RK_U32  ui_alpha;
    RK_U32  u32WbcWidth;
    RK_S32  u32WbcHeight;
    COMPRESS_MODE_E  u32WbcCompressMode;
    RK_S32  u32WbcPixFormat;
    RK_U32  u32WbcSourceType;
    RK_U32  u32WbcSourceId;

    VO_LAYER   VoVideoLayer;
    RK_U32     u32VideoWindows;
    RK_U32     u32GFXLayers;
    VO_LAYER   VOGfxLayer;
    RK_BOOL    threadExit;

    RK_BOOL     bVoPlay;
    const char *cfgFileUri;
    RK_S32      s32LoopCount;
    RK_U32      u32ChnIndex;
    RK_U32      u32Screen0Chn;
    RK_U32      u32ChnDismode;
    RK_U32      u32Screen1Chn;
    RK_BOOL     bThreadExit;

    RK_BOOL     bEnWbc;
    RK_BOOL     bEnWbcToVO;
    RK_BOOL     bChnPriority;

    PARSER_CFG  stParserCfg;
    VDEC_CFG    stVdecCfg;
    VO_CFG      stVoCfg;
    WBC_CFG     stWbcCfg;
    RK_BOOL     wbc_bind;
    RK_U32      bHomologous;
    Border_CFG  stBorderCfg;
    MIRROR_E enMirror;
    ROTATION_E enRotation;
    RK_BOOL bUseRga;
} TEST_VO_CTX_S;

typedef struct _VO_Send_Thread_Param {
    RK_U32 u32VideoWindows;
    VO_LAYER VoVideoLayer;
    RK_U32 u32GFXLayers;
    VO_LAYER VOGfxLayer;
    RK_U32 u32Exit;
} VoThreadParam;

static RK_S32 SampleSetVoInterfacetiming(VO_PUB_ATTR_S *stVoPubAttr, RK_U32 enIntfSync) {
    RK_U32 i;
    RK_S32 s32Ret = RK_SUCCESS;

    for (i = 0; i < ARRAY_LENGTH(test_mode_table); i++) {
        if (enIntfSync == test_mode_table[i].mode)
            break;
    }

    if (i == ARRAY_LENGTH(test_mode_table)) {
        Sample_Print("%s not found supported mode in test mode_table\n", __func__);
        return RK_FAILURE;
     }

    stVoPubAttr->enIntfSync = test_mode_table[i].enIntfSync;

    return s32Ret;
}
static PIXEL_FORMAT_E Sample_wbc_formt(RK_S32 format) {
    switch (format) {
        case WBC_FORMAT_BGRA8888:
            return RK_FMT_BGRA8888;
        case WBC_FORMAT_RGBA8888:
            return RK_FMT_RGBA8888;
        case WBC_FORMAT_RGB888:
            return RK_FMT_RGB888;
        case WBC_FORMAT_BGR888:
            return RK_FMT_BGR888;
        case WBC_FORMAT_YUV420SP:
            return RK_FMT_YUV420SP;
        case WBC_FORMAT_YUV420SP_VU:
            return RK_FMT_YUV420SP_VU;
        default:
            return RK_FMT_BUTT;
    }
}

static void* vochn_test_thread_func(void *pArgs) {
    TEST_VO_CTX_S *ctx = reinterpret_cast<TEST_VO_CTX_S *>(pArgs);
    RK_S32 i, j;

    while (1) {
        if (ctx->u32ChnDismode == VO_CHANNEL_PLAY_PAUSE) {
            for (i = 0; i < ctx->u32Screen0Chn; i++) {
                RK_MPI_VO_PauseChn(VoLayer, i);
            }

            usleep(1000llu * 2000);

            for (i = 0; i < ctx->u32Screen0Chn; i++) {
                RK_MPI_VO_ResumeChn(VoLayer, i);
            }
        } else if (ctx->u32ChnDismode == VO_CHANNEL_PLAY_STEP) {
            for (i = 0; i < ctx->u32Screen0Chn; i++) {
                for (RK_S32 j = 0; j < MAX_STEP_FRAME_NUM; j++) {
                    RK_MPI_VO_StepChn(VoLayer, i);
                }
            }

            usleep(1000llu * 2000);

            for (i = 0; i < ctx->u32Screen0Chn; i++) {
                RK_MPI_VO_ResumeChn(VoLayer, i);
            }
        } else if (ctx->u32ChnDismode == VO_CHANNEL_PLAY_SPEED) {
            for (i = 0; i < ctx->u32Screen0Chn; i++) {
                RK_MPI_VO_SetChnFrameRate(VoLayer, i, ctx->u32DispFrmRt * ctx->u32DispFrmRtRatio);
            }

            usleep(1000llu * 2000);

            for (i = 0; i < ctx->u32Screen0Chn; i++) {
                RK_MPI_VO_ResumeChn(VoLayer, i);
            }
        }

        if (ctx->stVoCfg.bDoubleScreen) {
            if (ctx->u32ChnDismode == VO_CHANNEL_PLAY_PAUSE) {
                for (i = 0; i < ctx->u32Screen1Chn; i++) {
                    RK_MPI_VO_PauseChn(VoLayer_second, i);
                }
                usleep(1000llu * 2000);
                for (i = 0; i < ctx->u32Screen1Chn; i++) {
                    RK_MPI_VO_ResumeChn(VoLayer_second, i);
                }
            } else if (ctx->u32ChnDismode == VO_CHANNEL_PLAY_STEP) {
                for (i = 0; i < ctx->u32Screen1Chn; i++) {
                    for (RK_S32 j = 0; j < MAX_STEP_FRAME_NUM; j++) {
                        RK_MPI_VO_StepChn(VoLayer_second, i);
                    }
                }

                usleep(1000llu * 2000);

                for (i = 0; i < ctx->u32Screen1Chn; i++) {
                    RK_MPI_VO_ResumeChn(VoLayer_second, i);
                }
           } else if (ctx->u32ChnDismode == VO_CHANNEL_PLAY_SPEED) {
                for (i = 0; i < ctx->u32Screen1Chn; i++) {
                    RK_MPI_VO_SetChnFrameRate(VoLayer_second, i, ctx->u32DispFrmRt * ctx->u32DispFrmRtRatio);
                }

                usleep(1000llu * 2000);

                for (i = 0; i < ctx->u32Screen1Chn; i++) {
                    RK_MPI_VO_ResumeChn(VoLayer_second, i);
                }
            }
        }
    }

    return RK_NULL;
}

static RK_S32 mpi_vo_init_sample(const TEST_VO_CTX_S *ctx, RK_S32 primRows , RK_S32 secondRows) {
    VO_PUB_ATTR_S VoPubAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VO_CSC_S  VideoCSC;
    VO_CHN_ATTR_S VoChnAttr[64];
    VO_BORDER_S border;
    RK_S32  i;
    RK_U32 count = 0;
    RK_U32 u32DispBufLen;
    RK_U32 s32Ret;
    RK_U32 u32DispWidth = ctx->u32DispWidth;
    RK_U32 u32DispHeight = ctx->u32DispHeight;
    RK_U32 u32ImageWidth = u32DispWidth;
    RK_U32 u32ImageHeight = u32DispHeight;

    memset(&VoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
    memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
    memset(&VideoCSC, 0, sizeof(VO_CSC_S));
    memset(&VoChnAttr, 0, sizeof(VoChnAttr));
    memset(&border, 0, sizeof(VO_BORDER_S));

    VoPubAttr.enIntfType = VO_INTF_HDMI;
    VoPubAttr.enIntfSync = VO_OUTPUT_1080P60;

    s32Ret = RK_MPI_VO_SetPubAttr(RK356X_VO_DEV_HD0, &VoPubAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VO_Enable(RK356X_VO_DEV_HD0);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    if (ctx->stVoCfg.bDoubleScreen) {
        VO_PUB_ATTR_S VoPubAttr1;
        VoPubAttr1.enIntfType = VO_INTF_EDP;
        VoPubAttr1.enIntfSync = VO_OUTPUT_1024x768_60;
        VoPubAttr1.u32BgColor = 0x000000;
        s32Ret = RK_MPI_VO_SetPubAttr(RK356X_VO_DEV_HD1, &VoPubAttr1);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VO_Enable(RK356X_VO_DEV_HD1);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }

        s32Ret = RK_MPI_VO_GetLayerDispBufLen(VoLayer_second, &u32DispBufLen);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("Get display buf len failed with error code %d!\n", s32Ret);
            return s32Ret;
        }
        u32DispBufLen = 4;
        s32Ret = RK_MPI_VO_SetLayerDispBufLen(VoLayer_second, u32DispBufLen);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VO_GetLayerDispBufLen(VoLayer, &u32DispBufLen);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("Get display buf len failed with error code %d!\n", s32Ret);
        return s32Ret;
    }
    u32DispBufLen = 4;
    s32Ret = RK_MPI_VO_SetLayerDispBufLen(VoLayer, u32DispBufLen);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    Sample_VO_GetDisplaySize(VoPubAttr.enIntfSync, &u32DispWidth, &u32DispHeight);
    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    stLayerAttr.stDispRect.u32Width = u32DispWidth;
    stLayerAttr.stDispRect.u32Height = u32DispHeight;
    stLayerAttr.stImageSize.u32Width = u32ImageWidth;
    stLayerAttr.stImageSize.u32Height = u32ImageHeight;

    stLayerAttr.u32DispFrmRt = 30;
    if (VoLayer == RK356X_VOP_LAYER_ESMART_0) {
        stLayerAttr.enPixFormat = RK_FMT_YUV420SP;
    } else {
       stLayerAttr.enPixFormat = RK_FMT_RGBA8888;
    }
    stLayerAttr.bDoubleFrame = RK_TRUE;
    VideoCSC.enCscMatrix = VO_CSC_MATRIX_IDENTITY;
    VideoCSC.u32Contrast =  50;
    VideoCSC.u32Hue = 50;
    VideoCSC.u32Luma = 50;
    VideoCSC.u32Satuature = 50;

    s32Ret = RK_MPI_VO_SetLayerAttr(VoLayer, &stLayerAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VO_EnableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VO_SetLayerCSC(VoLayer, &VideoCSC);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    RK_S32 totalCH = primRows * primRows;
    for (i = 0; i < totalCH; i++) {
        VoChnAttr[i].bDeflicker = RK_FALSE;
        VoChnAttr[i].u32Priority = 1;
        VoChnAttr[i].stRect.s32X = (u32ImageWidth/primRows)*(i%primRows);
        VoChnAttr[i].stRect.s32Y = (u32ImageHeight/primRows)*(i/primRows);
        VoChnAttr[i].stRect.u32Width = u32ImageWidth/primRows;
        VoChnAttr[i].stRect.u32Width = VoChnAttr[i].stRect.u32Width;
        VoChnAttr[i].stRect.u32Height = u32ImageHeight/primRows;
        VoChnAttr[i].stRect.u32Height = VoChnAttr[i].stRect.u32Height;
    }

    for (i = 0; i < totalCH; i++) {
        RK_S32 s32ret = RK_MPI_VO_SetChnAttr(VoLayer, i, &VoChnAttr[i]);
        if (RK_SUCCESS != s32ret) {
            RK_LOGE("vo set dev %d chn %d attr failed! \n", VoLayer, i);
        }
        border.bBorderEn = RK_TRUE;
        border.stBorder.u32Color = 0x191970;
        border.stBorder.u32LeftWidth = 2;
        border.stBorder.u32RightWidth = 2;
        border.stBorder.u32TopWidth = 2;
        border.stBorder.u32BottomWidth = 2;
        s32Ret = RK_MPI_VO_SetChnBorder(VoLayer, i, &border);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    if (ctx->stVoCfg.bDoubleScreen) {
        u32DispWidth = ctx->u32Screen1DisplayWidth;
        u32DispHeight = ctx->u32Screen1DisplayHeight;
        u32ImageWidth = u32DispWidth;
        u32ImageHeight = u32DispHeight;
        Sample_VO_GetDisplaySize(VO_OUTPUT_1024x768_60, &u32DispWidth, &u32DispHeight);
        stLayerAttr.stDispRect.s32X = 0;
        stLayerAttr.stDispRect.s32Y = 0;
        stLayerAttr.stDispRect.u32Width = u32DispWidth;
        stLayerAttr.stDispRect.u32Height = u32DispHeight;
        stLayerAttr.stImageSize.u32Width = u32ImageWidth;
        stLayerAttr.stImageSize.u32Height = u32ImageHeight;

        stLayerAttr.u32DispFrmRt = 30;
        if (VoLayer_second == RK356X_VOP_LAYER_ESMART_1) {
            stLayerAttr.enPixFormat = RK_FMT_YUV420SP;
        } else {
           stLayerAttr.enPixFormat = RK_FMT_BGR888;
        }
        stLayerAttr.bDoubleFrame = RK_TRUE;

        s32Ret = RK_MPI_VO_SetLayerAttr(VoLayer_second, &stLayerAttr);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VO_EnableLayer(VoLayer_second);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
        s32Ret = RK_MPI_VO_SetLayerCSC(VoLayer_second, &VideoCSC);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }

        totalCH = secondRows * secondRows;
        for (i = 0; i < totalCH; i++) {
            VoChnAttr[i].bDeflicker = RK_FALSE;
            VoChnAttr[i].u32Priority = 1;
            VoChnAttr[i].stRect.s32X = (u32ImageWidth/secondRows)*(i%secondRows);
            VoChnAttr[i].stRect.s32Y = (u32ImageHeight/secondRows)*(i/secondRows);
            VoChnAttr[i].stRect.u32Width = u32ImageWidth/secondRows;
            VoChnAttr[i].stRect.u32Width = VoChnAttr[i].stRect.u32Width;
            VoChnAttr[i].stRect.u32Height = u32ImageHeight/secondRows;
            VoChnAttr[i].stRect.u32Height = VoChnAttr[i].stRect.u32Height;
        }
        /* demo Chn Priority test */
        if (ctx->bChnPriority) {
            VoChnAttr[0].u32Priority = 15;

            VoChnAttr[1].u32Priority = 10;
            VoChnAttr[1].stRect.s32X = (u32ImageWidth/secondRows)*(1%secondRows) -  100;
            VoChnAttr[1].stRect.s32Y = (u32ImageHeight/secondRows)*(1/secondRows) + 100;
            VoChnAttr[1].stRect.u32Width = u32ImageWidth/secondRows + 200;
            VoChnAttr[1].stRect.u32Width = VoChnAttr[1].stRect.u32Width;
            VoChnAttr[1].stRect.u32Height = u32ImageWidth/secondRows + 200;
            VoChnAttr[1].stRect.u32Height = VoChnAttr[1].stRect.u32Height;

            VoChnAttr[2].u32Priority = 5;
        }

        for (i = 0; i < totalCH; i++) {
            RK_S32 s32ret = RK_MPI_VO_SetChnAttr(VoLayer_second, i, &VoChnAttr[i]);
            if (RK_SUCCESS != s32ret) {
                RK_LOGE("vo set dev %d chn %d attr failed! \n", VoLayer_second, i);
            }
            border.bBorderEn = RK_TRUE;
            border.stBorder.u32Color = 0x191970;
            border.stBorder.u32LeftWidth = 2;
            border.stBorder.u32RightWidth = 2;
            border.stBorder.u32TopWidth = 2;
            border.stBorder.u32BottomWidth = 2;
            s32Ret = RK_MPI_VO_SetChnBorder(VoLayer_second, i, &border);
            if (s32Ret != RK_SUCCESS) {
                return s32Ret;
            }
        }
    }

    return RK_SUCCESS;
}


static RK_S32 mpi_vo_deinit_sample(const TEST_VO_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VO_DisableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    if (ctx->stVoCfg.bDoubleScreen) {
        s32Ret = RK_MPI_VO_DisableLayer(VoLayer_second);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    s32Ret = RK_MPI_VO_Disable(RK356X_VO_DEV_HD0);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    if (ctx->stVoCfg.bDoubleScreen) {
        s32Ret = RK_MPI_VO_Disable(RK356X_VO_DEV_HD1);
        if (s32Ret != RK_SUCCESS) {
            return s32Ret;
        }
    }

    return s32Ret;
}

static RK_S32 open_config(TEST_VO_CTX_S *ctx) {
    FILE *fp = RK_NULL;
    RK_U32 u32Count = 0;
    RK_U32 u32FileLen = 0;
    RK_U32 u32Len = 0;
    RK_S32 s32Ret = 0;
    char *line = RK_NULL;
    char *pBuffer = RK_NULL;

    if ((fp = fopen(ctx->cfgFileUri, "r")) == RK_NULL) {
        RK_LOGE("Error! opening file");
        return RK_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    u32FileLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    size_t sLen = 0;
    size_t read = 0;
    while (read = getline(&line, &sLen, fp) != -1) {
        pBuffer = reinterpret_cast<char *>(malloc(strlen(line)));
        RK_S32 len = strlen(line);
        snprintf(pBuffer, strlen(line), "%s", line);
        while (len) {
            if (pBuffer[len] == '\r') {
                memcpy(&(pBuffer[len]), &(pBuffer[len + 1]), strlen(line) - len);
            }
            len--;
        }
        ctx->stParserCfg.srcFileUri[u32Count] = pBuffer;

        RK_LOGI("url %s", ctx->stParserCfg.srcFileUri[u32Count]);
        u32Count++;

        if (u32Count >= VDEC_MAX_CHN_NUM)
            break;
    }

    fclose(fp);
    return RK_SUCCESS;
}

static RK_S32 close_config(TEST_VO_CTX_S *ctx) {
    RK_S32 i;

    for (i = 0; i < VDEC_MAX_CHN_NUM; i++) {
        if (ctx->stParserCfg.srcFileUri[i] != RK_NULL) {
            rk_safe_free(ctx->stParserCfg.srcFileUri[i]);
        }
    }
    return RK_SUCCESS;
}

static RK_S32 creat_wbc(TEST_VO_CTX_S *ctx, RK_U32 u32Ch) {
    RK_S32 s32Ret = RK_SUCCESS;
    VO_WBC VoWbc = u32Ch;

    switch (ctx->u32WbcSourceType) {
        case WBC_SOURCE_DEV:
            ctx->stWbcCfg.stWbcSource.enSourceType = VO_WBC_SOURCE_DEV;
            break;
        default:
            ctx->stWbcCfg.stWbcSource.enSourceType = VO_WBC_SOURCE_VIDEO;
    }
    s32Ret = RK_MPI_VO_SetWbcSource(VoWbc, &ctx->stWbcCfg.stWbcSource);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VO_SetWbcAttr(VoWbc, &ctx->stWbcCfg.stWbcAttr);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VO_EnableWbc(VoWbc);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    return RK_SUCCESS;
}

static RK_S32 destory_wbc(RK_U32 u32Ch) {
    VO_WBC VoWbc = u32Ch;
    RK_MPI_VO_DisableWbc(VoWbc);

    return RK_SUCCESS;
}

static RK_S32 creat_vdec(TEST_VO_CTX_S *ctx, RK_U32 u32Ch) {
    RK_S32 s32Ret = RK_SUCCESS;
    VDEC_CHN_ATTR_S pstAttr;
    VDEC_CHN_PARAM_S stVdecParam;

    pstAttr.enMode = VIDEO_MODE_FRAME;
    pstAttr.enType = ctx->stParserCfg.enCodecId;
    pstAttr.u32PicWidth = ctx->stParserCfg.u32SrcWidth;
    pstAttr.u32PicHeight = ctx->stParserCfg.u32SrcHeight;
    pstAttr.u32FrameBufCnt = ctx->stVdecCfg.u32FrameBufferCnt;

    s32Ret = RK_MPI_VDEC_CreateChn(u32Ch, &pstAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create %d vdec failed! ", u32Ch);
        RK_LOGE(R"(enMode %d enType %d u32PicWidth %d u32PicHeight %d \
            u32PicVirWidth %d u32PicVirHeight %d u32FrameBufCnt %d)", \
            pstAttr.enMode, pstAttr.enType, pstAttr.u32PicWidth, pstAttr.u32PicHeight, \
            pstAttr.u32PicVirWidth, pstAttr.u32PicVirHeight, pstAttr.u32FrameBufCnt);
        return s32Ret;
    }

    stVdecParam.stVdecVideoParam.enCompressMode = ctx->stVdecCfg.enCompressMode;
    s32Ret = RK_MPI_VDEC_SetChnParam(u32Ch, &stVdecParam);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    s32Ret = RK_MPI_VDEC_StartRecvStream(u32Ch);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    if (TEST_COMM_GetUriSchemeType(ctx->stParserCfg.srcFileUri[u32Ch]) != RK_URI_SCHEME_LOCAL) {
        s32Ret = RK_MPI_VDEC_SetDisplayMode(u32Ch, VIDEO_DISPLAY_MODE_PREVIEW);
    } else {
        s32Ret = RK_MPI_VDEC_SetDisplayMode(u32Ch, VIDEO_DISPLAY_MODE_PLAYBACK);
    }
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    RK_LOGI("create %d vdec success", u32Ch);
    return RK_SUCCESS;
}

static RK_S32 screen1_process(TEST_VO_CTX_S *ctx, RK_U32 u32Index) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn, stDstChn;

    s32Ret = creat_vdec(ctx, u32Index);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("creat %d ch vdec failed!", u32Index);
        return s32Ret;
    }

    s32Ret = RK_MPI_VO_EnableChn(ctx->stVoCfg.u32Screen1VoLayer, u32Index - ctx->u32Screen0Chn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("creat %d layer %d ch vo failed!",
            ctx->stVoCfg.u32Screen1VoLayer, u32Index - ctx->u32Screen0Chn);
        return s32Ret;
    }
    RK_LOGD("create vo layer %d ch %d", ctx->stVoCfg.u32Screen1VoLayer,  u32Index - ctx->u32Screen0Chn);

    stSrcChn.enModId = RK_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = u32Index;

    RK_LOGD("vdec  Ch %d", stSrcChn.s32ChnId);
    stDstChn.enModId = RK_ID_VO;
    stDstChn.s32DevId = ctx->stVoCfg.u32Screen1VoLayer;
    stDstChn.s32ChnId = u32Index - ctx->u32Screen0Chn;

    RK_LOGD("VoLayer %d, voCh %d", ctx->stVoCfg.u32Screen1VoLayer,  u32Index - ctx->u32Screen0Chn);

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDstChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vdec and vo bind error");
        return s32Ret;
    }

    return RK_SUCCESS;
}

static RK_S32 screen0_process(TEST_VO_CTX_S *ctx, RK_U32 u32Index) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn, stDstChn;

    s32Ret = creat_vdec(ctx, u32Index);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("creat %d ch vdec failed!", u32Index);
        return s32Ret;
    }

    s32Ret = RK_MPI_VO_EnableChn(ctx->stVoCfg.u32Screen0VoLayer, u32Index);
    if (s32Ret != RK_SUCCESS) {
         RK_LOGE("creat %d layer %d ch vo failed!", ctx->stVoCfg.u32Screen0VoLayer, u32Index);
         return s32Ret;
    }

    RK_LOGD("create vo layer %d ch %d", ctx->stVoCfg.u32Screen0VoLayer, u32Index);

    stSrcChn.enModId = RK_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = u32Index;
    RK_LOGD("vdec  Ch %d", stSrcChn.s32ChnId);

    stDstChn.enModId = RK_ID_VO;
    stDstChn.s32DevId = ctx->stVoCfg.u32Screen0VoLayer;
    stDstChn.s32ChnId = u32Index;
    RK_LOGD("voLayer %d, voCh %d", stDstChn.s32DevId,  stDstChn.s32ChnId);

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDstChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("vdec and vo bind error ");
        return s32Ret;
    }

    return RK_SUCCESS;
}

static RK_S32 wbc_process(TEST_VO_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn, stDstChn;

    s32Ret = creat_wbc(ctx, 0);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create wbc failed");
        return s32Ret;
    }

    stSrcChn.enModId = RK_ID_WBC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    if (ctx->bEnWbcToVO) {
        stDstChn.enModId   = RK_ID_VO;

        if (ctx->stWbcCfg.stWbcSource.u32SourceId == RK356X_VO_DEV_HD1) {
            s32Ret = RK_MPI_VO_EnableChn(ctx->stVoCfg.u32Screen0VoLayer, ctx->u32Screen0Chn);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("creat %d layer %d ch vo failed!",
                    ctx->stVoCfg.u32Screen0VoLayer, ctx->u32Screen0Chn);
                return s32Ret;
            }
            stDstChn.s32DevId  = ctx->stVoCfg.u32Screen0VoLayer;
        } else {
            s32Ret = RK_MPI_VO_EnableChn(ctx->stVoCfg.u32Screen1VoLayer, ctx->u32Screen1Chn);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("creat %d layer %d ch vo failed!",
                    ctx->stVoCfg.u32Screen1VoLayer, ctx->u32Screen1Chn);
                return s32Ret;
            }
            stDstChn.s32DevId  = ctx->stVoCfg.u32Screen1VoLayer;
        }
        stDstChn.s32ChnId  = ctx->u32Screen1Chn;

        if (ctx->wbc_bind == RK_TRUE) {
            s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDstChn);
            if (s32Ret != RK_SUCCESS) {
                RK_LOGE("creat wbc bind vo failed!");
                return RK_FAILURE;
            }
        } else {
            RK_LOGD("WBC enabled,but not bind");
        }
    }

    return RK_SUCCESS;
}

static void Sample_VO_GetPictureData(VO_FRAME_INFO_S *pstFrameInfo, const char *pFileName) {
    RK_S32 u32Fd;
    RK_U32 u32Size, u32Bpp, u32Stride;

    switch (pstFrameInfo->enPixelFormat) {
    case RK_FMT_RGBA8888:
        u32Bpp = 4;
        break;
    case RK_FMT_RGB888:
    case RK_FMT_BGR888:
        u32Bpp = 3;
        break;
    case RK_FMT_YUV420SP:
    case RK_FMT_YUV420SP_VU:
        u32Bpp = 1;
        break;
    default:
        Sample_Print("%s Unsupported Format %d\n", __func__, pstFrameInfo->enPixelFormat);
    return;
    }

    u32Fd = open(pFileName, O_RDONLY);
    if (u32Fd < 0) {
        Sample_Print("cat not find file\n");
        return;
    }
    u32Size = lseek(u32Fd, 0, SEEK_END);
    lseek(u32Fd, 0, SEEK_SET);

    Sample_Print("%dx%d %dx%d\n",
                  pstFrameInfo->u32Width, pstFrameInfo->u32Height,
                  pstFrameInfo->u32VirWidth, pstFrameInfo->u32VirHeight);

    if (pstFrameInfo->u32Width != pstFrameInfo->u32VirWidth) {
        u32Stride = pstFrameInfo->u32VirWidth * u32Bpp;

        for (RK_S32 i = 0; i < pstFrameInfo->u32Height; i++)
            read(u32Fd, reinterpret_cast<RK_S8 *>(pstFrameInfo->pData) + u32Stride * i,
                        pstFrameInfo->u32Width * u32Bpp);

            if (pstFrameInfo->enPixelFormat == RK_FMT_YUV420SP ||
                pstFrameInfo->enPixelFormat == RK_FMT_YUV420SP_VU) {
                for (RK_S32 i = 0; i < pstFrameInfo->u32Height / 2; i++)
                    read(u32Fd, reinterpret_cast<RK_S8 *>(pstFrameInfo->pData)
                                + u32Stride * pstFrameInfo->u32VirHeight + i * u32Stride,
                                pstFrameInfo->u32Width);
                }
            } else {
                read(u32Fd, pstFrameInfo->pData, u32Size);
            }
            close(u32Fd);
}

static void filewrite(void *buf, uint32_t size, char *file) {
    int fd = open(file, O_WRONLY | O_CREAT);

    if (fd < 0) {
        printf("cat not open file\n");
        return;
    }

    write(fd, buf, size);
    close(fd);
}

static void Sample_VO_DrawUI(VO_FRAME_INFO_S *pstFrameInfo, RK_S32 format, RK_S32 value) {
    RK_S32 PixelVal0, PixelVal1;
    RK_S8 *pData = reinterpret_cast<RK_S8 *>(pstFrameInfo->pData);
    RK_U32 u32Offset;

    for (RK_S32 i = 0; i < pstFrameInfo->u32Height; i++) {
        for (RK_S32 j = 0; j < pstFrameInfo->u32Width; j++) {
            u32Offset = i * pstFrameInfo->u32VirWidth + j;
            if (format == RK_FMT_BGRA5551) {
                u32Offset = u32Offset * 2;
                PixelVal0 = value | 0x8000;
                PixelVal1 = value & 0x7FFF;
            } else if (format == RK_FMT_BGRA8888) {
                u32Offset = u32Offset * 4;
                PixelVal0 = 0x000000FF;
            } else {
                continue;
            }

            if (j < pstFrameInfo->u32Width / 2) {
                if (format == RK_FMT_BGRA5551) {
                    pData[u32Offset] = PixelVal0 & 0xff;
                    pData[u32Offset + 1] = (PixelVal0 >> 8) & 0xff;
                } else if (format == RK_FMT_BGRA8888) {
                    pData[u32Offset] = PixelVal0 & 0xff;
                    pData[u32Offset + 1] = (PixelVal0 >> 8) & 0xff;
                    pData[u32Offset + 1] = (PixelVal0 >> 16) & 0xff;
                    pData[u32Offset + 1] = (PixelVal0 >> 24) & 0xff;
                }
            } else {
                if (format == RK_FMT_BGRA5551) {
                    pData[u32Offset] = PixelVal1 & 0xff;
                    pData[u32Offset + 1] = (PixelVal1 >> 8) & 0xff;
                } else if (format == RK_FMT_BGRA8888) {
                    pData[u32Offset] = PixelVal0 & 0xff;
                    pData[u32Offset + 1] = (PixelVal0 >> 8) & 0xff;
                    pData[u32Offset + 1] = (PixelVal0 >> 16) & 0xff;
                    pData[u32Offset + 1] = (PixelVal0 >> 24) & 0xff;
               }
            }
        }
    }
}

static RK_S32 Sample_VO_MultiGFXLayer_Start(VO_LAYER VoLayer, RK_U32 u32Layers) {
    VO_VIDEO_LAYER_ATTR_S    stLayerAttr;
    VO_CHN_ATTR_S            stChnAttr;
    VO_CHN_PARAM_S           stParam;
    VO_BORDER_S              stBorder;
    RK_U32                   u32Row, u32Column, i;
    RK_U32                   u32WinWidth, u32WinHeight;
    RK_S32                   s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VO_GetLayerAttr(VoLayer, &stLayerAttr);
    if (s32Ret != RK_SUCCESS) {
        Sample_Print("[%s] Get Layer Attr failed\n", __func__);
        return RK_FAILURE;
    }

    for (i = 0; i < u32Layers; i++) {
        stChnAttr.stRect.s32X = 0;
        stChnAttr.stRect.s32Y = 0;
        stChnAttr.stRect.u32Width = stLayerAttr.stDispRect.u32Width;
        stChnAttr.stRect.u32Height = stLayerAttr.stDispRect.u32Height;
        stChnAttr.u32Priority = i;
        if (i == 0) {
            stChnAttr.u32FgAlpha = 128;
            stChnAttr.u32BgAlpha = 0;
        } else {
            stChnAttr.u32FgAlpha = 0;
            stChnAttr.u32BgAlpha = 128;
        }

        s32Ret = RK_MPI_VO_SetChnAttr(VoLayer, i, &stChnAttr);
        if (s32Ret != RK_SUCCESS) {
            Sample_Print("[%s] set chn Attr failed\n", __func__);
            return RK_FAILURE;
        }

        s32Ret = RK_MPI_VO_EnableChn(VoLayer, i);
        if (s32Ret != RK_SUCCESS) {
            Sample_Print("[%s] Enalbe chn failed\n", __func__);
            return RK_FAILURE;
        }
    }

    Sample_Print("[%s] success\n", __func__);
    return s32Ret;
}

static RK_S32 Sample_VO_MultiWindowns_Start(TEST_VO_CTX_S *ctx) {
    VO_VIDEO_LAYER_ATTR_S    stLayerAttr;
    VO_CHN_ATTR_S            stChnAttr;
    VO_CHN_PARAM_S           stChnParam;
    VO_BORDER_S              stBorder;
    RK_U32                   u32Row, u32Column, i;
    RK_U32                   u32WinWidth, u32WinHeight;
    RK_U32                   u32X, u32Y;
    RK_S32                   s32Ret = RK_SUCCESS;
    RK_U32 disp_width = 1920;
    RK_U32 disp_height = 1080;

    if (ctx->u32Windows <= 2)
        u32Row = 1;
    else if (ctx->u32Windows <= 4)
        u32Row = 2;
    else if (ctx->u32Windows <= 9)
        u32Row = 3;
    else if (ctx->u32Windows <= 16)
        u32Row = 4;
    else if (ctx->u32Windows <= 36)
        u32Row = 6;
    else if (ctx->u32Windows <= 64)
        u32Row = 8;
    else
        return RK_FAILURE;

    u32Column = ctx->u32Windows / u32Row + ((ctx->u32Windows % u32Row)? 1: 0);

    s32Ret = RK_MPI_VO_GetLayerAttr(ctx->VoLayer, &stLayerAttr);

    if (s32Ret != RK_SUCCESS)
        return RK_FAILURE;

    u32WinWidth = stLayerAttr.stDispRect.u32Width / u32Column;
    u32WinHeight = stLayerAttr.stDispRect.u32Height / u32Row;

    for (i = 0; i <ctx->u32Windows; i++) {
        u32X = i % u32Column;
        u32Y = i / u32Column;
        stChnAttr.stRect.s32X = stLayerAttr.stDispRect.s32X + u32X * u32WinWidth;
        stChnAttr.stRect.s32Y = stLayerAttr.stDispRect.s32Y + u32Y * u32WinHeight;
        stChnAttr.stRect.u32Width = u32WinWidth;
        stChnAttr.stRect.u32Height = u32WinHeight;
        stChnAttr.u32FgAlpha = 255;
        stChnAttr.u32BgAlpha = 0;
        stChnAttr.enMirror = ctx->enMirror;
        stChnAttr.enRotation = ctx->enRotation;
        // set priority
        stChnAttr.u32Priority = 1;

        s32Ret = RK_MPI_VO_SetChnAttr(ctx->VoLayer, i, &stChnAttr);
        if (s32Ret != RK_SUCCESS)
            return RK_FAILURE;

        // set video aspect ratio
        if (ctx->uEnMode == 2) {
            stChnParam.stAspectRatio.enMode = ASPECT_RATIO_MANUAL;
            stChnParam.stAspectRatio.stVideoRect.s32X = stLayerAttr.stDispRect.s32X + u32X * u32WinWidth;
            stChnParam.stAspectRatio.stVideoRect.s32Y = stLayerAttr.stDispRect.s32Y + u32Y * u32WinHeight;
            stChnParam.stAspectRatio.stVideoRect.u32Width = u32WinWidth/2;
            stChnParam.stAspectRatio.stVideoRect.u32Height = u32WinHeight/2;
            RK_MPI_VO_SetChnParam(ctx->VoLayer, i, &stChnParam);
        } else if (ctx->uEnMode == 1) {
            stChnParam.stAspectRatio.enMode = ASPECT_RATIO_AUTO;
            stChnParam.stAspectRatio.stVideoRect.s32X = stLayerAttr.stDispRect.s32X + u32X * u32WinWidth;
            stChnParam.stAspectRatio.stVideoRect.s32Y = stLayerAttr.stDispRect.s32Y + u32Y * u32WinHeight;
            stChnParam.stAspectRatio.stVideoRect.u32Width = u32WinWidth;
            stChnParam.stAspectRatio.stVideoRect.u32Height = u32WinHeight;
            RK_MPI_VO_SetChnParam(ctx->VoLayer, i, &stChnParam);
        }

        stBorder.stBorder.u32Color = 0xFFFAFA;
        stBorder.stBorder.u32TopWidth = ctx->stBorderCfg.u32TopWidth;
        stBorder.stBorder.u32BottomWidth = ctx->stBorderCfg.u32BottomWidth;
        stBorder.stBorder.u32LeftWidth = ctx->stBorderCfg.u32LeftWidth;
        stBorder.stBorder.u32RightWidth = ctx->stBorderCfg.u32RightWidth;
        stBorder.bBorderEn = ctx->bBorder;
        s32Ret = RK_MPI_VO_SetChnBorder(ctx->VoLayer, i, &stBorder);
        if (s32Ret != RK_SUCCESS)
            return RK_FAILURE;

        s32Ret = RK_MPI_VO_EnableChn(ctx->VoLayer, i);
        if (s32Ret != RK_SUCCESS)
            return RK_FAILURE;
    }

    return s32Ret;
}

static RK_S32 Sample_VO_MultiWindowns_Stop(VO_LAYER VoLayer, RK_U32 u32Windows) {
    RK_U32 i;
    RK_S32 s32Ret = RK_SUCCESS;

    for (i = 0; i < u32Windows; i++) {
        s32Ret = RK_MPI_VO_DisableChn(VoLayer, i);
        if (s32Ret != RK_SUCCESS)
            return RK_FAILURE;
    }

    return s32Ret;
}

static RK_S32 Sample_VO_StartDev(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VO_SetPubAttr(VoDev, pstPubAttr);
    if (s32Ret != RK_SUCCESS)
        return RK_FAILURE;

    s32Ret = RK_MPI_VO_Enable(VoDev);
    if (s32Ret != RK_SUCCESS)
        return RK_FAILURE;

     return s32Ret;
}

static RK_S32 Sample_VO_StopDev(VO_DEV VoDev) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VO_Disable(VoDev);
    if (s32Ret != RK_SUCCESS)
        return RK_FAILURE;

    return s32Ret;
}

static RK_S32 Sample_VO_StartLayer(VO_LAYER VoLayer, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr,
                                RK_BOOL bUseRga) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VO_SetLayerAttr(VoLayer, pstLayerAttr);
    if (s32Ret != RK_SUCCESS) {
        Sample_Print("RK_MPI_VO_SetLayerAttr fail = %p", s32Ret);
        return RK_FAILURE;
    }
    if (bUseRga)
        RK_MPI_VO_SetLayerSpliceMode(VoLayer, VO_SPLICE_MODE_RGA);

    s32Ret = RK_MPI_VO_EnableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS) {
        Sample_Print("RK_MPI_VO_EnableLayer fail = %p", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

static RK_S32 Sample_VO_StopLayer(VO_LAYER VoLayer) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VO_DisableLayer(VoLayer);
    if (s32Ret != RK_SUCCESS)
        return RK_FAILURE;

    return s32Ret;
}

static RK_S32 Sample_VO_SetVFrame(VIDEO_FRAME_INFO_S *pVFrame,
                                   RK_U32              u32Width,
                                   RK_U32              u32Height,
                                   RK_U32              u32VirWidth,
                                   RK_U32              u32VirHeight,
                                   PIXEL_FORMAT_E      enPixelFormat,
                                   RK_VOID             *pMblk) {
    RK_S32 s32Ret = RK_SUCCESS;

    if (pVFrame == NULL && pMblk == NULL)
        return RK_FAILURE;

    if (!u32Width || !u32Height ||
        !u32VirWidth || !u32VirHeight ||
        enPixelFormat >= RK_FMT_BUTT)
        s32Ret = RK_FAILURE;

    pVFrame->stVFrame.pMbBlk = pMblk;
    pVFrame->stVFrame.u32Width = u32Width;
    pVFrame->stVFrame.u32Height = u32Height;
    pVFrame->stVFrame.u32VirWidth = u32VirWidth;
    pVFrame->stVFrame.u32VirHeight = u32VirHeight;
    pVFrame->stVFrame.enPixelFormat = enPixelFormat;

    return s32Ret;
}
static RK_S32 Sample_VO_BindVoWbc(VO_DEV VoWbcDev, VO_LAYER VoLayer, VO_CHN VoChn) {
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId    = RK_ID_WBC;
    stSrcChn.s32DevId   = VoWbcDev;
    stSrcChn.s32ChnId   = 0;

    stDestChn.enModId   = RK_ID_VO;
    stDestChn.s32ChnId  = VoChn;
    stDestChn.s32DevId  = VoLayer;

    return RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

static RK_S32 Sample_VO_UnBindVoWbc(VO_DEV VoWbcDev, VO_LAYER VoLayer, VO_CHN VoChn) {
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId    = RK_ID_WBC;
    stSrcChn.s32DevId   = VoWbcDev;
    stSrcChn.s32ChnId   = 0;

    stDestChn.enModId   = RK_ID_VO;
    stDestChn.s32ChnId  = VoChn;
    stDestChn.s32DevId  = VoLayer;

    return RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
}

static RK_S32 Sample_VO_CreateGFXData(RK_U32 u32Width, RK_U32 u32Height, RK_U32 foramt, RK_U32 value, RK_VOID **pMblk) {
    VO_FRAME_INFO_S stFrameInfo;
    RK_U32 u32BuffSize;

    u32BuffSize = RK_MPI_VO_CreateGraphicsFrameBuffer(u32Width, u32Height, foramt, pMblk);
    if (u32BuffSize == 0) {
        Sample_Print("can not create gfx buffer\n");
        return RK_FAILURE;
    }

    RK_MPI_VO_GetFrameInfo(*pMblk, &stFrameInfo);
    if (foramt == RK_FMT_RGB888) {
        if (u32Width == 1920 && u32Height == 1080 && value == 0)
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_1080P_RGB_0.yuv");
        else if (u32Width == 1920 && u32Height == 1080 && value == 1 )
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_1080P_RGB_1.yuv");
        else if (u32Width == 1024 && u32Height == 768 && value == 0 )
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_768P_RGB_0.yuv");
        else if (u32Width == 1024 && u32Height == 768 && value == 1)
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_768P_RGB_1.yuv");
        else
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_480P_RGB.yuv");
    } else if (foramt == RK_FMT_RGBA8888) {
        if (u32Width == 1920 && u32Height == 1080 && value == 0 )
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_1080P_RGBA.yuv");
        else if (u32Width == 1920 && u32Height == 1080 && value == 1 )
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_1080P2_RGBA.yuv");
        else if (u32Width == 1024 && u32Height == 768)
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_768P_RGBA.yuv");
        else
            Sample_VO_GetPictureData(&stFrameInfo, "/userdata/test/vo/res_480P_RGBA.yuv");
    } else {
        Sample_VO_DrawUI(&stFrameInfo, foramt, value);
    }

    return RK_SUCCESS;
}

static RK_VOID * Sample_VO_SendPicture_Mosaic(RK_VOID *pArg) {
    VoThreadParam *pParam = reinterpret_cast<VoThreadParam *>(pArg);
    VIDEO_FRAME_INFO_S *pstVFrame;
    RK_VOID *pMblk, *pMblk_1080P, *pMblk_1080P2, *pMblk_768P, *pMblk_480P, *pMblk_G0, *pMblk_G1;
    RK_U32 count;
    RK_S32 ret;

    Sample_VO_CreateGFXData(1920, 1080, RK_FMT_RGBA8888, 0, &pMblk_1080P);
    Sample_VO_CreateGFXData(1920, 1080, RK_FMT_RGBA8888, 1, &pMblk_1080P2);
    Sample_VO_CreateGFXData(720,  480,  RK_FMT_RGBA8888, 0, &pMblk_480P);
    Sample_VO_CreateGFXData(1024, 768,  RK_FMT_RGBA8888, 0, &pMblk_768P);
    Sample_VO_CreateGFXData(1920, 1080, RK_FMT_BGRA5551, 0x1F, &pMblk_G0);
    Sample_VO_CreateGFXData(1920, 1080, RK_FMT_BGRA5551, 0x1F << 10, &pMblk_G1);

    pstVFrame = reinterpret_cast<VIDEO_FRAME_INFO_S *>(malloc(sizeof(VIDEO_FRAME_INFO_S)));

    while (!pParam->u32Exit) {
        if (pParam->VoVideoLayer >= 0) {
            for (RK_U32 i = 0; i < pParam->u32VideoWindows; i++) {
                if (pParam->u32VideoWindows > 10) {
                    pstVFrame->stVFrame.u32Width = 720;
                    pstVFrame->stVFrame.u32Height = 480;
                    pstVFrame->stVFrame.u32VirWidth = 720;
                    pstVFrame->stVFrame.u32VirHeight = 480;
                    pstVFrame->stVFrame.enPixelFormat = RK_FMT_RGBA8888;
                    pMblk = pMblk_480P;
                } else if (count % 2) {
                    pMblk = pMblk_1080P;
                    pstVFrame->stVFrame.u32Width = 1920;
                    pstVFrame->stVFrame.u32Height = 1080;
                    pstVFrame->stVFrame.u32VirWidth = 1920;
                    pstVFrame->stVFrame.u32VirHeight = 1080;
                    pstVFrame->stVFrame.enPixelFormat = RK_FMT_RGBA8888;
                    pstVFrame->stVFrame.pMbBlk = pMblk;
                } else {
                    pMblk = pMblk_1080P2;
                    pstVFrame->stVFrame.u32Width = 1920;
                    pstVFrame->stVFrame.u32Height = 1080;
                    pstVFrame->stVFrame.u32VirWidth = 1920;
                    pstVFrame->stVFrame.u32VirHeight = 1080;
                    pstVFrame->stVFrame.enPixelFormat = RK_FMT_RGBA8888;
                    pstVFrame->stVFrame.pMbBlk = pMblk;
                }

                pstVFrame->stVFrame.pMbBlk = pMblk;
                ret = RK_MPI_VO_SendFrame(pParam->VoVideoLayer, i, pstVFrame, 0);
                if (!ret)
                     Sample_Print("RK_MPI_VO_SendFrame send fail ret = %08x\n", ret);
            }
        }
        if (pParam->VOGfxLayer >= 0) {
            for (RK_U32 i = 0; i < pParam->u32GFXLayers; i++) {
                if (i == 0)
                    pMblk = pMblk_G0;
                else if (i == 1)
                    pMblk = pMblk_G1;
                else
                   continue;

                pstVFrame->stVFrame.u32Width = 1920;
                pstVFrame->stVFrame.u32Height = 1080;
                pstVFrame->stVFrame.u32VirWidth = 1920;
                pstVFrame->stVFrame.u32VirHeight = 1080;
                pstVFrame->stVFrame.enPixelFormat = RK_FMT_BGRA5551;
                pstVFrame->stVFrame.pMbBlk = pMblk;
                ret = RK_MPI_VO_SendFrame(pParam->VOGfxLayer, i, pstVFrame, 0);
                if (!ret)
                    Sample_Print("RK_MPI_VO_SendFrame send fail ret = %08x\n", ret);
            }
        }
        usleep(1000000);
        count++;
    }
    free(pstVFrame);
    RK_MPI_VO_DestroyGraphicsFrameBuffer(pMblk);
    Sample_Print("%s exit\n", __func__);

    return NULL;
}

static RK_VOID Sample_VO_StartSendPicture_Mosaic(pthread_t *pThreadID, VoThreadParam *pstParam) {
    pthread_create(pThreadID, NULL, Sample_VO_SendPicture_Mosaic, reinterpret_cast<RK_VOID *>(pstParam));
}

static RK_VOID Sample_VO_StopSendPicture_Mosaic(pthread_t tThreadID, VoThreadParam *pstParam) {
    pstParam->u32Exit = 1;
    pthread_join(tThreadID, RK_NULL);
}

static RK_S32 Sample_VO_Demo_Video_Mosaic(TEST_VO_CTX_S *ctx) {
    VO_PUB_ATTR_S            stVoPubAttr;
    VO_VIDEO_LAYER_ATTR_S    stLayerAttr;
    RK_U32                   u32DispWidth, u32DispHeight;
    RK_U32                   u32ImageWidth, u32ImageHeight;
    VO_LAYER                 VoLayer;
    VO_DEV                   VoDev;
    RK_S32                   u32Windows;
    RK_U32                   u32Fps, i;
    pthread_t                tThreadID;
    RK_S32                   s32Ret = RK_SUCCESS;
    VO_LAYER_MODE_E          Vo_layer_mode;
    VoThreadParam            stThreadParam;
    PIXEL_FORMAT_E           video_format;

    VoDev = ctx->VoDev;
    u32Windows = ctx->u32Windows;
    u32Fps =  ctx->u32DispFrmRt;
    VoLayer = ctx->VoLayer;
    for (RK_U32 j = 0; j < VDEC_ARRAY_ELEMS(testPixelFormat); j++) {
        if (ctx->s32PixFormat == testPixelFormat[j].index) {
            video_format =  testPixelFormat[j].format;
            break;
        }
    }

    Sample_Print("%s VoDev %u Windows %u video_format %x\n", __func__, VoDev, u32Windows, video_format);

    /* Bind Layer */
    switch (ctx->VoLayerMode) {
        case 0:
            Vo_layer_mode = VO_LAYER_MODE_CURSOR;
            break;
        case 1:
            Vo_layer_mode = VO_LAYER_MODE_GRAPHIC;
            break;
        case 2:
            Vo_layer_mode = VO_LAYER_MODE_VIDEO;
            break;
        default:
            Vo_layer_mode = VO_LAYER_MODE_VIDEO;
    }

    RK_MPI_VO_BindLayer(VoLayer, VoDev, Vo_layer_mode);

    switch (ctx->enIntfType) {
        case DISPLAY_TYPE_VGA:
            stVoPubAttr.enIntfType = VO_INTF_VGA;
            break;
        case DISPLAY_TYPE_HDMI:
            stVoPubAttr.enIntfType = VO_INTF_HDMI;
            break;
        case DISPLAY_TYPE_EDP:
            stVoPubAttr.enIntfType = VO_INTF_EDP;
            break;
        case DISPLAY_TYPE_DP:
            stVoPubAttr.enIntfType = VO_INTF_DP;
            break;
        case DISPLAY_TYPE_HDMI_EDP :
            stVoPubAttr.enIntfType = VO_INTF_HDMI | VO_INTF_EDP;
            break;
        case DISPLAY_TYPE_MIPI :
            stVoPubAttr.enIntfType = VO_INTF_MIPI;
            break;
        default:
            stVoPubAttr.enIntfType = VO_INTF_DEFAULT;
            Sample_Print("IntfType not set,use INTF_HDMI default\n");
    }

    for (i = 0; i < ARRAY_LENGTH(test_mode_table); i++) {
        if (ctx->enIntfSync == test_mode_table[i].mode)
            break;
    }

    if (i == ARRAY_LENGTH(test_mode_table)) {
        Sample_Print("%s not found supported mode in test mode_table\n", __func__);
        return RK_FAILURE;
    }

    stVoPubAttr.enIntfSync = test_mode_table[i].enIntfSync;
    stVoPubAttr.u32BgColor = 0x000000;
    Sample_VO_StartDev(VoDev, &stVoPubAttr);

     /* Enable Layer */
    stLayerAttr.enPixFormat           = video_format;
    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    u32DispWidth = ctx->u32DispWidth;
    u32DispHeight = ctx->u32DispHeight;
    Sample_VO_GetDisplaySize(stVoPubAttr.enIntfSync, &u32DispWidth, &u32DispHeight);

    u32ImageWidth  = u32DispWidth;
    u32ImageHeight = u32DispHeight;
    stLayerAttr.stDispRect.u32Width   = u32DispWidth;
    stLayerAttr.stDispRect.u32Height  = u32DispHeight;
    stLayerAttr.stImageSize.u32Width  = u32ImageWidth;
    stLayerAttr.stImageSize.u32Height = u32ImageHeight;
    stLayerAttr.u32DispFrmRt          = ctx->u32DispFrmRt;
    Sample_VO_StartLayer(VoLayer, &stLayerAttr, ctx->bUseRga);
    Sample_VO_MultiWindowns_Start(ctx);
    stThreadParam.u32Exit = 0;
    stThreadParam.u32VideoWindows = u32Windows;
    stThreadParam.VoVideoLayer = VoLayer;
    stThreadParam.VOGfxLayer = -1;
    stThreadParam.u32GFXLayers = -1;

    Sample_Print("Start Send Picture\n");
    Sample_VO_StartSendPicture_Mosaic(&tThreadID, &stThreadParam);

    while (1) {
        Sample_Print("Press 'q' to quit\n");
        if (getchar() == 'q')
            break;
    }

    Sample_VO_StopSendPicture_Mosaic(tThreadID, &stThreadParam);
    Sample_VO_MultiWindowns_Stop(VoLayer, u32Windows);
    Sample_VO_StopLayer(VoLayer);
    Sample_VO_StopDev(VoDev);
 end:
    RK_MPI_VO_UnBindLayer(VoLayer, VoDev);
    RK_MPI_VO_CloseFd();

    return s32Ret;
}

static RK_S32 Sample_VO_Demo_UI(TEST_VO_CTX_S *ctx) {
    VO_PUB_ATTR_S            stVoPubAttr;
    VO_VIDEO_LAYER_ATTR_S    stLayerAttr;
    VIDEO_FRAME_INFO_S       *pstVFrame;
    RK_U32                   u32DispWidth, u32DispHeight;
    RK_U32                   u32ImgWidth, u32ImgHeight;
    VO_LAYER                 VoLayer;
    RK_S32                   VoDev;
    RK_S32                   VoDev_second;
    VO_LAYER                 VoLayer_second;
    RK_VOID *pMblk_1080P_1   = RK_NULL;
    RK_VOID *pMblk_1080P_2   = RK_NULL;
    RK_VOID *pMblk_768P_1    = RK_NULL;
    RK_VOID *pMblk_768P_2    = RK_NULL;
    RK_U32   s32Ret          = RK_SUCCESS;
    RK_S32   count           = 0;
    PIXEL_FORMAT_E           plane_format;

    for (RK_U32 j = 0; j < VDEC_ARRAY_ELEMS(testPixelFormat); j++) {
        if (ctx->s32PixFormat == testPixelFormat[j].index) {
            plane_format =  testPixelFormat[j].format;
            break;
        }
    }

    pstVFrame = reinterpret_cast<VIDEO_FRAME_INFO_S *>(malloc(sizeof(VIDEO_FRAME_INFO_S)));
    /* GFX buffer dispaly on VoDev 0 */
    Sample_VO_CreateGFXData(1920, 1080,
                            RK_FMT_RGB888, 0, &pMblk_1080P_1);
    Sample_VO_CreateGFXData(1920, 1080,
                            RK_FMT_RGB888, 1, &pMblk_1080P_2);

    /* GFX buffer dispaly on VoDev 1 */
    if (ctx->bDoubleScreen) {
        Sample_Print("DoubleScreen UI Demo test");
        Sample_VO_CreateGFXData(1024, 768,
                                RK_FMT_RGB888, 0, &pMblk_768P_1);
        Sample_VO_CreateGFXData(1024, 768,
                                RK_FMT_RGB888, 1, &pMblk_768P_2);
    } else if (ctx->bHomologous) {
         Sample_Print("Homologous UI Demo test");
    } else {
        Sample_Print("Single HDMI Screen UI Demo test");
    }
    /* Bind Layer */
    VoLayer = RK356X_VOP_LAYER_ESMART_0;
    VoDev   = RK356X_VO_DEV_HD0;
    if (ctx->bDoubleScreen) {
        VoLayer_second = RK356X_VOP_LAYER_ESMART_1;
        VoDev_second   = RK356X_VO_DEV_HD1;
        RK_MPI_VO_BindLayer(VoLayer_second, VoDev_second, VO_LAYER_MODE_GRAPHIC);
    }
    RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_GRAPHIC);

    if (ctx->bHomologous) {
        stVoPubAttr.enIntfType = VO_INTF_HDMI | VO_INTF_EDP;
        stVoPubAttr.u32BgColor = 0x000000;
    } else {
        stVoPubAttr.enIntfType = VO_INTF_HDMI;
    }
    SampleSetVoInterfacetiming(&stVoPubAttr, ctx->enIntfSync);
    Sample_VO_StartDev(VoDev, &stVoPubAttr);

    stLayerAttr.enPixFormat                  = plane_format;
    stLayerAttr.stDispRect.s32X              = 0;
    stLayerAttr.stDispRect.s32Y              = 0;
    stLayerAttr.stDispRect.u32Width          = ctx->u32Screen0DisplayWidth;
    stLayerAttr.stDispRect.u32Height         = ctx->u32Screen0DisplayHeight;
    stLayerAttr.stImageSize.u32Width         = ctx->u32Screen0ImageWidth;
    stLayerAttr.stImageSize.u32Height        = ctx->u32Screen0ImageHeight;
    Sample_VO_StartLayer(VoLayer, &stLayerAttr, ctx->bUseRga);

    if (ctx->bDoubleScreen) {
        stVoPubAttr.enIntfType = VO_INTF_EDP;
        stVoPubAttr.u32BgColor = 0x000000;
        SampleSetVoInterfacetiming(&stVoPubAttr, ctx->enIntfSync_second);
        Sample_VO_StartDev(VoDev_second, &stVoPubAttr);
        stLayerAttr.enPixFormat                  = plane_format;
        stLayerAttr.stDispRect.s32X              = 0;
        stLayerAttr.stDispRect.s32Y              = 0;
        stLayerAttr.stDispRect.u32Width          = ctx->u32Screen1DisplayWidth;
        stLayerAttr.stDispRect.u32Height         = ctx->u32Screen1DisplayHeight;
        stLayerAttr.stImageSize.u32Width         = ctx->u32Screen1ImageWidth;
        stLayerAttr.stImageSize.u32Height        = ctx->u32Screen1ImageHeight;
        Sample_VO_StartLayer(VoLayer_second, &stLayerAttr, ctx->bUseRga);
    }

    while (1) {
        sleep(1);
        count++;
        if (count % 2) {
            Sample_VO_SetVFrame(pstVFrame,
                                1920,
                                1080,
                                1920,
                                1080,
                                RK_FMT_RGB888,
                                pMblk_1080P_1);
            RK_MPI_VO_SendLayerFrame(VoLayer, pstVFrame);
            if (ctx->bDoubleScreen) {
                Sample_VO_SetVFrame(pstVFrame,
                                    ctx->u32Screen1DisplayWidth,
                                    ctx->u32Screen1DisplayHeight,
                                    ctx->u32Screen1DisplayWidth,
                                    ctx->u32Screen1DisplayHeight,
                                    RK_FMT_RGB888,
                                    pMblk_768P_1);
                RK_MPI_VO_SendLayerFrame(VoLayer_second, pstVFrame);
            }
        } else {
            Sample_VO_SetVFrame(pstVFrame,
                                1920,
                                1080,
                                1920,
                                1080,
                                RK_FMT_RGB888,
                                pMblk_1080P_2);
            RK_MPI_VO_SendLayerFrame(VoLayer, pstVFrame);
            if (ctx->bDoubleScreen) {
                Sample_VO_SetVFrame(pstVFrame,
                                    ctx->u32Screen1DisplayWidth,
                                    ctx->u32Screen1DisplayHeight,
                                    ctx->u32Screen1DisplayWidth,
                                    ctx->u32Screen1DisplayHeight,
                                    RK_FMT_RGB888,
                                    pMblk_768P_2);
                RK_MPI_VO_SendLayerFrame(VoLayer_second, pstVFrame);
            }
        }

        if (count == ctx->loopCount )
            break;
    }

    Sample_VO_StopLayer(VoLayer);
    Sample_VO_StopDev(VoDev);
    RK_MPI_VO_UnBindLayer(VoLayer, VoDev);
    RK_MPI_VO_DestroyGraphicsFrameBuffer(pMblk_1080P_1);
    RK_MPI_VO_DestroyGraphicsFrameBuffer(pMblk_1080P_2);

    if (ctx->bDoubleScreen) {
        Sample_VO_StopLayer(VoLayer_second);
        Sample_VO_StopDev(VoDev_second);
        RK_MPI_VO_UnBindLayer(VoLayer_second, VoDev_second);
        RK_MPI_VO_DestroyGraphicsFrameBuffer(pMblk_768P_1);
        RK_MPI_VO_DestroyGraphicsFrameBuffer(pMblk_768P_2);
    }
end:
    RK_MPI_VO_CloseFd();

    return s32Ret;
}

static RK_S32 Sample_VO_Demo_Alpha(TEST_VO_CTX_S *ctx) {
    VO_PUB_ATTR_S           stVoPubAttr;
    VO_VIDEO_LAYER_ATTR_S   stLayerAttr;
    VoThreadParam           stThreadParam;
    RK_U32                  u32DispWidth, u32DispHeight, u32GfxLayers;
    VO_LAYER                VoGfxLayer, VoVideoLayer;
    RK_S32                  s32Ret = RK_SUCCESS;
    RK_U32                  u32VideoWindows;
    pthread_t               tThreadDev0;
    PIXEL_FORMAT_E          video_format;
    RK_U32                  VoWbc;
    PIXEL_FORMAT_E          wbc_format;
    VO_LAYER                VoLayer;
    VO_DEV                  VoDev;

    u32VideoWindows =  ctx->u32Windows;
    for (RK_U32 j = 0; j < VDEC_ARRAY_ELEMS(testPixelFormat); j++) {
        if (ctx->s32PixFormat == testPixelFormat[j].index) {
            video_format =  testPixelFormat[j].format;
            break;
        }
    }
    /* Bind Layer */
    VoDev == RK356X_VO_DEV_HD0;
    VoGfxLayer = RK356X_VOP_LAYER_ESMART_0;
    VoVideoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    RK_MPI_VO_BindLayer(VoGfxLayer, VoDev, VO_LAYER_MODE_GRAPHIC);
    RK_MPI_VO_BindLayer(VoVideoLayer, VoDev, VO_LAYER_MODE_VIDEO);
    /* Enable VO Dev0 Device */
    u32DispWidth = 1920;
    u32DispHeight = 1080;
    stVoPubAttr.enIntfType = VO_INTF_HDMI;
    stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
    Sample_VO_StartDev(VoDev, &stVoPubAttr);
    /* Enable Vodev 0  Video Layer */
    if (VoVideoLayer != -1) {
        stLayerAttr.enPixFormat              = video_format;
        stLayerAttr.stDispRect.s32X          = 0;
        stLayerAttr.stDispRect.s32Y          = 0;
        stLayerAttr.stDispRect.u32Width      = u32DispWidth;
        stLayerAttr.stDispRect.u32Height     = u32DispHeight;
        stLayerAttr.stImageSize.u32Width     = u32DispWidth;
        stLayerAttr.stImageSize.u32Height    = u32DispHeight;
        stLayerAttr.u32DispFrmRt             = 25;
        Sample_VO_StartLayer(VoVideoLayer, &stLayerAttr, ctx->bUseRga);
        ctx->VoLayer = VoVideoLayer;
        Sample_VO_MultiWindowns_Start(ctx);
    }
    /* Enable Vodev 0  GFX Layer */
    u32GfxLayers = 2;
    if (VoGfxLayer != -1) {
        stLayerAttr.enPixFormat                  = video_format;
        stLayerAttr.stDispRect.s32X              = 0;
        stLayerAttr.stDispRect.s32Y              = 0;
        stLayerAttr.stDispRect.u32Width          = u32DispWidth;
        stLayerAttr.stDispRect.u32Height         = u32DispHeight;
        stLayerAttr.stImageSize.u32Width         = u32DispWidth;
        stLayerAttr.stImageSize.u32Height        = u32DispHeight;
        stLayerAttr.u32DispFrmRt                 = 25;
        Sample_VO_StartLayer(VoGfxLayer, &stLayerAttr, ctx->bUseRga);
        Sample_VO_MultiGFXLayer_Start(VoGfxLayer, u32GfxLayers);
    }

    Sample_Print("HD0: Start Send Picture\n");

    stThreadParam.u32Exit         = 0;
    stThreadParam.u32VideoWindows = u32VideoWindows;
    stThreadParam.VoVideoLayer    = VoVideoLayer;
    stThreadParam.u32GFXLayers    = u32GfxLayers;
    stThreadParam.VOGfxLayer      = VoGfxLayer;
    Sample_VO_StartSendPicture_Mosaic(&tThreadDev0, &stThreadParam);

    Sample_Print("Start HD1\n");
    VoDev = RK356X_VO_DEV_HD1;
    /* Bind Vodev1 Layer */
    VoLayer = RK356X_VOP_LAYER_CLUSTER_1;
    RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_VIDEO);
    /* Enable VO Dev1 Device */
    u32DispWidth = 1024;
    u32DispHeight = 768;
    stVoPubAttr.enIntfType = VO_INTF_EDP;
    stVoPubAttr.enIntfSync = VO_OUTPUT_1024x768_60;
    stVoPubAttr.u32BgColor = 0x000000;
    Sample_VO_StartDev(VoDev, &stVoPubAttr);
    /* Enable Layer */
    stLayerAttr.enPixFormat              = RK_FMT_BGR888;
    stLayerAttr.stDispRect.s32X          = 0;
    stLayerAttr.stDispRect.s32Y          = 0;
    stLayerAttr.stDispRect.u32Width      = u32DispWidth;
    stLayerAttr.stDispRect.u32Height     = u32DispHeight;
    stLayerAttr.stImageSize.u32Width     = u32DispWidth;
    stLayerAttr.stImageSize.u32Height    = u32DispHeight;
    stLayerAttr.u32DispFrmRt             = 25;
    Sample_VO_StartLayer(VoLayer, &stLayerAttr, ctx->bUseRga);

    ctx->VoLayer = VoLayer;
    ctx->u32Windows = 1;
    Sample_VO_MultiWindowns_Start(ctx);

    Sample_Print("Start DEV0 WBC\n");
    /* Start WBC and bind to HD0 */
    VoWbc = 0;
    /* WBC bind source */
    ctx->stWbcCfg.stWbcSource.enSourceType = VO_WBC_SOURCE_DEV;
    ctx->stWbcCfg.stWbcSource.u32SourceId = RK356X_VO_DEV_HD0;

    RK_MPI_VO_SetWbcSource(VoWbc, &ctx->stWbcCfg.stWbcSource);
    /* Start WBC */
    wbc_format = Sample_wbc_formt(ctx->u32WbcPixFormat);

    ctx->stWbcCfg.stWbcAttr.enPixelFormat = wbc_format;
    Sample_Print("wbc format is %x\n", ctx->stWbcCfg.stWbcAttr.enPixelFormat);

    RK_MPI_VO_SetWbcAttr(VoWbc, &ctx->stWbcCfg.stWbcAttr);
    RK_MPI_VO_EnableWbc(VoWbc);

    if (ctx->wbc_bind == RK_TRUE)
        Sample_VO_BindVoWbc(VoWbc, VoLayer, 0);
    sleep(2);
    while (1) {
        Sample_Print("Press 'q' to quit\n");
        if (getchar() == 'q')
            break;
    }
    Sample_VO_StopSendPicture_Mosaic(tThreadDev0, &stThreadParam);
    Sample_VO_UnBindVoWbc(VoWbc, VoLayer, 0);
    Sample_Print("Stop dev0 WBC\n");
    RK_MPI_VO_DisableWbc(VoWbc);

    Sample_Print("Stop HD1\n");
    Sample_VO_MultiWindowns_Stop(VoLayer, 1);
    Sample_VO_StopLayer(VoLayer);
    Sample_VO_StopDev(VoDev);
    RK_MPI_VO_UnBindLayer(VoLayer, VoDev);

    Sample_Print("Stop HD0");
    VoGfxLayer   = RK356X_VOP_LAYER_ESMART_0;
    VoVideoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    VoDev        = RK356X_VO_DEV_HD0;
    Sample_VO_MultiWindowns_Stop(VoGfxLayer, u32GfxLayers);
    Sample_VO_StopLayer(VoGfxLayer);
    Sample_VO_MultiWindowns_Stop(VoVideoLayer, u32VideoWindows);
    Sample_VO_StopLayer(VoVideoLayer);
    RK_MPI_VO_UnBindLayer(VoGfxLayer, VoDev);
    RK_MPI_VO_UnBindLayer(VoVideoLayer, VoDev);
    Sample_VO_StopDev(VoDev);

    RK_MPI_VO_CloseFd();

    return s32Ret;
}

static RK_S32 Sample_VO_Video_Homologous(TEST_VO_CTX_S *ctx) {
    VO_WBC_SOURCE_S          stWbcSource;
    VO_WBC_ATTR_S            stWbcAttr;
    RK_U32 VoWbc;
    VIDEO_FRAME_INFO_S       *pFrame;
    VO_PUB_ATTR_S            stVoPubAttr;
    VO_VIDEO_LAYER_ATTR_S    stLayerAttr;
    RK_U32                   u32DispWidth, u32DispHeight;
    VO_LAYER                 VoLayer;
    VO_DEV                   VoDev;
    RK_S32                   u32Windows;
    RK_U32                   u32Fps, i;
    pthread_t                tThreadID;
    RK_S32                   s32Ret = RK_SUCCESS;
    VO_LAYER_MODE_E          Vo_layer_mode;
    VoThreadParam            stThreadParam;
    PIXEL_FORMAT_E           video_format;
    TEST_VO_CTX_S   stVoCtx;
    PIXEL_FORMAT_E  wbc_format;
    VoDev = ctx->VoDev;
    u32Windows = ctx->u32Windows;
    u32Fps =  ctx->u32DispFrmRt;
    VoLayer = ctx->VoLayer;
    for (RK_U32 j = 0; j < VDEC_ARRAY_ELEMS(testPixelFormat); j++) {
        if (ctx->s32PixFormat == testPixelFormat[j].index) {
            video_format =  testPixelFormat[j].format;
            break;
        }
    }

    Sample_Print("%s VoDev %u Windows %u video_format %x\n", __func__, VoDev, u32Windows, video_format);

    /* Bind Layer */
    switch (ctx->VoLayerMode) {
        case 0:
            Vo_layer_mode = VO_LAYER_MODE_CURSOR;
            break;
        case 1:
            Vo_layer_mode = VO_LAYER_MODE_GRAPHIC;
            break;
        case 2:
            Vo_layer_mode = VO_LAYER_MODE_VIDEO;
            break;
        default:
            Vo_layer_mode = VO_LAYER_MODE_VIDEO;
    }

    RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_VIDEO);

    switch (ctx->enIntfType) {
        case DISPLAY_TYPE_VGA:
            stVoPubAttr.enIntfType = VO_INTF_VGA;
            break;
        case DISPLAY_TYPE_HDMI:
            stVoPubAttr.enIntfType = VO_INTF_HDMI;
            break;
        case DISPLAY_TYPE_EDP:
            stVoPubAttr.enIntfType = VO_INTF_EDP;
            break;
        case DISPLAY_TYPE_DP:
            stVoPubAttr.enIntfType = VO_INTF_DP;
            break;
        default:
            stVoPubAttr.enIntfType = VO_INTF_HDMI;
            Sample_Print("IntfType not set,use INTF_HDMI default\n");
    }

    for (i = 0; i < ARRAY_LENGTH(test_mode_table); i++) {
        if (ctx->enIntfSync == test_mode_table[i].mode)
            break;
    }

    if (i == ARRAY_LENGTH(test_mode_table)) {
        Sample_Print("%s not found supported mode in test mode_table\n", __func__);
        return RK_FAILURE;
    }

    stVoPubAttr.enIntfSync = test_mode_table[i].enIntfSync;
    Sample_VO_StartDev(VoDev, &stVoPubAttr);

     /* Enable Layer */
    stLayerAttr.enPixFormat                  = video_format;
    stLayerAttr.stDispRect.s32X              = ctx->s32Y;
    stLayerAttr.stDispRect.s32Y              = ctx->s32Y;
    if (ctx->VoDev== RK356X_VO_DEV_HD0) {
    stLayerAttr.stDispRect.u32Width          = ctx->u32DispWidth;
    stLayerAttr.stDispRect.u32Height         = ctx->u32DispHeight;
    stLayerAttr.stImageSize.u32Width         = ctx->u32ImgeWidth;
    stLayerAttr.stImageSize.u32Height        = ctx->u32ImageHeight;
    } else if (ctx->VoDev== RK356X_VO_DEV_HD1) {
    stLayerAttr.stDispRect.u32Width          = 1024;
    stLayerAttr.stDispRect.u32Height         = 768;
    stLayerAttr.stImageSize.u32Width         = 1024;
    stLayerAttr.stImageSize.u32Height        = 768;
    }
    stLayerAttr.u32DispFrmRt                 = ctx->u32DispFrmRt;
    Sample_VO_StartLayer(VoLayer, &stLayerAttr, ctx->bUseRga);
    Sample_VO_MultiWindowns_Start(ctx);
    stThreadParam.u32Exit = 0;
    stThreadParam.u32VideoWindows = u32Windows;
    stThreadParam.VoVideoLayer = VoLayer;
    stThreadParam.VOGfxLayer = -1;
    stThreadParam.u32GFXLayers = -1;

    Sample_Print("Start Send Picture\n");
    Sample_VO_StartSendPicture_Mosaic(&tThreadID, &stThreadParam);

    Sample_Print("Start HD1\n");
    /* Start HD1 Device */
    VoDev = RK356X_VO_DEV_HD1;
    /* Bind Layer */
    VoLayer = RK356X_VOP_LAYER_CLUSTER_1;
    RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_VIDEO);

    /* Enable VO Device */
    if (VoDev == RK356X_VO_DEV_HD0) {
        u32DispWidth = 1920;
        u32DispHeight = 1080;
        stVoPubAttr.enIntfType = VO_INTF_HDMI;
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P50;
    } else if (VoDev == RK356X_VO_DEV_HD1) {
        u32DispWidth = 1024;
        u32DispHeight = 768;
        stVoPubAttr.u32BgColor = 0x000000;
        stVoPubAttr.enIntfType = VO_INTF_EDP;
        stVoPubAttr.enIntfSync = VO_OUTPUT_1024x768_60;
    } else {
        s32Ret = RK_FAILURE;
       // goto end;
    }
    Sample_VO_StartDev(VoDev, &stVoPubAttr);

    /* Enable Layer */
    stLayerAttr.enPixFormat              = RK_FMT_BGR888;
    stLayerAttr.stDispRect.s32X          = 0;
    stLayerAttr.stDispRect.s32Y          = 0;
    stLayerAttr.stDispRect.u32Width      = u32DispWidth;
    stLayerAttr.stDispRect.u32Height     = u32DispHeight;
    stLayerAttr.stImageSize.u32Width     = u32DispWidth;
    stLayerAttr.stImageSize.u32Height    = u32DispHeight;
    stLayerAttr.u32DispFrmRt             = 25;
    Sample_VO_StartLayer(VoLayer, &stLayerAttr, ctx->bUseRga);

    ctx->VoLayer = RK356X_VOP_LAYER_CLUSTER_1;
    ctx->u32Windows = 1;
    Sample_VO_MultiWindowns_Start(ctx);

    Sample_Print("Start Cluster0 WBC\n");

    /* Start WBC and bind to HD0 */
    VoWbc = 0;
    /* WBC bind source */
    ctx->stWbcCfg.stWbcSource.enSourceType = VO_WBC_SOURCE_VIDEO;
    ctx->stWbcCfg.stWbcSource.u32SourceId = RK356X_VO_DEV_HD0;

    RK_MPI_VO_SetWbcSource(VoWbc, &ctx->stWbcCfg.stWbcSource);

    /* Start WBC */
    wbc_format = Sample_wbc_formt(ctx->u32WbcPixFormat);

    ctx->stWbcCfg.stWbcAttr.enPixelFormat = wbc_format;
    if (ctx->u32WbcWidth > 1920 && ctx->u32WbcHeight > 1080) {
        Sample_Print("WBC resolution exceeding 1080P\n");
        ctx->u32WbcWidth = 1920;
        ctx->u32WbcHeight = 1080;
    }

    ctx->stWbcCfg.stWbcAttr.stTargetSize.u32Width =  ctx->u32WbcWidth;
    ctx->stWbcCfg.stWbcAttr.stTargetSize.u32Height =  ctx->u32WbcHeight;

    Sample_Print("wbc format is %x\n", ctx->stWbcCfg.stWbcAttr.enPixelFormat);

    RK_MPI_VO_SetWbcAttr(VoWbc, &ctx->stWbcCfg.stWbcAttr);
    RK_MPI_VO_EnableWbc(VoWbc);

    Sample_Print("--------------\nWBC Enabled\n---------------\n");

    if (ctx->wbc_auto == RK_TRUE) {
        /* Bind WBC to HD1 */
        if (ctx->wbc_bind == RK_TRUE)
            Sample_VO_BindVoWbc(VoWbc, VoLayer, 0);

        while (1) {
            Sample_Print("Press 'q' to quit\n");
            if (getchar() == 'q')
                break;
        }
        Sample_VO_StopSendPicture_Mosaic(tThreadID, &stThreadParam);
        Sample_Print("Stop WBC\n");
        Sample_VO_UnBindVoWbc(VoWbc, VoLayer, 0);
        RK_MPI_VO_DisableWbc(VoWbc);
        Sample_Print("Stop HD1\n");
        Sample_VO_MultiWindowns_Stop(VoLayer, 1);
        Sample_VO_StopLayer(VoLayer);
        Sample_VO_StopDev(VoDev);
        RK_MPI_VO_UnBindLayer(VoLayer, VoDev);
        Sample_Print("Stop HD0\n");
        VoDev = RK356X_VO_DEV_HD0;
        VoLayer = RK356X_VOP_LAYER_CLUSTER_0;

        Sample_VO_MultiWindowns_Stop(VoLayer, u32Windows);
        Sample_VO_StopLayer(VoLayer);
        Sample_VO_StopDev(VoDev);
        RK_MPI_VO_CloseFd();
    }

    return s32Ret;
}

static RK_S32 check_options(const TEST_VO_CTX_S *ctx) {
    if (ctx->VoDev > VO_MAX_DEV_NUM) {
        RK_LOGE("illegal param, pls enter 0 or 1 max Vodev is %d!", VO_MAX_DEV_NUM);
        goto __FAILED;
    }
    if ((ctx->VoLayerMode == 2) && (ctx->VoLayer > VO_MAX_LAYER_NUM)) {
        RK_LOGE("illegal param, pls enter 0 or 2 max VoLyer is %d!", VO_MAX_LAYER_NUM);
        goto __FAILED;
    }
    if ((ctx->VoLayerMode == 1) && (ctx->VoLayer > VO_MAX_LAYER_NUM)) {
        RK_LOGE("illegal param, pls enter 4 or 5 max VoLyer is %d!", VO_MAX_LAYER_NUM);
        goto __FAILED;
    }
    if (ctx->u32Windows <= 0 || ctx->u32Windows >= MAX_WINDOWS_NUM) {
        RK_LOGE("illegal param, windos rang [1,63] max u32Windows is %d!", MAX_WINDOWS_NUM);
        goto __FAILED;
    }
    if (ctx->u32WbcPixFormat >= MAX_VO_FORMAT_RGB_NUM) {
        RK_LOGE("illegal param, WBC PixFormat rang [0,3]");
        goto __FAILED;
    }
    if (ctx->s32PixFormat > MAX_VO_FORMAT_RGB_NUM) {
        RK_LOGE("illegal param, Vidoe PixFormat rang [0,17]");
        goto __FAILED;
    }

    return RK_SUCCESS;

__FAILED:
    return RK_FAILURE;
}

static const char *const usages[] = {
    "./rk_mpi_vo_test [-i SRC_PATH] [--imag_width IMG_WIDTH]"
    "[--imag_height IMG_HEIGHT] [--dis_width DIS_WIDTH] [--dis_height DIS_HEIGHT]...",
    NULL,
};

static void  mpi_vo_test_show_options(const TEST_VO_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("vop device id                   : %d\n", ctx->VoDev);
    RK_PRINT("vop layer id                    : %d\n", ctx->VoLayer);
    RK_PRINT("window size                     : %d\n", ctx->u32Windows);
    RK_PRINT("connector type                  : %d\n", ctx->enIntfType);
    RK_PRINT("display mode                    : %d\n", ctx->enIntfSync);
    RK_PRINT("layer mode                      : %d\n", ctx->VoLayerMode);
    RK_PRINT("display resolution rect X       : %d\n", ctx->s32X);
    RK_PRINT("display resolution rect Y       : %d\n", ctx->s32Y);
    RK_PRINT("display pixel format            : %d\n", ctx->s32PixFormat);
    RK_PRINT("display resolution width        : %d\n", ctx->u32DispWidth);
    RK_PRINT("display resolution height       : %d\n", ctx->u32DispHeight);
    RK_PRINT("display resolution0 width       : %d\n", ctx->u32Screen0DisplayWidth);
    RK_PRINT("display resolution0 height      : %d\n", ctx->u32Screen0DisplayHeight);
    RK_PRINT("display resolution1 width       : %d\n", ctx->u32Screen1DisplayWidth);
    RK_PRINT("display resolution1 height      : %d\n", ctx->u32Screen1DisplayHeight);
    RK_PRINT("double screen                   : %d\n", ctx->bDoubleScreen);
    RK_PRINT("display rate                    : %d\n", ctx->u32DispFrmRt);
    RK_PRINT("display rate ratio              : %d\n", ctx->u32DispFrmRtRatio);
    RK_PRINT("chn display mode                : %d\n", ctx->u32ChnDismode);
    RK_PRINT("chn Border lpx                  : %d\n", ctx->stBorderCfg.u32LeftWidth);
    RK_PRINT("chn Border rpx                  : %d\n", ctx->stBorderCfg.u32RightWidth);
    RK_PRINT("chn Border tpx                  : %d\n", ctx->stBorderCfg.u32TopWidth);
    RK_PRINT("chn Border bpx                  : %d\n", ctx->stBorderCfg.u32BottomWidth);
    RK_PRINT("video aspect ratio mode         : %d\n", ctx->uEnMode);
    RK_PRINT("input ImgWidth                  : %d\n", ctx->u32ImgeWidth);
    RK_PRINT("input ImgHeight                 : %d\n", ctx->u32ImageHeight);
    RK_PRINT("ui                              : %d\n", ctx->ui);
    RK_PRINT("loopcount                       : %d\n", ctx->loopCount);
    RK_PRINT("ui alpha                        : %d\n", ctx->ui_alpha);
    RK_PRINT("wbc enable                      : %d\n", ctx->wbc_enable);
    RK_PRINT("wbc bind enable                 : %d\n", ctx->wbc_bind);
    RK_PRINT("wbc width                       : %d\n", ctx->u32WbcWidth);
    RK_PRINT("wbc height                      : %d\n", ctx->u32WbcHeight);
    RK_PRINT("wbc compress mode               : %d\n", ctx->u32WbcCompressMode);
    RK_PRINT("wbc pixel format                : %d\n", ctx->u32WbcPixFormat);
    RK_PRINT("wbc source type                 : %d\n", ctx->u32WbcSourceType);
    RK_PRINT("wbc souce id                    : %d\n", ctx->u32WbcSourceId);
    RK_PRINT("mirror                          : %d\n", ctx->enMirror);
    RK_PRINT("rotation                        : %d\n", ctx->enRotation);
    RK_PRINT("use rga                         : %d\n", ctx->bUseRga);
}

void init_cfg(TEST_VO_CTX_S *ctx) {
    RK_S32 i = 0;

    ctx->loopCount = 10;
    ctx->u32Windows = 4;
    ctx->enIntfType = 0;  /* 0: HDMI 1: EDP 2: VGA 3: DP */
    ctx->enIntfSync = 16; /* 1080P60 */
    ctx->enIntfSync_second = 6; /* 768P */

    ctx->VoDev = RK356X_VO_DEV_HD0;
    ctx->VoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    ctx->VoLayerMode = 1; /* CURSOR = 0,GRAPHIC = 1,VIDEO = 2,*/

    ctx->u32ImgeWidth = 1920;
    ctx->u32ImageHeight = 1080;

    ctx->bDoubleScreen = RK_TRUE;
    ctx->stVoCfg.bDoubleScreen = RK_TRUE;
    ctx->s32X = 0;
    ctx->s32Y = 0;
    ctx->u32Screen0DisplayWidth  = 1920;
    ctx->u32Screen0DisplayHeight = 1080;
    ctx->u32Screen1DisplayWidth  = 1024;
    ctx->u32Screen1DisplayHeight = 768;

    ctx->u32Screen0ImageWidth  = 1920;
    ctx->u32Screen0ImageHeight = 1080;
    ctx->u32Screen1ImageWidth  = 1024;
    ctx->u32Screen1ImageHeight = 768;

    ctx->u32DispWidth  = 1920;
    ctx->u32DispHeight = 1080;
    ctx->s32PixFormat = 5;
    ctx->u32DispFrmRt = 30;
    ctx->u32DispFrmRtRatio = 1;
    ctx->uEnMode = 1;

    ctx->wbc_auto = RK_TRUE;
    ctx->u32WbcWidth = 1024;
    ctx->u32WbcHeight = 768;
    ctx->u32WbcPixFormat = WBC_FORMAT_BGR888;
    ctx->u32WbcCompressMode = COMPRESS_MODE_NONE;
    ctx->u32WbcSourceType = WBC_SOURCE_VIDEO;
    ctx->u32WbcSourceId = RK356X_VO_DEV_HD0;

    ctx->bVoPlay = RK_FALSE;
    ctx->bBorder = RK_FALSE;
    ctx->bHomologous = RK_FALSE;
    ctx->u32Screen0Chn = 16;
    ctx->u32Screen1Chn = 4;
    ctx->bEnWbc = RK_FALSE;
    ctx->wbc_bind = RK_FALSE;
    ctx->bChnPriority = RK_FALSE;
    ctx->bEnWbcToVO = RK_TRUE;
    ctx->s32LoopCount = -1;
    ctx->u32ChnDismode = VO_CHANNEL_PLAY_NORMAL;

    ctx->stVoCfg.u32Screen0VoLayer = RK356X_VOP_LAYER_CLUSTER_0;
    ctx->stVoCfg.u32Screen1VoLayer = RK356X_VOP_LAYER_CLUSTER_1;
    ctx->stVoCfg.u32Screen0Rows = 4;
    ctx->stVoCfg.u32Screen1Rows = 3;

    ctx->stVdecCfg.u32FrameBufferCnt = MAX_FRAME_QUEUE;
    ctx->stVdecCfg.enCompressMode = COMPRESS_AFBC_16x16;
    ctx->stWbcCfg.stWbcSource.enSourceType = VO_WBC_SOURCE_VIDEO;
    ctx->stWbcCfg.stWbcSource.u32SourceId = RK356X_VO_DEV_HD0;
    ctx->stWbcCfg.stWbcAttr.enPixelFormat = RK_FMT_YUV420SP;
    ctx->stWbcCfg.stWbcAttr.stTargetSize.u32Width = 1024;
    ctx->stWbcCfg.stWbcAttr.stTargetSize.u32Height = 768;
    ctx->stWbcCfg.stWbcAttr.u32FrameRate = 25;
    ctx->stWbcCfg.stWbcAttr.enCompressMode = COMPRESS_MODE_NONE;
    ctx->stWbcCfg.s32ChnId = 0;
    ctx->stWbcCfg.s32VdecChnId = -1;

    ctx->stBorderCfg.u32BottomWidth = 2;
    ctx->stBorderCfg.u32TopWidth = 2;
    ctx->stBorderCfg.u32LeftWidth = 2;
    ctx->stBorderCfg.u32RightWidth = 2;
    ctx->enMirror = MIRROR_NONE;
    ctx->enRotation = ROTATION_0;
    ctx->bUseRga = RK_FALSE;
}

int main(int argc, const char **argv) {
    RK_S32  s32Ret;
    TEST_VO_CTX_S ctx;

    memset(&ctx, 0, sizeof(TEST_VO_CTX_S));

    init_cfg(&ctx);

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx.cfgFileUri),
                   "input config file. <required>", NULL, 0, 0),
        OPT_INTEGER('d', "device_id", &(ctx.VoDev),
                     "Vop id. e.g.(0/1). default(0).", NULL, 0, 0),
        OPT_INTEGER('l', "layer_id", &(ctx.VoLayer),
                     "Layer id. e.g.(0/2/4/6) default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_enable", &(ctx.wbc_enable),
                     "wbc_enalbe. e.g.(0) default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_bind",  &(ctx.wbc_bind),
                   "enable wbc bind, default(1): 0: disable, 1: enable", NULL, 0, 0),
        OPT_INTEGER('\0', "ui", &(ctx.ui),
                     "ui. e.g.(0) default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "loopCount", &(ctx.loopCount),
                     "loopCount. e.g.(0) default(10).", NULL, 0, 0),
        OPT_INTEGER('\0', "ui_alpha", &(ctx.ui_alpha),
                     "ui_alpha. e.g.(0) default(0).", NULL, 0, 0),
        OPT_INTEGER('w', "Windows", &(ctx.u32Windows),
                     "windows num. e.g [1-64] default(4),max is 63.", NULL, 0, 0),
        OPT_INTEGER('\0', "ConnectorType", &(ctx.enIntfType),
                     "Connctor Type. e.g.(0: HDMI 1: EDP 2: VGA 5: MIPI). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "layer_mode", &(ctx.VoLayerMode),
                     "Layer type. e.g.(0: CURSOR 1: UI 2: Video). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "display_mode", &(ctx.enIntfSync),
                     "dispaly. e.g.(12/14) default (12. 12 is 1080P60, 27 output default). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "display0_mode", &(ctx.enIntfSync_second),
                     "dispaly. e.g.(12/14) default (12. 12 is 1080P60). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "disp_frmrt", &(ctx.u32DispFrmRt),
                     "disp_frmrt. default(30).", NULL, 0, 0),
        OPT_INTEGER('\0', "disp_frmrt_ratio", &(ctx.u32DispFrmRtRatio),
                     "disp_frmrt_ratio. e.g.(32, 16, 8, 4, 2 ,1). default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "aspect_mode", &(ctx.uEnMode),
                     "video aspect ratio."
                     "e.g.(1: ratio no change 2: ratio manual set). default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "border_lpx", &(ctx.stBorderCfg.u32LeftWidth),
                     "chn Border lpx. default(2).", NULL, 0, 0),
        OPT_INTEGER('\0', "border_rpx", &(ctx.stBorderCfg.u32RightWidth),
                      "chn Border rpx. default(2).", NULL, 0, 0),
        OPT_INTEGER('\0', "border_tpx", &(ctx.stBorderCfg.u32TopWidth),
                     "chn Border tpx. default(2).", NULL, 0, 0),
        OPT_INTEGER('\0', "border_bpx", &(ctx.stBorderCfg.u32BottomWidth),
                     "chn Border bpx. default(2).", NULL, 0, 0),
        OPT_INTEGER('\0', "disp_x", &(ctx.s32X),
                     "disp_x. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "disp_y", &(ctx.s32Y),
                    "disp_y. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "video_format", &(ctx.s32PixFormat),
                     "video pixel format."
                     "e.g.(0: ARGB8888 1: ABGR888 2: RGB888 3: BGR888 4: RK_FMT_YUV420SP)."
                     "default(5. 5 is RGB888).", NULL, 0, 0),
        OPT_INTEGER('\0', "disp_width", &(ctx.u32DispWidth),
                     "dst width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "disp_height", &(ctx.u32DispHeight),
                     "dst height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "image0_width", &(ctx.u32Screen0ImageWidth),
                     "image0 width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "image0_height", &(ctx.u32Screen0ImageHeight),
                     "image0 height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "image1_width", &(ctx.u32Screen1ImageWidth),
                     "image1 width. e.g.(1024). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "image1_height", &(ctx.u32Screen1ImageHeight),
                     "image1 height. e.g.(768). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "disp0_width", &(ctx.u32Screen0DisplayWidth),
                     "dst width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "disp0_height", &(ctx.u32Screen0DisplayHeight),
                     "dst height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "disp1_width", &(ctx.u32Screen1DisplayWidth),
                     "dst width. e.g.(1024). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "disp1_height", &(ctx.u32Screen1DisplayHeight),
                     "dst height. e.g.(768). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "image_width", &(ctx.u32ImgeWidth),
                     "dst width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "image_height", &(ctx.u32ImageHeight),
                     "dst height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_width", &(ctx.u32WbcWidth),
                     "dst width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_height", &(ctx.u32WbcHeight),
                     "dst height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_compress", &(ctx.u32WbcCompressMode),
                     "wbc compress mode. default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_format", &(ctx.u32WbcPixFormat),
                     "wbc pixel format."
                     "e.g.(0: ARGB8888 1: ABGR888 2: RGB888 3: BGR888)."
                     "default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_type", &(ctx.u32WbcSourceType),
                     "wbc souce type. e.g.(0: dev 1: video default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_id", &(ctx.u32WbcSourceId),
                     "wbc souce id. default(0).", NULL, 0, 0),
        OPT_STRING('\0', "voplay",  &(ctx.bVoPlay),
                   "play video test, default(0): 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_STRING('\0', "bBorder",  &(ctx.bBorder),
                   "Border enable, default(0): 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_STRING('\0', "wbc_auto",  &(ctx.wbc_auto),
                   "wbc auto bind, default(1): 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "screen0_chn", &(ctx.u32Screen0Chn),
                    "the channel num of screen0. default(16)", NULL, 0, 0),
        OPT_INTEGER('\0', "chn_display", &(ctx.u32ChnDismode),
                     "the chn dispaly mode."
                     "e.g.(0: normol 1: pause 2: step 3: speed).default(0).", NULL, 0, 0),
        OPT_INTEGER('\0', "screen1_chn", &(ctx.u32Screen1Chn),
                    "the channel num of screen1 default(4)", NULL, 0, 0),
        OPT_INTEGER('\0', "screen0_rows", &(ctx.stVoCfg.u32Screen0Rows),
                    "the rows/cols of screen0 display. default(4: 4x4)", NULL, 0, 0),
        OPT_INTEGER('\0', "screen1_rows", &(ctx.stVoCfg.u32Screen1Rows),
                    "the rows/cols of screen1 display.default(3 : 3x3)", NULL, 0, 0),
        OPT_INTEGER('\0', "en_wbc", &(ctx.bEnWbc),
                    "enable wbc. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "en_chnPriority", &(ctx.bChnPriority),
                    "enable Chn Priority. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "wbc_src",  &(ctx.stWbcCfg.stWbcSource.u32SourceId),
                   "the source of wbc, default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "double_screen",  &(ctx.bDoubleScreen),
                   "double screen or not, default(1): 0: FALSE, 1: TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "Homologous",  &(ctx.bHomologous),
                   "Homologous Display, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "mirror",  &(ctx.enMirror),
                   "vo chn set mirror function, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "rotation",  &(ctx.enRotation),
                   "vo chn set rotation function, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "rga",  &(ctx.bUseRga),
                   "use rga vo composer, default(0)", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);

    mpi_vo_test_show_options(&ctx);

    if (check_options(&ctx)) {
        RK_LOGE("illegal input parameters");
        argparse_usage(&argparse);
        goto __FAILED;
    }

    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        goto __FAILED;
    }

    if (ctx.wbc_enable) {
        Sample_VO_Video_Homologous(&ctx);
    } else if (ctx.ui_alpha) {
        Sample_VO_Demo_Alpha(&ctx);
    } else if (ctx.ui) {
        Sample_VO_Demo_UI(&ctx);
    } else if (ctx.bVoPlay) {
         //unit_mpi_voplay_test(&ctx);
    } else {
        Sample_VO_Demo_Video_Mosaic(&ctx);
    }

    Sample_Print("test running ok.");

    return RK_SUCCESS;
__FAILED:
    RK_LOGE("test running failed!");
    return RK_FAILURE;
}
