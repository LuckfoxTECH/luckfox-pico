/**
 * @file mf_test.h
 * @brief Common process - mf test
 * @version 0.1
 * @date 2017-11-06
 *
 * @copyright Copyright 2017-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __MF_TEST_H
#define __MF_TEST_H

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"
#include "tuya_cloud_wifi_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief callback for URAT init function
 *
 * @param[in] baud data bits:8bit,parity:none,stop bits:1
 * @param[in] bufsz Size of buff
 *
 */
typedef VOID (*MF_UART_INIT_CB)(UINT_T baud, UINT_T bufsz);

/**
 * @brief callback for URAT send function
 *
 * @param[in] data Send data buf
 * @param[in] len Send data buf len
 *
 */
typedef VOID (*MF_UART_SEND_CB)(IN BYTE_T *data, IN CONST UINT_T len);

/**
 * @brief callback for URAT receive function
 *
 * @param[in] len buf len
 * @param[out] buf Received data buf
 *
 * @return read data len
 */
typedef UINT_T (*MF_UART_RECV_CB)(OUT BYTE_T *buf, IN CONST UINT_T len);

/**
 * @brief callback for GPIO test
 *
 * @param[in] in The data in
 * @param[out] out The data out
 *
 * @note if TY_GPIO_TEST_V2 was not defined,then will has no [in]\[out]
 *
 * @return TRUE/FALSE,GPIO test success or not
 */
#if defined(TY_GPIO_TEST_V2) && (TY_GPIO_TEST_V2 == 1)
typedef BOOL_T (*MF_GPIO_TEST_CB)(IN CONST CHAR_T *in, OUT CHAR_T *out);
#else
typedef BOOL_T (*MF_GPIO_TEST_CB)(VOID);
#endif

/**
 * @brief callback for preposed GPIO test
 *
 * @return TRUE/FALSE
 */
typedef BOOL_T (*MF_PRE_GPIO_TEST_CB)(VOID);

/**
 * @brief callback for user product test
 *
 * @param[in] cmd Test command
 * @param[in] data Test data
 * @param[in] len Test data len
 * @param[out] ret_data Test return data
 * @param[out] ret_len Test return data len
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*MF_USER_PRODUCT_TEST_CB)(USHORT_T cmd, UCHAR_T *data, UINT_T len,
                                               OUT UCHAR_T **ret_data, OUT USHORT_T *ret_len);

/**
 * @brief callback for UART free function
 *
 */
typedef VOID (*MF_UART_FREE_CB)(VOID);

/**
 * @brief callback for user
 *
 */
typedef VOID (*MF_USER_CALLBACK)(VOID);

/**
 * @brief Definition of mf test init configure
 */
typedef struct {
	MF_UART_INIT_CB uart_init;
	MF_UART_FREE_CB uart_free;
	MF_UART_SEND_CB uart_send;
	MF_UART_RECV_CB uart_recv;
	MF_GPIO_TEST_CB gpio_test;
	MF_USER_PRODUCT_TEST_CB mf_user_product_test;
	MF_USER_CALLBACK user_callback;
	MF_USER_CALLBACK user_enter_mf_callback;
	MF_PRE_GPIO_TEST_CB user_pre_gpio_test;
} MF_IMPORT_INTF_S;

/**
 * @brief callback after write user param finish
 *
 * @param[in] data The data user in
 * @param[in] len Data len
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*MF_USER_PARAM_CB)(IN CONST BYTE_T *data, IN CONST UINT_T len);

/**
 * @brief mf test init fucntion
 *
 * @param[in] intf mf test configure data
 * @param[in] file_name APP bin name
 * @param[in] file_ver User Software version
 * @param[in] wrMacEn Need wirte MAC or not
 *
 * @note some fireware need write MAC when mf test
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mf_init(IN CONST MF_IMPORT_INTF_S *intf, IN CONST CHAR_T *file_name,
                    IN CONST CHAR_T *file_ver, IN BOOL_T wrMacEn);

/**
 * @brief judge if mf test is timeout
 *
 * @return TRUE/FALSE
 */
BOOL_T mf_test_is_timeout(VOID);

/**
 * @brief callback for app product test
 *
 * @param[in] flag
 * @param[in] rssi
 *
 */
typedef VOID_T (*APP_PROD_CB)(BOOL_T flag, SCHAR_T rssi);

/**
 * @brief set callback function for product test
 *
 * @param[in] cb
 *
 */
VOID mf_user_param_cb_set(MF_USER_PARAM_CB cb);

/**
 * @brief The entry of product test
 * @param[in] cmd Test command
 * @param[in] data Test data
 * @param[in] len Test data len
 *
 */
VOID mf_cmd_product_send(IN CONST USHORT_T cmd, IN CONST BYTE_T *data, IN CONST UINT_T len);

/**
 * @brief ignore mf test closed flag
 *
 */
VOID mf_test_ignore_close_flag(VOID);

/***********************************************************
 *  Function: mf_test_set_thread_priority
 *  Input: none
 *  Output: none
 *  Return: none
 ***********************************************************/
VOID mf_test_set_thread_priority(UINT_T pri);

/**
 * @brief get facpin
 *
 * @param[out] facpin
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET mf_test_facpin_get(OUT CHAR_T facpin[20 + 1]);

/**
 * @brief close mf test,save the flag in flash
 *
 * @param[in] mf_close Close flag
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_mf_test_close_if_write(IN BOOL_T mf_close);

/**
 * @brief read the flag in flash,to check if mf test closed
 *
 * @return TRUE/FLASE
 */
BOOL_T wd_mf_test_close_if_read(VOID);
#ifdef __cplusplus
}
#endif
#endif
