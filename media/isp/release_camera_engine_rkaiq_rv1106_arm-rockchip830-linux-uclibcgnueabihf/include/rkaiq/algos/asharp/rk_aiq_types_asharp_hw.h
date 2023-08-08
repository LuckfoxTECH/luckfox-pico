
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

#ifndef _RK_AIQ_TYPE_ASHARP_HW_H_
#define _RK_AIQ_TYPE_ASHARP_HW_H_


RKAIQ_BEGIN_DECLARE

typedef struct RKAsharp_Sharp_HW_Fix_s {
    unsigned char sharp_en;
    unsigned char yin_flt_en;
    unsigned char edge_avg_en;

    unsigned short hbf_ratio;
    unsigned char ehf_th;
    unsigned char pbf_ratio;

    unsigned char pbf_k[3];
    unsigned char mrf_k[6];
    unsigned char mbf_k[12];
    unsigned char hrf_k[6];
    unsigned char hbf_k[3];

    unsigned char lum_point[6];

    unsigned char pbf_shf_bits;
    unsigned char mbf_shf_bits;
    unsigned char hbf_shf_bits;

    unsigned char pbf_sigma[8];
    unsigned char lum_clp_m[8];
    signed char lum_min_m[8];
    unsigned char mbf_sigma[8];
    unsigned char lum_clp_h[8];
    unsigned char hbf_sigma[8];

    unsigned short rfl_ratio;
    unsigned short rfh_ratio;

    unsigned char m_ratio;
    unsigned char h_ratio;

} RKAsharp_Sharp_HW_Fix_t;

typedef struct RKAsharp_Sharp_HW_V2_Fix_s {
    void* place_holder[1];
} RKAsharp_Sharp_HW_V2_Fix_t;

typedef struct RKAsharp_Sharp_HW_V3_Fix_s {
    void* place_holder[1];
} RKAsharp_Sharp_HW_V3_Fix_t;


typedef struct RKAsharp_Sharp_Fix_s {
    RKAsharp_Sharp_HW_Fix_t stSharpFixV1;
    RKAsharp_Sharp_HW_V2_Fix_t stSharpFixV2;
    RKAsharp_Sharp_HW_V3_Fix_t stSharpFixV3;

} RKAsharp_Sharp_Fix_t;

typedef struct RKAsharp_Edgefilter_Fix_s {
    unsigned char edgeflt_en;

    unsigned char alpha_adp_en;

    unsigned char edge_thed;
    unsigned char dir_min;
    unsigned short smoth_th4;
    unsigned short l_alpha;
    unsigned short g_alpha;

    signed char eg_coef[3];
    unsigned char eg_smoth[3];
    unsigned char eg_gaus[6];
    signed char dog_k[6];

    unsigned char edge_lum_thed[8];
    unsigned char clamp_pos[8];
    unsigned char clamp_neg[8];
    unsigned char detail_alpha[8];

} RKAsharp_Edgefilter_Fix_t;

RKAIQ_END_DECLARE

#endif


