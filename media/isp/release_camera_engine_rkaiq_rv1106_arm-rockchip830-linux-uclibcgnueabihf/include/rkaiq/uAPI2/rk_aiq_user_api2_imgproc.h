/*
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

#ifndef _RK_AIQ_UAPI2_IMGPROC_H_
#define _RK_AIQ_UAPI2_IMGPROC_H_

#include "base/xcam_common.h"
#include "rk_aiq_user_api2_a3dlut.h"
#include "rk_aiq_user_api2_abayer2dnr_v2.h"
#include "rk_aiq_user_api2_abayer2dnr_v23.h"
#include "rk_aiq_user_api2_abayernr_v2.h"
#include "rk_aiq_user_api2_abayertnr_v2.h"
#include "rk_aiq_user_api2_abayertnr_v23.h"
#include "rk_aiq_user_api2_ablc.h"
#include "rk_aiq_user_api2_accm.h"
#include "rk_aiq_user_api2_acnr_v1.h"
#include "rk_aiq_user_api2_acnr_v2.h"
#include "rk_aiq_user_api2_acnr_v30.h"
#include "rk_aiq_user_api2_acp.h"
#include "rk_aiq_user_api2_adebayer.h"
#include "rk_aiq_user_api2_adegamma.h"
#include "rk_aiq_user_api2_adehaze.h"
#include "rk_aiq_user_api2_adpcc.h"
#include "rk_aiq_user_api2_adrc.h"
#include "rk_aiq_user_api2_ae.h"
#include "rk_aiq_user_api2_af.h"
#include "rk_aiq_user_api2_afec.h"
#include "rk_aiq_user_api2_agamma.h"
#include "rk_aiq_user_api2_aie.h"
#include "rk_aiq_user_api2_aldch.h"
#include "rk_aiq_user_api2_alsc.h"
#include "rk_aiq_user_api2_amerge.h"
#include "rk_aiq_user_api2_anr.h"
#include "rk_aiq_user_api2_asharp_v3.h"
#include "rk_aiq_user_api2_asharp_v33.h"
#include "rk_aiq_user_api2_asharp_v4.h"
#include "rk_aiq_user_api2_atmo.h"
#include "rk_aiq_user_api2_awb.h"
#include "rk_aiq_user_api2_aynr_v2.h"
#include "rk_aiq_user_api2_aynr_v22.h"
#include "rk_aiq_user_api2_aynr_v3.h"
#include "rk_aiq_user_api_common.h"
#include "rk_aiq_user_api2_again_v2.h"
#include "rk_aiq_user_api2_aldch_v21.h"
#include "rk_aiq_user_api2_ablc_v32.h"

RKAIQ_BEGIN_DECLARE
/*
**********************************************************
*                        API of AEC module of V2
**********************************************************
*/
/*
*****************************
*
* Desc: set ae lock
* Argument:
*       true, ae lock
*           false, ae unlock
*****************************
*/
XCamReturn rk_aiq_uapi2_setAeLock(const rk_aiq_sys_ctx_t* ctx, bool on);

/*
*****************************
*
* Desc: set ae mode
* Argument:
*   mode contains: auto & manual
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setExpMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi2_getExpMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set exposure parameter
* Argument:
*    auto exposure mode:
*      exposure gain will be adjust between [gain->min, gain->max]
*    manual exposure mode:
*      gain->min == gain->max
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setExpGainRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *gain);
XCamReturn rk_aiq_uapi2_getExpGainRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *gain);
/*
*****************************
*
* Desc: set exposure parameter
* Argument:
*    auto exposure mode:
*       exposure time will be adjust between [time->min, time->max]
*    manual exposure mode:
*       exposure time will be set gain->min == gain->max;
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setExpTimeRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *time);
XCamReturn rk_aiq_uapi2_getExpTimeRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *time);

/*
*****************************
*
* Desc: blacklight compensation
* Argument:
*      on:  1  on
*           0  off
*      areaType: blacklight compensation area
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setBLCMode(const rk_aiq_sys_ctx_t* ctx, bool on, aeMeasAreaType_t areaType);

/*
*****************************
*
* Desc: backlight compensation strength,only available in normal mode
* Argument:
*      strength:  [1,100]
*****************************
*/
XCamReturn rk_aiq_uapi2_setBLCStrength(const rk_aiq_sys_ctx_t* ctx, int strength);

/*
*****************************
*
* Desc: highlight compensation
* Argument:
*      on:  1  on
*           0  off
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setHLCMode(const rk_aiq_sys_ctx_t* ctx, bool on);

/*
*****************************
*
* Desc: highlight compensation strength,only available in normal mode
* Argument:
*      strength:  [1,100]
*****************************
*/
XCamReturn rk_aiq_uapi2_setHLCStrength(const rk_aiq_sys_ctx_t* ctx, int strength);

