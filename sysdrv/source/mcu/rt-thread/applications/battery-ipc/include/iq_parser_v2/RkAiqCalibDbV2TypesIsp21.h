/*
 * Copyright (c) 2021-2022 Rockchip Eletronics Co., Ltd.
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
 */
#ifndef ___RK_AIQ_CALIB_DB_V2_TYPES_ISP21_H__
#define ___RK_AIQ_CALIB_DB_V2_TYPES_ISP21_H__

#include "ablc_head.h"
#include "adegamma_head.h"
#include "adehaze_head.h"
#include "adpcc_head.h"
#include "adrc_head.h"
#include "aec_head.h"
#include "af_head.h"
#include "agamma_head.h"
#include "agic_head.h"
#include "aie_head.h"
#include "alsc_head.h"
#include "amerge_head.h"
#include "atmo_head.h"
#include "awb_head.h"
#include "bayernr_head_v2.h"
#include "ccm_head.h"
#include "cgc_head.h"
#include "cnr_head_v1.h"
#include "cproc_head.h"
#include "cpsl_head.h"
#include "csm_head.h"
#include "debayer_head.h"
#include "fec_head.h"
#include "ldch_head.h"
#include "lut3d_head.h"
#include "sharp_head_v3.h"
#include "thumbnails_head.h"
#include "ynr_head_v2.h"

RKAIQ_BEGIN_DECLARE

typedef struct CalibDbV2_ColorAsGrey_Param_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
    // M4_NUMBER_DESC("skip_frame", "s8", M4_RANGE(0,255), "10", M4_DIGIT(0))
    int8_t skip_frame;
} CalibDbV2_ColorAsGrey_Param_t;

typedef struct CalibDbV2_ColorAsGrey_s {
    // M4_STRUCT_DESC("param", "normal_ui_style")
    CalibDbV2_ColorAsGrey_Param_t param;
} CalibDbV2_ColorAsGrey_t;

typedef struct CalibDbV2_LUMA_DETECT_s {
    unsigned char luma_detect_en;
    int fixed_times;
    float mutation_threshold;
    float mutation_threshold_level2;
} CalibDbV2_LUMA_DETECT_t;

typedef struct CamCalibDbV2ContextIsp21_s {
    // M4_STRUCT_DESC("AEC", "normal_ui_style")
    CalibDb_Aec_ParaV2_t ae_calib;
    // M4_STRUCT_DESC("wb_v21", "normal_ui_style")
    CalibDbV2_Wb_Para_V21_t wb_v21;
    // M4_STRUCT_DESC("agamma_calib", "normal_ui_style")
    CalibDbV2_gamma_v10_t agamma_calib_v10;
    // M4_STRUCT_DESC("ablc_calib", "normal_ui_style")
    CalibDbV2_Ablc_t ablc_calib;
    // M4_STRUCT_DESC("degamma", "curve_ui_type_A")
    CalibDbV2_Adegmma_t adegamma_calib;
    // M4_STRUCT_DESC("agic_calib_v21", "normal_ui_style")
    CalibDbV2_Gic_V21_t agic_calib_v21;
    // M4_STRUCT_DESC("adehaze_calib_v11", "normal_ui_style")
    CalibDbV2_dehaze_v11_t adehaze_calib_v11;
    // M4_STRUCT_DESC("adpcc_calib", "normal_ui_style")
    CalibDbV2_Dpcc_t adpcc_calib;
    // M4_STRUCT_DESC("amerge_calib", "normal_ui_style")
    CalibDbV2_merge_v10_t amerge_calib_v10;
    // M4_STRUCT_DESC("adrc_calib", "normal_ui_style")
    CalibDbV2_drc_V10_t adrc_calib_v10;
    // M4_STRUCT_DESC("cpsl", "normal_ui_style")
    CalibDbV2_Cpsl_t cpsl;
    // M4_STRUCT_DESC("debayer_calib", "normal_ui_style")
    CalibDbV2_Debayer_t debayer;
    // M4_STRUCT_DESC("cproc_calib", "normal_ui_style")
    CalibDbV2_Cproc_t cproc;
    // M4_STRUCT_DESC("aie_calib", "normal_ui_style")
    CalibDbV2_IE_t ie;
    // M4_STRUCT_DESC("lsc_v2", "normal_ui_style")
    CalibDbV2_LSC_t lsc_v2;
    // M4_STRUCT_DESC("colorAsGrey", "normal_ui_style")
    CalibDbV2_ColorAsGrey_t colorAsGrey;
    CalibDbV2_LUMA_DETECT_t lumaDetect;
    // M4_STRUCT_DESC("aldch", "normal_ui_style")
    CalibDbV2_LDCH_t aldch;
    // M4_STRUCT_DESC("ccm_calib", "normal_ui_style")
    CalibDbV2_Ccm_Para_V2_t ccm_calib;
    // M4_STRUCT_DESC("lut3d_calib", "normal_ui_style")
    CalibDbV2_Lut3D_Para_V2_t lut3d_calib;
    // M4_STRUCT_DESC("af_calib", "normal_ui_style")
    CalibDbV2_AF_t af;
    // M4_STRUCT_DESC("thumbnails", "normal_ui_style")
    CalibDbV2_Thumbnails_t thumbnails;
    // M4_STRUCT_DESC("bayernr_v2", "normal_ui_style")
    CalibDbV2_BayerNrV2_t bayernr_v2;
    // M4_STRUCT_DESC("cnr_v1", "normal_ui_style")
    CalibDbV2_CNR_t cnr_v1;
    // M4_STRUCT_DESC("ynr_v2", "normal_ui_style")
    CalibDbV2_YnrV2_t ynr_v2;
    // M4_STRUCT_DESC("sharp_v3", "normal_ui_style")
    CalibDbV2_SharpV3_t sharp_v3;
    // M4_STRUCT_DESC("csm", "normal_ui_style")
    CalibDbV2_csm_t csm;
    // M4_STRUCT_DESC("cgc", "normal_ui_style")
    CalibDbV2_cgc_t cgc;
} CamCalibDbV2ContextIsp21_t;

RKAIQ_END_DECLARE

#endif
