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

#ifndef _UAPI_RKISP32_CONFIG_H
#define _UAPI_RKISP32_CONFIG_H

#include "rkisp3-config.h"

#define ISP32_MODULE_DPCC       ISP3X_MODULE_DPCC
#define ISP32_MODULE_BLS        ISP3X_MODULE_BLS
#define ISP32_MODULE_SDG        ISP3X_MODULE_SDG
#define ISP32_MODULE_LSC        ISP3X_MODULE_LSC
#define ISP32_MODULE_AWB_GAIN       ISP3X_MODULE_AWB_GAIN
#define ISP32_MODULE_BDM        ISP3X_MODULE_BDM
#define ISP32_MODULE_CCM        ISP3X_MODULE_CCM
#define ISP32_MODULE_GOC        ISP3X_MODULE_GOC
#define ISP32_MODULE_CPROC      ISP3X_MODULE_CPROC
#define ISP32_MODULE_IE         ISP3X_MODULE_IE
#define ISP32_MODULE_RAWAF      ISP3X_MODULE_RAWAF
#define ISP32_MODULE_RAWAE0     ISP3X_MODULE_RAWAE0
#define ISP32_MODULE_RAWAE1     ISP3X_MODULE_RAWAE1
#define ISP32_MODULE_RAWAE2     ISP3X_MODULE_RAWAE2
#define ISP32_MODULE_RAWAE3     ISP3X_MODULE_RAWAE3
#define ISP32_MODULE_RAWAWB     ISP3X_MODULE_RAWAWB
#define ISP32_MODULE_RAWHIST0       ISP3X_MODULE_RAWHIST0
#define ISP32_MODULE_RAWHIST1       ISP3X_MODULE_RAWHIST1
#define ISP32_MODULE_RAWHIST2       ISP3X_MODULE_RAWHIST2
#define ISP32_MODULE_RAWHIST3       ISP3X_MODULE_RAWHIST3
#define ISP32_MODULE_HDRMGE     ISP3X_MODULE_HDRMGE
#define ISP32_MODULE_RAWNR      ISP3X_MODULE_RAWNR
#define ISP32_MODULE_GIC        ISP3X_MODULE_GIC
#define ISP32_MODULE_DHAZ       ISP3X_MODULE_DHAZ
#define ISP32_MODULE_3DLUT      ISP3X_MODULE_3DLUT
#define ISP32_MODULE_LDCH       ISP3X_MODULE_LDCH
#define ISP32_MODULE_GAIN       ISP3X_MODULE_GAIN
#define ISP32_MODULE_DEBAYER        ISP3X_MODULE_DEBAYER
#define ISP32_MODULE_BAYNR      ISP3X_MODULE_BAYNR
#define ISP32_MODULE_BAY3D      ISP3X_MODULE_BAY3D
#define ISP32_MODULE_YNR        ISP3X_MODULE_YNR
#define ISP32_MODULE_CNR        ISP3X_MODULE_CNR
#define ISP32_MODULE_SHARP      ISP3X_MODULE_SHARP
#define ISP32_MODULE_DRC        ISP3X_MODULE_DRC
#define ISP32_MODULE_CAC        ISP3X_MODULE_CAC
#define ISP32_MODULE_CSM        ISP3X_MODULE_CSM
#define ISP32_MODULE_CGC        ISP3X_MODULE_CGC
#define ISP32_MODULE_VSM        BIT_ULL(45)

#define ISP32_MODULE_FORCE      ISP3X_MODULE_FORCE

/* Measurement types */
#define ISP32_STAT_RAWAWB       ISP3X_STAT_RAWAWB
#define ISP32_STAT_RAWAF        ISP3X_STAT_RAWAF
#define ISP32_STAT_RAWAE0       ISP3X_STAT_RAWAE0
#define ISP32_STAT_RAWAE1       ISP3X_STAT_RAWAE1
#define ISP32_STAT_RAWAE2       ISP3X_STAT_RAWAE2
#define ISP32_STAT_RAWAE3       ISP3X_STAT_RAWAE3
#define ISP32_STAT_RAWHST0      ISP3X_STAT_RAWHST0
#define ISP32_STAT_RAWHST1      ISP3X_STAT_RAWHST1
#define ISP32_STAT_RAWHST2      ISP3X_STAT_RAWHST2
#define ISP32_STAT_RAWHST3      ISP3X_STAT_RAWHST3
#define ISP32_STAT_BLS          ISP3X_STAT_BLS
#define ISP32_STAT_DHAZ         ISP3X_STAT_DHAZ
#define ISP32_STAT_VSM          BIT(18)
#define ISP32_STAT_INFO2DDR     BIT(19)

#define ISP32_MESH_BUF_NUM      ISP3X_MESH_BUF_NUM

#define ISP32_LSC_GRAD_TBL_SIZE     ISP3X_LSC_GRAD_TBL_SIZE
#define ISP32_LSC_SIZE_TBL_SIZE     ISP3X_LSC_SIZE_TBL_SIZE
#define ISP32_LSC_DATA_TBL_SIZE     ISP3X_LSC_DATA_TBL_SIZE

#define ISP32_DEGAMMA_CURVE_SIZE    ISP3X_DEGAMMA_CURVE_SIZE

#define ISP32_GAIN_IDX_NUM      ISP3X_GAIN_IDX_NUM
#define ISP32_GAIN_LUT_NUM      ISP3X_GAIN_LUT_NUM

#define ISP32_RAWAWB_EXCL_STAT_NUM  ISP3X_RAWAWB_EXCL_STAT_NUM
#define ISP32_RAWAWB_HSTBIN_NUM     ISP3X_RAWAWB_HSTBIN_NUM
#define ISP32_RAWAWB_WEIGHT_NUM     ISP3X_RAWAWB_WEIGHT_NUM
#define ISP32_RAWAWB_SUM_NUM        4
#define ISP32_RAWAWB_RAMDATA_NUM    ISP3X_RAWAWB_RAMDATA_NUM
#define ISP32L_RAWAWB_WEIGHT_NUM	5
#define ISP32L_RAWAWB_RAMDATA_RGB_NUM	25
#define ISP32L_RAWAWB_RAMDATA_WP_NUM	13

#define ISP32_RAWAEBIG_SUBWIN_NUM   ISP3X_RAWAEBIG_SUBWIN_NUM
#define ISP32_RAWAEBIG_MEAN_NUM     ISP3X_RAWAEBIG_MEAN_NUM
#define ISP32_RAWAELITE_MEAN_NUM    ISP3X_RAWAELITE_MEAN_NUM

#define ISP32_RAWHISTBIG_SUBWIN_NUM ISP3X_RAWHISTBIG_SUBWIN_NUM
#define ISP32_RAWHISTLITE_SUBWIN_NUM    ISP3X_RAWHISTLITE_SUBWIN_NUM
#define ISP32_HIST_BIN_N_MAX        ISP3X_HIST_BIN_N_MAX
#define ISP32L_HIST_LITE_BIN_N_MAX	64

#define ISP32_RAWAF_CURVE_NUM       ISP3X_RAWAF_CURVE_NUM
#define ISP32_RAWAF_HIIR_COE_NUM    ISP3X_RAWAF_HIIR_COE_NUM
#define ISP32_RAWAF_VFIR_COE_NUM    ISP3X_RAWAF_VFIR_COE_NUM
#define ISP32_RAWAF_WIN_NUM     ISP3X_RAWAF_WIN_NUM
#define ISP32_RAWAF_LINE_NUM        ISP3X_RAWAF_LINE_NUM
#define ISP32_RAWAF_GAMMA_NUM       ISP3X_RAWAF_GAMMA_NUM
#define ISP32_RAWAF_SUMDATA_NUM     ISP3X_RAWAF_SUMDATA_NUM
#define ISP32_RAWAF_VIIR_COE_NUM    3
#define ISP32_RAWAF_GAUS_COE_NUM    9
#define ISP32L_RAWAF_WND_DATA		25

