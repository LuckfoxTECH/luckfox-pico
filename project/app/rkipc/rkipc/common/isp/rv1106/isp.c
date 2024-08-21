#include "isp.h"
#include "common.h"
#include "rk_gpio.h"
#include "rk_pwm.h"
#include "video.h"

#include <rk_aiq_user_api2_acgc.h>
#include <rk_aiq_user_api2_camgroup.h>
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>
#include <sys/mman.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "isp.c"

#define MAX_SCENARIO_NUM 2
#define MAX_AIQ_CTX 8

#define MAP_MASK (sysconf(_SC_PAGE_SIZE) - 1)
#define MAP_SIZE_COLOR_MODE (sizeof(int32_t))

char g_iq_file_dir_[256];
char main_scene[32];
char sub_scene[32];
static int light_level = -1;
static int light_state = -1;
static int current_scenario_id = 0;
static int fastboot_fd, file_size;
static void *iq_mem;
static int rkipc_aiq_use_group = 0;
static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
static rk_aiq_camgroup_ctx_t *g_camera_group_ctx[MAX_AIQ_CTX];
rk_aiq_working_mode_t g_WDRMode[MAX_AIQ_CTX];
rk_aiq_wb_gain_t gs_wb_gain = {2.083900, 1.000000, 1.000000, 2.018500};

#define RK_ISP_CHECK_CAMERA_ID(CAMERA_ID)                                                          \
	do {                                                                                           \
		if (rkipc_aiq_use_group) {                                                                 \
			if (CAMERA_ID >= MAX_AIQ_CTX || !g_camera_group_ctx[CAMERA_ID]) {                      \
				LOG_ERROR("camera_group_id is over 3 or not init\n");                              \
				return -1;                                                                         \
			}                                                                                      \
		} else {                                                                                   \
			if (CAMERA_ID >= MAX_AIQ_CTX || !g_aiq_ctx[CAMERA_ID]) {                               \
				LOG_ERROR("camera_id is over 3 or not init\n");                                    \
				return -1;                                                                         \
			}                                                                                      \
		}                                                                                          \
	} while (0)

#define RK_ISP_CHECK_NORMAL_MODE(CAMERA_ID)                                                        \
	do {                                                                                           \
		if (g_WDRMode[cam_id] != RK_AIQ_WORKING_MODE_NORMAL) {                                     \
			LOG_ERROR("Not support in HDR mode\n");                                                \
			return 0;                                                                              \
		}                                                                                          \
	} while (0)

rk_aiq_sys_ctx_t *rkipc_aiq_get_ctx(int cam_id) {
	if (rkipc_aiq_use_group)
		return (rk_aiq_sys_ctx_t *)g_camera_group_ctx[cam_id];

	return g_aiq_ctx[cam_id];
}

int rkipc_get_scenario_id(int cam_id) {
	int scenario_id = cam_id * MAX_SCENARIO_NUM + current_scenario_id;
	return scenario_id;
}

int sample_common_isp_init(int cam_id, rk_aiq_working_mode_t WDRMode, bool MultiCam,
                           const char *iq_file_dir) {
	if (cam_id >= MAX_AIQ_CTX) {
		LOG_ERROR("%s : cam_id is over 3\n", __FUNCTION__);
		return -1;
	}
	setlinebuf(stdout);
	if (iq_file_dir == NULL) {
		LOG_ERROR("rk_isp_init : not start.\n");
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
	rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(cam_id, &aiq_static_info);
	if (aiq_static_info.sensor_info.phyId == -1) {
		LOG_INFO("WARN: aiq_static_info.sensor_info.phyId is %d\n",
		         aiq_static_info.sensor_info.phyId);
	}

	LOG_INFO("ID: %d, sensor_name is %s, iqfiles is %s\n", cam_id,
	         aiq_static_info.sensor_info.sensor_name, iq_file_dir);

	int ret;
	rk_aiq_uapi2_sysctl_preInit_devBufCnt(aiq_static_info.sensor_info.sensor_name, "rkraw_rx", 2);

	if (WDRMode == RK_AIQ_WORKING_MODE_NORMAL)
		strcpy(main_scene, "normal");
	else
		strcpy(main_scene, "hdr");

	LOG_INFO("main_scene is %s, sub_scene is %s\n", main_scene, sub_scene);
	LOG_INFO("%s: rk_aiq_uapi2_sysctl_preInit_scene begin\n", get_time_string());
	ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name, main_scene,
	                                        sub_scene);
	if (ret < 0) {
		LOG_ERROR("%s: failed to set scene\n", aiq_static_info.sensor_info.sensor_name);
		return -1;
	}
	LOG_INFO("%s: rk_aiq_uapi2_sysctl_preInit_scene over\n", get_time_string());

	aiq_ctx =
	    rk_aiq_uapi2_sysctl_init(aiq_static_info.sensor_info.sensor_name, iq_file_dir, NULL, NULL);
	LOG_INFO("%s: rk_aiq_uapi2_sysctl_init over\n", get_time_string());
	if (!aiq_ctx) {
		LOG_ERROR("%s: failed to rk_aiq_uapi2_sysctl_init \n", __func__);
		return -1;
	}
	// if (MultiCam)
	// 	rk_aiq_uapi2_sysctl_setMulCamConc(aiq_ctx, true);

	g_aiq_ctx[cam_id] = aiq_ctx;

	return 0;
}

int sample_common_isp_run(int cam_id) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (rk_aiq_uapi2_sysctl_prepare(g_aiq_ctx[cam_id], 0, 0, g_WDRMode[cam_id])) {
		LOG_ERROR("rk_aiq_uapi2_sysctl_prepare failed !\n");
		g_aiq_ctx[cam_id] = NULL;
		return -1;
	}
	LOG_INFO("%s: rk_aiq_uapi2_sysctl_prepare succeed\n", get_time_string());
	if (rk_aiq_uapi2_sysctl_start(g_aiq_ctx[cam_id])) {
		LOG_ERROR("rk_aiq_uapi2_sysctl_start  failed\n");
		return -1;
	}
	LOG_INFO("%s: rk_aiq_uapi2_sysctl_start succeed\n", get_time_string());
	return 0;
}

int isp_camera_group_init(int cam_group_id, rk_aiq_working_mode_t WDRMode, bool MultiCam,
                          const char *iq_file_dir) {
	int ret;
	rk_aiq_static_info_t aiq_static_info;
	char sensor_name_array[MAX_AIQ_CTX][128];
	rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
	memset(&camgroup_cfg, 0, sizeof(camgroup_cfg));

	camgroup_cfg.sns_num = rk_param_get_int("avs:sensor_num", 6);
	LOG_INFO("camgroup_cfg.sns_num is %d\n", camgroup_cfg.sns_num);
	for (int i = 0; i < camgroup_cfg.sns_num; i++) {
		rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(i, &aiq_static_info);
		LOG_INFO("cam_group_id:%d, cam_id: %d, sensor_name is %s, iqfiles is %s\n", cam_group_id, i,
		         aiq_static_info.sensor_info.sensor_name, iq_file_dir);
		memcpy(sensor_name_array[i], aiq_static_info.sensor_info.sensor_name,
		       strlen(aiq_static_info.sensor_info.sensor_name) + 1);
		camgroup_cfg.sns_ent_nm_array[i] = sensor_name_array[i];
		LOG_INFO("camgroup_cfg.sns_ent_nm_array[%d] is %s\n", i, camgroup_cfg.sns_ent_nm_array[i]);
		rk_aiq_uapi2_sysctl_preInit_devBufCnt(aiq_static_info.sensor_info.sensor_name, "rkraw_rx",
		                                      2);
		if (WDRMode == RK_AIQ_WORKING_MODE_NORMAL)
			ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name,
			                                        "normal", "day");
		else
			ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name, "hdr",
			                                        "day");
		if (ret < 0)
			LOG_ERROR("%s: failed to set scene\n", aiq_static_info.sensor_info.sensor_name);
	}

	camgroup_cfg.config_file_dir = iq_file_dir;
	g_camera_group_ctx[cam_group_id] = rk_aiq_uapi2_camgroup_create(&camgroup_cfg);
	if (!g_camera_group_ctx[cam_group_id]) {
		LOG_ERROR("create camgroup ctx error!\n");
		return -1;
	}
	LOG_INFO("rk_aiq_uapi2_camgroup_create over\n");
	ret = rk_aiq_uapi2_camgroup_prepare(g_camera_group_ctx[cam_group_id], WDRMode);
	LOG_INFO("rk_aiq_uapi2_camgroup_prepare over\n");
	ret |= rk_aiq_uapi2_camgroup_start(g_camera_group_ctx[cam_group_id]);
	LOG_INFO("rk_aiq_uapi2_camgroup_start over\n");

	return ret;
}

