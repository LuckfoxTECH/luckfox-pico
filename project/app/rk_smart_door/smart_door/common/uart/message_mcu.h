/*
 * Copyright (c) 2018. Sensetime Technology Co.
 *
 * All Rights Reserved.
 *
 * Confidential and Proprietary - Sensetime Technology Co.
 */

/*
 * =====================================================================================
 *    Description:  Local message definitions
 *        Version:  1.0
 *         Author:  Xie, Hongbiao
 *   Organization:  Sensetime
 * =====================================================================================
 */

#ifndef __MESSAGE_MCU_H__
#define __MESSAGE_MCU_H__
#include "message_mcu_v1.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Internal message and data structure definitions
 * which is not expected to be exposed to customer
 */

#define RGB_IMAGE_NAME_SIZE 32

/* communication message ID definitions */
#define MID_GETIMAGE 0x14                     // to get the current image captured by the camera
#define MID_GETRAWIMAGE 0x15                  // to get the raw image captured by the camera
#define MID_PLAY 0x1B                         // to play an saved audio/video media file
#define MID_STOP_PLAYBACK 0x1C                // to stop the runing media playback
#define MID_ENROLL_RGB 0x1E                   // enroll with rgb image
#define MID_REFRESH_DATA 0x28                 // refresh data zone
#define MID_WRITE_SN 0x31                     // write sn to board, for factory production
#define MID_CHANGE_RELEASE_ENC_KEY 0x54       // change release encrypted key
#define MID_SET_RGBIMAGE_SIZE 0x88            // set rgbimage size
#define MID_TRANSFER_RGBIMAGE 0x89            // transfer rgbimage
#define MID_FACTORY_TEST 0xC0                 // factory test
#define MID_DDR_TEST 0xC1                     // DDR test
#define MID_MIC_TEST 0xC2                     // mic test
#define MID_SPEAKER_TEST 0xC3                 // speaker test
#define MID_MIC_RIGHT_TEST 0xC4               // mic right channel test
#define MID_RGB_SENSOR_TEST 0xC5              // rgb sensor test
#define MID_IR_CUT_LED_TEST 0xC6              // rgb module test including ircut irled
#define MID_DEL_MEDIA_FILE 0xC7               // delete media files
#define MID_GET_LAST_RGB_IMAGE_SIZE 0xC8      // get last rgb image size
#define MID_UPLOAD_LAST_RGB_IMAGE 0xC9        // upload last rgb image
#define MID_RGB_MODULE_TEST 0xCA              // take a photo to test rgb module
#define MID_CONTROL_IR_LED 0xCB               // open or close ir led on the rgb module
#define MID_LIGHT_SENSOR_TEST 0xCC            // light sensor test
#define MID_SINGLE_TEST 0xD0                  // Pick one image and save it
#define MID_CONTINUE_TEST 0xD1                // Pick images one by one
#define MID_STOP_TEST 0xD2                    // Stop CONTINUE_TEST
#define MID_SET_INTERACTIVE 0xD3              // Set Interactive parameter
#define MID_SET_THRESHOLD_LEVEL 0xD4          // Set threshold level
#define MID_GET_THRESHOLD_LEVEL 0xD5          // Set threshold level
#define MID_REBOOT_CMD 0xDA                   //reboot the device
#ifdef ENABLE_DAILY_TEST
#define ID_START_DAILY_TEST 0xE0
#define ID_STOP_DAILY_TEST 0xE1
#endif
/* communication message ID definitions END */

/* note ID definitions */
#define NID_STATISTICALDATA 9                 // statistical information
/* note ID definitions END */

/* message result code */
#define MR_FAILED4_CAMERAFOV 15               // camera fov test failed
#define MR_FAILED4_CAMERABRT 16               // image brightness test failed
#define MR_FAILED4_CAMERASTRU 17              // camera structure test failed
#define MR_FAILED4_BOOT_TIMEOUT 18            // boot up timeout
#define MR_FAILED4_WITHOUT_SMILE 22           // verify successful with smile
#define MR_FAILED4_LIGHT_SENSOR 24            // light sensor does not working
#define MR_FAILED4_CAMERAVRF 31               // camera verify test failed
/* message result code END */

/* msg face enroll type */
#define FACE_ENROLL_TYPE_RGB 2                // enroll with one frame image
/* msg face enroll type END */

