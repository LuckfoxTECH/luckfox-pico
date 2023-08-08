// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "rk_gpio.h"

#include <rk_aiq_user_api_imgproc.h>
#include <rk_aiq_user_api_sysctl.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "isp.c"

#define MAX_AIQ_CTX 4
#define FPS 30

char g_iq_file_dir_[256];

static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
rk_aiq_working_mode_t g_WDRMode[MAX_AIQ_CTX];
rk_aiq_wb_gain_t gs_wb_gain = {2.083900, 1.000000, 1.000000, 2.018500};

#define RK_ISP_CHECK_CAMERA_ID(CAMERA_ID)                                                          \
	do {                                                                                           \
		if (CAMERA_ID >= MAX_AIQ_CTX || !g_aiq_ctx[CAMERA_ID]) {                                   \
			LOG_ERROR("camera_id is over 3 or not init\n");                                        \
			return -1;                                                                             \
		}                                                                                          \
	} while (0)

int sample_common_isp_init(int cam_id, rk_aiq_working_mode_t WDRMode, bool MultiCam,
                           const char *iq_file_dir) {
	if (cam_id >= MAX_AIQ_CTX) {
		printf("%s : cam_id is over 3\n", __FUNCTION__);
		return -1;
	}
	setlinebuf(stdout);
	if (iq_file_dir == NULL) {
		printf("rk_isp_init : not start.\n");
		g_aiq_ctx[cam_id] = NULL;
		return 0;
	}

	// must set HDR_MODE, before init
	g_WDRMode[cam_id] = WDRMode;
	char hdr_str[16];
	snprintf(hdr_str, sizeof(hdr_str), "%d", (int)WDRMode);
	setenv("HDR_MODE", hdr_str, 1);

	rk_aiq_sys_ctx_t *aiq_ctx;
	rk_aiq_static_info_t aiq_static_info;
	rk_aiq_uapi_sysctl_enumStaticMetas(cam_id, &aiq_static_info);

	printf("ID: %d, sensor_name is %s, iq_file_dir is %s\n", cam_id,
	       aiq_static_info.sensor_info.sensor_name, iq_file_dir);

	aiq_ctx =
	    rk_aiq_uapi_sysctl_init(aiq_static_info.sensor_info.sensor_name, iq_file_dir, NULL, NULL);
	if (MultiCam)
		rk_aiq_uapi_sysctl_setMulCamConc(aiq_ctx, true);

	g_aiq_ctx[cam_id] = aiq_ctx;

	return 0;
}

int sample_common_isp_run(int cam_id) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (rk_aiq_uapi_sysctl_prepare(g_aiq_ctx[cam_id], 0, 0, g_WDRMode[cam_id])) {
		printf("rkaiq engine prepare failed !\n");
		g_aiq_ctx[cam_id] = NULL;
		return -1;
	}
	printf("rk_aiq_uapi_sysctl_init/prepare succeed\n");
	if (rk_aiq_uapi_sysctl_start(g_aiq_ctx[cam_id])) {
		printf("rk_aiq_uapi_sysctl_start  failed\n");
		return -1;
	}
	printf("rk_aiq_uapi_sysctl_start succeed\n");
	return 0;
}

int sample_common_isp_stop(int cam_id) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	printf("rk_aiq_uapi_sysctl_stop enter\n");
	rk_aiq_uapi_sysctl_stop(g_aiq_ctx[cam_id], false);
	printf("rk_aiq_uapi_sysctl_deinit enter\n");
	rk_aiq_uapi_sysctl_deinit(g_aiq_ctx[cam_id]);
	printf("rk_aiq_uapi_sysctl_deinit exit\n");
	g_aiq_ctx[cam_id] = NULL;
	return 0;
}

int rk_isp_get_frame_rate(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:fps", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_frame_rate(int cam_id, int uFps) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	LOG_INFO("start %d\n", uFps);

	frameRateInfo_t info;
	info.mode = OP_MANUAL;
	info.fps = uFps;
	ret = rk_aiq_uapi_setFrameRate(g_aiq_ctx[cam_id], info);

	LOG_INFO("end, %d\n", uFps);
	return ret;
}

// isp scenario

int rk_isp_get_scenario(int cam_id, const char **value) {
	*value = rk_param_get_string("isp:scenario", NULL);

	return 0;
}

