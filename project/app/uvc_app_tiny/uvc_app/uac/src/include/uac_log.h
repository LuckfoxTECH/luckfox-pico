// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_INCLUDE_UAC_LOGGER_H_
#define SRC_INCLUDE_UAC_LOGGER_H_
//#ifdef __cplusplus

#ifdef uac_minilogGER
#include "minilogger/log.h"
#else
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#define uac_minilog_warn(...)
#define uac_minilog_error(...)
#define uac_minilog_info(...)
#define uac_minilog_debug(...)
#define __uac_minilog_log_init(...)
#endif

extern int uac_app_log_level;

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#ifndef LOG_TAG
#define LOG_TAG "uac_app"
#endif

#define ALOGI(format, ...)                                                     \
  do {                                                                         \
    if (uac_app_log_level < LOG_LEVEL_INFO)                                    \
      break;                                                                   \
    \ fprintf(stderr, "[%s][%s]:" format, LOG_TAG, __FUNCTION__,               \
              ##__VA_ARGS__);                                                  \
  } while (0)

#define ALOGW(format, ...)                                                     \
  do {                                                                         \
    if (uac_app_log_level < LOG_LEVEL_WARN)                                    \
      break;                                                                   \
    \ fprintf(stderr, "[%s][%s]:" format, LOG_TAG, __FUNCTION__,               \
              ##__VA_ARGS__);                                                  \
  } while (0)

#define ALOGE(format, ...)                                                     \
  do {                                                                         \
    if (uac_app_log_level < LOG_LEVEL_ERROR)                                   \
      break;                                                                   \
    fprintf(stderr, "[%s][%s]:" format, LOG_TAG, __FUNCTION__, ##__VA_ARGS__); \
  } while (0)

#define ALOGD(format, ...)                                                     \
  do {                                                                         \
    if (uac_app_log_level < LOG_LEVEL_DEBUG)                                   \
      break;                                                                   \
    fprintf(stderr, "[%s][%s]:" format, LOG_TAG, __FUNCTION__, ##__VA_ARGS__); \
  } while (0)
//#endif
#endif //  SRC_INCLUDE_UAC_LOGGER_H_
