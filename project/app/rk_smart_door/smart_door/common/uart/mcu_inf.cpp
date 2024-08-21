/*
 *  Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 *
 */
#include "uart_parser.h"
#include "message_mcu.h"
#include "mcu_inf.h"
#include <sys/time.h>

struct mcu_info_s mcu_info;

void msg_mcu_state_init()
{
	memset(&mcu_info, 0x0, sizeof(struct mcu_info_s));
	mcu_info.mcu_state = RKSL_DOOR_ST_IDLE;
	mcu_info.rksl_door_ms = MS_STANDBY;
}

void mcu_ms_status_set(int status)
{
	mcu_info.rksl_door_ms = status;
}

int mcu_ms_status_get()
{
	return mcu_info.rksl_door_ms;
}

int msg_mcu_send(void *data)
{
	int ret = 0;
	return 0;
}

int msg_mcu_write_ota_region(uint8_t *data)
{
	return 0;
}

int msg_mcu_parser_sensetime(struct cmd_uart_msg_head_t *cmd)
{
	int ret = 1;
	s_msg_reply_data msg_reply_data;
	struct mcu_info_s *mcu_info_p = &mcu_info;

	//printf("parser %x %x %x\n", cmd->msg_id, cmd->msg_size, cmd->crc);

	switch (cmd->msg_id)
	{
		case MID_RESET: {
			cmd_uart_send_mid_replay_data(NULL, 0, MID_RESET, 1);
			break;
		}
		case MID_VERIFY: {
			sys_proc_verify_msg(cmd->msg_data);
			break;
		}
		case MID_ENROLL: {
			sys_proc_enroll_msg((void*)cmd->msg_data, MID_ENROLL);
			break;
		}
		case MID_ENROLL_SINGLE: {
			sys_proc_enroll_msg((void*)cmd->msg_data, MID_ENROLL_SINGLE);
			break;
		}
		case MID_FACERESET: {
			break;
		}
		case MID_GETSAVEDIMAGE: {
			sys_proc_get_save_image((void *)cmd->msg_data);
			break;
		}
		case MID_UPLOADIMAGE: {
			sys_proc_upload_image((void *)cmd->msg_data);
			break;
		}
		case MID_START_OTA: {
			printf("MID_START_OTA\n");
			if (mcu_ms_status_get() != MS_STANDBY) {
				printf("module busy\n");
				cmd_uart_send_mid_replay_data(
						(uint8_t *)NULL, 0, MID_START_OTA, MR_REJECTED);
			} else {
				sys_proc_ota_start();
				cmd_uart_send_mid_replay_data((uint8_t *)NULL, 0, MID_START_OTA, 0);
				mcu_ms_status_set(MS_OTA);
				mcu_info_p->mcu_state = RKSL_DOOR_ST_OTA;
			}
			break;
		}
		case MID_STOP_OTA:
			printf("MID_STOP_OTA\n");
			msg_reply_data.mid = MID_STOP_OTA;
			cmd_uart_send_mid_replay_data((uint8_t *)NULL, 0, MID_STOP_OTA, 0);
			mcu_info_p->ota.ota_packet_cnt = 0;
			if (mcu_info_p->ota.ota_data)
				free(mcu_info_p->ota.ota_data);
			mcu_info_p->ota.ota_data = NULL;
			mcu_ms_status_set(MS_STANDBY);
			mcu_info_p->mcu_state = RKSL_DOOR_ST_IDLE;
			break;
		case MID_OTA_HEADER:
			printf("MID_OTA_HEADER\n");
			memcpy(&mcu_info_p->ota.otahead_data, cmd->msg_data, sizeof(s_msg_otaheader_data));
			cmd_uart_send_mid_replay_data((uint8_t *)NULL, 0, MID_OTA_HEADER, 0);
			mcu_info_p->ota.ota_total_packet_num =
				(mcu_info_p->ota.otahead_data.num_pkt[0]) |
				(mcu_info_p->ota.otahead_data.num_pkt[1] << 8) |
				(mcu_info_p->ota.otahead_data.num_pkt[2] << 16) |
				(mcu_info_p->ota.otahead_data.num_pkt[3] << 24);
			mcu_info_p->ota.ota_total_file_size =
				(mcu_info_p->ota.otahead_data.fsize_b[0]) |
				(mcu_info_p->ota.otahead_data.fsize_b[1] << 8) |
				(mcu_info_p->ota.otahead_data.fsize_b[2] << 16) |
				(mcu_info_p->ota.otahead_data.fsize_b[3] << 24);
			mcu_info_p->ota.ota_data = (uint8_t *)malloc(mcu_info_p->ota.ota_total_file_size);
			mcu_info_p->ota.ota_packet_cnt = 0;
			mcu_info_p->mcu_state = RKSL_DOOR_ST_OTA_RECV_HEAD;
			break;
		case MID_OTA_PACKET: {
			//printf("MID_OTA_PACKET\n");
			short cur_packet_size;
			s_msg_otapacket_data *otapacket = (s_msg_otapacket_data *)cmd->msg_data;
			msg_reply_data.mid = MID_OTA_PACKET;
			cmd_uart_send_mid_replay_data((uint8_t *)NULL, 0, MID_OTA_PACKET, 0);
			if ((mcu_info_p->mcu_state == RKSL_DOOR_ST_OTA_RECV_HEAD) ||
				(mcu_info_p->mcu_state == RKSL_DOOR_ST_OTA_RECV_PACKET))
				mcu_info_p->mcu_state = RKSL_DOOR_ST_OTA_RECV_PACKET;

			cur_packet_size = (otapacket->psize[1] << 8) | otapacket->psize[0];
			memcpy((mcu_info_p->ota.ota_data + mcu_info_p->ota.ota_file_size_cnt),
				(uint8_t *)(otapacket) + 4, cur_packet_size);
			mcu_info_p->ota.ota_file_size_cnt += cur_packet_size;
			mcu_info_p->ota.ota_packet_cnt++;
			if (mcu_info_p->ota.ota_packet_cnt == mcu_info_p->ota.ota_total_packet_num)
			{
				printf("start B region cover\n");
				mcu_info_p->mcu_state = RKSL_DOOR_ST_OTA_WRITE_FLASH;
				int ret = 0; //todo
				mcu_info_p->mcu_state = RKSL_DOOR_ST_IDLE;
				mcu_info_p->ota.ota_packet_cnt = 0;
				mcu_info_p->ota.ota_file_size_cnt = 0;

				uint8_t result = (ret == 0)?0:1;
				cmd_uart_send_mid_note_replay(NID_OTA_DONE,
						(uint8_t*)&result, 1);
				printf("note finish\n");
				mcu_ms_status_set(MS_STANDBY);
			}
			break;
		}
		case MID_DELUSER: {
			sys_proc_del_user_msg(-1);
			break;
		}
		case MID_DELALL: {
			sys_proc_del_user_msg(-1);
			break;
		}
		case MID_SET_THRESHOLD_LEVEL: {
			memcpy(&mcu_info_p->algo_threshold_level, cmd->msg_data, sizeof(s_msg_algo_threshold_level));
			cmd_uart_send_mid_replay_data((uint8_t *)NULL, 0, MID_SET_THRESHOLD_LEVEL, 0);
			break;
		}
		case MID_SET_DEBUG_ENC_KEY: {
			sys_proc_set_aes_keysel(cmd->msg_data, 1);
			break;
		}
		case MID_SET_RELEASE_ENC_KEY: {
			break;
		}

		case MID_INIT_ENCRYPTION: {
			sys_proc_init_encryption(cmd->msg_data);
			break;
		}

		case MID_POWERDOWN: {
			cmd_uart_send_mid_replay_data(NULL, 0, MID_POWERDOWN, 0);
			break;
		}
		case MID_FACTORY_TEST: {
			printf("MID_FACTORY_TEST\n");
			//cmd_uart_send_mid_replay_data((uint8_t *)NULL, 0, MID_FACTORY_TEST, 1);
			break;
		}
		default: {
			ret = 0;
			break;
		}
	};

    if (cmd->msg_data)
    {
        free(cmd->msg_data);
        cmd->msg_data = NULL;
    }

	return ret;
}

