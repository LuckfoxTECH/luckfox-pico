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

#ifndef _RK_AIQ_TYPE_AYNR_HW_V1_H_
#define _RK_AIQ_TYPE_AYNR_HW_V1_H_
#include "rk_aiq_comm.h"



typedef struct RK_YNR_Fix_V1_s{
	
	 unsigned char ynr_en;
    unsigned char ynr_sgm_dx[16];
    unsigned short ynr_lsgm_y[17];
    unsigned char ynr_lci[4];
    unsigned char ynr_lgain_min[4];
    unsigned char ynr_lgain_max;

    unsigned char ynr_lmerge_bound;
    unsigned char ynr_lmerge_ratio;

    unsigned char ynr_lweit_flt[4];
    unsigned short ynr_hsgm_y[17];
    unsigned char ynr_hlci[4];
    unsigned char ynr_lhci[4];
    unsigned char ynr_hhci[4];
    unsigned char ynr_hgain_sgm[4];
    unsigned char ynr_hweit_d[20];
    unsigned char ynr_hgrad_y[24];
    unsigned short ynr_hweit[4];
    unsigned char ynr_hmax_adjust;
    unsigned char ynr_hstrength;
    unsigned char ynr_lweit_cmp[2];
    unsigned char ynr_lmaxgain_lv4;
    unsigned short ynr_hstv_y[17];
    unsigned short ynr_st_scale[3];

}RK_YNR_Fix_V1_t;


#endif



