/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @version V0.0.1
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-07-07     ISP Team      first implementation
  *
  ******************************************************************************
  */

#ifndef _UAPI_RKISP21_CONFIG_H
#define _UAPI_RKISP21_CONFIG_H

#include "rkisp2-config.h"

#define ISP2X_MODULE_BAYNR          BIT_ULL(36)
#define ISP2X_MODULE_BAY3D          BIT_ULL(37)
#define ISP2X_MODULE_YNR            BIT_ULL(38)
#define ISP2X_MODULE_CNR            BIT_ULL(39)
#define ISP2X_MODULE_SHARP          BIT_ULL(40)
#define ISP2X_MODULE_DRC            BIT_ULL(41)
#define ISP2X_MODULE_CSM            BIT_ULL(43)
#define ISP2X_MODULE_CGC            BIT_ULL(44)

#define ISP21_DHAZ_ENH_CURVE_NUM        17
#define ISP21_DHAZ_HIST_IIR_NUM         64
#define ISP21_RAWAWB_HSTBIN_NUM         8
#define ISP21_RAWAWB_WEIGHT_NUM         225
#define ISP21_DRC_Y_NUM             17
#define ISP21_YNR_XY_NUM            17
#define ISP21_BAYNR_XY_NUM          16
#define ISP21_BAY3D_XY_NUM          16
#define ISP21_SHARP_X_NUM           7
#define ISP21_SHARP_Y_NUM           8
#define ISP21_CSM_COEFF_NUM         9

struct isp21_cgc_cfg
{
    uint8_t yuv_limit;
    uint8_t ratio_en;
} __attribute__((packed));

struct isp21_csm_cfg
{
    uint8_t csm_full_range;
    uint16_t csm_y_offset;
    uint16_t csm_c_offset;

    uint32_t csm_coeff[ISP21_CSM_COEFF_NUM];
} __attribute__((packed));

struct isp21_bls_cfg
{
    uint8_t enable_auto;
    uint8_t en_windows;
    uint8_t bls1_en;
    struct isp2x_window bls_window1;
    struct isp2x_window bls_window2;
    uint8_t bls_samples;
    struct isp2x_bls_fixed_val fixed_val;
    struct isp2x_bls_fixed_val bls1_val;
} __attribute__((packed));

struct isp21_awb_gain_cfg
{
    uint16_t gain0_red;
    uint16_t gain0_green_r;
    uint16_t gain0_blue;
    uint16_t gain0_green_b;
    uint16_t gain1_red;
    uint16_t gain1_green_r;
    uint16_t gain1_blue;
    uint16_t gain1_green_b;
    uint16_t gain2_red;
    uint16_t gain2_green_r;
    uint16_t gain2_blue;
    uint16_t gain2_green_b;
} __attribute__((packed));

struct isp21_gic_cfg
{
    uint16_t regmingradthrdark2;
    uint16_t regmingradthrdark1;
    uint16_t regminbusythre;

    uint16_t regdarkthre;
    uint16_t regmaxcorvboth;
    uint16_t regdarktthrehi;

    uint8_t regkgrad2dark;
    uint8_t regkgrad1dark;
    uint8_t regstrengthglobal_fix;
    uint8_t regdarkthrestep;
    uint8_t regkgrad2;
    uint8_t regkgrad1;
    uint8_t reggbthre;

    uint16_t regmaxcorv;
    uint16_t regmingradthr2;
    uint16_t regmingradthr1;

    uint8_t gr_ratio;
    uint8_t noise_scale;
    uint16_t noise_base;
    uint16_t diff_clip;

    uint16_t sigma_y[ISP2X_GIC_SIGMA_Y_NUM];
} __attribute__((packed));

struct isp21_baynr_cfg
{
    uint8_t sw_baynr_gauss_en;
    uint8_t sw_baynr_log_bypass;
    uint16_t sw_baynr_dgain1;
    uint16_t sw_baynr_dgain0;
    uint16_t sw_baynr_dgain2;
    uint16_t sw_baynr_pix_diff;
    uint16_t sw_baynr_diff_thld;
    uint16_t sw_baynr_softthld;
    uint16_t sw_bltflt_streng;
    uint16_t sw_baynr_reg_w1;
    uint16_t sw_sigma_x[ISP21_BAYNR_XY_NUM];
    uint16_t sw_sigma_y[ISP21_BAYNR_XY_NUM];
    uint16_t weit_d2;
    uint16_t weit_d1;
    uint16_t weit_d0;
} __attribute__((packed));

struct isp21_bay3d_cfg
{
    uint8_t sw_bay3d_exp_sel;
    uint8_t sw_bay3d_bypass_en;
    uint8_t sw_bay3d_pk_en;
    uint16_t sw_bay3d_softwgt;
    uint16_t sw_bay3d_sigratio;
    uint32_t sw_bay3d_glbpk2;
    uint16_t sw_bay3d_exp_str;
    uint16_t sw_bay3d_str;
    uint16_t sw_bay3d_wgtlmt_h;
    uint16_t sw_bay3d_wgtlmt_l;
    uint16_t sw_bay3d_sig_x[ISP21_BAY3D_XY_NUM];
    uint16_t sw_bay3d_sig_y[ISP21_BAY3D_XY_NUM];
} __attribute__((packed));

