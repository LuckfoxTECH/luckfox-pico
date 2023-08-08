/*
 * Copyright 2022 Rockchip Electronics Co. LTD
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
#include <cstring>
#include <cstdlib>
#include "rk_debug.h"
#include "rk_mpi_amix.h"
#include "test_comm_argparse.h"

typedef struct _rkTEST_AMIX_CTX_S {
    RK_S32      s32DevId;
    const char *psControlName;
    char       *psControlValue;
    RK_BOOL     bListControls;
    RK_BOOL     bListContents;
} TEST_AMIX_CTX_S;

static RK_S32 unit_test_mpi_amix(TEST_AMIX_CTX_S *ctx) {
    RK_S32 i = 0;

    if (ctx == RK_NULL)
        return RK_FAILURE;

    if (ctx->psControlName && ctx->psControlValue) {
        RK_MPI_AMIX_SetControl(ctx->s32DevId, ctx->psControlName, ctx->psControlValue);
    } else if (ctx->psControlName && !ctx->psControlValue) {
        char value[64] = {0};
        // Usage 1: Print the current selected value of control
        RK_MPI_AMIX_GetControl(ctx->s32DevId, ctx->psControlName, value);
        RK_PRINT("Get current control value: %s\n", value);
        // Usage 2: List the values of control directly with the NULL value param
        RK_PRINT("List the control values:\n");
        RK_MPI_AMIX_GetControl(ctx->s32DevId, ctx->psControlName, NULL);
    }

    if (ctx->bListControls)
        RK_MPI_AMIX_ListControls(ctx->s32DevId);

    if (ctx->bListContents)
        RK_MPI_AMIX_ListContents(ctx->s32DevId);

    return RK_SUCCESS;
}

static const char *const usages[] = {
    "./rk_mpi_amix_test [-C card] [--control ctl_name] [--value ctl_val] [--list_controls] [--list_contents]...",
    NULL,
};

static void mpi_amix_test_show_options(const TEST_AMIX_CTX_S *ctx) {
    RK_PRINT("cmd parse result:\n");
    RK_PRINT("sound control id      : %d\n", ctx->s32DevId);
    RK_PRINT("control name          : %s\n", ctx->psControlName);
    RK_PRINT("control value         : %s\n", ctx->psControlValue);
    RK_PRINT("list controls         : %d\n", ctx->bListControls);
    RK_PRINT("list contents         : %d\n", ctx->bListContents);
}

int main(int argc, const char **argv) {
    TEST_AMIX_CTX_S  *ctx = reinterpret_cast<TEST_AMIX_CTX_S *>(malloc(sizeof(TEST_AMIX_CTX_S)));

    memset(ctx, 0, sizeof(TEST_AMIX_CTX_S));
    ctx->s32DevId           = 0;
    ctx->psControlName      = RK_NULL;
    ctx->psControlValue     = RK_NULL;
    ctx->bListControls      = RK_FALSE;
    ctx->bListContents      = RK_FALSE;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('C', "card",  &(ctx->s32DevId),
                   "specifies the card number of the mixer. default(0)", NULL, 0, 0),
        OPT_STRING('\0', "control", &(ctx->psControlName),
                    "sets (or gets without control value) the name of a control. default(required)", NULL, 0, 0),
        OPT_STRING('\0', "value", &(ctx->psControlValue),
                    "sets the value of a control. default(required)", NULL, 0, 0),
        OPT_BOOLEAN('\0', "list_controls", &(ctx->bListControls),
                    "lists controls of the mixer. default(false).", NULL, 0, 0),
        OPT_BOOLEAN('\0', "list_contents", &(ctx->bListContents),
                    "lists controls of the mixer and their contents. default(false).", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    mpi_amix_test_show_options(ctx);
    if (ctx->psControlName == RK_NULL
        && ctx->psControlValue == RK_NULL
        && ctx->bListControls == RK_FALSE
        && ctx->bListContents == RK_FALSE) {
        argparse_usage(&argparse);
        return RK_FAILURE;
    }

    unit_test_mpi_amix(ctx);

    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }
    return RK_SUCCESS;
}
