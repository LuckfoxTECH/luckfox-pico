/*
 * adpcc_head.h
 *
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

#ifndef __CALIBDBV2_ADPCC_HEAD_H__
#define __CALIBDBV2_ADPCC_HEAD_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define CALIBDB_ADPCC_KNOTS_NUM               13
#define CALIBDB_ADPCC_PDAF_KNOTS_NUM          16

typedef struct CalibDb_Dpcc_set_RK_V20_s {
    // M4_ARRAY_DESC("RK_enable", "u8", M4_SIZE(1,13),  M4_RANGE(0, 1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char RK_enable[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_sw_mindis", "u8", M4_SIZE(1,13),  M4_RANGE(0, 255), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_sw_mindis[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_sw_mindis", "u8", M4_SIZE(1,13),  M4_RANGE(0, 255), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_sw_mindis[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("sw_dis_scale_min", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char sw_dis_scale_min[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("sw_dis_scale_max", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char sw_dis_scale_max[CALIBDB_ADPCC_KNOTS_NUM];
} CalibDb_Dpcc_set_RK_V20_t;

typedef struct CalibDb_Dpcc_set_LC_V20_s {
    // M4_ARRAY_DESC("LC_enable", "u8", M4_SIZE(1,13),  M4_RANGE(0, 1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char LC_enable[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_line_thr", "u8", M4_SIZE(1,13),  M4_RANGE(0, 255), "[8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_line_thr[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_line_thr", "u8", M4_SIZE(1,13),  M4_RANGE(0, 255), "[8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_line_thr[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_line_mad_fac", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_line_mad_fac[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_line_mad_fac", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_line_mad_fac[CALIBDB_ADPCC_KNOTS_NUM];
} CalibDb_Dpcc_set_LC_V20_t;

typedef struct CalibDb_Dpcc_set_PG_V20_s {
    // M4_ARRAY_DESC("PG_enable", "u8", M4_SIZE(1,13),  M4_RANGE(0, 1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char PG_enable[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_pg_fac", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_pg_fac[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_pg_fac", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_pg_fac[CALIBDB_ADPCC_KNOTS_NUM];
} CalibDb_Dpcc_set_PG_V20_t;

typedef struct CalibDb_Dpcc_set_RND_V20_s {
    // M4_ARRAY_DESC("RND_enable", "u8", M4_SIZE(1,13),  M4_RANGE(0, 1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char RND_enable[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_rnd_thr", "u8", M4_SIZE(1,13),  M4_RANGE(0, 255), "[8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_rnd_thr[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_rnd_thr", "u8", M4_SIZE(1,13),  M4_RANGE(0, 255), "[8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_rnd_thr[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_rnd_offs", "u8", M4_SIZE(1,13),  M4_RANGE(0, 3), "[3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_rnd_offs[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_rnd_offs", "u8", M4_SIZE(1,13),  M4_RANGE(0, 3), "[3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_rnd_offs[CALIBDB_ADPCC_KNOTS_NUM];
} CalibDb_Dpcc_set_RND_V20_t;

typedef struct CalibDb_Dpcc_set_RG_V20_s {
    // M4_ARRAY_DESC("RG_enable", "u8", M4_SIZE(1,13),  M4_RANGE(0, 1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char RG_enable[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_rg_fac", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_rg_fac[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_rg_fac", "u8", M4_SIZE(1,13),  M4_RANGE(0, 63), "[8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_rg_fac[CALIBDB_ADPCC_KNOTS_NUM];
} CalibDb_Dpcc_set_RG_V20_t;

typedef struct CalibDb_Dpcc_set_RO_V20_s {
    // M4_ARRAY_DESC("RO_enable", "u8", M4_SIZE(1,13),  M4_RANGE(0, 1), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char RO_enable[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("rb_ro_lim", "u8", M4_SIZE(1,13),  M4_RANGE(0, 3), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char rb_ro_lim[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("g_ro_lim", "u8", M4_SIZE(1,13),  M4_RANGE(0, 3), "[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char g_ro_lim[CALIBDB_ADPCC_KNOTS_NUM];
} CalibDb_Dpcc_set_RO_V20_t;

typedef struct CalibDb_Dpcc_set_V20_s {
    // M4_STRUCT_DESC("RK", "normal_ui_style")
    CalibDb_Dpcc_set_RK_V20_t  RK;
    // M4_STRUCT_DESC("LC", "normal_ui_style")
    CalibDb_Dpcc_set_LC_V20_t  LC;
    // M4_STRUCT_DESC("PG", "normal_ui_style")
    CalibDb_Dpcc_set_PG_V20_t  PG;
    // M4_STRUCT_DESC("RND", "normal_ui_style")
    CalibDb_Dpcc_set_RND_V20_t RND;
    // M4_STRUCT_DESC("RG", "normal_ui_style")
    CalibDb_Dpcc_set_RG_V20_t  RG;
    // M4_STRUCT_DESC("RO", "normal_ui_style")
    CalibDb_Dpcc_set_RO_V20_t  RO;
} CalibDb_Dpcc_set_V20_t;

typedef struct FastData_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(50,10000000), "50",M4_DIGIT(2), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("Single_level", "u32", M4_SIZE(1,100), M4_RANGE(1,10), "1",M4_DIGIT(0), M4_DYNAMIC(1))
    int*        Single_level;
    int Single_level_len;
    // M4_ARRAY_DESC("Double_level", "u32", M4_SIZE(1,100), M4_RANGE(1,10), "1",M4_DIGIT(0), M4_DYNAMIC(1))
    int*        Double_level;
    int Double_level_len;
    // M4_ARRAY_DESC("Triple_level", "u32", M4_SIZE(1,100), M4_RANGE(1,10), "1",M4_DIGIT(0), M4_DYNAMIC(1))
    int*        Triple_level;
    int Triple_level_len;
} FastData_t;

typedef struct CalibDb_Dpcc_Fast_Mode_V20_s {
    // M4_BOOL_DESC("Fast_mode_en", "1")
    bool          Fast_mode_en;
    // M4_BOOL_DESC("Single_enable", "1")
    bool Single_enable;
    // M4_BOOL_DESC("Double_enable", "1")
    bool Double_enable;
    // M4_BOOL_DESC("Triple_enable", "1")
    bool Triple_enable;
    // M4_ARRAY_TABLE_DESC("Fast_Data", "array_table_ui", "none")
    FastData_t Fast_Data;
} CalibDb_Dpcc_Fast_Mode_V20_t;

typedef struct SetData_s {
    // M4_STRUCT_DESC("RK", "normal_ui_style")
    CalibDb_Dpcc_set_RK_V20_t  RK;
    // M4_STRUCT_DESC("LC", "normal_ui_style")
    CalibDb_Dpcc_set_LC_V20_t  LC;
    // M4_STRUCT_DESC("PG", "normal_ui_style")
    CalibDb_Dpcc_set_PG_V20_t  PG;
    // M4_STRUCT_DESC("RND", "normal_ui_style")
    CalibDb_Dpcc_set_RND_V20_t RND;
    // M4_STRUCT_DESC("RG", "normal_ui_style")
    CalibDb_Dpcc_set_RG_V20_t  RG;
    // M4_STRUCT_DESC("RO", "normal_ui_style")
    CalibDb_Dpcc_set_RO_V20_t  RO;
} SetData_t;

typedef struct SetEnable_s {
    // M4_ARRAY_DESC("ISO", "u32", M4_SIZE(1,13),  M4_RANGE(50, 100000000), "[50, 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800]", M4_DIGIT(4), M4_DYNAMIC(0))
    int  ISO[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("fix_set", "u16", M4_SIZE(1,13),  M4_RANGE(0, 1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  fix_set[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("set1", "u16", M4_SIZE(1,13),  M4_RANGE(0, 1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  set1[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("set2", "u16", M4_SIZE(1,13),  M4_RANGE(0, 1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  set2[CALIBDB_ADPCC_KNOTS_NUM];
    // M4_ARRAY_DESC("set3", "u16", M4_SIZE(1,13),  M4_RANGE(0, 1), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]", M4_DIGIT(0), M4_DYNAMIC(0))
    int  set3[CALIBDB_ADPCC_KNOTS_NUM];
} SetEnable_t;

typedef struct CalibDb_Dpcc_Expert_Mode_V20_s {
    //M4_BOOL_DESC("stage1_Enable", "1")
    bool       stage1_Enable;
    // M4_BOOL_DESC("grayscale_mode", "0")
    bool       grayscale_mode;
    // M4_BOOL_DESC("dpcc_out_sel", "1")
    bool       dpcc_out_sel;
    // M4_BOOL_DESC("stage1_g_3x3", "0")
    bool        stage1_g_3x3;
    // M4_BOOL_DESC("stage1_rb_3x3", "0")
    bool        stage1_rb_3x3;
    // M4_BOOL_DESC("stage1_inc_rb_center", "1")
    bool    stage1_inc_rb_center;
    // M4_BOOL_DESC("stage1_inc_g_center", "1")
    bool    stage1_inc_g_center;
    // M4_NUMBER_DESC("rk_out_sel", "u8", M4_RANGE(0,2), "0", M4_DIGIT(0))
    unsigned char       rk_out_sel;
    // M4_STRUCT_DESC("SetEnable", "array_table_ui")
    SetEnable_t SetEnable;
    // M4_ARRAY_TABLE_DESC("set1", "array_table_ui", "none")
    SetData_t set1;
    // M4_ARRAY_TABLE_DESC("set2", "array_table_ui", "none")
    SetData_t set2;
    // M4_ARRAY_TABLE_DESC("set3", "array_table_ui", "none")
    SetData_t set3;
} CalibDb_Dpcc_Expert_Mode_V20_t;

typedef struct CalibDb_Dpcc_Pdaf_V20_s {
    // M4_BOOL_DESC("en", "0")
    bool en;
    // M4_ARRAY_DESC("point_en", "u16", M4_SIZE(1,16),  M4_RANGE(1, 10), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char point_en[16];
    // M4_NUMBER_DESC("offsetx", "u8", M4_RANGE(0,100), "0", M4_DIGIT(0))
    int offsetx;
    // M4_NUMBER_DESC("offsety", "u8", M4_RANGE(0,100), "0", M4_DIGIT(0))
    int offsety;
    // M4_NUMBER_DESC("wrapx", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char wrapx;
    // M4_NUMBER_DESC("wrapy", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0))
    unsigned char wrapy;
    // M4_NUMBER_DESC("wrapx_num", "u8", M4_RANGE(0,1023), "0", M4_DIGIT(0))
    int wrapx_num;
    // M4_NUMBER_DESC("wrapy_num", "u8", M4_RANGE(0,1023), "0", M4_DIGIT(0))
    int wrapy_num;
    // M4_ARRAY_MARK_DESC("point_x", "u16", M4_SIZE(1,16),  M4_RANGE(1, 255), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char point_x[16];
    // M4_ARRAY_MARK_DESC("point_y", "u16", M4_SIZE(1,16),  M4_RANGE(1, 255), "[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0]", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char point_y[16];
    // M4_NUMBER_DESC("forward_med", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0))
    unsigned char forward_med;
} CalibDb_Dpcc_Pdaf_V20_t;

typedef struct SensorDpccData_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(50,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("level_single", "u32", M4_SIZE(1,100), M4_RANGE(1,10000000), "1",M4_DIGIT(0), M4_DYNAMIC(1))
    int*        level_single;
    int level_single_len;
    // M4_ARRAY_DESC("level_multiple", "u32", M4_SIZE(1,100), M4_RANGE(1,10000000), "1",M4_DIGIT(0), M4_DYNAMIC(1))
    int*        level_multiple;
    int level_multiple_len;
} SensorDpccData_t;

typedef struct CalibDb_Dpcc_Sensor_V20_s {
    // M4_BOOL_DESC("sensor_dpcc_auto_en", "1")
    bool sensor_dpcc_auto_en;
    // M4_NUMBER_DESC("max_level", "u8", M4_RANGE(0,100), "1", M4_DIGIT(0))
    int        max_level;
    // M4_ARRAY_TABLE_DESC("SensorDpcc_Data", "array_table_ui", "none")
    SensorDpccData_t SensorDpcc_Data;
} CalibDb_Dpcc_Sensor_V20_t;

typedef struct CalibDbDpccV20_s {
    // M4_BOOL_DESC("Enable", "1")
    bool                        Enable;
    // M4_STRUCT_DESC("Fast_Mode", "normal_ui_style")
    CalibDb_Dpcc_Fast_Mode_V20_t   Fast_Mode;
    // M4_STRUCT_DESC("Expert_Mode", "normal_ui_style")
    CalibDb_Dpcc_Expert_Mode_V20_t Expert_Mode;
    // M4_STRUCT_DESC("CalibDb_Dpcc_Pdaf_V20_t", "normal_ui_style")
    CalibDb_Dpcc_Pdaf_V20_t        Dpcc_pdaf;
    // M4_STRUCT_DESC("Sensor_dpcc", "normal_ui_style")
    CalibDb_Dpcc_Sensor_V20_t     Sensor_dpcc;
} CalibDbDpccV20_t;

typedef struct CalibDbV2_Dpcc_s {
    // M4_STRUCT_DESC("DpccTuningPara", "normal_ui_style")
    CalibDbDpccV20_t DpccTuningPara;
} CalibDbV2_Dpcc_t;

RKAIQ_END_DECLARE

#endif
