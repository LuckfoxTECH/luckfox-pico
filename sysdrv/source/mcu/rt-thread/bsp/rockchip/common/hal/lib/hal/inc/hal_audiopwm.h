/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_AUDIOPWM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup AUDIOPWM
 *  @{
 */

#ifndef _HAL_AUDIOPWM_H_
#define _HAL_AUDIOPWM_H_

#include "hal_audio.h"

/***************************** MACRO Definition ******************************/
/** @defgroup AUDIOPWM_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

/** audioPwm dev struct */
struct HAL_AUDIOPWM_DEV {
    struct AUDIOPWM_REG *pReg;
    eCLOCK_Name mclk;
    uint32_t mclkGate;
    eCLOCK_Name hclk;

    struct AUDIO_DMA_DATA txDmaData;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup AUDIOPWM_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_AUDIOPWM_Supsend(struct HAL_AUDIOPWM_DEV *audioPwm);
HAL_Status HAL_AUDIOPWM_Resume(struct HAL_AUDIOPWM_DEV *audioPwm);
HAL_Status HAL_AUDIOPWM_Init(struct HAL_AUDIOPWM_DEV *audioPwm, struct AUDIO_INIT_CONFIG *config);
HAL_Status HAL_AUDIOPWM_DeInit(struct HAL_AUDIOPWM_DEV *audioPwm);
HAL_Status HAL_AUDIOPWM_Enable(struct HAL_AUDIOPWM_DEV *audioPwm);
HAL_Status HAL_AUDIOPWM_Disable(struct HAL_AUDIOPWM_DEV *audioPwm);
HAL_Status HAL_AUDIOPWM_Config(struct HAL_AUDIOPWM_DEV *audioPwm, struct AUDIO_PARAMS *params);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_AUDIOPWM_MODULE_ENABLED */
