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

#ifndef _RK_AIQ_USER_API_CUSTOM_AWB_TYPE_V32_H_
#define _RK_AIQ_USER_API_CUSTOM_AWB_TYPE_V32_H_

#include "rk_aiq_types.h" /* common structs */
#include "awb/rk_aiq_types_awb_stat_v32.h" /* include awb structs*/

RKAIQ_BEGIN_DECLARE

/* all awb stats */
typedef struct rk_aiq_customAwb_stats_s
{
    rk_aiq_awb_stat_wp_res_light_v201_t light[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];
    int WpNo2[RK_AIQ_AWB_MAX_WHITEREGIONS_NUM_V32];
    rk_aiq_awb_stat_blk_res_v201_t   blockResult[RK_AIQ_AWB_GRID_NUM_TOTAL];
    rk_aiq_awb_stat_wp_res_v201_t excWpRangeResult[RK_AIQ_AWB_STAT_WP_RANGE_NUM_V201];
    unsigned int WpNoHist[RK_AIQ_AWB_WP_HIST_BIN_NUM];
    struct rk_aiq_customAwb_stats_s* next;
} rk_aiq_customAwb_stats_t;

typedef enum rk_aiq_customAwb_Select_Mode_s {
    CUSTOM_AWB_INPUT_RAW_SHORT = 0,
    CUSTOM_AWB_INPUT_RAW_LONG = 1,
    CUSTOM_AWB_INPUT_BAYERNR = 3,
    CUSTOM_AWB_INPUT_DRC = 4,
}rk_aiq_customAwb_Raw_Select_Mode_e;

typedef struct rk_aiq_customAwb_hw_cfg_s {
    bool awbEnable;
    rk_aiq_customAwb_Raw_Select_Mode_e frameChoose;
    unsigned short windowSet[4];
    unsigned char lightNum;
    unsigned short maxR;
    unsigned short minR;
    unsigned short maxG;
    unsigned short minG;
    unsigned short maxB;
    unsigned short minB;
    unsigned short maxY;
    unsigned short minY;
    bool multiwindow_en;
    unsigned short multiwindow[RK_AIQ_AWB_MULTIWINDOW_NUM_V201][4];
} rk_aiq_customAwb_hw_cfg_t;

typedef struct rk_aiq_customAwb_single_hw_cfg_t {
    unsigned short windowSet[4];
    bool multiwindow_en;
    unsigned short multiwindow[RK_AIQ_AWB_MULTIWINDOW_NUM_V201][4];
} rk_aiq_customAwb_single_hw_cfg_t;


/* different awb result for each camera */
typedef struct rk_aiq_customeAwb_single_results_s
{
    rk_aiq_wb_gain_t awb_gain_algo;//for each camera
    rk_aiq_customAwb_single_hw_cfg_t  awbHwConfig;//for each camera
    struct rk_aiq_customeAwb_single_results_s *next;
} rk_aiq_customeAwb_single_results_t;


/* full awb results */
typedef struct rk_aiq_customeAwb_results_s
{
    bool  IsConverged; //true: converged; false: not converged
    rk_aiq_wb_gain_t awb_gain_algo;
    float awb_smooth_factor;
    rk_aiq_customAwb_hw_cfg_t  awbHwConfig;
    rk_aiq_customeAwb_single_results_t *next;//defalut vaue is nullptr,which means all cameras with the same cfg;
} rk_aiq_customeAwb_results_t;



RKAIQ_END_DECLARE

#endif

