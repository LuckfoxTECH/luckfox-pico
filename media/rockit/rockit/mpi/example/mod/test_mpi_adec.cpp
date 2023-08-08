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
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "rk_debug.h"
#include "rk_mpi_adec.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_comm_adec.h"
#include "test_comm_argparse.h"
#define USE_EXT_ADEC 0

typedef struct _rkMpiADECCtx {
    const char *srcFilePath;
    const char *dstFilePath;
    RK_S32      s32LoopCount;
    RK_S32      s32ChnNum;
    RK_S32      s32SampleRate;
    RK_S32      s32Channel;
    char       *chCodecId;
    RK_S32      s32DecMode;
    RK_BOOL     bBlock;
    RK_S32      s32ChnIndex;
    RK_S32      s32QueryStat;
    RK_S32      s32ClrChnBuf;
    ADEC_CHN_ATTR_S *pstChnAttr;
    RK_S32 s32ExtCodecHandle;
} TEST_ADEC_CTX_S;

extern RK_S32 register_ext_adec(TEST_ADEC_CTX_S *params);
extern RK_S32 unregister_ext_adec(TEST_ADEC_CTX_S *params);

void query_adec_flow_graph_stat(ADEC_CHN AdChn) {
    RK_S32 ret = 0;
    ADEC_CHN_STATE_S pstStat;
    memset(&pstStat, 0, sizeof(ADEC_CHN_STATE_S));
    ret = RK_MPI_ADEC_QueryChnStat(AdChn, &pstStat);
    if (ret == RK_SUCCESS) {
        RK_LOGI("query adec flow status:");
        RK_LOGI("EOS flag : %d", pstStat.bEndOfStream);
        RK_LOGI("total number of channel buffer : %d", pstStat.u32BufferFrmNum);
        RK_LOGI("free number of channel buffer : %d", pstStat.u32BufferFreeNum);
        RK_LOGI("busy number of channel buffer : %d", pstStat.u32BufferBusyNum);
    }
}

static ADEC_CHN_ATTR_S* adec_create_chn_attr() {
    ADEC_CHN_ATTR_S *pstChnAttr = reinterpret_cast<ADEC_CHN_ATTR_S *>
                                        (malloc(sizeof(ADEC_CHN_ATTR_S)));
    memset(pstChnAttr, 0, sizeof(ADEC_CHN_ATTR_S));

    return pstChnAttr;
}

static void adec_destroy_chn_attr(ADEC_CHN_ATTR_S **ppstChnAttr) {
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

    // if user use this, add code here to free
    if (pstCodecAttr->pstResv != RK_NULL) {
        RK_LOGD("please add code here to free pstResv");
    }

    free(pstChnAttr);
    *ppstChnAttr = RK_NULL;
}
static RK_S32 adec_fill_codec_attr(ADEC_CHN_ATTR_S *pstChnAttr,
            RK_CODEC_ID_E  enCodecId, RK_U32 samplerate, RK_U32 channels,
            RK_U32 bitPerSample, void *extradata, RK_U32 extraSize) {
    if (pstChnAttr == RK_NULL)
        return RK_FAILURE;

    ADEC_ATTR_CODEC_S *pstCodecAttr    = &pstChnAttr->stCodecAttr;
    pstCodecAttr->u32Channels          = channels;
    pstCodecAttr->u32SampleRate        = samplerate;
    pstCodecAttr->u32BitPerCodedSample = bitPerSample;
    pstCodecAttr->u32ExtraDataSize     = extraSize;
    if (pstCodecAttr->u32ExtraDataSize > 0 && extradata != RK_NULL) {
        pstCodecAttr->pExtraData = malloc(pstCodecAttr->u32ExtraDataSize);
        memcpy(pstCodecAttr->pExtraData, extradata, pstCodecAttr->u32ExtraDataSize);
    } else {
        pstCodecAttr->pExtraData = RK_NULL;
        pstCodecAttr->u32ExtraDataSize = 0;
    }

    return RK_SUCCESS;
}
static RK_S32 adec_data_free(void *opaque) {
    if (opaque) {
        free(opaque);
        opaque = RK_NULL;
    }

    return 0;
}