#define ISP32_DPCC_PDAF_POINT_NUM   ISP3X_DPCC_PDAF_POINT_NUM

#define ISP32_HDRMGE_L_CURVE_NUM    ISP3X_HDRMGE_L_CURVE_NUM
#define ISP32_HDRMGE_E_CURVE_NUM    ISP3X_HDRMGE_E_CURVE_NUM

#define ISP32_GIC_SIGMA_Y_NUM       ISP3X_GIC_SIGMA_Y_NUM

#define ISP32_CCM_CURVE_NUM     18

#define ISP32_3DLUT_DATA_NUM        ISP3X_3DLUT_DATA_NUM

#define ISP32_LDCH_MESH_XY_NUM      ISP3X_LDCH_MESH_XY_NUM
#define ISP32_LDCH_BIC_NUM      36

#define ISP32_GAMMA_OUT_MAX_SAMPLES     ISP3X_GAMMA_OUT_MAX_SAMPLES

#define ISP32_DHAZ_SIGMA_IDX_NUM    ISP3X_DHAZ_SIGMA_IDX_NUM
#define ISP32_DHAZ_SIGMA_LUT_NUM    ISP3X_DHAZ_SIGMA_LUT_NUM
#define ISP32_DHAZ_HIST_WR_NUM      ISP3X_DHAZ_HIST_WR_NUM
#define ISP32_DHAZ_ENH_CURVE_NUM    ISP3X_DHAZ_ENH_CURVE_NUM
#define ISP32_DHAZ_HIST_IIR_NUM     ISP3X_DHAZ_HIST_IIR_NUM
#define ISP32_DHAZ_ENH_LUMA_NUM     17

#define ISP32_DRC_Y_NUM         ISP3X_DRC_Y_NUM

#define ISP32_CNR_SIGMA_Y_NUM       ISP3X_CNR_SIGMA_Y_NUM
#define ISP32_CNR_GAUS_COE_NUM      6

#define ISP32_YNR_XY_NUM        ISP3X_YNR_XY_NUM
#define ISP32_YNR_NLM_COE_NUM       6

#define ISP32_BAYNR_XY_NUM      ISP3X_BAYNR_XY_NUM
#define ISP32_BAYNR_GAIN_NUM        16

#define ISP32_BAY3D_XY_NUM      ISP3X_BAY3D_XY_NUM

#define ISP32_SHARP_X_NUM       ISP3X_SHARP_X_NUM
#define ISP32_SHARP_Y_NUM       ISP3X_SHARP_Y_NUM
#define ISP32_SHARP_GAUS_COEF_NUM   ISP3X_SHARP_GAUS_COEF_NUM
#define ISP32_SHARP_GAIN_ADJ_NUM    14
#define ISP32_SHARP_STRENGTH_NUM    22

#define ISP32_CAC_STRENGTH_NUM      ISP3X_CAC_STRENGTH_NUM

#define ISP32_CSM_COEFF_NUM     ISP3X_CSM_COEFF_NUM

struct isp32_ldch_cfg {
    uint8_t frm_end_dis;
    uint8_t zero_interp_en;
    uint8_t sample_avr_en;
    uint8_t bic_mode_en;
    uint8_t force_map_en;
    uint8_t map13p3_en;

    uint8_t bicubic[ISP32_LDCH_BIC_NUM];

    uint32_t hsize;
    uint32_t vsize;
    int32_t buf_fd;
} __attribute__ ((packed));

struct isp32_awb_gain_cfg {
    /* AWB1_GAIN_G */
    uint16_t awb1_gain_gb;
    uint16_t awb1_gain_gr;
    /* AWB1_GAIN_RB */
    uint16_t awb1_gain_b;
    uint16_t awb1_gain_r;
    /* AWB0_GAIN0_G */
    uint16_t gain0_green_b;
    uint16_t gain0_green_r;
    /* AWB0_GAIN0_RB*/
    uint16_t gain0_blue;
    uint16_t gain0_red;
    /* AWB0_GAIN1_G */
    uint16_t gain1_green_b;
    uint16_t gain1_green_r;
    /* AWB0_GAIN1_RB*/
    uint16_t gain1_blue;
    uint16_t gain1_red;
    /* AWB0_GAIN2_G */
    uint16_t gain2_green_b;
    uint16_t gain2_green_r;
    /* AWB0_GAIN2_RB*/
    uint16_t gain2_blue;
    uint16_t gain2_red;
} __attribute__ ((packed));

struct isp32_bls_cfg {
    uint8_t enable_auto;
    uint8_t en_windows;
    uint8_t bls1_en;

    uint8_t bls_samples;

    struct isp2x_window bls_window1;
    struct isp2x_window bls_window2;
    struct isp2x_bls_fixed_val fixed_val;
    struct isp2x_bls_fixed_val bls1_val;

    uint16_t isp_ob_offset;
    uint16_t isp_ob_predgain;
    uint32_t isp_ob_max;
} __attribute__ ((packed));

struct isp32_ccm_cfg {
    /* CTRL */
    uint8_t highy_adjust_dis;
    uint8_t enh_adj_en;
    uint8_t asym_adj_en;
    /* BOUND_BIT */
    uint8_t bound_bit;
    uint8_t right_bit;
    /* COEFF0_R */
    int16_t coeff0_r;
    int16_t coeff1_r;
    /* COEFF1_R */
    int16_t coeff2_r;
    int16_t offset_r;
    /* COEFF0_G */
    int16_t coeff0_g;
    int16_t coeff1_g;
    /* COEFF1_G */
    int16_t coeff2_g;
    int16_t offset_g;
    /* COEFF0_B */
    int16_t coeff0_b;
    int16_t coeff1_b;
    /* COEFF1_B */
    int16_t coeff2_b;
    int16_t offset_b;
    /* COEFF0_Y */
    uint16_t coeff0_y;
    uint16_t coeff1_y;
    /* COEFF1_Y */
    uint16_t coeff2_y;
    /* ALP_Y */
    uint16_t alp_y[ISP32_CCM_CURVE_NUM];
    /* ENHANCE0 */
    uint16_t color_coef0_r2y;
    uint16_t color_coef1_g2y;
    /* ENHANCE1 */
    uint16_t color_coef2_b2y;
    uint16_t color_enh_rat_max;
} __attribute__ ((packed));

struct isp32_debayer_cfg {
    /* CONTROL */
    uint8_t filter_g_en;
    uint8_t filter_c_en;
    /* G_INTERP */
    uint8_t clip_en;
    uint8_t dist_scale;
    uint8_t thed0;
    uint8_t thed1;
    uint8_t select_thed;
    uint8_t max_ratio;
    /* G_INTERP_FILTER1 */
    int8_t filter1_coe1;
    int8_t filter1_coe2;
    int8_t filter1_coe3;
    int8_t filter1_coe4;
    /* G_INTERP_FILTER2 */
    int8_t filter2_coe1;
    int8_t filter2_coe2;
    int8_t filter2_coe3;
    int8_t filter2_coe4;
    /* C_FILTER_GUIDE_GAUS */
    int8_t guid_gaus_coe0;
    int8_t guid_gaus_coe1;
    int8_t guid_gaus_coe2;
    /* C_FILTER_CE_GAUS */
    int8_t ce_gaus_coe0;
    int8_t ce_gaus_coe1;
    int8_t ce_gaus_coe2;
    /* C_FILTER_ALPHA_GAUS */
    int8_t alpha_gaus_coe0;
    int8_t alpha_gaus_coe1;
    int8_t alpha_gaus_coe2;
    /* C_FILTER_IIR_0 */
    uint8_t ce_sgm;
    uint8_t exp_shift;
    /* C_FILTER_IIR_1 */
    uint8_t wet_clip;
    uint8_t wet_ghost;
    /* C_FILTER_BF */
    uint8_t bf_clip;
    uint8_t bf_curwgt;
    uint16_t bf_sgm;
    /* G_INTERP_OFFSET */
    uint16_t hf_offset;
    uint16_t gain_offset;
    /* G_FILTER_OFFSET */
    uint16_t offset;
    /* C_FILTER_LOG_OFFSET */
    uint16_t loghf_offset;
    uint16_t loggd_offset;
    /* C_FILTER_IIR_0 */
    uint16_t wgtslope;
    /* C_FILTER_ALPHA */
    uint16_t alpha_offset;
    /* C_FILTER_EDGE */
    uint16_t edge_offset;
    uint32_t edge_scale;
    /* C_FILTER_ALPHA */
    uint32_t alpha_scale;
} __attribute__ ((packed));

