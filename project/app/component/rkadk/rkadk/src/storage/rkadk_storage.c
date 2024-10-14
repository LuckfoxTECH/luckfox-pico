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

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <time.h>
#include <unistd.h>
#include <sys/mount.h>
#include <rkfsmk.h>

#include "rkadk_storage.h"
#include "cjson/cJSON.h"

#define MAX_TYPE_NMSG_LEN 32
#define MAX_ATTR_LEN 256
#define MAX_STRLINE_LEN 1024
#define REPAIR_FILE_NUM 8

#define JSON_KEY_FOLDER_NAME "FolderName"
#define JSON_KEY_FILE_NUMBER "FileNumber"
#define JSON_KEY_TOTAL_SIZE "TotalSize"
#define JSON_KEY_TOTAL_SPACE "TotalSpace"
#define JSON_KEY_FILE_ARRAY "FileArray"
#define JSON_KEY_FILE_NAME "FileName"
#define JSON_KEY_MODIFY_TIME "ModifyTime"
#define JSON_KEY_FILE_SIZE "FileSize"
#define JSON_KEY_FILE_SPACE "FileSpace"

typedef RKADK_S32 (*RKADK_REC_MSG_CB)(RKADK_MW_PTR, RKADK_S32, RKADK_MW_PTR,
                                      RKADK_S32, RKADK_MW_PTR);

struct RKADK_STR_FILE {
  struct RKADK_STR_FILE *prev;
  struct RKADK_STR_FILE *next;
  RKADK_CHAR filename[RKADK_MAX_FILE_PATH_LEN];
  time_t stTime;
  off_t stSize;
  off_t stSpace;
};

typedef struct {
  RKADK_CHAR cpath[RKADK_MAX_FILE_PATH_LEN];
  RKADK_SORT_CONDITION s32SortCond;
  RKADK_S32 wd;
  RKADK_S32 s32FileNum;
  off_t totalSize;
  off_t totalSpace;
  pthread_mutex_t mutex;
  struct RKADK_STR_FILE *pstFileListFirst;
  struct RKADK_STR_FILE *pstFileListLast;
} RKADK_STR_FOLDER;

typedef struct {
  RKADK_CHAR cDevPath[RKADK_MAX_FILE_PATH_LEN];
  RKADK_CHAR cDevType[MAX_TYPE_NMSG_LEN];
  RKADK_CHAR cDevAttr1[MAX_ATTR_LEN];
  RKADK_MOUNT_STATUS s32MountStatus;
  pthread_t fileScanTid;
  RKADK_S32 s32FolderNum;
  RKADK_S32 s32TotalSize;
  RKADK_S32 s32FreeSize;
  RKADK_S32 s32FsckQuit;
  RKADK_STR_FOLDER *pstFolder;
} RKADK_STR_DEV_STA;

struct RKADK_TMSG_ELEMENT {
  struct RKADK_TMSG_ELEMENT *next;
  RKADK_S32 msg;
  RKADK_CHAR *data;
  RKADK_S32 s32DataLen;
};

typedef struct {
  struct RKADK_TMSG_ELEMENT *first;
  struct RKADK_TMSG_ELEMENT *last;
  RKADK_S32 num;
  RKADK_S32 quit;
  pthread_mutex_t mutex;
  pthread_cond_t notEmpty;
  RKADK_REC_MSG_CB recMsgCb;
  pthread_t recTid;
  RKADK_MW_PTR pHandlePath;
} RKADK_TMSG_BUFFER;

typedef enum {
  MSG_DEV_ADD = 1,
  MSG_DEV_REMOVE = 2,
  MSG_DEV_CHANGED = 3,
} RKADK_ENUM_MSG;

typedef struct {
  RKADK_TMSG_BUFFER stMsgHd;
  pthread_t eventListenerTid;
  RKADK_S32 eventListenerRun;
  RKADK_STR_DEV_STA stDevSta;
  RKADK_STR_DEV_ATTR stDevAttr;
  RKADK_MOUNT_STATUS_CALLBACK_FN pfnStatusCallback;
  RKADK_FILE_FILTER_CALLBACK_FN pfnFileFilterCB;
} RKADK_STORAGE_HANDLE;

static RKADK_S32 RKADK_STORAGE_RKFSCK(RKADK_STORAGE_HANDLE *pHandle, RKADK_STR_DEV_ATTR *pdevAttr);

void RKADK_STORAGE_ProcessStatus(RKADK_STORAGE_HANDLE *pHandle,
                              RKADK_MOUNT_STATUS status) {

  if (!pHandle->pfnStatusCallback) {
    RKADK_LOGD("Unregistered mount status callback");
    return;
  }

  pHandle->pfnStatusCallback(pHandle, status);
}

static RKADK_STR_DEV_ATTR
RKADK_STORAGE_GetParam(RKADK_STORAGE_HANDLE *pHandle) {
  return pHandle->stDevAttr;
}

RKADK_STR_DEV_ATTR RKADK_STORAGE_GetDevAttr(RKADK_MW_PTR pHandle) {
  return RKADK_STORAGE_GetParam((RKADK_STORAGE_HANDLE *)pHandle);
}

static RKADK_S32 RKADK_STORAGE_CreateFolder(RKADK_CHAR *folder) {
  RKADK_S32 i, len;

  RKADK_CHECK_POINTER(folder, RKADK_FAILURE);

  len = strlen(folder);
  if (!len) {
    RKADK_LOGE("Invalid path.");
    return -1;
  }

  for (i = 1; i < len; i++) {
    if (folder[i] != '/')
      continue;

    folder[i] = 0;
    if (access(folder, R_OK)) {
      if (mkdir(folder, 0755)) {
        RKADK_LOGE("mkdir error");
        return -1;
      }
    }
    folder[i] = '/';
  }

  if (access(folder, R_OK)) {
    if (mkdir(folder, 0755)) {
      RKADK_LOGE("mkdir error");
      return -1;
    }
  }

  RKADK_LOGD("Create %s finished", folder);
  return 0;
}

static RKADK_S32 RKADK_STORAGE_ReadTimeout(RKADK_S32 fd, RKADK_U32 u32WaitMs) {
  RKADK_S32 ret = 0;

  if (u32WaitMs > 0) {
    fd_set readFdset;
    struct timeval timeout;

    FD_ZERO(&readFdset);
    FD_SET(fd, &readFdset);

    timeout.tv_sec = u32WaitMs / 1000;
    timeout.tv_usec = (u32WaitMs % 1000) * 1000;

    do {
      ret = select(fd + 1, &readFdset, NULL, NULL, &timeout);
    } while (ret < 0 && errno == EINTR);

    if (ret == 0) {
      ret = -1;
      errno = ETIMEDOUT;
    } else if (ret == 1) {
      return 0;
    }
  }

  return ret;
}

static RKADK_S32 RKADK_STORAGE_GetDiskSize(RKADK_CHAR *path,
                                           RKADK_S32 *totalSize,
                                           RKADK_S32 *freeSize) {
  struct statfs diskInfo;

  RKADK_CHECK_POINTER(path, RKADK_FAILURE);
  RKADK_CHECK_POINTER(totalSize, RKADK_FAILURE);
  RKADK_CHECK_POINTER(freeSize, RKADK_FAILURE);

  if (statfs(path, &diskInfo)) {
    RKADK_LOGE("statfs[%s] failed", path);
    return -1;
  }

  *totalSize = (diskInfo.f_bsize * diskInfo.f_blocks) >> 10;
  *freeSize = (diskInfo.f_bfree * diskInfo.f_bsize) >> 10;
  return 0;
}

static RKADK_S32 RKADK_STORAGE_GetMountDev(RKADK_CHAR *path, RKADK_CHAR *dev,
                                           RKADK_CHAR *type,
                                           RKADK_CHAR *attributes) {
  FILE *fp;
  RKADK_CHAR strLine[MAX_STRLINE_LEN];
  RKADK_CHAR *tmp;

  RKADK_CHECK_POINTER(dev, RKADK_FAILURE);
  RKADK_CHECK_POINTER(path, RKADK_FAILURE);
  RKADK_CHECK_POINTER(type, RKADK_FAILURE);
  RKADK_CHECK_POINTER(attributes, RKADK_FAILURE);

  if ((fp = fopen("/proc/mounts", "r")) == NULL) {
    RKADK_LOGE("Open file error!");
    return -1;
  }

  while (!feof(fp)) {
    fgets(strLine, MAX_STRLINE_LEN, fp);
    tmp = strstr(strLine, path);

    if (tmp) {
      RKADK_CHAR MountPath[RKADK_MAX_FILE_PATH_LEN];
      sscanf(strLine, "%s %s %s %s", dev, MountPath, type, attributes);

      fclose(fp);
      return 0;
    }
  }

  fclose(fp);
  return -1;
}

static RKADK_S32 RKADK_STORAGE_GetMountPath(RKADK_CHAR *dev, RKADK_CHAR *path,
                                            RKADK_S32 s32PathLen) {
  RKADK_S32 ret = -1;
  FILE *fp;
  RKADK_CHAR strLine[MAX_STRLINE_LEN];
  RKADK_CHAR *tmp;

  RKADK_CHECK_POINTER(dev, RKADK_FAILURE);
  RKADK_CHECK_POINTER(path, RKADK_FAILURE);

  if ((fp = fopen("/proc/mounts", "r")) == NULL) {
    RKADK_LOGE("Open file error!");
    return -1;
  }

  memset(path, 0, s32PathLen);
  while (!feof(fp)) {
    fgets(strLine, MAX_STRLINE_LEN, fp);
    tmp = strstr(strLine, dev);

    if (tmp) {
      RKADK_S32 len;
      RKADK_CHAR *s = strstr(strLine, " ") + 1;
      RKADK_CHAR *e = strstr(s, " ");
      len = e - s;

      if ((len > 0) && (len < s32PathLen)) {
        memcpy(path, s, len);
        ret = 0;
      } else {
        RKADK_LOGE("len[%d], s32PathLen[%d]", len, s32PathLen);
        ret = -2;
      }

      goto exit;
    }
  }

exit:
  fclose(fp);
  return ret;
}

