/**
 * @file tuya_uart.h
 * @brief Common process - driver uart
 * @version 0.1
 * @date 2019-08-20
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_UART_LEGACY_H__
#define __TUYA_UART_LEGACY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

typedef enum {
	TYU_RATE_300 = 300,
	TYU_RATE_600 = 600,
	TYU_RATE_1200 = 1200,
	TYU_RATE_2400 = 2400,
	TYU_RATE_4800 = 4800,
	TYU_RATE_9600 = 9600,
	TYU_RATE_19200 = 19200,
	TYU_RATE_38400 = 38400,
	TYU_RATE_57600 = 57600,
	TYU_RATE_74880 = 74880,
	TYU_RATE_115200 = 115200,
	TYU_RATE_230400 = 230400,
	TYU_RATE_460800 = 460800,
	TYU_RATE_921600 = 921600,
	TYU_RATE_1500000 = 1500000,
	TYU_RATE_1843200 = 1843200,
	TYU_RATE_3686400 = 3686400,
} TY_UART_BAUD_E;

typedef enum {
	TY_UART0 = 0x00,
	TY_UART1,
	TY_UART_NUM,
} TY_UART_PORT_E;

typedef enum {
	TYWL_5B = 0x00,
	TYWL_6B = 0x01,
	TYWL_7B = 0x02,
	TYWL_8B = 0x03,
} TY_DATA_BIT_E;

typedef enum {
	TYS_STOPBIT1 = 0x01,
	TYS_STOPBIT1_5 = 0x02,
	TYS_STOPBIT2 = 0x03,
} TY_STOPBITS_E;

typedef enum {
	TYP_NONE = 0, // parity disable
	TYP_ODD = 1,  // odd parity enable
	TYP_EVEN = 2, // even paroty enable
} TY_PARITY_E;

/**
 * @brief uart init
 *
 * @param[in] port refer to TY_UART_PORT_E
 * @param[in] badu refer to TY_UART_BAUD_E
 * @param[in] bits refer to TY_DATA_BIT_E
 * @param[in] parity refer to TY_PARITY_E
 * @param[in] stop refer to TY_STOPBITS_E
 * @param[in] bufsz buf size
 * @param[in] unblock block flag
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_uart_init(IN CONST TY_UART_PORT_E port, IN CONST TY_UART_BAUD_E badu,
                         IN CONST TY_DATA_BIT_E bits, IN CONST TY_PARITY_E parity,
                         IN CONST TY_STOPBITS_E stop, IN CONST UINT_T bufsz,
                         IN CONST BOOL_T unblock);

/**
 * @brief uart init
 *
 * @param[in] port refer to TY_UART_PORT_E
 * @param[in] badu refer to TY_UART_BAUD_E
 * @param[in] bits refer to TY_DATA_BIT_E
 * @param[in] parity refer to TY_PARITY_E
 * @param[in] stop refer to TY_STOPBITS_E
 * @param[in] bufsz buf size
 * @param[in] unblock block flag
 * @param[in] flowctl
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_uart_flowctrl_init(IN CONST TY_UART_PORT_E port, IN CONST TY_UART_BAUD_E badu,
                                  IN CONST TY_DATA_BIT_E bits, IN CONST TY_PARITY_E parity,
                                  IN CONST TY_STOPBITS_E stop, IN CONST UINT_T bufsz,
                                  IN CONST BOOL_T unblock, IN CONST BOOL_T flowctl);

/**
 * @brief uart free
 *
 * @param[in] port refer to TY_UART_PORT_E
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ty_uart_free(IN CONST TY_UART_PORT_E port);

/**
 * @brief uart send data
 *
 * @param[in] port refer to TY_UART_PORT_E
 * @param[in] data send buf
 * @param[in] len send length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID ty_uart_send_data(IN CONST TY_UART_PORT_E port, IN CONST BYTE_T *data, IN CONST UINT_T len);

/**
 * @brief uart read data
 *
 * @param[in] port refer to TY_UART_PORT_E
 * @param[out] buf read buf
 * @param[in] len read length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT_T ty_uart_read_data(IN CONST TY_UART_PORT_E port, OUT BYTE_T *buf, IN CONST UINT_T len);

/**
 * @brief uart data size
 *
 * @param[in] port refer to TY_UART_PORT_E
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T ty_uart_read_data_size(IN CONST TY_UART_PORT_E port);

#ifdef __cplusplus
}
#endif
#endif
