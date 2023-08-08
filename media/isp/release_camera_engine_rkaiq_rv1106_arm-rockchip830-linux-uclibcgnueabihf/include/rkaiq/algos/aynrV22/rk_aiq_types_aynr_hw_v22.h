/*
 *rk_aiq_types_alsc_hw.h
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

#ifndef _RK_AIQ_TYPE_AYNR_HW_V22_H_
#define _RK_AIQ_TYPE_AYNR_HW_V22_H_
#include "rk_aiq_comm.h"



typedef struct RK_YNR_Fix_V22_s {
    /* YNR_GLOBAL_CTRL */
    uint8_t rnr_en;
    uint8_t gate_dis;
    uint8_t thumb_mix_cur_en;
    uint8_t global_gain_alpha;
    uint16_t global_gain;
    uint8_t flt1x1_bypass_sel;
    uint8_t nlm11x11_bypass;
    uint8_t flt1x1_bypass;
    uint8_t lgft3x3_bypass;
    uint8_t lbft5x5_bypass;
    uint8_t bft3x3_bypass;
    uint8_t ynr_en;


    /* YNR_RNR_MAX_R */
    uint16_t rnr_max_r;
    uint16_t local_gainscale;

    /* YNR_RNR_CENTER_COOR */
    uint16_t rnr_center_coorh;
    uint16_t rnr_center_coorv;

    /* YNR_LOCAL_GAIN_CTRL */
    uint16_t localgain_adj_thresh;
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
    uint16_t luma_points_x[17];
    /* YNR_LSGM_Y */
    uint16_t lsgm_y[17];

    /* YNR_RNR_STRENGTH */
    uint8_t rnr_strength[17];

    /* YNR_NLM_SIGMA_GAIN */
    uint16_t nlm_min_sigma;
    uint8_t  nlm_hi_gain_alpha;
    uint16_t nlm_hi_bf_scale;

    /* YNR_NLM_COE */
    uint8_t nlm_coe_0;
    uint8_t nlm_coe_1;
    uint8_t nlm_coe_2;
    uint8_t nlm_coe_3;
    uint8_t nlm_coe_4;
    uint8_t nlm_coe_5;

    /* YNR_NLM_WEIGHT */
    uint32_t nlm_center_weight;
    uint16_t nlm_weight_offset;

    /* YNR_NLM_NR_WEIGHT */
    uint16_t nlm_nr_weight;

} RK_YNR_Fix_V22_t;


#endif



