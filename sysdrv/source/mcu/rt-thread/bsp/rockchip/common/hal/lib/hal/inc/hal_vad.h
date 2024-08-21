/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_VAD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup VAD
 *  @{
 */

#ifndef _HAL_VAD_H_
#define _HAL_VAD_H_

#include "hal_audio.h"

/***************************** MACRO Definition ******************************/
/** @defgroup VAD_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

/**
 * enum VAD_storeMode - the mode of vad store voice data.
 */
typedef enum {
    VAD_STOREMODE_AFTER_DET, /**< store the audio data after the vad detection */
    VAD_STOREMODE_AFTER_EN, /**< store the audio data after the vad enable */
    VAD_STOREMODE_NONE, /**< don't store the audio data */
} eVAD_storeMode;

/**
 * enum VAD_intType - the type of vad interrupt.
 */
typedef enum {
    VAD_INTTYPE_DET, /**< vad detect interrupt */
    VAD_INTTYPE_TRANS, /**< vad data transfer interrupt */
} eVAD_intType;

/** audio source index : address mapping */
struct AUDIO_SRC_ADDR_MAP {
    uint32_t id; /**< refer to reg VAD_CONTROL[3:1] */
    uint32_t addr; /**< refer to reg VAD_CONTROL[3:1] */
};

/** vad buf description */
struct VAD_BUF {
    uint32_t begin; /**< The physical ring buffer begin address */
    uint32_t end; /**< The physical ring buffer end address */
    uint32_t cur; /**< The physical cur position when vad stop */
    uint32_t pos; /**< The start pos to be read */
    int size; /**< The buffer valid data size in bytes */
    bool loop; /**< To indicate whether the buffer is overlapped or not */
};

/** vad structure */
struct HAL_VAD_DEV {
    struct VAD_REG *pReg; /**< vad controller physical base address */
    uint32_t ramStartAddr; /**< The physical ring buffer start address, double word align */
    uint32_t ramEndAddr; /**< The physical ring buffer end address, double word align */
    uint32_t audioSrc; /**< Src 0: from i2s; src 1: from pdm */
    uint32_t audioSrcAddr; /**< Audio src controller's FIFO addr */
    uint32_t audioDetChnl; /**< Indicates which channel data used for vad detection */
    uint32_t channels; /**< The total audio channels */
    uint32_t sampleBytes; /**< Bytes per sample */
    uint32_t bufferTime; /**< Buffer time in msec */
    eCLOCK_Name hclk;
    ePD_Id pd;
    uint8_t irq;  /**< irq number */
    struct VAD_BUF vbuf;

    int mode; /**< Mode 0: store the audio data after the vad detection;
               *   Mode 1: store the audio data after the vad enable;
               *   Mode 2: don't store the audio data.
               */
    void *buf; /**< The malloc buf used for conversion */
    bool vswitch; /**< The vad switch on/off flag */
    bool h_16Bit; /**< Get he vad high 16 bit data for detection */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup VAD_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_VAD_Init(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_DeInit(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_Supsend(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_Resume(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_Start(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_Stop(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_Pause(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_SetAudioSource(struct HAL_VAD_DEV *vad, uint32_t source);
HAL_Status HAL_VAD_SetMode(struct HAL_VAD_DEV *vad, eVAD_storeMode mode);
HAL_Status HAL_VAD_SetBufferTime(struct HAL_VAD_DEV *vad, uint32_t bufferTime);
HAL_Status HAL_VAD_SetDetectChannel(struct HAL_VAD_DEV *vad,
                                    uint32_t detectChannel);
HAL_Status HAL_VAD_Config(struct HAL_VAD_DEV *vad, struct AUDIO_PARAMS *params);
HAL_Status HAL_VAD_ConfigRingBuffer(struct HAL_VAD_DEV *vad,
                                    uint32_t start, uint32_t end);
HAL_Status HAL_VAD_SetPeriodSize(struct HAL_VAD_DEV *vad, uint32_t words);
HAL_Status HAL_VAD_EnableBusMode(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_DisableBusMode(struct HAL_VAD_DEV *vad);

HAL_Status HAL_VAD_IrqHandler(struct HAL_VAD_DEV *vad);
HAL_Status HAL_VAD_GetCurAddr(struct HAL_VAD_DEV *vad, uint32_t *addr);
HAL_Status HAL_VAD_GetIrqStatus(struct HAL_VAD_DEV *vad, uint32_t *status);
HAL_Status HAL_VAD_EnableInt(struct HAL_VAD_DEV *vad, eVAD_intType type, uint32_t enable);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_VAD_MODULE_ENABLED */
