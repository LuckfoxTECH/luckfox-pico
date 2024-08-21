/*
 * rk_aiq_algo_ae_demo_itf.c
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

#include "rk_aiq_algo_types.h"
#include "rk_aiq_algo_ae_demo_itf.h"
#include "rk_aiq_user_api_sysctl.h"

XCAM_BEGIN_DECLARE

#define RKISP_ALGO_AE_DEMO_VERSION     "v0.0.1"
#define RKISP_ALGO_AE_DEMO_VENDOR      "Rockchip"
#define RKISP_ALGO_AE_DEMO_DESCRIPTION "Rockchip Ae Demo"


/*
 * 定义AE模块内部结构体
 */

typedef struct AeLocalConfig_s {
    rk_aiq_ae_param_t stAeParam;
    CalibDb_Sensor_Para_t stSensorInfo;
    CalibDb_Dcg_t stDcgInfo;
    rk_aiq_ae_statistics_t stAeInfo;
    RkAiqAecHwConfig_t aeHwConfig;
    rk_aiq_ae_result_t stAeResult;
    rk_aiq_ae_func_t func;
    int chanId;
    /* more definitions */
} AeLocalConfig_t;

typedef struct _RkAiqAlgoContext {
    AeLocalConfig_t config;
    const rk_aiq_sys_ctx_t* aiq_ctx;
} RkAiqAlgoContext;

/******************************************************************************
 * AeRegConv()
 *****************************************************************************/
static XCamReturn AeRegConv
(
    AeLocalConfig_t* pConfig,
    float SplitIntegrationTime,
    float SplitGain,
    unsigned int *regIntegrationTime,
    unsigned int *regGain,
    int *pDcgMode
) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    float C1 = 0.0f, C0 = 0.0f, M0 = 0.0f, minReg = 0.0f, maxReg = 0.0f, ag = 0.0f;
    int i;

    float SplitGainIn = 0.0f, SplitIntegrationTimeIn = 0.0f;

    SplitGainIn = SplitGain;
    SplitIntegrationTimeIn = SplitIntegrationTime;

    //gain convertion
    ag = SplitGainIn / (pDcgMode[AEC_NORMAL_FRAME] >= 1 ? pConfig->stDcgInfo.Normal.dcg_ratio : 1.0f);

    if(pConfig->stSensorInfo.GainRange.GainMode == RKAIQ_EXPGAIN_MODE_LINEAR) {
        for (i = 0; i < pConfig->stSensorInfo.GainRange.array_size; i += 7) {
            if (ag >= pConfig->stSensorInfo.GainRange.pGainRange[i] && ag <= pConfig->stSensorInfo.GainRange.pGainRange[i + 1]) {
                C1 = pConfig->stSensorInfo.GainRange.pGainRange[i + 2];
                C0 = pConfig->stSensorInfo.GainRange.pGainRange[i + 3];
                M0 = pConfig->stSensorInfo.GainRange.pGainRange[i + 4];
                minReg = pConfig->stSensorInfo.GainRange.pGainRange[i + 5];
                maxReg = pConfig->stSensorInfo.GainRange.pGainRange[i + 6];
                break;
            }
        }

        if (C1 == 0.0f) {
            printf("GAIN OUT OF RANGE: lasttime-gain: %f-%f", SplitIntegrationTimeIn, SplitGainIn);
            C1 = 16;
            C0 = 0;
            M0 = 1;
            minReg = 16;
            maxReg = 255;
        }


        printf("ag: %2.2f, C1: %2.2f  C0: %2.2f M0: %2.2f, minReg: %2.2f maxReg: %2.2f",
               ag, C1, C0, M0, minReg, maxReg);

        *regGain = (int)(C1 * pow(ag, M0) - C0 + 0.5f);
        if (*regGain < minReg)
            *regGain = minReg;
        if (*regGain > maxReg)
            *regGain = maxReg;

    } else if(pConfig->stSensorInfo.GainRange.GainMode == RKAIQ_EXPGAIN_MODE_NONLINEAR_DB) {
        *regGain = (int)(20.0f * log10(ag) * 10.0f / 3.0f + 0.5f);
    }

    //time convertion
    float timeC0 = pConfig->stSensorInfo.TimeFactor[0];
    float timeC1 = pConfig->stSensorInfo.TimeFactor[1];
    float timeC2 = pConfig->stSensorInfo.TimeFactor[2];
    float timeC3 = pConfig->stSensorInfo.TimeFactor[3];
    printf("time coefficient: %f-%f-%f-%f", timeC0, timeC1, timeC2, timeC3);


    float pclk = pConfig->stAeParam.sensor_desc.pixel_clock_freq_mhz;
    float hts = pConfig->stAeParam.sensor_desc.pixel_periods_per_line;
    float vts = pConfig->stAeParam.sensor_desc.frame_length_lines;

    *regIntegrationTime = (int)(timeC0 * vts + timeC1 + timeC2 * ((SplitIntegrationTimeIn * pclk * 1000000 / hts) + timeC3));


    int Index = (*regIntegrationTime - pConfig->stSensorInfo.CISTimeRegOdevity.fCoeff[1]) / (pConfig->stSensorInfo.CISTimeRegOdevity.fCoeff[0]);
    *regIntegrationTime = pConfig->stSensorInfo.CISTimeRegOdevity.fCoeff[0] * Index + pConfig->stSensorInfo.CISTimeRegOdevity.fCoeff[1];
    *regIntegrationTime = MAX(*regIntegrationTime, pConfig->stSensorInfo.CISTimeRegMin);

    return (ret);
}