/*
*****************************
*
* Desc: set anti-flicker mode
* Argument:
*    mode
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAntiFlickerEn(const rk_aiq_sys_ctx_t* ctx, bool on);
XCamReturn rk_aiq_uapi2_getAntiFlickerEn(const rk_aiq_sys_ctx_t* ctx, bool* on);

/*
*****************************
*
* Desc: set anti-flicker mode
* Argument:
*    mode
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAntiFlickerMode(const rk_aiq_sys_ctx_t* ctx, antiFlickerMode_t mode);
XCamReturn rk_aiq_uapi2_getAntiFlickerMode(const rk_aiq_sys_ctx_t* ctx, antiFlickerMode_t *mode);

/*
**********************************************************
* White balance & Color
**********************************************************
*/

/*
*****************************
*
* Desc: set white balance mode
* Argument:
*   mode:  auto: auto white balance
*          manual: manual white balance
*****************************
*/
XCamReturn rk_aiq_uapi2_setWBMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi2_getWBMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);


/*
*****************************
*
* Desc: lock/unlock auto white balance
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_lockAWB(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi2_unlockAWB(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set manual white balance scene mode
* Argument:
*   ct_scene:
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMWBScene(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_scene_t scene);
XCamReturn rk_aiq_uapi2_getMWBScene(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_scene_t *scene);


/*
*****************************
*
* Desc: set manual white balance r/b gain
* Argument:
*   ct_scene:
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMWBGain(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_gain_t *gain);
XCamReturn rk_aiq_uapi2_getWBGain(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_gain_t *gain);

/*
*****************************
*
* Desc: set manual white balance color temperature
* Argument:
*   ct: color temperature value [2800, 7500]K
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMWBCT(const rk_aiq_sys_ctx_t* ctx, unsigned int ct);
XCamReturn rk_aiq_uapi2_getWBCT(const rk_aiq_sys_ctx_t* ctx, unsigned int *ct);

/*
*****************************
*
* Desc: set wbgain offset for auto white balance
* Argument:
*   attr: wbgain offset  [-4, 4]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAwbGainOffsetAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wb_awb_wbGainOffset_t attr);
XCamReturn rk_aiq_uapi2_getAwbGainOffsetAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wb_awb_wbGainOffset_t *attr);

/*
*****************************
*
* Desc: set hue adjustment para  for auto white balance
* Argument:
*   attr :  hue adjustment para
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAwbGainAdjustAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wb_awb_wbGainAdjust_t attr);
XCamReturn rk_aiq_uapi2_getAwbGainAdjustAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wb_awb_wbGainAdjust_t *attr);

/*
*****************************
*
* Desc: set multiwindow  para  for auto white balance; only for 1109 1126
* Argument:
*   attr :   set multiwindow  para
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAwbMultiWindowAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_uapiV2_wb_awb_mulWindow_t attr);
XCamReturn rk_aiq_uapi2_getAwbMultiWindowAttrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_uapiV2_wb_awb_mulWindow_t *attr);


/*
*****************************
*
* Desc: set all api  para  for auto white balance ; only for 3588
* Argument:
*   attr :   all paras for awb api
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAwbV30AllAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wbV30_attrib_t attr);
XCamReturn rk_aiq_uapi2_getAwbV30AllAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wbV30_attrib_t *attr);


/*
*****************************
*
* Desc: set all api  para  for auto white balance ; only for 3566 3568
* Argument:
*   attr :   all paras for awb api
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAwbV21AllAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wbV21_attrib_t attr);
XCamReturn rk_aiq_uapi2_getAwbV21AllAttrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapiV2_wbV21_attrib_t *attr);



/*
*****************************
*
* Desc: set power line frequence
* Argument:
*    freq
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setExpPwrLineFreqMode(const rk_aiq_sys_ctx_t* ctx, expPwrLineFreq_t freq);
XCamReturn rk_aiq_uapi2_getExpPwrLineFreqMode(const rk_aiq_sys_ctx_t* ctx, expPwrLineFreq_t *freq);

/*
*****************************
*
* Desc: Adjust image gamma
*
* Argument:
*   GammaCoef: [0, 100]
*   SlopeAtZero: [-0.05, 0.05]
*****************************
*/
XCamReturn rk_aiq_uapi2_setGammaCoef(const rk_aiq_sys_ctx_t* ctx, float GammaCoef, float SlopeAtZero);

