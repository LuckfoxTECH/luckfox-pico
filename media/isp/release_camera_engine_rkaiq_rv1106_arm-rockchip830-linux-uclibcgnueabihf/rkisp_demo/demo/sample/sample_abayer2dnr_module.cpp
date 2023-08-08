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

static void sample_abayer2dnr_usage()
{
    printf("Usage : \n");
    printf("\t 0) ABAYER2DNR:         get abayer2dnr attri on sync mode.\n");
    printf("\t 1) ABAYER2DNR:         get abayer2dnr strength on sync mode.\n");
    printf("\t 2) ABAYER2DNR:         set abayer2dnr attri auto on sync mode.\n");
    printf("\t 3) ABAYER2DNR:         set abayer2dnr attri manual on sync mode.\n");
    printf("\t 4) ABAYER2DNR:         set abayer2dnr attri reg value on sync mode.\n");
    printf("\t 5) ABAYER2DNR:         set abayer2dnr strength max value 1.0 on sync mode, only on auto mode has effect.\n");
    printf("\t 6) ABAYER2DNR:         set abayer2dnr strength min value 0.0 on sync mode, only on auto mode has effect.\n");
    printf("\t 7) ABAYER2DNR:         set abayer2dnr strength med value 0.5 on sync mode, only on auto mode has effect.\n");
    printf("\t 8) ABAYER2DNR:         set abayer2dnr attri default vaule on sync mode.\n");
    printf("\t a) ABAYER2DNR:         get abayer2dnr attri on async mode.\n");
    printf("\t b) ABAYER2DNR:         get abayer2dnr strength on async mode.\n");
    printf("\t c) ABAYER2DNR:         set abayer2dnr attri auto on async mode.\n");
    printf("\t d) ABAYER2DNR:         set abayer2dnr attri manual on async mode.\n");
    printf("\t e) ABAYER2DNR:         set abayer2dnr attri reg value on async mode.\n");
    printf("\t f) ABAYER2DNR:         set abayer2dnr strength max value 1.0 on async mode, only on auto mode has effect.\n");
    printf("\t g) ABAYER2DNR:         set abayer2dnr strength min value 0.0 on async mode, only on auto mode has effect.\n");
    printf("\t h) ABAYER2DNR:         set abayer2dnr strength med value 0.5 on async mode, only on auto mode has effect.\n");
    printf("\t i) ABAYER2DNR:         set abayer2dnr attri default vaule on async mode.\n");
    printf("\t q) ABAYER2DNR:         press key q or Q to quit.\n");
}

void sample_print_abayer2dnr_info(const void *arg)
{
    printf ("enter ABAYER2DNR modult test!\n");
}

XCamReturn sample_abayer2dnr_getAttri_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v2_t bayer2dnrV2_attr;
    bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &bayer2dnrV2_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, bayer2dnrV2_attr.sync.done);
    return ret;
}

XCamReturn sample_abayer2dnr_getAttri_v23 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v23_t bayer2dnrV23_attr;
    bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &bayer2dnrV23_attr);
    printf("get abayer2dnr v23 attri ret:%d done:%d \n\n", ret, bayer2dnrV23_attr.sync.done);

    return ret;
}


XCamReturn sample_abayer2dnr_getStrength_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_strength_v2_t bayer2dnrV2Strenght;
    bayer2dnrV2Strenght.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetStrength(ctx, &bayer2dnrV2Strenght);
    printf("get abayer2dnr v2 attri ret:%d strength:%f done:%d\n\n",
           ret, bayer2dnrV2Strenght.percent, bayer2dnrV2Strenght.sync.done);


    return ret;
}

XCamReturn sample_abayer2dnr_getStrength_v23 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_strength_v23_t bayer2dnrV23Strenght;
    bayer2dnrV23Strenght.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetStrength(ctx, &bayer2dnrV23Strenght);
    printf("get abayer2dnr v23 attri ret:%d strength:%f done:%d\n\n",
           ret, bayer2dnrV23Strenght.percent, bayer2dnrV23Strenght.sync.done);

    return ret;
}


