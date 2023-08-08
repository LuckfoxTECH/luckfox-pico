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

#ifndef _RK_AIQ_TYPE_ABAYERTNR_HW_V2_H_
#define _RK_AIQ_TYPE_ABAYERTNR_HW_V2_H_
#include "rk_aiq_comm.h"




typedef struct RK_Bayertnr_Fix_V2_s {

    // BAY3D_BAY3D_CTRL 0x2c00
    uint8_t bay3d_exp_sel;
    uint8_t bay3d_soft_st;
    uint8_t bay3d_soft_mode;
    uint8_t bay3d_bwsaving_en;
    uint8_t bay3d_loswitch_protect;
    uint8_t bay3d_glbpk_en;
    uint8_t bay3d_logaus3_bypass_en;
    uint8_t bay3d_logaus5_bypass_en;
    uint8_t bay3d_lomed_bypass_en;
    uint8_t bay3d_hichnsplit_en;
    uint8_t bay3d_hiabs_pssel;
    uint8_t bay3d_higaus_bypass_en;
    uint8_t bay3d_himed_bypass_en;
    uint8_t bay3d_lobypass_en;
    uint8_t bay3d_hibypass_en;
    uint8_t bay3d_bypass_en;
    uint8_t bay3d_en_i;

    // BAY3D_BAY3D_KALRATIO 0x2c04
    uint16_t bay3d_softwgt;
    uint16_t bay3d_hidif_th;

    // BAY3D_BAY3D_GLBPK2 0x2c08
    uint32_t bay3d_glbpk2;

    // BAY3D_BAY3D_WGTLMT 0x2c10
    uint16_t bay3d_wgtlmt;
    uint16_t bay3d_wgtratio;

    // BAY3D_BAY3D_SIG_X0  0x2c14 - 0x2c30
    uint16_t bay3d_sig0_x[16];

    // BAY3D_BAY3D_SIG0_Y0 0x2c34 - 0x2c50
    uint16_t bay3d_sig0_y[16];

    // BAY3D_BAY3D_SIG_X0  0x2c54 - 0x2c70
    uint16_t bay3d_sig1_x[16];

    // BAY3D_BAY3D_SIG1_Y0 0x2c74 - 0x2c90
    uint16_t bay3d_sig1_y[16];

    // BAY3D_BAY3D_SIG2_Y0 0x2c94 - 0x2cb0
    uint16_t bay3d_sig2_y[16];

    //BAY3D_BAY3D_LODIF_STAT0 0x2cb4 -0x2cb8
    uint64_t ro_sum_lodif;

    //BAY3D_BAY3D_LODIF_STAT0 0x2cbc -0x2cc0
    uint64_t ro_sum_hidif0;

    //BAY3D_BAY3D_MI_ST 0x2CC8
    uint8_t sw_bay3dmi_st_linemode;
    uint8_t sw_bay3d_mi2cur_linecnt;





} RK_Bayertnr_Fix_V2_t;



#endif



