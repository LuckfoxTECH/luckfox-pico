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
#include <sys/poll.h>
#include "rk_debug.h"
#include "rk_mpi_aenc.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"

#include "test_comm_argparse.h"
#define TEST_AENC_WITH_FD 0
#define USE_EXT_AENC 0

typedef struct _rkTEST_AENC_CTX_S {
    const char *srcFilePath;
    const char *dstFilePath;
    RK_S32      s32LoopCount;
    RK_S32      s32ChnNum;
    RK_S32      s32SampleRate;
    RK_S32      s32Channel;
    RK_S32      s32Format;
    char       *chCodecId;
    RK_S32      s32MilliSec;
    RK_S32      s32ChnIndex;
    RK_S32      s32FrameSize;
    RK_S32      s32DevFd;
    RK_S32      s32ExtCodecHandle;
    AENC_CHN_ATTR_S *pstChnAttr;
} TEST_AENC_CTX_S;

static RK_S32 aenc_data_free(void *opaque) {
    if (opaque) {
        free(opaque);
        opaque = RK_NULL;
    }

    return 0;
}

static RK_U32 test_find_audio_enc_codec_id(TEST_AENC_CTX_S *params) {
    char *format = params->chCodecId;
   if (strstr(format, "mp2")) {
        return RK_AUDIO_ID_MP2;
    } else if (strstr(format, "mp3")) {
        return RK_AUDIO_ID_MP3;
    } else if (strstr(format, "g726")) {
        return RK_AUDIO_ID_ADPCM_G726;
    } else if (strstr(format, "g711a")) {
        return RK_AUDIO_ID_PCM_ALAW;
    } else if (strstr(format, "g711u")) {
        return RK_AUDIO_ID_PCM_MULAW;
    }

    RK_LOGE("test not find codec id : %s", params->chCodecId);
    return -1;
}

static RK_U32 test_find_audio_enc_format(TEST_AENC_CTX_S *params) {
    if (params->s32Format == 8) {
        return AUDIO_BIT_WIDTH_8;
    } else if (params->s32Format == 16) {
        return AUDIO_BIT_WIDTH_16;
    } else if (params->s32Format == 24) {
        return AUDIO_BIT_WIDTH_24;
    }

    RK_LOGE("test not find format : %s", params->s32Format);
    return -1;
}

static RK_U32 find_audio_frame_size(TEST_AENC_CTX_S *params) {
    RK_U32 channels = params->s32Channel;
    RK_U32 bytes    = 0;
    switch (params->s32Format) {
        case 8:
            bytes = 1;
            break;
        case 16:
            bytes = 2;
            break;
        case 32:
            bytes = 4;
            break;
        default:
            bytes = 2;
            RK_LOGE("test not find format : %d", params->s32Format);
            break;
    }
    return bytes*params->s32Channel;
}


static AENC_CHN_ATTR_S* aenc_create_chn_attr() {
    AENC_CHN_ATTR_S *pstChnAttr = reinterpret_cast<AENC_CHN_ATTR_S *>
                                        (malloc(sizeof(AENC_CHN_ATTR_S)));

    memset(pstChnAttr, 0, sizeof(AENC_CHN_ATTR_S));
    return pstChnAttr;
}

static RK_S32 aenc_fill_codec_attr(AENC_CHN_ATTR_S *pstChnAttr,
            RK_CODEC_ID_E  enCodecId, RK_U32 samplerate, RK_U32 channels,
            AUDIO_BIT_WIDTH_E enBitwidth) {
    AENC_ATTR_CODEC_S *pstCodecAttr = &pstChnAttr->stCodecAttr;

    pstCodecAttr->enType            = enCodecId;
    pstCodecAttr->u32Channels       = channels;
    pstCodecAttr->u32SampleRate     = samplerate;
    pstCodecAttr->enBitwidth        = enBitwidth;
    pstCodecAttr->pstResv           = RK_NULL;
    return 0;
}

static void aenc_destroy_chn_attr(AENC_CHN_ATTR_S **ppstChnAttr) {
    AENC_CHN_ATTR_S *pstChnAttr = *ppstChnAttr;

    if (pstChnAttr == RK_NULL) {
        return;
    }

    // add code here to free pstResv if it is used it.

    free(pstChnAttr);
    *ppstChnAttr = RK_NULL;
}