static RK_U32 test_find_audio_codec_id(TEST_ADEC_CTX_S *params) {
    if (params == RK_NULL)
        return -1;

    char *format = params->chCodecId;
    if (strstr(format, "mp2")) {
        return RK_AUDIO_ID_MP2;
    } else if (strstr(format, "g726")) {
        return RK_AUDIO_ID_ADPCM_G726;
    } else if (strstr(format, "g711a")) {
        return RK_AUDIO_ID_PCM_ALAW;
    } else if (strstr(format, "g711u")) {
        return RK_AUDIO_ID_PCM_MULAW;
    }

    if (params->s32DecMode == ADEC_MODE_STREAM) {
        RK_LOGE("test not find codec id : %s", params->chCodecId);
        return RK_AUDIO_ID_Unused;
    } else {
        // if set packet mode, try to get codecId, channels, samplerate
        return RK_AUDIO_ID_AutoDetect;
    }
}

RK_S32 test_init_mpi_adec(TEST_ADEC_CTX_S *params) {
    RK_S32 i = 0;
    RK_S32 s32ret = 0;
    RK_U32 codecId = 0;
    ADEC_CHN AdChn = (ADEC_CHN)(params->s32ChnIndex);
    ADEC_CHN_ATTR_S *pstChnAttr = RK_NULL;
    ADEC_ATTR_CODEC_S *pstCodecAttr = RK_NULL;
    params->pstChnAttr = adec_create_chn_attr();
    pstChnAttr = params->pstChnAttr;
    pstCodecAttr = &pstChnAttr->stCodecAttr;
    if (pstCodecAttr->u32Channels == 0) {
        pstCodecAttr->u32Channels = params->s32Channel;
        pstCodecAttr->u32SampleRate = params->s32SampleRate;
        pstCodecAttr->u32BitPerCodedSample = 4;
    }

    codecId = test_find_audio_codec_id(params);
    if (codecId == RK_AUDIO_ID_Unused) {
        s32ret = RK_FAILURE;
        goto EXIT;
    }

    pstCodecAttr->enType    = (RK_CODEC_ID_E)codecId;
    pstChnAttr->enType      = (RK_CODEC_ID_E)codecId;
    pstChnAttr->enMode      = (ADEC_MODE_E)params->s32DecMode;
    pstChnAttr->u32BufCount = 4;
    pstChnAttr->u32BufSize  = 50*1024;  // 50K
    s32ret = RK_MPI_ADEC_CreateChn(AdChn, pstChnAttr);
    if (s32ret) {
        RK_LOGE("create adec chn %d err:0x%x\n", AdChn, s32ret);
        s32ret = RK_FAILURE;
    } else {
        s32ret = RK_SUCCESS;
    }

EXIT:
    return s32ret;
}

RK_S32 test_deinit_mpi_adec(TEST_ADEC_CTX_S *ctx) {
    RK_MPI_ADEC_DestroyChn(ctx->s32ChnIndex);
    adec_destroy_chn_attr(&ctx->pstChnAttr);

    return RK_SUCCESS;
}

