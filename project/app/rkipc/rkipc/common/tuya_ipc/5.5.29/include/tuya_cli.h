/**
 * @file tuya_cli.h
 * @brief Common process - cli
 * @version 0.1
 * @date 2020-05-13
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __SVC_ASTRO_TIMER__
#define __SVC_ASTRO_TIMER__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
 * @brief callback for one cli command
 *
 * @param[in] argc The number of argumnet
 * @param[in] argv The point date of argument
 *
 */
typedef void (*cli_cmd_func_cb_t)(int argc, char *argv[]);

typedef struct {
	/** cli command name */
	char *name;
	/** cli command help */
	char *help;
	/** the callback of one command */
	cli_cmd_func_cb_t func;
} cli_cmd_t;

/**
 * @brief cli init function,default uart0
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_cli_init(void);

/**
 * @brief cli command register
 *
 * @param[in] cmd Info of one command
 * @param[in] num Number
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_cli_cmd_register(const cli_cmd_t *cmd, uint8_t num);

/**
 * @brief uart cli init
 *
 * @param[in] uart_num The number of UART
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_cli_init_with_uart(uint8_t uart_num);

#ifdef __cplusplus
}
#endif
#endif