struct isp21_ynr_cfg
{
    uint8_t sw_ynr_thumb_mix_cur_en;
    uint8_t sw_ynr_global_gain_alpha;
    uint8_t sw_ynr_global_gain;
    uint8_t sw_ynr_flt1x1_bypass_sel;
    uint8_t sw_ynr_sft5x5_bypass;
    uint8_t sw_ynr_flt1x1_bypass;
    uint8_t sw_ynr_lgft3x3_bypass;
    uint8_t sw_ynr_lbft5x5_bypass;
    uint8_t sw_ynr_bft3x3_bypass;

    uint16_t sw_ynr_rnr_max_r;

    uint16_t sw_ynr_low_bf_inv1;
    uint16_t sw_ynr_low_bf_inv0;

    uint16_t sw_ynr_low_peak_supress;
    uint16_t sw_ynr_low_thred_adj;

    uint16_t sw_ynr_low_dist_adj;
    uint16_t sw_ynr_low_edge_adj_thresh;

    uint16_t sw_ynr_low_bi_weight;
    uint16_t sw_ynr_low_weight;
    uint16_t sw_ynr_low_center_weight;
    uint16_t sw_ynr_hi_min_adj;
    uint16_t sw_ynr_high_thred_adj;
    uint8_t sw_ynr_high_retain_weight;
    uint8_t sw_ynr_hi_edge_thed;
    uint8_t sw_ynr_base_filter_weight2;
    uint8_t sw_ynr_base_filter_weight1;
    uint8_t sw_ynr_base_filter_weight0;
    uint16_t sw_ynr_low_gauss1_coeff2;
    uint16_t sw_ynr_low_gauss1_coeff1;
    uint16_t sw_ynr_low_gauss1_coeff0;
    uint16_t sw_ynr_low_gauss2_coeff2;
    uint16_t sw_ynr_low_gauss2_coeff1;
    uint16_t sw_ynr_low_gauss2_coeff0;
    uint8_t sw_ynr_direction_weight3;
    uint8_t sw_ynr_direction_weight2;
    uint8_t sw_ynr_direction_weight1;
    uint8_t sw_ynr_direction_weight0;
    uint8_t sw_ynr_direction_weight7;
    uint8_t sw_ynr_direction_weight6;
    uint8_t sw_ynr_direction_weight5;
    uint8_t sw_ynr_direction_weight4;
    uint16_t sw_ynr_luma_points_x[ISP21_YNR_XY_NUM];
    uint16_t sw_ynr_lsgm_y[ISP21_YNR_XY_NUM];
    uint16_t sw_ynr_hsgm_y[ISP21_YNR_XY_NUM];
    uint8_t sw_ynr_rnr_strength3[ISP21_YNR_XY_NUM];
} __attribute__((packed));

struct isp21_cnr_cfg
{
    uint8_t sw_cnr_thumb_mix_cur_en;
    uint8_t sw_cnr_lq_bila_bypass;
    uint8_t sw_cnr_hq_bila_bypass;
    uint8_t sw_cnr_exgain_bypass;
    uint8_t sw_cnr_exgain_mux;
    uint8_t sw_cnr_gain_iso;
    uint8_t sw_cnr_gain_offset;
    uint8_t sw_cnr_gain_1sigma;
    uint8_t sw_cnr_gain_uvgain1;
    uint8_t sw_cnr_gain_uvgain0;
    uint8_t sw_cnr_lmed3_alpha;
    uint8_t sw_cnr_lbf5_gain_y;
    uint8_t sw_cnr_lbf5_gain_c;
    uint8_t sw_cnr_lbf5_weit_d3;
    uint8_t sw_cnr_lbf5_weit_d2;
    uint8_t sw_cnr_lbf5_weit_d1;
    uint8_t sw_cnr_lbf5_weit_d0;
    uint8_t sw_cnr_lbf5_weit_d4;
    uint8_t sw_cnr_hmed3_alpha;
    uint16_t sw_cnr_hbf5_weit_src;
    uint16_t sw_cnr_hbf5_min_wgt;
    uint16_t sw_cnr_hbf5_sigma;
    uint16_t sw_cnr_lbf5_weit_src;
    uint16_t sw_cnr_lbf3_sigma;
} __attribute__((packed));

struct isp21_sharp_cfg
{
    uint8_t sw_sharp_bypass;
    uint8_t sw_sharp_sharp_ratio;
    uint8_t sw_sharp_bf_ratio;
    uint8_t sw_sharp_gaus_ratio;
    uint8_t sw_sharp_pbf_ratio;
    uint8_t sw_sharp_luma_dx[ISP21_SHARP_X_NUM];
    uint16_t sw_sharp_pbf_sigma_inv[ISP21_SHARP_Y_NUM];
    uint16_t sw_sharp_bf_sigma_inv[ISP21_SHARP_Y_NUM];
    uint8_t sw_sharp_bf_sigma_shift;
    uint8_t sw_sharp_pbf_sigma_shift;
    uint16_t sw_sharp_ehf_th[ISP21_SHARP_Y_NUM];
    uint16_t sw_sharp_clip_hf[ISP21_SHARP_Y_NUM];
    uint8_t sw_sharp_pbf_coef_2;
    uint8_t sw_sharp_pbf_coef_1;
    uint8_t sw_sharp_pbf_coef_0;
    uint8_t sw_sharp_bf_coef_2;
    uint8_t sw_sharp_bf_coef_1;
    uint8_t sw_sharp_bf_coef_0;
    uint8_t sw_sharp_gaus_coef_2;
    uint8_t sw_sharp_gaus_coef_1;
    uint8_t sw_sharp_gaus_coef_0;
} __attribute__((packed));

