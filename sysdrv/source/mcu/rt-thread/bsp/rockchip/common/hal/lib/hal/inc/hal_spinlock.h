/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_SPINLOCK_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SPINLOCK
 *  @{
 */

#ifndef _HAL_SPINLOCK_H_
#define _HAL_SPINLOCK_H_

#include "hal_base.h"
#include "hal_def.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/

/********************* Public Function Definition ****************************/
/** @defgroup SPINLOCK_Exported_Functions_Group5 Other Functions
 *  @{
 */

HAL_Status HAL_SPINLOCK_Init(uint32_t ownerID);
HAL_Check HAL_SPINLOCK_TryLock(uint32_t lockID);
void HAL_SPINLOCK_Lock(uint32_t lockID);
void HAL_SPINLOCK_Unlock(uint32_t lockID);
uint32_t HAL_SPINLOCK_GetOwner(uint32_t lockID);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_SPINLOCK_MODULE_ENABLED */