static bool RKADK_STORAGE_FileCompare(struct RKADK_STR_FILE *existingFile,
                                      struct RKADK_STR_FILE *newFile,
                                      RKADK_SORT_CONDITION cond) {
  bool ret = false;

  switch (cond) {
  case SORT_MODIFY_TIME: {
    ret = (newFile->stTime <= existingFile->stTime);
    break;
  }
  case SORT_FILE_NAME: {
    ret = (strcmp(newFile->filename, existingFile->filename) <= 0);
    break;
  }
  case SORT_BUTT: {
    ret = false;
    RKADK_LOGE("Invalid condition.");
    break;
  }
  }

  return ret;
}

static RKADK_S32 RKADK_STORAGE_FileListCheck(RKADK_STR_FOLDER *folder,
                                           RKADK_CHAR *filename,
                                           struct stat *statbuf) {
  RKADK_S32 ret = 0;
  struct RKADK_STR_FILE *tmp = NULL;

  RKADK_CHECK_POINTER(folder, RKADK_FAILURE);
  RKADK_CHECK_POINTER(filename, RKADK_FAILURE);

  pthread_mutex_lock(&folder->mutex);

  if (folder->pstFileListFirst) {
    tmp = folder->pstFileListFirst;

    if (!strcmp(tmp->filename, filename)) {
      ret = 1;
    } else {
      while (tmp->next) {
        if (!strcmp(tmp->next->filename, filename)) {
          ret = 1;
          break;
        }
        tmp = tmp->next;
      }
    }
  }

  pthread_mutex_unlock(&folder->mutex);

  return ret;
}

static RKADK_S32 RKADK_STORAGE_FileListAdd(RKADK_STR_FOLDER *folder,
                                           RKADK_CHAR *filename,
                                           struct stat *statbuf) {
  struct RKADK_STR_FILE *tmp = NULL;
  struct RKADK_STR_FILE *tmp_1 = NULL;

  RKADK_CHECK_POINTER(folder, RKADK_FAILURE);
  RKADK_CHECK_POINTER(filename, RKADK_FAILURE);

  pthread_mutex_lock(&folder->mutex);

  tmp_1 = (struct RKADK_STR_FILE *)malloc(sizeof(struct RKADK_STR_FILE));

  if (!tmp_1) {
    RKADK_LOGE("tmp malloc failed.");
    pthread_mutex_unlock(&folder->mutex);
    return -1;
  }

  sprintf(tmp_1->filename, "%s", filename);
  tmp_1->stSize = statbuf->st_size;
  tmp_1->stSpace = statbuf->st_blocks << 9;
  tmp_1->stTime = statbuf->st_mtime;
  tmp_1->prev = NULL;
  tmp_1->next = NULL;

  if (folder->pstFileListFirst) {
    tmp = folder->pstFileListFirst;
    if (tmp_1->stTime >= tmp->stTime) {
      tmp_1->next = tmp;
      tmp->prev = tmp_1;
      folder->pstFileListFirst = tmp_1;
    } else {
      while (tmp->next) {
        if (tmp_1->stTime >= tmp->next->stTime) {
          tmp_1->next = tmp->next;
          tmp_1->prev = tmp;
          if (tmp_1->next)
            tmp_1->next->prev = tmp_1;
          tmp->next = tmp_1;
          break;
        }
        tmp = tmp->next;
      }
      if (tmp->next == NULL) {
        tmp->next = tmp_1;
        tmp_1->prev = tmp;
        folder->pstFileListLast = tmp_1;
      }
    }
  } else {
    folder->pstFileListFirst = tmp_1;
    folder->pstFileListLast = tmp_1;
  }

  folder->totalSize += tmp_1->stSize;
  folder->totalSpace += tmp_1->stSpace;
  folder->s32FileNum++;

  pthread_mutex_unlock(&folder->mutex);
  return 0;
}

static RKADK_S32 RKADK_STORAGE_FileListDel(RKADK_STR_FOLDER *folder,
                                           RKADK_CHAR *filename) {
  RKADK_S32 s32FileNum = 0;
  off_t totalSize = 0;
  off_t totalSpace = 0;
  struct RKADK_STR_FILE *next = NULL;
  struct RKADK_STR_FILE *tmp = NULL;

  RKADK_CHECK_POINTER(folder, RKADK_FAILURE);
  RKADK_CHECK_POINTER(filename, RKADK_FAILURE);

  pthread_mutex_lock(&folder->mutex);

again:
  if (folder->pstFileListFirst) {
    tmp = folder->pstFileListFirst;
    if (!strcmp(tmp->filename, filename)) {
      folder->pstFileListFirst = folder->pstFileListFirst->next;
      free(tmp);

      if (folder->pstFileListFirst == NULL)
        folder->pstFileListLast = NULL;
      else
        folder->pstFileListFirst->prev = NULL;

      goto again;
    }

    while (tmp) {
      next = tmp->next;
      totalSize += tmp->stSize;
      totalSpace += tmp->stSpace;
      s32FileNum++;
      if (next == NULL) {
        folder->pstFileListLast = tmp;
        break;
      }
      if (!strcmp(next->filename, filename)) {
        tmp->next = next->next;
        if (tmp->next)
          tmp->next->prev = tmp;

        free(next);
        next = tmp->next;
        if(tmp->next == NULL)
          folder->pstFileListLast = tmp;
      }
      tmp = next;
    }
  }
  folder->s32FileNum = s32FileNum;
  folder->totalSize = totalSize;
  folder->totalSpace = totalSpace;

  pthread_mutex_unlock(&folder->mutex);
  return 0;
}

static RKADK_S32 RKADK_STORAGE_FileListSave(RKADK_STR_FOLDER pstFolder,
                                            RKADK_STR_FOLDER_ATTR pstFolderAttr,
                                            RKADK_CHAR *cMountPath) {
  RKADK_S32 i, len;
  RKADK_CHAR dataFileName[RKADK_MAX_FILE_PATH_LEN];
  RKADK_CHAR jsonFileName[2 * RKADK_MAX_FILE_PATH_LEN];
  FILE *fp;
  cJSON *folder = NULL;
  cJSON *fileArray = NULL;
  cJSON *info = NULL;
  RKADK_CHAR *folderStr = NULL;
  struct RKADK_STR_FILE *tmp = pstFolder.pstFileListFirst;

  folder = cJSON_CreateObject();
  cJSON_AddStringToObject(folder, JSON_KEY_FOLDER_NAME,
                          pstFolderAttr.cFolderPath);
  cJSON_AddNumberToObject(folder, JSON_KEY_FILE_NUMBER, pstFolder.s32FileNum);
  cJSON_AddNumberToObject(folder, JSON_KEY_TOTAL_SIZE, pstFolder.totalSize);
  cJSON_AddNumberToObject(folder, JSON_KEY_TOTAL_SPACE, pstFolder.totalSpace);
  cJSON_AddItemToObject(folder, JSON_KEY_FILE_ARRAY,
                        fileArray = cJSON_CreateArray());
  for (i = 0; i < pstFolder.s32FileNum && tmp != NULL; i++) {
    cJSON_AddItemToArray(fileArray, info = cJSON_CreateObject());
    cJSON_AddStringToObject(info, JSON_KEY_FILE_NAME, tmp->filename);
    cJSON_AddNumberToObject(info, JSON_KEY_MODIFY_TIME, tmp->stTime);
    cJSON_AddNumberToObject(info, JSON_KEY_FILE_SIZE, tmp->stSize);
    cJSON_AddNumberToObject(info, JSON_KEY_FILE_SPACE, tmp->stSpace);
    tmp = tmp->next;
  }
  folderStr = cJSON_Print(folder);
  cJSON_Delete(folder);

  len = strlen(pstFolderAttr.cFolderPath) - 2;
  strncpy(dataFileName, pstFolderAttr.cFolderPath + 1, len);
  dataFileName[len] = '\0';
  sprintf(jsonFileName, "%s/.%s.json", cMountPath, dataFileName);
  RKADK_LOGD("Save fileList data in %s", jsonFileName);

  if ((fp = fopen(jsonFileName, "w+")) == NULL) {
    RKADK_LOGE("Open %s error!", jsonFileName);
    free(folderStr);
    return -1;
  }

  if (fwrite(folderStr, strlen(folderStr), 1, fp) != 1) {
    RKADK_LOGE("Write file error!");
    fclose(fp);
    free(folderStr);
    return -1;
  }

  fclose(fp);
  sync();
  free(folderStr);
  return 0;
}

