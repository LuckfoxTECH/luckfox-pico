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

static void sample_csm_usage()
{
    printf("Usage : \n");
    printf("\t 0) CSM:         Set CSM MANUAL Mode.\n");
    printf("\t 1) CSM:         Set CSM AUTO Mode.\n");
    printf("\t h) CSM:         help.\n");
    printf("\t q) CSM:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_csm_info(const void *arg)
{
    printf ("enter CSM modult test!\n");
}

static int sample_set_csm_manual(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_uapi_acsm_attrib_t attrib;
    memset(&attrib, 0, sizeof(attrib));
    // TODO: get attrib first ?
    ret = rk_aiq_user_api2_acsm_GetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setCsmAttr failed in getting csm attrib!");
    attrib.sync.sync_mode = RK_AIQ_UAPI_MODE_ASYNC;
    /* NOTE: RK_AIQ_OP_MODE_AUTO means default value now */
    attrib.param.op_mode = RK_AIQ_OP_MODE_MANUAL;
    // limit range param
    attrib.param.full_range = false;
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

    ret = rk_aiq_user_api2_acsm_SetAttrib(ctx, &attrib);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set CSM Attr failed!");
    return 0;
}

static int sample_set_csm_auto(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi_acsm_attrib_t attrib;
    memset(&attrib, 0, sizeof(attrib));
    // TODO: get attrib first ?
    attrib.sync.sync_mode = RK_AIQ_UAPI_MODE_ASYNC;
    /* NOTE: RK_AIQ_OP_MODE_AUTO means default value now */
    attrib.param.op_mode = RK_AIQ_OP_MODE_AUTO;

    rk_aiq_user_api2_acsm_SetAttrib(ctx, &attrib);

    return 0;
}

static int sample_get_csm_attrib(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi_acsm_attrib_t attrib;

    rk_aiq_user_api2_acsm_GetAttrib(ctx, &attrib);
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

XCamReturn sample_csm_module(const void *arg)
{
    int key = -1;
    CLEAR();
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be null?*/
    if (ctx == NULL) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_csm_usage ();

    do {

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                sample_csm_usage ();
                break;
            case '0':
                sample_set_csm_manual(ctx);
                sample_get_csm_attrib(ctx);
                printf("Set CSM MANUAL Mode\n\n");
                break;
            case '1':
                sample_set_csm_auto(ctx);
                sample_get_csm_attrib(ctx);
                printf("Set CSM AUTO Mode\n\n");
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
