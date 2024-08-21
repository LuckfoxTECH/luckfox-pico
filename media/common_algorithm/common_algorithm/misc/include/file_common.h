/* Copyright (c) Rockchip Electronics Co. Ltd. */

#ifndef __FILE_COMMON_H__
#define __FILE_COMMON_H__

#include <pthread.h>

typedef void (*FILE_CACHE_SDCARD_LOCK)(void);
typedef void (*FILE_CACHE_SDCARD_UNLOCK)(void);
typedef int (*FILE_CACHE_MOUMNT_SDCARD)(void);
typedef int (*FILE_CACHE_UMOUMNT_SDCARD)(void);

typedef enum _FILE_WRITE_MODE {
	NORMAL_MODE = 0,
	AOV_MODE,
	MODE_BUTT,
} FILE_WRITE_MODE;

typedef enum _FILE_SCHED_POLICY {
	FILE_SCHED_OTHER = 0,
	FILE_SCHED_BATCH,
	FILE_SCHED_IDLE,
	FILE_SCHED_FIFO,      /* sched_priority[1, 99] */
	FILE_SCHED_RR,        /* sched_priority[1, 99] */
} FILE_SCHED_POLICY;

typedef struct _FILE_WRITE_THREAD_ARG {
	FILE_SCHED_POLICY sched_policy;
	int priority;                 /* SCHED_OTHER/SCHED_IDLE/SCHED_BATCH inoperative */
} FILE_WRITE_THREAD_ARG;

typedef struct _FILE_SDCARD_ARG {
	FILE_CACHE_SDCARD_LOCK lock;
	FILE_CACHE_SDCARD_UNLOCK unlock;
	FILE_CACHE_MOUMNT_SDCARD mount_sdcard;
	FILE_CACHE_UMOUMNT_SDCARD umount_sdcard;
} FILE_SDCARD_ARG;

typedef struct _FILE_CACHE_ARG {
	const char *sdcard_path;
	int write_cache;
	int total_cache;
	FILE_WRITE_THREAD_ARG write_thread_arg;
	FILE_SDCARD_ARG sdcard_arg;
} FILE_CACHE_ARG;

#endif