static RKADK_S32 RKADK_STORAGE_FileListLoad(RKADK_STR_FOLDER *pstFolder,
                                            RKADK_STR_FOLDER_ATTR pstFolderAttr,
                                            RKADK_CHAR *cMountPath) {
  RKADK_S32 i, len;
  RKADK_S64 lenStr;
  RKADK_CHAR *str;
  RKADK_CHAR dataFileName[RKADK_MAX_FILE_PATH_LEN];
  RKADK_CHAR jsonFileName[2 * RKADK_MAX_FILE_PATH_LEN];
  FILE *fp;
  cJSON *value = NULL;
  cJSON *folder = NULL;
  cJSON *fileArray = NULL;
  cJSON *info = NULL;
  struct RKADK_STR_FILE *tmp = NULL;

  RKADK_CHECK_POINTER(pstFolder, RKADK_FAILURE);

  len = strlen(pstFolderAttr.cFolderPath) - 2;
  strncpy(dataFileName, pstFolderAttr.cFolderPath + 1, len);
  dataFileName[len] = '\0';
  sprintf(jsonFileName, "%s/.%s.json", cMountPath, dataFileName);
  RKADK_LOGD("Load fileList data from %s", jsonFileName);

  if ((fp = fopen(jsonFileName, "r")) == NULL) {
    RKADK_LOGE("Open %s error!", jsonFileName);
    return -1;
  }

  fseek(fp, 0, SEEK_END);
  lenStr = ftell(fp);
  str = (RKADK_CHAR *)malloc(lenStr + 1);
  if (str == NULL) {
    RKADK_LOGE("malloc str failed!");
    fclose(fp);
    return -1;
  }

  fseek(fp, 0, SEEK_SET);
  if (fread(str, lenStr, 1, fp) != 1) {
    RKADK_LOGE("Read file error!");
    fclose(fp);
    free(str);
    return -1;
  }
  str[lenStr] = '\0';
  fclose(fp);

  if ((folder = cJSON_Parse(str)) == NULL) {
    RKADK_LOGE("Parse error!");
    free(str);
    return -1;
  }
  free(str);

  pthread_mutex_lock(&pstFolder->mutex);
  value = cJSON_GetObjectItem(folder, JSON_KEY_FILE_NUMBER);
  pstFolder->s32FileNum = value->valuedouble;
  value = cJSON_GetObjectItem(folder, JSON_KEY_TOTAL_SIZE);
  pstFolder->totalSize = value->valuedouble;
  value = cJSON_GetObjectItem(folder, JSON_KEY_TOTAL_SPACE);
  pstFolder->totalSpace = value->valuedouble;
  if ((fileArray = cJSON_GetObjectItem(folder, JSON_KEY_FILE_ARRAY)) == NULL) {
    RKADK_LOGE("Get fileArray object item error!");
    cJSON_Delete(folder);
    pthread_mutex_unlock(&pstFolder->mutex);
    return -1;
  }

  for (i = 0; i < pstFolder->s32FileNum; i++) {
    tmp = (struct RKADK_STR_FILE *)malloc(sizeof(struct RKADK_STR_FILE));
    if (!tmp) {
      RKADK_LOGE("tmp malloc failed.");
      cJSON_Delete(folder);
      pthread_mutex_unlock(&pstFolder->mutex);
      return -1;
    }

    memset(tmp, 0, sizeof(struct RKADK_STR_FILE));
    info = cJSON_GetArrayItem(fileArray, i);
    value = cJSON_GetObjectItem(info, JSON_KEY_FILE_NAME);
    sprintf(tmp->filename, "%s", value->valuestring);
    value = cJSON_GetObjectItem(info, JSON_KEY_FILE_SIZE);
    tmp->stSize = value->valuedouble;
    value = cJSON_GetObjectItem(info, JSON_KEY_FILE_SPACE);
    tmp->stSpace = value->valuedouble;
    value = cJSON_GetObjectItem(info, JSON_KEY_MODIFY_TIME);
    tmp->stTime = value->valuedouble;
    tmp->next = NULL;

    if (pstFolder->pstFileListFirst) {
      pstFolder->pstFileListLast->next = tmp;
      pstFolder->pstFileListLast = tmp;
    } else {
      pstFolder->pstFileListFirst = tmp;
      pstFolder->pstFileListLast = tmp;
    }
  }

  cJSON_Delete(folder);
  pthread_mutex_unlock(&pstFolder->mutex);
  return 0;
}

static RKADK_S32 RKADK_STORAGE_Repair(RKADK_STORAGE_HANDLE *pHandle, RKADK_STR_DEV_ATTR *pdevAttr)
{
  int i;
  int j;
  RKADK_S32 ret = 0;
  RKADK_CHAR file[3 * RKADK_MAX_FILE_PATH_LEN];

  for (i = 0; i < pdevAttr->s32FolderNum; i++) {
    RKADK_STR_FOLDER *folder = &pHandle->stDevSta.pstFolder[i];
    struct RKADK_STR_FILE *current = NULL;
    struct RKADK_STR_FILE *next = NULL;

    pthread_mutex_lock(&folder->mutex);
    j = 0;
again:
    current = folder->pstFileListFirst;
    if ((current) && (j < REPAIR_FILE_NUM)) {
      j++;
      snprintf(file, 3 * RKADK_MAX_FILE_PATH_LEN, "%s%s%s", pdevAttr->cMountPath,
              pdevAttr->pstFolderAttr[i].cFolderPath,
              current->filename);
      if ((current->stSize == 0) || (repair_mp4(file) == REPA_FAIL)) {
        RKADK_LOGE("Delete %s file. %lld", file, current->stSize);
        if (remove(file))
          RKADK_LOGE("Delete %s file error.", file);
        folder->pstFileListFirst = current->next;
        free(current);
        if (folder->pstFileListFirst == NULL) {
          folder->pstFileListLast = NULL;
        } else {
          folder->pstFileListFirst->prev = NULL;
          if (folder->pstFileListFirst->next == NULL)
            folder->pstFileListLast = folder->pstFileListFirst;
        }
        folder->s32FileNum--;
        goto again;
      }
    }

    current = folder->pstFileListFirst;
    for (; j < REPAIR_FILE_NUM && current && current->next; j++) {
      snprintf(file, 3 * RKADK_MAX_FILE_PATH_LEN, "%s%s%s", pdevAttr->cMountPath,
              pdevAttr->pstFolderAttr[i].cFolderPath,
              current->next->filename);
      if ((current->next->stSize == 0) || (repair_mp4(file) == REPA_FAIL)) {
        RKADK_LOGE("Delete %s file. %lld", file, current->next->stSize);
        if (remove(file))
          RKADK_LOGE("Delete %s file error.", file);
        next = current->next;
        current->next = next->next;
        if (current->next)
          current->next->prev = current;

        free(next);
        if (current->next == NULL)
          folder->pstFileListLast = current;
        folder->s32FileNum--;
      }
      current = current->next;
    }
    pthread_mutex_unlock(&folder->mutex);
  }
  sync();

  return ret;
}

static RKADK_MW_PTR RKADK_STORAGE_FileMonitorThread(RKADK_MW_PTR arg) {
  RKADK_STORAGE_HANDLE *pHandle = (RKADK_STORAGE_HANDLE *)arg;
  RKADK_S32 fd;
  RKADK_S32 len;
  RKADK_S32 nread;
  RKADK_CHAR buf[BUFSIZ];
  struct inotify_event *event;
  RKADK_S32 j;

  if (!pHandle) {
    RKADK_LOGE("invalid pHandle");
    return NULL;
  }

  prctl(PR_SET_NAME, "RKADK_STORAGE_FileMonitorThread", 0, 0, 0);
  fd = inotify_init();
  if (fd < 0) {
    RKADK_LOGE("inotify_init failed");
    return NULL;
  }

  for (j = 0; j < pHandle->stDevSta.s32FolderNum; j++) {
    pHandle->stDevSta.pstFolder[j].wd =
        inotify_add_watch(fd, pHandle->stDevSta.pstFolder[j].cpath,
                          IN_CREATE | IN_MOVED_TO | IN_DELETE | IN_MOVED_FROM |
                              IN_CLOSE_WRITE | IN_UNMOUNT);
  }

  memset(buf, 0, BUFSIZ);
  while (pHandle->stDevSta.s32MountStatus == DISK_MOUNTED) {
    if (RKADK_STORAGE_ReadTimeout(fd, 10))
      continue;

    len = read(fd, buf, BUFSIZ - 1);
    nread = 0;
    while (len > 0) {
      event = (struct inotify_event *)&buf[nread];
      if (event->mask & IN_UNMOUNT) {
        pHandle->stDevSta.s32MountStatus = DISK_UNMOUNTED;
        //RKADK_STORAGE_ProcessStatus(pHandle, pHandle->stDevSta.s32MountStatus);
      }

      if (event->len > 0) {
        for (j = 0; j < pHandle->stDevSta.s32FolderNum; j++) {
          if (event->wd == pHandle->stDevSta.pstFolder[j].wd) {
            if (event->mask & IN_MOVED_TO) {
              RKADK_CHAR d_name[RKADK_MAX_FILE_PATH_LEN];
              struct stat statbuf;
              sprintf(d_name, "%s%s", pHandle->stDevSta.pstFolder[j].cpath,
                      event->name);
              if (lstat(d_name, &statbuf)) {
                RKADK_LOGE("lstat[%s](IN_MOVED_TO) failed", d_name);
              } else {
                if ((RKADK_STORAGE_FileListCheck(&pHandle->stDevSta.pstFolder[j],
                                              event->name, &statbuf) == 0) &&
                    RKADK_STORAGE_FileListAdd(&pHandle->stDevSta.pstFolder[j],
                                                event->name, &statbuf))
                  RKADK_LOGE("FileListAdd failed");
              }
            }

            if ((event->mask & IN_DELETE) || (event->mask & IN_MOVED_FROM))
              if (RKADK_STORAGE_FileListDel(&pHandle->stDevSta.pstFolder[j],
                                            event->name))
                RKADK_LOGE("FileListDel failed");

            if (event->mask & IN_CLOSE_WRITE) {
              RKADK_CHAR d_name[RKADK_MAX_FILE_PATH_LEN];
              struct stat statbuf;
              sprintf(d_name, "%s%s", pHandle->stDevSta.pstFolder[j].cpath,
                      event->name);
              if (lstat(d_name, &statbuf)) {
                RKADK_LOGE("lstat[%s](IN_CLOSE_WRITE) failed", d_name);
              } else {
                if (statbuf.st_size == 0) {
                  if (remove(d_name))
                    RKADK_LOGE("Delete %s file error.", d_name);
                } else if ((RKADK_STORAGE_FileListCheck(&pHandle->stDevSta.pstFolder[j],
                                              event->name, &statbuf) == 0) &&
                            RKADK_STORAGE_FileListAdd(&pHandle->stDevSta.pstFolder[j],
                                              event->name, &statbuf)) {
                  RKADK_LOGE("FileListAdd failed");
                }
              }
            }
          }
        }
      }

      nread = nread + sizeof(struct inotify_event) + event->len;
      len = len - sizeof(struct inotify_event) - event->len;
    }
  }

  RKADK_LOGD("Exit!");
  close(fd);
  return NULL;
}

