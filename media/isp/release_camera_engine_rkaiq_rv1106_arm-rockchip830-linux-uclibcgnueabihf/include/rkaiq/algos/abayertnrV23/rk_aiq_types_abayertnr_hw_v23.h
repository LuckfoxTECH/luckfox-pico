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

#ifndef _RK_AIQ_TYPE_ABAYERTNR_HW_V23_H_
#define _RK_AIQ_TYPE_ABAYERTNR_HW_V23_H_
#include "rk_aiq_comm.h"




typedef struct RK_Bayertnr_Fix_V23_s {

    /* BAY3D_CTRL */
    uint8_t soft_st;
    uint8_t soft_mode;
    uint8_t bay3d_en;
    uint8_t bypass_en;
    uint8_t hibypass_en;
    uint8_t lobypass_en;
    uint8_t himed_bypass_en;
    uint8_t higaus_bypass_en;
    uint8_t hiabs_possel;
    uint8_t hichnsplit_en;
    uint8_t lomed_bypass_en;
    uint8_t logaus5_bypass_en;
    uint8_t logaus3_bypass_en;
    uint8_t glbpk_en;
    uint8_t loswitch_protect;
    uint8_t bwsaving_en;

    /* BAY3D_KALRATIO */
    uint16_t softwgt;
    uint16_t hidif_th;

    /* BAY3D_GLBPK2 */
    uint32_t glbpk2;

    /* BAY3D_CTRL1 */
    uint8_t hiwgt_opt_en;
    uint8_t hichncor_en;
    uint8_t bwopt_gain_dis;
    uint8_t lo4x8_en;
    uint8_t lo4x4_en;
    uint8_t hisig_ind_sel;
    uint8_t pksig_ind_sel;
    uint8_t iirwr_rnd_en;
    uint8_t curds_high_en;
    uint8_t higaus3_mode;
    uint8_t higaus5x5_en;
    uint8_t wgtmix_opt_en;


    /* BAY3D_WGTLMT */
    uint16_t wgtlmt;
    uint16_t wgtratio;

    /* BAY3D_SIG */
    uint16_t sig0_x[16];
    uint16_t sig0_y[16];
    uint16_t sig1_x[16];
    uint16_t sig1_y[16];
    uint16_t sig2_y[16];

    /* BAY3D_HISIGRAT */
    uint16_t hisigrat0;
    uint16_t hisigrat1;
    /* BAY3D_HISIGOFF */
    uint16_t hisigoff0;
    uint16_t hisigoff1;
    /* BAY3D_LOSIG */
    uint16_t losigoff;
    uint16_t losigrat;
    /* BAY3D_SIGPK */
    uint16_t rgain_off;
    uint16_t bgain_off;

    /* BAY3D_SIGGAUS */
    uint8_t siggaus0;
    uint8_t siggaus1;
    uint8_t siggaus2;
    uint8_t siggaus3;



} RK_Bayertnr_Fix_V23_t;



#endif



