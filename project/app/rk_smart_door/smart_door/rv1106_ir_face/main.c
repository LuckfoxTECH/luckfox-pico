// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "smart_door.h"
#include "param.h"
#include "comm_isp.h"
#include "vi_face.h"

static void isp_mpi_init(void)
{
	RK_S32 s32Ret = RK_FAILURE;
	char *iq_file_dir = "/etc/iqfiles/";
	int cam_num = CAM_NUM;
	RK_BOOL bMultictx = (cam_num > 1) ? RK_TRUE : RK_FALSE;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	int cam_id = 0;

	SAMPLE_COMM_ISP_Init(cam_id, hdr_mode, bMultictx, iq_file_dir);
	SAMPLE_COMM_ISP_Run(cam_id);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_Init failed!");
	}
}

static void isp_mpi_deinit(void)
{
	RK_S32 s32Ret = RK_FAILURE;
	int cam_id = 0;

	RK_MPI_SYS_Exit();
	SAMPLE_COMM_ISP_Stop(cam_id);
}

static void isp_mpi_group_init(RK_S32 s32CamGroupId)
{
	RK_S32 s32Ret = RK_FAILURE;
	int cam_num = CAM_NUM;
	char *iq_file_dir = "/etc/iqfiles/";
	RK_BOOL bMultictx = (cam_num > 1) ? RK_TRUE : RK_FALSE;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

	rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
	memset(&camgroup_cfg, 0, sizeof(camgroup_cfg));
	camgroup_cfg.sns_num = cam_num;
	camgroup_cfg.config_file_dir = iq_file_dir;
	s32Ret = SAMPLE_COMM_ISP_CamGroup_Init(s32CamGroupId, hdr_mode, bMultictx, 0,
					       RK_NULL, &camgroup_cfg);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init failure:%#X", s32Ret);
		return;
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_Init failed!");
	}
}

static void isp_mpi_group_deinit(RK_S32 s32CamGroupId)
{
	RK_S32 s32Ret = RK_FAILURE;

	RK_MPI_SYS_Exit();

	s32Ret = SAMPLE_COMM_ISP_CamGroup_Stop(s32CamGroupId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Stop failure:%#X", s32Ret);
	}
}

