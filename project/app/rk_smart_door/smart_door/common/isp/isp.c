#include "iniparser.h"
#include "param.h"
#include "uvc_log.h"
#include <rk_aiq_user_api2_acsm.h>
#include <rk_aiq_user_api2_camgroup.h>
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "isp.c"

#define MAX_AIQ_CTX 8
char main_scene[32];
char sub_scene[32];
char g_iq_file_dir_[256];

static int rkuvc_aiq_use_group = 0;
static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
static rk_aiq_camgroup_ctx_t *g_camera_group_ctx[MAX_AIQ_CTX];
rk_aiq_working_mode_t g_WDRMode[MAX_AIQ_CTX];
rk_aiq_wb_gain_t gs_wb_gain = {2.083900, 1.000000, 1.000000, 2.018500};

#define RK_ISP_CHECK_CAMERA_ID(CAMERA_ID)                                      \
  do {                                                                         \
    if (rkuvc_aiq_use_group) {                                                 \
      if (CAMERA_ID >= MAX_AIQ_CTX || !g_camera_group_ctx[CAMERA_ID]) {        \
        LOG_ERROR("camera_group_id is over 3 or not init\n");                  \
        return -1;                                                             \
      }                                                                        \
    } else {                                                                   \
      if (CAMERA_ID >= MAX_AIQ_CTX || !g_aiq_ctx[CAMERA_ID]) {                 \
        LOG_ERROR("camera_id is over 3 or not init\n");                        \
        return -1;                                                             \
      }                                                                        \
    }                                                                          \
  } while (0)

#define RK_ISP_CHECK_NORMAL_MODE(CAMERA_ID)                                    \
  do {                                                                         \
    if (g_WDRMode[cam_id] != RK_AIQ_WORKING_MODE_NORMAL) {                     \
      LOG_ERROR("Not support in HDR mode\n");                                  \
      return 0;                                                                \
    }                                                                          \
  } while (0)

rk_aiq_sys_ctx_t *rkuvc_aiq_get_ctx(int cam_id) {
  if (rkuvc_aiq_use_group)
    return (rk_aiq_sys_ctx_t *)g_camera_group_ctx[cam_id];

  return g_aiq_ctx[cam_id];
}

int sample_common_isp_init(int cam_id, rk_aiq_working_mode_t WDRMode,
                           bool MultiCam, const char *iq_file_dir) {
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
  rk_aiq_uapi2_sysctl_enumStaticMetas(cam_id, &aiq_static_info);
  if (aiq_static_info.sensor_info.phyId == -1) {
    LOG_INFO("WARN: aiq_static_info.sensor_info.phyId is %d\n",
             aiq_static_info.sensor_info.phyId);
  }

  LOG_INFO("ID: %d, sensor_name is %s, iqfiles is %s\n", cam_id,
           aiq_static_info.sensor_info.sensor_name, iq_file_dir);

  int ret;
  if (WDRMode == RK_AIQ_WORKING_MODE_NORMAL)
    ret = rk_aiq_uapi2_sysctl_preInit_scene(
        aiq_static_info.sensor_info.sensor_name, "normal", "day");
  else
    ret = rk_aiq_uapi2_sysctl_preInit_scene(
        aiq_static_info.sensor_info.sensor_name, "hdr", "day");
  if (ret < 0)
    LOG_ERROR("%s: failed to set scene\n",
              aiq_static_info.sensor_info.sensor_name);

#if 1
  aiq_ctx = rk_aiq_uapi2_sysctl_init(aiq_static_info.sensor_info.sensor_name,
                                     iq_file_dir, NULL, NULL);
// LOG_ERROR("tmp force use m00_b_imx464 3-001a\n");
#else
  if (cam_id == 0)
    aiq_ctx = rk_aiq_uapi2_sysctl_init("m02_b_imx464 3-001a", iq_file_dir, NULL,
                                       NULL);
  else if (cam_id == 1)
    aiq_ctx = rk_aiq_uapi2_sysctl_init("m03_b_imx464 3-0036", iq_file_dir, NULL,
                                       NULL);
  else if (cam_id == 2)
    aiq_ctx = rk_aiq_uapi2_sysctl_init("m00_b_imx464 4-001a", iq_file_dir, NULL,
                                       NULL);
  else if (cam_id == 3)
    aiq_ctx = rk_aiq_uapi2_sysctl_init("m01_b_imx464 4-0036", iq_file_dir, NULL,
                                       NULL);
  else if (cam_id == 4)
    aiq_ctx = rk_aiq_uapi2_sysctl_init("m04_b_imx464 5-001a", iq_file_dir, NULL,
                                       NULL);
  else if (cam_id == 5)
    aiq_ctx = rk_aiq_uapi2_sysctl_init("m05_b_imx464 5-0036", iq_file_dir, NULL,
                                       NULL);
#endif
  // if (MultiCam)
  // 	rk_aiq_uapi2_sysctl_setMulCamConc(aiq_ctx, true);

  g_aiq_ctx[cam_id] = aiq_ctx;

  return 0;
}

