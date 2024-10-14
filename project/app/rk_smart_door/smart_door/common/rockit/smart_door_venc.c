// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"

typedef struct RKCodecExtension{
    RK_CODEC_ID_E enRkCodecId;
    char extension[16];
} CODEC_EXTEN;

static CODEC_EXTEN gCodecExtenMapList[] = {
    { RK_VIDEO_ID_MPEG1VIDEO,   "mpeg1video" },
    { RK_VIDEO_ID_MPEG2VIDEO,   "mpeg2video" },
    { RK_VIDEO_ID_H263,         "h263"  },
    { RK_VIDEO_ID_MPEG4,        "mpeg4"  },
    { RK_VIDEO_ID_WMV,          "wmv3" },
    { RK_VIDEO_ID_AVC,          "h264" },
    { RK_VIDEO_ID_MJPEG,        "mjpeg" },
    { RK_VIDEO_ID_VP8,          "vp8" },
    { RK_VIDEO_ID_VP9,          "vp9" },
    { RK_VIDEO_ID_HEVC,         "h265" },
    { RK_VIDEO_ID_VC1,          "vc1" },
    { RK_VIDEO_ID_AVS,          "avs" },
    { RK_VIDEO_ID_AVSPLUS,      "avs+" },
    { RK_VIDEO_ID_FLV1,         "flv1" },
};

static char* smart_door_venc_get_extension(RK_CODEC_ID_E enRkCodecId) {
    RK_U32 i;

    for (i = 0; i < (sizeof(gCodecExtenMapList) / sizeof(gCodecExtenMapList[0])); i++) {
        if (enRkCodecId == gCodecExtenMapList[i].enRkCodecId) {
            return gCodecExtenMapList[i].extension;
        }
    }
    return "unknow";
}

static void smart_door_venc_print_stream(RK_U32 chn, RK_S32 cnt, RK_CODEC_ID_E enType, VENC_STREAM_S stFrame) {
    switch (enType) {
      case RK_VIDEO_ID_AVC:
        RK_LOGD("venc get chn %d stream %d refType %d", chn, cnt, stFrame.stH264Info.enRefType);
        break;
      case RK_VIDEO_ID_HEVC:
       RK_LOGD("venc get chn %d stream %d refType %d", chn, cnt, stFrame.stH265Info.enRefType);
        break;
      default:
        RK_LOGD("venc get chn %d stream %d", chn, cnt);
        break;
    }

    return;
}

