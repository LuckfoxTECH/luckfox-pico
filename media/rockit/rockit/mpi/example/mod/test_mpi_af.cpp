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
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <sys/poll.h>
#include "rk_defines.h"
#include "rk_debug.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"
#include "rk_comm_af.h"
#include "rk_mpi_af.h"
#include "test_comm_argparse.h"

typedef enum rkTEST_FILTER_TYPE_E {
    TEST_FILTER_RESAMPLE = 0,
    TEST_FILTER_3A       = 1,
    TEST_FILTER_BUTT
} TEST_FILTER_TYPE_E;

typedef struct _rkTEST_AF_CTX_S {
    RK_S32      s32LoopCount;
    const char *srcFilePath;
    const char *dstFilePath;
    const char *chAfType;
    const char *agCfgsPath;
    RK_S32      s32InputRate;
    RK_S32      s32OutRate;
    RK_S32      s32InputCh;
    RK_S32      s32OutCh;
    RK_S32      s32InputBitWidth;
    RK_S32      s32OutputBitWidth;
    RK_S32      s32InputChnLayout;
    RK_S32      s32InputRefLayout;
    RK_S32      s32InputRecLayout;
    RK_S32      s32ChnNum;
    RK_S32      s32ChnIndex;
    RK_S32      bBlock;
    RK_VOID    *afHandle;
} TEST_AF_CTX_S;

static AUDIO_BIT_WIDTH_E find_af_bit_width(RK_S32 bit) {
    AUDIO_BIT_WIDTH_E bitWidth = AUDIO_BIT_WIDTH_BUTT;
    switch (bit) {
      case 8:
        bitWidth = AUDIO_BIT_WIDTH_8;
        break;
      case 16:
        bitWidth = AUDIO_BIT_WIDTH_16;
        break;
      case 24:
        bitWidth = AUDIO_BIT_WIDTH_24;
        break;
      default:
        RK_LOGE("bitwidth(%d) not support", bit);
        return AUDIO_BIT_WIDTH_BUTT;
    }

    return bitWidth;
}

static AUDIO_SOUND_MODE_E find_af_sound_mode(RK_S32 ch) {
    AUDIO_SOUND_MODE_E channel = AUDIO_SOUND_MODE_BUTT;
    switch (ch) {
      case 1:
        channel = AUDIO_SOUND_MODE_MONO;
        break;
      case 2:
        channel = AUDIO_SOUND_MODE_STEREO;
        break;
      default:
        RK_LOGE("channel = %d not support", ch);
        return AUDIO_SOUND_MODE_BUTT;
    }

    return channel;
}

RK_S32 test_init_af_resample(TEST_AF_CTX_S *ctx) {
    RK_S32 ret = 0;
    AF_ATTR_S attr;
    memset(&attr, 0, sizeof(AF_ATTR_S));

    attr.enType = AUDIO_FILTER_RESAMPLE;
    attr.u32InBufCount  = 2;
    attr.u32OutBufCount = 2;

    attr.stResAttr.u32InRate      = ctx->s32InputRate;
    attr.stResAttr.u32InChn       = ctx->s32InputCh;
    attr.stResAttr.enInBitWidth   = find_af_bit_width(ctx->s32InputBitWidth);

    attr.stResAttr.u32OutRate     = ctx->s32OutRate;
    attr.stResAttr.u32OutChn      = ctx->s32OutCh;
    attr.stResAttr.enOutBitWidth  = find_af_bit_width(ctx->s32OutputBitWidth);
    ret = RK_MPI_AF_Create(ctx->s32ChnIndex, &attr);
    if (ret != RK_SUCCESS) {
        RK_LOGE("create af chn %d err:0x%x\n", 0, ret);
        return RK_FAILURE;
    }
    return RK_SUCCESS;
}

