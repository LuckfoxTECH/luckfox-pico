/**
 * @file http_inf.h
 * @brief Wrapper of HTTP request and response
 * @version 0.1
 * @date 2016-10-28
 *
 * @copyright Copyright 2016-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _HTTP_INF_H
#define _HTTP_INF_H

#include "httpc.h"
#include "log_seq.h"
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This API is used to handle HTTP response content
 *
 * @param[in] hand http_session_t
 * @param[in] buf Buffer of HTTP response content
 * @param[in] len length of buf
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef INT_T (*HTTP_DATA_RECV)(PVOID_T hand, PVOID_T buf, UINT_T len);

/**
 * @brief Definition of HTTP request/response structure
 */
typedef struct {
	/** chunk is enabled or not */
	BOOL_T chunked;
	/** if(chunked == FALSE) then valid */
	UINT_T content_len;
	/** http_session_t */
	PVOID_T hand;
	/** system auto fill */
	HTTP_DATA_RECV recv;
	/** private data */
	PVOID_T *pri_data;
	/** status code of HTTP response */
	INT_T status_code;
	/** decode key of HTTP response content */
	CHAR_T *p_decode_key;
} HTTP_INF_H_S;

/**
 * @brief This API is used to handle customized HTTP response
 *
 * @param[in] hand Pointer to HTTP_INF_H_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*HTTP_INF_CB)(HTTP_INF_H_S *hand);

/**
 * @brief Definition of HTTP URL param structure
 */
typedef struct {
	/** max count of URL params */
	BYTE_T total;
	/** used count of URL params */
	BYTE_T cnt;
	/** pointer to URL param */
	CHAR_T *pos[0];
} HTTP_PARAM_H_S;

#define DEF_URL_LEN 1024 // max length of HTTP url
/**
 * @brief Definition of HTTP URL structure
 */
typedef struct {
	/** param handle */
	HTTP_PARAM_H_S *param_h;
	/** param insert pos */
	CHAR_T *param_in;
	/** head_size == "url?" or "url" */
	USHORT_T head_size;
	/** bufer len */
	USHORT_T buf_len;
	/** "url?key=value" + "kev=value statistics" */
	CHAR_T buf[0];
} HTTP_URL_H_S;

