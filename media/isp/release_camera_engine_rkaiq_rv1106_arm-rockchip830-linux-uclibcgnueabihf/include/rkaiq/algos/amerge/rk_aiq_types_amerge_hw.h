/*
 * rk_aiq_algo_amerge_hw.h
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

#ifndef _RK_AIQ_ALGO_AMERGE_HW_H_
#define _RK_AIQ_ALGO_AMERGE_HW_H_

#define HDRMGE_V10_OE_CURVE_NUM (17)
#define HDRMGE_V10_MD_CURVE_NUM (17)
#define HDRMGE_V11_OE_CURVE_NUM HDRMGE_V10_OE_CURVE_NUM
#define HDRMGE_V11_MD_CURVE_NUM HDRMGE_V10_MD_CURVE_NUM
#define HDRMGE_V12_OE_CURVE_NUM HDRMGE_V10_OE_CURVE_NUM
#define HDRMGE_V12_MD_CURVE_NUM HDRMGE_V10_MD_CURVE_NUM

typedef struct MgeProcResV10_s {
    unsigned char sw_hdrmge_mode;
    unsigned short sw_hdrmge_gain0_inv;
    unsigned short sw_hdrmge_gain0;
    unsigned short sw_hdrmge_gain1_inv;
    unsigned short sw_hdrmge_gain1;
    unsigned char sw_hdrmge_gain2;
    unsigned char sw_hdrmge_ms_dif_0p8;
    unsigned char sw_hdrmge_lm_dif_0p9;
    unsigned char sw_hdrmge_ms_dif_0p15;
    unsigned char sw_hdrmge_lm_dif_0p15;
    unsigned short sw_hdrmge_l0_y[HDRMGE_V10_MD_CURVE_NUM];
    unsigned short sw_hdrmge_l1_y[HDRMGE_V10_MD_CURVE_NUM];
    unsigned short sw_hdrmge_e_y[HDRMGE_V10_OE_CURVE_NUM];
} MgeProcResV10_t;

typedef struct MgeProcResV11_s {
    unsigned char sw_hdrmge_s_base;
    unsigned char sw_hdrmge_mode;
    unsigned short sw_hdrmge_gain0_inv;
    unsigned short sw_hdrmge_gain0;
    unsigned short sw_hdrmge_gain1_inv;
    unsigned short sw_hdrmge_gain1;
    unsigned char sw_hdrmge_gain2;
    unsigned char sw_hdrmge_ms_dif_0p8;
    unsigned char sw_hdrmge_lm_dif_0p9;
    unsigned char sw_hdrmge_ms_dif_0p15;
    unsigned char sw_hdrmge_lm_dif_0p15;
    unsigned short sw_hdrmge_l0_y[HDRMGE_V11_MD_CURVE_NUM];
    unsigned short sw_hdrmge_l1_y[HDRMGE_V11_MD_CURVE_NUM];
    unsigned short sw_hdrmge_e_y[HDRMGE_V11_OE_CURVE_NUM];
    unsigned short sw_hdrmge_ms_thd1;
    unsigned short sw_hdrmge_ms_thd0;
    unsigned short sw_hdrmge_ms_scl;
    unsigned short sw_hdrmge_lm_thd1;
    unsigned short sw_hdrmge_lm_thd0;
    unsigned short sw_hdrmge_lm_scl;
} MgeProcResV11_t;

typedef struct MgeProcResV12_s {
    unsigned char s_base;
    unsigned char mode;
    unsigned char dbg_mode;
    unsigned char each_raw_en;
    unsigned char lm_dif_0p15;
    unsigned char lm_dif_0p9;
    unsigned char ms_dif_0p15;
    unsigned char ms_dif_0p8;
    unsigned short gain0_inv;
    unsigned short gain0;
    unsigned short gain1_inv;
    unsigned short gain1;
    unsigned char gain2;
    unsigned short ms_thd1;
    unsigned short ms_thd0;
    unsigned short ms_scl;
    unsigned short lm_thd1;
    unsigned short lm_thd0;
    unsigned short lm_scl;
    unsigned short l0_y[HDRMGE_V12_MD_CURVE_NUM];
    unsigned short l1_y[HDRMGE_V12_MD_CURVE_NUM];
    unsigned short e_y[HDRMGE_V12_OE_CURVE_NUM];
    unsigned short l_raw0[HDRMGE_V12_OE_CURVE_NUM];
    unsigned short l_raw1[HDRMGE_V12_OE_CURVE_NUM];
    unsigned short each_raw_gain0;
    unsigned short each_raw_gain1;
} MgeProcResV12_t;

typedef struct RkAiqAmergeProcResult_s {
#if RKAIQ_HAVE_MERGE_V10
    MgeProcResV10_t Merge_v10;
#endif
#if RKAIQ_HAVE_MERGE_V11
    MgeProcResV11_t Merge_v11;
#endif
#if RKAIQ_HAVE_MERGE_V12
    MgeProcResV12_t Merge_v12;
#endif
} RkAiqAmergeProcResult_t;

#endif  //_RK_AIQ_ALGO_AMERGE_HW_H_
