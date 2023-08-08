/*
 *  Copyright (c) 2022 Rockchip Corporation
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

#include "sample_comm.h"
#include "uAPI2/rk_aiq_user_api2_acgc.h"

static void sample_cgc_usage()
{
    printf("Usage : \n");
    printf("\t 0) CGC:         Set CGC MANUAL Mode.\n");
    printf("\t 1) CGC:         Set CGC AUTO Mode.\n");
    printf("\t h) CGC:         help.\n");
    printf("\t q) CGC:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_cgc_info(const void* arg)
{
    printf("enter CGC modult test!\n");
}

static int sample_set_cgc_manual(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_uapi_acgc_attrib_t attrib;
    memset(&attrib, 0, sizeof(attrib));
    // TODO: get attrib first ?
    ret = rk_aiq_user_api2_acgc_GetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setCgcAttr failed in getting cgc attrib!");
    attrib.sync.sync_mode = RK_AIQ_UAPI_MODE_ASYNC;
    /* NOTE: RK_AIQ_OP_MODE_AUTO means default value now */
    attrib.param.op_mode       = RK_AIQ_OP_MODE_MANUAL;
    attrib.param.cgc_ratio_en  = false;
    attrib.param.cgc_yuv_limit = true;

    ret = rk_aiq_user_api2_acgc_SetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set CGC Attr failed!");
    return 0;
}

static int sample_set_cgc_auto(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi_acgc_attrib_t attrib;
    memset(&attrib, 0, sizeof(attrib));
    // TODO: get attrib first ?
    attrib.sync.sync_mode = RK_AIQ_UAPI_MODE_ASYNC;
    /* NOTE: RK_AIQ_OP_MODE_AUTO means default value now */
    attrib.param.op_mode = RK_AIQ_OP_MODE_AUTO;

    rk_aiq_user_api2_acgc_SetAttrib(ctx, &attrib);

    return 0;
}

static int sample_get_cgc_attrib(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi_acgc_attrib_t attrib;

    rk_aiq_user_api2_acgc_GetAttrib(ctx, &attrib);
    printf("cgc ratio en: %s\n", attrib.param.cgc_ratio_en ? "true" : "false");
    printf("cgc yuv limit: %s\n", attrib.param.cgc_yuv_limit ? "limit" : "full");

    return 0;
}

XCamReturn sample_cgc_module(const void* arg)
{
    int key = -1;
    CLEAR();
    const demo_context_t* demo_ctx = (demo_context_t*)arg;
    const rk_aiq_sys_ctx_t* ctx    = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be
     * null?*/
    if (ctx == NULL) {
        ERR("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_cgc_usage();

    do {
        key = getchar();
        while (key == '\n' || key == '\r') key = getchar();
        printf("\n");

        switch (key) {
            case 'h':
                sample_cgc_usage();
                break;
            case '0':
                sample_set_cgc_manual(ctx);
                sample_get_cgc_attrib(ctx);
                printf("Set CGC MANUAL Mode\n\n");
                break;
            case '1':
                sample_set_cgc_auto(ctx);
                sample_get_cgc_attrib(ctx);
                printf("Set CGC AUTO Mode\n\n");
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
