/*
 *  Copyright (c) 2022 Rockchip Corporation
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

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <linux/v4l2-subdev.h>
#include "mediactl/mediactl.h"
#include "rk_debug.h"
#include "smartIr/rk_smart_ir_api.h"
#include "uAPI2/rk_aiq_user_api2_ae.h"
#include "uAPI2/rk_aiq_user_api2_awb.h"
#include "uAPI2/rk_aiq_user_api2_sysctl.h"
#include "uAPI2/rk_aiq_user_api2_aie.h"

#define RKAIQ_FILE_PATH_LEN                       64

/* 1 vicap + 2 mipi + 1 bridge + 1 redundance */
#define MAX_MEDIA_NODES                           5

typedef struct smartIr_server_s {
	const rk_aiq_sys_ctx_t	*aiq_ctx;
	rk_smart_ir_ctx_t	*ir_ctx;
	rk_smart_ir_params_t	ir_configs;
	RK_SMART_IR_STATUS_t	cur_ir_status;
	char			ir_cut_path[RKAIQ_FILE_PATH_LEN];

	pthread_t		tid;
	bool			tquit;
	bool			started;
	bool			is_use_aie;
} smartIr_server_t;

static smartIr_server_t g_smartIr_server_ctx;

static int rkaiq_get_devname(const char *mdev_path, const char *name, char *dev_name)
{
	int ret;
	const char *devname;
	struct media_entity *entity = NULL;
	struct media_device *device = NULL;

	device = rk_media_device_new(mdev_path);
	if (!device)
		return -ENOMEM;

	/* Enumerate entities, pads and links. */
	ret = rk_media_device_enumerate(device);
	if (ret)
		return ret;

	entity = rk_media_get_entity_by_name(device, name, strlen(name));
	if (!entity) {
		ret = -1;
		goto exit;
	}

	devname = rk_media_entity_get_devname(entity);
	if (!devname) {
		ret = -1;
		goto exit;
	}

	strncpy(dev_name, devname, RKAIQ_FILE_PATH_LEN);

	ret = 0;
exit:
	rk_media_device_unref(device);

	return ret;
}

int rkaiq_get_ircut_path(char *dev_name)
{
	char mdev_path[RKAIQ_FILE_PATH_LEN];

	for (int i = 0; i < MAX_MEDIA_NODES; i++) {
		sprintf(mdev_path, "/dev/media%d", i);

		if (rkaiq_get_devname(mdev_path, "m00_b_ircut", dev_name) == 0) {
			RK_LOGV("ircut dev_name = %s\n", dev_name);

			return 0;
		}
	}

	return -1;
}

static int ir_cutter_setting(const char *ir_cut_path, bool on)
{
	int ret = -1;
	struct v4l2_control control;

	control.id = V4L2_CID_BAND_STOP_FILTER;

	if (on)
		control.value = 3; // filter ir
	else
		control.value = 0; // ir in

	int _fd = open(ir_cut_path, O_RDWR | O_CLOEXEC);

	if (_fd != -1) {
		ret = ioctl(_fd, VIDIOC_S_CTRL, &control);
		if (ret < 0) {
			RK_LOGE("failed to set ircut value %d to device, %s!\n",
				control.value, on ? "on" : "off");
			goto exit;
		}

		ret = 0;

		close(_fd);
	} else {
		RK_LOGE("open %s for ircut failed!\n", ir_cut_path);
	}

exit:
	return ret;
}

static int set_sub_scene(smartIr_server_t *smartIr_ctx, const char *sub_scene)
{
	int ret;
	const char *main_scene = NULL;

	if (strcmp("day", sub_scene) && strcmp("night", sub_scene)) {
		RK_LOGE("scene value[%s] unsupport\n", sub_scene);
		return -1;
	}

	rk_aiq_working_mode_t working_mode;

	ret = rk_aiq_uapi2_sysctl_getWorkingMode(smartIr_ctx->aiq_ctx, &working_mode);
	if (ret != XCAM_RETURN_NO_ERROR) {
		RK_LOGE("rk_aiq_uapi2_sysctl_getWorkingMode	error.\n");
		return -1;
	}

	main_scene = (working_mode == RK_AIQ_WORKING_MODE_NORMAL) ? "normal" : "hdr";

	if (!smartIr_ctx->is_use_aie)
		ret = rk_aiq_uapi2_sysctl_switch_scene(smartIr_ctx->aiq_ctx, main_scene, sub_scene);

	if (ret || smartIr_ctx->is_use_aie) {
		aie_attrib_t attr;

		smartIr_ctx->is_use_aie = true;

		RK_LOGW("switch_scene error %s, sub_scene = %s, use aie setting\n",
			main_scene, sub_scene);

		memset(&attr, 0x00, sizeof(attr));

		ret = rk_aiq_user_api2_aie_GetAttrib(smartIr_ctx->aiq_ctx, &attr);
		if (ret) {
			RK_LOGE("rk_aiq_user_api2_aie_GetAttrib	error[%d].\n", ret);
			goto exit;
		}

		attr.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
		if (strcmp(sub_scene, "night") == 0)
			attr.mode = RK_AIQ_IE_EFFECT_BW;
		else
			attr.mode = RK_AIQ_IE_EFFECT_NONE;

		ret = rk_aiq_user_api2_aie_SetAttrib(smartIr_ctx->aiq_ctx, &attr);
	}

exit:
	if (ret)
		RK_LOGE("switch to %s failed[%d]\n", sub_scene, ret);

	return ret;
}

