// Copyright 2020 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#ifdef RKAIQ

#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

#define MAX_AIQ_CTX 4
static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
static pthread_mutex_t aiq_ctx_mutex[MAX_AIQ_CTX] = {
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER};
pthread_mutex_t lock[MAX_AIQ_CTX] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
                                     PTHREAD_MUTEX_INITIALIZER,
                                     PTHREAD_MUTEX_INITIALIZER};
static unsigned char gs_LDC_mode[MAX_AIQ_CTX] = {-1, -1, -1, -1};
rk_aiq_cpsl_cfg_t g_cpsl_cfg[MAX_AIQ_CTX];
rk_aiq_wb_gain_t gs_wb_auto_gain = {2.083900, 1.000000, 1.000000, 2.018500};
RK_U32 g_2dnr_default_level = 50;
RK_U32 g_3dnr_default_level = 50;
rk_aiq_working_mode_t g_WDRMode[MAX_AIQ_CTX];

typedef enum _SHUTTERSPEED_TYPE_E {
	SHUTTERSPEED_1_25 = 0,
	SHUTTERSPEED_1_30,
	SHUTTERSPEED_1_75,
	SHUTTERSPEED_1_100,
	SHUTTERSPEED_1_120,
	SHUTTERSPEED_1_150,
	SHUTTERSPEED_1_250,
	SHUTTERSPEED_1_300,
	SHUTTERSPEED_1_425,
	SHUTTERSPEED_1_600,
	SHUTTERSPEED_1_1000,
	SHUTTERSPEED_1_1250,
	SHUTTERSPEED_1_1750,
	SHUTTERSPEED_1_2500,
	SHUTTERSPEED_1_3000,
	SHUTTERSPEED_1_6000,
	SHUTTERSPEED_1_10000,
	SHUTTERSPEED_BUTT
} SHUTTERSPEED_TYPE_E;

typedef struct rk_SHUTTER_ATTR_S {
	SHUTTERSPEED_TYPE_E enShutterSpeed;
	float fExposureTime;
} SHUTTER_ATTR_S;

static SHUTTER_ATTR_S g_stShutterAttr[SHUTTERSPEED_BUTT] = {
    {SHUTTERSPEED_1_25, 1.0 / 25.0},      {SHUTTERSPEED_1_30, 1.0 / 30.0},
    {SHUTTERSPEED_1_75, 1.0 / 75.0},      {SHUTTERSPEED_1_100, 1.0 / 100.0},
    {SHUTTERSPEED_1_120, 1.0 / 120.0},    {SHUTTERSPEED_1_150, 1.0 / 150.0},
    {SHUTTERSPEED_1_250, 1.0 / 250.0},    {SHUTTERSPEED_1_300, 1.0 / 300.0},
    {SHUTTERSPEED_1_425, 1.0 / 425.0},    {SHUTTERSPEED_1_600, 1.0 / 600.0},
    {SHUTTERSPEED_1_1000, 1.0 / 1000.0},  {SHUTTERSPEED_1_1250, 1.0 / 1250.0},
    {SHUTTERSPEED_1_1750, 1.0 / 1750.0},  {SHUTTERSPEED_1_2500, 1.0 / 2500.0},
    {SHUTTERSPEED_1_3000, 1.0 / 3000.0},  {SHUTTERSPEED_1_6000, 1.0 / 6000.0},
    {SHUTTERSPEED_1_10000, 1.0 / 10000.0}};

typedef enum rk_HDR_MODE_E {
	HDR_MODE_OFF,
	HDR_MODE_HDR2,
	HDR_MODE_HDR3,
} HDR_MODE_E;

