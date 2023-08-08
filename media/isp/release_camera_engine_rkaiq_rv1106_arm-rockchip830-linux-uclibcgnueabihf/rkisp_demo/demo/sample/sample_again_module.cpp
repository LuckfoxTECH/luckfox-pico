/*
 *  Copyright (c) 2019 Rockchip Corporation
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

static void sample_again_usage()
{
    printf("Usage : \n");
    printf("\t 0) AGAIN:         get again attri on sync mode.\n");
    printf("\t 1) AGAIN:         set again attri auto on sync mode.\n");
    printf("\t 2) AGAIN:         set again attri manual on sync mode.\n");
    printf("\t 3) AGAIN:         set again attri to default vaule on sync mode.\n");
    printf("\t 4) AGAIN:         get again attri on async mode.\n");
    printf("\t 5) AGAIN:         set again attri auto on async mode.\n");
    printf("\t 6) AGAIN:         set again attri manual on async mode.\n");
    printf("\t 7) AGAIN:         set again attri to default vaule on async mode.\n");
    printf("\t q) AGAIN:         press key q or Q to quit.\n");

}

void sample_print_again_info(const void *arg)
{
    printf ("enter AGAIN modult test!\n");
}

XCamReturn sample_again_getAttr_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_gain_attrib_v2_t gainV2_attr;
    gainV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_againV2_GetAttrib(ctx, &gainV2_attr);
    printf("get again v2 attri ret:%d done:%d\n\n", ret, gainV2_attr.sync.done);

    return ret;
}

XCamReturn sample_again_setAuto_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_gain_attrib_v2_t gainV2_attr;
    gainV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_againV2_GetAttrib(ctx, &gainV2_attr);
    gainV2_attr.sync.sync_mode = sync_mode;
    gainV2_attr.eMode = AGAINV2_OP_MODE_AUTO;
    gainV2_attr.stAuto.stParams.hdrgain_ctrl_enable = 1;
    for(int i = 0; i < RK_GAIN_V2_MAX_ISO_NUM; i++) {
        gainV2_attr.stAuto.stParams.iso[i] = 50 * pow(2, i);

        gainV2_attr.stAuto.stParams.iso_params[i].hdr_gain_scale_s = 1.0;
        gainV2_attr.stAuto.stParams.iso_params[i].hdr_gain_scale_m = 1.0;
    }

    ret = rk_aiq_user_api2_againV2_SetAttrib(ctx, &gainV2_attr);
    printf("set again attri auto ret:%d \n\n", ret);

    rk_aiq_gain_attrib_v2_t get_gainV2_attr;
    get_gainV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_againV2_GetAttrib(ctx, &get_gainV2_attr);
    printf("get again v2 attri ret:%d done:%d\n\n", ret, get_gainV2_attr.sync.done);
    return ret;
}

XCamReturn sample_again_setManual_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_gain_attrib_v2_t gainV2_attr;
    gainV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_againV2_GetAttrib(ctx, &gainV2_attr);
    gainV2_attr.sync.sync_mode = sync_mode;
    gainV2_attr.eMode = AGAINV2_OP_MODE_MANUAL;
    gainV2_attr.stManual.stSelect.hdrgain_ctrl_enable = 1;

    gainV2_attr.stManual.stSelect.hdr_gain_scale_s = 1.0;
    gainV2_attr.stManual.stSelect.hdr_gain_scale_m = 1.0;

    ret = rk_aiq_user_api2_againV2_SetAttrib(ctx, &gainV2_attr);
    printf("set gain v2 attri manual ret:%d \n\n", ret);

    rk_aiq_gain_attrib_v2_t get_gainV2_attr;
    get_gainV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_againV2_GetAttrib(ctx, &get_gainV2_attr);
    printf("get again v2 attri ret:%d done:%d\n\n", ret, get_gainV2_attr.sync.done);

    return ret;
}

XCamReturn sample_again_setDefault_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_gain_attrib_v2_t& default_gainV2_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    default_gainV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_againV2_SetAttrib(ctx, &default_gainV2_attr);
    printf("Set again v2 set default attri ret:%d \n\n", ret);

    rk_aiq_gain_attrib_v2_t get_gainV2_attr;
    get_gainV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_againV2_GetAttrib(ctx, &get_gainV2_attr);
    printf("get again v2 attri ret:%d done:%d\n\n", ret, get_gainV2_attr.sync.done);

    return ret;
}

XCamReturn sample_again_module (const void *arg)
{
    int key = -1;
    CLEAR();
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx ;

    if(demo_ctx->camGroup) {
        ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
        printf("##################group !!!!########################\n");
    } else {
        ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
        printf("##################sigle !!!!########################\n");
    }

    if (ctx == nullptr) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    rk_aiq_gain_attrib_v2_t default_gainV2_attr;
    if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
        ret = rk_aiq_user_api2_againV2_GetAttrib(ctx, &default_gainV2_attr);
        printf("get again v2 default attri ret:%d \n\n", ret);
    }

    do {
        sample_again_usage ();

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar ();
        printf ("\n");

        switch (key) {
        case '0':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_getAttr_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '1':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_setAuto_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '2':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_setManual_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '3':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_setDefault_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, default_gainV2_attr);
            }
            break;
        case '4':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_getAttr_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case '5':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_setAuto_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case '6':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_setManual_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case '7':
            if (CHECK_ISP_HW_V30() || CHECK_ISP_HW_V32()) {
                sample_again_setDefault_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, default_gainV2_attr);
            }
            break;
        default:
            printf("not support test\n\n");
            break;
        }

    } while (key != 'q' && key != 'Q');

    return ret;
}