void* smart_door_venc_get_stream(void *pArgs) {
    SMART_DOOR_VENC_CFG *stVencCfg = (SMART_DOOR_VENC_CFG *)pArgs;
    void            *pData      = RK_NULL;
    RK_S32           s32Ret     = RK_SUCCESS;
    FILE            *fp         = RK_NULL;
    char             name[256]  = {0};
    RK_U32           u32Ch      = stVencCfg->u32ChnId;
    RK_S32           s32StreamCnt = 0;
    VENC_STREAM_S    stFrame;
    struct timeval time1, time2;
    float use_time;

    if (stVencCfg->dstFilePath != RK_NULL) {
        mkdir(stVencCfg->dstFilePath, 0777);

        snprintf(name, sizeof(name), "%s/venc_output_%d.%s",
            stVencCfg->dstFilePath, u32Ch, smart_door_venc_get_extension((RK_CODEC_ID_E)stVencCfg->u32DstCodec));

        fp = fopen(name, "wb");
        if (fp == RK_NULL) {
            RK_LOGE("chn %d can't open file %s in get stream thread!\n", u32Ch, name);
            return RK_NULL;
        }
    }
    stFrame.pstPack = (VENC_PACK_S *)(malloc(sizeof(VENC_PACK_S)));
    gettimeofday(&time1, NULL);

    while (!stVencCfg->threadExit) {
        s32Ret = RK_MPI_VENC_GetStream(u32Ch, &stFrame, 3000);
        if (s32Ret >= 0) {
            s32StreamCnt++;
            smart_door_venc_print_stream(u32Ch, s32StreamCnt, (RK_CODEC_ID_E)stVencCfg->u32DstCodec, stFrame);
            if (stVencCfg->dstFilePath != RK_NULL) {
                pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
                fwrite(pData, 1, stFrame.pstPack->u32Len, fp);
                fflush(fp);
            }
            if (stVencCfg->sendRtsp == RK_TRUE) {
                pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
                smart_door_rtsp_send_live0(pData, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
            }
            RK_MPI_VENC_ReleaseStream(u32Ch, &stFrame);
            if (stVencCfg->bChangeResolution == RK_TRUE) {
                stVencCfg->change_res_func(stVencCfg->u32ResWidth, stVencCfg->u32ResHeight);
                stVencCfg->bChangeResolution = RK_FALSE;
            }
            if (stFrame.pstPack->bStreamEnd == RK_TRUE) {
                gettimeofday(&time2, NULL);
                use_time = (time2.tv_sec - time1.tv_sec) + (float)(time2.tv_usec - time1.tv_usec) / 1000000;
                RK_LOGI("s32StreamCnt %d, use_time %f", s32StreamCnt, use_time);
                RK_LOGI("venc chn %d reach EOS stream, frame rate is %d fps.", u32Ch, (RK_S32)(s32StreamCnt / use_time));
                break;
            }
            if (stVencCfg->loopCount < 0) {
                RK_LOGI("venc exit!");
                break;
            }
            if (stVencCfg->loopCount > 0 && s32StreamCnt == stVencCfg->loopCount) {
                RK_LOGI("venc get loopCount=%d stream!", stVencCfg->loopCount);
                break;
            }
        } else {
            RK_LOGI("Not get stream, timeout!");
            stVencCfg->threadExit = RK_TRUE;
            if (stVencCfg->threadExit) {
                break;
            }

            usleep(1000llu);
        }

    }

    if (stFrame.pstPack)
        free(stFrame.pstPack);

    if (fp)
        fclose(fp);

    stVencCfg->vencTaskEnd = RK_TRUE;
    RK_LOGI("%s out", __FUNCTION__);
    return RK_NULL;
}

static RK_S32 VENC_SET_GopSize(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32GopSize) {
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

static RK_S32 VENC_SET_BitRate(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32BitRate) {
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
        break;
      case VENC_RC_MODE_H264VBR:
        pRcAttr->stH264Vbr.u32BitRate = u32BitRate;
        break;
      case VENC_RC_MODE_H265VBR:
        pRcAttr->stH265Vbr.u32BitRate = u32BitRate;
        break;
      case VENC_RC_MODE_H264AVBR:
        pRcAttr->stH264Avbr.u32BitRate = u32BitRate;
        break;
      case VENC_RC_MODE_H265AVBR:
        pRcAttr->stH265Avbr.u32BitRate = u32BitRate;
        break;
      default:
        return RK_ERR_VENC_NOT_SUPPORT;
    }

    return RK_SUCCESS;
}

static RK_S32 VENC_SET_FixQp(VENC_RC_ATTR_S *pRcAttr, RK_U32 u32FixIQp,
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

RK_S32 smart_door_init_venc(SMART_DOOR_VENC_CFG *stVencCfg) {
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;
    MB_POOL_CONFIG_S stMbPoolCfg;

    stVencCfg->bForceIdr = RK_FALSE;
    stVencCfg->bFrameRate = RK_TRUE;
    stVencCfg->bFrmLost = RK_FALSE;
    stVencCfg->bFullRange = RK_TRUE;
    stVencCfg->bHierarchicalQp = RK_FALSE;
    stVencCfg->bInsertUserData = RK_FALSE;
    stVencCfg->bIntraRefresh = RK_FALSE;
    stVencCfg->bMjpegParam = RK_FALSE;
    stVencCfg->enCompressMode = COMPRESS_MODE_NONE;
    stVencCfg->enCropType = VENC_CROP_NONE;
    stVencCfg->enMirror = MIRROR_NONE;
    stVencCfg->enRotation = ROTATION_0;
    stVencCfg->s32SnapPicCount = -1;
    stVencCfg->enSrcPixFormat = RK_FMT_YUV420SP;
    stVencCfg->u32StreamBufCnt = 2;

    stPicBufAttr.u32Width = stVencCfg->u32SrcWidth;
    stPicBufAttr.u32Height = stVencCfg->u32SrcHeight;
    stPicBufAttr.enPixelFormat = stVencCfg->enSrcPixFormat;
    stPicBufAttr.enCompMode = stVencCfg->enCompressMode;
    s32Ret = RK_MPI_CAL_COMM_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
        return s32Ret;
    }

    if (stVencCfg->u32BufferSize == 0)
        stVencCfg->u32BufferSize = stMbPicCalResult.u32MBSize;
    RK_LOGD("calc picture size: %d", stVencCfg->u32BufferSize);

    if (stVencCfg->u32BufferSize > 8192 * 8192 * 4) {
        RK_LOGE("too large picture size: %d", stVencCfg->u32BufferSize);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 smart_door_create_venc(SMART_DOOR_VENC_CFG stVencCfg) {
    RK_S32                  s32Ret = RK_SUCCESS;
    VENC_CHN_ATTR_S         stAttr;
    VENC_CHN_PARAM_S        stParam;
    VENC_RECV_PIC_PARAM_S   stRecvParam;
    VENC_RC_PARAM_S         stRcParam;
    pthread_t               getStreamThread[VENC_MAX_CHN_NUM];
    pthread_t               forceIdrThread[VENC_MAX_CHN_NUM];

#if VIDEO_CHN0_WORKING
    if (stVencCfg.u32ChnId == 0) {
        RK_LOGI("%s skip for channel %d", __func__, stVencCfg.u32ChnId);
        return RK_SUCCESS;
    }
#endif

    memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
    memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
    memset(&stRcParam, 0, sizeof(VENC_RC_PARAM_S));
    memset(&stParam, 0, sizeof(VENC_CHN_PARAM_S));

    if (stVencCfg.u32GopSize == 0) {
        stVencCfg.u32GopSize = 60;
    }

    stAttr.stRcAttr.enRcMode = stVencCfg.enRcMode;
    VENC_SET_GopSize(&stAttr.stRcAttr, stVencCfg.u32GopSize);
    VENC_SET_BitRate(&stAttr.stRcAttr, stVencCfg.u32BitRateKb);
    VENC_SET_FixQp(&stAttr.stRcAttr, stVencCfg.u32FixQp, stVencCfg.u32FixQp + 2, stVencCfg.u32FixQp);

    stAttr.stVencAttr.enType = (RK_CODEC_ID_E)stVencCfg.u32DstCodec;
    stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
    stAttr.stVencAttr.enPixelFormat = (PIXEL_FORMAT_E)stVencCfg.enSrcPixFormat;
    stAttr.stVencAttr.u32PicWidth = stVencCfg.u32SrcWidth;
    stAttr.stVencAttr.u32PicHeight = stVencCfg.u32SrcHeight;
    stAttr.stVencAttr.enMirror = stVencCfg.enMirror;
    stAttr.stVencAttr.u32VirWidth = stVencCfg.u32srcVirWidth;
    stAttr.stVencAttr.u32VirHeight = stVencCfg.u32srcVirHeight;
    stAttr.stVencAttr.u32StreamBufCnt = stVencCfg.u32StreamBufCnt;
    stAttr.stVencAttr.u32BufSize = stVencCfg.u32BufferSize;

    if (stAttr.stVencAttr.enType == RK_VIDEO_ID_JPEG) {
        stAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
        stAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
        stAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
    }

    RK_MPI_VENC_CreateChn(stVencCfg.u32ChnId, &stAttr);

    if (stVencCfg.u32DstCodec == RK_VIDEO_ID_AVC) {
        // RK_MPI_VENC_EnableMotionDeblur applies only to h264 and H265.
        // It will increase DMA memory by 64KB at 1080P.
        s32Ret = RK_MPI_VENC_EnableMotionDeblur(stVencCfg.u32ChnId, RK_TRUE);
        if (s32Ret)
            RK_LOGE("RK_MPI_VENC_EnableMotionDeblur error! ret=%#x\n", s32Ret);

        VENC_H264_TRANS_S pstH264Trans;
        RK_MPI_VENC_GetH264Trans(stVencCfg.u32ChnId, &pstH264Trans);
        pstH264Trans.bScalingListValid = 0;
        RK_MPI_VENC_SetH264Trans(stVencCfg.u32ChnId, &pstH264Trans);
    } else if (stVencCfg.u32DstCodec == RK_VIDEO_ID_HEVC) {
        s32Ret = RK_MPI_VENC_EnableMotionDeblur(stVencCfg.u32ChnId, RK_TRUE);
        if (s32Ret)
            RK_LOGE("RK_MPI_VENC_EnableMotionDeblur error! ret=%#x\n", s32Ret);

        VENC_H265_TRANS_S pstH265Trans;
        RK_MPI_VENC_GetH265Trans(stVencCfg.u32ChnId, &pstH265Trans);
        pstH265Trans.bScalingListEnabled = 0;
        RK_MPI_VENC_SetH265Trans(stVencCfg.u32ChnId, &pstH265Trans);
    }

    VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
    memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
    stVencChnRefBufShare.bEnable = 1;
    RK_MPI_VENC_SetChnRefBufShareAttr(stVencCfg.u32ChnId, &stVencChnRefBufShare);

    stParam.stCropCfg.enCropType = stVencCfg.enCropType;
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
        stParam.stCropCfg.stScaleRect.stSrc.u32Width = stVencCfg.u32SrcWidth / 2;
        stParam.stCropCfg.stScaleRect.stSrc.u32Height = stVencCfg.u32SrcHeight / 2;
        stParam.stCropCfg.stScaleRect.stDst.s32X = 0;
        stParam.stCropCfg.stScaleRect.stDst.s32Y = 0;
        stParam.stCropCfg.stScaleRect.stDst.u32Width = 200;
        stParam.stCropCfg.stScaleRect.stDst.u32Height = 200;
    }
    // for framerate test
    {
        stParam.stFrameRate.bEnable = stVencCfg.bFrameRate;
        stParam.stFrameRate.s32SrcFrmRateNum = 25;
        stParam.stFrameRate.s32SrcFrmRateDen = 1;
        stParam.stFrameRate.s32DstFrmRateNum = 25;
        stParam.stFrameRate.s32DstFrmRateDen = 1;
    }
    RK_MPI_VENC_SetChnParam(stVencCfg.u32ChnId, &stParam);

    if (stVencCfg.enGopMode) {
        stAttr.stGopAttr.enGopMode = stVencCfg.enGopMode;
        stAttr.stGopAttr.s32VirIdrLen = stVencCfg.u32GopSize / 2;
        RK_MPI_VENC_SetChnAttr(stVencCfg.u32ChnId, &stAttr);
    }

    if (stVencCfg.enRotation) {
        RK_MPI_VENC_SetChnRotation(stVencCfg.u32ChnId, stVencCfg.enRotation);
    }

    stRecvParam.s32RecvPicNum = stVencCfg.s32SnapPicCount;
    RK_MPI_VENC_StartRecvFrame(stVencCfg.u32ChnId, &stRecvParam);

    if (stAttr.stVencAttr.enType == RK_VIDEO_ID_JPEG) {
        VENC_JPEG_PARAM_S stJpegParam;
        memset(&stJpegParam, 0, sizeof(stJpegParam));
        stJpegParam.u32Qfactor = 77;
        RK_MPI_VENC_SetJpegParam(stVencCfg.u32ChnId, &stJpegParam);
    }

    for (RK_U32 i = 0; i < stVencCfg.u32RoiTestCount; i++) {
        VENC_ROI_ATTR_S stRoiAttr;
        stRoiAttr.u32Index = i;
        stRoiAttr.bEnable = RK_TRUE;
        stRoiAttr.bAbsQp = i % 2 ? RK_FALSE : RK_TRUE;
        stRoiAttr.s32Qp = i + 42;
        stRoiAttr.bIntra = i % 2 ? RK_TRUE : RK_FALSE;
        stRoiAttr.stRect.s32X = RK_ALIGN_16((i + 1) * 16);
        stRoiAttr.stRect.s32Y = RK_ALIGN_16((i + 1) * 16);
        stRoiAttr.stRect.u32Width = RK_ALIGN_16(stVencCfg.u32SrcWidth / (i + 2));
        stRoiAttr.stRect.u32Height = RK_ALIGN_16(stVencCfg.u32SrcHeight / (i + 2));
        RK_MPI_VENC_SetRoiAttr(stVencCfg.u32ChnId, &stRoiAttr);
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
    RK_MPI_VENC_SetRcParam(stVencCfg.u32ChnId, &stRcParam);
#endif

    if (!stVencCfg.bFullRange) {
        if (stAttr.stVencAttr.enType == RK_VIDEO_ID_AVC) {
            VENC_H264_VUI_S stH264Vui;
            RK_MPI_VENC_GetH264Vui(stVencCfg.u32ChnId, &stH264Vui);
            stH264Vui.stVuiVideoSignal.video_full_range_flag = 0;
            RK_MPI_VENC_SetH264Vui(stVencCfg.u32ChnId, &stH264Vui);
        } else if (stAttr.stVencAttr.enType == RK_VIDEO_ID_HEVC) {
            VENC_H265_VUI_S stH265Vui;
            RK_MPI_VENC_GetH265Vui(stVencCfg.u32ChnId, &stH265Vui);
            stH265Vui.stVuiVideoSignal.video_full_range_flag = 0;
            RK_MPI_VENC_SetH265Vui(stVencCfg.u32ChnId, &stH265Vui);
        }
    }

    if (stVencCfg.bSuperFrm) {
        VENC_SUPERFRAME_CFG_S stSuperFrameCfg;
        memset(&stSuperFrameCfg, 0, sizeof(stSuperFrameCfg));
        stSuperFrameCfg.enSuperFrmMode = SUPERFRM_DISCARD;
        stSuperFrameCfg.u32SuperIFrmBitsThr = 100 * 1024 * 8;  // 100KByte
        stSuperFrameCfg.u32SuperPFrmBitsThr = 20 * 1024 * 8;  // 20KByte
        stSuperFrameCfg.enRcPriority = VENC_RC_PRIORITY_FRAMEBITS_FIRST;
        RK_MPI_VENC_SetSuperFrameStrategy(stVencCfg.u32ChnId, &stSuperFrameCfg);
    }

    if (stVencCfg.bIntraRefresh) {
        VENC_INTRA_REFRESH_S stIntraRefresh;
        memset(&stIntraRefresh, 0, sizeof(stIntraRefresh));
        stIntraRefresh.bRefreshEnable = RK_TRUE;
        stIntraRefresh.enIntraRefreshMode = INTRA_REFRESH_ROW;
        stIntraRefresh.u32RefreshNum = 5;
        RK_MPI_VENC_SetIntraRefresh(stVencCfg.u32ChnId, &stIntraRefresh);
    }

    if (stVencCfg.bHierarchicalQp) {
        VENC_HIERARCHICAL_QP_S stHierarchicalQp;
        memset(&stHierarchicalQp, 0, sizeof(stHierarchicalQp));
        stHierarchicalQp.bHierarchicalQpEn = RK_TRUE;
        stHierarchicalQp.s32HierarchicalQpDelta[0] = -10;
        stHierarchicalQp.s32HierarchicalQpDelta[1] = -5;
        stHierarchicalQp.s32HierarchicalFrameNum[0] = 2;
        stHierarchicalQp.s32HierarchicalFrameNum[1] = 3;
        RK_MPI_VENC_SetHierarchicalQp(stVencCfg.u32ChnId, &stHierarchicalQp);
    }

    if (stVencCfg.bMjpegParam && stAttr.stVencAttr.enType == RK_VIDEO_ID_MJPEG) {
        VENC_MJPEG_PARAM_S stMjpegParam;
        memset(&stMjpegParam, 0, sizeof(stMjpegParam));
        // set qtable for test only, users need set actual qtable
        memset(&stMjpegParam.u8YQt, 1, sizeof(stMjpegParam.u8YQt));
        memset(&stMjpegParam.u8CbQt, 1, sizeof(stMjpegParam.u8CbQt));
        memset(&stMjpegParam.u8CrQt, 1, sizeof(stMjpegParam.u8CrQt));
        stMjpegParam.u32MCUPerECS = 100;
        RK_MPI_VENC_SetMjpegParam(stVencCfg.u32ChnId, &stMjpegParam);
    }

    if (stVencCfg.bFrmLost) {
        VENC_FRAMELOST_S stFrmLost;
        memset(&stFrmLost, 0, sizeof(stFrmLost));
        stFrmLost.bFrmLostOpen = RK_TRUE;
        stFrmLost.u32FrmLostBpsThr = 1;
        stFrmLost.enFrmLostMode = FRMLOST_NORMAL;
        stFrmLost.u32EncFrmGaps = 0;
        RK_MPI_VENC_SetFrameLostStrategy(stVencCfg.u32ChnId, &stFrmLost);
    }

    return RK_SUCCESS;
}

RK_S32 smart_door_start_venc(SMART_DOOR_VENC_CFG *stVencCfg) {
    pthread_create(&stVencCfg->pGetStreamThread, 0, stVencCfg->venc_func, (void *)stVencCfg);
    return RK_SUCCESS;
}

RK_S32 smart_door_destroy_venc(SMART_DOOR_VENC_CFG stVencCfg) {
    RK_S32 s32Ret;
    RK_U32 u32Ch = stVencCfg.u32ChnId;

    if (stVencCfg.pGetStreamThread)
        pthread_join(stVencCfg.pGetStreamThread, RK_NULL);

#if VIDEO_CHN0_WORKING
    if (stVencCfg.u32ChnId == 0) {
        RK_LOGI("%s skip for channel %d", __func__, stVencCfg.u32ChnId);
        return RK_SUCCESS;
    }
#endif

    s32Ret = RK_MPI_VENC_StopRecvFrame(u32Ch);
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    RK_LOGD("destroy enc chn:%d", u32Ch);
    s32Ret = RK_MPI_VENC_DestroyChn(u32Ch);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
        return s32Ret;
    }

    if (stVencCfg.vencPool)
        RK_MPI_MB_DestroyPool(stVencCfg.vencPool);

    return RK_SUCCESS;
}