XCamReturn sample_abayer2dnr_setDefault_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_bayer2dnr_attrib_v2_t &default_bayer2dnrV2_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    default_bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_SetAttrib(ctx, &default_bayer2dnrV2_attr);
    printf("set bayer2dnr v2 default auto attri ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v2_t get_bayer2dnrV2_attr;
    get_bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &get_bayer2dnrV2_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV2_attr.sync.done);

    return ret;
}

XCamReturn sample_abayer2dnr_setDefault_v23 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_bayer2dnr_attrib_v23_t &default_bayer2dnrV23_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    default_bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_SetAttrib(ctx, &default_bayer2dnrV23_attr);
    printf("set bayer2dnr v2 default auto attri ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v23_t get_bayer2dnrV23_attr;
    get_bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &get_bayer2dnrV23_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV23_attr.sync.done);

    return ret;
}


XCamReturn sample_abayer2dnr_setAuto_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v2_t bayer2dnrV2_attr;
    bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &bayer2dnrV2_attr);
    bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    bayer2dnrV2_attr.eMode = ABAYER2DNR_OP_MODE_AUTO;
    bayer2dnrV2_attr.stAuto.st2DParams.enable = 1;
    for(int i = 0; i < RK_BAYER2DNR_V2_MAX_ISO_NUM; i++) {
        bayer2dnrV2_attr.stAuto.st2DParams.iso[i] = 50 * pow(2, i);

        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[0] = 512;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[1] = 1024;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[2] = 1536;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[3] = 2048;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[4] = 3072;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[5] = 4096;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[6] = 5120;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[7] = 6144;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[8] = 7168;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[9] = 8192;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[10] = 9216;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[11] = 10240;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[12] = 11264;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[13] = 12288;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[14] = 13312;
        bayer2dnrV2_attr.stAuto.st2DParams.lumapoint[15] = 14336;

        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][0] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][1] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][2] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][3] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][4] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][5] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][6] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][7] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][8] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][9] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][10] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][11] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][12] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][13] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][14] = 90;
        bayer2dnrV2_attr.stAuto.st2DParams.sigma[i][15] = 90;

        bayer2dnrV2_attr.stAuto.st2DParams.filter_strength[i] = 1.0;
        bayer2dnrV2_attr.stAuto.st2DParams.edgesofts[i] = 1.0;
        bayer2dnrV2_attr.stAuto.st2DParams.ratio[i] = 0.01;
        bayer2dnrV2_attr.stAuto.st2DParams.weight[i] = 0.7;
        bayer2dnrV2_attr.stAuto.st2DParams.gauss_guide[i] = 0;
        bayer2dnrV2_attr.stAuto.st2DParams.pix_diff[i] = 16383;
        bayer2dnrV2_attr.stAuto.st2DParams.diff_thld[i] = 1024;

    }

    ret = rk_aiq_user_api2_abayer2dnrV2_SetAttrib(ctx, &bayer2dnrV2_attr);
    printf("set abayer2dnr attri auto ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v2_t get_bayer2dnrV2_attr;
    get_bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &get_bayer2dnrV2_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV2_attr.sync.done);

    return ret;
}

