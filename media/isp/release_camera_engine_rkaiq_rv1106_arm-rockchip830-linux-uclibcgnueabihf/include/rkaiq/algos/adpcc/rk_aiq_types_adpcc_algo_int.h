/*
 *rk_aiq_types_alsc_algo_int.h
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

#ifndef _RK_AIQ_TYPE_ADPCC_ALGO_INT_H_
#define _RK_AIQ_TYPE_ADPCC_ALGO_INT_H_
#include "RkAiqCalibDbTypes.h"
#include "adpcc_head.h"
#include "adpcc/rk_aiq_types_adpcc_algo.h"
#include "base/xcam_common.h"


#define DPCC_MAX_ISO_LEVEL CALIBDB_ADPCC_KNOTS_NUM
#define DPCC_PDAF_POINT_NUM (16)

typedef enum AdpccResult_e {
    ADPCC_RET_SUCCESS               = 0,   // this has to be 0, if clauses rely on it
    ADPCC_RET_FAILURE               = 1,   // process failure
    ADPCC_RET_INVALID_PARM      = 2,   // invalid parameter
    ADPCC_RET_WRONG_CONFIG      = 3,   // feature not supported
    ADPCC_RET_BUSY              = 4,   // there's already something going on...
    ADPCC_RET_CANCELED          = 5,   // operation canceled
    ADPCC_RET_OUTOFMEM          = 6,   // out of memory
    ADPCC_RET_OUTOFRANGE            = 7,   // parameter/value out of range
    ADPCC_RET_NULL_POINTER      = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ADPCC_RET_DIVISION_BY_ZERO  = 9,   // a divisor equals ZERO
    ADPCC_RET_NO_INPUTIMAGE     = 10   // no input image
} AdpccResult_t;

typedef enum AdpccState_e {
    ADPCC_STATE_INVALID           = 0,                   /**< initialization value */
    ADPCC_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ADPCC_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ADPCC_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ADPCC_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ADPCC_STATE_MAX                                      /**< max */
} AdpccState_t;

typedef enum AdpccOPMode_e {
    ADPCC_OP_MODE_INVALID        = 0,                   /**< initialization value */
    ADPCC_OP_MODE_AUTO            = 1,                   /**< instance is created, but not initialized */
    ADPCC_OP_MODE_MANUAL          = 2,                   /**< instance is confiured (ready to start) or stopped */
    //ADPCC_OP_MODE_TOOL            = 3,                   /**< instance is confiured (ready to start) or stopped */
    ADPCC_OP_MODE_MAX                                      /**< max */
} AdpccOPMode_t;

