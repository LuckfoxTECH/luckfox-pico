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

#include "rkadk_thread.h"
#include "rkadk_log.h"
#include <string.h>

typedef struct {
  RKADK_THREAD_PROC_FN func;
  void *param;
  pthread_t tid;
  int exit_flag;
} RKADK_THREAD_HANDLE_S;

static void *RKADK_THREAD_Proc(void *params) {
  char name[40];
  RKADK_THREAD_HANDLE_S *pstThread = (RKADK_THREAD_HANDLE_S *)params;

  while (!pstThread->exit_flag) {
    pstThread->func(pstThread->param);
  }

  memset(name, 0, sizeof(name));
  pthread_getname_np(pthread_self(), name, sizeof(name));

  RKADK_LOGD("Exit %s thread", name);
  pthread_exit(&(pstThread->tid));
  return NULL;
}

void *RKADK_THREAD_Create(RKADK_THREAD_PROC_FN func, void *param, char *name) {
  int ret;
  RKADK_THREAD_HANDLE_S *pstThread = NULL;

  pstThread = (RKADK_THREAD_HANDLE_S *)malloc(sizeof(RKADK_THREAD_HANDLE_S));
  if (!pstThread) {
    RKADK_LOGE("malloc pstThread failed");
    return NULL;
  }
  memset(pstThread, 0, sizeof(RKADK_THREAD_HANDLE_S));

  pstThread->func = func;
  pstThread->param = param;

  ret = pthread_create(&pstThread->tid, NULL, RKADK_THREAD_Proc, pstThread);
  if (ret) {
    RKADK_LOGE("Create thread failed %d", ret);
    free(pstThread);
    return NULL;
  }

  if (name) {
    pthread_setname_np(pstThread->tid, name);
    RKADK_LOGI("Create %s thread success!", name);
  } else {
    RKADK_LOGI("Create thread success!");
  }
  return (void *)pstThread;
}

int RKADK_THREAD_Destory(void *handle) {
  int ret = 0;
  RKADK_THREAD_HANDLE_S *pstThread;

  if (!handle) {
    RKADK_LOGD("handle has been destory");
    return 0;
  }

  pstThread = (RKADK_THREAD_HANDLE_S *)handle;

  do {
    pstThread->exit_flag = 1;
    ret = pthread_join(pstThread->tid, NULL);
    if (ret)
      RKADK_LOGE("Exit thread failed!");
  } while (0);

  free(pstThread);
  RKADK_LOGI("Exit thread success!");
  return ret;
}

int RKADK_THREAD_SetExit(void *handle) {
  int ret = 0;
  RKADK_THREAD_HANDLE_S *pstThread;

  if (!handle) {
    RKADK_LOGD("handle has been destory");
    return 0;
  }
  pstThread = (RKADK_THREAD_HANDLE_S *)handle;
  pstThread->exit_flag = 1;
  return ret;
}
