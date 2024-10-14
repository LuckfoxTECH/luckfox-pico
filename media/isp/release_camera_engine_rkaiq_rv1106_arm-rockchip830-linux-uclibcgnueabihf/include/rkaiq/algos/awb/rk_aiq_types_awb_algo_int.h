/*
* rk_aiq_types_awb_algo_int.h

* for rockchip v2.0.0
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
/* for rockchip v2.0.0*/

#ifndef __RK_AIQ_TYPE_AWB_ALGO_INT_H__
#define __RK_AIQ_TYPE_AWB_ALGO_INT_H__

#include "awb/rk_aiq_types_awb_algo.h"
#include "RkAiqCalibDbTypes.h"
#include "awb_head.h"
#include "awb_uapi_head.h"
#define RK_UAPI_AWB_CT_GRID_NUM 15
#define RK_UAPI_AWB_CT_LUT_NUM 8
#define RK_UAPI_AWB_LWR_NUM_MAX 8
#define ABLC_MAX_ISO_LEVEL (13)
typedef enum rk_aiq_wb_scene_e {
    RK_AIQ_WBCT_INCANDESCENT = 0,
    RK_AIQ_WBCT_FLUORESCENT,
    RK_AIQ_WBCT_WARM_FLUORESCENT,
    RK_AIQ_WBCT_DAYLIGHT,
    RK_AIQ_WBCT_CLOUDY_DAYLIGHT,
    RK_AIQ_WBCT_TWILIGHT,
    RK_AIQ_WBCT_SHADE
} rk_aiq_wb_scene_t;

typedef struct rk_aiq_wb_cct_s {
    float CCT;
    float CCRI;
} rk_aiq_wb_cct_t;

typedef struct color_tempture_info_s {
    bool valid;
    float CCT;
    float CCRI;
} color_tempture_info_t;

typedef struct awb_measure_blk_res_fl_s {
    float R;//mean r value and normalize to 0~1
    float G;
    float B;
} awb_measure_blk_res_fl_t;

typedef enum rk_aiq_wb_mwb_mode_e {
    RK_AIQ_MWB_MODE_INVAILD              = 0,
    RK_AIQ_MWB_MODE_CCT                  = 1,        /**< run manual white balance by cct*/
    RK_AIQ_MWB_MODE_WBGAIN               = 2,        /**< run manual white balance by wbgain*/
    RK_AIQ_MWB_MODE_SCENE                = 3,       /**< run manual white balance by scene selection*/
} rk_aiq_wb_mwb_mode_t;

typedef struct rk_aiq_wb_mwb_attrib_s {
    rk_aiq_uapi_sync_t sync;

    rk_aiq_wb_mwb_mode_t mode;
    union MWBPara_u {
        rk_aiq_wb_gain_t gain;
        rk_aiq_wb_scene_t scene;
        rk_aiq_wb_cct_t cct;
    } para;
} rk_aiq_wb_mwb_attrib_t;


typedef enum rk_aiq_wb_awb_alg_method_s {
    RK_AIQ_AWB_ALG_TYPE_INVAILD = 0,
    RK_AIQ_AWB_ALG_TYPE_GLOABL = 1,
    RK_AIQ_AWB_ALG_TYPE_GRAYWORD = 2,
    //add more
} rk_aiq_wb_awb_alg_method_t;

typedef enum hdr_frame_choose_mode_e {
    hdr_frame_choose_mode_manual = 0,
    hdr_frame_choose_mode_auto
} hdr_frame_choose_mode_t;

typedef struct rk_aiq_wb_awb_cct_lut_cfg_lv_s {
    float lv;
    int ct_grid_num;
    int cri_grid_num;
    float ct_range[2];//min,max, equal distance sapmle
    float cri_range[2];//min,max
    float ct_lut_out[CALD_AWB_CT_GRID_NUM_MAX * CALD_AWB_CRI_GRID_NUM_MAX];
    float cri_lut_out[CALD_AWB_CT_GRID_NUM_MAX * CALD_AWB_CRI_GRID_NUM_MAX];
} rk_aiq_wb_awb_cct_lut_cfg_lv_t;

