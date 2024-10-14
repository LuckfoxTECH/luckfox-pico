/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_CKCAL_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CKCAL
 *  @{
 */

/** @defgroup CKCAL_How_To_Use How To Use
 *  @{

 The CKCAL driver can be used as follows:

 # JSHASH(32bits)

 * step1: Begin with HAL_CKCAL_JSHashInit
 * step2: Point to a mount of source data by HAL_CKCAL_JSHashCalc
 * step3: Assume that the transmission is completed by consult to
 *  HAL_CKCAL_CheckComplete then:
   * Continue to calculate: Return to step 2
   * Continue to next step: step 4
 * step4: Get the result by HAL_CKCAL_JSHashGetResult

 @} */

/** @defgroup CKCAL_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup CKCAL_Exported_Functions_Group2 State and Errors Functions
 *  @{
 */

/**
 * @brief  Confirm whether the last operation is completed.
 * @param  dev: CKCAL device.
 * @return HAL_Status
 *     HAL_TRUE : Transmission complete
 *     HAL_FALSE : No transfer or transfering
 */
HAL_Check HAL_CKCAL_CheckComplete(struct HAL_CKCAL_DEV *dev)
{
    struct CKCAL_REG *pReg = dev->pReg;

    HAL_ASSERT(IS_CKCAL_INSTANCE(pReg));

    return (pReg->STATE & CKCAL_STATE_DONE_MASK ? HAL_TRUE : HAL_FALSE);
}

/** @} */

/** @defgroup CKCAL_Exported_Functions_Group3 IO Functions
 *  @{
 */

/**
 * @brief  Start or continue CKCAL JSHash calculation.
 * @param  dev: CKCAL device.
 * @param  buf: source buffer.
 * @param  len: the length of source buffer.
 * @return HAL_Status
 */
HAL_Status HAL_CKCAL_JSHashCalc(struct HAL_CKCAL_DEV *dev, uint8_t *buf, uint32_t len)
{
    struct CKCAL_REG *pReg = dev->pReg;
    HAL_Status ret = HAL_OK;

    HAL_ASSERT(IS_CKCAL_INSTANCE(pReg));

    if (!HAL_IS_ALIGNED((uint32_t)buf, 4U)) {
        ret = HAL_ERROR;
    }

    pReg->BASE_ADDR = (uint32_t)buf;
    pReg->LEN = len;
    pReg->STATE = 1U << CKCAL_STATE_DONE_SHIFT; /* clear status */
    pReg->CTRL = 1U << CKCAL_CTRL_START_SHIFT;

    return ret;
}

/**
 * @brief  Get CKCAL calculation result.
 * @param  dev: CKCAL device.
 * @param  result: Hash result.
 * @return HAL_Status
 */
HAL_Status HAL_CKCAL_JSHashGetResult(struct HAL_CKCAL_DEV *dev, uint32_t *result)
{
    struct CKCAL_REG *pReg = dev->pReg;

    HAL_ASSERT(IS_CKCAL_INSTANCE(pReg));

    pReg->STATE = 1U << CKCAL_STATE_DONE_SHIFT; /* clear status */
    *result = pReg->RESULT;

    return HAL_OK;
}

/** @} */

/** @defgroup CKCAL_Exported_Functions_Group4 Init and DeInit Functions
 *  @{
 */

/**
 * @brief  Initial CKCAL JSHash function.
 * @param  dev: CKCAL device.
 * @param  hash: 32bits hash seed.
 * @return HAL_Status
 */
HAL_Status HAL_CKCAL_JSHashInit(struct HAL_CKCAL_DEV *dev, uint32_t hash)
{
    struct CKCAL_REG *pReg = dev->pReg;

    HAL_ASSERT(IS_CKCAL_INSTANCE(pReg));

    pReg->INI_DATA = hash;
    pReg->SEL = 0;
    pReg->BURST = 2; /* INCR8, non-aligned will be burst single */
    pReg->RESULT_CTRL = 1U << CKCAL_RESULT_CTRL_RESULT_SEL_SHIFT;
    pReg->STATE = (1U << CKCAL_STATE_DONE_SHIFT) | (1U << CKCAL_STATE_ERROR_SHIFT); /* clear status */

    return 0;
}

/** @} */

/** @defgroup CKCAL_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  CKCAL irq handler.
 * @param  dev: CKCAL device.
 * @return HAL_Status
 */
HAL_Status HAL_CKCAL_IrqHelper(struct HAL_CKCAL_DEV *dev)
{
    struct CKCAL_REG *pReg = dev->pReg;

    HAL_ASSERT(IS_CKCAL_INSTANCE(pReg));

    pReg->STATE = 1U << CKCAL_STATE_DONE_SHIFT; /* clear status */

    return HAL_OK;
}

/**
 * @brief  CKCAL irq unmask.
 * @param  dev: CKCAL device.
 * @return HAL_Status
 */
HAL_Status HAL_CKCAL_IntUnmask(struct HAL_CKCAL_DEV *dev)
{
    struct CKCAL_REG *pReg = dev->pReg;

    HAL_ASSERT(IS_CKCAL_INSTANCE(pReg));

    SET_BIT(pReg->INT_EN, 1U << CKCAL_INT_EN_DONE_INT_EN_SHIFT);

    return HAL_OK;
}

/**
 * @brief  CKCAL irq mask.
 * @param  dev: CKCAL device.
 * @return HAL_Status
 */
HAL_Status HAL_CKCAL_IntMask(struct HAL_CKCAL_DEV *dev)
{
    struct CKCAL_REG *pReg = dev->pReg;

    HAL_ASSERT(IS_CKCAL_INSTANCE(pReg));

    CLEAR_BIT(pReg->INT_EN, CKCAL_INT_EN_DONE_INT_EN_MASK);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_CKCAL_MODULE_ENABLED */
