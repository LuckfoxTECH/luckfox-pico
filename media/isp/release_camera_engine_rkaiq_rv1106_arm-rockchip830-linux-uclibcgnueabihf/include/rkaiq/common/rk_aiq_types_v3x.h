/*
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef _RK_AIQ_TYPES_V3X_H_
#define _RK_AIQ_TYPES_V3X_H_

/*
 * @file
 * @brief this file describle the isp v3 structs, will be included by rk_aiq_types.h
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "a3dlut/rk_aiq_types_a3dlut_algo.h"
#include "abayer2dnr2/rk_aiq_types_abayer2dnr_algo_v2.h"
#include "abayertnr2/rk_aiq_types_abayertnr_algo_v2.h"
#include "ablc/rk_aiq_types_ablc_algo.h"
#include "ablc/rk_aiq_types_ablc_algo_int.h"
#include "acac/rk_aiq_types_acac_algo.h"
#include "accm/rk_aiq_types_accm_algo.h"
#include "acnr/rk_aiq_types_acnr_algo_v1.h"
#include "acnr2/rk_aiq_types_acnr_algo_v2.h"
#include "acp/rk_aiq_types_acp_algo.h"
#include "adebayer/rk_aiq_types_adebayer_algo.h"
#include "adegamma/rk_aiq_types_adegamma_algo_int.h"
#include "adehaze/rk_aiq_types_adehaze_algo.h"
#include "adehaze/rk_aiq_types_adehaze_algo_int.h"
#include "adpcc/rk_aiq_types_adpcc_algo.h"
#include "adpcc/rk_aiq_types_adpcc_algo_int.h"
#include "adrc/rk_aiq_types_adrc_algo.h"
#include "adrc/rk_aiq_types_adrc_algo_int.h"
#include "ae/rk_aiq_types_ae_algo.h"
#include "af/rk_aiq_types_af_algo.h"
#include "afec/rk_aiq_types_afec_algo.h"
#include "again/rk_aiq_types_again_algo.h"
#include "again2/rk_aiq_types_again_algo_v2.h"
#include "agamma/rk_aiq_types_agamma_algo.h"
#include "agamma/rk_aiq_types_agamma_algo_int.h"
#include "agic/rk_aiq_types_algo_agic_int.h"
#include "aie/rk_aiq_types_aie_algo_int.h"
#include "aldch/rk_aiq_types_aldch_algo.h"
#include "alsc/rk_aiq_types_alsc_algo.h"
#include "amd/rk_aiq_types_amd_algo.h"
#include "amerge/rk_aiq_types_amerge_algo.h"
#include "amerge/rk_aiq_types_amerge_algo_int.h"
#include "amfnr/rk_aiq_types_amfnr_algo_v1.h"
#include "anr/rk_aiq_types_anr_algo.h"
#include "anr/rk_aiq_types_anr_algo_int.h"
#include "aorb/rk_aiq_types_orb_algo.h"
#include "arawnr/rk_aiq_types_abayernr_algo_v1.h"
#include "arawnr2/rk_aiq_types_abayernr_algo_v2.h"
#include "asd/rk_aiq_types_asd_algo.h"
#include "asharp/rk_aiq_types_asharp_algo.h"
#include "asharp/rk_aiq_types_asharp_algo_int.h"
#include "asharp3/rk_aiq_types_asharp_algo_v3.h"
#include "asharp4/rk_aiq_types_asharp_algo_v4.h"
#include "atmo/rk_aiq_types_atmo_algo.h"
#include "atmo/rk_aiq_types_atmo_algo_int.h"
#include "auvnr/rk_aiq_types_auvnr_algo_v1.h"
#include "awb/rk_aiq_types_awb_algo.h"
#include "aynr/rk_aiq_types_aynr_algo_v1.h"
#include "aynr2/rk_aiq_types_aynr_algo_v2.h"
#include "aynr3/rk_aiq_types_aynr_algo_v3.h"
#include "rk_aiq_comm.h"
#include "rk_aiq_mems_sensor.h"

//typedef struct rk_aiq_isp_blc_v3x_s {
//} rk_aiq_isp_blc_v3x_t;

//typedef rk_aiq_isp_blc_v21_t rk_aiq_isp_blc_v3x_t;

typedef AgammaProcRes_t rk_aiq_isp_goc_v3x_t;

typedef RkAiqAdrcProcResult_t rk_aiq_isp_drc_v3x_t;

typedef RkAiqAmergeProcResult_t rk_aiq_isp_merge_v3x_t;

typedef RkAiqAdehazeProcResult_t rk_aiq_isp_dehaze_v3x_t;


typedef RK_Bayer2dnr_Fix_V2_t rk_aiq_isp_baynr_v3x_t ;

typedef RK_Bayertnr_Fix_V2_t rk_aiq_isp_tnr_v3x_t ;

typedef RK_YNR_Fix_V3_t rk_aiq_isp_ynr_v3x_t ;

typedef RK_CNR_Fix_V2_t rk_aiq_isp_cnr_v3x_t ;

typedef RK_SHARP_Fix_V4_t rk_aiq_isp_sharp_v3x_t ;

typedef struct rk_aiq_isp_bay3d_v3x_s {
} rk_aiq_isp_bay3d_v3x_t;


typedef struct rk_aiq_isp_cac_v3x_s {
    bool enable;
    rkaiq_cac_v10_hw_param_t cfg[2];
} rk_aiq_isp_cac_v3x_t;

typedef RK_GAIN_Fix_V2_t rk_aiq_isp_gain_v3x_t;


typedef rk_aiq_awb_stat_cfg_v201_t rk_aiq_isp_awb_meas_cfg_v3x_t;
typedef rk_aiq_awb_stat_res_v201_t rk_aiq_isp_awb_stats_v3x_t;
typedef rk_aiq_awb_stat_res2_v30_t rk_aiq_isp_awb_stats2_v3x_t;

typedef rk_aiq_af_algo_stat_v30_t rk_aiq_isp_af_stats_v3x_t;
typedef rk_aiq_af_algo_meas_v30_t rk_aiq_isp_af_meas_v3x_t;

typedef struct rk_aiq_isp_dehaze_stats_v3x_s {
} rk_aiq_isp_dehaze_stats_v3x_t;

#endif