typedef struct rk_aiq_wb_awb_attrib_s {
    rk_aiq_wb_awb_alg_method_t algMethod;
    CalibDb_Awb_tolerance_t tolerance;//wb gain diff th for awb gain update, set 0 to disable this function
    CalibDb_Awb_runinterval_t runInterval;
    bool cagaEn;
    bool wbGainAdjustEn;
    int cct_lut_cfg_num;
    rk_aiq_wb_awb_cct_lut_cfg_lv_t cct_lut_cfg[CALD_AWB_CT_LV_NUM_MAX];
    bool wbGainClipEn;
    bool wbGainDaylightClipEn;
    CalibDb_Awb_Cct_Clip_Cfg_t cct_clip_cfg;
    hdr_frame_choose_mode_t hdrFrameChooseMode;
    unsigned char   hdrFrameChoose;
    CalibDb_StatWindow_t measeureWindow;
    CalibDb_Awb_gain_offset_cfg_t wbGainOffset;
} rk_aiq_wb_awb_attrib_t;

typedef enum rk_aiq_wb_op_mode_s {
    RK_AIQ_WB_MODE_MANUAL                      = 0,        /**< run manual white balance */
    RK_AIQ_WB_MODE_AUTO                        = 1,        /**< run auto white balance */
    RK_AIQ_WB_MODE_MAX
} rk_aiq_wb_op_mode_t;

typedef struct rk_aiq_uapiV2_wb_opMode_s {
  rk_aiq_uapi_sync_t sync;

  rk_aiq_wb_op_mode_t mode;
} rk_aiq_uapiV2_wb_opMode_t;

typedef struct rk_aiq_wb_attrib_s {
    bool byPass;
    rk_aiq_wb_op_mode_t mode;
    rk_aiq_wb_mwb_attrib_t stManual;
    rk_aiq_wb_awb_attrib_t stAuto;
} rk_aiq_wb_attrib_t;

typedef struct rk_aiq_wb_querry_info_s {
    rk_aiq_wb_gain_t gain;//effective gain
    rk_aiq_wb_cct_t cctGloabl;
    bool awbConverged;
    uint32_t LVValue;
    rk_aiq_wb_gain_t stat_gain_glb;
    rk_aiq_wb_gain_t stat_gain_blk;
} rk_aiq_wb_querry_info_t;

typedef enum rk_aiq_wb_lock_state_s {
    RK_AIQ_WB_LOCK_INVALID                    = 0,        /**< initialization value */
    RK_AIQ_WB_LOCKED,
    RK_AIQ_WB_UNLOCKED,
    RK_AIQ_WB_LOCK_MAX
} rk_aiq_wb_lock_state_t;

typedef enum awb_hardware_version_e
{
    AWB_HARDWARE_V200 = 0,
    AWB_HARDWARE_V201 = 1,
    AWB_HARDWARE_V32 = 2,
    AWB_HARDWARE_VMAX
} awb_hardware_version_t;

typedef enum {
    AWB_CHANNEL_R = 0,
    AWB_CHANNEL_GR,
    AWB_CHANNEL_GB,
    AWB_CHANNEL_B,
    AWB_CHANNEL_MAX
} awb_channel_t;

