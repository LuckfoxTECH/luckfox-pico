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
#include "rkadk_signal.h"

typedef struct {
  sem_t sem;
  int max_val;
} RKADK_SIGNAL_S;

void *RKADK_SIGNAL_Create(int defval, int maxval) {
  RKADK_SIGNAL_S *h = (RKADK_SIGNAL_S *)malloc(sizeof(RKADK_SIGNAL_S));

  if (h == NULL) {
    return NULL;
  }
  if (sem_init(&(h->sem), 0, defval) ==
      -1) { /* 初始化信号量失败,失败原因见error */
    perror("sem_init: ");
    free(h);
    h = NULL;
  } else
    h->max_val = maxval;
  return h;
}

void RKADK_SIGNAL_Destroy(void *sem) {
  if (sem == NULL) {
    return;
  }

  sem_destroy((sem_t *)sem);
  free(sem);
}

int RKADK_SIGNAL_Wait(void *sem, int timeout) {
  struct timespec tv;

  if (sem == NULL) {
    return 0;
  }

  if (timeout < 0) { /* 需要判断返回值,因为如果信号量被destroy了也会返回的 */
    return sem_wait((sem_t *)sem) == 0 ? 0 : -1;
  } else {
    clock_gettime(CLOCK_REALTIME, &tv);

    tv.tv_nsec += (timeout % 1000) * 1000000;
    if (tv.tv_nsec >= 1000000000) {
      tv.tv_sec += 1;
      tv.tv_nsec -= 1000000000;
    }
    tv.tv_sec += timeout / 1000;
    if (sem_timedwait((sem_t *)sem, (const struct timespec *)&tv)) {
      return -1;
    }
    return 0;
  }
}

void RKADK_SIGNAL_Give(void *sem) {
  int val;

  if (sem == NULL) {
    return;
  }

  sem_getvalue((sem_t *)sem, &val);

  if (val < ((RKADK_SIGNAL_S *)sem)->max_val) {
    sem_post((sem_t *)sem);
  }
}

void RKADK_SIGNAL_Reset(void *sem) { RKADK_SIGNAL_Give(sem); }