int sample_common_isp_run(int cam_id) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  if (rk_aiq_uapi2_sysctl_prepare(g_aiq_ctx[cam_id], 0, 0, g_WDRMode[cam_id])) {
    LOG_ERROR("rkaiq engine prepare failed !\n");
    g_aiq_ctx[cam_id] = NULL;
    return -1;
  }
  LOG_INFO("rk_aiq_uapi2_sysctl_init/prepare succeed\n");
  if (rk_aiq_uapi2_sysctl_start(g_aiq_ctx[cam_id])) {
    LOG_ERROR("rk_aiq_uapi2_sysctl_start  failed\n");
    return -1;
  }
  LOG_INFO("rk_aiq_uapi2_sysctl_start succeed\n");
  return 0;
}

int isp_camera_group_init(int cam_group_id, rk_aiq_working_mode_t WDRMode,
                          bool MultiCam, const char *iq_file_dir) {
  int ret;
  rk_aiq_sys_ctx_t *aiq_ctx;
  rk_aiq_static_info_t aiq_static_info;
  char sensor_name_array[6][128];
  rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
  memset(&camgroup_cfg, 0, sizeof(camgroup_cfg));

  camgroup_cfg.sns_num = rk_param_get_int("avs:sensor_num", 6);
  LOG_INFO("camgroup_cfg.sns_num is %d\n", camgroup_cfg.sns_num);
  for (int i = 0; i < camgroup_cfg.sns_num; i++) {
    rk_aiq_uapi2_sysctl_enumStaticMetas(i, &aiq_static_info);
    LOG_INFO("cam_group_id:%d, cam_id: %d, sensor_name is %s, iqfiles is %s\n",
             cam_group_id, i, aiq_static_info.sensor_info.sensor_name,
             iq_file_dir);
    memcpy(sensor_name_array[i], aiq_static_info.sensor_info.sensor_name,
           strlen(aiq_static_info.sensor_info.sensor_name) + 1);
    camgroup_cfg.sns_ent_nm_array[i] = sensor_name_array[i];
    LOG_INFO("camgroup_cfg.sns_ent_nm_array[%d] is %s\n", i,
             camgroup_cfg.sns_ent_nm_array[i]);
    if (WDRMode == RK_AIQ_WORKING_MODE_NORMAL)
      ret = rk_aiq_uapi2_sysctl_preInit_scene(
          aiq_static_info.sensor_info.sensor_name, "normal", "day");
    else
      ret = rk_aiq_uapi2_sysctl_preInit_scene(
          aiq_static_info.sensor_info.sensor_name, "hdr", "day");
    if (ret < 0)
      LOG_ERROR("%s: failed to set scene\n",
                aiq_static_info.sensor_info.sensor_name);
  }

  camgroup_cfg.config_file_dir = iq_file_dir;
  g_camera_group_ctx[cam_group_id] =
      rk_aiq_uapi2_camgroup_create(&camgroup_cfg);
  if (!g_camera_group_ctx[cam_group_id]) {
    LOG_ERROR("create camgroup ctx error!\n");
    return -1;
  }
  LOG_INFO("rk_aiq_uapi2_camgroup_create over\n");
  ret =
      rk_aiq_uapi2_camgroup_prepare(g_camera_group_ctx[cam_group_id], WDRMode);
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

int rk_isp_set_frame_rate(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  char entry[128] = {'\0'};
  Uapi_ExpSwAttrV2_t expSwAttr;
  LOG_INFO("start %d\n", value);
  ret = rk_aiq_user_api2_ae_getExpSwAttr(rkuvc_aiq_get_ctx(cam_id), &expSwAttr);
  expSwAttr.stAuto.stFrmRate.isFpsFix = true;
  expSwAttr.stAuto.stFrmRate.FpsValue = value;
  ret = rk_aiq_user_api2_ae_setExpSwAttr(rkuvc_aiq_get_ctx(cam_id), expSwAttr);
  LOG_INFO("end, %d\n", value);

  snprintf(entry, 127, "isp.%d.adjustment:fps", cam_id);
  rk_param_set_int(entry, value);

  return 0;
}

// image adjustment

int rk_isp_get_contrast(int cam_id, int *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.adjustment:contrast", cam_id);
  *value = rk_param_get_int(entry, -1);

  return 0;
}

int rk_isp_set_contrast(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  char entry[128] = {'\0'};
  acp_attrib_t attrib;
  ret = rk_aiq_user_api2_acp_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  attrib.contrast = value * 2.55; // value[0,255]
  ret |= rk_aiq_user_api2_acp_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  snprintf(entry, 127, "isp.%d.adjustment:contrast", cam_id);
  rk_param_set_int(entry, value);

  return ret;
}

int rk_isp_get_brightness(int cam_id, int *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.adjustment:brightness", cam_id);
  *value = rk_param_get_int(entry, -1);

  return 0;
}

int rk_isp_set_brightness(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  char entry[128] = {'\0'};
  acp_attrib_t attrib;
  ret = rk_aiq_user_api2_acp_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  attrib.brightness = value * 2.55; // value[0,255]
  ret |= rk_aiq_user_api2_acp_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  snprintf(entry, 127, "isp.%d.adjustment:brightness", cam_id);
  rk_param_set_int(entry, value);

  return ret;
}

int rk_isp_get_saturation(int cam_id, int *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.adjustment:saturation", cam_id);
  *value = rk_param_get_int(entry, -1);

  return 0;
}

int rk_isp_set_saturation(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  char entry[128] = {'\0'};
  acp_attrib_t attrib;
  ret = rk_aiq_user_api2_acp_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  attrib.saturation = value * 2.55; // value[0,255]
  ret |= rk_aiq_user_api2_acp_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  snprintf(entry, 127, "isp.%d.adjustment:saturation", cam_id);
  rk_param_set_int(entry, value);

  return ret;
}

int rk_isp_get_sharpness(int cam_id, int *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.adjustment:sharpness", cam_id);
  *value = rk_param_get_int(entry, -1);

  return 0;
}

int rk_isp_set_sharpness(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret = 0;
  ret = rk_aiq_uapi2_setSharpness(rkuvc_aiq_get_ctx(cam_id),
                                             value);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.adjustment:sharpness", cam_id);
  rk_param_set_int(entry, value);

  return ret;
}

int rk_isp_get_hue(int cam_id, int *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.adjustment:hue", cam_id);
  *value = rk_param_get_int(entry, -1);

  return 0;
}

int rk_isp_set_hue(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  char entry[128] = {'\0'};
  acp_attrib_t attrib;
  ret = rk_aiq_user_api2_acp_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  attrib.hue = value * 2.55; // value[0,255]
  ret |= rk_aiq_user_api2_acp_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  snprintf(entry, 127, "isp.%d.adjustment:hue", cam_id);
  rk_param_set_int(entry, value);

  return ret;
}

int rk_isp_get_hue_mode(int cam_id, int *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.adjustment:hueauto", cam_id);
  *value = rk_param_get_int(entry, -1);

  return 0;
}

int rk_isp_set_hue_mode(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  if (value == 1)
    value = 50; // auto
  else {
    rk_isp_get_hue(cam_id, &value);
    if (value == -1)
      value = 50;
  }
  LOG_INFO("hue_mode: value: %d\n", value);
  int ret;
  char entry[128] = {'\0'};
  acp_attrib_t attrib;
  ret = rk_aiq_user_api2_acp_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  attrib.hue = value * 2.55; // value[0,255]
  ret |= rk_aiq_user_api2_acp_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &attrib);
  snprintf(entry, 127, "isp.%d.adjustment:hueauto", cam_id);
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
  Uapi_ExpSwAttrV2_t expSwAttr;
  rk_aiq_user_api2_ae_getExpSwAttr(rkuvc_aiq_get_ctx(cam_id), &expSwAttr);
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
  int ret =
      rk_aiq_user_api2_ae_setExpSwAttr(rkuvc_aiq_get_ctx(cam_id), expSwAttr);
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
  Uapi_ExpSwAttrV2_t stExpSwAttr;

  rk_aiq_user_api2_ae_getExpSwAttr(rkuvc_aiq_get_ctx(cam_id), &stExpSwAttr);
  if (!strcmp(value, "auto")) {
    stExpSwAttr.stManual.LinearAE.ManualGainEn = false;
    stExpSwAttr.stManual.HdrAE.ManualGainEn = false;
  } else {
    stExpSwAttr.stManual.LinearAE.ManualGainEn = true;
    stExpSwAttr.stManual.HdrAE.ManualGainEn = true;
  }
  int ret =
      rk_aiq_user_api2_ae_setExpSwAttr(rkuvc_aiq_get_ctx(cam_id), stExpSwAttr);
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
  Uapi_ExpSwAttrV2_t stExpSwAttr;
  float den, num, result;
  if (strchr(value, '/') == NULL) {
    den = 1;
    sscanf(value, "%f", &result);
  } else {
    sscanf(value, "%f/%f", &num, &den);
    result = num / den;
  }
  rk_aiq_user_api2_ae_getExpSwAttr(rkuvc_aiq_get_ctx(cam_id), &stExpSwAttr);
  stExpSwAttr.stManual.LinearAE.TimeValue = result;
  stExpSwAttr.stManual.HdrAE.TimeValue[0] = result;
  stExpSwAttr.stManual.HdrAE.TimeValue[1] = result;
  stExpSwAttr.stManual.HdrAE.TimeValue[2] = result;
  int ret =
      rk_aiq_user_api2_ae_setExpSwAttr(rkuvc_aiq_get_ctx(cam_id), stExpSwAttr);
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
  Uapi_ExpSwAttrV2_t stExpSwAttr;
  float gain_set = (value * 1.0f);
  rk_aiq_user_api2_ae_getExpSwAttr(rkuvc_aiq_get_ctx(cam_id), &stExpSwAttr);
  stExpSwAttr.stManual.LinearAE.GainValue = gain_set;
  stExpSwAttr.stManual.HdrAE.GainValue[0] = gain_set;
  stExpSwAttr.stManual.HdrAE.GainValue[1] = gain_set;
  stExpSwAttr.stManual.HdrAE.GainValue[2] = gain_set;
  int ret =
      rk_aiq_user_api2_ae_setExpSwAttr(rkuvc_aiq_get_ctx(cam_id), stExpSwAttr);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.exposure:exposure_gain", cam_id);
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
  RK_ISP_CHECK_NORMAL_MODE(cam_id);
  Uapi_LinExpAttrV2_t LineExpAttr;

  ret = rk_aiq_user_api2_ae_getLinExpAttr(rkuvc_aiq_get_ctx(cam_id),
                                          &LineExpAttr);
  if (!strcmp(value, "close"))
    LineExpAttr.Params.BackLightCtrl.Enable = 0;
  else
    LineExpAttr.Params.BackLightCtrl.Enable = 1;
  LineExpAttr.Params.BackLightCtrl.MeasArea = AECV2_MEASURE_AREA_AUTO;
  LineExpAttr.Params.BackLightCtrl.StrBias = 0;
  ret =
      rk_aiq_user_api2_ae_setLinExpAttr(rkuvc_aiq_get_ctx(cam_id), LineExpAttr);
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
  RK_ISP_CHECK_NORMAL_MODE(cam_id);
  Uapi_LinExpAttrV2_t LinExpAttr;

  ret =
      rk_aiq_user_api2_ae_getLinExpAttr(rkuvc_aiq_get_ctx(cam_id), &LinExpAttr);
  if (ret)
    LOG_ERROR("get exp attr failed\n");
  if (!strcmp(value, "close"))
    LinExpAttr.Params.OverExpCtrl.Enable = 0;
  else
    LinExpAttr.Params.OverExpCtrl.Enable = 1;
  LinExpAttr.Params.OverExpCtrl.StrBias = 0;
  ret =
      rk_aiq_user_api2_ae_setLinExpAttr(rkuvc_aiq_get_ctx(cam_id), LinExpAttr);
  if (ret)
    LOG_ERROR("set exp attr failed\n");
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
  rk_aiq_uapi2_setDrcGain(rkuvc_aiq_get_ctx(cam_id), (float)value, 0.1,
                          16); // Gain: [1, 8]
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
  RK_ISP_CHECK_NORMAL_MODE(cam_id);
  Uapi_LinExpAttrV2_t LineExpAttr;

  ret = rk_aiq_user_api2_ae_getLinExpAttr(rkuvc_aiq_get_ctx(cam_id),
                                          &LineExpAttr);
  if (ret)
    LOG_ERROR("getLinExpAttr error\n");
  if (LineExpAttr.Params.BackLightCtrl.Enable == 0) {
    LOG_ERROR("blc mode is not enabled\n");
    return 0;
  }
  LineExpAttr.Params.BackLightCtrl.StrBias = value;
  ret =
      rk_aiq_user_api2_ae_setLinExpAttr(rkuvc_aiq_get_ctx(cam_id), LineExpAttr);
  if (ret)
    LOG_ERROR("setLinExpAttr error\n");
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
  RK_ISP_CHECK_NORMAL_MODE(cam_id);
  Uapi_LinExpAttrV2_t LineExpAttr;

  if (value == 0)
    value = 1;
  ret = rk_aiq_user_api2_ae_getLinExpAttr(rkuvc_aiq_get_ctx(cam_id),
                                          &LineExpAttr);
  if (ret)
    LOG_ERROR("getLinExpAttr error\n");
  if (LineExpAttr.Params.OverExpCtrl.Enable == 0) {
    LOG_ERROR("hlc mode is not enabled\n");
    return 0;
  }
  LineExpAttr.Params.OverExpCtrl.StrBias = value;
  ret =
      rk_aiq_user_api2_ae_setLinExpAttr(rkuvc_aiq_get_ctx(cam_id), LineExpAttr);
  if (ret)
    LOG_ERROR("setLinExpAttr error\n");
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
  LOG_ERROR("ISP3.0 do not support tmo api\n");
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
  rk_aiq_uapiV2_wb_opMode_t attr;

  if (!strcmp(value, "manualWhiteBalance")) {
    attr.mode = RK_AIQ_WB_MODE_MANUAL;
  } else {
    attr.mode = RK_AIQ_WB_MODE_AUTO;
  }
  ret = rk_aiq_user_api2_awb_SetWpModeAttrib(rkuvc_aiq_get_ctx(cam_id), attr);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.white_blance:white_blance_style", cam_id);
  rk_param_set_string(entry, value);

  return ret;
}