static RKADK_MW_PTR RKADK_STORAGE_FileScanThread(RKADK_MW_PTR arg) {
  RKADK_STORAGE_HANDLE *pHandle = (RKADK_STORAGE_HANDLE *)arg;
  RKADK_S32 cnt = 0;
  RKADK_S32 i;
  pthread_t fileMonitorTid = 0;
  RKADK_STR_DEV_ATTR devAttr;
  RKFSCK_RET_TYPE fsck_ret;

  if (!pHandle) {
    RKADK_LOGE("invalid pHandle");
    return NULL;
  }

  devAttr = RKADK_STORAGE_GetParam(pHandle);
  prctl(PR_SET_NAME, "file_scan_thread", 0, 0, 0);
  RKADK_LOGI("%s, %s, %s, %s", devAttr.cMountPath, pHandle->stDevSta.cDevPath,
             pHandle->stDevSta.cDevType, pHandle->stDevSta.cDevAttr1);

  RKADK_LOGI("devAttr.s32FolderNum = %d", devAttr.s32FolderNum);
  pHandle->stDevSta.s32FolderNum = devAttr.s32FolderNum;

  pHandle->stDevSta.pstFolder = (RKADK_STR_FOLDER *)malloc(
      sizeof(RKADK_STR_FOLDER) * devAttr.s32FolderNum);
  if (!pHandle->stDevSta.pstFolder) {
    RKADK_LOGE("pHandle->stDevSta.pstFolder malloc failed.");
    return NULL;
  }
  memset(pHandle->stDevSta.pstFolder, 0,
          sizeof(RKADK_STR_FOLDER) * devAttr.s32FolderNum);
  for (i = 0; i < pHandle->stDevSta.s32FolderNum; i++) {
    sprintf(pHandle->stDevSta.pstFolder[i].cpath, "%s%s", devAttr.cMountPath,
            devAttr.pstFolderAttr[i].cFolderPath);
    RKADK_LOGI("%s", pHandle->stDevSta.pstFolder[i].cpath);

    pthread_mutex_init(&(pHandle->stDevSta.pstFolder[i].mutex), NULL);
    if (pHandle->stDevSta.s32MountStatus != DISK_UNMOUNTED) {
      if (RKADK_STORAGE_CreateFolder(pHandle->stDevSta.pstFolder[i].cpath)) {
        RKADK_LOGE("CreateFolder failed");
        goto file_scan_out;
      }
    }
  }

  fsck_ret = RKADK_STORAGE_RKFSCK(pHandle, &devAttr);
  if (RKFSCK_ID_ERR == fsck_ret ||
      RKFSCK_FAIL == fsck_ret) {
    RKADK_LOGE("RKFSCK_ID_ERR");
    pHandle->stDevSta.s32MountStatus = DISK_NOT_FORMATTED;
    RKADK_STORAGE_ProcessStatus(pHandle, pHandle->stDevSta.s32MountStatus);
    goto file_scan_out;
  }

  RKADK_STORAGE_Repair(pHandle, &devAttr);

  if (pHandle->stDevSta.s32MountStatus == DISK_UNMOUNTED) {
    goto file_scan_out;
  } else {
    pHandle->stDevSta.s32MountStatus = DISK_MOUNTED;
    RKADK_STORAGE_ProcessStatus(pHandle, pHandle->stDevSta.s32MountStatus);
  }


  if (pHandle->stDevSta.s32MountStatus != DISK_UNMOUNTED) {
    if (RKADK_STORAGE_GetDiskSize(devAttr.cMountPath,
                                  &pHandle->stDevSta.s32TotalSize,
                                  &pHandle->stDevSta.s32FreeSize)) {
      RKADK_LOGE("GetDiskSize failed");
      goto file_scan_out;
    }
  } else {
    pHandle->stDevSta.s32TotalSize = 0;
    pHandle->stDevSta.s32FreeSize = 0;
  }
  RKADK_LOGI("s32TotalSize = %d, s32FreeSize = %d",
             pHandle->stDevSta.s32TotalSize, pHandle->stDevSta.s32FreeSize);

  if (pthread_create(&fileMonitorTid, NULL, RKADK_STORAGE_FileMonitorThread,
                     (RKADK_MW_PTR)pHandle)) {
    RKADK_LOGE("FileMonitorThread create failed.");
    goto file_scan_out;
  }

  while (pHandle->stDevSta.s32MountStatus == DISK_MOUNTED) {
    if (cnt++ > 50) {
      RKADK_S32 limit;
      off_t totalSpace = 0;
      cnt = 0;
      for (i = 0; i < devAttr.s32FolderNum; i++) {
        if (devAttr.pstFolderAttr[i].bNumLimit == RKADK_TRUE) {
          RKADK_CHAR file[3 * RKADK_MAX_FILE_PATH_LEN];

          pthread_mutex_lock(&pHandle->stDevSta.pstFolder[i].mutex);
          limit = pHandle->stDevSta.pstFolder[i].s32FileNum;
          if (limit > devAttr.pstFolderAttr[i].s32Limit) {
            if (pHandle->stDevSta.pstFolder[i].pstFileListLast) {
              sprintf(
                  file, "%s%s%s", devAttr.cMountPath,
                  devAttr.pstFolderAttr[i].cFolderPath,
                  pHandle->stDevSta.pstFolder[i].pstFileListLast->filename);
              RKADK_LOGI("Delete file:%s", file);
              pthread_mutex_unlock(&pHandle->stDevSta.pstFolder[i].mutex);

              if (remove(file)) {
                RKADK_CHAR filename[RKADK_MAX_FILE_PATH_LEN];
                snprintf(filename, RKADK_MAX_FILE_PATH_LEN, "%s",
                    pHandle->stDevSta.pstFolder[i].pstFileListLast->filename);
                RKADK_STORAGE_FileListDel(&pHandle->stDevSta.pstFolder[i],
                                            filename);
                RKADK_LOGE("Delete %s file error.", file);
              }
              usleep(100);
              cnt = 51;
              continue;
            }
          }
          pthread_mutex_unlock(&pHandle->stDevSta.pstFolder[i].mutex);
        }
      }

      if (RKADK_STORAGE_GetDiskSize(devAttr.cMountPath,
                                    &pHandle->stDevSta.s32TotalSize,
                                    &pHandle->stDevSta.s32FreeSize)) {
        RKADK_LOGE("GetDiskSize failed");
        goto file_scan_out;
      }

      if (pHandle->stDevSta.s32FreeSize <= (devAttr.s32FreeSizeDelMin * 1024))
        devAttr.s32AutoDel = 1;

      if (pHandle->stDevSta.s32FreeSize >= (devAttr.s32FreeSizeDelMax * 1024))
        devAttr.s32AutoDel = 0;

      if (devAttr.s32AutoDel) {
        for (i = 0; i < devAttr.s32FolderNum; i++) {
          pthread_mutex_lock(&pHandle->stDevSta.pstFolder[i].mutex);
          if (devAttr.pstFolderAttr[i].bNumLimit == RKADK_FALSE)
            totalSpace += pHandle->stDevSta.pstFolder[i].totalSpace;
          pthread_mutex_unlock(&pHandle->stDevSta.pstFolder[i].mutex);
        }
        if (totalSpace) {
          for (i = 0; i < devAttr.s32FolderNum; i++) {
            if (devAttr.pstFolderAttr[i].bNumLimit == RKADK_FALSE) {
              RKADK_CHAR file[3 * RKADK_MAX_FILE_PATH_LEN];
              pthread_mutex_lock(&pHandle->stDevSta.pstFolder[i].mutex);
              limit =
                  pHandle->stDevSta.pstFolder[i].totalSpace * 100 / totalSpace;
              if (limit > devAttr.pstFolderAttr[i].s32Limit) {
                if (pHandle->stDevSta.pstFolder[i].pstFileListLast) {
                  sprintf(
                      file, "%s%s%s", devAttr.cMountPath,
                      devAttr.pstFolderAttr[i].cFolderPath,
                      pHandle->stDevSta.pstFolder[i].pstFileListLast->filename);
                  RKADK_LOGI("Delete file:%s", file);
                  pthread_mutex_unlock(&pHandle->stDevSta.pstFolder[i].mutex);

                  if (remove(file)) {
                    RKADK_CHAR filename[RKADK_MAX_FILE_PATH_LEN];
                    snprintf(filename, RKADK_MAX_FILE_PATH_LEN, "%s",
                        pHandle->stDevSta.pstFolder[i].pstFileListLast->filename);
                    RKADK_STORAGE_FileListDel(&pHandle->stDevSta.pstFolder[i],
                                            filename);
                    RKADK_LOGE("Delete %s file error.", file);
                  }
                  usleep(100);
                  cnt = 51;
                  continue;
                }
              }
              pthread_mutex_unlock(&pHandle->stDevSta.pstFolder[i].mutex);
            }
          }
        }
      }
    }
    usleep(10000);
  }

file_scan_out:
  if (fileMonitorTid)
    if (pthread_join(fileMonitorTid, NULL))
      RKADK_LOGE("FileMonitorThread join failed.");
  RKADK_LOGD("out");

  if (pHandle->stDevSta.pstFolder) {
    for (i = 0; i < pHandle->stDevSta.s32FolderNum; i++)
      pthread_mutex_destroy(&(pHandle->stDevSta.pstFolder[i].mutex));

    free(pHandle->stDevSta.pstFolder);
    pHandle->stDevSta.pstFolder = NULL;
  }
  pHandle->stDevSta.s32FolderNum = 0;

  return NULL;
}

