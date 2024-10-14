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
#ifndef _RK_AIQ_TYPES_V21_H_
#define _RK_AIQ_TYPES_V21_H_

#include "rk_aiq_types_v20.h"


typedef rk_aiq_awb_stat_cfg_v201_t rk_aiq_isp_awb_meas_cfg_v21_t;
typedef RkAiqAdrcProcResult_t rk_aiq_isp_drc_v21_t;

typedef struct rk_aiq_isp_blc_v21_s {
    rk_aiq_isp_blc_t v0;
    //TODO: additional blc1 params
} rk_aiq_isp_blc_v21_t;

typedef struct rk_aiq_isp_wb_gain_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_wb_gain_v21_t;

typedef AgicProcResult_t rk_aiq_isp_gic_v21_t;

typedef struct rk_aiq_isp_ccm_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_ccm_v21_t;
/*
typedef struct rk_aiq_isp_dhaz_cfg_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_dhaz_cfg_v21_t;*/

typedef RkAiqAdehazeProcResult_t rk_aiq_isp_dehaze_v21_t;

typedef struct rk_aiq_isp_dhaz_stats_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_dhaz_stats_v21_t;

// baynr, alias name of rawnr
#if 0
typedef struct rk_aiq_isp_baynr_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_baynr_v21_t;
#else
typedef RK_Bayernr_Fix_V2_t rk_aiq_isp_baynr_v21_t ;

#endif
typedef struct rk_aiq_isp_bay3d_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_bay3d_v21_t;

#if 0
typedef struct rk_aiq_isp_ynr_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_ynr_v21_t;
#else
typedef RK_YNR_Fix_V2_t rk_aiq_isp_ynr_v21_t ;

#endif

#if 0
typedef struct rk_aiq_isp_cnr_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_cnr_v21_t;
#else
typedef RK_CNR_Fix_V1_t rk_aiq_isp_cnr_v21_t ;

#endif

#if 0
typedef struct rk_aiq_isp_sharp_v21_s {
    //TODO:
    void* place_holder;
} rk_aiq_isp_sharp_v21_t;
#else
typedef RK_SHARP_Fix_V3_t rk_aiq_isp_sharp_v21_t ;

#endif

typedef struct rk_aiq_awb_stat_res_v201_s rk_aiq_isp_raw_awb_meas_v21_t;

#endif