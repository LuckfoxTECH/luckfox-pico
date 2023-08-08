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
#include "adrc_uapi_head.h"
#include "aec_uapi_head.h"
#include "agamma_uapi_head.h"
#include "amerge_uapi_head.h"
#include "atmo_uapi_head.h"
#include "awb_uapi_head.h"
#include "rk_aiq_user_api_common.h"
#if defined(ISP_HW_V32)
#include "bayertnr_uapi_head_v23.h"
#include "bayer2dnr_uapi_head_v23.h"
#include "ynr_uapi_head_v22.h"
#include "cnr_uapi_head_v30.h"
#include "sharp_uapi_head_v33.h"
#endif
#if defined(ISP_HW_V32) || defined(ISP_HW_V30)
#include "gain_uapi_head_v2.h"
#endif
#include "ccm_uapi_head.h"
#include "csm_head.h"
#include "cgc_head.h"

#if defined(ISP_HW_V32)
#include "bayer2dnr_uapi_head_v23.h"
#include "bayertnr_uapi_head_v23.h"
#include "ynr_uapi_head_v22.h"
#include "cnr_uapi_head_v30.h"
#include "sharp_uapi_head_v33.h"
#include "gain_uapi_head_v2.h"
#include "ablc_uapi_head_v32.h"
#endif

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
    // M4_STRUCT_DESC("Info", "normal_ui_style")
    uapiMergeCurrCtlData_t Info;
#if defined(ISP_HW_V21)
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mMergeAttrV10_t stManual;
#endif
#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mMergeAttrV11_t stManual;
#endif
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mMergeAttrV12_t stManual;
#endif
} amerge_uapi_t;

typedef struct __atmo_uapi {
    // M4_STRUCT_DESC("ctldata", "normal_ui_style")
    uapiTmoCurrCtlData_t ctldata;
} atmo_uapi_t;

typedef struct __adrc_uapi {
#if defined(ISP_HW_V21)
    // M4_STRUCT_DESC("Info", "normal_ui_style")
    DrcInfoV10_t Info;
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mdrcAttr_V10_t stManual;
#endif
#if defined(ISP_HW_V30)
    // M4_STRUCT_DESC("Info", "normal_ui_style")
    DrcInfoV11_t Info;
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mdrcAttr_V11_t stManual;
#endif
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("Info", "normal_ui_style")
    DrcInfoV12_t Info;
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mdrcAttr_V12_t stManual;
#endif
} adrc_uapi_t;

typedef struct __agamma_uapi {
#if defined(ISP_HW_V20) || defined(ISP_HW_V21)
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    AgammaApiManualV10_t stManual;
#else
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    AgammaApiManualV11_t stManual;
#endif
} agamma_uapi_t;

typedef struct __adehaze_uapi {
    // M4_STRUCT_DESC("Info", "normal_ui_style")
    mDehazeAttrInfoV11_t Info;
#if defined(ISP_HW_V21) || defined(ISP_HW_V30)
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mDehazeAttrV11_t stManual;
#endif
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("stManual", "normal_ui_style")
    mDehazeAttrV12_t stManual;
#endif
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

#if ISP_HW_V21
typedef struct __aiq_measure_info {
    // M4_STRUCT_DESC("ae_hwstats", "normal_ui_style")
    uapi_ae_hwstats_t ae_hwstats;
    // M4_STRUCT_DESC("wb_log", "normal_ui_style")
    uapi_wbV21_log_t wb_log;
} aiq_measure_info_t;
#elif ISP_HW_V30
typedef struct __aiq_measure_info {
    // M4_STRUCT_DESC("ae_hwstats", "normal_ui_style")
    uapi_ae_hwstats_t ae_hwstats;
    // M4_STRUCT_DESC("wb_log", "normal_ui_style")
    uapi_wbV30_log_t wb_log;
} aiq_measure_info_t;

#elif ISP_HW_V32
typedef struct __aiq_measure_info {
    // M4_STRUCT_DESC("ae_hwstats", "normal_ui_style")
    uapi_ae_hwstats_t ae_hwstats;
    // M4_STRUCT_DESC("wb_log", "normal_ui_style")
    uapi_wbV32_log_t wb_log;
} aiq_measure_info_t;
#endif

typedef struct __ablc_uapi_manual {
    // M4_ENUM_DESC("OPMOde", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AblcOPMode;

#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("blc0_para", "normal_ui_style")
    AblcSelect_V32_t blc0_para;

    // M4_STRUCT_DESC("blc1_para", "normal_ui_style")
    AblcSelect_V32_t blc1_para;

    // M4_STRUCT_DESC("blc_ob_para", "normal_ui_style")
    AblcOBSelect_V32_t blc_ob_para;
#endif
} ablc_uapi_manual_t;

typedef struct __alb_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V32)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    AblcExpInfo_V32_t expo_info;
#endif
} ablc_uapi_info_t;

typedef struct __ablc_uapi {
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    ablc_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    ablc_uapi_info_t info;
#endif
} ablc_uapi_t;

