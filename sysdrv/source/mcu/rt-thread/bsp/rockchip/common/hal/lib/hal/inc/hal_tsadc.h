/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_TSADC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup TSADC
 *  @{
 */

#ifndef _HAL_TSADC_H_
#define _HAL_TSADC_H_

#include "hal_def.h"

/*************************** MACRO Definition ****************************/
/** @defgroup TSADC_Exported_Definition_Group1 Basic Definition
 *  @{
 */

typedef enum {
    TSHUT_MODE_CRU = 0,
    TSHUT_MODE_GPIO,
} eTSADC_tshutMode;

typedef enum {
    TSHUT_LOW_ACTIVE = 0,
    TSHUT_HIGH_ACTIVE,
} eTSADC_tshutPolarity;

struct HAL_TSADC_DEV {
    eCLOCK_Name sclkID;
    uint32_t sclkGateID;
    uint32_t pclkGateID;
    uint32_t sclkResetID;
    uint32_t pclkResetID;
    uint32_t speed;
    eTSADC_tshutPolarity polarity;
    eTSADC_tshutMode mode;
};
/***************************** Structure Definition **************************/

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup TSADC_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_TSADC_Enable_AUTO(int chn, eTSADC_tshutPolarity polarity, eTSADC_tshutMode mode);
HAL_Status HAL_TSADC_Disable_AUTO(int chn);
HAL_Check HAL_TSADC_IsEnabled_AUTO(int chn);
int HAL_TSADC_GetTemperature_AUTO(int chn);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_TSADC_MODULE_ENABLED */
