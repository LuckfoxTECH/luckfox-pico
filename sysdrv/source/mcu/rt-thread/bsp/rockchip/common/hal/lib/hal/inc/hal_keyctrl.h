/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_KEYCTRL_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup KEYCTRL
 *  @{
 */

#ifndef _HAL_KEYCTRL_H_
#define _HAL_KEYCTRL_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/

/***************************** Function Declare ******************************/
/** @defgroup KEYCTRL_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_KeyCtrl_Init(struct KEY_CTRL_REG *reg, uint32_t keyDetectionTh, uint32_t keyCalculatePeriodTh, uint32_t keyFilterIrqTh);
HAL_Status HAL_KeyCtrl_DeInit(struct KEY_CTRL_REG *reg);
void HAL_KeyCtrl_ClearInt(struct KEY_CTRL_REG *reg);
uint32_t HAL_KeyCtrl_GetValue(struct KEY_CTRL_REG *reg);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_KEYCTRL_MODULE_ENABLED */
