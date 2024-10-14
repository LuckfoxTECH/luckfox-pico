/*
 * Copyright (c) 2021-2022 Rockchip Eletronics Co., Ltd.
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
 */
#ifndef _RK_AIQ_TYPES_V20_H_
#define _RK_AIQ_TYPES_V20_H_
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "rk_aiq_comm.h"
#include "rk_aiq_mems_sensor.h"
#include "common/rk-camera-module.h"
#include "adebayer/rk_aiq_types_adebayer_algo.h"
#include "ae/rk_aiq_types_ae_algo.h"
#include "afd/rk_aiq_types_afd_algo.h"
#include "awb/rk_aiq_types_awb_algo.h"
#include "alsc/rk_aiq_types_alsc_algo.h"
#include "accm/rk_aiq_types_accm_algo.h"
#include "a3dlut/rk_aiq_types_a3dlut_algo.h"
#include "amerge/rk_aiq_types_amerge_algo_int.h"
#include "amerge/rk_aiq_types_amerge_algo.h"
#include "atmo/rk_aiq_types_atmo_algo_int.h"
#include "atmo/rk_aiq_types_atmo_algo.h"
#include "adrc/rk_aiq_types_adrc_algo_int.h"
#include "adrc/rk_aiq_types_adrc_algo.h"
#include "agamma/rk_aiq_types_agamma_algo_int.h"
#include "adegamma/rk_aiq_types_adegamma_algo_int.h"
#include "adehaze/rk_aiq_types_adehaze_algo_int.h"
#include "adehaze/rk_aiq_types_adehaze_algo.h"
#include "acp/rk_aiq_types_acp_algo.h"
#include "aie/rk_aiq_types_aie_algo_int.h"
#include "aorb/rk_aiq_types_orb_algo.h"
#include "asd/rk_aiq_types_asd_algo.h"
#include "anr/rk_aiq_types_anr_algo.h"
#include "anr/rk_aiq_types_anr_algo_int.h"
#include "asharp/rk_aiq_types_asharp_algo.h"
#include "asharp/rk_aiq_types_asharp_algo_int.h"
#include "adpcc/rk_aiq_types_adpcc_algo.h"
#include "adpcc/rk_aiq_types_adpcc_algo_int.h"
#include "ablc/rk_aiq_types_ablc_algo.h"
#include "ablc/rk_aiq_types_ablc_algo_int.h"
#include "agic/rk_aiq_types_algo_agic_int.h"
#include "aldch/rk_aiq_types_aldch_algo.h"
#include "afec/rk_aiq_types_afec_algo.h"
#include "asharp3/rk_aiq_types_asharp_algo_v3.h"
#include "aynr2/rk_aiq_types_aynr_algo_v2.h"
#include "acnr/rk_aiq_types_acnr_algo_v1.h"
#include "arawnr2/rk_aiq_types_abayernr_algo_v2.h"
#include "amd/rk_aiq_types_amd_algo.h"
#include "arawnr/rk_aiq_types_abayernr_algo_v1.h"
#include "amfnr/rk_aiq_types_amfnr_algo_v1.h"
#include "aynr/rk_aiq_types_aynr_algo_v1.h"
#include "auvnr/rk_aiq_types_auvnr_algo_v1.h"
#include "again/rk_aiq_types_again_algo.h"
#include "acsm/rk_aiq_types_acsm_algo.h"
#include "acgc/rk_aiq_types_acgc_algo.h"

#ifdef RK_SIMULATOR_HW
#include "simulator/isp20_hw_simulator.h"
#include "af/rk_aiq_af_hw_v200.h"
#else
#include "af/rk_aiq_types_af_algo.h"
#endif


