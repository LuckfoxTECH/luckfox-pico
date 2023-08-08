// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
int rkipc_audio_rtsp_init();
int rkipc_ao_init();
int rkipc_ao_deinit();
int rkipc_ao_write(unsigned char *data, int data_len);
int rkipc_audio_init();
int rkipc_audio_deinit();
// export api
int rk_audio_restart();
int rk_audio_get_bit_rate(int stream_id, int *value);
int rk_audio_set_bit_rate(int stream_id, int value);
int rk_audio_get_sample_rate(int stream_id, int *value);
int rk_audio_set_sample_rate(int stream_id, int value);
int rk_audio_get_volume(int stream_id, int *value);
int rk_audio_set_volume(int stream_id, int value);
int rk_audio_get_enable_vqe(int stream_id, int *value);
int rk_audio_set_enable_vqe(int stream_id, int value);
int rk_audio_get_encode_type(int stream_id, const char **value);
int rk_audio_set_encode_type(int stream_id, const char *value);