int msg_mcu_parser_sensetime_irq(struct cmd_uart_msg_head_t *cmd)
{
	int ret = 1;
	s_msg_reply_data msg_reply_data;
	struct mcu_info_s *mcu_info_p = &mcu_info;

	//printf("parser irq %x %x %x\n", cmd->msg_id, cmd->msg_size, cmd->crc);

	switch (cmd->msg_id)
	{
		case MID_GETSTATUS: {
			s_msg_reply_getstatus_data status;
			status.status = 0;
			cmd_uart_send_mid_replay_data((uint8_t *)&status, sizeof(s_msg_reply_verify_data), MID_GETSTATUS, 0);
			break;
		}
		case MID_GETUSERINFO: {
			uint16_t id;
			s_msg_reply_getuserinfo_data *user_reply_info_p;
			s_msg_getuserinfo_data *user_info = (s_msg_getuserinfo_data *)cmd->msg_data;
			id = (user_info->user_id_heb << 8) | (user_info->user_id_leb);
			user_reply_info_p = &mcu_info_p->gesture_info.guest[id];
			cmd_uart_send_mid_replay_gesture_info((uint8_t *)user_reply_info_p, sizeof(s_msg_reply_getuserinfo_data), 0);
			break;
		}
		case MID_GET_ALL_USERID: {
			s_msg_reply_all_userid_data all_user_reply_info;
			all_user_reply_info.user_counts = mcu_info_p->gesture_info.cnt;
			for(uint32_t i = 0; i<mcu_info_p->gesture_info.cnt; i++)
			{
				all_user_reply_info.users_id[i*2 + 0] = mcu_info_p->gesture_info.guest[i].user_id_heb;
				all_user_reply_info.users_id[i*2 + 1] = mcu_info_p->gesture_info.guest[i].user_id_leb;
			}
			cmd_uart_send_mid_replay_alluser_id((uint8_t *)&all_user_reply_info, sizeof(s_msg_reply_all_userid_data), 0);
			break;
		}
		case MID_GET_OTA_STATUS: {
			s_msg_reply_getotastatus_data ota_status;
			memset(&ota_status, 0x0, sizeof(s_msg_reply_getotastatus_data));
			if (mcu_info_p->rksl_door_ms == MS_OTA) {
				ota_status.ota_status = MS_OTA;
				ota_status.next_pid_e[0] = (mcu_info_p->ota.ota_packet_cnt >> 8) & 0xff;
				ota_status.next_pid_e[1] = (mcu_info_p->ota.ota_packet_cnt) & 0xff;
			}
			cmd_uart_send_mid_replay_data((uint8_t *)&ota_status, sizeof(s_msg_reply_getotastatus_data), MID_GET_OTA_STATUS, 0);
			break;
		}
		case MID_GET_VERSION: {
			s_msg_reply_version_data version = {0};
			strcpy((char *)version.version_info,"SEV1S_A0_RLS-V2.0.005");
			cmd_uart_send_mid_replay_data((uint8_t *)&version, sizeof(s_msg_reply_version_data), MID_GET_VERSION, 0);
			break;
		}
		case MID_GET_THRESHOLD_LEVEL: {
			cmd_uart_send_mid_replay_data((uint8_t *)&mcu_info_p->algo_threshold_level,
				sizeof(s_msg_algo_threshold_level), MID_GET_THRESHOLD_LEVEL, 0);
			break;
		}
		default: {
			ret = 0;
			break;
		}
	};

	return ret;
}

