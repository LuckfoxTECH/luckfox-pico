/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 *
 */

#ifndef __CMD_UART_PARSER_H__
#define __CMD_UART_PARSER_H__

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* legacy, will be removed in the future */
typedef unsigned int        uint32_t;
typedef signed int      int32_t;
typedef unsigned short      uint16_t;
typedef signed short        int16_t;
typedef unsigned char       uint8_t;
typedef signed char     int8_t;
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;

enum EN_MOD_STATUS {
	MS_STANDBY 					= 0,  // IDLE, waiting for commands
	MS_BUSY 					= 1,  // in working of processing commands
	MS_ERROR 					= 2,  // in error state. can't work properly
	MS_INVALID 					= 3,  // not initialized
	MS_OTA 						= 4   // in ota state
};

enum EN_WK_MOD_STATUS {
	WKMOD_NULL					= 0,
	WKMOD_RECOGNIZE				= 1,
	WKMOD_ENROLL				= 2,
	WKMOD_ENROLL_SINGLE			= 3
};

enum EN_MMI_RET {
	MR_SUCCESS    						= 0, // success
	MR_REJECTED   						= 1, // module rejected this command
	MR_ABORTED    						= 2, // algo aborted
	MR_FAILED4_CAMERA 					= 4, // camera open failed
	MR_FAILED4_UNKNOWNREASON 			= 5, // UNKNOWN_ERROR
    MR_FAILED4_INVALIDPARAM 			= 6, // invalid param
	MR_FAILED4_NOMEMORY 				= 7, // no enough memory
	MR_FAILED4_UNKNOWNUSER 				= 8, // no user enrolled
	MR_FAILED4_MAXUSER 					= 9, // exceed maximum user number
	MR_FAILED4_FACEENROLLED 			= 10,// this face has been enrolled
	MR_FAILED4_LIVENESSCHECK 			= 12,// liveness check failed
	MR_FAILED4_TIMEOUT 					= 13,// exceed the time limit
	MR_FAILED4_AUTHORIZATION 			= 14,// authorization failed
	MR_FAILED4_READ_FILE 				= 19,// read file failed
	MR_FAILED4_WRITE_FILE 				= 20,// write file failed
	MR_FAILED4_NO_ENCRYPT 				= 21,// encrypt must be set
	MR_FAILED4_NO_RGBIMAGE				= 23 // rgb image is not ready
};

enum EN_MMI_CMD {
	// Module to Host (m->h)
	MID_REPLY 					= 0x00,     // request(command) reply message, success with data or fail with reason
	MID_NOTE  					= 0x01,     // note to host e.g. the position or angle of the face
	MID_IMAGE 					= 0x02,     // send image to host
	// Host to Module (h->m)
	MID_RESET 					= 0x10,     // stop and clear all in-processing messages. enter standby mode
	MID_GETSTATUS 				= 0x11, 	// to ping the module and get the status
	MID_VERIFY 					= 0x12,    	// to verify the person in front of the camera
	MID_ENROLL 					= 0x13,     // to enroll and register the persion in front of the camera
	MID_ENROLL_SINGLE 			= 0x1D,     // to enroll and register the persion in front of the camera, with one frame image
	MID_ENROLL_ITG 				= 0x26,     // Integrated enroll message, support all existing enroll type
	MID_SNAPIMAGE 				= 0x16,		// to snap a picture and save it
	MID_GETSAVEDIMAGE 			= 0x17, 	// to get size of saved image
	MID_UPLOADIMAGE 			= 0x18,   	// upload images
	MID_DELUSER 				= 0x20,   	// Delete the specified user with user id
	MID_DELALL  				= 0x21,   	// Delete all registerred users
	MID_GETUSERINFO 			= 0x22,   	// Get user info
	MID_FACERESET 				= 0x23,     // Reset face status
	MID_GET_ALL_USERID 			= 0x24,  	// get all users ID
	MID_GET_VERSION 			= 0x30,   	// get version information
	MID_START_OTA 				= 0x40,     // ask the module to enter OTA mode
	MID_STOP_OTA 				= 0x41,     // ask the module to exit OTA mode
	MID_GET_OTA_STATUS 			= 0x42,		// query the current ota status
	MID_OTA_HEADER 				= 0x43,    	// the ota header data
	MID_OTA_PACKET 				= 0x44,     // the data packet, carries real firmware data
	MID_INIT_ENCRYPTION 		= 0x50,     // initialize encrypted communication
	MID_CONFIG_BAUDRATE     	= 0x51,     // config uart baudrate
	MID_SET_RELEASE_ENC_KEY 	= 0x52, 	// set release encrypted key(Warning!!!:Once set, the KEY will not be able to modify)
	MID_SET_DEBUG_ENC_KEY   	= 0x53,   	// set debug encrypted key
	MID_GET_LOGFILE 			= 0x60,     // get log file
	MID_UPLOAD_LOGFILE  		= 0x61,     // upload log file
	MID_FACTORY_RESET			= 0xEC,		// gm_cmd, reset uid = 0
	MID_POWERDOWN 				= 0xED,     // be prepared to power off
	MID_DEBUG_MODE 				= 0xF0,     // enter debug mode
	MID_GET_DEBUG_INFO     		= 0xF1,     // get size of debug information
	MID_UPLOAD_DEBUG_INFO  		= 0xF2,   	// upload debug information
	MID_GETLIBRARY_VERSION 		= 0xF3,  	// Get library version
	MID_DEMOMODE 				= 0xFE,     // enter demo mode, verify flow will skip feature comparation step.

