/*
 * $ Copyright Cypress Semiconductor $
 */

#pragma once
#include <stdint.h>
#include "wiced_platform.h"
#include "platform.h"
#include "platform_audio.h"
#include "wwd_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *            Constants
 ******************************************************/

/* CODEC clock master/slave, frame master/slave. */
#define WM8533_FMT_CCM_CFM          (0x0)
#define WM8533_FMT_CCS_CFM          (0x1)
#define WM8533_FMT_CCM_CFS          (0x2)
#define WM8533_FMT_CCS_CFS          (0x3)

#define WM8533_FMT_MASTER_MASK      (0x3)

/* standard audio device info */

/* standard audio device information */
#define WM8533_DAC_NAME         "WM8533_dac"
#define WM8533_DAC_DIRECTION    PLATFORM_AUDIO_DEVICE_OUTPUT
#define WM8533_DAC_PORT_TYPE    PLATFORM_AUDIO_LINE
#define WM8533_DAC_CHANNELS     2
#define WM8533_DAC_SIZES        (PLATFORM_AUDIO_SAMPLE_SIZE_16_BIT | PLATFORM_AUDIO_SAMPLE_SIZE_24_BIT)
#define WM8533_DAC_RATES        (PLATFORM_AUDIO_SAMPLE_RATE_8KHZ    | PLATFORM_AUDIO_SAMPLE_RATE_32KHZ |    \
                                 PLATFORM_AUDIO_SAMPLE_RATE_44_1KHZ | PLATFORM_AUDIO_SAMPLE_RATE_48KHZ |    \
                                 PLATFORM_AUDIO_SAMPLE_RATE_96KHZ   | PLATFORM_AUDIO_SAMPLE_RATE_192KHZ)

#define AUDIO_DEVICE_ID_WM8533_DAC_LINE_INFO                \
        { AUDIO_DEVICE_ID_WM8533_DAC_LINE, WM8533_DAC_NAME, \
          WM8533_DAC_DESCRIPTION, WM8533_DAC_DIRECTION,     \
          WM8533_DAC_PORT_TYPE, WM8533_DAC_CHANNELS,        \
          WM8533_DAC_SIZES, WM8533_DAC_RATES }

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *            Enumerations
 ******************************************************/
typedef enum
{
    CS43l22_SPEAKERS,
    CS43l22_HEADPHONES,
    CS43l22_SPEAKERS_AND_HEADPHONES
} cs43l22_audio_output_t;

typedef enum
{
    AIN1A,
    AIN1B,
    AIN2A,
    AIN2B,
    AIN3A,
    AIN3B,
    AIN4A,
    AIN4B,
} cs43l22_analog_in_channel_t;

typedef enum
{
    BEEPON_86_MSEC,
    BEEPON_430_MSEC,
    BEEPON_780_MSEC,
    BEEPON_1P20_SEC,
    BEEPON_1P50_SEC,
    BEEPON_1P80_SEC,
    BEEPON_2P20_SEC,
    BEEPON_2P50_SEC,
    BEEPON_2P80_SEC,
    BEEPON_3P20_SEC,
    BEEPON_3P50_SEC,
    BEEP_3P80_SEC,
    BEEPON_4P20_SEC,
    BEEPON_4P50_SEC,
    BEEPON_4P80_SEC,
    BEEPON_5P20_SEC,
} cs43l22_beep_on_time_t;

typedef enum
{
    BEEPOFF_1P23_SEC,
    BEEPOFF_2P58_SEC,
    BEEPOFF_3P90_SEC,
    BEEPOFF_5P20_SEC,
    BEEPOFF_6P60_SEC,
    BEEPOFF_8P05_SEC,
    BEEPOFF_9P35_SEC,
    BEEPOFF_10P80_SEC,
} cs43l22_beep_off_time_t;

