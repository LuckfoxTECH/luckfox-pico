// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdatomic.h>
#include "comm_isp.h"
#include "rk_debug.h"
#include "uAPI2/rk_aiq_user_api2_sysctl.h"

#define MAX_AIQ_CTX 3
static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
static rk_aiq_camgroup_ctx_t *g_aiq_camgroup_ctx[MAX_AIQ_CTX];
rk_aiq_working_mode_t g_WDRMode[MAX_AIQ_CTX];
static atomic_int g_sof_cnt = 0;
static atomic_bool g_should_quit = false;

static XCamReturn SAMPLE_COMM_ISP_ErrCb(rk_aiq_err_msg_t *msg) {
	if (msg->err_code == XCAM_RETURN_BYPASS)
		g_should_quit = true;
	return XCAM_RETURN_NO_ERROR;
}

static XCamReturn SAMPLE_COMM_ISP_SofCb(rk_aiq_metas_t *meta) {
	g_sof_cnt++;
	if (g_sof_cnt <= 2)
		RK_LOGD("=== %u ===\n", meta->frame_id);
	return XCAM_RETURN_NO_ERROR;
}

RK_S32 SAMPLE_COMM_ISP_Init(RK_S32 CamId, rk_aiq_working_mode_t WDRMode, RK_BOOL MultiCam,
			    const char *iq_file_dir)
{
	if (CamId >= MAX_AIQ_CTX) {
		RK_LOGE("%s : CamId is over 3\n", __FUNCTION__);
		return -1;
	}
	// char *iq_file_dir = "iqfiles/";
	setlinebuf(stdout);
	if (iq_file_dir == NULL) {
		RK_LOGE("SAMPLE_COMM_ISP_Init : not start.\n");
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

	RK_LOGD("ID: %d, sensor_name is %s, iqfiles is %s\n", CamId,
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

RK_S32 SAMPLE_COMM_ISP_Run(RK_S32 CamId)
{
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		RK_LOGE("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	if (rk_aiq_uapi2_sysctl_prepare(g_aiq_ctx[CamId], 0, 0, g_WDRMode[CamId])) {
		RK_LOGE("rkaiq engine prepare failed !\n");
		g_aiq_ctx[CamId] = NULL;
		return -1;
	}
	RK_LOGD("rk_aiq_uapi2_sysctl_init/prepare succeed\n");
	if (rk_aiq_uapi2_sysctl_start(g_aiq_ctx[CamId])) {
		RK_LOGE("rk_aiq_uapi2_sysctl_start  failed\n");
		return -1;
	}
	RK_LOGD("rk_aiq_uapi2_sysctl_start succeed\n");
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_CamGroup_Init(RK_S32 CamGroupId, rk_aiq_working_mode_t WDRMode,
                                     bool MultiCam, int OpenLdch, void *LdchMesh[],
                                     rk_aiq_camgroup_instance_cfg_t *pCamGroupCfg) {
	int i, ret;
	char sensor_name_array[MAX_AIQ_CTX][128];
	rk_aiq_static_info_t aiq_static_info;

	if (CamGroupId >= MAX_AIQ_CTX) {
		RK_LOGE("%s : CamId is over %d\n", __FUNCTION__, MAX_AIQ_CTX);
		return -1;
	}

	for (i = 0; i < pCamGroupCfg->sns_num; i++) {
		ret = rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(i, &aiq_static_info);
		if (ret != 0) {
			RK_LOGE("rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId failure \n");
			return -1;
		}

		RK_LOGD("CamGroupId:%d, cam_id: %d, sensor_name is %s, iqfiles is %s\n",
		       CamGroupId, i, aiq_static_info.sensor_info.sensor_name,
		       pCamGroupCfg->config_file_dir);
		memcpy(sensor_name_array[i], aiq_static_info.sensor_info.sensor_name,
		       strlen(aiq_static_info.sensor_info.sensor_name) + 1);
		pCamGroupCfg->sns_ent_nm_array[i] = sensor_name_array[i];
		RK_LOGD("pCamGroupCfg->sns_ent_nm_array[%d] is %s\n", i,
		       pCamGroupCfg->sns_ent_nm_array[i]);
		ret = rk_aiq_uapi2_sysctl_preInit_devBufCnt(
		    aiq_static_info.sensor_info.sensor_name, "rkraw_rx", 2);
		if (ret != 0) {
			RK_LOGE("rk_aiq_uapi2_sysctl_preInit_devBufCnt failure\n");
			return -1;
		}
	}

	g_aiq_camgroup_ctx[CamGroupId] = rk_aiq_uapi2_camgroup_create(pCamGroupCfg);
	if (!g_aiq_camgroup_ctx[CamGroupId]) {
		RK_LOGE("create camgroup ctx error!\n");
		return -1;
	}
	/* set LDCH must before <camgroup prepare>*/
	// if (OpenLdch) {
	// 	SAMPLE_COMM_ISP_CamGroup_setMeshToLdch(CamGroupId, OpenLdch, LdchMesh);
	// }

	ret = rk_aiq_uapi2_camgroup_prepare(g_aiq_camgroup_ctx[CamGroupId], WDRMode);

	ret |= rk_aiq_uapi2_camgroup_start(g_aiq_camgroup_ctx[CamGroupId]);
	if (ret != 0) {
		RK_LOGE("rk_aiq_uapi2_camgroup_prepare / start failure \n");
		return -1;
	}
	RK_LOGD("rk_aiq_uapi2_camgroup_start over\n");

	return ret;
}

RK_S32 SAMPLE_COMM_ISP_Stop(RK_S32 CamId)
{
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		RK_LOGE("%s : CamId is over 3 or not init g_aiq_ctx[%d] = %p\n", __FUNCTION__,
		       CamId, g_aiq_ctx[CamId]);
		return -1;
	}
	RK_LOGD("rk_aiq_uapi2_sysctl_stop enter\n");
	rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[CamId], false);
	RK_LOGD("rk_aiq_uapi2_sysctl_deinit enter\n");
	rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[CamId]);
	RK_LOGD("rk_aiq_uapi2_sysctl_deinit exit\n");
	g_aiq_ctx[CamId] = NULL;
	return 0;
}

RK_S32 SAMPLE_COMM_ISP_CamGroup_Stop(RK_S32 CamGroupId) {
	if (CamGroupId >= MAX_AIQ_CTX || !g_aiq_camgroup_ctx[CamGroupId]) {
		RK_LOGE("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}
	RK_LOGD("rk_aiq_uapi2_camgroup_stop enter\n");
	rk_aiq_uapi2_camgroup_stop(g_aiq_camgroup_ctx[CamGroupId]);
	RK_LOGD("rk_aiq_uapi2_camgroup_destroy enter\n");
	rk_aiq_uapi2_camgroup_destroy(g_aiq_camgroup_ctx[CamGroupId]);
	RK_LOGD("rk_aiq_uapi2_camgroup_destroy exit\n");
	g_aiq_camgroup_ctx[CamGroupId] = NULL;

	return 0;
}
