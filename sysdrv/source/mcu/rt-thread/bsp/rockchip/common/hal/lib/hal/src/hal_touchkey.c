/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_TOUCHKEY_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup TOUCHKEY
 *  @{
 */

/** @defgroup TOUCHKEY_How_To_Use How To Use
 *  @{

 The TOUCHKEY driver can be used as follows:

 @} */

/** @defgroup TOUCHKEY_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define TOUCHKEY_CHANNEL_PER_REGISTER 16
#define TOUCHKEY_REG_WRITE_MASK       16
#define TOUCHKEY_INPUT_CLK            40 * 1000 * 1000
#define TOUCHKEY_OUTPUT_CLK           1000
#define TOUCHKEY_FILTER_THRESHOLD     0x8
#define TOUCHKEY_FILTER_STABLE_TIME   0x800
#define TOUCHKEY_NEG_REG_OFFSET       0x2
/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup TOUCHKEY_Exported_Functions_Group4 Init and DeInit

  This section provides functions allowing to init and deinit the module:

  *  @{
  */

/**
 * @brief  TOUCHKEY Init
 * @param  chn_num: touchkey channel number
 * @param  touchkey: touchkey base address
 * @return HAL_Status
 */
HAL_Status HAL_TouchKey_Init(uint32_t chn_num, struct TOUCH_SENSOR_REG *touchkey)
{
    uint32_t reg_idx;
    uint32_t i;
    uint32_t idx;

    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    if (chn_num > HAL_TOUCHKEY_MAX_CHANNEL) {
        chn_num = HAL_TOUCHKEY_MAX_CHANNEL;
    }

    touchkey->CH_CHARGE_THRESHOLD = TOUCHKEY_FILTER_THRESHOLD;
    touchkey->CH_DIV = (TOUCHKEY_INPUT_CLK / TOUCHKEY_OUTPUT_CLK) / 2;
    touchkey->CH_FILTER_STABLE_TIME = 0xffff << TOUCHKEY_REG_WRITE_MASK | TOUCHKEY_FILTER_STABLE_TIME;

    for (i = 0; i < chn_num; i++) {
        reg_idx = i / TOUCHKEY_CHANNEL_PER_REGISTER;
        idx = i % TOUCHKEY_CHANNEL_PER_REGISTER;
        touchkey->CH_ENABLE[reg_idx] |= HAL_BIT(idx) << TOUCHKEY_REG_WRITE_MASK | HAL_BIT(idx);
        touchkey->CH_IRQ_EN[reg_idx] |= HAL_BIT(idx) << TOUCHKEY_REG_WRITE_MASK | HAL_BIT(idx);
        touchkey->CH_IRQ_EN[reg_idx + TOUCHKEY_NEG_REG_OFFSET] |= HAL_BIT(idx) << TOUCHKEY_REG_WRITE_MASK | HAL_BIT(idx);
    }

    touchkey->CH_START = HAL_BIT(0) << TOUCHKEY_REG_WRITE_MASK | HAL_BIT(0);

    return HAL_OK;
}

/**
 * @brief  TOUCHKEY DeInit
 * @param  touchkey: touchkey base address
 * @return HAL_Status
 */
HAL_Status HAL_TouchKey_DeInit(struct TOUCH_SENSOR_REG *touchkey)
{
    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    touchkey->CH_ENABLE[0] = 0xffff0000;
    touchkey->CH_ENABLE[1] = 0xffff0000;
    touchkey->CH_IRQ_EN[0] = 0xffff0000;
    touchkey->CH_IRQ_EN[1] = 0xffff0000;
    touchkey->CH_IRQ_EN[2] = 0xffff0000;
    touchkey->CH_IRQ_EN[3] = 0xffff0000;
    touchkey->CH_DIV = 0xfffff;
    touchkey->CH_CHARGE_THRESHOLD = 0xfffff;
    touchkey->CH_START = 0xffff0000;

    return HAL_OK;
}

/** @} */

/** @defgroup TOUCHKEY_Exported_Functions_Group5 Other Functions

  This section provides functions allowing to other controlling:

  *  @{
  */

/**
 * @brief  TouchKey get interrupt pos status
 * @param  touchkey: touchkey base address
 * @return TouchKey pos interrupt status
 */
uint32_t HAL_TouchKey_GetIntPos(struct TOUCH_SENSOR_REG *touchkey)
{
    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    return touchkey->CH_IRQ_ST_POS;
}

/**
 * @brief  TouchKey get interrupt neg status
 * @param  touchkey: touchkey base address
 * @return TouchKey neg interrupt status
 */
uint32_t HAL_TouchKey_GetIntNeg(struct TOUCH_SENSOR_REG *touchkey)
{
    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    return touchkey->CH_IRQ_ST_NEG;
}

/**
 * @brief  TouchKey get interrupt raw status
 * @return TouchKey raw interrupt status
 * @param  touchkey: touchkey base address
 */
uint32_t HAL_TouchKey_GetIntRaw(struct TOUCH_SENSOR_REG *touchkey)
{
    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    return touchkey->CH_IRQ_RAW;
}

/**
 * @brief  TouchKey clear pos interrupt
 * @param  irq: irq source to clear
 * @param  touchkey: touchkey base address
 */
void HAL_TouchKey_ClearIntPos(uint32_t irq, struct TOUCH_SENSOR_REG *touchkey)
{
    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    touchkey->CH_IRQ_CLEAR[0] = irq;
}

/**
 * @brief  TouchKey clear neg interrupt
 * @param  irq: irq source to clear
 * @param  touchkey: touchkey base address
 */
void HAL_TouchKey_ClearIntNeg(uint32_t irq, struct TOUCH_SENSOR_REG *touchkey)
{
    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    touchkey->CH_IRQ_CLEAR[1] = irq;
}

/**
 * @brief  TouchKey get raw counter
 * @param  idx: which channel
 * @param  touchkey: touchkey base address
 * @return TouchKey raw counter
 */
uint32_t HAL_TouchKey_GetRawCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey)
{
    uint32_t count = 0;

    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    switch (idx) {
    case 0:
        count = touchkey->CH0_CNT;
        break;
    case 1:
        count = touchkey->CH1_CNT;
        break;
    case 2:
        count = touchkey->CH2_CNT;
        break;
    case 3:
        count = touchkey->CH3_CNT;
        break;
    case 4:
        count = touchkey->CH4_CNT;
        break;
    default:
        HAL_DBG("%s, %d, error idx\n", __func__, __LINE__);
    }

    return count;
}

