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

#ifndef _RK_AIQ_TYPE_AYNR_HW_V2_H_
#define _RK_AIQ_TYPE_AYNR_HW_V2_H_
#include "rk_aiq_comm.h"



typedef struct RK_YNR_Fix_V2_s{
	
	// YNR_2700_GLOBAL_CTRL (0x0000)
	uint8_t ynr_debug_en;
	uint8_t ynr_gate_dis;
	uint8_t ynr_thumb_mix_cur_en;
	uint8_t ynr_global_gain_alpha;
	uint8_t ynr_global_gain;
	uint8_t ynr_flt1x1_bypass_sel;
	uint8_t ynr_sft5x5_bypass;
	uint8_t ynr_flt1x1_bypass;
	uint8_t ynr_lgft3x3_bypass;
	uint8_t ynr_lbft5x5_bypass;
	uint8_t ynr_bft3x3_bypass;
	uint8_t ynr_en;


	// YNR_2700_RNR_MAX_R  (0x0004)
	uint16_t ynr_rnr_max_r;

	// YNR_2700_LOWNR_CTRL0 (0x0010)
	uint16_t ynr_low_bf_inv[2];

	// YNR_2700_LOWNR_CTRL1  (0x0014)
	uint8_t ynr_low_peak_supress;
	uint16_t ynr_low_thred_adj;

	// YNR_2700_LOWNR_CTRL2 (0x0018)
	uint16_t ynr_low_dist_adj;
	uint16_t ynr_low_edge_adj_thresh;

	// YNR_2700_LOWNR_CTRL3 (0x001c)
	uint8_t ynr_low_bi_weight;
	uint8_t ynr_low_weight;
	uint16_t ynr_low_center_weight;

	// YNR_2700_HIGHNR_CTRL0 (0x0020)
	uint8_t ynr_hi_min_adj;
	uint16_t ynr_high_thred_adj;

	// YNR_2700_HIGHNR_CTRL1  (0x0024)
	uint8_t ynr_high_retain_weight;
	uint8_t ynr_hi_edge_thed;

	// YNR_2700_HIGHNR_BASE_FILTER_WEIGHT  (0x0028)
	uint8_t ynr_base_filter_weight[3];

	// YNR_2700_GAUSS1_COEFF  (0x0030)
	uint16_t ynr_low_gauss1_coeff[3];

	// YNR_2700_GAUSS2_COEFF  (0x0034)
	uint16_t ynr_low_gauss2_coeff[3];

	// YNR_2700_DIRECTION_W_0_3  (0x0038 - 0x003c)
	uint8_t ynr_direction_weight[8];

	// YNR_2700_SGM_DX_0_1 (0x0040 - 0x0060)
	uint16_t ynr_luma_points_x[17];

	// YNR_2700_LSGM_Y_0_1 (0x0070- 0x0090)
	uint16_t ynr_lsgm_y[17];

	// YNR_2700_HSGM_Y_0_1 (0x00a0- 0x00c0)
	uint16_t ynr_hsgm_y[17];

	// YNR_2700_RNR_STRENGTH03 (0x00d0- 0x00e0)
	uint16_t ynr_rnr_strength[17];

}RK_YNR_Fix_V2_t;


#endif