XCamReturn sample_abayer2dnr_setAuto_v23 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v23_t bayer2dnrV23_attr;
    bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &bayer2dnrV23_attr);
    bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    bayer2dnrV23_attr.eMode = ABAYER2DNR_V23_OP_MODE_AUTO;
    bayer2dnrV23_attr.stAuto.st2DParams.enable = 1;
    bayer2dnrV23_attr.stAuto.st2DParams.hdrdgain_ctrl_en = 1;

    for(int i = 0; i < RK_BAYER2DNR_V23_MAX_ISO_NUM; i++) {
        bayer2dnrV23_attr.stAuto.st2DParams.iso[i] = 50 * pow(2, i);

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[0] = 512;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[1] = 1024;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[2] = 1536;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[3] = 2048;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[4] = 3072;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[5] = 4096;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[6] = 5120;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[7] = 6144;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[8] = 7168;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[9] = 8192;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[10] = 9216;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[11] = 10240;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[12] = 11264;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[13] = 12288;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[14] = 13312;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].lumapoint[15] = 14336;

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[0] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[1] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[2] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[3] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[4] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[5] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[6] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[7] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[8] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[9] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[10] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[11] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[12] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[13] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[14] = 90;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].sigma[15] = 90;

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[0] = 16;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[1] = 32;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[2] = 48;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[3] = 64;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[4] = 80;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[5] = 96;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[6] = 112;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[7] = 128;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[8] = 144;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[9] = 160;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[10] = 176;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[11] = 192;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[12] = 208;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[13] = 224;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[14] = 240;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_lumapoint[15] = 256;

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[0] = 16;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[1] = 32;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[2] = 48;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[3] = 64;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[4] = 80;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[5] = 96;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[6] = 112;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[7] = 128;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[8] = 144;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[9] = 160;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[10] = 176;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[11] = 192;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[12] = 208;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[13] = 224;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[14] = 240;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_adj[15] = 256;

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].filter_strength = 1.0;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].edgesofts = 1.0;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].ratio = 0.01;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].weight = 0.7;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gauss_guide = 0;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].pix_diff = 16383;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].diff_thld = 1024;

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_bypass = 0;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].gain_scale = 1;

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].trans_mode = 0;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].trans_offset = 256;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].itrans_offset = 32768;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].trans_datmax = 1048575;

        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].hdr_dgain_scale_s = 1;
        bayer2dnrV23_attr.stAuto.st2DParams.Bayer2dnrParamsISO[i].hdr_dgain_scale_m = 1;

    }

    ret = rk_aiq_user_api2_abayer2dnrV23_SetAttrib(ctx, &bayer2dnrV23_attr);
    printf("set abayer2dnr attri auto ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v23_t get_bayer2dnrV23_attr;
    get_bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &get_bayer2dnrV23_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV23_attr.sync.done);

    return ret;
}


XCamReturn sample_abayer2dnr_setManual_v2 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v2_t bayer2dnrV2_attr;
    bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &bayer2dnrV2_attr);
    bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    bayer2dnrV2_attr.eMode = ABAYER2DNR_OP_MODE_MANUAL;
    bayer2dnrV2_attr.stManual.st2DSelect.enable = 1;

    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[0] = 512;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[1] = 1024;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[2] = 1536;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[3] = 2048;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[4] = 3072;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[5] = 4096;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[6] = 5120;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[7] = 6144;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[8] = 7168;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[9] = 8192;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[10] = 9216;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[11] = 10240;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[12] = 11264;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[13] = 12288;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[14] = 13312;
    bayer2dnrV2_attr.stManual.st2DSelect.lumapoint[15] = 14336;

    bayer2dnrV2_attr.stManual.st2DSelect.sigma[0] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[1] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[2] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[3] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[4] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[5] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[6] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[7] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[8] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[9] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[10] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[11] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[12] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[13] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[14] = 90;
    bayer2dnrV2_attr.stManual.st2DSelect.sigma[15] = 90;

    bayer2dnrV2_attr.stManual.st2DSelect.filter_strength = 1.0;
    bayer2dnrV2_attr.stManual.st2DSelect.edgesofts = 1.0;
    bayer2dnrV2_attr.stManual.st2DSelect.ratio = 0.01;
    bayer2dnrV2_attr.stManual.st2DSelect.weight = 0.7;
    bayer2dnrV2_attr.stManual.st2DSelect.gauss_guide = 0;
    bayer2dnrV2_attr.stManual.st2DSelect.pix_diff = 16383;
    bayer2dnrV2_attr.stManual.st2DSelect.diff_thld = 1023;

    ret = rk_aiq_user_api2_abayer2dnrV2_SetAttrib(ctx, &bayer2dnrV2_attr);
    printf("set bayer2dnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v2_t get_bayer2dnrV2_attr;
    get_bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &get_bayer2dnrV2_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV2_attr.sync.done);

    return ret;

}

