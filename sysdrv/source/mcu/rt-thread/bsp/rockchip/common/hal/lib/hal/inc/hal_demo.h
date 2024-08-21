/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_DEMO_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup DEMO
 *  @{
 */

#ifndef _HAL_DEMO_H_
#define _HAL_DEMO_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup DEMO_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** Add brief to here */
#define DEMO_TEST_DEFINE 0

/***************************** Structure Definition **************************/

/**
 *  Add multi line brief to here
 *  ...
 */
typedef enum {
    TEST_ENUM_ONE,
    TEST_ENUM_TWO,
    TEST_ENUM_THREE
} eDEMO_Test;

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup DEMO_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_DEMO_Supsend(uint32_t inputTest);
HAL_Status HAL_DEMO_Resume(uint32_t inputTest);
HAL_Status HAL_DEMO_GetXXXState(uint32_t inputTest);
HAL_Status HAL_DEMO_WriteByte_DMA(uint32_t inputTest);
HAL_Status HAL_DEMO_Init(uint32_t inputTest);
HAL_Status HAL_DEMO_DeInit(uint32_t inputTest);
HAL_Status HAL_DEMO_CommonFunction(uint32_t inputTest);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_DEMO_MODULE_ENABLED */
