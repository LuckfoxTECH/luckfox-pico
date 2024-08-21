/*
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 * Defines LPC17xx common peripheral structures, macros, constants and declares  peripheral API
 */
#pragma once

#include "platform_constants.h"
#include "wwd_constants.h"
#include "RTOS/wwd_rtos_interface.h"
//#include "ring_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/
#undef  PLATFORM_UART_ENABLE
#undef  PLATFORM_SPI_ENABLE
#define PLATFORM_SDIO_ENABLE
#undef  PLATFORM_ADC_ENABLE
#undef  PLATFORM_PWM_ENABLE
#undef  PLATFORM_I2C_ENABLE

/******************************************************
 *                    Constants
 ******************************************************/

/* GPIOA to I */
#define NUMBER_OF_GPIO_PORTS     (3)

#define NUMBER_OF_GPIO_IRQ_LINES (8)

#define NUMBER_OF_SPI_PORTS      (1)
/* Invalid UART port number */
#define INVALID_UART_PORT_NUMBER (0xff)

/* Default STDIO buffer size */
#ifndef STDIO_BUFFER_SIZE
#define STDIO_BUFFER_SIZE        (64)
#endif

#define RK_PIN_INPUT            ((uint8_t)0)
#define RK_PIN_OUTPUT           ((uint8_t)1)

/******************************************************
 *                   Enumerations
 ******************************************************/
typedef enum
{
    RK_GPIO_PORT_0    = 0,
    RK_GPIO_PORT_1    = 1,
    RK_GPIO_PORT_2    = 2,
    RK_GPIO_PORT_3    = 3,
    RK_GPIO_PORT_4    = 4,
    RK_GPIO_PORT_5    = 5,
    RK_GPIO_PORT_6    = 6,
    RK_GPIO_PORT_7    = 7
} rk_gpio_port_t;

typedef enum
{
    RK_PIN_GROUP_0      = 0,
    RK_PIN_GROUP_1      = 1,
    RK_PIN_GROUP_2      = 2,
    RK_PIN_GROUP_3      = 3,
    RK_PIN_GROUP_4      = 4,
    RK_PIN_GROUP_5      = 5,
    RK_PIN_GROUP_6      = 6,
    RK_PIN_GROUP_7      = 7,
    RK_PIN_GROUP_8      = 8,
    RK_PIN_GROUP_9      = 9,
    RK_PIN_GROUP_A      = 10,
    RK_PIN_GROUP_B      = 11,
    RK_PIN_GROUP_C      = 12,
    RK_PIN_GROUP_D      = 13,
    RK_PIN_GROUP_E      = 14,
    RK_PIN_GROUP_F      = 15,
    RK_PIN_GROUP_CLK    = 24,
} rk_pin_group_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/
/* LPC USART structure.
 * For more details see uart_18xx_43xx.h
 */
#ifdef PLATFORM_UART_ENABLE
typedef void *platform_uart_port_t;
#endif

#ifdef PLATFORM_SPI_ENABLE
typedef void *platform_spi_port_t;
typedef void *platform_spifi_port_t;
#endif

#ifdef PLATFORM_SDIO_ENABLE
typedef void *platform_sdio_port_t;
#endif

typedef struct
{
    uint8_t to_be_filled;
} platform_gpio_port_t;

typedef uint8_t platform_gpio_alternate_function_t;

/* Changed temporary to get stuff working */
typedef struct
{
    uint8_t to_be_filled;
} platform_gpio_interrupt_trigger;

typedef void (*platform_gpio_irq_handler_t)(void *arg);

/******************************************************
 *                    Structures
 ******************************************************/
/* This generic structure specifies a hardware pin. */
typedef struct
{
    rk_pin_group_t group;           /* GROUP of pins to which pin is connected. */
    uint8_t         pin;            /* Pin within the port to which pin is connected.   */
    uint16_t        mode_function;  /* SCU function and mode selection definitions from scu_18xx_43xx.h*/
} platform_pin_t;

