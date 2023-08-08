/*
 *rk_aiq_types_adehaze_algo_int.h
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

#ifndef _RK_AIQ_TYPE_ADEHAZE_ALGO_INT_H_
#define _RK_AIQ_TYPE_ADEHAZE_ALGO_INT_H_
#include "RkAiqCalibDbTypes.h"
#include "adehaze/rk_aiq_types_adehaze_algo.h"
#include "adehaze_head.h"
#include "adehaze_uapi_head.h"

typedef enum AdehazeVersion_e {
    ADEHAZE_ISP20      = 0,
    ADEHAZE_ISP21      = 1,
    ADEHAZE_ISP30      = 2,
    ADEHAZE_VERSION_MAX
} AdehazeVersion_t;

typedef enum dehaze_api_mode_s {
    DEHAZE_API_AUTO   = 0, /**< run auto mode */
    DEHAZE_API_MANUAL = 1, /**< run manual mode*/
} dehaze_api_mode_t;

typedef struct DehazeManuAttr_s {
    bool update;
    int level;//0~100
} DehazeManuAttr_t;

typedef struct EnhanceManuAttr_s {
    bool update;
    int level;//0~100
} EnhanceManuAttr_t;

typedef struct adehaze_sw_V2_s {
    rk_aiq_uapi_sync_t sync;

    dehaze_api_mode_t mode;
    aDehazeAttr_t stAuto;
    mDehazeAttr_t stManual;
    DehazeManuAttr_t stDehazeManu;
    EnhanceManuAttr_t stEnhanceManu;
} adehaze_sw_V2_t;

typedef struct AdehazeExpInfo_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
} AdehazeExpInfo_t;

typedef struct AdehazeV20ProcResult_s
{
    bool enable;
    bool update;
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
    int conv_t0[6];
    int conv_t1[6];
    int conv_t2[6];
} AdehazeV20ProcResult_t;

typedef struct AdehazeV21ProcResult_s
{
    bool enable;
    bool update;
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
    int enh_curve[17];
    int gaus_h2;
    int gaus_h1;
    int gaus_h0;
} AdehazeV21ProcResult_t;

typedef struct AdehazeV30ProcResult_s
{
    bool enable;
    bool update;
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
    int enh_curve[17];
    int gaus_h2;
    int gaus_h1;
    int gaus_h0;
    int sigma_idx[ISP3X_DHAZ_SIGMA_IDX_NUM];
    int sigma_lut[ISP3X_DHAZ_SIGMA_LUT_NUM];
    int adp_wt_wr;//calc in kernel
    int adp_air_wr;//calc in kernel
    int adp_tmax_wr;//calc in kernel
    int adp_gratio_wr;//calc in kernel
    int hist_wr[64];//calc in kernel
} AdehazeV30ProcResult_t;

typedef struct RkAiqAdehazeProcResult_s
{
    union {
        AdehazeV20ProcResult_t ProcResV20;
        AdehazeV21ProcResult_t ProcResV21;
        AdehazeV30ProcResult_t ProcResV30;
    };

} RkAiqAdehazeProcResult_t;

#endif