typedef struct rk_aiq_uapiV2_wb_awb_wbGainAdjustLut_s {
  // M4_NUMBER_DESC("lumaValue", "f32", M4_RANGE(0, 255000), "0", M4_DIGIT(0))
  float lumaValue;
  // M4_NUMBER_DESC("ct_grid_num", "s32", M4_RANGE(0, 32), "0", M4_DIGIT(0))
  int ct_grid_num;
  // M4_NUMBER_DESC("cri_grid_num", "s32", M4_RANGE(0, 32), "0", M4_DIGIT(0))
  int cri_grid_num;
  // M4_ARRAY_DESC("ct_in_range", "f32", M4_SIZE(1,2), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
  float ct_in_range[2];//min,max, equal distance sapmle
  // M4_ARRAY_DESC("cri_in_range", "f32", M4_SIZE(1,2), M4_RANGE(-2,2), "0", M4_DIGIT(4), M4_DYNAMIC(0))
  float cri_in_range[2];//min,max
  // M4_ARRAY_DESC("ct_lut_out", "f32", M4_SIZE(9,7), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
  float *ct_lut_out;//size is ct_grid_num*cri_grid_num
  // M4_ARRAY_DESC("cri_lut_out", "f32", M4_SIZE(9,7), M4_RANGE(-2,2), "0", M4_DIGIT(4), M4_DYNAMIC(0))
  float *cri_lut_out;
} rk_aiq_uapiV2_wb_awb_wbGainAdjustLut_t;

typedef struct rk_aiq_uapiV2_wb_awb_wbGainAdjust_s {
  rk_aiq_uapi_sync_t sync;

  // M4_BOOL_DESC("enable", "1")
  bool enable;
  // M4_STRUCT_LIST_DESC("lutAll", M4_SIZE(1,8), "normal_ui_style")
  rk_aiq_uapiV2_wb_awb_wbGainAdjustLut_t *lutAll;
  int lutAll_len;
} rk_aiq_uapiV2_wb_awb_wbGainAdjust_t;

typedef struct rk_aiq_uapiV2_wb_awb_wbGainOffset_s{
  rk_aiq_uapi_sync_t sync;

  CalibDbV2_Awb_gain_offset_cfg_t gainOffset;
}rk_aiq_uapiV2_wb_awb_wbGainOffset_t;


typedef struct rk_aiq_uapiV2_wb_awb_mulWindow_s {
  rk_aiq_uapi_sync_t sync;

  CalibDbV2_Awb_Mul_Win_t multiWindw;
} rk_aiq_uapiV2_wb_awb_mulWindow_t;


typedef struct rk_aiq_uapiV2_wbV21_awb_attrib_s {
    rk_aiq_uapiV2_wb_awb_wbGainAdjust_t wbGainAdjust;
    CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset;
} rk_aiq_uapiV2_wbV21_awb_attrib_t,rk_aiq_uapiV2_wbV30_awb_attrib_t;

typedef struct rk_aiq_uapiV2_wbV20_awb_attrib_s {
    rk_aiq_uapiV2_wb_awb_wbGainAdjust_t wbGainAdjust;
    CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset;
    CalibDbV2_Awb_Mul_Win_t  multiWindow;
} rk_aiq_uapiV2_wbV20_awb_attrib_t;

typedef struct rk_aiq_uapiV2_wbV21_attrib_s {
    rk_aiq_uapi_sync_t sync;

    bool byPass;
    rk_aiq_wb_op_mode_t mode;
    rk_aiq_wb_mwb_attrib_t stManual;
    rk_aiq_uapiV2_wbV30_awb_attrib_t stAuto;
} rk_aiq_uapiV2_wbV21_attrib_t;

typedef struct rk_aiq_uapiV2_wbV20_attrib_s {
    bool byPass;
    rk_aiq_wb_op_mode_t mode;
    rk_aiq_wb_mwb_attrib_t stManual;
    rk_aiq_uapiV2_wbV20_awb_attrib_t stAuto;
} rk_aiq_uapiV2_wbV20_attrib_t;

typedef rk_aiq_uapiV2_wbV21_attrib_t rk_aiq_uapiV2_wbV30_attrib_t;

typedef struct rk_aiq_uapiV2_wbV32_awb_mulWindow_s {
  rk_aiq_uapi_sync_t sync;
  bool  enable;
  float window[4][4];//percent
} rk_aiq_uapiV2_wbV32_awb_mulWindow_t;

