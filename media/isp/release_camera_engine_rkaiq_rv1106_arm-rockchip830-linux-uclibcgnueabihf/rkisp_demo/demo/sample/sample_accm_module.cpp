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

static void sample_accm_usage()
{
    printf("Usage : \n");
    printf("  ImgProc API: \n");
    printf("\t 0) CCM:         Set CCM MANUAL Mode.\n");
    printf("\t 1) CCM:         Set CCM AUTO Mode.\n");
    printf("\t 2) CCM:         Get CCM Mode.\n");
    printf("\t 3) CCM:         Set CCM Manual Matrix.\n");
    printf("\t 4) CCM:         Get CCM Matrix.\n");
    printf("\t 5) CCM:         Get ACCM Sat-level.\n");
    printf("\t 6) CCM:         Get ACCM Matrix Name.\n");
    printf("  Module API: \n");
	printf("\t a) CCM:         Set Ccm Attr & Sync .\n");
    printf("\t b) CCM:         Set Ccm Attr & Async .\n");
    printf("\t c) CCM:         Set Mode Manual & Sync.\n");
    printf("\t d) CCM:         Set Mode Manual & Async.\n");
    printf("\t e) CCM:         Set Mode Auto & Sync.\n");
    printf("\t f) CCM:         Set Mode Auto & Async.\n");
    printf("\t g) CCM:         Set Manual attr & Sync.\n");
    printf("\t i) CCM:         Set Manual attr & Async.\n");
    printf("\t j) CCM:         Set Auto attr & Sync.\n");
    printf("\t k) CCM:         Set Auto attr & Async.\n");
    printf("\t l) CCM:         Query CCM Info.\n");
    printf("\n");
    printf("\t h) CCM:         help.\n");
    printf("\t q) CCM:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_accm_info(const void *arg)
{
    printf ("enter CCM module test!\n");
}

void sample_print_accm_v2_info(const void *arg)
{
    printf ("enter CCM V2 module test!\n");
}

/*
******************************
*
* ImgProc level API Sample Func
*
******************************
*/

static int sample_set_ccm_manual(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode = OP_MANUAL;
    rk_aiq_uapi2_setCCMMode(ctx, mode);
    return 0;
}

static int sample_set_ccm_auto(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode = OP_AUTO;
    rk_aiq_uapi2_setCCMMode(ctx, mode);
    return 0;
}

static int sample_get_ccm_mode(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode;
    rk_aiq_uapi2_getCCMMode(ctx, &mode);
    if (mode == OP_MANUAL)
        printf("CCM Mode is: Manual \n");
    else if (mode == OP_AUTO)
        printf("CCM Mode is: AUTO\n");
    else
        printf("CCM Mode is: INVALID\n");
    return 0;
}

static int sample_set_ccm_manual_matrix(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_ccm_matrix_t mccm;
    memset(&mccm, 0, sizeof(rk_aiq_ccm_matrix_t));
    mccm.ccMatrix[0] = 1.5;
    mccm.ccMatrix[1] = 0;
    mccm.ccMatrix[2] = -0.5;
    mccm.ccMatrix[3] = 0;
    mccm.ccMatrix[4] = 1;
    mccm.ccMatrix[5] = 0;
    mccm.ccMatrix[6] = 0;
    mccm.ccMatrix[7] = 0;
    mccm.ccMatrix[8] = 1;
    mccm.ccOffsets[0] = 1000;
    mccm.ccOffsets[1] = 0;
    mccm.ccOffsets[2] = 0;
    rk_aiq_uapi2_setMCcCoef(ctx, &mccm);
    return 0;
}

static int sample_get_ccm_matrix(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_ccm_matrix_t mccm;
    memset(&mccm, 0, sizeof(rk_aiq_ccm_matrix_t));
    rk_aiq_uapi2_getMCcCoef(ctx, &mccm);
    printf("Get CCM Matrix: ");
    for (int j = 0; j < 9; j++)
        printf("%f ", mccm.ccMatrix[j]);
    printf("\n");
    printf("offs: %f, %f, %f \n", mccm.ccOffsets[0], mccm.ccOffsets[1], mccm.ccOffsets[2]);
    return 0;
}

static int sample_get_accm_sat(const rk_aiq_sys_ctx_t* ctx)
{
    float finalsat;
    rk_aiq_uapi2_getACcmSat(ctx, &finalsat);
    printf("Get ACCM Sat-level: %f\n", finalsat);
    return 0;
}

static int sample_get_accm_matrix_name(const rk_aiq_sys_ctx_t* ctx)
{
    char* ccmname[2];
    ccmname[0] = (char*)malloc(sizeof(char)*25);
    ccmname[1] = (char*)malloc(sizeof(char)*25);
    rk_aiq_uapi2_getACcmMatrixName(ctx, ccmname);
    printf("Get ACCM Matrix Name:  %s   %s \n\n", ccmname[0], ccmname[1]);
    return 0;
}

/*
******************************
*
* Module level API Sample Func
*
******************************
*/

static int sample_ccm_setCcmAttr(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setCcmAttr failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_AUTO;
    if (attr.stAuto.color_saturation.level[0] == 100) {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 50;
    } else {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 100;
    }

    //set
    ret = rk_aiq_user_api2_accm_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set CCM Attr failed!");
    printf("set CCM Attr\n\n");

    return 0;
}

static int sample_ccm_getCcmAttr(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get Ccm Attr failed!");
    printf("get CCM Attr:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t enable = %s\n", (attr.byPass ? "false" : "true"));
    if (attr.mode == RK_AIQ_CCM_MODE_MANUAL){
        printf("\t mode = Manual\n");
        printf("\t matrix = [");
        for (int i = 0; i < 9; i++)
            printf("%f ", attr.stManual.ccMatrix[i]);
        printf("\n\t offset = [%f %f %f]\n", attr.stManual.ccOffsets[0],
                                attr.stManual.ccOffsets[1],
                                attr.stManual.ccOffsets[2]);
    } else if (attr.mode == RK_AIQ_CCM_MODE_AUTO){
        printf("\t mode = Auto\n");
        printf("\t color_saturation level = {%f %f %f %f}\n",
                        attr.stAuto.color_saturation.level[0],
                        attr.stAuto.color_saturation.level[1],
                        attr.stAuto.color_saturation.level[2],
                        attr.stAuto.color_saturation.level[3]);
    } else {
        printf("\t mode is invalid!");
    }
    return 0;
}

static int sample_ccm_setModeManual(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setModeManual failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_MANUAL;
    //set
    ret = rk_aiq_user_api2_accm_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Mode Manual failed!");
    printf("set CCM Mode Manual\n\n");

    return 0;
}

static int sample_ccm_setModeAuto(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setModeAuto failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_AUTO;
    //set
    ret = rk_aiq_user_api2_accm_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Mode Auto failed!");
    printf("set CCM Mode Auto\n\n");

    return 0;
}

static int sample_ccm_getMode(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get Ccm Mode failed!");
    printf("get CCM Mode:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    if (attr.mode == RK_AIQ_CCM_MODE_MANUAL){
        printf("\t mode = Manual\n");
    } else if (attr.mode == RK_AIQ_CCM_MODE_AUTO){
        printf("\t mode = Auto\n");
    } else {
        printf("\t mode is invalid!");
    }
    return 0;
}

static int sample_ccm_setMCcm(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setMCcm failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_MANUAL;
    if (attr.stManual.ccOffsets[0] < 1000) {
        attr.stManual.ccOffsets[0] = 1000;
        attr.stManual.ccOffsets[1] = 0;
        attr.stManual.ccOffsets[2] = 0;
    } else {
        attr.stManual.ccOffsets[0] = 0;
        attr.stManual.ccOffsets[1] = 0;
        attr.stManual.ccOffsets[2] = 1000;
    }

    //set
    ret = rk_aiq_user_api2_accm_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Manual attr failed!");
    printf("set Manual attr\n\n");

    return 0;
}

static int sample_ccm_setACcm(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setACcm failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_AUTO;
    if (attr.stAuto.color_saturation.level[0] == 100) {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 50;
    } else {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 100;
    }
    //set
    ret = rk_aiq_user_api2_accm_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Auto attr failed!");
    printf("set Auto attr\n\n");

    return 0;
}

static int sample_query_ccm_info(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_ccm_querry_info_t info;
    rk_aiq_user_api2_accm_QueryCcmInfo(ctx, &info);
        printf("Query CCM Info:\n\n");
    printf("\t enable: %s, \n", (info.ccm_en ? "true" : "false"));
    printf("\t matrix: ");
    for (int j = 0; j < 9; j++)
        printf("%f ", info.ccMatrix[j]);
    printf("\n");
    printf("\t offs: %f, %f, %f \n", info.ccOffsets[0], info.ccOffsets[1], info.ccOffsets[2]);
    printf("\t alp_y: ");
    for (int j = 0; j < 17; j++)
        printf("%f ", info.y_alpha_curve[j]);
    printf("\n");
    printf("\t low bound bit: %f \n", info.low_bound_pos_bit);
    printf("\t color inhibition level: %f, color saturation level: %f \n",
					info.color_inhibition_level, info.color_saturation_level);
    printf("\t finalSat: %f \n", info.finalSat);
    printf("\t ccmname1: %s, ccmname2: %s \n", info.ccmname1, info.ccmname2);
    return 0;
}

// CCM V2
static int sample_ccm_setCcmAttr_V2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setCcmAttr failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_AUTO;
    if (attr.stAuto.color_saturation.level[0] == 100) {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 50;
    } else {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 100;
    }

    //set
    ret = rk_aiq_user_api2_accm_v2_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set CCM Attr failed!");
    printf("set CCM Attr\n\n");

    return 0;
}

static int sample_ccm_getCcmAttr_V2(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get Ccm Attr failed!");
    printf("get CCM Attr:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t enable = %s\n", (attr.byPass ? "false" : "true"));
    if (attr.mode == RK_AIQ_CCM_MODE_MANUAL){
        printf("\t mode = Manual\n");
        printf("\t matrix = [");
        for (int i = 0; i < 9; i++)
            printf("%f ", attr.stManual.ccMatrix[i]);
        printf("\n\t offset = [%f %f %f]\n", attr.stManual.ccOffsets[0],
                                attr.stManual.ccOffsets[1],
                                attr.stManual.ccOffsets[2]);
    } else if (attr.mode == RK_AIQ_CCM_MODE_AUTO){
        printf("\t mode = Auto\n");
        printf("\t color_saturation level = {%f %f %f %f}\n",
                        attr.stAuto.color_saturation.level[0],
                        attr.stAuto.color_saturation.level[1],
                        attr.stAuto.color_saturation.level[2],
                        attr.stAuto.color_saturation.level[3]);
    } else {
        printf("\t mode is invalid!");
    }
    return 0;
}

static int sample_ccm_setModeManual_V2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setModeManual failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_MANUAL;
    //set
    ret = rk_aiq_user_api2_accm_v2_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Mode Manual failed!");
    printf("set CCM Mode Manual\n\n");

    return 0;
}

