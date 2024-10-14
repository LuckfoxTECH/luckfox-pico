/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_SARADC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SARADC
 *  @{
 */

#ifndef _HAL_SARADC_H_
#define _HAL_SARADC_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup SARADC_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/
typedef enum {
    SARADC_INT_MOD,
    SARADC_POL_MOD,
} eSARADC_mode;

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup SARADC_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_SARADC_Start(struct SARADC_REG *reg, eSARADC_mode mode, uint32_t chn);
HAL_Status HAL_SARADC_Stop(struct SARADC_REG *reg);
uint32_t HAL_SARADC_GetRaw(struct SARADC_REG *reg, uint32_t chn);
HAL_Check HAL_SARADC_PollIsEOC(struct SARADC_REG *reg, uint32_t chn);
void HAL_SARADC_IrqHandler(struct SARADC_REG *reg);
void HAL_SARADC_ClearIrq(struct SARADC_REG *reg);
void HAL_SARADC_SetHighThreshold(struct SARADC_REG *reg, uint32_t chn, uint32_t val);
void HAL_SARADC_EnableHighThresholdIrq(struct SARADC_REG *reg, uint32_t chn);
void HAL_SARADC_ClearHighThresholdIrq(struct SARADC_REG *reg, uint32_t chn);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_SARADC_MODULE_ENABLED */
