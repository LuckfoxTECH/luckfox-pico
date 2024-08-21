#ifndef __MCU_INF_H__
#define __MCU_INF_H__

#include "message_mcu_v1.h"
#include "message_mcu.h"
#include "uart_parser.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

enum rksl_door_status
{
	RKSL_DOOR_ST_IDLE,
	RKSL_DOOR_ST_BUSY,
	RKSL_DOOR_ST_OTA,
	RKSL_DOOR_ST_OTA_RECV_HEAD,
	RKSL_DOOR_ST_OTA_RECV_PACKET,
	RKSL_DOOR_ST_OTA_WRITE_FLASH,
	RKSL_DOOR_ST_VERIFY
};

typedef struct mcu_ota_info_s
{
    int ota_packet_cnt;
    int ota_total_packet_num;
    int ota_total_file_size;
    int ota_file_size_cnt;
    s_msg_otaheader_data otahead_data;
    uint8_t *ota_data;
} mcu_ota_info_t;

typedef struct gesture_info_s
{
    int cnt;
    s_msg_reply_getuserinfo_data guest[MAX_USER_COUNTS];
} gesture_info_t;


typedef struct mcu_info_s
{
    int rksl_door_ms;
    enum rksl_door_status mcu_state;
    struct mcu_ota_info_s ota;
    s_msg_reply_all_userid_data user_info;
    struct gesture_info_s gesture_info;
    s_msg_algo_threshold_level algo_threshold_level;
} mcu_info_t;

typedef int (*algo_enable_callback)(void* param);
typedef void (*algo_exit_callback)(void);
typedef struct mcu_cmd_s
{
	bool algo_enable;
	unsigned int verify_timeout;
	bool ap_cmd_mode;
	uint8_t enroll_mode;
	uint8_t enroll_dir;

	char fix; /* below use fix data */
	algo_exit_callback algo_exit_cb;
	algo_enable_callback algo_enable_cb;
	void *cb_data;
} mcu_cmd_t;

typedef enum _EN_FACE_STATE {
	FACE_STATE_NORMAL     				 = 0, 	// normal state, the face is available
	FACE_STATE_NOFACE     				 = 1, 	// no face detected
	FACE_STATE_TOOUP      				 = 2, 	// face is too up side
	FACE_STATE_TOODOWN    				 = 3, 	// face is too down side
	FACE_STATE_TOOLEFT    				 = 4, 	// face is too left side
	FACE_STATE_TOORIGHT   				 = 5, 	// face is too right side
	FACE_STATE_TOOFAR     				 = 6, 	// face is too far
	FACE_STATE_TOOCLOSE   				 = 7, 	// face is too near
	FACE_STATE_EYEBROW_OCCLUSION 		 = 8,   // eyebrow occlusion
	FACE_STATE_EYE_OCCLUSION     		 = 9,   // eye occlusion
	FACE_STATE_FACE_OCCLUSION    		 = 10,  // face occlusion
	FACE_STATE_DIRECTION_ERROR  		 = 11,  // face direction error
	FACE_STATE_EYE_CLOSE_STATUS_OPEN_EYE = 12,  // open eye in the eye close status
	FACE_STATE_EYE_CLOSE_STATUS          = 13,  // confirm eye close status
	FACE_STATE_EYE_CLOSE_UNKNOW_STATUS   = 14   // eye close unknow status
} EN_FACE_STATE;

typedef struct {
	int16_t state; // corresponding to FACE_STATE_*
	// position
	int16_t left; // in pixel
	int16_t top;
	int16_t right;
	int16_t bottom;
	// pose
	int16_t yaw; // up and down in vertical orientation
	int16_t pitch; // right or left turned in horizontal orientation
	int16_t roll; // slope
	//u16 resv[56];     //128-16 bytes
} s_note_data_face;

int msg_mcu_receive();
int msg_mcu_buf_empty_check();

void mcu_ms_status_set(int status);
int mcu_ms_status_get();

void mcu_inf_clear_cmd(void);
void mcu_inf_set_cmd_data(struct mcu_cmd_s *cmd);

void sys_proc_get_save_image(void *param);
void sys_proc_get_save_image_replay(void *param);
void sys_proc_upload_image(void *param);
void sys_proc_ota_start(void);
void sys_proc_dump_image(void *data, unsigned int size);
void sys_send_factory_replay(int result);
void cmd_uart_set_del_usr_cb(void *param);

int ap_proc_verify_cmd(uint16_t seconds);
int ap_proc_enroll_cmd(uint16_t seconds);
int ap_proc_enroll_multi_dir_cmd(uint16_t seconds, uint8_t enroll_dir);
int ap_proc_del_user_cmd(int id);
int ap_proc_face_exit(void);
#ifdef __cplusplus
}
#endif
#endif
