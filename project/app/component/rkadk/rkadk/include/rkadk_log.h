/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __RKADK_LOG_H__
#define __RKADK_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define RKADK_LOG_LEVEL_ERROR 0
#define RKADK_LOG_LEVEL_WARNING 1
#define RKADK_LOG_LEVEL_INFO 2
#define RKADK_LOG_LEVEL_DEBUG 3

#define RKADK_LOG_LEVEL RKADK_LOG_LEVEL_DEBUG

#define RKADK_LOG_LEVEL_JUDGE(LEVEL)                                           \
  {                                                                            \
    if (RKADK_LOG_LEVEL < LEVEL)                                               \
      break;                                                                   \
  }

#define RKADK_LOGE(format, ...)                                                \
  do {                                                                         \
    RKADK_LOG_LEVEL_JUDGE(RKADK_LOG_LEVEL_ERROR);                              \
    printf("[RKADK_E] %s[%d]: " format "\n", __func__, __LINE__, ##__VA_ARGS__);\
  } while (0)

#define RKADK_LOGW(format, ...)                                                \
  do {                                                                         \
    RKADK_LOG_LEVEL_JUDGE(RKADK_LOG_LEVEL_WARNING);                            \
    printf("[RKADK_W] %s[%d]: " format "\n", __func__, __LINE__, ##__VA_ARGS__);\
  } while (0)

#define RKADK_LOGI(format, ...)                                                \
  do {                                                                         \
    RKADK_LOG_LEVEL_JUDGE(RKADK_LOG_LEVEL_INFO);                               \
    printf("[RKADK_I] %s[%d]: " format "\n", __func__, __LINE__, ##__VA_ARGS__);\
  } while (0)

#define RKADK_LOGD(format, ...)                                                \
  do {                                                                         \
    RKADK_LOG_LEVEL_JUDGE(RKADK_LOG_LEVEL_DEBUG);                              \
    printf("[RKADK_D] %s[%d]: " format "\n", __func__, __LINE__, ##__VA_ARGS__);\
  } while (0)

#define RKADK_KLOG(format, ...)                                                \
  do {                                                                         \
    FILE *fp = fopen("/dev/kmsg", "w");                                        \
    if (NULL != fp) {                                                          \
      fprintf(fp, "[RKADK]: " format"\n", ##__VA_ARGS__);                      \
      fclose(fp);                                                              \
    }                                                                          \
  } while (0)

#ifdef __cplusplus
}
#endif
#endif