static int switch_ir_status(smartIr_server_t *smartIr_ctx, RK_SMART_IR_STATUS_t status)
{
	int ret = -1;

	if (status == smartIr_ctx->cur_ir_status)
		return 0;

	if (status == RK_SMART_IR_STATUS_DAY) {
		// ir-cutter on
		ir_cutter_setting(smartIr_ctx->ir_cut_path, true);

		// switch to isp day params
		ret = set_sub_scene(smartIr_ctx, "day");

		RK_LOGI("smart ir switch to day.\n");
	} else if (status == RK_SMART_IR_STATUS_NIGHT) {
		// switch to isp night params
		ret = set_sub_scene(smartIr_ctx, "night");

		/* skip two frame */
		usleep(80 * 1000);

		// ir-cutter off
		ir_cutter_setting(smartIr_ctx->ir_cut_path, false);

		RK_LOGI("smart ir switch to night.\n");
	} else {
		RK_LOGW("smart ir switch unknown status.\n");
	}

	if (ret == 0)
		smartIr_ctx->cur_ir_status = status;
}

static void load_ir_configs(smartIr_server_t *smartIr_ctx, RK_SMART_IR_STATUS_t init_ir_status)
{
	rk_smart_ir_result_t init_ir_res;

	smartIr_ctx->ir_configs.d2n_envL_th    = 0.04f;
	smartIr_ctx->ir_configs.n2d_envL_th    = 0.2f;
	smartIr_ctx->ir_configs.rggain_base    = 1.0f;
	smartIr_ctx->ir_configs.bggain_base    = 1.0f;
	smartIr_ctx->ir_configs.awbgain_rad    = 0.1f;
	smartIr_ctx->ir_configs.awbgain_dis    = 0.3f;
	smartIr_ctx->ir_configs.switch_cnts_th = 25;

	rkaiq_get_ircut_path(smartIr_ctx->ir_cut_path);

	rk_smart_ir_config(smartIr_ctx->ir_ctx, &smartIr_ctx->ir_configs);

	smartIr_ctx->cur_ir_status = -1;

	init_ir_res.status = init_ir_status;
	rk_smart_ir_set_status(smartIr_ctx->ir_ctx, init_ir_res);

	// set initial status
	switch_ir_status(smartIr_ctx, init_ir_status);
}

static void *switch_ir_thread(void *args)
{
	smartIr_server_t *smartIr_ctx = (smartIr_server_t *)args;
	rk_smart_ir_result_t ir_res;
	rk_aiq_isp_stats_t *stats_ref = NULL;
	XCamReturn ret = XCAM_RETURN_NO_ERROR;

	RK_LOGI("smart ir thread runing...\n");

	while (!smartIr_ctx->tquit) {
		ret = rk_aiq_uapi2_sysctl_get3AStatsBlk(smartIr_ctx->aiq_ctx, &stats_ref, 300);
		if (ret == XCAM_RETURN_NO_ERROR && stats_ref != NULL) {
			rk_smart_ir_runOnce(smartIr_ctx->ir_ctx, stats_ref, &ir_res);

			rk_aiq_uapi2_sysctl_release3AStatsRef(smartIr_ctx->aiq_ctx, stats_ref);

			switch_ir_status(smartIr_ctx, ir_res.status);
		} else {
			if (ret	== XCAM_RETURN_NO_ERROR)
				RK_LOGE("aiq has stopped !\n");
			else if (ret == XCAM_RETURN_ERROR_TIMEOUT)
				;//RK_LOGE("aiq timeout!\n");
			else if (ret == XCAM_RETURN_ERROR_FAILED)
				RK_LOGE("aiq failed!\n");
		}
	}

	RK_LOGI("smart ir thread exit...\n");

	return NULL;
}