static int sample_ccm_setModeAuto_V2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setModeAuto failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_AUTO;
    //set
    ret = rk_aiq_user_api2_accm_v2_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Mode Auto failed!");
    printf("set CCM Mode Auto\n\n");

    return 0;
}

static int sample_ccm_getMode_V2(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get Ccm Mode failed!");
    printf("get CCM Mode:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    if (attr.mode == RK_AIQ_CCM_MODE_MANUAL){
        printf("\t mode = Manual\n");
    } else if (attr.mode == RK_AIQ_CCM_MODE_AUTO){
        printf("\t mode = Auto\n");
    } else {
        printf("\t mode is invalid!");
    }
    return 0;
}

static int sample_ccm_setMCcm_V2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setMCcm failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_MANUAL;
    if (attr.stManual.ccOffsets[0] < 1000) {
        attr.stManual.ccOffsets[0] = 1000;
        attr.stManual.ccOffsets[1] = 0;
        attr.stManual.ccOffsets[2] = 0;
    } else {
        attr.stManual.ccOffsets[0] = 0;
        attr.stManual.ccOffsets[1] = 0;
        attr.stManual.ccOffsets[2] = 1000;
    }

    //set
    ret = rk_aiq_user_api2_accm_v2_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Manual attr failed!");
    printf("set Manual attr\n\n");

    return 0;
}