typedef enum
{
    BEEP_FREQ_260P87,
    BEEP_FREQ_521P74,
    BEEP_FREQ_585P37,
    BEEP_FREQ_666P67,
    BEEP_FREQ_705P88,
    BEEP_FREQ_774P19,
    BEEP_FREQ_888P89,
    BEEP_FREQ_1000P00,
    BEEP_FREQ_1043P48,
    BEEP_FREQ_1200P00,
    BEEP_FREQ_1333P33,
    BEEP_FREQ_1411P76,
    BEEP_FREQ_1600P00,
    BEEP_FREQ_2000P00,
    BEEP_FREQ_2181P82,
} cs43l22_beep_freq_t;

typedef enum
{
    CORNER_50HZ,
    CORNER_100HZ,
    CORNER_200HZ,
    CORNER_250HZ
} bass_corner_freq_t;

typedef enum
{
    CORNER_5KHZ,
    CORNER_7KHZ,
    CORNER_10KHZ,
    CORNER_15KHZ
} trebble_corner_freq_t;

/* limiter configuration */
typedef enum
{
    TH_0DB,
    TH_M3DB,
    TH_M6DB,
    TH_M9DB,
    TH_M12DB,
    TH_M18DB,
    TH_M24DB,
} cs43l22_limiter_threshold_t;

typedef enum
{
    VA1P8_1P5V,
    VA1P8_2P0V,
    VA1P8_2P5V,
    VA1P8_3P0V,
    VA1P8_3P5V,
    VA1P8_4P0V,
    VA1P8_4P5V,
    VA1P8_5P0V,

    VA2P5_1P5V,
    VA2P5_2P0V,
    VA2P5_2P5V,
    VA2P5_3P0V,
    VA2P5_3P5V,
    VA2P5_4P0V,
    VA2P5_4P5V,
    VA2P5_5P0V,

} cs43l22_ref_voltage_t;

typedef enum
{
    CHANNEL_A,
    CHANNEL_B,
} cs43l22_pcm_channel_t;


typedef enum
{
    LEFT_JUSTIFIED,
    I2S,
    RIGHT_JUSTIFIED,
} cs43l22_dac_interface_t;


/******************************************************
 *             Structures
 ******************************************************/

typedef struct
{
    wiced_bool_t          enable_comensation;
    cs43l22_ref_voltage_t reference_voltage;
} cs43l22_battery_t;


typedef struct
{
    uint8_t Reserved_0: 1;
    uint8_t Reserved_1: 1;
    uint8_t PCMBOVFL:    1;
    uint8_t PCMAOVFL:    1;
    uint8_t DSPBOVFL:    1;
    uint8_t DSPAOVFL:    1;
    uint8_t SPCLKERR:    1;
    uint8_t Reserved_7:    1;
} cs43l22_status_t;



typedef struct
{
    cs43l22_limiter_threshold_t threshold;
    uint8_t                     release_rate;
    uint8_t                     attack_rate;
} cs43l22_limiter_config_t;

typedef struct
{
    /* enables mixing of the beep signal with data from I2S */
    wiced_bool_t            mix_enable;
    cs43l22_beep_on_time_t  on_time;
    cs43l22_beep_on_time_t  off_time;
    uint8_t                 volume;

} cs43l22_beep_params_t;

typedef struct
{
    cs43l22_audio_output_t      output;
    uint32_t                    sample_rate;
    uint8_t                     sample_length; /* this codec supports only 16 bit samples and 24 bit samples */
    /* applicable only when output is set to either SPEAKERS or SPEAKERS_AND_HEADPHONES */
    wiced_bool_t                stereo_speakers;
} cs43l22_config_t;


typedef struct
{
    wiced_gpio_t            addr0;
    wiced_gpio_t            cifmode;
    wiced_i2c_device_t     *i2c_data;
    wiced_i2s_t             data_port;
    uint8_t                 fmt;
} wm8533_device_data_t;

/******************************************************
 *             Function declarations
 ******************************************************/
wiced_result_t wm8533_platform_configure(wm8533_device_data_t *device_data, uint32_t mclk, uint32_t fs, uint8_t width);

wiced_result_t wm8533_device_register(wm8533_device_data_t *device_data, const platform_audio_device_id_t device_id);

#ifdef __cplusplus
} /* extern "C" */
#endif
