// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "log.h"
#include "param.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "system.c"

void rk_system_init() { LOG_DEBUG("%s\n", __func__); }

void rk_system_deinit() { LOG_DEBUG("%s\n", __func__); }

int rk_system_get_deivce_name(const char **value) {
	*value = rk_param_get_string("system.device_info:deivce_name", NULL);

	return 0;
}

int rk_system_get_telecontrol_id(const char **value) {
	*value = rk_param_get_string("system.device_info:telecontrol_id", NULL);

	return 0;
}

int rk_system_get_model(const char **value) {
	*value = rk_param_get_string("system.device_info:model", NULL);

	return 0;
}

int rk_system_get_serial_number(const char **value) {
	*value = rk_param_get_string("system.device_info:serial_number", NULL);

	return 0;
}

int rk_system_get_firmware_version(const char **value) {
	*value = rk_param_get_string("system.device_info:firmware_version", NULL);

	return 0;
}

int rk_system_get_encoder_version(const char **value) {
	*value = rk_param_get_string("system.device_info:encoder_version", NULL);

	return 0;
}

int rk_system_get_web_version(const char **value) {
	*value = rk_param_get_string("system.device_info:web_version", NULL);

	return 0;
}

int rk_system_get_plugin_version(const char **value) {
	*value = rk_param_get_string("system.device_info:plugin_version", NULL);

	return 0;
}

int rk_system_get_channels_number(const char **value) {
	*value = rk_param_get_string("system.device_info:channels_number", NULL);

	return 0;
}

int rk_system_get_hard_disks_number(const char **value) {
	*value = rk_param_get_string("system.device_info:hard_disks_number", NULL);

	return 0;
}

int rk_system_get_alarm_inputs_number(const char **value) {
	*value = rk_param_get_string("system.device_info:alarm_inputs_number", NULL);

	return 0;
}

int rk_system_get_alarm_outputs_number(const char **value) {
	*value = rk_param_get_string("system.device_info:alarm_outputs_number", NULL);

	return 0;
}

int rk_system_get_firmware_version_info(const char **value) {
	*value = rk_param_get_string("system.device_info:firmware_version_info", NULL);

	return 0;
}

int rk_system_get_manufacturer(const char **value) {
	*value = rk_param_get_string("system.device_info:manufacturer", NULL);

	return 0;
}

int rk_system_get_hardware_id(const char **value) {
	*value = rk_param_get_string("system.device_info:hardware_id", NULL);

	return 0;
}

int rk_system_set_deivce_name(const char *value) {
	rk_param_set_string("system.device_info:deivce_name", value);
	return 0;
}

int rk_system_set_telecontrol_id(const char *value) {
	rk_param_set_string("system.device_info:telecontrol_id", value);
	return 0;
}

// action

int rk_system_reboot() {
	system("reboot");
	return 0;
}

int rk_system_factory_reset() {
	system("cp /tmp/rkipc-factory-config.ini /userdata/rkipc.ini");
	system("sync");
	system("reboot");
	// system("update factory");
	return 0;
}

int rk_system_export_log(const char *path) {
	char cmd[128] = {'\0'};
	char *cmd_list[] = {
	    "cat /tmp/messages",
	    "cat /proc/uptime",
	    "cat /proc/meminfo",
	    "cat /proc/net/snmp",
	    "cat /proc/interrupts",
	    "top -b -n 1",
	    "cat /sys/class/net/eth0/speed",
	    "netstat -an",
	    "ifconfig -a",
	    "route -n",
	    "cat /etc/resolv.conf",
	    "cat /proc/net/wireless",
	};
	if (!access(path, F_OK)) {
		snprintf(cmd, 127, "rm -f %s", path);
		system(cmd);
	}
	for (int i = 0; i < sizeof(cmd_list) / sizeof(cmd_list[0]); i++) {
		snprintf(cmd, 127, "echo %s >> %s", cmd_list[i], path);
		system(cmd);
		snprintf(cmd, 127, "%s >> %s", cmd_list[i], path);
		system(cmd);
	}
	return 0;
}

