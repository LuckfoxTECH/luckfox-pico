/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_INTMUX_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup INTMUX
 *  @{
 */

#ifndef __HAL_INTMUX_H
#define __HAL_INTMUX_H

/***************************** MACRO Definition ******************************/
/** @defgroup INTMUX_Exported_Definition_Group1 Basic Definition
 *  @{
 */
typedef HAL_Status (*HAL_INTMUX_HANDLER)(uint32_t irq, void *args);
#ifdef HAL_INTMUX_CUSTOM_DISTRIBUTION_FEATURE_ENABLED
extern uint32_t g_intmuxFastIrqTable[];
#endif

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup INTMUX_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_INTMUX_EnableIRQ(uint32_t irq);
HAL_Status HAL_INTMUX_DisableIRQ(uint32_t irq);
HAL_Status HAL_INTMUX_SetIRQHandler(uint32_t irq, HAL_INTMUX_HANDLER handler, void *args);
HAL_Status HAL_INTMUX_Init(void);
void HAL_INTMUX_DirectDispatch(uint32_t irq);

/** @} */

#endif

/** @} */

/** @} */

#endif