RK_S32 register_ext_aenc(TEST_AENC_CTX_S *params) {
    AENC_ENCODER_S aencCtx;

    memset(&aencCtx, 0, sizeof(AENC_ENCODER_S));
    params->s32ExtCodecHandle = -1;

    aencCtx.enType = RK_AUDIO_ID_PCM_ALAW;
    snprintf(reinterpret_cast<RK_CHAR*>(aencCtx.aszName),
                               sizeof(aencCtx.aszName), "ext_g711a");
    /*aencCtx.u32MaxFrmLen     = 8192;
    aencCtx.pfnOpenEncoder   = RKMpiAEncFFmpegEncoderDemo::open;
    aencCtx.pfnEncodeFrm     = RKMpiAEncFFmpegEncoderDemo::encode;
    aencCtx.pfnCloseEncoder  = RKMpiAEncFFmpegEncoderDemo::close;*/

    RK_LOGD("register external aenc(%s)", aencCtx.aszName);
    RK_S32 ret = RK_MPI_AENC_RegisterEncoder(&params->s32ExtCodecHandle, &aencCtx);
    if (ret != RK_SUCCESS) {
        RK_LOGE("aenc %s register decoder fail", aencCtx.aszName, ret);
        return RK_FAILURE;
    }
    return RK_SUCCESS;
}

RK_S32 unregister_ext_aenc(TEST_AENC_CTX_S *params) {
    if (params == NULL || params->s32ExtCodecHandle == -1) {
        return RK_SUCCESS;
    }
    RK_LOGD("unregister external aenc");
    RK_S32 ret = RK_MPI_AENC_UnRegisterEncoder(params->s32ExtCodecHandle);
    if (ret != RK_SUCCESS) {
        RK_LOGE("aenc unregister decoder fail", ret);
        return RK_FAILURE;
    }

    params->s32ExtCodecHandle = -1;
    return RK_SUCCESS;
}

RK_S32 test_aenc_poll_event(RK_S32 timeoutMsec, RK_S32 fd) {
    RK_S32 num_fds = 1;
    struct pollfd pollFds[num_fds];
    RK_S32 ret = 0;

    RK_ASSERT(fd > 0);
    memset(pollFds, 0, sizeof(pollFds));
    pollFds[0].fd = fd;
    pollFds[0].events = (POLLPRI | POLLIN | POLLERR | POLLNVAL | POLLHUP);

    ret = poll(pollFds, num_fds, timeoutMsec);

    if (ret > 0 && (pollFds[0].revents & (POLLERR | POLLNVAL | POLLHUP))) {
        RK_LOGE("fd:%d polled error", fd);
        return -1;
    }

    return ret;
}

RK_S32 test_init_mpi_aenc(TEST_AENC_CTX_S *params) {
    RK_S32 s32ret = 0;
    AENC_CHN AdChn = (AENC_CHN)(params->s32ChnIndex);
    AENC_CHN_ATTR_S *pstChnAttr = RK_NULL;

    RK_U32 codecId = test_find_audio_enc_codec_id(params);
    if (codecId == -1) {
        return RK_FAILURE;
    }

    RK_U32 format = test_find_audio_enc_format(params);
    if (codecId == -1) {
        return RK_FAILURE;
    }

    params->pstChnAttr = aenc_create_chn_attr();
    pstChnAttr = params->pstChnAttr;
    aenc_fill_codec_attr(pstChnAttr, (RK_CODEC_ID_E)codecId,
        params->s32SampleRate, params->s32Channel, (AUDIO_BIT_WIDTH_E)format);

    pstChnAttr->enType = (RK_CODEC_ID_E)codecId;
    pstChnAttr->u32BufCount = 4;
    s32ret = RK_MPI_AENC_CreateChn(AdChn, pstChnAttr);
    if (s32ret) {
        RK_LOGE("create aenc chn %d err:0x%x\n", AdChn, s32ret);
        return RK_FAILURE;
    }

#if TEST_AENC_WITH_FD
    // open fd immediate after enable chn will be better.
    params->s32DevFd = RK_MPI_AENC_GetFd(AdChn);
    RK_LOGI("ai chnId: %d, selectFd:%d", (RK_S32)AdChn, params->s32DevFd);
#endif

    return RK_SUCCESS;
}

