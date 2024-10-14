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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_AO_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_AO_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "rk_comm_avs.h"
#include "rk_comm_mb.h"
#include "rk_comm_video.h"

#ifdef MAX_FILE_NAME_LEN
#undef MAX_FILE_NAME_LEN
#define MAX_FILE_NAME_LEN          64
#else
#define MAX_FILE_NAME_LEN          64
#endif

#ifdef MAX_FILE_PATH_LEN
#undef MAX_FILE_PATH_LEN
#define MAX_FILE_PATH_LEN          256
#else
#define MAX_FILE_PATH_LEN          256
#endif

#define AVS_GET_CHN_FRAME_TIMEOUT_MS 200
#define VO_RGA                       0

#ifdef ENABLE_COM_POOL
#undef ENABLE_COM_POOL
#define ENABLE_COM_POOL              0
#else
#define ENABLE_COM_POOL              0
#endif

typedef enum rkTEST_MODE_E {
    TEST_MODE_AVS_ONLY       = 0,
    TEST_MODE_AVS_VO         = 1,
    TEST_MODE_AVS_VPSS_VENC  = 2,
    TEST_MODE_VI_AVS_VO      = 3,
    TEST_MODE_VI_AVS_VPSS_VENC  = 4,
} TEST_MODE_E;

typedef struct _rkMpiAvsCtx {
    const RK_CHAR        *srcFilePath;
    const RK_CHAR        *dstFilePath;
    RK_S32                s32LoopCount;
    RK_S32                s32GrpNum;
    RK_S32                s32PipeNum;
    RK_S32                s32ChnNum;

    RK_U32                u32SrcWidth;
    RK_U32                u32SrcHeight;
    RK_U32                u32SrcVirWidth;
    RK_U32                u32SrcVirHeight;
    PIXEL_FORMAT_E        enSrcPixFormat;
    COMPRESS_MODE_E       enSrcCompressMode;

    RK_U32                u32DstWidth;
    RK_U32                u32DstHeight;
    PIXEL_FORMAT_E        enDstPixFormat;
    COMPRESS_MODE_E       enDstCompressMode;
    RK_U32                u32ChnDepth;
    RK_U32                u32FrameBufCnt;
    RK_S32                s32SrcChnRate;
    RK_S32                s32DstChnRate;

    TEST_MODE_E           enTestMode;
    AVS_MODE_E            enAvsWorkMode;
    AVS_PARAM_SOURCE_E    enParamSource;
    AVS_PROJECTION_MODE_E enAvsProjMode;

    RK_BOOL               bFrameSync;
    AVS_ROTATION_S        stOriRotation;
    AVS_ROTATION_S        stRotation;
    AVS_FOV_S             stFov;
    POINT_S               stCenter;
    AVS_LUT_ACCURACY_E    enLutAccuracy;
    AVS_FUSE_WIDTH_E      enLutFuseWidth;
    AVS_STEP_ATTR_S       stLutStep;
    const RK_CHAR        *lutFilePath;
    const RK_CHAR        *calibFileName;
    const RK_CHAR        *meshFilePath;
    RK_FLOAT              fDistance;
    RK_S32                s32SrcGrpRate;
    RK_S32                s32DstGrpRate;
    RK_S32                s32GrpIndex;
    RK_BOOL               bCommPool;
    MB_CONFIG_S           stMbConfig;
    MB_POOL               srcVFramePools[AVS_PIPE_NUM];
    RK_BOOL               bGrpExSize;
    RK_U32                u32GrpWidth;
    RK_U32                u32GrpHeight;
    RK_BOOL               bGetFinalLut;
} TEST_AVS_CTX_S;

RK_U32 TEST_AVS_GetFileLen(const RK_CHAR* File);
RK_S32 TEST_AVS_LoadFile(const RK_CHAR* File, RK_VOID* Addr, RK_U32 u32Size);

RK_S32 TEST_AVS_Start(AVS_GRP AvsGrp, RK_U32 u32ChnNum,
                        AVS_GRP_ATTR_S *pstAvsGrpAttr,
                        AVS_CHN_ATTR_S *pstAvsChnAttr);