typedef struct rk_aiq_uapiV2_wb_awb_cctClipCfg_s {
    bool enable;
    float cct[RK_UAPI_AWB_CT_GRID_NUM];
    int cct_len;
    float cri_bound_up[RK_UAPI_AWB_CT_GRID_NUM];
    float cri_bound_low[RK_UAPI_AWB_CT_GRID_NUM];
} rk_aiq_uapiV2_wb_awb_cctClipCfg_t;

typedef struct rk_aiq_uapiV2_wbV32_awb_gainAdjust_s {
    bool enable;
    CalibDbV2_Awb_Ctrl_Dat_Selt_t ctrlDataSelt;
    CalibDbV2_Awb_Gain_Adj_Dat_Sl_t adjDataSelt;
    CalibDbV2_Awb_Cct_Lut_Cfg_Lv2_t lutAll[RK_UAPI_AWB_CT_LUT_NUM];
    int lutAll_len;
} rk_aiq_uapiV2_wbV32_awb_gainAdjust_t;

typedef struct rk_aiq_uapiV2_wb_awb_dampFactor_s {
    float dFStep;
    float dFMin;
    float dFMax;
} rk_aiq_uapiV2_wb_awb_dampFactor_t;

typedef struct rk_aiq_uapiV2_wbV32_awb_attrib_s {
    rk_aiq_wb_awb_alg_method_t algMtdTp;
    bool algMtdTp_valid;
    rk_aiq_uapiV2_wb_awb_dampFactor_t dampFactor;
    bool dampFactor_valid;
    CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset;
    bool wbGainOffset_valid;
    rk_aiq_uapiV2_wbV32_awb_mulWindow_t  multiWindow;
    bool multiWindow_valid;
    CalibDbV2_Awb_DaylgtClip_Cfg_t wbGainDaylightClip;
    bool wbGainDaylightClip_valid;
    rk_aiq_uapiV2_wb_awb_cctClipCfg_t wbGainClip;
    bool wbGainClip_valid;
    rk_aiq_uapiV2_wbV32_awb_gainAdjust_t wbGainAdjust;
    bool wbGainAdjust_valid;

} rk_aiq_uapiV2_wbV32_awb_attrib_t;

typedef struct rk_aiq_uapiV2_wbV32_attrib_t {
    rk_aiq_uapi_sync_t sync;

    bool byPass;
    rk_aiq_wb_op_mode_t mode;
    rk_aiq_wb_mwb_attrib_t stManual;
    rk_aiq_uapiV2_wbV32_awb_attrib_t stAuto;
} rk_aiq_uapiV2_wbV32_attrib_t;

typedef struct rk_aiq_uapiV2_awb_wrtIn_attr_s {
    rk_aiq_uapi_sync_t sync;
    bool en;
    int mode;
    char path[100];
    int call_cnt;
} rk_aiq_uapiV2_awb_wrtIn_attr_t;

typedef struct rk_aiq_uapiV2_awb_ffwbgain_attr_s{
    rk_aiq_uapi_sync_t sync;
    rk_aiq_wb_gain_t wggain;
}rk_aiq_uapiV2_awb_ffwbgain_attr_t;

typedef struct rk_aiq_uapiV2_awb_ExtR_Wei_V32_s {
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    int lumaValue_len;
    float weight[CALD_AWB_LV_NUM_MAX];
    int weight_len;
} rk_aiq_uapiV2_awb_ExtR_Wei_V32_t;



typedef struct rk_aiq_uapiV2_awb_Gain_Lut_Cfg_s {
    int ct_grid_num;
    int cri_grid_num;
    float *rgct_in_ds;
    float *bgcri_in_ds;
    float *rgct_lut_out;
    float *bgcri_lut_out;
} rk_aiq_uapiV2_awb_Gain_Lut_Cfg_t;

typedef struct rk_aiq_uapiV2_awb_CamMain_info_s {
    rk_aiq_wb_gain_t wbgain;
    float fLV;
    bool fLV_valid;
} rk_aiq_uapiV2_awb_CamMain_info_t;