struct isp21_ccm_cfg
{
    uint8_t highy_adjust_dis;
    uint8_t bound_bit;

    int16_t coeff0_r;
    int16_t coeff1_r;
    int16_t coeff2_r;
    int16_t offset_r;

    int16_t coeff0_g;
    int16_t coeff1_g;
    int16_t coeff2_g;
    int16_t offset_g;

    int16_t coeff0_b;
    int16_t coeff1_b;
    int16_t coeff2_b;
    int16_t offset_b;

    uint16_t coeff0_y;
    uint16_t coeff1_y;
    uint16_t coeff2_y;

    uint16_t alp_y[ISP21_DHAZ_ENH_CURVE_NUM];
} __attribute__((packed));

struct isp21_dhaz_cfg
{
    uint8_t enhance_en;
    uint8_t air_lc_en;
    uint8_t hpara_en;
    uint8_t hist_en;
    uint8_t dc_en;

    uint8_t yblk_th;
    uint8_t yhist_th;
    uint8_t dc_max_th;
    uint8_t dc_min_th;

    uint16_t wt_max;
    uint8_t bright_max;
    uint8_t bright_min;

    uint8_t tmax_base;
    uint8_t dark_th;
    uint8_t air_max;
    uint8_t air_min;

    uint16_t tmax_max;
    uint16_t tmax_off;

    uint8_t hist_k;
    uint8_t hist_th_off;
    uint16_t hist_min;

    uint16_t hist_gratio;
    uint16_t hist_scale;

    uint16_t enhance_value;
    uint16_t enhance_chroma;

    uint16_t iir_wt_sigma;
    uint16_t iir_sigma;
    uint16_t stab_fnum;

    uint16_t iir_tmax_sigma;
    uint16_t iir_air_sigma;
    uint8_t iir_pre_wet;

    uint16_t cfg_wt;
    uint16_t cfg_air;
    uint16_t cfg_alpha;

    uint16_t cfg_gratio;
    uint16_t cfg_tmax;

    uint16_t range_sima;
    uint8_t space_sigma_pre;
    uint8_t space_sigma_cur;

    uint16_t dc_weitcur;
    uint16_t bf_weight;

    uint16_t enh_curve[ISP21_DHAZ_ENH_CURVE_NUM];

    uint8_t gaus_h2;
    uint8_t gaus_h1;
    uint8_t gaus_h0;
} __attribute__((packed));

struct isp21_dhaz_stat
{
    uint16_t dhaz_adp_air_base;
    uint16_t dhaz_adp_wt;

    uint16_t dhaz_adp_gratio;
    uint16_t dhaz_adp_tmax;

    uint16_t h_rgb_iir[ISP21_DHAZ_HIST_IIR_NUM];
} __attribute__((packed));

struct isp21_drc_cfg
{
    uint8_t sw_drc_offset_pow2;
    uint16_t sw_drc_compres_scl;
    uint16_t sw_drc_position;
    uint16_t sw_drc_delta_scalein;
    uint16_t sw_drc_hpdetail_ratio;
    uint16_t sw_drc_lpdetail_ratio;
    uint8_t sw_drc_weicur_pix;
    uint8_t sw_drc_weipre_frame;
    uint16_t sw_drc_force_sgm_inv0;
    uint8_t sw_drc_motion_scl;
    uint8_t sw_drc_edge_scl;
    uint16_t sw_drc_space_sgm_inv1;
    uint16_t sw_drc_space_sgm_inv0;
    uint16_t sw_drc_range_sgm_inv1;
    uint16_t sw_drc_range_sgm_inv0;
    uint8_t sw_drc_weig_maxl;
    uint8_t sw_drc_weig_bilat;
    uint16_t sw_drc_gain_y[ISP21_DRC_Y_NUM];
    uint16_t sw_drc_compres_y[ISP21_DRC_Y_NUM];
    uint16_t sw_drc_scale_y[ISP21_DRC_Y_NUM];
    uint16_t sw_drc_iir_weight;
    uint16_t sw_drc_min_ogain;
} __attribute__((packed));

