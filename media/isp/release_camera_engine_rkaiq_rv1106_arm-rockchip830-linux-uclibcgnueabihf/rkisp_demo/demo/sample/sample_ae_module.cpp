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

#include "sample_comm.h"

static void sample_ae_usage()
{
    printf("Usage : \n");
    printf("\t 0) AE:         set ae speed.\n");
    printf("\t 1) AE:         set ae delay.\n\n");
    printf("\t 2) AE:         set exp manual.\n");
    printf("\t 3) AE:         set exp auto.\n");
    printf("\t 4) AE:         set manual time & auto gain.\n");
    printf("\t 5) AE:         set auto time & manual gain.\n");
    printf("\t 6) AE:         set time range.\n");
    printf("\t 7) AE:         get time range.\n");
    printf("\t 8) AE:         set gain range.\n");
    printf("\t 9) AE:         get gain range.\n");
    printf("\t a) AE:         set fix fps.\n");
    printf("\t b) AE:         set auto fps.\n");
    printf("\t c) AE:         set anti_flicker frequency 50HZ.\n");
    printf("\t d) AE:         set anti_flicker frequency 60HZ.\n");
    printf("\t e) AE:         set anti_flicker disable.\n");
    printf("\t f) AE:         set grid_weight directly.\n");
    printf("\t g) AE:         set grid_weight by advanced.\n");
    printf("\t h) AE:         set ae on(= unlock ae).\n");
    printf("\t i) AE:         set ae off(= lock ae).\n");
    printf("\t j) AE:         set exp windows.\n");
    printf("\t k) AE:         set back_light_ctrl.\n");
    printf("\t l) AE:         set over_exp_ctrl.\n");
    printf("\t m) AE:         set hdr exp ratio.\n");
    printf("\t n) AE:         set linear route.\n");
    printf("\t o) AE:         set hdr mframe params.\n");

    printf("\t W) AE:         test default mode.\n");
    printf("\t X) AE:         test sync mode.\n");
    printf("\t Y) AE:         test async mode.\n");
    printf("\t Z) AE:         test async mode, twice set.\n\n");

    printf("\t q/Q) AE:         return to main sample screen.\n");
    printf("\n");
    printf("\t please press the key: \n\n");

    return;
}

void sample_print_ae_info(const void *arg)
{
    printf ("enter AE modult test!\n");
}

