#ifndef _TY_MONITOR_H_
#define _TY_MONITOR_H_

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_WORK 10

typedef VOID (*WORK_CB)(VOID *param);

typedef struct {
	BOOL_T used;
	INT_T id;

	INT_T expire;

	INT_T work_period;
	WORK_CB monitor_work_cb;
	VOID *param;
} MONITOR_WORK_T;

typedef struct {
	BOOL_T inited;
	MONITOR_WORK_T work_pool[MAX_WORK];
	INT_T head;
} MONITOR_MGR_T;

typedef enum {
	DEBUG_RING_BUF = 0 - MAX_WORK,
	DEBUG_STREAM_STORAGE,
	DEBUG_CLOUD_STORAGE,

	DEBUG_MAX = -1
} DEBUG_MODULE_ID;
INT_T register_monitor_work(WORK_CB cb, VOID *param, INT_T period);
INT_T unregister_monitor_work(INT_T id);
INT_T ty_monitor_init();
INT_T ty_monitor_exit();

#ifdef __cplusplus
}
#endif

#endif
