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

#ifndef _RK_AIQ_TYPE_ABAYERNR_HW_V2_H_
#define _RK_AIQ_TYPE_ABAYERNR_HW_V2_H_
#include "rk_aiq_comm.h"



typedef struct RK_Bayernr_2D_Fix_V2_s{
	
	//ISP_BAYNR_3A00_CTRL
	uint8_t baynr_gauss_en;
	uint8_t baynr_log_bypass;
	uint8_t baynr_en;
		
	// ISP_BAYNR_3A00_DGAIN0-2
	uint16_t baynr_dgain[3];

	// ISP_BAYNR_3A00_PIXDIFF
	uint16_t baynr_pix_diff;

	// ISP_BAYNR_3A00_THLD
	uint16_t baynr_diff_thld;
	uint16_t baynr_softthld;

	// ISP_BAYNR_3A00_W1_STRENG
	uint16_t bltflt_streng;
	uint16_t baynr_reg_w1;

	// ISP_BAYNR_3A00_SIGMAX0-15
	uint16_t sigma_x[16];

	// ISP_BAYNR_3A00_SIGMAY0-15
	uint16_t sigma_y[16];

	// ISP_BAYNR_3A00_WRIT_D
	uint16_t weit_d[3];
}RK_Bayernr_2D_Fix_V2_t;


typedef struct RK_Bayernr_3D_Fix_V2_s{
	
	// BAY3D_BAY3D_CTRL 
	uint8_t bay3d_exp_sel;
	uint8_t bay3d_bypass_en;
	uint8_t bay3d_pk_en;
	uint8_t bay3d_en_i;

	// BAY3D_BAY3D_KALRATIO 
	uint16_t bay3d_softwgt;
	uint16_t bay3d_sigratio;

	// BAY3D_BAY3D_GLBPK2 
	uint32_t bay3d_glbpk2;

	// BAY3D_BAY3D_KALSTR 
	uint16_t bay3d_exp_str;
	uint16_t bay3d_str;

	// BAY3D_BAY3D_WGTLMT 
	uint16_t bay3d_wgtlmt_h;
	uint16_t bay3d_wgtlmt_l;

	// BAY3D_BAY3D_SIG_X0  
	uint16_t bay3d_sig_x[16];

	// BAY3D_BAY3D_SIG_Y0 
	uint16_t bay3d_sig_y[16];

}RK_Bayernr_3D_Fix_V2_t;


typedef struct RK_Bayernr_Fix_V2_s{
	RK_Bayernr_2D_Fix_V2_t st2DParam;
	RK_Bayernr_3D_Fix_V2_t st3DParam;

}RK_Bayernr_Fix_V2_t;



#endif



