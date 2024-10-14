/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_HWSPINLOCK_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup HWSPINLOCK
 *  @{
 */

#ifndef _HAL_HWSPINLOCK_H_
#define _HAL_HWSPINLOCK_H_

#include "hal_def.h"

/***************************** Function Declare ******************************/
/** @defgroup HWSPINLOCK_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Check HAL_HWSPINLOCK_TryLock(uint32_t lockID);
void HAL_HWSPINLOCK_Unlock(uint32_t lockID);
uint32_t HAL_HWSPINLOCK_GetOwner(uint32_t lockID);
HAL_Check HAL_HWSPINLOCK_Init(uint32_t ownerID);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_HWSPINLOCK_MODULE_ENABLED */
