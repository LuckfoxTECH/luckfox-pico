// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __BT_LOG__
#define __BT_LOG__
#include <syslog.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TAG "BLUEZ_LOG"
#define SYSLOG_DEBUG

#ifdef SYSLOG_DEBUG
#define pr_debug(fmt, ...) syslog(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define pr_info(fmt, ...) syslog(LOG_INFO, fmt, ##__VA_ARGS__)
#define pr_warning(fmt, ...) syslog(LOG_WARNING, fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...) syslog(LOG_ERR, fmt, ##__VA_ARGS__)
#else
#define pr_debug printf
#define pr_info printf
#define pr_warning printf
#define pr_err printf
#endif

#ifdef __cplusplus
}
#endif

#endif