/* GPIO */
/*~~~~~~*/
typedef struct
{
    platform_pin_t  hw_pin;          /* The actual hardwar pin the GPIO is connected to. */
    rk_gpio_port_t gpio_port;       /* GPIO port. platform_gpio_port_t is defined in <WICED-SDK>/MCU/<MCU>/platform_mcu_interface.h */
    uint8_t         gpio_pin;        /* pin number. Valid range is defined in <WICED-SDK>/MCU/<MCU>/platform_mcu_interface.h         */
} platform_gpio_t;

#ifdef PLATFORM_UART_ENABLE
/* USART */
/*~~~~~~~*/
typedef struct
{
    platform_uart_port_t   *uart_base;
    platform_pin_t          tx_pin;
    platform_pin_t          rx_pin;
    platform_pin_t          cts_pin;
    platform_pin_t          rts_pin;
} platform_uart_t;

typedef struct
{
    platform_uart_t        *interface;
    wiced_ring_buffer_t    *rx_buffer;
    host_semaphore_type_t   rx_complete;
    host_semaphore_type_t   tx_complete;
    volatile uint32_t       tx_size;
    volatile uint32_t       rx_size;
    volatile platform_result_t last_receive_result;
    volatile platform_result_t last_transmit_result;
} platform_uart_driver_t;
#endif

#ifdef PLATFORM_SPI_ENABLE
/* SPI */
/*~~~~~*/
typedef struct
{
    uint8_t                 enable_cs_emulation;    /* CS will be emulated through software. Should be used for WLAN SPI interface. */
    platform_spi_port_t    *spi_base;
    platform_pin_t          clock;
    platform_pin_t          mosi;
    platform_pin_t          miso;
    host_semaphore_type_t   in_use_semaphore;
    wiced_bool_t            semaphore_is_inited;
} platform_spi_t;

typedef struct
{
    uint8_t unimplemented;
} platform_spi_slave_driver_t;

typedef struct
{
    uint32_t bit_rate;
} platform_spi_peripheral_config_t;

/* SPIFI */
/*~~~~~~~*/
typedef struct
{
    platform_spifi_port_t  *spifi_base;
    platform_pin_t          clock;
    platform_pin_t          miso;
    platform_pin_t          mosi;
    platform_pin_t          sio2;
    platform_pin_t          sio3;
    platform_pin_t          cs;
    host_semaphore_type_t   in_use_semaphore;
    wiced_bool_t            semaphore_is_inited;
} platform_spifi_t;
#endif

#ifdef PLATFORM_ADC_ENABLE
/* ADC */
/*~~~~~*/
typedef struct
{
    uint8_t unimplemented;
} platform_adc_t;
#endif

#ifdef PLATFORM_PWM_ENABLE
/* PWM */
/*~~~~~*/
typedef struct
{
    uint8_t unimplemented;
} platform_pwm_t;
#endif

#ifdef PLATFORM_I2C_ENABLE
/* I2c */
/*~~~~~*/
typedef struct
{
    uint8_t unimplemented;
} platform_i2c_t;
#endif

/* Changed temporary to get stuff working */
typedef void (*platform_peripheral_clock_function_t)(int clock);

#ifdef PLATFORM_SDIO_ENABLE
/* SDIO */
/*~~~~~~~*/
typedef struct
{
    platform_sdio_port_t   *base;
    platform_pin_t          clock;
    platform_pin_t          D0;
    platform_pin_t          D1;
    platform_pin_t          D2;
    platform_pin_t          D3;
    platform_pin_t          cmd;
    host_semaphore_type_t   in_use_semaphore;
    wiced_bool_t            semaphore_is_inited;
} platform_sdio_t;
#endif

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
#ifdef PLATFORM_UART_ENABLE
void              platform_uart_irq(platform_uart_driver_t *driver);
#endif

platform_result_t platform_gpio_irq_init(void);
platform_result_t platform_pin_set_alternate_function(const platform_pin_t *pin);

#ifdef __cplusplus
} /* extern "C" */
#endif