typedef struct __abayertnr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AbayertnrOpMode;

#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual_v23", "normal_ui_style")
    RK_Bayertnr_Params_V23_Select_t manual_v23;
#endif
} abayertnr_uapi_manual_t;

typedef struct __abayertnr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V32)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayertnr_ExpInfo_V23_t expo_info;
#endif
} abayertnr_uapi_info_t;

typedef struct __abayertnr_uapi {
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    abayertnr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    abayertnr_uapi_info_t info;
#endif
} abayertnr_uapi_t;

typedef struct __abayer2dnr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t Abayer2dnrOpMode;

#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual_v23", "normal_ui_style")
    RK_Bayer2dnrV23_Params_Select_t manual_v23;
#endif
} abayer2dnr_uapi_manual_t;

typedef struct __abayer2dnr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V32)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Abayer2dnr_ExpInfo_V23_t expo_info;
#endif
} abayer2dnr_uapi_info_t;


typedef struct __abayer2dnr_uapi {
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    abayer2dnr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    abayer2dnr_uapi_info_t info;

#endif
} abayer2dnr_uapi_t;

typedef struct __aynr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AynrOpMode;

#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual_v22", "normal_ui_style")
    RK_YNR_Params_V22_Select_t manual_v22;
#endif
} aynr_uapi_manual_t;

typedef struct __aynr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V32)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Aynr_ExpInfo_V22_t expo_info;
#endif
} aynr_uapi_info_t;

typedef struct __aynr_uapi {
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    aynr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    aynr_uapi_info_t info;

#endif
} aynr_uapi_t;

typedef struct __acnr_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AcnrOpMode;

#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual_v30", "normal_ui_style")
    RK_CNR_Params_V30_Select_t manual_v30;
#endif
} acnr_uapi_manual_t;

typedef struct __acnr_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V32)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    AcnrV30_ExpInfo_t expo_info;
#endif
} acnr_uapi_info_t;


typedef struct __acnr_uapi {
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    acnr_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    acnr_uapi_info_t info;

#endif
} acnr_uapi_t;

typedef struct __asharp_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AsharpOpMode;

#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual_v33", "normal_ui_style")
    RK_SHARP_Params_V33_Select_t manual_v33;
#endif
} asharp_uapi_manual_t;

typedef struct __asharp_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V32)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Asharp_ExpInfo_V33_t expo_info;
#endif
} asharp_uapi_info_t;


typedef struct __asharp_uapi {
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    asharp_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    asharp_uapi_info_t info;
#endif
} asharp_uapi_t;

typedef struct __again_uapi_manual {
    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_uapi_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t AgainOpMode;

#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual_v2", "normal_ui_style")
    RK_GAIN_Select_V2_t manual_v2;
#endif
} again_uapi_manual_t;

typedef struct __again_uapi_info {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0, 204800), "50", M4_DIGIT(0), "0", "0")
    int iso;
#if defined(ISP_HW_V32)
    // M4_ARRAY_TABLE_DESC("expo_info", "normal_ui_style", "none", "0", "0")
    Again_ExpInfo_V2_t expo_info;
#endif
} again_uapi_info_t;


typedef struct __again_uapi {
#if defined(ISP_HW_V32) || defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    again_uapi_manual_t manual;
    // M4_STRUCT_DESC("info", "normal_ui_style")
    again_uapi_info_t info;
#endif
} again_uapi_t;

typedef struct __accm_uapi {
    // M4_STRUCT_DESC("Info", "normal_ui_style", "0", "1")
    rk_aiq_ccm_querry_info_t Info;
#if defined(ISP_HW_V20) || defined(ISP_HW_V21) || defined(ISP_HW_V30)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    rk_aiq_ccm_mccm_attrib_t stManual;
#endif
#if defined(ISP_HW_V32)
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    rk_aiq_ccm_mccm_attrib_v2_t stManual;
#endif
} accm_uapi_t;

typedef struct __acsm_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    Csm_Param_t manual;
} acsm_uapi_t;

typedef struct __acgc_uapi {
    // M4_STRUCT_DESC("manual", "normal_ui_style")
    Cgc_Param_t manual;
} acgc_uapi_t;

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

    // M4_STRUCT_DESC("ablc_uapi_t", "normal_ui_style")
    ablc_uapi_t ablc_uapi;
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
    // M4_STRUCT_DESC("measure_info", "normal_ui_style",M4_HIDE(1))
    aiq_measure_info_t measure_info;
    // M4_STRUCT_DESC("accm_uapi", "normal_ui_style")
    accm_uapi_t accm_uapi;
    // M4_STRUCT_DESC("acsm_uapi", "normal_ui_style")
    acsm_uapi_t acsm_uapi;
#ifndef ISP_HW_V20
    // M4_STRUCT_DESC("acgc_uapi", "normal_ui_style")
    acgc_uapi_t acgc_uapi;
#endif
} RkaiqUapi_t;

#ifdef __cplusplus
}
#endif

#endif  /*___RK_AIQ_UAPITYPES_H__*/
