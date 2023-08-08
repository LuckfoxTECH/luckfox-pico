// Copyright 2020 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

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

#define MAX_AIQ_CTX 8
static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
static rk_aiq_camgroup_ctx_t *g_aiq_camgroup_ctx[MAX_AIQ_CTX];

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

#include <stdatomic.h>
static atomic_int g_sof_cnt = 0;
static atomic_bool g_should_quit = false;

static XCamReturn SAMPLE_COMM_ISP_SofCb(rk_aiq_metas_t *meta) {
	g_sof_cnt++;
	if (g_sof_cnt <= 2)
		printf("=== %u ===\n", meta->frame_id);
	return XCAM_RETURN_NO_ERROR;
}

RK_S32 SAMPLE_COMM_ISP_GetSofCnt(void) { return g_sof_cnt; }

static XCamReturn SAMPLE_COMM_ISP_ErrCb(rk_aiq_err_msg_t *msg) {
	if (msg->err_code == XCAM_RETURN_BYPASS)
		g_should_quit = true;
}

RK_BOOL SAMPLE_COMM_ISP_ShouldQuit() { return g_should_quit; }

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
	rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(CamId, &aiq_static_info);

	printf("ID: %d, sensor_name is %s, iqfiles is %s\n", CamId,
	       aiq_static_info.sensor_info.sensor_name, iq_file_dir);
	rk_aiq_uapi2_sysctl_preInit_devBufCnt(aiq_static_info.sensor_info.sensor_name,
	                                      "rkraw_rx", 2);
	aiq_ctx =
	    rk_aiq_uapi2_sysctl_init(aiq_static_info.sensor_info.sensor_name, iq_file_dir,
	                             SAMPLE_COMM_ISP_ErrCb, SAMPLE_COMM_ISP_SofCb);

	if (MultiCam)
		rk_aiq_uapi2_sysctl_setMulCamConc(aiq_ctx, true);

	g_aiq_ctx[CamId] = aiq_ctx;
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_CamGroup_Init(RK_S32 CamGroupId, rk_aiq_working_mode_t WDRMode,
                                     bool MultiCam,
                                     rk_aiq_camgroup_instance_cfg_t *pCamGroupCfg) {
	int i, ret;
	char sensor_name_array[MAX_AIQ_CTX][128];
	rk_aiq_static_info_t aiq_static_info;

	if (CamGroupId >= MAX_AIQ_CTX) {
		printf("%s : CamId is over 3\n", __FUNCTION__);
		return -1;
	}

	for (i = 0; i < pCamGroupCfg->sns_num; i++) {
		rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(i, &aiq_static_info);
		printf("CamGroupId:%d, cam_id: %d, sensor_name is %s, iqfiles is %s\n",
		       CamGroupId, i, aiq_static_info.sensor_info.sensor_name,
		       pCamGroupCfg->config_file_dir);
		memcpy(sensor_name_array[i], aiq_static_info.sensor_info.sensor_name,
		       strlen(aiq_static_info.sensor_info.sensor_name) + 1);
		pCamGroupCfg->sns_ent_nm_array[i] = sensor_name_array[i];
		printf("pCamGroupCfg->sns_ent_nm_array[%d] is %s\n", i,
		       pCamGroupCfg->sns_ent_nm_array[i]);
	}

	g_aiq_camgroup_ctx[CamGroupId] = rk_aiq_uapi2_camgroup_create(pCamGroupCfg);
	if (!g_aiq_camgroup_ctx[CamGroupId]) {
		printf("create camgroup ctx error!\n");
		return -1;
	}

	printf("rk_aiq_uapi2_camgroup_create over\n");
	ret = rk_aiq_uapi2_camgroup_prepare(g_aiq_camgroup_ctx[CamGroupId], WDRMode);
	printf("rk_aiq_uapi2_camgroup_prepare over\n");
	ret |= rk_aiq_uapi2_camgroup_start(g_aiq_camgroup_ctx[CamGroupId]);
	printf("rk_aiq_uapi2_camgroup_start over\n");

	return ret;
}

