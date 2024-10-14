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

#ifndef __ADRC_UAPI_COMPACT_H__
#define __ADRC_UAPI_COMPACT_H__

#include "adrc_head.h"
#include "adrc_uapi_head.h"

typedef enum AdrcVersion_e {
    ADRC_VERSION_356X = 0,
    ADRC_VERSION_3588 = 1,
    ADRC_VERSION_MAX
} AdrcVersion_t;

typedef CalibDbV2_drc_V10_t adrcAttr_V21_t;
typedef CalibDbV2_drc_V11_t adrcAttr_V30_t;
typedef mdrcAttr_V10_t mdrcAttr_V21_t;
typedef mdrcAttr_V11_t mdrcAttr_V30_t;
typedef DrcInfo_t DrcCtrlInfo_t;

typedef struct DrcInfoV30_s {
    // M4_STRUCT_DESC("CtrlInfo", "normal_ui_style")
    DrcCtrlInfo_t CtrlInfo;
    // M4_STRUCT_DESC("ValidParamsV21", "normal_ui_style")
    mdrcAttr_V21_t ValidParamsV21;
    // M4_STRUCT_DESC("ValidParamsV30", "normal_ui_style")
    mdrcAttr_V30_t ValidParamsV30;
} DrcInfoV30_t;

typedef struct drcAttr_s {
    rk_aiq_uapi_sync_t sync;

    AdrcVersion_t Version;
    drc_OpMode_t opMode;
    adrcAttr_V21_t stAutoV21;
    adrcAttr_V30_t stAutoV30;
    mdrcAttr_V21_t stManualV21;
    mdrcAttr_V30_t stManualV30;
    DrcInfoV30_t Info;
} drcAttr_t;

typedef drcAttr_t drc_attrib_t;

#endif /*__ADRC_UAPI_COMPACT_H__*/
