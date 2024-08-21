/*
 * rk_aiq_algo_types.h
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

#ifndef _RK_AIQ_ALGO_TYPES_H_
#define _RK_AIQ_ALGO_TYPES_H_

#include "rk_aiq_algo_des.h"
#include "rk_aiq_types.h"
#include "ae/rk_aiq_types_ae_algo_int.h"
// forward declare
typedef struct _RkAiqAlgoPreResAe     RkAiqAlgoPreResAe;
typedef struct _RkAiqAlgoPreResAfd    RkAiqAlgoPreResAfd;
typedef struct _RkAiqAlgoPreResAwb    RkAiqAlgoPreResAwb;
typedef struct _RkAiqAlgoPreResAf     RkAiqAlgoPreResAf;
typedef struct _RkAiqAlgoPreResAsd    RkAiqAlgoPreResAsd;
typedef struct _RkAiqAlgoPreResAnr    RkAiqAlgoPreResAnr;
typedef struct _RkAiqAlgoPreResAhdr   RkAiqAlgoPreResAhdr;
typedef struct _RkAiqAlgoPreResAcp    RkAiqAlgoPreResAcp;
typedef struct _RkAiqAlgoPreResAsharp RkAiqAlgoPreResAsharp;
typedef struct _RkAiqAlgoPreResAdhaz  RkAiqAlgoPreResAdhaz;
typedef struct _RkAiqAlgoPreResA3dlut RkAiqAlgoPreResA3dlut;
typedef struct _RkAiqAlgoPreResAblc   RkAiqAlgoPreResAblc;
typedef struct _RkAiqAlgoPreResAccm   RkAiqAlgoPreResAccm;
typedef struct _RkAiqAlgoPreResAcgc   RkAiqAlgoPreResAcgc;
typedef struct _RkAiqAlgoPreResAdebayer RkAiqAlgoPreResAdebayer;
typedef struct _RkAiqAlgoPreResAdpcc  RkAiqAlgoPreResAdpcc;
typedef struct _RkAiqAlgoPreResAfec   RkAiqAlgoPreResAfec;
typedef struct _RkAiqAlgoPreResAgamma RkAiqAlgoPreResAgamma;
typedef struct _RkAiqAlgoPreResAdegamma RkAiqAlgoPreResAdegamma;
typedef struct _RkAiqAlgoPreResAgic   RkAiqAlgoPreResAgic;
typedef struct _RkAiqAlgoPreResAie    RkAiqAlgoPreResAie;
typedef struct _RkAiqAlgoPreResAldch  RkAiqAlgoPreResAldch;
typedef struct _RkAiqAlgoPreResAlsc   RkAiqAlgoPreResAlsc;
typedef struct _RkAiqAlgoPreResAorb   RkAiqAlgoPreResAorb;
typedef struct _RkAiqAlgoPreResAr2y   RkAiqAlgoPreResAr2y;
typedef struct _RkAiqAlgoPreResAwdr   RkAiqAlgoPreResAwdr;
typedef struct _RkAiqAlgoPreResAeis   RkAiqAlgoPreResAeis;

typedef struct _RkAiqAlgoProcResAe    RkAiqAlgoProcResAe;
typedef struct _RkAiqAlgoProcResAfd   RkAiqAlgoProcResAfd;
typedef struct _RkAiqAlgoProcResAwb   RkAiqAlgoProcResAwb;
typedef struct _RkAiqAlgoProcResAf    RkAiqAlgoProcResAf;
typedef struct _RkAiqAlgoProcResAsd   RkAiqAlgoProcResAsd;
typedef struct _RkAiqAlgoProcResAnr   RkAiqAlgoProcResAnr;
typedef struct _RkAiqAlgoProcResAhdr  RkAiqAlgoProcResAhdr;
typedef struct _RkAiqAlgoProcResAcp   RkAiqAlgoProcResAcp;
typedef struct _RkAiqAlgoProcResAsharp RkAiqAlgoProcResAsharp;
typedef struct _RkAiqAlgoProcResAdhaz RkAiqAlgoProcResAdhaz;
typedef struct _RkAiqAlgoProcResA3dlut RkAiqAlgoProcResA3dlut;
typedef struct _RkAiqAlgoProcResAblc  RkAiqAlgoProcResAblc;
typedef struct _RkAiqAlgoProcResAccm  RkAiqAlgoProcResAccm;
typedef struct _RkAiqAlgoProcResAcgc  RkAiqAlgoProcResAcgc;
typedef struct _RkAiqAlgoProcResAdebayer RkAiqAlgoProcResAdebayer;
typedef struct _RkAiqAlgoProcResAdpcc RkAiqAlgoProcResAdpcc;
typedef struct _RkAiqAlgoProcResAfec  RkAiqAlgoProcResAfec;
typedef struct _RkAiqAlgoProcResAgamma RkAiqAlgoProcResAgamma;
typedef struct _RkAiqAlgoProcResAdegamma RkAiqAlgoProcResAdegamma;
typedef struct _RkAiqAlgoProcResAgic  RkAiqAlgoProcResAgic;
typedef struct _RkAiqAlgoProcResAie   RkAiqAlgoProcResAie;
typedef struct _RkAiqAlgoProcResAldch RkAiqAlgoProcResAldch;
typedef struct _RkAiqAlgoProcResAlsc  RkAiqAlgoProcResAlsc;
typedef struct _RkAiqAlgoProcResAorb  RkAiqAlgoProcResAorb;
typedef struct _RkAiqAlgoProcResAr2y  RkAiqAlgoProcResAr2y;
typedef struct _RkAiqAlgoProcResAwdr  RkAiqAlgoProcResAwdr;
typedef struct _RkAiqAlgoProcResAeis  RkAiqAlgoProcResAeis;

typedef struct _RkAiqAlgoPostResAe    RkAiqAlgoPostResAe;
typedef struct _RkAiqAlgoPostResAfd   RkAiqAlgoPostResAfd;
typedef struct _RkAiqAlgoPostResAwb   RkAiqAlgoPostResAwb;
typedef struct _RkAiqAlgoPostResAf    RkAiqAlgoPostResAf;
typedef struct _RkAiqAlgoPostResAsd   RkAiqAlgoPostResAsd;
typedef struct _RkAiqAlgoPostResAnr   RkAiqAlgoPostResAnr;
typedef struct _RkAiqAlgoPostResAhdr  RkAiqAlgoPostResAhdr;
typedef struct _RkAiqAlgoPostResAcp   RkAiqAlgoPostResAcp;
typedef struct _RkAiqAlgoPostResAsharp RkAiqAlgoPostResAsharp;
typedef struct _RkAiqAlgoPostResAdhaz RkAiqAlgoPostResAdhaz;
typedef struct _RkAiqAlgoPostResA3dlut RkAiqAlgoPostResA3dlut;
typedef struct _RkAiqAlgoPostResAblc  RkAiqAlgoPostResAblc;
typedef struct _RkAiqAlgoPostResAccm  RkAiqAlgoPostResAccm;
typedef struct _RkAiqAlgoPostResAcgc  RkAiqAlgoPostResAcgc;
typedef struct _RkAiqAlgoPostResAdebayer RkAiqAlgoPostResAdebayer;
typedef struct _RkAiqAlgoPostResAdpcc RkAiqAlgoPostResAdpcc;
typedef struct _RkAiqAlgoPostResAfec  RkAiqAlgoPostResAfec;
typedef struct _RkAiqAlgoPostResAgamma RkAiqAlgoPostResAgamma;
typedef struct _RkAiqAlgoPostResAdegamma RkAiqAlgoPostResAdegamma;
typedef struct _RkAiqAlgoPostResAgic  RkAiqAlgoPostResAgic;
typedef struct _RkAiqAlgoPostResAie   RkAiqAlgoPostResAie;
typedef struct _RkAiqAlgoPostResAldch RkAiqAlgoPostResAldch;
typedef struct _RkAiqAlgoPostResAlsc  RkAiqAlgoPostResAlsc;
typedef struct _RkAiqAlgoPostResAorb  RkAiqAlgoPostResAorb;
typedef struct _RkAiqAlgoPostResAr2y  RkAiqAlgoPostResAr2y;
typedef struct _RkAiqAlgoPostResAwdr  RkAiqAlgoPostResAwdr;
typedef struct _RkAiqAlgoPostResAeis  RkAiqAlgoPostResAeis;

typedef struct _RkAiqPreResComb {
    RkAiqAlgoPreResAe*    ae_pre_res;
    RkAiqAlgoPreResAfd*   afd_pre_res;
    RkAiqAlgoPreResAwb*   awb_pre_res;
    RkAiqAlgoPreResAf*    af_pre_res;
    RkAiqAlgoPreResAsd*   asd_pre_res;
    RkAiqAlgoPreResAnr*   anr_pre_res;
    RkAiqAlgoPreResAhdr*  ahdr_pre_res;
    RkAiqAlgoPreResAcp* acp_pre_res;
    RkAiqAlgoPreResAsharp* asharp_pre_res;
    RkAiqAlgoPreResAdhaz* adhaz_pre_res;
    RkAiqAlgoPreResA3dlut* a3dlut_pre_res;
    RkAiqAlgoPreResAblc* ablc_pre_res;
    RkAiqAlgoPreResAccm* accm_pre_res;
    RkAiqAlgoPreResAcgc* acgc_pre_res;
    RkAiqAlgoPreResAdebayer* adebayer_pre_res;
    RkAiqAlgoPreResAdpcc* adpcc_pre_res;
    RkAiqAlgoPreResAfec* afec_pre_res;
    RkAiqAlgoPreResAgamma* agamma_pre_res;
    RkAiqAlgoPreResAdegamma* adegamma_pre_res;
    RkAiqAlgoPreResAgic* agic_pre_res;
    RkAiqAlgoPreResAie* aie_pre_res;
    RkAiqAlgoPreResAldch* aldch_pre_res;
    RkAiqAlgoPreResAlsc* alsc_pre_res;
    RkAiqAlgoPreResAorb* aorb_pre_res;
    RkAiqAlgoPreResAr2y* ar2y_pre_res;
    RkAiqAlgoPreResAwdr* awdr_pre_res;
    RkAiqAlgoPreResAeis* aeis_pre_res;
} RkAiqPreResComb;

typedef struct _RkAiqProcResComb {
    RkAiqAlgoProcResAe*    ae_proc_res;
    RkAiqAlgoProcResAfd*   afd_proc_res;
    RkAiqAlgoProcResAwb*   awb_proc_res;
    RkAiqAlgoProcResAf*    af_proc_res;
    RkAiqAlgoProcResAsd*   asd_proc_res;
    RkAiqAlgoProcResAnr*   anr_proc_res;
    RkAiqAlgoProcResAhdr*  ahdr_proc_res;
    RkAiqAlgoProcResAcp* acp_proc_res;
    RkAiqAlgoProcResAsharp* asharp_proc_res;
    RkAiqAlgoProcResAdhaz* adhaz_proc_res;
    RkAiqAlgoProcResA3dlut* a3dlut_proc_res;
    RkAiqAlgoProcResAblc* ablc_proc_res;
    RkAiqAlgoProcResAccm* accm_proc_res;
    RkAiqAlgoProcResAcgc* acgc_proc_res;
    RkAiqAlgoProcResAdebayer* adebayer_proc_res;
    RkAiqAlgoProcResAdpcc* adpcc_proc_res;
    RkAiqAlgoProcResAfec* afec_proc_res;
    RkAiqAlgoProcResAgamma* agamma_proc_res;
    RkAiqAlgoProcResAdegamma* adegamma_proc_res;
    RkAiqAlgoProcResAgic* agic_proc_res;
    RkAiqAlgoProcResAie* aie_proc_res;
    RkAiqAlgoProcResAldch* aldch_proc_res;
    RkAiqAlgoProcResAlsc* alsc_proc_res;
    RkAiqAlgoProcResAorb* aorb_proc_res;
    RkAiqAlgoProcResAr2y* ar2y_proc_res;
    RkAiqAlgoProcResAwdr* awdr_proc_res;
    RkAiqAlgoProcResAeis* aeis_proc_res;
} RkAiqProcResComb;

typedef struct _RkAiqPostResComb {
    RkAiqAlgoPostResAe*    ae_post_res;
    RkAiqAlgoPostResAfd*   afd_post_res;
    RkAiqAlgoPostResAwb*   awb_post_res;
    RkAiqAlgoPostResAf*    af_post_res;
    RkAiqAlgoPostResAsd*   asd_post_res;
    RkAiqAlgoPostResAnr*   anr_post_res;
    RkAiqAlgoPostResAhdr*  ahdr_post_res;
    RkAiqAlgoPostResAcp* acp_post_res;
    RkAiqAlgoPostResAsharp* asharp_post_res;
    RkAiqAlgoPostResAdhaz* adhaz_post_res;
    RkAiqAlgoPostResA3dlut* a3dlut_post_res;
    RkAiqAlgoPostResAblc* ablc_post_res;
    RkAiqAlgoPostResAccm* accm_post_res;
    RkAiqAlgoPostResAcgc* acgc_post_res;
    RkAiqAlgoPostResAdebayer* adebayer_post_res;
    RkAiqAlgoPostResAdpcc* adpcc_post_res;
    RkAiqAlgoPostResAfec* afec_post_res;
    RkAiqAlgoPostResAgamma* agamma_post_res;
    RkAiqAlgoPostResAdegamma* adegamma_post_res;
    RkAiqAlgoPostResAgic* agic_post_res;
    RkAiqAlgoPostResAie* aie_post_res;
    RkAiqAlgoPostResAldch* aldch_post_res;
    RkAiqAlgoPostResAlsc* alsc_post_res;
    RkAiqAlgoPostResAorb* aorb_post_res;
    RkAiqAlgoPostResAr2y* ar2y_post_res;
    RkAiqAlgoPostResAwdr* awdr_post_res;
    RkAiqAlgoPostResAeis* aeis_post_res;
} RkAiqPostResComb;

typedef struct _RkAiqAlgoComExt {
    union {
        struct {
        } prepare;

        struct {
            RkAiqPreResComb*  pre_res_comb;
            RkAiqProcResComb* proc_res_comb;
            RkAiqPostResComb* post_res_comb;
            bool fill_light_on;
            bool gray_mode;
            bool is_bw_sensor;
            int iso;
            RKAiqAecExpInfo_t *preExp;
            RKAiqAecExpInfo_t *curExp;
        } proc;
    } u;
} RkAiqAlgoComExt;

// Ae
typedef struct _RkAiqAlgoConfigAe {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
    /*params related to Hardware setting*/
    RkAiqAecHwConfig_t HwCtrl;
    /*params related to driver setting*/
    float        LinePeriodsPerField;
    float        PixelClockFreqMHZ;
    float        PixelPeriodsPerLine;
    rk_aiq_sensor_nr_switch_t nr_switch;
    rk_aiq_sensor_dcg_ratio_t dcg_ratio;
    uint32_t                 sony_brl;
} RkAiqAlgoConfigAe;

