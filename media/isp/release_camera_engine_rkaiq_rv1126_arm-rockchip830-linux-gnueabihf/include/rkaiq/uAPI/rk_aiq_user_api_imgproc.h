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

#ifndef _RK_AIQ_UAPI_IMGPROC_H_
#define _RK_AIQ_UAPI_IMGPROC_H_

#include "base/xcam_common.h"
#include "rk_aiq_user_api_sysctl.h"
#include "rk_aiq_user_api_awb.h"
#include "rk_aiq_user_api_ae.h"
#include "rk_aiq_user_api_af.h"
#include "rk_aiq_user_api_anr.h"
#include "rk_aiq_user_api_ahdr.h"
#include "rk_aiq_user_api_adehaze.h"
#include "rk_aiq_user_api_alsc.h"
#include "rk_aiq_user_api_accm.h"
#include "rk_aiq_user_api_a3dlut.h"
#include "rk_aiq_user_api_asharp.h"
#include "rk_aiq_user_api_agamma.h"
#include "rk_aiq_user_api_afec.h"
#include "rk_aiq_user_api_aldch.h"
#include "rk_aiq_user_api_acp.h"

/*
*****************************
* Common
*****************************
*/

typedef enum dayNightScene_e {
    DAYNIGHT_SCENE_DAY   = 0,
    DAYNIGHT_SCENE_NIGHT = 1,
    DAYNIGHT_SCENE_INVAL,
} dayNightScene_t;

typedef enum opMode_e {
    OP_AUTO   = 0,
    OP_MANUAL = 1,
    OP_SEMI_AUTO = 2,
    OP_INVAL
} opMode_t;


typedef struct paRange_s {
    float max;
    float min;
} paRange_t;

typedef enum awbRange_e {
    AWB_RANGE_0 = 0,
    AWB_RANGE_1 = 1,
    AWB_RANGE_INVAL,
} awbRange_t;

typedef enum aeMode_e {
    AE_AUTO          = 0,
    AE_IRIS_PRIOR    = 1,
    AE_SHUTTER_PRIOR = 2,
} aeMode_t;


/*
*****************
*   (-1000, -1000)    (1000, -1000)
*   -------------------
*   |                 |
*   |                 |
*   |       (0,0)     |
*   |                 |
*   |                 |
*   -------------------
*                     (1000, 1000)
*****************
*/
typedef struct paRect_s {
    int x;
    int y;
    unsigned int w;
    unsigned int h;
} paRect_t;

typedef enum aeMeasAreaType_e {
    AE_MEAS_AREA_AUTO = 0,
    AE_MEAS_AREA_UP,
    AE_MEAS_AREA_BOTTOM,
    AE_MEAS_AREA_LEFT,
    AE_MEAS_AREA_RIGHT,
    AE_MEAS_AREA_CENTER,
} aeMeasAreaType_t;

typedef enum expPwrLineFreq_e {
    EXP_PWR_LINE_FREQ_DIS   = 0,
    EXP_PWR_LINE_FREQ_50HZ  = 1,
    EXP_PWR_LINE_FREQ_60HZ  = 2,
} expPwrLineFreq_t;

typedef enum antiFlickerMode_e {
    ANTIFLICKER_NORMAL_MODE = 0,
    ANTIFLICKER_AUTO_MODE   = 1,
} antiFlickerMode_t;

typedef struct frameRateInfo_s {
    opMode_t         mode;
    unsigned int     fps; /* valid when manual mode*/
} frameRateInfo_t;

RKAIQ_BEGIN_DECLARE

/*
**********************************************************
* Exposure
**********************************************************
*/