	MID_GETIMAGE 				= 0x14,     // to get the current image captured by the camera
	MID_GETRAWIMAGE 			= 0x15,     // to get the raw image captured by the camera
	MID_PLAY 					= 0x1B,     // to play an saved audio/video media file
	MID_STOP_PLAYBACK 			= 0x1C,     // to stop the runing media playback
	MID_ENROLL_RGB 				= 0x1E,     // enroll with rgb image
	MID_WRITE_SN 				= 0x31,     // write sn to board, for factory production
	MID_CHANGE_RELEASE_ENC_KEY  = 0x54,     // change release encrypted key
	MID_ENABLE_ATTRIBUTE 		= 0x80,     // enable attribute function
	MID_SET_RGBIMAGE_SIZE 		= 0x88,     // set rgbimage size
	MID_TRANSFER_RGBIMAGE 		= 0x89,     // transfer rgbimage
	MID_FACTORY_TEST 			= 0xC0,     // factory test
	MID_DDR_TEST 				= 0xC1,     // DDR test
	MID_MIC_TEST 				= 0xC2,     // mic test
	MID_SPEAKER_TEST 			= 0xC3,     // speaker test
	MID_MIC_RIGHT_TEST 			= 0xC4,     // mic right channel test
	MID_RGB_SENSOR_TEST 		= 0xC5,     // rgb sensor test
	MID_IR_CUT_LED_TEST 		= 0xC6,     // rgb module test including ircut irled
	MID_DEL_MEDIA_FILE 			= 0xC7,     // delete media files
	MID_GET_LAST_RGB_IMAGE_SIZE = 0xC8,     // get last rgb image size
	MID_UPLOAD_LAST_RGB_IMAGE   = 0xC9,     // upload last rgb image
	MID_RGB_MODULE_TEST 		= 0xCA,     // take a photo to test rgb module
	MID_CONTROL_IR_LED 			= 0xCB,     // open or close ir led on the rgb module
	MID_LIGHT_SENSOR_TEST 		= 0xCC,     // light sensor test
	MID_SINGLE_TEST 			= 0xD0,     // Pick one image and save it
	MID_CONTINUE_TEST 			= 0xD1,     // Pick images one by one
	MID_STOP_TEST 				= 0xD2,     // Stop CONTINUE_TEST
	MID_SET_INTERACTIVE 		= 0xD3,     // Set Interactive parameter
	MID_SET_THRESHOLD_LEVEL 	= 0xD4,     // Set threshold level
	MID_REBOOT_CMD 				= 0xDA,     // reboot the device
	MID_MAX 					= 0xFF      // reserved
};

typedef enum _EN_NID {
	NID_READY	       		    = 0,
	NID_FACE_STATE				= 1,
	NID_UNKNOWNERROR			= 2,
	NID_OTA_DONE				= 3,
	NID_EYE_STATE				= 4,
	NID_MAX						= 5
}EN_NID;