static int sample_set_ae_onoff(const rk_aiq_sys_ctx_t* ctx, bool on)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.Enable = on;
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_exp_manual(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    expSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
    //LinearAE
    expSwAttr.stManual.LinearAE.ManualGainEn = true;
    expSwAttr.stManual.LinearAE.ManualTimeEn = true;
    expSwAttr.stManual.LinearAE.GainValue = 1.0f; /*gain = 1x*/
    expSwAttr.stManual.LinearAE.TimeValue = 0.02f; /*time = 1/50s*/
    //HdrAE (should set all frames)
    expSwAttr.stManual.HdrAE.ManualGainEn = true;
    expSwAttr.stManual.HdrAE.ManualTimeEn = true;
    expSwAttr.stManual.HdrAE.GainValue[0] = 1.0f; /*sframe gain = 1x*/
    expSwAttr.stManual.HdrAE.TimeValue[0] = 0.002f; /*sframe time = 1/500s*/
    expSwAttr.stManual.HdrAE.GainValue[1] = 2.0f; /*mframe gain = 2x*/
    expSwAttr.stManual.HdrAE.TimeValue[1] = 0.01f; /*mframe time = 1/100s*/
    expSwAttr.stManual.HdrAE.GainValue[2] = 4.0f; /*lframe gain = 4x*/
    expSwAttr.stManual.HdrAE.TimeValue[2] = 0.02f; /*lframe time = 1/50s*/
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_exp_manual_test(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    expSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
    //LinearAE
    expSwAttr.stManual.LinearAE.ManualGainEn = true;
    expSwAttr.stManual.LinearAE.ManualTimeEn = true;
    expSwAttr.stManual.LinearAE.GainValue = 1.3f; /*gain = 1.3x*/
    expSwAttr.stManual.LinearAE.TimeValue = 0.03f; /*time =0.030s*/
    //HdrAE (should set all frames)
    expSwAttr.stManual.HdrAE.ManualGainEn = true;
    expSwAttr.stManual.HdrAE.ManualTimeEn = true;
    expSwAttr.stManual.HdrAE.GainValue[0] = 1.1f; /*sframe gain = 1x*/
    expSwAttr.stManual.HdrAE.TimeValue[0] = 0.003f; /*sframe time = 1/500s*/
    expSwAttr.stManual.HdrAE.GainValue[1] = 2.1f; /*mframe gain = 2x*/
    expSwAttr.stManual.HdrAE.TimeValue[1] = 0.02f; /*mframe time = 1/100s*/
    expSwAttr.stManual.HdrAE.GainValue[2] = 4.1f; /*lframe gain = 4x*/
    expSwAttr.stManual.HdrAE.TimeValue[2] = 0.03f; /*lframe time = 1/50s*/
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_exp_auto(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    expSwAttr.AecOpType = RK_AIQ_OP_MODE_AUTO;
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_time_manual_and_gain_auto(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    expSwAttr.AecOpType = RK_AIQ_OP_MODE_AUTO;
    //set time range
    expSwAttr.stAdvanced.SetAeRangeEn = true;/*must enable*/
    //LinAE
    expSwAttr.stAdvanced.SetLinAeRange.stExpTimeRange.Max = 0.03f; /*time_max = 0.03*/
    expSwAttr.stAdvanced.SetLinAeRange.stExpTimeRange.Min = 0.03f; /*time_min = 0.03*/
    //HdrAE (can only set one frame, stime + mtime + ltime <= timeMax)
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[0].Max = 0.01f; /*sframe time_max = 0.01*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[0].Min = 0.01f; /*sframe time_min = 0.01*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[1].Max = 0.01f; /*mframe time_max = 0.01*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[1].Min = 0.01f; /*mframe time_min = 0.01*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[2].Max = 0.01f; /*lframe time_max = 0.01*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[2].Min = 0.01f; /*lframe time_min = 0.01*/
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}


static int sample_set_time_auto_and_gain_manual(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    expSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
    //LinearAE
    expSwAttr.stManual.LinearAE.ManualGainEn = true;
    expSwAttr.stManual.LinearAE.ManualTimeEn = false;
    expSwAttr.stManual.LinearAE.ManualIspDgainEn = false;
    expSwAttr.stManual.LinearAE.GainValue = 2.0f; /*gain = 2x*/
    //HdrAE (can only set one frame)
    expSwAttr.stManual.HdrAE.ManualGainEn = true;
    expSwAttr.stManual.HdrAE.ManualTimeEn = false;
    expSwAttr.stManual.HdrAE.ManualIspDgainEn = false;
    expSwAttr.stManual.HdrAE.GainValue[0] = 1.0f; /*sframe gain = 1x*/
    expSwAttr.stManual.HdrAE.GainValue[1] = 2.0f; /*mframe gain = 2x*/
    expSwAttr.stManual.HdrAE.GainValue[2] = 4.0f; /*lframe gain = 4x*/
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_time_range(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    //set time range in struct "stAdvanced"
    expSwAttr.stAdvanced.SetAeRangeEn = true;/*must enable*/
    //LinAE
    expSwAttr.stAdvanced.SetLinAeRange.stExpTimeRange.Max = 0.04f; /*time_max = 0.04*/
    expSwAttr.stAdvanced.SetLinAeRange.stExpTimeRange.Min = 0.001f; /*time_min = 0.001*/
    //HdrAE
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[0].Max = 0.002f; /*sframe time_max = 0.02*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[0].Min = 0.001f; /*sframe time_min = 0.01*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[1].Max = 0.003f; /*mframe time_max = 0.03*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[1].Min = 0.002f; /*mframe time_min = 0.02*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[2].Max = 0.04f; /*lframe time_max = 0.03*/
    expSwAttr.stAdvanced.SetHdrAeRange.stExpTimeRange[2].Min = 0.03f; /*lframe time_min = 0.02*/
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_get_time_range(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
    //get time range in struct "stAuto"
    printf("linear time range=[%f,%f]\n",
           expSwAttr.stAuto.LinAeRange.stExpTimeRange.Min, expSwAttr.stAuto.LinAeRange.stExpTimeRange.Max);
    printf("hdr stime range=[%f,%f], mtime range=[%f,%f], ltime range=[%f,%f]\n",
           expSwAttr.stAuto.HdrAeRange.stExpTimeRange[0].Min, expSwAttr.stAuto.HdrAeRange.stExpTimeRange[0].Max,
           expSwAttr.stAuto.HdrAeRange.stExpTimeRange[1].Min, expSwAttr.stAuto.HdrAeRange.stExpTimeRange[1].Max,
           expSwAttr.stAuto.HdrAeRange.stExpTimeRange[2].Min, expSwAttr.stAuto.HdrAeRange.stExpTimeRange[2].Max);

    return 0;
}

static int sample_set_gain_range(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    //set gain range in struct "stAdvanced"
    expSwAttr.stAdvanced.SetAeRangeEn = true;/*must enable*/
    //LinAE
    expSwAttr.stAdvanced.SetLinAeRange.stGainRange.Max = 32.0f; /*gain_max = 32x*/
    expSwAttr.stAdvanced.SetLinAeRange.stGainRange.Min = 1.0f; /*gain_min = 1x*/
    //HdrAE
    expSwAttr.stAdvanced.SetHdrAeRange.stGainRange[0].Max = 32.0f; /*sframe gain_max = 2x*/
    expSwAttr.stAdvanced.SetHdrAeRange.stGainRange[0].Min = 1.0f; /*sframe gain_min = 1x*/
    expSwAttr.stAdvanced.SetHdrAeRange.stGainRange[1].Max = 64.0f; /*mframe gain_max = 64x*/
    expSwAttr.stAdvanced.SetHdrAeRange.stGainRange[1].Min = 1.0f; /*mframe gian_min = 1x*/
    expSwAttr.stAdvanced.SetHdrAeRange.stGainRange[2].Max = 64.0f; /*lframe gain_max = 64x*/
    expSwAttr.stAdvanced.SetHdrAeRange.stGainRange[2].Min = 1.0f; /*lframe gain_min = 1x*/
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_get_gain_range(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
    //get gain range in struct "stAuto"
    printf("linear gain range=[%f,%f]\n",
           expSwAttr.stAuto.LinAeRange.stGainRange.Min, expSwAttr.stAuto.LinAeRange.stGainRange.Max);
    printf("hdr sgain range=[%f,%f], mgain range=[%f,%f], lgain range=[%f,%f]\n",
           expSwAttr.stAuto.HdrAeRange.stGainRange[0].Min, expSwAttr.stAuto.HdrAeRange.stGainRange[0].Max,
           expSwAttr.stAuto.HdrAeRange.stGainRange[1].Min, expSwAttr.stAuto.HdrAeRange.stGainRange[1].Max,
           expSwAttr.stAuto.HdrAeRange.stGainRange[2].Min, expSwAttr.stAuto.HdrAeRange.stGainRange[2].Max);

    return 0;
}

static int sample_is_fps_fix(const rk_aiq_sys_ctx_t* ctx, bool on, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    expSwAttr.stAuto.stFrmRate.isFpsFix = on;
    expSwAttr.stAuto.stFrmRate.FpsValue = 25; /*fps = 25*/
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_anti_flicker(const rk_aiq_sys_ctx_t* ctx,
                                   bool on,
                                   CalibDb_FlickerFreqV2_t frequency,
                                   CalibDb_AntiFlickerModeV2_t mode,
                                   rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    //set antifilicker mode
    expSwAttr.stAuto.stAntiFlicker.enable = on;
    expSwAttr.stAuto.stAntiFlicker.Frequency = frequency;
    expSwAttr.stAuto.stAntiFlicker.Mode = mode;
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_gridweight(const rk_aiq_sys_ctx_t* ctx, int type, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t expSwAttr;
    uint8_t GridWeights[225] = {
        0,  0, 1,  2,  2,  3,  4,  5,  4, 3,  2,  2,  1,  0,  0,
        0,  1, 2,  3,  3,  4,  5,  6,  5, 4,  3,  3,  2,  1,  0,
        1,  2, 3,  5,  5,  6,  7,  8,  7, 6,  5,  5,  3,  2,  1,
        2,  3, 5,  7,  7,  8,  9, 10,  9, 8,  7,  7,  5,  3,  2,
        2,  3, 5,  7,  8,  9, 10, 11, 10, 9,  8,  7,  5,  3,  2,
        2,  4, 6,  8,  9, 10, 11, 12, 11, 10,  9,  8,  6,  4,  2,
        2,  4, 6,  9, 10, 11, 12, 13, 12, 11, 10,  9,  6,  4,  2,
        3,  5, 7, 10, 11, 12, 13, 14, 13, 12, 11, 10,  7,  5,  3,
        2,  4, 6,  9, 10, 11, 12, 13, 12, 11, 10,  9,  6,  4,  2,
        2,  4, 6,  8,  9, 10, 11, 12, 11, 10,  9,  8,  6,  4,  2,
        2,  3, 5,  7,  8,  9, 10, 11, 10, 9,  8,  7,  5,  3,  2,
        2,  3, 5,  7,  7,  8,  9, 10,  9, 8,  7,  7,  5,  3,  2,
        1,  2, 4,  6,  6,  7,  8,  9,  8, 7,  6,  6,  4,  2,  1,
        0,  1, 3,  5,  5,  6,  7,  8,  7, 6,  5,  5,  3,  1,  0,
        0,  1, 3,  5,  5,  6,  7,  8,  7, 6,  5,  5,  3,  1,  0
    };

    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    if (type == 0)
        memcpy(expSwAttr.GridWeights.uCoeff, GridWeights, sizeof(expSwAttr.GridWeights.uCoeff));
    else if (type == 1) {
        expSwAttr.stAdvanced.enable = true; //important! true means preferring to use these parameters
        memcpy(expSwAttr.stAdvanced.GridWeights, GridWeights, sizeof(expSwAttr.stAdvanced.GridWeights));
    }
    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int sample_set_BackLightCtrl(const rk_aiq_sys_ctx_t* ctx, bool on, float bias, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_LinExpAttrV2_t LinExpAttr;

    memset(&LinExpAttr, 0, sizeof(Uapi_LinExpAttrV2_t));
    ret = rk_aiq_user_api2_ae_getLinExpAttr(ctx, &LinExpAttr);
    LinExpAttr.sync.sync_mode = sync;
    LinExpAttr.sync.done = false;
    //set BackLightCtrl
    LinExpAttr.Params.BackLightCtrl.Enable = on;
    LinExpAttr.Params.BackLightCtrl.StrBias = bias;
    ret = rk_aiq_user_api2_ae_setLinExpAttr(ctx, LinExpAttr);

    return 0;
}

static int sample_set_OverExpCtrl(const rk_aiq_sys_ctx_t* ctx, bool on, float bias, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_LinExpAttrV2_t LinExpAttr;

    memset(&LinExpAttr, 0, sizeof(Uapi_LinExpAttrV2_t));
    ret = rk_aiq_user_api2_ae_getLinExpAttr(ctx, &LinExpAttr);
    LinExpAttr.sync.sync_mode = sync;
    LinExpAttr.sync.done = false;
    //set OverExpCtrl
    LinExpAttr.Params.OverExpCtrl.Enable = on;
    LinExpAttr.Params.OverExpCtrl.StrBias = bias;
    ret = rk_aiq_user_api2_ae_setLinExpAttr(ctx, LinExpAttr);

    return 0;
}

static int sample_set_exp_ratio_type(const rk_aiq_sys_ctx_t* ctx, CalibDb_HdrAeRatioTypeV2_t type, float bias, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_HdrExpAttrV2_t HdrExpAttr;

    memset(&HdrExpAttr, 0, sizeof(Uapi_HdrExpAttrV2_t));
    ret = rk_aiq_user_api2_ae_getHdrExpAttr(ctx, &HdrExpAttr);
    HdrExpAttr.sync.sync_mode = sync;
    HdrExpAttr.sync.done = false;
    //set ExpRatioType (AUTO/FIX)
    HdrExpAttr.Params.ExpRatioCtrl.ExpRatioType = type;
    //set Evbias
    HdrExpAttr.Params.Evbias = bias;
    ret = rk_aiq_user_api2_ae_setHdrExpAttr(ctx, HdrExpAttr);

    return 0;
}

static int sample_set_linear_route(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_LinAeRouteAttr_t LinExpRouteAttr;

    memset(&LinExpRouteAttr, 0x00, sizeof(Uapi_LinAeRouteAttr_t));
    ret = rk_aiq_user_api2_ae_getLinAeRouteAttr(ctx, &LinExpRouteAttr);
    LinExpRouteAttr.sync.sync_mode = sync;
    LinExpRouteAttr.sync.done = false;

    int len = 8;
    float TimeDot[8] = {0, 0.01, 0.01, 0.02, 0.02, 0.03, 0.03, 0.04};
    float GainDot[8] = {1, 1, 4, 4, 8, 8, 16, 32};
    float IspGainDot[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    int PirisDot[8] = {512, 512, 512, 512, 512, 512, 512, 512};

    LinExpRouteAttr.Params.TimeDot_len = len;
    LinExpRouteAttr.Params.GainDot_len = len;
    LinExpRouteAttr.Params.IspDGainDot_len = len;
    LinExpRouteAttr.Params.PIrisDot_len = len;

    LinExpRouteAttr.Params.GainDot = GainDot;
    LinExpRouteAttr.Params.IspDGainDot = IspGainDot;
    LinExpRouteAttr.Params.TimeDot = TimeDot;
    LinExpRouteAttr.Params.PIrisDot = PirisDot;
    ret = rk_aiq_user_api2_ae_setLinAeRouteAttr(ctx, LinExpRouteAttr);

    return 0;
}

static int  sample_set_hdr_mframe_params(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_HdrExpAttrV2_t HdrExpAttr;

    memset(&HdrExpAttr, 0, sizeof(Uapi_HdrExpAttrV2_t));
    ret = rk_aiq_user_api2_ae_getHdrExpAttr(ctx, &HdrExpAttr);
    HdrExpAttr.sync.sync_mode = sync;
    HdrExpAttr.sync.done = false;
    int len = 8;
    float explevel[8] = {0, 0.096, 0.192, 0.384, 0.96, 1.344, 1.92, 3};
    float setpoint[8] = {50, 45, 40, 35, 30, 25, 20, 15};

    HdrExpAttr.Params.MframeCtrl.MExpLevel_len = len;
    HdrExpAttr.Params.MframeCtrl.MSetPoint_len = len;
    HdrExpAttr.Params.MframeCtrl.MExpLevel = explevel;
    HdrExpAttr.Params.MframeCtrl.MSetPoint = setpoint;

    ret = rk_aiq_user_api2_ae_setHdrExpAttr(ctx, HdrExpAttr);

    return 0;
}

static int  sample_set_ae_speed(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    Uapi_ExpSwAttrV2_t expSwAttr;
    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    //set ae speed
    expSwAttr.stAuto.stAeSpeed.DampOver = 0.8f;
    expSwAttr.stAuto.stAeSpeed.DampDark2Bright = 0.8f;

    expSwAttr.stAuto.stAeSpeed.DampUnder = 0.8f;
    expSwAttr.stAuto.stAeSpeed.DampBright2Dark = 0.8f;

    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;

}

static int  sample_set_ae_delay(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    Uapi_ExpSwAttrV2_t expSwAttr;
    ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.sync.sync_mode = sync;
    expSwAttr.sync.done = false;
    //set ae delay

    expSwAttr.stAuto.DelayType = DELAY_TYPE_FRAME;
    expSwAttr.stAuto.BlackDelay = 10;
    expSwAttr.stAuto.WhiteDelay = 15;

    ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;

}

XCamReturn sample_ae_module (const void *arg)
{
    int key = -1;
    CLEAR();

    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup) {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    if (ctx == nullptr) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    Uapi_ExpWin_t ExpWin;
    Uapi_ExpSwAttrV2_t expSwAttr;

    do {
        sample_ae_usage ();

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar ();
        printf ("\n");

        switch (key)
        {
        case '0':
            sample_set_ae_speed(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set ae speed\n\n");
            break;
        case '1':
            sample_set_ae_delay(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set ae delay\n\n");
            break;
        case '2':
            sample_set_exp_manual(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set exp manual\n\n");
            break;
        case '3':
            sample_set_exp_auto(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set exp auto\n\n");
            break;
        case '4':
            sample_set_time_manual_and_gain_auto(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set manual time & auto gain\n\n");
            break;
        case '5':
            sample_set_time_auto_and_gain_manual(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set auto time & manual gain\n\n");
            break;
        case '6':
            sample_set_time_range(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set time range\n\n");
            break;
        case '7':
            sample_get_time_range(ctx);
            printf("get time range\n\n");
            break;
        case '8':
            sample_set_gain_range(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set gain range\n\n");
            break;
        case '9':
            sample_get_gain_range(ctx);
            printf("get gain range\n\n");
            break;
        case 'a':
            sample_is_fps_fix(ctx, true, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set fix fps\n\n");
            break;
        case 'b':
            sample_is_fps_fix(ctx, false, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set auto fps\n\n");
            break;
        case 'c':
            sample_set_anti_flicker(ctx, true, AECV2_FLICKER_FREQUENCY_50HZ, AECV2_ANTIFLICKER_AUTO_MODE, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set anti_flicker frequency 50HZ\n\n");
            break;
        case 'd':
            sample_set_anti_flicker(ctx, true, AECV2_FLICKER_FREQUENCY_60HZ, AECV2_ANTIFLICKER_NORMAL_MODE, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set anti_flicker frequency 60HZ\n\n");
            break;
        case 'e':
            sample_set_anti_flicker(ctx, false, AECV2_FLICKER_FREQUENCY_50HZ, AECV2_ANTIFLICKER_AUTO_MODE, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set anti_flicker disable\n\n");
            break;
        case 'f':
            sample_set_gridweight(ctx, 0, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set grid weight directly\n\n");
            break;
        case 'g':
            sample_set_gridweight(ctx, 1, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set grid weight using stadvanced\n\n");
            break;
        case 'h':
            sample_set_ae_onoff(ctx, true);
            printf("set ae on\n\n");
            break;
        case 'i':
            sample_set_ae_onoff(ctx, false);
            printf("set ae off\n\n");
            break;
        case 'j':
            ExpWin.Params.h_offs = 0;
            ExpWin.Params.v_offs = 0;
            ExpWin.Params.h_size = 240;
            ExpWin.Params.v_size = 100;
            ExpWin.sync.sync_mode = RK_AIQ_UAPI_MODE_DEFAULT;
            rk_aiq_user_api2_ae_setExpWinAttr(ctx, ExpWin);
            printf("set exp windows[0,0,240,100]\n\n");
            break;
        case 'k':
            sample_set_BackLightCtrl(ctx, true, 100.0f, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set back_light_ctrl\n\n");
            break;
        case 'l':
            sample_set_OverExpCtrl(ctx, true, -100.0f, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set over_exp_ctrl\n\n");
            break;
        case 'm':
            sample_set_exp_ratio_type(ctx, AECV2_HDR_RATIOTYPE_MODE_AUTO, 50.0f, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set hdr exp ratio\n\n");
            break;
        case 'n':
            sample_set_linear_route(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set linear route\n\n");
            break;
        case 'o':
            sample_set_hdr_mframe_params(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set hdr mframe params\n\n");
            break;
        // TEST SYNC MODE
        case 'W':
            sample_set_exp_manual(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
            printf("set exp manual, sync mode.\n\n");
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);

            usleep(40 * 1000);
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
            break;
        case 'X':
            sample_set_exp_manual(ctx, RK_AIQ_UAPI_MODE_SYNC);
            printf("set exp manual, sync mode.\n\n");
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);

            usleep(40 * 1000);
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
            break;
        case 'Y':
            sample_set_exp_manual(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            printf("set exp manual, async mode.\n\n");
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);

            usleep(40 * 1000);
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
            break;
        case 'Z':
            sample_set_exp_manual(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            printf("set exp manual, async mode(two groups).\n\n");
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
            printf("\t get time = %f gain = %f\n", expSwAttr.stManual.LinearAE.TimeValue, expSwAttr.stManual.LinearAE.GainValue);

            sample_set_exp_manual_test(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
            printf("\t get time = %f gain = %f\n", expSwAttr.stManual.LinearAE.TimeValue, expSwAttr.stManual.LinearAE.GainValue);

            usleep(40 * 1000);
            memset(&expSwAttr, 0, sizeof(Uapi_ExpSwAttrV2_t));
            rk_aiq_user_api2_ae_getExpSwAttr(ctx, &expSwAttr);
            printf("\t sync = %d, done = %d\n", expSwAttr.sync.sync_mode, expSwAttr.sync.done);
            printf("\t get time = %f gain = %f\n", expSwAttr.stManual.LinearAE.TimeValue, expSwAttr.stManual.LinearAE.GainValue);
            break;
        default:
            break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
