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

#ifndef ___RK_AIQ_UAPITYPES_H__
#define ___RK_AIQ_UAPITYPES_H__

#include "adehaze_uapi_head.h"
#include "adpcc/rk_aiq_types_adpcc_ext.h"
#include "adrc_uapi_head.h"
#include "aec_uapi_head.h"
#include "agamma_uapi_head.h"
#include "amerge_uapi_head.h"
#include "atmo_uapi_head.h"
#include "awb_uapi_head.h"
#include "rk_aiq_user_api_common.h"
#include "adrc_uapi_head.h"
#include "adpcc/rk_aiq_types_adpcc_ext.h"
#include "bayer2dnr_uapi_head_v2.h"
#include "bayertnr_uapi_head_v2.h"
#include "ynr_uapi_head_v3.h"
#include "cnr_uapi_head_v2.h"
#include "sharp_uapi_head_v4.h"
#include "gain_uapi_head_v2.h"
#include "ablc_uapi_head.h"
#include "af_uapi_head.h"
#include "accm/rk_aiq_types_accm_ext.h"
#include "a3dlut/rk_aiq_types_a3dlut_ext.h"
#include "csm_head.h"
#include "agic_uapi_head.h"
#include "aie_uapi_head.h"
#include "acp_uapi_head.h"
#include "alsc_uapi_head.h"
#include "adebayer/rk_aiq_types_adebayer_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __ae_uapi {
    // M4_STRUCT_DESC("expsw_attr", "normal_ui_style")
    uapi_expsw_attr_t expsw_attr;
    // M4_STRUCT_DESC("QueryExpInfo", "normal_ui_style")
    uapi_expinfo_t expinfo;
} ae_uapi_t;

typedef struct __awb_uapi {
    // M4_STRUCT_DESC("mode", "normal_ui_style")
    uapi_wb_mode_t mode;
    // M4_STRUCT_DESC("wbgain", "normal_ui_style")
    uapi_wb_gain_t wbgain;
} awb_uapi_t;

typedef struct __amerge_uapi {
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mMergeAttrV30_t stManual;
    // M4_STRUCT_DESC("ctldata", "normal_ui_style")
    uapiMergeCurrCtlData_t ctldata;
} amerge_uapi_t;

typedef struct __atmo_uapi {
    // M4_STRUCT_DESC("ctldata", "normal_ui_style")
    uapiTmoCurrCtlData_t ctldata;
} atmo_uapi_t;

typedef struct __adrc_uapi {
    // M4_STRUCT_DESC("stManualV30", "normal_ui_style")
    mdrcAttr_V30_t stManualV30;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    DrcInfo_t info;
} adrc_uapi_t;

typedef struct __agamma_uapi {
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    Agamma_api_manualV30_t stManual;
} agamma_uapi_t;

typedef struct __adehaze_uapi {
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mDehazeAttr_t stManual;
} adehaze_uapi_t;

typedef struct __aiq_scene {
    // M4_STRING_DESC("main_scene", M4_SIZE(1,1), M4_RANGE(0, 32), "normal", M4_DYNAMIC(0))
    char* main_scene;
    // M4_STRING_DESC("sub_scene", M4_SIZE(1,1), M4_RANGE(0, 32), "day", M4_DYNAMIC(0))
    char* sub_scene;
} aiq_scene_t;

typedef struct __work_mode {
    // M4_ENUM_DESC("mode", "rk_aiq_working_mode_t", "RK_AIQ_WORKING_MODE_NORMAL");
    rk_aiq_working_mode_t mode;
} work_mode_t;

typedef struct __aiq_sysctl_desc {
    // M4_STRUCT_DESC("scene", "normal_ui_style")
    aiq_scene_t scene;
    // M4_STRUCT_DESC("work_mode", "normal_ui_style")
    work_mode_t work_mode;
} RkaiqSysCtl_t;

typedef struct __aiq_measure_info {
    // M4_STRUCT_DESC("ae_hwstats", "normal_ui_style")
    uapi_ae_hwstats_t ae_hwstats;
    // M4_STRUCT_DESC("wb_log", "normal_ui_style")
    uapi_wb_log_t wb_log;
} aiq_measure_info_t;


typedef struct __ablc_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AblcOpMode;

#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("bls0", "normal_ui_style")
    AblcSelect_t bls0;
    // M4_STRUCT_DESC("bls1", "normal_ui_style")
    AblcSelect_t bls1;