RK_S32 SAMPLE_COMM_ISP_Stop(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init g_aiq_ctx[%d] = %p\n", __FUNCTION__,
		       CamId, g_aiq_ctx[CamId]);
		return -1;
	}
	printf("rk_aiq_uapi2_sysctl_stop enter\n");
	rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[CamId], false);
	printf("rk_aiq_uapi2_sysctl_deinit enter\n");
	rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[CamId]);
	printf("rk_aiq_uapi2_sysctl_deinit exit\n");
	g_aiq_ctx[CamId] = NULL;
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_CamGroup_Stop(RK_S32 CamGroupId) {
	if (CamGroupId >= MAX_AIQ_CTX || !g_aiq_camgroup_ctx[CamGroupId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	printf("rk_aiq_uapi2_camgroup_stop enter\n");
	rk_aiq_uapi2_camgroup_stop(g_aiq_camgroup_ctx[CamGroupId]);
	printf("rk_aiq_uapi2_camgroup_destroy enter\n");
	rk_aiq_uapi2_camgroup_destroy(g_aiq_camgroup_ctx[CamGroupId]);
	printf("rk_aiq_uapi2_camgroup_destroy exit\n");
	g_aiq_camgroup_ctx[CamGroupId] = NULL;
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_Run(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	if (rk_aiq_uapi2_sysctl_prepare(g_aiq_ctx[CamId], 0, 0, g_WDRMode[CamId])) {
		printf("rkaiq engine prepare failed !\n");
		g_aiq_ctx[CamId] = NULL;
		return -1;
	}
	printf("rk_aiq_uapi2_sysctl_init/prepare succeed\n");
	if (rk_aiq_uapi2_sysctl_start(g_aiq_ctx[CamId])) {
		printf("rk_aiq_uapi2_sysctl_start  failed\n");
		return -1;
	}
	printf("rk_aiq_uapi2_sysctl_start succeed\n");
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_SetFrameRate(RK_S32 CamId, RK_U32 uFps) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	int ret;
	Uapi_ExpSwAttrV2_t expSwAttr;
	ret = rk_aiq_user_api2_ae_getExpSwAttr(g_aiq_ctx[CamId], &expSwAttr);
	if (uFps == 0) {
		expSwAttr.stAuto.stFrmRate.isFpsFix = false;
	} else {
		expSwAttr.stAuto.stFrmRate.isFpsFix = true;
		expSwAttr.stAuto.stFrmRate.FpsValue = uFps;
	}

	ret = rk_aiq_user_api2_ae_setExpSwAttr(g_aiq_ctx[CamId], expSwAttr);
	return ret;
}

RK_S32 SAMPLE_COMM_ISP_SetMirrorFlip(int CamId, int mirror, int flip) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	return rk_aiq_uapi2_setMirrorFlip(g_aiq_ctx[CamId], mirror, flip, 4); // skip 4 frame
}

RK_S32 SAMPLE_comm_ISP_SWITCH_SCENE(int CamId, const char *main_scene,
                                    const char *sub_scene) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	return rk_aiq_uapi2_sysctl_switch_scene(g_aiq_ctx[CamId], main_scene, sub_scene);
}

RK_S32 SAMPLE_COMM_ISP_SetLDCH(RK_U32 CamId, RK_U32 u32Level, RK_BOOL bIfEnable) {
	RK_S32 s32Ret = RK_FAILURE;
	rk_aiq_ldch_v21_attrib_t ldchAttr;
	memset(&ldchAttr, 0, sizeof(rk_aiq_ldch_v21_attrib_t));
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over %d or not init\n", __FUNCTION__, MAX_AIQ_CTX);
		return RK_FAILURE;
	}
	s32Ret = rk_aiq_user_api2_aldch_v21_GetAttrib(g_aiq_ctx[CamId], &ldchAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rk_aiq_user_api2_aldch_v21_GetAttrib FAILURE:%X", s32Ret);
		return s32Ret;
	}
	ldchAttr.en = bIfEnable;
	ldchAttr.correct_level = u32Level;
	s32Ret = rk_aiq_user_api2_aldch_v21_SetAttrib(g_aiq_ctx[CamId], &ldchAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rk_aiq_user_api2_aldch_v21_SetAttrib FAILURE:%X", s32Ret);
		return s32Ret;
	}

	return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
