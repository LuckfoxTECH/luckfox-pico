/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_SARADC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SARADC
 *  @{
 */

/** @defgroup SARADC_How_To_Use How To Use
 *  @{

 The SARADC driver can be used as follows:

 1. IRQ mode
    1. HAL_SARADC_Start()
    2. Wait adc convert done: HAL_SARADC_IrqHandler()
    3. HAL_SARADC_GetRaw()
    4. HAL_SARADC_Stop()

 2. POLL mode
    1. Enable poll object
    ```
    if (saradc v2) {
           HAL_SARADC_SetHighThreshold()
           HAL_SARADC_EnableHighThresholdIrq()
    }
    ```
    2. HAL_SARADC_Start()
    3. Poll adc convert done:HAL_SARADC_PollIsEOC()
    4. HAL_SARADC_GetRaw()
    5. clear highthreshold irq if saradc v2
    ```
    if (saradc v2)
           HAL_SARADC_ClearHighThresholdIrq()
    ```
    6. HAL_SARADC_Stop()

 The purpose of HighThreshold is to indicate a channel if convert done when
 in poll mode.

 @attention
 KEY NOTE: Do not start new convert until previous done!

 @} */

/** @defgroup SARADC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup SARADC_Exported_Functions_Group5 Other Functions

 This section provides functions allowing to other controlling:

 *  @{
 */

/**
 * @brief  Start adc, don't start new convert until previous done
 * @param  reg: saradc base address
 * @param  mode: saradc mode
 * @param  chn: saradc channel
 * @return hal status
 */
HAL_Status HAL_SARADC_Start(struct SARADC_REG *reg, eSARADC_mode mode, uint32_t chn)
{
    uint32_t value;

    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifdef SARADC_CONV_CON_OFFSET
    HAL_ASSERT(chn < HAL_ARRAY_SIZE(reg->DATA));

    /* saradc v2 */
    reg->T_DAS_SOC = 0xc;
    reg->T_PD_SOC = 0x20;

    WRITE_REG_MASK_WE(reg->END_INT_EN, SARADC_END_INT_EN_END_INT_EN_MASK,
                      SARADC_END_INT_EN_END_INT_EN_MASK);
    value = SARADC_CONV_CON_START_ADC_MASK | SARADC_CONV_CON_SINGLE_PD_MODE_MASK | chn;
    WRITE_REG_MASK_WE(reg->CONV_CON, SARADC_CONV_CON_START_ADC_MASK |
                      SARADC_CONV_CON_SINGLE_PD_MODE_MASK |
                      SARADC_CONV_CON_CHANNEL_SEL_MASK,
                      value);
#else
    reg->DLY_PU_SOC = 0x8;

    value = chn & SARADC_CTRL_ADC_INPUT_SRC_SEL_MASK;

    if (mode == SARADC_INT_MOD) {
        value |= SARADC_CTRL_INT_EN_MASK;
    }

    value |= SARADC_CTRL_ADC_POWER_CTRL_MASK;

    reg->CTRL = value;
#endif

    return HAL_OK;
}

/**
 * @brief  Stop adc
 * @param  reg: saradc base address
 * @return hal status
 */
HAL_Status HAL_SARADC_Stop(struct SARADC_REG *reg)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifndef SARADC_CONV_CON_OFFSET
    /* Clear irq & power down adc */
    reg->CTRL = 0x0;
#endif

    return HAL_OK;
}

/**
 * @brief  clear irq
 * @param  reg: saradc base address
 */
void HAL_SARADC_ClearIrq(struct SARADC_REG *reg)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifdef SARADC_CONV_CON_OFFSET
    reg->END_INT_ST = 0x1;
#else
    /* Clear irq */
    MODIFY_REG(reg->CTRL, SARADC_CTRL_INT_STATUS_MASK, 0);
#endif
}

/**
 * @brief  set high threshold value
 * @param  reg: saradc base address
 * @param  chn: saradc channel
  *@param  val: high threshold value
 */
void HAL_SARADC_SetHighThreshold(struct SARADC_REG *reg, uint32_t chn, uint32_t val)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifdef SARADC_CONV_CON_OFFSET
    HAL_ASSERT(chn < HAL_ARRAY_SIZE(reg->DATA));

    reg->HIGH_COMP[chn] = val;
#endif
}

/**
 * @brief  enable high threshold irq, high threshold irq will triggered if adc value higher than HIGH_COMP[chn]
 * @param  reg: saradc base address
 * @param  chn: saradc channel
 */
void HAL_SARADC_EnableHighThresholdIrq(struct SARADC_REG *reg, uint32_t chn)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifdef SARADC_CONV_CON_OFFSET
    HAL_ASSERT(chn < HAL_ARRAY_SIZE(reg->DATA));

    WRITE_REG_MASK_WE(reg->HT_INT_EN, HAL_BIT(chn), HAL_BIT(chn));
#endif
}

/**
 * @brief  clear high threshold irq
 * @param  reg: saradc base address
 * @param  chn: saradc channel
 */
void HAL_SARADC_ClearHighThresholdIrq(struct SARADC_REG *reg, uint32_t chn)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifdef SARADC_CONV_CON_OFFSET
    HAL_ASSERT(chn < HAL_ARRAY_SIZE(reg->DATA));

    reg->HT_INT_ST = HAL_BIT(chn);
#endif
}

/**
 * @brief  Get adc data
 * @param  reg: saradc base address
 * @param  chn: saradc channel
 * @return adc value
 */
uint32_t HAL_SARADC_GetRaw(struct SARADC_REG *reg, uint32_t chn)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifdef SARADC_CONV_CON_OFFSET
    HAL_ASSERT(chn < HAL_ARRAY_SIZE(reg->DATA));

    reg->END_INT_ST = 0x1;

    return reg->DATA[chn];
#else

    return reg->DATA & SARADC_DATA_ADC_DATA_MASK;
#endif
}

/**
 * @brief  Poll adc status
 * @param  reg: saradc base address
 * @param  chn: saradc channel
 * @return HAL_TRUE if end of conversation else HAL_FALSE
 */
HAL_Check HAL_SARADC_PollIsEOC(struct SARADC_REG *reg, uint32_t chn)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

#ifdef SARADC_CONV_CON_OFFSET
    HAL_ASSERT(chn < HAL_ARRAY_SIZE(reg->DATA));

    return reg->HT_INT_ST & HAL_BIT(chn) ? HAL_TRUE : HAL_FALSE;
#else

    return reg->STAS & SARADC_STAS_ADC_STATUS_MASK ? HAL_FALSE : HAL_TRUE;
#endif
}

/**
 * @brief  Common saradc interrupt handler
 * Common interrupt handler is always wrapped by the driver.
 */
void HAL_SARADC_IrqHandler(struct SARADC_REG *reg)
{
    HAL_ASSERT(IS_SARADC_INSTANCE(reg));

    /* Clear irq */
#ifdef SARADC_CONV_CON_OFFSET
    reg->END_INT_ST = 0x1;
#else
    MODIFY_REG(reg->CTRL, SARADC_CTRL_INT_STATUS_MASK, 0);
#endif
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_SARADC_MODULE_ENABLED */