#endif
} ablc_uapi_manual_t;

typedef struct __ablc_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V30)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    AblcExpInfo_t expo_info;
#endif
} ablc_uapi_info_t;

typedef struct __ablc_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    ablc_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    ablc_uapi_info_t info;
} ablc_uapi_t;


typedef struct __abayertnr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AbayertnrOpMode;

#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual_v2", "normal_ui_style")
    RK_Bayertnr_Params_V2_Select_t manual_v2;
#endif
} abayertnr_uapi_manual_t;

typedef struct __abayertnr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V30)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayertnr_ExpInfo_V2_t expo_info;
#endif
} abayertnr_uapi_info_t;

typedef struct __abayertnr_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    abayertnr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    abayertnr_uapi_info_t info;
} abayertnr_uapi_t;

typedef struct __abayer2dnr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t Abayer2dnrOpMode;

#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual_v2", "normal_ui_style")
    RK_Bayer2dnr_Params_V2_Select_t manual_v2;
#endif

} abayer2dnr_uapi_manual_t;

typedef struct __abayer2dnr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V30)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayer2dnr_ExpInfo_V2_t expo_info;
#endif
} abayer2dnr_uapi_info_t;


typedef struct __abayer2dnr_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    abayer2dnr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    abayer2dnr_uapi_info_t info;
} abayer2dnr_uapi_t;

typedef struct __aynr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AynrOpMode;

#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual_v3", "normal_ui_style")
    RK_YNR_Params_V3_Select_t manual_v3;
#endif
} aynr_uapi_manual_t;

typedef struct __aynr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V30)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Aynr_ExpInfo_V3_t expo_info;
#endif
} aynr_uapi_info_t;

typedef struct __aynr_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    aynr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    aynr_uapi_info_t info;
} aynr_uapi_t;

typedef struct __acnr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AcnrOpMode;

#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual_v2", "normal_ui_style")
    RK_CNR_Params_V2_Select_t manual_v2;
#endif
} acnr_uapi_manual_t;

typedef struct __acnr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V30)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    AcnrV2_ExpInfo_t expo_info;
#endif
} acnr_uapi_info_t;


typedef struct __acnr_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    acnr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    acnr_uapi_info_t info;
} acnr_uapi_t;

typedef struct __asharp_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AsharpOpMode;

#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual_v4", "normal_ui_style")
    RK_SHARP_Params_V4_Select_t manual_v4;
#endif
} asharp_uapi_manual_t;

typedef struct __asharp_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V30)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Asharp4_ExpInfo_t expo_info;
#endif
} asharp_uapi_info_t;

typedef struct __asharp_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    asharp_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    asharp_uapi_info_t info;
} asharp_uapi_t;

typedef struct __again_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AgainOpMode;

#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual_v2", "normal_ui_style")
    RK_GAIN_Select_V2_t manual_v2;
#endif
} again_uapi_manual_t;

typedef struct __again_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V30)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Again_ExpInfo_V2_t expo_info;
#endif
} again_uapi_info_t;


typedef struct __again_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    again_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    again_uapi_info_t info;
} again_uapi_t;

typedef struct __custom_af_uapi {
    // M4_STRUCT_DESC("custom_af_res", "normal_ui_style")
    rk_tool_customAf_res_t custom_af_res;
} custom_af_uapi_t;
typedef struct __adebayer_uapi {
    // M4_STRUCT_DESC("adebayer_attr", "normal_ui_style")
    adebayer_attrib_t   adebayer_attr;
} adebayer_uapi_t;

typedef struct __accm_uapi_v30 {
    // M4_STRUCT_DESC("mode", "normal_ui_style")
    uapi_wb_mode_t mode;
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    rk_aiq_ccm_mccm_attrib_t manual;
} accm_uapi_v30_t;

typedef struct __a3dlut_uapi {
    // M4_STRUCT_DESC("mode", "normal_ui_style")
    uapi_wb_mode_t mode;
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    rk_aiq_lut3d_mlut3d_attrib_t manual;
} a3dlut_uapi_t;

typedef struct __agicv2_uapi {
    // M4_STRUCT_DESC("attr", "normal_ui_style")
    rkaiq_gic_v2_api_attr_t attr;
} agicv2_uapi_t;

typedef struct __aie_uapi {
    // M4_STRUCT_DESC("attr", "normal_ui_style")
    aie_attrib_t attr;
} aie_uapi_t;

