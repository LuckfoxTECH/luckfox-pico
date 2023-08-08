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

static void sample_asharp_usage()
{
    printf("Usage : \n");
    printf("\t 0) ASHARP:         get asharp attri on sync mode.\n");
    printf("\t 1) ASHARP:         get asharp strength on sync mode.\n");
    printf("\t 2) ASHARP:         set asharp attri auto on sync mode.\n");
    printf("\t 3) ASHARP:         set asharp attri manual on sync mode.\n");
    printf("\t 4) ASHARP:         set asharp attri reg value on sync mode.\n");
    printf("\t 5) ASHARP:         set asharp strength max value 1.0 on sync mode, only on auto mode has effect.\n");
    printf("\t 6) ASHARP:         set asharp strength min value 0.0 on sync mode, only on auto mode has effect.\n");
    printf("\t 7) ASHARP:         set asharp strength med value 0.5 on sync mode, only on auto mode has effect.\n");
    printf("\t 8) ASHARP:         set asharp attri to default value on sync mode.\n");
    printf("\t a) ASHARP:         get asharp attri on async mode.\n");
    printf("\t b) ASHARP:         get asharp strength on async mode.\n");
    printf("\t c) ASHARP:         set asharp attri auto on async mode.\n");
    printf("\t d) ASHARP:         set asharp attri manual on async mode.\n");
    printf("\t e) ASHARP:         set asharp attri reg value on async mode.\n");
    printf("\t f) ASHARP:         set asharp strength max value 1.0 on async mode, only on auto mode has effect.\n");
    printf("\t g) ASHARP:         set asharp strength min value 0.0 on async mode, only on auto mode has effect.\n");
    printf("\t h) ASHARP:         set asharp strength med value 0.5 on async mode, only on auto mode has effect.\n");
    printf("\t i) ASHARP:         set asharp attri to default value on async mode.\n");
    printf("\t q) ASHARP:         press key q or Q to quit.\n");
}

void sample_print_asharp_info(const void *arg)
{
    printf ("enter ASHARP modult test!\n");
}

XCamReturn sample_sharp_setAuto_v4(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_sharp_attrib_v4_t sharpV4_attr;
    sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &sharpV4_attr);
    sharpV4_attr.sync.sync_mode = sync_mode;
    sharpV4_attr.eMode = ASHARP4_OP_MODE_AUTO;
    sharpV4_attr.stAuto.stParams.enable = 1;
    for(int i = 0; i < RK_SHARP_V4_MAX_ISO_NUM; i++) {
        sharpV4_attr.stAuto.stParams.iso[i] = 50 * pow(2, i);

        sharpV4_attr.stAuto.stParams.luma_point[0] = 0;
        sharpV4_attr.stAuto.stParams.luma_point[1] = 64;
        sharpV4_attr.stAuto.stParams.luma_point[2] = 128;
        sharpV4_attr.stAuto.stParams.luma_point[3] = 256;
        sharpV4_attr.stAuto.stParams.luma_point[4] = 384;
        sharpV4_attr.stAuto.stParams.luma_point[5] = 640;
        sharpV4_attr.stAuto.stParams.luma_point[6] = 896;
        sharpV4_attr.stAuto.stParams.luma_point[7] = 1024;

        sharpV4_attr.stAuto.stParams.luma_sigma[i][0] = 16;
        sharpV4_attr.stAuto.stParams.luma_sigma[i][1] = 16;
        sharpV4_attr.stAuto.stParams.luma_sigma[i][2] = 16;
        sharpV4_attr.stAuto.stParams.luma_sigma[i][3] = 16;
        sharpV4_attr.stAuto.stParams.luma_sigma[i][4] = 16;
        sharpV4_attr.stAuto.stParams.luma_sigma[i][5] = 16;
        sharpV4_attr.stAuto.stParams.luma_sigma[i][6] = 16;
        sharpV4_attr.stAuto.stParams.luma_sigma[i][7] = 16;

        sharpV4_attr.stAuto.stParams.hf_clip[i][0] = 128;
        sharpV4_attr.stAuto.stParams.hf_clip[i][1] = 128;
        sharpV4_attr.stAuto.stParams.hf_clip[i][2] = 128;
        sharpV4_attr.stAuto.stParams.hf_clip[i][3] = 128;
        sharpV4_attr.stAuto.stParams.hf_clip[i][4] = 128;
        sharpV4_attr.stAuto.stParams.hf_clip[i][5] = 128;
        sharpV4_attr.stAuto.stParams.hf_clip[i][6] = 128;
        sharpV4_attr.stAuto.stParams.hf_clip[i][7] = 128;

        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][0] = 256;
        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][1] = 256;
        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][2] = 256;
        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][3] = 256;
        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][4] = 256;
        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][5] = 256;
        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][6] = 256;
        sharpV4_attr.stAuto.stParams.local_sharp_strength[i][7] = 256;


        sharpV4_attr.stAuto.stParams.pbf_gain[i] = 1.0;
        sharpV4_attr.stAuto.stParams.pbf_add[i] = 0.0;
        sharpV4_attr.stAuto.stParams.pbf_ratio[i] = 0.5;
        sharpV4_attr.stAuto.stParams.gaus_ratio[i] = 0.0;
        sharpV4_attr.stAuto.stParams.sharp_ratio[i] = 10.0;
        sharpV4_attr.stAuto.stParams.bf_gain[i] = 1.0;
        sharpV4_attr.stAuto.stParams.bf_add[i] = 0.0;
        sharpV4_attr.stAuto.stParams.bf_ratio[i] = 0.5;

        sharpV4_attr.stAuto.stParams.prefilter_coeff[i][0] = 0.2042;
        sharpV4_attr.stAuto.stParams.prefilter_coeff[i][1] = 0.1238;
        sharpV4_attr.stAuto.stParams.prefilter_coeff[i][2] = 0.0751;

        sharpV4_attr.stAuto.stParams.hfBilateralFilter_coeff[i][0] = 0.2042;
        sharpV4_attr.stAuto.stParams.hfBilateralFilter_coeff[i][1] = 0.1238;
        sharpV4_attr.stAuto.stParams.hfBilateralFilter_coeff[i][2] = 0.0751;


        sharpV4_attr.stAuto.stParams.GaussianFilter_coeff[i][0] = 0.0935;
        sharpV4_attr.stAuto.stParams.GaussianFilter_coeff[i][1] = 0.0724;
        sharpV4_attr.stAuto.stParams.GaussianFilter_coeff[i][2] = 0.0561;
        sharpV4_attr.stAuto.stParams.GaussianFilter_coeff[i][3] = 0.0337;
        sharpV4_attr.stAuto.stParams.GaussianFilter_coeff[i][4] = 0.0261;
        sharpV4_attr.stAuto.stParams.GaussianFilter_coeff[i][5] = 0.0121;

        sharpV4_attr.stAuto.stParams.kernel_sigma_enable = 0;
        sharpV4_attr.stAuto.stParams.prefilter_sigma[i] = 1.0;
        sharpV4_attr.stAuto.stParams.hfBilateralFilter_sigma[i] = 1.0;
        sharpV4_attr.stAuto.stParams.GaussianFilter_sigma[i] = 1.0;
        sharpV4_attr.stAuto.stParams.GaussianFilter_radius[i] = 2.0;

    }

    ret = rk_aiq_user_api2_asharpV4_SetAttrib(ctx, &sharpV4_attr);
    printf("set asharp attri auto ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v4_t get_sharpV4_attr;
    get_sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &get_sharpV4_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV4_attr.sync.done);
    return ret;
}