static XCamReturn initAecHwConfig(AeLocalConfig_t* pConfig)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    //set rawae_sel
    printf("%s:Enter,pConfig->stAeParam.working_mode = %d \n", __FUNCTION__, pConfig->stAeParam.working_mode);
    if(pConfig->stAeParam.working_mode == RK_AIQ_WORKING_MODE_NORMAL) {
        pConfig->aeHwConfig.ae_meas.rawae0.rawae_sel = 2;
        pConfig->aeHwConfig.ae_meas.rawae1.rawae_sel = 2;
        pConfig->aeHwConfig.ae_meas.rawae2.rawae_sel = 2;
        pConfig->aeHwConfig.ae_meas.rawae3.rawae_sel = 3;
    } else if(RK_AIQ_HDR_GET_WORKING_MODE(pConfig->stAeParam.working_mode) == RK_AIQ_WORKING_MODE_ISP_HDR2) {
        pConfig->aeHwConfig.ae_meas.rawae0.rawae_sel = 2;
        pConfig->aeHwConfig.ae_meas.rawae1.rawae_sel = 2;
        pConfig->aeHwConfig.ae_meas.rawae2.rawae_sel = 2;
        pConfig->aeHwConfig.ae_meas.rawae3.rawae_sel = 3;
    } else if(RK_AIQ_HDR_GET_WORKING_MODE(pConfig->stAeParam.working_mode) == RK_AIQ_WORKING_MODE_ISP_HDR3) {
        pConfig->aeHwConfig.ae_meas.rawae0.rawae_sel = 1;
        pConfig->aeHwConfig.ae_meas.rawae1.rawae_sel = 1;
        pConfig->aeHwConfig.ae_meas.rawae2.rawae_sel = 1;
        pConfig->aeHwConfig.ae_meas.rawae3.rawae_sel = 3;
    }
    printf("%s:Enter raw_width = %d raw_height = %d \n", __FUNCTION__, pConfig->stAeParam.raw_width, pConfig->stAeParam.raw_height);
    /*****rawae0, LITE mode****/
    pConfig->aeHwConfig.ae_meas.rawae0.wnd_num = 1;
    pConfig->aeHwConfig.ae_meas.rawae0.win.h_offs = 0;
    pConfig->aeHwConfig.ae_meas.rawae0.win.v_offs = 0;
    pConfig->aeHwConfig.ae_meas.rawae0.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.ae_meas.rawae0.win.v_size = pConfig->stAeParam.raw_height;

    /*****rawae1-3, BIG mode****/
    pConfig->aeHwConfig.ae_meas.rawae1.wnd_num = 2;
    pConfig->aeHwConfig.ae_meas.rawae1.win.h_offs = 0;
    pConfig->aeHwConfig.ae_meas.rawae1.win.v_offs = 0;
    pConfig->aeHwConfig.ae_meas.rawae1.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.ae_meas.rawae1.win.v_size = pConfig->stAeParam.raw_height;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[0].h_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[0].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[0].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[0].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[1].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[1].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[1].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[1].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[2].h_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[2].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[2].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[2].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[3].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[3].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[3].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin[3].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae1.subwin_en[0] = 1;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin_en[1] = 1;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin_en[2] = 1;
    pConfig->aeHwConfig.ae_meas.rawae1.subwin_en[3] = 1;

    pConfig->aeHwConfig.ae_meas.rawae2.wnd_num = 2;
    pConfig->aeHwConfig.ae_meas.rawae2.win.h_offs = 0;
    pConfig->aeHwConfig.ae_meas.rawae2.win.v_offs = 0;
    pConfig->aeHwConfig.ae_meas.rawae2.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.ae_meas.rawae2.win.v_size = pConfig->stAeParam.raw_height;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[0].h_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[0].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[0].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[0].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[1].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[1].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[1].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[1].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[2].h_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[2].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[2].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[2].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[3].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[3].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[3].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin[3].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae2.subwin_en[0] = 1;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin_en[1] = 1;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin_en[2] = 1;
    pConfig->aeHwConfig.ae_meas.rawae2.subwin_en[3] = 1;

    pConfig->aeHwConfig.ae_meas.rawae3.wnd_num = 2;//窗口个数
    pConfig->aeHwConfig.ae_meas.rawae3.win.h_offs = 0;//窗口信息
    pConfig->aeHwConfig.ae_meas.rawae3.win.v_offs = 0;
    pConfig->aeHwConfig.ae_meas.rawae3.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.ae_meas.rawae3.win.v_size = pConfig->stAeParam.raw_height;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[0].h_offs = 2;//子窗口信息
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[0].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[0].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[0].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[1].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[1].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[1].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[1].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[2].h_offs = 2;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[2].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[2].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[2].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[3].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[3].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[3].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin[3].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.rawae3.subwin_en[0] = 1;//子窗口使能设置
    pConfig->aeHwConfig.ae_meas.rawae3.subwin_en[1] = 1;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin_en[2] = 1;
    pConfig->aeHwConfig.ae_meas.rawae3.subwin_en[3] = 1;

    /****rawhist0, LITE mode****/
    pConfig->aeHwConfig.hist_meas.rawhist0.data_sel = 0;
    pConfig->aeHwConfig.hist_meas.rawhist0.waterline = 0;
    pConfig->aeHwConfig.hist_meas.rawhist0.mode = 5;
    pConfig->aeHwConfig.hist_meas.rawhist0.stepsize = 0;
    pConfig->aeHwConfig.hist_meas.rawhist0.win.h_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist0.win.v_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist0.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.hist_meas.rawhist0.win.v_size = pConfig->stAeParam.raw_height;
    memset(pConfig->aeHwConfig.hist_meas.rawhist0.weight, 0x20, RAWHISTBIG_WIN_NUM * sizeof(unsigned char));
    pConfig->aeHwConfig.hist_meas.rawhist0.rcc = 0x4d;
    pConfig->aeHwConfig.hist_meas.rawhist0.gcc = 0x4b;
    pConfig->aeHwConfig.hist_meas.rawhist0.bcc = 0x1d;
    pConfig->aeHwConfig.hist_meas.rawhist0.off = 0x00;

    /****rawhist1-3, BIG mode****/
    pConfig->aeHwConfig.hist_meas.rawhist1.data_sel = 0;
    pConfig->aeHwConfig.hist_meas.rawhist1.waterline = 0;
    pConfig->aeHwConfig.hist_meas.rawhist1.mode = 5;
    pConfig->aeHwConfig.hist_meas.rawhist1.wnd_num = 2;
    pConfig->aeHwConfig.hist_meas.rawhist1.stepsize = 0;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.h_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.v_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.v_size = pConfig->stAeParam.raw_height;
    memset(pConfig->aeHwConfig.hist_meas.rawhist1.weight, 0x20, RAWHISTBIG_WIN_NUM * sizeof(unsigned char));
    pConfig->aeHwConfig.hist_meas.rawhist1.rcc = 0x4d;
    pConfig->aeHwConfig.hist_meas.rawhist1.gcc = 0x4b;
    pConfig->aeHwConfig.hist_meas.rawhist1.bcc = 0x1d;
    pConfig->aeHwConfig.hist_meas.rawhist1.off = 0x00;

    pConfig->aeHwConfig.hist_meas.rawhist2.data_sel = 0;
    pConfig->aeHwConfig.hist_meas.rawhist2.waterline = 0;
    pConfig->aeHwConfig.hist_meas.rawhist2.mode = 5;
    pConfig->aeHwConfig.hist_meas.rawhist2.wnd_num = 2;
    pConfig->aeHwConfig.hist_meas.rawhist2.stepsize = 0;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.h_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.v_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.v_size = pConfig->stAeParam.raw_height;
    memset(pConfig->aeHwConfig.hist_meas.rawhist2.weight, 0x20, RAWHISTBIG_WIN_NUM * sizeof(unsigned char));
    pConfig->aeHwConfig.hist_meas.rawhist2.rcc = 0x4d;
    pConfig->aeHwConfig.hist_meas.rawhist2.gcc = 0x4b;
    pConfig->aeHwConfig.hist_meas.rawhist2.bcc = 0x1d;
    pConfig->aeHwConfig.hist_meas.rawhist2.off = 0x00;

    pConfig->aeHwConfig.hist_meas.rawhist3.data_sel = 0;
    pConfig->aeHwConfig.hist_meas.rawhist3.waterline = 0;
    pConfig->aeHwConfig.hist_meas.rawhist3.mode = 5;
    pConfig->aeHwConfig.hist_meas.rawhist3.wnd_num = 2;
    pConfig->aeHwConfig.hist_meas.rawhist3.stepsize = 0;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.h_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.v_offs = 0;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.v_size = pConfig->stAeParam.raw_height;
    memset(pConfig->aeHwConfig.hist_meas.rawhist3.weight, 0x20, RAWHISTBIG_WIN_NUM * sizeof(unsigned char));//直方图统计权重，需要每帧动态计算
    pConfig->aeHwConfig.hist_meas.rawhist3.rcc = 0x4d;
    pConfig->aeHwConfig.hist_meas.rawhist3.gcc = 0x4b;
    pConfig->aeHwConfig.hist_meas.rawhist3.bcc = 0x1d;
    pConfig->aeHwConfig.hist_meas.rawhist3.off = 0x00;

    /***** yuvae ****/
    pConfig->aeHwConfig.ae_meas.yuvae.ysel = 1; //1:Y full range 0:Y limited range
    pConfig->aeHwConfig.ae_meas.yuvae.wnd_num = 1; //0:1x1, 1:15x15
    pConfig->aeHwConfig.ae_meas.yuvae.win.h_offs = 0;
    pConfig->aeHwConfig.ae_meas.yuvae.win.v_offs = 0;
    pConfig->aeHwConfig.ae_meas.yuvae.win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.ae_meas.yuvae.win.v_size = pConfig->stAeParam.raw_height;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[0].h_offs = 2;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[0].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[0].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[0].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[1].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[1].v_offs = 2;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[1].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[1].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[2].h_offs = 2;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[2].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[2].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[2].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[3].h_offs = 150;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[3].v_offs = 150;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[3].h_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin[3].v_size = 100;  // must even number
    pConfig->aeHwConfig.ae_meas.yuvae.subwin_en[0] = 1;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin_en[1] = 1;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin_en[2] = 1;
    pConfig->aeHwConfig.ae_meas.yuvae.subwin_en[3] = 1;

    /****sihist****/
    pConfig->aeHwConfig.hist_meas.sihist.wnd_num = 3;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].data_sel = 0;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].waterline = 0;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].auto_stop = 0;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].mode = 5;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].stepsize = 3;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.h_offs = 0;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.v_offs = 0;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.h_size = pConfig->stAeParam.raw_width;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.v_size = pConfig->stAeParam.raw_height;
    memset(pConfig->aeHwConfig.hist_meas.sihist.hist_weight, 0x20, SIHIST_WIN_NUM * sizeof(unsigned char));

    return ret;
}
/******************************************************************************
* GridWeight15x15to5x5()
*****************************************************************************/
static XCamReturn GridWeight15x15to5x5
(
    const unsigned char* inWeights,
    unsigned char*  outWeights
)
{

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    //initial check
    if (inWeights == NULL) {
        printf( "%s: inWeights = null!\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_MEM;
    }

    uint8_t line_5x5, col_5x5;
    uint8_t line_15x15, col_15x15;
    int i, j, k;

    for(int i = 0; i < 225; i++) {
        line_15x15 = i / 15;
        col_15x15 = i % 15;

        line_5x5 = line_15x15 / 3;
        col_5x5 = col_15x15 / 3;

        outWeights[line_5x5 * 5 + col_5x5] += inWeights[line_15x15 * 15 + col_15x15];
    }

    for(int i = 0; i < 25; i++)
        outWeights[i] /= 9;

    /*for (int i = 0; i<5; i++){
           printf("%2d %2d %2d %2d %2d\n",
             outWeights[i * 5 + 0], outWeights[i * 5 + 1], outWeights[i * 5 + 2], outWeights[i * 5 + 3], outWeights[i * 5 + 4]);
    }*/
    return ret;
}

static XCamReturn updateAecHwConfig(AeLocalConfig_t* pConfig)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    if (pConfig->stAeResult.meas_win.h_size == 0 ||
            pConfig->stAeResult.meas_win.v_size == 0) {
        pConfig->stAeResult.meas_win.h_size = pConfig->stAeParam.raw_width;
        pConfig->stAeResult.meas_win.v_size = pConfig->stAeParam.raw_height;
    }

    //update win
    pConfig->aeHwConfig.ae_meas.rawae0.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.ae_meas.rawae0.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.ae_meas.rawae1.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.ae_meas.rawae1.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.ae_meas.rawae2.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.ae_meas.rawae2.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.ae_meas.rawae3.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.ae_meas.rawae3.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.ae_meas.yuvae.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.ae_meas.yuvae.win.v_offs = pConfig->stAeResult.meas_win.v_offs;

    pConfig->aeHwConfig.hist_meas.rawhist0.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.hist_meas.rawhist0.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.v_offs = pConfig->stAeResult.meas_win.v_offs;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.h_offs = pConfig->stAeResult.meas_win.h_offs;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.v_offs = pConfig->stAeResult.meas_win.v_offs;

    pConfig->aeHwConfig.ae_meas.rawae0.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.ae_meas.rawae0.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.ae_meas.rawae1.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.ae_meas.rawae1.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.ae_meas.rawae2.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.ae_meas.rawae2.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.ae_meas.rawae3.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.ae_meas.rawae3.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.ae_meas.yuvae.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.ae_meas.yuvae.win.v_size = pConfig->stAeResult.meas_win.v_size;

    pConfig->aeHwConfig.hist_meas.rawhist0.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.hist_meas.rawhist0.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.hist_meas.rawhist1.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.hist_meas.rawhist2.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.hist_meas.rawhist3.win.v_size = pConfig->stAeResult.meas_win.v_size;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.h_size = pConfig->stAeResult.meas_win.h_size;
    pConfig->aeHwConfig.hist_meas.sihist.win_cfg[0].win.v_size = pConfig->stAeResult.meas_win.v_size;

    //update weight
    unsigned char outWeights[5 * 5] = {0};
    ret = GridWeight15x15to5x5(pConfig->stAeResult.meas_weight, outWeights);
    memcpy(pConfig->aeHwConfig.hist_meas.rawhist0.weight, outWeights, 5 * 5 * sizeof(unsigned char)); //LITE MODE, use 5X5 weight

    memcpy(pConfig->aeHwConfig.hist_meas.rawhist1.weight, pConfig->stAeResult.meas_weight, 15 * 15 * sizeof(unsigned char));
    memcpy(pConfig->aeHwConfig.hist_meas.rawhist2.weight, pConfig->stAeResult.meas_weight, 15 * 15 * sizeof(unsigned char));
    memcpy(pConfig->aeHwConfig.hist_meas.rawhist3.weight, pConfig->stAeResult.meas_weight, 15 * 15 * sizeof(unsigned char));
    memcpy(pConfig->aeHwConfig.hist_meas.sihist.hist_weight, pConfig->stAeResult.meas_weight, 15 * 15 * sizeof(unsigned char));

    return (ret);
}
/*
 * AE模块初始化创建工作
 *
 * 该函数在系统init阶段被调用一次，可在该函数中做AE算法的初始化工作。
 */
