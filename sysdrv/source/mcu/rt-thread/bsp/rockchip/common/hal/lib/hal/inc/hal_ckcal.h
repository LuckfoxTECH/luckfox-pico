/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_CKCAL_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CKCAL
 *  @{
 */

#ifndef _HAL_CKCAL_H_
#define _HAL_CKCAL_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup CKCAL_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

struct HAL_CKCAL_DEV {
    struct CKCAL_REG *pReg;
    IRQn_Type irqNum;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup CKCAL_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Check HAL_CKCAL_CheckComplete(struct HAL_CKCAL_DEV *dev);
HAL_Status HAL_CKCAL_JSHashCalc(struct HAL_CKCAL_DEV *dev, uint8_t *buf, uint32_t len);
HAL_Status HAL_CKCAL_JSHashGetResult(struct HAL_CKCAL_DEV *dev, uint32_t *result);
HAL_Status HAL_CKCAL_JSHashInit(struct HAL_CKCAL_DEV *dev, uint32_t hash);
HAL_Status HAL_CKCAL_IrqHelper(struct HAL_CKCAL_DEV *dev);
HAL_Status HAL_CKCAL_IntUnmask(struct HAL_CKCAL_DEV *dev);
HAL_Status HAL_CKCAL_IntMask(struct HAL_CKCAL_DEV *dev);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_CKCAL_MODULE_ENABLED */
