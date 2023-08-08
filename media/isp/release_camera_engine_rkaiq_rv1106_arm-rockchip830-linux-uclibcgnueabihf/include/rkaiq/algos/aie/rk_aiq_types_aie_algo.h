/*
 * rk_aiq_types_aie_algo.h
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

#ifndef _RK_AIQ_TYPES_AIE_ALGO_H_
#define _RK_AIQ_TYPES_AIE_ALGO_H_

typedef enum rk_aiq_ie_effect_e {
    RK_AIQ_IE_EFFECT_NONE,
    RK_AIQ_IE_EFFECT_BW,
    RK_AIQ_IE_EFFECT_NEGATIVE,
    RK_AIQ_IE_EFFECT_SEPIA,
    RK_AIQ_IE_EFFECT_EMBOSS,
    RK_AIQ_IE_EFFECT_SKETCH,
    RK_AIQ_IE_EFFECT_SHARPEN, /*!< deprecated */
} rk_aiq_ie_effect_t;

typedef struct rk_aiq_aie_params_s {
    rk_aiq_ie_effect_t mode;
} rk_aiq_aie_params_t;

typedef struct rk_aiq_aie_last_params_s {
    rk_aiq_ie_effect_t mode;
    int skip_frame;
} rk_aiq_aie_last_params_t;

#endif
