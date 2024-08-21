/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_IRQ_HANDLER_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup IRQ_HANDLER
 *  @{
 */

#ifndef __HAL_IRQ_HANDLER_H
#define __HAL_IRQ_HANDLER_H

/***************************** MACRO Definition ******************************/
/** @defgroup IRQ_HANDLER_Exported_Definition_Group1 Basic Definition
 *  @{
 */
typedef HAL_Status (*HAL_IRQ_HANDLER)(uint32_t irq, void *args);
#ifdef HAL_GPIO_MODULE_ENABLED
typedef HAL_Status (*HAL_IRQ_GPIO_HANDLER)(eGPIO_bankId bank, uint32_t pin, void *args);
#endif

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup IRQ_HANDLER_Public_Function_Declare Public Function Declare
 *  @{
 */
struct GPIO_IRQ_GROUP_OPS *HAL_IRQ_HANDLER_GetGpioIrqGroupOps(void);
HAL_Status HAL_IRQ_HANDLER_IRQHandler(uint32_t irq);
HAL_Status HAL_IRQ_HANDLER_SetIRQHandler(uint32_t irq,
                                         HAL_IRQ_HANDLER handler, void *args);
#ifdef HAL_GPIO_MODULE_ENABLED
HAL_Status HAL_IRQ_HANDLER_SetGpioIRQHandler(eGPIO_bankId bank,
                                             ePINCTRL_GPIO_PINS pin,
                                             HAL_IRQ_GPIO_HANDLER handler,
                                             void *args);
#endif
/** @} */

#endif

/** @} */

/** @} */

#endif