struct isp21_rawawb_meas_cfg
{
    uint8_t rawawb_sel;
    uint8_t sw_rawawb_xy_en0;
    uint8_t sw_rawawb_uv_en0;
    uint8_t sw_rawawb_xy_en1;
    uint8_t sw_rawawb_uv_en1;
    uint8_t sw_rawawb_3dyuv_en0;
    uint8_t sw_rawawb_3dyuv_en1;
    uint8_t sw_rawawb_wp_blk_wei_en0;
    uint8_t sw_rawawb_wp_blk_wei_en1;
    uint8_t sw_rawawb_wp_luma_wei_en0;
    uint8_t sw_rawawb_wp_luma_wei_en1;
    uint8_t sw_rawlsc_bypass_en;
    uint8_t sw_rawawb_blk_measure_enable;
    uint8_t sw_rawawb_blk_measure_mode;
    uint8_t sw_rawawb_blk_measure_xytype;
    uint8_t sw_rawawb_blk_measure_illu_idx;
    uint8_t sw_rawawb_wp_hist_xytype;
    uint8_t sw_rawawb_light_num;
    uint8_t sw_rawawb_wind_size;
    uint8_t sw_rawawb_r_max;
    uint8_t sw_rawawb_g_max;
    uint8_t sw_rawawb_b_max;
    uint8_t sw_rawawb_y_max;
    uint8_t sw_rawawb_r_min;
    uint8_t sw_rawawb_g_min;
    uint8_t sw_rawawb_b_min;
    uint8_t sw_rawawb_y_min;
    uint8_t sw_rawawb_3dyuv_ls_idx0;
    uint8_t sw_rawawb_3dyuv_ls_idx1;
    uint8_t sw_rawawb_3dyuv_ls_idx2;
    uint8_t sw_rawawb_3dyuv_ls_idx3;
    uint8_t sw_rawawb_exc_wp_region0_excen0;
    uint8_t sw_rawawb_exc_wp_region0_excen1;
    uint8_t sw_rawawb_exc_wp_region0_domain;
    uint8_t sw_rawawb_exc_wp_region1_excen0;
    uint8_t sw_rawawb_exc_wp_region1_excen1;
    uint8_t sw_rawawb_exc_wp_region1_domain;
    uint8_t sw_rawawb_exc_wp_region2_excen0;
    uint8_t sw_rawawb_exc_wp_region2_excen1;
    uint8_t sw_rawawb_exc_wp_region2_domain;
    uint8_t sw_rawawb_exc_wp_region3_excen0;
    uint8_t sw_rawawb_exc_wp_region3_excen1;
    uint8_t sw_rawawb_exc_wp_region3_domain;
    uint8_t sw_rawawb_exc_wp_region4_excen0;
    uint8_t sw_rawawb_exc_wp_region4_excen1;
    uint8_t sw_rawawb_exc_wp_region4_domain;
    uint8_t sw_rawawb_exc_wp_region5_excen0;
    uint8_t sw_rawawb_exc_wp_region5_excen1;
    uint8_t sw_rawawb_exc_wp_region5_domain;
    uint8_t sw_rawawb_exc_wp_region6_excen0;
    uint8_t sw_rawawb_exc_wp_region6_excen1;
    uint8_t sw_rawawb_exc_wp_region6_domain;
    uint8_t sw_rawawb_wp_luma_weicurve_y0;
    uint8_t sw_rawawb_wp_luma_weicurve_y1;
    uint8_t sw_rawawb_wp_luma_weicurve_y2;
    uint8_t sw_rawawb_wp_luma_weicurve_y3;
    uint8_t sw_rawawb_wp_luma_weicurve_y4;
    uint8_t sw_rawawb_wp_luma_weicurve_y5;
    uint8_t sw_rawawb_wp_luma_weicurve_y6;
    uint8_t sw_rawawb_wp_luma_weicurve_y7;
    uint8_t sw_rawawb_wp_luma_weicurve_y8;
    uint8_t sw_rawawb_wp_luma_weicurve_w0;
    uint8_t sw_rawawb_wp_luma_weicurve_w1;
    uint8_t sw_rawawb_wp_luma_weicurve_w2;
    uint8_t sw_rawawb_wp_luma_weicurve_w3;
    uint8_t sw_rawawb_wp_luma_weicurve_w4;
    uint8_t sw_rawawb_wp_luma_weicurve_w5;
    uint8_t sw_rawawb_wp_luma_weicurve_w6;
    uint8_t sw_rawawb_wp_luma_weicurve_w7;
    uint8_t sw_rawawb_wp_luma_weicurve_w8;
    uint8_t sw_rawawb_rotu0_ls0;
    uint8_t sw_rawawb_rotu1_ls0;
    uint8_t sw_rawawb_rotu2_ls0;
    uint8_t sw_rawawb_rotu3_ls0;
    uint8_t sw_rawawb_rotu4_ls0;
    uint8_t sw_rawawb_rotu5_ls0;
    uint8_t sw_rawawb_dis_x1x2_ls0;
    uint8_t sw_rawawb_rotu0_ls1;
    uint8_t sw_rawawb_rotu1_ls1;
    uint8_t sw_rawawb_rotu2_ls1;
    uint8_t sw_rawawb_rotu3_ls1;
    uint8_t sw_rawawb_rotu4_ls1;
    uint8_t sw_rawawb_rotu5_ls1;
    uint8_t sw_rawawb_dis_x1x2_ls1;
    uint8_t sw_rawawb_rotu0_ls2;
    uint8_t sw_rawawb_rotu1_ls2;
    uint8_t sw_rawawb_rotu2_ls2;
    uint8_t sw_rawawb_rotu3_ls2;
    uint8_t sw_rawawb_rotu4_ls2;
    uint8_t sw_rawawb_rotu5_ls2;
    uint8_t sw_rawawb_dis_x1x2_ls2;
    uint8_t sw_rawawb_rotu0_ls3;
    uint8_t sw_rawawb_rotu1_ls3;
    uint8_t sw_rawawb_rotu2_ls3;
    uint8_t sw_rawawb_rotu3_ls3;
    uint8_t sw_rawawb_rotu4_ls3;
    uint8_t sw_rawawb_rotu5_ls3;
    uint8_t sw_rawawb_dis_x1x2_ls3;
    uint8_t sw_rawawb_blk_rtdw_measure_en;
    uint8_t sw_rawawb_blk_with_luma_wei_en;
    uint8_t sw_rawawb_wp_blk_wei_w[ISP21_RAWAWB_WEIGHT_NUM];