XCamReturn sample_sharp_setAuto_v33(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_sharp_attrib_v33_t sharpV33_attr;
    sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &sharpV33_attr);
    sharpV33_attr.sync.sync_mode = sync_mode;
    sharpV33_attr.eMode = ASHARP_V33_OP_MODE_AUTO;
    sharpV33_attr.stAuto.stParams.enable = 1;
    sharpV33_attr.stAuto.stParams.kernel_sigma_enable = 1;

    sharpV33_attr.stAuto.stParams.Center_Mode = 0;
    sharpV33_attr.stAuto.stParams.center_x = 0;
    sharpV33_attr.stAuto.stParams.center_y = 0;


    int luma_sigma[RK_SHARP_V33_LUMA_POINT_NUM];
    int luma_point[RK_SHARP_V33_LUMA_POINT_NUM];
    float pbf_gain;
    float pbf_add;
    float pbf_ratio;
    float gaus_ratio;
    float sharp_ratio;
    int lum_clip_h[RK_SHARP_V33_LUMA_POINT_NUM];
    float bf_gain;
    float bf_add;
    float bf_ratio;
    int global_clip_pos;

    float prefilter_coeff[RK_SHARP_V33_PBF_DIAM * RK_SHARP_V33_PBF_DIAM];
    float GaussianFilter_coeff[RK_SHARP_V33_RF_DIAM * RK_SHARP_V33_RF_DIAM];
    float hfBilateralFilter_coeff[RK_SHARP_V33_BF_DIAM * RK_SHARP_V33_BF_DIAM];

    float global_gain;
    float global_gain_alpha;
    float local_gainscale;

    float gain_adj_sharp_strength[RK_SHARP_V33_SHARP_ADJ_GAIN_TABLE_LEN];
    int exgain_bypass;

    float dis_adj_sharp_strength[RK_SHARP_V33_STRENGTH_TABLE_LEN];

    float noiseclip_strength;
    int enhance_bit;
    int noiseclip_mode;
    int noise_sigma_clip;

    float prefilter_sigma;
    float hfBilateralFilter_sigma;
    float GaussianFilter_sigma;
    float GaussianFilter_radius;

    for(int i = 0; i < RK_SHARP_V4_MAX_ISO_NUM; i++) {
        sharpV33_attr.stAuto.stParams.iso[i] = 50 * pow(2, i);

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[0] = 0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[1] = 64;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[2] = 128;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[3] = 256;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[4] = 384;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[5] = 640;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[6] = 896;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_point[7] = 1024;

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[0] = 16;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[1] = 16;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[2] = 16;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[3] = 16;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[4] = 16;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[5] = 16;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[6] = 16;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].luma_sigma[7] = 16;


        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].pbf_gain = 1.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].pbf_add = 0.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].pbf_ratio = 0.5;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gaus_ratio = 0.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].sharp_ratio = 10.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].bf_gain = 1.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].bf_add = 0.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].bf_ratio = 0.5;

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].prefilter_coeff[0] = 0.2042;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].prefilter_coeff[1] = 0.1238;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].prefilter_coeff[2] = 0.0751;

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].hfBilateralFilter_coeff[0] = 0.2042;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].hfBilateralFilter_coeff[1] = 0.1238;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].hfBilateralFilter_coeff[2] = 0.0751;


        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_coeff[0] = 0.0935;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_coeff[1] = 0.0724;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_coeff[2] = 0.0561;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_coeff[3] = 0.0337;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_coeff[4] = 0.0261;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_coeff[5] = 0.0121;

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].prefilter_sigma = 1.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].hfBilateralFilter_sigma = 1.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_sigma = 1.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].GaussianFilter_radius = 2.0;

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].global_gain = 1.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].global_gain_alpha = 0.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].local_gainscale = 1.0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].exgain_bypass = 0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].noiseclip_strength = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].enhance_bit = 3;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].noiseclip_mode = 0;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].noise_sigma_clip = 1023;

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[0] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[1] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[2] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[3] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[4] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[5] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[6] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[7] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[8] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[9] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[10] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[11] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[12] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].gain_adj_sharp_strength[13] = 1;

        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[0] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[1] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[2] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[3] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[4] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[5] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[6] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[7] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[8] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[9] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[10] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[11] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[12] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[13] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[14] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[15] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[16] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[17] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[18] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[19] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[20] = 1;
        sharpV33_attr.stAuto.stParams.sharpParamsISO[i].dis_adj_sharp_strength[21] = 1;

    }

    ret = rk_aiq_user_api2_asharpV33_SetAttrib(ctx, &sharpV33_attr);
    printf("set asharp attri auto ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v33_t get_sharpV33_attr;
    get_sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &get_sharpV33_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV33_attr.sync.done);
    return ret;
}

