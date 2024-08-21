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

#ifndef __AF_UAPI_HEAD_H__
#define __AF_UAPI_HEAD_H__


#include "rk_aiq_user_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rk_tool_customAf_res_s {
    // M4_NUMBER_DESC("af enable", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char af_en;
    // M4_NUMBER_DESC("af selection", "u8", M4_RANGE(0, 3), "0", M4_DIGIT(0))
    unsigned char rawaf_sel;
    // M4_NUMBER_DESC("gamma enable", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char gamma_en;
    // M4_NUMBER_DESC("gause enable", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char gaus_en;
    // M4_NUMBER_DESC("v1 fir selection", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char v1_fir_sel;
    // M4_NUMBER_DESC("hiir enable", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char hiir_en;
    // M4_NUMBER_DESC("viir enable", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char viir_en;
    // M4_NUMBER_DESC("v1 fv mode", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char v1_fv_outmode;
    // M4_NUMBER_DESC("v2 fv mode", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char v2_fv_outmode;
    // M4_NUMBER_DESC("h1 fv mode", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char h1_fv_outmode;
    // M4_NUMBER_DESC("h2 fv mode", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char h2_fv_outmode;
    // M4_NUMBER_DESC("ldg enable", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char ldg_en;
    // M4_NUMBER_DESC("accu 8bit mode", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char accu_8bit_mode;
    // M4_NUMBER_DESC("ae mode", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned char ae_mode;
    // M4_NUMBER_DESC("y mode", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char y_mode;
    // M4_ARRAY_DESC("line enable", "u8", M4_SIZE(1,5), M4_RANGE(0,1), "0", M4_DIGIT(0), M4_DYNAMIC(0), M4_HIDE(1))
    unsigned char line_en[5];
    // M4_ARRAY_DESC("line number", "u8", M4_SIZE(1,5), M4_RANGE(0,15), "0", M4_DIGIT(0), M4_DYNAMIC(0), M4_HIDE(1))
    unsigned char line_num[5];

    // M4_NUMBER_DESC("window num", "u8", M4_RANGE(1, 2), "0", M4_DIGIT(0))
    unsigned char window_num;
    // M4_NUMBER_DESC("wina h offs", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short wina_h_offs;
    // M4_NUMBER_DESC("wina v offs", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short wina_v_offs;
    // M4_NUMBER_DESC("wina h size", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short wina_h_size;
    // M4_NUMBER_DESC("wina v size", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short wina_v_size;
    // M4_NUMBER_DESC("winb h offs", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short winb_h_offs;
    // M4_NUMBER_DESC("winb v offs", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short winb_v_offs;
    // M4_NUMBER_DESC("winb h size", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short winb_h_size;
    // M4_NUMBER_DESC("winb v size", "u16", M4_RANGE(0, 8192), "0", M4_DIGIT(0))
    unsigned short winb_v_size;

    // M4_ARRAY_MARK_DESC("Gamma Curve", "u16", M4_SIZE(1,17), M4_RANGE(0, 1023), "[0,45,108,179,245,344,409,459,500,567,622,676,759,833,896,962,1023]", M4_DIGIT(0), M4_DYNAMIC(0), "curve_table")
    unsigned short gamma_y[17];

    // M4_NUMBER_DESC("afmThres", "u16", M4_RANGE(0, 65535), "4", M4_DIGIT(0))
    unsigned short thres;
    // M4_NUMBER_DESC("wina fv shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char shift_sum_a;
    // M4_NUMBER_DESC("winb fv shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0))
    unsigned char shift_sum_b;
    // M4_NUMBER_DESC("wina luma shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0), M4_HIDE(1))
    unsigned char shift_y_a;
    // M4_NUMBER_DESC("winb luma shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0))
    unsigned char shift_y_b;

    // M4_ARRAY_DESC("vertical first iir filter", "s16", M4_SIZE(1,9), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short v1_iir_coe[9];
    // M4_ARRAY_DESC("vertical first fir filter", "s16", M4_SIZE(1,3), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short v1_fir_coe[3];
    // M4_ARRAY_DESC("vertical second iir filter", "s16", M4_SIZE(1,3), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short v2_iir_coe[3];
    // M4_ARRAY_DESC("vertical second fir filter", "s16", M4_SIZE(1,3), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short v2_fir_coe[3];

    // M4_ARRAY_DESC("horizontal first iir1 filter", "s16", M4_SIZE(1,6), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short h1_iir1_coe[6];
    // M4_ARRAY_DESC("horizontal first iir2 filter", "s16", M4_SIZE(1,6), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short h2_iir1_coe[6];
    // M4_ARRAY_DESC("horizontal second iir1 filter", "s16", M4_SIZE(1,6), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short h1_iir2_coe[6];
    // M4_ARRAY_DESC("horizontal second iir2 filter", "s16", M4_SIZE(1,6), M4_RANGE(-2047,2047), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    short h2_iir2_coe[6];

    // M4_ARRAY_DESC("horizontal thresh", "u8", M4_SIZE(1,2), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char h_ldg_lumth[2];
    // M4_ARRAY_DESC("horizontal gain", "u8", M4_SIZE(1,2), M4_RANGE(0, 255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char h_ldg_gain[2];
    // M4_ARRAY_DESC("horizontal slope", "u16", M4_SIZE(1,2), M4_RANGE(0, 8191), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short h_ldg_gslp[2];
    // M4_ARRAY_DESC("vertical thresh", "u8", M4_SIZE(1,2), M4_RANGE(0,255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char v_ldg_lumth[2];
    // M4_ARRAY_DESC("vertical gain", "u8", M4_SIZE(1,2), M4_RANGE(0, 255), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned char v_ldg_gain[2];
    // M4_ARRAY_DESC("vertical slope", "u16", M4_SIZE(1,2), M4_RANGE(0, 8191), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    unsigned short v_ldg_gslp[2];

    // M4_NUMBER_DESC("vertical fv thresh", "u16", M4_RANGE(0, 4095), "0", M4_DIGIT(0))
    unsigned short v_fv_thresh;
    // M4_NUMBER_DESC("horizontal fv thresh", "u16", M4_RANGE(0, 4095), "0", M4_DIGIT(0))
    unsigned short h_fv_thresh;

    // M4_NUMBER_DESC("wina v1 shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0))
    unsigned char v1_fv_shift;
    // M4_NUMBER_DESC("wina v2 shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0))
    unsigned char v2_fv_shift;
    // M4_NUMBER_DESC("wina h1 shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0))
    unsigned char h1_fv_shift;
    // M4_NUMBER_DESC("wina h2 shift", "u8", M4_RANGE(0, 7), "0", M4_DIGIT(0))
    unsigned char h2_fv_shift;

    // M4_NUMBER_DESC("highlight thresh", "u16", M4_RANGE(0, 4095), "0", M4_DIGIT(0))
    unsigned short highlit_thresh;
}  rk_tool_customAf_res_t;

#ifdef __cplusplus
}
#endif


#endif  /*__AF_UAPI_HEAD_H__*/