RK_S32 test_init_af_ag(TEST_AF_CTX_S *ctx) {
    RK_S32 ret = 0;
    AF_ATTR_S attr;
    memset(&attr, 0, sizeof(AF_ATTR_S));

    attr.enType = AUDIO_FILTER_3A;
    attr.u32InBufCount  = 2;
    attr.u32OutBufCount = 2;

    snprintf(reinterpret_cast<char *>(attr.st3AAttr.cfgPath),
            sizeof(attr.st3AAttr.cfgPath), "%s", ctx->agCfgsPath);
    attr.st3AAttr.u32SampleRate = ctx->s32InputRate;
    attr.st3AAttr.enBitWidth = find_af_bit_width(ctx->s32InputBitWidth);
    attr.st3AAttr.u32Channels = ctx->s32InputCh;
    attr.st3AAttr.u32ChnLayout = ctx->s32InputChnLayout;
    attr.st3AAttr.u32RecLayout = ctx->s32InputRecLayout;
    attr.st3AAttr.u32RefLayout = ctx->s32InputRefLayout;
    ret = RK_MPI_AF_Create(ctx->s32ChnIndex, &attr);
    if (ret != RK_SUCCESS) {
        RK_LOGE("create af chn %d err:0x%x\n", 0, ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 test_init_af(TEST_AF_CTX_S *ctx) {
    if (strstr(ctx->chAfType, "resample")) {
        test_init_af_resample(ctx);
    } else if (strstr(ctx->chAfType, "3a")) {
        test_init_af_ag(ctx);
    }
    return RK_SUCCESS;
}

RK_S32 test_deinit_af(TEST_AF_CTX_S *ctx) {
    if (ctx->afHandle != RK_NULL) {
        RK_MPI_AF_Destroy(ctx->s32ChnIndex);
    }
    return RK_SUCCESS;
}

void* send_af_thread(void * ptr) {
    TEST_AF_CTX_S *params = reinterpret_cast<TEST_AF_CTX_S *>(ptr);
    MB_POOL_CONFIG_S pool_config;
    // set default value for struct
    RK_U8 *srcData = RK_NULL;
    AUDIO_FRAME_S frame;
    RK_U64 timeStamp = 0;
    RK_S32 s32MilliSec = -1;
    RK_S32 size = 0;
    RK_S32 result = 0;
    RK_U32 seq = 0;

    AUDIO_FRAME_S stFrame;
    memset(&stFrame, 0, sizeof(AUDIO_FRAME_S));

    AUDIO_FRAME_INFO_S inFrmInfo;
    memset(&inFrmInfo, 0, sizeof(AUDIO_FRAME_INFO_S));

    s32MilliSec = -1;
    FILE *file = RK_NULL;
    if (params->srcFilePath) {
        file = fopen(params->srcFilePath, "rb");
    }

    if (file == RK_NULL) {
        RK_LOGE("open save file %s failed because %s.", params->srcFilePath, strerror(errno));
        goto __EXIT;
    }

    srcData = reinterpret_cast<RK_U8 *>(calloc(1024, sizeof(RK_U8)));
    memset(srcData, 0, 1024);
    while (1) {
        size = fread(srcData, 1, 1024, file);

        stFrame.u32Len = size;
        stFrame.enBitWidth = AUDIO_BIT_WIDTH_16;
        stFrame.u64TimeStamp = timeStamp;
        stFrame.u32Seq = seq;
        stFrame.enSoundMode = find_af_sound_mode(params->s32InputCh);
        stFrame.bBypassMbBlk = RK_FALSE;

        MB_EXT_CONFIG_S extConfig;
        memset(&extConfig, 0, sizeof(extConfig));
        extConfig.pOpaque = srcData;
        extConfig.pu8VirAddr = srcData;
        extConfig.u64Size = size;
        RK_MPI_SYS_CreateMB(&(stFrame.pMbBlk), &extConfig);

        inFrmInfo.pstFrame = &stFrame;
        inFrmInfo.u32Id = 0;

        result = RK_MPI_AF_SendFrame(params->s32ChnIndex, &inFrmInfo, s32MilliSec);
        if (result != RK_SUCCESS) {
            RK_LOGE("send frame error, seq = %d", seq);
        }
        RK_MPI_MB_ReleaseMB(stFrame.pMbBlk);
        timeStamp++;
        seq++;
        if (size <= 0) {
            RK_LOGI("eof");
            break;
        }
    }

__EXIT:
    if (file) {
        fclose(file);
        file = RK_NULL;
    }
    free(srcData);
    return RK_NULL;
}

void* receive_af_thread(void * ptr) {
    TEST_AF_CTX_S *params = reinterpret_cast<TEST_AF_CTX_S *>(ptr);
    MB_POOL_CONFIG_S pool_config;
    // set default value for struct
    RK_U8 *srcData = RK_NULL;
    AUDIO_FRAME_S frame;
    RK_U64 timeStamp = 0;
    RK_S32 s32MilliSec = -1;
    RK_S32 size = 0;
    RK_S32 result = 0;
    RK_U32 seq = 0;
    RK_S32 eos = 0;
    RK_VOID *outdata = RK_NULL;

    AUDIO_FRAME_S stFrame;
    memset(&stFrame, 0, sizeof(AUDIO_FRAME_S));

    AUDIO_FRAME_INFO_S outFrmInfo;
    memset(&outFrmInfo, 0, sizeof(AUDIO_FRAME_INFO_S));

    s32MilliSec = -1;
    FILE *file = RK_NULL;
    if (params->dstFilePath) {
        file = fopen(params->dstFilePath, "wb+");
        if (file == RK_NULL) {
            RK_LOGE("failed to open output file %s, error: %s.", params->dstFilePath, strerror(errno));
            goto __EXIT;
        }
    }

    if (file == RK_NULL) {
        RK_LOGE("open save file %s failed because %s.", params->srcFilePath, strerror(errno));
        goto __EXIT;
    }

    srcData = reinterpret_cast<RK_U8 *>(calloc(1024, sizeof(RK_U8)));
    memset(srcData, 0, 1024);
    while (1) {
        outFrmInfo.pstFrame = RK_NULL;
        result = RK_MPI_AF_GetFrame(params->s32ChnIndex, &outFrmInfo, s32MilliSec);
        if (result != RK_SUCCESS) {
            RK_LOGE("get frame error, seq = %d", seq);
        } else {
            outdata = RK_MPI_MB_Handle2VirAddr(outFrmInfo.pstFrame->pMbBlk);
            size = outFrmInfo.pstFrame->u32Len;
            eos = (size <= 0) ? 1 : 0;
            RK_LOGV("get frame (size: %d, outdata: %p)", size, outdata);
            if (file != RK_NULL && size >= 0) {
                fwrite(outdata, 1, size, file);
            }

            RK_MPI_AF_ReleaseFrame(params->s32ChnIndex, &outFrmInfo);
        }
        if (eos) {
            RK_LOGI("get eos stream.");
            break;
        }
    }

__EXIT:
    if (file) {
        fclose(file);
        file = RK_NULL;
    }

    return RK_NULL;
}

RK_S32 unit_test_mpi_af(TEST_AF_CTX_S *ctx) {
    RK_S32 i = 0;
    TEST_AF_CTX_S afCtx[AF_MAX_CHN_NUM];
    pthread_t tidSend[AF_MAX_CHN_NUM];
    pthread_t tidReceive[AF_MAX_CHN_NUM];
    ctx->bBlock = RK_TRUE;
    if (ctx->s32ChnNum > AF_MAX_CHN_NUM) {
        RK_LOGE("af chn(%d) > max_chn(%d)", ctx->s32ChnNum, AF_MAX_CHN_NUM);
        goto __FAILED;
    }
    for (i = 0; i < ctx->s32ChnNum; i++) {
        afCtx[i].s32ChnIndex = i;
        memcpy(&afCtx, ctx, sizeof(TEST_AF_CTX_S));
        if (test_init_af(&afCtx[i]) == RK_FAILURE) {
            goto __FAILED;
        }

        pthread_create(&tidSend[i], RK_NULL, send_af_thread, reinterpret_cast<void *>(&afCtx));
        pthread_create(&tidReceive[i], RK_NULL, receive_af_thread, reinterpret_cast<void *>(&afCtx));
    }

    for (i = 0; i < ctx->s32ChnNum; i++) {
        pthread_join(tidSend[i], RK_NULL);
        pthread_join(tidReceive[i], RK_NULL);
        test_deinit_af(&afCtx[i]);
    }

    return RK_SUCCESS;
__FAILED:

    return RK_FAILURE;
}

static const char *const usages[] = {
    "./rk_mpi_af_test [-i src_path] [-o dst_patch] [--af_type af_type] [--input_rate rate] [--input_ch ch]...",
    NULL,
};

void mpi_af_test_show_options(const TEST_AF_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("loop count            : %d\n", ctx->s32LoopCount);
    RK_PRINT("input  file name      : %s\n", ctx->srcFilePath);
    RK_PRINT("output file name      : %s\n", ctx->dstFilePath);
    RK_PRINT("af type               : %s\n", ctx->chAfType);
    RK_PRINT("af config patch       : %s\n", ctx->agCfgsPath);
    RK_PRINT("input rate            : %d\n", ctx->s32InputRate);
    RK_PRINT("output rate           : %d\n", ctx->s32OutRate);
    RK_PRINT("input channel         : %d\n", ctx->s32InputCh);
    RK_PRINT("output channel        : %d\n", ctx->s32OutCh);
    RK_PRINT("input bitwidth        : %d\n", ctx->s32InputBitWidth);
    RK_PRINT("output bitwidth       : %d\n", ctx->s32OutputBitWidth);
    RK_PRINT("input chnLayout       : %d\n", ctx->s32InputChnLayout);
    RK_PRINT("input refLayout      : %d\n", ctx->s32InputRefLayout);
    RK_PRINT("input recLayout      : %d\n", ctx->s32InputRecLayout);
}

int main(int argc, const char **argv) {
    RK_S32          i;
    RK_S32          s32Ret;
    TEST_AF_CTX_S  *ctx;

    ctx = reinterpret_cast<TEST_AF_CTX_S *>(malloc(sizeof(TEST_AF_CTX_S)));
    memset(ctx, 0, sizeof(TEST_AF_CTX_S));

    ctx->s32LoopCount       = 1;
    ctx->s32ChnNum          = 1;
    ctx->srcFilePath        = RK_NULL;
    ctx->dstFilePath        = RK_NULL;
    ctx->chAfType           = RK_NULL;
    ctx->agCfgsPath         = RK_NULL;
    ctx->s32InputRate       = 0;
    ctx->s32OutRate         = 0;
    ctx->s32InputCh         = 0;
    ctx->s32OutCh           = 0;
    ctx->s32InputBitWidth   = 0;
    ctx->s32OutputBitWidth  = 0;
    ctx->s32InputChnLayout  = 0;
    ctx->s32InputRefLayout  = 0;
    ctx->s32InputRecLayout  = 0;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx->srcFilePath),
                   "input file name , e.g.(./*.pcm). <required>", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx->dstFilePath),
                    "output file name, e.g.(./ao). default(required).", NULL, 0, 0),
        OPT_STRING('\0', "af_type", &(ctx->chAfType),
                    "set audio filter type, default(required)", NULL, 0, 0),
        OPT_STRING('\0', "3a_cfgs", &(ctx->agCfgsPath),
                    "3a configs patch, default(NULL)", NULL, 0, 0),
        OPT_INTEGER('\0', "input_rate", &(ctx->s32InputRate),
                    "the sample rate of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_ch", &(ctx->s32InputCh),
                    "the channels of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_bit", &(ctx->s32InputBitWidth),
                    "the bit width of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "out_rate", &(ctx->s32OutRate),
                    "the sample rate of out data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "out_ch", &(ctx->s32OutCh),
                    "the channels of out data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "out_bit", &(ctx->s32OutputBitWidth),
                    "the bit width of out data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_ch_layout", &(ctx->s32InputChnLayout),
                    "the channels layout of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_rec_layout", &(ctx->s32InputRecLayout),
                    "the mic channels layout of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_ref_layout", &(ctx->s32InputRefLayout),
                    "the ref channels layout of input data. <required>", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx->s32LoopCount),
                    "loop running count. can be any count. default(1)", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_af_test_show_options(ctx);
    RK_LOGI("ctx->srcFilePath : %s, ctx->chAfType: %s, ctx->s32InputRate : %d, ctx->s32InputCh: %d",
            ctx->srcFilePath, ctx->chAfType, ctx->s32InputRate, ctx->s32InputCh);
    if (ctx->srcFilePath == RK_NULL
        || ctx->chAfType == RK_NULL
        || ctx->s32InputRate <= 0
        || ctx->s32InputCh <= 0) {
        argparse_usage(&argparse);
        goto __FAILED;
    }

    RK_MPI_SYS_Init();

    for (i = 0; i < ctx->s32LoopCount; i++) {
        RK_LOGI("start running loop count  = %d", i);
        s32Ret = unit_test_mpi_af(ctx);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        RK_LOGI("end running loop count  = %d", i);
    }

__FAILED:
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }

    RK_MPI_SYS_Exit();
    return 0;
}