typedef struct _RkAiqAlgoPreAe {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
    RKAiqAecStats_t* ispAeStats;
} RkAiqAlgoPreAe;

typedef struct _RkAiqAlgoPreResAe {
    RkAiqAlgoResCom pre_res_com;
    AecPreResult_t ae_pre_res;
} RkAiqAlgoPreResAe;

typedef struct _RkAiqAlgoProcAe {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
    RKAiqAecStats_t* ispAeStats;
} RkAiqAlgoProcAe;

typedef struct _RkAiqAlgoProcResAe {
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_exposure_params_t cur_ae_exp;
    rk_aiq_exposure_params_t new_ae_exp;
    rk_aiq_isp_aec_meas_t ae_meas;
    rk_aiq_isp_hist_meas_t hist_meas;
    AecProcResult_t ae_proc_res;
} RkAiqAlgoProcResAe;

typedef struct _RkAiqAlgoPostAe {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoPostAe;

typedef struct _RkAiqAlgoPostResAe {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAe;

// Afd
typedef struct _RkAiqAlgoConfigAfd {
    RkAiqAlgoCom com;
    /*params related to driver setting*/
    float        LinePeriodsPerField;
    float        PixelClockFreqMHZ;
    float        PixelPeriodsPerLine;
} RkAiqAlgoConfigAfd;

typedef struct _RkAiqAlgoPreAfd {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAfd;

typedef struct _RkAiqAlgoPreResAfd {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAfd;

typedef struct _RkAiqAlgoProcAfd {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAfd;

typedef struct _RkAiqAlgoProcResAfd {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAfd;

typedef struct _RkAiqAlgoPostAfd {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAfd;

typedef struct _RkAiqAlgoPostResAfd {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAfd;

//Awb
typedef struct _RkAiqAlgoConfigAwb {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAwb;

typedef struct _RkAiqAlgoPreAwb {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAwb;

typedef struct _RkAiqAlgoPreResAwb {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAwb;

typedef struct _RkAiqAlgoProcAwb {
    RkAiqAlgoCom com;
    union {
        rk_aiq_awb_stat_res_v200_t awb_hw0_statis;
        rk_aiq_awb_stat_res_v201_t awb_hw1_statis;
    };
} RkAiqAlgoProcAwb;

typedef struct _RkAiqAlgoProcResAwb {
    RkAiqAlgoResCom proc_res_com;
    bool awb_cfg_update;
    union {
        rk_aiq_awb_stat_cfg_v200_t awb_hw0_para;
        rk_aiq_awb_stat_cfg_v201_t awb_hw1_para;
    };
    bool awb_gain_update;
    rk_aiq_wb_gain_t awb_gain_algo;
} RkAiqAlgoProcResAwb;

typedef struct _RkAiqAlgoPostAwb {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAwb;

typedef struct _RkAiqAlgoPostResAwb {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAwb;

// af
typedef struct _RkAiqAlgoConfigAf {
    RkAiqAlgoCom af_com;
    unsigned char af_mode;
    unsigned short win_h_offs;
    unsigned short win_v_offs;
    unsigned short win_h_size;
    unsigned short win_v_size;
    rk_aiq_lens_descriptor lens_des;
} RkAiqAlgoConfigAf;

typedef struct _RkAiqAlgoPreAf {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAf;

typedef struct _RkAiqAlgoPreResAf {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAf;

typedef struct _RkAiqAlgoProcAf {
    RkAiqAlgoCom com;
    rk_aiq_isp_af_stats_t *af_stats;
    rk_aiq_isp_aec_stats_t *aec_stats;
} RkAiqAlgoProcAf;

typedef struct _RkAiqAlgoProcResAf {
    bool af_cfg_update;
    bool lockae;
    bool lockae_en;
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_isp_af_meas_t af_isp_param;
    rk_aiq_af_focus_pos_meas_t af_focus_param;
} RkAiqAlgoProcResAf;

typedef struct _RkAiqAlgoPostAf {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAf;

typedef struct _RkAiqAlgoPostResAf {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAf;

// anr
typedef struct _RkAiqAlgoConfigAnr {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAnr ;

typedef struct _RkAiqAlgoPreAnr {
    RkAiqAlgoCom anr_com;
} RkAiqAlgoPreAnr;

typedef struct _RkAiqAlgoPreResAnr {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAnr;

typedef struct _RkAiqAlgoProcAnr {
    RkAiqAlgoCom anr_com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAnr;

typedef struct _RkAiqAlgoProcResAnr {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAnr;

typedef struct _RkAiqAlgoPostAnr {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAnr;

typedef struct _RkAiqAlgoPostResAnr {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAnr;

// asharp
typedef struct _RkAiqAlgoConfigAsharp {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAsharp ;

typedef struct _RkAiqAlgoPreAsharp {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAsharp;

typedef struct _RkAiqAlgoPreResAsharp {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAsharp;

typedef struct _RkAiqAlgoProcAsharp {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAsharp;

typedef struct _RkAiqAlgoProcResAsharp {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAsharp;

typedef struct _RkAiqAlgoPostAsharp {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAsharp;

typedef struct _RkAiqAlgoPostResAsharp {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAsharp;

// asd
typedef struct _RkAiqAlgoConfigAsd {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAsd ;

typedef struct _RkAiqAlgoPreAsd {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoPreAsd;

typedef struct _RkAiqAlgoPreResAsd {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAsd;

typedef struct _RkAiqAlgoProcAsd {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAsd;

typedef struct _RkAiqAlgoProcResAsd {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAsd;

typedef struct _RkAiqAlgoPostAsd {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAsd;

typedef struct _RkAiqAlgoPostResAsd {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAsd;

// ahdr
typedef struct _RkAiqAlgoConfigAhdr {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAhdr;

typedef struct _RkAiqAlgoPreAhdr {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAhdr;

typedef struct _RkAiqAlgoPreResAhdr {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAhdr;

typedef struct _RkAiqAlgoProcAhdr {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAhdr;

typedef struct _RkAiqAlgoProcResAhdr {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAhdr;

typedef struct _RkAiqAlgoPostAhdr {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAhdr;

typedef struct _RkAiqAlgoPostResAhdr {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAhdr;

// acp
typedef struct _RkAiqAlgoConfigAcp {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAcp;

typedef struct _RkAiqAlgoPreAcp {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAcp;

typedef struct _RkAiqAlgoPreResAcp {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAcp;

typedef struct _RkAiqAlgoProcAcp {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAcp;

typedef struct _RkAiqAlgoProcResAcp {
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_acp_params_t acp_res;
} RkAiqAlgoProcResAcp;

typedef struct _RkAiqAlgoPostAcp {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAcp;

typedef struct _RkAiqAlgoPostResAcp {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAcp;

//adehaze
typedef struct _RkAiqAlgoConfigAdhaz {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAdhaz;

typedef struct _RkAiqAlgoPreAdhaz {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAdhaz;

typedef struct _RkAiqAlgoPreResAdhaz {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAdhaz;

typedef struct _RkAiqAlgoProcAdhaz {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAdhaz;

typedef struct _RkAiqAlgoProcResAdhaz {
    RkAiqAlgoResCom proc_res_com;
    RKAiqAdhazConfig_t adhaz_config;
} RkAiqAlgoProcResAdhaz;

typedef struct _RkAiqAlgoPostAdhaz {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAdhaz;

typedef struct _RkAiqAlgoPostResAdhaz {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAdhaz;

// a3dlut
typedef struct _RkAiqAlgoConfigA3dlut {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigA3dlut;

typedef struct _RkAiqAlgoPreA3dlut {
    RkAiqAlgoCom com;
} RkAiqAlgoPreA3dlut;

typedef struct _RkAiqAlgoPreResA3dlut {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResA3dlut;

typedef struct _RkAiqAlgoProcA3dlut {
    RkAiqAlgoCom com;
} RkAiqAlgoProcA3dlut;

typedef struct _RkAiqAlgoProcResA3dlut {
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_lut3d_cfg_t lut3d_hw_conf;
} RkAiqAlgoProcResA3dlut;

typedef struct _RkAiqAlgoPostA3dlut {
    RkAiqAlgoCom com;
} RkAiqAlgoPostA3dlut;

typedef struct _RkAiqAlgoPostResA3dlut {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResA3dlut;

// ablc
typedef struct _RkAiqAlgoConfigAblc {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAblc;

typedef struct _RkAiqAlgoPreAblc {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAblc;

typedef struct _RkAiqAlgoPreResAblc {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAblc;

typedef struct _RkAiqAlgoProcAblc {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAblc;

typedef struct _RkAiqAlgoProcResAblc {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAblc;

typedef struct _RkAiqAlgoPostAblc {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAblc;

typedef struct _RkAiqAlgoPostResAblc {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAblc;

// accm
typedef struct _RkAiqAlgoConfigAccm {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAccm;

typedef struct _RkAiqAlgoPreAccm {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAccm;

typedef struct _RkAiqAlgoPreResAccm {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAccm;

typedef struct _RkAiqAlgoProcAccm {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAccm;

typedef struct _RkAiqAlgoProcResAccm {
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_ccm_cfg_t accm_hw_conf;
} RkAiqAlgoProcResAccm;

typedef struct _RkAiqAlgoPostAccm {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAccm;

typedef struct _RkAiqAlgoPostResAccm {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAccm;

// cgc
typedef struct _RkAiqAlgoConfigAcgc {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAcgc;

typedef struct _RkAiqAlgoPreAcgc {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAcgc;


typedef struct _RkAiqAlgoPreResAcgc {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAcgc;

typedef struct _RkAiqAlgoProcAcgc {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAcgc;

typedef struct _RkAiqAlgoProcResAcgc {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAcgc;

typedef struct _RkAiqAlgoPostAcgc {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAcgc;

typedef struct _RkAiqAlgoPostResAcgc {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAcgc;

// adebayer
typedef struct _RkAiqAlgoConfigAdebayer {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAdebayer;

typedef struct _RkAiqAlgoPreAdebayer {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAdebayer;

typedef struct _RkAiqAlgoPreResAdebayer {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAdebayer;

typedef struct _RkAiqAlgoProcAdebayer {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAdebayer;

typedef struct _RkAiqAlgoProcResAdebayer {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAdebayer;

typedef struct _RkAiqAlgoPostAdebayer {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAdebayer;

typedef struct _RkAiqAlgoPostResAdebayer {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAdebayer;

// adpcc
typedef struct _RkAiqAlgoConfigAdpcc {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAdpcc;

typedef struct _RkAiqAlgoPreAdpcc {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAdpcc;

typedef struct _RkAiqAlgoPreResAdpcc {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAdpcc;

typedef struct _RkAiqAlgoProcAdpcc {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAdpcc;

typedef struct _RkAiqAlgoProcResAdpcc {
    RkAiqAlgoResCom proc_res_com;
    Sensor_dpcc_res_t SenDpccRes;
} RkAiqAlgoProcResAdpcc;

typedef struct _RkAiqAlgoPostAdpcc {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAdpcc;

typedef struct _RkAiqAlgoPostResAdpcc {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAdpcc;

// afec
typedef struct _RkAiqAlgoConfigAfec {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAfec;

typedef struct _RkAiqAlgoPreAfec {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAfec;

typedef struct _RkAiqAlgoPreResAfec {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAfec;

typedef struct _RkAiqAlgoProcAfec {
    RkAiqAlgoCom com;
    uint32_t frame_id;
} RkAiqAlgoProcAfec;

typedef struct _RkAiqAlgoProcResAfec {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAfec;

typedef struct _RkAiqAlgoPostAfec {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAfec;

typedef struct _RkAiqAlgoPostResAfec {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAfec;

// agamma
typedef struct _RkAiqAlgoConfigAgamma {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAgamma;

typedef struct _RkAiqAlgoPreAgamma {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAgamma;

typedef struct _RkAiqAlgoPreResAgamma {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAgamma;

typedef struct _RkAiqAlgoProcAgamma {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAgamma;

typedef struct _RkAiqAlgoProcResAgamma {
    RkAiqAlgoResCom proc_res_com;
    AgammaProcRes_t agamma_proc_res;
} RkAiqAlgoProcResAgamma;

typedef struct _RkAiqAlgoPostAgamma {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAgamma;

typedef struct _RkAiqAlgoPostResAgamma {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAgamma;

// adegamma
typedef struct _RkAiqAlgoConfigAdegamma {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAdegamma;

typedef struct _RkAiqAlgoPreAdegamma {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAdegamma;

typedef struct _RkAiqAlgoPreResAdegamma {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAdegamma;

typedef struct _RkAiqAlgoProcAdegamma {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAdegamma;

typedef struct _RkAiqAlgoProcResAdegamma {
    RkAiqAlgoResCom proc_res_com;
    AdegammaProcRes_t adegamma_proc_res;
} RkAiqAlgoProcResAdegamma;

typedef struct _RkAiqAlgoPostAdegamma {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAdegamma;

typedef struct _RkAiqAlgoPostResAdegamma {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAdegamma;

// agic
typedef struct _RkAiqAlgoConfigAgic {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAgic;

typedef struct _RkAiqAlgoPreAgic {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAgic;

typedef struct _RkAiqAlgoPreResAgic {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAgic;

typedef struct _RkAiqAlgoProcAgic {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoProcAgic;

typedef struct _RkAiqAlgoProcResAgic {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAgic;

typedef struct _RkAiqAlgoPostAgic {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAgic;

typedef struct _RkAiqAlgoPostResAgic {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAgic;

// aie
typedef struct _RkAiqAlgoConfigAie {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAie;

typedef struct _RkAiqAlgoPreAie {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAie;

typedef struct _RkAiqAlgoPreResAie {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAie;

typedef struct _RkAiqAlgoProcAie {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAie;

typedef struct _RkAiqAlgoProcResAie {
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_aie_params_t params;
} RkAiqAlgoProcResAie;

typedef struct _RkAiqAlgoPostAie {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAie;

typedef struct _RkAiqAlgoPostResAie {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAie;

// aldch
typedef struct _RkAiqAlgoConfigAldch {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAldch;

typedef struct _RkAiqAlgoPreAldch {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAldch;

typedef struct _RkAiqAlgoPreResAldch {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAldch;

typedef struct _RkAiqAlgoProcAldch {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAldch;

typedef struct _RkAiqAlgoProcResAldch {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAldch;

typedef struct _RkAiqAlgoPostAldch {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAldch;

typedef struct _RkAiqAlgoPostResAldch {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAldch;

// alsc
typedef struct _RkAiqAlgoConfigAlsc {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAlsc;

typedef struct _RkAiqAlgoPreAlsc {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAlsc;

typedef struct _RkAiqAlgoPreResAlsc {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAlsc;

typedef struct _RkAiqAlgoProcAlsc {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAlsc;

typedef struct _RkAiqAlgoProcResAlsc {
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_lsc_cfg_t alsc_hw_conf;
} RkAiqAlgoProcResAlsc;

typedef struct _RkAiqAlgoPostAlsc {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAlsc;

typedef struct _RkAiqAlgoPostResAlsc {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAlsc;

// aorb
typedef struct _RkAiqAlgoConfigAorb {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAorb;

typedef struct _RkAiqAlgoPreAorb {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAorb;

typedef struct _RkAiqAlgoPreResAorb {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAorb;

typedef struct _RkAiqAlgoProcAorb {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAorb;

typedef struct _RkAiqAlgoProcResAorb {
    RkAiqAlgoResCom proc_res_com;
    rk_aiq_orb_algo_meas_t aorb_meas;
} RkAiqAlgoProcResAorb;

typedef struct _RkAiqAlgoPostAorb {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAorb;

typedef struct _RkAiqAlgoPostResAorb {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAorb;

// ar2y
typedef struct _RkAiqAlgoConfigAr2y {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAr2y;

typedef struct _RkAiqAlgoPreAr2y {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAr2y;

typedef struct _RkAiqAlgoPreResAr2y {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAr2y;

typedef struct _RkAiqAlgoProcAr2y {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAr2y;

typedef struct _RkAiqAlgoProcResAr2y {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAr2y;

typedef struct _RkAiqAlgoPostAr2y {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAr2y;

typedef struct _RkAiqAlgoPostResAr2y {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAr2y;

// awdr
typedef struct _RkAiqAlgoConfigAwdr {
    RkAiqAlgoCom com;
} RkAiqAlgoConfigAwdr;

typedef struct _RkAiqAlgoPreAwdr {
    RkAiqAlgoCom com;
} RkAiqAlgoPreAwdr;

typedef struct _RkAiqAlgoPreResAwdr {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAwdr;

typedef struct _RkAiqAlgoProcAwdr {
    RkAiqAlgoCom com;
} RkAiqAlgoProcAwdr;

typedef struct _RkAiqAlgoProcResAwdr {
    RkAiqAlgoResCom proc_res_com;
    RkAiqAwdrProcResult_t AwdrProcRes;
} RkAiqAlgoProcResAwdr;

typedef struct _RkAiqAlgoPostAwdr {
    RkAiqAlgoCom com;
} RkAiqAlgoPostAwdr;

typedef struct _RkAiqAlgoPostResAwdr {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAwdr;

// aeis
typedef struct _RkAiqAlgoConfigAeis {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
    const rk_aiq_mems_sensor_intf_t *mems_sensor_intf;
} RkAiqAlgoConfigAeis;

typedef struct _RkAiqAlgoPreAeis {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoPreAeis;

typedef struct _RkAiqAlgoPreResAeis {
    RkAiqAlgoResCom pre_res_com;
} RkAiqAlgoPreResAeis;

typedef struct _RkAiqAlgoProcAeis {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
    int32_t frame_id;
    int64_t sof;
    double rolling_shutter_skew;
    float integration_time;
    int analog_gain;
    uint32_t frame_width;
    uint32_t frame_height;
} RkAiqAlgoProcAeis;

typedef struct _RkAiqAlgoProcResAeis {
    RkAiqAlgoResCom proc_res_com;
} RkAiqAlgoProcResAeis;

typedef struct _RkAiqAlgoPostAeis {
    RkAiqAlgoCom com;
    RkAiqAlgoComExt com_ext;
} RkAiqAlgoPostAeis;

typedef struct _RkAiqAlgoPostResAeis {
    RkAiqAlgoResCom post_res_com;
} RkAiqAlgoPostResAeis;

#endif