RK_S32 SAMPLE_COMM_ISP_Init(RK_S32 CamId, rk_aiq_working_mode_t WDRMode, RK_BOOL MultiCam,
                            const char *iq_file_dir) {
	if (CamId >= MAX_AIQ_CTX) {
		printf("%s : CamId is over 3\n", __FUNCTION__);
		return -1;
	}
	// char *iq_file_dir = "iqfiles/";
	setlinebuf(stdout);
	if (iq_file_dir == NULL) {
		printf("SAMPLE_COMM_ISP_Init : not start.\n");
		g_aiq_ctx[CamId] = NULL;
		return 0;
	}

	// must set HDR_MODE, before init
	g_WDRMode[CamId] = WDRMode;
	char hdr_str[16];
	snprintf(hdr_str, sizeof(hdr_str), "%d", (int)WDRMode);
	setenv("HDR_MODE", hdr_str, 1);

	rk_aiq_sys_ctx_t *aiq_ctx;
	rk_aiq_static_info_t aiq_static_info;
	rk_aiq_uapi_sysctl_enumStaticMetas(CamId, &aiq_static_info);

	printf("ID: %d, sensor_name is %s, iqfiles is %s\n", CamId,
	       aiq_static_info.sensor_info.sensor_name, iq_file_dir);

	aiq_ctx = rk_aiq_uapi_sysctl_init(aiq_static_info.sensor_info.sensor_name,
	                                  iq_file_dir, NULL, NULL);
	if (MultiCam)
		rk_aiq_uapi_sysctl_setMulCamConc(aiq_ctx, true);

	g_aiq_ctx[CamId] = aiq_ctx;
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_UpdateIq(RK_S32 CamId, char *iqfile) {

	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}

	RK_S32 ret = 0;
	printf(" rk_aiq_uapi_sysctl_updateIq %s\n", iqfile);
	ret = rk_aiq_uapi_sysctl_updateIq(g_aiq_ctx[CamId], iqfile);
	return ret;
}