/*
*****************************
*
* Desc: set exposure control mode
* Argument:
*   mode:  auto: auto exposure
*          manual: manual exposure
*****************************
*/
XCamReturn rk_aiq_uapi_setExpMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi_getExpMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set exposure mode
* Argument:
*   mode: if mode is set to manual, gain&time initial value will be used
*
*****************************
*/
XCamReturn rk_aiq_uapi_setAeMode(const rk_aiq_sys_ctx_t* ctx, aeMode_t mode);
XCamReturn rk_aiq_uapi_getAeMode(const rk_aiq_sys_ctx_t* ctx, aeMode_t *mode);
/*
*****************************
*
* Desc: set manual exposure
* Argument:
*   gain:
*   time:
*
*****************************
*/
XCamReturn rk_aiq_uapi_setManualExp(const rk_aiq_sys_ctx_t* ctx, float gain, float time);
/*
*****************************
*
* Desc: set exposure parameter
* Argument:
*    auto exposure mode:
*      exposure gain will be adjust between [gain->min, gain->max]；
*    manual exposure mode:
*      gain->min == gain->max
*
*****************************
*/
XCamReturn rk_aiq_uapi_setExpGainRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *gain);
XCamReturn rk_aiq_uapi_getExpGainRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *gain);
/*
*****************************
*
* Desc: set exposure parameter
* Argument:
*    auto exposure mode:
*       exposure time will be adjust between [time->min, time->max]；
*    manual exposure mode:
*       exposure time will be set gain->min == gain->max;
*
*****************************
*/
XCamReturn rk_aiq_uapi_setExpTimeRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *time);
XCamReturn rk_aiq_uapi_getExpTimeRange(const rk_aiq_sys_ctx_t* ctx, paRange_t *time);

/*
**********************************************************
* Auto exposure advanced features
**********************************************************
*/
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
XCamReturn rk_aiq_uapi_setBLCMode(const rk_aiq_sys_ctx_t* ctx, bool on, aeMeasAreaType_t areaType);

/*
*****************************
*
* Desc: backlight compensation strength,only available in normal mode
* Argument:
*      strength:  [1,100]
*****************************
*/
XCamReturn rk_aiq_uapi_setBLCStrength(const rk_aiq_sys_ctx_t* ctx, int strength);

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
XCamReturn rk_aiq_uapi_setHLCMode(const rk_aiq_sys_ctx_t* ctx, bool on);

/*
*****************************
*
* Desc: highlight compensation strength,only available in normal mode
* Argument:
*      strength:  [1,100]
*****************************
*/
XCamReturn rk_aiq_uapi_setHLCStrength(const rk_aiq_sys_ctx_t* ctx, int strength);

/*
*****************************
*
* Desc: set anti-flicker mode
* Argument:
*    mode
*
*****************************
*/
XCamReturn rk_aiq_uapi_setAntiFlickerMode(const rk_aiq_sys_ctx_t* ctx, antiFlickerMode_t mode);
XCamReturn rk_aiq_uapi_getAntiFlickerMode(const rk_aiq_sys_ctx_t* ctx, antiFlickerMode_t *mode);

/*
*****************************
*
* Desc: set power line frequence
* Argument:
*    freq
*
*****************************
*/
XCamReturn rk_aiq_uapi_setExpPwrLineFreqMode(const rk_aiq_sys_ctx_t* ctx, expPwrLineFreq_t freq);
XCamReturn rk_aiq_uapi_getExpPwrLineFreqMode(const rk_aiq_sys_ctx_t* ctx, expPwrLineFreq_t *freq);

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
XCamReturn rk_aiq_uapi_setWBMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi_getWBMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);


/*
*****************************
*
* Desc: lock/unlock auto white balance
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_lockAWB(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi_unlockAWB(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set manual white balance scene mode
* Argument:
*   ct_scene:
*
*****************************
*/
XCamReturn rk_aiq_uapi_setMWBScene(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_scene_t scene);
XCamReturn rk_aiq_uapi_getMWBScene(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_scene_t *scene);


/*
*****************************
*
* Desc: set manual white balance r/b gain
* Argument:
*   ct_scene:
*
*****************************
*/
XCamReturn rk_aiq_uapi_setMWBGain(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_gain_t *gain);
XCamReturn rk_aiq_uapi_getMWBGain(const rk_aiq_sys_ctx_t* ctx, rk_aiq_wb_gain_t *gain);

