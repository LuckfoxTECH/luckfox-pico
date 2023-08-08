// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "common.h"
#include "iniparser.h"
#include "log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "param.c"

char g_ini_path_[256];
dictionary *g_ini_d_;
static pthread_mutex_t g_param_mutex = PTHREAD_MUTEX_INITIALIZER;

int rk_param_dump() {
	const char *section_name;
	const char *keys[32];
	int section_keys;
	int section_num = iniparser_getnsec(g_ini_d_);
	LOG_DEBUG("section_num is %d\n", section_num);

	for (int i = 0; i < section_num; i++) {
		section_name = iniparser_getsecname(g_ini_d_, i);
		LOG_DEBUG("section_name is %s\n", section_name);
		section_keys = iniparser_getsecnkeys(g_ini_d_, section_name);
		for (int j = 0; j < section_keys; j++) {
			iniparser_getseckeys(g_ini_d_, section_name, keys);
			LOG_DEBUG("%s = %s\n", keys[j], iniparser_getstring(g_ini_d_, keys[j], ""));
		}
	}

	return 0;
}

int rk_param_save() {
	FILE *fp = fopen(g_ini_path_, "w");
	if (fp == NULL) {
		LOG_ERROR("%s, fopen error!\n", g_ini_path_);
		iniparser_freedict(g_ini_d_);
		g_ini_d_ = NULL;
		return -1;
	}
	iniparser_dump_ini(g_ini_d_, fp);

	fflush(fp);
	fclose(fp);

	return 0;
}

int rk_param_get_int(const char *entry, int default_val) {
	int ret;
	pthread_mutex_lock(&g_param_mutex);
	ret = iniparser_getint(g_ini_d_, entry, default_val);
	pthread_mutex_unlock(&g_param_mutex);

	return ret;
}

int rk_param_set_int(const char *entry, int val) {
	char tmp[8];
	sprintf(tmp, "%d", val);
	pthread_mutex_lock(&g_param_mutex);
	iniparser_set(g_ini_d_, entry, tmp);
	pthread_mutex_unlock(&g_param_mutex);

	return 0;
}

const char *rk_param_get_string(const char *entry, const char *default_val) {
	const char *ret;
	pthread_mutex_lock(&g_param_mutex);
	ret = iniparser_getstring(g_ini_d_, entry, default_val);
	pthread_mutex_unlock(&g_param_mutex);

	return ret;
}

int rk_param_set_string(const char *entry, const char *val) {
	pthread_mutex_lock(&g_param_mutex);
	iniparser_set(g_ini_d_, entry, val);
	pthread_mutex_unlock(&g_param_mutex);

	return 0;
}

int rk_param_init(char *ini_path) {
	LOG_DEBUG("%s\n", __func__);
	char cmd[256];
	pthread_mutex_lock(&g_param_mutex);
	g_ini_d_ = NULL;
	if (ini_path)
		memcpy(g_ini_path_, ini_path, strlen(ini_path));
	else
		memcpy(g_ini_path_, "/userdata/rkipc.ini", strlen("/userdata/rkipc.ini"));
	LOG_INFO("g_ini_path_ is %s\n", g_ini_path_);

	g_ini_d_ = iniparser_load(g_ini_path_);
	if (g_ini_d_ == NULL) {
		LOG_ERROR("iniparser_load %s error! use /tmp/rkipc-factory-config.ini\n", g_ini_path_);
		snprintf(cmd, 127, "cp /tmp/rkipc-factory-config.ini %s", g_ini_path_);
		LOG_INFO("cmd is %s\n", cmd);
		system(cmd);
		g_ini_d_ = iniparser_load(g_ini_path_);
		if (g_ini_d_ == NULL) {
			LOG_ERROR("iniparser_load error again!\n");
			pthread_mutex_unlock(&g_param_mutex);
			return -1;
		}
	}
	rk_param_dump();
	pthread_mutex_unlock(&g_param_mutex);

	return 0;
}

int rk_param_deinit() {
	LOG_INFO("%s\n", __func__);
	if (g_ini_d_ == NULL)
		return 0;
	pthread_mutex_lock(&g_param_mutex);
	rk_param_save();
	if (g_ini_d_)
		iniparser_freedict(g_ini_d_);
	pthread_mutex_unlock(&g_param_mutex);

	return 0;
}

int rk_param_reload() {
	LOG_INFO("%s\n", __func__);
	pthread_mutex_lock(&g_param_mutex);
	if (g_ini_d_)
		iniparser_freedict(g_ini_d_);
	g_ini_d_ = iniparser_load(g_ini_path_);
	if (g_ini_d_ == NULL) {
		LOG_ERROR("iniparser_load error!\n");
		pthread_mutex_unlock(&g_param_mutex);
		return -1;
	}
	rk_param_dump();
	pthread_mutex_unlock(&g_param_mutex);

	return 0;
}