static void cb(void *userdata, char *filename, int dir, struct stat *statbuf)
{
    if (dir == 0) {
        RKADK_STR_FOLDER *pstFolder = (RKADK_STR_FOLDER *)userdata;
        RKADK_STORAGE_FileListAdd(pstFolder, filename, statbuf);
    }
}

static RKADK_S32 RKADK_STORAGE_RKFSCK(RKADK_STORAGE_HANDLE *pHandle, RKADK_STR_DEV_ATTR *pdevAttr)
{
  int i;
  RKADK_S32 ret = 0;
  struct reg_para para;

  para.folder_num = pdevAttr->s32FolderNum;
  para.folder = (struct folder_para *)malloc(sizeof(struct folder_para) * para.folder_num);
  if (para.folder == NULL) {
    RKADK_LOGE("malloc para.folder failed!");
    return -1;
  }

  memcpy(para.format_id, pdevAttr->cFormatId, RKADK_MAX_FORMAT_ID_LEN);
  para.check_format_id = pdevAttr->s32CheckFormatId;
  para.quit = &pHandle->stDevSta.s32FsckQuit;
  pHandle->stDevSta.s32FsckQuit = 0;
  for (i = 0; i < para.folder_num; i ++) {
    para.folder[i].path = pdevAttr->pstFolderAttr[i].cFolderPath;
    para.folder[i].userdata = &pHandle->stDevSta.pstFolder[i];
    para.folder[i].cb = &cb;
  }

  umount2(pHandle->stDevAttr.cMountPath, MNT_DETACH);
  ret = rkfsmk_fat_check(pHandle->stDevSta.cDevPath, &para);
  sync();
  mount(pHandle->stDevSta.cDevPath, pHandle->stDevAttr.cMountPath, "vfat", MS_NOATIME | MS_NOSUID, NULL);

  return ret;
}

