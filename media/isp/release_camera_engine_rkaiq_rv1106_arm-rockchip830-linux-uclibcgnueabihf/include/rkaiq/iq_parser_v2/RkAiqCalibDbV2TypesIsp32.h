/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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

#ifndef ___RK_AIQ_CALIB_DB_V2_TYPES_ISP32_H__
#define ___RK_AIQ_CALIB_DB_V2_TYPES_ISP32_H__

#include "ablc_head_V32.h"
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
#include "bayer2dnr_head_v23.h"
#include "bayertnr_head_v23.h"
#include "cac_head.h"
#include "ccm_head.h"
#include "cgc_head.h"
#include "cnr_head_v30.h"
#include "cproc_head.h"
#include "cpsl_head.h"
#include "csm_head.h"
#include "debayer_head.h"
#include "fec_head.h"
#include "gain_head_v2.h"
#include "ldch_head.h"
#include "lut3d_head.h"
#include "sharp_head_v33.h"
#include "thumbnails_head.h"
#include "ynr_head_v22.h"

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

typedef struct CamCalibDbV2ContextIsp32_s {
    // M4_STRUCT_DESC("AEC", "normal_ui_style")
    CalibDb_Aec_ParaV2_t ae_calib;
    // M4_STRUCT_DESC("wb_v32", "normal_ui_style")
    CalibDbV2_Wb_Para_V32_t wb_v32;
    // M4_STRUCT_DESC("ablcV32_calib", "normal_ui_style")
    CalibDbV2_Blc_V32_t ablcV32_calib;
    // M4_STRUCT_DESC("ccm_calib", "normal_ui_style")
    CalibDbV2_Ccm_Para_V32_t ccm_calib_v2;
    // M4_STRUCT_DESC("lut3d_calib", "normal_ui_style")
    CalibDbV2_Lut3D_Para_V2_t lut3d_calib;
    // M4_STRUCT_DESC("degamma", "curve_ui_type_A")
    CalibDbV2_Adegmma_t adegamma_calib;
    // M4_STRUCT_DESC("agic_calib_v21", "normal_ui_style")
    CalibDbV2_Gic_V21_t agic_calib_v21;
    // M4_STRUCT_DESC("debayer_calib", "normal_ui_style")
    CalibDbV2_Debayer_v2_t debayer_v2;
    // M4_STRUCT_DESC("Amerge_calib", "normal_ui_style")
    CalibDbV2_merge_v12_t amerge_calib_v12;
    // M4_STRUCT_DESC("Adrc_calib", "normal_ui_style")
    CalibDbV2_drc_V12_t adrc_calib_v12;
    // M4_STRUCT_DESC("agamma_calib", "normal_ui_style")
    CalibDbV2_gamma_v11_t agamma_calib_v11;
    // M4_STRUCT_DESC("adehaze_calib", "normal_ui_style")
    CalibDbV2_dehaze_v12_t adehaze_calib_v12;
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
    // M4_STRUCT_DESC("bayer2dnr_v23", "normal_ui_style")
    CalibDbV2_Bayer2dnrV23_t bayer2dnr_v23;
    // M4_STRUCT_DESC("bayertnr_v23", "normal_ui_style")
    CalibDbV2_BayerTnrV23_t bayertnr_v23;
    // M4_STRUCT_DESC("ynr_v22", "normal_ui_style")
    CalibDbV2_YnrV22_t ynr_v22;
    // M4_STRUCT_DESC("cnr_v30", "normal_ui_style")
    CalibDbV2_CNRV30_t cnr_v30;
    // M4_STRUCT_DESC("sharp_v33", "normal_ui_style")
    CalibDbV2_SharpV33_t sharp_v33;
    // M4_STRUCT_DESC("cac_v11", "normal_ui_style")
    CalibDbV2_Cac_V11_t cac_v11;
    // M4_STRUCT_DESC("af_calib", "normal_ui_style")
    CalibDbV2_AFV31_t af_v31;
    // M4_STRUCT_DESC("gain_v2", "normal_ui_style")
    CalibDbV2_GainV2_t gain_v2;
    // M4_STRUCT_DESC("csm", "normal_ui_style")
    CalibDbV2_csm_t csm;
    // M4_STRUCT_DESC("cgc", "normal_ui_style")
    CalibDbV2_cgc_t cgc;
} CamCalibDbV2ContextIsp32_t;

RKAIQ_END_DECLARE

#endif
