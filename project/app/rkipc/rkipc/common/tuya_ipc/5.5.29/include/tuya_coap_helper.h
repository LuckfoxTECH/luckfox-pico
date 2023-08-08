/**
 * @file tuya_coap_helper.h
 * @brief TUYA COAP service
 * @version 0.1
 * @date 2019-08-28
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef _COAP_SERVER_H
#define _COAP_SERVER_H

#include "tuya_cloud_types.h"
#include "tuya_os_adapter.h"

#include "coap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef PVOID_T TY_COAP_SERVER_HANLDER;
typedef OPERATE_RET (*ty_coap_init_resource_cb)(IN TY_COAP_SERVER_HANLDER handler);

/**
 * @brief Create COAP wrapper instance
 *
 * @param[out] p_handler Handler of COAP wrapper instance
 * @param[in] port Port of COAP server
 * @param[in] init_res_cb Callback after COAP wrapper inited
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_coap_server_start(OUT TY_COAP_SERVER_HANLDER *p_handler, IN CONST UINT_T port,
                                 ty_coap_init_resource_cb init_res_cb);

/**
 * @brief Add COAP resource
 *
 * @param[out] handler Handler of COAP wrapper instance
 * @param[in] p_uri COAP uri
 * @param[in] post_cb Callback when POST
 * @param[in] get_cb Callback when GET
 * @param[in] put_cb Callback when PUT
 * @param[in] delete_cb Callback when DELETE
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_coap_add_resource(IN TY_COAP_SERVER_HANLDER handler, IN CONST CHAR_T *p_uri,
                                 coap_method_handler_t post_cb, coap_method_handler_t get_cb,
                                 coap_method_handler_t put_cb, coap_method_handler_t delete_cb);

/**
 * @brief Definition of COAP option
 */
typedef struct {
	/** option type */
	SHORT_T type;
	/** option length */
	UINT_T length;
	/** option data */
	CHAR_T *p_data;
} TY_COAP_OPTION_S;

/**
 * @brief Send COAP msg
 *
 * @param[in] server_handler Handler created by ty_coap_server_start
 * @param[in] url Coap url
 * @param[in] upload_mode Upload mode
 * @param[in] msg_type Msg type
 * @param[in] data Buffer of data buffer
 * @param[in] option_arr Options list
 * @param[in] arr_size Count of options
 * @param[in] handler Callback when msg recv
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_coap_send_custom_msg(IN TY_COAP_SERVER_HANLDER server_handler, CHAR_T *url,
                                    UCHAR_T upload_mode, UCHAR_T msg_type, CHAR_T *data,
                                    TY_COAP_OPTION_S *option_arr, UINT_T arr_size,
                                    coap_response_handler_t handler);

#ifdef __cplusplus
}
#endif
#endif
