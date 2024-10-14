/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_TSADC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup TSADC
 *  @{
 */

/** @defgroup TSADC_How_To_Use How To Use
 *  @{

 The TSADC driver can be used as follows:

 - Invoke tsadc functions to enable tsadc and get temperature in each device.
 - More details refer to APIs' descriptions as below.

 @} */

/** @defgroup TSADC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define TSADC_USER_SRC_EN(chn)       HAL_BIT(chn)
#define TSADC_AUTO_SRC_EN(chn)       HAL_BIT(4 + (chn))
#define TSADC_INT_SRC_EN(chn)        HAL_BIT(chn)
#define TSADC_SHUT_2GPIO_SRC_EN(chn) HAL_BIT(4 + (chn))
#define TSADC_SHUT_2CRU_SRC_EN(chn)  HAL_BIT(8 + (chn))

#define TSADC_DATA_MASK 0xfff

#define TSADC_HIGHT_INT_DEBOUNCE_COUNT   4
#define TSADC_HIGHT_TSHUT_DEBOUNCE_COUNT 4
#define TSADC_AUTO_PERIOD_TIME           250
#define TSADC_AUTO_PERIOD_HT_TIME        50
#define TSADC_TSHUT_TEMP                 (120000)

#define TSADCV2_USER_INTER_PD_SOC   0x8fc0 /* 97us, at least 90us */
#define TSADCV2_AUTO_PERIOD_TIME    1622 /* 2.5ms */
#define TSADCV2_AUTO_PERIOD_HT_TIME 1622 /* 2.5ms */

/* -40 to 125 is reliable, outside the range existed unreliability */
#define MIN_TEMP (-60000)
#define MAX_TEMP (180000)

/********************* Private Structure Definition **************************/

struct TSADC_TABLE {
    uint32_t code;
    int temp;
};

struct TSADC_CONFIG {
    const struct TSADC_TABLE *table;
    int length;
    uint32_t dataMask;
};

/********************* Private Variable Definition ***************************/

#if defined(RKMCU_RK2206)
static const struct TSADC_TABLE s_tsadcTable[] =
{
    { 3426, MIN_TEMP },
    { 3461, -40000 },
    { 3751, 125000 },
    { 3848, MAX_TEMP },
};
#elif defined(SOC_RK3568)
static const struct TSADC_TABLE s_tsadcTable[] =
{
    { 1448, MIN_TEMP },
    { 1584, -40000 },
    { 2704, 125000 },
    { 3076, MAX_TEMP },
};
#elif defined(SOC_RK3308) || defined(SOC_RK3358)
static const struct TSADC_TABLE s_tsadcTable[] =
{
    { 0, MIN_TEMP },
    { 261, MIN_TEMP },
    { 296, -40000 },
    { 304, -35000 },
    { 313, -30000 },
    { 331, -20000 },
    { 340, -15000 },
    { 349, -10000 },
    { 359, -5000 },
    { 368, 0 },
    { 378, 5000 },
    { 388, 10000 },
    { 398, 15000 },
    { 408, 20000 },
    { 418, 25000 },
    { 429, 30000 },
    { 440, 35000 },
    { 451, 40000 },
    { 462, 45000 },
    { 473, 50000 },
    { 485, 55000 },
    { 496, 60000 },
    { 508, 65000 },
    { 521, 70000 },
    { 533, 75000 },
    { 546, 80000 },
    { 559, 85000 },
    { 572, 90000 },
    { 586, 95000 },
    { 600, 100000 },
    { 614, 105000 },
    { 629, 110000 },
    { 644, 115000 },
    { 659, 120000 },
    { 675, 125000 },
    { 745, 145000 },
    { 825, 165000 },
    { 900, MAX_TEMP },
    { 0xfff, MAX_TEMP },
};
#endif

static const struct TSADC_CONFIG s_tsadcConfig =
{
    .table = s_tsadcTable,
    .length = HAL_ARRAY_SIZE(s_tsadcTable),
    .dataMask = TSADC_DATA_MASK,
};

#ifndef TSADC_NONLINEAR
static int kNum = 0;
static int bNum = 0;
#endif

/********************* Private Function Definition ***************************/

#ifdef TSADC_SORT_MODE_DECREMENT

/**
 * @brief tsadc code to temperature.
 * @param config: struct TSADC_CONFIG, Contains tsadc information
 * @param code: tsadc data value.
 * @return temperature(C) * 1000 .
 */
static int TSADC_CodeToTemp(const struct TSADC_CONFIG *config, uint32_t code)
{
    uint32_t low = 0;
    uint32_t high = config->length - 1;
    int temp;

    code &= config->dataMask;
    if (code > config->table[low].code) {
        return config->table[low].temp;
    } else if (code < config->table[high].code) {
        return config->table[high].temp;
    }

#ifdef TSADC_NONLINEAR
    int num, denom;
    uint32_t mid = (low + high) / 2;

    while (low <= high) {
        HAL_ASSERT(mid >= 1 && mid <= (config->length - 1));
        if (code >= config->table[mid].code && code < config->table[mid - 1].code) {
            break;
        } else if (code < config->table[mid].code) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
        mid = (low + high) / 2;
    }
    /**
    * The 5C granularity provided by the table is too much. Let's
    * assume that the relationship between sensor readings and
    * temperature between 2 table entries is linear and interpolate
    * to produce less granular result.
    */
    num = config->table[mid].temp - config->table[mid - 1].temp;
    num *= config->table[mid - 1].code - code;
    denom = config->table[mid - 1].code - config->table[mid].code;
    temp = config->table[mid - 1].temp + (num / denom);
#else
    temp = kNum * code + bNum;
#endif

    return temp;
}
#else

/**
 * @brief tsadc code to temperature.
 * @param config: struct TSADC_CONFIG, Contains tsadc information
 * @param code: tsadc data value.
 * @return temperature(C) * 1000 .
 */
static int TSADC_CodeToTemp(const struct TSADC_CONFIG *config, uint32_t code)
{
    uint32_t low = 0;
    uint32_t high = config->length - 1;
    int temp;

    code &= config->dataMask;
    if (code > config->table[high].code) {
        return config->table[high].temp;
    } else if (code < config->table[low].code) {
        return config->table[low].temp;
    }

#ifdef TSADC_NONLINEAR
    int num, denom;
    uint32_t mid = (low + high) / 2;

    while (low <= high) {
        HAL_ASSERT(mid >= 1 && mid <= (config->length - 1));
        if (code <= config->table[mid].code && code > config->table[mid - 1].code) {
            break;
        } else if (code > config->table[mid].code) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
        mid = (low + high) / 2;
    }
    /**
    * The 5C granularity provided by the table is too much. Let's
    * assume that the relationship between sensor readings and
    * temperature between 2 table entries is linear and interpolate
    * to produce less granular result.
    */
    num = config->table[mid].temp - config->table[mid - 1].temp;
    num *= code - config->table[mid - 1].code;
    denom = config->table[mid].code - config->table[mid - 1].code;
    temp = config->table[mid - 1].temp + (num / denom);
#else
    temp = kNum * code + bNum;
#endif

    return temp;
}

#endif

/**
 * @brief tsadc temperature to code.
 * @param config: struct TSADC_CONFIG, Contains tsadc information
 * @param temp: temperature.
 * @return code .
 */
static uint32_t TSADC_TempToCode(const struct TSADC_CONFIG *config, int temp)
{
    uint32_t high, low;
    uint32_t error = config->dataMask;

    low = 0;
    high = config->length - 1;

    /* Return mask code data when the temp is over table range */
    if (temp < config->table[low].temp || temp > config->table[high].temp) {
        return error;
    }

#ifdef TSADC_NONLINEAR
    uint32_t mid = (low + high) / 2;

    while (low <= high) {
        HAL_ASSERT(mid >= 0 && mid <= (config->length - 1));
        if (temp == config->table[mid].temp) {
            return config->table[mid].code;
        } else if (temp < config->table[mid].temp) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
        mid = (low + high) / 2;
    }

    return error;
#else
    int code;

    HAL_ASSERT(kNum != 0);
    code = (temp - bNum) / kNum;
    if (code > 0) {
        error = code;
    }

    return error;
#endif
}

/**
 * @brief tsadc get temperature.
 * @param config: struct TSADC_CONFIG, Contains tsadc information
 * @param chn: tsadc chn.
 * @return temperature .
 */
static int TSADC_GetTemp(const struct TSADC_CONFIG *config, int chn)
{
    uint32_t val;

    val = TSADC->DATA[chn];

    return TSADC_CodeToTemp(config, val);
}

/**
 * @brief tsadc config.
 * @param tshut_polarity: tsadc tshut pin polarity
 */
#if defined(RKMCU_RK2206) || defined(SOC_RK3308)
static void TSADC_Config(eTSADC_tshutPolarity polarity)
{
    /* set tshut_prolarity 0: Low active , 1: High active */
#ifdef TSADC_AUTO_CON_TSHUT_PROLARITY_MASK
    CLEAR_BIT(TSADC->AUTO_CON, TSADC_AUTO_CON_TSHUT_PROLARITY_MASK);
    SET_BIT(TSADC->AUTO_CON, (polarity << TSADC_AUTO_CON_TSHUT_PROLARITY_SHIFT));
#endif

    TSADC->AUTO_PERIOD = TSADC_AUTO_PERIOD_TIME;
    TSADC->HIGHT_INT_DEBOUNCE = TSADC_HIGHT_INT_DEBOUNCE_COUNT;
    TSADC->AUTO_PERIOD_HT = TSADC_AUTO_PERIOD_HT_TIME;
    TSADC->HIGHT_TSHUT_DEBOUNCE = TSADC_HIGHT_TSHUT_DEBOUNCE_COUNT;
}
#elif defined(SOC_RK3568)
static void TSADC_Config(eTSADC_tshutPolarity polarity)
{
#ifdef TSADC_AUTO_CON_TSHUT_PROLARITY_MASK
    CLEAR_BIT(TSADC->AUTO_CON, TSADC_AUTO_CON_TSHUT_PROLARITY_MASK);
    SET_BIT(TSADC->AUTO_CON, (polarity << TSADC_AUTO_CON_TSHUT_PROLARITY_SHIFT));
#endif

    TSADC->USER_CON = TSADCV2_USER_INTER_PD_SOC;
    TSADC->AUTO_PERIOD = TSADCV2_AUTO_PERIOD_TIME;
    TSADC->HIGHT_INT_DEBOUNCE = TSADC_HIGHT_INT_DEBOUNCE_COUNT;
    TSADC->AUTO_PERIOD_HT = TSADCV2_AUTO_PERIOD_HT_TIME;
    TSADC->HIGHT_TSHUT_DEBOUNCE = TSADC_HIGHT_TSHUT_DEBOUNCE_COUNT;

    GRF->TSADC_CON = GRF->TSADC_CON | (0x10001 << 8);
    HAL_DelayUs(15);
    GRF->TSADC_CON = GRF->TSADC_CON | (0x10001 << 0);
    GRF->TSADC_CON = GRF->TSADC_CON | (0x10001 << 1);
    GRF->TSADC_CON = GRF->TSADC_CON | (0x10001 << 2);
    HAL_DelayUs(200);
}
#elif defined(SOC_RK3358)
static void TSADC_Config(eTSADC_tshutPolarity polarity)
{
    /* set tshut_prolarity 0: Low active , 1: High active */
#ifdef TSADC_AUTO_CON_TSHUT_PROLARITY_MASK
    CLEAR_BIT(TSADC->AUTO_CON, TSADC_AUTO_CON_TSHUT_PROLARITY_MASK);
    SET_BIT(TSADC->AUTO_CON, (polarity << TSADC_AUTO_CON_TSHUT_PROLARITY_SHIFT));
#endif

    TSADC->AUTO_PERIOD = TSADC_AUTO_PERIOD_TIME;
    TSADC->HIGHT_INT_DEBOUNCE = TSADC_HIGHT_INT_DEBOUNCE_COUNT;
    TSADC->AUTO_PERIOD_HT = TSADC_AUTO_PERIOD_HT_TIME;
    TSADC->HIGHT_TSHUT_DEBOUNCE = TSADC_HIGHT_TSHUT_DEBOUNCE_COUNT;

    GRF->SOC_CON[2] = 0x10001 << 1;
    HAL_DelayUs(15);
}
#endif

/**
 * @brief tsadc enable auto mode.
 */
static void TSADC_EnAuto(void)
{
    /* enable tsadc pd */
#ifdef GRF_SOC_CON30_OFFSET
    WRITE_REG_MASK_WE(GRF->SOC_CON30, GRF_SOC_CON30_GRF_TSADC_TSEN_PD_MASK, 0 << GRF_SOC_CON30_GRF_TSADC_TSEN_PD_SHIFT);
#endif

    /* t_pd timing parameter */
    HAL_DelayUs(10);

    /* enable tsadc calibration */
#ifdef GRF_SOC_CON31_OFFSET
    WRITE_REG_MASK_WE(GRF->SOC_CON31, GRF_SOC_CON31_GRF_TSADC_ANA_REG2_MASK, 1 << GRF_SOC_CON31_GRF_TSADC_ANA_REG2_SHIFT);
#endif

#ifdef TSADC_AUTO_CON_AUTO_EN_MASK
    /* enable tsadc auto mode */
    SET_BIT(TSADC->AUTO_CON, TSADC_AUTO_CON_AUTO_EN_MASK);
#endif

#ifdef TSADC_AUTO_CON_TSADC_Q_SEL_MASK
    /* set temperature coefficient positive or negative */
    SET_BIT(TSADC->AUTO_CON, TSADC_AUTO_CON_TSADC_Q_SEL_MASK);
#endif
}

/**
 * @brief tsadc disable auto mode.
 */
static void TSADC_DisAuto(void)
{
#ifdef TSADC_AUTO_CON_AUTO_EN_MASK
    /* disable tsadc auto mode */
    CLEAR_BIT(TSADC->AUTO_CON, TSADC_AUTO_CON_AUTO_EN_MASK);
#endif

    /* disable tsadc pd */
#ifdef GRF_SOC_CON30_OFFSET
    WRITE_REG_MASK_WE(GRF->SOC_CON30, GRF_SOC_CON30_GRF_TSADC_TSEN_PD_MASK, 1 << GRF_SOC_CON30_GRF_TSADC_TSEN_PD_SHIFT);
#endif
}

/**
 * @brief tsadc eoc ack.
 */
static void TSADC_IrqAck(void)
{
#ifdef TSADC_INT_PD_EOC_INT_PD_MASK
    CLEAR_BIT(TSADC->INT_PD, TSADC_INT_PD_EOC_INT_PD_MASK);
#endif
}

/**
 * @brief tsadc tshut temperature setting and enable tshut.
 * @param config: struct TSADC_CONFIG struct, Contains tsadc information
 * @param chn: tsadc chn.
 * @param temp: tshut temperature
 */
static void TSADC_TshutTemp(const struct TSADC_CONFIG *config, int chn, int temp)
{
    TSADC->COMP_SHUT[chn] = TSADC_TempToCode(config, temp);
    SET_BIT(TSADC->AUTO_CON, TSADC_AUTO_SRC_EN(chn));
}

/**
 * @brief tsadc tshut mode setting.
 * @param chn: tsadc chn.
 * @param mode: tshut mode
 */
static void TSADC_TshutMode(int chn, eTSADC_tshutMode mode)
{
    if (mode == TSHUT_MODE_GPIO) {
        CLEAR_BIT(TSADC->INT_EN, TSADC_SHUT_2CRU_SRC_EN(chn));
        SET_BIT(TSADC->INT_EN, TSADC_SHUT_2GPIO_SRC_EN(chn));
    } else {
        CLEAR_BIT(TSADC->INT_EN, TSADC_SHUT_2GPIO_SRC_EN(chn));
        SET_BIT(TSADC->INT_EN, TSADC_SHUT_2CRU_SRC_EN(chn));
    }
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup TSADC_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief enable tsadc auto mode.
 * @param chn: tsadc chn
 * @param polarity: tshut pin polarity
 * @param mode: tshut mode
 * @return HAL_Status
 */
HAL_Status HAL_TSADC_Enable_AUTO(int chn, eTSADC_tshutPolarity polarity, eTSADC_tshutMode mode)
{
#ifndef TSADC_NONLINEAR
    int deltaTemp, deltaCode;

    deltaTemp = s_tsadcTable[1].temp - s_tsadcTable[0].temp;
    deltaCode = s_tsadcTable[1].code - s_tsadcTable[0].code;
    kNum = HAL_DIV_ROUND_UP(deltaTemp, deltaCode);
    bNum = s_tsadcTable[0].temp - (kNum * s_tsadcTable[0].code);
    HAL_ASSERT(kNum != 0);
#endif

    HAL_ASSERT(polarity <= TSHUT_HIGH_ACTIVE);
    HAL_ASSERT(mode <= TSHUT_MODE_GPIO);

    TSADC_Config(polarity);
    TSADC_IrqAck();
    TSADC_TshutTemp(&s_tsadcConfig, chn, TSADC_TSHUT_TEMP);
    TSADC_TshutMode(chn, mode);
    TSADC_EnAuto();
    HAL_DelayMs(1);

    return HAL_OK;
}

/**
 * @brief Check if a channel of tsadc is enabled with auto mode.
 * @param chn: tsadc chn
 * @return HAL_TRUE: tsadc enabled
 * @return HAL_FALSE: tsadc disabled
 */
HAL_Check HAL_TSADC_IsEnabled_AUTO(int chn)
{
    int val;

    val = TSADC->AUTO_CON;
    val &= TSADC_AUTO_SRC_EN(chn);

    return val ? HAL_TRUE : HAL_FALSE;
}

/**
 * @brief disable tsadc auto mode.
 * @param chn: tsadc chn
 * @return HAL_Status
 */
HAL_Status HAL_TSADC_Disable_AUTO(int chn)
{
    TSADC_DisAuto();

    return HAL_OK;
}

/**
 * @brief tsadc get temperature by auto.
 * @param chn: tsadc chn
 * @return temperature(C)
 */
int HAL_TSADC_GetTemperature_AUTO(int chn)
{
    int temp = 0;

    temp = TSADC_GetTemp(&s_tsadcConfig, chn);

    return HAL_DIV_ROUND_UP(temp, 1000);
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_TSADC_MODULE_ENABLED */
