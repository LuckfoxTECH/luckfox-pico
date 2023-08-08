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

#ifndef _RK_AIQ_TYPE_AGAIN_HW_V2_H_
#define _RK_AIQ_TYPE_AGAIN_HW_V2_H_
#include "rk_aiq_comm.h"

typedef struct RK_GAIN_Fix_V2_s {

    unsigned char sw_gain2ddr_mode;
    unsigned char sw_gain2ddr_wr_en;
    unsigned char sw_3dlut_gain_en;
    unsigned char sw_dhaz_gain_en;
    unsigned char sw_adrc_gain_en;
    unsigned char sw_lsc_gain_en;
    unsigned char sw_gain_module_free_mode;
    unsigned char sw_gain_dmard_mode_en;
    unsigned char sw_bayer3dnr_gain_en;
    unsigned char sw_gain_mp_pipe_dis;
    unsigned char sw_gain_gate_always_on;
    unsigned char sw_mge_gain_en;
    unsigned char sw_gain_en;

    unsigned int sw_gain[3];
} RK_GAIN_Fix_V2_t;



#endif



