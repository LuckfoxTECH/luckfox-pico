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

#ifndef __RKADK_THREAD_H__
#define __RKADK_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

typedef bool (*RKADK_THREAD_PROC_FN)(void *param);

void *RKADK_THREAD_Create(RKADK_THREAD_PROC_FN func, void *param, char *name);

int RKADK_THREAD_Destory(void *handle);

int RKADK_THREAD_SetExit(void *handle);

#ifdef __cplusplus
}
#endif
#endif
