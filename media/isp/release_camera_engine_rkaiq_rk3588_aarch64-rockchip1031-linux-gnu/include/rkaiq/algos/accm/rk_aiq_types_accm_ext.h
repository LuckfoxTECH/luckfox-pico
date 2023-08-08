/*
 * rk_aiq_types_accm_ext.h
 *
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef __RK_AIQ_TYPES_ACCM_EXT_H__
#define __RK_AIQ_TYPES_ACCM_EXT_H__

#include "rk_aiq_comm.h"
#define CCM_CURVE_DOT_NUM 17
#define RK_AIQ_ACCM_COLOR_GAIN_NUM 4

typedef enum rk_aiq_ccm_op_mode_s {
    RK_AIQ_CCM_MODE_INVALID                     = 0,        /**< initialization value */
    RK_AIQ_CCM_MODE_MANUAL                      = 1,        /**< run manual lens shading correction */
    RK_AIQ_CCM_MODE_AUTO                        = 2,        /**< run auto lens shading correction */
    RK_AIQ_CCM_MODE_MAX
} rk_aiq_ccm_op_mode_t;

typedef struct rk_aiq_ccm_color_inhibition_s {
    // M4_ARRAY_DESC("sensorGain", "f32", M4_SIZE(1,4), M4_RANGE(1,512), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    float sensorGain[RK_AIQ_ACCM_COLOR_GAIN_NUM];
    // M4_ARRAY_DESC("inhib_level", "f32", M4_SIZE(1,4), M4_RANGE(1,100), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float level[RK_AIQ_ACCM_COLOR_GAIN_NUM];//max value 100,default value 0
} rk_aiq_ccm_color_inhibition_t;

typedef struct rk_aiq_ccm_color_saturation_s {
    // M4_ARRAY_DESC("sensorGain", "f32", M4_SIZE(1,4), M4_RANGE(1,512), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    float sensorGain[RK_AIQ_ACCM_COLOR_GAIN_NUM];
    // M4_ARRAY_DESC("sat_level", "f32", M4_SIZE(1,4), M4_RANGE(1,100), "100", M4_DIGIT(1), M4_DYNAMIC(0))
    float level[RK_AIQ_ACCM_COLOR_GAIN_NUM];//max value 100, default value 100
} rk_aiq_ccm_color_saturation_t;

typedef struct rk_aiq_ccm_accm_attrib_s {
    // M4_STRUCT_DESC("color_inhibition", "normal_ui_style")
    rk_aiq_ccm_color_inhibition_t color_inhibition;
    // M4_STRUCT_DESC("color_saturation", "normal_ui_style")
    rk_aiq_ccm_color_saturation_t color_saturation;
} rk_aiq_ccm_accm_attrib_t;

typedef struct rk_aiq_ccm_mccm_attrib_s {
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "0", M4_DIGIT(4), M4_DYNAMIC(0))
    float  ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float  ccOffsets[3];
    // M4_ARRAY_DESC("y_alpha_curve", "f32", M4_SIZE(1,17), M4_RANGE(-4095,4095), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    float  y_alpha_curve[CCM_CURVE_DOT_NUM];
    // M4_NUMBER_DESC("bound pos bit", "f32", M4_RANGE(0, 10), "8", M4_DIGIT(0))
    float low_bound_pos_bit;
} rk_aiq_ccm_mccm_attrib_t;

typedef struct accm_uapi_s {
    rk_aiq_uapi_sync_t sync;
    // M4_NUMBER_DESC("bypass", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0))
    bool byPass;
    // M4_ENUM_DESC("mode", "rk_aiq_ccm_op_mode_t", "RK_AIQ_CCM_MODE_AUTO")
    rk_aiq_ccm_op_mode_t mode;
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    rk_aiq_ccm_mccm_attrib_t stManual;
    // M4_STRUCT_DESC("stAuto", "normal_ui_style")
    rk_aiq_ccm_accm_attrib_t stAuto;
} accm_uapi_t;


#endif /*__RK_AIQ_TYPES_ACCM_EXT_H__*/