int rk_isp_get_white_blance_ct(int cam_id, int *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.white_blance:white_blance_red", cam_id);
  *value = rk_param_get_int(entry, -1);

  return 0;
}

int rk_isp_set_white_blance_ct(int cam_id, int value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  rk_aiq_uapiV2_wb_opMode_t mode_attr;

  rk_aiq_user_api2_awb_GetWpModeAttrib(rkuvc_aiq_get_ctx(cam_id), &mode_attr);
  if (mode_attr.mode == RK_AIQ_WB_MODE_AUTO) {
    LOG_WARN("white blance is auto, not support set color temperature\n");
    return 0;
  }
  ret = rk_aiq_uapi2_setMWBCT(rkuvc_aiq_get_ctx(cam_id), value);

  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.white_blance:white_blance_ct", cam_id);
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

// Turn off noise reduction, the actual default value is set to 50,
// and it is done in the interface of setting level
int rk_isp_set_noise_reduce_mode(int cam_id, const char *value) {
  int ret;
  RK_ISP_CHECK_CAMERA_ID(cam_id);
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
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  char entry[128] = {'\0'};
  // adehaze_sw_V2_t attr;
  // memset(&attr, 0, sizeof(attr));
  // attr.sync.sync_mode = RK_AIQ_UAPI_MODE_DEFAULT;
  // if (!strcmp(value, "close")) {
  // 	attr.mode = DEHAZE_API_BYPASS;
  // } else if (!strcmp(value, "open")) {
  // 	attr.mode = DEHAZE_API_MANUAL;
  // } else if (!strcmp(value, "auto")) {
  // 	attr.mode = DEHAZE_API_DEHAZE_AUTO;
  // }
  // ret = rk_aiq_user_api2_adehaze_setSwAttrib(rkuvc_aiq_get_ctx(cam_id),
  // attr);
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
  rk_aiq_uapi_acsm_attrib_t attr;
  rk_aiq_user_api2_acsm_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &attr);
  if (!strcmp(value, "[16-235]"))
    attr.param.full_range = false;
  else
    attr.param.full_range = true;
  rk_aiq_user_api2_acsm_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &attr);
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
  rk_aiq_ldch_attrib_t ldchAttr;
  ret = rk_aiq_user_api2_aldch_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &ldchAttr);
  if (!strcmp(value, "close"))
    ldchAttr.en = false;
  else
    ldchAttr.en = true;
  ret = rk_aiq_user_api2_aldch_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &ldchAttr);

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
  int ret = 0;
  const char *noise_reduce_mode;
  rk_aiq_ynr_strength_v3_t ynrStrenght;
  rk_aiq_bayer2dnr_strength_v2_t bayer2dnrV2Strenght;

  rk_isp_get_noise_reduce_mode(cam_id, &noise_reduce_mode);
  LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode, value);
  if ((!strcmp(noise_reduce_mode, "close")) ||
      (!strcmp(noise_reduce_mode, "3dnr"))) {
    value = 50;
    LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode,
              value);
  }
  ynrStrenght.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
  ynrStrenght.percent = value / 100.0;
  ret = rk_aiq_user_api2_aynrV3_SetStrength(rkuvc_aiq_get_ctx(cam_id),
                                            &ynrStrenght);
  bayer2dnrV2Strenght.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
  bayer2dnrV2Strenght.percent = value / 100.0;
  ret = rk_aiq_user_api2_abayer2dnrV2_SetStrength(rkuvc_aiq_get_ctx(cam_id),
                                                  &bayer2dnrV2Strenght);

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
  int ret = 0;
  const char *noise_reduce_mode;
  rk_aiq_bayertnr_strength_v2_t bayertnrV2Strenght;

  rk_isp_get_noise_reduce_mode(cam_id, &noise_reduce_mode);
  LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode, value);
  if ((!strcmp(noise_reduce_mode, "close")) ||
      (!strcmp(noise_reduce_mode, "2dnr"))) {
    value = 50;
    LOG_DEBUG("noise_reduce_mode is %s, value is %d\n", noise_reduce_mode,
              value);
  }
  bayertnrV2Strenght.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
  bayertnrV2Strenght.percent = value / 100.0;
  ret = rk_aiq_user_api2_abayertnrV2_SetStrength(rkuvc_aiq_get_ctx(cam_id),
                                                 &bayertnrV2Strenght);
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
  int ret;
  // adehaze_sw_V2_t attr;

  // attr.sync.sync_mode = RK_AIQ_UAPI_MODE_DEFAULT;
  // attr.sync.done = false;
  // attr.mode = DEHAZE_API_DEHAZE_MANUAL;
  // attr.stDehazeManu.level = value;
  // int ret = rk_aiq_user_api2_adehaze_setSwAttrib(rkuvc_aiq_get_ctx(cam_id),
  // attr);
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
  // int ret = rk_aiq_uapi2_setFecCorrectLevel(g_aiq_ctx[cam_id],
  //                                          (int)(value * 2.55)); // [0-100]
  //                                          -> [0->255]
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.enhancement:fec_level", cam_id);
  rk_param_set_int(entry, value);

  return 0;
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
  int ret = 0;
  rk_aiq_ldch_attrib_t ldchAttr;

  value = value < 0 ? 0 : value;
  ret = rk_aiq_user_api2_aldch_GetAttrib(rkuvc_aiq_get_ctx(cam_id), &ldchAttr);
  ldchAttr.correct_level = (int)(value * 2.53 + 2); // [0, 100] -> [2 , 255]
  ret = rk_aiq_user_api2_aldch_SetAttrib(rkuvc_aiq_get_ctx(cam_id), &ldchAttr);

  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.enhancement:ldch_level", cam_id);
  rk_param_set_int(entry, value);

  return ret;
}

