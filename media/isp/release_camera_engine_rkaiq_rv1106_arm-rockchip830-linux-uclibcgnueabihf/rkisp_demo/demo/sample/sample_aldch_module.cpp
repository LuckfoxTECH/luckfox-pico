/*
 *  Copyright (c) 2021 Rockchip Corporation
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

static void sample_aldch_usage()
{
    printf("Usage : \n");
    printf("\t 0) ALDCH:         enable/disable ldch.\n");
    printf("\t 1) ALDCH:         test the correction level of ALDCH iteratively in sync.\n");
    printf("\t 2) ALDCH:         test the correction level of ALDCH iteratively in async.\n");
    printf("\n");

    printf("\t h) ALDCH: help.\n");
    printf("\t q/Q) ALDCH:       return to main sample screen.\n");
    printf("\n");
    printf("\t please press the key: \n\n");

    return;
}

void sample_print_aldch_info(const void *arg)
{
    printf ("enter ALDCH test!\n");
}

XCamReturn sample_aldch_en(const rk_aiq_sys_ctx_t* ctx, bool en)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_attrib_t ldchAttr;
    ret = rk_aiq_user_api2_aldch_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    ldchAttr.en = en;
    ret = rk_aiq_user_api2_aldch_SetAttrib(ctx, &ldchAttr);
    return ret;
}

XCamReturn sample_aldch_setCorrectLevel(const rk_aiq_sys_ctx_t* ctx, int correctLevel,  rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_attrib_t ldchAttr;
    memset(&ldchAttr, 0, sizeof(ldchAttr));
    ret = rk_aiq_user_api2_aldch_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    ldchAttr.sync.sync_mode = sync;
    ldchAttr.correct_level = correctLevel;
    ret = rk_aiq_user_api2_aldch_SetAttrib(ctx, &ldchAttr);
    ret = rk_aiq_user_api2_aldch_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");

    printf ("sync_mode: %d, done: %d\n", ldchAttr.sync.sync_mode, ldchAttr.sync.done);

    return ret;
}

XCamReturn sample_aldch_module (const void *arg)
{
    int key = -1;
    CLEAR();

    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup) {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    if (ctx == nullptr) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_aldch_usage ();
    do {
        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar ();
        printf ("\n");

        switch (key)
        {
        case 'h':
            CLEAR();
            sample_aldch_usage ();
            break;
        case '0': {
            static bool on = false;
            on = !on;
            sample_aldch_en(ctx, on);
            printf("%s aldch\n\n", on ? "enable" : "disable");
            break;
        }
        case '1':
            printf("test the correction level of ALDCH iteratively in sync mode...\n");
            for (int level = 1; level <= 255; level++) {
                usleep(100*1000);
                sample_aldch_setCorrectLevel(ctx, level, RK_AIQ_UAPI_MODE_DEFAULT);
            }
            printf("end of the test\n\n");
            break;
        case '2':
            printf("test the correction level of ALDCH iteratively in async mode...\n");
            for (int level = 1; level <= 255; level++) {
                usleep(100*1000);
                sample_aldch_setCorrectLevel(ctx, level, RK_AIQ_UAPI_MODE_ASYNC);
            }
            printf("end of the test\n\n");
            break;
        default:
            break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
