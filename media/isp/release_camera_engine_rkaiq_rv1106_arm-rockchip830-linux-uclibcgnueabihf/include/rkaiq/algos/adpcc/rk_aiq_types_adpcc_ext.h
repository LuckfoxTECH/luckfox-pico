/*
 * rk_aiq_types_adpcc_ext.h
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

#ifndef __RK_AIQ_TYPES_ADPCC_EXT_H__
#define __RK_AIQ_TYPES_ADPCC_EXT_H__

#include "rk_aiq_comm.h"

#define CALIBDB_MAX_ISO_LEVEL          13
#define CALIBDB_NR_SHARP_MAX_ISO_LEVEL CALIBDB_MAX_ISO_LEVEL
#define CALIBDB_DPCC_MAX_ISO_LEVEL     CALIBDB_MAX_ISO_LEVEL
#define CALIBDB_BLC_MAX_ISO_LEVEL      CALIBDB_MAX_ISO_LEVEL
#define CALIBDB_NR_SHARP_SETTING_LEVEL 6
#define CALIBDB_MAX_MODE_NUM           5
#define CALIBDB_ZOOM_FOCUS_TBL_SIZE    256
#define CALIBDB_DEGAMMA_CRUVE_KNOTS    17
#define CALIBDB_ADPCC_KNOTS_NUM        13
#define CALIBDB_ADPCC_PDAF_KNOTS_NUM   16
#define DPCC_MAX_ISO_LEVEL             CALIBDB_ADPCC_KNOTS_NUM
#define DPCC_PDAF_POINT_NUM            (16)

#define CALIBDB_MAX_MODE_NAME_LENGTH  (20)
#define CALIBDB_MAX_SCENE_NAME_LENGTH (10)

typedef enum AdpccOPMode_e {
    ADPCC_OP_MODE_INVALID = 0, /**< initialization value */
    ADPCC_OP_MODE_AUTO    = 1, /**< instance is created, but not initialized */
    ADPCC_OP_MODE_MANUAL  = 2, /**< instance is confiured (ready to start) or stopped */
    ADPCC_OP_MODE_MAX          /**< max */
} AdpccOPMode_t;

typedef enum Adpcc_onfly_mode_e {
    ADPCC_ONFLY_MODE_FAST   = 0, /**< dpcc manual fast mode */
    ADPCC_ONFLY_MODE_EXPERT = 1, /**< dpcc manual expert mode */
    ADPCC_ONFLY_MODE_MAX         /**< max */
} Adpcc_onfly_mode_t;

