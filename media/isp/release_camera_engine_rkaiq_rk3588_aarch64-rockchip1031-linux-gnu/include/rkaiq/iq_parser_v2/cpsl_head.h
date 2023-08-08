/*
 * cpsl_head.h
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

#ifndef __CALIBDBV2_CPSL_HEADER_H__
#define __CALIBDBV2_CPSL_HEADER_H__

#include <rk_aiq_comm.h>

RKAIQ_BEGIN_DECLARE

typedef enum Cpsl_LightSource_e {
  INVALID = -1,
  LED = 1,
  IR  = 2,
  MIX = 3, /*< led and ir mixture */
  MAX
} Cpsl_LightSource_t;

typedef struct CalibDbV2_Cpsl_Param_s {
  // M4_BOOL_DESC("enable", "0")
  bool enable;
  // M4_ENUM_DESC("mode", "RKAiqOPMode_t", "RK_AIQ_OP_MODE_AUTO")
  RKAiqOPMode_t mode;
  // M4_BOOL_DESC("force_gray", "0")
  bool force_gray;
  // M4_ENUM_DESC("light_src", "rk_aiq_cpsls_t", "LED")
  Cpsl_LightSource_t light_src;
  // M4_NUMBER_DESC("auto_adjust_sens", "f32", M4_RANGE(0.0,100.0), "50.0", M4_DIGIT(0))
  float auto_adjust_sens;
  // M4_NUMBER_DESC("auto_on2off_th", u32, M4_RANGE(0,65535), 3000, 0)
  uint32_t auto_on2off_th;
  // M4_NUMBER_DESC("auto_off2on_th", u32, M4_RANGE(0,65535), 100, 0)
  uint32_t auto_off2on_th;
  // M4_NUMBER_DESC("auto_sw_interval", u32, M4_RANGE(0,255), 60, 0)
  uint32_t auto_sw_interval;
  // M4_BOOL_DESC("manual_on", "0")
  bool manual_on;
  // M4_NUMBER_DESC("manual_strength", "f32", M4_RANGE(0.0,100.0), "100.0", M4_DIGIT(2))
  float manual_strength;

} CalibDbV2_Cpsl_Param_t;

typedef struct CalibDbV2_Cpsl_s {
    // M4_STRUCT_DESC("param", "normal_ui_style")
    CalibDbV2_Cpsl_Param_t param;
} CalibDbV2_Cpsl_t;

RKAIQ_END_DECLARE

#endif