typedef struct rk_aiq_uapiV2_awb_Slave2Main_Cfg_s {
    bool enable;
    rk_aiq_uapiV2_awb_Gain_Lut_Cfg_t cct_lut_cfg;
    rk_aiq_uapiV2_awb_CamMain_info_t camM;
} rk_aiq_uapiV2_awb_Slave2Main_Cfg_t;

typedef struct rk_aiq_uapiV2_ExtRange_V32_s {
    CalibDbV2_Awb_Ext_Range_Dom_t domain;
    CalibDbV2_Awb_Ext_Range_Mode_t mode;
    int region[4];
    rk_aiq_uapiV2_awb_ExtR_Wei_V32_t weightInculde;
} rk_aiq_uapiV2_ExtRange_V32_t;

typedef struct rk_aiq_uapiV2_Awb_Luma_Weight_Lv_s {
    float LvValue;
    CalibDbV2_Awb_Lum_Wgt_Lv_Rto_t ratioSet[RK_UAPI_AWB_LWR_NUM_MAX];
    int ratioSet_len;
} rk_aiq_uapiV2_Awb_Luma_Weight_Lv_t;

typedef struct rk_aiq_uapiV2_Awb_Luma_Weight_s {
    bool enable;
    CalibDbV2_Awb_Lu_Wgt_En_Th_t wpDiffWeiEnableTh;
    unsigned char wpDiffwei_y[9];
    unsigned char  perfectBin[8];// true means the luma is appropriate
    rk_aiq_uapiV2_Awb_Luma_Weight_Lv_t wpDiffWeightLvSet[CALD_AWB_LV_NUM_MAX];
    int wpDiffWeightLvSet_len;
} rk_aiq_uapiV2_Awb_Luma_Weight_t;


typedef struct rk_aiq_uapiV2_Awb_Light_V32_s {
    char name[CALD_AWB_ILLUMINATION_NAME];
    CalibDbV2_Awb_DoorType_t doorType;
    float standardGainValue[4];
    CalibDbV2_Uv_Range_Ill_t uvRegion;
    CalibDbV2_Tcs_Range_Ill2_t xyRegion;
    CalibDbV2_Yuv3D_2_Range_Ill_t rtYuvRegion;
    unsigned char staWeight[CALD_AWB_LV_NUM_MAX];
    unsigned int dayGainLvThSet[2];
    float defaultDayGainLow[4];
    float defaultDayGainHigh[4];
    rk_aiq_uapiV2_awb_ExtR_Wei_V32_t weight;
} rk_aiq_uapiV2_Awb_Light_V32_t;


typedef struct rk_aiq_uapiV2_Awb_offset_data_s {
    float ISO[ABLC_MAX_ISO_LEVEL];
    int ISO_len;
    float R_Channel[ABLC_MAX_ISO_LEVEL];
    int R_Channel_len;
    float Gr_Channel[ABLC_MAX_ISO_LEVEL];
    int Gr_Channel_len;
    float Gb_Channel[ABLC_MAX_ISO_LEVEL];
    int Gb_Channel_len;
    float B_Channel[ABLC_MAX_ISO_LEVEL];
    int B_Channel_len;
} rk_aiq_uapiV2_Awb_offset_data_t;

typedef struct rk_aiq_uapiV2_Awb_Blc_s {
    bool enable;
    rk_aiq_uapiV2_Awb_offset_data_t offset;//r gr gb b
} rk_aiq_uapiV2_Awb_Blc_t;

