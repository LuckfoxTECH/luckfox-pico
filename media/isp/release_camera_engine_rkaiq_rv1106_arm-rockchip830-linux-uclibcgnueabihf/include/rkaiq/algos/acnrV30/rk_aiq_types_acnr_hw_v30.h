/*
 *rk_aiq_types_alsc_hw.h
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

#ifndef _RK_AIQ_TYPE_ACNR_HW_V30_H_
#define _RK_AIQ_TYPE_ACNR_HW_V30_H_
#include "rk_aiq_comm.h"

typedef struct RK_CNR_Fix_V30_s {
    /* CNR_CTRL */
    uint8_t  cnr_en;
    uint8_t  exgain_bypass;
    uint8_t  yuv422_mode;
    uint8_t  thumb_mode;
    uint8_t  bf3x3_wgt0_sel;

    /* CNR_EXGAIN */
    uint8_t  gain_iso;
    uint8_t  global_gain_alpha;
    uint16_t global_gain;

    /* CNR_THUMB1 */
    uint16_t thumb_sigma_c;
    uint16_t thumb_sigma_y;

    /* CNR_THUMB_BF_RATIO */
    uint16_t thumb_bf_ratio;

    /* CNR_LBF_WEITD */
    uint8_t  lbf1x7_weit_d[4];

    /* CNR_IIR_PARA1 */
    uint8_t  iir_uvgain;
    uint8_t  iir_strength;
    uint8_t  exp_shift;
    uint16_t wgt_slope;

    /* CNR_IIR_PARA2 */
    uint8_t  chroma_ghost;
    uint8_t  iir_uv_clip;

    /* CNR_GAUS_COE */
    uint8_t  gaus_coe[6];

    /* CNR_GAUS_RATIO */
    uint16_t gaus_ratio;
    uint8_t  bf_wgt_clip;
    uint16_t global_alpha;

    /* CNR_BF_PARA1 */
    uint8_t  uv_gain;
    uint16_t sigma_r;
    uint8_t  bf_ratio;

    /* CNR_BF_PARA2 */
    uint16_t adj_offset;
    uint16_t adj_ratio;

    /* CNR_SIGMA */
    uint8_t  sigma_y[13];

    /* CNR_IIR_GLOBAL_GAIN */
    uint8_t  iir_gain_alpha;
    uint8_t  iir_global_gain;

} RK_CNR_Fix_V30_t;



#endif



