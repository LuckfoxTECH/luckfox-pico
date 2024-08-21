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
#include "uAPI2/rk_aiq_user_api2_acsm.h"

void sample_print_csm_info(const void *arg)
{
    printf ("enter CSM modult test!\n");
}

static int sample_set_csm_attrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_uapi_acsm_attrib_t attrib;
    memset(&attrib, 0, sizeof(attrib));
    // TODO: get attrib first ?
    ret = rk_aiq_user_api2_acsm_GetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setCsmAttr failed in getting csm attrib!");
    printf("csm coeff[0]: %f\n", attrib.param.coeff[0]);

    attrib.sync.sync_mode = sync;
    /* NOTE: RK_AIQ_OP_MODE_AUTO means default value now */
    if (attrib.param.op_mode == RK_AIQ_OP_MODE_AUTO) {
        attrib.param.op_mode = RK_AIQ_OP_MODE_MANUAL;
        //limit_range coeff
        attrib.param.full_range = true;
        attrib.param.y_offset = 0;
        attrib.param.c_offset = 0;
        attrib.param.coeff[0] = 0.257;
        attrib.param.coeff[1] = 0.504;
        attrib.param.coeff[2] = 0.098;
        attrib.param.coeff[3] = -0.148;
        attrib.param.coeff[4] = -0.291;
        attrib.param.coeff[5] = 0.439;
        attrib.param.coeff[6] = 0.439;
        attrib.param.coeff[7] = -0.368;
        attrib.param.coeff[8] = -0.071;
    } else {
        attrib.param.op_mode = RK_AIQ_OP_MODE_AUTO;
    }

    ret = rk_aiq_user_api2_acsm_SetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set CSM Attr failed!");
    printf("set CSM Attr\n\n");

    return 0;
}

static int sample_get_csm_attrib(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_uapi_acsm_attrib_t attrib;
    memset(&attrib, 0, sizeof(rk_aiq_uapi_acsm_attrib_t));

    ret = rk_aiq_user_api2_acsm_GetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get CSM Attr failed!");
    printf("get CSM Attr\n\n");
    printf("\t sync = %d, done = %d\n", attrib.sync.sync_mode, attrib.sync.done);
    if (attrib.param.op_mode == RK_AIQ_OP_MODE_AUTO) {
        printf("\t mode = Auto\n");
    } else if (attrib.param.op_mode == RK_AIQ_OP_MODE_MANUAL) {
        printf("\t mode = Manual\n");
    } else {
        printf("\t mode = Invalid\n");
    }
    printf("csm range: %s\n", attrib.param.full_range ? "full" : "limit");
    printf("csm y_offset: %d\n", attrib.param.y_offset);
    printf("csm c_offset: %d\n", attrib.param.c_offset);
    printf("csm coeff:\n");
    for (int i = 0; i < 3; i+=3) {
        printf("[%f %f %f]\n",
               attrib.param.coeff[i],
               attrib.param.coeff[i + 1],
               attrib.param.coeff[i + 2]);
    }
    return 0;
}

static int sample_set_csm_cspace(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    int Cspace = -1;
    printf("\t please press the Cspace: (0-601f 1-601l 2-709f 3-709l)");

    Cspace = getchar();
    while (Cspace == '\n' || Cspace == '\r')
        Cspace = getchar();
    printf ("\n");
    Cspace = Cspace - '0';
    ret = rk_aiq_uapi2_setColorSpace(ctx, Cspace);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set colorspace failed!");
    printf("Set Cspace: %d (0-601f 1-601l 2-709f 3-709l)\n", Cspace);

    return 0;
}

static int sample_get_colorspace(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    int Cspace = -1;
    ret = rk_aiq_uapi2_getColorSpace(ctx, &Cspace);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get colorspace failed!");
    switch (Cspace)
    {
        case 0:
            printf("Color Space: BT.601 full\n\n");
            break;
        case 1:
            printf("Color Space: BT.601 limit\n\n");
            break;
        case 2:
            printf("Color Space: BT.709 full\n\n");
            break;
        case 3:
            printf("Color Space: BT.709 limit\n\n");
            break;
        default:
            printf("Color Space: other\n\n");
            break;
    }
    return 0;
}

static int sample_csm_set_attr_async(const rk_aiq_sys_ctx_t* ctx)
{
    sample_set_csm_attrib(ctx, RK_AIQ_UAPI_MODE_ASYNC);
    sample_get_csm_attrib(ctx);
    usleep(40*1000);
    sample_get_csm_attrib(ctx);

  return 0;
}

static int sample_csm_set_attr_sync(const rk_aiq_sys_ctx_t* ctx)
{
    sample_set_csm_attrib(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
    sample_get_csm_attrib(ctx);

  return 0;
}

uapi_case_t csm_uapi_list[] = {
  { .desc = "CSM: set csm attr async",
    .func = (uapi_case_func)sample_csm_set_attr_async
  },
  { .desc = "CSM: set csm attr sync",
    .func = (uapi_case_func)sample_csm_set_attr_sync
  },
  { .desc = "CSM: set csm color space",
    .func = (uapi_case_func)sample_set_csm_cspace
  },
  { .desc = "CSM: get csm color space",
    .func = (uapi_case_func)sample_get_colorspace
  },
  {
    .desc = NULL,
    .func = NULL,
  }
};

XCamReturn sample_csm_module(const void *arg)
{
    CLEAR();
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup) {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be null?*/
    if (ctx == NULL) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    uapi_process_loop(ctx, csm_uapi_list);

    return XCAM_RETURN_NO_ERROR;
}