static int sample_ccm_setACcm_V2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setACcm failed in getting accm attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_CCM_MODE_AUTO;
    if (attr.stAuto.color_saturation.level[0] == 100) {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 50;
    } else {
        for (int i = 0; i < 4; i++)
            attr.stAuto.color_saturation.level[i] = 100;
    }
    //set
    ret = rk_aiq_user_api2_accm_v2_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Auto attr failed!");
    printf("set Auto attr\n\n");

    return 0;
}

static int sample_query_ccm_info_V2(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_ccm_querry_info_t info;
    rk_aiq_user_api2_accm_QueryCcmInfo(ctx, &info);
    printf("Query CCM Info:\n\n");
    printf("\t enable: %s, \n", (info.ccm_en ? "true" : "false"));
    printf("\t matrix: ");
    for (int j = 0; j < 9; j++) printf("%f ", info.ccMatrix[j]);
    printf("\n");
    printf("\t offs: %f, %f, %f \n", info.ccOffsets[0], info.ccOffsets[1], info.ccOffsets[2]);
    printf("\t highy adjust enable: %s \n", (info.highy_adj_en ? "true" : "false"));
    printf("\t asym enable: %s \n", (info.asym_enable ? "true" : "false"));
    printf("\t alp_y: ");
    for (int j = 0; j < 17; j++) printf("%f ", info.y_alpha_curve[j]);
    if (info.asym_enable)
        printf("%f\n", info.y_alpha_curve[17]);
    else
        printf("\n");
    printf("\t low bound bit: %f \n", info.low_bound_pos_bit);
    printf("\t color inhibition level: %f, color saturation level: %f \n",
           info.color_inhibition_level, info.color_saturation_level);
    printf("\t finalSat: %f \n", info.finalSat);
    printf("\t ccmname1: %s, ccmname2: %s \n", info.ccmname1, info.ccmname2);
    return 0;
}