int rk_system_export_db(const char *path) {
	char cmd[128] = {'\0'};
	snprintf(cmd, 127, "cp /usr/share/rkipc.ini %s", path);
	LOG_INFO("cmd is %s\n", cmd);
	system(cmd);
	return 0;
}

int rk_system_import_db(const char *path) {
	char cmd[128] = {'\0'};
	snprintf(cmd, 127, "cp %s /usr/share/rkipc.ini", path);
	LOG_INFO("cmd is %s\n", cmd);
	system(cmd);
	rk_param_reload();
	rk_system_reboot();
	return 0;
}

int rk_system_upgrade(const char *path) {
	char cmd[128] = {'\0'};
	if (path)
		snprintf(cmd, 127, "updateEngine --image_url=%s --misc=update --savepath=%s --reboot &",
		         path, path);
	else
		return -1;
	LOG_INFO("cmd is %s\n", cmd);

	return system(cmd);
}

// user

int rk_system_get_user_num(int *value) {
	*value = rk_param_get_int("system.device_info:user_num", 1);

	return 0;
}

int rk_system_set_user_num(int value) {
	rk_param_set_int("system.device_info:user_num", value);

	return 0;
}

int rk_system_get_user_level(int id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "user.%d:user_level", id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_system_set_user_level(int id, int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "user.%d:user_level", id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_system_get_user_name(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "user.%d:user_name", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_system_set_user_name(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "user.%d:user_name", id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_system_get_password(int id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "user.%d:password", id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_system_set_password(int id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "user.%d:password", id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_system_add_user(int id, int user_level, const char *user_name, const char *password) {
	int user_num;
	char entry[128] = {'\0'};
	char user_level_char[3] = {'\0'};

	snprintf(entry, 127, "user.%d", id);
	iniparser_set(g_ini_d_, entry, NULL);
	snprintf(entry, 127, "user.%d:user_level", id);
	snprintf(user_level_char, 2, "%d", user_level);
	iniparser_set(g_ini_d_, entry, user_level_char);
	snprintf(entry, 127, "user.%d:user_name", id);
	iniparser_set(g_ini_d_, entry, user_name);
	snprintf(entry, 127, "user.%d:password", id);
	iniparser_set(g_ini_d_, entry, password);

	rk_system_get_user_num(&user_num);
	user_num += 1;
	rk_system_set_user_num(user_num);
	rk_param_save();

	return 0;
}

int rk_system_del_user(int id) {
	int user_num;
	char entry[128] = {'\0'};

	snprintf(entry, 127, "user.%d", id);
	iniparser_unset(g_ini_d_, entry);

	rk_system_get_user_num(&user_num);
	user_num -= 1;
	rk_system_set_user_num(user_num);
	rk_param_save();

	return 0;
}

int rk_system_capability_get_video(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.video");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.video:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}

int rk_system_capability_get_image_adjustment(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.image_adjustment");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.image_adjustment:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}

int rk_system_capability_get_image_blc(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.image_blc");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.image_blc:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}

int rk_system_capability_get_image_enhancement(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.image_enhancement");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.image_enhancement:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}

int rk_system_capability_get_image_exposure(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.image_exposure");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.image_exposure:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}

int rk_system_capability_get_image_night_to_day(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.image_night_to_day");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.image_night_to_day:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}

int rk_system_capability_get_image_video_adjustment(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.image_video_adjustment");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.image_video_adjustment:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}

int rk_system_capability_get_image_white_blance(char *value) {
	char entry[128] = {'\0'};
	int section_keys = iniparser_getsecnkeys(g_ini_d_, "capability.image_white_blance");
	LOG_DEBUG("section_keys is %d\n", section_keys);
	if (!section_keys)
		return 0;

	int total_length = 0;
	const char *tmp;
	for (int i = 0; i < section_keys; i++) {
		snprintf(entry, 127, "capability.image_white_blance:%d", i);
		tmp = rk_param_get_string(entry, NULL);
		strcpy(value + total_length, tmp);
		total_length += strlen(tmp);
	}

	return 0;
}