typedef struct Adpcc_basic_params_select_s {
    int iso;
    //mode 0x0000
    unsigned char stage1_enable;
    unsigned char grayscale_mode;
    unsigned char enable;

    //output_mode 0x0004
    unsigned char sw_rk_out_sel;
    unsigned char sw_dpcc_output_sel;
    unsigned char stage1_rb_3x3;
    unsigned char stage1_g_3x3;
    unsigned char stage1_incl_rb_center;
    unsigned char stage1_incl_green_center;

    //set_use 0x0008
    unsigned char stage1_use_fix_set;
    unsigned char stage1_use_set_3;
    unsigned char stage1_use_set_2;
    unsigned char stage1_use_set_1;

    //methods_set_1 0x000c
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

    //methods_set_2 0x0010
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

    //methods_set_3 0x0014
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

    //line_thresh_1 0x0018
    unsigned char sw_mindis1_rb;
    unsigned char sw_mindis1_g;
    unsigned char line_thr_1_rb;
    unsigned char line_thr_1_g;

    //line_mad_fac_1 0x001c
    unsigned char sw_dis_scale_min1;
    unsigned char sw_dis_scale_max1;
    unsigned char line_mad_fac_1_rb;
    unsigned char line_mad_fac_1_g;

    //pg_fac_1 0x0020
    unsigned char pg_fac_1_rb;
    unsigned char pg_fac_1_g;

    //rnd_thresh_1 0x0024
    unsigned char rnd_thr_1_rb;
    unsigned char rnd_thr_1_g;

    //rg_fac_1 0x0028
    unsigned char rg_fac_1_rb;
    unsigned char rg_fac_1_g;

    //line_thresh_2 0x002c
    unsigned char sw_mindis2_rb;
    unsigned char sw_mindis2_g;
    unsigned char line_thr_2_rb;
    unsigned char line_thr_2_g;

    //line_mad_fac_2 0x0030
    unsigned char sw_dis_scale_min2;
    unsigned char sw_dis_scale_max2;
    unsigned char line_mad_fac_2_rb;
    unsigned char line_mad_fac_2_g;

    //pg_fac_2 0x0034
    unsigned char pg_fac_2_rb;
    unsigned char pg_fac_2_g;

    //rnd_thresh_2 0x0038
    unsigned char rnd_thr_2_rb;
    unsigned char rnd_thr_2_g;

    //rg_fac_2 0x003c
    unsigned char rg_fac_2_rb;
    unsigned char rg_fac_2_g;

    //line_thresh_3 0x0040
    unsigned char sw_mindis3_rb;
    unsigned char sw_mindis3_g;
    unsigned char line_thr_3_rb;
    unsigned char line_thr_3_g;

    //line_mad_fac_3 0x0044
    unsigned char sw_dis_scale_min3;
    unsigned char sw_dis_scale_max3;
    unsigned char line_mad_fac_3_rb;
    unsigned char line_mad_fac_3_g;

    //pg_fac_3 0x0048
    unsigned char pg_fac_3_rb;
    unsigned char pg_fac_3_g;

    //rnd_thresh_3 0x004c
    unsigned char rnd_thr_3_rb;
    unsigned char rnd_thr_3_g;

    //rg_fac_3 0x0050
    unsigned char rg_fac_3_rb;
    unsigned char rg_fac_3_g;

    //ro_limits 0x0054
    unsigned char ro_lim_3_rb;
    unsigned char ro_lim_3_g;
    unsigned char ro_lim_2_rb;
    unsigned char ro_lim_2_g;
    unsigned char ro_lim_1_rb;
    unsigned char ro_lim_1_g;

    //rnd_offs 0x0058
    unsigned char rnd_offs_3_rb;
    unsigned char rnd_offs_3_g;
    unsigned char rnd_offs_2_rb;
    unsigned char rnd_offs_2_g;
    unsigned char rnd_offs_1_rb;
    unsigned char rnd_offs_1_g;
} Adpcc_basic_params_select_t;

typedef struct Adpcc_bpt_params_select_s {
    //bpt_ctrl 0x005c
    unsigned char bpt_rb_3x3;
    unsigned char bpt_g_3x3;
    unsigned char bpt_incl_rb_center;
    unsigned char bpt_incl_green_center;
    unsigned char bpt_use_fix_set;
    unsigned char bpt_use_set_3;
    unsigned char bpt_use_set_2;
    unsigned char bpt_use_set_1;
    unsigned char bpt_cor_en;
    unsigned char bpt_det_en;

    //bpt_number 0x0060
    unsigned short int bp_number;

    //bpt_addr 0x0064
    unsigned short int bp_table_addr;

    //bpt_data 0x0068
    unsigned short int bpt_v_addr;
    unsigned short int bpt_h_addr;

    //bp_cnt 0x006c
    unsigned int bp_cnt;
} Adpcc_bpt_params_select_t;

typedef struct dpcc_pdaf_point_s {
    unsigned char y;
    unsigned char x;
} dpcc_pdaf_point_t;

typedef struct Adpcc_pdaf_params_select_s {
    //pdaf_en 0x0070
    unsigned char sw_pdaf_en;

    //pdaf_point_en 0x0074
    unsigned char pdaf_point_en[DPCC_PDAF_POINT_NUM];

    //pdaf_offset 0x0078
    unsigned short int pdaf_offsety;
    unsigned short int pdaf_offsetx;

    //pdaf_wrap 0x007c
    unsigned char pdaf_wrapy;
    unsigned char pdaf_wrapx;

    //pdaf_scope 0x0080
    unsigned short int pdaf_wrapy_num;
    unsigned short int pdaf_wrapx_num;

    //pdaf_point_0 0x0084
    dpcc_pdaf_point_t point[DPCC_PDAF_POINT_NUM];

    //pdaf_forward_med 0x00a4
    unsigned char pdaf_forward_med;
} Adpcc_pdaf_params_select_t;