/*
*****************************
*
* Desc: set manual white balance color temperature
* Argument:
*   ct: color temperature value [2800, 7500]K
*
*****************************
*/
XCamReturn rk_aiq_uapi_setMWBCT(const rk_aiq_sys_ctx_t* ctx, unsigned int ct);
XCamReturn rk_aiq_uapi_getMWBCT(const rk_aiq_sys_ctx_t* ctx, unsigned int *ct);

/*
*****************************
*
* Desc: set lut3d adjust strength
* Argument:
*  alpha: [0, 1]
*
*****************************
*/

XCamReturn rk_aiq_uapi_setLut3dstrth(const rk_aiq_sys_ctx_t* ctx, float alpha);

/*
*****************************
*
* Desc: set color supperssion level
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi_setCrSuppsn(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi_getCrSuppsn(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

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
XCamReturn rk_aiq_uapi_setFocusMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi_getFocusMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set focus window
* Argument:
*
*****************************
*/
XCamReturn rk_aiq_uapi_setFocusWin(const rk_aiq_sys_ctx_t* ctx, paRect_t *rect);
XCamReturn rk_aiq_uapi_getFocusWin(const rk_aiq_sys_ctx_t* ctx, paRect_t *rect);

/*
*****************************
*
* Desc: set focus meas config
* Argument:
*
*****************************
*/
XCamReturn rk_aiq_uapi_setFocusMeasCfg(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_algo_meas_t* meascfg);
XCamReturn rk_aiq_uapi_getFocusMeasCfg(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_algo_meas_t* meascfg);

/*
*****************************
*
* Desc: set fix mode code
* Argument:
*
*****************************
*/
XCamReturn rk_aiq_uapi_setFixedModeCode(const rk_aiq_sys_ctx_t* ctx, short code);
XCamReturn rk_aiq_uapi_getFixedModeCode(const rk_aiq_sys_ctx_t* ctx, short *code);

/*
*****************************
*
* Desc: lock/unlock auto focus
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_lockFocus(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi_unlockFocus(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: oneshot focus
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_oneshotFocus(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi_trackingFocus(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: ManualTriger focus
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_manualTrigerFocus(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: vcm config
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_setVcmCfg(const rk_aiq_sys_ctx_t* ctx, rk_aiq_lens_vcmcfg* cfg);
XCamReturn rk_aiq_uapi_getVcmCfg(const rk_aiq_sys_ctx_t* ctx, rk_aiq_lens_vcmcfg* cfg);

/*
*****************************
*
* Desc: af serach path record
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_getSearchPath(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_sec_path_t* path);

/*
*****************************
*
* Desc: af serach path record
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_getSearchResult(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_result_t* result);

/*
*****************************
*
* Desc: Focus Correstion
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_FocusCorrestion(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: Zoom
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_getZoomRange(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_zoomrange* range);
XCamReturn rk_aiq_uapi_getFocusRange(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_focusrange* range);
XCamReturn rk_aiq_uapi_setOpZoomPosition(const rk_aiq_sys_ctx_t* ctx, int pos);
XCamReturn rk_aiq_uapi_getOpZoomPosition(const rk_aiq_sys_ctx_t* ctx, int *pos);
XCamReturn rk_aiq_uapi_endOpZoomChange(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi_startZoomCalib(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi_getZoomCalibStat(const rk_aiq_sys_ctx_t* ctx, rk_aiq_af_zoomcalib_stat_t* stat);
XCamReturn rk_aiq_uapi_resetZoom(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: Zoom Correstion
* Argument:
*
*
*****************************
*/
XCamReturn rk_aiq_uapi_ZoomCorrestion(const rk_aiq_sys_ctx_t* ctx);

