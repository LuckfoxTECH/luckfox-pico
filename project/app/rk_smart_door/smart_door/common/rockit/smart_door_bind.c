// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"

/*
 * VI ---> VENC (RTSP or tuya)
 */
RK_S32 smart_door_pipe0_init(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_init_vi(&ctx->stViCfg[0]);
    if (s32Ret) {
        RK_LOGE("init vi failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_init_venc(&ctx->stVencCfg[0]);
    if (s32Ret) {
        RK_LOGE("init venc failed: %d", s32Ret);
        return s32Ret;
    }

    if (ctx->stVencCfg[0].sendRtsp == RK_TRUE)
        smart_door_rtsp_init();

    return s32Ret;
}

RK_S32 smart_door_pipe0_bind(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    MPP_CHN_S stViChn, stVencChn;

#if VIDEO_CHN0_WORKING
    if (ctx->stViCfg[0].channelId == 0) {
        RK_LOGI("%s skip for channel %d", __func__, ctx->stViCfg[0].channelId);
        return RK_SUCCESS;
    }
#endif

    stViChn.enModId = RK_ID_VI;
    stViChn.s32DevId = ctx->stViCfg[0].devId;
    stViChn.s32ChnId = ctx->stViCfg[0].channelId;

    stVencChn.enModId = RK_ID_VENC;
    stVencChn.s32DevId = 0;
    stVencChn.s32ChnId = ctx->stVencCfg[0].u32ChnId;

    s32Ret = RK_MPI_SYS_Bind(&stViChn, &stVencChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind %d channel vi and venc failed", stViChn.s32ChnId);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_pipe0_unbind(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    MPP_CHN_S stViChn, stVencChn;

#if VIDEO_CHN0_WORKING
    if (ctx->stViCfg[0].channelId == 0) {
        RK_LOGI("%s skip for channel %d", __func__, ctx->stViCfg[0].channelId);
        return RK_SUCCESS;
    }
#endif

    stViChn.enModId = RK_ID_VI;
    stViChn.s32DevId = ctx->stViCfg[0].devId;
    stViChn.s32ChnId = ctx->stViCfg[0].channelId;

    stVencChn.enModId = RK_ID_VENC;
    stVencChn.s32DevId = 0;
    stVencChn.s32ChnId = ctx->stVencCfg[0].u32ChnId;

    s32Ret = RK_MPI_SYS_UnBind(&stViChn, &stVencChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind %d channel vi and venc failed", stViChn.s32ChnId);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_pipe0_create(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    smart_door_create_vi(ctx->stViCfg[0]);
    smart_door_create_venc(ctx->stVencCfg[0]);

    s32Ret = smart_door_pipe0_bind(ctx);
    if (s32Ret) {
        RK_LOGE("bind vi venc failed: %d", s32Ret);
        return s32Ret;
    }

    smart_door_start_venc(&ctx->stVencCfg[0]);

    return s32Ret;
}

RK_S32 smart_door_pipe0_destroy(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    while (ctx->stVencCfg[0].vencTaskEnd != RK_TRUE) {
        usleep(1000llu);
    }

    s32Ret = smart_door_pipe0_unbind(ctx);
    if (s32Ret) {
        RK_LOGE("unbind vi venc failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_destroy_vi(ctx->stViCfg[0]);
    if (s32Ret) {
        RK_LOGE("destroy vi failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_destroy_venc(ctx->stVencCfg[0]);
    if (s32Ret) {
        RK_LOGE("destroy venc failed: %d", s32Ret);
        return s32Ret;
    }

    if (ctx->stVencCfg[0].sendRtsp == RK_TRUE)
        smart_door_rtsp_deinit();

    return s32Ret;
}

RK_S32 smart_door_pipe0_set_resolution(int width, int height) {
    RK_S32 s32Ret = RK_FAILURE;

    RK_LOGI("set resolution, width is %d, height is %d\n", width, height);

    if (!ctx) {
        RK_LOGE("error! ctx is null when set resolution.\n");
        return s32Ret;
    }

    // unbind
    MPP_CHN_S stViChn, stVencChn;

    stViChn.enModId = RK_ID_VI;
    stViChn.s32DevId = ctx->stViCfg[0].devId;
    stViChn.s32ChnId = ctx->stViCfg[0].channelId;

    stVencChn.enModId = RK_ID_VENC;
    stVencChn.s32DevId = 0;
    stVencChn.s32ChnId = ctx->stVencCfg[0].u32ChnId;

    s32Ret = RK_MPI_SYS_UnBind(&stViChn, &stVencChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind %d channel vi and venc failed", stViChn.s32ChnId);
        return s32Ret;
    }

    // set venc
    VENC_CHN_ATTR_S venc_chn_attr;
    RK_MPI_VENC_GetChnAttr(ctx->stVencCfg[0].u32ChnId, &venc_chn_attr);
    venc_chn_attr.stVencAttr.u32PicWidth = width;
    venc_chn_attr.stVencAttr.u32PicHeight = height;
    venc_chn_attr.stVencAttr.u32VirWidth = width;
    venc_chn_attr.stVencAttr.u32VirHeight = height;
    s32Ret = RK_MPI_VENC_SetChnAttr(ctx->stVencCfg[0].u32ChnId, &venc_chn_attr);
    if (s32Ret) {
        RK_LOGE("RK_MPI_VENC_SetChnAttr error! ret=%#x\n", s32Ret);
        return s32Ret;
    }

    // set vi
    VI_CHN_ATTR_S vi_chn_attr;
    RK_MPI_VI_GetChnAttr(ctx->stViCfg[0].pipeId, ctx->stViCfg[0].channelId, &vi_chn_attr);
    vi_chn_attr.stSize.u32Width = width;
    vi_chn_attr.stSize.u32Height = height;
    s32Ret = RK_MPI_VI_SetChnAttr(ctx->stViCfg[0].pipeId, ctx->stViCfg[0].channelId, &vi_chn_attr);
    if (s32Ret) {
        RK_LOGE("RK_MPI_VI_SetChnAttr error! ret=%#x\n", s32Ret);
        return s32Ret;
    }

    // bind
    stViChn.enModId = RK_ID_VI;
    stViChn.s32DevId = ctx->stViCfg[0].devId;
    stViChn.s32ChnId = ctx->stViCfg[0].channelId;

    stVencChn.enModId = RK_ID_VENC;
    stVencChn.s32DevId = 0;
    stVencChn.s32ChnId = ctx->stVencCfg[0].u32ChnId;

    s32Ret = RK_MPI_SYS_Bind(&stViChn, &stVencChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind %d channel vi and venc failed", stViChn.s32ChnId);
        return s32Ret;
    }

    return s32Ret;
}

int smart_door_pipe_rtsp_start(void) {
    RK_S32 s32Ret = RK_SUCCESS;

    if (!ctx) {
        ctx = (SMART_DOOR_CTX_S *)(malloc(sizeof(SMART_DOOR_CTX_S)));
        if (!ctx) {
            RK_LOGE("malloc SMART_DOOR_CTX_S memory failed");
            return -1;
        }
        memset(ctx, 0, sizeof(SMART_DOOR_CTX_S));
    } else {
        memset(&ctx->stViCfg[0], 0, sizeof(ctx->stViCfg[0]));
        memset(&ctx->stVencCfg[0], 0, sizeof(ctx->stVencCfg[0]));
    }

    ctx->stViCfg[0].devId = 0;
    ctx->stViCfg[0].pipeId = ctx->stViCfg[0].devId;
    ctx->stViCfg[0].channelId = 0;
    memcpy(ctx->stViCfg[0].stChnAttr.stIspOpt.aEntityName, "rkisp_mainpath", strlen("rkisp_mainpath"));
    ctx->stViCfg[0].stChnAttr.stSize.u32Width = 1920;
    ctx->stViCfg[0].stChnAttr.stSize.u32Height = 1080;

    ctx->stVencCfg[0].u32ChnId = 0;
    ctx->stVencCfg[0].u32DstCodec = RK_VIDEO_ID_AVC;
    ctx->stVencCfg[0].u32SrcWidth = ctx->stViCfg[0].stChnAttr.stSize.u32Width;
    ctx->stVencCfg[0].u32SrcHeight = ctx->stViCfg[0].stChnAttr.stSize.u32Height;
    ctx->stVencCfg[0].u32srcVirWidth = ctx->stViCfg[0].stChnAttr.stSize.u32Width;
    ctx->stVencCfg[0].u32srcVirHeight = ctx->stViCfg[0].stChnAttr.stSize.u32Height;
    ctx->stVencCfg[0].enRcMode = VENC_RC_MODE_H264CBR;
    ctx->stVencCfg[0].u32BufferSize = ctx->stVencCfg[0].u32SrcWidth * ctx->stVencCfg[0].u32SrcHeight / 2;
    ctx->stVencCfg[0].u32BitRateKb = 2 * 1024;
    ctx->stVencCfg[0].u32FixQp = 0;// valid when rc_mode is fixqp
    ctx->stVencCfg[0].bSuperFrm = RK_FALSE;
    ctx->stVencCfg[0].enGopMode = VENC_GOPMODE_NORMALP;
    ctx->stVencCfg[0].u32GopSize = 60;
    ctx->stVencCfg[0].venc_func = smart_door_venc_get_stream;
    ctx->stVencCfg[0].loopCount = 0;
    ctx->stVencCfg[0].sendRtsp = RK_TRUE;
    //ctx->stVencCfg[0].dstFilePath = "/tmp/";

    s32Ret = smart_door_pipe0_init(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_pipe0_init failed: %d!", s32Ret);
        goto __FAILED;
    }

    s32Ret = smart_door_pipe0_create(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_pipe0_create failed: %d!", s32Ret);
        goto __FAILED;
    }

    s32Ret = smart_door_pipe0_destroy(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_pipe0_destroy failed: %d!", s32Ret);
        goto __FAILED;
    }

__FAILED:

    return s32Ret;
}

void smart_door_pipe_rtsp_stop(void) {
    ctx->stVencCfg[0].loopCount = -1;
}

void smart_door_pipe_rtsp_set_resolution(int width, int height) {
    ctx->stVencCfg[0].bChangeResolution = RK_TRUE;
    ctx->stVencCfg[0].u32ResWidth = width;
    ctx->stVencCfg[0].u32ResHeight = height;
    ctx->stVencCfg[0].change_res_func = smart_door_pipe0_set_resolution;
}

/*
 * AI ---> AENC
 */
RK_S32 smart_door_init_ai_aenc(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_init_ai(ctx);
    if (s32Ret) {
        RK_LOGE("init ai failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_init_aenc(ctx);
    if (s32Ret) {
        RK_LOGE("init aenc failed: %d", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_bind_ai_aenc(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId    = RK_ID_AI;
    stSrcChn.s32DevId   = ctx->stAiCfg.s32DevId;
    stSrcChn.s32ChnId   = ctx->stAiCfg.s32ChnIndex;

    stDestChn.enModId   = RK_ID_AENC;
    stDestChn.s32DevId  = ctx->stAencCfg.s32DevId;
    stDestChn.s32ChnId  = ctx->stAencCfg.u32ChnId;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind ai and aenc failed");
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_unbind_ai_aenc(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId    = RK_ID_AI;
    stSrcChn.s32DevId   = ctx->stAiCfg.s32DevId;
    stSrcChn.s32ChnId   = ctx->stAiCfg.s32ChnIndex;

    stDestChn.enModId   = RK_ID_AENC;
    stDestChn.s32DevId  = ctx->stAencCfg.s32DevId;
    stDestChn.s32ChnId  = ctx->stAencCfg.u32ChnId;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind ai and aenc failed");
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_create_ai_aenc(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    smart_door_create_ai(ctx->stAiCfg);
    smart_door_create_aenc(ctx->stAencCfg);
    s32Ret = smart_door_bind_ai_aenc(ctx);
    if (s32Ret) {
        RK_LOGE("bind ai and aenc failed: %d", s32Ret);
        return s32Ret;
    }
    smart_door_start_aenc(ctx);

    return s32Ret;
}

RK_S32 smart_door_destroy_ai_aenc(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    while (ctx->aencTaskEnd != RK_TRUE) {
        usleep(1000llu);
    }

    s32Ret = smart_door_unbind_ai_aenc(ctx);
    if (s32Ret) {
        RK_LOGE("unbind ai and aenc failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_destroy_ai(ctx->stAiCfg);
    if (s32Ret) {
        RK_LOGE("destroy ai failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_destroy_aenc(ctx);
    if (s32Ret) {
        RK_LOGE("destroy aenc failed: %d", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

int smart_door_pipe_ai_aenc(void) {
    SMART_DOOR_CTX_S *ctx;
    RK_S32 s32Ret = RK_SUCCESS;

    ctx = (SMART_DOOR_CTX_S *)(malloc(sizeof(SMART_DOOR_CTX_S)));
    if (!ctx) {
        RK_LOGE("malloc SMART_DOOR_CTX_S memory failed");
        return -1;
    }
    memset(ctx, 0, sizeof(SMART_DOOR_CTX_S));

    s32Ret = smart_door_init_ai_aenc(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_init_ai_aenc failed: %d!", s32Ret);
        goto __FAILED;
    }

    s32Ret = smart_door_create_ai_aenc(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_create_ai_aenc failed: %d!", s32Ret);
        goto __FAILED;
    }

    s32Ret = smart_door_destroy_ai_aenc(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_destroy_ai_aenc failed: %d!", s32Ret);
        goto __FAILED;
    }

__FAILED:
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }

    return s32Ret;
}

/*
 * ADEC ---> AO
 */
RK_S32 smart_door_init_adec_ao(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = smart_door_init_adec(ctx);
    if (s32Ret) {
        RK_LOGE("init adec failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_init_ao(ctx);
    if (s32Ret) {
        RK_LOGE("init ao failed: %d", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_bind_adec_ao(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId    = RK_ID_ADEC;
    stSrcChn.s32DevId   = ctx->stAdecCfg.s32DevId;
    stSrcChn.s32ChnId   = ctx->stAdecCfg.u32ChnId;

    stDestChn.enModId   = RK_ID_AO;
    stDestChn.s32DevId  = ctx->stAoCfg.s32DevId;
    stDestChn.s32ChnId  = ctx->stAoCfg.s32ChnIndex;

    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("bind adec and ao failed");
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_unbind_adec_ao(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_FAILURE;
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId    = RK_ID_ADEC;
    stSrcChn.s32DevId   = ctx->stAdecCfg.s32DevId;
    stSrcChn.s32ChnId   = ctx->stAdecCfg.u32ChnId;

    stDestChn.enModId   = RK_ID_AO;
    stDestChn.s32DevId  = ctx->stAoCfg.s32DevId;
    stDestChn.s32ChnId  = ctx->stAoCfg.s32ChnIndex;

    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("unbind adec and ao failed");
        return s32Ret;
    }

    return s32Ret;
}

RK_S32 smart_door_create_adec_ao(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    smart_door_create_adec(ctx->stAdecCfg);
    smart_door_create_ao(ctx->stAoCfg);
    s32Ret = smart_door_bind_adec_ao(ctx);
    if (s32Ret) {
        RK_LOGE("bind adec and ao failed: %d", s32Ret);
        return s32Ret;
    }

    smart_door_start_adec(ctx);

    return s32Ret;
}

RK_S32 smart_door_destroy_adec_ao(SMART_DOOR_CTX_S *ctx) {
    RK_S32 s32Ret = RK_SUCCESS;

    while (ctx->adecTaskEnd != RK_TRUE) {
        usleep(1000llu);
    }

    s32Ret = smart_door_unbind_adec_ao(ctx);
    if (s32Ret) {
        RK_LOGE("unbind adec and ao failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_destroy_adec(ctx);
    if (s32Ret) {
        RK_LOGE("destroy adec failed: %d", s32Ret);
        return s32Ret;
    }

    s32Ret = smart_door_destroy_ao(ctx->stAoCfg);
    if (s32Ret) {
        RK_LOGE("destroy ao failed: %d", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

int smart_door_pipe_adec_ao(void) {
    SMART_DOOR_CTX_S *ctx;
    RK_S32 s32Ret = RK_SUCCESS;

    ctx = (SMART_DOOR_CTX_S *)(malloc(sizeof(SMART_DOOR_CTX_S)));
    if (!ctx) {
        RK_LOGE("malloc SMART_DOOR_CTX_S memory failed");
        return -1;
    }
    memset(ctx, 0, sizeof(SMART_DOOR_CTX_S));

    s32Ret = smart_door_init_adec_ao(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_init_adec_ao failed: %d!", s32Ret);
        goto __FAILED;
    }

    s32Ret = smart_door_create_adec_ao(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_create_adec_ao failed: %d!", s32Ret);
        goto __FAILED;
    }

    s32Ret = smart_door_destroy_adec_ao(ctx);
    if (s32Ret) {
        RK_LOGI("smart_door_destroy_adec_ao failed: %d!", s32Ret);
        goto __FAILED;
    }

__FAILED:
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }

    return s32Ret;
}
