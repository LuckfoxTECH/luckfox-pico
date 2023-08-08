/*
 * RkAiqCalibDbV2TypesIsp21.h
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

#ifndef ___RK_AIQ_CALIB_DB_V2_TYPES_ISP30_H__
#define ___RK_AIQ_CALIB_DB_V2_TYPES_ISP30_H__

#include "RkAiqCalibDbV2TypesIsp21.h"
#include "ynr_head_v3.h"
#include "cnr_head_v2.h"
#include "sharp_head_v4.h"
#include "bayer2dnr_head_v2.h"
#include "bayertnr_head_v2.h"
#include "gain_head_v2.h"
#include "cgc_head.h"
#include "csm_head.h"
#include "cac_head.h"

RKAIQ_BEGIN_DECLARE

typedef struct CamCalibDbV2ContextIsp30_s {
    // M4_STRUCT_DESC("AEC", "normal_ui_style")
    CalibDb_Aec_ParaV2_t ae_calib;
    // M4_STRUCT_DESC("wb_v21", "normal_ui_style")
    CalibDbV2_Wb_Para_V21_t wb_v21;
    // M4_STRUCT_DESC("ablc_calib", "normal_ui_style")
    CalibDbV2_Ablc_t ablc_calib;
    // M4_STRUCT_DESC("ccm_calib", "normal_ui_style")
    CalibDbV2_Ccm_Para_V2_t ccm_calib;
    // M4_STRUCT_DESC("lut3d_calib", "normal_ui_style")
    CalibDbV2_Lut3D_Para_V2_t lut3d_calib;
    // M4_STRUCT_DESC("degamma", "curve_ui_type_A")
    CalibDbV2_Adegmma_t adegamma_calib;
    // M4_STRUCT_DESC("agic_calib_v21", "normal_ui_style")
    CalibDbV2_Gic_V21_t agic_calib_v21;
    // M4_STRUCT_DESC("debayer_calib", "normal_ui_style")
    CalibDbV2_Debayer_t debayer;
    // M4_STRUCT_DESC("Amerge_calib", "normal_ui_style")
    CalibDbV2_merge_V2_t amerge_calib_V2;
    // M4_STRUCT_DESC("Adrc_calib", "normal_ui_style")
    CalibDbV2_drc_V2_t adrc_calib_V2;
    // M4_STRUCT_DESC("agamma_calib", "normal_ui_style")
    CalibDbV2_gamma_V30_t agamma_calib_V30;
    // M4_STRUCT_DESC("adehaze_calib", "normal_ui_style")
    CalibDbV2_dehaze_V21_t adehaze_calib_v30;
    // M4_STRUCT_DESC("adpcc_calib", "normal_ui_style")
    CalibDbV2_Dpcc_t adpcc_calib;
    // M4_STRUCT_DESC("aldch", "normal_ui_style")
    CalibDbV2_LDCH_t aldch;
    // M4_STRUCT_DESC("cpsl", "normal_ui_style")
    CalibDbV2_Cpsl_t cpsl;
    // M4_STRUCT_DESC("cproc_calib", "normal_ui_style")
    CalibDbV2_Cproc_t cproc;
    // M4_STRUCT_DESC("aie_calib", "normal_ui_style")
    CalibDbV2_IE_t ie;
    // M4_STRUCT_DESC("colorAsGrey", "normal_ui_style")
    CalibDbV2_ColorAsGrey_t colorAsGrey;
    // M4_STRUCT_DESC("lsc_v2", "normal_ui_style")
    CalibDbV2_LSC_t lsc_v2;
    // M4_STRUCT_DESC("bayer2dnr_v2", "normal_ui_style")
    CalibDbV2_Bayer2dnr_V2_t bayer2dnr_v2;
    // M4_STRUCT_DESC("bayertnr_v2", "normal_ui_style")
    CalibDbV2_BayerTnr_V2_t bayertnr_v2;
    // M4_STRUCT_DESC("ynr_v3", "normal_ui_style")
    CalibDbV2_YnrV3_t ynr_v3;
    // M4_STRUCT_DESC("cnr_v2", "normal_ui_style")
    CalibDbV2_CNRV2_t cnr_v2;
    // M4_STRUCT_DESC("sharp_v4", "normal_ui_style")
    CalibDbV2_SharpV4_t sharp_v4;
    // M4_STRUCT_DESC("cac_calib", "normal_ui_style")
    CalibDbV2_Cac_t cac_calib;
    // M4_STRUCT_DESC("af_calib", "normal_ui_style")
    CalibDbV2_AFV30_t af_v30;
    // M4_STRUCT_DESC("gain_v2", "normal_ui_style")
    CalibDbV2_GainV2_t gain_v2;
    // M4_STRUCT_DESC("csm", "normal_ui_style")
    CalibDbV2_csm_t csm;
    // M4_STRUCT_DESC("cgc", "normal_ui_style")
    CalibDbV2_cgc_t cgc;
} CamCalibDbV2ContextIsp30_t;

RKAIQ_END_DECLARE

#endif
