/**
 * @file log_seq_http.h
 * @brief provide http log sequence
 * @version 0.1
 * @date 2020-03-27
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __LOG_SEQ_HTTP_H
#define __LOG_SEQ_HTTP_H

#include "log_seq.h"

// define log sequence object is "http", include this head file can recorde log sequence to "http"
// module
#define LOGSEQ_OBJ "http"

/**
 * @brief define http detail log sequence information
 *        user can easy know bug location from this sequence, which include type, value, location
 */
typedef enum {
	// generic
	LOGSEQ_HTTP_TIME = 0,  // type:string, value:system time, location:NULL
	LOGSEQ_HTTP_TOKEN = 1, // typeLstring, value:device Token, location:NULL

	// URL config
	LOGSEQ_HTTP_URL_CFG =
	    11, // type: error code, value: URL from URL config api, location:http_iot_dns_get_cloud_url

	// request
	LOGSEQ_HTTP_API = 20,          // type:string, value:API name, location:__iot_common_post
	LOGSEQ_HTTP_URL = 21,          // type:string, value:API name, location:__iot_common_post
	LOGSEQ_HTTP_DATA = 22,         // type:string, value:API name, location:__iot_common_post
	LOGSEQ_HTTP_OPEN_SESSION = 23, // type:error code, value:fail on connect http session,
	                               // locaiton：__http_inf_com_hanle->http_open_session
	LOGSEQ_HTTP_PREPARE_REQ = 24,  // type:error code, value:fail on prepare http request data,
	                               // location:__http_inf_com_hanle->http_prepare_req
	LOGSEQ_HTTP_SEND_REQUEST = 25, // type:error code, value:fail on http send request,
	                               // location:__http_inf_com_hanle->http_send_request

	// response
	LOGSEQ_HTTP_GET_RESP_HEAD = 30, // type:error code, value:get head of http responsed data fail,
	                                // location:http_get_response_hdr
	LOGSEQ_HTTP_CHK_RESP_RET =
	    31, // type:error code, value:return value of http response error, location:NULL
	LOGSEQ_HTTP_RECV_RESP = 32,       // type:error code, value:http failed to accept response data,
	                                  // location:__httpc_return_json_cb和__httpc_gw_common_cb
	LOGSEQ_HTTP_RECV_BODY_CJSON = 33, // type:error code, value:http received response data decode
	// failed,location:__httpc_return_json_cb和__httpc_gw_common_cb
	LOGSEQ_HTTP_RECV_DECODE =
	    34, // type:error code, value:http decryption failed, location:__httpc_gw_common_cb
	LOGSEQ_HTTP_RECV_CONTENT = 35, // type:error code, value:http content error, location:NULL
	LOGSEQ_HTTP_RECV_DATA = 36,    // type:error code, value:http data error, location:NULL

	LOGSEQ_HTTP_DB_RECOVERY = 100, // 类型：错误码，值：KV被破坏并恢复，位置：NULL
} HTTP_LOG_SEQ_E;

#endif