RK_S32 TEST_AVS_Stop(AVS_GRP AvsGrp, RK_U32 u32ChnNum);

RK_S32 TEST_AVS_ModCreateFrame(TEST_AVS_CTX_S *pstCtx,
                               VIDEO_FRAME_INFO_S **pstVideoFramess);

RK_S32 TEST_AVS_ModSendFrame(AVS_GRP AvsGrp,
                             RK_U32 u32PipeNum,
                             VIDEO_FRAME_INFO_S **pstVideoFrames);

RK_S32 TEST_AVS_ModGetChnFrame(AVS_GRP AvsGrp,
                               RK_U32 u32ChnNum,
                               VIDEO_FRAME_INFO_S **pstVideoFrames);

RK_S32 TEST_AVS_ModSaveChnFrame(AVS_GRP AvsGrp,
                               RK_U32 u32ChnNum,
                               const RK_CHAR *dstFilePath,
                               VIDEO_FRAME_INFO_S **pstVideoFrames);

RK_S32 TEST_AVS_ModReleaseChnFrame(AVS_GRP AvsGrp,
                                   RK_U32 u32ChnNum,
                                   VIDEO_FRAME_INFO_S **pstVideoFrames);

RK_S32 TEST_AVS_ModInit(TEST_AVS_CTX_S *pstCtx);

RK_S32 TEST_AVS_ModDeInit(TEST_AVS_CTX_S *pstCtx);

RK_S32 TEST_AVS_UnitTest(TEST_AVS_CTX_S *pstCtx);

RK_S32 TEST_AVS_InitAttr(TEST_AVS_CTX_S *pstCtx,
                       AVS_GRP_ATTR_S *pstAvsGrpAttr,
                       AVS_CHN_ATTR_S *pstAvsChnAttrs);

RK_S32 TEST_AVS_InitAttr_Blend_Lut(TEST_AVS_CTX_S *pstCtx,
                                    AVS_GRP_ATTR_S *pstAvsGrpAttr,
                                    AVS_CHN_ATTR_S *pstAvsChnAttrs);

RK_S32 TEST_AVS_InitAttr_Blend_Calib(TEST_AVS_CTX_S *pstCtx,
                                    AVS_GRP_ATTR_S *pstAvsGrpAttr,
                                    AVS_CHN_ATTR_S *pstAvsChnAttrs);

RK_S32 TEST_AVS_InitAttr_NoBlend(TEST_AVS_CTX_S *pstCtx,
                                AVS_GRP_ATTR_S *pstAvsGrpAttr,
                                AVS_CHN_ATTR_S *pstAvsChnAttrs);

RK_S32 TEST_AVS_MallocLutBuf(TEST_AVS_CTX_S *pstCtx, RK_VOID ***lutBufsVirAddr);
RK_S32 TEST_AVS_FreeLutBuf(AVS_GRP_ATTR_S *pstAvsGrpAttr);


RK_S32 TEST_AVS_ModCreateFramePool(TEST_AVS_CTX_S *pstCtx);
RK_S32 TEST_AVS_ModDestroyFramePool(TEST_AVS_CTX_S *pstCtx);

RK_S32 TEST_AVS_ModGetFrameFromPool(TEST_AVS_CTX_S *pstCtx,
                                VIDEO_FRAME_INFO_S **pstVideoFrames);
RK_S32 TEST_AVS_ModReleaseFrameToPool(TEST_AVS_CTX_S *pstCtx,
                                VIDEO_FRAME_INFO_S **pstVideoFrames);


RK_S32 TEST_AVS_ComChnSetScale(TEST_AVS_CTX_S *pstCtx);
RK_S32 TEST_AVS_ComGrpChnSetScale(TEST_AVS_CTX_S *pstCtx);

RK_S32 TEST_AVS_GetFinalLut(AVS_GRP AvsGrp, AVS_GRP_ATTR_S *pstAvsGrpAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_AO_H_
