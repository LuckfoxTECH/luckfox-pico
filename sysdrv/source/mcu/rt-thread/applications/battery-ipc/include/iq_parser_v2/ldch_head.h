/*
 * ldch_head.h
 *
 *  Copyright (c) 2021 Rockchip Electronics Co., Ltd.
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

#ifndef __CALIBDBV2_LDCH_HEAD_H__
#define __CALIBDBV2_LDCH_HEAD_H__

#include <rk_aiq_comm.h>

RKAIQ_BEGIN_DECLARE

typedef struct CalibDbV2_Ldch_Param_s {
	// M4_BOOL_DESC("ldch_en", "0");
    bool ldch_en;
	// M4_STRING_DESC("meshfile", M4_SIZE(1,1), M4_RANGE(0, 256), "default_meshfile", M4_DYNAMIC(0))
    char meshfile[256];
	// M4_NUMBER_DESC("correct_level", "u8", M4_RANGE(0,255), "255", M4_DIGIT(0))
    unsigned char correct_level;
	// M4_NUMBER_DESC("correct_level_max", "u8", M4_RANGE(0,255), "255", M4_DIGIT(0))
    unsigned char correct_level_max;
	// M4_ARRAY_DESC("light_center", "f64", M4_SIZE(1,2), M4_RANGE(-10000000000000000,10000000000000000), "0", M4_DIGIT(20), M4_DYNAMIC(0))
    double light_center[2]; // light center
	// M4_ARRAY_DESC("coefficient", "f64", M4_SIZE(1,4), M4_RANGE(-10000000000000000,10000000000000000), "0", M4_DIGIT(20), M4_DYNAMIC(0))
    double coefficient[4]; // the distortion coefficient of the fisheye lens
} CalibDbV2_Ldch_Param_t;

typedef struct CalibDbV2_LDCH_s {
    // M4_STRUCT_DESC("param", "normal_ui_style")
    CalibDbV2_Ldch_Param_t param;
} CalibDbV2_LDCH_t;

RKAIQ_END_DECLARE

#endif
