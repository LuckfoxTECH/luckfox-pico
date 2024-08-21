/*
 * rk_aiq_types_adebayer_ext.h
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

#ifndef __RK_AIQ_TYPES_ADEBAYER_EXT_H__
#define __RK_AIQ_TYPES_ADEBAYER_EXT_H__

#include "rk_aiq_comm.h"

#define UAPI_DEBAYER_ISO_LEN 13

typedef enum rk_aiq_debayer_op_mode_s {
    RK_AIQ_DEBAYER_MODE_INVALID                     = 0,        /**< initialization value */
    RK_AIQ_DEBAYER_MODE_MANUAL                      = 1,        /**< run manual lens shading correction */
    RK_AIQ_DEBAYER_MODE_AUTO                        = 2,        /**< run auto lens shading correction */
    RK_AIQ_DEBAYER_MODE_MAX
} rk_aiq_debayer_op_mode_t;

typedef struct adebayer_attrib_auto_s {
    // M4_ARRAY_DESC("debayer_filter1", "s8", M4_SIZE(1,5),  M4_RANGE(-128, 128), "[2,-6,0,6,-2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int debayer_filter1[5];

    // M4_ARRAY_DESC("debayer_filter2", "s8", M4_SIZE(1,5),  M4_RANGE(-128, 128), "[2,-4,4,-4,2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int debayer_filter2[5];

    // M4_ARRAY_DESC("ISO", "u32", M4_SIZE(1,13),  M4_RANGE(0, 204800), "[50,100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int ISO[13];

    // M4_ARRAY_DESC("debayer_gain_offset", "u8",M4_SIZE(1,13),  M4_RANGE(0,15), "4", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_gain_offset[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_offset", "u8",M4_SIZE(1,13),  M4_RANGE(0,31), "1", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_offset[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_clip_en", "u8", M4_SIZE(1,13),  M4_RANGE(0,1),"1", M4_DYNAMIC(0), 0)
    unsigned char debayer_clip_en[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_filter_g_en", "u8",M4_SIZE(1,13), M4_RANGE(0,1), "1", M4_DYNAMIC(0), 0)
    unsigned char debayer_filter_g_en[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_filter_c_en", "u8", M4_SIZE(1,13), M4_RANGE(0,1),"1", M4_DYNAMIC(0), 0)
    unsigned char debayer_filter_c_en[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_thed0", "u8", M4_SIZE(1,13), M4_RANGE(0,15), "3", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_thed0[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_thed1", "u8", M4_SIZE(1,13), M4_RANGE(0,15), "6", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_thed1[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_dist_scale", "u8", M4_SIZE(1,13), M4_RANGE(0,15), "8", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_dist_scale[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_cnr_strength", "u8", M4_SIZE(1,13), M4_RANGE(0,9), "5", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char debayer_cnr_strength[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_shift_num", "u8", M4_SIZE(1,13), M4_RANGE(0,3), "2", M4_DIGIT(0),  M4_DYNAMIC(0),0)
    unsigned char debayer_shift_num[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_sharp_strength", "u8", M4_SIZE(1,13),  M4_RANGE(0, 7), "[4,4,4,4,4,4,4,4,4,4,4,4,4]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned char sharp_strength[UAPI_DEBAYER_ISO_LEN];

    // M4_ARRAY_DESC("debayer_hf_offset", "u16", M4_SIZE(1,13),  M4_RANGE(0, 65535), "[1,1,1,1,1,1,1,1,1,1,1,1,1]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    unsigned short debayer_hf_offset[UAPI_DEBAYER_ISO_LEN];

} adebayer_attrib_auto_t;

typedef struct adebayer_attrib_manual_s {

    // M4_ARRAY_DESC("filter1", "s8", M4_SIZE(1,5),  M4_RANGE(-8, 7), "[2,-6,0,6,-2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int8_t      filter1[5];

    // M4_ARRAY_DESC("filter2", "s8", M4_SIZE(1,5),  M4_RANGE(-8, 7), "[2,-6,0,6,-2]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int8_t      filter2[5];

    // M4_NUMBER_DESC("gain_offset", "u8", M4_RANGE(0,15), "4", M4_DIGIT(0), 0)
    uint8_t     gain_offset;

    // M4_NUMBER_DESC("sharp_strength", "u8", M4_RANGE(0, 7), "4", M4_DIGIT(0), 0)
    uint8_t     sharp_strength;

    // M4_NUMBER_DESC("hf_offset", "u16", M4_RANGE(0, 65535), "1", M4_DIGIT(0), 0)
    uint16_t     hf_offset;

    // M4_NUMBER_DESC("offset", "u8", M4_RANGE(0,31), "1", M4_DIGIT(0), 0)
    uint8_t     offset;

    // M4_NUMBER_DESC("clip_en", "u8", M4_RANGE(0,1), "1", M4_DIGIT(0), 0)
    uint8_t     clip_en;

    // M4_NUMBER_DESC("filter_g_en", "u8", M4_RANGE(0,1), "1", M4_DIGIT(0), 0)
    uint8_t     filter_g_en;

    // M4_NUMBER_DESC("filter_c_en","u8",  M4_RANGE(0,1), "1", M4_DIGIT(0), 0)
    uint8_t     filter_c_en;

    // M4_NUMBER_DESC("thed0", "u8", M4_RANGE(0,15), "3", M4_DIGIT(0), 0)
    uint8_t     thed0;

    // M4_NUMBER_DESC("thed1", "u8", M4_RANGE(0,15), "3", M4_DIGIT(0), 0)
    uint8_t     thed1;

    // M4_NUMBER_DESC("dist_scale", "u8", M4_RANGE(0,15), "3", M4_DIGIT(0), 0)
    uint8_t     dist_scale;

    // M4_NUMBER_DESC("cnr_strength", "u8", M4_RANGE(0,9), "3", M4_DIGIT(0), 0)
    uint8_t     cnr_strength;

    // M4_NUMBER_DESC("shift_num", "u8", M4_RANGE(0,3), "2", M4_DIGIT(0), 0)
    uint8_t     shift_num;
} adebayer_attrib_manual_t;

typedef struct adebayer_attrib_s {
    rk_aiq_uapi_sync_t          sync;

    // M4_NUMBER_DESC("enable", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0))
    uint8_t                     enable;

    // M4_ENUM_DESC("mode", "rk_aiq_debayer_op_mode_t","RK_AIQ_DEBAYER_MODE_AUTO")
    rk_aiq_debayer_op_mode_t    mode;

    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    adebayer_attrib_manual_t    stManual;

    // M4_STRUCT_DESC("stAuto", "normal_ui_style")
    adebayer_attrib_auto_t      stAuto;
} adebayer_attrib_t;

#endif //__RK_AIQ_TYPES_ADEBAYER_EXT_H__