struct isp32_baynr_cfg {
    /* BAYNR_CTRL */
    uint8_t bay3d_gain_en;
    uint8_t lg2_mode;
    uint8_t gauss_en;
    uint8_t log_bypass;
    /* BAYNR_DGAIN */
    uint16_t dgain1;
    uint16_t dgain0;
    uint16_t dgain2;
    /* BAYNR_PIXDIFF */
    uint16_t pix_diff;
    /* BAYNR_THLD */
    uint16_t diff_thld;
    uint16_t softthld;
    /* BAYNR_W1_STRENG */
    uint16_t bltflt_streng;
    uint16_t reg_w1;
    /* BAYNR_SIGMA */
    uint16_t sigma_x[ISP32_BAYNR_XY_NUM];
    uint16_t sigma_y[ISP32_BAYNR_XY_NUM];
    /* BAYNR_WRIT_D */
    uint16_t weit_d2;
    uint16_t weit_d1;
    uint16_t weit_d0;
    /* BAYNR_LG_OFF */
    uint16_t lg2_lgoff;
    uint16_t lg2_off;
    /* BAYNR_DAT_MAX */
    uint32_t dat_max;
    /* BAYNR_SIGOFF */
    uint16_t rgain_off;
    uint16_t bgain_off;
    /* BAYNR_GAIN */
    uint8_t gain_x[ISP32_BAYNR_GAIN_NUM];
    uint16_t gain_y[ISP32_BAYNR_GAIN_NUM];
} __attribute__ ((packed));

struct isp32_bay3d_cfg {
    /* BAY3D_CTRL */
    uint8_t bypass_en;
    uint8_t hibypass_en;
    uint8_t lobypass_en;
    uint8_t himed_bypass_en;
    uint8_t higaus_bypass_en;
    uint8_t hiabs_possel;
    uint8_t hichnsplit_en;
    uint8_t lomed_bypass_en;
    uint8_t logaus5_bypass_en;
    uint8_t logaus3_bypass_en;
    uint8_t glbpk_en;
    uint8_t loswitch_protect;
    uint8_t bwsaving_en;
    /* BAY3D_CTRL1 */
    uint8_t hiwgt_opt_en;
    uint8_t hichncor_en;
    uint8_t bwopt_gain_dis;
    uint8_t lo4x8_en;
    uint8_t lo4x4_en;
    uint8_t hisig_ind_sel;
    uint8_t pksig_ind_sel;
    uint8_t iirwr_rnd_en;
    uint8_t curds_high_en;
    uint8_t higaus3_mode;
    uint8_t higaus5x5_en;
    uint8_t wgtmix_opt_en;

    /* for isp32_lite */
    uint8_t wgtmm_opt_en;
    uint8_t wgtmm_sel_en;

    /* BAY3D_SIGGAUS */
    uint8_t siggaus0;
    uint8_t siggaus1;
    uint8_t siggaus2;
    uint8_t siggaus3;
    /* BAY3D_KALRATIO */
    uint16_t softwgt;
    uint16_t hidif_th;
    /* BAY3D_WGTLMT */
    uint16_t wgtlmt;
    uint16_t wgtratio;
    /* BAY3D_SIG */
    uint16_t sig0_x[ISP32_BAY3D_XY_NUM];
    uint16_t sig0_y[ISP32_BAY3D_XY_NUM];
    uint16_t sig1_x[ISP32_BAY3D_XY_NUM];
    uint16_t sig1_y[ISP32_BAY3D_XY_NUM];
    uint16_t sig2_x[ISP32_BAY3D_XY_NUM];
    uint16_t sig2_y[ISP32_BAY3D_XY_NUM];

    /* LODIF_STAT1 for isp32_lite */
    uint16_t wgtmin;

    /* BAY3D_HISIGRAT */
    uint16_t hisigrat0;
    uint16_t hisigrat1;
    /* BAY3D_HISIGOFF */
    uint16_t hisigoff0;
    uint16_t hisigoff1;
    /* BAY3D_LOSIG */
    uint16_t losigoff;
    uint16_t losigrat;
    /* BAY3D_SIGPK */
    uint16_t rgain_off;
    uint16_t bgain_off;
    /* BAY3D_GLBPK2 */
    uint32_t glbpk2;
} __attribute__ ((packed));

struct isp32_ynr_cfg {
    /* YNR_GLOBAL_CTRL */
    uint8_t rnr_en;
    uint8_t thumb_mix_cur_en;
    uint8_t global_gain_alpha;
    uint8_t flt1x1_bypass_sel;
    uint8_t nlm11x11_bypass;
    uint8_t flt1x1_bypass;
    uint8_t lgft3x3_bypass;
    uint8_t lbft5x5_bypass;
    uint8_t bft3x3_bypass;
    /* YNR_RNR_STRENGTH */
    uint8_t rnr_strength3[ISP32_YNR_XY_NUM];
    /* YNR_NLM_SIGMA_GAIN */
    uint8_t nlm_hi_gain_alpha;
    /* YNR_NLM_COE */
    uint8_t nlm_coe[ISP32_YNR_NLM_COE_NUM];

    /* LOWNR_CTRL4 for isp32_lite */
    uint8_t frame_add4line;

    uint16_t global_gain;

    /* YNR_RNR_MAX_R */
    uint16_t rnr_max_r;
    uint16_t local_gainscale;
    /* YNR_RNR_CENTER_COOR */
    uint16_t rnr_center_coorh;
    uint16_t rnr_center_coorv;
    /* YNR_LOCAL_GAIN_CTRL */
    uint16_t loclagain_adj_thresh;
    uint16_t localgain_adj;
    /* YNR_LOWNR_CTRL0 */
    uint16_t low_bf_inv1;
    uint16_t low_bf_inv0;
    /* YNR_LOWNR_CTRL1 */
    uint16_t low_peak_supress;
    uint16_t low_thred_adj;
    /* YNR_LOWNR_CTRL2 */
    uint16_t low_dist_adj;
    uint16_t low_edge_adj_thresh;
    /* YNR_LOWNR_CTRL3 */
    uint16_t low_bi_weight;
    uint16_t low_weight;
    uint16_t low_center_weight;
    /* YNR_LOWNR_CTRL4 */
    uint16_t frame_full_size;
    uint16_t lbf_weight_thres;
    /* YNR_GAUSS1_COEFF */
    uint16_t low_gauss1_coeff2;
    uint16_t low_gauss1_coeff1;
    uint16_t low_gauss1_coeff0;
    /* YNR_GAUSS2_COEFF */
    uint16_t low_gauss2_coeff2;
    uint16_t low_gauss2_coeff1;
    uint16_t low_gauss2_coeff0;
    /* YNR_SGM_DX */
    uint16_t luma_points_x[ISP32_YNR_XY_NUM];
    /* YNR_LSGM_Y */
    uint16_t lsgm_y[ISP32_YNR_XY_NUM];
    /* YNR_NLM_SIGMA_GAIN */
    uint16_t nlm_min_sigma;
    uint16_t nlm_hi_bf_scale;
    /* YNR_NLM_WEIGHT */
    uint16_t nlm_nr_weight;
    uint16_t nlm_weight_offset;
    /* YNR_NLM_NR_WEIGHT */
    uint32_t nlm_center_weight;
} __attribute__ ((packed));

