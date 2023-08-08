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

static const uint8_t default_bic_table[9][4] = {
    {0x00, 0x80, 0x00, 0x00}, // table0: 0, 0, 128, 0
    {0xfc, 0x7f, 0x05, 0x00}, // table1: 0, 5, 127, -4
    {0xfa, 0x7b, 0x0c, 0xff}, // table2: -1, 12, 123, -6
    {0xf8, 0x76, 0x14, 0xfe}, // table3: -2, 20, 118, -8
    {0xf7, 0x6f, 0x1d, 0xfd}, // table4: -3, 29, 111, -9
    {0xf7, 0x66, 0x27, 0xfc}, // table4: -4, 39, 102, -9
    {0xf7, 0x5d, 0x32, 0xfa}, // table4: -6, 50, 93, -9
    {0xf7, 0x53, 0x3d, 0xf9}, // table4: -7, 61, 83, -9
    {0xf8, 0x48, 0x48, 0xf8}, // table4: -8, 72, 72, -8
};

static const uint8_t bic_weight_table[9][4] = {
    {0x20, 0x20, 0x20, 0x20}, // table0: 0, 0, 128, 0
    {0x20, 0x20, 0x20, 0x20}, // table1: 0, 5, 127, -4
    {0x20, 0x20, 0x20, 0x20}, // table2: -1, 12, 123, -6
    {0x20, 0x20, 0x20, 0x20}, // table3: -2, 20, 118, -8
    {0x20, 0x20, 0x20, 0x20}, // table4: -3, 29, 111, -9
    {0x20, 0x20, 0x20, 0x20}, // table4: -4, 39, 102, -9
    {0x20, 0x20, 0x20, 0x20}, // table4: -6, 50, 93, -9
    {0x20, 0x20, 0x20, 0x20}, // table4: -7, 61, 83, -9
    {0x20, 0x20, 0x20, 0x20}, // table4: -8, 72, 72, -8
};

static void sample_aldch_v21_usage()
{
    printf("Usage : \n");
    printf("\t 0) ALDCH_V21:         enable/disable ldch.\n");
    printf("\t 1) ALDCH_V21:         test the correction level of ALDCH_V21 iteratively in sync.\n");
    printf("\t 2) ALDCH_V21:         test the correction level of ALDCH_V21 iteratively in async.\n");
    printf("\t 3) ALDCH_V21:         enable/disable bic mode.\n");
    printf("\t 4) ALDCH_V21:         enable/disable zero_interp.\n");
    printf("\t 5) ALDCH_V21:         enable/disable sample_avr.\n");
    printf("\t 6) ALDCH_V21:         set all the weight table of bicubic to 0x20.\n");
    printf("\n");

    printf("\t h) ALDCH_V21: help.\n");
    printf("\t q/Q) ALDCH_V21:       return to main sample screen.\n");
    printf("\n");
    printf("\t please press the key: \n\n");

    return;
}

void sample_print_aldch_v21_info(const void *arg)
{
    printf ("enter ALDCH test!\n");
}

XCamReturn sample_aldch_v21_en(const rk_aiq_sys_ctx_t* ctx, bool en)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_v21_attrib_t ldchAttr;
    ret = rk_aiq_user_api2_aldch_v21_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    ldchAttr.en = en;
    ret = rk_aiq_user_api2_aldch_v21_SetAttrib(ctx, &ldchAttr);
    return ret;
}

XCamReturn sample_aldch_v21_setCorrectLevel(const rk_aiq_sys_ctx_t* ctx, int correctLevel,  rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_v21_attrib_t ldchAttr;
    memset(&ldchAttr, 0, sizeof(ldchAttr));
    ret = rk_aiq_user_api2_aldch_v21_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    ldchAttr.sync.sync_mode = sync;
    ldchAttr.correct_level = correctLevel;
    ret = rk_aiq_user_api2_aldch_v21_SetAttrib(ctx, &ldchAttr);
    ret = rk_aiq_user_api2_aldch_v21_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");

    printf ("sync_mode: %d, level: %d, done: %d\n", ldchAttr.sync.sync_mode, correctLevel, ldchAttr.sync.done);

    return ret;
}

