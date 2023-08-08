/*
 * rk_aiq_types_acp_algo_int.h
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

#ifndef _RK_AIQ_TYPES_AIE_ALGO_INT_H_
#define _RK_AIQ_TYPES_AIE_ALGO_INT_H_

#include "rk_aiq_types_aie_algo.h"

typedef struct rk_aiq_aie_params_int_s {
    unsigned char mode_coeffs[9];
    /* range: 0 to 15.75*/
    float sharp_factor; /*!< only used by SHARPEN mode */
    /*
    Threshold for coring function. This value is used to avoid amplifying noise too much by suppressing sharpening
    for small gradients. Higher value means less sharpening for smooth edges. Threshold zero means no coring,
    so all gradients are treated the same. Threshold 255 means nearly no sharpening. An absolute value for the
    highpass signal is defined here. The highpass signal is truncated at the defined level.
    */
    unsigned char sharp_thres; /*!< only used by SHARPEN mode */
} rk_aiq_aie_params_int_t;

#endif