static void *send_frame_thread(void *arg) {
    RK_S32 s32ret = 0;
    TEST_AENC_CTX_S *params = reinterpret_cast<TEST_AENC_CTX_S *>(arg);
    RK_U8 *srcData = RK_NULL;
    RK_S32 srcSize = 0;
    FILE  *file = RK_NULL;
    RK_S32 frameEos = 0;
    AUDIO_FRAME_S stAudioFrm;
    ADEC_CHN AdChn = (AENC_CHN)(params->s32ChnIndex);
    RK_U64 timeStamp = 0;
    RK_S32 count = 0;
    RK_S32 frmLen = params->s32FrameSize;
    RK_S32 framesize = find_audio_frame_size(params);
    RK_U64 duration = 0;
    RK_S32 frames = 0;

    file = fopen(params->srcFilePath, "rb");
    if (file == RK_NULL) {
        RK_LOGE("failed to open input file(%s), error: %s", params->srcFilePath, strerror(errno));
        goto __FAILED;
    }

    while (1) {
        srcData = reinterpret_cast<RK_U8 *>(calloc(frmLen, sizeof(RK_U8)));
        memset(srcData, 0, frmLen);

        srcSize = fread(srcData, 1, frmLen, file);

        if (srcSize == 0 || srcData == RK_NULL) {
            RK_LOGI("read eos frame, now send eos frame!");
            frameEos = 1;
            duration = 0;
        } else {
            frames = srcSize/framesize;
            duration = ((RK_U64)frames*1000000)/params->s32SampleRate;
        }
        RK_LOGV("send frame srcSize = %d, srcData = %p, timeStamp = %lld", srcSize, srcData, timeStamp);
        stAudioFrm.u32Len = srcSize;
        stAudioFrm.u64TimeStamp = timeStamp;
        stAudioFrm.u32Seq = ++count;
        stAudioFrm.bBypassMbBlk = RK_TRUE;

        MB_EXT_CONFIG_S extConfig = {0};
        extConfig.pFreeCB = aenc_data_free;
        extConfig.pOpaque = srcData;
        extConfig.pu8VirAddr = srcData;
        extConfig.u64Size    = srcSize;
        RK_MPI_SYS_CreateMB(&(stAudioFrm.pMbBlk), &extConfig);

        s32ret = RK_MPI_AENC_SendFrame(AdChn, &stAudioFrm, RK_NULL, params->s32MilliSec);
        if (s32ret != RK_SUCCESS) {
            RK_LOGV("fail to send aenc stream.");
        }
        RK_MPI_MB_ReleaseMB(stAudioFrm.pMbBlk);

        if (frameEos)
            break;

        timeStamp += duration;
    }

__FAILED:
    if (file) {
        fclose(file);
        file = RK_NULL;
    }

    return RK_NULL;
}

static void *receive_stream_thread(void *arg) {
    RK_S32 s32ret = 0;
    FILE *file = RK_NULL;
    TEST_AENC_CTX_S *params = reinterpret_cast<TEST_AENC_CTX_S *>(arg);
    AUDIO_STREAM_S pstStream;
    AENC_CHN AdChn = (AENC_CHN)(params->s32ChnIndex);
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
#if TEST_AENC_WITH_FD
        test_aenc_poll_event(-1, params->s32DevFd);
#endif
        s32ret = RK_MPI_AENC_GetStream(AdChn, &pstStream, params->s32MilliSec);
        if (s32ret == RK_SUCCESS) {
            MB_BLK bBlk = pstStream.pMbBlk;
            RK_VOID *pstFrame = RK_MPI_MB_Handle2VirAddr(bBlk);
            RK_S32 frameSize = pstStream.u32Len;
            RK_S32 timeStamp = pstStream.u64TimeStamp;
            eos = (frameSize <= 0) ? 1 : 0;
            if (pstFrame) {
                RK_LOGV("get frame data = %p, size = %d, timeStamp = %lld",
                                                  pstFrame, frameSize, timeStamp);
                if (file) {
                    fwrite(pstFrame, frameSize, 1, file);
                    fflush(file);
                }
                RK_MPI_AENC_ReleaseStream(AdChn, &pstStream);
                count++;
            }
        } else {
            RK_LOGE("fail to get aenc frame.");
        }

        if (eos) {
            RK_LOGI("get eos stream.");
            break;
        }
    }

__FAILED:
    if (file) {
        fclose(file);
        file = RK_NULL;
    }
    return RK_NULL;
}