XCamReturn sample_sharp_setManual_v4(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_attrib_v4_t sharpV4_attr;
    sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &sharpV4_attr);
    sharpV4_attr.sync.sync_mode = sync_mode;
    sharpV4_attr.eMode = ASHARP4_OP_MODE_MANUAL;
    sharpV4_attr.stManual.stSelect.enable = 1;

    sharpV4_attr.stManual.stSelect.luma_point[0] = 0;
    sharpV4_attr.stManual.stSelect.luma_point[1] = 64;
    sharpV4_attr.stManual.stSelect.luma_point[2] = 128;
    sharpV4_attr.stManual.stSelect.luma_point[3] = 256;
    sharpV4_attr.stManual.stSelect.luma_point[4] = 384;
    sharpV4_attr.stManual.stSelect.luma_point[5] = 640;
    sharpV4_attr.stManual.stSelect.luma_point[6] = 896;
    sharpV4_attr.stManual.stSelect.luma_point[7] = 1024;

    sharpV4_attr.stManual.stSelect.luma_sigma[0] = 16;
    sharpV4_attr.stManual.stSelect.luma_sigma[1] = 16;
    sharpV4_attr.stManual.stSelect.luma_sigma[2] = 16;
    sharpV4_attr.stManual.stSelect.luma_sigma[3] = 16;
    sharpV4_attr.stManual.stSelect.luma_sigma[4] = 16;
    sharpV4_attr.stManual.stSelect.luma_sigma[5] = 16;
    sharpV4_attr.stManual.stSelect.luma_sigma[6] = 16;
    sharpV4_attr.stManual.stSelect.luma_sigma[7] = 16;

    sharpV4_attr.stManual.stSelect.hf_clip[0] = 128;
    sharpV4_attr.stManual.stSelect.hf_clip[1] = 128;
    sharpV4_attr.stManual.stSelect.hf_clip[2] = 128;
    sharpV4_attr.stManual.stSelect.hf_clip[3] = 128;
    sharpV4_attr.stManual.stSelect.hf_clip[4] = 128;
    sharpV4_attr.stManual.stSelect.hf_clip[5] = 128;
    sharpV4_attr.stManual.stSelect.hf_clip[6] = 128;
    sharpV4_attr.stManual.stSelect.hf_clip[7] = 128;

    sharpV4_attr.stManual.stSelect.local_sharp_strength[0] = 256;
    sharpV4_attr.stManual.stSelect.local_sharp_strength[1] = 256;
    sharpV4_attr.stManual.stSelect.local_sharp_strength[2] = 256;
    sharpV4_attr.stManual.stSelect.local_sharp_strength[3] = 256;
    sharpV4_attr.stManual.stSelect.local_sharp_strength[4] = 256;
    sharpV4_attr.stManual.stSelect.local_sharp_strength[5] = 256;
    sharpV4_attr.stManual.stSelect.local_sharp_strength[6] = 256;
    sharpV4_attr.stManual.stSelect.local_sharp_strength[7] = 256;


    sharpV4_attr.stManual.stSelect.pbf_gain = 1.0;
    sharpV4_attr.stManual.stSelect.pbf_add = 0.0;
    sharpV4_attr.stManual.stSelect.pbf_ratio = 0.5;
    sharpV4_attr.stManual.stSelect.gaus_ratio = 0.0;
    sharpV4_attr.stManual.stSelect.sharp_ratio = 10.0;
    sharpV4_attr.stManual.stSelect.bf_gain = 1.0;
    sharpV4_attr.stManual.stSelect.bf_add = 0.0;
    sharpV4_attr.stManual.stSelect.bf_ratio = 0.5;

    sharpV4_attr.stManual.stSelect.prefilter_coeff[0] = 0.2042;
    sharpV4_attr.stManual.stSelect.prefilter_coeff[1] = 0.1238;
    sharpV4_attr.stManual.stSelect.prefilter_coeff[2] = 0.0751;

    sharpV4_attr.stManual.stSelect.hfBilateralFilter_coeff[0] = 0.2042;
    sharpV4_attr.stManual.stSelect.hfBilateralFilter_coeff[1] = 0.1238;
    sharpV4_attr.stManual.stSelect.hfBilateralFilter_coeff[2] = 0.0751;


    sharpV4_attr.stManual.stSelect.GaussianFilter_coeff[0] = 0.0935;
    sharpV4_attr.stManual.stSelect.GaussianFilter_coeff[1] = 0.0724;
    sharpV4_attr.stManual.stSelect.GaussianFilter_coeff[2] = 0.0561;
    sharpV4_attr.stManual.stSelect.GaussianFilter_coeff[3] = 0.0337;
    sharpV4_attr.stManual.stSelect.GaussianFilter_coeff[4] = 0.0261;
    sharpV4_attr.stManual.stSelect.GaussianFilter_coeff[5] = 0.0121;

    sharpV4_attr.stManual.stSelect.kernel_sigma_enable = 0;
    sharpV4_attr.stManual.stSelect.prefilter_sigma = 1.0;
    sharpV4_attr.stManual.stSelect.hfBilateralFilter_sigma = 1.0;
    sharpV4_attr.stManual.stSelect.GaussianFilter_sigma = 1.0;
    sharpV4_attr.stManual.stSelect.GaussianFilter_radius = 2.0;

    ret = rk_aiq_user_api2_asharpV4_SetAttrib(ctx, &sharpV4_attr);
    printf("set sharp v4 attri manual ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v4_t get_sharpV4_attr;
    get_sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &get_sharpV4_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV4_attr.sync.done);

    return ret;
}