typedef enum _EN_LOCKSTATUS {
	ST_FACE_MODULE_STATUS_UNLOCK_OK   			 = 0xC8,
	ST_FACE_MODULE_STATUS_UNLOCK_WITH_EYES_CLOSE = 0xCC
}EN_LOCKSTATUS;

enum EN_ENCTYPE {
	ENCRYPT_TYPE_NULL 					= 0x00,
	ENCRYPT_TYPE_AES128					= 0x01,
	ENCRYPT_TYPE_XOR					= 0x02
};

struct frm_fmt {
	unsigned char head1;
	unsigned char head2;
	unsigned char msgid;
	unsigned char len_hb;
	unsigned char len_lb;
	unsigned char *pdata;
	unsigned char cs;
};


int  cmd_uart_postinit();
void cmd_uart_parser(unsigned char str);
void cmd_uart_event_handle();

/* interface for system */
void sys_send_system_ready_msg(void);
void sys_send_recognize_result_msg(void* param);
void sys_send_enroll_result_msg(int16_t enroll_mode, int16_t result, int16_t usr_id, unsigned char dir);
void sys_send_facestate_result_msg(void *param);
void sys_send_del_user_result_msg(void* param);
void sys_send_get_userinfo_result_msg(void* param);
void sys_send_get_all_userid_result_msg(void *param);
void sys_send_factory_reset_result_msg(void *param);
void sys_send_verify_result_msg(int16_t result, int16_t usr_id);


void sys_proc_verify_msg(void *param);
void sys_proc_recognize_msg(void *param);
void sys_proc_enroll_msg(void *param, uint8_t enroll_mode);
void sys_proc_facereset_msg(void *param);
void sys_proc_del_user_msg(int id);
void sys_proc_get_userinfo_msg(void *param);
void sys_proc_get_all_userid_msg(void *param);

void sys_proc_set_aes_keysel(uint8_t *keysel, unsigned char debug);
void sys_proc_init_encryption(void *param);

#ifdef EN_UPLOAD_IMG
int  sys_proc_get_saved_img(void *param);
void sys_proc_upload_img(void *param);
void sys_proc_dump_image(void *data, unsigned int size);
#endif
void sys_proc_factory_reset_msg(void *param);


#define CMD_MAX_LEN (128)
#define CMD_UART_MAX (8)

enum cmd_uart_state
{

    CMD_UART_STATE_WAIT,
    CMD_UART_STATE_HEAD_MSG_ID,
    CMD_UART_STATE_HEAD_SIZE,
    CMD_UART_STATE_HEAD_RCV_DATA,
    CMD_UART_STATE_HEAD_RCV_CRC,
    CMD_UART_STATE_HEAD_ENC_SIZE,
    CMD_UART_STATE_HEAD_ENC_DATA,
    CMD_UART_STATE_HEAD_ENC_CRC
};

struct cmd_uart_msg_head_t
{
    unsigned char msg_id;
    unsigned short msg_size;
    unsigned char *msg_data;
    unsigned char crc;
};

struct cmd_uart_msg_buf_t
{
	struct cmd_uart_msg_head_t cmd[8];
	short front_num;
	short back_num;
    int full_flag;
	pthread_mutex_t mutex_lock;
};

void cmd_uart_parser(unsigned char str);
void cmd_uart_init(void* param);
void cmd_uart_deinit(void);
void cmd_uart_send_nid_ready();
void cmd_uart_send_nid_unknownerror();
void cmd_uart_send_nid_ota_done();

void cmd_uart_send_mid_replay(uint8_t *data, uint32_t data_len);
void cmd_uart_send_mid_replay_gesture_info(uint8_t *data, uint32_t data_len, uint8_t result);
void cmd_uart_send_mid_replay_alluser_id(uint8_t *data, uint32_t data_len, uint8_t result);
void cmd_uart_send_mid_replay_data(uint8_t *data, uint32_t data_len, uint8_t mid_type, uint8_t result);

void cmd_uart_send_mid_note_replay(uint8_t nid_type, uint8_t *data, uint32_t data_len);
void cmd_uart_send_data_direct(uint8_t *data, uint32_t data_len,uint8_t mid_type, uint8_t result);

extern struct cmd_uart_msg_buf_t g_cmd_uart_msg_buf;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __CMD_UART_PARSER_H__ */