typedef struct Adpcc_fast_mode_params_s {
    int fast_mode_en;
    int ISO[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_single_en;
    int fast_mode_single_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_double_en;
    int fast_mode_double_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
    int fast_mode_triple_en;
    int fast_mode_triple_level[CALIBDB_DPCC_MAX_ISO_LEVEL];
} Adpcc_fast_mode_params_t;

typedef struct Adpcc_fast_mode_attr_s {
    bool fast_mode_en;
    bool fast_mode_single_en;
    int fast_mode_single_level;
    bool fast_mode_double_en;
    int fast_mode_double_level;
    bool fast_mode_triple_en;
    int fast_mode_triple_level;
} Adpcc_fast_mode_attr_t;

typedef struct Adpcc_sensor_dpcc_attr_s {
    bool en;
    int max_level;
    int single_level;
    int double_level;
} Adpcc_sensor_dpcc_attr_t;

typedef struct Adpcc_sensor_dpcc_s {
    bool en;
    int max_level;
    int level;
} Adpcc_sensor_dpcc_t;


typedef struct Adpcc_basic_params_s {
    Adpcc_basic_params_select_t arBasic[DPCC_MAX_ISO_LEVEL];

} Adpcc_basic_params_t;

typedef Adpcc_bpt_params_select_t Adpcc_bpt_params_t;
typedef Adpcc_pdaf_params_select_t Adpcc_pdaf_params_t;

typedef struct Adpcc_html_param_s {
    Adpcc_basic_params_t stBasic;
    Adpcc_bpt_params_t  stBpt;
    Adpcc_pdaf_params_t stPdaf;
} Adpcc_html_param_t;

typedef struct Adpcc_Manual_Attr_s
{
    Adpcc_basic_params_select_t stBasic;
    Adpcc_bpt_params_select_t stBpt;
    Adpcc_pdaf_params_select_t stPdaf;
    Adpcc_fast_mode_attr_t stFastMode;
    Adpcc_sensor_dpcc_attr_t stSensorDpcc;
} Adpcc_Manual_Attr_t;

typedef struct Adpcc_Auto_Attr_s
{
    Adpcc_basic_params_t stBasicParams;
    Adpcc_bpt_params_t  stBptParams;
    Adpcc_pdaf_params_t stPdafParams;
    CalibDb_Dpcc_Fast_Mode_t stFastMode;
    CalibDb_Dpcc_Sensor_t stSensorDpcc;

    Adpcc_basic_params_select_t stBasicSelect;
    Adpcc_bpt_params_select_t stBptSelect;
    Adpcc_pdaf_params_select_t stPdafSelect;

} Adpcc_Auto_Attr_t;

typedef struct AdpccProcResult_s {
    Adpcc_basic_params_select_t stBasic;
    Adpcc_bpt_params_select_t stBpt;
    Adpcc_pdaf_params_select_t stPdaf;
} AdpccProcResult_t;

typedef struct AdpccConfig_s {
    AdpccOPMode_t eMode;
    AdpccState_t eState;
} AdpccConfig_t;

typedef struct rk_aiq_dpcc_attrib_V20_s {
    AdpccOPMode_t eMode;
    Adpcc_Auto_Attr_t stAuto;
    Adpcc_Manual_Attr_t stManual;
    rk_aiq_uapi_sync_t sync;
} rk_aiq_dpcc_attrib_V20_t;

typedef struct AdpccExpInfo_s {
    int hdr_mode;
    float arPreResTime[3];
    float arPreResAGain[3];
    float arPreResDGain[3];
    int   arPreResIso[3];

    float arProcResTime[3];
    float arProcResAGain[3];
    float arProcResDGain[3];
    int   arProcResIso[3];
} AdpccExpInfo_t;

typedef struct Sensor_dpcc_res_s {
    int enable;
    int cur_single_dpcc;
    int cur_multiple_dpcc;
    int total_dpcc;
} Sensor_dpcc_res_t;

typedef struct Adpcc_pre_ae_res_s {
    float arProcResTime[3];
    float arProcResAGain[3];
    float arProcResDGain[3];
    int   arProcResIso[3];
} Adpcc_pre_ae_res_t;


#endif