// video_adjustment
int rk_isp_get_power_line_frequency_mode(int cam_id, const char **value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.video_adjustment:power_line_frequency_mode",
           cam_id);
  *value = rk_param_get_string(entry, NULL);

  return 0;
}

int rk_isp_set_power_line_frequency_mode(int cam_id, const char *value) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret;
  char entry[128] = {'\0'};
  Uapi_ExpSwAttrV2_t expSwAttr;

  ret = rk_aiq_user_api2_ae_getExpSwAttr(rkuvc_aiq_get_ctx(cam_id), &expSwAttr);
  if (!strcmp(value, "NTSC(60HZ)")) {
    expSwAttr.stAuto.stAntiFlicker.enable = true;
    expSwAttr.stAuto.stAntiFlicker.Frequency = AECV2_FLICKER_FREQUENCY_60HZ;
  } else {
    expSwAttr.stAuto.stAntiFlicker.enable = true;
    expSwAttr.stAuto.stAntiFlicker.Frequency = AECV2_FLICKER_FREQUENCY_50HZ;
  }
  ret = rk_aiq_user_api2_ae_setExpSwAttr(rkuvc_aiq_get_ctx(cam_id), expSwAttr);
  snprintf(entry, 127, "isp.%d.video_adjustment:power_line_frequency_mode",
           cam_id);
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
  rk_aiq_uapi2_setMirrorFlip(rkuvc_aiq_get_ctx(cam_id), mirror, flip, 4); //
  // skip 4 frame
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
  ret = rk_aiq_uapi2_setFocusMode(rkuvc_aiq_get_ctx(cam_id), af_mode);
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
  ret = rk_aiq_uapi2_getZoomRange(rkuvc_aiq_get_ctx(cam_id), &af_zoom_range);
  if (ret) {
    LOG_ERROR("get zoom range fail: %d\n", ret);
    return ret;
  }
  rk_aiq_uapi2_getOpZoomPosition(rkuvc_aiq_get_ctx(cam_id), &code);
  code += change;
  if ((code < af_zoom_range.min_pos) || (code > af_zoom_range.max_pos)) {
    LOG_ERROR("set zoom: %d over range [%d, %d]\n", code, af_zoom_range.min_pos,
              af_zoom_range.max_pos);
    ret = -1;
  }
  ret = rk_aiq_uapi2_setOpZoomPosition(rkuvc_aiq_get_ctx(cam_id), code);
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
  ret = rk_aiq_uapi2_getFocusRange(rkuvc_aiq_get_ctx(cam_id), &af_focus_range);
  if (ret) {
    LOG_ERROR("get focus range fail: %d\n", ret);
    return ret;
  }
  rk_aiq_uapi2_getFocusPosition(rkuvc_aiq_get_ctx(cam_id), &code);
  code += change;
  if ((code < af_focus_range.min_pos) || (code > af_focus_range.max_pos)) {
    LOG_ERROR("before set getFocusPosition: %d over range (%d, %d)\n", code,
              af_focus_range.min_pos, af_focus_range.max_pos);
    return -1;
  }
  ret = rk_aiq_uapi2_setFocusPosition(rkuvc_aiq_get_ctx(cam_id), code);
  LOG_INFO("set setFocusPosition: %d, ret: %d\n", code, ret);
  snprintf(entry, 127, "isp.%d.auto_focus:focus_level", cam_id);
  rk_param_set_int(entry, code);

  return ret;
}