static RKADK_S32 RKADK_STORAGE_DevAdd(RKADK_CHAR *dev,
                                      RKADK_STORAGE_HANDLE *pHandle) {
  RKADK_S32 ret;
  RKADK_STR_DEV_ATTR stDevAttr;
  RKADK_CHAR mountPath[RKADK_MAX_FILE_PATH_LEN];

  RKADK_CHECK_POINTER(dev, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  stDevAttr = RKADK_STORAGE_GetParam(pHandle);
  RKADK_LOGI("%s, %s", dev, mountPath);

  if (stDevAttr.cDevPath[0]) {
    if (strcmp(stDevAttr.cDevPath, dev)) {
      RKADK_LOGE("stDevAttr.cDevPath[%s] != dev[%s]",
                 stDevAttr.cDevPath, dev);
      return -1;
    }
    sprintf(pHandle->stDevSta.cDevPath, stDevAttr.cDevPath);
  }

  ret = RKADK_STORAGE_GetMountPath(dev, mountPath, RKADK_MAX_FILE_PATH_LEN);
  if (ret) {
    RKADK_LOGE("RKADK_STORAGE_GetMountPath failed[%d]", ret);
    if (stDevAttr.cDevPath[0]) {
      pHandle->stDevSta.s32MountStatus = DISK_NOT_FORMATTED;
      RKADK_STORAGE_ProcessStatus(pHandle, pHandle->stDevSta.s32MountStatus);
    }
    return ret;
  }

  if (stDevAttr.cMountPath[0]) {
    if (strcmp(stDevAttr.cMountPath, mountPath)) {
      RKADK_LOGE("stDevAttr.cMountPath[%s] != mountPath[%s]",
                 stDevAttr.cMountPath, mountPath);
      return -1;
    }
  } else {
    sprintf(stDevAttr.cMountPath, mountPath);
  }

  ret = RKADK_STORAGE_GetMountDev(
      stDevAttr.cMountPath, pHandle->stDevSta.cDevPath,
      pHandle->stDevSta.cDevType, pHandle->stDevSta.cDevAttr1);
  if (ret) {
    RKADK_LOGE("RKADK_STORAGE_GetMountDev failed[%d]", ret);
    return ret;
  }

  pHandle->stDevSta.s32MountStatus = DISK_SCANNING;
  RKADK_STORAGE_ProcessStatus(pHandle, pHandle->stDevSta.s32MountStatus);
  if (pthread_create(&pHandle->stDevSta.fileScanTid, NULL,
                     RKADK_STORAGE_FileScanThread, (RKADK_MW_PTR)pHandle))
    RKADK_LOGE("FileScanThread create failed.");

  return 0;
}

static RKADK_S32 RKADK_STORAGE_DevRemove(RKADK_CHAR *dev,
                                         RKADK_STORAGE_HANDLE *pHandle) {
  RKADK_CHECK_POINTER(dev, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  if (!strcmp(pHandle->stDevSta.cDevPath, dev)) {
    pHandle->stDevSta.s32MountStatus = DISK_UNMOUNTED;
    RKADK_STORAGE_ProcessStatus(pHandle, pHandle->stDevSta.s32MountStatus);
    pHandle->stDevSta.s32TotalSize = 0;
    pHandle->stDevSta.s32FreeSize = 0;
    pHandle->stDevSta.s32FsckQuit = 1;

    if (pHandle->stDevSta.fileScanTid) {
      if (pthread_join(pHandle->stDevSta.fileScanTid, NULL))
        RKADK_LOGE("FileScanThread join failed.");
      pHandle->stDevSta.fileScanTid = 0;
    }
    umount2(pHandle->stDevAttr.cMountPath, MNT_DETACH);
  }

  return 0;
}

static RKADK_S32 RKADK_STORAGE_MsgPutMsgToBuffer(RKADK_TMSG_BUFFER *buf,
                                                 struct RKADK_TMSG_ELEMENT *elm) {
  RKADK_CHECK_POINTER(buf, RKADK_FAILURE);
  RKADK_CHECK_POINTER(elm, RKADK_FAILURE);

  if (NULL != elm->next)
    elm->next = NULL;

  pthread_mutex_lock(&buf->mutex);
  if (buf->first) {
    struct RKADK_TMSG_ELEMENT *tmp = buf->first;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = elm;
  } else {
    buf->first = elm;
  }
  buf->num++;

  pthread_cond_signal(&buf->notEmpty);
  pthread_mutex_unlock(&buf->mutex);
  return 0;
}

static struct RKADK_TMSG_ELEMENT *
RKADK_STORAGE_MsgGetMsgFromBufferTimeout(RKADK_TMSG_BUFFER *buf,
                                         RKADK_S32 s32TimeoutMs) {
  struct RKADK_TMSG_ELEMENT *elm = NULL;
  struct timeval timeNow;
  struct timespec timeout;

  if (!buf) {
    return NULL;
  }

  pthread_mutex_lock(&buf->mutex);
  if (0 == buf->num) {
    gettimeofday(&timeNow, NULL);
    timeout.tv_sec = timeNow.tv_sec + s32TimeoutMs / 1000;
    timeout.tv_nsec = (timeNow.tv_usec + (s32TimeoutMs % 1000) * 1000) * 1000;
    pthread_cond_timedwait(&buf->notEmpty, &buf->mutex, &timeout);
  }

  if (buf->num > 0) {
    elm = buf->first;
    if (1 == buf->num) {
      buf->first = buf->last = NULL;
      buf->num = 0;
    } else {
      buf->first = buf->first->next;
      buf->num--;
    }
  }

  pthread_mutex_unlock(&buf->mutex);
  return elm;
}

static RKADK_S32 RKADK_STORAGE_MsgFreeMsg(struct RKADK_TMSG_ELEMENT *elm) {
  RKADK_CHECK_POINTER(elm, RKADK_FAILURE);

  if (elm->data != NULL) {
    free(elm->data);
    elm->data = NULL;
  }
  free(elm);
  elm = NULL;

  return 0;
}

static RKADK_MW_PTR RKADK_STORAGE_MsgRecMsgThread(RKADK_MW_PTR arg) {
  RKADK_TMSG_BUFFER *msgBuffer = (RKADK_TMSG_BUFFER *)arg;

  if (!msgBuffer) {
    RKADK_LOGE("invalid msgBuffer");
    return NULL;
  }

  prctl(PR_SET_NAME, "RKADK_STORAGE_MsgRecMsgThread", 0, 0, 0);
  while (msgBuffer->quit == 0) {
    struct RKADK_TMSG_ELEMENT *elm =
        RKADK_STORAGE_MsgGetMsgFromBufferTimeout(msgBuffer, 50);

    if (elm) {
      if (msgBuffer->recMsgCb)
        msgBuffer->recMsgCb(msgBuffer, elm->msg, elm->data, elm->s32DataLen,
                            msgBuffer->pHandlePath);
      if (RKADK_STORAGE_MsgFreeMsg(elm))
        RKADK_LOGE("Free msg failed.");
    }
  }

  RKADK_LOGD("out");
  return NULL;
}

static RKADK_S32 RKADK_STORAGE_MsgRecCb(RKADK_MW_PTR hd, RKADK_S32 msg,
                                        RKADK_MW_PTR data, RKADK_S32 s32DataLen,
                                        RKADK_MW_PTR pHandle) {
  RKADK_LOGI("msg = %d", msg);
  switch (msg) {
  case MSG_DEV_ADD:
    if (RKADK_STORAGE_DevAdd((RKADK_CHAR *)data,
                             (RKADK_STORAGE_HANDLE *)pHandle)) {
      RKADK_LOGE("DevAdd failed");
      return -1;
    }
    break;
  case MSG_DEV_REMOVE:
    if (RKADK_STORAGE_DevRemove((RKADK_CHAR *)data,
                                (RKADK_STORAGE_HANDLE *)pHandle)) {
      RKADK_LOGE("DevRemove failed");
      return -1;
    }
    break;
  case MSG_DEV_CHANGED:
    break;
  }

  return 0;
}

static RKADK_S32 RKADK_STORAGE_MsgCreate(RKADK_REC_MSG_CB recMsgCb,
                                         RKADK_STORAGE_HANDLE *pHandle) {
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  pHandle->stMsgHd.first = NULL;
  pHandle->stMsgHd.last = NULL;
  pHandle->stMsgHd.num = 0;
  pHandle->stMsgHd.quit = 0;
  pHandle->stMsgHd.recMsgCb = recMsgCb;
  pHandle->stMsgHd.pHandlePath = (RKADK_MW_PTR)pHandle;

  pthread_mutex_init(&(pHandle->stMsgHd.mutex), NULL);
  pthread_cond_init(&(pHandle->stMsgHd.notEmpty), NULL);
  if (pthread_create(&(pHandle->stMsgHd.recTid), NULL,
                     RKADK_STORAGE_MsgRecMsgThread,
                     (RKADK_MW_PTR)(&pHandle->stMsgHd))) {
    RKADK_LOGE("RecMsgThread create failed!");
    return -1;
  }

  return 0;
}

static RKADK_S32 RKADK_STORAGE_MsgDestroy(RKADK_STORAGE_HANDLE *pHandle) {
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  pHandle->stMsgHd.quit = 1;
  if (pHandle->stMsgHd.recTid)
    if (pthread_join(pHandle->stMsgHd.recTid, NULL)) {
      RKADK_LOGE("RecMsgThread join failed!");
      return -1;
    }

  pthread_cond_destroy(&pHandle->stMsgHd.notEmpty);
  pthread_mutex_destroy(&pHandle->stMsgHd.mutex);
  return 0;
}

static RKADK_S32 RKADK_STORAGE_MsgSendMsg(RKADK_S32 msg, RKADK_CHAR *data,
                                          RKADK_S32 s32DataLen,
                                          RKADK_TMSG_BUFFER *buf) {
  struct RKADK_TMSG_ELEMENT *elm = NULL;

  RKADK_CHECK_POINTER(buf, RKADK_FAILURE);
  RKADK_CHECK_POINTER(data, RKADK_FAILURE);

  elm = (struct RKADK_TMSG_ELEMENT *)malloc(sizeof(struct RKADK_TMSG_ELEMENT));
  if (!elm) {
    RKADK_LOGE("elm malloc failed.");
    return -1;
  }

  memset(elm, 0, sizeof(struct RKADK_TMSG_ELEMENT));
  elm->msg = msg;
  elm->data = NULL;
  elm->s32DataLen = s32DataLen;

  if (data && s32DataLen > 0) {
    elm->data = (RKADK_CHAR *)malloc(s32DataLen);
    if (!elm->data) {
      RKADK_LOGE("elm->data malloc failed.");
      free(elm);
      return -1;
    }
    memset(elm->data, 0, s32DataLen);
    memcpy(elm->data, data, s32DataLen);
  }

  elm->next = NULL;

  if (RKADK_STORAGE_MsgPutMsgToBuffer(buf, elm)) {
    if (!elm->data)
      free(elm->data);
    free(elm);
    RKADK_LOGE("Put msg to buffer failed.");
    return -1;
  }

  return 0;
}

static RKADK_CHAR *RKADK_STORAGE_Search(RKADK_CHAR *buf, RKADK_S32 len,
                                        const RKADK_CHAR *str) {
  RKADK_CHAR *ret = 0;
  RKADK_S32 i = 0;

  ret = strstr(buf, str);
  if (ret)
    return ret;
  for (i = 1; i < len; i++) {
    if (buf[i - 1] == 0) {
      ret = strstr(&buf[i], str);
      if (ret)
        return ret;
    }
  }
  return ret;
}

static RKADK_CHAR *RKADK_STORAGE_Getparameters(RKADK_CHAR *buf, RKADK_S32 len,
                                               const RKADK_CHAR *str) {
  RKADK_CHAR *ret = RKADK_STORAGE_Search(buf, len, str);

  if (ret)
    ret += strlen(str) + 1;

  return ret;
}

static RKADK_MW_PTR RKADK_STORAGE_EventListenerThread(RKADK_MW_PTR arg) {
  RKADK_STORAGE_HANDLE *pHandle = (RKADK_STORAGE_HANDLE *)arg;
  RKADK_S32 sockfd;
  RKADK_S32 len;
  RKADK_S32 bufLen = 2000;
  RKADK_CHAR buf[bufLen];
  struct iovec iov;
  struct msghdr msg;
  struct sockaddr_nl sa;
  struct timeval timeout;

  if (!pHandle) {
    RKADK_LOGE("invalid pHandle");
    return NULL;
  }

  timeout.tv_sec = 0;
  timeout.tv_usec = 100000;
  prctl(PR_SET_NAME, "event_monitor", 0, 0, 0);

  sa.nl_family = AF_NETLINK;
  sa.nl_groups = NETLINK_KOBJECT_UEVENT;
  sa.nl_pid = 0;
  iov.iov_base = (RKADK_MW_PTR)buf;
  iov.iov_len = bufLen;
  memset(&msg, 0, sizeof(msg));
  msg.msg_name = (RKADK_MW_PTR)&sa;
  msg.msg_namelen = sizeof(sa);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
  if (sockfd == -1) {
    RKADK_LOGE("socket creating failed:%s", strerror(errno));
    return NULL;
  }

  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (RKADK_MW_PTR)&timeout,
             (socklen_t)sizeof(struct timeval));

  if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    RKADK_LOGE("bind error:%s", strerror(errno));
    goto err_event_listener;
  }

  while (pHandle->eventListenerRun) {
    len = recvmsg(sockfd, &msg, 0);
    if (len < 0) {
      // RKADK_LOGW("receive time out");
    } else if (len < MAX_TYPE_NMSG_LEN || len > bufLen) {
      RKADK_LOGW("invalid message");
    } else {
      RKADK_CHAR *p = strstr(buf, "libudev");

      if (p == buf) {
        if (RKADK_STORAGE_Search(buf, len, "DEVTYPE=partition") ||
            RKADK_STORAGE_Search(buf, len, "DEVTYPE=disk")) {
          RKADK_CHAR *dev = RKADK_STORAGE_Getparameters(buf, len, "DEVNAME");

          if (RKADK_STORAGE_Search(buf, len, "ACTION=add")) {
            if (RKADK_STORAGE_MsgSendMsg(MSG_DEV_ADD, dev, strlen(dev) + 1,
                                         &(pHandle->stMsgHd)))
              RKADK_LOGE("Send msg: MSG_DEV_ADD failed.");
          } else if (RKADK_STORAGE_Search(buf, len, "ACTION=remove")) {
            RKADK_LOGI("%s remove", dev);
            if (RKADK_STORAGE_MsgSendMsg(MSG_DEV_REMOVE, dev, strlen(dev) + 1,
                                         &(pHandle->stMsgHd)))
              RKADK_LOGE("Send msg: MSG_DEV_REMOVE failed.");
          } else if (RKADK_STORAGE_Search(buf, len, "ACTION=change")) {
            RKADK_LOGI("%s change", dev);
            if (RKADK_STORAGE_MsgSendMsg(MSG_DEV_CHANGED, dev, strlen(dev) + 1,
                                         &(pHandle->stMsgHd)))
              RKADK_LOGE("Send msg: MSG_DEV_CHANGED failed.");
          }
        }
      }
    }
  }
err_event_listener:
  if (close(sockfd))
    RKADK_LOGE("Close sockfd failed.\n");

  RKADK_LOGD("out");
  return NULL;
}

