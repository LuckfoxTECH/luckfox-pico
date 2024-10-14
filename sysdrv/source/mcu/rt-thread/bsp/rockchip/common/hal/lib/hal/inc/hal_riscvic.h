/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_RISCVIC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup RISCVIC
 *  @{
 */

#ifndef _HAL_RISCVIC_H_
#define _HAL_RISCVIC_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup RISCVIC_Exported_Definition_Group1 Basic Definition
 *  @{
 */
/** @brief Type define of RISCVIC trap entry */
typedef void (*RISCVIC_Entry)(void);

/***************************** Structure Definition **************************/

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup RISCVIC_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_RISCVIC_Init(void);
HAL_Status HAL_RISCVIC_IRQHandler(unsigned int iMcause, unsigned int iMepc, unsigned int iSp);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_RISCVIC_MODULE_ENABLED */