XCamReturn sample_sharp_setManual_v33(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_attrib_v33_t sharpV33_attr;
    sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &sharpV33_attr);
    sharpV33_attr.sync.sync_mode = sync_mode;
    sharpV33_attr.eMode = ASHARP_V33_OP_MODE_MANUAL;
    sharpV33_attr.stManual.stSelect.enable = 1;
    sharpV33_attr.stManual.stSelect.kernel_sigma_enable = 0;

    sharpV33_attr.stManual.stSelect.luma_point[0] = 0;
    sharpV33_attr.stManual.stSelect.luma_point[1] = 64;
    sharpV33_attr.stManual.stSelect.luma_point[2] = 128;
    sharpV33_attr.stManual.stSelect.luma_point[3] = 256;
    sharpV33_attr.stManual.stSelect.luma_point[4] = 384;
    sharpV33_attr.stManual.stSelect.luma_point[5] = 640;
    sharpV33_attr.stManual.stSelect.luma_point[6] = 896;
    sharpV33_attr.stManual.stSelect.luma_point[7] = 1024;

    sharpV33_attr.stManual.stSelect.luma_sigma[0] = 16;
    sharpV33_attr.stManual.stSelect.luma_sigma[1] = 16;
    sharpV33_attr.stManual.stSelect.luma_sigma[2] = 16;
    sharpV33_attr.stManual.stSelect.luma_sigma[3] = 16;
    sharpV33_attr.stManual.stSelect.luma_sigma[4] = 16;
    sharpV33_attr.stManual.stSelect.luma_sigma[5] = 16;
    sharpV33_attr.stManual.stSelect.luma_sigma[6] = 16;
    sharpV33_attr.stManual.stSelect.luma_sigma[7] = 16;


    sharpV33_attr.stManual.stSelect.pbf_gain = 1.0;
    sharpV33_attr.stManual.stSelect.pbf_add = 0.0;
    sharpV33_attr.stManual.stSelect.pbf_ratio = 0.5;
    sharpV33_attr.stManual.stSelect.gaus_ratio = 0.0;
    sharpV33_attr.stManual.stSelect.sharp_ratio = 10.0;
    sharpV33_attr.stManual.stSelect.bf_gain = 1.0;
    sharpV33_attr.stManual.stSelect.bf_add = 0.0;
    sharpV33_attr.stManual.stSelect.bf_ratio = 0.5;

    sharpV33_attr.stManual.stSelect.prefilter_coeff[0] = 0.2042;
    sharpV33_attr.stManual.stSelect.prefilter_coeff[1] = 0.1238;
    sharpV33_attr.stManual.stSelect.prefilter_coeff[2] = 0.0751;

    sharpV33_attr.stManual.stSelect.hfBilateralFilter_coeff[0] = 0.2042;
    sharpV33_attr.stManual.stSelect.hfBilateralFilter_coeff[1] = 0.1238;
    sharpV33_attr.stManual.stSelect.hfBilateralFilter_coeff[2] = 0.0751;


    sharpV33_attr.stManual.stSelect.GaussianFilter_coeff[0] = 0.0935;
    sharpV33_attr.stManual.stSelect.GaussianFilter_coeff[1] = 0.0724;
    sharpV33_attr.stManual.stSelect.GaussianFilter_coeff[2] = 0.0561;
    sharpV33_attr.stManual.stSelect.GaussianFilter_coeff[3] = 0.0337;
    sharpV33_attr.stManual.stSelect.GaussianFilter_coeff[4] = 0.0261;
    sharpV33_attr.stManual.stSelect.GaussianFilter_coeff[5] = 0.0121;


    sharpV33_attr.stManual.stSelect.prefilter_sigma = 1.0;
    sharpV33_attr.stManual.stSelect.hfBilateralFilter_sigma = 1.0;
    sharpV33_attr.stManual.stSelect.GaussianFilter_sigma = 1.0;
    sharpV33_attr.stManual.stSelect.GaussianFilter_radius = 2.0;

    sharpV33_attr.stManual.stSelect.global_gain = 1.0;
    sharpV33_attr.stManual.stSelect.global_gain_alpha = 0.0;
    sharpV33_attr.stManual.stSelect.local_gainscale = 1.0;
    sharpV33_attr.stManual.stSelect.exgain_bypass = 0;
    sharpV33_attr.stManual.stSelect.noiseclip_strength = 1;
    sharpV33_attr.stManual.stSelect.enhance_bit = 3;
    sharpV33_attr.stManual.stSelect.noiseclip_mode = 0;
    sharpV33_attr.stManual.stSelect.noise_sigma_clip = 1023;

    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[0] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[1] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[2] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[3] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[4] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[5] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[6] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[7] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[8] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[9] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[10] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[11] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[12] = 1;
    sharpV33_attr.stManual.stSelect.gain_adj_sharp_strength[13] = 1;

    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[0] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[1] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[2] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[3] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[4] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[5] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[6] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[7] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[8] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[9] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[10] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[11] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[12] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[13] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[14] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[15] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[16] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[17] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[18] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[19] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[20] = 1;
    sharpV33_attr.stManual.stSelect.dis_adj_sharp_strength[21] = 1;

    ret = rk_aiq_user_api2_asharpV33_SetAttrib(ctx, &sharpV33_attr);
    printf("set sharp v4 attri manual ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v33_t get_sharpV33_attr;
    get_sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &get_sharpV33_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV33_attr.sync.done);

    return ret;
}


