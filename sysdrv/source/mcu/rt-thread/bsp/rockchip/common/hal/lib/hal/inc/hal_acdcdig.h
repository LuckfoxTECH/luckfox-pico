/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_ACDCDIG_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup ACDCDIG
 *  @{
 */

#ifndef _HAL_ACDCDIG_H_
#define _HAL_ACDCDIG_H_

#include "hal_audio.h"
#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup ACDCDIG_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/
/**
  * @brief Indicate Which module use I2C
  */
typedef enum {
    CODEC_USE_I2C = 0, /**< Codec exclusive i2c by default. */
    MCU_USE_I2C, /**< MCU exclusive i2c. */
} eACDCDIG_i2cUsed;

/** acdcDig dev struct */
struct HAL_ACDCDIG_DEV {
    struct ACDCDIG_REG *pReg;
    eCLOCK_Name hclk;
    eCLOCK_Name clkI2cCodec;
    uint16_t bclkFs;
    uint16_t i2cFlags;
    uint16_t enabled;
    eAUDIO_fmtType format;
    ePD_Id pd;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup ACDCDIG_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_ACDCDIG_Supsend(struct HAL_ACDCDIG_DEV *acdcDig);
HAL_Status HAL_ACDCDIG_Resume(struct HAL_ACDCDIG_DEV *acdcDig);
HAL_Status HAL_ACDCDIG_Init(struct HAL_ACDCDIG_DEV *acdcDig, struct AUDIO_INIT_CONFIG *config);
HAL_Status HAL_ACDCDIG_DeInit(struct HAL_ACDCDIG_DEV *acdcDig);
HAL_Status HAL_ACDCDIG_Enable(struct HAL_ACDCDIG_DEV *acdcDig, eAUDIO_streamType stream);
HAL_Status HAL_ACDCDIG_Disable(struct HAL_ACDCDIG_DEV *acdcDig, eAUDIO_streamType stream);
HAL_Status HAL_ACDCDIG_Config(struct HAL_ACDCDIG_DEV *acdcDig, eAUDIO_streamType stream, struct AUDIO_PARAMS *params);
HAL_Status HAL_ACDCDIG_SetClock(struct HAL_ACDCDIG_DEV *acdcDig, eAUDIO_streamType stream, uint32_t freq);
HAL_Status HAL_ACDCDIG_SetGain(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream, struct AUDIO_DB_CONFIG *dBConfig);
HAL_Status HAL_ACDCDIG_GetGain(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream, struct AUDIO_DB_CONFIG *dBConfig);
HAL_Status HAL_ACDCDIG_GetGainInfo(struct HAL_ACDCDIG_DEV *acdcDig,
                                   struct AUDIO_GAIN_INFO *info);
HAL_Status HAL_ACDCDIG_RequestI2C(struct HAL_ACDCDIG_DEV *acdcDig, eACDCDIG_i2cUsed i2cUsed);
eACDCDIG_i2cUsed HAL_ACDCDIG_CheckI2C(struct HAL_ACDCDIG_DEV *acdcDig);
uint32_t HAL_ACDCDIG_GetSyncClk(eAUDIO_sampleRate sampleRate);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_ACDCDIG_MODULE_ENABLED */