XCamReturn sample_abayer2dnr_setManual_v23 (const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v23_t bayer2dnrV23_attr;
    bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &bayer2dnrV23_attr);
    bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    bayer2dnrV23_attr.eMode = ABAYER2DNR_V23_OP_MODE_MANUAL;
    bayer2dnrV23_attr.stManual.st2DSelect.enable = 1;
    bayer2dnrV23_attr.stManual.st2DSelect.hdrdgain_ctrl_en = 1;

    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[0] = 512;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[1] = 1024;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[2] = 1536;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[3] = 2048;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[4] = 3072;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[5] = 4096;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[6] = 5120;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[7] = 6144;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[8] = 7168;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[9] = 8192;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[10] = 9216;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[11] = 10240;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[12] = 11264;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[13] = 12288;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[14] = 13312;
    bayer2dnrV23_attr.stManual.st2DSelect.lumapoint[15] = 14336;

    bayer2dnrV23_attr.stManual.st2DSelect.sigma[0] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[1] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[2] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[3] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[4] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[5] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[6] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[7] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[8] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[9] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[10] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[11] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[12] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[13] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[14] = 90;
    bayer2dnrV23_attr.stManual.st2DSelect.sigma[15] = 90;

    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[0] = 16;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[1] = 32;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[2] = 48;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[3] = 64;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[4] = 80;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[5] = 96;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[6] = 112;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[7] = 128;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[8] = 144;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[9] = 160;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[10] = 176;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[11] = 192;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[12] = 208;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[13] = 224;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[14] = 240;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_lumapoint[15] = 256;

    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[0] = 16;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[1] = 32;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[2] = 48;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[3] = 64;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[4] = 80;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[5] = 96;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[6] = 112;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[7] = 128;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[8] = 144;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[9] = 160;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[10] = 176;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[11] = 192;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[12] = 208;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[13] = 224;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[14] = 240;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_adj[15] = 256;

    bayer2dnrV23_attr.stManual.st2DSelect.filter_strength = 1.0;
    bayer2dnrV23_attr.stManual.st2DSelect.edgesofts = 1.0;
    bayer2dnrV23_attr.stManual.st2DSelect.ratio = 0.01;
    bayer2dnrV23_attr.stManual.st2DSelect.weight = 0.7;
    bayer2dnrV23_attr.stManual.st2DSelect.gauss_guide = 0;
    bayer2dnrV23_attr.stManual.st2DSelect.pix_diff = 16383;
    bayer2dnrV23_attr.stManual.st2DSelect.diff_thld = 1024;

    bayer2dnrV23_attr.stManual.st2DSelect.gain_bypass = 0;
    bayer2dnrV23_attr.stManual.st2DSelect.gain_scale = 1;

    bayer2dnrV23_attr.stManual.st2DSelect.trans_mode = 0;
    bayer2dnrV23_attr.stManual.st2DSelect.trans_offset = 256;
    bayer2dnrV23_attr.stManual.st2DSelect.itrans_offset = 32768;
    bayer2dnrV23_attr.stManual.st2DSelect.trans_datmax = 1048575;

    bayer2dnrV23_attr.stManual.st2DSelect.hdr_dgain_scale_s = 1;
    bayer2dnrV23_attr.stManual.st2DSelect.hdr_dgain_scale_m = 1;


    ret = rk_aiq_user_api2_abayer2dnrV23_SetAttrib(ctx, &bayer2dnrV23_attr);
    printf("set bayer2dnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v23_t get_bayer2dnrV23_attr;
    get_bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &get_bayer2dnrV23_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV23_attr.sync.done);

    return ret;
}

