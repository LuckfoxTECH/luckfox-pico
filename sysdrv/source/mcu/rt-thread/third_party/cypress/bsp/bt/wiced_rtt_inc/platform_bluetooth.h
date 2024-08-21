/*
 * $ Copyright Cypress Semiconductor $
 */
#pragma once
#ifndef RTT
#include "platform_peripheral.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
/******************************************************
 *                      Macros
 ******************************************************/
#ifndef UART_RX_FIFO_SIZE
#define UART_RX_FIFO_SIZE (3000)
#endif
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
    WICED_BT_PIN_MAX,
} wiced_bt_control_pin_t;

typedef enum
{
    WICED_BT_PIN_UART_TX,
    WICED_BT_PIN_UART_RX,
    WICED_BT_PIN_UART_CTS,
    WICED_BT_PIN_UART_RTS,
} wiced_bt_uart_pin_t;

typedef enum
{
    PATCHRAM_DOWNLOAD_MODE_NO_MINIDRV_CMD,
    PATCHRAM_DOWNLOAD_MODE_MINIDRV_CMD,
} wiced_bt_patchram_download_mode_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/

typedef struct
{
    uint32_t                          patchram_download_baud_rate;
    wiced_bt_patchram_download_mode_t patchram_download_mode;
    uint32_t                          featured_baud_rate;
} platform_bluetooth_config_t;

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

//TODO: put all these variables into a single structure.
/* Variables to be defined by the Bluetooth supporting platform */
#ifndef RTT
extern const platform_gpio_t        *wiced_bt_control_pins[];
extern const platform_gpio_t        *wiced_bt_uart_pins[];
extern const platform_uart_t        *wiced_bt_uart_peripheral;
extern       platform_uart_driver_t *wiced_bt_uart_driver;
extern const platform_uart_config_t  wiced_bt_uart_config;
extern const platform_bluetooth_config_t wiced_bt_config;
#endif
/******************************************************
 *               Function Declarations
 ******************************************************/

/*
 * init 3 GPIOs
 *      bt_wake   - PC5, PIN37
 *      host_wake - PC6, PIN38
 *      bt_reset  - PC8, PIN40
*/
#define PIN_BT_RST   37
#define PIN_BT_DEV   38
#define PIN_BT_HOST  40

#ifdef __cplusplus
} /* extern "C" */
#endif
