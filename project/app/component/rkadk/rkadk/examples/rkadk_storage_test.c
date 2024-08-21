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

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "rkadk_storage.h"

#define SIZE_1MB (1024 * 1024)
#define MAX_CH 2
#define FILE_SIZE (64 * SIZE_1MB)

static bool quit = false;
static void SigtermHandler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  quit = true;
}

static RKADK_VOID MountStatusCallback(RKADK_MW_PTR pHandle,
              RKADK_MOUNT_STATUS status) {
  switch(status) {
  case DISK_UNMOUNTED:
    RKADK_LOGD("+++++ DISK_UNMOUNTED +++++");
    break;
  case DISK_NOT_FORMATTED:
    RKADK_LOGD("+++++ DISK_NOT_FORMATTED +++++");
    break;
  case DISK_FORMAT_ERR:
    RKADK_LOGD("+++++ DISK_FORMAT_ERR +++++");
    break;
  case DISK_SCANNING:
    RKADK_LOGD("+++++ DISK_SCANNING +++++");
    break;
  case DISK_MOUNTED:
    RKADK_LOGD("+++++ DISK_MOUNTED +++++");
    break;
  default:
    RKADK_LOGE("Unsupport status: %d", status);
    break;
  }
}

RKADK_S32 CreatFile(char *name, long size) {
  int fd;
  int ret;
  int wlLen;
  struct timeval tvAllBegin;
  struct timeval tvAllEnd;
  double timeCons;
  int bufLen = 4 * 1024;
  unsigned char buf[bufLen];

  RKADK_CHECK_POINTER(name, RKADK_FAILURE);
  RKADK_LOGI("Create file:%s size:%ld", name, size);
  gettimeofday(&tvAllBegin, NULL);

  fd = open(name, O_CREAT | O_RDWR);
  if (fd < 0) {
    RKADK_LOGE("Open file failed.");
    return -1;
  }

  gettimeofday(&tvAllEnd, NULL);
  timeCons = 1000 * (tvAllEnd.tv_sec - tvAllBegin.tv_sec) +
             ((tvAllEnd.tv_usec - tvAllBegin.tv_usec) / 1000.0);
  RKADK_LOGD("Open name:%s, timeCons = %fms", name, timeCons);

  if (fd) {
    while (size > 0 && !quit) {
      wlLen = (size > bufLen) ? bufLen : size;
      ret = write(fd, buf, wlLen);
      if (ret < 0) {
        RKADK_LOGE("Write failed.\n");
        close(fd);
        return -1;
      }
      size -= wlLen;
    }
    close(fd);
  }

  return 0;
}

