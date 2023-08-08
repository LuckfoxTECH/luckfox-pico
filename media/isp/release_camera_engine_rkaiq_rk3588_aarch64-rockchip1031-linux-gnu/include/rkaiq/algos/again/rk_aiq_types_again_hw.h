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

#ifndef _RK_AIQ_TYPE_AGAIN_HW_V1_H_
#define _RK_AIQ_TYPE_AGAIN_HW_V1_H_
#include "rk_aiq_comm.h"

typedef struct RK_GAIN_Fix_V1_s{
	unsigned char gain_table_en;
    unsigned char dhaz_en;
    unsigned char wdr_en;
    unsigned char tmo_en;
    unsigned char lsc_en;
    unsigned char mge_en;

    unsigned int mge_gain[3];
    unsigned short int idx[15];
    unsigned short int lut[17];
}RK_GAIN_Fix_V1_t;



#endif



