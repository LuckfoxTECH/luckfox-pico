/**
 * @file log_seq_timer_task.h
 * @brief provide timer task log sequence
 * @version 0.1
 * @date 2020-03-27
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __LOG_SEQ_TIMER_TASK_H
#define __LOG_SEQ_TIMER_TASK_H

#include "log_seq.h"

// define log sequence object is "timer_task", include this head file can recorde log sequence to
// "timer_task" module
#define LOGSEQ_OBJ "svc_timer_task"

/**
 * @brief define timer task detail log sequence information
 *        user can easy know bug location from this sequence, which include type, value, location
 */
typedef enum {
	// run timer task
	LOGSEQ_TIMER_TASK_EXECUTE = 0, // type:string, value:timer id and execute result, location:NULL

	// generic
	LOGSEQ_TIMER_TASK_TIME = 1,  // type:string, value:system time, location:NULL
	LOGSEQ_TIMER_TASK_TOKEN = 2, // type:string, value:device Token, location:NULL

	// load from disk
	LOGSEQ_TIMER_TASK_LOAD = 10, // type:error code, value:failed to load timing task from flash,
	                             // location:__load_timer_task_from_disk
	LOGSEQ_TIMER_TASK_LOAD_DPS_FAILD = 11, // type: decimal number, value:failed to request memory
	                                       // for dps, location:__load_timer_task_from_disk
	LOGSEQ_TIMER_TASK_LOAD_TIMER_ID_FAILD =
	    12, // type: decimal number, value:failed to request memory for timer id,
	        // location:__load_timer_task_from_disk

	// save to disk
	LOGSEQ_TIMER_TASK_SAVE = 20, // type:error code, value:failed to save timing task to flash,
	                             // location:__save_time_task_to_disk

	// update from cloud
	LOGSEQ_TIMER_TASK_UPDATE = 30,     // type:error code, value:failed to update timing task,
	                                   // location:__update_time_task_from_cloud
	LOGSEQ_TIMER_TASK_UPDATE_GET = 31, // type:error code, value:failed to update timing content,
	                                   // location:__update_time_task_from_cloud
	LOGSEQ_TIMER_TASK_UPDATE_GET_ITEM = 32, // type: decimal number, value:get timer index of the
	                                        // error, location:__update_time_task_from_cloud
	LOGSEQ_TIMER_TASK_UPDATE_INCREASE =
	    33, // type: decimal number, value:total timer number, location__increase_arr_size
	LOGSEQ_TIMER_TASK_UPDATE_ADD =
	    34, // type: decimal number, value:add timer index of the error, location:__add_task_to_arr
} TIMER_TASK_LOG_SEQ_E;

#endif