    uint16_t sw_rawawb_h_offs;
    uint16_t sw_rawawb_v_offs;
    uint16_t sw_rawawb_h_size;
    uint16_t sw_rawawb_v_size;
    uint16_t sw_rawawb_vertex0_u_0;
    uint16_t sw_rawawb_vertex0_v_0;
    uint16_t sw_rawawb_vertex1_u_0;
    uint16_t sw_rawawb_vertex1_v_0;
    uint16_t sw_rawawb_vertex2_u_0;
    uint16_t sw_rawawb_vertex2_v_0;
    uint16_t sw_rawawb_vertex3_u_0;
    uint16_t sw_rawawb_vertex3_v_0;
    uint16_t sw_rawawb_vertex0_u_1;
    uint16_t sw_rawawb_vertex0_v_1;
    uint16_t sw_rawawb_vertex1_u_1;
    uint16_t sw_rawawb_vertex1_v_1;
    uint16_t sw_rawawb_vertex2_u_1;
    uint16_t sw_rawawb_vertex2_v_1;
    uint16_t sw_rawawb_vertex3_u_1;
    uint16_t sw_rawawb_vertex3_v_1;
    uint16_t sw_rawawb_vertex0_u_2;
    uint16_t sw_rawawb_vertex0_v_2;
    uint16_t sw_rawawb_vertex1_u_2;
    uint16_t sw_rawawb_vertex1_v_2;
    uint16_t sw_rawawb_vertex2_u_2;
    uint16_t sw_rawawb_vertex2_v_2;
    uint16_t sw_rawawb_vertex3_u_2;
    uint16_t sw_rawawb_vertex3_v_2;
    uint16_t sw_rawawb_vertex0_u_3;
    uint16_t sw_rawawb_vertex0_v_3;
    uint16_t sw_rawawb_vertex1_u_3;
    uint16_t sw_rawawb_vertex1_v_3;
    uint16_t sw_rawawb_vertex2_u_3;
    uint16_t sw_rawawb_vertex2_v_3;
    uint16_t sw_rawawb_vertex3_u_3;
    uint16_t sw_rawawb_vertex3_v_3;
    uint16_t sw_rawawb_vertex0_u_4;
    uint16_t sw_rawawb_vertex0_v_4;
    uint16_t sw_rawawb_vertex1_u_4;
    uint16_t sw_rawawb_vertex1_v_4;
    uint16_t sw_rawawb_vertex2_u_4;
    uint16_t sw_rawawb_vertex2_v_4;
    uint16_t sw_rawawb_vertex3_u_4;
    uint16_t sw_rawawb_vertex3_v_4;
    uint16_t sw_rawawb_vertex0_u_5;
    uint16_t sw_rawawb_vertex0_v_5;
    uint16_t sw_rawawb_vertex1_u_5;
    uint16_t sw_rawawb_vertex1_v_5;
    uint16_t sw_rawawb_vertex2_u_5;
    uint16_t sw_rawawb_vertex2_v_5;
    uint16_t sw_rawawb_vertex3_u_5;
    uint16_t sw_rawawb_vertex3_v_5;
    uint16_t sw_rawawb_vertex0_u_6;
    uint16_t sw_rawawb_vertex0_v_6;
    uint16_t sw_rawawb_vertex1_u_6;
    uint16_t sw_rawawb_vertex1_v_6;
    uint16_t sw_rawawb_vertex2_u_6;
    uint16_t sw_rawawb_vertex2_v_6;
    uint16_t sw_rawawb_vertex3_u_6;
    uint16_t sw_rawawb_vertex3_v_6;

