// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __UNLOCK_ALGO_H__
#define __UNLOCK_ALGO_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum unlock_algo_state_enum {
	UNLOCK_RESULT_DETECT_FACE_OK,     //= 0,
	UNLOCK_RESULT_NO_FACE,            //= 1,
	UNLOCK_RESULT_BIG_ANGLE,          //= 2,
	UNLOCK_RESULT_IR_FAIL,            //= 3,
	UNLOCK_RESULT_DEPTH_FAIL,         //= 4,
	UNLOCK_RESULT_TOO_CLOSE,          //= 5,
	UNLOCK_RESULT_TOO_FAR,            //= 6,
	UNLOCK_RESULT_TOO_LEFT,           //= 7,
	UNLOCK_RESULT_TOO_RIGHT,          //= 8,
	UNLOCK_RESULT_TOO_HIGH,           //= 9,
	UNLOCK_RESULT_TOO_LOW,            //= 10,
	UNLOCK_RESULT_NO_ID,              //= 11,
	UNLOCK_RESULT_COMPARE_FACE_OK,    //= 12,
	UNLOCK_RESULT_SAVE_FACE_OK,       //= 13,
	UNLOCK_RESULT_SAVE_FACE_FAIL,     //= 14,
	UNLOCK_RESULT_NULL,               //= 15,
	UNLOCK_RESULT_FAS_OK,             //= 16,
	UNLOCK_RESULT_FACE_EXIST,         //= 17,
	UNLOCK_RESULT_REGISTER_FULL,      //= 18,
	UNLOCK_RESULT_IR_OK,              //= 19
	UNLOCK_RESULT_DEPTH_OK,           //= 20
	UNLOCK_RESULT_DIR_ERROR,          //=21
} unlock_algo_res;

void unlock_algo_init(int width, int height);
int unlock_algo_process(
		short *raw_ir, short *raw_pro,
		int ir_fd, int pro_fd,
		int *face_id, uint8_t enroll_mode, uint8_t dir);
int unlock_algo_yuv_process(
		char *raw_ir, char *raw_pro,
		int ir_fd, int pro_fd,
		int *face_id, uint8_t enroll_mode, uint8_t dir);
void unlock_algo_clear_face(int id);
void unlock_algo_deinit();

void get_face_rect(int *xmin, int *ymin, int *xmax, int *ymax);
float unlock_cal_target_exp(uint16_t *pro, int pro_fd, int width, int height);
int dump_raw(void *input, int size, const char *name);
int read_test_val(const char *path);
#ifdef __cplusplus
} //extern "C"
#endif

#endif