XCamReturn sample_sharp_setReg_v4(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_attrib_v4_t sharpV4_attr;
    sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &sharpV4_attr);
    sharpV4_attr.sync.sync_mode = sync_mode;
    sharpV4_attr.eMode = ASHARP4_OP_MODE_REG_MANUAL;

    // SHARP_SHARP_EN (0x0000)
    sharpV4_attr.stManual.stFix.sharp_clk_dis = 0;
    sharpV4_attr.stManual.stFix.sharp_exgain_bypass = 0;
    sharpV4_attr.stManual.stFix.sharp_center_mode = 0;
    sharpV4_attr.stManual.stFix.sharp_bypass = 0;
    sharpV4_attr.stManual.stFix.sharp_en = 1;

    // SHARP_SHARP_RATIO  (0x0004)
    sharpV4_attr.stManual.stFix.sharp_sharp_ratio = 0x28;
    sharpV4_attr.stManual.stFix.sharp_bf_ratio = 0x80;
    sharpV4_attr.stManual.stFix.sharp_gaus_ratio = 0x00;
    sharpV4_attr.stManual.stFix.sharp_pbf_ratio = 0x33;

    // SHARP_SHARP_LUMA_DX (0x0008)
    sharpV4_attr.stManual.stFix.sharp_luma_dx[0] = 0x6;
    sharpV4_attr.stManual.stFix.sharp_luma_dx[1] = 0x6;
    sharpV4_attr.stManual.stFix.sharp_luma_dx[2] = 0x7;
    sharpV4_attr.stManual.stFix.sharp_luma_dx[3] = 0x7;
    sharpV4_attr.stManual.stFix.sharp_luma_dx[4] = 0x8;
    sharpV4_attr.stManual.stFix.sharp_luma_dx[5] = 0x8;
    sharpV4_attr.stManual.stFix.sharp_luma_dx[6] = 0x7;

    // SHARP_SHARP_PBF_SIGMA_INV_0 (0x000c - 0x0014)
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[0] = 0x115;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[1] = 0x0c1;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[2] = 0x094;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[3] = 0x094;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[4] = 0x065;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[5] = 0x078;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[6] = 0x094;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_inv[7] = 0x094;

    // SHARP_SHARP_BF_SIGMA_INV_0 (0x0018 -  0x0020)
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[0] = 0x1c7;
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[1] = 0x13b;
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[2] = 0x0f1;
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[3] = 0x0f1;
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[4] = 0x0a4;
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[5] = 0x0c3;
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[6] = 0x0f1;
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_inv[7] = 0x0f1;

    // SHARP_SHARP_SIGMA_SHIFT (0x00024)
    sharpV4_attr.stManual.stFix.sharp_bf_sigma_shift = 0x6;
    sharpV4_attr.stManual.stFix.sharp_pbf_sigma_shift = 0x7;

    // SHARP_SHARP_EHF_TH_0 (0x0028 -  0x0030)
    sharpV4_attr.stManual.stFix.sharp_ehf_th[0] = 0x80;
    sharpV4_attr.stManual.stFix.sharp_ehf_th[1] = 0x80;
    sharpV4_attr.stManual.stFix.sharp_ehf_th[2] = 0x80;
    sharpV4_attr.stManual.stFix.sharp_ehf_th[3] = 0x80;
    sharpV4_attr.stManual.stFix.sharp_ehf_th[4] = 0x80;
    sharpV4_attr.stManual.stFix.sharp_ehf_th[5] = 0x80;
    sharpV4_attr.stManual.stFix.sharp_ehf_th[6] = 0x80;
    sharpV4_attr.stManual.stFix.sharp_ehf_th[7] = 0x80;

    // SHARP_SHARP_CLIP_HF_0 (0x0034 -  0x003c)
    sharpV4_attr.stManual.stFix.sharp_clip_hf[0] = 0x40;
    sharpV4_attr.stManual.stFix.sharp_clip_hf[1] = 0x40;
    sharpV4_attr.stManual.stFix.sharp_clip_hf[2] = 0x40;
    sharpV4_attr.stManual.stFix.sharp_clip_hf[3] = 0x40;
    sharpV4_attr.stManual.stFix.sharp_clip_hf[4] = 0x40;
    sharpV4_attr.stManual.stFix.sharp_clip_hf[5] = 0x40;
    sharpV4_attr.stManual.stFix.sharp_clip_hf[6] = 0x40;
    sharpV4_attr.stManual.stFix.sharp_clip_hf[7] = 0x40;

    // SHARP_SHARP_PBF_COEF (0x00040)
    sharpV4_attr.stManual.stFix.sharp_pbf_coef[0] = 0x18;
    sharpV4_attr.stManual.stFix.sharp_pbf_coef[1] = 0x10;
    sharpV4_attr.stManual.stFix.sharp_pbf_coef[2] = 0x0a;

    // SHARP_SHARP_BF_COEF (0x00044)
    sharpV4_attr.stManual.stFix.sharp_bf_coef[0] = 0x18;
    sharpV4_attr.stManual.stFix.sharp_bf_coef[1] = 0x10;
    sharpV4_attr.stManual.stFix.sharp_bf_coef[2] = 0x0a;

    // SHARP_SHARP_GAUS_COEF (0x00048 - 0x0004c)
    sharpV4_attr.stManual.stFix.sharp_gaus_coef[0] = 0x0c;
    sharpV4_attr.stManual.stFix.sharp_gaus_coef[1] = 0x07;
    sharpV4_attr.stManual.stFix.sharp_gaus_coef[2] = 0x06;
    sharpV4_attr.stManual.stFix.sharp_gaus_coef[3] = 0x05;
    sharpV4_attr.stManual.stFix.sharp_gaus_coef[4] = 0x04;
    sharpV4_attr.stManual.stFix.sharp_gaus_coef[5] = 0x03;


    ret = rk_aiq_user_api2_asharpV4_SetAttrib(ctx, &sharpV4_attr);
    printf("set sharp v4 attri manual ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v4_t get_sharpV4_attr;
    get_sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &get_sharpV4_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV4_attr.sync.done);

    return ret;
}