/**
 * @brief This API is used to GET HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] callback Handler of HTTP response
 * @param[in] pri_data Private data used by HTTP request
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
#define http_inf_client_get(url, callback, pri_data)                                               \
	http_inf_client_get_with_head(url, callback, pri_data, NULL, NULL)

/**
 * @brief This API is used to GET HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] callback Handler of HTTP response
 * @param[in] pri_data Private data used by HTTP request
 * @param[in] add_head_cb Hook before HTTP request sent, to add more HTTP header
 * @param[in] add_head_data Header data of add_head_cb
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_get_with_head(IN CONST CHAR_T *url, IN CONST HTTP_INF_CB callback,
                                          INOUT PVOID_T *pri_data, IN HTTP_HEAD_ADD_CB add_head_cb,
                                          IN VOID *add_head_data);

/**
 * @brief This API is used to GET HTTP file
 *
 * @param[in] url URL of HTTP request
 * @param[in] callback Handler of HTTP response
 * @param[in] pri_data Private data used by HTTP request
 * @param[in] file_offset Offset of file content
 * @param[in] file_size Max size to get from file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_get_file(IN CONST CHAR_T *url, IN CONST HTTP_INF_CB callback,
                                     INOUT PVOID_T *pri_data, IN CONST UINT_T file_offset,
                                     IN CONST UINT_T file_size);

#define http_inf_client_post(url, callback, data, len, add_head_cb, add_head_data, p_decode_key,   \
                             pri_data)                                                             \
	http_inf_client_post_field(                                                                    \
	    url, callback, data, len, add_head_cb, add_head_data, p_decode_key, pri_data,              \
	    STANDARD_HDR_FLAGS | HDR_ADD_CONN_KEEP_ALIVE | HDR_ADD_CONTENT_TYPE_FORM_URLENCODE)

/**
 * @brief This API is used to POST HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] callback Handler of HTTP response
 * @param[in] data data of HTTP request
 * @param[in] len length of HTTP request data
 * @param[in] add_head_cb Hook before HTTP request sent, to add more HTTP header
 * @param[in] add_head_data Header data of add_head_cb
 * @param[in] p_decode_key Decode key of HTTP content
 * @param[in] pri_data Private data used by HTTP request
 * @param[in] field_flags The http_hdr_field_sel_t, OR of zero or more flags
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_post_field(IN CONST CHAR_T *url, IN CONST HTTP_INF_CB callback,
                                       IN CONST BYTE_T *data, IN CONST UINT_T len,
                                       IN HTTP_HEAD_ADD_CB add_head_cb, IN VOID *add_head_data,
                                       IN CONST CHAR_T *p_decode_key, INOUT PVOID_T *pri_data,
                                       IN CONST http_hdr_field_sel_t field_flags);

#define http_inf_client_post_raw(url, len, p_inf)                                                  \
	http_inf_client_post_raw_field(url, len, p_inf,                                                \
	                               HRD_ADD_HTTP_RAW | STANDARD_HDR_FLAGS |                         \
	                                   HDR_ADD_CONN_KEEP_ALIVE | HDR_ADD_CONTENT_TYPE_JSON)

/**
 * @brief This API is used to POST raw HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] len Max length of HTTP response data
 * @param[in] p_inf Pointer to HTTP_INF_H_S
 * @param[in] field_flags The http_hdr_field_sel_t, OR of zero or more flags
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_post_raw_field(IN CONST CHAR_T *url, IN CONST UINT_T len,
                                           INOUT HTTP_INF_H_S *p_inf,
                                           IN http_hdr_field_sel_t field_flags);

/**
 * @brief This API is used to GET raw HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] p_inf Pointer to HTTP_INF_H_S
 * @param[in] offset Offset of file content
 * @param[in] total_len Max size to get from file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
#define http_inf_client_get_raw(url, p_inf, offset, total_len)                                     \
	http_inf_client_get_raw_with_head(url, p_inf, offset, total_len, NULL, NULL)

/**
 * @brief This API is used to GET raw HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] p_inf Pointer to HTTP_INF_H_S
 * @param[in] offset Offset of file content
 * @param[in] total_len Max size to get from file
 * @param[in] add_head_cb Hook before HTTP request sent, to add more HTTP header
 * @param[in] add_head_data Header data of add_head_cb
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_get_raw_with_head(IN CONST CHAR_T *url, INOUT HTTP_INF_H_S *p_inf,
                                              IN UINT_T offset, IN UINT_T total_len,
                                              IN HTTP_HEAD_ADD_CB add_head_cb,
                                              IN VOID *add_head_data);

/**
 * @brief This API is used to PUT raw HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] len Max length of HTTP response data
 * @param[in] p_inf Pointer to HTTP_INF_H_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_put_raw(IN CONST CHAR_T *url, IN INT_T len, INOUT HTTP_INF_H_S *p_inf);

/**
 * @brief This API is used to write raw HTTP request content
 *
 * @param[in] p_inf Pointer to HTTP_INF_H_S
 * @param[in] p_data Buffer to write of HTTP request
 * @param[in] buf_len Max length of HTTP request data
 * @param[in] chunked Chunk is enabled or not
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T http_inf_write_content_raw(INOUT HTTP_INF_H_S *p_inf, IN CONST BYTE_T *p_data,
                                 IN CONST UINT_T len, BOOL_T chunked);

/**
 * @brief This API is used to read raw HTTP response header
 *
 * @param[in,out] p_inf Pointer to HTTP_INF_H_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET httpc_inf_read_header_raw(INOUT HTTP_INF_H_S *p_inf);

/**
 * @brief This API is used to read raw HTTP response content
 *
 * @param[in] p_inf Pointer to HTTP_INF_H_S
 * @param[in] buf Buffer to read HTTP response
 * @param[in] buf_len Max length of HTTP response data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T httpc_inf_read_content_raw(IN HTTP_INF_H_S *p_inf, IN BYTE_T *buf, IN CONST UINT_T buf_len);

/**
 * @brief This API is used to close raw HTTP request
 *
 * @param[in] p_inf Pointer to HTTP_INF_H_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_close_raw(INOUT HTTP_INF_H_S *p_inf);

/**
 * @brief This API is used to POST HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] callback Handler of HTTP response
 * @param[in] data data of HTTP request
 * @param[in] len length of HTTP request data
 * @param[in] add_head_cb Hook before HTTP request sent, to add more HTTP header
 * @param[in] add_head_data Header data of add_head_cb
 * @param[in] pri_data Private data used by HTTP request
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_add_head_cb_post(IN CONST char *url, IN CONST HTTP_INF_CB callback,
                                             IN CONST BYTE_T *data, IN CONST UINT_T len,
                                             IN HTTP_HEAD_ADD_CB add_head_cb,
                                             IN VOID *add_head_data, INOUT PVOID_T *pri_data);

/**
 * @brief This API is used to PUT HTTP request/response
 *
 * @param[in] url URL of HTTP request
 * @param[in] callback Handler of HTTP response
 * @param[in] data data of HTTP request
 * @param[in] len length of HTTP request data
 * @param[in] add_head_cb Hook before HTTP request sent, to add more HTTP header
 * @param[in] add_head_data Header data of add_head_cb
 * @param[in] pri_data Private data used by HTTP request
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_put(IN CONST CHAR_T *url, IN CONST HTTP_INF_CB callback,
                                IN CONST BYTE_T *data, IN CONST UINT_T len,
                                IN HTTP_HEAD_ADD_CB add_head_cb, IN VOID *add_head_data,
                                INOUT PVOID_T *pri_data);

/**
 * @brief This API is used to PUT HTTP request/response with customized content
 *
 * @param[in] url URL of HTTP request
 * @param[in] callback Handler of HTTP response
 * @param[in] p_ctx Context of HTTP request customized
 * @param[in] add_head_cb Hook before HTTP request sent, to add more HTTP header
 * @param[in] add_head_data Header data of add_head_cb
 * @param[in] pri_data Private data used by HTTP request
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET http_inf_client_put_custom_content(IN CONST CHAR_T *url, IN CONST HTTP_INF_CB callback,
                                               IN http_custom_content_ctx_s *p_ctx,
                                               IN HTTP_HEAD_ADD_CB add_head_cb,
                                               IN VOID *add_head_data, INOUT PVOID_T *pri_data);

/**
 * @brief Create url of HTTP request
 *
 * @param[in] buf_len max lenth of URL(include params)
 * @param[in] param_cnt max count of url params
 *
 * @return NULL on error. Pointer to HTTP_URL_H_S on success
 */
HTTP_URL_H_S *create_http_url_h(IN CONST USHORT_T buf_len, IN CONST USHORT_T param_cnt);

/**
 * @brief Free url of HTTP request
 *
 * @param[in] hu_h A pointer that points to the structure returned from the call to
 * create_http_url_h
 *
 */
VOID del_http_url_h(IN HTTP_URL_H_S *hu_h);

/**
 * @brief Initiaze url of HTTP request
 *
 * @param[in,out] hu_h A pointer that points to the structure returned from the call to
 * create_http_url_h
 * @param[in] url_h HTTP url
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET fill_url_head(INOUT HTTP_URL_H_S *hu_h, IN CONST CHAR_T *url_h);

/**
 * @brief Add key/value param to HTTP url
 *
 * @param[in,out] hu_h A pointer that points to the structure returned from the call to
 * create_http_url_h
 * @param[in] key Key to be added to url
 * @param[in] value Value to be added to url
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET fill_url_param(INOUT HTTP_URL_H_S *hu_h, IN CONST CHAR_T *key, IN CONST CHAR_T *value);

#ifdef __cplusplus
}
#endif

#endif