static void *send_stream_thread(void *arg) {
    RK_S32 s32ret = 0;
    TEST_ADEC_CTX_S *params = reinterpret_cast<TEST_ADEC_CTX_S *>(arg);
    RK_U8 *srcData = RK_NULL;
    RK_S32 srcSize = 0;
    FILE  *file = RK_NULL;
    RK_S32 err = 0;
    RK_S32 pktEos = 0;
    AUDIO_STREAM_S stAudioStream;
    RK_BOOL bBlock = params->bBlock;
    ADEC_CHN AdChn = (ADEC_CHN)(params->s32ChnIndex);
    RK_U64 timeStamp = 0;
    RK_S32 count = 0;
    RK_S64 dataOffset = 0;
    RK_U32 codecId = 0;

    file = fopen(params->srcFilePath, "rb");
    if (file == RK_NULL) {
        RK_LOGE("failed to open input file(%s), error: %s", params->srcFilePath, strerror(errno));
        goto __FAILED;
    codecId = test_find_audio_codec_id(params);
    }

    while (1) {
        srcData = reinterpret_cast<RK_U8 *>(calloc(1024, sizeof(RK_U8)));
        memset(srcData, 0, 1024);

        srcSize = fread(srcData, 1, 1024, file);

        if (srcSize == 0 || srcData == RK_NULL) {
            RK_LOGI("read eos packet, now send eos packet!");
            pktEos = 1;
        }

        if (pktEos) {
            RK_MPI_ADEC_SendEndOfStream(AdChn, RK_FALSE);
            // free srcData here, otherwise it will be leaked
            free(srcData);
            break;
        } else {
            stAudioStream.u32Len = srcSize;
            stAudioStream.u64TimeStamp = timeStamp;
            stAudioStream.u32Seq = ++count;
            stAudioStream.bBypassMbBlk = RK_TRUE;
            MB_EXT_CONFIG_S extConfig = {0};
            extConfig.pFreeCB = adec_data_free;
            extConfig.pOpaque = srcData;
            extConfig.pu8VirAddr = srcData;
            extConfig.u64Size    = srcSize;
            RK_MPI_SYS_CreateMB(&(stAudioStream.pMbBlk), &extConfig);
__RETRY:
            s32ret = RK_MPI_ADEC_SendStream(AdChn, &stAudioStream, bBlock);
            if (s32ret != RK_SUCCESS) {
                RK_LOGE("fail to send adec stream.");
                goto __RETRY;
            }
            RK_MPI_MB_ReleaseMB(stAudioStream.pMbBlk);
        }
        timeStamp++;
    }

__FAILED:
    if (file) {
        fclose(file);
        file = RK_NULL;
    }

    return RK_NULL;
}


static void *receive_data_thread(void *arg) {
    RK_S32 s32ret = 0;
    FILE  *file = RK_NULL;
    TEST_ADEC_CTX_S *params = reinterpret_cast<TEST_ADEC_CTX_S *>(arg);
    AUDIO_FRAME_INFO_S  *pstFrmInfo = RK_NULL;

    pstFrmInfo = reinterpret_cast<AUDIO_FRAME_INFO_S *>(malloc(sizeof(AUDIO_FRAME_INFO_S)));
    memset(pstFrmInfo, 0, sizeof(AUDIO_FRAME_INFO_S));

    RK_BOOL bBlock = params->bBlock;
    ADEC_CHN AdChn = (ADEC_CHN)(params->s32ChnIndex);
    RK_S32 eos = 0;
    RK_S32 count = 0;

    if (params->dstFilePath) {
        file = fopen(params->dstFilePath, "wb+");
        if (file == RK_NULL) {
            RK_LOGE("failed to open output file %s, error: %s.", params->dstFilePath, strerror(errno));
            goto __FAILED;
        }
    }

    while (1) {
        s32ret = RK_MPI_ADEC_GetFrame(AdChn, pstFrmInfo, bBlock);
        if (s32ret == RK_SUCCESS) {
            MB_BLK bBlk = pstFrmInfo->pstFrame->pMbBlk;
            RK_VOID *pstFrame = RK_MPI_MB_Handle2VirAddr(bBlk);
            RK_S32 frameSize = pstFrmInfo->pstFrame->u32Len;
            eos = (frameSize <= 0) ? 1 : 0;
            if (pstFrame) {
                RK_LOGV("get frame data = %p, size = %d", pstFrame, frameSize);
                if (file) {
                    fwrite(pstFrame, frameSize, 1, file);
                    fflush(file);
                }
                //  release frame
                RK_MPI_ADEC_ReleaseFrame(AdChn, pstFrmInfo);
                count++;
            }
        } else {
            RK_LOGE("fail to get adec frame.");
        }

        if (params->s32QueryStat) {
            query_adec_flow_graph_stat(AdChn);
            params->s32QueryStat = 0;
        }

        if (params->s32ClrChnBuf) {
            RK_LOGI("test clear chn(%d) buf", AdChn);
            RK_MPI_ADEC_ClearChnBuf(AdChn);
            params->s32ClrChnBuf = 0;
        }

        if (eos) {
            RK_LOGI("get eos packet.");
            break;
        }
    }

__FAILED:
    if (pstFrmInfo) {
        free(pstFrmInfo);
        pstFrmInfo = RK_NULL;
    }

    if (file) {
        fclose(file);
        file = RK_NULL;
    }
    return RK_NULL;
}

