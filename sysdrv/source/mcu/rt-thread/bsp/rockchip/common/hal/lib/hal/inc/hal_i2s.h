/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_I2S_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup I2S
 *  @{
 */

#ifndef _HAL_I2S_H_
#define _HAL_I2S_H_

#include "hal_audio.h"
#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup I2S_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

/** i2s dev struct */
struct HAL_I2S_DEV {
    struct I2S_REG *pReg;
    eCLOCK_Name mclk;
    uint32_t mclkGate;
    eCLOCK_Name hclk;
    ePD_Id pd;
    uint16_t bclkFs;
    struct AUDIO_DMA_DATA rxDmaData;
    struct AUDIO_DMA_DATA txDmaData;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup I2S_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_I2S_Supsend(struct HAL_I2S_DEV *i2s);
HAL_Status HAL_I2S_Resume(struct HAL_I2S_DEV *i2s);
HAL_Status HAL_I2S_Init(struct HAL_I2S_DEV *i2s, struct AUDIO_INIT_CONFIG *config);
HAL_Status HAL_I2S_DeInit(struct HAL_I2S_DEV *i2s);
HAL_Status HAL_I2S_Enable(struct HAL_I2S_DEV *i2s, eAUDIO_streamType stream);
HAL_Status HAL_I2S_Disable(struct HAL_I2S_DEV *i2s, eAUDIO_streamType stream);
HAL_Status HAL_I2S_Config(struct HAL_I2S_DEV *i2s, eAUDIO_streamType stream,
                          struct AUDIO_PARAMS *params);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_I2S_MODULE_ENABLED */
