/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 * Defines peripherals available for use on BCM94WCD1AUDIO board
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
/*
BCM9WCD1AUDIO platform pin definitions
*/


/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum
{
    WICED_GPIO_1,
    WICED_GPIO_2,
    WICED_GPIO_3,
    WICED_GPIO_4,
    WICED_GPIO_5,
    WICED_GPIO_6,
    WICED_GPIO_7,
    WICED_GPIO_8,
    WICED_GPIO_9,
    WICED_GPIO_10,
    WICED_GPIO_11,
    WICED_GPIO_12,
    WICED_GPIO_13,
    WICED_GPIO_14,
    WICED_GPIO_15,
    WICED_GPIO_16,
    WICED_GPIO_17,
    WICED_GPIO_18,
    WICED_GPIO_19,
    WICED_GPIO_20,
    WICED_GPIO_21,
    WICED_GPIO_22,
    WICED_GPIO_23,
    WICED_GPIO_24,
    WICED_GPIO_25,
    WICED_GPIO_26,
    WICED_GPIO_27,
    WICED_GPIO_28,
    WICED_GPIO_29,
    WICED_GPIO_30,
    WICED_GPIO_31,
    WICED_GPIO_32,
    WICED_GPIO_33,
    WICED_GPIO_34,
    WICED_GPIO_35,
    WICED_GPIO_36,
    WICED_GPIO_37,
    WICED_GPIO_38,
    WICED_GPIO_39,
    WICED_GPIO_40,
    WICED_GPIO_41,
    WICED_GPIO_MAX, /* Denotes the total number of GPIO port aliases. Not a valid GPIO alias */
    WICED_GPIO_32BIT = 0x7FFFFFFF,
} wiced_gpio_t;

typedef enum
{
    WICED_SPI_1,
    WICED_SPI_MAX, /* Denotes the total number of SPI port aliases. Not a valid SPI alias */
    WICED_SPI_32BIT = 0x7FFFFFFF,
} wiced_spi_t;

typedef enum
{
    WICED_I2C_1,
    WICED_I2C_MAX,
    WICED_I2C_32BIT = 0x7FFFFFFF,
} wiced_i2c_t;

typedef enum
{
    WICED_I2S_1,
    WICED_I2S_MAX, /* Denotes the total number of I2S port aliases.  Not a valid I2S alias */
    WICED_I2S_32BIT = 0x7FFFFFFF
} wiced_i2s_t;

typedef enum
{
    WICED_PWM_1,
    WICED_PWM_2,
    WICED_PWM_3,
    WICED_PWM_4,
    WICED_PWM_5,
    WICED_PWM_6,
    WICED_PWM_7,
    WICED_PWM_8,
    WICED_PWM_9,
    WICED_PWM_MAX, /* Denotes the total number of PWM port aliases. Not a valid PWM alias */
    WICED_PWM_32BIT = 0x7FFFFFFF,
} wiced_pwm_t;

typedef enum
{
    WICED_ADC_1,
    WICED_ADC_2,
    WICED_ADC_3,
    WICED_ADC_MAX, /* Denotes the total number of ADC port aliases. Not a valid ADC alias */
    WICED_ADC_32BIT = 0x7FFFFFFF,
} wiced_adc_t;

typedef enum
{
    WICED_UART_1,
    WICED_UART_2,
    WICED_UART_MAX, /* Denotes the total number of UART port aliases. Not a valid UART alias */
    WICED_UART_32BIT = 0x7FFFFFFF,
} wiced_uart_t;

/* Logical Button-ids which map to phyiscal buttons on the board */
typedef enum
{
    PLATFORM_BUTTON_1,      /* Back         */
    PLATFORM_BUTTON_2,      /* Volume Down  */
    PLATFORM_BUTTON_3,      /* Volume Up    */
    PLATFORM_BUTTON_4,      /* Pause / Play */
    PLATFORM_BUTTON_5,      /* Multi        */
    PLATFORM_BUTTON_6,      /* Forward      */
    PLATFORM_BUTTON_MAX, /* Denotes the total number of Buttons on the board. Not a valid Button alias */
} platform_button_t;