/**
 * @brief  TouchKey get counter after DC
 * @param  idx: which channel
 * @param  touchkey: touchkey base address
 * @return TouchKey counter after DC
 */
uint32_t HAL_TouchKey_GetDcCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey)
{
    uint32_t count = 0;

    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    switch (idx) {
    case 0:
        count = touchkey->CH0_CNT_DC;
        break;
    case 1:
        count = touchkey->CH1_CNT_DC;
        break;
    case 2:
        count = touchkey->CH2_CNT_DC;
        break;
    case 3:
        count = touchkey->CH3_CNT_DC;
        break;
    case 4:
        count = touchkey->CH4_CNT_DC;
        break;
    default:
        HAL_DBG("%s, %d, error idx\n", __func__, __LINE__);
    }

    return count;
}

/**
 * @brief  TouchKey get counter after DO
 * @param  idx: which channel
 * @param  touchkey: touchkey base address
 * @return TouchKey counter after DO
 */
uint32_t HAL_TouchKey_GetDoCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey)
{
    uint32_t count = 0;

    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    switch (idx) {
    case 0:
        count = touchkey->CH0_CNT_DO;
        break;
    case 1:
        count = touchkey->CH1_CNT_DO;
        break;
    case 2:
        count = touchkey->CH2_CNT_DO;
        break;
    case 3:
        count = touchkey->CH3_CNT_DO;
        break;
    case 4:
        count = touchkey->CH4_CNT_DO;
        break;
    default:
        HAL_DBG("%s, %d, error idx\n", __func__, __LINE__);
    }

    return count;
}

/**
 * @brief  TouchKey get counter after filter
 * @param  idx: which channel
 * @param  touchkey: touchkey base address
 * @return TouchKey counter after filter
 */
uint32_t HAL_TouchKey_GetFilterCount(uint32_t idx, struct TOUCH_SENSOR_REG *touchkey)
{
    uint32_t count = 0;

    HAL_ASSERT(IS_TOUCH_SENSOR_INSTANCE(touchkey));

    switch (idx) {
    case 0:
        count = touchkey->CH0_CNT_FILTER;
        break;
    case 1:
        count = touchkey->CH1_CNT_FILTER;
        break;
    case 2:
        count = touchkey->CH2_CNT_FILTER;
        break;
    case 3:
        count = touchkey->CH3_CNT_FILTER;
        break;
    case 4:
        count = touchkey->CH4_CNT_FILTER;
        break;
    default:
        HAL_DBG("%s, %d, error idx\n", __func__, __LINE__);
    }

    return count;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_TOUCHKEY_MODULE_ENABLED */
