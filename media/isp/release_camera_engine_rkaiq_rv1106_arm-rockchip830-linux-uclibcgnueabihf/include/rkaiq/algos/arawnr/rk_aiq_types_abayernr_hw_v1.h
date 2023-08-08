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

#ifndef _RK_AIQ_TYPE_ABAYERNR_HW_V1_H_
#define _RK_AIQ_TYPE_ABAYERNR_HW_V1_H_
#include "rk_aiq_comm.h"



typedef struct RKAnr_Bayernr_Fix_V1_s {
    unsigned char rawnr_en;
    unsigned char gauss_en;
    unsigned char log_bypass;

    unsigned short filtpar0;
    unsigned short filtpar1;
    unsigned short filtpar2;

    unsigned int dgain0;
    unsigned int dgain1;
    unsigned int dgain2;

    unsigned short luration[8];
    unsigned short lulevel[8];

    unsigned int gauss;
    unsigned short sigma;
    unsigned short pix_diff;

    unsigned int thld_diff;

    unsigned char gas_weig_scl2;
    unsigned char gas_weig_scl1;
    unsigned short thld_chanelw;

    unsigned short lamda;

    unsigned short fixw0;
    unsigned short fixw1;
    unsigned short fixw2;
    unsigned short fixw3;

    unsigned int wlamda0;
    unsigned int wlamda1;
    unsigned int wlamda2;

    unsigned short rgain_filp;
    unsigned short bgain_filp;
} RK_Bayernr_Fix_V1_t;




#endif