XCamReturn sample_sharp_setReg_v33(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_attrib_v33_t sharpV33_attr;
    sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &sharpV33_attr);
    sharpV33_attr.sync.sync_mode = sync_mode;
    sharpV33_attr.eMode = ASHARP_V33_OP_MODE_REG_MANUAL;


    // SHARP_SHARP_EN (0x0000)sharp_noiseclip_mode
    sharpV33_attr.stManual.stFix.sharp_noiseclip_mode = 0;
    sharpV33_attr.stManual.stFix.sharp_radius_ds_mode = 0;
    sharpV33_attr.stManual.stFix.sharp_exgain_bypass = 0;
    sharpV33_attr.stManual.stFix.sharp_center_mode = 0;
    sharpV33_attr.stManual.stFix.sharp_bypass = 0;
    sharpV33_attr.stManual.stFix.sharp_en = 1;

    // SHARP_SHARP_RATIO  (0x0004)
    sharpV33_attr.stManual.stFix.sharp_sharp_ratio = 0x28;
    sharpV33_attr.stManual.stFix.sharp_bf_ratio = 0x80;
    sharpV33_attr.stManual.stFix.sharp_gaus_ratio = 0x00;
    sharpV33_attr.stManual.stFix.sharp_pbf_ratio = 0x33;

    // SHARP_SHARP_LUMA_DX (0x0008)
    sharpV33_attr.stManual.stFix.sharp_luma_dx[0] = 0x6;
    sharpV33_attr.stManual.stFix.sharp_luma_dx[1] = 0x6;
    sharpV33_attr.stManual.stFix.sharp_luma_dx[2] = 0x7;
    sharpV33_attr.stManual.stFix.sharp_luma_dx[3] = 0x7;
    sharpV33_attr.stManual.stFix.sharp_luma_dx[4] = 0x8;
    sharpV33_attr.stManual.stFix.sharp_luma_dx[5] = 0x8;
    sharpV33_attr.stManual.stFix.sharp_luma_dx[6] = 0x7;

    // SHARP_SHARP_PBF_SIGMA_INV_0 (0x000c - 0x0014)
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[0] = 0x115;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[1] = 0x0c1;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[2] = 0x094;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[3] = 0x094;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[4] = 0x065;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[5] = 0x078;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[6] = 0x094;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_inv[7] = 0x094;

    // SHARP_SHARP_BF_SIGMA_INV_0 (0x0018 -  0x0020)
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[0] = 0x1c7;
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[1] = 0x13b;
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[2] = 0x0f1;
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[3] = 0x0f1;
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[4] = 0x0a4;
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[5] = 0x0c3;
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[6] = 0x0f1;
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_inv[7] = 0x0f1;

    // SHARP_SHARP_SIGMA_SHIFT (0x00024)
    sharpV33_attr.stManual.stFix.sharp_bf_sigma_shift = 0x6;
    sharpV33_attr.stManual.stFix.sharp_pbf_sigma_shift = 0x7;

    // SHARP_SHARP_CLIP_HF_0 (0x0034 -  0x003c)
    sharpV33_attr.stManual.stFix.sharp_clip_hf[0] = 0x40;
    sharpV33_attr.stManual.stFix.sharp_clip_hf[1] = 0x40;
    sharpV33_attr.stManual.stFix.sharp_clip_hf[2] = 0x40;
    sharpV33_attr.stManual.stFix.sharp_clip_hf[3] = 0x40;
    sharpV33_attr.stManual.stFix.sharp_clip_hf[4] = 0x40;
    sharpV33_attr.stManual.stFix.sharp_clip_hf[5] = 0x40;
    sharpV33_attr.stManual.stFix.sharp_clip_hf[6] = 0x40;
    sharpV33_attr.stManual.stFix.sharp_clip_hf[7] = 0x40;

    // SHARP_SHARP_PBF_COEF (0x00040)
    sharpV33_attr.stManual.stFix.sharp_pbf_coef[0] = 0x18;
    sharpV33_attr.stManual.stFix.sharp_pbf_coef[1] = 0x10;
    sharpV33_attr.stManual.stFix.sharp_pbf_coef[2] = 0x0a;

    // SHARP_SHARP_BF_COEF (0x00044)
    sharpV33_attr.stManual.stFix.sharp_bf_coef[0] = 0x18;
    sharpV33_attr.stManual.stFix.sharp_bf_coef[1] = 0x10;
    sharpV33_attr.stManual.stFix.sharp_bf_coef[2] = 0x0a;

    // SHARP_SHARP_GAUS_COEF (0x00048 - 0x0004c)
    sharpV33_attr.stManual.stFix.sharp_gaus_coef[0] = 0x0c;
    sharpV33_attr.stManual.stFix.sharp_gaus_coef[1] = 0x07;
    sharpV33_attr.stManual.stFix.sharp_gaus_coef[2] = 0x06;
    sharpV33_attr.stManual.stFix.sharp_gaus_coef[3] = 0x05;
    sharpV33_attr.stManual.stFix.sharp_gaus_coef[4] = 0x04;
    sharpV33_attr.stManual.stFix.sharp_gaus_coef[5] = 0x03;

    // SHARP_GAIN (0x50)
    sharpV33_attr.stManual.stFix.sharp_global_gain = 0x10;
    sharpV33_attr.stManual.stFix.sharp_global_gain_alpha = 0x0;
    sharpV33_attr.stManual.stFix.sharp_local_gainscale = 0x80;

    // SHARP_GAIN_ADJUST (0x54)
    sharpV33_attr.stManual.stFix.sharp_gain_adj[0] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[1] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[2] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[3] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[4] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[5] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[6] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[7] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[8] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[9] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[10] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[11] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[12] = 0x400;
    sharpV33_attr.stManual.stFix.sharp_gain_adj[13] = 0x400;

    // SHARP_CENTER (0x70)
    sharpV33_attr.stManual.stFix.sharp_center_wid = 0x0;
    sharpV33_attr.stManual.stFix.sharp_center_wid = 0x0;

    // SHARP_GAIN_DIS_STRENGTH (0x74)
    sharpV33_attr.stManual.stFix.sharp_strength[0] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[1] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[2] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[3] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[4] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[5] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[6] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[7] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[8] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[9] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[10] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[11] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[12] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[13] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[14] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[15] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[16] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[17] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[18] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[19] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[20] = 0x80;
    sharpV33_attr.stManual.stFix.sharp_strength[21] = 0x80;

    // SHARP_TEXTURE (0x8c)
    sharpV33_attr.stManual.stFix.sharp_noise_sigma = 0x3ff;
    sharpV33_attr.stManual.stFix.sharp_enhance_bit = 0x5;
    sharpV33_attr.stManual.stFix.sharp_noise_strength = 0x400;

    ret = rk_aiq_user_api2_asharpV33_SetAttrib(ctx, &sharpV33_attr);
    printf("set sharp v4 attri manual ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v33_t get_sharpV33_attr;
    get_sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &get_sharpV33_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV33_attr.sync.done);

    return ret;
}