int rk_isp_af_zoom_in(int cam_id) { return rk_isp_af_zoom_change(cam_id, 20); }

int rk_isp_af_zoom_out(int cam_id) {
  return rk_isp_af_zoom_change(cam_id, -20);
}

int rk_isp_af_focus_in(int cam_id) { return rk_isp_af_focus_change(cam_id, 1); }

int rk_isp_af_focus_out(int cam_id) {
  return rk_isp_af_focus_change(cam_id, -1);
}

int rk_isp_af_focus_once(int cam_id) {
  LOG_INFO("af_focus_once\n");
  return rk_aiq_uapi2_endOpZoomChange(rkuvc_aiq_get_ctx(cam_id));
}

int rk_isp_set_from_ini(int cam_id) {
  RK_ISP_CHECK_CAMERA_ID(cam_id);
  int ret = 0;
  LOG_INFO("start\n");
  rk_isp_set_frame_rate(cam_id, rk_param_get_int("isp.0.adjustment:fps", 30));
  // rk_isp_set_contrast(cam_id, rk_param_get_int("isp.0.adjustment:contrast",
  // 50));
  // rk_isp_set_brightness(cam_id,
  // rk_param_get_int("isp.0.adjustment:brightness", 50));
  // rk_isp_set_saturation(cam_id,
  // rk_param_get_int("isp.0.adjustment:saturation", 50));
  // rk_isp_set_sharpness(cam_id, rk_param_get_int("isp.0.adjustment:sharpness",
  // 50));
  // rk_isp_set_hue(cam_id, rk_param_get_int("isp.0.adjustment:hue", 50));
  LOG_INFO("end\n");

  return ret;
}

