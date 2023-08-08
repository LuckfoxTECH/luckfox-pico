/*
 * Copyright (c) 2022 Rockchip Eletronics Co., Ltd.
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
#include "sample_agic_module.h"

#include "sample_comm.h"
#include "uAPI2/rk_aiq_user_api2_agic.h"

static void sample_agic_usage() {
    printf("Usage : \n");
    printf("  Module API: \n");
    printf("\t a) GIC:         Set gic Attr & Sync .\n");
    printf("\t b) GIC:         Set gic Attr & Async .\n");
    printf("\n");
    printf("\t h) GIC:         help.\n");
    printf("\t q) GIC:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_agic_info(const void* arg) { printf("enter GIC modult test!\n"); }

/*
******************************
*
* Module level API Sample Func
*
******************************
*/

static int sample_gic_setgicAttr(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rkaiq_gic_v2_api_attr_t attr;

    memset(&attr, 0, sizeof(rkaiq_gic_v2_api_attr_t));
    ret = rk_aiq_user_api2_agic_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setgicAttr failed in getting agic attrib!");

    memcpy(&attr.manual_param, &attr.auto_params[0], sizeof(rkaiq_gic_v2_param_selected_t));
    attr.sync.sync_mode = sync;
    attr.op_mode        = RKAIQ_GIC_API_OPMODE_MANUAL;
    attr.gic_en         = 0;
    attr.manual_param.globalStrength = 0.5;

    ret = rk_aiq_user_api2_agic_v2_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set GIC Attr failed!");
    printf("set GIC Attr\n\n");

    return 0;
}

static int sample_gic_getgicAttr(const rk_aiq_sys_ctx_t* ctx) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rkaiq_gic_v2_api_attr_t attr;

    memset(&attr, 0, sizeof(rkaiq_gic_v2_api_attr_t));
    ret = rk_aiq_user_api2_agic_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get gic Attr failed!");
    printf("get GIC Attr:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t Mode = %s\n",
           (attr.op_mode == RKAIQ_GIC_API_OPMODE_AUTO
                ? "auto"
                : ((attr.op_mode == RKAIQ_GIC_API_OPMODE_MANUAL) ? "manual" : "off")));
    printf("\t globalStrength = %f\n", attr.manual_param.globalStrength);

    return 0;
}

XCamReturn sample_agic_module(const void* arg) {
    int key = -1;
    CLEAR();

    const demo_context_t* demo_ctx = (demo_context_t*)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup) {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    if (ctx == NULL) {
        ERR("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_agic_usage();

    do {
        key = getchar();
        while (key == '\n' || key == '\r') key = getchar();
        printf("\n");

        switch (key) {
            case 'h':
                sample_agic_usage();
                CLEAR();
                break;
            case 'a':
                sample_gic_setgicAttr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_gic_getgicAttr(ctx);
                break;
            case 'b':
                sample_gic_setgicAttr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_gic_getgicAttr(ctx);
                usleep(40 * 1000);
                sample_gic_getgicAttr(ctx);
                break;
            default:
                CLEAR();
                sample_agic_usage();
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