static void *CreatFileThread(void *arg) {
  int ch = *(int *)arg;
  char name[400];
  time_t timep;
  struct tm *p;

  while (!quit) {
    usleep(100);
    time(&timep);
    p = gmtime(&timep);
    if (ch == 0)
      sprintf(name, "/mnt/sdcard/video_front/%d-%d-%d_%d-%d-%d.mp4",
              (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour,
              p->tm_min, p->tm_sec);
    else
      sprintf(name, "/mnt/sdcard/video_back/%d-%d-%d_%d-%d-%d.mp4",
              (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour,
              p->tm_min, p->tm_sec);

    if (CreatFile(name, FILE_SIZE)) {
      RKADK_LOGE("Create file failed.");
      quit = true;
    }
  }

  return NULL;
}

RKADK_S32 CreatFileTest(RKADK_MW_PTR *ppHandle) {
  int ret;
  int i;
  int totalSize;
  int freeSize;
  pthread_t tid[MAX_CH];

  for (i = 0; i < MAX_CH; i++) {
    ret = pthread_create(&tid[i], NULL, CreatFileThread, (void *)(&i));

    if (ret) {
      RKADK_LOGE("pthread_create failed.");
      return -1;
    }
    usleep(500);
  }

  while (!quit) {
    usleep(5000);
    RKADK_LOGD("sync start");
    sync();
    RKADK_LOGD("sync end");
    RKADK_STORAGE_GetCapacity(ppHandle, &totalSize, &freeSize);
    RKADK_LOGI("sdcard totalSize: %d, freeSize: %d", totalSize, freeSize);

    if (RKADK_STORAGE_GetMountStatus(*ppHandle) == DISK_UNMOUNTED)
      quit = true;
  }

  for (i = 0; i < MAX_CH; i++) {
    ret = pthread_join(tid[i], NULL);

    if (ret) {
      RKADK_LOGE("pthread_join failed.");
      return -1;
    }
  }
  sync();

  return 0;
}

RKADK_S32 SetDevAttr(RKADK_STR_DEV_ATTR *pstDevAttr) {
  RKADK_CHECK_POINTER(pstDevAttr, RKADK_FAILURE);
  RKADK_LOGD("The DevAttr will be user-defined.");

  memset(pstDevAttr, 0, sizeof(RKADK_STR_DEV_ATTR));
  sprintf(pstDevAttr->cMountPath, "/mnt/sdcard");
  pstDevAttr->s32AutoDel = 1;
  pstDevAttr->s32FreeSizeDelMin = 200;
  pstDevAttr->s32FreeSizeDelMax = 1000;
  pstDevAttr->s32FolderNum = 4;
  pstDevAttr->pstFolderAttr = (RKADK_STR_FOLDER_ATTR *)malloc(
      sizeof(RKADK_STR_FOLDER_ATTR) * pstDevAttr->s32FolderNum);

  if (!pstDevAttr->pstFolderAttr) {
    RKADK_LOGE("pstDevAttr->pstFolderAttr malloc failed.");
    return -1;
  }
  memset(pstDevAttr->pstFolderAttr, 0,
         sizeof(RKADK_STR_FOLDER_ATTR) * pstDevAttr->s32FolderNum);

  pstDevAttr->pstFolderAttr[0].s32SortCond = SORT_FILE_NAME;
  pstDevAttr->pstFolderAttr[0].bNumLimit = RKADK_FALSE;
  pstDevAttr->pstFolderAttr[0].s32Limit = 35;
  sprintf(pstDevAttr->pstFolderAttr[0].cFolderPath, "/video_front/");
  pstDevAttr->pstFolderAttr[1].s32SortCond = SORT_FILE_NAME;
  pstDevAttr->pstFolderAttr[1].bNumLimit = RKADK_FALSE;
  pstDevAttr->pstFolderAttr[1].s32Limit = 35;
  sprintf(pstDevAttr->pstFolderAttr[1].cFolderPath, "/video_back/");
  pstDevAttr->pstFolderAttr[2].s32SortCond = SORT_FILE_NAME;
  pstDevAttr->pstFolderAttr[2].bNumLimit = RKADK_TRUE;
  pstDevAttr->pstFolderAttr[2].s32Limit = 10;
  sprintf(pstDevAttr->pstFolderAttr[2].cFolderPath, "/photo/");
  pstDevAttr->pstFolderAttr[3].s32SortCond = SORT_FILE_NAME;
  pstDevAttr->pstFolderAttr[3].bNumLimit = RKADK_FALSE;
  pstDevAttr->pstFolderAttr[3].s32Limit = 15;
  sprintf(pstDevAttr->pstFolderAttr[3].cFolderPath, "/video_urgent/");
  pstDevAttr->pfnStatusCallback = MountStatusCallback;

  return 0;
}

RKADK_S32 FreeDevAttr(RKADK_STR_DEV_ATTR devAttr) {
  if (devAttr.pstFolderAttr) {
    free(devAttr.pstFolderAttr);
    devAttr.pstFolderAttr = NULL;
  }

  return 0;
}

static void sigterm_handler(int sig) {
  fprintf(stderr, "signal %d\n", sig);
  quit = true;
}

bool FileFilterCb(const char *fileName) {
  if (!strstr(fileName, "sub"))
    return true;

  return false;
}

int main(int argc, char *argv[]) {
  RKADK_S32 i;
  RKADK_MW_PTR pHandle = NULL;
  RKADK_STR_DEV_ATTR stDevAttr;
  RKADK_FILE_LIST list;

  memset(&list, 0, sizeof(RKADK_FILE_LIST));
  sprintf(list.path, "/mnt/sdcard/video_front/");

  if (argc > 0)
    RKADK_LOGI("%s run", argv[0]);

  if (SetDevAttr(&stDevAttr)) {
    RKADK_LOGE("Set devAttr failed.");
    return -1;
  }

  if (RKADK_STORAGE_Init(&pHandle, &stDevAttr)) {
    RKADK_LOGE("Storage init failed.");
    return -1;
  }

  RKADK_STORAGE_RegisterFileFilterCB(pHandle, FileFilterCb);

  RKADK_LOGI("Dev path: %s", RKADK_STORAGE_GetDevPath(pHandle));
  signal(SIGINT, SigtermHandler);
  sleep(10);
  if (CreatFileTest(&pHandle))
    RKADK_LOGW("CreatFileTest failed.");

  signal(SIGINT, sigterm_handler);

  while (!quit) {
    usleep(5000);
  }

  if (!RKADK_STORAGE_GetFileList(&list, pHandle, LIST_DESCENDING)) {
    RKADK_LOGD("LIST_DESCENDING: %d", list.s32FileNum);
    for (i = 0; i < list.s32FileNum; i++)
      RKADK_LOGI("%s  %lld", list.file[i].filename, list.file[i].stSize);
  }
  RKADK_STORAGE_FreeFileList(&list);

  if (!RKADK_STORAGE_GetFileList(&list, pHandle, LIST_ASCENDING)) {
    RKADK_LOGD("LIST_ASCENDING: %d", list.s32FileNum);
    for (i = 0; i < list.s32FileNum; i++)
      RKADK_LOGI("%s  %lld", list.file[i].filename, list.file[i].stSize);
  }
  RKADK_STORAGE_FreeFileList(&list);

  FreeDevAttr(stDevAttr);
  RKADK_STORAGE_UnRegisterFileFilterCB(pHandle);
  RKADK_STORAGE_Deinit(pHandle);
  RKADK_LOGD("%s out", argv[0]);

  return 0;
}
