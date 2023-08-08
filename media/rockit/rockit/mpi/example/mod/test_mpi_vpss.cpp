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
#ifdef DBG_MOD_ID
#undef DBG_MOD_ID
#endif
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_VPSS)

#include <string.h>

#include "test_comm_argparse.h"
#include "test_mod_vpss.h"

#include "rk_debug.h"
#include "rk_mpi_sys.h"

static const char *const usages[] = {
    "./rk_mpi_vpss_test [-i SRC_PATH] [-w SRC_WIDTH] [-h SRC_HEIGHT] [-W DST_WIDTH] [-H DST_HEIGHT]...",
    NULL,
};

static void mpi_vpss_test_show_options(const TEST_VPSS_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("input  file name       : %s\n", ctx->srcFileName);
    RK_PRINT("output file name       : %s\n", ctx->dstFilePath);
    RK_PRINT("loop count             : %d\n", ctx->s32LoopCount);
    RK_PRINT("video process device   : %d\n", ctx->s32VProcDevType);
    RK_PRINT("group number           : %d\n", ctx->s32GrpNum);
    RK_PRINT("channel number         : %d\n", ctx->s32ChnNum);
    RK_PRINT("group crop enabled     : %d\n", ctx->bGrpCropEn ? 1 : 0);
    RK_PRINT("chn crop enabled       : %d\n", ctx->bChnCropEn ? 1 : 0);
    RK_PRINT("group crop ratio       : %d\n", ctx->s32GrpCropRatio);
    RK_PRINT("chn crop ratio         : %d\n", ctx->s32ChnCropRatio);
    RK_PRINT("input width            : %d\n", ctx->s32SrcWidth);
    RK_PRINT("input height           : %d\n", ctx->s32SrcHeight);
    RK_PRINT("input virtual width    : %d\n", ctx->s32SrcVirWidth);
    RK_PRINT("input virtual height   : %d\n", ctx->s32SrcVirHeight);
    RK_PRINT("input compress mode    : %d\n", ctx->s32SrcCompressMode);
    RK_PRINT("input pixel format     : %d\n", ctx->s32SrcPixFormat);
    RK_PRINT("output width           : %d\n", ctx->s32DstWidth);
    RK_PRINT("output height          : %d\n", ctx->s32DstHeight);
    RK_PRINT("output compress mode   : %d\n", ctx->s32DstCompressMode);
    RK_PRINT("output pixel format    : %d\n", ctx->s32DstPixFormat);
    RK_PRINT("fixed rotation         : %d\n", ctx->s32Rotation);
    RK_PRINT("any rotation angle     : %d\n", ctx->s32RotationEx);
    RK_PRINT("enable mirror          : %d\n", ctx->s32Mirror);
    RK_PRINT("enable flip            : %d\n", ctx->s32Flip);
    RK_PRINT("enable attach mb pool  : %d\n", ctx->bAttachPool);
}

