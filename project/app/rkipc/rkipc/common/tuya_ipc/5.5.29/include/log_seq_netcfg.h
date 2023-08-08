/**
 * @file log_seq_netcfg.h
 * @brief provide netcfg log sequence
 * @version 0.1
 * @date 2020-03-27
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __LOG_SEQ_NETCFG_H
#define __LOG_SEQ_NETCFG_H

#include "log_seq.h"

// define log sequence object is "netcfg", include this head file can recorde log sequence to
// "netcfg" module
#define LOGSEQ_OBJ "netcfg"

/**
 * @brief define network configuration detail log sequence information
 *        user can easy know bug location from this sequence, which include type, value, location
 */
typedef enum {
	// generic
	LOGSEQ_NETCFG_TIME = 0,  // type:string, value:system time, location:NULL
	LOGSEQ_NETCFG_TOKEN = 1, // type:string, value:device Token, location:NULL
	LOGSEQ_NETCFG_TYPE = 9,  // type:string, value:network configure type, location:NULL

	// netcfg
	LOGSEQ_NETCFG_FAIL =
	    10, // type:error code, value:reasons for network configuration failure, location:NULL

	// route
	LOGSEQ_NETCFG_LAST_STATUS =
	    20, // type: decimal number, value:previous network status, location:get_wf_gw_nw_status
	LOGSEQ_NETCFG_NEW_STATUS =
	    21, // type: decimal number, value: new network status, location:get_wf_gw_nw_status
	LOGSEQ_NETCFG_OTHERS_START =
	    22, // type: decimal number, value: exit type, location:__gw_net_conn_cb
} NETCFG_LOG_SEQ_E;

#endif
