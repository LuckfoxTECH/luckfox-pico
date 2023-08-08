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
} rk_aiq_wb_awb_alg_method_t;

typedef enum hdr_frame_choose_mode_e {
    hdr_frame_choose_mode_manual = 0,
    hdr_frame_choose_mode_auto
} hdr_frame_choose_mode_t;

typedef struct rk_aiq_wb_awb_damp_s {
    float dFStep;
    float dFMin;
    float dFMax;
    float LvVarTh;
}rk_aiq_wb_awb_damp_t;

typedef struct rk_aiq_wb_awb_attrib_s {
    rk_aiq_wb_awb_alg_method_t algMethod;
    rk_aiq_wb_awb_tolerance_t tolerance;//wb gain diff th for awb gain update, set 0 to disable this function
    rk_aiq_wb_awb_runinterval_t runInterval;
    bool cagaEn;
    bool wbGainAdjustEn;
    int cct_lut_cfg_num;
    cct_lut_cfg_lv_t cct_lut_cfg[CALD_AWB_CT_LV_NUM_MAX];
    bool wbGainClipEn;
    bool wbGainDaylightClipEn;
    cct_clip_cfg_t cct_clip_cfg;
    hdr_frame_choose_mode_t hdrFrameChooseMode;
    unsigned char   hdrFrameChoose;
    CalibDb_StatWindow_t measeureWindow;
    CalibDb_Awb_gain_offset_cfg_t wbGainOffset;
    rk_aiq_wb_awb_damp_t dampFactor;
    float convergedVarTh;
} rk_aiq_wb_awb_attrib_t;

typedef enum rk_aiq_wb_op_mode_s {
    RK_AIQ_WB_MODE_MANUAL                      = 0,        /**< run manual white balance */
    RK_AIQ_WB_MODE_AUTO                        = 1,        /**< run auto white balance */
    RK_AIQ_WB_MODE_MAX
} rk_aiq_wb_op_mode_t;

typedef struct rk_aiq_wb_attrib_s {
    bool byPass;
    rk_aiq_wb_op_mode_t mode;
    rk_aiq_wb_mwb_attrib_t stManual;
    rk_aiq_wb_awb_attrib_t stAuto;
} rk_aiq_wb_attrib_t;

typedef struct rk_aiq_wb_querry_info_s {
    rk_aiq_wb_gain_t gain;
    rk_aiq_wb_cct_t cctGloabl;
    bool awbConverged;
    uint32_t LVValue;
} rk_aiq_wb_querry_info_t;

typedef enum rk_aiq_wb_lock_state_s {
    RK_AIQ_WB_LOCK_INVALID                    = 0,        /**< initialization value */
    RK_AIQ_WB_LOCKED,
    RK_AIQ_WB_UNLOCKED,
    RK_AIQ_WB_LOCK_MAX
} rk_aiq_wb_lock_state_t;


#endif