/*
*****************************
*
* Desc: set/get dark area boost strength
*    this function is active for normal mode
* Argument:
*   level: [1, 10]
*   only valid in RV1109/RV1126
*****************************
*/
XCamReturn rk_aiq_uapi2_setDarkAreaBoostStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi2_getDarkAreaBoostStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: set manual hdr strength
*    this function is active for HDR is manual mode
* Argument:
*   level: [1, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMHDRStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn rk_aiq_uapi2_getMHDRStrth(const rk_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
*****************************
*
* Desc: set manual dehaze module enable
*     enable/disable dehaze module function, including dehaze, enhance and hist
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setDehazeModuleEnable(const rk_aiq_sys_ctx_t* ctx, bool on);

/*
*****************************
*
* Desc: set manual dehaze enable
*     enable/disable dehaze function
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setDehazeEnable(const rk_aiq_sys_ctx_t* ctx, bool on);

/*
*****************************
*
* Desc: set/get manual dehaze strength
*     this function is active for dehaze is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMDehazeStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi2_getMDehazeStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: set manual enhance enable
*     enable/disable enhance function
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setEnhanceEnable(const rk_aiq_sys_ctx_t* ctx, bool on);

/*
*****************************
*
* Desc: set/get manual enhance strength
*     this function is active for dehaze is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMEnhanceStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi2_getMEnhanceStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: set/get manual enhance Chrome strength
*     this function is active for dehaze is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMEnhanceChromeStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi2_getMEnhanceChromeStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int* level);

/*
*****************************
*
* Desc: set/get manual drc Local TMO
*     this function is active for DRC is DRC Gain mode
*     use in RK356x
* Argument:
*   LocalWeit: [0, 1]
*   GlobalContrast: [0, 1]
*   LoLitContrast: [0, 1]
*   only valid in RK356x
*****************************
*/
XCamReturn rk_aiq_uapi2_setDrcLocalTMO(const rk_aiq_sys_ctx_t* ctx, float LocalWeit, float GlobalContrast, float LoLitContrast);
XCamReturn rk_aiq_uapi2_getDrcLocalTMO(const rk_aiq_sys_ctx_t* ctx, float* LocalWeit, float* GlobalContrast, float* LoLitContrast);

/*
*****************************
*
* Desc: set/get manual drc Local Data
*     this function is active for DRC is Auto mode
*     use in RK3588, RV1106
* Argument:
*   LocalWeit: [0, 1]
*   GlobalContrast: [0, 1]
*   LoLitContrast: [0, 1]
*   LocalAutoEnable: [0, 1]
*   LocalAutoWeit: [0, 1]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setDrcLocalData(const rk_aiq_sys_ctx_t* ctx, float LocalWeit, float GlobalContrast, float LoLitContrast, int LocalAutoEnable, float LocalAutoWeit);
XCamReturn rk_aiq_uapi2_getDrcLocalData(const rk_aiq_sys_ctx_t* ctx, float* LocalWeit, float* GlobalContrast, float* LoLitContrast, int* LocalAutoEnable, float* LocalAutoWeit);

/*
*****************************
*
* Desc: set/get manual drc HiLit
*     this function is active for DRC is Auto mode
*     use in RK3588, RV1106
* Argument:
*   Strength: [0, 1]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setDrcHiLit(const rk_aiq_sys_ctx_t* ctx, float Strength);
XCamReturn rk_aiq_uapi2_getDrcHiLit(const rk_aiq_sys_ctx_t* ctx, float* Strength);

/*
*****************************
*
* Desc: set/get manual drc Gain
*     this function is active for DRC is Auto mode
*     use in RK3588, RV1106
* Argument:
*   Gain: [1, 8]
*   Alpha: [0, 1]
*   Clip: [0, 64]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setDrcGain(const rk_aiq_sys_ctx_t* ctx, float Gain, float Alpha, float Clip);
XCamReturn rk_aiq_uapi2_getDrcGain(const rk_aiq_sys_ctx_t* ctx, float* Gain, float* Alpha, float* Clip);

/*
**********************************************************
* Noise reduction
**********************************************************
*/
/*
*****************************
*
* Desc: set noise reduction mode
* Argument:
*   mode:
*     auto: auto noise reduction
*     manual：manual noise reduction
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setNRMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi2_getNRMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set normal noise reduction strength
* Argument:
*   level: [0, 100]
* Normal mode
*****************************
*/
XCamReturn rk_aiq_uapi2_setANRStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi2_getANRStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);


/*
*****************************
*
* Desc: set manual spatial noise reduction strength
*    this function is active for NR is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMSpaNRStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn rk_aiq_uapi2_getMSpaNRStrth(const rk_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
*****************************
*
* Desc: set manual time noise reduction strength
*     this function is active for NR is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMTNRStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);

XCamReturn rk_aiq_uapi2_getMTNRStrth(const rk_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
*****************************
*
* Desc: Adjust image sharpness level
* Argument:
*    level: sharpness level, [0, 100]
*****************************
*/
XCamReturn rk_aiq_uapi2_setSharpness(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi2_getSharpness(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
**********************************************************
* Focus & Zoom
**********************************************************
*/
/*
*****************************
*
* Desc: set focus mode
* Argument:
*   mode:  auto: auto focus
*          manual: manual focus
*          semi-auto: semi-auto focus
*****************************
*/
XCamReturn rk_aiq_uapi2_setFocusMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi2_getFocusMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set fix mode code
* Argument:
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setFocusPosition(const rk_aiq_sys_ctx_t* ctx, short code);
XCamReturn rk_aiq_uapi2_getFocusPosition(const rk_aiq_sys_ctx_t* ctx, short *code);

/*
*****************************
*
* Desc: set focus window
* Argument:
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setFocusWin(const rk_aiq_sys_ctx_t* ctx, paRect_t *rect);
XCamReturn rk_aiq_uapi2_getFocusWin(const rk_aiq_sys_ctx_t* ctx, paRect_t *rect);

/*
*****************************
*
* Desc: lock/unlock auto focus
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_lockFocus(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi2_unlockFocus(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: oneshot focus
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_oneshotFocus(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: manual triger focus
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_manualTrigerFocus(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: tracking focus
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_trackingFocus(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: af serach path record
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_getSearchPath(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_sec_path_t* path);

/*
*****************************
*
* Desc: af serach path record
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_getSearchResult(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_result_t* result);

/*
*****************************
*
* Desc: set/get zoom position
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setOpZoomPosition(const rk_aiq_sys_ctx_t* ctx, int pos);
XCamReturn rk_aiq_uapi2_getOpZoomPosition(const rk_aiq_sys_ctx_t* ctx, int *pos);
XCamReturn rk_aiq_uapi2_endOpZoomChange(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: get zoom range
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_getZoomRange(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_zoomrange* range);
XCamReturn rk_aiq_uapi2_getFocusRange(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_focusrange* range);

/*
*****************************
*
* Desc: zoom calibration
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_startZoomCalib(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi2_resetZoom(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set AngleZ
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setAngleZ(const rk_aiq_sys_ctx_t* ctx, float angleZ);

/*
**********************************************************
* Color Correction
**********************************************************
*/
/*
*****************************
*
* Desc: set/get color correction mode
* Argument:
*   mode:
*     auto: auto color correction
*     manual: manual color correction
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setCCMMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi2_getCCMMode(const rk_aiq_sys_ctx_t* ctx, opMode_t* mode);

/*
*****************************
*
* Desc: set manual color correction matrix
*     this function is active for color correction is manual mode
        get color correction matrix
* Argument:
* mccm:
*                   3x3 matrix
*                   1x3 offset
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setMCcCoef(const rk_aiq_sys_ctx_t* ctx, rk_aiq_ccm_matrix_t* mccm);
XCamReturn rk_aiq_uapi2_getMCcCoef(const rk_aiq_sys_ctx_t* ctx, rk_aiq_ccm_matrix_t* mccm);

/*
*****************************
*
* Desc: set/get auto color correction saturation
*     this function is active for color correction is auto mode
* Argument:
*   finalsat : range in [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_getACcmSat(const rk_aiq_sys_ctx_t* ctx, float* finalsat);

/*
*****************************
*
* Desc: get auto color correction used ccm name
*     this function is active for color correction is auto mode
* Argument:
*    ccm_name[2]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_getACcmMatrixName(const rk_aiq_sys_ctx_t* ctx, char** ccm_name);

/*
**********************************************************
* 3-Dimensional Look Up Table
**********************************************************
*/
/*
*****************************
*
* Desc: set/get 3dlut mode
* Argument:
*   mode:
*     auto: auto 3dlut
*     manual: manual 3dlut
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setLut3dMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi2_getLut3dMode(const rk_aiq_sys_ctx_t* ctx, opMode_t* mode);

/*
*****************************
*
* Desc: set manual 3d Look-up-table
*     this function is active for 3dlut is manual mode
        get 3d Look-up-table
* Argument:
*     mlut:
*           lut_r[729]
*           lut_g[729]
*           lut_b[729]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_setM3dLut(const rk_aiq_sys_ctx_t* ctx, rk_aiq_lut3d_table_t* mlut);
XCamReturn rk_aiq_uapi2_getM3dLut(const rk_aiq_sys_ctx_t* ctx, rk_aiq_lut3d_table_t* mlut);

/*
*****************************
*
* Desc: set/get auto 3d Look-up-table strength
*     this function is active for 3d Look-up-table is auto mode
* Argument:
*   alpha : range in [0, 1]
*
*****************************
*/
XCamReturn rk_aiq_uapi2_getA3dLutStrth(const rk_aiq_sys_ctx_t* ctx, float* alpha);

/*
*****************************
*
* Desc: get auto 3d Look-up-table used lut name
*     this function is active for 3d Look-up-table is auto mode
* Argument:
*    name
*
*****************************
*/
XCamReturn rk_aiq_uapi2_getA3dLutName(const rk_aiq_sys_ctx_t* ctx, char* name);

/*
*****************************
*
* Desc:
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setLdchEn(const rk_aiq_sys_ctx_t* ctx, bool en);
/*
*****************************
*
* Desc: the adjustment range of distortion intensity is 0~255
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setLdchCorrectLevel(const rk_aiq_sys_ctx_t* ctx, int correctLevel);

/*
*****************************
*
* Desc: fec dynamic switch, valid only if aiq hasn't executed the 'prepare' action
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setFecEn(const rk_aiq_sys_ctx_t* ctx, bool en);

/*
*****************************
*
* Desc: set corrective direction of FEC, valid only if aiq hasn't executed the 'prepare' action
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setFecCorrectDirection(const rk_aiq_sys_ctx_t* ctx,
        const fec_correct_direction_t direction);
/*
*****************************
*
* Desc: The FEC module is still working in bypass state
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setFecBypass(const rk_aiq_sys_ctx_t* ctx, bool en);

/*
*****************************
*
* Desc: the adjustment range of distortion intensity is 0~255
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setFecCorrectLevel(const rk_aiq_sys_ctx_t* ctx, int correctLevel);

/*
*****************************
*
* Desc:
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setFecCorrectMode(const rk_aiq_sys_ctx_t* ctx,
        const fec_correct_mode_t mode);

/*
*****************************
*
* Desc: Set Sensor output HFLIP and VFLIP
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_setMirrorFlip(const rk_aiq_sys_ctx_t* ctx, bool mirror, bool flip,
                                      int skip_frm_cnt);

/*
*****************************
*
* Desc: Get Sensor output HFLIP and VFLIP
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi2_getMirrorFlip(const rk_aiq_sys_ctx_t* ctx, bool* mirror, bool* flip);

/*
**********************************************************
* Color Process
**********************************************************
*/

/*
*****************************
*
* Desc: Adjust image contrast level
* Argument:
*    level: contrast level, [0, 255]
*****************************
*/
XCamReturn rk_aiq_uapi2_setContrast(const rk_aiq_sys_ctx_t* ctx, unsigned int level);

XCamReturn rk_aiq_uapi2_getContrast(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: Adjust image brightness level
* Argument:
*    level: brightness level, [0, 255]
*****************************
*/
XCamReturn rk_aiq_uapi2_setBrightness(const rk_aiq_sys_ctx_t* ctx, unsigned int level);

XCamReturn rk_aiq_uapi2_getBrightness(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: Adjust image saturation level
* Argument:
*    level: saturation level, [0, 255]
*****************************
*/
XCamReturn rk_aiq_uapi2_setSaturation(const rk_aiq_sys_ctx_t* ctx, unsigned int level);

XCamReturn rk_aiq_uapi2_getSaturation(const rk_aiq_sys_ctx_t* ctx, unsigned int* level);

/*
*****************************
*
* Desc: Adjust image hue level
* Argument:
*    level: hue level, [0, 255]
*****************************
*/
XCamReturn rk_aiq_uapi2_setHue(const rk_aiq_sys_ctx_t* ctx, unsigned int level);

XCamReturn rk_aiq_uapi2_getHue(const rk_aiq_sys_ctx_t* ctx, unsigned int* level);

/*
**********************************************************
* Image Effect
**********************************************************
*/

/*
*****************************
*
* Desc: Set Color Mode
* Argument:
*    mode:
*        [0  NONE
*         1  BW
*         2  NEGATIVE
*         3  SEPIA
*         4  EMBOSS
*         5  SKETCH
*         6  SHARPEN]
* Note: rv1106 only support {0,1}
*****************************
*/
XCamReturn rk_aiq_uapi2_setColorMode(const rk_aiq_sys_ctx_t* ctx, unsigned int mode);

XCamReturn rk_aiq_uapi2_getColorMode(const rk_aiq_sys_ctx_t* ctx, unsigned int *mode);
RKAIQ_END_DECLARE

#endif
