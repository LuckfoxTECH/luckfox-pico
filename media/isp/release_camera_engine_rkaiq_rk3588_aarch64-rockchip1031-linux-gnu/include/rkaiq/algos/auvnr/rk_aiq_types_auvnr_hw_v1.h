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

#ifndef _RK_AIQ_TYPE_AUVNR_HW_V1_H_
#define _RK_AIQ_TYPE_AUVNR_HW_V1_H_
#include "rk_aiq_comm.h"

typedef struct RK_UVNR_Fix_V1_s{
	unsigned char uvnr_en;
    unsigned char uvnr_step1_en;
    unsigned char uvnr_step2_en;
    unsigned char nr_gain_en;
    unsigned char uvnr_nobig_en;
    unsigned char uvnr_big_en;

    unsigned char uvnr_gain_1sigma;
    unsigned char uvnr_gain_offset;
    unsigned char uvnr_gain_uvgain[2];
    unsigned char uvnr_gain_t2gen;
    unsigned char uvnr_gain_iso;
    unsigned char uvnr_t1gen_m3alpha;
    unsigned char uvnr_t1flt_mode;
    unsigned short uvnr_t1flt_msigma;
    unsigned char uvnr_t1flt_wtp;
    unsigned char uvnr_t1flt_wtq[8];
    unsigned char uvnr_t2gen_m3alpha;
    unsigned short uvnr_t2gen_msigma;
    unsigned char uvnr_t2gen_wtp;
    unsigned char uvnr_t2gen_wtq[4];
    unsigned short uvnr_t2flt_msigma;

    unsigned char uvnr_t2flt_wtp;
    unsigned char uvnr_t2flt_wt[3];

}RK_UVNR_Fix_V1_t;



#endif



