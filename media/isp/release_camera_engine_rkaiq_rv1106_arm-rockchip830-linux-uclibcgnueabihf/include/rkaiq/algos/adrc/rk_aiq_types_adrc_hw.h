/*
 * rk_aiq_algo_adrc_hw.h
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
#include "adrc_head.h"

#ifndef _RK_AIQ_ALGO_ADRC_HW_H_
#define _RK_AIQ_ALGO_ADRC_HW_H_

#define DRC_V10_Y_NUM 17
#define DRC_V11_Y_NUM DRC_V10_Y_NUM
#define DRC_V12_Y_NUM DRC_V10_Y_NUM

typedef struct DrcProcResV10_s {
    int sw_drc_offset_pow2;
    int sw_drc_compres_scl;
    int sw_drc_position;
    int sw_drc_delta_scalein;
    int sw_drc_hpdetail_ratio;
    int sw_drc_lpdetail_ratio;
    int sw_drc_weicur_pix;
    int sw_drc_weipre_frame;
    int sw_drc_force_sgm_inv0;
    int sw_drc_motion_scl;
    int sw_drc_edge_scl;
    int sw_drc_space_sgm_inv1;
    int sw_drc_space_sgm_inv0;
    int sw_drc_range_sgm_inv1;
    int sw_drc_range_sgm_inv0;
    int sw_drc_weig_maxl;
    int sw_drc_weig_bilat;
    int sw_drc_gain_y[DRC_V10_Y_NUM];
    int sw_drc_compres_y[DRC_V10_Y_NUM];
    int sw_drc_scale_y[DRC_V10_Y_NUM];
    float sw_drc_adrc_gain;
    int sw_drc_iir_weight;
    int sw_drc_min_ogain;
} DrcProcResV10_t;

typedef struct DrcProcResV11_s {
    int bypass_en;
    int offset_pow2;
    int compres_scl;
    int position;
    int delta_scalein;
    int hpdetail_ratio;
    int lpdetail_ratio;
    int weicur_pix;
    int weipre_frame;
    int bilat_wt_off;
    int force_sgm_inv0;
    int motion_scl;
    int edge_scl;
    int space_sgm_inv1;
    int space_sgm_inv0;
    int range_sgm_inv1;
    int range_sgm_inv0;
    int weig_maxl;
    int weig_bilat;
    int enable_soft_thd;
    int bilat_soft_thd;
    int gain_y[DRC_V11_Y_NUM];
    int compres_y[DRC_V11_Y_NUM];
    int scale_y[DRC_V11_Y_NUM];
    float adrc_gain;
    int wr_cycle;
    int iir_weight;
    int min_ogain;
} DrcProcResV11_t;

typedef struct DrcProcResV12_s {
    unsigned char bypass_en;
    unsigned char offset_pow2;
    unsigned short compres_scl;
    unsigned short position;
    unsigned short hpdetail_ratio;
    unsigned short lpdetail_ratio;
    unsigned char delta_scalein;
    unsigned char weicur_pix;
    unsigned char weipre_frame;
    unsigned char bilat_wt_off;
    unsigned char edge_scl;
    unsigned char motion_scl;
    unsigned short force_sgm_inv0;
    unsigned short space_sgm_inv1;
    unsigned short space_sgm_inv0;
    unsigned short range_sgm_inv1;
    unsigned short range_sgm_inv0;
    unsigned short bilat_soft_thd;
    unsigned char weig_maxl;
    unsigned char weig_bilat;
    unsigned char enable_soft_thd;
    unsigned char iir_weight;
    unsigned short min_ogain;
    unsigned short gas_t;
    unsigned char gas_l0;
    unsigned char gas_l1;
    unsigned char gas_l2;
    unsigned char gas_l3;
    unsigned short gain_y[DRC_V12_Y_NUM];
    unsigned short compres_y[DRC_V12_Y_NUM];
    unsigned short scale_y[DRC_V12_Y_NUM];
} DrcProcResV12_t;

typedef struct DrcProcRes_s {
#if RKAIQ_HAVE_DRC_V10
    DrcProcResV10_t Drc_v10;
#endif
#if RKAIQ_HAVE_DRC_V11
    DrcProcResV11_t Drc_v11;
#endif
#if RKAIQ_HAVE_DRC_V12
    DrcProcResV12_t Drc_v12;
#endif
} DrcProcRes_t;

typedef struct RkAiqAdrcProcResult_s {
    bool update;
    bool bDrcEn;
    DrcProcRes_t DrcProcRes;
} RkAiqAdrcProcResult_t;

#endif  //_RK_AIQ_ALGO_ADRC_HW_H_
