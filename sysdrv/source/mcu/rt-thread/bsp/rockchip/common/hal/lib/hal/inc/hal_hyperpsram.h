/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_HYPERPSRAM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup HYPERPSRAM
 *  @{
 */

#ifndef _HAL_HYPERBUS_H_
#define _HAL_HYPERBUS_H_

#include "hal_base.h"

/***************************** Structure Definition **************************/
/**
  * @brief  HYPERPSRAM psram chip information definition
  */
struct HYPERPSRAM_CHIP_INFO {
    uint16_t id;
    uint32_t spc;
    uint32_t cap;
};

/**
  * @brief  HYPERPSRAM register saving definition
  */
struct HYPERPSRAM_RESUME_REG {
    uint32_t available;
    struct HYPERBUS_REG hyperbus;
};

/**
  * @brief  HYPERPSRAM information definition
  */
struct HAL_HYPERPSRAM_DEV {
    struct HYPERBUS_REG *pReg;
    struct HYPERPSRAM_CHIP_INFO psramChip;
    struct HYPERPSRAM_RESUME_REG hyperResumeReg;
    eCLOCK_Name clkID; /**< The hyperbus clk id */
    uint32_t aclkGateID; /**< The hyperbus aclk gate id */
    uint32_t sclkGateID; /**< The hyperbus sclk gate id */
    uint32_t hyperMem[2]; /**< The hyperbus psram base address
                            *  which should be initialized by device.
                            */
    uint32_t hyperMaxFreq; /**< The user setting hyperbus max frequency, unit Hz */
    uint32_t psramFreq; /**< The hyperbus psram frequency which
                          *  should initialized by device.
                          */
};

/***************************** Function Declare ******************************/
/** @defgroup HYPERPSRAM_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_HYPERPSRAM_Init(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev);
HAL_Status HAL_HYPERPSRAM_DeInit(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev);
HAL_Status HAL_HYPERPSRAM_ReInit(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev);
HAL_Status HAL_HYPERPSRAM_ModifyTiming(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev);
HAL_Status HAL_HYPERPSRAM_Suspend(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev);
HAL_Status HAL_HYPERPSRAM_Resume(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_HYPERPSRAM_MODULE_ENABLED */