static XCamReturn AeDemoCreateCtx(RkAiqAlgoContext **context, const AlgoCtxInstanceCfg* cfg)
{
    /**********************************************************************************
     *                            RK code
     **********************************************************************************/
    RESULT ret = RK_AIQ_RET_SUCCESS;
    printf("%s:Enter!\n", __FUNCTION__);
    //RkAiqAlgoContext *ctx = new RkAiqAlgoContext();
    RkAiqAlgoContext *ctx = (RkAiqAlgoContext *)malloc((size_t)sizeof(RkAiqAlgoContext));
    if (ctx == NULL) {
        printf( "%s: create ae context fail!\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_MEM;
    }
    memset(&ctx->config, 0, sizeof(ctx->config));

    CamCalibDbContext_t* pcalib = rk_aiq_uapi_sysctl_getCurCalib(ctx->aiq_ctx);

    ctx->config.stSensorInfo = pcalib->sensor;
    ctx->config.stDcgInfo = pcalib->sysContrl.dcg;

    *context = ctx;
    printf("%s:Exit!\n", __FUNCTION__);
    return XCAM_RETURN_NO_ERROR;
}

/*
 * AE模块资源释放工作
 *
 * 该函数在系统deinit阶段被调用一次，可在该函数中做AE算法的资源释放工作。
 */
static XCamReturn AeDemoDestroyCtx(RkAiqAlgoContext *context)
{
    RESULT ret = RK_AIQ_RET_SUCCESS;
    printf("%s:Enter!\n", __FUNCTION__);


    /**********************************************************************************
     *                            customer code
     **********************************************************************************/
    context->config.func.pfn_ae_exit(0);



    /**********************************************************************************
     *                            RK code
     **********************************************************************************/
    if(context != NULL) {
        //delete context;
        //context = NULL;
        free(context);
        context = NULL;
    }

    printf("%s:Exit!\n", __FUNCTION__);
    return XCAM_RETURN_NO_ERROR;
}

/*
 * prepare回调函数的实现
 *
 * 该函数在系统prepare阶段被调用一次，传入AIQ prepare阶段环境初始化的参数。
 */
static XCamReturn AeDemoPrepare(RkAiqAlgoCom* params)
{

    RESULT ret = RK_AIQ_RET_SUCCESS;
    printf("%s:Enter!\n", __FUNCTION__);
    AeLocalConfig_t* pConfig = (AeLocalConfig_t*)&params->ctx->config;

    /**********************************************************************************
     *                            customer code
     **********************************************************************************/

    pConfig->stAeParam.working_mode = params->u.prepare.working_mode;
    pConfig->stAeParam.raw_width = params->u.prepare.sns_op_width;
    pConfig->stAeParam.raw_height = params->u.prepare.sns_op_height;

    RkAiqAlgoConfigAe* AeCfgParam = (RkAiqAlgoConfigAe*)params;

    pConfig->stAeParam.sensor_desc.frame_length_lines = AeCfgParam->LinePeriodsPerField;
    pConfig->stAeParam.sensor_desc.pixel_clock_freq_mhz = AeCfgParam->PixelClockFreqMHZ;
    pConfig->stAeParam.sensor_desc.pixel_periods_per_line = AeCfgParam->PixelPeriodsPerLine;

    printf("--------------- working_mode:%lu raw_width:%lu raw_height:%lu",
           pConfig->stAeParam.working_mode, pConfig->stAeParam.raw_width, pConfig->stAeParam.raw_height);

    initAecHwConfig(pConfig);
    rk_aiq_uapi_sysctl_getSensorDiscrib(params->ctx->aiq_ctx, &pConfig->stAeParam.sensor_desc);
    pConfig->func.pfn_ae_init(pConfig->chanId, &pConfig->stAeParam);


    printf("%s:Exit!\n", __FUNCTION__);
    return XCAM_RETURN_NO_ERROR;
}

/*
 * preProcess回调函数的实现
 *
 * 该函数用于图像帧AE预处理，在process之前调用。
 *
 * 需要根据AePreParams->ispAeStats指针所指向的AE统计信息，得到AePreResult所指向的结构体的信息，返回给AIQ使用
 * 本函数中计算出的参数可以在AeLocalConfig_t中保存，在AeDemoProcessing函数中可以引用
 */
static XCamReturn AeDemoPreProcess(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    RESULT ret = RK_AIQ_RET_SUCCESS;

    printf("%s:Enter!\n", __FUNCTION__);
    AeLocalConfig_t* pConfig = (AeLocalConfig_t*)&inparams->ctx->config;
    RkAiqAlgoPreAe* AePreParams = (RkAiqAlgoPreAe*)inparams;
    RkAiqAlgoPreResAe* AePreResParams = (RkAiqAlgoPreResAe*)outparams;
    AecPreResult_t* AePreResult = &AePreResParams->ae_pre_res;
#if 0
    // init process, stats would be NULL
    if(inparams->u.proc.init) {
    } else
#endif
    {

        /**********************************************************************************
         *                            customer code
         **********************************************************************************/

        pConfig->stAeInfo.aec_stats = *AePreParams->ispAeStats;
        pConfig->func.pfn_ae_run(pConfig->chanId, &pConfig->stAeInfo, &pConfig->stAeResult, 0);
        printf("%s:Enter pfn_ae_run!\n", __FUNCTION__);


        /**********************************************************************************
         *                            RK code
         **********************************************************************************/
        for(int i = 0; i < 3; i++) {
            AePreResult->HdrExp[i] = pConfig->stAeInfo.aec_stats.ae_exp.HdrExp[i];
        }
        pConfig->stAeResult.LinearExp.exp_real_params.digital_gain = 1.0f;
        AePreResult->LinearExp = pConfig->stAeInfo.aec_stats.ae_exp.LinearExp;

        //memset(&AePreResult->CISFeature, 0, sizeof(AePreResult->CISFeature));


        /*-----AeResult dynamicly convert to AeHwConfig------*/
        ret = updateAecHwConfig(pConfig);

    }
    printf("%s:Exit!\n", __FUNCTION__);
    return XCAM_RETURN_NO_ERROR;
}

/*
 * process回调函数的实现
 *
 * 该函数用于图像帧AE算法处理，每一帧图像的AE统计信息都会传入该函数处理。
 *
 * 根据prepare/preprocess中或保存的、或计算出来的参数，实现算法, 得到AeProcResParams结构体出口参数，传出给AIQ使用。
 */
static XCamReturn AeDemoProcessing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    /**********************************************************************************
     *                            RK code
     **********************************************************************************/
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    AeLocalConfig_t* pConfig = (AeLocalConfig_t*)&inparams->ctx->config;
    RkAiqAlgoProcAe* AeProcParams = (RkAiqAlgoProcAe*)inparams;
    RkAiqAlgoProcResAe* AeProcResParams = (RkAiqAlgoProcResAe*)outparams;

    AeProcResParams->new_ae_exp.frame_length_lines = pConfig->stAeResult.frame_length_lines;
    AeProcResParams->new_ae_exp.line_length_pixels = pConfig->stAeResult.line_length_pixels;
    AeProcResParams->new_ae_exp.LinearExp = pConfig->stAeResult.LinearExp;
    for(int i = 0; i < 3; i++) {
        AeProcResParams->new_ae_exp.HdrExp[i] = pConfig->stAeResult.HdrExp[i];
    }
    /*convert exp real values to reg values*/
    ret = AeRegConv(pConfig, AeProcResParams->new_ae_exp.LinearExp.exp_real_params.integration_time,
                    AeProcResParams->new_ae_exp.LinearExp.exp_real_params.analog_gain,
                    &AeProcResParams->new_ae_exp.LinearExp.exp_sensor_params.coarse_integration_time,
                    &AeProcResParams->new_ae_exp.LinearExp.exp_sensor_params.analog_gain_code_global,
                    &AeProcResParams->new_ae_exp.LinearExp.exp_real_params.dcg_mode);


    /* 0119：RK修改 */

    AeProcResParams->new_ae_exp.frame_length_lines = pConfig->stAeParam.sensor_desc.frame_length_lines;
    AeProcResParams->new_ae_exp.line_length_pixels = pConfig->stAeParam.sensor_desc.line_length_pck;
    AeProcResParams->new_ae_exp.LinearExp.exp_real_params.dcg_mode = -1;
    AeProcResParams->ae_proc_res.exp_set_cnt = 1;
    AeProcResParams->ae_proc_res.exp_set_tbl[0] = AeProcResParams->new_ae_exp;

    AeProcResParams->ae_meas = pConfig->aeHwConfig.ae_meas;
    AeProcResParams->hist_meas = pConfig->aeHwConfig.hist_meas;
    AeProcResParams->cur_ae_exp = pConfig->stAeInfo.aec_stats.ae_exp;
    return XCAM_RETURN_NO_ERROR;
}

