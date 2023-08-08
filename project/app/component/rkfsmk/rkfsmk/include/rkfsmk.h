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

#ifndef __RKFSMK_H
#define __RKFSMK_H

#ifdef __cplusplus
extern "C" {
#endif

struct folder_para {
  char *path;
  void *userdata;
  void (*cb)(void *, char *, int, struct stat *);
};

struct reg_para {
  struct folder_para *folder;
  int folder_num;
  int *quit;
  char format_id[8];
  int check_format_id;
};

typedef enum {
  RKFSCK_ID_ERR = -2,
  RKFSCK_FAIL = -1,
  RKFSCK_SUS = 0,
} RKFSCK_RET_TYPE;

typedef enum {
  REPA_FAIL = -1,
  REPA_NOSP = 0,
  REPA_SUCC = 1,
  REPA_NONE = 2,
} REPA_RET_TYPE;

int repair_mp4(char *file);

int rkfsmk_fat_check(char *dev, struct reg_para *para);

/* ret: 0 success,-1 no devvice, -4 devvice no umount*/
int rkfsmk_format(char *device_name, char *volume_name);
int rkfsmk_format_ex(char *device_name, char *volume_name, char format_id[8]);

int rkfsmk_start(void *handle);

/* ret: 0 success,-1 no devvice, -4 devvice no umount*/
int rkfsmk_create(void **handle, char *device_name, char *volume_name,
                  unsigned int align_size);

int rkfsmk_set_format_id(void *handle, char format_id[8]);

void rkfsmk_add_dir(void *handle, char *path, int hidden, int size);
int rkfsmk_add_file(void *handle, char *path, char *filename, int hidden,
                    int size);
void rkfsmk_destroy(void *handle);
unsigned long long rkfsmk_disk_size_get(void *handle);

int kernel_get_file_size(char *filename, off_t *size, off_t *space);
int kernel_pre_created_file(char *filename, off_t size);
int kernel_set_alignsize(int file_fd, unsigned int alignsize);
int kernel_chk_format(char *path);

#ifdef __cplusplus
}
#endif

#endif