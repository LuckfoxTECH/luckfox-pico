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

#ifndef __RKADK_SIGNAL_H__
#define __RKADK_SIGNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <getopt.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief 创建信号量
 *
 * @param defval 取值为0
 * @param maxval 取值为1
 *
 * @return 成功,返回信号量句柄; 否则,返回空值
 */
void *RKADK_SIGNAL_Create(int defval, int maxval);

/**
 * @brief 销毁信号量
 *
 * @param signal 信号量句柄
 */
void RKADK_SIGNAL_Destroy(void *sem);

/**
 * @brief 等待信号量
 *
 * @param signal 信号量句柄
 * @param timeout -1表示无限等待;其他值表示等待的时间(ms)
 *
 * @return 成功,返回0; 否则,返回-1
 */
int RKADK_SIGNAL_Wait(void *sem, int timeout);

/**
 * @brief 释放信号量
 *
 * @param signal 信号量句柄
 */
void RKADK_SIGNAL_Give(void *sem);

/**
 * @brief 重置信号量
 *
 * @param signal 信号量句柄
 */
void RKADK_SIGNAL_Reset(void *sem);

#ifdef __cplusplus
}
#endif
#endif