int smart_ir_server_start(const void *aiq_ctx, int init_ir_status)
{
	int ret = 0;
	const rk_aiq_sys_ctx_t *ctx = (rk_aiq_sys_ctx_t *)(aiq_ctx);
	smartIr_server_t *smartIr_ctx = &g_smartIr_server_ctx;

	if (smartIr_ctx->tid > 0 && smartIr_ctx->started) {
		RK_LOGW("smart_ir_server already started.\n");
		return 0;
	}

	memset(smartIr_ctx, 0x00, sizeof(*smartIr_ctx));

	smartIr_ctx->ir_ctx  = rk_smart_ir_init(ctx);
	if (!smartIr_ctx->ir_ctx) {
		RK_LOGE("rk_smart_ir_init failed.\n");
		return -1;
	}

	smartIr_ctx->aiq_ctx = ctx;

	load_ir_configs(smartIr_ctx, init_ir_status);

	ret = pthread_create(&smartIr_ctx->tid, NULL, switch_ir_thread, &g_smartIr_server_ctx);
	if (ret) {
		RK_LOGE("create switch_ir_thread error %d.\n", ret);
		goto error;
	}

	smartIr_ctx->started = true;

	return ret;
error:
	if (smartIr_ctx->ir_ctx)
		rk_smart_ir_deInit(smartIr_ctx->ir_ctx);

	memset(smartIr_ctx, 0x00, sizeof(*smartIr_ctx));

	return ret;
}

void smart_ir_server_stop(const void *aiq_ctx)
{
	const rk_aiq_sys_ctx_t *ctx = (rk_aiq_sys_ctx_t *)(aiq_ctx);
	smartIr_server_t *smartIr_ctx = &g_smartIr_server_ctx;

	if (smartIr_ctx->started) {
		smartIr_ctx->tquit = true;
		pthread_join(smartIr_ctx->tid, NULL);
	}

	if (smartIr_ctx->ir_ctx)
		rk_smart_ir_deInit(smartIr_ctx->ir_ctx);

	memset(smartIr_ctx, 0x00, sizeof(*smartIr_ctx));
}

void smart_ir_calib(const void *aiq_ctx)
{
	const rk_aiq_sys_ctx_t *ctx = (rk_aiq_sys_ctx_t *)(aiq_ctx);
	smartIr_server_t *smartIr_ctx = &g_smartIr_server_ctx;

	//1. make sure no visible light
	//2. ircutter off, ir on
	smartIr_ctx->cur_ir_status = -1;
	switch_ir_status(smartIr_ctx, RK_SMART_IR_STATUS_NIGHT);

	//3. query wb info
	rk_aiq_wb_querry_info_t wb_info;
	int counts = 0;
	float RGgain = 0.0f, BGgain = 0.0f;
	rk_aiq_isp_stats_t *stats_ref = NULL;
	XCamReturn ret = XCAM_RETURN_NO_ERROR;

	RK_LOGI("SmartIr Calib start......\n");

	while (counts++ < 100) {
		ret = rk_aiq_uapi2_sysctl_get3AStatsBlk(ctx, &stats_ref, -1);
		if (ret == XCAM_RETURN_NO_ERROR && stats_ref != NULL) {
			RK_LOGI("stats frame id %d\n", stats_ref->frame_id);

			if (stats_ref->awb_hw_ver == 4) { //isp32
				double Rvalue = 0, Gvalue = 0, Bvalue = 0, wpNo = 0;

				for (int i = 0; i < RK_AIQ_AWB_GRID_NUM_TOTAL; i++) {
					Rvalue += stats_ref->awb_stats_v32.blockResult[i].Rvalue;
					Gvalue += stats_ref->awb_stats_v32.blockResult[i].Gvalue;
					Bvalue += stats_ref->awb_stats_v32.blockResult[i].Bvalue;
					wpNo   += stats_ref->awb_stats_v32.blockResult[i].WpNo;
				}

				Rvalue /= wpNo;
				Gvalue /= wpNo;
				Bvalue /= wpNo;

				RK_LOGI("origin Rvalue:%0.3f, Gvalue:%0.3f, Bvalue:%0.3f, RGgain:%0.3f, BGgain:%0.3f\n",
					Rvalue, Gvalue, Bvalue, Rvalue / Gvalue, Bvalue / Gvalue);
			}
			rk_aiq_uapi2_sysctl_release3AStatsRef(ctx, stats_ref);
		} else {
			if (ret == XCAM_RETURN_NO_ERROR)
				RK_LOGE("aiq has stopped !\n");
			else if (ret == XCAM_RETURN_ERROR_TIMEOUT)
				RK_LOGE("aiq timeout!\n");
			else if (ret == XCAM_RETURN_ERROR_FAILED)
				RK_LOGE("aiq failed!\n");
		}

		ret = rk_aiq_user_api2_awb_QueryWBInfo(ctx, &wb_info);
		if (ret == XCAM_RETURN_NO_ERROR) {
			RGgain = 1.0 / wb_info.stat_gain_blk.rgain;
			BGgain = 1.0 / wb_info.stat_gain_blk.bgain;
			RK_LOGI("RGgain:%0.3f, BGgain: %0.3f\n", RGgain, BGgain);
		}
	}

	RK_LOGI("SmartIr Calib Done......\n");
}
