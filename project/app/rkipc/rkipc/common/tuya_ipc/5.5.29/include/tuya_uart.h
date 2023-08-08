/**
 * @file tuya_uart.h
 * @brief Common process - driver uart
 * @version 0.1
 * @date 2019-08-20
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_UART_H__
#define __TUYA_UART_H__

#include "tuya_driver.h"
#include "tuya_uart_legacy.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TUYA_UART_8N1_CFG(__UART, __BAUDRATE, __BUFSZ, __FLAG)                                     \
	(__UART)->cfg.baudrate = __BAUDRATE;                                                           \
	(__UART)->cfg.flag = TUYA_DRV_INT_RX_FLAG | __FLAG;                                            \
	(__UART)->cfg.bufsz = __BUFSZ;                                                                 \
	(__UART)->cfg.databits = TUYA_UART_DATA_BIT8;                                                  \
	(__UART)->cfg.stopbits = TUYA_UART_STOP_BIT1;                                                  \
	(__UART)->cfg.parity = TUYA_UART_PARITY_NONE

#define TUYA_UART_8N1_CFG_INIT(__CFG, __BAUDRATE, __BUFSZ, __FLAG)                                 \
	(__CFG)->baudrate = __BAUDRATE;                                                                \
	(__CFG)->flag = TUYA_DRV_INT_RX_FLAG | __FLAG;                                                 \
	(__CFG)->bufsz = __BUFSZ;                                                                      \
	(__CFG)->databits = TUYA_UART_DATA_BIT8;                                                       \
	(__CFG)->stopbits = TUYA_UART_STOP_BIT1;                                                       \
	(__CFG)->parity = TUYA_UART_PARITY_NONE

typedef enum {
	TUYA_UART0 = 0x00,
	TUYA_UART1,
	TUYA_UART2,
	TUYA_UART3,
} tuya_uart_port_t;

typedef enum {
	TUYA_UART_BAUDRATE_300 = 300,
	TUYA_UART_BAUDRATE_600 = 600,
	TUYA_UART_BAUDRATE_1200 = 1200,
	TUYA_UART_BAUDRATE_2400 = 2400,
	TUYA_UART_BAUDRATE_4800 = 4800,
	TUYA_UART_BAUDRATE_9600 = 9600,
	TUYA_UART_BAUDRATE_19200 = 19200,
	TUYA_UART_BAUDRATE_38400 = 38400,
	TUYA_UART_BAUDRATE_57600 = 57600,
	TUYA_UART_BAUDRATE_74880 = 74880,
	TUYA_UART_BAUDRATE_115200 = 115200,
	TUYA_UART_BAUDRATE_230400 = 230400,
	TUYA_UART_BAUDRATE_460800 = 460800,
	TUYA_UART_BAUDRATE_921600 = 921600,
	TUYA_UART_BAUDRATE_1500000 = 1500000,
	TUYA_UART_BAUDRATE_1843200 = 1843200,
	TUYA_UART_BAUDRATE_3686400 = 3686400,
} tuya_uart_baudrate_t;

typedef enum {
	TUYA_UART_DATA_BIT5 = 0x05,
	TUYA_UART_DATA_BIT6 = 0x06,
	TUYA_UART_DATA_BIT7 = 0x07,
	TUYA_UART_DATA_BIT8 = 0x08,
} tuya_uart_databits_t;

typedef enum {
	TUYA_UART_STOP_BIT1 = 0x01,
	TUYA_UART_STOP_BIT1_5 = 0x02,
	TUYA_UART_STOP_BIT2 = 0x03,
} tuya_uart_stopbits_t;

typedef enum {
	TUYA_UART_PARITY_NONE = 0,
	TUYA_UART_PARITY_ODD = 1,
	TUYA_UART_PARITY_EVEN = 2,
} tuya_uart_parity_t;

typedef enum {
	TUYA_UART_INT_TX_EVENT = 1,
	TUYA_UART_INT_RX_EVENT = 2,
} tuya_uart_event_t;

enum tuya_uart_rx_isr_mode {
	TUYA_UART_RX_ISR_FIFO_MODE = 1, // rx has a fifo, need loop read until to failed
};
typedef uint8_t tuya_uart_rx_isr_mode_t;

typedef enum {
	TUYA_UART_FLOWCTRL_NONE = 0,
	TUYA_UART_FLOWCTRL_ENABLE = 1,
} tuya_uart_flowctrl_t;

typedef struct {
	uint16_t flag;
	uint16_t bufsz;
	tuya_uart_baudrate_t baudrate;
	tuya_uart_databits_t databits;
	tuya_uart_stopbits_t stopbits;
	tuya_uart_parity_t parity;
	tuya_uart_flowctrl_t flowctrl;
} tuya_uart_cfg_t;

typedef enum {
	TUYA_UART_RXBUF_SIZE_CMD = TUYA_DRV_CUSTOM_CMD,
	TUYA_UART_FLUSH_CMD,
	TUYA_UART_RECONFIG_CMD,
} tuya_uart_cmd_t;

typedef struct tuya_uart tuya_uart_t;

typedef void (*tuya_uart_isr_cb)(tuya_uart_t *uart, uint32_t event);

typedef struct {
	int (*tx_finish)(tuya_uart_t *uart, void *buffer);
	int (*rx_notify)(tuya_uart_t *uart, uint16_t size);
} tuya_uart_cb_t;

typedef struct {
	int (*init)(tuya_uart_t *uart, tuya_uart_cfg_t *cfg);
	int (*read_byte)(tuya_uart_t *uart, uint8_t *byte);
	int (*write_byte)(tuya_uart_t *uart, uint8_t byte);
	int (*control)(tuya_uart_t *uart, uint8_t cmd, void *arg);
	int (*deinit)(tuya_uart_t *uart);
} tuya_uart_ops_t;

struct tuya_uart {
	tuya_drv_node_t node;
	tuya_uart_cfg_t cfg;
	tuya_uart_cb_t cb;
	tuya_uart_ops_t *ops;
	void *rxfifo;
};

/**
 * @brief uart init
 *
 * @param[in] uart refer to tuya_uart_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_uart_init(tuya_uart_t *uart);

/**
 * @brief uart read
 *
 * @param[in] uart refer to tuya_uart_t
 * @param[out] data read buf
 * @param[in] len read length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_uart_read(tuya_uart_t *uart, void *data, uint16_t len);

/**
 * @brief uart write
 *
 * @param[in] uart refer to tuya_uart_t
 * @param[in] data write buf
 * @param[in] len write length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_uart_write(tuya_uart_t *uart, void *data, uint16_t len);

/**
 * @brief uart control
 *
 * @param[in] uart refer to tuya_uart_t
 * @param[in] cmd control command
 * @param[in] arg command argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_uart_control(tuya_uart_t *uart, uint8_t cmd, void *arg);

/**
 * @brief uart deinit
 *
 * @param[in] uart refer to tuya_uart_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_uart_deinit(tuya_uart_t *uart);

/**
 * @brief uart data size
 *
 * @param[in] uart refer to tuya_uart_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_uart_int_bufsize(tuya_uart_t *uart);

/**
 * @brief uart read with timeout
 *
 * @param[in] uart refer to tuya_uart_t
 * @param[out] data read buf
 * @param[in] len read length
 * @param[in] timeout read timeout
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_uart_read_timeout(tuya_uart_t *uart, void *data, uint16_t len, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