/******************************************************
 *                    Constants
 ******************************************************/

#define WICED_PLATFORM_BUTTON_COUNT  ( 6 )

/* UART port used for standard I/O */
#define STDIO_UART ( WICED_UART_1 )

/* SPI flash is present */
#define WICED_PLATFORM_INCLUDES_SPI_FLASH
#define WICED_SPI_FLASH_CS  ( WICED_GPIO_5 )

/* Components connected to external I/Os */
#define PLATFORM_LED_COUNT               ( 2 )
#define WICED_LED1           ( WICED_GPIO_11 )
#define WICED_LED2           ( WICED_GPIO_12 )
#define WICED_LED1_ON_STATE  ( WICED_ACTIVE_LOW )
#define WICED_LED2_ON_STATE  ( WICED_ACTIVE_LOW )
#define WICED_BUTTON1        ( WICED_GPIO_3 )
#define WICED_BUTTON2        ( WICED_GPIO_2 )
#define WICED_THERMISTOR     ( WICED_GPIO_4 )
#define WICED_GPIO_AUTH_RST  ( WICED_GPIO_40 )
/* #define WICED_GPIO_AUTH_SCL  ( WICED_GPIO_10 ) */
/* #define WICED_GPIO_AUTH_SDA  ( WICED_GPIO_11 ) */

/* I/O connection <-> Peripheral Connections */
#define WICED_LED1_JOINS_PWM       ( WICED_PWM_1 )
#define WICED_LED2_JOINS_PWM       ( WICED_PWM_2 )
#define WICED_THERMISTOR_JOINS_ADC ( WICED_ADC_3 )

#define AUTH_IC_I2C_PORT           (WICED_I2C_1)

/* common audio device defines */
#define PLATFORM_DEFAULT_AUDIO_INPUT            AUDIO_DEVICE_ID_NONE
#define PLATFORM_AUDIO_NUM_INPUTS               1
#define PLATFORM_DEFAULT_AUDIO_OUTPUT           AUDIO_DEVICE_ID_WM8533_DAC_LINE
#define PLATFORM_AUDIO_NUM_OUTPUTS              1

#define WICED_BUTTON_MULTI_FUNC                 (WICED_GPIO_2)
#define WICED_BUTTON_PAUSE_PLAY                 (WICED_GPIO_NONE)
#define WICED_BUTTON_VOLUME_UP                  (WICED_GPIO_NONE)
#define WICED_BUTTON_VOLUME_DOWN                (WICED_GPIO_NONE)
#define WICED_BUTTON_FORWARD                    (WICED_GPIO_NONE)
#define WICED_BUTTON_BACK                       (WICED_GPIO_3)

/* Logical Button-ids which map to phyiscal buttons on the board (see platform_gpio_buttons[] in platform.c)*/
#define PLATFORM_BUTTON_BACK                    ( PLATFORM_BUTTON_1 )
#define PLATFORM_BUTTON_VOLUME_DOWN             ( PLATFORM_BUTTON_2 )
#define PLATFORM_BUTTON_VOLUME_UP               ( PLATFORM_BUTTON_3 )
#define PLATFORM_BUTTON_PAUSE_PLAY              ( PLATFORM_BUTTON_4 )
#define PLATFORM_BUTTON_MULTI_FUNC              ( PLATFORM_BUTTON_5 )
#define PLATFORM_BUTTON_FORWARD                 ( PLATFORM_BUTTON_6 )

/* Bootloader OTA and OTA2 factory reset during settings */
#define PLATFORM_FACTORY_RESET_BUTTON_INDEX     ( PLATFORM_BUTTON_1 )
#define PLATFORM_FACTORY_RESET_TIMEOUT          ( 10000 )

/* Generic button checking defines */
#define PLATFORM_BUTTON_PRESS_CHECK_PERIOD      ( 100 )
#define PLATFORM_BUTTON_PRESSED_STATE           (   0 )

#define PLATFORM_GREEN_LED_INDEX                ( WICED_LED_INDEX_1 )
#define PLATFORM_RED_LED_INDEX                  ( WICED_LED_INDEX_2 )

#ifdef __cplusplus
} /*extern "C" */
#endif
