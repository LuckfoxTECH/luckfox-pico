/*
 * Copyright (c) 2019, Fuzhou Rockchip Electronics Co., Ltd
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
#ifndef _CAM_HW_RK_ISP20_HW_H
#define _CAM_HW_RK_ISP20_HW_H

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

#define BIT_ULL(nr)            (1ULL << (nr))

enum RK_ISP_HW_MODULE_ID_e {
    RK_ISP2X_DPCC_ID,
    RK_ISP2X_BLS_ID,
    RK_ISP2X_SDG_ID,
    RK_ISP2X_SIHST_ID,
    RK_ISP2X_LSC_ID,
    RK_ISP2X_AWB_GAIN_ID,
    RK_ISP2X_RESERVED_ID,
    RK_ISP2X_BDM_ID,
    RK_ISP2X_CTK_ID,
    RK_ISP2X_GOC_ID,
    RK_ISP2X_CPROC_ID,
    RK_ISP2X_SIAF_ID,
    RK_ISP2X_SIAWB_ID,
    RK_ISP2X_IE_ID,
    RK_ISP2X_YUVAE_ID,
    RK_ISP2X_WDR_ID,
    RK_ISP2X_RK_IESHARP_ID,
    RK_ISP2X_RAWAF_ID,
    RK_ISP2X_RAWAE0_ID,
    RK_ISP2X_RAWAE1_ID,
    RK_ISP2X_RAWAE2_ID,
    RK_ISP2X_RAWAE3_ID,
    RK_ISP2X_RAWAWB_ID,
    RK_ISP2X_RAWHIST0_ID,
    RK_ISP2X_RAWHIST1_ID,
    RK_ISP2X_RAWHIST2_ID,
    RK_ISP2X_RAWHIST3_ID,
    RK_ISP2X_HDRMGE_ID,
    RK_ISP2X_RAWNR_ID,
    RK_ISP2X_HDRTMO_ID,
    RK_ISP2X_GIC_ID,
    RK_ISP2X_DHAZ_ID,
    RK_ISP2X_3DLUT_ID,
    RK_ISP2X_LDCH_ID,
    RK_ISP2X_GAIN_ID,
    RK_ISP2X_DEBAYER_ID,
    Rk_ISP21_BAYNR_ID,
    Rk_ISP21_BAY3D_ID,
    Rk_ISP21_YNR_ID,
    Rk_ISP21_CNR_ID,
    Rk_ISP21_SHARP_ID,
    Rk_ISP21_DRC_ID,
    Rk_ISP3x_CAC_ID,
    Rk_ISP2x_CSM_ID,
    Rk_ISP2x_CGC_ID,
    RK_ISP2X_MAX_ID,
    RK_ISP2X_PP_TNR_ID,
    RK_ISP2X_PP_NR_ID,
    RK_ISP2X_PP_TSHP_ID,
    RK_ISP2X_PP_TFEC_ID,
    RK_ISP2X_PP_ORB_ID,
    RK_ISP2X_PP_MAX_ID,
};

#endif