struct isp32_cnr_cfg {
    /* CNR_CTRL */
    uint8_t exgain_bypass;
    uint8_t yuv422_mode;
    uint8_t thumb_mode;
    uint8_t bf3x3_wgt0_sel;
    /* CNR_LBF_WEITD */
    uint8_t lbf1x7_weit_d0;
    uint8_t lbf1x7_weit_d1;
    uint8_t lbf1x7_weit_d2;
    uint8_t lbf1x7_weit_d3;
    /* CNR_IIR_PARA1 */
    uint8_t iir_uvgain;
    uint8_t iir_strength;
    uint8_t exp_shift;
    /* CNR_IIR_PARA2 */
    uint8_t chroma_ghost;
    uint8_t iir_uv_clip;
    /* CNR_GAUS_COE */
    uint8_t gaus_coe[ISP32_CNR_GAUS_COE_NUM];
    /* CNR_GAUS_RATIO */
    uint8_t bf_wgt_clip;
    /* CNR_BF_PARA1 */
    uint8_t uv_gain;
    uint8_t bf_ratio;
    /* CNR_SIGMA */
    uint8_t sigma_y[ISP32_CNR_SIGMA_Y_NUM];
    /* CNR_IIR_GLOBAL_GAIN */
    uint8_t iir_gain_alpha;
    uint8_t iir_global_gain;
    /* CNR_EXGAIN */
    uint8_t gain_iso;
    uint8_t global_gain_alpha;
    uint16_t global_gain;
    /* CNR_THUMB1 */
    uint16_t thumb_sigma_c;
    uint16_t thumb_sigma_y;
    /* CNR_THUMB_BF_RATIO */
    uint16_t thumb_bf_ratio;
    /* CNR_IIR_PARA1 */
    uint16_t wgt_slope;
    /* CNR_GAUS_RATIO */
    uint16_t gaus_ratio;
    uint16_t global_alpha;
    /* CNR_BF_PARA1 */
    uint16_t sigma_r;
    /* CNR_BF_PARA2 */
    uint16_t adj_offset;
    uint16_t adj_ratio;
} __attribute__ ((packed));

struct isp32_sharp_cfg {
    /* SHARP_EN */
    uint8_t bypass;
    uint8_t center_mode;
    uint8_t exgain_bypass;
    uint8_t radius_ds_mode;
    uint8_t noiseclip_mode;

    /* for isp32_lite */
    uint8_t clip_hf_mode;
    uint8_t add_mode;

    /* SHARP_RATIO */
    uint8_t sharp_ratio;
    uint8_t bf_ratio;
    uint8_t gaus_ratio;
    uint8_t pbf_ratio;
    /* SHARP_LUMA_DX */
    uint8_t luma_dx[ISP32_SHARP_X_NUM];
    /* SHARP_SIGMA_SHIFT */
    uint8_t bf_sigma_shift;
    uint8_t pbf_sigma_shift;
    /* SHARP_PBF_COEF */
    uint8_t pbf_coef2;
    uint8_t pbf_coef1;
    uint8_t pbf_coef0;
    /* SHARP_BF_COEF */
    uint8_t bf_coef2;
    uint8_t bf_coef1;
    uint8_t bf_coef0;
    /* SHARP_GAUS_COEF */
    uint8_t gaus_coef[ISP32_SHARP_GAUS_COEF_NUM];
    /* SHARP_GAIN */
    uint8_t global_gain_alpha;
    uint8_t local_gainscale;
    /* SHARP_GAIN_DIS_STRENGTH */
    uint8_t strength[ISP32_SHARP_STRENGTH_NUM];
    /* SHARP_TEXTURE */
    uint8_t enhance_bit;
    /* SHARP_PBF_SIGMA_INV */
    uint16_t pbf_sigma_inv[ISP32_SHARP_Y_NUM];
    /* SHARP_BF_SIGMA_INV */
    uint16_t bf_sigma_inv[ISP32_SHARP_Y_NUM];
    /* SHARP_CLIP_HF */
    uint16_t clip_hf[ISP32_SHARP_Y_NUM];
    /* SHARP_GAIN */
    uint16_t global_gain;
    /* SHARP_GAIN_ADJUST */
    uint16_t gain_adj[ISP32_SHARP_GAIN_ADJ_NUM];
    /* SHARP_CENTER */
    uint16_t center_wid;
    uint16_t center_het;
    /* SHARP_TEXTURE */
    uint16_t noise_sigma;
    uint16_t noise_strength;

    /* EHF_TH for isp32_lite */
    uint16_t ehf_th[ISP32_SHARP_Y_NUM];
    /* CLIP_NEG for isp32_lite */
    uint16_t clip_neg[ISP32_SHARP_Y_NUM];
} __attribute__ ((packed));

struct isp32_dhaz_cfg {
    /* DHAZ_CTRL */
    uint8_t enh_luma_en;
    uint8_t color_deviate_en;
    uint8_t round_en;
    uint8_t soft_wr_en;
    uint8_t enhance_en;
    uint8_t air_lc_en;
    uint8_t hpara_en;
    uint8_t hist_en;
    uint8_t dc_en;
    /* DHAZ_ADP0 */
    uint8_t yblk_th;
    uint8_t yhist_th;
    uint8_t dc_max_th;
    uint8_t dc_min_th;
    /* DHAZ_ADP2 */
    uint8_t tmax_base;
    uint8_t dark_th;
    uint8_t air_max;
    uint8_t air_min;
    /* DHAZ_GAUS */
    uint8_t gaus_h2;
    uint8_t gaus_h1;
    uint8_t gaus_h0;
    /* DHAZ_GAIN_IDX */
    uint8_t sigma_idx[ISP32_DHAZ_SIGMA_IDX_NUM];
    /* DHAZ_ADP_HIST1 */
    uint8_t hist_gratio;
    uint16_t hist_scale;
    /* DHAZ_ADP1 */
    uint8_t bright_max;
    uint8_t bright_min;
    uint16_t wt_max;
    /* DHAZ_ADP_TMAX */
    uint16_t tmax_max;
    uint16_t tmax_off;
    /* DHAZ_ADP_HIST0 */
    uint8_t hist_k;
    uint8_t hist_th_off;
    uint16_t hist_min;
    /* DHAZ_ENHANCE */
    uint16_t enhance_value;
    uint16_t enhance_chroma;
    /* DHAZ_IIR0 */
    uint16_t iir_wt_sigma;
    uint8_t iir_sigma;
    uint8_t stab_fnum;
    /* DHAZ_IIR1 */
    uint16_t iir_tmax_sigma;
    uint8_t iir_air_sigma;
    uint8_t iir_pre_wet;
    /* DHAZ_SOFT_CFG0 */
    uint16_t cfg_wt;
    uint8_t cfg_air;
    uint8_t cfg_alpha;
    /* DHAZ_SOFT_CFG1 */
    uint16_t cfg_gratio;
    uint16_t cfg_tmax;
    /* DHAZ_BF_SIGMA */
    uint16_t range_sima;
    uint8_t space_sigma_pre;
    uint8_t space_sigma_cur;
    /* DHAZ_BF_WET */
    uint16_t dc_weitcur;
    uint16_t bf_weight;
    /* DHAZ_ENH_CURVE */
    uint16_t enh_curve[ISP32_DHAZ_ENH_CURVE_NUM];

    uint16_t sigma_lut[ISP32_DHAZ_SIGMA_LUT_NUM];

