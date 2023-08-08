/*
 *rk_aiq_types_alsc_algo_int.h
 *
 *  Copyright (c) 2019 Rockchip Corporation
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

#ifndef _RK_AIQ_TYPE_ASHARP_ALGO_INT_H_
#define _RK_AIQ_TYPE_ASHARP_ALGO_INT_H_
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "asharp/rk_aiq_types_asharp_algo.h"
#include "sharp_head_v1.h"
#include "edgefilter_head_v1.h"

RKAIQ_BEGIN_DECLARE

#define SHARP_MAX_STRENGTH_PERCENT (100.0)

#define MAX_ISO_STEP                CALIBDB_NR_SHARP_MAX_ISO_LEVEL
//sharp
#define IDX_SIGN                            0
#define IDX_INTE                            1
#define IDX_DECI                            2
#define IDX_NUM                             3

#define RK_EDGEFILTER_LUMA_POINT_NUM        8
#define RK_SHARPEN_HW_PARAMS_NUM            28
#define RK_SHARPFILTER_LUMA_POINT_NUM       8
#define RK_SHARPEN_HW_SHARP_PAR_NUM         (23)
#define ISP_PP_SHARP_BLK_WIDTH              32
#define ISP_PP_SHARP_BLK_HEIGHT             128

#define RKSHAPRENHW_GAU_DIAM                            3
#define RKSHAPRENHW_PBF_DIAM                            3
#define RKSHAPRENHW_MRF_DIAM                            5
#define RKSHAPRENHW_MBF_DIAM_X                          17
#define RKSHAPRENHW_MBF_DIAM_Y                          13
#define RKSHAPRENHW_HRF_DIAM                            5
#define RKSHAPRENHW_HBF_DIAM                            3
#define RKSHAPRENHW_AVG_DIAM                            3

#define RKSHAPRENHW_V2_GAU_DIAM                         3
#define RKSHAPRENHW_V2_PBF_DIAM                         3
#define RKSHAPRENHW_V2_HRF_DIAM                         5
#define RKSHAPRENHW_V2_HBF_DIAM_X                       9
#define RKSHAPRENHW_V2_HBF_DIAM_Y                       9
#define RKSHAPRENHW_V2_AVG_DIAM                         3

#define RKSHAPRENHW_V3_RF_DIAM                          5
#define RKSHAPRENHW_V3_AVG_DIAM                         5


//edgefilter
#define RK_EDGEFILTER_LUMA_POINT_NUM                    8
#define RK_EDGEFILTER_PARAMS_NUM                        15//21
#define RKEDGEFILTER_LUMA_GAU_DIAM                      3
#define RKEDGEFILTER_DIR_SMTH_DIAM                      5
#define RKEDGEFILTER_GAUS_DIAM                          5
#define RKEDGEFILTER_SHRP_DIAM                          3
#define RKEDGEFILTER_DOG_DIAM                           5


typedef struct RKAsharp_Sharp_HW_Params_s
{
#ifndef RK_SIMULATOR_HW
    float iso[MAX_ISO_STEP];
#endif
    float lratio        [MAX_ISO_STEP];
    float hratio        [MAX_ISO_STEP];
    float M_ratio       [MAX_ISO_STEP];
    float H_ratio       [MAX_ISO_STEP];
    short luma_point    [RK_SHARPFILTER_LUMA_POINT_NUM];
    float luma_sigma    [MAX_ISO_STEP][RK_SHARPFILTER_LUMA_POINT_NUM];
    float pbf_gain      [MAX_ISO_STEP];
    float pbf_add       [MAX_ISO_STEP];
    float pbf_ratio     [MAX_ISO_STEP];
    short lum_clp_m     [MAX_ISO_STEP][RK_SHARPFILTER_LUMA_POINT_NUM];
    float lum_min_m     [MAX_ISO_STEP][RK_SHARPFILTER_LUMA_POINT_NUM];
    float mbf_gain      [MAX_ISO_STEP];
    float mbf_add       [MAX_ISO_STEP];
    short lum_clp_h     [MAX_ISO_STEP][RK_SHARPFILTER_LUMA_POINT_NUM];
    float hbf_ratio     [MAX_ISO_STEP];
    float hbf_gain      [MAX_ISO_STEP];
    float hbf_add       [MAX_ISO_STEP];
    short ehf_th        [MAX_ISO_STEP];
    float pbf_coeff_percent[MAX_ISO_STEP];
    float rf_m_coeff_percent[MAX_ISO_STEP];
    float rf_h_coeff_percent[MAX_ISO_STEP];
    float hbf_coeff_percent[MAX_ISO_STEP];

    
    float gaus_luma_kernel  [MAX_ISO_STEP][RKSHAPRENHW_PBF_DIAM * RKSHAPRENHW_PBF_DIAM];
    float kernel_pbf_l    [MAX_ISO_STEP][RKSHAPRENHW_PBF_DIAM * RKSHAPRENHW_PBF_DIAM];
    float kernel_pbf_h    [MAX_ISO_STEP][RKSHAPRENHW_PBF_DIAM * RKSHAPRENHW_PBF_DIAM];
    float h_rf_m_l        [MAX_ISO_STEP][RKSHAPRENHW_MRF_DIAM * RKSHAPRENHW_MRF_DIAM];
    float h_rf_m_h        [MAX_ISO_STEP][RKSHAPRENHW_MRF_DIAM * RKSHAPRENHW_MRF_DIAM];
    float kernel_mbf    [MAX_ISO_STEP][RKSHAPRENHW_MBF_DIAM_Y * RKSHAPRENHW_MBF_DIAM_X];
    float h_rf_h_l        [MAX_ISO_STEP][RKSHAPRENHW_HRF_DIAM * RKSHAPRENHW_HRF_DIAM];
    float h_rf_h_h        [MAX_ISO_STEP][RKSHAPRENHW_HRF_DIAM * RKSHAPRENHW_HRF_DIAM];
    float kernel_hbf_l    [MAX_ISO_STEP][RKSHAPRENHW_HBF_DIAM * RKSHAPRENHW_HBF_DIAM];
    float kernel_hbf_h    [MAX_ISO_STEP][RKSHAPRENHW_HBF_DIAM * RKSHAPRENHW_HBF_DIAM];

} RKAsharp_Sharp_HW_Params_t;


typedef struct RKAsharp_Sharp_HW_Params_Select_s
{
    float lratio        ;
    float hratio        ;
    float M_ratio       ;
    float H_ratio       ;
    short luma_point    [RK_SHARPFILTER_LUMA_POINT_NUM];
    float luma_sigma    [RK_SHARPFILTER_LUMA_POINT_NUM];
    float pbf_gain      ;
    float pbf_add       ;
    float pbf_ratio;
    short lum_clp_m     [RK_SHARPFILTER_LUMA_POINT_NUM];
    float lum_min_m     [RK_SHARPFILTER_LUMA_POINT_NUM];
    float mbf_gain      ;
    float mbf_add       ;
    short lum_clp_h     [RK_SHARPFILTER_LUMA_POINT_NUM];
    float hbf_ratio     ;
    float hbf_gain      ;
    float hbf_add       ;
    short ehf_th        ;
    float gaus_luma_kernel  [RKSHAPRENHW_GAU_DIAM * RKSHAPRENHW_GAU_DIAM];
    float kernel_pbf    [RKSHAPRENHW_PBF_DIAM * RKSHAPRENHW_PBF_DIAM];
    float h_rf_m        [RKSHAPRENHW_MRF_DIAM * RKSHAPRENHW_MRF_DIAM];
    float kernel_mbf    [RKSHAPRENHW_MBF_DIAM_Y * RKSHAPRENHW_MBF_DIAM_X];
    float h_rf_h        [RKSHAPRENHW_HRF_DIAM * RKSHAPRENHW_HRF_DIAM];
    float kernel_hbf    [RKSHAPRENHW_HBF_DIAM * RKSHAPRENHW_HBF_DIAM];



} RKAsharp_Sharp_HW_Params_Select_t;

typedef struct RKAsharp_Sharp_HW_V2_Params_s
{
    short luma_point        [RK_SHARPFILTER_LUMA_POINT_NUM];
    short luma_sigma        [MAX_ISO_STEP][RK_SHARPFILTER_LUMA_POINT_NUM];
    float pbf_gain          [MAX_ISO_STEP];
    float pbf_add           [MAX_ISO_STEP];
    float pbf_ratio         [MAX_ISO_STEP];
    float lratio            [MAX_ISO_STEP];
    float hratio            [MAX_ISO_STEP];
    float sharp_ratio       [MAX_ISO_STEP];
    short lum_clip_h        [MAX_ISO_STEP][RK_SHARPFILTER_LUMA_POINT_NUM];
    float hbf_gain          [MAX_ISO_STEP];
    float hbf_add           [MAX_ISO_STEP];
    float hbf_ratio         [MAX_ISO_STEP];
    short ehf_th            [MAX_ISO_STEP];

    short gaus_luma_kernel      [MAX_ISO_STEP][RKSHAPRENHW_V2_PBF_DIAM * RKSHAPRENHW_V2_PBF_DIAM];
    short kernel_pre_bf         [MAX_ISO_STEP][RKSHAPRENHW_V2_PBF_DIAM * RKSHAPRENHW_V2_PBF_DIAM];
    short kernel_range_filter   [MAX_ISO_STEP][RKSHAPRENHW_V2_HRF_DIAM * RKSHAPRENHW_V2_HRF_DIAM];
    short kernel_hf_bf          [MAX_ISO_STEP][RKSHAPRENHW_V2_HBF_DIAM_Y * RKSHAPRENHW_V2_HBF_DIAM_X];

} RKAsharp_Sharp_HW_V2_Params_t;

typedef struct RKAsharp_Sharp_HW_V2_Params_Select_s
{
    short luma_point    [RK_SHARPFILTER_LUMA_POINT_NUM];
    short luma_sigma    [RK_SHARPFILTER_LUMA_POINT_NUM];
    float pbf_gain      ;
    float pbf_add       ;
    float pbf_ratio     ;
    float lratio        ;
    float hratio        ;
    float sharp_ratio   ;
    short lum_clip_h    [RK_SHARPFILTER_LUMA_POINT_NUM];
    float hbf_gain      ;
    float hbf_add       ;
    float hbf_ratio     ;
    short ehf_th        ;

    short gaus_luma_kernel      [RKSHAPRENHW_V2_PBF_DIAM * RKSHAPRENHW_V2_PBF_DIAM];
    short kernel_pre_bf         [RKSHAPRENHW_V2_PBF_DIAM * RKSHAPRENHW_V2_PBF_DIAM];
    short kernel_range_filter   [RKSHAPRENHW_V2_HRF_DIAM * RKSHAPRENHW_V2_HRF_DIAM];
    short kernel_hf_bf          [RKSHAPRENHW_V2_HBF_DIAM_Y * RKSHAPRENHW_V2_HBF_DIAM_X];

} RKAsharp_Sharp_HW_V2_Params_Select_t;

typedef struct RKAsharp_Sharp_HW_V3_Params_s
{
    short luma_point        [RK_SHARPFILTER_LUMA_POINT_NUM];
    float lratio            [MAX_ISO_STEP];
    float hratio            [MAX_ISO_STEP];
    float sharp_ratio       [MAX_ISO_STEP];
    short lum_clip_h        [MAX_ISO_STEP][RK_SHARPFILTER_LUMA_POINT_NUM];
    short ehf_th            [MAX_ISO_STEP];

    short kernel_range_filter   [MAX_ISO_STEP][RKSHAPRENHW_V3_RF_DIAM * RKSHAPRENHW_V3_RF_DIAM];
} RKAsharp_Sharp_HW_V3_Params_t;

typedef struct RKAsharp_Sharp_HW_V3_Params_Select_s
{
    short luma_point    [RK_SHARPFILTER_LUMA_POINT_NUM];
    float lratio        ;
    float hratio        ;
    float sharp_ratio   ;
    short lum_clip_h    [RK_SHARPFILTER_LUMA_POINT_NUM];
    short ehf_th        ;

    short kernel_range_filter   [RKSHAPRENHW_V3_RF_DIAM * RKSHAPRENHW_V3_RF_DIAM];
} RKAsharp_Sharp_HW_V3_Params_Select_t;


typedef struct RKAsharp_Sharp_Params_s
{
    RKAsharp_Sharp_HW_Params_t          rk_sharpen_params_V1;
    RKAsharp_Sharp_HW_V2_Params_t       rk_sharpen_params_V2;
    RKAsharp_Sharp_HW_V3_Params_t       rk_sharpen_params_V3;

} RKAsharp_Sharp_Params_t;

typedef struct RKAsharp_Sharp_Params_Select_s
{
    RKAsharp_Sharp_HW_Params_Select_t           rk_sharpen_params_selected_V1;
    RKAsharp_Sharp_HW_V2_Params_Select_t        rk_sharpen_params_selected_V2;
    RKAsharp_Sharp_HW_V3_Params_Select_t        rk_sharpen_params_selected_V3;

} RKAsharp_Sharp_Params_Select_t;


typedef struct RKAsharp_EdgeFilter_Params_s
{
#ifndef RK_SIMULATOR_HW
    float iso[MAX_ISO_STEP];
#endif

    short edge_thed             [MAX_ISO_STEP];
    float dir_min               [MAX_ISO_STEP];
    float smoth4                [MAX_ISO_STEP];
    short alpha_adp_en          [MAX_ISO_STEP];
    float l_alpha               [MAX_ISO_STEP];
    float g_alpha               [MAX_ISO_STEP];
    short enhance_luma_point    [RK_EDGEFILTER_LUMA_POINT_NUM];
    short edge_thed_1           [MAX_ISO_STEP][RK_EDGEFILTER_LUMA_POINT_NUM];
    float clamp_pos_gaus        [MAX_ISO_STEP][RK_EDGEFILTER_LUMA_POINT_NUM];
    float clamp_neg_gaus        [MAX_ISO_STEP][RK_EDGEFILTER_LUMA_POINT_NUM];
    short clamp_pos_dog         [MAX_ISO_STEP][RK_EDGEFILTER_LUMA_POINT_NUM];
    short clamp_neg_dog         [MAX_ISO_STEP][RK_EDGEFILTER_LUMA_POINT_NUM];
    float detail_alpha_gaus     [MAX_ISO_STEP][RK_EDGEFILTER_LUMA_POINT_NUM];
    float detail_alpha_dog      [MAX_ISO_STEP][RK_EDGEFILTER_LUMA_POINT_NUM];

    float gaus_luma_kernel      [MAX_ISO_STEP][RKEDGEFILTER_LUMA_GAU_DIAM * RKEDGEFILTER_LUMA_GAU_DIAM];
    float h0_h_coef_5x5         [MAX_ISO_STEP][RKEDGEFILTER_DIR_SMTH_DIAM   * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h1_v_coef_5x5         [MAX_ISO_STEP][RKEDGEFILTER_DIR_SMTH_DIAM   * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h2_m_coef_5x5         [MAX_ISO_STEP][RKEDGEFILTER_DIR_SMTH_DIAM   * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h3_p_coef_5x5         [MAX_ISO_STEP][RKEDGEFILTER_DIR_SMTH_DIAM   * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h_coef_5x5            [MAX_ISO_STEP][RKEDGEFILTER_GAUS_DIAM       * RKEDGEFILTER_GAUS_DIAM];
    float gf_coef_3x3           [MAX_ISO_STEP][RKEDGEFILTER_SHRP_DIAM       * RKEDGEFILTER_SHRP_DIAM];
    float dog_kernel_l            [MAX_ISO_STEP][RKEDGEFILTER_DOG_DIAM        * RKEDGEFILTER_DOG_DIAM];
    float dog_kernel_h            [MAX_ISO_STEP][RKEDGEFILTER_DOG_DIAM        * RKEDGEFILTER_DOG_DIAM];
    float dog_kernel_percent      [MAX_ISO_STEP];
} RKAsharp_EdgeFilter_Params_t;

typedef struct RKAsharp_EdgeFilter_Params_Select_s
{

    short edge_thed;
    float dir_min;
    float smoth4;
    short alpha_adp_en;
    float l_alpha;
    float g_alpha;
    short enhance_luma_point    [RK_EDGEFILTER_LUMA_POINT_NUM];
    short edge_thed_1           [RK_EDGEFILTER_LUMA_POINT_NUM];
    float clamp_pos_gaus        [RK_EDGEFILTER_LUMA_POINT_NUM];
    float clamp_neg_gaus        [RK_EDGEFILTER_LUMA_POINT_NUM];
    short clamp_pos_dog         [RK_EDGEFILTER_LUMA_POINT_NUM];
    short clamp_neg_dog         [RK_EDGEFILTER_LUMA_POINT_NUM];
    float detail_alpha_gaus     [RK_EDGEFILTER_LUMA_POINT_NUM];
    float detail_alpha_dog      [RK_EDGEFILTER_LUMA_POINT_NUM];

    float gaus_luma_kernel      [RKEDGEFILTER_LUMA_GAU_DIAM     * RKEDGEFILTER_LUMA_GAU_DIAM];
    float h0_h_coef_5x5         [RKEDGEFILTER_DIR_SMTH_DIAM     * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h1_v_coef_5x5         [RKEDGEFILTER_DIR_SMTH_DIAM     * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h2_m_coef_5x5         [RKEDGEFILTER_DIR_SMTH_DIAM     * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h3_p_coef_5x5         [RKEDGEFILTER_DIR_SMTH_DIAM     * RKEDGEFILTER_DIR_SMTH_DIAM];
    float h_coef_5x5            [RKEDGEFILTER_GAUS_DIAM         * RKEDGEFILTER_GAUS_DIAM];
    float gf_coef_3x3           [RKEDGEFILTER_SHRP_DIAM         * RKEDGEFILTER_SHRP_DIAM];
    float dog_kernel            [RKEDGEFILTER_DOG_DIAM          * RKEDGEFILTER_DOG_DIAM];

} RKAsharp_EdgeFilter_Params_Select_t;

typedef enum AsharpResult_e {
    ASHARP_RET_SUCCESS              = 0,   // this has to be 0, if clauses rely on it
    ASHARP_RET_FAILURE              = 1,   // process failure
    ASHARP_RET_INVALID_PARM     = 2,   // invalid parameter
    ASHARP_RET_WRONG_CONFIG     = 3,   // feature not supported
    ASHARP_RET_BUSY             = 4,   // there's already something going on...
    ASHARP_RET_CANCELED         = 5,   // operation canceled
    ASHARP_RET_OUTOFMEM         = 6,   // out of memory
    ASHARP_RET_OUTOFRANGE           = 7,   // parameter/value out of range
    ASHARP_RET_NULL_POINTER     = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    ASHARP_RET_DIVISION_BY_ZERO = 9,   // a divisor equals ZERO
    ASHARP_RET_NO_INPUTIMAGE        = 10   // no input image
} AsharpResult_t;

typedef enum AsharpState_e {
    ASHARP_STATE_INVALID           = 0,                   /**< initialization value */
    ASHARP_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    ASHARP_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    ASHARP_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    ASHARP_STATE_MAX                                      /**< max */
} AsharpState_t;