XCamReturn sample_sharp_setStrength_v4(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, float fStrength)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_strength_v4_t set_sharpV4Strength;
    set_sharpV4Strength.sync.sync_mode = sync_mode;
    set_sharpV4Strength.percent = fStrength;
    ret = rk_aiq_user_api2_asharpV4_SetStrength(ctx, &set_sharpV4Strength);
    printf("Set asharp v4 set streangth ret:%d strength:%f \n\n", ret, set_sharpV4Strength.percent);

    rk_aiq_sharp_strength_v4_t get_sharpV4Strength;
    get_sharpV4Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetStrength(ctx, &get_sharpV4Strength);
    printf("get asharp v4 attri ret:%d strength:%f done:%d\n\n",
           ret, get_sharpV4Strength.percent, get_sharpV4Strength.sync.done);

    return ret;
}

XCamReturn sample_sharp_setStrength_v33(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, float fStrength)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_strength_v33_t set_sharpV33Strength;
    set_sharpV33Strength.sync.sync_mode = sync_mode;
    set_sharpV33Strength.percent = fStrength;
    set_sharpV33Strength.strength_enable = true;
    ret = rk_aiq_user_api2_asharpV33_SetStrength(ctx, &set_sharpV33Strength);
    printf("Set asharp v4 set streangth ret:%d strength:%f \n\n", ret, set_sharpV33Strength.percent);

    rk_aiq_sharp_strength_v33_t get_sharpV33Strength;
    get_sharpV33Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetStrength(ctx, &get_sharpV33Strength);
    printf("get asharp v4 attri ret:%d strength:%f done:%d\n\n",
           ret, get_sharpV33Strength.percent, get_sharpV33Strength.sync.done);

    return ret;
}