/* message: host -> module */
typedef struct {
    uint8_t ddr_test_counts; // number of DDR testing
} s_msg_ddr_test_data;

typedef struct {
    uint8_t enable_camera_test;   // 0:disable camera test  !0:enable camera test
} s_msg_factory_test_data;

typedef struct {
    uint8_t upload_image_offset[4]; // upload image offset, int -> [o1 o2 o3 o4]
    uint8_t upload_image_size[4];   // upload image size, int -> [s1 s2 s3 s4]
} s_msg_upload_last_rgb_image_data;

typedef struct {
    uint8_t open_ir_led;    // 0:close ir led  1:open it led
} s_msg_control_ir_led_data;

typedef struct {
    uint8_t open_ir_cut;    // 0:close ir cut  1:open it cut
} s_msg_rgb_module_test_data;

typedef struct {
    uint8_t capture_time_s;
} s_msg_mic_test_data;

typedef struct {
    uint8_t media_type;       // 0:audio 1:image 2:video 3:all
} s_msg_del_media_file_data;

typedef struct {
    uint8_t enable_pose_single;     // enable pose detection in single test
} s_msg_single_test_data;

typedef struct {
    uint8_t enable_pose_continue;   // enable pose detection in continue test
} s_msg_continue_test_data;

// set RGBImage Size
typedef struct {
    uint8_t rgbimage_size[4];   // image size int -> [s1, s2, s3, s4]
} s_msg_set_rgbimage_size_data;

// transfer rgbimage data
typedef struct {
    uint8_t rgbimage_offset[4]; // offset of rgbimage data for this package data
    uint8_t rgbimage_size[4];   // size of rgbimage data for this package data
    uint8_t data;
} s_msg_transfer_rgbimage_data;

typedef struct {
    uint8_t range_left_min;     // left face range in [0, 90]锛�
    uint8_t range_left_max;     // left face range in [range_left_min, 90]
    uint8_t range_right_min;    // right face range in [0, 90]
    uint8_t range_right_max;    // right face range in [range_right_min, 90]
    uint8_t range_up_min;       // up face range in [0, 90]
    uint8_t range_up_max;       // up face range in [range_top_min, 90]
    uint8_t range_down_min;     // down face range in [0, 90]
    uint8_t range_down_max;     // down face range in [range_bottom_min, 90]
} s_msg_interactive_param;

typedef struct {
    uint8_t verify_threshold_level;       // level 0~4, safety from low to high, default 2
    uint8_t liveness_threshold_level;     // level 0~4, safety from low to high, default 2
} s_msg_algo_threshold_level;

// trigger audio/video playback
typedef struct {
    uint8_t media_type;
} s_msg_trigger_playback_data;

// stop audio/video playback
typedef struct {
    uint8_t media_type;
} s_msg_stop_playback_data;
/* message: host -> module END*/

/* message: module -> host */
typedef struct {
    uint8_t time_heb;      // high eight bits of factory test time
    uint8_t time_leb;      // low eight bits of facetory test time
} s_msg_reply_factory_test_data;

typedef struct {
    uint8_t image_name[RGB_IMAGE_NAME_SIZE];
    uint8_t image_size[4];   // image size int -> [s1, s2, s3, s4]
} s_msg_reply_get_last_rgb_image_data;

typedef struct {
    uint8_t image_size[4];   // image size int -> [s1, s2, s3, s4]
} s_msg_reply_get_image_data;

typedef struct {
    uint8_t image_size[4];   // image size int -> [s1, s2, s3, s4]
} s_msg_reply_get_raw_image_data;

typedef struct {
    uint8_t log_size[4];   // log size int -> [s1, s2, s3, s4]
} s_msg_reply_get_log_data;

typedef struct {
    uint8_t light_sensor_value[4];   // light sensor value int -> [s1, s2, s3, s4]
} s_msg_reply_light_sensor_test_data;
/* message: module -> host END */

/* note: module -> host */
typedef struct {
    uint16_t total_time;   // total enrolling or verifing time
    uint16_t single_time;  // single enrolling or verifing time
    uint16_t frames;       // frames of enrolling or verifing
} s_note_data_statistical;
/* note: module -> host END */

typedef struct {
    uint8_t enter_maskrom; // default 0: only reboot device; > 0: reboot to maskrom mode
} s_msg_reboot_data;

#ifdef __cplusplus
}
#endif
#endif// __SENSE_LOCK_MESSAGE_L_H__
