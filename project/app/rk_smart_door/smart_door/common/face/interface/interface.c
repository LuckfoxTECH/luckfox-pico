// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "uart_parser.h"
#include "mcu_inf.h"
#include "interface.h"

#include "server.h"

/*****************************
   face module to ap interface
*****************************/

#if 0// result define
 enum EN_MMI_RET {
	MR_SUCCESS                = 0, // success
	MR_REJECTED               = 1, // module rejected this command
	MR_ABORTED                = 2, // algo aborted
	MR_FAILED4_CAMERA         = 4, // camera open failed
	MR_FAILED4_UNKNOWNREASON  = 5, // UNKNOWN_ERROR
	MR_FAILED4_INVALIDPARAM   = 6, // invalid param
	MR_FAILED4_NOMEMORY       = 7, // no enough memory
	MR_FAILED4_UNKNOWNUSER    = 8, // no user enrolled
	MR_FAILED4_MAXUSER        = 9, // exceed maximum user number
	MR_FAILED4_FACEENROLLED   = 10,// this face has been enrolled
	MR_FAILED4_LIVENESSCHECK  = 12,// liveness check failed
	MR_FAILED4_TIMEOUT        = 13,// exceed the time limit
	MR_FAILED4_AUTHORIZATION  = 14,// authorization failed
	MR_FAILED4_READ_FILE      = 19,// read file failed
	MR_FAILED4_WRITE_FILE     = 20,// write file failed
	MR_FAILED4_NO_ENCRYPT     = 21,// encrypt must be set
	MR_FAILED4_NO_RGBIMAGE    = 23 // rgb image is not ready
};
#endif

enum cmd_type {
	/* server cmd */
	CMD_SERVER_READY,
	CMD_SERVER_FACE_STATE,
	CMD_SERVER_VERIFY_RESULT,
	CMD_SERVER_ENROLL_RESULT,

	/* client cmd */
	CMD_CLIENT_VERIFY,
	CMD_CLIENT_ENROLL,
	CMD_CLIENT_DEL_ALL_USR,
	CMD_CLIENT_DEL_USR_ID,
	CMD_CLIENT_EXIT_FACE,
	CMD_CLIENT_ENROLL_MULTI_DIR,
};

struct server_cmd_s {
	enum cmd_type type;
	int param;
	int param2;
};

/* module msg:
   notify ap module is ready */
void intf_module_ready()
{
	printf("%s\n", __func__);
	struct server_cmd_s cmd;
	cmd.type = CMD_SERVER_READY;
	server_write_cmd((char*)&cmd, sizeof(cmd));
}

/* module msg:
   notify ap face state */
void intf_module_send_face_state(int result)
{
	printf("face state:%d\n", result);
	struct server_cmd_s cmd;
	cmd.type = CMD_SERVER_FACE_STATE;
	cmd.param = result;
	server_write_cmd((char*)&cmd, sizeof(cmd));
}

/* module msg:
   notify ap verify result */
void intf_module_send_verify_result(int result, int usr_id)
{
	printf("verify result:%d, usr_id=%d\n", result, usr_id);
	mcu_inf_clear_cmd();
	mcu_ms_status_set(MS_STANDBY);

	struct server_cmd_s cmd;
	cmd.type = CMD_SERVER_VERIFY_RESULT;
	cmd.param = result;
	cmd.param2 = usr_id;
	server_write_cmd((char*)&cmd, sizeof(cmd));
}

/* module msg:
   notify ap enroll result */
void intf_module_send_enroll_result(int result, int usr_id)
{
	printf("%s\n", __func__);
	printf("enroll result:%d, usr_id=%d\n", result, usr_id);
	mcu_inf_clear_cmd();
	mcu_ms_status_set(MS_STANDBY);

	struct server_cmd_s cmd;
	cmd.type = CMD_SERVER_ENROLL_RESULT;
	cmd.param = result;
	cmd.param2 = usr_id;
	server_write_cmd((char*)&cmd, sizeof(cmd));
}

/*****************************
   ap to face module interface
*****************************/
/* ap cmd:
   notify module to verify face */
int intf_set_module_face_verify(int seconds)
{
	printf("%s\n", __func__);
	int ret = ap_proc_verify_cmd(seconds);
	if (ret)
		printf("%s fail\n", __func__);

	return ret;
}

/* ap cmd:
   notify module to enroll face */
int intf_set_module_face_enroll(int seconds)
{
	printf("%s\n", __func__);
	int ret = ap_proc_enroll_cmd(seconds);
	if (ret)
		printf("%s fail\n", __func__);

	return ret;
}

/* ap cmd:
   notify module to enroll face */
int intf_set_module_face_enroll_multi_dir(int seconds, int dir)
{
	printf("%s\n", __func__);
	int ret = ap_proc_enroll_multi_dir_cmd(seconds, dir);
	if (ret)
		printf("%s fail\n", __func__);

	return ret;
}

/* ap cmd:
   notify module to clear face data */
int intf_set_module_clr_face_data(int face_id)
{
	printf("%s\n", __func__);
	int ret = ap_proc_del_user_cmd(face_id);
	if (ret)
		printf("%s fail\n", __func__);

	return ret;
}

/* ap cmd:
   notify module to exit face */
int intf_set_module_face_exit(void)
{
	printf("%s\n", __func__);
	int ret = ap_proc_face_exit();
	if (ret)
		printf("%s fail\n", __func__);

	return ret;
}

void intf_cmd_parser(char *data, int len)
{
	struct server_cmd_s *cmd = (struct server_cmd_s *)data;
	printf("recv cmd:%d param:%d\n", cmd->type, cmd->param);

	switch (cmd->type) {
		case CMD_CLIENT_VERIFY:
			intf_set_module_face_verify(cmd->param);
			break;
		case CMD_CLIENT_ENROLL:
			intf_set_module_face_enroll(cmd->param);
			break;
		case CMD_CLIENT_DEL_ALL_USR:
			intf_set_module_clr_face_data(-1);
			break;
		case CMD_CLIENT_DEL_USR_ID:
			intf_set_module_clr_face_data(cmd->param);
			break;
		case CMD_CLIENT_EXIT_FACE:
			intf_set_module_face_exit();
			break;
		case CMD_CLIENT_ENROLL_MULTI_DIR:
			intf_set_module_face_enroll_multi_dir(cmd->param, cmd->param2);
			break;
		default:
			printf("unknow cmd:%d\n", cmd->type);
			break;
	}
}