static int sample_ccm_getIqParam_V2(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_calib_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_calib_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetIqParam(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "GetIqParam failed!");
    printf("GetIqParam:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t effect aCcmCof_len = %d\n", attr.iqparam.aCcmCof_len);
    for (int i = 0; i < attr.iqparam.aCcmCof_len; i++) {
        printf("\t aCcmCof[%d] = {\n ", i);
        printf("\t     .name       = %s,\n ", attr.iqparam.aCcmCof[i].name);
        printf("\t     .awbgain    = [%f, %f],\n ",
                    attr.iqparam.aCcmCof[i].awbGain[0],
                    attr.iqparam.aCcmCof[i].awbGain[1]);
        printf("\t     .matrixUsed_len = %d,\n ", attr.iqparam.aCcmCof[i].matrixUsed_len);
        printf("\t     .matrixUsed = [");
        for (int j = 0; j < attr.iqparam.aCcmCof[i].matrixUsed_len-1; j++) {
            printf("%s, ", attr.iqparam.aCcmCof[i].matrixUsed[j]);
        }
        printf("%s] \n", attr.iqparam.aCcmCof[i].matrixUsed[attr.iqparam.aCcmCof[i].matrixUsed_len-1]);
        printf("\t     .gainSatCurve = {\n");
        printf("\t         .gains = [");
        for (int j = 0; j < 3; j++) {
            printf("%f, ", attr.iqparam.aCcmCof[i].gain_sat_curve.gains[j]);
        }
        printf("%f], \n", attr.iqparam.aCcmCof[i].gain_sat_curve.gains[3]);
        printf("\t         .sat   = [");
        for (int j = 0; j < 3; j++) {
            printf("%f, ", attr.iqparam.aCcmCof[i].gain_sat_curve.sat[j]);
        }
        printf("%f] \n", attr.iqparam.aCcmCof[i].gain_sat_curve.sat[3]);
        printf("\t     }\n");
        printf("\t }\n");
    }

    printf("\t effect matrixAll_len = %d\n", attr.iqparam.matrixAll_len);
    for (int i = 0; i < attr.iqparam.matrixAll_len; i++) {
        printf("\t matrixAll[%d] = {\n ", i);
        printf("\t     .name       = %s,\n ", attr.iqparam.matrixAll[i].name);
        printf("\t     .illuname   = %s,\n ", attr.iqparam.matrixAll[i].illumination);
        printf("\t     .saturation = %f,\n ", attr.iqparam.matrixAll[i].saturation);
        printf("\t     .ccMatrix   = [%f, ..., %f],\n",
                        attr.iqparam.matrixAll[i].ccMatrix[0],
                        attr.iqparam.matrixAll[i].ccMatrix[8]);
        printf("\t     .ccOffsets  = [%f, %f, %f]\n",
                        attr.iqparam.matrixAll[i].ccOffsets[0],
                        attr.iqparam.matrixAll[i].ccOffsets[1],
                        attr.iqparam.matrixAll[i].ccOffsets[2]);
        printf("\t }\n ");
    }

    return 0;
}

