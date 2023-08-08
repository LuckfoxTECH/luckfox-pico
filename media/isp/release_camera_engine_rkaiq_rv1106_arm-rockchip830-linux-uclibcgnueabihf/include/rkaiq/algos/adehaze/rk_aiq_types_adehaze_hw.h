/*
 * rk_aiq_algo_adehaze_hw.h
 *
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

#ifndef _RK_AIQ_ALGO_ADEHAZE_HW_H_
#define _RK_AIQ_ALGO_ADEHAZE_HW_H_

#define DHAZ_V10_CONV_T_NUM        6
#define DHAZ_V11_SIGMA_IDX_NUM     15
#define DHAZ_V11_SIGMA_LUT_NUM     17
#define DHAZ_V11_ENHANCE_CRUVE_NUM 17
#define DHAZ_V11_HIST_WR_NUM       64
#define DHAZ_V12_SIGMA_IDX_NUM     DHAZ_V11_SIGMA_IDX_NUM
#define DHAZ_V12_SIGMA_LUT_NUM     DHAZ_V11_SIGMA_LUT_NUM
#define DHAZ_V12_ENHANCE_CRUVE_NUM DHAZ_V11_ENHANCE_CRUVE_NUM
#define DHAZ_V12_HIST_WR_NUM       DHAZ_V11_HIST_WR_NUM
#define DHAZ_V12_ENH_LUMA_NUM      17

typedef struct AdehazeV10ProcResult_s {
    int enhance_en;
    int hist_chn;
    int hpara_en;
    int hist_en;
    int dc_en;
    int big_en;
    int nobig_en;
    int yblk_th;
    int yhist_th;
    int dc_max_th;
    int dc_min_th;
    int wt_max;
    int bright_max;
    int bright_min;
    int tmax_base;
    int dark_th;
    int air_max;
    int air_min;
    int tmax_max;
    int tmax_off;
    int hist_th_off;
    int hist_gratio;
    int hist_min;
    int hist_k;
    int enhance_value;
    int hist_scale;
    int iir_wt_sigma;
    int iir_sigma;
    int stab_fnum;
    int iir_tmax_sigma;
    int iir_air_sigma;
    int cfg_wt;
    int cfg_air;
    int cfg_alpha;
    int cfg_gratio;
    int cfg_tmax;
    int dc_weitcur;
    int dc_thed;
    int sw_dhaz_dc_bf_h3;
    int sw_dhaz_dc_bf_h2;
    int sw_dhaz_dc_bf_h1;
    int sw_dhaz_dc_bf_h0;
    int sw_dhaz_dc_bf_h5;
    int sw_dhaz_dc_bf_h4;
    int air_weitcur;
    int air_thed;
    int air_bf_h2;
    int air_bf_h1;
    int air_bf_h0;
    int gaus_h2;
    int gaus_h1;
    int gaus_h0;
    int conv_t0[DHAZ_V10_CONV_T_NUM];
    int conv_t1[DHAZ_V10_CONV_T_NUM];
    int conv_t2[DHAZ_V10_CONV_T_NUM];
} AdehazeV10ProcResult_t;

typedef struct AdehazeV11ProcResult_s {
    int enhance_en;
    int air_lc_en;
    int hpara_en;
    int hist_en;
    int dc_en;
    int yblk_th;
    int yhist_th;
    int dc_max_th;
    int dc_min_th;
    int wt_max;
    int bright_max;
    int bright_min;
    int tmax_base;
    int dark_th;
    int air_max;
    int air_min;
    int tmax_max;
    int tmax_off;
    int hist_k;
    int hist_th_off;
    int hist_min;
    int hist_gratio;
    int hist_scale;
    int enhance_value;
    int enhance_chroma;
    int iir_wt_sigma;
    int iir_sigma;
    int stab_fnum;
    int iir_tmax_sigma;
    int iir_air_sigma;
    int iir_pre_wet;
    int cfg_wt;
    int cfg_air;
    int cfg_alpha;
    int cfg_gratio;
    int cfg_tmax;
    int range_sima;
    int space_sigma_pre;
    int space_sigma_cur;
    int dc_weitcur;
    int bf_weight;
    int enh_curve[DHAZ_V11_ENHANCE_CRUVE_NUM];
    int gaus_h2;
    int gaus_h1;
    int gaus_h0;
} AdehazeV11ProcResult_t;

typedef struct AdehazeV11duoProcResult_s {
    int round_en;
    int soft_wr_en;
    int enhance_en;
    int air_lc_en;
    int hpara_en;
    int hist_en;
    int dc_en;
    int yblk_th;
    int yhist_th;
    int dc_max_th;
    int dc_min_th;
    int wt_max;
    int bright_max;
    int bright_min;
    int tmax_base;
    int dark_th;
    int air_max;
    int air_min;
    int tmax_max;
    int tmax_off;
    int hist_k;
    int hist_th_off;
    int hist_min;
    int hist_gratio;
    int hist_scale;
    int enhance_value;
    int enhance_chroma;
    int iir_wt_sigma;
    int iir_sigma;
    int stab_fnum;
    int iir_tmax_sigma;
    int iir_air_sigma;
    int iir_pre_wet;
    int cfg_wt;
    int cfg_air;
    int cfg_alpha;
    int cfg_gratio;
    int cfg_tmax;
    int range_sima;
    int space_sigma_pre;
    int space_sigma_cur;
    int dc_weitcur;
    int bf_weight;
    int enh_curve[DHAZ_V11_ENHANCE_CRUVE_NUM];
    int gaus_h2;
    int gaus_h1;
    int gaus_h0;
    int sigma_idx[DHAZ_V11_SIGMA_IDX_NUM];
    int sigma_lut[DHAZ_V11_SIGMA_LUT_NUM];
    int adp_wt_wr;                      // calc in kernel
    int adp_air_wr;                     // calc in kernel
    int adp_tmax_wr;                    // calc in kernel
    int adp_gratio_wr;                  // calc in kernel
    int hist_wr[DHAZ_V11_HIST_WR_NUM];  // calc in kernel
} AdehazeV11duoProcResult_t;

typedef struct AdehazeV12ProcResult_s {
    unsigned char enh_luma_en;
    unsigned char color_deviate_en;
    unsigned char round_en;
    unsigned char soft_wr_en;
    unsigned char enhance_en;
    unsigned char air_lc_en;
    unsigned char hpara_en;
    unsigned char hist_en;
    unsigned char dc_en;
    unsigned char yblk_th;
    unsigned char yhist_th;
    unsigned char dc_max_th;
    unsigned char dc_min_th;
    unsigned char tmax_base;
    unsigned char dark_th;
    unsigned char air_max;
    unsigned char air_min;
    unsigned char gaus_h2;
    unsigned char gaus_h1;
    unsigned char gaus_h0;
    unsigned char hist_gratio;
    unsigned short hist_scale;
    unsigned char bright_max;
    unsigned char bright_min;
    unsigned short wt_max;
    unsigned short tmax_max;
    unsigned short tmax_off;
    unsigned char hist_k;
    unsigned char hist_th_off;
    unsigned short hist_min;
    unsigned short enhance_value;
    unsigned short enhance_chroma;
    unsigned short iir_wt_sigma;
    unsigned char iir_sigma;
    unsigned char stab_fnum;
    unsigned short iir_tmax_sigma;
    unsigned char iir_air_sigma;
    unsigned char iir_pre_wet;
    unsigned short cfg_wt;
    unsigned char cfg_air;
    unsigned char cfg_alpha;
    unsigned short cfg_gratio;
    unsigned short cfg_tmax;
    unsigned short range_sima;
    unsigned char space_sigma_pre;
    unsigned char space_sigma_cur;
    unsigned short dc_weitcur;
    unsigned short bf_weight;
    unsigned short enh_curve[DHAZ_V12_ENHANCE_CRUVE_NUM];
    unsigned short sigma_lut[DHAZ_V12_SIGMA_LUT_NUM];
    unsigned char sigma_idx[DHAZ_V12_SIGMA_IDX_NUM];
    unsigned short hist_wr[DHAZ_V12_HIST_WR_NUM];
    unsigned short enh_luma[DHAZ_V12_ENH_LUMA_NUM];
} AdehazeV12ProcResult_t;

typedef struct RkAiqAdehazeProcResult_s {
    bool enable;
    bool update;
#if RKAIQ_HAVE_DEHAZE_V10
    AdehazeV10ProcResult_t ProcResV10;
#endif
#if RKAIQ_HAVE_DEHAZE_V11
    AdehazeV11ProcResult_t ProcResV11;
#endif
#if RKAIQ_HAVE_DEHAZE_V11_DUO
    AdehazeV11duoProcResult_t ProcResV11duo;
#endif
#if RKAIQ_HAVE_DEHAZE_V12
    AdehazeV12ProcResult_t ProcResV12;
#endif
} RkAiqAdehazeProcResult_t;

#endif  //_RK_AIQ_ALGO_ADEHAZE_HW_H_