RK_S32 unit_test_mpi_aenc(TEST_AENC_CTX_S *params) {
    RK_S32 i = 0;
    RK_S32 counter = 0;
    TEST_AENC_CTX_S aencCtx[AENC_MAX_CHN_NUM];
    pthread_t tidSend[AENC_MAX_CHN_NUM];
    pthread_t tidReceive[AENC_MAX_CHN_NUM];

    if (params->s32ChnNum > AENC_MAX_CHN_NUM) {
        RK_LOGE("aenc chn(%d) > max_chn(%d)", params->s32ChnNum, AENC_MAX_CHN_NUM);
        goto __FAILED;
    }

    for (i = 0; i < params->s32ChnNum; i++) {
        memcpy(&(aencCtx[i]), params, sizeof(TEST_AENC_CTX_S));
        aencCtx[i].s32ChnIndex = i;
        aencCtx[i].s32MilliSec = -1;
        aencCtx[i].pstChnAttr  = RK_NULL;
        counter++;

        if (test_init_mpi_aenc(&aencCtx[i]) == RK_FAILURE) {
            goto __FAILED;
        }

        pthread_create(&tidSend[i], RK_NULL, send_frame_thread, reinterpret_cast<void *>(&aencCtx[i]));
        pthread_create(&tidReceive[i], RK_NULL, receive_stream_thread, reinterpret_cast<void *>(&aencCtx[i]));
    }

    for (i = 0; i < params->s32ChnNum; i++) {
        pthread_join(tidSend[i], RK_NULL);
        pthread_join(tidReceive[i], RK_NULL);
        RK_MPI_AENC_DestroyChn((AENC_CHN)i);
        aenc_destroy_chn_attr(&(aencCtx[i].pstChnAttr));
    }

    return RK_SUCCESS;
__FAILED:
    for (i = 0; i < counter; i++) {
        RK_MPI_AENC_DestroyChn((AENC_CHN)i);
        aenc_destroy_chn_attr(&(aencCtx[i].pstChnAttr));
    }
    return RK_FAILURE;
}

static const char *const usages[] = {
    "./rk_mpi_aenc_test [-i src_path] [-C name] [--input_rate rate] [--input_ch ch] [--input_format format]...",
    NULL,
};

static void mpi_aenc_test_show_options(const TEST_AENC_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input  file name       : %s\n", ctx->srcFilePath);
    RK_PRINT("output file name       : %s\n", ctx->dstFilePath);
    RK_PRINT("loop count             : %d\n", ctx->s32LoopCount);
    RK_PRINT("channel number         : %d\n", ctx->s32ChnNum);
    RK_PRINT("input sample rate      : %d\n", ctx->s32SampleRate);
    RK_PRINT("input channel          : %d\n", ctx->s32Channel);
    RK_PRINT("input format           : %d\n", ctx->s32Format);
    RK_PRINT("input codec name       : %s\n", ctx->chCodecId);
}

int main(int argc, const char **argv) {
    RK_S32           i;
    RK_S32           s32Ret;
    TEST_AENC_CTX_S *ctx;

    ctx = reinterpret_cast<TEST_AENC_CTX_S *>(malloc(sizeof(TEST_AENC_CTX_S)));
    memset(ctx, 0, sizeof(TEST_AENC_CTX_S));

    ctx->srcFilePath     = RK_NULL;
    ctx->dstFilePath     = RK_NULL;
    ctx->s32LoopCount    = 1;
    ctx->s32ChnNum       = 1;
    ctx->chCodecId       = RK_NULL;
    ctx->s32Format       = 16;
    ctx->s32FrameSize    = 1024;
    ctx->s32ExtCodecHandle = -1;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx->srcFilePath),
                   "input file name , e.g.(./*.mp3). <required>", NULL, 0, 0),
        OPT_STRING('C', "codec", &(ctx->chCodecId),
                    "codec, e.g.(mp3/mp2/g711a/g726). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_ch", &(ctx->s32Channel),
                    "the number of input stream channels. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_rate", &(ctx->s32SampleRate),
                    "the sample rate of input stream. <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "input_format", &(ctx->s32Format),
                    "the input format of input stream. <required>", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx->dstFilePath),
                    "output file name, e.g.(./*.pcm). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx->s32LoopCount),
                    "loop running count. default(1)", NULL, 0, 0),
        OPT_INTEGER('c', "channel_count", &(ctx->s32ChnNum),
                    "the count of adec channel. default(1).", NULL, 0, 0),
        OPT_INTEGER('\0', "frame_size", &(ctx->s32FrameSize),
                    "the size of send frame. default(1024).", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_aenc_test_show_options(ctx);

    // must set params
    if (ctx->srcFilePath == RK_NULL
        || ctx->s32Channel <= 0
        || ctx->s32SampleRate <= 0
        || ctx->chCodecId == RK_NULL) {
        argparse_usage(&argparse);
        goto __FAILED;
    }
    RK_MPI_SYS_Init();

    if (USE_EXT_AENC) {
        register_ext_aenc(ctx);
    }
    for (i = 0; i < ctx->s32LoopCount; i++) {
        RK_LOGI("start running loop count  = %d", i);
        s32Ret = unit_test_mpi_aenc(ctx);
        if (s32Ret != RK_SUCCESS) {
            goto __FAILED;
        }
        RK_LOGI("end running loop count  = %d", i);
    }

__FAILED:
    unregister_ext_aenc(ctx);
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }
    RK_MPI_SYS_Exit();
    return 0;
}
