// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
void rk_system_init();
void rk_system_deinit();
int rk_system_get_deivce_name(const char **value);
int rk_system_get_telecontrol_id(const char **value);
int rk_system_get_model(const char **value);
int rk_system_get_serial_number(const char **value);
int rk_system_get_firmware_version(const char **value);
int rk_system_get_encoder_version(const char **value);
int rk_system_get_web_version(const char **value);
int rk_system_get_plugin_version(const char **value);
int rk_system_get_channels_number(const char **value);
int rk_system_get_hard_disks_number(const char **value);
int rk_system_get_alarm_inputs_number(const char **value);
int rk_system_get_alarm_outputs_number(const char **value);
int rk_system_get_firmware_version_info(const char **value);
int rk_system_get_manufacturer(const char **value);
int rk_system_get_hardware_id(const char **value);
int rk_system_set_deivce_name(const char *value);
int rk_system_set_telecontrol_id(const char *value);
// action
int rk_system_reboot();
int rk_system_factory_reset();
int rk_system_export_log(const char *path);
int rk_system_export_db(const char *path);
int rk_system_import_db(const char *path);
int rk_system_upgrade(const char *path);
// user
int rk_system_get_user_num(int *value);
int rk_system_set_user_num(int value);
int rk_system_get_user_level(int id, int *value);
int rk_system_set_user_level(int id, int value);
int rk_system_get_user_name(int id, const char **value);
int rk_system_set_user_name(int id, const char *value);
int rk_system_set_password(int id, const char *value);
int rk_system_get_password(int id, const char **value);
int rk_system_add_user(int id, int user_level, const char *user_name, const char *password);
int rk_system_del_user(int id);
// capability
int rk_system_capability_get_video(char *value);
int rk_system_capability_get_image_adjustment(char *value);
int rk_system_capability_get_image_blc(char *value);
int rk_system_capability_get_image_enhancement(char *value);
int rk_system_capability_get_image_exposure(char *value);
int rk_system_capability_get_image_night_to_day(char *value);
int rk_system_capability_get_image_video_adjustment(char *value);
int rk_system_capability_get_image_white_blance(char *value);