    uint16_t hist_wr[ISP32_DHAZ_HIST_WR_NUM];

    uint16_t enh_luma[ISP32_DHAZ_ENH_LUMA_NUM];
} __attribute__ ((packed));

struct isp32_drc_cfg {
    uint8_t bypass_en;
    /* DRC_CTRL1 */
    uint8_t offset_pow2;
    uint16_t compres_scl;
    uint16_t position;
    /* DRC_LPRATIO */
    uint16_t hpdetail_ratio;
    uint16_t lpdetail_ratio;
    uint8_t delta_scalein;
    /* DRC_EXPLRATIO */
    uint8_t weicur_pix;
    uint8_t weipre_frame;
    uint8_t bilat_wt_off;
    /* DRC_SIGMA */
    uint8_t edge_scl;
    uint8_t motion_scl;
    uint16_t force_sgm_inv0;
    /* DRC_SPACESGM */
    uint16_t space_sgm_inv1;
    uint16_t space_sgm_inv0;
    /* DRC_RANESGM */
    uint16_t range_sgm_inv1;
    uint16_t range_sgm_inv0;
    /* DRC_BILAT */
    uint16_t bilat_soft_thd;
    uint8_t weig_maxl;
    uint8_t weig_bilat;
    uint8_t enable_soft_thd;
    /* DRC_IIRWG_GAIN */
    uint8_t iir_weight;
    uint16_t min_ogain;
    /* DRC_LUM3X2_CTRL */
    uint16_t gas_t;
    /* DRC_LUM3X2_GAS */
    uint8_t gas_l0;
    uint8_t gas_l1;
    uint8_t gas_l2;
    uint8_t gas_l3;

    uint16_t gain_y[ISP32_DRC_Y_NUM];
    uint16_t compres_y[ISP32_DRC_Y_NUM];
    uint16_t scale_y[ISP32_DRC_Y_NUM];
} __attribute__ ((packed));

struct isp32_hdrmge_cfg {
    uint8_t s_base;
    uint8_t mode;
    uint8_t dbg_mode;
    uint8_t each_raw_en;

    uint8_t gain2;

    uint8_t lm_dif_0p15;
    uint8_t lm_dif_0p9;
    uint8_t ms_diff_0p15;
    uint8_t ms_dif_0p8;

    uint16_t gain0_inv;
    uint16_t gain0;
    uint16_t gain1_inv;
    uint16_t gain1;

    uint16_t ms_thd1;
    uint16_t ms_thd0;
    uint16_t ms_scl;
    uint16_t lm_thd1;
    uint16_t lm_thd0;
    uint16_t lm_scl;
    struct isp2x_hdrmge_curve curve;
    uint16_t e_y[ISP32_HDRMGE_E_CURVE_NUM];
    uint16_t l_raw0[ISP32_HDRMGE_E_CURVE_NUM];
    uint16_t l_raw1[ISP32_HDRMGE_E_CURVE_NUM];
    uint16_t each_raw_gain0;
    uint16_t each_raw_gain1;
} __attribute__ ((packed));

struct isp32_rawawb_meas_cfg {
    uint8_t bls2_en;

    uint8_t rawawb_sel;
    uint8_t bnr2awb_sel;
    uint8_t drc2awb_sel;
    /* RAWAWB_CTRL */
    uint8_t uv_en0;
    uint8_t xy_en0;
    uint8_t yuv3d_en0;
    uint8_t yuv3d_ls_idx0;
    uint8_t yuv3d_ls_idx1;
    uint8_t yuv3d_ls_idx2;
    uint8_t yuv3d_ls_idx3;
    uint8_t in_rshift_to_12bit_en;
    uint8_t in_overexposure_check_en;
    uint8_t wind_size;
    uint8_t rawlsc_bypass_en;
    uint8_t light_num;
    uint8_t uv_en1;
    uint8_t xy_en1;
    uint8_t yuv3d_en1;
    uint8_t low12bit_val;
    /* RAWAWB_WEIGHT_CURVE_CTRL */
    uint8_t wp_luma_wei_en0;
    uint8_t wp_luma_wei_en1;
    uint8_t wp_blk_wei_en0;
    uint8_t wp_blk_wei_en1;
    uint8_t wp_hist_xytype;
    /* RAWAWB_MULTIWINDOW_EXC_CTRL */
    uint8_t exc_wp_region0_excen;
    uint8_t exc_wp_region0_measen;
    uint8_t exc_wp_region0_domain;
    uint8_t exc_wp_region1_excen;
    uint8_t exc_wp_region1_measen;
    uint8_t exc_wp_region1_domain;
    uint8_t exc_wp_region2_excen;
    uint8_t exc_wp_region2_measen;
    uint8_t exc_wp_region2_domain;
    uint8_t exc_wp_region3_excen;
    uint8_t exc_wp_region3_measen;
    uint8_t exc_wp_region3_domain;
    uint8_t exc_wp_region4_excen;
    uint8_t exc_wp_region4_domain;
    uint8_t exc_wp_region5_excen;
    uint8_t exc_wp_region5_domain;
    uint8_t exc_wp_region6_excen;
    uint8_t exc_wp_region6_domain;
    uint8_t multiwindow_en;
    /* RAWAWB_YWEIGHT_CURVE_XCOOR03 */
    uint8_t wp_luma_weicurve_y0;
    uint8_t wp_luma_weicurve_y1;
    uint8_t wp_luma_weicurve_y2;
    uint8_t wp_luma_weicurve_y3;
    /* RAWAWB_YWEIGHT_CURVE_XCOOR47 */
    uint8_t wp_luma_weicurve_y4;
    uint8_t wp_luma_weicurve_y5;
    uint8_t wp_luma_weicurve_y6;
    uint8_t wp_luma_weicurve_y7;
    /* RAWAWB_YWEIGHT_CURVE_XCOOR8 */
    uint8_t wp_luma_weicurve_y8;
    /* RAWAWB_YWEIGHT_CURVE_YCOOR03 */
    uint8_t wp_luma_weicurve_w0;
    uint8_t wp_luma_weicurve_w1;
    uint8_t wp_luma_weicurve_w2;
    uint8_t wp_luma_weicurve_w3;
    /* RAWAWB_YWEIGHT_CURVE_YCOOR47 */
    uint8_t wp_luma_weicurve_w4;
    uint8_t wp_luma_weicurve_w5;
    uint8_t wp_luma_weicurve_w6;
    uint8_t wp_luma_weicurve_w7;
    /* RAWAWB_YWEIGHT_CURVE_YCOOR8 */
    uint8_t wp_luma_weicurve_w8;
    /* RAWAWB_YUV_X1X2_DIS_0 */
    uint8_t dis_x1x2_ls0;
    uint8_t rotu0_ls0;
    uint8_t rotu1_ls0;
    /* RAWAWB_YUV_INTERP_CURVE_UCOOR_0 */
    uint8_t rotu2_ls0;
    uint8_t rotu3_ls0;
    uint8_t rotu4_ls0;
    uint8_t rotu5_ls0;
    /* RAWAWB_YUV_X1X2_DIS_1 */
    uint8_t dis_x1x2_ls1;
    uint8_t rotu0_ls1;
    uint8_t rotu1_ls1;
    /* YUV_INTERP_CURVE_UCOOR_1 */
    uint8_t rotu2_ls1;
    uint8_t rotu3_ls1;
    uint8_t rotu4_ls1;
    uint8_t rotu5_ls1;
    /* RAWAWB_YUV_X1X2_DIS_2 */
    uint8_t dis_x1x2_ls2;
    uint8_t rotu0_ls2;
    uint8_t rotu1_ls2;
    /* YUV_INTERP_CURVE_UCOOR_2 */
    uint8_t rotu2_ls2;
    uint8_t rotu3_ls2;
    uint8_t rotu4_ls2;
    uint8_t rotu5_ls2;
    /* RAWAWB_YUV_X1X2_DIS_3 */
    uint8_t dis_x1x2_ls3;
    uint8_t rotu0_ls3;
    uint8_t rotu1_ls3;
    uint8_t rotu2_ls3;
    uint8_t rotu3_ls3;
    uint8_t rotu4_ls3;
    uint8_t rotu5_ls3;
    /* RAWAWB_EXC_WP_WEIGHT */
    uint8_t exc_wp_region0_weight;
    uint8_t exc_wp_region1_weight;
    uint8_t exc_wp_region2_weight;
    uint8_t exc_wp_region3_weight;
    uint8_t exc_wp_region4_weight;
    uint8_t exc_wp_region5_weight;
    uint8_t exc_wp_region6_weight;
    /* RAWAWB_WRAM_DATA */
    uint8_t wp_blk_wei_w[ISP32_RAWAWB_WEIGHT_NUM];
    /* RAWAWB_BLK_CTRL */
    uint8_t blk_measure_enable;
    uint8_t blk_measure_mode;
    uint8_t blk_measure_xytype;
    uint8_t blk_rtdw_measure_en;
    uint8_t blk_measure_illu_idx;

