/*
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

#ifndef __CCM_UAPI_HEAD_H__
#define __CCM_UAPI_HEAD_H__


#ifdef __cplusplus
extern "C" {
#endif

#define RK_AIQ_ACCM_COLOR_GAIN_NUM 4
#ifndef CCM_CURVE_DOT_NUM
#define CCM_CURVE_DOT_NUM 17
#endif
#ifndef CCM_CURVE_DOT_NUM_V2
#define CCM_CURVE_DOT_NUM_V2 18
#endif

typedef struct rk_aiq_ccm_mccm_attrib_s {
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "[1,0,0,0,1,0,0,0,1]", M4_DIGIT(4), M4_DYNAMIC(0))
    float  ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float  ccOffsets[3];
    // M4_ARRAY_DESC("y_alpha_curve", "f32", M4_SIZE(1,17), M4_RANGE(-4095,4095), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    float  y_alpha_curve[CCM_CURVE_DOT_NUM];
    // M4_NUMBER_DESC("bound pos bit", "f32", M4_RANGE(4, 10), "8", M4_DIGIT(0))
    float low_bound_pos_bit;
} rk_aiq_ccm_mccm_attrib_t;

typedef struct rk_aiq_ccm_mccm_attrib_v2_s {
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "[1,0,0,0,1,0,0,0,1]", M4_DIGIT(4), M4_DYNAMIC(0))
    float ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(1), M4_DYNAMIC(0))
    float ccOffsets[3];
    // M4_BOOL_DESC("high Y adjust enable", "1")
    bool highy_adj_en;
    // M4_BOOL_DESC("asym enable", "0")
    bool asym_enable;
    // M4_NUMBER_DESC("left bound pos bit", "f32", M4_RANGE(3, 11), "10", M4_DIGIT(0))
    float bound_pos_bit;  // low y alpha adjust
    // M4_NUMBER_DESC("right bound pos bit", "f32", M4_RANGE(3, 11), "10", M4_DIGIT(0))
    float right_pos_bit;  // high y alpha adjust
    // M4_ARRAY_DESC("y alpha curve", "f32", M4_SIZE(1,18), M4_RANGE(-4095,4095), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    float y_alpha_curve[CCM_CURVE_DOT_NUM_V2];
    // M4_NUMBER_DESC("ccm enhance enable", "u8", M4_RANGE(0, 1), "0", M4_DIGIT(0))
    unsigned short enh_adj_en;
    // M4_ARRAY_DESC("Enhance RGB2Y para", "u8", M4_SIZE(1,3), M4_RANGE(0,128), "[38 75 15]", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned char enh_rgb2y_para[3];
    // M4_NUMBER_DESC("Enhance ratio max", "f32", M4_RANGE(0, 8), "0", M4_DIGIT(1))
    float enh_rat_max;
} rk_aiq_ccm_mccm_attrib_v2_t;

typedef enum rk_aiq_ccm_op_mode_s {
    RK_AIQ_CCM_MODE_INVALID                     = 0,        /**< initialization value */
    RK_AIQ_CCM_MODE_MANUAL                      = 1,        /**< run manual lens shading correction */
    RK_AIQ_CCM_MODE_AUTO                        = 2,        /**< run auto lens shading correction */
    RK_AIQ_CCM_MODE_MAX
} rk_aiq_ccm_op_mode_t;

typedef struct rk_aiq_ccm_querry_info_s {
    bool ccm_en;
    // M4_ARRAY_DESC("ccMatrix", "f32", M4_SIZE(3,3), M4_RANGE(-8,7.992), "[1,0,0,0,1,0,0,0,1]", M4_DIGIT(4), M4_DYNAMIC(0), "0", "1")
    float ccMatrix[9];
    // M4_ARRAY_DESC("ccOffsets", "f32", M4_SIZE(1,3), M4_RANGE(-4095,4095), "0", M4_DIGIT(0), M4_DYNAMIC(0), "0", "1")
    float ccOffsets[3];
    bool highy_adj_en;
    bool asym_enable;
    float y_alpha_curve[CCM_CURVE_DOT_NUM_V2];
    // M4_NUMBER_DESC("bound pos bit", "f32", M4_RANGE(3, 11), "8", M4_DIGIT(0), "0", "1")
    float low_bound_pos_bit;
    // M4_NUMBER_DESC("high bound pos bit", "f32", M4_RANGE(3, 11), "8", M4_DIGIT(0), "0", "1")
    float right_pos_bit;
    float color_inhibition_level;
    float color_saturation_level;
    // M4_NUMBER_DESC("CCM Saturation", "f32", M4_RANGE(0,200), "0", M4_DIGIT(2), "0", "1")
    float finalSat;
    // M4_STRING_DESC("usedCcm1", M4_SIZE(1,1), M4_RANGE(0, 25), "A_100",M4_DYNAMIC(0), "0", "1")
    char ccmname1[25];
    // M4_STRING_DESC("usedCcm2", M4_SIZE(1,1), M4_RANGE(0, 25), "A_100",M4_DYNAMIC(0), "0", "1")
    char ccmname2[25];
} rk_aiq_ccm_querry_info_t;


#ifdef __cplusplus
}
#endif


#endif  /*__AWB_UAPI_HEAD_H__*/
