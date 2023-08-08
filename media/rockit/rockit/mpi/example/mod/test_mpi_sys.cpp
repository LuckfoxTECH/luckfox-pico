/* Copyright 2020 Rockchip Electronics Co. LTD
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

#ifdef DBG_MOD_ID
#undef DBG_MOD_ID
#endif
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_SYS)

#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_ao.h"
#include "rk_mpi_adec.h"

#include "test_comm_argparse.h"

typedef struct _rkTestSysCtx {
    RK_S32      s32LoopCount;
    RK_S32      s32DevId;
    RK_S32      s32SrcChnId;
    RK_S32      s32DstChnNum;

    ADEC_CHN_ATTR_S *pstADecChnAttr;
} TEST_SYS_CTX_S;

RK_S32 test_ao_dev_init(TEST_SYS_CTX_S *pstCtx) {
    AUDIO_DEV aoDevId = (AUDIO_DEV)pstCtx->s32DevId;
    AIO_ATTR_S aoAttr;

    memset(&aoAttr, 0, sizeof(AIO_ATTR_S));
    snprintf(reinterpret_cast<char *>(aoAttr.u8CardName),
                 sizeof(aoAttr.u8CardName), "%s", "hw:0,0");
    aoAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
    aoAttr.enSamplerate = AUDIO_SAMPLE_RATE_44100;
    aoAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
    aoAttr.u32FrmNum = 512;
    aoAttr.u32PtNumPerFrm = 1024;
    aoAttr.u32EXFlag = 0;
    aoAttr.u32ChnCnt = 2;

    RK_MPI_AO_SetPubAttr(aoDevId, &aoAttr);
    RK_MPI_AO_Enable(aoDevId);

    return RK_SUCCESS;
}

RK_S32 test_ao_dev_deinit(TEST_SYS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret =  RK_MPI_AO_Disable(pstCtx->s32DevId);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed to disable ao dev, err: %d", s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

ADEC_CHN_ATTR_S* test_adec_create_chn_attr() {
    ADEC_CHN_ATTR_S *pstChnAttr = reinterpret_cast<ADEC_CHN_ATTR_S *>
                                        (malloc(sizeof(ADEC_CHN_ATTR_S)));
    memset(pstChnAttr, 0, sizeof(ADEC_CHN_ATTR_S));
    ADEC_ATTR_CODEC_S *pstCodecAttr = &pstChnAttr->stCodecAttr;

    // attr of adec
    pstCodecAttr->u32Channels      = 1;
    pstCodecAttr->u32SampleRate    = 8000;
    pstCodecAttr->u32ExtraDataSize = 0;
    pstCodecAttr->u32BitPerCodedSample = 4;
    pstCodecAttr->pExtraData       = RK_NULL;

    // attr of chn
    pstChnAttr->enType          = RK_AUDIO_ID_ADPCM_G726;
    pstChnAttr->enMode          = ADEC_MODE_PACK;
    pstChnAttr->u32BufCount     = 4;
    pstChnAttr->u32BufSize      = 8192;

    return pstChnAttr;
}

void test_adec_destroy_chn_attr(ADEC_CHN_ATTR_S **ppstChnAttr) {
    ADEC_CHN_ATTR_S *pstChnAttr = *ppstChnAttr;
    if (pstChnAttr == RK_NULL) {
        return;
    }

    ADEC_ATTR_CODEC_S *pstCodecAttr = &pstChnAttr->stCodecAttr;
    if (pstCodecAttr->pExtraData != RK_NULL) {
        free(pstCodecAttr->pExtraData);
        pstCodecAttr->pExtraData = RK_NULL;
    }
    pstCodecAttr->u32ExtraDataSize = 0;

    free(pstChnAttr);
    *ppstChnAttr = RK_NULL;
}

RK_S32 test_adec_create_channel(TEST_SYS_CTX_S *pstCtx, RK_S32 s32ChnId) {
    RK_S32 s32Ret  = RK_SUCCESS;
    ADEC_CHN AdChn = (ADEC_CHN)s32ChnId;
    ADEC_CHN_ATTR_S *pstChnAttr = test_adec_create_chn_attr();
    s32Ret = RK_MPI_ADEC_CreateChn(AdChn, pstChnAttr);
    if (s32Ret) {
        RK_LOGE("failed to create adec chn %d, err %d", AdChn, s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 test_adec_destroy_channel(TEST_SYS_CTX_S *pstCtx, RK_S32 s32ChnId) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_S32 s32DevId = pstCtx->s32DevId;
    ADEC_CHN AdChn  = (ADEC_CHN)s32ChnId;

    s32Ret = RK_MPI_ADEC_DestroyChn(AdChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed to destroy adec channel(%d), err: %d", AdChn, s32Ret);
    }

    test_adec_destroy_chn_attr(&pstCtx->pstADecChnAttr);
    return s32Ret;
}

RK_S32 test_ao_enable_channel(TEST_SYS_CTX_S *pstCtx, RK_S32 s32ChnId) {
    RK_S32 s32Ret = RK_SUCCESS;
    ADEC_CHN AdChn  = (ADEC_CHN)s32ChnId;

    s32Ret = RK_MPI_AO_EnableChn(pstCtx->s32DevId, AdChn);
    if (s32Ret != 0) {
        RK_LOGE("failed to enable ao chn %d, err %d", AdChn, s32Ret);
        return RK_FAILURE;
    }

    return s32Ret;
}

RK_S32 test_ao_disable_channel(TEST_SYS_CTX_S *pstCtx, RK_S32 s32ChnId) {
    RK_S32 s32Ret = RK_SUCCESS;
    AO_CHN AoChn  = (AO_CHN)s32ChnId;

    s32Ret = RK_MPI_AO_DisableChn(pstCtx->s32DevId, AoChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed to disable ao channel(%d), err: %d", AoChn, s32Ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 test_bind_adec_ao(TEST_SYS_CTX_S *pstCtx, RK_S32 s32SrcChnId, RK_S32 s32DstChnId) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_S32 s32DevId = pstCtx->s32DevId;
    MPP_CHN_S stSrcChn, stDstChn;

    stSrcChn.enModId = RK_ID_ADEC;
    stSrcChn.s32DevId = s32DevId;
    stSrcChn.s32ChnId = s32SrcChnId;

    stDstChn.enModId = RK_ID_AO;
    stDstChn.s32DevId = s32DevId;
    stDstChn.s32ChnId = s32DstChnId;
    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDstChn);

    return s32Ret;
}

RK_S32 test_unbind_adec_ao(TEST_SYS_CTX_S *pstCtx, RK_S32 s32SrcChnId, RK_S32 s32DstChnId) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_S32 s32DevId = pstCtx->s32DevId;

    MPP_CHN_S stSrcChn, stDstChn;
    stSrcChn.enModId = RK_ID_ADEC;
    stSrcChn.s32DevId = s32DevId;
    stSrcChn.s32ChnId = s32SrcChnId;

    stDstChn.enModId = RK_ID_AO;
    stDstChn.s32DevId = s32DevId;
    stDstChn.s32ChnId = s32DstChnId;
    s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDstChn);

    return s32Ret;
}

RK_S32 test_mpi_sys_get_bind_by_src(TEST_SYS_CTX_S *pstCtx, RK_S32 s32ChnId) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_BIND_DEST_S pstBindDest;

    memset(&stSrcChn, 0, sizeof(MPP_CHN_S));
    memset(&pstBindDest, 0, sizeof(MPP_BIND_DEST_S));
    stSrcChn.enModId = RK_ID_ADEC;
    stSrcChn.s32DevId = pstCtx->s32DevId;
    stSrcChn.s32ChnId = s32ChnId;
    s32Ret = RK_MPI_SYS_GetBindbySrc(&stSrcChn, &pstBindDest);
    if (s32Ret == RK_SUCCESS) {
        for (RK_S32 i=0; i < pstBindDest.u32Num; i++) {
            MPP_CHN_S *pstDstChn = &pstBindDest.astMppChn[i];
            RK_LOGD("get dst channel(modId=%d, devId=%d, chnId=%d)",
                pstDstChn->enModId, pstDstChn->s32DevId, pstDstChn->s32ChnId);
        }
    } else {
        RK_LOGE("failed to RK_MPI_SYS_GetBindbySrc");
    }

    return s32Ret;
}

RK_S32 test_mpi_sys_get_bind_by_dest(TEST_SYS_CTX_S *pstCtx, RK_S32 s32ChnId) {
    RK_S32 s32Ret = RK_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDstChn;

    memset(&stSrcChn, 0, sizeof(MPP_CHN_S));
    memset(&stDstChn, 0, sizeof(MPP_CHN_S));
    stDstChn.enModId = RK_ID_AO;
    stDstChn.s32DevId = pstCtx->s32DevId;
    stDstChn.s32ChnId = s32ChnId;
    s32Ret = RK_MPI_SYS_GetBindbyDest(&stDstChn, &stSrcChn);
    if (s32Ret == RK_SUCCESS) {
        RK_LOGD("get src channel(modId=%d, devId=%d, chnId=%d)",
                stSrcChn.enModId, stSrcChn.s32DevId, stSrcChn.s32ChnId);
    } else {
        RK_LOGE("failed to RK_MPI_SYS_GetBindbyDest");
    }

    return s32Ret;
}

RK_S32 unit_test_mpi_sys_bind(TEST_SYS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_S32 s32SrcChnId = pstCtx->s32SrcChnId;
    RK_S32 s32DstNumChn = pstCtx->s32DstChnNum;

    // init Ao device
    s32Ret = test_ao_dev_init(pstCtx);

    // create adec channel
    s32Ret = test_adec_create_channel(pstCtx, s32SrcChnId);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED_ADEC;
    }

    // enable ao channel
    for (RK_S32 s32DstChnId = 0; s32DstChnId < s32DstNumChn; s32DstChnId++) {
        s32Ret = test_ao_enable_channel(pstCtx, s32DstChnId);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED_AO;
        }
        // bind adec->ao
        s32Ret = test_bind_adec_ao(pstCtx, s32SrcChnId, s32DstChnId);
        if (s32Ret == RK_SUCCESS) {
            RK_LOGD("succeed to bind ADEC(%d) AO(%d)", s32SrcChnId, s32DstChnId);
        }
    }

    // test RK_MPI_SYS_GetBindbyDest/GetBindbySrc
    s32Ret = test_mpi_sys_get_bind_by_src(pstCtx, s32SrcChnId);
    for (RK_S32 s32DstChnId = 0; s32DstChnId < s32DstNumChn; s32DstChnId++) {
        s32Ret = test_mpi_sys_get_bind_by_dest(pstCtx, s32DstChnId);
    }

__FAILED_AO:
    for (RK_S32 s32DstChnId = 0; s32DstChnId < s32DstNumChn; s32DstChnId++) {
        // unbind adec->ao
        s32Ret = test_unbind_adec_ao(pstCtx, s32SrcChnId, s32DstChnId);
        if (s32Ret == RK_SUCCESS) {
            RK_LOGD("succeed to unbind ADEC(%d) AO(%d)", s32SrcChnId, s32DstChnId);
        }
        // disable ao channel
        test_ao_disable_channel(pstCtx, s32DstChnId);
    }

    test_adec_destroy_channel(pstCtx, s32SrcChnId);

 __FAILED_ADEC:
    // deinit Ao device
    s32Ret = test_ao_dev_deinit(pstCtx);

    return s32Ret;
}

RK_S32 unit_test_mpi_sys(TEST_SYS_CTX_S *pstCtx) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_S32 loopCount = pstCtx->s32LoopCount;
    do {
        s32Ret = unit_test_mpi_sys_bind(pstCtx);
        loopCount--;
        RK_LOGI("looping times %d", pstCtx->s32LoopCount - loopCount);
    } while (loopCount > 0);

    return s32Ret;
}

static const char *const usages[] = {
    "./rk_mpi_sys_test...",
    NULL,
};

RK_S32 main(RK_S32 argc, const char **argv) {
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_SYS_CTX_S stCtx;

    memset(&stCtx, 0, sizeof(TEST_SYS_CTX_S));
    stCtx.s32LoopCount = 1;
    stCtx.s32DevId = 0;
    stCtx.s32SrcChnId = 0;
    stCtx.s32DstChnNum = 1;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('n', "loop_count", &(stCtx.s32LoopCount),
                    "loop running count. default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "device_id", &(stCtx.s32DevId),
                    "MODULE device id. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "src_channel_id", &(stCtx.s32SrcChnId),
                    "source MODULE channel id. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_channel_count", &(stCtx.s32DstChnNum),
                    "the count of dst MODULE channel. default(1)", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);

    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = unit_test_mpi_sys(&stCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    s32Ret = RK_MPI_SYS_Exit();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }
    RK_LOGI("test running ok.");
    return RK_SUCCESS;

__FAILED:
    RK_MPI_SYS_Exit();
    RK_LOGE("test running failed!");
    return s32Ret;
}
