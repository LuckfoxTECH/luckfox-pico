// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __FACE_CLIENT_H__
#define __FACE_CLIENT_H__


// enroll face dir define
#define FACE_DIRECTION_UP       0x10       // face up
#define FACE_DIRECTION_DOWN     0x08       // face down
#define FACE_DIRECTION_LEFT     0x04       // face left
#define FACE_DIRECTION_RIGHT    0x02       // face right
#define FACE_DIRECTION_MIDDLE   0x01       // face middle
#define FACE_DIRECTION_UNDEFINE 0x00       // face undefine

// face state define
typedef enum _EN_FACE_STATE {
	FACE_STATE_NORMAL                    = 0, // normal state, the face is available
	FACE_STATE_NOFACE                    = 1, // no face detected
	FACE_STATE_TOOUP                     = 2, // face is too up side
	FACE_STATE_TOODOWN                   = 3, // face is too down side
	FACE_STATE_TOOLEFT                   = 4, // face is too left side
	FACE_STATE_TOORIGHT                  = 5, // face is too right side
	FACE_STATE_TOOFAR                    = 6, // face is too far
	FACE_STATE_TOOCLOSE                  = 7, // face is too near
	FACE_STATE_EYEBROW_OCCLUSION         = 8, // eyebrow occlusion
	FACE_STATE_EYE_OCCLUSION             = 9, // eye occlusion
	FACE_STATE_FACE_OCCLUSION            = 10,// face occlusion
	FACE_STATE_DIRECTION_ERROR           = 11,// face direction error
	FACE_STATE_EYE_CLOSE_STATUS_OPEN_EYE = 12,// open eye in the eye close status
	FACE_STATE_EYE_CLOSE_STATUS          = 13,// confirm eye close status
	FACE_STATE_EYE_CLOSE_UNKNOW_STATUS   = 14,// eye close unknow status
} EN_FACE_STATE;

// result define
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

/*****************************
  server msg callback init
 *****************************/
typedef void (*module_ready_cb_t)(void);
typedef void (*verity_result_cb_t)(int result, int usr_id);
typedef void (*enroll_result_cb_t)(int result, int usr_id);
typedef void (*face_state_cb_t)(int result);

void set_module_ready_cb(module_ready_cb_t cb);
void set_verify_result_cb(verity_result_cb_t cb);
void set_enroll_result_cb(enroll_result_cb_t cb);
void set_face_state_cb(face_state_cb_t cb);
/*****************************
  client cmd
 *****************************/
int set_module_face_verify(int seconds);
int set_module_face_enroll(int seconds);
int set_module_clr_all_face(void);
int set_module_clr_face_id(int face_id);
int set_module_exit_face(void);
int set_module_face_enroll_multi_dir(int seconds, int dir);

/*****************************
  init
 *****************************/
int face_client_init(void);
int face_client_check_connect(void);
int face_client_deinit(void);

#endif
