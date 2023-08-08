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

#ifndef _RK_AIQ_TYPE_ACNR_HW_V1_H_
#define _RK_AIQ_TYPE_ACNR_HW_V1_H_
#include "rk_aiq_comm.h"

typedef struct RK_CNR_Fix_V1_s{
	//ISP_CNR_2800_CTRL
	uint8_t cnr_thumb_mix_cur_en;
	uint8_t cnr_lq_bila_bypass;
	uint8_t cnr_hq_bila_bypass;
	uint8_t cnr_exgain_bypass;
	uint8_t cnr_en_i;

	// ISP_CNR_2800_EXGAIN 
	uint8_t cnr_exgain_mux;

	// ISP_CNR_2800_GAIN_PARA 
	uint8_t cnr_gain_iso;
	uint8_t cnr_gain_offset;
	uint8_t cnr_gain_1sigma;

	// ISP_CNR_2800_GAIN_UV_PARA 
	uint8_t cnr_gain_uvgain1;
	uint8_t cnr_gain_uvgain0;
	
	// ISP_CNR_2800_LMED3 
	uint8_t cnr_lmed3_alpha;
	
	// ISP_CNR_2800_LBF5_GAIN
	uint8_t cnr_lbf5_gain_y;
	uint8_t cnr_lbf5_gain_c;
	
	// ISP_CNR_2800_LBF5_WEITD0_4 
	uint8_t cnr_lbf5_weit_d[5];
	
	// ISP_CNR_2800_HMED3 
	uint8_t cnr_hmed3_alpha;
	
	// ISP_CNR_2800_HBF5
	uint8_t cnr_hbf5_weit_src;
	uint8_t cnr_hbf5_min_wgt;
	uint16_t cnr_hbf5_sigma;
	
	// ISP_CNR_2800_LBF3 
	uint8_t cnr_lbf5_weit_src;
	uint16_t cnr_lbf3_sigma;

}RK_CNR_Fix_V1_t;



#endif



