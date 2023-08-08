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

#ifndef _RK_AIQ_TYPE_AMFNR_HW_V1_H_
#define _RK_AIQ_TYPE_AMFNR_HW_V1_H_
#include "rk_aiq_comm.h"



typedef struct RK_MFNR_Fix_V1_s{
	
	 unsigned char tnr_en;
    unsigned char mode;
    unsigned char opty_en;
    unsigned char optc_en;
    unsigned char gain_en;

    unsigned char pk0_y;
    unsigned char pk1_y;
    unsigned char pk0_c;
    unsigned char pk1_c;

    unsigned short glb_gain_cur;
    unsigned short glb_gain_nxt;
    unsigned short glb_gain_cur_div;
    unsigned char glb_gain_cur_sqrt;

    unsigned char sigma_x[16];
    unsigned short sigma_y[17];

    unsigned short luma_curve[6];

    unsigned short txt_th0_y;
    unsigned short txt_th1_y;
    unsigned short txt_th0_c;
    unsigned short txt_th1_c;
    unsigned short txt_thy_dlt;
    unsigned short txt_thc_dlt;

    unsigned char gfcoef_y0[6];
    unsigned char gfcoef_y1[3];
    unsigned char gfcoef_y2[3];
    unsigned char gfcoef_y3[3];

    unsigned char gfcoef_yg0[6];
    unsigned char gfcoef_yg1[3];
    unsigned char gfcoef_yg2[3];
    unsigned char gfcoef_yg3[3];

    unsigned char gfcoef_yl0[6];
    unsigned char gfcoef_yl1[3];
    unsigned char gfcoef_yl2[3];

    unsigned char gfcoef_cg0[6];
    unsigned char gfcoef_cg1[3];
    unsigned char gfcoef_cg2[3];

    unsigned char gfcoef_cl0[6];
    unsigned char gfcoef_cl1[3];

    unsigned short scale_yg[4];
    unsigned short scale_yl[3];
    unsigned short scale_cg[3];
    unsigned short scale_y2cg[3];
    unsigned short scale_cl[2];
    unsigned short scale_y2cl[3];

    unsigned char weight_y[3];

}RK_MFNR_Fix_V1_t;


#endif



