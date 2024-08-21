/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */
#include "hal_conf.h"

#ifdef HAL_EFUSE_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup EFUSE
 *  @{
 */

#ifndef _HAL_EFUSE_H_
#define _HAL_EFUSE_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/

/***************************** Function Declare ******************************/
/** @defgroup EFUSE_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_EFUSE_ReadByte(struct EFUSE_CTL_REG *reg, uint32_t offset, uint8_t *val);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_EFUSE_MODULE_ENABLED */
