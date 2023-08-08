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

#ifndef _RK_AIQ_TYPE_ABAYER2DNR_HW_V23_H_
#define _RK_AIQ_TYPE_ABAYER2DNR_HW_V23_H_
#include "rk_aiq_comm.h"



typedef struct RK_Bayer2dnr_Fix_V23_s {
    /* BAYNR_CTRL */
    uint8_t bay3d_gain_en;
    uint8_t lg2_mode;
    uint8_t gauss_en;
    uint8_t log_bypass;
    uint8_t bayer_en;

    /* BAYNR_DGAIN */
    uint16_t dgain[3];

    /* BAYNR_PIXDIFF */
    uint16_t pix_diff;
    /* BAYNR_THLD */
    uint16_t diff_thld;
    uint16_t softthld;
    /* BAYNR_W1_STRENG */
    uint16_t bltflt_streng;
    uint16_t reg_w1;
    /* BAYNR_SIGMA */
    uint16_t sigma_x[16];
    uint16_t sigma_y[16];
    /* BAYNR_WRIT_D */
    uint16_t weit_d[3];
    /* BAYNR_LG_OFF */
    uint16_t lg2_lgoff;
    uint16_t lg2_off;
    /* BAYNR_DAT_MAX */
    uint32_t dat_max;
    /* BAYNR_SIGOFF */
    uint16_t rgain_off;
    uint16_t bgain_off;
    /* BAYNR_GAIN */
    uint8_t gain_x[16];
    uint16_t gain_y[16];
} RK_Bayer2dnr_Fix_V23_t;



#endif



