// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "storage.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "storage.c"

#define STORAGE_NUM 3

static int record_flag[STORAGE_NUM] = {-1};
static void *g_sd_phandle = NULL;
static void *g_file_scan_signal = NULL;
static rkipc_str_dev_attr g_sd_dev_attr;
static pthread_mutex_t g_rkmuxer_mutex = PTHREAD_MUTEX_INITIALIZER;
static int g_storage_record_flag[3]; // only for recording thread
static int rk_storage_muxer_init_by_id(int id);
static int rk_storage_muxer_deinit_by_id(int id);

static rk_storage_muxer_struct rk_storage_muxer_group[STORAGE_NUM];

static rkipc_str_dev_attr rkipc_storage_get_param(rkipc_storage_handle *pHandle) {
	return pHandle->dev_attr;
}

RKIPC_MAYBE_UNUSED static rkipc_str_dev_attr rkipc_storage_get_dev_attr(void *pHandle) {
	return rkipc_storage_get_param((rkipc_storage_handle *)pHandle);
}

static int rkipc_storage_get_mount_dev(const char *path, char *dev, char *type, char *attributes) {
	FILE *fp;
	char strLine[MAX_STRLINE_LEN];
	char *tmp;

	RKIPC_CHECK_POINTER(dev, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(path, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(type, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(attributes, RKIPC_STORAGE_FAIL);

	if ((fp = fopen("/proc/mounts", "r")) == NULL) {
		LOG_ERROR("Open file error!");
		return -1;
	}

	while (!feof(fp)) {
		fgets(strLine, MAX_STRLINE_LEN, fp);
		tmp = strstr(strLine, path);

		if (tmp) {
			char MountPath[RKIPC_MAX_FILE_PATH_LEN];
			sscanf(strLine, "%s %s %s %s", dev, MountPath, type, attributes);

			fclose(fp);
			return 0;
		}
	}

	fclose(fp);
	return -1;
}

int rkipc_storage_set_dev_attr(rkipc_str_dev_attr *pstDevAttr) {
	const char *folder_name = NULL;
	const char *mount_path = NULL;
	char dev_path[64];
	char type[64];
	char attributes[128];
	char entry[128] = {'\0'};
	memset(dev_path, 0, sizeof(dev_path));
	memset(type, 0, sizeof(type));
	memset(attributes, 0, sizeof(attributes));
	RKIPC_CHECK_POINTER(pstDevAttr, RKIPC_STORAGE_FAIL);
	LOG_DEBUG("The DevAttr will be user-defined.\n");

	memset(pstDevAttr, 0, sizeof(rkipc_str_dev_attr));
	mount_path = rk_param_get_string("storage:mount_path", "/userdata");
	sprintf(pstDevAttr->mount_path, mount_path);
	rkipc_storage_get_mount_dev(mount_path, dev_path, type, attributes);
	if (strlen(dev_path) == 0) {
		for (int i = 0; i < STORAGE_NUM - 1; i++) {
			record_flag[i] = 0;
		}
		LOG_ERROR("unrecognized dev_path,stop record!\n");
	} else {
		sprintf(pstDevAttr->dev_path, dev_path);
		for (int i = 0; i < STORAGE_NUM - 1; i++) {
			snprintf(entry, 127, "storage.%d:enable", i);
			if (rk_param_get_int(entry, 0) == 0)
				record_flag[i] = 0;
			else
				record_flag[i] = 1;
		}
	}
	LOG_INFO("mount path is %s, dev_path is %s\n", mount_path, dev_path);

	pstDevAttr->auto_delete = 1;
	pstDevAttr->free_size_del_min = rk_param_get_int("storage:free_size_del_min", 200);
	pstDevAttr->free_size_del_max = rk_param_get_int("storage:free_size_del_max", 500);
	pstDevAttr->folder_num = STORAGE_NUM;
	pstDevAttr->folder_attr =
	    (rkipc_str_folder_attr *)malloc(sizeof(rkipc_str_folder_attr) * pstDevAttr->folder_num);

	if (!pstDevAttr->folder_attr) {
		LOG_ERROR("pstDevAttr->folder_attr malloc failed.\n");
		return -1;
	}
	memset(pstDevAttr->folder_attr, 0, sizeof(rkipc_str_folder_attr) * pstDevAttr->folder_num);

	folder_name = rk_param_get_string("storage.0:folder_name", "video0");
	pstDevAttr->folder_attr[0].sort_cond = SORT_FILE_NAME;
	sprintf(pstDevAttr->folder_attr[0].folder_path, folder_name);

	folder_name = rk_param_get_string("storage.1:folder_name", "video1");
	pstDevAttr->folder_attr[1].sort_cond = SORT_FILE_NAME;
	sprintf(pstDevAttr->folder_attr[1].folder_path, folder_name);

	folder_name = rk_param_get_string("storage.2:folder_name", "video2");
	pstDevAttr->folder_attr[2].sort_cond = SORT_FILE_NAME;
	sprintf(pstDevAttr->folder_attr[2].folder_path, folder_name);

	if (rk_param_get_int("storage:num_limit_enable", 0)) {
		pstDevAttr->folder_attr[0].num_limit = true;
		pstDevAttr->folder_attr[1].num_limit = true;
		pstDevAttr->folder_attr[2].num_limit = true;
		pstDevAttr->folder_attr[0].limit = rk_param_get_int("storage.0:file_max_num", 300);
		pstDevAttr->folder_attr[1].limit = rk_param_get_int("storage.1:file_max_num", 300);
		pstDevAttr->folder_attr[2].limit = rk_param_get_int("storage.2:file_max_num", 300);
	} else {
		pstDevAttr->folder_attr[0].num_limit = false;
		pstDevAttr->folder_attr[1].num_limit = false;
		pstDevAttr->folder_attr[2].num_limit = false;
		pstDevAttr->folder_attr[0].limit = rk_param_get_int("storage.0:video_quota", 30);
		pstDevAttr->folder_attr[1].limit = rk_param_get_int("storage.1:video_quota", 30);
		pstDevAttr->folder_attr[2].limit = rk_param_get_int("storage.2:video_quota", 30);
	}

	return 0;
}

int rkipc_storage_free_dev_attr(rkipc_str_dev_attr devAttr) {
	if (devAttr.folder_attr) {
		free(devAttr.folder_attr);
		devAttr.folder_attr = NULL;
	}

	return 0;
}

static int rkipc_storage_create_folder(char *folder) {
	int i, len;

	RKIPC_CHECK_POINTER(folder, RKIPC_STORAGE_FAIL);
	len = strlen(folder);
	if (!len) {
		LOG_ERROR("Invalid path.\n");
		return -1;
	}
	for (i = 1; i < len; i++) {
		if (folder[i] != '/')
			continue;
		folder[i] = 0;
		if (access(folder, R_OK)) {
			if (mkdir(folder, 0755)) {
				LOG_ERROR("mkdir error\n");
				return -1;
			}
		}
		folder[i] = '/';
	}
	if (access(folder, R_OK)) {
		if (mkdir(folder, 0755)) {
			LOG_ERROR("mkdir error\n");
			return -1;
		}
	}
	LOG_DEBUG("Create %s finished\n", folder);

	return 0;
}

static int rkipc_storage_read_timeout(int fd, unsigned int u32WaitMs) {
	int ret = 0;

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

static int rkipc_storage_get_disk_size(char *path, int *total_size, int *free_size) {
	struct statfs diskInfo;

	RKIPC_CHECK_POINTER(path, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(total_size, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(free_size, RKIPC_STORAGE_FAIL);

	if (statfs(path, &diskInfo)) {
		LOG_ERROR("statfs[%s] failed", path);
		return -1;
	}

	*total_size = (diskInfo.f_bsize * diskInfo.f_blocks) >> 10;
	*free_size = (diskInfo.f_bfree * diskInfo.f_bsize) >> 10;
	return 0;
}

static int rkipc_storage_get_mount_path(char *dev, char *path, int path_len) {
	int ret = -1;
	FILE *fp;
	char strLine[MAX_STRLINE_LEN];
	char *tmp;

	RKIPC_CHECK_POINTER(dev, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(path, RKIPC_STORAGE_FAIL);

	if ((fp = fopen("/proc/mounts", "r")) == NULL) {
		LOG_ERROR("Open file error!\n");
		return -1;
	}

	memset(path, 0, path_len);
	while (!feof(fp)) {
		fgets(strLine, MAX_STRLINE_LEN, fp);
		tmp = strstr(strLine, dev);

		if (tmp) {
			int len;
			char *s = strstr(strLine, " ") + 1;
			char *e = strstr(s, " ");
			len = e - s;

			if ((len > 0) && (len < path_len)) {
				memcpy(path, s, len);
				ret = 0;
			} else {
				LOG_ERROR("len[%d], path_len[%d]", len, path_len);
				ret = -2;
			}

			goto exit;
		}
	}

exit:
	fclose(fp);
	return ret;
}

RKIPC_MAYBE_UNUSED static bool rkipc_storage_file_compare(rkipc_str_file *existingFile,
                                                          rkipc_str_file *newFile,
                                                          rkipc_sort_condition cond) {
	bool ret = false;

	switch (cond) {
	case SORT_MODIFY_TIME: {
		ret = (newFile->time <= existingFile->time);
		break;
	}
	case SORT_FILE_NAME: {
		ret = (strcmp(newFile->filename, existingFile->filename) <= 0);
		break;
	}
	case SORT_BUTT: {
		ret = false;
		LOG_ERROR("Invalid condition.");
		break;
	}
	}

	return ret;
}

static int rkipc_storage_file_list_check(rkipc_str_folder *folder, char *filename,
                                         struct stat *statbuf) {
	int ret = 0;
	rkipc_str_file *tmp = NULL;

	RKIPC_CHECK_POINTER(folder, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(filename, RKIPC_STORAGE_FAIL);

	pthread_mutex_lock(&folder->mutex);

	if (folder->file_list_first) {
		tmp = folder->file_list_first;

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

static int rkipc_storage_file_list_add(rkipc_str_folder *folder, char *filename,
                                       struct stat *statbuf) {
	rkipc_str_file *tmp = NULL;
	rkipc_str_file *tmp_1 = NULL;

	RKIPC_CHECK_POINTER(folder, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(filename, RKIPC_STORAGE_FAIL);
	// LOG_DEBUG("folder/filename is %s/%s\n", folder, filename);

	pthread_mutex_lock(&folder->mutex);
	tmp_1 = (rkipc_str_file *)malloc(sizeof(rkipc_str_file));
	if (!tmp_1) {
		LOG_ERROR("tmp malloc failed.");
		pthread_mutex_unlock(&folder->mutex);
		return -1;
	}
	sprintf(tmp_1->filename, "%s", filename);
	tmp_1->size = statbuf->st_size;
	tmp_1->space = statbuf->st_blocks << 9;
	tmp_1->time = statbuf->st_mtime;
	tmp_1->next = NULL;

	if (folder->file_list_first) {
		tmp = folder->file_list_first;
		if (tmp_1->time >= tmp->time) {
			tmp_1->next = tmp;
			folder->file_list_first = tmp_1;
		} else {
			while (tmp->next) {
				if (tmp_1->time >= tmp->next->time) {
					tmp_1->next = tmp->next;
					tmp->next = tmp_1;
					break;
				}
				tmp = tmp->next;
			}
			if (tmp->next == NULL) {
				tmp->next = tmp_1;
				folder->file_list_last = tmp_1;
			}
		}
	} else {
		folder->file_list_first = tmp_1;
		folder->file_list_last = tmp_1;
	}

	folder->total_size += tmp_1->size;
	folder->total_space += tmp_1->space;
	folder->file_num++;

	pthread_mutex_unlock(&folder->mutex);
	return 0;
}

static int rkipc_storage_file_list_del(rkipc_str_folder *folder, char *filename) {
	int file_num = 0;
	off_t total_size = 0;
	off_t total_space = 0;
	rkipc_str_file *next = NULL;

	RKIPC_CHECK_POINTER(folder, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(filename, RKIPC_STORAGE_FAIL);

	pthread_mutex_lock(&folder->mutex);

again:
	if (folder->file_list_first) {
		rkipc_str_file *tmp = folder->file_list_first;
		if (!strcmp(tmp->filename, filename)) {
			folder->file_list_first = folder->file_list_first->next;
			free(tmp);
			tmp = folder->file_list_first;
			if (folder->file_list_first == NULL) {
				folder->file_list_last = NULL;
			}
			goto again;
		}

		while (tmp) {
			next = tmp->next;
			total_size += tmp->size;
			total_space += tmp->space;
			file_num++;
			if (next == NULL) {
				folder->file_list_last = tmp;
				break;
			}
			if (!strcmp(next->filename, filename)) {
				tmp->next = next->next;
				free(next);
				next = tmp->next;
				if (tmp->next == NULL)
					folder->file_list_last = tmp;
			}
			tmp = next;
		}
	}
	folder->file_num = file_num;
	folder->total_size = total_size;
	folder->total_space = total_space;

	pthread_mutex_unlock(&folder->mutex);
	return 0;
}

static void *rkipc_storage_file_monitor_thread(void *arg) {
	rkipc_storage_handle *pHandle = (rkipc_storage_handle *)arg;
	int fd;
	int len;
	int nread;
	char buf[BUFSIZ];
	struct inotify_event *event;
	int j;
	char d_name[RKIPC_MAX_FILE_PATH_LEN * 3];
	struct stat statbuf;

	if (!pHandle) {
		LOG_ERROR("invalid pHandle");
		return NULL;
	}
	LOG_DEBUG("rkipc_storage_file_monitor_thread\n");
	prctl(PR_SET_NAME, "rkipc_storage_file_monitor_thread", 0, 0, 0);
	fd = inotify_init();
	if (fd < 0) {
		LOG_ERROR("inotify_init failed\n");
		return NULL;
	}

	for (j = 0; j < pHandle->dev_sta.folder_num; j++) {
		pHandle->dev_sta.folder[j].wd = inotify_add_watch(
		    fd, pHandle->dev_sta.folder[j].cpath,
		    IN_CREATE | IN_MOVED_TO | IN_DELETE | IN_MOVED_FROM | IN_CLOSE_WRITE | IN_UNMOUNT);
	}

	memset(buf, 0, BUFSIZ);
	LOG_INFO("pHandle->dev_sta.mount_status is %d\n", pHandle->dev_sta.mount_status);
	while (pHandle->dev_sta.mount_status == DISK_MOUNTED) {
		if (rkipc_storage_read_timeout(fd, 10))
			continue;

		len = read(fd, buf, BUFSIZ - 1);
		nread = 0;
		while (len > 0) {
			event = (struct inotify_event *)&buf[nread];
			if (event->mask & IN_UNMOUNT)
				pHandle->dev_sta.mount_status = DISK_UNMOUNTED;

			if (event->len > 0) {
				for (j = 0; j < pHandle->dev_sta.folder_num; j++) {
					if (event->wd == pHandle->dev_sta.folder[j].wd) {
						if (event->mask & IN_MOVED_TO) {
							sprintf(d_name, "%s/%s", pHandle->dev_sta.folder[j].cpath, event->name);
							if (lstat(d_name, &statbuf)) {
								LOG_ERROR("lstat[%s](IN_MOVED_TO) failed", d_name);
							} else {
								if ((rkipc_storage_file_list_check(&pHandle->dev_sta.folder[j],
								                                   event->name, &statbuf) == 0) &&
								    rkipc_storage_file_list_add(&pHandle->dev_sta.folder[j],
								                                event->name, &statbuf))
									LOG_ERROR("FileListAdd failed");
							}
						}

						if ((event->mask & IN_DELETE) || (event->mask & IN_MOVED_FROM))
							if (rkipc_storage_file_list_del(&pHandle->dev_sta.folder[j],
							                                event->name))
								LOG_ERROR("FileListDel failed");

						if (event->mask & IN_CLOSE_WRITE) {
							sprintf(d_name, "%s/%s", pHandle->dev_sta.folder[j].cpath, event->name);
							if (lstat(d_name, &statbuf)) {
								LOG_ERROR("lstat[%s](IN_CLOSE_WRITE) failed", d_name);
							} else {
								if (statbuf.st_size == 0) {
									if (remove(d_name))
										LOG_ERROR("Delete %s file error.", d_name);
								} else if ((rkipc_storage_file_list_check(
								                &pHandle->dev_sta.folder[j], event->name,
								                &statbuf) == 0) &&
								           rkipc_storage_file_list_add(&pHandle->dev_sta.folder[j],
								                                       event->name, &statbuf)) {
									LOG_ERROR("FileListAdd failed");
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

	LOG_DEBUG("Exit!");
	close(fd);
	return NULL;
}

static void cb(void *userdata, char *filename, int dir, struct stat *statbuf) {
	if (dir == 0) {
		rkipc_str_folder *folder = (rkipc_str_folder *)userdata;
		rkipc_storage_file_list_add(folder, filename, statbuf);
	}
}

int rkipc_storage_read_file_list(rkipc_str_folder *folder, rkipc_str_folder_attr *folder_attr) {
	DIR *dir;
	struct dirent *ptr;
	struct stat statbuf;
	char filename[RKIPC_MAX_FILE_PATH_LEN];
	char d_name[RKIPC_MAX_FILE_PATH_LEN * 3];
	int file_count = 0;

	if ((dir = opendir(folder->cpath)) == NULL) {
		LOG_ERROR("Open dir error\n");
		return -1;
	}

	// use clear list instead of file_list_check every time before file_list_add,
	// and the list is empty when first init, this method takes less time
	while (folder->file_list_last) {
		snprintf(filename, RKIPC_MAX_FILE_PATH_LEN, "%s", folder->file_list_last->filename);
		rkipc_storage_file_list_del(folder, filename);
	}

	while ((ptr = readdir(dir)) != NULL) {
		// judge file conut, and maybe have some jpeg files not delete, so add 300 to limit
		if (file_count++ > (folder_attr->limit + 300)) {
			LOG_ERROR("%s file num more than %d"
			          "\n\n -----Please confirm whether formatting is required-----\n\n",
			          folder->cpath, folder_attr->limit + 300);
			break;
		}
		if (strcmp(ptr->d_name, ".") == 0 ||
		    strcmp(ptr->d_name, "..") == 0) /// current dir OR parrent dir
			continue;
		if (ptr->d_type == 8) { // file
			sprintf(d_name, "%s/%s", folder->cpath, ptr->d_name);
			// LOG_DEBUG("d_name:%s\n", d_name);
			if (lstat(d_name, &statbuf)) {
				LOG_ERROR("lstat[%s](IN_MOVED_TO) failed\n", d_name);
			} else {
				if (rkipc_storage_file_list_add(folder, ptr->d_name, &statbuf))
					LOG_ERROR("FileListAdd failed\n");
			}
		}
	}
	closedir(dir);

	return 0;
}

static void *rkipc_storage_file_scan_thread(void *arg) {
	rkipc_storage_handle *pHandle = (rkipc_storage_handle *)arg;
	int i, limit;
	off_t total_space = 0;
	pthread_t fileMonitorTid = 0;
	rkipc_str_dev_attr devAttr;
	char file[3 * RKIPC_MAX_FILE_PATH_LEN];

	if (!pHandle) {
		LOG_ERROR("invalid pHandle\n");
		return NULL;
	}
	devAttr = rkipc_storage_get_param(pHandle);
	prctl(PR_SET_NAME, "file_scan_thread", 0, 0, 0);
	LOG_INFO("%s, %s, %s, %s\n", devAttr.mount_path, pHandle->dev_sta.dev_path,
	         pHandle->dev_sta.dev_type, pHandle->dev_sta.dev_attr_1);

	if (pHandle->dev_sta.mount_status != DISK_UNMOUNTED) {
		LOG_DEBUG("devAttr.folder_num = %d\n", devAttr.folder_num);
		pHandle->dev_sta.folder_num = devAttr.folder_num;
		pHandle->dev_sta.folder =
		    (rkipc_str_folder *)malloc(sizeof(rkipc_str_folder) * devAttr.folder_num);

		if (!pHandle->dev_sta.folder) {
			LOG_ERROR("pHandle->dev_sta.folder malloc failed.\n");
			return NULL;
		}
		memset(pHandle->dev_sta.folder, 0, sizeof(rkipc_str_folder) * devAttr.folder_num);
		for (i = 0; i < pHandle->dev_sta.folder_num; i++) {
			sprintf(pHandle->dev_sta.folder[i].cpath, "%s/%s", devAttr.mount_path,
			        devAttr.folder_attr[i].folder_path);
			LOG_DEBUG("%s\n", pHandle->dev_sta.folder[i].cpath);
			pthread_mutex_init(&(pHandle->dev_sta.folder[i].mutex), NULL);
			if (rkipc_storage_create_folder(pHandle->dev_sta.folder[i].cpath)) {
				LOG_ERROR("CreateFolder failed\n");
				goto file_scan_out;
			}
			LOG_INFO("[%s] i is %d, before rkipc_storage_read_file_list\n", get_time_string(), i);
			rkipc_storage_read_file_list(&pHandle->dev_sta.folder[i], &devAttr.folder_attr[i]);
			LOG_INFO("[%s] i is %d, after rkipc_storage_read_file_list\n", get_time_string(), i);
		}
	}

	if (pHandle->dev_sta.mount_status == DISK_UNMOUNTED)
		goto file_scan_out;
	else
		pHandle->dev_sta.mount_status = DISK_MOUNTED;

	if (pHandle->dev_sta.mount_status != DISK_UNMOUNTED) {
		if (rkipc_storage_get_disk_size(devAttr.mount_path, &pHandle->dev_sta.total_size,
		                                &pHandle->dev_sta.free_size)) {
			LOG_ERROR("GetDiskSize failed\n");
			return NULL;
		}
	} else {
		pHandle->dev_sta.total_size = 0;
		pHandle->dev_sta.free_size = 0;
	}
	LOG_INFO("total_size = %d, free_size = %d\n", pHandle->dev_sta.total_size,
	         pHandle->dev_sta.free_size);

	if (pthread_create(&fileMonitorTid, NULL, rkipc_storage_file_monitor_thread, (void *)pHandle)) {
		LOG_ERROR("FileMonitorThread create failed.\n");
		goto file_scan_out;
	}

	if (g_file_scan_signal)
		rk_signal_destroy(g_file_scan_signal);
	g_file_scan_signal = rk_signal_create(0, 1);
	if (!g_file_scan_signal) {
		LOG_ERROR("create signal fail\n");
		return NULL;
	}
	while (pHandle->dev_sta.mount_status == DISK_MOUNTED) {
		rk_signal_wait(g_file_scan_signal, 60 * 1000); // 60s
		if (pHandle->dev_sta.mount_status != DISK_MOUNTED)
			break;

		total_space = 0;
		// delete file by num limit
		for (i = 0; i < devAttr.folder_num; i++) {
			if (devAttr.folder_attr[i].num_limit == false)
				continue;
			pthread_mutex_lock(&pHandle->dev_sta.folder[i].mutex);
			limit = pHandle->dev_sta.folder[i].file_num;
			while (limit > devAttr.folder_attr[i].limit) {
				limit = pHandle->dev_sta.folder[i].file_num;
				if (pHandle->dev_sta.folder[i].file_list_last) {
					sprintf(file, "%s/%s/%s", devAttr.mount_path,
					        devAttr.folder_attr[i].folder_path,
					        pHandle->dev_sta.folder[i].file_list_last->filename);
					LOG_INFO("delete file by num limit: %s\n", file);
					pthread_mutex_unlock(&pHandle->dev_sta.folder[i].mutex);
					// when the deletion is too fast,
					// the other listener thread cannot respond in time,
					// which will cause duplication twice, so delete it directly here first
					char filename[RKIPC_MAX_FILE_PATH_LEN];
					snprintf(filename, RKIPC_MAX_FILE_PATH_LEN, "%s",
					         pHandle->dev_sta.folder[i].file_list_last->filename);
					rkipc_storage_file_list_del(&pHandle->dev_sta.folder[i], filename);
					if (remove(file))
						LOG_ERROR("Delete %s file error.\n", file);
					usleep(100);
					continue;
				}
			}
			pthread_mutex_unlock(&pHandle->dev_sta.folder[i].mutex);
		}

		if (rkipc_storage_get_disk_size(devAttr.mount_path, &pHandle->dev_sta.total_size,
		                                &pHandle->dev_sta.free_size)) {
			LOG_ERROR("GetDiskSize failed\n");
			goto file_scan_out;
		}
		if (pHandle->dev_sta.free_size > (devAttr.free_size_del_min * 1024))
			continue;

		// delete file by space limit
		for (i = 0; i < devAttr.folder_num; i++) {
			pthread_mutex_lock(&pHandle->dev_sta.folder[i].mutex);
			if (devAttr.folder_attr[i].num_limit == false)
				total_space += pHandle->dev_sta.folder[i].total_space;
			pthread_mutex_unlock(&pHandle->dev_sta.folder[i].mutex);
		}
		if (!total_space) {
			usleep(1 * 1000 * 1000);
			continue;
		}
		LOG_INFO("pHandle->dev_sta.free_size is %d, min is %d, max is %d\n",
		         pHandle->dev_sta.free_size, devAttr.free_size_del_min, devAttr.free_size_del_max);
		LOG_INFO("total_space is %lld\n", total_space);
		for (i = 0; i < devAttr.folder_num; i++) {
			if (devAttr.folder_attr[i].num_limit == true)
				continue;
			pthread_mutex_lock(&pHandle->dev_sta.folder[i].mutex);
			limit = pHandle->dev_sta.folder[i].total_space * 100 / total_space;
			// LOG_INFO("pHandle->dev_sta.folder[i].total_space*100 is %lld, total_space is %lld\n",
			// pHandle->dev_sta.folder[i].total_space*100, total_space); LOG_INFO("limit is %d,
			// devAttr.folder_attr[i].limit is %d\n", limit, devAttr.folder_attr[i].limit);
			while (limit > devAttr.folder_attr[i].limit) {
				limit = pHandle->dev_sta.folder[i].total_space * 100 / total_space;
				if (pHandle->dev_sta.folder[i].file_list_last) {
					sprintf(file, "%s/%s/%s", devAttr.mount_path,
					        devAttr.folder_attr[i].folder_path,
					        pHandle->dev_sta.folder[i].file_list_last->filename);
					LOG_INFO("delete file by space limit: %s\n", file);
					pthread_mutex_unlock(&pHandle->dev_sta.folder[i].mutex);
					// when the deletion is too fast,
					// the other listener thread cannot respond in time,
					// which will cause duplication twice, so delete it directly here first
					char filename[RKIPC_MAX_FILE_PATH_LEN];
					snprintf(filename, RKIPC_MAX_FILE_PATH_LEN, "%s",
					         pHandle->dev_sta.folder[i].file_list_last->filename);
					rkipc_storage_file_list_del(&pHandle->dev_sta.folder[i], filename);
					if (remove(file))
						LOG_ERROR("Delete %s file error.\n", file);
					usleep(100);
					continue;
				}
			}
			pthread_mutex_unlock(&pHandle->dev_sta.folder[i].mutex);
		}
	}

file_scan_out:
	if (fileMonitorTid)
		if (pthread_join(fileMonitorTid, NULL))
			LOG_ERROR("FileMonitorThread join failed.\n");
	LOG_DEBUG("out\n");

	if (pHandle->dev_sta.folder) {
		for (int i = 0; i < pHandle->dev_sta.folder_num; i++) {
			while (pHandle->dev_sta.folder[i].file_list_first) {
				rkipc_str_file *tmp = pHandle->dev_sta.folder[i].file_list_first;
				pHandle->dev_sta.folder[i].file_list_first =
				    pHandle->dev_sta.folder[i].file_list_first->next;
				free(tmp);
				if (pHandle->dev_sta.folder[i].file_list_first == NULL) {
					pHandle->dev_sta.folder[i].file_list_last = NULL;
				}
			}
		}
		free(pHandle->dev_sta.folder);
		pHandle->dev_sta.folder = NULL;
	}
	pHandle->dev_sta.folder_num = 0;

	return NULL;
}

static int rkipc_storage_dev_add(char *dev, rkipc_storage_handle *pHandle) {
	int ret;
	rkipc_str_dev_attr dev_attr;
	char mountPath[RKIPC_MAX_FILE_PATH_LEN];

	RKIPC_CHECK_POINTER(dev, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);

	dev_attr = rkipc_storage_get_param(pHandle);
	LOG_INFO("%s, %s", dev, mountPath);

	if (dev_attr.dev_path[0]) {
		if (strcmp(dev_attr.dev_path, dev)) {
			LOG_ERROR("dev_attr.dev_path[%s] != dev[%s]\n", dev_attr.dev_path, dev);
			return -1;
		}
		sprintf(pHandle->dev_sta.dev_path, dev_attr.dev_path);
	}

	ret = rkipc_storage_get_mount_path(dev, mountPath, RKIPC_MAX_FILE_PATH_LEN);
	if (ret) {
		LOG_ERROR("rkipc_storage_get_mount_path failed[%d]\n", ret);
		if (dev_attr.dev_path[0]) {
			pHandle->dev_sta.mount_status = DISK_NOT_FORMATTED;
		}
		return ret;
	}

	if (dev_attr.mount_path[0]) {
		if (strcmp(dev_attr.mount_path, mountPath)) {
			LOG_ERROR("dev_attr.mount_path[%s] != mountPath[%s]\n", dev_attr.mount_path, mountPath);
			return -1;
		}
	} else {
		sprintf(dev_attr.mount_path, mountPath);
	}

	ret = rkipc_storage_get_mount_dev(dev_attr.mount_path, pHandle->dev_sta.dev_path,
	                                  pHandle->dev_sta.dev_type, pHandle->dev_sta.dev_attr_1);
	if (ret) {
		LOG_ERROR("rkipc_storage_get_mount_dev failed[%d]\n", ret);
		return ret;
	}

	pHandle->dev_sta.mount_status = DISK_SCANNING;
	if (pthread_create(&pHandle->dev_sta.file_scan_tid, NULL, rkipc_storage_file_scan_thread,
	                   (void *)pHandle))
		LOG_ERROR("FileScanThread create failed.\n");

	return 0;
}

static int rkipc_storage_dev_remove(char *dev, rkipc_storage_handle *pHandle) {
	RKIPC_CHECK_POINTER(dev, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);

	if (!strcmp(pHandle->dev_sta.dev_path, dev)) {
		pHandle->dev_sta.mount_status = DISK_UNMOUNTED;
		pHandle->dev_sta.total_size = 0;
		pHandle->dev_sta.free_size = 0;
		pHandle->dev_sta.fsck_quit = 1;
		if (g_file_scan_signal)
			rk_signal_give(g_file_scan_signal);
		if (pHandle->dev_sta.file_scan_tid) {
			if (pthread_join(pHandle->dev_sta.file_scan_tid, NULL))
				LOG_ERROR("FileScanThread join failed.");
			pHandle->dev_sta.file_scan_tid = 0;
		}
		if (g_file_scan_signal) {
			rk_signal_destroy(g_file_scan_signal);
			g_file_scan_signal = NULL;
		}
		umount2(pHandle->dev_attr.mount_path, MNT_DETACH);
	}

	return 0;
}

static int rkipc_storage_msg_put_msg_to_buffer(rkipc_tmsg_buffer *buf, rkipc_tmsg_element *elm) {
	RKIPC_CHECK_POINTER(buf, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(elm, RKIPC_STORAGE_FAIL);

	if (NULL != elm->next)
		elm->next = NULL;

	pthread_mutex_lock(&buf->mutex);
	if (buf->first) {
		rkipc_tmsg_element *tmp = buf->first;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = elm;
	} else {
		buf->first = elm;
	}
	buf->num++;

	pthread_cond_signal(&buf->not_empty);
	pthread_mutex_unlock(&buf->mutex);
	return 0;
}

static rkipc_tmsg_element *rkipc_storage_msg_get_msg_from_buffer_timeout(rkipc_tmsg_buffer *buf,
                                                                         int s32TimeoutMs) {
	rkipc_tmsg_element *elm = NULL;
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
		pthread_cond_timedwait(&buf->not_empty, &buf->mutex, &timeout);
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

static int rkipc_storage_msg_free_msg(rkipc_tmsg_element *elm) {
	RKIPC_CHECK_POINTER(elm, RKIPC_STORAGE_FAIL);

	if (elm->data != NULL) {
		free(elm->data);
		elm->data = NULL;
	}
	free(elm);
	elm = NULL;

	return 0;
}

static void *rkipc_storage_msg_rec_msg_thread(void *arg) {
	rkipc_tmsg_buffer *msgBuffer = (rkipc_tmsg_buffer *)arg;

	if (!msgBuffer) {
		LOG_ERROR("invalid msgBuffer");
		return NULL;
	}

	prctl(PR_SET_NAME, "rkipc_storage_msg_rec_msg_thread", 0, 0, 0);
	while (msgBuffer->quit == 0) {
		rkipc_tmsg_element *elm = rkipc_storage_msg_get_msg_from_buffer_timeout(msgBuffer, 1000);

		if (elm) {
			if (msgBuffer->rec_msg_cb)
				msgBuffer->rec_msg_cb(msgBuffer, elm->msg, elm->data, elm->data_len,
				                      msgBuffer->handle_path);
			if (rkipc_storage_msg_free_msg(elm))
				LOG_ERROR("Free msg failed.");
		}
	}

	LOG_DEBUG("out\n");
	return NULL;
}

static int rkipc_storage_msg_rec_cb(void *hd, int msg, void *data, int data_len, void *pHandle) {
	LOG_INFO("msg = %d\n", msg);
	rkipc_storage_handle *ppHandle = (rkipc_storage_handle *)pHandle;
	switch (msg) {
	case MSG_DEV_ADD:
		if (rkipc_storage_dev_add((char *)data, ppHandle)) {
			LOG_ERROR("DevAdd failed\n");
			return -1;
		}
		if (ppHandle->dev_sta.dev_path != NULL) {
			char entry[128] = {'\0'};
			LOG_INFO("recognized dev_path\n");
			for (int i = 0; i < STORAGE_NUM - 1; i++) {
				snprintf(entry, 127, "storage.%d:enable", i);
				if (rk_param_get_int(entry, 0) == 1) {
					LOG_INFO("start record!\n");
					record_flag[i] = 1;
					rk_storage_muxer_deinit_by_id(i);
					rk_storage_muxer_init_by_id(i);
				} else if (rk_param_get_int(entry, 0) == 0) {
					record_flag[i] = 0;
					rk_storage_muxer_deinit_by_id(i);
				}
			}
		}
		break;
	case MSG_DEV_REMOVE:
		if (rkipc_storage_dev_remove((char *)data, ppHandle)) {
			LOG_ERROR("DevRemove failed\n");
			return -1;
		}
		LOG_ERROR("unrecognized dev_path,stop record!\n");
		for (int i = 0; i < STORAGE_NUM - 1; i++) {
			record_flag[i] = 0;
			rk_storage_muxer_deinit_by_id(i);
		}
		break;
	case MSG_DEV_CHANGED:
		break;
	}

	return 0;
}

static int rkipc_storage_msg_create(rkipc_reg_msg_cb rec_msg_cb, rkipc_storage_handle *pHandle) {
	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);

	pHandle->msg_hd.first = NULL;
	pHandle->msg_hd.last = NULL;
	pHandle->msg_hd.num = 0;
	pHandle->msg_hd.quit = 0;
	pHandle->msg_hd.rec_msg_cb = rec_msg_cb;
	pHandle->msg_hd.handle_path = (void *)pHandle;

	pthread_mutex_init(&(pHandle->msg_hd.mutex), NULL);
	pthread_cond_init(&(pHandle->msg_hd.not_empty), NULL);
	if (pthread_create(&(pHandle->msg_hd.rec_tid), NULL, rkipc_storage_msg_rec_msg_thread,
	                   (void *)(&pHandle->msg_hd))) {
		LOG_ERROR("RecMsgThread create failed!");
		return -1;
	}

	return 0;
}

static int rkipc_storage_msg_destroy(rkipc_storage_handle *pHandle) {
	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);

	pHandle->msg_hd.quit = 1;
	if (pHandle->msg_hd.rec_tid)
		if (pthread_join(pHandle->msg_hd.rec_tid, NULL)) {
			LOG_ERROR("RecMsgThread join failed!");
			return -1;
		}

	return 0;
}

static int rkipc_storage_msg_send_msg(int msg, char *data, int data_len, rkipc_tmsg_buffer *buf) {
	rkipc_tmsg_element *elm = NULL;

	RKIPC_CHECK_POINTER(buf, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(data, RKIPC_STORAGE_FAIL);

	elm = (rkipc_tmsg_element *)malloc(sizeof(rkipc_tmsg_element));
	if (!elm) {
		LOG_ERROR("elm malloc failed.");
		return -1;
	}

	memset(elm, 0, sizeof(rkipc_tmsg_element));
	elm->msg = msg;
	elm->data = NULL;
	elm->data_len = data_len;

	if (data && data_len > 0) {
		elm->data = (char *)malloc(data_len);
		if (!elm->data) {
			LOG_ERROR("elm->data malloc failed.");
			free(elm);
			return -1;
		}
		memset(elm->data, 0, data_len);
		memcpy(elm->data, data, data_len);
	}

	elm->next = NULL;

	if (rkipc_storage_msg_put_msg_to_buffer(buf, elm)) {
		if (!elm->data)
			free(elm->data);
		free(elm);
		LOG_ERROR("Put msg to buffer failed.");
		return -1;
	}

	return 0;
}

static char *rkipc_storage_str_search(char *buf, int len, const char *str) {
	char *ret = 0;
	int i = 0;

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

static char *rkipc_storage_get_para(char *buf, int len, const char *str) {
	char *ret = rkipc_storage_str_search(buf, len, str);

	if (ret)
		ret += strlen(str) + 1;

	return ret;
}

static void *rkipc_storage_event_listener_thread(void *arg) {
	rkipc_storage_handle *pHandle = (rkipc_storage_handle *)arg;
	int sockfd;
	int len;
	int bufLen = 2000;
	char buf[bufLen];
	struct iovec iov;
	struct msghdr msg;
	struct sockaddr_nl sa;
	struct timeval timeout;

	if (!pHandle) {
		LOG_ERROR("invalid pHandle\n");
		return NULL;
	}

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	prctl(PR_SET_NAME, "event_monitor", 0, 0, 0);

	sa.nl_family = AF_NETLINK;
	sa.nl_groups = NETLINK_KOBJECT_UEVENT;
	sa.nl_pid = 0;
	iov.iov_base = (void *)buf;
	iov.iov_len = bufLen;
	memset(&msg, 0, sizeof(msg));
	msg.msg_name = (void *)&sa;
	msg.msg_namelen = sizeof(sa);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
	if (sockfd == -1) {
		LOG_ERROR("socket creating failed:%s\n", strerror(errno));
		return NULL;
	}

	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout,
	           (socklen_t)sizeof(struct timeval));

	if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
		LOG_ERROR("bind error:%s\n", strerror(errno));
		goto err_event_listener;
	}

	while (pHandle->event_listener_run) {
		len = recvmsg(sockfd, &msg, 0);
		if (len < 0) {
			// LOG_WARN("receive time out\n");
		} else if (len < MAX_TYPE_NMSG_LEN || len > bufLen) {
			LOG_WARN("invalid message\n");
		} else {
			char *p = strstr(buf, "libudev");

			if (p == buf) {
				if (rkipc_storage_str_search(buf, len, "DEVTYPE=partition") ||
				    rkipc_storage_str_search(buf, len, "DEVTYPE=disk")) {
					char *dev = rkipc_storage_get_para(buf, len, "DEVNAME");

					if (rkipc_storage_str_search(buf, len, "ACTION=add")) {
						if (rkipc_storage_msg_send_msg(MSG_DEV_ADD, dev, strlen(dev) + 1,
						                               &(pHandle->msg_hd)))
							LOG_ERROR("Send msg: MSG_DEV_ADD failed.\n");
					} else if (rkipc_storage_str_search(buf, len, "ACTION=remove")) {
						LOG_INFO("%s remove\n", dev);
						for (int i = 0; i < STORAGE_NUM; i++) {
							g_storage_record_flag[i] = 0;
						}
						if (rkipc_storage_msg_send_msg(MSG_DEV_REMOVE, dev, strlen(dev) + 1,
						                               &(pHandle->msg_hd)))
							LOG_ERROR("Send msg: MSG_DEV_REMOVE failed.");
					} else if (rkipc_storage_str_search(buf, len, "ACTION=change")) {
						LOG_INFO("%s change\n", dev);
						if (rkipc_storage_msg_send_msg(MSG_DEV_CHANGED, dev, strlen(dev) + 1,
						                               &(pHandle->msg_hd)))
							LOG_ERROR("Send msg: MSG_DEV_CHANGED failed.\n");
					}
				}
			}
		}
	}
err_event_listener:
	if (close(sockfd))
		LOG_ERROR("Close sockfd failed.\n");

	LOG_DEBUG("out\n");
	return NULL;
}

static int rkipc_storage_para_init(rkipc_storage_handle *pstHandle,
                                   rkipc_str_dev_attr *pstDevAttr) {
	int i;

	RKIPC_CHECK_POINTER(pstHandle, RKIPC_STORAGE_FAIL);

	if (pstDevAttr) {
		if (pstDevAttr->folder_attr) {
			sprintf(pstHandle->dev_attr.mount_path, pstDevAttr->mount_path);
			sprintf(pstHandle->dev_attr.dev_path, pstDevAttr->dev_path);
			LOG_DEBUG(" pstHandle->dev_attr mount path is %s, dev_path is %s\n",
			          pstHandle->dev_attr.mount_path, pstHandle->dev_attr.dev_path);
			pstHandle->dev_attr.auto_delete = pstDevAttr->auto_delete;
			pstHandle->dev_attr.free_size_del_min = pstDevAttr->free_size_del_min;
			pstHandle->dev_attr.free_size_del_max = pstDevAttr->free_size_del_max;
			pstHandle->dev_attr.folder_num = pstDevAttr->folder_num;
			pstHandle->dev_attr.check_format_id = pstDevAttr->check_format_id;
			memcpy(pstHandle->dev_attr.format_id, pstDevAttr->format_id, RKIPC_MAX_FORMAT_ID_LEN);
			memcpy(pstHandle->dev_attr.volume, pstDevAttr->volume, RKIPC_MAX_VOLUME_LEN);

			pstHandle->dev_attr.folder_attr = (rkipc_str_folder_attr *)malloc(
			    sizeof(rkipc_str_folder_attr) * pstHandle->dev_attr.folder_num);
			if (!pstHandle->dev_attr.folder_attr) {
				LOG_ERROR("pstHandle->dev_attr.folder_attr malloc failed.\n");
				return -1;
			}
			memset(pstHandle->dev_attr.folder_attr, 0,
			       sizeof(rkipc_str_folder_attr) * pstHandle->dev_attr.folder_num);

			for (i = 0; i < pstDevAttr->folder_num; i++) {
				pstHandle->dev_attr.folder_attr[i].sort_cond = pstDevAttr->folder_attr[i].sort_cond;
				pstHandle->dev_attr.folder_attr[i].num_limit = pstDevAttr->folder_attr[i].num_limit;
				pstHandle->dev_attr.folder_attr[i].limit = pstDevAttr->folder_attr[i].limit;
				sprintf(pstHandle->dev_attr.folder_attr[i].folder_path,
				        pstDevAttr->folder_attr[i].folder_path);
			}

			for (i = 0; i < pstDevAttr->folder_num; i++) {
				LOG_INFO("DevAttr set:  AutoDel--%d, FreeSizeDel--%d~%d, Path--%s/%s, "
				         "Limit--%d\n",
				         pstHandle->dev_attr.auto_delete, pstHandle->dev_attr.free_size_del_min,
				         pstHandle->dev_attr.free_size_del_max, pstHandle->dev_attr.mount_path,
				         pstHandle->dev_attr.folder_attr[i].folder_path,
				         pstHandle->dev_attr.folder_attr[i].limit);
			}

			LOG_DEBUG("Set user-defined device attributes done.\n");
			return 0;
		} else {
			LOG_ERROR("The device attributes set failed.\n");
			return -1;
		}
	}
	rkipc_storage_set_dev_attr(&pstHandle->dev_attr);

	return 0;
}

static int rkipc_storage_para_deinit(rkipc_storage_handle *pHandle) {
	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);

	if (pHandle->dev_attr.folder_attr) {
		free(pHandle->dev_attr.folder_attr);
		pHandle->dev_attr.folder_attr = NULL;
	}

	return 0;
}

static int rkipc_storage_auto_delete_init(rkipc_storage_handle *pstHandle) {
	rkipc_str_dev_attr dev_attr;
	RKIPC_CHECK_POINTER(pstHandle, RKIPC_STORAGE_FAIL);
	dev_attr = rkipc_storage_get_param(pstHandle);
	LOG_INFO("mountpath:%s,devpath:%s,devtype:%s,devattr:%s\n", dev_attr.mount_path,
	         pstHandle->dev_sta.dev_path, pstHandle->dev_sta.dev_type,
	         pstHandle->dev_sta.dev_attr_1);
	if (!rkipc_storage_get_mount_dev(dev_attr.mount_path, pstHandle->dev_sta.dev_path,
	                                 pstHandle->dev_sta.dev_type, pstHandle->dev_sta.dev_attr_1)) {
		pstHandle->dev_sta.mount_status = DISK_SCANNING;
		if (pthread_create(&(pstHandle->dev_sta.file_scan_tid), NULL,
		                   rkipc_storage_file_scan_thread, (void *)(pstHandle))) {
			LOG_ERROR("FileScanThread create failed.\n");
			return -1;
		}
	} else {
		pstHandle->dev_sta.mount_status = DISK_UNMOUNTED;
		LOG_ERROR("GetMountDev failed.\n");
		return -1;
	}

	return 0;
}

static int rkipc_storage_auto_delete_deinit(rkipc_storage_handle *pHandle) {
	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);

	pHandle->dev_sta.mount_status = DISK_UNMOUNTED;
	if (g_file_scan_signal)
		rk_signal_give(g_file_scan_signal);
	if (pHandle->dev_sta.file_scan_tid) {
		if (pthread_join(pHandle->dev_sta.file_scan_tid, NULL))
			LOG_ERROR("FileScanThread join failed.\n");
	}
	if (g_file_scan_signal) {
		rk_signal_destroy(g_file_scan_signal);
		g_file_scan_signal = NULL;
	}

	return 0;
}

static int rkipc_storage_listen_msg_init(rkipc_storage_handle *pstHandle) {
	RKIPC_CHECK_POINTER(pstHandle, RKIPC_STORAGE_FAIL);

	pstHandle->event_listener_run = 1;

	if (rkipc_storage_msg_create(&rkipc_storage_msg_rec_cb, pstHandle)) {
		LOG_ERROR("Msg create failed.");
		return -1;
	}

	if (pthread_create(&pstHandle->event_listener_tid, NULL, rkipc_storage_event_listener_thread,
	                   (void *)pstHandle)) {
		LOG_ERROR("EventListenerThread create failed.");
		return -1;
	}

	return 0;
}

int rkipc_storage_manager_init(void **ppHandle, rkipc_str_dev_attr *pstDevAttr) {
	rkipc_storage_handle *pstHandle = NULL;

	if (*ppHandle) {
		LOG_ERROR("Storage handle has been inited.\n");
		return -1;
	}

	pstHandle = (rkipc_storage_handle *)malloc(sizeof(rkipc_storage_handle));
	if (!pstHandle) {
		LOG_ERROR("pstHandle malloc failed.\n");
		return -1;
	}
	memset(pstHandle, 0, sizeof(rkipc_storage_handle));

	if (rkipc_storage_para_init(pstHandle, pstDevAttr)) {
		LOG_ERROR("Parameter init failed.\n");
		goto failed;
	}

	if (rkipc_storage_auto_delete_init(pstHandle))
		LOG_ERROR("AutoDelete init failed.\n");

	if (rkipc_storage_listen_msg_init(pstHandle)) {
		LOG_ERROR("Listener and Msg init failed.\n");
		goto failed;
	}

	*ppHandle = (void *)pstHandle;
	return 0;

failed:
	if (pstHandle) {
		free(pstHandle);
	}

	return -1;
}

int rkipc_storage_manager_deinit(void *pHandle) {
	rkipc_storage_handle *pstHandle = NULL;

	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);
	pstHandle = (rkipc_storage_handle *)pHandle;
	pstHandle->event_listener_run = 0;
	pstHandle->dev_sta.fsck_quit = 1;

	if (pstHandle->event_listener_tid)
		if (pthread_join(pstHandle->event_listener_tid, NULL))
			LOG_ERROR("EventListenerThread join failed.");

	if (rkipc_storage_msg_destroy(pstHandle))
		LOG_ERROR("Msg destroy failed.");

	if (rkipc_storage_auto_delete_deinit(pstHandle))
		LOG_ERROR("AutoDelete deinit failed.");

	if (rkipc_storage_para_deinit(pstHandle))
		LOG_ERROR("Paramete deinit failed.");

	free(pstHandle);
	pstHandle = NULL;

	return 0;
}

rkipc_mount_status rkipc_storage_get_mount_status(void *pHandle) {
	rkipc_storage_handle *pstHandle;

	RKIPC_CHECK_POINTER(pHandle, DISK_MOUNT_BUTT);
	pstHandle = (rkipc_storage_handle *)pHandle;
	return pstHandle->dev_sta.mount_status;
}

int rkipc_storage_get_capacity(void **ppHandle, int *total_size, int *free_size) {
	rkipc_storage_handle *pstHandle = NULL;
	rkipc_str_dev_attr dev_attr;

	RKIPC_CHECK_POINTER(ppHandle, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(*ppHandle, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(total_size, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(free_size, RKIPC_STORAGE_FAIL);
	pstHandle = (rkipc_storage_handle *)*ppHandle;
	dev_attr = rkipc_storage_get_param(pstHandle);

	if (pstHandle->dev_sta.mount_status == DISK_MOUNTED) {
		rkipc_storage_get_disk_size(dev_attr.mount_path, &pstHandle->dev_sta.total_size,
		                            &pstHandle->dev_sta.free_size);
	} else {
		pstHandle->dev_sta.total_size = 0;
		pstHandle->dev_sta.free_size = 0;
	}
	*total_size = pstHandle->dev_sta.total_size;
	*free_size = pstHandle->dev_sta.free_size;

	*ppHandle = (void *)pstHandle;
	return 0;
}

int rkipc_storage_get_file_list(rkipc_filelist *list, void *pHandle, rkipc_sort_type sort) {
	int i, j;
	rkipc_storage_handle *pstHandle = NULL;

	RKIPC_CHECK_POINTER(list, RKIPC_STORAGE_FAIL);
	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);
	pstHandle = (rkipc_storage_handle *)pHandle;

	for (i = 0; i < pstHandle->dev_sta.folder_num; i++) {
		if (!strcmp(list->path, pstHandle->dev_sta.folder[i].cpath))
			break;
	}

	if (i == pstHandle->dev_sta.folder_num) {
		LOG_ERROR("No folder found. Please check the folder path.\n");
		return -1;
	}

	pthread_mutex_lock(&pstHandle->dev_sta.folder[i].mutex);

	rkipc_str_file *tmp = pstHandle->dev_sta.folder[i].file_list_first;
	list->file_num = pstHandle->dev_sta.folder[i].file_num;
	list->file = (rkipc_fileinfo *)malloc(sizeof(rkipc_fileinfo) * list->file_num);
	if (!list->file) {
		LOG_ERROR("list->file malloc failed.");
		return -1;
	}
	memset(list->file, 0, sizeof(rkipc_fileinfo) * list->file_num);

	if (sort == LIST_ASCENDING) {
		for (j = 0; j < list->file_num && tmp != NULL; j++) {
			strcpy(list->file[j].filename, tmp->filename);
			list->file[j].size = tmp->size;
			list->file[j].time = tmp->time;
			tmp = tmp->next;
		}
	} else {
		for (j = list->file_num - 1; j >= 0 && tmp != NULL; j--) {
			strcpy(list->file[j].filename, tmp->filename);
			list->file[j].size = tmp->size;
			list->file[j].time = tmp->time;
			tmp = tmp->next;
		}
	}

	pthread_mutex_unlock(&pstHandle->dev_sta.folder[i].mutex);
	return 0;
}

int rkipc_storage_free_file_list(rkipc_filelist *list) {
	if (list->file) {
		free(list->file);
		list->file = NULL;
	}

	return 0;
}

int rkipc_storage_get_file_num(char *fileListPath, void *pHandle) {
	int i;
	rkipc_storage_handle *pstHandle = NULL;

	RKIPC_CHECK_POINTER(pHandle, RKIPC_STORAGE_FAIL);
	pstHandle = (rkipc_storage_handle *)pHandle;

	for (i = 0; i < pstHandle->dev_sta.folder_num; i++) {
		if (!strcmp(fileListPath, pstHandle->dev_sta.folder[i].cpath))
			break;
	}

	if (i == pstHandle->dev_sta.folder_num)
		return 0;

	return pstHandle->dev_sta.folder[i].file_num;
}

char *rkipc_storage_get_dev_path(void *pHandle) {
	rkipc_storage_handle *pstHandle = NULL;

	RKIPC_CHECK_POINTER(pHandle, NULL);
	pstHandle = (rkipc_storage_handle *)pHandle;

	return pstHandle->dev_sta.dev_path;
}

// get quota ,response set quota by id
// int rkipc_storage_quota_get(int id, char **value) {

// 	cJSON *Array = NULL;
// 	cJSON *sd_info = NULL;
// 	Array = cJSON_CreateArray();
// 	cJSON_AddItemToArray(Array, sd_info = cJSON_CreateObject());
// 	cJSON_AddNumberToObject(sd_info, "iFreePictureQuota", 0);
// 	cJSON_AddNumberToObject(sd_info, "iFreeVideoQuota", 0);
// 	cJSON_AddNumberToObject(sd_info, "iPictureQuotaRatio", g_sd_dev_attr.folder_attr[1].limit);
// 	cJSON_AddStringToObject(sd_info, "iTotalPictureVolume", g_sd_dev_attr.volume);
// 	cJSON_AddStringToObject(sd_info, "iTotalVideoVolume", g_sd_dev_attr.volume);
// 	cJSON_AddNumberToObject(sd_info, "iVideoQuotaRatio", g_sd_dev_attr.folder_attr[0].limit);
// 	cJSON_AddNumberToObject(sd_info, "id", 0);

// 	*value = cJSON_Print(sd_info);
// 	cJSON_Delete(sd_info);

// 	return 0;
// }

// TODO
int rkipc_storage_quota_set(int id, char *value) { return 0; }

// get hdd_list
// int rkipc_storage_hdd_list_get(int id, char **value) {
// 	int total_size;
// 	int free_size;
// 	rkipc_storage_handle *phandle = (rkipc_storage_handle *)g_sd_phandle;
// 	rkipc_storage_get_capacity(g_sd_phandle, &total_size, &free_size);
// 	char status[12];
// 	if (phandle->dev_sta.mount_status == DISK_MOUNTED) {
// 		strcpy(status, "mounted");
// 	} else {
// 		strcpy(status, "unmounted");
// 	}
// 	cJSON *Array = NULL;
// 	cJSON *sd_info = NULL;
// 	Array = cJSON_CreateArray();
// 	cJSON_AddItemToArray(Array, sd_info = cJSON_CreateObject());
// 	cJSON_AddNumberToObject(sd_info, "iFormatProg", 0);
// 	cJSON_AddNumberToObject(sd_info, "iFormatStatus", 0);
// 	cJSON_AddNumberToObject(sd_info, "iMediaSize", 0);
// 	cJSON_AddNumberToObject(sd_info, "iFreeSize", free_size);
// 	cJSON_AddNumberToObject(sd_info, "iTotalSize", total_size);
// 	cJSON_AddNumberToObject(sd_info, "id", 0);
// 	cJSON_AddStringToObject(sd_info, "sDev", "");
// 	cJSON_AddStringToObject(sd_info, "sFormatErr", "");
// 	cJSON_AddStringToObject(sd_info, "sMountPath", g_sd_dev_attr.mount_path);
// 	cJSON_AddStringToObject(sd_info, "sName", "SD Card");
// 	cJSON_AddStringToObject(sd_info, "sStatus", status);
// 	cJSON_AddStringToObject(sd_info, "sType", "");

// 	*value = cJSON_Print(sd_info);
// 	cJSON_Delete(sd_info);

// 	return 0;
// }

// response set snap_plan by id
// int rkipc_storage_snap_plan_get(int id, char **value) {
// 	cJSON *plan_info = NULL;
// 	plan_info = cJSON_CreateObject();
// 	cJSON_AddNumberToObject(plan_info, "iEnabled", 0);
// 	cJSON_AddNumberToObject(plan_info, "iImageQuality", 10);
// 	cJSON_AddNumberToObject(plan_info, "iShotInterval", 1000);
// 	cJSON_AddNumberToObject(plan_info, "iShotNumber", 4);
// 	cJSON_AddStringToObject(plan_info, "sImageType", "JPEG");
// 	cJSON_AddStringToObject(plan_info, "sResolution", "2688*1520");

// 	*value = cJSON_Print(plan_info);
// 	cJSON_Delete(plan_info);

// 	return 0;
// }

// TODO
int rkipc_storage_snap_plan_set(int id, char *value) { return 0; }

// int rkipc_storage_current_path_get(char **value) {
// 	cJSON *path_info = NULL;
// 	path_info = cJSON_CreateObject();
// 	cJSON_AddStringToObject(path_info, "sMountPath", g_sd_dev_attr.mount_path);
// 	*value = cJSON_Print(path_info);
// 	cJSON_Delete(path_info);

// 	return 0;
// }

// TODO
int rkipc_storage_current_path_set(char *value) { return 0; }

// int rkipc_storage_search(char *file_info);

// delete
// char *rkipc_response_delete(int num, int id, char *name_list) {
// 	int ret;
// 	cJSON *del_info = NULL;
// 	del_info = cJSON_CreateObject();
// 	rkipc_storage_handle *phandle = (rkipc_storage_handle *)g_sd_phandle;
// 	if (id == 0) {
// 		for (int i = 0; i < num; i++) {
// 			ret = rkipc_storage_file_list_del(&phandle->dev_sta.folder[0], &name_list[i]);
// 			if (ret) {
// 				LOG_ERROR("delete %s failed!\n", &name_list[i]);
// 				cJSON_AddNumberToObject(del_info, "rst", 0);
// 				char *out = cJSON_Print(del_info);
// 				cJSON_Delete(del_info);
// 				return out;
// 			}
// 		}
// 	} else if (id == 1) {
// 		for (int i = 0; i < num; i++) {
// 			ret = rkipc_storage_file_list_del(&phandle->dev_sta.folder[1], &name_list[i]);
// 			if (ret) {
// 				LOG_ERROR("delete %s failed!\n", &name_list[i]);
// 				cJSON_AddNumberToObject(del_info, "rst", 0);
// 				char *out = cJSON_Print(del_info);
// 				cJSON_Delete(del_info);
// 				return out;
// 			}
// 		}
// 	} else if (id == 2) {
// 		for (int i = 0; i < num; i++) {
// 			ret = rkipc_storage_file_list_del(&phandle->dev_sta.folder[2], &name_list[i]);
// 			if (ret) {
// 				LOG_ERROR("delete %s failed!\n", &name_list[i]);
// 				cJSON_AddNumberToObject(del_info, "rst", 0);
// 				char *out = cJSON_Print(del_info);
// 				cJSON_Delete(del_info);
// 				return out;
// 			}
// 		}
// 	}
// 	cJSON_AddNumberToObject(del_info, "rst", 1);
// 	char *out = cJSON_Print(del_info);
// 	cJSON_Delete(del_info);
// 	return out;
// }

static void *rk_storage_record(void *arg) {
	int *id_ptr = arg;
	int id = *id_ptr;
	printf("id: %d, #Start %s thread, arg:%p\n", id, __func__, arg);
	prctl(PR_SET_NAME, "rk_storage_record", 0, 0, 0);
	while (g_storage_record_flag[id] && record_flag[id] == 1) {
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		snprintf(rk_storage_muxer_group[id].file_name, 512, "%s/%d%02d%02d%02d%02d%02d.%s",
		         rk_storage_muxer_group[id].record_path, tm.tm_year + 1900, tm.tm_mon + 1,
		         tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
		         rk_storage_muxer_group[id].file_format);
		LOG_INFO("[%d], file_name is %s\n", id, rk_storage_muxer_group[id].file_name);
		pthread_mutex_lock(&g_rkmuxer_mutex);
		rk_storage_muxer_group[0].g_record_run_ = 0;
		rkmuxer_deinit(id);
		rkmuxer_init(id, NULL, rk_storage_muxer_group[id].file_name,
		             &rk_storage_muxer_group[id].g_video_param,
		             &rk_storage_muxer_group[id].g_audio_param);
		rk_storage_muxer_group[0].g_record_run_ = 1;
		pthread_mutex_unlock(&g_rkmuxer_mutex);
		rk_signal_wait(rk_storage_muxer_group[id].g_storage_signal,
		               rk_storage_muxer_group[id].file_duration * 1000);
	}
	pthread_mutex_lock(&g_rkmuxer_mutex);
	rk_storage_muxer_group[0].g_record_run_ = 0;
	rkmuxer_deinit(id);
	pthread_mutex_unlock(&g_rkmuxer_mutex);

	return NULL;
}

static int rk_storage_muxer_init_by_id(int id) {
	LOG_DEBUG("begin\n");
	char entry[128] = {'\0'};
	const char *mount_path = NULL;
	const char *folder_name = NULL;

	rk_storage_muxer_group[id].id = id;
	// set rk_storage_muxer_group[id].g_video_param
	rk_storage_muxer_group[id].g_video_param.level = 52;
	snprintf(entry, 127, "video.%d:width", id);
	rk_storage_muxer_group[id].g_video_param.width = rk_param_get_int(entry, 1920);
	snprintf(entry, 127, "video.%d:height", id);
	rk_storage_muxer_group[id].g_video_param.height = rk_param_get_int(entry, 1080);
	snprintf(entry, 127, "video.%d:max_rate", id);
	rk_storage_muxer_group[id].g_video_param.bit_rate = rk_param_get_int(entry, 512) * 1024;
	snprintf(entry, 127, "video.%d:dst_frame_rate_den", id);
	rk_storage_muxer_group[id].g_video_param.frame_rate_den = rk_param_get_int(entry, 1);
	snprintf(entry, 127, "video.%d:dst_frame_rate_num", id);
	rk_storage_muxer_group[id].g_video_param.frame_rate_num = rk_param_get_int(entry, 30);
	snprintf(entry, 127, "video.%d:output_data_type", id);
	const char *output_data_type = rk_param_get_string(entry, NULL);
	if (output_data_type)
		memcpy(rk_storage_muxer_group[id].g_video_param.codec, output_data_type,
		       strlen(output_data_type));
	snprintf(entry, 127, "video.%d:h264_profile", id);
	const char *h264_profile = rk_param_get_string(entry, "high");
	if (!strcmp(h264_profile, "high"))
		rk_storage_muxer_group[id].g_video_param.profile = 100;
	else if (!strcmp(h264_profile, "main"))
		rk_storage_muxer_group[id].g_video_param.profile = 77;
	else if (!strcmp(h264_profile, "baseline"))
		rk_storage_muxer_group[id].g_video_param.profile = 66;
	memcpy(rk_storage_muxer_group[id].g_video_param.format, "NV12", strlen("NV12"));
	// set g_audio_param
	rk_storage_muxer_group[id].g_audio_param.channels = rk_param_get_int("audio.0:channels", 2);
	rk_storage_muxer_group[id].g_audio_param.sample_rate =
	    rk_param_get_int("audio.0:sample_rate", 16000);
	rk_storage_muxer_group[id].g_audio_param.frame_size =
	    rk_param_get_int("audio.0:frame_size", 1024);
	const char *format = rk_param_get_string("audio.0:format", NULL);
	if (format)
		memcpy(rk_storage_muxer_group[id].g_audio_param.format, format, strlen(format));
	const char *codec = rk_param_get_string("audio.0:encode_type", NULL);
	if (codec)
		memcpy(rk_storage_muxer_group[id].g_audio_param.codec, codec, strlen(codec));

	mount_path = rk_param_get_string("storage:mount_path", "/userdata");
	snprintf(entry, 127, "storage.%d:folder_name", id);
	folder_name = rk_param_get_string(entry, "video0");
	memset(&rk_storage_muxer_group[id].record_path, 0,
	       sizeof(rk_storage_muxer_group[id].record_path));
	strcat(rk_storage_muxer_group[id].record_path, mount_path);
	strcat(rk_storage_muxer_group[id].record_path, "/");
	strcat(rk_storage_muxer_group[id].record_path, folder_name);
	LOG_DEBUG("%d: record_path is %s\n", id, rk_storage_muxer_group[id].record_path);
	// create record_path if no exit
	DIR *d = opendir(rk_storage_muxer_group[id].record_path);
	if (d == NULL) {
		if (mkdir(rk_storage_muxer_group[id].record_path, 0777) == -1) {
			LOG_ERROR("Create %s fail\n", rk_storage_muxer_group[id].record_path);
			return -1;
		}
	} else {
		closedir(d);
	}

	snprintf(entry, 127, "storage.%d:file_format", id);
	rk_storage_muxer_group[id].file_format = rk_param_get_string(entry, "mp4");
	snprintf(entry, 127, "storage.%d:file_duration", id);
	rk_storage_muxer_group[id].file_duration = rk_param_get_int(entry, 60);

	snprintf(entry, 127, "storage.%d:enable", id);
	if (rk_param_get_int(entry, 0) == 0) {
		LOG_DEBUG("storage[%d]:enable is 0\n", id);
		return 0;
	}

	if (rk_storage_muxer_group[id].g_storage_signal)
		rk_signal_destroy(rk_storage_muxer_group[id].g_storage_signal);
	rk_storage_muxer_group[id].g_storage_signal = rk_signal_create(0, 1);
	if (!rk_storage_muxer_group[id].g_storage_signal) {
		LOG_ERROR("create signal fail\n");
		return -1;
	}
	g_storage_record_flag[id] = 1;
	pthread_create(&rk_storage_muxer_group[id].record_thread_id, NULL, rk_storage_record,
	               (void *)&rk_storage_muxer_group[id].id);
	LOG_INFO("end\n");

	return 0;
}

int rkipc_storage_dev_mount_status_get() {
	if (g_sd_phandle == NULL) {
		return DISK_UNMOUNTED;
	}
	rkipc_storage_handle *phandle = (rkipc_storage_handle *)g_sd_phandle;
	return phandle->dev_sta.mount_status;
}

int rk_storage_muxer_deinit_by_id(int id) {
	LOG_DEBUG("begin\n");
	char entry[128] = {'\0'};
	snprintf(entry, 127, "storage.%d:enable", id);
	if (rk_param_get_int(entry, 0) == 0) {
		LOG_DEBUG("storage[%d]:enable is 0\n", id);
		return 0;
	}
	g_storage_record_flag[id] = 0;
	if (rk_storage_muxer_group[id].g_storage_signal) {
		rk_signal_give(rk_storage_muxer_group[id].g_storage_signal);
		pthread_join(rk_storage_muxer_group[id].record_thread_id, NULL);
		rk_signal_destroy(rk_storage_muxer_group[id].g_storage_signal);
		rk_storage_muxer_group[id].g_storage_signal = NULL;
	}
	LOG_DEBUG("end\n");

	return 0;
}

// TODO, need record plan
int rk_storage_init() {
	if (rkipc_storage_set_dev_attr(&g_sd_dev_attr)) {
		LOG_ERROR("Set devAttr failed.\n");
		return -1;
	}

	if (rkipc_storage_manager_init(&g_sd_phandle, &g_sd_dev_attr)) {
		LOG_ERROR("Storage init failed.\n");
		return -1;
	}
	for (int i = 0; i < STORAGE_NUM; i++) {
		LOG_DEBUG("i:%d\n", i);
		rk_storage_muxer_init_by_id(i);
	}

	return 0;
}

int rk_storage_deinit() {
	for (int i = 0; i < STORAGE_NUM; i++) {
		rk_storage_muxer_deinit_by_id(i);
	}
	rkipc_storage_free_dev_attr(g_sd_dev_attr);
	rkipc_storage_manager_deinit(g_sd_phandle);
	g_sd_phandle = NULL;

	return 0;
}

int rk_storage_write_video_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time, int key_frame) {
	pthread_mutex_lock(&g_rkmuxer_mutex);
	if (rk_storage_muxer_group[id].g_record_run_)
		rkmuxer_write_video_frame(id, buffer, buffer_size, present_time, key_frame);
	pthread_mutex_unlock(&g_rkmuxer_mutex);

	return 0;
}

int rk_storage_write_audio_frame(int id, unsigned char *buffer, unsigned int buffer_size,
                                 int64_t present_time) {
	pthread_mutex_lock(&g_rkmuxer_mutex);
	if (rk_storage_muxer_group[id].g_record_run_)
		rkmuxer_write_audio_frame(id, buffer, buffer_size, present_time);
	pthread_mutex_unlock(&g_rkmuxer_mutex);

	return 0;
}

int rk_storage_record_start() {
	// only main stream
	LOG_INFO("start\n");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	snprintf(rk_storage_muxer_group[0].file_name, 512, "%s/%d%02d%02d%02d%02d%02d.%s",
	         rk_storage_muxer_group[0].record_path, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
	         tm.tm_hour, tm.tm_min, tm.tm_sec, rk_storage_muxer_group[0].file_format);
	LOG_INFO("file_name is %s\n", rk_storage_muxer_group[0].file_name);
	pthread_mutex_lock(&g_rkmuxer_mutex);
	rk_storage_muxer_group[0].g_record_run_ = 0;
	rkmuxer_deinit(0);
	rkmuxer_init(0, NULL, rk_storage_muxer_group[0].file_name,
	             &rk_storage_muxer_group[0].g_video_param,
	             &rk_storage_muxer_group[0].g_audio_param);
	rk_storage_muxer_group[0].g_record_run_ = 1;
	pthread_mutex_unlock(&g_rkmuxer_mutex);
	LOG_INFO("end\n");

	return 0;
}

int rk_storage_record_stop() {
	// only main stream
	LOG_INFO("start\n");
	pthread_mutex_lock(&g_rkmuxer_mutex);
	rk_storage_muxer_group[0].g_record_run_ = 0;
	rkmuxer_deinit(0);
	pthread_mutex_unlock(&g_rkmuxer_mutex);
	LOG_INFO("end\n");

	return 0;
}

int rk_storage_record_statue_get(int *value) {
	*value = rk_storage_muxer_group[0].g_record_run_;
	return 0;
}