/*
set after SAMPLE_COMM_ISP_Init and before SAMPLE_COMM_ISP_Run
*/
RK_S32 SAMPLE_COMM_ISP_SetFecEn(RK_S32 CamId, RK_BOOL bFECEnable) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	printf("rk_aiq_uapi_sysctl_init bFECEnable %d\n", bFECEnable);
	ret = rk_aiq_uapi_setFecEn(g_aiq_ctx[CamId], bFECEnable);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_Stop(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	printf("rk_aiq_uapi_sysctl_stop enter\n");
	rk_aiq_uapi_sysctl_stop(g_aiq_ctx[CamId], false);
	printf("rk_aiq_uapi_sysctl_deinit enter\n");
	rk_aiq_uapi_sysctl_deinit(g_aiq_ctx[CamId]);
	printf("rk_aiq_uapi_sysctl_deinit exit\n");
	g_aiq_ctx[CamId] = NULL;
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_Run(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	if (rk_aiq_uapi_sysctl_prepare(g_aiq_ctx[CamId], 0, 0, g_WDRMode[CamId])) {
		printf("rkaiq engine prepare failed !\n");
		g_aiq_ctx[CamId] = NULL;
		return -1;
	}
	printf("rk_aiq_uapi_sysctl_init/prepare succeed\n");
	if (rk_aiq_uapi_sysctl_start(g_aiq_ctx[CamId])) {
		printf("rk_aiq_uapi_sysctl_start  failed\n");
		return -1;
	}
	printf("rk_aiq_uapi_sysctl_start succeed\n");
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_DumpExpInfo(RK_S32 CamId, rk_aiq_working_mode_t WDRMode) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	char aStr[128] = {'\0'};
	Uapi_ExpQueryInfo_t stExpInfo;
	rk_aiq_wb_cct_t stCCT;
	RK_S32 ret = 0;
	ret = rk_aiq_user_api_ae_queryExpResInfo(g_aiq_ctx[CamId], &stExpInfo);
	ret |= rk_aiq_user_api_awb_GetCCT(g_aiq_ctx[CamId], &stCCT);

	if (WDRMode == RK_AIQ_WORKING_MODE_NORMAL) {
		sprintf(aStr, "M:%.0f-%.1f LM:%.1f CT:%.1f",
		        stExpInfo.CurExpInfo.LinearExp.exp_real_params.integration_time * 1000 *
		            1000,
		        stExpInfo.CurExpInfo.LinearExp.exp_real_params.analog_gain,
		        stExpInfo.MeanLuma, stCCT.CCT);
	} else {
		sprintf(
		    aStr,
		    "S:%.0f-%.1f M:%.0f-%.1f L:%.0f-%.1f SLM:%.1f MLM:%.1f "
		    "LLM:%.1f CT:%.1f",
		    stExpInfo.CurExpInfo.HdrExp[0].exp_real_params.integration_time * 1000 * 1000,
		    stExpInfo.CurExpInfo.HdrExp[0].exp_real_params.analog_gain,
		    stExpInfo.CurExpInfo.HdrExp[1].exp_real_params.integration_time * 1000 * 1000,
		    stExpInfo.CurExpInfo.HdrExp[1].exp_real_params.analog_gain,
		    stExpInfo.CurExpInfo.HdrExp[2].exp_real_params.integration_time * 1000 * 1000,
		    stExpInfo.CurExpInfo.HdrExp[2].exp_real_params.analog_gain,
		    stExpInfo.HdrMeanLuma[0], stExpInfo.HdrMeanLuma[1], stExpInfo.HdrMeanLuma[2],
		    stCCT.CCT);
	}
	printf("isp exp dump: %s\n", aStr);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SetFrameRate(RK_S32 CamId, RK_U32 uFps) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	printf("SAMPLE_COMM_ISP_SetFrameRate start %d\n", uFps);

	frameRateInfo_t info;
	info.mode = OP_MANUAL;
	info.fps = uFps;
	ret = rk_aiq_uapi_setFrameRate(g_aiq_ctx[CamId], info);

	printf("SAMPLE_COMM_ISP_SetFrameRate %d\n", uFps);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SetLDCH(RK_S32 CamId, RK_U32 level, RK_BOOL bIfEnable) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	ret = rk_aiq_uapi_setLdchEn(g_aiq_ctx[CamId], level > 0);
	if (level > 0 && level <= 255)
		ret |= rk_aiq_uapi_setLdchCorrectLevel(g_aiq_ctx[CamId], level);
	return ret;
}

/*only support switch between HDR and normal*/
RK_S32 SAMPLE_COMM_ISP_SetWDRModeDyn(RK_S32 CamId, rk_aiq_working_mode_t WDRMode) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	ret = rk_aiq_uapi_sysctl_swWorkingModeDyn(g_aiq_ctx[CamId], WDRMode);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_Brightness(RK_S32 CamId, RK_U32 value) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_setBrightness(g_aiq_ctx[CamId], value); // value[0,255]
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_Contrast(RK_S32 CamId, RK_U32 value) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_setContrast(g_aiq_ctx[CamId], value); // value[0,255]
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_Saturation(RK_S32 CamId, RK_U32 value) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_setSaturation(g_aiq_ctx[CamId], value); // value[0,255]
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_Sharpness(RK_S32 CamId, RK_U32 value) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	RK_U32 level = value / 2.55;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_setSharpness(g_aiq_ctx[CamId],
		                               level); // value[0,255]->level[0,100]
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_ManualExposureAutoGain(RK_S32 CamId, RK_U32 u32Shutter) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	Uapi_ExpSwAttr_t stExpSwAttr;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_user_api_ae_getExpSwAttr(g_aiq_ctx[CamId], &stExpSwAttr);
		stExpSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
		stExpSwAttr.stManual.stLinMe.ManualGainEn = RK_FALSE;
		stExpSwAttr.stManual.stLinMe.ManualTimeEn = RK_TRUE;
		stExpSwAttr.stManual.stLinMe.TimeValue =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;

		stExpSwAttr.stManual.stHdrMe.ManualGainEn = RK_FALSE;
		stExpSwAttr.stManual.stHdrMe.ManualTimeEn = RK_TRUE;
		stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[0] =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;
		stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[1] =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;
		stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[2] =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;
		ret |= rk_aiq_user_api_ae_setExpSwAttr(g_aiq_ctx[CamId], stExpSwAttr);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_ManualExposureManualGain(RK_S32 CamId, RK_U32 u32Shutter,
                                                    RK_U32 u32Gain) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	Uapi_ExpSwAttr_t stExpSwAttr;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_user_api_ae_getExpSwAttr(g_aiq_ctx[CamId], &stExpSwAttr);
		stExpSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
		stExpSwAttr.stManual.stLinMe.ManualGainEn = RK_TRUE;
		stExpSwAttr.stManual.stLinMe.ManualTimeEn = RK_TRUE;
		stExpSwAttr.stManual.stLinMe.TimeValue =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;
		//(1+gain)*4;//gain[0~255] -> GainValue[1~1024]
		stExpSwAttr.stManual.stLinMe.GainValue = (1.0 + u32Gain);

		stExpSwAttr.stManual.stHdrMe.ManualGainEn = RK_TRUE;
		stExpSwAttr.stManual.stHdrMe.ManualTimeEn = RK_TRUE;
		stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[0] =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;
		stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[1] =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;
		stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[2] =
		    g_stShutterAttr[u32Shutter % SHUTTERSPEED_BUTT].fExposureTime;
		//(1+gain)*4;//gain[0~255] -> GainValue[1~1024]
		stExpSwAttr.stManual.stHdrMe.GainValue.fCoeff[0] = (1.0 + u32Gain);
		//(1+gain)*4;//gain[0~255] -> GainValue[1~1024]
		stExpSwAttr.stManual.stHdrMe.GainValue.fCoeff[1] = (1.0 + u32Gain);
		//(1+gain)*4;//gain[0~255] -> GainValue[1~1024]
		stExpSwAttr.stManual.stHdrMe.GainValue.fCoeff[2] = (1.0 + u32Gain);
		ret |= rk_aiq_user_api_ae_setExpSwAttr(g_aiq_ctx[CamId], stExpSwAttr);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_AutoExposure(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	Uapi_ExpSwAttr_t stExpSwAttr;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_user_api_ae_getExpSwAttr(g_aiq_ctx[CamId], &stExpSwAttr);
		stExpSwAttr.AecOpType = RK_AIQ_OP_MODE_AUTO;
		ret |= rk_aiq_user_api_ae_setExpSwAttr(g_aiq_ctx[CamId], stExpSwAttr);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_Exposure(RK_S32 CamId, RK_BOOL bIsAutoExposure, RK_U32 bIsAGC,
                                    RK_U32 u32ElectronicShutter, RK_U32 u32Agc) {
	RK_S32 ret = 0;
	if (!bIsAutoExposure) {
		if (bIsAGC) {
			ret = SAMPLE_COMM_ISP_SET_ManualExposureAutoGain(CamId, u32ElectronicShutter);
		} else {
			ret = SAMPLE_COMM_ISP_SET_ManualExposureManualGain(
			    CamId, u32ElectronicShutter, u32Agc);
		}
	} else {
		ret = SAMPLE_COMM_ISP_SET_AutoExposure(CamId);
	}
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_BackLight(RK_S32 CamId, RK_BOOL bEnable, RK_U32 u32Strength) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		if (bEnable) {
			ret = rk_aiq_uapi_setBLCMode(g_aiq_ctx[CamId], RK_TRUE, AE_MEAS_AREA_AUTO);
			usleep(30000);
			//[0~2]->[1~100]
			ret |= rk_aiq_uapi_setBLCStrength(g_aiq_ctx[CamId], 33 * (u32Strength + 1));
		} else {
			ret = rk_aiq_uapi_setBLCMode(g_aiq_ctx[CamId], RK_FALSE, AE_MEAS_AREA_AUTO);
		}
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_LightInhibition(RK_S32 CamId, RK_BOOL bEnable,
                                           RK_U8 u8Strength, RK_U8 u8Level) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		if (bEnable) {
			ret = rk_aiq_uapi_setHLCMode(g_aiq_ctx[CamId], RK_TRUE);
			if (ret == 0) {
				//[0~255] -> level[0~100]
				ret = rk_aiq_uapi_setHLCStrength(g_aiq_ctx[CamId], u8Strength / 2.55);
				//[0~255] -> level[0~10]
				ret = rk_aiq_uapi_setDarkAreaBoostStrth(g_aiq_ctx[CamId], u8Level / 25);
			}
		} else {
			ret = rk_aiq_uapi_setHLCMode(g_aiq_ctx[CamId], RK_FALSE);
		}
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_CPSL_CFG(RK_S32 CamId, rk_aiq_cpsl_cfg_t *cpsl) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_sysctl_setCpsLtCfg(g_aiq_ctx[CamId], cpsl);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_OpenColorCloseLed(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&lock[CamId]);
	g_cpsl_cfg[CamId].lght_src = RK_AIQ_CPSLS_IR;
	g_cpsl_cfg[CamId].mode = RK_AIQ_OP_MODE_MANUAL;
	g_cpsl_cfg[CamId].gray_on = RK_FALSE;
	g_cpsl_cfg[CamId].u.m.on = 0;
	g_cpsl_cfg[CamId].u.m.strength_led = 0;
	g_cpsl_cfg[CamId].u.m.strength_ir = 0;
	ret = SAMPLE_COMM_ISP_SET_CPSL_CFG(CamId, &g_cpsl_cfg[CamId]);
	pthread_mutex_unlock(&lock[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_GrayOpenLed(RK_S32 CamId, RK_U8 u8Strength) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&lock[CamId]);
	g_cpsl_cfg[CamId].mode = RK_AIQ_OP_MODE_MANUAL;
	g_cpsl_cfg[CamId].lght_src = RK_AIQ_CPSLS_IR;
	g_cpsl_cfg[CamId].gray_on = RK_TRUE;
	g_cpsl_cfg[CamId].u.m.on = 1;
	g_cpsl_cfg[CamId].u.m.strength_led = u8Strength / 5 + 3;
	g_cpsl_cfg[CamId].u.m.strength_ir = u8Strength / 5 + 3;
	ret = SAMPLE_COMM_ISP_SET_CPSL_CFG(CamId, &g_cpsl_cfg[CamId]);
	pthread_mutex_unlock(&lock[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMMON_ISP_SET_AutoWhiteBalance(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_setWBMode(g_aiq_ctx[CamId], OP_AUTO);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMMON_ISP_SET_ManualWhiteBalance(RK_S32 CamId, RK_U32 u32RGain,
                                                RK_U32 u32GGain, RK_U32 u32BGain) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	rk_aiq_wb_gain_t gain;

	u32RGain = (u32RGain == 0) ? 1 : u32RGain;
	u32GGain = (u32GGain == 0) ? 1 : u32GGain;
	u32BGain = (u32BGain == 0) ? 1 : u32BGain;
	// gain.bgain =  (b_gain / 255.0f) * 4;//[0,255]->(0.0, 4.0]
	// gain.grgain = (g_gain / 255.0f) * 4;//[0,255]->(0.0, 4.0]
	// gain.gbgain = (g_gain / 255.0f) * 4;//[0,255]->(0.0, 4.0]
	// gain.rgain =  (r_gain / 255.0f) * 4;//[0,255]->(0.0, 4.0]

	//[0,255]->(0.0, 4.0]
	gain.rgain = (u32RGain / 128.0f) * gs_wb_auto_gain.rgain;
	//[0,255]->(0.0, 4.0]
	gain.grgain = (u32GGain / 128.0f) * gs_wb_auto_gain.grgain;
	//[0,255]->(0.0, 4.0]
	gain.gbgain = (u32GGain / 128.0f) * gs_wb_auto_gain.gbgain;
	//[0,255]->(0.0, 4.0]
	gain.bgain = (u32BGain / 128.0f) * gs_wb_auto_gain.bgain;

	printf("convert gain r g g b %f, %f, %f, %f\n", gain.rgain, gain.grgain, gain.gbgain,
	       gain.bgain);
	printf("auto gain r g g b %f, %f, %f, %f\n", gs_wb_auto_gain.rgain,
	       gs_wb_auto_gain.grgain, gs_wb_auto_gain.gbgain, gs_wb_auto_gain.bgain);
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_setMWBGain(g_aiq_ctx[CamId], &gain);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMMON_ISP_GET_WhiteBalanceGain(RK_S32 CamId, rk_aiq_wb_gain_t *gain) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_getMWBGain(g_aiq_ctx[CamId], gain);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);

	printf("Rgain = %f, Grgain = %f, Gbgain = %f, Bgain = %f\n", gain->rgain,
	       gain->grgain, gain->gbgain, gain->bgain);
	return ret;
}

// mode 0:off, 1:2d, 2:3d, 3: 2d+3d
RK_S32 SAMPLE_COMMON_ISP_SET_DNRStrength(RK_S32 CamId, RK_U32 u32Mode, RK_U32 u322DValue,
                                         RK_U32 u323Dvalue) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	RK_U32 u32_2d_value = (u322DValue / 128.0f) * g_2dnr_default_level;
	RK_U32 u32_3d_value = (u323Dvalue / 128.0f) * g_3dnr_default_level;
	printf(" mode = %d n_2d_value = %d n_3d_value = %d\r\n", u32Mode, u322DValue,
	       u323Dvalue);
	printf("u_2d_value = %d n_3d_value = %d\r\n", u32_2d_value, u32_3d_value);
	printf("g_2dnr_default_level = %d g_3dnr_default_level = %d\r\n",
	       g_2dnr_default_level, g_3dnr_default_level);
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		if (u32Mode == 0) {
			ret = rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_NR,
			                                      true); // 2D
			ret |= rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_TNR,
			                                       true); // 3D
			ret |= rk_aiq_uapi_setMSpaNRStrth(g_aiq_ctx[CamId], true,
			                                  g_2dnr_default_level); //[0,100]
			ret |= rk_aiq_uapi_setMTNRStrth(g_aiq_ctx[CamId], true,
			                                g_3dnr_default_level); //[0,100]
		} else if (u32Mode == 1)                                   // 2D
		{
			ret = rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_NR,
			                                      true); // 2D
			ret |= rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_TNR,
			                                       true); // 3D
			ret |= rk_aiq_uapi_setMSpaNRStrth(g_aiq_ctx[CamId], true,
			                                  u32_2d_value); ////[0,255] -> [0,100]
			ret |= rk_aiq_uapi_setMTNRStrth(g_aiq_ctx[CamId], true, g_3dnr_default_level);
		} else if (u32Mode == 2) // 3D
		{
			ret = rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_NR,
			                                      true); // 2D
			ret |= rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_TNR,
			                                       true); // 3D
			ret |= rk_aiq_uapi_setMSpaNRStrth(g_aiq_ctx[CamId], true,
			                                  g_2dnr_default_level); //[0,100]->[0,255]
			ret |= rk_aiq_uapi_setMTNRStrth(g_aiq_ctx[CamId], true, u32_3d_value);
		} else if (u32Mode == 3) //)//2D+3D
		{
			ret = rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_NR,
			                                      true); // 2D
			ret |= rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[CamId], RK_MODULE_TNR,
			                                       true); // 3D
			ret |= rk_aiq_uapi_setMSpaNRStrth(g_aiq_ctx[CamId], true,
			                                  u32_2d_value); //[0,255] -> [0,100]
			ret |= rk_aiq_uapi_setMTNRStrth(g_aiq_ctx[CamId], true,
			                                u32_3d_value); //[0,255] -> [0,100]
		}
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMMON_ISP_GET_DNRStrength(RK_S32 CamId, RK_U32 *u322DValue,
                                         RK_U32 *u323Dvalue) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	bool on;
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_getMSpaNRStrth(g_aiq_ctx[CamId], &on, u322DValue);
		ret |= rk_aiq_uapi_getMTNRStrth(g_aiq_ctx[CamId], &on, u323Dvalue);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMMON_ISP_SET_Flicker(RK_S32 CamId, RK_U8 u32Flicker) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	frameRateInfo_t info;

	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		if (u32Flicker == 0) // NTSC(60HZ)
		{

			ret = rk_aiq_uapi_setExpPwrLineFreqMode(g_aiq_ctx[CamId],
			                                        EXP_PWR_LINE_FREQ_60HZ);
			info.mode = OP_MANUAL;
			info.fps = 30;
			ret |= rk_aiq_uapi_setFrameRate(g_aiq_ctx[CamId], info);
		} else if (u32Flicker == 1) // PAL(50HZ)
		{
			ret = rk_aiq_uapi_setExpPwrLineFreqMode(g_aiq_ctx[CamId],
			                                        EXP_PWR_LINE_FREQ_50HZ);
			info.mode = OP_MANUAL;
			info.fps = 25;
			ret |= rk_aiq_uapi_setFrameRate(g_aiq_ctx[CamId], info);
		}
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