typedef enum AsharpOPMode_e {
    ASHARP_OP_MODE_INVALID           = 0,                   /**< initialization value */
    ASHARP_OP_MODE_AUTO               = 1,                   /**< instance is created, but not initialized */
    ASHARP_OP_MODE_MANUAL             = 2,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP_OP_MODE_MAX                                      /**< max */
} AsharpOPMode_t;

typedef enum AsharpParamMode_e {
    ASHARP_PARAM_MODE_INVALID           = 0,                   /**< initialization value */
    ASHARP_PARAM_MODE_NORMAL               = 1,                   /**< instance is created, but not initialized */
    ASHARP_PARAM_MODE_HDR             = 2,                   /**< instance is confiured (ready to start) or stopped */
    ASHARP_PARAM_MODE_GRAY            =3,                          /**< max */
    ASHARP_PARAM_MODE_MAX
} AsharpParamMode_t;



typedef struct Asharp_Manual_Attr_s
{
    int sharpEn;
    RKAsharp_Sharp_Params_Select_t stSharpParamSelect;

    int edgeFltEn;
    RKAsharp_EdgeFilter_Params_Select_t stEdgefilterParamSelect;

} Asharp_Manual_Attr_t;

typedef struct Asharp_Auto_Attr_s
{
    //all ISO params and select param
    //sharp
    int sharpEn;
    RKAsharp_Sharp_Params_t stSharpParam;
    RKAsharp_Sharp_Params_Select_t stSharpParamSelect;

    //edgefilter
    int edgeFltEn;
    RKAsharp_EdgeFilter_Params_t stEdgefilterParams;
    RKAsharp_EdgeFilter_Params_Select_t stEdgefilterParamSelect;
} Asharp_Auto_Attr_t;