    uint16_t sw_rawawb_wt0;
    uint16_t sw_rawawb_wt1;
    uint16_t sw_rawawb_wt2;
    uint16_t sw_rawawb_mat0_x;
    uint16_t sw_rawawb_mat1_x;
    uint16_t sw_rawawb_mat2_x;
    uint16_t sw_rawawb_mat0_y;
    uint16_t sw_rawawb_mat1_y;
    uint16_t sw_rawawb_mat2_y;
    uint16_t sw_rawawb_nor_x0_0;
    uint16_t sw_rawawb_nor_x1_0;
    uint16_t sw_rawawb_nor_y0_0;
    uint16_t sw_rawawb_nor_y1_0;
    uint16_t sw_rawawb_big_x0_0;
    uint16_t sw_rawawb_big_x1_0;
    uint16_t sw_rawawb_big_y0_0;
    uint16_t sw_rawawb_big_y1_0;
    uint16_t sw_rawawb_nor_x0_1;
    uint16_t sw_rawawb_nor_x1_1;
    uint16_t sw_rawawb_nor_y0_1;
    uint16_t sw_rawawb_nor_y1_1;
    uint16_t sw_rawawb_big_x0_1;
    uint16_t sw_rawawb_big_x1_1;
    uint16_t sw_rawawb_big_y0_1;
    uint16_t sw_rawawb_big_y1_1;
    uint16_t sw_rawawb_nor_x0_2;
    uint16_t sw_rawawb_nor_x1_2;
    uint16_t sw_rawawb_nor_y0_2;
    uint16_t sw_rawawb_nor_y1_2;
    uint16_t sw_rawawb_big_x0_2;
    uint16_t sw_rawawb_big_x1_2;
    uint16_t sw_rawawb_big_y0_2;
    uint16_t sw_rawawb_big_y1_2;
    uint16_t sw_rawawb_nor_x0_3;
    uint16_t sw_rawawb_nor_x1_3;
    uint16_t sw_rawawb_nor_y0_3;
    uint16_t sw_rawawb_nor_y1_3;
    uint16_t sw_rawawb_big_x0_3;
    uint16_t sw_rawawb_big_x1_3;
    uint16_t sw_rawawb_big_y0_3;
    uint16_t sw_rawawb_big_y1_3;
    uint16_t sw_rawawb_nor_x0_4;
    uint16_t sw_rawawb_nor_x1_4;
    uint16_t sw_rawawb_nor_y0_4;
    uint16_t sw_rawawb_nor_y1_4;
    uint16_t sw_rawawb_big_x0_4;
    uint16_t sw_rawawb_big_x1_4;
    uint16_t sw_rawawb_big_y0_4;
    uint16_t sw_rawawb_big_y1_4;
    uint16_t sw_rawawb_nor_x0_5;
    uint16_t sw_rawawb_nor_x1_5;
    uint16_t sw_rawawb_nor_y0_5;
    uint16_t sw_rawawb_nor_y1_5;
    uint16_t sw_rawawb_big_x0_5;
    uint16_t sw_rawawb_big_x1_5;
    uint16_t sw_rawawb_big_y0_5;
    uint16_t sw_rawawb_big_y1_5;
    uint16_t sw_rawawb_nor_x0_6;
    uint16_t sw_rawawb_nor_x1_6;
    uint16_t sw_rawawb_nor_y0_6;
    uint16_t sw_rawawb_nor_y1_6;
    uint16_t sw_rawawb_big_x0_6;
    uint16_t sw_rawawb_big_x1_6;
    uint16_t sw_rawawb_big_y0_6;
    uint16_t sw_rawawb_big_y1_6;
    uint16_t sw_rawawb_pre_wbgain_inv_r;
    uint16_t sw_rawawb_pre_wbgain_inv_g;
    uint16_t sw_rawawb_pre_wbgain_inv_b;
    uint16_t sw_rawawb_exc_wp_region0_xu0;
    uint16_t sw_rawawb_exc_wp_region0_xu1;
    uint16_t sw_rawawb_exc_wp_region0_yv0;
    uint16_t sw_rawawb_exc_wp_region0_yv1;
    uint16_t sw_rawawb_exc_wp_region1_xu0;
    uint16_t sw_rawawb_exc_wp_region1_xu1;
    uint16_t sw_rawawb_exc_wp_region1_yv0;
    uint16_t sw_rawawb_exc_wp_region1_yv1;
    uint16_t sw_rawawb_exc_wp_region2_xu0;
    uint16_t sw_rawawb_exc_wp_region2_xu1;
    uint16_t sw_rawawb_exc_wp_region2_yv0;
    uint16_t sw_rawawb_exc_wp_region2_yv1;
    uint16_t sw_rawawb_exc_wp_region3_xu0;
    uint16_t sw_rawawb_exc_wp_region3_xu1;
    uint16_t sw_rawawb_exc_wp_region3_yv0;
    uint16_t sw_rawawb_exc_wp_region3_yv1;
    uint16_t sw_rawawb_exc_wp_region4_xu0;
    uint16_t sw_rawawb_exc_wp_region4_xu1;
    uint16_t sw_rawawb_exc_wp_region4_yv0;
    uint16_t sw_rawawb_exc_wp_region4_yv1;
    uint16_t sw_rawawb_exc_wp_region5_xu0;
    uint16_t sw_rawawb_exc_wp_region5_xu1;
    uint16_t sw_rawawb_exc_wp_region5_yv0;
    uint16_t sw_rawawb_exc_wp_region5_yv1;
    uint16_t sw_rawawb_exc_wp_region6_xu0;
    uint16_t sw_rawawb_exc_wp_region6_xu1;
    uint16_t sw_rawawb_exc_wp_region6_yv0;
    uint16_t sw_rawawb_exc_wp_region6_yv1;
    uint16_t sw_rawawb_rgb2ryuvmat0_u;
    uint16_t sw_rawawb_rgb2ryuvmat1_u;
    uint16_t sw_rawawb_rgb2ryuvmat2_u;
    uint16_t sw_rawawb_rgb2ryuvofs_u;
    uint16_t sw_rawawb_rgb2ryuvmat0_v;
    uint16_t sw_rawawb_rgb2ryuvmat1_v;
    uint16_t sw_rawawb_rgb2ryuvmat2_v;
    uint16_t sw_rawawb_rgb2ryuvofs_v;
    uint16_t sw_rawawb_rgb2ryuvmat0_y;
    uint16_t sw_rawawb_rgb2ryuvmat1_y;
    uint16_t sw_rawawb_rgb2ryuvmat2_y;
    uint16_t sw_rawawb_rgb2ryuvofs_y;
    uint16_t sw_rawawb_th0_ls0;
    uint16_t sw_rawawb_th1_ls0;
    uint16_t sw_rawawb_th2_ls0;
    uint16_t sw_rawawb_th3_ls0;
    uint16_t sw_rawawb_th4_ls0;
    uint16_t sw_rawawb_th5_ls0;
    uint16_t sw_rawawb_coor_x1_ls0_u;
    uint16_t sw_rawawb_coor_x1_ls0_v;
    uint16_t sw_rawawb_coor_x1_ls0_y;
    uint16_t sw_rawawb_vec_x21_ls0_u;
    uint16_t sw_rawawb_vec_x21_ls0_v;
    uint16_t sw_rawawb_vec_x21_ls0_y;
    uint16_t sw_rawawb_th0_ls1;
    uint16_t sw_rawawb_th1_ls1;
    uint16_t sw_rawawb_th2_ls1;
    uint16_t sw_rawawb_th3_ls1;
    uint16_t sw_rawawb_th4_ls1;
    uint16_t sw_rawawb_th5_ls1;
    uint16_t sw_rawawb_coor_x1_ls1_u;
    uint16_t sw_rawawb_coor_x1_ls1_v;
    uint16_t sw_rawawb_coor_x1_ls1_y;
    uint16_t sw_rawawb_vec_x21_ls1_u;
    uint16_t sw_rawawb_vec_x21_ls1_v;
    uint16_t sw_rawawb_vec_x21_ls1_y;
    uint16_t sw_rawawb_th0_ls2;
    uint16_t sw_rawawb_th1_ls2;
    uint16_t sw_rawawb_th2_ls2;
    uint16_t sw_rawawb_th3_ls2;
    uint16_t sw_rawawb_th4_ls2;
    uint16_t sw_rawawb_th5_ls2;
    uint16_t sw_rawawb_coor_x1_ls2_u;
    uint16_t sw_rawawb_coor_x1_ls2_v;
    uint16_t sw_rawawb_coor_x1_ls2_y;
    uint16_t sw_rawawb_vec_x21_ls2_u;
    uint16_t sw_rawawb_vec_x21_ls2_v;
    uint16_t sw_rawawb_vec_x21_ls2_y;
    uint16_t sw_rawawb_th0_ls3;
    uint16_t sw_rawawb_th1_ls3;
    uint16_t sw_rawawb_th2_ls3;
    uint16_t sw_rawawb_th3_ls3;
    uint16_t sw_rawawb_th4_ls3;
    uint16_t sw_rawawb_th5_ls3;
    uint16_t sw_rawawb_coor_x1_ls3_u;
    uint16_t sw_rawawb_coor_x1_ls3_v;
    uint16_t sw_rawawb_coor_x1_ls3_y;
    uint16_t sw_rawawb_vec_x21_ls3_u;
    uint16_t sw_rawawb_vec_x21_ls3_v;
    uint16_t sw_rawawb_vec_x21_ls3_y;