int isp_camera_group_stop(int cam_group_id) {
	RK_ISP_CHECK_CAMERA_ID(cam_group_id);
	LOG_INFO("rk_aiq_uapi2_camgroup_stop enter\n");
	rk_aiq_uapi2_camgroup_stop(g_camera_group_ctx[cam_group_id]);
	LOG_INFO("rk_aiq_uapi2_camgroup_destroy enter\n");
	rk_aiq_uapi2_camgroup_destroy(g_camera_group_ctx[cam_group_id]);
	LOG_INFO("rk_aiq_uapi2_camgroup_destroy exit\n");
	g_camera_group_ctx[cam_group_id] = NULL;

	return 0;
}

int rk_isp_get_frame_rate(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:fps", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
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

int rk_isp_set_light_strength(uint32_t pwm, uint32_t period, uint32_t duty,
                              enum pwm_polarity polarity) {
	int ret;

	ret = rk_pwm_init(pwm, period, duty, polarity);
	if (ret) {
		LOG_ERROR("pwm%d init failed %d\n", pwm, ret);
		light_state = 0;
		return ret;
	}
	light_state = 1;
	ret = rk_pwm_set_enable(pwm, true);
}

int rk_isp_close_light(uint32_t pwm) {
	int ret;
	light_state = 0;
	ret = rk_pwm_deinit(pwm);
	if (ret)
		LOG_ERROR("pwm%d deinit failed %d\n", pwm, ret);
}

int rk_isp_set_frame_rate(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	Uapi_ExpSwAttrV2_t expSwAttr;
	LOG_DEBUG("start %d\n", value);
	ret = rk_aiq_user_api2_ae_getExpSwAttr(rkipc_aiq_get_ctx(cam_id), &expSwAttr);
	if (value == 0) {
		expSwAttr.stAuto.stFrmRate.isFpsFix = false;
	} else {
		expSwAttr.stAuto.stFrmRate.isFpsFix = true;
		expSwAttr.stAuto.stFrmRate.FpsValue = value;
	}
	ret = rk_aiq_user_api2_ae_setExpSwAttr(rkipc_aiq_get_ctx(cam_id), expSwAttr);
	LOG_DEBUG("end, %d\n", value);

	snprintf(entry, 127, "isp.%d.adjustment:fps", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return 0;
}

int rk_isp_set_frame_rate_without_ini(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	Uapi_ExpSwAttrV2_t expSwAttr;
	LOG_DEBUG("start %d\n", value);
	ret = rk_aiq_user_api2_ae_getExpSwAttr(rkipc_aiq_get_ctx(cam_id), &expSwAttr);
	if (value == 0) {
		expSwAttr.stAuto.stFrmRate.isFpsFix = false;
	} else {
		expSwAttr.stAuto.stFrmRate.isFpsFix = true;
		expSwAttr.stAuto.stFrmRate.FpsValue = value;
	}

	ret = rk_aiq_user_api2_ae_setExpSwAttr(rkipc_aiq_get_ctx(cam_id), expSwAttr);
	LOG_DEBUG("end, %d\n", value);

	return 0;
}

// isp scenario

int rk_isp_get_scenario(int cam_id, const char **value) {
	*value = rk_param_get_string("isp:scenario", NULL);

	return 0;
}

int rk_isp_set_scenario(int cam_id, const char *value) {
	if (!strcmp(value, "normal")) {
		current_scenario_id = 0;
		strcpy(sub_scene, rk_param_get_string("isp:normal_scene", "day"));
	} else if (!strcmp(value, "custom1")) {
		current_scenario_id = 1;
		strcpy(sub_scene, rk_param_get_string("isp:custom1_scene", "night"));
	}
	LOG_INFO("main_scene is %s, sub_scene is %s\n", main_scene, sub_scene);
	rk_aiq_uapi2_sysctl_switch_scene(rkipc_aiq_get_ctx(cam_id), main_scene, sub_scene);

	if (rk_param_get_int("isp:init_form_ini", 1))
		rk_isp_set_from_ini(0);
	rk_param_set_string("isp:scenario", value);

	return 0;
}

// image adjustment

int rk_isp_get_contrast(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:contrast", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_contrast(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	acp_attrib_t attrib;
	ret = rk_aiq_user_api2_acp_GetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	attrib.contrast = value * 2.55; // value[0,255]
	ret |= rk_aiq_user_api2_acp_SetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	snprintf(entry, 127, "isp.%d.adjustment:contrast", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_brightness(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:brightness", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);
	return 0;
}

int rk_isp_set_brightness(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	acp_attrib_t attrib;
	ret = rk_aiq_user_api2_acp_GetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	attrib.brightness = value * 2.55; // value[0,255]
	ret |= rk_aiq_user_api2_acp_SetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	snprintf(entry, 127, "isp.%d.adjustment:brightness", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_saturation(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:saturation", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_saturation(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	acp_attrib_t attrib;
	ret = rk_aiq_user_api2_acp_GetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	attrib.saturation = value * 2.55; // value[0,255]
	ret |= rk_aiq_user_api2_acp_SetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	snprintf(entry, 127, "isp.%d.adjustment:saturation", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_sharpness(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:sharpness", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_sharpness(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	float fPercent = 0.0f;
	fPercent = value / 100.0f;
	rk_aiq_sharp_strength_v33_t sharpV33Strength;
	sharpV33Strength.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
	sharpV33Strength.percent = fPercent;
	sharpV33Strength.strength_enable = true;
	ret = rk_aiq_user_api2_asharpV33_SetStrength(rkipc_aiq_get_ctx(cam_id), &sharpV33Strength);
	if (ret) {
		LOG_ERROR("rk_isp_set_sharpness failed %d", ret);
		return ret;
	}
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:sharpness", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_hue(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.adjustment:hue", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_hue(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	acp_attrib_t attrib;
	ret = rk_aiq_user_api2_acp_GetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	attrib.hue = value * 2.55; // value[0,255]
	ret |= rk_aiq_user_api2_acp_SetAttrib(rkipc_aiq_get_ctx(cam_id), &attrib);
	snprintf(entry, 127, "isp.%d.adjustment:hue", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

// exposure
int rk_isp_get_exposure_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_mode", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_exposure_mode(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	Uapi_ExpSwAttrV2_t expSwAttr;
	rk_aiq_user_api2_ae_getExpSwAttr(rkipc_aiq_get_ctx(cam_id), &expSwAttr);
	if (!strcmp(value, "auto")) {
		expSwAttr.AecOpType = RK_AIQ_OP_MODE_AUTO;
	} else {
		if (g_WDRMode[cam_id] != RK_AIQ_WORKING_MODE_NORMAL) {
			expSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
			expSwAttr.stManual.HdrAE.ManualGainEn = true;
			expSwAttr.stManual.HdrAE.ManualTimeEn = true;
		} else {
			expSwAttr.AecOpType = RK_AIQ_OP_MODE_MANUAL;
			expSwAttr.stManual.LinearAE.ManualGainEn = true;
			expSwAttr.stManual.LinearAE.ManualTimeEn = true;
		}
	}
	int ret = rk_aiq_user_api2_ae_setExpSwAttr(rkipc_aiq_get_ctx(cam_id), expSwAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_mode", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return 0;
}

int rk_isp_get_gain_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:gain_mode", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_gain_mode(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	Uapi_ExpSwAttrV2_t stExpSwAttr;

	rk_aiq_user_api2_ae_getExpSwAttr(rkipc_aiq_get_ctx(cam_id), &stExpSwAttr);
	if (!strcmp(value, "auto")) {
		stExpSwAttr.stManual.LinearAE.ManualGainEn = false;
		stExpSwAttr.stManual.HdrAE.ManualGainEn = false;
	} else {
		stExpSwAttr.stManual.LinearAE.ManualGainEn = true;
		stExpSwAttr.stManual.HdrAE.ManualGainEn = true;
	}
	int ret = rk_aiq_user_api2_ae_setExpSwAttr(rkipc_aiq_get_ctx(cam_id), stExpSwAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:gain_mode", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_exposure_time(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_time", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_exposure_time(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	Uapi_ExpSwAttrV2_t stExpSwAttr;
	float den, num, result;
	if (strchr(value, '/') == NULL) {
		den = 1;
		sscanf(value, "%f", &result);
	} else {
		sscanf(value, "%f/%f", &num, &den);
		result = num / den;
	}
	rk_aiq_user_api2_ae_getExpSwAttr(rkipc_aiq_get_ctx(cam_id), &stExpSwAttr);
	stExpSwAttr.stManual.LinearAE.TimeValue = result;
	stExpSwAttr.stManual.HdrAE.TimeValue[0] = result;
	stExpSwAttr.stManual.HdrAE.TimeValue[1] = result;
	stExpSwAttr.stManual.HdrAE.TimeValue[2] = result;
	int ret = rk_aiq_user_api2_ae_setExpSwAttr(rkipc_aiq_get_ctx(cam_id), stExpSwAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_time", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_exposure_gain(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.exposure:exposure_gain", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_exposure_gain(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	Uapi_ExpSwAttrV2_t stExpSwAttr;
	float gain_set = (value * 1.0f);
	rk_aiq_user_api2_ae_getExpSwAttr(rkipc_aiq_get_ctx(cam_id), &stExpSwAttr);
	if ((stExpSwAttr.stManual.LinearAE.ManualGainEn = false) ||
	    (stExpSwAttr.stManual.HdrAE.ManualGainEn = false)) {
		LOG_WARN("exposure mode is auto, not support set gain\n");
		return 0;
	}
	stExpSwAttr.stManual.LinearAE.GainValue = gain_set;
	stExpSwAttr.stManual.HdrAE.GainValue[0] = gain_set;
	stExpSwAttr.stManual.HdrAE.GainValue[1] = gain_set;
	stExpSwAttr.stManual.HdrAE.GainValue[2] = gain_set;
	int ret = rk_aiq_user_api2_ae_setExpSwAttr(rkipc_aiq_get_ctx(cam_id), stExpSwAttr);

	snprintf(entry, 127, "isp.%d.exposure:exposure_gain", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

// night_to_day
int rk_isp_get_night_to_day(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_night_to_day(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	aie_attrib_t attr;
	rk_aiq_user_api2_aie_GetAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	if (!strcmp(value, "night")) {
		attr.mode = RK_AIQ_IE_EFFECT_BW;
		rk_aiq_user_api2_aie_SetAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
		rk_isp_enable_ircut(false);
	} else {
		rk_isp_enable_ircut(true);
		if (light_state == 1)
			rk_isp_close_light(3);
		attr.mode = RK_AIQ_IE_EFFECT_NONE;
		rk_aiq_user_api2_aie_SetAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_fill_light_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:fill_light_mode", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_fill_light_mode(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
#if 0
	rk_aiq_cpsl_cfg_t cpsl_cfg;
	if (!strcmp(value, "IR")) {
		cpsl_cfg.lght_src = RK_AIQ_CPSLS_IR;
	} else if (!strcmp(value, "LED")) {
		cpsl_cfg.lght_src = RK_AIQ_CPSLS_LED;
	}
	ret = rk_aiq_uapi2_sysctl_setCpsLtCfg(rkipc_aiq_get_ctx(cam_id), &cpsl_cfg);
#endif
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:fill_light_mode", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_light_brightness(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:light_brightness", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_light_brightness(int cam_id, int value) {
	if (light_level == value) {
		LOG_INFO("light brightness unchanged\n");
		return 0;
	}
	int ret;
	uint32_t pwm, period, duty = 0;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	pwm = 3;
	period = 10000;
	duty = 5000;
	ret = rk_isp_set_light_strength(pwm, period, duty, PWM_POLARITY_NORMAL);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:light_brightness", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);
	light_level = value;
	return ret;
}

int rk_isp_get_night_to_day_filter_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_level",
	         rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_night_to_day_filter_level(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// TODO
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_level",
	         rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_night_to_day_filter_time(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_time",
	         rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_night_to_day_filter_time(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// TODO
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day_filter_time",
	         rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

// blc
int rk_isp_get_hdr(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_hdr(int cam_id, const char *value) {
	int ret = 0;
	int pipe_id, vi_chn_id;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	const char *old_value = NULL;
	rk_isp_get_hdr(cam_id, &old_value);
	LOG_INFO("cam_id is %d, value is %s, old_value is %s\n", cam_id, value, old_value);
	snprintf(entry, 127, "isp.%d.blc:hdr", rkipc_get_scenario_id(cam_id));
	int enable_npu = rk_param_get_int("video.source:enable_npu", 0);
	int enable_venc_0 = rk_param_get_int("video.source:enable_venc_0", 1);
	int enable_venc_1 = rk_param_get_int("video.source:enable_venc_1", 1);
	int enable_pp = rk_param_get_int("video.source:enable_pp", 1);
	if (strcmp(value, old_value)) {
		if (enable_venc_0)
			RK_MPI_VI_PauseChn(0, 0);
		if (enable_venc_1)
			RK_MPI_VI_PauseChn(0, 1);
		if (enable_npu)
			RK_MPI_VI_PauseChn(0, 2);
		if (enable_pp)
			RK_MPI_VI_PauseChn(0, 3);
		rk_isp_deinit(0);
		rk_param_set_string(entry, value);
		// usleep(100 * 1000);
		rk_isp_init(0, g_iq_file_dir_);
		if (rk_param_get_int("isp:init_form_ini", 1))
			ret |= rk_isp_set_from_ini(0);
		if (enable_venc_0)
			RK_MPI_VI_ResumeChn(0, 0);
		if (enable_venc_1)
			RK_MPI_VI_ResumeChn(0, 1);
		if (enable_npu)
			RK_MPI_VI_ResumeChn(0, 2);
		if (enable_pp)
			RK_MPI_VI_ResumeChn(0, 3);
	}

	return ret;
}

int rk_isp_get_blc_region(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_region", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_blc_region(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	RK_ISP_CHECK_NORMAL_MODE(cam_id);
	Uapi_LinExpAttrV2_t LineExpAttr;

	ret = rk_aiq_user_api2_ae_getLinExpAttr(rkipc_aiq_get_ctx(cam_id), &LineExpAttr);
	if (!strcmp(value, "close"))
		LineExpAttr.Params.BackLightCtrl.Enable = 0;
	else
		LineExpAttr.Params.BackLightCtrl.Enable = 1;
	LineExpAttr.Params.BackLightCtrl.MeasArea = AECV2_MEASURE_AREA_AUTO;
	LineExpAttr.Params.BackLightCtrl.StrBias = 0;
	ret = rk_aiq_user_api2_ae_setLinExpAttr(rkipc_aiq_get_ctx(cam_id), LineExpAttr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_region", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_hlc(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_hlc(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	RK_ISP_CHECK_NORMAL_MODE(cam_id);
	Uapi_LinExpAttrV2_t LinExpAttr;

	ret = rk_aiq_user_api2_ae_getLinExpAttr(rkipc_aiq_get_ctx(cam_id), &LinExpAttr);
	if (ret)
		LOG_ERROR("get exp attr failed\n");
	if (!strcmp(value, "close"))
		LinExpAttr.Params.OverExpCtrl.Enable = 0;
	else
		LinExpAttr.Params.OverExpCtrl.Enable = 1;
	LinExpAttr.Params.OverExpCtrl.StrBias = 0;
	ret = rk_aiq_user_api2_ae_setLinExpAttr(rkipc_aiq_get_ctx(cam_id), LinExpAttr);
	if (ret)
		LOG_ERROR("set exp attr failed\n");
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_hdr_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_hdr_level(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_uapi2_setDrcGain(rkipc_aiq_get_ctx(cam_id), (float)value, 0.1, 16); // Gain: [1, 8]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_blc_strength(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_strength", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_blc_strength(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	RK_ISP_CHECK_NORMAL_MODE(cam_id);
	Uapi_LinExpAttrV2_t LineExpAttr;

	ret = rk_aiq_user_api2_ae_getLinExpAttr(rkipc_aiq_get_ctx(cam_id), &LineExpAttr);
	if (ret)
		LOG_ERROR("getLinExpAttr error\n");
	if (LineExpAttr.Params.BackLightCtrl.Enable == 0) {
		LOG_ERROR("blc mode is not enabled\n");
		return 0;
	}
	LineExpAttr.Params.BackLightCtrl.StrBias = value;
	ret = rk_aiq_user_api2_ae_setLinExpAttr(rkipc_aiq_get_ctx(cam_id), LineExpAttr);
	if (ret)
		LOG_ERROR("setLinExpAttr error\n");
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:blc_strength", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_hlc_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_hlc_level(int cam_id, int value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	RK_ISP_CHECK_NORMAL_MODE(cam_id);
	Uapi_LinExpAttrV2_t LineExpAttr;

	if (value == 0)
		value = 1;
	ret = rk_aiq_user_api2_ae_getLinExpAttr(rkipc_aiq_get_ctx(cam_id), &LineExpAttr);
	if (ret)
		LOG_ERROR("getLinExpAttr error\n");
	if (LineExpAttr.Params.OverExpCtrl.Enable == 0) {
		LOG_ERROR("hlc mode is not enabled\n");
		return 0;
	}
	LineExpAttr.Params.OverExpCtrl.StrBias = value;
	ret = rk_aiq_user_api2_ae_setLinExpAttr(rkipc_aiq_get_ctx(cam_id), LineExpAttr);
	if (ret)
		LOG_ERROR("setLinExpAttr error\n");
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hlc_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_dark_boost_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:dark_boost_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_dark_boost_level(int cam_id, int value) {
	int ret;
	float Alpha = 0.1;
	float Clip = 16.0;
	float Gain = ((value / 14.3f + 1) * 1.0f);
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	ret = rk_aiq_uapi2_setDrcGain(rkipc_aiq_get_ctx(cam_id), Gain, Alpha, Clip); // [0,100]→[1,8]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:dark_boost_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

// white_blance
int rk_isp_get_white_blance_style(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_style", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_white_blance_style(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_uapiV2_wb_opMode_t attr;

	if (!strcmp(value, "manualWhiteBalance")) {
		attr.mode = RK_AIQ_WB_MODE_MANUAL;
	} else {
		attr.mode = RK_AIQ_WB_MODE_AUTO;
	}
	ret = rk_aiq_user_api2_awb_SetWpModeAttrib(rkipc_aiq_get_ctx(cam_id), attr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_style", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_white_blance_red(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_red", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_white_blance_red(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	rk_aiq_uapiV2_wb_opMode_t mode_attr;
	rk_aiq_wb_mwb_attrib_t wb_mwb_attr;

	rk_aiq_user_api2_awb_GetWpModeAttrib(rkipc_aiq_get_ctx(cam_id), &mode_attr);
	if (mode_attr.mode == RK_AIQ_WB_MODE_AUTO) {
		LOG_WARN("white blance is auto, not support set gain\n");
		return 0;
	}
	// get
	rk_aiq_user_api2_awb_GetMwbAttrib(rkipc_aiq_get_ctx(cam_id), &wb_mwb_attr);
	// modify
	wb_mwb_attr.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
	wb_mwb_attr.mode = RK_AIQ_MWB_MODE_WBGAIN;
	value = (value == 0) ? 1 : value;
	wb_mwb_attr.para.gain.rgain = value / 50.0f * gs_wb_gain.rgain;
	// set
	rk_aiq_user_api2_awb_SetMwbAttrib(rkipc_aiq_get_ctx(cam_id), wb_mwb_attr);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_red", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_white_blance_green(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_green", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_white_blance_green(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	rk_aiq_uapiV2_wb_opMode_t mode_attr;
	rk_aiq_wb_mwb_attrib_t wb_mwb_attr;

	rk_aiq_user_api2_awb_GetWpModeAttrib(rkipc_aiq_get_ctx(cam_id), &mode_attr);
	if (mode_attr.mode == RK_AIQ_WB_MODE_AUTO) {
		LOG_WARN("white blance is auto, not support set gain\n");
		return 0;
	}
	// get
	rk_aiq_user_api2_awb_GetMwbAttrib(rkipc_aiq_get_ctx(cam_id), &wb_mwb_attr);
	// modify
	wb_mwb_attr.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
	wb_mwb_attr.mode = RK_AIQ_MWB_MODE_WBGAIN;
	value = (value == 0) ? 1 : value;
	wb_mwb_attr.para.gain.grgain = value / 50.0f * gs_wb_gain.grgain;
	wb_mwb_attr.para.gain.gbgain = value / 50.0f * gs_wb_gain.gbgain;
	// set
	rk_aiq_user_api2_awb_SetMwbAttrib(rkipc_aiq_get_ctx(cam_id), wb_mwb_attr);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_green", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_white_blance_blue(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_blue", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_white_blance_blue(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	rk_aiq_uapiV2_wb_opMode_t mode_attr;
	rk_aiq_wb_mwb_attrib_t wb_mwb_attr;

	rk_aiq_user_api2_awb_GetWpModeAttrib(rkipc_aiq_get_ctx(cam_id), &mode_attr);
	if (mode_attr.mode == RK_AIQ_WB_MODE_AUTO) {
		LOG_WARN("white blance is auto, not support set gain\n");
		return 0;
	}
	// get
	rk_aiq_user_api2_awb_GetMwbAttrib(rkipc_aiq_get_ctx(cam_id), &wb_mwb_attr);
	// modify
	wb_mwb_attr.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
	wb_mwb_attr.mode = RK_AIQ_MWB_MODE_WBGAIN;
	value = (value == 0) ? 1 : value;
	wb_mwb_attr.para.gain.bgain = value / 50.0f * gs_wb_gain.bgain;
	// set
	rk_aiq_user_api2_awb_SetMwbAttrib(rkipc_aiq_get_ctx(cam_id), wb_mwb_attr);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_blue", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

// enhancement

int rk_isp_get_noise_reduce_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:noise_reduce_mode", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

// Turn off noise reduction, the actual default value is set to 50,
// and it is done in the interface of setting level
int rk_isp_set_noise_reduce_mode(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:noise_reduce_mode", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_dehaze(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:dehaze", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_dehaze(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	adehaze_sw_v12_t attr;
	memset(&attr, 0, sizeof(attr));

	ret = rk_aiq_user_api2_adehaze_v12_getSwAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	if (ret)
		LOG_ERROR("dehaze get SwAttrib failed %d", ret);
	attr.sync.sync_mode = RK_AIQ_UAPI_MODE_DEFAULT;
	attr.sync.done = false;
	if (!strcmp(value, "close")) {
		if (attr.mode == DEHAZE_API_AUTO) {
			attr.stAuto.DehazeTuningPara.dehaze_setting.en = false;
		} else if (attr.mode == DEHAZE_API_MANUAL) {
			attr.stManual.dehaze_setting.en = false;
		}
	} else if (!strcmp(value, "open")) {
		if (attr.mode == DEHAZE_API_AUTO) {
			attr.stAuto.DehazeTuningPara.Enable = true;
			attr.stAuto.DehazeTuningPara.dehaze_setting.en = true;
			attr.stAuto.DehazeTuningPara.enhance_setting.en = false;
			attr.stAuto.DehazeTuningPara.cfg_alpha = 1.0f;
		} else if (attr.mode == DEHAZE_API_MANUAL) {
			attr.stManual.Enable = true;
			attr.stManual.dehaze_setting.en = true;
			attr.stManual.enhance_setting.en = false;
			attr.stManual.cfg_alpha = 1.0f;
		}
		attr.Info.updateMDehazeStrth = true;
		attr.Info.MDehazeStrth = 50;
	} else if (!strcmp(value, "auto")) {
		if (attr.mode == DEHAZE_API_AUTO) {
			attr.stAuto.DehazeTuningPara.Enable = true;
			attr.stAuto.DehazeTuningPara.dehaze_setting.en = true;
			attr.stAuto.DehazeTuningPara.enhance_setting.en = false;
			attr.stAuto.DehazeTuningPara.cfg_alpha = 1.0f;
		} else if (attr.mode == DEHAZE_API_MANUAL) {
			attr.stManual.Enable = true;
			attr.stManual.dehaze_setting.en = true;
			attr.stManual.enhance_setting.en = false;
			attr.stManual.cfg_alpha = 1.0f;
		}
		attr.Info.updateMDehazeStrth = true;
		attr.Info.MDehazeStrth = 50;
	}
	ret = rk_aiq_user_api2_adehaze_v12_setSwAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	snprintf(entry, 127, "isp.%d.enhancement:dehaze", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_gray_scale_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:gray_scale_mode", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_gray_scale_mode(int cam_id, const char *value) {
	int ret;
	char entry[128] = {'\0'};
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_uapi_acgc_attrib_t attr;
	rk_aiq_user_api2_acgc_GetAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	if (!strcmp(value, "[16-235]"))
		attr.param.cgc_yuv_limit = false;
	else
		attr.param.cgc_yuv_limit = true;
	rk_aiq_user_api2_acgc_SetAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	snprintf(entry, 127, "isp.%d.enhancement:gray_scale_mode", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_distortion_correction(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:distortion_correction", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_distortion_correction(int cam_id, const char *value) {
	int ret;
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	rk_aiq_ldch_v21_attrib_t ldchAttr;
	ret = rk_aiq_user_api2_aldch_v21_GetAttrib(rkipc_aiq_get_ctx(cam_id), &ldchAttr);
	if (!strcmp(value, "close"))
		ldchAttr.en = false;
	else
		ldchAttr.en = true;
	ret = rk_aiq_user_api2_aldch_v21_SetAttrib(rkipc_aiq_get_ctx(cam_id), &ldchAttr);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:distortion_correction", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_spatial_denoise_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:spatial_denoise_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_spatial_denoise_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	const char *noise_reduce_mode;
	rk_aiq_ynr_strength_v22_t ynrStrength;
	rk_aiq_bayer2dnr_strength_v23_t bayer2dnrV23Strength;

	rk_isp_get_noise_reduce_mode(cam_id, &noise_reduce_mode);
	LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode, value);
	if ((!strcmp(noise_reduce_mode, "close")) || (!strcmp(noise_reduce_mode, "3dnr"))) {
		value = 50;
		LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode, value);
	}
	ynrStrength.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
	ynrStrength.percent = value / 100.0;
	ynrStrength.strength_enable = true;
	ret = rk_aiq_user_api2_aynrV22_SetStrength(rkipc_aiq_get_ctx(cam_id), &ynrStrength);
	bayer2dnrV23Strength.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
	bayer2dnrV23Strength.percent = value / 100.0;
	bayer2dnrV23Strength.strength_enable = true;
	ret = rk_aiq_user_api2_abayer2dnrV23_GetStrength(rkipc_aiq_get_ctx(cam_id),
	                                                 &bayer2dnrV23Strength);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:spatial_denoise_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_temporal_denoise_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:temporal_denoise_level",
	         rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_temporal_denoise_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	const char *noise_reduce_mode;
	rk_aiq_bayertnr_strength_v23_t bayertnrV23Strength;

	rk_isp_get_noise_reduce_mode(cam_id, &noise_reduce_mode);
	LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode, value);
	if ((!strcmp(noise_reduce_mode, "close")) || (!strcmp(noise_reduce_mode, "2dnr"))) {
		value = 50;
		LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode, value);
	}
	bayertnrV23Strength.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
	bayertnrV23Strength.percent = value / 100.0;
	bayertnrV23Strength.strength_enable = true;
	ret =
	    rk_aiq_user_api2_abayertnrV23_SetStrength(rkipc_aiq_get_ctx(cam_id), &bayertnrV23Strength);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:temporal_denoise_level",
	         rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_dehaze_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:dehaze_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_dehaze_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:dehaze", cam_id);
	const char *mode = rk_param_get_string(entry, "close");
	if (!strcmp(mode, "close"))
		return 0;

	adehaze_sw_v12_t attr;
	memset(&attr, 0, sizeof(attr));
	rk_aiq_user_api2_adehaze_v12_getSwAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	attr.sync.sync_mode = RK_AIQ_UAPI_MODE_DEFAULT;
	attr.sync.done = false;
	attr.Info.updateMDehazeStrth = true;
	attr.Info.MDehazeStrth = value * 10;
	ret = rk_aiq_user_api2_adehaze_v12_setSwAttrib(rkipc_aiq_get_ctx(cam_id), &attr);
	snprintf(entry, 127, "isp.%d.enhancement:dehaze_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_get_fec_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:fec_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_fec_level(int cam_id, int value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	// int ret = rk_aiq_uapi2_setFecCorrectLevel(g_aiq_ctx[cam_id],
	//                                          (int)(value * 2.55)); // [0-100] -> [0->255]
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:fec_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return 0;
}

int rk_isp_get_ldch_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:ldch_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_set_ldch_level(int cam_id, int value) {
	if (rk_param_get_int("isp:group_ldch", 0)) {
		LOG_INFO("group ldch open for avs, not set ldch level\n");
		return 0;
	}

	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	rk_aiq_ldch_v21_attrib_t ldchAttr;

	value = value < 0 ? 0 : value;
	ret = rk_aiq_user_api2_aldch_v21_GetAttrib(rkipc_aiq_get_ctx(cam_id), &ldchAttr);
	ldchAttr.correct_level = (int)(value * 2.53 + 2); // [0, 100] -> [2 , 255]
	ret = rk_aiq_user_api2_aldch_v21_SetAttrib(rkipc_aiq_get_ctx(cam_id), &ldchAttr);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.enhancement:ldch_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, value);

	return ret;
}

int rk_isp_set_group_ldch_level_form_file(int cam_id) {
	int ret;
	rk_aiq_camgroup_camInfos_t camInfos;
#if 0
	memset(&camInfos, 0, sizeof(camInfos));
	if (rk_aiq_uapi2_camgroup_getCamInfos(rkipc_aiq_get_ctx(cam_id), &camInfos) !=
	    XCAM_RETURN_NO_ERROR) {
		LOG_ERROR("rk_aiq_uapi2_camgroup_getCamInfos fail\n");
		return -1;
	}
	for (int i = 0; i < camInfos.valid_sns_num; i++) {
		rk_aiq_sys_ctx_t *aiq_ctx = NULL;
		rk_aiq_ldch_v21_attrib_t ldchAttr;
		memset(&ldchAttr, 0, sizeof(ldchAttr));
		aiq_ctx = rk_aiq_uapi2_camgroup_getAiqCtxBySnsNm(rkipc_aiq_get_ctx(cam_id),
		                                                 camInfos.sns_ent_nm[i]);
		if (!aiq_ctx)
			continue;
		LOG_INFO("aiq_ctx sns name: %s, camPhyId %d\n", camInfos.sns_ent_nm[i],
		         camInfos.sns_camPhyId[i]);
		ret = rk_aiq_user_api2_aldch_v21_GetAttrib(aiq_ctx, &ldchAttr);
		if (ret != XCAM_RETURN_NO_ERROR) {
			LOG_ERROR("rk_aiq_user_api2_aldch_v21_GetAttrib fail\n");
			return -1;
		}
		ldchAttr.en = true;
		ldchAttr.lut.flag = true;
		strcpy(ldchAttr.lut.config_file_dir,
		       rk_param_get_string("avs:middle_lut_path", "/oem/usr/share/middle_lut/5m/"));
		if (camInfos.sns_camPhyId[i] > 0) {
			strcpy(ldchAttr.lut.mesh_file, "cam1_ldch_mesh.bin");
		} else {
			strcpy(ldchAttr.lut.mesh_file, "cam0_ldch_mesh.bin");
		}

		LOG_INFO("sns name %s, camPhyId %d\n", camInfos.sns_ent_nm[i], camInfos.sns_camPhyId[i]);
		LOG_INFO("lut file_dir %s, mesh_file %s\n", ldchAttr.lut.config_file_dir,
		         ldchAttr.lut.mesh_file);

		ret = rk_aiq_user_api2_aldch_v21_SetAttrib(aiq_ctx, &ldchAttr);
		if (ret != XCAM_RETURN_NO_ERROR) {
			LOG_ERROR("Failed to set ldch attrib : %d\n", ret);
			return -1;
		}
	}
#endif
	return 0;
}

int rk_isp_set_group_ldch_level_form_buffer(int cam_id, void *ldch_0, void *ldch_1, int ldch_size_0,
                                            int ldch_size_1) {
	int ret;
	rk_aiq_camgroup_camInfos_t camInfos;
	memset(&camInfos, 0, sizeof(camInfos));
	if (rk_aiq_uapi2_camgroup_getCamInfos(rkipc_aiq_get_ctx(cam_id), &camInfos) !=
	    XCAM_RETURN_NO_ERROR) {
		LOG_ERROR("rk_aiq_uapi2_camgroup_getCamInfos fail\n");
		return -1;
	}
	LOG_INFO("camInfos.valid_sns_num is %d\n", camInfos.valid_sns_num);
	for (int i = 0; i < camInfos.valid_sns_num; i++) {
		rk_aiq_sys_ctx_t *aiq_ctx = NULL;
		rk_aiq_ldch_v21_attrib_t ldchAttr;
		memset(&ldchAttr, 0, sizeof(ldchAttr));
		aiq_ctx = rk_aiq_uapi2_camgroup_getAiqCtxBySnsNm(rkipc_aiq_get_ctx(cam_id),
		                                                 camInfos.sns_ent_nm[i]);
		if (!aiq_ctx)
			continue;
		LOG_INFO("aiq_ctx sns name: %s, camPhyId %d\n", camInfos.sns_ent_nm[i],
		         camInfos.sns_camPhyId[i]);
		ret = rk_aiq_user_api2_aldch_v21_GetAttrib(aiq_ctx, &ldchAttr);
		if (ret != XCAM_RETURN_NO_ERROR) {
			LOG_ERROR("rk_aiq_user_api2_aldch_v21_GetAttrib fail\n");
			return -1;
		}
		ldchAttr.en = true;
		ldchAttr.lut.update_flag = true;
		ldchAttr.update_lut_mode = RK_AIQ_LDCH_UPDATE_LUT_FROM_EXTERNAL_BUFFER;
		if (i == 0) {
			ldchAttr.lut.u.buffer.addr = ldch_0;
			ldchAttr.lut.u.buffer.size = ldch_size_0;
		} else {
			ldchAttr.lut.u.buffer.addr = ldch_1;
			ldchAttr.lut.u.buffer.size = ldch_size_1;
		}

		LOG_INFO("sns name %s, camPhyId %d\n", camInfos.sns_ent_nm[i], camInfos.sns_camPhyId[i]);

		ret = rk_aiq_user_api2_aldch_v21_SetAttrib(aiq_ctx, &ldchAttr);
		if (ret != XCAM_RETURN_NO_ERROR) {
			LOG_ERROR("Failed to set ldch attrib : %d\n", ret);
			return -1;
		}
	}

	return 0;
}

// video_adjustment
int rk_isp_get_power_line_frequency_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.video_adjustment:power_line_frequency_mode",
	         rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_power_line_frequency_mode(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret;
	char entry[128] = {'\0'};
	Uapi_ExpSwAttrV2_t expSwAttr;

	ret = rk_aiq_user_api2_ae_getExpSwAttr(rkipc_aiq_get_ctx(cam_id), &expSwAttr);
	if (!strcmp(value, "NTSC(60HZ)")) {
		expSwAttr.stAuto.stAntiFlicker.enable = true;
		expSwAttr.stAuto.stAntiFlicker.Frequency = AECV2_FLICKER_FREQUENCY_60HZ;
		expSwAttr.stAuto.stAntiFlicker.Mode = AECV2_ANTIFLICKER_NORMAL_MODE;
	} else {
		expSwAttr.stAuto.stAntiFlicker.enable = true;
		expSwAttr.stAuto.stAntiFlicker.Frequency = AECV2_FLICKER_FREQUENCY_50HZ;
		expSwAttr.stAuto.stAntiFlicker.Mode = AECV2_ANTIFLICKER_NORMAL_MODE;
	}
	ret = rk_aiq_user_api2_ae_setExpSwAttr(rkipc_aiq_get_ctx(cam_id), expSwAttr);
	if (ret != RK_SUCCESS)
		LOG_ERROR("rk_aiq_user_api2_ae_setExpSwAttr failed\n");
	snprintf(entry, 127, "isp.%d.video_adjustment:power_line_frequency_mode",
	         rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

int rk_isp_get_image_flip(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.video_adjustment:image_flip", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_isp_set_image_flip(int cam_id, const char *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	if (rkipc_aiq_use_group) {
		LOG_INFO("group mode, not support set mirror/flip\n");
		return 0;
	}
	int ret, mirror, flip;
	char entry[128] = {'\0'};
	if (!strcmp(value, "close")) {
		mirror = 0;
		flip = 0;
		if (access("/dev/block/by-name/meta", F_OK) == 0)
			system("make_meta --update --meta_path /dev/block/by-name/meta --rk_cam_mirror_flip 0");
	}
	if (!strcmp(value, "flip")) {
		mirror = 0;
		flip = 1;
		if (access("/dev/block/by-name/meta", F_OK) == 0)
			system("make_meta --update --meta_path /dev/block/by-name/meta --rk_cam_mirror_flip 1");
	}
	if (!strcmp(value, "mirror")) {
		mirror = 1;
		flip = 0;
		if (access("/dev/block/by-name/meta", F_OK) == 0)
			system("make_meta --update --meta_path /dev/block/by-name/meta --rk_cam_mirror_flip 2");
	}
	if (!strcmp(value, "centrosymmetric")) {
		mirror = 1;
		flip = 1;
		if (access("/dev/block/by-name/meta", F_OK) == 0)
			system("make_meta --update --meta_path /dev/block/by-name/meta --rk_cam_mirror_flip 3");
	}
	rk_aiq_uapi2_setMirrorFlip(rkipc_aiq_get_ctx(cam_id), mirror, flip, 4); // skip 4 frame
	snprintf(entry, 127, "isp.%d.video_adjustment:image_flip", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return ret;
}

// auto focus

int rk_isp_get_af_mode(int cam_id, const char **value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:af_mode", rkipc_get_scenario_id(cam_id));
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
	ret = rk_aiq_uapi2_setFocusMode(rkipc_aiq_get_ctx(cam_id), af_mode);
	LOG_INFO("set af mode: %s, ret: %d\n", value, ret);
	snprintf(entry, 127, "isp.%d.auto_focus:af_mode", rkipc_get_scenario_id(cam_id));
	rk_param_set_string(entry, value);

	return 0;
}

int rk_isp_get_zoom_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:zoom_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_get_focus_level(int cam_id, int *value) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:focus_level", rkipc_get_scenario_id(cam_id));
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_isp_af_zoom_change(int cam_id, int change) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	int code = 0;
	char entry[128] = {'\0'};

	rk_aiq_af_zoomrange af_zoom_range = {0};
	ret = rk_aiq_uapi2_getZoomRange(rkipc_aiq_get_ctx(cam_id), &af_zoom_range);
	if (ret) {
		LOG_ERROR("get zoom range fail: %d\n", ret);
		return ret;
	}
	rk_aiq_uapi2_getOpZoomPosition(rkipc_aiq_get_ctx(cam_id), &code);
	code += change;
	if ((code < af_zoom_range.min_pos) || (code > af_zoom_range.max_pos)) {
		LOG_ERROR("set zoom: %d over range [%d, %d]\n", code, af_zoom_range.min_pos,
		          af_zoom_range.max_pos);
		ret = -1;
	}
	ret = rk_aiq_uapi2_setOpZoomPosition(rkipc_aiq_get_ctx(cam_id), code);
	LOG_INFO("set zoom: %d, ret: %d\n", code, ret);
	snprintf(entry, 127, "isp.%d.auto_focus:zoom_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, code);

	return ret;
}

int rk_isp_af_focus_change(int cam_id, int change) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	short code = 0;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.auto_focus:af_mode", rkipc_get_scenario_id(cam_id));
	const char *af_mode = rk_param_get_string(entry, "auto");
	if (!strcmp(af_mode, "auto"))
		return 0;

	rk_aiq_af_focusrange af_focus_range = {0};
	ret = rk_aiq_uapi2_getFocusRange(rkipc_aiq_get_ctx(cam_id), &af_focus_range);
	if (ret) {
		LOG_ERROR("get focus range fail: %d\n", ret);
		return ret;
	}
	rk_aiq_uapi2_getFocusPosition(rkipc_aiq_get_ctx(cam_id), &code);
	code += change;
	if ((code < af_focus_range.min_pos) || (code > af_focus_range.max_pos)) {
		LOG_ERROR("before set getFocusPosition: %d over range (%d, %d)\n", code,
		          af_focus_range.min_pos, af_focus_range.max_pos);
		return -1;
	}
	ret = rk_aiq_uapi2_setFocusPosition(rkipc_aiq_get_ctx(cam_id), code);
	LOG_INFO("set setFocusPosition: %d, ret: %d\n", code, ret);
	snprintf(entry, 127, "isp.%d.auto_focus:focus_level", rkipc_get_scenario_id(cam_id));
	rk_param_set_int(entry, code);

	return ret;
}

int rk_isp_af_zoom_in(int cam_id) { return rk_isp_af_zoom_change(cam_id, 20); }

int rk_isp_af_zoom_out(int cam_id) { return rk_isp_af_zoom_change(cam_id, -20); }

int rk_isp_af_focus_in(int cam_id) { return rk_isp_af_focus_change(cam_id, 1); }

int rk_isp_af_focus_out(int cam_id) { return rk_isp_af_focus_change(cam_id, -1); }

int rk_isp_af_focus_once(int cam_id) {
	LOG_INFO("af_focus_once\n");
	return rk_aiq_uapi2_endOpZoomChange(rkipc_aiq_get_ctx(cam_id));
}

int rk_isp_fastboot_init(int cam_id) {
	RK_S32 s32chnlId = 0;
	int rk_color_mode, file_size, ret = 0;
	void *mem, *vir_addr, *vir_iqaddr;
	off_t rk_color_mode_addr, addr_iq;

	RK_S64 s64AiqInitStart = rkipc_get_curren_time_ms();
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

	rk_color_mode_addr = (off_t)get_cmd_val("rk_color_mode", 16);
	if ((fastboot_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		perror("open error");
		return -1;
	}

	mem = mmap(0, MAP_SIZE_COLOR_MODE, PROT_READ | PROT_WRITE, MAP_SHARED, fastboot_fd,
	           rk_color_mode_addr & ~(MAP_MASK));
	vir_addr = mem + (rk_color_mode_addr & (MAP_MASK));
	rk_color_mode = *((unsigned long *)vir_addr);
	if (mem != MAP_FAILED)
		munmap(mem, MAP_SIZE_COLOR_MODE);
	addr_iq = (off_t)get_cmd_val("rk_iqbin_addr", 16);
	file_size = (int)get_cmd_val("rk_iqbin_size", 16);
	iq_mem =
	    mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fastboot_fd, addr_iq & ~(MAP_MASK));
	if (iq_mem != MAP_FAILED) {
		printf("mmap iq ok\n");
	} else {
		printf("mmap iq failed\n");
	}
	vir_iqaddr = iq_mem + (addr_iq & MAP_MASK);

	rk_aiq_static_info_t aiq_static_info;
	rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(s32chnlId, &aiq_static_info);

	if (rk_color_mode) {
		LOG_INFO("=====night mode=====\n");
		ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name, "normal",
		                                        "night");
		if (ret < 0) {
			LOG_ERROR("%s: failed to set night scene\n", aiq_static_info.sensor_info.sensor_name);
			return -1;
		}
	} else {
		LOG_INFO("=====day mode=======\n");
		ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name, "normal",
		                                        "day");
		if (ret < 0) {
			LOG_ERROR("%s: failed to set day scene\n", aiq_static_info.sensor_info.sensor_name);
			return -1;
		}
	}

	ret = rk_aiq_uapi2_sysctl_preInit_iq_addr(aiq_static_info.sensor_info.sensor_name, vir_iqaddr,
	                                          (size_t *)file_size);
	if (ret < 0) {
		LOG_ERROR("%s: failed to load binary iqfiles\n", aiq_static_info.sensor_info.sensor_name);
	}

	rk_aiq_tb_info_t tb_info;
	memset(&tb_info, 0, sizeof(rk_aiq_tb_info_t));
	tb_info.magic = sizeof(rk_aiq_tb_info_t) - 2;
	tb_info.is_pre_aiq = false;
	tb_info.prd_type = RK_AIQ_PRD_TYPE_TB_BATIPC;
	rk_aiq_uapi2_sysctl_preInit_tb_info(aiq_static_info.sensor_info.sensor_name, &tb_info);
	g_aiq_ctx[cam_id] = rk_aiq_uapi2_sysctl_init(aiq_static_info.sensor_info.sensor_name,
	                                             "/etc/iqfiles/", NULL, NULL);
	if (g_aiq_ctx[cam_id] == NULL) {
		LOG_ERROR("%s: failed to init aiq\n", aiq_static_info.sensor_info.sensor_name);
		return -1;
	}

	if (rk_aiq_uapi2_sysctl_prepare(g_aiq_ctx[cam_id], 0, 0, hdr_mode)) {
		LOG_ERROR("rkaiq engine prepare failed !\n");
		return -1;
	}
	if (rk_aiq_uapi2_sysctl_start(g_aiq_ctx[cam_id])) {
		LOG_ERROR("rk_aiq_uapi2_sysctl_start  failed\n");
		return -1;
	}
	LOG_INFO("aiq start\n");
	LOG_INFO("Aiq:%lld ms\n", rkipc_get_curren_time_ms() - s64AiqInitStart);

	return 0;
}

int rk_isp_fastboot_deinit(int cam_id) {
	if (fastboot_fd > 0)
		close(fastboot_fd);
	if (iq_mem != MAP_FAILED)
		munmap(iq_mem, file_size);

	rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[cam_id], false);
	rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[cam_id]);
}

int rk_isp_set_from_ini(int cam_id) {
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	int ret = 0;
	char value[128];
	char entry[128] = {'\0'};
	LOG_DEBUG("start\n");
	snprintf(entry, 127, "isp.%d.adjustment:fps", rkipc_get_scenario_id(cam_id));
	rk_isp_set_frame_rate(cam_id, rk_param_get_int(entry, 30));
	// image adjustment
	LOG_DEBUG("image adjustment\n");
	snprintf(entry, 127, "isp.%d.adjustment:contrast", rkipc_get_scenario_id(cam_id));
	rk_isp_set_contrast(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.adjustment:brightness", rkipc_get_scenario_id(cam_id));
	rk_isp_set_brightness(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.adjustment:saturation", rkipc_get_scenario_id(cam_id));
	rk_isp_set_saturation(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.adjustment:sharpness", rkipc_get_scenario_id(cam_id));
	rk_isp_set_sharpness(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.adjustment:hue", rkipc_get_scenario_id(cam_id));
	rk_isp_set_hue(cam_id, rk_param_get_int(entry, 50));
	// exposure
	LOG_DEBUG("exposure\n");
	snprintf(entry, 127, "isp.%d.exposure:exposure_mode", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "auto"));
	rk_isp_set_exposure_mode(cam_id, value);
	snprintf(entry, 127, "isp.%d.exposure:gain_mode", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "auto"));
	rk_isp_set_gain_mode(cam_id, value);
	snprintf(entry, 127, "isp.%d.exposure:exposure_time", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "1/6"));
	rk_isp_set_exposure_time(cam_id, value);
	snprintf(entry, 127, "isp.%d.exposure:exposure_gain", rkipc_get_scenario_id(cam_id));
	rk_isp_set_exposure_gain(cam_id, rk_param_get_int(entry, 1));
	// night_to_day
	LOG_DEBUG("night_to_day\n");
	snprintf(entry, 127, "isp.%d.night_to_day:night_to_day", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "day"));
	rk_isp_set_night_to_day(cam_id, value);
	snprintf(entry, 127, "isp.%d.night_to_day:fill_light_mode", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "IR"));
	rk_isp_set_fill_light_mode(cam_id, value);
	snprintf(entry, 127, "isp.%d.night_to_day:light_brightness", rkipc_get_scenario_id(cam_id));
	rk_isp_set_light_brightness(cam_id, rk_param_get_int(entry, 1));
	// rk_isp_set_night_to_day_filter_level
	// rk_isp_set_night_to_day_filter_time
	// blc
	LOG_DEBUG("blc\n");
	// rk_isp_set_hdr will loop infinitely, and it has been set during init
	snprintf(entry, 127, "isp.%d.blc:blc_region", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "close"));
	rk_isp_set_blc_region(cam_id, value);
	snprintf(entry, 127, "isp.%d.blc:hlc", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "close"));
	rk_isp_set_hlc(cam_id, value);
	snprintf(entry, 127, "isp.%d.blc:hdr_level", rkipc_get_scenario_id(cam_id));
	rk_isp_set_hdr_level(cam_id, rk_param_get_int(entry, 1));
	snprintf(entry, 127, "isp.%d.blc:blc_strength", rkipc_get_scenario_id(cam_id));
	rk_isp_set_blc_strength(cam_id, rk_param_get_int(entry, 1));
	snprintf(entry, 127, "isp.%d.blc:hlc_level", rkipc_get_scenario_id(cam_id));
	rk_isp_set_hlc_level(cam_id, rk_param_get_int(entry, 0));
	snprintf(entry, 127, "isp.%d.blc:dark_boost_level", rkipc_get_scenario_id(cam_id));
	rk_isp_set_dark_boost_level(cam_id, rk_param_get_int(entry, 0));
	// white_blance
	LOG_DEBUG("white_blance\n");
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_style", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "autoWhiteBalance"));
	rk_isp_set_white_blance_style(cam_id, value);
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_red", rkipc_get_scenario_id(cam_id));
	rk_isp_set_white_blance_red(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_green", rkipc_get_scenario_id(cam_id));
	rk_isp_set_white_blance_green(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.white_blance:white_blance_blue", rkipc_get_scenario_id(cam_id));
	rk_isp_set_white_blance_blue(cam_id, rk_param_get_int(entry, 50));
	// enhancement
	LOG_DEBUG("enhancement\n");
	snprintf(entry, 127, "isp.%d.enhancement:noise_reduce_mode", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "close"));
	rk_isp_set_noise_reduce_mode(cam_id, value);
	snprintf(entry, 127, "isp.%d.enhancement:dehaze", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "close"));
	rk_isp_set_dehaze(cam_id, value);
	snprintf(entry, 127, "isp.%d.enhancement:gray_scale_mode", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "[0-255]"));
	rk_isp_set_gray_scale_mode(cam_id, value);
	snprintf(entry, 127, "isp.%d.enhancement:distortion_correction", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "close"));
	rk_isp_set_distortion_correction(cam_id, value);
	snprintf(entry, 127, "isp.%d.enhancement:spatial_denoise_level", rkipc_get_scenario_id(cam_id));
	rk_isp_set_spatial_denoise_level(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.enhancement:temporal_denoise_level",
	         rkipc_get_scenario_id(cam_id));
	rk_isp_set_temporal_denoise_level(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.enhancement:dehaze_level", rkipc_get_scenario_id(cam_id));
	rk_isp_set_dehaze_level(cam_id, rk_param_get_int(entry, 50));
	snprintf(entry, 127, "isp.%d.enhancement:ldch_level", rkipc_get_scenario_id(cam_id));
	rk_isp_set_ldch_level(cam_id, rk_param_get_int(entry, 0));
	// video_adjustment
	LOG_DEBUG("video_adjustment\n");
	snprintf(entry, 127, "isp.%d.video_adjustment:power_line_frequency_mode",
	         rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "PAL(50HZ)"));
	rk_isp_set_power_line_frequency_mode(cam_id, value);
	snprintf(entry, 127, "isp.%d.video_adjustment:image_flip", rkipc_get_scenario_id(cam_id));
	strcpy(value, rk_param_get_string(entry, "close"));
	rk_isp_set_image_flip(cam_id, value);
	// auto focus
	// LOG_DEBUG("auto focus\n");
	// rk_isp_set_af_mode(cam_id, const char *value);

	LOG_DEBUG("end\n");

	return ret;
}

int rk_isp_init(int cam_id, char *iqfile_path) {
	int ret;
	if (iqfile_path)
		memcpy(g_iq_file_dir_, iqfile_path, strlen(iqfile_path));
	else
		memcpy(g_iq_file_dir_, "/etc/iqfiles", strlen("/etc/iqfiles"));
	LOG_INFO("g_iq_file_dir_ is %s\n", g_iq_file_dir_);

	const char *scenario = rk_param_get_string("isp:scenario", "normal");
	if (!strcmp(scenario, "normal")) {
		current_scenario_id = 0;
		strcpy(sub_scene, rk_param_get_string("isp:normal_scene", "day"));
	} else {
		current_scenario_id = 1;
		strcpy(sub_scene, rk_param_get_string("isp:custom1_scene", "night"));
	}

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr", rkipc_get_scenario_id(cam_id));
	const char *hdr_mode = rk_param_get_string(entry, "close");
	LOG_INFO("cam_id is %d, hdr_mode is %s, scenario is %s\n", cam_id, hdr_mode, scenario);

	if (!strcmp(hdr_mode, "HDR2")) {
		ret = sample_common_isp_init(cam_id, RK_AIQ_WORKING_MODE_ISP_HDR2, true, g_iq_file_dir_);
	} else {
		ret = sample_common_isp_init(cam_id, RK_AIQ_WORKING_MODE_NORMAL, true, g_iq_file_dir_);
	}

	ret |= sample_common_isp_run(cam_id);

	return ret;
}

int rk_isp_deinit(int cam_id) {
	LOG_INFO("cam_id is %d\n", cam_id);
	RK_ISP_CHECK_CAMERA_ID(cam_id);
	LOG_INFO("rk_aiq_uapi2_sysctl_stop enter\n");
	rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[cam_id], false);
	LOG_INFO("rk_aiq_uapi2_sysctl_deinit enter\n");
	rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[cam_id]);
	LOG_INFO("rk_aiq_uapi2_sysctl_deinit exit\n");
	g_aiq_ctx[cam_id] = NULL;

	return 0;
}

int rk_isp_group_init(int cam_group_id, char *iqfile_path) {
	LOG_INFO("cam_group_id is %d\n", cam_group_id);
	int ret;
	rkipc_aiq_use_group = 1;
	if (iqfile_path)
		memcpy(g_iq_file_dir_, iqfile_path, strlen(iqfile_path));
	else
		memcpy(g_iq_file_dir_, "/etc/iqfiles", strlen("/etc/iqfiles"));
	LOG_INFO("g_iq_file_dir_ is %s\n", g_iq_file_dir_);

	char entry[128] = {'\0'};
	snprintf(entry, 127, "isp.%d.blc:hdr", cam_group_id);
	const char *hdr_mode = rk_param_get_string(entry, "close");
	LOG_INFO("cam_group_id is %d, hdr_mode is %s\n", cam_group_id, hdr_mode);
	if (!strcmp(hdr_mode, "HDR2")) {
		ret = isp_camera_group_init(cam_group_id, RK_AIQ_WORKING_MODE_ISP_HDR2, false,
		                            g_iq_file_dir_);
	} else {
		ret =
		    isp_camera_group_init(cam_group_id, RK_AIQ_WORKING_MODE_NORMAL, false, g_iq_file_dir_);
	}
	// ret |= rk_isp_set_from_ini(cam_group_id);

	return ret;
}

int rk_isp_group_deinit(int cam_group_id) {
	LOG_INFO("cam_group_id is %d\n", cam_group_id);
	return isp_camera_group_stop(cam_group_id);
}