typedef struct AsharpProcResult_s {
    int sharpEn;
    int edgeFltEn;

    //for sw simulator
    RKAsharp_Sharp_Params_Select_t stSharpParamSelect;
    RKAsharp_EdgeFilter_Params_Select_t stEdgefilterParamSelect;

    //for fgpa
    RKAsharp_Sharp_Fix_t stSharpFix;
    RKAsharp_Edgefilter_Fix_t stEdgefltFix;

} AsharpProcResult_t;

typedef struct AsharpConfig_s {

    AsharpState_t eState;
    AsharpOPMode_t eMode;

} AsharpConfig_t;

typedef struct rk_aiq_sharp_attrib_s {
    AsharpOPMode_t eMode;
    Asharp_Auto_Attr_t stAuto;
    Asharp_Manual_Attr_t stManual;
} rk_aiq_sharp_attrib_t;

typedef struct AsharpExpInfo_s {
    int hdr_mode;
    float arTime[3];
    float arAGain[3];
    float arDGain[3];
    int   arIso[3];
    int   arDcgMode[3];
    int   cur_snr_mode;
	int   snr_mode;

	float preTime[3];
	float preAGain[3];
	float preDGain[3];
	int preIso[3];
	int preDcgMode[3];
    int pre_snr_mode;
	
	int mfnr_mode_3to1;
} AsharpExpInfo_t;

typedef enum rk_aiq_sharp_module_e{
    ASHARP_MODULE_SHARP     	= 0,          /**< instance is created, but not initialized */
    ASHARP_MODULE_EDGEFILTER    = 1,          /**< instance is confiured (ready to start) or stopped */
} rk_aiq_sharp_module_t;


typedef struct rk_aiq_sharp_IQpara_s{
	int module_bits;
	
	CalibDb_Sharp_t stSharpPara;
    CalibDb_EdgeFilter_t stEdgeFltPara;
}rk_aiq_sharp_IQpara_t;


typedef struct rk_aiq_sharp_JsonPara_s{
	int module_bits;
	
	CalibDbV2_SharpV1_t sharp_v1;
    CalibDbV2_Edgefilter_t edgefilter_v1;
}rk_aiq_sharp_JsonPara_t;


RKAIQ_END_DECLARE

#endif


