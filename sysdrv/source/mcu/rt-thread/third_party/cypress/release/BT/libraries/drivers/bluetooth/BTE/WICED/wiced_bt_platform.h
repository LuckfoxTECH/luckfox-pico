/*
 * $ Copyright Broadcom Corporation $
 */
#pragma once

/* Pin Assignments
 */

#include "platform_peripheral.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum
{
    WICED_BT_PIN_POWER,
    WICED_BT_PIN_RESET,
    WICED_BT_PIN_HOST_WAKE,
    WICED_BT_PIN_DEVICE_WAKE,
} wiced_bt_control_pin_t;

typedef enum
{
    WICED_BT_PIN_UART_TX,
    WICED_BT_PIN_UART_RX,
    WICED_BT_PIN_UART_CTS,
    WICED_BT_PIN_UART_RTS,
} wiced_bt_uart_pin_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/* Externed from <WICED-SDK>/platforms/<Platform>/platform.c */
extern const platform_gpio_t        *wiced_bt_control_pins[];
extern const platform_gpio_t        *wiced_bt_uart_pins[];
extern const platform_uart_t        *wiced_bt_uart_peripheral;
extern       platform_uart_driver_t *wiced_bt_uart_driver;
extern const platform_uart_config_t  wiced_bt_uart_config;

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