XCamReturn sample_aldch_v21_bic_mode_en(const rk_aiq_sys_ctx_t* ctx, bool en)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_v21_attrib_t ldchAttr;
    ret = rk_aiq_user_api2_aldch_v21_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    ldchAttr.bic_mode_en = en;
    ret = rk_aiq_user_api2_aldch_v21_SetAttrib(ctx, &ldchAttr);
    return ret;
}

XCamReturn sample_aldch_v21_zero_interp_en(const rk_aiq_sys_ctx_t* ctx, bool en)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_v21_attrib_t ldchAttr;
    ret = rk_aiq_user_api2_aldch_v21_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    ldchAttr.zero_interp_en = en;
    ret = rk_aiq_user_api2_aldch_v21_SetAttrib(ctx, &ldchAttr);
    return ret;
}

XCamReturn sample_aldch_v21_sample_avr_en(const rk_aiq_sys_ctx_t* ctx, bool en)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_v21_attrib_t ldchAttr;
    ret = rk_aiq_user_api2_aldch_v21_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    ldchAttr.sample_avr_en = en;
    ret = rk_aiq_user_api2_aldch_v21_SetAttrib(ctx, &ldchAttr);
    return ret;
}

XCamReturn sample_aldch_v21_bic_weight_table(const rk_aiq_sys_ctx_t* ctx, bool isSwitch)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (ctx == NULL) {
        ret = XCAM_RETURN_ERROR_PARAM;
        RKAIQ_SAMPLE_CHECK_RET(ret, "param error!");
    }
    rk_aiq_ldch_v21_attrib_t ldchAttr;
    ret = rk_aiq_user_api2_aldch_v21_GetAttrib(ctx, &ldchAttr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get ldch attrib failed!");
    if (isSwitch)
        memcpy(ldchAttr.bic_weight, bic_weight_table, sizeof(bic_weight_table));
    else
        memcpy(ldchAttr.bic_weight, default_bic_table, sizeof(default_bic_table));
    ret = rk_aiq_user_api2_aldch_v21_SetAttrib(ctx, &ldchAttr);
    return ret;
}

XCamReturn sample_aldch_v21_module (const void *arg)
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

    sample_aldch_v21_usage ();
    do {
        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar ();
        printf ("\n");

        switch (key)
        {
        case 'h':
            CLEAR();
            sample_aldch_v21_usage ();
            break;
        case '0': {
            static bool on = false;
            on = !on;
            sample_aldch_v21_en(ctx, on);
            printf("%s aldch\n\n", on ? "enable" : "disable");
            break;
        }
        case '1':
            printf("test the correction level of ALDCH iteratively in sync mode...\n");
            for (int level = 1; level <= 255; level++) {
                usleep(100*1000);
                sample_aldch_v21_setCorrectLevel(ctx, level, RK_AIQ_UAPI_MODE_DEFAULT);
            }
            printf("end of the test\n\n");
            break;
        case '2':
            printf("test the correction level of ALDCH iteratively in async mode...\n");
            for (int level = 1; level <= 255; level++) {
                usleep(100*1000);
                sample_aldch_v21_setCorrectLevel(ctx, level, RK_AIQ_UAPI_MODE_ASYNC);
            }
            printf("end of the test\n\n");
            break;
        case '3': {
            static bool on = false;
            on = !on;
            sample_aldch_v21_bic_mode_en(ctx, on);
            printf("%s aldch bic_mode_en\n\n", on ? "enable" : "disable");
            break;
        }
        case '4': {
            static bool on = false;
            on = !on;
            sample_aldch_v21_zero_interp_en(ctx, on);
            printf("%s aldch zero_interp_en\n\n", on ? "enable" : "disable");
            break;
        }
        case '5': {
            static bool on = false;
            on = !on;
            sample_aldch_v21_sample_avr_en(ctx, on);
            printf("%s aldch sample_avr_en\n\n", on ? "enable" : "disable");
            break;
        }
        case '6': {
            static bool isSwitch = false;
            isSwitch = !isSwitch;
            sample_aldch_v21_bic_weight_table(ctx, isSwitch);
            printf("aldch switch bic weight table to %s\n\n", isSwitch ? "0x20" : "default");
            break;
        }
        default:
            break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