typedef struct rk_aiq_uapiV2_Awb_Limit_Range_V32_s {
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    int lumaValue_len;
    float maxR[CALD_AWB_LV_NUM_MAX];
    int maxR_len;
    float minR[CALD_AWB_LV_NUM_MAX];
    int minR_len;
    float maxG[CALD_AWB_LV_NUM_MAX];
    int maxG_len;
    float minG[CALD_AWB_LV_NUM_MAX];
    int minG_len;
    float maxB[CALD_AWB_LV_NUM_MAX];
    int maxB_len;
    float minB[CALD_AWB_LV_NUM_MAX];
    int minB_len;
    float maxY[CALD_AWB_LV_NUM_MAX];
    int maxY_len;
    float minY[CALD_AWB_LV_NUM_MAX];
    int minY_len;
} rk_aiq_uapiV2_Awb_Limit_Range_V32_t;

typedef struct rk_aiq_uapiV2_Wb_Awb_IqAtPa_V32_s {
    CalibDbV2_Awb_Raw_Select_t rawSelectPara;
    rk_aiq_uapiV2_Awb_Blc_t blc2ForAwb;
    bool                lscBypass;
    bool                uvDetectionEnable;
    bool                xyDetectionEnable;
    bool                yuvDetectionEnable;
    bool                blkStatisticsEnable;
    CalibDbV2_Awb_Down_Scale_Mode_t       downScaleMode;
    CalibDbV2_Awb_Blk_Stat_V21_t      blkMeasureMode;
    CalibDbV2_StatWindow_t mainWindow;
    rk_aiq_uapiV2_Awb_Limit_Range_V32_t limitRange;
    CalibDbV2_Rgb2Tcs_t rgb2TcsPara;
    float rgb2RotationYuvMat[16];
    rk_aiq_uapiV2_ExtRange_V32_t extraWpRange[CALD_AWB_EXCRANGE_NUM_MAX];
    rk_aiq_uapiV2_Awb_Luma_Weight_t wpDiffLumaWeight;
    bool           wpDiffBlkWeiEnable;
    unsigned short wpDiffBlkWeight[CALD_AWB_GRID_NUM_TOTAL];
    rk_aiq_uapiV2_Awb_Light_V32_t lightSources[CALD_AWB_LS_NUM_MAX];
    int lightSources_len;
    CalibDbV2_Wb_Awb_EarlAct_t earlierAwbAct;

} rk_aiq_uapiV2_Wb_Awb_IqAtPa_V32_t;

typedef struct rk_aiq_uapiV2_Awb_xyRg_stb_WpTh_s {
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    int lumaValue_len;
    float forBigType[CALD_AWB_LV_NUM_MAX];
    int forBigType_len;
    float forExtraType[CALD_AWB_LV_NUM_MAX];
    int forExtraType_len;
} rk_aiq_uapiV2_Awb_xyRg_stb_WpTh_t;

typedef struct rk_aiq_uapiV2_Awb_xyRegion_stable_s {
    bool enable;
    rk_aiq_uapiV2_Awb_xyRg_stb_WpTh_t wpNumTh;
    int xyTypeListSize;
    float varianceLumaTh;
} rk_aiq_uapiV2_Awb_xyRegion_stable_t;

typedef struct rk_aiq_uapiV2_Awb_Sgc_Ls_s {
    char name[CALD_AWB_ILLUMINATION_NAME];
    float RGain;
    float BGain;
} rk_aiq_uapiV2_Awb_Sgc_Ls_t;

typedef struct rk_aiq_uapiV2_Awb_Sgc_s {
    bool enable;
    CalibDbV2_Awb_Sgc_Cblk_t colorBlock[CALD_AWB_SGC_NUM_MAX];
    int colorBlock_len;
    rk_aiq_uapiV2_Awb_Sgc_Ls_t lsUsedForEstimation[CALD_AWB_LS_NUM_MAX];
    int lsUsedForEstimation_len;
    float alpha;
} rk_aiq_uapiV2_Awb_Sgc_t;

typedef struct rk_aiq_uapiV2_Wb_Awb_Div_WpTh_s {
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    int lumaValue_len;
    float low[CALD_AWB_LV_NUM_MAX];
    int low_len;
    float high[CALD_AWB_LV_NUM_MAX];
    int high_len;
} rk_aiq_uapiV2_Wb_Awb_Div_WpTh_t;

