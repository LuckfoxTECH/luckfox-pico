/*
 * $ Copyright Cypress Semiconductor $
 */
#pragma once

#include "wiced_platform.h"
#include "wiced_audio.h"


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
    WICED_I2S_READ,
    WICED_I2S_WRITE
} wiced_i2s_transfer_t;

typedef enum
{
    WICED_I2S_SPDIF_MODE_OFF,
    WICED_I2S_SPDIF_MODE_ON,
} wiced_i2s_spdif_mode_t;

typedef struct
{
    void *useless;
} useless_t;
typedef useless_t platform_dma_t;
typedef useless_t I2S_InitTypeDef;
typedef useless_t platform_i2s_t;
typedef useless_t SPI_TypeDef;
/******************************************************
 *                 Type Definitions
 ******************************************************/

//typedef DMA_Stream_TypeDef dma_stream_registers_t;
typedef useless_t dma_stream_registers_t;
typedef uint32_t           dma_channel_t;

typedef struct
{
    uint32_t sample_rate;
    uint16_t period_size;
    uint8_t bits_per_sample;
    uint8_t channels;
    wiced_i2s_spdif_mode_t i2s_spdif_mode;
} wiced_i2s_params_t;

typedef struct
{
    void    *next;
    void    *buffer;
    uint32_t buffer_size;
} platform_i2s_transfer_t;

/* the callback will give a neww buffer pointer to the i2s device */
typedef void(*wiced_i2s_tx_callback_t)(uint8_t **buffer, uint16_t *size, void *context);

/* returned buffer and new buffer to receive more data */
typedef void(*wiced_i2s_rx_callback_t)(uint8_t *buffer, uint16_t read_size,  uint8_t **rx_buffer, uint16_t *rx_buf_size);

/******************************************************
 *               Function Declarations
 ******************************************************/

wiced_result_t wiced_i2s_init(wiced_audio_session_ref sh, wiced_i2s_t i2s, wiced_i2s_params_t *params, uint32_t *mclk);
wiced_result_t wiced_i2s_deinit(wiced_i2s_t i2s);
wiced_result_t wiced_i2s_set_audio_buffer_details(wiced_i2s_t i2s, uint8_t *buffer_ptr, uint16_t size);
wiced_result_t wiced_i2s_start(wiced_i2s_t i2s);
wiced_result_t wiced_i2s_stop(wiced_i2s_t);
wiced_result_t wiced_i2s_get_current_hw_pointer(wiced_i2s_t i2s, uint32_t *hw_pointer);
wiced_result_t wiced_i2s_pll_set_fractional_divider(wiced_i2s_t i2s, float value);
wiced_result_t wiced_i2s_get_clocking_details(const wiced_i2s_params_t *config, uint32_t *mclk);


#ifdef __cplusplus
} /* extern "C" */
#endif