    /* for isp32_lite */
    uint8_t ds16x8_mode_en;

    uint8_t blk_with_luma_wei_en;
    uint16_t in_overexposure_threshold;
    /* RAWAWB_LIMIT_RG_MAX*/
    uint16_t r_max;
    uint16_t g_max;
    /* RAWAWB_LIMIT_BY_MAX */
    uint16_t b_max;
    uint16_t y_max;
    /* RAWAWB_LIMIT_RG_MIN */
    uint16_t r_min;
    uint16_t g_min;
    /* RAWAWB_LIMIT_BY_MIN */
    uint16_t b_min;
    uint16_t y_min;
    /* RAWAWB_WIN_OFFS */
    uint16_t h_offs;
    uint16_t v_offs;
    /* RAWAWB_WIN_SIZE */
    uint16_t h_size;
    uint16_t v_size;
    /* RAWAWB_YWEIGHT_CURVE_YCOOR8 */
    uint16_t pre_wbgain_inv_r;
    /* RAWAWB_PRE_WBGAIN_INV */
    uint16_t pre_wbgain_inv_g;
    uint16_t pre_wbgain_inv_b;
    /* RAWAWB_UV_DETC_VERTEX */
    uint16_t vertex0_u_0;
    uint16_t vertex0_v_0;

    uint16_t vertex1_u_0;
    uint16_t vertex1_v_0;

    uint16_t vertex2_u_0;
    uint16_t vertex2_v_0;

    uint16_t vertex3_u_0;
    uint16_t vertex3_v_0;

    uint16_t vertex0_u_1;
    uint16_t vertex0_v_1;

    uint16_t vertex1_u_1;
    uint16_t vertex1_v_1;

    uint16_t vertex2_u_1;
    uint16_t vertex2_v_1;

    uint16_t vertex3_u_1;
    uint16_t vertex3_v_1;

    uint16_t vertex0_u_2;
    uint16_t vertex0_v_2;

    uint16_t vertex1_u_2;
    uint16_t vertex1_v_2;

    uint16_t vertex2_u_2;
    uint16_t vertex2_v_2;

    uint16_t vertex3_u_2;
    uint16_t vertex3_v_2;

    uint16_t vertex0_u_3;
    uint16_t vertex0_v_3;

    uint16_t vertex1_u_3;
    uint16_t vertex1_v_3;

    uint16_t vertex2_u_3;
    uint16_t vertex2_v_3;

    uint16_t vertex3_u_3;
    uint16_t vertex3_v_3;
    /* RAWAWB_RGB2XY_WT */
    uint16_t wt0;
    uint16_t wt1;
    uint16_t wt2;
    /* RAWAWB_RGB2XY_MAT */
    uint16_t mat0_x;
    uint16_t mat0_y;

    uint16_t mat1_x;
    uint16_t mat1_y;

    uint16_t mat2_x;
    uint16_t mat2_y;
    /* RAWAWB_XY_DETC_NOR */
    uint16_t nor_x0_0;
    uint16_t nor_x1_0;
    uint16_t nor_y0_0;
    uint16_t nor_y1_0;

    uint16_t nor_x0_1;
    uint16_t nor_x1_1;
    uint16_t nor_y0_1;
    uint16_t nor_y1_1;

    uint16_t nor_x0_2;
    uint16_t nor_x1_2;
    uint16_t nor_y0_2;
    uint16_t nor_y1_2;

    uint16_t nor_x0_3;
    uint16_t nor_x1_3;
    uint16_t nor_y0_3;
    uint16_t nor_y1_3;
    /* RAWAWB_XY_DETC_BIG */
    uint16_t big_x0_0;
    uint16_t big_x1_0;
    uint16_t big_y0_0;
    uint16_t big_y1_0;

    uint16_t big_x0_1;
    uint16_t big_x1_1;
    uint16_t big_y0_1;
    uint16_t big_y1_1;

    uint16_t big_x0_2;
    uint16_t big_x1_2;
    uint16_t big_y0_2;
    uint16_t big_y1_2;

    uint16_t big_x0_3;
    uint16_t big_x1_3;
    uint16_t big_y0_3;
    uint16_t big_y1_3;
    /* RAWAWB_MULTIWINDOW */
    uint16_t multiwindow0_v_offs;
    uint16_t multiwindow0_h_offs;
    uint16_t multiwindow0_v_size;
    uint16_t multiwindow0_h_size;

    uint16_t multiwindow1_v_offs;
    uint16_t multiwindow1_h_offs;
    uint16_t multiwindow1_v_size;
    uint16_t multiwindow1_h_size;

    uint16_t multiwindow2_v_offs;
    uint16_t multiwindow2_h_offs;
    uint16_t multiwindow2_v_size;
    uint16_t multiwindow2_h_size;

    uint16_t multiwindow3_v_offs;
    uint16_t multiwindow3_h_offs;
    uint16_t multiwindow3_v_size;
    uint16_t multiwindow3_h_size;
    /* RAWAWB_EXC_WP_REGION */
    uint16_t exc_wp_region0_xu0;
    uint16_t exc_wp_region0_xu1;

    uint16_t exc_wp_region0_yv0;
    uint16_t exc_wp_region0_yv1;

    uint16_t exc_wp_region1_xu0;
    uint16_t exc_wp_region1_xu1;

    uint16_t exc_wp_region1_yv0;
    uint16_t exc_wp_region1_yv1;

    uint16_t exc_wp_region2_xu0;
    uint16_t exc_wp_region2_xu1;

    uint16_t exc_wp_region2_yv0;
    uint16_t exc_wp_region2_yv1;

    uint16_t exc_wp_region3_xu0;
    uint16_t exc_wp_region3_xu1;

    uint16_t exc_wp_region3_yv0;
    uint16_t exc_wp_region3_yv1;

    uint16_t exc_wp_region4_xu0;
    uint16_t exc_wp_region4_xu1;

    uint16_t exc_wp_region4_yv0;
    uint16_t exc_wp_region4_yv1;

    uint16_t exc_wp_region5_xu0;
    uint16_t exc_wp_region5_xu1;

    uint16_t exc_wp_region5_yv0;
    uint16_t exc_wp_region5_yv1;

    uint16_t exc_wp_region6_xu0;
    uint16_t exc_wp_region6_xu1;

