/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup AUDIO
 *  @{
 */

#ifndef _HAL_AUDIO_H_
#define _HAL_AUDIO_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup AUDIO_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

/**
 * enum AUDIO_sampleRate - audio samplerate: up to 192k.
 */
typedef enum {
    AUDIO_SAMPLERATE_8000   = 8000,
    AUDIO_SAMPLERATE_11025  = 11025,
    AUDIO_SAMPLERATE_12000  = 12000,
    AUDIO_SAMPLERATE_16000  = 16000,
    AUDIO_SAMPLERATE_22050  = 22050,
    AUDIO_SAMPLERATE_24000  = 24000,
    AUDIO_SAMPLERATE_32000  = 32000,
    AUDIO_SAMPLERATE_44100  = 44100,
    AUDIO_SAMPLERATE_48000  = 48000,
    AUDIO_SAMPLERATE_64000  = 64000,
    AUDIO_SAMPLERATE_88200  = 88200,
    AUDIO_SAMPLERATE_96000  = 96000,
    AUDIO_SAMPLERATE_128000 = 128000,
    AUDIO_SAMPLERATE_176400 = 176400,
    AUDIO_SAMPLERATE_192000 = 192000,
} eAUDIO_sampleRate;

/**
 * enum AUDIO_sampleBits - audio bits per sample.
 */
typedef enum {
    AUDIO_SAMPLEBITS_8  = 8,
    AUDIO_SAMPLEBITS_16 = 16,
    AUDIO_SAMPLEBITS_18 = 18,
    AUDIO_SAMPLEBITS_20 = 20,
    AUDIO_SAMPLEBITS_24 = 24,
    AUDIO_SAMPLEBITS_32 = 32,
} eAUDIO_sampleBits;

/**
 * enum AUDIO_channels - audio channels: up to 8 channels.
 */
typedef enum {
    AUDIO_CHANNELS_2 = 2,
    AUDIO_CHANNELS_4 = 4,
    AUDIO_CHANNELS_6 = 6,
    AUDIO_CHANNELS_8 = 8,
} eAUDIO_channels;

/**
 * enum AUDIO_streamType - audio stream is playback or capture.
 */
typedef enum {
    AUDIO_STREAM_PLAYBACK = 0,
    AUDIO_STREAM_CAPTURE,
} eAUDIO_streamType;

/**
 * enum AUDIO_fmtType - audio format.
 */
typedef enum {
    AUDIO_FMT_I2S,
    AUDIO_FMT_RIGHT_J,
    AUDIO_FMT_LEFT_J,
    AUDIO_FMT_PCM,
    AUDIO_FMT_PCM_DELAY1,
    AUDIO_FMT_PCM_DELAY2,
    AUDIO_FMT_PCM_DELAY3,
    AUDIO_FMT_TDM_PCM,
    AUDIO_FMT_TDM_PCM_L_SHIFT_MODE0,
    AUDIO_FMT_TDM_PCM_L_SHIFT_MODE1,
    AUDIO_FMT_TDM_PCM_L_SHIFT_MODE2,
    AUDIO_FMT_TDM_PCM_L_SHIFT_MODE3,
    AUDIO_FMT_TDM_I2S_HALF_FRAME,
    AUDIO_FMT_TDM_I2S_ONE_FRAME,
    AUDIO_FMT_TDM_RIGHT_J_HALF_FRAME,
    AUDIO_FMT_TDM_RIGHT_J_ONE_FRAME,
    AUDIO_FMT_TDM_LEFT_J_HALF_FRAME,
    AUDIO_FMT_TDM_LEFT_J_ONE_FRAME,
    AUDIO_FMT_PDM,
} eAUDIO_fmtType;

/**
 * enum TRCM_modeType - TRCM modes just for i2stdm.
 */
typedef enum {
    TRCM_NONE = 0,
    TRCM_TXONLY,
    TRCM_RXONLY,
} eTRCM_modeType;

/**
 * enum ePDM_mode - PDM modes
 */
typedef enum {
    PDM_NORMAL_MODE = 0, /**< normal mode: 2.048/2.8224M/3.072M clk */
    PDM_LOW_MODE, /**< lower power mode: 1.024M/1.4112M/1.536M clk */
    PDM_HIGH_MODE, /**< high mode: 4.096M/5.6448M/6.144M clk */
} ePDM_mode;

/**
 * struct AUDIO_INIT_CONFIG - init config for dai/codec init.
 */
struct AUDIO_INIT_CONFIG {
    uint32_t master : 1;
    uint32_t clkInvert : 1;
    uint32_t frameInvert : 1;
    eAUDIO_fmtType format;
    eTRCM_modeType trcmMode;
    ePDM_mode pdmMode;
    uint16_t txMap; /**< route mapping of PATHx to SDOx, 4 bits per path.
                      *  |15:12|11:8|7:4|3:0|-->|p3|p2|p1|p0|
                      *  each path can choose one sdo as its sink.
                      *  e.g.
                      *  txMap = 0x3210: p3->sdo3, p2->sdo2, p1->sdo1, p0->sdo0
                      *  txMap = 0x3012: p3->sdo3, p2->sdo0, p1->sdo1, p0->sdo2
                      *  txMap = 0x0: ignored
                      */
    uint16_t rxMap; /**< route mapping of SDIx to PATHx, 4 bits per path.
                      *  |15:12|11:8|7:4|3:0|-->|p3|p2|p1|p0|
                      *  each path can choose one sdi as its source.
                      *  e.g.
                      *  rxMap = 0x3210: p3<-sdi3, p2<-sdi2, p1<-sdi1, p0<-sdi0
                      *  rxMap = 0x3012: p3<-sdi3, p2<-sdi0, p1<-sdi1, p0<-sdi2
                      *  rxMap = 0x0: ignored
                      */
};

/**
 * struct AUDIO_PARAMS - audio params for config dai.
 */
struct AUDIO_PARAMS {
    eAUDIO_sampleRate sampleRate; /**< sample rate: from 8k ~ 192k. */
    eAUDIO_sampleBits sampleBits; /**< bit per sample: 8bits, 16bits, 32bits. */
    eAUDIO_channels channels; /**< channels: up to 8ch. */
};

/**
 * struct AUDIO_DMA_DATA - audio dma data.
 */
struct AUDIO_DMA_DATA {
    uint32_t addr; /**< The fifo address of dai. */
    uint16_t addrWidth; /**< The width of the addr */
    uint8_t maxBurst; /**< Max number of words(in units of the addrWidth) */
    uint8_t dmaReqCh; /**< audio dma request channel number */
    struct DMA_REG *dmac; /**< dmac reg base ptr */
};

/**
 * struct audio_gain_info - audio gain information struct.
 */
struct AUDIO_GAIN_INFO {
    int32_t mindB; /**< The min dB and scaled 1000 times for interger handing. */
    int32_t maxdB; /**< The max dB and scaled 1000 times for interger handing. */
    int32_t step; /**< The step dB and scaled 1000 times for interger handing. */
};

/**
 * struct audio_gain_info - audio gain information struct.
 */
struct AUDIO_DB_CONFIG {
    int dB; /**< The current dB and scaled 1000 times for interger handing. */
    uint8_t ch; /**< The specified channel for ADC or DAC to set/get dB. */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup AUDIO_Public_Function_Declare Public Function Declare
 *  @{
 */

/** @} */

#endif

/** @} */

/** @} */