static RKADK_S32 RKADK_STORAGE_ParameterInit(RKADK_STORAGE_HANDLE *pstHandle,
                                             RKADK_STR_DEV_ATTR *pstDevAttr) {
  RKADK_S32 i;

  RKADK_CHECK_POINTER(pstHandle, RKADK_FAILURE);

  if (pstDevAttr) {
    if (pstDevAttr->pstFolderAttr) {
      sprintf(pstHandle->stDevAttr.cMountPath, pstDevAttr->cMountPath);
      sprintf(pstHandle->stDevAttr.cDevPath, pstDevAttr->cDevPath);
      pstHandle->stDevAttr.s32AutoDel = pstDevAttr->s32AutoDel;
      pstHandle->stDevAttr.s32FreeSizeDelMin = pstDevAttr->s32FreeSizeDelMin;
      pstHandle->stDevAttr.s32FreeSizeDelMax = pstDevAttr->s32FreeSizeDelMax;
      pstHandle->stDevAttr.s32FolderNum = pstDevAttr->s32FolderNum;
      pstHandle->stDevAttr.s32CheckFormatId = pstDevAttr->s32CheckFormatId;
      memcpy(pstHandle->stDevAttr.cFormatId, pstDevAttr->cFormatId, RKADK_MAX_FORMAT_ID_LEN);
      memcpy(pstHandle->stDevAttr.cVolume, pstDevAttr->cVolume, RKADK_MAX_VOLUME_LEN);

      pstHandle->stDevAttr.pstFolderAttr = (RKADK_STR_FOLDER_ATTR *)malloc(
          sizeof(RKADK_STR_FOLDER_ATTR) * pstHandle->stDevAttr.s32FolderNum);
      if (!pstHandle->stDevAttr.pstFolderAttr) {
        RKADK_LOGE("pstHandle->stDevAttr.pstFolderAttr malloc failed.");
        return -1;
      }
      memset(pstHandle->stDevAttr.pstFolderAttr, 0,
             sizeof(RKADK_STR_FOLDER_ATTR) * pstHandle->stDevAttr.s32FolderNum);

      for (i = 0; i < pstDevAttr->s32FolderNum; i++) {
        pstHandle->stDevAttr.pstFolderAttr[i].s32SortCond =
            pstDevAttr->pstFolderAttr[i].s32SortCond;
        pstHandle->stDevAttr.pstFolderAttr[i].bNumLimit =
            pstDevAttr->pstFolderAttr[i].bNumLimit;
        pstHandle->stDevAttr.pstFolderAttr[i].s32Limit =
            pstDevAttr->pstFolderAttr[i].s32Limit;
        sprintf(pstHandle->stDevAttr.pstFolderAttr[i].cFolderPath,
                pstDevAttr->pstFolderAttr[i].cFolderPath);
      }

      for (i = 0; i < pstDevAttr->s32FolderNum; i++) {
        RKADK_LOGI("DevAttr set:  AutoDel--%d, FreeSizeDel--%d~%d, Path--%s%s, "
                   "Limit--%d",
                   pstHandle->stDevAttr.s32AutoDel,
                   pstHandle->stDevAttr.s32FreeSizeDelMin,
                   pstHandle->stDevAttr.s32FreeSizeDelMax,
                   pstHandle->stDevAttr.cMountPath,
                   pstHandle->stDevAttr.pstFolderAttr[i].cFolderPath,
                   pstHandle->stDevAttr.pstFolderAttr[i].s32Limit);
      }

      RKADK_LOGD("Set user-defined device attributes done.");
      return 0;
    } else {
      RKADK_LOGE("The device attributes set failed.");
      return -1;
    }
  }

  RKADK_LOGD("Set default device attributes.");
  sprintf(pstHandle->stDevAttr.cMountPath, "/mnt/sdcard");
  sprintf(pstHandle->stDevAttr.cDevPath, "/dev/mmcblk2p1");
  pstHandle->stDevAttr.s32AutoDel = 1;
  pstHandle->stDevAttr.s32FreeSizeDelMin = 500;
  pstHandle->stDevAttr.s32FreeSizeDelMax = 1000;
  pstHandle->stDevAttr.s32FolderNum = 2;
  pstHandle->stDevAttr.pstFolderAttr = (RKADK_STR_FOLDER_ATTR *)malloc(
      sizeof(RKADK_STR_FOLDER_ATTR) * pstHandle->stDevAttr.s32FolderNum);

  if (!pstHandle->stDevAttr.pstFolderAttr) {
    RKADK_LOGE("stDevAttr.pstFolderAttr malloc failed.");
    return -1;
  }
  memset(pstHandle->stDevAttr.pstFolderAttr, 0,
         sizeof(RKADK_STR_FOLDER_ATTR) * pstHandle->stDevAttr.s32FolderNum);

  pstHandle->stDevAttr.pstFolderAttr[0].s32SortCond = SORT_FILE_NAME;
  pstHandle->stDevAttr.pstFolderAttr[0].bNumLimit = RKADK_FALSE;
  pstHandle->stDevAttr.pstFolderAttr[0].s32Limit = 50;
  sprintf(pstHandle->stDevAttr.pstFolderAttr[0].cFolderPath, "/video_front/");
  pstHandle->stDevAttr.pstFolderAttr[1].s32SortCond = SORT_FILE_NAME;
  pstHandle->stDevAttr.pstFolderAttr[1].bNumLimit = RKADK_FALSE;
  pstHandle->stDevAttr.pstFolderAttr[1].s32Limit = 50;
  sprintf(pstHandle->stDevAttr.pstFolderAttr[1].cFolderPath, "/video_back/");

  for (i = 0; i < pstHandle->stDevAttr.s32FolderNum; i++) {
    RKADK_LOGI(
        "DevAttr set:  AutoDel--%d, FreeSizeDel--%d~%d, Path--%s%s, Limit--%d",
        pstHandle->stDevAttr.s32AutoDel, pstHandle->stDevAttr.s32FreeSizeDelMin,
        pstHandle->stDevAttr.s32FreeSizeDelMax, pstHandle->stDevAttr.cMountPath,
        pstHandle->stDevAttr.pstFolderAttr[i].cFolderPath,
        pstHandle->stDevAttr.pstFolderAttr[i].s32Limit);
  }

  return 0;
}

static RKADK_S32 RKADK_STORAGE_ParameterDeinit(RKADK_STORAGE_HANDLE *pHandle) {
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  if (pHandle->stDevAttr.pstFolderAttr) {
    free(pHandle->stDevAttr.pstFolderAttr);
    pHandle->stDevAttr.pstFolderAttr = NULL;
  }

  return 0;
}

static RKADK_S32 RKADK_STORAGE_AutoDeleteInit(RKADK_STORAGE_HANDLE *pstHandle) {
  RKADK_STR_DEV_ATTR stDevAttr;

  RKADK_CHECK_POINTER(pstHandle, RKADK_FAILURE);
  stDevAttr = RKADK_STORAGE_GetParam(pstHandle);

  if (!RKADK_STORAGE_GetMountDev(stDevAttr.cMountPath,
                               pstHandle->stDevSta.cDevPath,
                               pstHandle->stDevSta.cDevType,
                               pstHandle->stDevSta.cDevAttr1)) {
      pstHandle->stDevSta.s32MountStatus = DISK_SCANNING;
  } else {
      if (0 != access(stDevAttr.cDevPath, F_OK))
          pstHandle->stDevSta.s32MountStatus = DISK_NOT_EXIST;
      else
          pstHandle->stDevSta.s32MountStatus = DISK_UNMOUNTED;
  }

  RKADK_STORAGE_ProcessStatus(pstHandle, pstHandle->stDevSta.s32MountStatus);

  if (DISK_NOT_EXIST == pstHandle->stDevSta.s32MountStatus) {
    RKADK_LOGE("Device node does not exist.");
    return -1;
  }

  if (pthread_create(&(pstHandle->stDevSta.fileScanTid), NULL,
                     RKADK_STORAGE_FileScanThread,
                     (RKADK_MW_PTR)(pstHandle))) {
    RKADK_LOGE("FileScanThread create failed.");
    return -1;
  }

  return 0;
}

static RKADK_S32 RKADK_STORAGE_AutoDeleteDeinit(RKADK_STORAGE_HANDLE *pHandle) {
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);

  pHandle->stDevSta.s32MountStatus = DISK_UNMOUNTED;

  if (pHandle->stDevSta.fileScanTid)
    if (pthread_join(pHandle->stDevSta.fileScanTid, NULL))
      RKADK_LOGE("FileScanThread join failed.");

  return 0;
}

static RKADK_S32 RKADK_STORAGE_ListenMsgInit(RKADK_STORAGE_HANDLE *pstHandle) {
  RKADK_CHECK_POINTER(pstHandle, RKADK_FAILURE);

  pstHandle->eventListenerRun = 1;

  if (RKADK_STORAGE_MsgCreate(&RKADK_STORAGE_MsgRecCb, pstHandle)) {
    RKADK_LOGE("Msg create failed.");
    return -1;
  }

  if (pthread_create(&pstHandle->eventListenerTid, NULL,
                     RKADK_STORAGE_EventListenerThread,
                     (RKADK_MW_PTR)pstHandle)) {
    RKADK_LOGE("EventListenerThread create failed.");
    return -1;
  }

  return 0;
}

RKADK_S32 RKADK_STORAGE_Init(RKADK_MW_PTR *ppHandle,
                             RKADK_STR_DEV_ATTR *pstDevAttr) {
  RKADK_STORAGE_HANDLE *pstHandle = NULL;

  RKADK_CHECK_POINTER(pstDevAttr, RKADK_FAILURE);

  if (*ppHandle) {
    RKADK_LOGE("Storage handle has been inited.");
    return -1;
  }

  pstHandle = (RKADK_STORAGE_HANDLE *)malloc(sizeof(RKADK_STORAGE_HANDLE));
  if (!pstHandle) {
    RKADK_LOGE("pstHandle malloc failed.");
    return -1;
  }
  memset(pstHandle, 0, sizeof(RKADK_STORAGE_HANDLE));
  pstHandle->pfnStatusCallback = pstDevAttr->pfnStatusCallback;

  if (RKADK_STORAGE_ParameterInit(pstHandle, pstDevAttr)) {
    RKADK_LOGE("Parameter init failed.");
    goto failed;
  }

  if (RKADK_STORAGE_AutoDeleteInit(pstHandle))
    RKADK_LOGE("AutoDelete init failed.");

  if (RKADK_STORAGE_ListenMsgInit(pstHandle)) {
    RKADK_LOGE("Listener and Msg init failed.");
    goto failed;
  }

  *ppHandle = (RKADK_MW_PTR)pstHandle;
  return 0;

failed:
  if (pstHandle)
    free(pstHandle);

  return -1;
}