    uint16_t exc_wp_region6_yv0;
    uint16_t exc_wp_region6_yv1;
    /* RAWAWB_YUV_RGB2ROTY */
    uint16_t rgb2ryuvmat0_y;
    uint16_t rgb2ryuvmat1_y;
    uint16_t rgb2ryuvmat2_y;
    uint16_t rgb2ryuvofs_y;
    /* RAWAWB_YUV_RGB2ROTU */
    uint16_t rgb2ryuvmat0_u;
    uint16_t rgb2ryuvmat1_u;
    uint16_t rgb2ryuvmat2_u;
    uint16_t rgb2ryuvofs_u;
    /* RAWAWB_YUV_RGB2ROTV */
    uint16_t rgb2ryuvmat0_v;
    uint16_t rgb2ryuvmat1_v;
    uint16_t rgb2ryuvmat2_v;
    uint16_t rgb2ryuvofs_v;
    /* RAWAWB_YUV_X_COOR */
    uint16_t coor_x1_ls0_y;
    uint16_t vec_x21_ls0_y;
    uint16_t coor_x1_ls0_u;
    uint16_t vec_x21_ls0_u;
    uint16_t coor_x1_ls0_v;
    uint16_t vec_x21_ls0_v;

    uint16_t coor_x1_ls1_y;
    uint16_t vec_x21_ls1_y;
    uint16_t coor_x1_ls1_u;
    uint16_t vec_x21_ls1_u;
    uint16_t coor_x1_ls1_v;
    uint16_t vec_x21_ls1_v;

    uint16_t coor_x1_ls2_y;
    uint16_t vec_x21_ls2_y;
    uint16_t coor_x1_ls2_u;
    uint16_t vec_x21_ls2_v;
    uint16_t coor_x1_ls2_v;
    uint16_t vec_x21_ls2_u;

    uint16_t coor_x1_ls3_y;
    uint16_t vec_x21_ls3_y;
    uint16_t coor_x1_ls3_u;
    uint16_t vec_x21_ls3_u;
    uint16_t coor_x1_ls3_v;
    uint16_t vec_x21_ls3_v;
    /* RAWAWB_YUV_INTERP_CURVE_TH */
    uint16_t th0_ls0;
    uint16_t th1_ls0;
    uint16_t th2_ls0;
    uint16_t th3_ls0;
    uint16_t th4_ls0;
    uint16_t th5_ls0;

    uint16_t th0_ls1;
    uint16_t th1_ls1;
    uint16_t th2_ls1;
    uint16_t th3_ls1;
    uint16_t th4_ls1;
    uint16_t th5_ls1;

    uint16_t th0_ls2;
    uint16_t th1_ls2;
    uint16_t th2_ls2;
    uint16_t th3_ls2;
    uint16_t th4_ls2;
    uint16_t th5_ls2;

    uint16_t th0_ls3;
    uint16_t th1_ls3;
    uint16_t th2_ls3;
    uint16_t th3_ls3;
    uint16_t th4_ls3;
    uint16_t th5_ls3;
    /* RAWAWB_UV_DETC_ISLOPE */
    uint32_t islope01_0;
    uint32_t islope12_0;
    uint32_t islope23_0;
    uint32_t islope30_0;
    uint32_t islope01_1;
    uint32_t islope12_1;
    uint32_t islope23_1;
    uint32_t islope30_1;
    uint32_t islope01_2;
    uint32_t islope12_2;
    uint32_t islope23_2;
    uint32_t islope30_2;
    uint32_t islope01_3;
    uint32_t islope12_3;
    uint32_t islope23_3;
    uint32_t islope30_3;

    /* WIN_WEIGHT for isp32_lite */
    uint32_t win_weight[ISP32L_RAWAWB_WEIGHT_NUM];
    struct isp2x_bls_fixed_val bls2_val;
} __attribute__((packed));

struct isp32_rawaf_meas_cfg {
    uint8_t rawaf_sel;
    uint8_t num_afm_win;

    /* for isp32_lite */
    uint8_t bnr2af_sel;
    /* CTRL */
    uint8_t gamma_en;
    uint8_t gaus_en;
    uint8_t v1_fir_sel;
    uint8_t hiir_en;
    uint8_t viir_en;
    uint8_t accu_8bit_mode;
    uint8_t ldg_en;
    uint8_t h1_fv_mode;
    uint8_t h2_fv_mode;
    uint8_t v1_fv_mode;
    uint8_t v2_fv_mode;
    uint8_t ae_mode;
    uint8_t y_mode;
    uint8_t vldg_sel;
    uint8_t sobel_sel;
    uint8_t v_dnscl_mode;
    uint8_t from_awb;
    uint8_t from_ynr;
    uint8_t ae_config_use;

    /* for isp32_lite */
    uint8_t ae_sel;

    /* for isp32_lite */
    uint8_t hiir_left_border_mode;
    uint8_t avg_ds_en;
    uint8_t avg_ds_mode;
    uint8_t h1_acc_mode;
    uint8_t h2_acc_mode;
    uint8_t v1_acc_mode;
    uint8_t v2_acc_mode;

    /* CTRL1 for isp32_lite */
    int16_t bls_offset;
    uint8_t bls_en;
    uint8_t hldg_dilate_num;
    /* WINA_B */
    struct isp2x_window win[ISP32_RAWAF_WIN_NUM];
    /* INT_LINE */
    uint8_t line_num[ISP32_RAWAF_LINE_NUM];
    uint8_t line_en[ISP32_RAWAF_LINE_NUM];
    /* THRES */
    uint16_t afm_thres;
    /* VAR_SHIFT */
    uint8_t afm_var_shift[ISP32_RAWAF_WIN_NUM];
    uint8_t lum_var_shift[ISP32_RAWAF_WIN_NUM];
    /* for isp32_lite */
    uint8_t tnrin_shift;
    /* HVIIR_VAR_SHIFT */
    uint8_t h1iir_var_shift;
    uint8_t h2iir_var_shift;
    uint8_t v1iir_var_shift;
    uint8_t v2iir_var_shift;
    /* GAUS_COE */
    int8_t gaus_coe[ISP32_RAWAF_GAUS_COE_NUM];

    /* GAMMA_Y */
    uint16_t gamma_y[ISP32_RAWAF_GAMMA_NUM];
    /* HIIR_THRESH */
    uint16_t h_fv_thresh;
    uint16_t v_fv_thresh;
    struct isp3x_rawaf_curve curve_h[ISP32_RAWAF_CURVE_NUM];
    struct isp3x_rawaf_curve curve_v[ISP32_RAWAF_CURVE_NUM];
    int16_t h1iir1_coe[ISP32_RAWAF_HIIR_COE_NUM];
    int16_t h1iir2_coe[ISP32_RAWAF_HIIR_COE_NUM];
    int16_t h2iir1_coe[ISP32_RAWAF_HIIR_COE_NUM];
    int16_t h2iir2_coe[ISP32_RAWAF_HIIR_COE_NUM];
    int16_t v1iir_coe[ISP32_RAWAF_VIIR_COE_NUM];
    int16_t v2iir_coe[ISP32_RAWAF_VIIR_COE_NUM];
    int16_t v1fir_coe[ISP32_RAWAF_VFIR_COE_NUM];
    int16_t v2fir_coe[ISP32_RAWAF_VFIR_COE_NUM];
    uint16_t highlit_thresh;

    /* CORING_H for isp32_lite */
    uint16_t h_fv_limit;
    uint16_t h_fv_slope;
    /* CORING_V for isp32_lite */
    uint16_t v_fv_limit;
    uint16_t v_fv_slope;
} __attribute__ ((packed));

struct isp32_cac_cfg {
    uint8_t bypass_en;
    uint8_t center_en;
    uint8_t clip_g_mode;
    uint8_t edge_detect_en;
    uint8_t neg_clip0_en;

    uint8_t flat_thed_b;
    uint8_t flat_thed_r;

    uint8_t psf_sft_bit;
    uint16_t cfg_num;

    uint16_t center_width;
    uint16_t center_height;

