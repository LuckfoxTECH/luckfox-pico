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

#ifndef _RK_AIQ_TYPE_ANR_HW_H_
#define _RK_AIQ_TYPE_ANR_HW_H_
#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE


typedef struct RKAnr_Bayernr_Fix_s {
    unsigned char rawnr_en;
    unsigned char gauss_en;
    unsigned char log_bypass;

    unsigned short filtpar0;
    unsigned short filtpar1;
    unsigned short filtpar2;

    unsigned int dgain0;
    unsigned int dgain1;
    unsigned int dgain2;

    unsigned short luration[8];
    unsigned short lulevel[8];

    unsigned int gauss;
    unsigned short sigma;
    unsigned short pix_diff;

    unsigned int thld_diff;

    unsigned char gas_weig_scl2;
    unsigned char gas_weig_scl1;
    unsigned short thld_chanelw;

    unsigned short lamda;

    unsigned short fixw0;
    unsigned short fixw1;
    unsigned short fixw2;
    unsigned short fixw3;

    unsigned int wlamda0;
    unsigned int wlamda1;
    unsigned int wlamda2;

    unsigned short rgain_filp;
    unsigned short bgain_filp;
} RKAnr_Bayernr_Fix_t;



typedef struct RKAnr_Uvnr_Fix_s {
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

} RKAnr_Uvnr_Fix_t;



typedef struct RKAnr_Ynr_Fix_s {
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
} RKAnr_Ynr_Fix_t;



typedef struct RKAnr_Mfnr_Fix_s {
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
} RKAnr_Mfnr_Fix_t;

typedef struct RKAnr_Gain_Fix_s {
    unsigned char gain_table_en;
    unsigned char dhaz_en;
    unsigned char wdr_en;
    unsigned char tmo_en;
    unsigned char lsc_en;
    unsigned char mge_en;

    unsigned int mge_gain[3];
    unsigned short int idx[15];
    unsigned short int lut[17];
} RKAnr_Gain_Fix_t;


RKAIQ_END_DECLARE

#endif



