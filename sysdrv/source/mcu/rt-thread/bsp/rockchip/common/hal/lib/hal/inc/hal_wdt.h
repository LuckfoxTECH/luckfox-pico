/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_WDT_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup WDT
 *  @{
 */

#ifndef _HAL_WDT_H_
#define _HAL_WDT_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup WDT_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

enum WDT_RESP_MODE {
    DIRECT_SYSTEM_RESET,   /**< Immediately reset system if WDT timeout */
    INDIRECT_SYSTEM_RESET, /**< First generate an interrupt and if the interrupt is not cleard by the time,
                            *   a second timeout occur then generate a system reset
                            */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup WDT_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_WDT_Init(uint32_t freq, struct WDT_REG *wdt);
uint32_t HAL_WDT_GetTimeout(void);
HAL_Status HAL_WDT_SetTimeout(uint32_t top);
HAL_Status HAL_WDT_KeepAlive(void);
HAL_Status HAL_WDT_Start(enum WDT_RESP_MODE mode);
uint32_t HAL_WDT_ClearInterrupt(void);
uint32_t HAL_WDT_GetIntStatus(void);
uint32_t HAL_WDT_GetTimeLeft(void);
HAL_Status HAL_WDT_DynFreqUpdata(uint32_t freq);
HAL_Status HAL_WDT_DynFreqResume(void);
HAL_Status HAL_WDT_SetTopMsec(uint32_t topMsec, uint32_t freq);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_WDT_MODULE_ENABLED */