XCamReturn sample_abayer2dnr_setReg_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v2_t bayer2dnrV2_attr;
    bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &bayer2dnrV2_attr);
    bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    bayer2dnrV2_attr.eMode = ABAYER2DNR_OP_MODE_REG_MANUAL;

    //ISP_BAYNR_3A00_CTRL
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_lg2_mode = 0;
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_gauss_en = 1;
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_log_bypass = 0;
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_en = 1;

    // ISP_BAYNR_3A00_DGAIN0-2
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_dgain[0] = 0x4000;
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_dgain[1] = 0x4000;
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_dgain[2] = 0x4000;

    // ISP_BAYNR_3A00_PIXDIFF
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_pix_diff = 0x3fff;

    // ISP_BAYNR_3A00_THLD
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_diff_thld = 0x3fff;
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_softthld = 0x00a;

    // ISP_BAYNR_3A00_W1_STRENG
    bayer2dnrV2_attr.stManual.st2Dfix.bltflt_streng = 0x0a3;
    bayer2dnrV2_attr.stManual.st2Dfix.baynr_reg_w1 = 0x3ff;

    // ISP_BAYNR_3A00_SIGMAX0-15
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[0] = 0x1010;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[1] = 0x1210;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[2] = 0x1410;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[3] = 0x1610;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[4] = 0x1810;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[5] = 0x1c10;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[6] = 0x2010;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[7] = 0x2410;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[8] = 0x2810;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[9] = 0x2c10;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[10] = 0x3010;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[11] = 0x3410;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[12] = 0x3810;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[13] = 0x3a10;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[14] = 0x3c10;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_x[15] = 0x4010;

    // ISP_BAYNR_3A00_SIGMAY0-15
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[0] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[1] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[2] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[3] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[4] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[5] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[6] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[7] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[8] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[9] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[10] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[11] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[12] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[13] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[14] = 0x0300;
    bayer2dnrV2_attr.stManual.st2Dfix.sigma_y[15] = 0x0300;

    // ISP_BAYNR_3A00_WRIT_D
    bayer2dnrV2_attr.stManual.st2Dfix.weit_d[0] = 0x178;
    bayer2dnrV2_attr.stManual.st2Dfix.weit_d[1] = 0x26d;
    bayer2dnrV2_attr.stManual.st2Dfix.weit_d[2] = 0x31d;


    ret = rk_aiq_user_api2_abayer2dnrV2_SetAttrib(ctx, &bayer2dnrV2_attr);
    printf("set bayer2dnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v2_t get_bayer2dnrV2_attr;
    get_bayer2dnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &get_bayer2dnrV2_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV2_attr.sync.done);

    return ret;
}