static int sample_ccm_setIqParam_V2_T0(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_calib_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_calib_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetIqParam(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setIqParam failed in GetIqParam!");
    //modify
    attr.sync.sync_mode = sync;

    attr.iqparam.aCcmCof[0].awbGain[0] = 1.3959;
    attr.iqparam.aCcmCof[0].awbGain[1] = 3.179;
    attr.iqparam.matrixAll[0].ccMatrix[0] = 1.7416;
    attr.iqparam.matrixAll[0].ccMatrix[1] = -0.518;
    attr.iqparam.matrixAll[0].ccMatrix[2] = -0.2236;
    attr.iqparam.matrixAll[0].ccMatrix[3] = -0.3781;
    attr.iqparam.matrixAll[0].ccMatrix[4] = 1.3448;
    attr.iqparam.matrixAll[0].ccMatrix[5] = 0.0333;
    attr.iqparam.matrixAll[0].ccMatrix[6] = -0.028;
    attr.iqparam.matrixAll[0].ccMatrix[7] = -1.6919;
    attr.iqparam.matrixAll[0].ccMatrix[8] = 2.7199;

    //set
    ret = rk_aiq_user_api2_accm_v2_SetIqParam(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "IqParam failed!");
    printf("IqParam test 0\n");

    return 0;
}


static int sample_ccm_setIqParam_V2_T1(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_ccm_v2_calib_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_ccm_v2_calib_attrib_t));
    //get
    ret = rk_aiq_user_api2_accm_v2_GetIqParam(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setIqParam failed in GetIqParam!");
    //modify
    attr.sync.sync_mode = sync;

    attr.iqparam.aCcmCof_len = 2;
    for (int i = 1; i < attr.iqparam.aCcmCof_len; i++) {
        attr.iqparam.aCcmCof[i].gain_sat_curve = attr.iqparam.aCcmCof[0].gain_sat_curve;
    }
    sprintf(attr.iqparam.aCcmCof[0].name, "%s", "TL84");
    attr.iqparam.aCcmCof[0].matrixUsed_len = 3;
    sprintf(attr.iqparam.aCcmCof[0].matrixUsed[0], "%s", "TL84_100");
    sprintf(attr.iqparam.aCcmCof[0].matrixUsed[1], "%s", "TL84_70");
    sprintf(attr.iqparam.aCcmCof[0].matrixUsed[2], "%s", "TL84_0");
    attr.iqparam.aCcmCof[0].awbGain[0] = 1.3959;
    attr.iqparam.aCcmCof[0].awbGain[1] = 3.179;
    sprintf(attr.iqparam.aCcmCof[1].name, "%s", "D65");
    attr.iqparam.aCcmCof[1].matrixUsed_len = 2;
    sprintf(attr.iqparam.aCcmCof[1].matrixUsed[0], "%s", "D65_100");
    sprintf(attr.iqparam.aCcmCof[1].matrixUsed[1], "%s", "D65_70");
    attr.iqparam.aCcmCof[1].awbGain[0] = 2.4965;
    attr.iqparam.aCcmCof[1].awbGain[1] = 1.5597;

    attr.iqparam.matrixAll_len = 5;
    int k = 0;
    for (int j = 0; j < attr.iqparam.aCcmCof_len; j++) {
        if (k > (attr.iqparam.matrixAll_len-1)) break;
        strcpy(attr.iqparam.matrixAll[k].illumination, attr.iqparam.aCcmCof[j].name);
        for (int i = 0; i < attr.iqparam.aCcmCof[j].matrixUsed_len; i++) {
            if (k > (attr.iqparam.matrixAll_len-1)) break;
            strcpy(attr.iqparam.matrixAll[k].name, attr.iqparam.aCcmCof[j].matrixUsed[i]);
            k++;
        }
    }
    attr.iqparam.matrixAll[0].saturation = 100;
    attr.iqparam.matrixAll[0].ccMatrix[0] = 1.7416;
    attr.iqparam.matrixAll[0].ccMatrix[1] = -0.518;
    attr.iqparam.matrixAll[0].ccMatrix[2] = -0.2236;
    attr.iqparam.matrixAll[0].ccMatrix[3] = -0.3781;
    attr.iqparam.matrixAll[0].ccMatrix[4] = 1.3448;
    attr.iqparam.matrixAll[0].ccMatrix[5] = 0.0333;
    attr.iqparam.matrixAll[0].ccMatrix[6] = -0.028;
    attr.iqparam.matrixAll[0].ccMatrix[7] = -1.6919;
    attr.iqparam.matrixAll[0].ccMatrix[8] = 2.7199;

    attr.iqparam.matrixAll[1].saturation = 70;
    attr.iqparam.matrixAll[1].ccMatrix[0] = 1.2241;
    attr.iqparam.matrixAll[1].ccMatrix[1] = -0.2362;
    attr.iqparam.matrixAll[1].ccMatrix[2] = 0.0121;
    attr.iqparam.matrixAll[1].ccMatrix[3] = -0.2307;
    attr.iqparam.matrixAll[1].ccMatrix[4] = 1.1783;
    attr.iqparam.matrixAll[1].ccMatrix[5] = 0.0524;
    attr.iqparam.matrixAll[1].ccMatrix[6] = 0.0405;
    attr.iqparam.matrixAll[1].ccMatrix[7] = -1.3682;
    attr.iqparam.matrixAll[1].ccMatrix[8] = 2.3277;

    attr.iqparam.matrixAll[2].saturation = 0;
    attr.iqparam.matrixAll[2].ccMatrix[0] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[1] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[2] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[3] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[4] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[5] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[6] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[7] = 0;
    attr.iqparam.matrixAll[2].ccMatrix[8] = 0;

    attr.iqparam.matrixAll[3].saturation = 100;
    attr.iqparam.matrixAll[3].ccMatrix[0] = 1.5676;
    attr.iqparam.matrixAll[3].ccMatrix[1] = -0.6055;
    attr.iqparam.matrixAll[3].ccMatrix[2] = 0.0378;
    attr.iqparam.matrixAll[3].ccMatrix[3] = -0.2491;
    attr.iqparam.matrixAll[3].ccMatrix[4] = 1.5287;
    attr.iqparam.matrixAll[3].ccMatrix[5] = -0.2796;
    attr.iqparam.matrixAll[3].ccMatrix[6] = 0.0423;
    attr.iqparam.matrixAll[3].ccMatrix[7] = -0.7914;
    attr.iqparam.matrixAll[3].ccMatrix[8] = 1.7491;

    attr.iqparam.matrixAll[4].saturation = 70;
    attr.iqparam.matrixAll[4].ccMatrix[0] = 1.3244;
    attr.iqparam.matrixAll[4].ccMatrix[1] = -0.3646;
    attr.iqparam.matrixAll[4].ccMatrix[2] = 0.0402;
    attr.iqparam.matrixAll[4].ccMatrix[3] = -0.1083;
    attr.iqparam.matrixAll[4].ccMatrix[4] = 1.3275;
    attr.iqparam.matrixAll[4].ccMatrix[5] = -0.2193;
    attr.iqparam.matrixAll[4].ccMatrix[6] = 0.1245;
    attr.iqparam.matrixAll[4].ccMatrix[7] = -0.4216;
    attr.iqparam.matrixAll[4].ccMatrix[8] = 1.2972;
    //set
    ret = rk_aiq_user_api2_accm_v2_SetIqParam(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "IqParam failed!");
    printf("IqParam test 1\n\n");

    return 0;
}

