// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
#ifndef __RKIPC_STORAGE_H__
#define __RKIPC_STORAGE_H__

#include <dirent.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <sys/mount.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/vfs.h>

//#include "cJSON.h"
#include "common.h"
#include "rkmuxer.h"

#define RKIPC_MAX_FORMAT_ID_LEN 8
#define RKIPC_MAX_VOLUME_LEN 11
#define RKIPC_MAX_FILE_PATH_LEN 256

#define JSON_KEY_FOLDER_NAME "FolderName"
#define JSON_KEY_FILE_NUMBER "FileNumber"
#define JSON_KEY_TOTAL_SIZE "total_size"
#define JSON_KEY_TOTAL_SPACE "total_space"
#define JSON_KEY_FILE_ARRAY "FileArray"
#define JSON_KEY_FILE_NAME "FileName"
#define JSON_KEY_MODIFY_TIME "ModifyTime"
#define JSON_KEY_FILE_SIZE "FileSize"
#define JSON_KEY_FILE_SPACE "FileSpace"

#define RKIPC_STORAGE_FAIL (-1)
#define MAX_TYPE_NMSG_LEN 32
#define MAX_ATTR_LEN 256
#define MAX_STRLINE_LEN 1024

/* Pointer Check */
#define RKIPC_CHECK_POINTER(p, errcode)                                                            \
	do {                                                                                           \
		if (!(p)) {                                                                                \
			LOG_DEBUG("pointer[%s] is NULL", #p);                                                  \
			return errcode;                                                                        \
		}                                                                                          \
	} while (0)

typedef int (*rkipc_reg_msg_cb)(void *, int, void *, int, void *);

typedef enum {
	DISK_UNMOUNTED = 0,
	DISK_NOT_FORMATTED,
	DISK_FORMAT_ERR,
	DISK_SCANNING,
	DISK_MOUNTED,
	DISK_MOUNT_BUTT,
} rkipc_mount_status;

typedef enum {
	LIST_ASCENDING = 0,
	LIST_DESCENDING,
	LIST_BUTT,
} rkipc_sort_type;

typedef enum {
	SORT_MODIFY_TIME = 0,
	SORT_FILE_NAME,
	SORT_BUTT,
} rkipc_sort_condition;

typedef enum {
	MSG_DEV_ADD = 1,
	MSG_DEV_REMOVE = 2,
	MSG_DEV_CHANGED = 3,
} rkipc_enum_msg;

typedef struct {
	char folder_path[RKIPC_MAX_FILE_PATH_LEN];
	rkipc_sort_condition sort_cond;
	bool num_limit;
	int limit;
} rkipc_str_folder_attr;

typedef struct {
	char dev_path[RKIPC_MAX_FILE_PATH_LEN];
	char mount_path[RKIPC_MAX_FILE_PATH_LEN];
	int free_size_del_min;
	int free_size_del_max;
	int auto_delete;
	int folder_num;
	char format_id[RKIPC_MAX_FORMAT_ID_LEN];
	char volume[RKIPC_MAX_VOLUME_LEN];
	int check_format_id;
	rkipc_str_folder_attr *folder_attr;
} rkipc_str_dev_attr;

typedef struct {
	char filename[RKIPC_MAX_FILE_PATH_LEN];
	long size;
	long long time;
	void *thumb;
} rkipc_fileinfo;

typedef struct {
	char path[RKIPC_MAX_FILE_PATH_LEN];
	int file_num;
	rkipc_fileinfo *file;
} rkipc_filelist;

typedef struct {
	int list_num;
	rkipc_filelist *list;
} rkipc_filelist_array;

typedef struct _rkipc_str_file {
	struct _rkipc_str_file *next;
	char filename[RKIPC_MAX_FILE_PATH_LEN];
	time_t time;
	off_t size;
	off_t space;
	mode_t mode;
} rkipc_str_file;

typedef struct {
	char cpath[RKIPC_MAX_FILE_PATH_LEN * 2];
	rkipc_sort_condition sort_cond;
	int wd;
	int file_num;
	off_t total_size;
	off_t total_space;
	pthread_mutex_t mutex;
	rkipc_str_file *file_list_first;
	rkipc_str_file *file_list_last;
} rkipc_str_folder;

typedef struct {
	char dev_path[RKIPC_MAX_FILE_PATH_LEN];
	char dev_type[MAX_TYPE_NMSG_LEN];
	char dev_attr_1[MAX_ATTR_LEN];
	rkipc_mount_status mount_status;
	pthread_t file_scan_tid;
	int folder_num;
	int total_size;
	int free_size;
	int fsck_quit;
	rkipc_str_folder *folder;
} rkipc_str_dev_sta;

typedef struct _rkipc_tmsg_element {
	struct _rkipc_tmsg_element *next;
	int msg;
	char *data;
	int data_len;
} rkipc_tmsg_element;

typedef struct {
	rkipc_tmsg_element *first;
	rkipc_tmsg_element *last;
	int num;
	int quit;
	pthread_mutex_t mutex;
	pthread_cond_t not_empty;
	rkipc_reg_msg_cb rec_msg_cb;
	pthread_t rec_tid;
	void *handle_path;
} rkipc_tmsg_buffer;

typedef struct {
	rkipc_tmsg_buffer msg_hd;
	pthread_t event_listener_tid;
	int event_listener_run;
	rkipc_str_dev_sta dev_sta;
	rkipc_str_dev_attr dev_attr;
} rkipc_storage_handle;

typedef struct rk_storage_muxer_struct_ {
	int id;
	char file_name[256 * 2];
	char record_path[256];
	const char *file_format;
	int file_duration;
	int g_record_run_;
	void *g_storage_signal;
	pthread_t record_thread_id;
	VideoParam g_video_param;
	AudioParam g_audio_param;
} rk_storage_muxer_struct;

int rk_storage_init();
int rk_storage_deinit();
int rk_storage_write_video_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time, int key_frame);
int rk_storage_write_audio_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time);
int rk_storage_record_start();
int rk_storage_record_stop();
int rk_storage_record_statue_get(int *value);

// int rkipc_storage_quota_get(int id, char **value);    // TODO, current only sd card
int rkipc_storage_quota_set(int id, char *value); // TODO
// int rkipc_storage_hdd_list_get(int id, char **value); // TODO, current only sd card
// int rkipc_storage_snap_plan_get(int id, char **value);
int rkipc_storage_snap_plan_set(int id, char *value); // TODO
int rkipc_storage_current_path_get(char **value);
int rkipc_storage_current_path_set(char *value);
int rkipc_storage_search(char *file_info); // TODO
int rkipc_storage_dev_mount_status_get();
// num:The number of files to delete
// namel_ist:The list of files to be deleted, the number of lists matches the num
// char *rkipc_response_delete(int id, int num, char *name_list);

#ifdef __cplusplus
}
#endif
#endif