int rk_isp_set_scenario(int cam_id, const char *value) {
	rk_param_set_string("isp:scenario", value);

	return 0;
}

// image adjustment

int rk_isp_get_contrast(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// int ret = rk_aiq_uapi_getContrast(g_aiq_ctx[cam_id], value);
	// *value = (int)(*value / 2.55);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:contrast", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_contrast(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setContrast(g_aiq_ctx[cam_id], (int)(value * 2.55)); // value[0,255]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:contrast", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_brightness(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// int ret = rk_aiq_uapi_getBrightness(g_aiq_ctx[cam_id], value);
	// *value = (int)(*value / 2.55);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:brightness", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_brightness(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setBrightness(g_aiq_ctx[cam_id], (int)(value * 2.55)); // value[0,255]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:brightness", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_saturation(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// int ret = rk_aiq_uapi_getSaturation(g_aiq_ctx[cam_id], value);
	// *value = (int)(*value / 2.55);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:saturation", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_saturation(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setSaturation(g_aiq_ctx[cam_id], (int)(value * 2.55)); // value[0,255]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:saturation", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_sharpness(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// int ret = rk_aiq_uapi_getSharpness(g_aiq_ctx[cam_id], value);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:sharpness", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_sharpness(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setSharpness(g_aiq_ctx[cam_id], value); // value[0,100]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:sharpness", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_hue(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// int ret = rk_aiq_uapi_getHue(g_aiq_ctx[cam_id], value);
	// *value = (int)(*value / 2.55);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:hue", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_hue(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setHue(g_aiq_ctx[cam_id], (int)(value * 2.55)); // value[0,255]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:hue", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

// exposure
int rk_isp_get_exposure_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_mode", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_exposure_mode(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	Uapi_ExpSwAttr_t stExpSwAttr;
	rk_aiq_user_api_ae_getExpSwAttr(g_aiq_ctx[cam_id], &stExpSwAttr);
	if (!strcmp(value, "auto")) {
		stExpSwAttr.AecOpType = RK_AIQ_OP_MODE_AUTO;
	} else {
		if (g_WDRMode[cam_id] != RK_AIQ_WORKING_MODE_NORMAL) {
			stExpSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
			stExpSwAttr.stManual.stHdrMe.ManualGainEn = true;
			stExpSwAttr.stManual.stHdrMe.ManualTimeEn = true;
		} else {
			stExpSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
			stExpSwAttr.stManual.stLinMe.ManualGainEn = true;
			stExpSwAttr.stManual.stLinMe.ManualTimeEn = true;
		}
	}
	int ret = rk_aiq_user_api_ae_setExpSwAttr(g_aiq_ctx[cam_id], stExpSwAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_mode", cam_id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_isp_get_gain_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:gain_mode", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_gain_mode(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	Uapi_ExpSwAttr_t stExpSwAttr;

	rk_aiq_user_api_ae_getExpSwAttr(g_aiq_ctx[cam_id], &stExpSwAttr);
	if (!strcmp(value, "auto")) {
		stExpSwAttr.stManual.stLinMe.ManualGainEn = false;
		stExpSwAttr.stManual.stHdrMe.ManualGainEn = false;
	} else {
		stExpSwAttr.stManual.stLinMe.ManualGainEn = true;
		stExpSwAttr.stManual.stHdrMe.ManualGainEn = true;
	}
	int ret = rk_aiq_user_api_ae_setExpSwAttr(g_aiq_ctx[cam_id], stExpSwAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:gain_mode", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_exposure_time(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_time", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_exposure_time(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	Uapi_ExpSwAttr_t stExpSwAttr;
	float den, num, result;
	if (strchr(value, '/') == NULL) {
		den = 1;
		sscanf(value, "%f", &result);
	} else {
		sscanf(value, "%f/%f", &num, &den);
		result = num / den;
	}
	rk_aiq_user_api_ae_getExpSwAttr(g_aiq_ctx[cam_id], &stExpSwAttr);
	stExpSwAttr.stManual.stLinMe.TimeValue = result;
	stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[0] = result;
	stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[1] = result;
	stExpSwAttr.stManual.stHdrMe.TimeValue.fCoeff[2] = result;
	int ret = rk_aiq_user_api_ae_setExpSwAttr(g_aiq_ctx[cam_id], stExpSwAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_time", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_exposure_gain(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_gain", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_exposure_gain(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	Uapi_ExpSwAttr_t stExpSwAttr;
	float gain_set = (value * 1.0f);
	rk_aiq_user_api_ae_getExpSwAttr(g_aiq_ctx[cam_id], &stExpSwAttr);
	stExpSwAttr.stManual.stLinMe.GainValue = gain_set;
	stExpSwAttr.stManual.stHdrMe.GainValue.fCoeff[0] = gain_set;
	stExpSwAttr.stManual.stHdrMe.GainValue.fCoeff[1] = gain_set;
	stExpSwAttr.stManual.stHdrMe.GainValue.fCoeff[2] = gain_set;
	int ret = rk_aiq_user_api_ae_setExpSwAttr(g_aiq_ctx[cam_id], stExpSwAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_gain", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_enable_ircut(bool on) {
	int ret, open_gpio, close_gpio;

	open_gpio = rk_param_get_int("isp:ircut_open_gpio", -1);
	close_gpio = rk_param_get_int("isp:ircut_close_gpio", -1);
	if ((open_gpio < 0) || (close_gpio < 0)) {
		LOG_ERROR("fail get gpio form ini file\n");
		return -1;
	}
	ret = rk_gpio_export_direction(open_gpio, false);
	ret |= rk_gpio_export_direction(close_gpio, false);

	if (on) {
		rk_gpio_set_value(open_gpio, 1);
		usleep(100 * 1000);
		rk_gpio_set_value(open_gpio, 0);

	} else {
		rk_gpio_set_value(close_gpio, 1);
		usleep(100 * 1000);
		rk_gpio_set_value(close_gpio, 0);
	}

	rk_gpio_unexport(open_gpio);
	rk_gpio_unexport(close_gpio);

	return ret;
}

// night_to_day
int rk_isp_get_night_to_day(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_night_to_day(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_gray_mode_t gray_mode;
	if (!strcmp(value, "day")) {
		gray_mode = RK_AIQ_GRAY_MODE_OFF;
		rk_isp_enable_ircut(true);
	} else if (!strcmp(value, "night")) {
		gray_mode = RK_AIQ_GRAY_MODE_ON;
		rk_isp_enable_ircut(false);
	}

	LOG_INFO("gray_mode is %d\n", gray_mode);
	ret = rk_aiq_uapi_setGrayMode(g_aiq_ctx[cam_id], gray_mode);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_fill_light_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:fill_light_mode", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_fill_light_mode(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_cpsl_cfg_t cpsl_cfg;
	if (!strcmp(value, "IR")) {
		cpsl_cfg.lght_src = RK_AIQ_CPSLS_IR;
	} else if (!strcmp(value, "LED")) {
		cpsl_cfg.lght_src = RK_AIQ_CPSLS_LED;
	}
	ret = rk_aiq_uapi_sysctl_setCpsLtCfg(g_aiq_ctx[cam_id], &cpsl_cfg);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:fill_light_mode", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_light_brightness(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:light_brightness", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_light_brightness(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_cpsl_cfg_t cpsl_cfg;
	cpsl_cfg.u.m.strength_led = value;
	cpsl_cfg.u.m.strength_ir = value;
	ret = rk_aiq_uapi_sysctl_setCpsLtCfg(g_aiq_ctx[cam_id], &cpsl_cfg);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:light_brightness", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_night_to_day_filter_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_level", g_aiq_ctx[cam_id]);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_night_to_day_filter_level(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// TODO
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_level", g_aiq_ctx[cam_id]);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_night_to_day_filter_time(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_time", g_aiq_ctx[cam_id]);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_night_to_day_filter_time(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// TODO
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_time", g_aiq_ctx[cam_id]);
	rk_param_set_int(entry, value);

	return ret;
}

// blc
int rk_isp_get_hdr(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_hdr(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (!strcmp(value, "close")) {
		ret = rk_aiq_uapi_sysctl_swWorkingModeDyn(g_aiq_ctx[cam_id], RK_AIQ_WORKING_MODE_NORMAL);
	} else if (!strcmp(value, "HDR2")) {
		ret = rk_aiq_uapi_sysctl_swWorkingModeDyn(g_aiq_ctx[cam_id], RK_AIQ_WORKING_MODE_ISP_HDR2);
	} else if (!strcmp(value, "HDR3")) {
		ret = rk_aiq_uapi_sysctl_swWorkingModeDyn(g_aiq_ctx[cam_id], RK_AIQ_WORKING_MODE_ISP_HDR3);
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_blc_region(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_region", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_blc_region(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (!strcmp(value, "close")) {
		ret = rk_aiq_uapi_setBLCMode(g_aiq_ctx[cam_id], false, AE_MEAS_AREA_AUTO);
	} else if (!strcmp(value, "open")) {
		ret = rk_aiq_uapi_setBLCMode(g_aiq_ctx[cam_id], true, AE_MEAS_AREA_AUTO);
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_region", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_hlc(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_hlc(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (!strcmp(value, "close")) {
		ret = rk_aiq_uapi_setHLCMode(g_aiq_ctx[cam_id], false);
	} else if (!strcmp(value, "open")) {
		ret = rk_aiq_uapi_setHLCMode(g_aiq_ctx[cam_id], true);
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_hdr_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_hdr_level(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (value)
		ret = rk_aiq_uapi_setMHDRStrth(g_aiq_ctx[cam_id], true, value);
	else
		ret = rk_aiq_uapi_setMHDRStrth(g_aiq_ctx[cam_id], true, 1);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_blc_strength(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_strength", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_blc_strength(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	ret = rk_aiq_uapi_setBLCStrength(g_aiq_ctx[cam_id], value); // [0, 100]

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_strength", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_hlc_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_hlc_level(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (value == 0)
		value = 1;
	ret = rk_aiq_uapi_setHLCStrength(g_aiq_ctx[cam_id], value); // level[1, 100]

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_dark_boost_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:dark_boost_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_dark_boost_level(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	ret = rk_aiq_uapi_setDarkAreaBoostStrth(g_aiq_ctx[cam_id], (int)(value / 10)); // [0, 10]

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:dark_boost_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

// white_blance
int rk_isp_get_white_blance_style(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_style", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_white_blance_style(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (!strcmp(value, "autoWhiteBalance")) {
		ret = rk_aiq_uapi_setWBMode(g_aiq_ctx[cam_id], OP_AUTO);
	} else if (!strcmp(value, "manualWhiteBalance")) {
		ret = rk_aiq_uapi_setWBMode(g_aiq_ctx[cam_id], OP_MANUAL);
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_style", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_white_blance_red(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_red", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_white_blance_red(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_wb_gain_t gain;
	opMode_t mode;

	rk_aiq_uapi_getWBMode(g_aiq_ctx[cam_id], &mode);
	if (mode == OP_AUTO) {
		LOG_WARN("white blance is auto, not support set gain\n");
		return 0;
	}
	rk_aiq_uapi_getMWBGain(g_aiq_ctx[cam_id], &gain);
	value = (value == 0) ? 1 : value;
	gain.rgain = value / 50.0f * gs_wb_gain.rgain; // [0, 100]->[1.0, 4.0]
	int ret = rk_aiq_uapi_setMWBGain(g_aiq_ctx[cam_id], &gain);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_red", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_white_blance_green(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_green", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_white_blance_green(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_wb_gain_t gain;
	opMode_t mode;

	rk_aiq_uapi_getWBMode(g_aiq_ctx[cam_id], &mode);
	if (mode == OP_AUTO) {
		LOG_WARN("white blance is auto, not support set gain\n");
		return 0;
	}
	rk_aiq_uapi_getMWBGain(g_aiq_ctx[cam_id], &gain);
	value = (value == 0) ? 1 : value;
	gain.grgain = value / 50.0f * gs_wb_gain.grgain; // [0, 100]->[1.0, 4.0]
	gain.gbgain = value / 50.0f * gs_wb_gain.gbgain; // [0, 100]->[1.0, 4.0]
	int ret = rk_aiq_uapi_setMWBGain(g_aiq_ctx[cam_id], &gain);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_green", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_white_blance_blue(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_blue", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_white_blance_blue(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_wb_gain_t gain;
	opMode_t mode;

	rk_aiq_uapi_getWBMode(g_aiq_ctx[cam_id], &mode);
	if (mode == OP_AUTO) {
		LOG_WARN("white blance is auto, not support set gain\n");
		return 0;
	}
	rk_aiq_uapi_getMWBGain(g_aiq_ctx[cam_id], &gain);
	value = (value == 0) ? 1 : value;
	gain.bgain = value / 50.0f * gs_wb_gain.bgain; // [0, 100]->[1.0, 4.0]
	int ret = rk_aiq_uapi_setMWBGain(g_aiq_ctx[cam_id], &gain);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_blue", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

// enhancement

int rk_isp_get_noise_reduce_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:noise_reduce_mode", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_noise_reduce_mode(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (!strcmp(value, "close")) {
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_NR, false);
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_TNR, false);
	} else if (!strcmp(value, "2dnr")) {
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_NR, true);
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_TNR, false);
		// rk_aiq_uapi_setMSpaNRStrth(g_aiq_ctx[cam_id], true, spatial_level);
		// //[0,100]
	} else if (!strcmp(value, "3dnr")) {
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_NR, false);
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_TNR, true);
		// rk_aiq_uapi_setMTNRStrth(g_aiq_ctx[cam_id], true, temporal_level);
		// //[0,100]
	} else if (!strcmp(value, "mixnr")) {
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_NR, true);
		rk_aiq_uapi_sysctl_setModuleCtl(g_aiq_ctx[cam_id], RK_MODULE_TNR, true);
		// rk_aiq_uapi_setMSpaNRStrth(g_aiq_ctx[cam_id], true, spatial_level);
		// //[0,100]
		// rk_aiq_uapi_setMTNRStrth(g_aiq_ctx[cam_id], true, temporal_level);
		// //[0,100]
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:noise_reduce_mode", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_dehaze(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:dehaze", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_dehaze(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (!strcmp(value, "close")) {
		rk_aiq_uapi_disableDhz(g_aiq_ctx[cam_id]);
	} else if (!strcmp(value, "open")) {
		rk_aiq_uapi_enableDhz(g_aiq_ctx[cam_id]);
		ret = rk_aiq_uapi_setDhzMode(g_aiq_ctx[cam_id], OP_MANUAL);
		// rk_aiq_uapi_setMDhzStrth(db_aiq_ctx, true, level);
	} else if (!strcmp(value, "auto")) {
		rk_aiq_uapi_enableDhz(g_aiq_ctx[cam_id]);
		ret = rk_aiq_uapi_setDhzMode(g_aiq_ctx[cam_id], OP_AUTO);
		// rk_aiq_uapi_setMDhzStrth(db_aiq_ctx, true, level);
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:dehaze", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_gray_scale_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:gray_scale_mode", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_gray_scale_mode(int cam_id, const char *value) {
	int ret;
	char entry[128] = {'\0'};
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// 1126 should set by venc
	// rk_aiq_uapi_acsm_attrib_t attr;
	// rk_aiq_user_api_acsm_GetAttrib(g_aiq_ctx[cam_id], &attr);
	// if (!strcmp(value, "[16-235]"))
	// 	attr.param.full_range = false;
	// else
	// 	attr.param.full_range = true;
	// rk_aiq_user_api_acsm_SetAttrib(g_aiq_ctx[cam_id], attr);
	snprintf(entry, 127, "isp.%d.enhancement:gray_scale_mode", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_distortion_correction(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:distortion_correction", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_distortion_correction(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (!strcmp(value, "close")) {
		rk_aiq_uapi_setFecEn(g_aiq_ctx[cam_id], false);
		rk_aiq_uapi_setLdchEn(g_aiq_ctx[cam_id], false);
	} else if (!strcmp(value, "FEC")) {
		rk_aiq_uapi_setFecEn(g_aiq_ctx[cam_id], true);
		rk_aiq_uapi_setLdchEn(g_aiq_ctx[cam_id], false);
		// rk_aiq_uapi_setFecCorrectLevel(g_aiq_ctx[cam_id], level); // [0-100] ->
		// [0->255]
	} else if (!strcmp(value, "LDCH")) {
		rk_aiq_uapi_setFecEn(g_aiq_ctx[cam_id], false);
		rk_aiq_uapi_setLdchEn(g_aiq_ctx[cam_id], true);
		// rk_aiq_uapi_setLdchCorrectLevel(g_aiq_ctx[cam_id], level); // [1, 100] ->
		// [2 , 255]
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:distortion_correction", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_spatial_denoise_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:spatial_denoise_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_spatial_denoise_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setMSpaNRStrth(g_aiq_ctx[cam_id], true, value); //[0,100]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:spatial_denoise_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_temporal_denoise_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:temporal_denoise_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_temporal_denoise_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setMTNRStrth(g_aiq_ctx[cam_id], true, value); //[0,100]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:temporal_denoise_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_dehaze_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:dehaze_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_dehaze_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setMDhzStrth(g_aiq_ctx[cam_id], true, value);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:dehaze_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_fec_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:fec_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_fec_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = rk_aiq_uapi_setFecCorrectLevel(g_aiq_ctx[cam_id],
	                                         (int)(value * 2.55)); // [0-100] -> [0->255]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:fec_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_ldch_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:ldch_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_ldch_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	value = value < 0 ? 0 : value;
	int set_value = (int)(value * 2.53 + 2);
	int ret =
	    rk_aiq_uapi_setLdchCorrectLevel(g_aiq_ctx[cam_id], set_value); // [0, 100] -> [2 , 255]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:ldch_level", cam_id);
	rk_param_set_int(entry, value);

	return ret;
}

// video_adjustment
int rk_isp_get_power_line_frequency_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.video_adjustment:power_line_frequency_mode", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_power_line_frequency_mode(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	if (!strcmp(value, "NTSC(60HZ)"))
		ret = rk_aiq_uapi_setExpPwrLineFreqMode(g_aiq_ctx[cam_id], EXP_PWR_LINE_FREQ_60HZ);
	else
		ret = rk_aiq_uapi_setExpPwrLineFreqMode(g_aiq_ctx[cam_id], EXP_PWR_LINE_FREQ_50HZ);
	snprintf(entry, 127, "isp.%d.video_adjustment:power_line_frequency_mode", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_image_flip(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.video_adjustment:image_flip", cam_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_image_flip(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret, mirror, flip;
	char entry[128] = {'\0'};
	if (!strcmp(value, "close")) {
		mirror = 0;
		flip = 0;
	}
	if (!strcmp(value, "flip")) {
		mirror = 0;
		flip = 1;
	}
	if (!strcmp(value, "mirror")) {
		mirror = 1;
		flip = 0;
	}
	if (!strcmp(value, "centrosymmetric")) {
		mirror = 1;
		flip = 1;
	}
	rk_aiq_uapi_setMirroFlip(g_aiq_ctx[cam_id], mirror, flip, 4); // skip 4 frame
	snprintf(entry, 127, "isp.%d.video_adjustment:image_flip", cam_id);
	rk_param_set_string(entry, value);

	return ret;
}

// auto focus

int rk_isp_get_af_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:af_mode", cam_id);
	*value = rk_param_get_string(entry, "auto");

	return 0;
}

int rk_isp_set_af_mode(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	char entry[128] = {'\0'};
	opMode_t af_mode = OP_AUTO;
	if (value == NULL)
		return -1;
	if (!strcmp(value, "auto")) {
		af_mode = OP_AUTO;
	} else if (!strcmp(value, "semi-auto")) {
		af_mode = OP_SEMI_AUTO;
	} else if (!strcmp(value, "manual")) {
		af_mode = OP_MANUAL;
	} else {
		return -1;
	}
	ret = rk_aiq_uapi_setFocusMode(g_aiq_ctx[cam_id], af_mode);
	LOG_INFO("set af mode: %s, ret: %d\n", value, ret);
	snprintf(entry, 127, "isp.%d.auto_focus:af_mode", cam_id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_isp_get_zoom_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:zoom_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_get_focus_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:focus_level", cam_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_af_zoom_change(int cam_id, int change) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	int code = 0;
	char entry[128] = {'\0'};

	rk_aiq_af_zoomrange af_zoom_range = {0};
	ret = rk_aiq_uapi_getZoomRange(g_aiq_ctx[cam_id], &af_zoom_range);
	if (ret) {
		LOG_ERROR("get zoom range fail: %d\n", ret);
		return ret;
	}
	rk_aiq_uapi_getOpZoomPosition(g_aiq_ctx[cam_id], &code);
	code += change;
	if ((code < af_zoom_range.min_pos) || (code > af_zoom_range.max_pos)) {
		LOG_ERROR("set zoom: %d over range [%d, %d]\n", code, af_zoom_range.min_pos,
		          af_zoom_range.max_pos);
		ret = -1;
	}
	ret = rk_aiq_uapi_setOpZoomPosition(g_aiq_ctx[cam_id], code);
	LOG_INFO("set zoom: %d, ret: %d\n", code, ret);
	snprintf(entry, 127, "isp.%d.auto_focus:zoom_level", cam_id);
	rk_param_set_int(entry, code);

	return ret;
}

int rk_isp_af_focus_change(int cam_id, int change) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	short code = 0;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:af_mode", cam_id);
	const char *af_mode = rk_param_get_string(entry, "auto");
	if (!strcmp(af_mode, "auto"))
		return 0;

	rk_aiq_af_focusrange af_focus_range = {0};
	ret = rk_aiq_uapi_getFocusRange(g_aiq_ctx[cam_id], &af_focus_range);
	if (ret) {
		LOG_ERROR("get focus range fail: %d\n", ret);
		return ret;
	}
	rk_aiq_uapi_getFixedModeCode(g_aiq_ctx[cam_id], &code);
	code += change;
	if ((code < af_focus_range.min_pos) || (code > af_focus_range.max_pos)) {
		LOG_ERROR("before set getFocusPosition: %d over range (%d, %d)\n", code,
		          af_focus_range.min_pos, af_focus_range.max_pos);
		return -1;
	}
	ret = rk_aiq_uapi_setFixedModeCode(g_aiq_ctx[cam_id], code);
	LOG_INFO("set setFocusPosition: %d, ret: %d\n", code, ret);
	snprintf(entry, 127, "isp.%d.auto_focus:focus_level", cam_id);
	rk_param_set_int(entry, code);

	return ret;
}

int rk_isp_af_zoom_in(int cam_id) { return rk_isp_af_zoom_change(cam_id, 20); }

int rk_isp_af_zoom_out(int cam_id) { return rk_isp_af_zoom_change(cam_id, -20); }

int rk_isp_af_focus_in(int cam_id) { return rk_isp_af_focus_change(cam_id, 1); }

int rk_isp_af_focus_out(int cam_id) { return rk_isp_af_focus_change(cam_id, -1); }

int rk_isp_af_focus_once(int cam_id) {
	LOG_INFO("af_focus_once\n");
	return rk_aiq_uapi_endOpZoomChange(g_aiq_ctx[cam_id]);
}

int rk_isp_set_from_ini(int cam_id) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	LOG_INFO("start\n");
	rk_isp_set_frame_rate(cam_id, rk_param_get_int("isp.0.adjustment:fps", 30));
	rk_isp_set_contrast(cam_id, rk_param_get_int("isp.0.adjustment:contrast", 50));
	rk_isp_set_brightness(cam_id, rk_param_get_int("isp.0.adjustment:brightness", 50));
	rk_isp_set_saturation(cam_id, rk_param_get_int("isp.0.adjustment:saturation", 50));
	rk_isp_set_sharpness(cam_id, rk_param_get_int("isp.0.adjustment:sharpness", 50));
	rk_isp_set_hue(cam_id, rk_param_get_int("isp.0.adjustment:hue", 50));
	LOG_INFO("end\n");

	return ret;
}

int rk_isp_init(int cam_id, char *iqfile_path) {
	LOG_INFO("%s, cam_id is %d\n", __func__, cam_id);
	int ret;
	char entry[128] = {'\0'};
	rk_aiq_working_mode_t mode = RK_AIQ_WORKING_MODE_NORMAL;
	if (iqfile_path)
		memcpy(g_iq_file_dir_, iqfile_path, strlen(iqfile_path));
	else
		memcpy(g_iq_file_dir_, "/etc/iqfiles", strlen("/etc/iqfiles"));
	LOG_INFO("g_iq_file_dir_ is %s\n", g_iq_file_dir_);

	snprintf(entry, 127, "isp.%d.blc:hdr", cam_id);
	const char *value = rk_param_get_string(entry, "close");
	LOG_INFO("hdr mode is %s\n", value);
	if (!strcmp(value, "close"))
		mode = RK_AIQ_WORKING_MODE_NORMAL;
	else if (!strcmp(value, "HDR2"))
		mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
	else if (!strcmp(value, "HDR3"))
		mode = RK_AIQ_WORKING_MODE_ISP_HDR3;

	ret = sample_common_isp_init(cam_id, mode, true, g_iq_file_dir_);
	ret |= sample_common_isp_run(cam_id);

	return ret;
}

int rk_isp_deinit(int cam_id) {
	LOG_INFO("%s\n", __func__);
	return sample_common_isp_stop(cam_id);
}
