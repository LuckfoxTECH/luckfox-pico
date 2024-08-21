/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_ACODEC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup ACODEC
 *  @{
 */

#ifndef _HAL_ACODEC_H_
#define _HAL_ACODEC_H_

#include "hal_audio.h"
#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup ACODEC_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

/** acodec dev struct */
struct HAL_ACODEC_DEV {
    struct ACODEC_REG *pReg;
    eCLOCK_Name hclk;
    uint32_t mclk;
    uint32_t micBias;
    bool micDifferential;
    void *privateData;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup ACODEC_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_ACODEC_Supsend(struct HAL_ACODEC_DEV *acodec);
HAL_Status HAL_ACODEC_Resume(struct HAL_ACODEC_DEV *acodec);
HAL_Status HAL_ACODEC_Init(struct HAL_ACODEC_DEV *acodec, struct AUDIO_INIT_CONFIG *config);
HAL_Status HAL_ACODEC_DeInit(struct HAL_ACODEC_DEV *acodec);
HAL_Status HAL_ACODEC_Enable(struct HAL_ACODEC_DEV *acodec, eAUDIO_streamType stream);
HAL_Status HAL_ACODEC_Disable(struct HAL_ACODEC_DEV *acodec, eAUDIO_streamType stream);
HAL_Status HAL_ACODEC_Config(struct HAL_ACODEC_DEV *acodec, eAUDIO_streamType stream, struct AUDIO_PARAMS *params);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_ACODEC_MODULE_ENABLED */