    uint32_t sw_rawawb_islope01_0;
    uint32_t sw_rawawb_islope12_0;
    uint32_t sw_rawawb_islope23_0;
    uint32_t sw_rawawb_islope30_0;
    uint32_t sw_rawawb_islope01_1;
    uint32_t sw_rawawb_islope12_1;
    uint32_t sw_rawawb_islope23_1;
    uint32_t sw_rawawb_islope30_1;
    uint32_t sw_rawawb_islope01_2;
    uint32_t sw_rawawb_islope12_2;
    uint32_t sw_rawawb_islope23_2;
    uint32_t sw_rawawb_islope30_2;
    uint32_t sw_rawawb_islope01_3;
    uint32_t sw_rawawb_islope12_3;
    uint32_t sw_rawawb_islope23_3;
    uint32_t sw_rawawb_islope30_3;
    uint32_t sw_rawawb_islope01_4;
    uint32_t sw_rawawb_islope12_4;
    uint32_t sw_rawawb_islope23_4;
    uint32_t sw_rawawb_islope30_4;
    uint32_t sw_rawawb_islope01_5;
    uint32_t sw_rawawb_islope12_5;
    uint32_t sw_rawawb_islope23_5;
    uint32_t sw_rawawb_islope30_5;
    uint32_t sw_rawawb_islope01_6;
    uint32_t sw_rawawb_islope12_6;
    uint32_t sw_rawawb_islope23_6;
    uint32_t sw_rawawb_islope30_6;
} __attribute__((packed));

