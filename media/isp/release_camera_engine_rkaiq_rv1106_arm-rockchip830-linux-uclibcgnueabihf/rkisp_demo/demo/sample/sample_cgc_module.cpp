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

void sample_print_cgc_info(const void* arg)
{
    printf("enter CGC modult test!\n");
}

static int sample_set_cgc_attrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_uapi_acgc_attrib_t attrib;
    memset(&attrib, 0, sizeof(attrib));
    // TODO: get attrib first ?
    ret = rk_aiq_user_api2_acgc_GetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setCgcAttr failed in getting cgc attrib!\n");
    printf("cgc cgc_yuv_limit: %d\n", attrib.param.cgc_yuv_limit);

    attrib.sync.sync_mode = sync;
    /* NOTE: RK_AIQ_OP_MODE_AUTO means default value now */
    if (attrib.param.op_mode == RK_AIQ_OP_MODE_AUTO) {
        attrib.param.op_mode = RK_AIQ_OP_MODE_MANUAL;
        //limit_range coeff
        attrib.param.cgc_ratio_en  = false;
        attrib.param.cgc_yuv_limit = true;
    } else {
        attrib.param.op_mode = RK_AIQ_OP_MODE_AUTO;
    }

    ret = rk_aiq_user_api2_acgc_SetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set CGC Attr failed!");
    printf("set CGC Attr\n\n");

    return 0;
}

static int sample_get_cgc_attrib(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_uapi_acgc_attrib_t attrib;
    memset(&attrib, 0, sizeof(rk_aiq_uapi_acgc_attrib_t));

    ret = rk_aiq_user_api2_acgc_GetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get CGC Attr failed!");
    printf("get CGC Attr\n\n");
    printf("\t sync = %d, done = %d\n", attrib.sync.sync_mode, attrib.sync.done);
    if (attrib.param.op_mode == RK_AIQ_OP_MODE_AUTO) {
        printf("\t mode = Auto\n");
    } else if (attrib.param.op_mode == RK_AIQ_OP_MODE_MANUAL) {
        printf("\t mode = Manual\n");
    } else {
        printf("\t mode = Invalid\n");
    }
    printf("cgc ratio en: %s\n", attrib.param.cgc_ratio_en ? "true" : "false");
    printf("cgc yuv limit: %s\n", attrib.param.cgc_yuv_limit ? "limit" : "full");
    return 0;
}

static int sample_cgc_set_attr_async(const rk_aiq_sys_ctx_t* ctx)
{
    sample_set_cgc_attrib(ctx, RK_AIQ_UAPI_MODE_ASYNC);
    sample_get_cgc_attrib(ctx);
    usleep(40*1000);
    sample_get_cgc_attrib(ctx);

  return 0;
}

static int sample_cgc_set_attr_sync(const rk_aiq_sys_ctx_t* ctx)
{
    sample_set_cgc_attrib(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
    sample_get_cgc_attrib(ctx);

  return 0;
}

uapi_case_t cgc_uapi_list[] = {
  { .desc = "CGC: set cgc attr async",
    .func = (uapi_case_func)sample_cgc_set_attr_async
  },
  { .desc = "CGC: set cgc attr sync",
    .func = (uapi_case_func)sample_cgc_set_attr_sync
  },
  {
    .desc = NULL,
    .func = NULL,
  }
};

XCamReturn sample_cgc_module(const void* arg)
{
    int key = -1;
    CLEAR();
    const demo_context_t* demo_ctx = (demo_context_t*)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup) {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be
     * null?*/
    if (ctx == NULL) {
        ERR("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    uapi_process_loop(ctx, cgc_uapi_list);

    return XCAM_RETURN_NO_ERROR;
}