XCamReturn sample_sharp_getStrength_v4(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_strength_v4_t sharpV4Strength;
    sharpV4Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetStrength(ctx, &sharpV4Strength);
    printf("get asharp v4 attri ret:%d strength:%f done:%d\n\n",
           ret, sharpV4Strength.percent, sharpV4Strength.sync.done);

    return ret;
}


XCamReturn sample_sharp_getStrength_v33(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_strength_v33_t sharpV33Strength;
    sharpV33Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetStrength(ctx, &sharpV33Strength);
    printf("get asharp v4 attri ret:%d strength:%f done:%d\n\n",
           ret, sharpV33Strength.percent, sharpV33Strength.sync.done);

    return ret;
}

XCamReturn sample_sharp_getAttri_v4(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_attrib_v4_t sharpV4_attr;
    sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &sharpV4_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, sharpV4_attr.sync.done);

    return ret;
}

XCamReturn sample_sharp_getAttri_v33(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_sharp_attrib_v33_t sharpV33_attr;
    sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &sharpV33_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, sharpV33_attr.sync.done);

    return ret;
}


XCamReturn sample_sharp_SetDefault_v4(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_sharp_attrib_v4_t &default_sharpV4_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = rk_aiq_user_api2_asharpV4_SetAttrib(ctx, &default_sharpV4_attr);
    printf("set sharp v4 default auto attri ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v4_t get_sharpV4_attr;
    get_sharpV4_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &get_sharpV4_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV4_attr.sync.done);

    return ret;
}

XCamReturn sample_sharp_SetDefault_v33(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_sharp_attrib_v33_t &default_sharpV33_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = rk_aiq_user_api2_asharpV33_SetAttrib(ctx, &default_sharpV33_attr);
    printf("set sharp v4 default auto attri ret:%d \n\n", ret);

    rk_aiq_sharp_attrib_v33_t get_sharpV33_attr;
    get_sharpV33_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &get_sharpV33_attr);
    printf("get asharp v4 attri ret:%d done:%d\n\n", ret, get_sharpV33_attr.sync.done);

    return ret;
}


XCamReturn sample_asharp_module (const void *arg)
{
    int key = -1;
    CLEAR();
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx ;

    if (demo_ctx->camGroup) {
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

    rk_aiq_sharp_attrib_v4_t default_sharpV4_attr;
    rk_aiq_sharp_attrib_v33_t default_sharpV33_attr;
    if (CHECK_ISP_HW_V30()) {
        ret = rk_aiq_user_api2_asharpV4_GetAttrib(ctx, &default_sharpV4_attr);
        printf("get asharp v4 default auto attri ret:%d \n\n", ret);
    }

    if (CHECK_ISP_HW_V32()) {
        ret = rk_aiq_user_api2_asharpV33_GetAttrib(ctx, &default_sharpV33_attr);
        printf("get asharp v33 default auto attri ret:%d \n\n", ret);
    }

    do {
        sample_asharp_usage ();

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar ();
        printf ("\n");

        switch (key) {
        case '0':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_getAttri_v4(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_getAttri_v33(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '1':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_getStrength_v4(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_getStrength_v33(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '2':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setAuto_v4(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setAuto_v33(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '3':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setManual_v4(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setManual_v33(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '4':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setReg_v4(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setReg_v33(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '5':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setStrength_v4(ctx, RK_AIQ_UAPI_MODE_SYNC, 1.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setStrength_v33(ctx, RK_AIQ_UAPI_MODE_SYNC, 1.0);
            }
            break;
        case '6':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setStrength_v4(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setStrength_v33(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.0);
            }
            break;
        case '7':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setStrength_v4(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.5);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setStrength_v33(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.5);
            }
            break;
        case '8':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_SetDefault_v4(ctx, RK_AIQ_UAPI_MODE_SYNC, default_sharpV4_attr);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_SetDefault_v33(ctx, RK_AIQ_UAPI_MODE_SYNC, default_sharpV33_attr);
            }
            break;
        case 'a':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_getAttri_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_getAttri_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'b':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_getStrength_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_getStrength_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'c':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setAuto_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setAuto_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'd':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setManual_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setManual_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'e':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setReg_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setReg_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'f':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setStrength_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC, 1.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setStrength_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC, 1.0);
            }
            break;
        case 'g':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setStrength_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.0);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setStrength_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.0);
            }
            break;
        case 'h':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_setStrength_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.5);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_setStrength_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.5);
            }
            break;
        case 'i':
            if (CHECK_ISP_HW_V30()) {
                sample_sharp_SetDefault_v4(ctx, RK_AIQ_UAPI_MODE_ASYNC, default_sharpV4_attr);
            }
            if (CHECK_ISP_HW_V32()) {
                sample_sharp_SetDefault_v33(ctx, RK_AIQ_UAPI_MODE_ASYNC, default_sharpV33_attr);
            }
            break;
        default:
            printf("not support test\n\n");
            break;
        }

    } while (key != 'q' && key != 'Q');

    return ret;
}