/*
**********************************************************
* HDR
**********************************************************
*/
/*
*****************************
*
* Desc: set manual hdr strength
*    this function is active for HDR is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi_setMHDRStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn rk_aiq_uapi_getMHDRStrth(const rk_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
*****************************
*
* Desc: set/get dark area boost strength
*    this function is active for normal mode
* Argument:
*   level: [1, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi_setDarkAreaBoostStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi_getDarkAreaBoostStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);


/*
**********************************************************
* Noise reduction
**********************************************************
*/
/*
*****************************
*
* Desc: set/get noise reduction mode
* Argument:
*   mode:
*     auto: auto noise reduction
*     manual：manual noise reduction
*
*****************************
*/
XCamReturn rk_aiq_uapi_setNRMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi_getNRMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set/get normal noise reduction strength
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi_setANRStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi_getANRStrth(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

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
XCamReturn rk_aiq_uapi_setMSpaNRStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn rk_aiq_uapi_getMSpaNRStrth(const rk_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
*****************************
*
* Desc: set/get manual time noise reduction strength
*     this function is active for NR is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn rk_aiq_uapi_setMTNRStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn rk_aiq_uapi_getMTNRStrth(const rk_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
**********************************************************
* Dehaze
**********************************************************
*/
/*
*****************************
*
* Desc: set/get dehaze mode
* Argument:
*   mode:
*     auto: auto dehaze, when use auto, equal use rk_aiq_uapi_enableDhz
*     manual: Manual dehaze, when needs to use manual, please use rk_aiq_uapi_setMDhzStrth
*
*****************************
*/
XCamReturn rk_aiq_uapi_setDhzMode(const rk_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn rk_aiq_uapi_getDhzMode(const rk_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set/get manual dehaze strength
*     this function is active for dehaze is manual mode
* Argument:
*   level: [0, 100]
*   need to use rk_aiq_uapi_SetDhzManualMode before use this
*
*****************************
*/
XCamReturn rk_aiq_uapi_setMDhzStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn rk_aiq_uapi_getMDhzStrth(const rk_aiq_sys_ctx_t* ctx, bool* on, unsigned int* level);

/*
*****************************
*
* Desc: enable dehaze
* Argument:
*   When dehaze enable, dehaze on and para use use IQ xml
*   When dehaze disable, dehaze off and enhance para use use IQ xml
*
*****************************
*/
XCamReturn rk_aiq_uapi_enableDhz(const rk_aiq_sys_ctx_t* ctx);
XCamReturn rk_aiq_uapi_disableDhz(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set/get manual enhance strength
*     this function is active for enhance is manual mode
* Argument:
*   level: [0, 100]
*   need to use rk_aiq_uapi_SetDhzManualMode before use this
*
*****************************
*/
XCamReturn rk_aiq_uapi_setMEnhanceStrth(const rk_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn rk_aiq_uapi_getMEnhanceStrth(const rk_aiq_sys_ctx_t* ctx, bool* on, unsigned int* level);

//use api before rk_aiq_uapi_setMDhzStrth and rk_aiq_uapi_setMDhzStrth
XCamReturn rk_aiq_uapi_SetDhzManualMode(const rk_aiq_sys_ctx_t* ctx, rk_aiq_dehaze_op_mode_t mode);

/*
**********************************************************
* Image adjust
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
XCamReturn rk_aiq_uapi_setContrast(const rk_aiq_sys_ctx_t* ctx, unsigned int level);

/*
*****************************
*
* Desc: Adjust image brightness level
* Argument:
*    level: brightness level, [0, 255]
*****************************
*/
XCamReturn rk_aiq_uapi_setBrightness(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi_getBrightness(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);
/*
*****************************
*
* Desc: Adjust image saturation level
* Argument:
*    level: saturation level, [0, 255]
*****************************
*/
XCamReturn rk_aiq_uapi_setSaturation(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi_getSaturation(const rk_aiq_sys_ctx_t* ctx, unsigned int* level);
/*
*****************************
*
* Desc: Adjust image hue level
* Argument:
*    level: hue level, [0, 255]
*****************************
*/
XCamReturn rk_aiq_uapi_setHue(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi_getHue(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);
/*
*****************************
*
* Desc: Adjust image sharpness level
* Argument:
*    level: sharpness level, [0, 100]
*****************************
*/
XCamReturn rk_aiq_uapi_setSharpness(const rk_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn rk_aiq_uapi_getSharpness(const rk_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: Adjust image gamma coef
* Argument:
*    Gammacoef:  (0, 10]
*    GammaOfst:  [-0.05, 0.05]
*****************************
*/
XCamReturn rk_aiq_uapi_setGammaCoef(const rk_aiq_sys_ctx_t* ctx, float GammaCoef, float GammaOfst);
XCamReturn rk_aiq_uapi_getGammaCoef(const rk_aiq_sys_ctx_t* ctx, float* GammaCoef, float* GammaOfst);

/*
*****************************
*
* Desc: Adjust image gamma coef
* Argument:
*    GammaTable:  45 dot gamma curve
*****************************
*/
XCamReturn rk_aiq_uapi_setGammaCurve(const rk_aiq_sys_ctx_t* ctx, int* GammaTable);
XCamReturn rk_aiq_uapi_getGammaCurve(const rk_aiq_sys_ctx_t* ctx, int* GammaTable);



/*
*****************************
*
* Desc: set gray mode
* Argument:
*    mode : RK_AIQ_GRAY_MODE_CPSL
*           RK_AIQ_GRAY_MODE_OFF
*           RK_AIQ_GRAY_MODE_ON
* related: rk_aiq_uapi_sysctl_setCpsLtCfg
*****************************
*/
XCamReturn rk_aiq_uapi_setGrayMode(const rk_aiq_sys_ctx_t* ctx, rk_aiq_gray_mode_t mode);
rk_aiq_gray_mode_t rk_aiq_uapi_getGrayMode(const rk_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set frame rate
* Argument:
*    frameRateInfo_t :
*           auto mode: vary fps
*         manual mode: fixed fps
*****************************
*/
XCamReturn rk_aiq_uapi_setFrameRate(const rk_aiq_sys_ctx_t* ctx, frameRateInfo_t info);
XCamReturn rk_aiq_uapi_getFrameRate(const rk_aiq_sys_ctx_t* ctx, frameRateInfo_t *info);

/*
*****************************
*
* Desc: set mirro & flip
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setMirroFlip(const rk_aiq_sys_ctx_t* ctx,
                                    bool mirror,
                                    bool flip,
                                    int skip_frm_cnt);

/*
*****************************
*
* Desc: get mirro & flip
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_getMirrorFlip(const rk_aiq_sys_ctx_t* ctx, bool* mirror, bool* flip);

/*
*****************************
*
* Desc: fec dynamic switch, valid only if aiq hasn't executed the 'prepare' action
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setFecEn(const rk_aiq_sys_ctx_t* ctx, bool en);

/*
*****************************
*
* Desc: set corrective direction of FEC, valid only if aiq hasn't executed the 'prepare' action
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setFecCorrectDirection(const rk_aiq_sys_ctx_t* ctx,
        const fec_correct_direction_t direction);
/*
*****************************
*
* Desc: The FEC module is still working in bypass state
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setFecBypass(const rk_aiq_sys_ctx_t* ctx, bool en);

/*
*****************************
*
* Desc: the adjustment range of distortion intensity is 0~255
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setFecCorrectLevel(const rk_aiq_sys_ctx_t* ctx, int correctLevel);

/*
*****************************
*
* Desc:
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setFecCorrectMode(const rk_aiq_sys_ctx_t* ctx,
        const fec_correct_mode_t mode);

/*
*****************************
*
* Desc:
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setLdchEn(const rk_aiq_sys_ctx_t* ctx, bool en);
/*
*****************************
*
* Desc: the adjustment range of distortion intensity is 0~255
* Argument:
*****************************
*/
XCamReturn rk_aiq_uapi_setLdchCorrectLevel(const rk_aiq_sys_ctx_t* ctx, int correctLevel);

RKAIQ_END_DECLARE

#endif
