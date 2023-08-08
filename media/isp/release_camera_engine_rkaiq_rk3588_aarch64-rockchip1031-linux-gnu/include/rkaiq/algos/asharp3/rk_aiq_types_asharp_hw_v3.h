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

#ifndef _RK_AIQ_TYPE_ASHARP_HW_V3_H_
#define _RK_AIQ_TYPE_ASHARP_HW_V3_H_
#include "rk_aiq_comm.h"

typedef struct RK_SHARP_Fix_V3_s
{
	// SHARP_SHARP_EN (0x0000)
	uint8_t sharp_bypass;
	uint8_t sharp_en;

	// SHARP_SHARP_RATIO  (0x0004)
	uint8_t sharp_sharp_ratio;
	uint8_t sharp_bf_ratio;
	uint8_t sharp_gaus_ratio;
	uint8_t sharp_pbf_ratio;

	// SHARP_SHARP_LUMA_DX (0x0008)
	uint8_t sharp_luma_dx[7];

	// SHARP_SHARP_PBF_SIGMA_INV_0 (0x000c - 0x0014)
	uint16_t sharp_pbf_sigma_inv[8];

	// SHARP_SHARP_BF_SIGMA_INV_0 (0x0018 -  0x0020)
	uint16_t sharp_bf_sigma_inv[8];

	// SHARP_SHARP_SIGMA_SHIFT (0x00024)
	uint8_t sharp_bf_sigma_shift;
	uint8_t sharp_pbf_sigma_shift;

	// SHARP_SHARP_EHF_TH_0 (0x0028 -  0x0030)
	uint16_t sharp_ehf_th[8];

	// SHARP_SHARP_CLIP_HF_0 (0x0034 -  0x003c)
	uint16_t sharp_clip_hf[8];

	// SHARP_SHARP_PBF_COEF (0x00040)
	uint8_t sharp_pbf_coef[3];

	// SHARP_SHARP_BF_COEF (0x00044)
	uint8_t sharp_bf_coef[3];

	// SHARP_SHARP_GAUS_COEF (0x00048)
	uint8_t sharp_gaus_coef[3];

}RK_SHARP_Fix_V3_t;


#endif



