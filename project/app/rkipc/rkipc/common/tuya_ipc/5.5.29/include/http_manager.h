/**
 * @file http_manager.h
 * @brief wrapper for HTTP session manager
 * @version 0.1
 * @date 2016-01-11
 *
 * @copyright Copyright 2016-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __HTTP_MANAGER_H_
#define __HTTP_MANAGER_H_

#include "httpc.h"
#include "tuya_cloud_types.h"
#include "tuya_hal_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_HTTP_SESSION_NUM 16            // max number of active session
#define INVALID_HTTP_SESSION_ID 0xFFFFFFFF // invalid HTTP session handle
#define MAX_HTTP_URL_LEN 256               // max len of url

/**
 * @brief Definition of HTTP session state
 */
typedef enum {
	HTTP_FREE = 0,   // session is free
	HTTP_DISCONNECT, // session is disconnected
	HTTP_CONNECTING, // session is connecting
	HTTP_CONNECTED,  // session is connected and ready to send/recv
	HTTP_UPLOADING,  // session has sent data and is ready to recv
} E_HTTP_SESSION_STATE;

/**
 * @brief The HTTP session's Request structure
 */
typedef struct {
	/** internal HTTP session */
	http_session_t s;

	/** persistent or not */
	BOOL_T is_persistent;

	/** hdr field flags */
	http_hdr_field_sel_t flags;

	/** url content */
	CHAR_T url[MAX_HTTP_URL_LEN];

	/** http request */
	http_req_t req;

	/** session state, please refer to enum E_HTTP_SESSION_STATE */
	E_HTTP_SESSION_STATE state;
} S_HTTP_SESSION;

typedef S_HTTP_SESSION *SESSION_ID;

/**
 * @brief This API is used to create HTTP session
 *
 * @param[in] url URL of HTTP session, max len of URL is 256
 * @param[in] is_persistent Session is persistent or not
 *
 * @return SESSION_ID on success, NULL on error
 */
typedef SESSION_ID (*FUNC_HTTP_SESSION_CREATE)(IN CONST CHAR_T *url, BOOL_T is_persistent);

/**
 * @brief This API is used to send HTTP session request
 *
 * @param[in] session Session returned from the call to FUNC_HTTP_SESSION_CREATE
 * @param[in] req The http_req_t structure filled up with appropriate parameters
 * @param[in] field_flags The http_hdr_field_sel_t, OR of zero or more flags
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*FUNC_HTTP_SESSION_SEND)(IN CONST SESSION_ID session, IN CONST http_req_t *req,
                                              http_hdr_field_sel_t field_flags);

/**
 * @brief This API is used to recv response header from HTTP session
 *
 * @param[in] session Session returned from the call to FUNC_HTTP_SESSION_CREATE
 * @param[in,out] resp Pointer to a pointer of type http_resp_t .The
 * structure will be allocated by the callee. Note that the caller does
 * not need to free the structure allocated and returned from this
 * API. The allocation and free is done by the callee automatically
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*FUNC_HTTP_SESSION_RECEIVE)(SESSION_ID session, http_resp_t **resp);

/**
 * @brief This API is used to destroy HTTP session
 *
 * @param[in] session Session returned from the call to FUNC_HTTP_SESSION_CREATE
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*FUNC_HTTP_SESSION_DESTORY)(SESSION_ID session);

/**
 * @brief This API is used to recv response content from HTTP session.
 *
 * @param[in] session Session returned from the call to FUNC_HTTP_SESSION_CREATE
 * @param[in] pResp The http_resp_t structure returned from the call to FUNC_HTTP_SESSION_RECEIVE
 * @param[in, out] pDataOut Caller allocated buffer
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*FUNC_HTTP_SESSION_RECEIVE_DATA)(SESSION_ID session, http_resp_t *pResp,
                                                      BYTE_T **pDataOut);

/**
 * @brief The HTTP session manager structure.
 */
typedef struct {
	/** session resources */
	S_HTTP_SESSION *session[MAX_HTTP_SESSION_NUM];
	/** exclusive access to session resources*/
	MUTEX_HANDLE mutex;

	/** session manager inited or not */
	BOOL_T inited;

	/** handler to create HTTP session */
	FUNC_HTTP_SESSION_CREATE create_http_session;
	/** handler to send HTTP session request */
	FUNC_HTTP_SESSION_SEND send_http_request;
	/** handler to recv response header from HTTP session */
	FUNC_HTTP_SESSION_RECEIVE receive_http_response;
	/** handler to destroy HTTP session */
	FUNC_HTTP_SESSION_DESTORY destory_http_session;
	/** handler to recv response content from HTTP session */
	FUNC_HTTP_SESSION_RECEIVE_DATA receive_http_data;
} S_HTTP_MANAGER;

/**
 * @brief Retrieve instance of HTTP session manager
 *
 * @note HTTP manager is singleton, multiple call to this API will return the same instance
 *
 * @return NULL on error. Instance on success, please refer to S_HTTP_MANAGER
 */
S_HTTP_MANAGER *get_http_manager_instance(VOID_T);

#ifdef __cplusplus
}
#endif

#endif