XCamReturn sample_accm_module(const void *arg)
{
    int key = -1;
    CLEAR();
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup){
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be null?*/
    if (ctx == NULL) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_accm_usage ();

    do {

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_accm_usage ();
                break;
            case '0':
                sample_set_ccm_manual(ctx);
                printf("Set CCM MANUAL Mode\n\n");
                break;
            case '1':
                sample_set_ccm_auto(ctx);
                printf("Set CCM AUTO Mode\n\n");
                break;
            case '2':
                sample_get_ccm_mode(ctx);
                break;
            case '3':
                sample_set_ccm_manual_matrix(ctx);
                printf("Set CCM Manual Matrix\n\n");
                break;
            case '4':
                sample_get_ccm_matrix(ctx);
                break;
            case '5':
                sample_get_accm_sat(ctx);
                break;
            case '6':
                sample_get_accm_matrix_name(ctx);
                break;
            case 'a':
                sample_ccm_setCcmAttr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getCcmAttr(ctx);
                break;
            case 'b':
                sample_ccm_setCcmAttr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getCcmAttr(ctx);
                usleep(40*1000);
                sample_ccm_getCcmAttr(ctx);
                break;
            case 'c':
                sample_ccm_setModeManual(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getMode(ctx);
                break;
            case 'd':
                sample_ccm_setModeManual(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getMode(ctx);
                usleep(40*1000);
                sample_ccm_getMode(ctx);
                break;
            case 'e':
                sample_ccm_setModeAuto(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getMode(ctx);
                break;
            case 'f':
                sample_ccm_setModeAuto(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getMode(ctx);
                usleep(40*1000);
                sample_ccm_getMode(ctx);
                break;
            case 'g':
                sample_ccm_setMCcm(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getCcmAttr(ctx);
                break;
            case 'i':
                sample_ccm_setMCcm(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getCcmAttr(ctx);
                usleep(40*1000);
                sample_ccm_getCcmAttr(ctx);
                break;
            case 'j':
                sample_ccm_setACcm(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getCcmAttr(ctx);
                break;
            case 'k':
                sample_ccm_setACcm(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getCcmAttr(ctx);
                usleep(40*1000);
                sample_ccm_getCcmAttr(ctx);
                break;
            case 'l':
                sample_query_ccm_info(ctx);
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn sample_accm_v2_module(const void *arg)
{
    int key = -1;
    CLEAR();
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup){
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be null?*/
    if (ctx == NULL) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_accm_usage ();

    do {

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_accm_usage ();
                break;
            case '0':
                sample_set_ccm_manual(ctx);
                printf("Set CCM MANUAL Mode\n\n");
                break;
            case '1':
                sample_set_ccm_auto(ctx);
                printf("Set CCM AUTO Mode\n\n");
                break;
            case '2':
                sample_get_ccm_mode(ctx);
                break;
            case '3':
                sample_set_ccm_manual_matrix(ctx);
                printf("Set CCM Manual Matrix\n\n");
                break;
            case '4':
                sample_get_ccm_matrix(ctx);
                break;
            case '5':
                sample_get_accm_sat(ctx);
                break;
            case '6':
                sample_get_accm_matrix_name(ctx);
                break;
            case 'a':
                sample_ccm_setCcmAttr_V2(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getCcmAttr_V2(ctx);
                break;
            case 'b':
                sample_ccm_setCcmAttr_V2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getCcmAttr_V2(ctx);
                usleep(40*1000);
                sample_ccm_getCcmAttr_V2(ctx);
                break;
            case 'c':
                sample_ccm_setModeManual_V2(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getMode_V2(ctx);
                break;
            case 'd':
                sample_ccm_setModeManual_V2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getMode_V2(ctx);
                usleep(40*1000);
                sample_ccm_getMode_V2(ctx);
                break;
            case 'e':
                sample_ccm_setModeAuto_V2(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getMode_V2(ctx);
                break;
            case 'f':
                sample_ccm_setModeAuto_V2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getMode_V2(ctx);
                usleep(40*1000);
                sample_ccm_getMode_V2(ctx);
                break;
            case 'g':
                sample_ccm_setMCcm_V2(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getCcmAttr_V2(ctx);
                break;
            case 'i':
                sample_ccm_setMCcm_V2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getCcmAttr_V2(ctx);
                usleep(40*1000);
                sample_ccm_getCcmAttr_V2(ctx);
                break;
            case 'j':
                sample_ccm_setACcm_V2(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_ccm_getCcmAttr_V2(ctx);
                break;
            case 'k':
                sample_ccm_setACcm_V2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_ccm_getCcmAttr_V2(ctx);
                usleep(40*1000);
                sample_ccm_getCcmAttr_V2(ctx);
                break;
            case 'l':
                sample_query_ccm_info_V2(ctx);
                break;
            case 'm':
                sample_ccm_getIqParam_V2(ctx);
                sample_ccm_setIqParam_V2_T0(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                usleep(40*1000);
                sample_ccm_getIqParam_V2(ctx);
                break;
            case 'n':
                sample_ccm_getIqParam_V2(ctx);
                sample_ccm_setIqParam_V2_T1(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                usleep(40*1000);
                sample_ccm_getIqParam_V2(ctx);
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