static void dump_guide(void)
{
	printf("\n################ smart door demo ################\n");
	system("sdkinfo");

	printf("\n[FUNCTION]Input and create face ID (take ID=1 as example)\n");
	printf("[COMMAND]#echo face_create:1 > /tmp/smart_door.ini\n");

	printf("\n[FUNCTION]Face recognition:\n");
	printf("[COMMAND]#echo face_recognition > /tmp/smart_door.ini\n");

	printf("\n[FUNCTION]Delete one face ID (take ID=1 as example):\n");
	printf("[COMMAND]#echo face_delete:1 > /tmp/smart_door.ini\n");

	printf("\n[FUNCTION]Delete all face ID:\n");
	printf("[COMMAND]#echo face_delete > /tmp/smart_door.ini\n");

	printf("\n[FUNCTION]Preview image by UVC:\n");
	printf("[COMMAND]#echo uvc_start > /tmp/smart_door.ini\n");
	printf("[COMMAND]#echo uvc_stop > /tmp/smart_door.ini\n");

	printf("\n[FUNCTION]Preview image by RTSP (require network):\n");
	printf("[COMMAND]#echo rtsp_start > /tmp/smart_door.ini\n");
	printf("[COMMAND]#echo rtsp_stop > /tmp/smart_door.ini\n");
}

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	const char *ini_file = "/tmp/smart_door.ini";
	char *param_ini_path = NULL; // use default param init file
	char cmd_line[20];
	pthread_t pipe0_thread = 0, uvc_thread = 0;
	RK_S32 s32CamGroupId = 0;
	int face_id;
	RK_BOOL is_dump_guide = RK_TRUE;
	//sync from face_unlock.hh
	enum face_use_enum {
		REGISTER,
		COMPARE,
		SAME_ID_REGISTER,
		REGISTER_DIR_MIDDLE,
		REGISTER_DIR_LEFT,
		REGISTER_DIR_RIGHT,
		REGISTER_DIR_UP,
		REGISTER_DIR_DOWN,
	};

	if ((fp = fopen(ini_file, "w+")) == NULL) {
		RK_LOGE("open ini file failed!");
		return RK_SUCCESS;
	}

	while (1) {
		memset(cmd_line, 0, sizeof(cmd_line));
		fseek(fp, 0, SEEK_SET);
		fgets(cmd_line, 20, fp);
		truncate(ini_file, 0);

		if (strstr(cmd_line, "face_create")) {
			face_id = 0;
			sscanf(cmd_line, "face_create:%d", &face_id);
			RK_LOGI("create face ID=%d", face_id);
			isp_mpi_group_init(s32CamGroupId);
			if (smart_door_face_init()) {
				RK_LOGE("smart_door_face_init error.");
				continue;
			}

			if (smart_door_face_unlock_flow(REGISTER, face_id)) {
				RK_LOGE("smart_door_face_unlock_flow error.");
			}

			smart_door_face_deinit();
			isp_mpi_group_deinit(s32CamGroupId);
		} else if (strstr(cmd_line, "face_delete")) {
			face_id = 0;
			sscanf(cmd_line, "face_delete:%d", &face_id);
			if (smart_door_face_init()) {
				RK_LOGE("smart_door_face_init error.");
				continue;
			}

			if (face_id) {
				RK_LOGI("delete face ID=%d", face_id);
				smart_door_face_delete_one(face_id);
			} else {
				RK_LOGI("delete all face ID");
				smart_door_face_delete_all();
			}

			smart_door_face_deinit();
		} else if (strstr(cmd_line, "face_recognition")) {
			RK_LOGI("face recognition start");
			isp_mpi_group_init(s32CamGroupId);
			if (smart_door_face_init()) {
				RK_LOGE("smart_door_face_init error.");
				continue;
			}

			if (smart_door_face_unlock_flow(COMPARE, 0)) {
				RK_LOGE("smart_door_face_unlock_flow error.");
			}

			smart_door_face_deinit();
			isp_mpi_group_deinit(s32CamGroupId);
		} else if (strstr(cmd_line, "rtsp_start")) {
			if (pipe0_thread)
				continue;

			isp_mpi_init();
			RK_LOGI("rtsp running");
			pthread_create(&pipe0_thread, 0, (void *)smart_door_pipe_rtsp_start, NULL);
		} else if (strstr(cmd_line, "rtsp_stop")) {
			if (!pipe0_thread)
				continue;

			RK_LOGI("rtsp stop");
			smart_door_pipe_rtsp_stop();
			pthread_join(pipe0_thread, NULL);
			isp_mpi_deinit();
			pipe0_thread = 0;
		} else if (strstr(cmd_line, "uvc_start")) {
			if (uvc_thread)
				continue;

			if (rk_param_init(param_ini_path)) {
				RK_LOGE("rk_param_init failed!");
				goto exit;
			}

			isp_mpi_init();

			RK_LOGI("smart_door uvc start!");
			pthread_create(&uvc_thread, 0, (void *)uvc_start, NULL);
		} else if (strstr(cmd_line, "uvc_stop")) {
			if (!uvc_thread)
				continue;

			RK_LOGI("smart_door uvc stop!");
			uvc_stop();
			pthread_join(uvc_thread, NULL);
			isp_mpi_deinit();
			rk_param_deinit();
			uvc_thread = 0;
		} else if (strstr(cmd_line, "exit")) {
			RK_LOGI("smart_door exit!");
			remove(ini_file);
			break;
		} else {
			usleep(1000000llu);
			if (is_dump_guide) {
				dump_guide();
				is_dump_guide = RK_FALSE;
			}
		}
	}

exit:
	if (fp) {
		fclose(fp);
		fp = NULL;
	}

	return RK_SUCCESS;
}
