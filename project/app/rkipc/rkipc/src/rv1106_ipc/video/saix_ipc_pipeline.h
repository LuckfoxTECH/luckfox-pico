/*
 * Copyright (c) 2025 NeuralSense AI Private Limited
 * Trading as swatah.ai. All rights reserved.
 *
 * This file is part of the swatah.ai software stack and is licensed under
 * the terms defined in the accompanying LICENSE file. Unauthorized copying,
 * distribution, or modification of this file, via any medium, is strictly prohibited.
 *
 * For more information, visit: https://swatah.ai
*/
#ifndef _SAIX_IPC_PIPELINE_H_
#define _SAIX_IPC_PIPELINE_H_

#include "rk_common.h"
#include <pthread.h>
extern int pipe_id_;
extern MPP_CHN_S vi_chn, venc_chn, ivs_chn;

extern int enable_npu, g_enable_vo;
extern const char *tmp_smart;
extern const char *tmp_gop_mode;
extern const char *tmp_rc_quality;
extern const char *tmp_output_data_type;
extern const char *tmp_rc_mode;
extern const char *tmp_h264_profile;
extern pthread_t saix_thread_venc0_handle;
extern pthread_t vi_thread_1, venc_thread_1, venc_thread_2, jpeg_venc_thread_id,
                vpss_thread_rgb, cycle_snapshot_thread_id, get_nn_update_osd_thread_id,
                get_vi_send_jpeg_thread_id, get_vi_2_send_thread, get_ivs_result_thread;

int saix_setup_pipeline0();
int saix_teardown_pipeline0();
int saix_setup_pipe1() ;
int saix_teardown_pipe1();
int saix_setup_ivs_pipe(); 
int saix_teardown_ivs_pipe();
int saix_setup_jpeg_pipe();
int saix_teardown_jpeg_pipe();


#endif /* _SAIX_IPC_PIPELINE_H_ */