XCamReturn sample_abayer2dnr_setReg_v23(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_attrib_v23_t bayer2dnrV23_attr;
    bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &bayer2dnrV23_attr);
    bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    bayer2dnrV23_attr.eMode = ABAYER2DNR_V23_OP_MODE_REG_MANUAL;

    //ISP_BAYNR_3A00_CTRL
    bayer2dnrV23_attr.stManual.st2Dfix.bay3d_gain_en = 1;
    bayer2dnrV23_attr.stManual.st2Dfix.lg2_mode = 0;
    bayer2dnrV23_attr.stManual.st2Dfix.gauss_en = 1;
    bayer2dnrV23_attr.stManual.st2Dfix.log_bypass = 0;
    bayer2dnrV23_attr.stManual.st2Dfix.bayer_en = 1;

    // ISP_BAYNR_3A00_DGAIN0-2
    bayer2dnrV23_attr.stManual.st2Dfix.dgain[0] = 0x4000;
    bayer2dnrV23_attr.stManual.st2Dfix.dgain[1] = 0x4000;
    bayer2dnrV23_attr.stManual.st2Dfix.dgain[2] = 0x4000;

    // ISP_BAYNR_3A00_PIXDIFF
    bayer2dnrV23_attr.stManual.st2Dfix.pix_diff = 0x3fff;

    // ISP_BAYNR_3A00_THLD
    bayer2dnrV23_attr.stManual.st2Dfix.diff_thld = 0x3fff;
    bayer2dnrV23_attr.stManual.st2Dfix.softthld = 0x00a;

    // ISP_BAYNR_3A00_W1_STRENG
    bayer2dnrV23_attr.stManual.st2Dfix.bltflt_streng = 0x0a3;
    bayer2dnrV23_attr.stManual.st2Dfix.reg_w1 = 0x3ff;

    // ISP_BAYNR_3A00_SIGMAX0-15
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[0] = 0x1010;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[1] = 0x1210;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[2] = 0x1410;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[3] = 0x1610;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[4] = 0x1810;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[5] = 0x1c10;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[6] = 0x2010;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[7] = 0x2410;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[8] = 0x2810;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[9] = 0x2c10;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[10] = 0x3010;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[11] = 0x3410;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[12] = 0x3810;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[13] = 0x3a10;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[14] = 0x3c10;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_x[15] = 0x4010;

    // ISP_BAYNR_3A00_SIGMAY0-15
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[0] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[1] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[2] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[3] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[4] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[5] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[6] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[7] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[8] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[9] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[10] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[11] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[12] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[13] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[14] = 0x0300;
    bayer2dnrV23_attr.stManual.st2Dfix.sigma_y[15] = 0x0300;

    // ISP_BAYNR_3A00_WRIT_D
    bayer2dnrV23_attr.stManual.st2Dfix.weit_d[0] = 0x178;
    bayer2dnrV23_attr.stManual.st2Dfix.weit_d[1] = 0x26d;
    bayer2dnrV23_attr.stManual.st2Dfix.weit_d[2] = 0x31d;

    bayer2dnrV23_attr.stManual.st2Dfix.lg2_lgoff = 0x8000;
    bayer2dnrV23_attr.stManual.st2Dfix.lg2_off = 0x100;

    bayer2dnrV23_attr.stManual.st2Dfix.dat_max = 0xfffff;

    bayer2dnrV23_attr.stManual.st2Dfix.rgain_off = 0x00;
    bayer2dnrV23_attr.stManual.st2Dfix.bgain_off = 0x00;

    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[0] = 0x10;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[1] = 0x20;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[2] = 0x30;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[3] = 0x40;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[4] = 0x50;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[5] = 0x60;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[6] = 0x70;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[7] = 0x80;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[8] = 0x90;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[9] = 0xa0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[10] = 0xb0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[11] = 0xc0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[12] = 0xd0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[13] = 0xe0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[14] = 0xf0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_x[15] = 0xff;

    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[0] = 0x10;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[1] = 0x20;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[2] = 0x30;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[3] = 0x40;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[4] = 0x50;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[5] = 0x60;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[6] = 0x70;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[7] = 0x80;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[8] = 0x90;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[9] = 0xa0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[10] = 0xb0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[11] = 0xc0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[12] = 0xd0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[13] = 0x30;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[14] = 0xf0;
    bayer2dnrV23_attr.stManual.st2Dfix.gain_y[15] = 0xff;

    ret = rk_aiq_user_api2_abayer2dnrV23_SetAttrib(ctx, &bayer2dnrV23_attr);
    printf("set bayer2dnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_bayer2dnr_attrib_v23_t get_bayer2dnrV23_attr;
    get_bayer2dnrV23_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &get_bayer2dnrV23_attr);
    printf("get abayer2dnr v2 attri ret:%d done:%d \n\n", ret, get_bayer2dnrV23_attr.sync.done);

    return ret;
}

XCamReturn sample_abayer2dnr_setStrength_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, float fStrength)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_strength_v2_t bayer2dnrV2Strenght;
    bayer2dnrV2Strenght.sync.sync_mode = sync_mode;
    bayer2dnrV2Strenght.percent = fStrength;
    ret = rk_aiq_user_api2_abayer2dnrV2_SetStrength(ctx, &bayer2dnrV2Strenght);
    printf("Set abayer2dnr v2 set strength ret:%d strength:%f \n\n", ret, bayer2dnrV2Strenght.percent);

    rk_aiq_bayer2dnr_strength_v2_t get_bayer2dnrV2Strenght;
    get_bayer2dnrV2Strenght.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV2_GetStrength(ctx, &get_bayer2dnrV2Strenght);
    printf("get abayer2dnr v2 attri ret:%d strength:%f done:%d\n\n",
           ret, get_bayer2dnrV2Strenght.percent, get_bayer2dnrV2Strenght.sync.done);

    return ret;
}

