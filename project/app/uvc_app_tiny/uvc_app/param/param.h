// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "iniparser.h"

extern dictionary *g_ini_d_;

int rk_param_get_int(const char *entry, int default_val);
int rk_param_set_int(const char *entry, int val);
const char *rk_param_get_string(const char *entry, const char *default_val);
int rk_param_set_string(const char *entry, const char *val);
int rk_param_save();
int rk_param_init(char *ini_path);
int rk_param_deinit();
int rk_param_reload();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