typedef rk_aiq_ae_meas_params_t rk_aiq_isp_aec_meas_t;
typedef rk_aiq_hist_meas_params_t rk_aiq_isp_hist_meas_t;
typedef rk_aiq_awb_stat_cfg_v200_t rk_aiq_isp_awb_meas_cfg_t;
typedef rk_aiq_wb_gain_t rk_aiq_isp_wb_gain_t;
typedef rk_aiq_af_algo_meas_v20_t rk_aiq_isp_af_meas_t;
typedef AdpccProcResult_t rk_aiq_isp_dpcc_t;
typedef RkAiqAmergeProcResult_t rk_aiq_isp_merge_t;
typedef RkAiqAtmoProcResult_t rk_aiq_isp_tmo_t;
typedef rk_aiq_ccm_cfg_t rk_aiq_isp_ccm_t;
typedef rk_aiq_lsc_cfg_t rk_aiq_isp_lsc_t;
typedef AblcProc_t rk_aiq_isp_blc_t;

#if ANR_NO_SEPERATE_MARCO
typedef RKAnr_Bayernr_Fix_t rk_aiq_isp_rawnr_t;
typedef RKAnr_Mfnr_Fix_t rk_aiq_isp_tnr_t;
typedef RKAnr_Ynr_Fix_t rk_aiq_isp_ynr_t;
typedef RKAnr_Uvnr_Fix_t rk_aiq_isp_uvnr_t;
typedef ANRProcResult_t rkaiq_anr_procRes_t;
#else
typedef RK_Bayernr_Fix_V1_t rk_aiq_isp_rawnr_t;
typedef RK_MFNR_Fix_V1_t rk_aiq_isp_tnr_t;
typedef RK_YNR_Fix_V1_t rk_aiq_isp_ynr_t;
typedef RK_UVNR_Fix_V1_t rk_aiq_isp_uvnr_t;
typedef ANRProcResult_t rkaiq_anr_procRes_t;
#endif

typedef AgicProcResult_t rk_aiq_isp_gic_t;
typedef AdebayerHwConfigV1_t rk_aiq_isp_debayer_t;
typedef ldch_process_result_t rk_aiq_isp_ldch_t;
typedef rk_aiq_lut3d_cfg_t rk_aiq_isp_lut3d_t;
typedef RkAiqAdehazeProcResult_t rk_aiq_isp_dehaze_t;
typedef AgammaProcRes_t rk_aiq_isp_gamma_t;
typedef AdegammaProcRes_t rk_aiq_isp_degamma_t;

typedef struct {
    int UNKNOWN;
} rk_aiq_isp_wdr_t;

typedef rk_aiq_acsm_params_t rk_aiq_isp_csm_t;
typedef rk_aiq_acgc_params_t rk_aiq_isp_cgc_t;

typedef struct {
    int UNKNOWN;
} rk_aiq_isp_conv422_t;

typedef struct {
    int UNKNOWN;
} rk_aiq_isp_yuvconv_t;

typedef RKAnr_Gain_Fix_t rk_aiq_isp_gain_t;
typedef rk_aiq_acp_params_t rk_aiq_isp_acp_t;

typedef struct rk_aiq_isp_ie_s {
    rk_aiq_aie_params_t base;
    rk_aiq_aie_params_int_t extra;
} rk_aiq_isp_ie_t;

typedef CalibDb_MFNR_Motion_t rk_aiq_isp_motion_t;
typedef rk_aiq_amd_params_t rk_aiq_isp_amd_t;

typedef RKAsharp_Sharp_Fix_t rk_aiq_isp_sharpen_t;
typedef RKAsharp_Edgefilter_Fix_t rk_aiq_isp_edgeflt_t;
typedef fec_preprocess_result_t rk_aiq_isp_fec_t;

typedef struct {
    unsigned char orb_en;
    unsigned char limit_value;
    unsigned int max_feature;
} rk_aiq_isp_orb_t;

typedef rk_aiq_afd_cfg_t rk_aiq_isp_afd_t;

typedef struct rk_aiq_awb_stat_res_v200_s rk_aiq_isp_raw_awb_meas_t;

#endif