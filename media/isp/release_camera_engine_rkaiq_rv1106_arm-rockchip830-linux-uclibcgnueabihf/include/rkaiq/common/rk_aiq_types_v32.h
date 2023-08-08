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
#ifndef _RK_AIQ_TYPES_V32_H_
#define _RK_AIQ_TYPES_V32_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "abayer2dnrV23/rk_aiq_types_abayer2dnr_algo_v23.h"
#include "abayertnrV23/rk_aiq_types_abayertnr_algo_v23.h"
#include "ablcV32/rk_aiq_types_ablc_algo_int_v32.h"
#include "acnrV30/rk_aiq_types_acnr_algo_v30.h"
#include "adebayer/rk_aiq_types_adebayer_algo.h"
#include "algos/acac/rk_aiq_types_acac_algo.h"
#include "asharpV33/rk_aiq_types_asharp_algo_v33.h"
#include "aynrV22/rk_aiq_types_aynr_algo_v22.h"
#include "rk_aiq_comm.h"
#include "rk_aiq_mems_sensor.h"

#ifndef ISP32_LDCH_BIC_NUM
#define ISP32_LDCH_BIC_NUM		36
#endif

// typedef struct rk_aiq_isp_blc_v32_s {
// } rk_aiq_isp_blc_v32_t;

typedef RkAiqAmergeProcResult_t rk_aiq_isp_merge_v32_t;

typedef RkAiqAdrcProcResult_t rk_aiq_isp_drc_v32_t;

typedef struct rk_aiq_isp_bay3d_v32_s {
} rk_aiq_isp_bay3d_v32_t;

typedef struct rk_aiq_isp_cac_v32_s {
    bool enable;
    rkaiq_cac_v11_hw_param_t cfg;
} rk_aiq_isp_cac_v32_t;


typedef AdebayerHwConfigV2_t rk_aiq_isp_debayer_v32_t;

typedef struct rk_aiq_isp_ccm_v32_s {
} rk_aiq_isp_ccm_v32_t;

typedef RkAiqAdehazeProcResult_t rk_aiq_isp_dehaze_v32_t;

typedef struct rk_aiq_isp_ldch_v21_s {
    int32_t ldch_en;
    int32_t lut_h_size;
    int32_t lut_v_size;
    int32_t lut_size;
    uint32_t lut_mem_fd;

    uint8_t frm_end_dis;
    uint8_t zero_interp_en;
    uint8_t sample_avr_en;
    uint8_t bic_mode_en;
    uint8_t force_map_en;
    uint8_t map13p3_en;

    uint8_t bicubic[ISP32_LDCH_BIC_NUM];
} rk_aiq_isp_ldch_v21_t;

typedef RK_YNR_Fix_V22_t rk_aiq_isp_ynr_v32_t ;


typedef RK_CNR_Fix_V30_t rk_aiq_isp_cnr_v32_t ;

typedef RK_SHARP_Fix_V33_t rk_aiq_isp_sharp_v32_t;

typedef AblcProc_V32_t rk_aiq_isp_blc_v32_t;

typedef rk_aiq_awb_stat_cfg_v32_t rk_aiq_isp_awb_meas_cfg_v32_t;
typedef rk_aiq_awb_stat_res_v32_t rk_aiq_isp_awb_stats_v32_t;


typedef RK_Bayer2dnr_Fix_V23_t rk_aiq_isp_baynr_v32_t;
typedef RK_Bayertnr_Fix_V23_t rk_aiq_isp_tnr_v32_t ;

typedef rk_aiq_af_algo_meas_v31_t rk_aiq_isp_af_v31_t;

#endif
