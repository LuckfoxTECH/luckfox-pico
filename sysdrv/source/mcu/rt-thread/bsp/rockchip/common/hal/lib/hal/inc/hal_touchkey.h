/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_TOUCHKEY_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup TOUCHKEY
 *  @{
 */

#ifndef _HAL_TOUCHKEY_H_
#define _HAL_TOUCHKEY_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
#define HAL_TOUCHKEY_MAX_CHANNEL 20

/***************************** Structure Definition **************************/

/***************************** Function Declare ******************************/
/** @defgroup TOUCHKEY_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_TouchKey_Init(uint32_t chn_num, struct TOUCH_SENSOR_REG *touchkey);
HAL_Status HAL_TouchKey_DeInit(struct TOUCH_SENSOR_REG *touchkey);
uint32_t HAL_TouchKey_GetIntPos(struct TOUCH_SENSOR_REG *touchkey);
uint32_t HAL_TouchKey_GetIntNeg(struct TOUCH_SENSOR_REG *touchkey);
uint32_t HAL_TouchKey_GetIntRaw(struct TOUCH_SENSOR_REG *touchkey);
void HAL_TouchKey_ClearIntPos(uint32_t irq, struct TOUCH_SENSOR_REG *touchkey);
void HAL_TouchKey_ClearIntNeg(uint32_t irq, struct TOUCH_SENSOR_REG *touchkey);
HAL_Status HAL_TouchKey_HandlePosIrq(struct TOUCH_SENSOR_REG *touchkey);
HAL_Status HAL_TouchKey_HandleNegIrq(struct TOUCH_SENSOR_REG *touchkey);
uint32_t HAL_TouchKey_GetRawCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey);
uint32_t HAL_TouchKey_GetDcCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey);
uint32_t HAL_TouchKey_GetDoCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey);
uint32_t HAL_TouchKey_GetFilterCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_TOUCHKEY_MODULE_ENABLED */