int _msg_mcu_receive(uint8_t flag)
{
	int ret = 0;

	struct cmd_uart_msg_buf_t *msg_buf_p = &g_cmd_uart_msg_buf;

	//pthread_mutex_lock(&msg_buf_p->mutex_lock);

	if(((msg_buf_p->front_num + 8) != msg_buf_p->back_num) ||
			msg_buf_p->full_flag) {
		if (flag == 0)
			ret = msg_mcu_parser_sensetime((struct cmd_uart_msg_head_t *)&msg_buf_p->cmd[msg_buf_p->back_num - 8]);
		else
			ret = msg_mcu_parser_sensetime_irq((struct cmd_uart_msg_head_t *)&msg_buf_p->cmd[msg_buf_p->back_num - 8]);

		//printf("msg_mcu_receive %x %x %x\n", msg_buf_p->front_num, msg_buf_p->back_num, ret);

		if (ret) {
			msg_buf_p->back_num++;
			if (msg_buf_p->back_num == 16)
				msg_buf_p->back_num = 8;
			msg_buf_p->full_flag = 0;
		}
	}
	//pthread_mutex_unlock(&msg_buf_p->mutex_lock);

	return ret;
}

int msg_mcu_receive()
{
	_msg_mcu_receive(1);
	_msg_mcu_receive(0);
	return 0;
}