typedef struct __acp_uapi {
    // M4_STRUCT_DESC("attr", "normal_ui_style")
    acp_attrib_t attr;
} acp_uapi_t;

typedef struct __alsc_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    rk_aiq_lsc_table_t manual;
} alsc_uapi_t;

typedef struct __camgroup_uapi {
    // M4_NUMBER_DESC("current_index", "u8", M4_RANGE(0, 8), "0", M4_DIGIT(0), M4_HIDE(1))
    uint8_t current_index;
} camgroup_uapi_t;

typedef struct __aiq_uapi_t {
    // M4_STRUCT_DESC("ae_uapi", "normal_ui_style")
    ae_uapi_t ae_uapi;
    // M4_STRUCT_DESC("awb_uapi", "normal_ui_style")
    awb_uapi_t awb_uapi;
    // M4_STRUCT_DESC("amerge_uapi", "normal_ui_style")
    amerge_uapi_t amerge_uapi;
#if defined(ISP_HW_V20)
    // M4_STRUCT_DESC("atmo_uapi", "normal_ui_style")
    atmo_uapi_t atmo_uapi;
#else
    // M4_STRUCT_DESC("adrc_uapi", "normal_ui_style")
    adrc_uapi_t adrc_uapi;
#endif
    // M4_STRUCT_DESC("agamma_uapi", "normal_ui_style")
    agamma_uapi_t agamma_uapi;
    // M4_STRUCT_DESC("adehaze_uapi", "normal_ui_style")
    adehaze_uapi_t adehaze_uapi;
    // M4_STRUCT_DESC("SystemCtl", "normal_ui_style")
    RkaiqSysCtl_t system;
    // M4_STRUCT_DESC("measure_info", "normal_ui_style")
    aiq_measure_info_t measure_info;
    // M4_STRUCT_DESC("adpcc_manual", "normal_ui_style")
    Adpcc_Manual_Attr_t adpcc_manual;
    // M4_STRUCT_DESC("a3dlut_uapi", "normal_ui_style")
    a3dlut_uapi_t a3dlut_uapi;
    // M4_STRUCT_DESC("csm_uapi", "normal_ui_style")
    CalibDbV2_csm_t csm_uapi;
    // M4_STRUCT_DESC("agicv2_uapi", "normal_ui_style")
    agicv2_uapi_t agicv2_uapi;
    // M4_STRUCT_DESC("aie_uapi", "normal_ui_style")
    aie_uapi_t aie_uapi;
    // M4_STRUCT_DESC("acp_uapi", "normal_ui_style")
    acp_uapi_t acp_uapi;
    // M4_STRUCT_DESC("alsc_uapi", "normal_ui_style")
    alsc_uapi_t alsc_uapi;
#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("custom_af_uapi", "normal_ui_style")
    custom_af_uapi_t custom_af_uapi;
    // M4_STRUCT_DESC("accm_uapi", "normal_ui_style")
    accm_uapi_v30_t accm_uapi;
    // M4_STRUCT_DESC("camgroup_uapi", "normal_ui_style", M4_HIDE(1))
    camgroup_uapi_t camgroup_uapi;
#endif
    // M4_STRUCT_DESC("ablc_uapi", "normal_ui_style")
    ablc_uapi_t ablc_uapi;
    // M4_STRUCT_DESC("adebayer_uapi", "normal_ui_style")
    adebayer_uapi_t   adebayer_uapi;
    // M4_STRUCT_DESC("abayertnr_uapi", "normal_ui_style")
    abayertnr_uapi_t abayertnr_uapi;
    // M4_STRUCT_DESC("abayer2dnr_uapi", "normal_ui_style")
    abayer2dnr_uapi_t abayer2dnr_uapi;
    // M4_STRUCT_DESC("aynr_uapi", "normal_ui_style")
    aynr_uapi_t aynr_uapi;
    // M4_STRUCT_DESC("acnr_uapi", "normal_ui_style")
    acnr_uapi_t acnr_uapi;
    // M4_STRUCT_DESC("asharp_uapi", "normal_ui_style")
    asharp_uapi_t asharp_uapi;
    // M4_STRUCT_DESC("again_uapi", "normal_ui_style")
    again_uapi_t again_uapi;

} RkaiqUapi_t;

#ifdef __cplusplus
}
#endif

#endif  /*___RK_AIQ_UAPITYPES_H__*/