RKADK_S32 RKADK_STORAGE_Deinit(RKADK_MW_PTR pHandle) {
  RKADK_STORAGE_HANDLE *pstHandle = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;
  pstHandle->eventListenerRun = 0;
  pstHandle->stDevSta.s32FsckQuit = 1;

  if (pstHandle->eventListenerTid)
    if (pthread_join(pstHandle->eventListenerTid, NULL))
      RKADK_LOGE("EventListenerThread join failed.");

  if (RKADK_STORAGE_MsgDestroy(pstHandle))
    RKADK_LOGE("Msg destroy failed.");

  if (RKADK_STORAGE_AutoDeleteDeinit(pstHandle))
    RKADK_LOGE("AutoDelete deinit failed.");

  if (RKADK_STORAGE_ParameterDeinit(pstHandle))
    RKADK_LOGE("Paramete deinit failed.");

  free(pstHandle);
  pstHandle = NULL;

  return 0;
}

RKADK_MOUNT_STATUS RKADK_STORAGE_GetMountStatus(RKADK_MW_PTR pHandle) {
  RKADK_STORAGE_HANDLE *pstHandle;

  RKADK_CHECK_POINTER(pHandle, DISK_MOUNT_BUTT);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;
  return pstHandle->stDevSta.s32MountStatus;
}

RKADK_S32 RKADK_STORAGE_GetCapacity(RKADK_MW_PTR *ppHandle,
                                      RKADK_S32 *totalSize,
                                      RKADK_S32 *freeSize) {
  RKADK_STORAGE_HANDLE *pstHandle = NULL;
  RKADK_STR_DEV_ATTR stDevAttr;

  RKADK_CHECK_POINTER(ppHandle, RKADK_FAILURE);
  RKADK_CHECK_POINTER(*ppHandle, RKADK_FAILURE);
  RKADK_CHECK_POINTER(totalSize, RKADK_FAILURE);
  RKADK_CHECK_POINTER(freeSize, RKADK_FAILURE);
  pstHandle = (RKADK_STORAGE_HANDLE *)*ppHandle;
  stDevAttr = RKADK_STORAGE_GetParam(pstHandle);

  if (pstHandle->stDevSta.s32MountStatus == DISK_MOUNTED) {
    RKADK_STORAGE_GetDiskSize(stDevAttr.cMountPath,
                              &pstHandle->stDevSta.s32TotalSize,
                              &pstHandle->stDevSta.s32FreeSize);
  } else {
    pstHandle->stDevSta.s32TotalSize = 0;
    pstHandle->stDevSta.s32FreeSize = 0;
  }
  *totalSize = pstHandle->stDevSta.s32TotalSize;
  *freeSize = pstHandle->stDevSta.s32FreeSize;

  *ppHandle = (RKADK_MW_PTR)pstHandle;
  return 0;
}

RKADK_S32 RKADK_STORAGE_GetFileList(RKADK_FILE_LIST *list, RKADK_MW_PTR pHandle,
                                    RKADK_SORT_TYPE sort) {
  RKADK_S32 i, j;
  RKADK_STORAGE_HANDLE *pstHandle = NULL;
  RKADK_S32 s32FileNum = 0;
  struct RKADK_STR_FILE *tmp = NULL;

  RKADK_CHECK_POINTER(list, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;

  if (list->file) {
    RKADK_LOGE("list already exists");
    return -1;
  }

  for (i = 0; i < pstHandle->stDevSta.s32FolderNum; i++) {
    if (!strcmp(list->path, pstHandle->stDevSta.pstFolder[i].cpath))
      break;
  }

  if (i == pstHandle->stDevSta.s32FolderNum) {
    RKADK_LOGE("No folder found. Please check the folder path.\n");
    return -1;
  }

  pthread_mutex_lock(&pstHandle->stDevSta.pstFolder[i].mutex);

  s32FileNum = pstHandle->stDevSta.pstFolder[i].s32FileNum;
  list->file =
      (RKADK_FILE_INFO *)malloc(sizeof(RKADK_FILE_INFO) * s32FileNum);
  if (!list->file) {
    RKADK_LOGE("list->file malloc failed.");
    pthread_mutex_unlock(&pstHandle->stDevSta.pstFolder[i].mutex);
    return -1;
  }
  memset(list->file, 0, sizeof(RKADK_FILE_INFO) * s32FileNum);

  list->s32FileNum = 0;
  if (sort == LIST_DESCENDING) {
    tmp = pstHandle->stDevSta.pstFolder[i].pstFileListFirst;
    for (j = 0; j < s32FileNum && tmp != NULL; j++, tmp = tmp->next) {
      if (pstHandle->pfnFileFilterCB)
        if (!pstHandle->pfnFileFilterCB(tmp->filename))
          continue;

      snprintf(list->file[list->s32FileNum].filename, sizeof(list->file[list->s32FileNum].filename), "%s", tmp->filename);
      list->file[list->s32FileNum].stSize = tmp->stSize;
      list->file[list->s32FileNum].stTime = tmp->stTime;
      list->s32FileNum++;
    }
  } else {
    tmp = pstHandle->stDevSta.pstFolder[i].pstFileListLast;
    for (j = 0; j < s32FileNum && tmp != NULL; j++, tmp = tmp->prev) {
      if (pstHandle->pfnFileFilterCB)
        if (!pstHandle->pfnFileFilterCB(tmp->filename))
          continue;

      snprintf(list->file[list->s32FileNum].filename, sizeof(list->file[list->s32FileNum].filename), "%s", tmp->filename);
      list->file[list->s32FileNum].stSize = tmp->stSize;
      list->file[list->s32FileNum].stTime = tmp->stTime;
      list->s32FileNum++;
    }
  }

  pthread_mutex_unlock(&pstHandle->stDevSta.pstFolder[i].mutex);
  return 0;
}

RKADK_S32 RKADK_STORAGE_FreeFileList(RKADK_FILE_LIST *list) {
  if (list->file) {
    free(list->file);
    list->file = NULL;
  }

  return 0;
}

RKADK_S32 RKADK_STORAGE_GetFileNum(RKADK_CHAR *fileListPath,
                                   RKADK_MW_PTR pHandle) {
  RKADK_S32 i;
  RKADK_STORAGE_HANDLE *pstHandle = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;

  for (i = 0; i < pstHandle->stDevSta.s32FolderNum; i++) {
    if (!strcmp(fileListPath, pstHandle->stDevSta.pstFolder[i].cpath))
      break;
  }

  if (i == pstHandle->stDevSta.s32FolderNum)
    return 0;

  return pstHandle->stDevSta.pstFolder[i].s32FileNum;
}

RKADK_CHAR *RKADK_STORAGE_GetDevPath(RKADK_MW_PTR pHandle) {
  RKADK_STORAGE_HANDLE *pstHandle = NULL;

  RKADK_CHECK_POINTER(pHandle, NULL);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;

  return pstHandle->stDevSta.cDevPath;
}

RKADK_S32 RKADK_STORAGE_Format(RKADK_MW_PTR pHandle, RKADK_CHAR* cFormat)
{
  RKADK_S32 err = 0;
  RKADK_CHAR *pDevPath = NULL;
  RKADK_STORAGE_HANDLE *pstHandle = NULL;
  RKADK_CHAR mountPath[RKADK_MAX_FILE_PATH_LEN];

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;

  if (pstHandle->stDevSta.s32MountStatus != DISK_UNMOUNTED) {
    RKADK_S32 ret = 0;
    sync();

    if (pstHandle->stDevAttr.cDevPath[0]) {
      pDevPath = pstHandle->stDevAttr.cDevPath;
    } else if (pstHandle->stDevSta.cDevPath[0]) {
      pDevPath = pstHandle->stDevSta.cDevPath;
    } else {
      RKADK_LOGE("cDevPath is null");
      return -1;
    }

    if (pstHandle->stDevSta.s32MountStatus != DISK_NOT_FORMATTED)
      RKADK_STORAGE_DevRemove(pDevPath, pstHandle);

    ret = RKADK_STORAGE_GetMountPath(pDevPath, mountPath, RKADK_MAX_FILE_PATH_LEN);
    if (!ret)
        umount2(mountPath, MNT_FORCE);

    ret = rkfsmk_format_ex(pDevPath, pstHandle->stDevAttr.cVolume, pstHandle->stDevAttr.cFormatId);
    if (!ret)
      err = -1;
    ret = mount(pDevPath, pstHandle->stDevAttr.cMountPath, cFormat, MS_NOATIME | MS_NOSUID, NULL);
    if (ret == 0)
      RKADK_STORAGE_DevAdd(pDevPath, pstHandle);
    else
      err = -1;
  }

  RKADK_LOGD("Format %s[%d]", pDevPath, err);
  return err;
}

RKADK_S32 RKADK_STORAGE_RegisterFileFilterCB(RKADK_MW_PTR pHandle,
                                      RKADK_FILE_FILTER_CALLBACK_FN pfnFileFilterCB) {
  RKADK_STORAGE_HANDLE *pstHandle = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;
  pstHandle->pfnFileFilterCB = pfnFileFilterCB;
  return 0;
}

RKADK_S32 RKADK_STORAGE_UnRegisterFileFilterCB(RKADK_MW_PTR pHandle) {
  RKADK_STORAGE_HANDLE *pstHandle = NULL;

  RKADK_CHECK_POINTER(pHandle, RKADK_FAILURE);
  pstHandle = (RKADK_STORAGE_HANDLE *)pHandle;
  pstHandle->pfnFileFilterCB = NULL;
  return 0;
}