RK_S32 main(int argc, const char **argv) {
    RK_S32          s32Ret;
    TEST_VPSS_CTX_S ctx;

    memset(&ctx, 0, sizeof(TEST_VPSS_CTX_S));

    //  set default params.
    ctx.dstFilePath     = RK_NULL;
    ctx.s32LoopCount    = 1;
    ctx.s32VProcDevType = VIDEO_PROC_DEV_GPU;
    ctx.s32GrpNum       = 1;
    ctx.s32ChnNum       = 1;
    ctx.bGrpCropEn      = RK_FALSE;
    ctx.bChnCropEn      = RK_FALSE;
    ctx.s32GrpCropRatio = 1000;
    ctx.s32ChnCropRatio = 1000;
    ctx.s32SrcCompressMode  = 0;
    ctx.s32SrcPixFormat     = RK_FMT_YUV420SP;
    ctx.s32DstCompressMode  = 0;
    ctx.s32DstPixFormat     = RK_FMT_YUV420SP;
    ctx.s32GrpIndex         = 0;
    ctx.u32ChnDepth         = 8;
    ctx.s32SrcChnRate       = -1;
    ctx.s32DstChnRate       = -1;
    ctx.s32SrcGrpRate       = -1;
    ctx.s32DstGrpRate       = -1;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_STRING('i', "input",  &(ctx.srcFileName),
                   "input file name. e.g.(/userdata/1080p.nv12). default(NULL)", NULL, 0, 0),
        OPT_STRING('o', "output", &(ctx.dstFilePath),
                    "output file path. e.g.(/userdata/vpss/). default(NULL).", NULL, 0, 0),
        OPT_INTEGER('n', "loop_count", &(ctx.s32LoopCount),
                    "loop running count. default(1)", NULL, 0, 0),
        OPT_INTEGER('\0', "video_proc_dev_type", &(ctx.s32VProcDevType),
                    "the device type of video process. default(0. 0 is GPU, 1 is RGA)", NULL, 0, 0),
        OPT_INTEGER('g', "group_count", &(ctx.s32GrpNum),
                    "the count of vpss group. default(1).", NULL, 0, 0),
        OPT_INTEGER('c', "channel_count", &(ctx.s32ChnNum),
                    "the count of vpss channel. default(1).", NULL, 0, 0),
        OPT_BOOLEAN('\0', "group_crop_en", &(ctx.bGrpCropEn),
                    "vpss group crop is enabled. default(0).", NULL, 0, 0),
        OPT_BOOLEAN('\0', "channel_crop_en", &(ctx.bChnCropEn),
                    "vpss channel crop is enabled. default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "group_crop_ratio", &(ctx.s32GrpCropRatio),
                    "vpss group crop ratio. range(1,1000). default(1000)", NULL, 0, 0),
        OPT_INTEGER('\0', "channel_crop_ratio", &(ctx.s32ChnCropRatio),
                    "vpss channel crop ratio. range(1,1000). default(1000)", NULL, 0, 0),
        OPT_INTEGER('w', "src_width", &(ctx.s32SrcWidth),
                    "src width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('h', "src_height", &(ctx.s32SrcHeight),
                    "src height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('\0', "src_vir_width", &(ctx.s32SrcVirWidth),
                    "src virtual width. e.g.(1920). default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "src_vir_height", &(ctx.s32SrcVirHeight),
                    "src virtual height. e.g.(1080). default(0)", NULL, 0, 0),
        OPT_INTEGER('m', "src_compress", &(ctx.s32SrcCompressMode),
                    "src compress mode. default(0).", NULL, 0, 0),
        OPT_INTEGER('f', "src_format", &(ctx.s32SrcPixFormat),
                    "src pixel format. default(0. 0 is NV12).", NULL, 0, 0),
        OPT_INTEGER('W', "dst_width", &(ctx.s32DstWidth),
                    "dst width. e.g.(1920). <required>", NULL, 0, 0),
        OPT_INTEGER('H', "dst_height", &(ctx.s32DstHeight),
                    "dst height. e.g.(1080). <required>", NULL, 0, 0),
        OPT_INTEGER('M', "dst_compress", &(ctx.s32DstCompressMode),
                    "dst compress mode. default(0).", NULL, 0, 0),
        OPT_INTEGER('F', "dst_format", &(ctx.s32DstPixFormat),
                    "dst pixel format. default(0. 0 is NV12).", NULL, 0, 0),
        OPT_INTEGER('r', "rotation", &(ctx.s32Rotation),
                    "fixed rotation angle. default(0). 0: 0. 1: 90. 2: 180. 3: 270", NULL, 0, 0),
        OPT_INTEGER('R', "rotation_ex", &(ctx.s32RotationEx),
                    "any rotation angle. default(0). ", NULL, 0, 0),
        OPT_INTEGER('a', "attach_mb_pool", &(ctx.bAttachPool),
                    "enable attach mb pool or not, default(0), 0: RK_FALSE, 1: RK_TRUE", NULL, 0, 0),
        OPT_INTEGER('\0', "chn_mode", &(ctx.s32ChnMode),
                    "channel mode, default(0), 0: USER, 1: AUTO, 2: PASS-THOUGH", NULL, 0, 0),
        OPT_INTEGER('d', "chn_depth", &(ctx.u32ChnDepth),
                    "channel output depth, default(8)", NULL, 0, 0),
        OPT_INTEGER('\0', "mirror", &(ctx.s32Mirror),
                    "picture mirror, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "flip", &(ctx.s32Flip),
                    "picture flip, default(0)", NULL, 0, 0),
        OPT_INTEGER('\0', "src_chn_rate", &(ctx.s32SrcChnRate),
                    "src vpss channel frame rate, default(-1)", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_chn_rate", &(ctx.s32DstChnRate),
                    "dst vpss channel frame rate, default(-1)", NULL, 0, 0),
        OPT_INTEGER('\0', "src_grp_rate", &(ctx.s32SrcGrpRate),
                    "src vpss group frame rate, default(-1)", NULL, 0, 0),
        OPT_INTEGER('\0', "dst_grp_rate", &(ctx.s32DstGrpRate),
                    "dst vpss group frame rate, default(-1)", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_vpss_test_show_options(&ctx);

    if (ctx.s32SrcWidth <= 0
        || ctx.s32SrcHeight <= 0
        || ctx.s32DstWidth <= 0
        || ctx.s32DstHeight <= 0) {
        argparse_usage(&argparse);
        return RK_FAILURE;
    }

    s32Ret = RK_MPI_SYS_Init();
    if (s32Ret != RK_SUCCESS) {
        return s32Ret;
    }

    s32Ret = TEST_VPSS_ModTest(&ctx);
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