int msg_mcu_buf_empty_check()
{
	int ret = 0;
	struct cmd_uart_msg_buf_t *msg_buf_p = &g_cmd_uart_msg_buf;

	//pthread_mutex_lock(&msg_buf_p->mutex_lock);

	if(((msg_buf_p->front_num + 8) != msg_buf_p->back_num) ||
			msg_buf_p->full_flag) {
		ret = false;
	} else {
		ret = true;
	}

	//pthread_mutex_unlock(&msg_buf_p->mutex_lock);

	return ret;
}


/* interface for system proc */
static struct mcu_cmd_s *g_mcu_cmd = NULL;

void mcu_inf_set_cmd_data(struct mcu_cmd_s *cmd)
{
	g_mcu_cmd = cmd;
}

void mcu_inf_clear_cmd_nolock(void)
{
	memset(g_mcu_cmd, 0, sizeof(mcu_cmd_t) - offsetof(mcu_cmd_t, fix));
}

void mcu_inf_clear_cmd(void)
{
	struct cmd_uart_msg_buf_t *msg_buf_p = &g_cmd_uart_msg_buf;
	pthread_mutex_lock(&msg_buf_p->mutex_lock);
	if (g_mcu_cmd)
		mcu_inf_clear_cmd_nolock();
	pthread_mutex_unlock(&msg_buf_p->mutex_lock);
}

void sys_proc_ota_start(void)
{
}

void sys_proc_get_save_image(void *param)
{
}

void sys_proc_get_save_image_replay(void *param)
{
	uint32_t image_size = 0;
	cmd_uart_send_mid_replay_data((uint8_t *)&image_size, 4, MID_GETSAVEDIMAGE, 1);
}

void sys_proc_upload_image(void *param)
{
}

void sys_proc_dump_image(void *data, unsigned int size)
{
	printf("%s\n", __func__);
}

void sys_send_factory_replay(int result)
{
	cmd_uart_send_mid_replay_data((uint8_t *)&result,
			4, MID_FACTORY_TEST, 1);
}

void sys_verify_enroll_proc(uint8_t enroll_mode, uint8_t enroll_dir,
		bool is_ap_cmd, uint16_t seconds)
{
	struct cmd_uart_msg_buf_t *msg_buf_p = &g_cmd_uart_msg_buf;

	if (seconds == 0)
		seconds = 5;

	pthread_mutex_lock(&msg_buf_p->mutex_lock);

	mcu_inf_clear_cmd_nolock();
	g_mcu_cmd->algo_enable = true;
	if (g_mcu_cmd->algo_enable_cb)
		g_mcu_cmd->algo_enable_cb(g_mcu_cmd->cb_data);
	struct timeval time_now;
	gettimeofday(&time_now, NULL);
	g_mcu_cmd->verify_timeout = time_now.tv_sec
		+ time_now.tv_usec / 1000000 + seconds;

	if (is_ap_cmd)
		g_mcu_cmd->ap_cmd_mode = true;

	if (enroll_mode)
		g_mcu_cmd->enroll_mode = enroll_mode;

	if (enroll_dir)
		g_mcu_cmd->enroll_dir = enroll_dir;

	pthread_mutex_unlock(&msg_buf_p->mutex_lock);
}