RK_S32 register_ext_adec(TEST_ADEC_CTX_S *params) {
    ADEC_DECODER_S adecCtx;
    memset(&adecCtx, 0, sizeof(ADEC_DECODER_S));

    params->s32ExtCodecHandle = -1;
    adecCtx.enType = RK_AUDIO_ID_PCM_ALAW;
    snprintf(reinterpret_cast<RK_CHAR*>(adecCtx.aszName),
                               sizeof(adecCtx.aszName), "ext_g711a");

    /*adecCtx.pfnOpenDecoder  = RKMpiAdecFFmpegDecoderDemo::open;
    adecCtx.pfnDecodeFrm    = RKMpiAdecFFmpegDecoderDemo::decode;
    adecCtx.pfnGetFrmInfo   = RKMpiAdecFFmpegDecoderDemo::getFrameInfo;
    adecCtx.pfnCloseDecoder = RKMpiAdecFFmpegDecoderDemo::close;
    adecCtx.pfnResetDecoder = RKMpiAdecFFmpegDecoderDemo::reset;*/

    RK_LOGD("register ext audio decoder");
    RK_S32 ret = RK_MPI_ADEC_RegisterDecoder(&params->s32ExtCodecHandle, &adecCtx);
    if (ret != RK_SUCCESS) {
        RK_LOGE("adec register decoder fail, ret = 0x%x", ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 unregister_ext_adec(TEST_ADEC_CTX_S *params) {
    if (params == NULL || params->s32ExtCodecHandle == -1) {
        return RK_SUCCESS;
    }
    RK_LOGD("unregister ext audio decoder");
    RK_S32 ret = RK_MPI_ADEC_UnRegisterDecoder(params->s32ExtCodecHandle);
    if (ret != RK_SUCCESS) {
        RK_LOGE("adec unregister decoder fail, ret = 0x%x", ret);
        return RK_FAILURE;
    }

    params->s32ExtCodecHandle = -1;
    return RK_SUCCESS;
}


RK_S32 unit_test_mpi_adec(TEST_ADEC_CTX_S *params) {
    RK_S32 i = 0;
    RK_S32 counter = 0;
    TEST_ADEC_CTX_S adecCtx[ADEC_MAX_CHN_NUM];
    pthread_t tidSend[ADEC_MAX_CHN_NUM];
    pthread_t tidReceive[ADEC_MAX_CHN_NUM];

    if (params->s32ChnNum > ADEC_MAX_CHN_NUM) {
        RK_LOGE("adec chn(%d) > max_chn(%d)", params->s32ChnNum, ADEC_MAX_CHN_NUM);
        goto __FAILED;
    }

    params->bBlock = RK_TRUE;

    for (i = 0; i < params->s32ChnNum; i++) {
        memcpy(&(adecCtx[i]), params, sizeof(TEST_ADEC_CTX_S));
        adecCtx[i].s32ChnIndex = i;
        adecCtx[i].pstChnAttr = RK_NULL;
        counter++;

        if (test_init_mpi_adec(&adecCtx[i]) == RK_FAILURE) {
            goto __FAILED;
        }

        if (params->s32DecMode == ADEC_MODE_STREAM) {
            pthread_create(&tidSend[i], RK_NULL, send_stream_thread, reinterpret_cast<void *>(&adecCtx[i]));
        } else {

        }
        pthread_create(&tidReceive[i], RK_NULL, receive_data_thread, reinterpret_cast<void *>(&adecCtx[i]));
    }

    for (i = 0; i < params->s32ChnNum; i++) {
        pthread_join(tidSend[i], RK_NULL);
        pthread_join(tidReceive[i], RK_NULL);
        test_deinit_mpi_adec(&adecCtx[i]);
    }

    return RK_SUCCESS;
__FAILED:
    for (i = 0; i < counter; i++) {
        test_deinit_mpi_adec(&adecCtx[i]);
    }

    return RK_FAILURE;
}

static const char *const usages[] = {
    "./rk_mpi_adec_test [-i src_path] [-C name] [--input_rate rate] [--input_ch ch]...",
    NULL,
};

static void mpi_adec_test_show_options(const TEST_ADEC_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input  file name       : %s\n", ctx->srcFilePath);
    RK_PRINT("output file name       : %s\n", ctx->dstFilePath);
    RK_PRINT("loop count             : %d\n", ctx->s32LoopCount);
    RK_PRINT("channel number         : %d\n", ctx->s32ChnNum);
    RK_PRINT("input sample rate      : %d\n", ctx->s32SampleRate);
    RK_PRINT("input channel          : %d\n", ctx->s32Channel);
    RK_PRINT("input codec name       : %s\n", ctx->chCodecId);
    RK_PRINT("input decode mode      : %d\n", ctx->s32DecMode);
    RK_PRINT("query stat             : %d\n", ctx->s32QueryStat);
    RK_PRINT("clear buf              : %d\n", ctx->s32ClrChnBuf);
}

int main(int argc, const char **argv) {
    RK_S32           i;
    RK_S32           s32Ret;
    TEST_ADEC_CTX_S *ctx;

    ctx = reinterpret_cast<TEST_ADEC_CTX_S *>(malloc(sizeof(TEST_ADEC_CTX_S)));
    memset(ctx, 0, sizeof(TEST_ADEC_CTX_S));

    ctx->srcFilePath     = RK_NULL;
    ctx->dstFilePath     = RK_NULL;
    ctx->s32LoopCount    = 1;
    ctx->s32ChnNum       = 1;
    ctx->s32DecMode      = 0;
    ctx->chCodecId       = RK_NULL;
    ctx->pstChnAttr      = RK_NULL;
    ctx->s32ExtCodecHandle = -1;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx->srcFilePath),
                   "input file name , e.g.(./*.mp3). <required>", NULL, 0, 0),
        OPT_STRING('C', "codec", &(ctx->chCodecId),
                    "codec, e.g.(mp2/g711a/g711u/g726). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_ch", &(ctx->s32Channel),
                    "the number of input stream channels. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_rate", &(ctx->s32SampleRate),
                    "the sample rate of input stream. <required>", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx->dstFilePath),
                    "output file name, e.g.(./*.pcm). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx->s32LoopCount),
                    "loop running count. default(1)", NULL, 0, 0),
        OPT_INTEGER('c', "channel_count", &(ctx->s32ChnNum),
                    "the count of adec channel. default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "dec_mode", &(ctx->s32DecMode),
                    "the audio stream decode mode, range(0: pack mode, 1: stream mode), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "query_stat", &(ctx->s32QueryStat),
                    "query adec statistics info, range(0: query, 1: not query), default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "clr_buf", &(ctx->s32ClrChnBuf),
                    "clear buffer of channel, range(0, 1), default(0)", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_adec_test_show_options(ctx);

    // must set params
    if (ctx->srcFilePath == RK_NULL
        || ctx->s32Channel <= 0
        || ctx->s32SampleRate <= 0
        || ctx->chCodecId == RK_NULL) {
        argparse_usage(&argparse);
        goto __FAILED;
    }

    RK_MPI_SYS_Init();

    if (USE_EXT_ADEC) {
        register_ext_adec(ctx);
    }

    for (i = 0; i < ctx->s32LoopCount; i++) {
        RK_LOGI("start running loop count  = %d", i);
        s32Ret = unit_test_mpi_adec(ctx);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        RK_LOGI("end running loop count  = %d", i);
    }

__FAILED:
    unregister_ext_adec(ctx);
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }

    RK_MPI_SYS_Exit();
    return 0;
}