    uint16_t strength[ISP32_CAC_STRENGTH_NUM];

    uint16_t offset_b;
    uint16_t offset_r;

    uint32_t expo_thed_b;
    uint32_t expo_thed_r;
    uint32_t expo_adj_b;
    uint32_t expo_adj_r;

    uint32_t hsize;
    uint32_t vsize;
    int32_t buf_fd;
} __attribute__ ((packed));

struct isp32_vsm_cfg {
    uint8_t h_segments;
    uint8_t v_segments;
    uint16_t h_offs;
    uint16_t v_offs;
    uint16_t h_size;
    uint16_t v_size;
} __attribute__((packed));

struct isp32_isp_other_cfg {
    struct isp32_bls_cfg bls_cfg;
    struct isp2x_dpcc_cfg dpcc_cfg;
    struct isp3x_lsc_cfg lsc_cfg;
    struct isp32_awb_gain_cfg awb_gain_cfg;
    struct isp21_gic_cfg gic_cfg;
    struct isp32_debayer_cfg debayer_cfg;
    struct isp32_ccm_cfg ccm_cfg;
    struct isp3x_gammaout_cfg gammaout_cfg;
    struct isp2x_cproc_cfg cproc_cfg;
    struct isp2x_ie_cfg ie_cfg;
    struct isp2x_sdg_cfg sdg_cfg;
    struct isp32_drc_cfg drc_cfg;
    struct isp32_hdrmge_cfg hdrmge_cfg;
    struct isp32_dhaz_cfg dhaz_cfg;
    struct isp2x_3dlut_cfg isp3dlut_cfg;
    struct isp32_ldch_cfg ldch_cfg;
    struct isp32_baynr_cfg baynr_cfg;
    struct isp32_bay3d_cfg bay3d_cfg;
    struct isp32_ynr_cfg ynr_cfg;
    struct isp32_cnr_cfg cnr_cfg;
    struct isp32_sharp_cfg sharp_cfg;
    struct isp32_cac_cfg cac_cfg;
    struct isp3x_gain_cfg gain_cfg;
    struct isp21_csm_cfg csm_cfg;
    struct isp21_cgc_cfg cgc_cfg;
    struct isp32_vsm_cfg vsm_cfg;
} __attribute__ ((packed));

struct isp32_isp_meas_cfg {
    struct isp32_rawaf_meas_cfg rawaf;
    struct isp32_rawawb_meas_cfg rawawb;
    struct isp2x_rawaelite_meas_cfg rawae0;
    struct isp2x_rawaebig_meas_cfg rawae1;
    struct isp2x_rawaebig_meas_cfg rawae2;
    struct isp2x_rawaebig_meas_cfg rawae3;
    struct isp2x_rawhistlite_cfg rawhist0;
    struct isp2x_rawhistbig_cfg rawhist1;
    struct isp2x_rawhistbig_cfg rawhist2;
    struct isp2x_rawhistbig_cfg rawhist3;
} __attribute__ ((packed));

struct isp32_rawae_meas_data {
    uint32_t channelg_xy: 12;
    uint32_t channelb_xy: 10;
    uint32_t channelr_xy: 10;
} __attribute__ ((packed));

struct isp32_rawaebig_stat0 {
    struct isp32_rawae_meas_data data[ISP32_RAWAEBIG_MEAN_NUM];
    uint32_t reserved[3];
} __attribute__ ((packed));

struct isp32_rawaebig_stat1 {
    uint32_t sumr[ISP32_RAWAEBIG_SUBWIN_NUM];
    uint32_t sumg[ISP32_RAWAEBIG_SUBWIN_NUM];
    uint32_t sumb[ISP32_RAWAEBIG_SUBWIN_NUM];
} __attribute__ ((packed));

struct isp32_rawaelite_stat {
    struct isp32_rawae_meas_data data[ISP32_RAWAELITE_MEAN_NUM];
    uint32_t reserved[21];
} __attribute__ ((packed));

struct isp32_rawaf_stat {
    struct isp3x_rawaf_ramdata ramdata[ISP32_RAWAF_SUMDATA_NUM];
    uint32_t int_state;
    uint32_t afm_sum_b;
    uint32_t afm_lum_b;
    uint32_t highlit_cnt_winb;
    uint32_t reserved[18];
} __attribute__ ((packed));

struct isp32_rawawb_ramdata {
    uint64_t b: 18;
    uint64_t g: 18;
    uint64_t r: 18;
    uint64_t wp: 10;
} __attribute__ ((packed));

struct isp32_rawawb_sum {
    uint32_t rgain_nor;
    uint32_t bgain_nor;
    uint32_t wp_num_nor;
    uint32_t wp_num2;

    uint32_t rgain_big;
    uint32_t bgain_big;
    uint32_t wp_num_big;
    uint32_t reserved;
} __attribute__ ((packed));

struct isp32_rawawb_sum_exc {
    uint32_t rgain_exc;
    uint32_t bgain_exc;
    uint32_t wp_num_exc;
    uint32_t reserved;
} __attribute__ ((packed));

struct isp32_rawawb_meas_stat {
    struct isp32_rawawb_ramdata ramdata[ISP32_RAWAWB_RAMDATA_NUM];
    uint64_t reserved;
    struct isp32_rawawb_sum sum[ISP32_RAWAWB_SUM_NUM];
    uint16_t yhist_bin[ISP32_RAWAWB_HSTBIN_NUM];
    struct isp32_rawawb_sum_exc sum_exc[ISP32_RAWAWB_EXCL_STAT_NUM];
} __attribute__ ((packed));

struct isp32_vsm_stat {
    uint16_t delta_h;
    uint16_t delta_v;
} __attribute__ ((packed));

struct isp32_info2ddr_stat {
    uint32_t owner;
    int32_t buf_fd;
} __attribute__ ((packed));

struct isp32_isp_params_cfg {
    uint64_t module_en_update;
    uint64_t module_ens;
    uint64_t module_cfg_update;

    uint32_t frame_id;
    struct isp32_isp_meas_cfg meas;
    struct isp32_isp_other_cfg others;
} __attribute__ ((packed));

struct isp32_stat {
    struct isp32_rawaebig_stat0 rawae3_0;   //offset 0
    struct isp32_rawaebig_stat0 rawae1_0;   //offset 0x390
    struct isp32_rawaebig_stat0 rawae2_0;   //offset 0x720
    struct isp32_rawaelite_stat rawae0; //offset 0xab0
    struct isp32_rawaebig_stat1 rawae3_1;
    struct isp32_rawaebig_stat1 rawae1_1;
    struct isp32_rawaebig_stat1 rawae2_1;
    struct isp2x_bls_stat bls;
    struct isp2x_rawhistbig_stat rawhist3;  //offset 0xc00
    struct isp2x_rawhistlite_stat rawhist0; //offset 0x1000
    struct isp2x_rawhistbig_stat rawhist1;  //offset 0x1400
    struct isp2x_rawhistbig_stat rawhist2;  //offset 0x1800
    struct isp32_rawaf_stat rawaf;      //offset 0x1c00
    struct isp3x_dhaz_stat dhaz;
    struct isp32_vsm_stat vsm;
    struct isp32_info2ddr_stat info2ddr;
    struct isp32_rawawb_meas_stat rawawb;   //offset 0x2b00
} __attribute__ ((packed));

/**
 * struct rkisp32_isp_stat_buffer - Rockchip ISP32 Statistics Meta Data
 *
 * @meas_type: measurement types (ISP3X_STAT_ definitions)
 * @frame_id: frame ID for sync
 * @params: statistics data
 */
struct rkisp32_isp_stat_buffer {
    struct isp32_stat params;
    uint32_t meas_type;
    uint32_t frame_id;
    uint32_t params_id;
} __attribute__ ((packed));

#endif /* _UAPI_RKISP32_CONFIG_H */