/* interface for system proc */
void sys_proc_verify_msg(void *param)
{
	s_msg_verify_data *verify_input =
		(s_msg_verify_data *)param;

	printf("verify input: timeout:%d\n", verify_input->timeout);

	if (mcu_ms_status_get() != MS_STANDBY) {
		printf("module busy\n");
		cmd_uart_send_mid_replay_data(
				(uint8_t *)NULL, 0, MID_VERIFY, MR_REJECTED);
		return;
	}

	sys_verify_enroll_proc(0, 0, false, verify_input->timeout);
}

void sys_proc_enroll_msg(void *param, uint8_t enroll_mode)
{
	s_msg_enroll_data *enroll = (s_msg_enroll_data *)param;
	printf("recv enroll: mode:%d admin:%d name:%s direction:%d time:%d\n",
			enroll_mode,
			enroll->admin,
			enroll->user_name,
			enroll->face_direction,
			enroll->timeout);

	if (mcu_ms_status_get() != MS_STANDBY) {
		cmd_uart_send_mid_replay_data(
				(uint8_t *)NULL, 0, enroll_mode, MR_REJECTED);
		return;
	}

	sys_verify_enroll_proc(enroll_mode,
			enroll->face_direction, false, enroll->timeout);
}

typedef void (*del_user_callback)(int id);
static del_user_callback del_user_cb = NULL;

void cmd_uart_set_del_usr_cb(void *param)
{
	del_user_cb = (del_user_callback)param;
}

void sys_proc_del_user_msg(int id)
{
	printf("sys_proc_del_all_user_msg\n");

	if (mcu_ms_status_get() != MS_STANDBY) {
		printf("module busy\n");
		cmd_uart_send_mid_replay_data(
				(uint8_t *)NULL, 0, MID_DELALL, MR_REJECTED);
		return;
	}

	//mcu_ms_status_set(MS_BUSY);
	int ret = 0;
	if (del_user_cb)
		del_user_cb(id);

	unsigned char data[2] = {0};
	data[0] = MID_DELALL;
	data[1] = ret;
	cmd_uart_send_mid_replay(data, 2);
	mcu_ms_status_set(MS_STANDBY);
}

int ap_proc_verify_cmd(uint16_t seconds)
{
	printf("ap verify input: seconds:%d\n", seconds);

	if (mcu_ms_status_get() != MS_STANDBY) {
		printf("module busy\n");
		return -1;
	}

	sys_verify_enroll_proc(0, 0, true, seconds);

	return 0;
}

int ap_proc_enroll_cmd(uint16_t seconds)
{
	printf("ap enroll: time:%d\n", seconds);

	if (mcu_ms_status_get() != MS_STANDBY) {
		return -1;
	}

	sys_verify_enroll_proc(MID_ENROLL_SINGLE, 0, true, seconds);

	return 0;
}

int ap_proc_enroll_multi_dir_cmd(uint16_t seconds, uint8_t enroll_dir)
{
	printf("ap enroll: time:%d\n", seconds);

	if (mcu_ms_status_get() != MS_STANDBY) {
		return -1;
	}

	sys_verify_enroll_proc(MID_ENROLL, enroll_dir, true, seconds);

	return 0;
}

int ap_proc_del_user_cmd(int id)
{
	printf("sys_proc_del_all_user_msg\n");

	if (mcu_ms_status_get() != MS_STANDBY) {
		printf("module busy\n");
		return -1;
	}

	mcu_ms_status_set(MS_BUSY);
	int ret = 0;
	if (del_user_cb)
		del_user_cb(id);

	mcu_ms_status_set(MS_STANDBY);
	return 0;
}

int ap_proc_face_exit(void)
{
	struct cmd_uart_msg_buf_t *msg_buf_p = &g_cmd_uart_msg_buf;

	pthread_mutex_lock(&msg_buf_p->mutex_lock);
	mcu_inf_clear_cmd_nolock();
	if (g_mcu_cmd->algo_exit_cb)
		g_mcu_cmd->algo_exit_cb();
	pthread_mutex_unlock(&msg_buf_p->mutex_lock);
	return 0;
}
