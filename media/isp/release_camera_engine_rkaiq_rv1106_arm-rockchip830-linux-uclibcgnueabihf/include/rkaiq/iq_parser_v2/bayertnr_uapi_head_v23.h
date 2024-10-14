/*
 * bayertnr_uapi_head_v23.h
 *
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

#ifndef __CALIBDBV2_BAYER_TNR_UAPI_HEADER_V23_H__
#define __CALIBDBV2_BAYER_TNR_UAPI_HEADER_V23_H__

#include "rk_aiq_comm.h"
// clang-format off

RKAIQ_BEGIN_DECLARE

////////////////////////bayernr V23//////////////////////////////////////
#if 1

typedef struct RK_Bayertnr_Params_V23_Select_s
{
    // M4_BOOL_DESC("enable", "1")
    bool enable;

    //calib
    // M4_ARRAY_DESC("lumapoint", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[512, 1024, 1536, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216, 10240, 11264, 12288, 13312, 14336]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint[16];
    // M4_ARRAY_DESC("sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "32", M4_DIGIT(0), M4_DYNAMIC(0))
    int sigma[16];
    // M4_ARRAY_DESC("lumapoint2", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[512, 1024, 1536, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216, 10240, 11264, 12288, 13312, 14336]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint2[16];
    // M4_ARRAY_DESC("lo_sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "32", M4_DIGIT(0), M4_DYNAMIC(0))
    int lo_sigma[16];
    // M4_ARRAY_DESC("hi_sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "256", M4_DIGIT(0), M4_DYNAMIC(0))
    int hi_sigma[16];

    //tuning
    // M4_NUMBER_DESC("thumbds_w", "u8", M4_RANGE(4, 8), "8", M4_DIGIT(0))
    uint8_t thumbds_w;
    // M4_NUMBER_DESC("thumbds_h", "u8", M4_RANGE(4, 8), "4", M4_DIGIT(0))
    uint8_t thumbds_h;

    // M4_BOOL_DESC("lo_enable", "1")
    bool lo_enable;
    // M4_BOOL_DESC("hi_enable", "1")
    bool hi_enable;
    // M4_BOOL_DESC("lo_med_en", "1")
    bool lo_med_en;
    // M4_BOOL_DESC("lo_gsbay_en", "1")
    bool lo_gsbay_en;
    // M4_BOOL_DESC("lo_gslum_en", "1")
    bool lo_gslum_en;
    // M4_BOOL_DESC("hi_med_en", "1")
    bool hi_med_en;
    // M4_BOOL_DESC("hi_gslum_en", "1")
    bool hi_gslum_en;

    // M4_BOOL_DESC("trans_en", "1")
    bool trans_en;

    // M4_BOOL_DESC("wgt_use_mode", "1")
    bool wgt_use_mode;
    // M4_BOOL_DESC("wgt_mge_mode", "1")
    bool wgt_mge_mode;
    // M4_BOOL_DESC("hi_guass", "0")
    bool hi_guass;
    // M4_BOOL_DESC("kl_guass", "0")
    bool kl_guass;

    // M4_BOOL_DESC("global_pk_en", "0")
    bool global_pk_en;
    // M4_NUMBER_DESC("global_pksq", "s32", M4_RANGE(0, 268435455), "1024", M4_DIGIT(0))
    int global_pksq;

    // M4_NUMBER_DESC("lo_filter_strength", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float lo_filter_strength;
    // M4_NUMBER_DESC("hi_filter_strength", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float hi_filter_strength;
    // M4_NUMBER_DESC("soft_threshold_ratio", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(2))
    float soft_threshold_ratio;

    // M4_NUMBER_DESC("lo_clipwgt", "f32", M4_RANGE(0, 1.0), "0.03215", M4_DIGIT(5))
    float lo_clipwgt;
    // M4_NUMBER_DESC("hi_wgt_comp", "f32", M4_RANGE(0, 1.0), "0.16", M4_DIGIT(2))
    float hi_wgt_comp;
    // M4_NUMBER_DESC("hidif_th", "s32", M4_RANGE(0, 65535), "32767", M4_DIGIT(0))
    int   hidif_th;

    // M4_NUMBER_DESC("lo_filter_rat0", "f32", M4_RANGE(0, 16), "0", M4_DIGIT(2))
    float lo_filter_rat0;
    // M4_NUMBER_DESC("lo_filter_thed0", "u16", M4_RANGE(0, 4095), "1024", M4_DIGIT(0))
    uint16_t   lo_filter_thed0;

    // M4_NUMBER_DESC("hi_filter_abs_ctrl", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t hi_filter_abs_ctrl;
    // M4_NUMBER_DESC("hi_filter_filt_bay", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t hi_filter_filt_bay;
    // M4_NUMBER_DESC("hi_filter_filt_avg", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t hi_filter_filt_avg;
    // M4_NUMBER_DESC("hi_filter_filt_mode", "u8", M4_RANGE(0, 4), "4", M4_DIGIT(0))
    uint8_t hi_filter_filt_mode;


    // M4_NUMBER_DESC("hi_filter_rat0", "f32", M4_RANGE(0, 16.0), "0", M4_DIGIT(2))
    float hi_filter_rat0;
    // M4_NUMBER_DESC("hi_filter_thed0", "u16", M4_RANGE(0, 4095), "1024", M4_DIGIT(0))
    uint16_t hi_filter_thed0;
    // M4_NUMBER_DESC("hi_filter_rat1", "f32", M4_RANGE(0, 16), "0", M4_DIGIT(2))
    float hi_filter_rat1;
    // M4_NUMBER_DESC("hi_filter_thed1", "u16", M4_RANGE(0, 4095), "1024", M4_DIGIT(0))
    uint16_t hi_filter_thed1;


    // M4_NUMBER_DESC("guass_guide_coeff0", "u8", M4_RANGE(0, 63), "16", M4_DIGIT(0))
    uint8_t guass_guide_coeff0;
    // M4_NUMBER_DESC("guass_guide_coeff1", "u8", M4_RANGE(0, 63), "8", M4_DIGIT(0))
    uint8_t guass_guide_coeff1;
    // M4_NUMBER_DESC("guass_guide_coeff2", "u8", M4_RANGE(0, 63), "16", M4_DIGIT(0))
    uint8_t guass_guide_coeff2;
    // M4_NUMBER_DESC("guass_guide_coeff3", "u8", M4_RANGE(0, 63), "8", M4_DIGIT(0))
    uint8_t guass_guide_coeff3;

} RK_Bayertnr_Params_V23_Select_t;

typedef struct RK_Bayertnr_Param_V23L_Select_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;

    // calib
    // M4_ARRAY_DESC("lumapoint", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[512, 1024, 1536, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216, 10240, 11264, 12288, 13312, 14336]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint[16];
    // M4_ARRAY_DESC("sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "32", M4_DIGIT(0),M4_DYNAMIC(0))
    int sigma[16];
    // M4_ARRAY_DESC("lumapoint2", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "[512, 1024, 1536, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216, 10240, 11264, 12288, 13312, 14336]", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint2[16];
    // M4_ARRAY_DESC("lo_sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "32", M4_DIGIT(0), M4_DYNAMIC(0))
    int lo_sigma[16];
    // M4_ARRAY_DESC("hi_sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "256", M4_DIGIT(0), M4_DYNAMIC(0))
    int hi_sigma[16];

    // tuning
    // M4_NUMBER_DESC("thumbds_w", "u8", M4_RANGE(4, 8), "8", M4_DIGIT(0))
    uint8_t thumbds_w;
    // M4_NUMBER_DESC("thumbds_h", "u8", M4_RANGE(4, 8), "4", M4_DIGIT(0))
    uint8_t thumbds_h;

    // M4_BOOL_DESC("lo_enable", "1")
    bool lo_enable;
    // M4_BOOL_DESC("hi_enable", "1")
    bool hi_enable;
#if 0
    // M4_BOOL_DESC("lo_med_en", "1")
    bool lo_med_en;
#endif
    // M4_BOOL_DESC("lo_gsbay_en", "1")
    bool lo_gsbay_en;
    // M4_BOOL_DESC("lo_gslum_en", "1")
    bool lo_gslum_en;
#if 0
    // M4_BOOL_DESC("hi_med_en", "1")
    bool hi_med_en;
#endif
    // M4_BOOL_DESC("hi_gslum_en", "1")
    bool hi_gslum_en;

    // M4_BOOL_DESC("trans_en", "1")
    bool trans_en;

    // M4_BOOL_DESC("wgt_use_mode", "1")
    bool wgt_use_mode;
    // M4_BOOL_DESC("wgt_mge_mode", "1")
    bool wgt_mge_mode;
#if 0
    // M4_BOOL_DESC("hi_guass", "0")
    bool hi_guass;
    // M4_BOOL_DESC("kl_guass", "0")
    bool kl_guass;
#endif

    // M4_BOOL_DESC("global_pk_en", "0")
    bool global_pk_en;
    // M4_NUMBER_DESC("global_pksq", "s32", M4_RANGE(0, 268435455), "1024", M4_DIGIT(0))
    int global_pksq;

    // M4_NUMBER_DESC("lo_filter_strength", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float lo_filter_strength;
    // M4_NUMBER_DESC("hi_filter_strength", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float hi_filter_strength;
    // M4_NUMBER_DESC("soft_threshold_ratio", "f32", M4_RANGE(0, 1.0), "0.0", M4_DIGIT(2))
    float soft_threshold_ratio;

    // M4_NUMBER_DESC("lo_clipwgt", "f32", M4_RANGE(0, 1.0), "0.03215", M4_DIGIT(5))
    float lo_clipwgt;
    // M4_NUMBER_DESC("hi_wgt_comp", "f32", M4_RANGE(0, 1.0), "0.16", M4_DIGIT(2))
    float hi_wgt_comp;
    // M4_NUMBER_DESC("hidif_th", "s32", M4_RANGE(0, 65535), "32767", M4_DIGIT(0))
    int hidif_th;

    // M4_NUMBER_DESC("lo_filter_rat0", "f32", M4_RANGE(0, 16), "0", M4_DIGIT(2))
    float lo_filter_rat0;
    // M4_NUMBER_DESC("lo_filter_thed0", "u16", M4_RANGE(0, 4095), "1024", M4_DIGIT(0))
    uint16_t lo_filter_thed0;

    // M4_NUMBER_DESC("hi_filter_abs_ctrl", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t hi_filter_abs_ctrl;
#if 0
    // M4_NUMBER_DESC("hi_filter_filt_bay", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t hi_filter_filt_bay;
    // M4_NUMBER_DESC("hi_filter_filt_avg", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    uint8_t hi_filter_filt_avg;
    // M4_NUMBER_DESC("hi_filter_filt_mode", "u8", M4_RANGE(0, 4), "4", M4_DIGIT(0))
    uint8_t hi_filter_filt_mode;
#endif

    // isp32 lite new param
#if 1
    // M4_BOOL_DESC("wgtmm_opt_en", "0")
    bool wgtmm_opt_en;
    // M4_BOOL_DESC("wgtmm_sel_en", "0")
    bool wgtmm_sel_en;
    // M4_NUMBER_DESC("wgtmin", "f32", M4_RANGE(0, 1), "0", M4_DIGIT(3))
    float wgtmin;
#endif

    // M4_NUMBER_DESC("hi_filter_rat0", "f32", M4_RANGE(0, 16.0), "0", M4_DIGIT(2))
    float hi_filter_rat0;
    // M4_NUMBER_DESC("hi_filter_thed0", "u16", M4_RANGE(0, 4095), "1024", M4_DIGIT(0))
    uint16_t hi_filter_thed0;
    // M4_NUMBER_DESC("hi_filter_rat1", "f32", M4_RANGE(0, 16), "0", M4_DIGIT(2))
    float hi_filter_rat1;
    // M4_NUMBER_DESC("hi_filter_thed1", "u16", M4_RANGE(0, 4095), "1024", M4_DIGIT(0))
    uint16_t hi_filter_thed1;

    // M4_NUMBER_DESC("guass_guide_coeff0", "u8", M4_RANGE(0, 63), "16", M4_DIGIT(0))
    uint8_t guass_guide_coeff0;
    // M4_NUMBER_DESC("guass_guide_coeff1", "u8", M4_RANGE(0, 63), "8", M4_DIGIT(0))
    uint8_t guass_guide_coeff1;
    // M4_NUMBER_DESC("guass_guide_coeff2", "u8", M4_RANGE(0, 63), "4", M4_DIGIT(0))
    uint8_t guass_guide_coeff2;
#if 0
    // M4_NUMBER_DESC("guass_guide_coeff3", "u8", M4_RANGE(0, 63), "0", M4_DIGIT(0))
    uint8_t guass_guide_coeff3;
#endif

} RK_Bayertnr_Param_V23L_Select_t;

typedef struct Abayertnr_ExpInfo_V23_s {
    // M4_NUMBER_DESC("hdr_mode", "u8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int hdr_mode;

    // M4_NUMBER_DESC("snr_mode", "s8", M4_RANGE(0, 2), "0", M4_DIGIT(0))
    int   snr_mode;

    // M4_ARRAY_DESC("time", "f32", M4_SIZE(1,3), M4_RANGE(0, 1024), "0.01", M4_DIGIT(6))
    float arTime[3];
    // M4_ARRAY_DESC("again", "f32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float arAGain[3];
    // M4_ARRAY_DESC("dgain", "f32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float arDGain[3];
    // M4_ARRAY_DESC("isp_dgain", "f32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float isp_dgain[3];
    // M4_NUMBER_DESC("blc_ob_predgain", "f32", M4_RANGE(0, 204800), "1", M4_DIGIT(3))
    float blc_ob_predgain;
    // M4_ARRAY_DESC("iso", "u32", M4_SIZE(1,3), M4_RANGE(0, 204800), "1", M4_DIGIT(0))
    int   arIso[3];

    // M4_NUMBER_DESC("isoLevelLow", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelLow;
    // M4_NUMBER_DESC("isoLevelHig", "u8", M4_RANGE(0, 12), "0", M4_DIGIT(0))
    int isoLevelHig;

} Abayertnr_ExpInfo_V23_t;

typedef struct rk_aiq_bayertnr_info_v23_s {
    // M4_ARRAY_TABLE_DESC("sync", "array_table_ui", "none", "1")
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayertnr_ExpInfo_V23_t expo_info;
} rk_aiq_bayertnr_info_v23_t;


#endif


RKAIQ_END_DECLARE
// clang-format on

#endif