int rk_isp_init(int cam_id, char *iqfile_path) {
  LOG_INFO("cam_id is %d\n", cam_id);
  int ret;
  rkuvc_aiq_use_group = 0;
  if (iqfile_path)
    memcpy(g_iq_file_dir_, iqfile_path, strlen(iqfile_path));
  else
    memcpy(g_iq_file_dir_, "/etc/iqfiles", strlen("/etc/iqfiles"));
  LOG_INFO("g_iq_file_dir_ is %s\n", g_iq_file_dir_);

  char entry[128] = {'\0'};
  snprintf(entry, 127, "isp.%d.blc:hdr", cam_id);
  const char *hdr_mode = rk_param_get_string(entry, "close");
  LOG_INFO("cam_id is %d, hdr_mode is %s\n", cam_id, hdr_mode);
  if (!strcmp(hdr_mode, "HDR2")) {
    ret = sample_common_isp_init(cam_id, RK_AIQ_WORKING_MODE_ISP_HDR2, true,
                                 g_iq_file_dir_);
    // rk_aiq_uapi2_sysctl_switch_scene(g_aiq_ctx[cam_id], "hdr" , "day");
  } else {
    ret = sample_common_isp_init(cam_id, RK_AIQ_WORKING_MODE_NORMAL, true,
                                 g_iq_file_dir_);
    // rk_aiq_uapi2_sysctl_switch_scene(g_aiq_ctx[cam_id], "normal" , "day");
  }

  ret |= sample_common_isp_run(cam_id);
  ret |= rk_isp_set_from_ini(cam_id);

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
  rkuvc_aiq_use_group = 1;
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
    ret = isp_camera_group_init(cam_group_id, RK_AIQ_WORKING_MODE_ISP_HDR2,
                                false, g_iq_file_dir_);
  } else {
    ret = isp_camera_group_init(cam_group_id, RK_AIQ_WORKING_MODE_NORMAL, false,
                                g_iq_file_dir_);
  }
  ret |= rk_isp_set_from_ini(cam_group_id);

  return ret;
}

