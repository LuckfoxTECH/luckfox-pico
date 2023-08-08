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

typedef struct rk_aiq_uapiV2_wbV32_awb_attrib_s {
    //rk_aiq_uapiV2_wb_awb_wbGainAdjust_t wbGainAdjust;
    CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset;
    rk_aiq_uapiV2_wbV32_awb_mulWindow_t  multiWindow;
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

#endif