typedef struct CalibDb_Dpcc_Sensor_s {
    float en;
    float max_level;
    float iso[CALIBDB_DPCC_MAX_ISO_LEVEL];
    float level_single[CALIBDB_DPCC_MAX_ISO_LEVEL];
    float level_multiple[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_Sensor_t;

typedef struct CalibDb_Dpcc_Fast_Mode_s {
    int fast_mode_en;
    int ISO[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_single_en;
    int fast_mode_single_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_double_en;
    int fast_mode_double_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_triple_en;
    int fast_mode_triple_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
} CalibDb_Dpcc_Fast_Mode_t;

typedef struct Adpcc_basic_params_select_s {
    int iso;
    // mode 0x0000
    unsigned char stage1_enable;
    unsigned char grayscale_mode;
    unsigned char enable;

    // output_mode 0x0004
    unsigned char sw_rk_out_sel;
    unsigned char sw_dpcc_output_sel;
    unsigned char stage1_rb_3x3;
    unsigned char stage1_g_3x3;
    unsigned char stage1_incl_rb_center;
    unsigned char stage1_incl_green_center;

    // set_use 0x0008
    unsigned char stage1_use_fix_set;
    unsigned char stage1_use_set_3;
    unsigned char stage1_use_set_2;
    unsigned char stage1_use_set_1;

    // methods_set_1 0x000c
    unsigned char sw_rk_red_blue1_en;
    unsigned char rg_red_blue1_enable;
    unsigned char rnd_red_blue1_enable;
    unsigned char ro_red_blue1_enable;
    unsigned char lc_red_blue1_enable;
    unsigned char pg_red_blue1_enable;
    unsigned char sw_rk_green1_en;
    unsigned char rg_green1_enable;
    unsigned char rnd_green1_enable;
    unsigned char ro_green1_enable;
    unsigned char lc_green1_enable;
    unsigned char pg_green1_enable;

    // methods_set_2 0x0010
    unsigned char sw_rk_red_blue2_en;
    unsigned char rg_red_blue2_enable;
    unsigned char rnd_red_blue2_enable;
    unsigned char ro_red_blue2_enable;
    unsigned char lc_red_blue2_enable;
    unsigned char pg_red_blue2_enable;
    unsigned char sw_rk_green2_en;
    unsigned char rg_green2_enable;
    unsigned char rnd_green2_enable;
    unsigned char ro_green2_enable;
    unsigned char lc_green2_enable;
    unsigned char pg_green2_enable;

    // methods_set_3 0x0014
    unsigned char sw_rk_red_blue3_en;
    unsigned char rg_red_blue3_enable;
    unsigned char rnd_red_blue3_enable;
    unsigned char ro_red_blue3_enable;
    unsigned char lc_red_blue3_enable;
    unsigned char pg_red_blue3_enable;
    unsigned char sw_rk_green3_en;
    unsigned char rg_green3_enable;
    unsigned char rnd_green3_enable;
    unsigned char ro_green3_enable;
    unsigned char lc_green3_enable;
    unsigned char pg_green3_enable;

    // line_thresh_1 0x0018
    unsigned char sw_mindis1_rb;
    unsigned char sw_mindis1_g;
    unsigned char line_thr_1_rb;
    unsigned char line_thr_1_g;

    // line_mad_fac_1 0x001c
    unsigned char sw_dis_scale_min1;
    unsigned char sw_dis_scale_max1;
    unsigned char line_mad_fac_1_rb;
    unsigned char line_mad_fac_1_g;

    // pg_fac_1 0x0020
    unsigned char pg_fac_1_rb;
    unsigned char pg_fac_1_g;

    // rnd_thresh_1 0x0024
    unsigned char rnd_thr_1_rb;
    unsigned char rnd_thr_1_g;

    // rg_fac_1 0x0028
    unsigned char rg_fac_1_rb;
    unsigned char rg_fac_1_g;

    // line_thresh_2 0x002c
    unsigned char sw_mindis2_rb;
    unsigned char sw_mindis2_g;
    unsigned char line_thr_2_rb;
    unsigned char line_thr_2_g;

    // line_mad_fac_2 0x0030
    unsigned char sw_dis_scale_min2;
    unsigned char sw_dis_scale_max2;
    unsigned char line_mad_fac_2_rb;
    unsigned char line_mad_fac_2_g;

    // pg_fac_2 0x0034
    unsigned char pg_fac_2_rb;
    unsigned char pg_fac_2_g;

    // rnd_thresh_2 0x0038
    unsigned char rnd_thr_2_rb;
    unsigned char rnd_thr_2_g;

    // rg_fac_2 0x003c
    unsigned char rg_fac_2_rb;
    unsigned char rg_fac_2_g;

    // line_thresh_3 0x0040
    unsigned char sw_mindis3_rb;
    unsigned char sw_mindis3_g;
    unsigned char line_thr_3_rb;
    unsigned char line_thr_3_g;

    // line_mad_fac_3 0x0044
    unsigned char sw_dis_scale_min3;
    unsigned char sw_dis_scale_max3;
    unsigned char line_mad_fac_3_rb;
    unsigned char line_mad_fac_3_g;

    // pg_fac_3 0x0048
    unsigned char pg_fac_3_rb;
    unsigned char pg_fac_3_g;

    // rnd_thresh_3 0x004c
    unsigned char rnd_thr_3_rb;
    unsigned char rnd_thr_3_g;

    // rg_fac_3 0x0050
    unsigned char rg_fac_3_rb;
    unsigned char rg_fac_3_g;

    // ro_limits 0x0054
    unsigned char ro_lim_3_rb;
    unsigned char ro_lim_3_g;
    unsigned char ro_lim_2_rb;
    unsigned char ro_lim_2_g;
    unsigned char ro_lim_1_rb;
    unsigned char ro_lim_1_g;

    // rnd_offs 0x0058
    unsigned char rnd_offs_3_rb;
    unsigned char rnd_offs_3_g;
    unsigned char rnd_offs_2_rb;
    unsigned char rnd_offs_2_g;
    unsigned char rnd_offs_1_rb;
    unsigned char rnd_offs_1_g;
} Adpcc_basic_params_select_t;

typedef struct Adpcc_basic_params_s {
    Adpcc_basic_params_select_t arBasic[DPCC_MAX_ISO_LEVEL];
} Adpcc_basic_params_t;

typedef struct Adpcc_basic_cfg_params_s {
    // mode 0x0000
    // M4_NUMBER_DESC("stage1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_enable;
    // M4_NUMBER_DESC("grayscale_mode", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char grayscale_mode;
    // M4_NUMBER_DESC("enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char enable;

    // output_mode 0x0004
    // M4_NUMBER_DESC("sw_rk_out_sel", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_rk_out_sel;
    // M4_NUMBER_DESC("sw_dpcc_output_sel", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_dpcc_output_sel;
    // M4_NUMBER_DESC("stage1_rb_3x3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_rb_3x3;
    // M4_NUMBER_DESC("stage1_g_3x3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_g_3x3;
    // M4_NUMBER_DESC("stage1_incl_rb_center", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_incl_rb_center;
    // M4_NUMBER_DESC("stage1_incl_green_center", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_incl_green_center;

    // set_use 0x0008
    // M4_NUMBER_DESC("stage1_use_fix_set", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_use_fix_set;
    // M4_NUMBER_DESC("stage1_use_set_3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_use_set_3;
    // M4_NUMBER_DESC("stage1_use_set_2", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_use_set_2;
    // M4_NUMBER_DESC("stage1_use_set_1", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char stage1_use_set_1;

    // methods_set_1 0x000c
    // M4_NUMBER_DESC("sw_rk_red_blue1_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_rk_red_blue1_en;
    // M4_NUMBER_DESC("rg_red_blue1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_red_blue1_enable;
    // M4_NUMBER_DESC("rnd_red_blue1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_red_blue1_enable;
    // M4_NUMBER_DESC("ro_red_blue1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_red_blue1_enable;
    // M4_NUMBER_DESC("lc_red_blue1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char lc_red_blue1_enable;
    // M4_NUMBER_DESC("pg_red_blue1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_red_blue1_enable;
    // M4_NUMBER_DESC("sw_rk_green1_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_rk_green1_en;
    // M4_NUMBER_DESC("rg_green1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_green1_enable;
    // M4_NUMBER_DESC("rnd_green1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_green1_enable;
    // M4_NUMBER_DESC("ro_green1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_green1_enable;
    // M4_NUMBER_DESC("lc_green1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char lc_green1_enable;
    // M4_NUMBER_DESC("pg_green1_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_green1_enable;

    // methods_set_2 0x0010
    // M4_NUMBER_DESC("sw_rk_red_blue2_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_rk_red_blue2_en;
    // M4_NUMBER_DESC("rg_red_blue2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_red_blue2_enable;
    // M4_NUMBER_DESC("rnd_red_blue2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_red_blue2_enable;
    // M4_NUMBER_DESC("ro_red_blue2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_red_blue2_enable;
    // M4_NUMBER_DESC("lc_red_blue2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char lc_red_blue2_enable;
    // M4_NUMBER_DESC("pg_red_blue2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_red_blue2_enable;
    // M4_NUMBER_DESC("sw_rk_green2_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_rk_green2_en;
    // M4_NUMBER_DESC("rg_green2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_green2_enable;
    // M4_NUMBER_DESC("rnd_green2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_green2_enable;
    // M4_NUMBER_DESC("ro_green2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_green2_enable;
    // M4_NUMBER_DESC("lc_green2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char lc_green2_enable;
    // M4_NUMBER_DESC("pg_green2_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_green2_enable;

    // methods_set_3 0x0014
    // M4_NUMBER_DESC("sw_rk_red_blue3_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_rk_red_blue3_en;
    // M4_NUMBER_DESC("rg_red_blue3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_red_blue3_enable;
    // M4_NUMBER_DESC("rnd_red_blue3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_red_blue3_enable;
    // M4_NUMBER_DESC("ro_red_blue3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_red_blue3_enable;
    // M4_NUMBER_DESC("lc_red_blue3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char lc_red_blue3_enable;
    // M4_NUMBER_DESC("pg_red_blue3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_red_blue3_enable;
    // M4_NUMBER_DESC("sw_rk_green3_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_rk_green3_en;
    // M4_NUMBER_DESC("rg_green3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_green3_enable;
    // M4_NUMBER_DESC("rnd_green3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_green3_enable;
    // M4_NUMBER_DESC("ro_green3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_green3_enable;
    // M4_NUMBER_DESC("lc_green3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char lc_green3_enable;
    // M4_NUMBER_DESC("pg_green3_enable", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_green3_enable;

    // line_thresh_1 0x0018
    // M4_NUMBER_DESC("sw_mindis1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_mindis1_rb;
    // M4_NUMBER_DESC("sw_mindis1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_mindis1_g;
    // M4_NUMBER_DESC("line_thr_1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_thr_1_rb;
    // M4_NUMBER_DESC("line_thr_1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_thr_1_g;

    // line_mad_fac_1 0x001c
    // M4_NUMBER_DESC("sw_dis_scale_min1", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_dis_scale_min1;
    // M4_NUMBER_DESC("sw_dis_scale_max1", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_dis_scale_max1;
    // M4_NUMBER_DESC("line_mad_fac_1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_mad_fac_1_rb;
    // M4_NUMBER_DESC("line_mad_fac_1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_mad_fac_1_g;

    // pg_fac_1 0x0020
    // M4_NUMBER_DESC("pg_fac_1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_fac_1_rb;
    // M4_NUMBER_DESC("pg_fac_1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_fac_1_g;

    // rnd_thresh_1 0x0024
    // M4_NUMBER_DESC("rnd_thr_1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_thr_1_rb;
    // M4_NUMBER_DESC("rnd_thr_1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_thr_1_g;

    // rg_fac_1 0x0028
    // M4_NUMBER_DESC("rg_fac_1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_fac_1_rb;
    // M4_NUMBER_DESC("rg_fac_1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_fac_1_g;

    // line_thresh_2 0x002c
    // M4_NUMBER_DESC("sw_mindis2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_mindis2_rb;
    // M4_NUMBER_DESC("sw_mindis2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_mindis2_g;
    // M4_NUMBER_DESC("line_thr_2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_thr_2_rb;
    // M4_NUMBER_DESC("line_thr_2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_thr_2_g;

    // line_mad_fac_2 0x0030
    // M4_NUMBER_DESC("sw_dis_scale_min2", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_dis_scale_min2;
    // M4_NUMBER_DESC("sw_dis_scale_max2", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_dis_scale_max2;
    // M4_NUMBER_DESC("line_mad_fac_2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_mad_fac_2_rb;
    // M4_NUMBER_DESC("line_mad_fac_2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_mad_fac_2_g;

    // pg_fac_2 0x0034
    // M4_NUMBER_DESC("pg_fac_2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_fac_2_rb;
    // M4_NUMBER_DESC("pg_fac_2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_fac_2_g;

    // rnd_thresh_2 0x0038
    // M4_NUMBER_DESC("rnd_thr_2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_thr_2_rb;
    // M4_NUMBER_DESC("rnd_thr_2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_thr_2_g;

    // rg_fac_2 0x003c
    // M4_NUMBER_DESC("rg_fac_2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_fac_2_rb;
    // M4_NUMBER_DESC("rg_fac_2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_fac_2_g;

    // line_thresh_3 0x0040
    // M4_NUMBER_DESC("sw_mindis3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_mindis3_rb;
    // M4_NUMBER_DESC("sw_mindis3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_mindis3_g;
    // M4_NUMBER_DESC("line_thr_3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_thr_3_rb;
    // M4_NUMBER_DESC("line_thr_3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_thr_3_g;

    // line_mad_fac_3 0x0044
    // M4_NUMBER_DESC("sw_dis_scale_min3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_dis_scale_min3;
    // M4_NUMBER_DESC("sw_dis_scale_max3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_dis_scale_max3;
    // M4_NUMBER_DESC("line_mad_fac_3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_mad_fac_3_rb;
    // M4_NUMBER_DESC("line_mad_fac_3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char line_mad_fac_3_g;

    // pg_fac_3 0x0048
    // M4_NUMBER_DESC("pg_fac_3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_fac_3_rb;
    // M4_NUMBER_DESC("pg_fac_3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pg_fac_3_g;

    // rnd_thresh_3 0x004c
    // M4_NUMBER_DESC("rnd_thr_3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_thr_3_rb;
    // M4_NUMBER_DESC("rnd_thr_3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_thr_3_g;

    // rg_fac_3 0x0050
    // M4_NUMBER_DESC("rg_fac_3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_fac_3_rb;
    // M4_NUMBER_DESC("rg_fac_3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rg_fac_3_g;

    // ro_limits 0x0054
    // M4_NUMBER_DESC("ro_lim_3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_lim_3_rb;
    // M4_NUMBER_DESC("ro_lim_3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_lim_3_g;
    // M4_NUMBER_DESC("ro_lim_2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_lim_2_rb;
    // M4_NUMBER_DESC("ro_lim_2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_lim_2_g;
    // M4_NUMBER_DESC("ro_lim_1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_lim_1_rb;
    // M4_NUMBER_DESC("ro_lim_1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char ro_lim_1_g;

    // rnd_offs 0x0058
    // M4_NUMBER_DESC("rnd_offs_3_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_offs_3_rb;
    // M4_NUMBER_DESC("rnd_offs_3_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_offs_3_g;
    // M4_NUMBER_DESC("rnd_offs_2_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_offs_2_rb;
    // M4_NUMBER_DESC("rnd_offs_2_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_offs_2_g;
    // M4_NUMBER_DESC("rnd_offs_1_rb", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_offs_1_rb;
    // M4_NUMBER_DESC("rnd_offs_1_g", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char rnd_offs_1_g;
} Adpcc_basic_cfg_params_t;

typedef struct Adpcc_bpt_params_s {
    // bpt_ctrl 0x005c
    // M4_NUMBER_DESC("bpt_rb_3x3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_rb_3x3;
    // M4_NUMBER_DESC("bpt_g_3x3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_g_3x3;
    // M4_NUMBER_DESC("bpt_incl_rb_center", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_incl_rb_center;
    // M4_NUMBER_DESC("bpt_incl_green_center", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_incl_green_center;
    // M4_NUMBER_DESC("bpt_use_fix_set", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_use_fix_set;
    // M4_NUMBER_DESC("bpt_use_set_3", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_use_set_3;
    // M4_NUMBER_DESC("bpt_use_set_2", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_use_set_2;
    // M4_NUMBER_DESC("bpt_use_set_1", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_use_set_1;
    // M4_NUMBER_DESC("bpt_cor_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_cor_en;
    // M4_NUMBER_DESC("bpt_det_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char bpt_det_en;

    // bpt_number 0x0060
    // M4_NUMBER_DESC("bp_number", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t bp_number;

    // bpt_addr 0x0064
    // M4_NUMBER_DESC("bp_table_addr", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t bp_table_addr;

    // bpt_data 0x0068
    // M4_NUMBER_DESC("bpt_v_addr", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t bpt_v_addr;
    // M4_NUMBER_DESC("bpt_h_addr", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t bpt_h_addr;

    // bp_cnt 0x006c
    // M4_NUMBER_DESC("bp_cnt", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned int bp_cnt;
} Adpcc_bpt_params_t;

typedef struct dpcc_pdaf_point_s {
    // M4_NUMBER_DESC("y", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char y;
    // M4_NUMBER_DESC("x", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char x;
} dpcc_pdaf_point_t;

typedef struct Adpcc_pdaf_params_s {
    // pdaf_en 0x0070
    // M4_NUMBER_DESC("sw_pdaf_en", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char sw_pdaf_en;

    // pdaf_point_en 0x0074
    // M4_ARRAY_DESC("pdaf_point_en", "u8", M4_SIZE(1,16), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char pdaf_point_en[DPCC_PDAF_POINT_NUM];

    // pdaf_offset 0x0078
    // M4_NUMBER_DESC("pdaf_offsety", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t pdaf_offsety;
    // M4_NUMBER_DESC("pdaf_offsetx", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t pdaf_offsetx;

    // pdaf_wrap 0x007c
    // M4_NUMBER_DESC("pdaf_wrapy", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pdaf_wrapy;
    // M4_NUMBER_DESC("pdaf_wrapx", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pdaf_wrapx;

    // pdaf_scope 0x0080
    // M4_NUMBER_DESC("pdaf_wrapy_num", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t pdaf_wrapy_num;
    // M4_NUMBER_DESC("pdaf_wrapx_num", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    uint16_t pdaf_wrapx_num;

    // pdaf_point_0 0x0084
    // M4_STRUCT_LIST_DESC("point", M4_SIZE(1,16), "normal_ui_style")
    dpcc_pdaf_point_t point[DPCC_PDAF_POINT_NUM];

    // pdaf_forward_med 0x00a4
    // M4_NUMBER_DESC("pdaf_forward_med", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char pdaf_forward_med;
} Adpcc_pdaf_params_t;

typedef struct Adpcc_Auto_Attr_s {
    Adpcc_basic_params_t stBasicParams;
    Adpcc_bpt_params_t stBptParams;
    Adpcc_pdaf_params_t stPdafParams;
    CalibDb_Dpcc_Fast_Mode_t stFastMode;
    CalibDb_Dpcc_Sensor_t stSensorDpcc;
} Adpcc_Auto_Attr_t;

typedef struct Adpcc_fast_mode_attr_s {
    // M4_BOOL_DESC("fast_mode_en", "0")
    bool fast_mode_en;
    // M4_BOOL_DESC("fast_mode_single_en", "0")
    bool fast_mode_single_en;
    // M4_NUMBER_DESC("fast_mode_single_level", "s32", M4_RANGE(0,255), "0", M4_DIGIT(0))
    int fast_mode_single_level;
    // M4_BOOL_DESC("fast_mode_double_en", "0")
    bool fast_mode_double_en;
    // M4_NUMBER_DESC("fast_mode_double_level", "s32", M4_RANGE(0,255), "0", M4_DIGIT(0))
    int fast_mode_double_level;
    // M4_BOOL_DESC("fast_mode_triple_en", "0")
    bool fast_mode_triple_en;
    // M4_NUMBER_DESC("fast_mode_triple_level", "f32", M4_RANGE(0,255), "0", M4_DIGIT(0))
    int fast_mode_triple_level;
} Adpcc_fast_mode_attr_t;

typedef struct Adpcc_onfly_cfg_s {
    // M4_ENUM_DESC("mode", "Adpcc_onfly_mode_t", "ADPCC_ONFLY_MODE_FAST");
    Adpcc_onfly_mode_t mode;  // expert or fast mode
    // M4_STRUCT_DESC("fast_mode", "normal_ui_style")
    Adpcc_fast_mode_attr_t fast_mode;
    // M4_STRUCT_DESC("expert_mode", "normal_ui_style")
    Adpcc_basic_cfg_params_t expert_mode;
} Adpcc_onfly_cfg_t;

typedef struct Adpcc_sensor_dpcc_attr_s {
    // M4_BOOL_DESC("en", "0")
    bool en;
    // M4_NUMBER_DESC("max_level", "s16", M4_RANGE(0,32767), "0", M4_DIGIT(0))
    int max_level;
    // M4_NUMBER_DESC("single_level", "s16", M4_RANGE(0,32767), "0", M4_DIGIT(0))
    int single_level;
    // M4_NUMBER_DESC("double_level", "s16", M4_RANGE(0,32767), "0", M4_DIGIT(0))
    int double_level;
} Adpcc_sensor_dpcc_attr_t;

typedef struct Adpcc_Manual_Attr_s {
    // M4_NUMBER_DESC("enable", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0))
    unsigned char enable;
    // M4_STRUCT_DESC("stOnfly", "normal_ui_style")
    Adpcc_onfly_cfg_t stOnfly;
    // M4_STRUCT_DESC("stBpt", "normal_ui_style")
    Adpcc_bpt_params_t stBpt;
    // M4_STRUCT_DESC("stPdaf", "normal_ui_style")
    Adpcc_pdaf_params_t stPdaf;
    // M4_STRUCT_DESC("stSensorDpcc", "normal_ui_style")
    Adpcc_sensor_dpcc_attr_t stSensorDpcc;
} Adpcc_Manual_Attr_t;

typedef struct rk_aiq_dpcc_attrib_V20_s {
    AdpccOPMode_t eMode;
    Adpcc_Auto_Attr_t stAuto;
    Adpcc_Manual_Attr_t stManual;
    rk_aiq_uapi_sync_t sync;
} rk_aiq_dpcc_attrib_V20_t;

#endif /*__RK_AIQ_TYPES_ADPCC_EXT_H__*/
