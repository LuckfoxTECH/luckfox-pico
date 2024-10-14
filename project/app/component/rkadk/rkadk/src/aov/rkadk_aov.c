/*
 * Copyright (c) 2023 Rockchip, Inc. All Rights Reserved.
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

#include "rkadk_common.h"
#include "rkadk_log.h"
#include "rkadk_aov.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SOC_SLEEP_STR "mem"
#define SOC_SLEEP_PATH "/sys/power/state"
static pthread_mutex_t gWakeupRunMutex = PTHREAD_MUTEX_INITIALIZER;
static RKADK_AOV_NOTIFY_CALLBACK gpfnNotifyCallback = NULL;

void RKADK_AOV_WakeupLock() {
  pthread_mutex_lock(&gWakeupRunMutex);
}

void RKADK_AOV_WakeupUnlock() {
  pthread_mutex_unlock(&gWakeupRunMutex);
}

int RKADK_AOV_Init(RKADK_AOV_ARG_S *pstAovAttr) {
  pthread_mutex_init(&gWakeupRunMutex, NULL);
  gpfnNotifyCallback = pstAovAttr->pfnNotifyCallback;
  return 0;
}

int RKADK_AOV_DeInit() {
  pthread_mutex_destroy(&gWakeupRunMutex);
  return 0;
}

int RKADK_AOV_EnterSleep() {
  int fd = -1;
  ssize_t ret = -1;

  //pthread_mutex_lock(&gWakeupRunMutex);
  fd = open(SOC_SLEEP_PATH, O_WRONLY | O_TRUNC);
  if (fd == -1) {
    RKADK_LOGE("Failed to open %s, errno=%d, %s", SOC_SLEEP_PATH, errno, strerror(errno));
    goto EXIT;
  }

  ret = write(fd, SOC_SLEEP_STR, strlen(SOC_SLEEP_STR));
  if (ret == -1) {
    RKADK_LOGE("Failed to write %s, errno=%d, %s", SOC_SLEEP_STR, errno, strerror(errno));
    goto EXIT;
  }

  RKADK_LOGD("echo \"%s\" > %s successfully", SOC_SLEEP_STR, SOC_SLEEP_PATH);
  ret = 0;

EXIT:
  if (fd >= 0)
    close(fd);

  //pthread_mutex_unlock(&gWakeupRunMutex);
  return ret;
}

void RKADK_AOV_Notify(RKADK_AOV_EVENT_E enEvent, void *msg) {
  if (gpfnNotifyCallback)
    gpfnNotifyCallback(enEvent, msg);
  else
    RKADK_LOGW("Unregistered notify callback");
}

int RKADK_AOV_SetSuspendTime(int u32WakeupSuspendTime) {
  char wakeupCmd[256];

  memset(wakeupCmd, 0, 256);
  sprintf(wakeupCmd, "io -4 0xff300048 %d", u32WakeupSuspendTime * 32);
  system(wakeupCmd);
  RKADK_LOGD("wakeup suspend time = %d", u32WakeupSuspendTime);
  return 0;
}

#if 0
void RKADK_AOV_DumpPtsToTMP(uint32_t seq, uint64_t pts, int max_dump_pts_count) {
  static int line_count = 0;
  static FILE *file;
  const char *file_path = "/tmp/pts.txt";

  if (line_count >= max_dump_pts_count) {
    return;
  }

  if (line_count == 0) {
    file = fopen(file_path, "w");
    if (file == NULL) {
      perror("Error opening file");
      return;
    }
  }

  if (file != NULL)
    fprintf(file, "seq: %u, pts: %llums\n", seq, (unsigned long long)pts / 1000);

  line_count++;

  if (line_count >= max_dump_pts_count) {
    printf("Closed file after writing %d lines.\n", max_dump_pts_count);
    fclose(file);
    file = NULL;
  }
}
#endif

int RKADK_AOV_DisableNonBootCPUs() { return RKADK_SUCCESS; }
int RKADK_AOV_EnableNonBootCPUs() { return RKADK_SUCCESS; }