int rk_isp_group_deinit(int cam_group_id) {
  LOG_INFO("cam_group_id is %d\n", cam_group_id);
  return isp_camera_group_stop(cam_group_id);
}

int rk_isp_configure_media_pipeline(int cam_id) {
    int ret;
    rk_aiq_working_mode_t WDRMode;

    LOG_INFO("cam_id is %d\n", cam_id);

    if (cam_id >= MAX_AIQ_CTX) {
        LOG_ERROR("%s : cam_id is over 3\n", __FUNCTION__);
        return -1;
    }

    rkuvc_aiq_use_group = 0;

    const char *scenario = rk_param_get_string("isp:scenario", "normal");
    if (!strcmp(scenario, "normal")) {
        strcpy(sub_scene, rk_param_get_string("isp:normal_scene", "day"));
    } else {
        strcpy(sub_scene, rk_param_get_string("isp:custom1_scene", "night"));
    }

    char entry[128] = {'\0'};
    snprintf(entry, 127, "isp.%d.blc:hdr", cam_id);
    const char *hdr_mode = rk_param_get_string(entry, "close");
    LOG_INFO("cam_id is %d, hdr_mode is %s, scenario is %s\n", cam_id, hdr_mode, scenario);

    if (!strcmp(hdr_mode, "HDR2")) {
        WDRMode = RK_AIQ_WORKING_MODE_ISP_HDR2;
    } else {
        WDRMode = RK_AIQ_WORKING_MODE_NORMAL;
    }

    setlinebuf(stdout);
    // must set HDR_MODE, before init
    g_WDRMode[cam_id] = WDRMode;
    char hdr_str[16];
    snprintf(hdr_str, sizeof(hdr_str), "%d", (int)WDRMode);
    setenv("HDR_MODE", hdr_str, 1);

    rk_aiq_static_info_t aiq_static_info;
    rk_aiq_uapi2_sysctl_enumStaticMetas(cam_id, &aiq_static_info);
    if (aiq_static_info.sensor_info.phyId == -1) {
        LOG_INFO("WARN: aiq_static_info.sensor_info.phyId is %d\n",
                 aiq_static_info.sensor_info.phyId);
    }

    LOG_INFO("ID: %d, sensor_name is %s\n", cam_id, aiq_static_info.sensor_info.sensor_name);

    if (WDRMode == RK_AIQ_WORKING_MODE_NORMAL)
        strcpy(main_scene, "normal");
    else
        strcpy(main_scene, "hdr");

    LOG_INFO("main_scene is %s, sub_scene is %s\n", main_scene, sub_scene);
    LOG_INFO("rk_aiq_uapi2_sysctl_preInit_scene begin\n");
    ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name, main_scene,
                                            sub_scene);
    if (ret < 0)
        LOG_ERROR("%s: failed to set scene\n", aiq_static_info.sensor_info.sensor_name);

    LOG_INFO("rk_aiq_uapi2_sysctl_preInit_scene over\n");

    return ret;
}

int rk_isp_setExpSwAttr(int cam_id, void *ctx) {
    if (!ctx) {
        LOG_ERROR("rk_isp_set_exp: ctx error\n");
        return -1;
    }

    g_aiq_ctx[cam_id] = (rk_aiq_sys_ctx_t *)ctx;

    return rk_isp_set_from_ini(cam_id);
}