typedef struct rk_aiq_uapiV2_Wb_Awb_Div_s {
    unsigned int lumaValThLow;
    unsigned int lumaValThLow2;
    unsigned int lumaValThHigh;
    unsigned int lumaValThHigh2;
    rk_aiq_uapiV2_Wb_Awb_Div_WpTh_t wpNumTh;
} rk_aiq_uapiV2_Wb_Awb_Div_t;


typedef struct rk_aiq_uapiV2_Awb_Tolerance_s {
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    int lumaValue_len;
    float toleranceValue[CALD_AWB_LV_NUM_MAX];
    int toleranceValue_len;
}  rk_aiq_uapiV2_Awb_Tolerance_t;

typedef struct rk_aiq_uapiV2_Awb_runinterval_s {
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    int lumaValue_len;
    float intervalValue[CALD_AWB_LV_NUM_MAX];
    int intervalValue_len;
}  rk_aiq_uapiV2_Awb_runinterval_t;

typedef struct rk_aiq_uapiV2_Awb_SmartRun_cfg_s {
    float lumaValue[CALD_AWB_LV_NUM_MAX];
    int lumaValue_len;
    float lvVarTh[CALD_AWB_LV_NUM_MAX];
    int lvVarTh_len;
    float wbgainAlgDiffTh[CALD_AWB_LV_NUM_MAX];
    int wbgainAlgDiffTh_len;
    float wbgainHwDiffTh[CALD_AWB_LV_NUM_MAX];
    int wbgainHwDiffTh_len;
} rk_aiq_uapiV2_Awb_SmartRun_cfg_t;

typedef struct rk_aiq_uapiV2_Awb_SmartRun_s {
    bool  enable;
    rk_aiq_uapiV2_Awb_SmartRun_cfg_t cfg;
} rk_aiq_uapiV2_Awb_SmartRun_t;

typedef struct rk_aiq_uapiV2_Wb_Awb_IqAtExtPa_V32_s {
    char lightSourceForFirstFrame[CALD_AWB_ILLUMINATION_NAME];
    rk_aiq_uapiV2_Awb_SmartRun_t smartRun;
    rk_aiq_uapiV2_Awb_Tolerance_t tolerance;
    rk_aiq_uapiV2_Awb_runinterval_t runInterval;
    CalibDbV2_Awb_DampFactor_t dampFactor;
    rk_aiq_uapiV2_wbV32_awb_gainAdjust_t wbGainAdjust;
    CalibDbV2_Awb_DaylgtClip_Cfg_t wbGainDaylightClip;
    rk_aiq_uapiV2_wb_awb_cctClipCfg_t wbGainClip;
    rk_aiq_uapiV2_Wb_Awb_Div_t division;
    float defaultNightGain[4];
    unsigned int lumaValueMatrix[CALD_AWB_LV_NUM_MAX];
    unsigned char defaultNightGainWeight[CALD_AWB_LV_NUM_MAX];
    CalibDbV2_Wb_Awb_Prob_Calc_Dt_t probCalcDis;
    CalibDbV2_Wb_Awb_Prob_Calc_Lv_t probCalcLv;
    CalibDbV2_Wb_Awb_Prob_Calc_Wp_t probCalcWp;
    CalibDbV2_Wb_Awb_Convg_t converged;
    rk_aiq_uapiV2_Awb_xyRegion_stable_t xyRegionStableSelection;
    unsigned char weightForNightGainCalc[CALD_AWB_LV_NUM_MAX];
    int weightForNightGainCalc_len;
    rk_aiq_uapiV2_Awb_Sgc_t singleColorProces;
    float lineRgBg[3];
    float lineRgProjCCT[3];
    CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset;
    CalibDbV2_Awb_Ava_Site_Rec_t avaSiteRec;
} rk_aiq_uapiV2_Wb_Awb_IqAtExtPa_V32_t;

#endif