/*
 * postProcess回调函数的实现
 *
 * 该函数用于图像帧AE算法后处理，在process之后调用。
 */
static XCamReturn AeDemoPostProcess(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    RESULT ret = RK_AIQ_RET_SUCCESS;

    return XCAM_RETURN_NO_ERROR;
}
#if 0
RkAiqAlgoDescription g_RkIspAlgoDescAeDemo = {
    .common = {
        .version = RKISP_ALGO_AE_DEMO_VERSION,
        .vendor  = RKISP_ALGO_AE_DEMO_VENDOR,
        .description = RKISP_ALGO_AE_DEMO_DESCRIPTION,
        .type    = RK_AIQ_ALGO_TYPE_AE,
        .id      = 0,
        .create_context  = AeDemoCreateCtx,
        .destroy_context = AeDemoDestroyCtx,
    },
    .prepare = AeDemoPrepare,
    .pre_process = AeDemoPreProcess,
    .processing = AeDemoProcessing,
    .post_process = AeDemoPostProcess,
};
#endif

#if 1
/* 区分可见光及红外通道 */
#endif

static RkAiqAlgoDescription* g_RkIspAlgoDescAeDemo[2] = { NULL, NULL };

XCamReturn
rk_aiq_AELibRegCallBack(const rk_aiq_sys_ctx_t* ctx, rk_aiq_ae_register_t *pstRegister, int ChannelID)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    if (!pstRegister)
        return XCAM_RETURN_ERROR_PARAM;

    printf("IN rk_aiq_uapi_sysctl_regLib\n");
    g_RkIspAlgoDescAeDemo[ChannelID] = (RkAiqAlgoDescription*)malloc(sizeof(RkAiqAlgoDescription));
    g_RkIspAlgoDescAeDemo[ChannelID]->common.version = RKISP_ALGO_AE_DEMO_VERSION;
    g_RkIspAlgoDescAeDemo[ChannelID]->common.vendor  = RKISP_ALGO_AE_DEMO_VENDOR;
    g_RkIspAlgoDescAeDemo[ChannelID]->common.description = RKISP_ALGO_AE_DEMO_DESCRIPTION;
    g_RkIspAlgoDescAeDemo[ChannelID]->common.type    = RK_AIQ_ALGO_TYPE_AE;
    g_RkIspAlgoDescAeDemo[ChannelID]->common.id      = 0;
    g_RkIspAlgoDescAeDemo[ChannelID]->common.create_context  = AeDemoCreateCtx;
    g_RkIspAlgoDescAeDemo[ChannelID]->common.destroy_context = AeDemoDestroyCtx;
    g_RkIspAlgoDescAeDemo[ChannelID]->prepare = AeDemoPrepare;
    g_RkIspAlgoDescAeDemo[ChannelID]->pre_process = AeDemoPreProcess;
    g_RkIspAlgoDescAeDemo[ChannelID]->processing = AeDemoProcessing;
    g_RkIspAlgoDescAeDemo[ChannelID]->post_process = AeDemoPostProcess;

    ret = rk_aiq_uapi_sysctl_regLib(ctx, &g_RkIspAlgoDescAeDemo[ChannelID]->common);
    if (ret != XCAM_RETURN_NO_ERROR)
        return ret;
    printf("%s:id = %d\n", __FUNCTION__, g_RkIspAlgoDescAeDemo[ChannelID]->common.id);
    ret = rk_aiq_uapi_sysctl_enableAxlib(ctx, g_RkIspAlgoDescAeDemo[ChannelID]->common.type, g_RkIspAlgoDescAeDemo[ChannelID]->common.id, true);
    if (ret != XCAM_RETURN_NO_ERROR)
        return ret;

    printf("IN rk_aiq_uapi_sysctl_getEnabledAxlibCtx\n");
    RkAiqAlgoContext* algoCtx = rk_aiq_uapi_sysctl_getEnabledAxlibCtx(ctx, g_RkIspAlgoDescAeDemo[ChannelID]->common.type);
    if (algoCtx != NULL) {
        printf("--------------REG REG REG REG REG REG\n");
        algoCtx->config.func = pstRegister->stAeExpFunc;
        algoCtx->aiq_ctx = ctx;
        algoCtx->config.chanId = ChannelID;
    }

    printf("OUT rk_aiq_uapi_sysctl_getEnabledAxlibCtx\n");

    return ret;
}

XCamReturn
rk_aiq_AELibunRegCallBack(const rk_aiq_sys_ctx_t* ctx, int ChannelID)
{
    rk_aiq_uapi_sysctl_unRegLib(ctx,
                                g_RkIspAlgoDescAeDemo[ChannelID]->common.type,
                                g_RkIspAlgoDescAeDemo[ChannelID]->common.id);
    free(g_RkIspAlgoDescAeDemo[ChannelID]);

    return XCAM_RETURN_NO_ERROR;

}

RKAIQ_END_DECLARE