// for normal or hdr2
RK_S32 SAMPLE_COMM_ISP_SET_HDR(RK_S32 CamId, rk_aiq_working_mode_t mode) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_sysctl_swWorkingModeDyn(g_aiq_ctx[CamId], mode);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

// mode 0 0ff 1 on 2 auto
RK_S32 SAMPLE_COMM_ISP_SET_DefogEnable(RK_S32 CamId, RK_U32 u32Mode) {
	/*
	      int defog = (u32Mode == 0) ? 0 : 1;
	      if(gs_defog_mode == defog)
	      {
	              return 0;
	      }
	      gs_defog_mode = defog;
	      */
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		if (u32Mode == 0) {
			ret = rk_aiq_uapi_disableDhz(g_aiq_ctx[CamId]);
		} else {
			ret = rk_aiq_uapi_enableDhz(g_aiq_ctx[CamId]);
		}
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_DefogStrength(RK_S32 CamId, RK_U32 u32Mode, RK_U32 u32Value) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	unsigned int level = u32Value / 25.5;
	level = (level < 1) ? 1 : level;
	level = (level > 10) ? 10 : level;
	printf("defog level = %d\n", level);
	if (g_aiq_ctx[CamId]) {
		if (u32Mode == 1) {
			ret = rk_aiq_uapi_setDhzMode(g_aiq_ctx[CamId], OP_MANUAL);
			ret |=
			    rk_aiq_uapi_setMDhzStrth(g_aiq_ctx[CamId], true, level); //[0,255]->[1,10]
		} else if (u32Mode == 2) {
			ret = rk_aiq_uapi_setDhzMode(g_aiq_ctx[CamId], OP_AUTO);
		}
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_Correction(RK_S32 CamId, RK_U32 u32Mode, RK_U32 u32Value) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		if (gs_LDC_mode[CamId] != u32Mode) {
			if (u32Mode) {
				ret = rk_aiq_uapi_setLdchEn(g_aiq_ctx[CamId], true);
			} else {
				ret = rk_aiq_uapi_setLdchEn(g_aiq_ctx[CamId], false);
			}
			gs_LDC_mode[CamId] = u32Mode;
		}

		if (u32Mode) {
			ret |= rk_aiq_uapi_setLdchCorrectLevel(g_aiq_ctx[CamId],
			                                       (u32Value < 2 ? 2 : u32Value));
		}
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_mirror(RK_S32 CamId, RK_U32 u32Value) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	int mirror = 0;
	int flip = 0;
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	switch (u32Value) {
	case 0:
		mirror = 0;
		flip = 0;
		break;
	case 1:
		mirror = 1;
		flip = 0;
		break;
	case 2:
		mirror = 0;
		flip = 1;
		break;
	case 3:
		mirror = 1;
		flip = 1;
		break;
	default:
		mirror = 0;
		flip = 0;
		break;
	}

	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_setMirroFlip(g_aiq_ctx[CamId], mirror, flip, 4);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_BypassStreamRotation(RK_S32 CamId, RK_S32 S32Rotation) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	rk_aiq_rotation_t rk_rotation = RK_AIQ_ROTATION_0;
	if (S32Rotation == 0) {
		rk_rotation = RK_AIQ_ROTATION_0;
	} else if (S32Rotation == 90) {
		rk_rotation = RK_AIQ_ROTATION_90;
	} else if (S32Rotation == 270) {
		rk_rotation = RK_AIQ_ROTATION_270;
	}

	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_sysctl_setSharpFbcRotation(g_aiq_ctx[CamId], rk_rotation);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SET_Crop(RK_S32 CamId, rk_aiq_rect_t rect) {
	/*
  rk_aiq_rect_t rect;
  rect.left = 0;
  rect.top = 0;
  rect.width = 2560;
  rect.height = 1440;
  rk_aiq_uapi_sysctl_setCrop(aiq_ctx, rect);*/
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_S32 ret = 0;
	pthread_mutex_lock(&aiq_ctx_mutex[CamId]);
	if (g_aiq_ctx[CamId]) {
		ret = rk_aiq_uapi_sysctl_setCrop(g_aiq_ctx[CamId], rect);
	}
	pthread_mutex_unlock(&aiq_ctx_mutex[CamId]);
	return ret;
}
//#endif