struct isp21_isp_other_cfg
{
    struct isp21_bls_cfg bls_cfg;
    struct isp2x_dpcc_cfg dpcc_cfg;
    struct isp2x_lsc_cfg lsc_cfg;
    struct isp21_awb_gain_cfg awb_gain_cfg;
    struct isp21_gic_cfg gic_cfg;
    struct isp2x_debayer_cfg debayer_cfg;
    struct isp21_ccm_cfg ccm_cfg;
    struct isp2x_gammaout_cfg gammaout_cfg;
    struct isp2x_cproc_cfg cproc_cfg;
    struct isp2x_ie_cfg ie_cfg;
    struct isp2x_sdg_cfg sdg_cfg;
    struct isp21_drc_cfg drc_cfg;
    struct isp2x_hdrmge_cfg hdrmge_cfg;
    struct isp21_dhaz_cfg dhaz_cfg;
    struct isp2x_3dlut_cfg isp3dlut_cfg;
    struct isp2x_ldch_cfg ldch_cfg;
    struct isp21_baynr_cfg baynr_cfg;
    struct isp21_bay3d_cfg bay3d_cfg;
    struct isp21_ynr_cfg ynr_cfg;
    struct isp21_cnr_cfg cnr_cfg;
    struct isp21_sharp_cfg sharp_cfg;
    struct isp21_csm_cfg csm_cfg;
    struct isp21_cgc_cfg cgc_cfg;
} __attribute__((packed));

struct isp21_isp_meas_cfg
{
    struct isp2x_siawb_meas_cfg siawb;
    struct isp21_rawawb_meas_cfg rawawb;
    struct isp2x_rawaelite_meas_cfg rawae0;
    struct isp2x_rawaebig_meas_cfg rawae1;
    struct isp2x_rawaebig_meas_cfg rawae2;
    struct isp2x_rawaebig_meas_cfg rawae3;
    struct isp2x_yuvae_meas_cfg yuvae;
    struct isp2x_rawaf_meas_cfg rawaf;
    struct isp2x_siaf_cfg siaf;
    struct isp2x_rawhistlite_cfg rawhist0;
    struct isp2x_rawhistbig_cfg rawhist1;
    struct isp2x_rawhistbig_cfg rawhist2;
    struct isp2x_rawhistbig_cfg rawhist3;
    struct isp2x_sihst_cfg sihst;
} __attribute__((packed));

struct isp21_isp_params_cfg
{
    uint64_t module_en_update;
    uint64_t module_ens;
    uint64_t module_cfg_update;

    uint32_t frame_id;
    struct isp21_isp_meas_cfg meas;
    struct isp21_isp_other_cfg others;
} __attribute__((packed));

struct isp21_rawawb_meas_stat
{
    uint16_t ro_yhist_bin[ISP21_RAWAWB_HSTBIN_NUM];
    uint32_t ro_rawawb_sum_rgain_nor[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_rawawb_sum_bgain_nor[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_rawawb_wp_num_nor[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_rawawb_sum_rgain_big[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_rawawb_sum_bgain_big[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_rawawb_wp_num_big[ISP2X_RAWAWB_SUM_NUM];
    struct isp2x_rawawb_ramdata ramdata[ISP2X_RAWAWB_RAMDATA_NUM];
} __attribute__((packed));

struct isp21_stat
{
    struct isp2x_siawb_stat siawb;
    struct isp21_rawawb_meas_stat rawawb;
    struct isp2x_rawaelite_stat rawae0;
    struct isp2x_rawaebig_stat rawae1;
    struct isp2x_rawaebig_stat rawae2;
    struct isp2x_rawaebig_stat rawae3;
    struct isp2x_yuvae_stat yuvae;
    struct isp2x_rawaf_stat rawaf;
    struct isp2x_siaf_stat siaf;
    struct isp2x_rawhistlite_stat rawhist0;
    struct isp2x_rawhistbig_stat rawhist1;
    struct isp2x_rawhistbig_stat rawhist2;
    struct isp2x_rawhistbig_stat rawhist3;
    struct isp2x_sihst_stat sihst;

    struct isp2x_bls_stat bls;
    struct isp21_dhaz_stat dhaz;
} __attribute__((packed));

/**
 * struct rkisp_isp21_stat_buffer - Rockchip ISP2 Statistics Meta Data
 *
 * @meas_type: measurement types (CIFISP_STAT_ definitions)
 * @frame_id: frame ID for sync
 * @params: statistics data
 */
struct rkisp_isp21_stat_buffer
{
    unsigned int meas_type;
    unsigned int frame_id;
    struct isp21_stat params;
} __attribute__((packed));

#endif /* _UAPI_RKISP21_CONFIG_H */