XCamReturn sample_abayer2dnr_setStrength_v23(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, float fStrength)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_bayer2dnr_strength_v23_t bayer2dnrV23Strenght;
    bayer2dnrV23Strenght.sync.sync_mode = sync_mode;
    bayer2dnrV23Strenght.percent = fStrength;
    bayer2dnrV23Strenght.strength_enable = true;
    ret = rk_aiq_user_api2_abayer2dnrV23_SetStrength(ctx, &bayer2dnrV23Strenght);
    printf("Set abayer2dnr v2 set strength ret:%d strength:%f \n\n", ret, bayer2dnrV23Strenght.percent);

    rk_aiq_bayer2dnr_strength_v23_t get_bayer2dnrV23Strenght;
    get_bayer2dnrV23Strenght.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_abayer2dnrV23_GetStrength(ctx, &get_bayer2dnrV23Strenght);
    printf("get abayer2dnr v2 attri ret:%d strength:%f done:%d\n\n",
           ret, get_bayer2dnrV23Strenght.percent, get_bayer2dnrV23Strenght.sync.done);

    return ret;
}


XCamReturn sample_abayer2dnr_module (const void *arg)
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

    rk_aiq_bayer2dnr_attrib_v2_t default_bayer2dnrV2_attr;
    rk_aiq_bayer2dnr_attrib_v23_t default_bayer2dnrV23_attr;

    if (CHECK_ISP_HW_V30()) {
        ret = rk_aiq_user_api2_abayer2dnrV2_GetAttrib(ctx, &default_bayer2dnrV2_attr);
        printf("get abayer2dnr v2 default auto attri ret:%d \n\n", ret);
    }
    if (CHECK_ISP_HW_V32()) {
        ret = rk_aiq_user_api2_abayer2dnrV23_GetAttrib(ctx, &default_bayer2dnrV23_attr);
        printf("get abayer2dnr v23 default auto attri ret:%d \n\n", ret);
    }

    do {
        sample_abayer2dnr_usage ();

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar ();
        printf ("\n");

        switch (key) {
        case '0':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_getAttri_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_getAttri_v23(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '1':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_getStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_getStrength_v23(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '2':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setAuto_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }

            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setAuto_v23(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '3':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setManual_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }

            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setManual_v23(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '4':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setReg_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setReg_v23(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '5':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, 1.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setStrength_v23(ctx, RK_AIQ_UAPI_MODE_SYNC, 1.0);
            }
            break;
        case '6':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setStrength_v23(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.0);
            }
            break;
        case '7':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.5);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setStrength_v23(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.5);
            }
            break;
        case '8':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setDefault_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, default_bayer2dnrV2_attr);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setDefault_v23(ctx, RK_AIQ_UAPI_MODE_SYNC, default_bayer2dnrV23_attr);
            }
            break;
        case 'a':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_getAttri_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_getAttri_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'b':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_getStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_getStrength_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'c':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setAuto_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }

            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setAuto_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }

            break;
        case 'd':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setManual_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }

            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setManual_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }

            break;

        case 'e':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setReg_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setReg_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'f':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, 1.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setStrength_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC, 1.0);
            }
            break;
        case 'g':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setStrength_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.0);
            }
            break;
        case 'h':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.5);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setStrength_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.5);
            }
            break;
        case 'i':
            if (CHECK_ISP_HW_V30()) {
                sample_abayer2dnr_setDefault_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, default_bayer2dnrV2_attr);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_abayer2dnr_setDefault_v23(ctx, RK_AIQ_UAPI_MODE_ASYNC, default_bayer2dnrV23_attr);
            }
            break;
        default:
            printf("not support test\n\n");
            break;
        }

    } while (key != 'q' && key != 'Q');

    return ret;
}

